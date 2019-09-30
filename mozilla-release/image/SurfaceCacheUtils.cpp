/* -*- Mode: C++; tab-width: 2; indent-tabs-mode: nil; c-basic-offset: 2 -*- */
/* This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/. */

#include "SurfaceCacheUtils.h"

#include "SurfaceCache.h"

namespace mozilla {
namespace image {

<<<<<<< HEAD
/* static */ void SurfaceCacheUtils::DiscardAll() {
  SurfaceCache::DiscardAll();
}
||||||| merged common ancestors
/* static */ void
SurfaceCacheUtils::DiscardAll()
{
  SurfaceCache::DiscardAll();
}
=======
/* static */
void SurfaceCacheUtils::DiscardAll() { SurfaceCache::DiscardAll(); }
>>>>>>> upstream-releases

}  // namespace image
}  // namespace mozilla
