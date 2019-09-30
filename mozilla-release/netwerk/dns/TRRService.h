/* -*- Mode: C++; tab-width: 8; indent-tabs-mode: nil; c-basic-offset: 2 -*- */
/* This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/. */

#ifndef TRRService_h_
#define TRRService_h_

#include "mozilla/Atomics.h"
#include "mozilla/DataStorage.h"
#include "nsHostResolver.h"
#include "nsIObserver.h"
#include "nsWeakReference.h"

class nsIPrefBranch;

namespace mozilla {
namespace net {

class TRRService : public nsIObserver,
                   public nsITimerCallback,
                   public nsSupportsWeakReference,
                   public AHostResolver {
 public:
  NS_DECL_THREADSAFE_ISUPPORTS
  NS_DECL_NSIOBSERVER
  NS_DECL_NSITIMERCALLBACK

  TRRService();
  nsresult Init();
  nsresult Start();
  bool Enabled();

  uint32_t Mode() { return mMode; }
  bool AllowRFC1918() { return mRfc1918; }
  bool UseGET() { return mUseGET; }
  bool EarlyAAAA() { return mEarlyAAAA; }
  bool CheckIPv6Connectivity() { return mCheckIPv6Connectivity; }
  bool WaitForAllResponses() { return mWaitForAllResponses; }
  bool DisableIPv6() { return mDisableIPv6; }
  bool DisableECS() { return mDisableECS; }
  nsresult GetURI(nsCString& result);
  nsresult GetCredentials(nsCString& result);
  uint32_t GetRequestTimeout() { return mTRRTimeout; }

<<<<<<< HEAD
  LookupStatus CompleteLookup(nsHostRecord *, nsresult,
                              mozilla::net::AddrInfo *, bool pb,
                              const nsACString &aOriginSuffix) override;
  LookupStatus CompleteLookupByType(nsHostRecord *, nsresult,
                                    const nsTArray<nsCString> *, uint32_t,
                                    bool pb) override;
  void TRRBlacklist(const nsACString &host, const nsACString &originSuffix,
                    bool privateBrowsing, bool aParentsToo);
  bool IsTRRBlacklisted(const nsACString &aHost,
                        const nsACString &aOriginSuffix, bool aPrivateBrowsing,
                        bool aParentsToo);

  bool MaybeBootstrap(const nsACString &possible, nsACString &result);
  enum TrrOkay { OKAY_NORMAL = 0, OKAY_TIMEOUT = 1, OKAY_BAD = 2 };
||||||| merged common ancestors
  LookupStatus CompleteLookup(nsHostRecord *, nsresult, mozilla::net::AddrInfo *, bool pb) override;
  LookupStatus CompleteLookupByType(nsHostRecord *, nsresult, const nsTArray<nsCString> *, uint32_t, bool pb) override;
  void TRRBlacklist(const nsACString &host, bool privateBrowsing, bool aParentsToo);
  bool IsTRRBlacklisted(const nsACString &host, bool privateBrowsing, bool fullhost);

  bool MaybeBootstrap(const nsACString &possible, nsACString &result);
  enum TrrOkay {
    OKAY_NORMAL = 0,
    OKAY_TIMEOUT = 1,
    OKAY_BAD = 2
  };
=======
  LookupStatus CompleteLookup(nsHostRecord*, nsresult, mozilla::net::AddrInfo*,
                              bool pb,
                              const nsACString& aOriginSuffix) override;
  LookupStatus CompleteLookupByType(nsHostRecord*, nsresult,
                                    const nsTArray<nsCString>*, uint32_t,
                                    bool pb) override;
  void TRRBlacklist(const nsACString& host, const nsACString& originSuffix,
                    bool privateBrowsing, bool aParentsToo);
  bool IsTRRBlacklisted(const nsACString& aHost,
                        const nsACString& aOriginSuffix, bool aPrivateBrowsing,
                        bool aParentsToo);
  bool IsExcludedFromTRR(const nsACString& aHost);

  bool MaybeBootstrap(const nsACString& possible, nsACString& result);
  enum TrrOkay { OKAY_NORMAL = 0, OKAY_TIMEOUT = 1, OKAY_BAD = 2 };
>>>>>>> upstream-releases
  void TRRIsOkay(enum TrrOkay aReason);

<<<<<<< HEAD
 private:
  virtual ~TRRService();
  nsresult ReadPrefs(const char *name);
  void GetPrefBranch(nsIPrefBranch **result);
||||||| merged common ancestors
private:
  virtual  ~TRRService();
  nsresult ReadPrefs(const char *name);
  void GetPrefBranch(nsIPrefBranch **result);
=======
 private:
  virtual ~TRRService();
  nsresult ReadPrefs(const char* name);
  void GetPrefBranch(nsIPrefBranch** result);
>>>>>>> upstream-releases
  void MaybeConfirm();
  void MaybeConfirm_locked();

  bool mInitialized;
  Atomic<uint32_t, Relaxed> mMode;
  Atomic<uint32_t, Relaxed> mTRRBlacklistExpireTime;
  Atomic<uint32_t, Relaxed> mTRRTimeout;

<<<<<<< HEAD
  Mutex mLock;             // protects mPrivate* string
  nsCString mPrivateURI;   // main thread only
  nsCString mPrivateCred;  // main thread only
||||||| merged common ancestors
  Mutex mLock; // protects mPrivate* string
  nsCString mPrivateURI; // main thread only
  nsCString mPrivateCred; // main thread only
=======
  Mutex mLock;

  nsCString mPrivateURI;   // main thread only
  nsCString mPrivateCred;  // main thread only
>>>>>>> upstream-releases
  nsCString mConfirmationNS;
  nsCString mBootstrapAddr;

<<<<<<< HEAD
  Atomic<bool, Relaxed> mWaitForCaptive;  // wait for the captive portal to say
                                          // OK before using TRR
  Atomic<bool, Relaxed>
      mRfc1918;  // okay with local IP addresses in DOH responses?
  Atomic<bool, Relaxed>
      mCaptiveIsPassed;           // set when captive portal check is passed
  Atomic<bool, Relaxed> mUseGET;  // do DOH using GET requests (instead of POST)
  Atomic<bool, Relaxed> mEarlyAAAA;  // allow use of AAAA results before A is in
  Atomic<bool, Relaxed> mDisableIPv6;  // don't even try
  Atomic<bool, Relaxed> mDisableECS;   // disable EDNS Client Subnet in requests
  Atomic<uint32_t, Relaxed>
      mDisableAfterFails;  // this many fails in a row means failed TRR service
||||||| merged common ancestors
  Atomic<bool, Relaxed> mWaitForCaptive; // wait for the captive portal to say OK before using TRR
  Atomic<bool, Relaxed> mRfc1918; // okay with local IP addresses in DOH responses?
  Atomic<bool, Relaxed> mCaptiveIsPassed; // set when captive portal check is passed
  Atomic<bool, Relaxed> mUseGET; // do DOH using GET requests (instead of POST)
  Atomic<bool, Relaxed> mEarlyAAAA; // allow use of AAAA results before A is in
  Atomic<bool, Relaxed> mDisableIPv6; // don't even try
  Atomic<bool, Relaxed> mDisableECS;  // disable EDNS Client Subnet in requests
  Atomic<uint32_t, Relaxed> mDisableAfterFails;  // this many fails in a row means failed TRR service
=======
  Atomic<bool, Relaxed> mWaitForCaptive;  // wait for the captive portal to say
                                          // OK before using TRR
  Atomic<bool, Relaxed>
      mRfc1918;  // okay with local IP addresses in DOH responses?
  Atomic<bool, Relaxed>
      mCaptiveIsPassed;           // set when captive portal check is passed
  Atomic<bool, Relaxed> mUseGET;  // do DOH using GET requests (instead of POST)
  Atomic<bool, Relaxed> mEarlyAAAA;  // allow use of AAAA results before A is in
  Atomic<bool, Relaxed> mCheckIPv6Connectivity;  // check IPv6 connectivity
  Atomic<bool, Relaxed> mWaitForAllResponses;  // Don't notify until all are in
  Atomic<bool, Relaxed> mDisableIPv6;          // don't even try
  Atomic<bool, Relaxed> mDisableECS;  // disable EDNS Client Subnet in requests
  Atomic<uint32_t, Relaxed>
      mDisableAfterFails;  // this many fails in a row means failed TRR service
>>>>>>> upstream-releases

  // TRR Blacklist storage
  // mTRRBLStorage is only modified on the main thread, but we query whether it
  // is initialized or not off the main thread as well. Therefore we need to
  // lock while creating it and while accessing it off the main thread.
  RefPtr<DataStorage> mTRRBLStorage;
  Atomic<bool, Relaxed> mClearTRRBLStorage;

  // A set of domains that we should not use TRR for.
  nsTHashtable<nsCStringHashKey> mExcludedDomains;

  enum ConfirmationState {
    CONFIRM_INIT = 0,
    CONFIRM_TRYING = 1,
    CONFIRM_OK = 2,
    CONFIRM_FAILED = 3
  };
  Atomic<ConfirmationState, Relaxed> mConfirmationState;
  RefPtr<TRR> mConfirmer;
  nsCOMPtr<nsITimer> mRetryConfirmTimer;
  uint32_t mRetryConfirmInterval;  // milliseconds until retry
  Atomic<uint32_t, Relaxed> mTRRFailures;
};

extern TRRService* gTRRService;

}  // namespace net
}  // namespace mozilla

#endif  // TRRService_h_
