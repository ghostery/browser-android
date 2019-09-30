/* -*- Mode: C++; tab-width: 8; indent-tabs-mode: nil; c-basic-offset: 2 -*- */
/* vim: set ts=8 sts=2 et sw=2 tw=80: */
/* This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/. */

/**
 * This frame type is used for input type=date, time, month, week, and
 * datetime-local.
 *
 * NOTE: some of the above-mentioned input types are still to-be-implemented.
 * See nsCSSFrameConstructor::FindInputData, as well as bug 1286182 (date),
 * bug 1306215 (month), bug 1306216 (week) and bug 1306217 (datetime-local).
 */

#ifndef nsDateTimeControlFrame_h__
#define nsDateTimeControlFrame_h__

#include "mozilla/Attributes.h"
#include "nsContainerFrame.h"
#include "nsIAnonymousContentCreator.h"
#include "nsCOMPtr.h"

namespace mozilla {
class PresShell;
namespace dom {
struct DateTimeValue;
}  // namespace dom
}  // namespace mozilla

<<<<<<< HEAD
class nsDateTimeControlFrame final : public nsContainerFrame,
                                     public nsIAnonymousContentCreator {
||||||| merged common ancestors
class nsDateTimeControlFrame final : public nsContainerFrame,
                                     public nsIAnonymousContentCreator
{
=======
class nsDateTimeControlFrame final : public nsContainerFrame {
>>>>>>> upstream-releases
  typedef mozilla::dom::DateTimeValue DateTimeValue;

  explicit nsDateTimeControlFrame(ComputedStyle* aStyle,
                                  nsPresContext* aPresContext);

<<<<<<< HEAD
 public:
  friend nsIFrame* NS_NewDateTimeControlFrame(nsIPresShell* aPresShell,
||||||| merged common ancestors
public:
  friend nsIFrame* NS_NewDateTimeControlFrame(nsIPresShell* aPresShell,
=======
 public:
  friend nsIFrame* NS_NewDateTimeControlFrame(mozilla::PresShell* aPresShell,
>>>>>>> upstream-releases
                                              ComputedStyle* aStyle);

  void ContentStatesChanged(mozilla::EventStates aStates) override;
<<<<<<< HEAD
  void DestroyFrom(nsIFrame* aDestructRoot,
                   PostDestroyData& aPostDestroyData) override;

||||||| merged common ancestors
  void DestroyFrom(nsIFrame* aDestructRoot, PostDestroyData& aPostDestroyData) override;

=======
>>>>>>> upstream-releases
  NS_DECL_QUERYFRAME
  NS_DECL_FRAMEARENA_HELPERS(nsDateTimeControlFrame)

#ifdef DEBUG_FRAME_DUMP
  nsresult GetFrameName(nsAString& aResult) const override {
    return MakeFrameName(NS_LITERAL_STRING("DateTimeControl"), aResult);
  }
#endif

  bool IsFrameOfType(uint32_t aFlags) const override {
    return nsContainerFrame::IsFrameOfType(
        aFlags & ~(nsIFrame::eReplaced | nsIFrame::eReplacedContainsBlock));
  }

  // Reflow
  nscoord GetMinISize(gfxContext* aRenderingContext) override;

  nscoord GetPrefISize(gfxContext* aRenderingContext) override;

  void Reflow(nsPresContext* aPresContext, ReflowOutput& aDesiredSize,
              const ReflowInput& aReflowInput,
              nsReflowStatus& aStatus) override;

<<<<<<< HEAD
  bool IsLeafDynamic() const override;

  // nsIAnonymousContentCreator
  nsresult CreateAnonymousContent(nsTArray<ContentInfo>& aElements) override;
  void AppendAnonymousContentTo(nsTArray<nsIContent*>& aElements,
                                uint32_t aFilter) override;

||||||| merged common ancestors
  // nsIAnonymousContentCreator
  nsresult CreateAnonymousContent(nsTArray<ContentInfo>& aElements) override;
  void AppendAnonymousContentTo(nsTArray<nsIContent*>& aElements,
                                uint32_t aFilter) override;

=======
>>>>>>> upstream-releases
  nsresult AttributeChanged(int32_t aNameSpaceID, nsAtom* aAttribute,
                            int32_t aModType) override;

<<<<<<< HEAD
  nsIContent* GetInputAreaContent();

  void OnValueChanged();
  void OnMinMaxStepAttrChanged();
  void HandleFocusEvent();
  void HandleBlurEvent();

 private:
||||||| merged common ancestors
  void OnValueChanged();
  void OnMinMaxStepAttrChanged();
  void SetValueFromPicker(const DateTimeValue& aValue);
  void HandleFocusEvent();
  void HandleBlurEvent();
  void SetPickerState(bool aOpen);
  bool HasBadInput();

private:
=======
 private:
>>>>>>> upstream-releases
  class SyncDisabledStateEvent;
  friend class SyncDisabledStateEvent;
  class SyncDisabledStateEvent : public mozilla::Runnable {
   public:
    explicit SyncDisabledStateEvent(nsDateTimeControlFrame* aFrame)
        : mozilla::Runnable("nsDateTimeControlFrame::SyncDisabledStateEvent"),
          mFrame(aFrame) {}

    NS_IMETHOD Run() override {
      nsDateTimeControlFrame* frame =
          static_cast<nsDateTimeControlFrame*>(mFrame.GetFrame());
      NS_ENSURE_STATE(frame);

      frame->SyncDisabledState();
      return NS_OK;
    }

   private:
    WeakFrame mFrame;
  };

  /**
   * Sync the disabled state of the datetimebox children up with our content's.
   */
  void SyncDisabledState();
};

#endif  // nsDateTimeControlFrame_h__
