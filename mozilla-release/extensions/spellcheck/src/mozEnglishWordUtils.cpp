/* -*- Mode: C++; tab-width: 2; indent-tabs-mode: nil; c-basic-offset: 2 -*- */
/* This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/. */

#include "mozEnglishWordUtils.h"
#include "nsReadableUtils.h"
#include "nsIServiceManager.h"
#include "nsUnicharUtils.h"
#include "nsUnicodeProperties.h"
#include "nsCRT.h"
#include "mozilla/Likely.h"
#include "nsMemory.h"

NS_IMPL_CYCLE_COLLECTION_ROOT_NATIVE(mozEnglishWordUtils, AddRef)
NS_IMPL_CYCLE_COLLECTION_UNROOT_NATIVE(mozEnglishWordUtils, Release)

NS_IMPL_CYCLE_COLLECTION(mozEnglishWordUtils, mURLDetector)

mozEnglishWordUtils::mozEnglishWordUtils() {
  mURLDetector = do_CreateInstance(MOZ_TXTTOHTMLCONV_CONTRACTID);
}

mozEnglishWordUtils::~mozEnglishWordUtils() {}

// This needs vast improvement

// static
bool mozEnglishWordUtils::ucIsAlpha(char16_t aChar) {
  // XXX we have to fix callers to handle the full Unicode range
  return nsUGenCategory::kLetter == mozilla::unicode::GetGenCategory(aChar);
}

<<<<<<< HEAD
nsresult mozEnglishWordUtils::FindNextWord(const char16_t *word,
                                           uint32_t length, uint32_t offset,
                                           int32_t *begin, int32_t *end) {
  const char16_t *p = word + offset;
  const char16_t *endbuf = word + length;
  const char16_t *startWord = p;
  if (p < endbuf) {
    // XXX These loops should be modified to handle non-BMP characters.
    // if previous character is a word character, need to advance out of the
    // word
    if (offset > 0 && ucIsAlpha(*(p - 1))) {
      while (p < endbuf && ucIsAlpha(*p)) p++;
    }
    while ((p < endbuf) && (!ucIsAlpha(*p))) {
      p++;
    }
    startWord = p;
    while ((p < endbuf) && ((ucIsAlpha(*p)) || (*p == '\''))) {
      p++;
    }

    // we could be trying to break down a url, we don't want to break a url into
    // parts, instead we want to find out if it really is a url and if so, skip
    // it, advancing startWord to a point after the url.
||||||| merged common ancestors
nsresult
mozEnglishWordUtils::FindNextWord(const char16_t *word, uint32_t length,
                                  uint32_t offset, int32_t *begin, int32_t *end)
{
  const char16_t *p = word + offset;
  const char16_t *endbuf = word + length;
  const char16_t *startWord=p;
  if(p<endbuf){
    // XXX These loops should be modified to handle non-BMP characters.
    // if previous character is a word character, need to advance out of the word
    if (offset > 0 && ucIsAlpha(*(p-1))) {
      while (p < endbuf && ucIsAlpha(*p))
        p++;
    }
    while((p < endbuf) && (!ucIsAlpha(*p)))
      {
        p++;
      }
    startWord=p;
    while((p < endbuf) && ((ucIsAlpha(*p))||(*p=='\'')))
      {
        p++;
      }

    // we could be trying to break down a url, we don't want to break a url into parts,
    // instead we want to find out if it really is a url and if so, skip it, advancing startWord
    // to a point after the url.
=======
bool mozEnglishWordUtils::FindNextWord(const nsAString& aWord, uint32_t offset,
                                       int32_t* begin, int32_t* end) {
  if (offset >= aWord.Length()) {
    *begin = -1;
    *end = -1;
    return false;
  }
>>>>>>> upstream-releases

<<<<<<< HEAD
    // before we spend more time looking to see if the word is a url, look for a
    // url identifer and make sure that identifer isn't the last character in
    // the word fragment.
    if ((p < endbuf - 1) && (*p == ':' || *p == '@' || *p == '.')) {
      // ok, we have a possible url...do more research to find out if we really
      // have one and determine the length of the url so we can skip over it.
||||||| merged common ancestors
    // before we spend more time looking to see if the word is a url, look for a url identifer
    // and make sure that identifer isn't the last character in the word fragment.
    if ((p < endbuf - 1) && (*p == ':' || *p == '@' || *p == '.')) {
=======
  const char16_t* word = aWord.BeginReading();
  uint32_t length = aWord.Length();
  const char16_t* p = word + offset;
  const char16_t* endbuf = word + length;
  const char16_t* startWord = p;

  // XXX These loops should be modified to handle non-BMP characters.
  // if previous character is a word character, need to advance out of the
  // word
  if (offset > 0 && ucIsAlpha(*(p - 1))) {
    while (p < endbuf && ucIsAlpha(*p)) {
      p++;
    }
  }
  while ((p < endbuf) && (!ucIsAlpha(*p))) {
    p++;
  }
  startWord = p;
  while ((p < endbuf) && ((ucIsAlpha(*p)) || (*p == '\''))) {
    p++;
  }
>>>>>>> upstream-releases

<<<<<<< HEAD
      if (mURLDetector) {
        int32_t startPos = -1;
        int32_t endPos = -1;
||||||| merged common ancestors
        // ok, we have a possible url...do more research to find out if we really have one
        // and determine the length of the url so we can skip over it.
=======
  // we could be trying to break down a url, we don't want to break a url into
  // parts, instead we want to find out if it really is a url and if so, skip
  // it, advancing startWord to a point after the url.
>>>>>>> upstream-releases

<<<<<<< HEAD
        mURLDetector->FindURLInPlaintext(startWord, endbuf - startWord,
                                         p - startWord, &startPos, &endPos);
||||||| merged common ancestors
        if (mURLDetector)
        {
          int32_t startPos = -1;
          int32_t endPos = -1;
=======
  // before we spend more time looking to see if the word is a url, look for a
  // url identifer and make sure that identifer isn't the last character in
  // the word fragment.
  if ((p < endbuf - 1) && (*p == ':' || *p == '@' || *p == '.')) {
    // ok, we have a possible url...do more research to find out if we really
    // have one and determine the length of the url so we can skip over it.
>>>>>>> upstream-releases

<<<<<<< HEAD
        // ok, if we got a url, adjust the array bounds, skip the current url
        // text and find the next word again
        if (startPos != -1 && endPos != -1) {
          startWord = p + endPos + 1;  // skip over the url
          p = startWord;               // reset p
||||||| merged common ancestors
          mURLDetector->FindURLInPlaintext(startWord, endbuf - startWord, p - startWord, &startPos, &endPos);
=======
    if (mURLDetector) {
      int32_t startPos = -1;
      int32_t endPos = -1;
>>>>>>> upstream-releases

<<<<<<< HEAD
          // now recursively call FindNextWord to search for the next word now
          // that we have skipped the url
          return FindNextWord(word, length, startWord - word, begin, end);
        }
      }
    }
||||||| merged common ancestors
          // ok, if we got a url, adjust the array bounds, skip the current url text and find the next word again
          if (startPos != -1 && endPos != -1) {
            startWord = p + endPos + 1; // skip over the url
            p = startWord; // reset p

            // now recursively call FindNextWord to search for the next word now that we have skipped the url
            return FindNextWord(word, length, startWord - word, begin, end);
          }
        }
    }
=======
      mURLDetector->FindURLInPlaintext(startWord, endbuf - startWord,
                                       p - startWord, &startPos, &endPos);

      // ok, if we got a url, adjust the array bounds, skip the current url
      // text and find the next word again
      if (startPos != -1 && endPos != -1) {
        startWord = p + endPos + 1;  // skip over the url
>>>>>>> upstream-releases

<<<<<<< HEAD
    while ((p > startWord) &&
           (*(p - 1) == '\'')) {  // trim trailing apostrophes
      p--;
||||||| merged common ancestors
    while((p > startWord)&&(*(p-1) == '\'')){  // trim trailing apostrophes
      p--;
=======
        // now recursively call FindNextWord to search for the next word now
        // that we have skipped the url
        return FindNextWord(aWord, startWord - word, begin, end);
      }
>>>>>>> upstream-releases
    }
<<<<<<< HEAD
  } else {
    startWord = endbuf;
||||||| merged common ancestors
  }
  else{
    startWord = endbuf;
=======
  }

  while ((p > startWord) && (*(p - 1) == '\'')) {  // trim trailing apostrophes
    p--;
>>>>>>> upstream-releases
  }
<<<<<<< HEAD
  if (startWord == endbuf) {
||||||| merged common ancestors
  if(startWord == endbuf){
=======

  if (startWord == endbuf) {
>>>>>>> upstream-releases
    *begin = -1;
    *end = -1;
<<<<<<< HEAD
  } else {
    *begin = startWord - word;
    *end = p - word;
  }
  return NS_OK;
||||||| merged common ancestors
  }
  else{
    *begin = startWord-word;
    *end = p-word;
  }
  return NS_OK;
=======
    return false;
  }
  *begin = startWord - word;
  *end = p - word;
  return true;
>>>>>>> upstream-releases
}
