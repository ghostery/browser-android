/* -*- Mode: C++; tab-width: 8; indent-tabs-mode: nil; c-basic-offset: 2 -*- */
/* vim: set ts=8 sts=2 et sw=2 tw=80: */
/* This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/. */

#include "mozilla/dom/SVGUseElement.h"

#include "mozilla/ArrayUtils.h"
#include "mozilla/ErrorResult.h"
#include "mozilla/dom/SVGLengthBinding.h"
#include "mozilla/dom/SVGUseElementBinding.h"
#include "nsGkAtoms.h"
#include "mozilla/dom/SVGSVGElement.h"
#include "mozilla/dom/Document.h"
#include "mozilla/dom/Element.h"
#include "nsContentUtils.h"
#include "nsIURI.h"
#include "mozilla/URLExtraData.h"
#include "SVGObserverUtils.h"
#include "nsSVGUseFrame.h"
#include "mozilla/net/ReferrerPolicy.h"

NS_IMPL_NS_NEW_SVG_ELEMENT(Use)

namespace mozilla {
namespace dom {

JSObject* SVGUseElement::WrapNode(JSContext* aCx,
                                  JS::Handle<JSObject*> aGivenProto) {
  return SVGUseElement_Binding::Wrap(aCx, this, aGivenProto);
}

////////////////////////////////////////////////////////////////////////
// implementation

<<<<<<< HEAD
nsSVGElement::LengthInfo SVGUseElement::sLengthInfo[4] = {
    {nsGkAtoms::x, 0, SVGLength_Binding::SVG_LENGTHTYPE_NUMBER,
     SVGContentUtils::X},
    {nsGkAtoms::y, 0, SVGLength_Binding::SVG_LENGTHTYPE_NUMBER,
     SVGContentUtils::Y},
    {nsGkAtoms::width, 0, SVGLength_Binding::SVG_LENGTHTYPE_NUMBER,
     SVGContentUtils::X},
    {nsGkAtoms::height, 0, SVGLength_Binding::SVG_LENGTHTYPE_NUMBER,
     SVGContentUtils::Y},
||||||| merged common ancestors
nsSVGElement::LengthInfo SVGUseElement::sLengthInfo[4] =
{
  { nsGkAtoms::x, 0, SVGLength_Binding::SVG_LENGTHTYPE_NUMBER, SVGContentUtils::X },
  { nsGkAtoms::y, 0, SVGLength_Binding::SVG_LENGTHTYPE_NUMBER, SVGContentUtils::Y },
  { nsGkAtoms::width, 0, SVGLength_Binding::SVG_LENGTHTYPE_NUMBER, SVGContentUtils::X },
  { nsGkAtoms::height, 0, SVGLength_Binding::SVG_LENGTHTYPE_NUMBER, SVGContentUtils::Y },
=======
SVGElement::LengthInfo SVGUseElement::sLengthInfo[4] = {
    {nsGkAtoms::x, 0, SVGLength_Binding::SVG_LENGTHTYPE_NUMBER,
     SVGContentUtils::X},
    {nsGkAtoms::y, 0, SVGLength_Binding::SVG_LENGTHTYPE_NUMBER,
     SVGContentUtils::Y},
    {nsGkAtoms::width, 0, SVGLength_Binding::SVG_LENGTHTYPE_NUMBER,
     SVGContentUtils::X},
    {nsGkAtoms::height, 0, SVGLength_Binding::SVG_LENGTHTYPE_NUMBER,
     SVGContentUtils::Y},
>>>>>>> upstream-releases
};

<<<<<<< HEAD
nsSVGElement::StringInfo SVGUseElement::sStringInfo[2] = {
    {nsGkAtoms::href, kNameSpaceID_None, true},
    {nsGkAtoms::href, kNameSpaceID_XLink, true}};
||||||| merged common ancestors
nsSVGElement::StringInfo SVGUseElement::sStringInfo[2] =
{
  { nsGkAtoms::href, kNameSpaceID_None, true },
  { nsGkAtoms::href, kNameSpaceID_XLink, true }
};
=======
SVGElement::StringInfo SVGUseElement::sStringInfo[2] = {
    {nsGkAtoms::href, kNameSpaceID_None, true},
    {nsGkAtoms::href, kNameSpaceID_XLink, true}};
>>>>>>> upstream-releases

//----------------------------------------------------------------------
// nsISupports methods

NS_IMPL_CYCLE_COLLECTION_CLASS(SVGUseElement)

NS_IMPL_CYCLE_COLLECTION_UNLINK_BEGIN_INHERITED(SVGUseElement,
                                                SVGUseElementBase)
  nsAutoScriptBlocker scriptBlocker;
  NS_IMPL_CYCLE_COLLECTION_UNLINK(mOriginal)
  tmp->UnlinkSource();
NS_IMPL_CYCLE_COLLECTION_UNLINK_END
NS_IMPL_CYCLE_COLLECTION_TRAVERSE_BEGIN_INHERITED(SVGUseElement,
                                                  SVGUseElementBase)
  NS_IMPL_CYCLE_COLLECTION_TRAVERSE(mOriginal)
  tmp->mReferencedElementTracker.Traverse(&cb);
NS_IMPL_CYCLE_COLLECTION_TRAVERSE_END

NS_IMPL_ISUPPORTS_CYCLE_COLLECTION_INHERITED(SVGUseElement, SVGUseElementBase,
                                             nsIMutationObserver)

//----------------------------------------------------------------------
// Implementation

SVGUseElement::SVGUseElement(
    already_AddRefed<mozilla::dom::NodeInfo>&& aNodeInfo)
    : SVGUseElementBase(std::move(aNodeInfo)),
      mReferencedElementTracker(this) {}

SVGUseElement::~SVGUseElement() {
  UnlinkSource();
  MOZ_DIAGNOSTIC_ASSERT(!OwnerDoc()->SVGUseElementNeedsShadowTreeUpdate(*this),
                        "Dying without unbinding?");
}

//----------------------------------------------------------------------
// nsINode methods

void SVGUseElement::ProcessAttributeChange(int32_t aNamespaceID,
                                           nsAtom* aAttribute) {
  if (aNamespaceID == kNameSpaceID_None) {
    if (aAttribute == nsGkAtoms::x || aAttribute == nsGkAtoms::y) {
      if (auto* frame = GetFrame()) {
        frame->PositionAttributeChanged();
      }
    } else if (aAttribute == nsGkAtoms::width ||
               aAttribute == nsGkAtoms::height) {
      const bool hadValidDimensions = HasValidDimensions();
      const bool isUsed = OurWidthAndHeightAreUsed();
      if (isUsed) {
        SyncWidthOrHeight(aAttribute);
      }

      if (auto* frame = GetFrame()) {
        frame->DimensionAttributeChanged(hadValidDimensions, isUsed);
      }
    }
  }

  if ((aNamespaceID == kNameSpaceID_XLink ||
       aNamespaceID == kNameSpaceID_None) &&
      aAttribute == nsGkAtoms::href) {
    // We're changing our nature, clear out the clone information.
    if (auto* frame = GetFrame()) {
      frame->HrefChanged();
    }
    mOriginal = nullptr;
    UnlinkSource();
    TriggerReclone();
  }
}

nsresult SVGUseElement::AfterSetAttr(int32_t aNamespaceID, nsAtom* aAttribute,
                                     const nsAttrValue* aValue,
                                     const nsAttrValue* aOldValue,
                                     nsIPrincipal* aSubjectPrincipal,
                                     bool aNotify) {
  ProcessAttributeChange(aNamespaceID, aAttribute);
  return SVGUseElementBase::AfterSetAttr(aNamespaceID, aAttribute, aValue,
                                         aOldValue, aSubjectPrincipal, aNotify);
}

nsresult SVGUseElement::Clone(dom::NodeInfo* aNodeInfo,
                              nsINode** aResult) const {
  *aResult = nullptr;
  SVGUseElement* it = new SVGUseElement(do_AddRef(aNodeInfo));

  nsCOMPtr<nsINode> kungFuDeathGrip(it);
  nsresult rv1 = it->Init();
  nsresult rv2 = const_cast<SVGUseElement*>(this)->CopyInnerTo(it);

  // SVGUseElement specific portion - record who we cloned from
  it->mOriginal = const_cast<SVGUseElement*>(this);

  if (NS_SUCCEEDED(rv1) && NS_SUCCEEDED(rv2)) {
    kungFuDeathGrip.swap(*aResult);
  }

  return NS_FAILED(rv1) ? rv1 : rv2;
}

<<<<<<< HEAD
nsresult SVGUseElement::BindToTree(nsIDocument* aDocument, nsIContent* aParent,
                                   nsIContent* aBindingParent) {
  nsresult rv =
      SVGUseElementBase::BindToTree(aDocument, aParent, aBindingParent);
||||||| merged common ancestors
nsresult
SVGUseElement::BindToTree(nsIDocument* aDocument,
                          nsIContent* aParent,
                          nsIContent* aBindingParent)
{
  nsresult rv = SVGUseElementBase::BindToTree(aDocument, aParent, aBindingParent);
=======
nsresult SVGUseElement::BindToTree(BindContext& aContext, nsINode& aParent) {
  nsresult rv = SVGUseElementBase::BindToTree(aContext, aParent);
>>>>>>> upstream-releases
  NS_ENSURE_SUCCESS(rv, rv);

  TriggerReclone();
  return NS_OK;
}

<<<<<<< HEAD
void SVGUseElement::UnbindFromTree(bool aDeep, bool aNullParent) {
  SVGUseElementBase::UnbindFromTree(aDeep, aNullParent);
||||||| merged common ancestors
void
SVGUseElement::UnbindFromTree(bool aDeep, bool aNullParent)
{
  SVGUseElementBase::UnbindFromTree(aDeep, aNullParent);
=======
void SVGUseElement::UnbindFromTree(bool aNullParent) {
  SVGUseElementBase::UnbindFromTree(aNullParent);
>>>>>>> upstream-releases
  OwnerDoc()->UnscheduleSVGUseElementShadowTreeUpdate(*this);
}

<<<<<<< HEAD
already_AddRefed<SVGAnimatedString> SVGUseElement::Href() {
||||||| merged common ancestors
already_AddRefed<SVGAnimatedString>
SVGUseElement::Href()
{
=======
already_AddRefed<DOMSVGAnimatedString> SVGUseElement::Href() {
>>>>>>> upstream-releases
  return mStringAttributes[HREF].IsExplicitlySet()
             ? mStringAttributes[HREF].ToDOMAnimatedString(this)
             : mStringAttributes[XLINK_HREF].ToDOMAnimatedString(this);
}

//----------------------------------------------------------------------

<<<<<<< HEAD
already_AddRefed<SVGAnimatedLength> SVGUseElement::X() {
||||||| merged common ancestors
already_AddRefed<SVGAnimatedLength>
SVGUseElement::X()
{
=======
already_AddRefed<DOMSVGAnimatedLength> SVGUseElement::X() {
>>>>>>> upstream-releases
  return mLengthAttributes[ATTR_X].ToDOMAnimatedLength(this);
}

<<<<<<< HEAD
already_AddRefed<SVGAnimatedLength> SVGUseElement::Y() {
||||||| merged common ancestors
already_AddRefed<SVGAnimatedLength>
SVGUseElement::Y()
{
=======
already_AddRefed<DOMSVGAnimatedLength> SVGUseElement::Y() {
>>>>>>> upstream-releases
  return mLengthAttributes[ATTR_Y].ToDOMAnimatedLength(this);
}

<<<<<<< HEAD
already_AddRefed<SVGAnimatedLength> SVGUseElement::Width() {
||||||| merged common ancestors
already_AddRefed<SVGAnimatedLength>
SVGUseElement::Width()
{
=======
already_AddRefed<DOMSVGAnimatedLength> SVGUseElement::Width() {
>>>>>>> upstream-releases
  return mLengthAttributes[ATTR_WIDTH].ToDOMAnimatedLength(this);
}

<<<<<<< HEAD
already_AddRefed<SVGAnimatedLength> SVGUseElement::Height() {
||||||| merged common ancestors
already_AddRefed<SVGAnimatedLength>
SVGUseElement::Height()
{
=======
already_AddRefed<DOMSVGAnimatedLength> SVGUseElement::Height() {
>>>>>>> upstream-releases
  return mLengthAttributes[ATTR_HEIGHT].ToDOMAnimatedLength(this);
}

//----------------------------------------------------------------------
// nsIMutationObserver methods

<<<<<<< HEAD
void SVGUseElement::CharacterDataChanged(nsIContent* aContent,
                                         const CharacterDataChangeInfo&) {
  if (nsContentUtils::IsInSameAnonymousTree(this, aContent)) {
||||||| merged common ancestors
void
SVGUseElement::CharacterDataChanged(nsIContent* aContent,
                                    const CharacterDataChangeInfo&)
{
  if (nsContentUtils::IsInSameAnonymousTree(this, aContent)) {
=======
void SVGUseElement::CharacterDataChanged(nsIContent* aContent,
                                         const CharacterDataChangeInfo&) {
  if (nsContentUtils::IsInSameAnonymousTree(mReferencedElementTracker.get(),
                                            aContent)) {
>>>>>>> upstream-releases
    TriggerReclone();
  }
}

<<<<<<< HEAD
void SVGUseElement::AttributeChanged(Element* aElement, int32_t aNamespaceID,
                                     nsAtom* aAttribute, int32_t aModType,
                                     const nsAttrValue* aOldValue) {
  if (nsContentUtils::IsInSameAnonymousTree(this, aElement)) {
||||||| merged common ancestors
void
SVGUseElement::AttributeChanged(Element* aElement,
                                int32_t aNamespaceID,
                                nsAtom* aAttribute,
                                int32_t aModType,
                                const nsAttrValue* aOldValue)
{
  if (nsContentUtils::IsInSameAnonymousTree(this, aElement)) {
=======
void SVGUseElement::AttributeChanged(Element* aElement, int32_t aNamespaceID,
                                     nsAtom* aAttribute, int32_t aModType,
                                     const nsAttrValue* aOldValue) {
  if (nsContentUtils::IsInSameAnonymousTree(mReferencedElementTracker.get(),
                                            aElement)) {
>>>>>>> upstream-releases
    TriggerReclone();
  }
}

void SVGUseElement::ContentAppended(nsIContent* aFirstNewContent) {
  // FIXME(emilio, bug 1442336): Why does this check the parent but
  // ContentInserted the child?
<<<<<<< HEAD
  if (nsContentUtils::IsInSameAnonymousTree(this,
                                            aFirstNewContent->GetParent())) {
||||||| merged common ancestors
  if (nsContentUtils::IsInSameAnonymousTree(this, aFirstNewContent->GetParent())) {
=======
  if (nsContentUtils::IsInSameAnonymousTree(mReferencedElementTracker.get(),
                                            aFirstNewContent->GetParent())) {
>>>>>>> upstream-releases
    TriggerReclone();
  }
}

void SVGUseElement::ContentInserted(nsIContent* aChild) {
  // FIXME(emilio, bug 1442336): Why does this check the child but
  // ContentAppended the parent?
  if (nsContentUtils::IsInSameAnonymousTree(mReferencedElementTracker.get(),
                                            aChild)) {
    TriggerReclone();
  }
}

<<<<<<< HEAD
void SVGUseElement::ContentRemoved(nsIContent* aChild,
                                   nsIContent* aPreviousSibling) {
  if (nsContentUtils::IsInSameAnonymousTree(this, aChild)) {
||||||| merged common ancestors
void
SVGUseElement::ContentRemoved(nsIContent* aChild, nsIContent* aPreviousSibling)
{
  if (nsContentUtils::IsInSameAnonymousTree(this, aChild)) {
=======
void SVGUseElement::ContentRemoved(nsIContent* aChild,
                                   nsIContent* aPreviousSibling) {
  if (nsContentUtils::IsInSameAnonymousTree(mReferencedElementTracker.get(),
                                            aChild)) {
>>>>>>> upstream-releases
    TriggerReclone();
  }
}

void SVGUseElement::NodeWillBeDestroyed(const nsINode* aNode) {
  nsCOMPtr<nsIMutationObserver> kungFuDeathGrip(this);
  UnlinkSource();
}

bool SVGUseElement::IsCyclicReferenceTo(const Element& aTarget) const {
  if (&aTarget == this) {
    return true;
  }
  if (mOriginal && mOriginal->IsCyclicReferenceTo(aTarget)) {
    return true;
  }
  for (nsINode* parent = GetParentOrHostNode(); parent;
       parent = parent->GetParentOrHostNode()) {
    if (parent == &aTarget) {
      return true;
    }
    if (auto* use = SVGUseElement::FromNode(*parent)) {
      if (mOriginal && use->mOriginal == mOriginal) {
        return true;
      }
    }
  }
  return false;
}

//----------------------------------------------------------------------

void SVGUseElement::UpdateShadowTree() {
  MOZ_ASSERT(IsInComposedDoc());

  if (mReferencedElementTracker.get()) {
    mReferencedElementTracker.get()->RemoveMutationObserver(this);
  }

  LookupHref();

  RefPtr<ShadowRoot> shadow = GetShadowRoot();
  if (!shadow) {
    shadow = AttachShadowWithoutNameChecks(ShadowRootMode::Closed);
  }
  MOZ_ASSERT(shadow);

  Element* targetElement = mReferencedElementTracker.get();
  RefPtr<Element> newElement;

  auto UpdateShadowTree = mozilla::MakeScopeExit([&]() {
    nsIContent* firstChild = shadow->GetFirstChild();
    if (firstChild) {
      MOZ_ASSERT(!firstChild->GetNextSibling());
      shadow->RemoveChildNode(firstChild, /* aNotify = */ true);
    }

    if (newElement) {
      shadow->AppendChildTo(newElement, /* aNotify = */ true);
    }
  });

  // make sure target is valid type for <use>
  // QIable nsSVGGraphicsElement would eliminate enumerating all elements
  if (!targetElement ||
      !targetElement->IsAnyOfSVGElements(
          nsGkAtoms::svg, nsGkAtoms::symbol, nsGkAtoms::g, nsGkAtoms::path,
          nsGkAtoms::text, nsGkAtoms::rect, nsGkAtoms::circle,
          nsGkAtoms::ellipse, nsGkAtoms::line, nsGkAtoms::polyline,
          nsGkAtoms::polygon, nsGkAtoms::image, nsGkAtoms::use)) {
    return;
  }

  // circular loop detection

<<<<<<< HEAD
  // check 1 - check if we're a document descendent of the target
  if (nsContentUtils::ContentIsShadowIncludingDescendantOf(this,
                                                           targetElement)) {
||||||| merged common ancestors
  // check 1 - check if we're a document descendent of the target
  if (nsContentUtils::ContentIsShadowIncludingDescendantOf(this, targetElement)) {
=======
  if (IsCyclicReferenceTo(*targetElement)) {
>>>>>>> upstream-releases
    return;
  }

<<<<<<< HEAD
  // check 2 - check if we're a clone, and if we already exist in the hierarchy
  if (mOriginal) {
    for (nsINode* parent = GetParentOrHostNode(); parent;
         parent = parent->GetParentOrHostNode()) {
      SVGUseElement* use = SVGUseElement::FromNode(*parent);
      if (use && use->mOriginal == mOriginal) {
        return;
      }
    }
  }

||||||| merged common ancestors
  // check 2 - check if we're a clone, and if we already exist in the hierarchy
  if (mOriginal) {
    for (nsINode* parent = GetParentOrHostNode();
         parent;
         parent = parent->GetParentOrHostNode()) {
      SVGUseElement* use = SVGUseElement::FromNode(*parent);
      if (use && use->mOriginal == mOriginal) {
        return;
      }
    }
  }

=======
>>>>>>> upstream-releases
  nsCOMPtr<nsIURI> baseURI = targetElement->GetBaseURI();
  if (!baseURI) {
    return;
  }

  {
    nsNodeInfoManager* nodeInfoManager = targetElement->OwnerDoc() == OwnerDoc()
                                             ? nullptr
                                             : OwnerDoc()->NodeInfoManager();

    nsCOMPtr<nsINode> newNode = nsNodeUtils::Clone(
        targetElement, true, nodeInfoManager, nullptr, IgnoreErrors());
    if (!newNode) {
      return;
    }

    MOZ_ASSERT(newNode->IsElement());
    newElement = newNode.forget().downcast<Element>();
  }

  if (newElement->IsAnyOfSVGElements(nsGkAtoms::svg, nsGkAtoms::symbol)) {
    auto* newSVGElement = static_cast<SVGElement*>(newElement.get());
    if (mLengthAttributes[ATTR_WIDTH].IsExplicitlySet())
      newSVGElement->SetLength(nsGkAtoms::width, mLengthAttributes[ATTR_WIDTH]);
    if (mLengthAttributes[ATTR_HEIGHT].IsExplicitlySet())
      newSVGElement->SetLength(nsGkAtoms::height,
                               mLengthAttributes[ATTR_HEIGHT]);
  }

  // The specs do not say which referrer policy we should use, pass RP_Unset for
  // now
  mContentURLData = new URLExtraData(
      baseURI.forget(), do_AddRef(OwnerDoc()->GetDocumentURI()),
      do_AddRef(NodePrincipal()), mozilla::net::RP_Unset);

  targetElement->AddMutationObserver(this);
}

nsIURI* SVGUseElement::GetSourceDocURI() {
  nsIContent* targetElement = mReferencedElementTracker.get();
  if (!targetElement) {
    return nullptr;
  }

  return targetElement->OwnerDoc()->GetDocumentURI();
}

static nsINode* GetClonedChild(const SVGUseElement& aUseElement) {
  const ShadowRoot* shadow = aUseElement.GetShadowRoot();
  return shadow ? shadow->GetFirstChild() : nullptr;
}

bool SVGUseElement::OurWidthAndHeightAreUsed() const {
  nsINode* clonedChild = GetClonedChild(*this);
  return clonedChild &&
         clonedChild->IsAnyOfSVGElements(nsGkAtoms::svg, nsGkAtoms::symbol);
}

//----------------------------------------------------------------------
// implementation helpers

void SVGUseElement::SyncWidthOrHeight(nsAtom* aName) {
  NS_ASSERTION(aName == nsGkAtoms::width || aName == nsGkAtoms::height,
               "The clue is in the function name");
  NS_ASSERTION(OurWidthAndHeightAreUsed(), "Don't call this");

  if (!OurWidthAndHeightAreUsed()) {
    return;
  }

<<<<<<< HEAD
  auto* target = nsSVGElement::FromNode(GetClonedChild(*this));
  uint32_t index =
      sLengthInfo[ATTR_WIDTH].mName == aName ? ATTR_WIDTH : ATTR_HEIGHT;
||||||| merged common ancestors
  auto* target = nsSVGElement::FromNode(GetClonedChild(*this));
  uint32_t index = sLengthInfo[ATTR_WIDTH].mName == aName ? ATTR_WIDTH : ATTR_HEIGHT;
=======
  auto* target = SVGElement::FromNode(GetClonedChild(*this));
  uint32_t index =
      sLengthInfo[ATTR_WIDTH].mName == aName ? ATTR_WIDTH : ATTR_HEIGHT;
>>>>>>> upstream-releases

  if (mLengthAttributes[index].IsExplicitlySet()) {
    target->SetLength(aName, mLengthAttributes[index]);
    return;
  }
  if (target->IsSVGElement(nsGkAtoms::svg)) {
    // Our width/height attribute is now no longer explicitly set, so we
    // need to revert the clone's width/height to the width/height of the
    // content that's being cloned.
    TriggerReclone();
    return;
  }
  // Our width/height attribute is now no longer explicitly set, so we
  // need to set the value to 100%
<<<<<<< HEAD
  nsSVGLength2 length;
  length.Init(SVGContentUtils::XY, 0xff, 100,
              SVGLength_Binding::SVG_LENGTHTYPE_PERCENTAGE);
||||||| merged common ancestors
  nsSVGLength2 length;
  length.Init(SVGContentUtils::XY, 0xff,
              100, SVGLength_Binding::SVG_LENGTHTYPE_PERCENTAGE);
=======
  SVGAnimatedLength length;
  length.Init(SVGContentUtils::XY, 0xff, 100,
              SVGLength_Binding::SVG_LENGTHTYPE_PERCENTAGE);
>>>>>>> upstream-releases
  target->SetLength(aName, length);
}

void SVGUseElement::LookupHref() {
  nsAutoString href;
  if (mStringAttributes[HREF].IsExplicitlySet()) {
    mStringAttributes[HREF].GetAnimValue(href, this);
  } else {
    mStringAttributes[XLINK_HREF].GetAnimValue(href, this);
  }

  if (href.IsEmpty()) {
    return;
  }

  nsCOMPtr<nsIURI> originURI =
      mOriginal ? mOriginal->GetBaseURI() : GetBaseURI();
  nsCOMPtr<nsIURI> baseURI =
      nsContentUtils::IsLocalRefURL(href)
          ? SVGObserverUtils::GetBaseURLForLocalRef(this, originURI)
          : originURI;

  nsCOMPtr<nsIURI> targetURI;
  nsContentUtils::NewURIWithDocumentCharset(getter_AddRefs(targetURI), href,
                                            GetComposedDoc(), baseURI);
  // Bug 1415044 to investigate which referrer we should use
  mReferencedElementTracker.ResetToURIFragmentID(
      this, targetURI, OwnerDoc()->GetDocumentURI(),
      OwnerDoc()->GetReferrerPolicy());
}

<<<<<<< HEAD
void SVGUseElement::TriggerReclone() {
  if (nsIDocument* doc = GetComposedDoc()) {
||||||| merged common ancestors
void
SVGUseElement::TriggerReclone()
{
  if (nsIDocument* doc = GetComposedDoc()) {
=======
void SVGUseElement::TriggerReclone() {
  if (Document* doc = GetComposedDoc()) {
>>>>>>> upstream-releases
    doc->ScheduleSVGUseElementShadowTreeUpdate(*this);
  }
}

void SVGUseElement::UnlinkSource() {
  if (mReferencedElementTracker.get()) {
    mReferencedElementTracker.get()->RemoveMutationObserver(this);
  }
  mReferencedElementTracker.Unlink();
}

//----------------------------------------------------------------------
// SVGElement methods

<<<<<<< HEAD
/* virtual */ gfxMatrix SVGUseElement::PrependLocalTransformsTo(
    const gfxMatrix& aMatrix, SVGTransformTypes aWhich) const {
||||||| merged common ancestors
/* virtual */ gfxMatrix
SVGUseElement::PrependLocalTransformsTo(
  const gfxMatrix &aMatrix, SVGTransformTypes aWhich) const
{
=======
/* virtual */
gfxMatrix SVGUseElement::PrependLocalTransformsTo(
    const gfxMatrix& aMatrix, SVGTransformTypes aWhich) const {
>>>>>>> upstream-releases
  // 'transform' attribute:
  gfxMatrix userToParent;

  if (aWhich == eUserSpaceToParent || aWhich == eAllTransforms) {
    userToParent =
        GetUserToParentTransform(mAnimateMotionTransform, mTransforms);
    if (aWhich == eUserSpaceToParent) {
      return userToParent * aMatrix;
    }
  }

  // our 'x' and 'y' attributes:
  float x, y;
  const_cast<SVGUseElement*>(this)->GetAnimatedLengthValues(&x, &y, nullptr);

  gfxMatrix childToUser = gfxMatrix::Translation(x, y);

  if (aWhich == eAllTransforms) {
    return childToUser * userToParent * aMatrix;
  }

  MOZ_ASSERT(aWhich == eChildToUserSpace, "Unknown TransformTypes");

  // The following may look broken because pre-multiplying our eChildToUserSpace
  // transform with another matrix without including our eUserSpaceToParent
  // transform between the two wouldn't make sense.  We don't expect that to
  // ever happen though.  We get here either when the identity matrix has been
  // passed because our caller just wants our eChildToUserSpace transform, or
  // when our eUserSpaceToParent transform has already been multiplied into the
  // matrix that our caller passes (such as when we're called from PaintSVG).
  return childToUser * aMatrix;
}

<<<<<<< HEAD
/* virtual */ bool SVGUseElement::HasValidDimensions() const {
||||||| merged common ancestors
/* virtual */ bool
SVGUseElement::HasValidDimensions() const
{
=======
/* virtual */
bool SVGUseElement::HasValidDimensions() const {
>>>>>>> upstream-releases
  return (!mLengthAttributes[ATTR_WIDTH].IsExplicitlySet() ||
          mLengthAttributes[ATTR_WIDTH].GetAnimValInSpecifiedUnits() > 0) &&
         (!mLengthAttributes[ATTR_HEIGHT].IsExplicitlySet() ||
          mLengthAttributes[ATTR_HEIGHT].GetAnimValInSpecifiedUnits() > 0);
}

<<<<<<< HEAD
nsSVGElement::LengthAttributesInfo SVGUseElement::GetLengthInfo() {
||||||| merged common ancestors
nsSVGElement::LengthAttributesInfo
SVGUseElement::GetLengthInfo()
{
=======
SVGElement::LengthAttributesInfo SVGUseElement::GetLengthInfo() {
>>>>>>> upstream-releases
  return LengthAttributesInfo(mLengthAttributes, sLengthInfo,
                              ArrayLength(sLengthInfo));
}

<<<<<<< HEAD
nsSVGElement::StringAttributesInfo SVGUseElement::GetStringInfo() {
||||||| merged common ancestors
nsSVGElement::StringAttributesInfo
SVGUseElement::GetStringInfo()
{
=======
SVGElement::StringAttributesInfo SVGUseElement::GetStringInfo() {
>>>>>>> upstream-releases
  return StringAttributesInfo(mStringAttributes, sStringInfo,
                              ArrayLength(sStringInfo));
}

nsSVGUseFrame* SVGUseElement::GetFrame() const {
  nsIFrame* frame = GetPrimaryFrame();
  // We might be a plain nsSVGContainerFrame if we didn't pass the conditional
  // processing checks.
  if (!frame || !frame->IsSVGUseFrame()) {
    MOZ_ASSERT_IF(frame, frame->Type() == LayoutFrameType::None);
    return nullptr;
  }
  return static_cast<nsSVGUseFrame*>(frame);
}

//----------------------------------------------------------------------
// nsIContent methods

NS_IMETHODIMP_(bool)
SVGUseElement::IsAttributeMapped(const nsAtom* name) const {
  static const MappedAttributeEntry* const map[] = {sFEFloodMap,
                                                    sFiltersMap,
                                                    sFontSpecificationMap,
                                                    sGradientStopMap,
                                                    sLightingEffectsMap,
                                                    sMarkersMap,
                                                    sTextContentElementsMap,
                                                    sViewportsMap};

  return FindAttributeDependence(name, map) ||
         SVGUseElementBase::IsAttributeMapped(name);
}

}  // namespace dom
}  // namespace mozilla
