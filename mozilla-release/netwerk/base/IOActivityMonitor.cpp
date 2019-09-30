/* -*- Mode: C++; tab-width: 2; indent-tabs-mode: nil; c-basic-offset: 2 -*-
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/. */
#include "IOActivityMonitor.h"
#include "nsIObserverService.h"
#include "nsPISocketTransportService.h"
#include "nsPrintfCString.h"
#include "nsSocketTransport2.h"
#include "nsSocketTransportService2.h"
#include "nsThreadUtils.h"
#include "mozilla/dom/Promise.h"
#include "prerror.h"
#include "prio.h"
#include "prmem.h"
#include <vector>

using namespace mozilla;
using namespace mozilla::net;

mozilla::StaticRefPtr<IOActivityMonitor> gInstance;
static PRDescIdentity sNetActivityMonitorLayerIdentity;
static PRIOMethods sNetActivityMonitorLayerMethods;
static PRIOMethods* sNetActivityMonitorLayerMethodsPtr = nullptr;

// Maximum number of activities entries in the monitoring class
#define MAX_ACTIVITY_ENTRIES 1000

// ActivityMonitorSecret is stored in the activity monitor layer
// and provides a method to get the location.
//
// A location can be :
// - a TCP or UDP socket. The form will be socket://ip:port
// - a File. The form will be file://path
//
// For other cases, the location will be fd://number
class ActivityMonitorSecret final {
 public:
  // constructor used for sockets
  explicit ActivityMonitorSecret(PRFileDesc *aFd) {
    mFd = aFd;
    mLocationSet = false;
  }

  // constructor used for files
  explicit ActivityMonitorSecret(PRFileDesc *aFd, const char *aLocation) {
    mFd = aFd;
    mLocation.AppendPrintf("file://%s", aLocation);
    mLocationSet = true;
  }

  nsCString getLocation() {
    if (!mLocationSet) {
      LazySetLocation();
    }
    return mLocation;
  }

 private:
  // Called to set the location using the FD on the first getLocation() usage
  // which is typically when a socket is opened. If done earlier, at
  // construction time, the host won't be bound yet.
  //
  // If the location is a file, it needs to be initialized in the
  // constructor.
  void LazySetLocation() {
    mLocationSet = true;
    PRFileDesc *extract = mFd;
    while (PR_GetDescType(extract) == PR_DESC_LAYERED) {
      if (!extract->lower) {
        break;
      }
      extract = extract->lower;
    }

    PRDescType fdType = PR_GetDescType(extract);
    // we should not use LazySetLocation for files
    MOZ_ASSERT(fdType != PR_DESC_FILE);

    switch (fdType) {
      case PR_DESC_SOCKET_TCP:
      case PR_DESC_SOCKET_UDP: {
        mLocation.AppendPrintf("socket://");
        PRNetAddr addr;
        PRStatus status = PR_GetSockName(mFd, &addr);
        if (NS_WARN_IF(status == PR_FAILURE)) {
          mLocation.AppendPrintf("unknown");
          break;
        }

        // grabbing the host
        char netAddr[mozilla::net::kNetAddrMaxCStrBufSize] = {0};
        status = PR_NetAddrToString(&addr, netAddr, sizeof(netAddr) - 1);
        if (NS_WARN_IF(status == PR_FAILURE) || netAddr[0] == 0) {
          mLocation.AppendPrintf("unknown");
          break;
        }
        mLocation.Append(netAddr);

        // adding the port
        uint16_t port;
        if (addr.raw.family == PR_AF_INET) {
          port = addr.inet.port;
        } else {
          port = addr.ipv6.port;
        }
        mLocation.AppendPrintf(":%d", port);
      } break;

      // for all other cases, we just send back fd://<value>
      default: {
        mLocation.AppendPrintf("fd://%d", PR_FileDesc2NativeHandle(mFd));
      }
    }  // end switch
  }

 private:
  nsCString mLocation;
  bool mLocationSet;
  PRFileDesc *mFd;
};

// FileDesc2Location converts a PRFileDesc into a "location" by
// grabbing the ActivityMonitorSecret in layer->secret
<<<<<<< HEAD
static nsAutoCString FileDesc2Location(PRFileDesc *fd) {
||||||| merged common ancestors
static nsAutoCString
FileDesc2Location(PRFileDesc *fd)
{
=======
static nsAutoCString FileDesc2Location(PRFileDesc* fd) {
>>>>>>> upstream-releases
  nsAutoCString location;
<<<<<<< HEAD
  PRFileDesc *monitorLayer =
      PR_GetIdentitiesLayer(fd, sNetActivityMonitorLayerIdentity);
||||||| merged common ancestors
  PRFileDesc *monitorLayer = PR_GetIdentitiesLayer(fd, sNetActivityMonitorLayerIdentity);
=======
  PRFileDesc* monitorLayer =
      PR_GetIdentitiesLayer(fd, sNetActivityMonitorLayerIdentity);
>>>>>>> upstream-releases
  if (!monitorLayer) {
    location.AppendPrintf("unknown");
    return location;
  }

  ActivityMonitorSecret *secret = (ActivityMonitorSecret *)monitorLayer->secret;
  location.AppendPrintf("%s", secret->getLocation().get());
  return location;
}

//
// Wrappers around the socket APIS
//
<<<<<<< HEAD
static PRStatus nsNetMon_Connect(PRFileDesc *fd, const PRNetAddr *addr,
                                 PRIntervalTime timeout) {
||||||| merged common ancestors
static PRStatus
nsNetMon_Connect(PRFileDesc *fd, const PRNetAddr *addr, PRIntervalTime timeout)
{
=======
static PRStatus nsNetMon_Connect(PRFileDesc* fd, const PRNetAddr* addr,
                                 PRIntervalTime timeout) {
>>>>>>> upstream-releases
  return fd->lower->methods->connect(fd->lower, addr, timeout);
}

<<<<<<< HEAD
static PRStatus nsNetMon_Close(PRFileDesc *fd) {
||||||| merged common ancestors

static PRStatus
nsNetMon_Close(PRFileDesc *fd)
{
=======
static PRStatus nsNetMon_Close(PRFileDesc* fd) {
>>>>>>> upstream-releases
  if (!fd) {
    return PR_FAILURE;
  }
<<<<<<< HEAD
  PRFileDesc *layer = PR_PopIOLayer(fd, PR_TOP_IO_LAYER);
  MOZ_RELEASE_ASSERT(
      layer && layer->identity == sNetActivityMonitorLayerIdentity,
      "NetActivityMonitor Layer not on top of stack");
||||||| merged common ancestors
  PRFileDesc* layer = PR_PopIOLayer(fd, PR_TOP_IO_LAYER);
  MOZ_RELEASE_ASSERT(layer &&
                     layer->identity == sNetActivityMonitorLayerIdentity,
                     "NetActivityMonitor Layer not on top of stack");
=======
  PRFileDesc* layer = PR_PopIOLayer(fd, PR_TOP_IO_LAYER);
  MOZ_RELEASE_ASSERT(
      layer && layer->identity == sNetActivityMonitorLayerIdentity,
      "NetActivityMonitor Layer not on top of stack");
>>>>>>> upstream-releases

  if (layer->secret) {
    delete (ActivityMonitorSecret*)layer->secret;
    layer->secret = nullptr;
  }
  layer->dtor(layer);
  return fd->methods->close(fd);
}

<<<<<<< HEAD
static int32_t nsNetMon_Read(PRFileDesc *fd, void *buf, int32_t len) {
||||||| merged common ancestors

static int32_t
nsNetMon_Read(PRFileDesc *fd, void *buf, int32_t len)
{
=======
static int32_t nsNetMon_Read(PRFileDesc* fd, void* buf, int32_t len) {
>>>>>>> upstream-releases
  int32_t ret = fd->lower->methods->read(fd->lower, buf, len);
  if (ret >= 0) {
    IOActivityMonitor::Read(fd, len);
  }
  return ret;
}

<<<<<<< HEAD
static int32_t nsNetMon_Write(PRFileDesc *fd, const void *buf, int32_t len) {
||||||| merged common ancestors
static int32_t
nsNetMon_Write(PRFileDesc *fd, const void *buf, int32_t len)
{
=======
static int32_t nsNetMon_Write(PRFileDesc* fd, const void* buf, int32_t len) {
>>>>>>> upstream-releases
  int32_t ret = fd->lower->methods->write(fd->lower, buf, len);
  if (ret > 0) {
    IOActivityMonitor::Write(fd, len);
  }
  return ret;
}

<<<<<<< HEAD
static int32_t nsNetMon_Writev(PRFileDesc *fd, const PRIOVec *iov, int32_t size,
                               PRIntervalTime timeout) {
||||||| merged common ancestors
static int32_t
nsNetMon_Writev(PRFileDesc *fd,
                const PRIOVec *iov,
                int32_t size,
                PRIntervalTime timeout)
{
=======
static int32_t nsNetMon_Writev(PRFileDesc* fd, const PRIOVec* iov, int32_t size,
                               PRIntervalTime timeout) {
>>>>>>> upstream-releases
  int32_t ret = fd->lower->methods->writev(fd->lower, iov, size, timeout);
  if (ret > 0) {
    IOActivityMonitor::Write(fd, size);
  }
  return ret;
}

<<<<<<< HEAD
static int32_t nsNetMon_Recv(PRFileDesc *fd, void *buf, int32_t amount,
                             int flags, PRIntervalTime timeout) {
  int32_t ret =
      fd->lower->methods->recv(fd->lower, buf, amount, flags, timeout);
||||||| merged common ancestors
static int32_t
nsNetMon_Recv(PRFileDesc *fd,
              void *buf,
              int32_t amount,
              int flags,
              PRIntervalTime timeout)
{
  int32_t ret = fd->lower->methods->recv(fd->lower, buf, amount, flags, timeout);
=======
static int32_t nsNetMon_Recv(PRFileDesc* fd, void* buf, int32_t amount,
                             int flags, PRIntervalTime timeout) {
  int32_t ret =
      fd->lower->methods->recv(fd->lower, buf, amount, flags, timeout);
>>>>>>> upstream-releases
  if (ret > 0) {
    IOActivityMonitor::Read(fd, amount);
  }
  return ret;
}

<<<<<<< HEAD
static int32_t nsNetMon_Send(PRFileDesc *fd, const void *buf, int32_t amount,
                             int flags, PRIntervalTime timeout) {
  int32_t ret =
      fd->lower->methods->send(fd->lower, buf, amount, flags, timeout);
||||||| merged common ancestors
static int32_t
nsNetMon_Send(PRFileDesc *fd,
              const void *buf,
              int32_t amount,
              int flags,
              PRIntervalTime timeout)
{
  int32_t ret = fd->lower->methods->send(fd->lower, buf, amount, flags, timeout);
=======
static int32_t nsNetMon_Send(PRFileDesc* fd, const void* buf, int32_t amount,
                             int flags, PRIntervalTime timeout) {
  int32_t ret =
      fd->lower->methods->send(fd->lower, buf, amount, flags, timeout);
>>>>>>> upstream-releases
  if (ret > 0) {
    IOActivityMonitor::Write(fd, amount);
  }
  return ret;
}

<<<<<<< HEAD
static int32_t nsNetMon_RecvFrom(PRFileDesc *fd, void *buf, int32_t amount,
                                 int flags, PRNetAddr *addr,
                                 PRIntervalTime timeout) {
||||||| merged common ancestors
static int32_t
nsNetMon_RecvFrom(PRFileDesc *fd,
                  void *buf,
                  int32_t amount,
                  int flags,
                  PRNetAddr *addr,
                  PRIntervalTime timeout)
{
=======
static int32_t nsNetMon_RecvFrom(PRFileDesc* fd, void* buf, int32_t amount,
                                 int flags, PRNetAddr* addr,
                                 PRIntervalTime timeout) {
>>>>>>> upstream-releases
  int32_t ret = fd->lower->methods->recvfrom(fd->lower, buf, amount, flags,
                                             addr, timeout);
  if (ret > 0) {
    IOActivityMonitor::Read(fd, amount);
  }
  return ret;
}

<<<<<<< HEAD
static int32_t nsNetMon_SendTo(PRFileDesc *fd, const void *buf, int32_t amount,
                               int flags, const PRNetAddr *addr,
                               PRIntervalTime timeout) {
  int32_t ret =
      fd->lower->methods->sendto(fd->lower, buf, amount, flags, addr, timeout);
||||||| merged common ancestors
static int32_t
nsNetMon_SendTo(PRFileDesc *fd,
                const void *buf,
                int32_t amount,
                int flags,
                const PRNetAddr *addr,
                PRIntervalTime timeout)
{
  int32_t ret = fd->lower->methods->sendto(fd->lower, buf, amount, flags,
                                           addr, timeout);
=======
static int32_t nsNetMon_SendTo(PRFileDesc* fd, const void* buf, int32_t amount,
                               int flags, const PRNetAddr* addr,
                               PRIntervalTime timeout) {
  int32_t ret =
      fd->lower->methods->sendto(fd->lower, buf, amount, flags, addr, timeout);
>>>>>>> upstream-releases
  if (ret > 0) {
    IOActivityMonitor::Write(fd, amount);
  }
  return ret;
}

<<<<<<< HEAD
static int32_t nsNetMon_AcceptRead(PRFileDesc *listenSock,
                                   PRFileDesc **acceptedSock,
                                   PRNetAddr **peerAddr, void *buf,
                                   int32_t amount, PRIntervalTime timeout) {
  int32_t ret = listenSock->lower->methods->acceptread(
      listenSock->lower, acceptedSock, peerAddr, buf, amount, timeout);
||||||| merged common ancestors
static int32_t
nsNetMon_AcceptRead(PRFileDesc *listenSock,
                    PRFileDesc **acceptedSock,
                    PRNetAddr **peerAddr,
                    void *buf,
                    int32_t amount,
                    PRIntervalTime timeout)
{
  int32_t ret = listenSock->lower->methods->acceptread(listenSock->lower, acceptedSock,
                                                       peerAddr, buf, amount, timeout);
=======
static int32_t nsNetMon_AcceptRead(PRFileDesc* listenSock,
                                   PRFileDesc** acceptedSock,
                                   PRNetAddr** peerAddr, void* buf,
                                   int32_t amount, PRIntervalTime timeout) {
  int32_t ret = listenSock->lower->methods->acceptread(
      listenSock->lower, acceptedSock, peerAddr, buf, amount, timeout);
>>>>>>> upstream-releases
  if (ret > 0) {
    IOActivityMonitor::Read(listenSock, amount);
  }
  return ret;
}

//
// Class IOActivityMonitor
//
NS_IMPL_ISUPPORTS(IOActivityMonitor, nsINamed)

IOActivityMonitor::IOActivityMonitor() : mLock("IOActivityMonitor::mLock") {
  RefPtr<IOActivityMonitor> mon(gInstance);
  MOZ_ASSERT(!mon, "multiple IOActivityMonitor instances!");
}

// static
<<<<<<< HEAD
void IOActivityMonitor::RequestActivities(dom::Promise *aPromise) {
||||||| merged common ancestors
void
IOActivityMonitor::RequestActivities(dom::Promise* aPromise)
{
=======
void IOActivityMonitor::RequestActivities(dom::Promise* aPromise) {
>>>>>>> upstream-releases
  MOZ_ASSERT(aPromise);
  RefPtr<IOActivityMonitor> mon(gInstance);
  if (!IsActive()) {
    aPromise->MaybeReject(NS_ERROR_FAILURE);
    return;
  }
  mon->RequestActivitiesInternal(aPromise);
}

<<<<<<< HEAD
void IOActivityMonitor::RequestActivitiesInternal(dom::Promise *aPromise) {
||||||| merged common ancestors
void
IOActivityMonitor::RequestActivitiesInternal(dom::Promise* aPromise)
{
=======
void IOActivityMonitor::RequestActivitiesInternal(dom::Promise* aPromise) {
>>>>>>> upstream-releases
  nsresult result = NS_OK;
  FallibleTArray<dom::IOActivityDataDictionary> activities;

  {
    mozilla::MutexAutoLock lock(mLock);
    // Remove inactive activities
    for (auto iter = mActivities.Iter(); !iter.Done(); iter.Next()) {
      dom::IOActivityDataDictionary *activity = &iter.Data();
      if (activity->mRx == 0 && activity->mTx == 0) {
        iter.Remove();
      } else {
        if (NS_WARN_IF(!activities.AppendElement(iter.Data(), fallible))) {
          result = NS_ERROR_OUT_OF_MEMORY;
          break;
        }
      }
    }
  }

  if (NS_WARN_IF(NS_FAILED(result))) {
    aPromise->MaybeReject(result);
    return;
  }
  aPromise->MaybeResolve(activities);
}

// static
NS_IMETHODIMP
<<<<<<< HEAD
IOActivityMonitor::GetName(nsACString &aName) {
||||||| merged common ancestors
IOActivityMonitor::GetName(nsACString& aName)
{
=======
IOActivityMonitor::GetName(nsACString& aName) {
>>>>>>> upstream-releases
  aName.AssignLiteral("IOActivityMonitor");
  return NS_OK;
}

bool IOActivityMonitor::IsActive() { return gInstance != nullptr; }

nsresult IOActivityMonitor::Init() {
  if (IsActive()) {
    return NS_ERROR_ALREADY_INITIALIZED;
  }
  RefPtr<IOActivityMonitor> mon = new IOActivityMonitor();
  nsresult rv = mon->InitInternal();
  if (NS_SUCCEEDED(rv)) {
    gInstance = mon;
  }
  return rv;
}

nsresult IOActivityMonitor::InitInternal() {
  // wraps the socket APIs
  if (!sNetActivityMonitorLayerMethodsPtr) {
    sNetActivityMonitorLayerIdentity =
        PR_GetUniqueIdentity("network activity monitor layer");
    sNetActivityMonitorLayerMethods = *PR_GetDefaultIOMethods();
    sNetActivityMonitorLayerMethods.connect = nsNetMon_Connect;
    sNetActivityMonitorLayerMethods.read = nsNetMon_Read;
    sNetActivityMonitorLayerMethods.write = nsNetMon_Write;
    sNetActivityMonitorLayerMethods.writev = nsNetMon_Writev;
    sNetActivityMonitorLayerMethods.recv = nsNetMon_Recv;
    sNetActivityMonitorLayerMethods.send = nsNetMon_Send;
    sNetActivityMonitorLayerMethods.recvfrom = nsNetMon_RecvFrom;
    sNetActivityMonitorLayerMethods.sendto = nsNetMon_SendTo;
    sNetActivityMonitorLayerMethods.acceptread = nsNetMon_AcceptRead;
    sNetActivityMonitorLayerMethods.close = nsNetMon_Close;
    sNetActivityMonitorLayerMethodsPtr = &sNetActivityMonitorLayerMethods;
  }

  return NS_OK;
}

nsresult IOActivityMonitor::Shutdown() {
  RefPtr<IOActivityMonitor> mon(gInstance);
  if (!mon) {
    return NS_ERROR_NOT_INITIALIZED;
  }
  return mon->ShutdownInternal();
}

nsresult IOActivityMonitor::ShutdownInternal() {
  mozilla::MutexAutoLock lock(mLock);
  mActivities.Clear();
  gInstance = nullptr;
  return NS_OK;
}

<<<<<<< HEAD
nsresult IOActivityMonitor::MonitorSocket(PRFileDesc *aFd) {
||||||| merged common ancestors
nsresult
IOActivityMonitor::MonitorSocket(PRFileDesc *aFd)
{
=======
nsresult IOActivityMonitor::MonitorSocket(PRFileDesc* aFd) {
>>>>>>> upstream-releases
  RefPtr<IOActivityMonitor> mon(gInstance);
  if (!IsActive()) {
    return NS_OK;
  }
  PRFileDesc *layer;
  PRStatus status;
  layer = PR_CreateIOLayerStub(sNetActivityMonitorLayerIdentity,
                               sNetActivityMonitorLayerMethodsPtr);
  if (!layer) {
    return NS_ERROR_FAILURE;
  }

<<<<<<< HEAD
  ActivityMonitorSecret *secret = new ActivityMonitorSecret(aFd);
  layer->secret = reinterpret_cast<PRFilePrivate *>(secret);
||||||| merged common ancestors
  ActivityMonitorSecret* secret = new ActivityMonitorSecret(aFd);
  layer->secret = reinterpret_cast<PRFilePrivate *>(secret);
=======
  ActivityMonitorSecret* secret = new ActivityMonitorSecret(aFd);
  layer->secret = reinterpret_cast<PRFilePrivate*>(secret);
>>>>>>> upstream-releases
  status = PR_PushIOLayer(aFd, PR_NSPR_IO_LAYER, layer);

  if (status == PR_FAILURE) {
    delete secret;
    PR_Free(layer);  // PR_CreateIOLayerStub() uses PR_Malloc().
    return NS_ERROR_FAILURE;
  }
  return NS_OK;
}

<<<<<<< HEAD
nsresult IOActivityMonitor::MonitorFile(PRFileDesc *aFd, const char *aPath) {
||||||| merged common ancestors
nsresult
IOActivityMonitor::MonitorFile(PRFileDesc *aFd, const char* aPath)
{
=======
nsresult IOActivityMonitor::MonitorFile(PRFileDesc* aFd, const char* aPath) {
>>>>>>> upstream-releases
  RefPtr<IOActivityMonitor> mon(gInstance);
  if (!IsActive()) {
    return NS_OK;
  }
  PRFileDesc *layer;
  PRStatus status;
  layer = PR_CreateIOLayerStub(sNetActivityMonitorLayerIdentity,
                               sNetActivityMonitorLayerMethodsPtr);
  if (!layer) {
    return NS_ERROR_FAILURE;
  }

<<<<<<< HEAD
  ActivityMonitorSecret *secret = new ActivityMonitorSecret(aFd, aPath);
  layer->secret = reinterpret_cast<PRFilePrivate *>(secret);
||||||| merged common ancestors
  ActivityMonitorSecret* secret = new ActivityMonitorSecret(aFd, aPath);
  layer->secret = reinterpret_cast<PRFilePrivate *>(secret);
=======
  ActivityMonitorSecret* secret = new ActivityMonitorSecret(aFd, aPath);
  layer->secret = reinterpret_cast<PRFilePrivate*>(secret);
>>>>>>> upstream-releases

  status = PR_PushIOLayer(aFd, PR_NSPR_IO_LAYER, layer);
  if (status == PR_FAILURE) {
    delete secret;
    PR_Free(layer);  // PR_CreateIOLayerStub() uses PR_Malloc().
    return NS_ERROR_FAILURE;
  }

  return NS_OK;
}

<<<<<<< HEAD
bool IOActivityMonitor::IncrementActivity(const nsACString &aLocation,
                                          uint32_t aRx, uint32_t aTx) {
||||||| merged common ancestors
bool
IOActivityMonitor::IncrementActivity(const nsACString& aLocation, uint32_t aRx, uint32_t aTx)
{
=======
bool IOActivityMonitor::IncrementActivity(const nsACString& aLocation,
                                          uint32_t aRx, uint32_t aTx) {
>>>>>>> upstream-releases
  mLock.AssertCurrentThreadOwns();
  if (auto entry = mActivities.Lookup(aLocation)) {
    // already registered
    entry.Data().mTx += aTx;
    entry.Data().mRx += aRx;
    return true;
  }
  // Creating a new IOActivity. Notice that mActivities will
  // grow indefinitely, which is OK since we won't have
  // but a few hundreds entries at the most, but we
  // want to assert we have at the most 1000 entries
  MOZ_ASSERT(mActivities.Count() < MAX_ACTIVITY_ENTRIES);

  dom::IOActivityDataDictionary activity;
  activity.mLocation.Assign(aLocation);
  activity.mTx = aTx;
  activity.mRx = aRx;

  if (NS_WARN_IF(!mActivities.Put(aLocation, activity, fallible))) {
    return false;
  }
  return true;
}

<<<<<<< HEAD
nsresult IOActivityMonitor::Write(const nsACString &aLocation,
                                  uint32_t aAmount) {
||||||| merged common ancestors
nsresult
IOActivityMonitor::Write(const nsACString& aLocation, uint32_t aAmount)
{
=======
nsresult IOActivityMonitor::Write(const nsACString& aLocation,
                                  uint32_t aAmount) {
>>>>>>> upstream-releases
  RefPtr<IOActivityMonitor> mon(gInstance);
  if (!mon) {
    return NS_ERROR_FAILURE;
  }
  return mon->WriteInternal(aLocation, aAmount);
}

<<<<<<< HEAD
nsresult IOActivityMonitor::Write(PRFileDesc *fd, uint32_t aAmount) {
||||||| merged common ancestors
nsresult
IOActivityMonitor::Write(PRFileDesc *fd, uint32_t aAmount)
{
=======
nsresult IOActivityMonitor::Write(PRFileDesc* fd, uint32_t aAmount) {
>>>>>>> upstream-releases
  RefPtr<IOActivityMonitor> mon(gInstance);
  if (!mon) {
    return NS_ERROR_FAILURE;
  }
  return mon->Write(FileDesc2Location(fd), aAmount);
}

<<<<<<< HEAD
nsresult IOActivityMonitor::WriteInternal(const nsACString &aLocation,
                                          uint32_t aAmount) {
||||||| merged common ancestors
nsresult
IOActivityMonitor::WriteInternal(const nsACString& aLocation, uint32_t aAmount)
{
=======
nsresult IOActivityMonitor::WriteInternal(const nsACString& aLocation,
                                          uint32_t aAmount) {
>>>>>>> upstream-releases
  mozilla::MutexAutoLock lock(mLock);
  if (!IncrementActivity(aLocation, aAmount, 0)) {
    return NS_ERROR_FAILURE;
  }
  return NS_OK;
}

<<<<<<< HEAD
nsresult IOActivityMonitor::Read(PRFileDesc *fd, uint32_t aAmount) {
||||||| merged common ancestors
nsresult
IOActivityMonitor::Read(PRFileDesc *fd, uint32_t aAmount)
{
=======
nsresult IOActivityMonitor::Read(PRFileDesc* fd, uint32_t aAmount) {
>>>>>>> upstream-releases
  RefPtr<IOActivityMonitor> mon(gInstance);
  if (!mon) {
    return NS_ERROR_FAILURE;
  }
  return mon->Read(FileDesc2Location(fd), aAmount);
}

<<<<<<< HEAD
nsresult IOActivityMonitor::Read(const nsACString &aLocation,
                                 uint32_t aAmount) {
||||||| merged common ancestors
nsresult
IOActivityMonitor::Read(const nsACString& aLocation, uint32_t aAmount)
{
=======
nsresult IOActivityMonitor::Read(const nsACString& aLocation,
                                 uint32_t aAmount) {
>>>>>>> upstream-releases
  RefPtr<IOActivityMonitor> mon(gInstance);
  if (!mon) {
    return NS_ERROR_FAILURE;
  }
  return mon->ReadInternal(aLocation, aAmount);
}

<<<<<<< HEAD
nsresult IOActivityMonitor::ReadInternal(const nsACString &aLocation,
                                         uint32_t aAmount) {
||||||| merged common ancestors
nsresult
IOActivityMonitor::ReadInternal(const nsACString& aLocation, uint32_t aAmount)
{
=======
nsresult IOActivityMonitor::ReadInternal(const nsACString& aLocation,
                                         uint32_t aAmount) {
>>>>>>> upstream-releases
  mozilla::MutexAutoLock lock(mLock);
  if (!IncrementActivity(aLocation, 0, aAmount)) {
    return NS_ERROR_FAILURE;
  }
  return NS_OK;
}
