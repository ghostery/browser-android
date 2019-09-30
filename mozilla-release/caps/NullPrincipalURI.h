/* -*- Mode: C++; tab-width: 2; indent-tabs-mode: nil; c-basic-offset: 2 -*-
 * vim: sw=2 ts=2 sts=2 expandtab
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/. */

/**
 * This wraps nsSimpleURI so that all calls to it are done on the main thread.
 */

#ifndef mozilla_NullPrincipalURI_h
#define mozilla_NullPrincipalURI_h

#include "nsIURI.h"
#include "nsISizeOf.h"
#include "nsString.h"
#include "mozilla/Attributes.h"
#include "mozilla/MemoryReporting.h"
#include "NullPrincipal.h"
#include "nsID.h"
#include "nsIURIMutator.h"

// {51fcd543-3b52-41f7-b91b-6b54102236e6}
#define NS_NULLPRINCIPALURI_IMPLEMENTATION_CID       \
  {                                                  \
    0x51fcd543, 0x3b52, 0x41f7, {                    \
      0xb9, 0x1b, 0x6b, 0x54, 0x10, 0x22, 0x36, 0xe6 \
    }                                                \
  }

namespace mozilla {

class Encoding;

<<<<<<< HEAD
class NullPrincipalURI final : public nsIURI,
                               public nsISizeOf,
                               public nsIIPCSerializableURI {
 public:
||||||| merged common ancestors
class NullPrincipalURI final : public nsIURI
                             , public nsISizeOf
                             , public nsIIPCSerializableURI
{
public:
=======
class NullPrincipalURI final : public nsIURI, public nsISizeOf {
 public:
>>>>>>> upstream-releases
  NS_DECL_THREADSAFE_ISUPPORTS
  NS_DECL_NSIURI

  // nsISizeOf
  virtual size_t SizeOfExcludingThis(MallocSizeOf aMallocSizeOf) const override;
  virtual size_t SizeOfIncludingThis(MallocSizeOf aMallocSizeOf) const override;

  // Returns null on failure.
  static already_AddRefed<NullPrincipalURI> Create();

 private:
  NullPrincipalURI();
  NullPrincipalURI(const NullPrincipalURI &aOther);

  ~NullPrincipalURI() {}

  nsresult Init();

  nsAutoCStringN<NSID_LENGTH> mPath;

<<<<<<< HEAD
  nsresult Clone(nsIURI **aURI);
  nsresult SetSpecInternal(const nsACString &input);
  nsresult SetScheme(const nsACString &input);
  nsresult SetUserPass(const nsACString &input);
  nsresult SetUsername(const nsACString &input);
  nsresult SetPassword(const nsACString &input);
  nsresult SetHostPort(const nsACString &aValue);
  nsresult SetHost(const nsACString &input);
||||||| merged common ancestors
  nsresult Clone(nsIURI** aURI);
  nsresult SetSpecInternal(const nsACString &input);
  nsresult SetScheme(const nsACString &input);
  nsresult SetUserPass(const nsACString &input);
  nsresult SetUsername(const nsACString &input);
  nsresult SetPassword(const nsACString &input);
  nsresult SetHostPort(const nsACString &aValue);
  nsresult SetHost(const nsACString &input);
=======
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
                                const Encoding *encoding);
  bool Deserialize(const mozilla::ipc::URIParams &);

 public:
  class Mutator final : public nsIURIMutator,
                        public BaseURIMutator<NullPrincipalURI> {
||||||| merged common ancestors
  nsresult SetPathQueryRef(const nsACString &input);
  nsresult SetRef(const nsACString &input);
  nsresult SetFilePath(const nsACString &input);
  nsresult SetQuery(const nsACString &input);
  nsresult SetQueryWithEncoding(const nsACString &input, const Encoding* encoding);
  bool Deserialize(const mozilla::ipc::URIParams&);

public:
  class Mutator final
      : public nsIURIMutator
      , public BaseURIMutator<NullPrincipalURI>
  {
=======
  nsresult SetPathQueryRef(const nsACString& input);
  nsresult SetRef(const nsACString& input);
  nsresult SetFilePath(const nsACString& input);
  nsresult SetQuery(const nsACString& input);
  nsresult SetQueryWithEncoding(const nsACString& input,
                                const Encoding* encoding);
  bool Deserialize(const mozilla::ipc::URIParams&);

 public:
  class Mutator final : public nsIURIMutator,
                        public BaseURIMutator<NullPrincipalURI> {
>>>>>>> upstream-releases
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
      return NS_ERROR_NOT_IMPLEMENTED;
    }

    explicit Mutator() {}

   private:
    virtual ~Mutator() {}

    friend class NullPrincipalURI;
  };

  friend class BaseURIMutator<NullPrincipalURI>;
};

}  // namespace mozilla

#endif  // mozilla_NullPrincipalURI_h
