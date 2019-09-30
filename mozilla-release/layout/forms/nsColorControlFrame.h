/* -*- Mode: C++; tab-width: 8; indent-tabs-mode: nil; c-basic-offset: 2 -*- */
/* vim: set ts=8 sts=2 et sw=2 tw=80: */
/* This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/. */

#ifndef nsColorControlFrame_h___
#define nsColorControlFrame_h___

#include "nsCOMPtr.h"
#include "nsHTMLButtonControlFrame.h"
#include "nsIAnonymousContentCreator.h"

namespace mozilla {
<<<<<<< HEAD
enum class CSSPseudoElementType : uint8_t;
}  // namespace mozilla
||||||| merged common ancestors
enum class CSSPseudoElementType : uint8_t;
} // namespace mozilla
=======
enum class PseudoStyleType : uint8_t;
class PresShell;
}  // namespace mozilla
>>>>>>> upstream-releases

// Class which implements the input type=color

class nsColorControlFrame final : public nsHTMLButtonControlFrame,
<<<<<<< HEAD
                                  public nsIAnonymousContentCreator {
  typedef mozilla::CSSPseudoElementType CSSPseudoElementType;
||||||| merged common ancestors
                                  public nsIAnonymousContentCreator
{
  typedef mozilla::CSSPseudoElementType CSSPseudoElementType;
=======
                                  public nsIAnonymousContentCreator {
  typedef mozilla::PseudoStyleType PseudoStyleType;
>>>>>>> upstream-releases
  typedef mozilla::dom::Element Element;

<<<<<<< HEAD
 public:
  friend nsIFrame* NS_NewColorControlFrame(nsIPresShell* aPresShell,
||||||| merged common ancestors
public:
  friend nsIFrame* NS_NewColorControlFrame(nsIPresShell* aPresShell,
=======
 public:
  friend nsIFrame* NS_NewColorControlFrame(mozilla::PresShell* aPresShell,
>>>>>>> upstream-releases
                                           ComputedStyle* aStyle);

  virtual void DestroyFrom(nsIFrame* aDestructRoot,
                           PostDestroyData& aPostDestroyData) override;

  NS_DECL_QUERYFRAME
  NS_DECL_FRAMEARENA_HELPERS(nsColorControlFrame)

#ifdef DEBUG_FRAME_DUMP
  virtual nsresult GetFrameName(nsAString& aResult) const override;
#endif

  // nsIAnonymousContentCreator
  virtual nsresult CreateAnonymousContent(
      nsTArray<ContentInfo>& aElements) override;
  virtual void AppendAnonymousContentTo(nsTArray<nsIContent*>& aElements,
                                        uint32_t aFilter) override;

  // nsIFrame
  virtual nsresult AttributeChanged(int32_t aNameSpaceID, nsAtom* aAttribute,
                                    int32_t aModType) override;
  virtual nsContainerFrame* GetContentInsertionFrame() override;

  // Refresh the color swatch, using associated input's value
  nsresult UpdateColor();

<<<<<<< HEAD
 private:
  explicit nsColorControlFrame(ComputedStyle* aStyle);
||||||| merged common ancestors
private:
  explicit nsColorControlFrame(ComputedStyle* aStyle);
=======
 private:
  explicit nsColorControlFrame(ComputedStyle* aStyle,
                               nsPresContext* aPresContext);
>>>>>>> upstream-releases

  nsCOMPtr<Element> mColorContent;
};

#endif  // nsColorControlFrame_h___
