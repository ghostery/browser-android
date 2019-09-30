/* -*- Mode: C++; tab-width: 8; indent-tabs-mode: nil; c-basic-offset: 2 -*-
 * vim: set ts=8 sts=2 et sw=2 tw=80:
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/. */

#include "gc/Memory.h"

#include "mozilla/Atomics.h"
#include "mozilla/MathAlgorithms.h"
#include "mozilla/RandomNum.h"
#include "mozilla/TaggedAnonymousMemory.h"

#include "js/HeapAPI.h"
#include "vm/Runtime.h"

<<<<<<< HEAD
#if defined(XP_WIN)

#include "mozilla/Sprintf.h"
#include "util/Windows.h"
#include <psapi.h>

#elif defined(SOLARIS)
||||||| merged common ancestors
#if defined(XP_WIN)

# include "mozilla/Sprintf.h"
# include "util/Windows.h"
# include <psapi.h>

#elif defined(SOLARIS)
=======
#ifdef XP_WIN
>>>>>>> upstream-releases

<<<<<<< HEAD
#include <sys/mman.h>
#include <unistd.h>
||||||| merged common ancestors
# include <sys/mman.h>
# include <unistd.h>
=======
#  include "util/Windows.h"
#  include <psapi.h>
>>>>>>> upstream-releases

#else

<<<<<<< HEAD
#include <algorithm>
#include <errno.h>
#include <sys/mman.h>
#include <sys/resource.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>
||||||| merged common ancestors
# include <algorithm>
# include <errno.h>
# include <sys/mman.h>
# include <sys/resource.h>
# include <sys/stat.h>
# include <sys/types.h>
# include <unistd.h>
=======
#  include <algorithm>
#  include <errno.h>
#  include <sys/mman.h>
#  include <sys/resource.h>
#  include <sys/stat.h>
#  include <sys/types.h>
#  include <unistd.h>
>>>>>>> upstream-releases

#endif

namespace js {
namespace gc {

/*
 * System allocation functions generally require the allocation size
 * to be an integer multiple of the page size of the running process.
 */
static size_t pageSize = 0;

/* The OS allocation granularity may not match the page size. */
static size_t allocGranularity = 0;

<<<<<<< HEAD
#if defined(XP_UNIX)
// The addresses handed out by mmap may grow up or down.
static mozilla::Atomic<int, mozilla::Relaxed,
                       mozilla::recordreplay::Behavior::DontPreserve>
    growthDirection(0);
||||||| merged common ancestors
#if defined(XP_UNIX)
// The addresses handed out by mmap may grow up or down.
static mozilla::Atomic<int,
                       mozilla::Relaxed,
                       mozilla::recordreplay::Behavior::DontPreserve> growthDirection(0);
=======
/* The number of bits used by addresses on this platform. */
static size_t numAddressBits = 0;

/*
 * System allocation functions may hand out regions of memory in increasing or
 * decreasing order. This ordering is used as a hint during chunk alignment to
 * reduce the number of system calls. On systems with 48-bit addresses, our
 * workarounds to obtain 47-bit pointers cause addresses to be handed out in
 * increasing order.
 *
 * We do not use the growth direction on Windows, as constraints on VirtualAlloc
 * would make its application failure prone and complex. Tests indicate that
 * VirtualAlloc always hands out regions of memory in increasing order.
 */
#if defined(XP_DARWIN)
static mozilla::Atomic<int, mozilla::Relaxed,
                       mozilla::recordreplay::Behavior::DontPreserve>
    growthDirection(1);
#elif defined(XP_UNIX)
static mozilla::Atomic<int, mozilla::Relaxed,
                       mozilla::recordreplay::Behavior::DontPreserve>
    growthDirection(0);
>>>>>>> upstream-releases
#endif

/*
 * Data from OOM crashes shows there may be up to 24 chunk-sized but unusable
 * chunks available in low memory situations. These chunks may all need to be
 * used up before we gain access to remaining *alignable* chunk-sized regions,
 * so we use a generous limit of 32 unusable chunks to ensure we reach them.
 */
static const int MaxLastDitchAttempts = 32;

<<<<<<< HEAD
static void GetNewChunk(void** aAddress, void** aRetainedAddr, size_t size,
                        size_t alignment);
static void* MapAlignedPagesSlow(size_t size, size_t alignment);
static void* MapAlignedPagesLastDitch(size_t size, size_t alignment);

size_t SystemPageSize() { return pageSize; }

static bool DecommitEnabled() { return pageSize == ArenaSize; }

||||||| merged common ancestors
static void GetNewChunk(void** aAddress, void** aRetainedAddr, size_t size, size_t alignment);
static void* MapAlignedPagesSlow(size_t size, size_t alignment);
static void* MapAlignedPagesLastDitch(size_t size, size_t alignment);

size_t
SystemPageSize()
{
    return pageSize;
}

static bool
DecommitEnabled()
{
    return pageSize == ArenaSize;
}

=======
#ifdef JS_64BIT
>>>>>>> upstream-releases
/*
 * On some 64-bit platforms we can use a random, scattershot allocator that
 * tries addresses from the available range at random. If the address range
 * is large enough this will have a high chance of success and additionally
 * makes the memory layout of our process less predictable.
 *
 * However, not all 64-bit platforms have a very large address range. For
 * example, AArch64 on Linux defaults to using 39-bit addresses to limit the
 * number of translation tables used. On such configurations the scattershot
 * approach to allocation creates a conflict with our desire to reserve large
 * regions of memory for applications like WebAssembly: Small allocations may
 * inadvertently block off all available 4-6GiB regions, and conversely
 * reserving such regions may lower the success rate for smaller allocations to
 * unacceptable levels.
 *
 * So we make a compromise: Instead of using the scattershot on all 64-bit
 * platforms, we only use it on platforms that meet a minimum requirement for
 * the available address range. In addition we split the address range,
 * reserving the upper half for huge allocations and the lower half for smaller
 * allocations. We use a limit of 43 bits so that at least 42 bits are available
 * for huge allocations - this matches the 8TiB per process address space limit
 * that we're already subject to on Windows.
 */
<<<<<<< HEAD
static inline size_t OffsetFromAligned(void* p, size_t alignment) {
  return uintptr_t(p) % alignment;
}
||||||| merged common ancestors
static inline size_t
OffsetFromAligned(void* p, size_t alignment)
{
    return uintptr_t(p) % alignment;
}
=======
static const size_t MinAddressBitsForRandomAlloc = 43;
>>>>>>> upstream-releases

<<<<<<< HEAD
void* TestMapAlignedPagesLastDitch(size_t size, size_t alignment) {
  return MapAlignedPagesLastDitch(size, alignment);
}
||||||| merged common ancestors
void*
TestMapAlignedPagesLastDitch(size_t size, size_t alignment)
{
    return MapAlignedPagesLastDitch(size, alignment);
}
=======
/* The lower limit for huge allocations. This is fairly arbitrary. */
static const size_t HugeAllocationSize = 1024 * 1024 * 1024;
>>>>>>> upstream-releases

<<<<<<< HEAD
#if defined(XP_WIN)

void InitMemorySubsystem() {
  if (pageSize == 0) {
    SYSTEM_INFO sysinfo;
    GetSystemInfo(&sysinfo);
    pageSize = sysinfo.dwPageSize;
    allocGranularity = sysinfo.dwAllocationGranularity;
  }
}
||||||| merged common ancestors

#if defined(XP_WIN)

void
InitMemorySubsystem()
{
    if (pageSize == 0) {
        SYSTEM_INFO sysinfo;
        GetSystemInfo(&sysinfo);
        pageSize = sysinfo.dwPageSize;
        allocGranularity = sysinfo.dwAllocationGranularity;
    }
}
=======
/* The minimum and maximum valid addresses that can be allocated into. */
static size_t minValidAddress = 0;
static size_t maxValidAddress = 0;

/* The upper limit for smaller allocations and the lower limit for huge ones. */
static size_t hugeSplit = 0;
#endif
>>>>>>> upstream-releases

<<<<<<< HEAD
#if WINAPI_FAMILY_PARTITION(WINAPI_PARTITION_DESKTOP)
||||||| merged common ancestors
#  if WINAPI_FAMILY_PARTITION(WINAPI_PARTITION_DESKTOP)
=======
size_t SystemPageSize() { return pageSize; }
>>>>>>> upstream-releases

<<<<<<< HEAD
static inline void* MapMemoryAt(void* desired, size_t length, int flags,
                                int prot = PAGE_READWRITE) {
  return VirtualAlloc(desired, length, flags, prot);
}
||||||| merged common ancestors
static inline void*
MapMemoryAt(void* desired, size_t length, int flags, int prot = PAGE_READWRITE)
{
    return VirtualAlloc(desired, length, flags, prot);
}
=======
size_t SystemAddressBits() { return numAddressBits; }
>>>>>>> upstream-releases

<<<<<<< HEAD
static inline void* MapMemory(size_t length, int flags,
                              int prot = PAGE_READWRITE) {
  return VirtualAlloc(nullptr, length, flags, prot);
||||||| merged common ancestors
static inline void*
MapMemory(size_t length, int flags, int prot = PAGE_READWRITE)
{
    return VirtualAlloc(nullptr, length, flags, prot);
=======
bool UsingScattershotAllocator() {
#ifdef JS_64BIT
  return numAddressBits >= MinAddressBitsForRandomAlloc;
#else
  return false;
#endif
>>>>>>> upstream-releases
}

<<<<<<< HEAD
void* MapAlignedPages(size_t size, size_t alignment) {
  MOZ_ASSERT(size >= alignment);
  MOZ_ASSERT(size >= allocGranularity);
  MOZ_ASSERT(size % alignment == 0);
  MOZ_ASSERT(size % pageSize == 0);
  MOZ_ASSERT_IF(alignment < allocGranularity,
                allocGranularity % alignment == 0);
  MOZ_ASSERT_IF(alignment > allocGranularity,
                alignment % allocGranularity == 0);

  void* p = MapMemory(size, MEM_COMMIT | MEM_RESERVE);

  // Special case: If we want allocation alignment, no further work is needed.
  if (alignment == allocGranularity) {
    return p;
  }

  if (OffsetFromAligned(p, alignment) == 0) {
    return p;
  }

  void* retainedAddr;
  GetNewChunk(&p, &retainedAddr, size, alignment);
  if (retainedAddr) {
    UnmapPages(retainedAddr, size);
  }
  if (p) {
    if (OffsetFromAligned(p, alignment) == 0) {
      return p;
    }
    UnmapPages(p, size);
  }
||||||| merged common ancestors
void*
MapAlignedPages(size_t size, size_t alignment)
{
    MOZ_ASSERT(size >= alignment);
    MOZ_ASSERT(size >= allocGranularity);
    MOZ_ASSERT(size % alignment == 0);
    MOZ_ASSERT(size % pageSize == 0);
    MOZ_ASSERT_IF(alignment < allocGranularity, allocGranularity % alignment == 0);
    MOZ_ASSERT_IF(alignment > allocGranularity, alignment % allocGranularity == 0);

    void* p = MapMemory(size, MEM_COMMIT | MEM_RESERVE);

    /* Special case: If we want allocation alignment, no further work is needed. */
    if (alignment == allocGranularity) {
        return p;
    }

    if (OffsetFromAligned(p, alignment) == 0) {
        return p;
    }

    void* retainedAddr;
    GetNewChunk(&p, &retainedAddr, size, alignment);
    if (retainedAddr) {
        UnmapPages(retainedAddr, size);
    }
    if (p) {
        if (OffsetFromAligned(p, alignment) == 0) {
            return p;
        }
        UnmapPages(p, size);
    }
=======
enum class Commit : bool {
  No = false,
  Yes = true,
};

#ifdef XP_WIN
enum class PageAccess : DWORD {
  None = PAGE_NOACCESS,
  Read = PAGE_READONLY,
  ReadWrite = PAGE_READWRITE,
  Execute = PAGE_EXECUTE,
  ReadExecute = PAGE_EXECUTE_READ,
  ReadWriteExecute = PAGE_EXECUTE_READWRITE,
};
#else
enum class PageAccess : int {
  None = PROT_NONE,
  Read = PROT_READ,
  ReadWrite = PROT_READ | PROT_WRITE,
  Execute = PROT_EXEC,
  ReadExecute = PROT_READ | PROT_EXEC,
  ReadWriteExecute = PROT_READ | PROT_WRITE | PROT_EXEC,
};
#endif
>>>>>>> upstream-releases

<<<<<<< HEAD
  p = MapAlignedPagesSlow(size, alignment);
  if (!p) {
    return MapAlignedPagesLastDitch(size, alignment);
  }
||||||| merged common ancestors
    p = MapAlignedPagesSlow(size, alignment);
    if (!p) {
        return MapAlignedPagesLastDitch(size, alignment);
    }
=======
template <bool AlwaysGetNew = true>
static bool TryToAlignChunk(void** aRegion, void** aRetainedRegion,
                            size_t length, size_t alignment);
>>>>>>> upstream-releases

<<<<<<< HEAD
  MOZ_ASSERT(OffsetFromAligned(p, alignment) == 0);
  return p;
}
||||||| merged common ancestors
    MOZ_ASSERT(OffsetFromAligned(p, alignment) == 0);
    return p;
}
=======
static void* MapAlignedPagesSlow(size_t length, size_t alignment);
static void* MapAlignedPagesLastDitch(size_t length, size_t alignment);
>>>>>>> upstream-releases

<<<<<<< HEAD
static void* MapAlignedPagesSlow(size_t size, size_t alignment) {
  /*
   * Windows requires that there be a 1:1 mapping between VM allocation
   * and deallocation operations.  Therefore, take care here to acquire the
   * final result via one mapping operation.  This means unmapping any
   * preliminary result that is not correctly aligned.
   */
  void* p;
  do {
    /*
     * Over-allocate in order to map a memory region that is definitely
     * large enough, then deallocate and allocate again the correct size,
     * within the over-sized mapping.
     *
     * Since we're going to unmap the whole thing anyway, the first
     * mapping doesn't have to commit pages.
     */
    size_t reserveSize = size + alignment - pageSize;
    p = MapMemory(reserveSize, MEM_RESERVE);
    if (!p) {
      return nullptr;
    }
    void* chunkStart = (void*)AlignBytes(uintptr_t(p), alignment);
    UnmapPages(p, reserveSize);
    p = MapMemoryAt(chunkStart, size, MEM_COMMIT | MEM_RESERVE);

    /* Failure here indicates a race with another thread, so try again. */
  } while (!p);
||||||| merged common ancestors
static void*
MapAlignedPagesSlow(size_t size, size_t alignment)
{
    /*
     * Windows requires that there be a 1:1 mapping between VM allocation
     * and deallocation operations.  Therefore, take care here to acquire the
     * final result via one mapping operation.  This means unmapping any
     * preliminary result that is not correctly aligned.
     */
    void* p;
    do {
        /*
         * Over-allocate in order to map a memory region that is definitely
         * large enough, then deallocate and allocate again the correct size,
         * within the over-sized mapping.
         *
         * Since we're going to unmap the whole thing anyway, the first
         * mapping doesn't have to commit pages.
         */
        size_t reserveSize = size + alignment - pageSize;
        p = MapMemory(reserveSize, MEM_RESERVE);
        if (!p) {
            return nullptr;
        }
        void* chunkStart = (void*)AlignBytes(uintptr_t(p), alignment);
        UnmapPages(p, reserveSize);
        p = MapMemoryAt(chunkStart, size, MEM_COMMIT | MEM_RESERVE);

        /* Failure here indicates a race with another thread, so try again. */
    } while (!p);
=======
#ifdef JS_64BIT
static void* MapAlignedPagesRandom(size_t length, size_t alignment);
#endif
>>>>>>> upstream-releases

<<<<<<< HEAD
  return p;
||||||| merged common ancestors
    return p;
=======
void* TestMapAlignedPagesLastDitch(size_t length, size_t alignment) {
  return MapAlignedPagesLastDitch(length, alignment);
>>>>>>> upstream-releases
}

/*
 * We can only decommit unused pages if the hardcoded Arena
 * size matches the page size for the running process.
 */
<<<<<<< HEAD
static void* MapAlignedPagesLastDitch(size_t size, size_t alignment) {
  void* tempMaps[MaxLastDitchAttempts];
  int attempt = 0;
  void* p = MapMemory(size, MEM_COMMIT | MEM_RESERVE);
  if (OffsetFromAligned(p, alignment) == 0) {
    return p;
  }
  for (; attempt < MaxLastDitchAttempts; ++attempt) {
    GetNewChunk(&p, tempMaps + attempt, size, alignment);
    if (OffsetFromAligned(p, alignment) == 0) {
      if (tempMaps[attempt]) {
        UnmapPages(tempMaps[attempt], size);
      }
      break;
    }
    if (!tempMaps[attempt]) {
      break; /* Bail if GetNewChunk failed. */
    }
  }
  if (OffsetFromAligned(p, alignment)) {
    UnmapPages(p, size);
    p = nullptr;
  }
  while (--attempt >= 0) {
    UnmapPages(tempMaps[attempt], size);
  }
  return p;
}

/*
 * On Windows, map and unmap calls must be matched, so we deallocate the
 * unaligned chunk, then reallocate the unaligned part to block off the
 * old address and force the allocator to give us a new one.
 */
static void GetNewChunk(void** aAddress, void** aRetainedAddr, size_t size,
                        size_t alignment) {
  void* address = *aAddress;
  void* retainedAddr = nullptr;
  do {
    size_t retainedSize;
    size_t offset = OffsetFromAligned(address, alignment);
    if (!offset) {
      break;
    }
    UnmapPages(address, size);
    retainedSize = alignment - offset;
    retainedAddr = MapMemoryAt(address, retainedSize, MEM_RESERVE);
    address = MapMemory(size, MEM_COMMIT | MEM_RESERVE);
    /* If retainedAddr is null here, we raced with another thread. */
  } while (!retainedAddr);
  *aAddress = address;
  *aRetainedAddr = retainedAddr;
}

void UnmapPages(void* p, size_t size) {
  // ASan does not automatically unpoison memory, so we have to do this here.
  MOZ_MAKE_MEM_UNDEFINED(p, size);

  MOZ_ALWAYS_TRUE(VirtualFree(p, 0, MEM_RELEASE));
}

bool MarkPagesUnused(void* p, size_t size) {
  MOZ_MAKE_MEM_NOACCESS(p, size);
||||||| merged common ancestors
static void*
MapAlignedPagesLastDitch(size_t size, size_t alignment)
{
    void* tempMaps[MaxLastDitchAttempts];
    int attempt = 0;
    void* p = MapMemory(size, MEM_COMMIT | MEM_RESERVE);
    if (OffsetFromAligned(p, alignment) == 0) {
        return p;
    }
    for (; attempt < MaxLastDitchAttempts; ++attempt) {
        GetNewChunk(&p, tempMaps + attempt, size, alignment);
        if (OffsetFromAligned(p, alignment) == 0) {
            if (tempMaps[attempt]) {
                UnmapPages(tempMaps[attempt], size);
            }
            break;
        }
        if (!tempMaps[attempt]) {
            break; /* Bail if GetNewChunk failed. */
        }
    }
    if (OffsetFromAligned(p, alignment)) {
        UnmapPages(p, size);
        p = nullptr;
    }
    while (--attempt >= 0) {
        UnmapPages(tempMaps[attempt], size);
    }
    return p;
}

/*
 * On Windows, map and unmap calls must be matched, so we deallocate the
 * unaligned chunk, then reallocate the unaligned part to block off the
 * old address and force the allocator to give us a new one.
 */
static void
GetNewChunk(void** aAddress, void** aRetainedAddr, size_t size, size_t alignment)
{
    void* address = *aAddress;
    void* retainedAddr = nullptr;
    do {
        size_t retainedSize;
        size_t offset = OffsetFromAligned(address, alignment);
        if (!offset) {
            break;
        }
        UnmapPages(address, size);
        retainedSize = alignment - offset;
        retainedAddr = MapMemoryAt(address, retainedSize, MEM_RESERVE);
        address = MapMemory(size, MEM_COMMIT | MEM_RESERVE);
        /* If retainedAddr is null here, we raced with another thread. */
    } while (!retainedAddr);
    *aAddress = address;
    *aRetainedAddr = retainedAddr;
}

void
UnmapPages(void* p, size_t size)
{
    // ASan does not automatically unpoison memory, so we have to do this here.
    MOZ_MAKE_MEM_UNDEFINED(p, size);

    MOZ_ALWAYS_TRUE(VirtualFree(p, 0, MEM_RELEASE));
}

bool
MarkPagesUnused(void* p, size_t size)
{
    MOZ_MAKE_MEM_NOACCESS(p, size);
=======
static inline bool DecommitEnabled() { return pageSize == ArenaSize; }
>>>>>>> upstream-releases

<<<<<<< HEAD
  if (!DecommitEnabled()) {
    return true;
  }

  MOZ_ASSERT(OffsetFromAligned(p, pageSize) == 0);
  LPVOID p2 = MapMemoryAt(p, size, MEM_RESET);
  return p2 == p;
||||||| merged common ancestors
    if (!DecommitEnabled()) {
        return true;
    }

    MOZ_ASSERT(OffsetFromAligned(p, pageSize) == 0);
    LPVOID p2 = MapMemoryAt(p, size, MEM_RESET);
    return p2 == p;
=======
/* Returns the offset from the nearest aligned address at or below |region|. */
static inline size_t OffsetFromAligned(void* region, size_t alignment) {
  return uintptr_t(region) % alignment;
>>>>>>> upstream-releases
}

<<<<<<< HEAD
void MarkPagesInUse(void* p, size_t size) {
  MOZ_MAKE_MEM_UNDEFINED(p, size);

  if (!DecommitEnabled()) {
    return;
  }

  MOZ_ASSERT(OffsetFromAligned(p, pageSize) == 0);
}

size_t GetPageFaultCount() {
  if (mozilla::recordreplay::IsRecordingOrReplaying()) {
    return 0;
  }
  PROCESS_MEMORY_COUNTERS pmc;
  if (!GetProcessMemoryInfo(GetCurrentProcess(), &pmc, sizeof(pmc))) {
    return 0;
  }
  return pmc.PageFaultCount;
||||||| merged common ancestors
void
MarkPagesInUse(void* p, size_t size)
{
    MOZ_MAKE_MEM_UNDEFINED(p, size);

    if (!DecommitEnabled()) {
        return;
    }

    MOZ_ASSERT(OffsetFromAligned(p, pageSize) == 0);
}

size_t
GetPageFaultCount()
{
    if (mozilla::recordreplay::IsRecordingOrReplaying()) {
        return 0;
    }
    PROCESS_MEMORY_COUNTERS pmc;
    if (!GetProcessMemoryInfo(GetCurrentProcess(), &pmc, sizeof(pmc))) {
        return 0;
    }
    return pmc.PageFaultCount;
=======
template <Commit commit, PageAccess prot>
static inline void* MapInternal(void* desired, size_t length) {
  void* region = nullptr;
#ifdef XP_WIN
  DWORD flags =
      (commit == Commit::Yes ? MEM_RESERVE | MEM_COMMIT : MEM_RESERVE);
  region = VirtualAlloc(desired, length, flags, DWORD(prot));
#else
  int flags = MAP_PRIVATE | MAP_ANON;
  region = MozTaggedAnonymousMmap(desired, length, int(prot), flags, -1, 0,
                                  "js-gc-heap");
  if (region == MAP_FAILED) {
    return nullptr;
  }
#endif
  return region;
>>>>>>> upstream-releases
}

<<<<<<< HEAD
void* AllocateMappedContent(int fd, size_t offset, size_t length,
                            size_t alignment) {
  MOZ_ASSERT(length && alignment);

  // The allocation granularity and the requested offset
  // must both be divisible by the requested alignment.
  // Alignments larger than the allocation granularity are not supported.
  if (allocGranularity % alignment != 0 || offset % alignment != 0) {
    return nullptr;
  }

  HANDLE hFile = reinterpret_cast<HANDLE>(intptr_t(fd));

  // This call will fail if the file does not exist, which is what we want.
  HANDLE hMap = CreateFileMapping(hFile, nullptr, PAGE_READONLY, 0, 0, nullptr);
  if (!hMap) {
    return nullptr;
  }

  size_t alignedOffset = offset - (offset % allocGranularity);
  size_t alignedLength = length + (offset % allocGranularity);
||||||| merged common ancestors
void*
AllocateMappedContent(int fd, size_t offset, size_t length, size_t alignment)
{
    MOZ_ASSERT(length && alignment);

    // The allocation granularity and the requested offset
    // must both be divisible by the requested alignment.
    // Alignments larger than the allocation granularity are not supported.
    if (allocGranularity % alignment != 0 || offset % alignment != 0) {
        return nullptr;
    }

    HANDLE hFile = reinterpret_cast<HANDLE>(intptr_t(fd));

    // This call will fail if the file does not exist, which is what we want.
    HANDLE hMap = CreateFileMapping(hFile, nullptr, PAGE_READONLY, 0, 0, nullptr);
    if (!hMap) {
        return nullptr;
    }

    size_t alignedOffset = offset - (offset % allocGranularity);
    size_t alignedLength = length + (offset % allocGranularity);
=======
static inline void UnmapInternal(void* region, size_t length) {
  MOZ_ASSERT(region && OffsetFromAligned(region, allocGranularity) == 0);
  MOZ_ASSERT(length > 0 && length % pageSize == 0);
>>>>>>> upstream-releases

<<<<<<< HEAD
  DWORD offsetH = uint32_t(uint64_t(alignedOffset) >> 32);
  DWORD offsetL = uint32_t(alignedOffset);

  // If the offset or length are out of bounds, this call will fail.
  uint8_t* map = static_cast<uint8_t*>(
      MapViewOfFile(hMap, FILE_MAP_COPY, offsetH, offsetL, alignedLength));

  // This just decreases the file mapping object's internal reference count;
  // it won't actually be destroyed until we unmap the associated view.
  CloseHandle(hMap);

  if (!map) {
    return nullptr;
  }

#ifdef DEBUG
  // Zero out data before and after the desired mapping to catch errors early.
  if (offset != alignedOffset) {
    memset(map, 0, offset - alignedOffset);
  }
  if (alignedLength % pageSize) {
    memset(map + alignedLength, 0, pageSize - (alignedLength % pageSize));
  }
||||||| merged common ancestors
    DWORD offsetH = uint32_t(uint64_t(alignedOffset) >> 32);
    DWORD offsetL = uint32_t(alignedOffset);

    // If the offset or length are out of bounds, this call will fail.
    uint8_t* map = static_cast<uint8_t*>(MapViewOfFile(hMap, FILE_MAP_COPY, offsetH,
                                                       offsetL, alignedLength));

    // This just decreases the file mapping object's internal reference count;
    // it won't actually be destroyed until we unmap the associated view.
    CloseHandle(hMap);

    if (!map) {
        return nullptr;
    }

#ifdef DEBUG
    // Zero out data before and after the desired mapping to catch errors early.
    if (offset != alignedOffset) {
        memset(map, 0, offset - alignedOffset);
    }
    if (alignedLength % pageSize) {
        memset(map + alignedLength, 0, pageSize - (alignedLength % pageSize));
    }
=======
#ifdef XP_WIN
  MOZ_RELEASE_ASSERT(VirtualFree(region, 0, MEM_RELEASE) != 0);
#else
  if (munmap(region, length)) {
    MOZ_RELEASE_ASSERT(errno == ENOMEM);
  }
>>>>>>> upstream-releases
#endif
<<<<<<< HEAD

  return map + (offset - alignedOffset);
||||||| merged common ancestors

    return map + (offset - alignedOffset);
=======
>>>>>>> upstream-releases
}

<<<<<<< HEAD
void DeallocateMappedContent(void* p, size_t /*length*/) {
  if (!p) {
    return;
  }
||||||| merged common ancestors
void
DeallocateMappedContent(void* p, size_t /*length*/)
{
    if (!p) {
        return;
    }
=======
template <Commit commit = Commit::Yes, PageAccess prot = PageAccess::ReadWrite>
static inline void* MapMemory(size_t length) {
  MOZ_ASSERT(length > 0);
>>>>>>> upstream-releases

<<<<<<< HEAD
  // Calculate the address originally returned by MapViewOfFile.
  // This is needed because AllocateMappedContent returns a pointer
  // that might be offset from the view, as the beginning of a
  // view must be aligned with the allocation granularity.
  uintptr_t map = uintptr_t(p) - (uintptr_t(p) % allocGranularity);
  MOZ_ALWAYS_TRUE(UnmapViewOfFile(reinterpret_cast<void*>(map)));
||||||| merged common ancestors
    // Calculate the address originally returned by MapViewOfFile.
    // This is needed because AllocateMappedContent returns a pointer
    // that might be offset from the view, as the beginning of a
    // view must be aligned with the allocation granularity.
    uintptr_t map = uintptr_t(p) - (uintptr_t(p) % allocGranularity);
    MOZ_ALWAYS_TRUE(UnmapViewOfFile(reinterpret_cast<void*>(map)));
=======
  return MapInternal<commit, prot>(nullptr, length);
>>>>>>> upstream-releases
}

<<<<<<< HEAD
#else  // Various APIs are unavailable.

void* MapAlignedPages(size_t size, size_t alignment) {
  MOZ_ASSERT(size >= alignment);
  MOZ_ASSERT(size >= allocGranularity);
  MOZ_ASSERT(size % alignment == 0);
  MOZ_ASSERT(size % pageSize == 0);
  MOZ_ASSERT_IF(alignment < allocGranularity,
                allocGranularity % alignment == 0);
  MOZ_ASSERT_IF(alignment > allocGranularity,
                alignment % allocGranularity == 0);

  void* p = _aligned_malloc(size, alignment);
||||||| merged common ancestors
#  else // Various APIs are unavailable.

void*
MapAlignedPages(size_t size, size_t alignment)
{
    MOZ_ASSERT(size >= alignment);
    MOZ_ASSERT(size >= allocGranularity);
    MOZ_ASSERT(size % alignment == 0);
    MOZ_ASSERT(size % pageSize == 0);
    MOZ_ASSERT_IF(alignment < allocGranularity, allocGranularity % alignment == 0);
    MOZ_ASSERT_IF(alignment > allocGranularity, alignment % allocGranularity == 0);

    void* p = _aligned_malloc(size, alignment);
=======
/*
 * Attempts to map memory at the given address, but allows the system
 * to return a different address that may still be suitable.
 */
template <Commit commit = Commit::Yes, PageAccess prot = PageAccess::ReadWrite>
static inline void* MapMemoryAtFuzzy(void* desired, size_t length) {
  MOZ_ASSERT(desired && OffsetFromAligned(desired, allocGranularity) == 0);
  MOZ_ASSERT(length > 0);
>>>>>>> upstream-releases

<<<<<<< HEAD
  MOZ_ASSERT(OffsetFromAligned(p, alignment) == 0);
  return p;
||||||| merged common ancestors
    MOZ_ASSERT(OffsetFromAligned(p, alignment) == 0);
    return p;
=======
  // Note that some platforms treat the requested address as a hint, so the
  // returned address might not match the requested address.
  return MapInternal<commit, prot>(desired, length);
>>>>>>> upstream-releases
}

<<<<<<< HEAD
static void* MapAlignedPagesLastDitch(size_t size, size_t alignment) {
  return nullptr;
}

void UnmapPages(void* p, size_t size) {
  // ASan does not automatically unpoison memory, so we have to do this here.
  MOZ_MAKE_MEM_UNDEFINED(p, size);
||||||| merged common ancestors
static void*
MapAlignedPagesLastDitch(size_t size, size_t alignment)
{
    return nullptr;
}

void
UnmapPages(void* p, size_t size)
{
    // ASan does not automatically unpoison memory, so we have to do this here.
    MOZ_MAKE_MEM_UNDEFINED(p, size);
=======
/*
 * Attempts to map memory at the given address, returning nullptr if
 * the system returns any address other than the requested one.
 */
template <Commit commit = Commit::Yes, PageAccess prot = PageAccess::ReadWrite>
static inline void* MapMemoryAt(void* desired, size_t length) {
  MOZ_ASSERT(desired && OffsetFromAligned(desired, allocGranularity) == 0);
  MOZ_ASSERT(length > 0);
>>>>>>> upstream-releases

<<<<<<< HEAD
  _aligned_free(p);
}
||||||| merged common ancestors
    _aligned_free(p);
}
=======
  void* region = MapInternal<commit, prot>(desired, length);
  if (!region) {
    return nullptr;
  }
>>>>>>> upstream-releases

<<<<<<< HEAD
bool MarkPagesUnused(void* p, size_t size) {
  MOZ_MAKE_MEM_NOACCESS(p, size);
  MOZ_ASSERT(OffsetFromAligned(p, pageSize) == 0);
  return true;
||||||| merged common ancestors
bool
MarkPagesUnused(void* p, size_t size)
{
    MOZ_MAKE_MEM_NOACCESS(p, size);
    MOZ_ASSERT(OffsetFromAligned(p, pageSize) == 0);
    return true;
=======
  // On some platforms mmap treats the desired address as a hint, so
  // check that the address we got is the address we requested.
  if (region != desired) {
    UnmapInternal(region, length);
    return nullptr;
  }
  return region;
>>>>>>> upstream-releases
}

<<<<<<< HEAD
bool MarkPagesInUse(void* p, size_t size) {
  MOZ_MAKE_MEM_UNDEFINED(p, size);
  MOZ_ASSERT(OffsetFromAligned(p, pageSize) == 0);
}
||||||| merged common ancestors
bool
MarkPagesInUse(void* p, size_t size)
{
    MOZ_MAKE_MEM_UNDEFINED(p, size);
    MOZ_ASSERT(OffsetFromAligned(p, pageSize) == 0);
}
=======
#ifdef JS_64BIT
>>>>>>> upstream-releases

<<<<<<< HEAD
size_t GetPageFaultCount() {
  // GetProcessMemoryInfo is unavailable.
  return 0;
}
||||||| merged common ancestors
size_t
GetPageFaultCount()
{
    // GetProcessMemoryInfo is unavailable.
    return 0;
}
=======
/* Returns a random number in the given range. */
static inline uint64_t GetNumberInRange(uint64_t minNum, uint64_t maxNum) {
  const uint64_t MaxRand = UINT64_C(0xffffffffffffffff);
  maxNum -= minNum;
  uint64_t binSize = 1 + (MaxRand - maxNum) / (maxNum + 1);
>>>>>>> upstream-releases

<<<<<<< HEAD
void* AllocateMappedContent(int fd, size_t offset, size_t length,
                            size_t alignment) {
  // Not implemented.
  return nullptr;
}
||||||| merged common ancestors
void*
AllocateMappedContent(int fd, size_t offset, size_t length, size_t alignment)
{
    // Not implemented.
    return nullptr;
}
=======
  uint64_t rndNum;
  do {
    mozilla::Maybe<uint64_t> result;
    do {
      mozilla::recordreplay::AutoPassThroughThreadEvents pt;
      result = mozilla::RandomUint64();
    } while (!result);
    rndNum = result.value() / binSize;
  } while (rndNum > maxNum);
>>>>>>> upstream-releases

<<<<<<< HEAD
// Deallocate mapped memory for object.
void DeallocateMappedContent(void* p, size_t length) {
  // Not implemented.
||||||| merged common ancestors
// Deallocate mapped memory for object.
void
DeallocateMappedContent(void* p, size_t length)
{
    // Not implemented.
=======
  return minNum + rndNum;
>>>>>>> upstream-releases
}

<<<<<<< HEAD
#endif

#elif defined(SOLARIS)
||||||| merged common ancestors
#  endif

#elif defined(SOLARIS)
=======
#  ifndef XP_WIN
static inline uint64_t FindAddressLimitInner(size_t highBit, size_t tries);
>>>>>>> upstream-releases

<<<<<<< HEAD
#ifndef MAP_NOSYNC
#define MAP_NOSYNC 0
||||||| merged common ancestors
#ifndef MAP_NOSYNC
# define MAP_NOSYNC 0
=======
/*
 * The address range available to applications depends on both hardware and
 * kernel configuration. For example, AArch64 on Linux uses addresses with
 * 39 significant bits by default, but can be configured to use addresses with
 * 48 significant bits by enabling a 4th translation table. Unfortunately,
 * there appears to be no standard way to query the limit at runtime
 * (Windows exposes this via GetSystemInfo()).
 *
 * This function tries to find the address limit by performing a binary search
 * on the index of the most significant set bit in the addresses it attempts to
 * allocate. As the requested address is often treated as a hint by the
 * operating system, we use the actual returned addresses to narrow the range.
 * We return the number of bits of an address that may be set.
 */
static size_t FindAddressLimit() {
  // Use 32 bits as a lower bound in case we keep getting nullptr.
  uint64_t low = 31;
  uint64_t highestSeen = (UINT64_C(1) << 32) - allocGranularity - 1;

  // Exclude 48-bit and 47-bit addresses first.
  uint64_t high = 47;
  for (; high >= std::max(low, UINT64_C(46)); --high) {
    highestSeen = std::max(FindAddressLimitInner(high, 4), highestSeen);
    low = mozilla::FloorLog2(highestSeen);
  }
  // If those didn't work, perform a modified binary search.
  while (high - 1 > low) {
    uint64_t middle = low + (high - low) / 2;
    highestSeen = std::max(FindAddressLimitInner(middle, 4), highestSeen);
    low = mozilla::FloorLog2(highestSeen);
    if (highestSeen < (UINT64_C(1) << middle)) {
      high = middle;
    }
  }
  // We can be sure of the lower bound, but check the upper bound again.
  do {
    high = low + 1;
    highestSeen = std::max(FindAddressLimitInner(high, 8), highestSeen);
    low = mozilla::FloorLog2(highestSeen);
  } while (low >= high);

  // `low` is the highest set bit, so `low + 1` is the number of bits.
  return low + 1;
}

static inline uint64_t FindAddressLimitInner(size_t highBit, size_t tries) {
  const size_t length = allocGranularity;  // Used as both length and alignment.

  uint64_t highestSeen = 0;
  uint64_t startRaw = UINT64_C(1) << highBit;
  uint64_t endRaw = 2 * startRaw - length - 1;
  uint64_t start = (startRaw + length - 1) / length;
  uint64_t end = (endRaw - (length - 1)) / length;
  for (size_t i = 0; i < tries; ++i) {
    uint64_t desired = length * GetNumberInRange(start, end);
    void* address = MapMemoryAtFuzzy(reinterpret_cast<void*>(desired), length);
    uint64_t actual = uint64_t(address);
    if (address) {
      UnmapInternal(address, length);
    }
    if (actual > highestSeen) {
      highestSeen = actual;
      if (actual >= startRaw) {
        break;
      }
    }
  }
  return highestSeen;
}
#  endif  // !defined(XP_WIN)

#endif  // defined(JS_64BIT)

void InitMemorySubsystem() {
  if (pageSize == 0) {
#ifdef XP_WIN
    SYSTEM_INFO sysinfo;
    GetSystemInfo(&sysinfo);
    pageSize = sysinfo.dwPageSize;
    allocGranularity = sysinfo.dwAllocationGranularity;
#else
    pageSize = size_t(sysconf(_SC_PAGESIZE));
    allocGranularity = pageSize;
>>>>>>> upstream-releases
#endif
<<<<<<< HEAD

void InitMemorySubsystem() {
  if (pageSize == 0) {
    pageSize = allocGranularity = size_t(sysconf(_SC_PAGESIZE));
  }
}

void* MapAlignedPages(size_t size, size_t alignment) {
  MOZ_ASSERT(size >= alignment);
  MOZ_ASSERT(size >= allocGranularity);
  MOZ_ASSERT(size % alignment == 0);
  MOZ_ASSERT(size % pageSize == 0);
  MOZ_ASSERT_IF(alignment < allocGranularity,
                allocGranularity % alignment == 0);
  MOZ_ASSERT_IF(alignment > allocGranularity,
                alignment % allocGranularity == 0);

  int prot = PROT_READ | PROT_WRITE;
  int flags = MAP_PRIVATE | MAP_ANON | MAP_ALIGN | MAP_NOSYNC;

  void* p = mmap((caddr_t)alignment, size, prot, flags, -1, 0);
  if (p == MAP_FAILED) {
    return nullptr;
  }
  return p;
||||||| merged common ancestors

void
InitMemorySubsystem()
{
    if (pageSize == 0) {
        pageSize = allocGranularity = size_t(sysconf(_SC_PAGESIZE));
    }
}

void*
MapAlignedPages(size_t size, size_t alignment)
{
    MOZ_ASSERT(size >= alignment);
    MOZ_ASSERT(size >= allocGranularity);
    MOZ_ASSERT(size % alignment == 0);
    MOZ_ASSERT(size % pageSize == 0);
    MOZ_ASSERT_IF(alignment < allocGranularity, allocGranularity % alignment == 0);
    MOZ_ASSERT_IF(alignment > allocGranularity, alignment % allocGranularity == 0);

    int prot = PROT_READ | PROT_WRITE;
    int flags = MAP_PRIVATE | MAP_ANON | MAP_ALIGN | MAP_NOSYNC;

    void* p = mmap((caddr_t)alignment, size, prot, flags, -1, 0);
    if (p == MAP_FAILED) {
        return nullptr;
    }
    return p;
=======
#ifdef JS_64BIT
#  ifdef XP_WIN
    minValidAddress = size_t(sysinfo.lpMinimumApplicationAddress);
    maxValidAddress = size_t(sysinfo.lpMaximumApplicationAddress);
    numAddressBits = mozilla::FloorLog2(maxValidAddress) + 1;
#  else
    // No standard way to determine these, so fall back to FindAddressLimit().
    numAddressBits = FindAddressLimit();
    minValidAddress = allocGranularity;
    maxValidAddress = (UINT64_C(1) << numAddressBits) - 1 - allocGranularity;
#  endif
    // Sanity check the address to ensure we don't use more than 47 bits.
    uint64_t maxJSAddress = UINT64_C(0x00007fffffffffff) - allocGranularity;
    if (maxValidAddress > maxJSAddress) {
      maxValidAddress = maxJSAddress;
      hugeSplit = UINT64_C(0x00003fffffffffff) - allocGranularity;
    } else {
      hugeSplit = (UINT64_C(1) << (numAddressBits - 1)) - 1 - allocGranularity;
    }
#else  // !defined(JS_64BIT)
    numAddressBits = 32;
#endif
  }
>>>>>>> upstream-releases
}

<<<<<<< HEAD
static void* MapAlignedPagesLastDitch(size_t size, size_t alignment) {
  return nullptr;
||||||| merged common ancestors
static void*
MapAlignedPagesLastDitch(size_t size, size_t alignment)
{
    return nullptr;
=======
#ifdef JS_64BIT
/* The JS engine uses 47-bit pointers; all higher bits must be clear. */
static inline bool IsInvalidRegion(void* region, size_t length) {
  const uint64_t invalidPointerMask = UINT64_C(0xffff800000000000);
  return (uintptr_t(region) + length - 1) & invalidPointerMask;
>>>>>>> upstream-releases
}
#endif

<<<<<<< HEAD
void UnmapPages(void* p, size_t size) {
  // ASan does not automatically unpoison memory, so we have to do this here.
  MOZ_MAKE_MEM_UNDEFINED(p, size);

  MOZ_ALWAYS_TRUE(0 == munmap((caddr_t)p, size));
}
||||||| merged common ancestors
void
UnmapPages(void* p, size_t size)
{
    // ASan does not automatically unpoison memory, so we have to do this here.
    MOZ_MAKE_MEM_UNDEFINED(p, size);

    MOZ_ALWAYS_TRUE(0 == munmap((caddr_t)p, size));
}
=======
void* MapAlignedPages(size_t length, size_t alignment) {
  MOZ_RELEASE_ASSERT(length > 0 && alignment > 0);
  MOZ_RELEASE_ASSERT(length % pageSize == 0);
  MOZ_RELEASE_ASSERT(std::max(alignment, allocGranularity) %
                         std::min(alignment, allocGranularity) ==
                     0);
>>>>>>> upstream-releases

<<<<<<< HEAD
bool MarkPagesUnused(void* p, size_t size) {
  MOZ_MAKE_MEM_NOACCESS(p, size);
  MOZ_ASSERT(OffsetFromAligned(p, pageSize) == 0);
  return true;
}
||||||| merged common ancestors
bool
MarkPagesUnused(void* p, size_t size)
{
    MOZ_MAKE_MEM_NOACCESS(p, size);
    MOZ_ASSERT(OffsetFromAligned(p, pageSize) == 0);
    return true;
}
=======
  // Smaller alignments aren't supported by the allocation functions.
  if (alignment < allocGranularity) {
    alignment = allocGranularity;
  }
>>>>>>> upstream-releases

<<<<<<< HEAD
bool MarkPagesInUse(void* p, size_t size) {
  MOZ_MAKE_MEM_UNDEFINED(p, size);
||||||| merged common ancestors
bool
MarkPagesInUse(void* p, size_t size)
{
    MOZ_MAKE_MEM_UNDEFINED(p, size);
=======
#ifdef JS_64BIT
  // Use the scattershot allocator if the address range is large enough.
  if (UsingScattershotAllocator()) {
    void* region = MapAlignedPagesRandom(length, alignment);
>>>>>>> upstream-releases

<<<<<<< HEAD
  if (!DecommitEnabled()) {
    return;
  }
||||||| merged common ancestors
    if (!DecommitEnabled()) {
        return;
    }
=======
    MOZ_RELEASE_ASSERT(!IsInvalidRegion(region, length));
    MOZ_ASSERT(OffsetFromAligned(region, alignment) == 0);
>>>>>>> upstream-releases

<<<<<<< HEAD
  MOZ_ASSERT(OffsetFromAligned(p, pageSize) == 0);
}
||||||| merged common ancestors
    MOZ_ASSERT(OffsetFromAligned(p, pageSize) == 0);
}
=======
    return region;
  }
#endif
>>>>>>> upstream-releases

<<<<<<< HEAD
size_t GetPageFaultCount() { return 0; }
||||||| merged common ancestors
size_t
GetPageFaultCount()
{
    return 0;
}
=======
  // Try to allocate the region. If the returned address is aligned,
  // either we OOMed (region is nullptr) or we're done.
  void* region = MapMemory(length);
  if (OffsetFromAligned(region, alignment) == 0) {
    return region;
  }

  // Try to align the region. On success, TryToAlignChunk() returns
  // true and we can return the aligned region immediately.
  void* retainedRegion;
  if (TryToAlignChunk(&region, &retainedRegion, length, alignment)) {
    MOZ_ASSERT(region && OffsetFromAligned(region, alignment) == 0);
    MOZ_ASSERT(!retainedRegion);
    return region;
  }
>>>>>>> upstream-releases

<<<<<<< HEAD
void* AllocateMappedContent(int fd, size_t offset, size_t length,
                            size_t alignment) {
  // Not implemented.
  return nullptr;
}
||||||| merged common ancestors
void*
AllocateMappedContent(int fd, size_t offset, size_t length, size_t alignment)
{
    // Not implemented.
    return nullptr;
}
=======
  // On failure, the unaligned region is retained unless we OOMed. We don't
  // use the retained region on this path (see the last ditch allocator).
  if (retainedRegion) {
    UnmapInternal(retainedRegion, length);
  }
>>>>>>> upstream-releases

<<<<<<< HEAD
// Deallocate mapped memory for object.
void DeallocateMappedContent(void* p, size_t length) {
  // Not implemented.
}
||||||| merged common ancestors
// Deallocate mapped memory for object.
void
DeallocateMappedContent(void* p, size_t length)
{
    // Not implemented.
}
=======
  // If it fails to align the given region, TryToAlignChunk() returns the
  // next valid region that we might be able to align (unless we OOMed).
  if (region) {
    MOZ_ASSERT(OffsetFromAligned(region, alignment) != 0);
    UnmapInternal(region, length);
  }
>>>>>>> upstream-releases

  // Since we couldn't align the first region, fall back to allocating a
  // region large enough that we can definitely align it.
  region = MapAlignedPagesSlow(length, alignment);
  if (!region) {
    // If there wasn't enough contiguous address space left for that,
    // try to find an alignable region using the last ditch allocator.
    region = MapAlignedPagesLastDitch(length, alignment);
  }

<<<<<<< HEAD
void InitMemorySubsystem() {
  if (pageSize == 0) {
    pageSize = allocGranularity = size_t(sysconf(_SC_PAGESIZE));
  }
||||||| merged common ancestors
void
InitMemorySubsystem()
{
    if (pageSize == 0) {
        pageSize = allocGranularity = size_t(sysconf(_SC_PAGESIZE));
    }
=======
  // At this point we should either have an aligned region or nullptr.
  MOZ_ASSERT(OffsetFromAligned(region, alignment) == 0);
  return region;
>>>>>>> upstream-releases
}

<<<<<<< HEAD
static inline void* MapMemoryAt(void* desired, size_t length,
                                int prot = PROT_READ | PROT_WRITE,
                                int flags = MAP_PRIVATE | MAP_ANON, int fd = -1,
                                off_t offset = 0) {
||||||| merged common ancestors
static inline void*
MapMemoryAt(void* desired, size_t length, int prot = PROT_READ | PROT_WRITE,
            int flags = MAP_PRIVATE | MAP_ANON, int fd = -1, off_t offset = 0)
{
=======
#ifdef JS_64BIT
>>>>>>> upstream-releases

<<<<<<< HEAD
#if defined(__ia64__) || defined(__aarch64__) ||  \
    (defined(__sparc__) && defined(__arch64__) && \
     (defined(__NetBSD__) || defined(__linux__)))
  MOZ_ASSERT((0xffff800000000000ULL & (uintptr_t(desired) + length - 1)) == 0);
#endif
  void* region = mmap(desired, length, prot, flags, fd, offset);
  if (region == MAP_FAILED) {
    return nullptr;
  }
  /*
   * mmap treats the given address as a hint unless the MAP_FIXED flag is
   * used (which isn't usually what you want, as this overrides existing
   * mappings), so check that the address we got is the address we wanted.
   */
  if (region != desired) {
    if (munmap(region, length)) {
      MOZ_ASSERT(errno == ENOMEM);
    }
    return nullptr;
  }
  return region;
}

static inline void* MapMemory(size_t length, int prot = PROT_READ | PROT_WRITE,
                              int flags = MAP_PRIVATE | MAP_ANON, int fd = -1,
                              off_t offset = 0) {
#if defined(__ia64__) || \
    (defined(__sparc__) && defined(__arch64__) && defined(__NetBSD__))
  /*
   * The JS engine assumes that all allocated pointers have their high 17 bits
   * clear, which ia64's mmap doesn't support directly. However, we can emulate
   * it by passing mmap an "addr" parameter with those bits clear. The mmap will
   * return that address, or the nearest available memory above that address,
   * providing a near-guarantee that those bits are clear. If they are not, we
   * return nullptr below to indicate out-of-memory.
   *
   * The addr is chosen as 0x0000070000000000, which still allows about 120TB of
   * virtual address space.
   *
   * See Bug 589735 for more information.
   */
  void* region =
      mmap((void*)0x0000070000000000, length, prot, flags, fd, offset);
  if (region == MAP_FAILED) {
    return nullptr;
  }
  /*
   * If the allocated memory doesn't have its upper 17 bits clear, consider it
   * as out of memory.
   */
  if ((uintptr_t(region) + (length - 1)) & 0xffff800000000000) {
    if (munmap(region, length)) {
      MOZ_ASSERT(errno == ENOMEM);
    }
    return nullptr;
  }
  return region;
#elif defined(__aarch64__) || \
    (defined(__sparc__) && defined(__arch64__) && defined(__linux__))
  /*
   * There might be similar virtual address issue on arm64 which depends on
   * hardware and kernel configurations. But the work around is slightly
   * different due to the different mmap behavior.
   *
   * TODO: Merge with the above code block if this implementation works for
   * ia64 and sparc64.
   */
  const uintptr_t start = UINT64_C(0x0000070000000000);
  const uintptr_t end = UINT64_C(0x0000800000000000);
  const uintptr_t step = ChunkSize;
  /*
   * Optimization options if there are too many retries in practice:
   * 1. Examine /proc/self/maps to find an available address. This file is
   *    not always available, however. In addition, even if we examine
   *    /proc/self/maps, we may still need to retry several times due to
   *    racing with other threads.
   * 2. Use a global/static variable with lock to track the addresses we have
   *    allocated or tried.
   */
  uintptr_t hint;
  void* region = MAP_FAILED;
  for (hint = start; region == MAP_FAILED && hint + length <= end;
       hint += step) {
    region = mmap((void*)hint, length, prot, flags, fd, offset);
    if (region != MAP_FAILED) {
      if ((uintptr_t(region) + (length - 1)) & 0xffff800000000000) {
        if (munmap(region, length)) {
          MOZ_ASSERT(errno == ENOMEM);
        }
        region = MAP_FAILED;
      }
    }
  }
  return region == MAP_FAILED ? nullptr : region;
#else
  void* region = MozTaggedAnonymousMmap(nullptr, length, prot, flags, fd,
                                        offset, "js-gc-heap");
  if (region == MAP_FAILED) {
    return nullptr;
  }
  return region;
#endif
}

void* MapAlignedPages(size_t size, size_t alignment) {
  MOZ_ASSERT(size >= alignment);
  MOZ_ASSERT(size >= allocGranularity);
  MOZ_ASSERT(size % alignment == 0);
  MOZ_ASSERT(size % pageSize == 0);
  MOZ_ASSERT_IF(alignment < allocGranularity,
                allocGranularity % alignment == 0);
  MOZ_ASSERT_IF(alignment > allocGranularity,
                alignment % allocGranularity == 0);

  void* p = MapMemory(size);

  /* Special case: If we want page alignment, no further work is needed. */
  if (alignment == allocGranularity) {
    return p;
  }

  if (OffsetFromAligned(p, alignment) == 0) {
    return p;
  }

  void* retainedAddr;
  GetNewChunk(&p, &retainedAddr, size, alignment);
  if (retainedAddr) {
    UnmapPages(retainedAddr, size);
  }
  if (p) {
    if (OffsetFromAligned(p, alignment) == 0) {
      return p;
||||||| merged common ancestors
#if defined(__ia64__) || defined(__aarch64__) || \
    (defined(__sparc__) && defined(__arch64__) && (defined(__NetBSD__) || defined(__linux__)))
    MOZ_ASSERT((0xffff800000000000ULL & (uintptr_t(desired) + length - 1)) == 0);
#endif
    void* region = mmap(desired, length, prot, flags, fd, offset);
    if (region == MAP_FAILED) {
        return nullptr;
    }
    /*
     * mmap treats the given address as a hint unless the MAP_FIXED flag is
     * used (which isn't usually what you want, as this overrides existing
     * mappings), so check that the address we got is the address we wanted.
     */
    if (region != desired) {
        if (munmap(region, length)) {
            MOZ_ASSERT(errno == ENOMEM);
        }
        return nullptr;
    }
    return region;
}

static inline void*
MapMemory(size_t length, int prot = PROT_READ | PROT_WRITE,
          int flags = MAP_PRIVATE | MAP_ANON, int fd = -1, off_t offset = 0)
{
#if defined(__ia64__) || (defined(__sparc__) && defined(__arch64__) && defined(__NetBSD__))
    /*
     * The JS engine assumes that all allocated pointers have their high 17 bits clear,
     * which ia64's mmap doesn't support directly. However, we can emulate it by passing
     * mmap an "addr" parameter with those bits clear. The mmap will return that address,
     * or the nearest available memory above that address, providing a near-guarantee
     * that those bits are clear. If they are not, we return nullptr below to indicate
     * out-of-memory.
     *
     * The addr is chosen as 0x0000070000000000, which still allows about 120TB of virtual
     * address space.
     *
     * See Bug 589735 for more information.
     */
    void* region = mmap((void*)0x0000070000000000, length, prot, flags, fd, offset);
    if (region == MAP_FAILED) {
        return nullptr;
    }
    /*
     * If the allocated memory doesn't have its upper 17 bits clear, consider it
     * as out of memory.
     */
    if ((uintptr_t(region) + (length - 1)) & 0xffff800000000000) {
        if (munmap(region, length)) {
            MOZ_ASSERT(errno == ENOMEM);
        }
        return nullptr;
    }
    return region;
#elif defined(__aarch64__) || (defined(__sparc__) && defined(__arch64__) && defined(__linux__))
   /*
    * There might be similar virtual address issue on arm64 which depends on
    * hardware and kernel configurations. But the work around is slightly
    * different due to the different mmap behavior.
    *
    * TODO: Merge with the above code block if this implementation works for
    * ia64 and sparc64.
    */
    const uintptr_t start = UINT64_C(0x0000070000000000);
    const uintptr_t end   = UINT64_C(0x0000800000000000);
    const uintptr_t step  = ChunkSize;
   /*
    * Optimization options if there are too many retries in practice:
    * 1. Examine /proc/self/maps to find an available address. This file is
    *    not always available, however. In addition, even if we examine
    *    /proc/self/maps, we may still need to retry several times due to
    *    racing with other threads.
    * 2. Use a global/static variable with lock to track the addresses we have
    *    allocated or tried.
    */
    uintptr_t hint;
    void* region = MAP_FAILED;
    for (hint = start; region == MAP_FAILED && hint + length <= end; hint += step) {
        region = mmap((void*)hint, length, prot, flags, fd, offset);
        if (region != MAP_FAILED) {
            if ((uintptr_t(region) + (length - 1)) & 0xffff800000000000) {
                if (munmap(region, length)) {
                    MOZ_ASSERT(errno == ENOMEM);
                }
                region = MAP_FAILED;
            }
        }
    }
    return region == MAP_FAILED ? nullptr : region;
#else
    void* region = MozTaggedAnonymousMmap(nullptr, length, prot, flags, fd, offset, "js-gc-heap");
    if (region == MAP_FAILED) {
        return nullptr;
    }
    return region;
#endif
}

void*
MapAlignedPages(size_t size, size_t alignment)
{
    MOZ_ASSERT(size >= alignment);
    MOZ_ASSERT(size >= allocGranularity);
    MOZ_ASSERT(size % alignment == 0);
    MOZ_ASSERT(size % pageSize == 0);
    MOZ_ASSERT_IF(alignment < allocGranularity, allocGranularity % alignment == 0);
    MOZ_ASSERT_IF(alignment > allocGranularity, alignment % allocGranularity == 0);

    void* p = MapMemory(size);

    /* Special case: If we want page alignment, no further work is needed. */
    if (alignment == allocGranularity) {
        return p;
    }

    if (OffsetFromAligned(p, alignment) == 0) {
        return p;
=======
/*
 * This allocator takes advantage of the large address range on some 64-bit
 * platforms to allocate in a scattershot manner, choosing addresses at random
 * from the range. By controlling the range we can avoid returning addresses
 * that have more than 47 significant bits (as required by SpiderMonkey).
 * This approach also has some other advantages over the methods employed by
 * the other allocation functions in this file:
 * 1) Allocations are extremely likely to succeed on the first try.
 * 2) The randomness makes our memory layout becomes harder to predict.
 * 3) The low probability of reusing regions guards against use-after-free.
 *
 * The main downside is that detecting physical OOM situations becomes more
 * difficult; to guard against this, we occasionally try a regular allocation.
 * In addition, sprinkling small allocations throughout the full address range
 * might get in the way of large address space reservations such as those
 * employed by WebAssembly. To avoid this (or the opposite problem of such
 * reservations reducing the chance of success for smaller allocations) we
 * split the address range in half, with one half reserved for huge allocations
 * and the other for regular (usually chunk sized) allocations.
 */
static void* MapAlignedPagesRandom(size_t length, size_t alignment) {
  uint64_t minNum, maxNum;
  if (length < HugeAllocationSize) {
    // Use the lower half of the range.
    minNum = (minValidAddress + alignment - 1) / alignment;
    maxNum = (hugeSplit - (length - 1)) / alignment;
  } else {
    // Use the upper half of the range.
    minNum = (hugeSplit + 1 + alignment - 1) / alignment;
    maxNum = (maxValidAddress - (length - 1)) / alignment;
  }

  // Try to allocate in random aligned locations.
  void* region = nullptr;
  for (size_t i = 1; i <= 1024; ++i) {
    if (i & 0xf) {
      uint64_t desired = alignment * GetNumberInRange(minNum, maxNum);
      region = MapMemoryAtFuzzy(reinterpret_cast<void*>(desired), length);
      if (!region) {
        continue;
      }
    } else {
      // Check for OOM.
      region = MapMemory(length);
      if (!region) {
        return nullptr;
      }
    }
    if (IsInvalidRegion(region, length)) {
      UnmapInternal(region, length);
      continue;
    }
    if (OffsetFromAligned(region, alignment) == 0) {
      return region;
>>>>>>> upstream-releases
    }
<<<<<<< HEAD
    UnmapPages(p, size);
  }

  p = MapAlignedPagesSlow(size, alignment);
  if (!p) {
    return MapAlignedPagesLastDitch(size, alignment);
  }
||||||| merged common ancestors

    void* retainedAddr;
    GetNewChunk(&p, &retainedAddr, size, alignment);
    if (retainedAddr) {
        UnmapPages(retainedAddr, size);
    }
    if (p) {
        if (OffsetFromAligned(p, alignment) == 0) {
            return p;
        }
        UnmapPages(p, size);
    }

    p = MapAlignedPagesSlow(size, alignment);
    if (!p) {
        return MapAlignedPagesLastDitch(size, alignment);
    }
=======
    void* retainedRegion = nullptr;
    if (TryToAlignChunk<false>(&region, &retainedRegion, length, alignment)) {
      MOZ_ASSERT(region && OffsetFromAligned(region, alignment) == 0);
      MOZ_ASSERT(!retainedRegion);
      return region;
    }
    MOZ_ASSERT(region && !retainedRegion);
    UnmapInternal(region, length);
  }

  if (numAddressBits < 48) {
    // Try the reliable fallback of overallocating.
    // Note: This will not respect the address space split.
    region = MapAlignedPagesSlow(length, alignment);
    if (region) {
      return region;
    }
  }
  if (length < HugeAllocationSize) {
    MOZ_CRASH("Couldn't allocate even after 1000 tries!");
  }
>>>>>>> upstream-releases

<<<<<<< HEAD
  MOZ_ASSERT(OffsetFromAligned(p, alignment) == 0);
  return p;
||||||| merged common ancestors
    MOZ_ASSERT(OffsetFromAligned(p, alignment) == 0);
    return p;
=======
  return nullptr;
>>>>>>> upstream-releases
}

<<<<<<< HEAD
static void* MapAlignedPagesSlow(size_t size, size_t alignment) {
  /* Overallocate and unmap the region's edges. */
  size_t reqSize = size + alignment - pageSize;
  void* region = MapMemory(reqSize);
  if (!region) {
    return nullptr;
  }

  void* regionEnd = (void*)(uintptr_t(region) + reqSize);
  void* front;
  void* end;
  if (growthDirection <= 0) {
    size_t offset = OffsetFromAligned(regionEnd, alignment);
    end = (void*)(uintptr_t(regionEnd) - offset);
    front = (void*)(uintptr_t(end) - size);
  } else {
    size_t offset = OffsetFromAligned(region, alignment);
    front = (void*)(uintptr_t(region) + (offset ? alignment - offset : 0));
    end = (void*)(uintptr_t(front) + size);
  }

  if (front != region) {
    UnmapPages(region, uintptr_t(front) - uintptr_t(region));
  }
  if (end != regionEnd) {
    UnmapPages(end, uintptr_t(regionEnd) - uintptr_t(end));
  }

  return front;
||||||| merged common ancestors
static void*
MapAlignedPagesSlow(size_t size, size_t alignment)
{
    /* Overallocate and unmap the region's edges. */
    size_t reqSize = size + alignment - pageSize;
    void* region = MapMemory(reqSize);
    if (!region) {
        return nullptr;
    }

    void* regionEnd = (void*)(uintptr_t(region) + reqSize);
    void* front;
    void* end;
    if (growthDirection <= 0) {
        size_t offset = OffsetFromAligned(regionEnd, alignment);
        end = (void*)(uintptr_t(regionEnd) - offset);
        front = (void*)(uintptr_t(end) - size);
    } else {
        size_t offset = OffsetFromAligned(region, alignment);
        front = (void*)(uintptr_t(region) + (offset ? alignment - offset : 0));
        end = (void*)(uintptr_t(front) + size);
    }

    if (front != region) {
        UnmapPages(region, uintptr_t(front) - uintptr_t(region));
    }
    if (end != regionEnd) {
        UnmapPages(end, uintptr_t(regionEnd) - uintptr_t(end));
    }

    return front;
=======
#endif  // defined(JS_64BIT)

static void* MapAlignedPagesSlow(size_t length, size_t alignment) {
  void* alignedRegion = nullptr;
  do {
    size_t reserveLength = length + alignment - pageSize;
#ifdef XP_WIN
    // Don't commit the requested pages as we won't use the region directly.
    void* region = MapMemory<Commit::No>(reserveLength);
#else
    void* region = MapMemory(reserveLength);
#endif
    if (!region) {
      return nullptr;
    }
    alignedRegion =
        reinterpret_cast<void*>(AlignBytes(uintptr_t(region), alignment));
#ifdef XP_WIN
    // Windows requires that map and unmap calls be matched, so deallocate
    // and immediately reallocate at the desired (aligned) address.
    UnmapInternal(region, reserveLength);
    alignedRegion = MapMemoryAt(alignedRegion, length);
#else
    // munmap allows us to simply unmap the pages that don't interest us.
    if (alignedRegion != region) {
      UnmapInternal(region, uintptr_t(alignedRegion) - uintptr_t(region));
    }
    void* regionEnd =
        reinterpret_cast<void*>(uintptr_t(region) + reserveLength);
    void* alignedEnd =
        reinterpret_cast<void*>(uintptr_t(alignedRegion) + length);
    if (alignedEnd != regionEnd) {
      UnmapInternal(alignedEnd, uintptr_t(regionEnd) - uintptr_t(alignedEnd));
    }
#endif
    // On Windows we may have raced with another thread; if so, try again.
  } while (!alignedRegion);

  return alignedRegion;
>>>>>>> upstream-releases
}

/*
 * In a low memory or high fragmentation situation, alignable chunks of the
 * desired length may still be available, even if there are no more contiguous
 * free chunks that meet the |length + alignment - pageSize| requirement of
 * MapAlignedPagesSlow. In this case, try harder to find an alignable chunk
 * by temporarily holding onto the unaligned parts of each chunk until the
 * allocator gives us a chunk that either is, or can be aligned.
 */
<<<<<<< HEAD
static void* MapAlignedPagesLastDitch(size_t size, size_t alignment) {
  void* tempMaps[MaxLastDitchAttempts];
  int attempt = 0;
  void* p = MapMemory(size);
  if (OffsetFromAligned(p, alignment) == 0) {
    return p;
  }
  for (; attempt < MaxLastDitchAttempts; ++attempt) {
    GetNewChunk(&p, tempMaps + attempt, size, alignment);
    if (OffsetFromAligned(p, alignment) == 0) {
      if (tempMaps[attempt]) {
        UnmapPages(tempMaps[attempt], size);
      }
      break;
||||||| merged common ancestors
static void*
MapAlignedPagesLastDitch(size_t size, size_t alignment)
{
    void* tempMaps[MaxLastDitchAttempts];
    int attempt = 0;
    void* p = MapMemory(size);
    if (OffsetFromAligned(p, alignment) == 0) {
        return p;
    }
    for (; attempt < MaxLastDitchAttempts; ++attempt) {
        GetNewChunk(&p, tempMaps + attempt, size, alignment);
        if (OffsetFromAligned(p, alignment) == 0) {
            if (tempMaps[attempt]) {
                UnmapPages(tempMaps[attempt], size);
            }
            break;
        }
        if (!tempMaps[attempt]) {
            break; /* Bail if GetNewChunk failed. */
        }
    }
    if (OffsetFromAligned(p, alignment)) {
        UnmapPages(p, size);
        p = nullptr;
    }
    while (--attempt >= 0) {
        UnmapPages(tempMaps[attempt], size);
=======
static void* MapAlignedPagesLastDitch(size_t length, size_t alignment) {
  void* tempMaps[MaxLastDitchAttempts];
  int attempt = 0;
  void* region = MapMemory(length);
  if (OffsetFromAligned(region, alignment) == 0) {
    return region;
  }
  for (; attempt < MaxLastDitchAttempts; ++attempt) {
    if (TryToAlignChunk(&region, tempMaps + attempt, length, alignment)) {
      MOZ_ASSERT(region && OffsetFromAligned(region, alignment) == 0);
      MOZ_ASSERT(!tempMaps[attempt]);
      break;  // Success!
    }
    if (!region || !tempMaps[attempt]) {
      break;  // We ran out of memory, so give up.
    }
  }
  if (OffsetFromAligned(region, alignment)) {
    UnmapInternal(region, length);
    region = nullptr;
  }
  while (--attempt >= 0) {
    UnmapInternal(tempMaps[attempt], length);
  }
  return region;
}

#ifdef XP_WIN

/*
 * On Windows, map and unmap calls must be matched, so we deallocate the
 * unaligned chunk, then reallocate the unaligned part to block off the
 * old address and force the allocator to give us a new one.
 */
template <bool>
static bool TryToAlignChunk(void** aRegion, void** aRetainedRegion,
                            size_t length, size_t alignment) {
  void* region = *aRegion;
  MOZ_ASSERT(region && OffsetFromAligned(region, alignment) != 0);

  size_t retainedLength = 0;
  void* retainedRegion = nullptr;
  do {
    size_t offset = OffsetFromAligned(region, alignment);
    if (offset == 0) {
      // If the address is aligned, either we hit OOM or we're done.
      break;
>>>>>>> upstream-releases
    }
<<<<<<< HEAD
    if (!tempMaps[attempt]) {
      break; /* Bail if GetNewChunk failed. */
    }
  }
  if (OffsetFromAligned(p, alignment)) {
    UnmapPages(p, size);
    p = nullptr;
  }
  while (--attempt >= 0) {
    UnmapPages(tempMaps[attempt], size);
  }
  return p;
||||||| merged common ancestors
    return p;
=======
    UnmapInternal(region, length);
    retainedLength = alignment - offset;
    retainedRegion = MapMemoryAt<Commit::No>(region, retainedLength);
    region = MapMemory(length);

    // If retainedRegion is null here, we raced with another thread.
  } while (!retainedRegion);

  bool result = OffsetFromAligned(region, alignment) == 0;
  if (result && retainedRegion) {
    UnmapInternal(retainedRegion, retainedLength);
    retainedRegion = nullptr;
  }

  *aRegion = region;
  *aRetainedRegion = retainedRegion;
  return region && result;
>>>>>>> upstream-releases
}

#else  // !defined(XP_WIN)

/*
 * mmap calls don't have to be matched with calls to munmap, so we can unmap
 * just the pages we don't need. However, as we don't know a priori if addresses
 * are handed out in increasing or decreasing order, we have to try both
 * directions (depending on the environment, one will always fail).
 */
<<<<<<< HEAD
static void GetNewChunk(void** aAddress, void** aRetainedAddr, size_t size,
                        size_t alignment) {
  void* address = *aAddress;
  void* retainedAddr = nullptr;
  bool addrsGrowDown = growthDirection <= 0;
  int i = 0;
  for (; i < 2; ++i) {
    /* Try the direction indicated by growthDirection. */
    if (addrsGrowDown) {
      size_t offset = OffsetFromAligned(address, alignment);
      void* head = (void*)((uintptr_t)address - offset);
      void* tail = (void*)((uintptr_t)head + size);
      if (MapMemoryAt(head, offset)) {
        UnmapPages(tail, offset);
        if (growthDirection >= -8) {
          --growthDirection;
||||||| merged common ancestors
static void
GetNewChunk(void** aAddress, void** aRetainedAddr, size_t size, size_t alignment)
{
    void* address = *aAddress;
    void* retainedAddr = nullptr;
    bool addrsGrowDown = growthDirection <= 0;
    int i = 0;
    for (; i < 2; ++i) {
        /* Try the direction indicated by growthDirection. */
        if (addrsGrowDown) {
            size_t offset = OffsetFromAligned(address, alignment);
            void* head = (void*)((uintptr_t)address - offset);
            void* tail = (void*)((uintptr_t)head + size);
            if (MapMemoryAt(head, offset)) {
                UnmapPages(tail, offset);
                if (growthDirection >= -8) {
                    --growthDirection;
                }
                address = head;
                break;
            }
        } else {
            size_t offset = alignment - OffsetFromAligned(address, alignment);
            void* head = (void*)((uintptr_t)address + offset);
            void* tail = (void*)((uintptr_t)address + size);
            if (MapMemoryAt(tail, offset)) {
                UnmapPages(address, offset);
                if (growthDirection <= 8) {
                    ++growthDirection;
                }
                address = head;
                break;
            }
=======
template <bool AlwaysGetNew>
static bool TryToAlignChunk(void** aRegion, void** aRetainedRegion,
                            size_t length, size_t alignment) {
  void* regionStart = *aRegion;
  MOZ_ASSERT(regionStart && OffsetFromAligned(regionStart, alignment) != 0);

  bool addressesGrowUpward = growthDirection > 0;
  bool directionUncertain = -8 < growthDirection && growthDirection <= 8;
  size_t offsetLower = OffsetFromAligned(regionStart, alignment);
  size_t offsetUpper = alignment - offsetLower;
  for (size_t i = 0; i < 2; ++i) {
    if (addressesGrowUpward) {
      void* upperStart =
          reinterpret_cast<void*>(uintptr_t(regionStart) + offsetUpper);
      void* regionEnd =
          reinterpret_cast<void*>(uintptr_t(regionStart) + length);
      if (MapMemoryAt(regionEnd, offsetUpper)) {
        UnmapInternal(regionStart, offsetUpper);
        if (directionUncertain) {
          ++growthDirection;
>>>>>>> upstream-releases
        }
<<<<<<< HEAD
        address = head;
        break;
      }
    } else {
      size_t offset = alignment - OffsetFromAligned(address, alignment);
      void* head = (void*)((uintptr_t)address + offset);
      void* tail = (void*)((uintptr_t)address + size);
      if (MapMemoryAt(tail, offset)) {
        UnmapPages(address, offset);
        if (growthDirection <= 8) {
          ++growthDirection;
||||||| merged common ancestors
        /* If we're confident in the growth direction, don't try the other. */
        if (growthDirection < -8 || growthDirection > 8) {
            break;
=======
        regionStart = upperStart;
        break;
      }
    } else {
      void* lowerStart =
          reinterpret_cast<void*>(uintptr_t(regionStart) - offsetLower);
      void* lowerEnd = reinterpret_cast<void*>(uintptr_t(lowerStart) + length);
      if (MapMemoryAt(lowerStart, offsetLower)) {
        UnmapInternal(lowerEnd, offsetLower);
        if (directionUncertain) {
          --growthDirection;
>>>>>>> upstream-releases
        }
<<<<<<< HEAD
        address = head;
        break;
      }
||||||| merged common ancestors
        /* If that failed, try the opposite direction. */
        addrsGrowDown = !addrsGrowDown;
=======
        regionStart = lowerStart;
        break;
      }
>>>>>>> upstream-releases
    }
<<<<<<< HEAD
    /* If we're confident in the growth direction, don't try the other. */
    if (growthDirection < -8 || growthDirection > 8) {
      break;
||||||| merged common ancestors
    /* If our current chunk cannot be aligned, see if the next one is aligned. */
    if (OffsetFromAligned(address, alignment)) {
        retainedAddr = address;
        address = MapMemory(size);
=======
    // If we're confident in the growth direction, don't try the other.
    if (!directionUncertain) {
      break;
>>>>>>> upstream-releases
    }
<<<<<<< HEAD
    /* If that failed, try the opposite direction. */
    addrsGrowDown = !addrsGrowDown;
  }
  /* If our current chunk cannot be aligned, see if the next one is aligned. */
  if (OffsetFromAligned(address, alignment)) {
    retainedAddr = address;
    address = MapMemory(size);
  }
  *aAddress = address;
  *aRetainedAddr = retainedAddr;
||||||| merged common ancestors
    *aAddress = address;
    *aRetainedAddr = retainedAddr;
=======
    addressesGrowUpward = !addressesGrowUpward;
  }

  void* retainedRegion = nullptr;
  bool result = OffsetFromAligned(regionStart, alignment) == 0;
  if (AlwaysGetNew && !result) {
    // If our current chunk cannot be aligned, just get a new one.
    retainedRegion = regionStart;
    regionStart = MapMemory(length);
    // Our new region might happen to already be aligned.
    result = OffsetFromAligned(regionStart, alignment) == 0;
    if (result) {
      UnmapInternal(retainedRegion, length);
      retainedRegion = nullptr;
    }
  }

  *aRegion = regionStart;
  *aRetainedRegion = retainedRegion;
  return regionStart && result;
>>>>>>> upstream-releases
}

<<<<<<< HEAD
void UnmapPages(void* p, size_t size) {
  // ASan does not automatically unpoison memory, so we have to do this here.
  MOZ_MAKE_MEM_UNDEFINED(p, size);
||||||| merged common ancestors
void
UnmapPages(void* p, size_t size)
{
    // ASan does not automatically unpoison memory, so we have to do this here.
    MOZ_MAKE_MEM_UNDEFINED(p, size);
=======
#endif
>>>>>>> upstream-releases

<<<<<<< HEAD
  if (munmap(p, size)) {
    MOZ_ASSERT(errno == ENOMEM);
  }
||||||| merged common ancestors
    if (munmap(p, size)) {
        MOZ_ASSERT(errno == ENOMEM);
    }
=======
void UnmapPages(void* region, size_t length) {
  MOZ_RELEASE_ASSERT(region &&
                     OffsetFromAligned(region, allocGranularity) == 0);
  MOZ_RELEASE_ASSERT(length > 0 && length % pageSize == 0);

  // ASan does not automatically unpoison memory, so we have to do this here.
  MOZ_MAKE_MEM_UNDEFINED(region, length);

  UnmapInternal(region, length);
>>>>>>> upstream-releases
}

<<<<<<< HEAD
bool MarkPagesUnused(void* p, size_t size) {
  MOZ_MAKE_MEM_NOACCESS(p, size);
||||||| merged common ancestors
bool
MarkPagesUnused(void* p, size_t size)
{
    MOZ_MAKE_MEM_NOACCESS(p, size);
=======
bool MarkPagesUnused(void* region, size_t length) {
  MOZ_RELEASE_ASSERT(region);
  MOZ_RELEASE_ASSERT(length > 0);
>>>>>>> upstream-releases

<<<<<<< HEAD
  if (!DecommitEnabled()) {
    return false;
  }
||||||| merged common ancestors
    if (!DecommitEnabled()) {
        return false;
    }
=======
  // pageSize == ArenaSize doesn't necessarily hold, but this function is
  // used by the GC to decommit unused Arenas, so we don't want to assert
  // if pageSize > ArenaSize.
  MOZ_ASSERT(OffsetFromAligned(region, ArenaSize) == 0);
  MOZ_ASSERT(length % ArenaSize == 0);

  MOZ_MAKE_MEM_NOACCESS(region, length);
>>>>>>> upstream-releases

<<<<<<< HEAD
  MOZ_ASSERT(OffsetFromAligned(p, pageSize) == 0);
#if defined(XP_SOLARIS)
  int result = posix_madvise(p, size, POSIX_MADV_DONTNEED);
||||||| merged common ancestors
    MOZ_ASSERT(OffsetFromAligned(p, pageSize) == 0);
#if defined(XP_SOLARIS)
    int result = posix_madvise(p, size, POSIX_MADV_DONTNEED);
=======
  if (!DecommitEnabled()) {
    return true;
  }
  // We can't decommit part of a page.
  MOZ_RELEASE_ASSERT(OffsetFromAligned(region, pageSize) == 0);
  MOZ_RELEASE_ASSERT(length % pageSize == 0);

#if defined(XP_WIN)
  return VirtualAlloc(region, length, MEM_RESET,
                      DWORD(PageAccess::ReadWrite)) == region;
#elif defined(XP_DARWIN)
  return madvise(region, length, MADV_FREE) == 0;
#elif defined(XP_SOLARIS)
  return posix_madvise(region, length, POSIX_MADV_DONTNEED) == 0;
>>>>>>> upstream-releases
#else
<<<<<<< HEAD
  int result = madvise(p, size, MADV_DONTNEED);
||||||| merged common ancestors
    int result = madvise(p, size, MADV_DONTNEED);
=======
  return madvise(region, length, MADV_DONTNEED) == 0;
>>>>>>> upstream-releases
#endif
<<<<<<< HEAD
  return result != -1;
||||||| merged common ancestors
    return result != -1;
=======
>>>>>>> upstream-releases
}

<<<<<<< HEAD
void MarkPagesInUse(void* p, size_t size) {
  MOZ_MAKE_MEM_UNDEFINED(p, size);
||||||| merged common ancestors
void
MarkPagesInUse(void* p, size_t size)
{
    MOZ_MAKE_MEM_UNDEFINED(p, size);
=======
void MarkPagesInUse(void* region, size_t length) {
  MOZ_RELEASE_ASSERT(region);
  MOZ_RELEASE_ASSERT(length > 0);
>>>>>>> upstream-releases

<<<<<<< HEAD
  if (!DecommitEnabled()) {
    return;
  }
||||||| merged common ancestors
    if (!DecommitEnabled()) {
        return;
    }
=======
  // pageSize == ArenaSize doesn't necessarily hold, but this function is
  // used by the GC to recommit Arenas that were previously decommitted,
  // so we don't want to assert if pageSize > ArenaSize.
  MOZ_ASSERT(OffsetFromAligned(region, ArenaSize) == 0);
  MOZ_ASSERT(length % ArenaSize == 0);
>>>>>>> upstream-releases

<<<<<<< HEAD
  MOZ_ASSERT(OffsetFromAligned(p, pageSize) == 0);
||||||| merged common ancestors
    MOZ_ASSERT(OffsetFromAligned(p, pageSize) == 0);
=======
  MOZ_MAKE_MEM_UNDEFINED(region, length);

  if (!DecommitEnabled()) {
    return;
  }
  // We can't commit part of a page.
  MOZ_RELEASE_ASSERT(OffsetFromAligned(region, pageSize) == 0);
  MOZ_RELEASE_ASSERT(length % pageSize == 0);
>>>>>>> upstream-releases
}

<<<<<<< HEAD
size_t GetPageFaultCount() {
  if (mozilla::recordreplay::IsRecordingOrReplaying()) {
    return 0;
  }
  struct rusage usage;
  int err = getrusage(RUSAGE_SELF, &usage);
  if (err) {
    return 0;
  }
  return usage.ru_majflt;
||||||| merged common ancestors
size_t
GetPageFaultCount()
{
    if (mozilla::recordreplay::IsRecordingOrReplaying()) {
        return 0;
    }
    struct rusage usage;
    int err = getrusage(RUSAGE_SELF, &usage);
    if (err) {
        return 0;
    }
    return usage.ru_majflt;
=======
size_t GetPageFaultCount() {
  if (mozilla::recordreplay::IsRecordingOrReplaying()) {
    return 0;
  }
#ifdef XP_WIN
  PROCESS_MEMORY_COUNTERS pmc;
  if (GetProcessMemoryInfo(GetCurrentProcess(), &pmc, sizeof(pmc)) == 0) {
    return 0;
  }
  return pmc.PageFaultCount;
#else
  struct rusage usage;
  int err = getrusage(RUSAGE_SELF, &usage);
  if (err) {
    return 0;
  }
  return usage.ru_majflt;
#endif
>>>>>>> upstream-releases
}

<<<<<<< HEAD
void* AllocateMappedContent(int fd, size_t offset, size_t length,
                            size_t alignment) {
  MOZ_ASSERT(length && alignment);
||||||| merged common ancestors
void*
AllocateMappedContent(int fd, size_t offset, size_t length, size_t alignment)
{
    MOZ_ASSERT(length && alignment);
=======
void* AllocateMappedContent(int fd, size_t offset, size_t length,
                            size_t alignment) {
  if (length == 0 || alignment == 0 || offset % alignment != 0 ||
      std::max(alignment, allocGranularity) %
              std::min(alignment, allocGranularity) !=
          0) {
    return nullptr;
  }
>>>>>>> upstream-releases

<<<<<<< HEAD
  // The allocation granularity and the requested offset
  // must both be divisible by the requested alignment.
  // Alignments larger than the allocation granularity are not supported.
  if (allocGranularity % alignment != 0 || offset % alignment != 0) {
    return nullptr;
  }
||||||| merged common ancestors
    // The allocation granularity and the requested offset
    // must both be divisible by the requested alignment.
    // Alignments larger than the allocation granularity are not supported.
    if (allocGranularity % alignment != 0 || offset % alignment != 0) {
        return nullptr;
    }
=======
  size_t alignedOffset = offset - (offset % allocGranularity);
  size_t alignedLength = length + (offset % allocGranularity);

  // We preallocate the mapping using MapAlignedPages, which expects
  // the length parameter to be an integer multiple of the page size.
  size_t mappedLength = alignedLength;
  if (alignedLength % pageSize != 0) {
    mappedLength += pageSize - alignedLength % pageSize;
  }

#ifdef XP_WIN
  HANDLE hFile = reinterpret_cast<HANDLE>(intptr_t(fd));

  // This call will fail if the file does not exist.
  HANDLE hMap = CreateFileMapping(hFile, nullptr, PAGE_READONLY, 0, 0, nullptr);
  if (!hMap) {
    return nullptr;
  }

  DWORD offsetH = uint32_t(uint64_t(alignedOffset) >> 32);
  DWORD offsetL = uint32_t(alignedOffset);

  uint8_t* map = nullptr;
  for (;;) {
    // The value of a pointer is technically only defined while the region
    // it points to is allocated, so explicitly treat this one as a number.
    uintptr_t region = uintptr_t(MapAlignedPages(mappedLength, alignment));
    if (region == 0) {
      break;
    }
    UnmapInternal(reinterpret_cast<void*>(region), mappedLength);
    // If the offset or length are out of bounds, this call will fail.
    map = static_cast<uint8_t*>(
        MapViewOfFileEx(hMap, FILE_MAP_COPY, offsetH, offsetL, alignedLength,
                        reinterpret_cast<void*>(region)));
>>>>>>> upstream-releases

<<<<<<< HEAD
  // Sanity check the offset and size, as mmap does not do this for us.
  struct stat st;
  if (fstat(fd, &st) || offset >= uint64_t(st.st_size) ||
      length > uint64_t(st.st_size) - offset) {
    return nullptr;
  }
||||||| merged common ancestors
    // Sanity check the offset and size, as mmap does not do this for us.
    struct stat st;
    if (fstat(fd, &st) || offset >= uint64_t(st.st_size) || length > uint64_t(st.st_size) - offset) {
        return nullptr;
    }
=======
    // Retry if another thread mapped the address we were trying to use.
    if (map || GetLastError() != ERROR_INVALID_ADDRESS) {
      break;
    }
  }
>>>>>>> upstream-releases

<<<<<<< HEAD
  size_t alignedOffset = offset - (offset % allocGranularity);
  size_t alignedLength = length + (offset % allocGranularity);
||||||| merged common ancestors
    size_t alignedOffset = offset - (offset % allocGranularity);
    size_t alignedLength = length + (offset % allocGranularity);
=======
  // This just decreases the file mapping object's internal reference count;
  // it won't actually be destroyed until we unmap the associated view.
  CloseHandle(hMap);
>>>>>>> upstream-releases

<<<<<<< HEAD
  uint8_t* map = static_cast<uint8_t*>(MapMemory(
      alignedLength, PROT_READ | PROT_WRITE, MAP_PRIVATE, fd, alignedOffset));
  if (!map) {
    return nullptr;
  }
||||||| merged common ancestors
    uint8_t* map = static_cast<uint8_t*>(MapMemory(alignedLength, PROT_READ | PROT_WRITE,
                                                   MAP_PRIVATE, fd, alignedOffset));
    if (!map) {
        return nullptr;
    }
=======
  if (!map) {
    return nullptr;
  }
#else  // !defined(XP_WIN)
  // Sanity check the offset and length, as mmap does not do this for us.
  struct stat st;
  if (fstat(fd, &st) || offset >= uint64_t(st.st_size) ||
      length > uint64_t(st.st_size) - offset) {
    return nullptr;
  }

  void* region = MapAlignedPages(mappedLength, alignment);
  if (!region) {
    return nullptr;
  }

  // Calling mmap with MAP_FIXED will replace the previous mapping, allowing
  // us to reuse the region we obtained without racing with other threads.
  uint8_t* map =
      static_cast<uint8_t*>(mmap(region, alignedLength, PROT_READ | PROT_WRITE,
                                 MAP_PRIVATE | MAP_FIXED, fd, alignedOffset));
  if (map == MAP_FAILED) {
    UnmapInternal(region, mappedLength);
    return nullptr;
  }
#endif
>>>>>>> upstream-releases

#ifdef DEBUG
  // Zero out data before and after the desired mapping to catch errors early.
  if (offset != alignedOffset) {
    memset(map, 0, offset - alignedOffset);
  }
  if (alignedLength % pageSize) {
    memset(map + alignedLength, 0, pageSize - (alignedLength % pageSize));
  }
#endif

  return map + (offset - alignedOffset);
}

<<<<<<< HEAD
void DeallocateMappedContent(void* p, size_t length) {
  if (!p) {
    return;
  }
||||||| merged common ancestors
void
DeallocateMappedContent(void* p, size_t length)
{
    if (!p) {
        return;
    }
=======
void DeallocateMappedContent(void* region, size_t length) {
  if (!region) {
    return;
  }
>>>>>>> upstream-releases

<<<<<<< HEAD
  // Calculate the address originally returned by mmap.
  // This is needed because AllocateMappedContent returns a pointer
  // that might be offset from the mapping, as the beginning of a
  // mapping must be aligned with the allocation granularity.
  uintptr_t map = uintptr_t(p) - (uintptr_t(p) % allocGranularity);
  size_t alignedLength = length + (uintptr_t(p) % allocGranularity);
  UnmapPages(reinterpret_cast<void*>(map), alignedLength);
}
||||||| merged common ancestors
    // Calculate the address originally returned by mmap.
    // This is needed because AllocateMappedContent returns a pointer
    // that might be offset from the mapping, as the beginning of a
    // mapping must be aligned with the allocation granularity.
    uintptr_t map = uintptr_t(p) - (uintptr_t(p) % allocGranularity);
    size_t alignedLength = length + (uintptr_t(p) % allocGranularity);
    UnmapPages(reinterpret_cast<void*>(map), alignedLength);
}
=======
  // Due to bug 1502562, the following assertion does not currently hold.
  // MOZ_RELEASE_ASSERT(length > 0);
>>>>>>> upstream-releases

  // Calculate the address originally returned by the system call.
  // This is needed because AllocateMappedContent returns a pointer
  // that might be offset from the mapping, as the beginning of a
  // mapping must be aligned with the allocation granularity.
  uintptr_t map = uintptr_t(region) - (uintptr_t(region) % allocGranularity);
#ifdef XP_WIN
  MOZ_RELEASE_ASSERT(UnmapViewOfFile(reinterpret_cast<void*>(map)) != 0);
#else
  size_t alignedLength = length + (uintptr_t(region) % allocGranularity);
  if (munmap(reinterpret_cast<void*>(map), alignedLength)) {
    MOZ_RELEASE_ASSERT(errno == ENOMEM);
  }
#endif
}

<<<<<<< HEAD
void ProtectPages(void* p, size_t size) {
  MOZ_ASSERT(size % pageSize == 0);
  MOZ_RELEASE_ASSERT(size > 0);
  MOZ_RELEASE_ASSERT(p);
#if defined(XP_WIN)
  DWORD oldProtect;
  if (!VirtualProtect(p, size, PAGE_NOACCESS, &oldProtect)) {
    MOZ_CRASH_UNSAFE_PRINTF(
        "VirtualProtect(PAGE_NOACCESS) failed! Error code: %lu",
        GetLastError());
  }
#else  // assume Unix
  if (mprotect(p, size, PROT_NONE)) {
    MOZ_CRASH("mprotect(PROT_NONE) failed");
  }
||||||| merged common ancestors
void
ProtectPages(void* p, size_t size)
{
    MOZ_ASSERT(size % pageSize == 0);
    MOZ_RELEASE_ASSERT(size > 0);
    MOZ_RELEASE_ASSERT(p);
#if defined(XP_WIN)
    DWORD oldProtect;
    if (!VirtualProtect(p, size, PAGE_NOACCESS, &oldProtect)) {
        MOZ_CRASH_UNSAFE_PRINTF("VirtualProtect(PAGE_NOACCESS) failed! Error code: %lu",
                                GetLastError());
    }
#else  // assume Unix
    if (mprotect(p, size, PROT_NONE)) {
        MOZ_CRASH("mprotect(PROT_NONE) failed");
    }
=======
static inline void ProtectMemory(void* region, size_t length, PageAccess prot) {
  MOZ_RELEASE_ASSERT(region && OffsetFromAligned(region, pageSize) == 0);
  MOZ_RELEASE_ASSERT(length > 0 && length % pageSize == 0);
#ifdef XP_WIN
  DWORD oldProtect;
  MOZ_RELEASE_ASSERT(VirtualProtect(region, length, DWORD(prot), &oldProtect) !=
                     0);
#else
  MOZ_RELEASE_ASSERT(mprotect(region, length, int(prot)) == 0);
>>>>>>> upstream-releases
#endif
}

<<<<<<< HEAD
void MakePagesReadOnly(void* p, size_t size) {
  MOZ_ASSERT(size % pageSize == 0);
  MOZ_RELEASE_ASSERT(size > 0);
  MOZ_RELEASE_ASSERT(p);
#if defined(XP_WIN)
  DWORD oldProtect;
  if (!VirtualProtect(p, size, PAGE_READONLY, &oldProtect)) {
    MOZ_CRASH_UNSAFE_PRINTF(
        "VirtualProtect(PAGE_READONLY) failed! Error code: %lu",
        GetLastError());
  }
#else  // assume Unix
  if (mprotect(p, size, PROT_READ)) {
    MOZ_CRASH("mprotect(PROT_READ) failed");
  }
#endif
||||||| merged common ancestors
void
MakePagesReadOnly(void* p, size_t size)
{
    MOZ_ASSERT(size % pageSize == 0);
    MOZ_RELEASE_ASSERT(size > 0);
    MOZ_RELEASE_ASSERT(p);
#if defined(XP_WIN)
    DWORD oldProtect;
    if (!VirtualProtect(p, size, PAGE_READONLY, &oldProtect)) {
        MOZ_CRASH_UNSAFE_PRINTF("VirtualProtect(PAGE_READONLY) failed! Error code: %lu",
                                GetLastError());
    }
#else  // assume Unix
    if (mprotect(p, size, PROT_READ)) {
        MOZ_CRASH("mprotect(PROT_READ) failed");
    }
#endif
=======
void ProtectPages(void* region, size_t length) {
  ProtectMemory(region, length, PageAccess::None);
>>>>>>> upstream-releases
}

<<<<<<< HEAD
void UnprotectPages(void* p, size_t size) {
  MOZ_ASSERT(size % pageSize == 0);
  MOZ_RELEASE_ASSERT(size > 0);
  MOZ_RELEASE_ASSERT(p);
#if defined(XP_WIN)
  DWORD oldProtect;
  if (!VirtualProtect(p, size, PAGE_READWRITE, &oldProtect)) {
    MOZ_CRASH_UNSAFE_PRINTF(
        "VirtualProtect(PAGE_READWRITE) failed! Error code: %lu",
        GetLastError());
  }
#else  // assume Unix
  if (mprotect(p, size, PROT_READ | PROT_WRITE)) {
    MOZ_CRASH("mprotect(PROT_READ | PROT_WRITE) failed");
  }
#endif
||||||| merged common ancestors
void
UnprotectPages(void* p, size_t size)
{
    MOZ_ASSERT(size % pageSize == 0);
    MOZ_RELEASE_ASSERT(size > 0);
    MOZ_RELEASE_ASSERT(p);
#if defined(XP_WIN)
    DWORD oldProtect;
    if (!VirtualProtect(p, size, PAGE_READWRITE, &oldProtect)) {
        MOZ_CRASH_UNSAFE_PRINTF("VirtualProtect(PAGE_READWRITE) failed! Error code: %lu",
                                GetLastError());
    }
#else  // assume Unix
    if (mprotect(p, size, PROT_READ | PROT_WRITE)) {
        MOZ_CRASH("mprotect(PROT_READ | PROT_WRITE) failed");
    }
#endif
=======
void MakePagesReadOnly(void* region, size_t length) {
  ProtectMemory(region, length, PageAccess::Read);
}

void UnprotectPages(void* region, size_t length) {
  ProtectMemory(region, length, PageAccess::ReadWrite);
>>>>>>> upstream-releases
}

}  // namespace gc
}  // namespace js
