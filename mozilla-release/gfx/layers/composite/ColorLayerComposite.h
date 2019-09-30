/* -*- Mode: C++; tab-width: 8; indent-tabs-mode: nil; c-basic-offset: 2 -*- */
/* vim: set ts=8 sts=2 et sw=2 tw=80: */
/* This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/. */

#ifndef GFX_ColorLayerComposite_H
#define GFX_ColorLayerComposite_H

#include "Layers.h"                                // for ColorLayer, etc
#include "mozilla/Attributes.h"                    // for override
#include "mozilla/layers/LayerManagerComposite.h"  // for LayerComposite, etc
#include "nsISupportsImpl.h"                       // for MOZ_COUNT_CTOR, etc

namespace mozilla {
namespace layers {

class CompositableHost;

class ColorLayerComposite : public ColorLayer, public LayerComposite {
 public:
  explicit ColorLayerComposite(LayerManagerComposite* aManager)
      : ColorLayer(aManager, nullptr), LayerComposite(aManager) {
    MOZ_COUNT_CTOR(ColorLayerComposite);
    mImplData = static_cast<LayerComposite*>(this);
  }

<<<<<<< HEAD
 protected:
  ~ColorLayerComposite() {
||||||| merged common ancestors
protected:
  ~ColorLayerComposite()
  {
=======
 protected:
  virtual ~ColorLayerComposite() {
>>>>>>> upstream-releases
    MOZ_COUNT_DTOR(ColorLayerComposite);
    Destroy();
  }

 public:
  // LayerComposite Implementation
  Layer* GetLayer() override { return this; }

<<<<<<< HEAD
  virtual void SetLayerManager(HostLayerManager* aManager) override {
||||||| merged common ancestors
  virtual void SetLayerManager(HostLayerManager* aManager) override
  {
=======
  void SetLayerManager(HostLayerManager* aManager) override {
>>>>>>> upstream-releases
    LayerComposite::SetLayerManager(aManager);
    mManager = aManager;
  }

  void Destroy() override { mDestroyed = true; }

  void RenderLayer(const gfx::IntRect& aClipRect,
                   const Maybe<gfx::Polygon>& aGeometry) override;

<<<<<<< HEAD
  virtual void CleanupResources() override{};
||||||| merged common ancestors
  virtual void CleanupResources() override {};
=======
  void CleanupResources() override{};
>>>>>>> upstream-releases

  void GenEffectChain(EffectChain& aEffect) override;

  CompositableHost* GetCompositableHost() override { return nullptr; }

  HostLayer* AsHostLayer() override { return this; }

  const char* Name() const override { return "ColorLayerComposite"; }
};

}  // namespace layers
}  // namespace mozilla

#endif /* GFX_ColorLayerComposite_H */
