/* -*- Mode: C++; tab-width: 8; indent-tabs-mode: nil; c-basic-offset: 2 -*- */
/* vim: set ts=8 sts=2 et sw=2 tw=80: */
/* This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/. */
#ifndef nsPageSequenceFrame_h___
#define nsPageSequenceFrame_h___

#include "mozilla/Attributes.h"
#include "nsPageSequenceFrame.h"
#include "nsContainerFrame.h"
#include "nsIPrintSettings.h"

namespace mozilla {

class PresShell;

namespace dom {

class HTMLCanvasElement;

}  // namespace dom
}  // namespace mozilla

//-----------------------------------------------
// This class maintains all the data that
// is used by all the page frame
// It lives while the nsPageSequenceFrame lives
class nsSharedPageData {
 public:
  // This object a shared by all the nsPageFrames
  // parented to a nsPageSequenceFrame
  nsSharedPageData() : mShrinkToFitRatio(1.0f) {}

  nsString mDateTimeStr;
  nsString mPageNumFormat;
  nsString mPageNumAndTotalsFormat;
  nsString mDocTitle;
  nsString mDocURL;
  nsFont mHeadFootFont;

  nsSize mReflowSize;
  nsMargin mReflowMargin;
  // Margin for headers and footers; it defaults to 4/100 of an inch on UNIX
  // and 0 elsewhere; I think it has to do with some inconsistency in page size
  // computations
  nsMargin mEdgePaperMargin;

  nsCOMPtr<nsIPrintSettings> mPrintSettings;

  // The scaling ratio we need to apply to make all pages fit horizontally. It's
  // the minimum "ComputedWidth / OverflowWidth" ratio of all page content
  // frames that overflowed.  It's 1.0 if none overflowed horizontally.
  float mShrinkToFitRatio;
};

// Simple page sequence frame class. Used when we're in paginated mode
<<<<<<< HEAD:mozilla-release/layout/generic/nsSimplePageSequenceFrame.h
class nsSimplePageSequenceFrame final : public nsContainerFrame,
                                        public nsIPageSequenceFrame {
 public:
  friend nsSimplePageSequenceFrame* NS_NewSimplePageSequenceFrame(
      nsIPresShell* aPresShell, ComputedStyle* aStyle);
||||||| merged common ancestors
class nsSimplePageSequenceFrame final
  : public nsContainerFrame
  , public nsIPageSequenceFrame
{
public:
  friend nsSimplePageSequenceFrame* NS_NewSimplePageSequenceFrame(nsIPresShell* aPresShell,
                                                                  ComputedStyle* aStyle);
=======
class nsPageSequenceFrame final : public nsContainerFrame {
 public:
  friend nsPageSequenceFrame* NS_NewPageSequenceFrame(
      mozilla::PresShell* aPresShell, ComputedStyle* aStyle);
>>>>>>> upstream-releases:mozilla-release/layout/generic/nsPageSequenceFrame.h

  NS_DECL_QUERYFRAME
  NS_DECL_FRAMEARENA_HELPERS(nsPageSequenceFrame)

  // nsIFrame
  void Reflow(nsPresContext* aPresContext, ReflowOutput& aDesiredSize,
              const ReflowInput& aReflowInput,
              nsReflowStatus& aStatus) override;

  void BuildDisplayList(nsDisplayListBuilder* aBuilder,
                        const nsDisplayListSet& aLists) override;

  // For Shrink To Fit
  float GetSTFPercent() const { return mPageData->mShrinkToFitRatio; }

  // Async Printing
<<<<<<< HEAD:mozilla-release/layout/generic/nsSimplePageSequenceFrame.h
  NS_IMETHOD StartPrint(nsPresContext* aPresContext,
                        nsIPrintSettings* aPrintSettings,
                        const nsAString& aDocTitle,
                        const nsAString& aDocURL) override;
  NS_IMETHOD PrePrintNextPage(nsITimerCallback* aCallback,
                              bool* aDone) override;
  NS_IMETHOD PrintNextPage() override;
  NS_IMETHOD ResetPrintCanvasList() override;
  NS_IMETHOD GetCurrentPageNum(int32_t* aPageNum) override;
  NS_IMETHOD GetNumPages(int32_t* aNumPages) override;
  NS_IMETHOD IsDoingPrintRange(bool* aDoing) override;
  NS_IMETHOD GetPrintRange(int32_t* aFromPage, int32_t* aToPage) override;
  NS_IMETHOD DoPageEnd() override;
||||||| merged common ancestors
  NS_IMETHOD StartPrint(nsPresContext*    aPresContext,
                        nsIPrintSettings* aPrintSettings,
                        const nsAString&  aDocTitle,
                        const nsAString&  aDocURL) override;
  NS_IMETHOD PrePrintNextPage(nsITimerCallback* aCallback, bool* aDone) override;
  NS_IMETHOD PrintNextPage() override;
  NS_IMETHOD ResetPrintCanvasList() override;
  NS_IMETHOD GetCurrentPageNum(int32_t* aPageNum) override;
  NS_IMETHOD GetNumPages(int32_t* aNumPages) override;
  NS_IMETHOD IsDoingPrintRange(bool* aDoing) override;
  NS_IMETHOD GetPrintRange(int32_t* aFromPage, int32_t* aToPage) override;
  NS_IMETHOD DoPageEnd() override;
=======
  nsresult StartPrint(nsPresContext* aPresContext,
                      nsIPrintSettings* aPrintSettings,
                      const nsAString& aDocTitle, const nsAString& aDocURL);
  nsresult PrePrintNextPage(nsITimerCallback* aCallback, bool* aDone);
  nsresult PrintNextPage();
  void ResetPrintCanvasList();
  int32_t GetCurrentPageNum() const { return mPageNum; }
  int32_t GetNumPages() const { return mTotalPages; }
  bool IsDoingPrintRange() const { return mDoingPageRange; }
  void GetPrintRange(int32_t* aFromPage, int32_t* aToPage) const;
  nsresult DoPageEnd();
>>>>>>> upstream-releases:mozilla-release/layout/generic/nsPageSequenceFrame.h

  // We must allow Print Preview UI to have a background, no matter what the
  // user's settings
  bool HonorPrintBackgroundSettings() override { return false; }

  bool HasTransformGetter() const override { return true; }

  /**
   * Return our first page frame.
   */
  void AppendDirectlyOwnedAnonBoxes(nsTArray<OwnedAnonBox>& aResult) override;

#ifdef DEBUG_FRAME_DUMP
  nsresult GetFrameName(nsAString& aResult) const override;
#endif

<<<<<<< HEAD:mozilla-release/layout/generic/nsSimplePageSequenceFrame.h
 protected:
  explicit nsSimplePageSequenceFrame(ComputedStyle* aStyle);
  virtual ~nsSimplePageSequenceFrame();
||||||| merged common ancestors
protected:
  explicit nsSimplePageSequenceFrame(ComputedStyle* aStyle);
  virtual ~nsSimplePageSequenceFrame();
=======
 protected:
  nsPageSequenceFrame(ComputedStyle*, nsPresContext*);
  virtual ~nsPageSequenceFrame();
>>>>>>> upstream-releases:mozilla-release/layout/generic/nsPageSequenceFrame.h

  void SetPageNumberFormat(const char* aPropName, const char* aDefPropVal,
                           bool aPageNumOnly);

  // SharedPageData Helper methods
  void SetDateTimeStr(const nsAString& aDateTimeStr);
  void SetPageNumberFormat(const nsAString& aFormatStr, bool aForPageNumOnly);

  // Sets the frame desired size to the size of the viewport, or the given
  // nscoords, whichever is larger. Print scaling is applied in this function.
  void SetDesiredSize(ReflowOutput& aDesiredSize,
                      const ReflowInput& aReflowInput, nscoord aWidth,
                      nscoord aHeight);

  // Helper function to compute the offset needed to center a child
  // page-frame's margin-box inside our content-box.
  nscoord ComputeCenteringMargin(nscoord aContainerContentBoxWidth,
                                 nscoord aChildPaddingBoxWidth,
                                 const nsMargin& aChildPhysicalMargin);

  void DetermineWhetherToPrintPage();
  nsIFrame* GetCurrentPageFrame();

  nsMargin mMargin;

  nsSize mSize;
  nsSharedPageData* mPageData;  // data shared by all the nsPageFrames

  // Asynch Printing
  int32_t mPageNum;
  int32_t mTotalPages;
  int32_t mPrintRangeType;
  int32_t mFromPageNum;
  int32_t mToPageNum;
  nsTArray<int32_t> mPageRanges;
  nsTArray<RefPtr<mozilla::dom::HTMLCanvasElement> > mCurrentCanvasList;

  // Asynch Printing
  bool mPrintThisPage;
  bool mDoingPageRange;

  bool mCalledBeginPage;

  bool mCurrentCanvasListSetup;
};

<<<<<<< HEAD:mozilla-release/layout/generic/nsSimplePageSequenceFrame.h
#endif /* nsSimplePageSequenceFrame_h___ */
||||||| merged common ancestors
#endif /* nsSimplePageSequenceFrame_h___ */

=======
#endif /* nsPageSequenceFrame_h___ */
>>>>>>> upstream-releases:mozilla-release/layout/generic/nsPageSequenceFrame.h
