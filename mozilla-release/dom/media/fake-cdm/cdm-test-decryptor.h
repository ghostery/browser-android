/* -*- Mode: C++; tab-width: 2; indent-tabs-mode: nil; c-basic-offset: 2 -*- */
/* This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/. */

#ifndef FAKE_DECRYPTOR_H__
#define FAKE_DECRYPTOR_H__

#include "content_decryption_module.h"
#include <string>
#include "mozilla/Attributes.h"

<<<<<<< HEAD
class FakeDecryptor : public cdm::ContentDecryptionModule_9 {
 public:
  explicit FakeDecryptor(cdm::Host_9* aHost);
||||||| merged common ancestors
class FakeDecryptor : public cdm::ContentDecryptionModule_9 {
public:
  explicit FakeDecryptor(cdm::Host_9* aHost);
=======
class FakeDecryptor : public cdm::ContentDecryptionModule_10 {
 public:
  explicit FakeDecryptor(cdm::Host_10* aHost);
>>>>>>> upstream-releases

<<<<<<< HEAD
  void Initialize(bool aAllowDistinctiveIdentifier,
                  bool aAllowPersistentState) override {}
||||||| merged common ancestors
  void Initialize(bool aAllowDistinctiveIdentifier,
                  bool aAllowPersistentState) override
  {
  }
=======
  void Initialize(bool aAllowDistinctiveIdentifier, bool aAllowPersistentState,
                  bool aUseHardwareSecureCodecs) override {
    mHost->OnInitialized(true);
  }
>>>>>>> upstream-releases

  void GetStatusForPolicy(uint32_t aPromiseId,
                          const cdm::Policy& aPolicy) override {}

  void SetServerCertificate(uint32_t aPromiseId,
                            const uint8_t* aServerCertificateData,
                            uint32_t aServerCertificateDataSize) override {}

  void CreateSessionAndGenerateRequest(uint32_t aPromiseId,
                                       cdm::SessionType aSessionType,
                                       cdm::InitDataType aInitDataType,
                                       const uint8_t* aInitData,
                                       uint32_t aInitDataSize) override {}

  void LoadSession(uint32_t aPromiseId, cdm::SessionType aSessionType,
                   const char* aSessionId, uint32_t aSessionIdSize) override {}

  void UpdateSession(uint32_t aPromiseId, const char* aSessionId,
                     uint32_t aSessionIdSize, const uint8_t* aResponse,
                     uint32_t aResponseSize) override;

  void CloseSession(uint32_t aPromiseId, const char* aSessionId,
                    uint32_t aSessionIdSize) override {}

  void RemoveSession(uint32_t aPromiseId, const char* aSessionId,
                     uint32_t aSessionIdSize) override {}

  void TimerExpired(void* aContext) override {}

<<<<<<< HEAD
  cdm::Status Decrypt(const cdm::InputBuffer_1& aEncryptedBuffer,
                      cdm::DecryptedBlock* aDecryptedBuffer) override {
||||||| merged common ancestors
  cdm::Status Decrypt(const cdm::InputBuffer_1& aEncryptedBuffer,
                      cdm::DecryptedBlock* aDecryptedBuffer) override
  {
=======
  cdm::Status Decrypt(const cdm::InputBuffer_2& aEncryptedBuffer,
                      cdm::DecryptedBlock* aDecryptedBuffer) override {
>>>>>>> upstream-releases
    return cdm::Status::kDecodeError;
  }

  cdm::Status InitializeAudioDecoder(
<<<<<<< HEAD
      const cdm::AudioDecoderConfig_1& aAudioDecoderConfig) override {
||||||| merged common ancestors
    const cdm::AudioDecoderConfig_1& aAudioDecoderConfig) override
  {
=======
      const cdm::AudioDecoderConfig_2& aAudioDecoderConfig) override {
>>>>>>> upstream-releases
    return cdm::Status::kDecodeError;
  }

  cdm::Status InitializeVideoDecoder(
<<<<<<< HEAD
      const cdm::VideoDecoderConfig_1& aVideoDecoderConfig) override {
||||||| merged common ancestors
    const cdm::VideoDecoderConfig_1& aVideoDecoderConfig) override
  {
=======
      const cdm::VideoDecoderConfig_2& aVideoDecoderConfig) override {
>>>>>>> upstream-releases
    return cdm::Status::kDecodeError;
  }

  void DeinitializeDecoder(cdm::StreamType aDecoderType) override {}

  void ResetDecoder(cdm::StreamType aDecoderType) override {}

<<<<<<< HEAD
  cdm::Status DecryptAndDecodeFrame(const cdm::InputBuffer_1& aEncryptedBuffer,
                                    cdm::VideoFrame* aVideoFrame) override {
||||||| merged common ancestors
  cdm::Status DecryptAndDecodeFrame(const cdm::InputBuffer_1& aEncryptedBuffer,
                                    cdm::VideoFrame* aVideoFrame) override
  {
=======
  cdm::Status DecryptAndDecodeFrame(const cdm::InputBuffer_2& aEncryptedBuffer,
                                    cdm::VideoFrame* aVideoFrame) override {
>>>>>>> upstream-releases
    return cdm::Status::kDecodeError;
  }

  cdm::Status DecryptAndDecodeSamples(
<<<<<<< HEAD
      const cdm::InputBuffer_1& aEncryptedBuffer,
      cdm::AudioFrames* aAudioFrame) override {
||||||| merged common ancestors
    const cdm::InputBuffer_1& aEncryptedBuffer,
    cdm::AudioFrames* aAudioFrame) override
  {
=======
      const cdm::InputBuffer_2& aEncryptedBuffer,
      cdm::AudioFrames* aAudioFrame) override {
>>>>>>> upstream-releases
    return cdm::Status::kDecodeError;
  }

  void OnPlatformChallengeResponse(
      const cdm::PlatformChallengeResponse& aResponse) override {}

  void OnQueryOutputProtectionStatus(cdm::QueryResult aResult,
                                     uint32_t aLinkMask,
                                     uint32_t aOutputProtectionMask) override {}

  void OnStorageId(uint32_t aVersion, const uint8_t* aStorageId,
                   uint32_t aStorageIdSize) override {}

  void Destroy() override {
    delete this;
    sInstance = nullptr;
  }

  static void Message(const std::string& aMessage);

  cdm::Host_10* mHost;

  static FakeDecryptor* sInstance;

 private:
  virtual ~FakeDecryptor() {}

  void TestStorage();
};

#endif
