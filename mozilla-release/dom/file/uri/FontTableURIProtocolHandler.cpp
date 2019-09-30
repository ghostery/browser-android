/* -*- Mode: C++; tab-width: 8; indent-tabs-mode: nil; c-basic-offset: 2 -*- */
/* vim: set ts=8 sts=2 et sw=2 tw=80: */
/* This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/. */

#include "FontTableURIProtocolHandler.h"
#include "nsIURIMutator.h"
#include "nsIUUIDGenerator.h"
#include "nsNetUtil.h"
#include "nsSimpleURI.h"

using namespace mozilla;
using namespace mozilla::dom;

<<<<<<< HEAD
/* static */ nsresult FontTableURIProtocolHandler::GenerateURIString(
    nsACString &aUri) {
||||||| merged common ancestors
/* static */ nsresult
FontTableURIProtocolHandler::GenerateURIString(nsACString& aUri)
{
=======
/* static */
nsresult FontTableURIProtocolHandler::GenerateURIString(nsACString& aUri) {
>>>>>>> upstream-releases
  nsresult rv;
  nsCOMPtr<nsIUUIDGenerator> uuidgen =
      do_GetService("@mozilla.org/uuid-generator;1", &rv);
  NS_ENSURE_SUCCESS(rv, rv);

  nsID id;
  rv = uuidgen->GenerateUUIDInPlace(&id);
  NS_ENSURE_SUCCESS(rv, rv);

  char chars[NSID_LENGTH];
  id.ToProvidedString(chars);

  aUri = FONTTABLEURI_SCHEME;
  aUri.Append(':');

  aUri += Substring(chars + 1, chars + NSID_LENGTH - 2);

  return NS_OK;
}

FontTableURIProtocolHandler::FontTableURIProtocolHandler() = default;
FontTableURIProtocolHandler::~FontTableURIProtocolHandler() = default;

NS_IMPL_ISUPPORTS(FontTableURIProtocolHandler, nsIProtocolHandler,
                  nsISupportsWeakReference)

NS_IMETHODIMP
<<<<<<< HEAD
FontTableURIProtocolHandler::GetDefaultPort(int32_t *result) {
||||||| merged common ancestors
FontTableURIProtocolHandler::GetDefaultPort(int32_t *result)
{
=======
FontTableURIProtocolHandler::GetDefaultPort(int32_t* result) {
>>>>>>> upstream-releases
  *result = -1;
  return NS_OK;
}

NS_IMETHODIMP
<<<<<<< HEAD
FontTableURIProtocolHandler::GetProtocolFlags(uint32_t *result) {
||||||| merged common ancestors
FontTableURIProtocolHandler::GetProtocolFlags(uint32_t *result)
{
=======
FontTableURIProtocolHandler::GetProtocolFlags(uint32_t* result) {
>>>>>>> upstream-releases
  *result = URI_NORELATIVE | URI_NOAUTH | URI_LOADABLE_BY_SUBSUMERS |
            URI_NON_PERSISTABLE | URI_IS_LOCAL_RESOURCE;
  return NS_OK;
}

NS_IMETHODIMP
<<<<<<< HEAD
FontTableURIProtocolHandler::GetFlagsForURI(nsIURI *aURI, uint32_t *aResult) {
||||||| merged common ancestors
FontTableURIProtocolHandler::GetFlagsForURI(nsIURI *aURI, uint32_t *aResult)
{
=======
FontTableURIProtocolHandler::GetFlagsForURI(nsIURI* aURI, uint32_t* aResult) {
>>>>>>> upstream-releases
  return FontTableURIProtocolHandler::GetProtocolFlags(aResult);
}

<<<<<<< HEAD
NS_IMETHODIMP
FontTableURIProtocolHandler::NewChannel2(nsIURI *uri, nsILoadInfo *aLoadInfo,
                                         nsIChannel **result) {
  return NS_ERROR_DOM_BAD_URI;
}

||||||| merged common ancestors

NS_IMETHODIMP
FontTableURIProtocolHandler::NewChannel2(nsIURI* uri,
                                         nsILoadInfo* aLoadInfo,
                                         nsIChannel** result)
{
  return NS_ERROR_DOM_BAD_URI;
}

=======
>>>>>>> upstream-releases
NS_IMETHODIMP
<<<<<<< HEAD
FontTableURIProtocolHandler::NewChannel(nsIURI *uri, nsIChannel **result) {
||||||| merged common ancestors
FontTableURIProtocolHandler::NewChannel(nsIURI* uri, nsIChannel* *result)
{
=======
FontTableURIProtocolHandler::NewChannel(nsIURI* uri, nsILoadInfo* aLoadInfo,
                                        nsIChannel** result) {
>>>>>>> upstream-releases
  return NS_ERROR_DOM_BAD_URI;
}

NS_IMETHODIMP
<<<<<<< HEAD
FontTableURIProtocolHandler::AllowPort(int32_t port, const char *scheme,
                                       bool *_retval) {
||||||| merged common ancestors
FontTableURIProtocolHandler::AllowPort(int32_t port, const char *scheme,
                                       bool *_retval)
{
=======
FontTableURIProtocolHandler::AllowPort(int32_t port, const char* scheme,
                                       bool* _retval) {
>>>>>>> upstream-releases
  *_retval = false;
  return NS_OK;
}

NS_IMETHODIMP
<<<<<<< HEAD
FontTableURIProtocolHandler::GetScheme(nsACString &result) {
||||||| merged common ancestors
FontTableURIProtocolHandler::GetScheme(nsACString &result)
{
=======
FontTableURIProtocolHandler::GetScheme(nsACString& result) {
>>>>>>> upstream-releases
  result.AssignLiteral(FONTTABLEURI_SCHEME);
  return NS_OK;
}
<<<<<<< HEAD

NS_IMETHODIMP
FontTableURIProtocolHandler::NewURI(const nsACString &aSpec,
                                    const char *aCharset, nsIURI *aBaseURI,
                                    nsIURI **aResult) {
  nsresult rv;
  nsCOMPtr<nsIURI> uri;

  // Either you got here via a ref or a fonttable: uri
  if (aSpec.Length() && aSpec.CharAt(0) == '#') {
    rv = NS_MutateURI(aBaseURI).SetRef(aSpec).Finalize(uri);
    NS_ENSURE_SUCCESS(rv, rv);
  } else {
    // Relative URIs (other than #ref) are not meaningful within the
    // fonttable: scheme.
    // If aSpec is a relative URI -other- than a bare #ref,
    // this will leave uri empty, and we'll return a failure code below.
    rv = NS_MutateURI(new mozilla::net::nsSimpleURI::Mutator())
             .SetSpec(aSpec)
             .Finalize(uri);
    NS_ENSURE_SUCCESS(rv, rv);
  }

  bool schemeIs;
  if (NS_FAILED(uri->SchemeIs(FONTTABLEURI_SCHEME, &schemeIs)) || !schemeIs) {
    NS_WARNING("Non-fonttable spec in FontTableURIProtocolHandler");
    return NS_ERROR_NOT_AVAILABLE;
  }

  uri.forget(aResult);
  return NS_OK;
}

#define NS_FONTTABLEPROTOCOLHANDLER_CID              \
  {                                                  \
    0x3fc8f04e, 0xd719, 0x43ca, {                    \
      0x9a, 0xd0, 0x18, 0xee, 0x32, 0x02, 0x11, 0xf2 \
    }                                                \
  }

NS_GENERIC_FACTORY_CONSTRUCTOR(FontTableURIProtocolHandler)

NS_DEFINE_NAMED_CID(NS_FONTTABLEPROTOCOLHANDLER_CID);

static const mozilla::Module::CIDEntry FontTableURIProtocolHandlerCIDs[] = {
    {&kNS_FONTTABLEPROTOCOLHANDLER_CID, false, nullptr,
     FontTableURIProtocolHandlerConstructor},
    {nullptr}};

static const mozilla::Module::ContractIDEntry
    FontTableURIProtocolHandlerContracts[] = {
        {NS_NETWORK_PROTOCOL_CONTRACTID_PREFIX FONTTABLEURI_SCHEME,
         &kNS_FONTTABLEPROTOCOLHANDLER_CID},
        {nullptr}};

static const mozilla::Module FontTableURIProtocolHandlerModule = {
    mozilla::Module::kVersion, FontTableURIProtocolHandlerCIDs,
    FontTableURIProtocolHandlerContracts};

NSMODULE_DEFN(FontTableURIProtocolHandler) = &FontTableURIProtocolHandlerModule;
||||||| merged common ancestors

NS_IMETHODIMP
FontTableURIProtocolHandler::NewURI(const nsACString& aSpec,
                                    const char *aCharset,
                                    nsIURI *aBaseURI,
                                    nsIURI **aResult)
{
  nsresult rv;
  nsCOMPtr<nsIURI> uri;

  // Either you got here via a ref or a fonttable: uri
  if (aSpec.Length() && aSpec.CharAt(0) == '#') {
    rv = NS_MutateURI(aBaseURI)
           .SetRef(aSpec)
           .Finalize(uri);
    NS_ENSURE_SUCCESS(rv, rv);
  } else {
    // Relative URIs (other than #ref) are not meaningful within the
    // fonttable: scheme.
    // If aSpec is a relative URI -other- than a bare #ref,
    // this will leave uri empty, and we'll return a failure code below.
    rv = NS_MutateURI(new mozilla::net::nsSimpleURI::Mutator())
           .SetSpec(aSpec)
           .Finalize(uri);
    NS_ENSURE_SUCCESS(rv, rv);
  }

  bool schemeIs;
  if (NS_FAILED(uri->SchemeIs(FONTTABLEURI_SCHEME, &schemeIs)) || !schemeIs) {
    NS_WARNING("Non-fonttable spec in FontTableURIProtocolHandler");
    return NS_ERROR_NOT_AVAILABLE;
  }

  uri.forget(aResult);
  return NS_OK;
}

#define NS_FONTTABLEPROTOCOLHANDLER_CID \
{ 0x3fc8f04e, 0xd719, 0x43ca, \
  { 0x9a, 0xd0, 0x18, 0xee, 0x32, 0x02, 0x11, 0xf2 } }

NS_GENERIC_FACTORY_CONSTRUCTOR(FontTableURIProtocolHandler)

NS_DEFINE_NAMED_CID(NS_FONTTABLEPROTOCOLHANDLER_CID);

static const mozilla::Module::CIDEntry FontTableURIProtocolHandlerCIDs[] = {
  { &kNS_FONTTABLEPROTOCOLHANDLER_CID, false, nullptr, FontTableURIProtocolHandlerConstructor },
  { nullptr }
};

static const mozilla::Module::ContractIDEntry FontTableURIProtocolHandlerContracts[] = {
  { NS_NETWORK_PROTOCOL_CONTRACTID_PREFIX FONTTABLEURI_SCHEME, &kNS_FONTTABLEPROTOCOLHANDLER_CID },
  { nullptr }
};

static const mozilla::Module FontTableURIProtocolHandlerModule = {
  mozilla::Module::kVersion,
  FontTableURIProtocolHandlerCIDs,
  FontTableURIProtocolHandlerContracts
};

NSMODULE_DEFN(FontTableURIProtocolHandler) = &FontTableURIProtocolHandlerModule;
=======
>>>>>>> upstream-releases
