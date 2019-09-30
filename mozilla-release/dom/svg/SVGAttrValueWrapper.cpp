/* -*- Mode: C++; tab-width: 8; indent-tabs-mode: nil; c-basic-offset: 2 -*- */
/* vim: set ts=8 sts=2 et sw=2 tw=80: */
/* This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/. */

#include "SVGAttrValueWrapper.h"

#include "SVGAnimatedIntegerPair.h"
#include "SVGAnimatedLength.h"
#include "SVGAnimatedNumberPair.h"
#include "SVGAnimatedOrient.h"
#include "SVGAnimatedPreserveAspectRatio.h"
#include "SVGAnimatedViewBox.h"
#include "SVGLengthList.h"
#include "SVGNumberList.h"
#include "SVGPathData.h"
#include "SVGPointList.h"
#include "SVGStringList.h"
#include "SVGTransformList.h"

namespace mozilla {

<<<<<<< HEAD
/*static*/ void SVGAttrValueWrapper::ToString(const nsSVGAngle* aAngle,
                                              nsAString& aResult) {
  aAngle->GetBaseValueString(aResult);
||||||| merged common ancestors
/*static*/ void
SVGAttrValueWrapper::ToString(const nsSVGAngle* aAngle, nsAString& aResult)
{
  aAngle->GetBaseValueString(aResult);
=======
/*static*/
void SVGAttrValueWrapper::ToString(const SVGAnimatedOrient* aOrient,
                                   nsAString& aResult) {
  aOrient->GetBaseValueString(aResult);
>>>>>>> upstream-releases
}

<<<<<<< HEAD
/*static*/ void SVGAttrValueWrapper::ToString(
    const nsSVGIntegerPair* aIntegerPair, nsAString& aResult) {
||||||| merged common ancestors
/*static*/ void
SVGAttrValueWrapper::ToString(const nsSVGIntegerPair* aIntegerPair,
                              nsAString& aResult)
{
=======
/*static*/
void SVGAttrValueWrapper::ToString(const SVGAnimatedIntegerPair* aIntegerPair,
                                   nsAString& aResult) {
>>>>>>> upstream-releases
  aIntegerPair->GetBaseValueString(aResult);
}

<<<<<<< HEAD
/*static*/ void SVGAttrValueWrapper::ToString(const nsSVGLength2* aLength,
                                              nsAString& aResult) {
||||||| merged common ancestors
/*static*/ void
SVGAttrValueWrapper::ToString(const nsSVGLength2* aLength, nsAString& aResult)
{
=======
/*static*/
void SVGAttrValueWrapper::ToString(const SVGAnimatedLength* aLength,
                                   nsAString& aResult) {
>>>>>>> upstream-releases
  aLength->GetBaseValueString(aResult);
}

<<<<<<< HEAD
/*static*/ void SVGAttrValueWrapper::ToString(const SVGLengthList* aLengthList,
                                              nsAString& aResult) {
||||||| merged common ancestors
/*static*/ void
SVGAttrValueWrapper::ToString(const SVGLengthList* aLengthList,
                              nsAString& aResult)
{
=======
/*static*/
void SVGAttrValueWrapper::ToString(const SVGLengthList* aLengthList,
                                   nsAString& aResult) {
>>>>>>> upstream-releases
  aLengthList->GetValueAsString(aResult);
}

<<<<<<< HEAD
/*static*/ void SVGAttrValueWrapper::ToString(const SVGNumberList* aNumberList,
                                              nsAString& aResult) {
||||||| merged common ancestors
/*static*/ void
SVGAttrValueWrapper::ToString(const SVGNumberList* aNumberList,
                              nsAString& aResult)
{
=======
/*static*/
void SVGAttrValueWrapper::ToString(const SVGNumberList* aNumberList,
                                   nsAString& aResult) {
>>>>>>> upstream-releases
  aNumberList->GetValueAsString(aResult);
}

<<<<<<< HEAD
/*static*/ void SVGAttrValueWrapper::ToString(
    const nsSVGNumberPair* aNumberPair, nsAString& aResult) {
||||||| merged common ancestors
/*static*/ void
SVGAttrValueWrapper::ToString(const nsSVGNumberPair* aNumberPair,
                              nsAString& aResult)
{
=======
/*static*/
void SVGAttrValueWrapper::ToString(const SVGAnimatedNumberPair* aNumberPair,
                                   nsAString& aResult) {
>>>>>>> upstream-releases
  aNumberPair->GetBaseValueString(aResult);
}

<<<<<<< HEAD
/*static*/ void SVGAttrValueWrapper::ToString(const SVGPathData* aPathData,
                                              nsAString& aResult) {
||||||| merged common ancestors
/*static*/ void
SVGAttrValueWrapper::ToString(const SVGPathData* aPathData, nsAString& aResult)
{
=======
/*static*/
void SVGAttrValueWrapper::ToString(const SVGPathData* aPathData,
                                   nsAString& aResult) {
>>>>>>> upstream-releases
  aPathData->GetValueAsString(aResult);
}

<<<<<<< HEAD
/*static*/ void SVGAttrValueWrapper::ToString(const SVGPointList* aPointList,
                                              nsAString& aResult) {
||||||| merged common ancestors
/*static*/ void
SVGAttrValueWrapper::ToString(const SVGPointList* aPointList,
                              nsAString& aResult)
{
=======
/*static*/
void SVGAttrValueWrapper::ToString(const SVGPointList* aPointList,
                                   nsAString& aResult) {
>>>>>>> upstream-releases
  aPointList->GetValueAsString(aResult);
}

<<<<<<< HEAD
/*static*/ void SVGAttrValueWrapper::ToString(
    const SVGAnimatedPreserveAspectRatio* aPreserveAspectRatio,
    nsAString& aResult) {
||||||| merged common ancestors
/*static*/ void
SVGAttrValueWrapper::ToString(
  const SVGAnimatedPreserveAspectRatio* aPreserveAspectRatio,
  nsAString& aResult)
{
=======
/*static*/
void SVGAttrValueWrapper::ToString(
    const SVGAnimatedPreserveAspectRatio* aPreserveAspectRatio,
    nsAString& aResult) {
>>>>>>> upstream-releases
  aPreserveAspectRatio->GetBaseValueString(aResult);
}

<<<<<<< HEAD
/*static*/ void SVGAttrValueWrapper::ToString(const SVGStringList* aStringList,
                                              nsAString& aResult) {
||||||| merged common ancestors
/*static*/ void
SVGAttrValueWrapper::ToString(const SVGStringList* aStringList,
                              nsAString& aResult)
{
=======
/*static*/
void SVGAttrValueWrapper::ToString(const SVGStringList* aStringList,
                                   nsAString& aResult) {
>>>>>>> upstream-releases
  aStringList->GetValue(aResult);
}

<<<<<<< HEAD
/*static*/ void SVGAttrValueWrapper::ToString(
    const SVGTransformList* aTransformList, nsAString& aResult) {
||||||| merged common ancestors
/*static*/ void
SVGAttrValueWrapper::ToString(const SVGTransformList* aTransformList,
                              nsAString& aResult)
{
=======
/*static*/
void SVGAttrValueWrapper::ToString(const SVGTransformList* aTransformList,
                                   nsAString& aResult) {
>>>>>>> upstream-releases
  aTransformList->GetValueAsString(aResult);
}

<<<<<<< HEAD
/*static*/ void SVGAttrValueWrapper::ToString(const nsSVGViewBox* aViewBox,
                                              nsAString& aResult) {
||||||| merged common ancestors
/*static*/ void
SVGAttrValueWrapper::ToString(const nsSVGViewBox* aViewBox, nsAString& aResult)
{
=======
/*static*/
void SVGAttrValueWrapper::ToString(const SVGAnimatedViewBox* aViewBox,
                                   nsAString& aResult) {
>>>>>>> upstream-releases
  aViewBox->GetBaseValueString(aResult);
}

}  // namespace mozilla
