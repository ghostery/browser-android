/* -*- Mode: C++; tab-width: 8; indent-tabs-mode: nil; c-basic-offset: 2 -*- */
/* vim: set ts=8 sts=2 et sw=2 tw=80: */
/* This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/. */

/*
 * Provides a common interface to the ASan (AddressSanitizer) and Valgrind
 * functions used to mark memory in certain ways. In detail, the following
 * three macros are provided:
 *
 *   MOZ_MAKE_MEM_NOACCESS  - Mark memory as unsafe to access (e.g. freed)
 *   MOZ_MAKE_MEM_UNDEFINED - Mark memory as accessible, with content undefined
 *   MOZ_MAKE_MEM_DEFINED - Mark memory as accessible, with content defined
 *
 * With Valgrind in use, these directly map to the three respective Valgrind
 * macros. With ASan in use, the NOACCESS macro maps to poisoning the memory,
 * while the UNDEFINED/DEFINED macros unpoison memory.
 *
 * With no memory checker available, all macros expand to the empty statement.
 */

#ifndef mozilla_MemoryChecking_h
#define mozilla_MemoryChecking_h

#if defined(MOZ_VALGRIND)
#  include "valgrind/memcheck.h"
#endif

#if defined(MOZ_ASAN) || defined(MOZ_VALGRIND)
#  define MOZ_HAVE_MEM_CHECKS 1
#endif

#if defined(MOZ_ASAN)
#  include <stddef.h>

#  include "mozilla/Attributes.h"
#  include "mozilla/Types.h"

#  ifdef _MSC_VER
// In clang-cl based ASAN, we link against the memory poisoning functions
// statically.
#    define MOZ_ASAN_VISIBILITY
#  else
#    define MOZ_ASAN_VISIBILITY MOZ_EXPORT
#  endif

extern "C" {
/* These definitions are usually provided through the
 * sanitizer/asan_interface.h header installed by ASan.
 */
<<<<<<< HEAD
void MOZ_ASAN_VISIBILITY __asan_poison_memory_region(void const volatile *addr,
                                                     size_t size);
void MOZ_ASAN_VISIBILITY
__asan_unpoison_memory_region(void const volatile *addr, size_t size);
||||||| merged common ancestors
void MOZ_ASAN_VISIBILITY
__asan_poison_memory_region(void const volatile *addr, size_t size);
void MOZ_ASAN_VISIBILITY
__asan_unpoison_memory_region(void const volatile *addr, size_t size);
=======
void MOZ_ASAN_VISIBILITY __asan_poison_memory_region(void const volatile* addr,
                                                     size_t size);
void MOZ_ASAN_VISIBILITY
__asan_unpoison_memory_region(void const volatile* addr, size_t size);
>>>>>>> upstream-releases

#  define MOZ_MAKE_MEM_NOACCESS(addr, size) \
    __asan_poison_memory_region((addr), (size))

#  define MOZ_MAKE_MEM_UNDEFINED(addr, size) \
    __asan_unpoison_memory_region((addr), (size))

#  define MOZ_MAKE_MEM_DEFINED(addr, size) \
    __asan_unpoison_memory_region((addr), (size))

/*
 * These definitions are usually provided through the
 * sanitizer/lsan_interface.h header installed by LSan.
 */
<<<<<<< HEAD
void MOZ_EXPORT __lsan_ignore_object(const void *p);
||||||| merged common ancestors
void MOZ_EXPORT
__lsan_ignore_object(const void *p);

=======
void MOZ_EXPORT __lsan_ignore_object(const void* p);
>>>>>>> upstream-releases
}
#elif defined(MOZ_MSAN)
#  include <stddef.h>

#  include "mozilla/Types.h"

extern "C" {
/* These definitions are usually provided through the
 * sanitizer/msan_interface.h header installed by MSan.
 */
<<<<<<< HEAD
void MOZ_EXPORT __msan_poison(void const volatile *addr, size_t size);
void MOZ_EXPORT __msan_unpoison(void const volatile *addr, size_t size);
||||||| merged common ancestors
void MOZ_EXPORT
__msan_poison(void const volatile *addr, size_t size);
void MOZ_EXPORT
__msan_unpoison(void const volatile *addr, size_t size);
=======
void MOZ_EXPORT __msan_poison(void const volatile* addr, size_t size);
void MOZ_EXPORT __msan_unpoison(void const volatile* addr, size_t size);
>>>>>>> upstream-releases

<<<<<<< HEAD
#define MOZ_MAKE_MEM_NOACCESS(addr, size) __msan_poison((addr), (size))
||||||| merged common ancestors
#define MOZ_MAKE_MEM_NOACCESS(addr, size) \
  __msan_poison((addr), (size))
=======
#  define MOZ_MAKE_MEM_NOACCESS(addr, size) __msan_poison((addr), (size))
>>>>>>> upstream-releases

<<<<<<< HEAD
#define MOZ_MAKE_MEM_UNDEFINED(addr, size) __msan_poison((addr), (size))
||||||| merged common ancestors
#define MOZ_MAKE_MEM_UNDEFINED(addr, size) \
  __msan_poison((addr), (size))
=======
#  define MOZ_MAKE_MEM_UNDEFINED(addr, size) __msan_poison((addr), (size))
>>>>>>> upstream-releases

<<<<<<< HEAD
#define MOZ_MAKE_MEM_DEFINED(addr, size) __msan_unpoison((addr), (size))
||||||| merged common ancestors
#define MOZ_MAKE_MEM_DEFINED(addr, size) \
  __msan_unpoison((addr), (size))
=======
#  define MOZ_MAKE_MEM_DEFINED(addr, size) __msan_unpoison((addr), (size))
>>>>>>> upstream-releases
}
#elif defined(MOZ_VALGRIND)
#  define MOZ_MAKE_MEM_NOACCESS(addr, size) \
    VALGRIND_MAKE_MEM_NOACCESS((addr), (size))

#  define MOZ_MAKE_MEM_UNDEFINED(addr, size) \
    VALGRIND_MAKE_MEM_UNDEFINED((addr), (size))

#  define MOZ_MAKE_MEM_DEFINED(addr, size) \
    VALGRIND_MAKE_MEM_DEFINED((addr), (size))
#else

<<<<<<< HEAD
#define MOZ_MAKE_MEM_NOACCESS(addr, size) \
  do {                                    \
  } while (0)
#define MOZ_MAKE_MEM_UNDEFINED(addr, size) \
  do {                                     \
  } while (0)
#define MOZ_MAKE_MEM_DEFINED(addr, size) \
  do {                                   \
  } while (0)
||||||| merged common ancestors
#define MOZ_MAKE_MEM_NOACCESS(addr, size) do {} while (0)
#define MOZ_MAKE_MEM_UNDEFINED(addr, size) do {} while (0)
#define MOZ_MAKE_MEM_DEFINED(addr, size) do {} while (0)
=======
#  define MOZ_MAKE_MEM_NOACCESS(addr, size) \
    do {                                    \
    } while (0)
#  define MOZ_MAKE_MEM_UNDEFINED(addr, size) \
    do {                                     \
    } while (0)
#  define MOZ_MAKE_MEM_DEFINED(addr, size) \
    do {                                   \
    } while (0)
>>>>>>> upstream-releases

#endif

/*
 * MOZ_LSAN_INTENTIONAL_LEAK(X) is a macro to tell LeakSanitizer that X
 * points to a value that will intentionally never be deallocated during
 * the execution of the process.
 *
 * Additional uses of this macro should be reviewed by people
 * conversant in leak-checking and/or MFBT peers.
 */
#if defined(MOZ_ASAN)
#define MOZ_LSAN_INTENTIONALLY_LEAK_OBJECT(X) __lsan_ignore_object(X)
#else
<<<<<<< HEAD
#define MOZ_LSAN_INTENTIONALLY_LEAK_OBJECT(X) /* nothing */
#endif                                        // defined(MOZ_ASAN)
||||||| merged common ancestors
#  define MOZ_LSAN_INTENTIONALLY_LEAK_OBJECT(X) /* nothing */
#endif // defined(MOZ_ASAN)

=======
#  define MOZ_LSAN_INTENTIONALLY_LEAK_OBJECT(X) /* nothing */
#endif                                          // defined(MOZ_ASAN)
>>>>>>> upstream-releases

#endif /* mozilla_MemoryChecking_h */
