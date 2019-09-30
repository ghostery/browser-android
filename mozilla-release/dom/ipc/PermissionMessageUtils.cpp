/* -*- Mode: C++; tab-width: 8; indent-tabs-mode: nil; c-basic-offset: 2 -*- */
/* vim: set ts=8 sts=2 et sw=2 tw=80: */
/* This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/. */

#include "mozilla/dom/PermissionMessageUtils.h"
<<<<<<< HEAD
#include "nsISerializable.h"
#include "nsSerializationHelper.h"

namespace IPC {

void ParamTraits<nsIPrincipal>::Write(Message* aMsg, nsIPrincipal* aParam) {
  bool isNull = !aParam;
  WriteParam(aMsg, isNull);
  if (isNull) {
    return;
  }

  nsCString principalString;
  nsresult rv = NS_SerializeToString(aParam, principalString);
  if (NS_FAILED(rv)) {
    MOZ_CRASH("Unable to serialize principal.");
    return;
||||||| merged common ancestors
#include "nsISerializable.h"
#include "nsSerializationHelper.h"

namespace IPC {

void
ParamTraits<nsIPrincipal>::Write(Message* aMsg, nsIPrincipal* aParam)
{
  bool isNull = !aParam;
  WriteParam(aMsg, isNull);
  if (isNull) {
    return;
  }

  nsCString principalString;
  nsresult rv = NS_SerializeToString(aParam, principalString);
  if (NS_FAILED(rv)) {
    MOZ_CRASH("Unable to serialize principal.");
    return;
=======
#include "mozilla/ipc/BackgroundUtils.h"
#include "mozilla/ipc/PBackgroundSharedTypes.h"

namespace mozilla {
namespace ipc {

void IPDLParamTraits<nsIPrincipal*>::Write(IPC::Message* aMsg,
                                           IProtocol* aActor,
                                           nsIPrincipal* aParam) {
  MOZ_DIAGNOSTIC_ASSERT(NS_IsMainThread());

  Maybe<PrincipalInfo> info;
  if (aParam) {
    info.emplace();
    nsresult rv = PrincipalToPrincipalInfo(aParam, info.ptr());
    MOZ_RELEASE_ASSERT(NS_SUCCEEDED(rv));
>>>>>>> upstream-releases
  }

  WriteIPDLParam(aMsg, aActor, info);
}

<<<<<<< HEAD
bool ParamTraits<nsIPrincipal>::Read(const Message* aMsg, PickleIterator* aIter,
                                     RefPtr<nsIPrincipal>* aResult) {
  bool isNull;
  if (!ReadParam(aMsg, aIter, &isNull)) {
    return false;
  }

  if (isNull) {
    *aResult = nullptr;
    return true;
  }

  nsCString principalString;
  if (!ReadParam(aMsg, aIter, &principalString)) {
||||||| merged common ancestors
bool
ParamTraits<nsIPrincipal>::Read(const Message* aMsg,
                                PickleIterator* aIter,
                                RefPtr<nsIPrincipal>* aResult)
{
  bool isNull;
  if (!ReadParam(aMsg, aIter, &isNull)) {
    return false;
  }

  if (isNull) {
    *aResult = nullptr;
    return true;
  }

  nsCString principalString;
  if (!ReadParam(aMsg, aIter, &principalString)) {
=======
bool IPDLParamTraits<nsIPrincipal*>::Read(const IPC::Message* aMsg,
                                          PickleIterator* aIter,
                                          IProtocol* aActor,
                                          RefPtr<nsIPrincipal>* aResult) {
  Maybe<PrincipalInfo> info;
  if (!ReadIPDLParam(aMsg, aIter, aActor, &info)) {
>>>>>>> upstream-releases
    return false;
  }

<<<<<<< HEAD
  nsCOMPtr<nsISupports> iSupports;
  nsresult rv =
      NS_DeserializeObject(principalString, getter_AddRefs(iSupports));
  NS_ENSURE_SUCCESS(rv, false);

  nsCOMPtr<nsIPrincipal> principal = do_QueryInterface(iSupports);
  NS_ENSURE_TRUE(principal, false);

  *aResult = principal.forget();
  return true;
||||||| merged common ancestors
  nsCOMPtr<nsISupports> iSupports;
  nsresult rv = NS_DeserializeObject(principalString, getter_AddRefs(iSupports));
  NS_ENSURE_SUCCESS(rv, false);

  nsCOMPtr<nsIPrincipal> principal = do_QueryInterface(iSupports);
  NS_ENSURE_TRUE(principal, false);

  *aResult = principal.forget();
  return true;
=======
  nsresult rv = NS_OK;
  *aResult = info ? PrincipalInfoToPrincipal(info.ref(), &rv) : nullptr;
  return NS_SUCCEEDED(rv);
>>>>>>> upstream-releases
}

<<<<<<< HEAD
}  // namespace IPC
||||||| merged common ancestors
} // namespace IPC

=======
}  // namespace ipc
}  // namespace mozilla
>>>>>>> upstream-releases
