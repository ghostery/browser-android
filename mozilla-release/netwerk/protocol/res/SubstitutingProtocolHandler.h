/* -*- Mode: C++; tab-width: 8; indent-tabs-mode: nil; c-basic-offset: 2 -*- */
/* vim: set ts=8 sts=2 et sw=2 tw=80: */
/* This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at http://mozilla.org/MPL/2.0/. */

#ifndef SubstitutingProtocolHandler_h___
#define SubstitutingProtocolHandler_h___

#include "nsISubstitutingProtocolHandler.h"

#include "nsISubstitutionObserver.h"
#include "nsDataHashtable.h"
#include "nsStandardURL.h"
#include "nsJARURI.h"
#include "mozilla/chrome/RegistryMessageUtils.h"
#include "mozilla/Maybe.h"
#include "mozilla/RWLock.h"

class nsIIOService;

namespace mozilla {
namespace net {

//
// Base class for resource://-like substitution protocols.
//
// If you add a new protocol, make sure to change nsChromeRegistryChrome
// to properly invoke CollectSubstitutions at the right time.
class SubstitutingProtocolHandler {
 public:
  SubstitutingProtocolHandler(const char* aScheme, uint32_t aFlags,
                              bool aEnforceFileOrJar = true);
  explicit SubstitutingProtocolHandler(const char* aScheme);

  NS_INLINE_DECL_THREADSAFE_REFCOUNTING(SubstitutingProtocolHandler);
  NS_DECL_NON_VIRTUAL_NSIPROTOCOLHANDLER;
  NS_DECL_NON_VIRTUAL_NSISUBSTITUTINGPROTOCOLHANDLER;

<<<<<<< HEAD
  bool HasSubstitution(const nsACString& aRoot) const {
    return mSubstitutions.Get(aRoot, nullptr);
  }
||||||| merged common ancestors
  bool HasSubstitution(const nsACString& aRoot) const { return mSubstitutions.Get(aRoot, nullptr); }
=======
  bool HasSubstitution(const nsACString& aRoot) const {
    AutoReadLock lock(const_cast<RWLock&>(mSubstitutionsLock));
    return mSubstitutions.Get(aRoot, nullptr);
  }

  nsresult NewURI(const nsACString& aSpec, const char* aCharset,
                  nsIURI* aBaseURI, nsIURI** aResult);
>>>>>>> upstream-releases

  MOZ_MUST_USE nsresult
  CollectSubstitutions(InfallibleTArray<SubstitutionMapping>& aResources);

 protected:
  virtual ~SubstitutingProtocolHandler() = default;
  void ConstructInternal();

  MOZ_MUST_USE nsresult SendSubstitution(const nsACString& aRoot,
                                         nsIURI* aBaseURI, uint32_t aFlags);

  nsresult GetSubstitutionFlags(const nsACString& root, uint32_t* flags);

  // Override this in the subclass to try additional lookups after checking
  // mSubstitutions.
  virtual MOZ_MUST_USE nsresult GetSubstitutionInternal(const nsACString& aRoot,
                                                        nsIURI** aResult,
                                                        uint32_t* aFlags) {
    *aResult = nullptr;
    *aFlags = 0;
    return NS_ERROR_NOT_AVAILABLE;
  }

  // Override this in the subclass to check for special case when resolving URIs
  // _before_ checking substitutions.
  virtual MOZ_MUST_USE bool ResolveSpecialCases(const nsACString& aHost,
                                                const nsACString& aPath,
                                                const nsACString& aPathname,
<<<<<<< HEAD
                                                nsACString& aResult) {
||||||| merged common ancestors
                                                nsACString& aResult)
  {
=======
                                                nsACString& aResult) {
    return false;
  }

  // This method should only return true if GetSubstitutionInternal would
  // return the RESOLVE_JAR_URI flag.
  virtual MOZ_MUST_USE bool MustResolveJAR(const nsACString& aRoot) {
>>>>>>> upstream-releases
    return false;
  }

  // Override this in the subclass to check for special case when opening
  // channels.
  virtual MOZ_MUST_USE nsresult SubstituteChannel(nsIURI* uri,
                                                  nsILoadInfo* aLoadInfo,
                                                  nsIChannel** result) {
    return NS_OK;
  }

  nsIIOService* IOService() { return mIOService; }

 private:
  struct SubstitutionEntry {
    SubstitutionEntry() : flags(0) {}

    ~SubstitutionEntry() = default;

    nsCOMPtr<nsIURI> baseURI;
    uint32_t flags;
  };

  // Notifies all observers that a new substitution from |aRoot| to
  // |aBaseURI| has been set/installed for this protocol handler.
  void NotifyObservers(const nsACString& aRoot, nsIURI* aBaseURI);

  nsCString mScheme;
  Maybe<uint32_t> mFlags;

  RWLock mSubstitutionsLock;
  nsDataHashtable<nsCStringHashKey, SubstitutionEntry> mSubstitutions;
  nsCOMPtr<nsIIOService> mIOService;

  // The list of observers added with AddObserver that will be
  // notified when substitutions are set or unset.
  nsTArray<nsCOMPtr<nsISubstitutionObserver>> mObservers;

  // Returns a SubstitutingJARURI if |aUrl| maps to a |jar:| URI,
  // otherwise will return |aURL|
  nsresult ResolveJARURI(nsIURL* aURL, nsIURI** aResult);

  // In general, we expect the principal of a document loaded from a
  // substituting URI to be a codebase principal for that URI (rather than
  // a principal for whatever is underneath). However, this only works if
  // the protocol handler for the underlying URI doesn't set an explicit
  // owner (which chrome:// does, for example). So we want to require that
  // substituting URIs only map to other URIs of the same type, or to
  // file:// and jar:// URIs.
  //
  // Enforcing this for ye olde resource:// URIs could carry compat risks, so
  // we just try to enforce it on new protocols going forward.
  bool mEnforceFileOrJar;
};

<<<<<<< HEAD
// SubstitutingURL : overrides nsStandardURL::GetFile to provide nsIFile
// resolution
class SubstitutingURL : public nsStandardURL {
 public:
  virtual nsStandardURL* StartClone() override;
  virtual MOZ_MUST_USE nsresult EnsureFile() override;
  NS_IMETHOD GetClassIDNoAlloc(nsCID* aCID) override;

 private:
  explicit SubstitutingURL() : nsStandardURL(true) {}
  explicit SubstitutingURL(bool aSupportsFileURL) : nsStandardURL(true) {
    MOZ_ASSERT(aSupportsFileURL);
  }
  virtual nsresult Clone(nsIURI** aURI) override {
    return nsStandardURL::Clone(aURI);
  }

 public:
  class Mutator : public TemplatedMutator<SubstitutingURL> {
    NS_DECL_ISUPPORTS
   public:
    explicit Mutator() = default;

   private:
    virtual ~Mutator() = default;

    SubstitutingURL* Create() override { return new SubstitutingURL(); }
  };

  NS_IMETHOD Mutate(nsIURIMutator** aMutator) override {
    RefPtr<SubstitutingURL::Mutator> mutator = new SubstitutingURL::Mutator();
    nsresult rv = mutator->InitFromURI(this);
    if (NS_FAILED(rv)) {
      return rv;
    }
    mutator.forget(aMutator);
    return NS_OK;
  }

  friend BaseURIMutator<SubstitutingURL>;
  friend TemplatedMutator<SubstitutingURL>;
};

}  // namespace net
}  // namespace mozilla
||||||| merged common ancestors
// SubstitutingURL : overrides nsStandardURL::GetFile to provide nsIFile resolution
class SubstitutingURL : public nsStandardURL
{
public:
  virtual nsStandardURL* StartClone() override;
  virtual MOZ_MUST_USE nsresult EnsureFile() override;
  NS_IMETHOD GetClassIDNoAlloc(nsCID *aCID) override;

private:
  explicit SubstitutingURL() : nsStandardURL(true) {}
  explicit SubstitutingURL(bool aSupportsFileURL) : nsStandardURL(true) { MOZ_ASSERT(aSupportsFileURL); }
  virtual nsresult Clone(nsIURI** aURI) override { return nsStandardURL::Clone(aURI); }

public:
  class Mutator
    : public TemplatedMutator<SubstitutingURL>
  {
    NS_DECL_ISUPPORTS
  public:
    explicit Mutator() = default;
  private:
    virtual ~Mutator() = default;

    SubstitutingURL* Create() override
    {
      return new SubstitutingURL();
    }
  };

  NS_IMETHOD Mutate(nsIURIMutator** aMutator) override
  {
    RefPtr<SubstitutingURL::Mutator> mutator = new SubstitutingURL::Mutator();
    nsresult rv = mutator->InitFromURI(this);
    if (NS_FAILED(rv)) {
      return rv;
    }
    mutator.forget(aMutator);
    return NS_OK;
  }

  friend BaseURIMutator<SubstitutingURL>;
  friend TemplatedMutator<SubstitutingURL>;
};

} // namespace net
} // namespace mozilla
=======
}  // namespace net
}  // namespace mozilla
>>>>>>> upstream-releases

#endif /* SubstitutingProtocolHandler_h___ */
