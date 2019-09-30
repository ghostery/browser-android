/* -*- Mode: C++; tab-width: 8; indent-tabs-mode: nil; c-basic-offset: 2 -*- */
/* vim: set ts=8 sts=2 et sw=2 tw=80: */
/* This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/. */

#include "SVGDataParser.h"
#include "nsContentUtils.h"
#include "SVGContentUtils.h"

<<<<<<< HEAD:mozilla-release/dom/svg/nsSVGDataParser.cpp
nsSVGDataParser::nsSVGDataParser(const nsAString& aValue)
    : mIter(SVGContentUtils::GetStartRangedPtr(aValue)),
      mEnd(SVGContentUtils::GetEndRangedPtr(aValue)) {}
||||||| merged common ancestors
nsSVGDataParser::nsSVGDataParser(const nsAString& aValue)
  : mIter(SVGContentUtils::GetStartRangedPtr(aValue)),
    mEnd(SVGContentUtils::GetEndRangedPtr(aValue))
{
}
=======
namespace mozilla {

SVGDataParser::SVGDataParser(const nsAString& aValue)
    : mIter(SVGContentUtils::GetStartRangedPtr(aValue)),
      mEnd(SVGContentUtils::GetEndRangedPtr(aValue)) {}
>>>>>>> upstream-releases:mozilla-release/dom/svg/SVGDataParser.cpp

<<<<<<< HEAD:mozilla-release/dom/svg/nsSVGDataParser.cpp
bool nsSVGDataParser::SkipCommaWsp() {
||||||| merged common ancestors
bool
nsSVGDataParser::SkipCommaWsp()
{
=======
bool SVGDataParser::SkipCommaWsp() {
>>>>>>> upstream-releases:mozilla-release/dom/svg/SVGDataParser.cpp
  if (!SkipWsp()) {
    // end of string
    return false;
  }
  if (*mIter != ',') {
    return true;
  }
  ++mIter;
  return SkipWsp();
}

<<<<<<< HEAD:mozilla-release/dom/svg/nsSVGDataParser.cpp
bool nsSVGDataParser::SkipWsp() {
||||||| merged common ancestors
bool
nsSVGDataParser::SkipWsp()
{
=======
bool SVGDataParser::SkipWsp() {
>>>>>>> upstream-releases:mozilla-release/dom/svg/SVGDataParser.cpp
  while (mIter != mEnd) {
    if (!nsContentUtils::IsHTMLWhitespace(*mIter)) {
      return true;
    }
    ++mIter;
  }
  return false;
}

}  // namespace mozilla
