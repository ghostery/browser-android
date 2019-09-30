/* -*- Mode: C++; tab-width: 8; indent-tabs-mode: nil; c-basic-offset: 2 -*- */
/* vim: set ts=8 sts=2 et sw=2 tw=80: */
/* This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/. */

#ifndef mozilla_dom_permission_message_utils_h__
#define mozilla_dom_permission_message_utils_h__

#include "mozilla/ipc/IPDLParamTraits.h"
#include "ipc/IPCMessageUtils.h"
#include "nsCOMPtr.h"
#include "nsIPrincipal.h"

namespace IPC {

<<<<<<< HEAD
template <>
struct ParamTraits<nsIPrincipal> {
  static void Write(Message* aMsg, nsIPrincipal* aParam);
  static bool Read(const Message* aMsg, PickleIterator* aIter,
                   RefPtr<nsIPrincipal>* aResult);
};

||||||| merged common ancestors
template<>
struct ParamTraits<nsIPrincipal>
{
  static void Write(Message* aMsg, nsIPrincipal* aParam);
  static bool Read(const Message* aMsg, PickleIterator* aIter, RefPtr<nsIPrincipal>* aResult);
};

=======
>>>>>>> upstream-releases
/**
 * Legacy IPC::Principal type. Use nsIPrincipal directly in new IPDL code.
 */
<<<<<<< HEAD
class Principal {
  friend struct ParamTraits<Principal>;
||||||| merged common ancestors
class Principal
{
  friend struct ParamTraits<Principal>;
=======
class Principal {
  friend struct mozilla::ipc::IPDLParamTraits<Principal>;
>>>>>>> upstream-releases

 public:
  Principal() : mPrincipal(nullptr) {}

  explicit Principal(nsIPrincipal* aPrincipal) : mPrincipal(aPrincipal) {}

  operator nsIPrincipal*() const { return mPrincipal.get(); }

  Principal& operator=(const Principal& aOther) {
    mPrincipal = aOther.mPrincipal;
    return *this;
  }

 private:
  RefPtr<nsIPrincipal> mPrincipal;
};

}  // namespace IPC

namespace mozilla {
namespace ipc {

template <>
<<<<<<< HEAD
struct ParamTraits<Principal> {
  typedef Principal paramType;
  static void Write(Message* aMsg, const paramType& aParam) {
    WriteParam(aMsg, aParam.mPrincipal);
  }
  static bool Read(const Message* aMsg, PickleIterator* aIter,
                   paramType* aResult) {
    return ReadParam(aMsg, aIter, &aResult->mPrincipal);
||||||| merged common ancestors
struct ParamTraits<Principal>
{
  typedef Principal paramType;
  static void Write(Message* aMsg, const paramType& aParam)
  {
    WriteParam(aMsg, aParam.mPrincipal);
  }
  static bool Read(const Message* aMsg, PickleIterator* aIter, paramType* aResult)
  {
    return ReadParam(aMsg, aIter, &aResult->mPrincipal);
=======
struct IPDLParamTraits<nsIPrincipal*> {
  static void Write(IPC::Message* aMsg, IProtocol* aActor,
                    nsIPrincipal* aParam);
  static bool Read(const IPC::Message* aMsg, PickleIterator* aIter,
                   IProtocol* aActor, RefPtr<nsIPrincipal>* aResult);

  // Overload to support deserializing nsCOMPtr<nsIPrincipal> directly.
  static bool Read(const IPC::Message* aMsg, PickleIterator* aIter,
                   IProtocol* aActor, nsCOMPtr<nsIPrincipal>* aResult) {
    RefPtr<nsIPrincipal> result;
    if (!Read(aMsg, aIter, aActor, &result)) {
      return false;
    }
    *aResult = result.forget();
    return true;
>>>>>>> upstream-releases
  }
};

<<<<<<< HEAD
}  // namespace IPC
||||||| merged common ancestors
} // namespace IPC

#endif // mozilla_dom_permission_message_utils_h__
=======
template <>
struct IPDLParamTraits<IPC::Principal> {
  typedef IPC::Principal paramType;
  static void Write(IPC::Message* aMsg, IProtocol* aActor,
                    const paramType& aParam) {
    WriteIPDLParam(aMsg, aActor, aParam.mPrincipal);
  }
  static bool Read(const IPC::Message* aMsg, PickleIterator* aIter,
                   IProtocol* aActor, paramType* aResult) {
    return ReadIPDLParam(aMsg, aIter, aActor, &aResult->mPrincipal);
  }
};

}  // namespace ipc
}  // namespace mozilla
>>>>>>> upstream-releases

#endif  // mozilla_dom_permission_message_utils_h__
