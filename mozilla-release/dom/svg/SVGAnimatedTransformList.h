/* -*- Mode: C++; tab-width: 8; indent-tabs-mode: nil; c-basic-offset: 2 -*- */
/* vim: set ts=8 sts=2 et sw=2 tw=80: */
/* This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/. */

#ifndef MOZILLA_SVGANIMATEDTRANSFORMLIST_H__
#define MOZILLA_SVGANIMATEDTRANSFORMLIST_H__

#include "mozilla/Attributes.h"
#include "mozilla/SMILAttr.h"
#include "mozilla/UniquePtr.h"
#include "nsAutoPtr.h"
#include "SVGTransformList.h"

class nsAtom;

namespace mozilla {

class SMILValue;

namespace dom {
class SVGAnimationElement;
class SVGElement;
class DOMSVGTransform;
}  // namespace dom

/**
 * Class SVGAnimatedTransformList
 *
 * This class is very different to the SVG DOM interface of the same name found
 * in the SVG specification. This is a lightweight internal class - see
 * DOMSVGAnimatedTransformList for the heavier DOM class that wraps instances of
 * this class and implements the SVG specification's SVGAnimatedTransformList
 * DOM interface.
 *
 * Except where noted otherwise, this class' methods take care of keeping the
 * appropriate DOM wrappers in sync (see the comment in
 * DOMSVGAnimatedTransformList::InternalBaseValListWillChangeTo) so that their
 * consumers don't need to concern themselves with that.
 */
<<<<<<< HEAD
class SVGAnimatedTransformList final : public nsWrapperCache {
  friend class mozilla::DOMSVGTransformList;
||||||| merged common ancestors
class SVGAnimatedTransformList final : public nsWrapperCache
{
  friend class mozilla::DOMSVGTransformList;
=======
class SVGAnimatedTransformList {
  // friends so that they can get write access to mBaseVal
  friend class dom::DOMSVGTransform;
  friend class DOMSVGTransformList;
>>>>>>> upstream-releases

<<<<<<< HEAD
 public:
  NS_INLINE_DECL_CYCLE_COLLECTING_NATIVE_REFCOUNTING(SVGAnimatedTransformList)
  NS_DECL_CYCLE_COLLECTION_SCRIPT_HOLDER_NATIVE_CLASS(SVGAnimatedTransformList)
||||||| merged common ancestors
public:
  NS_INLINE_DECL_CYCLE_COLLECTING_NATIVE_REFCOUNTING(SVGAnimatedTransformList)
  NS_DECL_CYCLE_COLLECTION_SCRIPT_HOLDER_NATIVE_CLASS(SVGAnimatedTransformList)
=======
 public:
  SVGAnimatedTransformList()
      : mIsAttrSet(false), mRequiresFrameReconstruction(true) {}
>>>>>>> upstream-releases

  /**
<<<<<<< HEAD
   * Factory method to create and return a SVGAnimatedTransformList wrapper
   * for a given internal nsSVGAnimatedTransformList object. The factory takes
   * care of caching the object that it returns so that the same object can be
   * returned for the given nsSVGAnimatedTransformList each time it is
   * requested. The cached object is only removed from the cache when it is
   * destroyed due to there being no more references to it or to any of its
   * descendant objects. If that happens, any subsequent call requesting the DOM
   * wrapper for the nsSVGAnimatedTransformList will naturally result in a new
   * SVGAnimatedTransformList being returned.
||||||| merged common ancestors
   * Factory method to create and return a SVGAnimatedTransformList wrapper
   * for a given internal nsSVGAnimatedTransformList object. The factory takes
   * care of caching the object that it returns so that the same object can be
   * returned for the given nsSVGAnimatedTransformList each time it is requested.
   * The cached object is only removed from the cache when it is destroyed due
   * to there being no more references to it or to any of its descendant
   * objects. If that happens, any subsequent call requesting the DOM wrapper
   * for the nsSVGAnimatedTransformList will naturally result in a new
   * SVGAnimatedTransformList being returned.
=======
   * Because it's so important that mBaseVal and its DOMSVGTransformList wrapper
   * (if any) be kept in sync (see the comment in
   * DOMSVGAnimatedTransformList::InternalBaseValListWillChangeTo), this method
   * returns a const reference. Only our friend classes may get mutable
   * references to mBaseVal.
>>>>>>> upstream-releases
   */
<<<<<<< HEAD
  static already_AddRefed<SVGAnimatedTransformList> GetDOMWrapper(
      nsSVGAnimatedTransformList* aList, nsSVGElement* aElement);
||||||| merged common ancestors
  static already_AddRefed<SVGAnimatedTransformList>
    GetDOMWrapper(nsSVGAnimatedTransformList *aList, nsSVGElement *aElement);
=======
  const SVGTransformList& GetBaseValue() const { return mBaseVal; }
>>>>>>> upstream-releases

<<<<<<< HEAD
  /**
   * This method returns the SVGAnimatedTransformList wrapper for an internal
   * nsSVGAnimatedTransformList object if it currently has a wrapper. If it does
   * not, then nullptr is returned.
   */
  static SVGAnimatedTransformList* GetDOMWrapperIfExists(
      nsSVGAnimatedTransformList* aList);
||||||| merged common ancestors
  /**
   * This method returns the SVGAnimatedTransformList wrapper for an internal
   * nsSVGAnimatedTransformList object if it currently has a wrapper. If it does
   * not, then nullptr is returned.
   */
  static SVGAnimatedTransformList*
    GetDOMWrapperIfExists(nsSVGAnimatedTransformList *aList);
=======
  nsresult SetBaseValue(const SVGTransformList& aValue,
                        dom::SVGElement* aSVGElement);

  nsresult SetBaseValueString(const nsAString& aValue,
                              dom::SVGElement* aSVGElement);

  void ClearBaseValue();

  const SVGTransformList& GetAnimValue() const {
    return mAnimVal ? *mAnimVal : mBaseVal;
  }

  nsresult SetAnimValue(const SVGTransformList& aValue,
                        dom::SVGElement* aElement);

  void ClearAnimValue(dom::SVGElement* aElement);
>>>>>>> upstream-releases

  /**
   * Returns true if the corresponding transform attribute is set (or animated)
   * to a valid value. Unlike HasTransform it will return true for an empty
   * transform.
   */
  bool IsExplicitlySet() const;

  /**
   * Returns true if the corresponding transform attribute is set (or animated)
   * to a valid value, such that we have at least one transform in our list.
   * Returns false otherwise (e.g. if the transform attribute is missing or
   * empty or invalid).
   */
<<<<<<< HEAD
  bool IsAnimating() const;

  // WebIDL
  nsSVGElement* GetParentObject() const { return mElement; }
  virtual JSObject* WrapObject(JSContext* aCx,
                               JS::Handle<JSObject*> aGivenProto) override;
  // These aren't weak refs because mBaseVal and mAnimVal are weak
  already_AddRefed<DOMSVGTransformList> BaseVal();
  already_AddRefed<DOMSVGTransformList> AnimVal();
||||||| merged common ancestors
  bool IsAnimating() const;

  // WebIDL
  nsSVGElement* GetParentObject() const { return mElement; }
  virtual JSObject* WrapObject(JSContext* aCx, JS::Handle<JSObject*> aGivenProto) override;
  // These aren't weak refs because mBaseVal and mAnimVal are weak
  already_AddRefed<DOMSVGTransformList> BaseVal();
  already_AddRefed<DOMSVGTransformList> AnimVal();
=======
  bool HasTransform() const {
    return (mAnimVal && !mAnimVal->IsEmpty()) || !mBaseVal.IsEmpty();
  }
>>>>>>> upstream-releases

<<<<<<< HEAD
 private:
||||||| merged common ancestors
private:

=======
  bool IsAnimating() const { return !!mAnimVal; }

>>>>>>> upstream-releases
  /**
   * Returns true if we need to reconstruct the frame of the element associated
   * with this transform list because the stacking context has changed.
   *
   * (This is used as part of an optimization in
   * SVGTransformableElement::GetAttributeChangeHint. That function reports an
   * inexpensive nsChangeHint when a transform has just modified -- but this
   * accessor lets it detect cases where the "modification" is actually adding
   * a transform where we previously had none. These cases require a more
   * thorough nsChangeHint.)
   */
<<<<<<< HEAD
  explicit SVGAnimatedTransformList(nsSVGElement* aElement)
      : mBaseVal(nullptr), mAnimVal(nullptr), mElement(aElement) {}
||||||| merged common ancestors
  explicit SVGAnimatedTransformList(nsSVGElement *aElement)
    : mBaseVal(nullptr)
    , mAnimVal(nullptr)
    , mElement(aElement)
  {
  }
=======
  bool RequiresFrameReconstruction() const {
    return mRequiresFrameReconstruction;
  }
>>>>>>> upstream-releases

<<<<<<< HEAD
  ~SVGAnimatedTransformList();

  /// Get a reference to this DOM wrapper object's internal counterpart.
  nsSVGAnimatedTransformList& InternalAList();
  const nsSVGAnimatedTransformList& InternalAList() const;

  // Weak refs to our DOMSVGTransformList baseVal/animVal objects. These objects
  // are friends and take care of clearing these pointers when they die, making
  // these true weak references.
  DOMSVGTransformList* mBaseVal;
  DOMSVGTransformList* mAnimVal;

  // Strong ref to our element to keep it alive. We hold this not only for
  // ourself, but also for our base/animVal and all of their items.
  RefPtr<nsSVGElement> mElement;
||||||| merged common ancestors
  ~SVGAnimatedTransformList();

  /// Get a reference to this DOM wrapper object's internal counterpart.
  nsSVGAnimatedTransformList& InternalAList();
  const nsSVGAnimatedTransformList& InternalAList() const;

  // Weak refs to our DOMSVGTransformList baseVal/animVal objects. These objects
  // are friends and take care of clearing these pointers when they die, making
  // these true weak references.
  DOMSVGTransformList *mBaseVal;
  DOMSVGTransformList *mAnimVal;

  // Strong ref to our element to keep it alive. We hold this not only for
  // ourself, but also for our base/animVal and all of their items.
  RefPtr<nsSVGElement> mElement;
=======
  mozilla::UniquePtr<SMILAttr> ToSMILAttr(dom::SVGElement* aSVGElement);

 private:
  // mAnimVal is a pointer to allow us to determine if we're being animated or
  // not. Making it a non-pointer member and using mAnimVal.IsEmpty() to check
  // if we're animating is not an option, since that would break animation *to*
  // the empty string (<set to="">).

  SVGTransformList mBaseVal;
  nsAutoPtr<SVGTransformList> mAnimVal;
  bool mIsAttrSet;
  // (See documentation for accessor, RequiresFrameReconstruction.)
  bool mRequiresFrameReconstruction;

  struct SMILAnimatedTransformList : public SMILAttr {
   public:
    SMILAnimatedTransformList(SVGAnimatedTransformList* aVal,
                              dom::SVGElement* aSVGElement)
        : mVal(aVal), mElement(aSVGElement) {}

    // SMILAttr methods
    virtual nsresult ValueFromString(
        const nsAString& aStr, const dom::SVGAnimationElement* aSrcElement,
        SMILValue& aValue, bool& aPreventCachingOfSandwich) const override;
    virtual SMILValue GetBaseValue() const override;
    virtual void ClearAnimValue() override;
    virtual nsresult SetAnimValue(const SMILValue& aNewAnimValue) override;

   protected:
    static void ParseValue(const nsAString& aSpec, const nsAtom* aTransformType,
                           SMILValue& aResult);
    static int32_t ParseParameterList(const nsAString& aSpec, float* aVars,
                                      int32_t aNVars);

    // These will stay alive because a SMILAttr only lives as long
    // as the Compositing step, and DOM elements don't get a chance to
    // die during that.
    SVGAnimatedTransformList* mVal;
    dom::SVGElement* mElement;
  };
>>>>>>> upstream-releases
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
#endif  // mozilla_dom_SVGAnimatedTransformList_h
||||||| merged common ancestors
#endif // mozilla_dom_SVGAnimatedTransformList_h
=======
#endif  // MOZILLA_SVGANIMATEDTRANSFORMLIST_H__
>>>>>>> upstream-releases
