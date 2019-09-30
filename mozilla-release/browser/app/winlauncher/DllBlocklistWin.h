/* -*- Mode: C++; tab-width: 8; indent-tabs-mode: nil; c-basic-offset: 2 -*- */
/* vim: set ts=8 sts=2 et sw=2 tw=80: */
/* This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at https://mozilla.org/MPL/2.0/. */

#ifndef mozilla_DllBlocklistWin_h
#define mozilla_DllBlocklistWin_h

#include <windows.h>

<<<<<<< HEAD
#include "LauncherResult.h"

||||||| merged common ancestors
=======
#include "mozilla/LauncherResult.h"

>>>>>>> upstream-releases
namespace mozilla {

<<<<<<< HEAD
LauncherVoidResult InitializeDllBlocklistOOP(HANDLE aChildProcess);
||||||| merged common ancestors
bool InitializeDllBlocklistOOP(HANDLE aChildProcess);
=======
LauncherVoidResult InitializeDllBlocklistOOP(const wchar_t* aFullImagePath,
                                             HANDLE aChildProcess);
>>>>>>> upstream-releases

}  // namespace mozilla

#endif  // mozilla_DllBlocklistWin_h
