/* -*- Mode: C++; tab-width: 8; indent-tabs-mode: nil; c-basic-offset: 2 -*- */
/* vim: set ts=8 sts=2 et sw=2 tw=80: */
/* This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/. */

#ifndef mozilla_dom_HTMLVideoElement_h
#define mozilla_dom_HTMLVideoElement_h

#include "mozilla/Attributes.h"
#include "mozilla/dom/HTMLMediaElement.h"
#include "mozilla/StaticPrefs.h"

namespace mozilla {

class FrameStatistics;

namespace dom {

class WakeLock;
class VideoPlaybackQuality;

<<<<<<< HEAD
class HTMLVideoElement final : public HTMLMediaElement {
 public:
||||||| merged common ancestors
class HTMLVideoElement final : public HTMLMediaElement
{
public:
=======
class HTMLVideoElement final : public HTMLMediaElement {
 public:
  NS_DECL_ISUPPORTS_INHERITED
  NS_DECL_CYCLE_COLLECTION_CLASS_INHERITED(HTMLVideoElement, HTMLMediaElement)

>>>>>>> upstream-releases
  typedef mozilla::dom::NodeInfo NodeInfo;

  explicit HTMLVideoElement(already_AddRefed<NodeInfo>&& aNodeInfo);

  NS_IMPL_FROMNODE_HTML_WITH_TAG(HTMLVideoElement, video)

  using HTMLMediaElement::GetPaused;

<<<<<<< HEAD
  virtual bool IsVideo() const override { return true; }
||||||| merged common ancestors
  virtual bool IsVideo() const override {
    return true;
  }
=======
  void Invalidate(bool aImageSizeChanged, Maybe<nsIntSize>& aNewIntrinsicSize,
                  bool aForceInvalidate) override;
>>>>>>> upstream-releases

<<<<<<< HEAD
  virtual bool ParseAttribute(int32_t aNamespaceID, nsAtom* aAttribute,
||||||| merged common ancestors
  virtual bool ParseAttribute(int32_t aNamespaceID,
                              nsAtom* aAttribute,
=======
  virtual bool IsVideo() const override { return true; }

  virtual bool ParseAttribute(int32_t aNamespaceID, nsAtom* aAttribute,
>>>>>>> upstream-releases
                              const nsAString& aValue,
                              nsIPrincipal* aMaybeScriptedPrincipal,
                              nsAttrValue& aResult) override;
  NS_IMETHOD_(bool) IsAttributeMapped(const nsAtom* aAttribute) const override;

  static void InitStatics();

  virtual nsMapRuleToAttributesFunc GetAttributeMappingFunction()
      const override;

  virtual nsresult Clone(NodeInfo*, nsINode** aResult) const override;

  virtual void UnbindFromTree(bool aNullParent = true) override;

  // Set size with the current video frame's height and width.
  // If there is no video frame, returns NS_ERROR_FAILURE.
  nsresult GetVideoSize(nsIntSize* size);

  virtual void UpdateMediaSize(const nsIntSize& aSize) override;

  virtual nsresult SetAcceptHeader(nsIHttpChannel* aChannel) override;

  // Element
  virtual bool IsInteractiveHTMLContent(bool aIgnoreTabindex) const override;

  // WebIDL

<<<<<<< HEAD
  uint32_t Width() const { return GetIntAttr(nsGkAtoms::width, 0); }
||||||| merged common ancestors
  uint32_t Width() const
  {
    return GetIntAttr(nsGkAtoms::width, 0);
  }
=======
  uint32_t Width() const {
    return GetDimensionAttrAsUnsignedInt(nsGkAtoms::width, 0);
  }
>>>>>>> upstream-releases

  void SetWidth(uint32_t aValue, ErrorResult& aRv) {
    SetUnsignedIntAttr(nsGkAtoms::width, aValue, 0, aRv);
  }

<<<<<<< HEAD
  uint32_t Height() const { return GetIntAttr(nsGkAtoms::height, 0); }
||||||| merged common ancestors
  uint32_t Height() const
  {
    return GetIntAttr(nsGkAtoms::height, 0);
  }
=======
  uint32_t Height() const {
    return GetDimensionAttrAsUnsignedInt(nsGkAtoms::height, 0);
  }
>>>>>>> upstream-releases

  void SetHeight(uint32_t aValue, ErrorResult& aRv) {
    SetUnsignedIntAttr(nsGkAtoms::height, aValue, 0, aRv);
  }

  uint32_t VideoWidth() const {
    if (mMediaInfo.HasVideo()) {
      if (mMediaInfo.mVideo.mRotation == VideoInfo::Rotation::kDegree_90 ||
          mMediaInfo.mVideo.mRotation == VideoInfo::Rotation::kDegree_270) {
        return mMediaInfo.mVideo.mDisplay.height;
      }
      return mMediaInfo.mVideo.mDisplay.width;
    }
    return 0;
  }

  uint32_t VideoHeight() const {
    if (mMediaInfo.HasVideo()) {
      if (mMediaInfo.mVideo.mRotation == VideoInfo::Rotation::kDegree_90 ||
          mMediaInfo.mVideo.mRotation == VideoInfo::Rotation::kDegree_270) {
        return mMediaInfo.mVideo.mDisplay.width;
      }
      return mMediaInfo.mVideo.mDisplay.height;
    }
    return 0;
  }

  VideoInfo::Rotation RotationDegrees() const {
    return mMediaInfo.mVideo.mRotation;
  }

<<<<<<< HEAD
  void GetPoster(nsAString& aValue) {
||||||| merged common ancestors
  void GetPoster(nsAString& aValue)
  {
=======
  bool HasAlpha() const { return mMediaInfo.mVideo.HasAlpha(); }

  void GetPoster(nsAString& aValue) {
>>>>>>> upstream-releases
    GetURIAttr(nsGkAtoms::poster, nullptr, aValue);
  }
  void SetPoster(const nsAString& aValue, ErrorResult& aRv) {
    SetHTMLAttr(nsGkAtoms::poster, aValue, aRv);
  }

  uint32_t MozParsedFrames() const;

  uint32_t MozDecodedFrames() const;

  uint32_t MozPresentedFrames() const;

  uint32_t MozPaintedFrames();

  double MozFrameDelay();

  bool MozHasAudio() const;

  // Gives access to the decoder's frame statistics, if present.
  FrameStatistics* GetFrameStatistics();

  already_AddRefed<VideoPlaybackQuality> GetVideoPlaybackQuality();

<<<<<<< HEAD
  bool MozOrientationLockEnabled() const {
    return StaticPrefs::MediaVideocontrolsLockVideoOrientation();
||||||| merged common ancestors

  bool MozOrientationLockEnabled() const
  {
    return StaticPrefs::MediaVideocontrolsLockVideoOrientation();
=======
  bool MozOrientationLockEnabled() const {
    return StaticPrefs::media_videocontrols_lock_video_orientation();
>>>>>>> upstream-releases
  }

  bool MozIsOrientationLocked() const { return mIsOrientationLocked; }

<<<<<<< HEAD
  void SetMozIsOrientationLocked(bool aLock) { mIsOrientationLocked = aLock; }
||||||| merged common ancestors
  void SetMozIsOrientationLocked(bool aLock)
  {
    mIsOrientationLocked = aLock;
  }
=======
  void SetMozIsOrientationLocked(bool aLock) { mIsOrientationLocked = aLock; }

  void CloneElementVisually(HTMLVideoElement& aTarget, ErrorResult& rv);

  void StopCloningElementVisually();

  bool IsCloningElementVisually() const { return !!mVisualCloneTarget; }
>>>>>>> upstream-releases

<<<<<<< HEAD
 protected:
||||||| merged common ancestors
protected:
=======
  void TogglePictureInPicture(ErrorResult& rv);

 protected:
>>>>>>> upstream-releases
  virtual ~HTMLVideoElement();

  virtual JSObject* WrapNode(JSContext* aCx,
                             JS::Handle<JSObject*> aGivenProto) override;

  /**
   * We create video wakelock when the video is playing and release it when
   * video pauses. Note, the actual platform wakelock will automatically be
   * released when the page is in the background, so we don't need to check the
   * video's visibility by ourselves.
   */
  void WakeLockRelease() override;
  void UpdateWakeLock() override;

  bool ShouldCreateVideoWakeLock() const;
  void CreateVideoWakeLockIfNeeded();
  void ReleaseVideoWakeLockIfExists();

  RefPtr<WakeLock> mScreenWakeLock;

  bool mIsOrientationLocked;

<<<<<<< HEAD
 private:
||||||| merged common ancestors
private:
=======
 private:
  bool SetVisualCloneTarget(HTMLVideoElement* aCloneTarget);
  bool SetVisualCloneSource(HTMLVideoElement* aCloneSource);

  // For video elements, we can clone the frames being played to
  // a secondary video element. If we're doing that, we hold a
  // reference to the video element we're cloning to in
  // mVisualCloneSource.
  //
  // Please don't set this to non-nullptr values directly - use
  // SetVisualCloneTarget() instead.
  RefPtr<HTMLVideoElement> mVisualCloneTarget;
  // If this video is the clone target of another video element,
  // then mVisualCloneSource points to that originating video
  // element.
  //
  // Please don't set this to non-nullptr values directly - use
  // SetVisualCloneTarget() instead.
  RefPtr<HTMLVideoElement> mVisualCloneSource;

>>>>>>> upstream-releases
  static void MapAttributesIntoRule(const nsMappedAttributes* aAttributes,
                                    MappedDeclarations&);

  static bool IsVideoStatsEnabled();
  double TotalPlayTime() const;

  virtual void MaybeBeginCloningVisually() override;
  void EndCloningVisually();
};

}  // namespace dom
}  // namespace mozilla

#endif  // mozilla_dom_HTMLVideoElement_h
