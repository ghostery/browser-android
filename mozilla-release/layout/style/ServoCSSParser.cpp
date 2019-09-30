/* -*- Mode: C++; tab-width: 8; indent-tabs-mode: nil; c-basic-offset: 2 -*- */
/* vim: set ts=8 sts=2 et sw=2 tw=80: */
/* This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/. */

/* CSS parsing utility functions */

#include "ServoCSSParser.h"

#include "mozilla/ServoBindings.h"
#include "mozilla/ServoStyleSet.h"
#include "mozilla/dom/Document.h"

using namespace mozilla;
using namespace mozilla::dom;

<<<<<<< HEAD
/* static */ bool ServoCSSParser::IsValidCSSColor(const nsAString& aValue) {
||||||| merged common ancestors
/* static */ bool
ServoCSSParser::IsValidCSSColor(const nsAString& aValue)
{
=======
/* static */
bool ServoCSSParser::IsValidCSSColor(const nsAString& aValue) {
>>>>>>> upstream-releases
  return Servo_IsValidCSSColor(&aValue);
}

<<<<<<< HEAD
/* static */ bool ServoCSSParser::ComputeColor(
    ServoStyleSet* aStyleSet, nscolor aCurrentColor, const nsAString& aValue,
    nscolor* aResultColor, bool* aWasCurrentColor, css::Loader* aLoader) {
||||||| merged common ancestors
/* static */ bool
ServoCSSParser::ComputeColor(ServoStyleSet* aStyleSet,
                             nscolor aCurrentColor,
                             const nsAString& aValue,
                             nscolor* aResultColor,
                             bool* aWasCurrentColor,
                             css::Loader* aLoader)
{
=======
/* static */
bool ServoCSSParser::ComputeColor(ServoStyleSet* aStyleSet,
                                  nscolor aCurrentColor,
                                  const nsAString& aValue,
                                  nscolor* aResultColor, bool* aWasCurrentColor,
                                  css::Loader* aLoader) {
>>>>>>> upstream-releases
  return Servo_ComputeColor(aStyleSet ? aStyleSet->RawSet() : nullptr,
                            aCurrentColor, &aValue, aResultColor,
                            aWasCurrentColor, aLoader);
}

<<<<<<< HEAD
/* static */ already_AddRefed<RawServoDeclarationBlock>
ServoCSSParser::ParseProperty(nsCSSPropertyID aProperty,
                              const nsAString& aValue,
                              const ParsingEnvironment& aParsingEnvironment,
                              ParsingMode aParsingMode) {
||||||| merged common ancestors
/* static */ already_AddRefed<RawServoDeclarationBlock>
ServoCSSParser::ParseProperty(nsCSSPropertyID aProperty,
                              const nsAString& aValue,
                              const ParsingEnvironment& aParsingEnvironment,
                              ParsingMode aParsingMode)
{
=======
/* static */
already_AddRefed<RawServoDeclarationBlock> ServoCSSParser::ParseProperty(
    nsCSSPropertyID aProperty, const nsAString& aValue,
    const ParsingEnvironment& aParsingEnvironment, ParsingMode aParsingMode) {
>>>>>>> upstream-releases
  NS_ConvertUTF16toUTF8 value(aValue);
  return Servo_ParseProperty(
             aProperty, &value, aParsingEnvironment.mUrlExtraData, aParsingMode,
             aParsingEnvironment.mCompatMode, aParsingEnvironment.mLoader)
      .Consume();
}

<<<<<<< HEAD
/* static */ bool ServoCSSParser::ParseEasing(const nsAString& aValue,
                                              URLExtraData* aUrl,
                                              nsTimingFunction& aResult) {
||||||| merged common ancestors
/* static */ bool
ServoCSSParser::ParseEasing(const nsAString& aValue,
                            URLExtraData* aUrl,
                            nsTimingFunction& aResult)
{
=======
/* static */
bool ServoCSSParser::ParseEasing(const nsAString& aValue, URLExtraData* aUrl,
                                 nsTimingFunction& aResult) {
>>>>>>> upstream-releases
  return Servo_ParseEasing(&aValue, aUrl, &aResult);
}

<<<<<<< HEAD
/* static */ bool ServoCSSParser::ParseTransformIntoMatrix(
    const nsAString& aValue, bool& aContains3DTransform,
    RawGeckoGfxMatrix4x4& aResult) {
  return Servo_ParseTransformIntoMatrix(&aValue, &aContains3DTransform,
                                        &aResult);
||||||| merged common ancestors
/* static */ bool
ServoCSSParser::ParseTransformIntoMatrix(const nsAString& aValue,
                                         bool& aContains3DTransform,
                                         RawGeckoGfxMatrix4x4& aResult)
{
  return Servo_ParseTransformIntoMatrix(&aValue,
                                        &aContains3DTransform,
                                        &aResult);
=======
/* static */
bool ServoCSSParser::ParseTransformIntoMatrix(const nsAString& aValue,
                                              bool& aContains3DTransform,
                                              gfx::Matrix4x4& aResult) {
  return Servo_ParseTransformIntoMatrix(&aValue, &aContains3DTransform,
                                        &aResult.components);
>>>>>>> upstream-releases
}

<<<<<<< HEAD
/* static */ bool ServoCSSParser::ParseFontShorthandForMatching(
    const nsAString& aValue, URLExtraData* aUrl, RefPtr<SharedFontList>& aList,
    StyleComputedFontStyleDescriptor& aStyle, float& aStretch, float& aWeight) {
  return Servo_ParseFontShorthandForMatching(&aValue, aUrl, &aList, &aStyle,
                                             &aStretch, &aWeight);
||||||| merged common ancestors
/* static */ bool
ServoCSSParser::ParseFontShorthandForMatching(const nsAString& aValue,
                                              URLExtraData* aUrl,
                                              RefPtr<SharedFontList>& aList,
                                              StyleComputedFontStyleDescriptor& aStyle,
                                              float& aStretch,
                                              float& aWeight)
{
  return Servo_ParseFontShorthandForMatching(&aValue, aUrl, &aList,
                                             &aStyle, &aStretch, &aWeight);
=======
/* static */
bool ServoCSSParser::ParseFontShorthandForMatching(
    const nsAString& aValue, URLExtraData* aUrl, RefPtr<SharedFontList>& aList,
    StyleComputedFontStyleDescriptor& aStyle, float& aStretch, float& aWeight) {
  return Servo_ParseFontShorthandForMatching(&aValue, aUrl, &aList, &aStyle,
                                             &aStretch, &aWeight);
>>>>>>> upstream-releases
}

<<<<<<< HEAD
/* static */ already_AddRefed<URLExtraData> ServoCSSParser::GetURLExtraData(
    nsIDocument* aDocument) {
||||||| merged common ancestors
/* static */ already_AddRefed<URLExtraData>
ServoCSSParser::GetURLExtraData(nsIDocument* aDocument)
{
=======
/* static */
already_AddRefed<URLExtraData> ServoCSSParser::GetURLExtraData(
    Document* aDocument) {
>>>>>>> upstream-releases
  MOZ_ASSERT(aDocument);

  // FIXME this is using the wrong base uri (bug 1343919)
  RefPtr<URLExtraData> url = new URLExtraData(
      aDocument->GetDocumentURI(), aDocument->GetDocumentURI(),
      aDocument->NodePrincipal(), aDocument->GetReferrerPolicy());
  return url.forget();
}

/* static */ ServoCSSParser::ParsingEnvironment
<<<<<<< HEAD
ServoCSSParser::GetParsingEnvironment(nsIDocument* aDocument) {
||||||| merged common ancestors
ServoCSSParser::GetParsingEnvironment(nsIDocument* aDocument)
{
=======
ServoCSSParser::GetParsingEnvironment(Document* aDocument) {
>>>>>>> upstream-releases
  return ParsingEnvironment(GetURLExtraData(aDocument),
                            aDocument->GetCompatibilityMode(),
                            aDocument->CSSLoader());
}
