/* -*- Mode: C++; tab-width: 8; indent-tabs-mode: nil; c-basic-offset: 2 -*- */
/* vim: set ts=8 sts=2 et sw=2 tw=80: */
/* This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/. */

#ifndef GFX_WEBRENDERLAYERMANAGER_H
#define GFX_WEBRENDERLAYERMANAGER_H

#include <unordered_set>
#include <unordered_map>
#include <vector>

#include "Layers.h"
#include "mozilla/Maybe.h"
#include "mozilla/MozPromise.h"
#include "mozilla/StaticPrefs.h"
#include "mozilla/layers/APZTestData.h"
#include "mozilla/layers/FocusTarget.h"
<<<<<<< HEAD
#include "mozilla/layers/IpcResourceUpdateQueue.h"
#include "mozilla/layers/SharedSurfacesChild.h"
||||||| merged common ancestors
=======
#include "mozilla/layers/IpcResourceUpdateQueue.h"
#include "mozilla/layers/RenderRootStateManager.h"
#include "mozilla/layers/SharedSurfacesChild.h"
>>>>>>> upstream-releases
#include "mozilla/layers/StackingContextHelper.h"
#include "mozilla/layers/TransactionIdAllocator.h"
#include "mozilla/layers/WebRenderCommandBuilder.h"
#include "mozilla/layers/WebRenderScrollData.h"
#include "mozilla/layers/WebRenderUserData.h"
#include "mozilla/webrender/WebRenderAPI.h"
#include "mozilla/webrender/WebRenderTypes.h"
#include "nsDisplayList.h"

class nsIWidget;

namespace mozilla {

struct ActiveScrolledRoot;

namespace dom {
class TabGroup;
}

namespace layers {

class CompositorBridgeChild;
class KnowsCompositor;
class PCompositorBridgeChild;
class WebRenderBridgeChild;
class WebRenderParentCommand;

class WebRenderLayerManager final : public LayerManager {
  typedef nsTArray<RefPtr<Layer>> LayerRefArray;
  typedef nsTHashtable<nsRefPtrHashKey<WebRenderUserData>>
      WebRenderUserDataRefTable;

 public:
  explicit WebRenderLayerManager(nsIWidget* aWidget);
  bool Initialize(PCompositorBridgeChild* aCBChild, wr::PipelineId aLayersId,
                  TextureFactoryIdentifier* aTextureFactoryIdentifier);

  void Destroy() override;

  void DoDestroy(bool aIsSync);

 protected:
  virtual ~WebRenderLayerManager();

<<<<<<< HEAD
 public:
  virtual KnowsCompositor* AsKnowsCompositor() override;
||||||| merged common ancestors
public:
  virtual KnowsCompositor* AsKnowsCompositor() override;
=======
 public:
  KnowsCompositor* AsKnowsCompositor() override;
>>>>>>> upstream-releases
  WebRenderLayerManager* AsWebRenderLayerManager() override { return this; }
  CompositorBridgeChild* GetCompositorBridgeChild() override;

  // WebRender can handle images larger than the max texture size via tiling.
<<<<<<< HEAD
  virtual int32_t GetMaxTextureSize() const override { return INT32_MAX; }

  virtual bool BeginTransactionWithTarget(gfxContext* aTarget,
                                          const nsCString& aURL) override;
  virtual bool BeginTransaction(const nsCString& aURL) override;
  virtual bool EndEmptyTransaction(
      EndTransactionFlags aFlags = END_DEFAULT) override;
  void EndTransactionWithoutLayer(
      nsDisplayList* aDisplayList, nsDisplayListBuilder* aDisplayListBuilder,
      const nsTArray<wr::WrFilterOp>& aFilters = nsTArray<wr::WrFilterOp>(),
      WebRenderBackgroundData* aBackground = nullptr);
  virtual void EndTransaction(
      DrawPaintedLayerCallback aCallback, void* aCallbackData,
      EndTransactionFlags aFlags = END_DEFAULT) override;

  virtual LayersBackend GetBackendType() override {
    return LayersBackend::LAYERS_WR;
  }
  virtual void GetBackendName(nsAString& name) override {
    name.AssignLiteral("WebRender");
  }
  virtual const char* Name() const override { return "WebRender"; }

  virtual void SetRoot(Layer* aLayer) override;

  already_AddRefed<PaintedLayer> CreatePaintedLayer() override {
    return nullptr;
  }
  already_AddRefed<ContainerLayer> CreateContainerLayer() override {
    return nullptr;
  }
||||||| merged common ancestors
  virtual int32_t GetMaxTextureSize() const override { return INT32_MAX; }

  virtual bool BeginTransactionWithTarget(gfxContext* aTarget) override;
  virtual bool BeginTransaction() override;
  virtual bool EndEmptyTransaction(EndTransactionFlags aFlags = END_DEFAULT) override;
  void EndTransactionWithoutLayer(nsDisplayList* aDisplayList,
                                  nsDisplayListBuilder* aDisplayListBuilder,
                                  const nsTArray<wr::WrFilterOp>& aFilters = nsTArray<wr::WrFilterOp>(),
                                  WebRenderBackgroundData* aBackground = nullptr);
  virtual void EndTransaction(DrawPaintedLayerCallback aCallback,
                              void* aCallbackData,
                              EndTransactionFlags aFlags = END_DEFAULT) override;

  virtual LayersBackend GetBackendType() override { return LayersBackend::LAYERS_WR; }
  virtual void GetBackendName(nsAString& name) override { name.AssignLiteral("WebRender"); }
  virtual const char* Name() const override { return "WebRender"; }

  virtual void SetRoot(Layer* aLayer) override;

  already_AddRefed<PaintedLayer> CreatePaintedLayer() override { return nullptr; }
  already_AddRefed<ContainerLayer> CreateContainerLayer() override { return nullptr; }
=======
  int32_t GetMaxTextureSize() const override { return INT32_MAX; }

  bool BeginTransactionWithTarget(gfxContext* aTarget,
                                  const nsCString& aURL) override;
  bool BeginTransaction(const nsCString& aURL) override;
  bool EndEmptyTransaction(EndTransactionFlags aFlags = END_DEFAULT) override;
  void EndTransactionWithoutLayer(
      nsDisplayList* aDisplayList, nsDisplayListBuilder* aDisplayListBuilder,
      WrFiltersHolder&& aFilters = WrFiltersHolder(),
      WebRenderBackgroundData* aBackground = nullptr);
  void EndTransaction(DrawPaintedLayerCallback aCallback, void* aCallbackData,
                      EndTransactionFlags aFlags = END_DEFAULT) override;

  LayersBackend GetBackendType() override { return LayersBackend::LAYERS_WR; }
  void GetBackendName(nsAString& name) override {
    name.AssignLiteral("WebRender");
  }
  const char* Name() const override { return "WebRender"; }

  void SetRoot(Layer* aLayer) override;

  already_AddRefed<PaintedLayer> CreatePaintedLayer() override {
    return nullptr;
  }
  already_AddRefed<ContainerLayer> CreateContainerLayer() override {
    return nullptr;
  }
>>>>>>> upstream-releases
  already_AddRefed<ImageLayer> CreateImageLayer() override { return nullptr; }
  already_AddRefed<ColorLayer> CreateColorLayer() override { return nullptr; }
  already_AddRefed<CanvasLayer> CreateCanvasLayer() override { return nullptr; }

  bool NeedsWidgetInvalidation() override { return false; }

  void SetLayersObserverEpoch(LayersObserverEpoch aEpoch) override;

  void DidComposite(TransactionId aTransactionId,
                    const mozilla::TimeStamp& aCompositeStart,
                    const mozilla::TimeStamp& aCompositeEnd) override;

<<<<<<< HEAD
  virtual void ClearCachedResources(Layer* aSubtree = nullptr) override;
  virtual void UpdateTextureFactoryIdentifier(
      const TextureFactoryIdentifier& aNewIdentifier) override;
  virtual TextureFactoryIdentifier GetTextureFactoryIdentifier() override;
||||||| merged common ancestors
  virtual void ClearCachedResources(Layer* aSubtree = nullptr) override;
  virtual void UpdateTextureFactoryIdentifier(const TextureFactoryIdentifier& aNewIdentifier) override;
  virtual TextureFactoryIdentifier GetTextureFactoryIdentifier() override;
=======
  void ClearCachedResources(Layer* aSubtree = nullptr) override;
  void UpdateTextureFactoryIdentifier(
      const TextureFactoryIdentifier& aNewIdentifier) override;
  TextureFactoryIdentifier GetTextureFactoryIdentifier() override;
>>>>>>> upstream-releases

<<<<<<< HEAD
  virtual void SetTransactionIdAllocator(
      TransactionIdAllocator* aAllocator) override;
  virtual TransactionId GetLastTransactionId() override;
||||||| merged common ancestors
  virtual void SetTransactionIdAllocator(TransactionIdAllocator* aAllocator) override;
  virtual TransactionId GetLastTransactionId() override;
=======
  void SetTransactionIdAllocator(TransactionIdAllocator* aAllocator) override;
  TransactionId GetLastTransactionId() override;
>>>>>>> upstream-releases

<<<<<<< HEAD
  virtual void AddDidCompositeObserver(
      DidCompositeObserver* aObserver) override;
  virtual void RemoveDidCompositeObserver(
      DidCompositeObserver* aObserver) override;
||||||| merged common ancestors
  virtual void AddDidCompositeObserver(DidCompositeObserver* aObserver) override;
  virtual void RemoveDidCompositeObserver(DidCompositeObserver* aObserver) override;
=======
  void AddDidCompositeObserver(DidCompositeObserver* aObserver) override;
  void RemoveDidCompositeObserver(DidCompositeObserver* aObserver) override;
>>>>>>> upstream-releases

  void FlushRendering() override;
  void WaitOnTransactionProcessed() override;

  void SendInvalidRegion(const nsIntRegion& aRegion) override;

  void ScheduleComposite() override;

<<<<<<< HEAD
  virtual void SetNeedsComposite(bool aNeedsComposite) override {
||||||| merged common ancestors
  virtual void SetNeedsComposite(bool aNeedsComposite) override
  {
=======
  void SetNeedsComposite(bool aNeedsComposite) override {
>>>>>>> upstream-releases
    mNeedsComposite = aNeedsComposite;
  }
  bool NeedsComposite() const override { return mNeedsComposite; }
  void SetIsFirstPaint() override { mIsFirstPaint = true; }
  bool GetIsFirstPaint() const override { return mIsFirstPaint; }
  void SetFocusTarget(const FocusTarget& aFocusTarget) override;

<<<<<<< HEAD
  virtual already_AddRefed<PersistentBufferProvider>
  CreatePersistentBufferProvider(const gfx::IntSize& aSize,
                                 gfx::SurfaceFormat aFormat) override;
||||||| merged common ancestors
  virtual already_AddRefed<PersistentBufferProvider>
  CreatePersistentBufferProvider(const gfx::IntSize& aSize, gfx::SurfaceFormat aFormat) override;
=======
  already_AddRefed<PersistentBufferProvider> CreatePersistentBufferProvider(
      const gfx::IntSize& aSize, gfx::SurfaceFormat aFormat) override;
>>>>>>> upstream-releases

  bool AsyncPanZoomEnabled() const override;

  // adds an imagekey to a list of keys that will be discarded on the next
  // transaction or destruction
<<<<<<< HEAD
  void AddImageKeyForDiscard(wr::ImageKey);
  void AddBlobImageKeyForDiscard(wr::BlobImageKey);
||||||| merged common ancestors
  void AddImageKeyForDiscard(wr::ImageKey);
=======
>>>>>>> upstream-releases
  void DiscardImages();
  void DiscardImagesInTransaction(wr::IpcResourceUpdateQueue& aResourceUpdates);
  void DiscardLocalImages();

<<<<<<< HEAD
  wr::IpcResourceUpdateQueue& AsyncResourceUpdates();
  void FlushAsyncResourceUpdates();

  void RegisterAsyncAnimation(const wr::ImageKey& aKey,
                              SharedSurfacesAnimation* aAnimation);
  void DeregisterAsyncAnimation(const wr::ImageKey& aKey);
  void ClearAsyncAnimations();
  void WrReleasedImages(const nsTArray<wr::ExternalImageKeyPair>& aPairs);

  // Methods to manage the compositor animation ids. Active animations are still
  // going, and when they end we discard them and remove them from the active
  // list.
  void AddActiveCompositorAnimationId(uint64_t aId);
  void AddCompositorAnimationsIdForDiscard(uint64_t aId);
  void DiscardCompositorAnimations();
||||||| merged common ancestors
  // Methods to manage the compositor animation ids. Active animations are still
  // going, and when they end we discard them and remove them from the active
  // list.
  void AddActiveCompositorAnimationId(uint64_t aId);
  void AddCompositorAnimationsIdForDiscard(uint64_t aId);
  void DiscardCompositorAnimations();
=======
  void ClearAsyncAnimations();
  void WrReleasedImages(const nsTArray<wr::ExternalImageKeyPair>& aPairs);
>>>>>>> upstream-releases

  WebRenderBridgeChild* WrBridge() const { return mWrChild; }

  // See equivalent function in ClientLayerManager
  void LogTestDataForCurrentPaint(ScrollableLayerGuid::ViewID aScrollId,
                                  const std::string& aKey,
                                  const std::string& aValue) {
<<<<<<< HEAD
    MOZ_ASSERT(gfxPrefs::APZTestLoggingEnabled(), "don't call me");
    mApzTestData.LogTestDataForPaint(mPaintSequenceNumber, aScrollId, aKey,
                                     aValue);
||||||| merged common ancestors
    MOZ_ASSERT(gfxPrefs::APZTestLoggingEnabled(), "don't call me");
    mApzTestData.LogTestDataForPaint(mPaintSequenceNumber, aScrollId, aKey, aValue);
=======
    MOZ_ASSERT(StaticPrefs::apz_test_logging_enabled(), "don't call me");
    mApzTestData.LogTestDataForPaint(mPaintSequenceNumber, aScrollId, aKey,
                                     aValue);
  }
  void LogAdditionalTestData(const std::string& aKey,
                             const std::string& aValue) {
    MOZ_ASSERT(StaticPrefs::apz_test_logging_enabled(), "don't call me");
    mApzTestData.RecordAdditionalData(aKey, aValue);
>>>>>>> upstream-releases
  }

  // See equivalent function in ClientLayerManager
  const APZTestData& GetAPZTestData() const { return mApzTestData; }

  WebRenderCommandBuilder& CommandBuilder() { return mWebRenderCommandBuilder; }
<<<<<<< HEAD
  WebRenderUserDataRefTable* GetWebRenderUserDataTable() {
    return mWebRenderCommandBuilder.GetWebRenderUserDataTable();
  }
  WebRenderScrollData& GetScrollData() { return mScrollData; }
||||||| merged common ancestors
  WebRenderUserDataRefTable* GetWebRenderUserDataTable() { return mWebRenderCommandBuilder.GetWebRenderUserDataTable(); }
  WebRenderScrollData& GetScrollData() { return mScrollData; }
=======
  WebRenderUserDataRefTable* GetWebRenderUserDataTable() {
    return mWebRenderCommandBuilder.GetWebRenderUserDataTable();
  }
  WebRenderScrollData& GetScrollData(wr::RenderRoot aRenderRoot) {
    return mScrollDatas[aRenderRoot];
  }
>>>>>>> upstream-releases

  void WrUpdated();
  void WindowOverlayChanged() { mWindowOverlayChanged = true; }
  nsIWidget* GetWidget() { return mWidget; }

  dom::TabGroup* GetTabGroup();

  uint32_t StartFrameTimeRecording(int32_t aBufferSize) override;
  void StopFrameTimeRecording(uint32_t aStartIndex,
                              nsTArray<float>& aFrameIntervals) override;

<<<<<<< HEAD
 private:
||||||| merged common ancestors
private:
=======
  RenderRootStateManager* GetRenderRootStateManager(
      wr::RenderRoot aRenderRoot) {
    return &mStateManagers[aRenderRoot];
  }

  virtual void PayloadPresented() override;

  void TakeCompositionPayloads(nsTArray<CompositionPayload>& aPayloads);

 private:
>>>>>>> upstream-releases
  /**
   * Take a snapshot of the parent context, and copy
   * it into mTarget.
   */
  void MakeSnapshotIfRequired(LayoutDeviceIntSize aSize);

 private:
  nsIWidget* MOZ_NON_OWNING_REF mWidget;
<<<<<<< HEAD
  nsTArray<wr::ImageKey> mImageKeysToDelete;
  nsTArray<wr::BlobImageKey> mBlobImageKeysToDelete;

  // Set of compositor animation ids for which there are active animations (as
  // of the last transaction) on the compositor side.
  std::unordered_set<uint64_t> mActiveCompositorAnimationIds;
  // Compositor animation ids for animations that are done now and that we want
  // the compositor to discard information for.
  nsTArray<uint64_t> mDiscardedCompositorAnimationsIds;
||||||| merged common ancestors
  nsTArray<wr::ImageKey> mImageKeysToDelete;

  // Set of compositor animation ids for which there are active animations (as
  // of the last transaction) on the compositor side.
  std::unordered_set<uint64_t> mActiveCompositorAnimationIds;
  // Compositor animation ids for animations that are done now and that we want
  // the compositor to discard information for.
  nsTArray<uint64_t> mDiscardedCompositorAnimationsIds;
=======
>>>>>>> upstream-releases

  RefPtr<WebRenderBridgeChild> mWrChild;

  RefPtr<TransactionIdAllocator> mTransactionIdAllocator;
  TransactionId mLatestTransactionId;

  nsTArray<DidCompositeObserver*> mDidCompositeObservers;

  // This holds the scroll data that we need to send to the compositor for
  // APZ to do it's job
  wr::RenderRootArray<WebRenderScrollData> mScrollDatas;

  bool mWindowOverlayChanged;
  bool mNeedsComposite;
  bool mIsFirstPaint;
  FocusTarget mFocusTarget;

  // When we're doing a transaction in order to draw to a non-default
  // target, the layers transaction is only performed in order to send
  // a PLayers:Update.  We save the original non-default target to
  // mTarget, and then perform the transaction. After the transaction ends,
  // we send a message to our remote side to capture the actual pixels
  // being drawn to the default target, and then copy those pixels
  // back to mTarget.
  RefPtr<gfxContext> mTarget;

  // See equivalent field in ClientLayerManager
  uint32_t mPaintSequenceNumber;
  // See equivalent field in ClientLayerManager
  APZTestData mApzTestData;

  TimeStamp mTransactionStart;
  nsCString mURL;
  WebRenderCommandBuilder mWebRenderCommandBuilder;

<<<<<<< HEAD
  size_t mLastDisplayListSize;

  Maybe<wr::IpcResourceUpdateQueue> mAsyncResourceUpdates;
  std::unordered_map<uint64_t, RefPtr<SharedSurfacesAnimation>>
      mAsyncAnimations;
||||||| merged common ancestors
  size_t mLastDisplayListSize;
=======
  wr::RenderRootArray<size_t> mLastDisplayListSizes;
  wr::RenderRootArray<RenderRootStateManager> mStateManagers;
>>>>>>> upstream-releases
};

}  // namespace layers
}  // namespace mozilla

#endif /* GFX_WEBRENDERLAYERMANAGER_H */
