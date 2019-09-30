/* -*- Mode: C++; tab-width: 8; indent-tabs-mode: nil; c-basic-offset: 2 -*- */
/* vim: set ts=8 sts=2 et sw=2 tw=80: */
/* This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/. */

#ifndef mozilla_layers_CompositorController_h
#define mozilla_layers_CompositorController_h

<<<<<<< HEAD
#include "nsISupportsImpl.h"  // for NS_INLINE_DECL_PURE_VIRTUAL_REFCOUNTING
||||||| merged common ancestors
#include "nsISupportsImpl.h" // for NS_INLINE_DECL_PURE_VIRTUAL_REFCOUNTING
=======
#include "nsISupportsImpl.h"  // for NS_INLINE_DECL_PURE_VIRTUAL_REFCOUNTING
#include "mozilla/Maybe.h"
#include "mozilla/webrender/WebRenderTypes.h"
>>>>>>> upstream-releases

namespace mozilla {
namespace layers {

class CompositorController {
 public:
  NS_INLINE_DECL_PURE_VIRTUAL_REFCOUNTING

  /**
   * Ask the compositor to schedule a new composite. If WebRender is enabled,
   * and the provided render root set is non-empty, then only those render roots
   * will be scheduled for a recomposite. Otherwise, all render roots will be
   * scheduled.
   */
  virtual void ScheduleRenderOnCompositorThread(
      const wr::RenderRootSet& aRenderRoots) = 0;
  virtual void ScheduleHideAllPluginWindows() = 0;
  virtual void ScheduleShowAllPluginWindows() = 0;

<<<<<<< HEAD
 protected:
  virtual ~CompositorController() {}
||||||| merged common ancestors
protected:
  virtual ~CompositorController() {}
=======
 protected:
  virtual ~CompositorController() = default;
>>>>>>> upstream-releases
};

}  // namespace layers
}  // namespace mozilla

#endif  // mozilla_layers_CompositorController_h
