/* -*- Mode: C++; tab-width: 2; indent-tabs-mode: nil; c-basic-offset: 2 -*-
 * vim:expandtab:shiftwidth=2:tabstop=2:cin:
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/. */

#ifndef __nsLocalHandlerAppImpl_h__
#define __nsLocalHandlerAppImpl_h__

#include "nsString.h"
#include "nsIMIMEInfo.h"
#include "nsIFile.h"
#include "nsTArray.h"

class nsLocalHandlerApp : public nsILocalHandlerApp {
 public:
  NS_DECL_ISUPPORTS
  NS_DECL_NSIHANDLERAPP
  NS_DECL_NSILOCALHANDLERAPP

  nsLocalHandlerApp() {}

<<<<<<< HEAD
  nsLocalHandlerApp(const char16_t *aName, nsIFile *aExecutable)
      : mName(aName), mExecutable(aExecutable) {}
||||||| merged common ancestors
  nsLocalHandlerApp(const char16_t *aName, nsIFile *aExecutable) 
    : mName(aName), mExecutable(aExecutable) { }
=======
  nsLocalHandlerApp(const char16_t* aName, nsIFile* aExecutable)
      : mName(aName), mExecutable(aExecutable) {}
>>>>>>> upstream-releases

<<<<<<< HEAD
  nsLocalHandlerApp(const nsAString &aName, nsIFile *aExecutable)
      : mName(aName), mExecutable(aExecutable) {}
||||||| merged common ancestors
  nsLocalHandlerApp(const nsAString & aName, nsIFile *aExecutable) 
    : mName(aName), mExecutable(aExecutable) { }
=======
  nsLocalHandlerApp(const nsAString& aName, nsIFile* aExecutable)
      : mName(aName), mExecutable(aExecutable) {}
>>>>>>> upstream-releases

 protected:
  virtual ~nsLocalHandlerApp() {}

  nsString mName;
  nsString mDetailedDescription;
  nsTArray<nsString> mParameters;
  nsCOMPtr<nsIFile> mExecutable;

  /**
   * Launches this application with a single argument (typically either
   * a file path or a URI spec).  This is meant as a helper method for
   * implementations of (e.g.) LaunchWithURI.
   *
   * @param aApp The application to launch (may not be null)
   * @param aArg The argument to pass on the command line
   */
  nsresult LaunchWithIProcess(const nsCString& aArg);
};

// any platforms that need a platform-specific class instead of just
// using nsLocalHandlerApp need to add an include and a typedef here.
#ifdef XP_MACOSX
<<<<<<< HEAD
#ifndef NSLOCALHANDLERAPPMAC_H_
#include "mac/nsLocalHandlerAppMac.h"
||||||| merged common ancestors
# ifndef NSLOCALHANDLERAPPMAC_H_  
# include "mac/nsLocalHandlerAppMac.h"
=======
#  ifndef NSLOCALHANDLERAPPMAC_H_
#    include "mac/nsLocalHandlerAppMac.h"
>>>>>>> upstream-releases
typedef nsLocalHandlerAppMac PlatformLocalHandlerApp_t;
<<<<<<< HEAD
#endif
#else
||||||| merged common ancestors
# endif
#else 
=======
#  endif
#else
>>>>>>> upstream-releases
typedef nsLocalHandlerApp PlatformLocalHandlerApp_t;
#endif

#endif  //  __nsLocalHandlerAppImpl_h__
