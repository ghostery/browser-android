/* -*- Mode: C++; tab-width: 8; indent-tabs-mode: nil; c-basic-offset: 2 -*- */
/* vim: set ts=8 sts=2 et sw=2 tw=80: */
/* This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/. */

#include "ResourceQueue.h"
#include "nsDeque.h"
#include "MediaData.h"
#include "mozilla/ErrorResult.h"
#include "mozilla/IntegerPrintfMacros.h"
#include "mozilla/Logging.h"
#include "mozilla/Sprintf.h"
#include "mozilla/Unused.h"

extern mozilla::LogModule* GetSourceBufferResourceLog();

#define SBR_DEBUG(arg, ...)                                       \
  MOZ_LOG(GetSourceBufferResourceLog(), mozilla::LogLevel::Debug, \
          ("ResourceQueue(%p)::%s: " arg, this, __func__, ##__VA_ARGS__))
#define SBR_DEBUGV(arg, ...)                                        \
  MOZ_LOG(GetSourceBufferResourceLog(), mozilla::LogLevel::Verbose, \
          ("ResourceQueue(%p)::%s: " arg, this, __func__, ##__VA_ARGS__))

namespace mozilla {

<<<<<<< HEAD
ResourceItem::ResourceItem(MediaByteBuffer* aData) : mData(aData) {}

size_t ResourceItem::SizeOfIncludingThis(MallocSizeOf aMallocSizeOf) const {
  // size including this
  size_t size = aMallocSizeOf(this);

  // size excluding this
  size += mData->ShallowSizeOfExcludingThis(aMallocSizeOf);
||||||| merged common ancestors
ResourceItem::ResourceItem(MediaByteBuffer* aData)
  : mData(aData)
{
}

size_t
ResourceItem::SizeOfIncludingThis(MallocSizeOf aMallocSizeOf) const
{
  // size including this
  size_t size = aMallocSizeOf(this);

  // size excluding this
  size += mData->ShallowSizeOfExcludingThis(aMallocSizeOf);
=======
ResourceItem::ResourceItem(const MediaSpan& aData, uint64_t aOffset)
    : mData(aData), mOffset(aOffset) {}
>>>>>>> upstream-releases

size_t ResourceItem::SizeOfIncludingThis(MallocSizeOf aMallocSizeOf) const {
  return aMallocSizeOf(this);
}

class ResourceQueueDeallocator : public nsDequeFunctor {
  void operator()(void* aObject) override {
    delete static_cast<ResourceItem*>(aObject);
  }
};

ResourceQueue::ResourceQueue()
    : nsDeque(new ResourceQueueDeallocator()), mLogicalLength(0), mOffset(0) {}

uint64_t ResourceQueue::GetOffset() { return mOffset; }

<<<<<<< HEAD
uint64_t ResourceQueue::GetLength() { return mLogicalLength; }
||||||| merged common ancestors
uint64_t
ResourceQueue::GetLength()
{
  return mLogicalLength;
}
=======
uint64_t ResourceQueue::GetLength() { return mLogicalLength; }

const uint8_t* ResourceQueue::GetContiguousAccess(int64_t aOffset,
                                                  size_t aSize) {
  uint32_t offset = 0;
  uint32_t start = GetAtOffset(aOffset, &offset);
  if (start >= GetSize()) {
    return nullptr;
  }
  ResourceItem* item = ResourceAt(start);
  if (offset + aSize > item->mData.Length()) {
    return nullptr;
  }
  return item->mData.Elements() + offset;
}
>>>>>>> upstream-releases

void ResourceQueue::CopyData(uint64_t aOffset, uint32_t aCount, char* aDest) {
  uint32_t offset = 0;
  uint32_t start = GetAtOffset(aOffset, &offset);
<<<<<<< HEAD
  uint32_t end =
      std::min(GetAtOffset(aOffset + aCount, nullptr) + 1, uint32_t(GetSize()));
  for (uint32_t i = start; i < end; ++i) {
    ResourceItem* item = ResourceAt(i);
    uint32_t bytes = std::min(aCount, uint32_t(item->mData->Length() - offset));
||||||| merged common ancestors
  uint32_t end = std::min(GetAtOffset(aOffset + aCount, nullptr) + 1, uint32_t(GetSize()));
  for (uint32_t i = start; i < end; ++i) {
    ResourceItem* item = ResourceAt(i);
    uint32_t bytes = std::min(aCount, uint32_t(item->mData->Length() - offset));
=======
  size_t i = start;
  while (i < uint32_t(GetSize()) && aCount > 0) {
    ResourceItem* item = ResourceAt(i++);
    uint32_t bytes = std::min(aCount, uint32_t(item->mData.Length() - offset));
>>>>>>> upstream-releases
    if (bytes != 0) {
      memcpy(aDest, item->mData.Elements() + offset, bytes);
      offset = 0;
      aCount -= bytes;
      aDest += bytes;
    }
  }
}

<<<<<<< HEAD
void ResourceQueue::AppendItem(MediaByteBuffer* aData) {
  mLogicalLength += aData->Length();
  Push(new ResourceItem(aData));
||||||| merged common ancestors
void
ResourceQueue::AppendItem(MediaByteBuffer* aData)
{
  mLogicalLength += aData->Length();
  Push(new ResourceItem(aData));
=======
void ResourceQueue::AppendItem(const MediaSpan& aData) {
  uint64_t offset = mLogicalLength;
  mLogicalLength += aData.Length();
  Push(new ResourceItem(aData, offset));
>>>>>>> upstream-releases
}

<<<<<<< HEAD
uint32_t ResourceQueue::Evict(uint64_t aOffset, uint32_t aSizeToEvict,
                              ErrorResult& aRv) {
  SBR_DEBUG("Evict(aOffset=%" PRIu64 ", aSizeToEvict=%u)", aOffset,
            aSizeToEvict);
  return EvictBefore(std::min(aOffset, mOffset + (uint64_t)aSizeToEvict), aRv);
||||||| merged common ancestors
uint32_t
ResourceQueue::Evict(uint64_t aOffset, uint32_t aSizeToEvict,
                     ErrorResult& aRv)
{
  SBR_DEBUG("Evict(aOffset=%" PRIu64 ", aSizeToEvict=%u)",
            aOffset, aSizeToEvict);
  return EvictBefore(std::min(aOffset, mOffset + (uint64_t)aSizeToEvict), aRv);
=======
uint32_t ResourceQueue::Evict(uint64_t aOffset, uint32_t aSizeToEvict) {
  SBR_DEBUG("Evict(aOffset=%" PRIu64 ", aSizeToEvict=%u)", aOffset,
            aSizeToEvict);
  return EvictBefore(std::min(aOffset, mOffset + (uint64_t)aSizeToEvict));
>>>>>>> upstream-releases
}

<<<<<<< HEAD
uint32_t ResourceQueue::EvictBefore(uint64_t aOffset, ErrorResult& aRv) {
||||||| merged common ancestors
uint32_t ResourceQueue::EvictBefore(uint64_t aOffset, ErrorResult& aRv)
{
=======
uint32_t ResourceQueue::EvictBefore(uint64_t aOffset) {
>>>>>>> upstream-releases
  SBR_DEBUG("EvictBefore(%" PRIu64 ")", aOffset);
  uint32_t evicted = 0;
  while (ResourceItem* item = ResourceAt(0)) {
<<<<<<< HEAD
    SBR_DEBUG("item=%p length=%zu offset=%" PRIu64, item, item->mData->Length(),
              mOffset);
    if (item->mData->Length() + mOffset >= aOffset) {
||||||| merged common ancestors
    SBR_DEBUG("item=%p length=%zu offset=%" PRIu64,
              item, item->mData->Length(), mOffset);
    if (item->mData->Length() + mOffset >= aOffset) {
=======
    SBR_DEBUG("item=%p length=%zu offset=%" PRIu64, item, item->mData.Length(),
              mOffset);
    if (item->mData.Length() + mOffset >= aOffset) {
>>>>>>> upstream-releases
      if (aOffset <= mOffset) {
        break;
      }
      uint32_t offset = aOffset - mOffset;
      mOffset += offset;
      evicted += offset;
<<<<<<< HEAD
      RefPtr<MediaByteBuffer> data = new MediaByteBuffer;
      if (!data->AppendElements(item->mData->Elements() + offset,
                                item->mData->Length() - offset, fallible)) {
        aRv.Throw(NS_ERROR_OUT_OF_MEMORY);
        return 0;
      }

      item->mData = data;
||||||| merged common ancestors
      RefPtr<MediaByteBuffer> data = new MediaByteBuffer;
      if (!data->AppendElements(item->mData->Elements() + offset,
                                item->mData->Length() - offset,
                                fallible)) {
        aRv.Throw(NS_ERROR_OUT_OF_MEMORY);
        return 0;
      }

      item->mData = data;
=======
      item->mData.RemoveFront(offset);
      item->mOffset += offset;
>>>>>>> upstream-releases
      break;
    }
    mOffset += item->mData.Length();
    evicted += item->mData.Length();
    delete PopFront();
  }
  return evicted;
}

uint32_t ResourceQueue::EvictAll() {
  SBR_DEBUG("EvictAll()");
  uint32_t evicted = 0;
  while (ResourceItem* item = ResourceAt(0)) {
<<<<<<< HEAD
    SBR_DEBUG("item=%p length=%zu offset=%" PRIu64, item, item->mData->Length(),
              mOffset);
    mOffset += item->mData->Length();
    evicted += item->mData->Length();
||||||| merged common ancestors
    SBR_DEBUG("item=%p length=%zu offset=%" PRIu64,
              item, item->mData->Length(), mOffset);
    mOffset += item->mData->Length();
    evicted += item->mData->Length();
=======
    SBR_DEBUG("item=%p length=%zu offset=%" PRIu64, item, item->mData.Length(),
              mOffset);
    mOffset += item->mData.Length();
    evicted += item->mData.Length();
>>>>>>> upstream-releases
    delete PopFront();
  }
  return evicted;
}

size_t ResourceQueue::SizeOfExcludingThis(MallocSizeOf aMallocSizeOf) const {
  // Calculate the size of the internal deque.
  size_t size = nsDeque::SizeOfExcludingThis(aMallocSizeOf);

  // Sum the ResourceItems. The ResourceItems's MediaSpans may share the
  // same underlying MediaByteBuffers, so we need to de-dupe the buffers
  // in order to report an accurate size.
  nsTArray<MediaByteBuffer*> buffers;
  for (uint32_t i = 0; i < uint32_t(GetSize()); ++i) {
    const ResourceItem* item = ResourceAt(i);
    size += item->SizeOfIncludingThis(aMallocSizeOf);
    if (!buffers.Contains(item->mData.Buffer())) {
      buffers.AppendElement(item->mData.Buffer());
    }
  }

  for (MediaByteBuffer* buffer : buffers) {
    size += buffer->ShallowSizeOfExcludingThis(aMallocSizeOf);
  }

  return size;
}

#if defined(DEBUG)
void ResourceQueue::Dump(const char* aPath) {
  for (uint32_t i = 0; i < uint32_t(GetSize()); ++i) {
    ResourceItem* item = ResourceAt(i);

    char buf[255];
    SprintfLiteral(buf, "%s/%08u.bin", aPath, i);
    FILE* fp = fopen(buf, "wb");
    if (!fp) {
      return;
    }
    Unused << fwrite(item->mData.Elements(), item->mData.Length(), 1, fp);
    fclose(fp);
  }
}
#endif

ResourceItem* ResourceQueue::ResourceAt(uint32_t aIndex) const {
  return static_cast<ResourceItem*>(ObjectAt(aIndex));
}

<<<<<<< HEAD
uint32_t ResourceQueue::GetAtOffset(uint64_t aOffset,
                                    uint32_t* aResourceOffset) {
||||||| merged common ancestors
uint32_t
ResourceQueue::GetAtOffset(uint64_t aOffset, uint32_t *aResourceOffset)
{
=======
uint32_t ResourceQueue::GetAtOffset(uint64_t aOffset,
                                    uint32_t* aResourceOffset) const {
>>>>>>> upstream-releases
  MOZ_RELEASE_ASSERT(aOffset >= mOffset);

  size_t hi = GetSize();
  size_t lo = 0;
  while (lo < hi) {
    size_t mid = lo + (hi - lo) / 2;
    const ResourceItem* resource = ResourceAt(mid);
    if (resource->mOffset <= aOffset &&
        aOffset < resource->mOffset + resource->mData.Length()) {
      if (aResourceOffset) {
        *aResourceOffset = aOffset - resource->mOffset;
      }
      return uint32_t(mid);
    }
    if (resource->mOffset + resource->mData.Length() <= aOffset) {
      lo = mid + 1;
    } else {
      hi = mid;
    }
  }

  return uint32_t(GetSize());
}

ResourceItem* ResourceQueue::PopFront() {
  return static_cast<ResourceItem*>(nsDeque::PopFront());
}

#undef SBR_DEBUG
#undef SBR_DEBUGV

}  // namespace mozilla
