/* -*- Mode: C++; tab-width: 8; indent-tabs-mode: nil; c-basic-offset: 2 -*- */
/* vim: set ts=8 sts=2 et sw=2 tw=80: */
/* This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/. */

#include "mozilla/gfx/Point.h"
#include "mozilla/layers/PLayerTransaction.h"
#include "mozilla/layers/ShadowLayers.h"
#include "mozilla/layers/LayerManagerComposite.h"
#include "mozilla/layers/CompositorTypes.h"

#include "gfx2DGlue.h"
#include "gfxPlatform.h"

using namespace mozilla::gl;

namespace mozilla {
namespace layers {

<<<<<<< HEAD
/*static*/ void ShadowLayerForwarder::PlatformSyncBeforeUpdate() {}
||||||| merged common ancestors
/*static*/ void
ShadowLayerForwarder::PlatformSyncBeforeUpdate()
{
}
=======
/*static*/
void ShadowLayerForwarder::PlatformSyncBeforeUpdate() {}
>>>>>>> upstream-releases

<<<<<<< HEAD
/*static*/ void LayerManagerComposite::PlatformSyncBeforeReplyUpdate() {}
||||||| merged common ancestors
/*static*/ void
LayerManagerComposite::PlatformSyncBeforeReplyUpdate()
{
}
=======
/*static*/
void LayerManagerComposite::PlatformSyncBeforeReplyUpdate() {}
>>>>>>> upstream-releases

<<<<<<< HEAD
/*static*/ bool LayerManagerComposite::SupportsDirectTexturing() {
  return false;
}
||||||| merged common ancestors
/*static*/ bool
LayerManagerComposite::SupportsDirectTexturing()
{
  return false;
}
=======
/*static*/
bool LayerManagerComposite::SupportsDirectTexturing() { return false; }
>>>>>>> upstream-releases

}  // namespace layers
}  // namespace mozilla
