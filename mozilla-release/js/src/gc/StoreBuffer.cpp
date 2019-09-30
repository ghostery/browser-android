/* -*- Mode: C++; tab-width: 8; indent-tabs-mode: nil; c-basic-offset: 2 -*-
 * vim: set ts=8 sts=2 et sw=2 tw=80:
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/. */

#include "gc/StoreBuffer-inl.h"

#include "mozilla/Assertions.h"

#include "gc/Statistics.h"
#include "vm/ArgumentsObject.h"
#include "vm/Runtime.h"

#include "gc/GC-inl.h"

using namespace js;
using namespace js::gc;

<<<<<<< HEAD
void StoreBuffer::GenericBuffer::trace(StoreBuffer* owner, JSTracer* trc) {
  mozilla::ReentrancyGuard g(*owner);
  MOZ_ASSERT(owner->isEnabled());
  if (!storage_) {
    return;
  }

  for (LifoAlloc::Enum e(*storage_); !e.empty();) {
    unsigned size = *e.read<unsigned>();
    BufferableRef* edge = e.read<BufferableRef>(size);
    edge->trace(trc);
  }
||||||| merged common ancestors
void
StoreBuffer::GenericBuffer::trace(StoreBuffer* owner, JSTracer* trc)
{
    mozilla::ReentrancyGuard g(*owner);
    MOZ_ASSERT(owner->isEnabled());
    if (!storage_) {
        return;
    }

    for (LifoAlloc::Enum e(*storage_); !e.empty();) {
        unsigned size = *e.read<unsigned>();
        BufferableRef* edge = e.read<BufferableRef>(size);
        edge->trace(trc);
    }
=======
bool StoreBuffer::WholeCellBuffer::init() {
  MOZ_ASSERT(!head_);
  if (!storage_) {
    storage_ = MakeUnique<LifoAlloc>(LifoAllocBlockSize);
    // This prevents LifoAlloc::Enum from crashing with a release
    // assertion if we ever allocate one entry larger than
    // LifoAllocBlockSize.
    if (storage_) {
      storage_->disableOversize();
    }
  }
  clear();
  return bool(storage_);
}

bool StoreBuffer::GenericBuffer::init() {
  if (!storage_) {
    storage_ = MakeUnique<LifoAlloc>(LifoAllocBlockSize);
  }
  clear();
  return bool(storage_);
>>>>>>> upstream-releases
}

<<<<<<< HEAD
void StoreBuffer::checkEmpty() const {
  MOZ_ASSERT(bufferVal.isEmpty());
  MOZ_ASSERT(bufferCell.isEmpty());
  MOZ_ASSERT(bufferSlot.isEmpty());
  MOZ_ASSERT(bufferWholeCell.isEmpty());
  MOZ_ASSERT(bufferGeneric.isEmpty());
||||||| merged common ancestors
inline void
StoreBuffer::checkEmpty() const
{
    MOZ_ASSERT(bufferVal.isEmpty());
    MOZ_ASSERT(bufferCell.isEmpty());
    MOZ_ASSERT(bufferSlot.isEmpty());
    MOZ_ASSERT(bufferWholeCell.isEmpty());
    MOZ_ASSERT(bufferGeneric.isEmpty());
=======
void StoreBuffer::GenericBuffer::trace(JSTracer* trc) {
  mozilla::ReentrancyGuard g(*owner_);
  MOZ_ASSERT(owner_->isEnabled());
  if (!storage_) {
    return;
  }

  for (LifoAlloc::Enum e(*storage_); !e.empty();) {
    unsigned size = *e.read<unsigned>();
    BufferableRef* edge = e.read<BufferableRef>(size);
    edge->trace(trc);
  }
>>>>>>> upstream-releases
}

<<<<<<< HEAD
bool StoreBuffer::enable() {
  if (enabled_) {
    return true;
  }

  checkEmpty();
||||||| merged common ancestors
bool
StoreBuffer::enable()
{
    if (enabled_) {
        return true;
    }

    checkEmpty();
=======
StoreBuffer::StoreBuffer(JSRuntime* rt, const Nursery& nursery)
    : bufferVal(this, JS::GCReason::FULL_VALUE_BUFFER),
      bufStrCell(this, JS::GCReason::FULL_CELL_PTR_STR_BUFFER),
      bufObjCell(this, JS::GCReason::FULL_CELL_PTR_OBJ_BUFFER),
      bufferSlot(this, JS::GCReason::FULL_SLOT_BUFFER),
      bufferWholeCell(this),
      bufferGeneric(this),
      cancelIonCompilations_(false),
      runtime_(rt),
      nursery_(nursery),
      aboutToOverflow_(false),
      enabled_(false)
#ifdef DEBUG
      ,
      mEntered(false)
#endif
{
}
>>>>>>> upstream-releases

<<<<<<< HEAD
  if (!bufferWholeCell.init() || !bufferGeneric.init()) {
    return false;
  }
||||||| merged common ancestors
    if (!bufferWholeCell.init() ||
        !bufferGeneric.init())
    {
        return false;
    }
=======
void StoreBuffer::checkEmpty() const {
  MOZ_ASSERT(bufferVal.isEmpty());
  MOZ_ASSERT(bufStrCell.isEmpty());
  MOZ_ASSERT(bufObjCell.isEmpty());
  MOZ_ASSERT(bufferSlot.isEmpty());
  MOZ_ASSERT(bufferWholeCell.isEmpty());
  MOZ_ASSERT(bufferGeneric.isEmpty());
}
>>>>>>> upstream-releases

<<<<<<< HEAD
  enabled_ = true;
  return true;
||||||| merged common ancestors
    enabled_ = true;
    return true;
=======
bool StoreBuffer::enable() {
  if (enabled_) {
    return true;
  }

  checkEmpty();

  if (!bufferWholeCell.init() || !bufferGeneric.init()) {
    return false;
  }

  enabled_ = true;
  return true;
>>>>>>> upstream-releases
}

void StoreBuffer::disable() {
  checkEmpty();

  if (!enabled_) {
    return;
  }

  aboutToOverflow_ = false;

  enabled_ = false;
}

<<<<<<< HEAD
void StoreBuffer::clear() {
  if (!enabled_) {
    return;
  }

  aboutToOverflow_ = false;
  cancelIonCompilations_ = false;

  bufferVal.clear();
  bufferCell.clear();
  bufferSlot.clear();
  bufferWholeCell.clear();
  bufferGeneric.clear();
||||||| merged common ancestors
void
StoreBuffer::clear()
{
    if (!enabled_) {
        return;
    }

    aboutToOverflow_ = false;
    cancelIonCompilations_ = false;

    bufferVal.clear();
    bufferCell.clear();
    bufferSlot.clear();
    bufferWholeCell.clear();
    bufferGeneric.clear();
=======
void StoreBuffer::clear() {
  if (!enabled_) {
    return;
  }

  aboutToOverflow_ = false;
  cancelIonCompilations_ = false;

  bufferVal.clear();
  bufStrCell.clear();
  bufObjCell.clear();
  bufferSlot.clear();
  bufferWholeCell.clear();
  bufferGeneric.clear();
>>>>>>> upstream-releases
}

<<<<<<< HEAD
void StoreBuffer::setAboutToOverflow(JS::gcreason::Reason reason) {
  if (!aboutToOverflow_) {
    aboutToOverflow_ = true;
    runtime_->gc.stats().count(gcstats::COUNT_STOREBUFFER_OVERFLOW);
  }
  nursery_.requestMinorGC(reason);
||||||| merged common ancestors
void
StoreBuffer::setAboutToOverflow(JS::gcreason::Reason reason)
{
    if (!aboutToOverflow_) {
        aboutToOverflow_ = true;
        runtime_->gc.stats().count(gcstats::STAT_STOREBUFFER_OVERFLOW);
    }
    nursery_.requestMinorGC(reason);
=======
void StoreBuffer::setAboutToOverflow(JS::GCReason reason) {
  if (!aboutToOverflow_) {
    aboutToOverflow_ = true;
    runtime_->gc.stats().count(gcstats::COUNT_STOREBUFFER_OVERFLOW);
  }
  nursery_.requestMinorGC(reason);
>>>>>>> upstream-releases
}

<<<<<<< HEAD
void StoreBuffer::addSizeOfExcludingThis(mozilla::MallocSizeOf mallocSizeOf,
                                         JS::GCSizes* sizes) {
  sizes->storeBufferVals += bufferVal.sizeOfExcludingThis(mallocSizeOf);
  sizes->storeBufferCells += bufferCell.sizeOfExcludingThis(mallocSizeOf);
  sizes->storeBufferSlots += bufferSlot.sizeOfExcludingThis(mallocSizeOf);
  sizes->storeBufferWholeCells +=
      bufferWholeCell.sizeOfExcludingThis(mallocSizeOf);
  sizes->storeBufferGenerics += bufferGeneric.sizeOfExcludingThis(mallocSizeOf);
||||||| merged common ancestors
void
StoreBuffer::addSizeOfExcludingThis(mozilla::MallocSizeOf mallocSizeOf, JS::GCSizes
*sizes)
{
    sizes->storeBufferVals       += bufferVal.sizeOfExcludingThis(mallocSizeOf);
    sizes->storeBufferCells      += bufferCell.sizeOfExcludingThis(mallocSizeOf);
    sizes->storeBufferSlots      += bufferSlot.sizeOfExcludingThis(mallocSizeOf);
    sizes->storeBufferWholeCells += bufferWholeCell.sizeOfExcludingThis(mallocSizeOf);
    sizes->storeBufferGenerics   += bufferGeneric.sizeOfExcludingThis(mallocSizeOf);
=======
void StoreBuffer::addSizeOfExcludingThis(mozilla::MallocSizeOf mallocSizeOf,
                                         JS::GCSizes* sizes) {
  sizes->storeBufferVals += bufferVal.sizeOfExcludingThis(mallocSizeOf);
  sizes->storeBufferCells += bufStrCell.sizeOfExcludingThis(mallocSizeOf) +
                             bufObjCell.sizeOfExcludingThis(mallocSizeOf);
  sizes->storeBufferSlots += bufferSlot.sizeOfExcludingThis(mallocSizeOf);
  sizes->storeBufferWholeCells +=
      bufferWholeCell.sizeOfExcludingThis(mallocSizeOf);
  sizes->storeBufferGenerics += bufferGeneric.sizeOfExcludingThis(mallocSizeOf);
>>>>>>> upstream-releases
}

ArenaCellSet ArenaCellSet::Empty;

ArenaCellSet::ArenaCellSet(Arena* arena, ArenaCellSet* next)
    : arena(arena),
      next(next)
#ifdef DEBUG
      ,
      minorGCNumberAtCreation(
          arena->zone->runtimeFromMainThread()->gc.minorGCCount())
#endif
{
  MOZ_ASSERT(arena);
  MOZ_ASSERT(bits.isAllClear());
}

<<<<<<< HEAD
ArenaCellSet* StoreBuffer::WholeCellBuffer::allocateCellSet(Arena* arena) {
  Zone* zone = arena->zone;
  JSRuntime* rt = zone->runtimeFromMainThread();
  if (!rt->gc.nursery().isEnabled()) {
    return nullptr;
  }

  AutoEnterOOMUnsafeRegion oomUnsafe;
  auto cells = storage_->new_<ArenaCellSet>(arena, head_);
  if (!cells) {
    oomUnsafe.crash("Failed to allocate ArenaCellSet");
  }

  arena->bufferedCells() = cells;
  head_ = cells;

  if (isAboutToOverflow()) {
    rt->gc.storeBuffer().setAboutToOverflow(
        JS::gcreason::FULL_WHOLE_CELL_BUFFER);
  }

  return cells;
||||||| merged common ancestors
ArenaCellSet*
StoreBuffer::WholeCellBuffer::allocateCellSet(Arena* arena)
{
    Zone* zone = arena->zone;
    JSRuntime* rt = zone->runtimeFromMainThread();
    if (!rt->gc.nursery().isEnabled()) {
        return nullptr;
    }

    AutoEnterOOMUnsafeRegion oomUnsafe;
    auto cells = storage_->new_<ArenaCellSet>(arena, head_);
    if (!cells) {
        oomUnsafe.crash("Failed to allocate ArenaCellSet");
    }

    arena->bufferedCells() = cells;
    head_ = cells;

    if (isAboutToOverflow()) {
        rt->gc.storeBuffer().setAboutToOverflow(JS::gcreason::FULL_WHOLE_CELL_BUFFER);
    }

    return cells;
=======
ArenaCellSet* StoreBuffer::WholeCellBuffer::allocateCellSet(Arena* arena) {
  Zone* zone = arena->zone;
  JSRuntime* rt = zone->runtimeFromMainThread();
  if (!rt->gc.nursery().isEnabled()) {
    return nullptr;
  }

  AutoEnterOOMUnsafeRegion oomUnsafe;
  auto cells = storage_->new_<ArenaCellSet>(arena, head_);
  if (!cells) {
    oomUnsafe.crash("Failed to allocate ArenaCellSet");
  }

  arena->bufferedCells() = cells;
  head_ = cells;

  if (isAboutToOverflow()) {
    rt->gc.storeBuffer().setAboutToOverflow(
        JS::GCReason::FULL_WHOLE_CELL_BUFFER);
  }

  return cells;
>>>>>>> upstream-releases
}

<<<<<<< HEAD
void StoreBuffer::WholeCellBuffer::clear() {
  for (ArenaCellSet* set = head_; set; set = set->next) {
    set->arena->bufferedCells() = &ArenaCellSet::Empty;
  }
  head_ = nullptr;

  if (storage_) {
    storage_->used() ? storage_->releaseAll() : storage_->freeAll();
  }
||||||| merged common ancestors
void
StoreBuffer::WholeCellBuffer::clear()
{
    for (ArenaCellSet* set = head_; set; set = set->next) {
        set->arena->bufferedCells() = &ArenaCellSet::Empty;
    }
    head_ = nullptr;

    if (storage_) {
        storage_->used() ? storage_->releaseAll() : storage_->freeAll();
    }
=======
void StoreBuffer::WholeCellBuffer::clear() {
  for (ArenaCellSet* set = head_; set; set = set->next) {
    set->arena->bufferedCells() = &ArenaCellSet::Empty;
  }
  head_ = nullptr;

  if (storage_) {
    storage_->used() ? storage_->releaseAll() : storage_->freeAll();
  }
>>>>>>> upstream-releases
}

template struct StoreBuffer::MonoTypeBuffer<StoreBuffer::ValueEdge>;
template struct StoreBuffer::MonoTypeBuffer<StoreBuffer::CellPtrEdge>;
template struct StoreBuffer::MonoTypeBuffer<StoreBuffer::SlotsEdge>;
