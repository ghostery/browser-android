/* -*- Mode: C++; tab-width: 8; indent-tabs-mode: nil; c-basic-offset: 2 -*- */
/* vim: set ts=8 sts=2 et sw=2 tw=80: */
/* This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/. */

#include "mozilla/AsyncEventDispatcher.h"
#include "mozilla/dom/BindContext.h"
#include "mozilla/dom/HTMLMetaElement.h"
#include "mozilla/dom/HTMLMetaElementBinding.h"
#include "mozilla/dom/nsCSPService.h"
#include "mozilla/Logging.h"
#include "nsContentUtils.h"
#include "nsStyleConsts.h"
#include "nsIContentSecurityPolicy.h"

static mozilla::LazyLogModule gMetaElementLog("nsMetaElement");
#define LOG(msg) MOZ_LOG(gMetaElementLog, mozilla::LogLevel::Debug, msg)
#define LOG_ENABLED() MOZ_LOG_TEST(gMetaElementLog, mozilla::LogLevel::Debug)

NS_IMPL_NS_NEW_HTML_ELEMENT(Meta)

namespace mozilla {
namespace dom {

HTMLMetaElement::HTMLMetaElement(
    already_AddRefed<mozilla::dom::NodeInfo>&& aNodeInfo)
    : nsGenericHTMLElement(std::move(aNodeInfo)) {}

HTMLMetaElement::~HTMLMetaElement() {}

NS_IMPL_ELEMENT_CLONE(HTMLMetaElement)

<<<<<<< HEAD
void HTMLMetaElement::SetMetaReferrer(nsIDocument* aDocument) {
  if (!aDocument || !AttrValueIs(kNameSpaceID_None, nsGkAtoms::name,
                                 nsGkAtoms::referrer, eIgnoreCase)) {
||||||| merged common ancestors

void
HTMLMetaElement::SetMetaReferrer(nsIDocument* aDocument)
{
  if (!aDocument ||
      !AttrValueIs(kNameSpaceID_None, nsGkAtoms::name, nsGkAtoms::referrer, eIgnoreCase)) {
=======
void HTMLMetaElement::SetMetaReferrer(Document* aDocument) {
  if (!aDocument || !AttrValueIs(kNameSpaceID_None, nsGkAtoms::name,
                                 nsGkAtoms::referrer, eIgnoreCase)) {
>>>>>>> upstream-releases
    return;
  }
  nsAutoString content;
  GetContent(content);

  Element* headElt = aDocument->GetHeadElement();
  if (headElt && nsContentUtils::ContentIsDescendantOf(this, headElt)) {
<<<<<<< HEAD
    content = nsContentUtils::TrimWhitespace<nsContentUtils::IsHTMLWhitespace>(
        content);
    aDocument->SetHeaderData(nsGkAtoms::referrer, content);
||||||| merged common ancestors
      content = nsContentUtils::TrimWhitespace<nsContentUtils::IsHTMLWhitespace>(content);
      aDocument->SetHeaderData(nsGkAtoms::referrer, content);
=======
    content = nsContentUtils::TrimWhitespace<nsContentUtils::IsHTMLWhitespace>(
        content);
    aDocument->UpdateReferrerInfoFromMeta(content, false);
>>>>>>> upstream-releases
  }
}

nsresult HTMLMetaElement::AfterSetAttr(int32_t aNameSpaceID, nsAtom* aName,
                                       const nsAttrValue* aValue,
                                       const nsAttrValue* aOldValue,
                                       nsIPrincipal* aSubjectPrincipal,
                                       bool aNotify) {
  if (aNameSpaceID == kNameSpaceID_None) {
<<<<<<< HEAD
    nsIDocument* document = GetUncomposedDoc();
||||||| merged common ancestors
    nsIDocument *document = GetUncomposedDoc();
=======
    Document* document = GetUncomposedDoc();
>>>>>>> upstream-releases
    if (aName == nsGkAtoms::content) {
      if (document && AttrValueIs(kNameSpaceID_None, nsGkAtoms::name,
                                  nsGkAtoms::viewport, eIgnoreCase)) {
        nsAutoString content;
        GetContent(content);
        nsContentUtils::ProcessViewportInfo(document, content);
      }
      CreateAndDispatchEvent(document, NS_LITERAL_STRING("DOMMetaChanged"));
    }
    // Update referrer policy when it got changed from JS
    SetMetaReferrer(document);
  }

  return nsGenericHTMLElement::AfterSetAttr(
      aNameSpaceID, aName, aValue, aOldValue, aSubjectPrincipal, aNotify);
}

<<<<<<< HEAD
nsresult HTMLMetaElement::BindToTree(nsIDocument* aDocument,
                                     nsIContent* aParent,
                                     nsIContent* aBindingParent) {
  nsresult rv =
      nsGenericHTMLElement::BindToTree(aDocument, aParent, aBindingParent);
||||||| merged common ancestors
nsresult
HTMLMetaElement::BindToTree(nsIDocument* aDocument, nsIContent* aParent,
                            nsIContent* aBindingParent)
{
  nsresult rv = nsGenericHTMLElement::BindToTree(aDocument, aParent,
                                                 aBindingParent);
=======
nsresult HTMLMetaElement::BindToTree(BindContext& aContext, nsINode& aParent) {
  nsresult rv = nsGenericHTMLElement::BindToTree(aContext, aParent);
>>>>>>> upstream-releases
  NS_ENSURE_SUCCESS(rv, rv);
<<<<<<< HEAD
  if (aDocument && AttrValueIs(kNameSpaceID_None, nsGkAtoms::name,
                               nsGkAtoms::viewport, eIgnoreCase)) {
||||||| merged common ancestors
  if (aDocument &&
      AttrValueIs(kNameSpaceID_None, nsGkAtoms::name, nsGkAtoms::viewport, eIgnoreCase)) {
=======
  if (!IsInUncomposedDoc()) {
    return rv;
  }
  Document& doc = aContext.OwnerDoc();
  if (AttrValueIs(kNameSpaceID_None, nsGkAtoms::name, nsGkAtoms::viewport,
                  eIgnoreCase)) {
>>>>>>> upstream-releases
    nsAutoString content;
    GetContent(content);
    nsContentUtils::ProcessViewportInfo(&doc, content);
  }

<<<<<<< HEAD
  if (StaticPrefs::security_csp_enable() && aDocument &&
      !aDocument->IsLoadedAsData() &&
      AttrValueIs(kNameSpaceID_None, nsGkAtoms::httpEquiv, nsGkAtoms::headerCSP,
                  eIgnoreCase)) {
    // only accept <meta http-equiv="Content-Security-Policy" content=""> if it
    // appears in the <head> element.
    Element* headElt = aDocument->GetHeadElement();
||||||| merged common ancestors
  if (StaticPrefs::security_csp_enable() && aDocument &&
      !aDocument->IsLoadedAsData() &&
      AttrValueIs(kNameSpaceID_None, nsGkAtoms::httpEquiv, nsGkAtoms::headerCSP, eIgnoreCase)) {

    // only accept <meta http-equiv="Content-Security-Policy" content=""> if it appears
    // in the <head> element.
    Element* headElt = aDocument->GetHeadElement();
=======
  if (StaticPrefs::security_csp_enable() && !doc.IsLoadedAsData() &&
      AttrValueIs(kNameSpaceID_None, nsGkAtoms::httpEquiv, nsGkAtoms::headerCSP,
                  eIgnoreCase)) {
    // only accept <meta http-equiv="Content-Security-Policy" content=""> if it
    // appears in the <head> element.
    Element* headElt = doc.GetHeadElement();
>>>>>>> upstream-releases
    if (headElt && nsContentUtils::ContentIsDescendantOf(this, headElt)) {
      nsAutoString content;
      GetContent(content);
      content =
          nsContentUtils::TrimWhitespace<nsContentUtils::IsHTMLWhitespace>(
              content);

      if (nsCOMPtr<nsIContentSecurityPolicy> csp = doc.GetCsp()) {
        if (LOG_ENABLED()) {
          nsAutoCString documentURIspec;
          if (nsIURI* documentURI = doc.GetDocumentURI()) {
            documentURI->GetAsciiSpec(documentURIspec);
          }

<<<<<<< HEAD
          LOG(
              ("HTMLMetaElement %p sets CSP '%s' on document=%p, "
               "document-uri=%s",
               this, NS_ConvertUTF16toUTF8(content).get(), aDocument,
               documentURIspec.get()));
||||||| merged common ancestors
          LOG(("HTMLMetaElement %p sets CSP '%s' on document=%p, document-uri=%s",
               this, NS_ConvertUTF16toUTF8(content).get(), aDocument, documentURIspec.get()));
=======
          LOG(
              ("HTMLMetaElement %p sets CSP '%s' on document=%p, "
               "document-uri=%s",
               this, NS_ConvertUTF16toUTF8(content).get(), &doc,
               documentURIspec.get()));
>>>>>>> upstream-releases
        }

        // Multiple CSPs (delivered through either header of meta tag) need to
        // be joined together, see:
        // https://w3c.github.io/webappsec/specs/content-security-policy/#delivery-html-meta-element
        rv =
            csp->AppendPolicy(content,
                              false,  // csp via meta tag can not be report only
                              true);  // delivered through the meta tag
        NS_ENSURE_SUCCESS(rv, rv);
        if (nsPIDOMWindowInner* inner = doc.GetInnerWindow()) {
          inner->SetCsp(csp);
        }
        doc.ApplySettingsFromCSP(false);
      }
    }
  }

  // Referrer Policy spec requires a <meta name="referrer" tag to be in the
  // <head> element.
  SetMetaReferrer(&doc);
  CreateAndDispatchEvent(&doc, NS_LITERAL_STRING("DOMMetaAdded"));
  return rv;
}

<<<<<<< HEAD
void HTMLMetaElement::UnbindFromTree(bool aDeep, bool aNullParent) {
  nsCOMPtr<nsIDocument> oldDoc = GetUncomposedDoc();
||||||| merged common ancestors
void
HTMLMetaElement::UnbindFromTree(bool aDeep, bool aNullParent)
{
  nsCOMPtr<nsIDocument> oldDoc = GetUncomposedDoc();
=======
void HTMLMetaElement::UnbindFromTree(bool aNullParent) {
  nsCOMPtr<Document> oldDoc = GetUncomposedDoc();
>>>>>>> upstream-releases
  CreateAndDispatchEvent(oldDoc, NS_LITERAL_STRING("DOMMetaRemoved"));
  nsGenericHTMLElement::UnbindFromTree(aNullParent);
}

<<<<<<< HEAD
void HTMLMetaElement::CreateAndDispatchEvent(nsIDocument* aDoc,
                                             const nsAString& aEventName) {
  if (!aDoc) return;
||||||| merged common ancestors
void
HTMLMetaElement::CreateAndDispatchEvent(nsIDocument* aDoc,
                                        const nsAString& aEventName)
{
  if (!aDoc)
    return;
=======
void HTMLMetaElement::CreateAndDispatchEvent(Document* aDoc,
                                             const nsAString& aEventName) {
  if (!aDoc) return;
>>>>>>> upstream-releases

  RefPtr<AsyncEventDispatcher> asyncDispatcher = new AsyncEventDispatcher(
      this, aEventName, CanBubble::eYes, ChromeOnlyDispatch::eYes);
  asyncDispatcher->RunDOMEventWhenSafe();
}

JSObject* HTMLMetaElement::WrapNode(JSContext* aCx,
                                    JS::Handle<JSObject*> aGivenProto) {
  return HTMLMetaElement_Binding::Wrap(aCx, this, aGivenProto);
}

}  // namespace dom
}  // namespace mozilla
