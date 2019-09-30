/* This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at http://mozilla.org/MPL/2.0/. */

#ifndef AUDIO_SESSION_H_
#define AUDIO_SESSION_H_

#include "mozilla/Attributes.h"
#include "mozilla/ReentrantMonitor.h"
#include "mozilla/TimeStamp.h"
#include "nsTArray.h"

#include "MediaConduitInterface.h"
#include "MediaEngineWrapper.h"
#include "RtpSourceObserver.h"
#include "RtpPacketQueue.h"

// Audio Engine Includes
#include "webrtc/common_types.h"
#include "webrtc/modules/audio_device/include/fake_audio_device.h"
#include "webrtc/voice_engine/include/voe_base.h"
#include "webrtc/voice_engine/channel_proxy.h"

/** This file hosts several structures identifying different aspects
 * of a RTP Session.
 */
namespace mozilla {
// Helper function

DOMHighResTimeStamp NTPtoDOMHighResTimeStamp(uint32_t ntpHigh, uint32_t ntpLow);

/**
 * Concrete class for Audio session. Hooks up
 *  - media-source and target to external transport
 */
class WebrtcAudioConduit : public AudioSessionConduit,
                           public webrtc::Transport,
                           public webrtc::RtpPacketObserver {
 public:
  // VoiceEngine defined constant for Payload Name Size.
  static const unsigned int CODEC_PLNAME_SIZE;

  /**
   * APIs used by the registered external transport to this Conduit to
   * feed in received RTP Frames to the VoiceEngine for decoding
   */
  MediaConduitErrorCode ReceivedRTPPacket(const void* data, int len,
                                          uint32_t ssrc) override;

  /**
   * APIs used by the registered external transport to this Conduit to
   * feed in received RTCP Frames to the VoiceEngine for decoding
   */
  MediaConduitErrorCode ReceivedRTCPPacket(const void* data, int len) override;

  MediaConduitErrorCode StopTransmitting() override;
  MediaConduitErrorCode StartTransmitting() override;
  MediaConduitErrorCode StopReceiving() override;
  MediaConduitErrorCode StartReceiving() override;

  MediaConduitErrorCode StopTransmittingLocked();
  MediaConduitErrorCode StartTransmittingLocked();
  MediaConduitErrorCode StopReceivingLocked();
  MediaConduitErrorCode StartReceivingLocked();

  /**
   * Function to configure send codec for the audio session
   * @param sendSessionConfig: CodecConfiguration
   * @result: On Success, the audio engine is configured with passed in codec
   * for send On failure, audio engine transmit functionality is disabled. NOTE:
   * This API can be invoked multiple time. Invoking this API may involve
   * restarting transmission sub-system on the engine.
   */
  MediaConduitErrorCode ConfigureSendMediaCodec(
      const AudioCodecConfig* codecConfig) override;
  /**
   * Function to configure list of receive codecs for the audio session
   * @param sendSessionConfig: CodecConfiguration
   * @result: On Success, the audio engine is configured with passed in codec
   * for send Also the playout is enabled. On failure, audio engine transmit
   * functionality is disabled. NOTE: This API can be invoked multiple time.
   * Invoking this API may involve restarting transmission sub-system on the
   * engine.
   */
  MediaConduitErrorCode ConfigureRecvMediaCodecs(
      const std::vector<UniquePtr<AudioCodecConfig>>& codecConfigList) override;

  MediaConduitErrorCode SetLocalRTPExtensions(
      MediaSessionConduitLocalDirection aDirection,
      const RtpExtList& extensions) override;

  /**
   * Register External Transport to this Conduit. RTP and RTCP frames from the
   * VoiceEngine shall be passed to the registered transport for transporting
   * externally.
   */
  MediaConduitErrorCode SetTransmitterTransport(
      RefPtr<TransportInterface> aTransport) override;

  MediaConduitErrorCode SetReceiverTransport(
      RefPtr<TransportInterface> aTransport) override;

  /**
   * Function to deliver externally captured audio sample for encoding and
   * transport
   * @param audioData [in]: Pointer to array containing a frame of audio
   * @param lengthSamples [in]: Length of audio frame in samples in multiple of
   *                             10 milliseconds
   *                             Ex: Frame length is 160, 320, 440 for 16, 32,
   *                             44 kHz sampling rates respectively.
   *                             audioData[] should be of lengthSamples in
   *                             size say, for 16kz sampling rate,
   *                             audioData[] should contain 160 samples of
   *                             16-bits each for a 10m audio frame.
   * @param samplingFreqHz [in]: Frequency/rate of the sampling in Hz
   *                             ( 16000, 32000 ...)
   * @param capture_delay [in]:  Approx Delay from recording until it is
   *                             delivered to VoiceEngine in milliseconds.
   * NOTE: ConfigureSendMediaCodec() SHOULD be called before this function can
   * be invoked. This ensures the inserted audio-samples can be transmitted by
   * the conduit
   */
  MediaConduitErrorCode SendAudioFrame(const int16_t speechData[],
                                       int32_t lengthSamples,
                                       int32_t samplingFreqHz,
                                       uint32_t channels,
                                       int32_t capture_time) override;

  /**
   * Function to grab a decoded audio-sample from the media engine for
   * rendering / playoutof length 10 milliseconds.
   *
   * @param speechData [in]: Pointer to a array to which a 10ms frame of audio
   *                         will be copied
   * @param samplingFreqHz [in]: Frequency of the sampling for playback in
   *                             Hertz (16000, 32000,..)
   * @param capture_delay [in]: Estimated Time between reading of the samples
   *                            to rendering/playback
   * @param lengthSamples [in]: Contain maximum length of speechData array.
   * @param lengthSamples [out]: Will contain length of the audio frame in
   *                             samples at return.
   *                             Ex: A value of 160 implies 160 samples each of
   *                             16-bits was copied into speechData
   * NOTE: This function should be invoked every 10 milliseconds for the best
   * peformance
   * NOTE: ConfigureRecvMediaCodec() SHOULD be called before this function can
   * be invoked
   * This ensures the decoded samples are ready for reading and playout is
   * enabled.
   */
  MediaConduitErrorCode GetAudioFrame(int16_t speechData[],
                                      int32_t samplingFreqHz,
                                      int32_t capture_delay,
                                      int& lengthSamples) override;

  /**
   * Webrtc transport implementation to send and receive RTP packet.
   * AudioConduit registers itself as ExternalTransport to the VoiceEngine
   */
  bool SendRtp(const uint8_t* data, size_t len,
               const webrtc::PacketOptions& options) override;

  /**
   * Webrtc transport implementation to send and receive RTCP packet.
   * AudioConduit registers itself as ExternalTransport to the VoiceEngine
   */
<<<<<<< HEAD
  bool SendRtcp(const uint8_t* data, size_t len) override;

  uint64_t CodecPluginID() override { return 0; }
  void SetPCHandle(const std::string& aPCHandle) override {}
  MediaConduitErrorCode DeliverPacket(const void* data, int len) override;

  void DeleteStreams() override {}

  WebrtcAudioConduit(RefPtr<WebRtcCallWrapper> aCall,
                     nsCOMPtr<nsIEventTarget> aStsThread)
      : mFakeAudioDevice(new webrtc::FakeAudioDeviceModule()),
        mTransportMonitor("WebrtcAudioConduit"),
        mTransmitterTransport(nullptr),
        mReceiverTransport(nullptr),
        mCall(aCall),
        mRecvStreamConfig(),
        mRecvStream(nullptr),
        mSendStreamConfig(
            this)  // 'this' is stored but not  dereferenced in the constructor.
        ,
        mSendStream(nullptr),
        mRecvSSRC(0),
        mEngineTransmitting(false),
        mEngineReceiving(false),
        mRecvChannel(-1),
        mSendChannel(-1),
        mDtmfEnabled(false),
        mMutex("WebrtcAudioConduit::mMutex"),
        mCaptureDelay(150),
        mSamples(0),
        mLastSyncLog(0),
        mStsThread(aStsThread) {}
||||||| merged common ancestors
  virtual bool SendRtcp(const uint8_t *data,
                        size_t len) override;

  virtual uint64_t CodecPluginID() override { return 0; }
  virtual void SetPCHandle(const std::string& aPCHandle) override {}

  virtual void DeleteStreams() override {}

  explicit WebrtcAudioConduit():
                      mVoiceEngine(nullptr),
                      mFakeAudioDevice(new webrtc::FakeAudioDeviceModule()),
                      mTransportMonitor("WebrtcAudioConduit"),
                      mTransmitterTransport(nullptr),
                      mReceiverTransport(nullptr),
                      mEngineTransmitting(false),
                      mEngineReceiving(false),
                      mChannel(-1),
                      mDtmfEnabled(false),
                      mCodecMutex("AudioConduit codec db"),
                      mCaptureDelay(150),
                      mSamples(0),
                      mLastSyncLog(0)
  {
  }
=======
  bool SendRtcp(const uint8_t* data, size_t len) override;

  uint64_t CodecPluginID() override { return 0; }
  void SetPCHandle(const std::string& aPCHandle) override {}
  MediaConduitErrorCode DeliverPacket(const void* data, int len) override;

  void DeleteStreams() override {}

  WebrtcAudioConduit(RefPtr<WebRtcCallWrapper> aCall,
                     nsCOMPtr<nsIEventTarget> aStsThread)
      : mTransportMonitor("WebrtcAudioConduit"),
        mTransmitterTransport(nullptr),
        mReceiverTransport(nullptr),
        mCall(aCall),
        mRecvStreamConfig(),
        mRecvStream(nullptr),
        mSendStreamConfig(
            this)  // 'this' is stored but not  dereferenced in the constructor.
        ,
        mSendStream(nullptr),
        mRecvSSRC(0),
        mEngineTransmitting(false),
        mEngineReceiving(false),
        mRecvChannel(-1),
        mSendChannel(-1),
        mDtmfEnabled(false),
        mMutex("WebrtcAudioConduit::mMutex"),
        mStsThread(aStsThread) {}
>>>>>>> upstream-releases

  virtual ~WebrtcAudioConduit();

  virtual MediaConduitErrorCode Init();

  int GetRecvChannel() { return mRecvChannel; }
  webrtc::VoiceEngine* GetVoiceEngine() {
    return mCall->Call()->voice_engine();
  }

  /* Set Local SSRC list.
   * Note: Until the refactor of the VoE into the call API is complete
   *   this list should contain only a single ssrc.
   */
  bool SetLocalSSRCs(const std::vector<unsigned int>& aSSRCs) override;
  std::vector<unsigned int> GetLocalSSRCs() override;
  bool SetRemoteSSRC(unsigned int ssrc) override;
  bool UnsetRemoteSSRC(uint32_t ssrc) override { return true; }
  bool GetRemoteSSRC(unsigned int* ssrc) override;
  bool SetLocalCNAME(const char* cname) override;
  bool SetLocalMID(const std::string& mid) override;

  void SetSyncGroup(const std::string& group) override;

  bool GetSendPacketTypeStats(
      webrtc::RtcpPacketTypeCounter* aPacketCounts) override;

  bool GetRecvPacketTypeStats(
      webrtc::RtcpPacketTypeCounter* aPacketCounts) override;

<<<<<<< HEAD
  bool GetVideoEncoderStats(double* framerateMean, double* framerateStdDev,
                            double* bitrateMean, double* bitrateStdDev,
                            uint32_t* droppedFrames,
                            uint32_t* framesEncoded) override {
    return false;
  }
  bool GetVideoDecoderStats(double* framerateMean, double* framerateStdDev,
                            double* bitrateMean, double* bitrateStdDev,
                            uint32_t* discardedPackets,
                            uint32_t* framesDecoded) override {
    return false;
  }
  bool GetAVStats(int32_t* jitterBufferDelayMs, int32_t* playoutBufferDelayMs,
                  int32_t* avSyncOffsetMs) override;
  bool GetRTPStats(unsigned int* jitterMs,
                   unsigned int* cumulativeLost) override;
  bool GetRTCPReceiverReport(uint32_t* jitterMs, uint32_t* packetsReceived,
                             uint64_t* bytesReceived, uint32_t* cumulativeLost,
                             int32_t* rttMs) override;
  bool GetRTCPSenderReport(unsigned int* packetsSent,
||||||| merged common ancestors
  bool GetVideoEncoderStats(double* framerateMean,
                            double* framerateStdDev,
                            double* bitrateMean,
                            double* bitrateStdDev,
                            uint32_t* droppedFrames,
                            uint32_t* framesEncoded) override
  {
    return false;
  }
  bool GetVideoDecoderStats(double* framerateMean,
                            double* framerateStdDev,
                            double* bitrateMean,
                            double* bitrateStdDev,
                            uint32_t* discardedPackets,
                            uint32_t* framesDecoded) override
  {
    return false;
  }
  bool GetAVStats(int32_t* jitterBufferDelayMs,
                  int32_t* playoutBufferDelayMs,
                  int32_t* avSyncOffsetMs) override;
  bool GetRTPStats(unsigned int* jitterMs, unsigned int* cumulativeLost) override;
  bool GetRTCPReceiverReport(DOMHighResTimeStamp* timestamp,
                             uint32_t* jitterMs,
                             uint32_t* packetsReceived,
                             uint64_t* bytesReceived,
                             uint32_t *cumulativeLost,
                             int32_t* rttMs) override;
  bool GetRTCPSenderReport(DOMHighResTimeStamp* timestamp,
                           unsigned int* packetsSent,
=======
  bool GetRTPReceiverStats(unsigned int* jitterMs,
                           unsigned int* cumulativeLost) override;
  bool GetRTCPReceiverReport(uint32_t* jitterMs, uint32_t* packetsReceived,
                             uint64_t* bytesReceived, uint32_t* cumulativeLost,
                             Maybe<double>* aOutRttSec) override;
  bool GetRTCPSenderReport(unsigned int* packetsSent,
>>>>>>> upstream-releases
                           uint64_t* bytesSent) override;

  bool SetDtmfPayloadType(unsigned char type, int freq) override;

  bool InsertDTMFTone(int channel, int eventCode, bool outOfBand, int lengthMs,
                      int attenuationDb) override;

  void GetRtpSources(const int64_t aTimeNow,
                     nsTArray<dom::RTCRtpSourceEntry>& outSources) override;

<<<<<<< HEAD
  void OnRtpPacket(const webrtc::WebRtcRTPHeader* aRtpHeader,
                   const int64_t aTimestamp, const uint32_t aJitter) override;
||||||| merged common ancestors
  void OnRtpPacket(const webrtc::WebRtcRTPHeader* aRtpHeader,
                   const int64_t aTimestamp,
                   const uint32_t aJitter) override;
=======
  void OnRtpPacket(const webrtc::RTPHeader& aRtpHeader,
                   const int64_t aTimestamp, const uint32_t aJitter) override;
>>>>>>> upstream-releases

  // test-only: inserts fake CSRCs and audio level data
  void InsertAudioLevelForContributingSource(uint32_t aSource,
                                             int64_t aTimestamp, bool aHasLevel,
                                             uint8_t aLevel);

  bool IsSamplingFreqSupported(int freq) const override;

<<<<<<< HEAD
 protected:
  // These are protected so they can be accessed by unit tests
  std::unique_ptr<webrtc::voe::ChannelProxy> mRecvChannelProxy = nullptr;
  std::unique_ptr<webrtc::voe::ChannelProxy> mSendChannelProxy = nullptr;

 private:
||||||| merged common ancestors
private:
=======
 protected:
  // These are protected so they can be accessed by unit tests

  // Written only on main thread. Accessed from audio thread.
  // Accessed from mStsThread during stats calls.
  // This is safe, provided audio and stats calls stop before we
  // destroy the AudioConduit.
  std::unique_ptr<webrtc::voe::ChannelProxy> mRecvChannelProxy = nullptr;

  // Written only on main thread. Accessed from mStsThread during stats calls.
  // This is safe, provided stats calls stop before we destroy the
  // AudioConduit.
  std::unique_ptr<webrtc::voe::ChannelProxy> mSendChannelProxy = nullptr;

 private:
>>>>>>> upstream-releases
  WebrtcAudioConduit(const WebrtcAudioConduit& other) = delete;
  void operator=(const WebrtcAudioConduit& other) = delete;

  // Function to convert between WebRTC and Conduit codec structures
  bool CodecConfigToWebRTCCodec(const AudioCodecConfig* codecInfo,
                                webrtc::AudioSendStream::Config& config);

  // Generate block size in sample lenght for a given sampling frequency
  unsigned int GetNum10msSamplesForFrequency(int samplingFreqHz) const;

  // Checks the codec to be applied
  MediaConduitErrorCode ValidateCodecConfig(const AudioCodecConfig* codecInfo,
                                            bool send);

  MediaConduitErrorCode CreateSendStream();
  void DeleteSendStream();
  MediaConduitErrorCode CreateRecvStream();
  void DeleteRecvStream();

  bool RecreateSendStreamIfExists();
  bool RecreateRecvStreamIfExists();

  MediaConduitErrorCode CreateChannels();
  virtual void DeleteChannels();

<<<<<<< HEAD
  UniquePtr<webrtc::FakeAudioDeviceModule> mFakeAudioDevice;
||||||| merged common ancestors
  webrtc::VoiceEngine* mVoiceEngine;
  UniquePtr<webrtc::FakeAudioDeviceModule> mFakeAudioDevice;
=======
>>>>>>> upstream-releases
  mozilla::ReentrantMonitor mTransportMonitor;

  // Accessed on any thread under mTransportMonitor.
  RefPtr<TransportInterface> mTransmitterTransport;

  // Accessed on any thread under mTransportMonitor.
  RefPtr<TransportInterface> mReceiverTransport;
<<<<<<< HEAD
  ScopedCustomReleasePtr<webrtc::VoEBase> mPtrVoEBase;

  const RefPtr<WebRtcCallWrapper> mCall;
  webrtc::AudioReceiveStream::Config mRecvStreamConfig;
  webrtc::AudioReceiveStream* mRecvStream;
  webrtc::AudioSendStream::Config mSendStreamConfig;
  webrtc::AudioSendStream* mSendStream;

  // accessed on creation, and when receiving packets
  Atomic<uint32_t> mRecvSSRC;  // this can change during a stream!
  RtpPacketQueue mRtpPacketQueue;

  // engine states of our interets
  mozilla::Atomic<bool>
      mEngineTransmitting;  // If true => VoiceEngine Send-subsystem is up
  mozilla::Atomic<bool>
      mEngineReceiving;  // If true => VoiceEngine Receive-subsystem is up
                         // and playout is enabled
  // Keep track of each inserted RTP block and the time it was inserted
  // so we can estimate the clock time for a specific TimeStamp coming out
  // (for when we send data to MediaStreamTracks).  Blocks are aged out as
  // needed.
  struct Processing {
    TimeStamp mTimeStamp;
    uint32_t mRTPTimeStamp;  // RTP timestamps received
  };
  AutoTArray<Processing, 8> mProcessing;

  int mRecvChannel;
  int mSendChannel;
  bool mDtmfEnabled;
||||||| merged common ancestors
  ScopedCustomReleasePtr<webrtc::VoENetwork>   mPtrVoENetwork;
  ScopedCustomReleasePtr<webrtc::VoEBase>      mPtrVoEBase;
  ScopedCustomReleasePtr<webrtc::VoECodec>     mPtrVoECodec;
  ScopedCustomReleasePtr<webrtc::VoEExternalMedia> mPtrVoEXmedia;
  ScopedCustomReleasePtr<webrtc::VoEAudioProcessing> mPtrVoEProcessing;
  ScopedCustomReleasePtr<webrtc::VoEVideoSync> mPtrVoEVideoSync;
  ScopedCustomReleasePtr<webrtc::VoERTP_RTCP>  mPtrVoERTP_RTCP;
  ScopedCustomReleasePtr<webrtc::VoERTP_RTCP>  mPtrRTP;
  //engine states of our interets
  mozilla::Atomic<bool> mEngineTransmitting; // If true => VoiceEngine Send-subsystem is up
  mozilla::Atomic<bool> mEngineReceiving;    // If true => VoiceEngine Receive-subsystem is up
                            // and playout is enabled
  // Keep track of each inserted RTP block and the time it was inserted
  // so we can estimate the clock time for a specific TimeStamp coming out
  // (for when we send data to MediaStreamTracks).  Blocks are aged out as needed.
  struct Processing {
    TimeStamp mTimeStamp;
    uint32_t mRTPTimeStamp; // RTP timestamps received
  };
  AutoTArray<Processing,8> mProcessing;

  int mChannel;
  std::unique_ptr<webrtc::voe::ChannelProxy> mChannelProxy;
  bool mDtmfEnabled;
  RecvCodecList    mRecvCodecList;
=======
>>>>>>> upstream-releases

<<<<<<< HEAD
  Mutex mMutex;
  nsAutoPtr<AudioCodecConfig> mCurSendCodecConfig;
||||||| merged common ancestors
  Mutex mCodecMutex; // protects mCurSendCodecConfig
  nsAutoPtr<AudioCodecConfig> mCurSendCodecConfig;
=======
  // Accessed from main thread and audio threads. Used to create and destroy
  // channels and to send audio data. Access to channels is protected by
  // locking in channel.cc.
  ScopedCustomReleasePtr<webrtc::VoEBase> mPtrVoEBase;

  // Const so can be accessed on any thread. Most methods are called on
  // main thread.
  const RefPtr<WebRtcCallWrapper> mCall;

  // Written only on main thread. Guarded by mMutex, except for reads on main.
  webrtc::AudioReceiveStream::Config mRecvStreamConfig;

  // Written only on main thread. Guarded by mMutex, except for reads on main.
  webrtc::AudioReceiveStream* mRecvStream;

  // Written only on main thread. Guarded by mMutex, except for reads on main.
  webrtc::AudioSendStream::Config mSendStreamConfig;

  // Written only on main thread. Guarded by mMutex, except for reads on main.
  webrtc::AudioSendStream* mSendStream;
>>>>>>> upstream-releases

  // accessed on creation, and when receiving packets
  Atomic<uint32_t> mRecvSSRC;  // this can change during a stream!

<<<<<<< HEAD
  webrtc::AudioFrame mAudioFrame;  // for output pulls
||||||| merged common ancestors
  webrtc::AudioFrame mAudioFrame; // for output pulls
=======
  // Accessed only on mStsThread.
  RtpPacketQueue mRtpPacketQueue;
>>>>>>> upstream-releases

  // engine states of our interets
  mozilla::Atomic<bool>
      mEngineTransmitting;  // If true => VoiceEngine Send-subsystem is up
  mozilla::Atomic<bool>
      mEngineReceiving;  // If true => VoiceEngine Receive-subsystem is up
                         // and playout is enabled

  // Accessed only on main thread.
  int mRecvChannel;

  // Accessed on main thread and from audio thread.
  int mSendChannel;

  // Accessed only on main thread.
  bool mDtmfEnabled;

  Mutex mMutex;

  // Accessed from audio thread.
  webrtc::AudioFrame mAudioFrame;  // for output pulls

  // Accessed from both main and mStsThread. Uses locks internally.
  RtpSourceObserver mRtpSourceObserver;
<<<<<<< HEAD

  // Socket transport service thread. Any thread.
  const nsCOMPtr<nsIEventTarget> mStsThread;
||||||| merged common ancestors
=======

  // Socket transport service thread. Any thread.
  const nsCOMPtr<nsIEventTarget> mStsThread;

  // Accessed from mStsThread. Last successfully polled RTT
  Maybe<DOMHighResTimeStamp> mRttSec;
>>>>>>> upstream-releases
};

}  // namespace mozilla

#endif
