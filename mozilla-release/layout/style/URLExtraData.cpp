/* -*- Mode: C++; tab-width: 8; indent-tabs-mode: nil; c-basic-offset: 2 -*- */
/* vim: set ts=8 sts=2 et sw=2 tw=80: */
/* This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/. */

/* thread-safe container of information for resolving url values */

#include "mozilla/URLExtraData.h"

#include "mozilla/NullPrincipalURI.h"
#include "nsProxyRelease.h"
#include "mozilla/net/ReferrerPolicy.h"

namespace mozilla {

StaticRefPtr<URLExtraData> URLExtraData::sDummy;

<<<<<<< HEAD
/* static */ void URLExtraData::InitDummy() {
||||||| merged common ancestors
/* static */ void
URLExtraData::InitDummy()
{
=======
/* static */
void URLExtraData::InitDummy() {
>>>>>>> upstream-releases
  RefPtr<nsIURI> baseURI = NullPrincipalURI::Create();
  RefPtr<nsIURI> referrer = baseURI;
  sDummy = new URLExtraData(baseURI.forget(), referrer.forget(),
                            NullPrincipal::CreateWithoutOriginAttributes(),
                            net::RP_Unset);
}

<<<<<<< HEAD
/* static */ void URLExtraData::ReleaseDummy() { sDummy = nullptr; }
||||||| merged common ancestors
/* static */ void
URLExtraData::ReleaseDummy()
{
  sDummy = nullptr;
}
=======
/* static */
void URLExtraData::ReleaseDummy() { sDummy = nullptr; }
>>>>>>> upstream-releases

URLExtraData::~URLExtraData() {
  if (!NS_IsMainThread()) {
    NS_ReleaseOnMainThreadSystemGroup("URLExtraData::mPrincipal",
                                      mPrincipal.forget());
  }
}

<<<<<<< HEAD
}  // namespace mozilla
||||||| merged common ancestors
} // namespace mozilla
=======
StaticRefPtr<URLExtraData>
    URLExtraData::sShared[size_t(UserAgentStyleSheetID::Count)];

}  // namespace mozilla
>>>>>>> upstream-releases
