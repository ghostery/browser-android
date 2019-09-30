/* -*- Mode: C++; tab-width: 8; indent-tabs-mode: nil; c-basic-offset: 2 -*- */
/* vim: set ts=8 sts=2 et sw=2 tw=80: */
/* This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/. */

#ifndef MOZILLA_SVGTRANSFORM_H__
#define MOZILLA_SVGTRANSFORM_H__

#include "gfxMatrix.h"
#include "mozilla/dom/SVGTransformBinding.h"
#include "mozilla/gfx/Matrix.h"
#include "nsDebug.h"
<<<<<<< HEAD
#include "nsID.h"
#include "nsSVGTransform.h"
#include "nsWrapperCache.h"
#include "mozilla/Attributes.h"

class nsSVGElement;

#define MOZ_SVG_LIST_INDEX_BIT_COUNT 31  // supports > 2 billion list items
||||||| merged common ancestors
#include "nsID.h"
#include "nsSVGTransform.h"
#include "nsWrapperCache.h"
#include "mozilla/Attributes.h"

class nsSVGElement;

#define MOZ_SVG_LIST_INDEX_BIT_COUNT 31 // supports > 2 billion list items
=======
>>>>>>> upstream-releases

namespace mozilla {

/*
 * The DOM wrapper class for this class is DOMSVGTransform.
 */
<<<<<<< HEAD
class SVGTransform final : public nsWrapperCache {
  friend class AutoChangeTransformNotifier;

 public:
  NS_INLINE_DECL_CYCLE_COLLECTING_NATIVE_REFCOUNTING(SVGTransform)
  NS_DECL_CYCLE_COLLECTION_SCRIPT_HOLDER_NATIVE_CLASS(SVGTransform)

  /**
   * Generic ctor for SVGTransform objects that are created for an attribute.
   */
  SVGTransform(DOMSVGTransformList* aList, uint32_t aListIndex,
               bool aIsAnimValItem);

  /**
   * Ctors for creating the objects returned by:
   *   SVGSVGElement.createSVGTransform(),
   *   SVGSVGElement.createSVGTransformFromMatrix(in SVGMatrix matrix),
   *   SVGTransformList.createSVGTransformFromMatrix(in SVGMatrix matrix)
   * which do not initially belong to an attribute.
   */
  explicit SVGTransform();
  explicit SVGTransform(const gfxMatrix& aMatrix);

  /**
   * Ctor for creating an unowned copy. Used with Clone().
   */
  explicit SVGTransform(const nsSVGTransform& aMatrix);

  /**
   * Create an unowned copy of an owned transform. The caller is responsible for
   * the first AddRef().
   */
  SVGTransform* Clone() {
    NS_ASSERTION(mList, "unexpected caller");
    return new SVGTransform(InternalItem());
||||||| merged common ancestors
class SVGTransform final : public nsWrapperCache
{
  friend class AutoChangeTransformNotifier;

public:
  NS_INLINE_DECL_CYCLE_COLLECTING_NATIVE_REFCOUNTING(SVGTransform)
  NS_DECL_CYCLE_COLLECTION_SCRIPT_HOLDER_NATIVE_CLASS(SVGTransform)

  /**
   * Generic ctor for SVGTransform objects that are created for an attribute.
   */
  SVGTransform(DOMSVGTransformList *aList,
               uint32_t aListIndex,
               bool aIsAnimValItem);

  /**
   * Ctors for creating the objects returned by:
   *   SVGSVGElement.createSVGTransform(),
   *   SVGSVGElement.createSVGTransformFromMatrix(in SVGMatrix matrix),
   *   SVGTransformList.createSVGTransformFromMatrix(in SVGMatrix matrix)
   * which do not initially belong to an attribute.
   */
  explicit SVGTransform();
  explicit SVGTransform(const gfxMatrix &aMatrix);

  /**
   * Ctor for creating an unowned copy. Used with Clone().
   */
  explicit SVGTransform(const nsSVGTransform &aMatrix);

  /**
   * Create an unowned copy of an owned transform. The caller is responsible for
   * the first AddRef().
   */
  SVGTransform* Clone() {
    NS_ASSERTION(mList, "unexpected caller");
    return new SVGTransform(InternalItem());
=======
class SVGTransform {
 public:
  // Default ctor initialises to matrix type with identity matrix
  SVGTransform()
      : mMatrix()  // Initialises to identity
        ,
        mAngle(0.f),
        mOriginX(0.f),
        mOriginY(0.f),
        mType(dom::SVGTransform_Binding::SVG_TRANSFORM_MATRIX) {}

  explicit SVGTransform(const gfxMatrix& aMatrix)
      : mMatrix(aMatrix),
        mAngle(0.f),
        mOriginX(0.f),
        mOriginY(0.f),
        mType(dom::SVGTransform_Binding::SVG_TRANSFORM_MATRIX) {}

  bool operator==(const SVGTransform& rhs) const {
    return mType == rhs.mType && MatricesEqual(mMatrix, rhs.mMatrix) &&
           mAngle == rhs.mAngle && mOriginX == rhs.mOriginX &&
           mOriginY == rhs.mOriginY;
>>>>>>> upstream-releases
  }

<<<<<<< HEAD
  bool IsInList() const { return !!mList; }
||||||| merged common ancestors
  bool IsInList() const {
    return !!mList;
  }
=======
  void GetValueAsString(nsAString& aValue) const;
>>>>>>> upstream-releases

<<<<<<< HEAD
  /**
   * In future, if this class is used for non-list transforms, this will be
   * different to IsInList().
   */
  bool HasOwner() const { return !!mList; }
||||||| merged common ancestors
  /**
   * In future, if this class is used for non-list transforms, this will be
   * different to IsInList().
   */
  bool HasOwner() const {
    return !!mList;
  }
=======
  float Angle() const { return mAngle; }
  void GetRotationOrigin(float& aOriginX, float& aOriginY) const {
    aOriginX = mOriginX;
    aOriginY = mOriginY;
  }
  uint16_t Type() const { return mType; }
>>>>>>> upstream-releases

<<<<<<< HEAD
  /**
   * This method is called to notify this DOM object that it is being inserted
   * into a list, and give it the information it needs as a result.
   *
   * This object MUST NOT already belong to a list when this method is called.
   * That's not to say that script can't move these DOM objects between
   * lists - it can - it's just that the logic to handle that (and send out
   * the necessary notifications) is located elsewhere (in
   * DOMSVGTransformList).)
   */
  void InsertingIntoList(DOMSVGTransformList* aList, uint32_t aListIndex,
                         bool aIsAnimValItem);

  static uint32_t MaxListIndex() {
    return (1U << MOZ_SVG_LIST_INDEX_BIT_COUNT) - 1;
  }

  /// This method is called to notify this object that its list index changed.
  void UpdateListIndex(uint32_t aListIndex) { mListIndex = aListIndex; }
||||||| merged common ancestors
  /**
   * This method is called to notify this DOM object that it is being inserted
   * into a list, and give it the information it needs as a result.
   *
   * This object MUST NOT already belong to a list when this method is called.
   * That's not to say that script can't move these DOM objects between
   * lists - it can - it's just that the logic to handle that (and send out
   * the necessary notifications) is located elsewhere (in
   * DOMSVGTransformList).)
   */
  void InsertingIntoList(DOMSVGTransformList *aList,
                         uint32_t aListIndex,
                         bool aIsAnimValItem);

  static uint32_t MaxListIndex() {
    return (1U << MOZ_SVG_LIST_INDEX_BIT_COUNT) - 1;
  }

  /// This method is called to notify this object that its list index changed.
  void UpdateListIndex(uint32_t aListIndex) {
    mListIndex = aListIndex;
  }
=======
  const gfxMatrix& GetMatrix() const { return mMatrix; }
  void SetMatrix(const gfxMatrix& aMatrix);
  void SetTranslate(float aTx, float aTy);
  void SetScale(float aSx, float aSy);
  void SetRotate(float aAngle, float aCx, float aCy);
  nsresult SetSkewX(float aAngle);
  nsresult SetSkewY(float aAngle);

  static bool MatricesEqual(const gfxMatrix& a, const gfxMatrix& b) {
    return a._11 == b._11 && a._12 == b._12 && a._21 == b._21 &&
           a._22 == b._22 && a._31 == b._31 && a._32 == b._32;
  }
>>>>>>> upstream-releases

 protected:
  gfxMatrix mMatrix;
  float mAngle, mOriginX, mOriginY;
  uint16_t mType;
};

<<<<<<< HEAD
  nsSVGTransform ToSVGTransform() const { return Transform(); }
||||||| merged common ancestors
  nsSVGTransform ToSVGTransform() const {
    return Transform();
  }
=======
/*
 * A slightly more light-weight version of SVGTransform for SMIL animation.
 *
 * Storing the parameters in an array (rather than a matrix) also allows simpler
 * (transform type-agnostic) interpolation and addition.
 *
 * The meaning of the mParams array depends on the transform type as follows:
 *
 * Type                | mParams[0], mParams[1], mParams[2], ...
 * --------------------+-----------------------------------------
 * translate           | tx, ty
 * scale               | sx, sy
 * rotate              | rotation-angle (in degrees), cx, cy
 * skewX               | skew-angle (in degrees)
 * skewY               | skew-angle (in degrees)
 * matrix              | a, b, c, d, e, f
 *
 * The matrix type is never generated by animation code (it is only produced
 * when the user inserts one via the DOM) and often requires special handling
 * when we do encounter it. Therefore many users of this class are only
 * interested in the first three parameters and so we provide a special
 * constructor for setting those parameters only.
 */
class SVGTransformSMILData {
 public:
  // Number of float-params required in constructor, if constructing one of the
  // 'simple' transform types (all but matrix type)
  static const uint32_t NUM_SIMPLE_PARAMS = 3;

  // Number of float-params required in constructor for matrix type.
  // This is also the number of params we actually store, regardless of type.
  static const uint32_t NUM_STORED_PARAMS = 6;

  explicit SVGTransformSMILData(uint16_t aType) : mTransformType(aType) {
    MOZ_ASSERT(aType >= dom::SVGTransform_Binding::SVG_TRANSFORM_MATRIX &&
                   aType <= dom::SVGTransform_Binding::SVG_TRANSFORM_SKEWY,
               "Unexpected transform type");
    for (uint32_t i = 0; i < NUM_STORED_PARAMS; ++i) {
      mParams[i] = 0.f;
    }
  }
>>>>>>> upstream-releases

<<<<<<< HEAD
  // WebIDL
  DOMSVGTransformList* GetParentObject() const { return mList; }
  virtual JSObject* WrapObject(JSContext* aCx,
                               JS::Handle<JSObject*> aGivenProto) override;
  uint16_t Type() const;
  dom::SVGMatrix* GetMatrix();
  float Angle() const;
  void SetMatrix(dom::SVGMatrix& matrix, ErrorResult& rv);
  void SetTranslate(float tx, float ty, ErrorResult& rv);
  void SetScale(float sx, float sy, ErrorResult& rv);
  void SetRotate(float angle, float cx, float cy, ErrorResult& rv);
  void SetSkewX(float angle, ErrorResult& rv);
  void SetSkewY(float angle, ErrorResult& rv);

 protected:
  ~SVGTransform();

  // Interface for SVGMatrix's use
  friend class dom::SVGMatrix;
  bool IsAnimVal() const { return mIsAnimValItem; }
  const gfxMatrix& Matrixgfx() const { return Transform().GetMatrix(); }
  void SetMatrix(const gfxMatrix& aMatrix);
||||||| merged common ancestors
  // WebIDL
  DOMSVGTransformList* GetParentObject() const { return mList; }
  virtual JSObject* WrapObject(JSContext* aCx, JS::Handle<JSObject*> aGivenProto) override;
  uint16_t Type() const;
  dom::SVGMatrix* GetMatrix();
  float Angle() const;
  void SetMatrix(dom::SVGMatrix& matrix, ErrorResult& rv);
  void SetTranslate(float tx, float ty, ErrorResult& rv);
  void SetScale(float sx, float sy, ErrorResult& rv);
  void SetRotate(float angle, float cx, float cy, ErrorResult& rv);
  void SetSkewX(float angle, ErrorResult& rv);
  void SetSkewY(float angle, ErrorResult& rv);

protected:
  ~SVGTransform();

  // Interface for SVGMatrix's use
  friend class dom::SVGMatrix;
  bool IsAnimVal() const {
    return mIsAnimValItem;
  }
  const gfxMatrix& Matrixgfx() const {
    return Transform().GetMatrix();
  }
  void SetMatrix(const gfxMatrix& aMatrix);
=======
  SVGTransformSMILData(uint16_t aType, float (&aParams)[NUM_SIMPLE_PARAMS])
      : mTransformType(aType) {
    MOZ_ASSERT(aType >= dom::SVGTransform_Binding::SVG_TRANSFORM_TRANSLATE &&
                   aType <= dom::SVGTransform_Binding::SVG_TRANSFORM_SKEWY,
               "Expected 'simple' transform type");
    for (uint32_t i = 0; i < NUM_SIMPLE_PARAMS; ++i) {
      mParams[i] = aParams[i];
    }
    for (uint32_t i = NUM_SIMPLE_PARAMS; i < NUM_STORED_PARAMS; ++i) {
      mParams[i] = 0.f;
    }
  }
>>>>>>> upstream-releases

<<<<<<< HEAD
 private:
  nsSVGElement* Element() { return mList->Element(); }
||||||| merged common ancestors
private:
  nsSVGElement* Element() {
    return mList->Element();
  }
=======
  // Conversion to/from a fully-fledged SVGTransform
  explicit SVGTransformSMILData(const SVGTransform& aTransform);
  SVGTransform ToSVGTransform() const;
>>>>>>> upstream-releases

  bool operator==(const SVGTransformSMILData& aOther) const {
    if (mTransformType != aOther.mTransformType) return false;

    for (uint32_t i = 0; i < NUM_STORED_PARAMS; ++i) {
      if (mParams[i] != aOther.mParams[i]) {
        return false;
      }
    }

    return true;
  }

<<<<<<< HEAD
  RefPtr<DOMSVGTransformList> mList;

  // Bounds for the following are checked in the ctor, so be sure to update
  // that if you change the capacity of any of the following.

  uint32_t mListIndex : MOZ_SVG_LIST_INDEX_BIT_COUNT;
  uint32_t mIsAnimValItem : 1;
||||||| merged common ancestors
  RefPtr<DOMSVGTransformList> mList;

  // Bounds for the following are checked in the ctor, so be sure to update
  // that if you change the capacity of any of the following.

  uint32_t mListIndex:MOZ_SVG_LIST_INDEX_BIT_COUNT;
  uint32_t mIsAnimValItem:1;
=======
  bool operator!=(const SVGTransformSMILData& aOther) const {
    return !(*this == aOther);
  }
>>>>>>> upstream-releases

<<<<<<< HEAD
  // Usually this class acts as a wrapper for an nsSVGTransform object which is
  // part of a list and is accessed by going via the owning Element.
  //
  // However, in some circumstances, objects of this class may not be associated
  // with any particular list and thus, no internal nsSVGTransform object. In
  // that case we allocate an nsSVGTransform object on the heap to store the
  // data.
  nsAutoPtr<nsSVGTransform> mTransform;
||||||| merged common ancestors
  // Usually this class acts as a wrapper for an nsSVGTransform object which is
  // part of a list and is accessed by going via the owning Element.
  //
  // However, in some circumstances, objects of this class may not be associated
  // with any particular list and thus, no internal nsSVGTransform object. In
  // that case we allocate an nsSVGTransform object on the heap to store the data.
  nsAutoPtr<nsSVGTransform> mTransform;
=======
  uint16_t mTransformType;
  float mParams[NUM_STORED_PARAMS];
>>>>>>> upstream-releases
};

<<<<<<< HEAD
}  // namespace dom
}  // namespace mozilla

#undef MOZ_SVG_LIST_INDEX_BIT_COUNT
||||||| merged common ancestors
} // namespace dom
} // namespace mozilla

#undef MOZ_SVG_LIST_INDEX_BIT_COUNT
=======
}  // namespace mozilla
>>>>>>> upstream-releases

<<<<<<< HEAD
#endif  // mozilla_dom_SVGTransform_h
||||||| merged common ancestors
#endif // mozilla_dom_SVGTransform_h
=======
#endif  // MOZILLA_SVGTRANSFORM_H__
>>>>>>> upstream-releases
