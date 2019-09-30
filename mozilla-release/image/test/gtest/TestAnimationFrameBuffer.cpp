/* -*- Mode: C++; tab-width: 2; indent-tabs-mode: nil; c-basic-offset: 2 -*-
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/. */

#include "gtest/gtest.h"

#include "mozilla/Move.h"
#include "AnimationFrameBuffer.h"

using namespace mozilla;
using namespace mozilla::image;

<<<<<<< HEAD
static already_AddRefed<imgFrame>
CreateEmptyFrame(const IntSize& aSize = IntSize(1, 1),
                 const IntRect& aFrameRect = IntRect(0, 0, 1, 1),
                 bool aCanRecycle = true)
{
||||||| merged common ancestors
static RawAccessFrameRef
CreateEmptyFrame()
{
=======
static already_AddRefed<imgFrame> CreateEmptyFrame(
    const IntSize& aSize = IntSize(1, 1),
    const IntRect& aFrameRect = IntRect(0, 0, 1, 1), bool aCanRecycle = true) {
>>>>>>> upstream-releases
  RefPtr<imgFrame> frame = new imgFrame();
<<<<<<< HEAD
  AnimationParams animParams { aFrameRect, FrameTimeout::Forever(),
                               /* aFrameNum */ 1, BlendMethod::OVER,
                               DisposalMethod::NOT_SPECIFIED };
  nsresult rv =
    frame->InitForDecoder(aSize, IntRect(IntPoint(0, 0), aSize),
                          SurfaceFormat::B8G8R8A8, 0, false,
                          Some(animParams), true, aCanRecycle);
||||||| merged common ancestors
  nsresult rv = frame->InitForAnimator(nsIntSize(1, 1), SurfaceFormat::B8G8R8A8);
=======
  AnimationParams animParams{aFrameRect, FrameTimeout::Forever(),
                             /* aFrameNum */ 1, BlendMethod::OVER,
                             DisposalMethod::NOT_SPECIFIED};
  nsresult rv = frame->InitForDecoder(aSize, SurfaceFormat::B8G8R8A8, false,
                                      Some(animParams), aCanRecycle);
>>>>>>> upstream-releases
  EXPECT_TRUE(NS_SUCCEEDED(rv));
  RawAccessFrameRef frameRef = frame->RawAccessRef();
  frame->SetRawAccessOnly();
  // Normally the blend animation filter would set the dirty rect, but since
  // we aren't producing an actual animation here, we need to fake it.
  frame->SetDirtyRect(aFrameRect);
  frame->Finish();
  return frame.forget();
}

<<<<<<< HEAD
static void
PrepareForDiscardingQueue(AnimationFrameRetainedBuffer& aQueue)
{
  ASSERT_EQ(size_t(0), aQueue.Size());
  ASSERT_LT(size_t(1), aQueue.Batch());

  AnimationFrameBuffer::InsertStatus status =
    aQueue.Insert(CreateEmptyFrame());
  EXPECT_EQ(AnimationFrameBuffer::InsertStatus::CONTINUE, status);

  while (true) {
    status = aQueue.Insert(CreateEmptyFrame());
    bool restartDecoder = aQueue.AdvanceTo(aQueue.Size() - 1);
    EXPECT_FALSE(restartDecoder);

    if (status == AnimationFrameBuffer::InsertStatus::DISCARD_CONTINUE) {
      break;
    }
    EXPECT_EQ(AnimationFrameBuffer::InsertStatus::CONTINUE, status);
||||||| merged common ancestors
static bool
Fill(AnimationFrameBuffer& buffer, size_t aLength)
{
  bool keepDecoding = false;
  for (size_t i = 0; i < aLength; ++i) {
    RawAccessFrameRef frame = CreateEmptyFrame();
    keepDecoding = buffer.Insert(frame->RawAccessRef());
=======
static bool ReinitForRecycle(RawAccessFrameRef& aFrame) {
  if (!aFrame) {
    return false;
>>>>>>> upstream-releases
  }
<<<<<<< HEAD

  EXPECT_EQ(aQueue.Threshold(), aQueue.Size());
}

static void
VerifyDiscardingQueueContents(AnimationFrameDiscardingQueue& aQueue)
{
  auto frames = aQueue.Display();
  for (auto i : frames) {
    EXPECT_TRUE(i != nullptr);
  }
}

static void
VerifyInsertInternal(AnimationFrameBuffer& aQueue,
                     imgFrame* aFrame)
{
  // Determine the frame index where we just inserted the frame.
  size_t frameIndex;
  if (aQueue.MayDiscard()) {
    const AnimationFrameDiscardingQueue& queue =
      *static_cast<AnimationFrameDiscardingQueue*>(&aQueue);
    frameIndex = queue.PendingInsert() == 0 ? queue.Size() - 1
                                            : queue.PendingInsert() - 1;
  } else {
    ASSERT_FALSE(aQueue.SizeKnown());
    frameIndex = aQueue.Size() - 1;
  }

  // Make sure we can get the frame from that index.
  RefPtr<imgFrame> frame = aQueue.Get(frameIndex, false);
  EXPECT_EQ(aFrame, frame.get());
}

static void
VerifyAdvance(AnimationFrameBuffer& aQueue,
              size_t aExpectedFrame,
              bool aExpectedRestartDecoder)
{
  RefPtr<imgFrame> oldFrame;
  size_t totalRecycled;
  if (aQueue.IsRecycling()) {
    AnimationFrameRecyclingQueue& queue =
      *static_cast<AnimationFrameRecyclingQueue*>(&aQueue);
    oldFrame = queue.Get(queue.Displayed(), false);
    totalRecycled = queue.Recycle().size();
  }

  bool restartDecoder = aQueue.AdvanceTo(aExpectedFrame);
  EXPECT_EQ(aExpectedRestartDecoder, restartDecoder);

  if (aQueue.IsRecycling()) {
    const AnimationFrameRecyclingQueue& queue =
      *static_cast<AnimationFrameRecyclingQueue*>(&aQueue);
    EXPECT_FALSE(queue.Recycle().back().mDirtyRect.IsEmpty());
    EXPECT_TRUE(queue.Recycle().back().mDirtyRect.Contains(oldFrame->GetDirtyRect()));
    EXPECT_EQ(totalRecycled + 1, queue.Recycle().size());
    if (oldFrame->ShouldRecycle()) {
      EXPECT_EQ(oldFrame.get(), queue.Recycle().back().mFrame.get());
    } else {
      EXPECT_EQ(nullptr, queue.Recycle().back().mFrame.get());
    }
  }
}

static void
VerifyInsertAndAdvance(AnimationFrameBuffer& aQueue,
                       size_t aExpectedFrame,
                       AnimationFrameBuffer::InsertStatus aExpectedStatus)
{
  // Insert the decoded frame.
  RefPtr<imgFrame> frame = CreateEmptyFrame();
  AnimationFrameBuffer::InsertStatus status =
    aQueue.Insert(RefPtr<imgFrame>(frame));
  EXPECT_EQ(aExpectedStatus, status);
  EXPECT_TRUE(aQueue.IsLastInsertedFrame(frame));
  VerifyInsertInternal(aQueue, frame);

  // Advance the display frame.
  bool expectedRestartDecoder =
    aExpectedStatus == AnimationFrameBuffer::InsertStatus::YIELD;
  VerifyAdvance(aQueue, aExpectedFrame, expectedRestartDecoder);
||||||| merged common ancestors
  return keepDecoding;
=======

  AnimationParams animParams{aFrame->GetRect(), FrameTimeout::Forever(),
                             /* aFrameNum */ 1, BlendMethod::OVER,
                             DisposalMethod::NOT_SPECIFIED};
  return NS_SUCCEEDED(aFrame->InitForDecoderRecycle(animParams));
>>>>>>> upstream-releases
}

<<<<<<< HEAD
static void
VerifyMarkComplete(AnimationFrameBuffer& aQueue,
                   bool aExpectedContinue,
                   const IntRect& aRefreshArea = IntRect(0, 0, 1, 1))
{
  if (aQueue.IsRecycling() && !aQueue.SizeKnown()) {
    const AnimationFrameRecyclingQueue& queue =
      *static_cast<AnimationFrameRecyclingQueue*>(&aQueue);
    EXPECT_EQ(queue.FirstFrame()->GetRect(),
              queue.FirstFrameRefreshArea());
  }

  bool keepDecoding = aQueue.MarkComplete(aRefreshArea);
  EXPECT_EQ(aExpectedContinue, keepDecoding);

  if (aQueue.IsRecycling()) {
    const AnimationFrameRecyclingQueue& queue =
      *static_cast<AnimationFrameRecyclingQueue*>(&aQueue);
    EXPECT_EQ(aRefreshArea, queue.FirstFrameRefreshArea());
||||||| merged common ancestors
static void
CheckFrames(const AnimationFrameBuffer& buffer, size_t aStart, size_t aEnd, bool aExpected)
{
  for (size_t i = aStart; i < aEnd; ++i) {
    EXPECT_EQ(aExpected, !!buffer.Frames()[i]);
=======
static void PrepareForDiscardingQueue(AnimationFrameRetainedBuffer& aQueue) {
  ASSERT_EQ(size_t(0), aQueue.Size());
  ASSERT_LT(size_t(1), aQueue.Batch());

  AnimationFrameBuffer::InsertStatus status = aQueue.Insert(CreateEmptyFrame());
  EXPECT_EQ(AnimationFrameBuffer::InsertStatus::CONTINUE, status);

  while (true) {
    status = aQueue.Insert(CreateEmptyFrame());
    bool restartDecoder = aQueue.AdvanceTo(aQueue.Size() - 1);
    EXPECT_FALSE(restartDecoder);

    if (status == AnimationFrameBuffer::InsertStatus::DISCARD_CONTINUE) {
      break;
    }
    EXPECT_EQ(AnimationFrameBuffer::InsertStatus::CONTINUE, status);
>>>>>>> upstream-releases
  }

  EXPECT_EQ(aQueue.Threshold(), aQueue.Size());
}

<<<<<<< HEAD
static void
VerifyInsert(AnimationFrameBuffer& aQueue,
             AnimationFrameBuffer::InsertStatus aExpectedStatus)
{
  RefPtr<imgFrame> frame = CreateEmptyFrame();
  AnimationFrameBuffer::InsertStatus status =
    aQueue.Insert(RefPtr<imgFrame>(frame));
  EXPECT_EQ(aExpectedStatus, status);
  EXPECT_TRUE(aQueue.IsLastInsertedFrame(frame));
  VerifyInsertInternal(aQueue, frame);
||||||| merged common ancestors
static void
CheckRemoved(const AnimationFrameBuffer& buffer, size_t aStart, size_t aEnd)
{
  CheckFrames(buffer, aStart, aEnd, false);
=======
static void VerifyDiscardingQueueContents(
    AnimationFrameDiscardingQueue& aQueue) {
  auto frames = aQueue.Display();
  for (auto i : frames) {
    EXPECT_TRUE(i != nullptr);
  }
>>>>>>> upstream-releases
}

<<<<<<< HEAD
static void
VerifyReset(AnimationFrameBuffer& aQueue,
            bool aExpectedContinue,
            const imgFrame* aFirstFrame)
{
  bool keepDecoding = aQueue.Reset();
  EXPECT_EQ(aExpectedContinue, keepDecoding);
  EXPECT_EQ(aQueue.Batch() * 2, aQueue.PendingDecode());
  EXPECT_EQ(aFirstFrame, aQueue.Get(0, true));

  if (!aQueue.MayDiscard()) {
    const AnimationFrameRetainedBuffer& queue =
      *static_cast<AnimationFrameRetainedBuffer*>(&aQueue);
    EXPECT_EQ(aFirstFrame, queue.Frames()[0].get());
    EXPECT_EQ(aFirstFrame, aQueue.Get(0, false));
  } else {
    const AnimationFrameDiscardingQueue& queue =
      *static_cast<AnimationFrameDiscardingQueue*>(&aQueue);
    EXPECT_EQ(size_t(0), queue.PendingInsert());
    EXPECT_EQ(size_t(0), queue.Display().size());
    EXPECT_EQ(aFirstFrame, queue.FirstFrame());
    EXPECT_EQ(nullptr, aQueue.Get(0, false));
  }
||||||| merged common ancestors
static void
CheckRetained(const AnimationFrameBuffer& buffer, size_t aStart, size_t aEnd)
{
  CheckFrames(buffer, aStart, aEnd, true);
=======
static void VerifyInsertInternal(AnimationFrameBuffer& aQueue,
                                 imgFrame* aFrame) {
  // Determine the frame index where we just inserted the frame.
  size_t frameIndex;
  if (aQueue.MayDiscard()) {
    const AnimationFrameDiscardingQueue& queue =
        *static_cast<AnimationFrameDiscardingQueue*>(&aQueue);
    frameIndex = queue.PendingInsert() == 0 ? queue.Size() - 1
                                            : queue.PendingInsert() - 1;
  } else {
    ASSERT_FALSE(aQueue.SizeKnown());
    frameIndex = aQueue.Size() - 1;
  }

  // Make sure we can get the frame from that index.
  RefPtr<imgFrame> frame = aQueue.Get(frameIndex, false);
  EXPECT_EQ(aFrame, frame.get());
>>>>>>> upstream-releases
}

static void VerifyAdvance(AnimationFrameBuffer& aQueue, size_t aExpectedFrame,
                          bool aExpectedRestartDecoder) {
  RefPtr<imgFrame> oldFrame;
  size_t totalRecycled;
  if (aQueue.IsRecycling()) {
    AnimationFrameRecyclingQueue& queue =
        *static_cast<AnimationFrameRecyclingQueue*>(&aQueue);
    oldFrame = queue.Get(queue.Displayed(), false);
    totalRecycled = queue.Recycle().size();
  }

  bool restartDecoder = aQueue.AdvanceTo(aExpectedFrame);
  EXPECT_EQ(aExpectedRestartDecoder, restartDecoder);

  if (aQueue.IsRecycling()) {
    const AnimationFrameRecyclingQueue& queue =
        *static_cast<AnimationFrameRecyclingQueue*>(&aQueue);
    EXPECT_FALSE(queue.Recycle().back().mDirtyRect.IsEmpty());
    EXPECT_TRUE(
        queue.Recycle().back().mDirtyRect.Contains(oldFrame->GetDirtyRect()));
    EXPECT_EQ(totalRecycled + 1, queue.Recycle().size());
    EXPECT_EQ(oldFrame.get(), queue.Recycle().back().mFrame.get());
  }
}

static void VerifyInsertAndAdvance(
    AnimationFrameBuffer& aQueue, size_t aExpectedFrame,
    AnimationFrameBuffer::InsertStatus aExpectedStatus) {
  // Insert the decoded frame.
  RefPtr<imgFrame> frame = CreateEmptyFrame();
  AnimationFrameBuffer::InsertStatus status =
      aQueue.Insert(RefPtr<imgFrame>(frame));
  EXPECT_EQ(aExpectedStatus, status);
  EXPECT_TRUE(aQueue.IsLastInsertedFrame(frame));
  VerifyInsertInternal(aQueue, frame);

  // Advance the display frame.
  bool expectedRestartDecoder =
      aExpectedStatus == AnimationFrameBuffer::InsertStatus::YIELD;
  VerifyAdvance(aQueue, aExpectedFrame, expectedRestartDecoder);
}

static void VerifyMarkComplete(AnimationFrameBuffer& aQueue,
                               bool aExpectedContinue,
                               const IntRect& aRefreshArea = IntRect(0, 0, 1,
                                                                     1)) {
  if (aQueue.IsRecycling() && !aQueue.SizeKnown()) {
    const AnimationFrameRecyclingQueue& queue =
        *static_cast<AnimationFrameRecyclingQueue*>(&aQueue);
    EXPECT_EQ(queue.FirstFrame()->GetRect(), queue.FirstFrameRefreshArea());
  }

  bool keepDecoding = aQueue.MarkComplete(aRefreshArea);
  EXPECT_EQ(aExpectedContinue, keepDecoding);

  if (aQueue.IsRecycling()) {
    const AnimationFrameRecyclingQueue& queue =
        *static_cast<AnimationFrameRecyclingQueue*>(&aQueue);
    EXPECT_EQ(aRefreshArea, queue.FirstFrameRefreshArea());
  }
}

static void VerifyInsert(AnimationFrameBuffer& aQueue,
                         AnimationFrameBuffer::InsertStatus aExpectedStatus) {
  RefPtr<imgFrame> frame = CreateEmptyFrame();
  AnimationFrameBuffer::InsertStatus status =
      aQueue.Insert(RefPtr<imgFrame>(frame));
  EXPECT_EQ(aExpectedStatus, status);
  EXPECT_TRUE(aQueue.IsLastInsertedFrame(frame));
  VerifyInsertInternal(aQueue, frame);
}

static void VerifyReset(AnimationFrameBuffer& aQueue, bool aExpectedContinue,
                        const imgFrame* aFirstFrame) {
  bool keepDecoding = aQueue.Reset();
  EXPECT_EQ(aExpectedContinue, keepDecoding);
  EXPECT_EQ(aQueue.Batch() * 2, aQueue.PendingDecode());
  EXPECT_EQ(aFirstFrame, aQueue.Get(0, true));

  if (!aQueue.MayDiscard()) {
    const AnimationFrameRetainedBuffer& queue =
        *static_cast<AnimationFrameRetainedBuffer*>(&aQueue);
    EXPECT_EQ(aFirstFrame, queue.Frames()[0].get());
    EXPECT_EQ(aFirstFrame, aQueue.Get(0, false));
  } else {
    const AnimationFrameDiscardingQueue& queue =
        *static_cast<AnimationFrameDiscardingQueue*>(&aQueue);
    EXPECT_EQ(size_t(0), queue.PendingInsert());
    EXPECT_EQ(size_t(0), queue.Display().size());
    EXPECT_EQ(aFirstFrame, queue.FirstFrame());
    EXPECT_EQ(nullptr, aQueue.Get(0, false));
  }
}

class ImageAnimationFrameBuffer : public ::testing::Test {
 public:
  ImageAnimationFrameBuffer() {}

 private:
  AutoInitializeImageLib mInit;
};

<<<<<<< HEAD
TEST_F(ImageAnimationFrameBuffer, RetainedInitialState)
{
||||||| merged common ancestors
TEST_F(ImageAnimationFrameBuffer, InitialState)
{
=======
TEST_F(ImageAnimationFrameBuffer, RetainedInitialState) {
>>>>>>> upstream-releases
  const size_t kThreshold = 800;
  const size_t kBatch = 100;
  AnimationFrameRetainedBuffer buffer(kThreshold, kBatch, 0);

  EXPECT_EQ(kThreshold, buffer.Threshold());
  EXPECT_EQ(kBatch, buffer.Batch());
  EXPECT_EQ(size_t(0), buffer.Displayed());
  EXPECT_EQ(kBatch * 2, buffer.PendingDecode());
  EXPECT_EQ(size_t(0), buffer.PendingAdvance());
  EXPECT_FALSE(buffer.MayDiscard());
  EXPECT_FALSE(buffer.SizeKnown());
  EXPECT_EQ(size_t(0), buffer.Size());
}

TEST_F(ImageAnimationFrameBuffer, ThresholdTooSmall) {
  const size_t kThreshold = 0;
  const size_t kBatch = 10;
  AnimationFrameRetainedBuffer buffer(kThreshold, kBatch, 0);

  EXPECT_EQ(kBatch * 2 + 1, buffer.Threshold());
  EXPECT_EQ(kBatch, buffer.Batch());
  EXPECT_EQ(kBatch * 2, buffer.PendingDecode());
  EXPECT_EQ(size_t(0), buffer.PendingAdvance());
}

TEST_F(ImageAnimationFrameBuffer, BatchTooSmall) {
  const size_t kThreshold = 10;
  const size_t kBatch = 0;
  AnimationFrameRetainedBuffer buffer(kThreshold, kBatch, 0);

  EXPECT_EQ(kThreshold, buffer.Threshold());
  EXPECT_EQ(size_t(1), buffer.Batch());
  EXPECT_EQ(size_t(2), buffer.PendingDecode());
  EXPECT_EQ(size_t(0), buffer.PendingAdvance());
}

TEST_F(ImageAnimationFrameBuffer, BatchTooBig) {
  const size_t kThreshold = 50;
  const size_t kBatch = SIZE_MAX;
  AnimationFrameRetainedBuffer buffer(kThreshold, kBatch, 0);

  // The rounding is important here (e.g. SIZE_MAX/4 * 2 != SIZE_MAX/2).
  EXPECT_EQ(SIZE_MAX / 4, buffer.Batch());
  EXPECT_EQ(buffer.Batch() * 2 + 1, buffer.Threshold());
  EXPECT_EQ(buffer.Batch() * 2, buffer.PendingDecode());
  EXPECT_EQ(size_t(0), buffer.PendingAdvance());
}

TEST_F(ImageAnimationFrameBuffer, FinishUnderBatchAndThreshold) {
  const size_t kThreshold = 30;
  const size_t kBatch = 10;
  AnimationFrameRetainedBuffer buffer(kThreshold, kBatch, 0);
  const auto& frames = buffer.Frames();

  EXPECT_EQ(kBatch * 2, buffer.PendingDecode());

  RefPtr<imgFrame> firstFrame;
  for (size_t i = 0; i < 5; ++i) {
    RefPtr<imgFrame> frame = CreateEmptyFrame();
    auto status = buffer.Insert(RefPtr<imgFrame>(frame));
    EXPECT_EQ(status, AnimationFrameBuffer::InsertStatus::CONTINUE);
    EXPECT_FALSE(buffer.SizeKnown());
    EXPECT_EQ(buffer.Size(), i + 1);

    if (i == 4) {
      EXPECT_EQ(size_t(15), buffer.PendingDecode());
      bool keepDecoding = buffer.MarkComplete(IntRect(0, 0, 1, 1));
      EXPECT_FALSE(keepDecoding);
      EXPECT_TRUE(buffer.SizeKnown());
      EXPECT_EQ(size_t(0), buffer.PendingDecode());
      EXPECT_FALSE(buffer.HasRedecodeError());
    }

    EXPECT_FALSE(buffer.MayDiscard());

    imgFrame* gotFrame = buffer.Get(i, false);
    EXPECT_EQ(frame.get(), gotFrame);
    ASSERT_EQ(i + 1, frames.Length());
    EXPECT_EQ(frame.get(), frames[i].get());

    if (i == 0) {
      firstFrame = std::move(frame);
      EXPECT_EQ(size_t(0), buffer.Displayed());
    } else {
      EXPECT_EQ(i - 1, buffer.Displayed());
      bool restartDecoder = buffer.AdvanceTo(i);
      EXPECT_FALSE(restartDecoder);
      EXPECT_EQ(i, buffer.Displayed());
    }

    gotFrame = buffer.Get(0, false);
    EXPECT_EQ(firstFrame.get(), gotFrame);
  }

  // Loop again over the animation and make sure it is still all there.
  for (size_t i = 0; i < frames.Length(); ++i) {
    EXPECT_TRUE(buffer.Get(i, false) != nullptr);

    bool restartDecoder = buffer.AdvanceTo(i);
    EXPECT_FALSE(restartDecoder);
  }
}

TEST_F(ImageAnimationFrameBuffer, FinishMultipleBatchesUnderThreshold) {
  const size_t kThreshold = 30;
  const size_t kBatch = 2;
  AnimationFrameRetainedBuffer buffer(kThreshold, kBatch, 0);
  const auto& frames = buffer.Frames();

  EXPECT_EQ(kBatch * 2, buffer.PendingDecode());

  // Add frames until it tells us to stop.
  AnimationFrameBuffer::InsertStatus status;
  do {
    status = buffer.Insert(CreateEmptyFrame());
    EXPECT_FALSE(buffer.SizeKnown());
    EXPECT_FALSE(buffer.MayDiscard());
  } while (status == AnimationFrameBuffer::InsertStatus::CONTINUE);

  EXPECT_EQ(size_t(0), buffer.PendingDecode());
  EXPECT_EQ(size_t(4), frames.Length());
  EXPECT_EQ(status, AnimationFrameBuffer::InsertStatus::YIELD);

  // Progress through the animation until it lets us decode again.
  bool restartDecoder = false;
  size_t i = 0;
  do {
    EXPECT_TRUE(buffer.Get(i, false) != nullptr);
    if (i > 0) {
      restartDecoder = buffer.AdvanceTo(i);
    }
    ++i;
  } while (!restartDecoder);

  EXPECT_EQ(size_t(2), buffer.PendingDecode());
  EXPECT_EQ(size_t(2), buffer.Displayed());

  // Add the last frame.
  status = buffer.Insert(CreateEmptyFrame());
  EXPECT_EQ(status, AnimationFrameBuffer::InsertStatus::CONTINUE);
  bool keepDecoding = buffer.MarkComplete(IntRect(0, 0, 1, 1));
  EXPECT_FALSE(keepDecoding);
  EXPECT_TRUE(buffer.SizeKnown());
  EXPECT_EQ(size_t(0), buffer.PendingDecode());
  EXPECT_EQ(size_t(5), frames.Length());
  EXPECT_FALSE(buffer.HasRedecodeError());

  // Finish progressing through the animation.
<<<<<<< HEAD
  for ( ; i < frames.Length(); ++i) {
    EXPECT_TRUE(buffer.Get(i, false) != nullptr);
||||||| merged common ancestors
  for ( ; i < frames.Length(); ++i) {
    EXPECT_TRUE(buffer.Get(i) != nullptr);
=======
  for (; i < frames.Length(); ++i) {
    EXPECT_TRUE(buffer.Get(i, false) != nullptr);
>>>>>>> upstream-releases
    restartDecoder = buffer.AdvanceTo(i);
    EXPECT_FALSE(restartDecoder);
  }

  // Loop again over the animation and make sure it is still all there.
  for (i = 0; i < frames.Length(); ++i) {
    EXPECT_TRUE(buffer.Get(i, false) != nullptr);
    restartDecoder = buffer.AdvanceTo(i);
    EXPECT_FALSE(restartDecoder);
  }

  // Loop to the third frame and then reset the animation.
  for (i = 0; i < 3; ++i) {
    EXPECT_TRUE(buffer.Get(i, false) != nullptr);
    restartDecoder = buffer.AdvanceTo(i);
    EXPECT_FALSE(restartDecoder);
  }

  // Since we are below the threshold, we can reset the get index only.
  // Nothing else should have changed.
  restartDecoder = buffer.Reset();
  EXPECT_FALSE(restartDecoder);
  for (i = 0; i < 5; ++i) {
    EXPECT_TRUE(buffer.Get(i, false) != nullptr);
  }
  EXPECT_EQ(size_t(0), buffer.PendingDecode());
  EXPECT_EQ(size_t(0), buffer.PendingAdvance());
  EXPECT_EQ(size_t(0), buffer.Displayed());
}

<<<<<<< HEAD
TEST_F(ImageAnimationFrameBuffer, StartAfterBeginning)
{
||||||| merged common ancestors
TEST_F(ImageAnimationFrameBuffer, ResetIncompleteAboveThreshold)
{
  const size_t kThreshold = 5;
  const size_t kBatch = 2;
  AnimationFrameBuffer buffer;
  buffer.Initialize(kThreshold, kBatch, 0);
  const auto& frames = buffer.Frames();

  // Add frames until we exceed the threshold.
  bool keepDecoding;
  bool restartDecoder;
  size_t i = 0;
  do {
    keepDecoding = buffer.Insert(CreateEmptyFrame());
    EXPECT_TRUE(keepDecoding);
    if (i > 0) {
      restartDecoder = buffer.AdvanceTo(i);
      EXPECT_FALSE(restartDecoder);
    }
    ++i;
  } while (!buffer.MayDiscard());

  // Should have threshold + 1 frames, and still not complete.
  EXPECT_EQ(size_t(6), frames.Length());
  EXPECT_FALSE(buffer.SizeKnown());

  // Restart the animation, we still had pending frames to decode since we
  // advanced in lockstep, so it should not ask us to restart the decoder.
  restartDecoder = buffer.Reset();
  EXPECT_FALSE(restartDecoder);
  CheckRetained(buffer, 0, 1);
  CheckRemoved(buffer, 1, frames.Length());
  EXPECT_EQ(kBatch * 2, buffer.PendingDecode());
  EXPECT_EQ(size_t(0), buffer.PendingAdvance());
  EXPECT_EQ(size_t(0), buffer.Displayed());

  // Adding new frames should not grow the insertion array, but instead
  // should reuse the space already allocated. Given that we are able to
  // discard frames once we cross the threshold, we should confirm that
  // we only do so if we have advanced beyond them.
  size_t oldFramesLength = frames.Length();
  size_t advanceUpTo = frames.Length() - kBatch;
  for (i = 0; i < oldFramesLength; ++i) {
    keepDecoding = buffer.Insert(CreateEmptyFrame());
    EXPECT_TRUE(keepDecoding);
    EXPECT_TRUE(frames[i]);
    EXPECT_EQ(oldFramesLength, frames.Length());
    if (i > 0) {
      // If we stop advancing, we should still retain the previous frames.
      EXPECT_TRUE(frames[i-1]);
      if (i <= advanceUpTo) {
        restartDecoder = buffer.AdvanceTo(i);
        EXPECT_FALSE(restartDecoder);
      }
    }
  }

  // Add one more frame. It should have grown the array this time.
  keepDecoding = buffer.Insert(CreateEmptyFrame());
  EXPECT_TRUE(keepDecoding);
  ASSERT_EQ(i + 1, frames.Length());
  EXPECT_TRUE(frames[i]);
}

TEST_F(ImageAnimationFrameBuffer, StartAfterBeginning)
{
=======
TEST_F(ImageAnimationFrameBuffer, StartAfterBeginning) {
>>>>>>> upstream-releases
  const size_t kThreshold = 30;
  const size_t kBatch = 2;
  const size_t kStartFrame = 7;
  AnimationFrameRetainedBuffer buffer(kThreshold, kBatch, kStartFrame);

  EXPECT_EQ(kStartFrame, buffer.PendingAdvance());

  // Add frames until it tells us to stop. It should be later than before,
  // because it auto-advances until its displayed frame is kStartFrame.
  AnimationFrameBuffer::InsertStatus status;
  size_t i = 0;
  do {
    status = buffer.Insert(CreateEmptyFrame());
    EXPECT_FALSE(buffer.SizeKnown());
    EXPECT_FALSE(buffer.MayDiscard());

    if (i <= kStartFrame) {
      EXPECT_EQ(i, buffer.Displayed());
      EXPECT_EQ(kStartFrame - i, buffer.PendingAdvance());
    } else {
      EXPECT_EQ(kStartFrame, buffer.Displayed());
      EXPECT_EQ(size_t(0), buffer.PendingAdvance());
    }

    i++;
  } while (status == AnimationFrameBuffer::InsertStatus::CONTINUE);

  EXPECT_EQ(size_t(0), buffer.PendingDecode());
  EXPECT_EQ(size_t(0), buffer.PendingAdvance());
  EXPECT_EQ(size_t(10), buffer.Size());
}

TEST_F(ImageAnimationFrameBuffer, StartAfterBeginningAndReset) {
  const size_t kThreshold = 30;
  const size_t kBatch = 2;
  const size_t kStartFrame = 7;
  AnimationFrameRetainedBuffer buffer(kThreshold, kBatch, kStartFrame);

  EXPECT_EQ(kStartFrame, buffer.PendingAdvance());

  // Add frames until it tells us to stop. It should be later than before,
  // because it auto-advances until its displayed frame is kStartFrame.
  for (size_t i = 0; i < 5; ++i) {
<<<<<<< HEAD
    AnimationFrameBuffer::InsertStatus status =
      buffer.Insert(CreateEmptyFrame());
    EXPECT_EQ(status, AnimationFrameBuffer::InsertStatus::CONTINUE);
||||||| merged common ancestors
    bool keepDecoding = buffer.Insert(CreateEmptyFrame());
    EXPECT_TRUE(keepDecoding);
=======
    AnimationFrameBuffer::InsertStatus status =
        buffer.Insert(CreateEmptyFrame());
    EXPECT_EQ(status, AnimationFrameBuffer::InsertStatus::CONTINUE);
>>>>>>> upstream-releases
    EXPECT_FALSE(buffer.SizeKnown());
    EXPECT_FALSE(buffer.MayDiscard());
    EXPECT_EQ(i, buffer.Displayed());
    EXPECT_EQ(kStartFrame - i, buffer.PendingAdvance());
  }

  // When we reset the animation, it goes back to the beginning. That means
  // we can forget about what we were told to advance to at the start. While
  // we have plenty of frames in our buffer, we still need one more because
  // in the real scenario, the decoder thread is still running and it is easier
  // to let it insert its last frame than to coordinate quitting earlier.
  buffer.Reset();
  EXPECT_EQ(size_t(0), buffer.Displayed());
  EXPECT_EQ(size_t(1), buffer.PendingDecode());
  EXPECT_EQ(size_t(0), buffer.PendingAdvance());
  EXPECT_EQ(size_t(5), buffer.Size());
}

<<<<<<< HEAD
static void TestDiscardingQueueLoop(AnimationFrameDiscardingQueue& aQueue,
                                    const imgFrame* aFirstFrame,
                                    size_t aThreshold,
                                    size_t aBatch,
                                    size_t aStartFrame)
{
  // We should be advanced right up to the last decoded frame.
  EXPECT_TRUE(aQueue.MayDiscard());
  EXPECT_FALSE(aQueue.SizeKnown());
  EXPECT_EQ(aBatch, aQueue.Batch());
  EXPECT_EQ(aThreshold, aQueue.PendingInsert());
  EXPECT_EQ(aThreshold, aQueue.Size());
  EXPECT_EQ(aFirstFrame, aQueue.FirstFrame());
  EXPECT_EQ(size_t(1), aQueue.Display().size());
  EXPECT_EQ(size_t(3), aQueue.PendingDecode());
  VerifyDiscardingQueueContents(aQueue);

  // Make sure frames get removed as we advance.
  VerifyInsertAndAdvance(aQueue, 5, AnimationFrameBuffer::InsertStatus::CONTINUE);
  EXPECT_EQ(size_t(1), aQueue.Display().size());
  VerifyInsertAndAdvance(aQueue, 6, AnimationFrameBuffer::InsertStatus::CONTINUE);
  EXPECT_EQ(size_t(1), aQueue.Display().size());

  // We actually will yield if we are recycling instead of continuing because
  // the pending calculation is slightly different. We will actually request one
  // less frame than we have to recycle.
  if (aQueue.IsRecycling()) {
    VerifyInsertAndAdvance(aQueue, 7, AnimationFrameBuffer::InsertStatus::YIELD);
  } else {
    VerifyInsertAndAdvance(aQueue, 7, AnimationFrameBuffer::InsertStatus::CONTINUE);
  }
  EXPECT_EQ(size_t(1), aQueue.Display().size());

  // We should get throttled if we insert too much.
  VerifyInsert(aQueue, AnimationFrameBuffer::InsertStatus::CONTINUE);
  EXPECT_EQ(size_t(2), aQueue.Display().size());
  EXPECT_EQ(size_t(1), aQueue.PendingDecode());
  VerifyInsert(aQueue, AnimationFrameBuffer::InsertStatus::YIELD);
  EXPECT_EQ(size_t(3), aQueue.Display().size());
  EXPECT_EQ(size_t(0), aQueue.PendingDecode());

  // We should get restarted if we advance.
  VerifyAdvance(aQueue, 8, true);
  EXPECT_EQ(size_t(2), aQueue.PendingDecode());
  VerifyAdvance(aQueue, 9, false);
  EXPECT_EQ(size_t(2), aQueue.PendingDecode());

  // We should continue decoding if we completed, since we are discarding.
  VerifyMarkComplete(aQueue, true);
  EXPECT_EQ(size_t(2), aQueue.PendingDecode());
  EXPECT_EQ(size_t(10), aQueue.Size());
  EXPECT_TRUE(aQueue.SizeKnown());
  EXPECT_FALSE(aQueue.HasRedecodeError());

  // Insert the first frames of the animation.
  VerifyInsert(aQueue, AnimationFrameBuffer::InsertStatus::CONTINUE);
  VerifyInsert(aQueue, AnimationFrameBuffer::InsertStatus::YIELD);
  EXPECT_EQ(size_t(0), aQueue.PendingDecode());
  EXPECT_EQ(size_t(10), aQueue.Size());

  // Advance back at the beginning. The first frame should only match for
  // display purposes.
  VerifyAdvance(aQueue, 0, true);
  EXPECT_EQ(size_t(2), aQueue.PendingDecode());
  EXPECT_TRUE(aQueue.FirstFrame() != nullptr);
  EXPECT_TRUE(aQueue.Get(0, false) != nullptr);
  EXPECT_NE(aQueue.FirstFrame(), aQueue.Get(0, false));
  EXPECT_EQ(aQueue.FirstFrame(), aQueue.Get(0, true));

  // Reiterate one more time and make it loops back.
  VerifyInsertAndAdvance(aQueue, 1, AnimationFrameBuffer::InsertStatus::CONTINUE);
  VerifyInsertAndAdvance(aQueue, 2, AnimationFrameBuffer::InsertStatus::YIELD);
  VerifyInsertAndAdvance(aQueue, 3, AnimationFrameBuffer::InsertStatus::CONTINUE);
  VerifyInsertAndAdvance(aQueue, 4, AnimationFrameBuffer::InsertStatus::YIELD);
  VerifyInsertAndAdvance(aQueue, 5, AnimationFrameBuffer::InsertStatus::CONTINUE);
  VerifyInsertAndAdvance(aQueue, 6, AnimationFrameBuffer::InsertStatus::YIELD);
  VerifyInsertAndAdvance(aQueue, 7, AnimationFrameBuffer::InsertStatus::CONTINUE);
  VerifyInsertAndAdvance(aQueue, 8, AnimationFrameBuffer::InsertStatus::YIELD);

  EXPECT_EQ(size_t(10), aQueue.PendingInsert());
  VerifyMarkComplete(aQueue, true);
  EXPECT_EQ(size_t(0), aQueue.PendingInsert());

  VerifyInsertAndAdvance(aQueue, 9, AnimationFrameBuffer::InsertStatus::CONTINUE);
  VerifyInsertAndAdvance(aQueue, 0, AnimationFrameBuffer::InsertStatus::YIELD);
  VerifyInsertAndAdvance(aQueue, 1, AnimationFrameBuffer::InsertStatus::CONTINUE);
}

TEST_F(ImageAnimationFrameBuffer, DiscardingLoop)
{
||||||| merged common ancestors
TEST_F(ImageAnimationFrameBuffer, RedecodeMoreFrames)
{
=======
static void TestDiscardingQueueLoop(AnimationFrameDiscardingQueue& aQueue,
                                    const imgFrame* aFirstFrame,
                                    size_t aThreshold, size_t aBatch,
                                    size_t aStartFrame) {
  // We should be advanced right up to the last decoded frame.
  EXPECT_TRUE(aQueue.MayDiscard());
  EXPECT_FALSE(aQueue.SizeKnown());
  EXPECT_EQ(aBatch, aQueue.Batch());
  EXPECT_EQ(aThreshold, aQueue.PendingInsert());
  EXPECT_EQ(aThreshold, aQueue.Size());
  EXPECT_EQ(aFirstFrame, aQueue.FirstFrame());
  EXPECT_EQ(size_t(1), aQueue.Display().size());
  EXPECT_EQ(size_t(3), aQueue.PendingDecode());
  VerifyDiscardingQueueContents(aQueue);

  // Make sure frames get removed as we advance.
  VerifyInsertAndAdvance(aQueue, 5,
                         AnimationFrameBuffer::InsertStatus::CONTINUE);
  EXPECT_EQ(size_t(1), aQueue.Display().size());
  VerifyInsertAndAdvance(aQueue, 6,
                         AnimationFrameBuffer::InsertStatus::CONTINUE);
  EXPECT_EQ(size_t(1), aQueue.Display().size());

  // We actually will yield if we are recycling instead of continuing because
  // the pending calculation is slightly different. We will actually request one
  // less frame than we have to recycle.
  if (aQueue.IsRecycling()) {
    VerifyInsertAndAdvance(aQueue, 7,
                           AnimationFrameBuffer::InsertStatus::YIELD);
  } else {
    VerifyInsertAndAdvance(aQueue, 7,
                           AnimationFrameBuffer::InsertStatus::CONTINUE);
  }
  EXPECT_EQ(size_t(1), aQueue.Display().size());

  // We should get throttled if we insert too much.
  VerifyInsert(aQueue, AnimationFrameBuffer::InsertStatus::CONTINUE);
  EXPECT_EQ(size_t(2), aQueue.Display().size());
  EXPECT_EQ(size_t(1), aQueue.PendingDecode());
  VerifyInsert(aQueue, AnimationFrameBuffer::InsertStatus::YIELD);
  EXPECT_EQ(size_t(3), aQueue.Display().size());
  EXPECT_EQ(size_t(0), aQueue.PendingDecode());

  // We should get restarted if we advance.
  VerifyAdvance(aQueue, 8, true);
  EXPECT_EQ(size_t(2), aQueue.PendingDecode());
  VerifyAdvance(aQueue, 9, false);
  EXPECT_EQ(size_t(2), aQueue.PendingDecode());

  // We should continue decoding if we completed, since we are discarding.
  VerifyMarkComplete(aQueue, true);
  EXPECT_EQ(size_t(2), aQueue.PendingDecode());
  EXPECT_EQ(size_t(10), aQueue.Size());
  EXPECT_TRUE(aQueue.SizeKnown());
  EXPECT_FALSE(aQueue.HasRedecodeError());

  // Insert the first frames of the animation.
  VerifyInsert(aQueue, AnimationFrameBuffer::InsertStatus::CONTINUE);
  VerifyInsert(aQueue, AnimationFrameBuffer::InsertStatus::YIELD);
  EXPECT_EQ(size_t(0), aQueue.PendingDecode());
  EXPECT_EQ(size_t(10), aQueue.Size());

  // Advance back at the beginning. The first frame should only match for
  // display purposes.
  VerifyAdvance(aQueue, 0, true);
  EXPECT_EQ(size_t(2), aQueue.PendingDecode());
  EXPECT_TRUE(aQueue.FirstFrame() != nullptr);
  EXPECT_TRUE(aQueue.Get(0, false) != nullptr);
  EXPECT_NE(aQueue.FirstFrame(), aQueue.Get(0, false));
  EXPECT_EQ(aQueue.FirstFrame(), aQueue.Get(0, true));

  // Reiterate one more time and make it loops back.
  VerifyInsertAndAdvance(aQueue, 1,
                         AnimationFrameBuffer::InsertStatus::CONTINUE);
  VerifyInsertAndAdvance(aQueue, 2, AnimationFrameBuffer::InsertStatus::YIELD);
  VerifyInsertAndAdvance(aQueue, 3,
                         AnimationFrameBuffer::InsertStatus::CONTINUE);
  VerifyInsertAndAdvance(aQueue, 4, AnimationFrameBuffer::InsertStatus::YIELD);
  VerifyInsertAndAdvance(aQueue, 5,
                         AnimationFrameBuffer::InsertStatus::CONTINUE);
  VerifyInsertAndAdvance(aQueue, 6, AnimationFrameBuffer::InsertStatus::YIELD);
  VerifyInsertAndAdvance(aQueue, 7,
                         AnimationFrameBuffer::InsertStatus::CONTINUE);
  VerifyInsertAndAdvance(aQueue, 8, AnimationFrameBuffer::InsertStatus::YIELD);

  EXPECT_EQ(size_t(10), aQueue.PendingInsert());
  VerifyMarkComplete(aQueue, true);
  EXPECT_EQ(size_t(0), aQueue.PendingInsert());

  VerifyInsertAndAdvance(aQueue, 9,
                         AnimationFrameBuffer::InsertStatus::CONTINUE);
  VerifyInsertAndAdvance(aQueue, 0, AnimationFrameBuffer::InsertStatus::YIELD);
  VerifyInsertAndAdvance(aQueue, 1,
                         AnimationFrameBuffer::InsertStatus::CONTINUE);
}

TEST_F(ImageAnimationFrameBuffer, DiscardingLoop) {
>>>>>>> upstream-releases
  const size_t kThreshold = 5;
  const size_t kBatch = 2;
  const size_t kStartFrame = 0;
  AnimationFrameRetainedBuffer retained(kThreshold, kBatch, kStartFrame);
  PrepareForDiscardingQueue(retained);
  const imgFrame* firstFrame = retained.Frames()[0].get();
  AnimationFrameDiscardingQueue buffer(std::move(retained));
  TestDiscardingQueueLoop(buffer, firstFrame, kThreshold, kBatch, kStartFrame);
}

<<<<<<< HEAD
TEST_F(ImageAnimationFrameBuffer, RecyclingLoop)
{
||||||| merged common ancestors
TEST_F(ImageAnimationFrameBuffer, RedecodeFewerFrames)
{
=======
TEST_F(ImageAnimationFrameBuffer, RecyclingLoop) {
>>>>>>> upstream-releases
  const size_t kThreshold = 5;
  const size_t kBatch = 2;
<<<<<<< HEAD
  const size_t kStartFrame = 0;
  AnimationFrameRetainedBuffer retained(kThreshold, kBatch, kStartFrame);
  PrepareForDiscardingQueue(retained);
  const imgFrame* firstFrame = retained.Frames()[0].get();
  AnimationFrameRecyclingQueue buffer(std::move(retained));

  // We should not start with any recycled frames.
  ASSERT_TRUE(buffer.Recycle().empty());

  TestDiscardingQueueLoop(buffer, firstFrame, kThreshold, kBatch, kStartFrame);

  // All the frames we inserted should have been recycleable.
  ASSERT_FALSE(buffer.Recycle().empty());
  while (!buffer.Recycle().empty()) {
    IntRect expectedRect(0, 0, 1, 1);
    RefPtr<imgFrame> expectedFrame = buffer.Recycle().front().mFrame;
    EXPECT_FALSE(expectedRect.IsEmpty());
    EXPECT_TRUE(expectedFrame.get() != nullptr);

    IntRect gotRect;
    RawAccessFrameRef gotFrame = buffer.RecycleFrame(gotRect);
    EXPECT_EQ(expectedFrame.get(), gotFrame.get());
    EXPECT_EQ(expectedRect, gotRect);
  }
||||||| merged common ancestors
  AnimationFrameBuffer buffer;
  buffer.Initialize(kThreshold, kBatch, 0);
  const auto& frames = buffer.Frames();

  // Add frames until we exceed the threshold.
  bool keepDecoding;
  bool restartDecoder;
  size_t i = 0;
  do {
    keepDecoding = buffer.Insert(CreateEmptyFrame());
    EXPECT_TRUE(keepDecoding);
    if (i > 0) {
      restartDecoder = buffer.AdvanceTo(i);
      EXPECT_FALSE(restartDecoder);
    }
    ++i;
  } while (!buffer.MayDiscard());
=======
  const size_t kStartFrame = 0;
  AnimationFrameRetainedBuffer retained(kThreshold, kBatch, kStartFrame);
  PrepareForDiscardingQueue(retained);
  const imgFrame* firstFrame = retained.Frames()[0].get();
  AnimationFrameRecyclingQueue buffer(std::move(retained));

  // We should not start with any recycled frames.
  ASSERT_TRUE(buffer.Recycle().empty());

  TestDiscardingQueueLoop(buffer, firstFrame, kThreshold, kBatch, kStartFrame);

  // All the frames we inserted should have been recycleable.
  ASSERT_FALSE(buffer.Recycle().empty());
  while (!buffer.Recycle().empty()) {
    IntRect expectedRect(0, 0, 1, 1);
    RefPtr<imgFrame> expectedFrame = buffer.Recycle().front().mFrame;
    EXPECT_FALSE(expectedRect.IsEmpty());
    EXPECT_TRUE(expectedFrame.get() != nullptr);

    IntRect gotRect;
    RawAccessFrameRef gotFrame = buffer.RecycleFrame(gotRect);
    EXPECT_EQ(expectedFrame.get(), gotFrame.get());
    EXPECT_EQ(expectedRect, gotRect);
    EXPECT_TRUE(ReinitForRecycle(gotFrame));
  }

  // Trying to pull a recycled frame when we have nothing should be safe too.
  IntRect gotRect;
  RawAccessFrameRef gotFrame = buffer.RecycleFrame(gotRect);
  EXPECT_TRUE(gotFrame.get() == nullptr);
  EXPECT_FALSE(ReinitForRecycle(gotFrame));
}
>>>>>>> upstream-releases

<<<<<<< HEAD
  // Trying to pull a recycled frame when we have nothing should be safe too.
  IntRect gotRect;
  RawAccessFrameRef gotFrame = buffer.RecycleFrame(gotRect);
  EXPECT_TRUE(gotFrame.get() == nullptr);
  EXPECT_TRUE(gotRect.IsEmpty());
}
||||||| merged common ancestors
  // Should have threshold + 1 frames, and still not complete.
  EXPECT_EQ(size_t(6), frames.Length());
  EXPECT_FALSE(buffer.SizeKnown());
=======
static void TestDiscardingQueueReset(AnimationFrameDiscardingQueue& aQueue,
                                     const imgFrame* aFirstFrame,
                                     size_t aThreshold, size_t aBatch,
                                     size_t aStartFrame) {
  // We should be advanced right up to the last decoded frame.
  EXPECT_TRUE(aQueue.MayDiscard());
  EXPECT_FALSE(aQueue.SizeKnown());
  EXPECT_EQ(aBatch, aQueue.Batch());
  EXPECT_EQ(aThreshold, aQueue.PendingInsert());
  EXPECT_EQ(aThreshold, aQueue.Size());
  EXPECT_EQ(aFirstFrame, aQueue.FirstFrame());
  EXPECT_EQ(size_t(1), aQueue.Display().size());
  EXPECT_EQ(size_t(4), aQueue.PendingDecode());
  VerifyDiscardingQueueContents(aQueue);

  // Reset should clear everything except the first frame.
  VerifyReset(aQueue, false, aFirstFrame);
}
>>>>>>> upstream-releases

<<<<<<< HEAD
static void TestDiscardingQueueReset(AnimationFrameDiscardingQueue& aQueue,
                                     const imgFrame* aFirstFrame,
                                     size_t aThreshold,
                                     size_t aBatch,
                                     size_t aStartFrame)
{
  // We should be advanced right up to the last decoded frame.
  EXPECT_TRUE(aQueue.MayDiscard());
  EXPECT_FALSE(aQueue.SizeKnown());
  EXPECT_EQ(aBatch, aQueue.Batch());
  EXPECT_EQ(aThreshold, aQueue.PendingInsert());
  EXPECT_EQ(aThreshold, aQueue.Size());
  EXPECT_EQ(aFirstFrame, aQueue.FirstFrame());
  EXPECT_EQ(size_t(1), aQueue.Display().size());
  EXPECT_EQ(size_t(4), aQueue.PendingDecode());
  VerifyDiscardingQueueContents(aQueue);

  // Reset should clear everything except the first frame.
  VerifyReset(aQueue, false, aFirstFrame);
}
||||||| merged common ancestors
  // Now we lock in at 6 frames.
  keepDecoding = buffer.MarkComplete();
  EXPECT_TRUE(keepDecoding);
  EXPECT_TRUE(buffer.SizeKnown());
  EXPECT_FALSE(buffer.HasRedecodeError());
=======
TEST_F(ImageAnimationFrameBuffer, DiscardingReset) {
  const size_t kThreshold = 8;
  const size_t kBatch = 3;
  const size_t kStartFrame = 0;
  AnimationFrameRetainedBuffer retained(kThreshold, kBatch, kStartFrame);
  PrepareForDiscardingQueue(retained);
  const imgFrame* firstFrame = retained.Frames()[0].get();
  AnimationFrameDiscardingQueue buffer(std::move(retained));
  TestDiscardingQueueReset(buffer, firstFrame, kThreshold, kBatch, kStartFrame);
}

TEST_F(ImageAnimationFrameBuffer, ResetBeforeDiscardingThreshold) {
  const size_t kThreshold = 3;
  const size_t kBatch = 1;
  const size_t kStartFrame = 0;

  // Get the starting buffer to just before the point where we need to switch
  // to a discarding buffer, reset the animation so advancing points at the
  // first frame, and insert the last frame to cross the threshold.
  AnimationFrameRetainedBuffer retained(kThreshold, kBatch, kStartFrame);
  VerifyInsert(retained, AnimationFrameBuffer::InsertStatus::CONTINUE);
  VerifyInsertAndAdvance(retained, 1,
                         AnimationFrameBuffer::InsertStatus::YIELD);
  bool restartDecoder = retained.Reset();
  EXPECT_FALSE(restartDecoder);
  VerifyInsert(retained, AnimationFrameBuffer::InsertStatus::DISCARD_YIELD);

  const imgFrame* firstFrame = retained.Frames()[0].get();
  EXPECT_TRUE(firstFrame != nullptr);
  AnimationFrameDiscardingQueue buffer(std::move(retained));
  const imgFrame* displayFirstFrame = buffer.Get(0, true);
  const imgFrame* advanceFirstFrame = buffer.Get(0, false);
  EXPECT_EQ(firstFrame, displayFirstFrame);
  EXPECT_EQ(firstFrame, advanceFirstFrame);
}

TEST_F(ImageAnimationFrameBuffer, DiscardingTooFewFrames) {
  const size_t kThreshold = 3;
  const size_t kBatch = 1;
  const size_t kStartFrame = 0;

  // First get us to a discarding buffer state.
  AnimationFrameRetainedBuffer retained(kThreshold, kBatch, kStartFrame);
  VerifyInsert(retained, AnimationFrameBuffer::InsertStatus::CONTINUE);
  VerifyInsertAndAdvance(retained, 1,
                         AnimationFrameBuffer::InsertStatus::YIELD);
  VerifyInsert(retained, AnimationFrameBuffer::InsertStatus::DISCARD_YIELD);

  // Insert one more frame.
  AnimationFrameDiscardingQueue buffer(std::move(retained));
  VerifyAdvance(buffer, 2, true);
  VerifyInsert(buffer, AnimationFrameBuffer::InsertStatus::YIELD);

  // Mark it as complete.
  bool restartDecoder = buffer.MarkComplete(IntRect(0, 0, 1, 1));
  EXPECT_FALSE(restartDecoder);
  EXPECT_FALSE(buffer.HasRedecodeError());
>>>>>>> upstream-releases

<<<<<<< HEAD
TEST_F(ImageAnimationFrameBuffer, DiscardingReset)
{
  const size_t kThreshold = 8;
  const size_t kBatch = 3;
  const size_t kStartFrame = 0;
  AnimationFrameRetainedBuffer retained(kThreshold, kBatch, kStartFrame);
  PrepareForDiscardingQueue(retained);
  const imgFrame* firstFrame = retained.Frames()[0].get();
  AnimationFrameDiscardingQueue buffer(std::move(retained));
  TestDiscardingQueueReset(buffer, firstFrame, kThreshold, kBatch, kStartFrame);
}
||||||| merged common ancestors
  // Reinsert 5 frames before marking complete.
  i = 0;
  do {
    keepDecoding = buffer.Insert(CreateEmptyFrame());
    EXPECT_TRUE(keepDecoding);
    restartDecoder = buffer.AdvanceTo(i);
    EXPECT_FALSE(restartDecoder);
    ++i;
  } while (i < 5);
=======
  // Insert one fewer frame than before.
  VerifyAdvance(buffer, 3, true);
  VerifyInsertAndAdvance(buffer, 0, AnimationFrameBuffer::InsertStatus::YIELD);
  VerifyInsertAndAdvance(buffer, 1, AnimationFrameBuffer::InsertStatus::YIELD);
  VerifyInsertAndAdvance(buffer, 2, AnimationFrameBuffer::InsertStatus::YIELD);
>>>>>>> upstream-releases

<<<<<<< HEAD
TEST_F(ImageAnimationFrameBuffer, ResetBeforeDiscardingThreshold)
{
  const size_t kThreshold = 3;
  const size_t kBatch = 1;
  const size_t kStartFrame = 0;

  // Get the starting buffer to just before the point where we need to switch
  // to a discarding buffer, reset the animation so advancing points at the
  // first frame, and insert the last frame to cross the threshold.
  AnimationFrameRetainedBuffer retained(kThreshold, kBatch, kStartFrame);
  VerifyInsert(retained, AnimationFrameBuffer::InsertStatus::CONTINUE);
  VerifyInsertAndAdvance(retained, 1, AnimationFrameBuffer::InsertStatus::YIELD);
  bool restartDecoder = retained.Reset();
  EXPECT_FALSE(restartDecoder);
  VerifyInsert(retained, AnimationFrameBuffer::InsertStatus::DISCARD_YIELD);

  const imgFrame* firstFrame = retained.Frames()[0].get();
  EXPECT_TRUE(firstFrame != nullptr);
  AnimationFrameDiscardingQueue buffer(std::move(retained));
  const imgFrame* displayFirstFrame = buffer.Get(0, true);
  const imgFrame* advanceFirstFrame = buffer.Get(0, false);
  EXPECT_EQ(firstFrame, displayFirstFrame);
  EXPECT_EQ(firstFrame, advanceFirstFrame);
||||||| merged common ancestors
  // We should now encounter an error and shutdown further decodes.
  keepDecoding = buffer.MarkComplete();
  EXPECT_FALSE(keepDecoding);
  EXPECT_EQ(size_t(0), buffer.PendingDecode());
  EXPECT_TRUE(buffer.HasRedecodeError());
=======
  // When we mark it as complete, it should fail due to too few frames.
  restartDecoder = buffer.MarkComplete(IntRect(0, 0, 1, 1));
  EXPECT_TRUE(buffer.HasRedecodeError());
  EXPECT_EQ(size_t(0), buffer.PendingDecode());
  EXPECT_EQ(size_t(4), buffer.Size());
>>>>>>> upstream-releases
}

<<<<<<< HEAD
TEST_F(ImageAnimationFrameBuffer, DiscardingTooFewFrames)
{
  const size_t kThreshold = 3;
  const size_t kBatch = 1;
  const size_t kStartFrame = 0;

  // First get us to a discarding buffer state.
  AnimationFrameRetainedBuffer retained(kThreshold, kBatch, kStartFrame);
  VerifyInsert(retained, AnimationFrameBuffer::InsertStatus::CONTINUE);
  VerifyInsertAndAdvance(retained, 1, AnimationFrameBuffer::InsertStatus::YIELD);
  VerifyInsert(retained, AnimationFrameBuffer::InsertStatus::DISCARD_YIELD);

  // Insert one more frame.
  AnimationFrameDiscardingQueue buffer(std::move(retained));
  VerifyAdvance(buffer, 2, true);
  VerifyInsert(buffer, AnimationFrameBuffer::InsertStatus::YIELD);

  // Mark it as complete.
  bool restartDecoder = buffer.MarkComplete(IntRect(0, 0, 1, 1));
  EXPECT_FALSE(restartDecoder);
  EXPECT_FALSE(buffer.HasRedecodeError());
||||||| merged common ancestors
TEST_F(ImageAnimationFrameBuffer, RedecodeFewerFramesAndBehindAdvancing)
{
  const size_t kThreshold = 5;
  const size_t kBatch = 2;
  AnimationFrameBuffer buffer;
  buffer.Initialize(kThreshold, kBatch, 0);
  const auto& frames = buffer.Frames();
=======
TEST_F(ImageAnimationFrameBuffer, DiscardingTooManyFrames) {
  const size_t kThreshold = 3;
  const size_t kBatch = 1;
  const size_t kStartFrame = 0;

  // First get us to a discarding buffer state.
  AnimationFrameRetainedBuffer retained(kThreshold, kBatch, kStartFrame);
  VerifyInsert(retained, AnimationFrameBuffer::InsertStatus::CONTINUE);
  VerifyInsertAndAdvance(retained, 1,
                         AnimationFrameBuffer::InsertStatus::YIELD);
  VerifyInsert(retained, AnimationFrameBuffer::InsertStatus::DISCARD_YIELD);

  // Insert one more frame.
  AnimationFrameDiscardingQueue buffer(std::move(retained));
  VerifyAdvance(buffer, 2, true);
  VerifyInsert(buffer, AnimationFrameBuffer::InsertStatus::YIELD);

  // Mark it as complete.
  bool restartDecoder = buffer.MarkComplete(IntRect(0, 0, 1, 1));
  EXPECT_FALSE(restartDecoder);
  EXPECT_FALSE(buffer.HasRedecodeError());
>>>>>>> upstream-releases

<<<<<<< HEAD
  // Insert one fewer frame than before.
  VerifyAdvance(buffer, 3, true);
  VerifyInsertAndAdvance(buffer, 0, AnimationFrameBuffer::InsertStatus::YIELD);
  VerifyInsertAndAdvance(buffer, 1, AnimationFrameBuffer::InsertStatus::YIELD);
  VerifyInsertAndAdvance(buffer, 2, AnimationFrameBuffer::InsertStatus::YIELD);
||||||| merged common ancestors
  // Add frames until we exceed the threshold.
  bool keepDecoding;
  bool restartDecoder;
  size_t i = 0;
  do {
    keepDecoding = buffer.Insert(CreateEmptyFrame());
    EXPECT_TRUE(keepDecoding);
    if (i > 0) {
      restartDecoder = buffer.AdvanceTo(i);
      EXPECT_FALSE(restartDecoder);
    }
    ++i;
  } while (!buffer.MayDiscard());
=======
  // Advance and insert to get us back to the end on the redecode.
  VerifyAdvance(buffer, 3, true);
  VerifyInsertAndAdvance(buffer, 0, AnimationFrameBuffer::InsertStatus::YIELD);
  VerifyInsertAndAdvance(buffer, 1, AnimationFrameBuffer::InsertStatus::YIELD);
  VerifyInsertAndAdvance(buffer, 2, AnimationFrameBuffer::InsertStatus::YIELD);
  VerifyInsertAndAdvance(buffer, 3, AnimationFrameBuffer::InsertStatus::YIELD);

  // Attempt to insert a 5th frame, it should fail.
  RefPtr<imgFrame> frame = CreateEmptyFrame();
  AnimationFrameBuffer::InsertStatus status = buffer.Insert(std::move(frame));
  EXPECT_EQ(AnimationFrameBuffer::InsertStatus::YIELD, status);
  EXPECT_TRUE(buffer.HasRedecodeError());
  EXPECT_EQ(size_t(0), buffer.PendingDecode());
  EXPECT_EQ(size_t(4), buffer.Size());
}
>>>>>>> upstream-releases

<<<<<<< HEAD
  // When we mark it as complete, it should fail due to too few frames.
  restartDecoder = buffer.MarkComplete(IntRect(0, 0, 1, 1));
  EXPECT_TRUE(buffer.HasRedecodeError());
  EXPECT_EQ(size_t(0), buffer.PendingDecode());
  EXPECT_EQ(size_t(4), buffer.Size());
}
||||||| merged common ancestors
  // Should have threshold + 1 frames, and still not complete.
  EXPECT_EQ(size_t(6), frames.Length());
  EXPECT_FALSE(buffer.SizeKnown());
=======
TEST_F(ImageAnimationFrameBuffer, RecyclingReset) {
  const size_t kThreshold = 8;
  const size_t kBatch = 3;
  const size_t kStartFrame = 0;
  AnimationFrameRetainedBuffer retained(kThreshold, kBatch, kStartFrame);
  PrepareForDiscardingQueue(retained);
  const imgFrame* firstFrame = retained.Frames()[0].get();
  AnimationFrameRecyclingQueue buffer(std::move(retained));
  TestDiscardingQueueReset(buffer, firstFrame, kThreshold, kBatch, kStartFrame);
}
>>>>>>> upstream-releases

<<<<<<< HEAD
TEST_F(ImageAnimationFrameBuffer, DiscardingTooManyFrames)
{
  const size_t kThreshold = 3;
  const size_t kBatch = 1;
  const size_t kStartFrame = 0;

  // First get us to a discarding buffer state.
  AnimationFrameRetainedBuffer retained(kThreshold, kBatch, kStartFrame);
  VerifyInsert(retained, AnimationFrameBuffer::InsertStatus::CONTINUE);
  VerifyInsertAndAdvance(retained, 1, AnimationFrameBuffer::InsertStatus::YIELD);
  VerifyInsert(retained, AnimationFrameBuffer::InsertStatus::DISCARD_YIELD);

  // Insert one more frame.
  AnimationFrameDiscardingQueue buffer(std::move(retained));
  VerifyAdvance(buffer, 2, true);
  VerifyInsert(buffer, AnimationFrameBuffer::InsertStatus::YIELD);

  // Mark it as complete.
  bool restartDecoder = buffer.MarkComplete(IntRect(0, 0, 1, 1));
  EXPECT_FALSE(restartDecoder);
  EXPECT_FALSE(buffer.HasRedecodeError());
||||||| merged common ancestors
  // Now we lock in at 6 frames.
  keepDecoding = buffer.MarkComplete();
  EXPECT_TRUE(keepDecoding);
  EXPECT_TRUE(buffer.SizeKnown());
  EXPECT_FALSE(buffer.HasRedecodeError());
=======
TEST_F(ImageAnimationFrameBuffer, RecyclingResetBeforeComplete) {
  const size_t kThreshold = 3;
  const size_t kBatch = 1;
  const size_t kStartFrame = 0;
  const IntSize kImageSize(100, 100);
  const IntRect kImageRect(IntPoint(0, 0), kImageSize);
  AnimationFrameRetainedBuffer retained(kThreshold, kBatch, kStartFrame);

  // Get the starting buffer to just before the point where we need to switch
  // to a discarding buffer, reset the animation so advancing points at the
  // first frame, and insert the last frame to cross the threshold.
  RefPtr<imgFrame> frame;
  frame = CreateEmptyFrame(kImageSize, kImageRect, false);
  AnimationFrameBuffer::InsertStatus status = retained.Insert(std::move(frame));
  EXPECT_EQ(AnimationFrameBuffer::InsertStatus::CONTINUE, status);

  frame = CreateEmptyFrame(kImageSize, IntRect(IntPoint(10, 10), IntSize(1, 1)),
                           false);
  status = retained.Insert(std::move(frame));
  EXPECT_EQ(AnimationFrameBuffer::InsertStatus::YIELD, status);

  VerifyAdvance(retained, 1, true);

  frame = CreateEmptyFrame(kImageSize, IntRect(IntPoint(20, 10), IntSize(1, 1)),
                           false);
  status = retained.Insert(std::move(frame));
  EXPECT_EQ(AnimationFrameBuffer::InsertStatus::DISCARD_YIELD, status);

  AnimationFrameRecyclingQueue buffer(std::move(retained));
  bool restartDecoding = buffer.Reset();
  EXPECT_TRUE(restartDecoding);

  // None of the buffers were recyclable.
  EXPECT_FALSE(buffer.Recycle().empty());
  while (!buffer.Recycle().empty()) {
    IntRect recycleRect;
    RawAccessFrameRef frameRef = buffer.RecycleFrame(recycleRect);
    EXPECT_TRUE(frameRef);
    EXPECT_FALSE(ReinitForRecycle(frameRef));
  }
>>>>>>> upstream-releases

<<<<<<< HEAD
  // Advance and insert to get us back to the end on the redecode.
  VerifyAdvance(buffer, 3, true);
  VerifyInsertAndAdvance(buffer, 0, AnimationFrameBuffer::InsertStatus::YIELD);
  VerifyInsertAndAdvance(buffer, 1, AnimationFrameBuffer::InsertStatus::YIELD);
  VerifyInsertAndAdvance(buffer, 2, AnimationFrameBuffer::InsertStatus::YIELD);
  VerifyInsertAndAdvance(buffer, 3, AnimationFrameBuffer::InsertStatus::YIELD);

  // Attempt to insert a 5th frame, it should fail.
  RefPtr<imgFrame> frame = CreateEmptyFrame();
  AnimationFrameBuffer::InsertStatus status = buffer.Insert(std::move(frame));
  EXPECT_EQ(AnimationFrameBuffer::InsertStatus::YIELD, status);
  EXPECT_TRUE(buffer.HasRedecodeError());
  EXPECT_EQ(size_t(0), buffer.PendingDecode());
  EXPECT_EQ(size_t(4), buffer.Size());
}
||||||| merged common ancestors
  // Reinsert frames without advancing until we exhaust our pending space. This
  // should be less than the current buffer length by definition.
  i = 0;
  do {
    keepDecoding = buffer.Insert(CreateEmptyFrame());
    ++i;
  } while (keepDecoding);

  EXPECT_EQ(size_t(2), i);

  // We should now encounter an error and shutdown further decodes.
  keepDecoding = buffer.MarkComplete();
  EXPECT_FALSE(keepDecoding);
  EXPECT_EQ(size_t(0), buffer.PendingDecode());
  EXPECT_TRUE(buffer.HasRedecodeError());
=======
  // Reinsert the first two frames as recyclable and reset again.
  frame = CreateEmptyFrame(kImageSize, kImageRect, true);
  status = buffer.Insert(std::move(frame));
  EXPECT_EQ(AnimationFrameBuffer::InsertStatus::CONTINUE, status);

  frame = CreateEmptyFrame(kImageSize, IntRect(IntPoint(10, 10), IntSize(1, 1)),
                           true);
  status = buffer.Insert(std::move(frame));
  EXPECT_EQ(AnimationFrameBuffer::InsertStatus::YIELD, status);

  restartDecoding = buffer.Reset();
  EXPECT_TRUE(restartDecoding);
>>>>>>> upstream-releases

<<<<<<< HEAD
TEST_F(ImageAnimationFrameBuffer, RecyclingReset)
{
  const size_t kThreshold = 8;
  const size_t kBatch = 3;
  const size_t kStartFrame = 0;
  AnimationFrameRetainedBuffer retained(kThreshold, kBatch, kStartFrame);
  PrepareForDiscardingQueue(retained);
  const imgFrame* firstFrame = retained.Frames()[0].get();
  AnimationFrameRecyclingQueue buffer(std::move(retained));
  TestDiscardingQueueReset(buffer, firstFrame, kThreshold, kBatch, kStartFrame);
}

TEST_F(ImageAnimationFrameBuffer, RecyclingResetBeforeComplete)
{
  const size_t kThreshold = 3;
  const size_t kBatch = 1;
  const size_t kStartFrame = 0;
  const IntSize kImageSize(100, 100);
  const IntRect kImageRect(IntPoint(0, 0), kImageSize);
  AnimationFrameRetainedBuffer retained(kThreshold, kBatch, kStartFrame);

  // Get the starting buffer to just before the point where we need to switch
  // to a discarding buffer, reset the animation so advancing points at the
  // first frame, and insert the last frame to cross the threshold.
  RefPtr<imgFrame> frame;
  frame = CreateEmptyFrame(kImageSize, kImageRect, false);
  AnimationFrameBuffer::InsertStatus status = retained.Insert(std::move(frame));
  EXPECT_EQ(AnimationFrameBuffer::InsertStatus::CONTINUE, status);

  frame = CreateEmptyFrame(kImageSize, IntRect(IntPoint(10, 10), IntSize(1, 1)), false);
  status = retained.Insert(std::move(frame));
  EXPECT_EQ(AnimationFrameBuffer::InsertStatus::YIELD, status);

  VerifyAdvance(retained, 1, true);

  frame = CreateEmptyFrame(kImageSize, IntRect(IntPoint(20, 10), IntSize(1, 1)), false);
  status = retained.Insert(std::move(frame));
  EXPECT_EQ(AnimationFrameBuffer::InsertStatus::DISCARD_YIELD, status);

  AnimationFrameRecyclingQueue buffer(std::move(retained));
  bool restartDecoding = buffer.Reset();
  EXPECT_TRUE(restartDecoding);

  // None of the buffers were recyclable.
  EXPECT_TRUE(buffer.Recycle().empty());

  // Reinsert the first two frames as recyclable and reset again.
  frame = CreateEmptyFrame(kImageSize, kImageRect, true);
  status = buffer.Insert(std::move(frame));
  EXPECT_EQ(AnimationFrameBuffer::InsertStatus::CONTINUE, status);

  frame = CreateEmptyFrame(kImageSize, IntRect(IntPoint(10, 10), IntSize(1, 1)), true);
  status = buffer.Insert(std::move(frame));
  EXPECT_EQ(AnimationFrameBuffer::InsertStatus::YIELD, status);

  restartDecoding = buffer.Reset();
  EXPECT_TRUE(restartDecoding);

  // Now both buffers should have been saved and the dirty rect replaced with
  // the full image rect since we don't know the first frame refresh area yet.
  EXPECT_EQ(size_t(2), buffer.Recycle().size());
  for (const auto& entry : buffer.Recycle()) {
    EXPECT_EQ(kImageRect, entry.mDirtyRect);
  }
||||||| merged common ancestors
  // We should however be able to continue advancing to the last decoded frame
  // without it requesting the decoder to restart.
  i = 0;
  do {
    restartDecoder = buffer.AdvanceTo(i);
    EXPECT_FALSE(restartDecoder);
    ++i;
  } while (i < 2);
=======
  // Now both buffers should have been saved and the dirty rect replaced with
  // the full image rect since we don't know the first frame refresh area yet.
  EXPECT_EQ(size_t(2), buffer.Recycle().size());
  for (const auto& entry : buffer.Recycle()) {
    EXPECT_EQ(kImageRect, entry.mDirtyRect);
  }
>>>>>>> upstream-releases
}

<<<<<<< HEAD
TEST_F(ImageAnimationFrameBuffer, RecyclingRect)
{
  const size_t kThreshold = 5;
  const size_t kBatch = 2;
  const size_t kStartFrame = 0;
  const IntSize kImageSize(100, 100);
  AnimationFrameRetainedBuffer retained(kThreshold, kBatch, kStartFrame);

  // Let's get to the recycling state while marking all of the frames as not
  // recyclable, just like AnimationFrameBuffer / the decoders would do.
  RefPtr<imgFrame> frame;
  frame = CreateEmptyFrame(kImageSize, IntRect(IntPoint(0, 0), kImageSize), false);
  AnimationFrameBuffer::InsertStatus status = retained.Insert(std::move(frame));
  EXPECT_EQ(AnimationFrameBuffer::InsertStatus::CONTINUE, status);

  frame = CreateEmptyFrame(kImageSize, IntRect(IntPoint(0, 0), kImageSize), false);
  status = retained.Insert(std::move(frame));
  EXPECT_EQ(AnimationFrameBuffer::InsertStatus::CONTINUE, status);

  frame = CreateEmptyFrame(kImageSize, IntRect(IntPoint(0, 0), kImageSize), false);
  status = retained.Insert(std::move(frame));
  EXPECT_EQ(AnimationFrameBuffer::InsertStatus::CONTINUE, status);

  frame = CreateEmptyFrame(kImageSize, IntRect(IntPoint(0, 0), kImageSize), false);
  status = retained.Insert(std::move(frame));
  EXPECT_EQ(AnimationFrameBuffer::InsertStatus::YIELD, status);

  VerifyAdvance(retained, 1, false);
  VerifyAdvance(retained, 2, true);
  VerifyAdvance(retained, 3, false);

  frame = CreateEmptyFrame(kImageSize, IntRect(IntPoint(0, 0), kImageSize), false);
  status = retained.Insert(std::move(frame));
  EXPECT_EQ(AnimationFrameBuffer::InsertStatus::DISCARD_CONTINUE, status);

  AnimationFrameRecyclingQueue buffer(std::move(retained));

  // The first frame is now the candidate for recycling. Since it was marked as
  // not recyclable, we should get nothing.
  VerifyAdvance(buffer, 4, false);

  IntRect recycleRect;
  EXPECT_FALSE(buffer.Recycle().empty());
  RawAccessFrameRef frameRef = buffer.RecycleFrame(recycleRect);
  EXPECT_FALSE(frameRef);
  EXPECT_TRUE(recycleRect.IsEmpty());
  EXPECT_TRUE(buffer.Recycle().empty());

  // Insert a recyclable partial frame. Its dirty rect shouldn't matter since
  // the previous frame was not recyclable.
  frame = CreateEmptyFrame(kImageSize, IntRect(0, 0, 25, 25));
  status = buffer.Insert(std::move(frame));
  EXPECT_EQ(AnimationFrameBuffer::InsertStatus::YIELD, status);

  VerifyAdvance(buffer, 5, true);
  EXPECT_FALSE(buffer.Recycle().empty());
  frameRef = buffer.RecycleFrame(recycleRect);
  EXPECT_FALSE(frameRef);
  EXPECT_TRUE(recycleRect.IsEmpty());
  EXPECT_TRUE(buffer.Recycle().empty());

  // Insert a recyclable partial frame. Its dirty rect should match the recycle
  // rect since it is the only frame in the buffer.
  frame = CreateEmptyFrame(kImageSize, IntRect(25, 0, 50, 50));
  status = buffer.Insert(std::move(frame));
  EXPECT_EQ(AnimationFrameBuffer::InsertStatus::YIELD, status);

  VerifyAdvance(buffer, 6, true);
  EXPECT_FALSE(buffer.Recycle().empty());
  frameRef = buffer.RecycleFrame(recycleRect);
  EXPECT_TRUE(frameRef);
  EXPECT_EQ(IntRect(25, 0, 50, 50), recycleRect);
  EXPECT_TRUE(buffer.Recycle().empty());

  // Insert the last frame and mark us as complete. The next recycled frame is
  // producing the first frame again, so we should use the first frame refresh
  // area instead of its dirty rect.
  frame = CreateEmptyFrame(kImageSize, IntRect(10, 10, 60, 10));
  status = buffer.Insert(std::move(frame));
  EXPECT_EQ(AnimationFrameBuffer::InsertStatus::YIELD, status);

  bool continueDecoding = buffer.MarkComplete(IntRect(0, 0, 75, 50));
  EXPECT_FALSE(continueDecoding);

  VerifyAdvance(buffer, 7, true);
  EXPECT_FALSE(buffer.Recycle().empty());
  frameRef = buffer.RecycleFrame(recycleRect);
  EXPECT_TRUE(frameRef);
  EXPECT_EQ(IntRect(0, 0, 75, 50), recycleRect);
  EXPECT_TRUE(buffer.Recycle().empty());

  // Now let's reinsert the first frame. The recycle rect should still be the
  // first frame refresh area instead of the dirty rect of the first frame (e.g.
  // the full frame).
  frame = CreateEmptyFrame(kImageSize, IntRect(IntPoint(0, 0), kImageSize), false);
  status = buffer.Insert(std::move(frame));
  EXPECT_EQ(AnimationFrameBuffer::InsertStatus::YIELD, status);

  VerifyAdvance(buffer, 0, true);
  EXPECT_FALSE(buffer.Recycle().empty());
  frameRef = buffer.RecycleFrame(recycleRect);
  EXPECT_TRUE(frameRef);
  EXPECT_EQ(IntRect(0, 0, 75, 50), recycleRect);
  EXPECT_TRUE(buffer.Recycle().empty());
}
||||||| merged common ancestors
=======
TEST_F(ImageAnimationFrameBuffer, RecyclingRect) {
  const size_t kThreshold = 5;
  const size_t kBatch = 2;
  const size_t kStartFrame = 0;
  const IntSize kImageSize(100, 100);
  const IntRect kImageRect(IntPoint(0, 0), kImageSize);
  AnimationFrameRetainedBuffer retained(kThreshold, kBatch, kStartFrame);

  // Let's get to the recycling state while marking all of the frames as not
  // recyclable, just like AnimationFrameBuffer / the decoders would do.
  RefPtr<imgFrame> frame;
  frame = CreateEmptyFrame(kImageSize, kImageRect, false);
  AnimationFrameBuffer::InsertStatus status = retained.Insert(std::move(frame));
  EXPECT_EQ(AnimationFrameBuffer::InsertStatus::CONTINUE, status);

  frame = CreateEmptyFrame(kImageSize, kImageRect, false);
  status = retained.Insert(std::move(frame));
  EXPECT_EQ(AnimationFrameBuffer::InsertStatus::CONTINUE, status);

  frame = CreateEmptyFrame(kImageSize, kImageRect, false);
  status = retained.Insert(std::move(frame));
  EXPECT_EQ(AnimationFrameBuffer::InsertStatus::CONTINUE, status);

  frame = CreateEmptyFrame(kImageSize, kImageRect, false);
  status = retained.Insert(std::move(frame));
  EXPECT_EQ(AnimationFrameBuffer::InsertStatus::YIELD, status);

  VerifyAdvance(retained, 1, false);
  VerifyAdvance(retained, 2, true);
  VerifyAdvance(retained, 3, false);

  frame = CreateEmptyFrame(kImageSize, kImageRect, false);
  status = retained.Insert(std::move(frame));
  EXPECT_EQ(AnimationFrameBuffer::InsertStatus::DISCARD_CONTINUE, status);

  AnimationFrameRecyclingQueue buffer(std::move(retained));

  // The first frame is now the candidate for recycling. Since it was marked as
  // not recyclable, we should get nothing.
  VerifyAdvance(buffer, 4, false);

  IntRect recycleRect;
  EXPECT_FALSE(buffer.Recycle().empty());
  RawAccessFrameRef frameRef = buffer.RecycleFrame(recycleRect);
  EXPECT_TRUE(frameRef);
  EXPECT_FALSE(ReinitForRecycle(frameRef));
  EXPECT_TRUE(buffer.Recycle().empty());

  // Insert a recyclable partial frame. Its dirty rect shouldn't matter since
  // the previous frame was not recyclable.
  frame = CreateEmptyFrame(kImageSize, IntRect(0, 0, 25, 25));
  status = buffer.Insert(std::move(frame));
  EXPECT_EQ(AnimationFrameBuffer::InsertStatus::YIELD, status);

  VerifyAdvance(buffer, 5, true);
  EXPECT_FALSE(buffer.Recycle().empty());
  frameRef = buffer.RecycleFrame(recycleRect);
  EXPECT_TRUE(frameRef);
  EXPECT_FALSE(ReinitForRecycle(frameRef));
  EXPECT_TRUE(buffer.Recycle().empty());

  // Insert a recyclable partial frame. Its dirty rect should match the recycle
  // rect since it is the only frame in the buffer.
  frame = CreateEmptyFrame(kImageSize, IntRect(25, 0, 50, 50));
  status = buffer.Insert(std::move(frame));
  EXPECT_EQ(AnimationFrameBuffer::InsertStatus::YIELD, status);

  VerifyAdvance(buffer, 6, true);
  EXPECT_FALSE(buffer.Recycle().empty());
  frameRef = buffer.RecycleFrame(recycleRect);
  EXPECT_TRUE(frameRef);
  EXPECT_TRUE(ReinitForRecycle(frameRef));
  EXPECT_EQ(IntRect(25, 0, 50, 50), recycleRect);
  EXPECT_TRUE(buffer.Recycle().empty());

  // Insert the last frame and mark us as complete. The next recycled frame is
  // producing the first frame again, so we should use the first frame refresh
  // area instead of its dirty rect.
  frame = CreateEmptyFrame(kImageSize, IntRect(10, 10, 60, 10));
  status = buffer.Insert(std::move(frame));
  EXPECT_EQ(AnimationFrameBuffer::InsertStatus::YIELD, status);

  bool continueDecoding = buffer.MarkComplete(IntRect(0, 0, 75, 50));
  EXPECT_FALSE(continueDecoding);

  VerifyAdvance(buffer, 7, true);
  EXPECT_FALSE(buffer.Recycle().empty());
  frameRef = buffer.RecycleFrame(recycleRect);
  EXPECT_TRUE(frameRef);
  EXPECT_TRUE(ReinitForRecycle(frameRef));
  EXPECT_EQ(IntRect(0, 0, 75, 50), recycleRect);
  EXPECT_TRUE(buffer.Recycle().empty());

  // Now let's reinsert the first frame. The recycle rect should still be the
  // first frame refresh area instead of the dirty rect of the first frame (e.g.
  // the full frame).
  frame = CreateEmptyFrame(kImageSize, kImageRect, false);
  status = buffer.Insert(std::move(frame));
  EXPECT_EQ(AnimationFrameBuffer::InsertStatus::YIELD, status);

  VerifyAdvance(buffer, 0, true);
  EXPECT_FALSE(buffer.Recycle().empty());
  frameRef = buffer.RecycleFrame(recycleRect);
  EXPECT_TRUE(frameRef);
  EXPECT_TRUE(ReinitForRecycle(frameRef));
  EXPECT_EQ(IntRect(0, 0, 75, 50), recycleRect);
  EXPECT_TRUE(buffer.Recycle().empty());
}
>>>>>>> upstream-releases
