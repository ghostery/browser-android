/* -*- Mode: C++; tab-width: 8; indent-tabs-mode: nil; c-basic-offset: 2 -*-
 * vim: set ts=8 sts=2 et sw=2 tw=80:
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/. */

#ifndef jit_SharedICRegisters_h
#define jit_SharedICRegisters_h

#if defined(JS_CODEGEN_X86)
<<<<<<< HEAD
#include "jit/x86/SharedICRegisters-x86.h"
||||||| merged common ancestors
# include "jit/x86/SharedICRegisters-x86.h"
=======
#  include "jit/x86/SharedICRegisters-x86.h"
>>>>>>> upstream-releases
#elif defined(JS_CODEGEN_X64)
<<<<<<< HEAD
#include "jit/x64/SharedICRegisters-x64.h"
||||||| merged common ancestors
# include "jit/x64/SharedICRegisters-x64.h"
=======
#  include "jit/x64/SharedICRegisters-x64.h"
>>>>>>> upstream-releases
#elif defined(JS_CODEGEN_ARM)
<<<<<<< HEAD
#include "jit/arm/SharedICRegisters-arm.h"
||||||| merged common ancestors
# include "jit/arm/SharedICRegisters-arm.h"
=======
#  include "jit/arm/SharedICRegisters-arm.h"
>>>>>>> upstream-releases
#elif defined(JS_CODEGEN_ARM64)
<<<<<<< HEAD
#include "jit/arm64/SharedICRegisters-arm64.h"
||||||| merged common ancestors
# include "jit/arm64/SharedICRegisters-arm64.h"
=======
#  include "jit/arm64/SharedICRegisters-arm64.h"
>>>>>>> upstream-releases
#elif defined(JS_CODEGEN_MIPS32)
<<<<<<< HEAD
#include "jit/mips32/SharedICRegisters-mips32.h"
||||||| merged common ancestors
# include "jit/mips32/SharedICRegisters-mips32.h"
=======
#  include "jit/mips32/SharedICRegisters-mips32.h"
>>>>>>> upstream-releases
#elif defined(JS_CODEGEN_MIPS64)
<<<<<<< HEAD
#include "jit/mips64/SharedICRegisters-mips64.h"
||||||| merged common ancestors
# include "jit/mips64/SharedICRegisters-mips64.h"
=======
#  include "jit/mips64/SharedICRegisters-mips64.h"
>>>>>>> upstream-releases
#elif defined(JS_CODEGEN_NONE)
<<<<<<< HEAD
#include "jit/none/SharedICRegisters-none.h"
||||||| merged common ancestors
# include "jit/none/SharedICRegisters-none.h"
=======
#  include "jit/none/SharedICRegisters-none.h"
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

#endif /* jit_SharedICRegisters_h */
