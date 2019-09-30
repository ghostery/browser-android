/* -*- Mode: C++; tab-width: 8; indent-tabs-mode: nil; c-basic-offset: 2 -*- */
/* vim: set ts=8 sts=2 et sw=2 tw=80: */
/* This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/. */

#include "ProfilerBacktrace.h"

#include "ProfileBuffer.h"
#include "ProfiledThreadData.h"
#include "ProfileJSONWriter.h"
#include "ThreadInfo.h"

ProfilerBacktrace::ProfilerBacktrace(const char* aName, int aThreadId,
<<<<<<< HEAD
                                     UniquePtr<ProfileBuffer> aBuffer)
    : mName(strdup(aName)), mThreadId(aThreadId), mBuffer(std::move(aBuffer)) {
||||||| merged common ancestors
                                     UniquePtr<ProfileBuffer> aBuffer)
  : mName(strdup(aName))
  , mThreadId(aThreadId)
  , mBuffer(std::move(aBuffer))
{
=======
                                     mozilla::UniquePtr<ProfileBuffer> aBuffer)
    : mName(strdup(aName)), mThreadId(aThreadId), mBuffer(std::move(aBuffer)) {
>>>>>>> upstream-releases
  MOZ_COUNT_CTOR(ProfilerBacktrace);
}

ProfilerBacktrace::~ProfilerBacktrace() { MOZ_COUNT_DTOR(ProfilerBacktrace); }

<<<<<<< HEAD
void ProfilerBacktrace::StreamJSON(SpliceableJSONWriter& aWriter,
                                   const TimeStamp& aProcessStartTime,
                                   UniqueStacks& aUniqueStacks) {
||||||| merged common ancestors
void
ProfilerBacktrace::StreamJSON(SpliceableJSONWriter& aWriter,
                              const TimeStamp& aProcessStartTime,
                              UniqueStacks& aUniqueStacks)
{
=======
void ProfilerBacktrace::StreamJSON(SpliceableJSONWriter& aWriter,
                                   const mozilla::TimeStamp& aProcessStartTime,
                                   UniqueStacks& aUniqueStacks) {
>>>>>>> upstream-releases
  // Unlike ProfiledThreadData::StreamJSON, we don't need to call
  // ProfileBuffer::AddJITInfoForRange because mBuffer does not contain any
  // JitReturnAddr entries. For synchronous samples, JIT frames get expanded
  // at sample time.
<<<<<<< HEAD
  StreamSamplesAndMarkers(mName.get(), mThreadId, *mBuffer.get(), aWriter,
                          aProcessStartTime,
                          /* aRegisterTime */ TimeStamp(),
                          /* aUnregisterTime */ TimeStamp(),
                          /* aSinceTime */ 0, aUniqueStacks);
||||||| merged common ancestors
  StreamSamplesAndMarkers(mName.get(), mThreadId,
                          *mBuffer.get(), aWriter, aProcessStartTime,
                          /* aRegisterTime */ TimeStamp(),
                          /* aUnregisterTime */ TimeStamp(),
                          /* aSinceTime */ 0,
                          aUniqueStacks);
=======
  StreamSamplesAndMarkers(mName.get(), mThreadId, *mBuffer.get(), aWriter,
                          NS_LITERAL_CSTRING(""), aProcessStartTime,
                          /* aRegisterTime */ mozilla::TimeStamp(),
                          /* aUnregisterTime */ mozilla::TimeStamp(),
                          /* aSinceTime */ 0, aUniqueStacks);
>>>>>>> upstream-releases
}
