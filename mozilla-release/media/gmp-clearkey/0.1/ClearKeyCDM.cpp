#include "ClearKeyCDM.h"

#include "ClearKeyUtils.h"

using namespace cdm;

<<<<<<< HEAD
ClearKeyCDM::ClearKeyCDM(Host_9* aHost) {
||||||| merged common ancestors
ClearKeyCDM::ClearKeyCDM(Host_9* aHost)
{
=======
ClearKeyCDM::ClearKeyCDM(Host_10* aHost) {
>>>>>>> upstream-releases
  mHost = aHost;
  mSessionManager = new ClearKeySessionManager(mHost);
}

<<<<<<< HEAD
void ClearKeyCDM::Initialize(bool aAllowDistinctiveIdentifier,
                             bool aAllowPersistentState) {
  mSessionManager->Init(aAllowDistinctiveIdentifier, aAllowPersistentState);
||||||| merged common ancestors
void
ClearKeyCDM::Initialize(bool aAllowDistinctiveIdentifier,
                        bool aAllowPersistentState)
{
  mSessionManager->Init(aAllowDistinctiveIdentifier,
                        aAllowPersistentState);
=======
void ClearKeyCDM::Initialize(bool aAllowDistinctiveIdentifier,
                             bool aAllowPersistentState,
                             bool aUseHardwareSecureCodecs) {
  mSessionManager->Init(aAllowDistinctiveIdentifier, aAllowPersistentState);
  // We call mHost->OnInitialized() in the session manager once it has
  // initialized.
>>>>>>> upstream-releases
}

void ClearKeyCDM::GetStatusForPolicy(uint32_t aPromiseId,
                                     const Policy& aPolicy) {
  // MediaKeys::GetStatusForPolicy checks the keysystem and
  // reject the promise with NS_ERROR_DOM_NOT_SUPPORTED_ERR without calling CDM.
  // This function should never be called and is not supported.
  assert(false);
}
void ClearKeyCDM::SetServerCertificate(uint32_t aPromiseId,
                                       const uint8_t* aServerCertificateData,
                                       uint32_t aServerCertificateDataSize) {
  mSessionManager->SetServerCertificate(aPromiseId, aServerCertificateData,
                                        aServerCertificateDataSize);
}

void ClearKeyCDM::CreateSessionAndGenerateRequest(uint32_t aPromiseId,
                                                  SessionType aSessionType,
                                                  InitDataType aInitDataType,
                                                  const uint8_t* aInitData,
                                                  uint32_t aInitDataSize) {
  mSessionManager->CreateSession(aPromiseId, aInitDataType, aInitData,
                                 aInitDataSize, aSessionType);
}

void ClearKeyCDM::LoadSession(uint32_t aPromiseId, SessionType aSessionType,
                              const char* aSessionId, uint32_t aSessionIdSize) {
  mSessionManager->LoadSession(aPromiseId, aSessionId, aSessionIdSize);
}

void ClearKeyCDM::UpdateSession(uint32_t aPromiseId, const char* aSessionId,
                                uint32_t aSessionIdSize,
                                const uint8_t* aResponse,
                                uint32_t aResponseSize) {
  mSessionManager->UpdateSession(aPromiseId, aSessionId, aSessionIdSize,
                                 aResponse, aResponseSize);
}

void ClearKeyCDM::CloseSession(uint32_t aPromiseId, const char* aSessionId,
                               uint32_t aSessionIdSize) {
  mSessionManager->CloseSession(aPromiseId, aSessionId, aSessionIdSize);
}

void ClearKeyCDM::RemoveSession(uint32_t aPromiseId, const char* aSessionId,
                                uint32_t aSessionIdSize) {
  mSessionManager->RemoveSession(aPromiseId, aSessionId, aSessionIdSize);
}

void ClearKeyCDM::TimerExpired(void* aContext) {
  // Clearkey is not interested in timers, so this method has not been
  // implemented.
  assert(false);
}

<<<<<<< HEAD
Status ClearKeyCDM::Decrypt(const InputBuffer_1& aEncryptedBuffer,
                            DecryptedBlock* aDecryptedBuffer) {
||||||| merged common ancestors
Status
ClearKeyCDM::Decrypt(const InputBuffer_1& aEncryptedBuffer,
                     DecryptedBlock* aDecryptedBuffer)
{
=======
Status ClearKeyCDM::Decrypt(const InputBuffer_2& aEncryptedBuffer,
                            DecryptedBlock* aDecryptedBuffer) {
>>>>>>> upstream-releases
  return mSessionManager->Decrypt(aEncryptedBuffer, aDecryptedBuffer);
}

<<<<<<< HEAD
Status ClearKeyCDM::InitializeAudioDecoder(
    const AudioDecoderConfig_1& aAudioDecoderConfig) {
||||||| merged common ancestors
Status
ClearKeyCDM::InitializeAudioDecoder(
  const AudioDecoderConfig_1& aAudioDecoderConfig)
{
=======
Status ClearKeyCDM::InitializeAudioDecoder(
    const AudioDecoderConfig_2& aAudioDecoderConfig) {
>>>>>>> upstream-releases
  // Audio decoding is not supported by Clearkey because Widevine doesn't
  // support it and Clearkey's raison d'etre is to provide test coverage
  // for paths that Widevine will exercise in the wild.
  return Status::kDecodeError;
}

<<<<<<< HEAD
Status ClearKeyCDM::InitializeVideoDecoder(
    const VideoDecoderConfig_1& aVideoDecoderConfig) {
||||||| merged common ancestors
Status
ClearKeyCDM::InitializeVideoDecoder(
  const VideoDecoderConfig_1& aVideoDecoderConfig)
{
=======
Status ClearKeyCDM::InitializeVideoDecoder(
    const VideoDecoderConfig_2& aVideoDecoderConfig) {
>>>>>>> upstream-releases
#ifdef ENABLE_WMF
  mVideoDecoder = new VideoDecoder(mHost);
  return mVideoDecoder->InitDecode(aVideoDecoderConfig);
#else
  return Status::kDecodeError;
#endif
}

void ClearKeyCDM::DeinitializeDecoder(StreamType aDecoderType) {
#ifdef ENABLE_WMF
  if (aDecoderType == StreamType::kStreamTypeVideo) {
    mVideoDecoder->DecodingComplete();
    mVideoDecoder = nullptr;
  }
#endif
}

void ClearKeyCDM::ResetDecoder(StreamType aDecoderType) {
#ifdef ENABLE_WMF
  if (aDecoderType == StreamType::kStreamTypeVideo) {
    mVideoDecoder->Reset();
  }
#endif
}

<<<<<<< HEAD
Status ClearKeyCDM::DecryptAndDecodeFrame(const InputBuffer_1& aEncryptedBuffer,
                                          VideoFrame* aVideoFrame) {
||||||| merged common ancestors
Status
ClearKeyCDM::DecryptAndDecodeFrame(const InputBuffer_1& aEncryptedBuffer,
                                   VideoFrame* aVideoFrame)
{
=======
Status ClearKeyCDM::DecryptAndDecodeFrame(const InputBuffer_2& aEncryptedBuffer,
                                          VideoFrame* aVideoFrame) {
>>>>>>> upstream-releases
#ifdef ENABLE_WMF
  return mVideoDecoder->Decode(aEncryptedBuffer, aVideoFrame);
#else
  return Status::kDecodeError;
#endif
}

<<<<<<< HEAD
Status ClearKeyCDM::DecryptAndDecodeSamples(
    const InputBuffer_1& aEncryptedBuffer, AudioFrames* aAudioFrame) {
||||||| merged common ancestors
Status
ClearKeyCDM::DecryptAndDecodeSamples(const InputBuffer_1& aEncryptedBuffer,
                                     AudioFrames* aAudioFrame)
{
=======
Status ClearKeyCDM::DecryptAndDecodeSamples(
    const InputBuffer_2& aEncryptedBuffer, AudioFrames* aAudioFrame) {
>>>>>>> upstream-releases
  // Audio decoding is not supported by Clearkey because Widevine doesn't
  // support it and Clearkey's raison d'etre is to provide test coverage
  // for paths that Widevine will exercise in the wild.
  return Status::kDecodeError;
}

void ClearKeyCDM::OnPlatformChallengeResponse(
    const PlatformChallengeResponse& aResponse) {
  // This function should never be called and is not supported.
  assert(false);
}

void ClearKeyCDM::OnQueryOutputProtectionStatus(
    QueryResult aResult, uint32_t aLinkMask, uint32_t aOutputProtectionMask) {
  // This function should never be called and is not supported.
  assert(false);
}

void ClearKeyCDM::OnStorageId(uint32_t aVersion, const uint8_t* aStorageId,
                              uint32_t aStorageIdSize) {
  // This function should never be called and is not supported.
  assert(false);
}

void ClearKeyCDM::Destroy() {
  mSessionManager->DecryptingComplete();
#ifdef ENABLE_WMF
  // If we have called 'DeinitializeDecoder' mVideoDecoder will be null.
  if (mVideoDecoder) {
    mVideoDecoder->DecodingComplete();
  }
#endif
  delete this;
}
