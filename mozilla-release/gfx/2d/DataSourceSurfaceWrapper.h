/* -*- Mode: C++; tab-width: 8; indent-tabs-mode: nil; c-basic-offset: 2 -*- */
/* vim: set ts=8 sts=2 et sw=2 tw=80: */
/* This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/. */

#ifndef MOZILLA_GFX_DATASOURCESURFACEWRAPPER_H_
#define MOZILLA_GFX_DATASOURCESURFACEWRAPPER_H_

#include "2D.h"

namespace mozilla {
namespace gfx {

// Wraps a DataSourceSurface and forwards all methods except for GetType(),
// from which it always returns SurfaceType::DATA.
class DataSourceSurfaceWrapper final : public DataSourceSurface {
 public:
  MOZ_DECLARE_REFCOUNTED_VIRTUAL_TYPENAME(DataSourceSurfaceWrapper, override)
<<<<<<< HEAD
  explicit DataSourceSurfaceWrapper(DataSourceSurface *aSurface)
      : mSurface(aSurface) {}
||||||| merged common ancestors
  explicit DataSourceSurfaceWrapper(DataSourceSurface *aSurface)
   : mSurface(aSurface)
  {}
=======
  explicit DataSourceSurfaceWrapper(DataSourceSurface* aSurface)
      : mSurface(aSurface) {}
>>>>>>> upstream-releases

<<<<<<< HEAD
  bool Equals(SourceSurface *aOther, bool aSymmetric = true) override {
||||||| merged common ancestors
  bool Equals(SourceSurface* aOther, bool aSymmetric = true) override
  {
=======
  bool Equals(SourceSurface* aOther, bool aSymmetric = true) override {
>>>>>>> upstream-releases
    return DataSourceSurface::Equals(aOther, aSymmetric) ||
           mSurface->Equals(aOther, aSymmetric);
  }

  SurfaceType GetType() const override { return SurfaceType::DATA; }

<<<<<<< HEAD
  virtual uint8_t *GetData() override { return mSurface->GetData(); }
  virtual int32_t Stride() override { return mSurface->Stride(); }
  virtual IntSize GetSize() const override { return mSurface->GetSize(); }
  virtual SurfaceFormat GetFormat() const override {
    return mSurface->GetFormat();
  }
  virtual bool IsValid() const override { return mSurface->IsValid(); }
||||||| merged common ancestors
  virtual uint8_t *GetData() override { return mSurface->GetData(); }
  virtual int32_t Stride() override { return mSurface->Stride(); }
  virtual IntSize GetSize() const override { return mSurface->GetSize(); }
  virtual SurfaceFormat GetFormat() const override { return mSurface->GetFormat(); }
  virtual bool IsValid() const override { return mSurface->IsValid(); }
=======
  uint8_t* GetData() override { return mSurface->GetData(); }
  int32_t Stride() override { return mSurface->Stride(); }
  IntSize GetSize() const override { return mSurface->GetSize(); }
  SurfaceFormat GetFormat() const override { return mSurface->GetFormat(); }
  bool IsValid() const override { return mSurface->IsValid(); }
>>>>>>> upstream-releases

<<<<<<< HEAD
  bool Map(MapType aType, MappedSurface *aMappedSurface) override {
||||||| merged common ancestors
  bool Map(MapType aType, MappedSurface *aMappedSurface) override
  {
=======
  bool Map(MapType aType, MappedSurface* aMappedSurface) override {
>>>>>>> upstream-releases
    return mSurface->Map(aType, aMappedSurface);
  }

  void Unmap() override { mSurface->Unmap(); }

 private:
  RefPtr<DataSourceSurface> mSurface;
};

}  // namespace gfx
}  // namespace mozilla

#endif /* MOZILLA_GFX_DATASOURCESURFACEWRAPPER_H_ */
