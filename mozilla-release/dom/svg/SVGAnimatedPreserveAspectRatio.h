/* -*- Mode: C++; tab-width: 8; indent-tabs-mode: nil; c-basic-offset: 2 -*- */
/* vim: set ts=8 sts=2 et sw=2 tw=80: */
/* This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/. */

#ifndef MOZILLA_SVGANIMATEDPRESERVEASPECTRATIO_H__
#define MOZILLA_SVGANIMATEDPRESERVEASPECTRATIO_H__

#include "nsCycleCollectionParticipant.h"
#include "nsError.h"
#include "SVGPreserveAspectRatio.h"
#include "mozilla/Attributes.h"
#include "mozilla/SMILAttr.h"
#include "mozilla/UniquePtr.h"
#include "mozilla/dom/SVGElement.h"

namespace mozilla {

class SMILValue;

namespace dom {
class DOMSVGAnimatedPreserveAspectRatio;
class SVGAnimationElement;
}  // namespace dom

class SVGAnimatedPreserveAspectRatio final {
 public:
  void Init() {
    mBaseVal.mAlign =
        dom::SVGPreserveAspectRatio_Binding::SVG_PRESERVEASPECTRATIO_XMIDYMID;
    mBaseVal.mMeetOrSlice =
        dom::SVGPreserveAspectRatio_Binding::SVG_MEETORSLICE_MEET;
    mAnimVal = mBaseVal;
    mIsAnimated = false;
    mIsBaseSet = false;
  }

  nsresult SetBaseValueString(const nsAString& aValue,
<<<<<<< HEAD
                              nsSVGElement* aSVGElement, bool aDoSetAttr);
||||||| merged common ancestors
                              nsSVGElement *aSVGElement,
                              bool aDoSetAttr);
=======
                              dom::SVGElement* aSVGElement, bool aDoSetAttr);
>>>>>>> upstream-releases
  void GetBaseValueString(nsAString& aValue) const;

<<<<<<< HEAD
  void SetBaseValue(const SVGPreserveAspectRatio& aValue,
                    nsSVGElement* aSVGElement);
  nsresult SetBaseAlign(uint16_t aAlign, nsSVGElement* aSVGElement) {
||||||| merged common ancestors
  void SetBaseValue(const SVGPreserveAspectRatio &aValue,
                    nsSVGElement *aSVGElement);
  nsresult SetBaseAlign(uint16_t aAlign, nsSVGElement *aSVGElement) {
=======
  void SetBaseValue(const SVGPreserveAspectRatio& aValue,
                    dom::SVGElement* aSVGElement);
  nsresult SetBaseAlign(uint16_t aAlign, dom::SVGElement* aSVGElement) {
>>>>>>> upstream-releases
    if (aAlign < SVG_ALIGN_MIN_VALID || aAlign > SVG_ALIGN_MAX_VALID) {
      return NS_ERROR_FAILURE;
    }
    SetBaseValue(SVGPreserveAspectRatio(static_cast<uint8_t>(aAlign),
                                        mBaseVal.GetMeetOrSlice()),
                 aSVGElement);
    return NS_OK;
  }
<<<<<<< HEAD
  nsresult SetBaseMeetOrSlice(uint16_t aMeetOrSlice,
                              nsSVGElement* aSVGElement) {
||||||| merged common ancestors
  nsresult SetBaseMeetOrSlice(uint16_t aMeetOrSlice, nsSVGElement *aSVGElement) {
=======
  nsresult SetBaseMeetOrSlice(uint16_t aMeetOrSlice,
                              dom::SVGElement* aSVGElement) {
>>>>>>> upstream-releases
    if (aMeetOrSlice < SVG_MEETORSLICE_MIN_VALID ||
        aMeetOrSlice > SVG_MEETORSLICE_MAX_VALID) {
      return NS_ERROR_FAILURE;
    }
    SetBaseValue(SVGPreserveAspectRatio(mBaseVal.GetAlign(),
                                        static_cast<uint8_t>(aMeetOrSlice)),
                 aSVGElement);
    return NS_OK;
  }
<<<<<<< HEAD
  void SetAnimValue(uint64_t aPackedValue, nsSVGElement* aSVGElement);
||||||| merged common ancestors
  void SetAnimValue(uint64_t aPackedValue, nsSVGElement *aSVGElement);
=======
  void SetAnimValue(uint64_t aPackedValue, dom::SVGElement* aSVGElement);
>>>>>>> upstream-releases

  const SVGPreserveAspectRatio& GetBaseValue() const { return mBaseVal; }
  const SVGPreserveAspectRatio& GetAnimValue() const { return mAnimVal; }
  bool IsAnimated() const { return mIsAnimated; }
  bool IsExplicitlySet() const { return mIsAnimated || mIsBaseSet; }

  already_AddRefed<mozilla::dom::DOMSVGAnimatedPreserveAspectRatio>
<<<<<<< HEAD
  ToDOMAnimatedPreserveAspectRatio(nsSVGElement* aSVGElement);
  UniquePtr<nsISMILAttr> ToSMILAttr(nsSVGElement* aSVGElement);

 private:
||||||| merged common ancestors
  ToDOMAnimatedPreserveAspectRatio(nsSVGElement* aSVGElement);
  UniquePtr<nsISMILAttr> ToSMILAttr(nsSVGElement* aSVGElement);

private:

=======
  ToDOMAnimatedPreserveAspectRatio(dom::SVGElement* aSVGElement);
  UniquePtr<SMILAttr> ToSMILAttr(dom::SVGElement* aSVGElement);

 private:
>>>>>>> upstream-releases
  SVGPreserveAspectRatio mAnimVal;
  SVGPreserveAspectRatio mBaseVal;
  bool mIsAnimated;
  bool mIsBaseSet;

<<<<<<< HEAD
 public:
  struct SMILPreserveAspectRatio final : public nsISMILAttr {
   public:
||||||| merged common ancestors
public:
  struct SMILPreserveAspectRatio final : public nsISMILAttr
  {
  public:
=======
 public:
  struct SMILPreserveAspectRatio final : public SMILAttr {
   public:
>>>>>>> upstream-releases
    SMILPreserveAspectRatio(SVGAnimatedPreserveAspectRatio* aVal,
<<<<<<< HEAD
                            nsSVGElement* aSVGElement)
        : mVal(aVal), mSVGElement(aSVGElement) {}
||||||| merged common ancestors
                            nsSVGElement* aSVGElement)
      : mVal(aVal), mSVGElement(aSVGElement) {}
=======
                            dom::SVGElement* aSVGElement)
        : mVal(aVal), mSVGElement(aSVGElement) {}
>>>>>>> upstream-releases

    // These will stay alive because a SMILAttr only lives as long
    // as the Compositing step, and DOM elements don't get a chance to
    // die during that.
    SVGAnimatedPreserveAspectRatio* mVal;
<<<<<<< HEAD
    nsSVGElement* mSVGElement;

    // nsISMILAttr methods
    virtual nsresult ValueFromString(
        const nsAString& aStr, const dom::SVGAnimationElement* aSrcElement,
        nsSMILValue& aValue, bool& aPreventCachingOfSandwich) const override;
    virtual nsSMILValue GetBaseValue() const override;
||||||| merged common ancestors
    nsSVGElement* mSVGElement;

    // nsISMILAttr methods
    virtual nsresult ValueFromString(const nsAString& aStr,
                                     const dom::SVGAnimationElement* aSrcElement,
                                     nsSMILValue& aValue,
                                     bool& aPreventCachingOfSandwich) const override;
    virtual nsSMILValue GetBaseValue() const override;
=======
    dom::SVGElement* mSVGElement;

    // SMILAttr methods
    virtual nsresult ValueFromString(
        const nsAString& aStr, const dom::SVGAnimationElement* aSrcElement,
        SMILValue& aValue, bool& aPreventCachingOfSandwich) const override;
    virtual SMILValue GetBaseValue() const override;
>>>>>>> upstream-releases
    virtual void ClearAnimValue() override;
    virtual nsresult SetAnimValue(const SMILValue& aValue) override;
  };
};

namespace dom {
class DOMSVGAnimatedPreserveAspectRatio final : public nsISupports,
                                                public nsWrapperCache {
  ~DOMSVGAnimatedPreserveAspectRatio();

  NS_DECL_CYCLE_COLLECTING_ISUPPORTS
  NS_DECL_CYCLE_COLLECTION_SCRIPT_HOLDER_CLASS(
      DOMSVGAnimatedPreserveAspectRatio)

  DOMSVGAnimatedPreserveAspectRatio(SVGAnimatedPreserveAspectRatio* aVal,
<<<<<<< HEAD
                                    nsSVGElement* aSVGElement)
      : mVal(aVal), mSVGElement(aSVGElement) {}
||||||| merged common ancestors
                                    nsSVGElement *aSVGElement)
    : mVal(aVal), mSVGElement(aSVGElement)
  {
  }
=======
                                    dom::SVGElement* aSVGElement)
      : mVal(aVal), mSVGElement(aSVGElement) {}
>>>>>>> upstream-releases

  // WebIDL
<<<<<<< HEAD
  nsSVGElement* GetParentObject() const { return mSVGElement; }
  virtual JSObject* WrapObject(JSContext* aCx,
                               JS::Handle<JSObject*> aGivenProto) override;
||||||| merged common ancestors
  nsSVGElement* GetParentObject() const { return mSVGElement; }
  virtual JSObject* WrapObject(JSContext* aCx, JS::Handle<JSObject*> aGivenProto) override;
=======
  dom::SVGElement* GetParentObject() const { return mSVGElement; }
  virtual JSObject* WrapObject(JSContext* aCx,
                               JS::Handle<JSObject*> aGivenProto) override;
>>>>>>> upstream-releases

  // These aren't weak refs because new objects are returned each time
  already_AddRefed<DOMSVGPreserveAspectRatio> BaseVal();
  already_AddRefed<DOMSVGPreserveAspectRatio> AnimVal();

 protected:
  // kept alive because it belongs to content:
  SVGAnimatedPreserveAspectRatio* mVal;
  RefPtr<dom::SVGElement> mSVGElement;
};

}  // namespace dom
}  // namespace mozilla

#endif  // MOZILLA_SVGANIMATEDPRESERVEASPECTRATIO_H__
