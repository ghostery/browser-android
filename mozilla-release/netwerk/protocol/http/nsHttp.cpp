/* -*- Mode: C++; tab-width: 4; indent-tabs-mode: nil; c-basic-offset: 2 -*- */
/* vim:set ts=4 sw=2 sts=2 et cin: */
/* This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/. */

// HttpLog.h should generally be included first
#include "HttpLog.h"

#include "nsHttp.h"
#include "CacheControlParser.h"
#include "PLDHashTable.h"
#include "mozilla/Mutex.h"
#include "mozilla/HashFunctions.h"
#include "mozilla/Preferences.h"
#include "mozilla/Unused.h"
#include "nsCRT.h"
#include "nsHttpRequestHead.h"
#include "nsHttpResponseHead.h"
#include "nsHttpHandler.h"
#include "nsICacheEntry.h"
#include "nsIRequest.h"
#include <errno.h>
#include <functional>

namespace mozilla {
namespace net {

// define storage for all atoms
namespace nsHttp {
#define HTTP_ATOM(_name, _value) nsHttpAtom _name = {_value};
#include "nsHttpAtomList.h"
#undef HTTP_ATOM
}  // namespace nsHttp

// find out how many atoms we have
#define HTTP_ATOM(_name, _value) Unused_##_name,
enum {
#include "nsHttpAtomList.h"
  NUM_HTTP_ATOMS
};
#undef HTTP_ATOM

// we keep a linked list of atoms allocated on the heap for easy clean up when
// the atom table is destroyed.  The structure and value string are allocated
// as one contiguous block.

struct HttpHeapAtom {
<<<<<<< HEAD
  struct HttpHeapAtom *next;
  char value[1];
||||||| merged common ancestors
    struct HttpHeapAtom *next;
    char                 value[1];
=======
  struct HttpHeapAtom* next;
  char value[1];
>>>>>>> upstream-releases
};

<<<<<<< HEAD
static PLDHashTable *sAtomTable;
static struct HttpHeapAtom *sHeapAtoms = nullptr;
static Mutex *sLock = nullptr;
||||||| merged common ancestors
static PLDHashTable        *sAtomTable;
static struct HttpHeapAtom *sHeapAtoms = nullptr;
static Mutex               *sLock = nullptr;
=======
static PLDHashTable* sAtomTable;
static struct HttpHeapAtom* sHeapAtoms = nullptr;
static Mutex* sLock = nullptr;
>>>>>>> upstream-releases

<<<<<<< HEAD
HttpHeapAtom *NewHeapAtom(const char *value) {
  int len = strlen(value);
||||||| merged common ancestors
HttpHeapAtom *
NewHeapAtom(const char *value) {
    int len = strlen(value);
=======
HttpHeapAtom* NewHeapAtom(const char* value) {
  int len = strlen(value);
>>>>>>> upstream-releases

<<<<<<< HEAD
  HttpHeapAtom *a = reinterpret_cast<HttpHeapAtom *>(malloc(sizeof(*a) + len));
  if (!a) return nullptr;
  memcpy(a->value, value, len + 1);
||||||| merged common ancestors
    HttpHeapAtom *a =
        reinterpret_cast<HttpHeapAtom *>(malloc(sizeof(*a) + len));
    if (!a)
        return nullptr;
    memcpy(a->value, value, len + 1);
=======
  HttpHeapAtom* a = reinterpret_cast<HttpHeapAtom*>(malloc(sizeof(*a) + len));
  if (!a) return nullptr;
  memcpy(a->value, value, len + 1);
>>>>>>> upstream-releases

  // add this heap atom to the list of all heap atoms
  a->next = sHeapAtoms;
  sHeapAtoms = a;

  return a;
}

// Hash string ignore case, based on PL_HashString
<<<<<<< HEAD
static PLDHashNumber StringHash(const void *key) {
  PLDHashNumber h = 0;
  for (const char *s = reinterpret_cast<const char *>(key); *s; ++s)
    h = AddToHash(h, nsCRT::ToLower(*s));
  return h;
}

static bool StringCompare(const PLDHashEntryHdr *entry, const void *testKey) {
  const void *entryKey = reinterpret_cast<const PLDHashEntryStub *>(entry)->key;

  return PL_strcasecmp(reinterpret_cast<const char *>(entryKey),
                       reinterpret_cast<const char *>(testKey)) == 0;
}

static const PLDHashTableOps ops = {StringHash, StringCompare,
                                    PLDHashTable::MoveEntryStub,
                                    PLDHashTable::ClearEntryStub, nullptr};
||||||| merged common ancestors
static PLDHashNumber
StringHash(const void *key)
{
    PLDHashNumber h = 0;
    for (const char *s = reinterpret_cast<const char*>(key); *s; ++s)
        h = AddToHash(h, nsCRT::ToLower(*s));
    return h;
}

static bool
StringCompare(const PLDHashEntryHdr *entry, const void *testKey)
{
    const void *entryKey =
            reinterpret_cast<const PLDHashEntryStub *>(entry)->key;

    return PL_strcasecmp(reinterpret_cast<const char *>(entryKey),
                         reinterpret_cast<const char *>(testKey)) == 0;
}

static const PLDHashTableOps ops = {
    StringHash,
    StringCompare,
    PLDHashTable::MoveEntryStub,
    PLDHashTable::ClearEntryStub,
    nullptr
};
=======
static PLDHashNumber StringHash(const void* key) {
  PLDHashNumber h = 0;
  for (const char* s = reinterpret_cast<const char*>(key); *s; ++s)
    h = AddToHash(h, nsCRT::ToLower(*s));
  return h;
}

static bool StringCompare(const PLDHashEntryHdr* entry, const void* testKey) {
  const void* entryKey = reinterpret_cast<const PLDHashEntryStub*>(entry)->key;

  return PL_strcasecmp(reinterpret_cast<const char*>(entryKey),
                       reinterpret_cast<const char*>(testKey)) == 0;
}

static const PLDHashTableOps ops = {StringHash, StringCompare,
                                    PLDHashTable::MoveEntryStub,
                                    PLDHashTable::ClearEntryStub, nullptr};
>>>>>>> upstream-releases

// We put the atoms in a hash table for speedy lookup.. see ResolveAtom.
namespace nsHttp {
nsresult CreateAtomTable() {
  MOZ_ASSERT(!sAtomTable, "atom table already initialized");

  if (!sLock) {
    sLock = new Mutex("nsHttp.sLock");
  }

  // The initial length for this table is a value greater than the number of
  // known atoms (NUM_HTTP_ATOMS) because we expect to encounter a few random
  // headers right off the bat.
  sAtomTable =
      new PLDHashTable(&ops, sizeof(PLDHashEntryStub), NUM_HTTP_ATOMS + 10);

<<<<<<< HEAD
  // fill the table with our known atoms
  const char *const atoms[] = {
||||||| merged common ancestors
    // fill the table with our known atoms
    const char *const atoms[] = {
=======
  // fill the table with our known atoms
  const char* const atoms[] = {
>>>>>>> upstream-releases
#define HTTP_ATOM(_name, _value) _name._val,
#include "nsHttpAtomList.h"
#undef HTTP_ATOM
      nullptr};

<<<<<<< HEAD
  for (int i = 0; atoms[i]; ++i) {
    auto stub =
        static_cast<PLDHashEntryStub *>(sAtomTable->Add(atoms[i], fallible));
    if (!stub) return NS_ERROR_OUT_OF_MEMORY;
||||||| merged common ancestors
    for (int i = 0; atoms[i]; ++i) {
        auto stub = static_cast<PLDHashEntryStub*>
                               (sAtomTable->Add(atoms[i], fallible));
        if (!stub)
            return NS_ERROR_OUT_OF_MEMORY;
=======
  for (int i = 0; atoms[i]; ++i) {
    auto stub =
        static_cast<PLDHashEntryStub*>(sAtomTable->Add(atoms[i], fallible));
    if (!stub) return NS_ERROR_OUT_OF_MEMORY;
>>>>>>> upstream-releases

    MOZ_ASSERT(!stub->key, "duplicate static atom");
    stub->key = atoms[i];
  }

  return NS_OK;
}

void DestroyAtomTable() {
  delete sAtomTable;
  sAtomTable = nullptr;

<<<<<<< HEAD
  while (sHeapAtoms) {
    HttpHeapAtom *next = sHeapAtoms->next;
    free(sHeapAtoms);
    sHeapAtoms = next;
  }
||||||| merged common ancestors
    while (sHeapAtoms) {
        HttpHeapAtom *next = sHeapAtoms->next;
        free(sHeapAtoms);
        sHeapAtoms = next;
    }
=======
  while (sHeapAtoms) {
    HttpHeapAtom* next = sHeapAtoms->next;
    free(sHeapAtoms);
    sHeapAtoms = next;
  }
>>>>>>> upstream-releases

  delete sLock;
  sLock = nullptr;
}

<<<<<<< HEAD
Mutex *GetLock() { return sLock; }
||||||| merged common ancestors
Mutex *
GetLock()
{
    return sLock;
}
=======
Mutex* GetLock() { return sLock; }
>>>>>>> upstream-releases

// this function may be called from multiple threads
<<<<<<< HEAD
nsHttpAtom ResolveAtom(const char *str) {
  nsHttpAtom atom = {nullptr};
||||||| merged common ancestors
nsHttpAtom
ResolveAtom(const char *str)
{
    nsHttpAtom atom = { nullptr };
=======
nsHttpAtom ResolveAtom(const char* str) {
  nsHttpAtom atom = {nullptr};
>>>>>>> upstream-releases

  if (!str || !sAtomTable) return atom;

  MutexAutoLock lock(*sLock);

<<<<<<< HEAD
  auto stub = static_cast<PLDHashEntryStub *>(sAtomTable->Add(str, fallible));
  if (!stub) return atom;  // out of memory
||||||| merged common ancestors
    auto stub = static_cast<PLDHashEntryStub*>(sAtomTable->Add(str, fallible));
    if (!stub)
        return atom;  // out of memory
=======
  auto stub = static_cast<PLDHashEntryStub*>(sAtomTable->Add(str, fallible));
  if (!stub) return atom;  // out of memory
>>>>>>> upstream-releases

<<<<<<< HEAD
  if (stub->key) {
    atom._val = reinterpret_cast<const char *>(stub->key);
    return atom;
  }
||||||| merged common ancestors
    if (stub->key) {
        atom._val = reinterpret_cast<const char *>(stub->key);
        return atom;
    }
=======
  if (stub->key) {
    atom._val = reinterpret_cast<const char*>(stub->key);
    return atom;
  }
>>>>>>> upstream-releases

<<<<<<< HEAD
  // if the atom could not be found in the atom table, then we'll go
  // and allocate a new atom on the heap.
  HttpHeapAtom *heapAtom = NewHeapAtom(str);
  if (!heapAtom) return atom;  // out of memory
||||||| merged common ancestors
    // if the atom could not be found in the atom table, then we'll go
    // and allocate a new atom on the heap.
    HttpHeapAtom *heapAtom = NewHeapAtom(str);
    if (!heapAtom)
        return atom;  // out of memory
=======
  // if the atom could not be found in the atom table, then we'll go
  // and allocate a new atom on the heap.
  HttpHeapAtom* heapAtom = NewHeapAtom(str);
  if (!heapAtom) return atom;  // out of memory
>>>>>>> upstream-releases

  stub->key = atom._val = heapAtom->value;
  return atom;
}

//
// From section 2.2 of RFC 2616, a token is defined as:
//
//   token          = 1*<any CHAR except CTLs or separators>
//   CHAR           = <any US-ASCII character (octets 0 - 127)>
//   separators     = "(" | ")" | "<" | ">" | "@"
//                  | "," | ";" | ":" | "\" | <">
//                  | "/" | "[" | "]" | "?" | "="
//                  | "{" | "}" | SP | HT
//   CTL            = <any US-ASCII control character
//                    (octets 0 - 31) and DEL (127)>
//   SP             = <US-ASCII SP, space (32)>
//   HT             = <US-ASCII HT, horizontal-tab (9)>
//
static const char kValidTokenMap[128] = {
    0, 0, 0, 0, 0, 0, 0, 0,  //   0
    0, 0, 0, 0, 0, 0, 0, 0,  //   8
    0, 0, 0, 0, 0, 0, 0, 0,  //  16
    0, 0, 0, 0, 0, 0, 0, 0,  //  24

    0, 1, 0, 1, 1, 1, 1, 1,  //  32
    0, 0, 1, 1, 0, 1, 1, 0,  //  40
    1, 1, 1, 1, 1, 1, 1, 1,  //  48
    1, 1, 0, 0, 0, 0, 0, 0,  //  56

    0, 1, 1, 1, 1, 1, 1, 1,  //  64
    1, 1, 1, 1, 1, 1, 1, 1,  //  72
    1, 1, 1, 1, 1, 1, 1, 1,  //  80
    1, 1, 1, 0, 0, 0, 1, 1,  //  88

    1, 1, 1, 1, 1, 1, 1, 1,  //  96
    1, 1, 1, 1, 1, 1, 1, 1,  // 104
    1, 1, 1, 1, 1, 1, 1, 1,  // 112
    1, 1, 1, 0, 1, 0, 1, 0   // 120
};
<<<<<<< HEAD
bool IsValidToken(const char *start, const char *end) {
  if (start == end) return false;
||||||| merged common ancestors
bool
IsValidToken(const char *start, const char *end)
{
    if (start == end)
        return false;

    for (; start != end; ++start) {
        const unsigned char idx = *start;
        if (idx > 127 || !kValidTokenMap[idx])
            return false;
    }
=======
bool IsValidToken(const char* start, const char* end) {
  if (start == end) return false;
>>>>>>> upstream-releases

  for (; start != end; ++start) {
    const unsigned char idx = *start;
    if (idx > 127 || !kValidTokenMap[idx]) return false;
  }

  return true;
}

<<<<<<< HEAD
const char *GetProtocolVersion(HttpVersion pv) {
  switch (pv) {
||||||| merged common ancestors
const char*
GetProtocolVersion(HttpVersion pv)
{
    switch (pv) {
=======
const char* GetProtocolVersion(HttpVersion pv) {
  switch (pv) {
>>>>>>> upstream-releases
    case HttpVersion::v2_0:
      return "h2";
    case HttpVersion::v1_0:
      return "http/1.0";
    case HttpVersion::v1_1:
      return "http/1.1";
    default:
      NS_WARNING(nsPrintfCString("Unkown protocol version: 0x%X. "
                                 "Please file a bug",
                                 static_cast<uint32_t>(pv))
                     .get());
      return "http/1.1";
  }
}

// static
<<<<<<< HEAD
void TrimHTTPWhitespace(const nsACString &aSource, nsACString &aDest) {
||||||| merged common ancestors
void
TrimHTTPWhitespace(const nsACString& aSource, nsACString& aDest)
{
=======
void TrimHTTPWhitespace(const nsACString& aSource, nsACString& aDest) {
>>>>>>> upstream-releases
  nsAutoCString str(aSource);

  // HTTP whitespace 0x09: '\t', 0x0A: '\n', 0x0D: '\r', 0x20: ' '
  static const char kHTTPWhitespace[] = "\t\n\r ";
  str.Trim(kHTTPWhitespace);
  aDest.Assign(str);
}

// static
<<<<<<< HEAD
bool IsReasonableHeaderValue(const nsACString &s) {
||||||| merged common ancestors
bool
IsReasonableHeaderValue(const nsACString &s)
{
=======
bool IsReasonableHeaderValue(const nsACString& s) {
>>>>>>> upstream-releases
  // Header values MUST NOT contain line-breaks.  RFC 2616 technically
  // permits CTL characters, including CR and LF, in header values provided
  // they are quoted.  However, this can lead to problems if servers do not
  // interpret quoted strings properly.  Disallowing CR and LF here seems
  // reasonable and keeps things simple.  We also disallow a null byte.
  const nsACString::char_type *end = s.EndReading();
  for (const nsACString::char_type *i = s.BeginReading(); i != end; ++i) {
    if (*i == '\r' || *i == '\n' || *i == '\0') {
      return false;
    }
  }
  return true;
}

<<<<<<< HEAD
const char *FindToken(const char *input, const char *token, const char *seps) {
  if (!input) return nullptr;
||||||| merged common ancestors
const char *
FindToken(const char *input, const char *token, const char *seps)
{
    if (!input)
        return nullptr;
=======
const char* FindToken(const char* input, const char* token, const char* seps) {
  if (!input) return nullptr;
>>>>>>> upstream-releases

  int inputLen = strlen(input);
  int tokenLen = strlen(token);

  if (inputLen < tokenLen) return nullptr;

<<<<<<< HEAD
  const char *inputTop = input;
  const char *inputEnd = input + inputLen - tokenLen;
  for (; input <= inputEnd; ++input) {
    if (PL_strncasecmp(input, token, tokenLen) == 0) {
      if (input > inputTop && !strchr(seps, *(input - 1))) continue;
      if (input < inputEnd && !strchr(seps, *(input + tokenLen))) continue;
      return input;
||||||| merged common ancestors
    const char *inputTop = input;
    const char *inputEnd = input + inputLen - tokenLen;
    for (; input <= inputEnd; ++input) {
        if (PL_strncasecmp(input, token, tokenLen) == 0) {
            if (input > inputTop && !strchr(seps, *(input - 1)))
                continue;
            if (input < inputEnd && !strchr(seps, *(input + tokenLen)))
                continue;
            return input;
        }
=======
  const char* inputTop = input;
  const char* inputEnd = input + inputLen - tokenLen;
  for (; input <= inputEnd; ++input) {
    if (PL_strncasecmp(input, token, tokenLen) == 0) {
      if (input > inputTop && !strchr(seps, *(input - 1))) continue;
      if (input < inputEnd && !strchr(seps, *(input + tokenLen))) continue;
      return input;
>>>>>>> upstream-releases
    }
  }

  return nullptr;
}

<<<<<<< HEAD
bool ParseInt64(const char *input, const char **next, int64_t *r) {
  MOZ_ASSERT(input);
  MOZ_ASSERT(r);
||||||| merged common ancestors
bool
ParseInt64(const char *input, const char **next, int64_t *r)
{
    MOZ_ASSERT(input);
    MOZ_ASSERT(r);
=======
bool ParseInt64(const char* input, const char** next, int64_t* r) {
  MOZ_ASSERT(input);
  MOZ_ASSERT(r);
>>>>>>> upstream-releases

<<<<<<< HEAD
  char *end = nullptr;
  errno = 0;  // Clear errno to make sure its value is set by strtoll
  int64_t value = strtoll(input, &end, /* base */ 10);
||||||| merged common ancestors
    char *end = nullptr;
    errno = 0; // Clear errno to make sure its value is set by strtoll
    int64_t value = strtoll(input, &end, /* base */ 10);
=======
  char* end = nullptr;
  errno = 0;  // Clear errno to make sure its value is set by strtoll
  int64_t value = strtoll(input, &end, /* base */ 10);
>>>>>>> upstream-releases

  // Fail if: - the parsed number overflows.
  //          - the end points to the start of the input string.
  //          - we parsed a negative value. Consumers don't expect that.
  if (errno != 0 || end == input || value < 0) {
    LOG(("nsHttp::ParseInt64 value=%" PRId64 " errno=%d", value, errno));
    return false;
  }

  if (next) {
    *next = end;
  }
  *r = value;
  return true;
}

bool IsPermanentRedirect(uint32_t httpStatus) {
  return httpStatus == 301 || httpStatus == 308;
}

<<<<<<< HEAD
bool ValidationRequired(bool isForcedValid,
                        nsHttpResponseHead *cachedResponseHead,
                        uint32_t loadFlags, bool allowStaleCacheContent,
                        bool isImmutable, bool customConditionalRequest,
                        nsHttpRequestHead &requestHead, nsICacheEntry *entry,
                        CacheControlParser &cacheControlRequest,
                        bool fromPreviousSession) {
  // Check isForcedValid to see if it is possible to skip validation.
  // Don't skip validation if we have serious reason to believe that this
  // content is invalid (it's expired).
  // See netwerk/cache2/nsICacheEntry.idl for details
  if (isForcedValid && (!cachedResponseHead->ExpiresInPast() ||
                        !cachedResponseHead->MustValidateIfExpired())) {
    LOG(("NOT validating based on isForcedValid being true.\n"));
    return false;
  }
||||||| merged common ancestors
bool
ValidationRequired(bool isForcedValid, nsHttpResponseHead *cachedResponseHead,
                   uint32_t loadFlags, bool allowStaleCacheContent,
                   bool isImmutable, bool customConditionalRequest,
                   nsHttpRequestHead &requestHead,
                   nsICacheEntry *entry, CacheControlParser &cacheControlRequest,
                   bool fromPreviousSession)
{
    // Check isForcedValid to see if it is possible to skip validation.
    // Don't skip validation if we have serious reason to believe that this
    // content is invalid (it's expired).
    // See netwerk/cache2/nsICacheEntry.idl for details
    if (isForcedValid &&
        (!cachedResponseHead->ExpiresInPast() ||
         !cachedResponseHead->MustValidateIfExpired())) {
        LOG(("NOT validating based on isForcedValid being true.\n"));
        return false;
    }
=======
bool ValidationRequired(bool isForcedValid,
                        nsHttpResponseHead* cachedResponseHead,
                        uint32_t loadFlags, bool allowStaleCacheContent,
                        bool isImmutable, bool customConditionalRequest,
                        nsHttpRequestHead& requestHead, nsICacheEntry* entry,
                        CacheControlParser& cacheControlRequest,
                        bool fromPreviousSession,
                        bool* performBackgroundRevalidation) {
  if (performBackgroundRevalidation) {
    *performBackgroundRevalidation = false;
  }
>>>>>>> upstream-releases

<<<<<<< HEAD
  // If the LOAD_FROM_CACHE flag is set, any cached data can simply be used
  if (loadFlags & nsIRequest::LOAD_FROM_CACHE || allowStaleCacheContent) {
    LOG(("NOT validating based on LOAD_FROM_CACHE load flag\n"));
    return false;
  }
||||||| merged common ancestors
    // If the LOAD_FROM_CACHE flag is set, any cached data can simply be used
    if (loadFlags & nsIRequest::LOAD_FROM_CACHE || allowStaleCacheContent) {
        LOG(("NOT validating based on LOAD_FROM_CACHE load flag\n"));
        return false;
    }
=======
  // Check isForcedValid to see if it is possible to skip validation.
  // Don't skip validation if we have serious reason to believe that this
  // content is invalid (it's expired).
  // See netwerk/cache2/nsICacheEntry.idl for details
  if (isForcedValid && (!cachedResponseHead->ExpiresInPast() ||
                        !cachedResponseHead->MustValidateIfExpired())) {
    LOG(("NOT validating based on isForcedValid being true.\n"));
    return false;
  }
>>>>>>> upstream-releases

<<<<<<< HEAD
  // If the VALIDATE_ALWAYS flag is set, any cached data won't be used until
  // it's revalidated with the server.
  if ((loadFlags & nsIRequest::VALIDATE_ALWAYS) && !isImmutable) {
    LOG(("Validating based on VALIDATE_ALWAYS load flag\n"));
    return true;
  }
||||||| merged common ancestors
    // If the VALIDATE_ALWAYS flag is set, any cached data won't be used until
    // it's revalidated with the server.
    if ((loadFlags & nsIRequest::VALIDATE_ALWAYS) && !isImmutable) {
        LOG(("Validating based on VALIDATE_ALWAYS load flag\n"));
        return true;
    }
=======
  // If the LOAD_FROM_CACHE flag is set, any cached data can simply be used
  if (loadFlags & nsIRequest::LOAD_FROM_CACHE || allowStaleCacheContent) {
    LOG(("NOT validating based on LOAD_FROM_CACHE load flag\n"));
    return false;
  }
>>>>>>> upstream-releases

<<<<<<< HEAD
  // Even if the VALIDATE_NEVER flag is set, there are still some cases in
  // which we must validate the cached response with the server.
  if (loadFlags & nsIRequest::VALIDATE_NEVER) {
    LOG(("VALIDATE_NEVER set\n"));
    // if no-store validate cached response (see bug 112564)
    if (cachedResponseHead->NoStore()) {
      LOG(("Validating based on no-store logic\n"));
      return true;
    }
    LOG(("NOT validating based on VALIDATE_NEVER load flag\n"));
    return false;
  }
||||||| merged common ancestors
    // Even if the VALIDATE_NEVER flag is set, there are still some cases in
    // which we must validate the cached response with the server.
    if (loadFlags & nsIRequest::VALIDATE_NEVER) {
        LOG(("VALIDATE_NEVER set\n"));
        // if no-store validate cached response (see bug 112564)
        if (cachedResponseHead->NoStore()) {
            LOG(("Validating based on no-store logic\n"));
            return true;
        }
        LOG(("NOT validating based on VALIDATE_NEVER load flag\n"));
        return false;
    }
=======
  // If the VALIDATE_ALWAYS flag is set, any cached data won't be used until
  // it's revalidated with the server.
  if ((loadFlags & nsIRequest::VALIDATE_ALWAYS) && !isImmutable) {
    LOG(("Validating based on VALIDATE_ALWAYS load flag\n"));
    return true;
  }
>>>>>>> upstream-releases

<<<<<<< HEAD
  // check if validation is strictly required...
  if (cachedResponseHead->MustValidate()) {
    LOG(("Validating based on MustValidate() returning TRUE\n"));
    return true;
  }
||||||| merged common ancestors
    // check if validation is strictly required...
    if (cachedResponseHead->MustValidate()) {
        LOG(("Validating based on MustValidate() returning TRUE\n"));
        return true;
    }
=======
  // Even if the VALIDATE_NEVER flag is set, there are still some cases in
  // which we must validate the cached response with the server.
  if (loadFlags & nsIRequest::VALIDATE_NEVER) {
    LOG(("VALIDATE_NEVER set\n"));
    // if no-store validate cached response (see bug 112564)
    if (cachedResponseHead->NoStore()) {
      LOG(("Validating based on no-store logic\n"));
      return true;
    }
    LOG(("NOT validating based on VALIDATE_NEVER load flag\n"));
    return false;
  }
>>>>>>> upstream-releases

<<<<<<< HEAD
  // possibly serve from cache for a custom If-Match/If-Unmodified-Since
  // conditional request
  if (customConditionalRequest && !requestHead.HasHeader(nsHttp::If_Match) &&
      !requestHead.HasHeader(nsHttp::If_Unmodified_Since)) {
    LOG(("Validating based on a custom conditional request\n"));
    return true;
  }
||||||| merged common ancestors
    // possibly serve from cache for a custom If-Match/If-Unmodified-Since
    // conditional request
    if (customConditionalRequest &&
        !requestHead.HasHeader(nsHttp::If_Match) &&
        !requestHead.HasHeader(nsHttp::If_Unmodified_Since)) {
        LOG(("Validating based on a custom conditional request\n"));
        return true;
    }
=======
  // check if validation is strictly required...
  if (cachedResponseHead->MustValidate()) {
    LOG(("Validating based on MustValidate() returning TRUE\n"));
    return true;
  }
>>>>>>> upstream-releases

<<<<<<< HEAD
  // previously we also checked for a query-url w/out expiration
  // and didn't do heuristic on it. but defacto that is allowed now.
  //
  // Check if the cache entry has expired...
||||||| merged common ancestors
    // previously we also checked for a query-url w/out expiration
    // and didn't do heuristic on it. but defacto that is allowed now.
    //
    // Check if the cache entry has expired...
=======
  // possibly serve from cache for a custom If-Match/If-Unmodified-Since
  // conditional request
  if (customConditionalRequest && !requestHead.HasHeader(nsHttp::If_Match) &&
      !requestHead.HasHeader(nsHttp::If_Unmodified_Since)) {
    LOG(("Validating based on a custom conditional request\n"));
    return true;
  }
>>>>>>> upstream-releases

<<<<<<< HEAD
  bool doValidation = true;
  uint32_t now = NowInSeconds();
||||||| merged common ancestors
    bool doValidation = true;
    uint32_t now = NowInSeconds();
=======
  // previously we also checked for a query-url w/out expiration
  // and didn't do heuristic on it. but defacto that is allowed now.
  //
  // Check if the cache entry has expired...
>>>>>>> upstream-releases

<<<<<<< HEAD
  uint32_t age = 0;
  nsresult rv = cachedResponseHead->ComputeCurrentAge(now, now, &age);
  if (NS_FAILED(rv)) {
    return true;
  }
||||||| merged common ancestors
    uint32_t age = 0;
    nsresult rv = cachedResponseHead->ComputeCurrentAge(now, now, &age);
    if (NS_FAILED(rv)) {
        return true;
    }
=======
  bool doValidation = true;
  uint32_t now = NowInSeconds();
>>>>>>> upstream-releases

<<<<<<< HEAD
  uint32_t freshness = 0;
  rv = cachedResponseHead->ComputeFreshnessLifetime(&freshness);
  if (NS_FAILED(rv)) {
    return true;
  }
||||||| merged common ancestors
    uint32_t freshness = 0;
    rv = cachedResponseHead->ComputeFreshnessLifetime(&freshness);
    if (NS_FAILED(rv)) {
        return true;
    }
=======
  uint32_t age = 0;
  nsresult rv = cachedResponseHead->ComputeCurrentAge(now, now, &age);
  if (NS_FAILED(rv)) {
    return true;
  }
>>>>>>> upstream-releases

<<<<<<< HEAD
  uint32_t expiration = 0;
  rv = entry->GetExpirationTime(&expiration);
  if (NS_FAILED(rv)) {
    return true;
  }
||||||| merged common ancestors
    uint32_t expiration = 0;
    rv = entry->GetExpirationTime(&expiration);
    if (NS_FAILED(rv)) {
        return true;
    }
=======
  uint32_t freshness = 0;
  rv = cachedResponseHead->ComputeFreshnessLifetime(&freshness);
  if (NS_FAILED(rv)) {
    return true;
  }
>>>>>>> upstream-releases

<<<<<<< HEAD
  uint32_t maxAgeRequest, maxStaleRequest, minFreshRequest;

  LOG(("  NowInSeconds()=%u, expiration time=%u, freshness lifetime=%u, age=%u",
       now, expiration, freshness, age));

  if (cacheControlRequest.NoCache()) {
    LOG(("  validating, no-cache request"));
    doValidation = true;
  } else if (cacheControlRequest.MaxStale(&maxStaleRequest)) {
    uint32_t staleTime = age > freshness ? age - freshness : 0;
    doValidation = staleTime > maxStaleRequest;
    LOG(("  validating=%d, max-stale=%u requested", doValidation,
         maxStaleRequest));
  } else if (cacheControlRequest.MaxAge(&maxAgeRequest)) {
    // The input information for age and freshness calculation are in seconds.
    // Hence, the internal logic can't have better resolution than seconds too.
    // To make max-age=0 case work even for requests made in less than a second
    // after the last response has been received, we use >= for compare.  This
    // is correct because of the rounding down of the age calculated value.
    doValidation = age >= maxAgeRequest;
    LOG(("  validating=%d, max-age=%u requested", doValidation, maxAgeRequest));
  } else if (cacheControlRequest.MinFresh(&minFreshRequest)) {
    uint32_t freshTime = freshness > age ? freshness - age : 0;
    doValidation = freshTime < minFreshRequest;
    LOG(("  validating=%d, min-fresh=%u requested", doValidation,
         minFreshRequest));
  } else if (now < expiration) {
    doValidation = false;
    LOG(("  not validating, expire time not in the past"));
  } else if (cachedResponseHead->MustValidateIfExpired()) {
    doValidation = true;
  } else if (loadFlags & nsIRequest::VALIDATE_ONCE_PER_SESSION) {
    // If the cached response does not include expiration infor-
    // mation, then we must validate the response, despite whether
    // or not this is the first access this session.  This behavior
    // is consistent with existing browsers and is generally expected
    // by web authors.
    if (freshness == 0)
      doValidation = true;
    else
      doValidation = fromPreviousSession;
  } else {
    doValidation = true;
  }
||||||| merged common ancestors
    uint32_t maxAgeRequest, maxStaleRequest, minFreshRequest;

    LOG(("  NowInSeconds()=%u, expiration time=%u, freshness lifetime=%u, age=%u",
            now, expiration, freshness, age));

    if (cacheControlRequest.NoCache()) {
        LOG(("  validating, no-cache request"));
        doValidation = true;
    } else if (cacheControlRequest.MaxStale(&maxStaleRequest)) {
        uint32_t staleTime = age > freshness ? age - freshness : 0;
        doValidation = staleTime > maxStaleRequest;
        LOG(("  validating=%d, max-stale=%u requested", doValidation, maxStaleRequest));
    } else if (cacheControlRequest.MaxAge(&maxAgeRequest)) {
        // The input information for age and freshness calculation are in seconds.
        // Hence, the internal logic can't have better resolution than seconds too.
        // To make max-age=0 case work even for requests made in less than a second
        // after the last response has been received, we use >= for compare.  This
        // is correct because of the rounding down of the age calculated value.
        doValidation = age >= maxAgeRequest;
        LOG(("  validating=%d, max-age=%u requested", doValidation, maxAgeRequest));
    } else if (cacheControlRequest.MinFresh(&minFreshRequest)) {
        uint32_t freshTime = freshness > age ? freshness - age : 0;
        doValidation = freshTime < minFreshRequest;
        LOG(("  validating=%d, min-fresh=%u requested", doValidation, minFreshRequest));
    } else if (now < expiration) {
        doValidation = false;
        LOG(("  not validating, expire time not in the past"));
    } else if (cachedResponseHead->MustValidateIfExpired()) {
        doValidation = true;
    } else if (loadFlags & nsIRequest::VALIDATE_ONCE_PER_SESSION) {
        // If the cached response does not include expiration infor-
        // mation, then we must validate the response, despite whether
        // or not this is the first access this session.  This behavior
        // is consistent with existing browsers and is generally expected
        // by web authors.
        if (freshness == 0)
            doValidation = true;
        else
            doValidation = fromPreviousSession;
    } else {
        doValidation = true;
    }
=======
  uint32_t expiration = 0;
  rv = entry->GetExpirationTime(&expiration);
  if (NS_FAILED(rv)) {
    return true;
  }
>>>>>>> upstream-releases

<<<<<<< HEAD
  LOG(("%salidating based on expiration time\n", doValidation ? "V" : "Not v"));
  return doValidation;
}

nsresult GetHttpResponseHeadFromCacheEntry(
    nsICacheEntry *entry, nsHttpResponseHead *cachedResponseHead) {
  nsCString buf;
  // A "original-response-headers" metadata element holds network original
  // headers, i.e. the headers in the form as they arrieved from the network. We
  // need to get the network original headers first, because we need to keep
  // them in order.
  nsresult rv = entry->GetMetaDataElement("original-response-headers",
                                          getter_Copies(buf));
  if (NS_SUCCEEDED(rv)) {
    rv = cachedResponseHead->ParseCachedOriginalHeaders((char *)buf.get());
    if (NS_FAILED(rv)) {
      LOG(("  failed to parse original-response-headers\n"));
||||||| merged common ancestors
    LOG(("%salidating based on expiration time\n", doValidation ? "V" : "Not v"));
    return doValidation;
}

nsresult
GetHttpResponseHeadFromCacheEntry(nsICacheEntry *entry, nsHttpResponseHead *cachedResponseHead)
{
    nsCString buf;
    // A "original-response-headers" metadata element holds network original headers,
    // i.e. the headers in the form as they arrieved from the network.
    // We need to get the network original headers first, because we need to keep them
    // in order.
    nsresult rv = entry->GetMetaDataElement("original-response-headers", getter_Copies(buf));
    if (NS_SUCCEEDED(rv)) {
        rv = cachedResponseHead->ParseCachedOriginalHeaders((char *) buf.get());
        if (NS_FAILED(rv)) {
            LOG(("  failed to parse original-response-headers\n"));
        }
=======
  uint32_t maxAgeRequest, maxStaleRequest, minFreshRequest;

  LOG(("  NowInSeconds()=%u, expiration time=%u, freshness lifetime=%u, age=%u",
       now, expiration, freshness, age));

  if (cacheControlRequest.NoCache()) {
    LOG(("  validating, no-cache request"));
    doValidation = true;
  } else if (cacheControlRequest.MaxStale(&maxStaleRequest)) {
    uint32_t staleTime = age > freshness ? age - freshness : 0;
    doValidation = staleTime > maxStaleRequest;
    LOG(("  validating=%d, max-stale=%u requested", doValidation,
         maxStaleRequest));
  } else if (cacheControlRequest.MaxAge(&maxAgeRequest)) {
    // The input information for age and freshness calculation are in seconds.
    // Hence, the internal logic can't have better resolution than seconds too.
    // To make max-age=0 case work even for requests made in less than a second
    // after the last response has been received, we use >= for compare.  This
    // is correct because of the rounding down of the age calculated value.
    doValidation = age >= maxAgeRequest;
    LOG(("  validating=%d, max-age=%u requested", doValidation, maxAgeRequest));
  } else if (cacheControlRequest.MinFresh(&minFreshRequest)) {
    uint32_t freshTime = freshness > age ? freshness - age : 0;
    doValidation = freshTime < minFreshRequest;
    LOG(("  validating=%d, min-fresh=%u requested", doValidation,
         minFreshRequest));
  } else if (now < expiration) {
    doValidation = false;
    LOG(("  not validating, expire time not in the past"));
  } else if (cachedResponseHead->MustValidateIfExpired()) {
    doValidation = true;
  } else if (cachedResponseHead->StaleWhileRevalidate(now, expiration)) {
    LOG(("  not validating, in the stall-while-revalidate window"));
    doValidation = false;
    if (performBackgroundRevalidation) {
      *performBackgroundRevalidation = true;
    }
  } else if (loadFlags & nsIRequest::VALIDATE_ONCE_PER_SESSION) {
    // If the cached response does not include expiration infor-
    // mation, then we must validate the response, despite whether
    // or not this is the first access this session.  This behavior
    // is consistent with existing browsers and is generally expected
    // by web authors.
    if (freshness == 0)
      doValidation = true;
    else
      doValidation = fromPreviousSession;
  } else {
    doValidation = true;
  }

  LOG(("%salidating based on expiration time\n", doValidation ? "V" : "Not v"));
  return doValidation;
}

nsresult GetHttpResponseHeadFromCacheEntry(
    nsICacheEntry* entry, nsHttpResponseHead* cachedResponseHead) {
  nsCString buf;
  // A "original-response-headers" metadata element holds network original
  // headers, i.e. the headers in the form as they arrieved from the network. We
  // need to get the network original headers first, because we need to keep
  // them in order.
  nsresult rv = entry->GetMetaDataElement("original-response-headers",
                                          getter_Copies(buf));
  if (NS_SUCCEEDED(rv)) {
    rv = cachedResponseHead->ParseCachedOriginalHeaders((char*)buf.get());
    if (NS_FAILED(rv)) {
      LOG(("  failed to parse original-response-headers\n"));
>>>>>>> upstream-releases
    }
  }

  buf.Adopt(nullptr);
  // A "response-head" metadata element holds response head, e.g. response
  // status line and headers in the form Firefox uses them internally (no
  // dupicate headers, etc.).
  rv = entry->GetMetaDataElement("response-head", getter_Copies(buf));
  NS_ENSURE_SUCCESS(rv, rv);

  // Parse string stored in a "response-head" metadata element.
  // These response headers will be merged with the orignal headers (i.e. the
  // headers stored in a "original-response-headers" metadata element).
  rv = cachedResponseHead->ParseCachedHead(buf.get());
  NS_ENSURE_SUCCESS(rv, rv);
  buf.Adopt(nullptr);

  return NS_OK;
}

<<<<<<< HEAD
nsresult CheckPartial(nsICacheEntry *aEntry, int64_t *aSize,
                      int64_t *aContentLength,
                      nsHttpResponseHead *responseHead) {
  nsresult rv;
||||||| merged common ancestors
nsresult
CheckPartial(nsICacheEntry* aEntry, int64_t *aSize, int64_t *aContentLength,
             nsHttpResponseHead *responseHead)
{
    nsresult rv;
=======
nsresult CheckPartial(nsICacheEntry* aEntry, int64_t* aSize,
                      int64_t* aContentLength,
                      nsHttpResponseHead* responseHead) {
  nsresult rv;
>>>>>>> upstream-releases

  rv = aEntry->GetDataSize(aSize);

  if (NS_ERROR_IN_PROGRESS == rv) {
    *aSize = -1;
    rv = NS_OK;
  }

  NS_ENSURE_SUCCESS(rv, rv);

  if (!responseHead) {
    return NS_ERROR_UNEXPECTED;
  }

  *aContentLength = responseHead->ContentLength();

  return NS_OK;
}

<<<<<<< HEAD
void DetermineFramingAndImmutability(nsICacheEntry *entry,
                                     nsHttpResponseHead *responseHead,
                                     bool isHttps, bool *weaklyFramed,
                                     bool *isImmutable) {
  nsCString framedBuf;
  nsresult rv =
      entry->GetMetaDataElement("strongly-framed", getter_Copies(framedBuf));
  // describe this in terms of explicitly weakly framed so as to be backwards
  // compatible with old cache contents which dont have strongly-framed makers
  *weaklyFramed = NS_SUCCEEDED(rv) && framedBuf.EqualsLiteral("0");
  *isImmutable = !*weaklyFramed && isHttps && responseHead->Immutable();
||||||| merged common ancestors
void
DetermineFramingAndImmutability(nsICacheEntry *entry,
        nsHttpResponseHead *responseHead, bool isHttps,
        bool *weaklyFramed, bool *isImmutable)
{
    nsCString framedBuf;
    nsresult rv = entry->GetMetaDataElement("strongly-framed", getter_Copies(framedBuf));
    // describe this in terms of explicitly weakly framed so as to be backwards
    // compatible with old cache contents which dont have strongly-framed makers
    *weaklyFramed = NS_SUCCEEDED(rv) && framedBuf.EqualsLiteral("0");
    *isImmutable = !*weaklyFramed && isHttps && responseHead->Immutable();
=======
void DetermineFramingAndImmutability(nsICacheEntry* entry,
                                     nsHttpResponseHead* responseHead,
                                     bool isHttps, bool* weaklyFramed,
                                     bool* isImmutable) {
  nsCString framedBuf;
  nsresult rv =
      entry->GetMetaDataElement("strongly-framed", getter_Copies(framedBuf));
  // describe this in terms of explicitly weakly framed so as to be backwards
  // compatible with old cache contents which dont have strongly-framed makers
  *weaklyFramed = NS_SUCCEEDED(rv) && framedBuf.EqualsLiteral("0");
  *isImmutable = !*weaklyFramed && isHttps && responseHead->Immutable();
>>>>>>> upstream-releases
}

<<<<<<< HEAD
bool IsBeforeLastActiveTabLoadOptimization(TimeStamp const &when) {
  return gHttpHandler &&
         gHttpHandler->IsBeforeLastActiveTabLoadOptimization(when);
||||||| merged common ancestors
bool
IsBeforeLastActiveTabLoadOptimization(TimeStamp const & when)
{
  return gHttpHandler && gHttpHandler->IsBeforeLastActiveTabLoadOptimization(when);
=======
bool IsBeforeLastActiveTabLoadOptimization(TimeStamp const& when) {
  return gHttpHandler &&
         gHttpHandler->IsBeforeLastActiveTabLoadOptimization(when);
>>>>>>> upstream-releases
}

void NotifyActiveTabLoadOptimization() {
  if (gHttpHandler) {
    gHttpHandler->NotifyActiveTabLoadOptimization();
  }
}

TimeStamp const GetLastActiveTabLoadOptimizationHit() {
  return gHttpHandler ? gHttpHandler->GetLastActiveTabLoadOptimizationHit()
                      : TimeStamp();
}

<<<<<<< HEAD
void SetLastActiveTabLoadOptimizationHit(TimeStamp const &when) {
||||||| merged common ancestors
void
SetLastActiveTabLoadOptimizationHit(TimeStamp const &when)
{
=======
void SetLastActiveTabLoadOptimizationHit(TimeStamp const& when) {
>>>>>>> upstream-releases
  if (gHttpHandler) {
    gHttpHandler->SetLastActiveTabLoadOptimizationHit(when);
  }
}

HttpVersion GetHttpVersionFromSpdy(SpdyVersion sv) {
  MOZ_DIAGNOSTIC_ASSERT(sv != SpdyVersion::NONE);
  MOZ_ASSERT(sv == SpdyVersion::HTTP_2);

  return HttpVersion::v2_0;
}

}  // namespace nsHttp

<<<<<<< HEAD
template <typename T>
void localEnsureBuffer(UniquePtr<T[]> &buf, uint32_t newSize, uint32_t preserve,
                       uint32_t &objSize) {
  if (objSize >= newSize) return;
||||||| merged common ancestors

template<typename T> void
localEnsureBuffer(UniquePtr<T[]> &buf, uint32_t newSize,
             uint32_t preserve, uint32_t &objSize)
{
  if (objSize >= newSize)
    return;
=======
template <typename T>
void localEnsureBuffer(UniquePtr<T[]>& buf, uint32_t newSize, uint32_t preserve,
                       uint32_t& objSize) {
  if (objSize >= newSize) return;
>>>>>>> upstream-releases

  // Leave a little slop on the new allocation - add 2KB to
  // what we need and then round the result up to a 4KB (page)
  // boundary.

  objSize = (newSize + 2048 + 4095) & ~4095;

  static_assert(sizeof(T) == 1, "sizeof(T) must be 1");
  auto tmp = MakeUnique<T[]>(objSize);
  if (preserve) {
    memcpy(tmp.get(), buf.get(), preserve);
  }
  buf = std::move(tmp);
}

<<<<<<< HEAD
void EnsureBuffer(UniquePtr<char[]> &buf, uint32_t newSize, uint32_t preserve,
                  uint32_t &objSize) {
  localEnsureBuffer<char>(buf, newSize, preserve, objSize);
||||||| merged common ancestors
void EnsureBuffer(UniquePtr<char[]> &buf, uint32_t newSize,
                  uint32_t preserve, uint32_t &objSize)
{
    localEnsureBuffer<char> (buf, newSize, preserve, objSize);
=======
void EnsureBuffer(UniquePtr<char[]>& buf, uint32_t newSize, uint32_t preserve,
                  uint32_t& objSize) {
  localEnsureBuffer<char>(buf, newSize, preserve, objSize);
>>>>>>> upstream-releases
}

<<<<<<< HEAD
void EnsureBuffer(UniquePtr<uint8_t[]> &buf, uint32_t newSize,
                  uint32_t preserve, uint32_t &objSize) {
  localEnsureBuffer<uint8_t>(buf, newSize, preserve, objSize);
}

static bool IsTokenSymbol(signed char chr) {
  if (chr < 33 || chr == 127 || chr == '(' || chr == ')' || chr == '<' ||
      chr == '>' || chr == '@' || chr == ',' || chr == ';' || chr == ':' ||
      chr == '"' || chr == '/' || chr == '[' || chr == ']' || chr == '?' ||
      chr == '=' || chr == '{' || chr == '}' || chr == '\\') {
||||||| merged common ancestors
void EnsureBuffer(UniquePtr<uint8_t[]> &buf, uint32_t newSize,
                  uint32_t preserve, uint32_t &objSize)
{
    localEnsureBuffer<uint8_t> (buf, newSize, preserve, objSize);
}

static bool
IsTokenSymbol(signed char chr) {
  if (chr < 33 || chr == 127 ||
      chr == '(' || chr == ')' || chr == '<' || chr == '>' ||
      chr == '@' || chr == ',' || chr == ';' || chr == ':' ||
      chr == '"' || chr == '/' || chr == '[' || chr == ']' ||
      chr == '?' || chr == '=' || chr == '{' || chr == '}' || chr == '\\') {
=======
void EnsureBuffer(UniquePtr<uint8_t[]>& buf, uint32_t newSize,
                  uint32_t preserve, uint32_t& objSize) {
  localEnsureBuffer<uint8_t>(buf, newSize, preserve, objSize);
}

static bool IsTokenSymbol(signed char chr) {
  if (chr < 33 || chr == 127 || chr == '(' || chr == ')' || chr == '<' ||
      chr == '>' || chr == '@' || chr == ',' || chr == ';' || chr == ':' ||
      chr == '"' || chr == '/' || chr == '[' || chr == ']' || chr == '?' ||
      chr == '=' || chr == '{' || chr == '}' || chr == '\\') {
>>>>>>> upstream-releases
    return false;
  }
  return true;
}

ParsedHeaderPair::ParsedHeaderPair(const char* name, int32_t nameLen,
                                   const char* val, int32_t valLen,
                                   bool isQuotedValue)
    : mName(nsDependentCSubstring(nullptr, 0u)),
      mValue(nsDependentCSubstring(nullptr, 0u)),
      mIsQuotedValue(isQuotedValue) {
  if (nameLen > 0) {
    mName.Rebind(name, name + nameLen);
  }
  if (valLen > 0) {
    if (mIsQuotedValue) {
      RemoveQuotedStringEscapes(val, valLen);
      mValue.Rebind(mUnquotedValue.BeginWriting(), mUnquotedValue.Length());
    } else {
      mValue.Rebind(val, val + valLen);
    }
  }
}

<<<<<<< HEAD
void ParsedHeaderPair::RemoveQuotedStringEscapes(const char *val,
                                                 int32_t valLen) {
  mUnquotedValue.Truncate();
  const char *c = val;
  for (int32_t i = 0; i < valLen; ++i) {
    if (c[i] == '\\' && c[i + 1]) {
      ++i;
||||||| merged common ancestors
void
ParsedHeaderPair::RemoveQuotedStringEscapes(const char *val, int32_t valLen)
{
    mUnquotedValue.Truncate();
    const char *c = val;
    for (int32_t i = 0; i < valLen; ++i) {
        if (c[i] == '\\' && c[i + 1]) {
            ++i;
        }
        mUnquotedValue.Append(c[i]);
=======
void ParsedHeaderPair::RemoveQuotedStringEscapes(const char* val,
                                                 int32_t valLen) {
  mUnquotedValue.Truncate();
  const char* c = val;
  for (int32_t i = 0; i < valLen; ++i) {
    if (c[i] == '\\' && c[i + 1]) {
      ++i;
>>>>>>> upstream-releases
    }
    mUnquotedValue.Append(c[i]);
  }
}

<<<<<<< HEAD
static void Tokenize(
    const char *input, uint32_t inputLen, const char token,
    const std::function<void(const char *, uint32_t)> &consumer) {
  auto trimWhitespace = [](const char *in, uint32_t inLen, const char **out,
                           uint32_t *outLen) {
    *out = in;
    *outLen = inLen;
    if (inLen == 0) {
      return;
||||||| merged common ancestors
static
void Tokenize(const char *input, uint32_t inputLen, const char token,
              const std::function<void(const char *, uint32_t)>& consumer)
{
    auto trimWhitespace =
        [] (const char *in, uint32_t inLen, const char **out, uint32_t *outLen) {
            *out = in;
            *outLen = inLen;
            if (inLen == 0) {
                return;
            }

            // Trim leading space
            while (nsCRT::IsAsciiSpace(**out)) {
                (*out)++;
                --(*outLen);
            }

            // Trim tailing space
            for (const char *i = *out + *outLen - 1; i >= *out; --i) {
                if (!nsCRT::IsAsciiSpace(*i)) {
                    break;
                }
                --(*outLen);
            }
        };

    const char *first = input;
    bool inQuote = false;
    const char *result = nullptr;
    uint32_t resultLen = 0;
    for (uint32_t index = 0; index < inputLen; ++index) {
        if (inQuote && input[index] == '\\' && input[index + 1]) {
            index++;
            continue;
        }
        if (input[index] == '"') {
            inQuote = !inQuote;
            continue;
        }
        if (inQuote) {
            continue;
        }
        if (input[index] == token) {
            trimWhitespace(first, (input + index) - first,
                           &result, &resultLen);
            consumer(result, resultLen);
            first = input + index + 1;
        }
=======
static void Tokenize(
    const char* input, uint32_t inputLen, const char token,
    const std::function<void(const char*, uint32_t)>& consumer) {
  auto trimWhitespace = [](const char* in, uint32_t inLen, const char** out,
                           uint32_t* outLen) {
    *out = in;
    *outLen = inLen;
    if (inLen == 0) {
      return;
>>>>>>> upstream-releases
    }

    // Trim leading space
    while (nsCRT::IsAsciiSpace(**out)) {
      (*out)++;
      --(*outLen);
    }

<<<<<<< HEAD
    // Trim tailing space
    for (const char *i = *out + *outLen - 1; i >= *out; --i) {
      if (!nsCRT::IsAsciiSpace(*i)) {
        break;
      }
      --(*outLen);
||||||| merged common ancestors
ParsedHeaderValueList::ParsedHeaderValueList(const char *t,
                                             uint32_t len,
                                             bool allowInvalidValue)
{
    if (!len) {
        return;
=======
    // Trim tailing space
    for (const char* i = *out + *outLen - 1; i >= *out; --i) {
      if (!nsCRT::IsAsciiSpace(*i)) {
        break;
      }
      --(*outLen);
>>>>>>> upstream-releases
    }
  };

<<<<<<< HEAD
  const char *first = input;
  bool inQuote = false;
  const char *result = nullptr;
  uint32_t resultLen = 0;
  for (uint32_t index = 0; index < inputLen; ++index) {
    if (inQuote && input[index] == '\\' && input[index + 1]) {
      index++;
      continue;
    }
    if (input[index] == '"') {
      inQuote = !inQuote;
      continue;
    }
    if (inQuote) {
      continue;
    }
    if (input[index] == token) {
      trimWhitespace(first, (input + index) - first, &result, &resultLen);
      consumer(result, resultLen);
      first = input + index + 1;
    }
  }
||||||| merged common ancestors
    ParsedHeaderValueList *self = this;
    auto consumer = [=] (const char *output, uint32_t outputLength) {
        self->ParseNameAndValue(output, allowInvalidValue);
    };
=======
  const char* first = input;
  bool inQuote = false;
  const char* result = nullptr;
  uint32_t resultLen = 0;
  for (uint32_t index = 0; index < inputLen; ++index) {
    if (inQuote && input[index] == '\\' && input[index + 1]) {
      index++;
      continue;
    }
    if (input[index] == '"') {
      inQuote = !inQuote;
      continue;
    }
    if (inQuote) {
      continue;
    }
    if (input[index] == token) {
      trimWhitespace(first, (input + index) - first, &result, &resultLen);
      consumer(result, resultLen);
      first = input + index + 1;
    }
  }
>>>>>>> upstream-releases

  trimWhitespace(first, (input + inputLen) - first, &result, &resultLen);
  consumer(result, resultLen);
}

<<<<<<< HEAD
ParsedHeaderValueList::ParsedHeaderValueList(const char *t, uint32_t len,
                                             bool allowInvalidValue) {
  if (!len) {
    return;
  }
||||||| merged common ancestors
void
ParsedHeaderValueList::ParseNameAndValue(const char *input, bool allowInvalidValue)
{
    const char *nameStart = input;
    const char *nameEnd = nullptr;
    const char *valueStart = input;
    const char *valueEnd = nullptr;
    bool isQuotedString = false;
    bool invalidValue = false;
=======
ParsedHeaderValueList::ParsedHeaderValueList(const char* t, uint32_t len,
                                             bool allowInvalidValue) {
  if (!len) {
    return;
  }
>>>>>>> upstream-releases

<<<<<<< HEAD
  ParsedHeaderValueList *self = this;
  auto consumer = [=](const char *output, uint32_t outputLength) {
    self->ParseNameAndValue(output, allowInvalidValue);
  };
||||||| merged common ancestors
    for (; *input && *input != ';' && *input != ',' &&
           !nsCRT::IsAsciiSpace(*input) && *input != '='; input++)
        ;
=======
  ParsedHeaderValueList* self = this;
  auto consumer = [=](const char* output, uint32_t outputLength) {
    self->ParseNameAndValue(output, allowInvalidValue);
  };
>>>>>>> upstream-releases

  Tokenize(t, len, ';', consumer);
}

<<<<<<< HEAD
void ParsedHeaderValueList::ParseNameAndValue(const char *input,
                                              bool allowInvalidValue) {
  const char *nameStart = input;
  const char *nameEnd = nullptr;
  const char *valueStart = input;
  const char *valueEnd = nullptr;
  bool isQuotedString = false;
  bool invalidValue = false;
||||||| merged common ancestors
    if (!(nameEnd - nameStart)) {
        return;
    }
=======
void ParsedHeaderValueList::ParseNameAndValue(const char* input,
                                              bool allowInvalidValue) {
  const char* nameStart = input;
  const char* nameEnd = nullptr;
  const char* valueStart = input;
  const char* valueEnd = nullptr;
  bool isQuotedString = false;
  bool invalidValue = false;
>>>>>>> upstream-releases

  for (; *input && *input != ';' && *input != ',' &&
         !nsCRT::IsAsciiSpace(*input) && *input != '=';
       input++)
    ;

  nameEnd = input;

  if (!(nameEnd - nameStart)) {
    return;
  }

<<<<<<< HEAD
  // Check whether param name is a valid token.
  for (const char *c = nameStart; c < nameEnd; c++) {
    if (!IsTokenSymbol(*c)) {
      nameEnd = c;
      break;
||||||| merged common ancestors
    if (!*input || *input++ != '=') {
        mValues.AppendElement(ParsedHeaderPair(nameStart, nameEnd - nameStart,
                                               nullptr, 0, false));
        return;
=======
  // Check whether param name is a valid token.
  for (const char* c = nameStart; c < nameEnd; c++) {
    if (!IsTokenSymbol(*c)) {
      nameEnd = c;
      break;
>>>>>>> upstream-releases
    }
  }

  if (!(nameEnd - nameStart)) {
    return;
  }

  while (nsCRT::IsAsciiSpace(*input)) {
    ++input;
  }

  if (!*input || *input++ != '=') {
    mValues.AppendElement(
        ParsedHeaderPair(nameStart, nameEnd - nameStart, nullptr, 0, false));
    return;
  }

  while (nsCRT::IsAsciiSpace(*input)) {
    ++input;
  }

<<<<<<< HEAD
  if (*input != '"') {
    // The value is a token, not a quoted string.
    valueStart = input;
    for (valueEnd = input; *valueEnd && !nsCRT::IsAsciiSpace(*valueEnd) &&
                           *valueEnd != ';' && *valueEnd != ',';
         valueEnd++)
      ;
    input = valueEnd;
    if (!allowInvalidValue) {
      for (const char *c = valueStart; c < valueEnd; c++) {
        if (!IsTokenSymbol(*c)) {
          valueEnd = c;
          break;
||||||| merged common ancestors
        input = valueEnd;
        // *valueEnd != null means that *valueEnd is quote character.
        if (*valueEnd) {
            input++;
=======
  if (*input != '"') {
    // The value is a token, not a quoted string.
    valueStart = input;
    for (valueEnd = input; *valueEnd && !nsCRT::IsAsciiSpace(*valueEnd) &&
                           *valueEnd != ';' && *valueEnd != ',';
         valueEnd++)
      ;
    if (!allowInvalidValue) {
      for (const char* c = valueStart; c < valueEnd; c++) {
        if (!IsTokenSymbol(*c)) {
          valueEnd = c;
          break;
>>>>>>> upstream-releases
        }
      }
    }
  } else {
    bool foundQuotedEnd = false;
    isQuotedString = true;

    ++input;
    valueStart = input;
    for (valueEnd = input; *valueEnd; ++valueEnd) {
      if (*valueEnd == '\\' && *(valueEnd + 1)) {
        ++valueEnd;
      } else if (*valueEnd == '"') {
        foundQuotedEnd = true;
        break;
      }
    }
    if (!foundQuotedEnd) {
      invalidValue = true;
    }

    input = valueEnd;
    // *valueEnd != null means that *valueEnd is quote character.
    if (*valueEnd) {
      input++;
    }
  }

  if (invalidValue) {
    valueEnd = valueStart;
  }

  mValues.AppendElement(ParsedHeaderPair(nameStart, nameEnd - nameStart,
                                         valueStart, valueEnd - valueStart,
                                         isQuotedString));
}

<<<<<<< HEAD
ParsedHeaderValueListList::ParsedHeaderValueListList(
    const nsCString &fullHeader, bool allowInvalidValue)
    : mFull(fullHeader) {
  auto &values = mValues;
  auto consumer = [&values, allowInvalidValue](const char *output,
                                               uint32_t outputLength) {
    values.AppendElement(
        ParsedHeaderValueList(output, outputLength, allowInvalidValue));
  };
||||||| merged common ancestors
ParsedHeaderValueListList::ParsedHeaderValueListList(const nsCString &fullHeader,
                                                     bool allowInvalidValue)
    : mFull(fullHeader)
{
    auto &values = mValues;
    auto consumer =
        [&values, allowInvalidValue] (const char *output, uint32_t outputLength) {
            values.AppendElement(ParsedHeaderValueList(output,
                                                       outputLength,
                                                       allowInvalidValue));
        };
=======
ParsedHeaderValueListList::ParsedHeaderValueListList(
    const nsCString& fullHeader, bool allowInvalidValue)
    : mFull(fullHeader) {
  auto& values = mValues;
  auto consumer = [&values, allowInvalidValue](const char* output,
                                               uint32_t outputLength) {
    values.AppendElement(
        ParsedHeaderValueList(output, outputLength, allowInvalidValue));
  };
>>>>>>> upstream-releases

  Tokenize(mFull.BeginReading(), mFull.Length(), ',', consumer);
}

<<<<<<< HEAD
void LogCallingScriptLocation(void *instance) {
  if (!LOG4_ENABLED()) {
    return;
  }
||||||| merged common ancestors
void LogCallingScriptLocation(void* instance)
{
    if (!LOG4_ENABLED()) {
        return;
    }
=======
void LogCallingScriptLocation(void* instance) {
  if (!LOG4_ENABLED()) {
    return;
  }
>>>>>>> upstream-releases

<<<<<<< HEAD
  JSContext *cx = nsContentUtils::GetCurrentJSContext();
  if (!cx) {
    return;
  }
||||||| merged common ancestors
    JSContext* cx = nsContentUtils::GetCurrentJSContext();
    if (!cx) {
        return;
    }
=======
  JSContext* cx = nsContentUtils::GetCurrentJSContext();
  if (!cx) {
    return;
  }
>>>>>>> upstream-releases

  nsAutoCString fileNameString;
  uint32_t line = 0, col = 0;
  if (!nsJSUtils::GetCallingLocation(cx, fileNameString, &line, &col)) {
    return;
  }

<<<<<<< HEAD
  LOG(("%p called from script: %s:%u:%u", instance, fileNameString.get(), line,
       col));
||||||| merged common ancestors
    LOG(("%p called from script: %s:%u:%u", instance, fileNameString.get(), line, col));
=======
  LOG(("%p called from script: %s:%u:%u", instance, fileNameString.get(), line,
       col));
}

void LogHeaders(const char* lineStart) {
  static bool sanitize = true;
  static nsresult once = Preferences::AddBoolVarCache(
      &sanitize, "network.http.sanitize-headers-in-logs", true);
  Unused << once;

  nsAutoCString buf;
  char* endOfLine;
  while ((endOfLine = PL_strstr(lineStart, "\r\n"))) {
    buf.Assign(lineStart, endOfLine - lineStart);
    if (sanitize && (PL_strcasestr(buf.get(), "authorization: ") ||
                     PL_strcasestr(buf.get(), "proxy-authorization: "))) {
      char* p = PL_strchr(buf.get(), ' ');
      while (p && *++p) {
        *p = '*';
      }
    }
    LOG1(("  %s\n", buf.get()));
    lineStart = endOfLine + 2;
  }
>>>>>>> upstream-releases
}

}  // namespace net
}  // namespace mozilla
