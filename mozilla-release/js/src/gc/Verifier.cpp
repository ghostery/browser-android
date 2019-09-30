/* -*- Mode: C++; tab-width: 8; indent-tabs-mode: nil; c-basic-offset: 2 -*-
 * vim: set ts=8 sts=2 et sw=2 tw=80:
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/. */

#include "gc/Verifier.h"

#include "mozilla/DebugOnly.h"
#include "mozilla/IntegerPrintfMacros.h"
#include "mozilla/Sprintf.h"

#ifdef MOZ_VALGRIND
<<<<<<< HEAD
#include <valgrind/memcheck.h>
||||||| merged common ancestors
# include <valgrind/memcheck.h>
=======
#  include <valgrind/memcheck.h>
>>>>>>> upstream-releases
#endif

#include "gc/GCInternals.h"
#include "gc/GCLock.h"
#include "gc/PublicIterators.h"
#include "gc/WeakMap.h"
#include "gc/Zone.h"
#include "js/HashTable.h"
#include "vm/JSContext.h"

#include "gc/ArenaList-inl.h"
#include "gc/GC-inl.h"
#include "gc/Marking-inl.h"
#include "vm/JSContext-inl.h"

using namespace js;
using namespace js::gc;

using mozilla::DebugOnly;

#ifdef JS_GC_ZEAL

/*
 * Write barrier verification
 *
 * The next few functions are for write barrier verification.
 *
 * The VerifyBarriers function is a shorthand. It checks if a verification phase
 * is currently running. If not, it starts one. Otherwise, it ends the current
 * phase and starts a new one.
 *
 * The user can adjust the frequency of verifications, which causes
 * VerifyBarriers to be a no-op all but one out of N calls. However, if the
 * |always| parameter is true, it starts a new phase no matter what.
 *
 * Pre-Barrier Verifier:
 *   When StartVerifyBarriers is called, a snapshot is taken of all objects in
 *   the GC heap and saved in an explicit graph data structure. Later,
 *   EndVerifyBarriers traverses the heap again. Any pointer values that were in
 *   the snapshot and are no longer found must be marked; otherwise an assertion
 *   triggers. Note that we must not GC in between starting and finishing a
 *   verification phase.
 */

struct EdgeValue {
  void* thing;
  JS::TraceKind kind;
  const char* label;
};

struct VerifyNode {
  void* thing;
  JS::TraceKind kind;
  uint32_t count;
  EdgeValue edges[1];
};

typedef HashMap<void*, VerifyNode*, DefaultHasher<void*>, SystemAllocPolicy>
    NodeMap;

/*
 * The verifier data structures are simple. The entire graph is stored in a
 * single block of memory. At the beginning is a VerifyNode for the root
 * node. It is followed by a sequence of EdgeValues--the exact number is given
 * in the node. After the edges come more nodes and their edges.
 *
 * The edgeptr and term fields are used to allocate out of the block of memory
 * for the graph. If we run out of memory (i.e., if edgeptr goes beyond term),
 * we just abandon the verification.
 *
 * The nodemap field is a hashtable that maps from the address of the GC thing
 * to the VerifyNode that represents it.
 */
class js::VerifyPreTracer final : public JS::CallbackTracer {
  JS::AutoDisableGenerationalGC noggc;

  void onChild(const JS::GCCellPtr& thing) override;

 public:
  /* The gcNumber when the verification began. */
  uint64_t number;

  /* This counts up to gcZealFrequency to decide whether to verify. */
  int count;

  /* This graph represents the initial GC "snapshot". */
  VerifyNode* curnode;
  VerifyNode* root;
  char* edgeptr;
  char* term;
  NodeMap nodemap;

  explicit VerifyPreTracer(JSRuntime* rt)
      : JS::CallbackTracer(rt),
        noggc(rt->mainContextFromOwnThread()),
        number(rt->gc.gcNumber()),
        count(0),
        curnode(nullptr),
        root(nullptr),
        edgeptr(nullptr),
        term(nullptr) {}

  ~VerifyPreTracer() { js_free(root); }
};

/*
 * This function builds up the heap snapshot by adding edges to the current
 * node.
 */
void VerifyPreTracer::onChild(const JS::GCCellPtr& thing) {
  MOZ_ASSERT(!IsInsideNursery(thing.asCell()));

  // Skip things in other runtimes.
  if (thing.asCell()->asTenured().runtimeFromAnyThread() != runtime()) {
    return;
  }

<<<<<<< HEAD
  edgeptr += sizeof(EdgeValue);
  if (edgeptr >= term) {
    edgeptr = term;
    return;
  }

  VerifyNode* node = curnode;
  uint32_t i = node->count;

  node->edges[i].thing = thing.asCell();
  node->edges[i].kind = thing.kind();
  node->edges[i].label = contextName();
  node->count++;
}
||||||| merged common ancestors
    VerifyNode* node = curnode;
    uint32_t i = node->count;

    node->edges[i].thing = thing.asCell();
    node->edges[i].kind = thing.kind();
    node->edges[i].label = contextName();
    node->count++;
}

static VerifyNode*
MakeNode(VerifyPreTracer* trc, void* thing, JS::TraceKind kind)
{
    NodeMap::AddPtr p = trc->nodemap.lookupForAdd(thing);
    if (!p) {
        VerifyNode* node = (VerifyNode*)trc->edgeptr;
        trc->edgeptr += sizeof(VerifyNode) - sizeof(EdgeValue);
        if (trc->edgeptr >= trc->term) {
            trc->edgeptr = trc->term;
            return nullptr;
        }
=======
  edgeptr += sizeof(EdgeValue);
  if (edgeptr >= term) {
    edgeptr = term;
    return;
  }
>>>>>>> upstream-releases

<<<<<<< HEAD
static VerifyNode* MakeNode(VerifyPreTracer* trc, void* thing,
                            JS::TraceKind kind) {
  NodeMap::AddPtr p = trc->nodemap.lookupForAdd(thing);
  if (!p) {
    VerifyNode* node = (VerifyNode*)trc->edgeptr;
    trc->edgeptr += sizeof(VerifyNode) - sizeof(EdgeValue);
    if (trc->edgeptr >= trc->term) {
      trc->edgeptr = trc->term;
      return nullptr;
    }
||||||| merged common ancestors
        node->thing = thing;
        node->count = 0;
        node->kind = kind;
        if (!trc->nodemap.add(p, thing, node)) {
            trc->edgeptr = trc->term;
            return nullptr;
        }
=======
  VerifyNode* node = curnode;
  uint32_t i = node->count;
>>>>>>> upstream-releases

<<<<<<< HEAD
    node->thing = thing;
    node->count = 0;
    node->kind = kind;
    if (!trc->nodemap.add(p, thing, node)) {
      trc->edgeptr = trc->term;
      return nullptr;
    }

    return node;
  }
  return nullptr;
||||||| merged common ancestors
        return node;
    }
    return nullptr;
=======
  node->edges[i].thing = thing.asCell();
  node->edges[i].kind = thing.kind();
  node->edges[i].label = contextName();
  node->count++;
>>>>>>> upstream-releases
}

<<<<<<< HEAD
static VerifyNode* NextNode(VerifyNode* node) {
  if (node->count == 0) {
    return (VerifyNode*)((char*)node + sizeof(VerifyNode) - sizeof(EdgeValue));
  } else {
    return (VerifyNode*)((char*)node + sizeof(VerifyNode) +
                         sizeof(EdgeValue) * (node->count - 1));
  }
}
||||||| merged common ancestors
static VerifyNode*
NextNode(VerifyNode* node)
{
    if (node->count == 0) {
        return (VerifyNode*)((char*)node + sizeof(VerifyNode) - sizeof(EdgeValue));
    } else {
        return (VerifyNode*)((char*)node + sizeof(VerifyNode) +
                             sizeof(EdgeValue)*(node->count - 1));
    }
}
=======
static VerifyNode* MakeNode(VerifyPreTracer* trc, void* thing,
                            JS::TraceKind kind) {
  NodeMap::AddPtr p = trc->nodemap.lookupForAdd(thing);
  if (!p) {
    VerifyNode* node = (VerifyNode*)trc->edgeptr;
    trc->edgeptr += sizeof(VerifyNode) - sizeof(EdgeValue);
    if (trc->edgeptr >= trc->term) {
      trc->edgeptr = trc->term;
      return nullptr;
    }
>>>>>>> upstream-releases

<<<<<<< HEAD
void gc::GCRuntime::startVerifyPreBarriers() {
  if (verifyPreData || isIncrementalGCInProgress()) {
    return;
  }
||||||| merged common ancestors
void
gc::GCRuntime::startVerifyPreBarriers()
{
    if (verifyPreData || isIncrementalGCInProgress()) {
        return;
    }
=======
    node->thing = thing;
    node->count = 0;
    node->kind = kind;
    if (!trc->nodemap.add(p, thing, node)) {
      trc->edgeptr = trc->term;
      return nullptr;
    }
>>>>>>> upstream-releases

<<<<<<< HEAD
  JSContext* cx = rt->mainContextFromOwnThread();
  if (temporaryAbortIfWasmGc(cx)) {
    return;
  }
||||||| merged common ancestors
    JSContext* cx = rt->mainContextFromOwnThread();
    if (temporaryAbortIfWasmGc(cx)) {
        return;
    }
=======
    return node;
  }
  return nullptr;
}
>>>>>>> upstream-releases

<<<<<<< HEAD
  if (IsIncrementalGCUnsafe(rt) != AbortReason::None ||
      rt->hasHelperThreadZones()) {
    return;
  }
||||||| merged common ancestors
    if (IsIncrementalGCUnsafe(rt) != AbortReason::None ||
        rt->hasHelperThreadZones())
    {
        return;
    }
=======
static VerifyNode* NextNode(VerifyNode* node) {
  if (node->count == 0) {
    return (VerifyNode*)((char*)node + sizeof(VerifyNode) - sizeof(EdgeValue));
  } else {
    return (VerifyNode*)((char*)node + sizeof(VerifyNode) +
                         sizeof(EdgeValue) * (node->count - 1));
  }
}
>>>>>>> upstream-releases

<<<<<<< HEAD
  number++;
||||||| merged common ancestors
    number++;
=======
void gc::GCRuntime::startVerifyPreBarriers() {
  if (verifyPreData || isIncrementalGCInProgress()) {
    return;
  }
>>>>>>> upstream-releases

<<<<<<< HEAD
  VerifyPreTracer* trc = js_new<VerifyPreTracer>(rt);
  if (!trc) {
    return;
  }
||||||| merged common ancestors
    VerifyPreTracer* trc = js_new<VerifyPreTracer>(rt);
    if (!trc) {
        return;
    }
=======
  JSContext* cx = rt->mainContextFromOwnThread();
>>>>>>> upstream-releases

<<<<<<< HEAD
  AutoPrepareForTracing prep(cx);
||||||| merged common ancestors
    AutoPrepareForTracing prep(cx);
=======
  if (IsIncrementalGCUnsafe(rt) != AbortReason::None ||
      rt->hasHelperThreadZones()) {
    return;
  }
>>>>>>> upstream-releases

<<<<<<< HEAD
  {
    AutoLockGC lock(cx->runtime());
    for (auto chunk = allNonEmptyChunks(lock); !chunk.done(); chunk.next()) {
      chunk->bitmap.clear();
    }
  }
||||||| merged common ancestors
    {
        AutoLockGC lock(cx->runtime());
        for (auto chunk = allNonEmptyChunks(lock); !chunk.done(); chunk.next()) {
            chunk->bitmap.clear();
        }
    }
=======
  number++;

  VerifyPreTracer* trc = js_new<VerifyPreTracer>(rt);
  if (!trc) {
    return;
  }
>>>>>>> upstream-releases

<<<<<<< HEAD
  gcstats::AutoPhase ap(stats(), gcstats::PhaseKind::TRACE_HEAP);
||||||| merged common ancestors
    gcstats::AutoPhase ap(stats(), gcstats::PhaseKind::TRACE_HEAP);
=======
  AutoPrepareForTracing prep(cx);
>>>>>>> upstream-releases

<<<<<<< HEAD
  const size_t size = 64 * 1024 * 1024;
  trc->root = (VerifyNode*)js_malloc(size);
  if (!trc->root) {
    goto oom;
  }
  trc->edgeptr = (char*)trc->root;
  trc->term = trc->edgeptr + size;

  /* Create the root node. */
  trc->curnode = MakeNode(trc, nullptr, JS::TraceKind(0));
||||||| merged common ancestors
    const size_t size = 64 * 1024 * 1024;
    trc->root = (VerifyNode*)js_malloc(size);
    if (!trc->root) {
        goto oom;
    }
    trc->edgeptr = (char*)trc->root;
    trc->term = trc->edgeptr + size;
=======
  {
    AutoLockGC lock(cx->runtime());
    for (auto chunk = allNonEmptyChunks(lock); !chunk.done(); chunk.next()) {
      chunk->bitmap.clear();
    }
  }
>>>>>>> upstream-releases

<<<<<<< HEAD
  incrementalState = State::MarkRoots;
||||||| merged common ancestors
    /* Create the root node. */
    trc->curnode = MakeNode(trc, nullptr, JS::TraceKind(0));
=======
  gcstats::AutoPhase ap(stats(), gcstats::PhaseKind::TRACE_HEAP);
>>>>>>> upstream-releases

<<<<<<< HEAD
  /* Make all the roots be edges emanating from the root node. */
  traceRuntime(trc, prep);
||||||| merged common ancestors
    incrementalState = State::MarkRoots;
=======
  const size_t size = 64 * 1024 * 1024;
  trc->root = (VerifyNode*)js_malloc(size);
  if (!trc->root) {
    goto oom;
  }
  trc->edgeptr = (char*)trc->root;
  trc->term = trc->edgeptr + size;
>>>>>>> upstream-releases

<<<<<<< HEAD
  VerifyNode* node;
  node = trc->curnode;
  if (trc->edgeptr == trc->term) {
    goto oom;
  }
||||||| merged common ancestors
    /* Make all the roots be edges emanating from the root node. */
    traceRuntime(trc, prep);
=======
  /* Create the root node. */
  trc->curnode = MakeNode(trc, nullptr, JS::TraceKind(0));
>>>>>>> upstream-releases

<<<<<<< HEAD
  /* For each edge, make a node for it if one doesn't already exist. */
  while ((char*)node < trc->edgeptr) {
    for (uint32_t i = 0; i < node->count; i++) {
      EdgeValue& e = node->edges[i];
      VerifyNode* child = MakeNode(trc, e.thing, e.kind);
      if (child) {
        trc->curnode = child;
        js::TraceChildren(trc, e.thing, e.kind);
      }
      if (trc->edgeptr == trc->term) {
        goto oom;
      }
    }
||||||| merged common ancestors
    VerifyNode* node;
    node = trc->curnode;
    if (trc->edgeptr == trc->term) {
        goto oom;
    }
=======
  incrementalState = State::MarkRoots;
>>>>>>> upstream-releases

<<<<<<< HEAD
    node = NextNode(node);
  }
||||||| merged common ancestors
    /* For each edge, make a node for it if one doesn't already exist. */
    while ((char*)node < trc->edgeptr) {
        for (uint32_t i = 0; i < node->count; i++) {
            EdgeValue& e = node->edges[i];
            VerifyNode* child = MakeNode(trc, e.thing, e.kind);
            if (child) {
                trc->curnode = child;
                js::TraceChildren(trc, e.thing, e.kind);
            }
            if (trc->edgeptr == trc->term) {
                goto oom;
            }
        }
=======
  /* Make all the roots be edges emanating from the root node. */
  traceRuntime(trc, prep);

  VerifyNode* node;
  node = trc->curnode;
  if (trc->edgeptr == trc->term) {
    goto oom;
  }
>>>>>>> upstream-releases

<<<<<<< HEAD
  verifyPreData = trc;
  incrementalState = State::Mark;
  marker.start();
||||||| merged common ancestors
        node = NextNode(node);
    }
=======
  /* For each edge, make a node for it if one doesn't already exist. */
  while ((char*)node < trc->edgeptr) {
    for (uint32_t i = 0; i < node->count; i++) {
      EdgeValue& e = node->edges[i];
      VerifyNode* child = MakeNode(trc, e.thing, e.kind);
      if (child) {
        trc->curnode = child;
        js::TraceChildren(trc, e.thing, e.kind);
      }
      if (trc->edgeptr == trc->term) {
        goto oom;
      }
    }
>>>>>>> upstream-releases

<<<<<<< HEAD
  for (ZonesIter zone(rt, WithAtoms); !zone.done(); zone.next()) {
    MOZ_ASSERT(!zone->usedByHelperThread());
    zone->setNeedsIncrementalBarrier(true);
    zone->arenas.clearFreeLists();
  }
||||||| merged common ancestors
    verifyPreData = trc;
    incrementalState = State::Mark;
    marker.start();
=======
    node = NextNode(node);
  }
>>>>>>> upstream-releases

<<<<<<< HEAD
  return;
||||||| merged common ancestors
    for (ZonesIter zone(rt, WithAtoms); !zone.done(); zone.next()) {
        MOZ_ASSERT(!zone->usedByHelperThread());
        zone->setNeedsIncrementalBarrier(true);
        zone->arenas.clearFreeLists();
    }

    return;
=======
  verifyPreData = trc;
  incrementalState = State::Mark;
  marker.start();

  for (ZonesIter zone(rt, WithAtoms); !zone.done(); zone.next()) {
    MOZ_ASSERT(!zone->usedByHelperThread());
    zone->setNeedsIncrementalBarrier(true);
    zone->arenas.clearFreeLists();
  }

  return;
>>>>>>> upstream-releases

oom:
  incrementalState = State::NotActive;
  js_delete(trc);
  verifyPreData = nullptr;
}

static bool IsMarkedOrAllocated(TenuredCell* cell) {
  return cell->isMarkedAny();
}

<<<<<<< HEAD
struct CheckEdgeTracer : public JS::CallbackTracer {
  VerifyNode* node;
  explicit CheckEdgeTracer(JSRuntime* rt)
      : JS::CallbackTracer(rt), node(nullptr) {}
  void onChild(const JS::GCCellPtr& thing) override;
||||||| merged common ancestors
struct CheckEdgeTracer : public JS::CallbackTracer {
    VerifyNode* node;
    explicit CheckEdgeTracer(JSRuntime* rt) : JS::CallbackTracer(rt), node(nullptr) {}
    void onChild(const JS::GCCellPtr& thing) override;
=======
struct CheckEdgeTracer final : public JS::CallbackTracer {
  VerifyNode* node;
  explicit CheckEdgeTracer(JSRuntime* rt)
      : JS::CallbackTracer(rt), node(nullptr) {}
  void onChild(const JS::GCCellPtr& thing) override;
>>>>>>> upstream-releases
};

static const uint32_t MAX_VERIFIER_EDGES = 1000;

/*
 * This function is called by EndVerifyBarriers for every heap edge. If the edge
 * already existed in the original snapshot, we "cancel it out" by overwriting
 * it with nullptr. EndVerifyBarriers later asserts that the remaining
 * non-nullptr edges (i.e., the ones from the original snapshot that must have
 * been modified) must point to marked objects.
 */
void CheckEdgeTracer::onChild(const JS::GCCellPtr& thing) {
  // Skip things in other runtimes.
  if (thing.asCell()->asTenured().runtimeFromAnyThread() != runtime()) {
    return;
  }

  /* Avoid n^2 behavior. */
  if (node->count > MAX_VERIFIER_EDGES) {
    return;
  }

  for (uint32_t i = 0; i < node->count; i++) {
    if (node->edges[i].thing == thing.asCell()) {
      MOZ_ASSERT(node->edges[i].kind == thing.kind());
      node->edges[i].thing = nullptr;
      return;
    }
  }
}

void js::gc::AssertSafeToSkipBarrier(TenuredCell* thing) {
  mozilla::DebugOnly<Zone*> zone = thing->zoneFromAnyThread();
  MOZ_ASSERT(!zone->needsIncrementalBarrier() || zone->isAtomsZone());
}

static bool IsMarkedOrAllocated(const EdgeValue& edge) {
  if (!edge.thing ||
      IsMarkedOrAllocated(TenuredCell::fromPointer(edge.thing))) {
    return true;
  }

  // Permanent atoms and well-known symbols aren't marked during graph
  // traversal.
  if (edge.kind == JS::TraceKind::String &&
      static_cast<JSString*>(edge.thing)->isPermanentAtom()) {
    return true;
  }
  if (edge.kind == JS::TraceKind::Symbol &&
      static_cast<JS::Symbol*>(edge.thing)->isWellKnownSymbol()) {
    return true;
  }

  return false;
}

void gc::GCRuntime::endVerifyPreBarriers() {
  VerifyPreTracer* trc = verifyPreData;

  if (!trc) {
    return;
  }

  MOZ_ASSERT(!JS::IsGenerationalGCEnabled(rt));

  AutoPrepareForTracing prep(rt->mainContextFromOwnThread());

  bool compartmentCreated = false;

  /* We need to disable barriers before tracing, which may invoke barriers. */
  for (ZonesIter zone(rt, WithAtoms); !zone.done(); zone.next()) {
    if (!zone->needsIncrementalBarrier()) {
      compartmentCreated = true;
    }

    zone->setNeedsIncrementalBarrier(false);
  }

  /*
   * We need to bump gcNumber so that the methodjit knows that jitcode has
   * been discarded.
   */
  MOZ_ASSERT(trc->number == number);
  number++;

  verifyPreData = nullptr;
  incrementalState = State::NotActive;

  if (!compartmentCreated && IsIncrementalGCUnsafe(rt) == AbortReason::None &&
      !rt->hasHelperThreadZones()) {
    CheckEdgeTracer cetrc(rt);

    /* Start after the roots. */
    VerifyNode* node = NextNode(trc->root);
    while ((char*)node < trc->edgeptr) {
      cetrc.node = node;
      js::TraceChildren(&cetrc, node->thing, node->kind);

      if (node->count <= MAX_VERIFIER_EDGES) {
        for (uint32_t i = 0; i < node->count; i++) {
          EdgeValue& edge = node->edges[i];
          if (!IsMarkedOrAllocated(edge)) {
            char msgbuf[1024];
            SprintfLiteral(
                msgbuf,
                "[barrier verifier] Unmarked edge: %s %p '%s' edge to %s %p",
                JS::GCTraceKindToAscii(node->kind), node->thing, edge.label,
                JS::GCTraceKindToAscii(edge.kind), edge.thing);
            MOZ_ReportAssertionFailure(msgbuf, __FILE__, __LINE__);
            MOZ_CRASH();
          }
        }
      }

      node = NextNode(node);
    }
  }

  marker.reset();
  marker.stop();

  js_delete(trc);
}

/*** Barrier Verifier Scheduling ***/

void gc::GCRuntime::verifyPreBarriers() {
  if (verifyPreData) {
    endVerifyPreBarriers();
  } else {
    startVerifyPreBarriers();
  }
}

<<<<<<< HEAD
void gc::VerifyBarriers(JSRuntime* rt, VerifierType type) {
  if (GCRuntime::temporaryAbortIfWasmGc(rt->mainContextFromOwnThread())) {
    return;
  }
  if (type == PreBarrierVerifier) {
    rt->gc.verifyPreBarriers();
  }
||||||| merged common ancestors
void
gc::VerifyBarriers(JSRuntime* rt, VerifierType type)
{
    if (GCRuntime::temporaryAbortIfWasmGc(rt->mainContextFromOwnThread())) {
        return;
    }
    if (type == PreBarrierVerifier) {
        rt->gc.verifyPreBarriers();
    }
=======
void gc::VerifyBarriers(JSRuntime* rt, VerifierType type) {
  if (type == PreBarrierVerifier) {
    rt->gc.verifyPreBarriers();
  }
>>>>>>> upstream-releases
}

void gc::GCRuntime::maybeVerifyPreBarriers(bool always) {
  if (!hasZealMode(ZealMode::VerifierPre)) {
    return;
  }

  if (rt->mainContextFromOwnThread()->suppressGC) {
    return;
  }

  if (verifyPreData) {
    if (++verifyPreData->count < zealFrequency && !always) {
      return;
    }

    endVerifyPreBarriers();
  }

  startVerifyPreBarriers();
}

void js::gc::MaybeVerifyBarriers(JSContext* cx, bool always) {
  GCRuntime* gc = &cx->runtime()->gc;
  gc->maybeVerifyPreBarriers(always);
}

void js::gc::GCRuntime::finishVerifier() {
  if (verifyPreData) {
    js_delete(verifyPreData.ref());
    verifyPreData = nullptr;
  }
}

#endif /* JS_GC_ZEAL */

#if defined(JS_GC_ZEAL) || defined(DEBUG)

<<<<<<< HEAD
class HeapCheckTracerBase : public JS::CallbackTracer {
 public:
  explicit HeapCheckTracerBase(JSRuntime* rt, WeakMapTraceKind weakTraceKind);
  bool traceHeap(AutoTraceSession& session);
  virtual void checkCell(Cell* cell) = 0;
||||||| merged common ancestors
class HeapCheckTracerBase : public JS::CallbackTracer
{
  public:
    explicit HeapCheckTracerBase(JSRuntime* rt, WeakMapTraceKind weakTraceKind);
    bool traceHeap(AutoTraceSession& session);
    virtual void checkCell(Cell* cell) = 0;
=======
static const char* CellColorName(CellColor color) {
  switch (color) {
    case CellColor::White:
      return "white";
    case CellColor::Black:
      return "black";
    case CellColor::Gray:
      return "gray";
    default:
      MOZ_CRASH("Unexpected cell color");
  }
}
>>>>>>> upstream-releases

<<<<<<< HEAD
 protected:
  void dumpCellInfo(Cell* cell);
  void dumpCellPath();
||||||| merged common ancestors
  protected:
    void dumpCellInfo(Cell* cell);
    void dumpCellPath();
=======
static const char* GetCellColorName(Cell* cell) {
  return CellColorName(GetCellColor(cell));
}
>>>>>>> upstream-releases

<<<<<<< HEAD
  Cell* parentCell() {
    return parentIndex == -1 ? nullptr : stack[parentIndex].thing.asCell();
  }
||||||| merged common ancestors
    Cell* parentCell() {
        return parentIndex == -1 ? nullptr : stack[parentIndex].thing.asCell();
    }

    size_t failures;
=======
class HeapCheckTracerBase : public JS::CallbackTracer {
 public:
  explicit HeapCheckTracerBase(JSRuntime* rt, WeakMapTraceKind weakTraceKind);
  bool traceHeap(AutoTraceSession& session);
  virtual void checkCell(Cell* cell) = 0;

 protected:
  void dumpCellInfo(Cell* cell);
  void dumpCellPath();

  Cell* parentCell() {
    return parentIndex == -1 ? nullptr : stack[parentIndex].thing.asCell();
  }
>>>>>>> upstream-releases

  size_t failures;

 private:
  void onChild(const JS::GCCellPtr& thing) override;

  struct WorkItem {
    WorkItem(JS::GCCellPtr thing, const char* name, int parentIndex)
        : thing(thing),
          name(name),
          parentIndex(parentIndex),
          processed(false) {}

    JS::GCCellPtr thing;
    const char* name;
    int parentIndex;
    bool processed;
  };

  JSRuntime* rt;
  bool oom;
  HashSet<Cell*, DefaultHasher<Cell*>, SystemAllocPolicy> visited;
  Vector<WorkItem, 0, SystemAllocPolicy> stack;
  int parentIndex;
};

<<<<<<< HEAD
HeapCheckTracerBase::HeapCheckTracerBase(JSRuntime* rt,
                                         WeakMapTraceKind weakTraceKind)
    : CallbackTracer(rt, weakTraceKind),
      failures(0),
      rt(rt),
      oom(false),
      parentIndex(-1) {
#ifdef DEBUG
  setCheckEdges(false);
#endif
||||||| merged common ancestors
HeapCheckTracerBase::HeapCheckTracerBase(JSRuntime* rt, WeakMapTraceKind weakTraceKind)
  : CallbackTracer(rt, weakTraceKind),
    failures(0),
    rt(rt),
    oom(false),
    parentIndex(-1)
{
#ifdef DEBUG
    setCheckEdges(false);
#endif
=======
HeapCheckTracerBase::HeapCheckTracerBase(JSRuntime* rt,
                                         WeakMapTraceKind weakTraceKind)
    : CallbackTracer(rt, weakTraceKind),
      failures(0),
      rt(rt),
      oom(false),
      parentIndex(-1) {
#  ifdef DEBUG
  setCheckEdges(false);
#  endif
>>>>>>> upstream-releases
}

<<<<<<< HEAD
void HeapCheckTracerBase::onChild(const JS::GCCellPtr& thing) {
  Cell* cell = thing.asCell();
  checkCell(cell);
||||||| merged common ancestors
void
HeapCheckTracerBase::onChild(const JS::GCCellPtr& thing)
{
    Cell* cell = thing.asCell();
    checkCell(cell);

    if (visited.lookup(cell)) {
        return;
    }

    if (!visited.put(cell)) {
        oom = true;
        return;
    }
=======
void HeapCheckTracerBase::onChild(const JS::GCCellPtr& thing) {
  Cell* cell = thing.asCell();
  checkCell(cell);

  if (visited.lookup(cell)) {
    return;
  }

  if (!visited.put(cell)) {
    oom = true;
    return;
  }
>>>>>>> upstream-releases

<<<<<<< HEAD
  if (visited.lookup(cell)) {
    return;
  }
||||||| merged common ancestors
    // Don't trace into GC things owned by another runtime.
    if (cell->runtimeFromAnyThread() != rt) {
        return;
    }
=======
  // Don't trace into GC things owned by another runtime.
  if (cell->runtimeFromAnyThread() != rt) {
    return;
  }

  // Don't trace into GC in zones being used by helper threads.
  Zone* zone;
  if (thing.is<JSObject>()) {
    zone = thing.as<JSObject>().zone();
  } else if (thing.is<JSString>()) {
    zone = thing.as<JSString>().zone();
  } else {
    zone = cell->asTenured().zone();
  }

  if (zone->usedByHelperThread()) {
    return;
  }
>>>>>>> upstream-releases

<<<<<<< HEAD
  if (!visited.put(cell)) {
    oom = true;
    return;
  }
||||||| merged common ancestors
    // Don't trace into GC in zones being used by helper threads.
    Zone* zone;
    if (thing.is<JSObject>()) {
        zone = thing.as<JSObject>().zone();
    } else if (thing.is<JSString>()) {
        zone = thing.as<JSString>().zone();
    } else {
        zone = cell->asTenured().zone();
    }
=======
  WorkItem item(thing, contextName(), parentIndex);
  if (!stack.append(item)) {
    oom = true;
  }
}

bool HeapCheckTracerBase::traceHeap(AutoTraceSession& session) {
  // The analysis thinks that traceRuntime might GC by calling a GC callback.
  JS::AutoSuppressGCAnalysis nogc;
  if (!rt->isBeingDestroyed()) {
    rt->gc.traceRuntime(this, session);
  }

  while (!stack.empty() && !oom) {
    WorkItem item = stack.back();
    if (item.processed) {
      stack.popBack();
    } else {
      parentIndex = stack.length() - 1;
      stack.back().processed = true;
      TraceChildren(this, item.thing);
    }
  }
>>>>>>> upstream-releases

<<<<<<< HEAD
  // Don't trace into GC things owned by another runtime.
  if (cell->runtimeFromAnyThread() != rt) {
    return;
  }

  // Don't trace into GC in zones being used by helper threads.
  Zone* zone;
  if (thing.is<JSObject>()) {
    zone = thing.as<JSObject>().zone();
  } else if (thing.is<JSString>()) {
    zone = thing.as<JSString>().zone();
  } else {
    zone = cell->asTenured().zone();
  }

  if (zone->usedByHelperThread()) {
    return;
  }
||||||| merged common ancestors
    if (zone->usedByHelperThread()) {
        return;
    }
=======
  return !oom;
}
>>>>>>> upstream-releases

<<<<<<< HEAD
  WorkItem item(thing, contextName(), parentIndex);
  if (!stack.append(item)) {
    oom = true;
  }
||||||| merged common ancestors
    WorkItem item(thing, contextName(), parentIndex);
    if (!stack.append(item)) {
        oom = true;
    }
=======
void HeapCheckTracerBase::dumpCellInfo(Cell* cell) {
  auto kind = cell->getTraceKind();
  JSObject* obj =
      kind == JS::TraceKind::Object ? static_cast<JSObject*>(cell) : nullptr;

  fprintf(stderr, "%s %s", GetCellColorName(cell), GCTraceKindToAscii(kind));
  if (obj) {
    fprintf(stderr, " %s", obj->getClass()->name);
  }
  fprintf(stderr, " %p", cell);
  if (obj) {
    fprintf(stderr, " (compartment %p)", obj->compartment());
  }
>>>>>>> upstream-releases
}

<<<<<<< HEAD
bool HeapCheckTracerBase::traceHeap(AutoTraceSession& session) {
  // The analysis thinks that traceRuntime might GC by calling a GC callback.
  JS::AutoSuppressGCAnalysis nogc;
  if (!rt->isBeingDestroyed()) {
    rt->gc.traceRuntime(this, session);
  }

  while (!stack.empty() && !oom) {
    WorkItem item = stack.back();
    if (item.processed) {
      stack.popBack();
    } else {
      parentIndex = stack.length() - 1;
      stack.back().processed = true;
      TraceChildren(this, item.thing);
    }
  }
||||||| merged common ancestors
bool
HeapCheckTracerBase::traceHeap(AutoTraceSession& session)
{
    // The analysis thinks that traceRuntime might GC by calling a GC callback.
    JS::AutoSuppressGCAnalysis nogc;
    if (!rt->isBeingDestroyed()) {
        rt->gc.traceRuntime(this, session);
    }

    while (!stack.empty() && !oom) {
        WorkItem item = stack.back();
        if (item.processed) {
            stack.popBack();
        } else {
            parentIndex = stack.length() - 1;
            stack.back().processed = true;
            TraceChildren(this, item.thing);
        }
    }
=======
void HeapCheckTracerBase::dumpCellPath() {
  const char* name = contextName();
  for (int index = parentIndex; index != -1; index = stack[index].parentIndex) {
    const WorkItem& parent = stack[index];
    Cell* cell = parent.thing.asCell();
    fprintf(stderr, "  from ");
    dumpCellInfo(cell);
    fprintf(stderr, " %s edge\n", name);
    name = parent.name;
  }
  fprintf(stderr, "  from root %s\n", name);
}

class CheckHeapTracer final : public HeapCheckTracerBase {
 public:
  enum GCType { Moving, NonMoving };
>>>>>>> upstream-releases

<<<<<<< HEAD
  return !oom;
}
||||||| merged common ancestors
    return !oom;
}
=======
  explicit CheckHeapTracer(JSRuntime* rt, GCType type);
  void check(AutoTraceSession& session);
>>>>>>> upstream-releases

<<<<<<< HEAD
static const char* GetCellColorName(Cell* cell) {
  if (cell->isMarkedBlack()) {
    return "black";
  }
  if (cell->isMarkedGray()) {
    return "gray";
  }
  return "white";
}
||||||| merged common ancestors
static const char*
GetCellColorName(Cell* cell)
{
    if (cell->isMarkedBlack()) {
        return "black";
    }
    if (cell->isMarkedGray()) {
        return "gray";
    }
    return "white";
}
=======
 private:
  void checkCell(Cell* cell) override;
  GCType gcType;
};
>>>>>>> upstream-releases

<<<<<<< HEAD
void HeapCheckTracerBase::dumpCellInfo(Cell* cell) {
  auto kind = cell->getTraceKind();
  JSObject* obj =
      kind == JS::TraceKind::Object ? static_cast<JSObject*>(cell) : nullptr;

  fprintf(stderr, "%s %s", GetCellColorName(cell), GCTraceKindToAscii(kind));
  if (obj) {
    fprintf(stderr, " %s", obj->getClass()->name);
  }
  fprintf(stderr, " %p", cell);
  if (obj) {
    fprintf(stderr, " (compartment %p)", obj->compartment());
  }
||||||| merged common ancestors
void
HeapCheckTracerBase::dumpCellInfo(Cell* cell)
{
    auto kind = cell->getTraceKind();
    JSObject* obj = kind == JS::TraceKind::Object ? static_cast<JSObject*>(cell) : nullptr;

    fprintf(stderr, "%s %s", GetCellColorName(cell), GCTraceKindToAscii(kind));
    if (obj) {
        fprintf(stderr, " %s", obj->getClass()->name);
    }
    fprintf(stderr, " %p", cell);
    if (obj) {
        fprintf(stderr, " (compartment %p)", obj->compartment());
    }
=======
CheckHeapTracer::CheckHeapTracer(JSRuntime* rt, GCType type)
    : HeapCheckTracerBase(rt, TraceWeakMapKeysValues), gcType(type) {}

inline static bool IsValidGCThingPointer(Cell* cell) {
  return (uintptr_t(cell) & CellAlignMask) == 0;
>>>>>>> upstream-releases
}

<<<<<<< HEAD
void HeapCheckTracerBase::dumpCellPath() {
  const char* name = contextName();
  for (int index = parentIndex; index != -1; index = stack[index].parentIndex) {
    const WorkItem& parent = stack[index];
    Cell* cell = parent.thing.asCell();
    fprintf(stderr, "  from ");
    dumpCellInfo(cell);
    fprintf(stderr, " %s edge\n", name);
    name = parent.name;
  }
  fprintf(stderr, "  from root %s\n", name);
||||||| merged common ancestors
void
HeapCheckTracerBase::dumpCellPath()
{
    const char* name = contextName();
    for (int index = parentIndex; index != -1; index = stack[index].parentIndex) {
        const WorkItem& parent = stack[index];
        Cell* cell = parent.thing.asCell();
        fprintf(stderr, "  from ");
        dumpCellInfo(cell);
        fprintf(stderr, " %s edge\n", name);
        name = parent.name;
    }
    fprintf(stderr, "  from root %s\n", name);
=======
void CheckHeapTracer::checkCell(Cell* cell) {
  // Moving
  if (!IsValidGCThingPointer(cell) ||
      ((gcType == GCType::Moving) && !IsGCThingValidAfterMovingGC(cell)) ||
      ((gcType == GCType::NonMoving) && cell->isForwarded())) {
    failures++;
    fprintf(stderr, "Bad pointer %p\n", cell);
    dumpCellPath();
  }
>>>>>>> upstream-releases
}

<<<<<<< HEAD
#endif  // defined(JSGC_HASH_TABLE_CHECKS) || defined(DEBUG)
||||||| merged common ancestors
#endif // defined(JSGC_HASH_TABLE_CHECKS) || defined(DEBUG)
=======
void CheckHeapTracer::check(AutoTraceSession& session) {
  if (!traceHeap(session)) {
    return;
  }

  if (failures) {
    fprintf(stderr, "Heap check: %zu failure(s)\n", failures);
  }
  MOZ_RELEASE_ASSERT(failures == 0);
}
>>>>>>> upstream-releases

void js::gc::CheckHeapAfterGC(JSRuntime* rt) {
  AutoTraceSession session(rt);
  CheckHeapTracer::GCType gcType;

<<<<<<< HEAD
class CheckHeapTracer final : public HeapCheckTracerBase {
 public:
  enum GCType { Moving, NonMoving };
||||||| merged common ancestors
class CheckHeapTracer final : public HeapCheckTracerBase
{
  public:
    enum GCType {
        Moving,
        NonMoving
    };
=======
  if (rt->gc.nursery().isEmpty()) {
    gcType = CheckHeapTracer::GCType::Moving;
  } else {
    gcType = CheckHeapTracer::GCType::NonMoving;
  }
>>>>>>> upstream-releases

<<<<<<< HEAD
  explicit CheckHeapTracer(JSRuntime* rt, GCType type);
  void check(AutoTraceSession& session);
||||||| merged common ancestors
    explicit CheckHeapTracer(JSRuntime* rt, GCType type);
    void check(AutoTraceSession& session);
=======
  CheckHeapTracer tracer(rt, gcType);
  tracer.check(session);
}
>>>>>>> upstream-releases

<<<<<<< HEAD
 private:
  void checkCell(Cell* cell) override;
  GCType gcType;
||||||| merged common ancestors
  private:
    void checkCell(Cell* cell) override;
    GCType gcType;
=======
class CheckGrayMarkingTracer final : public HeapCheckTracerBase {
 public:
  explicit CheckGrayMarkingTracer(JSRuntime* rt);
  bool check(AutoTraceSession& session);

 private:
  void checkCell(Cell* cell) override;
>>>>>>> upstream-releases
};

<<<<<<< HEAD
CheckHeapTracer::CheckHeapTracer(JSRuntime* rt, GCType type)
    : HeapCheckTracerBase(rt, TraceWeakMapKeysValues), gcType(type) {}

inline static bool IsValidGCThingPointer(Cell* cell) {
  return (uintptr_t(cell) & CellAlignMask) == 0;
}

void CheckHeapTracer::checkCell(Cell* cell) {
  // Moving
  if (!IsValidGCThingPointer(cell) ||
      ((gcType == GCType::Moving) && !IsGCThingValidAfterMovingGC(cell)) ||
      ((gcType == GCType::NonMoving) && cell->isForwarded())) {
    failures++;
    fprintf(stderr, "Bad pointer %p\n", cell);
    dumpCellPath();
  }
||||||| merged common ancestors
CheckHeapTracer::CheckHeapTracer(JSRuntime* rt, GCType type)
  : HeapCheckTracerBase(rt, TraceWeakMapKeysValues), gcType(type)
{}

inline static bool
IsValidGCThingPointer(Cell* cell)
{
    return (uintptr_t(cell) & CellAlignMask) == 0;
}

void
CheckHeapTracer::checkCell(Cell* cell)
{
    // Moving
    if (!IsValidGCThingPointer(cell) ||
        ((gcType == GCType::Moving) && !IsGCThingValidAfterMovingGC(cell)) ||
        ((gcType == GCType::NonMoving) && cell->isForwarded()))
    {
        failures++;
        fprintf(stderr, "Bad pointer %p\n", cell);
        dumpCellPath();
    }
=======
CheckGrayMarkingTracer::CheckGrayMarkingTracer(JSRuntime* rt)
    : HeapCheckTracerBase(rt, DoNotTraceWeakMaps) {
  // Weak gray->black edges are allowed.
  setTraceWeakEdges(false);
>>>>>>> upstream-releases
}

<<<<<<< HEAD
void CheckHeapTracer::check(AutoTraceSession& session) {
  if (!traceHeap(session)) {
    return;
  }
||||||| merged common ancestors
void
CheckHeapTracer::check(AutoTraceSession& session)
{
    if (!traceHeap(session)) {
        return;
    }
=======
void CheckGrayMarkingTracer::checkCell(Cell* cell) {
  Cell* parent = parentCell();
  if (!parent) {
    return;
  }
>>>>>>> upstream-releases

<<<<<<< HEAD
  if (failures) {
    fprintf(stderr, "Heap check: %zu failure(s)\n", failures);
  }
  MOZ_RELEASE_ASSERT(failures == 0);
}
||||||| merged common ancestors
    if (failures) {
        fprintf(stderr, "Heap check: %zu failure(s)\n", failures);
    }
    MOZ_RELEASE_ASSERT(failures == 0);
}
=======
  if (parent->isMarkedBlack() && cell->isMarkedGray()) {
    failures++;
>>>>>>> upstream-releases

<<<<<<< HEAD
void js::gc::CheckHeapAfterGC(JSRuntime* rt) {
  AutoTraceSession session(rt);
  CheckHeapTracer::GCType gcType;
||||||| merged common ancestors
void
js::gc::CheckHeapAfterGC(JSRuntime* rt)
{
    AutoTraceSession session(rt);
    CheckHeapTracer::GCType gcType;
=======
    fprintf(stderr, "Found black to gray edge to ");
    dumpCellInfo(cell);
    fprintf(stderr, "\n");
    dumpCellPath();
>>>>>>> upstream-releases

<<<<<<< HEAD
  if (rt->gc.nursery().isEmpty()) {
    gcType = CheckHeapTracer::GCType::Moving;
  } else {
    gcType = CheckHeapTracer::GCType::NonMoving;
  }

  CheckHeapTracer tracer(rt, gcType);
  tracer.check(session);
||||||| merged common ancestors
    if (rt->gc.nursery().isEmpty()) {
        gcType = CheckHeapTracer::GCType::Moving;
    } else {
        gcType = CheckHeapTracer::GCType::NonMoving;
    }

    CheckHeapTracer tracer(rt, gcType);
    tracer.check(session);
=======
#  ifdef DEBUG
    if (parent->is<JSObject>()) {
      fprintf(stderr, "\nSource: ");
      DumpObject(parent->as<JSObject>(), stderr);
    }
    if (cell->is<JSObject>()) {
      fprintf(stderr, "\nTarget: ");
      DumpObject(cell->as<JSObject>(), stderr);
    }
#  endif
  }
>>>>>>> upstream-releases
}

bool CheckGrayMarkingTracer::check(AutoTraceSession& session) {
  if (!traceHeap(session)) {
    return true;  // Ignore failure.
  }

  return failures == 0;
}

<<<<<<< HEAD
class CheckGrayMarkingTracer final : public HeapCheckTracerBase {
 public:
  explicit CheckGrayMarkingTracer(JSRuntime* rt);
  bool check(AutoTraceSession& session);
||||||| merged common ancestors
class CheckGrayMarkingTracer final : public HeapCheckTracerBase
{
  public:
    explicit CheckGrayMarkingTracer(JSRuntime* rt);
    bool check(AutoTraceSession& session);
=======
JS_FRIEND_API bool js::CheckGrayMarkingState(JSRuntime* rt) {
  MOZ_ASSERT(!JS::RuntimeHeapIsCollecting());
  MOZ_ASSERT(!rt->gc.isIncrementalGCInProgress());
  if (!rt->gc.areGrayBitsValid()) {
    return true;
  }
>>>>>>> upstream-releases

<<<<<<< HEAD
 private:
  void checkCell(Cell* cell) override;
};
||||||| merged common ancestors
  private:
    void checkCell(Cell* cell) override;
};
=======
  gcstats::AutoPhase ap(rt->gc.stats(), gcstats::PhaseKind::TRACE_HEAP);
  AutoTraceSession session(rt);
  CheckGrayMarkingTracer tracer(rt);
>>>>>>> upstream-releases

<<<<<<< HEAD
CheckGrayMarkingTracer::CheckGrayMarkingTracer(JSRuntime* rt)
    : HeapCheckTracerBase(rt, DoNotTraceWeakMaps) {
  // Weak gray->black edges are allowed.
  setTraceWeakEdges(false);
}

void CheckGrayMarkingTracer::checkCell(Cell* cell) {
  Cell* parent = parentCell();
  if (!parent) {
    return;
  }
||||||| merged common ancestors
CheckGrayMarkingTracer::CheckGrayMarkingTracer(JSRuntime* rt)
  : HeapCheckTracerBase(rt, DoNotTraceWeakMaps)
{
    // Weak gray->black edges are allowed.
    setTraceWeakEdges(false);
}

void
CheckGrayMarkingTracer::checkCell(Cell* cell)
{
    Cell* parent = parentCell();
    if (!parent) {
        return;
    }
=======
  return tracer.check(session);
}
>>>>>>> upstream-releases

<<<<<<< HEAD
  if (parent->isMarkedBlack() && cell->isMarkedGray()) {
    failures++;
||||||| merged common ancestors
    if (parent->isMarkedBlack() && cell->isMarkedGray()) {
        failures++;
=======
static Zone* GetCellZoneFromAnyThread(Cell* cell) {
  if (cell->is<JSObject>()) {
    return cell->as<JSObject>()->zoneFromAnyThread();
  }
>>>>>>> upstream-releases

<<<<<<< HEAD
    fprintf(stderr, "Found black to gray edge to ");
    dumpCellInfo(cell);
    fprintf(stderr, "\n");
    dumpCellPath();
||||||| merged common ancestors
        fprintf(stderr, "Found black to gray edge to ");
        dumpCellInfo(cell);
        fprintf(stderr, "\n");
        dumpCellPath();
=======
  if (cell->is<JSString>()) {
    return cell->as<JSString>()->zoneFromAnyThread();
  }
>>>>>>> upstream-releases

<<<<<<< HEAD
#ifdef DEBUG
    if (cell->is<JSObject>()) {
      fprintf(stderr, "\n");
      DumpObject(cell->as<JSObject>(), stderr);
    }
#endif
  }
||||||| merged common ancestors
#ifdef DEBUG
        if (cell->is<JSObject>()) {
            fprintf(stderr, "\n");
            DumpObject(cell->as<JSObject>(), stderr);
        }
#endif
    }
=======
  return cell->asTenured().zoneFromAnyThread();
>>>>>>> upstream-releases
}

<<<<<<< HEAD
bool CheckGrayMarkingTracer::check(AutoTraceSession& session) {
  if (!traceHeap(session)) {
    return true;  // Ignore failure.
  }
||||||| merged common ancestors
bool
CheckGrayMarkingTracer::check(AutoTraceSession& session)
{
    if (!traceHeap(session)) {
        return true; // Ignore failure.
    }
=======
static JSObject* MaybeGetDelegate(Cell* cell) {
  if (!cell->is<JSObject>()) {
    return nullptr;
  }
>>>>>>> upstream-releases

<<<<<<< HEAD
  return failures == 0;
||||||| merged common ancestors
    return failures == 0;
=======
  JSObject* object = cell->as<JSObject>();
  return js::UncheckedUnwrapWithoutExpose(object);
>>>>>>> upstream-releases
}

<<<<<<< HEAD
JS_FRIEND_API bool js::CheckGrayMarkingState(JSRuntime* rt) {
  MOZ_ASSERT(!JS::RuntimeHeapIsCollecting());
  MOZ_ASSERT(!rt->gc.isIncrementalGCInProgress());
  if (!rt->gc.areGrayBitsValid()) {
    return true;
  }

  gcstats::AutoPhase ap(rt->gc.stats(), gcstats::PhaseKind::TRACE_HEAP);
  AutoTraceSession session(rt);
  CheckGrayMarkingTracer tracer(rt);

  return tracer.check(session);
||||||| merged common ancestors
JS_FRIEND_API(bool)
js::CheckGrayMarkingState(JSRuntime* rt)
{
    MOZ_ASSERT(!JS::RuntimeHeapIsCollecting());
    MOZ_ASSERT(!rt->gc.isIncrementalGCInProgress());
    if (!rt->gc.areGrayBitsValid()) {
        return true;
    }

    gcstats::AutoPhase ap(rt->gc.stats(), gcstats::PhaseKind::TRACE_HEAP);
    AutoTraceSession session(rt);
    CheckGrayMarkingTracer tracer(rt);

    return tracer.check(session);
=======
bool js::gc::CheckWeakMapEntryMarking(const WeakMapBase* map, Cell* key,
                                      Cell* value) {
  bool ok = true;

  Zone* zone = map->zone();
  MOZ_ASSERT(CurrentThreadCanAccessZone(zone));
  MOZ_ASSERT(zone->isGCMarking());

  JSObject* object = map->memberOf;
  MOZ_ASSERT_IF(object, object->zone() == zone);

  // Debugger weak maps can have keys in different zones.
  Zone* keyZone = GetCellZoneFromAnyThread(key);
  MOZ_ASSERT_IF(!map->allowKeysInOtherZones(),
                keyZone == zone || keyZone->isAtomsZone());

  Zone* valueZone = GetCellZoneFromAnyThread(value);
  MOZ_ASSERT(valueZone == zone || valueZone->isAtomsZone());

  CellColor mapColor =
      map->markColor == MarkColor::Black ? CellColor::Black : CellColor::Gray;
  if (object && GetCellColor(object) != mapColor) {
    fprintf(stderr, "WeakMap object is marked differently to the map\n");
    fprintf(stderr, "(map %p is %s, object %p is %s)\n", map,
            CellColorName(mapColor), object,
            CellColorName(GetCellColor(object)));
    ok = false;
  }

  CellColor keyColor = GetCellColor(key);

  // Values belonging to other runtimes or in uncollected zones are treated as
  // black.
  CellColor valueColor = CellColor::Black;
  if (value->runtimeFromAnyThread() == zone->runtimeFromAnyThread() &&
      valueZone->isGCMarking()) {
    valueColor = GetCellColor(value);
  }

  if (valueColor < ExpectedWeakMapValueColor(mapColor, keyColor)) {
    fprintf(stderr, "WeakMap value is less marked than map and key\n");
    fprintf(stderr, "(map %p is %s, key %p is %s, value %p is %s)\n", map,
            CellColorName(mapColor), key, CellColorName(keyColor), value,
            CellColorName(valueColor));
    ok = false;
  }

  // Debugger weak maps map have keys in zones that are not or are
  // no longer collecting. We can't make guarantees about the mark
  // state of these keys.
  if (map->allowKeysInOtherZones() &&
      !(keyZone->isGCMarking() || keyZone->isGCSweeping())) {
    return ok;
  }

  JSObject* delegate = MaybeGetDelegate(key);
  if (!delegate) {
    return ok;
  }

  CellColor delegateColor;
  if (delegate->zone()->isGCMarking() || delegate->zone()->isGCSweeping()) {
    delegateColor = GetCellColor(delegate);
  } else {
    // IsMarked() assumes cells in uncollected zones are marked.
    delegateColor = CellColor::Black;
  }

  if (keyColor < ExpectedWeakMapKeyColor(mapColor, delegateColor)) {
    fprintf(stderr, "WeakMap key is less marked than map and delegate\n");
    fprintf(stderr, "(map %p is %s, delegate %p is %s, key %p is %s)\n", map,
            CellColorName(mapColor), delegate, CellColorName(delegateColor),
            key, CellColorName(keyColor));
    ok = false;
  }

  return ok;
>>>>>>> upstream-releases
}

#endif  // defined(JS_GC_ZEAL) || defined(DEBUG)
