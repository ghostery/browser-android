/* -*- Mode: C++; tab-width: 8; indent-tabs-mode: nil; c-basic-offset: 2 -*- */
/* vim: set ts=8 sts=2 et sw=2 tw=80: */
/* This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/. */

#ifndef nsMathMLSelectedFrame_h___
#define nsMathMLSelectedFrame_h___

#include "nsMathMLContainerFrame.h"

class nsMathMLSelectedFrame : public nsMathMLContainerFrame {
<<<<<<< HEAD
 public:
||||||| merged common ancestors
public:
=======
 public:
  NS_DECL_ABSTRACT_FRAME(nsMathMLSelectedFrame)

>>>>>>> upstream-releases
  NS_IMETHOD
  TransmitAutomaticData() override;

  virtual void SetInitialChildList(ChildListID aListID,
                                   nsFrameList& aChildList) override;

  virtual nsresult ChildListChanged(int32_t aModType) override;

  virtual void BuildDisplayList(nsDisplayListBuilder* aBuilder,
                                const nsDisplayListSet& aLists) override;

<<<<<<< HEAD
  virtual nsresult Place(DrawTarget* aDrawTarget, bool aPlaceOrigin,
                         ReflowOutput& aDesiredSize) override;

  virtual mozilla::LogicalSize ComputeSize(
      gfxContext* aRenderingContext, mozilla::WritingMode aWritingMode,
      const mozilla::LogicalSize& aCBSize, nscoord aAvailableISize,
      const mozilla::LogicalSize& aMargin, const mozilla::LogicalSize& aBorder,
      const mozilla::LogicalSize& aPadding, ComputeSizeFlags aFlags) override;

  virtual void Reflow(nsPresContext* aPresContext, ReflowOutput& aDesiredSize,
                      const ReflowInput& aReflowInput,
                      nsReflowStatus& aStatus) override;

  virtual nsQueryFrame::FrameIID GetFrameId() override = 0;

 protected:
  nsMathMLSelectedFrame(ComputedStyle* aStyle, ClassID aID)
      : nsMathMLContainerFrame(aStyle, aID),
        mSelectedFrame(nullptr),
        mInvalidMarkup(false) {}
||||||| merged common ancestors
  virtual nsresult
  Place(DrawTarget*          aDrawTarget,
        bool                 aPlaceOrigin,
        ReflowOutput& aDesiredSize) override;

  virtual mozilla::LogicalSize
  ComputeSize(gfxContext *aRenderingContext,
              mozilla::WritingMode aWritingMode,
              const mozilla::LogicalSize& aCBSize,
              nscoord aAvailableISize,
              const mozilla::LogicalSize& aMargin,
              const mozilla::LogicalSize& aBorder,
              const mozilla::LogicalSize& aPadding,
              ComputeSizeFlags aFlags) override;

  virtual void
  Reflow(nsPresContext*          aPresContext,
         ReflowOutput&     aDesiredSize,
         const ReflowInput& aReflowInput,
         nsReflowStatus&          aStatus) override;

  virtual nsQueryFrame::FrameIID GetFrameId() override = 0;

protected:
  nsMathMLSelectedFrame(ComputedStyle* aStyle, ClassID aID) :
    nsMathMLContainerFrame(aStyle, aID),
    mSelectedFrame(nullptr),
    mInvalidMarkup(false) {}
=======
  virtual nsresult Place(DrawTarget* aDrawTarget, bool aPlaceOrigin,
                         ReflowOutput& aDesiredSize) override;

  virtual mozilla::LogicalSize ComputeSize(
      gfxContext* aRenderingContext, mozilla::WritingMode aWritingMode,
      const mozilla::LogicalSize& aCBSize, nscoord aAvailableISize,
      const mozilla::LogicalSize& aMargin, const mozilla::LogicalSize& aBorder,
      const mozilla::LogicalSize& aPadding, ComputeSizeFlags aFlags) override;

  virtual void Reflow(nsPresContext* aPresContext, ReflowOutput& aDesiredSize,
                      const ReflowInput& aReflowInput,
                      nsReflowStatus& aStatus) override;

 protected:
  nsMathMLSelectedFrame(ComputedStyle* aStyle, nsPresContext* aPresContext,
                        ClassID aID)
      : nsMathMLContainerFrame(aStyle, aPresContext, aID),
        mSelectedFrame(nullptr),
        mInvalidMarkup(false) {}
>>>>>>> upstream-releases
  virtual ~nsMathMLSelectedFrame();

  virtual nsIFrame* GetSelectedFrame() = 0;
<<<<<<< HEAD
  nsIFrame* mSelectedFrame;

  bool mInvalidMarkup;
||||||| merged common ancestors
  nsIFrame*       mSelectedFrame;

  bool            mInvalidMarkup;
=======
  nsIFrame* mSelectedFrame;
>>>>>>> upstream-releases

<<<<<<< HEAD
 private:
  void* operator new(size_t, nsIPresShell*) MOZ_MUST_OVERRIDE = delete;
||||||| merged common ancestors
private:
  void* operator new(size_t, nsIPresShell*) MOZ_MUST_OVERRIDE = delete;
=======
  bool mInvalidMarkup;
>>>>>>> upstream-releases
};

#endif /* nsMathMLSelectedFrame_h___ */
