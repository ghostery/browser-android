/* -*- Mode: C++; tab-width: 8; indent-tabs-mode: nil; c-basic-offset: 2 -*- */
/* vim: set ts=8 sts=2 et sw=2 tw=80: */
/* This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/. */

#include "DrawEventRecorder.h"
#include "PathRecording.h"
#include "RecordingTypes.h"
#include "RecordedEventImpl.h"

namespace mozilla {
namespace gfx {

using namespace std;

DrawEventRecorderPrivate::DrawEventRecorderPrivate() : mExternalFonts(false) {}

<<<<<<< HEAD
void DrawEventRecorderPrivate::StoreExternalSurfaceRecording(
    SourceSurface *aSurface, uint64_t aKey) {
||||||| merged common ancestors
void
DrawEventRecorderPrivate::StoreExternalSurfaceRecording(SourceSurface* aSurface,
                                                        uint64_t aKey)
{
=======
void DrawEventRecorderPrivate::StoreExternalSurfaceRecording(
    SourceSurface* aSurface, uint64_t aKey) {
>>>>>>> upstream-releases
  RecordEvent(RecordedExternalSurfaceCreation(aSurface, aKey));
  mExternalSurfaces.push_back(aSurface);
}

<<<<<<< HEAD
void DrawEventRecorderPrivate::StoreSourceSurfaceRecording(
    SourceSurface *aSurface, const char *aReason) {
||||||| merged common ancestors
void
DrawEventRecorderPrivate::StoreSourceSurfaceRecording(SourceSurface *aSurface,
                                                      const char *aReason)
{
=======
void DrawEventRecorderPrivate::StoreSourceSurfaceRecording(
    SourceSurface* aSurface, const char* aReason) {
>>>>>>> upstream-releases
  RefPtr<DataSourceSurface> dataSurf = aSurface->GetDataSurface();
  if (dataSurf) {
    DataSourceSurface::ScopedMap map(dataSurf, DataSourceSurface::READ);
    RecordEvent(RecordedSourceSurfaceCreation(
        aSurface, map.GetData(), map.GetStride(), dataSurf->GetSize(),
        dataSurf->GetFormat()));
    return;
  }

  gfxWarning() << "Recording failed to record SourceSurface for " << aReason;
  // Insert a bogus source surface.
  int32_t stride =
      aSurface->GetSize().width * BytesPerPixel(aSurface->GetFormat());
  UniquePtr<uint8_t[]> sourceData(
      new uint8_t[stride * aSurface->GetSize().height]());
  RecordEvent(RecordedSourceSurfaceCreation(aSurface, sourceData.get(), stride,
                                            aSurface->GetSize(),
                                            aSurface->GetFormat()));
}

<<<<<<< HEAD
void DrawEventRecorderFile::RecordEvent(const RecordedEvent &aEvent) {
  WriteElement(mOutputStream, aEvent.mType);

||||||| merged common ancestors
void
DrawEventRecorderFile::RecordEvent(const RecordedEvent &aEvent)
{
  WriteElement(mOutputStream, aEvent.mType);

=======
void DrawEventRecorderFile::RecordEvent(const RecordedEvent& aEvent) {
>>>>>>> upstream-releases
  aEvent.RecordToStream(mOutputStream);

  Flush();
}

<<<<<<< HEAD
void DrawEventRecorderMemory::RecordEvent(const RecordedEvent &aEvent) {
  WriteElement(mOutputStream, aEvent.mType);

||||||| merged common ancestors
void
DrawEventRecorderMemory::RecordEvent(const RecordedEvent &aEvent)
{
  WriteElement(mOutputStream, aEvent.mType);

=======
void DrawEventRecorderMemory::RecordEvent(const RecordedEvent& aEvent) {
>>>>>>> upstream-releases
  aEvent.RecordToStream(mOutputStream);
}

void DrawEventRecorderMemory::AddDependentSurface(uint64_t aDependencyId) {
  mDependentSurfaces.PutEntry(aDependencyId);
}

<<<<<<< HEAD
nsTHashtable<nsUint64HashKey>
    &&DrawEventRecorderMemory::TakeDependentSurfaces() {
||||||| merged common ancestors
nsTHashtable<nsUint64HashKey>&&
DrawEventRecorderMemory::TakeDependentSurfaces()
{
=======
nsTHashtable<nsUint64HashKey>&&
DrawEventRecorderMemory::TakeDependentSurfaces() {
>>>>>>> upstream-releases
  return std::move(mDependentSurfaces);
}

<<<<<<< HEAD
DrawEventRecorderFile::DrawEventRecorderFile(const char_type *aFilename)
    : mOutputStream(aFilename, ofstream::binary) {
||||||| merged common ancestors
DrawEventRecorderFile::DrawEventRecorderFile(const char_type* aFilename)
  : mOutputStream(aFilename, ofstream::binary)
{
=======
DrawEventRecorderFile::DrawEventRecorderFile(const char_type* aFilename)
    : mOutputStream(aFilename, ofstream::binary) {
>>>>>>> upstream-releases
  WriteHeader(mOutputStream);
}

DrawEventRecorderFile::~DrawEventRecorderFile() { mOutputStream.close(); }

void DrawEventRecorderFile::Flush() { mOutputStream.flush(); }

bool DrawEventRecorderFile::IsOpen() { return mOutputStream.is_open(); }

<<<<<<< HEAD
void DrawEventRecorderFile::OpenNew(const char_type *aFilename) {
||||||| merged common ancestors
void
DrawEventRecorderFile::OpenNew(const char_type* aFilename)
{
=======
void DrawEventRecorderFile::OpenNew(const char_type* aFilename) {
>>>>>>> upstream-releases
  MOZ_ASSERT(!mOutputStream.is_open());

  mOutputStream.open(aFilename, ofstream::binary);
  WriteHeader(mOutputStream);
}

void DrawEventRecorderFile::Close() {
  MOZ_ASSERT(mOutputStream.is_open());

  mOutputStream.close();
}

DrawEventRecorderMemory::DrawEventRecorderMemory() {
  WriteHeader(mOutputStream);
}

<<<<<<< HEAD
DrawEventRecorderMemory::DrawEventRecorderMemory(
    const SerializeResourcesFn &aFn)
    : mSerializeCallback(aFn) {
||||||| merged common ancestors
DrawEventRecorderMemory::DrawEventRecorderMemory(const SerializeResourcesFn &aFn) :
  mSerializeCallback(aFn)
{
=======
DrawEventRecorderMemory::DrawEventRecorderMemory(
    const SerializeResourcesFn& aFn)
    : mSerializeCallback(aFn) {
>>>>>>> upstream-releases
  mExternalFonts = !!mSerializeCallback;
  WriteHeader(mOutputStream);
}

void DrawEventRecorderMemory::Flush() {}

void DrawEventRecorderMemory::FlushItem(IntRect aRect) {
  MOZ_RELEASE_ASSERT(!aRect.IsEmpty());
  // Detaching our existing resources will add some
  // destruction events to our stream so we need to do that
  // first.
  DetachResources();

  // See moz2d_renderer.rs for a description of the stream format
  WriteElement(mIndex, mOutputStream.mLength);

  // write out the fonts into the extra data section
  mSerializeCallback(mOutputStream, mScaledFonts);
  WriteElement(mIndex, mOutputStream.mLength);

  WriteElement(mIndex, aRect.x);
  WriteElement(mIndex, aRect.y);
  WriteElement(mIndex, aRect.XMost());
  WriteElement(mIndex, aRect.YMost());
  ClearResources();

  // write out a new header for the next recording in the stream
  WriteHeader(mOutputStream);
}

bool DrawEventRecorderMemory::Finish() {
  // this length might be 0, and things should still work.
  // for example if there are no items in a particular area
  size_t indexOffset = mOutputStream.mLength;
  // write out the index
  mOutputStream.write(mIndex.mData, mIndex.mLength);
  bool hasItems = mIndex.mLength != 0;
  mIndex = MemStream();
  // write out the offset of the Index to the end of the output stream
  WriteElement(mOutputStream, indexOffset);
  ClearResources();
  return hasItems;
}

size_t DrawEventRecorderMemory::RecordingSize() {
  return mOutputStream.mLength;
}

void DrawEventRecorderMemory::WipeRecording() {
  mOutputStream = MemStream();
  mIndex = MemStream();

  WriteHeader(mOutputStream);
}

}  // namespace gfx
}  // namespace mozilla
