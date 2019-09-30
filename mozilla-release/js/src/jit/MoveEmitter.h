/* -*- Mode: C++; tab-width: 8; indent-tabs-mode: nil; c-basic-offset: 2 -*-
 * vim: set ts=8 sts=2 et sw=2 tw=80:
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/. */

#ifndef jit_MoveEmitter_h
#define jit_MoveEmitter_h

#if defined(JS_CODEGEN_X86) || defined(JS_CODEGEN_X64)
<<<<<<< HEAD
#include "jit/x86-shared/MoveEmitter-x86-shared.h"
||||||| merged common ancestors
# include "jit/x86-shared/MoveEmitter-x86-shared.h"
=======
#  include "jit/x86-shared/MoveEmitter-x86-shared.h"
>>>>>>> upstream-releases
#elif defined(JS_CODEGEN_ARM)
<<<<<<< HEAD
#include "jit/arm/MoveEmitter-arm.h"
||||||| merged common ancestors
# include "jit/arm/MoveEmitter-arm.h"
=======
#  include "jit/arm/MoveEmitter-arm.h"
>>>>>>> upstream-releases
#elif defined(JS_CODEGEN_ARM64)
<<<<<<< HEAD
#include "jit/arm64/MoveEmitter-arm64.h"
||||||| merged common ancestors
# include "jit/arm64/MoveEmitter-arm64.h"
=======
#  include "jit/arm64/MoveEmitter-arm64.h"
>>>>>>> upstream-releases
#elif defined(JS_CODEGEN_MIPS32)
<<<<<<< HEAD
#include "jit/mips32/MoveEmitter-mips32.h"
||||||| merged common ancestors
# include "jit/mips32/MoveEmitter-mips32.h"
=======
#  include "jit/mips32/MoveEmitter-mips32.h"
>>>>>>> upstream-releases
#elif defined(JS_CODEGEN_MIPS64)
<<<<<<< HEAD
#include "jit/mips64/MoveEmitter-mips64.h"
||||||| merged common ancestors
# include "jit/mips64/MoveEmitter-mips64.h"
=======
#  include "jit/mips64/MoveEmitter-mips64.h"
>>>>>>> upstream-releases
#elif defined(JS_CODEGEN_NONE)
<<<<<<< HEAD
#include "jit/none/MoveEmitter-none.h"
||||||| merged common ancestors
# include "jit/none/MoveEmitter-none.h"
=======
#  include "jit/none/MoveEmitter-none.h"
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

#endif /* jit_MoveEmitter_h */
