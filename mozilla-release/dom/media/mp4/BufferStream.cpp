/* This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/. */

#include "BufferStream.h"
#include "MediaData.h"
#include "MediaResource.h"
#include <algorithm>

namespace mozilla {

BufferStream::BufferStream()
    : mStartOffset(0), mData(new mozilla::MediaByteBuffer) {}

BufferStream::BufferStream(mozilla::MediaByteBuffer* aBuffer)
    : mStartOffset(0), mData(aBuffer) {}

BufferStream::~BufferStream() {}

<<<<<<< HEAD
/*virtual*/ bool BufferStream::ReadAt(int64_t aOffset, void* aData,
                                      size_t aLength, size_t* aBytesRead) {
||||||| merged common ancestors
/*virtual*/ bool
BufferStream::ReadAt(int64_t aOffset, void* aData, size_t aLength,
                     size_t* aBytesRead)
{
=======
/*virtual*/
bool BufferStream::ReadAt(int64_t aOffset, void* aData, size_t aLength,
                          size_t* aBytesRead) {
>>>>>>> upstream-releases
  if (aOffset < mStartOffset || aOffset > mStartOffset + mData->Length()) {
    return false;
  }
  *aBytesRead =
      std::min(aLength, size_t(mStartOffset + mData->Length() - aOffset));
  memcpy(aData, mData->Elements() + aOffset - mStartOffset, *aBytesRead);
  return true;
}

<<<<<<< HEAD
/*virtual*/ bool BufferStream::CachedReadAt(int64_t aOffset, void* aData,
                                            size_t aLength,
                                            size_t* aBytesRead) {
||||||| merged common ancestors
/*virtual*/ bool
BufferStream::CachedReadAt(int64_t aOffset, void* aData, size_t aLength,
                           size_t* aBytesRead)
{
=======
/*virtual*/
bool BufferStream::CachedReadAt(int64_t aOffset, void* aData, size_t aLength,
                                size_t* aBytesRead) {
>>>>>>> upstream-releases
  return ReadAt(aOffset, aData, aLength, aBytesRead);
}

<<<<<<< HEAD
/*virtual*/ bool BufferStream::Length(int64_t* aLength) {
||||||| merged common ancestors
/*virtual*/ bool
BufferStream::Length(int64_t* aLength)
{
=======
/*virtual*/
bool BufferStream::Length(int64_t* aLength) {
>>>>>>> upstream-releases
  *aLength = mStartOffset + mData->Length();
  return true;
}

<<<<<<< HEAD
/* virtual */ void BufferStream::DiscardBefore(int64_t aOffset) {
||||||| merged common ancestors
/* virtual */ void
BufferStream::DiscardBefore(int64_t aOffset)
{
=======
/* virtual */
void BufferStream::DiscardBefore(int64_t aOffset) {
>>>>>>> upstream-releases
  if (aOffset > mStartOffset) {
    mData->RemoveElementsAt(0, aOffset - mStartOffset);
    mStartOffset = aOffset;
  }
}

bool BufferStream::AppendBytes(const uint8_t* aData, size_t aLength) {
  return mData->AppendElements(aData, aLength, fallible);
}

MediaByteRange BufferStream::GetByteRange() {
  return MediaByteRange(mStartOffset, mStartOffset + mData->Length());
}
}  // namespace mozilla
