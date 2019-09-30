//* -*- Mode: C++; tab-width: 8; indent-tabs-mode: nil; c-basic-offset: 2 -*-/
/* This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/. */

#ifndef nsUrlClassifierDBService_h_
#define nsUrlClassifierDBService_h_

#include <nsISupportsUtils.h>

#include "nsID.h"
#include "nsInterfaceHashtable.h"
#include "nsIObserver.h"
#include "nsUrlClassifierPrefixSet.h"
#include "nsIUrlClassifierHashCompleter.h"
#include "nsIUrlListManager.h"
#include "nsIUrlClassifierDBService.h"
#include "nsIUrlClassifierInfo.h"
#include "nsIURIClassifier.h"
#include "nsICryptoHMAC.h"
#include "mozilla/Attributes.h"
#include "mozilla/Mutex.h"
#include "mozilla/TimeStamp.h"

#include "Entries.h"
#include "LookupCache.h"
#include "HashStore.h"

// GCC < 6.1 workaround, see bug 1329593
#if defined(XP_WIN) && defined(__MINGW32__)
#  define GCC_MANGLING_WORKAROUND __stdcall
#else
#  define GCC_MANGLING_WORKAROUND
#endif

// The hash length for a domain key.
#define DOMAIN_LENGTH 4

// The hash length of a partial hash entry.
#define PARTIAL_LENGTH 4

// The hash length of a complete hash entry.
#define COMPLETE_LENGTH 32

<<<<<<< HEAD
// Prefs for implementing nsIURIClassifier to block page loads
#define CHECK_MALWARE_PREF "browser.safebrowsing.malware.enabled"
#define CHECK_MALWARE_DEFAULT false

#define CHECK_PHISHING_PREF "browser.safebrowsing.phishing.enabled"
#define CHECK_PHISHING_DEFAULT false

#define CHECK_BLOCKED_PREF "browser.safebrowsing.blockedURIs.enabled"
#define CHECK_BLOCKED_DEFAULT false

||||||| merged common ancestors
// Prefs for implementing nsIURIClassifier to block page loads
#define CHECK_MALWARE_PREF      "browser.safebrowsing.malware.enabled"
#define CHECK_MALWARE_DEFAULT   false

#define CHECK_PHISHING_PREF     "browser.safebrowsing.phishing.enabled"
#define CHECK_PHISHING_DEFAULT  false

#define CHECK_BLOCKED_PREF      "browser.safebrowsing.blockedURIs.enabled"
#define CHECK_BLOCKED_DEFAULT   false

=======
>>>>>>> upstream-releases
// Comma-separated lists
<<<<<<< HEAD
#define MALWARE_TABLE_PREF "urlclassifier.malwareTable"
#define PHISH_TABLE_PREF "urlclassifier.phishTable"
#define TRACKING_TABLE_PREF "urlclassifier.trackingTable"
#define TRACKING_TABLE_TEST_ENTRIES_PREF \
  "urlclassifier.trackingTable.testEntries"
#define TRACKING_WHITELIST_TABLE_PREF "urlclassifier.trackingWhitelistTable"
#define TRACKING_WHITELIST_TABLE_TEST_ENTRIES_PREF \
  "urlclassifier.trackingWhitelistTable.testEntries"
#define BLOCKED_TABLE_PREF "urlclassifier.blockedTable"
#define DOWNLOAD_BLOCK_TABLE_PREF "urlclassifier.downloadBlockTable"
#define DOWNLOAD_ALLOW_TABLE_PREF "urlclassifier.downloadAllowTable"
#define DISALLOW_COMPLETION_TABLE_PREF "urlclassifier.disallow_completions"
#define PASSWORD_ALLOW_TABLE_PREF "urlclassifier.passwordAllowTable"
||||||| merged common ancestors
#define MALWARE_TABLE_PREF              "urlclassifier.malwareTable"
#define PHISH_TABLE_PREF                "urlclassifier.phishTable"
#define TRACKING_TABLE_PREF             "urlclassifier.trackingTable"
#define TRACKING_TABLE_TEST_ENTRIES_PREF "urlclassifier.trackingTable.testEntries"
#define TRACKING_WHITELIST_TABLE_PREF   "urlclassifier.trackingWhitelistTable"
#define TRACKING_WHITELIST_TABLE_TEST_ENTRIES_PREF "urlclassifier.trackingWhitelistTable.testEntries"
#define BLOCKED_TABLE_PREF              "urlclassifier.blockedTable"
#define DOWNLOAD_BLOCK_TABLE_PREF       "urlclassifier.downloadBlockTable"
#define DOWNLOAD_ALLOW_TABLE_PREF       "urlclassifier.downloadAllowTable"
#define DISALLOW_COMPLETION_TABLE_PREF  "urlclassifier.disallow_completions"
#define PASSWORD_ALLOW_TABLE_PREF       "urlclassifier.passwordAllowTable"
=======
#define DISALLOW_COMPLETION_TABLE_PREF "urlclassifier.disallow_completions"
>>>>>>> upstream-releases

using namespace mozilla::safebrowsing;

class nsUrlClassifierDBServiceWorker;
class nsIThread;
class nsIURI;
class UrlClassifierDBServiceWorkerProxy;

namespace mozilla {

namespace safebrowsing {
class Classifier;
class ProtocolParser;

nsresult TablesToResponse(const nsACString& tables);

<<<<<<< HEAD
}  // namespace safebrowsing
}  // namespace mozilla
||||||| merged common ancestors
} // namespace safebrowsing
} // namespace mozilla
=======
}  // namespace safebrowsing

namespace net {
class AsyncUrlChannelClassifier;
}

}  // namespace mozilla
>>>>>>> upstream-releases

// This is a proxy class that just creates a background thread and delegates
// calls to the background thread.
class nsUrlClassifierDBService final : public nsIUrlClassifierDBService,
                                       public nsIURIClassifier,
                                       public nsIUrlClassifierInfo,
<<<<<<< HEAD
                                       public nsIObserver {
 public:
||||||| merged common ancestors
                                       public nsIObserver
{
public:
=======
                                       public nsIObserver {
  friend class mozilla::net::AsyncUrlChannelClassifier;

 public:
  class FeatureHolder;

>>>>>>> upstream-releases
  // This is thread safe. It throws an exception if the thread is busy.
  nsUrlClassifierDBService();

  nsresult Init();

<<<<<<< HEAD
  static nsUrlClassifierDBService* GetInstance(nsresult* result);
||||||| merged common ancestors
  static nsUrlClassifierDBService* GetInstance(nsresult *result);
=======
  static already_AddRefed<nsUrlClassifierDBService> GetInstance(
      nsresult* result);
>>>>>>> upstream-releases

  NS_DECLARE_STATIC_IID_ACCESSOR(NS_URLCLASSIFIERDBSERVICE_CID)

  NS_DECL_THREADSAFE_ISUPPORTS
  NS_DECL_NSIURLCLASSIFIERDBSERVICE
  NS_DECL_NSIURICLASSIFIER
  NS_DECL_NSIURLCLASSIFIERINFO
  NS_DECL_NSIOBSERVER

  bool CanComplete(const nsACString& tableName);
  bool GetCompleter(const nsACString& tableName,
                    nsIUrlClassifierHashCompleter** completer);
  nsresult CacheCompletions(
      const mozilla::safebrowsing::ConstCacheResultArray& results);

  static nsIThread* BackgroundThread();

  static bool ShutdownHasStarted();

<<<<<<< HEAD
 private:
  const nsTArray<nsCString> kObservedPrefs = {
      NS_LITERAL_CSTRING(CHECK_MALWARE_PREF),
      NS_LITERAL_CSTRING(CHECK_PHISHING_PREF),
      NS_LITERAL_CSTRING(CHECK_BLOCKED_PREF),
      NS_LITERAL_CSTRING(MALWARE_TABLE_PREF),
      NS_LITERAL_CSTRING(PHISH_TABLE_PREF),
      NS_LITERAL_CSTRING(TRACKING_TABLE_PREF),
      NS_LITERAL_CSTRING(TRACKING_TABLE_TEST_ENTRIES_PREF),
      NS_LITERAL_CSTRING(TRACKING_WHITELIST_TABLE_PREF),
      NS_LITERAL_CSTRING(TRACKING_WHITELIST_TABLE_TEST_ENTRIES_PREF),
      NS_LITERAL_CSTRING(BLOCKED_TABLE_PREF),
      NS_LITERAL_CSTRING(DOWNLOAD_BLOCK_TABLE_PREF),
      NS_LITERAL_CSTRING(DOWNLOAD_ALLOW_TABLE_PREF),
      NS_LITERAL_CSTRING(DISALLOW_COMPLETION_TABLE_PREF)};
||||||| merged common ancestors
private:

  const nsTArray<nsCString> kObservedPrefs = {
    NS_LITERAL_CSTRING(CHECK_MALWARE_PREF),
    NS_LITERAL_CSTRING(CHECK_PHISHING_PREF),
    NS_LITERAL_CSTRING(CHECK_BLOCKED_PREF),
    NS_LITERAL_CSTRING(MALWARE_TABLE_PREF),
    NS_LITERAL_CSTRING(PHISH_TABLE_PREF),
    NS_LITERAL_CSTRING(TRACKING_TABLE_PREF),
    NS_LITERAL_CSTRING(TRACKING_TABLE_TEST_ENTRIES_PREF),
    NS_LITERAL_CSTRING(TRACKING_WHITELIST_TABLE_PREF),
    NS_LITERAL_CSTRING(TRACKING_WHITELIST_TABLE_TEST_ENTRIES_PREF),
    NS_LITERAL_CSTRING(BLOCKED_TABLE_PREF),
    NS_LITERAL_CSTRING(DOWNLOAD_BLOCK_TABLE_PREF),
    NS_LITERAL_CSTRING(DOWNLOAD_ALLOW_TABLE_PREF),
    NS_LITERAL_CSTRING(DISALLOW_COMPLETION_TABLE_PREF)
  };
=======
 private:
  // This method is used only by AsyncUrlChannelClassifier. If you want to use
  // it, please contact a safebrowsing/URL-Classifier peer.
  static nsUrlClassifierDBServiceWorker* GetWorker();
>>>>>>> upstream-releases

  // No subclassing
  ~nsUrlClassifierDBService();

  // Disallow copy constructor
  nsUrlClassifierDBService(nsUrlClassifierDBService&);

<<<<<<< HEAD
  nsresult LookupURI(nsIPrincipal* aPrincipal, const nsACString& tables,
                     const nsTArray<nsCString>& extraTablesByPrefs,
                     const nsTArray<nsCString>& extraEntriesByPrefs,
                     nsIUrlClassifierCallback* c, bool forceCheck,
                     bool* didCheck);
||||||| merged common ancestors
  nsresult LookupURI(nsIPrincipal* aPrincipal,
                     const nsACString& tables,
                     const nsTArray<nsCString>& extraTablesByPrefs,
                     const nsTArray<nsCString>& extraEntriesByPrefs,
                     nsIUrlClassifierCallback* c,
                     bool forceCheck, bool *didCheck);
=======
  nsresult LookupURI(const nsACString& aKey, FeatureHolder* aHolder,
                     nsIUrlClassifierCallback* c);
>>>>>>> upstream-releases

  // Post an event to worker thread to release objects when receive
  // 'quit-application'
  nsresult PreShutdown();

  // Close db connection and join the background thread if it exists.
  nsresult Shutdown();

  nsresult ReadDisallowCompletionsTablesFromPrefs();

  // This method checks if the classification can be done just using
  // preferences. It returns true if the operation has been completed.
  bool AsyncClassifyLocalWithFeaturesUsingPreferences(
      nsIURI* aURI, const nsTArray<RefPtr<nsIUrlClassifierFeature>>& aFeatures,
      nsIUrlClassifierFeature::listType aListType,
      nsIUrlClassifierFeatureCallback* aCallback);

  RefPtr<nsUrlClassifierDBServiceWorker> mWorker;
  RefPtr<UrlClassifierDBServiceWorkerProxy> mWorkerProxy;

<<<<<<< HEAD
  nsInterfaceHashtable<nsCStringHashKey, nsIUrlClassifierHashCompleter>
      mCompleters;

  // TRUE if the nsURIClassifier implementation should check for malware
  // uris on document loads.
  bool mCheckMalware;

  // TRUE if the nsURIClassifier implementation should check for phishing
  // uris on document loads.
  bool mCheckPhishing;

  // TRUE if the nsURIClassifier implementation should check for blocked
  // uris on document loads.
  bool mCheckBlockedURIs;
||||||| merged common ancestors
  nsInterfaceHashtable<nsCStringHashKey, nsIUrlClassifierHashCompleter> mCompleters;

  // TRUE if the nsURIClassifier implementation should check for malware
  // uris on document loads.
  bool mCheckMalware;

  // TRUE if the nsURIClassifier implementation should check for phishing
  // uris on document loads.
  bool mCheckPhishing;

  // TRUE if the nsURIClassifier implementation should check for blocked
  // uris on document loads.
  bool mCheckBlockedURIs;
=======
  nsInterfaceHashtable<nsCStringHashKey, nsIUrlClassifierHashCompleter>
      mCompleters;
>>>>>>> upstream-releases

  // TRUE if a BeginUpdate() has been called without an accompanying
  // CancelUpdate()/FinishUpdate().  This is used to prevent competing
  // updates, not to determine whether an update is still being
  // processed.
  bool mInUpdate;

  // The list of tables that should never be hash completed.
  nsTArray<nsCString> mDisallowCompletionsTables;

  // Thread that we do the updates on.
  static nsIThread* gDbBackgroundThread;
};

class nsUrlClassifierDBServiceWorker final : public nsIUrlClassifierDBService {
 public:
  nsUrlClassifierDBServiceWorker();

  NS_DECL_THREADSAFE_ISUPPORTS
  NS_DECL_NSIURLCLASSIFIERDBSERVICE

  nsresult Init(uint32_t aGethashNoise, nsCOMPtr<nsIFile> aCacheDir,
                nsUrlClassifierDBService* aDBService);

  // Queue a lookup for the worker to perform, called in the main thread.
<<<<<<< HEAD
  // tables is a comma-separated list of tables to query
  nsresult QueueLookup(const nsACString& lookupKey, const nsACString& tables,
                       nsIUrlClassifierLookupCallback* callback);
||||||| merged common ancestors
  // tables is a comma-separated list of tables to query
  nsresult QueueLookup(const nsACString& lookupKey,
                       const nsACString& tables,
                       nsIUrlClassifierLookupCallback* callback);
=======
  nsresult QueueLookup(const nsACString& aLookupKey,
                       nsUrlClassifierDBService::FeatureHolder* aFeatureHolder,
                       nsIUrlClassifierLookupCallback* aLallback);
>>>>>>> upstream-releases

  // Handle any queued-up lookups.  We call this function during long-running
  // update operations to prevent lookups from blocking for too long.
  nsresult HandlePendingLookups();

<<<<<<< HEAD
  // Perform a blocking classifier lookup for a given url. Can be called on
  // either the main thread or the worker thread.
  nsresult DoLocalLookup(const nsACString& spec, const nsACString& tables,
                         LookupResultArray& results);
||||||| merged common ancestors
  // Perform a blocking classifier lookup for a given url. Can be called on
  // either the main thread or the worker thread.
  nsresult DoLocalLookup(const nsACString& spec,
                         const nsACString& tables,
                         LookupResultArray& results);
=======
  // Perform a blocking classifier lookup for a given url fragments set.
  // Can be called on either the main thread or the worker thread.
  nsresult DoSingleLocalLookupWithURIFragments(
      const nsTArray<nsCString>& aSpecFragments, const nsACString& aTable,
      LookupResultArray& aResults);
>>>>>>> upstream-releases

  // Open the DB connection
  nsresult GCC_MANGLING_WORKAROUND OpenDb();

  // Provide a way to forcibly close the db connection.
  nsresult GCC_MANGLING_WORKAROUND CloseDb();

  nsresult GCC_MANGLING_WORKAROUND PreShutdown();

  nsresult CacheCompletions(const ConstCacheResultArray& aEntries);

  // Used to probe the state of the worker thread. When the update begins,
  // mUpdateObserver will be set. When the update finished, mUpdateObserver
  // will be nulled out in NotifyUpdateObserver.
  bool IsBusyUpdating() const { return !!mUpdateObserver; }

  // Check the DB ready state of the worker thread
  bool IsDBOpened() const { return !!mClassifier; }

  // Delegate Classifier to disable async update. If there is an
  // ongoing update on the update thread, we will be blocked until
  // the background update is done and callback is fired.
  // Should be called on the worker thread.
  void FlushAndDisableAsyncUpdate();

  // A synchronous call to get cache information for the given table.
  // This is only used by about:url-classifier now.
  nsresult GetCacheInfo(const nsACString& aTable,
                        nsIUrlClassifierCacheInfo** aCache);

 private:
  // No subclassing
  ~nsUrlClassifierDBServiceWorker();

  // Disallow copy constructor
  nsUrlClassifierDBServiceWorker(nsUrlClassifierDBServiceWorker&);

  nsresult NotifyUpdateObserver(nsresult aUpdateStatus);

  // Reset the in-progress update stream
  void ResetStream();

  // Reset the in-progress update
  void ResetUpdate();

  // Perform a classifier lookup for a given url.
<<<<<<< HEAD
  nsresult DoLookup(const nsACString& spec, const nsACString& tables,
||||||| merged common ancestors
  nsresult DoLookup(const nsACString& spec,
                    const nsACString& tables,
=======
  nsresult DoLookup(const nsACString& spec,
                    nsUrlClassifierDBService::FeatureHolder* aFeatureHolder,
>>>>>>> upstream-releases
                    nsIUrlClassifierLookupCallback* c);

  nsresult AddNoise(const Prefix aPrefix, const nsCString tableName,
                    uint32_t aCount, LookupResultArray& results);

  nsresult CacheResultToTableUpdate(RefPtr<const CacheResult> aCacheResult,
                                    RefPtr<TableUpdate> aUpdate);

  bool IsSameAsLastResults(const ConstCacheResultArray& aResult) const;

  RefPtr<mozilla::safebrowsing::Classifier> mClassifier;
  // The class that actually parses the update chunks.
  mozilla::UniquePtr<ProtocolParser> mProtocolParser;

  // Directory where to store the SB databases.
  nsCOMPtr<nsIFile> mCacheDir;

  RefPtr<nsUrlClassifierDBService> mDBService;

  TableUpdateArray mTableUpdates;

  uint32_t mUpdateWaitSec;

  // Stores the last results that triggered a table update.
  ConstCacheResultArray mLastResults;

  nsresult mUpdateStatus;
  nsTArray<nsCString> mUpdateTables;

  nsCOMPtr<nsIUrlClassifierUpdateObserver> mUpdateObserver;
  bool mInStream;

  // The number of noise entries to add to the set of lookup results.
  uint32_t mGethashNoise;

  // Pending lookups are stored in a queue for processing.  The queue
  // is protected by mPendingLookupLock.
  mozilla::Mutex mPendingLookupLock;

  class PendingLookup {
   public:
    mozilla::TimeStamp mStartTime;
    nsCString mKey;
    RefPtr<nsUrlClassifierDBService::FeatureHolder> mFeatureHolder;
    nsCOMPtr<nsIUrlClassifierLookupCallback> mCallback;
  };

  // list of pending lookups
  nsTArray<PendingLookup> mPendingLookups;

#ifdef MOZ_SAFEBROWSING_DUMP_FAILED_UPDATES
  // The raw update response for debugging.
  nsCString mRawTableUpdates;
#endif
};

NS_DEFINE_STATIC_IID_ACCESSOR(nsUrlClassifierDBService,
                              NS_URLCLASSIFIERDBSERVICE_CID)

#endif  // nsUrlClassifierDBService_h_
