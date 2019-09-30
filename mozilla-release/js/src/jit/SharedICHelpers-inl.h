/* -*- Mode: C++; tab-width: 8; indent-tabs-mode: nil; c-basic-offset: 2 -*-
 * vim: set ts=8 sts=2 et sw=2 tw=80:
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/. */

#ifndef jit_SharedICHelpers_inl_h
#define jit_SharedICHelpers_inl_h

#if defined(JS_CODEGEN_X86)
<<<<<<< HEAD
#include "jit/x86/SharedICHelpers-x86-inl.h"
||||||| merged common ancestors
# include "jit/x86/SharedICHelpers-x86-inl.h"
=======
#  include "jit/x86/SharedICHelpers-x86-inl.h"
>>>>>>> upstream-releases
#elif defined(JS_CODEGEN_X64)
<<<<<<< HEAD
#include "jit/x64/SharedICHelpers-x64-inl.h"
||||||| merged common ancestors
# include "jit/x64/SharedICHelpers-x64-inl.h"
=======
#  include "jit/x64/SharedICHelpers-x64-inl.h"
>>>>>>> upstream-releases
#elif defined(JS_CODEGEN_ARM)
<<<<<<< HEAD
#include "jit/arm/SharedICHelpers-arm-inl.h"
||||||| merged common ancestors
# include "jit/arm/SharedICHelpers-arm-inl.h"
=======
#  include "jit/arm/SharedICHelpers-arm-inl.h"
>>>>>>> upstream-releases
#elif defined(JS_CODEGEN_ARM64)
<<<<<<< HEAD
#include "jit/arm64/SharedICHelpers-arm64-inl.h"
||||||| merged common ancestors
# include "jit/arm64/SharedICHelpers-arm64-inl.h"
=======
#  include "jit/arm64/SharedICHelpers-arm64-inl.h"
>>>>>>> upstream-releases
#elif defined(JS_CODEGEN_MIPS32) || defined(JS_CODEGEN_MIPS64)
#  include "jit/mips-shared/SharedICHelpers-mips-shared-inl.h"
#elif defined(JS_CODEGEN_NONE)
<<<<<<< HEAD
#include "jit/none/SharedICHelpers-none-inl.h"
||||||| merged common ancestors
# include "jit/none/SharedICHelpers-none-inl.h"
=======
#  include "jit/none/SharedICHelpers-none-inl.h"
>>>>>>> upstream-releases
#else
<<<<<<< HEAD
#error "Unknown architecture!"
||||||| merged common ancestors
# error "Unknown architecture!"
=======
#  error "Unknown architecture!"
>>>>>>> upstream-releases
#endif

namespace js {
namespace jit {}  // namespace jit
}  // namespace js

#endif /* jit_SharedICHelpers_inl_h */
