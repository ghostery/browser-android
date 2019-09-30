/* This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/. */

#ifndef RemoteSpellcheckEngineChild_h_
#define RemoteSpellcheckEngineChild_h_

#include "mozilla/MozPromise.h"
#include "mozilla/PRemoteSpellcheckEngineChild.h"
#include "mozSpellChecker.h"

class mozSpellChecker;

namespace mozilla {

<<<<<<< HEAD
class RemoteSpellcheckEngineChild
    : public mozilla::PRemoteSpellcheckEngineChild {
 public:
  explicit RemoteSpellcheckEngineChild(mozSpellChecker *aOwner);
||||||| merged common ancestors
class RemoteSpellcheckEngineChild : public mozilla::PRemoteSpellcheckEngineChild
{
public:
  explicit RemoteSpellcheckEngineChild(mozSpellChecker *aOwner);
=======
class RemoteSpellcheckEngineChild
    : public mozilla::PRemoteSpellcheckEngineChild {
 public:
  explicit RemoteSpellcheckEngineChild(mozSpellChecker* aOwner);
>>>>>>> upstream-releases
  virtual ~RemoteSpellcheckEngineChild();

  RefPtr<GenericPromise> SetCurrentDictionaryFromList(
<<<<<<< HEAD
      const nsTArray<nsString> &aList);
||||||| merged common ancestors
                           const nsTArray<nsString>& aList);
=======
      const nsTArray<nsString>& aList);
>>>>>>> upstream-releases

<<<<<<< HEAD
 private:
  mozSpellChecker *mOwner;
||||||| merged common ancestors
private:
  mozSpellChecker *mOwner;
=======
  RefPtr<CheckWordPromise> CheckWords(const nsTArray<nsString>& aWords);

 private:
  mozSpellChecker* mOwner;
>>>>>>> upstream-releases
};

}  // namespace mozilla

#endif  // RemoteSpellcheckEngineChild_h_
