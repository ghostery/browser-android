/* -*- Mode: C++; tab-width: 2; indent-tabs-mode: nil; c-basic-offset: 2 -*- */
/* This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/. */

#ifndef GMPVideoEncoderChild_h_
#define GMPVideoEncoderChild_h_

#include "nsString.h"
#include "mozilla/gmp/PGMPVideoEncoderChild.h"
#include "gmp-video-encode.h"
#include "GMPSharedMemManager.h"
#include "GMPVideoHost.h"

namespace mozilla {
namespace gmp {

class GMPContentChild;

class GMPVideoEncoderChild : public PGMPVideoEncoderChild,
                             public GMPVideoEncoderCallback,
<<<<<<< HEAD
                             public GMPSharedMemManager {
 public:
||||||| merged common ancestors
                             public GMPSharedMemManager
{
public:
=======
                             public GMPSharedMemManager {
  friend class PGMPVideoEncoderChild;

 public:
>>>>>>> upstream-releases
  NS_INLINE_DECL_THREADSAFE_REFCOUNTING(GMPVideoEncoderChild);

  explicit GMPVideoEncoderChild(GMPContentChild* aPlugin);

  void Init(GMPVideoEncoder* aEncoder);
  GMPVideoHostImpl& Host();

  // GMPVideoEncoderCallback
  void Encoded(GMPVideoEncodedFrame* aEncodedFrame,
               const uint8_t* aCodecSpecificInfo,
               uint32_t aCodecSpecificInfoLength) override;
  void Error(GMPErr aError) override;

  // GMPSharedMemManager
  bool Alloc(size_t aSize, Shmem::SharedMemory::SharedMemoryType aType,
<<<<<<< HEAD
             Shmem* aMem) override;
  void Dealloc(Shmem& aMem) override;
||||||| merged common ancestors
    Shmem* aMem) override;
  void Dealloc(Shmem& aMem) override;
=======
             Shmem* aMem) override;
  void Dealloc(Shmem&& aMem) override;
>>>>>>> upstream-releases

 private:
  virtual ~GMPVideoEncoderChild();

  // PGMPVideoEncoderChild
<<<<<<< HEAD
  mozilla::ipc::IPCResult RecvInitEncode(
      const GMPVideoCodec& aCodecSettings,
      InfallibleTArray<uint8_t>&& aCodecSpecific, const int32_t& aNumberOfCores,
      const uint32_t& aMaxPayloadSize) override;
  mozilla::ipc::IPCResult RecvEncode(
      const GMPVideoi420FrameData& aInputFrame,
      InfallibleTArray<uint8_t>&& aCodecSpecificInfo,
      InfallibleTArray<GMPVideoFrameType>&& aFrameTypes) override;
  mozilla::ipc::IPCResult RecvChildShmemForPool(
      Shmem&& aEncodedBuffer) override;
  mozilla::ipc::IPCResult RecvSetChannelParameters(
      const uint32_t& aPacketLoss, const uint32_t& aRTT) override;
||||||| merged common ancestors
  mozilla::ipc::IPCResult RecvInitEncode(const GMPVideoCodec& aCodecSettings,
                                         InfallibleTArray<uint8_t>&& aCodecSpecific,
                                         const int32_t& aNumberOfCores,
                                         const uint32_t& aMaxPayloadSize) override;
  mozilla::ipc::IPCResult RecvEncode(const GMPVideoi420FrameData& aInputFrame,
                                     InfallibleTArray<uint8_t>&& aCodecSpecificInfo,
                                     InfallibleTArray<GMPVideoFrameType>&& aFrameTypes) override;
  mozilla::ipc::IPCResult RecvChildShmemForPool(Shmem&& aEncodedBuffer) override;
  mozilla::ipc::IPCResult RecvSetChannelParameters(const uint32_t& aPacketLoss,
                                                   const uint32_t& aRTT) override;
=======
  mozilla::ipc::IPCResult RecvInitEncode(
      const GMPVideoCodec& aCodecSettings,
      InfallibleTArray<uint8_t>&& aCodecSpecific, const int32_t& aNumberOfCores,
      const uint32_t& aMaxPayloadSize);
  mozilla::ipc::IPCResult RecvEncode(
      const GMPVideoi420FrameData& aInputFrame,
      InfallibleTArray<uint8_t>&& aCodecSpecificInfo,
      InfallibleTArray<GMPVideoFrameType>&& aFrameTypes);
  mozilla::ipc::IPCResult RecvChildShmemForPool(Shmem&& aEncodedBuffer);
  mozilla::ipc::IPCResult RecvSetChannelParameters(const uint32_t& aPacketLoss,
                                                   const uint32_t& aRTT);
>>>>>>> upstream-releases
  mozilla::ipc::IPCResult RecvSetRates(const uint32_t& aNewBitRate,
<<<<<<< HEAD
                                       const uint32_t& aFrameRate) override;
  mozilla::ipc::IPCResult RecvSetPeriodicKeyFrames(
      const bool& aEnable) override;
  mozilla::ipc::IPCResult RecvEncodingComplete() override;
||||||| merged common ancestors
                                       const uint32_t& aFrameRate) override;
  mozilla::ipc::IPCResult RecvSetPeriodicKeyFrames(const bool& aEnable) override;
  mozilla::ipc::IPCResult RecvEncodingComplete() override;
=======
                                       const uint32_t& aFrameRate);
  mozilla::ipc::IPCResult RecvSetPeriodicKeyFrames(const bool& aEnable);
  mozilla::ipc::IPCResult RecvEncodingComplete();
>>>>>>> upstream-releases

  GMPContentChild* mPlugin;
  GMPVideoEncoder* mVideoEncoder;
  GMPVideoHostImpl mVideoHost;

  // Non-zero when a GMP is blocked spinning the IPC message loop while
  // waiting on an NeedShmem to complete.
  int mNeedShmemIntrCount;
  bool mPendingEncodeComplete;
};

}  // namespace gmp
}  // namespace mozilla

#endif  // GMPVideoEncoderChild_h_
