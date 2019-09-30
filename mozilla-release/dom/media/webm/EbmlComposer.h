/* -*- Mode: C++; tab-width: 2; indent-tabs-mode: nil; c-basic-offset: 2 -*-*/
/* This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at http://mozilla.org/MPL/2.0/. */

#ifndef EbmlComposer_h_
#define EbmlComposer_h_
#include "nsTArray.h"
#include "ContainerWriter.h"

namespace mozilla {

/*
 * A WebM muxer helper for package the valid WebM format.
 */
class EbmlComposer {
<<<<<<< HEAD
 public:
  EbmlComposer();
||||||| merged common ancestors
public:
  EbmlComposer();
=======
 public:
  EbmlComposer() = default;
>>>>>>> upstream-releases
  /*
   * Assign the parameters which header requires. These can be called multiple
   * times to change paramter values until GenerateHeader() is called, when this
   * becomes illegal to call again.
   */
<<<<<<< HEAD
  void SetVideoConfig(uint32_t aWidth, uint32_t aHeight, uint32_t aDisplayWidth,
                      uint32_t aDisplayHeight);

||||||| merged common ancestors
  void SetVideoConfig(uint32_t aWidth, uint32_t aHeight,
                      uint32_t aDisplayWidth, uint32_t aDisplayHeight);

=======
  void SetVideoConfig(uint32_t aWidth, uint32_t aHeight, uint32_t aDisplayWidth,
                      uint32_t aDisplayHeight);
>>>>>>> upstream-releases
  void SetAudioConfig(uint32_t aSampleFreq, uint32_t aChannels);
  /*
   * Set the CodecPrivateData for writing in header.
   */
  void SetAudioCodecPrivateData(nsTArray<uint8_t>& aBufs) {
    mCodecPrivateData.AppendElements(aBufs);
  }
  /*
   * Generate the whole WebM header with the configured tracks, and make
   * available to ExtractBuffer. Must only be called once.
   */
  void GenerateHeader();
  /*
   * Insert media encoded buffer into muxer and it would be package
   * into SimpleBlock. If no cluster is opened, new cluster will start for
   * writing.
   */
  void WriteSimpleBlock(EncodedFrame* aFrame);
  /*
   * Get valid cluster data.
   */
  void ExtractBuffer(nsTArray<nsTArray<uint8_t> >* aDestBufs,
                     uint32_t aFlag = 0);
<<<<<<< HEAD

 private:
  // Move the metadata data to mClusterCanFlushBuffs.
  void FinishMetadata();
  // Close current cluster and move data to mClusterCanFlushBuffs.
||||||| merged common ancestors
private:
  // Move the metadata data to mClusterCanFlushBuffs.
  void FinishMetadata();
  // Close current cluster and move data to mClusterCanFlushBuffs.
=======

 private:
  // Close current cluster and move data to mFinishedClusters. Idempotent.
>>>>>>> upstream-releases
  void FinishCluster();
  // Canonical storage of clusters. Each element in the outer array corresponds
  // to a cluster. These are never removed, to keep mClusterHeaderIndex
  // accurate. The payload data in the inner array is however removed. It is
  // moved to mFinishedClusters as a cluster is finished.
  nsTArray<nsTArray<uint8_t> > mClusters;
  // Finished clusters to be flushed out by ExtractBuffer().
  nsTArray<nsTArray<uint8_t> > mFinishedClusters;

<<<<<<< HEAD
  // Indicate the data types in mClusterBuffs.
  enum { FLUSH_NONE = 0, FLUSH_METADATA = 1 << 0, FLUSH_CLUSTER = 1 << 1 };
  uint32_t mFlushState;
  // Indicate the cluster header index in mClusterBuffs.
  uint32_t mClusterHeaderIndex;
||||||| merged common ancestors
  // Indicate the data types in mClusterBuffs.
  enum {
    FLUSH_NONE = 0,
    FLUSH_METADATA = 1 << 0,
    FLUSH_CLUSTER = 1 << 1
  };
  uint32_t mFlushState;
  // Indicate the cluster header index in mClusterBuffs.
  uint32_t mClusterHeaderIndex;
=======
  // Metadata has been serialized.
  bool mMetadataFinished = false;
  // True if we have an open cluster.
  bool mWritingCluster = false;
  // Indicate the current cluster's header index in mClusters.
  size_t mClusterHeaderIndex = 0;
>>>>>>> upstream-releases
  // The cluster length position.
  uint64_t mClusterLengthLoc = 0;
  // Audio codec specific header data.
  nsTArray<uint8_t> mCodecPrivateData;
  // Codec delay in nanoseconds.
  uint64_t mCodecDelay = 0;

  // The timecode of the cluster.
  uint64_t mClusterTimecode = 0;

  // Video configuration
  int mWidth = 0;
  int mHeight = 0;
  int mDisplayWidth = 0;
  int mDisplayHeight = 0;
  bool mHasVideo = false;
  // Audio configuration
  float mSampleFreq = 0;
  int mChannels = 0;
  bool mHasAudio = false;
};

}  // namespace mozilla

#endif
