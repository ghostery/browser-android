/* -*- Mode: C++; tab-width: 8; indent-tabs-mode: nil; c-basic-offset: 2 -*- */
/* vim: set ts=8 sts=2 et sw=2 tw=80: */
/* This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at http://mozilla.org/MPL/2.0/. */

#include "PermissionUtils.h"

namespace mozilla {
namespace dom {

<<<<<<< HEAD
const char* kPermissionTypes[] = {"geo", "desktop-notification",
                                  // Alias `push` to `desktop-notification`.
                                  "desktop-notification", "persistent-storage"};
||||||| merged common ancestors
const char* kPermissionTypes[] = {
  "geo",
  "desktop-notification",
  // Alias `push` to `desktop-notification`.
  "desktop-notification",
  "persistent-storage"
};
=======
static const nsLiteralCString kPermissionTypes[] = {
    // clang-format off
    NS_LITERAL_CSTRING("geo"),
    NS_LITERAL_CSTRING("desktop-notification"),
    // Alias `push` to `desktop-notification`.
    NS_LITERAL_CSTRING("desktop-notification"),
    NS_LITERAL_CSTRING("persistent-storage")
    // clang-format on
};
>>>>>>> upstream-releases

// `-1` for the last null entry.
const size_t kPermissionNameCount =
    MOZ_ARRAY_LENGTH(PermissionNameValues::strings) - 1;

static_assert(MOZ_ARRAY_LENGTH(kPermissionTypes) == kPermissionNameCount,
              "kPermissionTypes and PermissionName count should match");

<<<<<<< HEAD
const char* PermissionNameToType(PermissionName aName) {
||||||| merged common ancestors
const char*
PermissionNameToType(PermissionName aName)
{
=======
const nsLiteralCString& PermissionNameToType(PermissionName aName) {
>>>>>>> upstream-releases
  MOZ_ASSERT((size_t)aName < ArrayLength(kPermissionTypes));
  return kPermissionTypes[static_cast<size_t>(aName)];
}

<<<<<<< HEAD
Maybe<PermissionName> TypeToPermissionName(const char* aType) {
||||||| merged common ancestors
Maybe<PermissionName>
TypeToPermissionName(const char* aType)
{
=======
Maybe<PermissionName> TypeToPermissionName(const nsACString& aType) {
>>>>>>> upstream-releases
  for (size_t i = 0; i < ArrayLength(kPermissionTypes); ++i) {
    if (kPermissionTypes[i].Equals(aType)) {
      return Some(static_cast<PermissionName>(i));
    }
  }

  return Nothing();
}

PermissionState ActionToPermissionState(uint32_t aAction) {
  switch (aAction) {
    case nsIPermissionManager::ALLOW_ACTION:
      return PermissionState::Granted;

    case nsIPermissionManager::DENY_ACTION:
      return PermissionState::Denied;

    default:
    case nsIPermissionManager::PROMPT_ACTION:
      return PermissionState::Prompt;
  }
}

}  // namespace dom
}  // namespace mozilla
