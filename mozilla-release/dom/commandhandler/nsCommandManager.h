/* -*- Mode: C++; tab-width: 8; indent-tabs-mode: nil; c-basic-offset: 2 -*- */
/* vim: set ts=8 sts=2 et sw=2 tw=80: */
/* This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/. */

#ifndef nsCommandManager_h__
#define nsCommandManager_h__

#include "nsString.h"
#include "nsClassHashtable.h"
#include "nsWeakReference.h"

#include "nsICommandManager.h"
#include "nsCycleCollectionParticipant.h"

class nsIController;
<<<<<<< HEAD
template <class E>
class nsCOMArray;

class nsCommandManager : public nsICommandManager,
                         public nsPICommandUpdater,
                         public nsSupportsWeakReference {
 public:
||||||| merged common ancestors
template<class E> class nsCOMArray;

class nsCommandManager
  : public nsICommandManager
  , public nsPICommandUpdater
  , public nsSupportsWeakReference
{
public:
=======
template <class E>
class nsCOMArray;

class nsCommandManager final : public nsICommandManager,
                               public nsSupportsWeakReference {
 public:
>>>>>>> upstream-releases
  typedef nsTArray<nsCOMPtr<nsIObserver> > ObserverList;

  nsCommandManager() = delete;

  /**
   * @param aWindow     An window which is what this command manager lives on.
   */
  explicit nsCommandManager(mozIDOMWindowProxy* aWindow);

  NS_DECL_CYCLE_COLLECTING_ISUPPORTS
  NS_DECL_CYCLE_COLLECTION_CLASS_AMBIGUOUS(nsCommandManager, nsICommandManager)

  NS_DECL_NSICOMMANDMANAGER

<<<<<<< HEAD
 protected:
||||||| merged common ancestors
protected:
=======
  /**
   * Notify the command manager that the status of a command changed. It may
   * have changed from enabled to disabled, or vice versa, or become toggled
   * etc.
   */
  nsresult CommandStatusChanged(const char* aCommandName);

  bool IsCommandEnabled(const nsCString& aCommandName,
                        mozIDOMWindowProxy* aTargetWindow);

 protected:
>>>>>>> upstream-releases
  virtual ~nsCommandManager();

  nsresult GetControllerForCommand(const char* aCommand,
                                   mozIDOMWindowProxy* aDirectedToThisWindow,
                                   nsIController** aResult);

 protected:
  nsClassHashtable<nsCharPtrHashKey, ObserverList> mObserversTable;

  mozIDOMWindowProxy* mWindow;  // weak ptr. The window should always outlive us
};

<<<<<<< HEAD
#endif  // nsCommandManager_h__
||||||| merged common ancestors
#endif // nsCommandManager_h__
=======
nsCommandManager* nsICommandManager::AsCommandManager() {
  return static_cast<nsCommandManager*>(this);
}

const nsCommandManager* nsICommandManager::AsCommandManager() const {
  return static_cast<const nsCommandManager*>(this);
}

#endif  // nsCommandManager_h__
>>>>>>> upstream-releases
