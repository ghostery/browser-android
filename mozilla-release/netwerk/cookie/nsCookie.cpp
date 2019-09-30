/* -*- Mode: C++; tab-width: 4; indent-tabs-mode: nil; c-basic-offset: 2 -*- */
/* This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/. */

#include "mozilla/Encoding.h"
#include "mozilla/dom/ToJSValue.h"
#include "mozilla/StaticPrefs.h"
#include "nsAutoPtr.h"
#include "nsCookie.h"
#include <stdlib.h>

/******************************************************************************
 * nsCookie:
<<<<<<< HEAD
 * string helper impl
 ******************************************************************************/

// copy aSource strings into contiguous storage provided in aDest1,
// providing terminating nulls for each destination string.
static inline void StrBlockCopy(const nsACString &aSource1,
                                const nsACString &aSource2,
                                const nsACString &aSource3,
                                const nsACString &aSource4, char *&aDest1,
                                char *&aDest2, char *&aDest3, char *&aDest4,
                                char *&aDestEnd) {
  size_t len1 = aSource1.Length();
  memcpy(aDest1, aSource1.BeginReading(), len1);
  aDest1[len1] = 0;

  aDest2 = aDest1 + len1 + 1;

  size_t len2 = aSource2.Length();
  memcpy(aDest2, aSource2.BeginReading(), len2);
  aDest2[len2] = 0;

  aDest3 = aDest2 + len2 + 1;

  size_t len3 = aSource3.Length();
  memcpy(aDest3, aSource3.BeginReading(), len3);
  aDest3[len3] = 0;

  aDest4 = aDest3 + len3 + 1;

  size_t len4 = aSource4.Length();
  memcpy(aDest4, aSource4.BeginReading(), len4);
  aDest4[len4] = 0;

  // Intentionally no + 1 here!
  aDestEnd = aDest4 + len4;
}

/******************************************************************************
 * nsCookie:
||||||| merged common ancestors
 * string helper impl
 ******************************************************************************/

// copy aSource strings into contiguous storage provided in aDest1,
// providing terminating nulls for each destination string.
static inline void
StrBlockCopy(const nsACString &aSource1,
             const nsACString &aSource2,
             const nsACString &aSource3,
             const nsACString &aSource4,
             char             *&aDest1,
             char             *&aDest2,
             char             *&aDest3,
             char             *&aDest4,
             char             *&aDestEnd)
{
  size_t len1 = aSource1.Length();
  memcpy(aDest1, aSource1.BeginReading(), len1);
  aDest1[len1] = 0;

  aDest2 = aDest1 + len1 + 1;

  size_t len2 = aSource2.Length();
  memcpy(aDest2, aSource2.BeginReading(), len2);
  aDest2[len2] = 0;

  aDest3 = aDest2 + len2 + 1;

  size_t len3 = aSource3.Length();
  memcpy(aDest3, aSource3.BeginReading(), len3);
  aDest3[len3] = 0;

  aDest4 = aDest3 + len3 + 1;

  size_t len4 = aSource4.Length();
  memcpy(aDest4, aSource4.BeginReading(), len4);
  aDest4[len4] = 0;

  // Intentionally no + 1 here!
  aDestEnd = aDest4 + len4;
}

/******************************************************************************
 * nsCookie:
=======
>>>>>>> upstream-releases
 * creation helper
 ******************************************************************************/

// This is a counter that keeps track of the last used creation time, each time
// we create a new nsCookie. This is nominally the time (in microseconds) the
// cookie was created, but is guaranteed to be monotonically increasing for
// cookies added at runtime after the database has been read in. This is
// necessary to enforce ordering among cookies whose creation times would
// otherwise overlap, since it's possible two cookies may be created at the same
// time, or that the system clock isn't monotonic.
static int64_t gLastCreationTime;

int64_t nsCookie::GenerateUniqueCreationTime(int64_t aCreationTime) {
  // Check if the creation time given to us is greater than the running maximum
  // (it should always be monotonically increasing).
  if (aCreationTime > gLastCreationTime) {
    gLastCreationTime = aCreationTime;
    return aCreationTime;
  }

  // Make up our own.
  return ++gLastCreationTime;
}

<<<<<<< HEAD
nsCookie *nsCookie::Create(const nsACString &aName, const nsACString &aValue,
                           const nsACString &aHost, const nsACString &aPath,
                           int64_t aExpiry, int64_t aLastAccessed,
                           int64_t aCreationTime, bool aIsSession,
                           bool aIsSecure, bool aIsHttpOnly,
                           const OriginAttributes &aOriginAttributes,
                           int32_t aSameSite) {
||||||| merged common ancestors
nsCookie *
nsCookie::Create(const nsACString &aName,
                 const nsACString &aValue,
                 const nsACString &aHost,
                 const nsACString &aPath,
                 int64_t           aExpiry,
                 int64_t           aLastAccessed,
                 int64_t           aCreationTime,
                 bool              aIsSession,
                 bool              aIsSecure,
                 bool              aIsHttpOnly,
                 const OriginAttributes& aOriginAttributes,
                 int32_t           aSameSite)
{
=======
// static
already_AddRefed<nsCookie> nsCookie::Create(
    const nsACString& aName, const nsACString& aValue, const nsACString& aHost,
    const nsACString& aPath, int64_t aExpiry, int64_t aLastAccessed,
    int64_t aCreationTime, bool aIsSession, bool aIsSecure, bool aIsHttpOnly,
    const OriginAttributes& aOriginAttributes, int32_t aSameSite,
    int32_t aRawSameSite) {
  mozilla::net::CookieStruct cookieData(
      nsCString(aName), nsCString(aValue), nsCString(aHost), nsCString(aPath),
      aExpiry, aLastAccessed, aCreationTime, aIsHttpOnly, aIsSession, aIsSecure,
      aSameSite, aRawSameSite);

  return Create(cookieData, aOriginAttributes);
}

already_AddRefed<nsCookie> nsCookie::Create(
    const mozilla::net::CookieStruct& aCookieData,
    const OriginAttributes& aOriginAttributes) {
  RefPtr<nsCookie> cookie = new nsCookie(aCookieData, aOriginAttributes);

>>>>>>> upstream-releases
  // Ensure mValue contains a valid UTF-8 sequence. Otherwise XPConnect will
  // truncate the string after the first invalid octet.
<<<<<<< HEAD
  nsAutoCString aUTF8Value;
  UTF_8_ENCODING->DecodeWithoutBOMHandling(aValue, aUTF8Value);

  // find the required string buffer size, adding 4 for the terminating nulls
  const uint32_t stringLength = aName.Length() + aUTF8Value.Length() +
                                aHost.Length() + aPath.Length() + 4;

  // allocate contiguous space for the nsCookie and its strings -
  // we store the strings in-line with the nsCookie to save allocations
  void *place = ::operator new(sizeof(nsCookie) + stringLength);
  if (!place) return nullptr;

  // assign string members
  char *name, *value, *host, *path, *end;
  name = static_cast<char *>(place) + sizeof(nsCookie);
  StrBlockCopy(aName, aUTF8Value, aHost, aPath, name, value, host, path, end);

  // If the creationTime given to us is higher than the running maximum, update
  // our maximum.
  if (aCreationTime > gLastCreationTime) gLastCreationTime = aCreationTime;
||||||| merged common ancestors
  nsAutoCString aUTF8Value;
  UTF_8_ENCODING->DecodeWithoutBOMHandling(aValue, aUTF8Value);

  // find the required string buffer size, adding 4 for the terminating nulls
  const uint32_t stringLength = aName.Length() + aUTF8Value.Length() +
                                aHost.Length() + aPath.Length() + 4;

  // allocate contiguous space for the nsCookie and its strings -
  // we store the strings in-line with the nsCookie to save allocations
  void *place = ::operator new(sizeof(nsCookie) + stringLength);
  if (!place)
    return nullptr;

  // assign string members
  char *name, *value, *host, *path, *end;
  name = static_cast<char *>(place) + sizeof(nsCookie);
  StrBlockCopy(aName, aUTF8Value, aHost, aPath,
               name, value, host, path, end);

  // If the creationTime given to us is higher than the running maximum, update
  // our maximum.
  if (aCreationTime > gLastCreationTime)
    gLastCreationTime = aCreationTime;
=======
  UTF_8_ENCODING->DecodeWithoutBOMHandling(aCookieData.value(),
                                           cookie->mData.value());
>>>>>>> upstream-releases

  // If the creationTime given to us is higher than the running maximum,
  // update our maximum.
  if (cookie->mData.creationTime() > gLastCreationTime) {
    gLastCreationTime = cookie->mData.creationTime();
  }

<<<<<<< HEAD
  // construct the cookie. placement new, oh yeah!
  return new (place) nsCookie(
      name, value, host, path, end, aExpiry, aLastAccessed, aCreationTime,
      aIsSession, aIsSecure, aIsHttpOnly, aOriginAttributes, aSameSite);
||||||| merged common ancestors
  // construct the cookie. placement new, oh yeah!
  return new (place) nsCookie(name, value, host, path, end,
                              aExpiry, aLastAccessed, aCreationTime,
                              aIsSession, aIsSecure, aIsHttpOnly,
                              aOriginAttributes, aSameSite);
=======
  // If sameSite is not a sensible value, assume strict
  if (cookie->mData.sameSite() < 0 ||
      cookie->mData.sameSite() > nsICookie::SAMESITE_STRICT) {
    cookie->mData.sameSite() = nsICookie::SAMESITE_STRICT;
  }

  // If rawSameSite is not a sensible value, assume equal to sameSite.
  if (!nsCookie::ValidateRawSame(cookie->mData)) {
    cookie->mData.rawSameSite() = nsICookie::SAMESITE_NONE;
  }

  return cookie.forget();
>>>>>>> upstream-releases
}

<<<<<<< HEAD
size_t nsCookie::SizeOfIncludingThis(
    mozilla::MallocSizeOf aMallocSizeOf) const {
  // There is no need to measure the sizes of the individual string
  // members, since the strings are stored in-line with the nsCookie.
  return aMallocSizeOf(this);
||||||| merged common ancestors
size_t
nsCookie::SizeOfIncludingThis(mozilla::MallocSizeOf aMallocSizeOf) const
{
    // There is no need to measure the sizes of the individual string
    // members, since the strings are stored in-line with the nsCookie.
    return aMallocSizeOf(this);
=======
size_t nsCookie::SizeOfIncludingThis(
    mozilla::MallocSizeOf aMallocSizeOf) const {
  return aMallocSizeOf(this) +
         mData.name().SizeOfExcludingThisIfUnshared(MallocSizeOf) +
         mData.value().SizeOfExcludingThisIfUnshared(MallocSizeOf) +
         mData.host().SizeOfExcludingThisIfUnshared(MallocSizeOf) +
         mData.path().SizeOfExcludingThisIfUnshared(MallocSizeOf);
>>>>>>> upstream-releases
}

bool nsCookie::IsStale() const {
  int64_t currentTimeInUsec = PR_Now();

<<<<<<< HEAD
  return currentTimeInUsec - LastAccessed() >
         CookieStaleThreshold() * PR_USEC_PER_SEC;
||||||| merged common ancestors
  return currentTimeInUsec - LastAccessed() > CookieStaleThreshold() * PR_USEC_PER_SEC;
=======
  return currentTimeInUsec - LastAccessed() >
         mozilla::StaticPrefs::network_cookie_staleThreshold() *
             PR_USEC_PER_SEC;
>>>>>>> upstream-releases
}

/******************************************************************************
 * nsCookie:
 * xpcom impl
 ******************************************************************************/

// xpcom getters
<<<<<<< HEAD
NS_IMETHODIMP nsCookie::GetName(nsACString &aName) {
  aName = Name();
  return NS_OK;
}
NS_IMETHODIMP nsCookie::GetValue(nsACString &aValue) {
  aValue = Value();
  return NS_OK;
}
NS_IMETHODIMP nsCookie::GetHost(nsACString &aHost) {
  aHost = Host();
  return NS_OK;
}
NS_IMETHODIMP nsCookie::GetRawHost(nsACString &aHost) {
  aHost = RawHost();
  return NS_OK;
}
NS_IMETHODIMP nsCookie::GetPath(nsACString &aPath) {
  aPath = Path();
  return NS_OK;
}
NS_IMETHODIMP nsCookie::GetExpiry(int64_t *aExpiry) {
  *aExpiry = Expiry();
  return NS_OK;
}
NS_IMETHODIMP nsCookie::GetIsSession(bool *aIsSession) {
  *aIsSession = IsSession();
  return NS_OK;
}
NS_IMETHODIMP nsCookie::GetIsDomain(bool *aIsDomain) {
  *aIsDomain = IsDomain();
  return NS_OK;
}
NS_IMETHODIMP nsCookie::GetIsSecure(bool *aIsSecure) {
  *aIsSecure = IsSecure();
  return NS_OK;
}
NS_IMETHODIMP nsCookie::GetIsHttpOnly(bool *aHttpOnly) {
  *aHttpOnly = IsHttpOnly();
  return NS_OK;
}
NS_IMETHODIMP nsCookie::GetCreationTime(int64_t *aCreation) {
  *aCreation = CreationTime();
  return NS_OK;
}
NS_IMETHODIMP nsCookie::GetLastAccessed(int64_t *aTime) {
  *aTime = LastAccessed();
  return NS_OK;
}
NS_IMETHODIMP nsCookie::GetSameSite(int32_t *aSameSite) {
  *aSameSite = SameSite();
  return NS_OK;
}
||||||| merged common ancestors
NS_IMETHODIMP nsCookie::GetName(nsACString &aName)         { aName = Name();            return NS_OK; }
NS_IMETHODIMP nsCookie::GetValue(nsACString &aValue)       { aValue = Value();          return NS_OK; }
NS_IMETHODIMP nsCookie::GetHost(nsACString &aHost)         { aHost = Host();            return NS_OK; }
NS_IMETHODIMP nsCookie::GetRawHost(nsACString &aHost)      { aHost = RawHost();         return NS_OK; }
NS_IMETHODIMP nsCookie::GetPath(nsACString &aPath)         { aPath = Path();            return NS_OK; }
NS_IMETHODIMP nsCookie::GetExpiry(int64_t *aExpiry)        { *aExpiry = Expiry();       return NS_OK; }
NS_IMETHODIMP nsCookie::GetIsSession(bool *aIsSession)   { *aIsSession = IsSession(); return NS_OK; }
NS_IMETHODIMP nsCookie::GetIsDomain(bool *aIsDomain)     { *aIsDomain = IsDomain();   return NS_OK; }
NS_IMETHODIMP nsCookie::GetIsSecure(bool *aIsSecure)     { *aIsSecure = IsSecure();   return NS_OK; }
NS_IMETHODIMP nsCookie::GetIsHttpOnly(bool *aHttpOnly)   { *aHttpOnly = IsHttpOnly(); return NS_OK; }
NS_IMETHODIMP nsCookie::GetCreationTime(int64_t *aCreation){ *aCreation = CreationTime(); return NS_OK; }
NS_IMETHODIMP nsCookie::GetLastAccessed(int64_t *aTime)    { *aTime = LastAccessed();   return NS_OK; }
NS_IMETHODIMP nsCookie::GetSameSite(int32_t *aSameSite)    { *aSameSite = SameSite();   return NS_OK; }
=======
NS_IMETHODIMP nsCookie::GetName(nsACString& aName) {
  aName = Name();
  return NS_OK;
}
NS_IMETHODIMP nsCookie::GetValue(nsACString& aValue) {
  aValue = Value();
  return NS_OK;
}
NS_IMETHODIMP nsCookie::GetHost(nsACString& aHost) {
  aHost = Host();
  return NS_OK;
}
NS_IMETHODIMP nsCookie::GetRawHost(nsACString& aHost) {
  aHost = RawHost();
  return NS_OK;
}
NS_IMETHODIMP nsCookie::GetPath(nsACString& aPath) {
  aPath = Path();
  return NS_OK;
}
NS_IMETHODIMP nsCookie::GetExpiry(int64_t* aExpiry) {
  *aExpiry = Expiry();
  return NS_OK;
}
NS_IMETHODIMP nsCookie::GetIsSession(bool* aIsSession) {
  *aIsSession = IsSession();
  return NS_OK;
}
NS_IMETHODIMP nsCookie::GetIsDomain(bool* aIsDomain) {
  *aIsDomain = IsDomain();
  return NS_OK;
}
NS_IMETHODIMP nsCookie::GetIsSecure(bool* aIsSecure) {
  *aIsSecure = IsSecure();
  return NS_OK;
}
NS_IMETHODIMP nsCookie::GetIsHttpOnly(bool* aHttpOnly) {
  *aHttpOnly = IsHttpOnly();
  return NS_OK;
}
NS_IMETHODIMP nsCookie::GetCreationTime(int64_t* aCreation) {
  *aCreation = CreationTime();
  return NS_OK;
}
NS_IMETHODIMP nsCookie::GetLastAccessed(int64_t* aTime) {
  *aTime = LastAccessed();
  return NS_OK;
}
NS_IMETHODIMP nsCookie::GetSameSite(int32_t* aSameSite) {
  if (mozilla::StaticPrefs::network_cookie_sameSite_laxByDefault()) {
    *aSameSite = SameSite();
  } else {
    *aSameSite = RawSameSite();
  }
  return NS_OK;
}
>>>>>>> upstream-releases

NS_IMETHODIMP
<<<<<<< HEAD
nsCookie::GetOriginAttributes(JSContext *aCx,
                              JS::MutableHandle<JS::Value> aVal) {
||||||| merged common ancestors
nsCookie::GetOriginAttributes(JSContext *aCx, JS::MutableHandle<JS::Value> aVal)
{
=======
nsCookie::GetOriginAttributes(JSContext* aCx,
                              JS::MutableHandle<JS::Value> aVal) {
>>>>>>> upstream-releases
  if (NS_WARN_IF(!ToJSValue(aCx, mOriginAttributes, aVal))) {
    return NS_ERROR_FAILURE;
  }
  return NS_OK;
}

// compatibility method, for use with the legacy nsICookie interface.
// here, expires == 0 denotes a session cookie.
NS_IMETHODIMP
<<<<<<< HEAD
nsCookie::GetExpires(uint64_t *aExpires) {
||||||| merged common ancestors
nsCookie::GetExpires(uint64_t *aExpires)
{
=======
nsCookie::GetExpires(uint64_t* aExpires) {
>>>>>>> upstream-releases
  if (IsSession()) {
    *aExpires = 0;
  } else {
    *aExpires = Expiry() > 0 ? Expiry() : 1;
  }
  return NS_OK;
}

// static
bool nsCookie::ValidateRawSame(const mozilla::net::CookieStruct& aCookieData) {
  return aCookieData.rawSameSite() == aCookieData.sameSite() ||
         aCookieData.rawSameSite() == nsICookie::SAMESITE_NONE;
}

NS_IMPL_ISUPPORTS(nsCookie, nsICookie)
