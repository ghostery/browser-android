/* -*- Mode: C++; tab-width: 2; indent-tabs-mode: nil; c-basic-offset: 2 -*- */
/* This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/. */

#ifndef mozilla_HTMLEditorController_h__
#define mozilla_HTMLEditorController_h__

<<<<<<< HEAD
#include "nscore.h"  // for nsresult

class nsIControllerCommandTable;
||||||| merged common ancestors

#include "nscore.h"                     // for nsresult

class nsIControllerCommandTable;
=======
#include "nscore.h"  // for nsresult

class nsControllerCommandTable;
>>>>>>> upstream-releases

namespace mozilla {

<<<<<<< HEAD
class HTMLEditorController final {
 public:
  static nsresult RegisterEditorDocStateCommands(
      nsIControllerCommandTable* inCommandTable);
  static nsresult RegisterHTMLEditorCommands(
      nsIControllerCommandTable* inCommandTable);
||||||| merged common ancestors
class HTMLEditorController final
{
public:
  static nsresult RegisterEditorDocStateCommands(nsIControllerCommandTable* inCommandTable);
  static nsresult RegisterHTMLEditorCommands(nsIControllerCommandTable* inCommandTable);
=======
class HTMLEditorController final {
 public:
  static nsresult RegisterEditorDocStateCommands(
      nsControllerCommandTable* aCommandTable);
  static nsresult RegisterHTMLEditorCommands(
      nsControllerCommandTable* aCommandTable);
  static void Shutdown();
>>>>>>> upstream-releases
};

}  // namespace mozilla

#endif /* mozllla_HTMLEditorController_h__ */
