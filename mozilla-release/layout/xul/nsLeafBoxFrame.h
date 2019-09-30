/* -*- Mode: C++; tab-width: 8; indent-tabs-mode: nil; c-basic-offset: 2 -*- */
/* vim: set ts=8 sts=2 et sw=2 tw=80: */
/* This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/. */
#ifndef nsLeafBoxFrame_h___
#define nsLeafBoxFrame_h___

#include "mozilla/Attributes.h"
#include "nsLeafFrame.h"
#include "nsBox.h"

<<<<<<< HEAD
class nsLeafBoxFrame : public nsLeafFrame {
 public:
||||||| merged common ancestors
class nsLeafBoxFrame : public nsLeafFrame
{
public:
=======
namespace mozilla {
class PresShell;
}  // namespace mozilla

class nsLeafBoxFrame : public nsLeafFrame {
 public:
>>>>>>> upstream-releases
  NS_DECL_FRAMEARENA_HELPERS(nsLeafBoxFrame)

<<<<<<< HEAD
  friend nsIFrame* NS_NewLeafBoxFrame(nsIPresShell* aPresShell,
                                      ComputedStyle* aStyle);
||||||| merged common ancestors
  friend nsIFrame* NS_NewLeafBoxFrame(nsIPresShell* aPresShell, ComputedStyle* aStyle);
=======
  friend nsIFrame* NS_NewLeafBoxFrame(mozilla::PresShell* aPresShell,
                                      ComputedStyle* aStyle);
>>>>>>> upstream-releases

  virtual nsSize GetXULPrefSize(nsBoxLayoutState& aState) override;
  virtual nsSize GetXULMinSize(nsBoxLayoutState& aState) override;
  virtual nsSize GetXULMaxSize(nsBoxLayoutState& aState) override;
  virtual nscoord GetXULFlex() override;
  virtual nscoord GetXULBoxAscent(nsBoxLayoutState& aState) override;

  virtual bool IsFrameOfType(uint32_t aFlags) const override {
    // This is bogus, but it's what we've always done.
    // Note that nsLeafFrame is also eReplacedContainsBlock.
    return nsLeafFrame::IsFrameOfType(
        aFlags & ~(nsIFrame::eReplaced | nsIFrame::eReplacedContainsBlock |
                   nsIFrame::eXULBox));
  }

#ifdef DEBUG_FRAME_DUMP
  virtual nsresult GetFrameName(nsAString& aResult) const override;
#endif

  // nsIHTMLReflow overrides

<<<<<<< HEAD
  virtual void MarkIntrinsicISizesDirty() override;
  virtual nscoord GetMinISize(gfxContext* aRenderingContext) override;
  virtual nscoord GetPrefISize(gfxContext* aRenderingContext) override;
||||||| merged common ancestors
  virtual void MarkIntrinsicISizesDirty() override;
  virtual nscoord GetMinISize(gfxContext *aRenderingContext) override;
  virtual nscoord GetPrefISize(gfxContext *aRenderingContext) override;
=======
  virtual nscoord GetMinISize(gfxContext* aRenderingContext) override;
  virtual nscoord GetPrefISize(gfxContext* aRenderingContext) override;
>>>>>>> upstream-releases

  // Our auto size is that provided by nsFrame, not nsLeafFrame
  virtual mozilla::LogicalSize ComputeAutoSize(
      gfxContext* aRenderingContext, mozilla::WritingMode aWM,
      const mozilla::LogicalSize& aCBSize, nscoord aAvailableISize,
      const mozilla::LogicalSize& aMargin, const mozilla::LogicalSize& aBorder,
      const mozilla::LogicalSize& aPadding, ComputeSizeFlags aFlags) override;

  virtual void Reflow(nsPresContext* aPresContext, ReflowOutput& aDesiredSize,
                      const ReflowInput& aReflowInput,
                      nsReflowStatus& aStatus) override;

  virtual nsresult CharacterDataChanged(
      const CharacterDataChangeInfo&) override;

  virtual void Init(nsIContent* aContent, nsContainerFrame* aParent,
                    nsIFrame* asPrevInFlow) override;

  virtual void BuildDisplayList(nsDisplayListBuilder* aBuilder,
                                const nsDisplayListSet& aLists) override;

  virtual nsresult AttributeChanged(int32_t aNameSpaceID, nsAtom* aAttribute,
                                    int32_t aModType) override;

  virtual bool ComputesOwnOverflowArea() override { return false; }

 protected:
  NS_IMETHOD DoXULLayout(nsBoxLayoutState& aState) override;

  virtual nscoord GetIntrinsicISize() override;

<<<<<<< HEAD
  explicit nsLeafBoxFrame(ComputedStyle* aStyle, ClassID aID = kClassID)
      : nsLeafFrame(aStyle, aID) {}
||||||| merged common ancestors
  explicit nsLeafBoxFrame(ComputedStyle* aStyle, ClassID aID = kClassID)
    : nsLeafFrame(aStyle, aID)
  {}

private:

 void UpdateMouseThrough();
=======
  explicit nsLeafBoxFrame(ComputedStyle* aStyle, nsPresContext* aPresContext,
                          ClassID aID = kClassID)
      : nsLeafFrame(aStyle, aPresContext, aID) {}
>>>>>>> upstream-releases

 private:
  void UpdateMouseThrough();

};  // class nsLeafBoxFrame

#endif /* nsLeafBoxFrame_h___ */
