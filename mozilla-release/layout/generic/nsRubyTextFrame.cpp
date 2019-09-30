/* -*- Mode: C++; tab-width: 8; indent-tabs-mode: nil; c-basic-offset: 2 -*- */
/* vim: set ts=8 sts=2 et sw=2 tw=80: */
/* This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/. */

/* rendering object for CSS "display: ruby-text" */

#include "nsRubyTextFrame.h"

#include "mozilla/ComputedStyle.h"
#include "mozilla/PresShell.h"
#include "mozilla/WritingModes.h"
#include "nsLineLayout.h"
#include "nsPresContext.h"

using namespace mozilla;

//----------------------------------------------------------------------

// Frame class boilerplate
// =======================

NS_QUERYFRAME_HEAD(nsRubyTextFrame)
  NS_QUERYFRAME_ENTRY(nsRubyTextFrame)
NS_QUERYFRAME_TAIL_INHERITING(nsRubyContentFrame)

NS_IMPL_FRAMEARENA_HELPERS(nsRubyTextFrame)

<<<<<<< HEAD
nsContainerFrame* NS_NewRubyTextFrame(nsIPresShell* aPresShell,
                                      ComputedStyle* aStyle) {
  return new (aPresShell) nsRubyTextFrame(aStyle);
||||||| merged common ancestors
nsContainerFrame*
NS_NewRubyTextFrame(nsIPresShell* aPresShell,
                    ComputedStyle* aStyle)
{
  return new (aPresShell) nsRubyTextFrame(aStyle);
=======
nsContainerFrame* NS_NewRubyTextFrame(PresShell* aPresShell,
                                      ComputedStyle* aStyle) {
  return new (aPresShell) nsRubyTextFrame(aStyle, aPresShell->GetPresContext());
>>>>>>> upstream-releases
}

//----------------------------------------------------------------------

// nsRubyTextFrame Method Implementations
// ======================================

<<<<<<< HEAD
/* virtual */ bool nsRubyTextFrame::CanContinueTextRun() const { return false; }
||||||| merged common ancestors
/* virtual */ bool
nsRubyTextFrame::CanContinueTextRun() const
{
  return false;
}
=======
/* virtual */
bool nsRubyTextFrame::CanContinueTextRun() const { return false; }
>>>>>>> upstream-releases

#ifdef DEBUG_FRAME_DUMP
nsresult nsRubyTextFrame::GetFrameName(nsAString& aResult) const {
  return MakeFrameName(NS_LITERAL_STRING("RubyText"), aResult);
}
#endif

<<<<<<< HEAD
/* virtual */ void nsRubyTextFrame::BuildDisplayList(
    nsDisplayListBuilder* aBuilder, const nsDisplayListSet& aLists) {
||||||| merged common ancestors


/* virtual */ void
nsRubyTextFrame::BuildDisplayList(nsDisplayListBuilder*   aBuilder,
                                  const nsDisplayListSet& aLists)
{
=======
/* virtual */
void nsRubyTextFrame::BuildDisplayList(nsDisplayListBuilder* aBuilder,
                                       const nsDisplayListSet& aLists) {
>>>>>>> upstream-releases
  if (IsAutoHidden()) {
    return;
  }

  nsRubyContentFrame::BuildDisplayList(aBuilder, aLists);
}

<<<<<<< HEAD
/* virtual */ void nsRubyTextFrame::Reflow(nsPresContext* aPresContext,
                                           ReflowOutput& aDesiredSize,
                                           const ReflowInput& aReflowInput,
                                           nsReflowStatus& aStatus) {
||||||| merged common ancestors
/* virtual */ void
nsRubyTextFrame::Reflow(nsPresContext* aPresContext,
                        ReflowOutput& aDesiredSize,
                        const ReflowInput& aReflowInput,
                        nsReflowStatus& aStatus)
{
=======
/* virtual */
void nsRubyTextFrame::Reflow(nsPresContext* aPresContext,
                             ReflowOutput& aDesiredSize,
                             const ReflowInput& aReflowInput,
                             nsReflowStatus& aStatus) {
>>>>>>> upstream-releases
  // Even if we want to hide this frame, we have to reflow it first.
  // If we leave it dirty, changes to its content will never be
  // propagated to the ancestors, then it won't be displayed even if
  // the content is no longer the same, until next reflow triggered by
  // some other change. In general, we always reflow all the frames we
  // created. There might be other problems if we don't do that.
  nsRubyContentFrame::Reflow(aPresContext, aDesiredSize, aReflowInput, aStatus);

  if (IsAutoHidden()) {
    // Reset the ISize. The BSize is not changed so that it won't
    // affect vertical positioning in unexpected way.
    WritingMode lineWM = aReflowInput.mLineLayout->GetWritingMode();
    aDesiredSize.ISize(lineWM) = 0;
    aDesiredSize.SetOverflowAreasToDesiredBounds();
  }
}
