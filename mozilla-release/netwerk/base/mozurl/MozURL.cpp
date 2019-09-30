/* This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/. */

<<<<<<< HEAD:mozilla-release/modules/libjar/nsIJARFactory.h
#ifndef nsIJARFactory_h__
#define nsIJARFactory_h__
||||||| merged common ancestors

#ifndef nsIJARFactory_h__
#define nsIJARFactory_h__
=======
#include "mozilla/StaticPrefs.h"

extern "C" {
>>>>>>> upstream-releases:mozilla-release/netwerk/base/mozurl/MozURL.cpp

<<<<<<< HEAD:mozilla-release/modules/libjar/nsIJARFactory.h
#endif  // nsIJARFactory_h__
||||||| merged common ancestors
#endif // nsIJARFactory_h__
=======
bool Gecko_StrictFileOriginPolicy() {
  return mozilla::StaticPrefs::security_fileuri_strict_origin_policy();
}
}
>>>>>>> upstream-releases:mozilla-release/netwerk/base/mozurl/MozURL.cpp
