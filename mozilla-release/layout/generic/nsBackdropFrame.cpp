/* -*- Mode: C++; tab-width: 8; indent-tabs-mode: nil; c-basic-offset: 2 -*- */
/* vim: set ts=8 sts=2 et sw=2 tw=80: */
/* This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/. */

/* rendering object for CSS "::backdrop" */

#include "nsBackdropFrame.h"

#include "nsDisplayList.h"

using namespace mozilla;

NS_IMPL_FRAMEARENA_HELPERS(nsBackdropFrame)

#ifdef DEBUG_FRAME_DUMP
nsresult nsBackdropFrame::GetFrameName(nsAString& aResult) const {
  return MakeFrameName(NS_LITERAL_STRING("Backdrop"), aResult);
}
#endif

<<<<<<< HEAD
/* virtual */ ComputedStyle* nsBackdropFrame::GetParentComputedStyle(
    nsIFrame** aProviderFrame) const {
||||||| merged common ancestors
/* virtual */ ComputedStyle*
nsBackdropFrame::GetParentComputedStyle(nsIFrame** aProviderFrame) const
{
=======
/* virtual */
ComputedStyle* nsBackdropFrame::GetParentComputedStyle(
    nsIFrame** aProviderFrame) const {
>>>>>>> upstream-releases
  // Style context of backdrop pseudo-element does not inherit from
  // any element, per the Fullscreen API spec.
  *aProviderFrame = nullptr;
  return nullptr;
}

<<<<<<< HEAD
/* virtual */ void nsBackdropFrame::BuildDisplayList(
    nsDisplayListBuilder* aBuilder, const nsDisplayListSet& aLists) {
||||||| merged common ancestors
/* virtual */ void
nsBackdropFrame::BuildDisplayList(nsDisplayListBuilder* aBuilder,
                                  const nsDisplayListSet& aLists)
{
=======
/* virtual */
void nsBackdropFrame::BuildDisplayList(nsDisplayListBuilder* aBuilder,
                                       const nsDisplayListSet& aLists) {
>>>>>>> upstream-releases
  DO_GLOBAL_REFLOW_COUNT_DSP("nsBackdropFrame");
  // We want this frame to always be there even if its display value is
  // none or contents so that we can respond to style change on it. To
  // support those values, we skip painting ourselves in those cases.
  auto display = StyleDisplay()->mDisplay;
  if (display == mozilla::StyleDisplay::None ||
      display == mozilla::StyleDisplay::Contents) {
    return;
  }

  DisplayBorderBackgroundOutline(aBuilder, aLists);
}

<<<<<<< HEAD
/* virtual */ LogicalSize nsBackdropFrame::ComputeAutoSize(
    gfxContext* aRenderingContext, WritingMode aWM, const LogicalSize& aCBSize,
    nscoord aAvailableISize, const LogicalSize& aMargin,
    const LogicalSize& aBorder, const LogicalSize& aPadding,
    ComputeSizeFlags aFlags) {
||||||| merged common ancestors
/* virtual */ LogicalSize
nsBackdropFrame::ComputeAutoSize(gfxContext*         aRenderingContext,
                                 WritingMode         aWM,
                                 const LogicalSize&  aCBSize,
                                 nscoord             aAvailableISize,
                                 const LogicalSize&  aMargin,
                                 const LogicalSize&  aBorder,
                                 const LogicalSize&  aPadding,
                                 ComputeSizeFlags    aFlags)
{
=======
/* virtual */
LogicalSize nsBackdropFrame::ComputeAutoSize(
    gfxContext* aRenderingContext, WritingMode aWM, const LogicalSize& aCBSize,
    nscoord aAvailableISize, const LogicalSize& aMargin,
    const LogicalSize& aBorder, const LogicalSize& aPadding,
    ComputeSizeFlags aFlags) {
>>>>>>> upstream-releases
  // Note that this frame is a child of the viewport frame.
  LogicalSize result(aWM, 0xdeadbeef, NS_UNCONSTRAINEDSIZE);
  if (aFlags & ComputeSizeFlags::eShrinkWrap) {
    result.ISize(aWM) = 0;
  } else {
    result.ISize(aWM) = aAvailableISize - aMargin.ISize(aWM) -
                        aBorder.ISize(aWM) - aPadding.ISize(aWM);
  }
  return result;
}

<<<<<<< HEAD
/* virtual */ void nsBackdropFrame::Reflow(nsPresContext* aPresContext,
                                           ReflowOutput& aDesiredSize,
                                           const ReflowInput& aReflowInput,
                                           nsReflowStatus& aStatus) {
||||||| merged common ancestors
/* virtual */ void
nsBackdropFrame::Reflow(nsPresContext* aPresContext,
                        ReflowOutput& aDesiredSize,
                        const ReflowInput& aReflowInput,
                        nsReflowStatus& aStatus)
{
=======
/* virtual */
void nsBackdropFrame::Reflow(nsPresContext* aPresContext,
                             ReflowOutput& aDesiredSize,
                             const ReflowInput& aReflowInput,
                             nsReflowStatus& aStatus) {
>>>>>>> upstream-releases
  MarkInReflow();
  DO_GLOBAL_REFLOW_COUNT("nsBackdropFrame");
  DISPLAY_REFLOW(aPresContext, this, aReflowInput, aDesiredSize, aStatus);
  MOZ_ASSERT(aStatus.IsEmpty(), "Caller should pass a fresh reflow status!");

  // Note that this frame is a child of the viewport frame.
  WritingMode wm = aReflowInput.GetWritingMode();
  LogicalMargin borderPadding = aReflowInput.ComputedLogicalBorderPadding();
  nscoord isize = aReflowInput.ComputedISize() + borderPadding.IStartEnd(wm);
  nscoord bsize = aReflowInput.ComputedBSize() + borderPadding.BStartEnd(wm);
  aDesiredSize.SetSize(wm, LogicalSize(wm, isize, bsize));
}
