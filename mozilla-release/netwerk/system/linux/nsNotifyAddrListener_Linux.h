/* -*- Mode: C++; tab-width: 4; indent-tabs-mode: nil; c-basic-offset: 2 -*- */
/* vim:set et sw=2 ts=4: */
/* This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/. */
#ifndef NSNOTIFYADDRLISTENER_LINUX_H_
#define NSNOTIFYADDRLISTENER_LINUX_H_

#include <sys/socket.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <linux/netlink.h>
#include <linux/rtnetlink.h>
#include <arpa/inet.h>
#include <unistd.h>

#include "nsINetworkLinkService.h"
#include "nsIRunnable.h"
#include "nsIObserver.h"
#include "nsThreadUtils.h"
#include "nsCOMPtr.h"
#include "mozilla/Atomics.h"
#include "mozilla/Mutex.h"
#include "mozilla/TimeStamp.h"
#include "nsITimer.h"
#include "nsClassHashtable.h"

class nsNotifyAddrListener : public nsINetworkLinkService,
                             public nsIRunnable,
                             public nsIObserver {
  virtual ~nsNotifyAddrListener();

<<<<<<< HEAD
 public:
  NS_DECL_THREADSAFE_ISUPPORTS
  NS_DECL_NSINETWORKLINKSERVICE
  NS_DECL_NSIRUNNABLE
  NS_DECL_NSIOBSERVER

  nsNotifyAddrListener();
  nsresult Init(void);
||||||| merged common ancestors
public:
    NS_DECL_THREADSAFE_ISUPPORTS
    NS_DECL_NSINETWORKLINKSERVICE
    NS_DECL_NSIRUNNABLE
    NS_DECL_NSIOBSERVER
=======
 public:
  NS_DECL_THREADSAFE_ISUPPORTS
  NS_DECL_NSINETWORKLINKSERVICE
  NS_DECL_NSIRUNNABLE
  NS_DECL_NSIOBSERVER

  nsNotifyAddrListener();
  nsresult Init(void);

 private:
  class ChangeEvent : public mozilla::Runnable {
   public:
    NS_DECL_NSIRUNNABLE
    ChangeEvent(nsINetworkLinkService* aService, const char* aEventID)
        : mozilla::Runnable("nsNotifyAddrListener::ChangeEvent"),
          mService(aService),
          mEventID(aEventID) {}
>>>>>>> upstream-releases

<<<<<<< HEAD
 private:
  class ChangeEvent : public mozilla::Runnable {
   public:
    NS_DECL_NSIRUNNABLE
    ChangeEvent(nsINetworkLinkService* aService, const char* aEventID)
        : mozilla::Runnable("nsNotifyAddrListener::ChangeEvent"),
          mService(aService),
          mEventID(aEventID) {}
||||||| merged common ancestors
    nsNotifyAddrListener();
    nsresult Init(void);
=======
   private:
    nsCOMPtr<nsINetworkLinkService> mService;
    const char* mEventID;
  };
>>>>>>> upstream-releases

<<<<<<< HEAD
   private:
    nsCOMPtr<nsINetworkLinkService> mService;
    const char* mEventID;
  };
||||||| merged common ancestors
private:
    class ChangeEvent : public mozilla::Runnable {
    public:
        NS_DECL_NSIRUNNABLE
        ChangeEvent(nsINetworkLinkService* aService, const char* aEventID)
          : mozilla::Runnable("nsNotifyAddrListener::ChangeEvent")
          , mService(aService)
          , mEventID(aEventID)
        {
        }
    private:
        nsCOMPtr<nsINetworkLinkService> mService;
        const char *mEventID;
    };
=======
  // Called when xpcom-shutdown-threads is received.
  nsresult Shutdown(void);
>>>>>>> upstream-releases

<<<<<<< HEAD
  // Called when xpcom-shutdown-threads is received.
  nsresult Shutdown(void);
||||||| merged common ancestors
    // Called when xpcom-shutdown-threads is received.
    nsresult Shutdown(void);
=======
  // Called when a network change was detected
  nsresult NetworkChanged();
>>>>>>> upstream-releases

<<<<<<< HEAD
  // Called when a network change was detected
  nsresult NetworkChanged();
||||||| merged common ancestors
    // Called when a network change was detected
    nsresult NetworkChanged();
=======
  // Sends the network event.
  nsresult SendEvent(const char* aEventID);
>>>>>>> upstream-releases

<<<<<<< HEAD
  // Sends the network event.
  nsresult SendEvent(const char* aEventID);
||||||| merged common ancestors
    // Sends the network event.
    nsresult SendEvent(const char *aEventID);
=======
  // Figure out the current "network identification"
  void calculateNetworkId(void);
>>>>>>> upstream-releases

<<<<<<< HEAD
  // Figure out the current "network identification"
  void calculateNetworkId(void);
  nsCString mNetworkId;
||||||| merged common ancestors
    // Figure out the current "network identification"
    void calculateNetworkId(void);
    nsCString mNetworkId;
=======
  mozilla::Mutex mMutex;
  nsCString mNetworkId;
>>>>>>> upstream-releases

  // Checks if there's a network "link"
  void checkLink(void);

  // Deals with incoming NETLINK messages.
  void OnNetlinkMessage(int NetlinkSocket);

  nsCOMPtr<nsIThread> mThread;

  // The network is up.
  bool mLinkUp;

  // The network's up/down status is known.
  bool mStatusKnown;

  // A pipe to signal shutdown with.
  int mShutdownPipe[2];

  // Network changed events are enabled
  bool mAllowChangedEvent;

  // Flag set while coalescing change events
  bool mCoalescingActive;

  // Time stamp for first event during coalescing
  mozilla::TimeStamp mChangeTime;

  // Seen Ip addresses. For Ipv6 addresses some time router renews their
  // lifetime and we should not detect this as a network link change, so we
  // keep info about all seen addresses.
  nsClassHashtable<nsCStringHashKey, struct ifaddrmsg> mAddressInfo;
};

#endif /* NSNOTIFYADDRLISTENER_LINUX_H_ */
