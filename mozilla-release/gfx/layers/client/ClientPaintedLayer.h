/* -*- Mode: C++; tab-width: 8; indent-tabs-mode: nil; c-basic-offset: 2 -*- */
/* vim: set ts=8 sts=2 et sw=2 tw=80: */
/* This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/. */

#ifndef GFX_CLIENTPAINTEDLAYER_H
#define GFX_CLIENTPAINTEDLAYER_H

#include "ClientLayerManager.h"                // for ClientLayerManager, etc
#include "Layers.h"                            // for PaintedLayer, etc
#include "RotatedBuffer.h"                     // for RotatedBuffer, etc
#include "mozilla/Attributes.h"                // for override
#include "mozilla/RefPtr.h"                    // for RefPtr
#include "mozilla/layers/ContentClient.h"      // for ContentClient
#include "mozilla/mozalloc.h"                  // for operator delete
#include "nsDebug.h"                           // for NS_ASSERTION
#include "nsISupportsImpl.h"                   // for MOZ_COUNT_CTOR, etc
#include "nsRegion.h"                          // for nsIntRegion
#include "mozilla/layers/PLayerTransaction.h"  // for PaintedLayerAttributes

namespace mozilla {
namespace gfx {
class DrawEventRecorderMemory;
class DrawTargetCapture;
};  // namespace gfx

namespace layers {
class CompositableClient;
class ShadowableLayer;
class SpecificLayerAttributes;

class ClientPaintedLayer : public PaintedLayer, public ClientLayer {
 public:
  typedef ContentClient::PaintState PaintState;
  typedef ContentClient::ContentType ContentType;

  explicit ClientPaintedLayer(
      ClientLayerManager* aLayerManager,
      LayerManager::PaintedLayerCreationHint aCreationHint = LayerManager::NONE)
      : PaintedLayer(aLayerManager, static_cast<ClientLayer*>(this),
                     aCreationHint),
        mContentClient(nullptr) {
    MOZ_COUNT_CTOR(ClientPaintedLayer);
  }

 protected:
  virtual ~ClientPaintedLayer() {
    if (mContentClient) {
      mContentClient->OnDetach();
      mContentClient = nullptr;
    }
    MOZ_COUNT_DTOR(ClientPaintedLayer);
  }

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
    PaintedLayer::SetVisibleRegion(aRegion);
  }
<<<<<<< HEAD
  virtual void InvalidateRegion(const nsIntRegion& aRegion) override {
||||||| merged common ancestors
  virtual void InvalidateRegion(const nsIntRegion& aRegion) override
  {
=======
  void InvalidateRegion(const nsIntRegion& aRegion) override {
>>>>>>> upstream-releases
    NS_ASSERTION(ClientManager()->InConstruction(),
                 "Can only set properties in construction phase");
    mInvalidRegion.Add(aRegion);
    UpdateValidRegionAfterInvalidRegionChanged();
  }

  void RenderLayer() override { RenderLayerWithReadback(nullptr); }

<<<<<<< HEAD
  virtual void RenderLayerWithReadback(ReadbackProcessor* aReadback) override;
||||||| merged common ancestors
  virtual void RenderLayerWithReadback(ReadbackProcessor *aReadback) override;
=======
  void RenderLayerWithReadback(ReadbackProcessor* aReadback) override;
>>>>>>> upstream-releases

<<<<<<< HEAD
  virtual void ClearCachedResources() override {
||||||| merged common ancestors
  virtual void ClearCachedResources() override
  {
=======
  void ClearCachedResources() override {
>>>>>>> upstream-releases
    if (mContentClient) {
      mContentClient->Clear();
    }
    ClearValidRegion();
    DestroyBackBuffer();
  }

<<<<<<< HEAD
  virtual void HandleMemoryPressure() override {
||||||| merged common ancestors
  virtual void HandleMemoryPressure() override
  {
=======
  void HandleMemoryPressure() override {
>>>>>>> upstream-releases
    if (mContentClient) {
      mContentClient->HandleMemoryPressure();
    }
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
    aAttrs = PaintedLayerAttributes(GetValidRegion());
  }

  ClientLayerManager* ClientManager() {
    return static_cast<ClientLayerManager*>(mManager);
  }
<<<<<<< HEAD

  virtual Layer* AsLayer() override { return this; }
  virtual ShadowableLayer* AsShadowableLayer() override { return this; }
||||||| merged common ancestors
  
  virtual Layer* AsLayer() override { return this; }
  virtual ShadowableLayer* AsShadowableLayer() override { return this; }
=======
>>>>>>> upstream-releases

<<<<<<< HEAD
  virtual CompositableClient* GetCompositableClient() override {
||||||| merged common ancestors
  virtual CompositableClient* GetCompositableClient() override
  {
=======
  Layer* AsLayer() override { return this; }
  ShadowableLayer* AsShadowableLayer() override { return this; }

  CompositableClient* GetCompositableClient() override {
>>>>>>> upstream-releases
    return mContentClient;
  }

<<<<<<< HEAD
  virtual void Disconnect() override { mContentClient = nullptr; }
||||||| merged common ancestors
  virtual void Disconnect() override
  {
    mContentClient = nullptr;
  }
=======
  void Disconnect() override { mContentClient = nullptr; }
>>>>>>> upstream-releases

 protected:
  void RecordThebes();
  bool HasMaskLayers();
  bool EnsureContentClient();
  uint32_t GetPaintFlags(ReadbackProcessor* aReadback);
  void UpdateContentClient(PaintState& aState);
  bool UpdatePaintRegion(PaintState& aState);
  void FinishPaintState(PaintState& aState);

<<<<<<< HEAD
  virtual void PrintInfo(std::stringstream& aStream,
                         const char* aPrefix) override;
||||||| merged common ancestors
  virtual void PrintInfo(std::stringstream& aStream, const char* aPrefix) override;
=======
  void PrintInfo(std::stringstream& aStream, const char* aPrefix) override;
>>>>>>> upstream-releases

  void DestroyBackBuffer() { mContentClient = nullptr; }

  RefPtr<ContentClient> mContentClient;
};

}  // namespace layers
}  // namespace mozilla

#endif
