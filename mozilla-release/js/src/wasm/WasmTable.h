/* -*- Mode: C++; tab-width: 8; indent-tabs-mode: nil; c-basic-offset: 2 -*-
 * vim: set ts=8 sts=2 et sw=2 tw=80:
 *
 * Copyright 2016 Mozilla Foundation
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#ifndef wasm_table_h
#define wasm_table_h

#include "gc/Policy.h"
#include "wasm/WasmCode.h"

namespace js {
namespace wasm {

// A Table is an indexable array of opaque values. Tables are first-class
// stateful objects exposed to WebAssembly. asm.js also uses Tables to represent
// its homogeneous function-pointer tables.
<<<<<<< HEAD
//
// A table of AnyFunction holds FunctionTableElems, which are (instance*,index)
// pairs, where the instance must be traced.
//
// A table of AnyRef holds JSObject pointers, which must be traced.

typedef GCVector<JS::Heap<JSObject*>, 0, SystemAllocPolicy> TableAnyRefVector;

class Table : public ShareableBase<Table> {
  using InstanceSet = JS::WeakCache<GCHashSet<
      ReadBarrieredWasmInstanceObject,
      MovableCellHasher<ReadBarrieredWasmInstanceObject>, SystemAllocPolicy>>;
  using UniqueAnyFuncArray = UniquePtr<FunctionTableElem[], JS::FreePolicy>;

  ReadBarrieredWasmTableObject maybeObject_;
  InstanceSet observers_;
  UniqueAnyFuncArray functions_;  // either functions_ has data
  TableAnyRefVector objects_;     //   or objects_, but not both
  const TableKind kind_;
  uint32_t length_;
  const Maybe<uint32_t> maximum_;

  template <class>
  friend struct js::MallocProvider;
  Table(JSContext* cx, const TableDesc& td, HandleWasmTableObject maybeObject,
        UniqueAnyFuncArray functions);
  Table(JSContext* cx, const TableDesc& td, HandleWasmTableObject maybeObject,
        TableAnyRefVector&& objects);

  void tracePrivate(JSTracer* trc);
  friend class js::WasmTableObject;

 public:
  static RefPtr<Table> create(JSContext* cx, const TableDesc& desc,
                              HandleWasmTableObject maybeObject);
  void trace(JSTracer* trc);

  TableKind kind() const { return kind_; }
  bool isTypedFunction() const { return kind_ == TableKind::TypedFunction; }
  bool isFunction() const {
    return kind_ == TableKind::AnyFunction || kind_ == TableKind::TypedFunction;
  }
  uint32_t length() const { return length_; }
  Maybe<uint32_t> maximum() const { return maximum_; }

  // Only for function values.  Raw pointer to the table.
  uint8_t* functionBase() const;

  // get/setAnyFunc is allowed only on table-of-anyfunc.
  // get/setAnyRef is allowed only on table-of-anyref.
  // setNull is allowed on either.
  const FunctionTableElem& getAnyFunc(uint32_t index) const;
  void setAnyFunc(uint32_t index, void* code, const Instance* instance);

  JSObject* getAnyRef(uint32_t index) const;
  void setAnyRef(uint32_t index, JSObject* obj);

  void setNull(uint32_t index);

  // Copy entry from |srcTable| at |srcIndex| to this table at |dstIndex|.
  // Used by table.copy.
  void copy(const Table& srcTable, uint32_t dstIndex, uint32_t srcIndex);

  // grow() returns (uint32_t)-1 if it could not grow.
  uint32_t grow(uint32_t delta, JSContext* cx);
  bool movingGrowable() const;
  bool addMovingGrowObserver(JSContext* cx, WasmInstanceObject* instance);

  // about:memory reporting:

  size_t sizeOfExcludingThis(MallocSizeOf mallocSizeOf) const;
||||||| merged common ancestors

class Table : public ShareableBase<Table>
{
    using InstanceSet = JS::WeakCache<GCHashSet<ReadBarrieredWasmInstanceObject,
                                                MovableCellHasher<ReadBarrieredWasmInstanceObject>,
                                                SystemAllocPolicy>>;
    using UniqueByteArray = UniquePtr<uint8_t[], JS::FreePolicy>;

    ReadBarrieredWasmTableObject maybeObject_;
    InstanceSet                  observers_;
    UniqueByteArray              array_;
    const TableKind              kind_;
    uint32_t                     length_;
    const Maybe<uint32_t>        maximum_;
    const bool                   external_;

    template <class> friend struct js::MallocProvider;
    Table(JSContext* cx, const TableDesc& td, HandleWasmTableObject maybeObject,
          UniqueByteArray array);

    void tracePrivate(JSTracer* trc);
    friend class js::WasmTableObject;

  public:
    static RefPtr<Table> create(JSContext* cx, const TableDesc& desc,
                                HandleWasmTableObject maybeObject);
    void trace(JSTracer* trc);

    bool external() const { return external_; }
    bool isTypedFunction() const { return kind_ == TableKind::TypedFunction; }
    uint32_t length() const { return length_; }
    Maybe<uint32_t> maximum() const { return maximum_; }
    uint8_t* base() const { return array_.get(); }

    // All table updates must go through set() or setNull().

    void** internalArray() const;
    ExternalTableElem* externalArray() const;
    void set(uint32_t index, void* code, const Instance* instance);
    void setNull(uint32_t index);

    // Copy entry at |srcIndex| to |dstIndex|.  Used by table.copy.
    void copy(uint32_t dstIndex, uint32_t srcIndex);

    uint32_t grow(uint32_t delta, JSContext* cx);
    bool movingGrowable() const;
    bool addMovingGrowObserver(JSContext* cx, WasmInstanceObject* instance);

    // about:memory reporting:

    size_t sizeOfExcludingThis(MallocSizeOf mallocSizeOf) const;
=======
//
// A table of FuncRef holds FunctionTableElems, which are (instance*,index)
// pairs, where the instance must be traced.
//
// A table of AnyRef holds JSObject pointers, which must be traced.

// TODO/AnyRef-boxing: With boxed immediates and strings, JSObject* is no longer
// the most appropriate representation for Cell::anyref.
STATIC_ASSERT_ANYREF_IS_JSOBJECT;

typedef GCVector<HeapPtr<JSObject*>, 0, SystemAllocPolicy> TableAnyRefVector;

class Table : public ShareableBase<Table> {
  using InstanceSet =
      JS::WeakCache<GCHashSet<WeakHeapPtrWasmInstanceObject,
                              MovableCellHasher<WeakHeapPtrWasmInstanceObject>,
                              SystemAllocPolicy>>;
  using UniqueFuncRefArray = UniquePtr<FunctionTableElem[], JS::FreePolicy>;

  WeakHeapPtrWasmTableObject maybeObject_;
  InstanceSet observers_;
  UniqueFuncRefArray functions_;  // either functions_ has data
  TableAnyRefVector objects_;     //   or objects_, but not both
  const TableKind kind_;
  uint32_t length_;
  const Maybe<uint32_t> maximum_;

  template <class>
  friend struct js::MallocProvider;
  Table(JSContext* cx, const TableDesc& td, HandleWasmTableObject maybeObject,
        UniqueFuncRefArray functions);
  Table(JSContext* cx, const TableDesc& td, HandleWasmTableObject maybeObject,
        TableAnyRefVector&& objects);

  void tracePrivate(JSTracer* trc);
  friend class js::WasmTableObject;

 public:
  static RefPtr<Table> create(JSContext* cx, const TableDesc& desc,
                              HandleWasmTableObject maybeObject);
  void trace(JSTracer* trc);

  TableKind kind() const { return kind_; }
  bool isFunction() const {
    return kind_ == TableKind::FuncRef || kind_ == TableKind::AsmJS;
  }
  uint32_t length() const { return length_; }
  Maybe<uint32_t> maximum() const { return maximum_; }

  // Only for function values.  Raw pointer to the table.
  uint8_t* functionBase() const;

  // set/get/fillFuncRef is allowed only on table-of-funcref.
  // get/fillAnyRef is allowed only on table-of-anyref.
  // setNull is allowed on either.

  const FunctionTableElem& getFuncRef(uint32_t index) const;
  bool getFuncRef(JSContext* cx, uint32_t index,
                  MutableHandleFunction fun) const;
  void setFuncRef(uint32_t index, void* code, const Instance* instance);
  void fillFuncRef(uint32_t index, uint32_t fillCount, AnyRef ref,
                   JSContext* cx);

  AnyRef getAnyRef(uint32_t index) const;
  void fillAnyRef(uint32_t index, uint32_t fillCount, AnyRef ref);

  void setNull(uint32_t index);

  // Copy entry from |srcTable| at |srcIndex| to this table at |dstIndex|.
  // Used by table.copy.
  void copy(const Table& srcTable, uint32_t dstIndex, uint32_t srcIndex);

  // grow() returns (uint32_t)-1 if it could not grow.
  uint32_t grow(uint32_t delta, JSContext* cx);
  bool movingGrowable() const;
  bool addMovingGrowObserver(JSContext* cx, WasmInstanceObject* instance);

  // about:memory reporting:

  size_t sizeOfExcludingThis(MallocSizeOf mallocSizeOf) const;

  size_t gcMallocBytes() const;
>>>>>>> upstream-releases
};

typedef RefPtr<Table> SharedTable;
typedef Vector<SharedTable, 0, SystemAllocPolicy> SharedTableVector;

}  // namespace wasm
}  // namespace js

#endif  // wasm_table_h
