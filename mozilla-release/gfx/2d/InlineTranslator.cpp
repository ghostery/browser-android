/* -*- Mode: C++; tab-width: 8; indent-tabs-mode: nil; c-basic-offset: 2 -*- */
/* vim: set ts=8 sts=2 et sw=2 tw=80: */
/* This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/. */

#include "InlineTranslator.h"
#include "RecordedEventImpl.h"
#include "DrawEventRecorder.h"

#include "gfxContext.h"
#include "nsDeviceContext.h"
#include "mozilla/gfx/RecordingTypes.h"
#include "mozilla/UniquePtr.h"

using namespace mozilla::gfx;

namespace mozilla {
namespace gfx {

<<<<<<< HEAD
InlineTranslator::InlineTranslator(DrawTarget *aDT, void *aFontContext)
    : mBaseDT(aDT), mFontContext(aFontContext) {}
||||||| merged common ancestors
InlineTranslator::InlineTranslator(DrawTarget* aDT, void* aFontContext)
  : mBaseDT(aDT)
  , mFontContext(aFontContext)
{
}
=======
InlineTranslator::InlineTranslator(DrawTarget* aDT, void* aFontContext)
    : mBaseDT(aDT), mFontContext(aFontContext) {}
>>>>>>> upstream-releases

<<<<<<< HEAD
bool InlineTranslator::TranslateRecording(char *aData, size_t aLen) {
||||||| merged common ancestors
bool
InlineTranslator::TranslateRecording(char *aData, size_t aLen)
{
=======
bool InlineTranslator::TranslateRecording(char* aData, size_t aLen) {
>>>>>>> upstream-releases
  // an istream like class for reading from memory
  struct MemReader {
<<<<<<< HEAD
    MemReader(char *aData, size_t aLen) : mData(aData), mEnd(aData + aLen) {}
    void read(char *s, std::streamsize n) {
||||||| merged common ancestors
    MemReader(char *aData, size_t aLen) : mData(aData), mEnd(aData + aLen) {}
    void read(char* s, std::streamsize n) {
=======
    MemReader(char* aData, size_t aLen) : mData(aData), mEnd(aData + aLen) {}
    void read(char* s, std::streamsize n) {
>>>>>>> upstream-releases
      if (n <= (mEnd - mData)) {
        memcpy(s, mData, n);
        mData += n;
      } else {
        // We've requested more data than is available
        // set the Reader into an eof state
        SetIsBad();
      }
    }
<<<<<<< HEAD
    bool eof() { return mData > mEnd; }
    bool good() { return !eof(); }
||||||| merged common ancestors
    bool eof() {
      return mData > mEnd;
    }
    bool good() {
      return !eof();
    }
=======
    bool eof() { return mData > mEnd; }
    bool good() { return !eof(); }
    void SetIsBad() { mData = mEnd + 1; }
>>>>>>> upstream-releases

    char* mData;
    char* mEnd;
  };
  MemReader reader(aData, aLen);

  uint32_t magicInt;
  ReadElement(reader, magicInt);
  if (magicInt != mozilla::gfx::kMagicInt) {
    mError = "Magic";
    return false;
  }

  uint16_t majorRevision;
  ReadElement(reader, majorRevision);
  if (majorRevision != kMajorRevision) {
    mError = "Major";
    return false;
  }

  uint16_t minorRevision;
  ReadElement(reader, minorRevision);
  if (minorRevision > kMinorRevision) {
    mError = "Minor";
    return false;
  }

  int32_t eventType;
  ReadElement(reader, eventType);
  while (reader.good()) {
<<<<<<< HEAD
    bool success = RecordedEvent::DoWithEvent(
        reader, static_cast<RecordedEvent::EventType>(eventType),
        [&](RecordedEvent *recordedEvent) {
          // Make sure that the whole event was read from the stream
          // successfully.
          if (!reader.good()) {
            mError = " READ";
            return false;
          }

          if (!recordedEvent->PlayEvent(this)) {
            mError = " PLAY";
            return false;
          }

          return true;
        });
||||||| merged common ancestors
    bool success = RecordedEvent::DoWithEvent(reader, static_cast<RecordedEvent::EventType>(eventType),
                               [&] (RecordedEvent *recordedEvent) {
                                 // Make sure that the whole event was read from the stream successfully.
                                 if (!reader.good()) {
                                     return false;
                                 }

                                 if (!recordedEvent->PlayEvent(this)) {
                                     return false;
                                 }

                                 return true;
                              });
=======
    bool success = RecordedEvent::DoWithEvent(
        reader, static_cast<RecordedEvent::EventType>(eventType),
        [&](RecordedEvent* recordedEvent) -> bool {
          // Make sure that the whole event was read from the stream
          // successfully.
          if (!reader.good()) {
            mError = " READ";
            return false;
          }

          if (!recordedEvent->PlayEvent(this)) {
            mError = " PLAY";
            return false;
          }

          return true;
        });
>>>>>>> upstream-releases
    if (!success) {
      mError = RecordedEvent::GetEventName(
                   static_cast<RecordedEvent::EventType>(eventType)) +
               mError;
      return false;
    }

    ReadElement(reader, eventType);
  }

  return true;
}

<<<<<<< HEAD
already_AddRefed<DrawTarget> InlineTranslator::CreateDrawTarget(
    ReferencePtr aRefPtr, const gfx::IntSize &aSize,
    gfx::SurfaceFormat aFormat) {
||||||| merged common ancestors
already_AddRefed<DrawTarget>
InlineTranslator::CreateDrawTarget(ReferencePtr aRefPtr,
                                  const gfx::IntSize &aSize,
                                  gfx::SurfaceFormat aFormat)
{
=======
already_AddRefed<DrawTarget> InlineTranslator::CreateDrawTarget(
    ReferencePtr aRefPtr, const gfx::IntSize& aSize,
    gfx::SurfaceFormat aFormat) {
>>>>>>> upstream-releases
  RefPtr<DrawTarget> drawTarget = mBaseDT;
  AddDrawTarget(aRefPtr, drawTarget);
  return drawTarget.forget();
}

}  // namespace gfx
}  // namespace mozilla
