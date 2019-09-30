/* -*- Mode: C++; tab-width: 8; indent-tabs-mode: nil; c-basic-offset: 2 -*-
 * vim: set ts=8 sts=2 et sw=2 tw=80:
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/. */

#ifndef gc_StoreBuffer_h
#define gc_StoreBuffer_h

#include "mozilla/Attributes.h"
#include "mozilla/HashFunctions.h"
#include "mozilla/ReentrancyGuard.h"

#include <algorithm>

#include "ds/BitArray.h"
#include "ds/LifoAlloc.h"
#include "gc/Nursery.h"
#include "js/AllocPolicy.h"
#include "js/MemoryMetrics.h"
#include "js/UniquePtr.h"

namespace js {
namespace gc {

class Arena;
class ArenaCellSet;

/*
 * BufferableRef represents an abstract reference for use in the generational
 * GC's remembered set. Entries in the store buffer that cannot be represented
 * with the simple pointer-to-a-pointer scheme must derive from this class and
 * use the generic store buffer interface.
 *
 * A single BufferableRef entry in the generic buffer can represent many entries
 * in the remembered set.  For example js::OrderedHashTableRef represents all
 * the incoming edges corresponding to keys in an ordered hash table.
 */
class BufferableRef {
 public:
  virtual void trace(JSTracer* trc) = 0;
  bool maybeInRememberedSet(const Nursery&) const { return true; }
};

typedef HashSet<void*, PointerHasher<void*>, SystemAllocPolicy> EdgeSet;

/* The size of a single block of store buffer storage space. */
static const size_t LifoAllocBlockSize = 8 * 1024;

/*
 * The StoreBuffer observes all writes that occur in the system and performs
 * efficient filtering of them to derive a remembered set for nursery GC.
 */
class StoreBuffer {
  friend class mozilla::ReentrancyGuard;

  /* The size at which a block is about to overflow for the generic buffer. */
  static const size_t GenericBufferLowAvailableThreshold =
      LifoAllocBlockSize / 2;

  /* The size at which the whole cell buffer is about to overflow. */
  static const size_t WholeCellBufferOverflowThresholdBytes = 128 * 1024;

  /*
   * This buffer holds only a single type of edge. Using this buffer is more
   * efficient than the generic buffer when many writes will be to the same
   * type of edge: e.g. Value or Cell*.
   */
  template <typename T>
  struct MonoTypeBuffer {
    /* The canonical set of stores. */
    typedef HashSet<T, typename T::Hasher, SystemAllocPolicy> StoreSet;
    StoreSet stores_;

    /*
     * A one element cache in front of the canonical set to speed up
     * temporary instances of HeapPtr.
     */
    T last_;

<<<<<<< HEAD
    /* Maximum number of entries before we request a minor GC. */
    const static size_t MaxEntries = 48 * 1024 / sizeof(T);
||||||| merged common ancestors
        /* Add one item to the buffer. */
        void put(StoreBuffer* owner, const T& t) {
            sinkStore(owner);
            last_ = t;
        }
=======
    StoreBuffer* owner_;
>>>>>>> upstream-releases

<<<<<<< HEAD
    explicit MonoTypeBuffer() : last_(T()) {}
||||||| merged common ancestors
        /* Remove an item from the store buffer. */
        void unput(StoreBuffer* owner, const T& v) {
            // Fast, hashless remove of last put.
            if (last_ == v) {
                last_ = T();
                return;
            }
            stores_.remove(v);
        }
=======
    JS::GCReason gcReason_;
>>>>>>> upstream-releases

<<<<<<< HEAD
    void clear() {
      last_ = T();
      stores_.clear();
    }
||||||| merged common ancestors
        /* Move any buffered stores to the canonical store set. */
        void sinkStore(StoreBuffer* owner) {
            if (last_) {
                AutoEnterOOMUnsafeRegion oomUnsafe;
                if (!stores_.put(last_)) {
                    oomUnsafe.crash("Failed to allocate for MonoTypeBuffer::put.");
                }
            }
            last_ = T();

            if (MOZ_UNLIKELY(stores_.count() > MaxEntries)) {
                owner->setAboutToOverflow(T::FullBufferReason);
            }
        }
=======
    /* Maximum number of entries before we request a minor GC. */
    const static size_t MaxEntries = 48 * 1024 / sizeof(T);
>>>>>>> upstream-releases

<<<<<<< HEAD
    /* Add one item to the buffer. */
    void put(StoreBuffer* owner, const T& t) {
      sinkStore(owner);
      last_ = t;
    }

    /* Remove an item from the store buffer. */
    void unput(StoreBuffer* owner, const T& v) {
      // Fast, hashless remove of last put.
      if (last_ == v) {
        last_ = T();
        return;
      }
      stores_.remove(v);
    }
||||||| merged common ancestors
        bool has(StoreBuffer* owner, const T& v) {
            sinkStore(owner);
            return stores_.has(v);
        }

        /* Trace the source of all edges in the store buffer. */
        void trace(StoreBuffer* owner, TenuringTracer& mover);
=======
    explicit MonoTypeBuffer(StoreBuffer* owner, JS::GCReason reason)
        : last_(T()), owner_(owner), gcReason_(reason) {}
>>>>>>> upstream-releases

<<<<<<< HEAD
    /* Move any buffered stores to the canonical store set. */
    void sinkStore(StoreBuffer* owner) {
      if (last_) {
        AutoEnterOOMUnsafeRegion oomUnsafe;
        if (!stores_.put(last_)) {
          oomUnsafe.crash("Failed to allocate for MonoTypeBuffer::put.");
        }
      }
      last_ = T();

      if (MOZ_UNLIKELY(stores_.count() > MaxEntries)) {
        owner->setAboutToOverflow(T::FullBufferReason);
      }
    }

    bool has(StoreBuffer* owner, const T& v) {
      sinkStore(owner);
      return stores_.has(v);
    }

    /* Trace the source of all edges in the store buffer. */
    void trace(StoreBuffer* owner, TenuringTracer& mover);

    size_t sizeOfExcludingThis(mozilla::MallocSizeOf mallocSizeOf) {
      return stores_.shallowSizeOfExcludingThis(mallocSizeOf);
    }

    bool isEmpty() const { return last_ == T() && stores_.empty(); }
||||||| merged common ancestors
        size_t sizeOfExcludingThis(mozilla::MallocSizeOf mallocSizeOf) {
            return stores_.shallowSizeOfExcludingThis(mallocSizeOf);
        }

        bool isEmpty() const {
            return last_ == T() && stores_.empty();
        }

      private:
        MonoTypeBuffer(const MonoTypeBuffer& other) = delete;
        MonoTypeBuffer& operator=(const MonoTypeBuffer& other) = delete;
    };

    struct WholeCellBuffer
    {
        LifoAlloc* storage_;
        ArenaCellSet* head_;

        WholeCellBuffer() : storage_(nullptr), head_(nullptr) {}
        ~WholeCellBuffer() { js_delete(storage_); }

        MOZ_MUST_USE bool init() {
            MOZ_ASSERT(!head_);
            if (!storage_) {
                storage_ = js_new<LifoAlloc>(LifoAllocBlockSize);
            }
            clear();
            return bool(storage_);
        }

        void clear();

        bool isAboutToOverflow() const {
            return !storage_->isEmpty() && storage_->used() > WholeCellBufferOverflowThresholdBytes;
        }

        void trace(StoreBuffer* owner, TenuringTracer& mover);
=======
    void clear() {
      last_ = T();
      stores_.clear();
    }
>>>>>>> upstream-releases

<<<<<<< HEAD
   private:
    MonoTypeBuffer(const MonoTypeBuffer& other) = delete;
    MonoTypeBuffer& operator=(const MonoTypeBuffer& other) = delete;
  };
||||||| merged common ancestors
        inline void put(const Cell* cell);
=======
    /* Add one item to the buffer. */
    void put(const T& t) {
      sinkStore();
      last_ = t;
    }
>>>>>>> upstream-releases

<<<<<<< HEAD
  struct WholeCellBuffer {
    LifoAlloc* storage_;
    ArenaCellSet* head_;
||||||| merged common ancestors
        size_t sizeOfExcludingThis(mozilla::MallocSizeOf mallocSizeOf) {
            return storage_ ? storage_->sizeOfIncludingThis(mallocSizeOf) : 0;
        }
=======
    /* Remove an item from the store buffer. */
    void unput(const T& v) {
      // Fast, hashless remove of last put.
      if (last_ == v) {
        last_ = T();
        return;
      }
      stores_.remove(v);
    }
>>>>>>> upstream-releases

<<<<<<< HEAD
    WholeCellBuffer() : storage_(nullptr), head_(nullptr) {}
    ~WholeCellBuffer() { js_delete(storage_); }
||||||| merged common ancestors
        bool isEmpty() const {
            MOZ_ASSERT_IF(!head_, !storage_ || storage_->isEmpty());
            return !head_;
        }
=======
    /* Move any buffered stores to the canonical store set. */
    void sinkStore() {
      if (last_) {
        AutoEnterOOMUnsafeRegion oomUnsafe;
        if (!stores_.put(last_)) {
          oomUnsafe.crash("Failed to allocate for MonoTypeBuffer::put.");
        }
      }
      last_ = T();
>>>>>>> upstream-releases

<<<<<<< HEAD
    MOZ_MUST_USE bool init() {
      MOZ_ASSERT(!head_);
      if (!storage_) {
        storage_ = js_new<LifoAlloc>(LifoAllocBlockSize);
      }
      clear();
      return bool(storage_);
    }

    void clear();
||||||| merged common ancestors
      private:
        ArenaCellSet* allocateCellSet(Arena* arena);

        WholeCellBuffer(const WholeCellBuffer& other) = delete;
        WholeCellBuffer& operator=(const WholeCellBuffer& other) = delete;
    };
=======
      if (MOZ_UNLIKELY(stores_.count() > MaxEntries)) {
        owner_->setAboutToOverflow(gcReason_);
      }
    }
>>>>>>> upstream-releases

<<<<<<< HEAD
    bool isAboutToOverflow() const {
      return !storage_->isEmpty() &&
             storage_->used() > WholeCellBufferOverflowThresholdBytes;
    }
||||||| merged common ancestors
    struct GenericBuffer
    {
=======
    /* Trace the source of all edges in the store buffer. */
    void trace(TenuringTracer& mover);
>>>>>>> upstream-releases

<<<<<<< HEAD
    void trace(StoreBuffer* owner, TenuringTracer& mover);
||||||| merged common ancestors
        LifoAlloc* storage_;
=======
    template <typename CellType>
    void traceTyped(TenuringTracer& mover);
>>>>>>> upstream-releases

<<<<<<< HEAD
    inline void put(const Cell* cell);
||||||| merged common ancestors
        explicit GenericBuffer() : storage_(nullptr) {}
        ~GenericBuffer() { js_delete(storage_); }
=======
    size_t sizeOfExcludingThis(mozilla::MallocSizeOf mallocSizeOf) {
      return stores_.shallowSizeOfExcludingThis(mallocSizeOf);
    }
>>>>>>> upstream-releases

<<<<<<< HEAD
    size_t sizeOfExcludingThis(mozilla::MallocSizeOf mallocSizeOf) {
      return storage_ ? storage_->sizeOfIncludingThis(mallocSizeOf) : 0;
    }
||||||| merged common ancestors
        MOZ_MUST_USE bool init() {
            if (!storage_) {
                storage_ = js_new<LifoAlloc>(LifoAllocBlockSize);
            }
            clear();
            return bool(storage_);
        }
=======
    bool isEmpty() const { return last_ == T() && stores_.empty(); }
>>>>>>> upstream-releases

<<<<<<< HEAD
    bool isEmpty() const {
      MOZ_ASSERT_IF(!head_, !storage_ || storage_->isEmpty());
      return !head_;
    }
||||||| merged common ancestors
        void clear() {
            if (!storage_) {
                return;
            }
=======
   private:
    MonoTypeBuffer(const MonoTypeBuffer& other) = delete;
    MonoTypeBuffer& operator=(const MonoTypeBuffer& other) = delete;
  };
>>>>>>> upstream-releases

<<<<<<< HEAD
   private:
    ArenaCellSet* allocateCellSet(Arena* arena);
||||||| merged common ancestors
            storage_->used() ? storage_->releaseAll() : storage_->freeAll();
        }
=======
  struct WholeCellBuffer {
    UniquePtr<LifoAlloc> storage_;
    ArenaCellSet* head_;
    StoreBuffer* owner_;
>>>>>>> upstream-releases

<<<<<<< HEAD
    WholeCellBuffer(const WholeCellBuffer& other) = delete;
    WholeCellBuffer& operator=(const WholeCellBuffer& other) = delete;
  };
||||||| merged common ancestors
        bool isAboutToOverflow() const {
            return !storage_->isEmpty() &&
                   storage_->availableInCurrentChunk() < GenericBufferLowAvailableThreshold;
        }
=======
    explicit WholeCellBuffer(StoreBuffer* owner)
        : storage_(nullptr), head_(nullptr), owner_(owner) {}
>>>>>>> upstream-releases

<<<<<<< HEAD
  struct GenericBuffer {
    LifoAlloc* storage_;
||||||| merged common ancestors
        /* Trace all generic edges. */
        void trace(StoreBuffer* owner, JSTracer* trc);
=======
    MOZ_MUST_USE bool init();
>>>>>>> upstream-releases

<<<<<<< HEAD
    explicit GenericBuffer() : storage_(nullptr) {}
    ~GenericBuffer() { js_delete(storage_); }
||||||| merged common ancestors
        template <typename T>
        void put(StoreBuffer* owner, const T& t) {
            MOZ_ASSERT(storage_);
=======
    void clear();
>>>>>>> upstream-releases

<<<<<<< HEAD
    MOZ_MUST_USE bool init() {
      if (!storage_) {
        storage_ = js_new<LifoAlloc>(LifoAllocBlockSize);
      }
      clear();
      return bool(storage_);
    }
||||||| merged common ancestors
            /* Ensure T is derived from BufferableRef. */
            (void)static_cast<const BufferableRef*>(&t);
=======
    bool isAboutToOverflow() const {
      return !storage_->isEmpty() &&
             storage_->used() > WholeCellBufferOverflowThresholdBytes;
    }
>>>>>>> upstream-releases

<<<<<<< HEAD
    void clear() {
      if (!storage_) {
        return;
      }
||||||| merged common ancestors
            AutoEnterOOMUnsafeRegion oomUnsafe;
            unsigned size = sizeof(T);
            unsigned* sizep = storage_->pod_malloc<unsigned>();
            if (!sizep) {
                oomUnsafe.crash("Failed to allocate for GenericBuffer::put.");
            }
            *sizep = size;
=======
    void trace(TenuringTracer& mover);
>>>>>>> upstream-releases

<<<<<<< HEAD
      storage_->used() ? storage_->releaseAll() : storage_->freeAll();
    }
||||||| merged common ancestors
            T* tp = storage_->new_<T>(t);
            if (!tp) {
                oomUnsafe.crash("Failed to allocate for GenericBuffer::put.");
            }
=======
    inline void put(const Cell* cell);
>>>>>>> upstream-releases

<<<<<<< HEAD
    bool isAboutToOverflow() const {
      return !storage_->isEmpty() && storage_->availableInCurrentChunk() <
                                         GenericBufferLowAvailableThreshold;
    }

    /* Trace all generic edges. */
    void trace(StoreBuffer* owner, JSTracer* trc);
||||||| merged common ancestors
            if (isAboutToOverflow()) {
                owner->setAboutToOverflow(JS::gcreason::FULL_GENERIC_BUFFER);
            }
        }

        size_t sizeOfExcludingThis(mozilla::MallocSizeOf mallocSizeOf) {
            return storage_ ? storage_->sizeOfIncludingThis(mallocSizeOf) : 0;
        }
=======
    size_t sizeOfExcludingThis(mozilla::MallocSizeOf mallocSizeOf) {
      return storage_ ? storage_->sizeOfIncludingThis(mallocSizeOf) : 0;
    }
>>>>>>> upstream-releases

<<<<<<< HEAD
    template <typename T>
    void put(StoreBuffer* owner, const T& t) {
      MOZ_ASSERT(storage_);

      /* Ensure T is derived from BufferableRef. */
      (void)static_cast<const BufferableRef*>(&t);

      AutoEnterOOMUnsafeRegion oomUnsafe;
      unsigned size = sizeof(T);
      unsigned* sizep = storage_->pod_malloc<unsigned>();
      if (!sizep) {
        oomUnsafe.crash("Failed to allocate for GenericBuffer::put.");
      }
      *sizep = size;

      T* tp = storage_->new_<T>(t);
      if (!tp) {
        oomUnsafe.crash("Failed to allocate for GenericBuffer::put.");
      }

      if (isAboutToOverflow()) {
        owner->setAboutToOverflow(JS::gcreason::FULL_GENERIC_BUFFER);
      }
    }
||||||| merged common ancestors
        bool isEmpty() const {
            return !storage_ || storage_->isEmpty();
        }
=======
    bool isEmpty() const {
      MOZ_ASSERT_IF(!head_, !storage_ || storage_->isEmpty());
      return !head_;
    }
>>>>>>> upstream-releases

<<<<<<< HEAD
    size_t sizeOfExcludingThis(mozilla::MallocSizeOf mallocSizeOf) {
      return storage_ ? storage_->sizeOfIncludingThis(mallocSizeOf) : 0;
    }
||||||| merged common ancestors
      private:
        GenericBuffer(const GenericBuffer& other) = delete;
        GenericBuffer& operator=(const GenericBuffer& other) = delete;
    };

    template <typename Edge>
    struct PointerEdgeHasher
    {
        typedef Edge Lookup;
        static HashNumber hash(const Lookup& l) { return mozilla::HashGeneric(l.edge); }
        static bool match(const Edge& k, const Lookup& l) { return k == l; }
    };

    struct CellPtrEdge
    {
        Cell** edge;

        CellPtrEdge() : edge(nullptr) {}
        explicit CellPtrEdge(Cell** v) : edge(v) {}
        bool operator==(const CellPtrEdge& other) const { return edge == other.edge; }
        bool operator!=(const CellPtrEdge& other) const { return edge != other.edge; }

        bool maybeInRememberedSet(const Nursery& nursery) const {
            MOZ_ASSERT(IsInsideNursery(*edge));
            return !nursery.isInside(edge);
        }
=======
   private:
    ArenaCellSet* allocateCellSet(Arena* arena);
>>>>>>> upstream-releases

<<<<<<< HEAD
    bool isEmpty() const { return !storage_ || storage_->isEmpty(); }
||||||| merged common ancestors
        void trace(TenuringTracer& mover) const;
=======
    WholeCellBuffer(const WholeCellBuffer& other) = delete;
    WholeCellBuffer& operator=(const WholeCellBuffer& other) = delete;
  };
>>>>>>> upstream-releases

<<<<<<< HEAD
   private:
    GenericBuffer(const GenericBuffer& other) = delete;
    GenericBuffer& operator=(const GenericBuffer& other) = delete;
  };
||||||| merged common ancestors
        CellPtrEdge tagged() const { return CellPtrEdge((Cell**)(uintptr_t(edge) | 1)); }
        CellPtrEdge untagged() const { return CellPtrEdge((Cell**)(uintptr_t(edge) & ~1)); }
        bool isTagged() const { return bool(uintptr_t(edge) & 1); }
=======
  struct GenericBuffer {
    UniquePtr<LifoAlloc> storage_;
    StoreBuffer* owner_;
>>>>>>> upstream-releases

<<<<<<< HEAD
  template <typename Edge>
  struct PointerEdgeHasher {
    typedef Edge Lookup;
    static HashNumber hash(const Lookup& l) {
      return mozilla::HashGeneric(l.edge);
    }
    static bool match(const Edge& k, const Lookup& l) { return k == l; }
  };
||||||| merged common ancestors
        explicit operator bool() const { return edge != nullptr; }
=======
    explicit GenericBuffer(StoreBuffer* owner)
        : storage_(nullptr), owner_(owner) {}
>>>>>>> upstream-releases

<<<<<<< HEAD
  struct CellPtrEdge {
    Cell** edge;
||||||| merged common ancestors
        typedef PointerEdgeHasher<CellPtrEdge> Hasher;
=======
    MOZ_MUST_USE bool init();
>>>>>>> upstream-releases

<<<<<<< HEAD
    CellPtrEdge() : edge(nullptr) {}
    explicit CellPtrEdge(Cell** v) : edge(v) {}
    bool operator==(const CellPtrEdge& other) const {
      return edge == other.edge;
    }
    bool operator!=(const CellPtrEdge& other) const {
      return edge != other.edge;
    }
||||||| merged common ancestors
        static const auto FullBufferReason = JS::gcreason::FULL_CELL_PTR_BUFFER;
    };
=======
    void clear() {
      if (storage_) {
        storage_->used() ? storage_->releaseAll() : storage_->freeAll();
      }
    }
>>>>>>> upstream-releases

<<<<<<< HEAD
    bool maybeInRememberedSet(const Nursery& nursery) const {
      MOZ_ASSERT(IsInsideNursery(*edge));
      return !nursery.isInside(edge);
    }
||||||| merged common ancestors
    struct ValueEdge
    {
        JS::Value* edge;
=======
    bool isAboutToOverflow() const {
      return !storage_->isEmpty() && storage_->availableInCurrentChunk() <
                                         GenericBufferLowAvailableThreshold;
    }
>>>>>>> upstream-releases

<<<<<<< HEAD
    void trace(TenuringTracer& mover) const;
||||||| merged common ancestors
        ValueEdge() : edge(nullptr) {}
        explicit ValueEdge(JS::Value* v) : edge(v) {}
        bool operator==(const ValueEdge& other) const { return edge == other.edge; }
        bool operator!=(const ValueEdge& other) const { return edge != other.edge; }
=======
    /* Trace all generic edges. */
    void trace(JSTracer* trc);
>>>>>>> upstream-releases

<<<<<<< HEAD
    CellPtrEdge tagged() const {
      return CellPtrEdge((Cell**)(uintptr_t(edge) | 1));
    }
    CellPtrEdge untagged() const {
      return CellPtrEdge((Cell**)(uintptr_t(edge) & ~1));
    }
    bool isTagged() const { return bool(uintptr_t(edge) & 1); }
||||||| merged common ancestors
        Cell* deref() const { return edge->isGCThing() ? static_cast<Cell*>(edge->toGCThing()) : nullptr; }
=======
    template <typename T>
    void put(const T& t) {
      MOZ_ASSERT(storage_);

      /* Ensure T is derived from BufferableRef. */
      (void)static_cast<const BufferableRef*>(&t);

      AutoEnterOOMUnsafeRegion oomUnsafe;
      unsigned size = sizeof(T);
      unsigned* sizep = storage_->pod_malloc<unsigned>();
      if (!sizep) {
        oomUnsafe.crash("Failed to allocate for GenericBuffer::put.");
      }
      *sizep = size;

      T* tp = storage_->new_<T>(t);
      if (!tp) {
        oomUnsafe.crash("Failed to allocate for GenericBuffer::put.");
      }

      if (isAboutToOverflow()) {
        owner_->setAboutToOverflow(JS::GCReason::FULL_GENERIC_BUFFER);
      }
    }
>>>>>>> upstream-releases

<<<<<<< HEAD
    explicit operator bool() const { return edge != nullptr; }
||||||| merged common ancestors
        bool maybeInRememberedSet(const Nursery& nursery) const {
            MOZ_ASSERT(IsInsideNursery(deref()));
            return !nursery.isInside(edge);
        }
=======
    size_t sizeOfExcludingThis(mozilla::MallocSizeOf mallocSizeOf) {
      return storage_ ? storage_->sizeOfIncludingThis(mallocSizeOf) : 0;
    }
>>>>>>> upstream-releases

<<<<<<< HEAD
    typedef PointerEdgeHasher<CellPtrEdge> Hasher;
||||||| merged common ancestors
        void trace(TenuringTracer& mover) const;
=======
    bool isEmpty() const { return !storage_ || storage_->isEmpty(); }
>>>>>>> upstream-releases

<<<<<<< HEAD
    static const auto FullBufferReason = JS::gcreason::FULL_CELL_PTR_BUFFER;
  };
||||||| merged common ancestors
        ValueEdge tagged() const { return ValueEdge((JS::Value*)(uintptr_t(edge) | 1)); }
        ValueEdge untagged() const { return ValueEdge((JS::Value*)(uintptr_t(edge) & ~1)); }
        bool isTagged() const { return bool(uintptr_t(edge) & 1); }
=======
   private:
    GenericBuffer(const GenericBuffer& other) = delete;
    GenericBuffer& operator=(const GenericBuffer& other) = delete;
  };
>>>>>>> upstream-releases

<<<<<<< HEAD
  struct ValueEdge {
    JS::Value* edge;
||||||| merged common ancestors
        explicit operator bool() const { return edge != nullptr; }
=======
  template <typename Edge>
  struct PointerEdgeHasher {
    typedef Edge Lookup;
    static HashNumber hash(const Lookup& l) {
      return mozilla::HashGeneric(l.edge);
    }
    static bool match(const Edge& k, const Lookup& l) { return k == l; }
  };

  struct CellPtrEdge {
    Cell** edge;

    CellPtrEdge() : edge(nullptr) {}
    explicit CellPtrEdge(Cell** v) : edge(v) {}
    explicit CellPtrEdge(JSString** v) : edge(reinterpret_cast<Cell**>(v)) {}
    explicit CellPtrEdge(JSObject** v) : edge(reinterpret_cast<Cell**>(v)) {}
    bool operator==(const CellPtrEdge& other) const {
      return edge == other.edge;
    }
    bool operator!=(const CellPtrEdge& other) const {
      return edge != other.edge;
    }
>>>>>>> upstream-releases

<<<<<<< HEAD
    ValueEdge() : edge(nullptr) {}
    explicit ValueEdge(JS::Value* v) : edge(v) {}
    bool operator==(const ValueEdge& other) const { return edge == other.edge; }
    bool operator!=(const ValueEdge& other) const { return edge != other.edge; }
||||||| merged common ancestors
        typedef PointerEdgeHasher<ValueEdge> Hasher;
=======
    bool maybeInRememberedSet(const Nursery& nursery) const {
      MOZ_ASSERT(IsInsideNursery(*edge));
      return !nursery.isInside(edge);
    }
>>>>>>> upstream-releases

<<<<<<< HEAD
    Cell* deref() const {
      return edge->isGCThing() ? static_cast<Cell*>(edge->toGCThing())
                               : nullptr;
    }
||||||| merged common ancestors
        static const auto FullBufferReason = JS::gcreason::FULL_VALUE_BUFFER;
    };
=======
    template <typename CellType>
    void traceTyped(TenuringTracer& mover) const;
>>>>>>> upstream-releases

<<<<<<< HEAD
    bool maybeInRememberedSet(const Nursery& nursery) const {
      MOZ_ASSERT(IsInsideNursery(deref()));
      return !nursery.isInside(edge);
    }
||||||| merged common ancestors
    struct SlotsEdge
    {
        // These definitions must match those in HeapSlot::Kind.
        const static int SlotKind = 0;
        const static int ElementKind = 1;
=======
    CellPtrEdge tagged() const {
      return CellPtrEdge((Cell**)(uintptr_t(edge) | 1));
    }
    CellPtrEdge untagged() const {
      return CellPtrEdge((Cell**)(uintptr_t(edge) & ~1));
    }
    bool isTagged() const { return bool(uintptr_t(edge) & 1); }
>>>>>>> upstream-releases

<<<<<<< HEAD
    void trace(TenuringTracer& mover) const;
||||||| merged common ancestors
        uintptr_t objectAndKind_; // NativeObject* | Kind
        uint32_t start_;
        uint32_t count_;
=======
    explicit operator bool() const { return edge != nullptr; }
>>>>>>> upstream-releases

<<<<<<< HEAD
    ValueEdge tagged() const {
      return ValueEdge((JS::Value*)(uintptr_t(edge) | 1));
    }
    ValueEdge untagged() const {
      return ValueEdge((JS::Value*)(uintptr_t(edge) & ~1));
    }
    bool isTagged() const { return bool(uintptr_t(edge) & 1); }
||||||| merged common ancestors
        SlotsEdge() : objectAndKind_(0), start_(0), count_(0) {}
        SlotsEdge(NativeObject* object, int kind, uint32_t start, uint32_t count)
          : objectAndKind_(uintptr_t(object) | kind), start_(start), count_(count)
        {
            MOZ_ASSERT((uintptr_t(object) & 1) == 0);
            MOZ_ASSERT(kind <= 1);
            MOZ_ASSERT(count > 0);
            MOZ_ASSERT(start + count > start);
        }
=======
    typedef PointerEdgeHasher<CellPtrEdge> Hasher;
  };
>>>>>>> upstream-releases

<<<<<<< HEAD
    explicit operator bool() const { return edge != nullptr; }
||||||| merged common ancestors
        NativeObject* object() const { return reinterpret_cast<NativeObject*>(objectAndKind_ & ~1); }
        int kind() const { return (int)(objectAndKind_ & 1); }
=======
  struct ValueEdge {
    JS::Value* edge;
>>>>>>> upstream-releases

<<<<<<< HEAD
    typedef PointerEdgeHasher<ValueEdge> Hasher;
||||||| merged common ancestors
        bool operator==(const SlotsEdge& other) const {
            return objectAndKind_ == other.objectAndKind_ &&
                   start_ == other.start_ &&
                   count_ == other.count_;
        }
=======
    ValueEdge() : edge(nullptr) {}
    explicit ValueEdge(JS::Value* v) : edge(v) {}
    bool operator==(const ValueEdge& other) const { return edge == other.edge; }
    bool operator!=(const ValueEdge& other) const { return edge != other.edge; }
>>>>>>> upstream-releases

<<<<<<< HEAD
    static const auto FullBufferReason = JS::gcreason::FULL_VALUE_BUFFER;
  };
||||||| merged common ancestors
        bool operator!=(const SlotsEdge& other) const {
            return !(*this == other);
        }
=======
    Cell* deref() const {
      return edge->isGCThing() ? static_cast<Cell*>(edge->toGCThing())
                               : nullptr;
    }
>>>>>>> upstream-releases

<<<<<<< HEAD
  struct SlotsEdge {
    // These definitions must match those in HeapSlot::Kind.
    const static int SlotKind = 0;
    const static int ElementKind = 1;
||||||| merged common ancestors
        // True if this SlotsEdge range overlaps with the other SlotsEdge range,
        // false if they do not overlap.
        bool overlaps(const SlotsEdge& other) const {
            if (objectAndKind_ != other.objectAndKind_) {
                return false;
            }

            // Widen our range by one on each side so that we consider
            // adjacent-but-not-actually-overlapping ranges as overlapping. This
            // is particularly useful for coalescing a series of increasing or
            // decreasing single index writes 0, 1, 2, ..., N into a SlotsEdge
            // range of elements [0, N].
            uint32_t end = start_ + count_ + 1;
            uint32_t start = start_ > 0 ? start_ - 1 : 0;
            MOZ_ASSERT(start < end);

            uint32_t otherEnd = other.start_ + other.count_;
            MOZ_ASSERT(other.start_ <= otherEnd);
            return (start <= other.start_ && other.start_ <= end) ||
                   (start <= otherEnd && otherEnd <= end);
        }
=======
    bool maybeInRememberedSet(const Nursery& nursery) const {
      MOZ_ASSERT(IsInsideNursery(deref()));
      return !nursery.isInside(edge);
    }
>>>>>>> upstream-releases

<<<<<<< HEAD
    uintptr_t objectAndKind_;  // NativeObject* | Kind
    uint32_t start_;
    uint32_t count_;
||||||| merged common ancestors
        // Destructively make this SlotsEdge range the union of the other
        // SlotsEdge range and this one. A precondition is that the ranges must
        // overlap.
        void merge(const SlotsEdge& other) {
            MOZ_ASSERT(overlaps(other));
            uint32_t end = Max(start_ + count_, other.start_ + other.count_);
            start_ = Min(start_, other.start_);
            count_ = end - start_;
        }
=======
    void trace(TenuringTracer& mover) const;
>>>>>>> upstream-releases

<<<<<<< HEAD
    SlotsEdge() : objectAndKind_(0), start_(0), count_(0) {}
    SlotsEdge(NativeObject* object, int kind, uint32_t start, uint32_t count)
        : objectAndKind_(uintptr_t(object) | kind),
          start_(start),
          count_(count) {
      MOZ_ASSERT((uintptr_t(object) & 1) == 0);
      MOZ_ASSERT(kind <= 1);
      MOZ_ASSERT(count > 0);
      MOZ_ASSERT(start + count > start);
    }
||||||| merged common ancestors
        bool maybeInRememberedSet(const Nursery& n) const {
            return !IsInsideNursery(reinterpret_cast<Cell*>(object()));
        }
=======
    ValueEdge tagged() const {
      return ValueEdge((JS::Value*)(uintptr_t(edge) | 1));
    }
    ValueEdge untagged() const {
      return ValueEdge((JS::Value*)(uintptr_t(edge) & ~1));
    }
    bool isTagged() const { return bool(uintptr_t(edge) & 1); }

    explicit operator bool() const { return edge != nullptr; }

    typedef PointerEdgeHasher<ValueEdge> Hasher;
  };

  struct SlotsEdge {
    // These definitions must match those in HeapSlot::Kind.
    const static int SlotKind = 0;
    const static int ElementKind = 1;

    uintptr_t objectAndKind_;  // NativeObject* | Kind
    uint32_t start_;
    uint32_t count_;

    SlotsEdge() : objectAndKind_(0), start_(0), count_(0) {}
    SlotsEdge(NativeObject* object, int kind, uint32_t start, uint32_t count)
        : objectAndKind_(uintptr_t(object) | kind),
          start_(start),
          count_(count) {
      MOZ_ASSERT((uintptr_t(object) & 1) == 0);
      MOZ_ASSERT(kind <= 1);
      MOZ_ASSERT(count > 0);
      MOZ_ASSERT(start + count > start);
    }
>>>>>>> upstream-releases

    NativeObject* object() const {
      return reinterpret_cast<NativeObject*>(objectAndKind_ & ~1);
    }
    int kind() const { return (int)(objectAndKind_ & 1); }

    bool operator==(const SlotsEdge& other) const {
      return objectAndKind_ == other.objectAndKind_ && start_ == other.start_ &&
             count_ == other.count_;
    }

    bool operator!=(const SlotsEdge& other) const { return !(*this == other); }

    // True if this SlotsEdge range overlaps with the other SlotsEdge range,
    // false if they do not overlap.
    bool overlaps(const SlotsEdge& other) const {
      if (objectAndKind_ != other.objectAndKind_) {
        return false;
      }

      // Widen our range by one on each side so that we consider
      // adjacent-but-not-actually-overlapping ranges as overlapping. This
      // is particularly useful for coalescing a series of increasing or
      // decreasing single index writes 0, 1, 2, ..., N into a SlotsEdge
      // range of elements [0, N].
      uint32_t end = start_ + count_ + 1;
      uint32_t start = start_ > 0 ? start_ - 1 : 0;
      MOZ_ASSERT(start < end);

      uint32_t otherEnd = other.start_ + other.count_;
      MOZ_ASSERT(other.start_ <= otherEnd);
      return (start <= other.start_ && other.start_ <= end) ||
             (start <= otherEnd && otherEnd <= end);
    }

    // Destructively make this SlotsEdge range the union of the other
    // SlotsEdge range and this one. A precondition is that the ranges must
    // overlap.
    void merge(const SlotsEdge& other) {
      MOZ_ASSERT(overlaps(other));
      uint32_t end = Max(start_ + count_, other.start_ + other.count_);
      start_ = Min(start_, other.start_);
      count_ = end - start_;
    }

    bool maybeInRememberedSet(const Nursery& n) const {
      return !IsInsideNursery(reinterpret_cast<Cell*>(object()));
    }

<<<<<<< HEAD
    void trace(TenuringTracer& mover) const;

    explicit operator bool() const { return objectAndKind_ != 0; }

    typedef struct {
      typedef SlotsEdge Lookup;
      static HashNumber hash(const Lookup& l) {
        return mozilla::HashGeneric(l.objectAndKind_, l.start_, l.count_);
      }
      static bool match(const SlotsEdge& k, const Lookup& l) { return k == l; }
    } Hasher;

    static const auto FullBufferReason = JS::gcreason::FULL_SLOT_BUFFER;
  };

  template <typename Buffer, typename Edge>
  void unput(Buffer& buffer, const Edge& edge) {
    MOZ_ASSERT(!JS::RuntimeHeapIsBusy());
    MOZ_ASSERT(CurrentThreadCanAccessRuntime(runtime_));
    if (!isEnabled()) {
      return;
    }
    mozilla::ReentrancyGuard g(*this);
    buffer.unput(this, edge);
  }

  template <typename Buffer, typename Edge>
  void put(Buffer& buffer, const Edge& edge) {
    MOZ_ASSERT(!JS::RuntimeHeapIsBusy());
    MOZ_ASSERT(CurrentThreadCanAccessRuntime(runtime_));
    if (!isEnabled()) {
      return;
    }
    mozilla::ReentrancyGuard g(*this);
    if (edge.maybeInRememberedSet(nursery_)) {
      buffer.put(this, edge);
||||||| merged common ancestors
    template <typename Buffer, typename Edge>
    void put(Buffer& buffer, const Edge& edge) {
        MOZ_ASSERT(!JS::RuntimeHeapIsBusy());
        MOZ_ASSERT(CurrentThreadCanAccessRuntime(runtime_));
        if (!isEnabled()) {
            return;
        }
        mozilla::ReentrancyGuard g(*this);
        if (edge.maybeInRememberedSet(nursery_)) {
            buffer.put(this, edge);
        }
=======
    void trace(TenuringTracer& mover) const;

    explicit operator bool() const { return objectAndKind_ != 0; }

    typedef struct {
      typedef SlotsEdge Lookup;
      static HashNumber hash(const Lookup& l) {
        return mozilla::HashGeneric(l.objectAndKind_, l.start_, l.count_);
      }
      static bool match(const SlotsEdge& k, const Lookup& l) { return k == l; }
    } Hasher;
  };

  template <typename Buffer, typename Edge>
  void unput(Buffer& buffer, const Edge& edge) {
    MOZ_ASSERT(!JS::RuntimeHeapIsBusy());
    MOZ_ASSERT(CurrentThreadCanAccessRuntime(runtime_));
    if (!isEnabled()) {
      return;
    }
    mozilla::ReentrancyGuard g(*this);
    buffer.unput(edge);
  }

  template <typename Buffer, typename Edge>
  void put(Buffer& buffer, const Edge& edge) {
    MOZ_ASSERT(!JS::RuntimeHeapIsBusy());
    MOZ_ASSERT(CurrentThreadCanAccessRuntime(runtime_));
    if (!isEnabled()) {
      return;
    }
    mozilla::ReentrancyGuard g(*this);
    if (edge.maybeInRememberedSet(nursery_)) {
      buffer.put(edge);
>>>>>>> upstream-releases
    }
  }

<<<<<<< HEAD
  MonoTypeBuffer<ValueEdge> bufferVal;
  MonoTypeBuffer<CellPtrEdge> bufferCell;
  MonoTypeBuffer<SlotsEdge> bufferSlot;
  WholeCellBuffer bufferWholeCell;
  GenericBuffer bufferGeneric;
  bool cancelIonCompilations_;
||||||| merged common ancestors
    MonoTypeBuffer<ValueEdge> bufferVal;
    MonoTypeBuffer<CellPtrEdge> bufferCell;
    MonoTypeBuffer<SlotsEdge> bufferSlot;
    WholeCellBuffer bufferWholeCell;
    GenericBuffer bufferGeneric;
    bool cancelIonCompilations_;
=======
  MonoTypeBuffer<ValueEdge> bufferVal;
  MonoTypeBuffer<CellPtrEdge> bufStrCell;
  MonoTypeBuffer<CellPtrEdge> bufObjCell;
  MonoTypeBuffer<SlotsEdge> bufferSlot;
  WholeCellBuffer bufferWholeCell;
  GenericBuffer bufferGeneric;
  bool cancelIonCompilations_;
>>>>>>> upstream-releases

  JSRuntime* runtime_;
  const Nursery& nursery_;

  bool aboutToOverflow_;
  bool enabled_;
#ifdef DEBUG
  bool mEntered; /* For ReentrancyGuard. */
#endif

<<<<<<< HEAD
 public:
  explicit StoreBuffer(JSRuntime* rt, const Nursery& nursery)
      : bufferVal(),
        bufferCell(),
        bufferSlot(),
        bufferWholeCell(),
        bufferGeneric(),
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
||||||| merged common ancestors
  public:
    explicit StoreBuffer(JSRuntime* rt, const Nursery& nursery)
      : bufferVal(), bufferCell(), bufferSlot(), bufferWholeCell(), bufferGeneric(),
        cancelIonCompilations_(false), runtime_(rt), nursery_(nursery), aboutToOverflow_(false),
        enabled_(false)
#ifdef DEBUG
        , mEntered(false)
#endif
    {
    }
=======
 public:
  explicit StoreBuffer(JSRuntime* rt, const Nursery& nursery);
  MOZ_MUST_USE bool enable();
>>>>>>> upstream-releases

<<<<<<< HEAD
  MOZ_MUST_USE bool enable();
||||||| merged common ancestors
    MOZ_MUST_USE bool enable();
=======
  void disable();
  bool isEnabled() const { return enabled_; }
>>>>>>> upstream-releases

<<<<<<< HEAD
  void disable();
  bool isEnabled() const { return enabled_; }
||||||| merged common ancestors
    void disable();
    bool isEnabled() const { return enabled_; }
=======
  void clear();
>>>>>>> upstream-releases

<<<<<<< HEAD
  void clear();
||||||| merged common ancestors
    void clear();
=======
  const Nursery& nursery() const { return nursery_; }
>>>>>>> upstream-releases

<<<<<<< HEAD
  const Nursery& nursery() const { return nursery_; }
||||||| merged common ancestors
    const Nursery& nursery() const { return nursery_; }
=======
  /* Get the overflowed status. */
  bool isAboutToOverflow() const { return aboutToOverflow_; }
>>>>>>> upstream-releases

<<<<<<< HEAD
  /* Get the overflowed status. */
  bool isAboutToOverflow() const { return aboutToOverflow_; }
||||||| merged common ancestors
    /* Get the overflowed status. */
    bool isAboutToOverflow() const { return aboutToOverflow_; }
=======
  bool cancelIonCompilations() const { return cancelIonCompilations_; }
>>>>>>> upstream-releases

<<<<<<< HEAD
  bool cancelIonCompilations() const { return cancelIonCompilations_; }
||||||| merged common ancestors
    bool cancelIonCompilations() const { return cancelIonCompilations_; }
=======
  /* Insert a single edge into the buffer/remembered set. */
  void putValue(JS::Value* vp) { put(bufferVal, ValueEdge(vp)); }
  void unputValue(JS::Value* vp) { unput(bufferVal, ValueEdge(vp)); }
>>>>>>> upstream-releases

<<<<<<< HEAD
  /* Insert a single edge into the buffer/remembered set. */
  void putValue(JS::Value* vp) { put(bufferVal, ValueEdge(vp)); }
  void unputValue(JS::Value* vp) { unput(bufferVal, ValueEdge(vp)); }
  void putCell(Cell** cellp) { put(bufferCell, CellPtrEdge(cellp)); }
  void unputCell(Cell** cellp) { unput(bufferCell, CellPtrEdge(cellp)); }
  void putSlot(NativeObject* obj, int kind, uint32_t start, uint32_t count) {
    SlotsEdge edge(obj, kind, start, count);
    if (bufferSlot.last_.overlaps(edge)) {
      bufferSlot.last_.merge(edge);
    } else {
      put(bufferSlot, edge);
||||||| merged common ancestors
    /* Insert a single edge into the buffer/remembered set. */
    void putValue(JS::Value* vp) { put(bufferVal, ValueEdge(vp)); }
    void unputValue(JS::Value* vp) { unput(bufferVal, ValueEdge(vp)); }
    void putCell(Cell** cellp) { put(bufferCell, CellPtrEdge(cellp)); }
    void unputCell(Cell** cellp) { unput(bufferCell, CellPtrEdge(cellp)); }
    void putSlot(NativeObject* obj, int kind, uint32_t start, uint32_t count) {
        SlotsEdge edge(obj, kind, start, count);
        if (bufferSlot.last_.overlaps(edge)) {
            bufferSlot.last_.merge(edge);
        } else {
            put(bufferSlot, edge);
        }
=======
  void putCell(JSString** strp) { put(bufStrCell, CellPtrEdge(strp)); }
  void unputCell(JSString** strp) { unput(bufStrCell, CellPtrEdge(strp)); }

  void putCell(JSObject** strp) { put(bufObjCell, CellPtrEdge(strp)); }
  void unputCell(JSObject** strp) { unput(bufObjCell, CellPtrEdge(strp)); }

  void putSlot(NativeObject* obj, int kind, uint32_t start, uint32_t count) {
    SlotsEdge edge(obj, kind, start, count);
    if (bufferSlot.last_.overlaps(edge)) {
      bufferSlot.last_.merge(edge);
    } else {
      put(bufferSlot, edge);
>>>>>>> upstream-releases
    }
  }

  inline void putWholeCell(Cell* cell);

  /* Insert an entry into the generic buffer. */
  template <typename T>
  void putGeneric(const T& t) {
    put(bufferGeneric, t);
  }

  void setShouldCancelIonCompilations() { cancelIonCompilations_ = true; }

<<<<<<< HEAD
  /* Methods to trace the source of all edges in the store buffer. */
  void traceValues(TenuringTracer& mover) { bufferVal.trace(this, mover); }
  void traceCells(TenuringTracer& mover) { bufferCell.trace(this, mover); }
  void traceSlots(TenuringTracer& mover) { bufferSlot.trace(this, mover); }
  void traceWholeCells(TenuringTracer& mover) {
    bufferWholeCell.trace(this, mover);
  }
  void traceGenericEntries(JSTracer* trc) { bufferGeneric.trace(this, trc); }
||||||| merged common ancestors
    /* Methods to trace the source of all edges in the store buffer. */
    void traceValues(TenuringTracer& mover)            { bufferVal.trace(this, mover); }
    void traceCells(TenuringTracer& mover)             { bufferCell.trace(this, mover); }
    void traceSlots(TenuringTracer& mover)             { bufferSlot.trace(this, mover); }
    void traceWholeCells(TenuringTracer& mover)        { bufferWholeCell.trace(this, mover); }
    void traceGenericEntries(JSTracer *trc)            { bufferGeneric.trace(this, trc); }
=======
  /* Methods to trace the source of all edges in the store buffer. */
  void traceValues(TenuringTracer& mover) { bufferVal.trace(mover); }
  void traceCells(TenuringTracer& mover) {
    bufStrCell.traceTyped<JSString>(mover);
    bufObjCell.traceTyped<JSObject>(mover);
  }
  void traceSlots(TenuringTracer& mover) { bufferSlot.trace(mover); }
  void traceWholeCells(TenuringTracer& mover) { bufferWholeCell.trace(mover); }
  void traceGenericEntries(JSTracer* trc) { bufferGeneric.trace(trc); }
>>>>>>> upstream-releases

<<<<<<< HEAD
  /* For use by our owned buffers and for testing. */
  void setAboutToOverflow(JS::gcreason::Reason);
||||||| merged common ancestors
    /* For use by our owned buffers and for testing. */
    void setAboutToOverflow(JS::gcreason::Reason);
=======
  /* For use by our owned buffers and for testing. */
  void setAboutToOverflow(JS::GCReason);
>>>>>>> upstream-releases

  void addSizeOfExcludingThis(mozilla::MallocSizeOf mallocSizeOf,
                              JS::GCSizes* sizes);

  void checkEmpty() const;
};

// A set of cells in an arena used to implement the whole cell store buffer.
class ArenaCellSet {
  friend class StoreBuffer;

  // The arena this relates to.
  Arena* arena;

  // Pointer to next set forming a linked list.
  ArenaCellSet* next;

  // Bit vector for each possible cell start position.
  BitArray<MaxArenaCellIndex> bits;

#ifdef DEBUG
  // The minor GC number when this was created. This object should not survive
  // past the next minor collection.
  const uint64_t minorGCNumberAtCreation;
#endif

  // Construct the empty sentinel object.
  constexpr ArenaCellSet()
      : arena(nullptr),
        next(nullptr)
#ifdef DEBUG
        ,
        minorGCNumberAtCreation(0)
#endif
  {
  }

 public:
  ArenaCellSet(Arena* arena, ArenaCellSet* next);

  bool hasCell(const TenuredCell* cell) const {
    return hasCell(getCellIndex(cell));
  }

  void putCell(const TenuredCell* cell) { putCell(getCellIndex(cell)); }

  bool isEmpty() const { return this == &Empty; }

  bool hasCell(size_t cellIndex) const;

  void putCell(size_t cellIndex);

  void check() const;

  // Sentinel object used for all empty sets.
  //
  // We use a sentinel because it simplifies the JIT code slightly as we can
  // assume all arenas have a cell set.
  static ArenaCellSet Empty;

  static size_t getCellIndex(const TenuredCell* cell);
  static void getWordIndexAndMask(size_t cellIndex, size_t* wordp,
                                  uint32_t* maskp);

  // Attempt to trigger a minor GC if free space in the nursery (where these
  // objects are allocated) falls below this threshold.
  static const size_t NurseryFreeThresholdBytes = 64 * 1024;

  static size_t offsetOfArena() { return offsetof(ArenaCellSet, arena); }
  static size_t offsetOfBits() { return offsetof(ArenaCellSet, bits); }
};

} /* namespace gc */
} /* namespace js */

#endif /* gc_StoreBuffer_h */
