/* -*- Mode: C++; tab-width: 4; indent-tabs-mode: nil; c-basic-offset: 2 -*-
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/. */

#ifndef mozilla_image_decoders_icon_nsIconURI_h
#define mozilla_image_decoders_icon_nsIconURI_h

#include "nsIIconURI.h"
#include "nsCOMPtr.h"
#include "nsString.h"
#include "nsINestedURI.h"
#include "nsIURIMutator.h"

namespace mozilla {
class Encoding;
}

<<<<<<< HEAD
class nsMozIconURI final : public nsIMozIconURI,
                           public nsIIPCSerializableURI,
                           public nsINestedURI {
 public:
||||||| merged common ancestors
class nsMozIconURI final
  : public nsIMozIconURI
  , public nsIIPCSerializableURI
  , public nsINestedURI
{
public:
=======
class nsMozIconURI final : public nsIMozIconURI, public nsINestedURI {
 public:
>>>>>>> upstream-releases
  NS_DECL_THREADSAFE_ISUPPORTS
  NS_DECL_NSIURI
  NS_DECL_NSIMOZICONURI
  NS_DECL_NSINESTEDURI

 protected:
  nsMozIconURI();
  virtual ~nsMozIconURI();
  nsCOMPtr<nsIURL> mIconURL;  // a URL that we want the icon for
  uint32_t mSize;  // the # of pixels in a row that we want for this image.
                   // Typically 16, 32, 128, etc.
  nsCString mContentType;  // optional field explicitly specifying the content
                           // type
  nsCString mFileName;  // for if we don't have an actual file path, we're just
                        // given a filename with an extension
  nsCString mStockIcon;
  int32_t mIconSize;   // -1 if not specified, otherwise index into
                       // kSizeStrings
  int32_t mIconState;  // -1 if not specified, otherwise index into
                       // kStateStrings

<<<<<<< HEAD
 private:
  nsresult Clone(nsIURI **aURI);
  nsresult SetSpecInternal(const nsACString &input);
  nsresult SetScheme(const nsACString &input);
  nsresult SetUserPass(const nsACString &input);
  nsresult SetUsername(const nsACString &input);
  nsresult SetPassword(const nsACString &input);
  nsresult SetHostPort(const nsACString &aValue);
  nsresult SetHost(const nsACString &input);
||||||| merged common ancestors
private:
  nsresult Clone(nsIURI** aURI);
  nsresult SetSpecInternal(const nsACString &input);
  nsresult SetScheme(const nsACString &input);
  nsresult SetUserPass(const nsACString &input);
  nsresult SetUsername(const nsACString &input);
  nsresult SetPassword(const nsACString &input);
  nsresult SetHostPort(const nsACString &aValue);
  nsresult SetHost(const nsACString &input);
=======
 private:
  nsresult Clone(nsIURI** aURI);
  nsresult SetSpecInternal(const nsACString& input);
  nsresult SetScheme(const nsACString& input);
  nsresult SetUserPass(const nsACString& input);
  nsresult SetUsername(const nsACString& input);
  nsresult SetPassword(const nsACString& input);
  nsresult SetHostPort(const nsACString& aValue);
  nsresult SetHost(const nsACString& input);
>>>>>>> upstream-releases
  nsresult SetPort(int32_t port);
<<<<<<< HEAD
  nsresult SetPathQueryRef(const nsACString &input);
  nsresult SetRef(const nsACString &input);
  nsresult SetFilePath(const nsACString &input);
  nsresult SetQuery(const nsACString &input);
  nsresult SetQueryWithEncoding(const nsACString &input,
                                const mozilla::Encoding *encoding);
  bool Deserialize(const mozilla::ipc::URIParams &);
||||||| merged common ancestors
  nsresult SetPathQueryRef(const nsACString &input);
  nsresult SetRef(const nsACString &input);
  nsresult SetFilePath(const nsACString &input);
  nsresult SetQuery(const nsACString &input);
  nsresult SetQueryWithEncoding(const nsACString &input, const mozilla::Encoding* encoding);
  bool Deserialize(const mozilla::ipc::URIParams&);
=======
  nsresult SetPathQueryRef(const nsACString& input);
  nsresult SetRef(const nsACString& input);
  nsresult SetFilePath(const nsACString& input);
  nsresult SetQuery(const nsACString& input);
  nsresult SetQueryWithEncoding(const nsACString& input,
                                const mozilla::Encoding* encoding);
  bool Deserialize(const mozilla::ipc::URIParams&);
>>>>>>> upstream-releases

 public:
  class Mutator final : public nsIURIMutator,
                        public BaseURIMutator<nsMozIconURI> {
    NS_DECL_ISUPPORTS
    NS_FORWARD_SAFE_NSIURISETTERS_RET(mURI)

<<<<<<< HEAD
    NS_IMETHOD Deserialize(const mozilla::ipc::URIParams &aParams) override {
||||||| merged common ancestors
    NS_IMETHOD Deserialize(const mozilla::ipc::URIParams& aParams) override
    {
=======
    NS_IMETHOD Deserialize(const mozilla::ipc::URIParams& aParams) override {
>>>>>>> upstream-releases
      return InitFromIPCParams(aParams);
    }

<<<<<<< HEAD
    NS_IMETHOD Finalize(nsIURI **aURI) override {
||||||| merged common ancestors
    NS_IMETHOD Finalize(nsIURI** aURI) override
    {
=======
    NS_IMETHOD Finalize(nsIURI** aURI) override {
>>>>>>> upstream-releases
      mURI.forget(aURI);
      return NS_OK;
    }

<<<<<<< HEAD
    NS_IMETHOD SetSpec(const nsACString &aSpec,
                       nsIURIMutator **aMutator) override {
||||||| merged common ancestors
    NS_IMETHOD SetSpec(const nsACString & aSpec, nsIURIMutator** aMutator) override
    {
=======
    NS_IMETHOD SetSpec(const nsACString& aSpec,
                       nsIURIMutator** aMutator) override {
>>>>>>> upstream-releases
      if (aMutator) {
        nsCOMPtr<nsIURIMutator> mutator = this;
        mutator.forget(aMutator);
      }
      return InitFromSpec(aSpec);
    }

    explicit Mutator() {}

   private:
    virtual ~Mutator() {}

    friend class nsMozIconURI;
  };

  friend BaseURIMutator<nsMozIconURI>;
};

#endif  // mozilla_image_decoders_icon_nsIconURI_h
