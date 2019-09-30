/* -*- Mode: C++; tab-width: 4; indent-tabs-mode: nil; c-basic-offset: 2 -*- */
/* vim:set et sw=2 ts=4: */
/* This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/. */

// We define this to make our use of inet_ntoa() pass. The "proper" function
// inet_ntop() doesn't exist on Windows XP.
#define _WINSOCK_DEPRECATED_NO_WARNINGS

#include <stdarg.h>
#include <windef.h>
#include <winbase.h>
#include <wingdi.h>
#include <winuser.h>
#include <ole2.h>
#include <netcon.h>
#include <objbase.h>
#include <winsock2.h>
#include <ws2ipdef.h>
#include <tcpmib.h>
#include <iphlpapi.h>
#include <netioapi.h>
#include <netlistmgr.h>
#include <iprtrmib.h>
#include "plstr.h"
#include "mozilla/Logging.h"
#include "nsThreadUtils.h"
#include "nsIObserverService.h"
#include "nsServiceManagerUtils.h"
#include "nsNotifyAddrListener.h"
#include "nsString.h"
#include "nsPrintfCString.h"
#include "nsAutoPtr.h"
#include "mozilla/Services.h"
#include "nsCRT.h"
#include "mozilla/Preferences.h"
#include "mozilla/SHA1.h"
#include "mozilla/Base64.h"
#include "mozilla/ScopeExit.h"
#include "mozilla/Telemetry.h"
#include <iptypes.h>
#include <iphlpapi.h>

using namespace mozilla;

static LazyLogModule gNotifyAddrLog("nsNotifyAddr");
#define LOG(args) MOZ_LOG(gNotifyAddrLog, mozilla::LogLevel::Debug, args)

static HMODULE sNetshell;
static decltype(NcFreeNetconProperties) *sNcFreeNetconProperties;

static HMODULE sIphlpapi;
static decltype(NotifyIpInterfaceChange) *sNotifyIpInterfaceChange;
static decltype(CancelMibChangeNotify2) *sCancelMibChangeNotify2;

#define NETWORK_NOTIFY_CHANGED_PREF "network.notify.changed"
#define NETWORK_NOTIFY_IPV6_PREF "network.notify.IPv6"

// period during which to absorb subsequent network change events, in
// milliseconds
static const unsigned int kNetworkChangeCoalescingPeriod = 1000;

<<<<<<< HEAD
static void InitIphlpapi(void) {
  if (!sIphlpapi) {
    sIphlpapi = LoadLibraryW(L"Iphlpapi.dll");
    if (sIphlpapi) {
      sNotifyIpInterfaceChange =
          (decltype(NotifyIpInterfaceChange) *)GetProcAddress(
              sIphlpapi, "NotifyIpInterfaceChange");
      sCancelMibChangeNotify2 =
          (decltype(CancelMibChangeNotify2) *)GetProcAddress(
              sIphlpapi, "CancelMibChangeNotify2");
    } else {
      NS_WARNING(
          "Failed to load Iphlpapi.dll - cannot detect network"
          " changes!");
||||||| merged common ancestors
static void InitNetshellLibrary(void)
{
    if (!sNetshell) {
        sNetshell = LoadLibraryW(L"Netshell.dll");
        if (sNetshell) {
            sNcFreeNetconProperties = (decltype(NcFreeNetconProperties)*)
                GetProcAddress(sNetshell, "NcFreeNetconProperties");
        }
=======
static void InitIphlpapi(void) {
  if (!sIphlpapi) {
    sIphlpapi = LoadLibraryW(L"Iphlpapi.dll");
    if (sIphlpapi) {
      sNotifyIpInterfaceChange =
          (decltype(NotifyIpInterfaceChange)*)GetProcAddress(
              sIphlpapi, "NotifyIpInterfaceChange");
      sCancelMibChangeNotify2 =
          (decltype(CancelMibChangeNotify2)*)GetProcAddress(
              sIphlpapi, "CancelMibChangeNotify2");
    } else {
      NS_WARNING(
          "Failed to load Iphlpapi.dll - cannot detect network"
          " changes!");
>>>>>>> upstream-releases
    }
  }
}

static void InitNetshellLibrary(void) {
  if (!sNetshell) {
    sNetshell = LoadLibraryW(L"Netshell.dll");
    if (sNetshell) {
<<<<<<< HEAD
      sNcFreeNetconProperties =
          (decltype(NcFreeNetconProperties) *)GetProcAddress(
              sNetshell, "NcFreeNetconProperties");
||||||| merged common ancestors
        sNcFreeNetconProperties = nullptr;
        FreeLibrary(sNetshell);
        sNetshell = nullptr;
    }
    if (sIphlpapi) {
        sNotifyIpInterfaceChange = nullptr;
        sCancelMibChangeNotify2 = nullptr;
        FreeLibrary(sIphlpapi);
        sIphlpapi = nullptr;
=======
      sNcFreeNetconProperties =
          (decltype(NcFreeNetconProperties)*)GetProcAddress(
              sNetshell, "NcFreeNetconProperties");
>>>>>>> upstream-releases
    }
<<<<<<< HEAD
  }
}

static void FreeDynamicLibraries(void) {
  if (sNetshell) {
    sNcFreeNetconProperties = nullptr;
    FreeLibrary(sNetshell);
    sNetshell = nullptr;
  }
  if (sIphlpapi) {
    sNotifyIpInterfaceChange = nullptr;
    sCancelMibChangeNotify2 = nullptr;
    FreeLibrary(sIphlpapi);
    sIphlpapi = nullptr;
  }
||||||| merged common ancestors
=======
  }
>>>>>>> upstream-releases
}

<<<<<<< HEAD
NS_IMPL_ISUPPORTS(nsNotifyAddrListener, nsINetworkLinkService, nsIRunnable,
||||||| merged common ancestors
NS_IMPL_ISUPPORTS(nsNotifyAddrListener,
                  nsINetworkLinkService,
                  nsIRunnable,
=======
static void FreeDynamicLibraries(void) {
  if (sNetshell) {
    sNcFreeNetconProperties = nullptr;
    FreeLibrary(sNetshell);
    sNetshell = nullptr;
  }
  if (sIphlpapi) {
    sNotifyIpInterfaceChange = nullptr;
    sCancelMibChangeNotify2 = nullptr;
    FreeLibrary(sIphlpapi);
    sIphlpapi = nullptr;
  }
}

NS_IMPL_ISUPPORTS(nsNotifyAddrListener, nsINetworkLinkService, nsIRunnable,
>>>>>>> upstream-releases
                  nsIObserver)

nsNotifyAddrListener::nsNotifyAddrListener()
<<<<<<< HEAD
    : mLinkUp(true)  // assume true by default
      ,
      mStatusKnown(false),
      mCheckAttempted(false),
      mCheckEvent(nullptr),
      mShutdown(false),
      mIPInterfaceChecksum(0),
      mAllowChangedEvent(true),
      mIPv6Changes(false),
      mCoalescingActive(false) {
  InitIphlpapi();
||||||| merged common ancestors
    : mLinkUp(true)  // assume true by default
    , mStatusKnown(false)
    , mCheckAttempted(false)
    , mCheckEvent(nullptr)
    , mShutdown(false)
    , mIPInterfaceChecksum(0)
    , mAllowChangedEvent(true)
    , mIPv6Changes(false)
    , mCoalescingActive(false)
{
    InitIphlpapi();
=======
    : mLinkUp(true),  // assume true by default
      mStatusKnown(false),
      mCheckAttempted(false),
      mMutex("nsNotifyAddrListener::mMutex"),
      mCheckEvent(nullptr),
      mShutdown(false),
      mIPInterfaceChecksum(0),
      mAllowChangedEvent(true),
      mIPv6Changes(false),
      mCoalescingActive(false) {
  InitIphlpapi();
>>>>>>> upstream-releases
}

nsNotifyAddrListener::~nsNotifyAddrListener() {
  NS_ASSERTION(!mThread, "nsNotifyAddrListener thread shutdown failed");
  FreeDynamicLibraries();
}

NS_IMETHODIMP
<<<<<<< HEAD
nsNotifyAddrListener::GetIsLinkUp(bool *aIsUp) {
  if (!mCheckAttempted && !mStatusKnown) {
    mCheckAttempted = true;
    CheckLinkStatus();
  }

  *aIsUp = mLinkUp;
  return NS_OK;
||||||| merged common ancestors
nsNotifyAddrListener::GetIsLinkUp(bool *aIsUp)
{
    if (!mCheckAttempted && !mStatusKnown) {
        mCheckAttempted = true;
        CheckLinkStatus();
    }

    *aIsUp = mLinkUp;
    return NS_OK;
=======
nsNotifyAddrListener::GetIsLinkUp(bool* aIsUp) {
  if (!mCheckAttempted && !mStatusKnown) {
    mCheckAttempted = true;
    CheckLinkStatus();
  }

  *aIsUp = mLinkUp;
  return NS_OK;
>>>>>>> upstream-releases
}

NS_IMETHODIMP
<<<<<<< HEAD
nsNotifyAddrListener::GetLinkStatusKnown(bool *aIsUp) {
  *aIsUp = mStatusKnown;
  return NS_OK;
||||||| merged common ancestors
nsNotifyAddrListener::GetLinkStatusKnown(bool *aIsUp)
{
    *aIsUp = mStatusKnown;
    return NS_OK;
=======
nsNotifyAddrListener::GetLinkStatusKnown(bool* aIsUp) {
  *aIsUp = mStatusKnown;
  return NS_OK;
>>>>>>> upstream-releases
}

NS_IMETHODIMP
<<<<<<< HEAD
nsNotifyAddrListener::GetLinkType(uint32_t *aLinkType) {
  NS_ENSURE_ARG_POINTER(aLinkType);

  // XXX This function has not yet been implemented for this platform
  *aLinkType = nsINetworkLinkService::LINK_TYPE_UNKNOWN;
  return NS_OK;
}

static bool macAddr(BYTE addr[], DWORD len, char *buf, size_t buflen) {
  buf[0] = '\0';
  if (!addr || !len || (len * 3 > buflen)) {
    return false;
  }

  for (DWORD i = 0; i < len; ++i) {
    sprintf_s(buf + (i * 3), sizeof(buf + (i * 3)), "%02x%s", addr[i],
              (i == len - 1) ? "" : ":");
  }
  return true;
}
||||||| merged common ancestors
nsNotifyAddrListener::GetLinkType(uint32_t *aLinkType)
{
    NS_ENSURE_ARG_POINTER(aLinkType);

    // XXX This function has not yet been implemented for this platform
    *aLinkType = nsINetworkLinkService::LINK_TYPE_UNKNOWN;
    return NS_OK;
}

static bool macAddr(BYTE addr[], DWORD len, char *buf, size_t buflen)
{
    buf[0] = '\0';
    if (!addr || !len || (len * 3 > buflen)) {
        return false;
    }

    for (DWORD i = 0; i < len; ++i) {
        sprintf_s(buf + (i * 3), sizeof(buf + (i * 3)),
                  "%02x%s", addr[i], (i == len-1) ? "" : ":");
    }
    return true;
}
=======
nsNotifyAddrListener::GetLinkType(uint32_t* aLinkType) {
  NS_ENSURE_ARG_POINTER(aLinkType);
>>>>>>> upstream-releases

<<<<<<< HEAD
bool nsNotifyAddrListener::findMac(char *gateway) {
  // query for buffer size needed
  DWORD dwActualSize = 0;
  bool found = FALSE;

  // GetIpNetTable gets the IPv4 to physical address mapping table
  DWORD status = GetIpNetTable(NULL, &dwActualSize, FALSE);
  if (status == ERROR_INSUFFICIENT_BUFFER) {
    // the expected route, now with a known buffer size
    UniquePtr<char[]> buf(new char[dwActualSize]);
    PMIB_IPNETTABLE pIpNetTable = reinterpret_cast<PMIB_IPNETTABLE>(&buf[0]);

    status = GetIpNetTable(pIpNetTable, &dwActualSize, FALSE);

    if (status == NO_ERROR) {
      for (DWORD i = 0; i < pIpNetTable->dwNumEntries; ++i) {
        char hw[256];

        if (!macAddr(pIpNetTable->table[i].bPhysAddr,
                     pIpNetTable->table[i].dwPhysAddrLen, hw, sizeof(hw))) {
          // failed to get the MAC
          continue;
        }

        struct in_addr addr;
        addr.s_addr = pIpNetTable->table[i].dwAddr;

        if (!strcmp(gateway, inet_ntoa(addr))) {
          LOG(("networkid: MAC %s\n", hw));
          nsAutoCString mac(hw);
          // This 'addition' could potentially be a
          // fixed number from the profile or something.
          nsAutoCString addition("local-rubbish");
          nsAutoCString output;
          SHA1Sum sha1;
          nsCString combined(mac + addition);
          sha1.update(combined.get(), combined.Length());
          uint8_t digest[SHA1Sum::kHashSize];
          sha1.finish(digest);
          nsCString newString(reinterpret_cast<char *>(digest),
                              SHA1Sum::kHashSize);
          nsresult rv = Base64Encode(newString, output);
          if (NS_FAILED(rv)) {
            found = false;
            break;
          }
          LOG(("networkid: id %s\n", output.get()));
          if (mNetworkId != output) {
            // new id
            Telemetry::Accumulate(Telemetry::NETWORK_ID, 1);
            mNetworkId = output;
          } else {
            // same id
            Telemetry::Accumulate(Telemetry::NETWORK_ID, 2);
          }
          found = true;
          break;
        }
      }
    }
  }
  return found;
||||||| merged common ancestors
bool nsNotifyAddrListener::findMac(char *gateway)
{
    // query for buffer size needed
    DWORD dwActualSize = 0;
    bool found = FALSE;

    // GetIpNetTable gets the IPv4 to physical address mapping table
    DWORD status = GetIpNetTable(NULL, &dwActualSize, FALSE);
    if (status == ERROR_INSUFFICIENT_BUFFER) {
        // the expected route, now with a known buffer size
        UniquePtr <char[]>buf(new char[dwActualSize]);
        PMIB_IPNETTABLE pIpNetTable =
            reinterpret_cast<PMIB_IPNETTABLE>(&buf[0]);

        status = GetIpNetTable(pIpNetTable, &dwActualSize, FALSE);

        if (status == NO_ERROR) {
            for (DWORD i = 0; i < pIpNetTable->dwNumEntries; ++i) {
                char hw[256];

                if (!macAddr(pIpNetTable->table[i].bPhysAddr,
                             pIpNetTable->table[i].dwPhysAddrLen,
                             hw, sizeof(hw))) {
                    // failed to get the MAC
                    continue;
                }

                struct in_addr addr;
                addr.s_addr = pIpNetTable->table[i].dwAddr;

                if (!strcmp(gateway, inet_ntoa(addr))) {
                    LOG(("networkid: MAC %s\n", hw));
                    nsAutoCString mac(hw);
                    // This 'addition' could potentially be a
                    // fixed number from the profile or something.
                    nsAutoCString addition("local-rubbish");
                    nsAutoCString output;
                    SHA1Sum sha1;
                    nsCString combined(mac + addition);
                    sha1.update(combined.get(), combined.Length());
                    uint8_t digest[SHA1Sum::kHashSize];
                    sha1.finish(digest);
                    nsCString newString(reinterpret_cast<char*>(digest),
                                        SHA1Sum::kHashSize);
                    nsresult rv = Base64Encode(newString, output);
                    if (NS_FAILED(rv)) {
                        found = false;
                        break;
                    }
                    LOG(("networkid: id %s\n", output.get()));
                    if (mNetworkId != output) {
                        // new id
                        Telemetry::Accumulate(Telemetry::NETWORK_ID, 1);
                        mNetworkId = output;
                    }
                    else {
                        // same id
                        Telemetry::Accumulate(Telemetry::NETWORK_ID, 2);
                    }
                    found = true;
                    break;
                }
            }
        }
    }
    return found;
=======
  // XXX This function has not yet been implemented for this platform
  *aLinkType = nsINetworkLinkService::LINK_TYPE_UNKNOWN;
  return NS_OK;
>>>>>>> upstream-releases
}

<<<<<<< HEAD
// returns 'true' when the gw is found and stored
static bool defaultgw(char *aGateway, size_t aGatewayLen) {
  PMIB_IPFORWARDTABLE pIpForwardTable = NULL;

  DWORD dwSize = 0;
  if (GetIpForwardTable(NULL, &dwSize, 0) != ERROR_INSUFFICIENT_BUFFER) {
    return false;
  }

  UniquePtr<char[]> buf(new char[dwSize]);
  pIpForwardTable = reinterpret_cast<PMIB_IPFORWARDTABLE>(&buf[0]);

  // Note that the IPv4 addresses returned in GetIpForwardTable entries are
  // in network byte order

  DWORD retVal = GetIpForwardTable(pIpForwardTable, &dwSize, 0);
  if (retVal == NO_ERROR) {
    for (unsigned int i = 0; i < pIpForwardTable->dwNumEntries; ++i) {
      // Convert IPv4 addresses to strings
      struct in_addr IpAddr;
      IpAddr.S_un.S_addr =
          static_cast<u_long>(pIpForwardTable->table[i].dwForwardDest);
      char *ipStr = inet_ntoa(IpAddr);
      if (ipStr && !strcmp("0.0.0.0", ipStr)) {
        // Default gateway!
        IpAddr.S_un.S_addr =
            static_cast<u_long>(pIpForwardTable->table[i].dwForwardNextHop);
        ipStr = inet_ntoa(IpAddr);
        if (ipStr) {
          strcpy_s(aGateway, aGatewayLen, ipStr);
          return true;
        }
      }
    }  // for loop
  }

  return false;
||||||| merged common ancestors
// returns 'true' when the gw is found and stored
static bool defaultgw(char *aGateway, size_t aGatewayLen)
{
    PMIB_IPFORWARDTABLE pIpForwardTable = NULL;

    DWORD dwSize = 0;
    if (GetIpForwardTable(NULL, &dwSize, 0) != ERROR_INSUFFICIENT_BUFFER) {
        return false;
    }

    UniquePtr <char[]>buf(new char[dwSize]);
    pIpForwardTable = reinterpret_cast<PMIB_IPFORWARDTABLE>(&buf[0]);

    // Note that the IPv4 addresses returned in GetIpForwardTable entries are
    // in network byte order

    DWORD retVal = GetIpForwardTable(pIpForwardTable, &dwSize, 0);
    if (retVal == NO_ERROR) {
        for (unsigned int i = 0; i < pIpForwardTable->dwNumEntries; ++i) {
            // Convert IPv4 addresses to strings
            struct in_addr IpAddr;
            IpAddr.S_un.S_addr = static_cast<u_long>
                (pIpForwardTable->table[i].dwForwardDest);
            char *ipStr = inet_ntoa(IpAddr);
            if (ipStr && !strcmp("0.0.0.0", ipStr)) {
                // Default gateway!
                IpAddr.S_un.S_addr = static_cast<u_long>
                    (pIpForwardTable->table[i].dwForwardNextHop);
                ipStr = inet_ntoa(IpAddr);
                if (ipStr) {
                    strcpy_s(aGateway, aGatewayLen, ipStr);
                    return true;
                }
            }
        } // for loop
    }

    return false;
=======
NS_IMETHODIMP
nsNotifyAddrListener::GetNetworkID(nsACString& aNetworkID) {
  MutexAutoLock lock(mMutex);
  aNetworkID = mNetworkId;
  return NS_OK;
>>>>>>> upstream-releases
}

//
// Figure out the current "network identification" string.
//
<<<<<<< HEAD
// It detects the IP of the default gateway in the routing table, then the MAC
// address of that IP in the ARP table before it hashes that string (to avoid
// information leakage).
//
void nsNotifyAddrListener::calculateNetworkId(void) {
  bool found = FALSE;
  char gateway[128];
  if (defaultgw(gateway, sizeof(gateway))) {
    found = findMac(gateway);
  }
  if (!found) {
    // no id
    Telemetry::Accumulate(Telemetry::NETWORK_ID, 0);
  }
||||||| merged common ancestors
// It detects the IP of the default gateway in the routing table, then the MAC
// address of that IP in the ARP table before it hashes that string (to avoid
// information leakage).
//
void nsNotifyAddrListener::calculateNetworkId(void)
{
    bool found = FALSE;
    char gateway[128];
    if (defaultgw(gateway, sizeof(gateway) )) {
        found = findMac(gateway);
    }
    if (!found) {
        // no id
        Telemetry::Accumulate(Telemetry::NETWORK_ID, 0);
    }
=======
void nsNotifyAddrListener::calculateNetworkId(void) {
  MOZ_ASSERT(!NS_IsMainThread(), "Must not be called on the main thread");

  if (FAILED(CoInitializeEx(nullptr, COINIT_MULTITHREADED))) {
    return;
  }

  auto unitialize = MakeScopeExit([]() { CoUninitialize(); });

  RefPtr<INetworkListManager> nlm;
  HRESULT hr = CoCreateInstance(CLSID_NetworkListManager, nullptr, CLSCTX_ALL,
                                IID_INetworkListManager, getter_AddRefs(nlm));
  if (NS_WARN_IF(FAILED(hr))) {
    LOG(("CoCreateInstance error: %X", hr));
    return;
  }
  RefPtr<IEnumNetworks> enumNetworks;
  hr = nlm->GetNetworks(NLM_ENUM_NETWORK_CONNECTED,
                        getter_AddRefs(enumNetworks));
  if (NS_WARN_IF(FAILED(hr))) {
    LOG(("GetNetworks error: %X", hr));
    return;
  }

  SHA1Sum sha1;
  uint32_t networkCount = 0;
  while (true) {
    RefPtr<INetwork> network;
    hr = enumNetworks->Next(1, getter_AddRefs(network), nullptr);
    if (hr != S_OK) {
      break;
    }

    GUID nwGUID;
    hr = network->GetNetworkId(&nwGUID);
    if (hr != S_OK) {
      continue;
    }
    networkCount++;
    sha1.update(&nwGUID, sizeof(nwGUID));

    nsPrintfCString guid("%08lX%04X%04X%02X%02X%02X%02X%02X%02X%02X%02X%lX",
                         nwGUID.Data1, nwGUID.Data2, nwGUID.Data3,
                         nwGUID.Data4[0], nwGUID.Data4[1], nwGUID.Data4[2],
                         nwGUID.Data4[3], nwGUID.Data4[4], nwGUID.Data4[5],
                         nwGUID.Data4[6], nwGUID.Data4[7]);
    LOG(("calculateNetworkId: interface networkID: %s\n", guid.get()));
  }

  if (networkCount == 0) {
    MutexAutoLock lock(mMutex);
    mNetworkId.Truncate();
    LOG(("calculateNetworkId: no network ID - no active networks"));
    Telemetry::Accumulate(Telemetry::NETWORK_ID2, 0);
    return;
  }

  nsAutoCString output;
  SHA1Sum::Hash digest;
  sha1.finish(digest);
  nsCString newString(reinterpret_cast<char*>(digest), SHA1Sum::kHashSize);
  nsresult rv = Base64Encode(newString, output);
  if (NS_FAILED(rv)) {
    {
      MutexAutoLock lock(mMutex);
      mNetworkId.Truncate();
    }
    Telemetry::Accumulate(Telemetry::NETWORK_ID2, 0);
    LOG(("calculateNetworkId: no network ID Base64Encode error %X", rv));
    return;
  }

  MutexAutoLock lock(mMutex);
  if (output != mNetworkId) {
    mNetworkId = output;
    Telemetry::Accumulate(Telemetry::NETWORK_ID2, 1);
    LOG(("calculateNetworkId: new NetworkID: %s", output.get()));
  } else {
    Telemetry::Accumulate(Telemetry::NETWORK_ID2, 2);
    LOG(("calculateNetworkId: same NetworkID: %s", output.get()));
  }
>>>>>>> upstream-releases
}

// Static Callback function for NotifyIpInterfaceChange API.
static void WINAPI OnInterfaceChange(PVOID callerContext,
                                     PMIB_IPINTERFACE_ROW row,
<<<<<<< HEAD
                                     MIB_NOTIFICATION_TYPE notificationType) {
  nsNotifyAddrListener *notify =
      static_cast<nsNotifyAddrListener *>(callerContext);
  notify->CheckLinkStatus();
||||||| merged common ancestors
                                     MIB_NOTIFICATION_TYPE notificationType)
{
    nsNotifyAddrListener *notify = static_cast<nsNotifyAddrListener*>(callerContext);
    notify->CheckLinkStatus();
=======
                                     MIB_NOTIFICATION_TYPE notificationType) {
  nsNotifyAddrListener* notify =
      static_cast<nsNotifyAddrListener*>(callerContext);
  notify->CheckLinkStatus();
>>>>>>> upstream-releases
}

DWORD
<<<<<<< HEAD
nsNotifyAddrListener::nextCoalesceWaitTime() {
  // check if coalescing period should continue
  double period = (TimeStamp::Now() - mChangeTime).ToMilliseconds();
  if (period >= kNetworkChangeCoalescingPeriod) {
    calculateNetworkId();
    SendEvent(NS_NETWORK_LINK_DATA_CHANGED);
    mCoalescingActive = false;
    return INFINITE;  // return default
  } else {
    // wait no longer than to the end of the period
    return static_cast<DWORD>(kNetworkChangeCoalescingPeriod - period);
  }
||||||| merged common ancestors
nsNotifyAddrListener::nextCoalesceWaitTime()
{
    // check if coalescing period should continue
    double period = (TimeStamp::Now() - mChangeTime).ToMilliseconds();
    if (period >= kNetworkChangeCoalescingPeriod) {
        calculateNetworkId();
        SendEvent(NS_NETWORK_LINK_DATA_CHANGED);
        mCoalescingActive = false;
        return INFINITE; // return default
    } else {
        // wait no longer than to the end of the period
        return static_cast<DWORD>
            (kNetworkChangeCoalescingPeriod - period);
    }
=======
nsNotifyAddrListener::nextCoalesceWaitTime() {
  // check if coalescing period should continue
  double period = (TimeStamp::Now() - mChangeTime).ToMilliseconds();
  if (period >= kNetworkChangeCoalescingPeriod) {
    SendEvent(NS_NETWORK_LINK_DATA_CHANGED);
    mCoalescingActive = false;
    return INFINITE;  // return default
  } else {
    // wait no longer than to the end of the period
    return static_cast<DWORD>(kNetworkChangeCoalescingPeriod - period);
  }
>>>>>>> upstream-releases
}

NS_IMETHODIMP
nsNotifyAddrListener::Run() {
  mStartTime = TimeStamp::Now();

  calculateNetworkId();

  DWORD waitTime = INFINITE;

  if (!sNotifyIpInterfaceChange || !sCancelMibChangeNotify2 || !mIPv6Changes) {
    // For Windows versions which are older than Vista which lack
    // NotifyIpInterfaceChange. Note this means no IPv6 support.
    HANDLE ev = CreateEvent(nullptr, FALSE, FALSE, nullptr);
    NS_ENSURE_TRUE(ev, NS_ERROR_OUT_OF_MEMORY);

    HANDLE handles[2] = {ev, mCheckEvent};
    OVERLAPPED overlapped = {0};
    bool shuttingDown = false;

    overlapped.hEvent = ev;
    while (!shuttingDown) {
      HANDLE h;
      DWORD ret = NotifyAddrChange(&h, &overlapped);

      if (ret == ERROR_IO_PENDING) {
        ret = WaitForMultipleObjects(2, handles, FALSE, waitTime);
        if (ret == WAIT_OBJECT_0) {
          CheckLinkStatus();
        } else if (!mShutdown) {
          waitTime = nextCoalesceWaitTime();
        } else {
          shuttingDown = true;
        }
      } else {
        shuttingDown = true;
      }
    }
    CloseHandle(ev);
  } else {
    // Windows Vista and newer versions.
    HANDLE interfacechange;
    // The callback will simply invoke CheckLinkStatus()
    DWORD ret = sNotifyIpInterfaceChange(
        AF_UNSPEC,  // IPv4 and IPv6
        (PIPINTERFACE_CHANGE_CALLBACK)OnInterfaceChange,
        this,   // pass to callback
        false,  // no initial notification
        &interfacechange);

    if (ret == NO_ERROR) {
      do {
        ret = WaitForSingleObject(mCheckEvent, waitTime);
        if (!mShutdown) {
          waitTime = nextCoalesceWaitTime();
        } else {
          break;
        }
      } while (ret != WAIT_FAILED);
      sCancelMibChangeNotify2(interfacechange);
    } else {
      LOG(("Link Monitor: sNotifyIpInterfaceChange returned %d\n", (int)ret));
    }
  }
  return NS_OK;
}

NS_IMETHODIMP
<<<<<<< HEAD
nsNotifyAddrListener::Observe(nsISupports *subject, const char *topic,
                              const char16_t *data) {
  if (!strcmp("xpcom-shutdown-threads", topic)) Shutdown();

  return NS_OK;
||||||| merged common ancestors
nsNotifyAddrListener::Observe(nsISupports *subject,
                              const char *topic,
                              const char16_t *data)
{
    if (!strcmp("xpcom-shutdown-threads", topic))
        Shutdown();

    return NS_OK;
=======
nsNotifyAddrListener::Observe(nsISupports* subject, const char* topic,
                              const char16_t* data) {
  if (!strcmp("xpcom-shutdown-threads", topic)) Shutdown();

  return NS_OK;
>>>>>>> upstream-releases
}

nsresult nsNotifyAddrListener::Init(void) {
  nsCOMPtr<nsIObserverService> observerService =
      mozilla::services::GetObserverService();
  if (!observerService) return NS_ERROR_FAILURE;

  nsresult rv =
      observerService->AddObserver(this, "xpcom-shutdown-threads", false);
  NS_ENSURE_SUCCESS(rv, rv);

  Preferences::AddBoolVarCache(&mAllowChangedEvent, NETWORK_NOTIFY_CHANGED_PREF,
                               true);
  Preferences::AddBoolVarCache(&mIPv6Changes, NETWORK_NOTIFY_IPV6_PREF, false);

  mCheckEvent = CreateEvent(nullptr, FALSE, FALSE, nullptr);
  NS_ENSURE_TRUE(mCheckEvent, NS_ERROR_OUT_OF_MEMORY);

  rv = NS_NewNamedThread("Link Monitor", getter_AddRefs(mThread), this);
  NS_ENSURE_SUCCESS(rv, rv);

  return NS_OK;
}

nsresult nsNotifyAddrListener::Shutdown(void) {
  // remove xpcom shutdown observer
  nsCOMPtr<nsIObserverService> observerService =
      mozilla::services::GetObserverService();
  if (observerService)
    observerService->RemoveObserver(this, "xpcom-shutdown-threads");

  if (!mCheckEvent) return NS_OK;

  mShutdown = true;
  SetEvent(mCheckEvent);

  nsresult rv = mThread ? mThread->Shutdown() : NS_OK;

  // Have to break the cycle here, otherwise nsNotifyAddrListener holds
  // onto the thread and the thread holds onto the nsNotifyAddrListener
  // via its mRunnable
  mThread = nullptr;

  CloseHandle(mCheckEvent);
  mCheckEvent = nullptr;

  return rv;
}

/*
 * A network event has been registered. Delay the actual sending of the event
 * for a while and absorb subsequent events in the mean time in an effort to
 * squash potentially many triggers into a single event.
 * Only ever called from the same thread.
 */
nsresult nsNotifyAddrListener::NetworkChanged() {
  if (mCoalescingActive) {
    LOG(("NetworkChanged: absorbed an event (coalescing active)\n"));
  } else {
    // A fresh trigger!
    mChangeTime = TimeStamp::Now();
    mCoalescingActive = true;
    SetEvent(mCheckEvent);
    LOG(("NetworkChanged: coalescing period started\n"));
  }
  return NS_OK;
}

/* Sends the given event.  Assumes aEventID never goes out of scope (static
 * strings are ideal).
 */
<<<<<<< HEAD
nsresult nsNotifyAddrListener::SendEvent(const char *aEventID) {
  if (!aEventID) return NS_ERROR_NULL_POINTER;

  LOG(("SendEvent: network is '%s'\n", aEventID));

  nsresult rv;
  nsCOMPtr<nsIRunnable> event = new ChangeEvent(this, aEventID);
  if (NS_FAILED(rv = NS_DispatchToMainThread(event)))
    NS_WARNING("Failed to dispatch ChangeEvent");
  return rv;
||||||| merged common ancestors
nsresult
nsNotifyAddrListener::SendEvent(const char *aEventID)
{
    if (!aEventID)
        return NS_ERROR_NULL_POINTER;

    LOG(("SendEvent: network is '%s'\n", aEventID));

    nsresult rv;
    nsCOMPtr<nsIRunnable> event = new ChangeEvent(this, aEventID);
    if (NS_FAILED(rv = NS_DispatchToMainThread(event)))
        NS_WARNING("Failed to dispatch ChangeEvent");
    return rv;
=======
nsresult nsNotifyAddrListener::SendEvent(const char* aEventID) {
  if (!aEventID) return NS_ERROR_NULL_POINTER;

  LOG(("SendEvent: network is '%s'\n", aEventID));

  // The event was caused by a call to GetIsLinkUp on the main thread.
  // We only need to recalculate for events caused by NotifyAddrChange or
  // OnInterfaceChange
  if (!NS_IsMainThread()) {
    calculateNetworkId();
  }

  nsresult rv;
  nsCOMPtr<nsIRunnable> event = new ChangeEvent(this, aEventID);
  if (NS_FAILED(rv = NS_DispatchToMainThread(event)))
    NS_WARNING("Failed to dispatch ChangeEvent");
  return rv;
>>>>>>> upstream-releases
}

NS_IMETHODIMP
nsNotifyAddrListener::ChangeEvent::Run() {
  nsCOMPtr<nsIObserverService> observerService =
      mozilla::services::GetObserverService();
  if (observerService)
    observerService->NotifyObservers(mService, NS_NETWORK_LINK_TOPIC,
                                     NS_ConvertASCIItoUTF16(mEventID).get());
  return NS_OK;
}

// Bug 465158 features an explanation for this check. ICS being "Internet
// Connection Sharing). The description says it is always IP address
// 192.168.0.1 for this case.
bool nsNotifyAddrListener::CheckICSGateway(PIP_ADAPTER_ADDRESSES aAdapter) {
  if (!aAdapter->FirstUnicastAddress) return false;

  LPSOCKADDR aAddress = aAdapter->FirstUnicastAddress->Address.lpSockaddr;
  if (!aAddress) return false;

<<<<<<< HEAD
  PSOCKADDR_IN in_addr = (PSOCKADDR_IN)aAddress;
  bool isGateway = (aAddress->sa_family == AF_INET &&
                    in_addr->sin_addr.S_un.S_un_b.s_b1 == 192 &&
                    in_addr->sin_addr.S_un.S_un_b.s_b2 == 168 &&
                    in_addr->sin_addr.S_un.S_un_b.s_b3 == 0 &&
                    in_addr->sin_addr.S_un.S_un_b.s_b4 == 1);
||||||| merged common ancestors
    RefPtr<IEnumNetSharingPrivateConnection> privateEnum;
    if (SUCCEEDED(hr)) {
        RefPtr<IUnknown> privateEnumUnknown;
        hr = privateCollection->get__NewEnum(getter_AddRefs(privateEnumUnknown));
        if (SUCCEEDED(hr)) {
            hr = privateEnumUnknown->QueryInterface(
                        IID_IEnumNetSharingPrivateConnection,
                        getter_AddRefs(privateEnum));
        }
    }
=======
  PSOCKADDR_IN in_addr = (PSOCKADDR_IN)aAddress;
  bool isGateway = (aAddress->sa_family == AF_INET &&
                    in_addr->sin_addr.S_un.S_un_b.s_b1 == 192 &&
                    in_addr->sin_addr.S_un.S_un_b.s_b2 == 168 &&
                    in_addr->sin_addr.S_un.S_un_b.s_b3 == 0 &&
                    in_addr->sin_addr.S_un.S_un_b.s_b4 == 1);

  if (isGateway) isGateway = CheckICSStatus(aAdapter->FriendlyName);

  return isGateway;
}
>>>>>>> upstream-releases

<<<<<<< HEAD
  if (isGateway) isGateway = CheckICSStatus(aAdapter->FriendlyName);

  return isGateway;
}

bool nsNotifyAddrListener::CheckICSStatus(PWCHAR aAdapterName) {
  InitNetshellLibrary();

  // This method enumerates all privately shared connections and checks if some
  // of them has the same name as the one provided in aAdapterName. If such
  // connection is found in the collection the adapter is used as ICS gateway
  bool isICSGatewayAdapter = false;

  HRESULT hr;
  RefPtr<INetSharingManager> netSharingManager;
  hr = CoCreateInstance(CLSID_NetSharingManager, nullptr, CLSCTX_INPROC_SERVER,
                        IID_INetSharingManager,
                        getter_AddRefs(netSharingManager));

  RefPtr<INetSharingPrivateConnectionCollection> privateCollection;
  if (SUCCEEDED(hr)) {
    hr = netSharingManager->get_EnumPrivateConnections(
        ICSSC_DEFAULT, getter_AddRefs(privateCollection));
  }

  RefPtr<IEnumNetSharingPrivateConnection> privateEnum;
  if (SUCCEEDED(hr)) {
    RefPtr<IUnknown> privateEnumUnknown;
    hr = privateCollection->get__NewEnum(getter_AddRefs(privateEnumUnknown));
||||||| merged common ancestors
=======
bool nsNotifyAddrListener::CheckICSStatus(PWCHAR aAdapterName) {
  InitNetshellLibrary();

  // This method enumerates all privately shared connections and checks if some
  // of them has the same name as the one provided in aAdapterName. If such
  // connection is found in the collection the adapter is used as ICS gateway
  bool isICSGatewayAdapter = false;

  HRESULT hr;
  RefPtr<INetSharingManager> netSharingManager;
  hr = CoCreateInstance(CLSID_NetSharingManager, nullptr, CLSCTX_INPROC_SERVER,
                        IID_INetSharingManager,
                        getter_AddRefs(netSharingManager));

  RefPtr<INetSharingPrivateConnectionCollection> privateCollection;
  if (SUCCEEDED(hr)) {
    hr = netSharingManager->get_EnumPrivateConnections(
        ICSSC_DEFAULT, getter_AddRefs(privateCollection));
  }

  RefPtr<IEnumNetSharingPrivateConnection> privateEnum;
  if (SUCCEEDED(hr)) {
    RefPtr<IUnknown> privateEnumUnknown;
    hr = privateCollection->get__NewEnum(getter_AddRefs(privateEnumUnknown));
>>>>>>> upstream-releases
    if (SUCCEEDED(hr)) {
<<<<<<< HEAD
      hr = privateEnumUnknown->QueryInterface(
          IID_IEnumNetSharingPrivateConnection, getter_AddRefs(privateEnum));
    }
  }

  if (SUCCEEDED(hr)) {
    ULONG fetched;
    VARIANT connectionVariant;
    while (!isICSGatewayAdapter &&
           SUCCEEDED(hr = privateEnum->Next(1, &connectionVariant, &fetched)) &&
           fetched) {
      if (connectionVariant.vt != VT_UNKNOWN) {
        // We should call VariantClear here but it needs to link
        // with oleaut32.lib that produces a Ts incrase about 10ms
        // that is undesired. As it is quit unlikely the result would
        // be of a different type anyway, let's pass the variant
        // unfreed here.
        NS_ERROR(
            "Variant of unexpected type, expecting VT_UNKNOWN, we probably "
            "leak it!");
        continue;
      }

      RefPtr<INetConnection> connection;
      if (SUCCEEDED(connectionVariant.punkVal->QueryInterface(
              IID_INetConnection, getter_AddRefs(connection)))) {
        connectionVariant.punkVal->Release();

        NETCON_PROPERTIES *properties;
        if (SUCCEEDED(connection->GetProperties(&properties))) {
          if (!wcscmp(properties->pszwName, aAdapterName))
            isICSGatewayAdapter = true;

          if (sNcFreeNetconProperties) sNcFreeNetconProperties(properties);
||||||| merged common ancestors
        ULONG fetched;
        VARIANT connectionVariant;
        while (!isICSGatewayAdapter &&
               SUCCEEDED(hr = privateEnum->Next(1, &connectionVariant,
               &fetched)) &&
               fetched) {
            if (connectionVariant.vt != VT_UNKNOWN) {
                // We should call VariantClear here but it needs to link
                // with oleaut32.lib that produces a Ts incrase about 10ms
                // that is undesired. As it is quit unlikely the result would
                // be of a different type anyway, let's pass the variant
                // unfreed here.
                NS_ERROR("Variant of unexpected type, expecting VT_UNKNOWN, we probably leak it!");
                continue;
            }

            RefPtr<INetConnection> connection;
            if (SUCCEEDED(connectionVariant.punkVal->QueryInterface(
                              IID_INetConnection,
                              getter_AddRefs(connection)))) {
                connectionVariant.punkVal->Release();

                NETCON_PROPERTIES *properties;
                if (SUCCEEDED(connection->GetProperties(&properties))) {
                    if (!wcscmp(properties->pszwName, aAdapterName))
                        isICSGatewayAdapter = true;

                    if (sNcFreeNetconProperties)
                        sNcFreeNetconProperties(properties);
                }
            }
=======
      hr = privateEnumUnknown->QueryInterface(
          IID_IEnumNetSharingPrivateConnection, getter_AddRefs(privateEnum));
    }
  }

  if (SUCCEEDED(hr)) {
    ULONG fetched;
    VARIANT connectionVariant;
    while (!isICSGatewayAdapter &&
           SUCCEEDED(hr = privateEnum->Next(1, &connectionVariant, &fetched)) &&
           fetched) {
      if (connectionVariant.vt != VT_UNKNOWN) {
        // We should call VariantClear here but it needs to link
        // with oleaut32.lib that produces a Ts incrase about 10ms
        // that is undesired. As it is quit unlikely the result would
        // be of a different type anyway, let's pass the variant
        // unfreed here.
        NS_ERROR(
            "Variant of unexpected type, expecting VT_UNKNOWN, we probably "
            "leak it!");
        continue;
      }

      RefPtr<INetConnection> connection;
      if (SUCCEEDED(connectionVariant.punkVal->QueryInterface(
              IID_INetConnection, getter_AddRefs(connection)))) {
        connectionVariant.punkVal->Release();

        NETCON_PROPERTIES* properties;
        if (SUCCEEDED(connection->GetProperties(&properties))) {
          if (!wcscmp(properties->pszwName, aAdapterName))
            isICSGatewayAdapter = true;

          if (sNcFreeNetconProperties) sNcFreeNetconProperties(properties);
>>>>>>> upstream-releases
        }
      }
    }
  }

  return isICSGatewayAdapter;
}

DWORD
<<<<<<< HEAD
nsNotifyAddrListener::CheckAdaptersAddresses(void) {
  ULONG len = 16384;

  PIP_ADAPTER_ADDRESSES adapterList = (PIP_ADAPTER_ADDRESSES)moz_xmalloc(len);
||||||| merged common ancestors
nsNotifyAddrListener::CheckAdaptersAddresses(void)
{
    ULONG len = 16384;

    PIP_ADAPTER_ADDRESSES adapterList = (PIP_ADAPTER_ADDRESSES) moz_xmalloc(len);
=======
nsNotifyAddrListener::CheckAdaptersAddresses(void) {
  ULONG len = 16384;
>>>>>>> upstream-releases

<<<<<<< HEAD
  ULONG flags = GAA_FLAG_SKIP_DNS_SERVER | GAA_FLAG_SKIP_MULTICAST |
                GAA_FLAG_SKIP_ANYCAST;
||||||| merged common ancestors
    ULONG flags = GAA_FLAG_SKIP_DNS_SERVER|GAA_FLAG_SKIP_MULTICAST|
        GAA_FLAG_SKIP_ANYCAST;
=======
  PIP_ADAPTER_ADDRESSES adapterList = (PIP_ADAPTER_ADDRESSES)moz_xmalloc(len);
>>>>>>> upstream-releases

<<<<<<< HEAD
  DWORD ret =
      GetAdaptersAddresses(AF_UNSPEC, flags, nullptr, adapterList, &len);
  if (ret == ERROR_BUFFER_OVERFLOW) {
    free(adapterList);
    adapterList = static_cast<PIP_ADAPTER_ADDRESSES>(moz_xmalloc(len));
||||||| merged common ancestors
    DWORD ret = GetAdaptersAddresses(AF_UNSPEC, flags, nullptr, adapterList, &len);
    if (ret == ERROR_BUFFER_OVERFLOW) {
        free(adapterList);
        adapterList = static_cast<PIP_ADAPTER_ADDRESSES> (moz_xmalloc(len));
=======
  ULONG flags = GAA_FLAG_SKIP_DNS_SERVER | GAA_FLAG_SKIP_MULTICAST |
                GAA_FLAG_SKIP_ANYCAST;
>>>>>>> upstream-releases

<<<<<<< HEAD
    ret = GetAdaptersAddresses(AF_UNSPEC, flags, nullptr, adapterList, &len);
  }
||||||| merged common ancestors
        ret = GetAdaptersAddresses(AF_UNSPEC, flags, nullptr, adapterList, &len);
    }
=======
  DWORD ret =
      GetAdaptersAddresses(AF_UNSPEC, flags, nullptr, adapterList, &len);
  if (ret == ERROR_BUFFER_OVERFLOW) {
    free(adapterList);
    adapterList = static_cast<PIP_ADAPTER_ADDRESSES>(moz_xmalloc(len));
>>>>>>> upstream-releases

<<<<<<< HEAD
  if (FAILED(CoInitializeEx(nullptr, COINIT_MULTITHREADED))) {
    free(adapterList);
    return ERROR_NOT_SUPPORTED;
  }

  //
  // Since NotifyIpInterfaceChange() signals a change more often than we
  // think is a worthy change, we checksum the entire state of all interfaces
  // that are UP. If the checksum is the same as previous check, nothing
  // of interest changed!
  //
  ULONG sumAll = 0;

  if (ret == ERROR_SUCCESS) {
    bool linkUp = false;
    ULONG sum = 0;

    for (PIP_ADAPTER_ADDRESSES adapter = adapterList; adapter;
         adapter = adapter->Next) {
      if (adapter->OperStatus != IfOperStatusUp ||
          !adapter->FirstUnicastAddress ||
          adapter->IfType == IF_TYPE_SOFTWARE_LOOPBACK ||
          CheckICSGateway(adapter)) {
        continue;
      }

      sum <<= 2;
      // Add chars from AdapterName to the checksum.
      for (int i = 0; adapter->AdapterName[i]; ++i) {
        sum += adapter->AdapterName[i];
      }

      // Add bytes from each socket address to the checksum.
      for (PIP_ADAPTER_UNICAST_ADDRESS pip = adapter->FirstUnicastAddress; pip;
           pip = pip->Next) {
        SOCKET_ADDRESS *sockAddr = &pip->Address;
        for (int i = 0; i < sockAddr->iSockaddrLength; ++i) {
          sum += (reinterpret_cast<unsigned char *>(sockAddr->lpSockaddr))[i];
||||||| merged common ancestors
    if (FAILED(CoInitializeEx(nullptr, COINIT_MULTITHREADED))) {
        free(adapterList);
        return ERROR_NOT_SUPPORTED;
    }

    //
    // Since NotifyIpInterfaceChange() signals a change more often than we
    // think is a worthy change, we checksum the entire state of all interfaces
    // that are UP. If the checksum is the same as previous check, nothing
    // of interest changed!
    //
    ULONG sumAll = 0;

    if (ret == ERROR_SUCCESS) {
        bool linkUp = false;
        ULONG sum = 0;

        for (PIP_ADAPTER_ADDRESSES adapter = adapterList; adapter;
             adapter = adapter->Next) {
            if (adapter->OperStatus != IfOperStatusUp ||
                !adapter->FirstUnicastAddress ||
                adapter->IfType == IF_TYPE_SOFTWARE_LOOPBACK ||
                CheckICSGateway(adapter) ) {
                continue;
            }

            sum <<= 2;
            // Add chars from AdapterName to the checksum.
            for (int i = 0; adapter->AdapterName[i]; ++i) {
                sum += adapter->AdapterName[i];
            }

            // Add bytes from each socket address to the checksum.
            for (PIP_ADAPTER_UNICAST_ADDRESS pip = adapter->FirstUnicastAddress;
                 pip; pip = pip->Next) {
                SOCKET_ADDRESS *sockAddr = &pip->Address;
                for (int i = 0; i < sockAddr->iSockaddrLength; ++i) {
                    sum += (reinterpret_cast<unsigned char *>
                            (sockAddr->lpSockaddr))[i];
                }
            }
            linkUp = true;
            sumAll ^= sum;
=======
    ret = GetAdaptersAddresses(AF_UNSPEC, flags, nullptr, adapterList, &len);
  }

  if (FAILED(CoInitializeEx(nullptr, COINIT_MULTITHREADED))) {
    free(adapterList);
    return ERROR_NOT_SUPPORTED;
  }

  //
  // Since NotifyIpInterfaceChange() signals a change more often than we
  // think is a worthy change, we checksum the entire state of all interfaces
  // that are UP. If the checksum is the same as previous check, nothing
  // of interest changed!
  //
  ULONG sumAll = 0;

  if (ret == ERROR_SUCCESS) {
    bool linkUp = false;
    ULONG sum = 0;

    for (PIP_ADAPTER_ADDRESSES adapter = adapterList; adapter;
         adapter = adapter->Next) {
      if (adapter->OperStatus != IfOperStatusUp ||
          !adapter->FirstUnicastAddress ||
          adapter->IfType == IF_TYPE_SOFTWARE_LOOPBACK ||
          CheckICSGateway(adapter)) {
        continue;
      }

      sum <<= 2;
      // Add chars from AdapterName to the checksum.
      for (int i = 0; adapter->AdapterName[i]; ++i) {
        sum += adapter->AdapterName[i];
      }

      // Add bytes from each socket address to the checksum.
      for (PIP_ADAPTER_UNICAST_ADDRESS pip = adapter->FirstUnicastAddress; pip;
           pip = pip->Next) {
        SOCKET_ADDRESS* sockAddr = &pip->Address;
        for (int i = 0; i < sockAddr->iSockaddrLength; ++i) {
          sum += (reinterpret_cast<unsigned char*>(sockAddr->lpSockaddr))[i];
>>>>>>> upstream-releases
        }
      }
      linkUp = true;
      sumAll ^= sum;
    }
    mLinkUp = linkUp;
    mStatusKnown = true;
  }
  free(adapterList);

  if (mLinkUp) {
    /* Store the checksum only if one or more interfaces are up */
    mIPInterfaceChecksum = sumAll;
  }

  CoUninitialize();

  return ret;
}

/**
 * Checks the status of all network adapters.  If one is up and has a valid IP
 * address, sets mLinkUp to true.  Sets mStatusKnown to true if the link status
 * is definitive.
 */
<<<<<<< HEAD
void nsNotifyAddrListener::CheckLinkStatus(void) {
  DWORD ret;
  const char *event;
  bool prevLinkUp = mLinkUp;
  ULONG prevCsum = mIPInterfaceChecksum;

  LOG(("check status of all network adapters\n"));

  // The CheckAdaptersAddresses call is very expensive (~650 milliseconds),
  // so we don't want to call it synchronously. Instead, we just start up
  // assuming we have a network link, but we'll report that the status is
  // unknown.
  if (NS_IsMainThread()) {
    NS_WARNING(
        "CheckLinkStatus called on main thread! No check "
        "performed. Assuming link is up, status is unknown.");
    mLinkUp = true;

    if (!mStatusKnown) {
      event = NS_NETWORK_LINK_DATA_UNKNOWN;
    } else if (!prevLinkUp) {
      event = NS_NETWORK_LINK_DATA_UP;
||||||| merged common ancestors
void
nsNotifyAddrListener::CheckLinkStatus(void)
{
    DWORD ret;
    const char *event;
    bool prevLinkUp = mLinkUp;
    ULONG prevCsum = mIPInterfaceChecksum;

    LOG(("check status of all network adapters\n"));

    // The CheckAdaptersAddresses call is very expensive (~650 milliseconds),
    // so we don't want to call it synchronously. Instead, we just start up
    // assuming we have a network link, but we'll report that the status is
    // unknown.
    if (NS_IsMainThread()) {
        NS_WARNING("CheckLinkStatus called on main thread! No check "
                   "performed. Assuming link is up, status is unknown.");
        mLinkUp = true;

        if (!mStatusKnown) {
            event = NS_NETWORK_LINK_DATA_UNKNOWN;
        } else if (!prevLinkUp) {
            event = NS_NETWORK_LINK_DATA_UP;
        } else {
            // Known status and it was already UP
            event = nullptr;
        }

        if (event) {
            SendEvent(event);
        }
=======
void nsNotifyAddrListener::CheckLinkStatus(void) {
  DWORD ret;
  const char* event;
  bool prevLinkUp = mLinkUp;
  ULONG prevCsum = mIPInterfaceChecksum;

  LOG(("check status of all network adapters\n"));

  // The CheckAdaptersAddresses call is very expensive (~650 milliseconds),
  // so we don't want to call it synchronously. Instead, we just start up
  // assuming we have a network link, but we'll report that the status is
  // unknown.
  if (NS_IsMainThread()) {
    NS_WARNING(
        "CheckLinkStatus called on main thread! No check "
        "performed. Assuming link is up, status is unknown.");
    mLinkUp = true;

    if (!mStatusKnown) {
      event = NS_NETWORK_LINK_DATA_UNKNOWN;
    } else if (!prevLinkUp) {
      event = NS_NETWORK_LINK_DATA_UP;
>>>>>>> upstream-releases
    } else {
<<<<<<< HEAD
      // Known status and it was already UP
      event = nullptr;
    }
||||||| merged common ancestors
        ret = CheckAdaptersAddresses();
        if (ret != ERROR_SUCCESS) {
            mLinkUp = true;
        }
=======
      // Known status and it was already UP
      event = nullptr;
    }

    if (event) {
      SendEvent(event);
    }
  } else {
    ret = CheckAdaptersAddresses();
    if (ret != ERROR_SUCCESS) {
      mLinkUp = true;
    }
>>>>>>> upstream-releases

<<<<<<< HEAD
    if (event) {
      SendEvent(event);
    }
  } else {
    ret = CheckAdaptersAddresses();
    if (ret != ERROR_SUCCESS) {
      mLinkUp = true;
    }
||||||| merged common ancestors
        if (mLinkUp && (prevCsum != mIPInterfaceChecksum)) {
            TimeDuration since = TimeStamp::Now() - mStartTime;
=======
    if (mLinkUp && (prevCsum != mIPInterfaceChecksum)) {
      TimeDuration since = TimeStamp::Now() - mStartTime;
>>>>>>> upstream-releases

<<<<<<< HEAD
    if (mLinkUp && (prevCsum != mIPInterfaceChecksum)) {
      TimeDuration since = TimeStamp::Now() - mStartTime;

      // Network is online. Topology has changed. Always send CHANGED
      // before UP - if allowed to and having cooled down.
      if (mAllowChangedEvent && (since.ToMilliseconds() > 2000)) {
        NetworkChanged();
      }
    }
    if (prevLinkUp != mLinkUp) {
      // UP/DOWN status changed, send appropriate UP/DOWN event
      SendEvent(mLinkUp ? NS_NETWORK_LINK_DATA_UP : NS_NETWORK_LINK_DATA_DOWN);
||||||| merged common ancestors
            // Network is online. Topology has changed. Always send CHANGED
            // before UP - if allowed to and having cooled down.
            if (mAllowChangedEvent && (since.ToMilliseconds() > 2000)) {
                NetworkChanged();
            }
        }
        if (prevLinkUp != mLinkUp) {
            // UP/DOWN status changed, send appropriate UP/DOWN event
            SendEvent(mLinkUp ?
                      NS_NETWORK_LINK_DATA_UP : NS_NETWORK_LINK_DATA_DOWN);
        }
=======
      // Network is online. Topology has changed. Always send CHANGED
      // before UP - if allowed to and having cooled down.
      if (mAllowChangedEvent && (since.ToMilliseconds() > 2000)) {
        NetworkChanged();
      }
    }
    if (prevLinkUp != mLinkUp) {
      // UP/DOWN status changed, send appropriate UP/DOWN event
      SendEvent(mLinkUp ? NS_NETWORK_LINK_DATA_UP : NS_NETWORK_LINK_DATA_DOWN);
>>>>>>> upstream-releases
    }
  }
}
