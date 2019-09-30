/* -*- Mode: C++; tab-width: 8; indent-tabs-mode: nil; c-basic-offset: 2 -*- */
/* vim: set ts=8 sts=2 et sw=2 tw=80: */
/* This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/. */

#include "DrawTargetRecording.h"
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
struct RecordingSourceSurfaceUserData {
  void *refPtr;
||||||| merged common ancestors
struct RecordingSourceSurfaceUserData
{
  void *refPtr;
=======
struct RecordingSourceSurfaceUserData {
  void* refPtr;
>>>>>>> upstream-releases
  RefPtr<DrawEventRecorderPrivate> recorder;
};

<<<<<<< HEAD
void RecordingSourceSurfaceUserDataFunc(void *aUserData) {
  RecordingSourceSurfaceUserData *userData =
      static_cast<RecordingSourceSurfaceUserData *>(aUserData);
||||||| merged common ancestors
void RecordingSourceSurfaceUserDataFunc(void *aUserData)
{
  RecordingSourceSurfaceUserData *userData =
    static_cast<RecordingSourceSurfaceUserData*>(aUserData);
=======
static void RecordingSourceSurfaceUserDataFunc(void* aUserData) {
  RecordingSourceSurfaceUserData* userData =
      static_cast<RecordingSourceSurfaceUserData*>(aUserData);
>>>>>>> upstream-releases

  userData->recorder->RemoveSourceSurface((SourceSurface *)userData->refPtr);
  userData->recorder->RemoveStoredObject(userData->refPtr);
  userData->recorder->RecordEvent(
      RecordedSourceSurfaceDestruction(ReferencePtr(userData->refPtr)));

  delete userData;
}

<<<<<<< HEAD
static void EnsureSurfaceStoredRecording(DrawEventRecorderPrivate *aRecorder,
                                         SourceSurface *aSurface,
                                         const char *reason) {
||||||| merged common ancestors
static void
EnsureSurfaceStoredRecording(DrawEventRecorderPrivate *aRecorder, SourceSurface *aSurface,
                    const char *reason)
{
=======
static void EnsureSurfaceStoredRecording(DrawEventRecorderPrivate* aRecorder,
                                         SourceSurface* aSurface,
                                         const char* reason) {
>>>>>>> upstream-releases
  if (aRecorder->HasStoredObject(aSurface)) {
    return;
  }

  aRecorder->StoreSourceSurfaceRecording(aSurface, reason);
  aRecorder->AddStoredObject(aSurface);
  aRecorder->AddSourceSurface(aSurface);

  RecordingSourceSurfaceUserData* userData = new RecordingSourceSurfaceUserData;
  userData->refPtr = aSurface;
  userData->recorder = aRecorder;
<<<<<<< HEAD
  aSurface->AddUserData(reinterpret_cast<UserDataKey *>(aRecorder), userData,
                        &RecordingSourceSurfaceUserDataFunc);
||||||| merged common ancestors
  aSurface->AddUserData(reinterpret_cast<UserDataKey*>(aRecorder),
                        userData, &RecordingSourceSurfaceUserDataFunc);
=======
  aSurface->AddUserData(reinterpret_cast<UserDataKey*>(aRecorder), userData,
                        &RecordingSourceSurfaceUserDataFunc);
>>>>>>> upstream-releases
}

class SourceSurfaceRecording : public SourceSurface {
 public:
  MOZ_DECLARE_REFCOUNTED_VIRTUAL_TYPENAME(SourceSurfaceRecording, override)

<<<<<<< HEAD
  SourceSurfaceRecording(IntSize aSize, SurfaceFormat aFormat,
                         DrawEventRecorderPrivate *aRecorder)
      : mSize(aSize), mFormat(aFormat), mRecorder(aRecorder) {
||||||| merged common ancestors
  SourceSurfaceRecording(IntSize aSize, SurfaceFormat aFormat, DrawEventRecorderPrivate *aRecorder)
    : mSize(aSize), mFormat(aFormat), mRecorder(aRecorder)
  {
=======
  SourceSurfaceRecording(IntSize aSize, SurfaceFormat aFormat,
                         DrawEventRecorderPrivate* aRecorder)
      : mSize(aSize), mFormat(aFormat), mRecorder(aRecorder) {
>>>>>>> upstream-releases
    mRecorder->AddStoredObject(this);
  }

  ~SourceSurfaceRecording() {
    mRecorder->RemoveStoredObject(this);
    mRecorder->RecordEvent(
        RecordedSourceSurfaceDestruction(ReferencePtr(this)));
  }

<<<<<<< HEAD
  virtual SurfaceType GetType() const override {
    return SurfaceType::RECORDING;
  }
  virtual IntSize GetSize() const override { return mSize; }
  virtual SurfaceFormat GetFormat() const override { return mFormat; }
  virtual already_AddRefed<DataSourceSurface> GetDataSurface() override {
    return nullptr;
  }
||||||| merged common ancestors
  virtual SurfaceType GetType() const override { return SurfaceType::RECORDING; }
  virtual IntSize GetSize() const override { return mSize; }
  virtual SurfaceFormat GetFormat() const override { return mFormat; }
  virtual already_AddRefed<DataSourceSurface> GetDataSurface() override { return nullptr; }
=======
  SurfaceType GetType() const override { return SurfaceType::RECORDING; }
  IntSize GetSize() const override { return mSize; }
  SurfaceFormat GetFormat() const override { return mFormat; }
  already_AddRefed<DataSourceSurface> GetDataSurface() override {
    return nullptr;
  }
>>>>>>> upstream-releases

  IntSize mSize;
  SurfaceFormat mFormat;
  RefPtr<DrawEventRecorderPrivate> mRecorder;
};

class DataSourceSurfaceRecording : public DataSourceSurface {
 public:
  MOZ_DECLARE_REFCOUNTED_VIRTUAL_TYPENAME(DataSourceSurfaceRecording, override)
  DataSourceSurfaceRecording(UniquePtr<uint8_t[]> aData, IntSize aSize,
<<<<<<< HEAD
                             int32_t aStride, SurfaceFormat aFormat)
      : mData(std::move(aData)),
        mSize(aSize),
        mStride(aStride),
        mFormat(aFormat) {}
||||||| merged common ancestors
                             int32_t aStride, SurfaceFormat aFormat)
    : mData(std::move(aData))
    , mSize(aSize)
    , mStride(aStride)
    , mFormat(aFormat)
  {
  }
=======
                             int32_t aStride, SurfaceFormat aFormat,
                             DrawEventRecorderPrivate* aRecorder)
      : mData(std::move(aData)),
        mSize(aSize),
        mStride(aStride),
        mFormat(aFormat),
        mRecorder(aRecorder) {
    mRecorder->RecordEvent(RecordedSourceSurfaceCreation(
        ReferencePtr(this), mData.get(), mStride, mSize, mFormat));
    mRecorder->AddStoredObject(this);
  }
>>>>>>> upstream-releases

<<<<<<< HEAD
  ~DataSourceSurfaceRecording() {}
||||||| merged common ancestors
  ~DataSourceSurfaceRecording()
  {
  }
=======
  ~DataSourceSurfaceRecording() {
    mRecorder->RemoveStoredObject(this);
    mRecorder->RecordEvent(
        RecordedSourceSurfaceDestruction(ReferencePtr(this)));
  }
>>>>>>> upstream-releases

<<<<<<< HEAD
  static already_AddRefed<DataSourceSurface> Init(uint8_t *aData, IntSize aSize,
                                                  int32_t aStride,
                                                  SurfaceFormat aFormat) {
    // XXX: do we need to ensure any alignment here?
||||||| merged common ancestors
  static already_AddRefed<DataSourceSurface>
  Init(uint8_t *aData, IntSize aSize, int32_t aStride, SurfaceFormat aFormat)
  {
    //XXX: do we need to ensure any alignment here?
=======
  static already_AddRefed<DataSourceSurface> Init(
      uint8_t* aData, IntSize aSize, int32_t aStride, SurfaceFormat aFormat,
      DrawEventRecorderPrivate* aRecorder) {
    // XXX: do we need to ensure any alignment here?
>>>>>>> upstream-releases
    auto data = MakeUnique<uint8_t[]>(aStride * aSize.height);
    if (data) {
      memcpy(data.get(), aData, aStride * aSize.height);
<<<<<<< HEAD
      RefPtr<DataSourceSurfaceRecording> surf = new DataSourceSurfaceRecording(
          std::move(data), aSize, aStride, aFormat);
||||||| merged common ancestors
      RefPtr<DataSourceSurfaceRecording> surf = new DataSourceSurfaceRecording(std::move(data), aSize, aStride, aFormat);
=======
      RefPtr<DataSourceSurfaceRecording> surf = new DataSourceSurfaceRecording(
          std::move(data), aSize, aStride, aFormat, aRecorder);
>>>>>>> upstream-releases
      return surf.forget();
    }
    return nullptr;
  }

<<<<<<< HEAD
  virtual SurfaceType GetType() const override {
    return SurfaceType::RECORDING;
  }
  virtual IntSize GetSize() const override { return mSize; }
  virtual int32_t Stride() override { return mStride; }
  virtual SurfaceFormat GetFormat() const override { return mFormat; }
  virtual uint8_t *GetData() override { return mData.get(); }
||||||| merged common ancestors
  virtual SurfaceType GetType() const override { return SurfaceType::RECORDING; }
  virtual IntSize GetSize() const override { return mSize; }
  virtual int32_t Stride() override { return mStride; }
  virtual SurfaceFormat GetFormat() const override { return mFormat; }
  virtual uint8_t* GetData() override { return mData.get(); }
=======
  SurfaceType GetType() const override { return SurfaceType::RECORDING; }
  IntSize GetSize() const override { return mSize; }
  int32_t Stride() override { return mStride; }
  SurfaceFormat GetFormat() const override { return mFormat; }
  uint8_t* GetData() override { return mData.get(); }
>>>>>>> upstream-releases

  UniquePtr<uint8_t[]> mData;
  IntSize mSize;
  int32_t mStride;
  SurfaceFormat mFormat;
  RefPtr<DrawEventRecorderPrivate> mRecorder;
};

class GradientStopsRecording : public GradientStops {
 public:
  MOZ_DECLARE_REFCOUNTED_VIRTUAL_TYPENAME(GradientStopsRecording, override)

<<<<<<< HEAD
  explicit GradientStopsRecording(DrawEventRecorderPrivate *aRecorder)
      : mRecorder(aRecorder) {
||||||| merged common ancestors
  explicit GradientStopsRecording(DrawEventRecorderPrivate *aRecorder)
    : mRecorder(aRecorder)
  {
=======
  explicit GradientStopsRecording(DrawEventRecorderPrivate* aRecorder)
      : mRecorder(aRecorder) {
>>>>>>> upstream-releases
    mRecorder->AddStoredObject(this);
  }

<<<<<<< HEAD
  ~GradientStopsRecording() {
||||||| merged common ancestors
  ~GradientStopsRecording()
  {
=======
  virtual ~GradientStopsRecording() {
>>>>>>> upstream-releases
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

  RefPtr<DrawEventRecorderPrivate> mRecorder;
};

class FilterNodeRecording : public FilterNode {
 public:
  MOZ_DECLARE_REFCOUNTED_VIRTUAL_TYPENAME(FilterNodeRecording, override)
  using FilterNode::SetAttribute;

<<<<<<< HEAD
  explicit FilterNodeRecording(DrawEventRecorderPrivate *aRecorder)
      : mRecorder(aRecorder) {
||||||| merged common ancestors
  explicit FilterNodeRecording(DrawEventRecorderPrivate *aRecorder)
    : mRecorder(aRecorder)
  {
=======
  explicit FilterNodeRecording(DrawEventRecorderPrivate* aRecorder)
      : mRecorder(aRecorder) {
>>>>>>> upstream-releases
    mRecorder->AddStoredObject(this);
  }

<<<<<<< HEAD
  ~FilterNodeRecording() {
||||||| merged common ancestors
  ~FilterNodeRecording()
  {
=======
  virtual ~FilterNodeRecording() {
>>>>>>> upstream-releases
    mRecorder->RemoveStoredObject(this);
    mRecorder->RecordEvent(RecordedFilterNodeDestruction(ReferencePtr(this)));
  }

<<<<<<< HEAD
  virtual void SetInput(uint32_t aIndex, SourceSurface *aSurface) override {
    EnsureSurfaceStoredRecording(mRecorder, aSurface, "SetInput");
||||||| merged common ancestors
  virtual void SetInput(uint32_t aIndex, SourceSurface *aSurface) override
  {
    EnsureSurfaceStoredRecording(mRecorder, aSurface,  "SetInput");
=======
  void SetInput(uint32_t aIndex, SourceSurface* aSurface) override {
    EnsureSurfaceStoredRecording(mRecorder, aSurface, "SetInput");
>>>>>>> upstream-releases

    mRecorder->RecordEvent(RecordedFilterNodeSetInput(this, aIndex, aSurface));
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
  }

<<<<<<< HEAD
#define FORWARD_SET_ATTRIBUTE(type, argtype)                         \
  virtual void SetAttribute(uint32_t aIndex, type aValue) override { \
    mRecorder->RecordEvent(RecordedFilterNodeSetAttribute(           \
        this, aIndex, aValue,                                        \
        RecordedFilterNodeSetAttribute::ARGTYPE_##argtype));         \
||||||| merged common ancestors
#define FORWARD_SET_ATTRIBUTE(type, argtype) \
  virtual void SetAttribute(uint32_t aIndex, type aValue) override { \
    mRecorder->RecordEvent(RecordedFilterNodeSetAttribute(this, aIndex, aValue, RecordedFilterNodeSetAttribute::ARGTYPE_##argtype)); \
=======
#define FORWARD_SET_ATTRIBUTE(type, argtype)                 \
  void SetAttribute(uint32_t aIndex, type aValue) override { \
    mRecorder->RecordEvent(RecordedFilterNodeSetAttribute(   \
        this, aIndex, aValue,                                \
        RecordedFilterNodeSetAttribute::ARGTYPE_##argtype)); \
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
  void SetAttribute(uint32_t aIndex, const Float* aFloat,
                    uint32_t aSize) override {
    mRecorder->RecordEvent(
        RecordedFilterNodeSetAttribute(this, aIndex, aFloat, aSize));
>>>>>>> upstream-releases
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

  RefPtr<DrawEventRecorderPrivate> mRecorder;
};

<<<<<<< HEAD
DrawTargetRecording::DrawTargetRecording(DrawEventRecorder *aRecorder,
                                         DrawTarget *aDT, IntSize aSize,
                                         bool aHasData)
    : mRecorder(static_cast<DrawEventRecorderPrivate *>(aRecorder)),
      mFinalDT(aDT),
      mSize(aSize) {
||||||| merged common ancestors
DrawTargetRecording::DrawTargetRecording(DrawEventRecorder *aRecorder, DrawTarget *aDT, IntSize aSize, bool aHasData)
  : mRecorder(static_cast<DrawEventRecorderPrivate*>(aRecorder))
  , mFinalDT(aDT)
  , mSize(aSize)
{
=======
DrawTargetRecording::DrawTargetRecording(DrawEventRecorder* aRecorder,
                                         DrawTarget* aDT, IntSize aSize,
                                         bool aHasData)
    : mRecorder(static_cast<DrawEventRecorderPrivate*>(aRecorder)),
      mFinalDT(aDT),
      mSize(aSize) {
>>>>>>> upstream-releases
  RefPtr<SourceSurface> snapshot = aHasData ? mFinalDT->Snapshot() : nullptr;
  mRecorder->RecordEvent(
      RecordedDrawTargetCreation(this, mFinalDT->GetBackendType(), mSize,
                                 mFinalDT->GetFormat(), aHasData, snapshot));
  mFormat = mFinalDT->GetFormat();
}

<<<<<<< HEAD
DrawTargetRecording::DrawTargetRecording(const DrawTargetRecording *aDT,
                                         IntSize aSize, SurfaceFormat aFormat)
    : mRecorder(aDT->mRecorder), mFinalDT(aDT->mFinalDT), mSize(aSize) {
||||||| merged common ancestors
DrawTargetRecording::DrawTargetRecording(const DrawTargetRecording *aDT,
                                         IntSize aSize,
                                         SurfaceFormat aFormat)
  : mRecorder(aDT->mRecorder)
  , mFinalDT(aDT->mFinalDT)
  , mSize(aSize)
{
=======
DrawTargetRecording::DrawTargetRecording(const DrawTargetRecording* aDT,
                                         IntSize aSize, SurfaceFormat aFormat)
    : mRecorder(aDT->mRecorder), mFinalDT(aDT->mFinalDT), mSize(aSize) {
>>>>>>> upstream-releases
  mFormat = aFormat;
}

DrawTargetRecording::~DrawTargetRecording() {
  mRecorder->RecordEvent(RecordedDrawTargetDestruction(ReferencePtr(this)));
}

<<<<<<< HEAD
void DrawTargetRecording::FillRect(const Rect &aRect, const Pattern &aPattern,
                                   const DrawOptions &aOptions) {
||||||| merged common ancestors
void
DrawTargetRecording::FillRect(const Rect &aRect,
                              const Pattern &aPattern,
                              const DrawOptions &aOptions)
{
=======
void DrawTargetRecording::FillRect(const Rect& aRect, const Pattern& aPattern,
                                   const DrawOptions& aOptions) {
>>>>>>> upstream-releases
  EnsurePatternDependenciesStored(aPattern);

  mRecorder->RecordEvent(RecordedFillRect(this, aRect, aPattern, aOptions));
}

<<<<<<< HEAD
void DrawTargetRecording::StrokeRect(const Rect &aRect, const Pattern &aPattern,
                                     const StrokeOptions &aStrokeOptions,
                                     const DrawOptions &aOptions) {
||||||| merged common ancestors
void
DrawTargetRecording::StrokeRect(const Rect &aRect,
                                const Pattern &aPattern,
                                const StrokeOptions &aStrokeOptions,
                                const DrawOptions &aOptions)
{
=======
void DrawTargetRecording::StrokeRect(const Rect& aRect, const Pattern& aPattern,
                                     const StrokeOptions& aStrokeOptions,
                                     const DrawOptions& aOptions) {
>>>>>>> upstream-releases
  EnsurePatternDependenciesStored(aPattern);

  mRecorder->RecordEvent(
      RecordedStrokeRect(this, aRect, aPattern, aStrokeOptions, aOptions));
}

<<<<<<< HEAD
void DrawTargetRecording::StrokeLine(const Point &aBegin, const Point &aEnd,
                                     const Pattern &aPattern,
                                     const StrokeOptions &aStrokeOptions,
                                     const DrawOptions &aOptions) {
||||||| merged common ancestors
void
DrawTargetRecording::StrokeLine(const Point &aBegin,
                                const Point &aEnd,
                                const Pattern &aPattern,
                                const StrokeOptions &aStrokeOptions,
                                const DrawOptions &aOptions)
{
=======
void DrawTargetRecording::StrokeLine(const Point& aBegin, const Point& aEnd,
                                     const Pattern& aPattern,
                                     const StrokeOptions& aStrokeOptions,
                                     const DrawOptions& aOptions) {
>>>>>>> upstream-releases
  EnsurePatternDependenciesStored(aPattern);

  mRecorder->RecordEvent(RecordedStrokeLine(this, aBegin, aEnd, aPattern,
                                            aStrokeOptions, aOptions));
}

<<<<<<< HEAD
void DrawTargetRecording::Fill(const Path *aPath, const Pattern &aPattern,
                               const DrawOptions &aOptions) {
||||||| merged common ancestors
void
DrawTargetRecording::Fill(const Path *aPath,
                          const Pattern &aPattern,
                          const DrawOptions &aOptions)
{
=======
void DrawTargetRecording::Fill(const Path* aPath, const Pattern& aPattern,
                               const DrawOptions& aOptions) {
>>>>>>> upstream-releases
  RefPtr<PathRecording> pathRecording = EnsurePathStored(aPath);
  EnsurePatternDependenciesStored(aPattern);

  mRecorder->RecordEvent(RecordedFill(this, pathRecording, aPattern, aOptions));
}

<<<<<<< HEAD
struct RecordingFontUserData {
  void *refPtr;
||||||| merged common ancestors
struct RecordingFontUserData
{
  void *refPtr;
=======
struct RecordingFontUserData {
  void* refPtr;
>>>>>>> upstream-releases
  RefPtr<DrawEventRecorderPrivate> recorder;
};

<<<<<<< HEAD
void RecordingFontUserDataDestroyFunc(void *aUserData) {
  RecordingFontUserData *userData =
      static_cast<RecordingFontUserData *>(aUserData);
||||||| merged common ancestors
void RecordingFontUserDataDestroyFunc(void *aUserData)
{
  RecordingFontUserData *userData =
    static_cast<RecordingFontUserData*>(aUserData);
=======
static void RecordingFontUserDataDestroyFunc(void* aUserData) {
  RecordingFontUserData* userData =
      static_cast<RecordingFontUserData*>(aUserData);
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
void DrawTargetRecording::FillGlyphs(ScaledFont *aFont,
                                     const GlyphBuffer &aBuffer,
                                     const Pattern &aPattern,
                                     const DrawOptions &aOptions) {
||||||| merged common ancestors
void
DrawTargetRecording::FillGlyphs(ScaledFont *aFont,
                                const GlyphBuffer &aBuffer,
                                const Pattern &aPattern,
                                const DrawOptions &aOptions)
{
=======
void DrawTargetRecording::FillGlyphs(ScaledFont* aFont,
                                     const GlyphBuffer& aBuffer,
                                     const Pattern& aPattern,
                                     const DrawOptions& aOptions) {
>>>>>>> upstream-releases
  EnsurePatternDependenciesStored(aPattern);

  UserDataKey *userDataKey = reinterpret_cast<UserDataKey *>(mRecorder.get());
  if (mRecorder->WantsExternalFonts()) {
    mRecorder->AddScaledFont(aFont);
  } else if (!aFont->GetUserData(userDataKey)) {
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
          gfxWarning() << "DrawTargetRecording::FillGlyphs failed to serialise "
                          "UnscaledFont";
        }
      }
      mRecorder->AddStoredObject(unscaledFont);
    }
    mRecorder->RecordEvent(RecordedScaledFontCreation(aFont, unscaledFont));
    RecordingFontUserData* userData = new RecordingFontUserData;
    userData->refPtr = aFont;
    userData->recorder = mRecorder;
    aFont->AddUserData(userDataKey, userData,
                       &RecordingFontUserDataDestroyFunc);
    userData->recorder->AddScaledFont(aFont);
  }

  mRecorder->RecordEvent(RecordedFillGlyphs(
      this, aFont, aPattern, aOptions, aBuffer.mGlyphs, aBuffer.mNumGlyphs));
}

<<<<<<< HEAD
void DrawTargetRecording::Mask(const Pattern &aSource, const Pattern &aMask,
                               const DrawOptions &aOptions) {
||||||| merged common ancestors
void
DrawTargetRecording::Mask(const Pattern &aSource,
                          const Pattern &aMask,
                          const DrawOptions &aOptions)
{
=======
void DrawTargetRecording::Mask(const Pattern& aSource, const Pattern& aMask,
                               const DrawOptions& aOptions) {
>>>>>>> upstream-releases
  EnsurePatternDependenciesStored(aSource);
  EnsurePatternDependenciesStored(aMask);

  mRecorder->RecordEvent(RecordedMask(this, aSource, aMask, aOptions));
}

<<<<<<< HEAD
void DrawTargetRecording::MaskSurface(const Pattern &aSource,
                                      SourceSurface *aMask, Point aOffset,
                                      const DrawOptions &aOptions) {
||||||| merged common ancestors
void
DrawTargetRecording::MaskSurface(const Pattern &aSource,
                                 SourceSurface *aMask,
                                 Point aOffset,
                                 const DrawOptions &aOptions)
{
=======
void DrawTargetRecording::MaskSurface(const Pattern& aSource,
                                      SourceSurface* aMask, Point aOffset,
                                      const DrawOptions& aOptions) {
>>>>>>> upstream-releases
  EnsurePatternDependenciesStored(aSource);
  EnsureSurfaceStoredRecording(mRecorder, aMask, "MaskSurface");

  mRecorder->RecordEvent(
      RecordedMaskSurface(this, aSource, aMask, aOffset, aOptions));
}

<<<<<<< HEAD
void DrawTargetRecording::Stroke(const Path *aPath, const Pattern &aPattern,
                                 const StrokeOptions &aStrokeOptions,
                                 const DrawOptions &aOptions) {
||||||| merged common ancestors
void
DrawTargetRecording::Stroke(const Path *aPath,
                            const Pattern &aPattern,
                            const StrokeOptions &aStrokeOptions,
                            const DrawOptions &aOptions)
{
=======
void DrawTargetRecording::Stroke(const Path* aPath, const Pattern& aPattern,
                                 const StrokeOptions& aStrokeOptions,
                                 const DrawOptions& aOptions) {
>>>>>>> upstream-releases
  RefPtr<PathRecording> pathRecording = EnsurePathStored(aPath);
  EnsurePatternDependenciesStored(aPattern);

  mRecorder->RecordEvent(
      RecordedStroke(this, pathRecording, aPattern, aStrokeOptions, aOptions));
}

already_AddRefed<SourceSurface> DrawTargetRecording::Snapshot() {
  RefPtr<SourceSurface> retSurf =
      new SourceSurfaceRecording(mSize, mFormat, mRecorder);

  mRecorder->RecordEvent(RecordedSnapshot(retSurf, this));

  return retSurf.forget();
}

already_AddRefed<SourceSurface> DrawTargetRecording::IntoLuminanceSource(
    LuminanceType aLuminanceType, float aOpacity) {
  RefPtr<SourceSurface> retSurf =
      new SourceSurfaceRecording(mSize, SurfaceFormat::A8, mRecorder);

  mRecorder->RecordEvent(
      RecordedIntoLuminanceSource(retSurf, this, aLuminanceType, aOpacity));

  return retSurf.forget();
}

<<<<<<< HEAD
void DrawTargetRecording::DetachAllSnapshots() {}
||||||| merged common ancestors
void
DrawTargetRecording::DetachAllSnapshots()
{
}
=======
void DrawTargetRecording::Flush() {
  mRecorder->RecordEvent(RecordedFlush(this));
}

void DrawTargetRecording::DetachAllSnapshots() {
  mRecorder->RecordEvent(RecordedDetachAllSnapshots(this));
}
>>>>>>> upstream-releases

<<<<<<< HEAD
void DrawTargetRecording::DrawSurface(SourceSurface *aSurface,
                                      const Rect &aDest, const Rect &aSource,
                                      const DrawSurfaceOptions &aSurfOptions,
                                      const DrawOptions &aOptions) {
||||||| merged common ancestors
void
DrawTargetRecording::DrawSurface(SourceSurface *aSurface,
                                 const Rect &aDest,
                                 const Rect &aSource,
                                 const DrawSurfaceOptions &aSurfOptions,
                                 const DrawOptions &aOptions)
{
=======
void DrawTargetRecording::DrawSurface(SourceSurface* aSurface,
                                      const Rect& aDest, const Rect& aSource,
                                      const DrawSurfaceOptions& aSurfOptions,
                                      const DrawOptions& aOptions) {
>>>>>>> upstream-releases
  EnsureSurfaceStoredRecording(mRecorder, aSurface, "DrawSurface");

  mRecorder->RecordEvent(RecordedDrawSurface(this, aSurface, aDest, aSource,
                                             aSurfOptions, aOptions));
}

<<<<<<< HEAD
void DrawTargetRecording::DrawDependentSurface(
    uint64_t aId, const Rect &aDest, const DrawSurfaceOptions &aSurfOptions,
    const DrawOptions &aOptions) {
||||||| merged common ancestors
void
DrawTargetRecording::DrawDependentSurface(uint64_t aId,
                                          const Rect &aDest,
                                          const DrawSurfaceOptions &aSurfOptions,
                                          const DrawOptions &aOptions)
{
=======
void DrawTargetRecording::DrawDependentSurface(
    uint64_t aId, const Rect& aDest, const DrawSurfaceOptions& aSurfOptions,
    const DrawOptions& aOptions) {
>>>>>>> upstream-releases
  mRecorder->AddDependentSurface(aId);
  mRecorder->RecordEvent(
      RecordedDrawDependentSurface(this, aId, aDest, aSurfOptions, aOptions));
}

<<<<<<< HEAD
void DrawTargetRecording::DrawSurfaceWithShadow(
    SourceSurface *aSurface, const Point &aDest, const Color &aColor,
    const Point &aOffset, Float aSigma, CompositionOp aOp) {
||||||| merged common ancestors
void
DrawTargetRecording::DrawSurfaceWithShadow(SourceSurface *aSurface,
                                           const Point &aDest,
                                           const Color &aColor,
                                           const Point &aOffset,
                                           Float aSigma,
                                           CompositionOp aOp)
{
=======
void DrawTargetRecording::DrawSurfaceWithShadow(
    SourceSurface* aSurface, const Point& aDest, const Color& aColor,
    const Point& aOffset, Float aSigma, CompositionOp aOp) {
>>>>>>> upstream-releases
  EnsureSurfaceStoredRecording(mRecorder, aSurface, "DrawSurfaceWithShadow");

  mRecorder->RecordEvent(RecordedDrawSurfaceWithShadow(
      this, aSurface, aDest, aColor, aOffset, aSigma, aOp));
}

<<<<<<< HEAD
void DrawTargetRecording::DrawFilter(FilterNode *aNode, const Rect &aSourceRect,
                                     const Point &aDestPoint,
                                     const DrawOptions &aOptions) {
||||||| merged common ancestors
void
DrawTargetRecording::DrawFilter(FilterNode *aNode,
                                const Rect &aSourceRect,
                                const Point &aDestPoint,
                                const DrawOptions &aOptions)
{
=======
void DrawTargetRecording::DrawFilter(FilterNode* aNode, const Rect& aSourceRect,
                                     const Point& aDestPoint,
                                     const DrawOptions& aOptions) {
>>>>>>> upstream-releases
  MOZ_ASSERT(mRecorder->HasStoredObject(aNode));

  mRecorder->RecordEvent(
      RecordedDrawFilter(this, aNode, aSourceRect, aDestPoint, aOptions));
}

already_AddRefed<FilterNode> DrawTargetRecording::CreateFilter(
    FilterType aType) {
  RefPtr<FilterNode> retNode = new FilterNodeRecording(mRecorder);

  mRecorder->RecordEvent(RecordedFilterNodeCreation(retNode, aType));

  return retNode.forget();
}

<<<<<<< HEAD
void DrawTargetRecording::ClearRect(const Rect &aRect) {
||||||| merged common ancestors
void
DrawTargetRecording::ClearRect(const Rect &aRect)
{
=======
void DrawTargetRecording::ClearRect(const Rect& aRect) {
>>>>>>> upstream-releases
  mRecorder->RecordEvent(RecordedClearRect(this, aRect));
}

<<<<<<< HEAD
void DrawTargetRecording::CopySurface(SourceSurface *aSurface,
                                      const IntRect &aSourceRect,
                                      const IntPoint &aDestination) {
||||||| merged common ancestors
void
DrawTargetRecording::CopySurface(SourceSurface *aSurface,
                                 const IntRect &aSourceRect,
                                 const IntPoint &aDestination)
{
=======
void DrawTargetRecording::CopySurface(SourceSurface* aSurface,
                                      const IntRect& aSourceRect,
                                      const IntPoint& aDestination) {
>>>>>>> upstream-releases
  EnsureSurfaceStoredRecording(mRecorder, aSurface, "CopySurface");

  mRecorder->RecordEvent(
      RecordedCopySurface(this, aSurface, aSourceRect, aDestination));
}

<<<<<<< HEAD
void DrawTargetRecording::PushClip(const Path *aPath) {
||||||| merged common ancestors
void
DrawTargetRecording::PushClip(const Path *aPath)
{
=======
void DrawTargetRecording::PushClip(const Path* aPath) {
>>>>>>> upstream-releases
  RefPtr<PathRecording> pathRecording = EnsurePathStored(aPath);

  mRecorder->RecordEvent(RecordedPushClip(this, pathRecording));
}

<<<<<<< HEAD
void DrawTargetRecording::PushClipRect(const Rect &aRect) {
||||||| merged common ancestors
void
DrawTargetRecording::PushClipRect(const Rect &aRect)
{
=======
void DrawTargetRecording::PushClipRect(const Rect& aRect) {
>>>>>>> upstream-releases
  mRecorder->RecordEvent(RecordedPushClipRect(this, aRect));
}

<<<<<<< HEAD
void DrawTargetRecording::PopClip() {
  mRecorder->RecordEvent(RecordedPopClip(static_cast<DrawTarget *>(this)));
||||||| merged common ancestors
void
DrawTargetRecording::PopClip()
{
  mRecorder->RecordEvent(RecordedPopClip(static_cast<DrawTarget*>(this)));
=======
void DrawTargetRecording::PopClip() {
  mRecorder->RecordEvent(RecordedPopClip(static_cast<DrawTarget*>(this)));
>>>>>>> upstream-releases
}

<<<<<<< HEAD
void DrawTargetRecording::PushLayer(bool aOpaque, Float aOpacity,
                                    SourceSurface *aMask,
                                    const Matrix &aMaskTransform,
                                    const IntRect &aBounds,
                                    bool aCopyBackground) {
||||||| merged common ancestors
void
DrawTargetRecording::PushLayer(bool aOpaque, Float aOpacity,
                               SourceSurface* aMask,
                               const Matrix& aMaskTransform,
                               const IntRect& aBounds, bool aCopyBackground)
{
=======
void DrawTargetRecording::PushLayer(bool aOpaque, Float aOpacity,
                                    SourceSurface* aMask,
                                    const Matrix& aMaskTransform,
                                    const IntRect& aBounds,
                                    bool aCopyBackground) {
>>>>>>> upstream-releases
  if (aMask) {
    EnsureSurfaceStoredRecording(mRecorder, aMask, "PushLayer");
  }

  mRecorder->RecordEvent(RecordedPushLayer(this, aOpaque, aOpacity, aMask,
                                           aMaskTransform, aBounds,
                                           aCopyBackground));
}

<<<<<<< HEAD
void DrawTargetRecording::PushLayerWithBlend(bool aOpaque, Float aOpacity,
                                             SourceSurface *aMask,
                                             const Matrix &aMaskTransform,
                                             const IntRect &aBounds,
                                             bool aCopyBackground,
                                             CompositionOp aCompositionOp) {
||||||| merged common ancestors
void
DrawTargetRecording::PushLayerWithBlend(bool aOpaque, Float aOpacity,
                                        SourceSurface* aMask,
                                        const Matrix& aMaskTransform,
                                        const IntRect& aBounds,
                                        bool aCopyBackground,
                                        CompositionOp aCompositionOp)
{
=======
void DrawTargetRecording::PushLayerWithBlend(bool aOpaque, Float aOpacity,
                                             SourceSurface* aMask,
                                             const Matrix& aMaskTransform,
                                             const IntRect& aBounds,
                                             bool aCopyBackground,
                                             CompositionOp aCompositionOp) {
>>>>>>> upstream-releases
  if (aMask) {
    EnsureSurfaceStoredRecording(mRecorder, aMask, "PushLayer");
  }

  mRecorder->RecordEvent(
      RecordedPushLayerWithBlend(this, aOpaque, aOpacity, aMask, aMaskTransform,
                                 aBounds, aCopyBackground, aCompositionOp));
}

<<<<<<< HEAD
void DrawTargetRecording::PopLayer() {
  mRecorder->RecordEvent(RecordedPopLayer(static_cast<DrawTarget *>(this)));
||||||| merged common ancestors
void
DrawTargetRecording::PopLayer()
{
  mRecorder->RecordEvent(RecordedPopLayer(static_cast<DrawTarget*>(this)));
=======
void DrawTargetRecording::PopLayer() {
  mRecorder->RecordEvent(RecordedPopLayer(static_cast<DrawTarget*>(this)));
>>>>>>> upstream-releases
}

already_AddRefed<SourceSurface>
DrawTargetRecording::CreateSourceSurfaceFromData(unsigned char* aData,
                                                 const IntSize& aSize,
                                                 int32_t aStride,
<<<<<<< HEAD
                                                 SurfaceFormat aFormat) const {
  RefPtr<SourceSurface> surf =
      DataSourceSurfaceRecording::Init(aData, aSize, aStride, aFormat);

  RefPtr<SourceSurface> retSurf =
      new SourceSurfaceRecording(aSize, aFormat, mRecorder);

  mRecorder->RecordEvent(
      RecordedSourceSurfaceCreation(retSurf, aData, aStride, aSize, aFormat));

  return retSurf.forget();
||||||| merged common ancestors
                                                 SurfaceFormat aFormat) const
{
  RefPtr<SourceSurface> surf = DataSourceSurfaceRecording::Init(aData, aSize, aStride, aFormat);

  RefPtr<SourceSurface> retSurf = new SourceSurfaceRecording(aSize, aFormat, mRecorder);

  mRecorder->RecordEvent(RecordedSourceSurfaceCreation(retSurf, aData, aStride, aSize, aFormat));

  return retSurf.forget();
=======
                                                 SurfaceFormat aFormat) const {
  RefPtr<SourceSurface> surf = DataSourceSurfaceRecording::Init(
      aData, aSize, aStride, aFormat, mRecorder);
  return surf.forget();
>>>>>>> upstream-releases
}

<<<<<<< HEAD
already_AddRefed<SourceSurface> DrawTargetRecording::OptimizeSourceSurface(
    SourceSurface *aSurface) const {
||||||| merged common ancestors
already_AddRefed<SourceSurface>
DrawTargetRecording::OptimizeSourceSurface(SourceSurface *aSurface) const
{
=======
already_AddRefed<SourceSurface> DrawTargetRecording::OptimizeSourceSurface(
    SourceSurface* aSurface) const {
>>>>>>> upstream-releases
  RefPtr<SourceSurface> surf(aSurface);
  return surf.forget();
}

already_AddRefed<SourceSurface>
<<<<<<< HEAD
DrawTargetRecording::CreateSourceSurfaceFromNativeSurface(
    const NativeSurface &aSurface) const {
||||||| merged common ancestors
DrawTargetRecording::CreateSourceSurfaceFromNativeSurface(const NativeSurface &aSurface) const
{
=======
DrawTargetRecording::CreateSourceSurfaceFromNativeSurface(
    const NativeSurface& aSurface) const {
>>>>>>> upstream-releases
  MOZ_ASSERT(false);
  return nullptr;
}

<<<<<<< HEAD
already_AddRefed<DrawTarget> DrawTargetRecording::CreateSimilarDrawTarget(
    const IntSize &aSize, SurfaceFormat aFormat) const {
  RefPtr<DrawTarget> similarDT;
  if (mFinalDT->CanCreateSimilarDrawTarget(aSize, aFormat)) {
    similarDT = new DrawTargetRecording(this, aSize, aFormat);
    mRecorder->RecordEvent(
        RecordedCreateSimilarDrawTarget(similarDT.get(), aSize, aFormat));
  } else if (XRE_IsContentProcess()) {
    // Crash any content process that calls this function with arguments that
    // would fail to create a similar draw target. We do this to root out bad
    // callers. We don't want to crash any important processes though so for
    // for those we'll just gracefully return nullptr.
    MOZ_CRASH(
        "Content-process DrawTargetRecording can't create requested similar "
        "drawtarget");
  }
||||||| merged common ancestors
already_AddRefed<DrawTarget>
DrawTargetRecording::CreateSimilarDrawTarget(const IntSize &aSize, SurfaceFormat aFormat) const
{
  RefPtr<DrawTarget> similarDT = new DrawTargetRecording(this, aSize, aFormat);
  mRecorder->RecordEvent(RecordedCreateSimilarDrawTarget(similarDT.get(),
                                                         aSize,
                                                         aFormat));
=======
already_AddRefed<DrawTarget> DrawTargetRecording::CreateSimilarDrawTarget(
    const IntSize& aSize, SurfaceFormat aFormat) const {
  RefPtr<DrawTarget> similarDT;
  if (mFinalDT->CanCreateSimilarDrawTarget(aSize, aFormat)) {
    similarDT = new DrawTargetRecording(this, aSize, aFormat);
    mRecorder->RecordEvent(
        RecordedCreateSimilarDrawTarget(similarDT.get(), aSize, aFormat));
  } else if (XRE_IsContentProcess()) {
    // Crash any content process that calls this function with arguments that
    // would fail to create a similar draw target. We do this to root out bad
    // callers. We don't want to crash any important processes though so for
    // for those we'll just gracefully return nullptr.
    MOZ_CRASH(
        "Content-process DrawTargetRecording can't create requested similar "
        "drawtarget");
  }
>>>>>>> upstream-releases
  return similarDT.forget();
}

<<<<<<< HEAD
bool DrawTargetRecording::CanCreateSimilarDrawTarget(
    const IntSize &aSize, SurfaceFormat aFormat) const {
  return mFinalDT->CanCreateSimilarDrawTarget(aSize, aFormat);
}

RefPtr<DrawTarget> DrawTargetRecording::CreateClippedDrawTarget(
    const IntSize &aMaxSize, const Matrix &aTransform,
    SurfaceFormat aFormat) const {
  RefPtr<DrawTarget> similarDT;
  if (mFinalDT->CanCreateSimilarDrawTarget(aMaxSize, aFormat)) {
    similarDT = new DrawTargetRecording(this, aMaxSize, aFormat);
    mRecorder->RecordEvent(RecordedCreateClippedDrawTarget(
        similarDT.get(), aMaxSize, aTransform, aFormat));
  } else if (XRE_IsContentProcess()) {
    // See CreateSimilarDrawTarget
    MOZ_CRASH(
        "Content-process DrawTargetRecording can't create requested clipped "
        "drawtarget");
  }
||||||| merged common ancestors
RefPtr<DrawTarget>
DrawTargetRecording::CreateClippedDrawTarget(const IntSize& aMaxSize, const Matrix& aTransform, SurfaceFormat aFormat) const
{
  RefPtr<DrawTarget> similarDT = new DrawTargetRecording(this, aMaxSize, aFormat);
  mRecorder->RecordEvent(RecordedCreateClippedDrawTarget(similarDT.get(), aMaxSize, aTransform, aFormat));
=======
bool DrawTargetRecording::CanCreateSimilarDrawTarget(
    const IntSize& aSize, SurfaceFormat aFormat) const {
  return mFinalDT->CanCreateSimilarDrawTarget(aSize, aFormat);
}

RefPtr<DrawTarget> DrawTargetRecording::CreateClippedDrawTarget(
    const Rect& aBounds, SurfaceFormat aFormat) {
  RefPtr<DrawTarget> similarDT;
  similarDT = new DrawTargetRecording(this, mSize, aFormat);
  mRecorder->RecordEvent(
      RecordedCreateClippedDrawTarget(similarDT.get(), aBounds, aFormat));
  similarDT->SetTransform(mTransform);
>>>>>>> upstream-releases
  return similarDT;
}

<<<<<<< HEAD
already_AddRefed<PathBuilder> DrawTargetRecording::CreatePathBuilder(
    FillRule aFillRule) const {
||||||| merged common ancestors
already_AddRefed<PathBuilder>
DrawTargetRecording::CreatePathBuilder(FillRule aFillRule) const
{
=======
already_AddRefed<DrawTarget>
DrawTargetRecording::CreateSimilarDrawTargetForFilter(
    const IntSize& aMaxSize, SurfaceFormat aFormat, FilterNode* aFilter,
    FilterNode* aSource, const Rect& aSourceRect, const Point& aDestPoint) {
  RefPtr<DrawTarget> similarDT;
  if (mFinalDT->CanCreateSimilarDrawTarget(aMaxSize, aFormat)) {
    similarDT = new DrawTargetRecording(this, aMaxSize, aFormat);
    mRecorder->RecordEvent(RecordedCreateDrawTargetForFilter(
        this, similarDT.get(), aMaxSize, aFormat, aFilter, aSource, aSourceRect,
        aDestPoint));
  } else if (XRE_IsContentProcess()) {
    // See CreateSimilarDrawTarget
    MOZ_CRASH(
        "Content-process DrawTargetRecording can't create requested clipped "
        "drawtarget");
  }
  return similarDT.forget();
}

already_AddRefed<PathBuilder> DrawTargetRecording::CreatePathBuilder(
    FillRule aFillRule) const {
>>>>>>> upstream-releases
  RefPtr<PathBuilder> builder = mFinalDT->CreatePathBuilder(aFillRule);
  return MakeAndAddRef<PathBuilderRecording>(builder, aFillRule);
}

<<<<<<< HEAD
already_AddRefed<GradientStops> DrawTargetRecording::CreateGradientStops(
    GradientStop *aStops, uint32_t aNumStops, ExtendMode aExtendMode) const {
||||||| merged common ancestors
already_AddRefed<GradientStops>
DrawTargetRecording::CreateGradientStops(GradientStop *aStops,
                                         uint32_t aNumStops,
                                         ExtendMode aExtendMode) const
{
=======
already_AddRefed<GradientStops> DrawTargetRecording::CreateGradientStops(
    GradientStop* aStops, uint32_t aNumStops, ExtendMode aExtendMode) const {
>>>>>>> upstream-releases
  RefPtr<GradientStops> retStops = new GradientStopsRecording(mRecorder);

  mRecorder->RecordEvent(
      RecordedGradientStopsCreation(retStops, aStops, aNumStops, aExtendMode));

  return retStops.forget();
}

<<<<<<< HEAD
void DrawTargetRecording::SetTransform(const Matrix &aTransform) {
||||||| merged common ancestors
void
DrawTargetRecording::SetTransform(const Matrix &aTransform)
{
=======
void DrawTargetRecording::SetTransform(const Matrix& aTransform) {
>>>>>>> upstream-releases
  mRecorder->RecordEvent(RecordedSetTransform(this, aTransform));
  DrawTarget::SetTransform(aTransform);
}

<<<<<<< HEAD
already_AddRefed<PathRecording> DrawTargetRecording::EnsurePathStored(
    const Path *aPath) {
||||||| merged common ancestors
already_AddRefed<PathRecording>
DrawTargetRecording::EnsurePathStored(const Path *aPath)
{
=======
already_AddRefed<PathRecording> DrawTargetRecording::EnsurePathStored(
    const Path* aPath) {
>>>>>>> upstream-releases
  RefPtr<PathRecording> pathRecording;
  if (aPath->GetBackendType() == BackendType::RECORDING) {
<<<<<<< HEAD
    pathRecording =
        const_cast<PathRecording *>(static_cast<const PathRecording *>(aPath));
||||||| merged common ancestors
    pathRecording = const_cast<PathRecording*>(static_cast<const PathRecording*>(aPath));
=======
    pathRecording =
        const_cast<PathRecording*>(static_cast<const PathRecording*>(aPath));
>>>>>>> upstream-releases
    if (mRecorder->HasStoredObject(aPath)) {
      return pathRecording.forget();
    }
  } else {
    MOZ_ASSERT(!mRecorder->HasStoredObject(aPath));
    FillRule fillRule = aPath->GetFillRule();
    RefPtr<PathBuilder> builder = mFinalDT->CreatePathBuilder(fillRule);
    RefPtr<PathBuilderRecording> builderRecording =
        new PathBuilderRecording(builder, fillRule);
    aPath->StreamToSink(builderRecording);
    pathRecording = builderRecording->Finish().downcast<PathRecording>();
  }

  mRecorder->RecordEvent(RecordedPathCreation(pathRecording.get()));
  mRecorder->AddStoredObject(pathRecording);
  pathRecording->mStoredRecorders.push_back(mRecorder);

  return pathRecording.forget();
}

// This should only be called on the 'root' DrawTargetRecording.
// Calling it on a child DrawTargetRecordings will cause confusion.
<<<<<<< HEAD
void DrawTargetRecording::FlushItem(const IntRect &aBounds) {
||||||| merged common ancestors
void
DrawTargetRecording::FlushItem(const IntRect &aBounds)
{
=======
void DrawTargetRecording::FlushItem(const IntRect& aBounds) {
>>>>>>> upstream-releases
  mRecorder->FlushItem(aBounds);
  // Reinitialize the recorder (FlushItem will write a new recording header)
  // Tell the new recording about our draw target
  // This code should match what happens in the DrawTargetRecording constructor.
  mRecorder->RecordEvent(
      RecordedDrawTargetCreation(this, mFinalDT->GetBackendType(), mSize,
                                 mFinalDT->GetFormat(), false, nullptr));
  // Add the current transform to the new recording
  mRecorder->RecordEvent(
      RecordedSetTransform(this, DrawTarget::GetTransform()));
}

<<<<<<< HEAD
void DrawTargetRecording::EnsurePatternDependenciesStored(
    const Pattern &aPattern) {
||||||| merged common ancestors
void
DrawTargetRecording::EnsurePatternDependenciesStored(const Pattern &aPattern)
{
=======
void DrawTargetRecording::EnsurePatternDependenciesStored(
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
      EnsureSurfaceStoredRecording(mRecorder, pat->mSurface,
                                   "EnsurePatternDependenciesStored");
||||||| merged common ancestors
  case PatternType::SURFACE:
    {
      const SurfacePattern *pat = static_cast<const SurfacePattern*>(&aPattern);
      EnsureSurfaceStoredRecording(mRecorder, pat->mSurface, "EnsurePatternDependenciesStored");
=======
    case PatternType::SURFACE: {
      const SurfacePattern* pat = static_cast<const SurfacePattern*>(&aPattern);
      EnsureSurfaceStoredRecording(mRecorder, pat->mSurface,
                                   "EnsurePatternDependenciesStored");
>>>>>>> upstream-releases
      return;
    }
  }
}

}  // namespace gfx
}  // namespace mozilla
