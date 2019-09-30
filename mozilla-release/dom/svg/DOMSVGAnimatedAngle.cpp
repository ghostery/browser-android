/* -*- Mode: C++; tab-width: 8; indent-tabs-mode: nil; c-basic-offset: 2 -*- */
/* vim: set ts=8 sts=2 et sw=2 tw=80: */
/* This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/. */

#include "DOMSVGAnimatedAngle.h"

#include "SVGAnimatedOrient.h"
#include "mozilla/dom/SVGAnimatedAngleBinding.h"

using namespace mozilla;
using namespace mozilla::dom;

NS_SVG_VAL_IMPL_CYCLE_COLLECTION_WRAPPERCACHED(DOMSVGAnimatedAngle, mSVGElement)

NS_IMPL_CYCLE_COLLECTION_ROOT_NATIVE(DOMSVGAnimatedAngle, AddRef)
NS_IMPL_CYCLE_COLLECTION_UNROOT_NATIVE(DOMSVGAnimatedAngle, Release)

<<<<<<< HEAD:mozilla-release/dom/svg/SVGAnimatedAngle.cpp
JSObject* SVGAnimatedAngle::WrapObject(JSContext* aCx,
                                       JS::Handle<JSObject*> aGivenProto) {
||||||| merged common ancestors
JSObject*
SVGAnimatedAngle::WrapObject(JSContext* aCx, JS::Handle<JSObject*> aGivenProto)
{
=======
JSObject* DOMSVGAnimatedAngle::WrapObject(JSContext* aCx,
                                          JS::Handle<JSObject*> aGivenProto) {
>>>>>>> upstream-releases:mozilla-release/dom/svg/DOMSVGAnimatedAngle.cpp
  return SVGAnimatedAngle_Binding::Wrap(aCx, this, aGivenProto);
}

<<<<<<< HEAD:mozilla-release/dom/svg/SVGAnimatedAngle.cpp
already_AddRefed<SVGAngle> SVGAnimatedAngle::BaseVal() {
||||||| merged common ancestors
already_AddRefed<SVGAngle>
SVGAnimatedAngle::BaseVal()
{
=======
already_AddRefed<DOMSVGAngle> DOMSVGAnimatedAngle::BaseVal() {
>>>>>>> upstream-releases:mozilla-release/dom/svg/DOMSVGAnimatedAngle.cpp
  return mVal->ToDOMBaseVal(mSVGElement);
}

<<<<<<< HEAD:mozilla-release/dom/svg/SVGAnimatedAngle.cpp
already_AddRefed<SVGAngle> SVGAnimatedAngle::AnimVal() {
||||||| merged common ancestors
already_AddRefed<SVGAngle>
SVGAnimatedAngle::AnimVal()
{
=======
already_AddRefed<DOMSVGAngle> DOMSVGAnimatedAngle::AnimVal() {
>>>>>>> upstream-releases:mozilla-release/dom/svg/DOMSVGAnimatedAngle.cpp
  return mVal->ToDOMAnimVal(mSVGElement);
}
