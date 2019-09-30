/* -*- Mode: C++; tab-width: 2; indent-tabs-mode: nil; c-basic-offset: 2 -*- */
/* This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/. */

#ifndef nsHyphenator_h__
#define nsHyphenator_h__

#include "nsCOMPtr.h"
#include "nsString.h"
#include "nsTArray.h"

class nsIURI;

<<<<<<< HEAD
class nsHyphenator {
 public:
  explicit nsHyphenator(nsIURI* aURI);
||||||| merged common ancestors
class nsHyphenator
{
public:
  explicit nsHyphenator(nsIURI *aURI);
=======
class nsHyphenator {
 public:
  nsHyphenator(nsIURI* aURI, bool aHyphenateCapitalized);
>>>>>>> upstream-releases

  NS_INLINE_DECL_REFCOUNTING(nsHyphenator)

  bool IsValid();

  nsresult Hyphenate(const nsAString& aText, nsTArray<bool>& aHyphens);

 private:
  ~nsHyphenator();

<<<<<<< HEAD
 protected:
  void* mDict;
||||||| merged common ancestors
protected:
  void                      *mDict;
=======
  void HyphenateWord(const nsAString& aString, uint32_t aStart, uint32_t aLimit,
                     nsTArray<bool>& aHyphens);

  void* mDict;
  bool mHyphenateCapitalized;
>>>>>>> upstream-releases
};

#endif  // nsHyphenator_h__
