/* -*- Mode: C++; tab-width: 4; indent-tabs-mode: nil; c-basic-offset: 2 -*- */
/* This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/. */

#include "nsCOMPtr.h"
#include "nsString.h"
#include "nsIServiceManager.h"
#include "nsICategoryManager.h"
#include "nsXPCOM.h"
#include "nsISupportsPrimitives.h"
#include "nsAppStartupNotifier.h"
#include "mozilla/SimpleEnumerator.h"

<<<<<<< HEAD
/* static */ nsresult nsAppStartupNotifier::NotifyObservers(
    const char* aTopic) {
  NS_ENSURE_ARG(aTopic);
  nsresult rv;
||||||| merged common ancestors
/* static */ nsresult
nsAppStartupNotifier::NotifyObservers(const char* aTopic)
{
    NS_ENSURE_ARG(aTopic);
    nsresult rv;
=======
/* static */
nsresult nsAppStartupNotifier::NotifyObservers(const char* aCategory) {
  NS_ENSURE_ARG(aCategory);
  nsresult rv;
>>>>>>> upstream-releases

  // now initialize all startup listeners
  nsCOMPtr<nsICategoryManager> categoryManager =
      do_GetService(NS_CATEGORYMANAGER_CONTRACTID, &rv);
  NS_ENSURE_SUCCESS(rv, rv);

<<<<<<< HEAD
  nsDependentCString topic(aTopic);
||||||| merged common ancestors
    nsDependentCString topic(aTopic);
=======
  nsDependentCString category(aCategory);
>>>>>>> upstream-releases

<<<<<<< HEAD
  nsCOMPtr<nsISimpleEnumerator> enumerator;
  rv = categoryManager->EnumerateCategory(topic, getter_AddRefs(enumerator));
  if (NS_FAILED(rv)) return rv;
||||||| merged common ancestors
    nsCOMPtr<nsISimpleEnumerator> enumerator;
    rv = categoryManager->EnumerateCategory(topic,
                               getter_AddRefs(enumerator));
    if (NS_FAILED(rv)) return rv;
=======
  nsCOMPtr<nsISimpleEnumerator> enumerator;
  rv = categoryManager->EnumerateCategory(category, getter_AddRefs(enumerator));
  if (NS_WARN_IF(NS_FAILED(rv))) {
    return rv;
  }
>>>>>>> upstream-releases

<<<<<<< HEAD
  nsCOMPtr<nsISupports> entry;
  while (NS_SUCCEEDED(enumerator->GetNext(getter_AddRefs(entry)))) {
    nsCOMPtr<nsISupportsCString> category = do_QueryInterface(entry, &rv);
||||||| merged common ancestors
    nsCOMPtr<nsISupports> entry;
    while (NS_SUCCEEDED(enumerator->GetNext(getter_AddRefs(entry)))) {
        nsCOMPtr<nsISupportsCString> category = do_QueryInterface(entry, &rv);
=======
  for (auto& categoryEntry : SimpleEnumerator<nsICategoryEntry>(enumerator)) {
    nsAutoCString contractId;
    categoryEntry->GetValue(contractId);
>>>>>>> upstream-releases

<<<<<<< HEAD
    if (NS_SUCCEEDED(rv)) {
      nsAutoCString categoryEntry;
      rv = category->GetData(categoryEntry);

      nsCString contractId;
      categoryManager->GetCategoryEntry(topic, categoryEntry, contractId);

      if (NS_SUCCEEDED(rv)) {
        // If we see the word "service," in the beginning
        // of the contractId then we create it as a service
        // if not we do a createInstance

        nsCOMPtr<nsISupports> startupInstance;
        if (Substring(contractId, 0, 8).EqualsLiteral("service,"))
          startupInstance = do_GetService(contractId.get() + 8, &rv);
        else
          startupInstance = do_CreateInstance(contractId.get(), &rv);

        if (NS_SUCCEEDED(rv)) {
          // Try to QI to nsIObserver
          nsCOMPtr<nsIObserver> startupObserver =
              do_QueryInterface(startupInstance, &rv);
          if (NS_SUCCEEDED(rv)) {
            rv = startupObserver->Observe(nullptr, aTopic, nullptr);
||||||| merged common ancestors
        if (NS_SUCCEEDED(rv)) {
            nsAutoCString categoryEntry;
            rv = category->GetData(categoryEntry);

            nsCString contractId;
            categoryManager->GetCategoryEntry(topic, categoryEntry,
                                              contractId);

            if (NS_SUCCEEDED(rv)) {

                // If we see the word "service," in the beginning
                // of the contractId then we create it as a service
                // if not we do a createInstance

                nsCOMPtr<nsISupports> startupInstance;
                if (Substring(contractId, 0, 8).EqualsLiteral("service,"))
                    startupInstance = do_GetService(contractId.get() + 8, &rv);
                else
                    startupInstance = do_CreateInstance(contractId.get(), &rv);

                if (NS_SUCCEEDED(rv)) {
                    // Try to QI to nsIObserver
                    nsCOMPtr<nsIObserver> startupObserver =
                        do_QueryInterface(startupInstance, &rv);
                    if (NS_SUCCEEDED(rv)) {
                        rv = startupObserver->Observe(nullptr, aTopic, nullptr);

                        // mainly for debugging if you want to know if your observer worked.
                        NS_ASSERTION(NS_SUCCEEDED(rv), "Startup Observer failed!\n");
                    }
                }
                else {
                  #ifdef DEBUG
                    nsAutoCString warnStr("Cannot create startup observer : ");
                    warnStr += contractId.get();
                    NS_WARNING(warnStr.get());
                  #endif
                }
=======
    nsCOMPtr<nsISupports> startupInstance;

    // If we see the word "service," in the beginning
    // of the contractId then we create it as a service
    // if not we do a createInstance
    if (StringBeginsWith(contractId, NS_LITERAL_CSTRING("service,"))) {
      startupInstance = do_GetService(contractId.get() + 8, &rv);
    } else {
      startupInstance = do_CreateInstance(contractId.get(), &rv);
    }

    if (NS_SUCCEEDED(rv)) {
      // Try to QI to nsIObserver
      nsCOMPtr<nsIObserver> startupObserver =
          do_QueryInterface(startupInstance, &rv);
      if (NS_SUCCEEDED(rv)) {
        rv = startupObserver->Observe(nullptr, aCategory, nullptr);
>>>>>>> upstream-releases

<<<<<<< HEAD
            // mainly for debugging if you want to know if your observer worked.
            NS_ASSERTION(NS_SUCCEEDED(rv), "Startup Observer failed!\n");
          }
        } else {
#ifdef DEBUG
          nsAutoCString warnStr("Cannot create startup observer : ");
          warnStr += contractId.get();
          NS_WARNING(warnStr.get());
#endif
        }
      }
||||||| merged common ancestors
            }
        }
=======
        // mainly for debugging if you want to know if your observer worked.
        NS_ASSERTION(NS_SUCCEEDED(rv), "Startup Observer failed!\n");
      }
    } else {
#ifdef DEBUG
      nsAutoCString warnStr("Cannot create startup observer : ");
      warnStr += contractId.get();
      NS_WARNING(warnStr.get());
#endif
>>>>>>> upstream-releases
    }
  }

  return NS_OK;
}
