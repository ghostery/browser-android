/* -*- Mode: C++; tab-width: 2; indent-tabs-mode: nil; c-basic-offset: 2 -*- */
/* This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/. */

// data implementation header

#ifndef nsDataChannel_h___
#define nsDataChannel_h___

#include "nsBaseChannel.h"

class nsIInputStream;

<<<<<<< HEAD
class nsDataChannel : public nsBaseChannel {
 public:
  explicit nsDataChannel(nsIURI *uri) { SetURI(uri); }
||||||| merged common ancestors
class nsDataChannel : public nsBaseChannel
{
public:
    explicit nsDataChannel(nsIURI *uri) {
        SetURI(uri);
    }
=======
class nsDataChannel : public nsBaseChannel {
 public:
  explicit nsDataChannel(nsIURI* uri) { SetURI(uri); }
>>>>>>> upstream-releases

<<<<<<< HEAD
 protected:
  virtual MOZ_MUST_USE nsresult OpenContentStream(
      bool async, nsIInputStream **result, nsIChannel **channel) override;
||||||| merged common ancestors
protected:
    virtual MOZ_MUST_USE
    nsresult OpenContentStream(bool async,
                               nsIInputStream **result,
                               nsIChannel** channel) override;
=======
 protected:
  virtual MOZ_MUST_USE nsresult OpenContentStream(
      bool async, nsIInputStream** result, nsIChannel** channel) override;
>>>>>>> upstream-releases
};

#endif /* nsDataChannel_h___ */
