/* -*- Mode: C++; tab-width: 8; indent-tabs-mode: nil; c-basic-offset: 2 -*- */
/* vim: set ts=8 sts=2 et sw=2 tw=80: */
/* This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/. */

#include "mozilla/dom/CSSMediaRule.h"

#include "mozilla/dom/CSSMediaRuleBinding.h"
#include "mozilla/dom/MediaList.h"
#include "mozilla/ServoBindings.h"

namespace mozilla {
namespace dom {

CSSMediaRule::CSSMediaRule(RefPtr<RawServoMediaRule> aRawRule,
                           StyleSheet* aSheet, css::Rule* aParentRule,
                           uint32_t aLine, uint32_t aColumn)
    : ConditionRule(Servo_MediaRule_GetRules(aRawRule).Consume(), aSheet,
                    aParentRule, aLine, aColumn),
      mRawRule(std::move(aRawRule)) {}

CSSMediaRule::~CSSMediaRule() {
  if (mMediaList) {
    mMediaList->SetStyleSheet(nullptr);
  }
}

NS_IMPL_ADDREF_INHERITED(CSSMediaRule, css::ConditionRule)
NS_IMPL_RELEASE_INHERITED(CSSMediaRule, css::ConditionRule)

// QueryInterface implementation for MediaRule
NS_INTERFACE_MAP_BEGIN_CYCLE_COLLECTION(CSSMediaRule)
NS_INTERFACE_MAP_END_INHERITING(css::ConditionRule)

NS_IMPL_CYCLE_COLLECTION_CLASS(CSSMediaRule)

NS_IMPL_CYCLE_COLLECTION_UNLINK_BEGIN_INHERITED(CSSMediaRule,
                                                css::ConditionRule)
  if (tmp->mMediaList) {
    tmp->mMediaList->SetStyleSheet(nullptr);
    tmp->mMediaList = nullptr;
  }
NS_IMPL_CYCLE_COLLECTION_UNLINK_END

NS_IMPL_CYCLE_COLLECTION_TRAVERSE_BEGIN_INHERITED(CSSMediaRule,
                                                  css::ConditionRule)
  NS_IMPL_CYCLE_COLLECTION_TRAVERSE(mMediaList)
NS_IMPL_CYCLE_COLLECTION_TRAVERSE_END

<<<<<<< HEAD
/* virtual */ void CSSMediaRule::DropSheetReference() {
||||||| merged common ancestors
/* virtual */ void
CSSMediaRule::DropSheetReference()
{
=======
/* virtual */
void CSSMediaRule::DropSheetReference() {
>>>>>>> upstream-releases
  if (mMediaList) {
    mMediaList->SetStyleSheet(nullptr);
  }
  ConditionRule::DropSheetReference();
}

#ifdef DEBUG
<<<<<<< HEAD
/* virtual */ void CSSMediaRule::List(FILE* out, int32_t aIndent) const {
||||||| merged common ancestors
/* virtual */ void
CSSMediaRule::List(FILE* out, int32_t aIndent) const
{
=======
/* virtual */
void CSSMediaRule::List(FILE* out, int32_t aIndent) const {
>>>>>>> upstream-releases
  nsAutoCString str;
  for (int32_t i = 0; i < aIndent; i++) {
    str.AppendLiteral("  ");
  }
  Servo_MediaRule_Debug(mRawRule, &str);
  fprintf_stderr(out, "%s\n", str.get());
}
#endif

void CSSMediaRule::GetConditionText(nsAString& aConditionText) {
  Media()->GetMediaText(aConditionText);
}

<<<<<<< HEAD
void CSSMediaRule::SetConditionText(const nsAString& aConditionText,
                                    ErrorResult& aRv) {
||||||| merged common ancestors
void
CSSMediaRule::SetConditionText(const nsAString& aConditionText,
                               ErrorResult& aRv)
{
=======
void CSSMediaRule::SetConditionText(const nsAString& aConditionText,
                                    ErrorResult& aRv) {
  if (IsReadOnly()) {
    return;
  }
>>>>>>> upstream-releases
  Media()->SetMediaText(aConditionText);
}

<<<<<<< HEAD
/* virtual */ void CSSMediaRule::GetCssText(nsAString& aCssText) const {
||||||| merged common ancestors
/* virtual */ void
CSSMediaRule::GetCssText(nsAString& aCssText) const
{
=======
/* virtual */
void CSSMediaRule::GetCssText(nsAString& aCssText) const {
>>>>>>> upstream-releases
  Servo_MediaRule_GetCssText(mRawRule, &aCssText);
}

/* virtual */ dom::MediaList* CSSMediaRule::Media() {
  if (!mMediaList) {
    mMediaList = new MediaList(Servo_MediaRule_GetMedia(mRawRule).Consume());
    mMediaList->SetStyleSheet(GetStyleSheet());
  }
  return mMediaList;
}

<<<<<<< HEAD
/* virtual */ size_t CSSMediaRule::SizeOfIncludingThis(
    mozilla::MallocSizeOf aMallocSizeOf) const {
||||||| merged common ancestors
/* virtual */ size_t
CSSMediaRule::SizeOfIncludingThis(mozilla::MallocSizeOf aMallocSizeOf) const
{
=======
/* virtual */
size_t CSSMediaRule::SizeOfIncludingThis(
    mozilla::MallocSizeOf aMallocSizeOf) const {
>>>>>>> upstream-releases
  // TODO Implement this!
  return aMallocSizeOf(this);
}

<<<<<<< HEAD
/* virtual */ JSObject* CSSMediaRule::WrapObject(
    JSContext* aCx, JS::Handle<JSObject*> aGivenProto) {
||||||| merged common ancestors
/* virtual */ JSObject*
CSSMediaRule::WrapObject(JSContext* aCx, JS::Handle<JSObject*> aGivenProto)
{
=======
/* virtual */
JSObject* CSSMediaRule::WrapObject(JSContext* aCx,
                                   JS::Handle<JSObject*> aGivenProto) {
>>>>>>> upstream-releases
  return CSSMediaRule_Binding::Wrap(aCx, this, aGivenProto);
}

}  // namespace dom
}  // namespace mozilla
