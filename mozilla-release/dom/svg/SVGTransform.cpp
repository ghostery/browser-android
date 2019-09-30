/* -*- Mode: C++; tab-width: 8; indent-tabs-mode: nil; c-basic-offset: 2 -*- */
/* vim: set ts=8 sts=2 et sw=2 tw=80: */
/* This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/. */

#include "SVGTransform.h"

#include "nsError.h"
#include "nsContentUtils.h"  // for NS_ENSURE_FINITE
#include "nsTextFormatter.h"

namespace {
const double kRadPerDegree = 2.0 * M_PI / 360.0;
}  // namespace

namespace mozilla {

<<<<<<< HEAD
using namespace SVGTransform_Binding;

static nsSVGAttrTearoffTable<SVGTransform, SVGMatrix>& SVGMatrixTearoffTable() {
  static nsSVGAttrTearoffTable<SVGTransform, SVGMatrix> sSVGMatrixTearoffTable;
  return sSVGMatrixTearoffTable;
}

//----------------------------------------------------------------------

// We could use NS_IMPL_CYCLE_COLLECTION(, except that in Unlink() we need to
// clear our list's weak ref to us to be safe. (The other option would be to
// not unlink and rely on the breaking of the other edges in the cycle, as
// NS_SVG_VAL_IMPL_CYCLE_COLLECTION does.)
NS_IMPL_CYCLE_COLLECTION_CLASS(SVGTransform)

NS_IMPL_CYCLE_COLLECTION_UNLINK_BEGIN(SVGTransform)
  // We may not belong to a list, so we must null check tmp->mList.
  if (tmp->mList) {
    tmp->mList->mItems[tmp->mListIndex] = nullptr;
  }
  NS_IMPL_CYCLE_COLLECTION_UNLINK(mList)
  NS_IMPL_CYCLE_COLLECTION_UNLINK_PRESERVED_WRAPPER
NS_IMPL_CYCLE_COLLECTION_UNLINK_END

NS_IMPL_CYCLE_COLLECTION_TRAVERSE_BEGIN(SVGTransform)
  NS_IMPL_CYCLE_COLLECTION_TRAVERSE(mList)
  SVGMatrix* matrix = SVGMatrixTearoffTable().GetTearoff(tmp);
  CycleCollectionNoteChild(cb, matrix, "matrix");
NS_IMPL_CYCLE_COLLECTION_TRAVERSE_END

NS_IMPL_CYCLE_COLLECTION_TRACE_BEGIN(SVGTransform)
  NS_IMPL_CYCLE_COLLECTION_TRACE_PRESERVED_WRAPPER
NS_IMPL_CYCLE_COLLECTION_TRACE_END

NS_IMPL_CYCLE_COLLECTION_ROOT_NATIVE(SVGTransform, AddRef)
NS_IMPL_CYCLE_COLLECTION_UNROOT_NATIVE(SVGTransform, Release)

JSObject* SVGTransform::WrapObject(JSContext* aCx,
                                   JS::Handle<JSObject*> aGivenProto) {
  return SVGTransform_Binding::Wrap(aCx, this, aGivenProto);
}

//----------------------------------------------------------------------
// Helper class: AutoChangeTransformNotifier
// Stack-based helper class to pair calls to WillChangeTransformList
// and DidChangeTransformList.
class MOZ_RAII AutoChangeTransformNotifier {
 public:
  explicit AutoChangeTransformNotifier(
      SVGTransform* aTransform MOZ_GUARD_OBJECT_NOTIFIER_PARAM)
      : mTransform(aTransform) {
    MOZ_GUARD_OBJECT_NOTIFIER_INIT;
    MOZ_ASSERT(mTransform, "Expecting non-null transform");
    if (mTransform->HasOwner()) {
      mEmptyOrOldValue = mTransform->Element()->WillChangeTransformList();
||||||| merged common ancestors
using namespace SVGTransform_Binding;

static nsSVGAttrTearoffTable<SVGTransform, SVGMatrix>&
SVGMatrixTearoffTable()
{
  static nsSVGAttrTearoffTable<SVGTransform, SVGMatrix> sSVGMatrixTearoffTable;
  return sSVGMatrixTearoffTable;
}

//----------------------------------------------------------------------

// We could use NS_IMPL_CYCLE_COLLECTION(, except that in Unlink() we need to
// clear our list's weak ref to us to be safe. (The other option would be to
// not unlink and rely on the breaking of the other edges in the cycle, as
// NS_SVG_VAL_IMPL_CYCLE_COLLECTION does.)
NS_IMPL_CYCLE_COLLECTION_CLASS(SVGTransform)

NS_IMPL_CYCLE_COLLECTION_UNLINK_BEGIN(SVGTransform)
  // We may not belong to a list, so we must null check tmp->mList.
  if (tmp->mList) {
    tmp->mList->mItems[tmp->mListIndex] = nullptr;
  }
NS_IMPL_CYCLE_COLLECTION_UNLINK(mList)
NS_IMPL_CYCLE_COLLECTION_UNLINK_PRESERVED_WRAPPER
NS_IMPL_CYCLE_COLLECTION_UNLINK_END

NS_IMPL_CYCLE_COLLECTION_TRAVERSE_BEGIN(SVGTransform)
NS_IMPL_CYCLE_COLLECTION_TRAVERSE(mList)
  SVGMatrix* matrix =
    SVGMatrixTearoffTable().GetTearoff(tmp);
  CycleCollectionNoteChild(cb, matrix, "matrix");
NS_IMPL_CYCLE_COLLECTION_TRAVERSE_END

NS_IMPL_CYCLE_COLLECTION_TRACE_BEGIN(SVGTransform)
NS_IMPL_CYCLE_COLLECTION_TRACE_PRESERVED_WRAPPER
NS_IMPL_CYCLE_COLLECTION_TRACE_END

NS_IMPL_CYCLE_COLLECTION_ROOT_NATIVE(SVGTransform, AddRef)
NS_IMPL_CYCLE_COLLECTION_UNROOT_NATIVE(SVGTransform, Release)

JSObject*
SVGTransform::WrapObject(JSContext* aCx, JS::Handle<JSObject*> aGivenProto)
{
  return SVGTransform_Binding::Wrap(aCx, this, aGivenProto);
}

//----------------------------------------------------------------------
// Helper class: AutoChangeTransformNotifier
// Stack-based helper class to pair calls to WillChangeTransformList
// and DidChangeTransformList.
class MOZ_RAII AutoChangeTransformNotifier
{
public:
  explicit AutoChangeTransformNotifier(SVGTransform* aTransform MOZ_GUARD_OBJECT_NOTIFIER_PARAM)
    : mTransform(aTransform)
  {
    MOZ_GUARD_OBJECT_NOTIFIER_INIT;
    MOZ_ASSERT(mTransform, "Expecting non-null transform");
    if (mTransform->HasOwner()) {
      mEmptyOrOldValue =
        mTransform->Element()->WillChangeTransformList();
=======
using namespace dom::SVGTransform_Binding;

void SVGTransform::GetValueAsString(nsAString& aValue) const {
  switch (mType) {
    case SVG_TRANSFORM_TRANSLATE:
      // The spec say that if Y is not provided, it is assumed to be zero.
      if (mMatrix._32 != 0)
        nsTextFormatter::ssprintf(aValue, u"translate(%g, %g)", mMatrix._31,
                                  mMatrix._32);
      else
        nsTextFormatter::ssprintf(aValue, u"translate(%g)", mMatrix._31);
      break;
    case SVG_TRANSFORM_ROTATE:
      if (mOriginX != 0.0f || mOriginY != 0.0f)
        nsTextFormatter::ssprintf(aValue, u"rotate(%g, %g, %g)", mAngle,
                                  mOriginX, mOriginY);
      else
        nsTextFormatter::ssprintf(aValue, u"rotate(%g)", mAngle);
      break;
    case SVG_TRANSFORM_SCALE:
      if (mMatrix._11 != mMatrix._22)
        nsTextFormatter::ssprintf(aValue, u"scale(%g, %g)", mMatrix._11,
                                  mMatrix._22);
      else
        nsTextFormatter::ssprintf(aValue, u"scale(%g)", mMatrix._11);
      break;
    case SVG_TRANSFORM_SKEWX:
      nsTextFormatter::ssprintf(aValue, u"skewX(%g)", mAngle);
      break;
    case SVG_TRANSFORM_SKEWY:
      nsTextFormatter::ssprintf(aValue, u"skewY(%g)", mAngle);
      break;
    case SVG_TRANSFORM_MATRIX:
      nsTextFormatter::ssprintf(aValue, u"matrix(%g, %g, %g, %g, %g, %g)",
                                mMatrix._11, mMatrix._12, mMatrix._21,
                                mMatrix._22, mMatrix._31, mMatrix._32);
      break;
    default:
      aValue.Truncate();
      NS_ERROR("unknown transformation type");
      break;
  }
}

void SVGTransform::SetMatrix(const gfxMatrix& aMatrix) {
  mType = SVG_TRANSFORM_MATRIX;
  mMatrix = aMatrix;
  // We set the other members here too, since operator== requires it and
  // the DOM requires it for mAngle.
  mAngle = 0.f;
  mOriginX = 0.f;
  mOriginY = 0.f;
}

void SVGTransform::SetTranslate(float aTx, float aTy) {
  mType = SVG_TRANSFORM_TRANSLATE;
  mMatrix = gfxMatrix::Translation(aTx, aTy);
  mAngle = 0.f;
  mOriginX = 0.f;
  mOriginY = 0.f;
}

void SVGTransform::SetScale(float aSx, float aSy) {
  mType = SVG_TRANSFORM_SCALE;
  mMatrix = gfxMatrix::Scaling(aSx, aSy);
  mAngle = 0.f;
  mOriginX = 0.f;
  mOriginY = 0.f;
}

void SVGTransform::SetRotate(float aAngle, float aCx, float aCy) {
  mType = SVG_TRANSFORM_ROTATE;
  mMatrix = gfxMatrix::Translation(aCx, aCy)
                .PreRotate(aAngle * kRadPerDegree)
                .PreTranslate(-aCx, -aCy);
  mAngle = aAngle;
  mOriginX = aCx;
  mOriginY = aCy;
}

nsresult SVGTransform::SetSkewX(float aAngle) {
  double ta = tan(aAngle * kRadPerDegree);
  NS_ENSURE_FINITE(ta, NS_ERROR_RANGE_ERR);

  mType = SVG_TRANSFORM_SKEWX;
  mMatrix = gfxMatrix();
  mMatrix._21 = ta;
  mAngle = aAngle;
  mOriginX = 0.f;
  mOriginY = 0.f;
  return NS_OK;
}

nsresult SVGTransform::SetSkewY(float aAngle) {
  double ta = tan(aAngle * kRadPerDegree);
  NS_ENSURE_FINITE(ta, NS_ERROR_RANGE_ERR);

  mType = SVG_TRANSFORM_SKEWY;
  mMatrix = gfxMatrix();
  mMatrix._12 = ta;
  mAngle = aAngle;
  mOriginX = 0.f;
  mOriginY = 0.f;
  return NS_OK;
}

SVGTransformSMILData::SVGTransformSMILData(const SVGTransform& aTransform)
    : mTransformType(aTransform.Type()) {
  MOZ_ASSERT(mTransformType >= SVG_TRANSFORM_MATRIX &&
                 mTransformType <= SVG_TRANSFORM_SKEWY,
             "Unexpected transform type");

  for (uint32_t i = 0; i < NUM_STORED_PARAMS; ++i) {
    mParams[i] = 0.f;
  }

  switch (mTransformType) {
    case SVG_TRANSFORM_MATRIX: {
      const gfxMatrix& mx = aTransform.GetMatrix();
      mParams[0] = static_cast<float>(mx._11);
      mParams[1] = static_cast<float>(mx._12);
      mParams[2] = static_cast<float>(mx._21);
      mParams[3] = static_cast<float>(mx._22);
      mParams[4] = static_cast<float>(mx._31);
      mParams[5] = static_cast<float>(mx._32);
      break;
>>>>>>> upstream-releases
    }
<<<<<<< HEAD
  }

  ~AutoChangeTransformNotifier() {
    if (mTransform->HasOwner()) {
      mTransform->Element()->DidChangeTransformList(mEmptyOrOldValue);
      // Null check mTransform->mList, since DidChangeTransformList can run
      // script, potentially removing mTransform from its list.
      if (mTransform->mList && mTransform->mList->IsAnimating()) {
        mTransform->Element()->AnimationNeedsResample();
      }
||||||| merged common ancestors
  }

  ~AutoChangeTransformNotifier()
  {
    if (mTransform->HasOwner()) {
      mTransform->Element()->DidChangeTransformList(mEmptyOrOldValue);
      if (mTransform->mList->IsAnimating()) {
        mTransform->Element()->AnimationNeedsResample();
      }
=======
    case SVG_TRANSFORM_TRANSLATE: {
      const gfxMatrix& mx = aTransform.GetMatrix();
      mParams[0] = static_cast<float>(mx._31);
      mParams[1] = static_cast<float>(mx._32);
      break;
>>>>>>> upstream-releases
    }
<<<<<<< HEAD
  }

 private:
  SVGTransform* const mTransform;
  nsAttrValue mEmptyOrOldValue;
  MOZ_DECL_USE_GUARD_OBJECT_NOTIFIER
};

//----------------------------------------------------------------------
// Ctors:

SVGTransform::SVGTransform(DOMSVGTransformList* aList, uint32_t aListIndex,
                           bool aIsAnimValItem)
    : mList(aList),
      mListIndex(aListIndex),
      mIsAnimValItem(aIsAnimValItem),
      mTransform(nullptr) {
  // These shifts are in sync with the members in the header.
  MOZ_ASSERT(aList && aListIndex <= MaxListIndex(), "bad arg");

  MOZ_ASSERT(IndexIsValid(), "Bad index for DOMSVGNumber!");
}

SVGTransform::SVGTransform()
    : mList(nullptr),
      mListIndex(0),
      mIsAnimValItem(false),
      mTransform(new nsSVGTransform())  // Default ctor for objects not in a
                                        // list initialises to matrix type with
                                        // identity matrix
{}

SVGTransform::SVGTransform(const gfxMatrix& aMatrix)
    : mList(nullptr),
      mListIndex(0),
      mIsAnimValItem(false),
      mTransform(new nsSVGTransform(aMatrix)) {}

SVGTransform::SVGTransform(const nsSVGTransform& aTransform)
    : mList(nullptr),
      mListIndex(0),
      mIsAnimValItem(false),
      mTransform(new nsSVGTransform(aTransform)) {}

SVGTransform::~SVGTransform() {
  SVGMatrix* matrix = SVGMatrixTearoffTable().GetTearoff(this);
  if (matrix) {
    SVGMatrixTearoffTable().RemoveTearoff(this);
    NS_RELEASE(matrix);
  }
  // Our mList's weak ref to us must be nulled out when we die. If GC has
  // unlinked us using the cycle collector code, then that has already
  // happened, and mList is null.
  if (mList) {
    mList->mItems[mListIndex] = nullptr;
  }
}

uint16_t SVGTransform::Type() const { return Transform().Type(); }

SVGMatrix* SVGTransform::GetMatrix() {
  SVGMatrix* wrapper = SVGMatrixTearoffTable().GetTearoff(this);
  if (!wrapper) {
    NS_ADDREF(wrapper = new SVGMatrix(*this));
    SVGMatrixTearoffTable().AddTearoff(this, wrapper);
  }
  return wrapper;
}

float SVGTransform::Angle() const { return Transform().Angle(); }

void SVGTransform::SetMatrix(SVGMatrix& aMatrix, ErrorResult& rv) {
  if (mIsAnimValItem) {
    rv.Throw(NS_ERROR_DOM_NO_MODIFICATION_ALLOWED_ERR);
    return;
  }
  SetMatrix(aMatrix.GetMatrix());
}

void SVGTransform::SetTranslate(float tx, float ty, ErrorResult& rv) {
  if (mIsAnimValItem) {
    rv.Throw(NS_ERROR_DOM_NO_MODIFICATION_ALLOWED_ERR);
    return;
  }

  if (Transform().Type() == SVG_TRANSFORM_TRANSLATE && Matrixgfx()._31 == tx &&
      Matrixgfx()._32 == ty) {
    return;
  }

  AutoChangeTransformNotifier notifier(this);
  Transform().SetTranslate(tx, ty);
}

void SVGTransform::SetScale(float sx, float sy, ErrorResult& rv) {
  if (mIsAnimValItem) {
    rv.Throw(NS_ERROR_DOM_NO_MODIFICATION_ALLOWED_ERR);
    return;
  }

  if (Transform().Type() == SVG_TRANSFORM_SCALE && Matrixgfx()._11 == sx &&
      Matrixgfx()._22 == sy) {
    return;
  }
  AutoChangeTransformNotifier notifier(this);
  Transform().SetScale(sx, sy);
}

void SVGTransform::SetRotate(float angle, float cx, float cy, ErrorResult& rv) {
  if (mIsAnimValItem) {
    rv.Throw(NS_ERROR_DOM_NO_MODIFICATION_ALLOWED_ERR);
    return;
  }

  if (Transform().Type() == SVG_TRANSFORM_ROTATE) {
    float currentCx, currentCy;
    Transform().GetRotationOrigin(currentCx, currentCy);
    if (Transform().Angle() == angle && currentCx == cx && currentCy == cy) {
      return;
||||||| merged common ancestors
  }

private:
  SVGTransform* const mTransform;
  nsAttrValue   mEmptyOrOldValue;
  MOZ_DECL_USE_GUARD_OBJECT_NOTIFIER
};

//----------------------------------------------------------------------
// Ctors:

SVGTransform::SVGTransform(DOMSVGTransformList *aList,
                           uint32_t aListIndex,
                           bool aIsAnimValItem)
  : mList(aList)
  , mListIndex(aListIndex)
  , mIsAnimValItem(aIsAnimValItem)
  , mTransform(nullptr)
{
  // These shifts are in sync with the members in the header.
  MOZ_ASSERT(aList && aListIndex <= MaxListIndex(), "bad arg");

  MOZ_ASSERT(IndexIsValid(), "Bad index for DOMSVGNumber!");
}

SVGTransform::SVGTransform()
  : mList(nullptr)
  , mListIndex(0)
  , mIsAnimValItem(false)
  , mTransform(new nsSVGTransform()) // Default ctor for objects not in a list
                                     // initialises to matrix type with identity
                                     // matrix
{
}

SVGTransform::SVGTransform(const gfxMatrix &aMatrix)
  : mList(nullptr)
  , mListIndex(0)
  , mIsAnimValItem(false)
  , mTransform(new nsSVGTransform(aMatrix))
{
}

SVGTransform::SVGTransform(const nsSVGTransform &aTransform)
  : mList(nullptr)
  , mListIndex(0)
  , mIsAnimValItem(false)
  , mTransform(new nsSVGTransform(aTransform))
{
}

SVGTransform::~SVGTransform()
{
  SVGMatrix* matrix = SVGMatrixTearoffTable().GetTearoff(this);
  if (matrix) {
    SVGMatrixTearoffTable().RemoveTearoff(this);
    NS_RELEASE(matrix);
  }
  // Our mList's weak ref to us must be nulled out when we die. If GC has
  // unlinked us using the cycle collector code, then that has already
  // happened, and mList is null.
  if (mList) {
    mList->mItems[mListIndex] = nullptr;
  }
}

uint16_t
SVGTransform::Type() const
{
  return Transform().Type();
}

SVGMatrix*
SVGTransform::GetMatrix()
{
  SVGMatrix* wrapper =
    SVGMatrixTearoffTable().GetTearoff(this);
  if (!wrapper) {
    NS_ADDREF(wrapper = new SVGMatrix(*this));
    SVGMatrixTearoffTable().AddTearoff(this, wrapper);
  }
  return wrapper;
}

float
SVGTransform::Angle() const
{
  return Transform().Angle();
}

void
SVGTransform::SetMatrix(SVGMatrix& aMatrix, ErrorResult& rv)
{
  if (mIsAnimValItem) {
    rv.Throw(NS_ERROR_DOM_NO_MODIFICATION_ALLOWED_ERR);
    return;
  }
  SetMatrix(aMatrix.GetMatrix());
}

void
SVGTransform::SetTranslate(float tx, float ty, ErrorResult& rv)
{
  if (mIsAnimValItem) {
    rv.Throw(NS_ERROR_DOM_NO_MODIFICATION_ALLOWED_ERR);
    return;
  }

  if (Transform().Type() == SVG_TRANSFORM_TRANSLATE &&
      Matrixgfx()._31 == tx && Matrixgfx()._32 == ty) {
    return;
  }

  AutoChangeTransformNotifier notifier(this);
  Transform().SetTranslate(tx, ty);
}

void
SVGTransform::SetScale(float sx, float sy, ErrorResult& rv)
{
  if (mIsAnimValItem) {
    rv.Throw(NS_ERROR_DOM_NO_MODIFICATION_ALLOWED_ERR);
    return;
  }

  if (Transform().Type() == SVG_TRANSFORM_SCALE &&
      Matrixgfx()._11 == sx && Matrixgfx()._22 == sy) {
    return;
  }
  AutoChangeTransformNotifier notifier(this);
  Transform().SetScale(sx, sy);
}

void
SVGTransform::SetRotate(float angle, float cx, float cy, ErrorResult& rv)
{
  if (mIsAnimValItem) {
    rv.Throw(NS_ERROR_DOM_NO_MODIFICATION_ALLOWED_ERR);
    return;
  }

  if (Transform().Type() == SVG_TRANSFORM_ROTATE) {
    float currentCx, currentCy;
    Transform().GetRotationOrigin(currentCx, currentCy);
    if (Transform().Angle() == angle && currentCx == cx && currentCy == cy) {
      return;
=======
    case SVG_TRANSFORM_SCALE: {
      const gfxMatrix& mx = aTransform.GetMatrix();
      mParams[0] = static_cast<float>(mx._11);
      mParams[1] = static_cast<float>(mx._22);
      break;
>>>>>>> upstream-releases
    }
<<<<<<< HEAD
  }

  AutoChangeTransformNotifier notifier(this);
  Transform().SetRotate(angle, cx, cy);
}

void SVGTransform::SetSkewX(float angle, ErrorResult& rv) {
  if (mIsAnimValItem) {
    rv.Throw(NS_ERROR_DOM_NO_MODIFICATION_ALLOWED_ERR);
    return;
  }

  if (Transform().Type() == SVG_TRANSFORM_SKEWX &&
      Transform().Angle() == angle) {
    return;
  }

  if (!IsFinite(tan(angle * kRadPerDegree))) {
    rv.ThrowRangeError<MSG_INVALID_TRANSFORM_ANGLE_ERROR>();
    return;
  }

  AutoChangeTransformNotifier notifier(this);
  DebugOnly<nsresult> result = Transform().SetSkewX(angle);
  MOZ_ASSERT(NS_SUCCEEDED(result), "SetSkewX unexpectedly failed");
}
||||||| merged common ancestors
  }

  AutoChangeTransformNotifier notifier(this);
  Transform().SetRotate(angle, cx, cy);
}

void
SVGTransform::SetSkewX(float angle, ErrorResult& rv)
{
  if (mIsAnimValItem) {
    rv.Throw(NS_ERROR_DOM_NO_MODIFICATION_ALLOWED_ERR);
    return;
  }

  if (Transform().Type() == SVG_TRANSFORM_SKEWX &&
      Transform().Angle() == angle) {
    return;
  }

  if (!IsFinite(tan(angle * kRadPerDegree))) {
    rv.ThrowRangeError<MSG_INVALID_TRANSFORM_ANGLE_ERROR>();
    return;
  }

  AutoChangeTransformNotifier notifier(this);
  DebugOnly<nsresult> result = Transform().SetSkewX(angle);
  MOZ_ASSERT(NS_SUCCEEDED(result), "SetSkewX unexpectedly failed");
}
=======
    case SVG_TRANSFORM_ROTATE:
      mParams[0] = aTransform.Angle();
      aTransform.GetRotationOrigin(mParams[1], mParams[2]);
      break;
>>>>>>> upstream-releases

<<<<<<< HEAD
void SVGTransform::SetSkewY(float angle, ErrorResult& rv) {
  if (mIsAnimValItem) {
    rv.Throw(NS_ERROR_DOM_NO_MODIFICATION_ALLOWED_ERR);
    return;
  }

  if (Transform().Type() == SVG_TRANSFORM_SKEWY &&
      Transform().Angle() == angle) {
    return;
  }
||||||| merged common ancestors
void
SVGTransform::SetSkewY(float angle, ErrorResult& rv)
{
  if (mIsAnimValItem) {
    rv.Throw(NS_ERROR_DOM_NO_MODIFICATION_ALLOWED_ERR);
    return;
  }

  if (Transform().Type() == SVG_TRANSFORM_SKEWY &&
      Transform().Angle() == angle) {
    return;
  }
=======
    case SVG_TRANSFORM_SKEWX:
    case SVG_TRANSFORM_SKEWY:
      mParams[0] = aTransform.Angle();
      break;
>>>>>>> upstream-releases

    default:
      MOZ_ASSERT_UNREACHABLE("Unexpected transform type");
      break;
  }
<<<<<<< HEAD

  AutoChangeTransformNotifier notifier(this);
  DebugOnly<nsresult> result = Transform().SetSkewY(angle);
  MOZ_ASSERT(NS_SUCCEEDED(result), "SetSkewY unexpectedly failed");
}

//----------------------------------------------------------------------
// List management methods:

void SVGTransform::InsertingIntoList(DOMSVGTransformList* aList,
                                     uint32_t aListIndex, bool aIsAnimValItem) {
  MOZ_ASSERT(!HasOwner(), "Inserting item that is already in a list");

  mList = aList;
  mListIndex = aListIndex;
  mIsAnimValItem = aIsAnimValItem;
  mTransform = nullptr;

  MOZ_ASSERT(IndexIsValid(), "Bad index for DOMSVGLength!");
||||||| merged common ancestors

  AutoChangeTransformNotifier notifier(this);
  DebugOnly<nsresult> result = Transform().SetSkewY(angle);
  MOZ_ASSERT(NS_SUCCEEDED(result), "SetSkewY unexpectedly failed");
}

//----------------------------------------------------------------------
// List management methods:

void
SVGTransform::InsertingIntoList(DOMSVGTransformList *aList,
                                uint32_t aListIndex,
                                bool aIsAnimValItem)
{
  MOZ_ASSERT(!HasOwner(), "Inserting item that is already in a list");

  mList = aList;
  mListIndex = aListIndex;
  mIsAnimValItem = aIsAnimValItem;
  mTransform = nullptr;

  MOZ_ASSERT(IndexIsValid(), "Bad index for DOMSVGLength!");
=======
>>>>>>> upstream-releases
}

<<<<<<< HEAD
void SVGTransform::RemovingFromList() {
  MOZ_ASSERT(!mTransform,
             "Item in list also has another non-list value associated with it");

  mTransform = new nsSVGTransform(InternalItem());
  mList = nullptr;
  mIsAnimValItem = false;
}
||||||| merged common ancestors
void
SVGTransform::RemovingFromList()
{
  MOZ_ASSERT(!mTransform,
             "Item in list also has another non-list value associated with it");

  mTransform = new nsSVGTransform(InternalItem());
  mList = nullptr;
  mIsAnimValItem = false;
}
=======
SVGTransform SVGTransformSMILData::ToSVGTransform() const {
  SVGTransform result;
>>>>>>> upstream-releases

<<<<<<< HEAD
nsSVGTransform& SVGTransform::InternalItem() {
  nsSVGAnimatedTransformList* alist = Element()->GetAnimatedTransformList();
  return mIsAnimValItem && alist->mAnimVal ? (*alist->mAnimVal)[mListIndex]
                                           : alist->mBaseVal[mListIndex];
}
||||||| merged common ancestors
nsSVGTransform&
SVGTransform::InternalItem()
{
  nsSVGAnimatedTransformList *alist = Element()->GetAnimatedTransformList();
  return mIsAnimValItem && alist->mAnimVal ?
    (*alist->mAnimVal)[mListIndex] :
    alist->mBaseVal[mListIndex];
}
=======
  switch (mTransformType) {
    case SVG_TRANSFORM_MATRIX:
      result.SetMatrix(gfxMatrix(mParams[0], mParams[1], mParams[2], mParams[3],
                                 mParams[4], mParams[5]));
      break;
>>>>>>> upstream-releases

<<<<<<< HEAD
const nsSVGTransform& SVGTransform::InternalItem() const {
  return const_cast<SVGTransform*>(this)->InternalItem();
}
||||||| merged common ancestors
const nsSVGTransform&
SVGTransform::InternalItem() const
{
  return const_cast<SVGTransform*>(this)->InternalItem();
}
=======
    case SVG_TRANSFORM_TRANSLATE:
      result.SetTranslate(mParams[0], mParams[1]);
      break;
>>>>>>> upstream-releases

<<<<<<< HEAD
#ifdef DEBUG
bool SVGTransform::IndexIsValid() {
  nsSVGAnimatedTransformList* alist = Element()->GetAnimatedTransformList();
  return (mIsAnimValItem && mListIndex < alist->GetAnimValue().Length()) ||
         (!mIsAnimValItem && mListIndex < alist->GetBaseValue().Length());
}
#endif  // DEBUG
||||||| merged common ancestors
#ifdef DEBUG
bool
SVGTransform::IndexIsValid()
{
  nsSVGAnimatedTransformList *alist = Element()->GetAnimatedTransformList();
  return (mIsAnimValItem &&
          mListIndex < alist->GetAnimValue().Length()) ||
         (!mIsAnimValItem &&
          mListIndex < alist->GetBaseValue().Length());
}
#endif // DEBUG

=======
    case SVG_TRANSFORM_SCALE:
      result.SetScale(mParams[0], mParams[1]);
      break;

    case SVG_TRANSFORM_ROTATE:
      result.SetRotate(mParams[0], mParams[1], mParams[2]);
      break;
>>>>>>> upstream-releases

    case SVG_TRANSFORM_SKEWX:
      result.SetSkewX(mParams[0]);
      break;

<<<<<<< HEAD
void SVGTransform::SetMatrix(const gfxMatrix& aMatrix) {
  MOZ_ASSERT(!mIsAnimValItem, "Attempting to modify read-only transform");
||||||| merged common ancestors
void
SVGTransform::SetMatrix(const gfxMatrix& aMatrix)
{
  MOZ_ASSERT(!mIsAnimValItem,
             "Attempting to modify read-only transform");
=======
    case SVG_TRANSFORM_SKEWY:
      result.SetSkewY(mParams[0]);
      break;
>>>>>>> upstream-releases

    default:
      MOZ_ASSERT_UNREACHABLE("Unexpected transform type");
      break;
  }
  return result;
}

<<<<<<< HEAD
}  // namespace dom
}  // namespace mozilla
||||||| merged common ancestors
} // namespace dom
} // namespace mozilla
=======
}  // namespace mozilla
>>>>>>> upstream-releases
