/* -*- Mode: C++; tab-width: 8; indent-tabs-mode: nil; c-basic-offset: 2 -*- */
/* vim: set ts=8 sts=2 et sw=2 tw=80: */
/* This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/. */

#include "sandboxTarget.h"

#include "sandbox/win/src/sandbox.h"

namespace mozilla {

// We need to define this function out of line so that clang-cl doesn't inline
// it.
<<<<<<< HEAD
/* static */ SandboxTarget* SandboxTarget::Instance() {
||||||| merged common ancestors
/* static */ SandboxTarget*
SandboxTarget::Instance()
{
=======
/* static */
SandboxTarget* SandboxTarget::Instance() {
>>>>>>> upstream-releases
  static SandboxTarget sb;
  return &sb;
}

void SandboxTarget::StartSandbox() {
  if (mTargetServices) {
    mTargetServices->LowerToken();
    NotifyStartObservers();
  }
}

<<<<<<< HEAD
bool SandboxTarget::BrokerDuplicateHandle(HANDLE aSourceHandle,
                                          DWORD aTargetProcessId,
                                          HANDLE* aTargetHandle,
                                          DWORD aDesiredAccess,
                                          DWORD aOptions) {
||||||| merged common ancestors
bool
SandboxTarget::BrokerDuplicateHandle(HANDLE aSourceHandle,
                                     DWORD aTargetProcessId,
                                     HANDLE* aTargetHandle,
                                     DWORD aDesiredAccess,
                                     DWORD aOptions)
{
=======
void SandboxTarget::NotifyStartObservers() {
  for (auto&& obs : mStartObservers) {
    if (!obs) {
      continue;
    }

    obs();
  }

  mStartObservers.clear();
}

bool SandboxTarget::BrokerDuplicateHandle(HANDLE aSourceHandle,
                                          DWORD aTargetProcessId,
                                          HANDLE* aTargetHandle,
                                          DWORD aDesiredAccess,
                                          DWORD aOptions) {
>>>>>>> upstream-releases
  if (!mTargetServices) {
    return false;
  }

  sandbox::ResultCode result = mTargetServices->DuplicateHandle(
      aSourceHandle, aTargetProcessId, aTargetHandle, aDesiredAccess, aOptions);
  return (sandbox::SBOX_ALL_OK == result);
}

}  // namespace mozilla
