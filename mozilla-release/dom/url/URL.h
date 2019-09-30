/* -*- Mode: C++; tab-width: 8; indent-tabs-mode: nil; c-basic-offset: 2 -*- */
/* vim: set ts=8 sts=2 et sw=2 tw=80: */
/* This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/. */

#ifndef mozilla_dom_URL_h
#define mozilla_dom_URL_h

#include "mozilla/dom/BindingDeclarations.h"
#include "mozilla/dom/URLSearchParams.h"
#include "nsCycleCollectionParticipant.h"
#include "nsString.h"
#include "nsWrapperCache.h"

class nsISupports;
class nsIURI;

namespace mozilla {

class ErrorResult;

namespace dom {

class Blob;
class MediaSource;
class GlobalObject;

<<<<<<< HEAD
class URL : public URLSearchParamsObserver, public nsWrapperCache {
 public:
||||||| merged common ancestors
class URL : public URLSearchParamsObserver
          , public nsWrapperCache
{
public:
=======
class URL final : public URLSearchParamsObserver, public nsWrapperCache {
 public:
>>>>>>> upstream-releases
  NS_DECL_CYCLE_COLLECTING_ISUPPORTS
  NS_DECL_CYCLE_COLLECTION_SCRIPT_HOLDER_CLASS(URL)

<<<<<<< HEAD
  URL(nsISupports* aParent) : mParent(aParent) {}
||||||| merged common ancestors
  URL(nsISupports* aParent)
    : mParent(aParent)
  {}
=======
  explicit URL(nsISupports* aParent) : mParent(aParent) {}
>>>>>>> upstream-releases

  // WebIDL methods
  nsISupports* GetParentObject() const { return mParent; }

<<<<<<< HEAD
  virtual JSObject* WrapObject(JSContext* aCx,
                               JS::Handle<JSObject*> aGivenProto) override;
||||||| merged common ancestors
  virtual JSObject*
  WrapObject(JSContext* aCx, JS::Handle<JSObject*> aGivenProto) override;
=======
  JSObject* WrapObject(JSContext* aCx,
                       JS::Handle<JSObject*> aGivenProto) override;
>>>>>>> upstream-releases

  static already_AddRefed<URL> Constructor(const GlobalObject& aGlobal,
                                           const nsAString& aURL,
                                           const Optional<nsAString>& aBase,
                                           ErrorResult& aRv);

<<<<<<< HEAD
  // Helper for Fetch API
  static already_AddRefed<URL> WorkerConstructor(const GlobalObject& aGlobal,
                                                 const nsAString& aURL,
                                                 const nsAString& aBase,
                                                 ErrorResult& aRv);
||||||| merged common ancestors
  // Helper for Fetch API
  static already_AddRefed<URL>
  WorkerConstructor(const GlobalObject& aGlobal, const nsAString& aURL,
                    const nsAString& aBase, ErrorResult& aRv);
=======
  static already_AddRefed<URL> Constructor(nsISupports* aParent,
                                           const nsAString& aURL,
                                           const nsAString& aBase,
                                           ErrorResult& aRv);
>>>>>>> upstream-releases

<<<<<<< HEAD
  static void CreateObjectURL(const GlobalObject& aGlobal, Blob& aBlob,
                              nsAString& aResult, ErrorResult& aRv);
||||||| merged common ancestors
=======
  static already_AddRefed<URL> Constructor(nsISupports* aParent,
                                           const nsAString& aURL, nsIURI* aBase,
                                           ErrorResult& aRv);
>>>>>>> upstream-releases

<<<<<<< HEAD
  static void CreateObjectURL(const GlobalObject& aGlobal, MediaSource& aSource,
                              nsAString& aResult, ErrorResult& aRv);
||||||| merged common ancestors
  static void
  CreateObjectURL(const GlobalObject& aGlobal, Blob& aBlob,
                  nsAString& aResult, ErrorResult& aRv);
=======
  static void CreateObjectURL(const GlobalObject& aGlobal, Blob& aBlob,
                              nsAString& aResult, ErrorResult& aRv);
>>>>>>> upstream-releases

<<<<<<< HEAD
  static void RevokeObjectURL(const GlobalObject& aGlobal,
                              const nsAString& aURL, ErrorResult& aRv);
||||||| merged common ancestors
  static void
  CreateObjectURL(const GlobalObject& aGlobal, MediaSource& aSource,
                  nsAString& aResult, ErrorResult& aRv);
=======
  static void CreateObjectURL(const GlobalObject& aGlobal, MediaSource& aSource,
                              nsAString& aResult, ErrorResult& aRv);
>>>>>>> upstream-releases

<<<<<<< HEAD
  static bool IsValidURL(const GlobalObject& aGlobal, const nsAString& aURL,
                         ErrorResult& aRv);
||||||| merged common ancestors
  static void
  RevokeObjectURL(const GlobalObject& aGlobal, const nsAString& aURL,
                  ErrorResult& aRv);
=======
  static void RevokeObjectURL(const GlobalObject& aGlobal,
                              const nsAString& aURL, ErrorResult& aRv);
>>>>>>> upstream-releases

<<<<<<< HEAD
  void GetHref(nsAString& aHref) const;
||||||| merged common ancestors
  static bool
  IsValidURL(const GlobalObject& aGlobal, const nsAString& aURL,
             ErrorResult& aRv);
=======
  static bool IsValidURL(const GlobalObject& aGlobal, const nsAString& aURL,
                         ErrorResult& aRv);
>>>>>>> upstream-releases

<<<<<<< HEAD
  virtual void SetHref(const nsAString& aHref, ErrorResult& aRv) = 0;
||||||| merged common ancestors
  void
  GetHref(nsAString& aHref) const;
=======
  void GetHref(nsAString& aHref) const;
>>>>>>> upstream-releases

<<<<<<< HEAD
  virtual void GetOrigin(nsAString& aOrigin, ErrorResult& aRv) const = 0;
||||||| merged common ancestors
  virtual void
  SetHref(const nsAString& aHref, ErrorResult& aRv) = 0;
=======
  void SetHref(const nsAString& aHref, ErrorResult& aRv);
>>>>>>> upstream-releases

<<<<<<< HEAD
  void GetProtocol(nsAString& aProtocol) const;
||||||| merged common ancestors
  virtual void
  GetOrigin(nsAString& aOrigin, ErrorResult& aRv) const = 0;
=======
  void GetOrigin(nsAString& aOrigin, ErrorResult& aRv) const;
>>>>>>> upstream-releases

<<<<<<< HEAD
  virtual void SetProtocol(const nsAString& aProtocol, ErrorResult& aRv) = 0;
||||||| merged common ancestors
  void
  GetProtocol(nsAString& aProtocol) const;
=======
  void GetProtocol(nsAString& aProtocol) const;
>>>>>>> upstream-releases

<<<<<<< HEAD
  void GetUsername(nsAString& aUsername) const;
||||||| merged common ancestors
  virtual void
  SetProtocol(const nsAString& aProtocol, ErrorResult& aRv) = 0;
=======
  void SetProtocol(const nsAString& aProtocol, ErrorResult& aRv);
>>>>>>> upstream-releases

<<<<<<< HEAD
  void SetUsername(const nsAString& aUsername);
||||||| merged common ancestors
  void
  GetUsername(nsAString& aUsername) const;
=======
  void GetUsername(nsAString& aUsername) const;
>>>>>>> upstream-releases

<<<<<<< HEAD
  void GetPassword(nsAString& aPassword) const;
||||||| merged common ancestors
  void
  SetUsername(const nsAString& aUsername);
=======
  void SetUsername(const nsAString& aUsername);
>>>>>>> upstream-releases

<<<<<<< HEAD
  void SetPassword(const nsAString& aPassword);
||||||| merged common ancestors
  void
  GetPassword(nsAString& aPassword) const;
=======
  void GetPassword(nsAString& aPassword) const;
>>>>>>> upstream-releases

<<<<<<< HEAD
  void GetHost(nsAString& aHost) const;
||||||| merged common ancestors
  void
  SetPassword(const nsAString& aPassword);
=======
  void SetPassword(const nsAString& aPassword);
>>>>>>> upstream-releases

<<<<<<< HEAD
  void SetHost(const nsAString& aHost);
||||||| merged common ancestors
  void
  GetHost(nsAString& aHost) const;
=======
  void GetHost(nsAString& aHost) const;
>>>>>>> upstream-releases

<<<<<<< HEAD
  void GetHostname(nsAString& aHostname) const;
||||||| merged common ancestors
  void
  SetHost(const nsAString& aHost);
=======
  void SetHost(const nsAString& aHost);
>>>>>>> upstream-releases

<<<<<<< HEAD
  void SetHostname(const nsAString& aHostname);
||||||| merged common ancestors
  void
  GetHostname(nsAString& aHostname) const;
=======
  void GetHostname(nsAString& aHostname) const;
>>>>>>> upstream-releases

<<<<<<< HEAD
  void GetPort(nsAString& aPort) const;
||||||| merged common ancestors
  void
  SetHostname(const nsAString& aHostname);
=======
  void SetHostname(const nsAString& aHostname);
>>>>>>> upstream-releases

<<<<<<< HEAD
  void SetPort(const nsAString& aPort);
||||||| merged common ancestors
  void
  GetPort(nsAString& aPort) const;
=======
  void GetPort(nsAString& aPort) const;
>>>>>>> upstream-releases

<<<<<<< HEAD
  void GetPathname(nsAString& aPathname) const;
||||||| merged common ancestors
  void
  SetPort(const nsAString& aPort);
=======
  void SetPort(const nsAString& aPort);
>>>>>>> upstream-releases

<<<<<<< HEAD
  void SetPathname(const nsAString& aPathname);
||||||| merged common ancestors
  void
  GetPathname(nsAString& aPathname) const;
=======
  void GetPathname(nsAString& aPathname) const;
>>>>>>> upstream-releases

<<<<<<< HEAD
  void GetSearch(nsAString& aSearch) const;
||||||| merged common ancestors
  void
  SetPathname(const nsAString& aPathname);
=======
  void SetPathname(const nsAString& aPathname);
>>>>>>> upstream-releases

<<<<<<< HEAD
  virtual void SetSearch(const nsAString& aSearch);
||||||| merged common ancestors
  void
  GetSearch(nsAString& aSearch) const;

  virtual void
  SetSearch(const nsAString& aSearch);
=======
  void GetSearch(nsAString& aSearch) const;

  void SetSearch(const nsAString& aSearch);
>>>>>>> upstream-releases

  URLSearchParams* SearchParams();

  void GetHash(nsAString& aHost) const;

  void SetHash(const nsAString& aHash);

  void Stringify(nsAString& aRetval) const { GetHref(aRetval); }

  void ToJSON(nsAString& aResult) const { GetHref(aResult); }

  // URLSearchParamsObserver
  void URLSearchParamsUpdated(URLSearchParams* aSearchParams) override;

<<<<<<< HEAD
 protected:
  virtual ~URL() = default;
||||||| merged common ancestors
protected:
  virtual ~URL() = default;
=======
 private:
  ~URL() = default;
>>>>>>> upstream-releases

  void SetURI(already_AddRefed<nsIURI> aURI);

  nsIURI* GetURI() const;

  void UpdateURLSearchParams();

 private:
  void SetSearchInternal(const nsAString& aSearch);

  void CreateSearchParamsIfNeeded();

  nsCOMPtr<nsISupports> mParent;
  RefPtr<URLSearchParams> mSearchParams;
  nsCOMPtr<nsIURI> mURI;
};

bool IsChromeURI(nsIURI* aURI);

}  // namespace dom
}  // namespace mozilla

#endif /* mozilla_dom_URL_h */
