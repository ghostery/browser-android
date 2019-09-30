/* This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/. */

#include "mozilla/UniquePtr.h"
#include "RemoteSpellCheckEngineChild.h"

namespace mozilla {

RemoteSpellcheckEngineChild::RemoteSpellcheckEngineChild(
    mozSpellChecker* aOwner)
    : mOwner(aOwner) {}

RemoteSpellcheckEngineChild::~RemoteSpellcheckEngineChild() {
  // null out the owner's SpellcheckEngineChild to prevent state corruption
  // during shutdown
  mOwner->DeleteRemoteEngine();
}

RefPtr<GenericPromise>
RemoteSpellcheckEngineChild::SetCurrentDictionaryFromList(
<<<<<<< HEAD
    const nsTArray<nsString>& aList) {
  MozPromiseHolder<GenericPromise>* promiseHolder =
      new MozPromiseHolder<GenericPromise>();
  RefPtr<GenericPromise> promise = promiseHolder->Ensure(__func__);
||||||| merged common ancestors
  const nsTArray<nsString>& aList)
{
  MozPromiseHolder<GenericPromise>* promiseHolder =
    new MozPromiseHolder<GenericPromise>();
  RefPtr<GenericPromise> promise = promiseHolder->Ensure(__func__);
=======
    const nsTArray<nsString>& aList) {
>>>>>>> upstream-releases
  RefPtr<mozSpellChecker> spellChecker = mOwner;

<<<<<<< HEAD
  SendSetDictionaryFromList(aList)->Then(
      GetMainThreadSerialEventTarget(), __func__,
      [spellChecker, promiseHolder](const Tuple<bool, nsString>& aParam) {
        UniquePtr<MozPromiseHolder<GenericPromise>> holder(promiseHolder);
        if (!Get<0>(aParam)) {
          spellChecker->mCurrentDictionary.Truncate();
          holder->Reject(NS_ERROR_NOT_AVAILABLE, __func__);
          return;
        }
        spellChecker->mCurrentDictionary = Get<1>(aParam);
        holder->Resolve(true, __func__);
      },
      [spellChecker, promiseHolder](ResponseRejectReason aReason) {
        UniquePtr<MozPromiseHolder<GenericPromise>> holder(promiseHolder);
||||||| merged common ancestors
  SendSetDictionaryFromList(aList)->Then(
    GetMainThreadSerialEventTarget(),
    __func__,
    [spellChecker, promiseHolder](const Tuple<bool, nsString>& aParam) {
      UniquePtr<MozPromiseHolder<GenericPromise>> holder(promiseHolder);
      if (!Get<0>(aParam)) {
=======
  return SendSetDictionaryFromList(aList)->Then(
      GetMainThreadSerialEventTarget(), __func__,
      [spellChecker](Tuple<bool, nsString>&& aParam) {
        if (!Get<0>(aParam)) {
          spellChecker->mCurrentDictionary.Truncate();
          return GenericPromise::CreateAndReject(NS_ERROR_NOT_AVAILABLE,
                                                 __func__);
        }
        spellChecker->mCurrentDictionary = std::move(Get<1>(aParam));
        return GenericPromise::CreateAndResolve(true, __func__);
      },
      [spellChecker](ResponseRejectReason&& aReason) {
>>>>>>> upstream-releases
        spellChecker->mCurrentDictionary.Truncate();
<<<<<<< HEAD
        holder->Reject(NS_ERROR_NOT_AVAILABLE, __func__);
      });
  return promise;
||||||| merged common ancestors
        holder->Reject(NS_ERROR_NOT_AVAILABLE, __func__);
        return;
      }
      spellChecker->mCurrentDictionary = Get<1>(aParam);
      holder->Resolve(true, __func__);
    },
    [spellChecker, promiseHolder](ResponseRejectReason aReason) {
      UniquePtr<MozPromiseHolder<GenericPromise>> holder(promiseHolder);
      spellChecker->mCurrentDictionary.Truncate();
      holder->Reject(NS_ERROR_NOT_AVAILABLE, __func__);
    });
  return promise;
=======
        return GenericPromise::CreateAndReject(NS_ERROR_NOT_AVAILABLE,
                                               __func__);
      });
}

RefPtr<CheckWordPromise> RemoteSpellcheckEngineChild::CheckWords(
    const nsTArray<nsString>& aWords) {
  RefPtr<mozSpellChecker> kungFuDeathGrip = mOwner;

  return SendCheckAsync(aWords)->Then(
      GetMainThreadSerialEventTarget(), __func__,
      [kungFuDeathGrip](nsTArray<bool>&& aIsMisspelled) {
        return CheckWordPromise::CreateAndResolve(std::move(aIsMisspelled),
                                                  __func__);
      },
      [kungFuDeathGrip](mozilla::ipc::ResponseRejectReason&& aReason) {
        return CheckWordPromise::CreateAndReject(NS_ERROR_NOT_AVAILABLE,
                                                 __func__);
      });
>>>>>>> upstream-releases
}

}  // namespace mozilla
