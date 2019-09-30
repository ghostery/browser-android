/* -*- Mode: C++; tab-width: 8; indent-tabs-mode: nil; c-basic-offset: 2 -*- */
/* vim: set ts=8 sts=2 et sw=2 tw=80: */
/* This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/. */

#ifndef __NS_SVGENUM_H__
#define __NS_SVGENUM_H__

#include "DOMSVGAnimatedEnumeration.h"
#include "nsCycleCollectionParticipant.h"
#include "nsError.h"
#include "mozilla/Attributes.h"
#include "mozilla/SMILAttr.h"
#include "mozilla/UniquePtr.h"
#include "mozilla/dom/SVGElement.h"

class nsAtom;

namespace mozilla {

class SMILValue;

namespace dom {
class SVGAnimationElement;
}  // namespace dom

<<<<<<< HEAD
class SVGAnimatedEnumeration : public nsISupports, public nsWrapperCache {
 public:
  NS_DECL_CYCLE_COLLECTING_ISUPPORTS
  NS_DECL_CYCLE_COLLECTION_SCRIPT_HOLDER_CLASS(SVGAnimatedEnumeration)
||||||| merged common ancestors
class SVGAnimatedEnumeration : public nsISupports
                             , public nsWrapperCache
{
public:
  NS_DECL_CYCLE_COLLECTING_ISUPPORTS
  NS_DECL_CYCLE_COLLECTION_SCRIPT_HOLDER_CLASS(SVGAnimatedEnumeration)
=======
typedef uint8_t SVGEnumValue;

struct SVGEnumMapping {
  nsStaticAtom* const mKey;
  const SVGEnumValue mVal;
};
>>>>>>> upstream-releases

<<<<<<< HEAD
  nsSVGElement* GetParentObject() const { return mSVGElement; }
||||||| merged common ancestors
  nsSVGElement* GetParentObject() const
  {
    return mSVGElement;
  }
=======
class SVGAnimatedEnumeration {
 public:
  typedef mozilla::dom::SVGElement SVGElement;

  void Init(uint8_t aAttrEnum, uint16_t aValue) {
    mAnimVal = mBaseVal = uint8_t(aValue);
    mAttrEnum = aAttrEnum;
    mIsAnimated = false;
    mIsBaseSet = false;
  }
>>>>>>> upstream-releases

<<<<<<< HEAD
  JSObject* WrapObject(JSContext* aCx, JS::Handle<JSObject*> aGivenProto) final;
||||||| merged common ancestors
  JSObject* WrapObject(JSContext* aCx, JS::Handle<JSObject*> aGivenProto)
    final;
=======
  nsresult SetBaseValueAtom(const nsAtom* aValue, SVGElement* aSVGElement);
  nsAtom* GetBaseValueAtom(SVGElement* aSVGElement);
  nsresult SetBaseValue(uint16_t aValue, SVGElement* aSVGElement);
  uint16_t GetBaseValue() const { return mBaseVal; }
>>>>>>> upstream-releases

  void SetAnimValue(uint16_t aValue, SVGElement* aSVGElement);
  uint16_t GetAnimValue() const { return mAnimVal; }
  bool IsExplicitlySet() const { return mIsAnimated || mIsBaseSet; }

<<<<<<< HEAD
 protected:
  explicit SVGAnimatedEnumeration(nsSVGElement* aSVGElement)
      : mSVGElement(aSVGElement) {}
  virtual ~SVGAnimatedEnumeration(){};
||||||| merged common ancestors
protected:
  explicit SVGAnimatedEnumeration(nsSVGElement* aSVGElement)
    : mSVGElement(aSVGElement)
  {
  }
  virtual ~SVGAnimatedEnumeration() {};
=======
  already_AddRefed<mozilla::dom::DOMSVGAnimatedEnumeration> ToDOMAnimatedEnum(
      SVGElement* aSVGElement);

  mozilla::UniquePtr<SMILAttr> ToSMILAttr(SVGElement* aSVGElement);

 private:
  SVGEnumValue mAnimVal;
  SVGEnumValue mBaseVal;
  uint8_t mAttrEnum;  // element specified tracking for attribute
  bool mIsAnimated;
  bool mIsBaseSet;

  const SVGEnumMapping* GetMapping(SVGElement* aSVGElement);

 public:
  // DOM wrapper class for the (DOM)SVGAnimatedEnumeration interface where the
  // wrapped class is SVGAnimatedEnumeration.
  struct DOMAnimatedEnum final
      : public mozilla::dom::DOMSVGAnimatedEnumeration {
    DOMAnimatedEnum(SVGAnimatedEnumeration* aVal, SVGElement* aSVGElement)
        : mozilla::dom::DOMSVGAnimatedEnumeration(aSVGElement), mVal(aVal) {}
    virtual ~DOMAnimatedEnum();

    SVGAnimatedEnumeration* mVal;  // kept alive because it belongs to content

    using mozilla::dom::DOMSVGAnimatedEnumeration::SetBaseVal;
    virtual uint16_t BaseVal() override { return mVal->GetBaseValue(); }
    virtual void SetBaseVal(uint16_t aBaseVal,
                            mozilla::ErrorResult& aRv) override {
      aRv = mVal->SetBaseValue(aBaseVal, mSVGElement);
    }
    virtual uint16_t AnimVal() override {
      // Script may have modified animation parameters or timeline -- DOM
      // getters need to flush any resample requests to reflect these
      // modifications.
      mSVGElement->FlushAnimations();
      return mVal->GetAnimValue();
    }
  };

  struct SMILEnum : public SMILAttr {
   public:
    SMILEnum(SVGAnimatedEnumeration* aVal, SVGElement* aSVGElement)
        : mVal(aVal), mSVGElement(aSVGElement) {}

    // These will stay alive because a SMILAttr only lives as long
    // as the Compositing step, and DOM elements don't get a chance to
    // die during that.
    SVGAnimatedEnumeration* mVal;
    SVGElement* mSVGElement;
>>>>>>> upstream-releases

    // SMILAttr methods
    virtual nsresult ValueFromString(
        const nsAString& aStr,
        const mozilla::dom::SVGAnimationElement* aSrcElement, SMILValue& aValue,
        bool& aPreventCachingOfSandwich) const override;
    virtual SMILValue GetBaseValue() const override;
    virtual void ClearAnimValue() override;
    virtual nsresult SetAnimValue(const SMILValue& aValue) override;
  };
};

<<<<<<< HEAD
}  // namespace dom
}  // namespace mozilla
||||||| merged common ancestors
} // namespace dom
} // namespace mozilla
=======
}  // namespace mozilla
>>>>>>> upstream-releases

<<<<<<< HEAD
#endif  // mozilla_dom_SVGAnimatedEnumeration_h
||||||| merged common ancestors
#endif // mozilla_dom_SVGAnimatedEnumeration_h
=======
#endif  //__NS_SVGENUM_H__
>>>>>>> upstream-releases
