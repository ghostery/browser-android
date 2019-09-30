/* -*- Mode: C++; tab-width: 8; indent-tabs-mode: nil; c-basic-offset: 2 -*- */
/* vim: set ts=8 sts=2 et sw=2 tw=80: */
/* This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/. */

#ifndef mozilla_dom_SVGRect_h
#define mozilla_dom_SVGRect_h

#include "mozilla/dom/SVGElement.h"
#include "mozilla/gfx/Rect.h"

////////////////////////////////////////////////////////////////////////
// SVGRect class

namespace mozilla {
namespace dom {

<<<<<<< HEAD
class SVGRect final : public SVGIRect {
 public:
  explicit SVGRect(nsIContent* aParent, float x = 0.0f, float y = 0.0f,
                   float w = 0.0f, float h = 0.0f);
||||||| merged common ancestors
class SVGRect final : public SVGIRect
{
public:
  explicit SVGRect(nsIContent* aParent, float x=0.0f, float y=0.0f, float w=0.0f,
                   float h=0.0f);
=======
class SVGSVGElement;

class SVGRect final : public nsISupports, public nsWrapperCache {
 public:
  typedef enum { BaseValue, AnimValue, CreatedValue } RectType;
>>>>>>> upstream-releases

  NS_DECL_CYCLE_COLLECTING_ISUPPORTS
  NS_DECL_CYCLE_COLLECTION_SCRIPT_HOLDER_CLASS(SVGRect)

<<<<<<< HEAD
  // WebIDL
  float X() const final { return mX; }

  void SetX(float aX, ErrorResult& aRv) final { mX = aX; }

  float Y() const final { return mY; }

  void SetY(float aY, ErrorResult& aRv) final { mY = aY; }

  float Width() const final { return mWidth; }

  void SetWidth(float aWidth, ErrorResult& aRv) final { mWidth = aWidth; }

  float Height() const final { return mHeight; }

  void SetHeight(float aHeight, ErrorResult& aRv) final { mHeight = aHeight; }

  virtual nsIContent* GetParentObject() const override { return mParent; }

 protected:
  ~SVGRect() {}
||||||| merged common ancestors
  // WebIDL
  float X() const final
  {
    return mX;
  }

  void SetX(float aX, ErrorResult& aRv) final
  {
    mX = aX;
  }

  float Y() const final
  {
    return mY;
  }

  void SetY(float aY, ErrorResult& aRv) final
  {
    mY = aY;
  }

  float Width() const final
  {
    return mWidth;
  }

  void SetWidth(float aWidth, ErrorResult& aRv) final
  {
    mWidth = aWidth;
  }

  float Height() const final
  {
    return mHeight;
  }

  void SetHeight(float aHeight, ErrorResult& aRv) final
  {
    mHeight = aHeight;
  }

  virtual nsIContent* GetParentObject() const override
  {
    return mParent;
  }

protected:
  ~SVGRect() {}
=======
  /**
   * Generic ctor for objects that are created for an attribute.
   */
  SVGRect(SVGAnimatedViewBox* aVal, SVGElement* aSVGElement, RectType aType)
      : mVal(aVal), mParent(aSVGElement), mType(aType) {
    MOZ_ASSERT(mParent);
    MOZ_ASSERT(mType == BaseValue || mType == AnimValue);
  }

  /**
   * Ctor for creating the objects returned by SVGSVGElement.createSVGRect(),
   * which do not initially belong to an attribute.
   */
  explicit SVGRect(SVGSVGElement* aSVGElement);

  /**
   * Ctor for all other non-attribute usage i.e getBBox, getExtentOfChar etc.
   */
  SVGRect(nsIContent* aParent, const gfx::Rect& aRect)
      : mVal(nullptr), mRect(aRect), mParent(aParent), mType(CreatedValue) {
    MOZ_ASSERT(mParent);
  }

  JSObject* WrapObject(JSContext* aCx,
                       JS::Handle<JSObject*> aGivenProto) override;

  float X();
  float Y();
  float Width();
  float Height();

  void SetX(float aX, mozilla::ErrorResult& aRv);
  void SetY(float aY, mozilla::ErrorResult& aRv);
  void SetWidth(float aWidth, mozilla::ErrorResult& aRv);
  void SetHeight(float aHeight, mozilla::ErrorResult& aRv);

  nsIContent* GetParentObject() const {
    MOZ_ASSERT(mParent);
    return mParent;
  }

 private:
  virtual ~SVGRect();
>>>>>>> upstream-releases

<<<<<<< HEAD
  nsCOMPtr<nsIContent> mParent;
  float mX, mY, mWidth, mHeight;
};

}  // namespace dom
}  // namespace mozilla
||||||| merged common ancestors
  nsCOMPtr<nsIContent> mParent;
  float mX, mY, mWidth, mHeight;
};

} // namespace dom
} // namespace mozilla
=======
  // If we're actually representing a viewBox rect then our value
  // will come from that element's viewBox attribute's value.
  SVGAnimatedViewBox* mVal;  // kept alive because it belongs to content
  gfx::Rect mRect;
>>>>>>> upstream-releases

<<<<<<< HEAD
already_AddRefed<mozilla::dom::SVGRect> NS_NewSVGRect(nsIContent* aParent,
                                                      float x = 0.0f,
                                                      float y = 0.0f,
                                                      float width = 0.0f,
                                                      float height = 0.0f);
||||||| merged common ancestors
already_AddRefed<mozilla::dom::SVGRect>
NS_NewSVGRect(nsIContent* aParent, float x=0.0f, float y=0.0f,
              float width=0.0f, float height=0.0f);
=======
  // If mType is AnimValue or BaseValue this will be an element that
  // has a viewBox, otherwise it could be any nsIContent.
  RefPtr<nsIContent> mParent;
  const RectType mType;
};
>>>>>>> upstream-releases

<<<<<<< HEAD
already_AddRefed<mozilla::dom::SVGRect> NS_NewSVGRect(
    nsIContent* aParent, const mozilla::gfx::Rect& rect);
||||||| merged common ancestors
already_AddRefed<mozilla::dom::SVGRect>
NS_NewSVGRect(nsIContent* aParent, const mozilla::gfx::Rect& rect);
=======
}  // namespace dom
}  // namespace mozilla
>>>>>>> upstream-releases

#endif  // mozilla_dom_SVGRect_h
