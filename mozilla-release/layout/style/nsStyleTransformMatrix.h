/* -*- Mode: C++; tab-width: 8; indent-tabs-mode: nil; c-basic-offset: 2 -*- */
/* vim: set ts=8 sts=2 et sw=2 tw=80: */
/* This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/. */

/*
 * A class representing three matrices that can be used for style transforms.
 */

#ifndef nsStyleTransformMatrix_h_
#define nsStyleTransformMatrix_h_

#include "gfxPoint.h"
#include "mozilla/gfx/Matrix.h"
#include "mozilla/EnumeratedArray.h"
#include "nsSize.h"

#include <limits>

class nsIFrame;
class nsPresContext;
struct gfxQuaternion;
struct nsRect;

namespace mozilla {
struct MotionPathData;
}

/**
 * A helper to generate gfxMatrixes from css transform functions.
 */
namespace nsStyleTransformMatrix {
// The operator passed to Servo backend.
enum class MatrixTransformOperator : uint8_t { Interpolate, Accumulate };

// Function for applying perspective() transform function. We treat
// any value smaller than epsilon as perspective(infinity), which
// follows CSSWG's resolution on perspective(0). See bug 1316236.
inline void ApplyPerspectiveToMatrix(mozilla::gfx::Matrix4x4& aMatrix,
                                     float aDepth) {
  if (aDepth >= std::numeric_limits<float>::epsilon()) {
    aMatrix.Perspective(aDepth);
  }
}

<<<<<<< HEAD
/**
 * This class provides on-demand access to the 'reference box' for CSS
 * transforms (needed to resolve percentage values in 'transform',
 * 'transform-origin', etc.):
 *
 *    http://dev.w3.org/csswg/css-transforms/#reference-box
 *
 * This class helps us to avoid calculating the reference box unless and
 * until it is actually needed. This is important for performance when
 * transforms are applied to SVG elements since the reference box for SVG is
 * much more expensive to calculate (than for elements with a CSS layout box
 * where we can use the nsIFrame's cached mRect), much more common (than on
 * HTML), and yet very rarely have percentage values that require the
 * reference box to be resolved. We also don't want to cause SVG frames to
 * cache lots of ObjectBoundingBoxProperty objects that aren't needed.
 *
 * If UNIFIED_CONTINUATIONS (experimental, and currently broke) is defined,
 * we consider the reference box for non-SVG frames to be the smallest
 * rectangle containing a frame and all of its continuations.  For example,
 * if there is a <span> element with several continuations split over
 * several lines, this function will return the rectangle containing all of
 * those continuations. (This behavior is not currently in a spec.)
 */
class MOZ_STACK_CLASS TransformReferenceBox final {
 public:
  typedef nscoord (TransformReferenceBox::*DimensionGetter)();

  explicit TransformReferenceBox()
      : mFrame(nullptr),
        mX(0),
        mY(0),
        mWidth(0),
        mHeight(0),
        mIsCached(false) {}

  explicit TransformReferenceBox(const nsIFrame* aFrame)
      : mFrame(aFrame), mX(0), mY(0), mWidth(0), mHeight(0), mIsCached(false) {
    MOZ_ASSERT(mFrame);
  }

  explicit TransformReferenceBox(const nsIFrame* aFrame,
                                 const nsSize& aFallbackDimensions)
      : mX(0), mY(0), mWidth(0), mHeight(0) {
    mFrame = aFrame;
    mIsCached = false;
    if (!mFrame) {
      Init(aFallbackDimensions);
    }
  }

  void Init(const nsIFrame* aFrame) {
    MOZ_ASSERT(!mFrame && !mIsCached);
    mFrame = aFrame;
  }

  void Init(const nsSize& aDimensions);

  /**
   * The offset of the reference box from the nsIFrame's TopLeft(). This
   * is non-zero only in the case of SVG content. If we can successfully
   * implement UNIFIED_CONTINUATIONS at some point in the future then it
   * may also be non-zero for non-SVG content.
   */
  nscoord X() {
    EnsureDimensionsAreCached();
    return mX;
  }
  nscoord Y() {
    EnsureDimensionsAreCached();
    return mY;
  }
||||||| merged common ancestors
  /**
   * This class provides on-demand access to the 'reference box' for CSS
   * transforms (needed to resolve percentage values in 'transform',
   * 'transform-origin', etc.):
   *
   *    http://dev.w3.org/csswg/css-transforms/#reference-box
   *
   * This class helps us to avoid calculating the reference box unless and
   * until it is actually needed. This is important for performance when
   * transforms are applied to SVG elements since the reference box for SVG is
   * much more expensive to calculate (than for elements with a CSS layout box
   * where we can use the nsIFrame's cached mRect), much more common (than on
   * HTML), and yet very rarely have percentage values that require the
   * reference box to be resolved. We also don't want to cause SVG frames to
   * cache lots of ObjectBoundingBoxProperty objects that aren't needed.
   *
   * If UNIFIED_CONTINUATIONS (experimental, and currently broke) is defined,
   * we consider the reference box for non-SVG frames to be the smallest
   * rectangle containing a frame and all of its continuations.  For example,
   * if there is a <span> element with several continuations split over
   * several lines, this function will return the rectangle containing all of
   * those continuations. (This behavior is not currently in a spec.)
   */
  class MOZ_STACK_CLASS TransformReferenceBox final {
  public:
    typedef nscoord (TransformReferenceBox::*DimensionGetter)();

    explicit TransformReferenceBox()
      : mFrame(nullptr)
      , mX(0)
      , mY(0)
      , mWidth(0)
      , mHeight(0)
      , mIsCached(false)
    {}

    explicit TransformReferenceBox(const nsIFrame* aFrame)
      : mFrame(aFrame)
      , mX(0)
      , mY(0)
      , mWidth(0)
      , mHeight(0)
      , mIsCached(false)
    {
      MOZ_ASSERT(mFrame);
    }

    explicit TransformReferenceBox(const nsIFrame* aFrame,
                                   const nsSize& aFallbackDimensions)
      : mX(0)
      , mY(0)
      , mWidth(0)
      , mHeight(0)
    {
      mFrame = aFrame;
      mIsCached = false;
      if (!mFrame) {
        Init(aFallbackDimensions);
      }
    }

    void Init(const nsIFrame* aFrame) {
      MOZ_ASSERT(!mFrame && !mIsCached);
      mFrame = aFrame;
    }

    void Init(const nsSize& aDimensions);

    /**
     * The offset of the reference box from the nsIFrame's TopLeft(). This
     * is non-zero only in the case of SVG content. If we can successfully
     * implement UNIFIED_CONTINUATIONS at some point in the future then it
     * may also be non-zero for non-SVG content.
     */
    nscoord X() {
      EnsureDimensionsAreCached();
      return mX;
    }
    nscoord Y() {
      EnsureDimensionsAreCached();
      return mY;
    }

    /**
     * The size of the reference box.
     */
    nscoord Width() {
      EnsureDimensionsAreCached();
      return mWidth;
    }
    nscoord Height() {
      EnsureDimensionsAreCached();
      return mHeight;
    }
=======
/**
 * This class provides on-demand access to the 'reference box' for CSS
 * transforms (needed to resolve percentage values in 'transform',
 * 'transform-origin', etc.):
 *
 *    http://dev.w3.org/csswg/css-transforms/#reference-box
 *
 * This class helps us to avoid calculating the reference box unless and
 * until it is actually needed. This is important for performance when
 * transforms are applied to SVG elements since the reference box for SVG is
 * much more expensive to calculate (than for elements with a CSS layout box
 * where we can use the nsIFrame's cached mRect), much more common (than on
 * HTML), and yet very rarely have percentage values that require the
 * reference box to be resolved. We also don't want to cause SVG frames to
 * cache lots of ObjectBoundingBoxProperty objects that aren't needed.
 *
 * If UNIFIED_CONTINUATIONS (experimental, and currently broke) is defined,
 * we consider the reference box for non-SVG frames to be the smallest
 * rectangle containing a frame and all of its continuations.  For example,
 * if there is a <span> element with several continuations split over
 * several lines, this function will return the rectangle containing all of
 * those continuations. (This behavior is not currently in a spec.)
 */
class MOZ_STACK_CLASS TransformReferenceBox final {
 public:
  typedef nscoord (TransformReferenceBox::*DimensionGetter)();

  explicit TransformReferenceBox()
      : mFrame(nullptr),
        mX(0),
        mY(0),
        mWidth(0),
        mHeight(0),
        mIsCached(false) {}

  explicit TransformReferenceBox(const nsIFrame* aFrame)
      : mFrame(aFrame), mX(0), mY(0), mWidth(0), mHeight(0), mIsCached(false) {
    MOZ_ASSERT(mFrame);
  }
>>>>>>> upstream-releases

<<<<<<< HEAD
  /**
   * The size of the reference box.
   */
  nscoord Width() {
    EnsureDimensionsAreCached();
    return mWidth;
  }
  nscoord Height() {
    EnsureDimensionsAreCached();
    return mHeight;
  }
||||||| merged common ancestors
    bool IsEmpty() {
      return !mFrame;
    }
=======
  explicit TransformReferenceBox(const nsIFrame* aFrame,
                                 const nsSize& aFallbackDimensions)
      : mX(0), mY(0), mWidth(0), mHeight(0) {
    mFrame = aFrame;
    mIsCached = false;
    if (!mFrame) {
      Init(aFallbackDimensions);
    }
  }
>>>>>>> upstream-releases

<<<<<<< HEAD
  bool IsEmpty() { return !mFrame; }
||||||| merged common ancestors
  private:
    // We don't really need to prevent copying, but since none of our consumers
    // currently need to copy, preventing copying may allow us to catch some
    // cases where we use pass-by-value instead of pass-by-reference.
    TransformReferenceBox(const TransformReferenceBox&) = delete;
=======
  void Init(const nsIFrame* aFrame) {
    MOZ_ASSERT(!mFrame && !mIsCached);
    mFrame = aFrame;
  }
>>>>>>> upstream-releases

<<<<<<< HEAD
 private:
  // We don't really need to prevent copying, but since none of our consumers
  // currently need to copy, preventing copying may allow us to catch some
  // cases where we use pass-by-value instead of pass-by-reference.
  TransformReferenceBox(const TransformReferenceBox&) = delete;
||||||| merged common ancestors
    void EnsureDimensionsAreCached();
=======
  void Init(const nsSize& aDimensions);
>>>>>>> upstream-releases

<<<<<<< HEAD
  void EnsureDimensionsAreCached();
||||||| merged common ancestors
    const nsIFrame* mFrame;
    nscoord mX, mY, mWidth, mHeight;
    bool mIsCached;
  };
=======
  /**
   * The offset of the reference box from the nsIFrame's TopLeft(). This
   * is non-zero only in the case of SVG content. If we can successfully
   * implement UNIFIED_CONTINUATIONS at some point in the future then it
   * may also be non-zero for non-SVG content.
   */
  nscoord X() {
    EnsureDimensionsAreCached();
    return mX;
  }
  nscoord Y() {
    EnsureDimensionsAreCached();
    return mY;
  }
>>>>>>> upstream-releases

<<<<<<< HEAD
  const nsIFrame* mFrame;
  nscoord mX, mY, mWidth, mHeight;
  bool mIsCached;
};
||||||| merged common ancestors
  /**
   * Return the transform function, as an nsCSSKeyword, for the given
   * nsCSSValue::Array from a transform list.
   */
  nsCSSKeyword TransformFunctionOf(const nsCSSValue::Array* aData);
=======
  /**
   * The size of the reference box.
   */
  nscoord Width() {
    EnsureDimensionsAreCached();
    return mWidth;
  }
  nscoord Height() {
    EnsureDimensionsAreCached();
    return mHeight;
  }
>>>>>>> upstream-releases

<<<<<<< HEAD
/**
 * Return the transform function, as an nsCSSKeyword, for the given
 * nsCSSValue::Array from a transform list.
 */
nsCSSKeyword TransformFunctionOf(const nsCSSValue::Array* aData);
||||||| merged common ancestors
  void SetIdentityMatrix(nsCSSValue::Array* aMatrix);
=======
  bool IsEmpty() { return !mFrame; }
>>>>>>> upstream-releases

<<<<<<< HEAD
void SetIdentityMatrix(nsCSSValue::Array* aMatrix);
||||||| merged common ancestors
  float ProcessTranslatePart(const nsCSSValue& aValue,
                             TransformReferenceBox* aRefBox,
                             TransformReferenceBox::DimensionGetter aDimensionGetter = nullptr);
=======
 private:
  // We don't really need to prevent copying, but since none of our consumers
  // currently need to copy, preventing copying may allow us to catch some
  // cases where we use pass-by-value instead of pass-by-reference.
  TransformReferenceBox(const TransformReferenceBox&) = delete;
>>>>>>> upstream-releases

<<<<<<< HEAD
float ProcessTranslatePart(
    const nsCSSValue& aValue, TransformReferenceBox* aRefBox,
    TransformReferenceBox::DimensionGetter aDimensionGetter = nullptr);
||||||| merged common ancestors
  void
  ProcessInterpolateMatrix(mozilla::gfx::Matrix4x4& aMatrix,
                           const nsCSSValue::Array* aData,
                           TransformReferenceBox& aBounds,
                           bool* aContains3dTransform);
=======
  void EnsureDimensionsAreCached();
>>>>>>> upstream-releases

<<<<<<< HEAD
void ProcessInterpolateMatrix(mozilla::gfx::Matrix4x4& aMatrix,
                              const nsCSSValue::Array* aData,
                              TransformReferenceBox& aBounds);
||||||| merged common ancestors
  void
  ProcessAccumulateMatrix(mozilla::gfx::Matrix4x4& aMatrix,
                          const nsCSSValue::Array* aData,
                          TransformReferenceBox& aBounds,
                          bool* aContains3dTransform);
=======
  const nsIFrame* mFrame;
  nscoord mX, mY, mWidth, mHeight;
  bool mIsCached;
};
>>>>>>> upstream-releases

<<<<<<< HEAD
void ProcessAccumulateMatrix(mozilla::gfx::Matrix4x4& aMatrix,
                             const nsCSSValue::Array* aData,
                             TransformReferenceBox& aBounds);
||||||| merged common ancestors
=======
float ProcessTranslatePart(
    const mozilla::LengthPercentage& aValue, TransformReferenceBox* aRefBox,
    TransformReferenceBox::DimensionGetter aDimensionGetter = nullptr);
>>>>>>> upstream-releases

<<<<<<< HEAD
/**
 * Given an nsCSSValueList containing -moz-transform functions,
 * returns a matrix containing the value of those functions.
 *
 * @param aData The nsCSSValueList containing the transform functions
 * @param aBounds The frame's bounding rectangle.
 * @param aAppUnitsPerMatrixUnit The number of app units per device pixel.
 *
 * eCSSUnit_Pixel (as they are in an StyleAnimationValue)
 */
mozilla::gfx::Matrix4x4 ReadTransforms(const nsCSSValueList* aList,
                                       TransformReferenceBox& aBounds,
                                       float aAppUnitsPerMatrixUnit);

// Generate the gfx::Matrix for CSS Transform Module Level 2.
// https://drafts.csswg.org/css-transforms-2/#ctm
mozilla::gfx::Matrix4x4 ReadTransforms(
    const nsCSSValueList* aIndividualTransforms,
    const mozilla::Maybe<mozilla::MotionPathData>& aMotion,
    const nsCSSValueList* aTransform, TransformReferenceBox& aRefBox,
    float aAppUnitsPerMatrixUnit);
||||||| merged common ancestors
  /**
   * Given an nsCSSValueList containing -moz-transform functions,
   * returns a matrix containing the value of those functions.
   *
   * @param aData The nsCSSValueList containing the transform functions
   * @param aBounds The frame's bounding rectangle.
   * @param aAppUnitsPerMatrixUnit The number of app units per device pixel.
   * @param aContains3dTransform [out] Set to true if aList contains at least
   *   one 3d transform function (as defined in the CSS transforms
   *   specification), false otherwise.
   *
   * eCSSUnit_Pixel (as they are in an StyleAnimationValue)
   */
  mozilla::gfx::Matrix4x4 ReadTransforms(const nsCSSValueList* aList,
                                         TransformReferenceBox& aBounds,
                                         float aAppUnitsPerMatrixUnit,
                                         bool* aContains3dTransform);

  // Generate the gfx::Matrix for CSS Transform Module Level 2.
  // https://drafts.csswg.org/css-transforms-2/#ctm
  mozilla::gfx::Matrix4x4
  ReadTransforms(const nsCSSValueList* aIndividualTransforms,
                 const mozilla::Maybe<mozilla::MotionPathData>& aMotion,
                 const nsCSSValueList* aTransform,
                 TransformReferenceBox& aRefBox,
                 float aAppUnitsPerMatrixUnit,
                 bool* aContains3dTransform);
=======
void ProcessInterpolateMatrix(mozilla::gfx::Matrix4x4& aMatrix,
                              const mozilla::StyleTransformOperation& aOp,
                              TransformReferenceBox& aBounds);

void ProcessAccumulateMatrix(mozilla::gfx::Matrix4x4& aMatrix,
                             const mozilla::StyleTransformOperation& aOp,
                             TransformReferenceBox& aBounds);
>>>>>>> upstream-releases

<<<<<<< HEAD
/**
 * Given two nsStyleCoord values, compute the 2d position with respect to the
 * given TransformReferenceBox that these values describe, in device pixels.
 */
mozilla::gfx::Point Convert2DPosition(nsStyleCoord const (&aValue)[2],
                                      TransformReferenceBox& aRefBox,
                                      int32_t aAppUnitsPerDevPixel);

// Shear type for decomposition.
enum class ShearType { XYSHEAR, XZSHEAR, YZSHEAR, Count };
using ShearArray = mozilla::EnumeratedArray<ShearType, ShearType::Count, float>;

/*
 * Implements the 2d transform matrix decomposition algorithm.
 */
bool Decompose2DMatrix(const mozilla::gfx::Matrix& aMatrix,
                       mozilla::gfx::Point3D& aScale, ShearArray& aShear,
                       gfxQuaternion& aRotate,
                       mozilla::gfx::Point3D& aTranslate);
/*
 * Implements the 3d transform matrix decomposition algorithm.
 */
bool Decompose3DMatrix(const mozilla::gfx::Matrix4x4& aMatrix,
                       mozilla::gfx::Point3D& aScale, ShearArray& aShear,
                       gfxQuaternion& aRotate,
                       mozilla::gfx::Point3D& aTranslate,
                       mozilla::gfx::Point4D& aPerspective);

mozilla::gfx::Matrix CSSValueArrayTo2DMatrix(nsCSSValue::Array* aArray);
mozilla::gfx::Matrix4x4 CSSValueArrayTo3DMatrix(nsCSSValue::Array* aArray);

mozilla::gfx::Size GetScaleValue(const nsCSSValueSharedList* aList,
                                 const nsIFrame* aForFrame);
}  // namespace nsStyleTransformMatrix
||||||| merged common ancestors
  /**
   * Given two nsStyleCoord values, compute the 2d position with respect to the
   * given TransformReferenceBox that these values describe, in device pixels.
   */
  mozilla::gfx::Point Convert2DPosition(nsStyleCoord const (&aValue)[2],
                                        TransformReferenceBox& aRefBox,
                                        int32_t aAppUnitsPerDevPixel);

  // Shear type for decomposition.
  enum class ShearType {
    XYSHEAR,
    XZSHEAR,
    YZSHEAR,
    Count
  };
  using ShearArray =
    mozilla::EnumeratedArray<ShearType, ShearType::Count, float>;

  /*
   * Implements the 2d transform matrix decomposition algorithm.
   */
  bool Decompose2DMatrix(const mozilla::gfx::Matrix& aMatrix,
                         mozilla::gfx::Point3D& aScale,
                         ShearArray& aShear,
                         gfxQuaternion& aRotate,
                         mozilla::gfx::Point3D& aTranslate);
  /*
   * Implements the 3d transform matrix decomposition algorithm.
   */
  bool Decompose3DMatrix(const mozilla::gfx::Matrix4x4& aMatrix,
                         mozilla::gfx::Point3D& aScale,
                         ShearArray& aShear,
                         gfxQuaternion& aRotate,
                         mozilla::gfx::Point3D& aTranslate,
                         mozilla::gfx::Point4D& aPerspective);

  mozilla::gfx::Matrix CSSValueArrayTo2DMatrix(nsCSSValue::Array* aArray);
  mozilla::gfx::Matrix4x4 CSSValueArrayTo3DMatrix(nsCSSValue::Array* aArray);

  mozilla::gfx::Size GetScaleValue(const nsCSSValueSharedList* aList,
                                   const nsIFrame* aForFrame);
} // namespace nsStyleTransformMatrix
=======
/**
 * Given a StyleTransform containing transform functions, returns a matrix
 * containing the value of those functions.
 *
 * @param aList the transform operation list.
 * @param aBounds The frame's bounding rectangle.
 * @param aAppUnitsPerMatrixUnit The number of app units per device pixel.
 */
mozilla::gfx::Matrix4x4 ReadTransforms(const mozilla::StyleTransform& aList,
                                       TransformReferenceBox& aBounds,
                                       float aAppUnitsPerMatrixUnit);

// Generate the gfx::Matrix for CSS Transform Module Level 2.
// https://drafts.csswg.org/css-transforms-2/#ctm
mozilla::gfx::Matrix4x4 ReadTransforms(
    const mozilla::StyleTranslate&, const mozilla::StyleRotate&,
    const mozilla::StyleScale&,
    const mozilla::Maybe<mozilla::MotionPathData>& aMotion,
    const mozilla::StyleTransform&, TransformReferenceBox& aRefBox,
    float aAppUnitsPerMatrixUnit);

/**
 * Given the x and y values, compute the 2d position with respect to the given
 * TransformReferenceBox that these values describe, in CSS pixels.
 */
mozilla::CSSPoint Convert2DPosition(const mozilla::LengthPercentage& aX,
                                    const mozilla::LengthPercentage& aY,
                                    TransformReferenceBox& aRefBox);

/**
 * Given the x and y values, compute the 2d position with respect to the given
 * TransformReferenceBox that these values describe, in device pixels.
 */
mozilla::gfx::Point Convert2DPosition(const mozilla::LengthPercentage& aX,
                                      const mozilla::LengthPercentage& aY,
                                      TransformReferenceBox& aRefBox,
                                      int32_t aAppUnitsPerDevPixel);

// Shear type for decomposition.
enum class ShearType { XY, XZ, YZ, Count };
using ShearArray = mozilla::EnumeratedArray<ShearType, ShearType::Count, float>;

/*
 * Implements the 2d transform matrix decomposition algorithm.
 */
bool Decompose2DMatrix(const mozilla::gfx::Matrix& aMatrix,
                       mozilla::gfx::Point3D& aScale, ShearArray& aShear,
                       gfxQuaternion& aRotate,
                       mozilla::gfx::Point3D& aTranslate);
/*
 * Implements the 3d transform matrix decomposition algorithm.
 */
bool Decompose3DMatrix(const mozilla::gfx::Matrix4x4& aMatrix,
                       mozilla::gfx::Point3D& aScale, ShearArray& aShear,
                       gfxQuaternion& aRotate,
                       mozilla::gfx::Point3D& aTranslate,
                       mozilla::gfx::Point4D& aPerspective);

}  // namespace nsStyleTransformMatrix
>>>>>>> upstream-releases

#endif
