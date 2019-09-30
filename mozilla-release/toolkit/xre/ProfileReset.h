/* -*- Mode: C++; tab-width: 2; indent-tabs-mode: nil; c-basic-offset: 2 -*- */
/* This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at http://mozilla.org/MPL/2.0/. */

#include "nsToolkitProfileService.h"
#include "nsIFile.h"
#include "nsThreadUtils.h"

static bool gProfileResetCleanupCompleted = false;
static const char kResetProgressURL[] =
    "chrome://global/content/resetProfileProgress.xul";

<<<<<<< HEAD
nsresult CreateResetProfile(nsIToolkitProfileService* aProfileSvc,
                            const nsACString& aOldProfileName,
                            nsIToolkitProfile** aNewProfile);
||||||| merged common ancestors
nsresult CreateResetProfile(nsIToolkitProfileService* aProfileSvc,
                            const nsACString& aOldProfileName,
                            nsIToolkitProfile* *aNewProfile);
=======
nsresult ProfileResetCleanup(nsToolkitProfileService* aService,
                             nsIToolkitProfile* aOldProfile);
>>>>>>> upstream-releases

<<<<<<< HEAD
nsresult ProfileResetCleanup(nsIToolkitProfile* aOldProfile);

class ProfileResetCleanupResultTask : public mozilla::Runnable {
 public:
||||||| merged common ancestors
nsresult ProfileResetCleanup(nsIToolkitProfile* aOldProfile);

class ProfileResetCleanupResultTask : public mozilla::Runnable
{
public:
=======
class ProfileResetCleanupResultTask : public mozilla::Runnable {
 public:
>>>>>>> upstream-releases
  ProfileResetCleanupResultTask()
      : mozilla::Runnable("ProfileResetCleanupResultTask"),
        mWorkerThread(do_GetCurrentThread()) {
    MOZ_ASSERT(!NS_IsMainThread());
  }

  NS_IMETHOD Run() override {
    MOZ_ASSERT(NS_IsMainThread());
    mWorkerThread->Shutdown();
    return NS_OK;
  }

 private:
  nsCOMPtr<nsIThread> mWorkerThread;
};

class ProfileResetCleanupAsyncTask : public mozilla::Runnable {
 public:
  ProfileResetCleanupAsyncTask(nsIFile* aProfileDir, nsIFile* aProfileLocalDir,
                               nsIFile* aTargetDir, const nsAString& aLeafName)
      : mozilla::Runnable("ProfileResetCleanupAsyncTask"),
        mProfileDir(aProfileDir),
        mProfileLocalDir(aProfileLocalDir),
        mTargetDir(aTargetDir),
        mLeafName(aLeafName) {}

<<<<<<< HEAD
  /**
   * Copy a root profile to a backup folder before deleting it.  Then delete the
   * local profile dir.
   */
  NS_IMETHOD Run() override {
    // Copy to the destination then delete the profile. A move doesn't follow
    // links.
||||||| merged common ancestors
/**
 * Copy a root profile to a backup folder before deleting it.  Then delete the local profile dir.
 */
  NS_IMETHOD Run() override
  {
    // Copy to the destination then delete the profile. A move doesn't follow links.
=======
  /**
   * Copy a root profile to a backup folder before deleting it.  Then delete the
   * local profile dir.
   */
  NS_IMETHOD Run() override {
    // Copy profile's files to the destination. The profile folder will be
    // removed after the changes to the known profiles have been flushed to disk
    // in nsToolkitProfileService::ApplyResetProfile which isn't called until
    // after this thread finishes copying the files.
>>>>>>> upstream-releases
    nsresult rv = mProfileDir->CopyToFollowingLinks(mTargetDir, mLeafName);
<<<<<<< HEAD
    if (NS_SUCCEEDED(rv)) rv = mProfileDir->Remove(true);
||||||| merged common ancestors
    if (NS_SUCCEEDED(rv))
      rv = mProfileDir->Remove(true);
=======
    // I guess we just warn if we fail to make the backup?
>>>>>>> upstream-releases
    if (NS_WARN_IF(NS_FAILED(rv))) {
      NS_WARNING("Could not backup the root profile directory");
    }

    // If we have a separate local cache profile directory, just delete it.
    // Don't return an error if this fails so that reset can proceed if it can't
    // be deleted.
    bool sameDir;
    nsresult rvLocal = mProfileDir->Equals(mProfileLocalDir, &sameDir);
    if (NS_SUCCEEDED(rvLocal) && !sameDir) {
      rvLocal = mProfileLocalDir->Remove(true);
<<<<<<< HEAD
      if (NS_FAILED(rvLocal))
        NS_WARNING("Could not remove the old local profile directory (cache)");
||||||| merged common ancestors
      if (NS_FAILED(rvLocal)) NS_WARNING("Could not remove the old local profile directory (cache)");
=======
      if (NS_FAILED(rvLocal)) {
        NS_WARNING("Could not remove the old local profile directory (cache)");
      }
>>>>>>> upstream-releases
    }
    gProfileResetCleanupCompleted = true;

    nsCOMPtr<nsIRunnable> resultRunnable = new ProfileResetCleanupResultTask();
    NS_DispatchToMainThread(resultRunnable);
    return NS_OK;
  }

 private:
  nsCOMPtr<nsIFile> mProfileDir;
  nsCOMPtr<nsIFile> mProfileLocalDir;
  nsCOMPtr<nsIFile> mTargetDir;
  nsString mLeafName;
};
