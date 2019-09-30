/* This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/. */

#include "AndroidBridge.h"
#include "AndroidDecoderModule.h"
#include "JavaCallbacksSupport.h"
#include "SimpleMap.h"
#include "GLImages.h"
#include "MediaData.h"
#include "MediaInfo.h"
#include "VideoUtils.h"
#include "VPXDecoder.h"

#include "nsIGfxInfo.h"
#include "nsPromiseFlatString.h"
#include "nsThreadUtils.h"
#include "prlog.h"

#include <jni.h>

#undef LOG
#define LOG(arg, ...)                                         \
  MOZ_LOG(sAndroidDecoderModuleLog, mozilla::LogLevel::Debug, \
          ("RemoteDataDecoder(%p)::%s: " arg, this, __func__, ##__VA_ARGS__))

using namespace mozilla;
using namespace mozilla::gl;
using namespace mozilla::java;
using namespace mozilla::java::sdk;
using media::TimeUnit;

namespace mozilla {

<<<<<<< HEAD
class RemoteVideoDecoder : public RemoteDataDecoder {
 public:
  // Hold an output buffer and render it to the surface when the frame is sent
  // to compositor, or release it if not presented.
  class RenderOrReleaseOutput : public VideoData::Listener {
   public:
    RenderOrReleaseOutput(java::CodecProxy::Param aCodec,
                          java::Sample::Param aSample)
        : mCodec(aCodec), mSample(aSample) {}
||||||| merged common ancestors
class RemoteVideoDecoder : public RemoteDataDecoder
{
public:
  // Hold an output buffer and render it to the surface when the frame is sent
  // to compositor, or release it if not presented.
  class RenderOrReleaseOutput : public VideoData::Listener
  {
  public:
    RenderOrReleaseOutput(java::CodecProxy::Param aCodec,
                          java::Sample::Param aSample)
      : mCodec(aCodec)
      , mSample(aSample)
    {
    }
=======
// Hold a reference to the output buffer until we're ready to release it back to
// the Java codec (for rendering or not).
class RenderOrReleaseOutput {
 public:
  RenderOrReleaseOutput(CodecProxy::Param aCodec, Sample::Param aSample)
      : mCodec(aCodec), mSample(aSample) {}
>>>>>>> upstream-releases

<<<<<<< HEAD
    ~RenderOrReleaseOutput() { ReleaseOutput(false); }
||||||| merged common ancestors
    ~RenderOrReleaseOutput()
    {
      ReleaseOutput(false);
    }
=======
  virtual ~RenderOrReleaseOutput() { ReleaseOutput(false); }
>>>>>>> upstream-releases

<<<<<<< HEAD
    void OnSentToCompositor() override {
      ReleaseOutput(true);
||||||| merged common ancestors
    void OnSentToCompositor() override
    {
      ReleaseOutput(true);
=======
 protected:
  void ReleaseOutput(bool aToRender) {
    if (mCodec && mSample) {
      mCodec->ReleaseOutput(mSample, aToRender);
>>>>>>> upstream-releases
      mCodec = nullptr;
      mSample = nullptr;
    }
  }

<<<<<<< HEAD
   private:
    void ReleaseOutput(bool aToRender) {
      if (mCodec && mSample) {
        mCodec->ReleaseOutput(mSample, aToRender);
      }
    }
||||||| merged common ancestors
  private:
    void ReleaseOutput(bool aToRender)
    {
      if (mCodec && mSample) {
        mCodec->ReleaseOutput(mSample, aToRender);
      }
    }
=======
 private:
  CodecProxy::GlobalRef mCodec;
  Sample::GlobalRef mSample;
};
>>>>>>> upstream-releases

class RemoteVideoDecoder : public RemoteDataDecoder {
 public:
  // Render the output to the surface when the frame is sent
  // to compositor, or release it if not presented.
  class CompositeListener : private RenderOrReleaseOutput,
                            public VideoData::Listener {
   public:
    CompositeListener(CodecProxy::Param aCodec, Sample::Param aSample)
        : RenderOrReleaseOutput(aCodec, aSample) {}

<<<<<<< HEAD
  class InputInfo {
   public:
    InputInfo() {}
||||||| merged common ancestors
=======
    void OnSentToCompositor() override { ReleaseOutput(true); }
  };
>>>>>>> upstream-releases

<<<<<<< HEAD
    InputInfo(const int64_t aDurationUs, const gfx::IntSize& aImageSize,
              const gfx::IntSize& aDisplaySize)
        : mDurationUs(aDurationUs),
          mImageSize(aImageSize),
          mDisplaySize(aDisplaySize) {}
||||||| merged common ancestors
  class InputInfo
  {
  public:
    InputInfo() { }

    InputInfo(const int64_t aDurationUs, const gfx::IntSize& aImageSize, const gfx::IntSize& aDisplaySize)
      : mDurationUs(aDurationUs)
      , mImageSize(aImageSize)
      , mDisplaySize(aDisplaySize)
    {
    }
=======
  class InputInfo {
   public:
    InputInfo() {}

    InputInfo(const int64_t aDurationUs, const gfx::IntSize& aImageSize,
              const gfx::IntSize& aDisplaySize)
        : mDurationUs(aDurationUs),
          mImageSize(aImageSize),
          mDisplaySize(aDisplaySize) {}
>>>>>>> upstream-releases

    int64_t mDurationUs;
    gfx::IntSize mImageSize;
    gfx::IntSize mDisplaySize;
  };

  class CallbacksSupport final : public JavaCallbacksSupport {
   public:
    explicit CallbacksSupport(RemoteVideoDecoder* aDecoder)
        : mDecoder(aDecoder) {}

    void HandleInput(int64_t aTimestamp, bool aProcessed) override {
      mDecoder->UpdateInputStatus(aTimestamp, aProcessed);
    }

<<<<<<< HEAD
    void HandleOutput(Sample::Param aSample) override {
      UniquePtr<VideoData::Listener> releaseSample(
          new RenderOrReleaseOutput(mDecoder->mJavaDecoder, aSample));

      BufferInfo::LocalRef info = aSample->Info();

      int32_t flags;
      bool ok = NS_SUCCEEDED(info->Flags(&flags));

      int32_t offset;
      ok &= NS_SUCCEEDED(info->Offset(&offset));

      int32_t size;
      ok &= NS_SUCCEEDED(info->Size(&size));

      int64_t presentationTimeUs;
      ok &= NS_SUCCEEDED(info->PresentationTimeUs(&presentationTimeUs));

      if (!ok) {
        HandleError(MediaResult(NS_ERROR_DOM_MEDIA_FATAL_ERR,
                                RESULT_DETAIL("VideoCallBack::HandleOutput")));
        return;
      }

      InputInfo inputInfo;
      ok = mDecoder->mInputInfos.Find(presentationTimeUs, inputInfo);
      bool isEOS = !!(flags & MediaCodec::BUFFER_FLAG_END_OF_STREAM);
      if (!ok && !isEOS) {
        // Ignore output with no corresponding input.
        return;
      }

      if (ok && (size > 0 || presentationTimeUs >= 0)) {
        RefPtr<layers::Image> img = new SurfaceTextureImage(
            mDecoder->mSurfaceHandle, inputInfo.mImageSize,
            false /* NOT continuous */, gl::OriginPos::BottomLeft);

        RefPtr<VideoData> v = VideoData::CreateFromImage(
            inputInfo.mDisplaySize, offset,
            TimeUnit::FromMicroseconds(presentationTimeUs),
            TimeUnit::FromMicroseconds(inputInfo.mDurationUs), img,
            !!(flags & MediaCodec::BUFFER_FLAG_SYNC_FRAME),
            TimeUnit::FromMicroseconds(presentationTimeUs));

        v->SetListener(std::move(releaseSample));
        mDecoder->UpdateOutputStatus(std::move(v));
      }

      if (isEOS) {
        mDecoder->DrainComplete();
      }
||||||| merged common ancestors
    void HandleOutput(Sample::Param aSample) override
    {
      UniquePtr<VideoData::Listener> releaseSample(
        new RenderOrReleaseOutput(mDecoder->mJavaDecoder, aSample));

      BufferInfo::LocalRef info = aSample->Info();

      int32_t flags;
      bool ok = NS_SUCCEEDED(info->Flags(&flags));

      int32_t offset;
      ok &= NS_SUCCEEDED(info->Offset(&offset));

      int32_t size;
      ok &= NS_SUCCEEDED(info->Size(&size));

      int64_t presentationTimeUs;
      ok &= NS_SUCCEEDED(info->PresentationTimeUs(&presentationTimeUs));

      if (!ok) {
        HandleError(MediaResult(NS_ERROR_DOM_MEDIA_FATAL_ERR,
                                RESULT_DETAIL("VideoCallBack::HandleOutput")));
        return;
      }


      InputInfo inputInfo;
      ok = mDecoder->mInputInfos.Find(presentationTimeUs, inputInfo);
      bool isEOS = !!(flags & MediaCodec::BUFFER_FLAG_END_OF_STREAM);
      if (!ok && !isEOS) {
        // Ignore output with no corresponding input.
        return;
      }

      if (ok && (size > 0 || presentationTimeUs >= 0)) {
        RefPtr<layers::Image> img = new SurfaceTextureImage(
          mDecoder->mSurfaceHandle, inputInfo.mImageSize, false /* NOT continuous */,
          gl::OriginPos::BottomLeft);

        RefPtr<VideoData> v = VideoData::CreateFromImage(
          inputInfo.mDisplaySize, offset,
          TimeUnit::FromMicroseconds(presentationTimeUs),
          TimeUnit::FromMicroseconds(inputInfo.mDurationUs),
          img, !!(flags & MediaCodec::BUFFER_FLAG_SYNC_FRAME),
          TimeUnit::FromMicroseconds(presentationTimeUs));

        v->SetListener(std::move(releaseSample));
        mDecoder->UpdateOutputStatus(std::move(v));
      }

      if (isEOS) {
        mDecoder->DrainComplete();
      }
=======
    void HandleOutput(Sample::Param aSample,
                      java::SampleBuffer::Param aBuffer) override {
      MOZ_ASSERT(!aBuffer, "Video sample should be bufferless");
      // aSample will be implicitly converted into a GlobalRef.
      mDecoder->ProcessOutput(std::move(aSample));
>>>>>>> upstream-releases
    }

    void HandleError(const MediaResult& aError) override {
      mDecoder->Error(aError);
    }

    friend class RemoteDataDecoder;

   private:
    RemoteVideoDecoder* mDecoder;
  };

  RemoteVideoDecoder(const VideoInfo& aConfig, MediaFormat::Param aFormat,
                     const nsString& aDrmStubId, TaskQueue* aTaskQueue)
      : RemoteDataDecoder(MediaData::Type::VIDEO_DATA, aConfig.mMimeType,
                          aFormat, aDrmStubId, aTaskQueue),
        mConfig(aConfig) {}

  ~RemoteVideoDecoder() {
    if (mSurface) {
      SurfaceAllocator::DisposeSurface(mSurface);
    }
  }

<<<<<<< HEAD
  RefPtr<InitPromise> Init() override {
    mSurface = GeckoSurface::LocalRef(SurfaceAllocator::AcquireSurface(
        mConfig.mImage.width, mConfig.mImage.height, false));
||||||| merged common ancestors
  RefPtr<InitPromise> Init() override
  {
    mSurface = GeckoSurface::LocalRef(SurfaceAllocator::AcquireSurface(mConfig.mImage.width, mConfig.mImage.height, false));
=======
  RefPtr<InitPromise> Init() override {
    BufferInfo::LocalRef bufferInfo;
    if (NS_FAILED(BufferInfo::New(&bufferInfo)) || !bufferInfo) {
      return InitPromise::CreateAndReject(NS_ERROR_OUT_OF_MEMORY, __func__);
    }
    mInputBufferInfo = bufferInfo;

    mSurface = GeckoSurface::LocalRef(SurfaceAllocator::AcquireSurface(
        mConfig.mImage.width, mConfig.mImage.height, false));
>>>>>>> upstream-releases
    if (!mSurface) {
      return InitPromise::CreateAndReject(NS_ERROR_DOM_MEDIA_FATAL_ERR,
                                          __func__);
    }

    mSurfaceHandle = mSurface->GetHandle();

    // Register native methods.
    JavaCallbacksSupport::Init();

    mJavaCallbacks = CodecProxy::NativeCallbacks::New();
    if (!mJavaCallbacks) {
      return InitPromise::CreateAndReject(NS_ERROR_DOM_MEDIA_FATAL_ERR,
                                          __func__);
    }
    JavaCallbacksSupport::AttachNative(
        mJavaCallbacks, mozilla::MakeUnique<CallbacksSupport>(this));

    mJavaDecoder = CodecProxy::Create(
        false,  // false indicates to create a decoder and true denotes encoder
        mFormat, mSurface, mJavaCallbacks, mDrmStubId);
    if (mJavaDecoder == nullptr) {
      return InitPromise::CreateAndReject(NS_ERROR_DOM_MEDIA_FATAL_ERR,
                                          __func__);
    }
    mIsCodecSupportAdaptivePlayback =
        mJavaDecoder->IsAdaptivePlaybackSupported();
    mIsHardwareAccelerated = mJavaDecoder->IsHardwareAccelerated();
    return InitPromise::CreateAndResolve(TrackInfo::kVideoTrack, __func__);
  }

  RefPtr<MediaDataDecoder::FlushPromise> Flush() override {
    RefPtr<RemoteVideoDecoder> self = this;
<<<<<<< HEAD
    return RemoteDataDecoder::Flush()->Then(
        mTaskQueue, __func__,
        [self](const FlushPromise::ResolveOrRejectValue& aValue) {
          self->mInputInfos.Clear();
          self->mSeekTarget.reset();
          self->mLatestOutputTime.reset();
          return FlushPromise::CreateAndResolveOrReject(aValue, __func__);
        });
  }

  RefPtr<MediaDataDecoder::DecodePromise> Decode(
      MediaRawData* aSample) override {
    const VideoInfo* config =
        aSample->mTrackInfo ? aSample->mTrackInfo->GetAsVideoInfo() : &mConfig;
    MOZ_ASSERT(config);

    InputInfo info(aSample->mDuration.ToMicroseconds(), config->mImage,
                   config->mDisplay);
    mInputInfos.Insert(aSample->mTime.ToMicroseconds(), info);
    return RemoteDataDecoder::Decode(aSample);
  }

  bool SupportDecoderRecycling() const override {
||||||| merged common ancestors
    return RemoteDataDecoder::Flush()->Then(
      mTaskQueue,
      __func__,
      [self](const FlushPromise::ResolveOrRejectValue& aValue) {
        self->mInputInfos.Clear();
        self->mSeekTarget.reset();
        self->mLatestOutputTime.reset();
        return FlushPromise::CreateAndResolveOrReject(aValue, __func__);
      });
  }

  RefPtr<MediaDataDecoder::DecodePromise> Decode(MediaRawData* aSample) override
  {
    const VideoInfo* config = aSample->mTrackInfo
                              ? aSample->mTrackInfo->GetAsVideoInfo()
                              : &mConfig;
    MOZ_ASSERT(config);

    InputInfo info(
      aSample->mDuration.ToMicroseconds(), config->mImage, config->mDisplay);
    mInputInfos.Insert(aSample->mTime.ToMicroseconds(), info);
    return RemoteDataDecoder::Decode(aSample);
  }

  bool SupportDecoderRecycling() const override
  {
=======
    return InvokeAsync(mTaskQueue, __func__, [self, this]() {
      mInputInfos.Clear();
      mSeekTarget.reset();
      mLatestOutputTime.reset();
      return RemoteDataDecoder::ProcessFlush();
    });
  }

  RefPtr<MediaDataDecoder::DecodePromise> Decode(
      MediaRawData* aSample) override {
    RefPtr<RemoteVideoDecoder> self = this;
    RefPtr<MediaRawData> sample = aSample;
    return InvokeAsync(mTaskQueue, __func__, [self, sample]() {
      const VideoInfo* config = sample->mTrackInfo
                                    ? sample->mTrackInfo->GetAsVideoInfo()
                                    : &self->mConfig;
      MOZ_ASSERT(config);

      InputInfo info(sample->mDuration.ToMicroseconds(), config->mImage,
                     config->mDisplay);
      self->mInputInfos.Insert(sample->mTime.ToMicroseconds(), info);
      return self->RemoteDataDecoder::ProcessDecode(sample);
    });
  }

  bool SupportDecoderRecycling() const override {
>>>>>>> upstream-releases
    return mIsCodecSupportAdaptivePlayback;
  }

  void SetSeekThreshold(const TimeUnit& aTime) override {
    RefPtr<RemoteVideoDecoder> self = this;
    nsCOMPtr<nsIRunnable> runnable = NS_NewRunnableFunction(
<<<<<<< HEAD
        "RemoteVideoDecoder::SetSeekThreshold",
        [self, aTime]() { self->mSeekTarget = Some(aTime); });
||||||| merged common ancestors
      "RemoteVideoDecoder::SetSeekThreshold",
      [self, aTime]() { self->mSeekTarget = Some(aTime); });
=======
        "RemoteVideoDecoder::SetSeekThreshold", [self, aTime]() {
          if (aTime.IsValid()) {
            self->mSeekTarget = Some(aTime);
          } else {
            self->mSeekTarget.reset();
          }
        });
>>>>>>> upstream-releases
    nsresult rv = mTaskQueue->Dispatch(runnable.forget());
    MOZ_DIAGNOSTIC_ASSERT(NS_SUCCEEDED(rv));
    Unused << rv;
  }

  bool IsUsefulData(const RefPtr<MediaData>& aSample) override {
    AssertOnTaskQueue();

    if (mLatestOutputTime && aSample->mTime < mLatestOutputTime.value()) {
      return false;
    }

    const TimeUnit endTime = aSample->GetEndTime();
    if (mSeekTarget && endTime <= mSeekTarget.value()) {
      return false;
    }

    mSeekTarget.reset();
    mLatestOutputTime = Some(endTime);
    return true;
  }

  bool IsHardwareAccelerated(nsACString& aFailureReason) const override {
    return mIsHardwareAccelerated;
  }

<<<<<<< HEAD
  ConversionRequired NeedsConversion() const override {
    return ConversionRequired::kNeedAnnexB;
  }

 private:
||||||| merged common ancestors
private:
=======
  ConversionRequired NeedsConversion() const override {
    return ConversionRequired::kNeedAnnexB;
  }

 private:
  // Param and LocalRef are only valid for the duration of a JNI method call.
  // Use GlobalRef as the parameter type to keep the Java object referenced
  // until running.
  void ProcessOutput(Sample::GlobalRef&& aSample) {
    if (!mTaskQueue->IsCurrentThreadIn()) {
      nsresult rv = mTaskQueue->Dispatch(NewRunnableMethod<Sample::GlobalRef&&>(
          "RemoteVideoDecoder::ProcessOutput", this,
          &RemoteVideoDecoder::ProcessOutput, std::move(aSample)));
      MOZ_DIAGNOSTIC_ASSERT(NS_SUCCEEDED(rv));
      Unused << rv;
      return;
    }

    AssertOnTaskQueue();
    if (GetState() == State::SHUTDOWN) {
      aSample->Dispose();
      return;
    }

    UniquePtr<VideoData::Listener> releaseSample(
        new CompositeListener(mJavaDecoder, aSample));

    BufferInfo::LocalRef info = aSample->Info();
    MOZ_ASSERT(info);

    int32_t flags;
    bool ok = NS_SUCCEEDED(info->Flags(&flags));

    int32_t offset;
    ok &= NS_SUCCEEDED(info->Offset(&offset));

    int32_t size;
    ok &= NS_SUCCEEDED(info->Size(&size));

    int64_t presentationTimeUs;
    ok &= NS_SUCCEEDED(info->PresentationTimeUs(&presentationTimeUs));

    if (!ok) {
      Error(MediaResult(NS_ERROR_DOM_MEDIA_FATAL_ERR,
                        RESULT_DETAIL("VideoCallBack::HandleOutput")));
      return;
    }

    InputInfo inputInfo;
    ok = mInputInfos.Find(presentationTimeUs, inputInfo);
    bool isEOS = !!(flags & MediaCodec::BUFFER_FLAG_END_OF_STREAM);
    if (!ok && !isEOS) {
      // Ignore output with no corresponding input.
      return;
    }

    if (ok && (size > 0 || presentationTimeUs >= 0)) {
      RefPtr<layers::Image> img = new SurfaceTextureImage(
          mSurfaceHandle, inputInfo.mImageSize, false /* NOT continuous */,
          gl::OriginPos::BottomLeft, mConfig.HasAlpha());

      RefPtr<VideoData> v = VideoData::CreateFromImage(
          inputInfo.mDisplaySize, offset,
          TimeUnit::FromMicroseconds(presentationTimeUs),
          TimeUnit::FromMicroseconds(inputInfo.mDurationUs), img,
          !!(flags & MediaCodec::BUFFER_FLAG_SYNC_FRAME),
          TimeUnit::FromMicroseconds(presentationTimeUs));

      v->SetListener(std::move(releaseSample));
      RemoteDataDecoder::UpdateOutputStatus(std::move(v));
    }

    if (isEOS) {
      DrainComplete();
    }
  }

>>>>>>> upstream-releases
  const VideoInfo mConfig;
  GeckoSurface::GlobalRef mSurface;
  AndroidSurfaceTextureHandle mSurfaceHandle;
  // Only accessed on reader's task queue.
  bool mIsCodecSupportAdaptivePlayback = false;
  // Can be accessed on any thread, but only written on during init.
  bool mIsHardwareAccelerated = false;
  // Accessed on mTaskQueue and reader's TaskQueue. SimpleMap however is
  // thread-safe, so it's okay to do so.
  SimpleMap<InputInfo> mInputInfos;
  // Only accessed on the TaskQueue.
  Maybe<TimeUnit> mSeekTarget;
  Maybe<TimeUnit> mLatestOutputTime;
};

class RemoteAudioDecoder : public RemoteDataDecoder {
 public:
  RemoteAudioDecoder(const AudioInfo& aConfig, MediaFormat::Param aFormat,
                     const nsString& aDrmStubId, TaskQueue* aTaskQueue)
<<<<<<< HEAD
      : RemoteDataDecoder(MediaData::Type::AUDIO_DATA, aConfig.mMimeType,
                          aFormat, aDrmStubId, aTaskQueue),
        mConfig(aConfig) {
||||||| merged common ancestors
    : RemoteDataDecoder(MediaData::Type::AUDIO_DATA, aConfig.mMimeType,
                        aFormat, aDrmStubId, aTaskQueue)
    , mConfig(aConfig)
  {
=======
      : RemoteDataDecoder(MediaData::Type::AUDIO_DATA, aConfig.mMimeType,
                          aFormat, aDrmStubId, aTaskQueue) {
>>>>>>> upstream-releases
    JNIEnv* const env = jni::GetEnvForThread();

    bool formatHasCSD = false;
    NS_ENSURE_SUCCESS_VOID(
        aFormat->ContainsKey(NS_LITERAL_STRING("csd-0"), &formatHasCSD));

    if (!formatHasCSD && aConfig.mCodecSpecificConfig->Length() >= 2) {
      jni::ByteBuffer::LocalRef buffer(env);
      buffer = jni::ByteBuffer::New(aConfig.mCodecSpecificConfig->Elements(),
                                    aConfig.mCodecSpecificConfig->Length());
      NS_ENSURE_SUCCESS_VOID(
          aFormat->SetByteBuffer(NS_LITERAL_STRING("csd-0"), buffer));
    }
  }

<<<<<<< HEAD
  RefPtr<InitPromise> Init() override {
||||||| merged common ancestors
  RefPtr<InitPromise> Init() override
  {
=======
  RefPtr<InitPromise> Init() override {
    BufferInfo::LocalRef bufferInfo;
    if (NS_FAILED(BufferInfo::New(&bufferInfo)) || !bufferInfo) {
      return InitPromise::CreateAndReject(NS_ERROR_OUT_OF_MEMORY, __func__);
    }
    mInputBufferInfo = bufferInfo;

>>>>>>> upstream-releases
    // Register native methods.
    JavaCallbacksSupport::Init();

    mJavaCallbacks = CodecProxy::NativeCallbacks::New();
    if (!mJavaCallbacks) {
      return InitPromise::CreateAndReject(NS_ERROR_DOM_MEDIA_FATAL_ERR,
                                          __func__);
    }
    JavaCallbacksSupport::AttachNative(
        mJavaCallbacks, mozilla::MakeUnique<CallbacksSupport>(this));

    mJavaDecoder =
        CodecProxy::Create(false, mFormat, nullptr, mJavaCallbacks, mDrmStubId);
    if (mJavaDecoder == nullptr) {
      return InitPromise::CreateAndReject(NS_ERROR_DOM_MEDIA_FATAL_ERR,
                                          __func__);
    }

    return InitPromise::CreateAndResolve(TrackInfo::kAudioTrack, __func__);
  }

<<<<<<< HEAD
 private:
  class CallbacksSupport final : public JavaCallbacksSupport {
   public:
    explicit CallbacksSupport(RemoteAudioDecoder* aDecoder)
        : mDecoder(aDecoder) {}
||||||| merged common ancestors
  ConversionRequired NeedsConversion() const override
  {
    return ConversionRequired::kNeedAnnexB;
  }

private:
  class CallbacksSupport final : public JavaCallbacksSupport
  {
  public:
    explicit CallbacksSupport(RemoteAudioDecoder* aDecoder) : mDecoder(aDecoder) { }
=======
  RefPtr<FlushPromise> Flush() override {
    RefPtr<RemoteAudioDecoder> self = this;
    return InvokeAsync(mTaskQueue, __func__, [self]() {
      self->mFirstDemuxedSampleTime.reset();
      return self->RemoteDataDecoder::ProcessFlush();
    });
  }

  RefPtr<DecodePromise> Decode(MediaRawData* aSample) override {
    RefPtr<RemoteAudioDecoder> self = this;
    RefPtr<MediaRawData> sample = aSample;
    return InvokeAsync(mTaskQueue, __func__, [self, sample]() {
      if (!self->mFirstDemuxedSampleTime) {
        MOZ_ASSERT(sample->mTime.IsValid());
        self->mFirstDemuxedSampleTime.emplace(sample->mTime);
      }
      return self->RemoteDataDecoder::ProcessDecode(sample);
    });
  }

 private:
  class CallbacksSupport final : public JavaCallbacksSupport {
   public:
    explicit CallbacksSupport(RemoteAudioDecoder* aDecoder)
        : mDecoder(aDecoder) {}
>>>>>>> upstream-releases

    void HandleInput(int64_t aTimestamp, bool aProcessed) override {
      mDecoder->UpdateInputStatus(aTimestamp, aProcessed);
    }

<<<<<<< HEAD
    void HandleOutput(Sample::Param aSample) override {
      BufferInfo::LocalRef info = aSample->Info();
||||||| merged common ancestors
    void HandleOutput(Sample::Param aSample) override
    {
      BufferInfo::LocalRef info = aSample->Info();
=======
    void HandleOutput(Sample::Param aSample,
                      java::SampleBuffer::Param aBuffer) override {
      MOZ_ASSERT(aBuffer, "Audio sample should have buffer");
      // aSample will be implicitly converted into a GlobalRef.
      mDecoder->ProcessOutput(std::move(aSample), std::move(aBuffer));
    }
>>>>>>> upstream-releases

    void HandleOutputFormatChanged(MediaFormat::Param aFormat) override {
      int32_t outputChannels = 0;
      aFormat->GetInteger(NS_LITERAL_STRING("channel-count"), &outputChannels);
      AudioConfig::ChannelLayout layout(outputChannels);
      if (!layout.IsValid()) {
        mDecoder->Error(MediaResult(
            NS_ERROR_DOM_MEDIA_FATAL_ERR,
            RESULT_DETAIL("Invalid channel layout:%d", outputChannels)));
        return;
      }

      int32_t sampleRate = 0;
      aFormat->GetInteger(NS_LITERAL_STRING("sample-rate"), &sampleRate);
      LOG("Audio output format changed: channels:%d sample rate:%d",
          outputChannels, sampleRate);

      mDecoder->ProcessOutputFormatChange(outputChannels, sampleRate);
    }

    void HandleError(const MediaResult& aError) override {
      mDecoder->Error(aError);
    }

   private:
    RemoteAudioDecoder* mDecoder;
  };

  bool IsSampleTimeSmallerThanFirstDemuxedSampleTime(int64_t aTime) const {
    return mFirstDemuxedSampleTime->ToMicroseconds() > aTime;
  }

  bool ShouldDiscardSample(int64_t aSession) const {
    AssertOnTaskQueue();
    // HandleOutput() runs on Android binder thread pool and could be preempted
    // by RemoteDateDecoder task queue. That means ProcessOutput() could be
    // scheduled after ProcessShutdown() or ProcessFlush(). We won't need the
    // sample which is returned after calling Shutdown() and Flush(). We can
    // check mFirstDemuxedSampleTime to know whether the Flush() has been
    // called, becasue it would be reset in Flush().
    return GetState() == State::SHUTDOWN || !mFirstDemuxedSampleTime ||
           mSession != aSession;
  }

  // Param and LocalRef are only valid for the duration of a JNI method call.
  // Use GlobalRef as the parameter type to keep the Java object referenced
  // until running.
  void ProcessOutput(Sample::GlobalRef&& aSample,
                     SampleBuffer::GlobalRef&& aBuffer) {
    if (!mTaskQueue->IsCurrentThreadIn()) {
      nsresult rv = mTaskQueue->Dispatch(
          NewRunnableMethod<Sample::GlobalRef&&, SampleBuffer::GlobalRef&&>(
              "RemoteAudioDecoder::ProcessOutput", this,
              &RemoteAudioDecoder::ProcessOutput, std::move(aSample),
              std::move(aBuffer)));
      MOZ_DIAGNOSTIC_ASSERT(NS_SUCCEEDED(rv));
      Unused << rv;
      return;
    }

<<<<<<< HEAD
        jni::ByteBuffer::LocalRef dest =
            jni::ByteBuffer::New(audio.get(), size);
        aSample->WriteToByteBuffer(dest);
||||||| merged common ancestors
        jni::ByteBuffer::LocalRef dest =
          jni::ByteBuffer::New(audio.get(), size);
        aSample->WriteToByteBuffer(dest);
=======
    AssertOnTaskQueue();
>>>>>>> upstream-releases

<<<<<<< HEAD
        RefPtr<AudioData> data = new AudioData(
            0, TimeUnit::FromMicroseconds(presentationTimeUs),
            FramesToTimeUnit(numFrames, mOutputSampleRate), numFrames,
            std::move(audio), mOutputChannels, mOutputSampleRate);
||||||| merged common ancestors
        RefPtr<AudioData> data = new AudioData(
          0, TimeUnit::FromMicroseconds(presentationTimeUs),
          FramesToTimeUnit(numFrames, mOutputSampleRate), numFrames,
          std::move(audio), mOutputChannels, mOutputSampleRate);
=======
    if (ShouldDiscardSample(aSample->Session()) || !aBuffer->IsValid()) {
      aSample->Dispose();
      return;
    }
>>>>>>> upstream-releases

    RenderOrReleaseOutput autoRelease(mJavaDecoder, aSample);

    BufferInfo::LocalRef info = aSample->Info();
    MOZ_ASSERT(info);

    int32_t flags = 0;
    bool ok = NS_SUCCEEDED(info->Flags(&flags));
    bool isEOS = !!(flags & MediaCodec::BUFFER_FLAG_END_OF_STREAM);

    int32_t offset;
    ok &= NS_SUCCEEDED(info->Offset(&offset));

    int64_t presentationTimeUs;
    ok &= NS_SUCCEEDED(info->PresentationTimeUs(&presentationTimeUs));

    int32_t size;
    ok &= NS_SUCCEEDED(info->Size(&size));

    if (!ok ||
        (IsSampleTimeSmallerThanFirstDemuxedSampleTime(presentationTimeUs) &&
         !isEOS)) {
      Error(MediaResult(NS_ERROR_DOM_MEDIA_FATAL_ERR, __func__));
      return;
    }

<<<<<<< HEAD
    void HandleOutputFormatChanged(MediaFormat::Param aFormat) override {
      aFormat->GetInteger(NS_LITERAL_STRING("channel-count"), &mOutputChannels);
      AudioConfig::ChannelLayout layout(mOutputChannels);
      if (!layout.IsValid()) {
        mDecoder->Error(MediaResult(
            NS_ERROR_DOM_MEDIA_FATAL_ERR,
            RESULT_DETAIL("Invalid channel layout:%d", mOutputChannels)));
||||||| merged common ancestors
    void HandleOutputFormatChanged(MediaFormat::Param aFormat) override
    {
      aFormat->GetInteger(NS_LITERAL_STRING("channel-count"), &mOutputChannels);
      AudioConfig::ChannelLayout layout(mOutputChannels);
      if (!layout.IsValid()) {
        mDecoder->Error(MediaResult(
          NS_ERROR_DOM_MEDIA_FATAL_ERR,
          RESULT_DETAIL("Invalid channel layout:%d", mOutputChannels)));
=======
    if (size > 0) {
#ifdef MOZ_SAMPLE_TYPE_S16
      const int32_t numSamples = size / 2;
#else
#  error We only support 16-bit integer PCM
#endif

      AlignedAudioBuffer audio(numSamples);
      if (!audio) {
        Error(MediaResult(NS_ERROR_OUT_OF_MEMORY, __func__));
>>>>>>> upstream-releases
        return;
      }

      jni::ByteBuffer::LocalRef dest = jni::ByteBuffer::New(audio.get(), size);
      aBuffer->WriteToByteBuffer(dest, offset, size);

      RefPtr<AudioData> data =
          new AudioData(0, TimeUnit::FromMicroseconds(presentationTimeUs),
                        std::move(audio), mOutputChannels, mOutputSampleRate);

      UpdateOutputStatus(std::move(data));
    }

<<<<<<< HEAD
    void HandleError(const MediaResult& aError) override {
      mDecoder->Error(aError);
||||||| merged common ancestors
    void HandleError(const MediaResult& aError) override
    {
      mDecoder->Error(aError);
=======
    if (isEOS) {
      DrainComplete();
>>>>>>> upstream-releases
    }
  }

<<<<<<< HEAD
   private:
    RemoteAudioDecoder* mDecoder;
    int32_t mOutputChannels;
    int32_t mOutputSampleRate;
  };
||||||| merged common ancestors
  private:
    RemoteAudioDecoder* mDecoder;
    int32_t mOutputChannels;
    int32_t mOutputSampleRate;
  };
=======
  void ProcessOutputFormatChange(int32_t aChannels, int32_t aSampleRate) {
    if (!mTaskQueue->IsCurrentThreadIn()) {
      nsresult rv = mTaskQueue->Dispatch(NewRunnableMethod<int32_t, int32_t>(
          "RemoteAudioDecoder::ProcessOutputFormatChange", this,
          &RemoteAudioDecoder::ProcessOutputFormatChange, aChannels,
          aSampleRate));
      MOZ_DIAGNOSTIC_ASSERT(NS_SUCCEEDED(rv));
      Unused << rv;
      return;
    }
>>>>>>> upstream-releases

    AssertOnTaskQueue();

    mOutputChannels = aChannels;
    mOutputSampleRate = aSampleRate;
  }

  int32_t mOutputChannels;
  int32_t mOutputSampleRate;
  Maybe<TimeUnit> mFirstDemuxedSampleTime;
};

already_AddRefed<MediaDataDecoder> RemoteDataDecoder::CreateAudioDecoder(
    const CreateDecoderParams& aParams, const nsString& aDrmStubId,
    CDMProxy* aProxy) {
  const AudioInfo& config = aParams.AudioConfig();
  MediaFormat::LocalRef format;
  NS_ENSURE_SUCCESS(
      MediaFormat::CreateAudioFormat(config.mMimeType, config.mRate,
                                     config.mChannels, &format),
      nullptr);

  RefPtr<MediaDataDecoder> decoder =
      new RemoteAudioDecoder(config, format, aDrmStubId, aParams.mTaskQueue);
  if (aProxy) {
    decoder = new EMEMediaDataDecoderProxy(aParams, decoder.forget(), aProxy);
  }
  return decoder.forget();
}

already_AddRefed<MediaDataDecoder> RemoteDataDecoder::CreateVideoDecoder(
    const CreateDecoderParams& aParams, const nsString& aDrmStubId,
    CDMProxy* aProxy) {
  const VideoInfo& config = aParams.VideoConfig();
  MediaFormat::LocalRef format;
  NS_ENSURE_SUCCESS(MediaFormat::CreateVideoFormat(
                        TranslateMimeType(config.mMimeType),
                        config.mImage.width, config.mImage.height, &format),
                    nullptr);

  RefPtr<MediaDataDecoder> decoder =
      new RemoteVideoDecoder(config, format, aDrmStubId, aParams.mTaskQueue);
  if (aProxy) {
    decoder = new EMEMediaDataDecoderProxy(aParams, decoder.forget(), aProxy);
  }
  return decoder.forget();
}

RemoteDataDecoder::RemoteDataDecoder(MediaData::Type aType,
                                     const nsACString& aMimeType,
                                     MediaFormat::Param aFormat,
                                     const nsString& aDrmStubId,
                                     TaskQueue* aTaskQueue)
<<<<<<< HEAD
    : mType(aType),
      mMimeType(aMimeType),
      mFormat(aFormat),
      mDrmStubId(aDrmStubId),
      mTaskQueue(aTaskQueue),
      mNumPendingInputs(0) {}

RefPtr<MediaDataDecoder::FlushPromise> RemoteDataDecoder::Flush() {
  RefPtr<RemoteDataDecoder> self = this;
  return InvokeAsync(mTaskQueue, __func__, [self, this]() {
    mDecodedData = DecodedData();
    mNumPendingInputs = 0;
    mDecodePromise.RejectIfExists(NS_ERROR_DOM_MEDIA_CANCELED, __func__);
    mDrainPromise.RejectIfExists(NS_ERROR_DOM_MEDIA_CANCELED, __func__);
    mDrainStatus = DrainStatus::DRAINED;
    mJavaDecoder->Flush();
    return FlushPromise::CreateAndResolve(true, __func__);
  });
||||||| merged common ancestors
  : mType(aType)
  , mMimeType(aMimeType)
  , mFormat(aFormat)
  , mDrmStubId(aDrmStubId)
  , mTaskQueue(aTaskQueue)
  , mNumPendingInputs(0)
{
}

RefPtr<MediaDataDecoder::FlushPromise>
RemoteDataDecoder::Flush()
{
  RefPtr<RemoteDataDecoder> self = this;
  return InvokeAsync(mTaskQueue, __func__, [self, this]() {
    mDecodedData.Clear();
    mNumPendingInputs = 0;
    mDecodePromise.RejectIfExists(NS_ERROR_DOM_MEDIA_CANCELED, __func__);
    mDrainPromise.RejectIfExists(NS_ERROR_DOM_MEDIA_CANCELED, __func__);
    mDrainStatus = DrainStatus::DRAINED;
    mJavaDecoder->Flush();
    return FlushPromise::CreateAndResolve(true, __func__);
  });
=======
    : mType(aType),
      mMimeType(aMimeType),
      mFormat(aFormat),
      mDrmStubId(aDrmStubId),
      mTaskQueue(aTaskQueue),
      mSession(0),
      mNumPendingInputs(0) {}

RefPtr<MediaDataDecoder::FlushPromise> RemoteDataDecoder::Flush() {
  RefPtr<RemoteDataDecoder> self = this;
  return InvokeAsync(mTaskQueue, this, __func__,
                     &RemoteDataDecoder::ProcessFlush);
}

RefPtr<MediaDataDecoder::FlushPromise> RemoteDataDecoder::ProcessFlush() {
  AssertOnTaskQueue();

  mDecodedData = DecodedData();
  UpdatePendingInputStatus(PendingOp::CLEAR);
  mDecodePromise.RejectIfExists(NS_ERROR_DOM_MEDIA_CANCELED, __func__);
  mDrainPromise.RejectIfExists(NS_ERROR_DOM_MEDIA_CANCELED, __func__);
  SetState(State::DRAINED);
  mJavaDecoder->Flush();
  return FlushPromise::CreateAndResolve(true, __func__);
>>>>>>> upstream-releases
}

RefPtr<MediaDataDecoder::DecodePromise> RemoteDataDecoder::Drain() {
  RefPtr<RemoteDataDecoder> self = this;
  return InvokeAsync(mTaskQueue, __func__, [self, this]() {
<<<<<<< HEAD
    if (mShutdown) {
      return DecodePromise::CreateAndReject(NS_ERROR_DOM_MEDIA_CANCELED,
                                            __func__);
||||||| merged common ancestors
    if (mShutdown) {
      return DecodePromise::CreateAndReject(NS_ERROR_DOM_MEDIA_CANCELED, __func__);
=======
    if (GetState() == State::SHUTDOWN) {
      return DecodePromise::CreateAndReject(NS_ERROR_DOM_MEDIA_CANCELED,
                                            __func__);
>>>>>>> upstream-releases
    }
    RefPtr<DecodePromise> p = mDrainPromise.Ensure(__func__);
    if (GetState() == State::DRAINED) {
      // There's no operation to perform other than returning any already
      // decoded data.
      ReturnDecodedData();
      return p;
    }

    if (GetState() == State::DRAINING) {
      // Draining operation already pending, let it complete its course.
      return p;
    }

    SetState(State::DRAINING);
    self->mInputBufferInfo->Set(0, 0, -1,
                                MediaCodec::BUFFER_FLAG_END_OF_STREAM);
    mSession = mJavaDecoder->Input(nullptr, self->mInputBufferInfo, nullptr);
    return p;
  });
}

RefPtr<ShutdownPromise> RemoteDataDecoder::Shutdown() {
  LOG("");
  RefPtr<RemoteDataDecoder> self = this;
  return InvokeAsync(mTaskQueue, this, __func__,
                     &RemoteDataDecoder::ProcessShutdown);
}

RefPtr<ShutdownPromise> RemoteDataDecoder::ProcessShutdown() {
  AssertOnTaskQueue();
  SetState(State::SHUTDOWN);
  if (mJavaDecoder) {
    mJavaDecoder->Release();
    mJavaDecoder = nullptr;
  }

  if (mJavaCallbacks) {
    JavaCallbacksSupport::GetNative(mJavaCallbacks)->Cancel();
    JavaCallbacksSupport::DisposeNative(mJavaCallbacks);
    mJavaCallbacks = nullptr;
  }

  mFormat = nullptr;

  return ShutdownPromise::CreateAndResolve(true, __func__);
}

static CryptoInfo::LocalRef GetCryptoInfoFromSample(
    const MediaRawData* aSample) {
  auto& cryptoObj = aSample->mCrypto;

  if (!cryptoObj.IsEncrypted()) {
    return nullptr;
  }

  CryptoInfo::LocalRef cryptoInfo;
  nsresult rv = CryptoInfo::New(&cryptoInfo);
  NS_ENSURE_SUCCESS(rv, nullptr);

  uint32_t numSubSamples = std::min<uint32_t>(
      cryptoObj.mPlainSizes.Length(), cryptoObj.mEncryptedSizes.Length());

  uint32_t totalSubSamplesSize = 0;
  for (auto& size : cryptoObj.mEncryptedSizes) {
    totalSubSamplesSize += size;
  }

  // mPlainSizes is uint16_t, need to transform to uint32_t first.
  nsTArray<uint32_t> plainSizes;
  for (auto& size : cryptoObj.mPlainSizes) {
    totalSubSamplesSize += size;
    plainSizes.AppendElement(size);
  }

  uint32_t codecSpecificDataSize = aSample->Size() - totalSubSamplesSize;
  // Size of codec specific data("CSD") for Android MediaCodec usage should be
  // included in the 1st plain size.
  plainSizes[0] += codecSpecificDataSize;

  static const int kExpectedIVLength = 16;
  auto tempIV(cryptoObj.mIV);
  auto tempIVLength = tempIV.Length();
  MOZ_ASSERT(tempIVLength <= kExpectedIVLength);
  for (size_t i = tempIVLength; i < kExpectedIVLength; i++) {
    // Padding with 0
    tempIV.AppendElement(0);
  }

  auto numBytesOfPlainData = mozilla::jni::IntArray::New(
      reinterpret_cast<int32_t*>(&plainSizes[0]), plainSizes.Length());

  auto numBytesOfEncryptedData = mozilla::jni::IntArray::New(
      reinterpret_cast<const int32_t*>(&cryptoObj.mEncryptedSizes[0]),
      cryptoObj.mEncryptedSizes.Length());
  auto iv = mozilla::jni::ByteArray::New(reinterpret_cast<int8_t*>(&tempIV[0]),
                                         tempIV.Length());
  auto keyId = mozilla::jni::ByteArray::New(
      reinterpret_cast<const int8_t*>(&cryptoObj.mKeyId[0]),
      cryptoObj.mKeyId.Length());
  cryptoInfo->Set(numSubSamples, numBytesOfPlainData, numBytesOfEncryptedData,
                  keyId, iv, MediaCodec::CRYPTO_MODE_AES_CTR);

  return cryptoInfo;
}

<<<<<<< HEAD
RefPtr<MediaDataDecoder::DecodePromise> RemoteDataDecoder::Decode(
    MediaRawData* aSample) {
  MOZ_ASSERT(aSample != nullptr);

||||||| merged common ancestors
RefPtr<MediaDataDecoder::DecodePromise>
RemoteDataDecoder::Decode(MediaRawData* aSample)
{
  MOZ_ASSERT(aSample != nullptr);

=======
RefPtr<MediaDataDecoder::DecodePromise> RemoteDataDecoder::Decode(
    MediaRawData* aSample) {
>>>>>>> upstream-releases
  RefPtr<RemoteDataDecoder> self = this;
  RefPtr<MediaRawData> sample = aSample;
<<<<<<< HEAD
  return InvokeAsync(mTaskQueue, __func__, [self, sample]() {
    jni::ByteBuffer::LocalRef bytes = jni::ByteBuffer::New(
        const_cast<uint8_t*>(sample->Data()), sample->Size());
||||||| merged common ancestors
  return InvokeAsync(mTaskQueue, __func__, [self, sample]() {
    jni::ByteBuffer::LocalRef bytes = jni::ByteBuffer::New(
      const_cast<uint8_t*>(sample->Data()), sample->Size());
=======
  return InvokeAsync(mTaskQueue, __func__,
                     [self, sample]() { return self->ProcessDecode(sample); });
}
>>>>>>> upstream-releases

<<<<<<< HEAD
    BufferInfo::LocalRef bufferInfo;
    nsresult rv = BufferInfo::New(&bufferInfo);
    if (NS_FAILED(rv)) {
      return DecodePromise::CreateAndReject(
          MediaResult(NS_ERROR_OUT_OF_MEMORY, __func__), __func__);
    }
    bufferInfo->Set(0, sample->Size(), sample->mTime.ToMicroseconds(), 0);

    self->mDrainStatus = DrainStatus::DRAINABLE;
    return self->mJavaDecoder->Input(bytes, bufferInfo,
                                     GetCryptoInfoFromSample(sample))
               ? self->mDecodePromise.Ensure(__func__)
               : DecodePromise::CreateAndReject(
                     MediaResult(NS_ERROR_OUT_OF_MEMORY, __func__), __func__);
  });
||||||| merged common ancestors
    BufferInfo::LocalRef bufferInfo;
    nsresult rv = BufferInfo::New(&bufferInfo);
    if (NS_FAILED(rv)) {
      return DecodePromise::CreateAndReject(
        MediaResult(NS_ERROR_OUT_OF_MEMORY, __func__), __func__);
    }
    bufferInfo->Set(0, sample->Size(), sample->mTime.ToMicroseconds(), 0);

    self->mDrainStatus = DrainStatus::DRAINABLE;
    return self->mJavaDecoder->Input(bytes, bufferInfo, GetCryptoInfoFromSample(sample))
           ? self->mDecodePromise.Ensure(__func__)
           : DecodePromise::CreateAndReject(
               MediaResult(NS_ERROR_OUT_OF_MEMORY, __func__), __func__);

  });
=======
RefPtr<MediaDataDecoder::DecodePromise> RemoteDataDecoder::ProcessDecode(
    MediaRawData* aSample) {
  AssertOnTaskQueue();
  MOZ_ASSERT(aSample != nullptr);
  jni::ByteBuffer::LocalRef bytes = jni::ByteBuffer::New(
      const_cast<uint8_t*>(aSample->Data()), aSample->Size());

  SetState(State::DRAINABLE);
  mInputBufferInfo->Set(0, aSample->Size(), aSample->mTime.ToMicroseconds(), 0);
  int64_t session = mJavaDecoder->Input(bytes, mInputBufferInfo,
                                        GetCryptoInfoFromSample(aSample));
  if (session == java::CodecProxy::INVALID_SESSION) {
    return DecodePromise::CreateAndReject(
        MediaResult(NS_ERROR_OUT_OF_MEMORY, __func__), __func__);
  }
  mSession = session;
  return mDecodePromise.Ensure(__func__);
}

void RemoteDataDecoder::UpdatePendingInputStatus(PendingOp aOp) {
  AssertOnTaskQueue();
  switch (aOp) {
    case PendingOp::INCREASE:
      mNumPendingInputs++;
      break;
    case PendingOp::DECREASE:
      mNumPendingInputs--;
      break;
    case PendingOp::CLEAR:
      mNumPendingInputs = 0;
      break;
  }
>>>>>>> upstream-releases
}

void RemoteDataDecoder::UpdateInputStatus(int64_t aTimestamp, bool aProcessed) {
  if (!mTaskQueue->IsCurrentThreadIn()) {
    nsresult rv = mTaskQueue->Dispatch(NewRunnableMethod<int64_t, bool>(
        "RemoteDataDecoder::UpdateInputStatus", this,
        &RemoteDataDecoder::UpdateInputStatus, aTimestamp, aProcessed));
    MOZ_DIAGNOSTIC_ASSERT(NS_SUCCEEDED(rv));
    Unused << rv;
    return;
  }
  AssertOnTaskQueue();
  if (GetState() == State::SHUTDOWN) {
    return;
  }

  if (!aProcessed) {
    UpdatePendingInputStatus(PendingOp::INCREASE);
  } else if (HasPendingInputs()) {
    UpdatePendingInputStatus(PendingOp::DECREASE);
  }

<<<<<<< HEAD
  if (mNumPendingInputs ==
          0 ||  // Input has been processed, request the next one.
      !mDecodedData.IsEmpty()) {  // Previous output arrived before Decode().
||||||| merged common ancestors
  if (mNumPendingInputs == 0 || // Input has been processed, request the next one.
      !mDecodedData.IsEmpty()) { // Previous output arrived before Decode().
=======
  if (!HasPendingInputs() ||  // Input has been processed, request the next one.
      !mDecodedData.IsEmpty()) {  // Previous output arrived before Decode().
>>>>>>> upstream-releases
    ReturnDecodedData();
  }
}

<<<<<<< HEAD
void RemoteDataDecoder::UpdateOutputStatus(RefPtr<MediaData>&& aSample) {
  if (!mTaskQueue->IsCurrentThreadIn()) {
    nsresult rv =
        mTaskQueue->Dispatch(NewRunnableMethod<const RefPtr<MediaData>>(
            "RemoteDataDecoder::UpdateOutputStatus", this,
            &RemoteDataDecoder::UpdateOutputStatus, std::move(aSample)));
    MOZ_DIAGNOSTIC_ASSERT(NS_SUCCEEDED(rv));
    Unused << rv;
    return;
  }
||||||| merged common ancestors
void
RemoteDataDecoder::UpdateOutputStatus(RefPtr<MediaData>&& aSample)
{
  if (!mTaskQueue->IsCurrentThreadIn()) {
    nsresult rv =
      mTaskQueue->Dispatch(
        NewRunnableMethod<const RefPtr<MediaData>>("RemoteDataDecoder::UpdateOutputStatus",
                                                   this,
                                                   &RemoteDataDecoder::UpdateOutputStatus,
                                                   std::move(aSample)));
    MOZ_DIAGNOSTIC_ASSERT(NS_SUCCEEDED(rv));
    Unused << rv;
    return;
  }
=======
void RemoteDataDecoder::UpdateOutputStatus(RefPtr<MediaData>&& aSample) {
>>>>>>> upstream-releases
  AssertOnTaskQueue();
  if (GetState() == State::SHUTDOWN) {
    return;
  }
  if (IsUsefulData(aSample)) {
    mDecodedData.AppendElement(std::move(aSample));
  }
  ReturnDecodedData();
}

void RemoteDataDecoder::ReturnDecodedData() {
  AssertOnTaskQueue();
  MOZ_ASSERT(GetState() != State::SHUTDOWN);

  // We only want to clear mDecodedData when we have resolved the promises.
  if (!mDecodePromise.IsEmpty()) {
    mDecodePromise.Resolve(std::move(mDecodedData), __func__);
    mDecodedData = DecodedData();
  } else if (!mDrainPromise.IsEmpty() &&
<<<<<<< HEAD
             (!mDecodedData.IsEmpty() ||
              mDrainStatus == DrainStatus::DRAINED)) {
    mDrainPromise.Resolve(std::move(mDecodedData), __func__);
    mDecodedData = DecodedData();
||||||| merged common ancestors
             (!mDecodedData.IsEmpty() || mDrainStatus == DrainStatus::DRAINED)) {
    mDrainPromise.Resolve(mDecodedData, __func__);
    mDecodedData.Clear();
=======
             (!mDecodedData.IsEmpty() || GetState() == State::DRAINED)) {
    mDrainPromise.Resolve(std::move(mDecodedData), __func__);
    mDecodedData = DecodedData();
>>>>>>> upstream-releases
  }
}

void RemoteDataDecoder::DrainComplete() {
  if (!mTaskQueue->IsCurrentThreadIn()) {
    nsresult rv = mTaskQueue->Dispatch(
        NewRunnableMethod("RemoteDataDecoder::DrainComplete", this,
                          &RemoteDataDecoder::DrainComplete));
    MOZ_DIAGNOSTIC_ASSERT(NS_SUCCEEDED(rv));
    Unused << rv;
    return;
  }
  AssertOnTaskQueue();
  if (GetState() == State::SHUTDOWN) {
    return;
  }
  SetState(State::DRAINED);
  ReturnDecodedData();
  // Make decoder accept input again.
  mJavaDecoder->Flush();
}

void RemoteDataDecoder::Error(const MediaResult& aError) {
  if (!mTaskQueue->IsCurrentThreadIn()) {
    nsresult rv = mTaskQueue->Dispatch(NewRunnableMethod<MediaResult>(
        "RemoteDataDecoder::Error", this, &RemoteDataDecoder::Error, aError));
    MOZ_DIAGNOSTIC_ASSERT(NS_SUCCEEDED(rv));
    Unused << rv;
    return;
  }
  AssertOnTaskQueue();
  if (GetState() == State::SHUTDOWN) {
    return;
  }
  mDecodePromise.RejectIfExists(aError, __func__);
  mDrainPromise.RejectIfExists(aError, __func__);
}

}  // namespace mozilla
