/* -*- Mode: C++; tab-width: 4; indent-tabs-mode: nil; c-basic-offset: 2 -*- */
/* vim:set ts=4 sw=2 sts=2 et: */
/* This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/. */

#include "nsViewSourceHandler.h"
#include "nsViewSourceChannel.h"
#include "nsNetUtil.h"
#include "nsSimpleNestedURI.h"

#define VIEW_SOURCE "view-source"

#define DEFAULT_FLAGS \
  (URI_NORELATIVE | URI_NOAUTH | URI_DANGEROUS_TO_LOAD | URI_NON_PERSISTABLE)

namespace mozilla {
namespace net {

////////////////////////////////////////////////////////////////////////////////

NS_IMPL_ISUPPORTS(nsViewSourceHandler, nsIProtocolHandler,
                  nsIProtocolHandlerWithDynamicFlags)

////////////////////////////////////////////////////////////////////////////////
// nsIProtocolHandler methods:

NS_IMETHODIMP
<<<<<<< HEAD
nsViewSourceHandler::GetScheme(nsACString &result) {
  result.AssignLiteral(VIEW_SOURCE);
  return NS_OK;
||||||| merged common ancestors
nsViewSourceHandler::GetScheme(nsACString &result)
{
    result.AssignLiteral(VIEW_SOURCE);
    return NS_OK;
=======
nsViewSourceHandler::GetScheme(nsACString& result) {
  result.AssignLiteral(VIEW_SOURCE);
  return NS_OK;
>>>>>>> upstream-releases
}

NS_IMETHODIMP
<<<<<<< HEAD
nsViewSourceHandler::GetDefaultPort(int32_t *result) {
  *result = -1;
  return NS_OK;
||||||| merged common ancestors
nsViewSourceHandler::GetDefaultPort(int32_t *result)
{
    *result = -1;
    return NS_OK;
=======
nsViewSourceHandler::GetDefaultPort(int32_t* result) {
  *result = -1;
  return NS_OK;
>>>>>>> upstream-releases
}

NS_IMETHODIMP
<<<<<<< HEAD
nsViewSourceHandler::GetProtocolFlags(uint32_t *result) {
  *result = DEFAULT_FLAGS;
  return NS_OK;
||||||| merged common ancestors
nsViewSourceHandler::GetProtocolFlags(uint32_t *result)
{
    *result = DEFAULT_FLAGS;
    return NS_OK;
=======
nsViewSourceHandler::GetProtocolFlags(uint32_t* result) {
  *result = DEFAULT_FLAGS;
  return NS_OK;
>>>>>>> upstream-releases
}

NS_IMETHODIMP
<<<<<<< HEAD
nsViewSourceHandler::GetFlagsForURI(nsIURI *aURI, uint32_t *result) {
  *result = DEFAULT_FLAGS;
  nsCOMPtr<nsINestedURI> nestedURI(do_QueryInterface(aURI));
  if (!nestedURI) {
||||||| merged common ancestors
nsViewSourceHandler::GetFlagsForURI(nsIURI* aURI, uint32_t* result)
{
    *result = DEFAULT_FLAGS;
    nsCOMPtr<nsINestedURI> nestedURI(do_QueryInterface(aURI));
    if (!nestedURI) {
        return NS_OK;
    }

    nsCOMPtr<nsIURI> innerURI;
    nestedURI->GetInnerURI(getter_AddRefs(innerURI));
    nsCOMPtr<nsINetUtil> netUtil = do_GetNetUtil();
    bool isLoadable = false;
    nsresult rv = netUtil->ProtocolHasFlags(innerURI, URI_LOADABLE_BY_ANYONE, &isLoadable);
    NS_ENSURE_SUCCESS(rv, rv);
    if (isLoadable) {
        *result |= URI_LOADABLE_BY_EXTENSIONS;
    }
=======
nsViewSourceHandler::GetFlagsForURI(nsIURI* aURI, uint32_t* result) {
  *result = DEFAULT_FLAGS;
  nsCOMPtr<nsINestedURI> nestedURI(do_QueryInterface(aURI));
  if (!nestedURI) {
>>>>>>> upstream-releases
    return NS_OK;
  }

  nsCOMPtr<nsIURI> innerURI;
  nestedURI->GetInnerURI(getter_AddRefs(innerURI));
  nsCOMPtr<nsINetUtil> netUtil = do_GetNetUtil();
  bool isLoadable = false;
  nsresult rv =
      netUtil->ProtocolHasFlags(innerURI, URI_LOADABLE_BY_ANYONE, &isLoadable);
  NS_ENSURE_SUCCESS(rv, rv);
  if (isLoadable) {
    *result |= URI_LOADABLE_BY_EXTENSIONS;
  }
  return NS_OK;
}

<<<<<<< HEAD
NS_IMETHODIMP
nsViewSourceHandler::NewURI(const nsACString &aSpec, const char *aCharset,
                            nsIURI *aBaseURI, nsIURI **aResult) {
  *aResult = nullptr;
||||||| merged common ancestors
NS_IMETHODIMP
nsViewSourceHandler::NewURI(const nsACString &aSpec,
                            const char *aCharset,
                            nsIURI *aBaseURI,
                            nsIURI **aResult)
{
    *aResult = nullptr;
=======
// static
nsresult nsViewSourceHandler::CreateNewURI(const nsACString& aSpec,
                                           const char* aCharset,
                                           nsIURI* aBaseURI, nsIURI** aResult) {
  *aResult = nullptr;
>>>>>>> upstream-releases

  // Extract inner URL and normalize to ASCII.  This is done to properly
  // support IDN in cases like "view-source:http://www.szalagavató.hu/"

  int32_t colon = aSpec.FindChar(':');
  if (colon == kNotFound) return NS_ERROR_MALFORMED_URI;

  nsCOMPtr<nsIURI> innerURI;
  nsresult rv = NS_NewURI(getter_AddRefs(innerURI), Substring(aSpec, colon + 1),
                          aCharset, aBaseURI);
  if (NS_FAILED(rv)) return rv;

  nsAutoCString asciiSpec;
  rv = innerURI->GetAsciiSpec(asciiSpec);
  if (NS_FAILED(rv)) return rv;

  // put back our scheme and construct a simple-uri wrapper

  asciiSpec.Insert(VIEW_SOURCE ":", 0);

  nsCOMPtr<nsIURI> uri;
  rv = NS_MutateURI(new nsSimpleNestedURI::Mutator())
           .Apply(NS_MutatorMethod(&nsINestedURIMutator::Init, innerURI))
           .SetSpec(asciiSpec)
           .Finalize(uri);
  if (NS_FAILED(rv)) {
    return rv;
<<<<<<< HEAD
  }

  uri.swap(*aResult);
  return rv;
}
||||||| merged common ancestors
}
=======
  }
>>>>>>> upstream-releases

<<<<<<< HEAD
NS_IMETHODIMP
nsViewSourceHandler::NewChannel2(nsIURI *uri, nsILoadInfo *aLoadInfo,
                                 nsIChannel **result) {
  NS_ENSURE_ARG_POINTER(uri);
  nsViewSourceChannel *channel = new nsViewSourceChannel();
  if (!channel) return NS_ERROR_OUT_OF_MEMORY;
  NS_ADDREF(channel);

  nsresult rv = channel->Init(uri);
  if (NS_FAILED(rv)) {
    NS_RELEASE(channel);
    return rv;
  }

  // set the loadInfo on the new channel
  rv = channel->SetLoadInfo(aLoadInfo);
  if (NS_FAILED(rv)) {
    NS_RELEASE(channel);
    return rv;
  }

  *result = static_cast<nsIViewSourceChannel *>(channel);
  return NS_OK;
||||||| merged common ancestors
NS_IMETHODIMP
nsViewSourceHandler::NewChannel2(nsIURI* uri,
                                 nsILoadInfo* aLoadInfo,
                                 nsIChannel** result)
{
    NS_ENSURE_ARG_POINTER(uri);
    nsViewSourceChannel *channel = new nsViewSourceChannel();
    if (!channel)
        return NS_ERROR_OUT_OF_MEMORY;
    NS_ADDREF(channel);

    nsresult rv = channel->Init(uri);
    if (NS_FAILED(rv)) {
        NS_RELEASE(channel);
        return rv;
    }

    // set the loadInfo on the new channel
    rv = channel->SetLoadInfo(aLoadInfo);
    if (NS_FAILED(rv)) {
        NS_RELEASE(channel);
        return rv;
    }

    *result = static_cast<nsIViewSourceChannel*>(channel);
    return NS_OK;
=======
  uri.swap(*aResult);
  return rv;
>>>>>>> upstream-releases
}

NS_IMETHODIMP
<<<<<<< HEAD
nsViewSourceHandler::NewChannel(nsIURI *uri, nsIChannel **result) {
  return NewChannel2(uri, nullptr, result);
||||||| merged common ancestors
nsViewSourceHandler::NewChannel(nsIURI* uri, nsIChannel* *result)
{
    return NewChannel2(uri, nullptr, result);
=======
nsViewSourceHandler::NewChannel(nsIURI* uri, nsILoadInfo* aLoadInfo,
                                nsIChannel** result) {
  NS_ENSURE_ARG_POINTER(uri);
  RefPtr<nsViewSourceChannel> channel = new nsViewSourceChannel();

  nsresult rv = channel->Init(uri);
  if (NS_FAILED(rv)) {
    return rv;
  }

  // set the loadInfo on the new channel
  rv = channel->SetLoadInfo(aLoadInfo);
  if (NS_FAILED(rv)) {
    return rv;
  }

  *result = channel.forget().downcast<nsIViewSourceChannel>().take();
  return NS_OK;
>>>>>>> upstream-releases
}

<<<<<<< HEAD
nsresult nsViewSourceHandler::NewSrcdocChannel(nsIURI *aURI, nsIURI *aBaseURI,
                                               const nsAString &aSrcdoc,
                                               nsILoadInfo *aLoadInfo,
                                               nsIChannel **outChannel) {
  NS_ENSURE_ARG_POINTER(aURI);
  RefPtr<nsViewSourceChannel> channel = new nsViewSourceChannel();

  nsresult rv = channel->InitSrcdoc(aURI, aBaseURI, aSrcdoc, aLoadInfo);
  if (NS_FAILED(rv)) {
    return rv;
  }

  *outChannel = static_cast<nsIViewSourceChannel *>(channel.forget().take());
  return NS_OK;
||||||| merged common ancestors
nsresult
nsViewSourceHandler::NewSrcdocChannel(nsIURI *aURI,
                                      nsIURI *aBaseURI,
                                      const nsAString &aSrcdoc,
                                      nsILoadInfo* aLoadInfo,
                                      nsIChannel** outChannel)
{
    NS_ENSURE_ARG_POINTER(aURI);
    RefPtr<nsViewSourceChannel> channel = new nsViewSourceChannel();

    nsresult rv = channel->InitSrcdoc(aURI, aBaseURI, aSrcdoc, aLoadInfo);
    if (NS_FAILED(rv)) {
        return rv;
    }

    *outChannel = static_cast<nsIViewSourceChannel*>(channel.forget().take());
    return NS_OK;
=======
nsresult nsViewSourceHandler::NewSrcdocChannel(nsIURI* aURI, nsIURI* aBaseURI,
                                               const nsAString& aSrcdoc,
                                               nsILoadInfo* aLoadInfo,
                                               nsIChannel** outChannel) {
  NS_ENSURE_ARG_POINTER(aURI);
  RefPtr<nsViewSourceChannel> channel = new nsViewSourceChannel();

  nsresult rv = channel->InitSrcdoc(aURI, aBaseURI, aSrcdoc, aLoadInfo);
  if (NS_FAILED(rv)) {
    return rv;
  }

  *outChannel = static_cast<nsIViewSourceChannel*>(channel.forget().take());
  return NS_OK;
>>>>>>> upstream-releases
}

NS_IMETHODIMP
<<<<<<< HEAD
nsViewSourceHandler::AllowPort(int32_t port, const char *scheme,
                               bool *_retval) {
  // don't override anything.
  *_retval = false;
  return NS_OK;
||||||| merged common ancestors
nsViewSourceHandler::AllowPort(int32_t port, const char *scheme, bool *_retval)
{
    // don't override anything.
    *_retval = false;
    return NS_OK;
=======
nsViewSourceHandler::AllowPort(int32_t port, const char* scheme,
                               bool* _retval) {
  // don't override anything.
  *_retval = false;
  return NS_OK;
>>>>>>> upstream-releases
}

nsViewSourceHandler::nsViewSourceHandler() { gInstance = this; }

nsViewSourceHandler::~nsViewSourceHandler() { gInstance = nullptr; }

nsViewSourceHandler *nsViewSourceHandler::gInstance = nullptr;

<<<<<<< HEAD
nsViewSourceHandler *nsViewSourceHandler::GetInstance() { return gInstance; }
||||||| merged common ancestors
nsViewSourceHandler*
nsViewSourceHandler::GetInstance()
{
    return gInstance;
}
=======
nsViewSourceHandler* nsViewSourceHandler::GetInstance() { return gInstance; }
>>>>>>> upstream-releases

}  // namespace net
}  // namespace mozilla
