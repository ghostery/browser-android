/* -*- Mode: C++; tab-width: 8; indent-tabs-mode: nil; c-basic-offset: 2 -*- */
/* vim: set ts=8 sts=2 et sw=2 tw=80: */
/* This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/. */

#ifndef SVGTransformableElement_h
#define SVGTransformableElement_h

#include "nsAutoPtr.h"
#include "SVGAnimatedTransformList.h"
#include "gfxMatrix.h"
#include "mozilla/Attributes.h"
#include "mozilla/dom/SVGElement.h"
#include "mozilla/gfx/Matrix.h"

namespace mozilla {
namespace dom {

class DOMSVGAnimatedTransformList;
class SVGGraphicsElement;
class SVGMatrix;
class SVGRect;
struct SVGBoundingBoxOptions;

<<<<<<< HEAD
class SVGTransformableElement : public nsSVGElement {
 public:
||||||| merged common ancestors
class SVGTransformableElement : public nsSVGElement
{
public:
=======
class SVGTransformableElement : public SVGElement {
 public:
>>>>>>> upstream-releases
  explicit SVGTransformableElement(already_AddRefed<dom::NodeInfo>&& aNodeInfo)
<<<<<<< HEAD
      : nsSVGElement(std::move(aNodeInfo)) {}
  virtual ~SVGTransformableElement() {}
||||||| merged common ancestors
    : nsSVGElement(std::move(aNodeInfo)) {}
  virtual ~SVGTransformableElement() {}
=======
      : SVGElement(std::move(aNodeInfo)) {}
  virtual ~SVGTransformableElement() = default;
>>>>>>> upstream-releases

  virtual nsresult Clone(dom::NodeInfo*, nsINode** aResult) const override = 0;

  // WebIDL
  already_AddRefed<DOMSVGAnimatedTransformList> Transform();
  SVGElement* GetNearestViewportElement();
  SVGElement* GetFarthestViewportElement();
  MOZ_CAN_RUN_SCRIPT
  already_AddRefed<SVGRect> GetBBox(const SVGBoundingBoxOptions& aOptions,
                                    ErrorResult& rv);
  already_AddRefed<SVGMatrix> GetCTM();
  already_AddRefed<SVGMatrix> GetScreenCTM();
  already_AddRefed<SVGMatrix> GetTransformToElement(
      SVGGraphicsElement& aElement, ErrorResult& rv);

  // nsIContent interface
  NS_IMETHOD_(bool) IsAttributeMapped(const nsAtom* aAttribute) const override;

  nsChangeHint GetAttributeChangeHint(const nsAtom* aAttribute,
                                      int32_t aModType) const override;

<<<<<<< HEAD
  // nsSVGElement overrides
||||||| merged common ancestors

  // nsSVGElement overrides
=======
  // SVGElement overrides
>>>>>>> upstream-releases
  virtual bool IsEventAttributeNameInternal(nsAtom* aName) override;

  virtual gfxMatrix PrependLocalTransformsTo(
      const gfxMatrix& aMatrix,
      SVGTransformTypes aWhich = eAllTransforms) const override;
  virtual const gfx::Matrix* GetAnimateMotionTransform() const override;
  virtual void SetAnimateMotionTransform(const gfx::Matrix* aMatrix) override;

<<<<<<< HEAD
  virtual nsSVGAnimatedTransformList* GetAnimatedTransformList(
      uint32_t aFlags = 0) override;
||||||| merged common ancestors
  virtual nsSVGAnimatedTransformList*
    GetAnimatedTransformList(uint32_t aFlags = 0) override;
=======
  virtual SVGAnimatedTransformList* GetAnimatedTransformList(
      uint32_t aFlags = 0) override;
>>>>>>> upstream-releases
  virtual nsStaticAtom* GetTransformListAttrName() const override {
    return nsGkAtoms::transform;
  }

  virtual bool IsTransformable() override { return true; }

 protected:
  /**
   * Helper for overrides of PrependLocalTransformsTo.  If both arguments are
   * provided they are multiplied in the order in which the arguments appear,
   * and the result is returned.  If neither argument is provided, the identity
   * matrix is returned.  If only one argument is provided its transform is
   * returned.
   */
  static gfxMatrix GetUserToParentTransform(
<<<<<<< HEAD
      const gfx::Matrix* aAnimateMotionTransform,
      const nsSVGAnimatedTransformList* aTransforms);
||||||| merged common ancestors
                     const gfx::Matrix* aAnimateMotionTransform,
                     const nsSVGAnimatedTransformList* aTransforms);
=======
      const gfx::Matrix* aAnimateMotionTransform,
      const SVGAnimatedTransformList* aTransforms);
>>>>>>> upstream-releases

  nsAutoPtr<SVGAnimatedTransformList> mTransforms;

  // XXX maybe move this to property table, to save space on un-animated elems?
  nsAutoPtr<gfx::Matrix> mAnimateMotionTransform;
};

}  // namespace dom
}  // namespace mozilla

#endif  // SVGTransformableElement_h
