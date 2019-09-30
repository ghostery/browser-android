/* -*- Mode: C++; tab-width: 4; indent-tabs-mode: nil; c-basic-offset: 2 -*- */
/* vim:set ts=4 sw=2 sts=2 et: */
/* This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/. */

#include "nsViewSourceChannel.h"
#include "nsIIOService.h"
#include "nsMimeTypes.h"
#include "nsNetUtil.h"
#include "nsContentUtils.h"
#include "nsIHttpHeaderVisitor.h"
#include "nsContentSecurityManager.h"
#include "nsServiceManagerUtils.h"
#include "nsIInputStreamChannel.h"
#include "mozilla/DebugOnly.h"
#include "mozilla/NullPrincipal.h"
#include "nsIReferrerInfo.h"

NS_IMPL_ADDREF(nsViewSourceChannel)
NS_IMPL_RELEASE(nsViewSourceChannel)
/*
  This QI uses NS_INTERFACE_MAP_ENTRY_CONDITIONAL to check for
  non-nullness of mHttpChannel, mCachingChannel, and mUploadChannel.
*/
NS_INTERFACE_MAP_BEGIN(nsViewSourceChannel)
  NS_INTERFACE_MAP_ENTRY(nsIViewSourceChannel)
  NS_INTERFACE_MAP_ENTRY(nsIStreamListener)
  NS_INTERFACE_MAP_ENTRY(nsIRequestObserver)
  NS_INTERFACE_MAP_ENTRY_CONDITIONAL(nsIHttpChannel, mHttpChannel)
  NS_INTERFACE_MAP_ENTRY_CONDITIONAL(nsIHttpChannelInternal,
                                     mHttpChannelInternal)
  NS_INTERFACE_MAP_ENTRY_CONDITIONAL(nsICachingChannel, mCachingChannel)
  NS_INTERFACE_MAP_ENTRY_CONDITIONAL(nsICacheInfoChannel, mCacheInfoChannel)
  NS_INTERFACE_MAP_ENTRY_CONDITIONAL(nsIApplicationCacheChannel,
                                     mApplicationCacheChannel)
  NS_INTERFACE_MAP_ENTRY_CONDITIONAL(nsIUploadChannel, mUploadChannel)
  NS_INTERFACE_MAP_ENTRY_CONDITIONAL(nsIFormPOSTActionChannel, mPostChannel)
  NS_INTERFACE_MAP_ENTRY_AMBIGUOUS(nsIRequest, nsIViewSourceChannel)
  NS_INTERFACE_MAP_ENTRY_AMBIGUOUS(nsIChannel, nsIViewSourceChannel)
  NS_INTERFACE_MAP_ENTRY_AMBIGUOUS(nsISupports, nsIViewSourceChannel)
NS_INTERFACE_MAP_END

<<<<<<< HEAD
nsresult nsViewSourceChannel::Init(nsIURI *uri) {
  mOriginalURI = uri;
||||||| merged common ancestors
nsresult
nsViewSourceChannel::Init(nsIURI* uri)
{
    mOriginalURI = uri;
=======
nsresult nsViewSourceChannel::Init(nsIURI* uri) {
  mOriginalURI = uri;
>>>>>>> upstream-releases

  nsAutoCString path;
  nsresult rv = uri->GetPathQueryRef(path);
  if (NS_FAILED(rv)) return rv;

  nsCOMPtr<nsIIOService> pService(do_GetIOService(&rv));
  if (NS_FAILED(rv)) return rv;

  nsAutoCString scheme;
  rv = pService->ExtractScheme(path, scheme);
  if (NS_FAILED(rv)) return rv;

  // prevent viewing source of javascript URIs (see bug 204779)
  if (scheme.EqualsLiteral("javascript")) {
    NS_WARNING("blocking view-source:javascript:");
    return NS_ERROR_INVALID_ARG;
  }

<<<<<<< HEAD
  // This function is called from within nsViewSourceHandler::NewChannel2
  // and sets the right loadInfo right after returning from this function.
  // Until then we follow the principal of least privilege and use
  // nullPrincipal as the loadingPrincipal and the least permissive
  // securityflag.
  nsCOMPtr<nsIPrincipal> nullPrincipal =
||||||| merged common ancestors
    // This function is called from within nsViewSourceHandler::NewChannel2
    // and sets the right loadInfo right after returning from this function.
    // Until then we follow the principal of least privilege and use
    // nullPrincipal as the loadingPrincipal and the least permissive
    // securityflag.
    nsCOMPtr<nsIPrincipal> nullPrincipal =
=======
  // This function is called from within nsViewSourceHandler::NewChannel
  // and sets the right loadInfo right after returning from this function.
  // Until then we follow the principal of least privilege and use
  // nullPrincipal as the loadingPrincipal and the least permissive
  // securityflag.
  nsCOMPtr<nsIPrincipal> nullPrincipal =
>>>>>>> upstream-releases
      mozilla::NullPrincipal::CreateWithoutOriginAttributes();

<<<<<<< HEAD
  rv = pService->NewChannel2(
      path,
      nullptr,  // aOriginCharset
      nullptr,  // aCharSet
      nullptr,  // aLoadingNode
      nullPrincipal,
      nullptr,  // aTriggeringPrincipal
      nsILoadInfo::SEC_REQUIRE_SAME_ORIGIN_DATA_IS_BLOCKED,
      nsIContentPolicy::TYPE_OTHER, getter_AddRefs(mChannel));
  NS_ENSURE_SUCCESS(rv, rv);

  mIsSrcdocChannel = false;

  mChannel->SetOriginalURI(mOriginalURI);
  mHttpChannel = do_QueryInterface(mChannel);
  mHttpChannelInternal = do_QueryInterface(mChannel);
  mCachingChannel = do_QueryInterface(mChannel);
  mCacheInfoChannel = do_QueryInterface(mChannel);
  mApplicationCacheChannel = do_QueryInterface(mChannel);
  mUploadChannel = do_QueryInterface(mChannel);
  mPostChannel = do_QueryInterface(mChannel);
||||||| merged common ancestors
    rv = pService->NewChannel2(path,
                               nullptr, // aOriginCharset
                               nullptr, // aCharSet
                               nullptr, // aLoadingNode
                               nullPrincipal,
                               nullptr, // aTriggeringPrincipal
                               nsILoadInfo::SEC_REQUIRE_SAME_ORIGIN_DATA_IS_BLOCKED,
                               nsIContentPolicy::TYPE_OTHER,
                               getter_AddRefs(mChannel));
    NS_ENSURE_SUCCESS(rv, rv);

    mIsSrcdocChannel = false;

    mChannel->SetOriginalURI(mOriginalURI);
    mHttpChannel = do_QueryInterface(mChannel);
    mHttpChannelInternal = do_QueryInterface(mChannel);
    mCachingChannel = do_QueryInterface(mChannel);
    mCacheInfoChannel = do_QueryInterface(mChannel);
    mApplicationCacheChannel = do_QueryInterface(mChannel);
    mUploadChannel = do_QueryInterface(mChannel);
    mPostChannel = do_QueryInterface(mChannel);
=======
  rv = pService->NewChannel(
      path,
      nullptr,  // aOriginCharset
      nullptr,  // aCharSet
      nullptr,  // aLoadingNode
      nullPrincipal,
      nullptr,  // aTriggeringPrincipal
      nsILoadInfo::SEC_REQUIRE_SAME_ORIGIN_DATA_IS_BLOCKED,
      nsIContentPolicy::TYPE_OTHER, getter_AddRefs(mChannel));
  NS_ENSURE_SUCCESS(rv, rv);

  mIsSrcdocChannel = false;

  mChannel->SetOriginalURI(mOriginalURI);
  mHttpChannel = do_QueryInterface(mChannel);
  mHttpChannelInternal = do_QueryInterface(mChannel);
  mCachingChannel = do_QueryInterface(mChannel);
  mCacheInfoChannel = do_QueryInterface(mChannel);
  mApplicationCacheChannel = do_QueryInterface(mChannel);
  mUploadChannel = do_QueryInterface(mChannel);
  mPostChannel = do_QueryInterface(mChannel);
>>>>>>> upstream-releases

  return NS_OK;
}

<<<<<<< HEAD
nsresult nsViewSourceChannel::InitSrcdoc(nsIURI *aURI, nsIURI *aBaseURI,
                                         const nsAString &aSrcdoc,
                                         nsILoadInfo *aLoadInfo) {
  nsresult rv;

  nsCOMPtr<nsIURI> inStreamURI;
  // Need to strip view-source: from the URI.  Hardcoded to
  // about:srcdoc as this is the only permissible URI for srcdoc
  // loads
  rv =
      NS_NewURI(getter_AddRefs(inStreamURI), NS_LITERAL_STRING("about:srcdoc"));
  NS_ENSURE_SUCCESS(rv, rv);

  rv = NS_NewInputStreamChannelInternal(
      getter_AddRefs(mChannel), inStreamURI, aSrcdoc,
      NS_LITERAL_CSTRING("text/html"), aLoadInfo, true);

  NS_ENSURE_SUCCESS(rv, rv);
  mOriginalURI = aURI;
  mIsSrcdocChannel = true;

  mChannel->SetOriginalURI(mOriginalURI);
  mHttpChannel = do_QueryInterface(mChannel);
  mHttpChannelInternal = do_QueryInterface(mChannel);
  mCachingChannel = do_QueryInterface(mChannel);
  mCacheInfoChannel = do_QueryInterface(mChannel);
  mApplicationCacheChannel = do_QueryInterface(mChannel);
  mUploadChannel = do_QueryInterface(mChannel);

  rv = UpdateLoadInfoResultPrincipalURI();
  NS_ENSURE_SUCCESS(rv, rv);

  nsCOMPtr<nsIInputStreamChannel> isc = do_QueryInterface(mChannel);
  MOZ_ASSERT(isc);
  isc->SetBaseURI(aBaseURI);
  return NS_OK;
||||||| merged common ancestors
nsresult
nsViewSourceChannel::InitSrcdoc(nsIURI* aURI,
                                nsIURI* aBaseURI,
                                const nsAString &aSrcdoc,
                                nsILoadInfo* aLoadInfo)
{
    nsresult rv;

    nsCOMPtr<nsIURI> inStreamURI;
    // Need to strip view-source: from the URI.  Hardcoded to
    // about:srcdoc as this is the only permissible URI for srcdoc
    // loads
    rv = NS_NewURI(getter_AddRefs(inStreamURI),
                   NS_LITERAL_STRING("about:srcdoc"));
    NS_ENSURE_SUCCESS(rv, rv);

    rv = NS_NewInputStreamChannelInternal(getter_AddRefs(mChannel),
                                          inStreamURI,
                                          aSrcdoc,
                                          NS_LITERAL_CSTRING("text/html"),
                                          aLoadInfo,
                                          true);

    NS_ENSURE_SUCCESS(rv, rv);
    mOriginalURI = aURI;
    mIsSrcdocChannel = true;

    mChannel->SetOriginalURI(mOriginalURI);
    mHttpChannel = do_QueryInterface(mChannel);
    mHttpChannelInternal = do_QueryInterface(mChannel);
    mCachingChannel = do_QueryInterface(mChannel);
    mCacheInfoChannel = do_QueryInterface(mChannel);
    mApplicationCacheChannel = do_QueryInterface(mChannel);
    mUploadChannel = do_QueryInterface(mChannel);

    rv = UpdateLoadInfoResultPrincipalURI();
    NS_ENSURE_SUCCESS(rv, rv);

    nsCOMPtr<nsIInputStreamChannel> isc = do_QueryInterface(mChannel);
    MOZ_ASSERT(isc);
    isc->SetBaseURI(aBaseURI);
    return NS_OK;
=======
nsresult nsViewSourceChannel::InitSrcdoc(nsIURI* aURI, nsIURI* aBaseURI,
                                         const nsAString& aSrcdoc,
                                         nsILoadInfo* aLoadInfo) {
  nsresult rv;

  nsCOMPtr<nsIURI> inStreamURI;
  // Need to strip view-source: from the URI.  Hardcoded to
  // about:srcdoc as this is the only permissible URI for srcdoc
  // loads
  rv =
      NS_NewURI(getter_AddRefs(inStreamURI), NS_LITERAL_STRING("about:srcdoc"));
  NS_ENSURE_SUCCESS(rv, rv);

  rv = NS_NewInputStreamChannelInternal(
      getter_AddRefs(mChannel), inStreamURI, aSrcdoc,
      NS_LITERAL_CSTRING("text/html"), aLoadInfo, true);

  NS_ENSURE_SUCCESS(rv, rv);
  mOriginalURI = aURI;
  mIsSrcdocChannel = true;

  mChannel->SetOriginalURI(mOriginalURI);
  mHttpChannel = do_QueryInterface(mChannel);
  mHttpChannelInternal = do_QueryInterface(mChannel);
  mCachingChannel = do_QueryInterface(mChannel);
  mCacheInfoChannel = do_QueryInterface(mChannel);
  mApplicationCacheChannel = do_QueryInterface(mChannel);
  mUploadChannel = do_QueryInterface(mChannel);

  rv = UpdateLoadInfoResultPrincipalURI();
  NS_ENSURE_SUCCESS(rv, rv);

  nsCOMPtr<nsIInputStreamChannel> isc = do_QueryInterface(mChannel);
  MOZ_ASSERT(isc);
  isc->SetBaseURI(aBaseURI);
  return NS_OK;
>>>>>>> upstream-releases
}

nsresult nsViewSourceChannel::UpdateLoadInfoResultPrincipalURI() {
  nsresult rv;

  MOZ_ASSERT(mChannel);

<<<<<<< HEAD
  nsCOMPtr<nsILoadInfo> channelLoadInfo = mChannel->GetLoadInfo();
  if (!channelLoadInfo) {
    return NS_OK;
  }

  nsCOMPtr<nsIURI> channelResultPrincipalURI;
  rv = channelLoadInfo->GetResultPrincipalURI(
      getter_AddRefs(channelResultPrincipalURI));
  if (NS_FAILED(rv)) {
    return rv;
  }
||||||| merged common ancestors
    nsCOMPtr<nsILoadInfo> channelLoadInfo = mChannel->GetLoadInfo();
    if (!channelLoadInfo) {
        return NS_OK;
    }

    nsCOMPtr<nsIURI> channelResultPrincipalURI;
    rv = channelLoadInfo->GetResultPrincipalURI(getter_AddRefs(channelResultPrincipalURI));
    if (NS_FAILED(rv)) {
        return rv;
    }
=======
  nsCOMPtr<nsILoadInfo> channelLoadInfo = mChannel->LoadInfo();
  nsCOMPtr<nsIURI> channelResultPrincipalURI;
  rv = channelLoadInfo->GetResultPrincipalURI(
      getter_AddRefs(channelResultPrincipalURI));
  if (NS_FAILED(rv)) {
    return rv;
  }
>>>>>>> upstream-releases

  if (!channelResultPrincipalURI) {
    mChannel->GetOriginalURI(getter_AddRefs(channelResultPrincipalURI));
    return NS_OK;
  }

  if (!channelResultPrincipalURI) {
    return NS_ERROR_UNEXPECTED;
  }

  bool alreadyViewSource;
  if (NS_SUCCEEDED(channelResultPrincipalURI->SchemeIs("view-source",
                                                       &alreadyViewSource)) &&
      alreadyViewSource) {
    return NS_OK;
  }

  nsCOMPtr<nsIURI> updatedResultPrincipalURI;
  rv = BuildViewSourceURI(channelResultPrincipalURI,
                          getter_AddRefs(updatedResultPrincipalURI));
  if (NS_FAILED(rv)) {
    return rv;
  }

  rv = channelLoadInfo->SetResultPrincipalURI(updatedResultPrincipalURI);
  if (NS_FAILED(rv)) {
    return rv;
  }

  return NS_OK;
}

<<<<<<< HEAD
nsresult nsViewSourceChannel::BuildViewSourceURI(nsIURI *aURI,
                                                 nsIURI **aResult) {
  nsresult rv;
||||||| merged common ancestors
nsresult
nsViewSourceChannel::BuildViewSourceURI(nsIURI * aURI, nsIURI ** aResult)
{
    nsresult rv;
=======
nsresult nsViewSourceChannel::BuildViewSourceURI(nsIURI* aURI,
                                                 nsIURI** aResult) {
  nsresult rv;
>>>>>>> upstream-releases

  // protect ourselves against broken channel implementations
  if (!aURI) {
    NS_ERROR("no URI to build view-source uri!");
    return NS_ERROR_UNEXPECTED;
  }

  nsAutoCString spec;
  rv = aURI->GetSpec(spec);
  if (NS_FAILED(rv)) {
    return rv;
  }

  return NS_NewURI(aResult, NS_LITERAL_CSTRING("view-source:") + spec);
}

////////////////////////////////////////////////////////////////////////////////
// nsIRequest methods:

NS_IMETHODIMP
<<<<<<< HEAD
nsViewSourceChannel::GetName(nsACString &result) {
  return NS_ERROR_NOT_IMPLEMENTED;
||||||| merged common ancestors
nsViewSourceChannel::GetName(nsACString &result)
{
    return NS_ERROR_NOT_IMPLEMENTED;
=======
nsViewSourceChannel::GetName(nsACString& result) {
  return NS_ERROR_NOT_IMPLEMENTED;
>>>>>>> upstream-releases
}

NS_IMETHODIMP
<<<<<<< HEAD
nsViewSourceChannel::GetTransferSize(uint64_t *aTransferSize) {
  return NS_ERROR_NOT_IMPLEMENTED;
||||||| merged common ancestors
nsViewSourceChannel::GetTransferSize(uint64_t *aTransferSize)
{
    return NS_ERROR_NOT_IMPLEMENTED;
=======
nsViewSourceChannel::GetTransferSize(uint64_t* aTransferSize) {
  return NS_ERROR_NOT_IMPLEMENTED;
>>>>>>> upstream-releases
}

NS_IMETHODIMP
<<<<<<< HEAD
nsViewSourceChannel::GetDecodedBodySize(uint64_t *aDecodedBodySize) {
  return NS_ERROR_NOT_IMPLEMENTED;
||||||| merged common ancestors
nsViewSourceChannel::GetDecodedBodySize(uint64_t *aDecodedBodySize)
{
    return NS_ERROR_NOT_IMPLEMENTED;
=======
nsViewSourceChannel::GetDecodedBodySize(uint64_t* aDecodedBodySize) {
  return NS_ERROR_NOT_IMPLEMENTED;
>>>>>>> upstream-releases
}

NS_IMETHODIMP
<<<<<<< HEAD
nsViewSourceChannel::GetEncodedBodySize(uint64_t *aEncodedBodySize) {
  return NS_ERROR_NOT_IMPLEMENTED;
||||||| merged common ancestors
nsViewSourceChannel::GetEncodedBodySize(uint64_t *aEncodedBodySize)
{
    return NS_ERROR_NOT_IMPLEMENTED;
=======
nsViewSourceChannel::GetEncodedBodySize(uint64_t* aEncodedBodySize) {
  return NS_ERROR_NOT_IMPLEMENTED;
>>>>>>> upstream-releases
}

NS_IMETHODIMP
<<<<<<< HEAD
nsViewSourceChannel::IsPending(bool *result) {
  NS_ENSURE_TRUE(mChannel, NS_ERROR_FAILURE);
||||||| merged common ancestors
nsViewSourceChannel::IsPending(bool *result)
{
    NS_ENSURE_TRUE(mChannel, NS_ERROR_FAILURE);
=======
nsViewSourceChannel::IsPending(bool* result) {
  NS_ENSURE_TRUE(mChannel, NS_ERROR_FAILURE);
>>>>>>> upstream-releases

  return mChannel->IsPending(result);
}

NS_IMETHODIMP
<<<<<<< HEAD
nsViewSourceChannel::GetStatus(nsresult *status) {
  NS_ENSURE_TRUE(mChannel, NS_ERROR_FAILURE);
||||||| merged common ancestors
nsViewSourceChannel::GetStatus(nsresult *status)
{
    NS_ENSURE_TRUE(mChannel, NS_ERROR_FAILURE);
=======
nsViewSourceChannel::GetStatus(nsresult* status) {
  NS_ENSURE_TRUE(mChannel, NS_ERROR_FAILURE);
>>>>>>> upstream-releases

  return mChannel->GetStatus(status);
}

NS_IMETHODIMP
nsViewSourceChannel::Cancel(nsresult status) {
  NS_ENSURE_TRUE(mChannel, NS_ERROR_FAILURE);

  return mChannel->Cancel(status);
}

NS_IMETHODIMP
nsViewSourceChannel::Suspend(void) {
  NS_ENSURE_TRUE(mChannel, NS_ERROR_FAILURE);

  return mChannel->Suspend();
}

NS_IMETHODIMP
nsViewSourceChannel::Resume(void) {
  NS_ENSURE_TRUE(mChannel, NS_ERROR_FAILURE);

  return mChannel->Resume();
}

////////////////////////////////////////////////////////////////////////////////
// nsIChannel methods:

NS_IMETHODIMP
<<<<<<< HEAD
nsViewSourceChannel::GetOriginalURI(nsIURI **aURI) {
  NS_ASSERTION(aURI, "Null out param!");
  *aURI = mOriginalURI;
  NS_ADDREF(*aURI);
  return NS_OK;
||||||| merged common ancestors
nsViewSourceChannel::GetOriginalURI(nsIURI* *aURI)
{
    NS_ASSERTION(aURI, "Null out param!");
    *aURI = mOriginalURI;
    NS_ADDREF(*aURI);
    return NS_OK;
=======
nsViewSourceChannel::GetOriginalURI(nsIURI** aURI) {
  NS_ASSERTION(aURI, "Null out param!");
  *aURI = mOriginalURI;
  NS_ADDREF(*aURI);
  return NS_OK;
>>>>>>> upstream-releases
}

NS_IMETHODIMP
<<<<<<< HEAD
nsViewSourceChannel::SetOriginalURI(nsIURI *aURI) {
  NS_ENSURE_ARG_POINTER(aURI);
  mOriginalURI = aURI;
  return NS_OK;
||||||| merged common ancestors
nsViewSourceChannel::SetOriginalURI(nsIURI* aURI)
{
    NS_ENSURE_ARG_POINTER(aURI);
    mOriginalURI = aURI;
    return NS_OK;
=======
nsViewSourceChannel::SetOriginalURI(nsIURI* aURI) {
  NS_ENSURE_ARG_POINTER(aURI);
  mOriginalURI = aURI;
  return NS_OK;
>>>>>>> upstream-releases
}

NS_IMETHODIMP
<<<<<<< HEAD
nsViewSourceChannel::GetURI(nsIURI **aURI) {
  NS_ENSURE_TRUE(mChannel, NS_ERROR_FAILURE);
||||||| merged common ancestors
nsViewSourceChannel::GetURI(nsIURI* *aURI)
{
    NS_ENSURE_TRUE(mChannel, NS_ERROR_FAILURE);
=======
nsViewSourceChannel::GetURI(nsIURI** aURI) {
  NS_ENSURE_TRUE(mChannel, NS_ERROR_FAILURE);
>>>>>>> upstream-releases

  nsCOMPtr<nsIURI> uri;
  nsresult rv = mChannel->GetURI(getter_AddRefs(uri));
  if (NS_FAILED(rv)) {
    return rv;
  }

  return BuildViewSourceURI(uri, aURI);
}

NS_IMETHODIMP
<<<<<<< HEAD
nsViewSourceChannel::Open(nsIInputStream **_retval) {
  NS_ENSURE_TRUE(mChannel, NS_ERROR_FAILURE);

  nsresult rv = NS_MaybeOpenChannelUsingOpen2(mChannel, _retval);
  if (NS_SUCCEEDED(rv)) {
    mOpened = true;
  }
  return rv;
||||||| merged common ancestors
nsViewSourceChannel::Open(nsIInputStream **_retval)
{
    NS_ENSURE_TRUE(mChannel, NS_ERROR_FAILURE);

    nsresult rv = NS_MaybeOpenChannelUsingOpen2(mChannel, _retval);
    if (NS_SUCCEEDED(rv)) {
        mOpened = true;
    }
    return rv;
=======
nsViewSourceChannel::Open(nsIInputStream** aStream) {
  NS_ENSURE_TRUE(mChannel, NS_ERROR_FAILURE);
  return Open(aStream);
>>>>>>> upstream-releases
}

NS_IMETHODIMP
<<<<<<< HEAD
nsViewSourceChannel::Open2(nsIInputStream **aStream) {
  NS_ENSURE_TRUE(mChannel, NS_ERROR_FAILURE);
  nsCOMPtr<nsILoadInfo> loadInfo = mChannel->GetLoadInfo();
  if (!loadInfo) {
    MOZ_ASSERT(loadInfo, "can not enforce security without loadInfo");
    return NS_ERROR_UNEXPECTED;
  }
  // setting the flag on the loadInfo indicates that the underlying
  // channel will be openend using Open2() and hence performs
  // the necessary security checks.
  loadInfo->SetEnforceSecurity(true);
  return Open(aStream);
}

NS_IMETHODIMP
nsViewSourceChannel::AsyncOpen(nsIStreamListener *aListener,
                               nsISupports *ctxt) {
#ifdef DEBUG
  {
    nsCOMPtr<nsILoadInfo> loadInfo = mChannel->GetLoadInfo();
    MOZ_ASSERT(!loadInfo || loadInfo->GetSecurityMode() == 0 ||
                   loadInfo->GetEnforceSecurity(),
               "security flags in loadInfo but asyncOpen2() not called");
  }
#endif
||||||| merged common ancestors
nsViewSourceChannel::Open2(nsIInputStream** aStream)
{
    NS_ENSURE_TRUE(mChannel, NS_ERROR_FAILURE);
    nsCOMPtr<nsILoadInfo> loadInfo = mChannel->GetLoadInfo();
    if(!loadInfo) {
        MOZ_ASSERT(loadInfo, "can not enforce security without loadInfo");
        return NS_ERROR_UNEXPECTED;
    }
    // setting the flag on the loadInfo indicates that the underlying
    // channel will be openend using Open2() and hence performs
    // the necessary security checks.
    loadInfo->SetEnforceSecurity(true);
    return Open(aStream);
}

NS_IMETHODIMP
nsViewSourceChannel::AsyncOpen(nsIStreamListener *aListener, nsISupports *ctxt)
{
#ifdef DEBUG
    {
    nsCOMPtr<nsILoadInfo> loadInfo = mChannel->GetLoadInfo();
    MOZ_ASSERT(!loadInfo || loadInfo->GetSecurityMode() == 0 ||
               loadInfo->GetEnforceSecurity(),
               "security flags in loadInfo but asyncOpen2() not called");
    }
#endif
=======
nsViewSourceChannel::AsyncOpen(nsIStreamListener* aListener) {
  // We can't ensure GetInitialSecurityCheckDone here
>>>>>>> upstream-releases

  NS_ENSURE_TRUE(mChannel, NS_ERROR_FAILURE);

  mListener = aListener;

  /*
   * We want to add ourselves to the loadgroup before opening
   * mChannel, since we want to make sure we're in the loadgroup
   * when mChannel finishes and fires OnStopRequest()
   */

<<<<<<< HEAD
  nsCOMPtr<nsILoadGroup> loadGroup;
  mChannel->GetLoadGroup(getter_AddRefs(loadGroup));
  if (loadGroup)
    loadGroup->AddRequest(static_cast<nsIViewSourceChannel *>(this), nullptr);

  nsresult rv = NS_OK;
  nsCOMPtr<nsILoadInfo> loadInfo = mChannel->GetLoadInfo();
  if (loadInfo && loadInfo->GetEnforceSecurity()) {
    rv = mChannel->AsyncOpen2(this);
  } else {
    rv = mChannel->AsyncOpen(this, ctxt);
  }
||||||| merged common ancestors
    nsCOMPtr<nsILoadGroup> loadGroup;
    mChannel->GetLoadGroup(getter_AddRefs(loadGroup));
    if (loadGroup)
        loadGroup->AddRequest(static_cast<nsIViewSourceChannel*>
                                         (this), nullptr);

    nsresult rv = NS_OK;
    nsCOMPtr<nsILoadInfo> loadInfo = mChannel->GetLoadInfo();
    if (loadInfo && loadInfo->GetEnforceSecurity()) {
        rv = mChannel->AsyncOpen2(this);
    }
    else {
        rv = mChannel->AsyncOpen(this, ctxt);
    }
=======
  nsCOMPtr<nsILoadGroup> loadGroup;
  mChannel->GetLoadGroup(getter_AddRefs(loadGroup));
  if (loadGroup)
    loadGroup->AddRequest(static_cast<nsIViewSourceChannel*>(this), nullptr);
>>>>>>> upstream-releases

<<<<<<< HEAD
  if (NS_FAILED(rv) && loadGroup)
    loadGroup->RemoveRequest(static_cast<nsIViewSourceChannel *>(this), nullptr,
                             rv);
||||||| merged common ancestors
    if (NS_FAILED(rv) && loadGroup)
        loadGroup->RemoveRequest(static_cast<nsIViewSourceChannel*>
                                            (this),
                                 nullptr, rv);
=======
  nsresult rv = NS_OK;
  rv = mChannel->AsyncOpen(this);
>>>>>>> upstream-releases

<<<<<<< HEAD
  if (NS_SUCCEEDED(rv)) {
    mOpened = true;
  }
||||||| merged common ancestors
    if (NS_SUCCEEDED(rv)) {
        mOpened = true;
    }
=======
  if (NS_FAILED(rv) && loadGroup)
    loadGroup->RemoveRequest(static_cast<nsIViewSourceChannel*>(this), nullptr,
                             rv);
>>>>>>> upstream-releases

<<<<<<< HEAD
  return rv;
}

NS_IMETHODIMP
nsViewSourceChannel::AsyncOpen2(nsIStreamListener *aListener) {
  nsCOMPtr<nsILoadInfo> loadInfo = mChannel->GetLoadInfo();
  if (!loadInfo) {
    MOZ_ASSERT(loadInfo, "can not enforce security without loadInfo");
    return NS_ERROR_UNEXPECTED;
||||||| merged common ancestors
    return rv;
}

NS_IMETHODIMP
nsViewSourceChannel::AsyncOpen2(nsIStreamListener *aListener)
{
  nsCOMPtr<nsILoadInfo> loadInfo = mChannel->GetLoadInfo();
  if(!loadInfo) {
    MOZ_ASSERT(loadInfo, "can not enforce security without loadInfo");
    return NS_ERROR_UNEXPECTED;
=======
  if (NS_SUCCEEDED(rv)) {
    mOpened = true;
>>>>>>> upstream-releases
  }

  return rv;
}

/*
 * Both the view source channel and mChannel are added to the
 * loadgroup.  There should never be more than one request in the
 * loadgroup that has LOAD_DOCUMENT_URI set.  The one that has this
 * flag set is the request whose URI is used to refetch the document,
 * so it better be the viewsource channel.
 *
 * Therefore, we need to make sure that
 * 1) The load flags on mChannel _never_ include LOAD_DOCUMENT_URI
 * 2) The load flags on |this| include LOAD_DOCUMENT_URI when it was
 *    set via SetLoadFlags (mIsDocument keeps track of this flag).
 */

NS_IMETHODIMP
<<<<<<< HEAD
nsViewSourceChannel::GetLoadFlags(uint32_t *aLoadFlags) {
  NS_ENSURE_TRUE(mChannel, NS_ERROR_FAILURE);
||||||| merged common ancestors
nsViewSourceChannel::GetLoadFlags(uint32_t *aLoadFlags)
{
    NS_ENSURE_TRUE(mChannel, NS_ERROR_FAILURE);
=======
nsViewSourceChannel::GetLoadFlags(uint32_t* aLoadFlags) {
  NS_ENSURE_TRUE(mChannel, NS_ERROR_FAILURE);
>>>>>>> upstream-releases

  nsresult rv = mChannel->GetLoadFlags(aLoadFlags);
  if (NS_FAILED(rv)) return rv;

  // This should actually be just LOAD_DOCUMENT_URI but the win32 compiler
  // fails to deal due to amiguous inheritance.  nsIChannel::LOAD_DOCUMENT_URI
  // also fails; the Win32 compiler thinks that's supposed to be a method.
  if (mIsDocument) *aLoadFlags |= ::nsIChannel::LOAD_DOCUMENT_URI;

  return rv;
}

NS_IMETHODIMP
nsViewSourceChannel::SetLoadFlags(uint32_t aLoadFlags) {
  NS_ENSURE_TRUE(mChannel, NS_ERROR_FAILURE);

  // "View source" always wants the currently cached content.
  // We also want to have _this_ channel, not mChannel to be the
  // 'document' channel in the loadgroup.

  // These should actually be just LOAD_FROM_CACHE and LOAD_DOCUMENT_URI but
  // the win32 compiler fails to deal due to amiguous inheritance.
  // nsIChannel::LOAD_DOCUMENT_URI/nsIRequest::LOAD_FROM_CACHE also fails; the
  // Win32 compiler thinks that's supposed to be a method.
  mIsDocument = (aLoadFlags & ::nsIChannel::LOAD_DOCUMENT_URI) ? true : false;

  nsresult rv =
      mChannel->SetLoadFlags((aLoadFlags | ::nsIRequest::LOAD_FROM_CACHE) &
                             ~::nsIChannel::LOAD_DOCUMENT_URI);
  if (NS_WARN_IF(NS_FAILED(rv))) {
    return rv;
  }

  if (mHttpChannel) {
    rv = mHttpChannel->SetIsMainDocumentChannel(
        aLoadFlags & ::nsIChannel::LOAD_DOCUMENT_URI);
    MOZ_ASSERT(NS_SUCCEEDED(rv));
  }

  return NS_OK;
}

NS_IMETHODIMP
<<<<<<< HEAD
nsViewSourceChannel::GetContentType(nsACString &aContentType) {
  NS_ENSURE_TRUE(mChannel, NS_ERROR_FAILURE);

  aContentType.Truncate();
||||||| merged common ancestors
nsViewSourceChannel::GetContentType(nsACString &aContentType)
{
    NS_ENSURE_TRUE(mChannel, NS_ERROR_FAILURE);

    aContentType.Truncate();
=======
nsViewSourceChannel::GetContentType(nsACString& aContentType) {
  NS_ENSURE_TRUE(mChannel, NS_ERROR_FAILURE);
>>>>>>> upstream-releases

<<<<<<< HEAD
  if (mContentType.IsEmpty()) {
    // Get the current content type
    nsresult rv;
    nsAutoCString contentType;
    rv = mChannel->GetContentType(contentType);
    if (NS_FAILED(rv)) return rv;
||||||| merged common ancestors
    if (mContentType.IsEmpty())
    {
        // Get the current content type
        nsresult rv;
        nsAutoCString contentType;
        rv = mChannel->GetContentType(contentType);
        if (NS_FAILED(rv)) return rv;

        // If we don't know our type, just say so.  The unknown
        // content decoder will then kick in automatically, and it
        // will call our SetOriginalContentType method instead of our
        // SetContentType method to set the type it determines.
        if (!contentType.EqualsLiteral(UNKNOWN_CONTENT_TYPE)) {
          contentType = VIEWSOURCE_CONTENT_TYPE;
        }
=======
  aContentType.Truncate();

  if (mContentType.IsEmpty()) {
    // Get the current content type
    nsresult rv;
    nsAutoCString contentType;
    rv = mChannel->GetContentType(contentType);
    if (NS_FAILED(rv)) return rv;
>>>>>>> upstream-releases

    // If we don't know our type, just say so.  The unknown
    // content decoder will then kick in automatically, and it
    // will call our SetOriginalContentType method instead of our
    // SetContentType method to set the type it determines.
    if (!contentType.EqualsLiteral(UNKNOWN_CONTENT_TYPE)) {
      contentType = VIEWSOURCE_CONTENT_TYPE;
    }

    mContentType = contentType;
  }

  aContentType = mContentType;
  return NS_OK;
}

NS_IMETHODIMP
<<<<<<< HEAD
nsViewSourceChannel::SetContentType(const nsACString &aContentType) {
  // Our GetContentType() currently returns VIEWSOURCE_CONTENT_TYPE
  //
  // However, during the parsing phase the parser calls our
  // channel's GetContentType(). Returning the string above trips up
  // the parser. In order to avoid messy changes and not to have the
  // parser depend on nsIViewSourceChannel Vidur proposed the
  // following solution:
  //
  // The ViewSourceChannel initially returns a content type of
  // VIEWSOURCE_CONTENT_TYPE.  Based on this type decisions to
  // create a viewer for doing a view source are made.  After the
  // viewer is created, nsLayoutDLF::CreateInstance() calls this
  // SetContentType() with the original content type.  When it's
  // time for the parser to find out the content type it will call
  // our channel's GetContentType() and it will get the original
  // content type, such as, text/html and everything is kosher from
  // then on.

  if (!mOpened) {
    // We do not take hints
    return NS_ERROR_NOT_AVAILABLE;
  }
||||||| merged common ancestors
nsViewSourceChannel::SetContentType(const nsACString &aContentType)
{
    // Our GetContentType() currently returns VIEWSOURCE_CONTENT_TYPE
    //
    // However, during the parsing phase the parser calls our
    // channel's GetContentType(). Returning the string above trips up
    // the parser. In order to avoid messy changes and not to have the
    // parser depend on nsIViewSourceChannel Vidur proposed the
    // following solution:
    //
    // The ViewSourceChannel initially returns a content type of
    // VIEWSOURCE_CONTENT_TYPE.  Based on this type decisions to
    // create a viewer for doing a view source are made.  After the
    // viewer is created, nsLayoutDLF::CreateInstance() calls this
    // SetContentType() with the original content type.  When it's
    // time for the parser to find out the content type it will call
    // our channel's GetContentType() and it will get the original
    // content type, such as, text/html and everything is kosher from
    // then on.

    if (!mOpened) {
        // We do not take hints
        return NS_ERROR_NOT_AVAILABLE;
    }
=======
nsViewSourceChannel::SetContentType(const nsACString& aContentType) {
  // Our GetContentType() currently returns VIEWSOURCE_CONTENT_TYPE
  //
  // However, during the parsing phase the parser calls our
  // channel's GetContentType(). Returning the string above trips up
  // the parser. In order to avoid messy changes and not to have the
  // parser depend on nsIViewSourceChannel Vidur proposed the
  // following solution:
  //
  // The ViewSourceChannel initially returns a content type of
  // VIEWSOURCE_CONTENT_TYPE.  Based on this type decisions to
  // create a viewer for doing a view source are made.  After the
  // viewer is created, nsLayoutDLF::CreateInstance() calls this
  // SetContentType() with the original content type.  When it's
  // time for the parser to find out the content type it will call
  // our channel's GetContentType() and it will get the original
  // content type, such as, text/html and everything is kosher from
  // then on.

  if (!mOpened) {
    // We do not take hints
    return NS_ERROR_NOT_AVAILABLE;
  }
>>>>>>> upstream-releases

  mContentType = aContentType;
  return NS_OK;
}

NS_IMETHODIMP
<<<<<<< HEAD
nsViewSourceChannel::GetContentCharset(nsACString &aContentCharset) {
  NS_ENSURE_TRUE(mChannel, NS_ERROR_FAILURE);
||||||| merged common ancestors
nsViewSourceChannel::GetContentCharset(nsACString &aContentCharset)
{
    NS_ENSURE_TRUE(mChannel, NS_ERROR_FAILURE);
=======
nsViewSourceChannel::GetContentCharset(nsACString& aContentCharset) {
  NS_ENSURE_TRUE(mChannel, NS_ERROR_FAILURE);
>>>>>>> upstream-releases

  return mChannel->GetContentCharset(aContentCharset);
}

NS_IMETHODIMP
<<<<<<< HEAD
nsViewSourceChannel::SetContentCharset(const nsACString &aContentCharset) {
  NS_ENSURE_TRUE(mChannel, NS_ERROR_FAILURE);
||||||| merged common ancestors
nsViewSourceChannel::SetContentCharset(const nsACString &aContentCharset)
{
    NS_ENSURE_TRUE(mChannel, NS_ERROR_FAILURE);
=======
nsViewSourceChannel::SetContentCharset(const nsACString& aContentCharset) {
  NS_ENSURE_TRUE(mChannel, NS_ERROR_FAILURE);
>>>>>>> upstream-releases

  return mChannel->SetContentCharset(aContentCharset);
}

// We don't forward these methods becacuse content-disposition isn't whitelisted
// (see GetResponseHeader/VisitResponseHeaders).
NS_IMETHODIMP
<<<<<<< HEAD
nsViewSourceChannel::GetContentDisposition(uint32_t *aContentDisposition) {
  return NS_ERROR_NOT_AVAILABLE;
||||||| merged common ancestors
nsViewSourceChannel::GetContentDisposition(uint32_t *aContentDisposition)
{
    return NS_ERROR_NOT_AVAILABLE;
=======
nsViewSourceChannel::GetContentDisposition(uint32_t* aContentDisposition) {
  return NS_ERROR_NOT_AVAILABLE;
>>>>>>> upstream-releases
}

NS_IMETHODIMP
nsViewSourceChannel::SetContentDisposition(uint32_t aContentDisposition) {
  return NS_ERROR_NOT_AVAILABLE;
}

NS_IMETHODIMP
<<<<<<< HEAD
nsViewSourceChannel::GetContentDispositionFilename(
    nsAString &aContentDispositionFilename) {
  return NS_ERROR_NOT_AVAILABLE;
||||||| merged common ancestors
nsViewSourceChannel::GetContentDispositionFilename(nsAString &aContentDispositionFilename)
{
    return NS_ERROR_NOT_AVAILABLE;
=======
nsViewSourceChannel::GetContentDispositionFilename(
    nsAString& aContentDispositionFilename) {
  return NS_ERROR_NOT_AVAILABLE;
>>>>>>> upstream-releases
}

NS_IMETHODIMP
<<<<<<< HEAD
nsViewSourceChannel::SetContentDispositionFilename(
    const nsAString &aContentDispositionFilename) {
  return NS_ERROR_NOT_AVAILABLE;
||||||| merged common ancestors
nsViewSourceChannel::SetContentDispositionFilename(const nsAString &aContentDispositionFilename)
{
    return NS_ERROR_NOT_AVAILABLE;
=======
nsViewSourceChannel::SetContentDispositionFilename(
    const nsAString& aContentDispositionFilename) {
  return NS_ERROR_NOT_AVAILABLE;
>>>>>>> upstream-releases
}

NS_IMETHODIMP
<<<<<<< HEAD
nsViewSourceChannel::GetContentDispositionHeader(
    nsACString &aContentDispositionHeader) {
  return NS_ERROR_NOT_AVAILABLE;
||||||| merged common ancestors
nsViewSourceChannel::GetContentDispositionHeader(nsACString &aContentDispositionHeader)
{
    return NS_ERROR_NOT_AVAILABLE;
=======
nsViewSourceChannel::GetContentDispositionHeader(
    nsACString& aContentDispositionHeader) {
  return NS_ERROR_NOT_AVAILABLE;
>>>>>>> upstream-releases
}

NS_IMETHODIMP
<<<<<<< HEAD
nsViewSourceChannel::GetContentLength(int64_t *aContentLength) {
  NS_ENSURE_TRUE(mChannel, NS_ERROR_FAILURE);
||||||| merged common ancestors
nsViewSourceChannel::GetContentLength(int64_t *aContentLength)
{
    NS_ENSURE_TRUE(mChannel, NS_ERROR_FAILURE);
=======
nsViewSourceChannel::GetContentLength(int64_t* aContentLength) {
  NS_ENSURE_TRUE(mChannel, NS_ERROR_FAILURE);
>>>>>>> upstream-releases

  return mChannel->GetContentLength(aContentLength);
}

NS_IMETHODIMP
nsViewSourceChannel::SetContentLength(int64_t aContentLength) {
  NS_ENSURE_TRUE(mChannel, NS_ERROR_FAILURE);

  return mChannel->SetContentLength(aContentLength);
}

NS_IMETHODIMP
<<<<<<< HEAD
nsViewSourceChannel::GetLoadGroup(nsILoadGroup **aLoadGroup) {
  NS_ENSURE_TRUE(mChannel, NS_ERROR_FAILURE);
||||||| merged common ancestors
nsViewSourceChannel::GetLoadGroup(nsILoadGroup* *aLoadGroup)
{
    NS_ENSURE_TRUE(mChannel, NS_ERROR_FAILURE);
=======
nsViewSourceChannel::GetLoadGroup(nsILoadGroup** aLoadGroup) {
  NS_ENSURE_TRUE(mChannel, NS_ERROR_FAILURE);
>>>>>>> upstream-releases

  return mChannel->GetLoadGroup(aLoadGroup);
}

NS_IMETHODIMP
<<<<<<< HEAD
nsViewSourceChannel::SetLoadGroup(nsILoadGroup *aLoadGroup) {
  NS_ENSURE_TRUE(mChannel, NS_ERROR_FAILURE);
||||||| merged common ancestors
nsViewSourceChannel::SetLoadGroup(nsILoadGroup* aLoadGroup)
{
    NS_ENSURE_TRUE(mChannel, NS_ERROR_FAILURE);
=======
nsViewSourceChannel::SetLoadGroup(nsILoadGroup* aLoadGroup) {
  NS_ENSURE_TRUE(mChannel, NS_ERROR_FAILURE);
>>>>>>> upstream-releases

  return mChannel->SetLoadGroup(aLoadGroup);
}

NS_IMETHODIMP
<<<<<<< HEAD
nsViewSourceChannel::GetOwner(nsISupports **aOwner) {
  NS_ENSURE_TRUE(mChannel, NS_ERROR_FAILURE);
||||||| merged common ancestors
nsViewSourceChannel::GetOwner(nsISupports* *aOwner)
{
    NS_ENSURE_TRUE(mChannel, NS_ERROR_FAILURE);
=======
nsViewSourceChannel::GetOwner(nsISupports** aOwner) {
  NS_ENSURE_TRUE(mChannel, NS_ERROR_FAILURE);
>>>>>>> upstream-releases

  return mChannel->GetOwner(aOwner);
}

NS_IMETHODIMP
<<<<<<< HEAD
nsViewSourceChannel::SetOwner(nsISupports *aOwner) {
  NS_ENSURE_TRUE(mChannel, NS_ERROR_FAILURE);
||||||| merged common ancestors
nsViewSourceChannel::SetOwner(nsISupports* aOwner)
{
    NS_ENSURE_TRUE(mChannel, NS_ERROR_FAILURE);
=======
nsViewSourceChannel::SetOwner(nsISupports* aOwner) {
  NS_ENSURE_TRUE(mChannel, NS_ERROR_FAILURE);
>>>>>>> upstream-releases

  return mChannel->SetOwner(aOwner);
}

NS_IMETHODIMP
<<<<<<< HEAD
nsViewSourceChannel::GetLoadInfo(nsILoadInfo **aLoadInfo) {
  NS_ENSURE_TRUE(mChannel, NS_ERROR_FAILURE);
||||||| merged common ancestors
nsViewSourceChannel::GetLoadInfo(nsILoadInfo* *aLoadInfo)
{
    NS_ENSURE_TRUE(mChannel, NS_ERROR_FAILURE);
=======
nsViewSourceChannel::GetLoadInfo(nsILoadInfo** aLoadInfo) {
  NS_ENSURE_TRUE(mChannel, NS_ERROR_FAILURE);
>>>>>>> upstream-releases

  return mChannel->GetLoadInfo(aLoadInfo);
}

NS_IMETHODIMP
<<<<<<< HEAD
nsViewSourceChannel::SetLoadInfo(nsILoadInfo *aLoadInfo) {
  NS_ENSURE_TRUE(mChannel, NS_ERROR_FAILURE);

  return mChannel->SetLoadInfo(aLoadInfo);
||||||| merged common ancestors
nsViewSourceChannel::SetLoadInfo(nsILoadInfo* aLoadInfo)
{
    NS_ENSURE_TRUE(mChannel, NS_ERROR_FAILURE);

    return mChannel->SetLoadInfo(aLoadInfo);
=======
nsViewSourceChannel::SetLoadInfo(nsILoadInfo* aLoadInfo) {
  NS_ENSURE_TRUE(mChannel, NS_ERROR_FAILURE);
  MOZ_RELEASE_ASSERT(aLoadInfo, "loadinfo can't be null");
  return mChannel->SetLoadInfo(aLoadInfo);
>>>>>>> upstream-releases
}

NS_IMETHODIMP
<<<<<<< HEAD
nsViewSourceChannel::GetIsDocument(bool *aIsDocument) {
  NS_ENSURE_TRUE(mChannel, NS_ERROR_FAILURE);
||||||| merged common ancestors
nsViewSourceChannel::GetIsDocument(bool *aIsDocument)
{
    NS_ENSURE_TRUE(mChannel, NS_ERROR_FAILURE);
=======
nsViewSourceChannel::GetIsDocument(bool* aIsDocument) {
  NS_ENSURE_TRUE(mChannel, NS_ERROR_FAILURE);
>>>>>>> upstream-releases

  return mChannel->GetIsDocument(aIsDocument);
}

NS_IMETHODIMP
<<<<<<< HEAD
nsViewSourceChannel::GetNotificationCallbacks(
    nsIInterfaceRequestor **aNotificationCallbacks) {
  NS_ENSURE_TRUE(mChannel, NS_ERROR_FAILURE);
||||||| merged common ancestors
nsViewSourceChannel::GetNotificationCallbacks(nsIInterfaceRequestor* *aNotificationCallbacks)
{
    NS_ENSURE_TRUE(mChannel, NS_ERROR_FAILURE);
=======
nsViewSourceChannel::GetNotificationCallbacks(
    nsIInterfaceRequestor** aNotificationCallbacks) {
  NS_ENSURE_TRUE(mChannel, NS_ERROR_FAILURE);
>>>>>>> upstream-releases

  return mChannel->GetNotificationCallbacks(aNotificationCallbacks);
}

NS_IMETHODIMP
<<<<<<< HEAD
nsViewSourceChannel::SetNotificationCallbacks(
    nsIInterfaceRequestor *aNotificationCallbacks) {
  NS_ENSURE_TRUE(mChannel, NS_ERROR_FAILURE);
||||||| merged common ancestors
nsViewSourceChannel::SetNotificationCallbacks(nsIInterfaceRequestor* aNotificationCallbacks)
{
    NS_ENSURE_TRUE(mChannel, NS_ERROR_FAILURE);
=======
nsViewSourceChannel::SetNotificationCallbacks(
    nsIInterfaceRequestor* aNotificationCallbacks) {
  NS_ENSURE_TRUE(mChannel, NS_ERROR_FAILURE);
>>>>>>> upstream-releases

  return mChannel->SetNotificationCallbacks(aNotificationCallbacks);
}

NS_IMETHODIMP
<<<<<<< HEAD
nsViewSourceChannel::GetSecurityInfo(nsISupports **aSecurityInfo) {
  NS_ENSURE_TRUE(mChannel, NS_ERROR_FAILURE);
||||||| merged common ancestors
nsViewSourceChannel::GetSecurityInfo(nsISupports * *aSecurityInfo)
{
    NS_ENSURE_TRUE(mChannel, NS_ERROR_FAILURE);
=======
nsViewSourceChannel::GetSecurityInfo(nsISupports** aSecurityInfo) {
  NS_ENSURE_TRUE(mChannel, NS_ERROR_FAILURE);
>>>>>>> upstream-releases

  return mChannel->GetSecurityInfo(aSecurityInfo);
}

// nsIViewSourceChannel methods
NS_IMETHODIMP
<<<<<<< HEAD
nsViewSourceChannel::GetOriginalContentType(nsACString &aContentType) {
  NS_ENSURE_TRUE(mChannel, NS_ERROR_FAILURE);
||||||| merged common ancestors
nsViewSourceChannel::GetOriginalContentType(nsACString &aContentType)
{
    NS_ENSURE_TRUE(mChannel, NS_ERROR_FAILURE);
=======
nsViewSourceChannel::GetOriginalContentType(nsACString& aContentType) {
  NS_ENSURE_TRUE(mChannel, NS_ERROR_FAILURE);
>>>>>>> upstream-releases

  return mChannel->GetContentType(aContentType);
}

NS_IMETHODIMP
<<<<<<< HEAD
nsViewSourceChannel::SetOriginalContentType(const nsACString &aContentType) {
  NS_ENSURE_TRUE(mChannel, NS_ERROR_FAILURE);
||||||| merged common ancestors
nsViewSourceChannel::SetOriginalContentType(const nsACString &aContentType)
{
    NS_ENSURE_TRUE(mChannel, NS_ERROR_FAILURE);
=======
nsViewSourceChannel::SetOriginalContentType(const nsACString& aContentType) {
  NS_ENSURE_TRUE(mChannel, NS_ERROR_FAILURE);
>>>>>>> upstream-releases

  // clear our cached content-type value
  mContentType.Truncate();

  return mChannel->SetContentType(aContentType);
}

NS_IMETHODIMP
<<<<<<< HEAD
nsViewSourceChannel::GetIsSrcdocChannel(bool *aIsSrcdocChannel) {
  *aIsSrcdocChannel = mIsSrcdocChannel;
  return NS_OK;
||||||| merged common ancestors
nsViewSourceChannel::GetIsSrcdocChannel(bool* aIsSrcdocChannel)
{
    *aIsSrcdocChannel = mIsSrcdocChannel;
    return NS_OK;
=======
nsViewSourceChannel::GetIsSrcdocChannel(bool* aIsSrcdocChannel) {
  *aIsSrcdocChannel = mIsSrcdocChannel;
  return NS_OK;
>>>>>>> upstream-releases
}

NS_IMETHODIMP
<<<<<<< HEAD
nsViewSourceChannel::GetBaseURI(nsIURI **aBaseURI) {
||||||| merged common ancestors
nsViewSourceChannel::GetBaseURI(nsIURI** aBaseURI)
{
=======
nsViewSourceChannel::GetBaseURI(nsIURI** aBaseURI) {
>>>>>>> upstream-releases
  if (mIsSrcdocChannel) {
    nsCOMPtr<nsIInputStreamChannel> isc = do_QueryInterface(mChannel);
    if (isc) {
      return isc->GetBaseURI(aBaseURI);
    }
  }
  *aBaseURI = mBaseURI;
  NS_IF_ADDREF(*aBaseURI);
  return NS_OK;
}

NS_IMETHODIMP
<<<<<<< HEAD
nsViewSourceChannel::SetBaseURI(nsIURI *aBaseURI) {
||||||| merged common ancestors
nsViewSourceChannel::SetBaseURI(nsIURI* aBaseURI)
{
=======
nsViewSourceChannel::SetBaseURI(nsIURI* aBaseURI) {
>>>>>>> upstream-releases
  mBaseURI = aBaseURI;
  return NS_OK;
}

NS_IMETHODIMP
<<<<<<< HEAD
nsViewSourceChannel::GetProtocolVersion(nsACString &aProtocolVersion) {
||||||| merged common ancestors
nsViewSourceChannel::GetProtocolVersion(nsACString& aProtocolVersion)
{
=======
nsViewSourceChannel::GetProtocolVersion(nsACString& aProtocolVersion) {
>>>>>>> upstream-releases
  return NS_ERROR_NOT_IMPLEMENTED;
}

// nsIRequestObserver methods
NS_IMETHODIMP
<<<<<<< HEAD
nsViewSourceChannel::OnStartRequest(nsIRequest *aRequest,
                                    nsISupports *aContext) {
  NS_ENSURE_TRUE(mListener, NS_ERROR_FAILURE);
  // The channel may have gotten redirected... Time to update our info
  mChannel = do_QueryInterface(aRequest);
  mHttpChannel = do_QueryInterface(aRequest);
  mCachingChannel = do_QueryInterface(aRequest);
  mCacheInfoChannel = do_QueryInterface(mChannel);
  mUploadChannel = do_QueryInterface(aRequest);

  nsresult rv = UpdateLoadInfoResultPrincipalURI();
  if (NS_FAILED(rv)) {
    Cancel(rv);
  }
||||||| merged common ancestors
nsViewSourceChannel::OnStartRequest(nsIRequest *aRequest, nsISupports *aContext)
{
    NS_ENSURE_TRUE(mListener, NS_ERROR_FAILURE);
    // The channel may have gotten redirected... Time to update our info
    mChannel = do_QueryInterface(aRequest);
    mHttpChannel = do_QueryInterface(aRequest);
    mCachingChannel = do_QueryInterface(aRequest);
    mCacheInfoChannel = do_QueryInterface(mChannel);
    mUploadChannel = do_QueryInterface(aRequest);

    nsresult rv = UpdateLoadInfoResultPrincipalURI();
    if (NS_FAILED(rv)) {
        Cancel(rv);
    }
=======
nsViewSourceChannel::OnStartRequest(nsIRequest* aRequest) {
  NS_ENSURE_TRUE(mListener, NS_ERROR_FAILURE);
  // The channel may have gotten redirected... Time to update our info
  mChannel = do_QueryInterface(aRequest);
  mHttpChannel = do_QueryInterface(aRequest);
  mCachingChannel = do_QueryInterface(aRequest);
  mCacheInfoChannel = do_QueryInterface(mChannel);
  mUploadChannel = do_QueryInterface(aRequest);

  nsresult rv = UpdateLoadInfoResultPrincipalURI();
  if (NS_FAILED(rv)) {
    Cancel(rv);
  }
>>>>>>> upstream-releases

<<<<<<< HEAD
  return mListener->OnStartRequest(static_cast<nsIViewSourceChannel *>(this),
                                   aContext);
||||||| merged common ancestors
    return mListener->OnStartRequest(static_cast<nsIViewSourceChannel*>
                                                (this),
                                     aContext);
=======
  return mListener->OnStartRequest(static_cast<nsIViewSourceChannel*>(this));
>>>>>>> upstream-releases
}

NS_IMETHODIMP
<<<<<<< HEAD
nsViewSourceChannel::OnStopRequest(nsIRequest *aRequest, nsISupports *aContext,
                                   nsresult aStatus) {
  NS_ENSURE_TRUE(mListener, NS_ERROR_FAILURE);
  if (mChannel) {
    nsCOMPtr<nsILoadGroup> loadGroup;
    mChannel->GetLoadGroup(getter_AddRefs(loadGroup));
    if (loadGroup) {
      loadGroup->RemoveRequest(static_cast<nsIViewSourceChannel *>(this),
                               nullptr, aStatus);
||||||| merged common ancestors
nsViewSourceChannel::OnStopRequest(nsIRequest *aRequest, nsISupports* aContext,
                               nsresult aStatus)
{
    NS_ENSURE_TRUE(mListener, NS_ERROR_FAILURE);
    if (mChannel)
    {
        nsCOMPtr<nsILoadGroup> loadGroup;
        mChannel->GetLoadGroup(getter_AddRefs(loadGroup));
        if (loadGroup)
        {
            loadGroup->RemoveRequest(static_cast<nsIViewSourceChannel*>
                                                (this),
                                     nullptr, aStatus);
        }
=======
nsViewSourceChannel::OnStopRequest(nsIRequest* aRequest, nsresult aStatus) {
  NS_ENSURE_TRUE(mListener, NS_ERROR_FAILURE);
  if (mChannel) {
    nsCOMPtr<nsILoadGroup> loadGroup;
    mChannel->GetLoadGroup(getter_AddRefs(loadGroup));
    if (loadGroup) {
      loadGroup->RemoveRequest(static_cast<nsIViewSourceChannel*>(this),
                               nullptr, aStatus);
>>>>>>> upstream-releases
    }
<<<<<<< HEAD
  }
  return mListener->OnStopRequest(static_cast<nsIViewSourceChannel *>(this),
                                  aContext, aStatus);
||||||| merged common ancestors
    return mListener->OnStopRequest(static_cast<nsIViewSourceChannel*>
                                               (this),
                                    aContext, aStatus);
=======
  }
  return mListener->OnStopRequest(static_cast<nsIViewSourceChannel*>(this),
                                  aStatus);
>>>>>>> upstream-releases
}

// nsIStreamListener methods
NS_IMETHODIMP
<<<<<<< HEAD
nsViewSourceChannel::OnDataAvailable(nsIRequest *aRequest,
                                     nsISupports *aContext,
                                     nsIInputStream *aInputStream,
                                     uint64_t aSourceOffset, uint32_t aLength) {
  NS_ENSURE_TRUE(mListener, NS_ERROR_FAILURE);
  return mListener->OnDataAvailable(static_cast<nsIViewSourceChannel *>(this),
                                    aContext, aInputStream, aSourceOffset,
                                    aLength);
||||||| merged common ancestors
nsViewSourceChannel::OnDataAvailable(nsIRequest *aRequest, nsISupports* aContext,
                                     nsIInputStream *aInputStream,
                                     uint64_t aSourceOffset,
                                     uint32_t aLength)
{
    NS_ENSURE_TRUE(mListener, NS_ERROR_FAILURE);
    return mListener->OnDataAvailable(static_cast<nsIViewSourceChannel*>
                                                 (this),
                                      aContext, aInputStream,
                                      aSourceOffset, aLength);
=======
nsViewSourceChannel::OnDataAvailable(nsIRequest* aRequest,
                                     nsIInputStream* aInputStream,
                                     uint64_t aSourceOffset, uint32_t aLength) {
  NS_ENSURE_TRUE(mListener, NS_ERROR_FAILURE);
  return mListener->OnDataAvailable(static_cast<nsIViewSourceChannel*>(this),
                                    aInputStream, aSourceOffset, aLength);
>>>>>>> upstream-releases
}

// nsIHttpChannel methods

// We want to forward most of nsIHttpChannel over to mHttpChannel, but we want
// to override GetRequestHeader and VisitHeaders. The reason is that we don't
// want various headers like Link: and Refresh: applying to view-source.
NS_IMETHODIMP
<<<<<<< HEAD
nsViewSourceChannel::GetChannelId(uint64_t *aChannelId) {
  NS_ENSURE_ARG_POINTER(aChannelId);
  return !mHttpChannel ? NS_ERROR_NULL_POINTER
                       : mHttpChannel->GetChannelId(aChannelId);
||||||| merged common ancestors
nsViewSourceChannel::GetChannelId(uint64_t *aChannelId)
{
    NS_ENSURE_ARG_POINTER(aChannelId);
  return !mHttpChannel ? NS_ERROR_NULL_POINTER :
      mHttpChannel->GetChannelId(aChannelId);
=======
nsViewSourceChannel::GetChannelId(uint64_t* aChannelId) {
  NS_ENSURE_ARG_POINTER(aChannelId);
  return !mHttpChannel ? NS_ERROR_NULL_POINTER
                       : mHttpChannel->GetChannelId(aChannelId);
>>>>>>> upstream-releases
}

NS_IMETHODIMP
nsViewSourceChannel::SetChannelId(uint64_t aChannelId) {
  return !mHttpChannel ? NS_ERROR_NULL_POINTER
                       : mHttpChannel->SetChannelId(aChannelId);
}

NS_IMETHODIMP
<<<<<<< HEAD
nsViewSourceChannel::GetTopLevelContentWindowId(uint64_t *aWindowId) {
  return !mHttpChannel ? NS_ERROR_NULL_POINTER
                       : mHttpChannel->GetTopLevelContentWindowId(aWindowId);
||||||| merged common ancestors
nsViewSourceChannel::GetTopLevelContentWindowId(uint64_t *aWindowId)
{
  return !mHttpChannel ? NS_ERROR_NULL_POINTER :
      mHttpChannel->GetTopLevelContentWindowId(aWindowId);
=======
nsViewSourceChannel::GetTopLevelContentWindowId(uint64_t* aWindowId) {
  return !mHttpChannel ? NS_ERROR_NULL_POINTER
                       : mHttpChannel->GetTopLevelContentWindowId(aWindowId);
>>>>>>> upstream-releases
}

NS_IMETHODIMP
nsViewSourceChannel::SetTopLevelContentWindowId(uint64_t aWindowId) {
  return !mHttpChannel ? NS_ERROR_NULL_POINTER
                       : mHttpChannel->SetTopLevelContentWindowId(aWindowId);
}

NS_IMETHODIMP
<<<<<<< HEAD
nsViewSourceChannel::GetTopLevelOuterContentWindowId(uint64_t *aWindowId) {
  return !mHttpChannel
             ? NS_ERROR_NULL_POINTER
             : mHttpChannel->GetTopLevelOuterContentWindowId(aWindowId);
||||||| merged common ancestors
nsViewSourceChannel::GetTopLevelOuterContentWindowId(uint64_t *aWindowId)
{
  return !mHttpChannel ? NS_ERROR_NULL_POINTER :
      mHttpChannel->GetTopLevelOuterContentWindowId(aWindowId);
=======
nsViewSourceChannel::GetTopLevelOuterContentWindowId(uint64_t* aWindowId) {
  return !mHttpChannel
             ? NS_ERROR_NULL_POINTER
             : mHttpChannel->GetTopLevelOuterContentWindowId(aWindowId);
>>>>>>> upstream-releases
}

NS_IMETHODIMP
nsViewSourceChannel::SetTopLevelOuterContentWindowId(uint64_t aWindowId) {
  return !mHttpChannel
             ? NS_ERROR_NULL_POINTER
             : mHttpChannel->SetTopLevelOuterContentWindowId(aWindowId);
}

NS_IMETHODIMP
<<<<<<< HEAD
nsViewSourceChannel::GetIsTrackingResource(bool *aIsTrackingResource) {
  return !mHttpChannel
             ? NS_ERROR_NULL_POINTER
             : mHttpChannel->GetIsTrackingResource(aIsTrackingResource);
||||||| merged common ancestors
nsViewSourceChannel::GetIsTrackingResource(bool* aIsTrackingResource)
{
  return !mHttpChannel ? NS_ERROR_NULL_POINTER :
      mHttpChannel->GetIsTrackingResource(aIsTrackingResource);
=======
nsViewSourceChannel::IsTrackingResource(bool* aIsTrackingResource) {
  return !mHttpChannel ? NS_ERROR_NULL_POINTER
                       : mHttpChannel->IsTrackingResource(aIsTrackingResource);
>>>>>>> upstream-releases
}

NS_IMETHODIMP
<<<<<<< HEAD
nsViewSourceChannel::GetIsThirdPartyTrackingResource(
    bool *aIsTrackingResource) {
  return !mHttpChannel ? NS_ERROR_NULL_POINTER
                       : mHttpChannel->GetIsThirdPartyTrackingResource(
                             aIsTrackingResource);
||||||| merged common ancestors
nsViewSourceChannel::GetIsThirdPartyTrackingResource(bool* aIsTrackingResource)
{
  return !mHttpChannel ? NS_ERROR_NULL_POINTER :
      mHttpChannel->GetIsThirdPartyTrackingResource(aIsTrackingResource);
=======
nsViewSourceChannel::IsThirdPartyTrackingResource(bool* aIsTrackingResource) {
  return !mHttpChannel
             ? NS_ERROR_NULL_POINTER
             : mHttpChannel->IsThirdPartyTrackingResource(aIsTrackingResource);
>>>>>>> upstream-releases
}

NS_IMETHODIMP
<<<<<<< HEAD
nsViewSourceChannel::OverrideTrackingFlagsForDocumentCookieAccessor(
    nsIHttpChannel *aDocumentChannel) {
  return !mHttpChannel
             ? NS_ERROR_NULL_POINTER
             : mHttpChannel->OverrideTrackingFlagsForDocumentCookieAccessor(
                   aDocumentChannel);
||||||| merged common ancestors
nsViewSourceChannel::OverrideTrackingFlagsForDocumentCookieAccessor(nsIHttpChannel* aDocumentChannel)
{
  return !mHttpChannel ? NS_ERROR_NULL_POINTER :
      mHttpChannel->OverrideTrackingFlagsForDocumentCookieAccessor(aDocumentChannel);
=======
nsViewSourceChannel::GetClassificationFlags(uint32_t* aClassificationFlags) {
  return !mHttpChannel
             ? NS_ERROR_NULL_POINTER
             : mHttpChannel->GetClassificationFlags(aClassificationFlags);
>>>>>>> upstream-releases
}

NS_IMETHODIMP
<<<<<<< HEAD
nsViewSourceChannel::GetRequestMethod(nsACString &aRequestMethod) {
  return !mHttpChannel ? NS_ERROR_NULL_POINTER
                       : mHttpChannel->GetRequestMethod(aRequestMethod);
||||||| merged common ancestors
nsViewSourceChannel::GetRequestMethod(nsACString & aRequestMethod)
{
    return !mHttpChannel ? NS_ERROR_NULL_POINTER :
        mHttpChannel->GetRequestMethod(aRequestMethod);
=======
nsViewSourceChannel::GetFirstPartyClassificationFlags(
    uint32_t* aClassificationFlags) {
  return !mHttpChannel ? NS_ERROR_NULL_POINTER
                       : mHttpChannel->GetFirstPartyClassificationFlags(
                             aClassificationFlags);
>>>>>>> upstream-releases
}

NS_IMETHODIMP
<<<<<<< HEAD
nsViewSourceChannel::SetRequestMethod(const nsACString &aRequestMethod) {
  return !mHttpChannel ? NS_ERROR_NULL_POINTER
                       : mHttpChannel->SetRequestMethod(aRequestMethod);
||||||| merged common ancestors
nsViewSourceChannel::SetRequestMethod(const nsACString & aRequestMethod)
{
    return !mHttpChannel ? NS_ERROR_NULL_POINTER :
        mHttpChannel->SetRequestMethod(aRequestMethod);
=======
nsViewSourceChannel::GetThirdPartyClassificationFlags(
    uint32_t* aClassificationFlags) {
  return !mHttpChannel ? NS_ERROR_NULL_POINTER
                       : mHttpChannel->GetThirdPartyClassificationFlags(
                             aClassificationFlags);
>>>>>>> upstream-releases
}

NS_IMETHODIMP
<<<<<<< HEAD
nsViewSourceChannel::GetReferrer(nsIURI **aReferrer) {
  return !mHttpChannel ? NS_ERROR_NULL_POINTER
                       : mHttpChannel->GetReferrer(aReferrer);
||||||| merged common ancestors
nsViewSourceChannel::GetReferrer(nsIURI * *aReferrer)
{
    return !mHttpChannel ? NS_ERROR_NULL_POINTER :
        mHttpChannel->GetReferrer(aReferrer);
=======
nsViewSourceChannel::GetFlashPluginState(
    nsIHttpChannel::FlashPluginState* aResult) {
  return !mHttpChannel ? NS_ERROR_NULL_POINTER
                       : mHttpChannel->GetFlashPluginState(aResult);
>>>>>>> upstream-releases
}

NS_IMETHODIMP
<<<<<<< HEAD
nsViewSourceChannel::SetReferrer(nsIURI *aReferrer) {
  return !mHttpChannel ? NS_ERROR_NULL_POINTER
                       : mHttpChannel->SetReferrer(aReferrer);
||||||| merged common ancestors
nsViewSourceChannel::SetReferrer(nsIURI * aReferrer)
{
    return !mHttpChannel ? NS_ERROR_NULL_POINTER :
        mHttpChannel->SetReferrer(aReferrer);
=======
nsViewSourceChannel::GetRequestMethod(nsACString& aRequestMethod) {
  return !mHttpChannel ? NS_ERROR_NULL_POINTER
                       : mHttpChannel->GetRequestMethod(aRequestMethod);
>>>>>>> upstream-releases
}

NS_IMETHODIMP
<<<<<<< HEAD
nsViewSourceChannel::GetReferrerPolicy(uint32_t *aReferrerPolicy) {
  return !mHttpChannel ? NS_ERROR_NULL_POINTER
                       : mHttpChannel->GetReferrerPolicy(aReferrerPolicy);
||||||| merged common ancestors
nsViewSourceChannel::GetReferrerPolicy(uint32_t *aReferrerPolicy)
{
    return !mHttpChannel ? NS_ERROR_NULL_POINTER :
        mHttpChannel->GetReferrerPolicy(aReferrerPolicy);
=======
nsViewSourceChannel::SetRequestMethod(const nsACString& aRequestMethod) {
  return !mHttpChannel ? NS_ERROR_NULL_POINTER
                       : mHttpChannel->SetRequestMethod(aRequestMethod);
>>>>>>> upstream-releases
}

NS_IMETHODIMP
<<<<<<< HEAD
nsViewSourceChannel::SetReferrerWithPolicy(nsIURI *aReferrer,
                                           uint32_t aReferrerPolicy) {
  return !mHttpChannel
             ? NS_ERROR_NULL_POINTER
             : mHttpChannel->SetReferrerWithPolicy(aReferrer, aReferrerPolicy);
||||||| merged common ancestors
nsViewSourceChannel::SetReferrerWithPolicy(nsIURI * aReferrer,
                                           uint32_t aReferrerPolicy)
{
    return !mHttpChannel ? NS_ERROR_NULL_POINTER :
        mHttpChannel->SetReferrerWithPolicy(aReferrer, aReferrerPolicy);
=======
nsViewSourceChannel::GetReferrerInfo(nsIReferrerInfo** aReferrerInfo) {
  return !mHttpChannel ? NS_ERROR_NULL_POINTER
                       : mHttpChannel->GetReferrerInfo(aReferrerInfo);
>>>>>>> upstream-releases
}

NS_IMETHODIMP
<<<<<<< HEAD
nsViewSourceChannel::GetRequestHeader(const nsACString &aHeader,
                                      nsACString &aValue) {
  aValue.Truncate();
  return !mHttpChannel ? NS_ERROR_NULL_POINTER
                       : mHttpChannel->GetRequestHeader(aHeader, aValue);
||||||| merged common ancestors
nsViewSourceChannel::GetRequestHeader(const nsACString & aHeader,
                                      nsACString & aValue)
{
    aValue.Truncate();
    return !mHttpChannel ? NS_ERROR_NULL_POINTER :
        mHttpChannel->GetRequestHeader(aHeader, aValue);
=======
nsViewSourceChannel::SetReferrerInfo(nsIReferrerInfo* aReferrerInfo) {
  return !mHttpChannel ? NS_ERROR_NULL_POINTER
                       : mHttpChannel->SetReferrerInfo(aReferrerInfo);
}

NS_IMETHODIMP nsViewSourceChannel::SetReferrerInfoWithoutClone(
    nsIReferrerInfo* aReferrerInfo) {
  return !mHttpChannel
             ? NS_ERROR_NULL_POINTER
             : mHttpChannel->SetReferrerInfoWithoutClone(aReferrerInfo);
>>>>>>> upstream-releases
}

NS_IMETHODIMP
<<<<<<< HEAD
nsViewSourceChannel::SetRequestHeader(const nsACString &aHeader,
                                      const nsACString &aValue, bool aMerge) {
  return !mHttpChannel
             ? NS_ERROR_NULL_POINTER
             : mHttpChannel->SetRequestHeader(aHeader, aValue, aMerge);
||||||| merged common ancestors
nsViewSourceChannel::SetRequestHeader(const nsACString & aHeader,
                                      const nsACString & aValue,
                                      bool aMerge)
{
    return !mHttpChannel ? NS_ERROR_NULL_POINTER :
        mHttpChannel->SetRequestHeader(aHeader, aValue, aMerge);
=======
nsViewSourceChannel::GetRequestHeader(const nsACString& aHeader,
                                      nsACString& aValue) {
  aValue.Truncate();
  return !mHttpChannel ? NS_ERROR_NULL_POINTER
                       : mHttpChannel->GetRequestHeader(aHeader, aValue);
>>>>>>> upstream-releases
}

NS_IMETHODIMP
<<<<<<< HEAD
nsViewSourceChannel::SetEmptyRequestHeader(const nsACString &aHeader) {
  return !mHttpChannel ? NS_ERROR_NULL_POINTER
                       : mHttpChannel->SetEmptyRequestHeader(aHeader);
||||||| merged common ancestors
nsViewSourceChannel::SetEmptyRequestHeader(const nsACString & aHeader)
{
    return !mHttpChannel ? NS_ERROR_NULL_POINTER :
        mHttpChannel->SetEmptyRequestHeader(aHeader);
=======
nsViewSourceChannel::SetRequestHeader(const nsACString& aHeader,
                                      const nsACString& aValue, bool aMerge) {
  return !mHttpChannel
             ? NS_ERROR_NULL_POINTER
             : mHttpChannel->SetRequestHeader(aHeader, aValue, aMerge);
>>>>>>> upstream-releases
}

NS_IMETHODIMP
<<<<<<< HEAD
nsViewSourceChannel::VisitRequestHeaders(nsIHttpHeaderVisitor *aVisitor) {
  return !mHttpChannel ? NS_ERROR_NULL_POINTER
                       : mHttpChannel->VisitRequestHeaders(aVisitor);
||||||| merged common ancestors
nsViewSourceChannel::VisitRequestHeaders(nsIHttpHeaderVisitor *aVisitor)
{
    return !mHttpChannel ? NS_ERROR_NULL_POINTER :
        mHttpChannel->VisitRequestHeaders(aVisitor);
=======
nsViewSourceChannel::SetEmptyRequestHeader(const nsACString& aHeader) {
  return !mHttpChannel ? NS_ERROR_NULL_POINTER
                       : mHttpChannel->SetEmptyRequestHeader(aHeader);
>>>>>>> upstream-releases
}

NS_IMETHODIMP
<<<<<<< HEAD
nsViewSourceChannel::VisitNonDefaultRequestHeaders(
    nsIHttpHeaderVisitor *aVisitor) {
  return !mHttpChannel ? NS_ERROR_NULL_POINTER
                       : mHttpChannel->VisitNonDefaultRequestHeaders(aVisitor);
||||||| merged common ancestors
nsViewSourceChannel::VisitNonDefaultRequestHeaders(nsIHttpHeaderVisitor *aVisitor)
{
    return !mHttpChannel ? NS_ERROR_NULL_POINTER :
        mHttpChannel->VisitNonDefaultRequestHeaders(aVisitor);
=======
nsViewSourceChannel::VisitRequestHeaders(nsIHttpHeaderVisitor* aVisitor) {
  return !mHttpChannel ? NS_ERROR_NULL_POINTER
                       : mHttpChannel->VisitRequestHeaders(aVisitor);
>>>>>>> upstream-releases
}

NS_IMETHODIMP
<<<<<<< HEAD
nsViewSourceChannel::GetAllowPipelining(bool *aAllowPipelining) {
  return !mHttpChannel ? NS_ERROR_NULL_POINTER
                       : mHttpChannel->GetAllowPipelining(aAllowPipelining);
||||||| merged common ancestors
nsViewSourceChannel::GetAllowPipelining(bool *aAllowPipelining)
{
    return !mHttpChannel ? NS_ERROR_NULL_POINTER :
        mHttpChannel->GetAllowPipelining(aAllowPipelining);
=======
nsViewSourceChannel::VisitNonDefaultRequestHeaders(
    nsIHttpHeaderVisitor* aVisitor) {
  return !mHttpChannel ? NS_ERROR_NULL_POINTER
                       : mHttpChannel->VisitNonDefaultRequestHeaders(aVisitor);
>>>>>>> upstream-releases
}

NS_IMETHODIMP
<<<<<<< HEAD
nsViewSourceChannel::SetAllowPipelining(bool aAllowPipelining) {
  return !mHttpChannel ? NS_ERROR_NULL_POINTER
                       : mHttpChannel->SetAllowPipelining(aAllowPipelining);
||||||| merged common ancestors
nsViewSourceChannel::SetAllowPipelining(bool aAllowPipelining)
{
    return !mHttpChannel ? NS_ERROR_NULL_POINTER :
        mHttpChannel->SetAllowPipelining(aAllowPipelining);
=======
nsViewSourceChannel::GetAllowPipelining(bool* aAllowPipelining) {
  return !mHttpChannel ? NS_ERROR_NULL_POINTER
                       : mHttpChannel->GetAllowPipelining(aAllowPipelining);
>>>>>>> upstream-releases
}

NS_IMETHODIMP
<<<<<<< HEAD
nsViewSourceChannel::GetAllowSTS(bool *aAllowSTS) {
  return !mHttpChannel ? NS_ERROR_NULL_POINTER
                       : mHttpChannel->GetAllowSTS(aAllowSTS);
||||||| merged common ancestors
nsViewSourceChannel::GetAllowSTS(bool *aAllowSTS)
{
    return !mHttpChannel ? NS_ERROR_NULL_POINTER :
        mHttpChannel->GetAllowSTS(aAllowSTS);
=======
nsViewSourceChannel::SetAllowPipelining(bool aAllowPipelining) {
  return !mHttpChannel ? NS_ERROR_NULL_POINTER
                       : mHttpChannel->SetAllowPipelining(aAllowPipelining);
>>>>>>> upstream-releases
}

NS_IMETHODIMP
<<<<<<< HEAD
nsViewSourceChannel::SetAllowSTS(bool aAllowSTS) {
  return !mHttpChannel ? NS_ERROR_NULL_POINTER
                       : mHttpChannel->SetAllowSTS(aAllowSTS);
||||||| merged common ancestors
nsViewSourceChannel::SetAllowSTS(bool aAllowSTS)
{
    return !mHttpChannel ? NS_ERROR_NULL_POINTER :
        mHttpChannel->SetAllowSTS(aAllowSTS);
=======
nsViewSourceChannel::GetAllowSTS(bool* aAllowSTS) {
  return !mHttpChannel ? NS_ERROR_NULL_POINTER
                       : mHttpChannel->GetAllowSTS(aAllowSTS);
>>>>>>> upstream-releases
}

NS_IMETHODIMP
<<<<<<< HEAD
nsViewSourceChannel::GetRedirectionLimit(uint32_t *aRedirectionLimit) {
  return !mHttpChannel ? NS_ERROR_NULL_POINTER
                       : mHttpChannel->GetRedirectionLimit(aRedirectionLimit);
||||||| merged common ancestors
nsViewSourceChannel::GetRedirectionLimit(uint32_t *aRedirectionLimit)
{
    return !mHttpChannel ? NS_ERROR_NULL_POINTER :
        mHttpChannel->GetRedirectionLimit(aRedirectionLimit);
=======
nsViewSourceChannel::SetAllowSTS(bool aAllowSTS) {
  return !mHttpChannel ? NS_ERROR_NULL_POINTER
                       : mHttpChannel->SetAllowSTS(aAllowSTS);
>>>>>>> upstream-releases
}

NS_IMETHODIMP
<<<<<<< HEAD
nsViewSourceChannel::SetRedirectionLimit(uint32_t aRedirectionLimit) {
  return !mHttpChannel ? NS_ERROR_NULL_POINTER
                       : mHttpChannel->SetRedirectionLimit(aRedirectionLimit);
||||||| merged common ancestors
nsViewSourceChannel::SetRedirectionLimit(uint32_t aRedirectionLimit)
{
    return !mHttpChannel ? NS_ERROR_NULL_POINTER :
        mHttpChannel->SetRedirectionLimit(aRedirectionLimit);
=======
nsViewSourceChannel::GetRedirectionLimit(uint32_t* aRedirectionLimit) {
  return !mHttpChannel ? NS_ERROR_NULL_POINTER
                       : mHttpChannel->GetRedirectionLimit(aRedirectionLimit);
>>>>>>> upstream-releases
}

NS_IMETHODIMP
<<<<<<< HEAD
nsViewSourceChannel::GetResponseStatus(uint32_t *aResponseStatus) {
  return !mHttpChannel ? NS_ERROR_NULL_POINTER
                       : mHttpChannel->GetResponseStatus(aResponseStatus);
||||||| merged common ancestors
nsViewSourceChannel::GetResponseStatus(uint32_t *aResponseStatus)
{
    return !mHttpChannel ? NS_ERROR_NULL_POINTER :
        mHttpChannel->GetResponseStatus(aResponseStatus);
=======
nsViewSourceChannel::SetRedirectionLimit(uint32_t aRedirectionLimit) {
  return !mHttpChannel ? NS_ERROR_NULL_POINTER
                       : mHttpChannel->SetRedirectionLimit(aRedirectionLimit);
>>>>>>> upstream-releases
}

NS_IMETHODIMP
<<<<<<< HEAD
nsViewSourceChannel::GetResponseStatusText(nsACString &aResponseStatusText) {
  return !mHttpChannel
             ? NS_ERROR_NULL_POINTER
             : mHttpChannel->GetResponseStatusText(aResponseStatusText);
||||||| merged common ancestors
nsViewSourceChannel::GetResponseStatusText(nsACString & aResponseStatusText)
{
    return !mHttpChannel ? NS_ERROR_NULL_POINTER :
        mHttpChannel->GetResponseStatusText(aResponseStatusText);
=======
nsViewSourceChannel::GetResponseStatus(uint32_t* aResponseStatus) {
  return !mHttpChannel ? NS_ERROR_NULL_POINTER
                       : mHttpChannel->GetResponseStatus(aResponseStatus);
>>>>>>> upstream-releases
}

NS_IMETHODIMP
<<<<<<< HEAD
nsViewSourceChannel::GetRequestSucceeded(bool *aRequestSucceeded) {
  return !mHttpChannel ? NS_ERROR_NULL_POINTER
                       : mHttpChannel->GetRequestSucceeded(aRequestSucceeded);
||||||| merged common ancestors
nsViewSourceChannel::GetRequestSucceeded(bool *aRequestSucceeded)
{
    return !mHttpChannel ? NS_ERROR_NULL_POINTER :
        mHttpChannel->GetRequestSucceeded(aRequestSucceeded);
=======
nsViewSourceChannel::GetResponseStatusText(nsACString& aResponseStatusText) {
  return !mHttpChannel
             ? NS_ERROR_NULL_POINTER
             : mHttpChannel->GetResponseStatusText(aResponseStatusText);
>>>>>>> upstream-releases
}

NS_IMETHODIMP
<<<<<<< HEAD
nsViewSourceChannel::GetResponseHeader(const nsACString &aHeader,
                                       nsACString &aValue) {
  aValue.Truncate();
  if (!mHttpChannel) return NS_ERROR_NULL_POINTER;
||||||| merged common ancestors
nsViewSourceChannel::GetResponseHeader(const nsACString & aHeader,
                                       nsACString & aValue)
{
    aValue.Truncate();
    if (!mHttpChannel)
        return NS_ERROR_NULL_POINTER;
=======
nsViewSourceChannel::GetRequestSucceeded(bool* aRequestSucceeded) {
  return !mHttpChannel ? NS_ERROR_NULL_POINTER
                       : mHttpChannel->GetRequestSucceeded(aRequestSucceeded);
}
>>>>>>> upstream-releases

<<<<<<< HEAD
  if (!aHeader.Equals(NS_LITERAL_CSTRING("Content-Type"),
                      nsCaseInsensitiveCStringComparator()) &&
      !aHeader.Equals(NS_LITERAL_CSTRING("Content-Security-Policy"),
                      nsCaseInsensitiveCStringComparator()) &&
      !aHeader.Equals(NS_LITERAL_CSTRING("Content-Security-Policy-Report-Only"),
                      nsCaseInsensitiveCStringComparator()) &&
      !aHeader.Equals(NS_LITERAL_CSTRING("X-Frame-Options"),
                      nsCaseInsensitiveCStringComparator())) {
    // We simulate the NS_ERROR_NOT_AVAILABLE error which is produced by
    // GetResponseHeader via nsHttpHeaderArray::GetHeader when the entry is
    // not present, such that it appears as though no headers except for the
    // whitelisted ones were set on this channel.
    return NS_ERROR_NOT_AVAILABLE;
  }
||||||| merged common ancestors
    if (!aHeader.Equals(NS_LITERAL_CSTRING("Content-Type"),
                        nsCaseInsensitiveCStringComparator()) &&
        !aHeader.Equals(NS_LITERAL_CSTRING("Content-Security-Policy"),
                        nsCaseInsensitiveCStringComparator()) &&
        !aHeader.Equals(NS_LITERAL_CSTRING("Content-Security-Policy-Report-Only"),
                        nsCaseInsensitiveCStringComparator()) &&
        !aHeader.Equals(NS_LITERAL_CSTRING("X-Frame-Options"),
                        nsCaseInsensitiveCStringComparator())) {
        // We simulate the NS_ERROR_NOT_AVAILABLE error which is produced by
        // GetResponseHeader via nsHttpHeaderArray::GetHeader when the entry is
        // not present, such that it appears as though no headers except for the
        // whitelisted ones were set on this channel.
        return NS_ERROR_NOT_AVAILABLE;
    }
=======
NS_IMETHODIMP
nsViewSourceChannel::GetResponseHeader(const nsACString& aHeader,
                                       nsACString& aValue) {
  aValue.Truncate();
  if (!mHttpChannel) return NS_ERROR_NULL_POINTER;
>>>>>>> upstream-releases

<<<<<<< HEAD
  return mHttpChannel->GetResponseHeader(aHeader, aValue);
||||||| merged common ancestors
    return mHttpChannel->GetResponseHeader(aHeader, aValue);
=======
  if (!aHeader.Equals(NS_LITERAL_CSTRING("Content-Type"),
                      nsCaseInsensitiveCStringComparator()) &&
      !aHeader.Equals(NS_LITERAL_CSTRING("Content-Security-Policy"),
                      nsCaseInsensitiveCStringComparator()) &&
      !aHeader.Equals(NS_LITERAL_CSTRING("Content-Security-Policy-Report-Only"),
                      nsCaseInsensitiveCStringComparator()) &&
      !aHeader.Equals(NS_LITERAL_CSTRING("X-Frame-Options"),
                      nsCaseInsensitiveCStringComparator())) {
    // We simulate the NS_ERROR_NOT_AVAILABLE error which is produced by
    // GetResponseHeader via nsHttpHeaderArray::GetHeader when the entry is
    // not present, such that it appears as though no headers except for the
    // whitelisted ones were set on this channel.
    return NS_ERROR_NOT_AVAILABLE;
  }

  return mHttpChannel->GetResponseHeader(aHeader, aValue);
>>>>>>> upstream-releases
}

NS_IMETHODIMP
<<<<<<< HEAD
nsViewSourceChannel::SetResponseHeader(const nsACString &header,
                                       const nsACString &value, bool merge) {
  return !mHttpChannel ? NS_ERROR_NULL_POINTER
                       : mHttpChannel->SetResponseHeader(header, value, merge);
||||||| merged common ancestors
nsViewSourceChannel::SetResponseHeader(const nsACString & header,
                                       const nsACString & value, bool merge)
{
    return !mHttpChannel ? NS_ERROR_NULL_POINTER :
        mHttpChannel->SetResponseHeader(header, value, merge);
=======
nsViewSourceChannel::SetResponseHeader(const nsACString& header,
                                       const nsACString& value, bool merge) {
  return !mHttpChannel ? NS_ERROR_NULL_POINTER
                       : mHttpChannel->SetResponseHeader(header, value, merge);
>>>>>>> upstream-releases
}

NS_IMETHODIMP
<<<<<<< HEAD
nsViewSourceChannel::VisitResponseHeaders(nsIHttpHeaderVisitor *aVisitor) {
  if (!mHttpChannel) return NS_ERROR_NULL_POINTER;
||||||| merged common ancestors
nsViewSourceChannel::VisitResponseHeaders(nsIHttpHeaderVisitor *aVisitor)
{
    if (!mHttpChannel)
        return NS_ERROR_NULL_POINTER;
=======
nsViewSourceChannel::VisitResponseHeaders(nsIHttpHeaderVisitor* aVisitor) {
  if (!mHttpChannel) return NS_ERROR_NULL_POINTER;
>>>>>>> upstream-releases

  NS_NAMED_LITERAL_CSTRING(contentTypeStr, "Content-Type");
  nsAutoCString contentType;
  nsresult rv = mHttpChannel->GetResponseHeader(contentTypeStr, contentType);
  if (NS_SUCCEEDED(rv)) {
    return aVisitor->VisitHeader(contentTypeStr, contentType);
  }
  return NS_OK;
}

NS_IMETHODIMP
<<<<<<< HEAD
nsViewSourceChannel::GetOriginalResponseHeader(const nsACString &aHeader,
                                               nsIHttpHeaderVisitor *aVisitor) {
  nsAutoCString value;
  nsresult rv = GetResponseHeader(aHeader, value);
  if (NS_FAILED(rv)) {
    return rv;
  }
  return aVisitor->VisitHeader(aHeader, value);
||||||| merged common ancestors
nsViewSourceChannel::GetOriginalResponseHeader(const nsACString & aHeader,
                                               nsIHttpHeaderVisitor *aVisitor)
{
    nsAutoCString value;
    nsresult rv = GetResponseHeader(aHeader, value);
    if (NS_FAILED(rv)) {
        return rv;
    }
    return aVisitor->VisitHeader(aHeader, value);
=======
nsViewSourceChannel::GetOriginalResponseHeader(const nsACString& aHeader,
                                               nsIHttpHeaderVisitor* aVisitor) {
  nsAutoCString value;
  nsresult rv = GetResponseHeader(aHeader, value);
  if (NS_FAILED(rv)) {
    return rv;
  }
  return aVisitor->VisitHeader(aHeader, value);
>>>>>>> upstream-releases
}

NS_IMETHODIMP
<<<<<<< HEAD
nsViewSourceChannel::VisitOriginalResponseHeaders(
    nsIHttpHeaderVisitor *aVisitor) {
  return VisitResponseHeaders(aVisitor);
||||||| merged common ancestors
nsViewSourceChannel::VisitOriginalResponseHeaders(nsIHttpHeaderVisitor *aVisitor)
{
    return VisitResponseHeaders(aVisitor);
=======
nsViewSourceChannel::VisitOriginalResponseHeaders(
    nsIHttpHeaderVisitor* aVisitor) {
  return VisitResponseHeaders(aVisitor);
>>>>>>> upstream-releases
}

NS_IMETHODIMP
<<<<<<< HEAD
nsViewSourceChannel::IsNoStoreResponse(bool *_retval) {
  return !mHttpChannel ? NS_ERROR_NULL_POINTER
                       : mHttpChannel->IsNoStoreResponse(_retval);
||||||| merged common ancestors
nsViewSourceChannel::IsNoStoreResponse(bool *_retval)
{
    return !mHttpChannel ? NS_ERROR_NULL_POINTER :
        mHttpChannel->IsNoStoreResponse(_retval);
=======
nsViewSourceChannel::IsNoStoreResponse(bool* _retval) {
  return !mHttpChannel ? NS_ERROR_NULL_POINTER
                       : mHttpChannel->IsNoStoreResponse(_retval);
>>>>>>> upstream-releases
}

NS_IMETHODIMP
<<<<<<< HEAD
nsViewSourceChannel::IsNoCacheResponse(bool *_retval) {
  return !mHttpChannel ? NS_ERROR_NULL_POINTER
                       : mHttpChannel->IsNoCacheResponse(_retval);
||||||| merged common ancestors
nsViewSourceChannel::IsNoCacheResponse(bool *_retval)
{
    return !mHttpChannel ? NS_ERROR_NULL_POINTER :
        mHttpChannel->IsNoCacheResponse(_retval);
=======
nsViewSourceChannel::IsNoCacheResponse(bool* _retval) {
  return !mHttpChannel ? NS_ERROR_NULL_POINTER
                       : mHttpChannel->IsNoCacheResponse(_retval);
>>>>>>> upstream-releases
}

NS_IMETHODIMP
<<<<<<< HEAD
nsViewSourceChannel::IsPrivateResponse(bool *_retval) {
  return !mHttpChannel ? NS_ERROR_NULL_POINTER
                       : mHttpChannel->IsPrivateResponse(_retval);
||||||| merged common ancestors
nsViewSourceChannel::IsPrivateResponse(bool *_retval)
{
    return !mHttpChannel ? NS_ERROR_NULL_POINTER :
        mHttpChannel->IsPrivateResponse(_retval);
=======
nsViewSourceChannel::IsPrivateResponse(bool* _retval) {
  return !mHttpChannel ? NS_ERROR_NULL_POINTER
                       : mHttpChannel->IsPrivateResponse(_retval);
>>>>>>> upstream-releases
}

NS_IMETHODIMP
<<<<<<< HEAD
nsViewSourceChannel::RedirectTo(nsIURI *uri) {
  return !mHttpChannel ? NS_ERROR_NULL_POINTER : mHttpChannel->RedirectTo(uri);
||||||| merged common ancestors
nsViewSourceChannel::RedirectTo(nsIURI *uri)
{
    return !mHttpChannel ? NS_ERROR_NULL_POINTER :
        mHttpChannel->RedirectTo(uri);
=======
nsViewSourceChannel::RedirectTo(nsIURI* uri) {
  return !mHttpChannel ? NS_ERROR_NULL_POINTER : mHttpChannel->RedirectTo(uri);
>>>>>>> upstream-releases
}

NS_IMETHODIMP
<<<<<<< HEAD
nsViewSourceChannel::UpgradeToSecure() {
  return !mHttpChannel ? NS_ERROR_NULL_POINTER
                       : mHttpChannel->UpgradeToSecure();
||||||| merged common ancestors
nsViewSourceChannel::UpgradeToSecure()
{
    return !mHttpChannel ? NS_ERROR_NULL_POINTER :
        mHttpChannel->UpgradeToSecure();
=======
nsViewSourceChannel::SwitchProcessTo(mozilla::dom::Promise* aBrowserParent,
                                     uint64_t aIdentifier) {
  return !mHttpChannel
             ? NS_ERROR_NULL_POINTER
             : mHttpChannel->SwitchProcessTo(aBrowserParent, aIdentifier);
>>>>>>> upstream-releases
}

NS_IMETHODIMP
<<<<<<< HEAD
nsViewSourceChannel::GetRequestContextID(uint64_t *_retval) {
  return !mHttpChannel ? NS_ERROR_NULL_POINTER
                       : mHttpChannel->GetRequestContextID(_retval);
||||||| merged common ancestors
nsViewSourceChannel::GetRequestContextID(uint64_t *_retval)
{
    return !mHttpChannel ? NS_ERROR_NULL_POINTER :
        mHttpChannel->GetRequestContextID(_retval);
=======
nsViewSourceChannel::HasCrossOriginOpenerPolicyMismatch(bool* aMismatch) {
  MOZ_ASSERT(aMismatch);
  if (!aMismatch) {
    return NS_ERROR_INVALID_ARG;
  }
  *aMismatch = false;
  return NS_OK;
>>>>>>> upstream-releases
}

NS_IMETHODIMP
<<<<<<< HEAD
nsViewSourceChannel::SetRequestContextID(uint64_t rcid) {
  return !mHttpChannel ? NS_ERROR_NULL_POINTER
                       : mHttpChannel->SetRequestContextID(rcid);
||||||| merged common ancestors
nsViewSourceChannel::SetRequestContextID(uint64_t rcid)
{
    return !mHttpChannel ? NS_ERROR_NULL_POINTER :
        mHttpChannel->SetRequestContextID(rcid);
=======
nsViewSourceChannel::UpgradeToSecure() {
  return !mHttpChannel ? NS_ERROR_NULL_POINTER
                       : mHttpChannel->UpgradeToSecure();
>>>>>>> upstream-releases
}

NS_IMETHODIMP
<<<<<<< HEAD
nsViewSourceChannel::GetIsMainDocumentChannel(bool *aValue) {
  return !mHttpChannel ? NS_ERROR_NULL_POINTER
                       : mHttpChannel->GetIsMainDocumentChannel(aValue);
||||||| merged common ancestors
nsViewSourceChannel::GetIsMainDocumentChannel(bool* aValue)
{
    return !mHttpChannel ? NS_ERROR_NULL_POINTER :
        mHttpChannel->GetIsMainDocumentChannel(aValue);
=======
nsViewSourceChannel::GetRequestContextID(uint64_t* _retval) {
  return !mHttpChannel ? NS_ERROR_NULL_POINTER
                       : mHttpChannel->GetRequestContextID(_retval);
>>>>>>> upstream-releases
}

NS_IMETHODIMP
<<<<<<< HEAD
nsViewSourceChannel::SetIsMainDocumentChannel(bool aValue) {
  return !mHttpChannel ? NS_ERROR_NULL_POINTER
                       : mHttpChannel->SetIsMainDocumentChannel(aValue);
||||||| merged common ancestors
nsViewSourceChannel::SetIsMainDocumentChannel(bool aValue)
{
    return !mHttpChannel ? NS_ERROR_NULL_POINTER :
        mHttpChannel->SetIsMainDocumentChannel(aValue);
=======
nsViewSourceChannel::SetRequestContextID(uint64_t rcid) {
  return !mHttpChannel ? NS_ERROR_NULL_POINTER
                       : mHttpChannel->SetRequestContextID(rcid);
}

NS_IMETHODIMP
nsViewSourceChannel::GetIsMainDocumentChannel(bool* aValue) {
  return !mHttpChannel ? NS_ERROR_NULL_POINTER
                       : mHttpChannel->GetIsMainDocumentChannel(aValue);
}

NS_IMETHODIMP
nsViewSourceChannel::SetIsMainDocumentChannel(bool aValue) {
  return !mHttpChannel ? NS_ERROR_NULL_POINTER
                       : mHttpChannel->SetIsMainDocumentChannel(aValue);
>>>>>>> upstream-releases
}

// Have to manually forward SetCorsPreflightParameters since it's [notxpcom]
<<<<<<< HEAD
void nsViewSourceChannel::SetCorsPreflightParameters(
    const nsTArray<nsCString> &aUnsafeHeaders) {
||||||| merged common ancestors
void
nsViewSourceChannel::SetCorsPreflightParameters(const nsTArray<nsCString>& aUnsafeHeaders)
{
=======
void nsViewSourceChannel::SetCorsPreflightParameters(
    const nsTArray<nsCString>& aUnsafeHeaders) {
>>>>>>> upstream-releases
  mHttpChannelInternal->SetCorsPreflightParameters(aUnsafeHeaders);
}

void nsViewSourceChannel::SetAltDataForChild(bool aIsForChild) {
  mHttpChannelInternal->SetAltDataForChild(aIsForChild);
}

NS_IMETHODIMP
<<<<<<< HEAD
nsViewSourceChannel::LogBlockedCORSRequest(const nsAString &aMessage,
                                           const nsACString &aCategory) {
||||||| merged common ancestors
nsViewSourceChannel::LogBlockedCORSRequest(const nsAString& aMessage,
                                           const nsACString& aCategory)
{
=======
nsViewSourceChannel::LogBlockedCORSRequest(const nsAString& aMessage,
                                           const nsACString& aCategory) {
>>>>>>> upstream-releases
  if (!mHttpChannel) {
    NS_WARNING(
        "nsViewSourceChannel::LogBlockedCORSRequest mHttpChannel is null");
    return NS_ERROR_UNEXPECTED;
  }
  return mHttpChannel->LogBlockedCORSRequest(aMessage, aCategory);
}

<<<<<<< HEAD
const nsTArray<mozilla::Tuple<nsCString, nsCString>>
    &nsViewSourceChannel::PreferredAlternativeDataTypes() {
  if (mCacheInfoChannel) {
    return mCacheInfoChannel->PreferredAlternativeDataTypes();
  }
  return mEmptyArray;
}

void nsViewSourceChannel::SetIPv4Disabled() {
  if (mHttpChannelInternal) {
    mHttpChannelInternal->SetIPv4Disabled();
  }
}

void nsViewSourceChannel::SetIPv6Disabled() {
  if (mHttpChannelInternal) {
    mHttpChannelInternal->SetIPv6Disabled();
  }
||||||| merged common ancestors
const nsTArray<mozilla::Tuple<nsCString, nsCString>>&
nsViewSourceChannel::PreferredAlternativeDataTypes()
{
    if (mCacheInfoChannel) {
        return mCacheInfoChannel->PreferredAlternativeDataTypes();
    }
    return mEmptyArray;
=======
NS_IMETHODIMP
nsViewSourceChannel::LogMimeTypeMismatch(const nsACString& aMessageName,
                                         bool aWarning, const nsAString& aURL,
                                         const nsAString& aContentType) {
  if (!mHttpChannel) {
    NS_WARNING("nsViewSourceChannel::LogMimeTypeMismatch mHttpChannel is null");
    return NS_ERROR_UNEXPECTED;
  }
  return mHttpChannel->LogMimeTypeMismatch(aMessageName, aWarning, aURL,
                                           aContentType);
}

const nsTArray<mozilla::net::PreferredAlternativeDataTypeParams>&
nsViewSourceChannel::PreferredAlternativeDataTypes() {
  if (mCacheInfoChannel) {
    return mCacheInfoChannel->PreferredAlternativeDataTypes();
  }
  return mEmptyArray;
}

void nsViewSourceChannel::SetIPv4Disabled() {
  if (mHttpChannelInternal) {
    mHttpChannelInternal->SetIPv4Disabled();
  }
}

void nsViewSourceChannel::SetIPv6Disabled() {
  if (mHttpChannelInternal) {
    mHttpChannelInternal->SetIPv6Disabled();
  }
}

bool nsViewSourceChannel::GetHasSandboxedAuxiliaryNavigations() {
  if (mHttpChannelInternal) {
    return mHttpChannelInternal->GetHasSandboxedAuxiliaryNavigations();
  }
  return false;
}

void nsViewSourceChannel::SetHasSandboxedAuxiliaryNavigations(
    bool aHasSandboxedAuxiliaryNavigations) {
  if (mHttpChannelInternal) {
    mHttpChannelInternal->SetHasSandboxedAuxiliaryNavigations(
        aHasSandboxedAuxiliaryNavigations);
  }
>>>>>>> upstream-releases
}
