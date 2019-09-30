/*
 * Copyright 2015 Google Inc.
 *
 * Use of this source code is governed by a BSD-style license that can be
 * found in the LICENSE file.
 */

#include "GrOp.h"

<<<<<<< HEAD
int32_t GrOp::gCurrOpClassID = GrOp::kIllegalOpID;

int32_t GrOp::gCurrOpUniqueID = GrOp::kIllegalOpID;

#ifdef SK_DEBUG
void* GrOp::operator new(size_t size) {
    // All GrOp-derived class should be allocated in a GrMemoryPool
    SkASSERT(0);
    return ::operator new(size);
||||||| merged common ancestors
#include "GrMemoryPool.h"
#include "SkSpinlock.h"

// TODO I noticed a small benefit to using a larger exclusive pool for ops. Its very small, but
// seems to be mostly consistent.  There is a lot in flux right now, but we should really revisit
// this.


// We use a global pool protected by a mutex(spinlock). Chrome may use the same GrContext on
// different threads. The GrContext is not used concurrently on different threads and there is a
// memory barrier between accesses of a context on different threads. Also, there may be multiple
// GrContexts and those contexts may be in use concurrently on different threads.
namespace {
#if !defined(SK_BUILD_FOR_ANDROID_FRAMEWORK)
static SkSpinlock gOpPoolSpinLock;
#endif
class MemoryPoolAccessor {
public:

// We know in the Android framework there is only one GrContext.
#if defined(SK_BUILD_FOR_ANDROID_FRAMEWORK)
    MemoryPoolAccessor() {}
    ~MemoryPoolAccessor() {}
#else
    MemoryPoolAccessor() { gOpPoolSpinLock.acquire(); }
    ~MemoryPoolAccessor() { gOpPoolSpinLock.release(); }
#endif

    GrMemoryPool* pool() const {
        static GrMemoryPool gPool(16384, 16384);
        return &gPool;
    }
};
}

int32_t GrOp::gCurrOpClassID = GrOp::kIllegalOpID;

int32_t GrOp::gCurrOpUniqueID = GrOp::kIllegalOpID;

void* GrOp::operator new(size_t size) {
    return MemoryPoolAccessor().pool()->allocate(size);
=======
std::atomic<uint32_t> GrOp::gCurrOpClassID {GrOp::kIllegalOpID + 1};
std::atomic<uint32_t> GrOp::gCurrOpUniqueID{GrOp::kIllegalOpID + 1};

#ifdef SK_DEBUG
void* GrOp::operator new(size_t size) {
    // All GrOp-derived class should be allocated in a GrMemoryPool
    SkASSERT(0);
    return ::operator new(size);
}

void GrOp::operator delete(void* target) {
    // All GrOp-derived class should be released from their owning GrMemoryPool
    SkASSERT(0);
    ::operator delete(target);
}
#endif

GrOp::GrOp(uint32_t classID) : fClassID(classID) {
    SkASSERT(classID == SkToU32(fClassID));
    SkASSERT(classID);
    SkDEBUGCODE(fBoundsFlags = kUninitialized_BoundsFlag);
}

GrOp::CombineResult GrOp::combineIfPossible(GrOp* that, const GrCaps& caps) {
    SkASSERT(this != that);
    if (this->classID() != that->classID()) {
        return CombineResult::kCannotCombine;
    }
    auto result = this->onCombineIfPossible(that, caps);
    if (result == CombineResult::kMerged) {
        this->joinBounds(*that);
    }
    return result;
}

void GrOp::chainConcat(std::unique_ptr<GrOp> next) {
    SkASSERT(next);
    SkASSERT(this->classID() == next->classID());
    SkASSERT(this->isChainTail());
    SkASSERT(next->isChainHead());
    fNextInChain = std::move(next);
    fNextInChain->fPrevInChain = this;
>>>>>>> upstream-releases
}

<<<<<<< HEAD
void GrOp::operator delete(void* target) {
    // All GrOp-derived class should be released from their owning GrMemoryPool
    SkASSERT(0);
    ::operator delete(target);
||||||| merged common ancestors
void GrOp::operator delete(void* target) {
    return MemoryPoolAccessor().pool()->release(target);
=======
std::unique_ptr<GrOp> GrOp::cutChain() {
    if (fNextInChain) {
        fNextInChain->fPrevInChain = nullptr;
        return std::move(fNextInChain);
    }
    return nullptr;
>>>>>>> upstream-releases
}
#endif

<<<<<<< HEAD
GrOp::GrOp(uint32_t classID) : fClassID(classID) {
    SkASSERT(classID == SkToU32(fClassID));
    SkASSERT(classID);
    SkDEBUGCODE(fBoundsFlags = kUninitialized_BoundsFlag);
||||||| merged common ancestors
GrOp::GrOp(uint32_t classID)
    : fClassID(classID)
    , fUniqueID(kIllegalOpID) {
    SkASSERT(classID == SkToU32(fClassID));
    SkDEBUGCODE(fBoundsFlags = kUninitialized_BoundsFlag);
=======
#ifdef SK_DEBUG
void GrOp::validateChain(GrOp* expectedTail) const {
    SkASSERT(this->isChainHead());
    uint32_t classID = this->classID();
    const GrOp* op = this;
    while (op) {
        SkASSERT(op == this || (op->prevInChain() && op->prevInChain()->nextInChain() == op));
        SkASSERT(classID == op->classID());
        if (op->nextInChain()) {
            SkASSERT(op->nextInChain()->prevInChain() == op);
            SkASSERT(op != expectedTail);
        } else {
            SkASSERT(!expectedTail || op == expectedTail);
        }
        op = op->nextInChain();
    }
>>>>>>> upstream-releases
}
<<<<<<< HEAD

GrOp::~GrOp() {}

GrOp::CombineResult GrOp::combineIfPossible(GrOp* that, const GrCaps& caps) {
    if (this->classID() != that->classID()) {
        return CombineResult::kCannotCombine;
    }
    SkDEBUGCODE(bool thatWasChained = that->isChained());
    auto result = this->onCombineIfPossible(that, caps);
    // Merging a chained 'that' would cause problems given the way op lists currently manage chains.
    SkASSERT(!(thatWasChained && result == CombineResult::kMerged));
    if (fChainHead) {
        fChainHead->joinBounds(*that);
    }
    return result;
}

void GrOp::setNextInChain(GrOp* next) {
    SkASSERT(next);
    SkASSERT(this->classID() == next->classID());
    // Each op begins life as a 1 element list. We assume lists are appended only with
    SkASSERT(this->isChainTail());
    SkASSERT(!next->isChained());
    if (!fChainHead) {
        // We were using null to mark 'this' as unchained. Now 'this' is the head of the new chain.
        fChainHead = this;
    }
    fNextInChain = next;
    fChainHead->joinBounds(*next);
    next->fChainHead = this->fChainHead;
}
||||||| merged common ancestors

GrOp::~GrOp() {}
=======
#endif
>>>>>>> upstream-releases
