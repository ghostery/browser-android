/* -*- Mode: C++; tab-width: 8; indent-tabs-mode: nil; c-basic-offset: 2 -*- */
/* vim: set ts=8 sts=2 et sw=2 tw=80: */
/* This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/. */

#include "Link.h"

#include "mozilla/Components.h"
#include "mozilla/EventStates.h"
#include "mozilla/MemoryReporting.h"
#include "mozilla/dom/Element.h"
#if defined(MOZ_PLACES)
#  include "mozilla/places/History.h"
#else
#  include "mozilla/IHistory.h"
#endif
#include "nsIURL.h"
#include "nsIURIMutator.h"
#include "nsISizeOf.h"
#include "nsIDocShell.h"
#include "nsIPrefetchService.h"
#include "nsStyleLinkElement.h"

#include "nsEscape.h"
#include "nsGkAtoms.h"
#include "nsHTMLDNSPrefetch.h"
#include "nsString.h"
#include "mozAutoDocUpdate.h"

#include "mozilla/Services.h"
#include "nsAttrValueInlines.h"
#include "HTMLLinkElement.h"

namespace mozilla {
namespace dom {

#if defined(MOZ_PLACES)
using places::History;
#endif

<<<<<<< HEAD
Link::Link(Element *aElement)
    : mElement(aElement),
      mLinkState(eLinkState_NotLink),
      mNeedsRegistration(false),
      mRegistered(false),
      mHasPendingLinkUpdate(false),
      mInDNSPrefetch(false),
      mHistory(true) {
||||||| merged common ancestors
Link::Link(Element *aElement)
  : mElement(aElement)
  , mLinkState(eLinkState_NotLink)
  , mNeedsRegistration(false)
  , mRegistered(false)
  , mHasPendingLinkUpdate(false)
  , mInDNSPrefetch(false)
  , mHistory(true)
{
=======
Link::Link(Element* aElement)
    : mElement(aElement),
      mLinkState(eLinkState_NotLink),
      mNeedsRegistration(false),
      mRegistered(false),
      mHasPendingLinkUpdate(false),
      mInDNSPrefetch(false),
      mHistory(true) {
>>>>>>> upstream-releases
  MOZ_ASSERT(mElement, "Must have an element");
}

Link::Link()
    : mElement(nullptr),
      mLinkState(eLinkState_NotLink),
      mNeedsRegistration(false),
      mRegistered(false),
      mHasPendingLinkUpdate(false),
      mInDNSPrefetch(false),
      mHistory(false) {}

Link::~Link() {
  // !mElement is for mock_Link.
  MOZ_ASSERT(!mElement || !mElement->IsInComposedDoc());
  if (IsInDNSPrefetch()) {
    nsHTMLDNSPrefetch::LinkDestroyed(this);
  }
  UnregisterFromHistory();
}

bool Link::ElementHasHref() const {
  return mElement->HasAttr(kNameSpaceID_None, nsGkAtoms::href) ||
         (!mElement->IsHTMLElement() &&
          mElement->HasAttr(kNameSpaceID_XLink, nsGkAtoms::href));
}

void Link::TryDNSPrefetch() {
  MOZ_ASSERT(mElement->IsInComposedDoc());
  if (ElementHasHref() && nsHTMLDNSPrefetch::IsAllowed(mElement->OwnerDoc())) {
    nsHTMLDNSPrefetch::PrefetchLow(this);
  }
}

void Link::CancelDNSPrefetch(nsWrapperCache::FlagsType aDeferredFlag,
                             nsWrapperCache::FlagsType aRequestedFlag) {
  // If prefetch was deferred, clear flag and move on
  if (mElement->HasFlag(aDeferredFlag)) {
    mElement->UnsetFlags(aDeferredFlag);
    // Else if prefetch was requested, clear flag and send cancellation
  } else if (mElement->HasFlag(aRequestedFlag)) {
    mElement->UnsetFlags(aRequestedFlag);
    // Possible that hostname could have changed since binding, but since this
    // covers common cases, most DNS prefetch requests will be canceled
    nsHTMLDNSPrefetch::CancelPrefetchLow(this, NS_ERROR_ABORT);
  }
}

<<<<<<< HEAD
void Link::GetContentPolicyMimeTypeMedia(nsAttrValue &aAsAttr,
                                         nsContentPolicyType &aPolicyType,
                                         nsString &aMimeType,
                                         nsAString &aMedia) {
||||||| merged common ancestors
void
Link::GetContentPolicyMimeTypeMedia(nsAttrValue& aAsAttr,
                                    nsContentPolicyType& aPolicyType,
                                    nsString& aMimeType,
                                    nsAString& aMedia)
{
=======
void Link::GetContentPolicyMimeTypeMedia(nsAttrValue& aAsAttr,
                                         nsContentPolicyType& aPolicyType,
                                         nsString& aMimeType,
                                         nsAString& aMedia) {
>>>>>>> upstream-releases
  nsAutoString as;
  mElement->GetAttr(kNameSpaceID_None, nsGkAtoms::as, as);
  Link::ParseAsValue(as, aAsAttr);
  aPolicyType = AsValueToContentPolicy(aAsAttr);

  nsAutoString type;
  mElement->GetAttr(kNameSpaceID_None, nsGkAtoms::type, type);
  nsAutoString notUsed;
  nsContentUtils::SplitMimeType(type, aMimeType, notUsed);

  mElement->GetAttr(kNameSpaceID_None, nsGkAtoms::media, aMedia);
}

void Link::TryDNSPrefetchOrPreconnectOrPrefetchOrPreloadOrPrerender() {
  MOZ_ASSERT(mElement->IsInComposedDoc());
  if (!ElementHasHref()) {
    return;
  }

  nsAutoString rel;
  if (!mElement->GetAttr(kNameSpaceID_None, nsGkAtoms::rel, rel)) {
    return;
  }

  if (!nsContentUtils::PrefetchPreloadEnabled(
          mElement->OwnerDoc()->GetDocShell())) {
    return;
  }

  uint32_t linkTypes = nsStyleLinkElement::ParseLinkTypes(rel);

  if ((linkTypes & nsStyleLinkElement::ePREFETCH) ||
      (linkTypes & nsStyleLinkElement::eNEXT) ||
<<<<<<< HEAD
      (linkTypes & nsStyleLinkElement::ePRELOAD)) {
    nsCOMPtr<nsIPrefetchService> prefetchService(
        do_GetService(NS_PREFETCHSERVICE_CONTRACTID));
||||||| merged common ancestors
      (linkTypes & nsStyleLinkElement::ePRELOAD)){
    nsCOMPtr<nsIPrefetchService> prefetchService(do_GetService(NS_PREFETCHSERVICE_CONTRACTID));
=======
      (linkTypes & nsStyleLinkElement::ePRELOAD)) {
    nsCOMPtr<nsIPrefetchService> prefetchService(
        components::Prefetch::Service());
>>>>>>> upstream-releases
    if (prefetchService) {
      nsCOMPtr<nsIURI> uri(GetURI());
      if (uri) {
        if (linkTypes & nsStyleLinkElement::ePRELOAD) {
          nsAttrValue asAttr;
          nsContentPolicyType policyType;
          nsAutoString mimeType;
          nsAutoString media;
          GetContentPolicyMimeTypeMedia(asAttr, policyType, mimeType, media);

          if (policyType == nsIContentPolicy::TYPE_INVALID) {
            // Ignore preload with a wrong or empty as attribute.
            return;
          }

          if (!HTMLLinkElement::CheckPreloadAttrs(asAttr, mimeType, media,
                                                  mElement->OwnerDoc())) {
            policyType = nsIContentPolicy::TYPE_INVALID;
          }

          prefetchService->PreloadURI(uri,
                                      mElement->OwnerDoc()->GetDocumentURI(),
                                      mElement, policyType);
        } else {
          prefetchService->PrefetchURI(
              uri, mElement->OwnerDoc()->GetDocumentURI(), mElement,
              linkTypes & nsStyleLinkElement::ePREFETCH);
        }
        return;
      }
    }
  }

  if (linkTypes & nsStyleLinkElement::ePRECONNECT) {
    nsCOMPtr<nsIURI> uri(GetURI());
    if (uri && mElement->OwnerDoc()) {
      mElement->OwnerDoc()->MaybePreconnect(
          uri, Element::AttrValueToCORSMode(
                   mElement->GetParsedAttr(nsGkAtoms::crossorigin)));
      return;
    }
  }

  if (linkTypes & nsStyleLinkElement::eDNS_PREFETCH) {
    if (nsHTMLDNSPrefetch::IsAllowed(mElement->OwnerDoc())) {
      nsHTMLDNSPrefetch::PrefetchLow(this);
    }
  }
}

<<<<<<< HEAD
void Link::UpdatePreload(nsAtom *aName, const nsAttrValue *aValue,
                         const nsAttrValue *aOldValue) {
||||||| merged common ancestors
void
Link::UpdatePreload(nsAtom* aName, const nsAttrValue* aValue,
                    const nsAttrValue* aOldValue)
{
=======
void Link::UpdatePreload(nsAtom* aName, const nsAttrValue* aValue,
                         const nsAttrValue* aOldValue) {
>>>>>>> upstream-releases
  MOZ_ASSERT(mElement->IsInComposedDoc());

  if (!ElementHasHref()) {
    return;
  }

  nsAutoString rel;
  if (!mElement->GetAttr(kNameSpaceID_None, nsGkAtoms::rel, rel)) {
    return;
  }

  if (!nsContentUtils::PrefetchPreloadEnabled(
          mElement->OwnerDoc()->GetDocShell())) {
    return;
  }

  uint32_t linkTypes = nsStyleLinkElement::ParseLinkTypes(rel);

  if (!(linkTypes & nsStyleLinkElement::ePRELOAD)) {
    return;
  }

<<<<<<< HEAD
  nsCOMPtr<nsIPrefetchService> prefetchService(
      do_GetService(NS_PREFETCHSERVICE_CONTRACTID));
||||||| merged common ancestors
  nsCOMPtr<nsIPrefetchService> prefetchService(do_GetService(NS_PREFETCHSERVICE_CONTRACTID));
=======
  nsCOMPtr<nsIPrefetchService> prefetchService(components::Prefetch::Service());
>>>>>>> upstream-releases
  if (!prefetchService) {
    return;
  }

  nsCOMPtr<nsIURI> uri(GetURI());
  if (!uri) {
    return;
  }

  nsAttrValue asAttr;
  nsContentPolicyType asPolicyType;
  nsAutoString mimeType;
  nsAutoString media;
  GetContentPolicyMimeTypeMedia(asAttr, asPolicyType, mimeType, media);

  if (asPolicyType == nsIContentPolicy::TYPE_INVALID) {
    // Ignore preload with a wrong or empty as attribute, but be sure to cancel
    // the old one.
    prefetchService->CancelPrefetchPreloadURI(uri, mElement);
    return;
  }

  nsContentPolicyType policyType = asPolicyType;
  if (!HTMLLinkElement::CheckPreloadAttrs(asAttr, mimeType, media,
                                          mElement->OwnerDoc())) {
    policyType = nsIContentPolicy::TYPE_INVALID;
  }

  if (aName == nsGkAtoms::crossorigin) {
    CORSMode corsMode = Element::AttrValueToCORSMode(aValue);
    CORSMode oldCorsMode = Element::AttrValueToCORSMode(aOldValue);
    if (corsMode != oldCorsMode) {
      prefetchService->CancelPrefetchPreloadURI(uri, mElement);
      prefetchService->PreloadURI(uri, mElement->OwnerDoc()->GetDocumentURI(),
                                  mElement, policyType);
    }
    return;
  }

  nsContentPolicyType oldPolicyType;

  if (aName == nsGkAtoms::as) {
    if (aOldValue) {
      oldPolicyType = AsValueToContentPolicy(*aOldValue);
      if (!HTMLLinkElement::CheckPreloadAttrs(*aOldValue, mimeType, media,
                                              mElement->OwnerDoc())) {
        oldPolicyType = nsIContentPolicy::TYPE_INVALID;
      }
    } else {
      oldPolicyType = nsIContentPolicy::TYPE_INVALID;
    }
  } else if (aName == nsGkAtoms::type) {
    nsAutoString oldType;
    nsAutoString notUsed;
    if (aOldValue) {
      aOldValue->ToString(oldType);
    } else {
      oldType = EmptyString();
    }
    nsAutoString oldMimeType;
    nsContentUtils::SplitMimeType(oldType, oldMimeType, notUsed);
    if (HTMLLinkElement::CheckPreloadAttrs(asAttr, oldMimeType, media,
                                           mElement->OwnerDoc())) {
      oldPolicyType = asPolicyType;
    } else {
      oldPolicyType = nsIContentPolicy::TYPE_INVALID;
    }
  } else {
    MOZ_ASSERT(aName == nsGkAtoms::media);
    nsAutoString oldMedia;
    if (aOldValue) {
      aOldValue->ToString(oldMedia);
    } else {
      oldMedia = EmptyString();
    }
    if (HTMLLinkElement::CheckPreloadAttrs(asAttr, mimeType, oldMedia,
                                           mElement->OwnerDoc())) {
      oldPolicyType = asPolicyType;
    } else {
      oldPolicyType = nsIContentPolicy::TYPE_INVALID;
    }
  }

  if ((policyType != oldPolicyType) &&
      (oldPolicyType != nsIContentPolicy::TYPE_INVALID)) {
    prefetchService->CancelPrefetchPreloadURI(uri, mElement);
  }

  // Trigger a new preload if the policy type has changed.
  // Also trigger load if the new policy type is invalid, this will only
  // trigger an error event.
  if ((policyType != oldPolicyType) ||
      (policyType == nsIContentPolicy::TYPE_INVALID)) {
    prefetchService->PreloadURI(uri, mElement->OwnerDoc()->GetDocumentURI(),
                                mElement, policyType);
  }
}

<<<<<<< HEAD
void Link::CancelPrefetchOrPreload() {
  nsCOMPtr<nsIPrefetchService> prefetchService(
      do_GetService(NS_PREFETCHSERVICE_CONTRACTID));
||||||| merged common ancestors
void
Link::CancelPrefetchOrPreload()
{
  nsCOMPtr<nsIPrefetchService> prefetchService(do_GetService(NS_PREFETCHSERVICE_CONTRACTID));
=======
void Link::CancelPrefetchOrPreload() {
  nsCOMPtr<nsIPrefetchService> prefetchService(components::Prefetch::Service());
>>>>>>> upstream-releases
  if (prefetchService) {
    nsCOMPtr<nsIURI> uri(GetURI());
    if (uri) {
      prefetchService->CancelPrefetchPreloadURI(uri, mElement);
    }
  }
}

void Link::SetLinkState(nsLinkState aState) {
  NS_ASSERTION(mRegistered, "Setting the link state of an unregistered Link!");
  NS_ASSERTION(mLinkState != aState,
               "Setting state to the currently set state!");

  // Set our current state as appropriate.
  mLinkState = aState;

  // Per IHistory interface documentation, we are no longer registered.
  mRegistered = false;

  MOZ_ASSERT(LinkState() == NS_EVENT_STATE_VISITED ||
                 LinkState() == NS_EVENT_STATE_UNVISITED,
             "Unexpected state obtained from LinkState()!");

  // Tell the element to update its visited state
  mElement->UpdateState(true);
}

EventStates Link::LinkState() const {
  // We are a constant method, but we are just lazily doing things and have to
  // track that state.  Cast away that constness!
  Link* self = const_cast<Link*>(this);

  Element* element = self->mElement;

  // If we have not yet registered for notifications and need to,
  // due to our href changing, register now!
  if (!mRegistered && mNeedsRegistration && element->IsInComposedDoc() &&
      !HasPendingLinkUpdate()) {
    // Only try and register once.
    self->mNeedsRegistration = false;

    nsCOMPtr<nsIURI> hrefURI(GetURI());

    // Assume that we are not visited until we are told otherwise.
    self->mLinkState = eLinkState_Unvisited;

    // Make sure the href attribute has a valid link (bug 23209).
    // If we have a good href, register with History if available.
    if (mHistory && hrefURI) {
#ifdef ANDROID
      nsCOMPtr<IHistory> history = services::GetHistoryService();
#elif defined(MOZ_PLACES)
      History *history = History::GetService();
#else
      nsCOMPtr<IHistory> history;
#endif
      if (history) {
        nsresult rv = history->RegisterVisitedCallback(hrefURI, self);
        if (NS_SUCCEEDED(rv)) {
          self->mRegistered = true;

          // And make sure we are in the document's link map.
          element->GetComposedDoc()->AddStyleRelevantLink(self);
        }
      }
    }
  }

  // Otherwise, return our known state.
  if (mLinkState == eLinkState_Visited) {
    return NS_EVENT_STATE_VISITED;
  }

  if (mLinkState == eLinkState_Unvisited) {
    return NS_EVENT_STATE_UNVISITED;
  }

  return EventStates();
}

<<<<<<< HEAD
nsIURI *Link::GetURI() const {
||||||| merged common ancestors
nsIURI*
Link::GetURI() const
{
=======
nsIURI* Link::GetURI() const {
>>>>>>> upstream-releases
  // If we have this URI cached, use it.
  if (mCachedURI) {
    return mCachedURI;
  }

  // Otherwise obtain it.
  Link* self = const_cast<Link*>(this);
  Element* element = self->mElement;
  mCachedURI = element->GetHrefURI();

  return mCachedURI;
}

<<<<<<< HEAD
void Link::SetProtocol(const nsAString &aProtocol) {
||||||| merged common ancestors
void
Link::SetProtocol(const nsAString &aProtocol)
{
=======
void Link::SetProtocol(const nsAString& aProtocol) {
>>>>>>> upstream-releases
  nsCOMPtr<nsIURI> uri(GetURI());
  if (!uri) {
    // Ignore failures to be compatible with NS4.
    return;
  }

  nsAString::const_iterator start, end;
  aProtocol.BeginReading(start);
  aProtocol.EndReading(end);
  nsAString::const_iterator iter(start);
  (void)FindCharInReadable(':', iter, end);
  nsresult rv = NS_MutateURI(uri)
                    .SetScheme(NS_ConvertUTF16toUTF8(Substring(start, iter)))
                    .Finalize(uri);
  if (NS_FAILED(rv)) {
    return;
  }

  SetHrefAttribute(uri);
}

<<<<<<< HEAD
void Link::SetPassword(const nsAString &aPassword) {
||||||| merged common ancestors
void
Link::SetPassword(const nsAString &aPassword)
{
=======
void Link::SetPassword(const nsAString& aPassword) {
>>>>>>> upstream-releases
  nsCOMPtr<nsIURI> uri(GetURI());
  if (!uri) {
    // Ignore failures to be compatible with NS4.
    return;
  }

  nsresult rv = NS_MutateURI(uri)
                    .SetPassword(NS_ConvertUTF16toUTF8(aPassword))
                    .Finalize(uri);
  if (NS_SUCCEEDED(rv)) {
    SetHrefAttribute(uri);
  }
}

<<<<<<< HEAD
void Link::SetUsername(const nsAString &aUsername) {
||||||| merged common ancestors
void
Link::SetUsername(const nsAString &aUsername)
{
=======
void Link::SetUsername(const nsAString& aUsername) {
>>>>>>> upstream-releases
  nsCOMPtr<nsIURI> uri(GetURI());
  if (!uri) {
    // Ignore failures to be compatible with NS4.
    return;
  }

  nsresult rv = NS_MutateURI(uri)
                    .SetUsername(NS_ConvertUTF16toUTF8(aUsername))
                    .Finalize(uri);
  if (NS_SUCCEEDED(rv)) {
    SetHrefAttribute(uri);
  }
}

<<<<<<< HEAD
void Link::SetHost(const nsAString &aHost) {
||||||| merged common ancestors
void
Link::SetHost(const nsAString &aHost)
{
=======
void Link::SetHost(const nsAString& aHost) {
>>>>>>> upstream-releases
  nsCOMPtr<nsIURI> uri(GetURI());
  if (!uri) {
    // Ignore failures to be compatible with NS4.
    return;
  }

  nsresult rv =
      NS_MutateURI(uri).SetHostPort(NS_ConvertUTF16toUTF8(aHost)).Finalize(uri);
  if (NS_FAILED(rv)) {
    return;
  }
  SetHrefAttribute(uri);
}

<<<<<<< HEAD
void Link::SetHostname(const nsAString &aHostname) {
||||||| merged common ancestors
void
Link::SetHostname(const nsAString &aHostname)
{
=======
void Link::SetHostname(const nsAString& aHostname) {
>>>>>>> upstream-releases
  nsCOMPtr<nsIURI> uri(GetURI());
  if (!uri) {
    // Ignore failures to be compatible with NS4.
    return;
  }

  nsresult rv =
      NS_MutateURI(uri).SetHost(NS_ConvertUTF16toUTF8(aHostname)).Finalize(uri);
  if (NS_FAILED(rv)) {
    return;
  }
  SetHrefAttribute(uri);
}

<<<<<<< HEAD
void Link::SetPathname(const nsAString &aPathname) {
||||||| merged common ancestors
void
Link::SetPathname(const nsAString &aPathname)
{
=======
void Link::SetPathname(const nsAString& aPathname) {
>>>>>>> upstream-releases
  nsCOMPtr<nsIURI> uri(GetURI());
  nsCOMPtr<nsIURL> url(do_QueryInterface(uri));
  if (!url) {
    // Ignore failures to be compatible with NS4.
    return;
  }

  nsresult rv = NS_MutateURI(uri)
                    .SetFilePath(NS_ConvertUTF16toUTF8(aPathname))
                    .Finalize(uri);
  if (NS_FAILED(rv)) {
    return;
  }
  SetHrefAttribute(uri);
}

<<<<<<< HEAD
void Link::SetSearch(const nsAString &aSearch) {
||||||| merged common ancestors
void
Link::SetSearch(const nsAString& aSearch)
{
=======
void Link::SetSearch(const nsAString& aSearch) {
>>>>>>> upstream-releases
  nsCOMPtr<nsIURI> uri(GetURI());
  nsCOMPtr<nsIURL> url(do_QueryInterface(uri));
  if (!url) {
    // Ignore failures to be compatible with NS4.
    return;
  }

  auto encoding = mElement->OwnerDoc()->GetDocumentCharacterSet();
  nsresult rv =
      NS_MutateURI(uri)
          .SetQueryWithEncoding(NS_ConvertUTF16toUTF8(aSearch), encoding)
          .Finalize(uri);
  if (NS_FAILED(rv)) {
    return;
  }
  SetHrefAttribute(uri);
}

<<<<<<< HEAD
void Link::SetPort(const nsAString &aPort) {
||||||| merged common ancestors
void
Link::SetPort(const nsAString &aPort)
{
=======
void Link::SetPort(const nsAString& aPort) {
>>>>>>> upstream-releases
  nsCOMPtr<nsIURI> uri(GetURI());
  if (!uri) {
    // Ignore failures to be compatible with NS4.
    return;
  }

  nsresult rv;
  nsAutoString portStr(aPort);

  // nsIURI uses -1 as default value.
  int32_t port = -1;
  if (!aPort.IsEmpty()) {
    port = portStr.ToInteger(&rv);
    if (NS_FAILED(rv)) {
      return;
    }
  }

  rv = NS_MutateURI(uri).SetPort(port).Finalize(uri);
  if (NS_FAILED(rv)) {
    return;
  }
  SetHrefAttribute(uri);
}

<<<<<<< HEAD
void Link::SetHash(const nsAString &aHash) {
||||||| merged common ancestors
void
Link::SetHash(const nsAString &aHash)
{
=======
void Link::SetHash(const nsAString& aHash) {
>>>>>>> upstream-releases
  nsCOMPtr<nsIURI> uri(GetURI());
  if (!uri) {
    // Ignore failures to be compatible with NS4.
    return;
  }

  nsresult rv =
      NS_MutateURI(uri).SetRef(NS_ConvertUTF16toUTF8(aHash)).Finalize(uri);
  if (NS_FAILED(rv)) {
    return;
  }

  SetHrefAttribute(uri);
}

<<<<<<< HEAD
void Link::GetOrigin(nsAString &aOrigin) {
||||||| merged common ancestors
void
Link::GetOrigin(nsAString &aOrigin)
{
=======
void Link::GetOrigin(nsAString& aOrigin) {
>>>>>>> upstream-releases
  aOrigin.Truncate();

  nsCOMPtr<nsIURI> uri(GetURI());
  if (!uri) {
    return;
  }

  nsString origin;
  nsContentUtils::GetUTFOrigin(uri, origin);
  aOrigin.Assign(origin);
}

<<<<<<< HEAD
void Link::GetProtocol(nsAString &_protocol) {
||||||| merged common ancestors
void
Link::GetProtocol(nsAString &_protocol)
{
=======
void Link::GetProtocol(nsAString& _protocol) {
>>>>>>> upstream-releases
  nsCOMPtr<nsIURI> uri(GetURI());
  if (!uri) {
    _protocol.AssignLiteral("http");
  } else {
    nsAutoCString scheme;
    (void)uri->GetScheme(scheme);
    CopyASCIItoUTF16(scheme, _protocol);
  }
  _protocol.Append(char16_t(':'));
}

<<<<<<< HEAD
void Link::GetUsername(nsAString &aUsername) {
||||||| merged common ancestors
void
Link::GetUsername(nsAString& aUsername)
{
=======
void Link::GetUsername(nsAString& aUsername) {
>>>>>>> upstream-releases
  aUsername.Truncate();

  nsCOMPtr<nsIURI> uri(GetURI());
  if (!uri) {
    return;
  }

  nsAutoCString username;
  uri->GetUsername(username);
  CopyASCIItoUTF16(username, aUsername);
}

<<<<<<< HEAD
void Link::GetPassword(nsAString &aPassword) {
||||||| merged common ancestors
void
Link::GetPassword(nsAString &aPassword)
{
=======
void Link::GetPassword(nsAString& aPassword) {
>>>>>>> upstream-releases
  aPassword.Truncate();

  nsCOMPtr<nsIURI> uri(GetURI());
  if (!uri) {
    return;
  }

  nsAutoCString password;
  uri->GetPassword(password);
  CopyASCIItoUTF16(password, aPassword);
}

<<<<<<< HEAD
void Link::GetHost(nsAString &_host) {
||||||| merged common ancestors
void
Link::GetHost(nsAString &_host)
{
=======
void Link::GetHost(nsAString& _host) {
>>>>>>> upstream-releases
  _host.Truncate();

  nsCOMPtr<nsIURI> uri(GetURI());
  if (!uri) {
    // Do not throw!  Not having a valid URI should result in an empty string.
    return;
  }

  nsAutoCString hostport;
  nsresult rv = uri->GetHostPort(hostport);
  if (NS_SUCCEEDED(rv)) {
    CopyUTF8toUTF16(hostport, _host);
  }
}

<<<<<<< HEAD
void Link::GetHostname(nsAString &_hostname) {
||||||| merged common ancestors
void
Link::GetHostname(nsAString &_hostname)
{
=======
void Link::GetHostname(nsAString& _hostname) {
>>>>>>> upstream-releases
  _hostname.Truncate();

  nsCOMPtr<nsIURI> uri(GetURI());
  if (!uri) {
    // Do not throw!  Not having a valid URI should result in an empty string.
    return;
  }

  nsContentUtils::GetHostOrIPv6WithBrackets(uri, _hostname);
}

<<<<<<< HEAD
void Link::GetPathname(nsAString &_pathname) {
||||||| merged common ancestors
void
Link::GetPathname(nsAString &_pathname)
{
=======
void Link::GetPathname(nsAString& _pathname) {
>>>>>>> upstream-releases
  _pathname.Truncate();

  nsCOMPtr<nsIURI> uri(GetURI());
  nsCOMPtr<nsIURL> url(do_QueryInterface(uri));
  if (!url) {
    // Do not throw!  Not having a valid URI or URL should result in an empty
    // string.
    return;
  }

  nsAutoCString file;
  nsresult rv = url->GetFilePath(file);
  if (NS_SUCCEEDED(rv)) {
    CopyUTF8toUTF16(file, _pathname);
  }
}

<<<<<<< HEAD
void Link::GetSearch(nsAString &_search) {
||||||| merged common ancestors
void
Link::GetSearch(nsAString &_search)
{
=======
void Link::GetSearch(nsAString& _search) {
>>>>>>> upstream-releases
  _search.Truncate();

  nsCOMPtr<nsIURI> uri(GetURI());
  nsCOMPtr<nsIURL> url(do_QueryInterface(uri));
  if (!url) {
    // Do not throw!  Not having a valid URI or URL should result in an empty
    // string.
    return;
  }

  nsAutoCString search;
  nsresult rv = url->GetQuery(search);
  if (NS_SUCCEEDED(rv) && !search.IsEmpty()) {
    _search.Assign(u'?');
    AppendUTF8toUTF16(search, _search);
  }
}

<<<<<<< HEAD
void Link::GetPort(nsAString &_port) {
||||||| merged common ancestors
void
Link::GetPort(nsAString &_port)
{
=======
void Link::GetPort(nsAString& _port) {
>>>>>>> upstream-releases
  _port.Truncate();

  nsCOMPtr<nsIURI> uri(GetURI());
  if (!uri) {
    // Do not throw!  Not having a valid URI should result in an empty string.
    return;
  }

  int32_t port;
  nsresult rv = uri->GetPort(&port);
  // Note that failure to get the port from the URI is not necessarily a bad
  // thing.  Some URIs do not have a port.
  if (NS_SUCCEEDED(rv) && port != -1) {
    nsAutoString portStr;
    portStr.AppendInt(port, 10);
    _port.Assign(portStr);
  }
}

<<<<<<< HEAD
void Link::GetHash(nsAString &_hash) {
||||||| merged common ancestors
void
Link::GetHash(nsAString &_hash)
{
=======
void Link::GetHash(nsAString& _hash) {
>>>>>>> upstream-releases
  _hash.Truncate();

  nsCOMPtr<nsIURI> uri(GetURI());
  if (!uri) {
    // Do not throw!  Not having a valid URI should result in an empty
    // string.
    return;
  }

  nsAutoCString ref;
  nsresult rv = uri->GetRef(ref);
  if (NS_SUCCEEDED(rv) && !ref.IsEmpty()) {
    _hash.Assign(char16_t('#'));
    AppendUTF8toUTF16(ref, _hash);
  }
}

void Link::ResetLinkState(bool aNotify, bool aHasHref) {
  nsLinkState defaultState;

  // The default state for links with an href is unvisited.
  if (aHasHref) {
    defaultState = eLinkState_Unvisited;
  } else {
    defaultState = eLinkState_NotLink;
  }

  // If !mNeedsRegstration, then either we've never registered, or we're
  // currently registered; in either case, we should remove ourself
  // from the doc and the history.
  if (!mNeedsRegistration && mLinkState != eLinkState_NotLink) {
    Document* doc = mElement->GetComposedDoc();
    if (doc && (mRegistered || mLinkState == eLinkState_Visited)) {
      // Tell the document to forget about this link if we've registered
      // with it before.
      doc->ForgetLink(this);
    }
  }

  // If we have an href, we should register with the history.
  mNeedsRegistration = aHasHref;

  // If we've cached the URI, reset always invalidates it.
  UnregisterFromHistory();
  mCachedURI = nullptr;

  // Update our state back to the default.
  mLinkState = defaultState;

  // We have to be very careful here: if aNotify is false we do NOT
  // want to call UpdateState, because that will call into LinkState()
  // and try to start off loads, etc.  But ResetLinkState is called
  // with aNotify false when things are in inconsistent states, so
  // we'll get confused in that situation.  Instead, just silently
  // update the link state on mElement. Since we might have set the
  // link state to unvisited, make sure to update with that state if
  // required.
  if (aNotify) {
    mElement->UpdateState(aNotify);
  } else {
    if (mLinkState == eLinkState_Unvisited) {
      mElement->UpdateLinkState(NS_EVENT_STATE_UNVISITED);
    } else {
      mElement->UpdateLinkState(EventStates());
    }
  }
}

void Link::UnregisterFromHistory() {
  // If we are not registered, we have nothing to do.
  if (!mRegistered) {
    return;
  }

  // And tell History to stop tracking us.
  if (mHistory && mCachedURI) {
#ifdef ANDROID
    nsCOMPtr<IHistory> history = services::GetHistoryService();
#elif defined(MOZ_PLACES)
    History *history = History::GetService();
#else
    nsCOMPtr<IHistory> history;
#endif
    if (history) {
      nsresult rv = history->UnregisterVisitedCallback(mCachedURI, this);
      NS_ASSERTION(NS_SUCCEEDED(rv),
                   "This should only fail if we misuse the API!");
      if (NS_SUCCEEDED(rv)) {
        mRegistered = false;
      }
    }
  }
}

<<<<<<< HEAD
void Link::SetHrefAttribute(nsIURI *aURI) {
||||||| merged common ancestors
void
Link::SetHrefAttribute(nsIURI *aURI)
{
=======
void Link::SetHrefAttribute(nsIURI* aURI) {
>>>>>>> upstream-releases
  NS_ASSERTION(aURI, "Null URI is illegal!");

  // if we change this code to not reserialize we need to do something smarter
  // in SetProtocol because changing the protocol of an URI can change the
  // "nature" of the nsIURL/nsIURI implementation.
  nsAutoCString href;
  (void)aURI->GetSpec(href);
  (void)mElement->SetAttr(kNameSpaceID_None, nsGkAtoms::href,
                          NS_ConvertUTF8toUTF16(href), true);
}

<<<<<<< HEAD
size_t Link::SizeOfExcludingThis(mozilla::SizeOfState &aState) const {
||||||| merged common ancestors
size_t
Link::SizeOfExcludingThis(mozilla::SizeOfState& aState) const
{
=======
size_t Link::SizeOfExcludingThis(mozilla::SizeOfState& aState) const {
>>>>>>> upstream-releases
  size_t n = 0;

  if (mCachedURI) {
    nsCOMPtr<nsISizeOf> iface = do_QueryInterface(mCachedURI);
    if (iface) {
      n += iface->SizeOfIncludingThis(aState.mMallocSizeOf);
    }
  }

  // The following members don't need to be measured:
  // - mElement, because it is a pointer-to-self used to avoid QIs

  return n;
}

static const nsAttrValue::EnumTable kAsAttributeTable[] = {
<<<<<<< HEAD
    {"", DESTINATION_INVALID},      {"audio", DESTINATION_AUDIO},
    {"font", DESTINATION_FONT},     {"image", DESTINATION_IMAGE},
    {"script", DESTINATION_SCRIPT}, {"style", DESTINATION_STYLE},
    {"track", DESTINATION_TRACK},   {"video", DESTINATION_VIDEO},
    {"fetch", DESTINATION_FETCH},   {nullptr, 0}};

/* static */ void Link::ParseAsValue(const nsAString &aValue,
                                     nsAttrValue &aResult) {
||||||| merged common ancestors
  { "",              DESTINATION_INVALID       },
  { "audio",         DESTINATION_AUDIO         },
  { "font",          DESTINATION_FONT          },
  { "image",         DESTINATION_IMAGE         },
  { "script",        DESTINATION_SCRIPT        },
  { "style",         DESTINATION_STYLE         },
  { "track",         DESTINATION_TRACK         },
  { "video",         DESTINATION_VIDEO         },
  { "fetch",         DESTINATION_FETCH         },
  { nullptr,         0 }
};


/* static */ void
Link::ParseAsValue(const nsAString& aValue,
                   nsAttrValue& aResult)
{
=======
    {"", DESTINATION_INVALID},      {"audio", DESTINATION_AUDIO},
    {"font", DESTINATION_FONT},     {"image", DESTINATION_IMAGE},
    {"script", DESTINATION_SCRIPT}, {"style", DESTINATION_STYLE},
    {"track", DESTINATION_TRACK},   {"video", DESTINATION_VIDEO},
    {"fetch", DESTINATION_FETCH},   {nullptr, 0}};

/* static */
void Link::ParseAsValue(const nsAString& aValue, nsAttrValue& aResult) {
>>>>>>> upstream-releases
  DebugOnly<bool> success =
      aResult.ParseEnumValue(aValue, kAsAttributeTable, false,
                             // default value is a empty string
                             // if aValue is not a value we
                             // understand
                             &kAsAttributeTable[0]);
  MOZ_ASSERT(success);
}

<<<<<<< HEAD
/* static */ nsContentPolicyType Link::AsValueToContentPolicy(
    const nsAttrValue &aValue) {
  switch (aValue.GetEnumValue()) {
    case DESTINATION_INVALID:
      return nsIContentPolicy::TYPE_INVALID;
    case DESTINATION_AUDIO:
      return nsIContentPolicy::TYPE_INTERNAL_AUDIO;
    case DESTINATION_TRACK:
      return nsIContentPolicy::TYPE_INTERNAL_TRACK;
    case DESTINATION_VIDEO:
      return nsIContentPolicy::TYPE_INTERNAL_VIDEO;
    case DESTINATION_FONT:
      return nsIContentPolicy::TYPE_FONT;
    case DESTINATION_IMAGE:
      return nsIContentPolicy::TYPE_IMAGE;
    case DESTINATION_SCRIPT:
      return nsIContentPolicy::TYPE_SCRIPT;
    case DESTINATION_STYLE:
      return nsIContentPolicy::TYPE_STYLESHEET;
    case DESTINATION_FETCH:
      return nsIContentPolicy::TYPE_OTHER;
||||||| merged common ancestors
/* static */ nsContentPolicyType
Link::AsValueToContentPolicy(const nsAttrValue& aValue)
{
  switch(aValue.GetEnumValue()) {
  case DESTINATION_INVALID:
    return nsIContentPolicy::TYPE_INVALID;
  case DESTINATION_AUDIO:
    return nsIContentPolicy::TYPE_INTERNAL_AUDIO;
  case DESTINATION_TRACK:
    return nsIContentPolicy::TYPE_INTERNAL_TRACK;
  case DESTINATION_VIDEO:
    return nsIContentPolicy::TYPE_INTERNAL_VIDEO;
  case DESTINATION_FONT:
    return nsIContentPolicy::TYPE_FONT;
  case DESTINATION_IMAGE:
    return nsIContentPolicy::TYPE_IMAGE;
  case DESTINATION_SCRIPT:
    return nsIContentPolicy::TYPE_SCRIPT;
  case DESTINATION_STYLE:
    return nsIContentPolicy::TYPE_STYLESHEET;
  case DESTINATION_FETCH:
    return nsIContentPolicy::TYPE_OTHER;
=======
/* static */
nsContentPolicyType Link::AsValueToContentPolicy(const nsAttrValue& aValue) {
  switch (aValue.GetEnumValue()) {
    case DESTINATION_INVALID:
      return nsIContentPolicy::TYPE_INVALID;
    case DESTINATION_AUDIO:
      return nsIContentPolicy::TYPE_INTERNAL_AUDIO;
    case DESTINATION_TRACK:
      return nsIContentPolicy::TYPE_INTERNAL_TRACK;
    case DESTINATION_VIDEO:
      return nsIContentPolicy::TYPE_INTERNAL_VIDEO;
    case DESTINATION_FONT:
      return nsIContentPolicy::TYPE_FONT;
    case DESTINATION_IMAGE:
      return nsIContentPolicy::TYPE_IMAGE;
    case DESTINATION_SCRIPT:
      return nsIContentPolicy::TYPE_SCRIPT;
    case DESTINATION_STYLE:
      return nsIContentPolicy::TYPE_STYLESHEET;
    case DESTINATION_FETCH:
      return nsIContentPolicy::TYPE_OTHER;
>>>>>>> upstream-releases
  }
  return nsIContentPolicy::TYPE_INVALID;
}

}  // namespace dom
}  // namespace mozilla
