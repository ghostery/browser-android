/* This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/. */

#ifndef nsHtml5Module_h
#define nsHtml5Module_h

#include "nsIThread.h"

<<<<<<< HEAD
class nsHtml5Module {
 public:
||||||| merged common ancestors
class nsHtml5Module
{
public:
=======
class nsHtml5Parser;

class nsHtml5Module {
 public:
>>>>>>> upstream-releases
  static void InitializeStatics();
  static void ReleaseStatics();
<<<<<<< HEAD
  static already_AddRefed<nsIParser> NewHtml5Parser();
  static nsresult Initialize(nsIParser* aParser, nsIDocument* aDoc,
                             nsIURI* aURI, nsISupports* aContainer,
                             nsIChannel* aChannel);
||||||| merged common ancestors
  static already_AddRefed<nsIParser> NewHtml5Parser();
  static nsresult Initialize(nsIParser* aParser,
                             nsIDocument* aDoc,
                             nsIURI* aURI,
                             nsISupports* aContainer,
                             nsIChannel* aChannel);
=======
  static already_AddRefed<nsHtml5Parser> NewHtml5Parser();
>>>>>>> upstream-releases
  static nsIThread* GetStreamParserThread();

 private:
#ifdef DEBUG
  static bool sNsHtml5ModuleInitialized;
#endif
  static nsIThread* sStreamParserThread;
  static nsIThread* sMainThread;
};

#endif  // nsHtml5Module_h
