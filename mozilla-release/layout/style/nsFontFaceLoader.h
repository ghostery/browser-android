/* -*- Mode: C++; tab-width: 8; indent-tabs-mode: nil; c-basic-offset: 2 -*- */
/* vim: set ts=8 sts=2 et sw=2 tw=80: */
/* This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/. */

/* code for loading in @font-face defined font data */

#ifndef nsFontFaceLoader_h_
#define nsFontFaceLoader_h_

#include "mozilla/Attributes.h"
#include "mozilla/TimeStamp.h"
#include "mozilla/dom/FontFaceSet.h"
#include "nsCOMPtr.h"
#include "nsIFontLoadCompleteCallback.h"
#include "nsIStreamLoader.h"
#include "nsIChannel.h"
#include "nsIRequestObserver.h"
#include "gfxUserFontSet.h"
#include "nsHashKeys.h"
#include "nsTHashtable.h"

class nsIPrincipal;

<<<<<<< HEAD
class nsFontFaceLoader final : public nsIStreamLoaderObserver,
                               public nsIRequestObserver {
 public:
||||||| merged common ancestors
class nsFontFaceLoader final : public nsIStreamLoaderObserver
                             , public nsIRequestObserver
{
public:
=======
class nsFontFaceLoader final : public nsIStreamLoaderObserver,
                               public nsIRequestObserver,
                               public nsIFontLoadCompleteCallback {
 public:
>>>>>>> upstream-releases
  nsFontFaceLoader(gfxUserFontEntry* aFontToLoad, nsIURI* aFontURI,
                   mozilla::dom::FontFaceSet* aFontFaceSet,
                   nsIChannel* aChannel);

  NS_DECL_ISUPPORTS
  NS_DECL_NSISTREAMLOADEROBSERVER
  NS_DECL_NSIREQUESTOBSERVER

  // initiate the load
  nsresult Init();
  // cancel the load and remove its reference to mFontFaceSet
  void Cancel();

  void DropChannel() { mChannel = nullptr; }

  void StartedLoading(nsIStreamLoader* aStreamLoader);

  static void LoadTimerCallback(nsITimer* aTimer, void* aClosure);

  gfxUserFontEntry* GetUserFontEntry() const { return mUserFontEntry; }

<<<<<<< HEAD
 protected:
||||||| merged common ancestors
protected:
=======
  // Called by the gfxUserFontEntry once it has finished the platform font
  // loading.
  NS_IMETHODIMP FontLoadComplete() final;

 protected:
>>>>>>> upstream-releases
  virtual ~nsFontFaceLoader();

  // helper method for determining the font-display value
  mozilla::StyleFontDisplay GetFontDisplay();

<<<<<<< HEAD
 private:
  RefPtr<gfxUserFontEntry> mUserFontEntry;
  nsCOMPtr<nsIURI> mFontURI;
  RefPtr<mozilla::dom::FontFaceSet> mFontFaceSet;
  nsCOMPtr<nsIChannel> mChannel;
  nsCOMPtr<nsITimer> mLoadTimer;
  mozilla::TimeStamp mStartTime;
  nsIStreamLoader* mStreamLoader;
||||||| merged common ancestors
private:
  RefPtr<gfxUserFontEntry>  mUserFontEntry;
  nsCOMPtr<nsIURI>        mFontURI;
  RefPtr<mozilla::dom::FontFaceSet> mFontFaceSet;
  nsCOMPtr<nsIChannel>    mChannel;
  nsCOMPtr<nsITimer>      mLoadTimer;
  mozilla::TimeStamp      mStartTime;
  nsIStreamLoader*        mStreamLoader;
=======
 private:
  RefPtr<gfxUserFontEntry> mUserFontEntry;
  nsCOMPtr<nsIURI> mFontURI;
  // Cleared in FontFaceSet::~FontFaceSet, and on cancelation and such too.
  mozilla::dom::FontFaceSet* MOZ_NON_OWNING_REF mFontFaceSet;
  nsCOMPtr<nsIChannel> mChannel;
  nsCOMPtr<nsITimer> mLoadTimer;
  mozilla::TimeStamp mStartTime;
  nsIStreamLoader* mStreamLoader;
  bool mInStreamComplete = false;
  bool mInLoadTimerCallback = false;
>>>>>>> upstream-releases
};

#endif /* !defined(nsFontFaceLoader_h_) */
