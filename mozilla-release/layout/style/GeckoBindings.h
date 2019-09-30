/* -*- Mode: C++; tab-width: 8; indent-tabs-mode: nil; c-basic-offset: 2 -*- */
/* vim: set ts=8 sts=2 et sw=2 tw=80: */
/* This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/. */

/* FFI functions for Servo to call into Gecko */

#ifndef mozilla_GeckoBindings_h
#define mozilla_GeckoBindings_h

#include <stdint.h>

#include "mozilla/ServoTypes.h"
#include "mozilla/ServoBindingTypes.h"
#include "mozilla/css/DocumentMatchingFunction.h"
#include "mozilla/css/SheetLoadData.h"
#include "mozilla/EffectCompositor.h"
#include "mozilla/ComputedTimingFunction.h"
#include "mozilla/PreferenceSheet.h"
#include "nsStyleStruct.h"

class nsAtom;
class nsIURI;
class nsSimpleContentList;
struct nsFont;

namespace mozilla {
<<<<<<< HEAD
class ComputedStyle;
class SeenPtrs;
class ServoElementSnapshot;
class ServoElementSnapshotTable;
class SharedFontList;
class StyleSheet;
enum class CSSPseudoElementType : uint8_t;
enum class PointerCapabilities : uint8_t;
enum class UpdateAnimationsTasks : uint8_t;
struct FontFamilyName;
struct Keyframe;

namespace css {
class LoaderReusableStyleSheets;
||||||| merged common ancestors
  class ComputedStyle;
  class SeenPtrs;
  class ServoElementSnapshot;
  class ServoElementSnapshotTable;
  class SharedFontList;
  class StyleSheet;
  enum class CSSPseudoElementType : uint8_t;
  enum class PointerCapabilities : uint8_t;
  enum class UpdateAnimationsTasks : uint8_t;
  struct FontFamilyName;
  struct Keyframe;

  namespace css {
    class LoaderReusableStyleSheets;
  }
=======
class ComputedStyle;
class SeenPtrs;
class ServoElementSnapshot;
class ServoElementSnapshotTable;
class SharedFontList;
class StyleSheet;
enum class PseudoStyleType : uint8_t;
enum class PointerCapabilities : uint8_t;
enum class UpdateAnimationsTasks : uint8_t;
struct FontFamilyName;
struct Keyframe;

namespace css {
class LoaderReusableStyleSheets;
>>>>>>> upstream-releases
}
}  // namespace mozilla

#ifdef NIGHTLY_BUILD
const bool GECKO_IS_NIGHTLY = true;
#else
const bool GECKO_IS_NIGHTLY = false;
#endif

#define NS_DECL_THREADSAFE_FFI_REFCOUNTING(class_, name_)  \
  void Gecko_AddRef##name_##ArbitraryThread(class_* aPtr); \
  void Gecko_Release##name_##ArbitraryThread(class_* aPtr);
#define NS_IMPL_THREADSAFE_FFI_REFCOUNTING(class_, name_)                      \
  static_assert(class_::HasThreadSafeRefCnt::value,                            \
                "NS_DECL_THREADSAFE_FFI_REFCOUNTING can only be used with "    \
                "classes that have thread-safe refcounting");                  \
  void Gecko_AddRef##name_##ArbitraryThread(class_* aPtr) { NS_ADDREF(aPtr); } \
  void Gecko_Release##name_##ArbitraryThread(class_* aPtr) { NS_RELEASE(aPtr); }

extern "C" {

NS_DECL_THREADSAFE_FFI_REFCOUNTING(nsIURI, nsIURI);

<<<<<<< HEAD
void Gecko_Snapshot_DebugListAttributes(const mozilla::ServoElementSnapshot*,
                                        nsCString*);
||||||| merged common ancestors
void Gecko_Snapshot_DebugListAttributes(
  const mozilla::ServoElementSnapshot*,
  nsCString*);
=======
// Debugging stuff.
void Gecko_Element_DebugListAttributes(const mozilla::dom::Element*,
                                       nsCString*);
>>>>>>> upstream-releases

<<<<<<< HEAD
bool Gecko_IsSignificantChild(RawGeckoNodeBorrowed node,
                              bool whitespace_is_significant);
||||||| merged common ancestors
bool Gecko_IsSignificantChild(
  RawGeckoNodeBorrowed node,
  bool whitespace_is_significant);
=======
void Gecko_Snapshot_DebugListAttributes(const mozilla::ServoElementSnapshot*,
                                        nsCString*);
>>>>>>> upstream-releases

bool Gecko_IsSignificantChild(const nsINode*, bool whitespace_is_significant);

<<<<<<< HEAD
RawGeckoNodeBorrowedOrNull Gecko_GetFlattenedTreeParentNode(
    RawGeckoNodeBorrowed node);
||||||| merged common ancestors
RawGeckoNodeBorrowedOrNull Gecko_GetFlattenedTreeParentNode(
  RawGeckoNodeBorrowed node);
=======
const nsINode* Gecko_GetLastChild(const nsINode*);
const nsINode* Gecko_GetPreviousSibling(const nsINode*);
>>>>>>> upstream-releases

<<<<<<< HEAD
RawGeckoElementBorrowedOrNull Gecko_GetBeforeOrAfterPseudo(
    RawGeckoElementBorrowed element, bool is_before);
||||||| merged common ancestors
RawGeckoElementBorrowedOrNull Gecko_GetBeforeOrAfterPseudo(
  RawGeckoElementBorrowed element,
  bool is_before);
=======
const nsINode* Gecko_GetFlattenedTreeParentNode(const nsINode*);
const mozilla::dom::Element* Gecko_GetBeforeOrAfterPseudo(
    const mozilla::dom::Element*, bool is_before);
const mozilla::dom::Element* Gecko_GetMarkerPseudo(
    const mozilla::dom::Element*);
>>>>>>> upstream-releases

nsTArray<nsIContent*>* Gecko_GetAnonymousContentForElement(
<<<<<<< HEAD
    RawGeckoElementBorrowed element);
||||||| merged common ancestors
  RawGeckoElementBorrowed element);
=======
    const mozilla::dom::Element*);
void Gecko_DestroyAnonymousContentList(nsTArray<nsIContent*>* anon_content);
>>>>>>> upstream-releases

const nsTArray<RefPtr<nsINode>>* Gecko_GetAssignedNodes(
<<<<<<< HEAD
    RawGeckoElementBorrowed element);
||||||| merged common ancestors
  RawGeckoElementBorrowed element);
=======
    const mozilla::dom::Element*);
>>>>>>> upstream-releases

void Gecko_ComputedStyle_Init(mozilla::ComputedStyle* context,
                              const ServoComputedData* values,
                              mozilla::PseudoStyleType pseudo_type);

<<<<<<< HEAD
void Gecko_ComputedStyle_Init(mozilla::ComputedStyle* context,
                              RawGeckoPresContextBorrowed pres_context,
                              ServoComputedDataBorrowed values,
                              mozilla::CSSPseudoElementType pseudo_type,
                              nsAtom* pseudo_tag);

void Gecko_ComputedStyle_Destroy(mozilla::ComputedStyle* context);
||||||| merged common ancestors
void Gecko_ComputedStyle_Init(
  mozilla::ComputedStyle* context,
  RawGeckoPresContextBorrowed pres_context,
  ServoComputedDataBorrowed values,
  mozilla::CSSPseudoElementType pseudo_type,
  nsAtom* pseudo_tag);

void Gecko_ComputedStyle_Destroy(
  mozilla::ComputedStyle* context);
=======
void Gecko_ComputedStyle_Destroy(mozilla::ComputedStyle* context);
>>>>>>> upstream-releases

// By default, Servo walks the DOM by traversing the siblings of the DOM-view
// first child. This generally works, but misses anonymous children, which we
// want to traverse during styling. To support these cases, we create an
// optional stack-allocated iterator in aIterator for nodes that need it.
<<<<<<< HEAD
void Gecko_ConstructStyleChildrenIterator(
    RawGeckoElementBorrowed aElement,
    RawGeckoStyleChildrenIteratorBorrowedMut aIterator);
||||||| merged common ancestors
void Gecko_ConstructStyleChildrenIterator(
  RawGeckoElementBorrowed aElement,
  RawGeckoStyleChildrenIteratorBorrowedMut aIterator);
=======
void Gecko_ConstructStyleChildrenIterator(const mozilla::dom::Element*,
                                          mozilla::dom::StyleChildrenIterator*);
>>>>>>> upstream-releases

<<<<<<< HEAD
void Gecko_DestroyStyleChildrenIterator(
    RawGeckoStyleChildrenIteratorBorrowedMut aIterator);
||||||| merged common ancestors
void Gecko_DestroyStyleChildrenIterator(
  RawGeckoStyleChildrenIteratorBorrowedMut aIterator);
=======
void Gecko_DestroyStyleChildrenIterator(mozilla::dom::StyleChildrenIterator*);
>>>>>>> upstream-releases

<<<<<<< HEAD
RawGeckoNodeBorrowedOrNull Gecko_GetNextStyleChild(
    RawGeckoStyleChildrenIteratorBorrowedMut it);
||||||| merged common ancestors
RawGeckoNodeBorrowedOrNull Gecko_GetNextStyleChild(
  RawGeckoStyleChildrenIteratorBorrowedMut it);
=======
const nsINode* Gecko_GetNextStyleChild(mozilla::dom::StyleChildrenIterator*);
>>>>>>> upstream-releases

NS_DECL_THREADSAFE_FFI_REFCOUNTING(mozilla::css::SheetLoadDataHolder,
                                   SheetLoadDataHolder);

void Gecko_StyleSheet_FinishAsyncParse(
<<<<<<< HEAD
    mozilla::css::SheetLoadDataHolder* data,
    RawServoStyleSheetContentsStrong sheet_contents,
    StyleUseCountersOwnedOrNull use_counters);
||||||| merged common ancestors
  mozilla::css::SheetLoadDataHolder* data,
  RawServoStyleSheetContentsStrong sheet_contents,
  StyleUseCountersOwnedOrNull use_counters);
=======
    mozilla::css::SheetLoadDataHolder* data,
    mozilla::StyleStrong<RawServoStyleSheetContents> sheet_contents,
    mozilla::StyleOwnedOrNull<StyleUseCounters> use_counters);
>>>>>>> upstream-releases

mozilla::StyleSheet* Gecko_LoadStyleSheet(
<<<<<<< HEAD
    mozilla::css::Loader* loader, mozilla::StyleSheet* parent,
    mozilla::css::SheetLoadData* parent_load_data,
    mozilla::css::LoaderReusableStyleSheets* reusable_sheets,
    RawServoCssUrlDataStrong url, RawServoMediaListStrong media_list);

void Gecko_LoadStyleSheetAsync(mozilla::css::SheetLoadDataHolder* parent_data,
                               RawServoCssUrlDataStrong url,
                               RawServoMediaListStrong media_list,
                               RawServoImportRuleStrong import_rule);
||||||| merged common ancestors
  mozilla::css::Loader* loader,
  mozilla::StyleSheet* parent,
  mozilla::css::SheetLoadData* parent_load_data,
  mozilla::css::LoaderReusableStyleSheets* reusable_sheets,
  RawServoCssUrlDataStrong url,
  RawServoMediaListStrong media_list);

void Gecko_LoadStyleSheetAsync(
  mozilla::css::SheetLoadDataHolder* parent_data,
  RawServoCssUrlDataStrong url,
  RawServoMediaListStrong media_list,
  RawServoImportRuleStrong import_rule);
=======
    mozilla::css::Loader* loader, mozilla::StyleSheet* parent,
    mozilla::css::SheetLoadData* parent_load_data,
    mozilla::css::LoaderReusableStyleSheets* reusable_sheets,
    const mozilla::StyleCssUrl* url,
    mozilla::StyleStrong<RawServoMediaList> media_list);

void Gecko_LoadStyleSheetAsync(mozilla::css::SheetLoadDataHolder* parent_data,
                               const mozilla::StyleCssUrl* url,
                               mozilla::StyleStrong<RawServoMediaList>,
                               mozilla::StyleStrong<RawServoImportRule>);
>>>>>>> upstream-releases

// Selector Matching.
uint64_t Gecko_ElementState(const mozilla::dom::Element*);
bool Gecko_IsRootElement(const mozilla::dom::Element*);

bool Gecko_MatchLang(const mozilla::dom::Element*, nsAtom* override_lang,
                     bool has_override_lang, const char16_t* value);

<<<<<<< HEAD
bool Gecko_MatchLang(RawGeckoElementBorrowed element, nsAtom* override_lang,
                     bool has_override_lang, const char16_t* value);
||||||| merged common ancestors
bool Gecko_MatchLang(
  RawGeckoElementBorrowed element,
  nsAtom* override_lang,
  bool has_override_lang,
  const char16_t* value);
=======
nsAtom* Gecko_GetXMLLangValue(const mozilla::dom::Element*);
>>>>>>> upstream-releases

mozilla::dom::Document::DocumentTheme Gecko_GetDocumentLWTheme(
    const mozilla::dom::Document*);

<<<<<<< HEAD
nsIDocument::DocumentTheme Gecko_GetDocumentLWTheme(
    const nsIDocument* aDocument);
||||||| merged common ancestors
nsIDocument::DocumentTheme Gecko_GetDocumentLWTheme(
  const nsIDocument* aDocument);
=======
const mozilla::PreferenceSheet::Prefs* Gecko_GetPrefSheetPrefs(
    const mozilla::dom::Document*);
>>>>>>> upstream-releases

bool Gecko_IsTableBorderNonzero(const mozilla::dom::Element* element);
bool Gecko_IsBrowserFrame(const mozilla::dom::Element* element);

// Attributes.
#define SERVO_DECLARE_ELEMENT_ATTR_MATCHING_FUNCTIONS(prefix_, implementor_)   \
  nsAtom* prefix_##LangValue(implementor_ element);                            \
  bool prefix_##HasAttr(implementor_ element, nsAtom* ns, nsAtom* name);       \
  bool prefix_##AttrEquals(implementor_ element, nsAtom* ns, nsAtom* name,     \
                           nsAtom* str, bool ignoreCase);                      \
  bool prefix_##AttrDashEquals(implementor_ element, nsAtom* ns, nsAtom* name, \
                               nsAtom* str, bool ignore_case);                 \
  bool prefix_##AttrIncludes(implementor_ element, nsAtom* ns, nsAtom* name,   \
                             nsAtom* str, bool ignore_case);                   \
  bool prefix_##AttrHasSubstring(implementor_ element, nsAtom* ns,             \
                                 nsAtom* name, nsAtom* str, bool ignore_case); \
  bool prefix_##AttrHasPrefix(implementor_ element, nsAtom* ns, nsAtom* name,  \
                              nsAtom* str, bool ignore_case);                  \
  bool prefix_##AttrHasSuffix(implementor_ element, nsAtom* ns, nsAtom* name,  \
                              nsAtom* str, bool ignore_case);

bool Gecko_AssertClassAttrValueIsSane(const nsAttrValue*);
const nsAttrValue* Gecko_GetSVGAnimatedClass(const mozilla::dom::Element*);

SERVO_DECLARE_ELEMENT_ATTR_MATCHING_FUNCTIONS(Gecko_,
                                              const mozilla::dom::Element*)

SERVO_DECLARE_ELEMENT_ATTR_MATCHING_FUNCTIONS(
    Gecko_Snapshot, const mozilla::ServoElementSnapshot*)

#undef SERVO_DECLARE_ELEMENT_ATTR_MATCHING_FUNCTIONS

// Style attributes.
<<<<<<< HEAD
RawServoDeclarationBlockStrongBorrowedOrNull Gecko_GetStyleAttrDeclarationBlock(
    RawGeckoElementBorrowed element);
||||||| merged common ancestors
RawServoDeclarationBlockStrongBorrowedOrNull Gecko_GetStyleAttrDeclarationBlock(
  RawGeckoElementBorrowed element);
=======
const mozilla::StyleStrong<RawServoDeclarationBlock>*
Gecko_GetStyleAttrDeclarationBlock(const mozilla::dom::Element* element);
>>>>>>> upstream-releases

void Gecko_UnsetDirtyStyleAttr(const mozilla::dom::Element* element);

const mozilla::StyleStrong<RawServoDeclarationBlock>*
Gecko_GetHTMLPresentationAttrDeclarationBlock(
    const mozilla::dom::Element* element);

const mozilla::StyleStrong<RawServoDeclarationBlock>*
Gecko_GetExtraContentStyleDeclarations(const mozilla::dom::Element* element);

const mozilla::StyleStrong<RawServoDeclarationBlock>*
Gecko_GetUnvisitedLinkAttrDeclarationBlock(
    const mozilla::dom::Element* element);

const mozilla::StyleStrong<RawServoDeclarationBlock>*
Gecko_GetVisitedLinkAttrDeclarationBlock(const mozilla::dom::Element* element);

const mozilla::StyleStrong<RawServoDeclarationBlock>*
Gecko_GetActiveLinkAttrDeclarationBlock(const mozilla::dom::Element* element);

// Visited handling.

// Returns whether visited styles are enabled for a given document.
bool Gecko_VisitedStylesEnabled(const mozilla::dom::Document*);

// Animations
bool Gecko_GetAnimationRule(
<<<<<<< HEAD
    RawGeckoElementBorrowed aElementOrPseudo,
    mozilla::EffectCompositor::CascadeLevel aCascadeLevel,
    RawServoAnimationValueMapBorrowedMut aAnimationValues);
||||||| merged common ancestors
  RawGeckoElementBorrowed aElementOrPseudo,
  mozilla::EffectCompositor::CascadeLevel aCascadeLevel,
  RawServoAnimationValueMapBorrowedMut aAnimationValues);
=======
    const mozilla::dom::Element* aElementOrPseudo,
    mozilla::EffectCompositor::CascadeLevel aCascadeLevel,
    RawServoAnimationValueMap* aAnimationValues);
>>>>>>> upstream-releases

<<<<<<< HEAD
bool Gecko_StyleAnimationsEquals(RawGeckoStyleAnimationListBorrowed,
                                 RawGeckoStyleAnimationListBorrowed);
||||||| merged common ancestors
bool Gecko_StyleAnimationsEquals(
  RawGeckoStyleAnimationListBorrowed,
  RawGeckoStyleAnimationListBorrowed);
=======
bool Gecko_StyleAnimationsEquals(
    const nsStyleAutoArray<mozilla::StyleAnimation>*,
    const nsStyleAutoArray<mozilla::StyleAnimation>*);
>>>>>>> upstream-releases

<<<<<<< HEAD
void Gecko_CopyAnimationNames(RawGeckoStyleAnimationListBorrowedMut aDest,
                              RawGeckoStyleAnimationListBorrowed aSrc);
||||||| merged common ancestors
void Gecko_CopyAnimationNames(
  RawGeckoStyleAnimationListBorrowedMut aDest,
  RawGeckoStyleAnimationListBorrowed aSrc);
=======
void Gecko_CopyAnimationNames(
    nsStyleAutoArray<mozilla::StyleAnimation>* aDest,
    const nsStyleAutoArray<mozilla::StyleAnimation>* aSrc);
>>>>>>> upstream-releases

// This function takes an already addrefed nsAtom
<<<<<<< HEAD
void Gecko_SetAnimationName(mozilla::StyleAnimation* aStyleAnimation,
                            nsAtom* aAtom);

void Gecko_UpdateAnimations(RawGeckoElementBorrowed aElementOrPseudo,
                            ComputedStyleBorrowedOrNull aOldComputedValues,
                            ComputedStyleBorrowedOrNull aComputedValues,
                            mozilla::UpdateAnimationsTasks aTasks);

size_t Gecko_GetAnimationEffectCount(RawGeckoElementBorrowed aElementOrPseudo);
bool Gecko_ElementHasAnimations(RawGeckoElementBorrowed aElementOrPseudo);
bool Gecko_ElementHasCSSAnimations(RawGeckoElementBorrowed aElementOrPseudo);
bool Gecko_ElementHasCSSTransitions(RawGeckoElementBorrowed aElementOrPseudo);
size_t Gecko_ElementTransitions_Length(
    RawGeckoElementBorrowed aElementOrPseudo);
||||||| merged common ancestors
void Gecko_SetAnimationName(
  mozilla::StyleAnimation* aStyleAnimation,
  nsAtom* aAtom);

void Gecko_UpdateAnimations(
  RawGeckoElementBorrowed aElementOrPseudo,
  ComputedStyleBorrowedOrNull aOldComputedValues,
  ComputedStyleBorrowedOrNull aComputedValues,
  mozilla::UpdateAnimationsTasks aTasks);

size_t Gecko_GetAnimationEffectCount(RawGeckoElementBorrowed aElementOrPseudo);
bool Gecko_ElementHasAnimations(RawGeckoElementBorrowed aElementOrPseudo);
bool Gecko_ElementHasCSSAnimations(RawGeckoElementBorrowed aElementOrPseudo);
bool Gecko_ElementHasCSSTransitions(RawGeckoElementBorrowed aElementOrPseudo);
size_t Gecko_ElementTransitions_Length(RawGeckoElementBorrowed aElementOrPseudo);
=======
void Gecko_SetAnimationName(mozilla::StyleAnimation* aStyleAnimation,
                            nsAtom* aAtom);

void Gecko_UpdateAnimations(const mozilla::dom::Element* aElementOrPseudo,
                            const mozilla::ComputedStyle* aOldComputedValues,
                            const mozilla::ComputedStyle* aComputedValues,
                            mozilla::UpdateAnimationsTasks aTasks);

size_t Gecko_GetAnimationEffectCount(
    const mozilla::dom::Element* aElementOrPseudo);
bool Gecko_ElementHasAnimations(const mozilla::dom::Element* aElementOrPseudo);
bool Gecko_ElementHasCSSAnimations(
    const mozilla::dom::Element* aElementOrPseudo);
bool Gecko_ElementHasCSSTransitions(
    const mozilla::dom::Element* aElementOrPseudo);
bool Gecko_ElementHasWebAnimations(
    const mozilla::dom::Element* aElementOrPseudo);
size_t Gecko_ElementTransitions_Length(
    const mozilla::dom::Element* aElementOrPseudo);
>>>>>>> upstream-releases

nsCSSPropertyID Gecko_ElementTransitions_PropertyAt(
<<<<<<< HEAD
    RawGeckoElementBorrowed aElementOrPseudo, size_t aIndex);
||||||| merged common ancestors
  RawGeckoElementBorrowed aElementOrPseudo,
  size_t aIndex);
=======
    const mozilla::dom::Element* aElementOrPseudo, size_t aIndex);
>>>>>>> upstream-releases

<<<<<<< HEAD
RawServoAnimationValueBorrowedOrNull Gecko_ElementTransitions_EndValueAt(
    RawGeckoElementBorrowed aElementOrPseudo, size_t aIndex);
||||||| merged common ancestors
RawServoAnimationValueBorrowedOrNull Gecko_ElementTransitions_EndValueAt(
  RawGeckoElementBorrowed aElementOrPseudo,
  size_t aIndex);
=======
const RawServoAnimationValue* Gecko_ElementTransitions_EndValueAt(
    const mozilla::dom::Element* aElementOrPseudo, size_t aIndex);
>>>>>>> upstream-releases

<<<<<<< HEAD
double Gecko_GetProgressFromComputedTiming(
    RawGeckoComputedTimingBorrowed aComputedTiming);
||||||| merged common ancestors
double Gecko_GetProgressFromComputedTiming(
  RawGeckoComputedTimingBorrowed aComputedTiming);
=======
double Gecko_GetProgressFromComputedTiming(const mozilla::ComputedTiming*);
>>>>>>> upstream-releases

double Gecko_GetPositionInSegment(
<<<<<<< HEAD
    RawGeckoAnimationPropertySegmentBorrowed aSegment, double aProgress,
    mozilla::ComputedTimingFunction::BeforeFlag aBeforeFlag);
||||||| merged common ancestors
  RawGeckoAnimationPropertySegmentBorrowed aSegment,
  double aProgress,
  mozilla::ComputedTimingFunction::BeforeFlag aBeforeFlag);
=======
    const mozilla::AnimationPropertySegment*, double aProgress,
    mozilla::ComputedTimingFunction::BeforeFlag aBeforeFlag);
>>>>>>> upstream-releases

// Get servo's AnimationValue for |aProperty| from the cached base style
// |aBaseStyles|.
// |aBaseStyles| is nsRefPtrHashtable<nsUint32HashKey, RawServoAnimationValue>.
// We use RawServoAnimationValueTableBorrowed to avoid exposing
// nsRefPtrHashtable in FFI.
<<<<<<< HEAD
RawServoAnimationValueBorrowedOrNull Gecko_AnimationGetBaseStyle(
    RawServoAnimationValueTableBorrowed aBaseStyles, nsCSSPropertyID aProperty);
||||||| merged common ancestors
RawServoAnimationValueBorrowedOrNull Gecko_AnimationGetBaseStyle(
  RawServoAnimationValueTableBorrowed aBaseStyles,
  nsCSSPropertyID aProperty);
=======
const RawServoAnimationValue* Gecko_AnimationGetBaseStyle(
    const RawServoAnimationValueTable* aBaseStyles, nsCSSPropertyID aProperty);
>>>>>>> upstream-releases

void Gecko_StyleTransition_SetUnsupportedProperty(
    mozilla::StyleTransition* aTransition, nsAtom* aAtom);

// Atoms.
nsAtom* Gecko_Atomize(const char* aString, uint32_t aLength);
nsAtom* Gecko_Atomize16(const nsAString* aString);
void Gecko_AddRefAtom(nsAtom* aAtom);
void Gecko_ReleaseAtom(nsAtom* aAtom);

// Font style
void Gecko_CopyFontFamilyFrom(nsFont* dst, const nsFont* src);

void Gecko_nsTArray_FontFamilyName_AppendNamed(
<<<<<<< HEAD
    nsTArray<mozilla::FontFamilyName>* aNames, nsAtom* aName, bool aQuoted);
||||||| merged common ancestors
  nsTArray<mozilla::FontFamilyName>* aNames,
  nsAtom* aName,
  bool aQuoted);
=======
    nsTArray<mozilla::FontFamilyName>* aNames, nsAtom* aName,
    mozilla::StyleFontFamilyNameSyntax);
>>>>>>> upstream-releases

void Gecko_nsTArray_FontFamilyName_AppendGeneric(
<<<<<<< HEAD
    nsTArray<mozilla::FontFamilyName>* aNames, mozilla::FontFamilyType aType);
||||||| merged common ancestors
  nsTArray<mozilla::FontFamilyName>* aNames,
  mozilla::FontFamilyType aType);
=======
    nsTArray<mozilla::FontFamilyName>* aNames, mozilla::StyleGenericFontFamily);
>>>>>>> upstream-releases

// Returns an already-AddRefed SharedFontList with an empty mNames array.
mozilla::SharedFontList* Gecko_SharedFontList_Create();

size_t Gecko_SharedFontList_SizeOfIncludingThis(
    mozilla::SharedFontList* fontlist);

size_t Gecko_SharedFontList_SizeOfIncludingThisIfUnshared(
    mozilla::SharedFontList* fontlist);

NS_DECL_THREADSAFE_FFI_REFCOUNTING(mozilla::SharedFontList, SharedFontList);

// will not run destructors on dst, give it uninitialized memory
// font_id is LookAndFeel::FontID
<<<<<<< HEAD
void Gecko_nsFont_InitSystem(nsFont* dst, int32_t font_id,
                             const nsStyleFont* font,
                             RawGeckoPresContextBorrowed pres_context);
||||||| merged common ancestors
void Gecko_nsFont_InitSystem(
  nsFont* dst,
  int32_t font_id,
  const nsStyleFont* font,
  RawGeckoPresContextBorrowed pres_context);
=======
void Gecko_nsFont_InitSystem(nsFont* dst, int32_t font_id,
                             const nsStyleFont* font,
                             const mozilla::dom::Document*);
>>>>>>> upstream-releases

void Gecko_nsFont_Destroy(nsFont* dst);

// The gfxFontFeatureValueSet returned from this function has zero reference.
gfxFontFeatureValueSet* Gecko_ConstructFontFeatureValueSet();

nsTArray<unsigned int>* Gecko_AppendFeatureValueHashEntry(
<<<<<<< HEAD
    gfxFontFeatureValueSet* value_set, nsAtom* family, uint32_t alternate,
    nsAtom* name);

void Gecko_nsFont_SetFontFeatureValuesLookup(
    nsFont* font, const RawGeckoPresContext* pres_context);

void Gecko_nsFont_ResetFontFeatureValuesLookup(nsFont* font);
||||||| merged common ancestors
  gfxFontFeatureValueSet* value_set,
  nsAtom* family,
  uint32_t alternate,
  nsAtom* name);

void Gecko_nsFont_SetFontFeatureValuesLookup(
  nsFont* font,
  const RawGeckoPresContext* pres_context);

void Gecko_nsFont_ResetFontFeatureValuesLookup(nsFont* font);
=======
    gfxFontFeatureValueSet* value_set, nsAtom* family, uint32_t alternate,
    nsAtom* name);
>>>>>>> upstream-releases

// Font variant alternates
void Gecko_ClearAlternateValues(nsFont* font, size_t length);

void Gecko_AppendAlternateValues(nsFont* font, uint32_t alternate_name,
                                 nsAtom* atom);

void Gecko_CopyAlternateValuesFrom(nsFont* dest, const nsFont* src);

// Visibility style
void Gecko_SetImageOrientation(nsStyleVisibility* aVisibility,
                               uint8_t aOrientation, bool aFlip);

void Gecko_SetImageOrientationAsFromImage(nsStyleVisibility* aVisibility);

void Gecko_CopyImageOrientationFrom(nsStyleVisibility* aDst,
                                    const nsStyleVisibility* aSrc);

// Counter style.
// This function takes an already addrefed nsAtom
<<<<<<< HEAD
void Gecko_SetCounterStyleToName(mozilla::CounterStylePtr* ptr, nsAtom* name,
                                 RawGeckoPresContextBorrowed pres_context);
||||||| merged common ancestors
void Gecko_SetCounterStyleToName(
  mozilla::CounterStylePtr* ptr,
  nsAtom* name,
  RawGeckoPresContextBorrowed pres_context);
=======
void Gecko_SetCounterStyleToName(mozilla::CounterStylePtr* ptr, nsAtom* name);
>>>>>>> upstream-releases

void Gecko_SetCounterStyleToSymbols(mozilla::CounterStylePtr* ptr,
                                    uint8_t symbols_type,
                                    nsACString const* const* symbols,
                                    uint32_t symbols_count);

void Gecko_SetCounterStyleToString(mozilla::CounterStylePtr* ptr,
                                   const nsACString* symbol);

void Gecko_CopyCounterStyle(mozilla::CounterStylePtr* dst,
                            const mozilla::CounterStylePtr* src);

nsAtom* Gecko_CounterStyle_GetName(const mozilla::CounterStylePtr* ptr);

const mozilla::AnonymousCounterStyle* Gecko_CounterStyle_GetAnonymous(
    const mozilla::CounterStylePtr* ptr);

// background-image style.
void Gecko_SetNullImageValue(nsStyleImage* image);
<<<<<<< HEAD
void Gecko_SetGradientImageValue(nsStyleImage* image,
                                 nsStyleGradient* gradient);
||||||| merged common ancestors
void Gecko_SetGradientImageValue(nsStyleImage* image, nsStyleGradient* gradient);
=======
>>>>>>> upstream-releases

<<<<<<< HEAD
void Gecko_SetLayerImageImageValue(nsStyleImage* image,
                                   mozilla::css::URLValue* image_value);
||||||| merged common ancestors
void Gecko_SetLayerImageImageValue(
  nsStyleImage* image,
  mozilla::css::URLValue* image_value);
=======
// NOTE: Takes ownership of the gradient.
void Gecko_SetGradientImageValue(nsStyleImage*, mozilla::StyleGradient*);

void Gecko_SetLayerImageImageValue(nsStyleImage* image,
                                   const mozilla::StyleComputedImageUrl* url);
>>>>>>> upstream-releases

void Gecko_SetImageElement(nsStyleImage* image, nsAtom* atom);
void Gecko_CopyImageValueFrom(nsStyleImage* image, const nsStyleImage* other);
void Gecko_InitializeImageCropRect(nsStyleImage* image);

<<<<<<< HEAD
nsStyleGradient* Gecko_CreateGradient(uint8_t shape, uint8_t size,
                                      bool repeating, bool legacy_syntax,
                                      bool moz_legacy_syntax, uint32_t stops);

||||||| merged common ancestors
nsStyleGradient* Gecko_CreateGradient(
  uint8_t shape,
  uint8_t size,
  bool repeating,
  bool legacy_syntax,
  bool moz_legacy_syntax,
  uint32_t stops);

=======
>>>>>>> upstream-releases
const nsStyleImageRequest* Gecko_GetImageRequest(const nsStyleImage* image);
nsAtom* Gecko_GetImageElement(const nsStyleImage* image);

// list-style-image style.
void Gecko_SetListStyleImageNone(nsStyleList* style_struct);

<<<<<<< HEAD
void Gecko_SetListStyleImageImageValue(nsStyleList* style_struct,
                                       mozilla::css::URLValue* aImageValue);
||||||| merged common ancestors
void Gecko_SetListStyleImageImageValue(
  nsStyleList* style_struct,
  mozilla::css::URLValue* aImageValue);
=======
void Gecko_SetListStyleImageImageValue(
    nsStyleList* style_struct, const mozilla::StyleComputedImageUrl* url);
>>>>>>> upstream-releases

void Gecko_CopyListStyleImageFrom(nsStyleList* dest, const nsStyleList* src);

// cursor style.
void Gecko_SetCursorArrayLength(nsStyleUI* ui, size_t len);

<<<<<<< HEAD
void Gecko_SetCursorImageValue(nsCursorImage* aCursor,
                               mozilla::css::URLValue* aImageValue);
||||||| merged common ancestors
void Gecko_SetCursorImageValue(
  nsCursorImage* aCursor,
  mozilla::css::URLValue* aImageValue);
=======
void Gecko_SetCursorImageValue(nsCursorImage* aCursor,
                               const mozilla::StyleComputedImageUrl* url);
>>>>>>> upstream-releases

void Gecko_CopyCursorArrayFrom(nsStyleUI* dest, const nsStyleUI* src);

<<<<<<< HEAD
void Gecko_SetContentDataImageValue(nsStyleContentData* aList,
                                    mozilla::css::URLValue* aImageValue);
||||||| merged common ancestors
void Gecko_SetContentDataImageValue(
  nsStyleContentData* aList,
  mozilla::css::URLValue* aImageValue);
=======
void Gecko_SetContentDataImageValue(nsStyleContentData* aList,
                                    const mozilla::StyleComputedImageUrl* url);
>>>>>>> upstream-releases

nsStyleContentData::CounterFunction* Gecko_SetCounterFunction(
    nsStyleContentData* content_data, mozilla::StyleContentType);

// Dirtiness tracking.
void Gecko_SetNodeFlags(const nsINode* node, uint32_t flags);
void Gecko_UnsetNodeFlags(const nsINode* node, uint32_t flags);
void Gecko_NoteDirtyElement(const mozilla::dom::Element*);
void Gecko_NoteDirtySubtreeForInvalidation(const mozilla::dom::Element*);
void Gecko_NoteAnimationOnlyDirtyElement(const mozilla::dom::Element*);

<<<<<<< HEAD
bool Gecko_AnimationNameMayBeReferencedFromStyle(
    RawGeckoPresContextBorrowed pres_context, nsAtom* name);
||||||| merged common ancestors
bool Gecko_AnimationNameMayBeReferencedFromStyle(
  RawGeckoPresContextBorrowed pres_context,
  nsAtom* name);
=======
bool Gecko_AnimationNameMayBeReferencedFromStyle(const nsPresContext*,
                                                 nsAtom* name);
>>>>>>> upstream-releases

// Incremental restyle.
<<<<<<< HEAD
mozilla::CSSPseudoElementType Gecko_GetImplementedPseudo(
    RawGeckoElementBorrowed element);
||||||| merged common ancestors
mozilla::CSSPseudoElementType Gecko_GetImplementedPseudo(
  RawGeckoElementBorrowed element);
=======
mozilla::PseudoStyleType Gecko_GetImplementedPseudo(
    const mozilla::dom::Element*);
>>>>>>> upstream-releases

// We'd like to return `nsChangeHint` here, but bindgen bitfield enums don't
// work as return values with the Linux 32-bit ABI at the moment because
// they wrap the value in a struct.
<<<<<<< HEAD
uint32_t Gecko_CalcStyleDifference(ComputedStyleBorrowed old_style,
                                   ComputedStyleBorrowed new_style,
                                   bool* any_style_struct_changed,
                                   bool* reset_only_changed);
||||||| merged common ancestors
uint32_t Gecko_CalcStyleDifference(
  ComputedStyleBorrowed old_style,
  ComputedStyleBorrowed new_style,
  bool* any_style_struct_changed,
  bool* reset_only_changed);
=======
uint32_t Gecko_CalcStyleDifference(const mozilla::ComputedStyle* old_style,
                                   const mozilla::ComputedStyle* new_style,
                                   bool* any_style_struct_changed,
                                   bool* reset_only_changed);
>>>>>>> upstream-releases

// Get an element snapshot for a given element from the table.
<<<<<<< HEAD
const mozilla::ServoElementSnapshot* Gecko_GetElementSnapshot(
    const mozilla::ServoElementSnapshotTable* table,
    RawGeckoElementBorrowed element);
||||||| merged common ancestors
const ServoElementSnapshot* Gecko_GetElementSnapshot(
  const mozilla::ServoElementSnapshotTable* table,
  RawGeckoElementBorrowed element);
=======
const mozilla::ServoElementSnapshot* Gecko_GetElementSnapshot(
    const mozilla::ServoElementSnapshotTable* table,
    const mozilla::dom::Element*);
>>>>>>> upstream-releases

// Have we seen this pointer before?
bool Gecko_HaveSeenPtr(mozilla::SeenPtrs* table, const void* ptr);

// `array` must be an nsTArray
// If changing this signature, please update the
// friend function declaration in nsTArray.h
void Gecko_EnsureTArrayCapacity(void* array, size_t capacity, size_t elem_size);

// Same here, `array` must be an nsTArray<T>, for some T.
//
// Important note: Only valid for POD types, since destructors won't be run
// otherwise. This is ensured with rust traits for the relevant structs.
void Gecko_ClearPODTArray(void* array, size_t elem_size, size_t elem_align);

void Gecko_ResizeTArrayForStrings(nsTArray<nsString>* array, uint32_t length);
void Gecko_ResizeAtomArray(nsTArray<RefPtr<nsAtom>>* array, uint32_t length);

void Gecko_SetStyleGridTemplate(
    mozilla::UniquePtr<nsStyleGridTemplate>* grid_template,
    nsStyleGridTemplate* value);

nsStyleGridTemplate* Gecko_CreateStyleGridTemplate(uint32_t track_sizes,
                                                   uint32_t name_size);

void Gecko_CopyStyleGridTemplateValues(
    mozilla::UniquePtr<nsStyleGridTemplate>* grid_template,
    const nsStyleGridTemplate* other);

<<<<<<< HEAD
mozilla::css::GridTemplateAreasValue* Gecko_NewGridTemplateAreasValue(
    uint32_t areas, uint32_t templates, uint32_t columns);
||||||| merged common ancestors
mozilla::css::GridTemplateAreasValue* Gecko_NewGridTemplateAreasValue(
  uint32_t areas,
  uint32_t templates,
  uint32_t columns);
=======
// Clear the mContents, mCounterIncrements, mCounterResets, or mCounterSets
// field in nsStyleContent. This is needed to run the destructors, otherwise
// we'd leak the images, strings, and whatnot.
void Gecko_ClearAndResizeStyleContents(nsStyleContent* content,
                                       uint32_t how_many);
>>>>>>> upstream-releases

void Gecko_ClearAndResizeCounterIncrements(nsStyleContent* content,
                                           uint32_t how_many);

<<<<<<< HEAD
// Clear the mContents, mCounterIncrements, or mCounterResets field in
// nsStyleContent. This is needed to run the destructors, otherwise we'd
// leak the images, strings, and whatnot.
void Gecko_ClearAndResizeStyleContents(nsStyleContent* content,
                                       uint32_t how_many);
||||||| merged common ancestors
// Clear the mContents, mCounterIncrements, or mCounterResets field in
// nsStyleContent. This is needed to run the destructors, otherwise we'd
// leak the images, strings, and whatnot.
void Gecko_ClearAndResizeStyleContents( nsStyleContent* content,
  uint32_t how_many);
=======
void Gecko_ClearAndResizeCounterResets(nsStyleContent* content,
                                       uint32_t how_many);
>>>>>>> upstream-releases

<<<<<<< HEAD
void Gecko_ClearAndResizeCounterIncrements(nsStyleContent* content,
                                           uint32_t how_many);
||||||| merged common ancestors
void Gecko_ClearAndResizeCounterIncrements(
  nsStyleContent* content,
  uint32_t how_many);
=======
void Gecko_ClearAndResizeCounterSets(nsStyleContent* content,
                                     uint32_t how_many);
>>>>>>> upstream-releases

<<<<<<< HEAD
void Gecko_ClearAndResizeCounterResets(nsStyleContent* content,
                                       uint32_t how_many);
||||||| merged common ancestors
void Gecko_ClearAndResizeCounterResets(
  nsStyleContent* content,
  uint32_t how_many);
=======
void Gecko_CopyStyleContentsFrom(nsStyleContent* content,
                                 const nsStyleContent* other);
>>>>>>> upstream-releases

<<<<<<< HEAD
void Gecko_CopyStyleContentsFrom(nsStyleContent* content,
                                 const nsStyleContent* other);
||||||| merged common ancestors
void Gecko_CopyStyleContentsFrom(
  nsStyleContent* content,
  const nsStyleContent* other);
=======
void Gecko_CopyCounterResetsFrom(nsStyleContent* content,
                                 const nsStyleContent* other);
>>>>>>> upstream-releases

<<<<<<< HEAD
void Gecko_CopyCounterResetsFrom(nsStyleContent* content,
                                 const nsStyleContent* other);
||||||| merged common ancestors
void Gecko_CopyCounterResetsFrom(
  nsStyleContent* content,
  const nsStyleContent* other);
=======
void Gecko_CopyCounterSetsFrom(nsStyleContent* content,
                               const nsStyleContent* other);
>>>>>>> upstream-releases

void Gecko_CopyCounterIncrementsFrom(nsStyleContent* content,
                                     const nsStyleContent* other);

void Gecko_EnsureImageLayersLength(nsStyleImageLayers* layers, size_t len,
                                   nsStyleImageLayers::LayerType layer_type);

void Gecko_EnsureStyleAnimationArrayLength(void* array, size_t len);
void Gecko_EnsureStyleTransitionArrayLength(void* array, size_t len);

// Searches from the beginning of |keyframes| for a Keyframe object with the
// specified offset and timing function. If none is found, a new Keyframe object
// with the specified |offset| and |timingFunction| will be prepended to
// |keyframes|.
//
// @param keyframes  An array of Keyframe objects, sorted by offset.
//                   The first Keyframe in the array, if any, MUST have an
//                   offset greater than or equal to |offset|.
// @param offset  The offset to search for, or, if no suitable Keyframe is
//                found, the offset to use for the created Keyframe.
//                Must be a floating point number in the range [0.0, 1.0].
// @param timingFunction  The timing function to match, or, if no suitable
//                        Keyframe is found, to set on the created Keyframe.
//
// @returns  The matching or created Keyframe.
mozilla::Keyframe* Gecko_GetOrCreateKeyframeAtStart(
<<<<<<< HEAD
    RawGeckoKeyframeListBorrowedMut keyframes, float offset,
    const nsTimingFunction* timingFunction);
||||||| merged common ancestors
  RawGeckoKeyframeListBorrowedMut keyframes,
  float offset,
  const nsTimingFunction* timingFunction);
=======
    nsTArray<mozilla::Keyframe>* keyframes, float offset,
    const nsTimingFunction* timingFunction);
>>>>>>> upstream-releases

// As with Gecko_GetOrCreateKeyframeAtStart except that this method will search
// from the beginning of |keyframes| for a Keyframe with matching timing
// function and an offset of 0.0.
// Furthermore, if a matching Keyframe is not found, a new Keyframe will be
// inserted after the *last* Keyframe in |keyframes| with offset 0.0.
mozilla::Keyframe* Gecko_GetOrCreateInitialKeyframe(
<<<<<<< HEAD
    RawGeckoKeyframeListBorrowedMut keyframes,
    const nsTimingFunction* timingFunction);
||||||| merged common ancestors
  RawGeckoKeyframeListBorrowedMut keyframes,
  const nsTimingFunction* timingFunction);
=======
    nsTArray<mozilla::Keyframe>* keyframes,
    const nsTimingFunction* timingFunction);
>>>>>>> upstream-releases

// As with Gecko_GetOrCreateKeyframeAtStart except that this method will search
// from the *end* of |keyframes| for a Keyframe with matching timing function
// and an offset of 1.0. If a matching Keyframe is not found, a new Keyframe
// will be appended to the end of |keyframes|.
mozilla::Keyframe* Gecko_GetOrCreateFinalKeyframe(
<<<<<<< HEAD
    RawGeckoKeyframeListBorrowedMut keyframes,
    const nsTimingFunction* timingFunction);
||||||| merged common ancestors
  RawGeckoKeyframeListBorrowedMut keyframes,
  const nsTimingFunction* timingFunction);
=======
    nsTArray<mozilla::Keyframe>* keyframes,
    const nsTimingFunction* timingFunction);
>>>>>>> upstream-releases

// Appends and returns a new PropertyValuePair to |aProperties| initialized with
// its mProperty member set to |aProperty| and all other members initialized to
// their default values.
mozilla::PropertyValuePair* Gecko_AppendPropertyValuePair(
<<<<<<< HEAD
    RawGeckoPropertyValuePairListBorrowedMut aProperties,
    nsCSSPropertyID aProperty);
||||||| merged common ancestors
  RawGeckoPropertyValuePairListBorrowedMut aProperties,
  nsCSSPropertyID aProperty);
=======
    nsTArray<mozilla::PropertyValuePair>*, nsCSSPropertyID aProperty);
>>>>>>> upstream-releases

<<<<<<< HEAD
// Clean up pointer-based coordinates
void Gecko_ResetStyleCoord(nsStyleUnit* unit, nsStyleUnion* value);

// Set an nsStyleCoord to a computed `calc()` value
void Gecko_SetStyleCoordCalcValue(nsStyleUnit* unit, nsStyleUnion* value,
                                  nsStyleCoord::CalcValue calc);

void Gecko_CopyShapeSourceFrom(mozilla::StyleShapeSource* dst,
                               const mozilla::StyleShapeSource* src);
||||||| merged common ancestors
// Clean up pointer-based coordinates
void Gecko_ResetStyleCoord(
  nsStyleUnit* unit,
  nsStyleUnion* value);

// Set an nsStyleCoord to a computed `calc()` value
void Gecko_SetStyleCoordCalcValue(
  nsStyleUnit* unit,
  nsStyleUnion* value,
  nsStyleCoord::CalcValue calc);

void Gecko_CopyShapeSourceFrom(
  mozilla::StyleShapeSource* dst,
  const mozilla::StyleShapeSource* src);
=======
void Gecko_CopyShapeSourceFrom(mozilla::StyleShapeSource* dst,
                               const mozilla::StyleShapeSource* src);
>>>>>>> upstream-releases

void Gecko_DestroyShapeSource(mozilla::StyleShapeSource* shape);

<<<<<<< HEAD
void Gecko_NewBasicShape(mozilla::StyleShapeSource* shape,
                         mozilla::StyleBasicShapeType type);

||||||| merged common ancestors
void Gecko_NewBasicShape(
  mozilla::StyleShapeSource* shape,
  mozilla::StyleBasicShapeType type);

=======
>>>>>>> upstream-releases
void Gecko_NewShapeImage(mozilla::StyleShapeSource* shape);

<<<<<<< HEAD
void Gecko_StyleShapeSource_SetURLValue(mozilla::StyleShapeSource* shape,
                                        mozilla::css::URLValue* uri);

void Gecko_NewStyleSVGPath(mozilla::StyleShapeSource* shape);

void Gecko_SetStyleMotion(mozilla::UniquePtr<mozilla::StyleMotion>* aMotion,
                          mozilla::StyleMotion* aValue);

mozilla::StyleMotion* Gecko_NewStyleMotion();

void Gecko_CopyStyleMotions(mozilla::UniquePtr<mozilla::StyleMotion>* motion,
                            const mozilla::StyleMotion* other);
||||||| merged common ancestors
void Gecko_StyleShapeSource_SetURLValue(
  mozilla::StyleShapeSource* shape,
  mozilla::css::URLValue* uri);

void Gecko_NewStyleSVGPath(mozilla::StyleShapeSource* shape);

void Gecko_SetStyleMotion(
  mozilla::UniquePtr<mozilla::StyleMotion>* aMotion,
  mozilla::StyleMotion* aValue);

mozilla::StyleMotion* Gecko_NewStyleMotion();

void Gecko_CopyStyleMotions(
  mozilla::UniquePtr<mozilla::StyleMotion>* motion,
  const mozilla::StyleMotion* other);
=======
void Gecko_SetToSVGPath(
    mozilla::StyleShapeSource* shape,
    mozilla::StyleForgottenArcSlicePtr<mozilla::StylePathCommand>,
    mozilla::StyleFillRule);
>>>>>>> upstream-releases

void Gecko_ResetFilters(nsStyleEffects* effects, size_t new_len);

void Gecko_CopyFiltersFrom(nsStyleEffects* aSrc, nsStyleEffects* aDest);

<<<<<<< HEAD
void Gecko_nsStyleFilter_SetURLValue(nsStyleFilter* effects,
                                     mozilla::css::URLValue* uri);

void Gecko_nsStyleSVGPaint_CopyFrom(nsStyleSVGPaint* dest,
                                    const nsStyleSVGPaint* src);

void Gecko_nsStyleSVGPaint_SetURLValue(nsStyleSVGPaint* paint,
                                       mozilla::css::URLValue* uri);

void Gecko_nsStyleSVGPaint_Reset(nsStyleSVGPaint* paint);

||||||| merged common ancestors
void Gecko_nsStyleFilter_SetURLValue(
  nsStyleFilter* effects,
  mozilla::css::URLValue* uri);

void Gecko_nsStyleSVGPaint_CopyFrom(
  nsStyleSVGPaint* dest,
  const nsStyleSVGPaint* src);

void Gecko_nsStyleSVGPaint_SetURLValue(
  nsStyleSVGPaint* paint,
  mozilla::css::URLValue* uri);

void Gecko_nsStyleSVGPaint_Reset(nsStyleSVGPaint* paint);

=======
>>>>>>> upstream-releases
void Gecko_nsStyleSVG_SetDashArrayLength(nsStyleSVG* svg, uint32_t len);

void Gecko_nsStyleSVG_CopyDashArray(nsStyleSVG* dst, const nsStyleSVG* src);

void Gecko_nsStyleSVG_SetContextPropertiesLength(nsStyleSVG* svg, uint32_t len);

<<<<<<< HEAD
void Gecko_nsStyleSVG_CopyContextProperties(nsStyleSVG* dst,
                                            const nsStyleSVG* src);

mozilla::css::URLValue* Gecko_URLValue_Create(RawServoCssUrlDataStrong url,
                                              mozilla::CORSMode aCORSMode);
||||||| merged common ancestors
void Gecko_nsStyleSVG_CopyContextProperties(
  nsStyleSVG* dst,
  const nsStyleSVG* src);

mozilla::css::URLValue* Gecko_URLValue_Create(
  RawServoCssUrlDataStrong url,
  mozilla::CORSMode aCORSMode);
=======
void Gecko_nsStyleSVG_CopyContextProperties(nsStyleSVG* dst,
                                            const nsStyleSVG* src);
>>>>>>> upstream-releases

void Gecko_GetComputedURLSpec(const mozilla::StyleComputedUrl* url,
                              nsCString* spec);

<<<<<<< HEAD
void Gecko_GetComputedURLSpec(const mozilla::css::URLValue* url,
                              nsCString* spec);

void Gecko_GetComputedImageURLSpec(const mozilla::css::URLValue* url,
                                   nsCString* spec);
||||||| merged common ancestors
void Gecko_GetComputedURLSpec(
  const mozilla::css::URLValue* url,
  nsCString* spec);

void Gecko_GetComputedImageURLSpec(
  const mozilla::css::URLValue* url,
  nsCString* spec);
=======
void Gecko_GetComputedImageURLSpec(const mozilla::StyleComputedUrl* url,
                                   nsCString* spec);
>>>>>>> upstream-releases

void Gecko_nsIURI_Debug(nsIURI*, nsCString* spec);

NS_DECL_THREADSAFE_FFI_REFCOUNTING(mozilla::URLExtraData, URLExtraData);

void Gecko_FillAllImageLayers(nsStyleImageLayers* layers, uint32_t max_len);

<<<<<<< HEAD
NS_DECL_THREADSAFE_FFI_REFCOUNTING(nsStyleCoord::Calc, Calc);

nsCSSShadowArray* Gecko_NewCSSShadowArray(uint32_t len);

NS_DECL_THREADSAFE_FFI_REFCOUNTING(nsCSSShadowArray, CSSShadowArray);

nsCSSValueSharedList* Gecko_NewCSSValueSharedList(uint32_t len);
nsCSSValueSharedList* Gecko_NewNoneTransform();

// Getter for nsCSSValue
nsCSSValueBorrowedMut Gecko_CSSValue_GetArrayItem(
    nsCSSValueBorrowedMut css_value, int32_t index);

// const version of the above function.
nsCSSValueBorrowed Gecko_CSSValue_GetArrayItemConst(
    nsCSSValueBorrowed css_value, int32_t index);

nsCSSKeyword Gecko_CSSValue_GetKeyword(nsCSSValueBorrowed aCSSValue);
float Gecko_CSSValue_GetNumber(nsCSSValueBorrowed css_value);
float Gecko_CSSValue_GetPercentage(nsCSSValueBorrowed css_value);
nsStyleCoord::CalcValue Gecko_CSSValue_GetCalc(nsCSSValueBorrowed aCSSValue);
void Gecko_CSSValue_SetNumber(nsCSSValueBorrowedMut css_value, float number);

void Gecko_CSSValue_SetKeyword(nsCSSValueBorrowedMut css_value,
                               nsCSSKeyword keyword);

void Gecko_CSSValue_SetPercentage(nsCSSValueBorrowedMut css_value,
                                  float percent);

void Gecko_CSSValue_SetPixelLength(nsCSSValueBorrowedMut aCSSValue, float aLen);

void Gecko_CSSValue_SetCalc(nsCSSValueBorrowedMut css_value,
                            nsStyleCoord::CalcValue calc);

void Gecko_CSSValue_SetFunction(nsCSSValueBorrowedMut css_value, int32_t len);

void Gecko_CSSValue_SetString(nsCSSValueBorrowedMut css_value,
                              const uint8_t* string, uint32_t len,
                              nsCSSUnit unit);

void Gecko_CSSValue_SetStringFromAtom(nsCSSValueBorrowedMut css_value,
                                      nsAtom* atom, nsCSSUnit unit);

// Take an addrefed nsAtom and set it to the nsCSSValue
void Gecko_CSSValue_SetAtomIdent(nsCSSValueBorrowedMut css_value, nsAtom* atom);
void Gecko_CSSValue_SetArray(nsCSSValueBorrowedMut css_value, int32_t len);

void Gecko_CSSValue_SetInt(nsCSSValueBorrowedMut css_value, int32_t integer,
                           nsCSSUnit unit);

void Gecko_CSSValue_SetFloat(nsCSSValueBorrowedMut css_value, float value,
                             nsCSSUnit unit);

void Gecko_CSSValue_SetPair(nsCSSValueBorrowedMut css_value,
                            nsCSSValueBorrowed xvalue,
                            nsCSSValueBorrowed yvalue);

void Gecko_CSSValue_SetList(nsCSSValueBorrowedMut css_value, uint32_t len);
void Gecko_CSSValue_SetPairList(nsCSSValueBorrowedMut css_value, uint32_t len);

void Gecko_CSSValue_InitSharedList(nsCSSValueBorrowedMut css_value,
                                   uint32_t len);

void Gecko_CSSValue_Drop(nsCSSValueBorrowedMut css_value);

NS_DECL_THREADSAFE_FFI_REFCOUNTING(nsCSSValueSharedList, CSSValueSharedList);

||||||| merged common ancestors
NS_DECL_THREADSAFE_FFI_REFCOUNTING(nsStyleCoord::Calc, Calc);

nsCSSShadowArray* Gecko_NewCSSShadowArray(uint32_t len);

NS_DECL_THREADSAFE_FFI_REFCOUNTING(nsCSSShadowArray, CSSShadowArray);

nsStyleQuoteValues* Gecko_NewStyleQuoteValues(uint32_t len);

NS_DECL_THREADSAFE_FFI_REFCOUNTING(nsStyleQuoteValues, QuoteValues);

nsCSSValueSharedList* Gecko_NewCSSValueSharedList(uint32_t len);
nsCSSValueSharedList* Gecko_NewNoneTransform();

// Getter for nsCSSValue
nsCSSValueBorrowedMut Gecko_CSSValue_GetArrayItem(
  nsCSSValueBorrowedMut css_value,
  int32_t index);

// const version of the above function.
nsCSSValueBorrowed Gecko_CSSValue_GetArrayItemConst(
  nsCSSValueBorrowed css_value,
  int32_t index);

nsCSSKeyword Gecko_CSSValue_GetKeyword(nsCSSValueBorrowed aCSSValue);
float Gecko_CSSValue_GetNumber(nsCSSValueBorrowed css_value);
float Gecko_CSSValue_GetPercentage(nsCSSValueBorrowed css_value);
nsStyleCoord::CalcValue Gecko_CSSValue_GetCalc(nsCSSValueBorrowed aCSSValue);
void Gecko_CSSValue_SetNumber(nsCSSValueBorrowedMut css_value, float number);

void Gecko_CSSValue_SetKeyword(
  nsCSSValueBorrowedMut css_value,
  nsCSSKeyword keyword);

void Gecko_CSSValue_SetPercentage(
  nsCSSValueBorrowedMut css_value,
  float percent);

void Gecko_CSSValue_SetPixelLength(nsCSSValueBorrowedMut aCSSValue, float aLen);

void Gecko_CSSValue_SetCalc(
  nsCSSValueBorrowedMut css_value,
  nsStyleCoord::CalcValue calc);

void Gecko_CSSValue_SetFunction(nsCSSValueBorrowedMut css_value, int32_t len);

void Gecko_CSSValue_SetString(
  nsCSSValueBorrowedMut css_value,
  const uint8_t* string,
  uint32_t len,
  nsCSSUnit unit);

void Gecko_CSSValue_SetStringFromAtom(
  nsCSSValueBorrowedMut css_value,
  nsAtom* atom,
  nsCSSUnit unit);

// Take an addrefed nsAtom and set it to the nsCSSValue
void Gecko_CSSValue_SetAtomIdent(nsCSSValueBorrowedMut css_value, nsAtom* atom);
void Gecko_CSSValue_SetArray(nsCSSValueBorrowedMut css_value, int32_t len);

void Gecko_CSSValue_SetInt(
  nsCSSValueBorrowedMut css_value,
  int32_t integer,
  nsCSSUnit unit);

void Gecko_CSSValue_SetFloat(
  nsCSSValueBorrowedMut css_value,
  float value,
  nsCSSUnit unit);

void Gecko_CSSValue_SetPair(
  nsCSSValueBorrowedMut css_value,
  nsCSSValueBorrowed xvalue,
  nsCSSValueBorrowed yvalue);

void Gecko_CSSValue_SetList(nsCSSValueBorrowedMut css_value, uint32_t len);
void Gecko_CSSValue_SetPairList(nsCSSValueBorrowedMut css_value, uint32_t len);

void Gecko_CSSValue_InitSharedList(
  nsCSSValueBorrowedMut css_value,
  uint32_t len);

void Gecko_CSSValue_Drop(nsCSSValueBorrowedMut css_value);

NS_DECL_THREADSAFE_FFI_REFCOUNTING(nsCSSValueSharedList, CSSValueSharedList);

=======
>>>>>>> upstream-releases
float Gecko_FontStretch_ToFloat(mozilla::FontStretch aStretch);

<<<<<<< HEAD
void Gecko_FontStretch_SetFloat(mozilla::FontStretch* aStretch,
                                float aFloatValue);
||||||| merged common ancestors
void Gecko_FontStretch_SetFloat(
  mozilla::FontStretch* aStretch,
  float aFloatValue);
=======
void Gecko_FontStretch_SetFloat(mozilla::FontStretch* aStretch,
                                float aFloatValue);

void Gecko_LoadData_DeregisterLoad(const mozilla::StyleLoadData*);
>>>>>>> upstream-releases

float Gecko_FontSlantStyle_ToFloat(mozilla::FontSlantStyle aStyle);
void Gecko_FontSlantStyle_SetNormal(mozilla::FontSlantStyle*);
void Gecko_FontSlantStyle_SetItalic(mozilla::FontSlantStyle*);

void Gecko_FontSlantStyle_SetOblique(mozilla::FontSlantStyle*,
                                     float angle_degrees);

void Gecko_FontSlantStyle_Get(mozilla::FontSlantStyle, bool* normal,
                              bool* italic, float* oblique_angle);

float Gecko_FontWeight_ToFloat(mozilla::FontWeight aWeight);

void Gecko_FontWeight_SetFloat(mozilla::FontWeight* aWeight, float aFloatValue);

void Gecko_nsStyleFont_SetLang(nsStyleFont* font, nsAtom* atom);

void Gecko_nsStyleFont_CopyLangFrom(nsStyleFont* aFont,
                                    const nsStyleFont* aSource);

<<<<<<< HEAD
void Gecko_nsStyleFont_FixupNoneGeneric(
    nsStyleFont* font, RawGeckoPresContextBorrowed pres_context);
||||||| merged common ancestors
void Gecko_nsStyleFont_FixupNoneGeneric(
  nsStyleFont* font,
  RawGeckoPresContextBorrowed pres_context);
=======
// Moves the generic family in the font-family to the front, or prepends
// aDefaultGeneric, so that user-configured fonts take precedent over document
// fonts.
//
// Document fonts may still be used as fallback for unsupported glyphs though.
void Gecko_nsStyleFont_PrioritizeUserFonts(
    nsStyleFont* font, mozilla::StyleGenericFontFamily aDefaultGeneric);
>>>>>>> upstream-releases

<<<<<<< HEAD
void Gecko_nsStyleFont_PrefillDefaultForGeneric(
    nsStyleFont* font, RawGeckoPresContextBorrowed pres_context,
    uint8_t generic_id);
||||||| merged common ancestors
void Gecko_nsStyleFont_PrefillDefaultForGeneric(
  nsStyleFont* font,
  RawGeckoPresContextBorrowed pres_context,
  uint8_t generic_id);
=======
nscoord Gecko_nsStyleFont_ComputeMinSize(const nsStyleFont*,
                                         const mozilla::dom::Document*);
>>>>>>> upstream-releases

<<<<<<< HEAD
void Gecko_nsStyleFont_FixupMinFontSize(
    nsStyleFont* font, RawGeckoPresContextBorrowed pres_context);
||||||| merged common ancestors
void Gecko_nsStyleFont_FixupMinFontSize(
  nsStyleFont* font,
  RawGeckoPresContextBorrowed pres_context);
=======
// Computes the default generic font for a generic family and language.
mozilla::StyleGenericFontFamily Gecko_nsStyleFont_ComputeDefaultFontType(
    const mozilla::dom::Document*,
    mozilla::StyleGenericFontFamily generic_family, nsAtom* language);
>>>>>>> upstream-releases

mozilla::FontSizePrefs Gecko_GetBaseSize(nsAtom* lang);

// XBL related functions.
<<<<<<< HEAD
RawGeckoElementBorrowedOrNull Gecko_GetBindingParent(
    RawGeckoElementBorrowed aElement);

RawServoAuthorStylesBorrowedOrNull Gecko_XBLBinding_GetRawServoStyles(
    RawGeckoXBLBindingBorrowed aXBLBinding);
||||||| merged common ancestors
RawGeckoElementBorrowedOrNull Gecko_GetBindingParent(
  RawGeckoElementBorrowed aElement);

RawServoAuthorStylesBorrowedOrNull Gecko_XBLBinding_GetRawServoStyles(
  RawGeckoXBLBindingBorrowed aXBLBinding);
=======
const mozilla::dom::Element* Gecko_GetBindingParent(
    const mozilla::dom::Element*);
>>>>>>> upstream-releases

<<<<<<< HEAD
bool Gecko_XBLBinding_InheritsStyle(RawGeckoXBLBindingBorrowed aXBLBinding);

struct GeckoFontMetrics {
  nscoord mChSize;
||||||| merged common ancestors
bool Gecko_XBLBinding_InheritsStyle(RawGeckoXBLBindingBorrowed aXBLBinding);

struct GeckoFontMetrics
{
  nscoord mChSize;
=======
struct GeckoFontMetrics {
  nscoord mChSize;  // -1.0 indicates not found
>>>>>>> upstream-releases
  nscoord mXSize;
};

<<<<<<< HEAD
GeckoFontMetrics Gecko_GetFontMetrics(RawGeckoPresContextBorrowed pres_context,
                                      bool is_vertical, const nsStyleFont* font,
                                      nscoord font_size,
                                      bool use_user_font_set);

int32_t Gecko_GetAppUnitsPerPhysicalInch(
    RawGeckoPresContextBorrowed pres_context);
||||||| merged common ancestors
GeckoFontMetrics Gecko_GetFontMetrics(
  RawGeckoPresContextBorrowed pres_context,
  bool is_vertical,
  const nsStyleFont* font,
  nscoord font_size,
  bool use_user_font_set);

int32_t Gecko_GetAppUnitsPerPhysicalInch(
  RawGeckoPresContextBorrowed pres_context);
=======
GeckoFontMetrics Gecko_GetFontMetrics(const nsPresContext*, bool is_vertical,
                                      const nsStyleFont* font,
                                      nscoord font_size,
                                      bool use_user_font_set);
>>>>>>> upstream-releases

mozilla::StyleSheet* Gecko_StyleSheet_Clone(
    const mozilla::StyleSheet* aSheet,
    const mozilla::StyleSheet* aNewParentSheet);

void Gecko_StyleSheet_AddRef(const mozilla::StyleSheet* aSheet);
void Gecko_StyleSheet_Release(const mozilla::StyleSheet* aSheet);
nsCSSKeyword Gecko_LookupCSSKeyword(const uint8_t* string, uint32_t len);
const char* Gecko_CSSKeywordString(nsCSSKeyword keyword, uint32_t* len);
bool Gecko_IsDocumentBody(const mozilla::dom::Element* element);

// We use an int32_t here instead of a LookAndFeel::ColorID
// because forward-declaring a nested enum/struct is impossible
<<<<<<< HEAD
nscolor Gecko_GetLookAndFeelSystemColor(
    int32_t color_id, RawGeckoPresContextBorrowed pres_context);
||||||| merged common ancestors
nscolor Gecko_GetLookAndFeelSystemColor(
  int32_t color_id,
  RawGeckoPresContextBorrowed pres_context);
=======
nscolor Gecko_GetLookAndFeelSystemColor(int32_t color_id,
                                        const mozilla::dom::Document*);
>>>>>>> upstream-releases

void Gecko_AddPropertyToSet(nsCSSPropertyIDSet*, nsCSSPropertyID);

// Style-struct management.
<<<<<<< HEAD
#define STYLE_STRUCT(name)                                            \
  void Gecko_Construct_Default_nsStyle##name(                         \
      nsStyle##name* ptr, RawGeckoPresContextBorrowed pres_context);  \
  void Gecko_CopyConstruct_nsStyle##name(nsStyle##name* ptr,          \
                                         const nsStyle##name* other); \
||||||| merged common ancestors
#define STYLE_STRUCT(name)                                    \
  void Gecko_Construct_Default_nsStyle##name(                 \
    nsStyle##name* ptr,                                       \
    RawGeckoPresContextBorrowed pres_context);                \
  void Gecko_CopyConstruct_nsStyle##name(                     \
    nsStyle##name* ptr,                                       \
    const nsStyle##name* other);                              \
=======
#define STYLE_STRUCT(name)                                                   \
  void Gecko_Construct_Default_nsStyle##name(nsStyle##name* ptr,             \
                                             const mozilla::dom::Document*); \
  void Gecko_CopyConstruct_nsStyle##name(nsStyle##name* ptr,                 \
                                         const nsStyle##name* other);        \
>>>>>>> upstream-releases
  void Gecko_Destroy_nsStyle##name(nsStyle##name* ptr);
#include "nsStyleStructList.h"
#undef STYLE_STRUCT

void Gecko_RegisterProfilerThread(const char* name);
void Gecko_UnregisterProfilerThread();

#ifdef MOZ_GECKO_PROFILER
void Gecko_Construct_AutoProfilerLabel(mozilla::AutoProfilerLabel*,
                                       JS::ProfilingCategoryPair);
void Gecko_Destroy_AutoProfilerLabel(mozilla::AutoProfilerLabel*);
#endif

bool Gecko_DocumentRule_UseForPresentation(
<<<<<<< HEAD
    RawGeckoPresContextBorrowed, const nsACString* aPattern,
    mozilla::css::DocumentMatchingFunction);
||||||| merged common ancestors
  RawGeckoPresContextBorrowed,
  const nsACString* aPattern,
  mozilla::css::DocumentMatchingFunction);
=======
    const mozilla::dom::Document*, const nsACString* aPattern,
    mozilla::css::DocumentMatchingFunction);
>>>>>>> upstream-releases

// Allocator hinting.
void Gecko_SetJemallocThreadLocalArena(bool enabled);

// Pseudo-element flags.
#define CSS_PSEUDO_ELEMENT(name_, value_, flags_) \
  const uint32_t SERVO_CSS_PSEUDO_ELEMENT_FLAGS_##name_ = flags_;
#include "nsCSSPseudoElementList.h"
#undef CSS_PSEUDO_ELEMENT

<<<<<<< HEAD
bool Gecko_ErrorReportingEnabled(const mozilla::StyleSheet* sheet,
                                 const mozilla::css::Loader* loader);

void Gecko_ReportUnexpectedCSSError(const mozilla::StyleSheet* sheet,
                                    const mozilla::css::Loader* loader,
                                    nsIURI* uri, const char* message,
                                    const char* param, uint32_t paramLen,
                                    const char* prefix, const char* prefixParam,
                                    uint32_t prefixParamLen, const char* suffix,
                                    const char* source, uint32_t sourceLen,
                                    uint32_t lineNumber, uint32_t colNumber);
||||||| merged common ancestors
bool Gecko_ErrorReportingEnabled(
  const mozilla::StyleSheet* sheet,
  const mozilla::css::Loader* loader);

void Gecko_ReportUnexpectedCSSError(
  const mozilla::StyleSheet* sheet,
  const mozilla::css::Loader* loader,
  nsIURI* uri,
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
  uint32_t colNumber);
=======
bool Gecko_ErrorReportingEnabled(const mozilla::StyleSheet* sheet,
                                 const mozilla::css::Loader* loader);

void Gecko_ReportUnexpectedCSSError(
    const mozilla::StyleSheet* sheet, const mozilla::css::Loader* loader,
    nsIURI* uri, const char* message, const char* param, uint32_t paramLen,
    const char* prefix, const char* prefixParam, uint32_t prefixParamLen,
    const char* suffix, const char* source, uint32_t sourceLen,
    const char* selectors, uint32_t selectorsLen, uint32_t lineNumber,
    uint32_t colNumber);
>>>>>>> upstream-releases

// DOM APIs.
<<<<<<< HEAD
void Gecko_ContentList_AppendAll(nsSimpleContentList* aContentList,
                                 const RawGeckoElement** aElements,
                                 size_t aLength);
||||||| merged common ancestors
void Gecko_ContentList_AppendAll(
  nsSimpleContentList* aContentList,
  const RawGeckoElement** aElements,
  size_t aLength);
=======
void Gecko_ContentList_AppendAll(nsSimpleContentList* aContentList,
                                 const mozilla::dom::Element** aElements,
                                 size_t aLength);
>>>>>>> upstream-releases

// FIXME(emilio): These two below should be a single function that takes a
// `const DocumentOrShadowRoot*`, but that doesn't make MSVC builds happy for a
// reason I haven't really dug into.
const nsTArray<mozilla::dom::Element*>* Gecko_Document_GetElementsWithId(
<<<<<<< HEAD
    const nsIDocument* aDocument, nsAtom* aId);
||||||| merged common ancestors
  const nsIDocument* aDocument,
  nsAtom* aId);
=======
    const mozilla::dom::Document*, nsAtom* aId);
>>>>>>> upstream-releases

const nsTArray<mozilla::dom::Element*>* Gecko_ShadowRoot_GetElementsWithId(
<<<<<<< HEAD
    const mozilla::dom::ShadowRoot* aDocument, nsAtom* aId);
||||||| merged common ancestors
  const mozilla::dom::ShadowRoot* aDocument,
  nsAtom* aId);
=======
    const mozilla::dom::ShadowRoot*, nsAtom* aId);
>>>>>>> upstream-releases

// Check the value of the given bool preference. The pref name needs to
// be null-terminated.
bool Gecko_GetBoolPrefValue(const char* pref_name);

// Returns true if we're currently performing the servo traversal.
bool Gecko_IsInServoTraversal();

// Returns true if we're currently on the main thread.
bool Gecko_IsMainThread();

// Media feature helpers.
//
// Defined in nsMediaFeatures.cpp.
mozilla::StyleDisplayMode Gecko_MediaFeatures_GetDisplayMode(
    const mozilla::dom::Document*);

uint32_t Gecko_MediaFeatures_GetColorDepth(const mozilla::dom::Document*);

<<<<<<< HEAD
void Gecko_MediaFeatures_GetDeviceSize(nsIDocument*, nscoord* width,
                                       nscoord* height);
||||||| merged common ancestors
void Gecko_MediaFeatures_GetDeviceSize(
  nsIDocument*,
  nscoord* width,
  nscoord* height);
=======
void Gecko_MediaFeatures_GetDeviceSize(const mozilla::dom::Document*,
                                       nscoord* width, nscoord* height);
>>>>>>> upstream-releases

float Gecko_MediaFeatures_GetResolution(const mozilla::dom::Document*);
bool Gecko_MediaFeatures_PrefersReducedMotion(const mozilla::dom::Document*);
mozilla::StylePrefersColorScheme Gecko_MediaFeatures_PrefersColorScheme(
    const mozilla::dom::Document*);

mozilla::PointerCapabilities Gecko_MediaFeatures_PrimaryPointerCapabilities(
<<<<<<< HEAD
    nsIDocument*);
||||||| merged common ancestors
  nsIDocument*);
=======
    const mozilla::dom::Document*);
>>>>>>> upstream-releases

mozilla::PointerCapabilities Gecko_MediaFeatures_AllPointerCapabilities(
<<<<<<< HEAD
    nsIDocument*);
||||||| merged common ancestors
  nsIDocument*);
=======
    const mozilla::dom::Document*);

float Gecko_MediaFeatures_GetDevicePixelRatio(const mozilla::dom::Document*);
>>>>>>> upstream-releases

bool Gecko_MediaFeatures_HasSystemMetric(const mozilla::dom::Document*,
                                         nsAtom* metric,
                                         bool is_accessible_from_content);

<<<<<<< HEAD
bool Gecko_MediaFeatures_HasSystemMetric(nsIDocument*, nsAtom* metric,
                                         bool is_accessible_from_content);
||||||| merged common ancestors
bool Gecko_MediaFeatures_HasSystemMetric(
  nsIDocument*,
  nsAtom* metric,
  bool is_accessible_from_content);
=======
bool Gecko_MediaFeatures_IsResourceDocument(const mozilla::dom::Document*);
nsAtom* Gecko_MediaFeatures_GetOperatingSystemVersion(
    const mozilla::dom::Document*);
>>>>>>> upstream-releases

void Gecko_PrintfStderr(const nsCString*);

}  // extern "C"

#endif  // mozilla_GeckoBindings_h
