/* -*- Mode: C++; tab-width: 4; indent-tabs-mode: nil; c-basic-offset: 2 -*-
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/. */

#include "base/basictypes.h"

#include "nsJARURI.h"
#include "nsNetUtil.h"
#include "nsIIOService.h"
#include "nsIStandardURL.h"
#include "nsCRT.h"
#include "nsIComponentManager.h"
#include "nsIServiceManager.h"
#include "nsIZipReader.h"
#include "nsReadableUtils.h"
#include "nsAutoPtr.h"
#include "nsNetCID.h"
#include "nsIObjectInputStream.h"
#include "nsIObjectOutputStream.h"
#include "nsQueryObject.h"
#include "mozilla/ipc/URIUtils.h"

using namespace mozilla::ipc;

static NS_DEFINE_CID(kJARURICID, NS_JARURI_CID);

////////////////////////////////////////////////////////////////////////////////

nsJARURI::nsJARURI() {}

nsJARURI::~nsJARURI() {}

// XXX Why is this threadsafe?
NS_IMPL_ADDREF(nsJARURI)
NS_IMPL_RELEASE(nsJARURI)
NS_INTERFACE_MAP_BEGIN(nsJARURI)
  NS_INTERFACE_MAP_ENTRY_AMBIGUOUS(nsISupports, nsIJARURI)
  NS_INTERFACE_MAP_ENTRY(nsIURI)
  NS_INTERFACE_MAP_ENTRY(nsIURL)
  NS_INTERFACE_MAP_ENTRY(nsIJARURI)
  NS_INTERFACE_MAP_ENTRY(nsISerializable)
  NS_INTERFACE_MAP_ENTRY(nsIClassInfo)
  NS_INTERFACE_MAP_ENTRY(nsINestedURI)
  NS_INTERFACE_MAP_ENTRY_CONCRETE(nsJARURI)
NS_INTERFACE_MAP_END

<<<<<<< HEAD
nsresult nsJARURI::Init(const char *charsetHint) {
  mCharsetHint = charsetHint;
  return NS_OK;
||||||| merged common ancestors
nsresult
nsJARURI::Init(const char *charsetHint)
{
    mCharsetHint = charsetHint;
    return NS_OK;
=======
nsresult nsJARURI::Init(const char* charsetHint) {
  mCharsetHint = charsetHint;
  return NS_OK;
>>>>>>> upstream-releases
}

#define NS_JAR_SCHEME NS_LITERAL_CSTRING("jar:")
#define NS_JAR_DELIMITER NS_LITERAL_CSTRING("!/")
#define NS_BOGUS_ENTRY_SCHEME NS_LITERAL_CSTRING("x:///")

// FormatSpec takes the entry spec (including the "x:///" at the
// beginning) and gives us a full JAR spec.
<<<<<<< HEAD
nsresult nsJARURI::FormatSpec(const nsACString &entrySpec, nsACString &result,
                              bool aIncludeScheme) {
  // The entrySpec MUST start with "x:///"
  NS_ASSERTION(StringBeginsWith(entrySpec, NS_BOGUS_ENTRY_SCHEME),
               "bogus entry spec");

  nsAutoCString fileSpec;
  nsresult rv = mJARFile->GetSpec(fileSpec);
  if (NS_FAILED(rv)) return rv;

  if (aIncludeScheme)
    result = NS_JAR_SCHEME;
  else
    result.Truncate();

  result.Append(fileSpec + NS_JAR_DELIMITER +
                Substring(entrySpec, 5, entrySpec.Length() - 5));
  return NS_OK;
}

nsresult nsJARURI::CreateEntryURL(const nsACString &entryFilename,
                                  const char *charset, nsIURL **url) {
  *url = nullptr;
  // Flatten the concatenation, just in case.  See bug 128288
  nsAutoCString spec(NS_BOGUS_ENTRY_SCHEME + entryFilename);
  return NS_MutateURI(NS_STANDARDURLMUTATOR_CONTRACTID)
      .Apply(NS_MutatorMethod(&nsIStandardURLMutator::Init,
                              nsIStandardURL::URLTYPE_NO_AUTHORITY, -1, spec,
                              charset, nullptr, nullptr))
      .Finalize(url);
||||||| merged common ancestors
nsresult
nsJARURI::FormatSpec(const nsACString &entrySpec, nsACString &result,
                     bool aIncludeScheme)
{
    // The entrySpec MUST start with "x:///"
    NS_ASSERTION(StringBeginsWith(entrySpec, NS_BOGUS_ENTRY_SCHEME),
                 "bogus entry spec");

    nsAutoCString fileSpec;
    nsresult rv = mJARFile->GetSpec(fileSpec);
    if (NS_FAILED(rv)) return rv;

    if (aIncludeScheme)
        result = NS_JAR_SCHEME;
    else
        result.Truncate();

    result.Append(fileSpec + NS_JAR_DELIMITER +
                  Substring(entrySpec, 5, entrySpec.Length() - 5));
    return NS_OK;
}

nsresult
nsJARURI::CreateEntryURL(const nsACString& entryFilename,
                         const char* charset,
                         nsIURL** url)
{
    *url = nullptr;
    // Flatten the concatenation, just in case.  See bug 128288
    nsAutoCString spec(NS_BOGUS_ENTRY_SCHEME + entryFilename);
    return NS_MutateURI(NS_STANDARDURLMUTATOR_CONTRACTID)
        .Apply(NS_MutatorMethod(&nsIStandardURLMutator::Init,
                                nsIStandardURL::URLTYPE_NO_AUTHORITY, -1,
                                spec, charset, nullptr, nullptr))
        .Finalize(url);
=======
nsresult nsJARURI::FormatSpec(const nsACString& entrySpec, nsACString& result,
                              bool aIncludeScheme) {
  // The entrySpec MUST start with "x:///"
  NS_ASSERTION(StringBeginsWith(entrySpec, NS_BOGUS_ENTRY_SCHEME),
               "bogus entry spec");

  nsAutoCString fileSpec;
  nsresult rv = mJARFile->GetSpec(fileSpec);
  if (NS_FAILED(rv)) return rv;

  if (aIncludeScheme)
    result = NS_JAR_SCHEME;
  else
    result.Truncate();

  result.Append(fileSpec + NS_JAR_DELIMITER +
                Substring(entrySpec, 5, entrySpec.Length() - 5));
  return NS_OK;
}

nsresult nsJARURI::CreateEntryURL(const nsACString& entryFilename,
                                  const char* charset, nsIURL** url) {
  *url = nullptr;
  // Flatten the concatenation, just in case.  See bug 128288
  nsAutoCString spec(NS_BOGUS_ENTRY_SCHEME + entryFilename);
  return NS_MutateURI(NS_STANDARDURLMUTATOR_CONTRACTID)
      .Apply(NS_MutatorMethod(&nsIStandardURLMutator::Init,
                              nsIStandardURL::URLTYPE_NO_AUTHORITY, -1, spec,
                              charset, nullptr, nullptr))
      .Finalize(url);
>>>>>>> upstream-releases
}

////////////////////////////////////////////////////////////////////////////////
// nsISerializable methods:

NS_IMETHODIMP
<<<<<<< HEAD
nsJARURI::Read(nsIObjectInputStream *aStream) {
  MOZ_ASSERT_UNREACHABLE("Use nsIURIMutator.read() instead");
  return NS_ERROR_NOT_IMPLEMENTED;
||||||| merged common ancestors
nsJARURI::Read(nsIObjectInputStream *aStream)
{
    MOZ_ASSERT_UNREACHABLE("Use nsIURIMutator.read() instead");
    return NS_ERROR_NOT_IMPLEMENTED;
=======
nsJARURI::Read(nsIObjectInputStream* aStream) {
  MOZ_ASSERT_UNREACHABLE("Use nsIURIMutator.read() instead");
  return NS_ERROR_NOT_IMPLEMENTED;
>>>>>>> upstream-releases
}

<<<<<<< HEAD
nsresult nsJARURI::ReadPrivate(nsIObjectInputStream *aInputStream) {
  nsresult rv;
||||||| merged common ancestors
nsresult
nsJARURI::ReadPrivate(nsIObjectInputStream *aInputStream)
{
    nsresult rv;
=======
nsresult nsJARURI::ReadPrivate(nsIObjectInputStream* aInputStream) {
  nsresult rv;
>>>>>>> upstream-releases

  nsCOMPtr<nsISupports> supports;
  rv = aInputStream->ReadObject(true, getter_AddRefs(supports));
  NS_ENSURE_SUCCESS(rv, rv);

  mJARFile = do_QueryInterface(supports, &rv);
  NS_ENSURE_SUCCESS(rv, rv);

  rv = aInputStream->ReadObject(true, getter_AddRefs(supports));
  NS_ENSURE_SUCCESS(rv, rv);

  mJAREntry = do_QueryInterface(supports);
  NS_ENSURE_SUCCESS(rv, rv);

  rv = aInputStream->ReadCString(mCharsetHint);
  return rv;
}

NS_IMETHODIMP
<<<<<<< HEAD
nsJARURI::Write(nsIObjectOutputStream *aOutputStream) {
  nsresult rv;
||||||| merged common ancestors
nsJARURI::Write(nsIObjectOutputStream* aOutputStream)
{
    nsresult rv;
=======
nsJARURI::Write(nsIObjectOutputStream* aOutputStream) {
  nsresult rv;
>>>>>>> upstream-releases

  rv = aOutputStream->WriteCompoundObject(mJARFile, NS_GET_IID(nsIURI), true);
  NS_ENSURE_SUCCESS(rv, rv);

  rv = aOutputStream->WriteCompoundObject(mJAREntry, NS_GET_IID(nsIURL), true);
  NS_ENSURE_SUCCESS(rv, rv);

  rv = aOutputStream->WriteStringZ(mCharsetHint.get());
  return rv;
}

////////////////////////////////////////////////////////////////////////////////
// nsIClassInfo methods:

NS_IMETHODIMP
<<<<<<< HEAD
nsJARURI::GetInterfaces(uint32_t *count, nsIID ***array) {
  *count = 0;
  *array = nullptr;
  return NS_OK;
||||||| merged common ancestors
nsJARURI::GetInterfaces(uint32_t *count, nsIID * **array)
{
    *count = 0;
    *array = nullptr;
    return NS_OK;
=======
nsJARURI::GetInterfaces(nsTArray<nsIID>& array) {
  array.Clear();
  return NS_OK;
>>>>>>> upstream-releases
}

NS_IMETHODIMP
<<<<<<< HEAD
nsJARURI::GetScriptableHelper(nsIXPCScriptable **_retval) {
  *_retval = nullptr;
  return NS_OK;
||||||| merged common ancestors
nsJARURI::GetScriptableHelper(nsIXPCScriptable **_retval)
{
    *_retval = nullptr;
    return NS_OK;
=======
nsJARURI::GetScriptableHelper(nsIXPCScriptable** _retval) {
  *_retval = nullptr;
  return NS_OK;
>>>>>>> upstream-releases
}

NS_IMETHODIMP
<<<<<<< HEAD
nsJARURI::GetContractID(nsACString &aContractID) {
  aContractID.SetIsVoid(true);
  return NS_OK;
||||||| merged common ancestors
nsJARURI::GetContractID(nsACString& aContractID)
{
    aContractID.SetIsVoid(true);
    return NS_OK;
=======
nsJARURI::GetContractID(nsACString& aContractID) {
  aContractID.SetIsVoid(true);
  return NS_OK;
>>>>>>> upstream-releases
}

NS_IMETHODIMP
<<<<<<< HEAD
nsJARURI::GetClassDescription(nsACString &aClassDescription) {
  aClassDescription.SetIsVoid(true);
  return NS_OK;
||||||| merged common ancestors
nsJARURI::GetClassDescription(nsACString& aClassDescription)
{
    aClassDescription.SetIsVoid(true);
    return NS_OK;
=======
nsJARURI::GetClassDescription(nsACString& aClassDescription) {
  aClassDescription.SetIsVoid(true);
  return NS_OK;
>>>>>>> upstream-releases
}

NS_IMETHODIMP
<<<<<<< HEAD
nsJARURI::GetClassID(nsCID **aClassID) {
  *aClassID = (nsCID *)moz_xmalloc(sizeof(nsCID));
  return GetClassIDNoAlloc(*aClassID);
||||||| merged common ancestors
nsJARURI::GetClassID(nsCID * *aClassID)
{
    *aClassID = (nsCID*) moz_xmalloc(sizeof(nsCID));
    return GetClassIDNoAlloc(*aClassID);
=======
nsJARURI::GetClassID(nsCID** aClassID) {
  *aClassID = (nsCID*)moz_xmalloc(sizeof(nsCID));
  return GetClassIDNoAlloc(*aClassID);
>>>>>>> upstream-releases
}

NS_IMETHODIMP
<<<<<<< HEAD
nsJARURI::GetFlags(uint32_t *aFlags) {
  // XXX We implement THREADSAFE addref/release, but probably shouldn't.
  *aFlags = nsIClassInfo::MAIN_THREAD_ONLY;
  return NS_OK;
||||||| merged common ancestors
nsJARURI::GetFlags(uint32_t *aFlags)
{
    // XXX We implement THREADSAFE addref/release, but probably shouldn't.
    *aFlags = nsIClassInfo::MAIN_THREAD_ONLY;
    return NS_OK;
=======
nsJARURI::GetFlags(uint32_t* aFlags) {
  // XXX We implement THREADSAFE addref/release, but probably shouldn't.
  *aFlags = nsIClassInfo::MAIN_THREAD_ONLY;
  return NS_OK;
>>>>>>> upstream-releases
}

NS_IMETHODIMP
<<<<<<< HEAD
nsJARURI::GetClassIDNoAlloc(nsCID *aClassIDNoAlloc) {
  *aClassIDNoAlloc = kJARURICID;
  return NS_OK;
||||||| merged common ancestors
nsJARURI::GetClassIDNoAlloc(nsCID *aClassIDNoAlloc)
{
    *aClassIDNoAlloc = kJARURICID;
    return NS_OK;
=======
nsJARURI::GetClassIDNoAlloc(nsCID* aClassIDNoAlloc) {
  *aClassIDNoAlloc = kJARURICID;
  return NS_OK;
>>>>>>> upstream-releases
}

////////////////////////////////////////////////////////////////////////////////
// nsIURI methods:

NS_IMETHODIMP
<<<<<<< HEAD
nsJARURI::GetSpec(nsACString &aSpec) {
  nsAutoCString entrySpec;
  mJAREntry->GetSpec(entrySpec);
  return FormatSpec(entrySpec, aSpec);
||||||| merged common ancestors
nsJARURI::GetSpec(nsACString &aSpec)
{
    nsAutoCString entrySpec;
    mJAREntry->GetSpec(entrySpec);
    return FormatSpec(entrySpec, aSpec);
=======
nsJARURI::GetSpec(nsACString& aSpec) {
  nsAutoCString entrySpec;
  mJAREntry->GetSpec(entrySpec);
  return FormatSpec(entrySpec, aSpec);
>>>>>>> upstream-releases
}

NS_IMETHODIMP
<<<<<<< HEAD
nsJARURI::GetSpecIgnoringRef(nsACString &aSpec) {
  nsAutoCString entrySpec;
  mJAREntry->GetSpecIgnoringRef(entrySpec);
  return FormatSpec(entrySpec, aSpec);
||||||| merged common ancestors
nsJARURI::GetSpecIgnoringRef(nsACString &aSpec)
{
    nsAutoCString entrySpec;
    mJAREntry->GetSpecIgnoringRef(entrySpec);
    return FormatSpec(entrySpec, aSpec);
=======
nsJARURI::GetSpecIgnoringRef(nsACString& aSpec) {
  nsAutoCString entrySpec;
  mJAREntry->GetSpecIgnoringRef(entrySpec);
  return FormatSpec(entrySpec, aSpec);
>>>>>>> upstream-releases
}

NS_IMETHODIMP
<<<<<<< HEAD
nsJARURI::GetDisplaySpec(nsACString &aUnicodeSpec) {
  return GetSpec(aUnicodeSpec);
||||||| merged common ancestors
nsJARURI::GetDisplaySpec(nsACString &aUnicodeSpec)
{
    return GetSpec(aUnicodeSpec);
=======
nsJARURI::GetDisplaySpec(nsACString& aUnicodeSpec) {
  return GetSpec(aUnicodeSpec);
>>>>>>> upstream-releases
}

NS_IMETHODIMP
<<<<<<< HEAD
nsJARURI::GetDisplayHostPort(nsACString &aUnicodeHostPort) {
  return GetHostPort(aUnicodeHostPort);
||||||| merged common ancestors
nsJARURI::GetDisplayHostPort(nsACString &aUnicodeHostPort)
{
    return GetHostPort(aUnicodeHostPort);
=======
nsJARURI::GetDisplayHostPort(nsACString& aUnicodeHostPort) {
  return GetHostPort(aUnicodeHostPort);
>>>>>>> upstream-releases
}

NS_IMETHODIMP
<<<<<<< HEAD
nsJARURI::GetDisplayPrePath(nsACString &aPrePath) {
  return GetPrePath(aPrePath);
||||||| merged common ancestors
nsJARURI::GetDisplayPrePath(nsACString &aPrePath)
{
    return GetPrePath(aPrePath);
=======
nsJARURI::GetDisplayPrePath(nsACString& aPrePath) {
  return GetPrePath(aPrePath);
>>>>>>> upstream-releases
}

NS_IMETHODIMP
<<<<<<< HEAD
nsJARURI::GetDisplayHost(nsACString &aUnicodeHost) {
  return GetHost(aUnicodeHost);
||||||| merged common ancestors
nsJARURI::GetDisplayHost(nsACString &aUnicodeHost)
{
    return GetHost(aUnicodeHost);
=======
nsJARURI::GetDisplayHost(nsACString& aUnicodeHost) {
  return GetHost(aUnicodeHost);
>>>>>>> upstream-releases
}

NS_IMETHODIMP
<<<<<<< HEAD
nsJARURI::GetHasRef(bool *result) { return mJAREntry->GetHasRef(result); }
||||||| merged common ancestors
nsJARURI::GetHasRef(bool *result)
{
    return mJAREntry->GetHasRef(result);
}
=======
nsJARURI::GetHasRef(bool* result) { return mJAREntry->GetHasRef(result); }
>>>>>>> upstream-releases

<<<<<<< HEAD
nsresult nsJARURI::SetSpecInternal(const nsACString &aSpec) {
  return SetSpecWithBase(aSpec, nullptr);
||||||| merged common ancestors
nsresult
nsJARURI::SetSpecInternal(const nsACString& aSpec)
{
    return SetSpecWithBase(aSpec, nullptr);
=======
nsresult nsJARURI::SetSpecInternal(const nsACString& aSpec) {
  return SetSpecWithBase(aSpec, nullptr);
>>>>>>> upstream-releases
}

// Queries this list of interfaces. If none match, it queries mURI.
NS_IMPL_NSIURIMUTATOR_ISUPPORTS(nsJARURI::Mutator, nsIURISetters, nsIURIMutator,
                                nsIURLMutator, nsISerializable,
                                nsIJARURIMutator)

NS_IMETHODIMP
<<<<<<< HEAD
nsJARURI::Mutator::SetFileName(const nsACString &aFileName,
                               nsIURIMutator **aMutator) {
  if (!mURI) {
    return NS_ERROR_NULL_POINTER;
  }
  if (aMutator) {
    nsCOMPtr<nsIURIMutator> mutator = this;
    mutator.forget(aMutator);
  }
  return mURI->SetFileNameInternal(aFileName);
||||||| merged common ancestors
nsJARURI::Mutator::SetFileName(const nsACString& aFileName, nsIURIMutator** aMutator)
{
    if (!mURI) {
        return NS_ERROR_NULL_POINTER;
    }
    if (aMutator) {
        nsCOMPtr<nsIURIMutator> mutator = this;
        mutator.forget(aMutator);
    }
    return mURI->SetFileNameInternal(aFileName);
=======
nsJARURI::Mutator::SetFileName(const nsACString& aFileName,
                               nsIURIMutator** aMutator) {
  if (!mURI) {
    return NS_ERROR_NULL_POINTER;
  }
  if (aMutator) {
    nsCOMPtr<nsIURIMutator> mutator = this;
    mutator.forget(aMutator);
  }
  return mURI->SetFileNameInternal(aFileName);
>>>>>>> upstream-releases
}

NS_IMETHODIMP
<<<<<<< HEAD
nsJARURI::Mutator::SetFileBaseName(const nsACString &aFileBaseName,
                                   nsIURIMutator **aMutator) {
  if (!mURI) {
    return NS_ERROR_NULL_POINTER;
  }
  if (aMutator) {
    nsCOMPtr<nsIURIMutator> mutator = this;
    mutator.forget(aMutator);
  }
  return mURI->SetFileBaseNameInternal(aFileBaseName);
||||||| merged common ancestors
nsJARURI::Mutator::SetFileBaseName(const nsACString& aFileBaseName, nsIURIMutator** aMutator)
{
    if (!mURI) {
        return NS_ERROR_NULL_POINTER;
    }
    if (aMutator) {
        nsCOMPtr<nsIURIMutator> mutator = this;
        mutator.forget(aMutator);
    }
    return mURI->SetFileBaseNameInternal(aFileBaseName);
=======
nsJARURI::Mutator::SetFileBaseName(const nsACString& aFileBaseName,
                                   nsIURIMutator** aMutator) {
  if (!mURI) {
    return NS_ERROR_NULL_POINTER;
  }
  if (aMutator) {
    nsCOMPtr<nsIURIMutator> mutator = this;
    mutator.forget(aMutator);
  }
  return mURI->SetFileBaseNameInternal(aFileBaseName);
>>>>>>> upstream-releases
}

NS_IMETHODIMP
<<<<<<< HEAD
nsJARURI::Mutator::SetFileExtension(const nsACString &aFileExtension,
                                    nsIURIMutator **aMutator) {
  if (!mURI) {
    return NS_ERROR_NULL_POINTER;
  }
  if (aMutator) {
    nsCOMPtr<nsIURIMutator> mutator = this;
    mutator.forget(aMutator);
  }
  return mURI->SetFileExtensionInternal(aFileExtension);
||||||| merged common ancestors
nsJARURI::Mutator::SetFileExtension(const nsACString& aFileExtension, nsIURIMutator** aMutator)
{
    if (!mURI) {
        return NS_ERROR_NULL_POINTER;
    }
    if (aMutator) {
        nsCOMPtr<nsIURIMutator> mutator = this;
        mutator.forget(aMutator);
    }
    return mURI->SetFileExtensionInternal(aFileExtension);
=======
nsJARURI::Mutator::SetFileExtension(const nsACString& aFileExtension,
                                    nsIURIMutator** aMutator) {
  if (!mURI) {
    return NS_ERROR_NULL_POINTER;
  }
  if (aMutator) {
    nsCOMPtr<nsIURIMutator> mutator = this;
    mutator.forget(aMutator);
  }
  return mURI->SetFileExtensionInternal(aFileExtension);
>>>>>>> upstream-releases
}

NS_IMETHODIMP
<<<<<<< HEAD
nsJARURI::Mutate(nsIURIMutator **aMutator) {
  RefPtr<nsJARURI::Mutator> mutator = new nsJARURI::Mutator();
  nsresult rv = mutator->InitFromURI(this);
  if (NS_FAILED(rv)) {
    return rv;
  }
  mutator.forget(aMutator);
  return NS_OK;
||||||| merged common ancestors
nsJARURI::Mutate(nsIURIMutator** aMutator)
{
    RefPtr<nsJARURI::Mutator> mutator = new nsJARURI::Mutator();
    nsresult rv = mutator->InitFromURI(this);
    if (NS_FAILED(rv)) {
        return rv;
    }
    mutator.forget(aMutator);
    return NS_OK;
=======
nsJARURI::Mutate(nsIURIMutator** aMutator) {
  RefPtr<nsJARURI::Mutator> mutator = new nsJARURI::Mutator();
  nsresult rv = mutator->InitFromURI(this);
  if (NS_FAILED(rv)) {
    return rv;
  }
  mutator.forget(aMutator);
  return NS_OK;
>>>>>>> upstream-releases
}

<<<<<<< HEAD
nsresult nsJARURI::SetSpecWithBase(const nsACString &aSpec, nsIURI *aBaseURL) {
  nsresult rv;
||||||| merged common ancestors
nsresult
nsJARURI::SetSpecWithBase(const nsACString &aSpec, nsIURI* aBaseURL)
{
    nsresult rv;
=======
nsresult nsJARURI::SetSpecWithBase(const nsACString& aSpec, nsIURI* aBaseURL) {
  nsresult rv;
>>>>>>> upstream-releases

  nsCOMPtr<nsIIOService> ioServ(do_GetIOService(&rv));
  NS_ENSURE_SUCCESS(rv, rv);

  nsAutoCString scheme;
  rv = ioServ->ExtractScheme(aSpec, scheme);
  if (NS_FAILED(rv)) {
    // not an absolute URI
    if (!aBaseURL) return NS_ERROR_MALFORMED_URI;

    RefPtr<nsJARURI> otherJAR = do_QueryObject(aBaseURL);
    NS_ENSURE_TRUE(otherJAR, NS_NOINTERFACE);

    mJARFile = otherJAR->mJARFile;

    nsCOMPtr<nsIURI> entry;

    rv = NS_MutateURI(NS_STANDARDURLMUTATOR_CONTRACTID)
             .Apply(NS_MutatorMethod(&nsIStandardURLMutator::Init,
                                     nsIStandardURL::URLTYPE_NO_AUTHORITY, -1,
                                     nsCString(aSpec), mCharsetHint.get(),
                                     otherJAR->mJAREntry, nullptr))
             .Finalize(entry);
    if (NS_FAILED(rv)) {
      return rv;
    }

    mJAREntry = do_QueryInterface(entry);
    if (!mJAREntry) return NS_NOINTERFACE;

    return NS_OK;
  }

  NS_ENSURE_TRUE(scheme.EqualsLiteral("jar"), NS_ERROR_MALFORMED_URI);

  nsACString::const_iterator begin, end;
  aSpec.BeginReading(begin);
  aSpec.EndReading(end);

  while (begin != end && *begin != ':') ++begin;

  ++begin;  // now we're past the "jar:"

  nsACString::const_iterator delim_begin = begin;
  nsACString::const_iterator delim_end = end;
  nsACString::const_iterator frag = begin;

  if (FindInReadable(NS_JAR_DELIMITER, delim_begin, delim_end)) {
    frag = delim_end;
  }
  while (frag != end && (*frag != '#' && *frag != '?')) {
    ++frag;
  }
  if (frag != end) {
    // there was a fragment or query, mark that as the end of the URL to scan
    end = frag;
  }

  // Search backward from the end for the "!/" delimiter. Remember, jar URLs
  // can nest, e.g.:
  //    jar:jar:http://www.foo.com/bar.jar!/a.jar!/b.html
  // This gets the b.html document from out of the a.jar file, that's
  // contained within the bar.jar file.
  // Also, the outermost "inner" URI may be a relative URI:
  //   jar:../relative.jar!/a.html

  delim_begin = begin;
  delim_end = end;

  if (!RFindInReadable(NS_JAR_DELIMITER, delim_begin, delim_end)) {
    return NS_ERROR_MALFORMED_URI;
  }

  rv = ioServ->NewURI(Substring(begin, delim_begin), mCharsetHint.get(),
                      aBaseURL, getter_AddRefs(mJARFile));
  if (NS_FAILED(rv)) return rv;

  // skip over any extra '/' chars
  while (*delim_end == '/') ++delim_end;

  aSpec.EndReading(end);  // set to the original 'end'
  return SetJAREntry(Substring(delim_end, end));
}

NS_IMETHODIMP
<<<<<<< HEAD
nsJARURI::GetPrePath(nsACString &prePath) {
  prePath = NS_JAR_SCHEME;
  return NS_OK;
||||||| merged common ancestors
nsJARURI::GetPrePath(nsACString &prePath)
{
    prePath = NS_JAR_SCHEME;
    return NS_OK;
=======
nsJARURI::GetPrePath(nsACString& prePath) {
  prePath = NS_JAR_SCHEME;
  return NS_OK;
>>>>>>> upstream-releases
}

NS_IMETHODIMP
<<<<<<< HEAD
nsJARURI::GetScheme(nsACString &aScheme) {
  aScheme = "jar";
  return NS_OK;
||||||| merged common ancestors
nsJARURI::GetScheme(nsACString &aScheme)
{
    aScheme = "jar";
    return NS_OK;
=======
nsJARURI::GetScheme(nsACString& aScheme) {
  aScheme = "jar";
  return NS_OK;
>>>>>>> upstream-releases
}

<<<<<<< HEAD
nsresult nsJARURI::SetScheme(const nsACString &aScheme) {
  // doesn't make sense to set the scheme of a jar: URL
  return NS_ERROR_FAILURE;
||||||| merged common ancestors
nsresult
nsJARURI::SetScheme(const nsACString &aScheme)
{
    // doesn't make sense to set the scheme of a jar: URL
    return NS_ERROR_FAILURE;
=======
nsresult nsJARURI::SetScheme(const nsACString& aScheme) {
  // doesn't make sense to set the scheme of a jar: URL
  return NS_ERROR_FAILURE;
>>>>>>> upstream-releases
}

NS_IMETHODIMP
<<<<<<< HEAD
nsJARURI::GetUserPass(nsACString &aUserPass) { return NS_ERROR_FAILURE; }
||||||| merged common ancestors
nsJARURI::GetUserPass(nsACString &aUserPass)
{
    return NS_ERROR_FAILURE;
}
=======
nsJARURI::GetUserPass(nsACString& aUserPass) { return NS_ERROR_FAILURE; }
>>>>>>> upstream-releases

<<<<<<< HEAD
nsresult nsJARURI::SetUserPass(const nsACString &aUserPass) {
  return NS_ERROR_FAILURE;
||||||| merged common ancestors
nsresult
nsJARURI::SetUserPass(const nsACString &aUserPass)
{
    return NS_ERROR_FAILURE;
=======
nsresult nsJARURI::SetUserPass(const nsACString& aUserPass) {
  return NS_ERROR_FAILURE;
>>>>>>> upstream-releases
}

NS_IMETHODIMP
<<<<<<< HEAD
nsJARURI::GetUsername(nsACString &aUsername) { return NS_ERROR_FAILURE; }
||||||| merged common ancestors
nsJARURI::GetUsername(nsACString &aUsername)
{
    return NS_ERROR_FAILURE;
}
=======
nsJARURI::GetUsername(nsACString& aUsername) { return NS_ERROR_FAILURE; }
>>>>>>> upstream-releases

<<<<<<< HEAD
nsresult nsJARURI::SetUsername(const nsACString &aUsername) {
  return NS_ERROR_FAILURE;
||||||| merged common ancestors
nsresult
nsJARURI::SetUsername(const nsACString &aUsername)
{
    return NS_ERROR_FAILURE;
=======
nsresult nsJARURI::SetUsername(const nsACString& aUsername) {
  return NS_ERROR_FAILURE;
>>>>>>> upstream-releases
}

NS_IMETHODIMP
<<<<<<< HEAD
nsJARURI::GetPassword(nsACString &aPassword) { return NS_ERROR_FAILURE; }
||||||| merged common ancestors
nsJARURI::GetPassword(nsACString &aPassword)
{
    return NS_ERROR_FAILURE;
}
=======
nsJARURI::GetPassword(nsACString& aPassword) { return NS_ERROR_FAILURE; }
>>>>>>> upstream-releases

<<<<<<< HEAD
nsresult nsJARURI::SetPassword(const nsACString &aPassword) {
  return NS_ERROR_FAILURE;
||||||| merged common ancestors
nsresult
nsJARURI::SetPassword(const nsACString &aPassword)
{
    return NS_ERROR_FAILURE;
=======
nsresult nsJARURI::SetPassword(const nsACString& aPassword) {
  return NS_ERROR_FAILURE;
>>>>>>> upstream-releases
}

NS_IMETHODIMP
<<<<<<< HEAD
nsJARURI::GetHostPort(nsACString &aHostPort) { return NS_ERROR_FAILURE; }
||||||| merged common ancestors
nsJARURI::GetHostPort(nsACString &aHostPort)
{
    return NS_ERROR_FAILURE;
}
=======
nsJARURI::GetHostPort(nsACString& aHostPort) { return NS_ERROR_FAILURE; }
>>>>>>> upstream-releases

<<<<<<< HEAD
nsresult nsJARURI::SetHostPort(const nsACString &aHostPort) {
  return NS_ERROR_FAILURE;
||||||| merged common ancestors
nsresult
nsJARURI::SetHostPort(const nsACString &aHostPort)
{
    return NS_ERROR_FAILURE;
=======
nsresult nsJARURI::SetHostPort(const nsACString& aHostPort) {
  return NS_ERROR_FAILURE;
>>>>>>> upstream-releases
}

NS_IMETHODIMP
<<<<<<< HEAD
nsJARURI::GetHost(nsACString &aHost) { return NS_ERROR_FAILURE; }
||||||| merged common ancestors
nsJARURI::GetHost(nsACString &aHost)
{
    return NS_ERROR_FAILURE;
}
=======
nsJARURI::GetHost(nsACString& aHost) { return NS_ERROR_FAILURE; }
>>>>>>> upstream-releases

<<<<<<< HEAD
nsresult nsJARURI::SetHost(const nsACString &aHost) { return NS_ERROR_FAILURE; }
||||||| merged common ancestors
nsresult
nsJARURI::SetHost(const nsACString &aHost)
{
    return NS_ERROR_FAILURE;
}
=======
nsresult nsJARURI::SetHost(const nsACString& aHost) { return NS_ERROR_FAILURE; }
>>>>>>> upstream-releases

NS_IMETHODIMP
<<<<<<< HEAD
nsJARURI::GetPort(int32_t *aPort) { return NS_ERROR_FAILURE; }
||||||| merged common ancestors
nsJARURI::GetPort(int32_t *aPort)
{
    return NS_ERROR_FAILURE;
}
=======
nsJARURI::GetPort(int32_t* aPort) { return NS_ERROR_FAILURE; }
>>>>>>> upstream-releases

nsresult nsJARURI::SetPort(int32_t aPort) { return NS_ERROR_FAILURE; }

<<<<<<< HEAD
nsresult nsJARURI::GetPathQueryRef(nsACString &aPath) {
  nsAutoCString entrySpec;
  mJAREntry->GetSpec(entrySpec);
  return FormatSpec(entrySpec, aPath, false);
||||||| merged common ancestors
nsresult
nsJARURI::GetPathQueryRef(nsACString &aPath)
{
    nsAutoCString entrySpec;
    mJAREntry->GetSpec(entrySpec);
    return FormatSpec(entrySpec, aPath, false);
=======
nsresult nsJARURI::GetPathQueryRef(nsACString& aPath) {
  nsAutoCString entrySpec;
  mJAREntry->GetSpec(entrySpec);
  return FormatSpec(entrySpec, aPath, false);
>>>>>>> upstream-releases
}

<<<<<<< HEAD
nsresult nsJARURI::SetPathQueryRef(const nsACString &aPath) {
  return NS_ERROR_FAILURE;
||||||| merged common ancestors
nsresult
nsJARURI::SetPathQueryRef(const nsACString &aPath)
{
    return NS_ERROR_FAILURE;
=======
nsresult nsJARURI::SetPathQueryRef(const nsACString& aPath) {
  return NS_ERROR_FAILURE;
>>>>>>> upstream-releases
}

NS_IMETHODIMP
<<<<<<< HEAD
nsJARURI::GetAsciiSpec(nsACString &aSpec) {
  // XXX Shouldn't this like... make sure it returns ASCII or something?
  return GetSpec(aSpec);
||||||| merged common ancestors
nsJARURI::GetAsciiSpec(nsACString &aSpec)
{
    // XXX Shouldn't this like... make sure it returns ASCII or something?
    return GetSpec(aSpec);
=======
nsJARURI::GetAsciiSpec(nsACString& aSpec) {
  // XXX Shouldn't this like... make sure it returns ASCII or something?
  return GetSpec(aSpec);
>>>>>>> upstream-releases
}

NS_IMETHODIMP
<<<<<<< HEAD
nsJARURI::GetAsciiHostPort(nsACString &aHostPort) { return NS_ERROR_FAILURE; }
||||||| merged common ancestors
nsJARURI::GetAsciiHostPort(nsACString &aHostPort)
{
    return NS_ERROR_FAILURE;
}
=======
nsJARURI::GetAsciiHostPort(nsACString& aHostPort) { return NS_ERROR_FAILURE; }
>>>>>>> upstream-releases

NS_IMETHODIMP
<<<<<<< HEAD
nsJARURI::GetAsciiHost(nsACString &aHost) { return NS_ERROR_FAILURE; }
||||||| merged common ancestors
nsJARURI::GetAsciiHost(nsACString &aHost)
{
    return NS_ERROR_FAILURE;
}
=======
nsJARURI::GetAsciiHost(nsACString& aHost) { return NS_ERROR_FAILURE; }
>>>>>>> upstream-releases

NS_IMETHODIMP
<<<<<<< HEAD
nsJARURI::Equals(nsIURI *other, bool *result) {
  return EqualsInternal(other, eHonorRef, result);
||||||| merged common ancestors
nsJARURI::Equals(nsIURI *other, bool *result)
{
    return EqualsInternal(other, eHonorRef, result);
=======
nsJARURI::Equals(nsIURI* other, bool* result) {
  return EqualsInternal(other, eHonorRef, result);
>>>>>>> upstream-releases
}

NS_IMETHODIMP
<<<<<<< HEAD
nsJARURI::EqualsExceptRef(nsIURI *other, bool *result) {
  return EqualsInternal(other, eIgnoreRef, result);
||||||| merged common ancestors
nsJARURI::EqualsExceptRef(nsIURI *other, bool *result)
{
    return EqualsInternal(other, eIgnoreRef, result);
=======
nsJARURI::EqualsExceptRef(nsIURI* other, bool* result) {
  return EqualsInternal(other, eIgnoreRef, result);
>>>>>>> upstream-releases
}

// Helper method:
<<<<<<< HEAD
/* virtual */ nsresult nsJARURI::EqualsInternal(
    nsIURI *other, nsJARURI::RefHandlingEnum refHandlingMode, bool *result) {
  *result = false;

  if (!other) return NS_OK;  // not equal

  RefPtr<nsJARURI> otherJAR = do_QueryObject(other);
  if (!otherJAR) return NS_OK;  // not equal

  bool equal;
  nsresult rv = mJARFile->Equals(otherJAR->mJARFile, &equal);
  if (NS_FAILED(rv) || !equal) {
    return rv;  // not equal
  }
||||||| merged common ancestors
/* virtual */ nsresult
nsJARURI::EqualsInternal(nsIURI *other,
                         nsJARURI::RefHandlingEnum refHandlingMode,
                         bool *result)
{
    *result = false;

    if (!other)
        return NS_OK;	// not equal

    RefPtr<nsJARURI> otherJAR = do_QueryObject(other);
    if (!otherJAR)
        return NS_OK;   // not equal

    bool equal;
    nsresult rv = mJARFile->Equals(otherJAR->mJARFile, &equal);
    if (NS_FAILED(rv) || !equal) {
        return rv;   // not equal
    }
=======
/* virtual */
nsresult nsJARURI::EqualsInternal(nsIURI* other,
                                  nsJARURI::RefHandlingEnum refHandlingMode,
                                  bool* result) {
  *result = false;

  if (!other) return NS_OK;  // not equal
>>>>>>> upstream-releases

<<<<<<< HEAD
  return refHandlingMode == eHonorRef
             ? mJAREntry->Equals(otherJAR->mJAREntry, result)
             : mJAREntry->EqualsExceptRef(otherJAR->mJAREntry, result);
||||||| merged common ancestors
    return refHandlingMode == eHonorRef ?
        mJAREntry->Equals(otherJAR->mJAREntry, result) :
        mJAREntry->EqualsExceptRef(otherJAR->mJAREntry, result);
=======
  RefPtr<nsJARURI> otherJAR = do_QueryObject(other);
  if (!otherJAR) return NS_OK;  // not equal

  bool equal;
  nsresult rv = mJARFile->Equals(otherJAR->mJARFile, &equal);
  if (NS_FAILED(rv) || !equal) {
    return rv;  // not equal
  }

  return refHandlingMode == eHonorRef
             ? mJAREntry->Equals(otherJAR->mJAREntry, result)
             : mJAREntry->EqualsExceptRef(otherJAR->mJAREntry, result);
>>>>>>> upstream-releases
}

NS_IMETHODIMP
<<<<<<< HEAD
nsJARURI::SchemeIs(const char *i_Scheme, bool *o_Equals) {
  NS_ENSURE_ARG_POINTER(o_Equals);
  if (!i_Scheme) return NS_ERROR_INVALID_ARG;

  if (*i_Scheme == 'j' || *i_Scheme == 'J') {
    *o_Equals = PL_strcasecmp("jar", i_Scheme) ? false : true;
  } else {
    *o_Equals = false;
  }
  return NS_OK;
}
||||||| merged common ancestors
nsJARURI::SchemeIs(const char *i_Scheme, bool *o_Equals)
{
    NS_ENSURE_ARG_POINTER(o_Equals);
    if (!i_Scheme) return NS_ERROR_INVALID_ARG;

    if (*i_Scheme == 'j' || *i_Scheme == 'J') {
        *o_Equals = PL_strcasecmp("jar", i_Scheme) ? false : true;
    } else {
        *o_Equals = false;
    }
    return NS_OK;
}
=======
nsJARURI::SchemeIs(const char* i_Scheme, bool* o_Equals) {
  MOZ_ASSERT(o_Equals);
  if (!i_Scheme) {
    *o_Equals = false;
    return NS_OK;
  }
>>>>>>> upstream-releases

<<<<<<< HEAD
nsresult nsJARURI::Clone(nsIURI **result) {
  nsresult rv;
||||||| merged common ancestors
nsresult
nsJARURI::Clone(nsIURI **result)
{
    nsresult rv;
=======
  *o_Equals = PL_strcasecmp("jar", i_Scheme) ? false : true;
  return NS_OK;
}
>>>>>>> upstream-releases

<<<<<<< HEAD
  nsCOMPtr<nsIJARURI> uri;
  rv = CloneWithJARFileInternal(mJARFile, eHonorRef, getter_AddRefs(uri));
  if (NS_FAILED(rv)) return rv;
||||||| merged common ancestors
    nsCOMPtr<nsIJARURI> uri;
    rv = CloneWithJARFileInternal(mJARFile, eHonorRef, getter_AddRefs(uri));
    if (NS_FAILED(rv)) return rv;
=======
nsresult nsJARURI::Clone(nsIURI** result) {
  RefPtr<nsJARURI> uri = new nsJARURI();
  uri->mJARFile = mJARFile;
  uri->mJAREntry = mJAREntry;
  uri.forget(result);
>>>>>>> upstream-releases

<<<<<<< HEAD
  uri.forget(result);
  return NS_OK;
||||||| merged common ancestors
    uri.forget(result);
    return NS_OK;
=======
  return NS_OK;
>>>>>>> upstream-releases
}

NS_IMETHODIMP
<<<<<<< HEAD
nsJARURI::Resolve(const nsACString &relativePath, nsACString &result) {
  nsresult rv;
||||||| merged common ancestors
nsJARURI::Resolve(const nsACString &relativePath, nsACString &result)
{
    nsresult rv;
=======
nsJARURI::Resolve(const nsACString& relativePath, nsACString& result) {
  nsresult rv;
>>>>>>> upstream-releases

  nsCOMPtr<nsIIOService> ioServ(do_GetIOService(&rv));
  if (NS_FAILED(rv)) return rv;

  nsAutoCString scheme;
  rv = ioServ->ExtractScheme(relativePath, scheme);
  if (NS_SUCCEEDED(rv)) {
    // then aSpec is absolute
    result = relativePath;
    return NS_OK;
  }

  nsAutoCString resolvedPath;
  mJAREntry->Resolve(relativePath, resolvedPath);

  return FormatSpec(resolvedPath, result);
}

////////////////////////////////////////////////////////////////////////////////
// nsIURL methods:

NS_IMETHODIMP
<<<<<<< HEAD
nsJARURI::GetFilePath(nsACString &filePath) {
  return mJAREntry->GetFilePath(filePath);
||||||| merged common ancestors
nsJARURI::GetFilePath(nsACString& filePath)
{
    return mJAREntry->GetFilePath(filePath);
=======
nsJARURI::GetFilePath(nsACString& filePath) {
  return mJAREntry->GetFilePath(filePath);
>>>>>>> upstream-releases
}

<<<<<<< HEAD
nsresult nsJARURI::SetFilePath(const nsACString &filePath) {
  return NS_MutateURI(mJAREntry).SetFilePath(filePath).Finalize(mJAREntry);
||||||| merged common ancestors
nsresult
nsJARURI::SetFilePath(const nsACString& filePath)
{
    return NS_MutateURI(mJAREntry)
             .SetFilePath(filePath)
             .Finalize(mJAREntry);
=======
nsresult nsJARURI::SetFilePath(const nsACString& filePath) {
  return NS_MutateURI(mJAREntry).SetFilePath(filePath).Finalize(mJAREntry);
>>>>>>> upstream-releases
}

NS_IMETHODIMP
<<<<<<< HEAD
nsJARURI::GetQuery(nsACString &query) { return mJAREntry->GetQuery(query); }
||||||| merged common ancestors
nsJARURI::GetQuery(nsACString& query)
{
    return mJAREntry->GetQuery(query);
}
=======
nsJARURI::GetQuery(nsACString& query) { return mJAREntry->GetQuery(query); }
>>>>>>> upstream-releases

<<<<<<< HEAD
nsresult nsJARURI::SetQuery(const nsACString &query) {
  return NS_MutateURI(mJAREntry).SetQuery(query).Finalize(mJAREntry);
||||||| merged common ancestors
nsresult
nsJARURI::SetQuery(const nsACString& query)
{
    return NS_MutateURI(mJAREntry)
             .SetQuery(query)
             .Finalize(mJAREntry);
=======
nsresult nsJARURI::SetQuery(const nsACString& query) {
  return NS_MutateURI(mJAREntry).SetQuery(query).Finalize(mJAREntry);
>>>>>>> upstream-releases
}

<<<<<<< HEAD
nsresult nsJARURI::SetQueryWithEncoding(const nsACString &query,
                                        const Encoding *encoding) {
  return NS_MutateURI(mJAREntry)
      .SetQueryWithEncoding(query, encoding)
      .Finalize(mJAREntry);
||||||| merged common ancestors
nsresult
nsJARURI::SetQueryWithEncoding(const nsACString& query,
                               const Encoding* encoding)
{
    return NS_MutateURI(mJAREntry)
             .SetQueryWithEncoding(query, encoding)
             .Finalize(mJAREntry);
=======
nsresult nsJARURI::SetQueryWithEncoding(const nsACString& query,
                                        const Encoding* encoding) {
  return NS_MutateURI(mJAREntry)
      .SetQueryWithEncoding(query, encoding)
      .Finalize(mJAREntry);
>>>>>>> upstream-releases
}

NS_IMETHODIMP
<<<<<<< HEAD
nsJARURI::GetRef(nsACString &ref) { return mJAREntry->GetRef(ref); }
||||||| merged common ancestors
nsJARURI::GetRef(nsACString& ref)
{
    return mJAREntry->GetRef(ref);
}
=======
nsJARURI::GetRef(nsACString& ref) { return mJAREntry->GetRef(ref); }
>>>>>>> upstream-releases

<<<<<<< HEAD
nsresult nsJARURI::SetRef(const nsACString &ref) {
  return NS_MutateURI(mJAREntry).SetRef(ref).Finalize(mJAREntry);
||||||| merged common ancestors
nsresult
nsJARURI::SetRef(const nsACString& ref)
{
    return NS_MutateURI(mJAREntry)
             .SetRef(ref)
             .Finalize(mJAREntry);
=======
nsresult nsJARURI::SetRef(const nsACString& ref) {
  return NS_MutateURI(mJAREntry).SetRef(ref).Finalize(mJAREntry);
>>>>>>> upstream-releases
}

NS_IMETHODIMP
<<<<<<< HEAD
nsJARURI::GetDirectory(nsACString &directory) {
  return mJAREntry->GetDirectory(directory);
||||||| merged common ancestors
nsJARURI::GetDirectory(nsACString& directory)
{
    return mJAREntry->GetDirectory(directory);
=======
nsJARURI::GetDirectory(nsACString& directory) {
  return mJAREntry->GetDirectory(directory);
>>>>>>> upstream-releases
}

NS_IMETHODIMP
<<<<<<< HEAD
nsJARURI::GetFileName(nsACString &fileName) {
  return mJAREntry->GetFileName(fileName);
||||||| merged common ancestors
nsJARURI::GetFileName(nsACString& fileName)
{
    return mJAREntry->GetFileName(fileName);
=======
nsJARURI::GetFileName(nsACString& fileName) {
  return mJAREntry->GetFileName(fileName);
>>>>>>> upstream-releases
}

<<<<<<< HEAD
nsresult nsJARURI::SetFileNameInternal(const nsACString &fileName) {
  return NS_MutateURI(mJAREntry)
      .Apply(NS_MutatorMethod(&nsIURLMutator::SetFileName, nsCString(fileName),
                              nullptr))
      .Finalize(mJAREntry);
||||||| merged common ancestors
nsresult
nsJARURI::SetFileNameInternal(const nsACString& fileName)
{
    return NS_MutateURI(mJAREntry)
        .Apply(NS_MutatorMethod(&nsIURLMutator::SetFileName,
                                nsCString(fileName), nullptr))
        .Finalize(mJAREntry);
=======
nsresult nsJARURI::SetFileNameInternal(const nsACString& fileName) {
  return NS_MutateURI(mJAREntry)
      .Apply(NS_MutatorMethod(&nsIURLMutator::SetFileName, nsCString(fileName),
                              nullptr))
      .Finalize(mJAREntry);
>>>>>>> upstream-releases
}

NS_IMETHODIMP
<<<<<<< HEAD
nsJARURI::GetFileBaseName(nsACString &fileBaseName) {
  return mJAREntry->GetFileBaseName(fileBaseName);
||||||| merged common ancestors
nsJARURI::GetFileBaseName(nsACString& fileBaseName)
{
    return mJAREntry->GetFileBaseName(fileBaseName);
=======
nsJARURI::GetFileBaseName(nsACString& fileBaseName) {
  return mJAREntry->GetFileBaseName(fileBaseName);
>>>>>>> upstream-releases
}

<<<<<<< HEAD
nsresult nsJARURI::SetFileBaseNameInternal(const nsACString &fileBaseName) {
  return NS_MutateURI(mJAREntry)
      .Apply(NS_MutatorMethod(&nsIURLMutator::SetFileBaseName,
                              nsCString(fileBaseName), nullptr))
      .Finalize(mJAREntry);
||||||| merged common ancestors
nsresult
nsJARURI::SetFileBaseNameInternal(const nsACString& fileBaseName)
{
    return NS_MutateURI(mJAREntry)
        .Apply(NS_MutatorMethod(&nsIURLMutator::SetFileBaseName,
                                nsCString(fileBaseName), nullptr))
        .Finalize(mJAREntry);
=======
nsresult nsJARURI::SetFileBaseNameInternal(const nsACString& fileBaseName) {
  return NS_MutateURI(mJAREntry)
      .Apply(NS_MutatorMethod(&nsIURLMutator::SetFileBaseName,
                              nsCString(fileBaseName), nullptr))
      .Finalize(mJAREntry);
>>>>>>> upstream-releases
}

NS_IMETHODIMP
<<<<<<< HEAD
nsJARURI::GetFileExtension(nsACString &fileExtension) {
  return mJAREntry->GetFileExtension(fileExtension);
||||||| merged common ancestors
nsJARURI::GetFileExtension(nsACString& fileExtension)
{
    return mJAREntry->GetFileExtension(fileExtension);
=======
nsJARURI::GetFileExtension(nsACString& fileExtension) {
  return mJAREntry->GetFileExtension(fileExtension);
>>>>>>> upstream-releases
}

<<<<<<< HEAD
nsresult nsJARURI::SetFileExtensionInternal(const nsACString &fileExtension) {
  return NS_MutateURI(mJAREntry)
      .Apply(NS_MutatorMethod(&nsIURLMutator::SetFileExtension,
                              nsCString(fileExtension), nullptr))
      .Finalize(mJAREntry);
||||||| merged common ancestors
nsresult
nsJARURI::SetFileExtensionInternal(const nsACString& fileExtension)
{
    return NS_MutateURI(mJAREntry)
        .Apply(NS_MutatorMethod(&nsIURLMutator::SetFileExtension,
                                nsCString(fileExtension), nullptr))
        .Finalize(mJAREntry);
=======
nsresult nsJARURI::SetFileExtensionInternal(const nsACString& fileExtension) {
  return NS_MutateURI(mJAREntry)
      .Apply(NS_MutatorMethod(&nsIURLMutator::SetFileExtension,
                              nsCString(fileExtension), nullptr))
      .Finalize(mJAREntry);
>>>>>>> upstream-releases
}

NS_IMETHODIMP
<<<<<<< HEAD
nsJARURI::GetCommonBaseSpec(nsIURI *uriToCompare, nsACString &commonSpec) {
  commonSpec.Truncate();
||||||| merged common ancestors
nsJARURI::GetCommonBaseSpec(nsIURI* uriToCompare, nsACString& commonSpec)
{
    commonSpec.Truncate();
=======
nsJARURI::GetCommonBaseSpec(nsIURI* uriToCompare, nsACString& commonSpec) {
  commonSpec.Truncate();
>>>>>>> upstream-releases

  NS_ENSURE_ARG_POINTER(uriToCompare);

  commonSpec.Truncate();
  nsCOMPtr<nsIJARURI> otherJARURI(do_QueryInterface(uriToCompare));
  if (!otherJARURI) {
    // Nothing in common
    return NS_OK;
  }

  nsCOMPtr<nsIURI> otherJARFile;
  nsresult rv = otherJARURI->GetJARFile(getter_AddRefs(otherJARFile));
  if (NS_FAILED(rv)) return rv;

  bool equal;
  rv = mJARFile->Equals(otherJARFile, &equal);
  if (NS_FAILED(rv)) return rv;

  if (!equal) {
    // See what the JAR file URIs have in common
    nsCOMPtr<nsIURL> ourJARFileURL(do_QueryInterface(mJARFile));
    if (!ourJARFileURL) {
      // Not a URL, so nothing in common
      return NS_OK;
    }
    nsAutoCString common;
    rv = ourJARFileURL->GetCommonBaseSpec(otherJARFile, common);
    if (NS_FAILED(rv)) return rv;

    commonSpec = NS_JAR_SCHEME + common;
    return NS_OK;
  }

  // At this point we have the same JAR file.  Compare the JAREntrys
  nsAutoCString otherEntry;
  rv = otherJARURI->GetJAREntry(otherEntry);
  if (NS_FAILED(rv)) return rv;

  nsCOMPtr<nsIURL> url;
  rv = CreateEntryURL(otherEntry, nullptr, getter_AddRefs(url));
  if (NS_FAILED(rv)) return rv;

  nsAutoCString common;
  rv = mJAREntry->GetCommonBaseSpec(url, common);
  if (NS_FAILED(rv)) return rv;

  rv = FormatSpec(common, commonSpec);
  return rv;
}

NS_IMETHODIMP
<<<<<<< HEAD
nsJARURI::GetRelativeSpec(nsIURI *uriToCompare, nsACString &relativeSpec) {
  GetSpec(relativeSpec);
||||||| merged common ancestors
nsJARURI::GetRelativeSpec(nsIURI* uriToCompare, nsACString& relativeSpec)
{
    GetSpec(relativeSpec);
=======
nsJARURI::GetRelativeSpec(nsIURI* uriToCompare, nsACString& relativeSpec) {
  GetSpec(relativeSpec);
>>>>>>> upstream-releases

  NS_ENSURE_ARG_POINTER(uriToCompare);

  nsCOMPtr<nsIJARURI> otherJARURI(do_QueryInterface(uriToCompare));
  if (!otherJARURI) {
    // Nothing in common
    return NS_OK;
  }

  nsCOMPtr<nsIURI> otherJARFile;
  nsresult rv = otherJARURI->GetJARFile(getter_AddRefs(otherJARFile));
  if (NS_FAILED(rv)) return rv;

  bool equal;
  rv = mJARFile->Equals(otherJARFile, &equal);
  if (NS_FAILED(rv)) return rv;

  if (!equal) {
    // We live in different JAR files.  Nothing in common.
    return rv;
  }

  // Same JAR file.  Compare the JAREntrys
  nsAutoCString otherEntry;
  rv = otherJARURI->GetJAREntry(otherEntry);
  if (NS_FAILED(rv)) return rv;

  nsCOMPtr<nsIURL> url;
  rv = CreateEntryURL(otherEntry, nullptr, getter_AddRefs(url));
  if (NS_FAILED(rv)) return rv;

  nsAutoCString relativeEntrySpec;
  rv = mJAREntry->GetRelativeSpec(url, relativeEntrySpec);
  if (NS_FAILED(rv)) return rv;

  if (!StringBeginsWith(relativeEntrySpec, NS_BOGUS_ENTRY_SCHEME)) {
    // An actual relative spec!
    relativeSpec = relativeEntrySpec;
  }
  return rv;
}

////////////////////////////////////////////////////////////////////////////////
// nsIJARURI methods:

NS_IMETHODIMP
<<<<<<< HEAD
nsJARURI::GetJARFile(nsIURI **jarFile) { return GetInnerURI(jarFile); }
||||||| merged common ancestors
nsJARURI::GetJARFile(nsIURI* *jarFile)
{
    return GetInnerURI(jarFile);
}
=======
nsJARURI::GetJARFile(nsIURI** jarFile) { return GetInnerURI(jarFile); }
>>>>>>> upstream-releases

NS_IMETHODIMP
<<<<<<< HEAD
nsJARURI::GetJAREntry(nsACString &entryPath) {
  nsAutoCString filePath;
  mJAREntry->GetFilePath(filePath);
  NS_ASSERTION(filePath.Length() > 0, "path should never be empty!");
  // Trim off the leading '/'
  entryPath = Substring(filePath, 1, filePath.Length() - 1);
  return NS_OK;
||||||| merged common ancestors
nsJARURI::GetJAREntry(nsACString &entryPath)
{
    nsAutoCString filePath;
    mJAREntry->GetFilePath(filePath);
    NS_ASSERTION(filePath.Length() > 0, "path should never be empty!");
    // Trim off the leading '/'
    entryPath = Substring(filePath, 1, filePath.Length() - 1);
    return NS_OK;
=======
nsJARURI::GetJAREntry(nsACString& entryPath) {
  nsAutoCString filePath;
  mJAREntry->GetFilePath(filePath);
  NS_ASSERTION(filePath.Length() > 0, "path should never be empty!");
  // Trim off the leading '/'
  entryPath = Substring(filePath, 1, filePath.Length() - 1);
  return NS_OK;
>>>>>>> upstream-releases
}

<<<<<<< HEAD
nsresult nsJARURI::SetJAREntry(const nsACString &entryPath) {
  return CreateEntryURL(entryPath, mCharsetHint.get(),
                        getter_AddRefs(mJAREntry));
}

NS_IMETHODIMP
nsJARURI::CloneWithJARFile(nsIURI *jarFile, nsIJARURI **result) {
  return CloneWithJARFileInternal(jarFile, eHonorRef, result);
}

nsresult nsJARURI::CloneWithJARFileInternal(
    nsIURI *jarFile, nsJARURI::RefHandlingEnum refHandlingMode,
    nsIJARURI **result) {
  return CloneWithJARFileInternal(jarFile, refHandlingMode, EmptyCString(),
                                  result);
}

nsresult nsJARURI::CloneWithJARFileInternal(
    nsIURI *jarFile, nsJARURI::RefHandlingEnum refHandlingMode,
    const nsACString &newRef, nsIJARURI **result) {
  if (!jarFile) {
    return NS_ERROR_INVALID_ARG;
  }

  nsresult rv = NS_OK;
  nsCOMPtr<nsIURI> newJARFile = jarFile;
  nsCOMPtr<nsIURI> newJAREntryURI;
  if (refHandlingMode == eHonorRef) {
    newJAREntryURI = mJAREntry;
  } else if (refHandlingMode == eReplaceRef) {
    rv = NS_GetURIWithNewRef(mJAREntry, newRef, getter_AddRefs(newJAREntryURI));
  } else {
    rv = NS_GetURIWithoutRef(mJAREntry, getter_AddRefs(newJAREntryURI));
  }
  if (NS_FAILED(rv)) return rv;

  nsCOMPtr<nsIURL> newJAREntry(do_QueryInterface(newJAREntryURI));
  NS_ASSERTION(newJAREntry, "This had better QI to nsIURL!");

  RefPtr<nsJARURI> uri = new nsJARURI();
  uri->mJARFile = newJARFile;
  uri->mJAREntry = newJAREntry;
  uri.forget(result);

  return NS_OK;
||||||| merged common ancestors
nsresult
nsJARURI::SetJAREntry(const nsACString &entryPath)
{
    return CreateEntryURL(entryPath, mCharsetHint.get(),
                          getter_AddRefs(mJAREntry));
}

NS_IMETHODIMP
nsJARURI::CloneWithJARFile(nsIURI *jarFile, nsIJARURI **result)
{
    return CloneWithJARFileInternal(jarFile, eHonorRef, result);
}

nsresult
nsJARURI::CloneWithJARFileInternal(nsIURI *jarFile,
                                   nsJARURI::RefHandlingEnum refHandlingMode,
                                   nsIJARURI **result)
{
  return CloneWithJARFileInternal(jarFile, refHandlingMode, EmptyCString(), result);
}

nsresult
nsJARURI::CloneWithJARFileInternal(nsIURI *jarFile,
                                   nsJARURI::RefHandlingEnum refHandlingMode,
                                   const nsACString& newRef,
                                   nsIJARURI **result)
{
    if (!jarFile) {
        return NS_ERROR_INVALID_ARG;
    }

    nsresult rv = NS_OK;
    nsCOMPtr<nsIURI> newJARFile = jarFile;
    nsCOMPtr<nsIURI> newJAREntryURI;
    if (refHandlingMode == eHonorRef) {
      newJAREntryURI = mJAREntry;
    } else if (refHandlingMode == eReplaceRef) {
      rv = NS_GetURIWithNewRef(mJAREntry, newRef, getter_AddRefs(newJAREntryURI));
    } else {
      rv = NS_GetURIWithoutRef(mJAREntry, getter_AddRefs(newJAREntryURI));
    }
    if (NS_FAILED(rv)) return rv;

    nsCOMPtr<nsIURL> newJAREntry(do_QueryInterface(newJAREntryURI));
    NS_ASSERTION(newJAREntry, "This had better QI to nsIURL!");

    RefPtr<nsJARURI> uri = new nsJARURI();
    uri->mJARFile = newJARFile;
    uri->mJAREntry = newJAREntry;
    uri.forget(result);

    return NS_OK;
=======
nsresult nsJARURI::SetJAREntry(const nsACString& entryPath) {
  return CreateEntryURL(entryPath, mCharsetHint.get(),
                        getter_AddRefs(mJAREntry));
>>>>>>> upstream-releases
}

////////////////////////////////////////////////////////////////////////////////

NS_IMETHODIMP
<<<<<<< HEAD
nsJARURI::GetInnerURI(nsIURI **aURI) {
  nsCOMPtr<nsIURI> uri = mJARFile;
  uri.forget(aURI);
  return NS_OK;
||||||| merged common ancestors
nsJARURI::GetInnerURI(nsIURI **aURI)
{
    nsCOMPtr<nsIURI> uri = mJARFile;
    uri.forget(aURI);
    return NS_OK;
=======
nsJARURI::GetInnerURI(nsIURI** aURI) {
  nsCOMPtr<nsIURI> uri = mJARFile;
  uri.forget(aURI);
  return NS_OK;
>>>>>>> upstream-releases
}

NS_IMETHODIMP
<<<<<<< HEAD
nsJARURI::GetInnermostURI(nsIURI **uri) {
  return NS_ImplGetInnermostURI(this, uri);
||||||| merged common ancestors
nsJARURI::GetInnermostURI(nsIURI** uri)
{
    return NS_ImplGetInnermostURI(this, uri);
=======
nsJARURI::GetInnermostURI(nsIURI** uri) {
  return NS_ImplGetInnermostURI(this, uri);
>>>>>>> upstream-releases
}

<<<<<<< HEAD
////////////////////////////////////////////////////////////////////////////////
// nsIIPCSerializableURI methods:

void nsJARURI::Serialize(URIParams &aParams) {
  JARURIParams params;
||||||| merged common ancestors
////////////////////////////////////////////////////////////////////////////////
// nsIIPCSerializableURI methods:

void
nsJARURI::Serialize(URIParams& aParams)
{
    JARURIParams params;
=======
void nsJARURI::Serialize(URIParams& aParams) {
  JARURIParams params;
>>>>>>> upstream-releases

  SerializeURI(mJARFile, params.jarFile());
  SerializeURI(mJAREntry, params.jarEntry());
  params.charset() = mCharsetHint;

  aParams = params;
}

<<<<<<< HEAD
bool nsJARURI::Deserialize(const URIParams &aParams) {
  if (aParams.type() != URIParams::TJARURIParams) {
    NS_ERROR("Received unknown parameters from the other process!");
    return false;
  }
||||||| merged common ancestors
bool
nsJARURI::Deserialize(const URIParams& aParams)
{
    if (aParams.type() != URIParams::TJARURIParams) {
        NS_ERROR("Received unknown parameters from the other process!");
        return false;
    }
=======
bool nsJARURI::Deserialize(const URIParams& aParams) {
  if (aParams.type() != URIParams::TJARURIParams) {
    NS_ERROR("Received unknown parameters from the other process!");
    return false;
  }
>>>>>>> upstream-releases

<<<<<<< HEAD
  const JARURIParams &params = aParams.get_JARURIParams();
||||||| merged common ancestors
    const JARURIParams& params = aParams.get_JARURIParams();
=======
  const JARURIParams& params = aParams.get_JARURIParams();
>>>>>>> upstream-releases

  nsCOMPtr<nsIURI> file = DeserializeURI(params.jarFile());
  if (!file) {
    NS_ERROR("Couldn't deserialize jar file URI!");
    return false;
  }

  nsCOMPtr<nsIURI> entry = DeserializeURI(params.jarEntry());
  if (!entry) {
    NS_ERROR("Couldn't deserialize jar entry URI!");
    return false;
  }

  nsCOMPtr<nsIURL> entryURL = do_QueryInterface(entry);
  if (!entryURL) {
    NS_ERROR("Couldn't QI jar entry URI to nsIURL!");
    return false;
  }

  mJARFile.swap(file);
  mJAREntry.swap(entryURL);
  mCharsetHint = params.charset();

  return true;
}
