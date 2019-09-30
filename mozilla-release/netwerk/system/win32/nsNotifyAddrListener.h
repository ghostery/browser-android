/* -*- Mode: C++; tab-width: 4; indent-tabs-mode: nil; c-basic-offset: 2 -*- */
/* vim:set et sw=2 ts=4: */
/* This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/. */
#ifndef NSNOTIFYADDRLISTENER_H_
#define NSNOTIFYADDRLISTENER_H_

#include <windows.h>
#include <winsock2.h>
#include <iptypes.h>
#include "nsINetworkLinkService.h"
#include "nsIRunnable.h"
#include "nsIObserver.h"
#include "nsThreadUtils.h"
#include "nsCOMPtr.h"
#include "mozilla/TimeStamp.h"
#include "mozilla/Mutex.h"

class nsNotifyAddrListener : public nsINetworkLinkService,
                             public nsIRunnable,
                             public nsIObserver {
  virtual ~nsNotifyAddrListener();

 public:
  NS_DECL_THREADSAFE_ISUPPORTS
  NS_DECL_NSINETWORKLINKSERVICE
  NS_DECL_NSIRUNNABLE
  NS_DECL_NSIOBSERVER

  nsNotifyAddrListener();

<<<<<<< HEAD
  nsresult Init(void);
  void CheckLinkStatus(void);
||||||| merged common ancestors
    nsNotifyAddrListener();
=======
  nsresult Init(void);
  void CheckLinkStatus(void);

 protected:
  class ChangeEvent : public mozilla::Runnable {
   public:
    NS_DECL_NSIRUNNABLE
    ChangeEvent(nsINetworkLinkService* aService, const char* aEventID)
        : Runnable("nsNotifyAddrListener::ChangeEvent"),
          mService(aService),
          mEventID(aEventID) {}
>>>>>>> upstream-releases

<<<<<<< HEAD
 protected:
  class ChangeEvent : public mozilla::Runnable {
   public:
    NS_DECL_NSIRUNNABLE
    ChangeEvent(nsINetworkLinkService *aService, const char *aEventID)
        : Runnable("nsNotifyAddrListener::ChangeEvent"),
          mService(aService),
          mEventID(aEventID) {}
||||||| merged common ancestors
    nsresult Init(void);
    void CheckLinkStatus(void);
=======
   private:
    nsCOMPtr<nsINetworkLinkService> mService;
    const char* mEventID;
  };
>>>>>>> upstream-releases

<<<<<<< HEAD
   private:
    nsCOMPtr<nsINetworkLinkService> mService;
    const char *mEventID;
  };
||||||| merged common ancestors
protected:
    class ChangeEvent : public mozilla::Runnable {
    public:
        NS_DECL_NSIRUNNABLE
        ChangeEvent(nsINetworkLinkService *aService, const char *aEventID)
            : Runnable("nsNotifyAddrListener::ChangeEvent")
            , mService(aService)
            , mEventID(aEventID)
        {}
    private:
        nsCOMPtr<nsINetworkLinkService> mService;
        const char *mEventID;
    };
=======
  bool mLinkUp;
  bool mStatusKnown;
  bool mCheckAttempted;
>>>>>>> upstream-releases

<<<<<<< HEAD
  bool mLinkUp;
  bool mStatusKnown;
  bool mCheckAttempted;
||||||| merged common ancestors
    bool mLinkUp;
    bool mStatusKnown;
    bool mCheckAttempted;
=======
  nsresult Shutdown(void);
  nsresult SendEvent(const char* aEventID);
>>>>>>> upstream-releases

<<<<<<< HEAD
  nsresult Shutdown(void);
  nsresult SendEvent(const char *aEventID);
||||||| merged common ancestors
    nsresult Shutdown(void);
    nsresult SendEvent(const char *aEventID);
=======
  DWORD CheckAdaptersAddresses(void);
>>>>>>> upstream-releases

<<<<<<< HEAD
  DWORD CheckAdaptersAddresses(void);
||||||| merged common ancestors
    DWORD CheckAdaptersAddresses(void);
=======
  // Checks for an Internet Connection Sharing (ICS) gateway.
  bool CheckICSGateway(PIP_ADAPTER_ADDRESSES aAdapter);
  bool CheckICSStatus(PWCHAR aAdapterName);
>>>>>>> upstream-releases

<<<<<<< HEAD
  // Checks for an Internet Connection Sharing (ICS) gateway.
  bool CheckICSGateway(PIP_ADAPTER_ADDRESSES aAdapter);
  bool CheckICSStatus(PWCHAR aAdapterName);
||||||| merged common ancestors
    // Checks for an Internet Connection Sharing (ICS) gateway.
    bool  CheckICSGateway(PIP_ADAPTER_ADDRESSES aAdapter);
    bool  CheckICSStatus(PWCHAR aAdapterName);
=======
  nsCOMPtr<nsIThread> mThread;
>>>>>>> upstream-releases

<<<<<<< HEAD
  nsCOMPtr<nsIThread> mThread;
||||||| merged common ancestors
    nsCOMPtr<nsIThread> mThread;
=======
 private:
  // Returns the new timeout period for coalescing (or INFINITE)
  DWORD nextCoalesceWaitTime();
>>>>>>> upstream-releases

<<<<<<< HEAD
 private:
  // Returns the new timeout period for coalescing (or INFINITE)
  DWORD nextCoalesceWaitTime();
||||||| merged common ancestors
private:
    // Returns the new timeout period for coalescing (or INFINITE)
    DWORD nextCoalesceWaitTime();
=======
  // Called for every detected network change
  nsresult NetworkChanged();
>>>>>>> upstream-releases

<<<<<<< HEAD
  // Called for every detected network change
  nsresult NetworkChanged();
||||||| merged common ancestors
    // Called for every detected network change
    nsresult NetworkChanged();
=======
  // Figure out the current network identification
  void calculateNetworkId(void);
  bool findMac(char* gateway);
>>>>>>> upstream-releases

<<<<<<< HEAD
  // Figure out the current network identification
  void calculateNetworkId(void);
  bool findMac(char *gateway);
  nsCString mNetworkId;
||||||| merged common ancestors
    // Figure out the current network identification
    void calculateNetworkId(void);
    bool findMac(char *gateway);
    nsCString mNetworkId;
=======
  mozilla::Mutex mMutex;
  nsCString mNetworkId;
>>>>>>> upstream-releases

  HANDLE mCheckEvent;

  // set true when mCheckEvent means shutdown
  bool mShutdown;

  // This is a checksum of various meta data for all network interfaces
  // considered UP at last check.
  ULONG mIPInterfaceChecksum;

  // start time of the checking
  mozilla::TimeStamp mStartTime;

  // Network changed events are enabled
  bool mAllowChangedEvent;

  // Check for IPv6 network changes
  bool mIPv6Changes;

  // Flag set while coalescing change events
  bool mCoalescingActive;

  // Time stamp for first event during coalescing
  mozilla::TimeStamp mChangeTime;
};

#endif /* NSNOTIFYADDRLISTENER_H_ */
