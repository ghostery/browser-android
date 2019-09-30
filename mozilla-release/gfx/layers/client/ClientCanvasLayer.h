/* -*- Mode: C++; tab-width: 8; indent-tabs-mode: nil; c-basic-offset: 2 -*- */
/* vim: set ts=8 sts=2 et sw=2 tw=80: */
/* This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/. */

#ifndef GFX_CLIENTCANVASLAYER_H
#define GFX_CLIENTCANVASLAYER_H

#include "ClientCanvasRenderer.h"
#include "ClientLayerManager.h"             // for ClientLayerManager, etc
#include "Layers.h"                         // for CanvasLayer, etc
#include "mozilla/Attributes.h"             // for override
#include "mozilla/layers/CanvasClient.h"    // for CanvasClient, etc
#include "mozilla/layers/LayersMessages.h"  // for CanvasLayerAttributes, etc
#include "mozilla/mozalloc.h"               // for operator delete
#include "nsDebug.h"                        // for NS_ASSERTION
#include "nsISupportsImpl.h"                // for MOZ_COUNT_CTOR, etc

namespace mozilla {
namespace layers {

class CompositableClient;
class ShadowableLayer;

class ClientCanvasLayer : public CanvasLayer, public ClientLayer {
 public:
  explicit ClientCanvasLayer(ClientLayerManager* aLayerManager)
      : CanvasLayer(aLayerManager, static_cast<ClientLayer*>(this)) {
    MOZ_COUNT_CTOR(ClientCanvasLayer);
  }

  CanvasRenderer* CreateCanvasRendererInternal() override;

 protected:
  virtual ~ClientCanvasLayer();

<<<<<<< HEAD
 public:
  virtual void SetVisibleRegion(const LayerIntRegion& aRegion) override {
||||||| merged common ancestors
public:
  virtual void SetVisibleRegion(const LayerIntRegion& aRegion) override
  {
=======
 public:
  void SetVisibleRegion(const LayerIntRegion& aRegion) override {
>>>>>>> upstream-releases
    NS_ASSERTION(ClientManager()->InConstruction(),
                 "Can only set properties in construction phase");
    CanvasLayer::SetVisibleRegion(aRegion);
  }

  void RenderLayer() override;

<<<<<<< HEAD
  virtual void ClearCachedResources() override {
||||||| merged common ancestors
  virtual void ClearCachedResources() override
  {
=======
  void ClearCachedResources() override {
>>>>>>> upstream-releases
    mCanvasRenderer->ClearCachedResources();
  }

<<<<<<< HEAD
  virtual void HandleMemoryPressure() override {
||||||| merged common ancestors
  virtual void HandleMemoryPressure() override
  {
=======
  void HandleMemoryPressure() override {
>>>>>>> upstream-releases
    mCanvasRenderer->ClearCachedResources();
  }

<<<<<<< HEAD
  virtual void FillSpecificAttributes(
      SpecificLayerAttributes& aAttrs) override {
||||||| merged common ancestors
  virtual void FillSpecificAttributes(SpecificLayerAttributes& aAttrs) override
  {
=======
  void FillSpecificAttributes(SpecificLayerAttributes& aAttrs) override {
>>>>>>> upstream-releases
    aAttrs = CanvasLayerAttributes(mSamplingFilter, mBounds);
  }

<<<<<<< HEAD
  virtual Layer* AsLayer() override { return this; }
  virtual ShadowableLayer* AsShadowableLayer() override { return this; }
||||||| merged common ancestors
  virtual Layer* AsLayer()  override { return this; }
  virtual ShadowableLayer* AsShadowableLayer()  override { return this; }
=======
  Layer* AsLayer() override { return this; }
  ShadowableLayer* AsShadowableLayer() override { return this; }
>>>>>>> upstream-releases

<<<<<<< HEAD
  virtual void Disconnect() override { mCanvasRenderer->Destroy(); }
||||||| merged common ancestors
  virtual void Disconnect() override
  {
    mCanvasRenderer->Destroy();
  }
=======
  void Disconnect() override { mCanvasRenderer->Destroy(); }
>>>>>>> upstream-releases

<<<<<<< HEAD
  virtual CompositableClient* GetCompositableClient() override {
    ClientCanvasRenderer* canvasRenderer =
        mCanvasRenderer->AsClientCanvasRenderer();
||||||| merged common ancestors
  virtual CompositableClient* GetCompositableClient() override
  {
    ClientCanvasRenderer* canvasRenderer = mCanvasRenderer->AsClientCanvasRenderer();
=======
  CompositableClient* GetCompositableClient() override {
    ClientCanvasRenderer* canvasRenderer =
        mCanvasRenderer->AsClientCanvasRenderer();
>>>>>>> upstream-releases
    MOZ_ASSERT(canvasRenderer);
    return canvasRenderer->GetCanvasClient();
  }

 protected:
  ClientLayerManager* ClientManager() {
    return static_cast<ClientLayerManager*>(mManager);
  }
};

}  // namespace layers
}  // namespace mozilla

#endif
