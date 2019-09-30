/* -*- Mode: C++; tab-width: 8; indent-tabs-mode: nil; c-basic-offset: 2 -*- */
/* vim: set ts=8 sts=2 et sw=2 tw=80: */
/* This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/. */

#include "mozilla/dom/CSSStyleRule.h"

#include "mozilla/DeclarationBlock.h"
#include "mozilla/ServoBindings.h"
#include "mozilla/dom/CSSStyleRuleBinding.h"

#include "mozAutoDocUpdate.h"

using namespace mozilla::dom;

namespace mozilla {
namespace dom {

// -- CSSStyleRuleDeclaration ---------------------------------------

CSSStyleRuleDeclaration::CSSStyleRuleDeclaration(
    already_AddRefed<RawServoDeclarationBlock> aDecls)
    : mDecls(new DeclarationBlock(std::move(aDecls))) {}

CSSStyleRuleDeclaration::~CSSStyleRuleDeclaration() {
  mDecls->SetOwningRule(nullptr);
}

// QueryInterface implementation for CSSStyleRuleDeclaration
NS_INTERFACE_MAP_BEGIN(CSSStyleRuleDeclaration)
  NS_WRAPPERCACHE_INTERFACE_MAP_ENTRY
  // We forward the cycle collection interfaces to Rule(), which is
  // never null (in fact, we're part of that object!)
  if (aIID.Equals(NS_GET_IID(nsCycleCollectionISupports)) ||
      aIID.Equals(NS_GET_IID(nsXPCOMCycleCollectionParticipant))) {
    return Rule()->QueryInterface(aIID, aInstancePtr);
<<<<<<< HEAD
  } else
    NS_IMPL_QUERY_TAIL_INHERITING(nsDOMCSSDeclaration)
||||||| merged common ancestors
  }
  else
NS_IMPL_QUERY_TAIL_INHERITING(nsDOMCSSDeclaration)
=======
  }
NS_INTERFACE_MAP_END_INHERITING(nsDOMCSSDeclaration)
>>>>>>> upstream-releases

  NS_IMPL_ADDREF_USING_AGGREGATOR(CSSStyleRuleDeclaration, Rule())
  NS_IMPL_RELEASE_USING_AGGREGATOR(CSSStyleRuleDeclaration, Rule())

  /* nsDOMCSSDeclaration implementation */

<<<<<<< HEAD
  css::Rule* CSSStyleRuleDeclaration::GetParentRule() { return Rule(); }
||||||| merged common ancestors
css::Rule*
CSSStyleRuleDeclaration::GetParentRule()
{
  return Rule();
}
=======
css::Rule* CSSStyleRuleDeclaration::GetParentRule() { return Rule(); }
>>>>>>> upstream-releases

<<<<<<< HEAD
  nsINode* CSSStyleRuleDeclaration::GetParentObject() {
    return Rule()->GetParentObject();
  }
||||||| merged common ancestors
nsINode*
CSSStyleRuleDeclaration::GetParentObject()
{
  return Rule()->GetParentObject();
}
=======
nsINode* CSSStyleRuleDeclaration::GetParentObject() {
  return Rule()->GetParentObject();
}
>>>>>>> upstream-releases

<<<<<<< HEAD
  DeclarationBlock* CSSStyleRuleDeclaration::GetOrCreateCSSDeclaration(
      Operation aOperation, DeclarationBlock * *aCreated) {
    return mDecls;
  }
||||||| merged common ancestors
DeclarationBlock*
CSSStyleRuleDeclaration::GetOrCreateCSSDeclaration(Operation aOperation,
                                                   DeclarationBlock** aCreated)
{
  return mDecls;
}
=======
DeclarationBlock* CSSStyleRuleDeclaration::GetOrCreateCSSDeclaration(
    Operation aOperation, DeclarationBlock** aCreated) {
  return mDecls;
}
>>>>>>> upstream-releases

<<<<<<< HEAD
  nsresult CSSStyleRuleDeclaration::SetCSSDeclaration(
      DeclarationBlock * aDecl, MutationClosureData * aClosureData) {
    CSSStyleRule* rule = Rule();
    if (RefPtr<StyleSheet> sheet = rule->GetStyleSheet()) {
      if (aDecl != mDecls) {
        mDecls->SetOwningRule(nullptr);
        RefPtr<DeclarationBlock> decls = aDecl;
        Servo_StyleRule_SetStyle(rule->Raw(), decls->Raw());
        mDecls = decls.forget();
        mDecls->SetOwningRule(rule);
      }
      sheet->RuleChanged(rule);
||||||| merged common ancestors
nsresult
CSSStyleRuleDeclaration::SetCSSDeclaration(DeclarationBlock* aDecl,
                                           MutationClosureData* aClosureData)
{
  CSSStyleRule* rule = Rule();
  if (RefPtr<StyleSheet> sheet = rule->GetStyleSheet()) {
    if (aDecl != mDecls) {
      mDecls->SetOwningRule(nullptr);
      RefPtr<DeclarationBlock> decls = aDecl;
      Servo_StyleRule_SetStyle(rule->Raw(), decls->Raw());
      mDecls = decls.forget();
      mDecls->SetOwningRule(rule);
=======
nsresult CSSStyleRuleDeclaration::SetCSSDeclaration(
    DeclarationBlock* aDecl, MutationClosureData* aClosureData) {
  CSSStyleRule* rule = Rule();

  if (rule->IsReadOnly()) {
    return NS_OK;
  }

  if (RefPtr<StyleSheet> sheet = rule->GetStyleSheet()) {
    if (aDecl != mDecls) {
      mDecls->SetOwningRule(nullptr);
      RefPtr<DeclarationBlock> decls = aDecl;
      Servo_StyleRule_SetStyle(rule->Raw(), decls->Raw());
      mDecls = decls.forget();
      mDecls->SetOwningRule(rule);
>>>>>>> upstream-releases
    }
    return NS_OK;
  }

<<<<<<< HEAD
  nsIDocument* CSSStyleRuleDeclaration::DocToUpdate() { return nullptr; }
||||||| merged common ancestors
nsIDocument*
CSSStyleRuleDeclaration::DocToUpdate()
{
  return nullptr;
}
=======
Document* CSSStyleRuleDeclaration::DocToUpdate() { return nullptr; }
>>>>>>> upstream-releases

<<<<<<< HEAD
  nsDOMCSSDeclaration::ParsingEnvironment
  CSSStyleRuleDeclaration::GetParsingEnvironment(nsIPrincipal *
                                                 aSubjectPrincipal) const {
    return GetParsingEnvironmentForRule(Rule());
  }
||||||| merged common ancestors
nsDOMCSSDeclaration::ParsingEnvironment
CSSStyleRuleDeclaration::GetParsingEnvironment(
  nsIPrincipal* aSubjectPrincipal) const
{
  return GetParsingEnvironmentForRule(Rule());
}
=======
nsDOMCSSDeclaration::ParsingEnvironment
CSSStyleRuleDeclaration::GetParsingEnvironment(
    nsIPrincipal* aSubjectPrincipal) const {
  return GetParsingEnvironmentForRule(Rule());
}
>>>>>>> upstream-releases

<<<<<<< HEAD
  // -- CSSStyleRule --------------------------------------------------
||||||| merged common ancestors
// -- CSSStyleRule --------------------------------------------------

CSSStyleRule::CSSStyleRule(already_AddRefed<RawServoStyleRule> aRawRule,
                           StyleSheet* aSheet,
                           css::Rule* aParentRule,
                           uint32_t aLine,
                           uint32_t aColumn)
  : BindingStyleRule(aSheet, aParentRule, aLine, aColumn)
  , mRawRule(aRawRule)
  , mDecls(Servo_StyleRule_GetStyle(mRawRule).Consume())
{
}
=======
// -- CSSStyleRule --------------------------------------------------

CSSStyleRule::CSSStyleRule(already_AddRefed<RawServoStyleRule> aRawRule,
                           StyleSheet* aSheet, css::Rule* aParentRule,
                           uint32_t aLine, uint32_t aColumn)
    : BindingStyleRule(aSheet, aParentRule, aLine, aColumn),
      mRawRule(aRawRule),
      mDecls(Servo_StyleRule_GetStyle(mRawRule).Consume()) {}
>>>>>>> upstream-releases

  CSSStyleRule::CSSStyleRule(already_AddRefed<RawServoStyleRule> aRawRule,
                             StyleSheet * aSheet, css::Rule * aParentRule,
                             uint32_t aLine, uint32_t aColumn)
      : BindingStyleRule(aSheet, aParentRule, aLine, aColumn),
        mRawRule(aRawRule),
        mDecls(Servo_StyleRule_GetStyle(mRawRule).Consume()) {}

  NS_IMPL_ISUPPORTS_CYCLE_COLLECTION_INHERITED_0(CSSStyleRule, css::Rule)

  NS_IMPL_CYCLE_COLLECTION_CLASS(CSSStyleRule)

  NS_IMPL_CYCLE_COLLECTION_TRACE_BEGIN_INHERITED(CSSStyleRule, css::Rule)
    // Keep this in sync with IsCCLeaf.

    // Trace the wrapper for our declaration.  This just expands out
    // NS_IMPL_CYCLE_COLLECTION_TRACE_PRESERVED_WRAPPER which we can't use
    // directly because the wrapper is on the declaration, not on us.
    tmp->mDecls.TraceWrapper(aCallbacks, aClosure);
  NS_IMPL_CYCLE_COLLECTION_TRACE_END

  NS_IMPL_CYCLE_COLLECTION_UNLINK_BEGIN_INHERITED(CSSStyleRule, css::Rule)
    // Keep this in sync with IsCCLeaf.

    // Unlink the wrapper for our declaraton.  This just expands out
    // NS_IMPL_CYCLE_COLLECTION_UNLINK_PRESERVED_WRAPPER which we can't use
    // directly because the wrapper is on the declaration, not on us.
    tmp->mDecls.ReleaseWrapper(static_cast<nsISupports*>(p));
  NS_IMPL_CYCLE_COLLECTION_UNLINK_END

<<<<<<< HEAD
  NS_IMPL_CYCLE_COLLECTION_TRAVERSE_BEGIN_INHERITED(CSSStyleRule, css::Rule)
    // Keep this in sync with IsCCLeaf.
  NS_IMPL_CYCLE_COLLECTION_TRAVERSE_END
||||||| merged common ancestors
bool
CSSStyleRule::IsCCLeaf() const
{
  if (!Rule::IsCCLeaf()) {
    return false;
  }
=======
bool CSSStyleRule::IsCCLeaf() const {
  if (!Rule::IsCCLeaf()) {
    return false;
  }
>>>>>>> upstream-releases

  bool CSSStyleRule::IsCCLeaf() const {
    if (!Rule::IsCCLeaf()) {
      return false;
    }

<<<<<<< HEAD
    return !mDecls.PreservingWrapper();
  }
||||||| merged common ancestors
size_t
CSSStyleRule::SizeOfIncludingThis(MallocSizeOf aMallocSizeOf) const
{
  size_t n = aMallocSizeOf(this);
=======
size_t CSSStyleRule::SizeOfIncludingThis(MallocSizeOf aMallocSizeOf) const {
  size_t n = aMallocSizeOf(this);
>>>>>>> upstream-releases

<<<<<<< HEAD
  size_t CSSStyleRule::SizeOfIncludingThis(MallocSizeOf aMallocSizeOf) const {
    size_t n = aMallocSizeOf(this);
||||||| merged common ancestors
  // Measurement of the following members may be added later if DMD finds it is
  // worthwhile:
  // - mRawRule
  // - mDecls
=======
  // Measurement of the following members may be added later if DMD finds it
  // is worthwhile:
  // - mRawRule
  // - mDecls
>>>>>>> upstream-releases

    // Measurement of the following members may be added later if DMD finds it
    // is worthwhile:
    // - mRawRule
    // - mDecls

    return n;
  }

#ifdef DEBUG
<<<<<<< HEAD
  void CSSStyleRule::List(FILE * out, int32_t aIndent) const {
    nsAutoCString str;
    for (int32_t i = 0; i < aIndent; i++) {
      str.AppendLiteral("  ");
    }
    Servo_StyleRule_Debug(mRawRule, &str);
    fprintf_stderr(out, "%s\n", str.get());
||||||| merged common ancestors
void
CSSStyleRule::List(FILE* out, int32_t aIndent) const
{
  nsAutoCString str;
  for (int32_t i = 0; i < aIndent; i++) {
    str.AppendLiteral("  ");
=======
void CSSStyleRule::List(FILE* out, int32_t aIndent) const {
  nsAutoCString str;
  for (int32_t i = 0; i < aIndent; i++) {
    str.AppendLiteral("  ");
>>>>>>> upstream-releases
  }
#endif

  /* CSSRule implementation */

<<<<<<< HEAD
  void CSSStyleRule::GetCssText(nsAString & aCssText) const {
    Servo_StyleRule_GetCssText(mRawRule, &aCssText);
  }
||||||| merged common ancestors
void
CSSStyleRule::GetCssText(nsAString& aCssText) const
{
  Servo_StyleRule_GetCssText(mRawRule, &aCssText);
}
=======
void CSSStyleRule::GetCssText(nsAString& aCssText) const {
  Servo_StyleRule_GetCssText(mRawRule, &aCssText);
}
>>>>>>> upstream-releases

<<<<<<< HEAD
  nsICSSDeclaration* CSSStyleRule::Style() { return &mDecls; }
||||||| merged common ancestors
nsICSSDeclaration*
CSSStyleRule::Style()
{
  return &mDecls;
}
=======
nsICSSDeclaration* CSSStyleRule::Style() { return &mDecls; }
>>>>>>> upstream-releases

  /* CSSStyleRule implementation */

<<<<<<< HEAD
  void CSSStyleRule::GetSelectorText(nsAString & aSelectorText) {
    Servo_StyleRule_GetSelectorText(mRawRule, &aSelectorText);
  }
||||||| merged common ancestors
void
CSSStyleRule::GetSelectorText(nsAString& aSelectorText)
{
  Servo_StyleRule_GetSelectorText(mRawRule, &aSelectorText);
}
=======
void CSSStyleRule::GetSelectorText(nsAString& aSelectorText) {
  Servo_StyleRule_GetSelectorText(mRawRule, &aSelectorText);
}
>>>>>>> upstream-releases

<<<<<<< HEAD
  void CSSStyleRule::SetSelectorText(const nsAString& aSelectorText) {
    if (RefPtr<StyleSheet> sheet = GetStyleSheet()) {
      // StyleRule lives inside of the Inner, it is unsafe to call WillDirty
      // if sheet does not already have a unique Inner.
      sheet->AssertHasUniqueInner();
      sheet->WillDirty();

      const RawServoStyleSheetContents* contents = sheet->RawContents();
      if (Servo_StyleRule_SetSelectorText(contents, mRawRule, &aSelectorText)) {
        sheet->RuleChanged(this);
      }
||||||| merged common ancestors
void
CSSStyleRule::SetSelectorText(const nsAString& aSelectorText)
{
  if (RefPtr<StyleSheet> sheet = GetStyleSheet()) {
    // StyleRule lives inside of the Inner, it is unsafe to call WillDirty
    // if sheet does not already have a unique Inner.
    sheet->AssertHasUniqueInner();
    sheet->WillDirty();

    const RawServoStyleSheetContents* contents = sheet->RawContents();
    if (Servo_StyleRule_SetSelectorText(contents, mRawRule, &aSelectorText)) {
      sheet->RuleChanged(this);
=======
void CSSStyleRule::SetSelectorText(const nsAString& aSelectorText) {
  if (IsReadOnly()) {
    return;
  }

  if (RefPtr<StyleSheet> sheet = GetStyleSheet()) {
    // StyleRule lives inside of the Inner, it is unsafe to call WillDirty
    // if sheet does not already have a unique Inner.
    sheet->AssertHasUniqueInner();
    sheet->WillDirty();

    const RawServoStyleSheetContents* contents = sheet->RawContents();
    if (Servo_StyleRule_SetSelectorText(contents, mRawRule, &aSelectorText)) {
      sheet->RuleChanged(this);
>>>>>>> upstream-releases
    }
  }

<<<<<<< HEAD
  uint32_t CSSStyleRule::GetSelectorCount() {
    uint32_t aCount;
    Servo_StyleRule_GetSelectorCount(mRawRule, &aCount);
    return aCount;
  }
||||||| merged common ancestors
uint32_t
CSSStyleRule::GetSelectorCount()
{
  uint32_t aCount;
  Servo_StyleRule_GetSelectorCount(mRawRule, &aCount);
  return aCount;
}
=======
uint32_t CSSStyleRule::GetSelectorCount() {
  uint32_t aCount;
  Servo_StyleRule_GetSelectorCount(mRawRule, &aCount);
  return aCount;
}
>>>>>>> upstream-releases

<<<<<<< HEAD
  nsresult CSSStyleRule::GetSelectorText(uint32_t aSelectorIndex,
                                         nsAString & aText) {
    Servo_StyleRule_GetSelectorTextAtIndex(mRawRule, aSelectorIndex, &aText);
    return NS_OK;
  }
||||||| merged common ancestors
nsresult
CSSStyleRule::GetSelectorText(uint32_t aSelectorIndex, nsAString& aText)
{
  Servo_StyleRule_GetSelectorTextAtIndex(mRawRule, aSelectorIndex, &aText);
  return NS_OK;
}
=======
nsresult CSSStyleRule::GetSelectorText(uint32_t aSelectorIndex,
                                       nsAString& aText) {
  Servo_StyleRule_GetSelectorTextAtIndex(mRawRule, aSelectorIndex, &aText);
  return NS_OK;
}
>>>>>>> upstream-releases

<<<<<<< HEAD
  nsresult CSSStyleRule::GetSpecificity(uint32_t aSelectorIndex,
                                        uint64_t * aSpecificity) {
    Servo_StyleRule_GetSpecificityAtIndex(mRawRule, aSelectorIndex,
                                          aSpecificity);
    return NS_OK;
  }
||||||| merged common ancestors
nsresult
CSSStyleRule::GetSpecificity(uint32_t aSelectorIndex, uint64_t* aSpecificity)
{
  Servo_StyleRule_GetSpecificityAtIndex(mRawRule, aSelectorIndex, aSpecificity);
  return NS_OK;
}
=======
nsresult CSSStyleRule::GetSpecificity(uint32_t aSelectorIndex,
                                      uint64_t* aSpecificity) {
  Servo_StyleRule_GetSpecificityAtIndex(mRawRule, aSelectorIndex, aSpecificity);
  return NS_OK;
}
>>>>>>> upstream-releases

<<<<<<< HEAD
  nsresult CSSStyleRule::SelectorMatchesElement(
      Element * aElement, uint32_t aSelectorIndex, const nsAString& aPseudo,
      bool* aMatches) {
    CSSPseudoElementType pseudoType = CSSPseudoElementType::NotPseudo;
    if (!aPseudo.IsEmpty()) {
      RefPtr<nsAtom> pseudoElt = NS_Atomize(aPseudo);
      pseudoType = nsCSSPseudoElements::GetPseudoType(
          pseudoElt, CSSEnabledState::eIgnoreEnabledState);

      if (pseudoType == CSSPseudoElementType::NotPseudo) {
        *aMatches = false;
        return NS_OK;
      }
||||||| merged common ancestors
nsresult
CSSStyleRule::SelectorMatchesElement(Element* aElement,
                                       uint32_t aSelectorIndex,
                                       const nsAString& aPseudo,
                                       bool* aMatches)
{
  CSSPseudoElementType pseudoType = CSSPseudoElementType::NotPseudo;
  if (!aPseudo.IsEmpty()) {
    RefPtr<nsAtom> pseudoElt = NS_Atomize(aPseudo);
    pseudoType = nsCSSPseudoElements::GetPseudoType(
        pseudoElt, CSSEnabledState::eIgnoreEnabledState);

    if (pseudoType == CSSPseudoElementType::NotPseudo) {
      *aMatches = false;
      return NS_OK;
=======
nsresult CSSStyleRule::SelectorMatchesElement(Element* aElement,
                                              uint32_t aSelectorIndex,
                                              const nsAString& aPseudo,
                                              bool* aMatches) {
  PseudoStyleType pseudoType = PseudoStyleType::NotPseudo;
  if (!aPseudo.IsEmpty()) {
    RefPtr<nsAtom> pseudoElt = NS_Atomize(aPseudo);
    pseudoType = nsCSSPseudoElements::GetPseudoType(
        pseudoElt, CSSEnabledState::IgnoreEnabledState);

    if (pseudoType == PseudoStyleType::NotPseudo) {
      *aMatches = false;
      return NS_OK;
>>>>>>> upstream-releases
    }

<<<<<<< HEAD
    *aMatches = Servo_StyleRule_SelectorMatchesElement(
        mRawRule, aElement, aSelectorIndex, pseudoType);
||||||| merged common ancestors
  *aMatches = Servo_StyleRule_SelectorMatchesElement(mRawRule, aElement,
                                                     aSelectorIndex, pseudoType);
=======
  *aMatches = Servo_StyleRule_SelectorMatchesElement(
      mRawRule, aElement, aSelectorIndex, pseudoType);
>>>>>>> upstream-releases

    return NS_OK;
  }

<<<<<<< HEAD
  NotNull<DeclarationBlock*> CSSStyleRule::GetDeclarationBlock() const {
    return WrapNotNull(mDecls.mDecls);
  }
||||||| merged common ancestors
NotNull<DeclarationBlock*>
CSSStyleRule::GetDeclarationBlock() const
{
  return WrapNotNull(mDecls.mDecls);
}
=======
NotNull<DeclarationBlock*> CSSStyleRule::GetDeclarationBlock() const {
  return WrapNotNull(mDecls.mDecls);
}
>>>>>>> upstream-releases

}  // namespace dom
}  // namespace mozilla
