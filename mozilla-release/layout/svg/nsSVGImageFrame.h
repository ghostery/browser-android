/* -*- Mode: C++; tab-width: 8; indent-tabs-mode: nil; c-basic-offset: 2 -*- */
/* vim: set ts=8 sts=2 et sw=2 tw=80: */
/* This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/. */

#ifndef __NS_SVGIMAGEFRAME_H__
#define __NS_SVGIMAGEFRAME_H__

// Keep in (case-insensitive) order:
#include "gfxContext.h"
#include "gfxPlatform.h"
#include "mozilla/gfx/2D.h"
#include "imgIContainer.h"
#include "nsContainerFrame.h"
#include "nsIImageLoadingContent.h"
#include "nsLayoutUtils.h"
#include "imgINotificationObserver.h"
#include "SVGObserverUtils.h"
#include "nsSVGUtils.h"
#include "SVGContentUtils.h"
#include "SVGGeometryFrame.h"
#include "SVGImageContext.h"
#include "mozilla/dom/SVGImageElement.h"
#include "nsIReflowCallback.h"
#include "mozilla/Unused.h"

class nsSVGImageFrame;

<<<<<<< HEAD
class nsSVGImageListener final : public imgINotificationObserver {
 public:
  explicit nsSVGImageListener(nsSVGImageFrame* aFrame);
||||||| merged common ancestors
class nsSVGImageListener final : public imgINotificationObserver
{
public:
  explicit nsSVGImageListener(nsSVGImageFrame *aFrame);
=======
namespace mozilla {
class PresShell;
}  // namespace mozilla

class nsSVGImageListener final : public imgINotificationObserver {
 public:
  explicit nsSVGImageListener(nsSVGImageFrame* aFrame);
>>>>>>> upstream-releases

  NS_DECL_ISUPPORTS
  NS_DECL_IMGINOTIFICATIONOBSERVER

  void SetFrame(nsSVGImageFrame* frame) { mFrame = frame; }

<<<<<<< HEAD
 private:
  ~nsSVGImageListener() {}
||||||| merged common ancestors
private:
  ~nsSVGImageListener() {}
=======
 private:
  ~nsSVGImageListener() = default;
>>>>>>> upstream-releases

  nsSVGImageFrame* mFrame;
};

<<<<<<< HEAD
class nsSVGImageFrame final : public mozilla::SVGGeometryFrame,
                              public nsIReflowCallback {
  friend nsIFrame* NS_NewSVGImageFrame(nsIPresShell* aPresShell,
                                       ComputedStyle* aStyle);

 protected:
  explicit nsSVGImageFrame(ComputedStyle* aStyle)
      : SVGGeometryFrame(aStyle, kClassID),
        mReflowCallbackPosted(false),
        mForceSyncDecoding(false) {
||||||| merged common ancestors
class nsSVGImageFrame final
  : public mozilla::SVGGeometryFrame
  , public nsIReflowCallback
{
  friend nsIFrame*
  NS_NewSVGImageFrame(nsIPresShell* aPresShell, ComputedStyle* aStyle);

protected:
  explicit nsSVGImageFrame(ComputedStyle* aStyle)
    : SVGGeometryFrame(aStyle, kClassID)
    , mReflowCallbackPosted(false)
    , mForceSyncDecoding(false)
  {
=======
class nsSVGImageFrame final : public mozilla::SVGGeometryFrame,
                              public nsIReflowCallback {
  friend nsIFrame* NS_NewSVGImageFrame(mozilla::PresShell* aPresShell,
                                       ComputedStyle* aStyle);

 protected:
  explicit nsSVGImageFrame(ComputedStyle* aStyle, nsPresContext* aPresContext)
      : SVGGeometryFrame(aStyle, aPresContext, kClassID),
        mReflowCallbackPosted(false),
        mForceSyncDecoding(false) {
>>>>>>> upstream-releases
    EnableVisibilityTracking();
  }

  virtual ~nsSVGImageFrame();

 public:
  NS_DECL_QUERYFRAME
  NS_DECL_FRAMEARENA_HELPERS(nsSVGImageFrame)

  // nsSVGDisplayableFrame interface:
  virtual void PaintSVG(gfxContext& aContext, const gfxMatrix& aTransform,
                        imgDrawingParams& aImgParams,
                        const nsIntRect* aDirtyRect = nullptr) override;
  virtual nsIFrame* GetFrameForPoint(const gfxPoint& aPoint) override;
  virtual void ReflowSVG() override;

  // SVGGeometryFrame methods:
  virtual uint16_t GetHitTestFlags() override;

  // nsIFrame interface:
<<<<<<< HEAD
  virtual nsresult AttributeChanged(int32_t aNameSpaceID, nsAtom* aAttribute,
                                    int32_t aModType) override;
||||||| merged common ancestors
  virtual nsresult  AttributeChanged(int32_t         aNameSpaceID,
                                     nsAtom*        aAttribute,
                                     int32_t         aModType) override;
=======
  virtual nsresult AttributeChanged(int32_t aNameSpaceID, nsAtom* aAttribute,
                                    int32_t aModType) override;

  void OnVisibilityChange(
      Visibility aNewVisibility,
      const Maybe<OnNonvisible>& aNonvisibleAction = Nothing()) override;
>>>>>>> upstream-releases

<<<<<<< HEAD
  void OnVisibilityChange(
      Visibility aNewVisibility,
      const Maybe<OnNonvisible>& aNonvisibleAction = Nothing()) override;
||||||| merged common ancestors
  void OnVisibilityChange(Visibility aNewVisibility,
                          const Maybe<OnNonvisible>& aNonvisibleAction = Nothing()) override;
=======
  virtual void Init(nsIContent* aContent, nsContainerFrame* aParent,
                    nsIFrame* aPrevInFlow) override;
  virtual void DestroyFrom(nsIFrame* aDestructRoot,
                           PostDestroyData& aPostDestroyData) override;
>>>>>>> upstream-releases

<<<<<<< HEAD
  virtual void Init(nsIContent* aContent, nsContainerFrame* aParent,
                    nsIFrame* aPrevInFlow) override;
  virtual void DestroyFrom(nsIFrame* aDestructRoot,
                           PostDestroyData& aPostDestroyData) override;
||||||| merged common ancestors
  virtual void Init(nsIContent*       aContent,
                    nsContainerFrame* aParent,
                    nsIFrame*         aPrevInFlow) override;
  virtual void DestroyFrom(nsIFrame* aDestructRoot, PostDestroyData& aPostDestroyData) override;
=======
  bool GetIntrinsicImageDimensions(mozilla::gfx::Size& aSize,
                                   mozilla::AspectRatio& aAspectRatio) const;
>>>>>>> upstream-releases

#ifdef DEBUG_FRAME_DUMP
  virtual nsresult GetFrameName(nsAString& aResult) const override {
    return MakeFrameName(NS_LITERAL_STRING("SVGImage"), aResult);
  }
#endif

  // nsIReflowCallback
  virtual bool ReflowFinished() override;
  virtual void ReflowCallbackCanceled() override;

  /// Always sync decode our image when painting if @aForce is true.
  void SetForceSyncDecoding(bool aForce) { mForceSyncDecoding = aForce; }

 private:
  mozilla::gfx::Matrix GetRasterImageTransform(int32_t aNativeWidth,
                                               int32_t aNativeHeight);
  mozilla::gfx::Matrix GetVectorImageTransform();
  bool TransformContextForPainting(gfxContext* aGfxContext,
                                   const gfxMatrix& aTransform);

  nsCOMPtr<imgINotificationObserver> mListener;

  nsCOMPtr<imgIContainer> mImageContainer;

  bool mReflowCallbackPosted;
  bool mForceSyncDecoding;

  friend class nsSVGImageListener;
};

#endif  // __NS_SVGIMAGEFRAME_H__
