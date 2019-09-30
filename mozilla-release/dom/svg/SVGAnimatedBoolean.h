/* -*- Mode: C++; tab-width: 8; indent-tabs-mode: nil; c-basic-offset: 2 -*- */
/* vim: set ts=8 sts=2 et sw=2 tw=80: */
/* This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/. */

#ifndef __NS_SVGBOOLEAN_H__
#define __NS_SVGBOOLEAN_H__

#include "nsError.h"
#include "mozilla/SMILAttr.h"
#include "mozilla/AlreadyAddRefed.h"
#include "mozilla/Attributes.h"
#include "mozilla/UniquePtr.h"

class nsAtom;

namespace mozilla {

class SMILValue;

namespace dom {
class DOMSVGAnimatedBoolean;
class SVGAnimationElement;
class SVGElement;
}  // namespace dom

<<<<<<< HEAD
class SVGAnimatedBoolean final : public nsWrapperCache {
  NS_INLINE_DECL_CYCLE_COLLECTING_NATIVE_REFCOUNTING(SVGAnimatedBoolean)
  NS_DECL_CYCLE_COLLECTION_SCRIPT_HOLDER_NATIVE_CLASS(SVGAnimatedBoolean)
||||||| merged common ancestors
class SVGAnimatedBoolean final : public nsWrapperCache
{
  NS_INLINE_DECL_CYCLE_COLLECTING_NATIVE_REFCOUNTING(SVGAnimatedBoolean)
  NS_DECL_CYCLE_COLLECTION_SCRIPT_HOLDER_NATIVE_CLASS(SVGAnimatedBoolean)
=======
class SVGAnimatedBoolean {
 public:
  typedef mozilla::dom::SVGElement SVGElement;
>>>>>>> upstream-releases

<<<<<<< HEAD
  SVGAnimatedBoolean(nsSVGBoolean* aVal, nsSVGElement* aSVGElement)
      : mVal(aVal), mSVGElement(aSVGElement) {}
||||||| merged common ancestors
  SVGAnimatedBoolean(nsSVGBoolean* aVal, nsSVGElement *aSVGElement)
    : mVal(aVal), mSVGElement(aSVGElement)
  {
  }
=======
  void Init(uint8_t aAttrEnum = 0xff, bool aValue = false) {
    mAnimVal = mBaseVal = aValue;
    mAttrEnum = aAttrEnum;
    mIsAnimated = false;
  }
>>>>>>> upstream-releases

<<<<<<< HEAD
  // WebIDL
  nsSVGElement* GetParentObject() const { return mSVGElement; }
  virtual JSObject* WrapObject(JSContext* aCx,
                               JS::Handle<JSObject*> aGivenProto) override;
  bool BaseVal() const { return mVal->GetBaseValue(); }
  void SetBaseVal(bool aValue) { mVal->SetBaseValue(aValue, mSVGElement); }
  bool AnimVal() const {
    mSVGElement->FlushAnimations();
    return mVal->GetAnimValue();
  }
||||||| merged common ancestors
  // WebIDL
  nsSVGElement* GetParentObject() const { return mSVGElement; }
  virtual JSObject* WrapObject(JSContext* aCx, JS::Handle<JSObject*> aGivenProto) override;
  bool BaseVal() const { return mVal->GetBaseValue(); }
  void SetBaseVal(bool aValue) { mVal->SetBaseValue(aValue, mSVGElement); }
  bool AnimVal() const { mSVGElement->FlushAnimations(); return mVal->GetAnimValue(); }
=======
  nsresult SetBaseValueAtom(const nsAtom* aValue, SVGElement* aSVGElement);
  nsAtom* GetBaseValueAtom() const;

  void SetBaseValue(bool aValue, SVGElement* aSVGElement);
  bool GetBaseValue() const { return mBaseVal; }

  void SetAnimValue(bool aValue, SVGElement* aSVGElement);
  bool GetAnimValue() const { return mAnimVal; }

  already_AddRefed<mozilla::dom::DOMSVGAnimatedBoolean> ToDOMAnimatedBoolean(
      SVGElement* aSVGElement);
  mozilla::UniquePtr<SMILAttr> ToSMILAttr(SVGElement* aSVGElement);

 private:
  bool mAnimVal;
  bool mBaseVal;
  bool mIsAnimated;
  uint8_t mAttrEnum;  // element specified tracking for attribute

 public:
  struct SMILBool : public SMILAttr {
   public:
    SMILBool(SVGAnimatedBoolean* aVal, SVGElement* aSVGElement)
        : mVal(aVal), mSVGElement(aSVGElement) {}
>>>>>>> upstream-releases

<<<<<<< HEAD
 protected:
  ~SVGAnimatedBoolean();
||||||| merged common ancestors
protected:
  ~SVGAnimatedBoolean();
=======
    // These will stay alive because a SMILAttr only lives as long
    // as the Compositing step, and DOM elements don't get a chance to
    // die during that.
    SVGAnimatedBoolean* mVal;
    SVGElement* mSVGElement;
>>>>>>> upstream-releases

<<<<<<< HEAD
  nsSVGBoolean* mVal;  // kept alive because it belongs to content
  RefPtr<nsSVGElement> mSVGElement;
||||||| merged common ancestors
  nsSVGBoolean* mVal; // kept alive because it belongs to content
  RefPtr<nsSVGElement> mSVGElement;
=======
    // SMILAttr methods
    virtual nsresult ValueFromString(
        const nsAString& aStr,
        const mozilla::dom::SVGAnimationElement* aSrcElement, SMILValue& aValue,
        bool& aPreventCachingOfSandwich) const override;
    virtual SMILValue GetBaseValue() const override;
    virtual void ClearAnimValue() override;
    virtual nsresult SetAnimValue(const SMILValue& aValue) override;
  };
>>>>>>> upstream-releases
};

<<<<<<< HEAD
}  // namespace dom
}  // namespace mozilla
||||||| merged common ancestors
} //namespace dom
} //namespace mozilla
=======
}  // namespace mozilla
>>>>>>> upstream-releases

<<<<<<< HEAD
#endif  // mozilla_dom_SVGAnimatedBoolean_h
||||||| merged common ancestors
#endif // mozilla_dom_SVGAnimatedBoolean_h
=======
#endif  //__NS_SVGBOOLEAN_H__
>>>>>>> upstream-releases
