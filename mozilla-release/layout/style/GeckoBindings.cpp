/* -*- Mode: C++; tab-width: 8; indent-tabs-mode: nil; c-basic-offset: 2 -*- */
/* vim: set ts=8 sts=2 et sw=2 tw=80: */
/* This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/. */

/* FFI functions for Servo to call into Gecko */

#include "mozilla/GeckoBindings.h"

#include "ChildIterator.h"
#include "ErrorReporter.h"
#include "GeckoProfiler.h"
#include "gfxFontFamilyList.h"
#include "gfxFontFeatures.h"
#include "nsAnimationManager.h"
#include "nsAttrValueInlines.h"
#include "nsCSSFrameConstructor.h"
#include "nsCSSProps.h"
#include "nsCSSPseudoElements.h"
#include "nsContentUtils.h"
#include "nsDOMTokenList.h"
#include "nsDeviceContext.h"
#include "nsIContentInlines.h"
#include "nsICrashReporter.h"
#include "mozilla/dom/DocumentInlines.h"
#include "nsILoadContext.h"
#include "nsIFrame.h"
#include "nsIMemoryReporter.h"
#include "nsIMozBrowserFrame.h"
#include "nsINode.h"
#include "nsIPrincipal.h"
#include "nsIURI.h"
#include "nsFontMetrics.h"
#include "nsHTMLStyleSheet.h"
#include "nsMappedAttributes.h"
#include "nsMediaFeatures.h"
#include "nsNameSpaceManager.h"
#include "nsNetUtil.h"
#include "nsProxyRelease.h"
#include "nsString.h"
#include "nsStyleStruct.h"
#include "nsStyleUtil.h"
#include "nsTArray.h"
#include "nsTransitionManager.h"
#include "nsWindowSizes.h"

#include "mozilla/css/ImageLoader.h"
#include "mozilla/DeclarationBlock.h"
#include "mozilla/EffectCompositor.h"
#include "mozilla/EffectSet.h"
#include "mozilla/EventStates.h"
#include "mozilla/FontPropertyTypes.h"
#include "mozilla/Keyframe.h"
#include "mozilla/Mutex.h"
#include "mozilla/Preferences.h"
#include "mozilla/ServoElementSnapshot.h"
#include "mozilla/StaticPrefs.h"
#include "mozilla/RestyleManager.h"
#include "mozilla/SizeOfState.h"
#include "mozilla/StyleAnimationValue.h"
#include "mozilla/SystemGroup.h"
#include "mozilla/ServoBindings.h"
#include "mozilla/ServoTraversalStatistics.h"
#include "mozilla/Telemetry.h"
#include "mozilla/RWLock.h"
#include "mozilla/dom/Element.h"
#include "mozilla/dom/ElementInlines.h"
#include "mozilla/dom/HTMLTableCellElement.h"
#include "mozilla/dom/HTMLBodyElement.h"
#include "mozilla/dom/HTMLSlotElement.h"
#include "mozilla/dom/MediaList.h"
#include "mozilla/dom/SVGElement.h"
#include "mozilla/LookAndFeel.h"
#include "mozilla/URLExtraData.h"
#include "mozilla/dom/CSSMozDocumentRule.h"

#if defined(MOZ_MEMORY)
<<<<<<< HEAD
#include "mozmemory.h"
||||||| merged common ancestors
# include "mozmemory.h"
=======
#  include "mozmemory.h"
>>>>>>> upstream-releases
#endif

using namespace mozilla;
using namespace mozilla::css;
using namespace mozilla::dom;

<<<<<<< HEAD
#define SERVO_ARC_TYPE(name_, type_)                 \
  already_AddRefed<type_> type_##Strong::Consume() { \
    RefPtr<type_> result;                            \
    result.swap(mPtr);                               \
    return result.forget();                          \
  }
#include "mozilla/ServoArcTypeList.h"
SERVO_ARC_TYPE(ComputedStyle, ComputedStyle)
#undef SERVO_ARC_TYPE

||||||| merged common ancestors
#define SERVO_ARC_TYPE(name_, type_) \
  already_AddRefed<type_>            \
  type_##Strong::Consume() {         \
    RefPtr<type_> result;            \
    result.swap(mPtr);               \
    return result.forget();          \
  }
#include "mozilla/ServoArcTypeList.h"
SERVO_ARC_TYPE(ComputedStyle, ComputedStyle)
#undef SERVO_ARC_TYPE

=======
>>>>>>> upstream-releases
// Definitions of the global traversal stats.
bool ServoTraversalStatistics::sActive = false;
ServoTraversalStatistics ServoTraversalStatistics::sSingleton;

static RWLock* sServoFFILock = nullptr;

<<<<<<< HEAD
static const nsFont* ThreadSafeGetDefaultFontHelper(
    const nsPresContext* aPresContext, nsAtom* aLanguage, uint8_t aGenericId) {
||||||| merged common ancestors
static
const nsFont*
ThreadSafeGetDefaultFontHelper(const nsPresContext* aPresContext,
                               nsAtom* aLanguage, uint8_t aGenericId)
{
=======
static const nsFont* ThreadSafeGetDefaultFontHelper(
    const Document& aDocument, nsAtom* aLanguage,
    StyleGenericFontFamily aGenericId) {
>>>>>>> upstream-releases
  bool needsCache = false;
  const nsFont* retval;

  auto GetDefaultFont = [&](bool* aNeedsToCache) {
    auto* prefs = aDocument.GetFontPrefsForLang(aLanguage, aNeedsToCache);
    return prefs ? prefs->GetDefaultFont(aGenericId) : nullptr;
  };

  {
    AutoReadLock guard(*sServoFFILock);
    retval = GetDefaultFont(&needsCache);
  }
  if (!needsCache) {
    return retval;
  }
  {
    AutoWriteLock guard(*sServoFFILock);
    retval = GetDefaultFont(nullptr);
  }
  return retval;
}

/*
 * Does this child count as significant for selector matching?
 *
 * See nsStyleUtil::IsSignificantChild for details.
 */
<<<<<<< HEAD
bool Gecko_IsSignificantChild(RawGeckoNodeBorrowed aNode,
                              bool aWhitespaceIsSignificant) {
||||||| merged common ancestors
bool
Gecko_IsSignificantChild(RawGeckoNodeBorrowed aNode,
                         bool aWhitespaceIsSignificant)
{
=======
bool Gecko_IsSignificantChild(const nsINode* aNode,
                              bool aWhitespaceIsSignificant) {
>>>>>>> upstream-releases
  return nsStyleUtil::ThreadSafeIsSignificantChild(aNode->AsContent(),
                                                   aWhitespaceIsSignificant);
}

<<<<<<< HEAD
RawGeckoNodeBorrowedOrNull Gecko_GetLastChild(RawGeckoNodeBorrowed aNode) {
||||||| merged common ancestors
RawGeckoNodeBorrowedOrNull
Gecko_GetLastChild(RawGeckoNodeBorrowed aNode)
{
=======
const nsINode* Gecko_GetLastChild(const nsINode* aNode) {
>>>>>>> upstream-releases
  return aNode->GetLastChild();
}

<<<<<<< HEAD
RawGeckoNodeBorrowedOrNull Gecko_GetPreviousSibling(
    RawGeckoNodeBorrowed aNode) {
||||||| merged common ancestors
RawGeckoNodeBorrowedOrNull
Gecko_GetPreviousSibling(RawGeckoNodeBorrowed aNode)
{
=======
const nsINode* Gecko_GetPreviousSibling(const nsINode* aNode) {
>>>>>>> upstream-releases
  return aNode->GetPreviousSibling();
}

<<<<<<< HEAD
RawGeckoNodeBorrowedOrNull Gecko_GetFlattenedTreeParentNode(
    RawGeckoNodeBorrowed aNode) {
||||||| merged common ancestors
RawGeckoNodeBorrowedOrNull
Gecko_GetFlattenedTreeParentNode(RawGeckoNodeBorrowed aNode)
{
=======
const nsINode* Gecko_GetFlattenedTreeParentNode(const nsINode* aNode) {
>>>>>>> upstream-releases
  return aNode->GetFlattenedTreeParentNodeForStyle();
}

<<<<<<< HEAD
RawGeckoElementBorrowedOrNull Gecko_GetBeforeOrAfterPseudo(
    RawGeckoElementBorrowed aElement, bool aIsBefore) {
||||||| merged common ancestors
RawGeckoElementBorrowedOrNull
Gecko_GetBeforeOrAfterPseudo(RawGeckoElementBorrowed aElement, bool aIsBefore)
{
=======
const Element* Gecko_GetBeforeOrAfterPseudo(const Element* aElement,
                                            bool aIsBefore) {
>>>>>>> upstream-releases
  MOZ_ASSERT(aElement);
  MOZ_ASSERT(aElement->HasProperties());

  return aIsBefore ? nsLayoutUtils::GetBeforePseudo(aElement)
                   : nsLayoutUtils::GetAfterPseudo(aElement);
}

<<<<<<< HEAD
nsTArray<nsIContent*>* Gecko_GetAnonymousContentForElement(
    RawGeckoElementBorrowed aElement) {
||||||| merged common ancestors
nsTArray<nsIContent*>*
Gecko_GetAnonymousContentForElement(RawGeckoElementBorrowed aElement)
{
=======
const Element* Gecko_GetMarkerPseudo(const Element* aElement) {
  MOZ_ASSERT(aElement);
  MOZ_ASSERT(aElement->HasProperties());

  return nsLayoutUtils::GetMarkerPseudo(aElement);
}

nsTArray<nsIContent*>* Gecko_GetAnonymousContentForElement(
    const Element* aElement) {
>>>>>>> upstream-releases
  nsIAnonymousContentCreator* ac = do_QueryFrame(aElement->GetPrimaryFrame());
  if (!ac) {
    return nullptr;
  }

  auto* array = new nsTArray<nsIContent*>();
  nsContentUtils::AppendNativeAnonymousChildren(aElement, *array, 0);
  return array;
}

void Gecko_DestroyAnonymousContentList(nsTArray<nsIContent*>* aAnonContent) {
  MOZ_ASSERT(aAnonContent);
  delete aAnonContent;
}

<<<<<<< HEAD
const nsTArray<RefPtr<nsINode>>* Gecko_GetAssignedNodes(
    RawGeckoElementBorrowed aElement) {
||||||| merged common ancestors
const nsTArray<RefPtr<nsINode>>*
Gecko_GetAssignedNodes(RawGeckoElementBorrowed aElement)
{
=======
const nsTArray<RefPtr<nsINode>>* Gecko_GetAssignedNodes(
    const Element* aElement) {
>>>>>>> upstream-releases
  MOZ_ASSERT(HTMLSlotElement::FromNode(aElement));
  return &static_cast<const HTMLSlotElement*>(aElement)->AssignedNodes();
}

<<<<<<< HEAD
void Gecko_ComputedStyle_Init(mozilla::ComputedStyle* aStyle,
                              RawGeckoPresContextBorrowed aPresContext,
                              const ServoComputedData* aValues,
                              mozilla::CSSPseudoElementType aPseudoType,
                              nsAtom* aPseudoTag) {
  auto* presContext = const_cast<nsPresContext*>(aPresContext);
  new (KnownNotNull, aStyle)
      mozilla::ComputedStyle(presContext, aPseudoTag, aPseudoType,
                             ServoComputedDataForgotten(aValues));
||||||| merged common ancestors
void
Gecko_ComputedStyle_Init(mozilla::ComputedStyle* aStyle,
                         RawGeckoPresContextBorrowed aPresContext,
                         const ServoComputedData* aValues,
                         mozilla::CSSPseudoElementType aPseudoType,
                         nsAtom* aPseudoTag)
{
  auto* presContext = const_cast<nsPresContext*>(aPresContext);
  new (KnownNotNull, aStyle) mozilla::ComputedStyle(
      presContext, aPseudoTag, aPseudoType,
      ServoComputedDataForgotten(aValues));
=======
void Gecko_ComputedStyle_Init(ComputedStyle* aStyle,
                              const ServoComputedData* aValues,
                              PseudoStyleType aPseudoType) {
  new (KnownNotNull, aStyle)
      ComputedStyle(aPseudoType, ServoComputedDataForgotten(aValues));
>>>>>>> upstream-releases
}

ServoComputedData::ServoComputedData(const ServoComputedDataForgotten aValue) {
  PodAssign(this, aValue.mPtr);
}

MOZ_DEFINE_MALLOC_ENCLOSING_SIZE_OF(ServoStyleStructsMallocEnclosingSizeOf)

void ServoComputedData::AddSizeOfExcludingThis(nsWindowSizes& aSizes) const {
  // Note: GetStyleFoo() returns a pointer to an nsStyleFoo that sits within a
  // servo_arc::Arc, i.e. it is preceded by a word-sized refcount. So we need
  // to measure it with a function that can handle an interior pointer. We use
  // ServoStyleStructsEnclosingMallocSizeOf to clearly identify in DMD's
  // output the memory measured here.
#define STYLE_STRUCT(name_)                                       \
  static_assert(alignof(nsStyle##name_) <= sizeof(size_t),        \
                "alignment will break AddSizeOfExcludingThis()"); \
  const void* p##name_ = GetStyle##name_();                       \
  if (!aSizes.mState.HaveSeenPtr(p##name_)) {                     \
    aSizes.mStyleSizes.NS_STYLE_SIZES_FIELD(name_) +=             \
        ServoStyleStructsMallocEnclosingSizeOf(p##name_);         \
  }
#include "nsStyleStructList.h"
#undef STYLE_STRUCT

  if (visited_style.mPtr && !aSizes.mState.HaveSeenPtr(visited_style.mPtr)) {
    visited_style.mPtr->AddSizeOfIncludingThis(
        aSizes, &aSizes.mLayoutComputedValuesVisited);
  }

  // Measurement of the following members may be added later if DMD finds it is
  // worthwhile:
  // - custom_properties
  // - writing_mode
  // - rules
  // - font_computation_data
}

<<<<<<< HEAD
void Gecko_ComputedStyle_Destroy(mozilla::ComputedStyle* aStyle) {
||||||| merged common ancestors
void
Gecko_ComputedStyle_Destroy(mozilla::ComputedStyle* aStyle)
{
=======
void Gecko_ComputedStyle_Destroy(ComputedStyle* aStyle) {
>>>>>>> upstream-releases
  aStyle->~ComputedStyle();
}

<<<<<<< HEAD
void Gecko_ConstructStyleChildrenIterator(
    RawGeckoElementBorrowed aElement,
    RawGeckoStyleChildrenIteratorBorrowedMut aIterator) {
||||||| merged common ancestors
void
Gecko_ConstructStyleChildrenIterator(
  RawGeckoElementBorrowed aElement,
  RawGeckoStyleChildrenIteratorBorrowedMut aIterator)
{
=======
void Gecko_ConstructStyleChildrenIterator(const Element* aElement,
                                          StyleChildrenIterator* aIterator) {
>>>>>>> upstream-releases
  MOZ_ASSERT(aElement);
  MOZ_ASSERT(aIterator);
  new (aIterator) StyleChildrenIterator(aElement);
}

<<<<<<< HEAD
void Gecko_DestroyStyleChildrenIterator(
    RawGeckoStyleChildrenIteratorBorrowedMut aIterator) {
||||||| merged common ancestors
void
Gecko_DestroyStyleChildrenIterator(
  RawGeckoStyleChildrenIteratorBorrowedMut aIterator)
{
=======
void Gecko_DestroyStyleChildrenIterator(StyleChildrenIterator* aIterator) {
>>>>>>> upstream-releases
  MOZ_ASSERT(aIterator);

  aIterator->~StyleChildrenIterator();
}

<<<<<<< HEAD
RawGeckoNodeBorrowed Gecko_GetNextStyleChild(
    RawGeckoStyleChildrenIteratorBorrowedMut aIterator) {
||||||| merged common ancestors
RawGeckoNodeBorrowed
Gecko_GetNextStyleChild(RawGeckoStyleChildrenIteratorBorrowedMut aIterator)
{
=======
const nsINode* Gecko_GetNextStyleChild(StyleChildrenIterator* aIterator) {
>>>>>>> upstream-releases
  MOZ_ASSERT(aIterator);
  return aIterator->GetNextChild();
}

<<<<<<< HEAD
bool Gecko_VisitedStylesEnabled(const nsIDocument* aDoc) {
||||||| merged common ancestors
bool
Gecko_VisitedStylesEnabled(const nsIDocument* aDoc)
{
=======
bool Gecko_VisitedStylesEnabled(const Document* aDoc) {
>>>>>>> upstream-releases
  MOZ_ASSERT(aDoc);
  MOZ_ASSERT(NS_IsMainThread());

  if (!StaticPrefs::layout_css_visited_links_enabled()) {
    return false;
  }

  if (aDoc->IsBeingUsedAsImage()) {
    return false;
  }

  nsILoadContext* loadContext = aDoc->GetLoadContext();
  if (loadContext && loadContext->UsePrivateBrowsing()) {
    return false;
  }

  return true;
}

<<<<<<< HEAD
EventStates::ServoType Gecko_ElementState(RawGeckoElementBorrowed aElement) {
||||||| merged common ancestors
EventStates::ServoType
Gecko_ElementState(RawGeckoElementBorrowed aElement)
{
=======
EventStates::ServoType Gecko_ElementState(const Element* aElement) {
>>>>>>> upstream-releases
  return aElement->StyleState().ServoValue();
}

<<<<<<< HEAD
bool Gecko_IsRootElement(RawGeckoElementBorrowed aElement) {
||||||| merged common ancestors
bool
Gecko_IsRootElement(RawGeckoElementBorrowed aElement)
{
=======
bool Gecko_IsRootElement(const Element* aElement) {
>>>>>>> upstream-releases
  return aElement->OwnerDoc()->GetRootElement() == aElement;
}

// Dirtiness tracking.
<<<<<<< HEAD
void Gecko_SetNodeFlags(RawGeckoNodeBorrowed aNode, uint32_t aFlags) {
||||||| merged common ancestors
void
Gecko_SetNodeFlags(RawGeckoNodeBorrowed aNode, uint32_t aFlags)
{
=======
void Gecko_SetNodeFlags(const nsINode* aNode, uint32_t aFlags) {
>>>>>>> upstream-releases
  const_cast<nsINode*>(aNode)->SetFlags(aFlags);
}

<<<<<<< HEAD
void Gecko_UnsetNodeFlags(RawGeckoNodeBorrowed aNode, uint32_t aFlags) {
||||||| merged common ancestors
void
Gecko_UnsetNodeFlags(RawGeckoNodeBorrowed aNode, uint32_t aFlags)
{
=======
void Gecko_UnsetNodeFlags(const nsINode* aNode, uint32_t aFlags) {
>>>>>>> upstream-releases
  const_cast<nsINode*>(aNode)->UnsetFlags(aFlags);
}

<<<<<<< HEAD
void Gecko_NoteDirtyElement(RawGeckoElementBorrowed aElement) {
||||||| merged common ancestors
void
Gecko_NoteDirtyElement(RawGeckoElementBorrowed aElement)
{
=======
void Gecko_NoteDirtyElement(const Element* aElement) {
>>>>>>> upstream-releases
  MOZ_ASSERT(NS_IsMainThread());
  const_cast<Element*>(aElement)->NoteDirtyForServo();
}

<<<<<<< HEAD
void Gecko_NoteDirtySubtreeForInvalidation(RawGeckoElementBorrowed aElement) {
||||||| merged common ancestors
void
Gecko_NoteDirtySubtreeForInvalidation(RawGeckoElementBorrowed aElement)
{
=======
void Gecko_NoteDirtySubtreeForInvalidation(const Element* aElement) {
>>>>>>> upstream-releases
  MOZ_ASSERT(NS_IsMainThread());
  const_cast<Element*>(aElement)->NoteDirtySubtreeForServo();
}

<<<<<<< HEAD
void Gecko_NoteAnimationOnlyDirtyElement(RawGeckoElementBorrowed aElement) {
||||||| merged common ancestors
void
Gecko_NoteAnimationOnlyDirtyElement(RawGeckoElementBorrowed aElement)
{
=======
void Gecko_NoteAnimationOnlyDirtyElement(const Element* aElement) {
>>>>>>> upstream-releases
  MOZ_ASSERT(NS_IsMainThread());
  const_cast<Element*>(aElement)->NoteAnimationOnlyDirtyForServo();
}

bool Gecko_AnimationNameMayBeReferencedFromStyle(
<<<<<<< HEAD
    RawGeckoPresContextBorrowed aPresContext, nsAtom* aName) {
||||||| merged common ancestors
  RawGeckoPresContextBorrowed aPresContext,
  nsAtom* aName)
{
=======
    const nsPresContext* aPresContext, nsAtom* aName) {
>>>>>>> upstream-releases
  MOZ_ASSERT(aPresContext);
  return aPresContext->AnimationManager()->AnimationMayBeReferenced(aName);
}

<<<<<<< HEAD
CSSPseudoElementType Gecko_GetImplementedPseudo(
    RawGeckoElementBorrowed aElement) {
||||||| merged common ancestors
CSSPseudoElementType
Gecko_GetImplementedPseudo(RawGeckoElementBorrowed aElement)
{
=======
PseudoStyleType Gecko_GetImplementedPseudo(const Element* aElement) {
>>>>>>> upstream-releases
  return aElement->GetPseudoElementType();
}

<<<<<<< HEAD
uint32_t Gecko_CalcStyleDifference(ComputedStyleBorrowed aOldStyle,
                                   ComputedStyleBorrowed aNewStyle,
                                   bool* aAnyStyleStructChanged,
                                   bool* aOnlyResetStructsChanged) {
||||||| merged common ancestors
uint32_t
Gecko_CalcStyleDifference(ComputedStyleBorrowed aOldStyle,
                          ComputedStyleBorrowed aNewStyle,
                          bool* aAnyStyleStructChanged,
                          bool* aOnlyResetStructsChanged)
{
=======
uint32_t Gecko_CalcStyleDifference(const ComputedStyle* aOldStyle,
                                   const ComputedStyle* aNewStyle,
                                   bool* aAnyStyleStructChanged,
                                   bool* aOnlyResetStructsChanged) {
>>>>>>> upstream-releases
  MOZ_ASSERT(aOldStyle);
  MOZ_ASSERT(aNewStyle);

  uint32_t equalStructs;
<<<<<<< HEAD
  nsChangeHint result =
      const_cast<ComputedStyle*>(aOldStyle)->CalcStyleDifference(
          const_cast<ComputedStyle*>(aNewStyle), &equalStructs);
||||||| merged common ancestors
  nsChangeHint result = const_cast<ComputedStyle*>(aOldStyle)->
    CalcStyleDifference(const_cast<ComputedStyle*>(aNewStyle), &equalStructs);
=======
  nsChangeHint result =
      aOldStyle->CalcStyleDifference(*aNewStyle, &equalStructs);
>>>>>>> upstream-releases

  *aAnyStyleStructChanged =
      equalStructs != StyleStructConstants::kAllStructsMask;

  const auto kInheritedStructsMask =
      StyleStructConstants::kInheritedStructsMask;
  *aOnlyResetStructsChanged =
      (equalStructs & kInheritedStructsMask) == kInheritedStructsMask;

  return result;
}

const ServoElementSnapshot* Gecko_GetElementSnapshot(
    const ServoElementSnapshotTable* aTable, const Element* aElement) {
  MOZ_ASSERT(aTable);
  MOZ_ASSERT(aElement);

  return aTable->Get(const_cast<Element*>(aElement));
}

bool Gecko_HaveSeenPtr(SeenPtrs* aTable, const void* aPtr) {
  MOZ_ASSERT(NS_IsMainThread());
  MOZ_ASSERT(aTable);
  // Empty Rust allocations are indicated by small values up to the alignment
  // of the relevant type. We shouldn't see anything like that here.
  MOZ_ASSERT(uintptr_t(aPtr) > 16);

  return aTable->HaveSeenPtr(aPtr);
}

<<<<<<< HEAD
RawServoDeclarationBlockStrongBorrowedOrNull Gecko_GetStyleAttrDeclarationBlock(
    RawGeckoElementBorrowed aElement) {
||||||| merged common ancestors
RawServoDeclarationBlockStrongBorrowedOrNull
Gecko_GetStyleAttrDeclarationBlock(RawGeckoElementBorrowed aElement)
{
=======
const StyleStrong<RawServoDeclarationBlock>* Gecko_GetStyleAttrDeclarationBlock(
    const Element* aElement) {
>>>>>>> upstream-releases
  DeclarationBlock* decl = aElement->GetInlineStyleDeclaration();
  if (!decl) {
    return nullptr;
  }
  return decl->RefRawStrong();
}

<<<<<<< HEAD
void Gecko_UnsetDirtyStyleAttr(RawGeckoElementBorrowed aElement) {
||||||| merged common ancestors
void
Gecko_UnsetDirtyStyleAttr(RawGeckoElementBorrowed aElement)
{
=======
void Gecko_UnsetDirtyStyleAttr(const Element* aElement) {
>>>>>>> upstream-releases
  DeclarationBlock* decl = aElement->GetInlineStyleDeclaration();
  if (!decl) {
    return;
  }
  decl->UnsetDirty();
}

<<<<<<< HEAD
static const RawServoDeclarationBlockStrong* AsRefRawStrong(
    const RefPtr<RawServoDeclarationBlock>& aDecl) {
||||||| merged common ancestors
static const RawServoDeclarationBlockStrong*
AsRefRawStrong(const RefPtr<RawServoDeclarationBlock>& aDecl)
{
=======
static const StyleStrong<RawServoDeclarationBlock>* AsRefRawStrong(
    const RefPtr<RawServoDeclarationBlock>& aDecl) {
>>>>>>> upstream-releases
  static_assert(sizeof(RefPtr<RawServoDeclarationBlock>) ==
<<<<<<< HEAD
                    sizeof(RawServoDeclarationBlockStrong),
||||||| merged common ancestors
                sizeof(RawServoDeclarationBlockStrong),
=======
                    sizeof(StyleStrong<RawServoDeclarationBlock>),
>>>>>>> upstream-releases
                "RefPtr should just be a pointer");
  return reinterpret_cast<const StyleStrong<RawServoDeclarationBlock>*>(&aDecl);
}

<<<<<<< HEAD
RawServoDeclarationBlockStrongBorrowedOrNull
Gecko_GetHTMLPresentationAttrDeclarationBlock(
    RawGeckoElementBorrowed aElement) {
||||||| merged common ancestors
RawServoDeclarationBlockStrongBorrowedOrNull
Gecko_GetHTMLPresentationAttrDeclarationBlock(RawGeckoElementBorrowed aElement)
{
=======
const StyleStrong<RawServoDeclarationBlock>*
Gecko_GetHTMLPresentationAttrDeclarationBlock(const Element* aElement) {
>>>>>>> upstream-releases
  const nsMappedAttributes* attrs = aElement->GetMappedAttributes();
  if (!attrs) {
    auto* svg = SVGElement::FromNodeOrNull(aElement);
    if (svg) {
      if (auto decl = svg->GetContentDeclarationBlock()) {
        return decl->RefRawStrong();
      }
    }
    return nullptr;
  }

  return AsRefRawStrong(attrs->GetServoStyle());
}

<<<<<<< HEAD
RawServoDeclarationBlockStrongBorrowedOrNull
Gecko_GetExtraContentStyleDeclarations(RawGeckoElementBorrowed aElement) {
||||||| merged common ancestors
RawServoDeclarationBlockStrongBorrowedOrNull
Gecko_GetExtraContentStyleDeclarations(RawGeckoElementBorrowed aElement)
{
=======
const StyleStrong<RawServoDeclarationBlock>*
Gecko_GetExtraContentStyleDeclarations(const Element* aElement) {
>>>>>>> upstream-releases
  if (!aElement->IsAnyOfHTMLElements(nsGkAtoms::td, nsGkAtoms::th)) {
    return nullptr;
  }
  const HTMLTableCellElement* cell =
      static_cast<const HTMLTableCellElement*>(aElement);
  if (nsMappedAttributes* attrs =
          cell->GetMappedAttributesInheritedFromTable()) {
    return AsRefRawStrong(attrs->GetServoStyle());
  }
  return nullptr;
}

<<<<<<< HEAD
RawServoDeclarationBlockStrongBorrowedOrNull
Gecko_GetUnvisitedLinkAttrDeclarationBlock(RawGeckoElementBorrowed aElement) {
||||||| merged common ancestors
RawServoDeclarationBlockStrongBorrowedOrNull
Gecko_GetUnvisitedLinkAttrDeclarationBlock(RawGeckoElementBorrowed aElement)
{
=======
const StyleStrong<RawServoDeclarationBlock>*
Gecko_GetUnvisitedLinkAttrDeclarationBlock(const Element* aElement) {
>>>>>>> upstream-releases
  nsHTMLStyleSheet* sheet = aElement->OwnerDoc()->GetAttributeStyleSheet();
  if (!sheet) {
    return nullptr;
  }

  return AsRefRawStrong(sheet->GetServoUnvisitedLinkDecl());
}

StyleSheet* Gecko_StyleSheet_Clone(const StyleSheet* aSheet,
                                   const StyleSheet* aNewParentSheet) {
  MOZ_ASSERT(aSheet);
  MOZ_ASSERT(aSheet->GetParentSheet(), "Should only be used for @import");
  MOZ_ASSERT(aNewParentSheet, "Wat");

  RefPtr<StyleSheet> newSheet =
      aSheet->Clone(nullptr, nullptr, nullptr, nullptr);

  // NOTE(emilio): This code runs in the StylesheetInner constructor, which
  // means that the inner pointer of `aNewParentSheet` still points to the old
  // one.
  //
  // So we _don't_ update neither the parent pointer of the stylesheet, nor the
  // child list (yet). This is fixed up in that same constructor.
  return static_cast<StyleSheet*>(newSheet.forget().take());
}

void Gecko_StyleSheet_AddRef(const StyleSheet* aSheet) {
  MOZ_ASSERT(NS_IsMainThread());
  const_cast<StyleSheet*>(aSheet)->AddRef();
}

void Gecko_StyleSheet_Release(const StyleSheet* aSheet) {
  MOZ_ASSERT(NS_IsMainThread());
  const_cast<StyleSheet*>(aSheet)->Release();
}

<<<<<<< HEAD
RawServoDeclarationBlockStrongBorrowedOrNull
Gecko_GetVisitedLinkAttrDeclarationBlock(RawGeckoElementBorrowed aElement) {
||||||| merged common ancestors
RawServoDeclarationBlockStrongBorrowedOrNull
Gecko_GetVisitedLinkAttrDeclarationBlock(RawGeckoElementBorrowed aElement)
{
=======
const StyleStrong<RawServoDeclarationBlock>*
Gecko_GetVisitedLinkAttrDeclarationBlock(const Element* aElement) {
>>>>>>> upstream-releases
  nsHTMLStyleSheet* sheet = aElement->OwnerDoc()->GetAttributeStyleSheet();
  if (!sheet) {
    return nullptr;
  }

  return AsRefRawStrong(sheet->GetServoVisitedLinkDecl());
}

<<<<<<< HEAD
RawServoDeclarationBlockStrongBorrowedOrNull
Gecko_GetActiveLinkAttrDeclarationBlock(RawGeckoElementBorrowed aElement) {
||||||| merged common ancestors
RawServoDeclarationBlockStrongBorrowedOrNull
Gecko_GetActiveLinkAttrDeclarationBlock(RawGeckoElementBorrowed aElement)
{
=======
const StyleStrong<RawServoDeclarationBlock>*
Gecko_GetActiveLinkAttrDeclarationBlock(const Element* aElement) {
>>>>>>> upstream-releases
  nsHTMLStyleSheet* sheet = aElement->OwnerDoc()->GetAttributeStyleSheet();
  if (!sheet) {
    return nullptr;
  }

  return AsRefRawStrong(sheet->GetServoActiveLinkDecl());
}

<<<<<<< HEAD
static CSSPseudoElementType GetPseudoTypeFromElementForAnimation(
    const Element*& aElementOrPseudo) {
||||||| merged common ancestors
static CSSPseudoElementType
GetPseudoTypeFromElementForAnimation(const Element*& aElementOrPseudo) {
=======
static PseudoStyleType GetPseudoTypeFromElementForAnimation(
    const Element*& aElementOrPseudo) {
>>>>>>> upstream-releases
  if (aElementOrPseudo->IsGeneratedContentContainerForBefore()) {
    aElementOrPseudo = aElementOrPseudo->GetParent()->AsElement();
    return PseudoStyleType::before;
  }

  if (aElementOrPseudo->IsGeneratedContentContainerForAfter()) {
    aElementOrPseudo = aElementOrPseudo->GetParent()->AsElement();
    return PseudoStyleType::after;
  }

  if (aElementOrPseudo->IsGeneratedContentContainerForMarker()) {
    aElementOrPseudo = aElementOrPseudo->GetParent()->AsElement();
    return PseudoStyleType::marker;
  }

  return PseudoStyleType::NotPseudo;
}

<<<<<<< HEAD
bool Gecko_GetAnimationRule(
    RawGeckoElementBorrowed aElement,
    EffectCompositor::CascadeLevel aCascadeLevel,
    RawServoAnimationValueMapBorrowedMut aAnimationValues) {
||||||| merged common ancestors
bool
Gecko_GetAnimationRule(RawGeckoElementBorrowed aElement,
                       EffectCompositor::CascadeLevel aCascadeLevel,
                       RawServoAnimationValueMapBorrowedMut aAnimationValues)
{
=======
bool Gecko_GetAnimationRule(const Element* aElement,
                            EffectCompositor::CascadeLevel aCascadeLevel,
                            RawServoAnimationValueMap* aAnimationValues) {
>>>>>>> upstream-releases
  MOZ_ASSERT(aElement);

  Document* doc = aElement->GetComposedDoc();
  if (!doc) {
    return false;
  }
  nsPresContext* presContext = doc->GetPresContext();
  if (!presContext || !presContext->IsDynamic()) {
    // For print or print preview, ignore animations.
    return false;
  }

<<<<<<< HEAD
  CSSPseudoElementType pseudoType =
      GetPseudoTypeFromElementForAnimation(aElement);
||||||| merged common ancestors
  CSSPseudoElementType pseudoType =
    GetPseudoTypeFromElementForAnimation(aElement);
=======
  PseudoStyleType pseudoType = GetPseudoTypeFromElementForAnimation(aElement);
>>>>>>> upstream-releases

  return presContext->EffectCompositor()->GetServoAnimationRule(
      aElement, pseudoType, aCascadeLevel, aAnimationValues);
}

<<<<<<< HEAD
bool Gecko_StyleAnimationsEquals(RawGeckoStyleAnimationListBorrowed aA,
                                 RawGeckoStyleAnimationListBorrowed aB) {
||||||| merged common ancestors
bool
Gecko_StyleAnimationsEquals(RawGeckoStyleAnimationListBorrowed aA,
                            RawGeckoStyleAnimationListBorrowed aB)
{
=======
bool Gecko_StyleAnimationsEquals(const nsStyleAutoArray<StyleAnimation>* aA,
                                 const nsStyleAutoArray<StyleAnimation>* aB) {
>>>>>>> upstream-releases
  return *aA == *aB;
}

<<<<<<< HEAD
void Gecko_CopyAnimationNames(RawGeckoStyleAnimationListBorrowedMut aDest,
                              RawGeckoStyleAnimationListBorrowed aSrc) {
||||||| merged common ancestors
void
Gecko_CopyAnimationNames(RawGeckoStyleAnimationListBorrowedMut aDest,
                         RawGeckoStyleAnimationListBorrowed aSrc)
{
=======
void Gecko_CopyAnimationNames(nsStyleAutoArray<StyleAnimation>* aDest,
                              const nsStyleAutoArray<StyleAnimation>* aSrc) {
>>>>>>> upstream-releases
  size_t srcLength = aSrc->Length();
  aDest->EnsureLengthAtLeast(srcLength);

  for (size_t index = 0; index < srcLength; index++) {
    (*aDest)[index].SetName((*aSrc)[index].GetName());
  }
}

void Gecko_SetAnimationName(StyleAnimation* aStyleAnimation, nsAtom* aAtom) {
  MOZ_ASSERT(aStyleAnimation);
  aStyleAnimation->SetName(already_AddRefed<nsAtom>(aAtom));
}

<<<<<<< HEAD
void Gecko_UpdateAnimations(RawGeckoElementBorrowed aElement,
                            ComputedStyleBorrowedOrNull aOldComputedData,
                            ComputedStyleBorrowedOrNull aComputedData,
                            UpdateAnimationsTasks aTasks) {
||||||| merged common ancestors
void
Gecko_UpdateAnimations(RawGeckoElementBorrowed aElement,
                       ComputedStyleBorrowedOrNull aOldComputedData,
                       ComputedStyleBorrowedOrNull aComputedData,
                       UpdateAnimationsTasks aTasks)
{
=======
void Gecko_UpdateAnimations(const Element* aElement,
                            const ComputedStyle* aOldComputedData,
                            const ComputedStyle* aComputedData,
                            UpdateAnimationsTasks aTasks) {
>>>>>>> upstream-releases
  MOZ_ASSERT(NS_IsMainThread());
  MOZ_ASSERT(aElement);

  if (!aElement->IsInComposedDoc()) {
    return;
  }

  nsPresContext* presContext = nsContentUtils::GetContextForContent(aElement);
  if (!presContext || !presContext->IsDynamic()) {
    return;
  }

  nsAutoAnimationMutationBatch mb(aElement->OwnerDoc());

<<<<<<< HEAD
  CSSPseudoElementType pseudoType =
      GetPseudoTypeFromElementForAnimation(aElement);
||||||| merged common ancestors
  CSSPseudoElementType pseudoType =
    GetPseudoTypeFromElementForAnimation(aElement);
=======
  PseudoStyleType pseudoType = GetPseudoTypeFromElementForAnimation(aElement);
>>>>>>> upstream-releases

  if (aTasks & UpdateAnimationsTasks::CSSAnimations) {
<<<<<<< HEAD
    presContext->AnimationManager()->UpdateAnimations(
        const_cast<dom::Element*>(aElement), pseudoType, aComputedData);
||||||| merged common ancestors
    presContext->AnimationManager()->
      UpdateAnimations(const_cast<dom::Element*>(aElement), pseudoType,
                       aComputedData);
=======
    presContext->AnimationManager()->UpdateAnimations(
        const_cast<Element*>(aElement), pseudoType, aComputedData);
>>>>>>> upstream-releases
  }

  // aComputedData might be nullptr if the target element is now in a
  // display:none subtree. We still call Gecko_UpdateAnimations in this case
  // because we need to stop CSS animations in the display:none subtree.
  // However, we don't need to update transitions since they are stopped by
  // RestyleManager::AnimationsWithDestroyedFrame so we just return early
  // here.
  if (!aComputedData) {
    return;
  }

  if (aTasks & UpdateAnimationsTasks::CSSTransitions) {
    MOZ_ASSERT(aOldComputedData);
<<<<<<< HEAD
    presContext->TransitionManager()->UpdateTransitions(
        const_cast<dom::Element*>(aElement), pseudoType, *aOldComputedData,
        *aComputedData);
||||||| merged common ancestors
    presContext->TransitionManager()->
      UpdateTransitions(const_cast<dom::Element*>(aElement), pseudoType,
                        *aOldComputedData,
                        *aComputedData);
=======
    presContext->TransitionManager()->UpdateTransitions(
        const_cast<Element*>(aElement), pseudoType, *aOldComputedData,
        *aComputedData);
>>>>>>> upstream-releases
  }

  if (aTasks & UpdateAnimationsTasks::EffectProperties) {
    presContext->EffectCompositor()->UpdateEffectProperties(
<<<<<<< HEAD
        aComputedData, const_cast<dom::Element*>(aElement), pseudoType);
||||||| merged common ancestors
      aComputedData, const_cast<dom::Element*>(aElement), pseudoType);
=======
        aComputedData, const_cast<Element*>(aElement), pseudoType);
>>>>>>> upstream-releases
  }

  if (aTasks & UpdateAnimationsTasks::CascadeResults) {
    EffectSet* effectSet = EffectSet::GetEffectSet(aElement, pseudoType);
    // CSS animations/transitions might have been destroyed as part of the above
    // steps so before updating cascade results, we check if there are still any
    // animations to update.
    if (effectSet) {
      // We call UpdateCascadeResults directly (intead of
      // MaybeUpdateCascadeResults) since we know for sure that the cascade has
      // changed, but we were unable to call MarkCascadeUpdated when we noticed
      // it since we avoid mutating state as part of the Servo parallel
      // traversal.
      presContext->EffectCompositor()->UpdateCascadeResults(
          *effectSet, const_cast<Element*>(aElement), pseudoType);
    }
  }

  if (aTasks & UpdateAnimationsTasks::DisplayChangedFromNone) {
    presContext->EffectCompositor()->RequestRestyle(
        const_cast<Element*>(aElement), pseudoType,
        EffectCompositor::RestyleType::Standard,
        EffectCompositor::CascadeLevel::Animations);
  }
}

<<<<<<< HEAD
size_t Gecko_GetAnimationEffectCount(RawGeckoElementBorrowed aElementOrPseudo) {
  CSSPseudoElementType pseudoType =
      GetPseudoTypeFromElementForAnimation(aElementOrPseudo);
||||||| merged common ancestors
size_t
Gecko_GetAnimationEffectCount(RawGeckoElementBorrowed aElementOrPseudo)
{
  CSSPseudoElementType pseudoType =
    GetPseudoTypeFromElementForAnimation(aElementOrPseudo);
=======
size_t Gecko_GetAnimationEffectCount(const Element* aElementOrPseudo) {
  PseudoStyleType pseudoType =
      GetPseudoTypeFromElementForAnimation(aElementOrPseudo);
>>>>>>> upstream-releases

  EffectSet* effectSet = EffectSet::GetEffectSet(aElementOrPseudo, pseudoType);
  return effectSet ? effectSet->Count() : 0;
}

<<<<<<< HEAD
bool Gecko_ElementHasAnimations(RawGeckoElementBorrowed aElement) {
  CSSPseudoElementType pseudoType =
      GetPseudoTypeFromElementForAnimation(aElement);
||||||| merged common ancestors
bool
Gecko_ElementHasAnimations(RawGeckoElementBorrowed aElement)
{
  CSSPseudoElementType pseudoType =
    GetPseudoTypeFromElementForAnimation(aElement);
=======
bool Gecko_ElementHasAnimations(const Element* aElement) {
  PseudoStyleType pseudoType = GetPseudoTypeFromElementForAnimation(aElement);
>>>>>>> upstream-releases

  return !!EffectSet::GetEffectSet(aElement, pseudoType);
}

<<<<<<< HEAD
bool Gecko_ElementHasCSSAnimations(RawGeckoElementBorrowed aElement) {
  CSSPseudoElementType pseudoType =
      GetPseudoTypeFromElementForAnimation(aElement);
||||||| merged common ancestors
bool
Gecko_ElementHasCSSAnimations(RawGeckoElementBorrowed aElement)
{
  CSSPseudoElementType pseudoType =
    GetPseudoTypeFromElementForAnimation(aElement);
=======
bool Gecko_ElementHasCSSAnimations(const Element* aElement) {
  PseudoStyleType pseudoType = GetPseudoTypeFromElementForAnimation(aElement);
>>>>>>> upstream-releases
  nsAnimationManager::CSSAnimationCollection* collection =
      nsAnimationManager::CSSAnimationCollection ::GetAnimationCollection(
          aElement, pseudoType);

  return collection && !collection->mAnimations.IsEmpty();
}

<<<<<<< HEAD
bool Gecko_ElementHasCSSTransitions(RawGeckoElementBorrowed aElement) {
  CSSPseudoElementType pseudoType =
      GetPseudoTypeFromElementForAnimation(aElement);
||||||| merged common ancestors
bool
Gecko_ElementHasCSSTransitions(RawGeckoElementBorrowed aElement)
{
  CSSPseudoElementType pseudoType =
    GetPseudoTypeFromElementForAnimation(aElement);
=======
bool Gecko_ElementHasCSSTransitions(const Element* aElement) {
  PseudoStyleType pseudoType = GetPseudoTypeFromElementForAnimation(aElement);
>>>>>>> upstream-releases
  nsTransitionManager::CSSTransitionCollection* collection =
      nsTransitionManager::CSSTransitionCollection ::GetAnimationCollection(
          aElement, pseudoType);

  return collection && !collection->mAnimations.IsEmpty();
}

<<<<<<< HEAD
size_t Gecko_ElementTransitions_Length(RawGeckoElementBorrowed aElement) {
  CSSPseudoElementType pseudoType =
      GetPseudoTypeFromElementForAnimation(aElement);
||||||| merged common ancestors
size_t
Gecko_ElementTransitions_Length(RawGeckoElementBorrowed aElement)
{
  CSSPseudoElementType pseudoType =
    GetPseudoTypeFromElementForAnimation(aElement);
=======
size_t Gecko_ElementTransitions_Length(const Element* aElement) {
  PseudoStyleType pseudoType = GetPseudoTypeFromElementForAnimation(aElement);
>>>>>>> upstream-releases
  nsTransitionManager::CSSTransitionCollection* collection =
      nsTransitionManager::CSSTransitionCollection ::GetAnimationCollection(
          aElement, pseudoType);

  return collection ? collection->mAnimations.Length() : 0;
}

<<<<<<< HEAD
static CSSTransition* GetCurrentTransitionAt(RawGeckoElementBorrowed aElement,
                                             size_t aIndex) {
  CSSPseudoElementType pseudoType =
      GetPseudoTypeFromElementForAnimation(aElement);
||||||| merged common ancestors
static CSSTransition*
GetCurrentTransitionAt(RawGeckoElementBorrowed aElement, size_t aIndex)
{
  CSSPseudoElementType pseudoType =
    GetPseudoTypeFromElementForAnimation(aElement);
=======
static CSSTransition* GetCurrentTransitionAt(const Element* aElement,
                                             size_t aIndex) {
  PseudoStyleType pseudoType = GetPseudoTypeFromElementForAnimation(aElement);
>>>>>>> upstream-releases
  nsTransitionManager::CSSTransitionCollection* collection =
      nsTransitionManager::CSSTransitionCollection ::GetAnimationCollection(
          aElement, pseudoType);
  if (!collection) {
    return nullptr;
  }
  nsTArray<RefPtr<CSSTransition>>& transitions = collection->mAnimations;
  return aIndex < transitions.Length() ? transitions[aIndex].get() : nullptr;
}

<<<<<<< HEAD
nsCSSPropertyID Gecko_ElementTransitions_PropertyAt(
    RawGeckoElementBorrowed aElement, size_t aIndex) {
||||||| merged common ancestors
nsCSSPropertyID
Gecko_ElementTransitions_PropertyAt(RawGeckoElementBorrowed aElement,
                                    size_t aIndex)
{
=======
nsCSSPropertyID Gecko_ElementTransitions_PropertyAt(const Element* aElement,
                                                    size_t aIndex) {
>>>>>>> upstream-releases
  CSSTransition* transition = GetCurrentTransitionAt(aElement, aIndex);
  return transition ? transition->TransitionProperty()
                    : nsCSSPropertyID::eCSSProperty_UNKNOWN;
}

<<<<<<< HEAD
RawServoAnimationValueBorrowedOrNull Gecko_ElementTransitions_EndValueAt(
    RawGeckoElementBorrowed aElement, size_t aIndex) {
  CSSTransition* transition = GetCurrentTransitionAt(aElement, aIndex);
||||||| merged common ancestors
RawServoAnimationValueBorrowedOrNull
Gecko_ElementTransitions_EndValueAt(RawGeckoElementBorrowed aElement,
                                    size_t aIndex)
{
  CSSTransition* transition = GetCurrentTransitionAt(aElement,
                                                     aIndex);
=======
const RawServoAnimationValue* Gecko_ElementTransitions_EndValueAt(
    const Element* aElement, size_t aIndex) {
  CSSTransition* transition = GetCurrentTransitionAt(aElement, aIndex);
>>>>>>> upstream-releases
  return transition ? transition->ToValue().mServo.get() : nullptr;
}

<<<<<<< HEAD
double Gecko_GetProgressFromComputedTiming(
    RawGeckoComputedTimingBorrowed aComputedTiming) {
  return aComputedTiming->mProgress.Value();
||||||| merged common ancestors
double
Gecko_GetProgressFromComputedTiming(RawGeckoComputedTimingBorrowed aComputedTiming)
{
  return aComputedTiming->mProgress.Value();
=======
double Gecko_GetProgressFromComputedTiming(const ComputedTiming* aTiming) {
  return aTiming->mProgress.Value();
>>>>>>> upstream-releases
}

<<<<<<< HEAD
double Gecko_GetPositionInSegment(
    RawGeckoAnimationPropertySegmentBorrowed aSegment, double aProgress,
    ComputedTimingFunction::BeforeFlag aBeforeFlag) {
||||||| merged common ancestors
double
Gecko_GetPositionInSegment(RawGeckoAnimationPropertySegmentBorrowed aSegment,
                           double aProgress,
                           ComputedTimingFunction::BeforeFlag aBeforeFlag)
{
=======
double Gecko_GetPositionInSegment(
    const AnimationPropertySegment* aSegment, double aProgress,
    ComputedTimingFunction::BeforeFlag aBeforeFlag) {
>>>>>>> upstream-releases
  MOZ_ASSERT(aSegment->mFromKey < aSegment->mToKey,
             "The segment from key should be less than to key");

  double positionInSegment = (aProgress - aSegment->mFromKey) /
                             // To avoid floating precision inaccuracies, make
                             // sure we calculate both the numerator and
                             // denominator using double precision.
                             (double(aSegment->mToKey) - aSegment->mFromKey);

  return ComputedTimingFunction::GetPortion(aSegment->mTimingFunction,
                                            positionInSegment, aBeforeFlag);
}

<<<<<<< HEAD
RawServoAnimationValueBorrowedOrNull Gecko_AnimationGetBaseStyle(
    void* aBaseStyles, nsCSSPropertyID aProperty) {
  auto base =
      static_cast<nsRefPtrHashtable<nsUint32HashKey, RawServoAnimationValue>*>(
          aBaseStyles);
||||||| merged common ancestors
RawServoAnimationValueBorrowedOrNull
Gecko_AnimationGetBaseStyle(void* aBaseStyles, nsCSSPropertyID aProperty)
{
  auto base =
    static_cast<nsRefPtrHashtable<nsUint32HashKey, RawServoAnimationValue>*>
      (aBaseStyles);
=======
const RawServoAnimationValue* Gecko_AnimationGetBaseStyle(
    const RawServoAnimationValueTable* aBaseStyles, nsCSSPropertyID aProperty) {
  auto base = reinterpret_cast<
      const nsRefPtrHashtable<nsUint32HashKey, RawServoAnimationValue>*>(
      aBaseStyles);
>>>>>>> upstream-releases
  return base->GetWeak(aProperty);
}

void Gecko_FillAllImageLayers(nsStyleImageLayers* aLayers, uint32_t aMaxLen) {
  aLayers->FillAllLayers(aMaxLen);
}

<<<<<<< HEAD
bool Gecko_IsDocumentBody(RawGeckoElementBorrowed aElement) {
  nsIDocument* doc = aElement->GetUncomposedDoc();
||||||| merged common ancestors
bool
Gecko_IsDocumentBody(RawGeckoElementBorrowed aElement)
{
  nsIDocument* doc = aElement->GetUncomposedDoc();
=======
bool Gecko_IsDocumentBody(const Element* aElement) {
  Document* doc = aElement->GetUncomposedDoc();
>>>>>>> upstream-releases
  return doc && doc->GetBodyElement() == aElement;
}

<<<<<<< HEAD
nscolor Gecko_GetLookAndFeelSystemColor(
    int32_t aId, RawGeckoPresContextBorrowed aPresContext) {
  bool useStandinsForNativeColors = aPresContext && !aPresContext->IsChrome();
||||||| merged common ancestors
nscolor
Gecko_GetLookAndFeelSystemColor(int32_t aId,
                                RawGeckoPresContextBorrowed aPresContext)
{
  bool useStandinsForNativeColors = aPresContext && !aPresContext->IsChrome();
=======
nscolor Gecko_GetLookAndFeelSystemColor(int32_t aId, const Document* aDoc) {
  bool useStandinsForNativeColors = !nsContentUtils::IsChromeDoc(aDoc);
>>>>>>> upstream-releases
  nscolor result;
  LookAndFeel::ColorID colorId = static_cast<LookAndFeel::ColorID>(aId);
  AutoWriteLock guard(*sServoFFILock);
  LookAndFeel::GetColor(colorId, useStandinsForNativeColors, &result);
  return result;
}

<<<<<<< HEAD
bool Gecko_MatchLang(RawGeckoElementBorrowed aElement, nsAtom* aOverrideLang,
                     bool aHasOverrideLang, const char16_t* aValue) {
||||||| merged common ancestors
bool
Gecko_MatchLang(RawGeckoElementBorrowed aElement,
                nsAtom* aOverrideLang,
                bool aHasOverrideLang,
                const char16_t* aValue)
{
=======
bool Gecko_MatchLang(const Element* aElement, nsAtom* aOverrideLang,
                     bool aHasOverrideLang, const char16_t* aValue) {
>>>>>>> upstream-releases
  MOZ_ASSERT(!(aOverrideLang && !aHasOverrideLang),
             "aHasOverrideLang should only be set when aOverrideLang is null");
  MOZ_ASSERT(aValue, "null lang parameter");
  if (!aValue || !*aValue) {
    return false;
  }

  // We have to determine the language of the current element.  Since
  // this is currently no property and since the language is inherited
  // from the parent we have to be prepared to look at all parent
  // nodes.  The language itself is encoded in the LANG attribute.
  if (auto* language = aHasOverrideLang ? aOverrideLang : aElement->GetLang()) {
    return nsStyleUtil::DashMatchCompare(
        nsDependentAtomString(language), nsDependentString(aValue),
        nsASCIICaseInsensitiveStringComparator());
  }

  // Try to get the language from the HTTP header or if this
  // is missing as well from the preferences.
  // The content language can be a comma-separated list of
  // language codes.
  nsAutoString language;
  aElement->OwnerDoc()->GetContentLanguage(language);

  nsDependentString langString(aValue);
  language.StripWhitespace();
  for (auto const& lang : language.Split(char16_t(','))) {
    if (nsStyleUtil::DashMatchCompare(
            lang, langString, nsASCIICaseInsensitiveStringComparator())) {
      return true;
    }
  }
  return false;
}

<<<<<<< HEAD
nsAtom* Gecko_GetXMLLangValue(RawGeckoElementBorrowed aElement) {
||||||| merged common ancestors
nsAtom*
Gecko_GetXMLLangValue(RawGeckoElementBorrowed aElement)
{
=======
nsAtom* Gecko_GetXMLLangValue(const Element* aElement) {
>>>>>>> upstream-releases
  const nsAttrValue* attr =
      aElement->GetParsedAttr(nsGkAtoms::lang, kNameSpaceID_XML);

  if (!attr) {
    return nullptr;
  }

  MOZ_ASSERT(attr->Type() == nsAttrValue::eAtom);

  RefPtr<nsAtom> atom = attr->GetAtomValue();
  return atom.forget().take();
}

<<<<<<< HEAD
nsIDocument::DocumentTheme Gecko_GetDocumentLWTheme(
    const nsIDocument* aDocument) {
||||||| merged common ancestors
nsIDocument::DocumentTheme
Gecko_GetDocumentLWTheme(const nsIDocument* aDocument)
{
=======
Document::DocumentTheme Gecko_GetDocumentLWTheme(const Document* aDocument) {
>>>>>>> upstream-releases
  return aDocument->ThreadSafeGetDocumentLWTheme();
}

<<<<<<< HEAD
bool Gecko_IsTableBorderNonzero(RawGeckoElementBorrowed aElement) {
||||||| merged common ancestors
bool
Gecko_IsTableBorderNonzero(RawGeckoElementBorrowed aElement)
{
=======
const PreferenceSheet::Prefs* Gecko_GetPrefSheetPrefs(const Document* aDoc) {
  return &PreferenceSheet::PrefsFor(*aDoc);
}

bool Gecko_IsTableBorderNonzero(const Element* aElement) {
>>>>>>> upstream-releases
  if (!aElement->IsHTMLElement(nsGkAtoms::table)) {
    return false;
  }
  const nsAttrValue* val = aElement->GetParsedAttr(nsGkAtoms::border);
  return val &&
         (val->Type() != nsAttrValue::eInteger || val->GetIntegerValue() != 0);
}

<<<<<<< HEAD
bool Gecko_IsBrowserFrame(RawGeckoElementBorrowed aElement) {
||||||| merged common ancestors
bool
Gecko_IsBrowserFrame(RawGeckoElementBorrowed aElement)
{
=======
bool Gecko_IsBrowserFrame(const Element* aElement) {
>>>>>>> upstream-releases
  nsIMozBrowserFrame* browserFrame =
      const_cast<Element*>(aElement)->GetAsMozBrowserFrame();
  return browserFrame && browserFrame->GetReallyIsBrowser();
}

template <typename Implementor>
static nsAtom* LangValue(Implementor* aElement) {
  // TODO(emilio): Deduplicate a bit with nsIContent::GetLang().
  const nsAttrValue* attr =
      aElement->GetParsedAttr(nsGkAtoms::lang, kNameSpaceID_XML);
  if (!attr && aElement->SupportsLangAttr()) {
    attr = aElement->GetParsedAttr(nsGkAtoms::lang);
  }

  if (!attr) {
    return nullptr;
  }

  MOZ_ASSERT(attr->Type() == nsAttrValue::eAtom);
  RefPtr<nsAtom> atom = attr->GetAtomValue();
  return atom.forget().take();
}

template <typename Implementor, typename MatchFn>
static bool DoMatch(Implementor* aElement, nsAtom* aNS, nsAtom* aName,
                    MatchFn aMatch) {
  if (MOZ_LIKELY(aNS)) {
    int32_t ns = aNS == nsGkAtoms::_empty
                     ? kNameSpaceID_None
                     : nsContentUtils::NameSpaceManager()->GetNameSpaceID(
                           aNS, aElement->IsInChromeDocument());

    MOZ_ASSERT(ns == nsContentUtils::NameSpaceManager()->GetNameSpaceID(
                         aNS, aElement->IsInChromeDocument()));
    NS_ENSURE_TRUE(ns != kNameSpaceID_Unknown, false);
    const nsAttrValue* value = aElement->GetParsedAttr(aName, ns);
    return value && aMatch(value);
  }

  // No namespace means any namespace - we have to check them all. :-(
  BorrowedAttrInfo attrInfo;
  for (uint32_t i = 0; (attrInfo = aElement->GetAttrInfoAt(i)); ++i) {
    if (attrInfo.mName->LocalName() != aName) {
      continue;
    }
    if (aMatch(attrInfo.mValue)) {
      return true;
    }
  }
  return false;
}

template <typename Implementor>
static bool HasAttr(Implementor* aElement, nsAtom* aNS, nsAtom* aName) {
  auto match = [](const nsAttrValue* aValue) { return true; };
  return DoMatch(aElement, aNS, aName, match);
}

template <typename Implementor>
static bool AttrEquals(Implementor* aElement, nsAtom* aNS, nsAtom* aName,
                       nsAtom* aStr, bool aIgnoreCase) {
  auto match = [aStr, aIgnoreCase](const nsAttrValue* aValue) {
    return aValue->Equals(aStr, aIgnoreCase ? eIgnoreCase : eCaseMatters);
  };
  return DoMatch(aElement, aNS, aName, match);
}

#define WITH_COMPARATOR(ignore_case_, c_, expr_) \
  if (ignore_case_) {                            \
    const nsCaseInsensitiveStringComparator c_;  \
    return expr_;                                \
  } else {                                       \
    const nsDefaultStringComparator c_;          \
    return expr_;                                \
  }

template <typename Implementor>
static bool AttrDashEquals(Implementor* aElement, nsAtom* aNS, nsAtom* aName,
                           nsAtom* aStr, bool aIgnoreCase) {
  auto match = [aStr, aIgnoreCase](const nsAttrValue* aValue) {
    nsAutoString str;
    aValue->ToString(str);
    WITH_COMPARATOR(
        aIgnoreCase, c,
        nsStyleUtil::DashMatchCompare(str, nsDependentAtomString(aStr), c))
  };
  return DoMatch(aElement, aNS, aName, match);
}

template <typename Implementor>
static bool AttrIncludes(Implementor* aElement, nsAtom* aNS, nsAtom* aName,
                         nsAtom* aStr, bool aIgnoreCase) {
  auto match = [aStr, aIgnoreCase](const nsAttrValue* aValue) {
    nsAutoString str;
    aValue->ToString(str);
    WITH_COMPARATOR(
        aIgnoreCase, c,
        nsStyleUtil::ValueIncludes(str, nsDependentAtomString(aStr), c))
  };
  return DoMatch(aElement, aNS, aName, match);
}

template <typename Implementor>
static bool AttrHasSubstring(Implementor* aElement, nsAtom* aNS, nsAtom* aName,
                             nsAtom* aStr, bool aIgnoreCase) {
  auto match = [aStr, aIgnoreCase](const nsAttrValue* aValue) {
    nsAutoString str;
    aValue->ToString(str);
    WITH_COMPARATOR(aIgnoreCase, c,
                    FindInReadable(nsDependentAtomString(aStr), str, c))
  };
  return DoMatch(aElement, aNS, aName, match);
}

template <typename Implementor>
static bool AttrHasPrefix(Implementor* aElement, nsAtom* aNS, nsAtom* aName,
                          nsAtom* aStr, bool aIgnoreCase) {
  auto match = [aStr, aIgnoreCase](const nsAttrValue* aValue) {
    nsAutoString str;
    aValue->ToString(str);
    WITH_COMPARATOR(aIgnoreCase, c,
                    StringBeginsWith(str, nsDependentAtomString(aStr), c))
  };
  return DoMatch(aElement, aNS, aName, match);
}

template <typename Implementor>
static bool AttrHasSuffix(Implementor* aElement, nsAtom* aNS, nsAtom* aName,
                          nsAtom* aStr, bool aIgnoreCase) {
  auto match = [aStr, aIgnoreCase](const nsAttrValue* aValue) {
    nsAutoString str;
    aValue->ToString(str);
    WITH_COMPARATOR(aIgnoreCase, c,
                    StringEndsWith(str, nsDependentAtomString(aStr), c))
  };
  return DoMatch(aElement, aNS, aName, match);
}

<<<<<<< HEAD
#define SERVO_IMPL_ELEMENT_ATTR_MATCHING_FUNCTIONS(prefix_, implementor_)      \
  nsAtom* prefix_##LangValue(implementor_ aElement) {                          \
    return LangValue(aElement);                                                \
  }                                                                            \
  bool prefix_##HasAttr(implementor_ aElement, nsAtom* aNS, nsAtom* aName) {   \
    return HasAttr(aElement, aNS, aName);                                      \
  }                                                                            \
  bool prefix_##AttrEquals(implementor_ aElement, nsAtom* aNS, nsAtom* aName,  \
                           nsAtom* aStr, bool aIgnoreCase) {                   \
    return AttrEquals(aElement, aNS, aName, aStr, aIgnoreCase);                \
  }                                                                            \
  bool prefix_##AttrDashEquals(implementor_ aElement, nsAtom* aNS,             \
                               nsAtom* aName, nsAtom* aStr,                    \
                               bool aIgnoreCase) {                             \
    return AttrDashEquals(aElement, aNS, aName, aStr, aIgnoreCase);            \
  }                                                                            \
  bool prefix_##AttrIncludes(implementor_ aElement, nsAtom* aNS,               \
                             nsAtom* aName, nsAtom* aStr, bool aIgnoreCase) {  \
    return AttrIncludes(aElement, aNS, aName, aStr, aIgnoreCase);              \
  }                                                                            \
  bool prefix_##AttrHasSubstring(implementor_ aElement, nsAtom* aNS,           \
                                 nsAtom* aName, nsAtom* aStr,                  \
                                 bool aIgnoreCase) {                           \
    return AttrHasSubstring(aElement, aNS, aName, aStr, aIgnoreCase);          \
  }                                                                            \
  bool prefix_##AttrHasPrefix(implementor_ aElement, nsAtom* aNS,              \
                              nsAtom* aName, nsAtom* aStr, bool aIgnoreCase) { \
    return AttrHasPrefix(aElement, aNS, aName, aStr, aIgnoreCase);             \
  }                                                                            \
  bool prefix_##AttrHasSuffix(implementor_ aElement, nsAtom* aNS,              \
                              nsAtom* aName, nsAtom* aStr, bool aIgnoreCase) { \
    return AttrHasSuffix(aElement, aNS, aName, aStr, aIgnoreCase);             \
  }

SERVO_IMPL_ELEMENT_ATTR_MATCHING_FUNCTIONS(Gecko_, RawGeckoElementBorrowed)
SERVO_IMPL_ELEMENT_ATTR_MATCHING_FUNCTIONS(Gecko_Snapshot,
                                           const ServoElementSnapshot*)
||||||| merged common ancestors
#define SERVO_IMPL_ELEMENT_ATTR_MATCHING_FUNCTIONS(prefix_, implementor_)        \
  nsAtom* prefix_##LangValue(implementor_ aElement)                              \
  {                                                                              \
    return LangValue(aElement);                                                  \
  }                                                                              \
  bool prefix_##HasAttr(implementor_ aElement, nsAtom* aNS, nsAtom* aName)       \
  {                                                                              \
    return HasAttr(aElement, aNS, aName);                                        \
  }                                                                              \
  bool prefix_##AttrEquals(implementor_ aElement, nsAtom* aNS,                   \
                           nsAtom* aName, nsAtom* aStr, bool aIgnoreCase)        \
  {                                                                              \
    return AttrEquals(aElement, aNS, aName, aStr, aIgnoreCase);                  \
  }                                                                              \
  bool prefix_##AttrDashEquals(implementor_ aElement, nsAtom* aNS,               \
                               nsAtom* aName, nsAtom* aStr, bool aIgnoreCase)    \
  {                                                                              \
    return AttrDashEquals(aElement, aNS, aName, aStr, aIgnoreCase);              \
  }                                                                              \
  bool prefix_##AttrIncludes(implementor_ aElement, nsAtom* aNS,                 \
                             nsAtom* aName, nsAtom* aStr, bool aIgnoreCase)      \
  {                                                                              \
    return AttrIncludes(aElement, aNS, aName, aStr, aIgnoreCase);                \
  }                                                                              \
  bool prefix_##AttrHasSubstring(implementor_ aElement, nsAtom* aNS,             \
                                 nsAtom* aName, nsAtom* aStr, bool aIgnoreCase)  \
  {                                                                              \
    return AttrHasSubstring(aElement, aNS, aName, aStr, aIgnoreCase);            \
  }                                                                              \
  bool prefix_##AttrHasPrefix(implementor_ aElement, nsAtom* aNS,                \
                              nsAtom* aName, nsAtom* aStr, bool aIgnoreCase)     \
  {                                                                              \
    return AttrHasPrefix(aElement, aNS, aName, aStr, aIgnoreCase);               \
  }                                                                              \
  bool prefix_##AttrHasSuffix(implementor_ aElement, nsAtom* aNS,                \
                              nsAtom* aName, nsAtom* aStr, bool aIgnoreCase)     \
  {                                                                              \
    return AttrHasSuffix(aElement, aNS, aName, aStr, aIgnoreCase);               \
  }


SERVO_IMPL_ELEMENT_ATTR_MATCHING_FUNCTIONS(Gecko_, RawGeckoElementBorrowed)
SERVO_IMPL_ELEMENT_ATTR_MATCHING_FUNCTIONS(Gecko_Snapshot, const ServoElementSnapshot*)
=======
#define SERVO_IMPL_ELEMENT_ATTR_MATCHING_FUNCTIONS(prefix_, implementor_)      \
  nsAtom* prefix_##LangValue(implementor_ aElement) {                          \
    return LangValue(aElement);                                                \
  }                                                                            \
  bool prefix_##HasAttr(implementor_ aElement, nsAtom* aNS, nsAtom* aName) {   \
    return HasAttr(aElement, aNS, aName);                                      \
  }                                                                            \
  bool prefix_##AttrEquals(implementor_ aElement, nsAtom* aNS, nsAtom* aName,  \
                           nsAtom* aStr, bool aIgnoreCase) {                   \
    return AttrEquals(aElement, aNS, aName, aStr, aIgnoreCase);                \
  }                                                                            \
  bool prefix_##AttrDashEquals(implementor_ aElement, nsAtom* aNS,             \
                               nsAtom* aName, nsAtom* aStr,                    \
                               bool aIgnoreCase) {                             \
    return AttrDashEquals(aElement, aNS, aName, aStr, aIgnoreCase);            \
  }                                                                            \
  bool prefix_##AttrIncludes(implementor_ aElement, nsAtom* aNS,               \
                             nsAtom* aName, nsAtom* aStr, bool aIgnoreCase) {  \
    return AttrIncludes(aElement, aNS, aName, aStr, aIgnoreCase);              \
  }                                                                            \
  bool prefix_##AttrHasSubstring(implementor_ aElement, nsAtom* aNS,           \
                                 nsAtom* aName, nsAtom* aStr,                  \
                                 bool aIgnoreCase) {                           \
    return AttrHasSubstring(aElement, aNS, aName, aStr, aIgnoreCase);          \
  }                                                                            \
  bool prefix_##AttrHasPrefix(implementor_ aElement, nsAtom* aNS,              \
                              nsAtom* aName, nsAtom* aStr, bool aIgnoreCase) { \
    return AttrHasPrefix(aElement, aNS, aName, aStr, aIgnoreCase);             \
  }                                                                            \
  bool prefix_##AttrHasSuffix(implementor_ aElement, nsAtom* aNS,              \
                              nsAtom* aName, nsAtom* aStr, bool aIgnoreCase) { \
    return AttrHasSuffix(aElement, aNS, aName, aStr, aIgnoreCase);             \
  }

SERVO_IMPL_ELEMENT_ATTR_MATCHING_FUNCTIONS(Gecko_, const Element*)
SERVO_IMPL_ELEMENT_ATTR_MATCHING_FUNCTIONS(Gecko_Snapshot,
                                           const ServoElementSnapshot*)
>>>>>>> upstream-releases

#undef SERVO_IMPL_ELEMENT_ATTR_MATCHING_FUNCTIONS

nsAtom* Gecko_Atomize(const char* aString, uint32_t aLength) {
  return NS_Atomize(nsDependentCSubstring(aString, aLength)).take();
}

nsAtom* Gecko_Atomize16(const nsAString* aString) {
  return NS_Atomize(*aString).take();
}

void Gecko_AddRefAtom(nsAtom* aAtom) { NS_ADDREF(aAtom); }

void Gecko_ReleaseAtom(nsAtom* aAtom) { NS_RELEASE(aAtom); }

<<<<<<< HEAD
void Gecko_nsTArray_FontFamilyName_AppendNamed(nsTArray<FontFamilyName>* aNames,
                                               nsAtom* aName, bool aQuoted) {
  aNames->AppendElement(
      FontFamilyName(aName, aQuoted ? eQuotedName : eUnquotedName));
||||||| merged common ancestors
void
Gecko_nsTArray_FontFamilyName_AppendNamed(nsTArray<FontFamilyName>* aNames,
                                          nsAtom* aName,
                                          bool aQuoted)
{
  aNames->AppendElement(FontFamilyName(aName, aQuoted ? eQuotedName : eUnquotedName));
=======
void Gecko_nsTArray_FontFamilyName_AppendNamed(
    nsTArray<FontFamilyName>* aNames, nsAtom* aName,
    StyleFontFamilyNameSyntax aSyntax) {
  aNames->AppendElement(FontFamilyName(aName, aSyntax));
>>>>>>> upstream-releases
}

<<<<<<< HEAD
void Gecko_nsTArray_FontFamilyName_AppendGeneric(
    nsTArray<FontFamilyName>* aNames, FontFamilyType aType) {
||||||| merged common ancestors
void
Gecko_nsTArray_FontFamilyName_AppendGeneric(nsTArray<FontFamilyName>* aNames,
                                            FontFamilyType aType)
{
=======
void Gecko_nsTArray_FontFamilyName_AppendGeneric(
    nsTArray<FontFamilyName>* aNames, StyleGenericFontFamily aType) {
>>>>>>> upstream-releases
  aNames->AppendElement(FontFamilyName(aType));
}

SharedFontList* Gecko_SharedFontList_Create() {
  RefPtr<SharedFontList> fontlist = new SharedFontList();
  return fontlist.forget().take();
}

MOZ_DEFINE_MALLOC_SIZE_OF(GeckoSharedFontListMallocSizeOf)

size_t Gecko_SharedFontList_SizeOfIncludingThisIfUnshared(
    SharedFontList* aFontlist) {
  MOZ_ASSERT(NS_IsMainThread());
  return aFontlist->SizeOfIncludingThisIfUnshared(
      GeckoSharedFontListMallocSizeOf);
}

size_t Gecko_SharedFontList_SizeOfIncludingThis(SharedFontList* aFontlist) {
  MOZ_ASSERT(NS_IsMainThread());
  return aFontlist->SizeOfIncludingThis(GeckoSharedFontListMallocSizeOf);
}

NS_IMPL_THREADSAFE_FFI_REFCOUNTING(SharedFontList, SharedFontList);

void Gecko_CopyFontFamilyFrom(nsFont* dst, const nsFont* src) {
  dst->fontlist = src->fontlist;
}

<<<<<<< HEAD
void Gecko_nsFont_InitSystem(nsFont* aDest, int32_t aFontId,
                             const nsStyleFont* aFont,
                             RawGeckoPresContextBorrowed aPresContext) {
  const nsFont* defaultVariableFont = ThreadSafeGetDefaultFontHelper(
      aPresContext, aFont->mLanguage, kPresContext_DefaultVariableFont_ID);
||||||| merged common ancestors
void
Gecko_nsFont_InitSystem(nsFont* aDest, int32_t aFontId,
                        const nsStyleFont* aFont, RawGeckoPresContextBorrowed aPresContext)
{
  const nsFont* defaultVariableFont = ThreadSafeGetDefaultFontHelper(aPresContext, aFont->mLanguage,
                                                                     kPresContext_DefaultVariableFont_ID);
=======
void Gecko_nsFont_InitSystem(nsFont* aDest, int32_t aFontId,
                             const nsStyleFont* aFont,
                             const Document* aDocument) {
  const nsFont* defaultVariableFont = ThreadSafeGetDefaultFontHelper(
      *aDocument, aFont->mLanguage, StyleGenericFontFamily::None);
>>>>>>> upstream-releases

  // We have passed uninitialized memory to this function,
  // initialize it. We can't simply return an nsFont because then
  // we need to know its size beforehand. Servo cannot initialize nsFont
  // itself, so this will do.
  new (aDest) nsFont(*defaultVariableFont);

  LookAndFeel::FontID fontID = static_cast<LookAndFeel::FontID>(aFontId);

  AutoWriteLock guard(*sServoFFILock);
  nsLayoutUtils::ComputeSystemFont(aDest, fontID, defaultVariableFont);
}

<<<<<<< HEAD
void Gecko_nsFont_Destroy(nsFont* aDest) { aDest->~nsFont(); }
||||||| merged common ancestors
void
Gecko_nsFont_Destroy(nsFont* aDest)
{
  aDest->~nsFont();
}
=======
void Gecko_nsFont_Destroy(nsFont* aDest) { aDest->~nsFont(); }

StyleGenericFontFamily Gecko_nsStyleFont_ComputeDefaultFontType(
    const Document* aDoc, StyleGenericFontFamily aGenericId,
    nsAtom* aLanguage) {
  const nsFont* defaultFont =
      ThreadSafeGetDefaultFontHelper(*aDoc, aLanguage, aGenericId);
  return defaultFont->fontlist.GetDefaultFontType();
}
>>>>>>> upstream-releases

gfxFontFeatureValueSet* Gecko_ConstructFontFeatureValueSet() {
  return new gfxFontFeatureValueSet();
}

nsTArray<unsigned int>* Gecko_AppendFeatureValueHashEntry(
    gfxFontFeatureValueSet* aFontFeatureValues, nsAtom* aFamily,
    uint32_t aAlternate, nsAtom* aName) {
  MOZ_ASSERT(NS_IsMainThread());
  static_assert(sizeof(unsigned int) == sizeof(uint32_t),
                "sizeof unsigned int and uint32_t must be the same");
  return aFontFeatureValues->AppendFeatureValueHashEntry(
<<<<<<< HEAD
      nsAtomCString(aFamily), nsDependentAtomString(aName), aAlternate);
}

void Gecko_nsFont_SetFontFeatureValuesLookup(
    nsFont* aFont, const RawGeckoPresContext* aPresContext) {
  aFont->featureValueLookup = aPresContext->GetFontFeatureValuesLookup();
||||||| merged common ancestors
    nsAtomCString(aFamily),
    nsDependentAtomString(aName),
    aAlternate
  );
}

void
Gecko_nsFont_SetFontFeatureValuesLookup(nsFont* aFont,
                                        const RawGeckoPresContext* aPresContext)
{
  aFont->featureValueLookup = aPresContext->GetFontFeatureValuesLookup();
=======
      nsAtomCString(aFamily), nsDependentAtomString(aName), aAlternate);
>>>>>>> upstream-releases
}

<<<<<<< HEAD
float Gecko_FontStretch_ToFloat(mozilla::FontStretch aStretch) {
||||||| merged common ancestors
float
Gecko_FontStretch_ToFloat(mozilla::FontStretch aStretch)
{
=======
float Gecko_FontStretch_ToFloat(FontStretch aStretch) {
>>>>>>> upstream-releases
  // Servo represents percentages with 1. being 100%.
  return aStretch.Percentage() / 100.0f;
}

<<<<<<< HEAD
void Gecko_FontStretch_SetFloat(mozilla::FontStretch* aStretch, float aFloat) {
||||||| merged common ancestors
void
Gecko_FontStretch_SetFloat(mozilla::FontStretch* aStretch, float aFloat)
{
=======
void Gecko_FontStretch_SetFloat(FontStretch* aStretch, float aFloat) {
>>>>>>> upstream-releases
  // Servo represents percentages with 1. being 100%.
  //
  // Also, the font code assumes a given maximum that style doesn't really need
  // to know about. So clamp here at the boundary.
  *aStretch = FontStretch(std::min(aFloat * 100.0f, float(FontStretch::kMax)));
}

<<<<<<< HEAD
void Gecko_FontSlantStyle_SetNormal(mozilla::FontSlantStyle* aStyle) {
  *aStyle = mozilla::FontSlantStyle::Normal();
||||||| merged common ancestors
void
Gecko_FontSlantStyle_SetNormal(mozilla::FontSlantStyle* aStyle)
{
  *aStyle = mozilla::FontSlantStyle::Normal();
=======
void Gecko_FontSlantStyle_SetNormal(FontSlantStyle* aStyle) {
  *aStyle = FontSlantStyle::Normal();
>>>>>>> upstream-releases
}

<<<<<<< HEAD
void Gecko_FontSlantStyle_SetItalic(mozilla::FontSlantStyle* aStyle) {
  *aStyle = mozilla::FontSlantStyle::Italic();
||||||| merged common ancestors
void
Gecko_FontSlantStyle_SetItalic(mozilla::FontSlantStyle* aStyle)
{
  *aStyle = mozilla::FontSlantStyle::Italic();
=======
void Gecko_FontSlantStyle_SetItalic(FontSlantStyle* aStyle) {
  *aStyle = FontSlantStyle::Italic();
>>>>>>> upstream-releases
}

<<<<<<< HEAD
void Gecko_FontSlantStyle_SetOblique(mozilla::FontSlantStyle* aStyle,
                                     float aAngleInDegrees) {
  *aStyle = mozilla::FontSlantStyle::Oblique(aAngleInDegrees);
||||||| merged common ancestors
void
Gecko_FontSlantStyle_SetOblique(mozilla::FontSlantStyle* aStyle,
                                float aAngleInDegrees)
{
  *aStyle = mozilla::FontSlantStyle::Oblique(aAngleInDegrees);
=======
void Gecko_FontSlantStyle_SetOblique(FontSlantStyle* aStyle,
                                     float aAngleInDegrees) {
  *aStyle = FontSlantStyle::Oblique(aAngleInDegrees);
>>>>>>> upstream-releases
}

<<<<<<< HEAD
void Gecko_FontSlantStyle_Get(mozilla::FontSlantStyle aStyle, bool* aNormal,
                              bool* aItalic, float* aObliqueAngle) {
||||||| merged common ancestors
void
Gecko_FontSlantStyle_Get(mozilla::FontSlantStyle aStyle,
                         bool* aNormal,
                         bool* aItalic,
                         float* aObliqueAngle)
{
=======
void Gecko_FontSlantStyle_Get(FontSlantStyle aStyle, bool* aNormal,
                              bool* aItalic, float* aObliqueAngle) {
>>>>>>> upstream-releases
  *aNormal = aStyle.IsNormal();
  *aItalic = aStyle.IsItalic();
  if (aStyle.IsOblique()) {
    *aObliqueAngle = aStyle.ObliqueAngle();
  }
}

<<<<<<< HEAD
float Gecko_FontWeight_ToFloat(mozilla::FontWeight aWeight) {
  return aWeight.ToFloat();
}

void Gecko_FontWeight_SetFloat(mozilla::FontWeight* aWeight, float aFloat) {
  *aWeight = mozilla::FontWeight(aFloat);
}
||||||| merged common ancestors
float
Gecko_FontWeight_ToFloat(mozilla::FontWeight aWeight)
{
  return aWeight.ToFloat();
}

void
Gecko_FontWeight_SetFloat(mozilla::FontWeight* aWeight,
                          float aFloat)
{
  *aWeight = mozilla::FontWeight(aFloat);
}
=======
float Gecko_FontWeight_ToFloat(FontWeight aWeight) { return aWeight.ToFloat(); }
>>>>>>> upstream-releases

<<<<<<< HEAD
void Gecko_nsFont_ResetFontFeatureValuesLookup(nsFont* aFont) {
  aFont->featureValueLookup = nullptr;
||||||| merged common ancestors
void
Gecko_nsFont_ResetFontFeatureValuesLookup(nsFont* aFont)
{
  aFont->featureValueLookup = nullptr;
=======
void Gecko_FontWeight_SetFloat(FontWeight* aWeight, float aFloat) {
  *aWeight = FontWeight(aFloat);
>>>>>>> upstream-releases
}

void Gecko_ClearAlternateValues(nsFont* aFont, size_t aLength) {
  aFont->alternateValues.Clear();
  aFont->alternateValues.SetCapacity(aLength);
}

void Gecko_AppendAlternateValues(nsFont* aFont, uint32_t aAlternateName,
                                 nsAtom* aAtom) {
  aFont->alternateValues.AppendElement(
      gfxAlternateValue{aAlternateName, nsDependentAtomString(aAtom)});
}

void Gecko_CopyAlternateValuesFrom(nsFont* aDest, const nsFont* aSrc) {
  aDest->alternateValues.Clear();
  aDest->alternateValues.AppendElements(aSrc->alternateValues);
}

<<<<<<< HEAD
void Gecko_SetCounterStyleToName(CounterStylePtr* aPtr, nsAtom* aName,
                                 RawGeckoPresContextBorrowed aPresContext) {
  // Try resolving the counter style if possible, and keep it unresolved
  // otherwise.
  CounterStyleManager* manager = aPresContext->CounterStyleManager();
||||||| merged common ancestors
void
Gecko_SetCounterStyleToName(CounterStylePtr* aPtr, nsAtom* aName,
                            RawGeckoPresContextBorrowed aPresContext)
{
  // Try resolving the counter style if possible, and keep it unresolved
  // otherwise.
  CounterStyleManager* manager = aPresContext->CounterStyleManager();
=======
void Gecko_SetCounterStyleToName(CounterStylePtr* aPtr, nsAtom* aName) {
>>>>>>> upstream-releases
  RefPtr<nsAtom> name = already_AddRefed<nsAtom>(aName);
  *aPtr = name.forget();
}

void Gecko_SetCounterStyleToSymbols(CounterStylePtr* aPtr, uint8_t aSymbolsType,
                                    nsACString const* const* aSymbols,
                                    uint32_t aSymbolsCount) {
  nsTArray<nsString> symbols(aSymbolsCount);
  for (uint32_t i = 0; i < aSymbolsCount; i++) {
    symbols.AppendElement(NS_ConvertUTF8toUTF16(*aSymbols[i]));
  }
  *aPtr = new AnonymousCounterStyle(aSymbolsType, std::move(symbols));
}

void Gecko_SetCounterStyleToString(CounterStylePtr* aPtr,
                                   const nsACString* aSymbol) {
  *aPtr = new AnonymousCounterStyle(NS_ConvertUTF8toUTF16(*aSymbol));
}

void Gecko_CopyCounterStyle(CounterStylePtr* aDst,
                            const CounterStylePtr* aSrc) {
  *aDst = *aSrc;
}

<<<<<<< HEAD
nsAtom* Gecko_CounterStyle_GetName(const CounterStylePtr* aPtr) {
  if (!aPtr->IsResolved()) {
    return aPtr->AsAtom();
  }
  return (*aPtr)->GetStyleName();
||||||| merged common ancestors
nsAtom*
Gecko_CounterStyle_GetName(const CounterStylePtr* aPtr)
{
  if (!aPtr->IsResolved()) {
    return aPtr->AsAtom();
  }
  return (*aPtr)->GetStyleName();
=======
nsAtom* Gecko_CounterStyle_GetName(const CounterStylePtr* aPtr) {
  return aPtr->IsAtom() ? aPtr->AsAtom() : nullptr;
>>>>>>> upstream-releases
}

const AnonymousCounterStyle* Gecko_CounterStyle_GetAnonymous(
    const CounterStylePtr* aPtr) {
  return aPtr->AsAnonymous();
}

void Gecko_SetNullImageValue(nsStyleImage* aImage) {
  MOZ_ASSERT(aImage);
  aImage->SetNull();
}

<<<<<<< HEAD
void Gecko_SetGradientImageValue(nsStyleImage* aImage,
                                 nsStyleGradient* aGradient) {
||||||| merged common ancestors
void
Gecko_SetGradientImageValue(nsStyleImage* aImage, nsStyleGradient* aGradient)
{
=======
void Gecko_SetGradientImageValue(nsStyleImage* aImage,
                                 StyleGradient* aGradient) {
>>>>>>> upstream-releases
  MOZ_ASSERT(aImage);
  aImage->SetGradientData(UniquePtr<StyleGradient>(aGradient));
}

<<<<<<< HEAD
static already_AddRefed<nsStyleImageRequest> CreateStyleImageRequest(
    nsStyleImageRequest::Mode aModeFlags, URLValue* aImageValue) {
  RefPtr<nsStyleImageRequest> req =
      new nsStyleImageRequest(aModeFlags, aImageValue);
||||||| merged common ancestors
static already_AddRefed<nsStyleImageRequest>
CreateStyleImageRequest(nsStyleImageRequest::Mode aModeFlags,
                        URLValue* aImageValue)
{
  RefPtr<nsStyleImageRequest> req =
    new nsStyleImageRequest(aModeFlags, aImageValue);
=======
static already_AddRefed<nsStyleImageRequest> CreateStyleImageRequest(
    nsStyleImageRequest::Mode aModeFlags, const StyleComputedImageUrl* aUrl) {
  RefPtr<nsStyleImageRequest> req = new nsStyleImageRequest(aModeFlags, *aUrl);
>>>>>>> upstream-releases
  return req.forget();
}

<<<<<<< HEAD
void Gecko_SetLayerImageImageValue(nsStyleImage* aImage,
                                   URLValue* aImageValue) {
  MOZ_ASSERT(aImage && aImageValue);
||||||| merged common ancestors
void
Gecko_SetLayerImageImageValue(nsStyleImage* aImage,
                              URLValue* aImageValue)
{
  MOZ_ASSERT(aImage && aImageValue);
=======
void Gecko_SetLayerImageImageValue(nsStyleImage* aImage,
                                   const StyleComputedImageUrl* aUrl) {
  MOZ_ASSERT(aImage && aUrl);
>>>>>>> upstream-releases

  RefPtr<nsStyleImageRequest> req =
<<<<<<< HEAD
      CreateStyleImageRequest(nsStyleImageRequest::Mode::Track, aImageValue);
||||||| merged common ancestors
    CreateStyleImageRequest(nsStyleImageRequest::Mode::Track, aImageValue);
=======
      CreateStyleImageRequest(nsStyleImageRequest::Mode::Track, aUrl);
>>>>>>> upstream-releases
  aImage->SetImageRequest(req.forget());
}

void Gecko_SetImageElement(nsStyleImage* aImage, nsAtom* aAtom) {
  MOZ_ASSERT(aImage);
  aImage->SetElementId(do_AddRef(aAtom));
}

void Gecko_CopyImageValueFrom(nsStyleImage* aImage,
                              const nsStyleImage* aOther) {
  MOZ_ASSERT(aImage);
  MOZ_ASSERT(aOther);

  *aImage = *aOther;
}

void Gecko_InitializeImageCropRect(nsStyleImage* aImage) {
  MOZ_ASSERT(aImage);
  aImage->SetCropRect(MakeUnique<nsStyleImage::CropRect>());
}

void Gecko_SetCursorArrayLength(nsStyleUI* aStyleUI, size_t aLen) {
  aStyleUI->mCursorImages.Clear();
  aStyleUI->mCursorImages.SetLength(aLen);
}

<<<<<<< HEAD
void Gecko_SetCursorImageValue(nsCursorImage* aCursor, URLValue* aImageValue) {
  MOZ_ASSERT(aCursor && aImageValue);
||||||| merged common ancestors
void
Gecko_SetCursorImageValue(nsCursorImage* aCursor,
                          URLValue* aImageValue)
{
  MOZ_ASSERT(aCursor && aImageValue);
=======
void Gecko_SetCursorImageValue(nsCursorImage* aCursor,
                               const StyleComputedImageUrl* aUrl) {
  MOZ_ASSERT(aCursor && aUrl);
>>>>>>> upstream-releases

  aCursor->mImage =
<<<<<<< HEAD
      CreateStyleImageRequest(nsStyleImageRequest::Mode::Discard, aImageValue);
||||||| merged common ancestors
    CreateStyleImageRequest(nsStyleImageRequest::Mode::Discard, aImageValue);
=======
      CreateStyleImageRequest(nsStyleImageRequest::Mode::Discard, aUrl);
>>>>>>> upstream-releases
}

void Gecko_CopyCursorArrayFrom(nsStyleUI* aDest, const nsStyleUI* aSrc) {
  aDest->mCursorImages = aSrc->mCursorImages;
}

<<<<<<< HEAD
void Gecko_SetContentDataImageValue(nsStyleContentData* aContent,
                                    URLValue* aImageValue) {
  MOZ_ASSERT(aContent && aImageValue);
||||||| merged common ancestors
void
Gecko_SetContentDataImageValue(nsStyleContentData* aContent,
                               URLValue* aImageValue)
{
  MOZ_ASSERT(aContent && aImageValue);
=======
void Gecko_SetContentDataImageValue(nsStyleContentData* aContent,
                                    const StyleComputedImageUrl* aUrl) {
  MOZ_ASSERT(aContent && aUrl);
>>>>>>> upstream-releases

  RefPtr<nsStyleImageRequest> req =
<<<<<<< HEAD
      CreateStyleImageRequest(nsStyleImageRequest::Mode::Track, aImageValue);
||||||| merged common ancestors
    CreateStyleImageRequest(nsStyleImageRequest::Mode::Track, aImageValue);
=======
      CreateStyleImageRequest(nsStyleImageRequest::Mode::Track, aUrl);
>>>>>>> upstream-releases
  aContent->SetImageRequest(req.forget());
}

nsStyleContentData::CounterFunction* Gecko_SetCounterFunction(
    nsStyleContentData* aContent, StyleContentType aType) {
  auto counterFunc = MakeRefPtr<nsStyleContentData::CounterFunction>();
  auto* ptr = counterFunc.get();
  aContent->SetCounters(aType, counterFunc.forget());
  return ptr;
}

<<<<<<< HEAD
nsStyleGradient* Gecko_CreateGradient(uint8_t aShape, uint8_t aSize,
                                      bool aRepeating, bool aLegacySyntax,
                                      bool aMozLegacySyntax,
                                      uint32_t aStopCount) {
  nsStyleGradient* result = new nsStyleGradient();

  result->mShape = aShape;
  result->mSize = aSize;
  result->mRepeating = aRepeating;
  result->mLegacySyntax = aLegacySyntax;
  result->mMozLegacySyntax = aMozLegacySyntax;

  result->mAngle.SetNoneValue();
  result->mBgPosX.SetNoneValue();
  result->mBgPosY.SetNoneValue();
  result->mRadiusX.SetNoneValue();
  result->mRadiusY.SetNoneValue();

  nsStyleGradientStop dummyStop = {nsStyleCoord(eStyleUnit_None),
                                   StyleComplexColor::Black(), 0};

  for (uint32_t i = 0; i < aStopCount; i++) {
    result->mStops.AppendElement(dummyStop);
  }

  return result;
}

const nsStyleImageRequest* Gecko_GetImageRequest(const nsStyleImage* aImage) {
||||||| merged common ancestors
nsStyleGradient*
Gecko_CreateGradient(uint8_t aShape,
                     uint8_t aSize,
                     bool aRepeating,
                     bool aLegacySyntax,
                     bool aMozLegacySyntax,
                     uint32_t aStopCount)
{
  nsStyleGradient* result = new nsStyleGradient();

  result->mShape = aShape;
  result->mSize = aSize;
  result->mRepeating = aRepeating;
  result->mLegacySyntax = aLegacySyntax;
  result->mMozLegacySyntax = aMozLegacySyntax;

  result->mAngle.SetNoneValue();
  result->mBgPosX.SetNoneValue();
  result->mBgPosY.SetNoneValue();
  result->mRadiusX.SetNoneValue();
  result->mRadiusY.SetNoneValue();

  nsStyleGradientStop dummyStop = {
    nsStyleCoord(eStyleUnit_None),
    StyleComplexColor::Black(),
    0
  };

  for (uint32_t i = 0; i < aStopCount; i++) {
    result->mStops.AppendElement(dummyStop);
  }

  return result;
}

const nsStyleImageRequest*
Gecko_GetImageRequest(const nsStyleImage* aImage)
{
=======
const nsStyleImageRequest* Gecko_GetImageRequest(const nsStyleImage* aImage) {
>>>>>>> upstream-releases
  MOZ_ASSERT(aImage);
  return aImage->ImageRequest();
}

nsAtom* Gecko_GetImageElement(const nsStyleImage* aImage) {
  MOZ_ASSERT(aImage && aImage->GetType() == eStyleImageType_Element);
  return const_cast<nsAtom*>(aImage->GetElementId());
}

<<<<<<< HEAD
const nsStyleGradient* Gecko_GetGradientImageValue(const nsStyleImage* aImage) {
  MOZ_ASSERT(aImage && aImage->GetType() == eStyleImageType_Gradient);
  return aImage->GetGradientData();
}

void Gecko_SetListStyleImageNone(nsStyleList* aList) {
||||||| merged common ancestors
const nsStyleGradient*
Gecko_GetGradientImageValue(const nsStyleImage* aImage)
{
  MOZ_ASSERT(aImage && aImage->GetType() == eStyleImageType_Gradient);
  return aImage->GetGradientData();
}

void
Gecko_SetListStyleImageNone(nsStyleList* aList)
{
=======
void Gecko_SetListStyleImageNone(nsStyleList* aList) {
>>>>>>> upstream-releases
  aList->mListStyleImage = nullptr;
}

<<<<<<< HEAD
void Gecko_SetListStyleImageImageValue(nsStyleList* aList,
                                       URLValue* aImageValue) {
  MOZ_ASSERT(aList && aImageValue);
||||||| merged common ancestors
void
Gecko_SetListStyleImageImageValue(nsStyleList* aList,
                                  URLValue* aImageValue)
{
  MOZ_ASSERT(aList && aImageValue);
=======
void Gecko_SetListStyleImageImageValue(nsStyleList* aList,
                                       const StyleComputedImageUrl* aUrl) {
  MOZ_ASSERT(aList && aUrl);
>>>>>>> upstream-releases

  aList->mListStyleImage =
<<<<<<< HEAD
      CreateStyleImageRequest(nsStyleImageRequest::Mode(0), aImageValue);
||||||| merged common ancestors
    CreateStyleImageRequest(nsStyleImageRequest::Mode(0), aImageValue);
=======
      CreateStyleImageRequest(nsStyleImageRequest::Mode(0), aUrl);
>>>>>>> upstream-releases
}

void Gecko_CopyListStyleImageFrom(nsStyleList* aList,
                                  const nsStyleList* aSource) {
  aList->mListStyleImage = aSource->mListStyleImage;
}

void Gecko_EnsureTArrayCapacity(void* aArray, size_t aCapacity,
                                size_t aElemSize) {
  auto base = reinterpret_cast<
      nsTArray_base<nsTArrayInfallibleAllocator, nsTArray_CopyWithMemutils>*>(
      aArray);

  base->EnsureCapacity<nsTArrayInfallibleAllocator>(aCapacity, aElemSize);
}

void Gecko_ClearPODTArray(void* aArray, size_t aElementSize,
                          size_t aElementAlign) {
  auto base = reinterpret_cast<
      nsTArray_base<nsTArrayInfallibleAllocator, nsTArray_CopyWithMemutils>*>(
      aArray);

  base->template ShiftData<nsTArrayInfallibleAllocator>(
      0, base->Length(), 0, aElementSize, aElementAlign);
}

void Gecko_ResizeTArrayForStrings(nsTArray<nsString>* aArray,
                                  uint32_t aLength) {
  aArray->SetLength(aLength);
}

<<<<<<< HEAD
void Gecko_SetStyleGridTemplate(UniquePtr<nsStyleGridTemplate>* aGridTemplate,
                                nsStyleGridTemplate* aValue) {
||||||| merged common ancestors
void
Gecko_SetStyleGridTemplate(UniquePtr<nsStyleGridTemplate>* aGridTemplate,
                           nsStyleGridTemplate* aValue)
{
=======
void Gecko_ResizeAtomArray(nsTArray<RefPtr<nsAtom>>* aArray, uint32_t aLength) {
  aArray->SetLength(aLength);
}

void Gecko_SetStyleGridTemplate(UniquePtr<nsStyleGridTemplate>* aGridTemplate,
                                nsStyleGridTemplate* aValue) {
>>>>>>> upstream-releases
  aGridTemplate->reset(aValue);
}

nsStyleGridTemplate* Gecko_CreateStyleGridTemplate(uint32_t aTrackSizes,
                                                   uint32_t aNameSize) {
  nsStyleGridTemplate* result = new nsStyleGridTemplate;
  result->mTrackSizingFunctions.SetCapacity(aTrackSizes);
  auto auto_ = StyleTrackSize::Breadth(StyleTrackBreadth::Auto());
  for (auto i : IntegerRange(aTrackSizes)) {
    Unused << i;
    result->mTrackSizingFunctions.AppendElement(auto_);
  }
  result->mLineNameLists.SetLength(aNameSize);
  return result;
}

void Gecko_CopyStyleGridTemplateValues(
    UniquePtr<nsStyleGridTemplate>* aGridTemplate,
    const nsStyleGridTemplate* aOther) {
  if (aOther) {
    *aGridTemplate = MakeUnique<nsStyleGridTemplate>(*aOther);
  } else {
    *aGridTemplate = nullptr;
  }
}

<<<<<<< HEAD
GridTemplateAreasValue* Gecko_NewGridTemplateAreasValue(uint32_t aAreas,
                                                        uint32_t aTemplates,
                                                        uint32_t aColumns) {
  RefPtr<GridTemplateAreasValue> value = new GridTemplateAreasValue;
  value->mNamedAreas.SetLength(aAreas);
  value->mTemplates.SetLength(aTemplates);
  value->mNColumns = aColumns;
  return value.forget().take();
}

NS_IMPL_THREADSAFE_FFI_REFCOUNTING(GridTemplateAreasValue,
                                   GridTemplateAreasValue);

void Gecko_ClearAndResizeStyleContents(nsStyleContent* aContent,
                                       uint32_t aHowMany) {
||||||| merged common ancestors
GridTemplateAreasValue*
Gecko_NewGridTemplateAreasValue(uint32_t aAreas, uint32_t aTemplates, uint32_t aColumns)
{
  RefPtr<GridTemplateAreasValue> value = new GridTemplateAreasValue;
  value->mNamedAreas.SetLength(aAreas);
  value->mTemplates.SetLength(aTemplates);
  value->mNColumns = aColumns;
  return value.forget().take();
}

NS_IMPL_THREADSAFE_FFI_REFCOUNTING(GridTemplateAreasValue, GridTemplateAreasValue);

void
Gecko_ClearAndResizeStyleContents(nsStyleContent* aContent, uint32_t aHowMany)
{
=======
void Gecko_ClearAndResizeStyleContents(nsStyleContent* aContent,
                                       uint32_t aHowMany) {
>>>>>>> upstream-releases
  aContent->AllocateContents(aHowMany);
}

void Gecko_CopyStyleContentsFrom(nsStyleContent* aContent,
                                 const nsStyleContent* aOther) {
  uint32_t count = aOther->ContentCount();

  aContent->AllocateContents(count);

  for (uint32_t i = 0; i < count; ++i) {
    aContent->ContentAt(i) = aOther->ContentAt(i);
  }
}

void Gecko_ClearAndResizeCounterIncrements(nsStyleContent* aContent,
                                           uint32_t aHowMany) {
  aContent->AllocateCounterIncrements(aHowMany);
}

void Gecko_CopyCounterIncrementsFrom(nsStyleContent* aContent,
                                     const nsStyleContent* aOther) {
  uint32_t count = aOther->CounterIncrementCount();

  aContent->AllocateCounterIncrements(count);

  for (uint32_t i = 0; i < count; ++i) {
    const nsStyleCounterData& data = aOther->CounterIncrementAt(i);
    aContent->SetCounterIncrementAt(i, data.mCounter, data.mValue);
  }
}

void Gecko_ClearAndResizeCounterResets(nsStyleContent* aContent,
                                       uint32_t aHowMany) {
  aContent->AllocateCounterResets(aHowMany);
}

void Gecko_CopyCounterResetsFrom(nsStyleContent* aContent,
                                 const nsStyleContent* aOther) {
  uint32_t count = aOther->CounterResetCount();

  aContent->AllocateCounterResets(count);

  for (uint32_t i = 0; i < count; ++i) {
    const nsStyleCounterData& data = aOther->CounterResetAt(i);
    aContent->SetCounterResetAt(i, data.mCounter, data.mValue);
  }
}

<<<<<<< HEAD
void Gecko_EnsureImageLayersLength(nsStyleImageLayers* aLayers, size_t aLen,
                                   nsStyleImageLayers::LayerType aLayerType) {
||||||| merged common ancestors
void
Gecko_EnsureImageLayersLength(nsStyleImageLayers* aLayers, size_t aLen,
                              nsStyleImageLayers::LayerType aLayerType)
{
=======
void Gecko_ClearAndResizeCounterSets(nsStyleContent* aContent,
                                     uint32_t aHowMany) {
  aContent->AllocateCounterSets(aHowMany);
}

void Gecko_CopyCounterSetsFrom(nsStyleContent* aContent,
                               const nsStyleContent* aOther) {
  uint32_t count = aOther->CounterSetCount();

  aContent->AllocateCounterSets(count);

  for (uint32_t i = 0; i < count; ++i) {
    const nsStyleCounterData& data = aOther->CounterSetAt(i);
    aContent->SetCounterSetAt(i, data.mCounter, data.mValue);
  }
}

void Gecko_EnsureImageLayersLength(nsStyleImageLayers* aLayers, size_t aLen,
                                   nsStyleImageLayers::LayerType aLayerType) {
>>>>>>> upstream-releases
  size_t oldLength = aLayers->mLayers.Length();

  aLayers->mLayers.EnsureLengthAtLeast(aLen);

  for (size_t i = oldLength; i < aLen; ++i) {
    aLayers->mLayers[i].Initialize(aLayerType);
  }
}

template <typename StyleType>
static void EnsureStyleAutoArrayLength(StyleType* aArray, size_t aLen) {
  size_t oldLength = aArray->Length();

  aArray->EnsureLengthAtLeast(aLen);

  for (size_t i = oldLength; i < aLen; ++i) {
    (*aArray)[i].SetInitialValues();
  }
}

void Gecko_EnsureStyleAnimationArrayLength(void* aArray, size_t aLen) {
  auto base = static_cast<nsStyleAutoArray<StyleAnimation>*>(aArray);
  EnsureStyleAutoArrayLength(base, aLen);
}

void Gecko_EnsureStyleTransitionArrayLength(void* aArray, size_t aLen) {
  auto base = reinterpret_cast<nsStyleAutoArray<StyleTransition>*>(aArray);
  EnsureStyleAutoArrayLength(base, aLen);
}

<<<<<<< HEAD
void Gecko_ClearWillChange(nsStyleDisplay* aDisplay, size_t aLength) {
  aDisplay->mWillChange.Clear();
  aDisplay->mWillChange.SetCapacity(aLength);
}

void Gecko_AppendWillChange(nsStyleDisplay* aDisplay, nsAtom* aAtom) {
  aDisplay->mWillChange.AppendElement(aAtom);
}

void Gecko_CopyWillChangeFrom(nsStyleDisplay* aDest, nsStyleDisplay* aSrc) {
  aDest->mWillChange.Clear();
  aDest->mWillChange.AppendElements(aSrc->mWillChange);
}

||||||| merged common ancestors
void
Gecko_ClearWillChange(nsStyleDisplay* aDisplay, size_t aLength)
{
  aDisplay->mWillChange.Clear();
  aDisplay->mWillChange.SetCapacity(aLength);
}

void
Gecko_AppendWillChange(nsStyleDisplay* aDisplay, nsAtom* aAtom)
{
  aDisplay->mWillChange.AppendElement(aAtom);
}

void
Gecko_CopyWillChangeFrom(nsStyleDisplay* aDest, nsStyleDisplay* aSrc)
{
  aDest->mWillChange.Clear();
  aDest->mWillChange.AppendElements(aSrc->mWillChange);
}

=======
>>>>>>> upstream-releases
enum class KeyframeSearchDirection {
  Forwards,
  Backwards,
};

enum class KeyframeInsertPosition {
  Prepend,
  LastForOffset,
};

static Keyframe* GetOrCreateKeyframe(nsTArray<Keyframe>* aKeyframes,
                                     float aOffset,
                                     const nsTimingFunction* aTimingFunction,
                                     KeyframeSearchDirection aSearchDirection,
                                     KeyframeInsertPosition aInsertPosition) {
  MOZ_ASSERT(aKeyframes, "The keyframe array should be valid");
  MOZ_ASSERT(aTimingFunction, "The timing function should be valid");
  MOZ_ASSERT(aOffset >= 0. && aOffset <= 1.,
             "The offset should be in the range of [0.0, 1.0]");

  size_t keyframeIndex;
  switch (aSearchDirection) {
    case KeyframeSearchDirection::Forwards:
      if (nsAnimationManager::FindMatchingKeyframe(
              *aKeyframes, aOffset, *aTimingFunction, keyframeIndex)) {
        return &(*aKeyframes)[keyframeIndex];
      }
      break;
    case KeyframeSearchDirection::Backwards:
      if (nsAnimationManager::FindMatchingKeyframe(Reversed(*aKeyframes),
                                                   aOffset, *aTimingFunction,
                                                   keyframeIndex)) {
        return &(*aKeyframes)[aKeyframes->Length() - 1 - keyframeIndex];
      }
      keyframeIndex = aKeyframes->Length() - 1;
      break;
  }

  Keyframe* keyframe = aKeyframes->InsertElementAt(
      aInsertPosition == KeyframeInsertPosition::Prepend ? 0 : keyframeIndex);
  keyframe->mOffset.emplace(aOffset);
  if (!aTimingFunction->IsLinear()) {
    keyframe->mTimingFunction.emplace();
    keyframe->mTimingFunction->Init(*aTimingFunction);
  }

  return keyframe;
}

Keyframe* Gecko_GetOrCreateKeyframeAtStart(
    nsTArray<Keyframe>* aKeyframes, float aOffset,
    const nsTimingFunction* aTimingFunction) {
  MOZ_ASSERT(aKeyframes->IsEmpty() ||
                 aKeyframes->ElementAt(0).mOffset.value() >= aOffset,
             "The offset should be less than or equal to the first keyframe's "
             "offset if there are exisiting keyframes");

  return GetOrCreateKeyframe(aKeyframes, aOffset, aTimingFunction,
                             KeyframeSearchDirection::Forwards,
                             KeyframeInsertPosition::Prepend);
}

Keyframe* Gecko_GetOrCreateInitialKeyframe(
    nsTArray<Keyframe>* aKeyframes, const nsTimingFunction* aTimingFunction) {
  return GetOrCreateKeyframe(aKeyframes, 0., aTimingFunction,
                             KeyframeSearchDirection::Forwards,
                             KeyframeInsertPosition::LastForOffset);
}

Keyframe* Gecko_GetOrCreateFinalKeyframe(
    nsTArray<Keyframe>* aKeyframes, const nsTimingFunction* aTimingFunction) {
  return GetOrCreateKeyframe(aKeyframes, 1., aTimingFunction,
                             KeyframeSearchDirection::Backwards,
                             KeyframeInsertPosition::LastForOffset);
}

PropertyValuePair* Gecko_AppendPropertyValuePair(
    nsTArray<PropertyValuePair>* aProperties, nsCSSPropertyID aProperty) {
  MOZ_ASSERT(aProperties);
  MOZ_ASSERT(aProperty == eCSSPropertyExtra_variable ||
             !nsCSSProps::PropHasFlags(aProperty, CSSPropFlags::IsLogical));
<<<<<<< HEAD
  return aProperties->AppendElement(PropertyValuePair{aProperty});
}

void Gecko_ResetStyleCoord(nsStyleUnit* aUnit, nsStyleUnion* aValue) {
  nsStyleCoord::Reset(*aUnit, *aValue);
||||||| merged common ancestors
  return aProperties->AppendElement(PropertyValuePair { aProperty });
}

void
Gecko_ResetStyleCoord(nsStyleUnit* aUnit, nsStyleUnion* aValue)
{
  nsStyleCoord::Reset(*aUnit, *aValue);
=======
  return aProperties->AppendElement(PropertyValuePair{aProperty});
>>>>>>> upstream-releases
}

<<<<<<< HEAD
void Gecko_SetStyleCoordCalcValue(nsStyleUnit* aUnit, nsStyleUnion* aValue,
                                  nsStyleCoord::CalcValue aCalc) {
  // Calc units should be cleaned up first
  MOZ_ASSERT(*aUnit != nsStyleUnit::eStyleUnit_Calc);
  nsStyleCoord::Calc* calcRef = new nsStyleCoord::Calc();
  calcRef->mLength = aCalc.mLength;
  calcRef->mPercent = aCalc.mPercent;
  calcRef->mHasPercent = aCalc.mHasPercent;
  *aUnit = nsStyleUnit::eStyleUnit_Calc;
  aValue->mPointer = calcRef;
  calcRef->AddRef();
}

void Gecko_CopyShapeSourceFrom(mozilla::StyleShapeSource* aDst,
                               const mozilla::StyleShapeSource* aSrc) {
||||||| merged common ancestors
void
Gecko_SetStyleCoordCalcValue(nsStyleUnit* aUnit, nsStyleUnion* aValue, nsStyleCoord::CalcValue aCalc)
{
  // Calc units should be cleaned up first
  MOZ_ASSERT(*aUnit != nsStyleUnit::eStyleUnit_Calc);
  nsStyleCoord::Calc* calcRef = new nsStyleCoord::Calc();
  calcRef->mLength = aCalc.mLength;
  calcRef->mPercent = aCalc.mPercent;
  calcRef->mHasPercent = aCalc.mHasPercent;
  *aUnit = nsStyleUnit::eStyleUnit_Calc;
  aValue->mPointer = calcRef;
  calcRef->AddRef();
}

void
Gecko_CopyShapeSourceFrom(mozilla::StyleShapeSource* aDst, const mozilla::StyleShapeSource* aSrc)
{
=======
void Gecko_CopyShapeSourceFrom(StyleShapeSource* aDst,
                               const StyleShapeSource* aSrc) {
>>>>>>> upstream-releases
  MOZ_ASSERT(aDst);
  MOZ_ASSERT(aSrc);

  *aDst = *aSrc;
}

<<<<<<< HEAD
void Gecko_DestroyShapeSource(mozilla::StyleShapeSource* aShape) {
||||||| merged common ancestors
void
Gecko_DestroyShapeSource(mozilla::StyleShapeSource* aShape)
{
=======
void Gecko_DestroyShapeSource(StyleShapeSource* aShape) {
>>>>>>> upstream-releases
  aShape->~StyleShapeSource();
}

<<<<<<< HEAD
void Gecko_StyleShapeSource_SetURLValue(StyleShapeSource* aShape,
                                        URLValue* aURL) {
  aShape->SetURL(*aURL);
}

void Gecko_NewBasicShape(mozilla::StyleShapeSource* aShape,
                         mozilla::StyleBasicShapeType aType) {
  aShape->SetBasicShape(MakeUnique<mozilla::StyleBasicShape>(aType),
                        StyleGeometryBox::NoBox);
}

void Gecko_NewShapeImage(mozilla::StyleShapeSource* aShape) {
||||||| merged common ancestors
void
Gecko_StyleShapeSource_SetURLValue(StyleShapeSource* aShape, URLValue* aURL)
{
  aShape->SetURL(aURL);
}

void
Gecko_NewBasicShape(mozilla::StyleShapeSource* aShape,
                    mozilla::StyleBasicShapeType aType)
{
  aShape->SetBasicShape(MakeUnique<mozilla::StyleBasicShape>(aType),
                        StyleGeometryBox::NoBox);
}

void
Gecko_NewShapeImage(mozilla::StyleShapeSource* aShape)
{
=======
void Gecko_NewShapeImage(StyleShapeSource* aShape) {
>>>>>>> upstream-releases
  aShape->SetShapeImage(MakeUnique<nsStyleImage>());
}

<<<<<<< HEAD
void Gecko_NewStyleSVGPath(mozilla::StyleShapeSource* aShape) {
||||||| merged common ancestors
void
Gecko_NewStyleSVGPath(mozilla::StyleShapeSource* aShape)
{
=======
void Gecko_SetToSVGPath(StyleShapeSource* aShape,
                        StyleForgottenArcSlicePtr<StylePathCommand> aCommands,
                        StyleFillRule aFill) {
>>>>>>> upstream-releases
  MOZ_ASSERT(aShape);
<<<<<<< HEAD
  aShape->SetPath(MakeUnique<mozilla::StyleSVGPath>());
}

void Gecko_SetStyleMotion(UniquePtr<mozilla::StyleMotion>* aMotion,
                          mozilla::StyleMotion* aValue) {
  MOZ_ASSERT(aMotion);
  aMotion->reset(aValue);
}

mozilla::StyleMotion* Gecko_NewStyleMotion() { return new StyleMotion(); }
||||||| merged common ancestors
  aShape->SetPath(MakeUnique<mozilla::StyleSVGPath>());
}

void
Gecko_SetStyleMotion(UniquePtr<mozilla::StyleMotion>* aMotion,
                     mozilla::StyleMotion* aValue)
{
  MOZ_ASSERT(aMotion);
  aMotion->reset(aValue);
}

mozilla::StyleMotion*
Gecko_NewStyleMotion()
{
  return new StyleMotion();
}
=======
  aShape->SetPath(MakeUnique<StyleSVGPath>(aCommands, aFill));
}
>>>>>>> upstream-releases

<<<<<<< HEAD
void Gecko_CopyStyleMotions(mozilla::UniquePtr<mozilla::StyleMotion>* aMotion,
                            const mozilla::StyleMotion* aOther) {
  MOZ_ASSERT(aMotion);
  *aMotion = aOther ? MakeUnique<StyleMotion>(*aOther) : nullptr;
}

void Gecko_ResetFilters(nsStyleEffects* effects, size_t new_len) {
  effects->mFilters.Clear();
  effects->mFilters.SetLength(new_len);
}

void Gecko_CopyFiltersFrom(nsStyleEffects* aSrc, nsStyleEffects* aDest) {
  aDest->mFilters = aSrc->mFilters;
}

void Gecko_nsStyleFilter_SetURLValue(nsStyleFilter* aEffects, URLValue* aURL) {
  aEffects->SetURL(aURL);
}

void Gecko_nsStyleSVGPaint_CopyFrom(nsStyleSVGPaint* aDest,
                                    const nsStyleSVGPaint* aSrc) {
  *aDest = *aSrc;
}

void Gecko_nsStyleSVGPaint_SetURLValue(nsStyleSVGPaint* aPaint,
                                       URLValue* aURL) {
  aPaint->SetPaintServer(aURL);
}

void Gecko_nsStyleSVGPaint_Reset(nsStyleSVGPaint* aPaint) { aPaint->SetNone(); }

void Gecko_nsStyleSVG_SetDashArrayLength(nsStyleSVG* aSvg, uint32_t aLen) {
||||||| merged common ancestors
void
Gecko_CopyStyleMotions(mozilla::UniquePtr<mozilla::StyleMotion>* aMotion,
                       const mozilla::StyleMotion* aOther)
{
  MOZ_ASSERT(aMotion);
  *aMotion = aOther ? MakeUnique<StyleMotion>(*aOther) : nullptr;
}

void
Gecko_ResetFilters(nsStyleEffects* effects, size_t new_len)
{
  effects->mFilters.Clear();
  effects->mFilters.SetLength(new_len);
}

void
Gecko_CopyFiltersFrom(nsStyleEffects* aSrc, nsStyleEffects* aDest)
{
  aDest->mFilters = aSrc->mFilters;
}

void
Gecko_nsStyleFilter_SetURLValue(nsStyleFilter* aEffects, URLValue* aURL)
{
  aEffects->SetURL(aURL);
}

void
Gecko_nsStyleSVGPaint_CopyFrom(nsStyleSVGPaint* aDest, const nsStyleSVGPaint* aSrc)
{
  *aDest = *aSrc;
}

void
Gecko_nsStyleSVGPaint_SetURLValue(nsStyleSVGPaint* aPaint, URLValue* aURL)
{
  aPaint->SetPaintServer(aURL);
}

void Gecko_nsStyleSVGPaint_Reset(nsStyleSVGPaint* aPaint)
{
  aPaint->SetNone();
}

void
Gecko_nsStyleSVG_SetDashArrayLength(nsStyleSVG* aSvg, uint32_t aLen)
{
=======
void Gecko_nsStyleSVG_SetDashArrayLength(nsStyleSVG* aSvg, uint32_t aLen) {
>>>>>>> upstream-releases
  aSvg->mStrokeDasharray.Clear();
  aSvg->mStrokeDasharray.SetLength(aLen);
}

void Gecko_nsStyleSVG_CopyDashArray(nsStyleSVG* aDst, const nsStyleSVG* aSrc) {
  aDst->mStrokeDasharray = aSrc->mStrokeDasharray;
}

<<<<<<< HEAD
void Gecko_nsStyleSVG_SetContextPropertiesLength(nsStyleSVG* aSvg,
                                                 uint32_t aLen) {
  aSvg->mContextProps.Clear();
  aSvg->mContextProps.SetLength(aLen);
}

void Gecko_nsStyleSVG_CopyContextProperties(nsStyleSVG* aDst,
                                            const nsStyleSVG* aSrc) {
  aDst->mContextProps = aSrc->mContextProps;
  aDst->mContextPropsBits = aSrc->mContextPropsBits;
}

css::URLValue* Gecko_URLValue_Create(RawServoCssUrlDataStrong aCssUrl,
                                     CORSMode aCORSMode) {
  RefPtr<css::URLValue> url = new css::URLValue(aCssUrl.Consume(), aCORSMode);
  return url.forget().take();
}

MOZ_DEFINE_MALLOC_SIZE_OF(GeckoURLValueMallocSizeOf)

size_t Gecko_URLValue_SizeOfIncludingThis(URLValue* aURL) {
  MOZ_ASSERT(NS_IsMainThread());
  return aURL->SizeOfIncludingThis(GeckoURLValueMallocSizeOf);
}

void Gecko_GetComputedURLSpec(const URLValue* aURL, nsCString* aOut) {
||||||| merged common ancestors
void
Gecko_nsStyleSVG_SetContextPropertiesLength(nsStyleSVG* aSvg, uint32_t aLen)
{
  aSvg->mContextProps.Clear();
  aSvg->mContextProps.SetLength(aLen);
}

void
Gecko_nsStyleSVG_CopyContextProperties(nsStyleSVG* aDst, const nsStyleSVG* aSrc)
{
  aDst->mContextProps = aSrc->mContextProps;
  aDst->mContextPropsBits = aSrc->mContextPropsBits;
}


css::URLValue*
Gecko_URLValue_Create(RawServoCssUrlDataStrong aCssUrl, CORSMode aCORSMode)
{
  RefPtr<css::URLValue> url = new css::URLValue(aCssUrl.Consume(), aCORSMode);
  return url.forget().take();
}

MOZ_DEFINE_MALLOC_SIZE_OF(GeckoURLValueMallocSizeOf)

size_t
Gecko_URLValue_SizeOfIncludingThis(URLValue* aURL)
{
  MOZ_ASSERT(NS_IsMainThread());
  return aURL->SizeOfIncludingThis(GeckoURLValueMallocSizeOf);
}

void
Gecko_GetComputedURLSpec(const URLValue* aURL, nsCString* aOut)
{
=======
void Gecko_GetComputedURLSpec(const StyleComputedUrl* aURL, nsCString* aOut) {
>>>>>>> upstream-releases
  MOZ_ASSERT(aURL);
  MOZ_ASSERT(aOut);
  if (aURL->IsLocalRef()) {
    aOut->Assign(aURL->SpecifiedSerialization());
    return;
  }
  Gecko_GetComputedImageURLSpec(aURL, aOut);
}

<<<<<<< HEAD
void Gecko_GetComputedImageURLSpec(const URLValue* aURL, nsCString* aOut) {
||||||| merged common ancestors
void
Gecko_GetComputedImageURLSpec(const URLValue* aURL, nsCString* aOut)
{
=======
void Gecko_GetComputedImageURLSpec(const StyleComputedUrl* aURL,
                                   nsCString* aOut) {
>>>>>>> upstream-releases
  // Image URIs don't serialize local refs as local.
  if (nsIURI* uri = aURL->GetURI()) {
    nsresult rv = uri->GetSpec(*aOut);
    if (NS_SUCCEEDED(rv)) {
      return;
    }
  }

  aOut->AssignLiteral("about:invalid");
}

void Gecko_nsIURI_Debug(nsIURI* aURI, nsCString* aOut) {
  // TODO(emilio): Do we have more useful stuff to put here, maybe?
  if (aURI) {
    *aOut = aURI->GetSpecOrDefault();
  }
}

// XXX Implemented by hand because even though it's thread-safe, only the
// subclasses have the HasThreadSafeRefCnt bits.
void Gecko_AddRefnsIURIArbitraryThread(nsIURI* aPtr) { NS_ADDREF(aPtr); }
void Gecko_ReleasensIURIArbitraryThread(nsIURI* aPtr) { NS_RELEASE(aPtr); }

template <typename ElementLike>
void DebugListAttributes(const ElementLike& aElement, nsCString& aOut) {
  const uint32_t kMaxAttributeLength = 40;

  uint32_t i = 0;
  while (BorrowedAttrInfo info = aElement.GetAttrInfoAt(i++)) {
    aOut.AppendLiteral(" ");
    if (nsAtom* prefix = info.mName->GetPrefix()) {
      aOut.Append(NS_ConvertUTF16toUTF8(nsDependentAtomString(prefix)));
      aOut.AppendLiteral(":");
    }
    aOut.Append(
        NS_ConvertUTF16toUTF8(nsDependentAtomString(info.mName->LocalName())));
    if (!info.mValue) {
      continue;
    }
    aOut.AppendLiteral("=\"");
    nsAutoString value;
    info.mValue->ToString(value);
    if (value.Length() > kMaxAttributeLength) {
      value.Truncate(kMaxAttributeLength - 3);
      value.AppendLiteral("...");
    }
    aOut.Append(NS_ConvertUTF16toUTF8(value));
    aOut.AppendLiteral("\"");
  }
}

<<<<<<< HEAD
void Gecko_Element_DebugListAttributes(RawGeckoElementBorrowed aElement,
                                       nsCString* aOut) {
||||||| merged common ancestors
void
Gecko_Element_DebugListAttributes(RawGeckoElementBorrowed aElement, nsCString* aOut)
{
=======
void Gecko_Element_DebugListAttributes(const Element* aElement,
                                       nsCString* aOut) {
>>>>>>> upstream-releases
  DebugListAttributes(*aElement, *aOut);
}

void Gecko_Snapshot_DebugListAttributes(const ServoElementSnapshot* aSnapshot,
                                        nsCString* aOut) {
  DebugListAttributes(*aSnapshot, *aOut);
}

NS_IMPL_THREADSAFE_FFI_REFCOUNTING(URLExtraData, URLExtraData);

<<<<<<< HEAD
NS_IMPL_THREADSAFE_FFI_REFCOUNTING(nsStyleCoord::Calc, Calc);

nsCSSShadowArray* Gecko_NewCSSShadowArray(uint32_t aLen) {
  RefPtr<nsCSSShadowArray> arr = new (aLen) nsCSSShadowArray(aLen);
  return arr.forget().take();
}

NS_IMPL_THREADSAFE_FFI_REFCOUNTING(nsCSSShadowArray, CSSShadowArray);

nsCSSValueSharedList* Gecko_NewCSSValueSharedList(uint32_t aLen) {
  RefPtr<nsCSSValueSharedList> list = new nsCSSValueSharedList;
  if (aLen == 0) {
    return list.forget().take();
  }

  list->mHead = new nsCSSValueList;
  nsCSSValueList* cur = list->mHead;
  for (uint32_t i = 0; i < aLen - 1; i++) {
    cur->mNext = new nsCSSValueList;
    cur = cur->mNext;
  }

  return list.forget().take();
}

nsCSSValueSharedList* Gecko_NewNoneTransform() {
  RefPtr<nsCSSValueSharedList> list = new nsCSSValueSharedList;
  list->mHead = new nsCSSValueList;
  list->mHead->mValue.SetNoneValue();
  return list.forget().take();
}

void Gecko_CSSValue_SetNumber(nsCSSValueBorrowedMut aCSSValue, float aNumber) {
  aCSSValue->SetFloatValue(aNumber, eCSSUnit_Number);
}

float Gecko_CSSValue_GetNumber(nsCSSValueBorrowed aCSSValue) {
  return aCSSValue->GetFloatValue();
}

void Gecko_CSSValue_SetKeyword(nsCSSValueBorrowedMut aCSSValue,
                               nsCSSKeyword aKeyword) {
  aCSSValue->SetEnumValue(aKeyword);
}

nsCSSKeyword Gecko_CSSValue_GetKeyword(nsCSSValueBorrowed aCSSValue) {
  return aCSSValue->GetKeywordValue();
}

void Gecko_CSSValue_SetPercentage(nsCSSValueBorrowedMut aCSSValue,
                                  float aPercent) {
  aCSSValue->SetPercentValue(aPercent);
}

float Gecko_CSSValue_GetPercentage(nsCSSValueBorrowed aCSSValue) {
  return aCSSValue->GetPercentValue();
}

void Gecko_CSSValue_SetPixelLength(nsCSSValueBorrowedMut aCSSValue,
                                   float aLen) {
  MOZ_ASSERT(aCSSValue->GetUnit() == eCSSUnit_Null ||
             aCSSValue->GetUnit() == eCSSUnit_Pixel);
  aCSSValue->SetFloatValue(aLen, eCSSUnit_Pixel);
}

void Gecko_CSSValue_SetCalc(nsCSSValueBorrowedMut aCSSValue,
                            nsStyleCoord::CalcValue aCalc) {
  aCSSValue->SetCalcValue(aCalc);
}

nsStyleCoord::CalcValue Gecko_CSSValue_GetCalc(nsCSSValueBorrowed aCSSValue) {
  return aCSSValue->GetCalcValue();
}

void Gecko_CSSValue_SetFunction(nsCSSValueBorrowedMut aCSSValue, int32_t aLen) {
  nsCSSValue::Array* arr = nsCSSValue::Array::Create(aLen);
  aCSSValue->SetArrayValue(arr, eCSSUnit_Function);
}

void Gecko_CSSValue_SetString(nsCSSValueBorrowedMut aCSSValue,
                              const uint8_t* aString, uint32_t aLength,
                              nsCSSUnit aUnit) {
  MOZ_ASSERT(aCSSValue->GetUnit() == eCSSUnit_Null);
  nsString string;
  nsDependentCSubstring slice(reinterpret_cast<const char*>(aString), aLength);
  AppendUTF8toUTF16(slice, string);
  aCSSValue->SetStringValue(string, aUnit);
}

void Gecko_CSSValue_SetStringFromAtom(nsCSSValueBorrowedMut aCSSValue,
                                      nsAtom* aAtom, nsCSSUnit aUnit) {
  aCSSValue->SetStringValue(nsDependentAtomString(aAtom), aUnit);
}

void Gecko_CSSValue_SetAtomIdent(nsCSSValueBorrowedMut aCSSValue,
                                 nsAtom* aAtom) {
  aCSSValue->SetAtomIdentValue(already_AddRefed<nsAtom>(aAtom));
}

void Gecko_CSSValue_SetArray(nsCSSValueBorrowedMut aCSSValue, int32_t aLength) {
  MOZ_ASSERT(aCSSValue->GetUnit() == eCSSUnit_Null);
  RefPtr<nsCSSValue::Array> array = nsCSSValue::Array::Create(aLength);
  aCSSValue->SetArrayValue(array, eCSSUnit_Array);
}

void Gecko_CSSValue_SetInt(nsCSSValueBorrowedMut aCSSValue, int32_t aInteger,
                           nsCSSUnit aUnit) {
  aCSSValue->SetIntValue(aInteger, aUnit);
}

void Gecko_CSSValue_SetFloat(nsCSSValueBorrowedMut aCSSValue, float aValue,
                             nsCSSUnit aUnit) {
  aCSSValue->SetFloatValue(aValue, aUnit);
}

nsCSSValueBorrowedMut Gecko_CSSValue_GetArrayItem(
    nsCSSValueBorrowedMut aCSSValue, int32_t aIndex) {
  return &aCSSValue->GetArrayValue()->Item(aIndex);
}

nsCSSValueBorrowed Gecko_CSSValue_GetArrayItemConst(
    nsCSSValueBorrowed aCSSValue, int32_t aIndex) {
  return &aCSSValue->GetArrayValue()->Item(aIndex);
}

void Gecko_CSSValue_SetPair(nsCSSValueBorrowedMut aCSSValue,
                            nsCSSValueBorrowed aXValue,
                            nsCSSValueBorrowed aYValue) {
  MOZ_ASSERT(NS_IsMainThread());
  aCSSValue->SetPairValue(*aXValue, *aYValue);
}

void Gecko_CSSValue_SetList(nsCSSValueBorrowedMut aCSSValue, uint32_t aLen) {
  MOZ_ASSERT(NS_IsMainThread());
  nsCSSValueList* item = aCSSValue->SetListValue();
  for (uint32_t i = 1; i < aLen; ++i) {
    item->mNext = new nsCSSValueList;
    item = item->mNext;
  }
}

void Gecko_CSSValue_SetPairList(nsCSSValueBorrowedMut aCSSValue,
                                uint32_t aLen) {
  MOZ_ASSERT(NS_IsMainThread());
  nsCSSValuePairList* item = aCSSValue->SetPairListValue();
  for (uint32_t i = 1; i < aLen; ++i) {
    item->mNext = new nsCSSValuePairList;
    item = item->mNext;
  }
}

void Gecko_CSSValue_InitSharedList(nsCSSValueBorrowedMut aCSSValue,
                                   uint32_t aLen) {
  MOZ_ASSERT(aLen > 0, "Must create at least one nsCSSValueList (mHead)");

  nsCSSValueSharedList* list = new nsCSSValueSharedList;
  aCSSValue->SetSharedListValue(list);
  list->mHead = new nsCSSValueList;
  nsCSSValueList* cur = list->mHead;
  for (uint32_t i = 1; i < aLen; ++i) {
    cur->mNext = new nsCSSValueList;
    cur = cur->mNext;
  }
}

void Gecko_CSSValue_Drop(nsCSSValueBorrowedMut aCSSValue) {
  aCSSValue->~nsCSSValue();
}

void Gecko_nsStyleFont_SetLang(nsStyleFont* aFont, nsAtom* aAtom) {
||||||| merged common ancestors
NS_IMPL_THREADSAFE_FFI_REFCOUNTING(nsStyleCoord::Calc, Calc);

nsCSSShadowArray*
Gecko_NewCSSShadowArray(uint32_t aLen)
{
  RefPtr<nsCSSShadowArray> arr = new(aLen) nsCSSShadowArray(aLen);
  return arr.forget().take();
}

NS_IMPL_THREADSAFE_FFI_REFCOUNTING(nsCSSShadowArray, CSSShadowArray);

nsStyleQuoteValues*
Gecko_NewStyleQuoteValues(uint32_t aLen)
{
  RefPtr<nsStyleQuoteValues> values = new nsStyleQuoteValues;
  values->mQuotePairs.SetLength(aLen);
  return values.forget().take();
}

NS_IMPL_THREADSAFE_FFI_REFCOUNTING(nsStyleQuoteValues, QuoteValues);

nsCSSValueSharedList*
Gecko_NewCSSValueSharedList(uint32_t aLen)
{
  RefPtr<nsCSSValueSharedList> list = new nsCSSValueSharedList;
  if (aLen == 0) {
    return list.forget().take();
  }

  list->mHead = new nsCSSValueList;
  nsCSSValueList* cur = list->mHead;
  for (uint32_t i = 0; i < aLen - 1; i++) {
    cur->mNext = new nsCSSValueList;
    cur = cur->mNext;
  }

  return list.forget().take();
}

nsCSSValueSharedList*
Gecko_NewNoneTransform()
{
  RefPtr<nsCSSValueSharedList> list = new nsCSSValueSharedList;
  list->mHead = new nsCSSValueList;
  list->mHead->mValue.SetNoneValue();
  return list.forget().take();
}

void
Gecko_CSSValue_SetNumber(nsCSSValueBorrowedMut aCSSValue, float aNumber)
{
  aCSSValue->SetFloatValue(aNumber, eCSSUnit_Number);
}

float
Gecko_CSSValue_GetNumber(nsCSSValueBorrowed aCSSValue)
{
  return aCSSValue->GetFloatValue();
}

void
Gecko_CSSValue_SetKeyword(nsCSSValueBorrowedMut aCSSValue, nsCSSKeyword aKeyword)
{
  aCSSValue->SetEnumValue(aKeyword);
}

nsCSSKeyword
Gecko_CSSValue_GetKeyword(nsCSSValueBorrowed aCSSValue)
{
  return aCSSValue->GetKeywordValue();
}

void
Gecko_CSSValue_SetPercentage(nsCSSValueBorrowedMut aCSSValue, float aPercent)
{
  aCSSValue->SetPercentValue(aPercent);
}

float
Gecko_CSSValue_GetPercentage(nsCSSValueBorrowed aCSSValue)
{
  return aCSSValue->GetPercentValue();
}

void
Gecko_CSSValue_SetPixelLength(nsCSSValueBorrowedMut aCSSValue, float aLen)
{
  MOZ_ASSERT(aCSSValue->GetUnit() == eCSSUnit_Null ||
             aCSSValue->GetUnit() == eCSSUnit_Pixel);
  aCSSValue->SetFloatValue(aLen, eCSSUnit_Pixel);
}

void
Gecko_CSSValue_SetCalc(nsCSSValueBorrowedMut aCSSValue, nsStyleCoord::CalcValue aCalc)
{
  aCSSValue->SetCalcValue(aCalc);
}

nsStyleCoord::CalcValue
Gecko_CSSValue_GetCalc(nsCSSValueBorrowed aCSSValue)
{
  return aCSSValue->GetCalcValue();
}

void
Gecko_CSSValue_SetFunction(nsCSSValueBorrowedMut aCSSValue, int32_t aLen)
{
  nsCSSValue::Array* arr = nsCSSValue::Array::Create(aLen);
  aCSSValue->SetArrayValue(arr, eCSSUnit_Function);
}

void
Gecko_CSSValue_SetString(nsCSSValueBorrowedMut aCSSValue,
                         const uint8_t* aString, uint32_t aLength,
                         nsCSSUnit aUnit)
{
  MOZ_ASSERT(aCSSValue->GetUnit() == eCSSUnit_Null);
  nsString string;
  nsDependentCSubstring slice(reinterpret_cast<const char*>(aString),
                                  aLength);
  AppendUTF8toUTF16(slice, string);
  aCSSValue->SetStringValue(string, aUnit);
}

void
Gecko_CSSValue_SetStringFromAtom(nsCSSValueBorrowedMut aCSSValue,
                                 nsAtom* aAtom, nsCSSUnit aUnit)
{
  aCSSValue->SetStringValue(nsDependentAtomString(aAtom), aUnit);
}

void
Gecko_CSSValue_SetAtomIdent(nsCSSValueBorrowedMut aCSSValue, nsAtom* aAtom)
{
  aCSSValue->SetAtomIdentValue(already_AddRefed<nsAtom>(aAtom));
}

void
Gecko_CSSValue_SetArray(nsCSSValueBorrowedMut aCSSValue, int32_t aLength)
{
  MOZ_ASSERT(aCSSValue->GetUnit() == eCSSUnit_Null);
  RefPtr<nsCSSValue::Array> array
    = nsCSSValue::Array::Create(aLength);
  aCSSValue->SetArrayValue(array, eCSSUnit_Array);
}

void
Gecko_CSSValue_SetInt(nsCSSValueBorrowedMut aCSSValue,
                      int32_t aInteger, nsCSSUnit aUnit)
{
  aCSSValue->SetIntValue(aInteger, aUnit);
}

void
Gecko_CSSValue_SetFloat(nsCSSValueBorrowedMut aCSSValue,
                        float aValue, nsCSSUnit aUnit)
{
  aCSSValue->SetFloatValue(aValue, aUnit);
}

nsCSSValueBorrowedMut
Gecko_CSSValue_GetArrayItem(nsCSSValueBorrowedMut aCSSValue, int32_t aIndex)
{
  return &aCSSValue->GetArrayValue()->Item(aIndex);
}

nsCSSValueBorrowed
Gecko_CSSValue_GetArrayItemConst(nsCSSValueBorrowed aCSSValue, int32_t aIndex)
{
  return &aCSSValue->GetArrayValue()->Item(aIndex);
}

void
Gecko_CSSValue_SetPair(nsCSSValueBorrowedMut aCSSValue,
                       nsCSSValueBorrowed aXValue, nsCSSValueBorrowed aYValue)
{
  MOZ_ASSERT(NS_IsMainThread());
  aCSSValue->SetPairValue(*aXValue, *aYValue);
}

void
Gecko_CSSValue_SetList(nsCSSValueBorrowedMut aCSSValue, uint32_t aLen)
{
  MOZ_ASSERT(NS_IsMainThread());
  nsCSSValueList* item = aCSSValue->SetListValue();
  for (uint32_t i = 1; i < aLen; ++i) {
    item->mNext = new nsCSSValueList;
    item = item->mNext;
  }
}

void
Gecko_CSSValue_SetPairList(nsCSSValueBorrowedMut aCSSValue, uint32_t aLen)
{
  MOZ_ASSERT(NS_IsMainThread());
  nsCSSValuePairList* item = aCSSValue->SetPairListValue();
  for (uint32_t i = 1; i < aLen; ++i) {
    item->mNext = new nsCSSValuePairList;
    item = item->mNext;
  }
}

void
Gecko_CSSValue_InitSharedList(nsCSSValueBorrowedMut aCSSValue,
                              uint32_t aLen)
{
  MOZ_ASSERT(aLen > 0, "Must create at least one nsCSSValueList (mHead)");

  nsCSSValueSharedList* list = new nsCSSValueSharedList;
  aCSSValue->SetSharedListValue(list);
  list->mHead = new nsCSSValueList;
  nsCSSValueList* cur = list->mHead;
  for (uint32_t i = 1; i < aLen; ++i) {
    cur->mNext = new nsCSSValueList;
    cur = cur->mNext;
  }
}

void
Gecko_CSSValue_Drop(nsCSSValueBorrowedMut aCSSValue)
{
  aCSSValue->~nsCSSValue();
}

void
Gecko_nsStyleFont_SetLang(nsStyleFont* aFont, nsAtom* aAtom)
{
=======
void Gecko_nsStyleFont_SetLang(nsStyleFont* aFont, nsAtom* aAtom) {
>>>>>>> upstream-releases
  aFont->mLanguage = dont_AddRef(aAtom);
  aFont->mExplicitLanguage = true;
}

void Gecko_nsStyleFont_CopyLangFrom(nsStyleFont* aFont,
                                    const nsStyleFont* aSource) {
  aFont->mLanguage = aSource->mLanguage;
}

<<<<<<< HEAD
void Gecko_nsStyleFont_FixupNoneGeneric(
    nsStyleFont* aFont, RawGeckoPresContextBorrowed aPresContext) {
  const nsFont* defaultVariableFont = ThreadSafeGetDefaultFontHelper(
      aPresContext, aFont->mLanguage, kPresContext_DefaultVariableFont_ID);
  nsLayoutUtils::FixupNoneGeneric(&aFont->mFont, aPresContext,
                                  aFont->mGenericID, defaultVariableFont);
}

void Gecko_nsStyleFont_PrefillDefaultForGeneric(
    nsStyleFont* aFont, RawGeckoPresContextBorrowed aPresContext,
    uint8_t aGenericId) {
  const nsFont* defaultFont = ThreadSafeGetDefaultFontHelper(
      aPresContext, aFont->mLanguage, aGenericId);
  // In case of just the language changing, the parent could have had no
  // generic, which Gecko just does regular cascading with. Do the same. This
  // can only happen in the case where the language changed but the family did
  // not
  if (aGenericId != kGenericFont_NONE) {
    aFont->mFont.fontlist = defaultFont->fontlist;
  } else {
    aFont->mFont.fontlist.SetDefaultFontType(
        defaultFont->fontlist.GetDefaultFontType());
||||||| merged common ancestors
void
Gecko_nsStyleFont_FixupNoneGeneric(nsStyleFont* aFont,
                                   RawGeckoPresContextBorrowed aPresContext)
{
  const nsFont* defaultVariableFont =
    ThreadSafeGetDefaultFontHelper(aPresContext, aFont->mLanguage,
                                   kPresContext_DefaultVariableFont_ID);
  nsLayoutUtils::FixupNoneGeneric(&aFont->mFont, aPresContext,
                                  aFont->mGenericID, defaultVariableFont);
}

void
Gecko_nsStyleFont_PrefillDefaultForGeneric(nsStyleFont* aFont,
                                           RawGeckoPresContextBorrowed aPresContext,
                                           uint8_t aGenericId)
{
  const nsFont* defaultFont = ThreadSafeGetDefaultFontHelper(aPresContext, aFont->mLanguage,
                                                             aGenericId);
  // In case of just the language changing, the parent could have had no generic,
  // which Gecko just does regular cascading with. Do the same.
  // This can only happen in the case where the language changed but the family did not
  if (aGenericId != kGenericFont_NONE) {
    aFont->mFont.fontlist = defaultFont->fontlist;
  } else {
    aFont->mFont.fontlist.SetDefaultFontType(defaultFont->fontlist.GetDefaultFontType());
=======
void Gecko_nsStyleFont_PrioritizeUserFonts(
    nsStyleFont* aFont, StyleGenericFontFamily aDefaultGeneric) {
  MOZ_ASSERT(!StaticPrefs::browser_display_use_document_fonts());
  MOZ_ASSERT(aDefaultGeneric != StyleGenericFontFamily::None);
  if (!aFont->mFont.fontlist.PrioritizeFirstGeneric()) {
    aFont->mFont.fontlist.PrependGeneric(aDefaultGeneric);
>>>>>>> upstream-releases
  }
}

<<<<<<< HEAD
void Gecko_nsStyleFont_FixupMinFontSize(
    nsStyleFont* aFont, RawGeckoPresContextBorrowed aPresContext) {
||||||| merged common ancestors
void
Gecko_nsStyleFont_FixupMinFontSize(nsStyleFont* aFont,
                                   RawGeckoPresContextBorrowed aPresContext)
{
=======
nscoord Gecko_nsStyleFont_ComputeMinSize(const nsStyleFont* aFont,
                                         const Document* aDocument) {
  // Don't change font-size:0, since that would un-hide hidden text, nor chrome
  // docs, we assume those know what they do.
  if (aFont->mSize == 0 || nsContentUtils::IsChromeDoc(aDocument)) {
    return 0;
  }

>>>>>>> upstream-releases
  nscoord minFontSize;
  bool needsCache = false;

  auto MinFontSize = [&](bool* aNeedsToCache) {
    auto* prefs =
        aDocument->GetFontPrefsForLang(aFont->mLanguage, aNeedsToCache);
    return prefs ? prefs->mMinimumFontSize : 0;
  };

  {
    AutoReadLock guard(*sServoFFILock);
    minFontSize = MinFontSize(&needsCache);
  }

  if (needsCache) {
    AutoWriteLock guard(*sServoFFILock);
    minFontSize = MinFontSize(nullptr);
  }

  if (minFontSize < 0) {
    return 0;
  }

  return (minFontSize * aFont->mMinFontSizeRatio) / 100;
}

void FontSizePrefs::CopyFrom(const LangGroupFontPrefs& prefs) {
  mDefaultVariableSize = prefs.mDefaultVariableFont.size;
  mDefaultSerifSize = prefs.mDefaultSerifFont.size;
  mDefaultSansSerifSize = prefs.mDefaultSansSerifFont.size;
  mDefaultMonospaceSize = prefs.mDefaultMonospaceFont.size;
  mDefaultCursiveSize = prefs.mDefaultCursiveFont.size;
  mDefaultFantasySize = prefs.mDefaultFantasyFont.size;
}

FontSizePrefs Gecko_GetBaseSize(nsAtom* aLanguage) {
  LangGroupFontPrefs prefs;
<<<<<<< HEAD
  RefPtr<nsAtom> langGroupAtom =
      StaticPresData::Get()->GetUncachedLangGroup(aLanguage);

||||||| merged common ancestors
  RefPtr<nsAtom> langGroupAtom = StaticPresData::Get()->GetUncachedLangGroup(aLanguage);

=======
  nsStaticAtom* langGroupAtom =
      StaticPresData::Get()->GetUncachedLangGroup(aLanguage);
>>>>>>> upstream-releases
  prefs.Initialize(langGroupAtom);
  FontSizePrefs sizes;
  sizes.CopyFrom(prefs);
  return sizes;
}

<<<<<<< HEAD
RawGeckoElementBorrowedOrNull Gecko_GetBindingParent(
    RawGeckoElementBorrowed aElement) {
  nsIContent* parent = aElement->GetBindingParent();
  return parent ? parent->AsElement() : nullptr;
}

RawServoAuthorStylesBorrowedOrNull Gecko_XBLBinding_GetRawServoStyles(
    RawGeckoXBLBindingBorrowed aXBLBinding) {
  return aXBLBinding->GetServoStyles();
}

bool Gecko_XBLBinding_InheritsStyle(RawGeckoXBLBindingBorrowed aXBLBinding) {
  return aXBLBinding->InheritsStyle();
||||||| merged common ancestors
RawGeckoElementBorrowedOrNull
Gecko_GetBindingParent(RawGeckoElementBorrowed aElement)
{
  nsIContent* parent = aElement->GetBindingParent();
  return parent ? parent->AsElement() : nullptr;
}

RawServoAuthorStylesBorrowedOrNull
Gecko_XBLBinding_GetRawServoStyles(RawGeckoXBLBindingBorrowed aXBLBinding)
{
  return aXBLBinding->GetServoStyles();
}

bool
Gecko_XBLBinding_InheritsStyle(RawGeckoXBLBindingBorrowed aXBLBinding)
{
  return aXBLBinding->InheritsStyle();
=======
const Element* Gecko_GetBindingParent(const Element* aElement) {
  return aElement->GetBindingParent();
>>>>>>> upstream-releases
}

static StaticRefPtr<UACacheReporter> gUACacheReporter;

namespace mozilla {

void InitializeServo() {
  URLExtraData::InitDummy();
  Servo_Initialize(URLExtraData::Dummy());

  gUACacheReporter = new UACacheReporter();
  RegisterWeakMemoryReporter(gUACacheReporter);

  sServoFFILock = new RWLock("Servo::FFILock");
}

void ShutdownServo() {
  MOZ_ASSERT(sServoFFILock);

  UnregisterWeakMemoryReporter(gUACacheReporter);
  gUACacheReporter = nullptr;

  delete sServoFFILock;
  Servo_Shutdown();
}

void AssertIsMainThreadOrServoFontMetricsLocked() {
  if (!NS_IsMainThread()) {
    MOZ_ASSERT(sServoFFILock &&
               sServoFFILock->LockedForWritingByCurrentThread());
  }
}

}  // namespace mozilla

<<<<<<< HEAD
GeckoFontMetrics Gecko_GetFontMetrics(RawGeckoPresContextBorrowed aPresContext,
                                      bool aIsVertical,
                                      const nsStyleFont* aFont,
                                      nscoord aFontSize, bool aUseUserFontSet) {
||||||| merged common ancestors
GeckoFontMetrics
Gecko_GetFontMetrics(RawGeckoPresContextBorrowed aPresContext,
                     bool aIsVertical,
                     const nsStyleFont* aFont,
                     nscoord aFontSize,
                     bool aUseUserFontSet)
{
=======
GeckoFontMetrics Gecko_GetFontMetrics(const nsPresContext* aPresContext,
                                      bool aIsVertical,
                                      const nsStyleFont* aFont,
                                      nscoord aFontSize, bool aUseUserFontSet) {
>>>>>>> upstream-releases
  AutoWriteLock guard(*sServoFFILock);
  GeckoFontMetrics ret;

  // Getting font metrics can require some main thread only work to be
  // done, such as work that needs to touch non-threadsafe refcounted
  // objects (like the DOM FontFace/FontFaceSet objects), network loads, etc.
  //
  // To handle this work, font code checks whether we are in a Servo traversal
  // and if so, appends PostTraversalTasks to the current ServoStyleSet
  // to be performed immediately after the traversal is finished.  This
  // works well for starting downloadable font loads, since we don't have
  // those fonts available to get metrics for anyway.  Platform fonts and
  // ArrayBuffer-backed FontFace objects are handled synchronously.

  nsPresContext* presContext = const_cast<nsPresContext*>(aPresContext);
  presContext->SetUsesExChUnits(true);
  RefPtr<nsFontMetrics> fm = nsLayoutUtils::GetMetricsFor(
      presContext, aIsVertical, aFont, aFontSize, aUseUserFontSet);

  ret.mXSize = fm->XHeight();
<<<<<<< HEAD
  gfxFloat zeroWidth = fm->GetThebesFontGroup()
                           ->GetFirstValidFont()
                           ->GetMetrics(fm->Orientation())
                           .zeroOrAveCharWidth;
  ret.mChSize = NS_round(aPresContext->AppUnitsPerDevPixel() * zeroWidth);
||||||| merged common ancestors
  gfxFloat zeroWidth = fm->GetThebesFontGroup()->GetFirstValidFont()->
                           GetMetrics(fm->Orientation()).zeroOrAveCharWidth;
  ret.mChSize = NS_round(aPresContext->AppUnitsPerDevPixel() * zeroWidth);
=======
  gfxFloat zeroWidth = fm->GetThebesFontGroup()
                           ->GetFirstValidFont()
                           ->GetMetrics(fm->Orientation())
                           .zeroWidth;
  ret.mChSize = zeroWidth >= 0.0
                    ? NS_round(aPresContext->AppUnitsPerDevPixel() * zeroWidth)
                    : -1.0;
>>>>>>> upstream-releases
  return ret;
}

<<<<<<< HEAD
int32_t Gecko_GetAppUnitsPerPhysicalInch(
    RawGeckoPresContextBorrowed aPresContext) {
  nsPresContext* presContext = const_cast<nsPresContext*>(aPresContext);
  return presContext->DeviceContext()->AppUnitsPerPhysicalInch();
}

||||||| merged common ancestors
int32_t
Gecko_GetAppUnitsPerPhysicalInch(RawGeckoPresContextBorrowed aPresContext)
{
  nsPresContext* presContext = const_cast<nsPresContext*>(aPresContext);
  return presContext->DeviceContext()->AppUnitsPerPhysicalInch();
}

=======
>>>>>>> upstream-releases
NS_IMPL_THREADSAFE_FFI_REFCOUNTING(SheetLoadDataHolder, SheetLoadDataHolder);

<<<<<<< HEAD
void Gecko_StyleSheet_FinishAsyncParse(
    SheetLoadDataHolder* aData, RawServoStyleSheetContentsStrong aSheetContents,
    StyleUseCountersOwned aUseCounters) {
  UniquePtr<StyleUseCounters> useCounters(aUseCounters);
||||||| merged common ancestors
void
Gecko_StyleSheet_FinishAsyncParse(SheetLoadDataHolder* aData,
                                  RawServoStyleSheetContentsStrong aSheetContents,
                                  StyleUseCountersOwned aUseCounters)
{
  UniquePtr<StyleUseCounters> useCounters(aUseCounters);
=======
void Gecko_StyleSheet_FinishAsyncParse(
    SheetLoadDataHolder* aData,
    StyleStrong<RawServoStyleSheetContents> aSheetContents,
    StyleOwnedOrNull<StyleUseCounters> aUseCounters) {
  UniquePtr<StyleUseCounters> useCounters = aUseCounters.Consume();
>>>>>>> upstream-releases
  RefPtr<SheetLoadDataHolder> loadData = aData;
  RefPtr<RawServoStyleSheetContents> sheetContents = aSheetContents.Consume();
<<<<<<< HEAD
  NS_DispatchToMainThread(NS_NewRunnableFunction(
      __func__, [d = std::move(loadData), contents = std::move(sheetContents),
                 counters = std::move(useCounters)]() mutable {
        MOZ_ASSERT(NS_IsMainThread());
        SheetLoadData* data = d->get();
        if (nsIDocument* doc = data->mLoader->GetDocument()) {
          if (const StyleUseCounters* docCounters =
                  doc->GetStyleUseCounters()) {
            Servo_UseCounters_Merge(docCounters, counters.get());
          }
        }
        data->mSheet->FinishAsyncParse(contents.forget());
      }));
}

static already_AddRefed<StyleSheet> LoadImportSheet(
    css::Loader* aLoader, StyleSheet* aParent, SheetLoadData* aParentLoadData,
    css::LoaderReusableStyleSheets* aReusableSheets, css::URLValue* aURL,
    already_AddRefed<RawServoMediaList> aMediaList) {
||||||| merged common ancestors
  NS_DispatchToMainThread(NS_NewRunnableFunction(__func__,
                                                 [d = std::move(loadData),
                                                  contents = std::move(sheetContents),
                                                  counters = std::move(useCounters)]() mutable {
    MOZ_ASSERT(NS_IsMainThread());
    SheetLoadData* data = d->get();
    if (nsIDocument* doc = data->mLoader->GetDocument()) {
      if (const StyleUseCounters* docCounters = doc->GetStyleUseCounters()) {
        Servo_UseCounters_Merge(docCounters, counters.get());
      }
    }
    data->mSheet->FinishAsyncParse(contents.forget());
  }));
}

static already_AddRefed<StyleSheet>
LoadImportSheet(css::Loader* aLoader,
                StyleSheet* aParent,
                SheetLoadData* aParentLoadData,
                css::LoaderReusableStyleSheets* aReusableSheets,
                css::URLValue* aURL,
                already_AddRefed<RawServoMediaList> aMediaList)
{
=======
  NS_DispatchToMainThread(NS_NewRunnableFunction(
      __func__, [d = std::move(loadData), contents = std::move(sheetContents),
                 counters = std::move(useCounters)]() mutable {
        MOZ_ASSERT(NS_IsMainThread());
        SheetLoadData* data = d->get();
        if (Document* doc = data->mLoader->GetDocument()) {
          if (const StyleUseCounters* docCounters =
                  doc->GetStyleUseCounters()) {
            Servo_UseCounters_Merge(docCounters, counters.get());
          }
        }
        data->mSheet->FinishAsyncParse(contents.forget());
      }));
}

static already_AddRefed<StyleSheet> LoadImportSheet(
    Loader* aLoader, StyleSheet* aParent, SheetLoadData* aParentLoadData,
    LoaderReusableStyleSheets* aReusableSheets, const StyleCssUrl& aURL,
    already_AddRefed<RawServoMediaList> aMediaList) {
>>>>>>> upstream-releases
  MOZ_ASSERT(NS_IsMainThread());
  MOZ_ASSERT(aLoader, "Should've catched this before");
  MOZ_ASSERT(aParent, "Only used for @import, so parent should exist!");

  RefPtr<MediaList> media = new MediaList(std::move(aMediaList));
  nsCOMPtr<nsIURI> uri = aURL.GetURI();
  nsresult rv = uri ? NS_OK : NS_ERROR_FAILURE;

  StyleSheet* previousFirstChild = aParent->GetFirstChild();
  if (NS_SUCCEEDED(rv)) {
    rv = aLoader->LoadChildSheet(aParent, aParentLoadData, uri, media,
                                 aReusableSheets);
  }

  if (NS_FAILED(rv) || !aParent->GetFirstChild() ||
      aParent->GetFirstChild() == previousFirstChild) {
    // Servo and Gecko have different ideas of what a valid URL is, so we might
    // get in here with a URL string that NS_NewURI can't handle.  We may also
    // reach here via an import cycle.  For the import cycle case, we need some
    // sheet object per spec, even if its empty.  DevTools uses the URI to
    // realize it has hit an import cycle, so we mark it complete to make the
    // sheet readable from JS.
    RefPtr<StyleSheet> emptySheet =
        aParent->CreateEmptyChildSheet(media.forget());
    // Make a dummy URI if we don't have one because some methods assume
    // non-null URIs.
    if (!uri) {
      NS_NewURI(getter_AddRefs(uri), NS_LITERAL_CSTRING("about:invalid"));
    }
    emptySheet->SetURIs(uri, uri, uri);
    emptySheet->SetPrincipal(aURL.ExtraData().Principal());
    emptySheet->SetComplete();
    aParent->PrependStyleSheet(emptySheet);
    return emptySheet.forget();
  }

  RefPtr<StyleSheet> sheet = static_cast<StyleSheet*>(aParent->GetFirstChild());
  return sheet.forget();
}

<<<<<<< HEAD
StyleSheet* Gecko_LoadStyleSheet(
    css::Loader* aLoader, StyleSheet* aParent, SheetLoadData* aParentLoadData,
    css::LoaderReusableStyleSheets* aReusableSheets,
    RawServoCssUrlDataStrong aCssUrl, RawServoMediaListStrong aMediaList) {
||||||| merged common ancestors
StyleSheet*
Gecko_LoadStyleSheet(css::Loader* aLoader,
                     StyleSheet* aParent,
                     SheetLoadData* aParentLoadData,
                     css::LoaderReusableStyleSheets* aReusableSheets,
                     RawServoCssUrlDataStrong aCssUrl,
                     RawServoMediaListStrong aMediaList)
{
=======
StyleSheet* Gecko_LoadStyleSheet(Loader* aLoader, StyleSheet* aParent,
                                 SheetLoadData* aParentLoadData,
                                 LoaderReusableStyleSheets* aReusableSheets,
                                 const StyleCssUrl* aUrl,
                                 StyleStrong<RawServoMediaList> aMediaList) {
>>>>>>> upstream-releases
  MOZ_ASSERT(NS_IsMainThread());
  MOZ_ASSERT(aUrl);

  return LoadImportSheet(aLoader, aParent, aParentLoadData, aReusableSheets,
<<<<<<< HEAD
                         url, aMediaList.Consume())
      .take();
||||||| merged common ancestors
                         url, aMediaList.Consume()).take();
=======
                         *aUrl, aMediaList.Consume())
      .take();
>>>>>>> upstream-releases
}

<<<<<<< HEAD
void Gecko_LoadStyleSheetAsync(css::SheetLoadDataHolder* aParentData,
                               RawServoCssUrlDataStrong aCssUrl,
                               RawServoMediaListStrong aMediaList,
                               RawServoImportRuleStrong aImportRule) {
||||||| merged common ancestors
void
Gecko_LoadStyleSheetAsync(css::SheetLoadDataHolder* aParentData,
                          RawServoCssUrlDataStrong aCssUrl,
                          RawServoMediaListStrong aMediaList,
                          RawServoImportRuleStrong aImportRule)
{
=======
void Gecko_LoadStyleSheetAsync(SheetLoadDataHolder* aParentData,
                               const StyleCssUrl* aUrl,
                               StyleStrong<RawServoMediaList> aMediaList,
                               StyleStrong<RawServoImportRule> aImportRule) {
  MOZ_ASSERT(aUrl);
>>>>>>> upstream-releases
  RefPtr<SheetLoadDataHolder> loadData = aParentData;
<<<<<<< HEAD
  // The CORS mode in the URLValue is irrelevant here.
  // (CORS_NONE is used for all imported sheets in Load::LoadChildSheet.)
  RefPtr<css::URLValue> urlVal =
      new css::URLValue(aCssUrl.Consume(), CORS_NONE);
||||||| merged common ancestors
  // The CORS mode in the URLValue is irrelevant here.
  // (CORS_NONE is used for all imported sheets in Load::LoadChildSheet.)
  RefPtr<css::URLValue> urlVal =
    new css::URLValue(aCssUrl.Consume(), CORS_NONE);
=======
>>>>>>> upstream-releases
  RefPtr<RawServoMediaList> mediaList = aMediaList.Consume();
  RefPtr<RawServoImportRule> importRule = aImportRule.Consume();
<<<<<<< HEAD
  NS_DispatchToMainThread(NS_NewRunnableFunction(
      __func__,
      [data = std::move(loadData), url = std::move(urlVal),
       media = std::move(mediaList), import = std::move(importRule)]() mutable {
        MOZ_ASSERT(NS_IsMainThread());
        SheetLoadData* d = data->get();
        RefPtr<StyleSheet> sheet = LoadImportSheet(
            d->mLoader, d->mSheet, d, nullptr, url, media.forget());
        Servo_ImportRule_SetSheet(import, sheet);
      }));
}

nsCSSKeyword Gecko_LookupCSSKeyword(const uint8_t* aString, uint32_t aLength) {
  nsDependentCSubstring keyword(reinterpret_cast<const char*>(aString),
                                aLength);
||||||| merged common ancestors
  NS_DispatchToMainThread(NS_NewRunnableFunction(__func__,
                                                 [data = std::move(loadData),
                                                  url = std::move(urlVal),
                                                  media = std::move(mediaList),
                                                  import = std::move(importRule)]() mutable {
    MOZ_ASSERT(NS_IsMainThread());
    SheetLoadData* d = data->get();
    RefPtr<StyleSheet> sheet =
      LoadImportSheet(d->mLoader, d->mSheet, d, nullptr, url, media.forget());
    Servo_ImportRule_SetSheet(import, sheet);
  }));
}

nsCSSKeyword
Gecko_LookupCSSKeyword(const uint8_t* aString, uint32_t aLength)
{
  nsDependentCSubstring keyword(reinterpret_cast<const char*>(aString), aLength);
=======
  NS_DispatchToMainThread(NS_NewRunnableFunction(
      __func__,
      [data = std::move(loadData), url = StyleCssUrl(*aUrl),
       media = std::move(mediaList), import = std::move(importRule)]() mutable {
        MOZ_ASSERT(NS_IsMainThread());
        SheetLoadData* d = data->get();
        RefPtr<StyleSheet> sheet = LoadImportSheet(
            d->mLoader, d->mSheet, d, nullptr, url, media.forget());
        Servo_ImportRule_SetSheet(import, sheet);
      }));
}

nsCSSKeyword Gecko_LookupCSSKeyword(const uint8_t* aString, uint32_t aLength) {
  nsDependentCSubstring keyword(reinterpret_cast<const char*>(aString),
                                aLength);
>>>>>>> upstream-releases
  return nsCSSKeywords::LookupKeyword(keyword);
}

const char* Gecko_CSSKeywordString(nsCSSKeyword aKeyword, uint32_t* aLength) {
  MOZ_ASSERT(NS_IsMainThread());
  MOZ_ASSERT(aLength);
  const nsCString& value = nsCSSKeywords::GetStringValue(aKeyword);
  *aLength = value.Length();
  return value.get();
}

<<<<<<< HEAD
void Gecko_AddPropertyToSet(nsCSSPropertyIDSetBorrowedMut aPropertySet,
                            nsCSSPropertyID aProperty) {
||||||| merged common ancestors
void
Gecko_AddPropertyToSet(nsCSSPropertyIDSetBorrowedMut aPropertySet,
                       nsCSSPropertyID aProperty)
{
=======
void Gecko_AddPropertyToSet(nsCSSPropertyIDSet* aPropertySet,
                            nsCSSPropertyID aProperty) {
>>>>>>> upstream-releases
  aPropertySet->AddProperty(aProperty);
}

<<<<<<< HEAD
NS_IMPL_THREADSAFE_FFI_REFCOUNTING(nsCSSValueSharedList, CSSValueSharedList);

#define STYLE_STRUCT(name)                                             \
                                                                       \
  void Gecko_Construct_Default_nsStyle##name(                          \
      nsStyle##name* ptr, const nsPresContext* pres_context) {         \
    new (ptr) nsStyle##name(pres_context);                             \
  }                                                                    \
                                                                       \
  void Gecko_CopyConstruct_nsStyle##name(nsStyle##name* ptr,           \
                                         const nsStyle##name* other) { \
    new (ptr) nsStyle##name(*other);                                   \
  }                                                                    \
                                                                       \
  void Gecko_Destroy_nsStyle##name(nsStyle##name* ptr) {               \
    ptr->~nsStyle##name();                                             \
  }

void Gecko_RegisterProfilerThread(const char* name) {
||||||| merged common ancestors
NS_IMPL_THREADSAFE_FFI_REFCOUNTING(nsCSSValueSharedList, CSSValueSharedList);

#define STYLE_STRUCT(name)                                                    \
                                                                              \
void                                                                          \
Gecko_Construct_Default_nsStyle##name(nsStyle##name* ptr,                     \
                                      const nsPresContext* pres_context)      \
{                                                                             \
  new (ptr) nsStyle##name(pres_context);                                      \
}                                                                             \
                                                                              \
void                                                                          \
Gecko_CopyConstruct_nsStyle##name(nsStyle##name* ptr,                         \
                                  const nsStyle##name* other)                 \
{                                                                             \
  new (ptr) nsStyle##name(*other);                                            \
}                                                                             \
                                                                              \
void                                                                          \
Gecko_Destroy_nsStyle##name(nsStyle##name* ptr)                               \
{                                                                             \
  ptr->~nsStyle##name();                                                      \
}

void
Gecko_RegisterProfilerThread(const char* name)
{
=======
#define STYLE_STRUCT(name)                                             \
                                                                       \
  void Gecko_Construct_Default_nsStyle##name(nsStyle##name* ptr,       \
                                             const Document* doc) {    \
    new (ptr) nsStyle##name(*doc);                                     \
  }                                                                    \
                                                                       \
  void Gecko_CopyConstruct_nsStyle##name(nsStyle##name* ptr,           \
                                         const nsStyle##name* other) { \
    new (ptr) nsStyle##name(*other);                                   \
  }                                                                    \
                                                                       \
  void Gecko_Destroy_nsStyle##name(nsStyle##name* ptr) {               \
    ptr->~nsStyle##name();                                             \
  }

void Gecko_RegisterProfilerThread(const char* name) {
>>>>>>> upstream-releases
  PROFILER_REGISTER_THREAD(name);
}

<<<<<<< HEAD
void Gecko_UnregisterProfilerThread() { PROFILER_UNREGISTER_THREAD(); }
||||||| merged common ancestors
void
Gecko_UnregisterProfilerThread()
{
  PROFILER_UNREGISTER_THREAD();
}
=======
void Gecko_UnregisterProfilerThread() { PROFILER_UNREGISTER_THREAD(); }

#ifdef MOZ_GECKO_PROFILER
void Gecko_Construct_AutoProfilerLabel(AutoProfilerLabel* aAutoLabel,
                                       JS::ProfilingCategoryPair aCatPair) {
  new (aAutoLabel) AutoProfilerLabel(
      "", nullptr, aCatPair,
      uint32_t(
          js::ProfilingStackFrame::Flags::LABEL_DETERMINED_BY_CATEGORY_PAIR));
}
>>>>>>> upstream-releases

<<<<<<< HEAD
bool Gecko_DocumentRule_UseForPresentation(
    RawGeckoPresContextBorrowed aPresContext, const nsACString* aPattern,
    css::DocumentMatchingFunction aMatchingFunction) {
||||||| merged common ancestors
bool
Gecko_DocumentRule_UseForPresentation(RawGeckoPresContextBorrowed aPresContext,
                                      const nsACString* aPattern,
                                      css::DocumentMatchingFunction aMatchingFunction)
{
=======
void Gecko_Destroy_AutoProfilerLabel(AutoProfilerLabel* aAutoLabel) {
  aAutoLabel->~AutoProfilerLabel();
}
#endif

bool Gecko_DocumentRule_UseForPresentation(
    const Document* aDocument, const nsACString* aPattern,
    DocumentMatchingFunction aMatchingFunction) {
>>>>>>> upstream-releases
  MOZ_ASSERT(NS_IsMainThread());

<<<<<<< HEAD
  nsIDocument* doc = aPresContext->Document();
  nsIURI* docURI = doc->GetDocumentURI();
||||||| merged common ancestors
  nsIDocument *doc = aPresContext->Document();
  nsIURI *docURI = doc->GetDocumentURI();
=======
  nsIURI* docURI = aDocument->GetDocumentURI();
>>>>>>> upstream-releases
  nsAutoCString docURISpec;
  if (docURI) {
    // If GetSpec fails (due to OOM) just skip these URI-specific CSS rules.
    nsresult rv = docURI->GetSpec(docURISpec);
    NS_ENSURE_SUCCESS(rv, false);
  }

  return CSSMozDocumentRule::Match(aDocument, docURI, docURISpec, *aPattern,
                                   aMatchingFunction);
}

void Gecko_SetJemallocThreadLocalArena(bool enabled) {
#if defined(MOZ_MEMORY)
  jemalloc_thread_local_arena(enabled);
#endif
}

#include "nsStyleStructList.h"

#undef STYLE_STRUCT

bool Gecko_ErrorReportingEnabled(const StyleSheet* aSheet,
                                 const Loader* aLoader) {
  return ErrorReporter::ShouldReportErrors(aSheet, aLoader);
}

<<<<<<< HEAD
void Gecko_ReportUnexpectedCSSError(const StyleSheet* aSheet,
                                    const Loader* aLoader, nsIURI* aURI,
                                    const char* message, const char* param,
                                    uint32_t paramLen, const char* prefix,
                                    const char* prefixParam,
                                    uint32_t prefixParamLen, const char* suffix,
                                    const char* source, uint32_t sourceLen,
                                    uint32_t lineNumber, uint32_t colNumber) {
||||||| merged common ancestors
void
Gecko_ReportUnexpectedCSSError(const StyleSheet* aSheet,
                               const Loader* aLoader,
                               nsIURI* aURI,
                               const char* message,
                               const char* param,
                               uint32_t paramLen,
                               const char* prefix,
                               const char* prefixParam,
                               uint32_t prefixParamLen,
                               const char* suffix,
                               const char* source,
                               uint32_t sourceLen,
                               uint32_t lineNumber,
                               uint32_t colNumber)
{
=======
void Gecko_ReportUnexpectedCSSError(
    const StyleSheet* aSheet, const Loader* aLoader, nsIURI* aURI,
    const char* message, const char* param, uint32_t paramLen,
    const char* prefix, const char* prefixParam, uint32_t prefixParamLen,
    const char* suffix, const char* source, uint32_t sourceLen,
    const char* selectors, uint32_t selectorsLen, uint32_t lineNumber,
    uint32_t colNumber) {
>>>>>>> upstream-releases
  MOZ_RELEASE_ASSERT(NS_IsMainThread());

  ErrorReporter reporter(aSheet, aLoader, aURI);

  if (prefix) {
    if (prefixParam) {
      nsDependentCSubstring paramValue(prefixParam, prefixParamLen);
      AutoTArray<nsString, 1> wideParam;
      CopyUTF8toUTF16(paramValue, *wideParam.AppendElement());
      reporter.ReportUnexpectedUnescaped(prefix, wideParam);
    } else {
      reporter.ReportUnexpected(prefix);
    }
  }

  if (param) {
    nsDependentCSubstring paramValue(param, paramLen);
    AutoTArray<nsString, 1> wideParam;
    CopyUTF8toUTF16(paramValue, *wideParam.AppendElement());
    reporter.ReportUnexpectedUnescaped(message, wideParam);
  } else {
    reporter.ReportUnexpected(message);
  }

  if (suffix) {
    reporter.ReportUnexpected(suffix);
  }
  nsDependentCSubstring sourceValue(source, sourceLen);
<<<<<<< HEAD
  reporter.OutputError(lineNumber, colNumber, sourceValue);
}

void Gecko_AddBufferToCrashReport(const void* addr, size_t len) {
  MOZ_ASSERT(NS_IsMainThread());
  nsCOMPtr<nsICrashReporter> cr =
      do_GetService("@mozilla.org/toolkit/crash-reporter;1");
  NS_ENSURE_TRUE_VOID(cr);
  cr->RegisterAppMemory((uint64_t)addr, len);
||||||| merged common ancestors
  reporter.OutputError(lineNumber, colNumber, sourceValue);
}

void
Gecko_AddBufferToCrashReport(const void* addr, size_t len)
{
  MOZ_ASSERT(NS_IsMainThread());
  nsCOMPtr<nsICrashReporter> cr = do_GetService("@mozilla.org/toolkit/crash-reporter;1");
  NS_ENSURE_TRUE_VOID(cr);
  cr->RegisterAppMemory((uint64_t) addr, len);
=======
  nsDependentCSubstring selectorsValue(selectors, selectorsLen);
  reporter.OutputError(lineNumber, colNumber, sourceValue, selectorsValue);
>>>>>>> upstream-releases
}

<<<<<<< HEAD
void Gecko_AnnotateCrashReport(const char* key_str, const char* value_str) {
  MOZ_ASSERT(NS_IsMainThread());
  nsDependentCString key(key_str);
  nsDependentCString value(value_str);
  nsCOMPtr<nsICrashReporter> cr =
      do_GetService("@mozilla.org/toolkit/crash-reporter;1");
  NS_ENSURE_TRUE_VOID(cr);
  cr->AnnotateCrashReport(key, value);
}

void Gecko_ContentList_AppendAll(nsSimpleContentList* aList,
                                 const Element** aElements, size_t aLength) {
||||||| merged common ancestors
void
Gecko_AnnotateCrashReport(const char* key_str, const char* value_str)
{
  MOZ_ASSERT(NS_IsMainThread());
  nsDependentCString key(key_str);
  nsDependentCString value(value_str);
  nsCOMPtr<nsICrashReporter> cr = do_GetService("@mozilla.org/toolkit/crash-reporter;1");
  NS_ENSURE_TRUE_VOID(cr);
  cr->AnnotateCrashReport(key, value);
}

void
Gecko_ContentList_AppendAll(
  nsSimpleContentList* aList,
  const Element** aElements,
  size_t aLength)
{
=======
void Gecko_ContentList_AppendAll(nsSimpleContentList* aList,
                                 const Element** aElements, size_t aLength) {
>>>>>>> upstream-releases
  MOZ_ASSERT(NS_IsMainThread());
  MOZ_ASSERT(aElements);
  MOZ_ASSERT(aLength);
  MOZ_ASSERT(aList);

  aList->SetCapacity(aLength);

  for (size_t i = 0; i < aLength; ++i) {
    aList->AppendElement(const_cast<Element*>(aElements[i]));
  }
}

<<<<<<< HEAD
const nsTArray<Element*>* Gecko_Document_GetElementsWithId(
    const nsIDocument* aDoc, nsAtom* aId) {
||||||| merged common ancestors
const nsTArray<Element*>*
Gecko_Document_GetElementsWithId(const nsIDocument* aDoc, nsAtom* aId)
{
=======
const nsTArray<Element*>* Gecko_Document_GetElementsWithId(const Document* aDoc,
                                                           nsAtom* aId) {
>>>>>>> upstream-releases
  MOZ_ASSERT(aDoc);
  MOZ_ASSERT(aId);

  return aDoc->GetAllElementsForId(nsDependentAtomString(aId));
}

const nsTArray<Element*>* Gecko_ShadowRoot_GetElementsWithId(
    const ShadowRoot* aShadowRoot, nsAtom* aId) {
  MOZ_ASSERT(aShadowRoot);
  MOZ_ASSERT(aId);

  return aShadowRoot->GetAllElementsForId(nsDependentAtomString(aId));
}

bool Gecko_GetBoolPrefValue(const char* aPrefName) {
  MOZ_ASSERT(NS_IsMainThread());
  return Preferences::GetBool(aPrefName);
}

bool Gecko_IsInServoTraversal() { return ServoStyleSet::IsInServoTraversal(); }

bool Gecko_IsMainThread() { return NS_IsMainThread(); }

<<<<<<< HEAD
const nsAttrValue* Gecko_GetSVGAnimatedClass(RawGeckoElementBorrowed aElement) {
||||||| merged common ancestors
const nsAttrValue*
Gecko_GetSVGAnimatedClass(RawGeckoElementBorrowed aElement)
{
=======
const nsAttrValue* Gecko_GetSVGAnimatedClass(const Element* aElement) {
>>>>>>> upstream-releases
  MOZ_ASSERT(aElement->IsSVGElement());
  return static_cast<const SVGElement*>(aElement)->GetAnimatedClassName();
}

bool Gecko_AssertClassAttrValueIsSane(const nsAttrValue* aValue) {
  MOZ_ASSERT(aValue->Type() == nsAttrValue::eAtom ||
             aValue->Type() == nsAttrValue::eString ||
             aValue->Type() == nsAttrValue::eAtomArray);
  MOZ_ASSERT_IF(
      aValue->Type() == nsAttrValue::eString,
      nsContentUtils::TrimWhitespace<nsContentUtils::IsHTMLWhitespace>(
          aValue->GetStringValue())
          .IsEmpty());
  return true;
}

void Gecko_LoadData_DeregisterLoad(const StyleLoadData* aData) {
  MOZ_ASSERT(aData->load_id != 0);
  ImageLoader::DeregisterCSSImageFromAllLoaders(*aData);
}

void Gecko_PrintfStderr(const nsCString* aStr) {
  printf_stderr("%s", aStr->get());
}
