/* -*- Mode: C++; tab-width: 8; indent-tabs-mode: nil; c-basic-offset: 2 -*- */
/* vim: set ts=8 sts=2 et sw=2 tw=80: */
/* This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/. */

#include "mozilla/layers/ClipManager.h"

#include "DisplayItemClipChain.h"
#include "FrameMetrics.h"
#include "LayersLogging.h"
#include "mozilla/layers/StackingContextHelper.h"
#include "mozilla/layers/WebRenderLayerManager.h"
#include "mozilla/webrender/WebRenderAPI.h"
#include "nsDisplayList.h"
#include "nsStyleStructInlines.h"
#include "UnitTransforms.h"

// clang-format off
#define CLIP_LOG(...)
<<<<<<< HEAD
/*
#define CLIP_LOG(...) printf_stderr("CLIP: " __VA_ARGS__)
#define CLIP_LOG(...) if (XRE_IsContentProcess()) {         \
                        printf_stderr("CLIP: " __VA_ARGS__) \
                      }
*/
||||||| merged common ancestors
//#define CLIP_LOG(...) printf_stderr("CLIP: " __VA_ARGS__)
//#define CLIP_LOG(...) if (XRE_IsContentProcess()) printf_stderr("CLIP: " __VA_ARGS__)
=======
//#define CLIP_LOG(...) printf_stderr("CLIP: " __VA_ARGS__)
//#define CLIP_LOG(...) if (XRE_IsContentProcess()) printf_stderr("CLIP: " __VA_ARGS__)
// clang-format on
>>>>>>> upstream-releases

namespace mozilla {
namespace layers {

ClipManager::ClipManager() : mManager(nullptr), mBuilder(nullptr) {}

void ClipManager::BeginBuild(WebRenderLayerManager* aManager,
                             wr::DisplayListBuilder& aBuilder) {
  MOZ_ASSERT(!mManager);
  mManager = aManager;
  MOZ_ASSERT(!mBuilder);
  mBuilder = &aBuilder;
  MOZ_ASSERT(mCacheStack.empty());
  mCacheStack.emplace();
  MOZ_ASSERT(mASROverride.empty());
  MOZ_ASSERT(mItemClipStack.empty());
}

void ClipManager::EndBuild() {
  mBuilder = nullptr;
  mManager = nullptr;
  mCacheStack.pop();
  MOZ_ASSERT(mCacheStack.empty());
  MOZ_ASSERT(mASROverride.empty());
  MOZ_ASSERT(mItemClipStack.empty());
}

void ClipManager::BeginList(const StackingContextHelper& aStackingContext) {
  if (aStackingContext.AffectsClipPositioning()) {
    if (aStackingContext.ReferenceFrameId()) {
      PushOverrideForASR(
          mItemClipStack.empty() ? nullptr : mItemClipStack.top().mASR,
          aStackingContext.ReferenceFrameId().ref());
    } else {
      // Start a new cache
      mCacheStack.emplace();
    }
  }

  ItemClips clips(nullptr, nullptr, false);
  if (!mItemClipStack.empty()) {
    clips.CopyOutputsFrom(mItemClipStack.top());
  }

  if (aStackingContext.ReferenceFrameId()) {
    clips.mScrollId = aStackingContext.ReferenceFrameId().ref();
  }

  mItemClipStack.push(clips);
}

void ClipManager::EndList(const StackingContextHelper& aStackingContext) {
  MOZ_ASSERT(!mItemClipStack.empty());
  mBuilder->SetClipChainLeaf(Nothing());
  mItemClipStack.pop();

  if (aStackingContext.AffectsClipPositioning()) {
    if (aStackingContext.ReferenceFrameId()) {
      PopOverrideForASR(mItemClipStack.empty() ? nullptr
                                               : mItemClipStack.top().mASR);
    } else {
      MOZ_ASSERT(!mCacheStack.empty());
      mCacheStack.pop();
    }
  }
}

<<<<<<< HEAD
void ClipManager::PushOverrideForASR(const ActiveScrolledRoot* aASR,
                                     const wr::WrClipId& aClipId) {
  Maybe<wr::WrClipId> scrollId = GetScrollLayer(aASR);
  MOZ_ASSERT(scrollId.isSome());
||||||| merged common ancestors
void
ClipManager::PushOverrideForASR(const ActiveScrolledRoot* aASR,
                                const Maybe<wr::WrClipId>& aClipId)
{
  Maybe<wr::WrClipId> scrollId = GetScrollLayer(aASR);
  MOZ_ASSERT(scrollId.isSome());
=======
void ClipManager::PushOverrideForASR(const ActiveScrolledRoot* aASR,
                                     const wr::WrSpatialId& aSpatialId) {
  Maybe<wr::WrSpaceAndClip> spaceAndClip = GetScrollLayer(aASR);
  MOZ_ASSERT(spaceAndClip.isSome());

  CLIP_LOG("Pushing %p override %zu -> %s\n", aASR, spaceAndClip->space.id,
           Stringify(aSpatialId.id).c_str());
>>>>>>> upstream-releases

<<<<<<< HEAD
  CLIP_LOG("Pushing override %zu -> %s\n", scrollId->id,
           Stringify(aClipId.id).c_str());
  auto it = mASROverride.insert({*scrollId, std::stack<wr::WrClipId>()});
  it.first->second.push(aClipId);
||||||| merged common ancestors
  CLIP_LOG("Pushing override %zu -> %s\n", scrollId->id,
      aClipId ? Stringify(aClipId->id).c_str() : "(none)");
  auto it = mASROverride.insert({ *scrollId, std::stack<Maybe<wr::WrClipId>>() });
  it.first->second.push(aClipId);
=======
  auto it =
      mASROverride.insert({spaceAndClip->space, std::stack<wr::WrSpatialId>()});
  it.first->second.push(aSpatialId);
>>>>>>> upstream-releases

  // Start a new cache
  mCacheStack.emplace();
}

void ClipManager::PopOverrideForASR(const ActiveScrolledRoot* aASR) {
  MOZ_ASSERT(!mCacheStack.empty());
  mCacheStack.pop();

  Maybe<wr::WrSpaceAndClip> spaceAndClip = GetScrollLayer(aASR);
  MOZ_ASSERT(spaceAndClip.isSome());

  auto it = mASROverride.find(spaceAndClip->space);
  CLIP_LOG("Popping %p override %zu -> %s\n", aASR, spaceAndClip->space.id,
           Stringify(it->second.top().id).c_str());

<<<<<<< HEAD
  auto it = mASROverride.find(*scrollId);
  MOZ_ASSERT(it != mASROverride.end());
  MOZ_ASSERT(!(it->second.empty()));
  CLIP_LOG("Popping override %zu -> %s\n", scrollId->id,
           Stringify(it->second.top().id).c_str());
||||||| merged common ancestors
  auto it = mASROverride.find(*scrollId);
  MOZ_ASSERT(it != mASROverride.end());
  MOZ_ASSERT(!(it->second.empty()));
  CLIP_LOG("Popping override %zu -> %s\n", scrollId->id,
      it->second.top() ? Stringify(it->second.top()->id).c_str() : "(none)");
=======
>>>>>>> upstream-releases
  it->second.pop();
  if (it->second.empty()) {
    mASROverride.erase(it);
  }
}

<<<<<<< HEAD
Maybe<wr::WrClipId> ClipManager::ClipIdAfterOverride(
    const Maybe<wr::WrClipId>& aClipId) {
  if (!aClipId) {
    return Nothing();
  }
  auto it = mASROverride.find(*aClipId);
||||||| merged common ancestors
Maybe<wr::WrClipId>
ClipManager::ClipIdAfterOverride(const Maybe<wr::WrClipId>& aClipId)
{
  if (!aClipId) {
    return Nothing();
  }
  auto it = mASROverride.find(*aClipId);
=======
wr::WrSpatialId ClipManager::SpatialIdAfterOverride(
    const wr::WrSpatialId& aSpatialId) {
  auto it = mASROverride.find(aSpatialId);
>>>>>>> upstream-releases
  if (it == mASROverride.end()) {
    return aSpatialId;
  }
  MOZ_ASSERT(!it->second.empty());
<<<<<<< HEAD
  CLIP_LOG("Overriding %zu with %s\n", aClipId->id,
           Stringify(it->second.top().id).c_str());
  return Some(it->second.top());
||||||| merged common ancestors
  CLIP_LOG("Overriding %zu with %s\n", aClipId->id,
      it->second.top() ? Stringify(it->second.top()->id).c_str() : "(none)");
  return it->second.top();
=======
  CLIP_LOG("Overriding %zu with %s\n", aSpatialId.id,
           Stringify(it->second.top().id).c_str());

  return it->second.top();
>>>>>>> upstream-releases
}

<<<<<<< HEAD
void ClipManager::BeginItem(nsDisplayItem* aItem,
                            const StackingContextHelper& aStackingContext) {
  CLIP_LOG("processing item %p\n", aItem);

||||||| merged common ancestors
void
ClipManager::BeginItem(nsDisplayItem* aItem,
                       const StackingContextHelper& aStackingContext)
{
  CLIP_LOG("processing item %p\n", aItem);

=======
wr::WrSpaceAndClipChain ClipManager::SwitchItem(nsDisplayItem* aItem) {
>>>>>>> upstream-releases
  const DisplayItemClipChain* clip = aItem->GetClipChain();
  const ActiveScrolledRoot* asr = aItem->GetActiveScrolledRoot();
  CLIP_LOG("processing item %p (%s) asr %p\n", aItem,
           DisplayItemTypeName(aItem->GetType()), asr);

  DisplayItemType type = aItem->GetType();
  if (type == DisplayItemType::TYPE_STICKY_POSITION) {
    // For sticky position items, the ASR is computed differently depending
    // on whether the item has a fixed descendant or not. But for WebRender
    // purposes we always want to use the ASR that would have been used if it
    // didn't have fixed descendants, which is stored as the "container ASR" on
    // the sticky item.
    asr = static_cast<nsDisplayStickyPosition*>(aItem)->GetContainerASR();
  }

  // In most cases we can combine the leaf of the clip chain with the clip rect
  // of the display item. This reduces the number of clip items, which avoids
  // some overhead further down the pipeline.
  bool separateLeaf = false;
  if (clip && clip->mASR == asr && clip->mClip.GetRoundedRectCount() == 0) {
    // Container display items are not currently supported because the clip
    // rect of a stacking context is not handled the same as normal display
    // items.
    separateLeaf = aItem->GetChildren() == nullptr;
  }

  ItemClips clips(asr, clip, separateLeaf);
  MOZ_ASSERT(!mItemClipStack.empty());
  if (clips.HasSameInputs(mItemClipStack.top())) {
    // Early-exit because if the clips are the same as aItem's previous sibling,
    // then we don't need to do do the work of popping the old stuff and then
    // pushing it right back on for the new item. Note that if aItem doesn't
    // have a previous sibling, that means BeginList would have been called
    // just before this, which will have pushed a ItemClips(nullptr, nullptr)
    // onto mItemClipStack, so the HasSameInputs check should return false.
    CLIP_LOG("\tearly-exit for %p\n", aItem);
    return mItemClipStack.top().GetSpaceAndClipChain();
  }

  // Pop aItem's previous sibling's stuff from mBuilder in preparation for
  // pushing aItem's stuff.
  mItemClipStack.pop();

  // Zoom display items report their bounds etc using the parent document's
  // APD because zoom items act as a conversion layer between the two different
  // APDs.
  int32_t auPerDevPixel;
  if (type == DisplayItemType::TYPE_ZOOM) {
    auPerDevPixel =
        static_cast<nsDisplayZoom*>(aItem)->GetParentAppUnitsPerDevPixel();
  } else {
    auPerDevPixel = aItem->Frame()->PresContext()->AppUnitsPerDevPixel();
  }

  // If the leaf of the clip chain is going to be merged with the display item's
  // clip rect, then we should create a clip chain id from the leaf's parent.
  if (separateLeaf) {
    CLIP_LOG("\tseparate leaf detected, ignoring the last clip\n");
    clip = clip->mParent;
  }

  // There are two ASR chains here that we need to be fully defined. One is the
  // ASR chain pointed to by |asr|. The other is the
  // ASR chain pointed to by clip->mASR. We pick the leafmost
  // of these two chains because that one will include the other. Calling
  // DefineScrollLayers with this leafmost ASR will recursively define all the
  // ASRs that we care about for this item, but will not actually push
  // anything onto the WR stack.
  const ActiveScrolledRoot* leafmostASR = asr;
  if (clip) {
    leafmostASR = ActiveScrolledRoot::PickDescendant(leafmostASR, clip->mASR);
  }
<<<<<<< HEAD
  Maybe<wr::WrClipId> leafmostId =
      DefineScrollLayers(leafmostASR, aItem, aStackingContext);
||||||| merged common ancestors
  Maybe<wr::WrClipId> leafmostId = DefineScrollLayers(leafmostASR, aItem, aStackingContext);
=======
  Maybe<wr::WrSpaceAndClip> leafmostId = DefineScrollLayers(leafmostASR, aItem);
>>>>>>> upstream-releases

  // Define all the clips in the item's clip chain, and obtain a clip chain id
  // for it.
<<<<<<< HEAD
  clips.mClipChainId = DefineClipChain(clip, auPerDevPixel, aStackingContext);

  if (clip) {
    // If the clip's ASR is different, then we need to set the scroll id
    // explicitly to match the desired ASR.
    Maybe<wr::WrClipId> scrollId = GetScrollLayer(asr);
    MOZ_ASSERT(scrollId.isSome());
    clips.mScrollId = ClipIdAfterOverride(scrollId);
  } else {
    // If we don't have a clip at all, then we don't want to explicitly push
    // the ASR either, because as with the first clause of this if condition,
    // the item might get hoisted out of a stacking context that was pushed
    // between the |asr| and this |aItem|. Instead we just leave clips.mScrollId
    // empty and things seem to work out.
    // XXX: there might be cases where things don't just "work out", in which
    // case we might need to do something smarter here.
  }
||||||| merged common ancestors
  clips.mClipChainId = DefineClipChain(clip, auPerDevPixel, aStackingContext);

  if (clip && clip->mASR == asr) {
    // If the clip's ASR is the same as the item's ASR, then we want to use
    // the clip as the "scrollframe" for the item, as WR will do the right thing
    // when building the ClipScrollTree and ensure the item scrolls with the
    // ASR. Note in particular that we don't want to use scroll id of |asr| here
    // because we might have a situation where there is a stacking context
    // between |asr| and |aItem|, and if we used |asr|'s scroll id, then WR
    // would effectively hoist the item out of the stacking context and attach
    // it directly to |asr|. This can produce incorrect results. Using the clip
    // instead of the ASR is strictly better because the clip is usually defined
    // inside the stacking context, and so the item also stays "inside" the
    // stacking context rather than geting hoisted out. Note that there might
    // be cases where the clip is also "outside" the stacking context and in
    // theory that situation might not be handled correctly, but I haven't seen
    // it in practice so far.
    const ClipIdMap& cache = mCacheStack.top();
    auto it = cache.find(clip);
    MOZ_ASSERT(it != cache.end());
    clips.mScrollId = Some(it->second);
  } else if (clip) {
    // If the clip's ASR is different, then we need to set the scroll id
    // explicitly to match the desired ASR.
    Maybe<wr::WrClipId> scrollId = GetScrollLayer(asr);
    MOZ_ASSERT(scrollId.isSome());
    clips.mScrollId = ClipIdAfterOverride(scrollId);
  } else {
    // If we don't have a clip at all, then we don't want to explicitly push
    // the ASR either, because as with the first clause of this if condition,
    // the item might get hoisted out of a stacking context that was pushed
    // between the |asr| and this |aItem|. Instead we just leave clips.mScrollId
    // empty and things seem to work out.
    // XXX: there might be cases where things don't just "work out", in which
    // case we might need to do something smarter here.
  }
=======
  clips.mClipChainId = DefineClipChain(clip, auPerDevPixel);

  Maybe<wr::WrSpaceAndClip> spaceAndClip = GetScrollLayer(asr);
  MOZ_ASSERT(spaceAndClip.isSome());
  clips.mScrollId = SpatialIdAfterOverride(spaceAndClip->space);
  CLIP_LOG("\tassigning %d -> %d\n", (int)spaceAndClip->space.id,
           (int)clips.mScrollId.id);
>>>>>>> upstream-releases

  // Now that we have the scroll id and a clip id for the item, push it onto
  // the WR stack.
  clips.UpdateSeparateLeaf(*mBuilder, auPerDevPixel);
  auto spaceAndClipChain = clips.GetSpaceAndClipChain();
  mItemClipStack.push(clips);

  CLIP_LOG("done setup for %p\n", aItem);
  return spaceAndClipChain;
}

<<<<<<< HEAD
Maybe<wr::WrClipId> ClipManager::GetScrollLayer(
    const ActiveScrolledRoot* aASR) {
||||||| merged common ancestors
Maybe<wr::WrClipId>
ClipManager::GetScrollLayer(const ActiveScrolledRoot* aASR)
{
=======
Maybe<wr::WrSpaceAndClip> ClipManager::GetScrollLayer(
    const ActiveScrolledRoot* aASR) {
>>>>>>> upstream-releases
  for (const ActiveScrolledRoot* asr = aASR; asr; asr = asr->mParent) {
<<<<<<< HEAD
    Maybe<wr::WrClipId> scrollId =
        mBuilder->GetScrollIdForDefinedScrollLayer(asr->GetViewId());
    if (scrollId) {
      return scrollId;
||||||| merged common ancestors
    Maybe<wr::WrClipId> scrollId =
      mBuilder->GetScrollIdForDefinedScrollLayer(asr->GetViewId());
    if (scrollId) {
      return scrollId;
=======
    Maybe<wr::WrSpaceAndClip> spaceAndClip =
        mBuilder->GetScrollIdForDefinedScrollLayer(asr->GetViewId());
    if (spaceAndClip) {
      return spaceAndClip;
>>>>>>> upstream-releases
    }

    // If this ASR doesn't have a scroll ID, then we should check its ancestor.
    // There may not be one defined because the ASR may not be scrollable or we
    // failed to get the scroll metadata.
  }

<<<<<<< HEAD
  Maybe<wr::WrClipId> scrollId = mBuilder->GetScrollIdForDefinedScrollLayer(
      ScrollableLayerGuid::NULL_SCROLL_ID);
  MOZ_ASSERT(scrollId.isSome());
  return scrollId;
||||||| merged common ancestors
  Maybe<wr::WrClipId> scrollId =
    mBuilder->GetScrollIdForDefinedScrollLayer(FrameMetrics::NULL_SCROLL_ID);
  MOZ_ASSERT(scrollId.isSome());
  return scrollId;
=======
  Maybe<wr::WrSpaceAndClip> spaceAndClip =
      mBuilder->GetScrollIdForDefinedScrollLayer(
          ScrollableLayerGuid::NULL_SCROLL_ID);
  MOZ_ASSERT(spaceAndClip.isSome());
  return spaceAndClip;
>>>>>>> upstream-releases
}

<<<<<<< HEAD
Maybe<wr::WrClipId> ClipManager::DefineScrollLayers(
    const ActiveScrolledRoot* aASR, nsDisplayItem* aItem,
    const StackingContextHelper& aSc) {
||||||| merged common ancestors
Maybe<wr::WrClipId>
ClipManager::DefineScrollLayers(const ActiveScrolledRoot* aASR,
                                nsDisplayItem* aItem,
                                const StackingContextHelper& aSc)
{
=======
Maybe<wr::WrSpaceAndClip> ClipManager::DefineScrollLayers(
    const ActiveScrolledRoot* aASR, nsDisplayItem* aItem) {
>>>>>>> upstream-releases
  if (!aASR) {
    // Recursion base case
    return Nothing();
  }
<<<<<<< HEAD
  ScrollableLayerGuid::ViewID viewId = aASR->GetViewId();
  Maybe<wr::WrClipId> scrollId =
      mBuilder->GetScrollIdForDefinedScrollLayer(viewId);
  if (scrollId) {
||||||| merged common ancestors
  FrameMetrics::ViewID viewId = aASR->GetViewId();
  Maybe<wr::WrClipId> scrollId = mBuilder->GetScrollIdForDefinedScrollLayer(viewId);
  if (scrollId) {
=======
  ScrollableLayerGuid::ViewID viewId = aASR->GetViewId();
  Maybe<wr::WrSpaceAndClip> spaceAndClip =
      mBuilder->GetScrollIdForDefinedScrollLayer(viewId);
  if (spaceAndClip) {
>>>>>>> upstream-releases
    // If we've already defined this scroll layer before, we can early-exit
    return spaceAndClip;
  }
  // Recurse to define the ancestors
<<<<<<< HEAD
  Maybe<wr::WrClipId> ancestorScrollId =
      DefineScrollLayers(aASR->mParent, aItem, aSc);
||||||| merged common ancestors
  Maybe<wr::WrClipId> ancestorScrollId = DefineScrollLayers(aASR->mParent, aItem, aSc);
=======
  Maybe<wr::WrSpaceAndClip> ancestorSpaceAndClip =
      DefineScrollLayers(aASR->mParent, aItem);
>>>>>>> upstream-releases

  Maybe<ScrollMetadata> metadata =
      aASR->mScrollableFrame->ComputeScrollMetadata(
          mManager, aItem->ReferenceFrame(), Nothing(), nullptr);
  if (!metadata) {
    MOZ_ASSERT_UNREACHABLE("Expected scroll metadata to be available!");
    return ancestorSpaceAndClip;
  }

  FrameMetrics& metrics = metadata->GetMetrics();
  if (!metrics.IsScrollable()) {
    // This item is a scrolling no-op, skip over it in the ASR chain.
    return ancestorSpaceAndClip;
  }

  LayoutDeviceRect contentRect =
      metrics.GetExpandedScrollableRect() * metrics.GetDevPixelsPerCSSPixel();
  LayoutDeviceRect clipBounds = LayoutDeviceRect::FromUnknownRect(
      metrics.GetCompositionBounds().ToUnknownRect());
  // The content rect that we hand to PushScrollLayer should be relative to
  // the same origin as the clipBounds that we hand to PushScrollLayer - that
  // is, both of them should be relative to the stacking context `aSc`.
  // However, when we get the scrollable rect from the FrameMetrics, the origin
  // has nothing to do with the position of the frame but instead represents
  // the minimum allowed scroll offset of the scrollable content. While APZ
  // uses this to clamp the scroll position, we don't need to send this to
  // WebRender at all. Instead, we take the position from the composition
  // bounds.
  contentRect.MoveTo(clipBounds.TopLeft());

<<<<<<< HEAD
  Maybe<wr::WrClipId> parent = ClipIdAfterOverride(ancestorScrollId);
  scrollId = Some(mBuilder->DefineScrollLayer(
      viewId, parent, wr::ToRoundedLayoutRect(contentRect),
      wr::ToRoundedLayoutRect(clipBounds)));

  return scrollId;
||||||| merged common ancestors
  Maybe<wr::WrClipId> parent = ClipIdAfterOverride(ancestorScrollId);
  scrollId = Some(mBuilder->DefineScrollLayer(viewId, parent,
      wr::ToRoundedLayoutRect(contentRect),
      wr::ToRoundedLayoutRect(clipBounds)));

  return scrollId;
=======
  Maybe<wr::WrSpaceAndClip> parent = ancestorSpaceAndClip;
  if (parent) {
    parent->space = SpatialIdAfterOverride(parent->space);
  }
  LayoutDevicePoint scrollOffset =
      metrics.GetScrollOffset() * metrics.GetDevPixelsPerCSSPixel();
  return Some(mBuilder->DefineScrollLayer(
      viewId, parent, wr::ToRoundedLayoutRect(contentRect),
      wr::ToRoundedLayoutRect(clipBounds), wr::ToLayoutPoint(scrollOffset)));
>>>>>>> upstream-releases
}

<<<<<<< HEAD
Maybe<wr::WrClipChainId> ClipManager::DefineClipChain(
    const DisplayItemClipChain* aChain, int32_t aAppUnitsPerDevPixel,
    const StackingContextHelper& aSc) {
  AutoTArray<wr::WrClipId, 6> clipIds;
||||||| merged common ancestors
Maybe<wr::WrClipChainId>
ClipManager::DefineClipChain(const DisplayItemClipChain* aChain,
                             int32_t aAppUnitsPerDevPixel,
                             const StackingContextHelper& aSc)
{
  nsTArray<wr::WrClipId> clipIds;
=======
Maybe<wr::WrClipChainId> ClipManager::DefineClipChain(
    const DisplayItemClipChain* aChain, int32_t aAppUnitsPerDevPixel) {
  AutoTArray<wr::WrClipId, 6> clipIds;
>>>>>>> upstream-releases
  // Iterate through the clips in the current item's clip chain, define them
  // in WR, and put their IDs into |clipIds|.
  for (const DisplayItemClipChain* chain = aChain; chain;
       chain = chain->mParent) {
    ClipIdMap& cache = mCacheStack.top();
    auto it = cache.find(chain);
    if (it != cache.end()) {
      // Found it in the currently-active cache, so just use the id we have for
      // it.
      CLIP_LOG("cache[%p] => %zu\n", chain, it->second.id);
      clipIds.AppendElement(it->second);
      continue;
    }
    if (!chain->mClip.HasClip()) {
      // This item in the chain is a no-op, skip over it
      continue;
    }

    LayoutDeviceRect clip = LayoutDeviceRect::FromAppUnits(
        chain->mClip.GetClipRect(), aAppUnitsPerDevPixel);
    nsTArray<wr::ComplexClipRegion> wrRoundedRects;
<<<<<<< HEAD
    chain->mClip.ToComplexClipRegions(aAppUnitsPerDevPixel, aSc,
                                      wrRoundedRects);
||||||| merged common ancestors
    chain->mClip.ToComplexClipRegions(aAppUnitsPerDevPixel, aSc, wrRoundedRects);
=======
    chain->mClip.ToComplexClipRegions(aAppUnitsPerDevPixel, wrRoundedRects);
>>>>>>> upstream-releases

    Maybe<wr::WrSpaceAndClip> spaceAndClip = GetScrollLayer(chain->mASR);
    // Before calling DefineClipChain we defined the ASRs by calling
    // DefineScrollLayers, so we must have a scrollId here.
    MOZ_ASSERT(spaceAndClip.isSome());

    // Define the clip
    spaceAndClip->space = SpatialIdAfterOverride(spaceAndClip->space);
    wr::WrClipId clipId = mBuilder->DefineClip(
<<<<<<< HEAD
        parent, wr::ToRoundedLayoutRect(clip), &wrRoundedRects);
||||||| merged common ancestors
        parent,
        wr::ToRoundedLayoutRect(clip), &wrRoundedRects);
=======
        spaceAndClip, wr::ToRoundedLayoutRect(clip), &wrRoundedRects);
>>>>>>> upstream-releases
    clipIds.AppendElement(clipId);
    cache[chain] = clipId;
    CLIP_LOG("cache[%p] <= %zu\n", chain, clipId.id);
  }

  if (clipIds.IsEmpty()) {
    return Nothing();
  }

  return Some(mBuilder->DefineClipChain(clipIds));
}

ClipManager::~ClipManager() {
  MOZ_ASSERT(!mBuilder);
  MOZ_ASSERT(mCacheStack.empty());
  MOZ_ASSERT(mItemClipStack.empty());
}

ClipManager::ItemClips::ItemClips(const ActiveScrolledRoot* aASR,
                                  const DisplayItemClipChain* aChain,
                                  bool aSeparateLeaf)
<<<<<<< HEAD
    : mASR(aASR),
      mChain(aChain),
      mSeparateLeaf(aSeparateLeaf),
      mApplied(false) {}
||||||| merged common ancestors
  : mASR(aASR)
  , mChain(aChain)
  , mSeparateLeaf(aSeparateLeaf)
  , mApplied(false)
{
}
=======
    : mASR(aASR), mChain(aChain), mSeparateLeaf(aSeparateLeaf) {
  mScrollId = wr::wr_root_scroll_node_id();
}
>>>>>>> upstream-releases

<<<<<<< HEAD
void ClipManager::ItemClips::Apply(wr::DisplayListBuilder* aBuilder,
                                   int32_t aAppUnitsPerDevPixel) {
  MOZ_ASSERT(!mApplied);
  mApplied = true;

||||||| merged common ancestors
void
ClipManager::ItemClips::Apply(wr::DisplayListBuilder* aBuilder,
                              int32_t aAppUnitsPerDevPixel)
{
  MOZ_ASSERT(!mApplied);
  mApplied = true;

=======
void ClipManager::ItemClips::UpdateSeparateLeaf(
    wr::DisplayListBuilder& aBuilder, int32_t aAppUnitsPerDevPixel) {
>>>>>>> upstream-releases
  Maybe<wr::LayoutRect> clipLeaf;
  if (mSeparateLeaf) {
    MOZ_ASSERT(mChain);
    clipLeaf.emplace(wr::ToRoundedLayoutRect(LayoutDeviceRect::FromAppUnits(
        mChain->mClip.GetClipRect(), aAppUnitsPerDevPixel)));
  }

<<<<<<< HEAD
  aBuilder->PushClipAndScrollInfo(mScrollId.ptrOr(nullptr),
                                  mClipChainId.ptrOr(nullptr), clipLeaf);
}

void ClipManager::ItemClips::Unapply(wr::DisplayListBuilder* aBuilder) {
  if (mApplied) {
    mApplied = false;
    aBuilder->PopClipAndScrollInfo(mScrollId.ptrOr(nullptr));
  }
||||||| merged common ancestors
  aBuilder->PushClipAndScrollInfo(mScrollId.ptrOr(nullptr),
                                  mClipChainId.ptrOr(nullptr),
                                  clipLeaf);
}

void
ClipManager::ItemClips::Unapply(wr::DisplayListBuilder* aBuilder)
{
  if (mApplied) {
    mApplied = false;
    aBuilder->PopClipAndScrollInfo(mScrollId.ptrOr(nullptr));
  }
=======
  aBuilder.SetClipChainLeaf(clipLeaf);
>>>>>>> upstream-releases
}

bool ClipManager::ItemClips::HasSameInputs(const ItemClips& aOther) {
  return mASR == aOther.mASR && mChain == aOther.mChain &&
         mSeparateLeaf == aOther.mSeparateLeaf;
}

void ClipManager::ItemClips::CopyOutputsFrom(const ItemClips& aOther) {
  mScrollId = aOther.mScrollId;
  mClipChainId = aOther.mClipChainId;
}

<<<<<<< HEAD
}  // namespace layers
}  // namespace mozilla
||||||| merged common ancestors
} // namespace layers
} // namespace mozilla
=======
wr::WrSpaceAndClipChain ClipManager::ItemClips::GetSpaceAndClipChain() const {
  auto spaceAndClipChain = wr::RootScrollNodeWithChain();
  spaceAndClipChain.space = mScrollId;
  if (mClipChainId) {
    spaceAndClipChain.clip_chain = mClipChainId->id;
  }
  return spaceAndClipChain;
}

}  // namespace layers
}  // namespace mozilla
>>>>>>> upstream-releases
