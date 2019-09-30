/* -*- Mode: C++; tab-width: 2; indent-tabs-mode: nil; c-basic-offset: 2 -*- */
/* This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/. */

#ifndef mozilla_net_CookieServiceChild_h__
#define mozilla_net_CookieServiceChild_h__

#include "mozilla/net/PCookieServiceChild.h"
#include "nsClassHashtable.h"
#include "nsCookieKey.h"
#include "nsICookieService.h"
#include "nsIObserver.h"
#include "nsIPrefBranch.h"
#include "mozIThirdPartyUtil.h"
#include "nsWeakReference.h"
#include "nsThreadUtils.h"

class nsCookie;
class nsICookiePermission;
class nsIEffectiveTLDService;
class nsILoadInfo;

namespace mozilla {
namespace net {
class CookieStruct;

<<<<<<< HEAD
class CookieServiceChild : public PCookieServiceChild,
                           public nsICookieService,
                           public nsIObserver,
                           public nsITimerCallback,
                           public nsSupportsWeakReference {
 public:
||||||| merged common ancestors
class CookieServiceChild : public PCookieServiceChild
                         , public nsICookieService
                         , public nsIObserver
                         , public nsITimerCallback
                         , public nsSupportsWeakReference
{
public:
=======
class CookieServiceChild : public PCookieServiceChild,
                           public nsICookieService,
                           public nsIObserver,
                           public nsITimerCallback,
                           public nsSupportsWeakReference {
  friend class PCookieServiceChild;

 public:
>>>>>>> upstream-releases
  NS_DECL_ISUPPORTS
  NS_DECL_NSICOOKIESERVICE
  NS_DECL_NSIOBSERVER
  NS_DECL_NSITIMERCALLBACK

  typedef nsTArray<RefPtr<nsCookie>> CookiesList;
  typedef nsClassHashtable<nsCookieKey, CookiesList> CookiesMap;

  CookieServiceChild();

  static already_AddRefed<CookieServiceChild> GetSingleton();

<<<<<<< HEAD
  void TrackCookieLoad(nsIChannel *aChannel);
||||||| merged common ancestors
  void
  TrackCookieLoad(nsIChannel *aChannel);
=======
  void TrackCookieLoad(nsIChannel* aChannel);
>>>>>>> upstream-releases

 protected:
  virtual ~CookieServiceChild();
  void MoveCookies();

<<<<<<< HEAD
  void SerializeURIs(nsIURI *aHostURI, nsIChannel *aChannel,
                     nsCString &aHostSpec, nsCString &aHostCharset,
                     nsCString &aOriginatingSpec,
                     nsCString &aOriginatingCharset);

  nsresult GetCookieStringInternal(nsIURI *aHostURI, nsIChannel *aChannel,
                                   char **aCookieString);

  void GetCookieStringFromCookieHashTable(nsIURI *aHostURI, bool aIsForeign,
                                          bool aIsTrackingResource,
                                          bool aFirstPartyStorageAccessGranted,
                                          bool aIsSafeTopLevelNav,
                                          bool aIsSameSiteForeign,
                                          const OriginAttributes &aAttrs,
                                          nsCString &aCookieString);

  nsresult SetCookieStringInternal(nsIURI *aHostURI, nsIChannel *aChannel,
                                   const char *aCookieString,
                                   const char *aServerTime, bool aFromHttp);
||||||| merged common ancestors
  void SerializeURIs(nsIURI *aHostURI,
                     nsIChannel *aChannel,
                     nsCString &aHostSpec,
                     nsCString &aHostCharset,
                     nsCString &aOriginatingSpec,
                     nsCString &aOriginatingCharset);

  nsresult GetCookieStringInternal(nsIURI *aHostURI,
                                   nsIChannel *aChannel,
                                   char **aCookieString);

  void GetCookieStringFromCookieHashTable(nsIURI *aHostURI,
                                          bool aIsForeign,
                                          bool aIsTrackingResource,
                                          bool aFirstPartyStorageAccessGranted,
                                          bool aIsSafeTopLevelNav,
                                          bool aIsSameSiteForeign,
                                          const OriginAttributes &aAttrs,
                                          nsCString &aCookieString);

  nsresult SetCookieStringInternal(nsIURI *aHostURI,
                                   nsIChannel *aChannel,
                                   const char *aCookieString,
                                   const char *aServerTime,
                                   bool aFromHttp);
=======
  void SerializeURIs(nsIURI* aHostURI, nsIChannel* aChannel,
                     nsCString& aHostSpec, nsCString& aHostCharset,
                     nsCString& aOriginatingSpec,
                     nsCString& aOriginatingCharset);
>>>>>>> upstream-releases

<<<<<<< HEAD
  void RecordDocumentCookie(nsCookie *aCookie, const OriginAttributes &aAttrs);
||||||| merged common ancestors
  void
  RecordDocumentCookie(nsCookie *aCookie,
                       const OriginAttributes &aAttrs);
=======
  nsresult GetCookieStringInternal(nsIURI* aHostURI, nsIChannel* aChannel,
                                   nsACString& aCookieString);
>>>>>>> upstream-releases

<<<<<<< HEAD
  void SetCookieInternal(nsCookieAttributes &aCookieAttributes,
                         const mozilla::OriginAttributes &aAttrs,
                         nsIChannel *aChannel, bool aFromHttp,
                         nsICookiePermission *aPermissionService);
||||||| merged common ancestors
  void
  SetCookieInternal(nsCookieAttributes &aCookieAttributes,
                    const mozilla::OriginAttributes &aAttrs,
                    nsIChannel *aChannel,
                    bool aFromHttp,
                    nsICookiePermission *aPermissionService);
=======
  void GetCookieStringFromCookieHashTable(
      nsIURI* aHostURI, bool aIsForeign, bool aIsTrackingResource,
      bool aFirstPartyStorageAccessGranted, uint32_t aRejectedReason,
      bool aIsSafeTopLevelNav, bool aIsSameSiteForeign, nsIChannel* aChannel,
      nsACString& aCookieString);
>>>>>>> upstream-releases

<<<<<<< HEAD
  uint32_t CountCookiesFromHashTable(const nsCString &aBaseDomain,
                                     const OriginAttributes &aOriginAttrs);
||||||| merged common ancestors
  uint32_t
  CountCookiesFromHashTable(const nsCString &aBaseDomain,
                            const OriginAttributes &aOriginAttrs);
=======
  nsresult SetCookieStringInternal(nsIURI* aHostURI, nsIChannel* aChannel,
                                   const nsACString& aCookieString,
                                   const nsACString& aServerTime,
                                   bool aFromHttp);
>>>>>>> upstream-releases

  void RecordDocumentCookie(nsCookie* aCookie, const OriginAttributes& aAttrs);

  void SetCookieInternal(const CookieStruct& aCookieData,
                         const mozilla::OriginAttributes& aAttrs,
                         nsIChannel* aChannel, bool aFromHttp,
                         nsICookiePermission* aPermissionService);

<<<<<<< HEAD
  virtual mozilla::ipc::IPCResult RecvTrackCookiesLoad(
      nsTArray<CookieStruct> &&aCookiesList,
      const OriginAttributes &aAttrs) override;
||||||| merged common ancestors
  virtual
  mozilla::ipc::IPCResult RecvTrackCookiesLoad(nsTArray<CookieStruct>&& aCookiesList,
                                               const OriginAttributes &aAttrs) override;
=======
  uint32_t CountCookiesFromHashTable(const nsCString& aBaseDomain,
                                     const OriginAttributes& aOriginAttrs);
>>>>>>> upstream-releases

<<<<<<< HEAD
  virtual mozilla::ipc::IPCResult RecvRemoveAll() override;
||||||| merged common ancestors
  virtual
  mozilla::ipc::IPCResult RecvRemoveAll() override;
=======
  void PrefChanged(nsIPrefBranch* aPrefBranch);
>>>>>>> upstream-releases

<<<<<<< HEAD
  virtual mozilla::ipc::IPCResult RecvRemoveBatchDeletedCookies(
      nsTArray<CookieStruct> &&aCookiesList,
      nsTArray<OriginAttributes> &&aAttrsList) override;
||||||| merged common ancestors
  virtual mozilla::ipc::IPCResult
  RecvRemoveBatchDeletedCookies(nsTArray<CookieStruct>&& aCookiesList,
                                nsTArray<OriginAttributes>&& aAttrsList) override;
=======
  bool RequireThirdPartyCheck(nsILoadInfo* aLoadInfo);
>>>>>>> upstream-releases

<<<<<<< HEAD
  virtual mozilla::ipc::IPCResult RecvRemoveCookie(
      const CookieStruct &aCookie, const OriginAttributes &aAttrs) override;
||||||| merged common ancestors
  virtual
  mozilla::ipc::IPCResult RecvRemoveCookie(const CookieStruct &aCookie,
                                           const OriginAttributes &aAttrs) override;
=======
  mozilla::ipc::IPCResult RecvTrackCookiesLoad(
      nsTArray<CookieStruct>&& aCookiesList, const OriginAttributes& aAttrs);
>>>>>>> upstream-releases

<<<<<<< HEAD
  virtual mozilla::ipc::IPCResult RecvAddCookie(
      const CookieStruct &aCookie, const OriginAttributes &aAttrs) override;
||||||| merged common ancestors
  virtual
  mozilla::ipc::IPCResult RecvAddCookie(const CookieStruct &aCookie,
                                        const OriginAttributes &aAttrs) override;
=======
  mozilla::ipc::IPCResult RecvRemoveAll();
>>>>>>> upstream-releases

  mozilla::ipc::IPCResult RecvRemoveBatchDeletedCookies(
      nsTArray<CookieStruct>&& aCookiesList,
      nsTArray<OriginAttributes>&& aAttrsList);

  mozilla::ipc::IPCResult RecvRemoveCookie(const CookieStruct& aCookie,
                                           const OriginAttributes& aAttrs);

  mozilla::ipc::IPCResult RecvAddCookie(const CookieStruct& aCookie,
                                        const OriginAttributes& aAttrs);

  CookiesMap mCookiesMap;
  nsCOMPtr<nsITimer> mCookieTimer;
  nsCOMPtr<mozIThirdPartyUtil> mThirdPartyUtil;
  nsCOMPtr<nsIEffectiveTLDService> mTLDService;
};

}  // namespace net
}  // namespace mozilla

#endif  // mozilla_net_CookieServiceChild_h__
