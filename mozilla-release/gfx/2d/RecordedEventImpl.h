/* -*- Mode: C++; tab-width: 8; indent-tabs-mode: nil; c-basic-offset: 2 -*- */
/* vim: set ts=8 sts=2 et sw=2 tw=80: */
/* This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/. */

#ifndef MOZILLA_GFX_RECORDEDEVENTIMPL_H_
#define MOZILLA_GFX_RECORDEDEVENTIMPL_H_

#include "RecordedEvent.h"

#include "PathRecording.h"
#include "RecordingTypes.h"
#include "Tools.h"
#include "Filters.h"
#include "Logging.h"
#include "ScaledFontBase.h"
#include "SFNTData.h"

namespace mozilla {
namespace gfx {

<<<<<<< HEAD
template <class Derived>
class RecordedEventDerived : public RecordedEvent {
  using RecordedEvent::RecordedEvent;

 public:
  void RecordToStream(std::ostream &aStream) const override {
    static_cast<const Derived *>(this)->Record(aStream);
  }
  void RecordToStream(EventStream &aStream) const override {
    static_cast<const Derived *>(this)->Record(aStream);
  }
  void RecordToStream(MemStream &aStream) const override {
    SizeCollector size;
    static_cast<const Derived *>(this)->Record(size);
    aStream.Resize(aStream.mLength + size.mTotalSize);
    MemWriter writer(aStream.mData + aStream.mLength - size.mTotalSize);
    static_cast<const Derived *>(this)->Record(writer);
  }
};

template <class Derived>
class RecordedDrawingEvent : public RecordedEventDerived<Derived> {
 public:
  virtual ReferencePtr GetDestinedDT() override { return mDT; }

 protected:
  RecordedDrawingEvent(RecordedEvent::EventType aType, DrawTarget *aTarget)
      : RecordedEventDerived<Derived>(aType), mDT(aTarget) {}
||||||| merged common ancestors
template<class Derived>
class RecordedEventDerived : public RecordedEvent {
  using RecordedEvent::RecordedEvent;
  public:
  void RecordToStream(std::ostream &aStream) const override {
    static_cast<const Derived*>(this)->Record(aStream);
  }
  void RecordToStream(EventStream& aStream) const override {
    static_cast<const Derived*>(this)->Record(aStream);
  }
  void RecordToStream(MemStream &aStream) const override {
    SizeCollector size;
    static_cast<const Derived*>(this)->Record(size);
    aStream.Resize(aStream.mLength + size.mTotalSize);
    MemWriter writer(aStream.mData + aStream.mLength - size.mTotalSize);
    static_cast<const Derived*>(this)->Record(writer);
  }
};

template<class Derived>
class RecordedDrawingEvent : public RecordedEventDerived<Derived>
{
public:
   virtual ReferencePtr GetDestinedDT() override { return mDT; }

protected:
  RecordedDrawingEvent(RecordedEvent::EventType aType, DrawTarget *aTarget)
    : RecordedEventDerived<Derived>(aType), mDT(aTarget)
  {
  }
=======
template <class Derived>
class RecordedDrawingEvent : public RecordedEventDerived<Derived> {
 public:
  ReferencePtr GetDestinedDT() override { return mDT; }
>>>>>>> upstream-releases

<<<<<<< HEAD
  template <class S>
  RecordedDrawingEvent(RecordedEvent::EventType aType, S &aStream);
  template <class S>
  void Record(S &aStream) const;
||||||| merged common ancestors
  template<class S>
  RecordedDrawingEvent(RecordedEvent::EventType aType, S &aStream);
  template<class S>
  void Record(S &aStream) const;
=======
 protected:
  RecordedDrawingEvent(RecordedEvent::EventType aType, DrawTarget* aTarget)
      : RecordedEventDerived<Derived>(aType), mDT(aTarget) {}
>>>>>>> upstream-releases

  template <class S>
  RecordedDrawingEvent(RecordedEvent::EventType aType, S& aStream);
  template <class S>
  void Record(S& aStream) const;

  ReferencePtr mDT;
};

<<<<<<< HEAD
class RecordedDrawTargetCreation
    : public RecordedEventDerived<RecordedDrawTargetCreation> {
 public:
  RecordedDrawTargetCreation(ReferencePtr aRefPtr, BackendType aType,
                             const IntSize &aSize, SurfaceFormat aFormat,
                             bool aHasExistingData = false,
                             SourceSurface *aExistingData = nullptr)
      : RecordedEventDerived(DRAWTARGETCREATION),
        mRefPtr(aRefPtr),
        mBackendType(aType),
        mSize(aSize),
        mFormat(aFormat),
        mHasExistingData(aHasExistingData),
        mExistingData(aExistingData) {}

  virtual bool PlayEvent(Translator *aTranslator) const override;

  template <class S>
  void Record(S &aStream) const;
  virtual void OutputSimpleEventInfo(
      std::stringstream &aStringStream) const override;

  virtual std::string GetName() const override { return "DrawTarget Creation"; }
  virtual ReferencePtr GetObjectRef() const override { return mRefPtr; }
||||||| merged common ancestors
class RecordedDrawTargetCreation : public RecordedEventDerived<RecordedDrawTargetCreation> {
public:
  RecordedDrawTargetCreation(ReferencePtr aRefPtr, BackendType aType, const IntSize &aSize, SurfaceFormat aFormat,
                             bool aHasExistingData = false, SourceSurface *aExistingData = nullptr)
    : RecordedEventDerived(DRAWTARGETCREATION), mRefPtr(aRefPtr), mBackendType(aType), mSize(aSize), mFormat(aFormat)
    , mHasExistingData(aHasExistingData), mExistingData(aExistingData)
  {}

  virtual bool PlayEvent(Translator *aTranslator) const override;

  template<class S>
  void Record(S &aStream) const;
  virtual void OutputSimpleEventInfo(std::stringstream &aStringStream) const override;

  virtual std::string GetName() const override { return "DrawTarget Creation"; }
  virtual ReferencePtr GetObjectRef() const override { return mRefPtr; }
=======
class RecordedDrawTargetCreation
    : public RecordedEventDerived<RecordedDrawTargetCreation> {
 public:
  RecordedDrawTargetCreation(ReferencePtr aRefPtr, BackendType aType,
                             const IntSize& aSize, SurfaceFormat aFormat,
                             bool aHasExistingData = false,
                             SourceSurface* aExistingData = nullptr)
      : RecordedEventDerived(DRAWTARGETCREATION),
        mRefPtr(aRefPtr),
        mBackendType(aType),
        mSize(aSize),
        mFormat(aFormat),
        mHasExistingData(aHasExistingData),
        mExistingData(aExistingData) {}

  bool PlayEvent(Translator* aTranslator) const override;

  template <class S>
  void Record(S& aStream) const;
  virtual void OutputSimpleEventInfo(
      std::stringstream& aStringStream) const override;

  std::string GetName() const override { return "DrawTarget Creation"; }
>>>>>>> upstream-releases

  ReferencePtr mRefPtr;
  BackendType mBackendType;
  IntSize mSize;
  SurfaceFormat mFormat;
  bool mHasExistingData;
  RefPtr<SourceSurface> mExistingData;

 private:
  friend class RecordedEvent;

<<<<<<< HEAD
  template <class S>
  MOZ_IMPLICIT RecordedDrawTargetCreation(S &aStream);
||||||| merged common ancestors
  template<class S>
  MOZ_IMPLICIT RecordedDrawTargetCreation(S &aStream);
=======
  template <class S>
  MOZ_IMPLICIT RecordedDrawTargetCreation(S& aStream);
>>>>>>> upstream-releases
};

class RecordedDrawTargetDestruction
    : public RecordedEventDerived<RecordedDrawTargetDestruction> {
 public:
  MOZ_IMPLICIT RecordedDrawTargetDestruction(ReferencePtr aRefPtr)
      : RecordedEventDerived(DRAWTARGETDESTRUCTION),
        mRefPtr(aRefPtr),
        mBackendType(BackendType::NONE) {}

  bool PlayEvent(Translator* aTranslator) const override;

<<<<<<< HEAD
  template <class S>
  void Record(S &aStream) const;
  virtual void OutputSimpleEventInfo(
      std::stringstream &aStringStream) const override;
||||||| merged common ancestors
  template<class S>
  void Record(S &aStream) const;
  virtual void OutputSimpleEventInfo(std::stringstream &aStringStream) const override;
=======
  template <class S>
  void Record(S& aStream) const;
  void OutputSimpleEventInfo(std::stringstream& aStringStream) const override;
>>>>>>> upstream-releases

<<<<<<< HEAD
  virtual std::string GetName() const override {
    return "DrawTarget Destruction";
  }
  virtual ReferencePtr GetObjectRef() const override { return mRefPtr; }
||||||| merged common ancestors
  virtual std::string GetName() const override { return "DrawTarget Destruction"; }
  virtual ReferencePtr GetObjectRef() const override { return mRefPtr; }
=======
  std::string GetName() const override { return "DrawTarget Destruction"; }
>>>>>>> upstream-releases

  ReferencePtr mRefPtr;

  BackendType mBackendType;

 private:
  friend class RecordedEvent;

<<<<<<< HEAD
  template <class S>
  MOZ_IMPLICIT RecordedDrawTargetDestruction(S &aStream);
||||||| merged common ancestors
  template<class S>
  MOZ_IMPLICIT RecordedDrawTargetDestruction(S &aStream);
=======
  template <class S>
  MOZ_IMPLICIT RecordedDrawTargetDestruction(S& aStream);
>>>>>>> upstream-releases
};

<<<<<<< HEAD
class RecordedCreateSimilarDrawTarget
    : public RecordedEventDerived<RecordedCreateSimilarDrawTarget> {
 public:
  RecordedCreateSimilarDrawTarget(ReferencePtr aRefPtr, const IntSize &aSize,
||||||| merged common ancestors
class RecordedCreateSimilarDrawTarget : public RecordedEventDerived<RecordedCreateSimilarDrawTarget> {
public:
  RecordedCreateSimilarDrawTarget(ReferencePtr aRefPtr, const IntSize &aSize,
=======
class RecordedCreateSimilarDrawTarget
    : public RecordedEventDerived<RecordedCreateSimilarDrawTarget> {
 public:
  RecordedCreateSimilarDrawTarget(ReferencePtr aRefPtr, const IntSize& aSize,
>>>>>>> upstream-releases
                                  SurfaceFormat aFormat)
      : RecordedEventDerived(CREATESIMILARDRAWTARGET),
        mRefPtr(aRefPtr),
        mSize(aSize),
        mFormat(aFormat) {}

  bool PlayEvent(Translator* aTranslator) const override;

<<<<<<< HEAD
  template <class S>
  void Record(S &aStream) const;
  virtual void OutputSimpleEventInfo(
      std::stringstream &aStringStream) const override;
||||||| merged common ancestors
  template<class S>
  void Record(S &aStream) const;
  virtual void OutputSimpleEventInfo(std::stringstream &aStringStream) const override;
=======
  template <class S>
  void Record(S& aStream) const;
  virtual void OutputSimpleEventInfo(
      std::stringstream& aStringStream) const override;
>>>>>>> upstream-releases

<<<<<<< HEAD
  virtual std::string GetName() const override {
    return "CreateSimilarDrawTarget";
  }
  virtual ReferencePtr GetObjectRef() const override { return mRefPtr; }
||||||| merged common ancestors
  virtual std::string GetName() const override { return "CreateSimilarDrawTarget"; }
  virtual ReferencePtr GetObjectRef() const override { return mRefPtr; }
=======
  std::string GetName() const override { return "CreateSimilarDrawTarget"; }
>>>>>>> upstream-releases

  ReferencePtr mRefPtr;
  IntSize mSize;
  SurfaceFormat mFormat;

 private:
  friend class RecordedEvent;

<<<<<<< HEAD
  template <class S>
  MOZ_IMPLICIT RecordedCreateSimilarDrawTarget(S &aStream);
||||||| merged common ancestors
  template<class S>
  MOZ_IMPLICIT RecordedCreateSimilarDrawTarget(S &aStream);
=======
  template <class S>
  MOZ_IMPLICIT RecordedCreateSimilarDrawTarget(S& aStream);
>>>>>>> upstream-releases
};

<<<<<<< HEAD
class RecordedCreateClippedDrawTarget
    : public RecordedEventDerived<RecordedCreateClippedDrawTarget> {
 public:
  RecordedCreateClippedDrawTarget(ReferencePtr aRefPtr, const IntSize &aMaxSize,
                                  const Matrix &aTransform,
                                  SurfaceFormat aFormat)
      : RecordedEventDerived(CREATECLIPPEDDRAWTARGET),
        mRefPtr(aRefPtr),
        mMaxSize(aMaxSize),
        mTransform(aTransform),
        mFormat(aFormat) {}
||||||| merged common ancestors
class RecordedCreateClippedDrawTarget : public RecordedEventDerived<RecordedCreateClippedDrawTarget> {
public:
  RecordedCreateClippedDrawTarget(ReferencePtr aRefPtr, const IntSize& aMaxSize, const Matrix& aTransform, SurfaceFormat aFormat)
    : RecordedEventDerived(CREATECLIPPEDDRAWTARGET)
    , mRefPtr(aRefPtr)
    , mMaxSize(aMaxSize)
    , mTransform(aTransform)
    , mFormat(aFormat)
  {
  }
=======
class RecordedCreateClippedDrawTarget
    : public RecordedEventDerived<RecordedCreateClippedDrawTarget> {
 public:
  RecordedCreateClippedDrawTarget(ReferencePtr aRefPtr, const Rect& aBounds,
                                  SurfaceFormat aFormat)
      : RecordedEventDerived(CREATECLIPPEDDRAWTARGET),
        mRefPtr(aRefPtr),
        mBounds(aBounds),
        mFormat(aFormat) {}
>>>>>>> upstream-releases

  bool PlayEvent(Translator* aTranslator) const override;

<<<<<<< HEAD
  template <class S>
  void Record(S &aStream) const;
  virtual void OutputSimpleEventInfo(
      std::stringstream &aStringStream) const override;
||||||| merged common ancestors
  template<class S>
  void Record(S &aStream) const;
  virtual void OutputSimpleEventInfo(std::stringstream &aStringStream) const override;
=======
  template <class S>
  void Record(S& aStream) const;
  virtual void OutputSimpleEventInfo(
      std::stringstream& aStringStream) const override;
>>>>>>> upstream-releases

<<<<<<< HEAD
  virtual std::string GetName() const override {
    return "CreateClippedDrawTarget";
  }
  virtual ReferencePtr GetObjectRef() const override { return mRefPtr; }
||||||| merged common ancestors
  virtual std::string GetName() const override { return "CreateClippedDrawTarget"; }
  virtual ReferencePtr GetObjectRef() const override { return mRefPtr; }
=======
  std::string GetName() const override { return "CreateClippedDrawTarget"; }

  ReferencePtr mRefPtr;
  Rect mBounds;
  SurfaceFormat mFormat;

 private:
  friend class RecordedEvent;

  template <class S>
  MOZ_IMPLICIT RecordedCreateClippedDrawTarget(S& aStream);
};

class RecordedCreateDrawTargetForFilter
    : public RecordedDrawingEvent<RecordedCreateDrawTargetForFilter> {
 public:
  RecordedCreateDrawTargetForFilter(DrawTarget* aDT, ReferencePtr aRefPtr,
                                    const IntSize& aMaxSize,
                                    SurfaceFormat aFormat, FilterNode* aFilter,
                                    FilterNode* aSource,
                                    const Rect& aSourceRect,
                                    const Point& aDestPoint)
      : RecordedDrawingEvent(CREATEDRAWTARGETFORFILTER, aDT),
        mRefPtr(aRefPtr),
        mMaxSize(aMaxSize),
        mFormat(aFormat),
        mFilter(aFilter),
        mSource(aSource),
        mSourceRect(aSourceRect),
        mDestPoint(aDestPoint) {}

  bool PlayEvent(Translator* aTranslator) const override;

  template <class S>
  void Record(S& aStream) const;
  virtual void OutputSimpleEventInfo(
      std::stringstream& aStringStream) const override;

  std::string GetName() const override {
    return "CreateSimilarDrawTargetForFilter";
  }
>>>>>>> upstream-releases

  ReferencePtr mRefPtr;
  IntSize mMaxSize;
  SurfaceFormat mFormat;
  ReferencePtr mFilter;
  ReferencePtr mSource;
  Rect mSourceRect;
  Point mDestPoint;

 private:
  friend class RecordedEvent;

<<<<<<< HEAD
  template <class S>
  MOZ_IMPLICIT RecordedCreateClippedDrawTarget(S &aStream);
||||||| merged common ancestors
  template<class S>
  MOZ_IMPLICIT RecordedCreateClippedDrawTarget(S &aStream);
=======
  template <class S>
  MOZ_IMPLICIT RecordedCreateDrawTargetForFilter(S& aStream);
>>>>>>> upstream-releases
};

class RecordedFillRect : public RecordedDrawingEvent<RecordedFillRect> {
<<<<<<< HEAD
 public:
  RecordedFillRect(DrawTarget *aDT, const Rect &aRect, const Pattern &aPattern,
                   const DrawOptions &aOptions)
      : RecordedDrawingEvent(FILLRECT, aDT),
        mRect(aRect),
        mPattern(),
        mOptions(aOptions) {
||||||| merged common ancestors
public:
  RecordedFillRect(DrawTarget *aDT, const Rect &aRect, const Pattern &aPattern, const DrawOptions &aOptions)
    : RecordedDrawingEvent(FILLRECT, aDT), mRect(aRect),
      mPattern(), mOptions(aOptions)
  {
=======
 public:
  RecordedFillRect(DrawTarget* aDT, const Rect& aRect, const Pattern& aPattern,
                   const DrawOptions& aOptions)
      : RecordedDrawingEvent(FILLRECT, aDT),
        mRect(aRect),
        mPattern(),
        mOptions(aOptions) {
>>>>>>> upstream-releases
    StorePattern(mPattern, aPattern);
  }

  bool PlayEvent(Translator* aTranslator) const override;

<<<<<<< HEAD
  template <class S>
  void Record(S &aStream) const;
  virtual void OutputSimpleEventInfo(
      std::stringstream &aStringStream) const override;
||||||| merged common ancestors
  template<class S>
  void Record(S &aStream) const;
  virtual void OutputSimpleEventInfo(std::stringstream &aStringStream) const override;
=======
  template <class S>
  void Record(S& aStream) const;
  void OutputSimpleEventInfo(std::stringstream& aStringStream) const override;
>>>>>>> upstream-releases

  std::string GetName() const override { return "FillRect"; }

 private:
  friend class RecordedEvent;

<<<<<<< HEAD
  template <class S>
  MOZ_IMPLICIT RecordedFillRect(S &aStream);
||||||| merged common ancestors
  template<class S>
  MOZ_IMPLICIT RecordedFillRect(S &aStream);
=======
  template <class S>
  MOZ_IMPLICIT RecordedFillRect(S& aStream);
>>>>>>> upstream-releases

  Rect mRect;
  PatternStorage mPattern;
  DrawOptions mOptions;
};

class RecordedStrokeRect : public RecordedDrawingEvent<RecordedStrokeRect> {
<<<<<<< HEAD
 public:
  RecordedStrokeRect(DrawTarget *aDT, const Rect &aRect,
                     const Pattern &aPattern,
                     const StrokeOptions &aStrokeOptions,
                     const DrawOptions &aOptions)
      : RecordedDrawingEvent(STROKERECT, aDT),
        mRect(aRect),
        mPattern(),
        mStrokeOptions(aStrokeOptions),
        mOptions(aOptions) {
||||||| merged common ancestors
public:
  RecordedStrokeRect(DrawTarget *aDT, const Rect &aRect, const Pattern &aPattern,
                     const StrokeOptions &aStrokeOptions, const DrawOptions &aOptions)
    : RecordedDrawingEvent(STROKERECT, aDT), mRect(aRect),
      mPattern(), mStrokeOptions(aStrokeOptions),
      mOptions(aOptions)
  {
=======
 public:
  RecordedStrokeRect(DrawTarget* aDT, const Rect& aRect,
                     const Pattern& aPattern,
                     const StrokeOptions& aStrokeOptions,
                     const DrawOptions& aOptions)
      : RecordedDrawingEvent(STROKERECT, aDT),
        mRect(aRect),
        mPattern(),
        mStrokeOptions(aStrokeOptions),
        mOptions(aOptions) {
>>>>>>> upstream-releases
    StorePattern(mPattern, aPattern);
  }

  bool PlayEvent(Translator* aTranslator) const override;

<<<<<<< HEAD
  template <class S>
  void Record(S &aStream) const;
  virtual void OutputSimpleEventInfo(
      std::stringstream &aStringStream) const override;
||||||| merged common ancestors
  template<class S>
  void Record(S &aStream) const;
  virtual void OutputSimpleEventInfo(std::stringstream &aStringStream) const override;
=======
  template <class S>
  void Record(S& aStream) const;
  void OutputSimpleEventInfo(std::stringstream& aStringStream) const override;
>>>>>>> upstream-releases

  std::string GetName() const override { return "StrokeRect"; }

 private:
  friend class RecordedEvent;

<<<<<<< HEAD
  template <class S>
  MOZ_IMPLICIT RecordedStrokeRect(S &aStream);
||||||| merged common ancestors
  template<class S>
  MOZ_IMPLICIT RecordedStrokeRect(S &aStream);
=======
  template <class S>
  MOZ_IMPLICIT RecordedStrokeRect(S& aStream);
>>>>>>> upstream-releases

  Rect mRect;
  PatternStorage mPattern;
  StrokeOptions mStrokeOptions;
  DrawOptions mOptions;
};

class RecordedStrokeLine : public RecordedDrawingEvent<RecordedStrokeLine> {
<<<<<<< HEAD
 public:
  RecordedStrokeLine(DrawTarget *aDT, const Point &aBegin, const Point &aEnd,
                     const Pattern &aPattern,
                     const StrokeOptions &aStrokeOptions,
                     const DrawOptions &aOptions)
      : RecordedDrawingEvent(STROKELINE, aDT),
        mBegin(aBegin),
        mEnd(aEnd),
        mPattern(),
        mStrokeOptions(aStrokeOptions),
        mOptions(aOptions) {
||||||| merged common ancestors
public:
  RecordedStrokeLine(DrawTarget *aDT, const Point &aBegin, const Point &aEnd,
                     const Pattern &aPattern, const StrokeOptions &aStrokeOptions,
                     const DrawOptions &aOptions)
    : RecordedDrawingEvent(STROKELINE, aDT), mBegin(aBegin), mEnd(aEnd),
      mPattern(), mStrokeOptions(aStrokeOptions), mOptions(aOptions)
  {
=======
 public:
  RecordedStrokeLine(DrawTarget* aDT, const Point& aBegin, const Point& aEnd,
                     const Pattern& aPattern,
                     const StrokeOptions& aStrokeOptions,
                     const DrawOptions& aOptions)
      : RecordedDrawingEvent(STROKELINE, aDT),
        mBegin(aBegin),
        mEnd(aEnd),
        mPattern(),
        mStrokeOptions(aStrokeOptions),
        mOptions(aOptions) {
>>>>>>> upstream-releases
    StorePattern(mPattern, aPattern);
  }

  bool PlayEvent(Translator* aTranslator) const override;

<<<<<<< HEAD
  template <class S>
  void Record(S &aStream) const;
  virtual void OutputSimpleEventInfo(
      std::stringstream &aStringStream) const override;
||||||| merged common ancestors
  template<class S>
  void Record(S &aStream) const;
  virtual void OutputSimpleEventInfo(std::stringstream &aStringStream) const override;
=======
  template <class S>
  void Record(S& aStream) const;
  void OutputSimpleEventInfo(std::stringstream& aStringStream) const override;
>>>>>>> upstream-releases

  std::string GetName() const override { return "StrokeLine"; }

 private:
  friend class RecordedEvent;

<<<<<<< HEAD
  template <class S>
  MOZ_IMPLICIT RecordedStrokeLine(S &aStream);
||||||| merged common ancestors
  template<class S>
  MOZ_IMPLICIT RecordedStrokeLine(S &aStream);
=======
  template <class S>
  MOZ_IMPLICIT RecordedStrokeLine(S& aStream);
>>>>>>> upstream-releases

  Point mBegin;
  Point mEnd;
  PatternStorage mPattern;
  StrokeOptions mStrokeOptions;
  DrawOptions mOptions;
};

class RecordedFill : public RecordedDrawingEvent<RecordedFill> {
<<<<<<< HEAD
 public:
  RecordedFill(DrawTarget *aDT, ReferencePtr aPath, const Pattern &aPattern,
               const DrawOptions &aOptions)
      : RecordedDrawingEvent(FILL, aDT),
        mPath(aPath),
        mPattern(),
        mOptions(aOptions) {
||||||| merged common ancestors
public:
  RecordedFill(DrawTarget *aDT, ReferencePtr aPath, const Pattern &aPattern, const DrawOptions &aOptions)
    : RecordedDrawingEvent(FILL, aDT), mPath(aPath),
      mPattern(), mOptions(aOptions)
  {
=======
 public:
  RecordedFill(DrawTarget* aDT, ReferencePtr aPath, const Pattern& aPattern,
               const DrawOptions& aOptions)
      : RecordedDrawingEvent(FILL, aDT),
        mPath(aPath),
        mPattern(),
        mOptions(aOptions) {
>>>>>>> upstream-releases
    StorePattern(mPattern, aPattern);
  }

  bool PlayEvent(Translator* aTranslator) const override;

<<<<<<< HEAD
  template <class S>
  void Record(S &aStream) const;
  virtual void OutputSimpleEventInfo(
      std::stringstream &aStringStream) const override;
||||||| merged common ancestors
  template<class S> void Record(S &aStream) const;
  virtual void OutputSimpleEventInfo(std::stringstream &aStringStream) const override;
=======
  template <class S>
  void Record(S& aStream) const;
  void OutputSimpleEventInfo(std::stringstream& aStringStream) const override;
>>>>>>> upstream-releases

  std::string GetName() const override { return "Fill"; }

 private:
  friend class RecordedEvent;

<<<<<<< HEAD
  template <class S>
  MOZ_IMPLICIT RecordedFill(S &aStream);
||||||| merged common ancestors
  template<class S>
  MOZ_IMPLICIT RecordedFill(S &aStream);
=======
  template <class S>
  MOZ_IMPLICIT RecordedFill(S& aStream);
>>>>>>> upstream-releases

  ReferencePtr mPath;
  PatternStorage mPattern;
  DrawOptions mOptions;
};

class RecordedFillGlyphs : public RecordedDrawingEvent<RecordedFillGlyphs> {
<<<<<<< HEAD
 public:
  RecordedFillGlyphs(DrawTarget *aDT, ReferencePtr aScaledFont,
                     const Pattern &aPattern, const DrawOptions &aOptions,
                     const Glyph *aGlyphs, uint32_t aNumGlyphs)
      : RecordedDrawingEvent(FILLGLYPHS, aDT),
        mScaledFont(aScaledFont),
        mPattern(),
        mOptions(aOptions) {
||||||| merged common ancestors
public:
  RecordedFillGlyphs(DrawTarget *aDT, ReferencePtr aScaledFont, const Pattern &aPattern, const DrawOptions &aOptions,
                     const Glyph *aGlyphs, uint32_t aNumGlyphs)
    : RecordedDrawingEvent(FILLGLYPHS, aDT), mScaledFont(aScaledFont),
      mPattern(), mOptions(aOptions)
  {
=======
 public:
  RecordedFillGlyphs(DrawTarget* aDT, ReferencePtr aScaledFont,
                     const Pattern& aPattern, const DrawOptions& aOptions,
                     const Glyph* aGlyphs, uint32_t aNumGlyphs)
      : RecordedDrawingEvent(FILLGLYPHS, aDT),
        mScaledFont(aScaledFont),
        mPattern(),
        mOptions(aOptions) {
>>>>>>> upstream-releases
    StorePattern(mPattern, aPattern);
    mNumGlyphs = aNumGlyphs;
    mGlyphs = new Glyph[aNumGlyphs];
    memcpy(mGlyphs, aGlyphs, sizeof(Glyph) * aNumGlyphs);
  }
  virtual ~RecordedFillGlyphs();

  bool PlayEvent(Translator* aTranslator) const override;

<<<<<<< HEAD
  template <class S>
  void Record(S &aStream) const;
  virtual void OutputSimpleEventInfo(
      std::stringstream &aStringStream) const override;
||||||| merged common ancestors
  template<class S> void Record(S &aStream) const;
  virtual void OutputSimpleEventInfo(std::stringstream &aStringStream) const override;
=======
  template <class S>
  void Record(S& aStream) const;
  void OutputSimpleEventInfo(std::stringstream& aStringStream) const override;
>>>>>>> upstream-releases

  std::string GetName() const override { return "FillGlyphs"; }

 private:
  friend class RecordedEvent;

<<<<<<< HEAD
  template <class S>
  MOZ_IMPLICIT RecordedFillGlyphs(S &aStream);
||||||| merged common ancestors
  template<class S>
  MOZ_IMPLICIT RecordedFillGlyphs(S &aStream);
=======
  template <class S>
  MOZ_IMPLICIT RecordedFillGlyphs(S& aStream);
>>>>>>> upstream-releases

  ReferencePtr mScaledFont;
  PatternStorage mPattern;
  DrawOptions mOptions;
  Glyph* mGlyphs;
  uint32_t mNumGlyphs;
};

class RecordedMask : public RecordedDrawingEvent<RecordedMask> {
<<<<<<< HEAD
 public:
  RecordedMask(DrawTarget *aDT, const Pattern &aSource, const Pattern &aMask,
               const DrawOptions &aOptions)
      : RecordedDrawingEvent(MASK, aDT),
        mSource(),
        mMask(),
        mOptions(aOptions) {
||||||| merged common ancestors
public:
  RecordedMask(DrawTarget *aDT, const Pattern &aSource, const Pattern &aMask, const DrawOptions &aOptions)
    : RecordedDrawingEvent(MASK, aDT), mSource(), mMask(), mOptions(aOptions)
  {
=======
 public:
  RecordedMask(DrawTarget* aDT, const Pattern& aSource, const Pattern& aMask,
               const DrawOptions& aOptions)
      : RecordedDrawingEvent(MASK, aDT),
        mSource(),
        mMask(),
        mOptions(aOptions) {
>>>>>>> upstream-releases
    StorePattern(mSource, aSource);
    StorePattern(mMask, aMask);
  }

  bool PlayEvent(Translator* aTranslator) const override;

<<<<<<< HEAD
  template <class S>
  void Record(S &aStream) const;
  virtual void OutputSimpleEventInfo(
      std::stringstream &aStringStream) const override;
||||||| merged common ancestors
  template<class S> void Record(S &aStream) const;
  virtual void OutputSimpleEventInfo(std::stringstream &aStringStream) const override;
=======
  template <class S>
  void Record(S& aStream) const;
  void OutputSimpleEventInfo(std::stringstream& aStringStream) const override;
>>>>>>> upstream-releases

  std::string GetName() const override { return "Mask"; }

 private:
  friend class RecordedEvent;

<<<<<<< HEAD
  template <class S>
  MOZ_IMPLICIT RecordedMask(S &aStream);
||||||| merged common ancestors
  template<class S>
  MOZ_IMPLICIT RecordedMask(S &aStream);
=======
  template <class S>
  MOZ_IMPLICIT RecordedMask(S& aStream);
>>>>>>> upstream-releases

  PatternStorage mSource;
  PatternStorage mMask;
  DrawOptions mOptions;
};

class RecordedStroke : public RecordedDrawingEvent<RecordedStroke> {
<<<<<<< HEAD
 public:
  RecordedStroke(DrawTarget *aDT, ReferencePtr aPath, const Pattern &aPattern,
                 const StrokeOptions &aStrokeOptions,
                 const DrawOptions &aOptions)
      : RecordedDrawingEvent(STROKE, aDT),
        mPath(aPath),
        mPattern(),
        mStrokeOptions(aStrokeOptions),
        mOptions(aOptions) {
||||||| merged common ancestors
public:
  RecordedStroke(DrawTarget *aDT, ReferencePtr aPath, const Pattern &aPattern,
                     const StrokeOptions &aStrokeOptions, const DrawOptions &aOptions)
    : RecordedDrawingEvent(STROKE, aDT), mPath(aPath), mPattern(),
      mStrokeOptions(aStrokeOptions), mOptions(aOptions)
  {
=======
 public:
  RecordedStroke(DrawTarget* aDT, ReferencePtr aPath, const Pattern& aPattern,
                 const StrokeOptions& aStrokeOptions,
                 const DrawOptions& aOptions)
      : RecordedDrawingEvent(STROKE, aDT),
        mPath(aPath),
        mPattern(),
        mStrokeOptions(aStrokeOptions),
        mOptions(aOptions) {
>>>>>>> upstream-releases
    StorePattern(mPattern, aPattern);
  }

  bool PlayEvent(Translator* aTranslator) const override;

<<<<<<< HEAD
  template <class S>
  void Record(S &aStream) const;
  virtual void OutputSimpleEventInfo(
      std::stringstream &aStringStream) const override;
||||||| merged common ancestors
  template<class S> void Record(S &aStream) const;
  virtual void OutputSimpleEventInfo(std::stringstream &aStringStream) const override;
=======
  template <class S>
  void Record(S& aStream) const;
  virtual void OutputSimpleEventInfo(
      std::stringstream& aStringStream) const override;
>>>>>>> upstream-releases

  std::string GetName() const override { return "Stroke"; }

 private:
  friend class RecordedEvent;

<<<<<<< HEAD
  template <class S>
  MOZ_IMPLICIT RecordedStroke(S &aStream);
||||||| merged common ancestors
  template<class S>
  MOZ_IMPLICIT RecordedStroke(S &aStream);
=======
  template <class S>
  MOZ_IMPLICIT RecordedStroke(S& aStream);
>>>>>>> upstream-releases

  ReferencePtr mPath;
  PatternStorage mPattern;
  StrokeOptions mStrokeOptions;
  DrawOptions mOptions;
};

class RecordedClearRect : public RecordedDrawingEvent<RecordedClearRect> {
<<<<<<< HEAD
 public:
  RecordedClearRect(DrawTarget *aDT, const Rect &aRect)
      : RecordedDrawingEvent(CLEARRECT, aDT), mRect(aRect) {}
||||||| merged common ancestors
public:
  RecordedClearRect(DrawTarget *aDT, const Rect &aRect)
    : RecordedDrawingEvent(CLEARRECT, aDT), mRect(aRect)
  {
  }
=======
 public:
  RecordedClearRect(DrawTarget* aDT, const Rect& aRect)
      : RecordedDrawingEvent(CLEARRECT, aDT), mRect(aRect) {}
>>>>>>> upstream-releases

  bool PlayEvent(Translator* aTranslator) const override;

<<<<<<< HEAD
  template <class S>
  void Record(S &aStream) const;
  virtual void OutputSimpleEventInfo(
      std::stringstream &aStringStream) const override;
||||||| merged common ancestors
  template<class S> void Record(S &aStream) const;
  virtual void OutputSimpleEventInfo(std::stringstream &aStringStream) const override;
=======
  template <class S>
  void Record(S& aStream) const;
  void OutputSimpleEventInfo(std::stringstream& aStringStream) const override;
>>>>>>> upstream-releases

  std::string GetName() const override { return "ClearRect"; }

 private:
  friend class RecordedEvent;

<<<<<<< HEAD
  template <class S>
  MOZ_IMPLICIT RecordedClearRect(S &aStream);
||||||| merged common ancestors
  template<class S>
  MOZ_IMPLICIT RecordedClearRect(S &aStream);
=======
  template <class S>
  MOZ_IMPLICIT RecordedClearRect(S& aStream);
>>>>>>> upstream-releases

  Rect mRect;
};

class RecordedCopySurface : public RecordedDrawingEvent<RecordedCopySurface> {
<<<<<<< HEAD
 public:
  RecordedCopySurface(DrawTarget *aDT, ReferencePtr aSourceSurface,
                      const IntRect &aSourceRect, const IntPoint &aDest)
      : RecordedDrawingEvent(COPYSURFACE, aDT),
        mSourceSurface(aSourceSurface),
        mSourceRect(aSourceRect),
        mDest(aDest) {}
||||||| merged common ancestors
public:
  RecordedCopySurface(DrawTarget *aDT, ReferencePtr aSourceSurface,
                      const IntRect &aSourceRect, const IntPoint &aDest)
    : RecordedDrawingEvent(COPYSURFACE, aDT), mSourceSurface(aSourceSurface),
	  mSourceRect(aSourceRect), mDest(aDest)
  {
  }
=======
 public:
  RecordedCopySurface(DrawTarget* aDT, ReferencePtr aSourceSurface,
                      const IntRect& aSourceRect, const IntPoint& aDest)
      : RecordedDrawingEvent(COPYSURFACE, aDT),
        mSourceSurface(aSourceSurface),
        mSourceRect(aSourceRect),
        mDest(aDest) {}
>>>>>>> upstream-releases

  bool PlayEvent(Translator* aTranslator) const override;

<<<<<<< HEAD
  template <class S>
  void Record(S &aStream) const;
  virtual void OutputSimpleEventInfo(
      std::stringstream &aStringStream) const override;
||||||| merged common ancestors
  template<class S> void Record(S &aStream) const;
  virtual void OutputSimpleEventInfo(std::stringstream &aStringStream) const override;
=======
  template <class S>
  void Record(S& aStream) const;
  void OutputSimpleEventInfo(std::stringstream& aStringStream) const override;
>>>>>>> upstream-releases

  std::string GetName() const override { return "CopySurface"; }

 private:
  friend class RecordedEvent;

<<<<<<< HEAD
  template <class S>
  MOZ_IMPLICIT RecordedCopySurface(S &aStream);
||||||| merged common ancestors
  template<class S>
  MOZ_IMPLICIT RecordedCopySurface(S &aStream);
=======
  template <class S>
  MOZ_IMPLICIT RecordedCopySurface(S& aStream);
>>>>>>> upstream-releases

  ReferencePtr mSourceSurface;
  IntRect mSourceRect;
  IntPoint mDest;
};

class RecordedPushClip : public RecordedDrawingEvent<RecordedPushClip> {
<<<<<<< HEAD
 public:
  RecordedPushClip(DrawTarget *aDT, ReferencePtr aPath)
      : RecordedDrawingEvent(PUSHCLIP, aDT), mPath(aPath) {}
||||||| merged common ancestors
public:
  RecordedPushClip(DrawTarget *aDT, ReferencePtr aPath)
    : RecordedDrawingEvent(PUSHCLIP, aDT), mPath(aPath)
  {
  }
=======
 public:
  RecordedPushClip(DrawTarget* aDT, ReferencePtr aPath)
      : RecordedDrawingEvent(PUSHCLIP, aDT), mPath(aPath) {}
>>>>>>> upstream-releases

  bool PlayEvent(Translator* aTranslator) const override;

<<<<<<< HEAD
  template <class S>
  void Record(S &aStream) const;
  virtual void OutputSimpleEventInfo(
      std::stringstream &aStringStream) const override;
||||||| merged common ancestors
  template<class S> void Record(S &aStream) const;
  virtual void OutputSimpleEventInfo(std::stringstream &aStringStream) const override;
=======
  template <class S>
  void Record(S& aStream) const;
  void OutputSimpleEventInfo(std::stringstream& aStringStream) const override;
>>>>>>> upstream-releases

  std::string GetName() const override { return "PushClip"; }

 private:
  friend class RecordedEvent;

<<<<<<< HEAD
  template <class S>
  MOZ_IMPLICIT RecordedPushClip(S &aStream);
||||||| merged common ancestors
  template<class S>
  MOZ_IMPLICIT RecordedPushClip(S &aStream);
=======
  template <class S>
  MOZ_IMPLICIT RecordedPushClip(S& aStream);
>>>>>>> upstream-releases

  ReferencePtr mPath;
};

class RecordedPushClipRect : public RecordedDrawingEvent<RecordedPushClipRect> {
<<<<<<< HEAD
 public:
  RecordedPushClipRect(DrawTarget *aDT, const Rect &aRect)
      : RecordedDrawingEvent(PUSHCLIPRECT, aDT), mRect(aRect) {}
||||||| merged common ancestors
public:
  RecordedPushClipRect(DrawTarget *aDT, const Rect &aRect)
    : RecordedDrawingEvent(PUSHCLIPRECT, aDT), mRect(aRect)
  {
  }
=======
 public:
  RecordedPushClipRect(DrawTarget* aDT, const Rect& aRect)
      : RecordedDrawingEvent(PUSHCLIPRECT, aDT), mRect(aRect) {}
>>>>>>> upstream-releases

  bool PlayEvent(Translator* aTranslator) const override;

<<<<<<< HEAD
  template <class S>
  void Record(S &aStream) const;
  virtual void OutputSimpleEventInfo(
      std::stringstream &aStringStream) const override;
||||||| merged common ancestors
  template<class S> void Record(S &aStream) const;
  virtual void OutputSimpleEventInfo(std::stringstream &aStringStream) const override;
=======
  template <class S>
  void Record(S& aStream) const;
  void OutputSimpleEventInfo(std::stringstream& aStringStream) const override;
>>>>>>> upstream-releases

  std::string GetName() const override { return "PushClipRect"; }

 private:
  friend class RecordedEvent;

<<<<<<< HEAD
  template <class S>
  MOZ_IMPLICIT RecordedPushClipRect(S &aStream);
||||||| merged common ancestors
  template<class S>
  MOZ_IMPLICIT RecordedPushClipRect(S &aStream);
=======
  template <class S>
  MOZ_IMPLICIT RecordedPushClipRect(S& aStream);
>>>>>>> upstream-releases

  Rect mRect;
};

class RecordedPopClip : public RecordedDrawingEvent<RecordedPopClip> {
<<<<<<< HEAD
 public:
  MOZ_IMPLICIT RecordedPopClip(DrawTarget *aDT)
      : RecordedDrawingEvent(POPCLIP, aDT) {}
||||||| merged common ancestors
public:
  MOZ_IMPLICIT RecordedPopClip(DrawTarget *aDT)
    : RecordedDrawingEvent(POPCLIP, aDT)
  {}
=======
 public:
  MOZ_IMPLICIT RecordedPopClip(DrawTarget* aDT)
      : RecordedDrawingEvent(POPCLIP, aDT) {}
>>>>>>> upstream-releases

  bool PlayEvent(Translator* aTranslator) const override;

<<<<<<< HEAD
  template <class S>
  void Record(S &aStream) const;
  virtual void OutputSimpleEventInfo(
      std::stringstream &aStringStream) const override;
||||||| merged common ancestors
  template<class S> void Record(S &aStream) const;
  virtual void OutputSimpleEventInfo(std::stringstream &aStringStream) const override;
=======
  template <class S>
  void Record(S& aStream) const;
  void OutputSimpleEventInfo(std::stringstream& aStringStream) const override;
>>>>>>> upstream-releases

  std::string GetName() const override { return "PopClip"; }

 private:
  friend class RecordedEvent;

<<<<<<< HEAD
  template <class S>
  MOZ_IMPLICIT RecordedPopClip(S &aStream);
||||||| merged common ancestors
  template<class S>
  MOZ_IMPLICIT RecordedPopClip(S &aStream);
=======
  template <class S>
  MOZ_IMPLICIT RecordedPopClip(S& aStream);
>>>>>>> upstream-releases
};

class RecordedPushLayer : public RecordedDrawingEvent<RecordedPushLayer> {
<<<<<<< HEAD
 public:
  RecordedPushLayer(DrawTarget *aDT, bool aOpaque, Float aOpacity,
                    SourceSurface *aMask, const Matrix &aMaskTransform,
                    const IntRect &aBounds, bool aCopyBackground)
      : RecordedDrawingEvent(PUSHLAYER, aDT),
        mOpaque(aOpaque),
        mOpacity(aOpacity),
        mMask(aMask),
        mMaskTransform(aMaskTransform),
        mBounds(aBounds),
        mCopyBackground(aCopyBackground) {}
||||||| merged common ancestors
public:
  RecordedPushLayer(DrawTarget* aDT, bool aOpaque, Float aOpacity,
                    SourceSurface* aMask, const Matrix& aMaskTransform,
                    const IntRect& aBounds, bool aCopyBackground)
    : RecordedDrawingEvent(PUSHLAYER, aDT), mOpaque(aOpaque)
    , mOpacity(aOpacity), mMask(aMask), mMaskTransform(aMaskTransform)
    , mBounds(aBounds), mCopyBackground(aCopyBackground)
  {
  }
=======
 public:
  RecordedPushLayer(DrawTarget* aDT, bool aOpaque, Float aOpacity,
                    SourceSurface* aMask, const Matrix& aMaskTransform,
                    const IntRect& aBounds, bool aCopyBackground)
      : RecordedDrawingEvent(PUSHLAYER, aDT),
        mOpaque(aOpaque),
        mOpacity(aOpacity),
        mMask(aMask),
        mMaskTransform(aMaskTransform),
        mBounds(aBounds),
        mCopyBackground(aCopyBackground) {}
>>>>>>> upstream-releases

  bool PlayEvent(Translator* aTranslator) const override;

<<<<<<< HEAD
  template <class S>
  void Record(S &aStream) const;
  virtual void OutputSimpleEventInfo(
      std::stringstream &aStringStream) const override;
||||||| merged common ancestors
  template<class S> void Record(S &aStream) const;
  virtual void OutputSimpleEventInfo(std::stringstream &aStringStream) const override;
=======
  template <class S>
  void Record(S& aStream) const;
  void OutputSimpleEventInfo(std::stringstream& aStringStream) const override;
>>>>>>> upstream-releases

  std::string GetName() const override { return "PushLayer"; }

 private:
  friend class RecordedEvent;

<<<<<<< HEAD
  template <class S>
  MOZ_IMPLICIT RecordedPushLayer(S &aStream);
||||||| merged common ancestors
  template<class S>
  MOZ_IMPLICIT RecordedPushLayer(S &aStream);
=======
  template <class S>
  MOZ_IMPLICIT RecordedPushLayer(S& aStream);
>>>>>>> upstream-releases

  bool mOpaque;
  Float mOpacity;
  ReferencePtr mMask;
  Matrix mMaskTransform;
  IntRect mBounds;
  bool mCopyBackground;
};

<<<<<<< HEAD
class RecordedPushLayerWithBlend
    : public RecordedDrawingEvent<RecordedPushLayerWithBlend> {
 public:
  RecordedPushLayerWithBlend(DrawTarget *aDT, bool aOpaque, Float aOpacity,
                             SourceSurface *aMask, const Matrix &aMaskTransform,
                             const IntRect &aBounds, bool aCopyBackground,
                             CompositionOp aCompositionOp)
      : RecordedDrawingEvent(PUSHLAYERWITHBLEND, aDT),
        mOpaque(aOpaque),
        mOpacity(aOpacity),
        mMask(aMask),
        mMaskTransform(aMaskTransform),
        mBounds(aBounds),
        mCopyBackground(aCopyBackground),
        mCompositionOp(aCompositionOp) {}

  virtual bool PlayEvent(Translator *aTranslator) const override;

  template <class S>
  void Record(S &aStream) const;
  virtual void OutputSimpleEventInfo(
      std::stringstream &aStringStream) const override;

  virtual std::string GetName() const override { return "PushLayerWithBlend"; }

 private:
||||||| merged common ancestors
class RecordedPushLayerWithBlend : public RecordedDrawingEvent<RecordedPushLayerWithBlend> {
public:
  RecordedPushLayerWithBlend(DrawTarget* aDT, bool aOpaque, Float aOpacity,
                    SourceSurface* aMask, const Matrix& aMaskTransform,
                    const IntRect& aBounds, bool aCopyBackground,
                    CompositionOp aCompositionOp)
    : RecordedDrawingEvent(PUSHLAYERWITHBLEND, aDT), mOpaque(aOpaque)
    , mOpacity(aOpacity), mMask(aMask), mMaskTransform(aMaskTransform)
    , mBounds(aBounds), mCopyBackground(aCopyBackground)
    , mCompositionOp(aCompositionOp)
  {
  }

  virtual bool PlayEvent(Translator *aTranslator) const override;

  template<class S> void Record(S &aStream) const;
  virtual void OutputSimpleEventInfo(std::stringstream &aStringStream) const override;

  virtual std::string GetName() const override { return "PushLayerWithBlend"; }

private:
=======
class RecordedPushLayerWithBlend
    : public RecordedDrawingEvent<RecordedPushLayerWithBlend> {
 public:
  RecordedPushLayerWithBlend(DrawTarget* aDT, bool aOpaque, Float aOpacity,
                             SourceSurface* aMask, const Matrix& aMaskTransform,
                             const IntRect& aBounds, bool aCopyBackground,
                             CompositionOp aCompositionOp)
      : RecordedDrawingEvent(PUSHLAYERWITHBLEND, aDT),
        mOpaque(aOpaque),
        mOpacity(aOpacity),
        mMask(aMask),
        mMaskTransform(aMaskTransform),
        mBounds(aBounds),
        mCopyBackground(aCopyBackground),
        mCompositionOp(aCompositionOp) {}

  bool PlayEvent(Translator* aTranslator) const override;

  template <class S>
  void Record(S& aStream) const;
  virtual void OutputSimpleEventInfo(
      std::stringstream& aStringStream) const override;

  std::string GetName() const override { return "PushLayerWithBlend"; }

 private:
>>>>>>> upstream-releases
  friend class RecordedEvent;

<<<<<<< HEAD
  template <class S>
  MOZ_IMPLICIT RecordedPushLayerWithBlend(S &aStream);
||||||| merged common ancestors
  template<class S>
  MOZ_IMPLICIT RecordedPushLayerWithBlend(S &aStream);
=======
  template <class S>
  MOZ_IMPLICIT RecordedPushLayerWithBlend(S& aStream);
>>>>>>> upstream-releases

  bool mOpaque;
  Float mOpacity;
  ReferencePtr mMask;
  Matrix mMaskTransform;
  IntRect mBounds;
  bool mCopyBackground;
  CompositionOp mCompositionOp;
};

class RecordedPopLayer : public RecordedDrawingEvent<RecordedPopLayer> {
<<<<<<< HEAD
 public:
  MOZ_IMPLICIT RecordedPopLayer(DrawTarget *aDT)
      : RecordedDrawingEvent(POPLAYER, aDT) {}
||||||| merged common ancestors
public:
  MOZ_IMPLICIT RecordedPopLayer(DrawTarget* aDT)
    : RecordedDrawingEvent(POPLAYER, aDT)
  {
  }
=======
 public:
  MOZ_IMPLICIT RecordedPopLayer(DrawTarget* aDT)
      : RecordedDrawingEvent(POPLAYER, aDT) {}
>>>>>>> upstream-releases

  bool PlayEvent(Translator* aTranslator) const override;

<<<<<<< HEAD
  template <class S>
  void Record(S &aStream) const;
  virtual void OutputSimpleEventInfo(
      std::stringstream &aStringStream) const override;
||||||| merged common ancestors
  template<class S> void Record(S &aStream) const;
  virtual void OutputSimpleEventInfo(std::stringstream &aStringStream) const override;
=======
  template <class S>
  void Record(S& aStream) const;
  void OutputSimpleEventInfo(std::stringstream& aStringStream) const override;
>>>>>>> upstream-releases

  std::string GetName() const override { return "PopLayer"; }

 private:
  friend class RecordedEvent;

<<<<<<< HEAD
  template <class S>
  MOZ_IMPLICIT RecordedPopLayer(S &aStream);
||||||| merged common ancestors
  template<class S>
  MOZ_IMPLICIT RecordedPopLayer(S &aStream);
=======
  template <class S>
  MOZ_IMPLICIT RecordedPopLayer(S& aStream);
>>>>>>> upstream-releases
};

class RecordedSetTransform : public RecordedDrawingEvent<RecordedSetTransform> {
<<<<<<< HEAD
 public:
  RecordedSetTransform(DrawTarget *aDT, const Matrix &aTransform)
      : RecordedDrawingEvent(SETTRANSFORM, aDT), mTransform(aTransform) {}
||||||| merged common ancestors
public:
  RecordedSetTransform(DrawTarget *aDT, const Matrix &aTransform)
    : RecordedDrawingEvent(SETTRANSFORM, aDT), mTransform(aTransform)
  {
  }
=======
 public:
  RecordedSetTransform(DrawTarget* aDT, const Matrix& aTransform)
      : RecordedDrawingEvent(SETTRANSFORM, aDT), mTransform(aTransform) {}
>>>>>>> upstream-releases

  bool PlayEvent(Translator* aTranslator) const override;

<<<<<<< HEAD
  template <class S>
  void Record(S &aStream) const;
  virtual void OutputSimpleEventInfo(
      std::stringstream &aStringStream) const override;
||||||| merged common ancestors
  template<class S> void Record(S &aStream) const;
  virtual void OutputSimpleEventInfo(std::stringstream &aStringStream) const override;
=======
  template <class S>
  void Record(S& aStream) const;
  void OutputSimpleEventInfo(std::stringstream& aStringStream) const override;
>>>>>>> upstream-releases

  std::string GetName() const override { return "SetTransform"; }

  Matrix mTransform;

 private:
  friend class RecordedEvent;

<<<<<<< HEAD
  template <class S>
  MOZ_IMPLICIT RecordedSetTransform(S &aStream);
||||||| merged common ancestors
   template<class S>
  MOZ_IMPLICIT RecordedSetTransform(S &aStream);
=======
  template <class S>
  MOZ_IMPLICIT RecordedSetTransform(S& aStream);
>>>>>>> upstream-releases
};

class RecordedDrawSurface : public RecordedDrawingEvent<RecordedDrawSurface> {
<<<<<<< HEAD
 public:
  RecordedDrawSurface(DrawTarget *aDT, ReferencePtr aRefSource,
                      const Rect &aDest, const Rect &aSource,
                      const DrawSurfaceOptions &aDSOptions,
                      const DrawOptions &aOptions)
      : RecordedDrawingEvent(DRAWSURFACE, aDT),
        mRefSource(aRefSource),
        mDest(aDest),
        mSource(aSource),
        mDSOptions(aDSOptions),
        mOptions(aOptions) {}

  virtual bool PlayEvent(Translator *aTranslator) const override;

  template <class S>
  void Record(S &aStream) const;
  virtual void OutputSimpleEventInfo(
      std::stringstream &aStringStream) const override;

  virtual std::string GetName() const override { return "DrawSurface"; }

 private:
||||||| merged common ancestors
public:
  RecordedDrawSurface(DrawTarget *aDT, ReferencePtr aRefSource, const Rect &aDest,
                      const Rect &aSource, const DrawSurfaceOptions &aDSOptions,
                      const DrawOptions &aOptions)
    : RecordedDrawingEvent(DRAWSURFACE, aDT), mRefSource(aRefSource), mDest(aDest)
    , mSource(aSource), mDSOptions(aDSOptions), mOptions(aOptions)
  {
  }

  virtual bool PlayEvent(Translator *aTranslator) const override;

  template<class S> void Record(S &aStream) const;
  virtual void OutputSimpleEventInfo(std::stringstream &aStringStream) const override;

  virtual std::string GetName() const override { return "DrawSurface"; }

private:
=======
 public:
  RecordedDrawSurface(DrawTarget* aDT, ReferencePtr aRefSource,
                      const Rect& aDest, const Rect& aSource,
                      const DrawSurfaceOptions& aDSOptions,
                      const DrawOptions& aOptions)
      : RecordedDrawingEvent(DRAWSURFACE, aDT),
        mRefSource(aRefSource),
        mDest(aDest),
        mSource(aSource),
        mDSOptions(aDSOptions),
        mOptions(aOptions) {}

  bool PlayEvent(Translator* aTranslator) const override;

  template <class S>
  void Record(S& aStream) const;
  void OutputSimpleEventInfo(std::stringstream& aStringStream) const override;

  std::string GetName() const override { return "DrawSurface"; }

 private:
>>>>>>> upstream-releases
  friend class RecordedEvent;

<<<<<<< HEAD
  template <class S>
  MOZ_IMPLICIT RecordedDrawSurface(S &aStream);
||||||| merged common ancestors
   template<class S>
  MOZ_IMPLICIT RecordedDrawSurface(S &aStream);
=======
  template <class S>
  MOZ_IMPLICIT RecordedDrawSurface(S& aStream);
>>>>>>> upstream-releases

  ReferencePtr mRefSource;
  Rect mDest;
  Rect mSource;
  DrawSurfaceOptions mDSOptions;
  DrawOptions mOptions;
};

<<<<<<< HEAD
class RecordedDrawDependentSurface
    : public RecordedDrawingEvent<RecordedDrawDependentSurface> {
 public:
  RecordedDrawDependentSurface(DrawTarget *aDT, uint64_t aId, const Rect &aDest,
                               const DrawSurfaceOptions &aDSOptions,
                               const DrawOptions &aOptions)
      : RecordedDrawingEvent(DRAWDEPENDENTSURFACE, aDT),
        mId(aId),
        mDest(aDest),
        mDSOptions(aDSOptions),
        mOptions(aOptions) {}
||||||| merged common ancestors
class RecordedDrawDependentSurface : public RecordedDrawingEvent<RecordedDrawDependentSurface> {
public:
  RecordedDrawDependentSurface(DrawTarget *aDT, uint64_t aId, const Rect &aDest,
                      const DrawSurfaceOptions &aDSOptions,
                      const DrawOptions &aOptions)
    : RecordedDrawingEvent(DRAWDEPENDENTSURFACE, aDT), mId(aId), mDest(aDest)
    , mDSOptions(aDSOptions), mOptions(aOptions)
  {
  }
=======
class RecordedDrawDependentSurface
    : public RecordedDrawingEvent<RecordedDrawDependentSurface> {
 public:
  RecordedDrawDependentSurface(DrawTarget* aDT, uint64_t aId, const Rect& aDest,
                               const DrawSurfaceOptions& aDSOptions,
                               const DrawOptions& aOptions)
      : RecordedDrawingEvent(DRAWDEPENDENTSURFACE, aDT),
        mId(aId),
        mDest(aDest),
        mDSOptions(aDSOptions),
        mOptions(aOptions) {}
>>>>>>> upstream-releases

  bool PlayEvent(Translator* aTranslator) const override;

<<<<<<< HEAD
  template <class S>
  void Record(S &aStream) const;
  virtual void OutputSimpleEventInfo(
      std::stringstream &aStringStream) const override;
||||||| merged common ancestors
  template<class S> void Record(S &aStream) const;
  virtual void OutputSimpleEventInfo(std::stringstream &aStringStream) const override;
=======
  template <class S>
  void Record(S& aStream) const;
  void OutputSimpleEventInfo(std::stringstream& aStringStream) const override;
>>>>>>> upstream-releases

<<<<<<< HEAD
  virtual std::string GetName() const override {
    return "DrawDependentSurface";
  }
||||||| merged common ancestors
  virtual std::string GetName() const override { return "DrawDependentSurface"; }
=======
  std::string GetName() const override { return "DrawDependentSurface"; }
>>>>>>> upstream-releases

 private:
  friend class RecordedEvent;

<<<<<<< HEAD
  template <class S>
  MOZ_IMPLICIT RecordedDrawDependentSurface(S &aStream);
||||||| merged common ancestors
  template<class S>
  MOZ_IMPLICIT RecordedDrawDependentSurface(S &aStream);
=======
  template <class S>
  MOZ_IMPLICIT RecordedDrawDependentSurface(S& aStream);
>>>>>>> upstream-releases

  uint64_t mId;
  Rect mDest;
  DrawSurfaceOptions mDSOptions;
  DrawOptions mOptions;
};

<<<<<<< HEAD
class RecordedDrawSurfaceWithShadow
    : public RecordedDrawingEvent<RecordedDrawSurfaceWithShadow> {
 public:
  RecordedDrawSurfaceWithShadow(DrawTarget *aDT, ReferencePtr aRefSource,
                                const Point &aDest, const Color &aColor,
                                const Point &aOffset, Float aSigma,
                                CompositionOp aOp)
      : RecordedDrawingEvent(DRAWSURFACEWITHSHADOW, aDT),
        mRefSource(aRefSource),
        mDest(aDest),
        mColor(aColor),
        mOffset(aOffset),
        mSigma(aSigma),
        mOp(aOp) {}

  virtual bool PlayEvent(Translator *aTranslator) const override;

  template <class S>
  void Record(S &aStream) const;
  virtual void OutputSimpleEventInfo(
      std::stringstream &aStringStream) const override;

  virtual std::string GetName() const override {
    return "DrawSurfaceWithShadow";
  }

 private:
||||||| merged common ancestors
class RecordedDrawSurfaceWithShadow : public RecordedDrawingEvent<RecordedDrawSurfaceWithShadow> {
public:
  RecordedDrawSurfaceWithShadow(DrawTarget *aDT, ReferencePtr aRefSource, const Point &aDest,
                                const Color &aColor, const Point &aOffset,
                                Float aSigma, CompositionOp aOp)
    : RecordedDrawingEvent(DRAWSURFACEWITHSHADOW, aDT), mRefSource(aRefSource), mDest(aDest)
    , mColor(aColor), mOffset(aOffset), mSigma(aSigma), mOp(aOp)
  {
  }

  virtual bool PlayEvent(Translator *aTranslator) const override;

  template<class S> void Record(S &aStream) const;
  virtual void OutputSimpleEventInfo(std::stringstream &aStringStream) const override;

  virtual std::string GetName() const override { return "DrawSurfaceWithShadow"; }

private:
=======
class RecordedDrawSurfaceWithShadow
    : public RecordedDrawingEvent<RecordedDrawSurfaceWithShadow> {
 public:
  RecordedDrawSurfaceWithShadow(DrawTarget* aDT, ReferencePtr aRefSource,
                                const Point& aDest, const Color& aColor,
                                const Point& aOffset, Float aSigma,
                                CompositionOp aOp)
      : RecordedDrawingEvent(DRAWSURFACEWITHSHADOW, aDT),
        mRefSource(aRefSource),
        mDest(aDest),
        mColor(aColor),
        mOffset(aOffset),
        mSigma(aSigma),
        mOp(aOp) {}

  bool PlayEvent(Translator* aTranslator) const override;

  template <class S>
  void Record(S& aStream) const;
  void OutputSimpleEventInfo(std::stringstream& aStringStream) const override;

  std::string GetName() const override { return "DrawSurfaceWithShadow"; }

 private:
>>>>>>> upstream-releases
  friend class RecordedEvent;

<<<<<<< HEAD
  template <class S>
  MOZ_IMPLICIT RecordedDrawSurfaceWithShadow(S &aStream);
||||||| merged common ancestors
  template<class S>
  MOZ_IMPLICIT RecordedDrawSurfaceWithShadow(S &aStream);
=======
  template <class S>
  MOZ_IMPLICIT RecordedDrawSurfaceWithShadow(S& aStream);
>>>>>>> upstream-releases

  ReferencePtr mRefSource;
  Point mDest;
  Color mColor;
  Point mOffset;
  Float mSigma;
  CompositionOp mOp;
};

class RecordedDrawFilter : public RecordedDrawingEvent<RecordedDrawFilter> {
<<<<<<< HEAD
 public:
  RecordedDrawFilter(DrawTarget *aDT, ReferencePtr aNode,
                     const Rect &aSourceRect, const Point &aDestPoint,
                     const DrawOptions &aOptions)
      : RecordedDrawingEvent(DRAWFILTER, aDT),
        mNode(aNode),
        mSourceRect(aSourceRect),
        mDestPoint(aDestPoint),
        mOptions(aOptions) {}
||||||| merged common ancestors
public:
  RecordedDrawFilter(DrawTarget *aDT, ReferencePtr aNode,
                     const Rect &aSourceRect,
                     const Point &aDestPoint,
                     const DrawOptions &aOptions)
    : RecordedDrawingEvent(DRAWFILTER, aDT), mNode(aNode), mSourceRect(aSourceRect)
    , mDestPoint(aDestPoint), mOptions(aOptions)
  {
  }
=======
 public:
  RecordedDrawFilter(DrawTarget* aDT, ReferencePtr aNode,
                     const Rect& aSourceRect, const Point& aDestPoint,
                     const DrawOptions& aOptions)
      : RecordedDrawingEvent(DRAWFILTER, aDT),
        mNode(aNode),
        mSourceRect(aSourceRect),
        mDestPoint(aDestPoint),
        mOptions(aOptions) {}
>>>>>>> upstream-releases

  bool PlayEvent(Translator* aTranslator) const override;

<<<<<<< HEAD
  template <class S>
  void Record(S &aStream) const;
  virtual void OutputSimpleEventInfo(
      std::stringstream &aStringStream) const override;
||||||| merged common ancestors
  template<class S> void Record(S &aStream) const;
  virtual void OutputSimpleEventInfo(std::stringstream &aStringStream) const override;
=======
  template <class S>
  void Record(S& aStream) const;
  void OutputSimpleEventInfo(std::stringstream& aStringStream) const override;
>>>>>>> upstream-releases

  std::string GetName() const override { return "DrawFilter"; }

 private:
  friend class RecordedEvent;

<<<<<<< HEAD
  template <class S>
  MOZ_IMPLICIT RecordedDrawFilter(S &aStream);
||||||| merged common ancestors
  template<class S>
  MOZ_IMPLICIT RecordedDrawFilter(S &aStream);
=======
  template <class S>
  MOZ_IMPLICIT RecordedDrawFilter(S& aStream);
>>>>>>> upstream-releases

  ReferencePtr mNode;
  Rect mSourceRect;
  Point mDestPoint;
  DrawOptions mOptions;
};

class RecordedPathCreation : public RecordedEventDerived<RecordedPathCreation> {
<<<<<<< HEAD
 public:
  MOZ_IMPLICIT RecordedPathCreation(PathRecording *aPath);
  ~RecordedPathCreation();
||||||| merged common ancestors
public:
  MOZ_IMPLICIT RecordedPathCreation(PathRecording *aPath);
  ~RecordedPathCreation();
=======
 public:
  MOZ_IMPLICIT RecordedPathCreation(PathRecording* aPath);
>>>>>>> upstream-releases

  bool PlayEvent(Translator* aTranslator) const override;

<<<<<<< HEAD
  template <class S>
  void Record(S &aStream) const;
  virtual void OutputSimpleEventInfo(
      std::stringstream &aStringStream) const override;
||||||| merged common ancestors
  template<class S> void Record(S &aStream) const;
  virtual void OutputSimpleEventInfo(std::stringstream &aStringStream) const override;
=======
  template <class S>
  void Record(S& aStream) const;
  void OutputSimpleEventInfo(std::stringstream& aStringStream) const override;
>>>>>>> upstream-releases

  std::string GetName() const override { return "Path Creation"; }

 private:
  friend class RecordedEvent;

  ReferencePtr mRefPtr;
  FillRule mFillRule;
  RefPtr<PathRecording> mPath;
  UniquePtr<PathOps> mPathOps;

<<<<<<< HEAD
  template <class S>
  MOZ_IMPLICIT RecordedPathCreation(S &aStream);
||||||| merged common ancestors
  template<class S>
  MOZ_IMPLICIT RecordedPathCreation(S &aStream);
=======
  template <class S>
  MOZ_IMPLICIT RecordedPathCreation(S& aStream);
>>>>>>> upstream-releases
};

<<<<<<< HEAD
class RecordedPathDestruction
    : public RecordedEventDerived<RecordedPathDestruction> {
 public:
  MOZ_IMPLICIT RecordedPathDestruction(PathRecording *aPath)
      : RecordedEventDerived(PATHDESTRUCTION), mRefPtr(aPath) {}
||||||| merged common ancestors
class RecordedPathDestruction : public RecordedEventDerived<RecordedPathDestruction> {
public:
  MOZ_IMPLICIT RecordedPathDestruction(PathRecording *aPath)
    : RecordedEventDerived(PATHDESTRUCTION), mRefPtr(aPath)
  {
  }
=======
class RecordedPathDestruction
    : public RecordedEventDerived<RecordedPathDestruction> {
 public:
  MOZ_IMPLICIT RecordedPathDestruction(PathRecording* aPath)
      : RecordedEventDerived(PATHDESTRUCTION), mRefPtr(aPath) {}
>>>>>>> upstream-releases

  bool PlayEvent(Translator* aTranslator) const override;

<<<<<<< HEAD
  template <class S>
  void Record(S &aStream) const;
  virtual void OutputSimpleEventInfo(
      std::stringstream &aStringStream) const override;
||||||| merged common ancestors
  template<class S> void Record(S &aStream) const;
  virtual void OutputSimpleEventInfo(std::stringstream &aStringStream) const override;
=======
  template <class S>
  void Record(S& aStream) const;
  void OutputSimpleEventInfo(std::stringstream& aStringStream) const override;
>>>>>>> upstream-releases

  std::string GetName() const override { return "Path Destruction"; }

 private:
  friend class RecordedEvent;

  ReferencePtr mRefPtr;

<<<<<<< HEAD
  template <class S>
  MOZ_IMPLICIT RecordedPathDestruction(S &aStream);
||||||| merged common ancestors
  template<class S>
  MOZ_IMPLICIT RecordedPathDestruction(S &aStream);
=======
  template <class S>
  MOZ_IMPLICIT RecordedPathDestruction(S& aStream);
>>>>>>> upstream-releases
};

<<<<<<< HEAD
class RecordedSourceSurfaceCreation
    : public RecordedEventDerived<RecordedSourceSurfaceCreation> {
 public:
  RecordedSourceSurfaceCreation(ReferencePtr aRefPtr, uint8_t *aData,
                                int32_t aStride, const IntSize &aSize,
                                SurfaceFormat aFormat)
      : RecordedEventDerived(SOURCESURFACECREATION),
        mRefPtr(aRefPtr),
        mData(aData),
        mStride(aStride),
        mSize(aSize),
        mFormat(aFormat),
        mDataOwned(false) {}
||||||| merged common ancestors
class RecordedSourceSurfaceCreation : public RecordedEventDerived<RecordedSourceSurfaceCreation> {
public:
  RecordedSourceSurfaceCreation(ReferencePtr aRefPtr, uint8_t *aData, int32_t aStride,
                                const IntSize &aSize, SurfaceFormat aFormat)
    : RecordedEventDerived(SOURCESURFACECREATION), mRefPtr(aRefPtr), mData(aData)
    , mStride(aStride), mSize(aSize), mFormat(aFormat), mDataOwned(false)
  {
  }
=======
class RecordedSourceSurfaceCreation
    : public RecordedEventDerived<RecordedSourceSurfaceCreation> {
 public:
  RecordedSourceSurfaceCreation(ReferencePtr aRefPtr, uint8_t* aData,
                                int32_t aStride, const IntSize& aSize,
                                SurfaceFormat aFormat)
      : RecordedEventDerived(SOURCESURFACECREATION),
        mRefPtr(aRefPtr),
        mData(aData),
        mStride(aStride),
        mSize(aSize),
        mFormat(aFormat),
        mDataOwned(false) {}
>>>>>>> upstream-releases

  ~RecordedSourceSurfaceCreation();

  bool PlayEvent(Translator* aTranslator) const override;

<<<<<<< HEAD
  template <class S>
  void Record(S &aStream) const;
  virtual void OutputSimpleEventInfo(
      std::stringstream &aStringStream) const override;
||||||| merged common ancestors
  template<class S> void Record(S &aStream) const;
  virtual void OutputSimpleEventInfo(std::stringstream &aStringStream) const override;
=======
  template <class S>
  void Record(S& aStream) const;
  void OutputSimpleEventInfo(std::stringstream& aStringStream) const override;
>>>>>>> upstream-releases

<<<<<<< HEAD
  virtual std::string GetName() const override {
    return "SourceSurface Creation";
  }
  virtual ReferencePtr GetObjectRef() const override { return mRefPtr; }
||||||| merged common ancestors
  virtual std::string GetName() const override { return "SourceSurface Creation"; }
  virtual ReferencePtr GetObjectRef() const override { return mRefPtr; }
=======
  std::string GetName() const override { return "SourceSurface Creation"; }
>>>>>>> upstream-releases

 private:
  friend class RecordedEvent;

  ReferencePtr mRefPtr;
  uint8_t* mData;
  int32_t mStride;
  IntSize mSize;
  SurfaceFormat mFormat;
  bool mDataOwned;

<<<<<<< HEAD
  template <class S>
  MOZ_IMPLICIT RecordedSourceSurfaceCreation(S &aStream);
||||||| merged common ancestors
  template<class S>
  MOZ_IMPLICIT RecordedSourceSurfaceCreation(S &aStream);
=======
  template <class S>
  MOZ_IMPLICIT RecordedSourceSurfaceCreation(S& aStream);
>>>>>>> upstream-releases
};

class RecordedSourceSurfaceDestruction
    : public RecordedEventDerived<RecordedSourceSurfaceDestruction> {
 public:
  MOZ_IMPLICIT RecordedSourceSurfaceDestruction(ReferencePtr aRefPtr)
      : RecordedEventDerived(SOURCESURFACEDESTRUCTION), mRefPtr(aRefPtr) {}

  bool PlayEvent(Translator* aTranslator) const override;

<<<<<<< HEAD
  template <class S>
  void Record(S &aStream) const;
  virtual void OutputSimpleEventInfo(
      std::stringstream &aStringStream) const override;
||||||| merged common ancestors
  template<class S> void Record(S &aStream) const;
  virtual void OutputSimpleEventInfo(std::stringstream &aStringStream) const override;
=======
  template <class S>
  void Record(S& aStream) const;
  void OutputSimpleEventInfo(std::stringstream& aStringStream) const override;
>>>>>>> upstream-releases

<<<<<<< HEAD
  virtual std::string GetName() const override {
    return "SourceSurface Destruction";
  }
  virtual ReferencePtr GetObjectRef() const override { return mRefPtr; }
||||||| merged common ancestors
  virtual std::string GetName() const override { return "SourceSurface Destruction"; }
  virtual ReferencePtr GetObjectRef() const override { return mRefPtr; }
=======
  std::string GetName() const override { return "SourceSurface Destruction"; }
>>>>>>> upstream-releases

 private:
  friend class RecordedEvent;

  ReferencePtr mRefPtr;

<<<<<<< HEAD
  template <class S>
  MOZ_IMPLICIT RecordedSourceSurfaceDestruction(S &aStream);
||||||| merged common ancestors
  template<class S>
  MOZ_IMPLICIT RecordedSourceSurfaceDestruction(S &aStream);
=======
  template <class S>
  MOZ_IMPLICIT RecordedSourceSurfaceDestruction(S& aStream);
>>>>>>> upstream-releases
};

class RecordedExternalSurfaceCreation
    : public RecordedEventDerived<RecordedExternalSurfaceCreation> {
 public:
  RecordedExternalSurfaceCreation(ReferencePtr aRefPtr, const uint64_t aKey)
      : RecordedEventDerived(EXTERNALSURFACECREATION),
        mRefPtr(aRefPtr),
        mKey(aKey) {}

  ~RecordedExternalSurfaceCreation() {}

  virtual bool PlayEvent(Translator* aTranslator) const;

<<<<<<< HEAD
  template <class S>
  void Record(S &aStream) const;
  virtual void OutputSimpleEventInfo(std::stringstream &aStringStream) const;
||||||| merged common ancestors
  template<class S> void Record(S &aStream) const;
  virtual void OutputSimpleEventInfo(std::stringstream &aStringStream) const;
=======
  template <class S>
  void Record(S& aStream) const;
  virtual void OutputSimpleEventInfo(std::stringstream& aStringStream) const;
>>>>>>> upstream-releases

<<<<<<< HEAD
  virtual std::string GetName() const {
    return "SourceSurfaceSharedData Creation";
  }
  virtual ReferencePtr GetObjectRef() const { return mRefPtr; }

 private:
||||||| merged common ancestors
  virtual std::string GetName() const { return "SourceSurfaceSharedData Creation"; }
  virtual ReferencePtr GetObjectRef() const { return mRefPtr; }
private:
=======
  virtual std::string GetName() const {
    return "SourceSurfaceSharedData Creation";
  }

 private:
>>>>>>> upstream-releases
  friend class RecordedEvent;

  ReferencePtr mRefPtr;
  uint64_t mKey;

<<<<<<< HEAD
  template <class S>
  MOZ_IMPLICIT RecordedExternalSurfaceCreation(S &aStream);
||||||| merged common ancestors
  template<class S>
  MOZ_IMPLICIT RecordedExternalSurfaceCreation(S &aStream);
=======
  template <class S>
  MOZ_IMPLICIT RecordedExternalSurfaceCreation(S& aStream);
>>>>>>> upstream-releases
};

class RecordedFilterNodeCreation
    : public RecordedEventDerived<RecordedFilterNodeCreation> {
 public:
  RecordedFilterNodeCreation(ReferencePtr aRefPtr, FilterType aType)
      : RecordedEventDerived(FILTERNODECREATION),
        mRefPtr(aRefPtr),
        mType(aType) {}

  ~RecordedFilterNodeCreation();

  bool PlayEvent(Translator* aTranslator) const override;

<<<<<<< HEAD
  template <class S>
  void Record(S &aStream) const;
  virtual void OutputSimpleEventInfo(
      std::stringstream &aStringStream) const override;
||||||| merged common ancestors
  template<class S> void Record(S &aStream) const;
  virtual void OutputSimpleEventInfo(std::stringstream &aStringStream) const override;
=======
  template <class S>
  void Record(S& aStream) const;
  void OutputSimpleEventInfo(std::stringstream& aStringStream) const override;
>>>>>>> upstream-releases

  std::string GetName() const override { return "FilterNode Creation"; }

 private:
  friend class RecordedEvent;

  ReferencePtr mRefPtr;
  FilterType mType;

<<<<<<< HEAD
  template <class S>
  MOZ_IMPLICIT RecordedFilterNodeCreation(S &aStream);
||||||| merged common ancestors
  template<class S>
  MOZ_IMPLICIT RecordedFilterNodeCreation(S &aStream);
=======
  template <class S>
  MOZ_IMPLICIT RecordedFilterNodeCreation(S& aStream);
>>>>>>> upstream-releases
};

class RecordedFilterNodeDestruction
    : public RecordedEventDerived<RecordedFilterNodeDestruction> {
 public:
  MOZ_IMPLICIT RecordedFilterNodeDestruction(ReferencePtr aRefPtr)
      : RecordedEventDerived(FILTERNODEDESTRUCTION), mRefPtr(aRefPtr) {}

  bool PlayEvent(Translator* aTranslator) const override;

<<<<<<< HEAD
  template <class S>
  void Record(S &aStream) const;
  virtual void OutputSimpleEventInfo(
      std::stringstream &aStringStream) const override;
||||||| merged common ancestors
  template<class S> void Record(S &aStream) const;
  virtual void OutputSimpleEventInfo(std::stringstream &aStringStream) const override;
=======
  template <class S>
  void Record(S& aStream) const;
  void OutputSimpleEventInfo(std::stringstream& aStringStream) const override;
>>>>>>> upstream-releases

<<<<<<< HEAD
  virtual std::string GetName() const override {
    return "FilterNode Destruction";
  }
  virtual ReferencePtr GetObjectRef() const override { return mRefPtr; }
||||||| merged common ancestors
  virtual std::string GetName() const override { return "FilterNode Destruction"; }
  virtual ReferencePtr GetObjectRef() const override { return mRefPtr; }
=======
  std::string GetName() const override { return "FilterNode Destruction"; }
>>>>>>> upstream-releases

 private:
  friend class RecordedEvent;

  ReferencePtr mRefPtr;

<<<<<<< HEAD
  template <class S>
  MOZ_IMPLICIT RecordedFilterNodeDestruction(S &aStream);
||||||| merged common ancestors
  template<class S>
  MOZ_IMPLICIT RecordedFilterNodeDestruction(S &aStream);
=======
  template <class S>
  MOZ_IMPLICIT RecordedFilterNodeDestruction(S& aStream);
>>>>>>> upstream-releases
};

<<<<<<< HEAD
class RecordedGradientStopsCreation
    : public RecordedEventDerived<RecordedGradientStopsCreation> {
 public:
  RecordedGradientStopsCreation(ReferencePtr aRefPtr, GradientStop *aStops,
||||||| merged common ancestors
class RecordedGradientStopsCreation : public RecordedEventDerived<RecordedGradientStopsCreation> {
public:
  RecordedGradientStopsCreation(ReferencePtr aRefPtr, GradientStop *aStops,
=======
class RecordedGradientStopsCreation
    : public RecordedEventDerived<RecordedGradientStopsCreation> {
 public:
  RecordedGradientStopsCreation(ReferencePtr aRefPtr, GradientStop* aStops,
>>>>>>> upstream-releases
                                uint32_t aNumStops, ExtendMode aExtendMode)
      : RecordedEventDerived(GRADIENTSTOPSCREATION),
        mRefPtr(aRefPtr),
        mStops(aStops),
        mNumStops(aNumStops),
        mExtendMode(aExtendMode),
        mDataOwned(false) {}

  ~RecordedGradientStopsCreation();

  bool PlayEvent(Translator* aTranslator) const override;

<<<<<<< HEAD
  template <class S>
  void Record(S &aStream) const;
  virtual void OutputSimpleEventInfo(
      std::stringstream &aStringStream) const override;
||||||| merged common ancestors
  template<class S> void Record(S &aStream) const;
  virtual void OutputSimpleEventInfo(std::stringstream &aStringStream) const override;
=======
  template <class S>
  void Record(S& aStream) const;
  void OutputSimpleEventInfo(std::stringstream& aStringStream) const override;
>>>>>>> upstream-releases

<<<<<<< HEAD
  virtual std::string GetName() const override {
    return "GradientStops Creation";
  }
  virtual ReferencePtr GetObjectRef() const override { return mRefPtr; }
||||||| merged common ancestors
  virtual std::string GetName() const override { return "GradientStops Creation"; }
  virtual ReferencePtr GetObjectRef() const override { return mRefPtr; }
=======
  std::string GetName() const override { return "GradientStops Creation"; }
>>>>>>> upstream-releases

 private:
  friend class RecordedEvent;

  ReferencePtr mRefPtr;
  GradientStop* mStops;
  uint32_t mNumStops;
  ExtendMode mExtendMode;
  bool mDataOwned;

<<<<<<< HEAD
  template <class S>
  MOZ_IMPLICIT RecordedGradientStopsCreation(S &aStream);
||||||| merged common ancestors
  template<class S>
  MOZ_IMPLICIT RecordedGradientStopsCreation(S &aStream);
=======
  template <class S>
  MOZ_IMPLICIT RecordedGradientStopsCreation(S& aStream);
>>>>>>> upstream-releases
};

class RecordedGradientStopsDestruction
    : public RecordedEventDerived<RecordedGradientStopsDestruction> {
 public:
  MOZ_IMPLICIT RecordedGradientStopsDestruction(ReferencePtr aRefPtr)
      : RecordedEventDerived(GRADIENTSTOPSDESTRUCTION), mRefPtr(aRefPtr) {}

  bool PlayEvent(Translator* aTranslator) const override;

<<<<<<< HEAD
  template <class S>
  void Record(S &aStream) const;
  virtual void OutputSimpleEventInfo(
      std::stringstream &aStringStream) const override;
||||||| merged common ancestors
  template<class S> void Record(S &aStream) const;
  virtual void OutputSimpleEventInfo(std::stringstream &aStringStream) const override;
=======
  template <class S>
  void Record(S& aStream) const;
  void OutputSimpleEventInfo(std::stringstream& aStringStream) const override;
>>>>>>> upstream-releases

<<<<<<< HEAD
  virtual std::string GetName() const override {
    return "GradientStops Destruction";
  }
  virtual ReferencePtr GetObjectRef() const override { return mRefPtr; }
||||||| merged common ancestors
  virtual std::string GetName() const override { return "GradientStops Destruction"; }
  virtual ReferencePtr GetObjectRef() const override { return mRefPtr; }
=======
  std::string GetName() const override { return "GradientStops Destruction"; }
>>>>>>> upstream-releases

 private:
  friend class RecordedEvent;

  ReferencePtr mRefPtr;

<<<<<<< HEAD
  template <class S>
  MOZ_IMPLICIT RecordedGradientStopsDestruction(S &aStream);
||||||| merged common ancestors
  template<class S>
  MOZ_IMPLICIT RecordedGradientStopsDestruction(S &aStream);
=======
  template <class S>
  MOZ_IMPLICIT RecordedGradientStopsDestruction(S& aStream);
};

class RecordedFlush : public RecordedDrawingEvent<RecordedFlush> {
 public:
  explicit RecordedFlush(DrawTarget* aDT) : RecordedDrawingEvent(FLUSH, aDT) {}

  bool PlayEvent(Translator* aTranslator) const final;

  template <class S>
  void Record(S& aStream) const;
  virtual void OutputSimpleEventInfo(
      std::stringstream& aStringStream) const override;

  virtual std::string GetName() const override { return "Flush"; }

 private:
  friend class RecordedEvent;

  template <class S>
  MOZ_IMPLICIT RecordedFlush(S& aStream);
};

class RecordedDetachAllSnapshots
    : public RecordedDrawingEvent<RecordedDetachAllSnapshots> {
 public:
  explicit RecordedDetachAllSnapshots(DrawTarget* aDT)
      : RecordedDrawingEvent(DETACHALLSNAPSHOTS, aDT) {}

  bool PlayEvent(Translator* aTranslator) const final;

  template <class S>
  void Record(S& aStream) const;
  virtual void OutputSimpleEventInfo(
      std::stringstream& aStringStream) const override;

  virtual std::string GetName() const override { return "DetachAllSnapshots"; }

 private:
  friend class RecordedEvent;

  template <class S>
  MOZ_IMPLICIT RecordedDetachAllSnapshots(S& aStream);
>>>>>>> upstream-releases
};

class RecordedSnapshot : public RecordedEventDerived<RecordedSnapshot> {
<<<<<<< HEAD
 public:
  RecordedSnapshot(ReferencePtr aRefPtr, DrawTarget *aDT)
      : RecordedEventDerived(SNAPSHOT), mRefPtr(aRefPtr), mDT(aDT) {}
||||||| merged common ancestors
public:
  RecordedSnapshot(ReferencePtr aRefPtr, DrawTarget *aDT)
    : RecordedEventDerived(SNAPSHOT), mRefPtr(aRefPtr), mDT(aDT)
  {
  }
=======
 public:
  RecordedSnapshot(ReferencePtr aRefPtr, DrawTarget* aDT)
      : RecordedEventDerived(SNAPSHOT), mRefPtr(aRefPtr), mDT(aDT) {}
>>>>>>> upstream-releases

  bool PlayEvent(Translator* aTranslator) const override;

<<<<<<< HEAD
  template <class S>
  void Record(S &aStream) const;
  virtual void OutputSimpleEventInfo(
      std::stringstream &aStringStream) const override;
||||||| merged common ancestors
  template<class S> void Record(S &aStream) const;
  virtual void OutputSimpleEventInfo(std::stringstream &aStringStream) const override;
=======
  template <class S>
  void Record(S& aStream) const;
  void OutputSimpleEventInfo(std::stringstream& aStringStream) const override;
>>>>>>> upstream-releases

  std::string GetName() const override { return "Snapshot"; }

 private:
  friend class RecordedEvent;

  ReferencePtr mRefPtr;
  ReferencePtr mDT;

<<<<<<< HEAD
  template <class S>
  MOZ_IMPLICIT RecordedSnapshot(S &aStream);
||||||| merged common ancestors
  template<class S>
  MOZ_IMPLICIT RecordedSnapshot(S &aStream);
=======
  template <class S>
  MOZ_IMPLICIT RecordedSnapshot(S& aStream);
>>>>>>> upstream-releases
};

<<<<<<< HEAD
class RecordedIntoLuminanceSource
    : public RecordedEventDerived<RecordedIntoLuminanceSource> {
 public:
  RecordedIntoLuminanceSource(ReferencePtr aRefPtr, DrawTarget *aDT,
||||||| merged common ancestors
class RecordedIntoLuminanceSource : public RecordedEventDerived<RecordedIntoLuminanceSource> {
public:
  RecordedIntoLuminanceSource(ReferencePtr aRefPtr, DrawTarget *aDT,
=======
class RecordedIntoLuminanceSource
    : public RecordedEventDerived<RecordedIntoLuminanceSource> {
 public:
  RecordedIntoLuminanceSource(ReferencePtr aRefPtr, DrawTarget* aDT,
>>>>>>> upstream-releases
                              LuminanceType aLuminanceType, float aOpacity)
      : RecordedEventDerived(INTOLUMINANCE),
        mRefPtr(aRefPtr),
        mDT(aDT),
        mLuminanceType(aLuminanceType),
        mOpacity(aOpacity) {}

  bool PlayEvent(Translator* aTranslator) const override;

<<<<<<< HEAD
  template <class S>
  void Record(S &aStream) const;
  virtual void OutputSimpleEventInfo(
      std::stringstream &aStringStream) const override;
||||||| merged common ancestors
  template<class S> void Record(S &aStream) const;
  virtual void OutputSimpleEventInfo(std::stringstream &aStringStream) const override;
=======
  template <class S>
  void Record(S& aStream) const;
  void OutputSimpleEventInfo(std::stringstream& aStringStream) const override;
>>>>>>> upstream-releases

  std::string GetName() const override { return "IntoLuminanceSource"; }

 private:
  friend class RecordedEvent;

  ReferencePtr mRefPtr;
  ReferencePtr mDT;
  LuminanceType mLuminanceType;
  float mOpacity;

<<<<<<< HEAD
  template <class S>
  MOZ_IMPLICIT RecordedIntoLuminanceSource(S &aStream);
||||||| merged common ancestors
  template<class S>
  MOZ_IMPLICIT RecordedIntoLuminanceSource(S &aStream);
=======
  template <class S>
  MOZ_IMPLICIT RecordedIntoLuminanceSource(S& aStream);
>>>>>>> upstream-releases
};

class RecordedFontData : public RecordedEventDerived<RecordedFontData> {
<<<<<<< HEAD
 public:
  static void FontDataProc(const uint8_t *aData, uint32_t aSize,
                           uint32_t aIndex, void *aBaton) {
    auto recordedFontData = static_cast<RecordedFontData *>(aBaton);
||||||| merged common ancestors
public:

  static void FontDataProc(const uint8_t *aData, uint32_t aSize,
                           uint32_t aIndex, void* aBaton)
  {
    auto recordedFontData = static_cast<RecordedFontData*>(aBaton);
=======
 public:
  static void FontDataProc(const uint8_t* aData, uint32_t aSize,
                           uint32_t aIndex, void* aBaton) {
    auto recordedFontData = static_cast<RecordedFontData*>(aBaton);
>>>>>>> upstream-releases
    recordedFontData->SetFontData(aData, aSize, aIndex);
  }

<<<<<<< HEAD
  explicit RecordedFontData(UnscaledFont *aUnscaledFont)
      : RecordedEventDerived(FONTDATA),
        mType(aUnscaledFont->GetType()),
        mData(nullptr),
        mFontDetails() {
    mGetFontFileDataSucceeded =
        aUnscaledFont->GetFontFileData(&FontDataProc, this) && mData;
||||||| merged common ancestors
  explicit RecordedFontData(UnscaledFont *aUnscaledFont)
    : RecordedEventDerived(FONTDATA)
    , mType(aUnscaledFont->GetType())
    , mData(nullptr)
    , mFontDetails()
  {
    mGetFontFileDataSucceeded = aUnscaledFont->GetFontFileData(&FontDataProc, this) && mData;
=======
  explicit RecordedFontData(UnscaledFont* aUnscaledFont)
      : RecordedEventDerived(FONTDATA),
        mType(aUnscaledFont->GetType()),
        mData(nullptr),
        mFontDetails() {
    mGetFontFileDataSucceeded =
        aUnscaledFont->GetFontFileData(&FontDataProc, this) && mData;
>>>>>>> upstream-releases
  }

  virtual ~RecordedFontData();

  bool IsValid() const { return mGetFontFileDataSucceeded; }

  bool PlayEvent(Translator* aTranslator) const override;

<<<<<<< HEAD
  template <class S>
  void Record(S &aStream) const;
  virtual void OutputSimpleEventInfo(
      std::stringstream &aStringStream) const override;
||||||| merged common ancestors
  template<class S> void Record(S &aStream) const;
  virtual void OutputSimpleEventInfo(std::stringstream &aStringStream) const override;
=======
  template <class S>
  void Record(S& aStream) const;
  void OutputSimpleEventInfo(std::stringstream& aStringStream) const override;
>>>>>>> upstream-releases

  std::string GetName() const override { return "Font Data"; }

  void SetFontData(const uint8_t* aData, uint32_t aSize, uint32_t aIndex);

  bool GetFontDetails(RecordedFontDetails &fontDetails);

 private:
  friend class RecordedEvent;

  FontType mType;
  uint8_t *mData;
  RecordedFontDetails mFontDetails;

  bool mGetFontFileDataSucceeded;

<<<<<<< HEAD
  template <class S>
  MOZ_IMPLICIT RecordedFontData(S &aStream);
||||||| merged common ancestors
  template<class S>
  MOZ_IMPLICIT RecordedFontData(S &aStream);
=======
  template <class S>
  MOZ_IMPLICIT RecordedFontData(S& aStream);
>>>>>>> upstream-releases
};

<<<<<<< HEAD
class RecordedFontDescriptor
    : public RecordedEventDerived<RecordedFontDescriptor> {
 public:
  static void FontDescCb(const uint8_t *aData, uint32_t aSize, uint32_t aIndex,
                         void *aBaton) {
    auto recordedFontDesc = static_cast<RecordedFontDescriptor *>(aBaton);
||||||| merged common ancestors
class RecordedFontDescriptor : public RecordedEventDerived<RecordedFontDescriptor> {
public:

  static void FontDescCb(const uint8_t* aData, uint32_t aSize, uint32_t aIndex,
                         void* aBaton)
  {
    auto recordedFontDesc = static_cast<RecordedFontDescriptor*>(aBaton);
=======
class RecordedFontDescriptor
    : public RecordedEventDerived<RecordedFontDescriptor> {
 public:
  static void FontDescCb(const uint8_t* aData, uint32_t aSize, uint32_t aIndex,
                         void* aBaton) {
    auto recordedFontDesc = static_cast<RecordedFontDescriptor*>(aBaton);
>>>>>>> upstream-releases
    recordedFontDesc->SetFontDescriptor(aData, aSize, aIndex);
  }

<<<<<<< HEAD
  explicit RecordedFontDescriptor(UnscaledFont *aUnscaledFont)
      : RecordedEventDerived(FONTDESC),
        mType(aUnscaledFont->GetType()),
        mIndex(0),
        mRefPtr(aUnscaledFont) {
||||||| merged common ancestors
  explicit RecordedFontDescriptor(UnscaledFont* aUnscaledFont)
    : RecordedEventDerived(FONTDESC)
    , mType(aUnscaledFont->GetType())
    , mIndex(0)
    , mRefPtr(aUnscaledFont)
  {
=======
  explicit RecordedFontDescriptor(UnscaledFont* aUnscaledFont)
      : RecordedEventDerived(FONTDESC),
        mType(aUnscaledFont->GetType()),
        mIndex(0),
        mRefPtr(aUnscaledFont) {
>>>>>>> upstream-releases
    mHasDesc = aUnscaledFont->GetFontDescriptor(FontDescCb, this);
  }

  virtual ~RecordedFontDescriptor();

  bool IsValid() const { return mHasDesc; }

  bool PlayEvent(Translator* aTranslator) const override;

<<<<<<< HEAD
  template <class S>
  void Record(S &aStream) const;
  virtual void OutputSimpleEventInfo(
      std::stringstream &aStringStream) const override;
||||||| merged common ancestors
  template<class S> void Record(S &aStream) const;
  virtual void OutputSimpleEventInfo(std::stringstream &aStringStream) const override;
=======
  template <class S>
  void Record(S& aStream) const;
  void OutputSimpleEventInfo(std::stringstream& aStringStream) const override;
>>>>>>> upstream-releases

  std::string GetName() const override { return "Font Desc"; }

 private:
  friend class RecordedEvent;

  void SetFontDescriptor(const uint8_t *aData, uint32_t aSize, uint32_t aIndex);

  bool mHasDesc;

  FontType mType;
  std::vector<uint8_t> mData;
  uint32_t mIndex;
  ReferencePtr mRefPtr;

<<<<<<< HEAD
  template <class S>
  MOZ_IMPLICIT RecordedFontDescriptor(S &aStream);
||||||| merged common ancestors
  template<class S>
  MOZ_IMPLICIT RecordedFontDescriptor(S &aStream);
=======
  template <class S>
  MOZ_IMPLICIT RecordedFontDescriptor(S& aStream);
>>>>>>> upstream-releases
};

<<<<<<< HEAD
class RecordedUnscaledFontCreation
    : public RecordedEventDerived<RecordedUnscaledFontCreation> {
 public:
  static void FontInstanceDataProc(const uint8_t *aData, uint32_t aSize,
                                   void *aBaton) {
    auto recordedUnscaledFontCreation =
        static_cast<RecordedUnscaledFontCreation *>(aBaton);
||||||| merged common ancestors
class RecordedUnscaledFontCreation : public RecordedEventDerived<RecordedUnscaledFontCreation> {
public:
  static void FontInstanceDataProc(const uint8_t* aData, uint32_t aSize, void* aBaton)
  {
    auto recordedUnscaledFontCreation = static_cast<RecordedUnscaledFontCreation*>(aBaton);
=======
class RecordedUnscaledFontCreation
    : public RecordedEventDerived<RecordedUnscaledFontCreation> {
 public:
  static void FontInstanceDataProc(const uint8_t* aData, uint32_t aSize,
                                   void* aBaton) {
    auto recordedUnscaledFontCreation =
        static_cast<RecordedUnscaledFontCreation*>(aBaton);
>>>>>>> upstream-releases
    recordedUnscaledFontCreation->SetFontInstanceData(aData, aSize);
  }

  RecordedUnscaledFontCreation(UnscaledFont *aUnscaledFont,
                               RecordedFontDetails aFontDetails)
      : RecordedEventDerived(UNSCALEDFONTCREATION),
        mRefPtr(aUnscaledFont),
        mFontDataKey(aFontDetails.fontDataKey),
        mIndex(aFontDetails.index) {
    aUnscaledFont->GetFontInstanceData(FontInstanceDataProc, this);
  }

  bool PlayEvent(Translator* aTranslator) const override;

<<<<<<< HEAD
  template <class S>
  void Record(S &aStream) const;
  virtual void OutputSimpleEventInfo(
      std::stringstream &aStringStream) const override;
||||||| merged common ancestors
  template<class S> void Record(S &aStream) const;
  virtual void OutputSimpleEventInfo(std::stringstream &aStringStream) const override;
=======
  template <class S>
  void Record(S& aStream) const;
  void OutputSimpleEventInfo(std::stringstream& aStringStream) const override;
>>>>>>> upstream-releases

<<<<<<< HEAD
  virtual std::string GetName() const override {
    return "UnscaledFont Creation";
  }
  virtual ReferencePtr GetObjectRef() const override { return mRefPtr; }
||||||| merged common ancestors
  virtual std::string GetName() const override { return "UnscaledFont Creation"; }
  virtual ReferencePtr GetObjectRef() const override { return mRefPtr; }
=======
  std::string GetName() const override { return "UnscaledFont Creation"; }
>>>>>>> upstream-releases

  void SetFontInstanceData(const uint8_t* aData, uint32_t aSize);

 private:
  friend class RecordedEvent;

  ReferencePtr mRefPtr;
  uint64_t mFontDataKey;
  uint32_t mIndex;
  std::vector<uint8_t> mInstanceData;

<<<<<<< HEAD
  template <class S>
  MOZ_IMPLICIT RecordedUnscaledFontCreation(S &aStream);
||||||| merged common ancestors
  template<class S>
  MOZ_IMPLICIT RecordedUnscaledFontCreation(S &aStream);
=======
  template <class S>
  MOZ_IMPLICIT RecordedUnscaledFontCreation(S& aStream);
>>>>>>> upstream-releases
};

class RecordedUnscaledFontDestruction
    : public RecordedEventDerived<RecordedUnscaledFontDestruction> {
 public:
  MOZ_IMPLICIT RecordedUnscaledFontDestruction(ReferencePtr aRefPtr)
      : RecordedEventDerived(UNSCALEDFONTDESTRUCTION), mRefPtr(aRefPtr) {}

<<<<<<< HEAD
  virtual bool PlayEvent(Translator *aTranslator) const override;
  template <class S>
  void Record(S &aStream) const;
  virtual void OutputSimpleEventInfo(
      std::stringstream &aStringStream) const override;
||||||| merged common ancestors
  virtual bool PlayEvent(Translator *aTranslator) const override;
  template<class S>
  void Record(S &aStream) const;
  virtual void OutputSimpleEventInfo(std::stringstream &aStringStream) const override;
=======
  bool PlayEvent(Translator* aTranslator) const override;
  template <class S>
  void Record(S& aStream) const;
  void OutputSimpleEventInfo(std::stringstream& aStringStream) const override;
>>>>>>> upstream-releases

<<<<<<< HEAD
  virtual std::string GetName() const override {
    return "UnscaledFont Destruction";
  }
  virtual ReferencePtr GetObjectRef() const override { return mRefPtr; }
||||||| merged common ancestors
  virtual std::string GetName() const override { return "UnscaledFont Destruction"; }
  virtual ReferencePtr GetObjectRef() const override { return mRefPtr; }
=======
  std::string GetName() const override { return "UnscaledFont Destruction"; }
>>>>>>> upstream-releases

 private:
  friend class RecordedEvent;

  ReferencePtr mRefPtr;

<<<<<<< HEAD
  template <class S>
  MOZ_IMPLICIT RecordedUnscaledFontDestruction(S &aStream);
||||||| merged common ancestors
  template<class S>
  MOZ_IMPLICIT RecordedUnscaledFontDestruction(S &aStream);
=======
  template <class S>
  MOZ_IMPLICIT RecordedUnscaledFontDestruction(S& aStream);
>>>>>>> upstream-releases
};

<<<<<<< HEAD
class RecordedScaledFontCreation
    : public RecordedEventDerived<RecordedScaledFontCreation> {
 public:
  static void FontInstanceDataProc(const uint8_t *aData, uint32_t aSize,
                                   const FontVariation *aVariations,
                                   uint32_t aNumVariations, void *aBaton) {
    auto recordedScaledFontCreation =
        static_cast<RecordedScaledFontCreation *>(aBaton);
    recordedScaledFontCreation->SetFontInstanceData(aData, aSize, aVariations,
                                                    aNumVariations);
  }

  RecordedScaledFontCreation(ScaledFont *aScaledFont,
                             UnscaledFont *aUnscaledFont)
      : RecordedEventDerived(SCALEDFONTCREATION),
        mRefPtr(aScaledFont),
        mUnscaledFont(aUnscaledFont),
        mGlyphSize(aScaledFont->GetSize()) {
||||||| merged common ancestors
class RecordedScaledFontCreation : public RecordedEventDerived<RecordedScaledFontCreation> {
public:

  static void FontInstanceDataProc(const uint8_t* aData, uint32_t aSize,
                                   const FontVariation* aVariations, uint32_t aNumVariations,
                                   void* aBaton)
  {
    auto recordedScaledFontCreation = static_cast<RecordedScaledFontCreation*>(aBaton);
    recordedScaledFontCreation->SetFontInstanceData(aData, aSize, aVariations, aNumVariations);
  }

  RecordedScaledFontCreation(ScaledFont* aScaledFont,
                             UnscaledFont* aUnscaledFont)
    : RecordedEventDerived(SCALEDFONTCREATION)
    , mRefPtr(aScaledFont)
    , mUnscaledFont(aUnscaledFont)
    , mGlyphSize(aScaledFont->GetSize())
  {
=======
class RecordedScaledFontCreation
    : public RecordedEventDerived<RecordedScaledFontCreation> {
 public:
  static void FontInstanceDataProc(const uint8_t* aData, uint32_t aSize,
                                   const FontVariation* aVariations,
                                   uint32_t aNumVariations, void* aBaton) {
    auto recordedScaledFontCreation =
        static_cast<RecordedScaledFontCreation*>(aBaton);
    recordedScaledFontCreation->SetFontInstanceData(aData, aSize, aVariations,
                                                    aNumVariations);
  }

  RecordedScaledFontCreation(ScaledFont* aScaledFont,
                             UnscaledFont* aUnscaledFont)
      : RecordedEventDerived(SCALEDFONTCREATION),
        mRefPtr(aScaledFont),
        mUnscaledFont(aUnscaledFont),
        mGlyphSize(aScaledFont->GetSize()) {
>>>>>>> upstream-releases
    aScaledFont->GetFontInstanceData(FontInstanceDataProc, this);
  }

  bool PlayEvent(Translator* aTranslator) const override;

<<<<<<< HEAD
  template <class S>
  void Record(S &aStream) const;
  virtual void OutputSimpleEventInfo(
      std::stringstream &aStringStream) const override;
||||||| merged common ancestors
  template<class S> void Record(S &aStream) const;
  virtual void OutputSimpleEventInfo(std::stringstream &aStringStream) const override;
=======
  template <class S>
  void Record(S& aStream) const;
  void OutputSimpleEventInfo(std::stringstream& aStringStream) const override;
>>>>>>> upstream-releases

  std::string GetName() const override { return "ScaledFont Creation"; }

<<<<<<< HEAD
  void SetFontInstanceData(const uint8_t *aData, uint32_t aSize,
                           const FontVariation *aVariations,
                           uint32_t aNumVariations);
||||||| merged common ancestors
  void SetFontInstanceData(const uint8_t *aData, uint32_t aSize,
                           const FontVariation* aVariations, uint32_t aNumVariations);
=======
  void SetFontInstanceData(const uint8_t* aData, uint32_t aSize,
                           const FontVariation* aVariations,
                           uint32_t aNumVariations);
>>>>>>> upstream-releases

 private:
  friend class RecordedEvent;

  ReferencePtr mRefPtr;
  ReferencePtr mUnscaledFont;
  Float mGlyphSize;
  std::vector<uint8_t> mInstanceData;
  std::vector<FontVariation> mVariations;

<<<<<<< HEAD
  template <class S>
  MOZ_IMPLICIT RecordedScaledFontCreation(S &aStream);
||||||| merged common ancestors
  template<class S>
  MOZ_IMPLICIT RecordedScaledFontCreation(S &aStream);
=======
  template <class S>
  MOZ_IMPLICIT RecordedScaledFontCreation(S& aStream);
>>>>>>> upstream-releases
};

class RecordedScaledFontDestruction
    : public RecordedEventDerived<RecordedScaledFontDestruction> {
 public:
  MOZ_IMPLICIT RecordedScaledFontDestruction(ReferencePtr aRefPtr)
      : RecordedEventDerived(SCALEDFONTDESTRUCTION), mRefPtr(aRefPtr) {}

  bool PlayEvent(Translator* aTranslator) const override;

<<<<<<< HEAD
  template <class S>
  void Record(S &aStream) const;
  virtual void OutputSimpleEventInfo(
      std::stringstream &aStringStream) const override;
||||||| merged common ancestors
  template<class S>
  void Record(S &aStream) const;
  virtual void OutputSimpleEventInfo(std::stringstream &aStringStream) const override;
=======
  template <class S>
  void Record(S& aStream) const;
  void OutputSimpleEventInfo(std::stringstream& aStringStream) const override;
>>>>>>> upstream-releases

<<<<<<< HEAD
  virtual std::string GetName() const override {
    return "ScaledFont Destruction";
  }
  virtual ReferencePtr GetObjectRef() const override { return mRefPtr; }
||||||| merged common ancestors
  virtual std::string GetName() const override { return "ScaledFont Destruction"; }
  virtual ReferencePtr GetObjectRef() const override { return mRefPtr; }
=======
  std::string GetName() const override { return "ScaledFont Destruction"; }
>>>>>>> upstream-releases

 private:
  friend class RecordedEvent;

  ReferencePtr mRefPtr;

<<<<<<< HEAD
  template <class S>
  MOZ_IMPLICIT RecordedScaledFontDestruction(S &aStream);
||||||| merged common ancestors
  template<class S>
  MOZ_IMPLICIT RecordedScaledFontDestruction(S &aStream);
=======
  template <class S>
  MOZ_IMPLICIT RecordedScaledFontDestruction(S& aStream);
>>>>>>> upstream-releases
};

class RecordedMaskSurface : public RecordedDrawingEvent<RecordedMaskSurface> {
<<<<<<< HEAD
 public:
  RecordedMaskSurface(DrawTarget *aDT, const Pattern &aPattern,
                      ReferencePtr aRefMask, const Point &aOffset,
                      const DrawOptions &aOptions)
      : RecordedDrawingEvent(MASKSURFACE, aDT),
        mPattern(),
        mRefMask(aRefMask),
        mOffset(aOffset),
        mOptions(aOptions) {
||||||| merged common ancestors
public:
  RecordedMaskSurface(DrawTarget *aDT, const Pattern &aPattern, ReferencePtr aRefMask,
                      const Point &aOffset, const DrawOptions &aOptions)
    : RecordedDrawingEvent(MASKSURFACE, aDT), mPattern(), mRefMask(aRefMask),
      mOffset(aOffset), mOptions(aOptions)
  {
=======
 public:
  RecordedMaskSurface(DrawTarget* aDT, const Pattern& aPattern,
                      ReferencePtr aRefMask, const Point& aOffset,
                      const DrawOptions& aOptions)
      : RecordedDrawingEvent(MASKSURFACE, aDT),
        mPattern(),
        mRefMask(aRefMask),
        mOffset(aOffset),
        mOptions(aOptions) {
>>>>>>> upstream-releases
    StorePattern(mPattern, aPattern);
  }

  bool PlayEvent(Translator* aTranslator) const override;

<<<<<<< HEAD
  template <class S>
  void Record(S &aStream) const;
  virtual void OutputSimpleEventInfo(
      std::stringstream &aStringStream) const override;
||||||| merged common ancestors
  template<class S> void Record(S &aStream) const;
  virtual void OutputSimpleEventInfo(std::stringstream &aStringStream) const override;
=======
  template <class S>
  void Record(S& aStream) const;
  void OutputSimpleEventInfo(std::stringstream& aStringStream) const override;
>>>>>>> upstream-releases

  std::string GetName() const override { return "MaskSurface"; }

 private:
  friend class RecordedEvent;

<<<<<<< HEAD
  template <class S>
  MOZ_IMPLICIT RecordedMaskSurface(S &aStream);
||||||| merged common ancestors
  template<class S>
  MOZ_IMPLICIT RecordedMaskSurface(S &aStream);
=======
  template <class S>
  MOZ_IMPLICIT RecordedMaskSurface(S& aStream);
>>>>>>> upstream-releases

  PatternStorage mPattern;
  ReferencePtr mRefMask;
  Point mOffset;
  DrawOptions mOptions;
};

class RecordedFilterNodeSetAttribute
    : public RecordedEventDerived<RecordedFilterNodeSetAttribute> {
 public:
  enum ArgType {
    ARGTYPE_UINT32,
    ARGTYPE_BOOL,
    ARGTYPE_FLOAT,
    ARGTYPE_SIZE,
    ARGTYPE_INTSIZE,
    ARGTYPE_INTPOINT,
    ARGTYPE_RECT,
    ARGTYPE_INTRECT,
    ARGTYPE_POINT,
    ARGTYPE_MATRIX,
    ARGTYPE_MATRIX5X4,
    ARGTYPE_POINT3D,
    ARGTYPE_COLOR,
    ARGTYPE_FLOAT_ARRAY
  };

<<<<<<< HEAD
  template <typename T>
  RecordedFilterNodeSetAttribute(FilterNode *aNode, uint32_t aIndex,
                                 T aArgument, ArgType aArgType)
      : RecordedEventDerived(FILTERNODESETATTRIBUTE),
        mNode(aNode),
        mIndex(aIndex),
        mArgType(aArgType) {
||||||| merged common ancestors
  template<typename T>
  RecordedFilterNodeSetAttribute(FilterNode *aNode, uint32_t aIndex, T aArgument, ArgType aArgType)
    : RecordedEventDerived(FILTERNODESETATTRIBUTE), mNode(aNode), mIndex(aIndex), mArgType(aArgType)
  {
=======
  template <typename T>
  RecordedFilterNodeSetAttribute(FilterNode* aNode, uint32_t aIndex,
                                 T aArgument, ArgType aArgType)
      : RecordedEventDerived(FILTERNODESETATTRIBUTE),
        mNode(aNode),
        mIndex(aIndex),
        mArgType(aArgType) {
>>>>>>> upstream-releases
    mPayload.resize(sizeof(T));
    memcpy(&mPayload.front(), &aArgument, sizeof(T));
  }

<<<<<<< HEAD
  RecordedFilterNodeSetAttribute(FilterNode *aNode, uint32_t aIndex,
                                 const Float *aFloat, uint32_t aSize)
      : RecordedEventDerived(FILTERNODESETATTRIBUTE),
        mNode(aNode),
        mIndex(aIndex),
        mArgType(ARGTYPE_FLOAT_ARRAY) {
||||||| merged common ancestors
  RecordedFilterNodeSetAttribute(FilterNode *aNode, uint32_t aIndex, const Float *aFloat, uint32_t aSize)
    : RecordedEventDerived(FILTERNODESETATTRIBUTE), mNode(aNode), mIndex(aIndex), mArgType(ARGTYPE_FLOAT_ARRAY)
  {
=======
  RecordedFilterNodeSetAttribute(FilterNode* aNode, uint32_t aIndex,
                                 const Float* aFloat, uint32_t aSize)
      : RecordedEventDerived(FILTERNODESETATTRIBUTE),
        mNode(aNode),
        mIndex(aIndex),
        mArgType(ARGTYPE_FLOAT_ARRAY) {
>>>>>>> upstream-releases
    mPayload.resize(sizeof(Float) * aSize);
    memcpy(&mPayload.front(), aFloat, sizeof(Float) * aSize);
  }

<<<<<<< HEAD
  virtual bool PlayEvent(Translator *aTranslator) const override;
  template <class S>
  void Record(S &aStream) const;
  virtual void OutputSimpleEventInfo(
      std::stringstream &aStringStream) const override;
||||||| merged common ancestors
  virtual bool PlayEvent(Translator *aTranslator) const override;
  template<class S> void Record(S &aStream) const;
  virtual void OutputSimpleEventInfo(std::stringstream &aStringStream) const override;
=======
  bool PlayEvent(Translator* aTranslator) const override;
  template <class S>
  void Record(S& aStream) const;
  void OutputSimpleEventInfo(std::stringstream& aStringStream) const override;
>>>>>>> upstream-releases

  std::string GetName() const override { return "SetAttribute"; }

 private:
  friend class RecordedEvent;

  ReferencePtr mNode;

  uint32_t mIndex;
  ArgType mArgType;
  std::vector<uint8_t> mPayload;

<<<<<<< HEAD
  template <class S>
  MOZ_IMPLICIT RecordedFilterNodeSetAttribute(S &aStream);
||||||| merged common ancestors
  template<class S>
  MOZ_IMPLICIT RecordedFilterNodeSetAttribute(S &aStream);
=======
  template <class S>
  MOZ_IMPLICIT RecordedFilterNodeSetAttribute(S& aStream);
>>>>>>> upstream-releases
};

<<<<<<< HEAD
class RecordedFilterNodeSetInput
    : public RecordedEventDerived<RecordedFilterNodeSetInput> {
 public:
  RecordedFilterNodeSetInput(FilterNode *aNode, uint32_t aIndex,
                             FilterNode *aInputNode)
      : RecordedEventDerived(FILTERNODESETINPUT),
        mNode(aNode),
        mIndex(aIndex),
        mInputFilter(aInputNode),
        mInputSurface(nullptr) {}

  RecordedFilterNodeSetInput(FilterNode *aNode, uint32_t aIndex,
                             SourceSurface *aInputSurface)
      : RecordedEventDerived(FILTERNODESETINPUT),
        mNode(aNode),
        mIndex(aIndex),
        mInputFilter(nullptr),
        mInputSurface(aInputSurface) {}

  virtual bool PlayEvent(Translator *aTranslator) const override;
  template <class S>
  void Record(S &aStream) const;
  virtual void OutputSimpleEventInfo(
      std::stringstream &aStringStream) const override;

  virtual std::string GetName() const override { return "SetInput"; }
  virtual ReferencePtr GetObjectRef() const override { return mNode; }

 private:
||||||| merged common ancestors
class RecordedFilterNodeSetInput : public RecordedEventDerived<RecordedFilterNodeSetInput>
{
public:
  RecordedFilterNodeSetInput(FilterNode* aNode, uint32_t aIndex, FilterNode* aInputNode)
    : RecordedEventDerived(FILTERNODESETINPUT), mNode(aNode), mIndex(aIndex)
    , mInputFilter(aInputNode), mInputSurface(nullptr)
  {
  }

  RecordedFilterNodeSetInput(FilterNode *aNode, uint32_t aIndex, SourceSurface *aInputSurface)
    : RecordedEventDerived(FILTERNODESETINPUT), mNode(aNode), mIndex(aIndex)
    , mInputFilter(nullptr), mInputSurface(aInputSurface)
  {
  }

  virtual bool PlayEvent(Translator *aTranslator) const override;
  template<class S> void Record(S &aStream) const;
  virtual void OutputSimpleEventInfo(std::stringstream &aStringStream) const override;

  virtual std::string GetName() const override { return "SetInput"; }
  virtual ReferencePtr GetObjectRef() const override { return mNode; }

private:
=======
class RecordedFilterNodeSetInput
    : public RecordedEventDerived<RecordedFilterNodeSetInput> {
 public:
  RecordedFilterNodeSetInput(FilterNode* aNode, uint32_t aIndex,
                             FilterNode* aInputNode)
      : RecordedEventDerived(FILTERNODESETINPUT),
        mNode(aNode),
        mIndex(aIndex),
        mInputFilter(aInputNode),
        mInputSurface(nullptr) {}

  RecordedFilterNodeSetInput(FilterNode* aNode, uint32_t aIndex,
                             SourceSurface* aInputSurface)
      : RecordedEventDerived(FILTERNODESETINPUT),
        mNode(aNode),
        mIndex(aIndex),
        mInputFilter(nullptr),
        mInputSurface(aInputSurface) {}

  bool PlayEvent(Translator* aTranslator) const override;
  template <class S>
  void Record(S& aStream) const;
  void OutputSimpleEventInfo(std::stringstream& aStringStream) const override;

  std::string GetName() const override { return "SetInput"; }

 private:
>>>>>>> upstream-releases
  friend class RecordedEvent;

  ReferencePtr mNode;
  uint32_t mIndex;
  ReferencePtr mInputFilter;
  ReferencePtr mInputSurface;

<<<<<<< HEAD
  template <class S>
  MOZ_IMPLICIT RecordedFilterNodeSetInput(S &aStream);
||||||| merged common ancestors
  template<class S>
  MOZ_IMPLICIT RecordedFilterNodeSetInput(S &aStream);
=======
  template <class S>
  MOZ_IMPLICIT RecordedFilterNodeSetInput(S& aStream);
>>>>>>> upstream-releases
};

static std::string NameFromBackend(BackendType aType) {
  switch (aType) {
    case BackendType::NONE:
      return "None";
    case BackendType::DIRECT2D:
      return "Direct2D";
    default:
      return "Unknown";
  }
}

<<<<<<< HEAD
template <class S>
void RecordedEvent::RecordPatternData(S &aStream,
                                      const PatternStorage &aPattern) const {
||||||| merged common ancestors
template<class S>
void
RecordedEvent::RecordPatternData(S &aStream, const PatternStorage &aPattern) const
{
=======
template <class S>
void RecordedEvent::RecordPatternData(S& aStream,
                                      const PatternStorage& aPattern) const {
>>>>>>> upstream-releases
  WriteElement(aStream, aPattern.mType);

  switch (aPattern.mType) {
<<<<<<< HEAD
    case PatternType::COLOR: {
      WriteElement(aStream, *reinterpret_cast<const ColorPatternStorage *>(
                                &aPattern.mStorage));
||||||| merged common ancestors
  case PatternType::COLOR:
    {
      WriteElement(aStream, *reinterpret_cast<const ColorPatternStorage*>(&aPattern.mStorage));
=======
    case PatternType::COLOR: {
      WriteElement(aStream, *reinterpret_cast<const ColorPatternStorage*>(
                                &aPattern.mStorage));
>>>>>>> upstream-releases
      return;
    }
<<<<<<< HEAD
    case PatternType::LINEAR_GRADIENT: {
      WriteElement(aStream,
                   *reinterpret_cast<const LinearGradientPatternStorage *>(
                       &aPattern.mStorage));
||||||| merged common ancestors
  case PatternType::LINEAR_GRADIENT:
    {
      WriteElement(aStream, *reinterpret_cast<const LinearGradientPatternStorage*>(&aPattern.mStorage));
=======
    case PatternType::LINEAR_GRADIENT: {
      WriteElement(aStream,
                   *reinterpret_cast<const LinearGradientPatternStorage*>(
                       &aPattern.mStorage));
>>>>>>> upstream-releases
      return;
    }
<<<<<<< HEAD
    case PatternType::RADIAL_GRADIENT: {
      WriteElement(aStream,
                   *reinterpret_cast<const RadialGradientPatternStorage *>(
                       &aPattern.mStorage));
||||||| merged common ancestors
  case PatternType::RADIAL_GRADIENT:
    {
      WriteElement(aStream, *reinterpret_cast<const RadialGradientPatternStorage*>(&aPattern.mStorage));
=======
    case PatternType::RADIAL_GRADIENT: {
      WriteElement(aStream,
                   *reinterpret_cast<const RadialGradientPatternStorage*>(
                       &aPattern.mStorage));
>>>>>>> upstream-releases
      return;
    }
<<<<<<< HEAD
    case PatternType::SURFACE: {
      WriteElement(aStream, *reinterpret_cast<const SurfacePatternStorage *>(
                                &aPattern.mStorage));
||||||| merged common ancestors
  case PatternType::SURFACE:
    {
      WriteElement(aStream, *reinterpret_cast<const SurfacePatternStorage*>(&aPattern.mStorage));
=======
    case PatternType::SURFACE: {
      WriteElement(aStream, *reinterpret_cast<const SurfacePatternStorage*>(
                                &aPattern.mStorage));
>>>>>>> upstream-releases
      return;
    }
    default:
      return;
  }
}

<<<<<<< HEAD
template <class S>
void RecordedEvent::ReadPatternData(S &aStream,
                                    PatternStorage &aPattern) const {
  ReadElement(aStream, aPattern.mType);
||||||| merged common ancestors
template<class S>
void
RecordedEvent::ReadPatternData(S &aStream, PatternStorage &aPattern) const
{
  ReadElement(aStream, aPattern.mType);
=======
template <class S>
void RecordedEvent::ReadPatternData(S& aStream,
                                    PatternStorage& aPattern) const {
  ReadElementConstrained(aStream, aPattern.mType, PatternType::COLOR,
                         PatternType::RADIAL_GRADIENT);
>>>>>>> upstream-releases

  switch (aPattern.mType) {
<<<<<<< HEAD
    case PatternType::COLOR: {
      ReadElement(aStream,
                  *reinterpret_cast<ColorPatternStorage *>(&aPattern.mStorage));
||||||| merged common ancestors
  case PatternType::COLOR:
    {
      ReadElement(aStream, *reinterpret_cast<ColorPatternStorage*>(&aPattern.mStorage));
=======
    case PatternType::COLOR: {
      ReadElement(aStream,
                  *reinterpret_cast<ColorPatternStorage*>(&aPattern.mStorage));
>>>>>>> upstream-releases
      return;
    }
<<<<<<< HEAD
    case PatternType::LINEAR_GRADIENT: {
      ReadElement(aStream, *reinterpret_cast<LinearGradientPatternStorage *>(
                               &aPattern.mStorage));
||||||| merged common ancestors
  case PatternType::LINEAR_GRADIENT:
    {
      ReadElement(aStream, *reinterpret_cast<LinearGradientPatternStorage*>(&aPattern.mStorage));
=======
    case PatternType::LINEAR_GRADIENT: {
      ReadElement(aStream, *reinterpret_cast<LinearGradientPatternStorage*>(
                               &aPattern.mStorage));
>>>>>>> upstream-releases
      return;
    }
<<<<<<< HEAD
    case PatternType::RADIAL_GRADIENT: {
      ReadElement(aStream, *reinterpret_cast<RadialGradientPatternStorage *>(
                               &aPattern.mStorage));
||||||| merged common ancestors
  case PatternType::RADIAL_GRADIENT:
    {
      ReadElement(aStream, *reinterpret_cast<RadialGradientPatternStorage*>(&aPattern.mStorage));
=======
    case PatternType::RADIAL_GRADIENT: {
      ReadElement(aStream, *reinterpret_cast<RadialGradientPatternStorage*>(
                               &aPattern.mStorage));
>>>>>>> upstream-releases
      return;
    }
<<<<<<< HEAD
    case PatternType::SURFACE: {
      ReadElement(aStream, *reinterpret_cast<SurfacePatternStorage *>(
                               &aPattern.mStorage));
||||||| merged common ancestors
  case PatternType::SURFACE:
    {
      ReadElement(aStream, *reinterpret_cast<SurfacePatternStorage*>(&aPattern.mStorage));
=======
    case PatternType::SURFACE: {
      SurfacePatternStorage* sps =
          reinterpret_cast<SurfacePatternStorage*>(&aPattern.mStorage);
      ReadElement(aStream, *sps);
      if (sps->mExtend < ExtendMode::CLAMP ||
          sps->mExtend > ExtendMode::REFLECT) {
        gfxDevCrash(LogReason::InvalidConstrainedValueRead)
            << "Invalid ExtendMode read: value: " << int(sps->mExtend)
            << ", min: " << int(ExtendMode::CLAMP)
            << ", max: " << int(ExtendMode::REFLECT);
        aStream.SetIsBad();
      }

      if (sps->mSamplingFilter < SamplingFilter::GOOD ||
          sps->mSamplingFilter >= SamplingFilter::SENTINEL) {
        gfxDevCrash(LogReason::InvalidConstrainedValueRead)
            << "Invalid SamplingFilter read: value: "
            << int(sps->mSamplingFilter)
            << ", min: " << int(SamplingFilter::GOOD)
            << ", sentinel: " << int(SamplingFilter::SENTINEL);
        aStream.SetIsBad();
      }
>>>>>>> upstream-releases
      return;
    }
    default:
      return;
  }
}

<<<<<<< HEAD
inline void RecordedEvent::StorePattern(PatternStorage &aDestination,
                                        const Pattern &aSource) const {
||||||| merged common ancestors
inline void
RecordedEvent::StorePattern(PatternStorage &aDestination, const Pattern &aSource) const
{
=======
inline void RecordedEvent::StorePattern(PatternStorage& aDestination,
                                        const Pattern& aSource) const {
>>>>>>> upstream-releases
  aDestination.mType = aSource.GetType();

  switch (aSource.GetType()) {
<<<<<<< HEAD
    case PatternType::COLOR: {
      reinterpret_cast<ColorPatternStorage *>(&aDestination.mStorage)->mColor =
          static_cast<const ColorPattern *>(&aSource)->mColor;
||||||| merged common ancestors
  case PatternType::COLOR:
    {
      reinterpret_cast<ColorPatternStorage*>(&aDestination.mStorage)->mColor =
        static_cast<const ColorPattern*>(&aSource)->mColor;
=======
    case PatternType::COLOR: {
      reinterpret_cast<ColorPatternStorage*>(&aDestination.mStorage)->mColor =
          static_cast<const ColorPattern*>(&aSource)->mColor;
>>>>>>> upstream-releases
      return;
    }
<<<<<<< HEAD
    case PatternType::LINEAR_GRADIENT: {
      LinearGradientPatternStorage *store =
          reinterpret_cast<LinearGradientPatternStorage *>(
              &aDestination.mStorage);
      const LinearGradientPattern *pat =
          static_cast<const LinearGradientPattern *>(&aSource);
||||||| merged common ancestors
  case PatternType::LINEAR_GRADIENT:
    {
      LinearGradientPatternStorage *store =
        reinterpret_cast<LinearGradientPatternStorage*>(&aDestination.mStorage);
      const LinearGradientPattern *pat =
        static_cast<const LinearGradientPattern*>(&aSource);
=======
    case PatternType::LINEAR_GRADIENT: {
      LinearGradientPatternStorage* store =
          reinterpret_cast<LinearGradientPatternStorage*>(
              &aDestination.mStorage);
      const LinearGradientPattern* pat =
          static_cast<const LinearGradientPattern*>(&aSource);
>>>>>>> upstream-releases
      store->mBegin = pat->mBegin;
      store->mEnd = pat->mEnd;
      store->mMatrix = pat->mMatrix;
      store->mStops = pat->mStops.get();
      return;
    }
<<<<<<< HEAD
    case PatternType::RADIAL_GRADIENT: {
      RadialGradientPatternStorage *store =
          reinterpret_cast<RadialGradientPatternStorage *>(
              &aDestination.mStorage);
      const RadialGradientPattern *pat =
          static_cast<const RadialGradientPattern *>(&aSource);
||||||| merged common ancestors
  case PatternType::RADIAL_GRADIENT:
    {
      RadialGradientPatternStorage *store =
        reinterpret_cast<RadialGradientPatternStorage*>(&aDestination.mStorage);
      const RadialGradientPattern *pat =
        static_cast<const RadialGradientPattern*>(&aSource);
=======
    case PatternType::RADIAL_GRADIENT: {
      RadialGradientPatternStorage* store =
          reinterpret_cast<RadialGradientPatternStorage*>(
              &aDestination.mStorage);
      const RadialGradientPattern* pat =
          static_cast<const RadialGradientPattern*>(&aSource);
>>>>>>> upstream-releases
      store->mCenter1 = pat->mCenter1;
      store->mCenter2 = pat->mCenter2;
      store->mRadius1 = pat->mRadius1;
      store->mRadius2 = pat->mRadius2;
      store->mMatrix = pat->mMatrix;
      store->mStops = pat->mStops.get();
      return;
    }
<<<<<<< HEAD
    case PatternType::SURFACE: {
      SurfacePatternStorage *store =
          reinterpret_cast<SurfacePatternStorage *>(&aDestination.mStorage);
      const SurfacePattern *pat = static_cast<const SurfacePattern *>(&aSource);
||||||| merged common ancestors
  case PatternType::SURFACE:
    {
      SurfacePatternStorage *store =
        reinterpret_cast<SurfacePatternStorage*>(&aDestination.mStorage);
      const SurfacePattern *pat =
        static_cast<const SurfacePattern*>(&aSource);
=======
    case PatternType::SURFACE: {
      SurfacePatternStorage* store =
          reinterpret_cast<SurfacePatternStorage*>(&aDestination.mStorage);
      const SurfacePattern* pat = static_cast<const SurfacePattern*>(&aSource);
>>>>>>> upstream-releases
      store->mExtend = pat->mExtendMode;
      store->mSamplingFilter = pat->mSamplingFilter;
      store->mMatrix = pat->mMatrix;
      store->mSurface = pat->mSurface;
      store->mSamplingRect = pat->mSamplingRect;
      return;
    }
  }
}

<<<<<<< HEAD
template <class S>
void RecordedEvent::RecordStrokeOptions(
    S &aStream, const StrokeOptions &aStrokeOptions) const {
||||||| merged common ancestors
template<class S>
void
RecordedEvent::RecordStrokeOptions(S &aStream, const StrokeOptions &aStrokeOptions) const
{
=======
template <class S>
void RecordedEvent::RecordStrokeOptions(
    S& aStream, const StrokeOptions& aStrokeOptions) const {
>>>>>>> upstream-releases
  JoinStyle joinStyle = aStrokeOptions.mLineJoin;
  CapStyle capStyle = aStrokeOptions.mLineCap;

  WriteElement(aStream, uint64_t(aStrokeOptions.mDashLength));
  WriteElement(aStream, aStrokeOptions.mDashOffset);
  WriteElement(aStream, aStrokeOptions.mLineWidth);
  WriteElement(aStream, aStrokeOptions.mMiterLimit);
  WriteElement(aStream, joinStyle);
  WriteElement(aStream, capStyle);

  if (!aStrokeOptions.mDashPattern) {
    return;
  }

<<<<<<< HEAD
  aStream.write((char *)aStrokeOptions.mDashPattern,
                sizeof(Float) * aStrokeOptions.mDashLength);
||||||| merged common ancestors
  aStream.write((char*)aStrokeOptions.mDashPattern, sizeof(Float) * aStrokeOptions.mDashLength);
=======
  aStream.write((char*)aStrokeOptions.mDashPattern,
                sizeof(Float) * aStrokeOptions.mDashLength);
>>>>>>> upstream-releases
}

<<<<<<< HEAD
template <class S>
void RecordedEvent::ReadStrokeOptions(S &aStream,
                                      StrokeOptions &aStrokeOptions) {
||||||| merged common ancestors
template<class S>
void
RecordedEvent::ReadStrokeOptions(S &aStream, StrokeOptions &aStrokeOptions)
{
=======
template <class S>
void RecordedEvent::ReadStrokeOptions(S& aStream,
                                      StrokeOptions& aStrokeOptions) {
>>>>>>> upstream-releases
  uint64_t dashLength;
  JoinStyle joinStyle;
  CapStyle capStyle;

  ReadElement(aStream, dashLength);
  ReadElement(aStream, aStrokeOptions.mDashOffset);
  ReadElement(aStream, aStrokeOptions.mLineWidth);
  ReadElement(aStream, aStrokeOptions.mMiterLimit);
  ReadElementConstrained(aStream, joinStyle, JoinStyle::BEVEL,
                         JoinStyle::MITER_OR_BEVEL);
  ReadElementConstrained(aStream, capStyle, CapStyle::BUTT, CapStyle::SQUARE);
  // On 32 bit we truncate the value of dashLength.
  // See also bug 811850 for history.
  aStrokeOptions.mDashLength = size_t(dashLength);
  aStrokeOptions.mLineJoin = joinStyle;
  aStrokeOptions.mLineCap = capStyle;

  if (!aStrokeOptions.mDashLength) {
    return;
  }

  mDashPatternStorage.resize(aStrokeOptions.mDashLength);
  aStrokeOptions.mDashPattern = &mDashPatternStorage.front();
<<<<<<< HEAD
  aStream.read((char *)aStrokeOptions.mDashPattern,
               sizeof(Float) * aStrokeOptions.mDashLength);
}

inline void RecordedEvent::OutputSimplePatternInfo(
    const PatternStorage &aStorage, std::stringstream &aOutput) const {
||||||| merged common ancestors
  aStream.read((char*)aStrokeOptions.mDashPattern, sizeof(Float) * aStrokeOptions.mDashLength);
}

inline void
RecordedEvent::OutputSimplePatternInfo(const PatternStorage &aStorage, std::stringstream &aOutput) const
{
=======
  aStream.read((char*)aStrokeOptions.mDashPattern,
               sizeof(Float) * aStrokeOptions.mDashLength);
}

template <class S>
static void ReadDrawOptions(S& aStream, DrawOptions& aDrawOptions) {
  ReadElement(aStream, aDrawOptions);
  if (aDrawOptions.mAntialiasMode < AntialiasMode::NONE ||
      aDrawOptions.mAntialiasMode > AntialiasMode::DEFAULT) {
    gfxDevCrash(LogReason::InvalidConstrainedValueRead)
        << "Invalid AntialiasMode read: value: "
        << int(aDrawOptions.mAntialiasMode)
        << ", min: " << int(AntialiasMode::NONE)
        << ", max: " << int(AntialiasMode::DEFAULT);
    aStream.SetIsBad();
  }

  if (aDrawOptions.mCompositionOp < CompositionOp::OP_OVER ||
      aDrawOptions.mCompositionOp > CompositionOp::OP_COUNT) {
    gfxDevCrash(LogReason::InvalidConstrainedValueRead)
        << "Invalid CompositionOp read: value: "
        << int(aDrawOptions.mCompositionOp)
        << ", min: " << int(CompositionOp::OP_OVER)
        << ", max: " << int(CompositionOp::OP_COUNT);
    aStream.SetIsBad();
  }
}

template <class S>
static void ReadDrawSurfaceOptions(S& aStream,
                                   DrawSurfaceOptions& aDrawSurfaceOptions) {
  ReadElement(aStream, aDrawSurfaceOptions);
  if (aDrawSurfaceOptions.mSamplingFilter < SamplingFilter::GOOD ||
      aDrawSurfaceOptions.mSamplingFilter >= SamplingFilter::SENTINEL) {
    gfxDevCrash(LogReason::InvalidConstrainedValueRead)
        << "Invalid SamplingFilter read: value: "
        << int(aDrawSurfaceOptions.mSamplingFilter)
        << ", min: " << int(SamplingFilter::GOOD)
        << ", sentinel: " << int(SamplingFilter::SENTINEL);
    aStream.SetIsBad();
  }

  if (aDrawSurfaceOptions.mSamplingBounds < SamplingBounds::UNBOUNDED ||
      aDrawSurfaceOptions.mSamplingBounds > SamplingBounds::BOUNDED) {
    gfxDevCrash(LogReason::InvalidConstrainedValueRead)
        << "Invalid SamplingBounds read: value: "
        << int(aDrawSurfaceOptions.mSamplingBounds)
        << ", min: " << int(SamplingBounds::UNBOUNDED)
        << ", max: " << int(SamplingBounds::BOUNDED);
    aStream.SetIsBad();
  }
}

inline void RecordedEvent::OutputSimplePatternInfo(
    const PatternStorage& aStorage, std::stringstream& aOutput) const {
>>>>>>> upstream-releases
  switch (aStorage.mType) {
<<<<<<< HEAD
    case PatternType::COLOR: {
      const Color color =
          reinterpret_cast<const ColorPatternStorage *>(&aStorage.mStorage)
              ->mColor;
      aOutput << "Color: (" << color.r << ", " << color.g << ", " << color.b
              << ", " << color.a << ")";
||||||| merged common ancestors
  case PatternType::COLOR:
    {
      const Color color = reinterpret_cast<const ColorPatternStorage*>(&aStorage.mStorage)->mColor;
      aOutput << "Color: (" << color.r << ", " << color.g << ", " << color.b << ", " << color.a << ")";
=======
    case PatternType::COLOR: {
      const Color color =
          reinterpret_cast<const ColorPatternStorage*>(&aStorage.mStorage)
              ->mColor;
      aOutput << "Color: (" << color.r << ", " << color.g << ", " << color.b
              << ", " << color.a << ")";
>>>>>>> upstream-releases
      return;
    }
<<<<<<< HEAD
    case PatternType::LINEAR_GRADIENT: {
      const LinearGradientPatternStorage *store =
          reinterpret_cast<const LinearGradientPatternStorage *>(
              &aStorage.mStorage);

      aOutput << "LinearGradient (" << store->mBegin.x << ", "
              << store->mBegin.y << ") - (" << store->mEnd.x << ", "
              << store->mEnd.y << ") Stops: " << store->mStops;
||||||| merged common ancestors
  case PatternType::LINEAR_GRADIENT:
    {
      const LinearGradientPatternStorage *store =
        reinterpret_cast<const LinearGradientPatternStorage*>(&aStorage.mStorage);

      aOutput << "LinearGradient (" << store->mBegin.x << ", " << store->mBegin.y <<
        ") - (" << store->mEnd.x << ", " << store->mEnd.y << ") Stops: " << store->mStops;
=======
    case PatternType::LINEAR_GRADIENT: {
      const LinearGradientPatternStorage* store =
          reinterpret_cast<const LinearGradientPatternStorage*>(
              &aStorage.mStorage);

      aOutput << "LinearGradient (" << store->mBegin.x << ", "
              << store->mBegin.y << ") - (" << store->mEnd.x << ", "
              << store->mEnd.y << ") Stops: " << store->mStops;
>>>>>>> upstream-releases
      return;
    }
<<<<<<< HEAD
    case PatternType::RADIAL_GRADIENT: {
      const RadialGradientPatternStorage *store =
          reinterpret_cast<const RadialGradientPatternStorage *>(
              &aStorage.mStorage);
      aOutput << "RadialGradient (Center 1: (" << store->mCenter1.x << ", "
              << store->mCenter2.y << ") Radius 2: " << store->mRadius2;
||||||| merged common ancestors
  case PatternType::RADIAL_GRADIENT:
    {
      const RadialGradientPatternStorage *store =
        reinterpret_cast<const RadialGradientPatternStorage*>(&aStorage.mStorage);
      aOutput << "RadialGradient (Center 1: (" << store->mCenter1.x << ", " <<
        store->mCenter2.y << ") Radius 2: " << store->mRadius2;
=======
    case PatternType::RADIAL_GRADIENT: {
      const RadialGradientPatternStorage* store =
          reinterpret_cast<const RadialGradientPatternStorage*>(
              &aStorage.mStorage);
      aOutput << "RadialGradient (Center 1: (" << store->mCenter1.x << ", "
              << store->mCenter2.y << ") Radius 2: " << store->mRadius2;
>>>>>>> upstream-releases
      return;
    }
<<<<<<< HEAD
    case PatternType::SURFACE: {
      const SurfacePatternStorage *store =
          reinterpret_cast<const SurfacePatternStorage *>(&aStorage.mStorage);
||||||| merged common ancestors
  case PatternType::SURFACE:
    {
      const SurfacePatternStorage *store =
        reinterpret_cast<const SurfacePatternStorage*>(&aStorage.mStorage);
=======
    case PatternType::SURFACE: {
      const SurfacePatternStorage* store =
          reinterpret_cast<const SurfacePatternStorage*>(&aStorage.mStorage);
>>>>>>> upstream-releases
      aOutput << "Surface (0x" << store->mSurface << ")";
      return;
    }
  }
}

<<<<<<< HEAD
template <class T>
template <class S>
RecordedDrawingEvent<T>::RecordedDrawingEvent(RecordedEvent::EventType aType,
                                              S &aStream)
    : RecordedEventDerived<T>(aType) {
||||||| merged common ancestors
template<class T>
template<class S>
RecordedDrawingEvent<T>::RecordedDrawingEvent(RecordedEvent::EventType aType, S &aStream)
  : RecordedEventDerived<T>(aType)
{
=======
template <class T>
template <class S>
RecordedDrawingEvent<T>::RecordedDrawingEvent(RecordedEvent::EventType aType,
                                              S& aStream)
    : RecordedEventDerived<T>(aType) {
>>>>>>> upstream-releases
  ReadElement(aStream, mDT);
}

<<<<<<< HEAD
template <class T>
template <class S>
void RecordedDrawingEvent<T>::Record(S &aStream) const {
||||||| merged common ancestors
template<class T>
template<class S>
void
RecordedDrawingEvent<T>::Record(S &aStream) const
{
=======
template <class T>
template <class S>
void RecordedDrawingEvent<T>::Record(S& aStream) const {
>>>>>>> upstream-releases
  WriteElement(aStream, mDT);
}

<<<<<<< HEAD
template <class T>
ReferencePtr RecordedDrawingEvent<T>::GetObjectRef() const {
  return mDT;
}

inline bool RecordedDrawTargetCreation::PlayEvent(
    Translator *aTranslator) const {
||||||| merged common ancestors
template<class T>
ReferencePtr
RecordedDrawingEvent<T>::GetObjectRef() const
{
  return mDT;
}

inline bool
RecordedDrawTargetCreation::PlayEvent(Translator *aTranslator) const
{
=======
inline bool RecordedDrawTargetCreation::PlayEvent(
    Translator* aTranslator) const {
>>>>>>> upstream-releases
  RefPtr<DrawTarget> newDT =
      aTranslator->CreateDrawTarget(mRefPtr, mSize, mFormat);

  // If we couldn't create a DrawTarget this will probably cause us to crash
  // with nullptr later in the playback, so return false to abort.
  if (!newDT) {
    return false;
  }

  if (mHasExistingData) {
    Rect dataRect(0, 0, mExistingData->GetSize().width,
                  mExistingData->GetSize().height);
    newDT->DrawSurface(mExistingData, dataRect, dataRect);
  }

  return true;
}

<<<<<<< HEAD
template <class S>
void RecordedDrawTargetCreation::Record(S &aStream) const {
||||||| merged common ancestors
template<class S>
void
RecordedDrawTargetCreation::Record(S &aStream) const
{
=======
template <class S>
void RecordedDrawTargetCreation::Record(S& aStream) const {
>>>>>>> upstream-releases
  WriteElement(aStream, mRefPtr);
  WriteElement(aStream, mBackendType);
  WriteElement(aStream, mSize);
  WriteElement(aStream, mFormat);
  WriteElement(aStream, mHasExistingData);

  if (mHasExistingData) {
    MOZ_ASSERT(mExistingData);
    MOZ_ASSERT(mExistingData->GetSize() == mSize);
    RefPtr<DataSourceSurface> dataSurf = mExistingData->GetDataSurface();

    DataSourceSurface::ScopedMap map(dataSurf, DataSourceSurface::READ);
    for (int y = 0; y < mSize.height; y++) {
      aStream.write((const char *)map.GetData() + y * map.GetStride(),
                    BytesPerPixel(mFormat) * mSize.width);
    }
  }
}

<<<<<<< HEAD
template <class S>
RecordedDrawTargetCreation::RecordedDrawTargetCreation(S &aStream)
    : RecordedEventDerived(DRAWTARGETCREATION), mExistingData(nullptr) {
||||||| merged common ancestors
template<class S>
RecordedDrawTargetCreation::RecordedDrawTargetCreation(S &aStream)
  : RecordedEventDerived(DRAWTARGETCREATION)
  , mExistingData(nullptr)
{
=======
template <class S>
RecordedDrawTargetCreation::RecordedDrawTargetCreation(S& aStream)
    : RecordedEventDerived(DRAWTARGETCREATION), mExistingData(nullptr) {
>>>>>>> upstream-releases
  ReadElement(aStream, mRefPtr);
  ReadElementConstrained(aStream, mBackendType, BackendType::NONE,
                         BackendType::CAPTURE);
  ReadElement(aStream, mSize);
  ReadElementConstrained(aStream, mFormat, SurfaceFormat::A8R8G8B8_UINT32,
                         SurfaceFormat::UNKNOWN);
  ReadElement(aStream, mHasExistingData);

  if (mHasExistingData) {
    RefPtr<DataSourceSurface> dataSurf =
        Factory::CreateDataSourceSurface(mSize, mFormat);
    if (!dataSurf) {
      gfxWarning()
          << "RecordedDrawTargetCreation had to reset mHasExistingData";
      mHasExistingData = false;
      return;
    }

    DataSourceSurface::ScopedMap map(dataSurf, DataSourceSurface::READ);
    for (int y = 0; y < mSize.height; y++) {
<<<<<<< HEAD
      aStream.read((char *)map.GetData() + y * map.GetStride(),
                   BytesPerPixel(mFormat) * mSize.width);
||||||| merged common ancestors
      aStream.read((char*)map.GetData() + y * map.GetStride(),
                    BytesPerPixel(mFormat) * mSize.width);
=======
      aStream.read((char*)map.GetData() + y * map.GetStride(),
                   BytesPerPixel(mFormat) * mSize.width);
>>>>>>> upstream-releases
    }
    mExistingData = dataSurf;
  }
}

<<<<<<< HEAD
inline void RecordedDrawTargetCreation::OutputSimpleEventInfo(
    std::stringstream &aStringStream) const {
  aStringStream << "[" << mRefPtr << "] DrawTarget Creation (Type: "
                << NameFromBackend(mBackendType) << ", Size: " << mSize.width
                << "x" << mSize.height << ")";
||||||| merged common ancestors
inline void
RecordedDrawTargetCreation::OutputSimpleEventInfo(std::stringstream &aStringStream) const
{
  aStringStream << "[" << mRefPtr << "] DrawTarget Creation (Type: " << NameFromBackend(mBackendType) << ", Size: " << mSize.width << "x" << mSize.height << ")";
=======
inline void RecordedDrawTargetCreation::OutputSimpleEventInfo(
    std::stringstream& aStringStream) const {
  aStringStream << "[" << mRefPtr << "] DrawTarget Creation (Type: "
                << NameFromBackend(mBackendType) << ", Size: " << mSize.width
                << "x" << mSize.height << ")";
>>>>>>> upstream-releases
}

<<<<<<< HEAD
inline bool RecordedDrawTargetDestruction::PlayEvent(
    Translator *aTranslator) const {
||||||| merged common ancestors

inline bool
RecordedDrawTargetDestruction::PlayEvent(Translator *aTranslator) const
{
=======
inline bool RecordedDrawTargetDestruction::PlayEvent(
    Translator* aTranslator) const {
>>>>>>> upstream-releases
  aTranslator->RemoveDrawTarget(mRefPtr);
  return true;
}

<<<<<<< HEAD
template <class S>
void RecordedDrawTargetDestruction::Record(S &aStream) const {
||||||| merged common ancestors
template<class S>
void
RecordedDrawTargetDestruction::Record(S &aStream) const
{
=======
template <class S>
void RecordedDrawTargetDestruction::Record(S& aStream) const {
>>>>>>> upstream-releases
  WriteElement(aStream, mRefPtr);
}

<<<<<<< HEAD
template <class S>
RecordedDrawTargetDestruction::RecordedDrawTargetDestruction(S &aStream)
    : RecordedEventDerived(DRAWTARGETDESTRUCTION) {
||||||| merged common ancestors
template<class S>
RecordedDrawTargetDestruction::RecordedDrawTargetDestruction(S &aStream)
  : RecordedEventDerived(DRAWTARGETDESTRUCTION)
{
=======
template <class S>
RecordedDrawTargetDestruction::RecordedDrawTargetDestruction(S& aStream)
    : RecordedEventDerived(DRAWTARGETDESTRUCTION) {
>>>>>>> upstream-releases
  ReadElement(aStream, mRefPtr);
}

<<<<<<< HEAD
inline void RecordedDrawTargetDestruction::OutputSimpleEventInfo(
    std::stringstream &aStringStream) const {
||||||| merged common ancestors
inline void
RecordedDrawTargetDestruction::OutputSimpleEventInfo(std::stringstream &aStringStream) const
{
=======
inline void RecordedDrawTargetDestruction::OutputSimpleEventInfo(
    std::stringstream& aStringStream) const {
>>>>>>> upstream-releases
  aStringStream << "[" << mRefPtr << "] DrawTarget Destruction";
}

<<<<<<< HEAD
inline bool RecordedCreateSimilarDrawTarget::PlayEvent(
    Translator *aTranslator) const {
||||||| merged common ancestors
inline bool
RecordedCreateSimilarDrawTarget::PlayEvent(Translator *aTranslator) const
{
=======
inline bool RecordedCreateSimilarDrawTarget::PlayEvent(
    Translator* aTranslator) const {
>>>>>>> upstream-releases
  RefPtr<DrawTarget> newDT =
      aTranslator->GetReferenceDrawTarget()->CreateSimilarDrawTarget(mSize,
                                                                     mFormat);

  // If we couldn't create a DrawTarget this will probably cause us to crash
  // with nullptr later in the playback, so return false to abort.
  if (!newDT) {
    return false;
  }

  aTranslator->AddDrawTarget(mRefPtr, newDT);
  return true;
}

<<<<<<< HEAD
template <class S>
void RecordedCreateSimilarDrawTarget::Record(S &aStream) const {
||||||| merged common ancestors
template<class S>
void
RecordedCreateSimilarDrawTarget::Record(S &aStream) const
{
=======
template <class S>
void RecordedCreateSimilarDrawTarget::Record(S& aStream) const {
>>>>>>> upstream-releases
  WriteElement(aStream, mRefPtr);
  WriteElement(aStream, mSize);
  WriteElement(aStream, mFormat);
}

<<<<<<< HEAD
template <class S>
RecordedCreateSimilarDrawTarget::RecordedCreateSimilarDrawTarget(S &aStream)
    : RecordedEventDerived(CREATESIMILARDRAWTARGET) {
||||||| merged common ancestors
template<class S>
RecordedCreateSimilarDrawTarget::RecordedCreateSimilarDrawTarget(S &aStream)
  : RecordedEventDerived(CREATESIMILARDRAWTARGET)
{
=======
template <class S>
RecordedCreateSimilarDrawTarget::RecordedCreateSimilarDrawTarget(S& aStream)
    : RecordedEventDerived(CREATESIMILARDRAWTARGET) {
>>>>>>> upstream-releases
  ReadElement(aStream, mRefPtr);
  ReadElement(aStream, mSize);
  ReadElementConstrained(aStream, mFormat, SurfaceFormat::A8R8G8B8_UINT32,
                         SurfaceFormat::UNKNOWN);
}

<<<<<<< HEAD
inline void RecordedCreateSimilarDrawTarget::OutputSimpleEventInfo(
    std::stringstream &aStringStream) const {
  aStringStream << "[" << mRefPtr
                << "] CreateSimilarDrawTarget (Size: " << mSize.width << "x"
                << mSize.height << ")";
||||||| merged common ancestors
inline void
RecordedCreateSimilarDrawTarget::OutputSimpleEventInfo(std::stringstream &aStringStream) const
{
  aStringStream << "[" << mRefPtr << "] CreateSimilarDrawTarget (Size: " << mSize.width << "x" << mSize.height << ")";
=======
inline void RecordedCreateSimilarDrawTarget::OutputSimpleEventInfo(
    std::stringstream& aStringStream) const {
  aStringStream << "[" << mRefPtr
                << "] CreateSimilarDrawTarget (Size: " << mSize.width << "x"
                << mSize.height << ")";
>>>>>>> upstream-releases
}

<<<<<<< HEAD
inline bool RecordedCreateClippedDrawTarget::PlayEvent(
    Translator *aTranslator) const {
  const IntRect baseRect = aTranslator->GetReferenceDrawTarget()->GetRect();
  const IntRect transformedRect = RoundedToInt(
      mTransform.Inverse().TransformBounds(IntRectToRect(baseRect)));
  const IntRect intersection =
      IntRect(IntPoint(0, 0), mMaxSize).Intersect(transformedRect);
||||||| merged common ancestors
inline bool
RecordedCreateClippedDrawTarget::PlayEvent(Translator *aTranslator) const
{
  const IntRect baseRect = aTranslator->GetReferenceDrawTarget()->GetRect();
  const IntRect transformedRect = RoundedToInt(mTransform.Inverse().TransformBounds(IntRectToRect(baseRect)));
  const IntRect intersection = IntRect(IntPoint(0, 0), mMaxSize).Intersect(transformedRect);
=======
inline bool RecordedCreateDrawTargetForFilter::PlayEvent(
    Translator* aTranslator) const {
  IntRect baseRect = aTranslator->LookupDrawTarget(mDT)->GetRect();

  auto maxRect = IntRect(IntPoint(0, 0), mMaxSize);

  auto clone = aTranslator->LookupDrawTarget(mDT)->GetTransform();
  bool invertible = clone.Invert();
  // mSourceRect is in filter space. The filter outputs from mSourceRect need
  // to be drawn at mDestPoint in user space.
  Rect userSpaceSource = Rect(mDestPoint, mSourceRect.Size());
  if (invertible) {
    // Try to reduce the source rect so that it's not much bigger
    // than the draw target. The result is not minimal. Examples
    // are left as an exercise for the reader.
    auto destRect = IntRectToRect(baseRect);
    Rect userSpaceBounds = clone.TransformBounds(destRect);
    userSpaceSource = userSpaceSource.Intersect(userSpaceBounds);
  }

  // Compute how much we moved the top-left of the source rect by, and use that
  // to compute the new dest point, and move our intersected source rect back
  // into the (new) filter space.
  Point shift = userSpaceSource.TopLeft() - mDestPoint;
  Rect filterSpaceSource =
      Rect(mSourceRect.TopLeft() + shift, userSpaceSource.Size());

  baseRect = RoundedOut(filterSpaceSource);
  IntRect transformedRect =
      aTranslator->LookupFilterNode(mFilter)->MapRectToSource(
          baseRect, maxRect, aTranslator->LookupFilterNode(mSource));
>>>>>>> upstream-releases

<<<<<<< HEAD
  RefPtr<DrawTarget> newDT =
      aTranslator->GetReferenceDrawTarget()->CreateSimilarDrawTarget(
          intersection.Size(), SurfaceFormat::A8);
  // It's overkill to use a TiledDrawTarget for a single tile
  // but it was the easiest way to get the offset handling working
  gfx::TileSet tileset;
  gfx::Tile tile;
  tile.mDrawTarget = newDT;
  tile.mTileOrigin = gfx::IntPoint(intersection.X(), intersection.Y());
  tileset.mTiles = &tile;
  tileset.mTileCount = 1;
  newDT = gfx::Factory::CreateTiledDrawTarget(tileset);
||||||| merged common ancestors
  RefPtr<DrawTarget> newDT = aTranslator->GetReferenceDrawTarget()->CreateSimilarDrawTarget(intersection.Size(), SurfaceFormat::A8);
  // It's overkill to use a TiledDrawTarget for a single tile
  // but it was the easiest way to get the offset handling working
  gfx::TileSet tileset;
  gfx::Tile tile;
  tile.mDrawTarget = newDT;
  tile.mTileOrigin = gfx::IntPoint(intersection.X(), intersection.Y());
  tileset.mTiles = &tile;
  tileset.mTileCount = 1;
  newDT = gfx::Factory::CreateTiledDrawTarget(tileset);
=======
  // Intersect with maxRect to make sure we didn't end up with something bigger
  transformedRect = transformedRect.Intersect(maxRect);

  // If we end up with an empty rect make it 1x1 so that things don't break.
  if (transformedRect.IsEmpty()) {
    transformedRect = IntRect(0, 0, 1, 1);
  }

  RefPtr<DrawTarget> newDT =
      aTranslator->GetReferenceDrawTarget()->CreateSimilarDrawTarget(
          transformedRect.Size(), mFormat);
  newDT =
      gfx::Factory::CreateOffsetDrawTarget(newDT, transformedRect.TopLeft());
>>>>>>> upstream-releases

  // If we couldn't create a DrawTarget this will probably cause us to crash
  // with nullptr later in the playback, so return false to abort.
  if (!newDT) {
    return false;
  }

  aTranslator->AddDrawTarget(mRefPtr, newDT);
  return true;
}

<<<<<<< HEAD
template <class S>
void RecordedCreateClippedDrawTarget::Record(S &aStream) const {
||||||| merged common ancestors
template<class S>
void
RecordedCreateClippedDrawTarget::Record(S &aStream) const
{
=======
inline bool RecordedCreateClippedDrawTarget::PlayEvent(
    Translator* aTranslator) const {
  RefPtr<DrawTarget> newDT =
      aTranslator->GetReferenceDrawTarget()->CreateClippedDrawTarget(mBounds,
                                                                     mFormat);

  // If we couldn't create a DrawTarget this will probably cause us to crash
  // with nullptr later in the playback, so return false to abort.
  if (!newDT) {
    return false;
  }

  aTranslator->AddDrawTarget(mRefPtr, newDT);
  return true;
}

template <class S>
void RecordedCreateClippedDrawTarget::Record(S& aStream) const {
  WriteElement(aStream, mRefPtr);
  WriteElement(aStream, mBounds);
  WriteElement(aStream, mFormat);
}

template <class S>
RecordedCreateClippedDrawTarget::RecordedCreateClippedDrawTarget(S& aStream)
    : RecordedEventDerived(CREATECLIPPEDDRAWTARGET) {
  ReadElement(aStream, mRefPtr);
  ReadElement(aStream, mBounds);
  ReadElementConstrained(aStream, mFormat, SurfaceFormat::A8R8G8B8_UINT32,
                         SurfaceFormat::UNKNOWN);
}

inline void RecordedCreateClippedDrawTarget::OutputSimpleEventInfo(
    std::stringstream& aStringStream) const {
  aStringStream << "[" << mRefPtr << "] CreateClippedDrawTarget ()";
}

template <class S>
void RecordedCreateDrawTargetForFilter::Record(S& aStream) const {
  RecordedDrawingEvent::Record(aStream);
>>>>>>> upstream-releases
  WriteElement(aStream, mRefPtr);
  WriteElement(aStream, mMaxSize);
  WriteElement(aStream, mFormat);
  WriteElement(aStream, mFilter);
  WriteElement(aStream, mSource);
  WriteElement(aStream, mSourceRect);
  WriteElement(aStream, mDestPoint);
}

<<<<<<< HEAD
template <class S>
RecordedCreateClippedDrawTarget::RecordedCreateClippedDrawTarget(S &aStream)
    : RecordedEventDerived(CREATECLIPPEDDRAWTARGET) {
||||||| merged common ancestors
template<class S>
RecordedCreateClippedDrawTarget::RecordedCreateClippedDrawTarget(S &aStream)
  : RecordedEventDerived(CREATECLIPPEDDRAWTARGET)
{
=======
template <class S>
RecordedCreateDrawTargetForFilter::RecordedCreateDrawTargetForFilter(S& aStream)
    : RecordedDrawingEvent(CREATEDRAWTARGETFORFILTER, aStream) {
>>>>>>> upstream-releases
  ReadElement(aStream, mRefPtr);
  ReadElement(aStream, mMaxSize);
  ReadElementConstrained(aStream, mFormat, SurfaceFormat::A8R8G8B8_UINT32,
                         SurfaceFormat::UNKNOWN);
  ReadElement(aStream, mFilter);
  ReadElement(aStream, mSource);
  ReadElement(aStream, mSourceRect);
  ReadElement(aStream, mDestPoint);
}

<<<<<<< HEAD
inline void RecordedCreateClippedDrawTarget::OutputSimpleEventInfo(
    std::stringstream &aStringStream) const {
  aStringStream << "[" << mRefPtr << "] CreateClippedDrawTarget ()";
||||||| merged common ancestors
inline void
RecordedCreateClippedDrawTarget::OutputSimpleEventInfo(std::stringstream &aStringStream) const
{
  aStringStream << "[" << mRefPtr << "] CreateClippedDrawTarget ()";
=======
inline void RecordedCreateDrawTargetForFilter::OutputSimpleEventInfo(
    std::stringstream& aStringStream) const {
  aStringStream << "[" << mRefPtr << "] CreateDrawTargetForFilter ()";
>>>>>>> upstream-releases
}

<<<<<<< HEAD
struct GenericPattern {
  GenericPattern(const PatternStorage &aStorage, Translator *aTranslator)
      : mPattern(nullptr), mTranslator(aTranslator) {
    mStorage = const_cast<PatternStorage *>(&aStorage);
||||||| merged common ancestors
struct GenericPattern
{
  GenericPattern(const PatternStorage &aStorage, Translator *aTranslator)
    : mPattern(nullptr), mTranslator(aTranslator)
  {
    mStorage = const_cast<PatternStorage*>(&aStorage);
=======
struct GenericPattern {
  GenericPattern(const PatternStorage& aStorage, Translator* aTranslator)
      : mPattern(nullptr), mTranslator(aTranslator) {
    mStorage = const_cast<PatternStorage*>(&aStorage);
>>>>>>> upstream-releases
  }

  ~GenericPattern() {
    if (mPattern) {
      mPattern->~Pattern();
    }
  }

<<<<<<< HEAD
  operator Pattern *() {
    switch (mStorage->mType) {
      case PatternType::COLOR:
        return new (mColPat) ColorPattern(
            reinterpret_cast<ColorPatternStorage *>(&mStorage->mStorage)
                ->mColor);
      case PatternType::SURFACE: {
        SurfacePatternStorage *storage =
            reinterpret_cast<SurfacePatternStorage *>(&mStorage->mStorage);
        mPattern = new (mSurfPat)
            SurfacePattern(mTranslator->LookupSourceSurface(storage->mSurface),
                           storage->mExtend, storage->mMatrix,
                           storage->mSamplingFilter, storage->mSamplingRect);
||||||| merged common ancestors
  operator Pattern*()
  {
    switch(mStorage->mType) {
    case PatternType::COLOR:
      return new (mColPat) ColorPattern(reinterpret_cast<ColorPatternStorage*>(&mStorage->mStorage)->mColor);
    case PatternType::SURFACE:
      {
        SurfacePatternStorage *storage = reinterpret_cast<SurfacePatternStorage*>(&mStorage->mStorage);
        mPattern =
          new (mSurfPat) SurfacePattern(mTranslator->LookupSourceSurface(storage->mSurface),
                                        storage->mExtend, storage->mMatrix,
                                        storage->mSamplingFilter,
                                        storage->mSamplingRect);
=======
  operator Pattern*() {
    switch (mStorage->mType) {
      case PatternType::COLOR:
        return new (mColPat) ColorPattern(
            reinterpret_cast<ColorPatternStorage*>(&mStorage->mStorage)
                ->mColor);
      case PatternType::SURFACE: {
        SurfacePatternStorage* storage =
            reinterpret_cast<SurfacePatternStorage*>(&mStorage->mStorage);
        mPattern = new (mSurfPat)
            SurfacePattern(mTranslator->LookupSourceSurface(storage->mSurface),
                           storage->mExtend, storage->mMatrix,
                           storage->mSamplingFilter, storage->mSamplingRect);
>>>>>>> upstream-releases
        return mPattern;
      }
<<<<<<< HEAD
      case PatternType::LINEAR_GRADIENT: {
        LinearGradientPatternStorage *storage =
            reinterpret_cast<LinearGradientPatternStorage *>(
                &mStorage->mStorage);
        mPattern = new (mLinGradPat) LinearGradientPattern(
            storage->mBegin, storage->mEnd,
            mTranslator->LookupGradientStops(storage->mStops),
            storage->mMatrix);
||||||| merged common ancestors
    case PatternType::LINEAR_GRADIENT:
      {
        LinearGradientPatternStorage *storage = reinterpret_cast<LinearGradientPatternStorage*>(&mStorage->mStorage);
        mPattern =
          new (mLinGradPat) LinearGradientPattern(storage->mBegin, storage->mEnd,
                                                  mTranslator->LookupGradientStops(storage->mStops),
                                                  storage->mMatrix);
=======
      case PatternType::LINEAR_GRADIENT: {
        LinearGradientPatternStorage* storage =
            reinterpret_cast<LinearGradientPatternStorage*>(
                &mStorage->mStorage);
        mPattern = new (mLinGradPat) LinearGradientPattern(
            storage->mBegin, storage->mEnd,
            mTranslator->LookupGradientStops(storage->mStops),
            storage->mMatrix);
>>>>>>> upstream-releases
        return mPattern;
      }
<<<<<<< HEAD
      case PatternType::RADIAL_GRADIENT: {
        RadialGradientPatternStorage *storage =
            reinterpret_cast<RadialGradientPatternStorage *>(
                &mStorage->mStorage);
        mPattern = new (mRadGradPat) RadialGradientPattern(
            storage->mCenter1, storage->mCenter2, storage->mRadius1,
            storage->mRadius2,
            mTranslator->LookupGradientStops(storage->mStops),
            storage->mMatrix);
||||||| merged common ancestors
    case PatternType::RADIAL_GRADIENT:
      {
        RadialGradientPatternStorage *storage = reinterpret_cast<RadialGradientPatternStorage*>(&mStorage->mStorage);
        mPattern =
          new (mRadGradPat) RadialGradientPattern(storage->mCenter1, storage->mCenter2,
                                                  storage->mRadius1, storage->mRadius2,
                                                  mTranslator->LookupGradientStops(storage->mStops),
                                                  storage->mMatrix);
=======
      case PatternType::RADIAL_GRADIENT: {
        RadialGradientPatternStorage* storage =
            reinterpret_cast<RadialGradientPatternStorage*>(
                &mStorage->mStorage);
        mPattern = new (mRadGradPat) RadialGradientPattern(
            storage->mCenter1, storage->mCenter2, storage->mRadius1,
            storage->mRadius2,
            mTranslator->LookupGradientStops(storage->mStops),
            storage->mMatrix);
>>>>>>> upstream-releases
        return mPattern;
      }
      default:
        return new (mColPat) ColorPattern(Color());
    }

    return mPattern;
  }

  union {
    char mColPat[sizeof(ColorPattern)];
    char mLinGradPat[sizeof(LinearGradientPattern)];
    char mRadGradPat[sizeof(RadialGradientPattern)];
    char mSurfPat[sizeof(SurfacePattern)];
  };

  PatternStorage* mStorage;
  Pattern* mPattern;
  Translator* mTranslator;
};

<<<<<<< HEAD
inline bool RecordedFillRect::PlayEvent(Translator *aTranslator) const {
  aTranslator->LookupDrawTarget(mDT)->FillRect(
      mRect, *GenericPattern(mPattern, aTranslator), mOptions);
||||||| merged common ancestors
inline bool
RecordedFillRect::PlayEvent(Translator *aTranslator) const
{
  aTranslator->LookupDrawTarget(mDT)->FillRect(mRect, *GenericPattern(mPattern, aTranslator), mOptions);
=======
inline bool RecordedFillRect::PlayEvent(Translator* aTranslator) const {
  aTranslator->LookupDrawTarget(mDT)->FillRect(
      mRect, *GenericPattern(mPattern, aTranslator), mOptions);
>>>>>>> upstream-releases
  return true;
}

<<<<<<< HEAD
template <class S>
void RecordedFillRect::Record(S &aStream) const {
||||||| merged common ancestors
template<class S>
void
RecordedFillRect::Record(S &aStream) const
{
=======
template <class S>
void RecordedFillRect::Record(S& aStream) const {
>>>>>>> upstream-releases
  RecordedDrawingEvent::Record(aStream);
  WriteElement(aStream, mRect);
  WriteElement(aStream, mOptions);
  RecordPatternData(aStream, mPattern);
}

<<<<<<< HEAD
template <class S>
RecordedFillRect::RecordedFillRect(S &aStream)
    : RecordedDrawingEvent(FILLRECT, aStream) {
||||||| merged common ancestors
template<class S>
RecordedFillRect::RecordedFillRect(S &aStream)
  : RecordedDrawingEvent(FILLRECT, aStream)
{
=======
template <class S>
RecordedFillRect::RecordedFillRect(S& aStream)
    : RecordedDrawingEvent(FILLRECT, aStream) {
>>>>>>> upstream-releases
  ReadElement(aStream, mRect);
  ReadDrawOptions(aStream, mOptions);
  ReadPatternData(aStream, mPattern);
}

<<<<<<< HEAD
inline void RecordedFillRect::OutputSimpleEventInfo(
    std::stringstream &aStringStream) const {
  aStringStream << "[" << mDT << "] FillRect (" << mRect.X() << ", "
                << mRect.Y() << " - " << mRect.Width() << " x "
                << mRect.Height() << ") ";
||||||| merged common ancestors
inline void
RecordedFillRect::OutputSimpleEventInfo(std::stringstream &aStringStream) const
{
  aStringStream << "[" << mDT << "] FillRect (" << mRect.X() << ", " << mRect.Y() << " - " << mRect.Width() << " x " << mRect.Height() << ") ";
=======
inline void RecordedFillRect::OutputSimpleEventInfo(
    std::stringstream& aStringStream) const {
  aStringStream << "[" << mDT << "] FillRect (" << mRect.X() << ", "
                << mRect.Y() << " - " << mRect.Width() << " x "
                << mRect.Height() << ") ";
>>>>>>> upstream-releases
  OutputSimplePatternInfo(mPattern, aStringStream);
}

<<<<<<< HEAD
inline bool RecordedStrokeRect::PlayEvent(Translator *aTranslator) const {
  aTranslator->LookupDrawTarget(mDT)->StrokeRect(
      mRect, *GenericPattern(mPattern, aTranslator), mStrokeOptions, mOptions);
||||||| merged common ancestors
inline bool
RecordedStrokeRect::PlayEvent(Translator *aTranslator) const
{
  aTranslator->LookupDrawTarget(mDT)->StrokeRect(mRect, *GenericPattern(mPattern, aTranslator), mStrokeOptions, mOptions);
=======
inline bool RecordedStrokeRect::PlayEvent(Translator* aTranslator) const {
  aTranslator->LookupDrawTarget(mDT)->StrokeRect(
      mRect, *GenericPattern(mPattern, aTranslator), mStrokeOptions, mOptions);
>>>>>>> upstream-releases
  return true;
}

<<<<<<< HEAD
template <class S>
void RecordedStrokeRect::Record(S &aStream) const {
||||||| merged common ancestors
template<class S>
void
RecordedStrokeRect::Record(S &aStream) const
{
=======
template <class S>
void RecordedStrokeRect::Record(S& aStream) const {
>>>>>>> upstream-releases
  RecordedDrawingEvent::Record(aStream);
  WriteElement(aStream, mRect);
  WriteElement(aStream, mOptions);
  RecordPatternData(aStream, mPattern);
  RecordStrokeOptions(aStream, mStrokeOptions);
}

<<<<<<< HEAD
template <class S>
RecordedStrokeRect::RecordedStrokeRect(S &aStream)
    : RecordedDrawingEvent(STROKERECT, aStream) {
||||||| merged common ancestors
template<class S>
RecordedStrokeRect::RecordedStrokeRect(S &aStream)
  : RecordedDrawingEvent(STROKERECT, aStream)
{
=======
template <class S>
RecordedStrokeRect::RecordedStrokeRect(S& aStream)
    : RecordedDrawingEvent(STROKERECT, aStream) {
>>>>>>> upstream-releases
  ReadElement(aStream, mRect);
  ReadDrawOptions(aStream, mOptions);
  ReadPatternData(aStream, mPattern);
  ReadStrokeOptions(aStream, mStrokeOptions);
}

<<<<<<< HEAD
inline void RecordedStrokeRect::OutputSimpleEventInfo(
    std::stringstream &aStringStream) const {
  aStringStream << "[" << mDT << "] StrokeRect (" << mRect.X() << ", "
                << mRect.Y() << " - " << mRect.Width() << " x "
                << mRect.Height()
||||||| merged common ancestors
inline void
RecordedStrokeRect::OutputSimpleEventInfo(std::stringstream &aStringStream) const
{
  aStringStream << "[" << mDT << "] StrokeRect (" << mRect.X() << ", " << mRect.Y() << " - " << mRect.Width() << " x " << mRect.Height()
=======
inline void RecordedStrokeRect::OutputSimpleEventInfo(
    std::stringstream& aStringStream) const {
  aStringStream << "[" << mDT << "] StrokeRect (" << mRect.X() << ", "
                << mRect.Y() << " - " << mRect.Width() << " x "
                << mRect.Height()
>>>>>>> upstream-releases
                << ") LineWidth: " << mStrokeOptions.mLineWidth << "px ";
  OutputSimplePatternInfo(mPattern, aStringStream);
}

<<<<<<< HEAD
inline bool RecordedStrokeLine::PlayEvent(Translator *aTranslator) const {
  aTranslator->LookupDrawTarget(mDT)->StrokeLine(
      mBegin, mEnd, *GenericPattern(mPattern, aTranslator), mStrokeOptions,
      mOptions);
||||||| merged common ancestors
inline bool
RecordedStrokeLine::PlayEvent(Translator *aTranslator) const
{
  aTranslator->LookupDrawTarget(mDT)->StrokeLine(mBegin, mEnd, *GenericPattern(mPattern, aTranslator), mStrokeOptions, mOptions);
=======
inline bool RecordedStrokeLine::PlayEvent(Translator* aTranslator) const {
  aTranslator->LookupDrawTarget(mDT)->StrokeLine(
      mBegin, mEnd, *GenericPattern(mPattern, aTranslator), mStrokeOptions,
      mOptions);
>>>>>>> upstream-releases
  return true;
}

<<<<<<< HEAD
template <class S>
void RecordedStrokeLine::Record(S &aStream) const {
||||||| merged common ancestors
template<class S>
void
RecordedStrokeLine::Record(S &aStream) const
{
=======
template <class S>
void RecordedStrokeLine::Record(S& aStream) const {
>>>>>>> upstream-releases
  RecordedDrawingEvent::Record(aStream);
  WriteElement(aStream, mBegin);
  WriteElement(aStream, mEnd);
  WriteElement(aStream, mOptions);
  RecordPatternData(aStream, mPattern);
  RecordStrokeOptions(aStream, mStrokeOptions);
}

<<<<<<< HEAD
template <class S>
RecordedStrokeLine::RecordedStrokeLine(S &aStream)
    : RecordedDrawingEvent(STROKELINE, aStream) {
||||||| merged common ancestors
template<class S>
RecordedStrokeLine::RecordedStrokeLine(S &aStream)
  : RecordedDrawingEvent(STROKELINE, aStream)
{
=======
template <class S>
RecordedStrokeLine::RecordedStrokeLine(S& aStream)
    : RecordedDrawingEvent(STROKELINE, aStream) {
>>>>>>> upstream-releases
  ReadElement(aStream, mBegin);
  ReadElement(aStream, mEnd);
  ReadDrawOptions(aStream, mOptions);
  ReadPatternData(aStream, mPattern);
  ReadStrokeOptions(aStream, mStrokeOptions);
}

<<<<<<< HEAD
inline void RecordedStrokeLine::OutputSimpleEventInfo(
    std::stringstream &aStringStream) const {
  aStringStream << "[" << mDT << "] StrokeLine (" << mBegin.x << ", "
                << mBegin.y << " - " << mEnd.x << ", " << mEnd.y
||||||| merged common ancestors
inline void
RecordedStrokeLine::OutputSimpleEventInfo(std::stringstream &aStringStream) const
{
  aStringStream << "[" << mDT << "] StrokeLine (" << mBegin.x << ", " << mBegin.y << " - " << mEnd.x << ", " << mEnd.y
=======
inline void RecordedStrokeLine::OutputSimpleEventInfo(
    std::stringstream& aStringStream) const {
  aStringStream << "[" << mDT << "] StrokeLine (" << mBegin.x << ", "
                << mBegin.y << " - " << mEnd.x << ", " << mEnd.y
>>>>>>> upstream-releases
                << ") LineWidth: " << mStrokeOptions.mLineWidth << "px ";
  OutputSimplePatternInfo(mPattern, aStringStream);
}

<<<<<<< HEAD
inline bool RecordedFill::PlayEvent(Translator *aTranslator) const {
  aTranslator->LookupDrawTarget(mDT)->Fill(
      aTranslator->LookupPath(mPath), *GenericPattern(mPattern, aTranslator),
      mOptions);
||||||| merged common ancestors
inline bool
RecordedFill::PlayEvent(Translator *aTranslator) const
{
  aTranslator->LookupDrawTarget(mDT)->Fill(aTranslator->LookupPath(mPath), *GenericPattern(mPattern, aTranslator), mOptions);
=======
inline bool RecordedFill::PlayEvent(Translator* aTranslator) const {
  aTranslator->LookupDrawTarget(mDT)->Fill(
      aTranslator->LookupPath(mPath), *GenericPattern(mPattern, aTranslator),
      mOptions);
>>>>>>> upstream-releases
  return true;
}

<<<<<<< HEAD
template <class S>
RecordedFill::RecordedFill(S &aStream) : RecordedDrawingEvent(FILL, aStream) {
||||||| merged common ancestors
template<class S>
RecordedFill::RecordedFill(S &aStream)
  : RecordedDrawingEvent(FILL, aStream)
{
=======
template <class S>
RecordedFill::RecordedFill(S& aStream) : RecordedDrawingEvent(FILL, aStream) {
>>>>>>> upstream-releases
  ReadElement(aStream, mPath);
  ReadDrawOptions(aStream, mOptions);
  ReadPatternData(aStream, mPattern);
}

<<<<<<< HEAD
template <class S>
void RecordedFill::Record(S &aStream) const {
||||||| merged common ancestors
template<class S>
void
RecordedFill::Record(S &aStream) const
{
=======
template <class S>
void RecordedFill::Record(S& aStream) const {
>>>>>>> upstream-releases
  RecordedDrawingEvent::Record(aStream);
  WriteElement(aStream, mPath);
  WriteElement(aStream, mOptions);
  RecordPatternData(aStream, mPattern);
}

<<<<<<< HEAD
inline void RecordedFill::OutputSimpleEventInfo(
    std::stringstream &aStringStream) const {
||||||| merged common ancestors
inline void
RecordedFill::OutputSimpleEventInfo(std::stringstream &aStringStream) const
{
=======
inline void RecordedFill::OutputSimpleEventInfo(
    std::stringstream& aStringStream) const {
>>>>>>> upstream-releases
  aStringStream << "[" << mDT << "] Fill (" << mPath << ") ";
  OutputSimplePatternInfo(mPattern, aStringStream);
}

inline RecordedFillGlyphs::~RecordedFillGlyphs() { delete[] mGlyphs; }

<<<<<<< HEAD
inline bool RecordedFillGlyphs::PlayEvent(Translator *aTranslator) const {
||||||| merged common ancestors
inline bool
RecordedFillGlyphs::PlayEvent(Translator *aTranslator) const
{
=======
inline bool RecordedFillGlyphs::PlayEvent(Translator* aTranslator) const {
>>>>>>> upstream-releases
  GlyphBuffer buffer;
  buffer.mGlyphs = mGlyphs;
  buffer.mNumGlyphs = mNumGlyphs;
  aTranslator->LookupDrawTarget(mDT)->FillGlyphs(
      aTranslator->LookupScaledFont(mScaledFont), buffer,
      *GenericPattern(mPattern, aTranslator), mOptions);
  return true;
}

<<<<<<< HEAD
template <class S>
RecordedFillGlyphs::RecordedFillGlyphs(S &aStream)
    : RecordedDrawingEvent(FILLGLYPHS, aStream) {
||||||| merged common ancestors
template<class S>
RecordedFillGlyphs::RecordedFillGlyphs(S &aStream)
  : RecordedDrawingEvent(FILLGLYPHS, aStream)
{
=======
template <class S>
RecordedFillGlyphs::RecordedFillGlyphs(S& aStream)
    : RecordedDrawingEvent(FILLGLYPHS, aStream) {
>>>>>>> upstream-releases
  ReadElement(aStream, mScaledFont);
  ReadDrawOptions(aStream, mOptions);
  ReadPatternData(aStream, mPattern);
  ReadElement(aStream, mNumGlyphs);
  mGlyphs = new Glyph[mNumGlyphs];
  aStream.read((char *)mGlyphs, sizeof(Glyph) * mNumGlyphs);
}

<<<<<<< HEAD
template <class S>
void RecordedFillGlyphs::Record(S &aStream) const {
||||||| merged common ancestors
template<class S>
void
RecordedFillGlyphs::Record(S &aStream) const
{
=======
template <class S>
void RecordedFillGlyphs::Record(S& aStream) const {
>>>>>>> upstream-releases
  RecordedDrawingEvent::Record(aStream);
  WriteElement(aStream, mScaledFont);
  WriteElement(aStream, mOptions);
  RecordPatternData(aStream, mPattern);
  WriteElement(aStream, mNumGlyphs);
  aStream.write((char *)mGlyphs, sizeof(Glyph) * mNumGlyphs);
}

<<<<<<< HEAD
inline void RecordedFillGlyphs::OutputSimpleEventInfo(
    std::stringstream &aStringStream) const {
||||||| merged common ancestors
inline void
RecordedFillGlyphs::OutputSimpleEventInfo(std::stringstream &aStringStream) const
{
=======
inline void RecordedFillGlyphs::OutputSimpleEventInfo(
    std::stringstream& aStringStream) const {
>>>>>>> upstream-releases
  aStringStream << "[" << mDT << "] FillGlyphs (" << mScaledFont << ") ";
  OutputSimplePatternInfo(mPattern, aStringStream);
}

<<<<<<< HEAD
inline bool RecordedMask::PlayEvent(Translator *aTranslator) const {
  aTranslator->LookupDrawTarget(mDT)->Mask(
      *GenericPattern(mSource, aTranslator),
      *GenericPattern(mMask, aTranslator), mOptions);
||||||| merged common ancestors
inline bool
RecordedMask::PlayEvent(Translator *aTranslator) const
{
  aTranslator->LookupDrawTarget(mDT)->Mask(*GenericPattern(mSource, aTranslator), *GenericPattern(mMask, aTranslator), mOptions);
=======
inline bool RecordedMask::PlayEvent(Translator* aTranslator) const {
  aTranslator->LookupDrawTarget(mDT)->Mask(
      *GenericPattern(mSource, aTranslator),
      *GenericPattern(mMask, aTranslator), mOptions);
>>>>>>> upstream-releases
  return true;
}

<<<<<<< HEAD
template <class S>
RecordedMask::RecordedMask(S &aStream) : RecordedDrawingEvent(MASK, aStream) {
  ReadElement(aStream, mOptions);
||||||| merged common ancestors
template<class S>
RecordedMask::RecordedMask(S &aStream)
  : RecordedDrawingEvent(MASK, aStream)
{
  ReadElement(aStream, mOptions);
=======
template <class S>
RecordedMask::RecordedMask(S& aStream) : RecordedDrawingEvent(MASK, aStream) {
  ReadDrawOptions(aStream, mOptions);
>>>>>>> upstream-releases
  ReadPatternData(aStream, mSource);
  ReadPatternData(aStream, mMask);
}

<<<<<<< HEAD
template <class S>
void RecordedMask::Record(S &aStream) const {
||||||| merged common ancestors
template<class S>
void
RecordedMask::Record(S &aStream) const
{
=======
template <class S>
void RecordedMask::Record(S& aStream) const {
>>>>>>> upstream-releases
  RecordedDrawingEvent::Record(aStream);
  WriteElement(aStream, mOptions);
  RecordPatternData(aStream, mSource);
  RecordPatternData(aStream, mMask);
}

<<<<<<< HEAD
inline void RecordedMask::OutputSimpleEventInfo(
    std::stringstream &aStringStream) const {
||||||| merged common ancestors
inline void
RecordedMask::OutputSimpleEventInfo(std::stringstream &aStringStream) const
{
=======
inline void RecordedMask::OutputSimpleEventInfo(
    std::stringstream& aStringStream) const {
>>>>>>> upstream-releases
  aStringStream << "[" << mDT << "] Mask (Source: ";
  OutputSimplePatternInfo(mSource, aStringStream);
  aStringStream << " Mask: ";
  OutputSimplePatternInfo(mMask, aStringStream);
}

<<<<<<< HEAD
inline bool RecordedStroke::PlayEvent(Translator *aTranslator) const {
  aTranslator->LookupDrawTarget(mDT)->Stroke(
      aTranslator->LookupPath(mPath), *GenericPattern(mPattern, aTranslator),
      mStrokeOptions, mOptions);
||||||| merged common ancestors
inline bool
RecordedStroke::PlayEvent(Translator *aTranslator) const
{
  aTranslator->LookupDrawTarget(mDT)->Stroke(aTranslator->LookupPath(mPath), *GenericPattern(mPattern, aTranslator), mStrokeOptions, mOptions);
=======
inline bool RecordedStroke::PlayEvent(Translator* aTranslator) const {
  aTranslator->LookupDrawTarget(mDT)->Stroke(
      aTranslator->LookupPath(mPath), *GenericPattern(mPattern, aTranslator),
      mStrokeOptions, mOptions);
>>>>>>> upstream-releases
  return true;
}

<<<<<<< HEAD
template <class S>
void RecordedStroke::Record(S &aStream) const {
||||||| merged common ancestors
template<class S>
void
RecordedStroke::Record(S &aStream) const
{
=======
template <class S>
void RecordedStroke::Record(S& aStream) const {
>>>>>>> upstream-releases
  RecordedDrawingEvent::Record(aStream);
  WriteElement(aStream, mPath);
  WriteElement(aStream, mOptions);
  RecordPatternData(aStream, mPattern);
  RecordStrokeOptions(aStream, mStrokeOptions);
}

<<<<<<< HEAD
template <class S>
RecordedStroke::RecordedStroke(S &aStream)
    : RecordedDrawingEvent(STROKE, aStream) {
||||||| merged common ancestors
template<class S>
RecordedStroke::RecordedStroke(S &aStream)
  : RecordedDrawingEvent(STROKE, aStream)
{
=======
template <class S>
RecordedStroke::RecordedStroke(S& aStream)
    : RecordedDrawingEvent(STROKE, aStream) {
>>>>>>> upstream-releases
  ReadElement(aStream, mPath);
  ReadDrawOptions(aStream, mOptions);
  ReadPatternData(aStream, mPattern);
  ReadStrokeOptions(aStream, mStrokeOptions);
}

<<<<<<< HEAD
inline void RecordedStroke::OutputSimpleEventInfo(
    std::stringstream &aStringStream) const {
  aStringStream << "[" << mDT << "] Stroke (" << mPath
                << ") LineWidth: " << mStrokeOptions.mLineWidth << "px ";
||||||| merged common ancestors
inline void
RecordedStroke::OutputSimpleEventInfo(std::stringstream &aStringStream) const
{
  aStringStream << "[" << mDT << "] Stroke ("<< mPath << ") LineWidth: " << mStrokeOptions.mLineWidth << "px ";
=======
inline void RecordedStroke::OutputSimpleEventInfo(
    std::stringstream& aStringStream) const {
  aStringStream << "[" << mDT << "] Stroke (" << mPath
                << ") LineWidth: " << mStrokeOptions.mLineWidth << "px ";
>>>>>>> upstream-releases
  OutputSimplePatternInfo(mPattern, aStringStream);
}

<<<<<<< HEAD
inline bool RecordedClearRect::PlayEvent(Translator *aTranslator) const {
||||||| merged common ancestors
inline bool
RecordedClearRect::PlayEvent(Translator *aTranslator) const
{
=======
inline bool RecordedClearRect::PlayEvent(Translator* aTranslator) const {
>>>>>>> upstream-releases
  aTranslator->LookupDrawTarget(mDT)->ClearRect(mRect);
  return true;
}

<<<<<<< HEAD
template <class S>
void RecordedClearRect::Record(S &aStream) const {
||||||| merged common ancestors
template<class S>
void
RecordedClearRect::Record(S &aStream) const
{
=======
template <class S>
void RecordedClearRect::Record(S& aStream) const {
>>>>>>> upstream-releases
  RecordedDrawingEvent::Record(aStream);
  WriteElement(aStream, mRect);
}

<<<<<<< HEAD
template <class S>
RecordedClearRect::RecordedClearRect(S &aStream)
    : RecordedDrawingEvent(CLEARRECT, aStream) {
  ReadElement(aStream, mRect);
||||||| merged common ancestors
template<class S>
RecordedClearRect::RecordedClearRect(S &aStream)
  : RecordedDrawingEvent(CLEARRECT, aStream)
{
    ReadElement(aStream, mRect);
=======
template <class S>
RecordedClearRect::RecordedClearRect(S& aStream)
    : RecordedDrawingEvent(CLEARRECT, aStream) {
  ReadElement(aStream, mRect);
>>>>>>> upstream-releases
}

<<<<<<< HEAD
inline void RecordedClearRect::OutputSimpleEventInfo(
    std::stringstream &aStringStream) const {
  aStringStream << "[" << mDT << "] ClearRect (" << mRect.X() << ", "
                << mRect.Y() << " - " << mRect.Width() << " x "
                << mRect.Height() << ") ";
||||||| merged common ancestors
inline void
RecordedClearRect::OutputSimpleEventInfo(std::stringstream &aStringStream) const
{
  aStringStream << "[" << mDT<< "] ClearRect (" << mRect.X() << ", " << mRect.Y() << " - " << mRect.Width() << " x " << mRect.Height() << ") ";
=======
inline void RecordedClearRect::OutputSimpleEventInfo(
    std::stringstream& aStringStream) const {
  aStringStream << "[" << mDT << "] ClearRect (" << mRect.X() << ", "
                << mRect.Y() << " - " << mRect.Width() << " x "
                << mRect.Height() << ") ";
>>>>>>> upstream-releases
}

<<<<<<< HEAD
inline bool RecordedCopySurface::PlayEvent(Translator *aTranslator) const {
  aTranslator->LookupDrawTarget(mDT)->CopySurface(
      aTranslator->LookupSourceSurface(mSourceSurface), mSourceRect, mDest);
||||||| merged common ancestors
inline bool
RecordedCopySurface::PlayEvent(Translator *aTranslator) const
{
	aTranslator->LookupDrawTarget(mDT)->CopySurface(aTranslator->LookupSourceSurface(mSourceSurface),
                                                  mSourceRect, mDest);
=======
inline bool RecordedCopySurface::PlayEvent(Translator* aTranslator) const {
  aTranslator->LookupDrawTarget(mDT)->CopySurface(
      aTranslator->LookupSourceSurface(mSourceSurface), mSourceRect, mDest);
>>>>>>> upstream-releases
  return true;
}

<<<<<<< HEAD
template <class S>
void RecordedCopySurface::Record(S &aStream) const {
||||||| merged common ancestors
template<class S>
void
RecordedCopySurface::Record(S &aStream) const
{
=======
template <class S>
void RecordedCopySurface::Record(S& aStream) const {
>>>>>>> upstream-releases
  RecordedDrawingEvent::Record(aStream);
  WriteElement(aStream, mSourceSurface);
  WriteElement(aStream, mSourceRect);
  WriteElement(aStream, mDest);
}

<<<<<<< HEAD
template <class S>
RecordedCopySurface::RecordedCopySurface(S &aStream)
    : RecordedDrawingEvent(COPYSURFACE, aStream) {
||||||| merged common ancestors
template<class S>
RecordedCopySurface::RecordedCopySurface(S &aStream)
  : RecordedDrawingEvent(COPYSURFACE, aStream)
{
=======
template <class S>
RecordedCopySurface::RecordedCopySurface(S& aStream)
    : RecordedDrawingEvent(COPYSURFACE, aStream) {
>>>>>>> upstream-releases
  ReadElement(aStream, mSourceSurface);
  ReadElement(aStream, mSourceRect);
  ReadElement(aStream, mDest);
}

<<<<<<< HEAD
inline void RecordedCopySurface::OutputSimpleEventInfo(
    std::stringstream &aStringStream) const {
  aStringStream << "[" << mDT << "] CopySurface (" << mSourceSurface << ")";
||||||| merged common ancestors
inline void
RecordedCopySurface::OutputSimpleEventInfo(std::stringstream &aStringStream) const
{
  aStringStream << "[" << mDT<< "] CopySurface (" << mSourceSurface << ")";
=======
inline void RecordedCopySurface::OutputSimpleEventInfo(
    std::stringstream& aStringStream) const {
  aStringStream << "[" << mDT << "] CopySurface (" << mSourceSurface << ")";
>>>>>>> upstream-releases
}

<<<<<<< HEAD
inline bool RecordedPushClip::PlayEvent(Translator *aTranslator) const {
||||||| merged common ancestors
inline bool
RecordedPushClip::PlayEvent(Translator *aTranslator) const
{
=======
inline bool RecordedPushClip::PlayEvent(Translator* aTranslator) const {
>>>>>>> upstream-releases
  aTranslator->LookupDrawTarget(mDT)->PushClip(aTranslator->LookupPath(mPath));
  return true;
}

<<<<<<< HEAD
template <class S>
void RecordedPushClip::Record(S &aStream) const {
||||||| merged common ancestors
template<class S>
void
RecordedPushClip::Record(S &aStream) const
{
=======
template <class S>
void RecordedPushClip::Record(S& aStream) const {
>>>>>>> upstream-releases
  RecordedDrawingEvent::Record(aStream);
  WriteElement(aStream, mPath);
}

<<<<<<< HEAD
template <class S>
RecordedPushClip::RecordedPushClip(S &aStream)
    : RecordedDrawingEvent(PUSHCLIP, aStream) {
||||||| merged common ancestors
template<class S>
RecordedPushClip::RecordedPushClip(S &aStream)
  : RecordedDrawingEvent(PUSHCLIP, aStream)
{
=======
template <class S>
RecordedPushClip::RecordedPushClip(S& aStream)
    : RecordedDrawingEvent(PUSHCLIP, aStream) {
>>>>>>> upstream-releases
  ReadElement(aStream, mPath);
}

<<<<<<< HEAD
inline void RecordedPushClip::OutputSimpleEventInfo(
    std::stringstream &aStringStream) const {
||||||| merged common ancestors
inline void
RecordedPushClip::OutputSimpleEventInfo(std::stringstream &aStringStream) const
{
=======
inline void RecordedPushClip::OutputSimpleEventInfo(
    std::stringstream& aStringStream) const {
>>>>>>> upstream-releases
  aStringStream << "[" << mDT << "] PushClip (" << mPath << ") ";
}

<<<<<<< HEAD
inline bool RecordedPushClipRect::PlayEvent(Translator *aTranslator) const {
||||||| merged common ancestors
inline bool
RecordedPushClipRect::PlayEvent(Translator *aTranslator) const
{
=======
inline bool RecordedPushClipRect::PlayEvent(Translator* aTranslator) const {
>>>>>>> upstream-releases
  aTranslator->LookupDrawTarget(mDT)->PushClipRect(mRect);
  return true;
}

<<<<<<< HEAD
template <class S>
void RecordedPushClipRect::Record(S &aStream) const {
||||||| merged common ancestors
template<class S>
void
RecordedPushClipRect::Record(S &aStream) const
{
=======
template <class S>
void RecordedPushClipRect::Record(S& aStream) const {
>>>>>>> upstream-releases
  RecordedDrawingEvent::Record(aStream);
  WriteElement(aStream, mRect);
}

<<<<<<< HEAD
template <class S>
RecordedPushClipRect::RecordedPushClipRect(S &aStream)
    : RecordedDrawingEvent(PUSHCLIPRECT, aStream) {
||||||| merged common ancestors
template<class S>
RecordedPushClipRect::RecordedPushClipRect(S &aStream)
  : RecordedDrawingEvent(PUSHCLIPRECT, aStream)
{
=======
template <class S>
RecordedPushClipRect::RecordedPushClipRect(S& aStream)
    : RecordedDrawingEvent(PUSHCLIPRECT, aStream) {
>>>>>>> upstream-releases
  ReadElement(aStream, mRect);
}

<<<<<<< HEAD
inline void RecordedPushClipRect::OutputSimpleEventInfo(
    std::stringstream &aStringStream) const {
  aStringStream << "[" << mDT << "] PushClipRect (" << mRect.X() << ", "
                << mRect.Y() << " - " << mRect.Width() << " x "
                << mRect.Height() << ") ";
||||||| merged common ancestors
inline void
RecordedPushClipRect::OutputSimpleEventInfo(std::stringstream &aStringStream) const
{
  aStringStream << "[" << mDT << "] PushClipRect (" << mRect.X() << ", " << mRect.Y() << " - " << mRect.Width() << " x " << mRect.Height() << ") ";
=======
inline void RecordedPushClipRect::OutputSimpleEventInfo(
    std::stringstream& aStringStream) const {
  aStringStream << "[" << mDT << "] PushClipRect (" << mRect.X() << ", "
                << mRect.Y() << " - " << mRect.Width() << " x "
                << mRect.Height() << ") ";
>>>>>>> upstream-releases
}

<<<<<<< HEAD
inline bool RecordedPopClip::PlayEvent(Translator *aTranslator) const {
||||||| merged common ancestors
inline bool
RecordedPopClip::PlayEvent(Translator *aTranslator) const
{
=======
inline bool RecordedPopClip::PlayEvent(Translator* aTranslator) const {
>>>>>>> upstream-releases
  aTranslator->LookupDrawTarget(mDT)->PopClip();
  return true;
}

<<<<<<< HEAD
template <class S>
void RecordedPopClip::Record(S &aStream) const {
||||||| merged common ancestors
template<class S>
void
RecordedPopClip::Record(S &aStream) const
{
=======
template <class S>
void RecordedPopClip::Record(S& aStream) const {
>>>>>>> upstream-releases
  RecordedDrawingEvent::Record(aStream);
}

<<<<<<< HEAD
template <class S>
RecordedPopClip::RecordedPopClip(S &aStream)
    : RecordedDrawingEvent(POPCLIP, aStream) {}
||||||| merged common ancestors
template<class S>
RecordedPopClip::RecordedPopClip(S &aStream)
  : RecordedDrawingEvent(POPCLIP, aStream)
{
}
=======
template <class S>
RecordedPopClip::RecordedPopClip(S& aStream)
    : RecordedDrawingEvent(POPCLIP, aStream) {}
>>>>>>> upstream-releases

<<<<<<< HEAD
inline void RecordedPopClip::OutputSimpleEventInfo(
    std::stringstream &aStringStream) const {
||||||| merged common ancestors
inline void
RecordedPopClip::OutputSimpleEventInfo(std::stringstream &aStringStream) const
{
=======
inline void RecordedPopClip::OutputSimpleEventInfo(
    std::stringstream& aStringStream) const {
>>>>>>> upstream-releases
  aStringStream << "[" << mDT << "] PopClip";
}

<<<<<<< HEAD
inline bool RecordedPushLayer::PlayEvent(Translator *aTranslator) const {
  SourceSurface *mask =
      mMask ? aTranslator->LookupSourceSurface(mMask) : nullptr;
  aTranslator->LookupDrawTarget(mDT)->PushLayer(
      mOpaque, mOpacity, mask, mMaskTransform, mBounds, mCopyBackground);
||||||| merged common ancestors
inline bool
RecordedPushLayer::PlayEvent(Translator *aTranslator) const
{
  SourceSurface* mask = mMask ? aTranslator->LookupSourceSurface(mMask)
                              : nullptr;
  aTranslator->LookupDrawTarget(mDT)->
    PushLayer(mOpaque, mOpacity, mask, mMaskTransform, mBounds, mCopyBackground);
=======
inline bool RecordedPushLayer::PlayEvent(Translator* aTranslator) const {
  SourceSurface* mask =
      mMask ? aTranslator->LookupSourceSurface(mMask) : nullptr;
  aTranslator->LookupDrawTarget(mDT)->PushLayer(
      mOpaque, mOpacity, mask, mMaskTransform, mBounds, mCopyBackground);
>>>>>>> upstream-releases
  return true;
}

<<<<<<< HEAD
template <class S>
void RecordedPushLayer::Record(S &aStream) const {
||||||| merged common ancestors
template<class S>
void
RecordedPushLayer::Record(S &aStream) const
{
=======
template <class S>
void RecordedPushLayer::Record(S& aStream) const {
>>>>>>> upstream-releases
  RecordedDrawingEvent::Record(aStream);
  WriteElement(aStream, mOpaque);
  WriteElement(aStream, mOpacity);
  WriteElement(aStream, mMask);
  WriteElement(aStream, mMaskTransform);
  WriteElement(aStream, mBounds);
  WriteElement(aStream, mCopyBackground);
}

<<<<<<< HEAD
template <class S>
RecordedPushLayer::RecordedPushLayer(S &aStream)
    : RecordedDrawingEvent(PUSHLAYER, aStream) {
||||||| merged common ancestors
template<class S>
RecordedPushLayer::RecordedPushLayer(S &aStream)
  : RecordedDrawingEvent(PUSHLAYER, aStream)
{
=======
template <class S>
RecordedPushLayer::RecordedPushLayer(S& aStream)
    : RecordedDrawingEvent(PUSHLAYER, aStream) {
>>>>>>> upstream-releases
  ReadElement(aStream, mOpaque);
  ReadElement(aStream, mOpacity);
  ReadElement(aStream, mMask);
  ReadElement(aStream, mMaskTransform);
  ReadElement(aStream, mBounds);
  ReadElement(aStream, mCopyBackground);
}

<<<<<<< HEAD
inline void RecordedPushLayer::OutputSimpleEventInfo(
    std::stringstream &aStringStream) const {
  aStringStream << "[" << mDT << "] PushPLayer (Opaque=" << mOpaque
                << ", Opacity=" << mOpacity << ", Mask Ref=" << mMask << ") ";
||||||| merged common ancestors
inline void
RecordedPushLayer::OutputSimpleEventInfo(std::stringstream &aStringStream) const
{
  aStringStream << "[" << mDT << "] PushPLayer (Opaque=" << mOpaque <<
    ", Opacity=" << mOpacity << ", Mask Ref=" << mMask << ") ";
=======
inline void RecordedPushLayer::OutputSimpleEventInfo(
    std::stringstream& aStringStream) const {
  aStringStream << "[" << mDT << "] PushPLayer (Opaque=" << mOpaque
                << ", Opacity=" << mOpacity << ", Mask Ref=" << mMask << ") ";
>>>>>>> upstream-releases
}

<<<<<<< HEAD
inline bool RecordedPushLayerWithBlend::PlayEvent(
    Translator *aTranslator) const {
  SourceSurface *mask =
      mMask ? aTranslator->LookupSourceSurface(mMask) : nullptr;
  aTranslator->LookupDrawTarget(mDT)->PushLayerWithBlend(
      mOpaque, mOpacity, mask, mMaskTransform, mBounds, mCopyBackground,
      mCompositionOp);
||||||| merged common ancestors
inline bool
RecordedPushLayerWithBlend::PlayEvent(Translator *aTranslator) const
{
  SourceSurface* mask = mMask ? aTranslator->LookupSourceSurface(mMask)
                              : nullptr;
  aTranslator->LookupDrawTarget(mDT)->
    PushLayerWithBlend(mOpaque, mOpacity, mask, mMaskTransform, mBounds, mCopyBackground, mCompositionOp);
=======
inline bool RecordedPushLayerWithBlend::PlayEvent(
    Translator* aTranslator) const {
  SourceSurface* mask =
      mMask ? aTranslator->LookupSourceSurface(mMask) : nullptr;
  aTranslator->LookupDrawTarget(mDT)->PushLayerWithBlend(
      mOpaque, mOpacity, mask, mMaskTransform, mBounds, mCopyBackground,
      mCompositionOp);
>>>>>>> upstream-releases
  return true;
}

<<<<<<< HEAD
template <class S>
void RecordedPushLayerWithBlend::Record(S &aStream) const {
||||||| merged common ancestors
template<class S>
void
RecordedPushLayerWithBlend::Record(S &aStream) const
{
=======
template <class S>
void RecordedPushLayerWithBlend::Record(S& aStream) const {
>>>>>>> upstream-releases
  RecordedDrawingEvent::Record(aStream);
  WriteElement(aStream, mOpaque);
  WriteElement(aStream, mOpacity);
  WriteElement(aStream, mMask);
  WriteElement(aStream, mMaskTransform);
  WriteElement(aStream, mBounds);
  WriteElement(aStream, mCopyBackground);
  WriteElement(aStream, mCompositionOp);
}

<<<<<<< HEAD
template <class S>
RecordedPushLayerWithBlend::RecordedPushLayerWithBlend(S &aStream)
    : RecordedDrawingEvent(PUSHLAYERWITHBLEND, aStream) {
||||||| merged common ancestors
template<class S>
RecordedPushLayerWithBlend::RecordedPushLayerWithBlend(S &aStream)
  : RecordedDrawingEvent(PUSHLAYERWITHBLEND, aStream)
{
=======
template <class S>
RecordedPushLayerWithBlend::RecordedPushLayerWithBlend(S& aStream)
    : RecordedDrawingEvent(PUSHLAYERWITHBLEND, aStream) {
>>>>>>> upstream-releases
  ReadElement(aStream, mOpaque);
  ReadElement(aStream, mOpacity);
  ReadElement(aStream, mMask);
  ReadElement(aStream, mMaskTransform);
  ReadElement(aStream, mBounds);
  ReadElement(aStream, mCopyBackground);
  ReadElementConstrained(aStream, mCompositionOp, CompositionOp::OP_OVER,
                         CompositionOp::OP_COUNT);
}

<<<<<<< HEAD
inline void RecordedPushLayerWithBlend::OutputSimpleEventInfo(
    std::stringstream &aStringStream) const {
  aStringStream << "[" << mDT << "] PushLayerWithBlend (Opaque=" << mOpaque
                << ", Opacity=" << mOpacity << ", Mask Ref=" << mMask << ") ";
||||||| merged common ancestors
inline void
RecordedPushLayerWithBlend::OutputSimpleEventInfo(std::stringstream &aStringStream) const
{
  aStringStream << "[" << mDT << "] PushLayerWithBlend (Opaque=" << mOpaque <<
    ", Opacity=" << mOpacity << ", Mask Ref=" << mMask << ") ";
=======
inline void RecordedPushLayerWithBlend::OutputSimpleEventInfo(
    std::stringstream& aStringStream) const {
  aStringStream << "[" << mDT << "] PushLayerWithBlend (Opaque=" << mOpaque
                << ", Opacity=" << mOpacity << ", Mask Ref=" << mMask << ") ";
>>>>>>> upstream-releases
}

<<<<<<< HEAD
inline bool RecordedPopLayer::PlayEvent(Translator *aTranslator) const {
||||||| merged common ancestors

inline bool
RecordedPopLayer::PlayEvent(Translator *aTranslator) const
{
=======
inline bool RecordedPopLayer::PlayEvent(Translator* aTranslator) const {
>>>>>>> upstream-releases
  aTranslator->LookupDrawTarget(mDT)->PopLayer();
  return true;
}

<<<<<<< HEAD
template <class S>
void RecordedPopLayer::Record(S &aStream) const {
||||||| merged common ancestors
template<class S>
void
RecordedPopLayer::Record(S &aStream) const
{
=======
template <class S>
void RecordedPopLayer::Record(S& aStream) const {
>>>>>>> upstream-releases
  RecordedDrawingEvent::Record(aStream);
}

<<<<<<< HEAD
template <class S>
RecordedPopLayer::RecordedPopLayer(S &aStream)
    : RecordedDrawingEvent(POPLAYER, aStream) {}
||||||| merged common ancestors
template<class S>
RecordedPopLayer::RecordedPopLayer(S &aStream)
  : RecordedDrawingEvent(POPLAYER, aStream)
{
}
=======
template <class S>
RecordedPopLayer::RecordedPopLayer(S& aStream)
    : RecordedDrawingEvent(POPLAYER, aStream) {}
>>>>>>> upstream-releases

<<<<<<< HEAD
inline void RecordedPopLayer::OutputSimpleEventInfo(
    std::stringstream &aStringStream) const {
||||||| merged common ancestors
inline void
RecordedPopLayer::OutputSimpleEventInfo(std::stringstream &aStringStream) const
{
=======
inline void RecordedPopLayer::OutputSimpleEventInfo(
    std::stringstream& aStringStream) const {
>>>>>>> upstream-releases
  aStringStream << "[" << mDT << "] PopLayer";
}

<<<<<<< HEAD
inline bool RecordedSetTransform::PlayEvent(Translator *aTranslator) const {
||||||| merged common ancestors
inline bool
RecordedSetTransform::PlayEvent(Translator *aTranslator) const
{
=======
inline bool RecordedSetTransform::PlayEvent(Translator* aTranslator) const {
>>>>>>> upstream-releases
  aTranslator->LookupDrawTarget(mDT)->SetTransform(mTransform);
  return true;
}

<<<<<<< HEAD
template <class S>
void RecordedSetTransform::Record(S &aStream) const {
||||||| merged common ancestors
template<class S>
void
RecordedSetTransform::Record(S &aStream) const
{
=======
template <class S>
void RecordedSetTransform::Record(S& aStream) const {
>>>>>>> upstream-releases
  RecordedDrawingEvent::Record(aStream);
  WriteElement(aStream, mTransform);
}

<<<<<<< HEAD
template <class S>
RecordedSetTransform::RecordedSetTransform(S &aStream)
    : RecordedDrawingEvent(SETTRANSFORM, aStream) {
||||||| merged common ancestors
template<class S>
RecordedSetTransform::RecordedSetTransform(S &aStream)
  : RecordedDrawingEvent(SETTRANSFORM, aStream)
{
=======
template <class S>
RecordedSetTransform::RecordedSetTransform(S& aStream)
    : RecordedDrawingEvent(SETTRANSFORM, aStream) {
>>>>>>> upstream-releases
  ReadElement(aStream, mTransform);
}

<<<<<<< HEAD
inline void RecordedSetTransform::OutputSimpleEventInfo(
    std::stringstream &aStringStream) const {
  aStringStream << "[" << mDT << "] SetTransform [ " << mTransform._11 << " "
                << mTransform._12 << " ; " << mTransform._21 << " "
                << mTransform._22 << " ; " << mTransform._31 << " "
                << mTransform._32 << " ]";
||||||| merged common ancestors
inline void
RecordedSetTransform::OutputSimpleEventInfo(std::stringstream &aStringStream) const
{
  aStringStream << "[" << mDT << "] SetTransform [ " << mTransform._11 << " " << mTransform._12 << " ; " <<
    mTransform._21 << " " << mTransform._22 << " ; " << mTransform._31 << " " << mTransform._32 << " ]";
=======
inline void RecordedSetTransform::OutputSimpleEventInfo(
    std::stringstream& aStringStream) const {
  aStringStream << "[" << mDT << "] SetTransform [ " << mTransform._11 << " "
                << mTransform._12 << " ; " << mTransform._21 << " "
                << mTransform._22 << " ; " << mTransform._31 << " "
                << mTransform._32 << " ]";
>>>>>>> upstream-releases
}

<<<<<<< HEAD
inline bool RecordedDrawSurface::PlayEvent(Translator *aTranslator) const {
  aTranslator->LookupDrawTarget(mDT)->DrawSurface(
      aTranslator->LookupSourceSurface(mRefSource), mDest, mSource, mDSOptions,
      mOptions);
||||||| merged common ancestors
inline bool
RecordedDrawSurface::PlayEvent(Translator *aTranslator) const
{
  aTranslator->LookupDrawTarget(mDT)->
    DrawSurface(aTranslator->LookupSourceSurface(mRefSource), mDest, mSource,
                mDSOptions, mOptions);
=======
inline bool RecordedDrawSurface::PlayEvent(Translator* aTranslator) const {
  aTranslator->LookupDrawTarget(mDT)->DrawSurface(
      aTranslator->LookupSourceSurface(mRefSource), mDest, mSource, mDSOptions,
      mOptions);
>>>>>>> upstream-releases
  return true;
}

<<<<<<< HEAD
template <class S>
void RecordedDrawSurface::Record(S &aStream) const {
||||||| merged common ancestors
template<class S>
void
RecordedDrawSurface::Record(S &aStream) const
{
=======
template <class S>
void RecordedDrawSurface::Record(S& aStream) const {
>>>>>>> upstream-releases
  RecordedDrawingEvent::Record(aStream);
  WriteElement(aStream, mRefSource);
  WriteElement(aStream, mDest);
  WriteElement(aStream, mSource);
  WriteElement(aStream, mDSOptions);
  WriteElement(aStream, mOptions);
}

<<<<<<< HEAD
template <class S>
RecordedDrawSurface::RecordedDrawSurface(S &aStream)
    : RecordedDrawingEvent(DRAWSURFACE, aStream) {
||||||| merged common ancestors
template<class S>
RecordedDrawSurface::RecordedDrawSurface(S &aStream)
  : RecordedDrawingEvent(DRAWSURFACE, aStream)
{
=======
template <class S>
RecordedDrawSurface::RecordedDrawSurface(S& aStream)
    : RecordedDrawingEvent(DRAWSURFACE, aStream) {
>>>>>>> upstream-releases
  ReadElement(aStream, mRefSource);
  ReadElement(aStream, mDest);
  ReadElement(aStream, mSource);
  ReadDrawSurfaceOptions(aStream, mDSOptions);
  ReadDrawOptions(aStream, mOptions);
}

<<<<<<< HEAD
inline void RecordedDrawSurface::OutputSimpleEventInfo(
    std::stringstream &aStringStream) const {
||||||| merged common ancestors
inline void
RecordedDrawSurface::OutputSimpleEventInfo(std::stringstream &aStringStream) const
{
=======
inline void RecordedDrawSurface::OutputSimpleEventInfo(
    std::stringstream& aStringStream) const {
>>>>>>> upstream-releases
  aStringStream << "[" << mDT << "] DrawSurface (" << mRefSource << ")";
}

<<<<<<< HEAD
inline bool RecordedDrawDependentSurface::PlayEvent(
    Translator *aTranslator) const {
||||||| merged common ancestors
inline bool
RecordedDrawDependentSurface::PlayEvent(Translator *aTranslator) const
{
=======
inline bool RecordedDrawDependentSurface::PlayEvent(
    Translator* aTranslator) const {
>>>>>>> upstream-releases
  RefPtr<SourceSurface> surface(aTranslator->LookupExternalSurface(mId));
  aTranslator->LookupDrawTarget(mDT)->DrawSurface(
      surface, mDest, Rect(Point(), Size(surface->GetSize())), mDSOptions,
      mOptions);
  return true;
}

<<<<<<< HEAD
template <class S>
void RecordedDrawDependentSurface::Record(S &aStream) const {
||||||| merged common ancestors
template<class S>
void
RecordedDrawDependentSurface::Record(S &aStream) const
{
=======
template <class S>
void RecordedDrawDependentSurface::Record(S& aStream) const {
>>>>>>> upstream-releases
  RecordedDrawingEvent::Record(aStream);
  WriteElement(aStream, mId);
  WriteElement(aStream, mDest);
  WriteElement(aStream, mDSOptions);
  WriteElement(aStream, mOptions);
}

<<<<<<< HEAD
template <class S>
RecordedDrawDependentSurface::RecordedDrawDependentSurface(S &aStream)
    : RecordedDrawingEvent(DRAWDEPENDENTSURFACE, aStream) {
||||||| merged common ancestors
template<class S>
RecordedDrawDependentSurface::RecordedDrawDependentSurface(S &aStream)
  : RecordedDrawingEvent(DRAWDEPENDENTSURFACE, aStream)
{
=======
template <class S>
RecordedDrawDependentSurface::RecordedDrawDependentSurface(S& aStream)
    : RecordedDrawingEvent(DRAWDEPENDENTSURFACE, aStream) {
>>>>>>> upstream-releases
  ReadElement(aStream, mId);
  ReadElement(aStream, mDest);
  ReadDrawSurfaceOptions(aStream, mDSOptions);
  ReadDrawOptions(aStream, mOptions);
}

<<<<<<< HEAD
inline void RecordedDrawDependentSurface::OutputSimpleEventInfo(
    std::stringstream &aStringStream) const {
||||||| merged common ancestors
inline void
RecordedDrawDependentSurface::OutputSimpleEventInfo(std::stringstream &aStringStream) const
{
=======
inline void RecordedDrawDependentSurface::OutputSimpleEventInfo(
    std::stringstream& aStringStream) const {
>>>>>>> upstream-releases
  aStringStream << "[" << mDT << "] DrawDependentSurface (" << mId << ")";
}

<<<<<<< HEAD
inline bool RecordedDrawFilter::PlayEvent(Translator *aTranslator) const {
  aTranslator->LookupDrawTarget(mDT)->DrawFilter(
      aTranslator->LookupFilterNode(mNode), mSourceRect, mDestPoint, mOptions);
||||||| merged common ancestors
inline bool
RecordedDrawFilter::PlayEvent(Translator *aTranslator) const
{
  aTranslator->LookupDrawTarget(mDT)->
    DrawFilter(aTranslator->LookupFilterNode(mNode), mSourceRect,
                mDestPoint, mOptions);
=======
inline bool RecordedDrawFilter::PlayEvent(Translator* aTranslator) const {
  aTranslator->LookupDrawTarget(mDT)->DrawFilter(
      aTranslator->LookupFilterNode(mNode), mSourceRect, mDestPoint, mOptions);
>>>>>>> upstream-releases
  return true;
}

<<<<<<< HEAD
template <class S>
void RecordedDrawFilter::Record(S &aStream) const {
||||||| merged common ancestors
template<class S>
void
RecordedDrawFilter::Record(S &aStream) const
{
=======
template <class S>
void RecordedDrawFilter::Record(S& aStream) const {
>>>>>>> upstream-releases
  RecordedDrawingEvent::Record(aStream);
  WriteElement(aStream, mNode);
  WriteElement(aStream, mSourceRect);
  WriteElement(aStream, mDestPoint);
  WriteElement(aStream, mOptions);
}

<<<<<<< HEAD
template <class S>
RecordedDrawFilter::RecordedDrawFilter(S &aStream)
    : RecordedDrawingEvent(DRAWFILTER, aStream) {
||||||| merged common ancestors
template<class S>
RecordedDrawFilter::RecordedDrawFilter(S &aStream)
  : RecordedDrawingEvent(DRAWFILTER, aStream)
{
=======
template <class S>
RecordedDrawFilter::RecordedDrawFilter(S& aStream)
    : RecordedDrawingEvent(DRAWFILTER, aStream) {
>>>>>>> upstream-releases
  ReadElement(aStream, mNode);
  ReadElement(aStream, mSourceRect);
  ReadElement(aStream, mDestPoint);
  ReadDrawOptions(aStream, mOptions);
}

<<<<<<< HEAD
inline void RecordedDrawFilter::OutputSimpleEventInfo(
    std::stringstream &aStringStream) const {
||||||| merged common ancestors
inline void
RecordedDrawFilter::OutputSimpleEventInfo(std::stringstream &aStringStream) const
{
=======
inline void RecordedDrawFilter::OutputSimpleEventInfo(
    std::stringstream& aStringStream) const {
>>>>>>> upstream-releases
  aStringStream << "[" << mDT << "] DrawFilter (" << mNode << ")";
}

<<<<<<< HEAD
inline bool RecordedDrawSurfaceWithShadow::PlayEvent(
    Translator *aTranslator) const {
  aTranslator->LookupDrawTarget(mDT)->DrawSurfaceWithShadow(
      aTranslator->LookupSourceSurface(mRefSource), mDest, mColor, mOffset,
      mSigma, mOp);
||||||| merged common ancestors
inline bool
RecordedDrawSurfaceWithShadow::PlayEvent(Translator *aTranslator) const
{
  aTranslator->LookupDrawTarget(mDT)->
    DrawSurfaceWithShadow(aTranslator->LookupSourceSurface(mRefSource),
                          mDest, mColor, mOffset, mSigma, mOp);
=======
inline bool RecordedDrawSurfaceWithShadow::PlayEvent(
    Translator* aTranslator) const {
  aTranslator->LookupDrawTarget(mDT)->DrawSurfaceWithShadow(
      aTranslator->LookupSourceSurface(mRefSource), mDest, mColor, mOffset,
      mSigma, mOp);
>>>>>>> upstream-releases
  return true;
}

<<<<<<< HEAD
template <class S>
void RecordedDrawSurfaceWithShadow::Record(S &aStream) const {
||||||| merged common ancestors
template<class S>
void
RecordedDrawSurfaceWithShadow::Record(S &aStream) const
{
=======
template <class S>
void RecordedDrawSurfaceWithShadow::Record(S& aStream) const {
>>>>>>> upstream-releases
  RecordedDrawingEvent::Record(aStream);
  WriteElement(aStream, mRefSource);
  WriteElement(aStream, mDest);
  WriteElement(aStream, mColor);
  WriteElement(aStream, mOffset);
  WriteElement(aStream, mSigma);
  WriteElement(aStream, mOp);
}

<<<<<<< HEAD
template <class S>
RecordedDrawSurfaceWithShadow::RecordedDrawSurfaceWithShadow(S &aStream)
    : RecordedDrawingEvent(DRAWSURFACEWITHSHADOW, aStream) {
||||||| merged common ancestors
template<class S>
RecordedDrawSurfaceWithShadow::RecordedDrawSurfaceWithShadow(S &aStream)
  : RecordedDrawingEvent(DRAWSURFACEWITHSHADOW, aStream)
{
=======
template <class S>
RecordedDrawSurfaceWithShadow::RecordedDrawSurfaceWithShadow(S& aStream)
    : RecordedDrawingEvent(DRAWSURFACEWITHSHADOW, aStream) {
>>>>>>> upstream-releases
  ReadElement(aStream, mRefSource);
  ReadElement(aStream, mDest);
  ReadElement(aStream, mColor);
  ReadElement(aStream, mOffset);
  ReadElement(aStream, mSigma);
<<<<<<< HEAD
  ReadElement(aStream, mOp);
}

inline void RecordedDrawSurfaceWithShadow::OutputSimpleEventInfo(
    std::stringstream &aStringStream) const {
  aStringStream << "[" << mDT << "] DrawSurfaceWithShadow (" << mRefSource
                << ") Color: (" << mColor.r << ", " << mColor.g << ", "
                << mColor.b << ", " << mColor.a << ")";
||||||| merged common ancestors
  ReadElement(aStream, mOp);
}

inline void
RecordedDrawSurfaceWithShadow::OutputSimpleEventInfo(std::stringstream &aStringStream) const
{
  aStringStream << "[" << mDT << "] DrawSurfaceWithShadow (" << mRefSource << ") Color: (" <<
    mColor.r << ", " << mColor.g << ", " << mColor.b << ", " << mColor.a << ")";
=======
  ReadElementConstrained(aStream, mOp, CompositionOp::OP_OVER, CompositionOp::OP_COUNT);
>>>>>>> upstream-releases
}

<<<<<<< HEAD
inline RecordedPathCreation::RecordedPathCreation(PathRecording *aPath)
    : RecordedEventDerived(PATHCREATION),
      mRefPtr(aPath),
      mFillRule(aPath->mFillRule),
      mPathOps(aPath->mPathOps) {}
||||||| merged common ancestors
inline
RecordedPathCreation::RecordedPathCreation(PathRecording *aPath)
  : RecordedEventDerived(PATHCREATION), mRefPtr(aPath), mFillRule(aPath->mFillRule), mPathOps(aPath->mPathOps)
{
}
=======
inline void RecordedDrawSurfaceWithShadow::OutputSimpleEventInfo(
    std::stringstream& aStringStream) const {
  aStringStream << "[" << mDT << "] DrawSurfaceWithShadow (" << mRefSource
                << ") Color: (" << mColor.r << ", " << mColor.g << ", "
                << mColor.b << ", " << mColor.a << ")";
}
>>>>>>> upstream-releases

<<<<<<< HEAD
inline RecordedPathCreation::~RecordedPathCreation() {}
||||||| merged common ancestors
inline
RecordedPathCreation::~RecordedPathCreation()
{
}
=======
inline RecordedPathCreation::RecordedPathCreation(PathRecording* aPath)
    : RecordedEventDerived(PATHCREATION),
      mRefPtr(aPath),
      mFillRule(aPath->mFillRule),
      mPath(aPath) {}
>>>>>>> upstream-releases

<<<<<<< HEAD
inline bool RecordedPathCreation::PlayEvent(Translator *aTranslator) const {
||||||| merged common ancestors
inline bool
RecordedPathCreation::PlayEvent(Translator *aTranslator) const
{
=======
inline bool RecordedPathCreation::PlayEvent(Translator* aTranslator) const {
>>>>>>> upstream-releases
  RefPtr<PathBuilder> builder =
<<<<<<< HEAD
      aTranslator->GetReferenceDrawTarget()->CreatePathBuilder(mFillRule);

  for (size_t i = 0; i < mPathOps.size(); i++) {
    const PathOp &op = mPathOps[i];
    switch (op.mType) {
      case PathOp::OP_MOVETO:
        builder->MoveTo(op.mP1);
        break;
      case PathOp::OP_LINETO:
        builder->LineTo(op.mP1);
        break;
      case PathOp::OP_BEZIERTO:
        builder->BezierTo(op.mP1, op.mP2, op.mP3);
        break;
      case PathOp::OP_QUADRATICBEZIERTO:
        builder->QuadraticBezierTo(op.mP1, op.mP2);
        break;
      case PathOp::OP_CLOSE:
        builder->Close();
        break;
    }
||||||| merged common ancestors
    aTranslator->GetReferenceDrawTarget()->CreatePathBuilder(mFillRule);

  for (size_t i = 0; i < mPathOps.size(); i++) {
    const PathOp &op = mPathOps[i];
    switch (op.mType) {
    case PathOp::OP_MOVETO:
      builder->MoveTo(op.mP1);
      break;
    case PathOp::OP_LINETO:
      builder->LineTo(op.mP1);
      break;
    case PathOp::OP_BEZIERTO:
      builder->BezierTo(op.mP1, op.mP2, op.mP3);
      break;
    case PathOp::OP_QUADRATICBEZIERTO:
      builder->QuadraticBezierTo(op.mP1, op.mP2);
      break;
    case PathOp::OP_CLOSE:
      builder->Close();
      break;
    }
=======
      aTranslator->GetReferenceDrawTarget()->CreatePathBuilder(mFillRule);
  if (!mPathOps->StreamToSink(*builder)) {
    return false;
>>>>>>> upstream-releases
  }

  RefPtr<Path> path = builder->Finish();
  aTranslator->AddPath(mRefPtr, path);
  return true;
}

<<<<<<< HEAD
template <class S>
void RecordedPathCreation::Record(S &aStream) const {
||||||| merged common ancestors
template<class S>
void
RecordedPathCreation::Record(S &aStream) const
{
=======
template <class S>
void RecordedPathCreation::Record(S& aStream) const {
>>>>>>> upstream-releases
  WriteElement(aStream, mRefPtr);
  WriteElement(aStream, mFillRule);
<<<<<<< HEAD
  typedef std::vector<PathOp> pathOpVec;
  for (pathOpVec::const_iterator iter = mPathOps.begin();
       iter != mPathOps.end(); iter++) {
    WriteElement(aStream, iter->mType);
    if (sPointCount[iter->mType] >= 1) {
      WriteElement(aStream, iter->mP1);
    }
    if (sPointCount[iter->mType] >= 2) {
      WriteElement(aStream, iter->mP2);
    }
    if (sPointCount[iter->mType] >= 3) {
      WriteElement(aStream, iter->mP3);
    }
  }
||||||| merged common ancestors
  typedef std::vector<PathOp> pathOpVec;
  for (pathOpVec::const_iterator iter = mPathOps.begin(); iter != mPathOps.end(); iter++) {
    WriteElement(aStream, iter->mType);
    if (sPointCount[iter->mType] >= 1) {
      WriteElement(aStream, iter->mP1);
    }
    if (sPointCount[iter->mType] >= 2) {
      WriteElement(aStream, iter->mP2);
    }
    if (sPointCount[iter->mType] >= 3) {
      WriteElement(aStream, iter->mP3);
    }
  }

=======
  mPath->mPathOps.Record(aStream);
>>>>>>> upstream-releases
}

<<<<<<< HEAD
template <class S>
RecordedPathCreation::RecordedPathCreation(S &aStream)
    : RecordedEventDerived(PATHCREATION) {
  uint64_t size;

||||||| merged common ancestors
template<class S>
RecordedPathCreation::RecordedPathCreation(S &aStream)
  : RecordedEventDerived(PATHCREATION)
{
  uint64_t size;

=======
template <class S>
RecordedPathCreation::RecordedPathCreation(S& aStream)
    : RecordedEventDerived(PATHCREATION) {
>>>>>>> upstream-releases
  ReadElement(aStream, mRefPtr);
<<<<<<< HEAD
  ReadElement(aStream, size);
  ReadElement(aStream, mFillRule);

  for (uint64_t i = 0; i < size; i++) {
    PathOp newPathOp;
    ReadElement(aStream, newPathOp.mType);
    if (sPointCount[newPathOp.mType] >= 1) {
      ReadElement(aStream, newPathOp.mP1);
    }
    if (sPointCount[newPathOp.mType] >= 2) {
      ReadElement(aStream, newPathOp.mP2);
    }
    if (sPointCount[newPathOp.mType] >= 3) {
      ReadElement(aStream, newPathOp.mP3);
    }

    mPathOps.push_back(newPathOp);
  }
||||||| merged common ancestors
  ReadElement(aStream, size);
  ReadElement(aStream, mFillRule);

  for (uint64_t i = 0; i < size; i++) {
    PathOp newPathOp;
    ReadElement(aStream, newPathOp.mType);
    if (sPointCount[newPathOp.mType] >= 1) {
      ReadElement(aStream, newPathOp.mP1);
    }
    if (sPointCount[newPathOp.mType] >= 2) {
      ReadElement(aStream, newPathOp.mP2);
    }
    if (sPointCount[newPathOp.mType] >= 3) {
      ReadElement(aStream, newPathOp.mP3);
    }

    mPathOps.push_back(newPathOp);
  }

=======
  ReadElementConstrained(aStream, mFillRule, FillRule::FILL_WINDING,
                         FillRule::FILL_EVEN_ODD);
  mPathOps = MakeUnique<PathOps>(aStream);
>>>>>>> upstream-releases
}

<<<<<<< HEAD
inline void RecordedPathCreation::OutputSimpleEventInfo(
    std::stringstream &aStringStream) const {
  aStringStream << "[" << mRefPtr
                << "] Path created (OpCount: " << mPathOps.size() << ")";
||||||| merged common ancestors
inline void
RecordedPathCreation::OutputSimpleEventInfo(std::stringstream &aStringStream) const
{
  aStringStream << "[" << mRefPtr << "] Path created (OpCount: " << mPathOps.size() << ")";
=======
inline void RecordedPathCreation::OutputSimpleEventInfo(
    std::stringstream& aStringStream) const {
  size_t numberOfOps =
      mPath ? mPath->mPathOps.NumberOfOps() : mPathOps->NumberOfOps();
  aStringStream << "[" << mRefPtr << "] Path created (OpCount: " << numberOfOps
                << ")";
>>>>>>> upstream-releases
}
<<<<<<< HEAD
inline bool RecordedPathDestruction::PlayEvent(Translator *aTranslator) const {
||||||| merged common ancestors
inline bool
RecordedPathDestruction::PlayEvent(Translator *aTranslator) const
{
=======
inline bool RecordedPathDestruction::PlayEvent(Translator* aTranslator) const {
>>>>>>> upstream-releases
  aTranslator->RemovePath(mRefPtr);
  return true;
}

<<<<<<< HEAD
template <class S>
void RecordedPathDestruction::Record(S &aStream) const {
||||||| merged common ancestors
template<class S>
void
RecordedPathDestruction::Record(S &aStream) const
{
=======
template <class S>
void RecordedPathDestruction::Record(S& aStream) const {
>>>>>>> upstream-releases
  WriteElement(aStream, mRefPtr);
}

<<<<<<< HEAD
template <class S>
RecordedPathDestruction::RecordedPathDestruction(S &aStream)
    : RecordedEventDerived(PATHDESTRUCTION) {
||||||| merged common ancestors
template<class S>
RecordedPathDestruction::RecordedPathDestruction(S &aStream)
  : RecordedEventDerived(PATHDESTRUCTION)
{
=======
template <class S>
RecordedPathDestruction::RecordedPathDestruction(S& aStream)
    : RecordedEventDerived(PATHDESTRUCTION) {
>>>>>>> upstream-releases
  ReadElement(aStream, mRefPtr);
}

<<<<<<< HEAD
inline void RecordedPathDestruction::OutputSimpleEventInfo(
    std::stringstream &aStringStream) const {
||||||| merged common ancestors
inline void
RecordedPathDestruction::OutputSimpleEventInfo(std::stringstream &aStringStream) const
{
=======
inline void RecordedPathDestruction::OutputSimpleEventInfo(
    std::stringstream& aStringStream) const {
>>>>>>> upstream-releases
  aStringStream << "[" << mRefPtr << "] Path Destroyed";
}

inline RecordedSourceSurfaceCreation::~RecordedSourceSurfaceCreation() {
  if (mDataOwned) {
    delete[] mData;
  }
}

<<<<<<< HEAD
inline bool RecordedSourceSurfaceCreation::PlayEvent(
    Translator *aTranslator) const {
||||||| merged common ancestors
inline bool
RecordedSourceSurfaceCreation::PlayEvent(Translator *aTranslator) const
{
=======
inline bool RecordedSourceSurfaceCreation::PlayEvent(
    Translator* aTranslator) const {
>>>>>>> upstream-releases
  if (!mData) {
    return false;
  }

  RefPtr<SourceSurface> src =
      aTranslator->GetReferenceDrawTarget()->CreateSourceSurfaceFromData(
          mData, mSize, mSize.width * BytesPerPixel(mFormat), mFormat);
  aTranslator->AddSourceSurface(mRefPtr, src);
  return true;
}

<<<<<<< HEAD
template <class S>
void RecordedSourceSurfaceCreation::Record(S &aStream) const {
||||||| merged common ancestors
template<class S>
void
RecordedSourceSurfaceCreation::Record(S &aStream) const
{
=======
template <class S>
void RecordedSourceSurfaceCreation::Record(S& aStream) const {
>>>>>>> upstream-releases
  WriteElement(aStream, mRefPtr);
  WriteElement(aStream, mSize);
  WriteElement(aStream, mFormat);
  MOZ_ASSERT(mData);
<<<<<<< HEAD
  for (int y = 0; y < mSize.height; y++) {
    aStream.write((const char *)mData + y * mStride,
                  BytesPerPixel(mFormat) * mSize.width);
||||||| merged common ancestors
  for (int y = 0; y < mSize.height; y++) {
    aStream.write((const char*)mData + y * mStride, BytesPerPixel(mFormat) * mSize.width);
=======
  size_t dataFormatWidth = BytesPerPixel(mFormat) * mSize.width;
  const char* endSrc = (const char*)(mData + (mSize.height * mStride));
  for (const char* src = (const char*)mData; src < endSrc; src += mStride) {
    aStream.write(src, dataFormatWidth);
>>>>>>> upstream-releases
  }
}

<<<<<<< HEAD
template <class S>
RecordedSourceSurfaceCreation::RecordedSourceSurfaceCreation(S &aStream)
    : RecordedEventDerived(SOURCESURFACECREATION), mDataOwned(true) {
||||||| merged common ancestors
template<class S>
RecordedSourceSurfaceCreation::RecordedSourceSurfaceCreation(S &aStream)
  : RecordedEventDerived(SOURCESURFACECREATION), mDataOwned(true)
{
=======
template <class S>
RecordedSourceSurfaceCreation::RecordedSourceSurfaceCreation(S& aStream)
    : RecordedEventDerived(SOURCESURFACECREATION), mDataOwned(true) {
>>>>>>> upstream-releases
  ReadElement(aStream, mRefPtr);
  ReadElement(aStream, mSize);
  ReadElementConstrained(aStream, mFormat, SurfaceFormat::A8R8G8B8_UINT32,
                         SurfaceFormat::UNKNOWN);
  size_t size = mSize.width * mSize.height * BytesPerPixel(mFormat);
  mData = new (fallible) uint8_t[size];
  if (!mData) {
    gfxCriticalNote
        << "RecordedSourceSurfaceCreation failed to allocate data of size "
        << size;
  } else {
    aStream.read((char *)mData, size);
  }
}

<<<<<<< HEAD
inline void RecordedSourceSurfaceCreation::OutputSimpleEventInfo(
    std::stringstream &aStringStream) const {
  aStringStream << "[" << mRefPtr
                << "] SourceSurface created (Size: " << mSize.width << "x"
                << mSize.height << ")";
||||||| merged common ancestors
inline void
RecordedSourceSurfaceCreation::OutputSimpleEventInfo(std::stringstream &aStringStream) const
{
  aStringStream << "[" << mRefPtr << "] SourceSurface created (Size: " << mSize.width << "x" << mSize.height << ")";
=======
inline void RecordedSourceSurfaceCreation::OutputSimpleEventInfo(
    std::stringstream& aStringStream) const {
  aStringStream << "[" << mRefPtr
                << "] SourceSurface created (Size: " << mSize.width << "x"
                << mSize.height << ")";
>>>>>>> upstream-releases
}

<<<<<<< HEAD
inline bool RecordedSourceSurfaceDestruction::PlayEvent(
    Translator *aTranslator) const {
||||||| merged common ancestors
inline bool
RecordedSourceSurfaceDestruction::PlayEvent(Translator *aTranslator) const
{
=======
inline bool RecordedSourceSurfaceDestruction::PlayEvent(
    Translator* aTranslator) const {
>>>>>>> upstream-releases
  aTranslator->RemoveSourceSurface(mRefPtr);
  return true;
}

<<<<<<< HEAD
template <class S>
void RecordedSourceSurfaceDestruction::Record(S &aStream) const {
||||||| merged common ancestors
template<class S>
void
RecordedSourceSurfaceDestruction::Record(S &aStream) const
{
=======
template <class S>
void RecordedSourceSurfaceDestruction::Record(S& aStream) const {
>>>>>>> upstream-releases
  WriteElement(aStream, mRefPtr);
}

<<<<<<< HEAD
template <class S>
RecordedSourceSurfaceDestruction::RecordedSourceSurfaceDestruction(S &aStream)
    : RecordedEventDerived(SOURCESURFACEDESTRUCTION) {
||||||| merged common ancestors
template<class S>
RecordedSourceSurfaceDestruction::RecordedSourceSurfaceDestruction(S &aStream)
  : RecordedEventDerived(SOURCESURFACEDESTRUCTION)
{
=======
template <class S>
RecordedSourceSurfaceDestruction::RecordedSourceSurfaceDestruction(S& aStream)
    : RecordedEventDerived(SOURCESURFACEDESTRUCTION) {
>>>>>>> upstream-releases
  ReadElement(aStream, mRefPtr);
}

<<<<<<< HEAD
inline void RecordedSourceSurfaceDestruction::OutputSimpleEventInfo(
    std::stringstream &aStringStream) const {
||||||| merged common ancestors
inline void
RecordedSourceSurfaceDestruction::OutputSimpleEventInfo(std::stringstream &aStringStream) const
{
=======
inline void RecordedSourceSurfaceDestruction::OutputSimpleEventInfo(
    std::stringstream& aStringStream) const {
>>>>>>> upstream-releases
  aStringStream << "[" << mRefPtr << "] SourceSurface Destroyed";
}

<<<<<<< HEAD
inline bool RecordedExternalSurfaceCreation::PlayEvent(
    Translator *aTranslator) const {
||||||| merged common ancestors
inline bool
RecordedExternalSurfaceCreation::PlayEvent(Translator *aTranslator) const
{
=======
inline bool RecordedExternalSurfaceCreation::PlayEvent(
    Translator* aTranslator) const {
>>>>>>> upstream-releases
  RefPtr<SourceSurface> surface = aTranslator->LookupExternalSurface(mKey);
  if (!surface) {
    return false;
  }

  aTranslator->AddSourceSurface(mRefPtr, surface);
  return true;
}

<<<<<<< HEAD
template <class S>
void RecordedExternalSurfaceCreation::Record(S &aStream) const {
||||||| merged common ancestors
template<class S>
void
RecordedExternalSurfaceCreation::Record(S &aStream) const
{
=======
template <class S>
void RecordedExternalSurfaceCreation::Record(S& aStream) const {
>>>>>>> upstream-releases
  WriteElement(aStream, mRefPtr);
  WriteElement(aStream, mKey);
}

<<<<<<< HEAD
template <class S>
RecordedExternalSurfaceCreation::RecordedExternalSurfaceCreation(S &aStream)
    : RecordedEventDerived(EXTERNALSURFACECREATION) {
||||||| merged common ancestors
template<class S>
RecordedExternalSurfaceCreation::RecordedExternalSurfaceCreation(S &aStream)
  : RecordedEventDerived(EXTERNALSURFACECREATION)
{
=======
template <class S>
RecordedExternalSurfaceCreation::RecordedExternalSurfaceCreation(S& aStream)
    : RecordedEventDerived(EXTERNALSURFACECREATION) {
>>>>>>> upstream-releases
  ReadElement(aStream, mRefPtr);
  ReadElement(aStream, mKey);
}

<<<<<<< HEAD
inline void RecordedExternalSurfaceCreation::OutputSimpleEventInfo(
    std::stringstream &aStringStream) const {
  aStringStream << "[" << mRefPtr
                << "] SourceSurfaceSharedData created (Key: " << mKey << ")";
||||||| merged common ancestors
inline void
RecordedExternalSurfaceCreation::OutputSimpleEventInfo(std::stringstream &aStringStream) const
{
  aStringStream << "[" << mRefPtr << "] SourceSurfaceSharedData created (Key: " << mKey << ")";
=======
inline void RecordedExternalSurfaceCreation::OutputSimpleEventInfo(
    std::stringstream& aStringStream) const {
  aStringStream << "[" << mRefPtr
                << "] SourceSurfaceSharedData created (Key: " << mKey << ")";
>>>>>>> upstream-releases
}

inline RecordedFilterNodeCreation::~RecordedFilterNodeCreation() {}

<<<<<<< HEAD
inline bool RecordedFilterNodeCreation::PlayEvent(
    Translator *aTranslator) const {
  RefPtr<FilterNode> node =
      aTranslator->GetReferenceDrawTarget()->CreateFilter(mType);
||||||| merged common ancestors
inline bool
RecordedFilterNodeCreation::PlayEvent(Translator *aTranslator) const
{
  RefPtr<FilterNode> node = aTranslator->GetReferenceDrawTarget()->
    CreateFilter(mType);
=======
inline bool RecordedFilterNodeCreation::PlayEvent(
    Translator* aTranslator) const {
  RefPtr<FilterNode> node =
      aTranslator->GetReferenceDrawTarget()->CreateFilter(mType);
>>>>>>> upstream-releases
  aTranslator->AddFilterNode(mRefPtr, node);
  return true;
}

<<<<<<< HEAD
template <class S>
void RecordedFilterNodeCreation::Record(S &aStream) const {
||||||| merged common ancestors
template<class S>
void
RecordedFilterNodeCreation::Record(S &aStream) const
{
=======
template <class S>
void RecordedFilterNodeCreation::Record(S& aStream) const {
>>>>>>> upstream-releases
  WriteElement(aStream, mRefPtr);
  WriteElement(aStream, mType);
}

<<<<<<< HEAD
template <class S>
RecordedFilterNodeCreation::RecordedFilterNodeCreation(S &aStream)
    : RecordedEventDerived(FILTERNODECREATION) {
||||||| merged common ancestors
template<class S>
RecordedFilterNodeCreation::RecordedFilterNodeCreation(S &aStream)
  : RecordedEventDerived(FILTERNODECREATION)
{
=======
template <class S>
RecordedFilterNodeCreation::RecordedFilterNodeCreation(S& aStream)
    : RecordedEventDerived(FILTERNODECREATION) {
>>>>>>> upstream-releases
  ReadElement(aStream, mRefPtr);
  ReadElementConstrained(aStream, mType, FilterType::BLEND,
                         FilterType::OPACITY);
}

<<<<<<< HEAD
inline void RecordedFilterNodeCreation::OutputSimpleEventInfo(
    std::stringstream &aStringStream) const {
  aStringStream << "[" << mRefPtr
                << "] FilterNode created (Type: " << int(mType) << ")";
||||||| merged common ancestors
inline void
RecordedFilterNodeCreation::OutputSimpleEventInfo(std::stringstream &aStringStream) const
{
  aStringStream << "[" << mRefPtr << "] FilterNode created (Type: " << int(mType) << ")";
=======
inline void RecordedFilterNodeCreation::OutputSimpleEventInfo(
    std::stringstream& aStringStream) const {
  aStringStream << "[" << mRefPtr
                << "] FilterNode created (Type: " << int(mType) << ")";
>>>>>>> upstream-releases
}

<<<<<<< HEAD
inline bool RecordedFilterNodeDestruction::PlayEvent(
    Translator *aTranslator) const {
||||||| merged common ancestors
inline bool
RecordedFilterNodeDestruction::PlayEvent(Translator *aTranslator) const
{
=======
inline bool RecordedFilterNodeDestruction::PlayEvent(
    Translator* aTranslator) const {
>>>>>>> upstream-releases
  aTranslator->RemoveFilterNode(mRefPtr);
  return true;
}

<<<<<<< HEAD
template <class S>
void RecordedFilterNodeDestruction::Record(S &aStream) const {
||||||| merged common ancestors
template<class S>
void
RecordedFilterNodeDestruction::Record(S &aStream) const
{
=======
template <class S>
void RecordedFilterNodeDestruction::Record(S& aStream) const {
>>>>>>> upstream-releases
  WriteElement(aStream, mRefPtr);
}

<<<<<<< HEAD
template <class S>
RecordedFilterNodeDestruction::RecordedFilterNodeDestruction(S &aStream)
    : RecordedEventDerived(FILTERNODEDESTRUCTION) {
||||||| merged common ancestors
template<class S>
RecordedFilterNodeDestruction::RecordedFilterNodeDestruction(S &aStream)
  : RecordedEventDerived(FILTERNODEDESTRUCTION)
{
=======
template <class S>
RecordedFilterNodeDestruction::RecordedFilterNodeDestruction(S& aStream)
    : RecordedEventDerived(FILTERNODEDESTRUCTION) {
>>>>>>> upstream-releases
  ReadElement(aStream, mRefPtr);
}

<<<<<<< HEAD
inline void RecordedFilterNodeDestruction::OutputSimpleEventInfo(
    std::stringstream &aStringStream) const {
||||||| merged common ancestors
inline void
RecordedFilterNodeDestruction::OutputSimpleEventInfo(std::stringstream &aStringStream) const
{
=======
inline void RecordedFilterNodeDestruction::OutputSimpleEventInfo(
    std::stringstream& aStringStream) const {
>>>>>>> upstream-releases
  aStringStream << "[" << mRefPtr << "] FilterNode Destroyed";
}

inline RecordedGradientStopsCreation::~RecordedGradientStopsCreation() {
  if (mDataOwned) {
    delete[] mStops;
  }
}

<<<<<<< HEAD
inline bool RecordedGradientStopsCreation::PlayEvent(
    Translator *aTranslator) const {
  RefPtr<GradientStops> src =
      aTranslator->GetReferenceDrawTarget()->CreateGradientStops(
          mStops, mNumStops, mExtendMode);
||||||| merged common ancestors
inline bool
RecordedGradientStopsCreation::PlayEvent(Translator *aTranslator) const
{
  RefPtr<GradientStops> src = aTranslator->GetReferenceDrawTarget()->
    CreateGradientStops(mStops, mNumStops, mExtendMode);
=======
inline bool RecordedGradientStopsCreation::PlayEvent(
    Translator* aTranslator) const {
  RefPtr<GradientStops> src =
      aTranslator->GetReferenceDrawTarget()->CreateGradientStops(
          mStops, mNumStops, mExtendMode);
>>>>>>> upstream-releases
  aTranslator->AddGradientStops(mRefPtr, src);
  return true;
}

<<<<<<< HEAD
template <class S>
void RecordedGradientStopsCreation::Record(S &aStream) const {
||||||| merged common ancestors
template<class S>
void
RecordedGradientStopsCreation::Record(S &aStream) const
{
=======
template <class S>
void RecordedGradientStopsCreation::Record(S& aStream) const {
>>>>>>> upstream-releases
  WriteElement(aStream, mRefPtr);
  WriteElement(aStream, mExtendMode);
  WriteElement(aStream, mNumStops);
  aStream.write((const char *)mStops, mNumStops * sizeof(GradientStop));
}

<<<<<<< HEAD
template <class S>
RecordedGradientStopsCreation::RecordedGradientStopsCreation(S &aStream)
    : RecordedEventDerived(GRADIENTSTOPSCREATION), mDataOwned(true) {
||||||| merged common ancestors
template<class S>
RecordedGradientStopsCreation::RecordedGradientStopsCreation(S &aStream)
  : RecordedEventDerived(GRADIENTSTOPSCREATION), mDataOwned(true)
{
=======
template <class S>
RecordedGradientStopsCreation::RecordedGradientStopsCreation(S& aStream)
    : RecordedEventDerived(GRADIENTSTOPSCREATION), mDataOwned(true) {
>>>>>>> upstream-releases
  ReadElement(aStream, mRefPtr);
  ReadElementConstrained(aStream, mExtendMode, ExtendMode::CLAMP,
                         ExtendMode::REFLECT);
  ReadElement(aStream, mNumStops);
  mStops = new GradientStop[mNumStops];

  aStream.read((char *)mStops, mNumStops * sizeof(GradientStop));
}

<<<<<<< HEAD
inline void RecordedGradientStopsCreation::OutputSimpleEventInfo(
    std::stringstream &aStringStream) const {
  aStringStream << "[" << mRefPtr
                << "] GradientStops created (Stops: " << mNumStops << ")";
||||||| merged common ancestors
inline void
RecordedGradientStopsCreation::OutputSimpleEventInfo(std::stringstream &aStringStream) const
{
  aStringStream << "[" << mRefPtr << "] GradientStops created (Stops: " << mNumStops << ")";
=======
inline void RecordedGradientStopsCreation::OutputSimpleEventInfo(
    std::stringstream& aStringStream) const {
  aStringStream << "[" << mRefPtr
                << "] GradientStops created (Stops: " << mNumStops << ")";
>>>>>>> upstream-releases
}

<<<<<<< HEAD
inline bool RecordedGradientStopsDestruction::PlayEvent(
    Translator *aTranslator) const {
||||||| merged common ancestors
inline bool
RecordedGradientStopsDestruction::PlayEvent(Translator *aTranslator) const
{
=======
inline bool RecordedGradientStopsDestruction::PlayEvent(
    Translator* aTranslator) const {
>>>>>>> upstream-releases
  aTranslator->RemoveGradientStops(mRefPtr);
  return true;
}

<<<<<<< HEAD
template <class S>
void RecordedGradientStopsDestruction::Record(S &aStream) const {
||||||| merged common ancestors
template<class S>
void
RecordedGradientStopsDestruction::Record(S &aStream) const
{
=======
template <class S>
void RecordedGradientStopsDestruction::Record(S& aStream) const {
>>>>>>> upstream-releases
  WriteElement(aStream, mRefPtr);
}

<<<<<<< HEAD
template <class S>
RecordedGradientStopsDestruction::RecordedGradientStopsDestruction(S &aStream)
    : RecordedEventDerived(GRADIENTSTOPSDESTRUCTION) {
||||||| merged common ancestors
template<class S>
RecordedGradientStopsDestruction::RecordedGradientStopsDestruction(S &aStream)
  : RecordedEventDerived(GRADIENTSTOPSDESTRUCTION)
{
=======
template <class S>
RecordedGradientStopsDestruction::RecordedGradientStopsDestruction(S& aStream)
    : RecordedEventDerived(GRADIENTSTOPSDESTRUCTION) {
>>>>>>> upstream-releases
  ReadElement(aStream, mRefPtr);
}

<<<<<<< HEAD
inline void RecordedGradientStopsDestruction::OutputSimpleEventInfo(
    std::stringstream &aStringStream) const {
||||||| merged common ancestors
inline void
RecordedGradientStopsDestruction::OutputSimpleEventInfo(std::stringstream &aStringStream) const
{
=======
inline void RecordedGradientStopsDestruction::OutputSimpleEventInfo(
    std::stringstream& aStringStream) const {
>>>>>>> upstream-releases
  aStringStream << "[" << mRefPtr << "] GradientStops Destroyed";
}

<<<<<<< HEAD
inline bool RecordedIntoLuminanceSource::PlayEvent(
    Translator *aTranslator) const {
  RefPtr<SourceSurface> src =
      aTranslator->LookupDrawTarget(mDT)->IntoLuminanceSource(mLuminanceType,
                                                              mOpacity);
||||||| merged common ancestors
inline bool
RecordedIntoLuminanceSource::PlayEvent(Translator *aTranslator) const
{
  RefPtr<SourceSurface> src = aTranslator->LookupDrawTarget(mDT)->IntoLuminanceSource(mLuminanceType, mOpacity);
=======
inline bool RecordedIntoLuminanceSource::PlayEvent(
    Translator* aTranslator) const {
  RefPtr<SourceSurface> src =
      aTranslator->LookupDrawTarget(mDT)->IntoLuminanceSource(mLuminanceType,
                                                              mOpacity);
>>>>>>> upstream-releases
  aTranslator->AddSourceSurface(mRefPtr, src);
  return true;
}

<<<<<<< HEAD
template <class S>
void RecordedIntoLuminanceSource::Record(S &aStream) const {
||||||| merged common ancestors
template<class S>
void
RecordedIntoLuminanceSource::Record(S &aStream) const
{
=======
template <class S>
void RecordedIntoLuminanceSource::Record(S& aStream) const {
>>>>>>> upstream-releases
  WriteElement(aStream, mRefPtr);
  WriteElement(aStream, mDT);
  WriteElement(aStream, mLuminanceType);
  WriteElement(aStream, mOpacity);
}

<<<<<<< HEAD
template <class S>
RecordedIntoLuminanceSource::RecordedIntoLuminanceSource(S &aStream)
    : RecordedEventDerived(INTOLUMINANCE) {
||||||| merged common ancestors
template<class S>
RecordedIntoLuminanceSource::RecordedIntoLuminanceSource(S &aStream)
  : RecordedEventDerived(INTOLUMINANCE)
{
=======
template <class S>
RecordedIntoLuminanceSource::RecordedIntoLuminanceSource(S& aStream)
    : RecordedEventDerived(INTOLUMINANCE) {
>>>>>>> upstream-releases
  ReadElement(aStream, mRefPtr);
  ReadElement(aStream, mDT);
  ReadElementConstrained(aStream, mLuminanceType, LuminanceType::LUMINANCE,
                         LuminanceType::LINEARRGB);
  ReadElement(aStream, mOpacity);
}

<<<<<<< HEAD
inline void RecordedIntoLuminanceSource::OutputSimpleEventInfo(
    std::stringstream &aStringStream) const {
  aStringStream << "[" << mRefPtr << "] Into Luminance Source (DT: " << mDT
                << ")";
||||||| merged common ancestors
inline void
RecordedIntoLuminanceSource::OutputSimpleEventInfo(std::stringstream &aStringStream) const
{
  aStringStream << "[" << mRefPtr << "] Into Luminance Source (DT: " << mDT << ")";
=======
inline void RecordedIntoLuminanceSource::OutputSimpleEventInfo(
    std::stringstream& aStringStream) const {
  aStringStream << "[" << mRefPtr << "] Into Luminance Source (DT: " << mDT
                << ")";
}

inline bool RecordedFlush::PlayEvent(Translator* aTranslator) const {
  aTranslator->LookupDrawTarget(mDT)->Flush();
  return true;
}

template <class S>
void RecordedFlush::Record(S& aStream) const {
  RecordedDrawingEvent::Record(aStream);
}

template <class S>
RecordedFlush::RecordedFlush(S& aStream)
    : RecordedDrawingEvent(FLUSH, aStream) {}

inline void RecordedFlush::OutputSimpleEventInfo(
    std::stringstream& aStringStream) const {
  aStringStream << "[" << mDT << "] Flush";
>>>>>>> upstream-releases
}

<<<<<<< HEAD
inline bool RecordedSnapshot::PlayEvent(Translator *aTranslator) const {
||||||| merged common ancestors
inline bool
RecordedSnapshot::PlayEvent(Translator *aTranslator) const
{
=======
inline bool RecordedDetachAllSnapshots::PlayEvent(
    Translator* aTranslator) const {
  aTranslator->LookupDrawTarget(mDT)->DetachAllSnapshots();
  return true;
}

template <class S>
void RecordedDetachAllSnapshots::Record(S& aStream) const {
  RecordedDrawingEvent::Record(aStream);
}

template <class S>
RecordedDetachAllSnapshots::RecordedDetachAllSnapshots(S& aStream)
    : RecordedDrawingEvent(DETACHALLSNAPSHOTS, aStream) {}

inline void RecordedDetachAllSnapshots::OutputSimpleEventInfo(
    std::stringstream& aStringStream) const {
  aStringStream << "[" << mDT << "] DetachAllSnapshots";
}

inline bool RecordedSnapshot::PlayEvent(Translator* aTranslator) const {
>>>>>>> upstream-releases
  RefPtr<SourceSurface> src = aTranslator->LookupDrawTarget(mDT)->Snapshot();
  aTranslator->AddSourceSurface(mRefPtr, src);
  return true;
}

<<<<<<< HEAD
template <class S>
void RecordedSnapshot::Record(S &aStream) const {
||||||| merged common ancestors
template<class S>
void
RecordedSnapshot::Record(S &aStream) const
{
=======
template <class S>
void RecordedSnapshot::Record(S& aStream) const {
>>>>>>> upstream-releases
  WriteElement(aStream, mRefPtr);
  WriteElement(aStream, mDT);
}

<<<<<<< HEAD
template <class S>
RecordedSnapshot::RecordedSnapshot(S &aStream)
    : RecordedEventDerived(SNAPSHOT) {
||||||| merged common ancestors
template<class S>
RecordedSnapshot::RecordedSnapshot(S &aStream)
  : RecordedEventDerived(SNAPSHOT)
{
=======
template <class S>
RecordedSnapshot::RecordedSnapshot(S& aStream)
    : RecordedEventDerived(SNAPSHOT) {
>>>>>>> upstream-releases
  ReadElement(aStream, mRefPtr);
  ReadElement(aStream, mDT);
}

<<<<<<< HEAD
inline void RecordedSnapshot::OutputSimpleEventInfo(
    std::stringstream &aStringStream) const {
||||||| merged common ancestors
inline void
RecordedSnapshot::OutputSimpleEventInfo(std::stringstream &aStringStream) const
{
=======
inline void RecordedSnapshot::OutputSimpleEventInfo(
    std::stringstream& aStringStream) const {
>>>>>>> upstream-releases
  aStringStream << "[" << mRefPtr << "] Snapshot Created (DT: " << mDT << ")";
}

inline RecordedFontData::~RecordedFontData() { delete[] mData; }

<<<<<<< HEAD
inline bool RecordedFontData::PlayEvent(Translator *aTranslator) const {
||||||| merged common ancestors
inline bool
RecordedFontData::PlayEvent(Translator *aTranslator) const
{
=======
inline bool RecordedFontData::PlayEvent(Translator* aTranslator) const {
>>>>>>> upstream-releases
  if (!mData) {
    return false;
  }

  RefPtr<NativeFontResource> fontResource = Factory::CreateNativeFontResource(
      mData, mFontDetails.size,
      aTranslator->GetReferenceDrawTarget()->GetBackendType(), mType,
      aTranslator->GetFontContext());
  if (!fontResource) {
    return false;
  }

  aTranslator->AddNativeFontResource(mFontDetails.fontDataKey, fontResource);
  return true;
}

<<<<<<< HEAD
template <class S>
void RecordedFontData::Record(S &aStream) const {
||||||| merged common ancestors
template<class S>
void
RecordedFontData::Record(S &aStream) const
{
=======
template <class S>
void RecordedFontData::Record(S& aStream) const {
>>>>>>> upstream-releases
  MOZ_ASSERT(mGetFontFileDataSucceeded);

  WriteElement(aStream, mType);
  WriteElement(aStream, mFontDetails.fontDataKey);
  WriteElement(aStream, mFontDetails.size);
  aStream.write((const char *)mData, mFontDetails.size);
}

<<<<<<< HEAD
inline void RecordedFontData::OutputSimpleEventInfo(
    std::stringstream &aStringStream) const {
||||||| merged common ancestors
inline void
RecordedFontData::OutputSimpleEventInfo(std::stringstream &aStringStream) const
{
=======
inline void RecordedFontData::OutputSimpleEventInfo(
    std::stringstream& aStringStream) const {
>>>>>>> upstream-releases
  aStringStream << "Font Data of size " << mFontDetails.size;
}

<<<<<<< HEAD
inline void RecordedFontData::SetFontData(const uint8_t *aData, uint32_t aSize,
                                          uint32_t aIndex) {
||||||| merged common ancestors
inline void
RecordedFontData::SetFontData(const uint8_t *aData, uint32_t aSize, uint32_t aIndex)
{
=======
inline void RecordedFontData::SetFontData(const uint8_t* aData, uint32_t aSize,
                                          uint32_t aIndex) {
>>>>>>> upstream-releases
  mData = new (fallible) uint8_t[aSize];
  if (!mData) {
    gfxCriticalNote
        << "RecordedFontData failed to allocate data for recording of size "
        << aSize;
  } else {
    memcpy(mData, aData, aSize);
  }
  mFontDetails.fontDataKey = SFNTData::GetUniqueKey(aData, aSize, 0, nullptr);
  mFontDetails.size = aSize;
  mFontDetails.index = aIndex;
}

<<<<<<< HEAD
inline bool RecordedFontData::GetFontDetails(RecordedFontDetails &fontDetails) {
||||||| merged common ancestors
inline bool
RecordedFontData::GetFontDetails(RecordedFontDetails& fontDetails)
{
=======
inline bool RecordedFontData::GetFontDetails(RecordedFontDetails& fontDetails) {
>>>>>>> upstream-releases
  if (!mGetFontFileDataSucceeded) {
    return false;
  }

  fontDetails.fontDataKey = mFontDetails.fontDataKey;
  fontDetails.size = mFontDetails.size;
  fontDetails.index = mFontDetails.index;
  return true;
}

<<<<<<< HEAD
template <class S>
RecordedFontData::RecordedFontData(S &aStream)
    : RecordedEventDerived(FONTDATA), mType(FontType::UNKNOWN), mData(nullptr) {
  ReadElement(aStream, mType);
||||||| merged common ancestors
template<class S>
RecordedFontData::RecordedFontData(S &aStream)
  : RecordedEventDerived(FONTDATA)
  , mType(FontType::UNKNOWN)
  , mData(nullptr)
{
  ReadElement(aStream, mType);
=======
template <class S>
RecordedFontData::RecordedFontData(S& aStream)
    : RecordedEventDerived(FONTDATA), mType(FontType::UNKNOWN), mData(nullptr) {
  ReadElementConstrained(aStream, mType, FontType::DWRITE, FontType::UNKNOWN);
>>>>>>> upstream-releases
  ReadElement(aStream, mFontDetails.fontDataKey);
  ReadElement(aStream, mFontDetails.size);
  mData = new (fallible) uint8_t[mFontDetails.size];
  if (!mData) {
    gfxCriticalNote
        << "RecordedFontData failed to allocate data for playback of size "
        << mFontDetails.size;
  } else {
    aStream.read((char *)mData, mFontDetails.size);
  }
}

inline RecordedFontDescriptor::~RecordedFontDescriptor() {}

<<<<<<< HEAD
inline bool RecordedFontDescriptor::PlayEvent(Translator *aTranslator) const {
  RefPtr<UnscaledFont> font = Factory::CreateUnscaledFontFromFontDescriptor(
      mType, mData.data(), mData.size(), mIndex);
||||||| merged common ancestors
inline bool
RecordedFontDescriptor::PlayEvent(Translator *aTranslator) const
{
  RefPtr<UnscaledFont> font =
    Factory::CreateUnscaledFontFromFontDescriptor(mType, mData.data(), mData.size(), mIndex);
=======
inline bool RecordedFontDescriptor::PlayEvent(Translator* aTranslator) const {
  RefPtr<UnscaledFont> font = Factory::CreateUnscaledFontFromFontDescriptor(
      mType, mData.data(), mData.size(), mIndex);
>>>>>>> upstream-releases
  if (!font) {
    gfxDevCrash(LogReason::InvalidFont)
        << "Failed creating UnscaledFont of type " << int(mType)
        << " from font descriptor";
    return false;
  }

  aTranslator->AddUnscaledFont(mRefPtr, font);
  return true;
}

<<<<<<< HEAD
template <class S>
void RecordedFontDescriptor::Record(S &aStream) const {
||||||| merged common ancestors
template<class S>
void
RecordedFontDescriptor::Record(S &aStream) const
{
=======
template <class S>
void RecordedFontDescriptor::Record(S& aStream) const {
>>>>>>> upstream-releases
  MOZ_ASSERT(mHasDesc);
  WriteElement(aStream, mType);
  WriteElement(aStream, mRefPtr);
  WriteElement(aStream, mIndex);
  WriteElement(aStream, (size_t)mData.size());
  aStream.write((char *)mData.data(), mData.size());
}

<<<<<<< HEAD
inline void RecordedFontDescriptor::OutputSimpleEventInfo(
    std::stringstream &aStringStream) const {
||||||| merged common ancestors
inline void
RecordedFontDescriptor::OutputSimpleEventInfo(std::stringstream &aStringStream) const
{
=======
inline void RecordedFontDescriptor::OutputSimpleEventInfo(
    std::stringstream& aStringStream) const {
>>>>>>> upstream-releases
  aStringStream << "[" << mRefPtr << "] Font Descriptor";
}

<<<<<<< HEAD
inline void RecordedFontDescriptor::SetFontDescriptor(const uint8_t *aData,
                                                      uint32_t aSize,
                                                      uint32_t aIndex) {
||||||| merged common ancestors
inline void
RecordedFontDescriptor::SetFontDescriptor(const uint8_t* aData, uint32_t aSize, uint32_t aIndex)
{
=======
inline void RecordedFontDescriptor::SetFontDescriptor(const uint8_t* aData,
                                                      uint32_t aSize,
                                                      uint32_t aIndex) {
>>>>>>> upstream-releases
  mData.assign(aData, aData + aSize);
  mIndex = aIndex;
}

<<<<<<< HEAD
template <class S>
RecordedFontDescriptor::RecordedFontDescriptor(S &aStream)
    : RecordedEventDerived(FONTDESC) {
  ReadElement(aStream, mType);
||||||| merged common ancestors
template<class S>
RecordedFontDescriptor::RecordedFontDescriptor(S &aStream)
  : RecordedEventDerived(FONTDESC)
{
  ReadElement(aStream, mType);
=======
template <class S>
RecordedFontDescriptor::RecordedFontDescriptor(S& aStream)
    : RecordedEventDerived(FONTDESC) {
  ReadElementConstrained(aStream, mType, FontType::DWRITE, FontType::UNKNOWN);
>>>>>>> upstream-releases
  ReadElement(aStream, mRefPtr);
  ReadElement(aStream, mIndex);

  size_t size;
  ReadElement(aStream, size);
  mData.resize(size);
  aStream.read((char *)mData.data(), size);
}

<<<<<<< HEAD
inline bool RecordedUnscaledFontCreation::PlayEvent(
    Translator *aTranslator) const {
  NativeFontResource *fontResource =
      aTranslator->LookupNativeFontResource(mFontDataKey);
||||||| merged common ancestors
inline bool
RecordedUnscaledFontCreation::PlayEvent(Translator *aTranslator) const
{
  NativeFontResource *fontResource = aTranslator->LookupNativeFontResource(mFontDataKey);
=======
inline bool RecordedUnscaledFontCreation::PlayEvent(
    Translator* aTranslator) const {
  NativeFontResource* fontResource =
      aTranslator->LookupNativeFontResource(mFontDataKey);
>>>>>>> upstream-releases
  if (!fontResource) {
    gfxDevCrash(LogReason::NativeFontResourceNotFound)
        << "NativeFontResource lookup failed for key |" << hexa(mFontDataKey)
        << "|.";
    return false;
  }

  RefPtr<UnscaledFont> unscaledFont = fontResource->CreateUnscaledFont(
      mIndex, mInstanceData.data(), mInstanceData.size());
  aTranslator->AddUnscaledFont(mRefPtr, unscaledFont);
  return true;
}

<<<<<<< HEAD
template <class S>
void RecordedUnscaledFontCreation::Record(S &aStream) const {
||||||| merged common ancestors
template<class S>
void
RecordedUnscaledFontCreation::Record(S &aStream) const
{
=======
template <class S>
void RecordedUnscaledFontCreation::Record(S& aStream) const {
>>>>>>> upstream-releases
  WriteElement(aStream, mRefPtr);
  WriteElement(aStream, mFontDataKey);
  WriteElement(aStream, mIndex);
  WriteElement(aStream, (size_t)mInstanceData.size());
  aStream.write((char *)mInstanceData.data(), mInstanceData.size());
}

<<<<<<< HEAD
inline void RecordedUnscaledFontCreation::OutputSimpleEventInfo(
    std::stringstream &aStringStream) const {
||||||| merged common ancestors
inline void
RecordedUnscaledFontCreation::OutputSimpleEventInfo(std::stringstream &aStringStream) const
{
=======
inline void RecordedUnscaledFontCreation::OutputSimpleEventInfo(
    std::stringstream& aStringStream) const {
>>>>>>> upstream-releases
  aStringStream << "[" << mRefPtr << "] UnscaledFont Created";
}

<<<<<<< HEAD
inline void RecordedUnscaledFontCreation::SetFontInstanceData(
    const uint8_t *aData, uint32_t aSize) {
||||||| merged common ancestors
inline void
RecordedUnscaledFontCreation::SetFontInstanceData(const uint8_t *aData, uint32_t aSize)
{
=======
inline void RecordedUnscaledFontCreation::SetFontInstanceData(
    const uint8_t* aData, uint32_t aSize) {
>>>>>>> upstream-releases
  mInstanceData.assign(aData, aData + aSize);
}

<<<<<<< HEAD
template <class S>
RecordedUnscaledFontCreation::RecordedUnscaledFontCreation(S &aStream)
    : RecordedEventDerived(UNSCALEDFONTCREATION) {
||||||| merged common ancestors
template<class S>
RecordedUnscaledFontCreation::RecordedUnscaledFontCreation(S &aStream)
  : RecordedEventDerived(UNSCALEDFONTCREATION)
{
=======
template <class S>
RecordedUnscaledFontCreation::RecordedUnscaledFontCreation(S& aStream)
    : RecordedEventDerived(UNSCALEDFONTCREATION) {
>>>>>>> upstream-releases
  ReadElement(aStream, mRefPtr);
  ReadElement(aStream, mFontDataKey);
  ReadElement(aStream, mIndex);

  size_t size;
  ReadElement(aStream, size);
  mInstanceData.resize(size);
  aStream.read((char *)mInstanceData.data(), size);
}

<<<<<<< HEAD
inline bool RecordedUnscaledFontDestruction::PlayEvent(
    Translator *aTranslator) const {
||||||| merged common ancestors
inline bool
RecordedUnscaledFontDestruction::PlayEvent(Translator *aTranslator) const
{
=======
inline bool RecordedUnscaledFontDestruction::PlayEvent(
    Translator* aTranslator) const {
>>>>>>> upstream-releases
  aTranslator->RemoveUnscaledFont(mRefPtr);
  return true;
}

<<<<<<< HEAD
template <class S>
void RecordedUnscaledFontDestruction::Record(S &aStream) const {
||||||| merged common ancestors
template<class S>
void
RecordedUnscaledFontDestruction::Record(S &aStream) const
{
=======
template <class S>
void RecordedUnscaledFontDestruction::Record(S& aStream) const {
>>>>>>> upstream-releases
  WriteElement(aStream, mRefPtr);
}

<<<<<<< HEAD
template <class S>
RecordedUnscaledFontDestruction::RecordedUnscaledFontDestruction(S &aStream)
    : RecordedEventDerived(UNSCALEDFONTDESTRUCTION) {
||||||| merged common ancestors
template<class S>
RecordedUnscaledFontDestruction::RecordedUnscaledFontDestruction(S &aStream)
  : RecordedEventDerived(UNSCALEDFONTDESTRUCTION)
{
=======
template <class S>
RecordedUnscaledFontDestruction::RecordedUnscaledFontDestruction(S& aStream)
    : RecordedEventDerived(UNSCALEDFONTDESTRUCTION) {
>>>>>>> upstream-releases
  ReadElement(aStream, mRefPtr);
}

<<<<<<< HEAD
inline void RecordedUnscaledFontDestruction::OutputSimpleEventInfo(
    std::stringstream &aStringStream) const {
||||||| merged common ancestors
inline void
RecordedUnscaledFontDestruction::OutputSimpleEventInfo(std::stringstream &aStringStream) const
{
=======
inline void RecordedUnscaledFontDestruction::OutputSimpleEventInfo(
    std::stringstream& aStringStream) const {
>>>>>>> upstream-releases
  aStringStream << "[" << mRefPtr << "] UnscaledFont Destroyed";
}

<<<<<<< HEAD
inline bool RecordedScaledFontCreation::PlayEvent(
    Translator *aTranslator) const {
  UnscaledFont *unscaledFont = aTranslator->LookupUnscaledFont(mUnscaledFont);
||||||| merged common ancestors
inline bool
RecordedScaledFontCreation::PlayEvent(Translator *aTranslator) const
{
  UnscaledFont* unscaledFont = aTranslator->LookupUnscaledFont(mUnscaledFont);
=======
inline bool RecordedScaledFontCreation::PlayEvent(
    Translator* aTranslator) const {
  UnscaledFont* unscaledFont = aTranslator->LookupUnscaledFont(mUnscaledFont);
>>>>>>> upstream-releases
  if (!unscaledFont) {
    gfxDevCrash(LogReason::UnscaledFontNotFound)
        << "UnscaledFont lookup failed for key |" << hexa(mUnscaledFont)
        << "|.";
    return false;
  }

  RefPtr<ScaledFont> scaledFont = unscaledFont->CreateScaledFont(
      mGlyphSize, mInstanceData.data(), mInstanceData.size(),
      mVariations.data(), mVariations.size());
  aTranslator->AddScaledFont(mRefPtr, scaledFont);
  return true;
}

<<<<<<< HEAD
template <class S>
void RecordedScaledFontCreation::Record(S &aStream) const {
||||||| merged common ancestors
template<class S>
void
RecordedScaledFontCreation::Record(S &aStream) const
{
=======
template <class S>
void RecordedScaledFontCreation::Record(S& aStream) const {
>>>>>>> upstream-releases
  WriteElement(aStream, mRefPtr);
  WriteElement(aStream, mUnscaledFont);
  WriteElement(aStream, mGlyphSize);
  WriteElement(aStream, (size_t)mInstanceData.size());
  aStream.write((char *)mInstanceData.data(), mInstanceData.size());
  WriteElement(aStream, (size_t)mVariations.size());
<<<<<<< HEAD
  aStream.write((char *)mVariations.data(),
                sizeof(FontVariation) * mVariations.size());
||||||| merged common ancestors
  aStream.write((char*)mVariations.data(), sizeof(FontVariation) * mVariations.size());
=======
  aStream.write((char*)mVariations.data(),
                sizeof(FontVariation) * mVariations.size());
>>>>>>> upstream-releases
}

<<<<<<< HEAD
inline void RecordedScaledFontCreation::OutputSimpleEventInfo(
    std::stringstream &aStringStream) const {
||||||| merged common ancestors
inline void
RecordedScaledFontCreation::OutputSimpleEventInfo(std::stringstream &aStringStream) const
{
=======
inline void RecordedScaledFontCreation::OutputSimpleEventInfo(
    std::stringstream& aStringStream) const {
>>>>>>> upstream-releases
  aStringStream << "[" << mRefPtr << "] ScaledFont Created";
}

<<<<<<< HEAD
inline void RecordedScaledFontCreation::SetFontInstanceData(
    const uint8_t *aData, uint32_t aSize, const FontVariation *aVariations,
    uint32_t aNumVariations) {
||||||| merged common ancestors
inline void
RecordedScaledFontCreation::SetFontInstanceData(const uint8_t *aData, uint32_t aSize,
                                                const FontVariation* aVariations, uint32_t aNumVariations)
{
=======
inline void RecordedScaledFontCreation::SetFontInstanceData(
    const uint8_t* aData, uint32_t aSize, const FontVariation* aVariations,
    uint32_t aNumVariations) {
>>>>>>> upstream-releases
  mInstanceData.assign(aData, aData + aSize);
  mVariations.assign(aVariations, aVariations + aNumVariations);
}

<<<<<<< HEAD
template <class S>
RecordedScaledFontCreation::RecordedScaledFontCreation(S &aStream)
    : RecordedEventDerived(SCALEDFONTCREATION) {
||||||| merged common ancestors
template<class S>
RecordedScaledFontCreation::RecordedScaledFontCreation(S &aStream)
  : RecordedEventDerived(SCALEDFONTCREATION)
{
=======
template <class S>
RecordedScaledFontCreation::RecordedScaledFontCreation(S& aStream)
    : RecordedEventDerived(SCALEDFONTCREATION) {
>>>>>>> upstream-releases
  ReadElement(aStream, mRefPtr);
  ReadElement(aStream, mUnscaledFont);
  ReadElement(aStream, mGlyphSize);

  size_t size;
  ReadElement(aStream, size);
  mInstanceData.resize(size);
  aStream.read((char *)mInstanceData.data(), size);
  size_t numVariations;
  ReadElement(aStream, numVariations);
  mVariations.resize(numVariations);
<<<<<<< HEAD
  aStream.read((char *)mVariations.data(),
               sizeof(FontVariation) * numVariations);
||||||| merged common ancestors
  aStream.read((char*)mVariations.data(), sizeof(FontVariation) * numVariations);
=======
  aStream.read((char*)mVariations.data(),
               sizeof(FontVariation) * numVariations);
>>>>>>> upstream-releases
}

<<<<<<< HEAD
inline bool RecordedScaledFontDestruction::PlayEvent(
    Translator *aTranslator) const {
||||||| merged common ancestors
inline bool
RecordedScaledFontDestruction::PlayEvent(Translator *aTranslator) const
{
=======
inline bool RecordedScaledFontDestruction::PlayEvent(
    Translator* aTranslator) const {
>>>>>>> upstream-releases
  aTranslator->RemoveScaledFont(mRefPtr);
  return true;
}

<<<<<<< HEAD
template <class S>
void RecordedScaledFontDestruction::Record(S &aStream) const {
||||||| merged common ancestors
template<class S>
void
RecordedScaledFontDestruction::Record(S &aStream) const
{
=======
template <class S>
void RecordedScaledFontDestruction::Record(S& aStream) const {
>>>>>>> upstream-releases
  WriteElement(aStream, mRefPtr);
}

<<<<<<< HEAD
template <class S>
RecordedScaledFontDestruction::RecordedScaledFontDestruction(S &aStream)
    : RecordedEventDerived(SCALEDFONTDESTRUCTION) {
||||||| merged common ancestors
template<class S>
RecordedScaledFontDestruction::RecordedScaledFontDestruction(S &aStream)
  : RecordedEventDerived(SCALEDFONTDESTRUCTION)
{
=======
template <class S>
RecordedScaledFontDestruction::RecordedScaledFontDestruction(S& aStream)
    : RecordedEventDerived(SCALEDFONTDESTRUCTION) {
>>>>>>> upstream-releases
  ReadElement(aStream, mRefPtr);
}

<<<<<<< HEAD
inline void RecordedScaledFontDestruction::OutputSimpleEventInfo(
    std::stringstream &aStringStream) const {
||||||| merged common ancestors
inline void
RecordedScaledFontDestruction::OutputSimpleEventInfo(std::stringstream &aStringStream) const
{
=======
inline void RecordedScaledFontDestruction::OutputSimpleEventInfo(
    std::stringstream& aStringStream) const {
>>>>>>> upstream-releases
  aStringStream << "[" << mRefPtr << "] ScaledFont Destroyed";
}

<<<<<<< HEAD
inline bool RecordedMaskSurface::PlayEvent(Translator *aTranslator) const {
  aTranslator->LookupDrawTarget(mDT)->MaskSurface(
      *GenericPattern(mPattern, aTranslator),
      aTranslator->LookupSourceSurface(mRefMask), mOffset, mOptions);
||||||| merged common ancestors
inline bool
RecordedMaskSurface::PlayEvent(Translator *aTranslator) const
{
  aTranslator->LookupDrawTarget(mDT)->
    MaskSurface(*GenericPattern(mPattern, aTranslator),
                aTranslator->LookupSourceSurface(mRefMask),
                mOffset, mOptions);
=======
inline bool RecordedMaskSurface::PlayEvent(Translator* aTranslator) const {
  aTranslator->LookupDrawTarget(mDT)->MaskSurface(
      *GenericPattern(mPattern, aTranslator),
      aTranslator->LookupSourceSurface(mRefMask), mOffset, mOptions);
>>>>>>> upstream-releases
  return true;
}

<<<<<<< HEAD
template <class S>
void RecordedMaskSurface::Record(S &aStream) const {
||||||| merged common ancestors
template<class S>
void
RecordedMaskSurface::Record(S &aStream) const
{
=======
template <class S>
void RecordedMaskSurface::Record(S& aStream) const {
>>>>>>> upstream-releases
  RecordedDrawingEvent::Record(aStream);
  RecordPatternData(aStream, mPattern);
  WriteElement(aStream, mRefMask);
  WriteElement(aStream, mOffset);
  WriteElement(aStream, mOptions);
}

<<<<<<< HEAD
template <class S>
RecordedMaskSurface::RecordedMaskSurface(S &aStream)
    : RecordedDrawingEvent(MASKSURFACE, aStream) {
||||||| merged common ancestors
template<class S>
RecordedMaskSurface::RecordedMaskSurface(S &aStream)
  : RecordedDrawingEvent(MASKSURFACE, aStream)
{
=======
template <class S>
RecordedMaskSurface::RecordedMaskSurface(S& aStream)
    : RecordedDrawingEvent(MASKSURFACE, aStream) {
>>>>>>> upstream-releases
  ReadPatternData(aStream, mPattern);
  ReadElement(aStream, mRefMask);
  ReadElement(aStream, mOffset);
  ReadDrawOptions(aStream, mOptions);
}

<<<<<<< HEAD
inline void RecordedMaskSurface::OutputSimpleEventInfo(
    std::stringstream &aStringStream) const {
  aStringStream << "[" << mDT << "] MaskSurface (" << mRefMask << ")  Offset: ("
                << mOffset.x << "x" << mOffset.y << ") Pattern: ";
||||||| merged common ancestors
inline void
RecordedMaskSurface::OutputSimpleEventInfo(std::stringstream &aStringStream) const
{
  aStringStream << "[" << mDT << "] MaskSurface (" << mRefMask << ")  Offset: (" << mOffset.x << "x" << mOffset.y << ") Pattern: ";
=======
inline void RecordedMaskSurface::OutputSimpleEventInfo(
    std::stringstream& aStringStream) const {
  aStringStream << "[" << mDT << "] MaskSurface (" << mRefMask << ")  Offset: ("
                << mOffset.x << "x" << mOffset.y << ") Pattern: ";
>>>>>>> upstream-releases
  OutputSimplePatternInfo(mPattern, aStringStream);
}

<<<<<<< HEAD
template <typename T>
void ReplaySetAttribute(FilterNode *aNode, uint32_t aIndex, T aValue) {
||||||| merged common ancestors
template<typename T>
void
ReplaySetAttribute(FilterNode *aNode, uint32_t aIndex, T aValue)
{
=======
template <typename T>
void ReplaySetAttribute(FilterNode* aNode, uint32_t aIndex, T aValue) {
>>>>>>> upstream-releases
  aNode->SetAttribute(aIndex, aValue);
}

<<<<<<< HEAD
inline bool RecordedFilterNodeSetAttribute::PlayEvent(
    Translator *aTranslator) const {
#define REPLAY_SET_ATTRIBUTE(type, argtype)                          \
  case ARGTYPE_##argtype:                                            \
    ReplaySetAttribute(aTranslator->LookupFilterNode(mNode), mIndex, \
                       *(type *)&mPayload.front());                  \
    break
||||||| merged common ancestors
inline bool
RecordedFilterNodeSetAttribute::PlayEvent(Translator *aTranslator) const
{
#define REPLAY_SET_ATTRIBUTE(type, argtype) \
  case ARGTYPE_##argtype: \
  ReplaySetAttribute(aTranslator->LookupFilterNode(mNode), mIndex, *(type*)&mPayload.front()); \
  break
=======
inline bool RecordedFilterNodeSetAttribute::PlayEvent(
    Translator* aTranslator) const {
#define REPLAY_SET_ATTRIBUTE(type, argtype)                          \
  case ARGTYPE_##argtype:                                            \
    ReplaySetAttribute(aTranslator->LookupFilterNode(mNode), mIndex, \
                       *(type*)&mPayload.front());                   \
    break
>>>>>>> upstream-releases

  switch (mArgType) {
    REPLAY_SET_ATTRIBUTE(bool, BOOL);
    REPLAY_SET_ATTRIBUTE(uint32_t, UINT32);
    REPLAY_SET_ATTRIBUTE(Float, FLOAT);
    REPLAY_SET_ATTRIBUTE(Size, SIZE);
    REPLAY_SET_ATTRIBUTE(IntSize, INTSIZE);
    REPLAY_SET_ATTRIBUTE(IntPoint, INTPOINT);
    REPLAY_SET_ATTRIBUTE(Rect, RECT);
    REPLAY_SET_ATTRIBUTE(IntRect, INTRECT);
    REPLAY_SET_ATTRIBUTE(Point, POINT);
    REPLAY_SET_ATTRIBUTE(Matrix, MATRIX);
    REPLAY_SET_ATTRIBUTE(Matrix5x4, MATRIX5X4);
    REPLAY_SET_ATTRIBUTE(Point3D, POINT3D);
    REPLAY_SET_ATTRIBUTE(Color, COLOR);
<<<<<<< HEAD
    case ARGTYPE_FLOAT_ARRAY:
      aTranslator->LookupFilterNode(mNode)->SetAttribute(
          mIndex, reinterpret_cast<const Float *>(&mPayload.front()),
          mPayload.size() / sizeof(Float));
      break;
||||||| merged common ancestors
  case ARGTYPE_FLOAT_ARRAY:
    aTranslator->LookupFilterNode(mNode)->SetAttribute(
      mIndex,
      reinterpret_cast<const Float*>(&mPayload.front()),
      mPayload.size() / sizeof(Float));
    break;
=======
    case ARGTYPE_FLOAT_ARRAY:
      aTranslator->LookupFilterNode(mNode)->SetAttribute(
          mIndex, reinterpret_cast<const Float*>(&mPayload.front()),
          mPayload.size() / sizeof(Float));
      break;
>>>>>>> upstream-releases
  }

  return true;
}

<<<<<<< HEAD
template <class S>
void RecordedFilterNodeSetAttribute::Record(S &aStream) const {
||||||| merged common ancestors
template<class S>
void
RecordedFilterNodeSetAttribute::Record(S &aStream) const
{
=======
template <class S>
void RecordedFilterNodeSetAttribute::Record(S& aStream) const {
>>>>>>> upstream-releases
  WriteElement(aStream, mNode);
  WriteElement(aStream, mIndex);
  WriteElement(aStream, mArgType);
  WriteElement(aStream, uint64_t(mPayload.size()));
  aStream.write((const char *)&mPayload.front(), mPayload.size());
}

<<<<<<< HEAD
template <class S>
RecordedFilterNodeSetAttribute::RecordedFilterNodeSetAttribute(S &aStream)
    : RecordedEventDerived(FILTERNODESETATTRIBUTE) {
||||||| merged common ancestors
template<class S>
RecordedFilterNodeSetAttribute::RecordedFilterNodeSetAttribute(S &aStream)
  : RecordedEventDerived(FILTERNODESETATTRIBUTE)
{
=======
template <class S>
RecordedFilterNodeSetAttribute::RecordedFilterNodeSetAttribute(S& aStream)
    : RecordedEventDerived(FILTERNODESETATTRIBUTE) {
>>>>>>> upstream-releases
  ReadElement(aStream, mNode);
  ReadElement(aStream, mIndex);
  ReadElementConstrained(aStream, mArgType, ArgType::ARGTYPE_UINT32,
                         ArgType::ARGTYPE_FLOAT_ARRAY);
  uint64_t size;
  ReadElement(aStream, size);
  mPayload.resize(size_t(size));
  aStream.read((char *)&mPayload.front(), size);
}

<<<<<<< HEAD
inline void RecordedFilterNodeSetAttribute::OutputSimpleEventInfo(
    std::stringstream &aStringStream) const {
||||||| merged common ancestors
inline void
RecordedFilterNodeSetAttribute::OutputSimpleEventInfo(std::stringstream &aStringStream) const
{
=======
inline void RecordedFilterNodeSetAttribute::OutputSimpleEventInfo(
    std::stringstream& aStringStream) const {
>>>>>>> upstream-releases
  aStringStream << "[" << mNode << "] SetAttribute (" << mIndex << ")";
}

<<<<<<< HEAD
inline bool RecordedFilterNodeSetInput::PlayEvent(
    Translator *aTranslator) const {
||||||| merged common ancestors
inline bool
RecordedFilterNodeSetInput::PlayEvent(Translator *aTranslator) const
{
=======
inline bool RecordedFilterNodeSetInput::PlayEvent(
    Translator* aTranslator) const {
>>>>>>> upstream-releases
  if (mInputFilter) {
    aTranslator->LookupFilterNode(mNode)->SetInput(
        mIndex, aTranslator->LookupFilterNode(mInputFilter));
  } else {
    aTranslator->LookupFilterNode(mNode)->SetInput(
        mIndex, aTranslator->LookupSourceSurface(mInputSurface));
  }

  return true;
}

<<<<<<< HEAD
template <class S>
void RecordedFilterNodeSetInput::Record(S &aStream) const {
||||||| merged common ancestors
template<class S>
void
RecordedFilterNodeSetInput::Record(S &aStream) const
{
=======
template <class S>
void RecordedFilterNodeSetInput::Record(S& aStream) const {
>>>>>>> upstream-releases
  WriteElement(aStream, mNode);
  WriteElement(aStream, mIndex);
  WriteElement(aStream, mInputFilter);
  WriteElement(aStream, mInputSurface);
}

<<<<<<< HEAD
template <class S>
RecordedFilterNodeSetInput::RecordedFilterNodeSetInput(S &aStream)
    : RecordedEventDerived(FILTERNODESETINPUT) {
||||||| merged common ancestors
template<class S>
RecordedFilterNodeSetInput::RecordedFilterNodeSetInput(S &aStream)
  : RecordedEventDerived(FILTERNODESETINPUT)
{
=======
template <class S>
RecordedFilterNodeSetInput::RecordedFilterNodeSetInput(S& aStream)
    : RecordedEventDerived(FILTERNODESETINPUT) {
>>>>>>> upstream-releases
  ReadElement(aStream, mNode);
  ReadElement(aStream, mIndex);
  ReadElement(aStream, mInputFilter);
  ReadElement(aStream, mInputSurface);
}

<<<<<<< HEAD
inline void RecordedFilterNodeSetInput::OutputSimpleEventInfo(
    std::stringstream &aStringStream) const {
||||||| merged common ancestors
inline void
RecordedFilterNodeSetInput::OutputSimpleEventInfo(std::stringstream &aStringStream) const
{
=======
inline void RecordedFilterNodeSetInput::OutputSimpleEventInfo(
    std::stringstream& aStringStream) const {
>>>>>>> upstream-releases
  aStringStream << "[" << mNode << "] SetAttribute (" << mIndex << ", ";

  if (mInputFilter) {
    aStringStream << "Filter: " << mInputFilter;
  } else {
    aStringStream << "Surface: " << mInputSurface;
  }

  aStringStream << ")";
}

<<<<<<< HEAD
#define LOAD_EVENT_TYPE(_typeenum, _class) \
  case _typeenum:                          \
    return new _class(aStream)

#define FOR_EACH_EVENT(f)                                        \
  f(DRAWTARGETCREATION, RecordedDrawTargetCreation);             \
  f(DRAWTARGETDESTRUCTION, RecordedDrawTargetDestruction);       \
  f(FILLRECT, RecordedFillRect);                                 \
  f(STROKERECT, RecordedStrokeRect);                             \
  f(STROKELINE, RecordedStrokeLine);                             \
  f(CLEARRECT, RecordedClearRect);                               \
  f(COPYSURFACE, RecordedCopySurface);                           \
  f(SETTRANSFORM, RecordedSetTransform);                         \
  f(PUSHCLIPRECT, RecordedPushClipRect);                         \
  f(PUSHCLIP, RecordedPushClip);                                 \
  f(POPCLIP, RecordedPopClip);                                   \
  f(FILL, RecordedFill);                                         \
  f(FILLGLYPHS, RecordedFillGlyphs);                             \
  f(MASK, RecordedMask);                                         \
  f(STROKE, RecordedStroke);                                     \
  f(DRAWSURFACE, RecordedDrawSurface);                           \
  f(DRAWDEPENDENTSURFACE, RecordedDrawDependentSurface);         \
  f(DRAWSURFACEWITHSHADOW, RecordedDrawSurfaceWithShadow);       \
  f(DRAWFILTER, RecordedDrawFilter);                             \
  f(PATHCREATION, RecordedPathCreation);                         \
  f(PATHDESTRUCTION, RecordedPathDestruction);                   \
  f(SOURCESURFACECREATION, RecordedSourceSurfaceCreation);       \
  f(SOURCESURFACEDESTRUCTION, RecordedSourceSurfaceDestruction); \
  f(FILTERNODECREATION, RecordedFilterNodeCreation);             \
  f(FILTERNODEDESTRUCTION, RecordedFilterNodeDestruction);       \
  f(GRADIENTSTOPSCREATION, RecordedGradientStopsCreation);       \
  f(GRADIENTSTOPSDESTRUCTION, RecordedGradientStopsDestruction); \
  f(SNAPSHOT, RecordedSnapshot);                                 \
  f(SCALEDFONTCREATION, RecordedScaledFontCreation);             \
  f(SCALEDFONTDESTRUCTION, RecordedScaledFontDestruction);       \
  f(MASKSURFACE, RecordedMaskSurface);                           \
  f(FILTERNODESETATTRIBUTE, RecordedFilterNodeSetAttribute);     \
  f(FILTERNODESETINPUT, RecordedFilterNodeSetInput);             \
  f(CREATESIMILARDRAWTARGET, RecordedCreateSimilarDrawTarget);   \
  f(CREATECLIPPEDDRAWTARGET, RecordedCreateClippedDrawTarget);   \
  f(FONTDATA, RecordedFontData);                                 \
  f(FONTDESC, RecordedFontDescriptor);                           \
  f(PUSHLAYER, RecordedPushLayer);                               \
  f(PUSHLAYERWITHBLEND, RecordedPushLayerWithBlend);             \
  f(POPLAYER, RecordedPopLayer);                                 \
  f(UNSCALEDFONTCREATION, RecordedUnscaledFontCreation);         \
  f(UNSCALEDFONTDESTRUCTION, RecordedUnscaledFontDestruction);   \
  f(INTOLUMINANCE, RecordedIntoLuminanceSource);                 \
  f(EXTERNALSURFACECREATION, RecordedExternalSurfaceCreation);

template <class S>
RecordedEvent *RecordedEvent::LoadEvent(S &aStream, EventType aType) {
  switch (aType) {
    FOR_EACH_EVENT(LOAD_EVENT_TYPE)
    default:
      return nullptr;
  }
}
||||||| merged common ancestors
#define LOAD_EVENT_TYPE(_typeenum, _class) \
  case _typeenum: return new _class(aStream)

#define FOR_EACH_EVENT(f) \
    f(DRAWTARGETCREATION, RecordedDrawTargetCreation); \
    f(DRAWTARGETDESTRUCTION, RecordedDrawTargetDestruction); \
    f(FILLRECT, RecordedFillRect); \
    f(STROKERECT, RecordedStrokeRect); \
    f(STROKELINE, RecordedStrokeLine); \
    f(CLEARRECT, RecordedClearRect); \
    f(COPYSURFACE, RecordedCopySurface); \
    f(SETTRANSFORM, RecordedSetTransform); \
    f(PUSHCLIPRECT, RecordedPushClipRect); \
    f(PUSHCLIP, RecordedPushClip); \
    f(POPCLIP, RecordedPopClip); \
    f(FILL, RecordedFill); \
    f(FILLGLYPHS, RecordedFillGlyphs); \
    f(MASK, RecordedMask); \
    f(STROKE, RecordedStroke); \
    f(DRAWSURFACE, RecordedDrawSurface); \
    f(DRAWDEPENDENTSURFACE, RecordedDrawDependentSurface); \
    f(DRAWSURFACEWITHSHADOW, RecordedDrawSurfaceWithShadow); \
    f(DRAWFILTER, RecordedDrawFilter); \
    f(PATHCREATION, RecordedPathCreation); \
    f(PATHDESTRUCTION, RecordedPathDestruction); \
    f(SOURCESURFACECREATION, RecordedSourceSurfaceCreation); \
    f(SOURCESURFACEDESTRUCTION, RecordedSourceSurfaceDestruction); \
    f(FILTERNODECREATION, RecordedFilterNodeCreation); \
    f(FILTERNODEDESTRUCTION, RecordedFilterNodeDestruction); \
    f(GRADIENTSTOPSCREATION, RecordedGradientStopsCreation); \
    f(GRADIENTSTOPSDESTRUCTION, RecordedGradientStopsDestruction); \
    f(SNAPSHOT, RecordedSnapshot); \
    f(SCALEDFONTCREATION, RecordedScaledFontCreation); \
    f(SCALEDFONTDESTRUCTION, RecordedScaledFontDestruction); \
    f(MASKSURFACE, RecordedMaskSurface); \
    f(FILTERNODESETATTRIBUTE, RecordedFilterNodeSetAttribute); \
    f(FILTERNODESETINPUT, RecordedFilterNodeSetInput); \
    f(CREATESIMILARDRAWTARGET, RecordedCreateSimilarDrawTarget); \
    f(CREATECLIPPEDDRAWTARGET, RecordedCreateClippedDrawTarget); \
    f(FONTDATA, RecordedFontData); \
    f(FONTDESC, RecordedFontDescriptor); \
    f(PUSHLAYER, RecordedPushLayer); \
    f(PUSHLAYERWITHBLEND, RecordedPushLayerWithBlend); \
    f(POPLAYER, RecordedPopLayer); \
    f(UNSCALEDFONTCREATION, RecordedUnscaledFontCreation); \
    f(UNSCALEDFONTDESTRUCTION, RecordedUnscaledFontDestruction); \
    f(INTOLUMINANCE, RecordedIntoLuminanceSource); \
    f(EXTERNALSURFACECREATION, RecordedExternalSurfaceCreation);

template<class S>
RecordedEvent *
RecordedEvent::LoadEvent(S &aStream, EventType aType)
{
  switch (aType) {
    FOR_EACH_EVENT(LOAD_EVENT_TYPE)
  default:
    return nullptr;
  }
}
=======
#define FOR_EACH_EVENT(f)                                          \
  f(DRAWTARGETCREATION, RecordedDrawTargetCreation);               \
  f(DRAWTARGETDESTRUCTION, RecordedDrawTargetDestruction);         \
  f(FILLRECT, RecordedFillRect);                                   \
  f(STROKERECT, RecordedStrokeRect);                               \
  f(STROKELINE, RecordedStrokeLine);                               \
  f(CLEARRECT, RecordedClearRect);                                 \
  f(COPYSURFACE, RecordedCopySurface);                             \
  f(SETTRANSFORM, RecordedSetTransform);                           \
  f(PUSHCLIPRECT, RecordedPushClipRect);                           \
  f(PUSHCLIP, RecordedPushClip);                                   \
  f(POPCLIP, RecordedPopClip);                                     \
  f(FILL, RecordedFill);                                           \
  f(FILLGLYPHS, RecordedFillGlyphs);                               \
  f(MASK, RecordedMask);                                           \
  f(STROKE, RecordedStroke);                                       \
  f(DRAWSURFACE, RecordedDrawSurface);                             \
  f(DRAWDEPENDENTSURFACE, RecordedDrawDependentSurface);           \
  f(DRAWSURFACEWITHSHADOW, RecordedDrawSurfaceWithShadow);         \
  f(DRAWFILTER, RecordedDrawFilter);                               \
  f(PATHCREATION, RecordedPathCreation);                           \
  f(PATHDESTRUCTION, RecordedPathDestruction);                     \
  f(SOURCESURFACECREATION, RecordedSourceSurfaceCreation);         \
  f(SOURCESURFACEDESTRUCTION, RecordedSourceSurfaceDestruction);   \
  f(FILTERNODECREATION, RecordedFilterNodeCreation);               \
  f(FILTERNODEDESTRUCTION, RecordedFilterNodeDestruction);         \
  f(GRADIENTSTOPSCREATION, RecordedGradientStopsCreation);         \
  f(GRADIENTSTOPSDESTRUCTION, RecordedGradientStopsDestruction);   \
  f(SNAPSHOT, RecordedSnapshot);                                   \
  f(SCALEDFONTCREATION, RecordedScaledFontCreation);               \
  f(SCALEDFONTDESTRUCTION, RecordedScaledFontDestruction);         \
  f(MASKSURFACE, RecordedMaskSurface);                             \
  f(FILTERNODESETATTRIBUTE, RecordedFilterNodeSetAttribute);       \
  f(FILTERNODESETINPUT, RecordedFilterNodeSetInput);               \
  f(CREATESIMILARDRAWTARGET, RecordedCreateSimilarDrawTarget);     \
  f(CREATECLIPPEDDRAWTARGET, RecordedCreateClippedDrawTarget);     \
  f(CREATEDRAWTARGETFORFILTER, RecordedCreateDrawTargetForFilter); \
  f(FONTDATA, RecordedFontData);                                   \
  f(FONTDESC, RecordedFontDescriptor);                             \
  f(PUSHLAYER, RecordedPushLayer);                                 \
  f(PUSHLAYERWITHBLEND, RecordedPushLayerWithBlend);               \
  f(POPLAYER, RecordedPopLayer);                                   \
  f(UNSCALEDFONTCREATION, RecordedUnscaledFontCreation);           \
  f(UNSCALEDFONTDESTRUCTION, RecordedUnscaledFontDestruction);     \
  f(INTOLUMINANCE, RecordedIntoLuminanceSource);                   \
  f(EXTERNALSURFACECREATION, RecordedExternalSurfaceCreation);     \
  f(FLUSH, RecordedFlush);                                         \
  f(DETACHALLSNAPSHOTS, RecordedDetachAllSnapshots);
>>>>>>> upstream-releases

#define DO_WITH_EVENT_TYPE(_typeenum, _class) \
<<<<<<< HEAD
  case _typeenum: {                           \
    auto e = _class(aStream);                 \
    return f(&e);                             \
  }
||||||| merged common ancestors
  case _typeenum: { auto e = _class(aStream); return f(&e); }
=======
  case _typeenum: {                           \
    auto e = _class(aStream);                 \
    return aAction(&e);                       \
  }
>>>>>>> upstream-releases

<<<<<<< HEAD
template <class S, class F>
bool RecordedEvent::DoWithEvent(S &aStream, EventType aType, F f) {
||||||| merged common ancestors
template<class S, class F>
bool
RecordedEvent::DoWithEvent(S &aStream, EventType aType, F f)
{
=======
template <class S>
bool RecordedEvent::DoWithEvent(
    S& aStream, EventType aType,
    const std::function<bool(RecordedEvent*)>& aAction) {
>>>>>>> upstream-releases
  switch (aType) {
    FOR_EACH_EVENT(DO_WITH_EVENT_TYPE)
    default:
      return false;
  }
}

}  // namespace gfx
}  // namespace mozilla

#endif
