/* -*- Mode: C++; tab-width: 2; indent-tabs-mode: nil; c-basic-offset: 2 -*- */
// vim:ts=4 sw=2 sts=2 et cin:
/* This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/. */

#include "nsIFile.h"
#include "nsFileProtocolHandler.h"
#include "nsFileChannel.h"
#include "nsStandardURL.h"
#include "nsURLHelper.h"
#include "nsIURIMutator.h"

#include "nsNetUtil.h"

#include "FileChannelChild.h"

// URL file handling, copied and modified from
// xpfe/components/bookmarks/src/nsBookmarksService.cpp
#ifdef XP_WIN
#  include <shlobj.h>
#  include <intshcut.h>
#  include "nsIFileURL.h"
#  ifdef CompareString
#    undef CompareString
#  endif
#endif

// URL file handling for freedesktop.org
#ifdef XP_UNIX
#  include "nsINIParser.h"
#  define DESKTOP_ENTRY_SECTION "Desktop Entry"
#endif

//-----------------------------------------------------------------------------

nsresult nsFileProtocolHandler::Init() { return NS_OK; }

NS_IMPL_ISUPPORTS(nsFileProtocolHandler, nsIFileProtocolHandler,
                  nsIProtocolHandler, nsISupportsWeakReference)

//-----------------------------------------------------------------------------
// nsIProtocolHandler methods:

#if defined(XP_WIN)
NS_IMETHODIMP
<<<<<<< HEAD
nsFileProtocolHandler::ReadURLFile(nsIFile *aFile, nsIURI **aURI) {
  nsAutoString path;
  nsresult rv = aFile->GetPath(path);
  if (NS_FAILED(rv)) return rv;

  if (path.Length() < 4) return NS_ERROR_NOT_AVAILABLE;
  if (!StringTail(path, 4).LowerCaseEqualsLiteral(".url"))
    return NS_ERROR_NOT_AVAILABLE;

  HRESULT result;

  rv = NS_ERROR_NOT_AVAILABLE;

  IUniformResourceLocatorW *urlLink = nullptr;
  result =
      ::CoCreateInstance(CLSID_InternetShortcut, nullptr, CLSCTX_INPROC_SERVER,
                         IID_IUniformResourceLocatorW, (void **)&urlLink);
  if (SUCCEEDED(result) && urlLink) {
    IPersistFile *urlFile = nullptr;
    result = urlLink->QueryInterface(IID_IPersistFile, (void **)&urlFile);
    if (SUCCEEDED(result) && urlFile) {
      result = urlFile->Load(path.get(), STGM_READ);
      if (SUCCEEDED(result)) {
        LPWSTR lpTemp = nullptr;

        // The URL this method will give us back seems to be already
        // escaped. Hence, do not do escaping of our own.
        result = urlLink->GetURL(&lpTemp);
        if (SUCCEEDED(result) && lpTemp) {
          rv = NS_NewURI(aURI, nsDependentString(lpTemp));
          // free the string that GetURL alloc'd
          CoTaskMemFree(lpTemp);
||||||| merged common ancestors
nsFileProtocolHandler::ReadURLFile(nsIFile* aFile, nsIURI** aURI)
{
    nsAutoString path;
    nsresult rv = aFile->GetPath(path);
    if (NS_FAILED(rv))
        return rv;

    if (path.Length() < 4)
        return NS_ERROR_NOT_AVAILABLE;
    if (!StringTail(path, 4).LowerCaseEqualsLiteral(".url"))
        return NS_ERROR_NOT_AVAILABLE;

    HRESULT result;

    rv = NS_ERROR_NOT_AVAILABLE;

    IUniformResourceLocatorW* urlLink = nullptr;
    result = ::CoCreateInstance(CLSID_InternetShortcut, nullptr, CLSCTX_INPROC_SERVER,
                                IID_IUniformResourceLocatorW, (void**)&urlLink);
    if (SUCCEEDED(result) && urlLink) {
        IPersistFile* urlFile = nullptr;
        result = urlLink->QueryInterface(IID_IPersistFile, (void**)&urlFile);
        if (SUCCEEDED(result) && urlFile) {
            result = urlFile->Load(path.get(), STGM_READ);
            if (SUCCEEDED(result) ) {
                LPWSTR lpTemp = nullptr;

                // The URL this method will give us back seems to be already
                // escaped. Hence, do not do escaping of our own.
                result = urlLink->GetURL(&lpTemp);
                if (SUCCEEDED(result) && lpTemp) {
                    rv = NS_NewURI(aURI, nsDependentString(lpTemp));
                    // free the string that GetURL alloc'd
                    CoTaskMemFree(lpTemp);
                }
            }
            urlFile->Release();
=======
nsFileProtocolHandler::ReadURLFile(nsIFile* aFile, nsIURI** aURI) {
  nsAutoString path;
  nsresult rv = aFile->GetPath(path);
  if (NS_FAILED(rv)) return rv;

  if (path.Length() < 4) return NS_ERROR_NOT_AVAILABLE;
  if (!StringTail(path, 4).LowerCaseEqualsLiteral(".url"))
    return NS_ERROR_NOT_AVAILABLE;

  HRESULT result;

  rv = NS_ERROR_NOT_AVAILABLE;

  IUniformResourceLocatorW* urlLink = nullptr;
  result =
      ::CoCreateInstance(CLSID_InternetShortcut, nullptr, CLSCTX_INPROC_SERVER,
                         IID_IUniformResourceLocatorW, (void**)&urlLink);
  if (SUCCEEDED(result) && urlLink) {
    IPersistFile* urlFile = nullptr;
    result = urlLink->QueryInterface(IID_IPersistFile, (void**)&urlFile);
    if (SUCCEEDED(result) && urlFile) {
      result = urlFile->Load(path.get(), STGM_READ);
      if (SUCCEEDED(result)) {
        LPWSTR lpTemp = nullptr;

        // The URL this method will give us back seems to be already
        // escaped. Hence, do not do escaping of our own.
        result = urlLink->GetURL(&lpTemp);
        if (SUCCEEDED(result) && lpTemp) {
          rv = NS_NewURI(aURI, nsDependentString(lpTemp));
          // free the string that GetURL alloc'd
          CoTaskMemFree(lpTemp);
>>>>>>> upstream-releases
        }
      }
      urlFile->Release();
    }
    urlLink->Release();
  }
  return rv;
}

#elif defined(XP_UNIX)
NS_IMETHODIMP
<<<<<<< HEAD
nsFileProtocolHandler::ReadURLFile(nsIFile *aFile, nsIURI **aURI) {
  // We only support desktop files that end in ".desktop" like the spec says:
  // http://standards.freedesktop.org/desktop-entry-spec/latest/ar01s02.html
  nsAutoCString leafName;
  nsresult rv = aFile->GetNativeLeafName(leafName);
  if (NS_FAILED(rv) ||
      !StringEndsWith(leafName, NS_LITERAL_CSTRING(".desktop")))
    return NS_ERROR_NOT_AVAILABLE;
||||||| merged common ancestors
nsFileProtocolHandler::ReadURLFile(nsIFile* aFile, nsIURI** aURI)
{
    // We only support desktop files that end in ".desktop" like the spec says:
    // http://standards.freedesktop.org/desktop-entry-spec/latest/ar01s02.html
    nsAutoCString leafName;
    nsresult rv = aFile->GetNativeLeafName(leafName);
    if (NS_FAILED(rv) ||
	!StringEndsWith(leafName, NS_LITERAL_CSTRING(".desktop")))
        return NS_ERROR_NOT_AVAILABLE;

    bool isFile = false;
    rv = aFile->IsFile(&isFile);
    if (NS_FAILED(rv) || !isFile) {
        return NS_ERROR_NOT_AVAILABLE;
    }
=======
nsFileProtocolHandler::ReadURLFile(nsIFile* aFile, nsIURI** aURI) {
  // We only support desktop files that end in ".desktop" like the spec says:
  // http://standards.freedesktop.org/desktop-entry-spec/latest/ar01s02.html
  nsAutoCString leafName;
  nsresult rv = aFile->GetNativeLeafName(leafName);
  if (NS_FAILED(rv) ||
      !StringEndsWith(leafName, NS_LITERAL_CSTRING(".desktop")))
    return NS_ERROR_NOT_AVAILABLE;
>>>>>>> upstream-releases

  bool isFile = false;
  rv = aFile->IsFile(&isFile);
  if (NS_FAILED(rv) || !isFile) {
    return NS_ERROR_NOT_AVAILABLE;
  }

  nsINIParser parser;
  rv = parser.Init(aFile);
  if (NS_FAILED(rv)) return rv;

  nsAutoCString type;
  parser.GetString(DESKTOP_ENTRY_SECTION, "Type", type);
  if (!type.EqualsLiteral("Link")) return NS_ERROR_NOT_AVAILABLE;

<<<<<<< HEAD
  nsAutoCString url;
  rv = parser.GetString(DESKTOP_ENTRY_SECTION, "URL", url);
  if (NS_FAILED(rv) || url.IsEmpty()) return NS_ERROR_NOT_AVAILABLE;

  return NS_NewURI(aURI, url);
}
||||||| merged common ancestors
    return NS_NewURI(aURI, url);
}
=======
  nsAutoCString url;
  rv = parser.GetString(DESKTOP_ENTRY_SECTION, "URL", url);
  if (NS_FAILED(rv) || url.IsEmpty()) return NS_ERROR_NOT_AVAILABLE;
>>>>>>> upstream-releases

<<<<<<< HEAD
#else   // other platforms
NS_IMETHODIMP
nsFileProtocolHandler::ReadURLFile(nsIFile *aFile, nsIURI **aURI) {
  return NS_ERROR_NOT_AVAILABLE;
||||||| merged common ancestors
#else // other platforms
NS_IMETHODIMP
nsFileProtocolHandler::ReadURLFile(nsIFile* aFile, nsIURI** aURI)
{
    return NS_ERROR_NOT_AVAILABLE;
=======
  return NS_NewURI(aURI, url);
>>>>>>> upstream-releases
}
<<<<<<< HEAD
#endif  // ReadURLFile()
||||||| merged common ancestors
#endif // ReadURLFile()
=======
>>>>>>> upstream-releases

#else   // other platforms
NS_IMETHODIMP
<<<<<<< HEAD
nsFileProtocolHandler::GetScheme(nsACString &result) {
  result.AssignLiteral("file");
  return NS_OK;
||||||| merged common ancestors
nsFileProtocolHandler::GetScheme(nsACString &result)
{
    result.AssignLiteral("file");
    return NS_OK;
=======
nsFileProtocolHandler::ReadURLFile(nsIFile* aFile, nsIURI** aURI) {
  return NS_ERROR_NOT_AVAILABLE;
>>>>>>> upstream-releases
}
#endif  // ReadURLFile()

NS_IMETHODIMP
<<<<<<< HEAD
nsFileProtocolHandler::GetDefaultPort(int32_t *result) {
  *result = -1;  // no port for file: URLs
  return NS_OK;
||||||| merged common ancestors
nsFileProtocolHandler::GetDefaultPort(int32_t *result)
{
    *result = -1;        // no port for file: URLs
    return NS_OK;
=======
nsFileProtocolHandler::GetScheme(nsACString& result) {
  result.AssignLiteral("file");
  return NS_OK;
>>>>>>> upstream-releases
}

NS_IMETHODIMP
<<<<<<< HEAD
nsFileProtocolHandler::GetProtocolFlags(uint32_t *result) {
  *result = URI_NOAUTH | URI_IS_LOCAL_FILE | URI_IS_LOCAL_RESOURCE |
            URI_IS_POTENTIALLY_TRUSTWORTHY;
  return NS_OK;
||||||| merged common ancestors
nsFileProtocolHandler::GetProtocolFlags(uint32_t *result)
{
    *result = URI_NOAUTH | URI_IS_LOCAL_FILE |
              URI_IS_LOCAL_RESOURCE | URI_IS_POTENTIALLY_TRUSTWORTHY;
    return NS_OK;
=======
nsFileProtocolHandler::GetDefaultPort(int32_t* result) {
  *result = -1;  // no port for file: URLs
  return NS_OK;
>>>>>>> upstream-releases
}

NS_IMETHODIMP
<<<<<<< HEAD
nsFileProtocolHandler::NewURI(const nsACString &spec, const char *charset,
                              nsIURI *aBaseURI, nsIURI **result) {
  nsAutoCString buf(spec);
#if defined(XP_WIN)
  buf.Truncate();
  if (!net_NormalizeFileURL(spec, buf)) {
    buf = spec;
  }
#endif

  nsCOMPtr<nsIURI> base(aBaseURI);
  return NS_MutateURI(new nsStandardURL::Mutator())
      .Apply(NS_MutatorMethod(&nsIFileURLMutator::MarkFileURL))
      .Apply(NS_MutatorMethod(&nsIStandardURLMutator::Init,
                              nsIStandardURL::URLTYPE_NO_AUTHORITY, -1, buf,
                              charset, base, nullptr))
      .Finalize(result);
||||||| merged common ancestors
nsFileProtocolHandler::NewURI(const nsACString &spec,
                              const char *charset,
                              nsIURI *aBaseURI,
                              nsIURI **result)
{
    nsAutoCString buf(spec);
#if defined(XP_WIN)
    buf.Truncate();
    if (!net_NormalizeFileURL(spec, buf)) {
        buf = spec;
    }
#endif

    nsCOMPtr<nsIURI> base(aBaseURI);
    return NS_MutateURI(new nsStandardURL::Mutator())
      .Apply(NS_MutatorMethod(&nsIFileURLMutator::MarkFileURL))
      .Apply(NS_MutatorMethod(&nsIStandardURLMutator::Init,
                              nsIStandardURL::URLTYPE_NO_AUTHORITY,
                              -1, buf, charset, base, nullptr))
      .Finalize(result);
=======
nsFileProtocolHandler::GetProtocolFlags(uint32_t* result) {
  *result = URI_NOAUTH | URI_IS_LOCAL_FILE | URI_IS_LOCAL_RESOURCE |
            URI_IS_POTENTIALLY_TRUSTWORTHY;
  return NS_OK;
>>>>>>> upstream-releases
}

NS_IMETHODIMP
<<<<<<< HEAD
nsFileProtocolHandler::NewChannel2(nsIURI *uri, nsILoadInfo *aLoadInfo,
                                   nsIChannel **result) {
  nsresult rv;

  nsFileChannel *chan;
  if (IsNeckoChild()) {
    chan = new mozilla::net::FileChannelChild(uri);
  } else {
    chan = new nsFileChannel(uri);
  }
  if (!chan) return NS_ERROR_OUT_OF_MEMORY;
  NS_ADDREF(chan);

  // set the loadInfo on the new channel ; must do this
  // before calling Init() on it, since it needs the load
  // info be already set.
  rv = chan->SetLoadInfo(aLoadInfo);
  if (NS_FAILED(rv)) {
    NS_RELEASE(chan);
    return rv;
  }
||||||| merged common ancestors
nsFileProtocolHandler::NewChannel2(nsIURI* uri,
                                   nsILoadInfo* aLoadInfo,
                                   nsIChannel** result)
{
    nsresult rv;

    nsFileChannel *chan;
    if (IsNeckoChild()) {
        chan = new mozilla::net::FileChannelChild(uri);
    } else {
        chan = new nsFileChannel(uri);
    }
    if (!chan)
        return NS_ERROR_OUT_OF_MEMORY;
    NS_ADDREF(chan);

    // set the loadInfo on the new channel ; must do this
    // before calling Init() on it, since it needs the load
    // info be already set.
    rv = chan->SetLoadInfo(aLoadInfo);
    if (NS_FAILED(rv)) {
        NS_RELEASE(chan);
        return rv;
    }
=======
nsFileProtocolHandler::NewChannel(nsIURI* uri, nsILoadInfo* aLoadInfo,
                                  nsIChannel** result) {
  nsresult rv;
>>>>>>> upstream-releases

<<<<<<< HEAD
  rv = chan->Init();
  if (NS_FAILED(rv)) {
    NS_RELEASE(chan);
    return rv;
  }
||||||| merged common ancestors
    rv = chan->Init();
    if (NS_FAILED(rv)) {
        NS_RELEASE(chan);
        return rv;
    }
=======
  RefPtr<nsFileChannel> chan;
  if (IsNeckoChild()) {
    chan = new mozilla::net::FileChannelChild(uri);
  } else {
    chan = new nsFileChannel(uri);
  }
>>>>>>> upstream-releases

<<<<<<< HEAD
  *result = chan;
  return NS_OK;
}
||||||| merged common ancestors
    *result = chan;
    return NS_OK;
}
=======
  // set the loadInfo on the new channel ; must do this
  // before calling Init() on it, since it needs the load
  // info be already set.
  rv = chan->SetLoadInfo(aLoadInfo);
  if (NS_FAILED(rv)) {
    return rv;
  }
>>>>>>> upstream-releases

<<<<<<< HEAD
NS_IMETHODIMP
nsFileProtocolHandler::NewChannel(nsIURI *uri, nsIChannel **result) {
  return NewChannel2(uri, nullptr, result);
||||||| merged common ancestors
NS_IMETHODIMP
nsFileProtocolHandler::NewChannel(nsIURI *uri, nsIChannel **result)
{
    return NewChannel2(uri, nullptr, result);
=======
  rv = chan->Init();
  if (NS_FAILED(rv)) {
    return rv;
  }

  chan.forget(result);
  return NS_OK;
>>>>>>> upstream-releases
}

NS_IMETHODIMP
<<<<<<< HEAD
nsFileProtocolHandler::AllowPort(int32_t port, const char *scheme,
                                 bool *result) {
  // don't override anything.
  *result = false;
  return NS_OK;
||||||| merged common ancestors
nsFileProtocolHandler::AllowPort(int32_t port, const char *scheme, bool *result)
{
    // don't override anything.
    *result = false;
    return NS_OK;
=======
nsFileProtocolHandler::AllowPort(int32_t port, const char* scheme,
                                 bool* result) {
  // don't override anything.
  *result = false;
  return NS_OK;
>>>>>>> upstream-releases
}

//-----------------------------------------------------------------------------
// nsIFileProtocolHandler methods:

NS_IMETHODIMP
<<<<<<< HEAD
nsFileProtocolHandler::NewFileURI(nsIFile *aFile, nsIURI **aResult) {
  NS_ENSURE_ARG_POINTER(aFile);

  RefPtr<nsIFile> file(aFile);
  // NOTE: the origin charset is assigned the value of the platform
  // charset by the SetFile method.
  return NS_MutateURI(new nsStandardURL::Mutator())
      .Apply(NS_MutatorMethod(&nsIFileURLMutator::SetFile, file))
      .Finalize(aResult);
||||||| merged common ancestors
nsFileProtocolHandler::NewFileURI(nsIFile *aFile, nsIURI **aResult)
{
    NS_ENSURE_ARG_POINTER(aFile);

    RefPtr<nsIFile> file(aFile);
    // NOTE: the origin charset is assigned the value of the platform
    // charset by the SetFile method.
    return NS_MutateURI(new nsStandardURL::Mutator())
             .Apply(NS_MutatorMethod(&nsIFileURLMutator::SetFile, file))
             .Finalize(aResult);
=======
nsFileProtocolHandler::NewFileURI(nsIFile* aFile, nsIURI** aResult) {
  NS_ENSURE_ARG_POINTER(aFile);

  RefPtr<nsIFile> file(aFile);
  // NOTE: the origin charset is assigned the value of the platform
  // charset by the SetFile method.
  return NS_MutateURI(new nsStandardURL::Mutator())
      .Apply(NS_MutatorMethod(&nsIFileURLMutator::SetFile, file))
      .Finalize(aResult);
>>>>>>> upstream-releases
}

NS_IMETHODIMP
<<<<<<< HEAD
nsFileProtocolHandler::NewFileURIMutator(nsIFile *aFile,
                                         nsIURIMutator **aResult) {
  NS_ENSURE_ARG_POINTER(aFile);
  nsresult rv;

  nsCOMPtr<nsIURIMutator> mutator = new nsStandardURL::Mutator();
  nsCOMPtr<nsIFileURLMutator> fileMutator = do_QueryInterface(mutator, &rv);
  if (NS_FAILED(rv)) {
    return rv;
  }
||||||| merged common ancestors
nsFileProtocolHandler::NewFileURIMutator(nsIFile *aFile, nsIURIMutator **aResult)
{
    NS_ENSURE_ARG_POINTER(aFile);
    nsresult rv;

    nsCOMPtr<nsIURIMutator> mutator = new nsStandardURL::Mutator();
    nsCOMPtr<nsIFileURLMutator> fileMutator = do_QueryInterface(mutator, &rv);
    if (NS_FAILED(rv)) {
        return rv;
    }
=======
nsFileProtocolHandler::NewFileURIMutator(nsIFile* aFile,
                                         nsIURIMutator** aResult) {
  NS_ENSURE_ARG_POINTER(aFile);
  nsresult rv;
>>>>>>> upstream-releases

<<<<<<< HEAD
  // NOTE: the origin charset is assigned the value of the platform
  // charset by the SetFile method.
  rv = fileMutator->SetFile(aFile);
  if (NS_FAILED(rv)) {
    return rv;
  }
||||||| merged common ancestors
    // NOTE: the origin charset is assigned the value of the platform
    // charset by the SetFile method.
    rv = fileMutator->SetFile(aFile);
    if (NS_FAILED(rv)) {
        return rv;
    }
=======
  nsCOMPtr<nsIURIMutator> mutator = new nsStandardURL::Mutator();
  nsCOMPtr<nsIFileURLMutator> fileMutator = do_QueryInterface(mutator, &rv);
  if (NS_FAILED(rv)) {
    return rv;
  }

  // NOTE: the origin charset is assigned the value of the platform
  // charset by the SetFile method.
  rv = fileMutator->SetFile(aFile);
  if (NS_FAILED(rv)) {
    return rv;
  }
>>>>>>> upstream-releases

  mutator.forget(aResult);
  return NS_OK;
}

NS_IMETHODIMP
<<<<<<< HEAD
nsFileProtocolHandler::GetURLSpecFromFile(nsIFile *file, nsACString &result) {
  NS_ENSURE_ARG_POINTER(file);
  return net_GetURLSpecFromFile(file, result);
||||||| merged common ancestors
nsFileProtocolHandler::GetURLSpecFromFile(nsIFile *file, nsACString &result)
{
    NS_ENSURE_ARG_POINTER(file);
    return net_GetURLSpecFromFile(file, result);
=======
nsFileProtocolHandler::GetURLSpecFromFile(nsIFile* file, nsACString& result) {
  NS_ENSURE_ARG_POINTER(file);
  return net_GetURLSpecFromFile(file, result);
>>>>>>> upstream-releases
}

NS_IMETHODIMP
<<<<<<< HEAD
nsFileProtocolHandler::GetURLSpecFromActualFile(nsIFile *file,
                                                nsACString &result) {
  NS_ENSURE_ARG_POINTER(file);
  return net_GetURLSpecFromActualFile(file, result);
||||||| merged common ancestors
nsFileProtocolHandler::GetURLSpecFromActualFile(nsIFile *file,
                                                nsACString &result)
{
    NS_ENSURE_ARG_POINTER(file);
    return net_GetURLSpecFromActualFile(file, result);
=======
nsFileProtocolHandler::GetURLSpecFromActualFile(nsIFile* file,
                                                nsACString& result) {
  NS_ENSURE_ARG_POINTER(file);
  return net_GetURLSpecFromActualFile(file, result);
>>>>>>> upstream-releases
}

NS_IMETHODIMP
<<<<<<< HEAD
nsFileProtocolHandler::GetURLSpecFromDir(nsIFile *file, nsACString &result) {
  NS_ENSURE_ARG_POINTER(file);
  return net_GetURLSpecFromDir(file, result);
||||||| merged common ancestors
nsFileProtocolHandler::GetURLSpecFromDir(nsIFile *file, nsACString &result)
{
    NS_ENSURE_ARG_POINTER(file);
    return net_GetURLSpecFromDir(file, result);
=======
nsFileProtocolHandler::GetURLSpecFromDir(nsIFile* file, nsACString& result) {
  NS_ENSURE_ARG_POINTER(file);
  return net_GetURLSpecFromDir(file, result);
>>>>>>> upstream-releases
}

NS_IMETHODIMP
<<<<<<< HEAD
nsFileProtocolHandler::GetFileFromURLSpec(const nsACString &spec,
                                          nsIFile **result) {
  return net_GetFileFromURLSpec(spec, result);
||||||| merged common ancestors
nsFileProtocolHandler::GetFileFromURLSpec(const nsACString &spec, nsIFile **result)
{
    return net_GetFileFromURLSpec(spec, result);
=======
nsFileProtocolHandler::GetFileFromURLSpec(const nsACString& spec,
                                          nsIFile** result) {
  return net_GetFileFromURLSpec(spec, result);
>>>>>>> upstream-releases
}
