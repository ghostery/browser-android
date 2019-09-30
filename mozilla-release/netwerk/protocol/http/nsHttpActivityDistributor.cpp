/* This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/. */

// HttpLog.h should generally be included first
#include "HttpLog.h"

#include "nsHttpActivityDistributor.h"
#include "nsCOMPtr.h"
#include "nsAutoPtr.h"
#include "nsThreadUtils.h"

namespace mozilla {
namespace net {

typedef nsMainThreadPtrHolder<nsIHttpActivityObserver> ObserverHolder;
typedef nsMainThreadPtrHandle<nsIHttpActivityObserver> ObserverHandle;
typedef nsTArray<ObserverHandle> ObserverArray;

<<<<<<< HEAD
class nsHttpActivityEvent : public Runnable {
 public:
  nsHttpActivityEvent(nsISupports *aHttpChannel, uint32_t aActivityType,
                      uint32_t aActivitySubtype, PRTime aTimestamp,
||||||| merged common ancestors
class nsHttpActivityEvent : public Runnable
{
public:
  nsHttpActivityEvent(nsISupports* aHttpChannel,
                      uint32_t aActivityType,
                      uint32_t aActivitySubtype,
                      PRTime aTimestamp,
=======
class nsHttpActivityEvent : public Runnable {
 public:
  nsHttpActivityEvent(nsISupports* aHttpChannel, uint32_t aActivityType,
                      uint32_t aActivitySubtype, PRTime aTimestamp,
>>>>>>> upstream-releases
                      uint64_t aExtraSizeData,
<<<<<<< HEAD
                      const nsACString &aExtraStringData,
                      ObserverArray *aObservers)
      : Runnable("net::nsHttpActivityEvent"),
        mHttpChannel(aHttpChannel),
        mActivityType(aActivityType),
        mActivitySubtype(aActivitySubtype),
        mTimestamp(aTimestamp),
        mExtraSizeData(aExtraSizeData),
        mExtraStringData(aExtraStringData),
        mObservers(*aObservers) {}

  NS_IMETHOD Run() override {
    for (size_t i = 0; i < mObservers.Length(); i++) {
      Unused << mObservers[i]->ObserveActivity(
          mHttpChannel, mActivityType, mActivitySubtype, mTimestamp,
          mExtraSizeData, mExtraStringData);
||||||| merged common ancestors
                      const nsACString& aExtraStringData,
                      ObserverArray* aObservers)
    : Runnable("net::nsHttpActivityEvent")
    , mHttpChannel(aHttpChannel)
    , mActivityType(aActivityType)
    , mActivitySubtype(aActivitySubtype)
    , mTimestamp(aTimestamp)
    , mExtraSizeData(aExtraSizeData)
    , mExtraStringData(aExtraStringData)
    , mObservers(*aObservers)
  {
    }

    NS_IMETHOD Run() override
    {
        for (size_t i = 0 ; i < mObservers.Length() ; i++) {
            Unused <<
                mObservers[i]->ObserveActivity(mHttpChannel, mActivityType,
                                               mActivitySubtype, mTimestamp,
                                               mExtraSizeData,
                                               mExtraStringData);
        }
        return NS_OK;
=======
                      const nsACString& aExtraStringData,
                      ObserverArray* aObservers)
      : Runnable("net::nsHttpActivityEvent"),
        mHttpChannel(aHttpChannel),
        mActivityType(aActivityType),
        mActivitySubtype(aActivitySubtype),
        mTimestamp(aTimestamp),
        mExtraSizeData(aExtraSizeData),
        mExtraStringData(aExtraStringData),
        mObservers(*aObservers) {}

  NS_IMETHOD Run() override {
    for (size_t i = 0; i < mObservers.Length(); i++) {
      Unused << mObservers[i]->ObserveActivity(
          mHttpChannel, mActivityType, mActivitySubtype, mTimestamp,
          mExtraSizeData, mExtraStringData);
>>>>>>> upstream-releases
    }
    return NS_OK;
  }

 private:
  virtual ~nsHttpActivityEvent() = default;

  nsCOMPtr<nsISupports> mHttpChannel;
  uint32_t mActivityType;
  uint32_t mActivitySubtype;
  PRTime mTimestamp;
  uint64_t mExtraSizeData;
  nsCString mExtraStringData;

  ObserverArray mObservers;
};

NS_IMPL_ISUPPORTS(nsHttpActivityDistributor, nsIHttpActivityDistributor,
                  nsIHttpActivityObserver)

nsHttpActivityDistributor::nsHttpActivityDistributor()
    : mLock("nsHttpActivityDistributor.mLock") {}

NS_IMETHODIMP
nsHttpActivityDistributor::ObserveActivity(nsISupports* aHttpChannel,
                                           uint32_t aActivityType,
                                           uint32_t aActivitySubtype,
                                           PRTime aTimestamp,
                                           uint64_t aExtraSizeData,
<<<<<<< HEAD
                                           const nsACString &aExtraStringData) {
  nsCOMPtr<nsIRunnable> event;
  {
    MutexAutoLock lock(mLock);

    if (!mObservers.Length()) return NS_OK;

    event = new nsHttpActivityEvent(
        aHttpChannel, aActivityType, aActivitySubtype, aTimestamp,
        aExtraSizeData, aExtraStringData, &mObservers);
  }
  NS_ENSURE_TRUE(event, NS_ERROR_OUT_OF_MEMORY);
  return NS_DispatchToMainThread(event);
||||||| merged common ancestors
                                           const nsACString & aExtraStringData)
{
    nsCOMPtr<nsIRunnable> event;
    {
        MutexAutoLock lock(mLock);

        if (!mObservers.Length())
            return NS_OK;

        event = new nsHttpActivityEvent(aHttpChannel, aActivityType,
                                        aActivitySubtype, aTimestamp,
                                        aExtraSizeData, aExtraStringData,
                                        &mObservers);
    }
    NS_ENSURE_TRUE(event, NS_ERROR_OUT_OF_MEMORY);
    return NS_DispatchToMainThread(event);
=======
                                           const nsACString& aExtraStringData) {
  nsCOMPtr<nsIRunnable> event;
  {
    MutexAutoLock lock(mLock);

    if (!mObservers.Length()) return NS_OK;

    event = new nsHttpActivityEvent(
        aHttpChannel, aActivityType, aActivitySubtype, aTimestamp,
        aExtraSizeData, aExtraStringData, &mObservers);
  }
  NS_ENSURE_TRUE(event, NS_ERROR_OUT_OF_MEMORY);
  return NS_DispatchToMainThread(event);
>>>>>>> upstream-releases
}

NS_IMETHODIMP
<<<<<<< HEAD
nsHttpActivityDistributor::GetIsActive(bool *isActive) {
  NS_ENSURE_ARG_POINTER(isActive);
  MutexAutoLock lock(mLock);
  *isActive = !!mObservers.Length();
  return NS_OK;
||||||| merged common ancestors
nsHttpActivityDistributor::GetIsActive(bool *isActive)
{
    NS_ENSURE_ARG_POINTER(isActive);
    MutexAutoLock lock(mLock);
    *isActive = !!mObservers.Length();
    return NS_OK;
=======
nsHttpActivityDistributor::GetIsActive(bool* isActive) {
  NS_ENSURE_ARG_POINTER(isActive);
  MutexAutoLock lock(mLock);
  *isActive = !!mObservers.Length();
  return NS_OK;
>>>>>>> upstream-releases
}

NS_IMETHODIMP
<<<<<<< HEAD
nsHttpActivityDistributor::AddObserver(nsIHttpActivityObserver *aObserver) {
  MutexAutoLock lock(mLock);
||||||| merged common ancestors
nsHttpActivityDistributor::AddObserver(nsIHttpActivityObserver *aObserver)
{
    MutexAutoLock lock(mLock);
=======
nsHttpActivityDistributor::AddObserver(nsIHttpActivityObserver* aObserver) {
  MutexAutoLock lock(mLock);
>>>>>>> upstream-releases

  ObserverHandle observer(
      new ObserverHolder("nsIHttpActivityObserver", aObserver));
  if (!mObservers.AppendElement(observer)) return NS_ERROR_OUT_OF_MEMORY;

  return NS_OK;
}

NS_IMETHODIMP
<<<<<<< HEAD
nsHttpActivityDistributor::RemoveObserver(nsIHttpActivityObserver *aObserver) {
  MutexAutoLock lock(mLock);
||||||| merged common ancestors
nsHttpActivityDistributor::RemoveObserver(nsIHttpActivityObserver *aObserver)
{
    MutexAutoLock lock(mLock);
=======
nsHttpActivityDistributor::RemoveObserver(nsIHttpActivityObserver* aObserver) {
  MutexAutoLock lock(mLock);
>>>>>>> upstream-releases

  ObserverHandle observer(
      new ObserverHolder("nsIHttpActivityObserver", aObserver));
  if (!mObservers.RemoveElement(observer)) return NS_ERROR_FAILURE;

  return NS_OK;
}

}  // namespace net
}  // namespace mozilla
