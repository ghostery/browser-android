/* -*- Mode: C++; tab-width: 2; indent-tabs-mode: nil; c-basic-offset: 2 -*-
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/. */

#include "mozilla/Assertions.h"
#include "ShutdownLayer.h"
#include "prerror.h"
#include "private/pprio.h"
#include "prmem.h"
#include <winsock2.h>

static PRDescIdentity sWinSockShutdownLayerIdentity;
static PRIOMethods sWinSockShutdownLayerMethods;
static PRIOMethods* sWinSockShutdownLayerMethodsPtr = nullptr;

namespace mozilla {
namespace net {

extern PRDescIdentity nsNamedPipeLayerIdentity;

}  // namespace net
}  // namespace mozilla

<<<<<<< HEAD
PRStatus WinSockClose(PRFileDesc *aFd) {
||||||| merged common ancestors

PRStatus
WinSockClose(PRFileDesc *aFd)
{
=======
PRStatus WinSockClose(PRFileDesc* aFd) {
>>>>>>> upstream-releases
  MOZ_RELEASE_ASSERT(aFd->identity == sWinSockShutdownLayerIdentity,
                     "Windows shutdown layer not on the top of the stack");

  PROsfd osfd = PR_FileDesc2NativeHandle(aFd);
  if (osfd != -1) {
    shutdown(osfd, SD_BOTH);
  }

  aFd->identity = PR_INVALID_IO_LAYER;

  if (aFd->lower) {
    return aFd->lower->methods->close(aFd->lower);
  } else {
    return PR_SUCCESS;
  }
}

<<<<<<< HEAD
nsresult mozilla::net::AttachShutdownLayer(PRFileDesc *aFd) {
||||||| merged common ancestors
nsresult mozilla::net::AttachShutdownLayer(PRFileDesc *aFd)
{
=======
nsresult mozilla::net::AttachShutdownLayer(PRFileDesc* aFd) {
>>>>>>> upstream-releases
  if (!sWinSockShutdownLayerMethodsPtr) {
    sWinSockShutdownLayerIdentity =
        PR_GetUniqueIdentity("windows shutdown call layer");
    sWinSockShutdownLayerMethods = *PR_GetDefaultIOMethods();
    sWinSockShutdownLayerMethods.close = WinSockClose;
    sWinSockShutdownLayerMethodsPtr = &sWinSockShutdownLayerMethods;
  }

  if (mozilla::net::nsNamedPipeLayerIdentity &&
      PR_GetIdentitiesLayer(aFd, mozilla::net::nsNamedPipeLayerIdentity)) {
    // Do not attach shutdown layer on named pipe layer,
    // it is for PR_NSPR_IO_LAYER only.
    return NS_OK;
  }

<<<<<<< HEAD
  PRFileDesc *layer;
  PRStatus status;
||||||| merged common ancestors
  PRFileDesc * layer;
  PRStatus     status;
=======
  PRFileDesc* layer;
  PRStatus status;
>>>>>>> upstream-releases

  layer = PR_CreateIOLayerStub(sWinSockShutdownLayerIdentity,
                               sWinSockShutdownLayerMethodsPtr);
  if (!layer) {
    return NS_OK;
  }

  status = PR_PushIOLayer(aFd, PR_NSPR_IO_LAYER, layer);

  if (status == PR_FAILURE) {
    PR_Free(layer);  // PR_CreateIOLayerStub() uses PR_Malloc().
    return NS_ERROR_FAILURE;
  }
  return NS_OK;
}
