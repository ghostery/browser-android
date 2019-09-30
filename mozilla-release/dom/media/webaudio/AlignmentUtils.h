/* -*- Mode: C++; tab-width: 2; indent-tabs-mode: nil; c-basic-offset: 2 -*- */
/* vim:set ts=2 sw=2 sts=2 et cindent: */
/* This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/. */

#ifndef AlignmentUtils_h__
#define AlignmentUtils_h__

#define IS_ALIGNED16(ptr) ((((uintptr_t)ptr + 15) & ~0x0F) == (uintptr_t)ptr)

#ifdef DEBUG
<<<<<<< HEAD
#define ASSERT_ALIGNED16(ptr)   \
  MOZ_ASSERT(IS_ALIGNED16(ptr), \
             #ptr " has to be aligned to a 16 byte boundary");
||||||| merged common ancestors
  #define ASSERT_ALIGNED16(ptr)                                                  \
            MOZ_ASSERT(IS_ALIGNED16(ptr), \
                       #ptr " has to be aligned to a 16 byte boundary");
=======
#  define ASSERT_ALIGNED16(ptr)   \
    MOZ_ASSERT(IS_ALIGNED16(ptr), \
               #ptr " has to be aligned to a 16 byte boundary");
>>>>>>> upstream-releases
#else
<<<<<<< HEAD
#define ASSERT_ALIGNED16(ptr)
||||||| merged common ancestors
  #define ASSERT_ALIGNED16(ptr)
=======
#  define ASSERT_ALIGNED16(ptr)
>>>>>>> upstream-releases
#endif

#ifdef DEBUG
<<<<<<< HEAD
#define ASSERT_MULTIPLE16(v) \
  MOZ_ASSERT(v % 16 == 0, #v " has to be a a multiple of 16");
||||||| merged common ancestors
  #define ASSERT_MULTIPLE16(v) \
            MOZ_ASSERT(v % 16 == 0, #v " has to be a a multiple of 16");
=======
#  define ASSERT_MULTIPLE16(v) \
    MOZ_ASSERT(v % 16 == 0, #v " has to be a a multiple of 16");
>>>>>>> upstream-releases
#else
<<<<<<< HEAD
#define ASSERT_MULTIPLE16(v)
||||||| merged common ancestors
  #define ASSERT_MULTIPLE16(v)
=======
#  define ASSERT_MULTIPLE16(v)
>>>>>>> upstream-releases
#endif

#define ALIGNED16(ptr) (float*)(((uintptr_t)ptr + 15) & ~0x0F);

#endif
