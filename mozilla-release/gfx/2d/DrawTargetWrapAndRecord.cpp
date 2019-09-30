/* -*- Mode: C++; tab-width: 8; indent-tabs-mode: nil; c-basic-offset: 2 -*- */
/* vim: set ts=8 sts=2 et sw=2 tw=80: */
/* This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/. */

#include "DrawTargetWrapAndRecord.h"
#include "PathRecording.h"
#include <stdio.h>

#include "Logging.h"
#include "Tools.h"
#include "Filters.h"
#include "mozilla/UniquePtr.h"
#include "RecordingTypes.h"
#include "RecordedEventImpl.h"

namespace mozilla {
namespace gfx {

<<<<<<< HEAD
struct WrapAndRecordSourceSurfaceUserData {
  void *refPtr;
||||||| merged common ancestors
struct WrapAndRecordSourceSurfaceUserData
{
  void *refPtr;
=======
struct WrapAndRecordSourceSurfaceUserData {
  void* refPtr;
>>>>>>> upstream-releases
  RefPtr<DrawEventRecorderPrivate> recorder;
};

<<<<<<< HEAD
void WrapAndRecordSourceSurfaceUserDataFunc(void *aUserData) {
  WrapAndRecordSourceSurfaceUserData *userData =
      static_cast<WrapAndRecordSourceSurfaceUserData *>(aUserData);
||||||| merged common ancestors
void WrapAndRecordSourceSurfaceUserDataFunc(void *aUserData)
{
  WrapAndRecordSourceSurfaceUserData *userData =
    static_cast<WrapAndRecordSourceSurfaceUserData*>(aUserData);
=======
static void WrapAndRecordSourceSurfaceUserDataFunc(void* aUserData) {
  WrapAndRecordSourceSurfaceUserData* userData =
      static_cast<WrapAndRecordSourceSurfaceUserData*>(aUserData);
>>>>>>> upstream-releases

  userData->recorder->RemoveSourceSurface((SourceSurface *)userData->refPtr);
  userData->recorder->RemoveStoredObject(userData->refPtr);
  userData->recorder->RecordEvent(
      RecordedSourceSurfaceDestruction(ReferencePtr(userData->refPtr)));

  delete userData;
}

<<<<<<< HEAD
static void StoreSourceSurface(DrawEventRecorderPrivate *aRecorder,
                               SourceSurface *aSurface,
                               DataSourceSurface *aDataSurf,
                               const char *reason) {
||||||| merged common ancestors
static void
StoreSourceSurface(DrawEventRecorderPrivate *aRecorder, SourceSurface *aSurface,
                   DataSourceSurface *aDataSurf, const char *reason)
{
=======
static void StoreSourceSurface(DrawEventRecorderPrivate* aRecorder,
                               SourceSurface* aSurface,
                               DataSourceSurface* aDataSurf,
                               const char* reason) {
>>>>>>> upstream-releases
  if (!aDataSurf) {
    gfxWarning() << "Recording failed to record SourceSurface for " << reason;
    // Insert a bogus source surface.
    int32_t stride =
        aSurface->GetSize().width * BytesPerPixel(aSurface->GetFormat());
    UniquePtr<uint8_t[]> sourceData(
        new uint8_t[stride * aSurface->GetSize().height]());
    aRecorder->RecordEvent(RecordedSourceSurfaceCreation(
        aSurface, sourceData.get(), stride, aSurface->GetSize(),
        aSurface->GetFormat()));
  } else {
    DataSourceSurface::ScopedMap map(aDataSurf, DataSourceSurface::READ);
    aRecorder->RecordEvent(RecordedSourceSurfaceCreation(
        aSurface, map.GetData(), map.GetStride(), aDataSurf->GetSize(),
        aDataSurf->GetFormat()));
  }
}

<<<<<<< HEAD
static void EnsureSurfaceStored(DrawEventRecorderPrivate *aRecorder,
                                SourceSurface *aSurface, const char *reason) {
||||||| merged common ancestors
static void
EnsureSurfaceStored(DrawEventRecorderPrivate *aRecorder, SourceSurface *aSurface,
                    const char *reason)
{
=======
static void EnsureSurfaceStored(DrawEventRecorderPrivate* aRecorder,
                                SourceSurface* aSurface, const char* reason) {
>>>>>>> upstream-releases
  if (aRecorder->HasStoredObject(aSurface)) {
    return;
  }

  RefPtr<DataSourceSurface> dataSurf = aSurface->GetDataSurface();
  StoreSourceSurface(aRecorder, aSurface, dataSurf, reason);
  aRecorder->AddStoredObject(aSurface);
  aRecorder->AddSourceSurface(aSurface);

<<<<<<< HEAD
  WrapAndRecordSourceSurfaceUserData *userData =
      new WrapAndRecordSourceSurfaceUserData;
||||||| merged common ancestors
  WrapAndRecordSourceSurfaceUserData *userData = new WrapAndRecordSourceSurfaceUserData;
=======
  WrapAndRecordSourceSurfaceUserData* userData =
      new WrapAndRecordSourceSurfaceUserData;
>>>>>>> upstream-releases
  userData->refPtr = aSurface;
  userData->recorder = aRecorder;
<<<<<<< HEAD
  aSurface->AddUserData(reinterpret_cast<UserDataKey *>(aRecorder), userData,
                        &WrapAndRecordSourceSurfaceUserDataFunc);
||||||| merged common ancestors
  aSurface->AddUserData(reinterpret_cast<UserDataKey*>(aRecorder),
                        userData, &WrapAndRecordSourceSurfaceUserDataFunc);
=======
  aSurface->AddUserData(reinterpret_cast<UserDataKey*>(aRecorder), userData,
                        &WrapAndRecordSourceSurfaceUserDataFunc);
>>>>>>> upstream-releases
}

class SourceSurfaceWrapAndRecord : public SourceSurface {
 public:
  MOZ_DECLARE_REFCOUNTED_VIRTUAL_TYPENAME(SourceSurfaceWrapAndRecord, override)

<<<<<<< HEAD
  SourceSurfaceWrapAndRecord(SourceSurface *aFinalSurface,
                             DrawEventRecorderPrivate *aRecorder)
      : mFinalSurface(aFinalSurface), mRecorder(aRecorder) {
||||||| merged common ancestors
  SourceSurfaceWrapAndRecord(SourceSurface *aFinalSurface, DrawEventRecorderPrivate *aRecorder)
    : mFinalSurface(aFinalSurface), mRecorder(aRecorder)
  {
=======
  SourceSurfaceWrapAndRecord(SourceSurface* aFinalSurface,
                             DrawEventRecorderPrivate* aRecorder)
      : mFinalSurface(aFinalSurface), mRecorder(aRecorder) {
>>>>>>> upstream-releases
    mRecorder->AddStoredObject(this);
  }

  ~SourceSurfaceWrapAndRecord() {
    mRecorder->RemoveStoredObject(this);
    mRecorder->RecordEvent(
        RecordedSourceSurfaceDestruction(ReferencePtr(this)));
  }

<<<<<<< HEAD
  virtual SurfaceType GetType() const override {
    return SurfaceType::RECORDING;
  }
  virtual IntSize GetSize() const override { return mFinalSurface->GetSize(); }
  virtual SurfaceFormat GetFormat() const override {
    return mFinalSurface->GetFormat();
  }
  virtual already_AddRefed<DataSourceSurface> GetDataSurface() override {
    return mFinalSurface->GetDataSurface();
  }
||||||| merged common ancestors
  virtual SurfaceType GetType() const override { return SurfaceType::RECORDING; }
  virtual IntSize GetSize() const override { return mFinalSurface->GetSize(); }
  virtual SurfaceFormat GetFormat() const override { return mFinalSurface->GetFormat(); }
  virtual already_AddRefed<DataSourceSurface> GetDataSurface() override { return mFinalSurface->GetDataSurface(); }
=======
  SurfaceType GetType() const override { return SurfaceType::RECORDING; }
  IntSize GetSize() const override { return mFinalSurface->GetSize(); }
  SurfaceFormat GetFormat() const override {
    return mFinalSurface->GetFormat();
  }
  already_AddRefed<DataSourceSurface> GetDataSurface() override {
    return mFinalSurface->GetDataSurface();
  }
>>>>>>> upstream-releases

  RefPtr<SourceSurface> mFinalSurface;
  RefPtr<DrawEventRecorderPrivate> mRecorder;
};

class GradientStopsWrapAndRecord : public GradientStops {
 public:
  MOZ_DECLARE_REFCOUNTED_VIRTUAL_TYPENAME(GradientStopsWrapAndRecord, override)

<<<<<<< HEAD
  GradientStopsWrapAndRecord(GradientStops *aFinalGradientStops,
                             DrawEventRecorderPrivate *aRecorder)
      : mFinalGradientStops(aFinalGradientStops), mRecorder(aRecorder) {
||||||| merged common ancestors
  GradientStopsWrapAndRecord(GradientStops *aFinalGradientStops, DrawEventRecorderPrivate *aRecorder)
    : mFinalGradientStops(aFinalGradientStops), mRecorder(aRecorder)
  {
=======
  GradientStopsWrapAndRecord(GradientStops* aFinalGradientStops,
                             DrawEventRecorderPrivate* aRecorder)
      : mFinalGradientStops(aFinalGradientStops), mRecorder(aRecorder) {
>>>>>>> upstream-releases
    mRecorder->AddStoredObject(this);
  }

  ~GradientStopsWrapAndRecord() {
    mRecorder->RemoveStoredObject(this);
    mRecorder->RecordEvent(
        RecordedGradientStopsDestruction(ReferencePtr(this)));
  }

<<<<<<< HEAD
  virtual BackendType GetBackendType() const override {
    return BackendType::RECORDING;
  }
||||||| merged common ancestors
  virtual BackendType GetBackendType() const override { return BackendType::RECORDING; }
=======
  BackendType GetBackendType() const override { return BackendType::RECORDING; }
>>>>>>> upstream-releases

  RefPtr<GradientStops> mFinalGradientStops;
  RefPtr<DrawEventRecorderPrivate> mRecorder;
};

<<<<<<< HEAD
static SourceSurface *GetSourceSurface(SourceSurface *aSurface) {
||||||| merged common ancestors
static SourceSurface *
GetSourceSurface(SourceSurface *aSurface)
{
=======
static SourceSurface* GetSourceSurface(SourceSurface* aSurface) {
>>>>>>> upstream-releases
  if (aSurface->GetType() != SurfaceType::RECORDING) {
    return aSurface;
  }

  return static_cast<SourceSurfaceWrapAndRecord *>(aSurface)->mFinalSurface;
}

<<<<<<< HEAD
static GradientStops *GetGradientStops(GradientStops *aStops) {
||||||| merged common ancestors
static GradientStops *
GetGradientStops(GradientStops *aStops)
{
=======
static GradientStops* GetGradientStops(GradientStops* aStops) {
>>>>>>> upstream-releases
  if (aStops->GetBackendType() != BackendType::RECORDING) {
    return aStops;
  }

  return static_cast<GradientStopsWrapAndRecord *>(aStops)->mFinalGradientStops;
}

class FilterNodeWrapAndRecord : public FilterNode {
 public:
  MOZ_DECLARE_REFCOUNTED_VIRTUAL_TYPENAME(FilterNodeWrapAndRecord, override)
  using FilterNode::SetAttribute;

<<<<<<< HEAD
  FilterNodeWrapAndRecord(FilterNode *aFinalFilterNode,
                          DrawEventRecorderPrivate *aRecorder)
      : mFinalFilterNode(aFinalFilterNode), mRecorder(aRecorder) {
||||||| merged common ancestors
  FilterNodeWrapAndRecord(FilterNode *aFinalFilterNode, DrawEventRecorderPrivate *aRecorder)
    : mFinalFilterNode(aFinalFilterNode), mRecorder(aRecorder)
  {
=======
  FilterNodeWrapAndRecord(FilterNode* aFinalFilterNode,
                          DrawEventRecorderPrivate* aRecorder)
      : mFinalFilterNode(aFinalFilterNode), mRecorder(aRecorder) {
>>>>>>> upstream-releases
    mRecorder->AddStoredObject(this);
  }

  ~FilterNodeWrapAndRecord() {
    mRecorder->RemoveStoredObject(this);
    mRecorder->RecordEvent(RecordedFilterNodeDestruction(ReferencePtr(this)));
  }

<<<<<<< HEAD
  static FilterNode *GetFilterNode(FilterNode *aNode) {
||||||| merged common ancestors
  static FilterNode*
  GetFilterNode(FilterNode* aNode)
  {
=======
  static FilterNode* GetFilterNode(FilterNode* aNode) {
>>>>>>> upstream-releases
    if (aNode->GetBackendType() != FILTER_BACKEND_RECORDING) {
      gfxWarning()
          << "Non recording filter node used with recording DrawTarget!";
      return aNode;
    }

    return static_cast<FilterNodeWrapAndRecord *>(aNode)->mFinalFilterNode;
  }

<<<<<<< HEAD
  virtual void SetInput(uint32_t aIndex, SourceSurface *aSurface) override {
    EnsureSurfaceStored(mRecorder, aSurface, "SetInput");
||||||| merged common ancestors
  virtual void SetInput(uint32_t aIndex, SourceSurface *aSurface) override
  {
    EnsureSurfaceStored(mRecorder, aSurface,  "SetInput");
=======
  void SetInput(uint32_t aIndex, SourceSurface* aSurface) override {
    EnsureSurfaceStored(mRecorder, aSurface, "SetInput");
>>>>>>> upstream-releases

    mRecorder->RecordEvent(RecordedFilterNodeSetInput(this, aIndex, aSurface));
    mFinalFilterNode->SetInput(aIndex, GetSourceSurface(aSurface));
  }
<<<<<<< HEAD
  virtual void SetInput(uint32_t aIndex, FilterNode *aFilter) override {
||||||| merged common ancestors
  virtual void SetInput(uint32_t aIndex, FilterNode *aFilter) override
  {
=======
  void SetInput(uint32_t aIndex, FilterNode* aFilter) override {
>>>>>>> upstream-releases
    MOZ_ASSERT(mRecorder->HasStoredObject(aFilter));

    mRecorder->RecordEvent(RecordedFilterNodeSetInput(this, aIndex, aFilter));
    mFinalFilterNode->SetInput(aIndex, GetFilterNode(aFilter));
  }

<<<<<<< HEAD
#define FORWARD_SET_ATTRIBUTE(type, argtype)                         \
  virtual void SetAttribute(uint32_t aIndex, type aValue) override { \
    mRecorder->RecordEvent(RecordedFilterNodeSetAttribute(           \
        this, aIndex, aValue,                                        \
        RecordedFilterNodeSetAttribute::ARGTYPE_##argtype));         \
    mFinalFilterNode->SetAttribute(aIndex, aValue);                  \
||||||| merged common ancestors

#define FORWARD_SET_ATTRIBUTE(type, argtype) \
  virtual void SetAttribute(uint32_t aIndex, type aValue) override { \
    mRecorder->RecordEvent(RecordedFilterNodeSetAttribute(this, aIndex, aValue, RecordedFilterNodeSetAttribute::ARGTYPE_##argtype)); \
    mFinalFilterNode->SetAttribute(aIndex, aValue); \
=======
#define FORWARD_SET_ATTRIBUTE(type, argtype)                 \
  void SetAttribute(uint32_t aIndex, type aValue) override { \
    mRecorder->RecordEvent(RecordedFilterNodeSetAttribute(   \
        this, aIndex, aValue,                                \
        RecordedFilterNodeSetAttribute::ARGTYPE_##argtype)); \
    mFinalFilterNode->SetAttribute(aIndex, aValue);          \
>>>>>>> upstream-releases
  }

  FORWARD_SET_ATTRIBUTE(bool, BOOL);
  FORWARD_SET_ATTRIBUTE(uint32_t, UINT32);
  FORWARD_SET_ATTRIBUTE(Float, FLOAT);
  FORWARD_SET_ATTRIBUTE(const Size &, SIZE);
  FORWARD_SET_ATTRIBUTE(const IntSize &, INTSIZE);
  FORWARD_SET_ATTRIBUTE(const IntPoint &, INTPOINT);
  FORWARD_SET_ATTRIBUTE(const Rect &, RECT);
  FORWARD_SET_ATTRIBUTE(const IntRect &, INTRECT);
  FORWARD_SET_ATTRIBUTE(const Point &, POINT);
  FORWARD_SET_ATTRIBUTE(const Matrix &, MATRIX);
  FORWARD_SET_ATTRIBUTE(const Matrix5x4 &, MATRIX5X4);
  FORWARD_SET_ATTRIBUTE(const Point3D &, POINT3D);
  FORWARD_SET_ATTRIBUTE(const Color &, COLOR);

#undef FORWARD_SET_ATTRIBUTE

<<<<<<< HEAD
  virtual void SetAttribute(uint32_t aIndex, const Float *aFloat,
                            uint32_t aSize) override {
    mRecorder->RecordEvent(
        RecordedFilterNodeSetAttribute(this, aIndex, aFloat, aSize));
||||||| merged common ancestors
  virtual void SetAttribute(uint32_t aIndex, const Float* aFloat, uint32_t aSize) override {
    mRecorder->RecordEvent(RecordedFilterNodeSetAttribute(this, aIndex, aFloat, aSize));
=======
  virtual void SetAttribute(uint32_t aIndex, const Float* aFloat,
                            uint32_t aSize) override {
    mRecorder->RecordEvent(
        RecordedFilterNodeSetAttribute(this, aIndex, aFloat, aSize));
>>>>>>> upstream-releases
    mFinalFilterNode->SetAttribute(aIndex, aFloat, aSize);
  }

<<<<<<< HEAD
  virtual FilterBackend GetBackendType() override {
    return FILTER_BACKEND_RECORDING;
  }
||||||| merged common ancestors
  virtual FilterBackend GetBackendType() override { return FILTER_BACKEND_RECORDING; }
=======
  FilterBackend GetBackendType() override { return FILTER_BACKEND_RECORDING; }
>>>>>>> upstream-releases

  RefPtr<FilterNode> mFinalFilterNode;
  RefPtr<DrawEventRecorderPrivate> mRecorder;
};

<<<<<<< HEAD
struct AdjustedPattern {
  explicit AdjustedPattern(const Pattern &aPattern) : mPattern(nullptr) {
    mOrigPattern = const_cast<Pattern *>(&aPattern);
||||||| merged common ancestors
struct AdjustedPattern
{
  explicit AdjustedPattern(const Pattern &aPattern)
    : mPattern(nullptr)
  {
    mOrigPattern = const_cast<Pattern*>(&aPattern);
=======
struct AdjustedPattern final {
  explicit AdjustedPattern(const Pattern& aPattern) : mPattern(nullptr) {
    mOrigPattern = const_cast<Pattern*>(&aPattern);
>>>>>>> upstream-releases
  }

  ~AdjustedPattern() {
    if (mPattern) {
      mPattern->~Pattern();
    }
  }

<<<<<<< HEAD
  operator Pattern *() {
    switch (mOrigPattern->GetType()) {
      case PatternType::COLOR:
        return mOrigPattern;
      case PatternType::SURFACE: {
        SurfacePattern *surfPat = static_cast<SurfacePattern *>(mOrigPattern);
        mPattern = new (mSurfPat) SurfacePattern(
            GetSourceSurface(surfPat->mSurface), surfPat->mExtendMode,
            surfPat->mMatrix, surfPat->mSamplingFilter, surfPat->mSamplingRect);
||||||| merged common ancestors
  operator Pattern*()
  {
    switch(mOrigPattern->GetType()) {
    case PatternType::COLOR:
      return mOrigPattern;
    case PatternType::SURFACE:
      {
        SurfacePattern *surfPat = static_cast<SurfacePattern*>(mOrigPattern);
        mPattern =
          new (mSurfPat) SurfacePattern(GetSourceSurface(surfPat->mSurface),
                                        surfPat->mExtendMode, surfPat->mMatrix,
                                        surfPat->mSamplingFilter,
                                        surfPat->mSamplingRect);
=======
  operator Pattern*() {
    switch (mOrigPattern->GetType()) {
      case PatternType::COLOR:
        return mOrigPattern;
      case PatternType::SURFACE: {
        SurfacePattern* surfPat = static_cast<SurfacePattern*>(mOrigPattern);
        mPattern = new (mSurfPat) SurfacePattern(
            GetSourceSurface(surfPat->mSurface), surfPat->mExtendMode,
            surfPat->mMatrix, surfPat->mSamplingFilter, surfPat->mSamplingRect);
>>>>>>> upstream-releases
        return mPattern;
      }
<<<<<<< HEAD
      case PatternType::LINEAR_GRADIENT: {
        LinearGradientPattern *linGradPat =
            static_cast<LinearGradientPattern *>(mOrigPattern);
        mPattern = new (mLinGradPat) LinearGradientPattern(
            linGradPat->mBegin, linGradPat->mEnd,
            GetGradientStops(linGradPat->mStops), linGradPat->mMatrix);
||||||| merged common ancestors
    case PatternType::LINEAR_GRADIENT:
      {
        LinearGradientPattern *linGradPat = static_cast<LinearGradientPattern*>(mOrigPattern);
        mPattern =
          new (mLinGradPat) LinearGradientPattern(linGradPat->mBegin, linGradPat->mEnd,
                                                  GetGradientStops(linGradPat->mStops),
                                                  linGradPat->mMatrix);
=======
      case PatternType::LINEAR_GRADIENT: {
        LinearGradientPattern* linGradPat =
            static_cast<LinearGradientPattern*>(mOrigPattern);
        mPattern = new (mLinGradPat) LinearGradientPattern(
            linGradPat->mBegin, linGradPat->mEnd,
            GetGradientStops(linGradPat->mStops), linGradPat->mMatrix);
>>>>>>> upstream-releases
        return mPattern;
      }
<<<<<<< HEAD
      case PatternType::RADIAL_GRADIENT: {
        RadialGradientPattern *radGradPat =
            static_cast<RadialGradientPattern *>(mOrigPattern);
        mPattern = new (mRadGradPat) RadialGradientPattern(
            radGradPat->mCenter1, radGradPat->mCenter2, radGradPat->mRadius1,
            radGradPat->mRadius2, GetGradientStops(radGradPat->mStops),
            radGradPat->mMatrix);
||||||| merged common ancestors
    case PatternType::RADIAL_GRADIENT:
      {
        RadialGradientPattern *radGradPat = static_cast<RadialGradientPattern*>(mOrigPattern);
        mPattern =
          new (mRadGradPat) RadialGradientPattern(radGradPat->mCenter1, radGradPat->mCenter2,
                                                  radGradPat->mRadius1, radGradPat->mRadius2,
                                                  GetGradientStops(radGradPat->mStops),
                                                  radGradPat->mMatrix);
=======
      case PatternType::RADIAL_GRADIENT: {
        RadialGradientPattern* radGradPat =
            static_cast<RadialGradientPattern*>(mOrigPattern);
        mPattern = new (mRadGradPat) RadialGradientPattern(
            radGradPat->mCenter1, radGradPat->mCenter2, radGradPat->mRadius1,
            radGradPat->mRadius2, GetGradientStops(radGradPat->mStops),
            radGradPat->mMatrix);
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

  Pattern* mOrigPattern;
  Pattern* mPattern;
};

<<<<<<< HEAD
DrawTargetWrapAndRecord::DrawTargetWrapAndRecord(DrawEventRecorder *aRecorder,
                                                 DrawTarget *aDT, bool aHasData)
    : mRecorder(static_cast<DrawEventRecorderPrivate *>(aRecorder)),
      mFinalDT(aDT) {
||||||| merged common ancestors
DrawTargetWrapAndRecord::DrawTargetWrapAndRecord(DrawEventRecorder *aRecorder, DrawTarget *aDT, bool aHasData)
  : mRecorder(static_cast<DrawEventRecorderPrivate*>(aRecorder))
  , mFinalDT(aDT)
{
=======
DrawTargetWrapAndRecord::DrawTargetWrapAndRecord(DrawEventRecorder* aRecorder,
                                                 DrawTarget* aDT, bool aHasData)
    : mRecorder(static_cast<DrawEventRecorderPrivate*>(aRecorder)),
      mFinalDT(aDT) {
>>>>>>> upstream-releases
  RefPtr<SourceSurface> snapshot = aHasData ? mFinalDT->Snapshot() : nullptr;
  mRecorder->RecordEvent(RecordedDrawTargetCreation(
      this, mFinalDT->GetBackendType(), mFinalDT->GetSize(),
      mFinalDT->GetFormat(), aHasData, snapshot));
  mFormat = mFinalDT->GetFormat();
}

<<<<<<< HEAD
DrawTargetWrapAndRecord::DrawTargetWrapAndRecord(
    const DrawTargetWrapAndRecord *aDT, DrawTarget *aSimilarDT)
    : mRecorder(aDT->mRecorder), mFinalDT(aSimilarDT) {
  mRecorder->RecordEvent(RecordedCreateSimilarDrawTarget(
      this, mFinalDT->GetSize(), mFinalDT->GetFormat()));
||||||| merged common ancestors
DrawTargetWrapAndRecord::DrawTargetWrapAndRecord(const DrawTargetWrapAndRecord *aDT,
                                         DrawTarget *aSimilarDT)
  : mRecorder(aDT->mRecorder)
  , mFinalDT(aSimilarDT)
{
  mRecorder->RecordEvent(RecordedCreateSimilarDrawTarget(this,
                                                         mFinalDT->GetSize(),
                                                         mFinalDT->GetFormat()));
=======
DrawTargetWrapAndRecord::DrawTargetWrapAndRecord(
    const DrawTargetWrapAndRecord* aDT, DrawTarget* aSimilarDT)
    : mRecorder(aDT->mRecorder), mFinalDT(aSimilarDT) {
  mRecorder->RecordEvent(RecordedCreateSimilarDrawTarget(
      this, mFinalDT->GetSize(), mFinalDT->GetFormat()));
>>>>>>> upstream-releases
  mFormat = mFinalDT->GetFormat();
}

<<<<<<< HEAD
DrawTargetWrapAndRecord::~DrawTargetWrapAndRecord() {
  mRecorder->RecordEvent(
      RecordedDrawTargetDestruction(static_cast<DrawTarget *>(this)));
||||||| merged common ancestors
DrawTargetWrapAndRecord::~DrawTargetWrapAndRecord()
{
  mRecorder->RecordEvent(RecordedDrawTargetDestruction(static_cast<DrawTarget*>(this)));
=======
DrawTargetWrapAndRecord::~DrawTargetWrapAndRecord() {
  mRecorder->RecordEvent(
      RecordedDrawTargetDestruction(static_cast<DrawTarget*>(this)));
>>>>>>> upstream-releases
}

<<<<<<< HEAD
void DrawTargetWrapAndRecord::FillRect(const Rect &aRect,
                                       const Pattern &aPattern,
                                       const DrawOptions &aOptions) {
||||||| merged common ancestors
void
DrawTargetWrapAndRecord::FillRect(const Rect &aRect,
                              const Pattern &aPattern,
                              const DrawOptions &aOptions)
{
=======
void DrawTargetWrapAndRecord::FillRect(const Rect& aRect,
                                       const Pattern& aPattern,
                                       const DrawOptions& aOptions) {
>>>>>>> upstream-releases
  EnsurePatternDependenciesStored(aPattern);

  mRecorder->RecordEvent(RecordedFillRect(this, aRect, aPattern, aOptions));
  mFinalDT->FillRect(aRect, *AdjustedPattern(aPattern), aOptions);
}

<<<<<<< HEAD
void DrawTargetWrapAndRecord::StrokeRect(const Rect &aRect,
                                         const Pattern &aPattern,
                                         const StrokeOptions &aStrokeOptions,
                                         const DrawOptions &aOptions) {
||||||| merged common ancestors
void
DrawTargetWrapAndRecord::StrokeRect(const Rect &aRect,
                                const Pattern &aPattern,
                                const StrokeOptions &aStrokeOptions,
                                const DrawOptions &aOptions)
{
=======
void DrawTargetWrapAndRecord::StrokeRect(const Rect& aRect,
                                         const Pattern& aPattern,
                                         const StrokeOptions& aStrokeOptions,
                                         const DrawOptions& aOptions) {
>>>>>>> upstream-releases
  EnsurePatternDependenciesStored(aPattern);

  mRecorder->RecordEvent(
      RecordedStrokeRect(this, aRect, aPattern, aStrokeOptions, aOptions));
  mFinalDT->StrokeRect(aRect, *AdjustedPattern(aPattern), aStrokeOptions,
                       aOptions);
}

<<<<<<< HEAD
void DrawTargetWrapAndRecord::StrokeLine(const Point &aBegin, const Point &aEnd,
                                         const Pattern &aPattern,
                                         const StrokeOptions &aStrokeOptions,
                                         const DrawOptions &aOptions) {
||||||| merged common ancestors
void
DrawTargetWrapAndRecord::StrokeLine(const Point &aBegin,
                                const Point &aEnd,
                                const Pattern &aPattern,
                                const StrokeOptions &aStrokeOptions,
                                const DrawOptions &aOptions)
{
=======
void DrawTargetWrapAndRecord::StrokeLine(const Point& aBegin, const Point& aEnd,
                                         const Pattern& aPattern,
                                         const StrokeOptions& aStrokeOptions,
                                         const DrawOptions& aOptions) {
>>>>>>> upstream-releases
  EnsurePatternDependenciesStored(aPattern);

  mRecorder->RecordEvent(RecordedStrokeLine(this, aBegin, aEnd, aPattern,
                                            aStrokeOptions, aOptions));
  mFinalDT->StrokeLine(aBegin, aEnd, *AdjustedPattern(aPattern), aStrokeOptions,
                       aOptions);
}

<<<<<<< HEAD
void DrawTargetWrapAndRecord::Fill(const Path *aPath, const Pattern &aPattern,
                                   const DrawOptions &aOptions) {
||||||| merged common ancestors
void
DrawTargetWrapAndRecord::Fill(const Path *aPath,
                          const Pattern &aPattern,
                          const DrawOptions &aOptions)
{
=======
void DrawTargetWrapAndRecord::Fill(const Path* aPath, const Pattern& aPattern,
                                   const DrawOptions& aOptions) {
>>>>>>> upstream-releases
  RefPtr<PathRecording> pathWrapAndRecord = EnsurePathStored(aPath);
  EnsurePatternDependenciesStored(aPattern);

  mRecorder->RecordEvent(
      RecordedFill(this, pathWrapAndRecord, aPattern, aOptions));
  mFinalDT->Fill(pathWrapAndRecord->mPath, *AdjustedPattern(aPattern),
                 aOptions);
}

<<<<<<< HEAD
struct WrapAndRecordFontUserData {
  void *refPtr;
||||||| merged common ancestors
struct WrapAndRecordFontUserData
{
  void *refPtr;
=======
struct WrapAndRecordFontUserData {
  void* refPtr;
>>>>>>> upstream-releases
  RefPtr<DrawEventRecorderPrivate> recorder;
};

<<<<<<< HEAD
void WrapAndRecordFontUserDataDestroyFunc(void *aUserData) {
  WrapAndRecordFontUserData *userData =
      static_cast<WrapAndRecordFontUserData *>(aUserData);
||||||| merged common ancestors
void WrapAndRecordFontUserDataDestroyFunc(void *aUserData)
{
  WrapAndRecordFontUserData *userData =
    static_cast<WrapAndRecordFontUserData*>(aUserData);
=======
static void WrapAndRecordFontUserDataDestroyFunc(void* aUserData) {
  WrapAndRecordFontUserData* userData =
      static_cast<WrapAndRecordFontUserData*>(aUserData);
>>>>>>> upstream-releases

<<<<<<< HEAD
  userData->recorder->RecordEvent(
      RecordedScaledFontDestruction(ReferencePtr(userData->refPtr)));
  userData->recorder->RemoveScaledFont((ScaledFont *)userData->refPtr);
||||||| merged common ancestors
  userData->recorder->RecordEvent(RecordedScaledFontDestruction(ReferencePtr(userData->refPtr)));
  userData->recorder->RemoveScaledFont((ScaledFont*)userData->refPtr);
=======
  userData->recorder->RecordEvent(
      RecordedScaledFontDestruction(ReferencePtr(userData->refPtr)));
  userData->recorder->RemoveScaledFont((ScaledFont*)userData->refPtr);
>>>>>>> upstream-releases
  delete userData;
}

<<<<<<< HEAD
void DrawTargetWrapAndRecord::FillGlyphs(ScaledFont *aFont,
                                         const GlyphBuffer &aBuffer,
                                         const Pattern &aPattern,
                                         const DrawOptions &aOptions) {
||||||| merged common ancestors
void
DrawTargetWrapAndRecord::FillGlyphs(ScaledFont *aFont,
                                const GlyphBuffer &aBuffer,
                                const Pattern &aPattern,
                                const DrawOptions &aOptions)
{
=======
void DrawTargetWrapAndRecord::FillGlyphs(ScaledFont* aFont,
                                         const GlyphBuffer& aBuffer,
                                         const Pattern& aPattern,
                                         const DrawOptions& aOptions) {
>>>>>>> upstream-releases
  EnsurePatternDependenciesStored(aPattern);

  UserDataKey *userDataKey = reinterpret_cast<UserDataKey *>(mRecorder.get());
  if (!aFont->GetUserData(userDataKey)) {
    UnscaledFont *unscaledFont = aFont->GetUnscaledFont();
    if (!mRecorder->HasStoredObject(unscaledFont)) {
      RecordedFontData fontData(unscaledFont);
      RecordedFontDetails fontDetails;
      if (fontData.GetFontDetails(fontDetails)) {
        // Try to serialise the whole font, just in case this is a web font that
        // is not present on the system.
        if (!mRecorder->HasStoredFontData(fontDetails.fontDataKey)) {
          mRecorder->RecordEvent(fontData);
          mRecorder->AddStoredFontData(fontDetails.fontDataKey);
        }
        mRecorder->RecordEvent(
            RecordedUnscaledFontCreation(unscaledFont, fontDetails));
      } else {
        // If that fails, record just the font description and try to load it
        // from the system on the other side.
        RecordedFontDescriptor fontDesc(unscaledFont);
        if (fontDesc.IsValid()) {
          mRecorder->RecordEvent(fontDesc);
        } else {
          gfxWarning() << "DrawTargetWrapAndRecord::FillGlyphs failed to "
                          "serialise UnscaledFont";
        }
      }
      mRecorder->AddStoredObject(unscaledFont);
    }

    mRecorder->RecordEvent(RecordedScaledFontCreation(aFont, unscaledFont));

    WrapAndRecordFontUserData* userData = new WrapAndRecordFontUserData;
    userData->refPtr = aFont;
    userData->recorder = mRecorder;
    aFont->AddUserData(userDataKey, userData,
                       &WrapAndRecordFontUserDataDestroyFunc);
    userData->recorder->AddScaledFont(aFont);
  }

  mRecorder->RecordEvent(RecordedFillGlyphs(
      this, aFont, aPattern, aOptions, aBuffer.mGlyphs, aBuffer.mNumGlyphs));
  mFinalDT->FillGlyphs(aFont, aBuffer, *AdjustedPattern(aPattern), aOptions);
}

<<<<<<< HEAD
void DrawTargetWrapAndRecord::Mask(const Pattern &aSource, const Pattern &aMask,
                                   const DrawOptions &aOptions) {
||||||| merged common ancestors
void
DrawTargetWrapAndRecord::Mask(const Pattern &aSource,
                          const Pattern &aMask,
                          const DrawOptions &aOptions)
{
=======
void DrawTargetWrapAndRecord::Mask(const Pattern& aSource, const Pattern& aMask,
                                   const DrawOptions& aOptions) {
>>>>>>> upstream-releases
  EnsurePatternDependenciesStored(aSource);
  EnsurePatternDependenciesStored(aMask);

  mRecorder->RecordEvent(RecordedMask(this, aSource, aMask, aOptions));
  mFinalDT->Mask(*AdjustedPattern(aSource), *AdjustedPattern(aMask), aOptions);
}

<<<<<<< HEAD
void DrawTargetWrapAndRecord::MaskSurface(const Pattern &aSource,
                                          SourceSurface *aMask, Point aOffset,
                                          const DrawOptions &aOptions) {
||||||| merged common ancestors
void
DrawTargetWrapAndRecord::MaskSurface(const Pattern &aSource,
                                 SourceSurface *aMask,
                                 Point aOffset,
                                 const DrawOptions &aOptions)
{
=======
void DrawTargetWrapAndRecord::MaskSurface(const Pattern& aSource,
                                          SourceSurface* aMask, Point aOffset,
                                          const DrawOptions& aOptions) {
>>>>>>> upstream-releases
  EnsurePatternDependenciesStored(aSource);
  EnsureSurfaceStored(mRecorder, aMask, "MaskSurface");

  mRecorder->RecordEvent(
      RecordedMaskSurface(this, aSource, aMask, aOffset, aOptions));
  mFinalDT->MaskSurface(*AdjustedPattern(aSource), GetSourceSurface(aMask),
                        aOffset, aOptions);
}

<<<<<<< HEAD
void DrawTargetWrapAndRecord::Stroke(const Path *aPath, const Pattern &aPattern,
                                     const StrokeOptions &aStrokeOptions,
                                     const DrawOptions &aOptions) {
||||||| merged common ancestors
void
DrawTargetWrapAndRecord::Stroke(const Path *aPath,
                            const Pattern &aPattern,
                            const StrokeOptions &aStrokeOptions,
                            const DrawOptions &aOptions)
{
=======
void DrawTargetWrapAndRecord::Stroke(const Path* aPath, const Pattern& aPattern,
                                     const StrokeOptions& aStrokeOptions,
                                     const DrawOptions& aOptions) {
>>>>>>> upstream-releases
  RefPtr<PathRecording> pathWrapAndRecord = EnsurePathStored(aPath);
  EnsurePatternDependenciesStored(aPattern);

  mRecorder->RecordEvent(RecordedStroke(this, pathWrapAndRecord, aPattern,
                                        aStrokeOptions, aOptions));
  mFinalDT->Stroke(pathWrapAndRecord->mPath, *AdjustedPattern(aPattern),
                   aStrokeOptions, aOptions);
}

already_AddRefed<SourceSurface> DrawTargetWrapAndRecord::Snapshot() {
  RefPtr<SourceSurface> surf = mFinalDT->Snapshot();

  RefPtr<SourceSurface> retSurf =
      new SourceSurfaceWrapAndRecord(surf, mRecorder);

  mRecorder->RecordEvent(RecordedSnapshot(retSurf, this));

  return retSurf.forget();
}

already_AddRefed<SourceSurface> DrawTargetWrapAndRecord::IntoLuminanceSource(
    LuminanceType aLuminanceType, float aOpacity) {
  RefPtr<SourceSurface> surf =
      mFinalDT->IntoLuminanceSource(aLuminanceType, aOpacity);

  RefPtr<SourceSurface> retSurf =
      new SourceSurfaceWrapAndRecord(surf, mRecorder);

  mRecorder->RecordEvent(
      RecordedIntoLuminanceSource(retSurf, this, aLuminanceType, aOpacity));

  return retSurf.forget();
}

void DrawTargetWrapAndRecord::DetachAllSnapshots() {
  mFinalDT->DetachAllSnapshots();
}

<<<<<<< HEAD
void DrawTargetWrapAndRecord::DrawSurface(
    SourceSurface *aSurface, const Rect &aDest, const Rect &aSource,
    const DrawSurfaceOptions &aSurfOptions, const DrawOptions &aOptions) {
||||||| merged common ancestors
void
DrawTargetWrapAndRecord::DrawSurface(SourceSurface *aSurface,
                                 const Rect &aDest,
                                 const Rect &aSource,
                                 const DrawSurfaceOptions &aSurfOptions,
                                 const DrawOptions &aOptions)
{
=======
void DrawTargetWrapAndRecord::DrawSurface(
    SourceSurface* aSurface, const Rect& aDest, const Rect& aSource,
    const DrawSurfaceOptions& aSurfOptions, const DrawOptions& aOptions) {
>>>>>>> upstream-releases
  EnsureSurfaceStored(mRecorder, aSurface, "DrawSurface");

  mRecorder->RecordEvent(RecordedDrawSurface(this, aSurface, aDest, aSource,
                                             aSurfOptions, aOptions));
  mFinalDT->DrawSurface(GetSourceSurface(aSurface), aDest, aSource,
                        aSurfOptions, aOptions);
}

<<<<<<< HEAD
void DrawTargetWrapAndRecord::DrawSurfaceWithShadow(
    SourceSurface *aSurface, const Point &aDest, const Color &aColor,
    const Point &aOffset, Float aSigma, CompositionOp aOp) {
||||||| merged common ancestors
void
DrawTargetWrapAndRecord::DrawSurfaceWithShadow(SourceSurface *aSurface,
                                           const Point &aDest,
                                           const Color &aColor,
                                           const Point &aOffset,
                                           Float aSigma,
                                           CompositionOp aOp)
{
=======
void DrawTargetWrapAndRecord::DrawSurfaceWithShadow(
    SourceSurface* aSurface, const Point& aDest, const Color& aColor,
    const Point& aOffset, Float aSigma, CompositionOp aOp) {
>>>>>>> upstream-releases
  EnsureSurfaceStored(mRecorder, aSurface, "DrawSurfaceWithShadow");

  mRecorder->RecordEvent(RecordedDrawSurfaceWithShadow(
      this, aSurface, aDest, aColor, aOffset, aSigma, aOp));
  mFinalDT->DrawSurfaceWithShadow(GetSourceSurface(aSurface), aDest, aColor,
                                  aOffset, aSigma, aOp);
}

<<<<<<< HEAD
void DrawTargetWrapAndRecord::DrawFilter(FilterNode *aNode,
                                         const Rect &aSourceRect,
                                         const Point &aDestPoint,
                                         const DrawOptions &aOptions) {
||||||| merged common ancestors
void
DrawTargetWrapAndRecord::DrawFilter(FilterNode *aNode,
                                const Rect &aSourceRect,
                                const Point &aDestPoint,
                                const DrawOptions &aOptions)
{
=======
void DrawTargetWrapAndRecord::DrawFilter(FilterNode* aNode,
                                         const Rect& aSourceRect,
                                         const Point& aDestPoint,
                                         const DrawOptions& aOptions) {
>>>>>>> upstream-releases
  MOZ_ASSERT(mRecorder->HasStoredObject(aNode));

  mRecorder->RecordEvent(
      RecordedDrawFilter(this, aNode, aSourceRect, aDestPoint, aOptions));
  mFinalDT->DrawFilter(FilterNodeWrapAndRecord::GetFilterNode(aNode),
                       aSourceRect, aDestPoint, aOptions);
}

already_AddRefed<FilterNode> DrawTargetWrapAndRecord::CreateFilter(
    FilterType aType) {
  RefPtr<FilterNode> node = mFinalDT->CreateFilter(aType);

  RefPtr<FilterNode> retNode = new FilterNodeWrapAndRecord(node, mRecorder);

  mRecorder->RecordEvent(RecordedFilterNodeCreation(retNode, aType));

  return retNode.forget();
}

<<<<<<< HEAD
void DrawTargetWrapAndRecord::ClearRect(const Rect &aRect) {
||||||| merged common ancestors
void
DrawTargetWrapAndRecord::ClearRect(const Rect &aRect)
{
=======
void DrawTargetWrapAndRecord::ClearRect(const Rect& aRect) {
>>>>>>> upstream-releases
  mRecorder->RecordEvent(RecordedClearRect(this, aRect));
  mFinalDT->ClearRect(aRect);
}

<<<<<<< HEAD
void DrawTargetWrapAndRecord::CopySurface(SourceSurface *aSurface,
                                          const IntRect &aSourceRect,
                                          const IntPoint &aDestination) {
||||||| merged common ancestors
void
DrawTargetWrapAndRecord::CopySurface(SourceSurface *aSurface,
                                 const IntRect &aSourceRect,
                                 const IntPoint &aDestination)
{
=======
void DrawTargetWrapAndRecord::CopySurface(SourceSurface* aSurface,
                                          const IntRect& aSourceRect,
                                          const IntPoint& aDestination) {
>>>>>>> upstream-releases
  EnsureSurfaceStored(mRecorder, aSurface, "CopySurface");

  mRecorder->RecordEvent(
      RecordedCopySurface(this, aSurface, aSourceRect, aDestination));
  mFinalDT->CopySurface(GetSourceSurface(aSurface), aSourceRect, aDestination);
}

<<<<<<< HEAD
void DrawTargetWrapAndRecord::PushClip(const Path *aPath) {
||||||| merged common ancestors
void
DrawTargetWrapAndRecord::PushClip(const Path *aPath)
{
=======
void DrawTargetWrapAndRecord::PushClip(const Path* aPath) {
>>>>>>> upstream-releases
  RefPtr<PathRecording> pathWrapAndRecord = EnsurePathStored(aPath);

  mRecorder->RecordEvent(RecordedPushClip(this, pathWrapAndRecord));
  mFinalDT->PushClip(pathWrapAndRecord->mPath);
}

<<<<<<< HEAD
void DrawTargetWrapAndRecord::PushClipRect(const Rect &aRect) {
||||||| merged common ancestors
void
DrawTargetWrapAndRecord::PushClipRect(const Rect &aRect)
{
=======
void DrawTargetWrapAndRecord::PushClipRect(const Rect& aRect) {
>>>>>>> upstream-releases
  mRecorder->RecordEvent(RecordedPushClipRect(this, aRect));
  mFinalDT->PushClipRect(aRect);
}

<<<<<<< HEAD
void DrawTargetWrapAndRecord::PopClip() {
  mRecorder->RecordEvent(RecordedPopClip(static_cast<DrawTarget *>(this)));
||||||| merged common ancestors
void
DrawTargetWrapAndRecord::PopClip()
{
  mRecorder->RecordEvent(RecordedPopClip(static_cast<DrawTarget*>(this)));
=======
void DrawTargetWrapAndRecord::PopClip() {
  mRecorder->RecordEvent(RecordedPopClip(static_cast<DrawTarget*>(this)));
>>>>>>> upstream-releases
  mFinalDT->PopClip();
}

<<<<<<< HEAD
void DrawTargetWrapAndRecord::PushLayer(bool aOpaque, Float aOpacity,
                                        SourceSurface *aMask,
                                        const Matrix &aMaskTransform,
                                        const IntRect &aBounds,
                                        bool aCopyBackground) {
||||||| merged common ancestors
void
DrawTargetWrapAndRecord::PushLayer(bool aOpaque, Float aOpacity,
                               SourceSurface* aMask,
                               const Matrix& aMaskTransform,
                               const IntRect& aBounds, bool aCopyBackground)
{
=======
void DrawTargetWrapAndRecord::PushLayer(bool aOpaque, Float aOpacity,
                                        SourceSurface* aMask,
                                        const Matrix& aMaskTransform,
                                        const IntRect& aBounds,
                                        bool aCopyBackground) {
>>>>>>> upstream-releases
  if (aMask) {
    EnsureSurfaceStored(mRecorder, aMask, "PushLayer");
  }

  mRecorder->RecordEvent(RecordedPushLayer(this, aOpaque, aOpacity, aMask,
                                           aMaskTransform, aBounds,
                                           aCopyBackground));
  mFinalDT->PushLayer(aOpaque, aOpacity, aMask, aMaskTransform, aBounds,
                      aCopyBackground);
}

<<<<<<< HEAD
void DrawTargetWrapAndRecord::PopLayer() {
  mRecorder->RecordEvent(RecordedPopLayer(static_cast<DrawTarget *>(this)));
||||||| merged common ancestors
void
DrawTargetWrapAndRecord::PopLayer()
{
  mRecorder->RecordEvent(RecordedPopLayer(static_cast<DrawTarget*>(this)));
=======
void DrawTargetWrapAndRecord::PopLayer() {
  mRecorder->RecordEvent(RecordedPopLayer(static_cast<DrawTarget*>(this)));
>>>>>>> upstream-releases
  mFinalDT->PopLayer();
}

already_AddRefed<SourceSurface>
<<<<<<< HEAD
DrawTargetWrapAndRecord::CreateSourceSurfaceFromData(
    unsigned char *aData, const IntSize &aSize, int32_t aStride,
    SurfaceFormat aFormat) const {
  RefPtr<SourceSurface> surf =
      mFinalDT->CreateSourceSurfaceFromData(aData, aSize, aStride, aFormat);
||||||| merged common ancestors
DrawTargetWrapAndRecord::CreateSourceSurfaceFromData(unsigned char *aData,
                                                 const IntSize &aSize,
                                                 int32_t aStride,
                                                 SurfaceFormat aFormat) const
{
  RefPtr<SourceSurface> surf = mFinalDT->CreateSourceSurfaceFromData(aData, aSize, aStride, aFormat);
=======
DrawTargetWrapAndRecord::CreateSourceSurfaceFromData(
    unsigned char* aData, const IntSize& aSize, int32_t aStride,
    SurfaceFormat aFormat) const {
  RefPtr<SourceSurface> surf =
      mFinalDT->CreateSourceSurfaceFromData(aData, aSize, aStride, aFormat);
>>>>>>> upstream-releases

  RefPtr<SourceSurface> retSurf =
      new SourceSurfaceWrapAndRecord(surf, mRecorder);

  mRecorder->RecordEvent(
      RecordedSourceSurfaceCreation(retSurf, aData, aStride, aSize, aFormat));

  return retSurf.forget();
}

<<<<<<< HEAD
already_AddRefed<SourceSurface> DrawTargetWrapAndRecord::OptimizeSourceSurface(
    SourceSurface *aSurface) const {
||||||| merged common ancestors
already_AddRefed<SourceSurface>
DrawTargetWrapAndRecord::OptimizeSourceSurface(SourceSurface *aSurface) const
{
=======
already_AddRefed<SourceSurface> DrawTargetWrapAndRecord::OptimizeSourceSurface(
    SourceSurface* aSurface) const {
>>>>>>> upstream-releases
  RefPtr<SourceSurface> surf = mFinalDT->OptimizeSourceSurface(aSurface);

  RefPtr<SourceSurface> retSurf =
      new SourceSurfaceWrapAndRecord(surf, mRecorder);

  RefPtr<DataSourceSurface> dataSurf = surf->GetDataSurface();

  if (!dataSurf) {
    // Let's try get it off the original surface.
    dataSurf = aSurface->GetDataSurface();
  }

  StoreSourceSurface(mRecorder, retSurf, dataSurf, "OptimizeSourceSurface");

  return retSurf.forget();
}

already_AddRefed<SourceSurface>
<<<<<<< HEAD
DrawTargetWrapAndRecord::CreateSourceSurfaceFromNativeSurface(
    const NativeSurface &aSurface) const {
  RefPtr<SourceSurface> surf =
      mFinalDT->CreateSourceSurfaceFromNativeSurface(aSurface);
||||||| merged common ancestors
DrawTargetWrapAndRecord::CreateSourceSurfaceFromNativeSurface(const NativeSurface &aSurface) const
{
  RefPtr<SourceSurface> surf = mFinalDT->CreateSourceSurfaceFromNativeSurface(aSurface);
=======
DrawTargetWrapAndRecord::CreateSourceSurfaceFromNativeSurface(
    const NativeSurface& aSurface) const {
  RefPtr<SourceSurface> surf =
      mFinalDT->CreateSourceSurfaceFromNativeSurface(aSurface);
>>>>>>> upstream-releases

  RefPtr<SourceSurface> retSurf =
      new SourceSurfaceWrapAndRecord(surf, mRecorder);

  RefPtr<DataSourceSurface> dataSurf = surf->GetDataSurface();
  StoreSourceSurface(mRecorder, retSurf, dataSurf,
                     "CreateSourceSurfaceFromNativeSurface");

  return retSurf.forget();
}

<<<<<<< HEAD
already_AddRefed<DrawTarget> DrawTargetWrapAndRecord::CreateSimilarDrawTarget(
    const IntSize &aSize, SurfaceFormat aFormat) const {
||||||| merged common ancestors
already_AddRefed<DrawTarget>
DrawTargetWrapAndRecord::CreateSimilarDrawTarget(const IntSize &aSize, SurfaceFormat aFormat) const
{
=======
already_AddRefed<DrawTarget> DrawTargetWrapAndRecord::CreateSimilarDrawTarget(
    const IntSize& aSize, SurfaceFormat aFormat) const {
>>>>>>> upstream-releases
  RefPtr<DrawTarget> similarDT =
      mFinalDT->CreateSimilarDrawTarget(aSize, aFormat);
  if (!similarDT) {
    return nullptr;
  }

  similarDT = new DrawTargetWrapAndRecord(this, similarDT);
  return similarDT.forget();
}

<<<<<<< HEAD
bool DrawTargetWrapAndRecord::CanCreateSimilarDrawTarget(
    const IntSize &aSize, SurfaceFormat aFormat) const {
  return mFinalDT->CanCreateSimilarDrawTarget(aSize, aFormat);
}

already_AddRefed<PathBuilder> DrawTargetWrapAndRecord::CreatePathBuilder(
    FillRule aFillRule) const {
||||||| merged common ancestors
already_AddRefed<PathBuilder>
DrawTargetWrapAndRecord::CreatePathBuilder(FillRule aFillRule) const
{
=======
bool DrawTargetWrapAndRecord::CanCreateSimilarDrawTarget(
    const IntSize& aSize, SurfaceFormat aFormat) const {
  return mFinalDT->CanCreateSimilarDrawTarget(aSize, aFormat);
}

RefPtr<DrawTarget> DrawTargetWrapAndRecord::CreateClippedDrawTarget(
    const Rect& aBounds, SurfaceFormat aFormat) {
  RefPtr<DrawTarget> similarDT;
  RefPtr<DrawTarget> innerDT =
      mFinalDT->CreateClippedDrawTarget(aBounds, aFormat);
  similarDT = new DrawTargetWrapAndRecord(this->mRecorder, innerDT);
  mRecorder->RecordEvent(
      RecordedCreateClippedDrawTarget(similarDT.get(), aBounds, aFormat));
  return similarDT;
}

already_AddRefed<PathBuilder> DrawTargetWrapAndRecord::CreatePathBuilder(
    FillRule aFillRule) const {
>>>>>>> upstream-releases
  RefPtr<PathBuilder> builder = mFinalDT->CreatePathBuilder(aFillRule);
  return MakeAndAddRef<PathBuilderRecording>(builder, aFillRule);
}

<<<<<<< HEAD
already_AddRefed<GradientStops> DrawTargetWrapAndRecord::CreateGradientStops(
    GradientStop *aStops, uint32_t aNumStops, ExtendMode aExtendMode) const {
  RefPtr<GradientStops> stops =
      mFinalDT->CreateGradientStops(aStops, aNumStops, aExtendMode);
||||||| merged common ancestors
already_AddRefed<GradientStops>
DrawTargetWrapAndRecord::CreateGradientStops(GradientStop *aStops,
                                         uint32_t aNumStops,
                                         ExtendMode aExtendMode) const
{
  RefPtr<GradientStops> stops = mFinalDT->CreateGradientStops(aStops, aNumStops, aExtendMode);
=======
already_AddRefed<GradientStops> DrawTargetWrapAndRecord::CreateGradientStops(
    GradientStop* aStops, uint32_t aNumStops, ExtendMode aExtendMode) const {
  RefPtr<GradientStops> stops =
      mFinalDT->CreateGradientStops(aStops, aNumStops, aExtendMode);
>>>>>>> upstream-releases

  RefPtr<GradientStops> retStops =
      new GradientStopsWrapAndRecord(stops, mRecorder);

  mRecorder->RecordEvent(
      RecordedGradientStopsCreation(retStops, aStops, aNumStops, aExtendMode));

  return retStops.forget();
}

<<<<<<< HEAD
void DrawTargetWrapAndRecord::SetTransform(const Matrix &aTransform) {
||||||| merged common ancestors
void
DrawTargetWrapAndRecord::SetTransform(const Matrix &aTransform)
{
=======
void DrawTargetWrapAndRecord::SetTransform(const Matrix& aTransform) {
>>>>>>> upstream-releases
  mRecorder->RecordEvent(RecordedSetTransform(this, aTransform));
  DrawTarget::SetTransform(aTransform);
  mFinalDT->SetTransform(aTransform);
}

<<<<<<< HEAD
already_AddRefed<PathRecording> DrawTargetWrapAndRecord::EnsurePathStored(
    const Path *aPath) {
||||||| merged common ancestors
already_AddRefed<PathRecording>
DrawTargetWrapAndRecord::EnsurePathStored(const Path *aPath)
{
=======
already_AddRefed<PathRecording> DrawTargetWrapAndRecord::EnsurePathStored(
    const Path* aPath) {
>>>>>>> upstream-releases
  RefPtr<PathRecording> pathWrapAndRecord;
  if (aPath->GetBackendType() == BackendType::RECORDING) {
<<<<<<< HEAD
    pathWrapAndRecord =
        const_cast<PathRecording *>(static_cast<const PathRecording *>(aPath));
||||||| merged common ancestors
    pathWrapAndRecord = const_cast<PathRecording*>(static_cast<const PathRecording*>(aPath));
=======
    pathWrapAndRecord =
        const_cast<PathRecording*>(static_cast<const PathRecording*>(aPath));
>>>>>>> upstream-releases
    if (mRecorder->HasStoredObject(aPath)) {
      return pathWrapAndRecord.forget();
    }
  } else {
    MOZ_ASSERT(!mRecorder->HasStoredObject(aPath));
    FillRule fillRule = aPath->GetFillRule();
    RefPtr<PathBuilder> builder = mFinalDT->CreatePathBuilder(fillRule);
    RefPtr<PathBuilderRecording> builderWrapAndRecord =
        new PathBuilderRecording(builder, fillRule);
    aPath->StreamToSink(builderWrapAndRecord);
    pathWrapAndRecord =
        builderWrapAndRecord->Finish().downcast<PathRecording>();
  }

  mRecorder->RecordEvent(RecordedPathCreation(pathWrapAndRecord.get()));
  mRecorder->AddStoredObject(pathWrapAndRecord);
  pathWrapAndRecord->mStoredRecorders.push_back(mRecorder);

  return pathWrapAndRecord.forget();
}

<<<<<<< HEAD
void DrawTargetWrapAndRecord::EnsurePatternDependenciesStored(
    const Pattern &aPattern) {
||||||| merged common ancestors
void
DrawTargetWrapAndRecord::EnsurePatternDependenciesStored(const Pattern &aPattern)
{
=======
void DrawTargetWrapAndRecord::EnsurePatternDependenciesStored(
    const Pattern& aPattern) {
>>>>>>> upstream-releases
  switch (aPattern.GetType()) {
<<<<<<< HEAD
    case PatternType::COLOR:
      // No dependencies here.
      return;
    case PatternType::LINEAR_GRADIENT: {
      MOZ_ASSERT(mRecorder->HasStoredObject(
          static_cast<const LinearGradientPattern *>(&aPattern)->mStops));
||||||| merged common ancestors
  case PatternType::COLOR:
    // No dependencies here.
    return;
  case PatternType::LINEAR_GRADIENT:
    {
      MOZ_ASSERT(mRecorder->HasStoredObject(static_cast<const LinearGradientPattern*>(&aPattern)->mStops));
=======
    case PatternType::COLOR:
      // No dependencies here.
      return;
    case PatternType::LINEAR_GRADIENT: {
      MOZ_ASSERT(mRecorder->HasStoredObject(
          static_cast<const LinearGradientPattern*>(&aPattern)->mStops));
>>>>>>> upstream-releases
      return;
    }
<<<<<<< HEAD
    case PatternType::RADIAL_GRADIENT: {
      MOZ_ASSERT(mRecorder->HasStoredObject(
          static_cast<const RadialGradientPattern *>(&aPattern)->mStops));
||||||| merged common ancestors
  case PatternType::RADIAL_GRADIENT:
    {
      MOZ_ASSERT(mRecorder->HasStoredObject(static_cast<const RadialGradientPattern*>(&aPattern)->mStops));
=======
    case PatternType::RADIAL_GRADIENT: {
      MOZ_ASSERT(mRecorder->HasStoredObject(
          static_cast<const RadialGradientPattern*>(&aPattern)->mStops));
>>>>>>> upstream-releases
      return;
    }
<<<<<<< HEAD
    case PatternType::SURFACE: {
      const SurfacePattern *pat =
          static_cast<const SurfacePattern *>(&aPattern);
      EnsureSurfaceStored(mRecorder, pat->mSurface,
                          "EnsurePatternDependenciesStored");
||||||| merged common ancestors
  case PatternType::SURFACE:
    {
      const SurfacePattern *pat = static_cast<const SurfacePattern*>(&aPattern);
      EnsureSurfaceStored(mRecorder, pat->mSurface, "EnsurePatternDependenciesStored");
=======
    case PatternType::SURFACE: {
      const SurfacePattern* pat = static_cast<const SurfacePattern*>(&aPattern);
      EnsureSurfaceStored(mRecorder, pat->mSurface,
                          "EnsurePatternDependenciesStored");
>>>>>>> upstream-releases
      return;
    }
  }
}

}  // namespace gfx
}  // namespace mozilla
