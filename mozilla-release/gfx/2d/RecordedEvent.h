/* -*- Mode: C++; tab-width: 8; indent-tabs-mode: nil; c-basic-offset: 2 -*- */
/* vim: set ts=8 sts=2 et sw=2 tw=80: */
/* This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/. */

#ifndef MOZILLA_GFX_RECORDEDEVENT_H_
#define MOZILLA_GFX_RECORDEDEVENT_H_

#include "2D.h"
#include <ostream>
#include <sstream>
#include <cstring>
#include <vector>

#include "RecordingTypes.h"

namespace mozilla {
namespace gfx {

const uint32_t kMagicInt = 0xc001feed;

// A change in major revision means a change in event binary format, causing
// loss of backwards compatibility. Old streams will not work in a player
// using a newer major revision. And new streams will not work in a player
// using an older major revision.
const uint16_t kMajorRevision = 10;
// A change in minor revision means additions of new events. New streams will
// not play in older players.
const uint16_t kMinorRevision = 0;

struct ReferencePtr {
  ReferencePtr() : mLongPtr(0) {}

<<<<<<< HEAD
  MOZ_IMPLICIT ReferencePtr(const void *aLongPtr)
      : mLongPtr(uint64_t(aLongPtr)) {}
||||||| merged common ancestors
  MOZ_IMPLICIT ReferencePtr(const void* aLongPtr)
    : mLongPtr(uint64_t(aLongPtr))
  {}
=======
  MOZ_IMPLICIT ReferencePtr(const void* aLongPtr)
      : mLongPtr(uint64_t(aLongPtr)) {}
>>>>>>> upstream-releases

  template <typename T>
<<<<<<< HEAD
  MOZ_IMPLICIT ReferencePtr(const RefPtr<T> &aPtr)
      : mLongPtr(uint64_t(aPtr.get())) {}
||||||| merged common ancestors
  MOZ_IMPLICIT ReferencePtr(const RefPtr<T>& aPtr)
    : mLongPtr(uint64_t(aPtr.get()))
  {}
=======
  MOZ_IMPLICIT ReferencePtr(const RefPtr<T>& aPtr)
      : mLongPtr(uint64_t(aPtr.get())) {}
>>>>>>> upstream-releases

<<<<<<< HEAD
  ReferencePtr &operator=(const void *aLongPtr) {
||||||| merged common ancestors
  ReferencePtr &operator =(const void* aLongPtr) {
=======
  ReferencePtr& operator=(const void* aLongPtr) {
>>>>>>> upstream-releases
    mLongPtr = uint64_t(aLongPtr);
    return *this;
  }

  template <typename T>
<<<<<<< HEAD
  ReferencePtr &operator=(const RefPtr<T> &aPtr) {
||||||| merged common ancestors
  ReferencePtr &operator =(const RefPtr<T>& aPtr) {
=======
  ReferencePtr& operator=(const RefPtr<T>& aPtr) {
>>>>>>> upstream-releases
    mLongPtr = uint64_t(aPtr.get());
    return *this;
  }

<<<<<<< HEAD
  operator void *() const { return (void *)mLongPtr; }
||||||| merged common ancestors
  operator void*() const {
    return (void*)mLongPtr;
  }
=======
  operator void*() const { return (void*)mLongPtr; }
>>>>>>> upstream-releases

  uint64_t mLongPtr;
};

struct RecordedFontDetails {
  uint64_t fontDataKey;
  uint32_t size;
  uint32_t index;
};

// Used by the Azure drawing debugger (player2d)
inline std::string StringFromPtr(ReferencePtr aPtr) {
  std::stringstream stream;
  stream << aPtr;
  return stream.str();
}

<<<<<<< HEAD
class Translator {
 public:
  virtual ~Translator() {}

  virtual DrawTarget *LookupDrawTarget(ReferencePtr aRefPtr) = 0;
  virtual Path *LookupPath(ReferencePtr aRefPtr) = 0;
  virtual SourceSurface *LookupSourceSurface(ReferencePtr aRefPtr) = 0;
  virtual FilterNode *LookupFilterNode(ReferencePtr aRefPtr) = 0;
  virtual GradientStops *LookupGradientStops(ReferencePtr aRefPtr) = 0;
  virtual ScaledFont *LookupScaledFont(ReferencePtr aRefPtr) = 0;
  virtual UnscaledFont *LookupUnscaledFont(ReferencePtr aRefPtr) = 0;
  virtual NativeFontResource *LookupNativeFontResource(uint64_t aKey) = 0;
  virtual already_AddRefed<SourceSurface> LookupExternalSurface(uint64_t aKey) {
    return nullptr;
  }
  virtual void AddDrawTarget(ReferencePtr aRefPtr, DrawTarget *aDT) = 0;
||||||| merged common ancestors
class Translator
{
public:
  virtual ~Translator() {}

  virtual DrawTarget *LookupDrawTarget(ReferencePtr aRefPtr) = 0;
  virtual Path *LookupPath(ReferencePtr aRefPtr) = 0;
  virtual SourceSurface *LookupSourceSurface(ReferencePtr aRefPtr) = 0;
  virtual FilterNode *LookupFilterNode(ReferencePtr aRefPtr) = 0;
  virtual GradientStops *LookupGradientStops(ReferencePtr aRefPtr) = 0;
  virtual ScaledFont *LookupScaledFont(ReferencePtr aRefPtr) = 0;
  virtual UnscaledFont* LookupUnscaledFont(ReferencePtr aRefPtr) = 0;
  virtual NativeFontResource *LookupNativeFontResource(uint64_t aKey) = 0;
  virtual already_AddRefed<SourceSurface> LookupExternalSurface(uint64_t aKey) { return nullptr; }
  virtual void AddDrawTarget(ReferencePtr aRefPtr, DrawTarget *aDT) = 0;
=======
class Translator {
 public:
  virtual ~Translator() = default;

  virtual DrawTarget* LookupDrawTarget(ReferencePtr aRefPtr) = 0;
  virtual Path* LookupPath(ReferencePtr aRefPtr) = 0;
  virtual SourceSurface* LookupSourceSurface(ReferencePtr aRefPtr) = 0;
  virtual FilterNode* LookupFilterNode(ReferencePtr aRefPtr) = 0;
  virtual GradientStops* LookupGradientStops(ReferencePtr aRefPtr) = 0;
  virtual ScaledFont* LookupScaledFont(ReferencePtr aRefPtr) = 0;
  virtual UnscaledFont* LookupUnscaledFont(ReferencePtr aRefPtr) = 0;
  virtual NativeFontResource* LookupNativeFontResource(uint64_t aKey) = 0;
  virtual already_AddRefed<SourceSurface> LookupExternalSurface(uint64_t aKey) {
    return nullptr;
  }
  virtual void AddDrawTarget(ReferencePtr aRefPtr, DrawTarget* aDT) = 0;
>>>>>>> upstream-releases
  virtual void RemoveDrawTarget(ReferencePtr aRefPtr) = 0;
  virtual void AddPath(ReferencePtr aRefPtr, Path* aPath) = 0;
  virtual void RemovePath(ReferencePtr aRefPtr) = 0;
  virtual void AddSourceSurface(ReferencePtr aRefPtr, SourceSurface* aPath) = 0;
  virtual void RemoveSourceSurface(ReferencePtr aRefPtr) = 0;
<<<<<<< HEAD
  virtual void AddFilterNode(mozilla::gfx::ReferencePtr aRefPtr,
                             FilterNode *aSurface) = 0;
||||||| merged common ancestors
  virtual void AddFilterNode(mozilla::gfx::ReferencePtr aRefPtr, FilterNode *aSurface) = 0;
=======
  virtual void AddFilterNode(mozilla::gfx::ReferencePtr aRefPtr,
                             FilterNode* aSurface) = 0;
>>>>>>> upstream-releases
  virtual void RemoveFilterNode(mozilla::gfx::ReferencePtr aRefPtr) = 0;
  virtual void AddGradientStops(ReferencePtr aRefPtr, GradientStops* aPath) = 0;
  virtual void RemoveGradientStops(ReferencePtr aRefPtr) = 0;
  virtual void AddScaledFont(ReferencePtr aRefPtr, ScaledFont* aScaledFont) = 0;
  virtual void RemoveScaledFont(ReferencePtr aRefPtr) = 0;
<<<<<<< HEAD
  virtual void AddUnscaledFont(ReferencePtr aRefPtr,
                               UnscaledFont *aUnscaledFont) = 0;
||||||| merged common ancestors
  virtual void AddUnscaledFont(ReferencePtr aRefPtr, UnscaledFont* aUnscaledFont) = 0;
=======
  virtual void AddUnscaledFont(ReferencePtr aRefPtr,
                               UnscaledFont* aUnscaledFont) = 0;
>>>>>>> upstream-releases
  virtual void RemoveUnscaledFont(ReferencePtr aRefPtr) = 0;
<<<<<<< HEAD
  virtual void AddNativeFontResource(
      uint64_t aKey, NativeFontResource *aNativeFontResource) = 0;
||||||| merged common ancestors
  virtual void AddNativeFontResource(uint64_t aKey,
                                     NativeFontResource *aNativeFontResource) = 0;
=======
  virtual void AddNativeFontResource(
      uint64_t aKey, NativeFontResource* aNativeFontResource) = 0;
>>>>>>> upstream-releases

  virtual already_AddRefed<DrawTarget> CreateDrawTarget(ReferencePtr aRefPtr,
                                                        const IntSize& aSize,
                                                        SurfaceFormat aFormat);
<<<<<<< HEAD
  virtual DrawTarget *GetReferenceDrawTarget() = 0;
  virtual void *GetFontContext() { return nullptr; }
||||||| merged common ancestors
  virtual DrawTarget *GetReferenceDrawTarget() = 0;
  virtual void* GetFontContext() { return nullptr; }
=======
  virtual DrawTarget* GetReferenceDrawTarget() = 0;
  virtual void* GetFontContext() { return nullptr; }
>>>>>>> upstream-releases
};

struct ColorPatternStorage {
  Color mColor;
};

struct LinearGradientPatternStorage {
  Point mBegin;
  Point mEnd;
  ReferencePtr mStops;
  Matrix mMatrix;
};

struct RadialGradientPatternStorage {
  Point mCenter1;
  Point mCenter2;
  Float mRadius1;
  Float mRadius2;
  ReferencePtr mStops;
  Matrix mMatrix;
};

struct SurfacePatternStorage {
  ExtendMode mExtend;
  SamplingFilter mSamplingFilter;
  ReferencePtr mSurface;
  Matrix mMatrix;
  IntRect mSamplingRect;
};

struct PatternStorage {
  PatternType mType;
  union {
    char* mStorage;
    char mColor[sizeof(ColorPatternStorage)];
    char mLinear[sizeof(LinearGradientPatternStorage)];
    char mRadial[sizeof(RadialGradientPatternStorage)];
    char mSurface[sizeof(SurfacePatternStorage)];
  };
};

/* SizeCollector and MemWriter are used
 * in a pair to first collect the size of the
 * event that we're going to write and then
 * to write it without checking each individual
 * size. */
struct SizeCollector {
  SizeCollector() : mTotalSize(0) {}
<<<<<<< HEAD
  void write(const char *, size_t s) { mTotalSize += s; }
||||||| merged common ancestors
    void write(const char*, size_t s) {
      mTotalSize += s;
    }
=======
  void write(const char*, size_t s) { mTotalSize += s; }
>>>>>>> upstream-releases
  size_t mTotalSize;
};

struct MemWriter {
<<<<<<< HEAD
  explicit MemWriter(char *aPtr) : mPtr(aPtr) {}
  void write(const char *aData, size_t aSize) {
    memcpy(mPtr, aData, aSize);
    mPtr += aSize;
  }
  char *mPtr;
||||||| merged common ancestors
  explicit MemWriter(char* aPtr) : mPtr(aPtr) {}
    void write(const char* aData, size_t aSize) {
       memcpy(mPtr, aData, aSize);
       mPtr += aSize;
    }
  char* mPtr;
=======
  explicit MemWriter(char* aPtr) : mPtr(aPtr) {}
  void write(const char* aData, size_t aSize) {
    memcpy(mPtr, aData, aSize);
    mPtr += aSize;
  }
  char* mPtr;
>>>>>>> upstream-releases
};

// This is a simple interface for an EventRingBuffer, so we can use it in the
// RecordedEvent reading and writing machinery.
class EventRingBuffer {
 public:
  /**
   * Templated RecordEvent function so that when we have enough contiguous
   * space we can record into the buffer quickly using MemWriter.
   *
   * @param aRecordedEvent the event to record
   */
  template <class RE>
  void RecordEvent(const RE* aRecordedEvent) {
    SizeCollector size;
    WriteElement(size, aRecordedEvent->GetType());
    aRecordedEvent->Record(size);
    if (size.mTotalSize > mAvailable) {
      WaitForAndRecalculateAvailableSpace();
    }
    if (size.mTotalSize <= mAvailable) {
      MemWriter writer(mBufPos);
      WriteElement(writer, aRecordedEvent->GetType());
      aRecordedEvent->Record(writer);
      UpdateWriteTotalsBy(size.mTotalSize);
    } else {
      WriteElement(*this, aRecordedEvent->GetType());
      aRecordedEvent->Record(*this);
    }
  }

  /**
   * Simple write function required by WriteElement.
   *
   * @param aData the data to be written to the buffer
   * @param aSize the number of chars to write
   */
  virtual void write(const char* const aData, const size_t aSize) = 0;

  /**
   * Simple read function required by ReadElement.
   *
   * @param aOut the pointer to read into
   * @param aSize the number of chars to read
   */
  virtual void read(char* const aOut, const size_t aSize) = 0;

  virtual bool good() const = 0;

  virtual void SetIsBad() = 0;

 protected:
  /**
   * Wait until space is available for writing and then set mBufPos and
   * mAvailable.
   */
  virtual bool WaitForAndRecalculateAvailableSpace() = 0;

  /**
   * Update write count, mBufPos and mAvailable.
   *
   * @param aCount number of bytes written
   */
  virtual void UpdateWriteTotalsBy(uint32_t aCount) = 0;

  char* mBufPos = nullptr;
  uint32_t mAvailable = 0;
};

struct MemStream {
  char* mData;
  size_t mLength;
  size_t mCapacity;
  void Resize(size_t aSize) {
    mLength = aSize;
    if (mLength > mCapacity) {
      mCapacity = mCapacity * 2;
      // check if the doubled capacity is enough
      // otherwise use double mLength
      if (mLength > mCapacity) {
        mCapacity = mLength * 2;
      }
      mData = (char *)realloc(mData, mCapacity);
    }
  }

  void write(const char *aData, size_t aSize) {
    Resize(mLength + aSize);
    memcpy(mData + mLength - aSize, aData, aSize);
  }

  MemStream() : mData(nullptr), mLength(0), mCapacity(0) {}
  ~MemStream() { free(mData); }
};

class EventStream {
<<<<<<< HEAD
 public:
  virtual void write(const char *aData, size_t aSize) = 0;
  virtual void read(char *aOut, size_t aSize) = 0;
||||||| merged common ancestors
public:
  virtual void write(const char* aData, size_t aSize) = 0;
  virtual void read(char* aOut, size_t aSize) = 0;
=======
 public:
  virtual void write(const char* aData, size_t aSize) = 0;
  virtual void read(char* aOut, size_t aSize) = 0;
  virtual bool good() = 0;
  virtual void SetIsBad() = 0;
>>>>>>> upstream-releases
};

class RecordedEvent {
 public:
  enum EventType {
    DRAWTARGETCREATION = 0,
    DRAWTARGETDESTRUCTION,
    FILLRECT,
    STROKERECT,
    STROKELINE,
    CLEARRECT,
    COPYSURFACE,
    SETTRANSFORM,
    PUSHCLIP,
    PUSHCLIPRECT,
    POPCLIP,
    FILL,
    FILLGLYPHS,
    MASK,
    STROKE,
    DRAWSURFACE,
    DRAWDEPENDENTSURFACE,
    DRAWSURFACEWITHSHADOW,
    PATHCREATION,
    PATHDESTRUCTION,
    SOURCESURFACECREATION,
    SOURCESURFACEDESTRUCTION,
    GRADIENTSTOPSCREATION,
    GRADIENTSTOPSDESTRUCTION,
    SNAPSHOT,
    SCALEDFONTCREATION,
    SCALEDFONTDESTRUCTION,
    MASKSURFACE,
    FILTERNODECREATION,
    FILTERNODEDESTRUCTION,
    DRAWFILTER,
    FILTERNODESETATTRIBUTE,
    FILTERNODESETINPUT,
    CREATESIMILARDRAWTARGET,
    CREATECLIPPEDDRAWTARGET,
    CREATEDRAWTARGETFORFILTER,
    FONTDATA,
    FONTDESC,
    PUSHLAYER,
    PUSHLAYERWITHBLEND,
    POPLAYER,
    UNSCALEDFONTCREATION,
    UNSCALEDFONTDESTRUCTION,
    INTOLUMINANCE,
    EXTERNALSURFACECREATION,
    FLUSH,
    DETACHALLSNAPSHOTS,
    LAST,
  };
<<<<<<< HEAD
  static const uint32_t kTotalEventTypes =
      RecordedEvent::FILTERNODESETINPUT + 1;
||||||| merged common ancestors
  static const uint32_t kTotalEventTypes = RecordedEvent::FILTERNODESETINPUT + 1;
=======
>>>>>>> upstream-releases

  virtual ~RecordedEvent() = default;

  static std::string GetEventName(EventType aType);

  /**
   * Play back this event using the translator. Note that derived classes
   * should
   * only return false when there is a fatal error, as it will probably mean
   * the
   * translation will abort.
   * @param aTranslator Translator to be used for retrieving other referenced
   *                    objects and making playback decisions.
   * @return true unless a fatal problem has occurred and playback should
   * abort.
   */
  virtual bool PlayEvent(Translator* aTranslator) const { return true; }

<<<<<<< HEAD
  virtual void RecordToStream(std::ostream &aStream) const = 0;
  virtual void RecordToStream(EventStream &aStream) const = 0;
  virtual void RecordToStream(MemStream &aStream) const = 0;
||||||| merged common ancestors
  virtual void RecordToStream(std::ostream& aStream) const = 0;
  virtual void RecordToStream(EventStream& aStream) const = 0;
  virtual void RecordToStream(MemStream& aStream) const = 0;
=======
  virtual void RecordToStream(std::ostream& aStream) const = 0;
  virtual void RecordToStream(EventStream& aStream) const = 0;
  virtual void RecordToStream(EventRingBuffer& aStream) const = 0;
  virtual void RecordToStream(MemStream& aStream) const = 0;
>>>>>>> upstream-releases

<<<<<<< HEAD
  virtual void OutputSimpleEventInfo(std::stringstream &aStringStream) const {}
||||||| merged common ancestors
  virtual void OutputSimpleEventInfo(std::stringstream &aStringStream) const { }
=======
  virtual void OutputSimpleEventInfo(std::stringstream& aStringStream) const {}
>>>>>>> upstream-releases

<<<<<<< HEAD
  template <class S>
  void RecordPatternData(S &aStream,
                         const PatternStorage &aPatternStorage) const;
  template <class S>
  void ReadPatternData(S &aStream, PatternStorage &aPatternStorage) const;
  void StorePattern(PatternStorage &aDestination, const Pattern &aSource) const;
  template <class S>
  void RecordStrokeOptions(S &aStream,
                           const StrokeOptions &aStrokeOptions) const;
  template <class S>
  void ReadStrokeOptions(S &aStream, StrokeOptions &aStrokeOptions);
||||||| merged common ancestors
  template<class S>
  void RecordPatternData(S &aStream, const PatternStorage &aPatternStorage) const;
  template<class S>
  void ReadPatternData(S &aStream, PatternStorage &aPatternStorage) const;
  void StorePattern(PatternStorage &aDestination, const Pattern &aSource) const;
  template<class S>
  void RecordStrokeOptions(S &aStream, const StrokeOptions &aStrokeOptions) const;
  template<class S>
  void ReadStrokeOptions(S &aStream, StrokeOptions &aStrokeOptions);
=======
  template <class S>
  void RecordPatternData(S& aStream,
                         const PatternStorage& aPatternStorage) const;
  template <class S>
  void ReadPatternData(S& aStream, PatternStorage& aPatternStorage) const;
  void StorePattern(PatternStorage& aDestination, const Pattern& aSource) const;
  template <class S>
  void RecordStrokeOptions(S& aStream,
                           const StrokeOptions& aStrokeOptions) const;
  template <class S>
  void ReadStrokeOptions(S& aStream, StrokeOptions& aStrokeOptions);
>>>>>>> upstream-releases

  virtual std::string GetName() const = 0;

  virtual ReferencePtr GetDestinedDT() { return nullptr; }

<<<<<<< HEAD
  void OutputSimplePatternInfo(const PatternStorage &aStorage,
                               std::stringstream &aOutput) const;
||||||| merged common ancestors
  void OutputSimplePatternInfo(const PatternStorage &aStorage, std::stringstream &aOutput) const;
=======
  void OutputSimplePatternInfo(const PatternStorage& aStorage,
                               std::stringstream& aOutput) const;
>>>>>>> upstream-releases

<<<<<<< HEAD
  template <class S>
  static RecordedEvent *LoadEvent(S &aStream, EventType aType);
  static RecordedEvent *LoadEventFromStream(std::istream &aStream,
                                            EventType aType);
  static RecordedEvent *LoadEventFromStream(EventStream &aStream,
                                            EventType aType);

  // An alternative to LoadEvent that avoids a heap allocation for the event.
  // This accepts a callable `f' that will take a RecordedEvent* as a single
  // parameter
  template <class S, class F>
  static bool DoWithEvent(S &aStream, EventType aType, F f);
||||||| merged common ancestors
  template<class S>
  static RecordedEvent *LoadEvent(S &aStream, EventType aType);
  static RecordedEvent *LoadEventFromStream(std::istream &aStream, EventType aType);
  static RecordedEvent *LoadEventFromStream(EventStream& aStream, EventType aType);

  // An alternative to LoadEvent that avoids a heap allocation for the event.
  // This accepts a callable `f' that will take a RecordedEvent* as a single parameter
  template<class S, class F>
  static bool DoWithEvent(S &aStream, EventType aType, F f);
=======
  template <class S>
  static bool DoWithEvent(S& aStream, EventType aType,
                          const std::function<bool(RecordedEvent*)>& aAction);
  static bool DoWithEventFromStream(
      EventStream& aStream, EventType aType,
      const std::function<bool(RecordedEvent*)>& aAction);
  static bool DoWithEventFromStream(
      EventRingBuffer& aStream, EventType aType,
      const std::function<bool(RecordedEvent*)>& aAction);
>>>>>>> upstream-releases

  EventType GetType() const { return (EventType)mType; }

 protected:
  friend class DrawEventRecorderPrivate;
  friend class DrawEventRecorderFile;
  friend class DrawEventRecorderMemory;
<<<<<<< HEAD
  static void RecordUnscaledFont(UnscaledFont *aUnscaledFont,
                                 std::ostream *aOutput);
  static void RecordUnscaledFont(UnscaledFont *aUnscaledFont,
                                 MemStream &aOutput);
  template <class S>
  static void RecordUnscaledFontImpl(UnscaledFont *aUnscaledFont, S &aOutput);
||||||| merged common ancestors
  static void RecordUnscaledFont(UnscaledFont *aUnscaledFont, std::ostream *aOutput);
  static void RecordUnscaledFont(UnscaledFont *aUnscaledFont, MemStream &aOutput);
  template<class S>
  static void RecordUnscaledFontImpl(UnscaledFont *aUnscaledFont, S &aOutput);
=======
  static void RecordUnscaledFont(UnscaledFont* aUnscaledFont,
                                 std::ostream* aOutput);
  static void RecordUnscaledFont(UnscaledFont* aUnscaledFont,
                                 MemStream& aOutput);
  template <class S>
  static void RecordUnscaledFontImpl(UnscaledFont* aUnscaledFont, S& aOutput);
>>>>>>> upstream-releases

  MOZ_IMPLICIT RecordedEvent(int32_t aType) : mType(aType) {}

  int32_t mType;
  std::vector<Float> mDashPatternStorage;
};

<<<<<<< HEAD
}  // namespace gfx
}  // namespace mozilla
||||||| merged common ancestors
} // namespace gfx
} // namespace mozilla
=======
template <class Derived>
class RecordedEventDerived : public RecordedEvent {
  using RecordedEvent::RecordedEvent;

 public:
  void RecordToStream(std::ostream& aStream) const override {
    WriteElement(aStream, this->mType);
    static_cast<const Derived*>(this)->Record(aStream);
  }
  void RecordToStream(EventStream& aStream) const override {
    WriteElement(aStream, this->mType);
    static_cast<const Derived*>(this)->Record(aStream);
  }
  void RecordToStream(EventRingBuffer& aStream) const final {
    aStream.RecordEvent(static_cast<const Derived*>(this));
  }
  void RecordToStream(MemStream& aStream) const override {
    SizeCollector size;
    WriteElement(size, this->mType);
    static_cast<const Derived*>(this)->Record(size);

    aStream.Resize(aStream.mLength + size.mTotalSize);

    MemWriter writer(aStream.mData + aStream.mLength - size.mTotalSize);
    WriteElement(writer, this->mType);
    static_cast<const Derived*>(this)->Record(writer);
  }
};

}  // namespace gfx
}  // namespace mozilla
>>>>>>> upstream-releases

#endif
