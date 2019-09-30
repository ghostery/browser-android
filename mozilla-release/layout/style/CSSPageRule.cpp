/* -*- Mode: C++; tab-width: 8; indent-tabs-mode: nil; c-basic-offset: 2 -*- */
/* vim: set ts=8 sts=2 et sw=2 tw=80: */
/* This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/. */

#include "mozilla/dom/CSSPageRule.h"
#include "mozilla/dom/CSSPageRuleBinding.h"

#include "mozilla/DeclarationBlock.h"
#include "mozilla/ServoBindings.h"

namespace mozilla {
namespace dom {

// -- CSSPageRuleDeclaration ---------------------------------------

CSSPageRuleDeclaration::CSSPageRuleDeclaration(
    already_AddRefed<RawServoDeclarationBlock> aDecls)
    : mDecls(new DeclarationBlock(std::move(aDecls))) {}

CSSPageRuleDeclaration::~CSSPageRuleDeclaration() {
  mDecls->SetOwningRule(nullptr);
}

// QueryInterface implementation for CSSPageRuleDeclaration
NS_INTERFACE_MAP_BEGIN(CSSPageRuleDeclaration)
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

  NS_IMPL_ADDREF_USING_AGGREGATOR(CSSPageRuleDeclaration, Rule())
  NS_IMPL_RELEASE_USING_AGGREGATOR(CSSPageRuleDeclaration, Rule())

  /* nsDOMCSSDeclaration implementation */

<<<<<<< HEAD
  css::Rule* CSSPageRuleDeclaration::GetParentRule() { return Rule(); }
||||||| merged common ancestors
css::Rule*
CSSPageRuleDeclaration::GetParentRule()
{
  return Rule();
}
=======
css::Rule* CSSPageRuleDeclaration::GetParentRule() { return Rule(); }
>>>>>>> upstream-releases

<<<<<<< HEAD
  nsINode* CSSPageRuleDeclaration::GetParentObject() {
    return Rule()->GetParentObject();
  }
||||||| merged common ancestors
nsINode*
CSSPageRuleDeclaration::GetParentObject()
{
  return Rule()->GetParentObject();
}
=======
nsINode* CSSPageRuleDeclaration::GetParentObject() {
  return Rule()->GetParentObject();
}
>>>>>>> upstream-releases

<<<<<<< HEAD
  DeclarationBlock* CSSPageRuleDeclaration::GetOrCreateCSSDeclaration(
      Operation aOperation, DeclarationBlock * *aCreated) {
    return mDecls;
  }
||||||| merged common ancestors
DeclarationBlock*
CSSPageRuleDeclaration::GetOrCreateCSSDeclaration(Operation aOperation,
                                                  DeclarationBlock** aCreated)
{
  return mDecls;
}
=======
DeclarationBlock* CSSPageRuleDeclaration::GetOrCreateCSSDeclaration(
    Operation aOperation, DeclarationBlock** aCreated) {
  return mDecls;
}
>>>>>>> upstream-releases

<<<<<<< HEAD
  nsresult CSSPageRuleDeclaration::SetCSSDeclaration(
      DeclarationBlock * aDecl, MutationClosureData * aClosureData) {
    MOZ_ASSERT(aDecl, "must be non-null");
    CSSPageRule* rule = Rule();

    if (aDecl != mDecls) {
      mDecls->SetOwningRule(nullptr);
      RefPtr<DeclarationBlock> decls = aDecl;
      Servo_PageRule_SetStyle(rule->Raw(), decls->Raw());
      mDecls = decls.forget();
      mDecls->SetOwningRule(rule);
    }

    return NS_OK;
||||||| merged common ancestors
nsresult
CSSPageRuleDeclaration::SetCSSDeclaration(DeclarationBlock* aDecl,
                                          MutationClosureData* aClosureData)
{
  MOZ_ASSERT(aDecl, "must be non-null");
  CSSPageRule* rule = Rule();

  if (aDecl != mDecls) {
    mDecls->SetOwningRule(nullptr);
    RefPtr<DeclarationBlock> decls = aDecl;
    Servo_PageRule_SetStyle(rule->Raw(), decls->Raw());
    mDecls = decls.forget();
    mDecls->SetOwningRule(rule);
=======
nsresult CSSPageRuleDeclaration::SetCSSDeclaration(
    DeclarationBlock* aDecl, MutationClosureData* aClosureData) {
  MOZ_ASSERT(aDecl, "must be non-null");
  CSSPageRule* rule = Rule();

  if (rule->IsReadOnly()) {
    return NS_OK;
  }

  if (aDecl != mDecls) {
    mDecls->SetOwningRule(nullptr);
    RefPtr<DeclarationBlock> decls = aDecl;
    Servo_PageRule_SetStyle(rule->Raw(), decls->Raw());
    mDecls = decls.forget();
    mDecls->SetOwningRule(rule);
>>>>>>> upstream-releases
  }

  nsIDocument* CSSPageRuleDeclaration::DocToUpdate() { return nullptr; }

<<<<<<< HEAD
  nsDOMCSSDeclaration::ParsingEnvironment
  CSSPageRuleDeclaration::GetParsingEnvironment(nsIPrincipal *
                                                aSubjectPrincipal) const {
    return GetParsingEnvironmentForRule(Rule());
  }

  // -- CSSPageRule --------------------------------------------------
||||||| merged common ancestors
nsIDocument*
CSSPageRuleDeclaration::DocToUpdate()
{
  return nullptr;
}

nsDOMCSSDeclaration::ParsingEnvironment
CSSPageRuleDeclaration::GetParsingEnvironment(
  nsIPrincipal* aSubjectPrincipal) const
{
  return GetParsingEnvironmentForRule(Rule());
}
=======
nsDOMCSSDeclaration::ParsingEnvironment
CSSPageRuleDeclaration::GetParsingEnvironment(
    nsIPrincipal* aSubjectPrincipal) const {
  return GetParsingEnvironmentForRule(Rule());
}
>>>>>>> upstream-releases

<<<<<<< HEAD
  CSSPageRule::CSSPageRule(RefPtr<RawServoPageRule> aRawRule,
                           StyleSheet * aSheet, css::Rule * aParentRule,
                           uint32_t aLine, uint32_t aColumn)
      : css::Rule(aSheet, aParentRule, aLine, aColumn),
        mRawRule(std::move(aRawRule)),
        mDecls(Servo_PageRule_GetStyle(mRawRule).Consume()) {}
||||||| merged common ancestors
// -- CSSPageRule --------------------------------------------------

CSSPageRule::CSSPageRule(RefPtr<RawServoPageRule> aRawRule,
                         StyleSheet* aSheet,
                         css::Rule* aParentRule,
                         uint32_t aLine,
                         uint32_t aColumn)
  : css::Rule(aSheet, aParentRule, aLine, aColumn)
  , mRawRule(std::move(aRawRule))
  , mDecls(Servo_PageRule_GetStyle(mRawRule).Consume())
{
}
=======
// -- CSSPageRule --------------------------------------------------

CSSPageRule::CSSPageRule(RefPtr<RawServoPageRule> aRawRule, StyleSheet* aSheet,
                         css::Rule* aParentRule, uint32_t aLine,
                         uint32_t aColumn)
    : css::Rule(aSheet, aParentRule, aLine, aColumn),
      mRawRule(std::move(aRawRule)),
      mDecls(Servo_PageRule_GetStyle(mRawRule).Consume()) {}
>>>>>>> upstream-releases

  NS_IMPL_ADDREF_INHERITED(CSSPageRule, css::Rule)
  NS_IMPL_RELEASE_INHERITED(CSSPageRule, css::Rule)

  // QueryInterface implementation for PageRule
  NS_INTERFACE_MAP_BEGIN_CYCLE_COLLECTION(CSSPageRule)
  NS_INTERFACE_MAP_END_INHERITING(css::Rule)

  NS_IMPL_CYCLE_COLLECTION_CLASS(CSSPageRule)

  NS_IMPL_CYCLE_COLLECTION_TRACE_BEGIN_INHERITED(CSSPageRule, css::Rule)
    // Keep this in sync with IsCCLeaf.

    // Trace the wrapper for our declaration.  This just expands out
    // NS_IMPL_CYCLE_COLLECTION_TRACE_PRESERVED_WRAPPER which we can't use
    // directly because the wrapper is on the declaration, not on us.
    tmp->mDecls.TraceWrapper(aCallbacks, aClosure);
  NS_IMPL_CYCLE_COLLECTION_TRACE_END

  NS_IMPL_CYCLE_COLLECTION_UNLINK_BEGIN_INHERITED(CSSPageRule, css::Rule)
    // Keep this in sync with IsCCLeaf.

    // Unlink the wrapper for our declaraton.  This just expands out
    // NS_IMPL_CYCLE_COLLECTION_UNLINK_PRESERVED_WRAPPER which we can't use
    // directly because the wrapper is on the declaration, not on us.
    tmp->mDecls.ReleaseWrapper(static_cast<nsISupports*>(p));
    tmp->mDecls.mDecls->SetOwningRule(nullptr);
  NS_IMPL_CYCLE_COLLECTION_UNLINK_END

  NS_IMPL_CYCLE_COLLECTION_TRAVERSE_BEGIN_INHERITED(CSSPageRule, css::Rule)
    // Keep this in sync with IsCCLeaf.
  NS_IMPL_CYCLE_COLLECTION_TRAVERSE_END

<<<<<<< HEAD
  bool CSSPageRule::IsCCLeaf() const {
    if (!Rule::IsCCLeaf()) {
      return false;
    }
||||||| merged common ancestors
bool
CSSPageRule::IsCCLeaf() const
{
  if (!Rule::IsCCLeaf()) {
    return false;
  }
=======
bool CSSPageRule::IsCCLeaf() const {
  if (!Rule::IsCCLeaf()) {
    return false;
  }
>>>>>>> upstream-releases

    return !mDecls.PreservingWrapper();
  }

<<<<<<< HEAD
  size_t CSSPageRule::SizeOfIncludingThis(MallocSizeOf aMallocSizeOf) const {
    // TODO Implement this!
    return aMallocSizeOf(this);
  }
||||||| merged common ancestors
size_t
CSSPageRule::SizeOfIncludingThis(MallocSizeOf aMallocSizeOf) const
{
  // TODO Implement this!
  return aMallocSizeOf(this);
}
=======
size_t CSSPageRule::SizeOfIncludingThis(MallocSizeOf aMallocSizeOf) const {
  // TODO Implement this!
  return aMallocSizeOf(this);
}
>>>>>>> upstream-releases

#ifdef DEBUG
<<<<<<< HEAD
  void CSSPageRule::List(FILE * out, int32_t aIndent) const {
    nsAutoCString str;
    for (int32_t i = 0; i < aIndent; i++) {
      str.AppendLiteral("  ");
    }
    Servo_PageRule_Debug(mRawRule, &str);
    fprintf_stderr(out, "%s\n", str.get());
||||||| merged common ancestors
void
CSSPageRule::List(FILE* out, int32_t aIndent) const
{
  nsAutoCString str;
  for (int32_t i = 0; i < aIndent; i++) {
    str.AppendLiteral("  ");
=======
void CSSPageRule::List(FILE* out, int32_t aIndent) const {
  nsAutoCString str;
  for (int32_t i = 0; i < aIndent; i++) {
    str.AppendLiteral("  ");
>>>>>>> upstream-releases
  }
#endif

  /* CSSRule implementation */

<<<<<<< HEAD
  void CSSPageRule::GetCssText(nsAString & aCssText) const {
    Servo_PageRule_GetCssText(mRawRule, &aCssText);
  }
||||||| merged common ancestors
void
CSSPageRule::GetCssText(nsAString& aCssText) const
{
  Servo_PageRule_GetCssText(mRawRule, &aCssText);
}
=======
void CSSPageRule::GetCssText(nsAString& aCssText) const {
  Servo_PageRule_GetCssText(mRawRule, &aCssText);
}
>>>>>>> upstream-releases

  /* CSSPageRule implementation */

<<<<<<< HEAD
  nsICSSDeclaration* CSSPageRule::Style() { return &mDecls; }
||||||| merged common ancestors
nsICSSDeclaration*
CSSPageRule::Style()
{
  return &mDecls;
}
=======
nsICSSDeclaration* CSSPageRule::Style() { return &mDecls; }
>>>>>>> upstream-releases

<<<<<<< HEAD
  JSObject* CSSPageRule::WrapObject(JSContext * aCx,
                                    JS::Handle<JSObject*> aGivenProto) {
    return CSSPageRule_Binding::Wrap(aCx, this, aGivenProto);
  }
||||||| merged common ancestors
JSObject*
CSSPageRule::WrapObject(JSContext* aCx, JS::Handle<JSObject*> aGivenProto)
{
  return CSSPageRule_Binding::Wrap(aCx, this, aGivenProto);
}
=======
JSObject* CSSPageRule::WrapObject(JSContext* aCx,
                                  JS::Handle<JSObject*> aGivenProto) {
  return CSSPageRule_Binding::Wrap(aCx, this, aGivenProto);
}
>>>>>>> upstream-releases

}  // namespace dom
}  // namespace mozilla
