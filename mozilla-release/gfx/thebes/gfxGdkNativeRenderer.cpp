/* -*- Mode: C++; tab-width: 20; indent-tabs-mode: nil; c-basic-offset: 2 -*-
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/. */

#include "gfxGdkNativeRenderer.h"
#include "gfxContext.h"
#include "gfxPlatformGtk.h"

#ifdef MOZ_X11
<<<<<<< HEAD
#include <gdk/gdkx.h>
#include "cairo-xlib.h"
#include "gfxXlibSurface.h"

||||||| merged common ancestors
#include <gdk/gdkx.h>
#include "cairo-xlib.h"
#include "gfxXlibSurface.h"


=======
#  include <gdk/gdkx.h>
#  include "cairo-xlib.h"
#  include "gfxXlibSurface.h"

>>>>>>> upstream-releases
#endif
