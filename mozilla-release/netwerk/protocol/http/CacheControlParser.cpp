/* -*- Mode: C++; tab-width: 8; indent-tabs-mode: nil; c-basic-offset: 2 -*- */
/* vim: set sw=2 ts=8 et tw=80 : */
/* This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/. */

#include "CacheControlParser.h"

namespace mozilla {
namespace net {

<<<<<<< HEAD
CacheControlParser::CacheControlParser(nsACString const &aHeader)
    : Tokenizer(aHeader, nullptr, "-_"),
      mMaxAgeSet(false),
      mMaxAge(0),
      mMaxStaleSet(false),
      mMaxStale(0),
      mMinFreshSet(false),
      mMinFresh(0),
      mNoCache(false),
      mNoStore(false) {
||||||| merged common ancestors
CacheControlParser::CacheControlParser(nsACString const &aHeader)
  : Tokenizer(aHeader, nullptr, "-_")
  , mMaxAgeSet(false)
  , mMaxAge(0)
  , mMaxStaleSet(false)
  , mMaxStale(0)
  , mMinFreshSet(false)
  , mMinFresh(0)
  , mNoCache(false)
  , mNoStore(false)
{
=======
CacheControlParser::CacheControlParser(nsACString const& aHeader)
    : Tokenizer(aHeader, nullptr, "-_"),
      mMaxAgeSet(false),
      mMaxAge(0),
      mMaxStaleSet(false),
      mMaxStale(0),
      mMinFreshSet(false),
      mMinFresh(0),
      mNoCache(false),
      mNoStore(false) {
>>>>>>> upstream-releases
  SkipWhites();
  if (!CheckEOF()) {
    Directive();
  }
}

void CacheControlParser::Directive() {
  if (CheckWord("no-cache")) {
    mNoCache = true;
    IgnoreDirective();  // ignore any optionally added values
  } else if (CheckWord("no-store")) {
    mNoStore = true;
  } else if (CheckWord("max-age")) {
    mMaxAgeSet = SecondsValue(&mMaxAge);
  } else if (CheckWord("max-stale")) {
    mMaxStaleSet = SecondsValue(&mMaxStale, PR_UINT32_MAX);
  } else if (CheckWord("min-fresh")) {
    mMinFreshSet = SecondsValue(&mMinFresh);
  } else {
    IgnoreDirective();
  }

  SkipWhites();
  if (CheckEOF()) {
    return;
  }
  if (CheckChar(',')) {
    SkipWhites();
    Directive();
    return;
  }

  NS_WARNING("Unexpected input in Cache-control header value");
}

<<<<<<< HEAD
bool CacheControlParser::SecondsValue(uint32_t *seconds, uint32_t defaultVal) {
||||||| merged common ancestors
bool CacheControlParser::SecondsValue(uint32_t *seconds, uint32_t defaultVal)
{
=======
bool CacheControlParser::SecondsValue(uint32_t* seconds, uint32_t defaultVal) {
>>>>>>> upstream-releases
  SkipWhites();
  if (!CheckChar('=')) {
    *seconds = defaultVal;
    return !!defaultVal;
  }

  SkipWhites();
  if (!ReadInteger(seconds)) {
    NS_WARNING("Unexpected value in Cache-control header value");
    return false;
  }

  return true;
}

void CacheControlParser::IgnoreDirective() {
  Token t;
  while (Next(t)) {
    if (t.Equals(Token::Char(',')) || t.Equals(Token::EndOfFile())) {
      Rollback();
      break;
    }
    if (t.Equals(Token::Char('"'))) {
      SkipUntil(Token::Char('"'));
      if (!CheckChar('"')) {
        NS_WARNING(
            "Missing quoted string expansion in Cache-control header value");
        break;
      }
    }
  }
}

<<<<<<< HEAD
bool CacheControlParser::MaxAge(uint32_t *seconds) {
||||||| merged common ancestors
bool CacheControlParser::MaxAge(uint32_t *seconds)
{
=======
bool CacheControlParser::MaxAge(uint32_t* seconds) {
>>>>>>> upstream-releases
  *seconds = mMaxAge;
  return mMaxAgeSet;
}

<<<<<<< HEAD
bool CacheControlParser::MaxStale(uint32_t *seconds) {
||||||| merged common ancestors
bool CacheControlParser::MaxStale(uint32_t *seconds)
{
=======
bool CacheControlParser::MaxStale(uint32_t* seconds) {
>>>>>>> upstream-releases
  *seconds = mMaxStale;
  return mMaxStaleSet;
}

<<<<<<< HEAD
bool CacheControlParser::MinFresh(uint32_t *seconds) {
||||||| merged common ancestors
bool CacheControlParser::MinFresh(uint32_t *seconds)
{
=======
bool CacheControlParser::MinFresh(uint32_t* seconds) {
>>>>>>> upstream-releases
  *seconds = mMinFresh;
  return mMinFreshSet;
}

bool CacheControlParser::NoCache() { return mNoCache; }

bool CacheControlParser::NoStore() { return mNoStore; }

}  // namespace net
}  // namespace mozilla
