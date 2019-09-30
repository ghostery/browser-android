/* -*- Mode: C++; tab-width: 8; indent-tabs-mode: nil; c-basic-offset: 2 -*-
 * vim: set ts=8 sts=2 et sw=2 tw=80:
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/. */

#include "builtin/Stream.h"

#include "js/Stream.h"

#include "gc/Heap.h"
<<<<<<< HEAD
#include "vm/Interpreter.h"
||||||| merged common ancestors
=======
#include "js/ArrayBuffer.h"  // JS::NewArrayBuffer
#include "js/PropertySpec.h"
#include "vm/Interpreter.h"
>>>>>>> upstream-releases
#include "vm/JSContext.h"
#include "vm/SelfHosting.h"

#include "vm/Compartment-inl.h"
#include "vm/List-inl.h"
#include "vm/NativeObject-inl.h"

using namespace js;

enum ReaderType { ReaderType_Default, ReaderType_BYOB };

template <class T>
bool Is(const HandleValue v) {
  return v.isObject() && v.toObject().is<T>();
}

template <class T>
bool IsMaybeWrapped(const HandleValue v) {
  return v.isObject() && v.toObject().canUnwrapAs<T>();
}

JS::ReadableStreamMode ReadableStream::mode() const {
  ReadableStreamController* controller = this->controller();
  if (controller->is<ReadableStreamDefaultController>()) {
    return JS::ReadableStreamMode::Default;
  }
  return controller->as<ReadableByteStreamController>().hasExternalSource()
             ? JS::ReadableStreamMode::ExternalSource
             : JS::ReadableStreamMode::Byte;
}

/**
 * Returns the stream associated with the given reader.
 */
static MOZ_MUST_USE ReadableStream* UnwrapStreamFromReader(
    JSContext* cx, Handle<ReadableStreamReader*> reader) {
  MOZ_ASSERT(reader->hasStream());
  return UnwrapInternalSlot<ReadableStream>(cx, reader,
                                            ReadableStreamReader::Slot_Stream);
}

/**
 * Returns the reader associated with the given stream.
 *
 * Must only be called on ReadableStreams that already have a reader
 * associated with them.
 *
 * If the reader is a wrapper, it will be unwrapped, so the result might not be
 * an object from the currently active compartment.
 */
static MOZ_MUST_USE ReadableStreamReader* UnwrapReaderFromStream(
    JSContext* cx, Handle<ReadableStream*> stream) {
  return UnwrapInternalSlot<ReadableStreamReader>(cx, stream,
                                                  ReadableStream::Slot_Reader);
}

static MOZ_MUST_USE ReadableStreamReader* UnwrapReaderFromStreamNoThrow(
    ReadableStream* stream) {
  JSObject* readerObj =
      &stream->getFixedSlot(ReadableStream::Slot_Reader).toObject();
  if (IsProxy(readerObj)) {
    if (JS_IsDeadWrapper(readerObj)) {
      return nullptr;
    }

<<<<<<< HEAD
    readerObj = CheckedUnwrap(readerObj);
    if (!readerObj) {
      return nullptr;
||||||| merged common ancestors
inline static MOZ_MUST_USE JSFunction*
NewHandler(JSContext* cx, Native handler, HandleObject target)
{
    RootedAtom funName(cx, cx->names().empty);
    RootedFunction handlerFun(cx, NewNativeFunction(cx, handler, 0, funName,
                                                    gc::AllocKind::FUNCTION_EXTENDED,
                                                    GenericObject));
    if (!handlerFun) {
        return nullptr;
=======
    readerObj = readerObj->maybeUnwrapAs<ReadableStreamReader>();
    if (!readerObj) {
      return nullptr;
>>>>>>> upstream-releases
    }
  }

  return &readerObj->as<ReadableStreamReader>();
}

<<<<<<< HEAD
constexpr size_t StreamHandlerFunctionSlot_Target = 0;

inline static MOZ_MUST_USE JSFunction* NewHandler(JSContext* cx, Native handler,
                                                  HandleObject target) {
  cx->check(target);
||||||| merged common ancestors
inline static MOZ_MUST_USE bool
ResetQueue(JSContext* cx, Handle<ReadableStreamController*> container);

inline static MOZ_MUST_USE bool
InvokeOrNoop(JSContext* cx, HandleValue O, HandlePropertyName P, HandleValue arg,
             MutableHandleValue rval);

static MOZ_MUST_USE JSObject*
PromiseInvokeOrNoop(JSContext* cx, HandleValue O, HandlePropertyName P, HandleValue arg);
=======
constexpr size_t StreamHandlerFunctionSlot_Target = 0;
>>>>>>> upstream-releases

<<<<<<< HEAD
  RootedAtom funName(cx, cx->names().empty);
  RootedFunction handlerFun(
      cx, NewNativeFunction(cx, handler, 0, funName,
                            gc::AllocKind::FUNCTION_EXTENDED, GenericObject));
  if (!handlerFun) {
    return nullptr;
  }
  handlerFun->setExtendedSlot(StreamHandlerFunctionSlot_Target,
                              ObjectValue(*target));
  return handlerFun;
||||||| merged common ancestors
static MOZ_MUST_USE JSObject*
PromiseRejectedWithPendingError(JSContext* cx) {
    // Not much we can do about uncatchable exceptions, just bail.
    RootedValue exn(cx);
    if (!GetAndClearException(cx, &exn)) {
        return nullptr;
    }
    return PromiseObject::unforgeableReject(cx, exn);
}

static void
ReportArgTypeError(JSContext* cx, const char* funName, const char* expectedType, HandleValue arg)
{
    UniqueChars bytes = DecompileValueGenerator(cx, JSDVG_SEARCH_STACK, arg, nullptr);
    if (!bytes) {
        return;
    }

    JS_ReportErrorNumberUTF8(cx, GetErrorMessage, nullptr, JSMSG_NOT_EXPECTED_TYPE, funName,
                             expectedType, bytes.get());
=======
inline static MOZ_MUST_USE JSFunction* NewHandler(JSContext* cx, Native handler,
                                                  HandleObject target) {
  cx->check(target);

  HandlePropertyName funName = cx->names().empty;
  RootedFunction handlerFun(
      cx, NewNativeFunction(cx, handler, 0, funName,
                            gc::AllocKind::FUNCTION_EXTENDED, GenericObject));
  if (!handlerFun) {
    return nullptr;
  }
  handlerFun->setExtendedSlot(StreamHandlerFunctionSlot_Target,
                              ObjectValue(*target));
  return handlerFun;
>>>>>>> upstream-releases
}

/**
 * Helper for handler functions that "close over" a value that is always a
 * direct reference to an object of class T, never a wrapper.
 */
template <class T>
inline static MOZ_MUST_USE T* TargetFromHandler(CallArgs& args) {
  JSFunction& func = args.callee().as<JSFunction>();
  return &func.getExtendedSlot(StreamHandlerFunctionSlot_Target)
              .toObject()
              .as<T>();
}

inline static MOZ_MUST_USE bool ResetQueue(
    JSContext* cx, Handle<ReadableStreamController*> unwrappedContainer);

inline static MOZ_MUST_USE bool InvokeOrNoop(JSContext* cx, HandleValue O,
                                             HandlePropertyName P,
                                             HandleValue arg,
                                             MutableHandleValue rval);

static MOZ_MUST_USE JSObject* PromiseRejectedWithPendingError(JSContext* cx) {
  RootedValue exn(cx);
  if (!cx->isExceptionPending() || !GetAndClearException(cx, &exn)) {
    // Uncatchable error. This happens when a slow script is killed or a
    // worker is terminated. Propagate the uncatchable error. This will
    // typically kill off the calling asynchronous process: the caller
    // can't hook its continuation to the new rejected promise.
    return nullptr;
  }
  return PromiseObject::unforgeableReject(cx, exn);
}

static MOZ_MUST_USE bool ReturnPromiseRejectedWithPendingError(
    JSContext* cx, const CallArgs& args) {
  JSObject* promise = PromiseRejectedWithPendingError(cx);
  if (!promise) {
    return false;
  }

  args.rval().setObject(*promise);
  return true;
}

/**
 * Creates a NativeObject to be used as a list and stores it on the given
 * container at the given fixed slot offset.
 */
inline static MOZ_MUST_USE bool SetNewList(
    JSContext* cx, HandleNativeObject unwrappedContainer, uint32_t slot) {
  AutoRealm ar(cx, unwrappedContainer);
  ListObject* list = ListObject::create(cx);
  if (!list) {
    return false;
  }
  unwrappedContainer->setFixedSlot(slot, ObjectValue(*list));
  return true;
}

#if 0  // disable user-defined byte streams

class ByteStreamChunk : public NativeObject
{
  private:
    enum Slots {
        Slot_Buffer = 0,
        Slot_ByteOffset,
        Slot_ByteLength,
        SlotCount
    };

  public:
    static const Class class_;

    ArrayBufferObject* buffer() {
        return &getFixedSlot(Slot_Buffer).toObject().as<ArrayBufferObject>();
    }
    uint32_t byteOffset() { return getFixedSlot(Slot_ByteOffset).toInt32(); }
    void SetByteOffset(uint32_t offset) {
        setFixedSlot(Slot_ByteOffset, Int32Value(offset));
    }
    uint32_t byteLength() { return getFixedSlot(Slot_ByteLength).toInt32(); }
    void SetByteLength(uint32_t length) {
        setFixedSlot(Slot_ByteLength, Int32Value(length));
    }

    static ByteStreamChunk* create(JSContext* cx, HandleObject buffer, uint32_t byteOffset,
                                   uint32_t byteLength)
    {
        Rooted<ByteStreamChunk*> chunk(cx, NewBuiltinClassInstance<ByteStreamChunk>(cx));
        if (!chunk) {
            return nullptr;
        }

        chunk->setFixedSlot(Slot_Buffer, ObjectValue(*buffer));
        chunk->setFixedSlot(Slot_ByteOffset, Int32Value(byteOffset));
        chunk->setFixedSlot(Slot_ByteLength, Int32Value(byteLength));
        return chunk;
    }
};

const Class ByteStreamChunk::class_ = {
    "ByteStreamChunk",
    JSCLASS_HAS_RESERVED_SLOTS(SlotCount)
};

#endif  // user-defined byte streams

class PullIntoDescriptor : public NativeObject {
 private:
  enum Slots {
    Slot_buffer,
    Slot_ByteOffset,
    Slot_ByteLength,
    Slot_BytesFilled,
    Slot_ElementSize,
    Slot_Ctor,
    Slot_ReaderType,
    SlotCount
  };

 public:
  static const Class class_;

  ArrayBufferObject* buffer() {
    return &getFixedSlot(Slot_buffer).toObject().as<ArrayBufferObject>();
  }
  void setBuffer(ArrayBufferObject* buffer) {
    setFixedSlot(Slot_buffer, ObjectValue(*buffer));
  }
  JSObject* ctor() { return getFixedSlot(Slot_Ctor).toObjectOrNull(); }
  uint32_t byteOffset() const {
    return getFixedSlot(Slot_ByteOffset).toInt32();
  }
  uint32_t byteLength() const {
    return getFixedSlot(Slot_ByteLength).toInt32();
  }
  uint32_t bytesFilled() const {
    return getFixedSlot(Slot_BytesFilled).toInt32();
  }
  void setBytesFilled(int32_t bytes) {
    setFixedSlot(Slot_BytesFilled, Int32Value(bytes));
  }
  uint32_t elementSize() const {
    return getFixedSlot(Slot_ElementSize).toInt32();
  }
  uint32_t readerType() const {
    return getFixedSlot(Slot_ReaderType).toInt32();
  }

  static PullIntoDescriptor* create(JSContext* cx,
                                    HandleArrayBufferObject buffer,
                                    uint32_t byteOffset, uint32_t byteLength,
                                    uint32_t bytesFilled, uint32_t elementSize,
                                    HandleObject ctor, uint32_t readerType) {
    Rooted<PullIntoDescriptor*> descriptor(
        cx, NewBuiltinClassInstance<PullIntoDescriptor>(cx));
    if (!descriptor) {
      return nullptr;
    }

    descriptor->setFixedSlot(Slot_buffer, ObjectValue(*buffer));
    descriptor->setFixedSlot(Slot_Ctor, ObjectOrNullValue(ctor));
    descriptor->setFixedSlot(Slot_ByteOffset, Int32Value(byteOffset));
    descriptor->setFixedSlot(Slot_ByteLength, Int32Value(byteLength));
    descriptor->setFixedSlot(Slot_BytesFilled, Int32Value(bytesFilled));
    descriptor->setFixedSlot(Slot_ElementSize, Int32Value(elementSize));
    descriptor->setFixedSlot(Slot_ReaderType, Int32Value(readerType));
    return descriptor;
  }
};

const Class PullIntoDescriptor::class_ = {
    "PullIntoDescriptor", JSCLASS_HAS_RESERVED_SLOTS(SlotCount)};

class QueueEntry : public NativeObject {
 private:
  enum Slots { Slot_Value = 0, Slot_Size, SlotCount };

 public:
  static const Class class_;

  Value value() { return getFixedSlot(Slot_Value); }
  double size() { return getFixedSlot(Slot_Size).toNumber(); }

  static QueueEntry* create(JSContext* cx, HandleValue value, double size) {
    Rooted<QueueEntry*> entry(cx, NewBuiltinClassInstance<QueueEntry>(cx));
    if (!entry) {
      return nullptr;
    }

    entry->setFixedSlot(Slot_Value, value);
    entry->setFixedSlot(Slot_Size, NumberValue(size));

    return entry;
  }
};

const Class QueueEntry::class_ = {"QueueEntry",
                                  JSCLASS_HAS_RESERVED_SLOTS(SlotCount)};

/**
 * TeeState objects implement the local variables in Streams spec 3.3.9
 * ReadableStreamTee, which are accessed by several algorithms.
 */
class TeeState : public NativeObject {
 public:
  /**
   * Memory layout for TeeState instances.
   *
   * The Reason1 and Reason2 slots store opaque values, which might be
   * wrapped objects from other compartments. Since we don't treat them as
   * objects in Streams-specific code, we don't have to worry about that
   * apart from ensuring that the values are properly wrapped before storing
   * them.
   *
   * CancelPromise is always created in TeeState::create below, so is
   * guaranteed to be in the same compartment as the TeeState instance
   * itself.
   *
   * Stream can be from another compartment. It is automatically wrapped
   * before storing it and unwrapped upon retrieval. That means that
   * TeeState consumers need to be able to deal with unwrapped
   * ReadableStream instances from non-current compartments.
   *
   * Branch1 and Branch2 are always created in the same compartment as the
   * TeeState instance, so cannot be from another compartment.
   */
  enum Slots {
    Slot_Flags = 0,
    Slot_Reason1,
    Slot_Reason2,
    Slot_CancelPromise,
    Slot_Stream,
    Slot_Branch1,
    Slot_Branch2,
    SlotCount
  };

 private:
  enum Flags {
    Flag_ClosedOrErrored = 1 << 0,
    Flag_Canceled1 = 1 << 1,
    Flag_Canceled2 = 1 << 2,
    Flag_CloneForBranch2 = 1 << 3,
  };
  uint32_t flags() const { return getFixedSlot(Slot_Flags).toInt32(); }
  void setFlags(uint32_t flags) { setFixedSlot(Slot_Flags, Int32Value(flags)); }

 public:
  static const Class class_;

  bool cloneForBranch2() const { return flags() & Flag_CloneForBranch2; }

  bool closedOrErrored() const { return flags() & Flag_ClosedOrErrored; }
  void setClosedOrErrored() {
    MOZ_ASSERT(!(flags() & Flag_ClosedOrErrored));
    setFlags(flags() | Flag_ClosedOrErrored);
  }

  bool canceled1() const { return flags() & Flag_Canceled1; }
  void setCanceled1(HandleValue reason) {
    MOZ_ASSERT(!(flags() & Flag_Canceled1));
    setFlags(flags() | Flag_Canceled1);
    setFixedSlot(Slot_Reason1, reason);
  }

  bool canceled2() const { return flags() & Flag_Canceled2; }
  void setCanceled2(HandleValue reason) {
    MOZ_ASSERT(!(flags() & Flag_Canceled2));
    setFlags(flags() | Flag_Canceled2);
    setFixedSlot(Slot_Reason2, reason);
  }

  Value reason1() const {
    MOZ_ASSERT(canceled1());
    return getFixedSlot(Slot_Reason1);
  }

  Value reason2() const {
    MOZ_ASSERT(canceled2());
    return getFixedSlot(Slot_Reason2);
  }

  PromiseObject* cancelPromise() {
    return &getFixedSlot(Slot_CancelPromise).toObject().as<PromiseObject>();
  }

  ReadableStreamDefaultController* branch1() {
    ReadableStreamDefaultController* controller =
        &getFixedSlot(Slot_Branch1)
             .toObject()
             .as<ReadableStreamDefaultController>();
    MOZ_ASSERT(controller->isTeeBranch1());
    return controller;
  }
  void setBranch1(ReadableStreamDefaultController* controller) {
    MOZ_ASSERT(controller->isTeeBranch1());
    setFixedSlot(Slot_Branch1, ObjectValue(*controller));
  }

  ReadableStreamDefaultController* branch2() {
    ReadableStreamDefaultController* controller =
        &getFixedSlot(Slot_Branch2)
             .toObject()
             .as<ReadableStreamDefaultController>();
    MOZ_ASSERT(controller->isTeeBranch2());
    return controller;
  }
  void setBranch2(ReadableStreamDefaultController* controller) {
    MOZ_ASSERT(controller->isTeeBranch2());
    setFixedSlot(Slot_Branch2, ObjectValue(*controller));
  }

  static TeeState* create(JSContext* cx,
                          Handle<ReadableStream*> unwrappedStream) {
    Rooted<TeeState*> state(cx, NewBuiltinClassInstance<TeeState>(cx));
    if (!state) {
      return nullptr;
    }

    Rooted<PromiseObject*> cancelPromise(
        cx, PromiseObject::createSkippingExecutor(cx));
    if (!cancelPromise) {
      return nullptr;
    }

    state->setFixedSlot(Slot_Flags, Int32Value(0));
    state->setFixedSlot(Slot_CancelPromise, ObjectValue(*cancelPromise));
    RootedObject wrappedStream(cx, unwrappedStream);
    if (!cx->compartment()->wrap(cx, &wrappedStream)) {
      return nullptr;
    }
    state->setFixedSlot(Slot_Stream, ObjectValue(*wrappedStream));

    return state;
  }
};

const Class TeeState::class_ = {"TeeState",
                                JSCLASS_HAS_RESERVED_SLOTS(SlotCount)};

<<<<<<< HEAD
#define CLASS_SPEC(cls, nCtorArgs, nSlots, specFlags, classFlags, classOps) \
  const ClassSpec cls::classSpec_ = {                                       \
      GenericCreateConstructor<cls::constructor, nCtorArgs,                 \
                               gc::AllocKind::FUNCTION>,                    \
      GenericCreatePrototype<cls>,                                          \
      nullptr,                                                              \
      nullptr,                                                              \
      cls##_methods,                                                        \
      cls##_properties,                                                     \
      nullptr,                                                              \
      specFlags};                                                           \
                                                                            \
  const Class cls::class_ = {#cls,                                          \
                             JSCLASS_HAS_RESERVED_SLOTS(nSlots) |           \
                                 JSCLASS_HAS_CACHED_PROTO(JSProto_##cls) |  \
                                 classFlags,                                \
                             classOps, &cls::classSpec_};                   \
                                                                            \
  const Class cls::protoClass_ = {"object",                                 \
                                  JSCLASS_HAS_CACHED_PROTO(JSProto_##cls),  \
                                  JS_NULL_CLASS_OPS, &cls::classSpec_};

/*** 3.2. Class ReadableStream **********************************************/
||||||| merged common ancestors
    PromiseObject* promise() {
        return &getFixedSlot(Slot_Promise).toObject().as<PromiseObject>();
    }
=======
#define CLASS_SPEC(cls, nCtorArgs, nSlots, specFlags, classFlags, classOps) \
  const ClassSpec cls::classSpec_ = {                                       \
      GenericCreateConstructor<cls::constructor, nCtorArgs,                 \
                               gc::AllocKind::FUNCTION>,                    \
      GenericCreatePrototype<cls>,                                          \
      nullptr,                                                              \
      nullptr,                                                              \
      cls##_methods,                                                        \
      cls##_properties,                                                     \
      nullptr,                                                              \
      specFlags};                                                           \
                                                                            \
  const Class cls::class_ = {#cls,                                          \
                             JSCLASS_HAS_RESERVED_SLOTS(nSlots) |           \
                                 JSCLASS_HAS_CACHED_PROTO(JSProto_##cls) |  \
                                 classFlags,                                \
                             classOps, &cls::classSpec_};                   \
                                                                            \
  const Class cls::protoClass_ = {"object",                                 \
                                  JSCLASS_HAS_CACHED_PROTO(JSProto_##cls),  \
                                  JS_NULL_CLASS_OPS, &cls::classSpec_};

/*** 3.2. Class ReadableStream **********************************************/

static MOZ_MUST_USE bool SetUpExternalReadableByteStreamController(
    JSContext* cx, Handle<ReadableStream*> stream,
    JS::ReadableStreamUnderlyingSource* source);

ReadableStream* ReadableStream::createExternalSourceStream(
    JSContext* cx, JS::ReadableStreamUnderlyingSource* source,
    void* nsISupportsObject_alreadyAddreffed /* = nullptr */,
    HandleObject proto /* = nullptr */) {
  Rooted<ReadableStream*> stream(
      cx, create(cx, nsISupportsObject_alreadyAddreffed, proto));
  if (!stream) {
    return nullptr;
  }
>>>>>>> upstream-releases

<<<<<<< HEAD
/**
 * Characterizes the family of algorithms, (startAlgorithm, pullAlgorithm,
 * cancelAlgorithm), associated with a readable stream.
 *
 * See the comment on SetUpReadableStreamDefaultController().
 */
enum class SourceAlgorithms {
  Script,
  Tee,
};

static MOZ_MUST_USE bool SetUpReadableStreamDefaultController(
    JSContext* cx, Handle<ReadableStream*> stream, SourceAlgorithms algorithms,
    HandleValue underlyingSource, HandleValue pullMethod,
    HandleValue cancelMethod, double highWaterMark, HandleValue size);

static MOZ_MUST_USE ReadableByteStreamController*
CreateExternalReadableByteStreamController(
    JSContext* cx, Handle<ReadableStream*> stream,
    JS::ReadableStreamUnderlyingSource* source);

ReadableStream* ReadableStream::createExternalSourceStream(
    JSContext* cx, JS::ReadableStreamUnderlyingSource* source,
    HandleObject proto /* = nullptr */) {
  Rooted<ReadableStream*> stream(cx, create(cx, proto));
  if (!stream) {
    return nullptr;
  }

  Rooted<ReadableStreamController*> controller(cx);
  controller = CreateExternalReadableByteStreamController(cx, stream, source);
  if (!controller) {
    return nullptr;
  }
||||||| merged common ancestors
    static MOZ_MUST_USE ReadableStream* stream(JSContext* cx, TeeState* teeState) {
        RootedValue streamVal(cx, teeState->getFixedSlot(Slot_Stream));
        return ToUnwrapped<ReadableStream>(cx, streamVal);
    }

    ReadableStreamDefaultController* branch1() {
        ReadableStreamDefaultController* controller = &getFixedSlot(Slot_Branch1).toObject()
                                                       .as<ReadableStreamDefaultController>();
        MOZ_ASSERT(ControllerFlags(controller) & ControllerFlag_TeeBranch);
        MOZ_ASSERT(ControllerFlags(controller) & ControllerFlag_TeeBranch1);
        return controller;
    }
    void setBranch1(ReadableStreamDefaultController* controller) {
        MOZ_ASSERT(ControllerFlags(controller) & ControllerFlag_TeeBranch);
        MOZ_ASSERT(ControllerFlags(controller) & ControllerFlag_TeeBranch1);
        setFixedSlot(Slot_Branch1, ObjectValue(*controller));
    }

    ReadableStreamDefaultController* branch2() {
        ReadableStreamDefaultController* controller = &getFixedSlot(Slot_Branch2).toObject()
                                                       .as<ReadableStreamDefaultController>();
        MOZ_ASSERT(ControllerFlags(controller) & ControllerFlag_TeeBranch);
        MOZ_ASSERT(ControllerFlags(controller) & ControllerFlag_TeeBranch2);
        return controller;
    }
    void setBranch2(ReadableStreamDefaultController* controller) {
        MOZ_ASSERT(ControllerFlags(controller) & ControllerFlag_TeeBranch);
        MOZ_ASSERT(ControllerFlags(controller) & ControllerFlag_TeeBranch2);
        setFixedSlot(Slot_Branch2, ObjectValue(*controller));
    }

    static TeeState* create(JSContext* cx, Handle<ReadableStream*> stream) {
        Rooted<TeeState*> state(cx, NewBuiltinClassInstance<TeeState>(cx));
        if (!state) {
            return nullptr;
        }

        Rooted<PromiseObject*> promise(cx, PromiseObject::createSkippingExecutor(cx));
        if (!promise) {
            return nullptr;
        }

        state->setFixedSlot(Slot_Flags, Int32Value(0));
        state->setFixedSlot(Slot_Promise, ObjectValue(*promise));
        RootedObject wrappedStream(cx, stream);
        if (!cx->compartment()->wrap(cx, &wrappedStream)) {
            return nullptr;
        }
        state->setFixedSlot(Slot_Stream, ObjectValue(*wrappedStream));

        return state;
   }
};

const Class TeeState::class_ = {
    "TeeState",
    JSCLASS_HAS_RESERVED_SLOTS(SlotCount)
};

#define CLASS_SPEC(cls, nCtorArgs, nSlots, specFlags, classFlags, classOps) \
const ClassSpec cls::classSpec_ = { \
    GenericCreateConstructor<cls::constructor, nCtorArgs, gc::AllocKind::FUNCTION>, \
    GenericCreatePrototype<cls>, \
    nullptr, \
    nullptr, \
    cls##_methods, \
    cls##_properties, \
    nullptr, \
    specFlags \
}; \
\
const Class cls::class_ = { \
    #cls, \
    JSCLASS_HAS_RESERVED_SLOTS(nSlots) | \
    JSCLASS_HAS_CACHED_PROTO(JSProto_##cls) | \
    classFlags, \
    classOps, \
    &cls::classSpec_ \
}; \
\
const Class cls::protoClass_ = { \
    "object", \
    JSCLASS_HAS_CACHED_PROTO(JSProto_##cls), \
    JS_NULL_CLASS_OPS, \
    &cls::classSpec_ \
};

// Streams spec, 3.2.3., steps 1-4.
ReadableStream*
ReadableStream::createStream(JSContext* cx, HandleObject proto /* = nullptr */)
{
    Rooted<ReadableStream*> stream(cx, NewObjectWithClassProto<ReadableStream>(cx, proto));
    if (!stream) {
        return nullptr;
    }

    // Step 1: Set this.[[state]] to "readable".
    // Step 2: Set this.[[reader]] and this.[[storedError]] to undefined (implicit).
    // Step 3: Set this.[[disturbed]] to false (implicit).
    // Step 4: Set this.[[readableStreamController]] to undefined (implicit).
    stream->setFixedSlot(StreamSlot_State, Int32Value(Readable));
=======
  if (!SetUpExternalReadableByteStreamController(cx, stream, source)) {
    return nullptr;
  }
>>>>>>> upstream-releases

<<<<<<< HEAD
  stream->setController(controller);
  return stream;
||||||| merged common ancestors
    return stream;
=======
  return stream;
>>>>>>> upstream-releases
}

static MOZ_MUST_USE bool MakeSizeAlgorithmFromSizeFunction(JSContext* cx,
                                                           HandleValue size);

static MOZ_MUST_USE bool ValidateAndNormalizeHighWaterMark(
    JSContext* cx, HandleValue highWaterMarkVal, double* highWaterMark);

static MOZ_MUST_USE bool
SetUpReadableStreamDefaultControllerFromUnderlyingSource(
    JSContext* cx, Handle<ReadableStream*> stream, HandleValue underlyingSource,
    double highWaterMark, HandleValue sizeAlgorithm);

/**
 * Streams spec, 3.2.3. new ReadableStream(underlyingSource = {}, strategy = {})
 */
bool ReadableStream::constructor(JSContext* cx, unsigned argc, Value* vp) {
  CallArgs args = CallArgsFromVp(argc, vp);

  if (!ThrowIfNotConstructing(cx, args, "ReadableStream")) {
    return false;
  }

  // Implicit in the spec: argument default values.
  RootedValue underlyingSource(cx, args.get(0));
  if (underlyingSource.isUndefined()) {
    JSObject* emptyObj = NewBuiltinClassInstance<PlainObject>(cx);
    if (!emptyObj) {
      return false;
    }
    underlyingSource = ObjectValue(*emptyObj);
  }

  RootedValue strategy(cx, args.get(1));
  if (strategy.isUndefined()) {
    JSObject* emptyObj = NewBuiltinClassInstance<PlainObject>(cx);
    if (!emptyObj) {
      return false;
    }
    strategy = ObjectValue(*emptyObj);
  }

<<<<<<< HEAD
  // Implicit in the spec: Set this to
  //     OrdinaryCreateFromConstructor(NewTarget, ...).
  // Step 1: Perform ! InitializeReadableStream(this).
  RootedObject proto(cx);
  if (!GetPrototypeFromBuiltinConstructor(cx, args, &proto)) {
    return false;
  }
  Rooted<ReadableStream*> stream(cx, ReadableStream::create(cx, proto));
  if (!stream) {
    return false;
  }
||||||| merged common ancestors
    if (!options.isUndefined()) {
        if (!GetProperty(cx, options, cx->names().size, &size)) {
            return false;
        }
=======
  // Implicit in the spec: Set this to
  //     OrdinaryCreateFromConstructor(NewTarget, ...).
  // Step 1: Perform ! InitializeReadableStream(this).
  RootedObject proto(cx);
  if (!GetPrototypeFromBuiltinConstructor(cx, args, JSProto_ReadableStream,
                                          &proto)) {
    return false;
  }
  Rooted<ReadableStream*> stream(cx,
                                 ReadableStream::create(cx, nullptr, proto));
  if (!stream) {
    return false;
  }
>>>>>>> upstream-releases

  // Step 2: Let size be ? GetV(strategy, "size").
  RootedValue size(cx);
  if (!GetProperty(cx, strategy, cx->names().size, &size)) {
    return false;
  }

  // Step 3: Let highWaterMark be ? GetV(strategy, "highWaterMark").
  RootedValue highWaterMarkVal(cx);
  if (!GetProperty(cx, strategy, cx->names().highWaterMark,
                   &highWaterMarkVal)) {
    return false;
  }

  // Step 4: Let type be ? GetV(underlyingSource, "type").
  RootedValue type(cx);
  if (!GetProperty(cx, underlyingSource, cx->names().type, &type)) {
    return false;
  }

  // Step 5: Let typeString be ? ToString(type).
  RootedString typeString(cx, ToString<CanGC>(cx, type));
  if (!typeString) {
    return false;
  }

<<<<<<< HEAD
  // Step 6: If typeString is "bytes",
  int32_t cmp;
  if (!CompareStrings(cx, typeString, cx->names().bytes, &cmp)) {
    return false;
  }
  if (cmp == 0) {
    // The rest of step 6 is unimplemented, since we don't support
    // user-defined byte streams yet.
    JS_ReportErrorNumberASCII(cx, GetErrorMessage, nullptr,
                              JSMSG_READABLESTREAM_BYTES_TYPE_NOT_IMPLEMENTED);
    return false;
  }
||||||| merged common ancestors
    int32_t notByteStream;
    if (!CompareStrings(cx, type, cx->names().bytes, &notByteStream)) {
        return false;
    }
=======
  // Step 6: If typeString is "bytes",
  bool equal;
  if (!EqualStrings(cx, typeString, cx->names().bytes, &equal)) {
    return false;
  }
  if (equal) {
    // The rest of step 6 is unimplemented, since we don't support
    // user-defined byte streams yet.
    JS_ReportErrorNumberASCII(cx, GetErrorMessage, nullptr,
                              JSMSG_READABLESTREAM_BYTES_TYPE_NOT_IMPLEMENTED);
    return false;
  }
>>>>>>> upstream-releases

  // Step 7: Otherwise, if type is undefined,
  if (type.isUndefined()) {
    // Step 7.a: Let sizeAlgorithm be ? MakeSizeAlgorithmFromSizeFunction(size).
    if (!MakeSizeAlgorithmFromSizeFunction(cx, size)) {
      return false;
    }

    // Step 7.b: If highWaterMark is undefined, let highWaterMark be 1.
    double highWaterMark;
    if (highWaterMarkVal.isUndefined()) {
      highWaterMark = 1;
    } else {
      // Step 7.c: Set highWaterMark to ?
      // ValidateAndNormalizeHighWaterMark(highWaterMark).
      if (!ValidateAndNormalizeHighWaterMark(cx, highWaterMarkVal,
                                             &highWaterMark)) {
        return false;
      }
    }

    // Step 7.d: Perform
    //           ? SetUpReadableStreamDefaultControllerFromUnderlyingSource(
    //           this, underlyingSource, highWaterMark, sizeAlgorithm).
    if (!SetUpReadableStreamDefaultControllerFromUnderlyingSource(
            cx, stream, underlyingSource, highWaterMark, size)) {
      return false;
    }

    args.rval().setObject(*stream);
    return true;
  }

  // Step 8: Otherwise, throw a RangeError exception.
  JS_ReportErrorNumberASCII(cx, GetErrorMessage, nullptr,
                            JSMSG_READABLESTREAM_UNDERLYINGSOURCE_TYPE_WRONG);
  return false;
}

/**
 * Streams spec, 3.2.5.1. get locked
 */
static bool ReadableStream_locked(JSContext* cx, unsigned argc, Value* vp) {
  CallArgs args = CallArgsFromVp(argc, vp);

  // Step 1: If ! IsReadableStream(this) is false, throw a TypeError exception.
  Rooted<ReadableStream*> unwrappedStream(
      cx, UnwrapAndTypeCheckThis<ReadableStream>(cx, args, "get locked"));
  if (!unwrappedStream) {
    return false;
  }

  // Step 2: Return ! IsReadableStreamLocked(this).
  args.rval().setBoolean(unwrappedStream->locked());
  return true;
}

static MOZ_MUST_USE JSObject* ReadableStreamCancel(
    JSContext* cx, Handle<ReadableStream*> unwrappedStream, HandleValue reason);

/**
 * Streams spec, 3.2.5.2. cancel ( reason )
 */
static MOZ_MUST_USE bool ReadableStream_cancel(JSContext* cx, unsigned argc,
                                               Value* vp) {
  CallArgs args = CallArgsFromVp(argc, vp);

  // Step 1: If ! IsReadableStream(this) is false, return a promise rejected
  //         with a TypeError exception.
  Rooted<ReadableStream*> unwrappedStream(
      cx, UnwrapAndTypeCheckThis<ReadableStream>(cx, args, "cancel"));
  if (!unwrappedStream) {
    return ReturnPromiseRejectedWithPendingError(cx, args);
  }

  // Step 2: If ! IsReadableStreamLocked(this) is true, return a promise
  //         rejected with a TypeError exception.
  if (unwrappedStream->locked()) {
    JS_ReportErrorNumberASCII(cx, GetErrorMessage, nullptr,
                              JSMSG_READABLESTREAM_LOCKED_METHOD, "cancel");
    return ReturnPromiseRejectedWithPendingError(cx, args);
  }

  // Step 3: Return ! ReadableStreamCancel(this, reason).
  RootedObject cancelPromise(
      cx, ::ReadableStreamCancel(cx, unwrappedStream, args.get(0)));
  if (!cancelPromise) {
    return false;
  }
  args.rval().setObject(*cancelPromise);
  return true;
}

static MOZ_MUST_USE ReadableStreamDefaultReader*
CreateReadableStreamDefaultReader(
    JSContext* cx, Handle<ReadableStream*> unwrappedStream,
    ForAuthorCodeBool forAuthorCode = ForAuthorCodeBool::No,
    HandleObject proto = nullptr);

<<<<<<< HEAD
/**
 * Streams spec, 3.2.5.3. getReader()
 */
static bool ReadableStream_getReader(JSContext* cx, unsigned argc, Value* vp) {
  CallArgs args = CallArgsFromVp(argc, vp);

  // Step 1: If ! IsReadableStream(this) is false, throw a TypeError exception.
  Rooted<ReadableStream*> unwrappedStream(
      cx, UnwrapAndTypeCheckThis<ReadableStream>(cx, args, "getReader"));
  if (!unwrappedStream) {
    return false;
  }
||||||| merged common ancestors
// Streams spec, 3.2.4.3. getReader()
static bool
ReadableStream_getReader(JSContext* cx, unsigned argc, Value* vp)
{
    CallArgs args = CallArgsFromVp(argc, vp);

    // Step 1: If ! IsReadableStream(this) is false, throw a TypeError exception.
    Rooted<ReadableStream*> stream(cx);
    stream = ToUnwrapped<ReadableStream>(cx, args.thisv(), "ReadableStream", "getReader");
    if (!stream) {
        return false;
    }
=======
/**
 * Streams spec, 3.2.5.3. getReader({ mode } = {})
 */
static bool ReadableStream_getReader(JSContext* cx, unsigned argc, Value* vp) {
  CallArgs args = CallArgsFromVp(argc, vp);

  // Implicit in the spec: Argument defaults and destructuring.
  RootedValue optionsVal(cx, args.get(0));
  if (optionsVal.isUndefined()) {
    JSObject* emptyObj = NewBuiltinClassInstance<PlainObject>(cx);
    if (!emptyObj) {
      return false;
    }
    optionsVal.setObject(*emptyObj);
  }
  RootedValue modeVal(cx);
  if (!GetProperty(cx, optionsVal, cx->names().mode, &modeVal)) {
    return false;
  }
>>>>>>> upstream-releases

<<<<<<< HEAD
  RootedObject reader(cx);

  // Step 2: If mode is undefined, return
  //         ? AcquireReadableStreamDefaultReader(this).
  RootedValue modeVal(cx);
  HandleValue optionsVal = args.get(0);
  if (!optionsVal.isUndefined()) {
    if (!GetProperty(cx, optionsVal, cx->names().mode, &modeVal)) {
      return false;
||||||| merged common ancestors
    RootedObject reader(cx);

    // Step 2: If mode is undefined, return
    //         ? AcquireReadableStreamDefaultReader(this).
    RootedValue modeVal(cx);
    HandleValue optionsVal = args.get(0);
    if (!optionsVal.isUndefined()) {
        if (!GetProperty(cx, optionsVal, cx->names().mode, &modeVal)) {
            return false;
        }
=======
  // Step 1: If ! IsReadableStream(this) is false, throw a TypeError exception.
  Rooted<ReadableStream*> unwrappedStream(
      cx, UnwrapAndTypeCheckThis<ReadableStream>(cx, args, "getReader"));
  if (!unwrappedStream) {
    return false;
  }

  // Step 2: If mode is undefined, return
  //         ? AcquireReadableStreamDefaultReader(this).
  RootedObject reader(cx);
  if (modeVal.isUndefined()) {
    reader = CreateReadableStreamDefaultReader(cx, unwrappedStream,
                                               ForAuthorCodeBool::Yes);
  } else {
    // Step 3: Set mode to ? ToString(mode) (implicit).
    RootedString mode(cx, ToString<CanGC>(cx, modeVal));
    if (!mode) {
      return false;
>>>>>>> upstream-releases
    }
  }

<<<<<<< HEAD
  if (modeVal.isUndefined()) {
    reader = CreateReadableStreamDefaultReader(cx, unwrappedStream,
                                               ForAuthorCodeBool::Yes);
  } else {
    // Step 3: Set mode to ? ToString(mode) (implicit).
    RootedString mode(cx, ToString<CanGC>(cx, modeVal));
    if (!mode) {
      return false;
||||||| merged common ancestors
    if (modeVal.isUndefined()) {
        reader = CreateReadableStreamDefaultReader(cx, stream);
    } else {
        // Step 3: Set mode to ? ToString(mode) (implicit).
        RootedString mode(cx, ToString<CanGC>(cx, modeVal));
        if (!mode) {
            return false;
        }

        // Step 4: If mode is "byob", return ? AcquireReadableStreamBYOBReader(this).
        int32_t notByob;
        if (!CompareStrings(cx, mode, cx->names().byob, &notByob)) {
            return false;
        }
        if (notByob) {
            JS_ReportErrorNumberASCII(cx, GetErrorMessage, nullptr,
                                      JSMSG_READABLESTREAM_INVALID_READER_MODE);
            // Step 5: Throw a RangeError exception.
            return false;
        }

        JS_ReportErrorNumberASCII(cx, GetErrorMessage, nullptr,
                                  JSMSG_READABLESTREAM_BYTES_TYPE_NOT_IMPLEMENTED);
=======
    // Step 4: If mode is "byob",
    //         return ? AcquireReadableStreamBYOBReader(this).
    bool equal;
    if (!EqualStrings(cx, mode, cx->names().byob, &equal)) {
      return false;
>>>>>>> upstream-releases
    }
<<<<<<< HEAD

    // Step 4: If mode is "byob",
    //         return ? AcquireReadableStreamBYOBReader(this).
    int32_t notByob;
    if (!CompareStrings(cx, mode, cx->names().byob, &notByob)) {
      return false;
    }
    if (notByob) {
      JS_ReportErrorNumberASCII(cx, GetErrorMessage, nullptr,
                                JSMSG_READABLESTREAM_INVALID_READER_MODE);
      // Step 5: Throw a RangeError exception.
      return false;
||||||| merged common ancestors

    // Reordered second part of steps 2 and 4.
    if (!reader) {
        return false;
=======
    if (equal) {
      JS_ReportErrorNumberASCII(
          cx, GetErrorMessage, nullptr,
          JSMSG_READABLESTREAM_BYTES_TYPE_NOT_IMPLEMENTED);
      return false;
>>>>>>> upstream-releases
    }

<<<<<<< HEAD
||||||| merged common ancestors
// Streams spec, 3.2.4.4. pipeThrough({ writable, readable }, options)
static MOZ_MUST_USE bool
ReadableStream_pipeThrough(JSContext* cx, unsigned argc, Value* vp)
{
=======
    // Step 5: Throw a RangeError exception.
>>>>>>> upstream-releases
    JS_ReportErrorNumberASCII(cx, GetErrorMessage, nullptr,
<<<<<<< HEAD
                              JSMSG_READABLESTREAM_BYTES_TYPE_NOT_IMPLEMENTED);
  }
||||||| merged common ancestors
                              JSMSG_READABLESTREAM_METHOD_NOT_IMPLEMENTED, "pipeThrough");
    return false;
    // // Step 1: Perform ? Invoke(this, "pipeTo", « writable, options »).

    // // Step 2: Return readable.
}
=======
                              JSMSG_READABLESTREAM_INVALID_READER_MODE);
    return false;
  }
>>>>>>> upstream-releases

  // Reordered second part of steps 2 and 4.
  if (!reader) {
    return false;
  }
  args.rval().setObject(*reader);
  return true;
}

// Streams spec, 3.2.5.4.
//      pipeThrough({ writable, readable }, options)
//
// Not implemented.

// Streams spec, 3.2.5.5.
//      pipeTo(dest, { preventClose, preventAbort, preventCancel } = {})
//
// Not implemented.

static MOZ_MUST_USE bool ReadableStreamTee(
    JSContext* cx, Handle<ReadableStream*> unwrappedStream,
    bool cloneForBranch2, MutableHandle<ReadableStream*> branch1,
    MutableHandle<ReadableStream*> branch2);

/**
 * Streams spec, 3.2.5.6. tee()
 */
static bool ReadableStream_tee(JSContext* cx, unsigned argc, Value* vp) {
  CallArgs args = CallArgsFromVp(argc, vp);

  // Step 1: If ! IsReadableStream(this) is false, throw a TypeError exception.
  Rooted<ReadableStream*> unwrappedStream(
      cx, UnwrapAndTypeCheckThis<ReadableStream>(cx, args, "tee"));
  if (!unwrappedStream) {
    return false;
  }

  // Step 2: Let branches be ? ReadableStreamTee(this, false).
  Rooted<ReadableStream*> branch1(cx);
  Rooted<ReadableStream*> branch2(cx);
  if (!ReadableStreamTee(cx, unwrappedStream, false, &branch1, &branch2)) {
    return false;
  }

  // Step 3: Return ! CreateArrayFromList(branches).
  RootedNativeObject branches(cx, NewDenseFullyAllocatedArray(cx, 2));
  if (!branches) {
    return false;
  }
  branches->setDenseInitializedLength(2);
  branches->initDenseElement(0, ObjectValue(*branch1));
  branches->initDenseElement(1, ObjectValue(*branch2));

  args.rval().setObject(*branches);
  return true;
}

static const JSFunctionSpec ReadableStream_methods[] = {
    JS_FN("cancel", ReadableStream_cancel, 1, 0),
    JS_FN("getReader", ReadableStream_getReader, 0, 0),
    JS_FN("tee", ReadableStream_tee, 0, 0), JS_FS_END};

static const JSPropertySpec ReadableStream_properties[] = {
<<<<<<< HEAD
    JS_PSG("locked", ReadableStream_locked, 0), JS_PS_END};
||||||| merged common ancestors
    JS_PSG("locked", ReadableStream_locked, 0),
    JS_PS_END
};
=======
    JS_PSG("locked", ReadableStream_locked, 0), JS_PS_END};

CLASS_SPEC(ReadableStream, 0, SlotCount, 0,
           JSCLASS_PRIVATE_IS_NSISUPPORTS | JSCLASS_HAS_PRIVATE,
           JS_NULL_CLASS_OPS);
>>>>>>> upstream-releases

<<<<<<< HEAD
CLASS_SPEC(ReadableStream, 0, SlotCount, 0, 0, JS_NULL_CLASS_OPS);

/*** 3.3. General readable stream abstract operations ***********************/
||||||| merged common ancestors
CLASS_SPEC(ReadableStream, 0, StreamSlotCount, 0, 0, JS_NULL_CLASS_OPS);
=======
/*** 3.3. General readable stream abstract operations ***********************/
>>>>>>> upstream-releases

// Streams spec, 3.3.1. AcquireReadableStreamBYOBReader ( stream )
// Always inlined.

// Streams spec, 3.3.2. AcquireReadableStreamDefaultReader ( stream )
// Always inlined. See CreateReadableStreamDefaultReader.

<<<<<<< HEAD
/**
 * Streams spec, 3.3.3. CreateReadableStream (
 *                          startAlgorithm, pullAlgorithm, cancelAlgorithm
 *                          [, highWaterMark [, sizeAlgorithm ] ] )
 *
 * The start/pull/cancelAlgorithm arguments are represented instead as four
 * arguments: sourceAlgorithms, underlyingSource, pullMethod, cancelMethod.
 * See the comment on SetUpReadableStreamDefaultController.
 */
MOZ_MUST_USE ReadableStream* CreateReadableStream(
    JSContext* cx, SourceAlgorithms sourceAlgorithms,
    HandleValue underlyingSource, HandleValue pullMethod = UndefinedHandleValue,
    HandleValue cancelMethod = UndefinedHandleValue, double highWaterMark = 1,
    HandleValue sizeAlgorithm = UndefinedHandleValue,
    HandleObject proto = nullptr) {
  cx->check(underlyingSource, sizeAlgorithm, proto);
  MOZ_ASSERT(sizeAlgorithm.isUndefined() || IsCallable(sizeAlgorithm));

  // Step 1: If highWaterMark was not passed, set it to 1 (implicit).
  // Step 2: If sizeAlgorithm was not passed, set it to an algorithm that
  // returns 1 (implicit). Step 3: Assert: ! IsNonNegativeNumber(highWaterMark)
  // is true.
  MOZ_ASSERT(highWaterMark >= 0);

  // Step 4: Let stream be ObjectCreate(the original value of ReadableStream's
  // prototype property). Step 5: Perform ! InitializeReadableStream(stream).
  Rooted<ReadableStream*> stream(cx, ReadableStream::create(cx, proto));
  if (!stream) {
    return nullptr;
  }

  // Step 6: Let controller be ObjectCreate(the original value of
  //         ReadableStreamDefaultController's prototype property).
  // Step 7: Perform ? SetUpReadableStreamDefaultController(stream,
  //         controller, startAlgorithm, pullAlgorithm, cancelAlgorithm,
  //         highWaterMark, sizeAlgorithm).

  if (!SetUpReadableStreamDefaultController(
          cx, stream, sourceAlgorithms, underlyingSource, pullMethod,
          cancelMethod, highWaterMark, sizeAlgorithm)) {
    return nullptr;
  }

  // Step 8: Return stream.
  return stream;
}

// Streams spec, 3.3.4. CreateReadableByteStream (
//                          startAlgorithm, pullAlgorithm, cancelAlgorithm
//                          [, highWaterMark [, autoAllocateChunkSize ] ] )
// Not implemented.

/**
 * Streams spec, 3.3.5. InitializeReadableStream ( stream )
 */
MOZ_MUST_USE /* static */ ReadableStream* ReadableStream::create(
    JSContext* cx, HandleObject proto /* = nullptr */) {
  // In the spec, InitializeReadableStream is always passed a newly created
  // ReadableStream object. We instead create it here and return it below.
  Rooted<ReadableStream*> stream(
      cx, NewObjectWithClassProto<ReadableStream>(cx, proto));
  if (!stream) {
    return nullptr;
  }

  // Step 1: Set stream.[[state]] to "readable".
  stream->initStateBits(Readable);
  MOZ_ASSERT(stream->readable());

  // Step 2: Set stream.[[reader]] and stream.[[storedError]] to
  //         undefined (implicit).
  MOZ_ASSERT(!stream->hasReader());
  MOZ_ASSERT(stream->storedError().isUndefined());

  // Step 3: Set stream.[[disturbed]] to false (done in step 1).
  MOZ_ASSERT(!stream->disturbed());

  return stream;
}

// Streams spec, 3.3.6. IsReadableStream ( x )
// Using is<T> instead.

// Streams spec, 3.3.7. IsReadableStreamDisturbed ( stream )
// Using stream->disturbed() instead.
||||||| merged common ancestors
// Streams spec, 3.3.3. IsReadableStream ( x )
// Using is<T> instead.

// Streams spec, 3.3.4. IsReadableStreamDisturbed ( stream )
// Using stream->disturbed() instead.
=======
/**
 * Characterizes the family of algorithms, (startAlgorithm, pullAlgorithm,
 * cancelAlgorithm), associated with a readable stream.
 *
 * See the comment on SetUpReadableStreamDefaultController().
 */
enum class SourceAlgorithms {
  Script,
  Tee,
};
>>>>>>> upstream-releases

<<<<<<< HEAD
/**
 * Streams spec, 3.3.8. IsReadableStreamLocked ( stream )
 */
bool ReadableStream::locked() const {
  // Step 1: Assert: ! IsReadableStream(stream) is true (implicit).
  // Step 2: If stream.[[reader]] is undefined, return false.
  // Step 3: Return true.
  // Special-casing for streams with external sources. Those can be locked
  // explicitly via JSAPI, which is indicated by a controller flag.
  // IsReadableStreamLocked is called from the controller's constructor, at
  // which point we can't yet call stream->controller(), but the source also
  // can't be locked yet.
  if (hasController() && controller()->sourceLocked()) {
    return true;
  }
  return hasReader();
}
||||||| merged common ancestors
// Streams spec, 3.3.5. IsReadableStreamLocked ( stream )
bool
ReadableStream::locked() const
{
    // Step 1: Assert: ! IsReadableStream(stream) is true (implicit).
    // Step 2: If stream.[[reader]] is undefined, return false.
    // Step 3: Return true.
    // Special-casing for streams with external sources. Those can be locked
    // explicitly via JSAPI, which is indicated by a controller flag.
    // IsReadableStreamLocked is called from the controller's constructor, at
    // which point we can't yet call ControllerFromStream(stream), but the
    // source also can't be locked yet.
    if (HasController(this) &&
        (ControllerFlags(ControllerFromStream(this)) & ControllerFlag_SourceLocked))
    {
        return true;
    }
    return HasReader(this);
}
=======
static MOZ_MUST_USE bool SetUpReadableStreamDefaultController(
    JSContext* cx, Handle<ReadableStream*> stream, SourceAlgorithms algorithms,
    HandleValue underlyingSource, HandleValue pullMethod,
    HandleValue cancelMethod, double highWaterMark, HandleValue size);
>>>>>>> upstream-releases

<<<<<<< HEAD
static MOZ_MUST_USE bool ReadableStreamDefaultControllerClose(
    JSContext* cx,
    Handle<ReadableStreamDefaultController*> unwrappedController);
||||||| merged common ancestors
static MOZ_MUST_USE bool
ReadableStreamDefaultControllerClose(JSContext* cx,
                                     Handle<ReadableStreamDefaultController*> controller);
static MOZ_MUST_USE bool
ReadableStreamDefaultControllerEnqueue(JSContext* cx,
                                       Handle<ReadableStreamDefaultController*> controller,
                                       HandleValue chunk);
=======
/**
 * Streams spec, 3.3.3. CreateReadableStream (
 *                          startAlgorithm, pullAlgorithm, cancelAlgorithm
 *                          [, highWaterMark [, sizeAlgorithm ] ] )
 *
 * The start/pull/cancelAlgorithm arguments are represented instead as four
 * arguments: sourceAlgorithms, underlyingSource, pullMethod, cancelMethod.
 * See the comment on SetUpReadableStreamDefaultController.
 */
MOZ_MUST_USE ReadableStream* CreateReadableStream(
    JSContext* cx, SourceAlgorithms sourceAlgorithms,
    HandleValue underlyingSource, HandleValue pullMethod = UndefinedHandleValue,
    HandleValue cancelMethod = UndefinedHandleValue, double highWaterMark = 1,
    HandleValue sizeAlgorithm = UndefinedHandleValue,
    HandleObject proto = nullptr) {
  cx->check(underlyingSource, sizeAlgorithm, proto);
  MOZ_ASSERT(sizeAlgorithm.isUndefined() || IsCallable(sizeAlgorithm));

  // Step 1: If highWaterMark was not passed, set it to 1 (implicit).
  // Step 2: If sizeAlgorithm was not passed, set it to an algorithm that
  //         returns 1 (implicit).
  // Step 3: Assert: ! IsNonNegativeNumber(highWaterMark) is true.
  MOZ_ASSERT(highWaterMark >= 0);

  // Step 4: Let stream be ObjectCreate(the original value of ReadableStream's
  //         prototype property).
  // Step 5: Perform ! InitializeReadableStream(stream).
  Rooted<ReadableStream*> stream(cx,
                                 ReadableStream::create(cx, nullptr, proto));
  if (!stream) {
    return nullptr;
  }

  // Step 6: Let controller be ObjectCreate(the original value of
  //         ReadableStreamDefaultController's prototype property).
  // Step 7: Perform ? SetUpReadableStreamDefaultController(stream,
  //         controller, startAlgorithm, pullAlgorithm, cancelAlgorithm,
  //         highWaterMark, sizeAlgorithm).
  if (!SetUpReadableStreamDefaultController(
          cx, stream, sourceAlgorithms, underlyingSource, pullMethod,
          cancelMethod, highWaterMark, sizeAlgorithm)) {
    return nullptr;
  }
>>>>>>> upstream-releases

<<<<<<< HEAD
static MOZ_MUST_USE bool ReadableStreamDefaultControllerEnqueue(
    JSContext* cx, Handle<ReadableStreamDefaultController*> unwrappedController,
    HandleValue chunk);
||||||| merged common ancestors
static bool
TeeReaderReadHandler(JSContext* cx, unsigned argc, Value* vp)
{
    CallArgs args = CallArgsFromVp(argc, vp);
    Rooted<TeeState*> teeState(cx, TargetFromHandler<TeeState>(args.callee()));
    HandleValue resultVal = args.get(0);
=======
  // Step 8: Return stream.
  return stream;
}
>>>>>>> upstream-releases

<<<<<<< HEAD
/**
 * Streams spec, 3.3.9. ReadableStreamTee steps 12.a.i-ix.
 */
static bool TeeReaderReadHandler(JSContext* cx, unsigned argc, Value* vp) {
  CallArgs args = CallArgsFromVp(argc, vp);
  Rooted<TeeState*> unwrappedTeeState(cx,
                                      UnwrapCalleeSlot<TeeState>(cx, args, 0));
  HandleValue resultVal = args.get(0);

  // Step i: Assert: Type(result) is Object.
  RootedObject result(cx, &resultVal.toObject());

  // Step ii: Let value be ? Get(result, "value").
  RootedValue value(cx);
  if (!GetPropertyPure(cx, result, NameToId(cx->names().value),
                       value.address())) {
    return false;
  }
||||||| merged common ancestors
    // Step a: Assert: Type(result) is Object.
    RootedObject result(cx, &resultVal.toObject());
=======
// Streams spec, 3.3.4. CreateReadableByteStream (
//                          startAlgorithm, pullAlgorithm, cancelAlgorithm
//                          [, highWaterMark [, autoAllocateChunkSize ] ] )
// Not implemented.
>>>>>>> upstream-releases

<<<<<<< HEAD
  // Step iii: Let done be ? Get(result, "done").
  RootedValue doneVal(cx);
  if (!GetPropertyPure(cx, result, NameToId(cx->names().done),
                       doneVal.address())) {
    return false;
  }

  // Step iv: Assert: Type(done) is Boolean.
  bool done = doneVal.toBoolean();

  // Step v: If done is true and closedOrErrored is false,
  if (done && !unwrappedTeeState->closedOrErrored()) {
    // Step v.1: If canceled1 is false,
    if (!unwrappedTeeState->canceled1()) {
      // Step v.1.a: Perform ! ReadableStreamDefaultControllerClose(
      //             branch1.[[readableStreamController]]).
      Rooted<ReadableStreamDefaultController*> unwrappedBranch1(
          cx, unwrappedTeeState->branch1());
      if (!ReadableStreamDefaultControllerClose(cx, unwrappedBranch1)) {
        return false;
      }
    }
||||||| merged common ancestors
    // Step b: Let value be ? Get(result, "value").
    RootedValue value(cx);
    if (!GetPropertyPure(cx, result, NameToId(cx->names().value), value.address())) {
        return false;
    }
=======
/**
 * Streams spec, 3.3.5. InitializeReadableStream ( stream )
 */
MOZ_MUST_USE /* static */
    ReadableStream*
    ReadableStream::create(
        JSContext* cx, void* nsISupportsObject_alreadyAddreffed /* = nullptr */,
        HandleObject proto /* = nullptr */) {
  // In the spec, InitializeReadableStream is always passed a newly created
  // ReadableStream object. We instead create it here and return it below.
  Rooted<ReadableStream*> stream(
      cx, NewObjectWithClassProto<ReadableStream>(cx, proto));
  if (!stream) {
    return nullptr;
  }
>>>>>>> upstream-releases

<<<<<<< HEAD
    // Step v.2: If teeState.[[canceled2]] is false,
    if (!unwrappedTeeState->canceled2()) {
      // Step v.2.a: Perform ! ReadableStreamDefaultControllerClose(
      //             branch2.[[readableStreamController]]).
      Rooted<ReadableStreamDefaultController*> unwrappedBranch2(
          cx, unwrappedTeeState->branch2());
      if (!ReadableStreamDefaultControllerClose(cx, unwrappedBranch2)) {
        return false;
      }
    }
||||||| merged common ancestors
    // Step c: Let done be ? Get(result, "done").
    RootedValue doneVal(cx);
    if (!GetPropertyPure(cx, result, NameToId(cx->names().done), doneVal.address())) {
        return false;
    }
=======
  JS_SetPrivate(stream, nsISupportsObject_alreadyAddreffed);
>>>>>>> upstream-releases

<<<<<<< HEAD
    // Step v.3: Set closedOrErrored to true.
    unwrappedTeeState->setClosedOrErrored();
  }
||||||| merged common ancestors
    // Step d: Assert: Type(done) is Boolean.
    bool done = doneVal.toBoolean();

    // Step e: If done is true and teeState.[[closedOrErrored]] is false,
    if (done && !teeState->closedOrErrored()) {
        // Step i: If teeState.[[canceled1]] is false,
        if (!teeState->canceled1()) {
            // Step 1: Perform ! ReadableStreamDefaultControllerClose(branch1).
            Rooted<ReadableStreamDefaultController*> branch1(cx, teeState->branch1());
            if (!ReadableStreamDefaultControllerClose(cx, branch1)) {
                return false;
            }
        }

        // Step ii: If teeState.[[canceled2]] is false,
        if (!teeState->canceled2()) {
            // Step 1: Perform ! ReadableStreamDefaultControllerClose(branch1).
            Rooted<ReadableStreamDefaultController*> branch2(cx, teeState->branch2());
            if (!ReadableStreamDefaultControllerClose(cx, branch2)) {
                return false;
            }
        }

        // Step iii: Set teeState.[[closedOrErrored]] to true.
        teeState->setClosedOrErrored();
    }

    // Step f: If teeState.[[closedOrErrored]] is true, return.
    if (teeState->closedOrErrored()) {
        return true;
    }
=======
  // Step 1: Set stream.[[state]] to "readable".
  stream->initStateBits(Readable);
  MOZ_ASSERT(stream->readable());

  // Step 2: Set stream.[[reader]] and stream.[[storedError]] to
  //         undefined (implicit).
  MOZ_ASSERT(!stream->hasReader());
  MOZ_ASSERT(stream->storedError().isUndefined());

  // Step 3: Set stream.[[disturbed]] to false (done in step 1).
  MOZ_ASSERT(!stream->disturbed());

  return stream;
}
>>>>>>> upstream-releases

<<<<<<< HEAD
  // Step vi: If closedOrErrored is true, return.
  if (unwrappedTeeState->closedOrErrored()) {
    return true;
  }

  // Step vii: Let value1 and value2 be value.
  RootedValue value1(cx, value);
  RootedValue value2(cx, value);

  // Step viii: If canceled2 is false and cloneForBranch2 is true,
  //            set value2 to
  //            ? StructuredDeserialize(? StructuredSerialize(value2),
  //                                    the current Realm Record).
  // We don't yet support any specifications that use cloneForBranch2, and
  // the Streams spec doesn't offer any way for author code to enable it,
  // so it's always false here.
  MOZ_ASSERT(!unwrappedTeeState->cloneForBranch2());

  // Step ix: If canceled1 is false, perform
  //          ? ReadableStreamDefaultControllerEnqueue(
  //                branch1.[[readableStreamController]], value1).
  Rooted<ReadableStreamDefaultController*> unwrappedController(cx);
  if (!unwrappedTeeState->canceled1()) {
    unwrappedController = unwrappedTeeState->branch1();
    if (!ReadableStreamDefaultControllerEnqueue(cx, unwrappedController,
                                                value1)) {
      return false;
    }
  }
||||||| merged common ancestors
    // Step g: Let value1 and value2 be value.
    RootedValue value1(cx, value);
    RootedValue value2(cx, value);

    // Step h: If teeState.[[canceled2]] is false and cloneForBranch2 is
    //         true, set value2 to
    //         ? StructuredDeserialize(StructuredSerialize(value2),
    //                                 the current Realm Record).
    // TODO: add StructuredClone() intrinsic.
    MOZ_ASSERT(!teeState->cloneForBranch2(), "tee(cloneForBranch2=true) should not be exposed");

    // Step i: If teeState.[[canceled1]] is false, perform
    //         ? ReadableStreamDefaultControllerEnqueue(branch1, value1).
    Rooted<ReadableStreamDefaultController*> controller(cx);
    if (!teeState->canceled1()) {
        controller = teeState->branch1();
        if (!ReadableStreamDefaultControllerEnqueue(cx, controller, value1)) {
            return false;
        }
    }
=======
// Streams spec, 3.3.6. IsReadableStream ( x )
// Using UnwrapAndTypeCheck templates instead.
>>>>>>> upstream-releases

<<<<<<< HEAD
  // Step x: If canceled2 is false, perform
  //         ? ReadableStreamDefaultControllerEnqueue(
  //               branch2.[[readableStreamController]], value2).
  if (!unwrappedTeeState->canceled2()) {
    unwrappedController = unwrappedTeeState->branch2();
    if (!ReadableStreamDefaultControllerEnqueue(cx, unwrappedController,
                                                value2)) {
      return false;
    }
  }
||||||| merged common ancestors
    // Step j: If teeState.[[canceled2]] is false,
    //         perform ? ReadableStreamDefaultControllerEnqueue(branch2, value2).
    if (!teeState->canceled2()) {
        controller = teeState->branch2();
        if (!ReadableStreamDefaultControllerEnqueue(cx, controller, value2)) {
            return false;
        }
    }
=======
// Streams spec, 3.3.7. IsReadableStreamDisturbed ( stream )
// Using stream->disturbed() instead.
>>>>>>> upstream-releases

<<<<<<< HEAD
  args.rval().setUndefined();
  return true;
||||||| merged common ancestors
    args.rval().setUndefined();
    return true;
=======
/**
 * Streams spec, 3.3.8. IsReadableStreamLocked ( stream )
 */
bool ReadableStream::locked() const {
  // Step 1: Assert: ! IsReadableStream(stream) is true (implicit).
  // Step 2: If stream.[[reader]] is undefined, return false.
  // Step 3: Return true.
  // Special-casing for streams with external sources. Those can be locked
  // explicitly via JSAPI, which is indicated by a controller flag.
  // IsReadableStreamLocked is called from the controller's constructor, at
  // which point we can't yet call stream->controller(), but the source also
  // can't be locked yet.
  if (hasController() && controller()->sourceLocked()) {
    return true;
  }
  return hasReader();
>>>>>>> upstream-releases
}

<<<<<<< HEAD
static MOZ_MUST_USE JSObject* ReadableStreamDefaultReaderRead(
    JSContext* cx, Handle<ReadableStreamDefaultReader*> unwrappedReader);
||||||| merged common ancestors
static MOZ_MUST_USE JSObject*
ReadableStreamTee_Pull(JSContext* cx, Handle<TeeState*> teeState,
                       Handle<ReadableStream*> branchStream)
{
    // Step 1: Let reader be F.[[reader]], branch1 be F.[[branch1]],
    //         branch2 be F.[[branch2]], teeState be F.[[teeState]], and
    //         cloneForBranch2 be F.[[cloneForBranch2]].

    // Step 2: Return the result of transforming
    //         ! ReadableStreamDefaultReaderRead(reader) by a fulfillment
    //         handler which takes the argument result and performs the
    //         following steps:
    Rooted<ReadableStream*> stream(cx, TeeState::stream(cx, teeState));
    if (!stream) {
        return nullptr;
    }
    RootedObject readerObj(cx, ReaderFromStream(cx, stream));
    if (!readerObj) {
        return nullptr;
    }
    Rooted<ReadableStreamDefaultReader*> reader(cx,
                                                &readerObj->as<ReadableStreamDefaultReader>());
=======
static MOZ_MUST_USE bool ReadableStreamDefaultControllerClose(
    JSContext* cx,
    Handle<ReadableStreamDefaultController*> unwrappedController);

static MOZ_MUST_USE bool ReadableStreamDefaultControllerEnqueue(
    JSContext* cx, Handle<ReadableStreamDefaultController*> unwrappedController,
    HandleValue chunk);

/**
 * Streams spec, 3.3.9. ReadableStreamTee steps 12.a.i-ix.
 */
static bool TeeReaderReadHandler(JSContext* cx, unsigned argc, Value* vp) {
  CallArgs args = CallArgsFromVp(argc, vp);
  Rooted<TeeState*> unwrappedTeeState(cx,
                                      UnwrapCalleeSlot<TeeState>(cx, args, 0));
  HandleValue resultVal = args.get(0);

  // Step i: Assert: Type(result) is Object.
  RootedObject result(cx, &resultVal.toObject());

  // Step ii: Let value be ? Get(result, "value").
  // (This can fail only if `result` was nuked.)
  RootedValue value(cx);
  if (!GetProperty(cx, result, result, cx->names().value, &value)) {
    return false;
  }

  // Step iii: Let done be ? Get(result, "done").
  RootedValue doneVal(cx);
  if (!GetProperty(cx, result, result, cx->names().done, &doneVal)) {
    return false;
  }

  // Step iv: Assert: Type(done) is Boolean.
  bool done = doneVal.toBoolean();

  // Step v: If done is true and closedOrErrored is false,
  if (done && !unwrappedTeeState->closedOrErrored()) {
    // Step v.1: If canceled1 is false,
    if (!unwrappedTeeState->canceled1()) {
      // Step v.1.a: Perform ! ReadableStreamDefaultControllerClose(
      //             branch1.[[readableStreamController]]).
      Rooted<ReadableStreamDefaultController*> unwrappedBranch1(
          cx, unwrappedTeeState->branch1());
      if (!ReadableStreamDefaultControllerClose(cx, unwrappedBranch1)) {
        return false;
      }
    }

    // Step v.2: If teeState.[[canceled2]] is false,
    if (!unwrappedTeeState->canceled2()) {
      // Step v.2.a: Perform ! ReadableStreamDefaultControllerClose(
      //             branch2.[[readableStreamController]]).
      Rooted<ReadableStreamDefaultController*> unwrappedBranch2(
          cx, unwrappedTeeState->branch2());
      if (!ReadableStreamDefaultControllerClose(cx, unwrappedBranch2)) {
        return false;
      }
    }
>>>>>>> upstream-releases

<<<<<<< HEAD
/**
 * Streams spec, 3.3.9. ReadableStreamTee step 12, "Let pullAlgorithm be the
 * following steps:"
 */
static MOZ_MUST_USE JSObject* ReadableStreamTee_Pull(
    JSContext* cx, Handle<TeeState*> unwrappedTeeState) {
  // Implicit in the spec: Unpack the closed-over variables `stream` and
  // `reader` from the TeeState.
  Rooted<ReadableStream*> unwrappedStream(
      cx, UnwrapInternalSlot<ReadableStream>(cx, unwrappedTeeState,
                                             TeeState::Slot_Stream));
  if (!unwrappedStream) {
    return nullptr;
  }
  Rooted<ReadableStreamReader*> unwrappedReaderObj(
      cx, UnwrapReaderFromStream(cx, unwrappedStream));
  if (!unwrappedReaderObj) {
    return nullptr;
  }
  Rooted<ReadableStreamDefaultReader*> unwrappedReader(
      cx, &unwrappedReaderObj->as<ReadableStreamDefaultReader>());

  // Step 12.a: Return the result of transforming
  // ! ReadableStreamDefaultReaderRead(reader) with a fulfillment handler
  // which takes the argument result and performs the following steps:
  //
  // The steps under 12.a are implemented in TeeReaderReadHandler.
  RootedObject readPromise(
      cx, ::ReadableStreamDefaultReaderRead(cx, unwrappedReader));
  if (!readPromise) {
    return nullptr;
  }
||||||| merged common ancestors
    RootedObject readPromise(cx, ReadableStreamDefaultReader::read(cx, reader));
    if (!readPromise) {
        return nullptr;
    }
=======
    // Step v.3: Set closedOrErrored to true.
    unwrappedTeeState->setClosedOrErrored();
  }

  // Step vi: If closedOrErrored is true, return.
  if (unwrappedTeeState->closedOrErrored()) {
    return true;
  }

  // Step vii: Let value1 and value2 be value.
  RootedValue value1(cx, value);
  RootedValue value2(cx, value);

  // Step viii: If canceled2 is false and cloneForBranch2 is true,
  //            set value2 to
  //            ? StructuredDeserialize(? StructuredSerialize(value2),
  //                                    the current Realm Record).
  // We don't yet support any specifications that use cloneForBranch2, and
  // the Streams spec doesn't offer any way for author code to enable it,
  // so it's always false here.
  MOZ_ASSERT(!unwrappedTeeState->cloneForBranch2());

  // Step ix: If canceled1 is false, perform
  //          ? ReadableStreamDefaultControllerEnqueue(
  //                branch1.[[readableStreamController]], value1).
  Rooted<ReadableStreamDefaultController*> unwrappedController(cx);
  if (!unwrappedTeeState->canceled1()) {
    unwrappedController = unwrappedTeeState->branch1();
    if (!ReadableStreamDefaultControllerEnqueue(cx, unwrappedController,
                                                value1)) {
      return false;
    }
  }
>>>>>>> upstream-releases

<<<<<<< HEAD
  RootedObject teeState(cx, unwrappedTeeState);
  if (!cx->compartment()->wrap(cx, &teeState)) {
    return nullptr;
  }
  RootedObject onFulfilled(cx, NewHandler(cx, TeeReaderReadHandler, teeState));
  if (!onFulfilled) {
    return nullptr;
  }
||||||| merged common ancestors
    RootedObject onFulfilled(cx, NewHandler(cx, TeeReaderReadHandler, teeState));
    if (!onFulfilled) {
        return nullptr;
    }
=======
  // Step x: If canceled2 is false, perform
  //         ? ReadableStreamDefaultControllerEnqueue(
  //               branch2.[[readableStreamController]], value2).
  if (!unwrappedTeeState->canceled2()) {
    unwrappedController = unwrappedTeeState->branch2();
    if (!ReadableStreamDefaultControllerEnqueue(cx, unwrappedController,
                                                value2)) {
      return false;
    }
  }

  args.rval().setUndefined();
  return true;
}

static MOZ_MUST_USE JSObject* ReadableStreamDefaultReaderRead(
    JSContext* cx, Handle<ReadableStreamDefaultReader*> unwrappedReader);

/**
 * Streams spec, 3.3.9. ReadableStreamTee step 12, "Let pullAlgorithm be the
 * following steps:"
 */
static MOZ_MUST_USE JSObject* ReadableStreamTee_Pull(
    JSContext* cx, Handle<TeeState*> unwrappedTeeState) {
  // Implicit in the spec: Unpack the closed-over variables `stream` and
  // `reader` from the TeeState.
  Rooted<ReadableStream*> unwrappedStream(
      cx, UnwrapInternalSlot<ReadableStream>(cx, unwrappedTeeState,
                                             TeeState::Slot_Stream));
  if (!unwrappedStream) {
    return nullptr;
  }
  Rooted<ReadableStreamReader*> unwrappedReaderObj(
      cx, UnwrapReaderFromStream(cx, unwrappedStream));
  if (!unwrappedReaderObj) {
    return nullptr;
  }
  Rooted<ReadableStreamDefaultReader*> unwrappedReader(
      cx, &unwrappedReaderObj->as<ReadableStreamDefaultReader>());

  // Step 12.a: Return the result of transforming
  // ! ReadableStreamDefaultReaderRead(reader) with a fulfillment handler
  // which takes the argument result and performs the following steps:
  //
  // The steps under 12.a are implemented in TeeReaderReadHandler.
  RootedObject readPromise(
      cx, ::ReadableStreamDefaultReaderRead(cx, unwrappedReader));
  if (!readPromise) {
    return nullptr;
  }
>>>>>>> upstream-releases

<<<<<<< HEAD
  return JS::CallOriginalPromiseThen(cx, readPromise, onFulfilled, nullptr);
||||||| merged common ancestors
    return JS::CallOriginalPromiseThen(cx, readPromise, onFulfilled, nullptr);
=======
  RootedObject teeState(cx, unwrappedTeeState);
  if (!cx->compartment()->wrap(cx, &teeState)) {
    return nullptr;
  }
  RootedObject onFulfilled(cx, NewHandler(cx, TeeReaderReadHandler, teeState));
  if (!onFulfilled) {
    return nullptr;
  }

  return JS::CallOriginalPromiseThen(cx, readPromise, onFulfilled, nullptr);
>>>>>>> upstream-releases
}

/**
<<<<<<< HEAD
 * Cancel one branch of a tee'd stream with the given |reason_|.
 *
 * Streams spec, 3.3.9. ReadableStreamTee steps 13 and 14: "Let
 * cancel1Algorithm/cancel2Algorithm be the following steps, taking a reason
 * argument:"
||||||| merged common ancestors
 * Cancel a tee'd stream's |branch| with the given |reason_|.
 *
 * Note: can operate on unwrapped values for |teeState| and |branch|.
 *
 * Objects created in the course of this function's operation are always
 * created in the current cx compartment.
=======
 * Cancel one branch of a tee'd stream with the given |reason_|.
 *
 * Streams spec, 3.3.9. ReadableStreamTee steps 13 and 14: "Let
 * cancel1Algorithm/cancel2Algorithm be the following steps, taking a reason
 * argument:"
>>>>>>> upstream-releases
 */
static MOZ_MUST_USE JSObject* ReadableStreamTee_Cancel(
    JSContext* cx, Handle<TeeState*> unwrappedTeeState,
    Handle<ReadableStreamDefaultController*> unwrappedBranch,
    HandleValue reason) {
  Rooted<ReadableStream*> unwrappedStream(
      cx, UnwrapInternalSlot<ReadableStream>(cx, unwrappedTeeState,
                                             TeeState::Slot_Stream));
  if (!unwrappedStream) {
    return nullptr;
  }

  bool bothBranchesCanceled = false;

  // Step 13/14.a: Set canceled1/canceled2 to true.
  // Step 13/14.b: Set reason1/reason2 to reason.
  {
    RootedValue unwrappedReason(cx, reason);
    {
<<<<<<< HEAD
      AutoRealm ar(cx, unwrappedTeeState);
      if (!cx->compartment()->wrap(cx, &unwrappedReason)) {
        return nullptr;
      }
    }
    if (unwrappedBranch->isTeeBranch1()) {
      unwrappedTeeState->setCanceled1(unwrappedReason);
      bothBranchesCanceled = unwrappedTeeState->canceled2();
    } else {
      MOZ_ASSERT(unwrappedBranch->isTeeBranch2());
      unwrappedTeeState->setCanceled2(unwrappedReason);
      bothBranchesCanceled = unwrappedTeeState->canceled1();
    }
  }

  // Step 13/14.c: If canceled2/canceled1 is true,
  if (bothBranchesCanceled) {
    // Step 13/14.c.i: Let compositeReason be
    //                 ! CreateArrayFromList(« reason1, reason2 »).
    RootedNativeObject compositeReason(cx, NewDenseFullyAllocatedArray(cx, 2));
    if (!compositeReason) {
      return nullptr;
    }

    compositeReason->setDenseInitializedLength(2);

    RootedValue reason1(cx, unwrappedTeeState->reason1());
    RootedValue reason2(cx, unwrappedTeeState->reason2());
    if (!cx->compartment()->wrap(cx, &reason1) ||
        !cx->compartment()->wrap(cx, &reason2)) {
      return nullptr;
    }
    compositeReason->initDenseElement(0, reason1);
    compositeReason->initDenseElement(1, reason2);
    RootedValue compositeReasonVal(cx, ObjectValue(*compositeReason));

    // Step 13/14.c.ii: Let cancelResult be
    //                  ! ReadableStreamCancel(stream, compositeReason).
    // In our implementation, this can fail with OOM. The best course then
    // is to reject cancelPromise with an OOM error.
    RootedObject cancelResult(
        cx, ::ReadableStreamCancel(cx, unwrappedStream, compositeReasonVal));
    {
      Rooted<PromiseObject*> cancelPromise(cx,
                                           unwrappedTeeState->cancelPromise());
      AutoRealm ar(cx, cancelPromise);

      if (!cancelResult) {
        // Handle the OOM case mentioned above.
        if (!RejectPromiseWithPendingError(cx, cancelPromise)) {
          return nullptr;
        }
      } else {
        // Step 13/14.c.iii: Resolve cancelPromise with cancelResult.
        RootedValue resultVal(cx, ObjectValue(*cancelResult));
        if (!cx->compartment()->wrap(cx, &resultVal)) {
          return nullptr;
        }
        if (!PromiseObject::resolve(cx, cancelPromise, resultVal)) {
          return nullptr;
        }
      }
||||||| merged common ancestors
        RootedValue reason(cx, reason_);
        if (reason.isGCThing() &&
            reason.toGCThing()->maybeCompartment() != teeState->compartment())
        {
            AutoRealm ar(cx, teeState);
            if (!cx->compartment()->wrap(cx, &reason)) {
                return nullptr;
            }
        }
        if (ControllerFlags(branch) & ControllerFlag_TeeBranch1) {
            teeState->setCanceled1(reason);
            bothBranchesCanceled = teeState->canceled2();
        } else {
            MOZ_ASSERT(ControllerFlags(branch) & ControllerFlag_TeeBranch2);
            teeState->setCanceled2(reason);
            bothBranchesCanceled = teeState->canceled1();
        }
=======
      AutoRealm ar(cx, unwrappedTeeState);
      if (!cx->compartment()->wrap(cx, &unwrappedReason)) {
        return nullptr;
      }
>>>>>>> upstream-releases
    }
<<<<<<< HEAD
  }

  // Step 13/14.d: Return cancelPromise.
  RootedObject cancelPromise(cx, unwrappedTeeState->cancelPromise());
  if (!cx->compartment()->wrap(cx, &cancelPromise)) {
    return nullptr;
  }
  return cancelPromise;
}

static MOZ_MUST_USE bool ReadableStreamControllerError(
    JSContext* cx, Handle<ReadableStreamController*> unwrappedController,
    HandleValue e);

/**
 * Streams spec, 3.3.9. step 18:
 * Upon rejection of reader.[[closedPromise]] with reason r,
 */
static bool TeeReaderClosedHandler(JSContext* cx, unsigned argc, Value* vp) {
  CallArgs args = CallArgsFromVp(argc, vp);
  Rooted<TeeState*> teeState(cx, TargetFromHandler<TeeState>(args));
  HandleValue reason = args.get(0);

  // Step a: If closedOrErrored is false, then:
  if (!teeState->closedOrErrored()) {
    // Step a.iii: Set closedOrErrored to true.
    // Reordered to ensure that internal errors in the other steps don't
    // leave the teeState in an undefined state.
    teeState->setClosedOrErrored();

    // Step a.i: Perform
    //           ! ReadableStreamDefaultControllerError(
    //               branch1.[[readableStreamController]], r).
    Rooted<ReadableStreamDefaultController*> branch1(cx, teeState->branch1());
    if (!ReadableStreamControllerError(cx, branch1, reason)) {
      return false;
||||||| merged common ancestors

    // Step 4: If teeState.[[canceled1]] is true,
    // Step 4: If teeState.[[canceled2]] is true,
    if (bothBranchesCanceled) {
        // Step a: Let compositeReason be
        //         ! CreateArrayFromList(« teeState.[[reason1]], teeState.[[reason2]] »).
        RootedNativeObject compositeReason(cx, NewDenseFullyAllocatedArray(cx, 2));
        if (!compositeReason) {
            return nullptr;
        }

        compositeReason->setDenseInitializedLength(2);

        RootedValue reason1(cx, teeState->reason1());
        RootedValue reason2(cx, teeState->reason2());
        if (teeState->compartment() != cx->compartment()) {
            if (!cx->compartment()->wrap(cx, &reason1) || !cx->compartment()->wrap(cx, &reason2)) {
                return nullptr;
            }
        }
        compositeReason->initDenseElement(0, reason1);
        compositeReason->initDenseElement(1, reason2);
        RootedValue compositeReasonVal(cx, ObjectValue(*compositeReason));

        Rooted<PromiseObject*> promise(cx, teeState->promise());

        // Step b: Let cancelResult be ! ReadableStreamCancel(stream, compositeReason).
        RootedObject cancelResult(cx, ReadableStream::cancel(cx, stream, compositeReasonVal));
        {
            AutoRealm ar(cx, promise);
            if (!cancelResult) {
                if (!RejectWithPendingError(cx, promise)) {
                    return nullptr;
                }
            } else {
                // Step c: Resolve teeState.[[promise]] with cancelResult.
                RootedValue resultVal(cx, ObjectValue(*cancelResult));
                if (!cx->compartment()->wrap(cx, &resultVal)) {
                    return nullptr;
                }
                if (!PromiseObject::resolve(cx, promise, resultVal)) {
                    return nullptr;
                }
            }
        }
=======
    if (unwrappedBranch->isTeeBranch1()) {
      unwrappedTeeState->setCanceled1(unwrappedReason);
      bothBranchesCanceled = unwrappedTeeState->canceled2();
    } else {
      MOZ_ASSERT(unwrappedBranch->isTeeBranch2());
      unwrappedTeeState->setCanceled2(unwrappedReason);
      bothBranchesCanceled = unwrappedTeeState->canceled1();
    }
  }

  // Step 13/14.c: If canceled2/canceled1 is true,
  if (bothBranchesCanceled) {
    // Step 13/14.c.i: Let compositeReason be
    //                 ! CreateArrayFromList(« reason1, reason2 »).
    RootedValue reason1(cx, unwrappedTeeState->reason1());
    RootedValue reason2(cx, unwrappedTeeState->reason2());
    if (!cx->compartment()->wrap(cx, &reason1) ||
        !cx->compartment()->wrap(cx, &reason2)) {
      return nullptr;
    }

    RootedNativeObject compositeReason(cx, NewDenseFullyAllocatedArray(cx, 2));
    if (!compositeReason) {
      return nullptr;
    }
    compositeReason->setDenseInitializedLength(2);
    compositeReason->initDenseElement(0, reason1);
    compositeReason->initDenseElement(1, reason2);
    RootedValue compositeReasonVal(cx, ObjectValue(*compositeReason));

    // Step 13/14.c.ii: Let cancelResult be
    //                  ! ReadableStreamCancel(stream, compositeReason).
    // In our implementation, this can fail with OOM. The best course then
    // is to reject cancelPromise with an OOM error.
    RootedObject cancelResult(
        cx, ::ReadableStreamCancel(cx, unwrappedStream, compositeReasonVal));
    {
      Rooted<PromiseObject*> cancelPromise(cx,
                                           unwrappedTeeState->cancelPromise());
      AutoRealm ar(cx, cancelPromise);

      if (!cancelResult) {
        // Handle the OOM case mentioned above.
        if (!RejectPromiseWithPendingError(cx, cancelPromise)) {
          return nullptr;
        }
      } else {
        // Step 13/14.c.iii: Resolve cancelPromise with cancelResult.
        RootedValue resultVal(cx, ObjectValue(*cancelResult));
        if (!cx->compartment()->wrap(cx, &resultVal)) {
          return nullptr;
        }
        if (!PromiseObject::resolve(cx, cancelPromise, resultVal)) {
          return nullptr;
        }
      }
>>>>>>> upstream-releases
    }
  }

<<<<<<< HEAD
    // Step a.ii: Perform
    //            ! ReadableStreamDefaultControllerError(
    //                branch2.[[readableStreamController]], r).
    Rooted<ReadableStreamDefaultController*> branch2(cx, teeState->branch2());
    if (!ReadableStreamControllerError(cx, branch2, reason)) {
      return false;
    }
  }

  args.rval().setUndefined();
  return true;
}
||||||| merged common ancestors
    // Step 5: Return teeState.[[promise]].
    RootedObject promise(cx, teeState->promise());
    if (promise->compartment() != cx->compartment()) {
        if (!cx->compartment()->wrap(cx, &promise)) {
            return nullptr;
        }
    }
    return promise;
}

static MOZ_MUST_USE bool
ReadableStreamDefaultControllerErrorIfNeeded(JSContext* cx,
                                             Handle<ReadableStreamDefaultController*> controller,
                                             HandleValue e);

// Streams spec, 3.3.6. step 21:
// Upon rejection of reader.[[closedPromise]] with reason r,
static bool
TeeReaderClosedHandler(JSContext* cx, unsigned argc, Value* vp)
{
    CallArgs args = CallArgsFromVp(argc, vp);
    Rooted<TeeState*> teeState(cx, TargetFromHandler<TeeState>(args.callee()));
    HandleValue reason = args.get(0);

    // Step a: If teeState.[[closedOrErrored]] is false, then:
    if (!teeState->closedOrErrored()) {
        // Step a.iii: Set teeState.[[closedOrErrored]] to true.
        // Reordered to ensure that internal errors in the other steps don't
        // leave the teeState in an undefined state.
        teeState->setClosedOrErrored();

        // Step a.i: Perform ! ReadableStreamDefaultControllerErrorIfNeeded(pull.[[branch1]], r).
        Rooted<ReadableStreamDefaultController*> branch1(cx, teeState->branch1());
        if (!ReadableStreamDefaultControllerErrorIfNeeded(cx, branch1, reason)) {
            return false;
        }

        // Step a.ii: Perform ! ReadableStreamDefaultControllerErrorIfNeeded(pull.[[branch2]], r).
        Rooted<ReadableStreamDefaultController*> branch2(cx, teeState->branch2());
        if (!ReadableStreamDefaultControllerErrorIfNeeded(cx, branch2, reason)) {
            return false;
        }
    }

    return true;
}
=======
  // Step 13/14.d: Return cancelPromise.
  RootedObject cancelPromise(cx, unwrappedTeeState->cancelPromise());
  if (!cx->compartment()->wrap(cx, &cancelPromise)) {
    return nullptr;
  }
  return cancelPromise;
}

static MOZ_MUST_USE bool ReadableStreamControllerError(
    JSContext* cx, Handle<ReadableStreamController*> unwrappedController,
    HandleValue e);
>>>>>>> upstream-releases

/**
<<<<<<< HEAD
 * Streams spec, 3.3.9. ReadableStreamTee ( stream, cloneForBranch2 )
||||||| merged common ancestors
 * Streams spec, 3.3.6. ReadableStreamTee ( stream, cloneForBranch2 )
 *
 * Note: can operate on unwrapped ReadableStream instances from
 * another compartment. The returned branch streams and their associated
 * controllers  are always created in the current cx compartment.
=======
 * Streams spec, 3.3.9. step 18:
 * Upon rejection of reader.[[closedPromise]] with reason r,
>>>>>>> upstream-releases
 */
<<<<<<< HEAD
static MOZ_MUST_USE bool ReadableStreamTee(
    JSContext* cx, Handle<ReadableStream*> unwrappedStream,
    bool cloneForBranch2, MutableHandle<ReadableStream*> branch1Stream,
    MutableHandle<ReadableStream*> branch2Stream) {
  // Step 1: Assert: ! IsReadableStream(stream) is true (implicit).
  // Step 2: Assert: Type(cloneForBranch2) is Boolean (implicit).

  // Step 3: Let reader be ? AcquireReadableStreamDefaultReader(stream).
  Rooted<ReadableStreamDefaultReader*> reader(
      cx, CreateReadableStreamDefaultReader(cx, unwrappedStream));
  if (!reader) {
    return false;
  }

  // Several algorithms close over the variables initialized in the next few
  // steps, so we allocate them in an object, the TeeState. The algorithms
  // also close over `stream` and `reader`, so TeeState gets a reference to
  // the stream.
  //
  // Step 4: Let closedOrErrored be false.
  // Step 5: Let canceled1 be false.
  // Step 6: Let canceled2 be false.
  // Step 7: Let reason1 be undefined.
  // Step 8: Let reason2 be undefined.
  // Step 9: Let branch1 be undefined.
  // Step 10: Let branch2 be undefined.
  // Step 11: Let cancelPromise be a new promise.
  Rooted<TeeState*> teeState(cx, TeeState::create(cx, unwrappedStream));
  if (!teeState) {
    return false;
  }

  // Step 12: Let pullAlgorithm be the following steps: [...]
  // Step 13: Let cancel1Algorithm be the following steps: [...]
  // Step 14: Let cancel2Algorithm be the following steps: [...]
  // Step 15: Let startAlgorithm be an algorithm that returns undefined.
  //
  // Implicit. Our implementation does not use objects to represent
  // [[pullAlgorithm]], [[cancelAlgorithm]], and so on. Instead, we decide
  // which one to perform based on class checks. For example, our
  // implementation of ReadableStreamControllerCallPullIfNeeded checks
  // whether the stream's underlyingSource is a TeeState object.

  // Step 16: Set branch1 to
  //          ! CreateReadableStream(startAlgorithm, pullAlgorithm,
  //                                 cancel1Algorithm).
  RootedValue underlyingSource(cx, ObjectValue(*teeState));
  branch1Stream.set(
      CreateReadableStream(cx, SourceAlgorithms::Tee, underlyingSource));
  if (!branch1Stream) {
    return false;
  }

  Rooted<ReadableStreamDefaultController*> branch1(cx);
  branch1 = &branch1Stream->controller()->as<ReadableStreamDefaultController>();
  branch1->setTeeBranch1();
  teeState->setBranch1(branch1);

  // Step 17: Set branch2 to
  //          ! CreateReadableStream(startAlgorithm, pullAlgorithm,
  //                                 cancel2Algorithm).
  branch2Stream.set(
      CreateReadableStream(cx, SourceAlgorithms::Tee, underlyingSource));
  if (!branch2Stream) {
    return false;
  }
||||||| merged common ancestors
static MOZ_MUST_USE bool
ReadableStreamTee(JSContext* cx, Handle<ReadableStream*> stream, bool cloneForBranch2,
                  MutableHandle<ReadableStream*> branch1Stream,
                  MutableHandle<ReadableStream*> branch2Stream)
{
    // Step 1: Assert: ! IsReadableStream(stream) is true (implicit).
    // Step 2: Assert: Type(cloneForBranch2) is Boolean (implicit).

    // Step 3: Let reader be ? AcquireReadableStreamDefaultReader(stream).
    Rooted<ReadableStreamDefaultReader*> reader(cx, CreateReadableStreamDefaultReader(cx, stream));
    if (!reader) {
        return false;
    }

    // Step 4: Let teeState be Record {[[closedOrErrored]]: false,
    //                                 [[canceled1]]: false,
    //                                 [[canceled2]]: false,
    //                                 [[reason1]]: undefined,
    //                                 [[reason2]]: undefined,
    //                                 [[promise]]: a new promise}.
    Rooted<TeeState*> teeState(cx, TeeState::create(cx, stream));
    if (!teeState) {
        return false;
    }

    // Steps 5-10 omitted because our implementation works differently.

    // Step 5: Let pull be a new ReadableStreamTee pull function.
    // Step 6: Set pull.[[reader]] to reader, pull.[[teeState]] to teeState, and
    //         pull.[[cloneForBranch2]] to cloneForBranch2.
    // Step 7: Let cancel1 be a new ReadableStreamTee branch 1 cancel function.
    // Step 8: Set cancel1.[[stream]] to stream and cancel1.[[teeState]] to
    //         teeState.

    // Step 9: Let cancel2 be a new ReadableStreamTee branch 2 cancel function.
    // Step 10: Set cancel2.[[stream]] to stream and cancel2.[[teeState]] to
    //          teeState.

    // Step 11: Let underlyingSource1 be ! ObjectCreate(%ObjectPrototype%).
    // Step 12: Perform ! CreateDataProperty(underlyingSource1, "pull", pull).
    // Step 13: Perform ! CreateDataProperty(underlyingSource1, "cancel", cancel1).

    // Step 14: Let branch1Stream be ! Construct(ReadableStream, underlyingSource1).
    RootedValue hwmValue(cx, NumberValue(1));
    RootedValue underlyingSource(cx, ObjectValue(*teeState));
    branch1Stream.set(ReadableStream::createDefaultStream(cx, underlyingSource,
                                                          UndefinedHandleValue,
                                                          hwmValue));
    if (!branch1Stream) {
        return false;
    }

    Rooted<ReadableStreamDefaultController*> branch1(cx);
    branch1 = &ControllerFromStream(branch1Stream)->as<ReadableStreamDefaultController>();
    AddControllerFlags(branch1, ControllerFlag_TeeBranch | ControllerFlag_TeeBranch1);
    teeState->setBranch1(branch1);

    // Step 15: Let underlyingSource2 be ! ObjectCreate(%ObjectPrototype%).
    // Step 16: Perform ! CreateDataProperty(underlyingSource2, "pull", pull).
    // Step 17: Perform ! CreateDataProperty(underlyingSource2, "cancel", cancel2).

    // Step 18: Let branch2Stream be ! Construct(ReadableStream, underlyingSource2).
    branch2Stream.set(ReadableStream::createDefaultStream(cx, underlyingSource,
                                                          UndefinedHandleValue,
                                                          hwmValue));
    if (!branch2Stream) {
        return false;
    }

    Rooted<ReadableStreamDefaultController*> branch2(cx);
    branch2 = &ControllerFromStream(branch2Stream)->as<ReadableStreamDefaultController>();
    AddControllerFlags(branch2, ControllerFlag_TeeBranch | ControllerFlag_TeeBranch2);
    teeState->setBranch2(branch2);
=======
static bool TeeReaderErroredHandler(JSContext* cx, unsigned argc, Value* vp) {
  CallArgs args = CallArgsFromVp(argc, vp);
  Rooted<TeeState*> teeState(cx, TargetFromHandler<TeeState>(args));
  HandleValue reason = args.get(0);

  // Step a: If closedOrErrored is false, then:
  if (!teeState->closedOrErrored()) {
    // Step a.iii: Set closedOrErrored to true.
    // Reordered to ensure that internal errors in the other steps don't
    // leave the teeState in an undefined state.
    teeState->setClosedOrErrored();

    // Step a.i: Perform
    //           ! ReadableStreamDefaultControllerError(
    //               branch1.[[readableStreamController]], r).
    Rooted<ReadableStreamDefaultController*> branch1(cx, teeState->branch1());
    if (!ReadableStreamControllerError(cx, branch1, reason)) {
      return false;
    }

    // Step a.ii: Perform
    //            ! ReadableStreamDefaultControllerError(
    //                branch2.[[readableStreamController]], r).
    Rooted<ReadableStreamDefaultController*> branch2(cx, teeState->branch2());
    if (!ReadableStreamControllerError(cx, branch2, reason)) {
      return false;
    }
  }

  args.rval().setUndefined();
  return true;
}

/**
 * Streams spec, 3.3.9. ReadableStreamTee ( stream, cloneForBranch2 )
 */
static MOZ_MUST_USE bool ReadableStreamTee(
    JSContext* cx, Handle<ReadableStream*> unwrappedStream,
    bool cloneForBranch2, MutableHandle<ReadableStream*> branch1Stream,
    MutableHandle<ReadableStream*> branch2Stream) {
  // Step 1: Assert: ! IsReadableStream(stream) is true (implicit).
  // Step 2: Assert: Type(cloneForBranch2) is Boolean (implicit).

  // Step 3: Let reader be ? AcquireReadableStreamDefaultReader(stream).
  Rooted<ReadableStreamDefaultReader*> reader(
      cx, CreateReadableStreamDefaultReader(cx, unwrappedStream));
  if (!reader) {
    return false;
  }

  // Several algorithms close over the variables initialized in the next few
  // steps, so we allocate them in an object, the TeeState. The algorithms
  // also close over `stream` and `reader`, so TeeState gets a reference to
  // the stream.
  //
  // Step 4: Let closedOrErrored be false.
  // Step 5: Let canceled1 be false.
  // Step 6: Let canceled2 be false.
  // Step 7: Let reason1 be undefined.
  // Step 8: Let reason2 be undefined.
  // Step 9: Let branch1 be undefined.
  // Step 10: Let branch2 be undefined.
  // Step 11: Let cancelPromise be a new promise.
  Rooted<TeeState*> teeState(cx, TeeState::create(cx, unwrappedStream));
  if (!teeState) {
    return false;
  }

  // Step 12: Let pullAlgorithm be the following steps: [...]
  // Step 13: Let cancel1Algorithm be the following steps: [...]
  // Step 14: Let cancel2Algorithm be the following steps: [...]
  // Step 15: Let startAlgorithm be an algorithm that returns undefined.
  //
  // Implicit. Our implementation does not use objects to represent
  // [[pullAlgorithm]], [[cancelAlgorithm]], and so on. Instead, we decide
  // which one to perform based on class checks. For example, our
  // implementation of ReadableStreamControllerCallPullIfNeeded checks
  // whether the stream's underlyingSource is a TeeState object.

  // Step 16: Set branch1 to
  //          ! CreateReadableStream(startAlgorithm, pullAlgorithm,
  //                                 cancel1Algorithm).
  RootedValue underlyingSource(cx, ObjectValue(*teeState));
  branch1Stream.set(
      CreateReadableStream(cx, SourceAlgorithms::Tee, underlyingSource));
  if (!branch1Stream) {
    return false;
  }

  Rooted<ReadableStreamDefaultController*> branch1(cx);
  branch1 = &branch1Stream->controller()->as<ReadableStreamDefaultController>();
  branch1->setTeeBranch1();
  teeState->setBranch1(branch1);

  // Step 17: Set branch2 to
  //          ! CreateReadableStream(startAlgorithm, pullAlgorithm,
  //                                 cancel2Algorithm).
  branch2Stream.set(
      CreateReadableStream(cx, SourceAlgorithms::Tee, underlyingSource));
  if (!branch2Stream) {
    return false;
  }
>>>>>>> upstream-releases

  Rooted<ReadableStreamDefaultController*> branch2(cx);
  branch2 = &branch2Stream->controller()->as<ReadableStreamDefaultController>();
  branch2->setTeeBranch2();
  teeState->setBranch2(branch2);

  // Step 18: Upon rejection of reader.[[closedPromise]] with reason r, [...]
  RootedObject closedPromise(cx, reader->closedPromise());

<<<<<<< HEAD
  RootedObject onRejected(cx, NewHandler(cx, TeeReaderClosedHandler, teeState));
  if (!onRejected) {
    return false;
  }
||||||| merged common ancestors
    RootedObject onRejected(cx, NewHandler(cx, TeeReaderClosedHandler, teeState));
    if (!onRejected) {
        return false;
    }
=======
  RootedObject onRejected(cx,
                          NewHandler(cx, TeeReaderErroredHandler, teeState));
  if (!onRejected) {
    return false;
  }
>>>>>>> upstream-releases

  if (!JS::AddPromiseReactions(cx, closedPromise, nullptr, onRejected)) {
    return false;
  }

  // Step 19: Return « branch1, branch2 ».
  return true;
}

/*** 3.4. The interface between readable streams and controllers ************/

inline static MOZ_MUST_USE bool AppendToListAtSlot(
    JSContext* cx, HandleNativeObject unwrappedContainer, uint32_t slot,
    HandleObject obj);

/**
<<<<<<< HEAD
 * Streams spec, 3.4.1. ReadableStreamAddReadIntoRequest ( stream, forAuthorCode
 * ) Streams spec, 3.4.2. ReadableStreamAddReadRequest ( stream, forAuthorCode )
||||||| merged common ancestors
 * Streams spec, 3.4.1. ReadableStreamAddReadIntoRequest ( stream )
 * Streams spec, 3.4.2. ReadableStreamAddReadRequest ( stream )
=======
 * Streams spec, 3.4.1.
 *      ReadableStreamAddReadIntoRequest ( stream, forAuthorCode )
 * Streams spec, 3.4.2.
 *      ReadableStreamAddReadRequest ( stream, forAuthorCode )
>>>>>>> upstream-releases
 *
 * Our implementation does not pass around forAuthorCode parameters in the same
 * places as the standard, but the effect is the same. See the comment on
 * `ReadableStreamReader::forAuthorCode()`.
 */
<<<<<<< HEAD
static MOZ_MUST_USE JSObject* ReadableStreamAddReadOrReadIntoRequest(
    JSContext* cx, Handle<ReadableStream*> unwrappedStream) {
  // Step 1: Assert: ! IsReadableStreamBYOBReader(stream.[[reader]]) is true.
  // Skipped: handles both kinds of readers.
  Rooted<ReadableStreamReader*> unwrappedReader(
      cx, UnwrapReaderFromStream(cx, unwrappedStream));
  if (!unwrappedReader) {
    return nullptr;
  }

  // Step 2 of 3.4.2: Assert: stream.[[state]] is "readable".
  MOZ_ASSERT_IF(unwrappedReader->is<ReadableStreamDefaultReader>(),
                unwrappedStream->readable());

  // Step 3: Let promise be a new promise.
  RootedObject promise(cx, PromiseObject::createSkippingExecutor(cx));
  if (!promise) {
    return nullptr;
  }

  // Step 4: Let read{Into}Request be
  //         Record {[[promise]]: promise, [[forAuthorCode]]: forAuthorCode}.
  // Step 5: Append read{Into}Request as the last element of
  //         stream.[[reader]].[[read{Into}Requests]].
  // Since we don't need the [[forAuthorCode]] field (see the comment on
  // `ReadableStreamReader::forAuthorCode()`), we elide the Record and store
  // only the promise.
  if (!AppendToListAtSlot(cx, unwrappedReader,
                          ReadableStreamReader::Slot_Requests, promise)) {
    return nullptr;
  }
||||||| merged common ancestors
static MOZ_MUST_USE JSObject*
ReadableStreamAddReadOrReadIntoRequest(JSContext* cx, Handle<ReadableStream*> stream)
{
    // Step 1: Assert: ! IsReadableStreamBYOBReader(stream.[[reader]]) is true.
    // Skipped: handles both kinds of readers.
    Rooted<ReadableStreamReader*> reader(cx, ReaderFromStream(cx, stream));
    if (!reader) {
        return nullptr;
    }

    // Step 2 of 3.4.2: Assert: stream.[[state]] is "readable".
    MOZ_ASSERT_IF(reader->is<ReadableStreamDefaultReader>(), stream->readable());

    // Step 3: Let promise be a new promise.
    RootedObject promise(cx, PromiseObject::createSkippingExecutor(cx));
    if (!promise) {
        return nullptr;
    }

    // Step 4: Let read{Into}Request be Record {[[promise]]: promise}.
    // Step 5: Append read{Into}Request as the last element of
    //         stream.[[reader]].[[read{Into}Requests]].
    // Since [[promise]] is the Record's only field, we store it directly.
    if (!AppendToListAtSlot(cx, reader, ReaderSlot_Requests, promise)) {
        return nullptr;
    }
=======
static MOZ_MUST_USE JSObject* ReadableStreamAddReadOrReadIntoRequest(
    JSContext* cx, Handle<ReadableStream*> unwrappedStream) {
  // Step 1: Assert: ! IsReadableStream{BYOB,Default}Reader(stream.[[reader]])
  //         is true.
  // (Only default readers exist so far.)
  Rooted<ReadableStreamReader*> unwrappedReader(
      cx, UnwrapReaderFromStream(cx, unwrappedStream));
  if (!unwrappedReader) {
    return nullptr;
  }
  MOZ_ASSERT(unwrappedReader->is<ReadableStreamDefaultReader>());

  // Step 2 of 3.4.1: Assert: stream.[[state]] is "readable" or "closed".
  // Step 2 of 3.4.2: Assert: stream.[[state]] is "readable".
  MOZ_ASSERT(unwrappedStream->readable() || unwrappedStream->closed());
  MOZ_ASSERT_IF(unwrappedReader->is<ReadableStreamDefaultReader>(),
                unwrappedStream->readable());

  // Step 3: Let promise be a new promise.
  RootedObject promise(cx, PromiseObject::createSkippingExecutor(cx));
  if (!promise) {
    return nullptr;
  }

  // Step 4: Let read{Into}Request be
  //         Record {[[promise]]: promise, [[forAuthorCode]]: forAuthorCode}.
  // Step 5: Append read{Into}Request as the last element of
  //         stream.[[reader]].[[read{Into}Requests]].
  // Since we don't need the [[forAuthorCode]] field (see the comment on
  // `ReadableStreamReader::forAuthorCode()`), we elide the Record and store
  // only the promise.
  if (!AppendToListAtSlot(cx, unwrappedReader,
                          ReadableStreamReader::Slot_Requests, promise)) {
    return nullptr;
  }
>>>>>>> upstream-releases

  // Step 6: Return promise.
  return promise;
}

static MOZ_MUST_USE JSObject* ReadableStreamControllerCancelSteps(
    JSContext* cx, Handle<ReadableStreamController*> unwrappedController,
    HandleValue reason);

/**
 * Used for transforming the result of promise fulfillment/rejection.
 */
static bool ReturnUndefined(JSContext* cx, unsigned argc, Value* vp) {
  CallArgs args = CallArgsFromVp(argc, vp);
  args.rval().setUndefined();
  return true;
}

MOZ_MUST_USE bool ReadableStreamCloseInternal(
    JSContext* cx, Handle<ReadableStream*> unwrappedStream);

/**
 * Streams spec, 3.4.3. ReadableStreamCancel ( stream, reason )
 */
<<<<<<< HEAD
static MOZ_MUST_USE JSObject* ReadableStreamCancel(
    JSContext* cx, Handle<ReadableStream*> unwrappedStream,
    HandleValue reason) {
  AssertSameCompartment(cx, reason);

  // Step 1: Set stream.[[disturbed]] to true.
  unwrappedStream->setDisturbed();

  // Step 2: If stream.[[state]] is "closed", return a new promise resolved
  //         with undefined.
  if (unwrappedStream->closed()) {
    return PromiseObject::unforgeableResolve(cx, UndefinedHandleValue);
  }

  // Step 3: If stream.[[state]] is "errored", return a new promise rejected
  //         with stream.[[storedError]].
  if (unwrappedStream->errored()) {
    RootedValue storedError(cx, unwrappedStream->storedError());
    if (!cx->compartment()->wrap(cx, &storedError)) {
      return nullptr;
    }
    return PromiseObject::unforgeableReject(cx, storedError);
  }

  // Step 4: Perform ! ReadableStreamClose(stream).
  if (!ReadableStreamCloseInternal(cx, unwrappedStream)) {
    return nullptr;
  }

  // Step 5: Let sourceCancelPromise be
  //         ! stream.[[readableStreamController]].[[CancelSteps]](reason).
  Rooted<ReadableStreamController*> unwrappedController(
      cx, unwrappedStream->controller());
  RootedObject sourceCancelPromise(
      cx, ReadableStreamControllerCancelSteps(cx, unwrappedController, reason));
  if (!sourceCancelPromise) {
    return nullptr;
  }

  // Step 6: Return the result of transforming sourceCancelPromise by a
  //         fulfillment handler that returns undefined.
  RootedAtom funName(cx, cx->names().empty);
  RootedFunction returnUndefined(
      cx, NewNativeFunction(cx, ReturnUndefined, 0, funName));
  if (!returnUndefined) {
    return nullptr;
  }
  return JS::CallOriginalPromiseThen(cx, sourceCancelPromise, returnUndefined,
                                     nullptr);
||||||| merged common ancestors
/* static */ MOZ_MUST_USE JSObject*
ReadableStream::cancel(JSContext* cx, Handle<ReadableStream*> stream, HandleValue reason)
{
    AssertSameCompartment(cx, reason);

    // Step 1: Set stream.[[disturbed]] to true.
    uint32_t state = StreamState(stream) | ReadableStream::Disturbed;
    SetStreamState(stream, state);

    // Step 2: If stream.[[state]] is "closed", return a new promise resolved
    //         with undefined.
    if (stream->closed()) {
        return PromiseObject::unforgeableResolve(cx, UndefinedHandleValue);
    }

    // Step 3: If stream.[[state]] is "errored", return a new promise rejected
    //         with stream.[[storedError]].
    if (stream->errored()) {
        RootedValue storedError(cx, stream->getFixedSlot(StreamSlot_StoredError));
        if (!cx->compartment()->wrap(cx, &storedError)) {
            return nullptr;
        }
        return PromiseObject::unforgeableReject(cx, storedError);
    }

    // Step 4: Perform ! ReadableStreamClose(stream).
    if (!ReadableStreamCloseInternal(cx, stream)) {
        return nullptr;
    }

    // Step 5: Let sourceCancelPromise be
    //         ! stream.[[readableStreamController]].[[CancelSteps]](reason).
    Rooted<ReadableStreamController*> controller(cx, ControllerFromStream(stream));
    RootedObject sourceCancelPromise(cx);
    sourceCancelPromise = ReadableStreamControllerCancelSteps(cx, controller, reason);
    if (!sourceCancelPromise) {
        return nullptr;
    }

    // Step 6: Return the result of transforming sourceCancelPromise by a
    //         fulfillment handler that returns undefined.
    RootedAtom funName(cx, cx->names().empty);
    RootedFunction returnUndefined(cx, NewNativeFunction(cx, ReturnUndefined, 0, funName));
    if (!returnUndefined) {
        return nullptr;
    }
    return JS::CallOriginalPromiseThen(cx, sourceCancelPromise, returnUndefined, nullptr);
=======
static MOZ_MUST_USE JSObject* ReadableStreamCancel(
    JSContext* cx, Handle<ReadableStream*> unwrappedStream,
    HandleValue reason) {
  AssertSameCompartment(cx, reason);

  // Step 1: Set stream.[[disturbed]] to true.
  unwrappedStream->setDisturbed();

  // Step 2: If stream.[[state]] is "closed", return a new promise resolved
  //         with undefined.
  if (unwrappedStream->closed()) {
    return PromiseObject::unforgeableResolve(cx, UndefinedHandleValue);
  }

  // Step 3: If stream.[[state]] is "errored", return a new promise rejected
  //         with stream.[[storedError]].
  if (unwrappedStream->errored()) {
    RootedValue storedError(cx, unwrappedStream->storedError());
    if (!cx->compartment()->wrap(cx, &storedError)) {
      return nullptr;
    }
    return PromiseObject::unforgeableReject(cx, storedError);
  }

  // Step 4: Perform ! ReadableStreamClose(stream).
  if (!ReadableStreamCloseInternal(cx, unwrappedStream)) {
    return nullptr;
  }

  // Step 5: Let sourceCancelPromise be
  //         ! stream.[[readableStreamController]].[[CancelSteps]](reason).
  Rooted<ReadableStreamController*> unwrappedController(
      cx, unwrappedStream->controller());
  RootedObject sourceCancelPromise(
      cx, ReadableStreamControllerCancelSteps(cx, unwrappedController, reason));
  if (!sourceCancelPromise) {
    return nullptr;
  }

  // Step 6: Return the result of transforming sourceCancelPromise with a
  //         fulfillment handler that returns undefined.
  HandlePropertyName funName = cx->names().empty;
  RootedFunction returnUndefined(
      cx, NewNativeFunction(cx, ReturnUndefined, 0, funName,
                            gc::AllocKind::FUNCTION, GenericObject));
  if (!returnUndefined) {
    return nullptr;
  }
  return JS::CallOriginalPromiseThen(cx, sourceCancelPromise, returnUndefined,
                                     nullptr);
>>>>>>> upstream-releases
}

static MOZ_MUST_USE JSObject* ReadableStreamCreateReadResult(
    JSContext* cx, HandleValue value, bool done,
    ForAuthorCodeBool forAuthorCode);

/**
 * Streams spec, 3.4.4. ReadableStreamClose ( stream )
 */
MOZ_MUST_USE bool ReadableStreamCloseInternal(
    JSContext* cx, Handle<ReadableStream*> unwrappedStream) {
  // Step 1: Assert: stream.[[state]] is "readable".
  MOZ_ASSERT(unwrappedStream->readable());

  // Step 2: Set stream.[[state]] to "closed".
  unwrappedStream->setClosed();

  // Step 4: If reader is undefined, return (reordered).
  if (!unwrappedStream->hasReader()) {
    return true;
  }

  // Step 3: Let reader be stream.[[reader]].
  Rooted<ReadableStreamReader*> unwrappedReader(
      cx, UnwrapReaderFromStream(cx, unwrappedStream));
  if (!unwrappedReader) {
    return false;
  }

  // Step 5: If ! IsReadableStreamDefaultReader(reader) is true,
  if (unwrappedReader->is<ReadableStreamDefaultReader>()) {
    ForAuthorCodeBool forAuthorCode = unwrappedReader->forAuthorCode();

    // Step a: Repeat for each readRequest that is an element of
    //         reader.[[readRequests]],
    Rooted<ListObject*> unwrappedReadRequests(cx, unwrappedReader->requests());
    uint32_t len = unwrappedReadRequests->length();
    RootedObject readRequest(cx);
    RootedObject resultObj(cx);
    RootedValue resultVal(cx);
    for (uint32_t i = 0; i < len; i++) {
      // Step i: Resolve readRequest.[[promise]] with
      //         ! ReadableStreamCreateReadResult(undefined, true,
      //                                          readRequest.[[forAuthorCode]]).
      readRequest = &unwrappedReadRequests->getAs<JSObject>(i);
      if (!cx->compartment()->wrap(cx, &readRequest)) {
        return false;
      }

      resultObj = ReadableStreamCreateReadResult(cx, UndefinedHandleValue, true,
                                                 forAuthorCode);
      if (!resultObj) {
        return false;
      }
      resultVal = ObjectValue(*resultObj);
      if (!ResolvePromise(cx, readRequest, resultVal)) {
        return false;
      }
    }

    // Step b: Set reader.[[readRequests]] to an empty List.
    unwrappedReader->clearRequests();
  }

<<<<<<< HEAD
  // Step 6: Resolve reader.[[closedPromise]] with undefined.
  // Step 7: Return (implicit).
  RootedObject closedPromise(cx, unwrappedReader->closedPromise());
  if (!cx->compartment()->wrap(cx, &closedPromise)) {
    return false;
  }
  if (!ResolvePromise(cx, closedPromise, UndefinedHandleValue)) {
    return false;
  }

  if (unwrappedStream->mode() == JS::ReadableStreamMode::ExternalSource) {
    // Make sure we're in the stream's compartment.
    AutoRealm ar(cx, unwrappedStream);
    JS::ReadableStreamUnderlyingSource* source =
        unwrappedStream->controller()->externalSource();
    source->onClosed(cx, unwrappedStream);
  }

  return true;
||||||| merged common ancestors
    return true;
=======
  // Step 6: Resolve reader.[[closedPromise]] with undefined.
  RootedObject closedPromise(cx, unwrappedReader->closedPromise());
  if (!cx->compartment()->wrap(cx, &closedPromise)) {
    return false;
  }
  if (!ResolvePromise(cx, closedPromise, UndefinedHandleValue)) {
    return false;
  }

  if (unwrappedStream->mode() == JS::ReadableStreamMode::ExternalSource) {
    // Make sure we're in the stream's compartment.
    AutoRealm ar(cx, unwrappedStream);
    JS::ReadableStreamUnderlyingSource* source =
        unwrappedStream->controller()->externalSource();
    source->onClosed(cx, unwrappedStream);
  }

  return true;
>>>>>>> upstream-releases
}

/**
 * Streams spec, 3.4.5. ReadableStreamCreateReadResult ( value, done,
 * forAuthorCode )
 */
static MOZ_MUST_USE JSObject* ReadableStreamCreateReadResult(
    JSContext* cx, HandleValue value, bool done,
    ForAuthorCodeBool forAuthorCode) {
  // Step 1: Let prototype be null.
  // Step 2: If forAuthorCode is true, set prototype to %ObjectPrototype%.
  RootedObject templateObject(
      cx,
      forAuthorCode == ForAuthorCodeBool::Yes
          ? cx->realm()->getOrCreateIterResultTemplateObject(cx)
          : cx->realm()->getOrCreateIterResultWithoutPrototypeTemplateObject(
                cx));
  if (!templateObject) {
    return nullptr;
  }

  // Step 3: Assert: Type(done) is Boolean (implicit).

<<<<<<< HEAD
  // Step 4: Let obj be ObjectCreate(prototype).
  NativeObject* obj;
  JS_TRY_VAR_OR_RETURN_NULL(
      cx, obj,
      NativeObject::createWithTemplate(cx, gc::DefaultHeap, templateObject));
||||||| merged common ancestors
    // Step 3: Set stream.[[state]] to "errored".
    uint32_t state = StreamState(stream);
    SetStreamState(stream, (state & ReadableStream::Disturbed) | ReadableStream::Errored);
=======
  // Step 4: Let obj be ObjectCreate(prototype).
  NativeObject* obj;
  JS_TRY_VAR_OR_RETURN_NULL(
      cx, obj, NativeObject::createWithTemplate(cx, templateObject));
>>>>>>> upstream-releases

  // Step 5: Perform CreateDataProperty(obj, "value", value).
  obj->setSlot(Realm::IterResultObjectValueSlot, value);

  // Step 6: Perform CreateDataProperty(obj, "done", done).
  obj->setSlot(Realm::IterResultObjectDoneSlot,
               done ? TrueHandleValue : FalseHandleValue);

  // Step 7: Return obj.
  return obj;
}

/**
 * Streams spec, 3.4.6. ReadableStreamError ( stream, e )
 */
MOZ_MUST_USE bool ReadableStreamErrorInternal(
    JSContext* cx, Handle<ReadableStream*> unwrappedStream, HandleValue e) {
  // Step 1: Assert: ! IsReadableStream(stream) is true (implicit).

  // Step 2: Assert: stream.[[state]] is "readable".
  MOZ_ASSERT(unwrappedStream->readable());

  // Step 3: Set stream.[[state]] to "errored".
  unwrappedStream->setErrored();

  // Step 4: Set stream.[[storedError]] to e.
  {
    AutoRealm ar(cx, unwrappedStream);
    RootedValue wrappedError(cx, e);
    if (!cx->compartment()->wrap(cx, &wrappedError)) {
      return false;
    }
    unwrappedStream->setStoredError(wrappedError);
  }

  // Step 6: If reader is undefined, return (reordered).
  if (!unwrappedStream->hasReader()) {
    return true;
  }

<<<<<<< HEAD
  // Step 5: Let reader be stream.[[reader]].
  Rooted<ReadableStreamReader*> unwrappedReader(
      cx, UnwrapReaderFromStream(cx, unwrappedStream));
  if (!unwrappedReader) {
    return false;
  }

  // Steps 7,8: (Identical in our implementation.)
  // Step a: Repeat for each readRequest that is an element of
  //         reader.[[readRequests]],
  Rooted<ListObject*> unwrappedReadRequests(cx, unwrappedReader->requests());
  RootedObject readRequest(cx);
  RootedValue val(cx);
  uint32_t len = unwrappedReadRequests->length();
  for (uint32_t i = 0; i < len; i++) {
    // Step i: Reject readRequest.[[promise]] with e.
    val = unwrappedReadRequests->get(i);
    readRequest = &val.toObject();

    // Responses have to be created in the compartment from which the
    // error was triggered, which might not be the same as the one the
    // request was created in, so we have to wrap requests here.
    if (!cx->compartment()->wrap(cx, &readRequest)) {
      return false;
||||||| merged common ancestors
    // The closedPromise might have been created in another compartment.
    // RejectPromise can deal with wrapped Promise objects, but has to be
    // with all arguments in the current compartment, so we do need to wrap
    // the Promise.
    if (!cx->compartment()->wrap(cx, &closedPromise)) {
        return false;
    }
    if (!RejectPromise(cx, closedPromise, e)) {
        return false;
=======
  // Step 5: Let reader be stream.[[reader]].
  Rooted<ReadableStreamReader*> unwrappedReader(
      cx, UnwrapReaderFromStream(cx, unwrappedStream));
  if (!unwrappedReader) {
    return false;
  }

  // Steps 7-8: (Identical in our implementation.)
  // Step 7.a/8.b: Repeat for each read{Into}Request that is an element of
  //               reader.[[read{Into}Requests]],
  Rooted<ListObject*> unwrappedReadRequests(cx, unwrappedReader->requests());
  RootedObject readRequest(cx);
  RootedValue val(cx);
  uint32_t len = unwrappedReadRequests->length();
  for (uint32_t i = 0; i < len; i++) {
    // Step i: Reject read{Into}Request.[[promise]] with e.
    val = unwrappedReadRequests->get(i);
    readRequest = &val.toObject();

    // Responses have to be created in the compartment from which the
    // error was triggered, which might not be the same as the one the
    // request was created in, so we have to wrap requests here.
    if (!cx->compartment()->wrap(cx, &readRequest)) {
      return false;
>>>>>>> upstream-releases
    }

    if (!RejectPromise(cx, readRequest, e)) {
      return false;
    }
  }

<<<<<<< HEAD
  // Step b: Set reader.[[readRequests]] to a new empty List.
  if (!SetNewList(cx, unwrappedReader, ReadableStreamReader::Slot_Requests)) {
    return false;
  }

  // Step 9: Reject reader.[[closedPromise]] with e.
  //
  // The closedPromise might have been created in another compartment.
  // RejectPromise can deal with wrapped Promise objects, but has to be
  // with all arguments in the current compartment, so we do need to wrap
  // the Promise.
  RootedObject closedPromise(cx, unwrappedReader->closedPromise());
  if (!cx->compartment()->wrap(cx, &closedPromise)) {
    return false;
  }
  if (!RejectPromise(cx, closedPromise, e)) {
    return false;
  }

  if (unwrappedStream->mode() == JS::ReadableStreamMode::ExternalSource) {
    // Make sure we're in the stream's compartment.
    AutoRealm ar(cx, unwrappedStream);
    JS::ReadableStreamUnderlyingSource* source =
        unwrappedStream->controller()->externalSource();

    // Ensure that the embedding doesn't have to deal with
    // mixed-compartment arguments to the callback.
    RootedValue error(cx, e);
    if (!cx->compartment()->wrap(cx, &error)) {
      return false;
||||||| merged common ancestors
        cx->runtime()->readableStreamErroredCallback(cx, stream, source,
                                                     stream->embeddingFlags(), error);
=======
  // Step 7.b/8.c: Set reader.[[read{Into}Requests]] to a new empty List.
  if (!SetNewList(cx, unwrappedReader, ReadableStreamReader::Slot_Requests)) {
    return false;
  }

  // Step 9: Reject reader.[[closedPromise]] with e.
  //
  // The closedPromise might have been created in another compartment.
  // RejectPromise can deal with wrapped Promise objects, but all its arguments
  // must be same-compartment with cx, so we do need to wrap the Promise.
  RootedObject closedPromise(cx, unwrappedReader->closedPromise());
  if (!cx->compartment()->wrap(cx, &closedPromise)) {
    return false;
  }
  if (!RejectPromise(cx, closedPromise, e)) {
    return false;
  }

  if (unwrappedStream->mode() == JS::ReadableStreamMode::ExternalSource) {
    // Make sure we're in the stream's compartment.
    AutoRealm ar(cx, unwrappedStream);
    JS::ReadableStreamUnderlyingSource* source =
        unwrappedStream->controller()->externalSource();

    // Ensure that the embedding doesn't have to deal with
    // mixed-compartment arguments to the callback.
    RootedValue error(cx, e);
    if (!cx->compartment()->wrap(cx, &error)) {
      return false;
>>>>>>> upstream-releases
    }
    source->onErrored(cx, unwrappedStream, error);
  }

  return true;
}

/**
 * Streams spec, 3.4.7.
 *      ReadableStreamFulfillReadIntoRequest( stream, chunk, done )
 * Streams spec, 3.4.8.
 *      ReadableStreamFulfillReadRequest ( stream, chunk, done )
 * These two spec functions are identical in our implementation.
 */
<<<<<<< HEAD
static MOZ_MUST_USE bool ReadableStreamFulfillReadOrReadIntoRequest(
    JSContext* cx, Handle<ReadableStream*> unwrappedStream, HandleValue chunk,
    bool done) {
  cx->check(chunk);

  // Step 1: Let reader be stream.[[reader]].
  Rooted<ReadableStreamReader*> unwrappedReader(
      cx, UnwrapReaderFromStream(cx, unwrappedStream));
  if (!unwrappedReader) {
    return false;
  }

  // Step 2: Let readIntoRequest be the first element of
  //         reader.[[readIntoRequests]].
  // Step 3: Remove readIntoRequest from reader.[[readIntoRequests]], shifting
  //         all other elements downward (so that the second becomes the first,
  //         and so on).
  Rooted<ListObject*> unwrappedReadIntoRequests(cx,
                                                unwrappedReader->requests());
  RootedObject readIntoRequest(
      cx, &unwrappedReadIntoRequests->popFirstAs<JSObject>(cx));
  MOZ_ASSERT(readIntoRequest);
  if (!cx->compartment()->wrap(cx, &readIntoRequest)) {
    return false;
  }

  // Step 4: Resolve read{Into}Request.[[promise]] with
  //         ! ReadableStreamCreateReadResult(chunk, done,
  //         readIntoRequest.[[forAuthorCode]]).
  RootedObject iterResult(
      cx, ReadableStreamCreateReadResult(cx, chunk, done,
                                         unwrappedReader->forAuthorCode()));
  if (!iterResult) {
    return false;
  }
  RootedValue val(cx, ObjectValue(*iterResult));
  return ResolvePromise(cx, readIntoRequest, val);
||||||| merged common ancestors
static MOZ_MUST_USE bool
ReadableStreamFulfillReadOrReadIntoRequest(JSContext* cx, Handle<ReadableStream*> stream,
                                           HandleValue chunk, bool done)
{
    // Step 1: Let reader be stream.[[reader]].
    Rooted<ReadableStreamReader*> reader(cx, ReaderFromStream(cx, stream));
    if (!reader) {
        return false;
    }

    // Step 2: Let readIntoRequest be the first element of
    //         reader.[[readIntoRequests]].
    // Step 3: Remove readIntoRequest from reader.[[readIntoRequests]], shifting
    //         all other elements downward (so that the second becomes the first,
    //         and so on).
    RootedValue val(cx, reader->getFixedSlot(ReaderSlot_Requests));
    RootedNativeObject readIntoRequests(cx, &val.toObject().as<NativeObject>());
    RootedObject readIntoRequest(cx, ShiftFromList<JSObject>(cx, readIntoRequests));
    MOZ_ASSERT(readIntoRequest);
    if (!cx->compartment()->wrap(cx, &readIntoRequest)) {
        return false;
    }

    // Step 4: Resolve readIntoRequest.[[promise]] with
    //         ! CreateIterResultObject(chunk, done).
    RootedValue wrappedChunk(cx, chunk);
    if (!cx->compartment()->wrap(cx, &wrappedChunk)) {
        return false;
    }
    RootedObject iterResult(cx, CreateIterResultObject(cx, wrappedChunk, done));
    if (!iterResult) {
        return false;
    }
    val = ObjectValue(*iterResult);
    return ResolvePromise(cx, readIntoRequest, val);
=======
static MOZ_MUST_USE bool ReadableStreamFulfillReadOrReadIntoRequest(
    JSContext* cx, Handle<ReadableStream*> unwrappedStream, HandleValue chunk,
    bool done) {
  cx->check(chunk);

  // Step 1: Let reader be stream.[[reader]].
  Rooted<ReadableStreamReader*> unwrappedReader(
      cx, UnwrapReaderFromStream(cx, unwrappedStream));
  if (!unwrappedReader) {
    return false;
  }

  // Step 2: Let read{Into}Request be the first element of
  //         reader.[[read{Into}Requests]].
  // Step 3: Remove read{Into}Request from reader.[[read{Into}Requests]],
  //         shifting all other elements downward (so that the second becomes
  //         the first, and so on).
  Rooted<ListObject*> unwrappedReadIntoRequests(cx,
                                                unwrappedReader->requests());
  RootedObject readIntoRequest(
      cx, &unwrappedReadIntoRequests->popFirstAs<JSObject>(cx));
  MOZ_ASSERT(readIntoRequest);
  if (!cx->compartment()->wrap(cx, &readIntoRequest)) {
    return false;
  }

  // Step 4: Resolve read{Into}Request.[[promise]] with
  //         ! ReadableStreamCreateReadResult(chunk, done,
  //         readIntoRequest.[[forAuthorCode]]).
  RootedObject iterResult(
      cx, ReadableStreamCreateReadResult(cx, chunk, done,
                                         unwrappedReader->forAuthorCode()));
  if (!iterResult) {
    return false;
  }
  RootedValue val(cx, ObjectValue(*iterResult));
  return ResolvePromise(cx, readIntoRequest, val);
>>>>>>> upstream-releases
}

<<<<<<< HEAD
/**
 * Streams spec, 3.4.9. ReadableStreamGetNumReadIntoRequests ( stream )
 * Streams spec, 3.4.10. ReadableStreamGetNumReadRequests ( stream )
 * (Identical implementation.)
 */
static uint32_t ReadableStreamGetNumReadRequests(ReadableStream* stream) {
  // Step 1: Return the number of elements in
  //         stream.[[reader]].[[readRequests]].
  if (!stream->hasReader()) {
    return 0;
  }
||||||| merged common ancestors
// Streams spec, 3.4.8. ReadableStreamGetNumReadIntoRequests ( stream )
// Streams spec, 3.4.9. ReadableStreamGetNumReadRequests ( stream )
// (Identical implementation.)
static uint32_t
ReadableStreamGetNumReadRequests(ReadableStream* stream)
{
    // Step 1: Return the number of elements in
    //         stream.[[reader]].[[readRequests]].
    if (!HasReader(stream)) {
        return 0;
    }
=======
/**
 * Streams spec, 3.4.9. ReadableStreamGetNumReadIntoRequests ( stream )
 * Streams spec, 3.4.10. ReadableStreamGetNumReadRequests ( stream )
 * (Identical implementation.)
 */
static uint32_t ReadableStreamGetNumReadRequests(ReadableStream* stream) {
  // Step 1: Return the number of elements in
  //         stream.[[reader]].[[read{Into}Requests]].
  if (!stream->hasReader()) {
    return 0;
  }
>>>>>>> upstream-releases

  JS::AutoSuppressGCAnalysis nogc;
  ReadableStreamReader* reader = UnwrapReaderFromStreamNoThrow(stream);

  // Reader is a dead wrapper, treat it as non-existent.
  if (!reader) {
    return 0;
  }

  return reader->requests()->length();
}

/**
 * Streams spec 3.4.12. ReadableStreamHasDefaultReader ( stream )
 */
static MOZ_MUST_USE bool ReadableStreamHasDefaultReader(
    JSContext* cx, Handle<ReadableStream*> unwrappedStream, bool* result) {
  // Step 1: Let reader be stream.[[reader]].
  // Step 2: If reader is undefined, return false.
  if (!unwrappedStream->hasReader()) {
    *result = false;
    return true;
<<<<<<< HEAD
  }

  // Step 3: If ! ReadableStreamDefaultReader(reader) is false, return false.
  // Step 4: Return true.
  Rooted<ReadableStreamReader*> unwrappedReader(
      cx, UnwrapReaderFromStream(cx, unwrappedStream));
  if (!unwrappedReader) {
    return false;
  }
||||||| merged common ancestors
}
#endif // DEBUG

static MOZ_MUST_USE bool
ReadableStreamGetReaderMode(JSContext* cx, ReadableStream* stream, ReaderMode* mode)
{
    if (stream->getFixedSlot(StreamSlot_Reader).isUndefined()) {
        *mode = ReaderMode::None;
        return true;
    }

    JSObject* readerObj = ReaderFromStream(cx, stream);
    if (!readerObj) {
        return false;
    }

    *mode = ReaderMode::Default;
=======
  }
  Rooted<ReadableStreamReader*> unwrappedReader(
      cx, UnwrapReaderFromStream(cx, unwrappedStream));
  if (!unwrappedReader) {
    return false;
  }
>>>>>>> upstream-releases

<<<<<<< HEAD
  *result = unwrappedReader->is<ReadableStreamDefaultReader>();
  return true;
||||||| merged common ancestors
    return true;
=======
  // Step 3: If ! ReadableStreamDefaultReader(reader) is false, return false.
  // Step 4: Return true.
  *result = unwrappedReader->is<ReadableStreamDefaultReader>();
  return true;
>>>>>>> upstream-releases
}

/*** 3.5. Class ReadableStreamDefaultReader *********************************/

static MOZ_MUST_USE bool ReadableStreamReaderGenericInitialize(
    JSContext* cx, Handle<ReadableStreamReader*> reader,
    Handle<ReadableStream*> unwrappedStream, ForAuthorCodeBool forAuthorCode);

/**
 * Stream spec, 3.5.3. new ReadableStreamDefaultReader ( stream )
 * Steps 2-4.
 */
static MOZ_MUST_USE ReadableStreamDefaultReader*
<<<<<<< HEAD
CreateReadableStreamDefaultReader(JSContext* cx,
                                  Handle<ReadableStream*> unwrappedStream,
                                  ForAuthorCodeBool forAuthorCode,
                                  HandleObject proto /* = nullptr */) {
  Rooted<ReadableStreamDefaultReader*> reader(
      cx, NewObjectWithClassProto<ReadableStreamDefaultReader>(cx, proto));
  if (!reader) {
    return nullptr;
  }

  // Step 2: If ! IsReadableStreamLocked(stream) is true, throw a TypeError
  //         exception.
  if (unwrappedStream->locked()) {
    JS_ReportErrorNumberASCII(cx, GetErrorMessage, nullptr,
                              JSMSG_READABLESTREAM_LOCKED);
    return nullptr;
  }
||||||| merged common ancestors
CreateReadableStreamDefaultReader(JSContext* cx, Handle<ReadableStream*> stream)
{
    Rooted<ReadableStreamDefaultReader*> reader(cx);
    reader = NewBuiltinClassInstance<ReadableStreamDefaultReader>(cx);
    if (!reader) {
        return nullptr;
    }

    // Step 2: If ! IsReadableStreamLocked(stream) is true, throw a TypeError
    //         exception.
    if (stream->locked()) {
        JS_ReportErrorNumberASCII(cx, GetErrorMessage, nullptr, JSMSG_READABLESTREAM_LOCKED);
        return nullptr;
    }
=======
CreateReadableStreamDefaultReader(JSContext* cx,
                                  Handle<ReadableStream*> unwrappedStream,
                                  ForAuthorCodeBool forAuthorCode,
                                  HandleObject proto /* = nullptr */) {
  Rooted<ReadableStreamDefaultReader*> reader(
      cx, NewObjectWithClassProto<ReadableStreamDefaultReader>(cx, proto));
  if (!reader) {
    return nullptr;
  }
>>>>>>> upstream-releases

<<<<<<< HEAD
  // Step 3: Perform ! ReadableStreamReaderGenericInitialize(this, stream).
  if (!ReadableStreamReaderGenericInitialize(cx, reader, unwrappedStream,
                                             forAuthorCode)) {
    return nullptr;
  }
||||||| merged common ancestors
    // Step 3: Perform ! ReadableStreamReaderGenericInitialize(this, stream).
    if (!ReadableStreamReaderGenericInitialize(cx, reader, stream)) {
        return nullptr;
    }
=======
  // Step 2: If ! IsReadableStreamLocked(stream) is true, throw a TypeError
  //         exception.
  if (unwrappedStream->locked()) {
    JS_ReportErrorNumberASCII(cx, GetErrorMessage, nullptr,
                              JSMSG_READABLESTREAM_LOCKED);
    return nullptr;
  }
>>>>>>> upstream-releases

<<<<<<< HEAD
  // Step 4: Set this.[[readRequests]] to a new empty List.
  if (!SetNewList(cx, reader, ReadableStreamReader::Slot_Requests)) {
    return nullptr;
  }
||||||| merged common ancestors
    // Step 4: Set this.[[readRequests]] to a new empty List.
    if (!SetNewList(cx, reader, ReaderSlot_Requests)) {
        return nullptr;
    }
=======
  // Step 3: Perform ! ReadableStreamReaderGenericInitialize(this, stream).
  // Step 4: Set this.[[readRequests]] to a new empty List.
  if (!ReadableStreamReaderGenericInitialize(cx, reader, unwrappedStream,
                                             forAuthorCode)) {
    return nullptr;
  }
>>>>>>> upstream-releases

  return reader;
}

/**
 * Stream spec, 3.5.3. new ReadableStreamDefaultReader ( stream )
 */
bool ReadableStreamDefaultReader::constructor(JSContext* cx, unsigned argc,
                                              Value* vp) {
  CallArgs args = CallArgsFromVp(argc, vp);

  if (!ThrowIfNotConstructing(cx, args, "ReadableStreamDefaultReader")) {
    return false;
  }

<<<<<<< HEAD
  // Implicit in the spec: Find the prototype object to use.
  RootedObject proto(cx);
  if (!GetPrototypeFromBuiltinConstructor(cx, args, &proto)) {
    return false;
  }

  // Step 1: If ! IsReadableStream(stream) is false, throw a TypeError
  // exception.
  Rooted<ReadableStream*> unwrappedStream(
      cx, UnwrapAndTypeCheckArgument<ReadableStream>(
              cx, args, "ReadableStreamDefaultReader constructor", 0));
  if (!unwrappedStream) {
    return false;
  }
||||||| merged common ancestors
    Rooted<ReadableStream*> stream(cx,
                                   &CheckedUnwrap(&args.get(0).toObject())->as<ReadableStream>());
=======
  // Implicit in the spec: Find the prototype object to use.
  RootedObject proto(cx);
  if (!GetPrototypeFromBuiltinConstructor(cx, args, JSProto_Null, &proto)) {
    return false;
  }

  // Step 1: If ! IsReadableStream(stream) is false, throw a TypeError
  //         exception.
  Rooted<ReadableStream*> unwrappedStream(
      cx, UnwrapAndTypeCheckArgument<ReadableStream>(
              cx, args, "ReadableStreamDefaultReader constructor", 0));
  if (!unwrappedStream) {
    return false;
  }
>>>>>>> upstream-releases

  RootedObject reader(
      cx, CreateReadableStreamDefaultReader(cx, unwrappedStream,
                                            ForAuthorCodeBool::Yes, proto));
  if (!reader) {
    return false;
  }

  args.rval().setObject(*reader);
  return true;
}

/**
 * Streams spec, 3.5.4.1 get closed
 */
static MOZ_MUST_USE bool ReadableStreamDefaultReader_closed(JSContext* cx,
                                                            unsigned argc,
                                                            Value* vp) {
  CallArgs args = CallArgsFromVp(argc, vp);

  // Step 1: If ! IsReadableStreamDefaultReader(this) is false, return a promise
  //         rejected with a TypeError exception.
  Rooted<ReadableStreamDefaultReader*> unwrappedReader(
      cx, UnwrapAndTypeCheckThis<ReadableStreamDefaultReader>(cx, args,
                                                              "get closed"));
  if (!unwrappedReader) {
    return ReturnPromiseRejectedWithPendingError(cx, args);
  }

  // Step 2: Return this.[[closedPromise]].
  RootedObject closedPromise(cx, unwrappedReader->closedPromise());
  if (!cx->compartment()->wrap(cx, &closedPromise)) {
    return false;
  }

  args.rval().setObject(*closedPromise);
  return true;
}

<<<<<<< HEAD
static MOZ_MUST_USE JSObject* ReadableStreamReaderGenericCancel(
    JSContext* cx, Handle<ReadableStreamReader*> unwrappedReader,
    HandleValue reason);

/**
 * Streams spec, 3.5.4.2. cancel ( reason )
 */
static MOZ_MUST_USE bool ReadableStreamDefaultReader_cancel(JSContext* cx,
                                                            unsigned argc,
                                                            Value* vp) {
  CallArgs args = CallArgsFromVp(argc, vp);

  // Step 1: If ! IsReadableStreamDefaultReader(this) is false, return a promise
  //         rejected with a TypeError exception.
  Rooted<ReadableStreamDefaultReader*> unwrappedReader(
      cx,
      UnwrapAndTypeCheckThis<ReadableStreamDefaultReader>(cx, args, "cancel"));
  if (!unwrappedReader) {
    return ReturnPromiseRejectedWithPendingError(cx, args);
  }

  // Step 2: If this.[[ownerReadableStream]] is undefined, return a promise
  //         rejected with a TypeError exception.
  if (!unwrappedReader->hasStream()) {
    JS_ReportErrorNumberASCII(cx, GetErrorMessage, nullptr,
                              JSMSG_READABLESTREAMREADER_NOT_OWNED, "cancel");
    return ReturnPromiseRejectedWithPendingError(cx, args);
  }

  // Step 3: Return ! ReadableStreamReaderGenericCancel(this, reason).
  JSObject* cancelPromise =
      ReadableStreamReaderGenericCancel(cx, unwrappedReader, args.get(0));
  if (!cancelPromise) {
    return false;
  }
  args.rval().setObject(*cancelPromise);
  return true;
||||||| merged common ancestors
static MOZ_MUST_USE JSObject*
ReadableStreamReaderGenericCancel(JSContext* cx, Handle<ReadableStreamReader*> reader,
                                  HandleValue reason);

// Streams spec, 3.5.4.2. cancel ( reason )
static MOZ_MUST_USE bool
ReadableStreamDefaultReader_cancel(JSContext* cx, unsigned argc, Value* vp)
{
    CallArgs args = CallArgsFromVp(argc, vp);

    // Step 1: If ! IsReadableStreamDefaultReader(this) is false, return a promise
    //         rejected with a TypeError exception.
    Rooted<ReadableStreamDefaultReader*> reader(cx);
    reader = ToUnwrapped<ReadableStreamDefaultReader>(cx, args.thisv(),
                                                      "ReadableStreamDefaultReader", "cancel");
    if (!reader) {
        return ReturnPromiseRejectedWithPendingError(cx, args);
    }

    // Step 2: If this.[[ownerReadableStream]] is undefined, return a promise
    //         rejected with a TypeError exception.
    if (!ReaderHasStream(reader)) {
        JS_ReportErrorNumberASCII(cx, GetErrorMessage, nullptr,
                                  JSMSG_READABLESTREAMREADER_NOT_OWNED, "cancel");
        return ReturnPromiseRejectedWithPendingError(cx, args);
    }

    // Step 3: Return ! ReadableStreamReaderGenericCancel(this, reason).
    JSObject* cancelPromise = ReadableStreamReaderGenericCancel(cx, reader, args.get(0));
    if (!cancelPromise) {
        return false;
    }
    args.rval().setObject(*cancelPromise);
    return true;
=======
static MOZ_MUST_USE JSObject* ReadableStreamReaderGenericCancel(
    JSContext* cx, Handle<ReadableStreamReader*> unwrappedReader,
    HandleValue reason);

/**
 * Streams spec, 3.5.4.2. cancel ( reason )
 */
static MOZ_MUST_USE bool ReadableStreamDefaultReader_cancel(JSContext* cx,
                                                            unsigned argc,
                                                            Value* vp) {
  CallArgs args = CallArgsFromVp(argc, vp);

  // Step 1: If ! IsReadableStreamDefaultReader(this) is false, return a promise
  //         rejected with a TypeError exception.
  Rooted<ReadableStreamDefaultReader*> unwrappedReader(
      cx,
      UnwrapAndTypeCheckThis<ReadableStreamDefaultReader>(cx, args, "cancel"));
  if (!unwrappedReader) {
    return ReturnPromiseRejectedWithPendingError(cx, args);
  }

  // Step 2: If this.[[ownerReadableStream]] is undefined, return a promise
  //         rejected with a TypeError exception.
  if (!unwrappedReader->hasStream()) {
    JS_ReportErrorNumberASCII(cx, GetErrorMessage, nullptr,
                              JSMSG_READABLESTREAMREADER_NOT_OWNED, "cancel");
    return ReturnPromiseRejectedWithPendingError(cx, args);
  }

  // Step 3: Return ! ReadableStreamReaderGenericCancel(this, reason).
  JSObject* cancelPromise =
      ReadableStreamReaderGenericCancel(cx, unwrappedReader, args.get(0));
  if (!cancelPromise) {
    return false;
  }
  args.rval().setObject(*cancelPromise);
  return true;
>>>>>>> upstream-releases
}

/**
 * Streams spec, 3.5.4.3 read ( )
 */
static MOZ_MUST_USE bool ReadableStreamDefaultReader_read(JSContext* cx,
                                                          unsigned argc,
                                                          Value* vp) {
  CallArgs args = CallArgsFromVp(argc, vp);

  // Step 1: If ! IsReadableStreamDefaultReader(this) is false, return a promise
  //         rejected with a TypeError exception.
  Rooted<ReadableStreamDefaultReader*> unwrappedReader(
      cx,
      UnwrapAndTypeCheckThis<ReadableStreamDefaultReader>(cx, args, "read"));
  if (!unwrappedReader) {
    return ReturnPromiseRejectedWithPendingError(cx, args);
  }

  // Step 2: If this.[[ownerReadableStream]] is undefined, return a promise
  //         rejected with a TypeError exception.
  if (!unwrappedReader->hasStream()) {
    JS_ReportErrorNumberASCII(cx, GetErrorMessage, nullptr,
                              JSMSG_READABLESTREAMREADER_NOT_OWNED, "read");
    return ReturnPromiseRejectedWithPendingError(cx, args);
  }

  // Step 3: Return ! ReadableStreamDefaultReaderRead(this, true).
  JSObject* readPromise =
      ::ReadableStreamDefaultReaderRead(cx, unwrappedReader);
  if (!readPromise) {
    return false;
  }
  args.rval().setObject(*readPromise);
  return true;
}

static MOZ_MUST_USE bool ReadableStreamReaderGenericRelease(
    JSContext* cx, Handle<ReadableStreamReader*> reader);

/**
 * Streams spec, 3.5.4.4. releaseLock ( )
 */
static bool ReadableStreamDefaultReader_releaseLock(JSContext* cx,
                                                    unsigned argc, Value* vp) {
  // Step 1: If ! IsReadableStreamDefaultReader(this) is false,
  //         throw a TypeError exception.
  CallArgs args = CallArgsFromVp(argc, vp);
  Rooted<ReadableStreamDefaultReader*> reader(
      cx, UnwrapAndTypeCheckThis<ReadableStreamDefaultReader>(cx, args,
                                                              "releaseLock"));
  if (!reader) {
    return false;
  }

  // Step 2: If this.[[ownerReadableStream]] is undefined, return.
  if (!reader->hasStream()) {
    args.rval().setUndefined();
    return true;
  }

  // Step 3: If this.[[readRequests]] is not empty, throw a TypeError exception.
  Value val = reader->getFixedSlot(ReadableStreamReader::Slot_Requests);
  if (!val.isUndefined()) {
    ListObject* readRequests = &val.toObject().as<ListObject>();
    if (readRequests->length() != 0) {
      JS_ReportErrorNumberASCII(cx, GetErrorMessage, nullptr,
                                JSMSG_READABLESTREAMREADER_NOT_EMPTY,
                                "releaseLock");
      return false;
    }
  }

  // Step 4: Perform ! ReadableStreamReaderGenericRelease(this).
  if (!ReadableStreamReaderGenericRelease(cx, reader)) {
    return false;
  }

  args.rval().setUndefined();
  return true;
}

static const JSFunctionSpec ReadableStreamDefaultReader_methods[] = {
    JS_FN("cancel", ReadableStreamDefaultReader_cancel, 1, 0),
    JS_FN("read", ReadableStreamDefaultReader_read, 0, 0),
    JS_FN("releaseLock", ReadableStreamDefaultReader_releaseLock, 0, 0),
    JS_FS_END};

static const JSPropertySpec ReadableStreamDefaultReader_properties[] = {
    JS_PSG("closed", ReadableStreamDefaultReader_closed, 0), JS_PS_END};

const Class ReadableStreamReader::class_ = {"ReadableStreamReader"};

CLASS_SPEC(ReadableStreamDefaultReader, 1, SlotCount,
           ClassSpec::DontDefineConstructor, 0, JS_NULL_CLASS_OPS);

/*** 3.7. Readable stream reader abstract operations ************************/

// Streams spec, 3.7.1. IsReadableStreamDefaultReader ( x )
// Implemented via is<ReadableStreamDefaultReader>()

// Streams spec, 3.7.2. IsReadableStreamBYOBReader ( x )
// Implemented via is<ReadableStreamBYOBReader>()

/**
 * Streams spec, 3.7.3. ReadableStreamReaderGenericCancel ( reader, reason )
<<<<<<< HEAD
||||||| merged common ancestors
 *
 * Note: can operate on unwrapped ReadableStream reader instances from
 * another compartment.
 */
static MOZ_MUST_USE JSObject*
ReadableStreamReaderGenericCancel(JSContext* cx, Handle<ReadableStreamReader*> reader,
                                  HandleValue reason)
{
    // Step 1: Let stream be reader.[[ownerReadableStream]].
    // Step 2: Assert: stream is not undefined (implicit).
    Rooted<ReadableStream*> stream(cx, StreamFromReader(cx, reader));
    if (!stream) {
        return nullptr;
    }

    // Step 3: Return ! ReadableStreamCancel(stream, reason).
    return ReadableStream::cancel(cx, stream, reason);
}

/**
 * Streams spec, 3.7.4. ReadableStreamReaderGenericInitialize ( reader, stream )
 *
 * Note: can operate on unwrapped ReadableStream reader instances from
 * another compartment.
=======
 */
static MOZ_MUST_USE JSObject* ReadableStreamReaderGenericCancel(
    JSContext* cx, Handle<ReadableStreamReader*> unwrappedReader,
    HandleValue reason) {
  // Step 1: Let stream be reader.[[ownerReadableStream]].
  // Step 2: Assert: stream is not undefined (implicit).
  Rooted<ReadableStream*> unwrappedStream(
      cx, UnwrapStreamFromReader(cx, unwrappedReader));
  if (!unwrappedStream) {
    return nullptr;
  }

  // Step 3: Return ! ReadableStreamCancel(stream, reason).
  return ::ReadableStreamCancel(cx, unwrappedStream, reason);
}

/**
 * Streams spec, 3.7.4.
 *      ReadableStreamReaderGenericInitialize ( reader, stream )
>>>>>>> upstream-releases
 */
<<<<<<< HEAD
static MOZ_MUST_USE JSObject* ReadableStreamReaderGenericCancel(
    JSContext* cx, Handle<ReadableStreamReader*> unwrappedReader,
    HandleValue reason) {
  // Step 1: Let stream be reader.[[ownerReadableStream]].
  // Step 2: Assert: stream is not undefined (implicit).
  Rooted<ReadableStream*> unwrappedStream(
      cx, UnwrapStreamFromReader(cx, unwrappedReader));
  if (!unwrappedStream) {
    return nullptr;
  }

  // Step 3: Return ! ReadableStreamCancel(stream, reason).
  return ::ReadableStreamCancel(cx, unwrappedStream, reason);
}

/**
 * Streams spec, 3.7.4.
 *      ReadableStreamReaderGenericInitialize ( reader, stream )
 */
static MOZ_MUST_USE bool ReadableStreamReaderGenericInitialize(
    JSContext* cx, Handle<ReadableStreamReader*> reader,
    Handle<ReadableStream*> unwrappedStream, ForAuthorCodeBool forAuthorCode) {
  cx->check(reader);

  // Step 1: Set reader.[[ownerReadableStream]] to stream.
  {
    RootedObject readerCompartmentStream(cx, unwrappedStream);
    if (!cx->compartment()->wrap(cx, &readerCompartmentStream)) {
      return false;
    }
    reader->setStream(readerCompartmentStream);
  }
||||||| merged common ancestors
static MOZ_MUST_USE bool
ReadableStreamReaderGenericInitialize(JSContext* cx, Handle<ReadableStreamReader*> reader,
                                      Handle<ReadableStream*> stream)
{
    // Step 1: Set reader.[[ownerReadableStream]] to stream.
    // Step 2: Set stream.[[reader]] to reader.
    if (!IsObjectInContextCompartment(stream, cx)) {
        RootedObject wrappedStream(cx, stream);
        if (!cx->compartment()->wrap(cx, &wrappedStream)) {
            return false;
        }
        reader->setFixedSlot(ReaderSlot_Stream, ObjectValue(*wrappedStream));
        AutoRealm ar(cx, stream);
        RootedObject wrappedReader(cx, reader);
        if (!cx->compartment()->wrap(cx, &wrappedReader)) {
            return false;
        }
        stream->setFixedSlot(StreamSlot_Reader, ObjectValue(*wrappedReader));
    } else {
        reader->setFixedSlot(ReaderSlot_Stream, ObjectValue(*stream));
        stream->setFixedSlot(StreamSlot_Reader, ObjectValue(*reader));
    }

    // Step 3: If stream.[[state]] is "readable",
    RootedObject promise(cx);
    if (stream->readable()) {
        // Step a: Set reader.[[closedPromise]] to a new promise.
        promise = PromiseObject::createSkippingExecutor(cx);
    } else if (stream->closed()) {
        // Step 4: Otherwise
        // Step a: If stream.[[state]] is "closed",
        // Step i: Set reader.[[closedPromise]] to a new promise resolved with
        //         undefined.
        promise = PromiseObject::unforgeableResolve(cx, UndefinedHandleValue);
    } else {
        // Step b: Otherwise,
        // Step i: Assert: stream.[[state]] is "errored".
        MOZ_ASSERT(stream->errored());

        // Step ii: Set reader.[[closedPromise]] to a new promise rejected with
        //          stream.[[storedError]].
        RootedValue storedError(cx, stream->getFixedSlot(StreamSlot_StoredError));
        if (!cx->compartment()->wrap(cx, &storedError)) {
            return false;
        }
        promise = PromiseObject::unforgeableReject(cx, storedError);
    }

    if (!promise) {
        return false;
    }

    reader->setFixedSlot(ReaderSlot_ClosedPromise, ObjectValue(*promise));
    return true;
}

/**
 * Streams spec, 3.7.5. ReadableStreamReaderGenericRelease ( reader )
 *
 * Note: can operate on unwrapped ReadableStream reader instances from
 * another compartment.
 */
static MOZ_MUST_USE bool
ReadableStreamReaderGenericRelease(JSContext* cx, Handle<ReadableStreamReader*> reader)
{
    // Step 1: Assert: reader.[[ownerReadableStream]] is not undefined.
    Rooted<ReadableStream*> stream(cx, StreamFromReader(cx, reader));
    if (!stream) {
        return false;
    }

    // Step 2: Assert: reader.[[ownerReadableStream]].[[reader]] is reader.
    MOZ_ASSERT(ReaderFromStream(cx, stream) == reader);

    // Create an exception to reject promises with below. We don't have a
    // clean way to do this, unfortunately.
    JS_ReportErrorNumberASCII(cx, GetErrorMessage, nullptr, JSMSG_READABLESTREAMREADER_RELEASED);
    RootedValue exn(cx);
    // Not much we can do about uncatchable exceptions, just bail.
    if (!GetAndClearException(cx, &exn)) {
        return false;
    }

    // The reader might be from another compartment. In that case we need to
    // enter the reader's compartment before storing the above-created
    // exception. We might delay entering the compartment until we have also
    // created the closedPromise in step 4 below.
    mozilla::Maybe<AutoRealm> ar;

    // Step 3: If reader.[[ownerReadableStream]].[[state]] is "readable", reject
    //         reader.[[closedPromise]] with a TypeError exception.
    if (stream->readable()) {
        Value val = reader->getFixedSlot(ReaderSlot_ClosedPromise);
        Rooted<PromiseObject*> closedPromise(cx, &val.toObject().as<PromiseObject>());
        if (closedPromise->compartment() != cx->compartment()) {
            ar.emplace(cx, closedPromise);
            if (!cx->compartment()->wrap(cx, &exn)) {
                return false;
            }
        }
        if (!PromiseObject::reject(cx, closedPromise, exn)) {
            return false;
        }
    } else {
        // Step 4: Otherwise, set reader.[[closedPromise]] to a new promise rejected
        //         with a TypeError exception.
        RootedObject closedPromise(cx, PromiseObject::unforgeableReject(cx, exn));
        if (!closedPromise) {
            return false;
        }
        if (reader->compartment() != cx->compartment()) {
            ar.emplace(cx, reader);
            if (!cx->compartment()->wrap(cx, &closedPromise)) {
                return false;
            }
        }
        reader->setFixedSlot(ReaderSlot_ClosedPromise, ObjectValue(*closedPromise));
    }

    // Step 5: Set reader.[[ownerReadableStream]].[[reader]] to undefined.
    stream->setFixedSlot(StreamSlot_Reader, UndefinedValue());

    // Step 6: Set reader.[[ownerReadableStream]] to undefined.
    reader->setFixedSlot(ReaderSlot_Stream, UndefinedValue());

    return true;
}

static MOZ_MUST_USE JSObject*
ReadableStreamControllerPullSteps(JSContext* cx, Handle<ReadableStreamController*> controller);

/**
 * Streams spec, 3.7.7. ReadableStreamDefaultReaderRead ( reader )
 *
 * Note: can operate on unwrapped ReadableStreamDefaultReader instances from
 * another compartment.
 */
/* static */ MOZ_MUST_USE JSObject*
ReadableStreamDefaultReader::read(JSContext* cx, Handle<ReadableStreamDefaultReader*> reader)
{
    // Step 1: Let stream be reader.[[ownerReadableStream]].
    // Step 2: Assert: stream is not undefined.
    Rooted<ReadableStream*> stream(cx, StreamFromReader(cx, reader));
    if (!stream) {
        return nullptr;
    }

    // Step 3: Set stream.[[disturbed]] to true.
    SetStreamState(stream, StreamState(stream) | ReadableStream::Disturbed);

    // Step 4: If stream.[[state]] is "closed", return a new promise resolved with
    //         ! CreateIterResultObject(undefined, true).
    if (stream->closed()) {
        RootedObject iterResult(cx, CreateIterResultObject(cx, UndefinedHandleValue, true));
        if (!iterResult) {
            return nullptr;
        }
        RootedValue iterResultVal(cx, ObjectValue(*iterResult));
        return PromiseObject::unforgeableResolve(cx, iterResultVal);
    }

    // Step 5: If stream.[[state]] is "errored", return a new promise rejected with
    //         stream.[[storedError]].
    if (stream->errored()) {
        RootedValue storedError(cx, stream->getFixedSlot(StreamSlot_StoredError));
        if (!cx->compartment()->wrap(cx, &storedError)) {
            return nullptr;
        }
        return PromiseObject::unforgeableReject(cx, storedError);
    }

    // Step 6: Assert: stream.[[state]] is "readable".
    MOZ_ASSERT(stream->readable());

    // Step 7: Return ! stream.[[readableStreamController]].[[PullSteps]]().
    Rooted<ReadableStreamController*> controller(cx, ControllerFromStream(stream));
    return ReadableStreamControllerPullSteps(cx, controller);
}

// Streams spec, 3.8.3, step 11.a.
// and
// Streams spec, 3.10.3, step 16.a.
static bool
ControllerStartHandler(JSContext* cx, unsigned argc, Value* vp)
{
    CallArgs args = CallArgsFromVp(argc, vp);
    Rooted<ReadableStreamController*> controller(cx);
    controller = TargetFromHandler<ReadableStreamController>(args.callee());

    // Step i: Set controller.[[started]] to true.
    AddControllerFlags(controller, ControllerFlag_Started);

    // Step ii: Assert: controller.[[pulling]] is false.
    // Step iii: Assert: controller.[[pullAgain]] is false.
    MOZ_ASSERT(!(ControllerFlags(controller) &
                 (ControllerFlag_Pulling | ControllerFlag_PullAgain)));

    // Step iv: Perform ! ReadableStreamDefaultControllerCallPullIfNeeded(controller).
    // or
    // Step iv: Perform ! ReadableByteStreamControllerCallPullIfNeeded((controller).
    if (!ReadableStreamControllerCallPullIfNeeded(cx, controller)) {
        return false;
    }
    args.rval().setUndefined();
    return true;
}

static MOZ_MUST_USE bool
ReadableStreamControllerError(JSContext* cx, Handle<ReadableStreamController*> controller,
                              HandleValue e);

// Streams spec, 3.8.3, step 11.b.
// and
// Streams spec, 3.10.3, step 16.b.
static bool
ControllerStartFailedHandler(JSContext* cx, unsigned argc, Value* vp)
{
    CallArgs args = CallArgsFromVp(argc, vp);
    Rooted<ReadableStreamController*> controllerObj(cx);
    controllerObj = TargetFromHandler<ReadableStreamController>(args.callee());

    // 3.8.3, Step 11.b.i:
    // Perform ! ReadableStreamDefaultControllerErrorIfNeeded(controller, r).
    if (controllerObj->is<ReadableStreamDefaultController>()) {
        Rooted<ReadableStreamDefaultController*> controller(cx);
        controller = &controllerObj->as<ReadableStreamDefaultController>();
        return ReadableStreamDefaultControllerErrorIfNeeded(cx, controller, args.get(0));
    }
=======
static MOZ_MUST_USE bool ReadableStreamReaderGenericInitialize(
    JSContext* cx, Handle<ReadableStreamReader*> reader,
    Handle<ReadableStream*> unwrappedStream, ForAuthorCodeBool forAuthorCode) {
  cx->check(reader);

  // Step 1: Set reader.[[ownerReadableStream]] to stream.
  {
    RootedObject readerCompartmentStream(cx, unwrappedStream);
    if (!cx->compartment()->wrap(cx, &readerCompartmentStream)) {
      return false;
    }
    reader->setStream(readerCompartmentStream);
  }

  // Step 2 is moved to the end.

  // Step 3: If stream.[[state]] is "readable",
  RootedObject promise(cx);
  if (unwrappedStream->readable()) {
    // Step a: Set reader.[[closedPromise]] to a new promise.
    promise = PromiseObject::createSkippingExecutor(cx);
  } else if (unwrappedStream->closed()) {
    // Step 4: Otherwise, if stream.[[state]] is "closed",
    // Step a: Set reader.[[closedPromise]] to a new promise resolved with
    //         undefined.
    promise = PromiseObject::unforgeableResolve(cx, UndefinedHandleValue);
  } else {
    // Step 5: Otherwise,
    // Step a: Assert: stream.[[state]] is "errored".
    MOZ_ASSERT(unwrappedStream->errored());

    // Step b: Set reader.[[closedPromise]] to a promise rejected with
    //         stream.[[storedError]].
    RootedValue storedError(cx, unwrappedStream->storedError());
    if (!cx->compartment()->wrap(cx, &storedError)) {
      return false;
    }
    promise = PromiseObject::unforgeableReject(cx, storedError);
    if (!promise) {
      return false;
    }

    // Step c. Set reader.[[closedPromise]].[[PromiseIsHandled]] to true.
    promise->as<PromiseObject>().setHandled();
    cx->runtime()->removeUnhandledRejectedPromise(cx, promise);
  }

  if (!promise) {
    return false;
  }

  reader->setClosedPromise(promise);

  // Extra step not in the standard. See the comment on
  // `ReadableStreamReader::forAuthorCode()`.
  reader->setForAuthorCode(forAuthorCode);
>>>>>>> upstream-releases

<<<<<<< HEAD
  // Step 2: Set stream.[[reader]] to reader.
  {
    AutoRealm ar(cx, unwrappedStream);
    RootedObject streamCompartmentReader(cx, reader);
    if (!cx->compartment()->wrap(cx, &streamCompartmentReader)) {
      return false;
||||||| merged common ancestors
    // 3.10.3, Step 16.b.i: If stream.[[state]] is "readable", perform
    //                      ! ReadableByteStreamControllerError(controller, r).
    if (StreamFromController(controllerObj)->readable()) {
        return ReadableStreamControllerError(cx, controllerObj, args.get(0));
=======
  // Step 4 of caller 3.5.3. new ReadableStreamDefaultReader(stream):
  // Step 5 of caller 3.6.3. new ReadableStreamBYOBReader(stream):
  //     Set this.[[read{Into}Requests]] to a new empty List.
  if (!SetNewList(cx, reader, ReadableStreamReader::Slot_Requests)) {
    return false;
  }

  // Step 2: Set stream.[[reader]] to reader.
  // Doing this last prevents a partially-initialized reader from being
  // attached to the stream (and possibly left there on OOM).
  {
    AutoRealm ar(cx, unwrappedStream);
    RootedObject streamCompartmentReader(cx, reader);
    if (!cx->compartment()->wrap(cx, &streamCompartmentReader)) {
      return false;
>>>>>>> upstream-releases
    }
<<<<<<< HEAD
    unwrappedStream->setReader(streamCompartmentReader);
  }

  // Step 3: If stream.[[state]] is "readable",
  RootedObject promise(cx);
  if (unwrappedStream->readable()) {
    // Step a: Set reader.[[closedPromise]] to a new promise.
    promise = PromiseObject::createSkippingExecutor(cx);
  } else if (unwrappedStream->closed()) {
    // Step 4: Otherwise
    // Step a: If stream.[[state]] is "closed",
    // Step i: Set reader.[[closedPromise]] to a new promise resolved with
    //         undefined.
    promise = PromiseObject::unforgeableResolve(cx, UndefinedHandleValue);
  } else {
    // Step b: Otherwise,
    // Step i: Assert: stream.[[state]] is "errored".
    MOZ_ASSERT(unwrappedStream->errored());

    // Step ii: Set reader.[[closedPromise]] to a new promise rejected with
    //          stream.[[storedError]].
    RootedValue storedError(cx, unwrappedStream->storedError());
    if (!cx->compartment()->wrap(cx, &storedError)) {
      return false;
    }
    promise = PromiseObject::unforgeableReject(cx, storedError);
  }

  if (!promise) {
    return false;
  }
||||||| merged common ancestors

    args.rval().setUndefined();
    return true;
}

static MOZ_MUST_USE bool
ValidateAndNormalizeHighWaterMark(JSContext* cx,
                                  HandleValue highWaterMarkVal,
                                  double* highWaterMark);

static MOZ_MUST_USE bool
ValidateAndNormalizeQueuingStrategy(JSContext* cx,
                                    HandleValue size,
                                    HandleValue highWaterMarkVal,
                                    double* highWaterMark);

/**
 * Streams spec, 3.8.3 new ReadableStreamDefaultController ( stream, underlyingSource,
 *                                                           size, highWaterMark )
 * Steps 3 - 11.
 *
 * Note: can NOT operate on unwrapped ReadableStream instances from
 * another compartment: ReadableStream controllers must be created in the same
 * compartment as the stream.
 */
static MOZ_MUST_USE ReadableStreamDefaultController*
CreateReadableStreamDefaultController(JSContext* cx, Handle<ReadableStream*> stream,
                                      HandleValue underlyingSource, HandleValue size,
                                      HandleValue highWaterMarkVal)
{
    Rooted<ReadableStreamDefaultController*> controller(cx);
    controller = NewBuiltinClassInstance<ReadableStreamDefaultController>(cx);
    if (!controller) {
        return nullptr;
    }

    // Step 3: Set this.[[controlledReadableStream]] to stream.
    controller->setFixedSlot(ControllerSlot_Stream, ObjectValue(*stream));

    // Step 4: Set this.[[underlyingSource]] to underlyingSource.
    controller->setFixedSlot(ControllerSlot_UnderlyingSource, underlyingSource);

    // Step 5: Perform ! ResetQueue(this).
    if (!ResetQueue(cx, controller)) {
        return nullptr;
    }
=======
    unwrappedStream->setReader(streamCompartmentReader);
  }

  return true;
}

/**
 * Streams spec, 3.7.5. ReadableStreamReaderGenericRelease ( reader )
 */
static MOZ_MUST_USE bool ReadableStreamReaderGenericRelease(
    JSContext* cx, Handle<ReadableStreamReader*> unwrappedReader) {
  // Step 1: Assert: reader.[[ownerReadableStream]] is not undefined.
  Rooted<ReadableStream*> unwrappedStream(
      cx, UnwrapStreamFromReader(cx, unwrappedReader));
  if (!unwrappedStream) {
    return false;
  }
>>>>>>> upstream-releases

<<<<<<< HEAD
  reader->setClosedPromise(promise);
||||||| merged common ancestors
    // Step 6: Set this.[[started]], this.[[closeRequested]], this.[[pullAgain]],
    //         and this.[[pulling]] to false.
    controller->setFixedSlot(ControllerSlot_Flags, Int32Value(0));
=======
  // Step 2: Assert: reader.[[ownerReadableStream]].[[reader]] is reader.
#ifdef DEBUG
  // The assertion is weakened a bit to allow for nuked wrappers.
  ReadableStreamReader* unwrappedReader2 =
      UnwrapReaderFromStreamNoThrow(unwrappedStream);
  MOZ_ASSERT_IF(unwrappedReader2, unwrappedReader2 == unwrappedReader);
#endif
>>>>>>> upstream-releases

<<<<<<< HEAD
  // Extra step not in the standard. See the comment on
  // `ReadableStreamReader::forAuthorCode()`.
  reader->setForAuthorCode(forAuthorCode);
||||||| merged common ancestors
    // Step 7: Let normalizedStrategy be
    //         ? ValidateAndNormalizeQueuingStrategy(size, highWaterMark).
    double highWaterMark;
    if (!ValidateAndNormalizeQueuingStrategy(cx, size, highWaterMarkVal, &highWaterMark)) {
        return nullptr;
    }
=======
  // Create an exception to reject promises with below. We don't have a
  // clean way to do this, unfortunately.
  JS_ReportErrorNumberASCII(cx, GetErrorMessage, nullptr,
                            JSMSG_READABLESTREAMREADER_RELEASED);
  RootedValue exn(cx);
  if (!cx->isExceptionPending() || !GetAndClearException(cx, &exn)) {
    // Uncatchable error. Die immediately without resolving
    // reader.[[closedPromise]].
    return false;
  }

  // Step 3: If reader.[[ownerReadableStream]].[[state]] is "readable", reject
  //         reader.[[closedPromise]] with a TypeError exception.
  Rooted<PromiseObject*> unwrappedClosedPromise(cx);
  if (unwrappedStream->readable()) {
    unwrappedClosedPromise = UnwrapInternalSlot<PromiseObject>(
        cx, unwrappedReader, ReadableStreamReader::Slot_ClosedPromise);
    if (!unwrappedClosedPromise) {
      return false;
    }
>>>>>>> upstream-releases

<<<<<<< HEAD
  return true;
}

/**
 * Streams spec, 3.7.5. ReadableStreamReaderGenericRelease ( reader )
 */
static MOZ_MUST_USE bool ReadableStreamReaderGenericRelease(
    JSContext* cx, Handle<ReadableStreamReader*> unwrappedReader) {
  // Step 1: Assert: reader.[[ownerReadableStream]] is not undefined.
  Rooted<ReadableStream*> unwrappedStream(
      cx, UnwrapStreamFromReader(cx, unwrappedReader));
  if (!unwrappedStream) {
    return false;
  }

  // Step 2: Assert: reader.[[ownerReadableStream]].[[reader]] is reader.
#ifdef DEBUG
  // The assertion is weakened a bit to allow for nuked wrappers.
  ReadableStreamReader* unwrappedReader2 =
      UnwrapReaderFromStreamNoThrow(unwrappedStream);
  MOZ_ASSERT_IF(unwrappedReader2, unwrappedReader2 == unwrappedReader);
#endif

  // Create an exception to reject promises with below. We don't have a
  // clean way to do this, unfortunately.
  JS_ReportErrorNumberASCII(cx, GetErrorMessage, nullptr,
                            JSMSG_READABLESTREAMREADER_RELEASED);
  RootedValue exn(cx);
  if (!cx->isExceptionPending() || !GetAndClearException(cx, &exn)) {
    // Uncatchable error. Die immediately without resolving
    // reader.[[closedPromise]].
    return false;
  }

  // Step 3: If reader.[[ownerReadableStream]].[[state]] is "readable", reject
  //         reader.[[closedPromise]] with a TypeError exception.
  if (unwrappedStream->readable()) {
    Rooted<PromiseObject*> closedPromise(
        cx, UnwrapInternalSlot<PromiseObject>(
                cx, unwrappedReader, ReadableStreamReader::Slot_ClosedPromise));
    if (!closedPromise) {
      return false;
||||||| merged common ancestors
    // Step 8: Set this.[[strategySize]] to normalizedStrategy.[[size]] and
    //         this.[[strategyHWM]] to normalizedStrategy.[[highWaterMark]].
    controller->setFixedSlot(DefaultControllerSlot_StrategySize, size);
    controller->setFixedSlot(ControllerSlot_StrategyHWM, NumberValue(highWaterMark));

    // Step 9: Let controller be this (implicit).

    // Step 10: Let startResult be
    //          ? InvokeOrNoop(underlyingSource, "start", « this »).
    RootedValue startResult(cx);
    RootedValue controllerVal(cx, ObjectValue(*controller));
    if (!InvokeOrNoop(cx, underlyingSource, cx->names().start, controllerVal, &startResult)) {
        return nullptr;
    }

    // Step 11: Let startPromise be a promise resolved with startResult:
    RootedObject startPromise(cx, PromiseObject::unforgeableResolve(cx, startResult));
    if (!startPromise) {
        return nullptr;
=======
    AutoRealm ar(cx, unwrappedClosedPromise);
    if (!cx->compartment()->wrap(cx, &exn)) {
      return false;
    }
    if (!PromiseObject::reject(cx, unwrappedClosedPromise, exn)) {
      return false;
>>>>>>> upstream-releases
    }
<<<<<<< HEAD

    AutoRealm ar(cx, closedPromise);
    if (!cx->compartment()->wrap(cx, &exn)) {
      return false;
||||||| merged common ancestors

    RootedObject onStartFulfilled(cx, NewHandler(cx, ControllerStartHandler, controller));
    if (!onStartFulfilled) {
        return nullptr;
=======
  } else {
    // Step 4: Otherwise, set reader.[[closedPromise]] to a new promise
    //         rejected with a TypeError exception.
    RootedObject closedPromise(cx, PromiseObject::unforgeableReject(cx, exn));
    if (!closedPromise) {
      return false;
>>>>>>> upstream-releases
    }
<<<<<<< HEAD
    if (!PromiseObject::reject(cx, closedPromise, exn)) {
      return false;
    }
  } else {
    // Step 4: Otherwise, set reader.[[closedPromise]] to a new promise
    //         rejected with a TypeError exception.
    RootedObject closedPromise(cx, PromiseObject::unforgeableReject(cx, exn));
    if (!closedPromise) {
      return false;
||||||| merged common ancestors

    RootedObject onStartRejected(cx, NewHandler(cx, ControllerStartFailedHandler, controller));
    if (!onStartRejected) {
        return nullptr;
=======
    unwrappedClosedPromise = &closedPromise->as<PromiseObject>();

    AutoRealm ar(cx, unwrappedReader);
    if (!cx->compartment()->wrap(cx, &closedPromise)) {
      return false;
>>>>>>> upstream-releases
    }
    unwrappedReader->setClosedPromise(closedPromise);
  }

<<<<<<< HEAD
    AutoRealm ar(cx, unwrappedReader);
    if (!cx->compartment()->wrap(cx, &closedPromise)) {
      return false;
    }
    unwrappedReader->setClosedPromise(closedPromise);
  }
||||||| merged common ancestors
    if (!JS::AddPromiseReactions(cx, startPromise, onStartFulfilled, onStartRejected)) {
        return nullptr;
    }
=======
  // Step 5: Set reader.[[closedPromise]].[[PromiseIsHandled]] to true.
  unwrappedClosedPromise->setHandled();
  cx->runtime()->removeUnhandledRejectedPromise(cx, unwrappedClosedPromise);
>>>>>>> upstream-releases

<<<<<<< HEAD
  // Step 5: Set reader.[[ownerReadableStream]].[[reader]] to undefined.
  unwrappedStream->clearReader();
||||||| merged common ancestors
    return controller;
}
=======
  // Step 6: Set reader.[[ownerReadableStream]].[[reader]] to undefined.
  unwrappedStream->clearReader();
>>>>>>> upstream-releases

<<<<<<< HEAD
  // Step 6: Set reader.[[ownerReadableStream]] to undefined.
  unwrappedReader->clearStream();
||||||| merged common ancestors
// Streams spec, 3.8.3.
// new ReadableStreamDefaultController( stream, underlyingSource, size,
//                                      highWaterMark )
bool
ReadableStreamDefaultController::constructor(JSContext* cx, unsigned argc, Value* vp)
{
    CallArgs args = CallArgsFromVp(argc, vp);
=======
  // Step 7: Set reader.[[ownerReadableStream]] to undefined.
  unwrappedReader->clearStream();
>>>>>>> upstream-releases

  return true;
}

<<<<<<< HEAD
static MOZ_MUST_USE JSObject* ReadableStreamControllerPullSteps(
    JSContext* cx, Handle<ReadableStreamController*> unwrappedController);

/**
 * Streams spec, 3.7.7. ReadableStreamDefaultReaderRead ( reader [,
 * forAuthorCode ] )
 */
static MOZ_MUST_USE JSObject* ReadableStreamDefaultReaderRead(
    JSContext* cx, Handle<ReadableStreamDefaultReader*> unwrappedReader) {
  // Step 1: If forAuthorCode was not passed, set it to false (implicit).

  // Step 2: Let stream be reader.[[ownerReadableStream]].
  // Step 3: Assert: stream is not undefined.
  Rooted<ReadableStream*> unwrappedStream(
      cx, UnwrapStreamFromReader(cx, unwrappedReader));
  if (!unwrappedStream) {
    return nullptr;
  }
||||||| merged common ancestors
    // Step 1: If ! IsReadableStream(stream) is false, throw a TypeError exception.
    HandleValue streamVal = args.get(0);
    if (!Is<ReadableStream>(streamVal)) {
        ReportArgTypeError(cx, "ReadableStreamDefaultController", "ReadableStream",
                           args.get(0));
        return false;
    }
=======
static MOZ_MUST_USE JSObject* ReadableStreamControllerPullSteps(
    JSContext* cx, Handle<ReadableStreamController*> unwrappedController);

/**
 * Streams spec, 3.7.7.
 *      ReadableStreamDefaultReaderRead ( reader [, forAuthorCode ] )
 */
static MOZ_MUST_USE JSObject* ReadableStreamDefaultReaderRead(
    JSContext* cx, Handle<ReadableStreamDefaultReader*> unwrappedReader) {
  // Step 1: If forAuthorCode was not passed, set it to false (implicit).

  // Step 2: Let stream be reader.[[ownerReadableStream]].
  // Step 3: Assert: stream is not undefined.
  Rooted<ReadableStream*> unwrappedStream(
      cx, UnwrapStreamFromReader(cx, unwrappedReader));
  if (!unwrappedStream) {
    return nullptr;
  }
>>>>>>> upstream-releases

  // Step 4: Set stream.[[disturbed]] to true.
  unwrappedStream->setDisturbed();

<<<<<<< HEAD
  // Step 5: If stream.[[state]] is "closed", return a new promise resolved with
  //         ! ReadableStreamCreateReadResult(undefined, true, forAuthorCode).
  if (unwrappedStream->closed()) {
    RootedObject iterResult(
        cx, ReadableStreamCreateReadResult(cx, UndefinedHandleValue, true,
                                           unwrappedReader->forAuthorCode()));
    if (!iterResult) {
      return nullptr;
||||||| merged common ancestors
    // Step 2: If stream.[[readableStreamController]] is not undefined, throw a
    //         TypeError exception.
    if (HasController(stream)) {
        JS_ReportErrorNumberASCII(cx, GetErrorMessage, nullptr,
                                  JSMSG_READABLESTREAM_CONTROLLER_SET);
        return false;
=======
  // Step 5: If stream.[[state]] is "closed", return a promise resolved with
  //         ! ReadableStreamCreateReadResult(undefined, true, forAuthorCode).
  if (unwrappedStream->closed()) {
    RootedObject iterResult(
        cx, ReadableStreamCreateReadResult(cx, UndefinedHandleValue, true,
                                           unwrappedReader->forAuthorCode()));
    if (!iterResult) {
      return nullptr;
>>>>>>> upstream-releases
    }
    RootedValue iterResultVal(cx, ObjectValue(*iterResult));
    return PromiseObject::unforgeableResolve(cx, iterResultVal);
  }

<<<<<<< HEAD
  // Step 6: If stream.[[state]] is "errored", return a new promise rejected
  //         with stream.[[storedError]].
  if (unwrappedStream->errored()) {
    RootedValue storedError(cx, unwrappedStream->storedError());
    if (!cx->compartment()->wrap(cx, &storedError)) {
      return nullptr;
||||||| merged common ancestors
    // Steps 3-11.
    RootedObject controller(cx, CreateReadableStreamDefaultController(cx, stream, args.get(1),
                                                                      args.get(2), args.get(3)));
    if (!controller) {
        return false;
=======
  // Step 6: If stream.[[state]] is "errored", return a promise rejected
  //         with stream.[[storedError]].
  if (unwrappedStream->errored()) {
    RootedValue storedError(cx, unwrappedStream->storedError());
    if (!cx->compartment()->wrap(cx, &storedError)) {
      return nullptr;
>>>>>>> upstream-releases
    }
    return PromiseObject::unforgeableReject(cx, storedError);
  }

  // Step 7: Assert: stream.[[state]] is "readable".
  MOZ_ASSERT(unwrappedStream->readable());

  // Step 8: Return ! stream.[[readableStreamController]].[[PullSteps]]().
  Rooted<ReadableStreamController*> unwrappedController(
      cx, unwrappedStream->controller());
  return ReadableStreamControllerPullSteps(cx, unwrappedController);
}

/*** 3.8. Class ReadableStreamDefaultController *****************************/

inline static MOZ_MUST_USE bool ReadableStreamControllerCallPullIfNeeded(
    JSContext* cx, Handle<ReadableStreamController*> unwrappedController);

<<<<<<< HEAD
/**
 * Streams spec, 3.8.3, step 11.a.
 * and
 * Streams spec, 3.10.3, step 16.a.
 */
static bool ControllerStartHandler(JSContext* cx, unsigned argc, Value* vp) {
  CallArgs args = CallArgsFromVp(argc, vp);
  Rooted<ReadableStreamController*> controller(
      cx, TargetFromHandler<ReadableStreamController>(args));

  // Step i: Set controller.[[started]] to true.
  controller->setStarted();

  // Step ii: Assert: controller.[[pulling]] is false.
  MOZ_ASSERT(!controller->pulling());

  // Step iii: Assert: controller.[[pullAgain]] is false.
  MOZ_ASSERT(!controller->pullAgain());

  // Step iv: Perform
  //          ! ReadableStreamDefaultControllerCallPullIfNeeded(controller).
  // or
  // Step iv: Perform
  //          ! ReadableByteStreamControllerCallPullIfNeeded((controller).
  if (!ReadableStreamControllerCallPullIfNeeded(cx, controller)) {
    return false;
  }
  args.rval().setUndefined();
  return true;
}
||||||| merged common ancestors
    // 3.9.8. Step 4: If state is "closed", return 0.
    if (stream->closed()) {
        args.rval().setInt32(0);
        return true;
    }
=======
/**
 * Streams spec, 3.9.11. SetUpReadableStreamDefaultController, step 11
 * and
 * Streams spec, 3.12.26. SetUpReadableByteStreamController, step 16:
 *      Upon fulfillment of startPromise, [...]
 */
static bool ControllerStartHandler(JSContext* cx, unsigned argc, Value* vp) {
  CallArgs args = CallArgsFromVp(argc, vp);
  Rooted<ReadableStreamController*> controller(
      cx, TargetFromHandler<ReadableStreamController>(args));
>>>>>>> upstream-releases

<<<<<<< HEAD
/**
 * Streams spec, 3.9.11, step 12.a.
 * and
 * Streams spec, 3.12.26, step 17.a.
 */
static bool ControllerStartFailedHandler(JSContext* cx, unsigned argc,
                                         Value* vp) {
  CallArgs args = CallArgsFromVp(argc, vp);
  Rooted<ReadableStreamController*> controller(
      cx, TargetFromHandler<ReadableStreamController>(args));

  // 3.9.11, Step 12.a: Perform
  //      ! ReadableStreamDefaultControllerError(controller, r).
  // 3.12.26, Step 17.a: Perform
  //      ! ReadableByteStreamControllerError(controller, r).
  if (!ReadableStreamControllerError(cx, controller, args.get(0))) {
    return false;
  }

  args.rval().setUndefined();
  return true;
}
||||||| merged common ancestors
    // Step 2: Return ! ReadableStreamDefaultControllerGetDesiredSize(this).
    args.rval().setNumber(ReadableStreamControllerGetDesiredSizeUnchecked(controller));
    return true;
}
=======
  // Step a: Set controller.[[started]] to true.
  controller->setStarted();
>>>>>>> upstream-releases

<<<<<<< HEAD
/**
 * Streams spec, 3.8.3.
 * new ReadableStreamDefaultController( stream, underlyingSource, size,
 *                                      highWaterMark )
 */
bool ReadableStreamDefaultController::constructor(JSContext* cx, unsigned argc,
                                                  Value* vp) {
  // Step 1: Throw a TypeError.
  JS_ReportErrorNumberASCII(cx, GetErrorMessage, nullptr,
                            JSMSG_BOGUS_CONSTRUCTOR,
                            "ReadableStreamDefaultController");
  return false;
}
||||||| merged common ancestors
static bool
ReadableStreamDefaultController_desiredSize(JSContext* cx, unsigned argc, Value* vp)
{
    // Step 1: If ! IsReadableStreamDefaultController(this) is false, throw a
    //         TypeError exception.
    CallArgs args = CallArgsFromVp(argc, vp);
    return CallNonGenericMethod<Is<ReadableStreamDefaultController>,
                                ReadableStreamController_desiredSize_impl>(cx, args);
}
=======
  // Step b: Assert: controller.[[pulling]] is false.
  MOZ_ASSERT(!controller->pulling());
>>>>>>> upstream-releases

<<<<<<< HEAD
static MOZ_MUST_USE double ReadableStreamControllerGetDesiredSizeUnchecked(
    ReadableStreamController* controller);
||||||| merged common ancestors
static MOZ_MUST_USE bool
ReadableStreamDefaultControllerClose(JSContext* cx,
                                     Handle<ReadableStreamDefaultController*> controller);
=======
  // Step c: Assert: controller.[[pullAgain]] is false.
  MOZ_ASSERT(!controller->pullAgain());

  // Step d: Perform
  //      ! ReadableStreamDefaultControllerCallPullIfNeeded(controller)
  //      (or ReadableByteStreamControllerCallPullIfNeeded(controller)).
  if (!ReadableStreamControllerCallPullIfNeeded(cx, controller)) {
    return false;
  }
  args.rval().setUndefined();
  return true;
}
>>>>>>> upstream-releases

/**
<<<<<<< HEAD
 * Streams spec, 3.8.4.1. get desiredSize
 * and
 * Streams spec, 3.10.4.2. get desiredSize
||||||| merged common ancestors
 * Unified implementation of steps 2-3 of 3.8.4.2 and 3.10.4.3.
 *
 * Note: can operate on unwrapped ReadableStream controller instances from
 * another compartment.
=======
 * Streams spec, 3.9.11. SetUpReadableStreamDefaultController, step 12
 * and
 * Streams spec, 3.12.26. SetUpReadableByteStreamController, step 17:
 *      Upon rejection of startPromise with reason r, [...]
>>>>>>> upstream-releases
 */
<<<<<<< HEAD
static bool ReadableStreamDefaultController_desiredSize(JSContext* cx,
                                                        unsigned argc,
                                                        Value* vp) {
  // Step 1: If ! IsReadableStreamDefaultController(this) is false, throw a
  //         TypeError exception.
  CallArgs args = CallArgsFromVp(argc, vp);
  Rooted<ReadableStreamController*> unwrappedController(
      cx, UnwrapAndTypeCheckThis<ReadableStreamDefaultController>(
              cx, args, "get desiredSize"));
  if (!unwrappedController) {
    return false;
  }
||||||| merged common ancestors
static MOZ_MUST_USE bool
VerifyControllerStateForClosing(JSContext* cx, Handle<ReadableStreamController*> controller)
{
    // Step 2: If this.[[closeRequested]] is true, throw a TypeError exception.
    if (ControllerFlags(controller) & ControllerFlag_CloseRequested) {
        JS_ReportErrorNumberASCII(cx, GetErrorMessage, nullptr,
                                  JSMSG_READABLESTREAMCONTROLLER_CLOSED, "close");
        return false;
    }
=======
static bool ControllerStartFailedHandler(JSContext* cx, unsigned argc,
                                         Value* vp) {
  CallArgs args = CallArgsFromVp(argc, vp);
  Rooted<ReadableStreamController*> controller(
      cx, TargetFromHandler<ReadableStreamController>(args));

  // Step a: Perform
  //      ! ReadableStreamDefaultControllerError(controller, r)
  //      (or ReadableByteStreamControllerError(controller, r)).
  if (!ReadableStreamControllerError(cx, controller, args.get(0))) {
    return false;
  }
>>>>>>> upstream-releases

<<<<<<< HEAD
  // Streams spec, 3.9.8. steps 1-4.
  // 3.9.8. Step 1: Let stream be controller.[[controlledReadableStream]].
  ReadableStream* unwrappedStream = unwrappedController->stream();

  // 3.9.8. Step 2: Let state be stream.[[state]].
  // 3.9.8. Step 3: If state is "errored", return null.
  if (unwrappedStream->errored()) {
    args.rval().setNull();
    return true;
  }
||||||| merged common ancestors
    // Step 3: If this.[[controlledReadableStream]].[[state]] is not "readable",
    //         throw a TypeError exception.
    ReadableStream* stream = StreamFromController(controller);
    if (!stream->readable()) {
        JS_ReportErrorNumberASCII(cx, GetErrorMessage, nullptr,
                                  JSMSG_READABLESTREAMCONTROLLER_NOT_READABLE, "close");
        return false;
    }
=======
  args.rval().setUndefined();
  return true;
}
>>>>>>> upstream-releases

<<<<<<< HEAD
  // 3.9.8. Step 4: If state is "closed", return 0.
  if (unwrappedStream->closed()) {
    args.rval().setInt32(0);
    return true;
  }

  // Step 2: Return ! ReadableStreamDefaultControllerGetDesiredSize(this).
  args.rval().setNumber(
      ReadableStreamControllerGetDesiredSizeUnchecked(unwrappedController));
  return true;
||||||| merged common ancestors
    return true;
=======
/**
 * Streams spec, 3.8.3.
 * new ReadableStreamDefaultController( stream, underlyingSource, size,
 *                                      highWaterMark )
 */
bool ReadableStreamDefaultController::constructor(JSContext* cx, unsigned argc,
                                                  Value* vp) {
  // Step 1: Throw a TypeError.
  JS_ReportErrorNumberASCII(cx, GetErrorMessage, nullptr,
                            JSMSG_BOGUS_CONSTRUCTOR,
                            "ReadableStreamDefaultController");
  return false;
>>>>>>> upstream-releases
}

<<<<<<< HEAD
static MOZ_MUST_USE bool ReadableStreamDefaultControllerClose(
    JSContext* cx,
    Handle<ReadableStreamDefaultController*> unwrappedController);

||||||| merged common ancestors
=======
static MOZ_MUST_USE double ReadableStreamControllerGetDesiredSizeUnchecked(
    ReadableStreamController* controller);

>>>>>>> upstream-releases
/**
<<<<<<< HEAD
 * Unified implementation of step 2 of 3.8.4.2 and steps 2-3 of 3.10.4.3.
||||||| merged common ancestors
 * Streams spec, 3.8.4.2 close()
 *
 * Note: can operate on unwrapped ReadableStreamDefaultController instances from
 * another compartment.
=======
 * Streams spec, 3.8.4.1. get desiredSize
>>>>>>> upstream-releases
 */
<<<<<<< HEAD
static MOZ_MUST_USE bool VerifyControllerStateForClosing(
    JSContext* cx, Handle<ReadableStreamController*> unwrappedController) {
  // Step 2: If this.[[closeRequested]] is true, throw a TypeError exception.
  if (unwrappedController->closeRequested()) {
    JS_ReportErrorNumberASCII(cx, GetErrorMessage, nullptr,
                              JSMSG_READABLESTREAMCONTROLLER_CLOSED, "close");
    return false;
  }
||||||| merged common ancestors
static MOZ_MUST_USE bool
ReadableStreamDefaultController_close_impl(JSContext* cx, const CallArgs& args)
{
    Rooted<ReadableStreamDefaultController*> controller(cx);
    controller = &UncheckedUnwrap(&args.thisv().toObject())->as<ReadableStreamDefaultController>();
=======
static bool ReadableStreamDefaultController_desiredSize(JSContext* cx,
                                                        unsigned argc,
                                                        Value* vp) {
  // Step 1: If ! IsReadableStreamDefaultController(this) is false, throw a
  //         TypeError exception.
  CallArgs args = CallArgsFromVp(argc, vp);
  Rooted<ReadableStreamController*> unwrappedController(
      cx, UnwrapAndTypeCheckThis<ReadableStreamDefaultController>(
              cx, args, "get desiredSize"));
  if (!unwrappedController) {
    return false;
  }
>>>>>>> upstream-releases

<<<<<<< HEAD
  // Step 3: If this.[[controlledReadableStream]].[[state]] is not "readable",
  //         throw a TypeError exception.
  ReadableStream* unwrappedStream = unwrappedController->stream();
  if (!unwrappedStream->readable()) {
    JS_ReportErrorNumberASCII(cx, GetErrorMessage, nullptr,
                              JSMSG_READABLESTREAMCONTROLLER_NOT_READABLE,
                              "close");
    return false;
  }
||||||| merged common ancestors
    // Steps 2-3.
    if (!VerifyControllerStateForClosing(cx, controller)) {
        return false;
    }
=======
  // 3.9.8. ReadableStreamDefaultControllerGetDesiredSize, steps 1-4.
  // 3.9.8. Step 1: Let stream be controller.[[controlledReadableStream]].
  ReadableStream* unwrappedStream = unwrappedController->stream();
>>>>>>> upstream-releases

<<<<<<< HEAD
  return true;
}
||||||| merged common ancestors
    // Step 4: Perform ! ReadableStreamDefaultControllerClose(this).
    if (!ReadableStreamDefaultControllerClose(cx, controller)) {
        return false;
    }
    args.rval().setUndefined();
    return true;
}
=======
  // 3.9.8. Step 2: Let state be stream.[[state]].
  // 3.9.8. Step 3: If state is "errored", return null.
  if (unwrappedStream->errored()) {
    args.rval().setNull();
    return true;
  }
>>>>>>> upstream-releases

<<<<<<< HEAD
/**
 * Streams spec, 3.8.4.2 close()
 */
static bool ReadableStreamDefaultController_close(JSContext* cx, unsigned argc,
                                                  Value* vp) {
  // Step 1: If ! IsReadableStreamDefaultController(this) is false, throw a
  //         TypeError exception.
  CallArgs args = CallArgsFromVp(argc, vp);
  Rooted<ReadableStreamDefaultController*> unwrappedController(
      cx, UnwrapAndTypeCheckThis<ReadableStreamDefaultController>(cx, args,
                                                                  "close"));
  if (!unwrappedController) {
    return false;
  }
||||||| merged common ancestors
static bool
ReadableStreamDefaultController_close(JSContext* cx, unsigned argc, Value* vp)
{
    // Step 1: If ! IsReadableStreamDefaultController(this) is false, throw a
    //         TypeError exception.
=======
  // 3.9.8. Step 4: If state is "closed", return 0.
  if (unwrappedStream->closed()) {
    args.rval().setInt32(0);
    return true;
  }
>>>>>>> upstream-releases

<<<<<<< HEAD
  // Steps 2-3.
  if (!VerifyControllerStateForClosing(cx, unwrappedController)) {
    return false;
  }
||||||| merged common ancestors
    CallArgs args = CallArgsFromVp(argc, vp);
    return CallNonGenericMethod<IsMaybeWrapped<ReadableStreamDefaultController>,
                                ReadableStreamDefaultController_close_impl>(cx, args);
}
=======
  // Step 2: Return ! ReadableStreamDefaultControllerGetDesiredSize(this).
  args.rval().setNumber(
      ReadableStreamControllerGetDesiredSizeUnchecked(unwrappedController));
  return true;
}
>>>>>>> upstream-releases

<<<<<<< HEAD
  // Step 4: Perform ! ReadableStreamDefaultControllerClose(this).
  if (!ReadableStreamDefaultControllerClose(cx, unwrappedController)) {
    return false;
  }
  args.rval().setUndefined();
  return true;
}
||||||| merged common ancestors
// Streams spec, 3.8.4.3. enqueue ( chunk )
static MOZ_MUST_USE bool
ReadableStreamDefaultController_enqueue_impl(JSContext* cx, const CallArgs& args)
{
    Rooted<ReadableStreamDefaultController*> controller(cx);
    controller = &UncheckedUnwrap(&args.thisv().toObject())->as<ReadableStreamDefaultController>();
=======
static MOZ_MUST_USE bool ReadableStreamDefaultControllerClose(
    JSContext* cx,
    Handle<ReadableStreamDefaultController*> unwrappedController);
>>>>>>> upstream-releases

<<<<<<< HEAD
/**
 * Streams spec, 3.8.4.3. enqueue ( chunk )
 */
static bool ReadableStreamDefaultController_enqueue(JSContext* cx,
                                                    unsigned argc, Value* vp) {
  // Step 1: If ! IsReadableStreamDefaultController(this) is false, throw a
  //         TypeError exception.
  CallArgs args = CallArgsFromVp(argc, vp);
  Rooted<ReadableStreamDefaultController*> unwrappedController(
      cx, UnwrapAndTypeCheckThis<ReadableStreamDefaultController>(cx, args,
                                                                  "enqueue"));
  if (!unwrappedController) {
    return false;
  }
||||||| merged common ancestors
    // Step 2: If this.[[closeRequested]] is true, throw a TypeError exception.
    if (ControllerFlags(controller) & ControllerFlag_CloseRequested) {
        JS_ReportErrorNumberASCII(cx, GetErrorMessage, nullptr,
                                  JSMSG_READABLESTREAMCONTROLLER_CLOSED, "enqueue");
        return false;
    }
=======
/**
 * Unified implementation of step 2 of 3.8.4.2 and 3.8.4.3,
 * and steps 2-3 of 3.10.4.3.
 */
static MOZ_MUST_USE bool CheckReadableStreamControllerCanCloseOrEnqueue(
    JSContext* cx, Handle<ReadableStreamController*> unwrappedController,
    const char* action) {
  // 3.8.4.2. close(), step 2, and
  // 3.8.4.3. enqueue(chunk), step 2:
  //      If ! ReadableStreamDefaultControllerCanCloseOrEnqueue(this) is false,
  //      throw a TypeError exception.
  // RSDCCanCloseOrEnqueue returns false in two cases: (1)
  // controller.[[closeRequested]] is true; (2) the stream is not readable,
  // i.e. already closed or errored. This amounts to exactly the same thing as
  // 3.10.4.3 steps 2-3 below, and we want different error messages for the two
  // cases anyway.

  // 3.10.4.3. Step 2: If this.[[closeRequested]] is true, throw a TypeError
  //                   exception.
  if (unwrappedController->closeRequested()) {
    JS_ReportErrorNumberASCII(cx, GetErrorMessage, nullptr,
                              JSMSG_READABLESTREAMCONTROLLER_CLOSED, action);
    return false;
  }
>>>>>>> upstream-releases

<<<<<<< HEAD
  // Step 2: If this.[[closeRequested]] is true, throw a TypeError exception.
  if (unwrappedController->closeRequested()) {
    JS_ReportErrorNumberASCII(cx, GetErrorMessage, nullptr,
                              JSMSG_READABLESTREAMCONTROLLER_CLOSED, "enqueue");
    return false;
  }
||||||| merged common ancestors
    // Step 3: If this.[[controlledReadableStream]].[[state]] is not "readable",
    //         throw a TypeError exception.
    ReadableStream* stream = StreamFromController(controller);
    if (!stream->readable()) {
        JS_ReportErrorNumberASCII(cx, GetErrorMessage, nullptr,
                                  JSMSG_READABLESTREAMCONTROLLER_NOT_READABLE, "enqueue");
        return false;
    }
=======
  // 3.10.4.3. Step 3: If this.[[controlledReadableByteStream]].[[state]] is
  //                   not "readable", throw a TypeError exception.
  ReadableStream* unwrappedStream = unwrappedController->stream();
  if (!unwrappedStream->readable()) {
    JS_ReportErrorNumberASCII(cx, GetErrorMessage, nullptr,
                              JSMSG_READABLESTREAMCONTROLLER_NOT_READABLE,
                              action);
    return false;
  }
>>>>>>> upstream-releases

<<<<<<< HEAD
  // Step 3: If this.[[controlledReadableStream]].[[state]] is not "readable",
  //         throw a TypeError exception.
  if (!unwrappedController->stream()->readable()) {
    JS_ReportErrorNumberASCII(cx, GetErrorMessage, nullptr,
                              JSMSG_READABLESTREAMCONTROLLER_NOT_READABLE,
                              "enqueue");
    return false;
  }
||||||| merged common ancestors
    // Step 4: Return ! ReadableStreamDefaultControllerEnqueue(this, chunk).
    if (!ReadableStreamDefaultControllerEnqueue(cx, controller, args.get(0))) {
        return false;
    }
    args.rval().setUndefined();
    return true;
}
=======
  return true;
}
>>>>>>> upstream-releases

<<<<<<< HEAD
  // Step 4: Return ! ReadableStreamDefaultControllerEnqueue(this, chunk).
  if (!ReadableStreamDefaultControllerEnqueue(cx, unwrappedController,
                                              args.get(0))) {
    return false;
  }
  args.rval().setUndefined();
  return true;
||||||| merged common ancestors
static bool
ReadableStreamDefaultController_enqueue(JSContext* cx, unsigned argc, Value* vp)
{
    // Step 1: If ! IsReadableStreamDefaultController(this) is false, throw a
    //         TypeError exception.
    CallArgs args = CallArgsFromVp(argc, vp);
    return CallNonGenericMethod<IsMaybeWrapped<ReadableStreamDefaultController>,
                                ReadableStreamDefaultController_enqueue_impl>(cx, args);
=======
/**
 * Streams spec, 3.8.4.2 close()
 */
static bool ReadableStreamDefaultController_close(JSContext* cx, unsigned argc,
                                                  Value* vp) {
  // Step 1: If ! IsReadableStreamDefaultController(this) is false, throw a
  //         TypeError exception.
  CallArgs args = CallArgsFromVp(argc, vp);
  Rooted<ReadableStreamDefaultController*> unwrappedController(
      cx, UnwrapAndTypeCheckThis<ReadableStreamDefaultController>(cx, args,
                                                                  "close"));
  if (!unwrappedController) {
    return false;
  }

  // Step 2: If ! ReadableStreamDefaultControllerCanCloseOrEnqueue(this) is
  //         false, throw a TypeError exception.
  if (!CheckReadableStreamControllerCanCloseOrEnqueue(cx, unwrappedController,
                                                      "close")) {
    return false;
  }

  // Step 3: Perform ! ReadableStreamDefaultControllerClose(this).
  if (!ReadableStreamDefaultControllerClose(cx, unwrappedController)) {
    return false;
  }

  args.rval().setUndefined();
  return true;
>>>>>>> upstream-releases
}

/**
<<<<<<< HEAD
 * Streams spec, 3.8.4.4. error ( e )
||||||| merged common ancestors
 * Streams spec, 3.8.4.4. error ( e )
 *
 * Note: can operate on unwrapped ReadableStreamDefaultController instances from
 * another compartment.
=======
 * Streams spec, 3.8.4.3. enqueue ( chunk )
>>>>>>> upstream-releases
 */
<<<<<<< HEAD
static bool ReadableStreamDefaultController_error(JSContext* cx, unsigned argc,
                                                  Value* vp) {
  // Step 1: If ! IsReadableStreamDefaultController(this) is false, throw a
  //         TypeError exception.
  CallArgs args = CallArgsFromVp(argc, vp);
  Rooted<ReadableStreamDefaultController*> unwrappedController(
      cx, UnwrapAndTypeCheckThis<ReadableStreamDefaultController>(cx, args,
                                                                  "enqueue"));
  if (!unwrappedController) {
    return false;
  }
||||||| merged common ancestors
static MOZ_MUST_USE bool
ReadableStreamDefaultController_error_impl(JSContext* cx, const CallArgs& args)
{
    Rooted<ReadableStreamDefaultController*> controller(cx);
    controller = &UncheckedUnwrap(&args.thisv().toObject())->as<ReadableStreamDefaultController>();

    // Step 2: Let stream be this.[[controlledReadableStream]].
    // Step 3: If stream.[[state]] is not "readable", throw a TypeError exception.
    if (!StreamFromController(controller)->readable()) {
        JS_ReportErrorNumberASCII(cx, GetErrorMessage, nullptr,
                                  JSMSG_READABLESTREAMCONTROLLER_NOT_READABLE, "error");
        return false;
    }

    // Step 4: Perform ! ReadableStreamDefaultControllerError(this, e).
    if (!ReadableStreamControllerError(cx, controller, args.get(0))) {
        return false;
    }
    args.rval().setUndefined();
    return true;
}
=======
static bool ReadableStreamDefaultController_enqueue(JSContext* cx,
                                                    unsigned argc, Value* vp) {
  // Step 1: If ! IsReadableStreamDefaultController(this) is false, throw a
  //         TypeError exception.
  CallArgs args = CallArgsFromVp(argc, vp);
  Rooted<ReadableStreamDefaultController*> unwrappedController(
      cx, UnwrapAndTypeCheckThis<ReadableStreamDefaultController>(cx, args,
                                                                  "enqueue"));
  if (!unwrappedController) {
    return false;
  }

  // Step 2: If ! ReadableStreamDefaultControllerCanCloseOrEnqueue(this) is
  //         false, throw a TypeError exception.
  if (!CheckReadableStreamControllerCanCloseOrEnqueue(cx, unwrappedController,
                                                      "enqueue")) {
    return false;
  }

  // Step 3: Return ! ReadableStreamDefaultControllerEnqueue(this, chunk).
  if (!ReadableStreamDefaultControllerEnqueue(cx, unwrappedController,
                                              args.get(0))) {
    return false;
  }
  args.rval().setUndefined();
  return true;
}
>>>>>>> upstream-releases

<<<<<<< HEAD
  // Step 2: Perform ! ReadableStreamDefaultControllerError(this, e).
  if (!ReadableStreamControllerError(cx, unwrappedController, args.get(0))) {
    return false;
  }
||||||| merged common ancestors
static bool
ReadableStreamDefaultController_error(JSContext* cx, unsigned argc, Value* vp)
{
    // Step 1: If ! IsReadableStreamDefaultController(this) is false, throw a
    //         TypeError exception.
=======
/**
 * Streams spec, 3.8.4.4. error ( e )
 */
static bool ReadableStreamDefaultController_error(JSContext* cx, unsigned argc,
                                                  Value* vp) {
  // Step 1: If ! IsReadableStreamDefaultController(this) is false, throw a
  //         TypeError exception.
  CallArgs args = CallArgsFromVp(argc, vp);
  Rooted<ReadableStreamDefaultController*> unwrappedController(
      cx, UnwrapAndTypeCheckThis<ReadableStreamDefaultController>(cx, args,
                                                                  "enqueue"));
  if (!unwrappedController) {
    return false;
  }
>>>>>>> upstream-releases

<<<<<<< HEAD
  args.rval().setUndefined();
  return true;
||||||| merged common ancestors
    CallArgs args = CallArgsFromVp(argc, vp);
    return CallNonGenericMethod<IsMaybeWrapped<ReadableStreamDefaultController>,
                                ReadableStreamDefaultController_error_impl>(cx, args);
=======
  // Step 2: Perform ! ReadableStreamDefaultControllerError(this, e).
  if (!ReadableStreamControllerError(cx, unwrappedController, args.get(0))) {
    return false;
  }

  args.rval().setUndefined();
  return true;
>>>>>>> upstream-releases
}

static const JSPropertySpec ReadableStreamDefaultController_properties[] = {
    JS_PSG("desiredSize", ReadableStreamDefaultController_desiredSize, 0),
    JS_PS_END};

static const JSFunctionSpec ReadableStreamDefaultController_methods[] = {
    JS_FN("close", ReadableStreamDefaultController_close, 0, 0),
    JS_FN("enqueue", ReadableStreamDefaultController_enqueue, 1, 0),
    JS_FN("error", ReadableStreamDefaultController_error, 1, 0), JS_FS_END};

<<<<<<< HEAD
CLASS_SPEC(ReadableStreamDefaultController, 0, SlotCount,
           ClassSpec::DontDefineConstructor, 0, JS_NULL_CLASS_OPS);

static MOZ_MUST_USE JSObject* PromiseCall(JSContext* cx, HandleValue F,
                                          HandleValue V, HandleValue arg);
||||||| merged common ancestors
const Class ReadableStreamController::class_ = {
    "ReadableStreamController"
};
=======
CLASS_SPEC(ReadableStreamDefaultController, 0, SlotCount,
           ClassSpec::DontDefineConstructor, 0, JS_NULL_CLASS_OPS);
>>>>>>> upstream-releases

<<<<<<< HEAD
static void ReadableStreamControllerClearAlgorithms(
    ReadableStreamController* controller);
||||||| merged common ancestors
CLASS_SPEC(ReadableStreamDefaultController, 4, 7, ClassSpec::DontDefineConstructor, 0,
           JS_NULL_CLASS_OPS);
=======
static MOZ_MUST_USE JSObject* PromiseCall(JSContext* cx, HandleValue F,
                                          HandleValue V, HandleValue arg);

static void ReadableStreamControllerClearAlgorithms(
    Handle<ReadableStreamController*> controller);
>>>>>>> upstream-releases

/**
 * Unified implementation of ReadableStream controllers' [[CancelSteps]]
 * internal methods.
 * Streams spec, 3.8.5.1. [[CancelSteps]] ( reason )
 * and
 * Streams spec, 3.10.5.1. [[CancelSteps]] ( reason )
 */
static MOZ_MUST_USE JSObject* ReadableStreamControllerCancelSteps(
    JSContext* cx, Handle<ReadableStreamController*> unwrappedController,
    HandleValue reason) {
  AssertSameCompartment(cx, reason);

  // Step 1 of 3.10.5.1: If this.[[pendingPullIntos]] is not empty,
  if (!unwrappedController->is<ReadableStreamDefaultController>()) {
    Rooted<ListObject*> unwrappedPendingPullIntos(
        cx, unwrappedController->as<ReadableByteStreamController>()
                .pendingPullIntos());

    if (unwrappedPendingPullIntos->length() != 0) {
      // Step a: Let firstDescriptor be the first element of
      //         this.[[pendingPullIntos]].
      PullIntoDescriptor* unwrappedDescriptor =
          UnwrapAndDowncastObject<PullIntoDescriptor>(
              cx, &unwrappedPendingPullIntos->get(0).toObject());
      if (!unwrappedDescriptor) {
        return nullptr;
      }

      // Step b: Set firstDescriptor.[[bytesFilled]] to 0.
      unwrappedDescriptor->setBytesFilled(0);
    }
  }

  RootedValue unwrappedUnderlyingSource(cx);
  unwrappedUnderlyingSource = unwrappedController->underlyingSource();

  // Step 1 of 3.8.5.1, step 2 of 3.10.5.1: Perform ! ResetQueue(this).
  if (!ResetQueue(cx, unwrappedController)) {
    return nullptr;
  }

  // Step 2 of 3.8.5.1, step 3 of 3.10.5.1: Let result be the result of
  //     performing this.[[cancelAlgorithm]], passing reason.
  //
  // Our representation of cancel algorithms is a bit awkward, for
  // performance, so we must figure out which algorithm is being invoked.
  RootedObject result(cx);
  if (IsMaybeWrapped<TeeState>(unwrappedUnderlyingSource)) {
    // The cancel algorithm given in ReadableStreamTee step 13 or 14.
    MOZ_ASSERT(unwrappedUnderlyingSource.toObject().is<TeeState>(),
               "tee streams and controllers are always same-compartment with "
               "the TeeState object");
    Rooted<TeeState*> unwrappedTeeState(
        cx, &unwrappedUnderlyingSource.toObject().as<TeeState>());
    Rooted<ReadableStreamDefaultController*> unwrappedDefaultController(
        cx, &unwrappedController->as<ReadableStreamDefaultController>());
    result = ReadableStreamTee_Cancel(cx, unwrappedTeeState,
                                      unwrappedDefaultController, reason);
  } else if (unwrappedController->hasExternalSource()) {
    // An embedding-provided cancel algorithm.
    RootedValue rval(cx);
    {
      AutoRealm ar(cx, unwrappedController);
      JS::ReadableStreamUnderlyingSource* source =
          unwrappedController->externalSource();
      Rooted<ReadableStream*> stream(cx, unwrappedController->stream());
      RootedValue wrappedReason(cx, reason);
      if (!cx->compartment()->wrap(cx, &wrappedReason)) {
        return nullptr;
      }

      cx->check(stream, wrappedReason);
      rval = source->cancel(cx, stream, wrappedReason);
    }

    // Make sure the ReadableStreamControllerClearAlgorithms call below is
    // reached, even on error.
    if (!cx->compartment()->wrap(cx, &rval)) {
      result = nullptr;
    } else {
      result = PromiseObject::unforgeableResolve(cx, rval);
    }
  } else {
    // The algorithm created in
    // SetUpReadableByteStreamControllerFromUnderlyingSource step 5.
    RootedValue unwrappedCancelMethod(cx, unwrappedController->cancelMethod());
    if (unwrappedCancelMethod.isUndefined()) {
      // CreateAlgorithmFromUnderlyingMethod step 7.
      result = PromiseObject::unforgeableResolve(cx, UndefinedHandleValue);
    } else {
      // CreateAlgorithmFromUnderlyingMethod steps 6.c.i-ii.
      {
        AutoRealm ar(cx, &unwrappedCancelMethod.toObject());
        RootedValue underlyingSource(cx, unwrappedUnderlyingSource);
        if (!cx->compartment()->wrap(cx, &underlyingSource)) {
          return nullptr;
        }
        RootedValue wrappedReason(cx, reason);
        if (!cx->compartment()->wrap(cx, &wrappedReason)) {
          return nullptr;
        }

        // If PromiseCall fails, don't bail out until after the
        // ReadableStreamControllerClearAlgorithms call below.
        result = PromiseCall(cx, unwrappedCancelMethod, underlyingSource,
                             wrappedReason);
      }
      if (!cx->compartment()->wrap(cx, &result)) {
        result = nullptr;
      }
    }
<<<<<<< HEAD
  }

  // Step 3 of 3.8.5.1, step 4 of 3.10.5.1: Perform
  //      ! ReadableByteStreamControllerClearAlgorithms(this).
  ReadableStreamControllerClearAlgorithms(unwrappedController);
||||||| merged common ancestors
=======
  }
>>>>>>> upstream-releases

<<<<<<< HEAD
  // Step 4 of 3.8.5.1, step 5 of 3.10.5.1: Return result.
  return result;
||||||| merged common ancestors
    return PromiseInvokeOrNoop(cx, underlyingSource, cx->names().cancel, reason);
=======
  // Step 3 (or 4): Perform
  //      ! ReadableStreamDefaultControllerClearAlgorithms(this)
  //      (or ReadableByteStreamControllerClearAlgorithms(this)).
  ReadableStreamControllerClearAlgorithms(unwrappedController);

  // Step 4 (or 5): Return result.
  return result;
>>>>>>> upstream-releases
}

inline static MOZ_MUST_USE bool DequeueValue(
    JSContext* cx, Handle<ReadableStreamController*> unwrappedContainer,
    MutableHandleValue chunk);

/**
<<<<<<< HEAD
 * Streams spec, 3.8.5.2. ReadableStreamDefaultController [[PullSteps]](
 * forAuthorCode )
||||||| merged common ancestors
 * Streams spec, 3.8.5.2. ReadableStreamDefaultController [[PullSteps]]()
 *
 * Note: can operate on unwrapped ReadableStreamDefaultController instances
 * from another compartment.
=======
 * Streams spec, 3.8.5.2.
 *     ReadableStreamDefaultController [[PullSteps]]( forAuthorCode )
>>>>>>> upstream-releases
 */
<<<<<<< HEAD
static JSObject* ReadableStreamDefaultControllerPullSteps(
    JSContext* cx,
    Handle<ReadableStreamDefaultController*> unwrappedController) {
  // Step 1: Let stream be this.[[controlledReadableStream]].
  Rooted<ReadableStream*> unwrappedStream(cx, unwrappedController->stream());

  // Step 2: If this.[[queue]] is not empty,
  Rooted<ListObject*> unwrappedQueue(cx);
  RootedValue val(
      cx, unwrappedController->getFixedSlot(StreamController::Slot_Queue));
  if (val.isObject()) {
    unwrappedQueue = &val.toObject().as<ListObject>();
  }

  if (unwrappedQueue && unwrappedQueue->length() != 0) {
    // Step a: Let chunk be ! DequeueValue(this.[[queue]]).
    RootedValue chunk(cx);
    if (!DequeueValue(cx, unwrappedController, &chunk)) {
      return nullptr;
    }

    // Step b: If this.[[closeRequested]] is true and this.[[queue]] is empty,
    //         perform ! ReadableStreamClose(stream).
    if (unwrappedController->closeRequested() &&
        unwrappedQueue->length() == 0) {
      if (!ReadableStreamCloseInternal(cx, unwrappedStream)) {
        return nullptr;
      }
    }

    // Step c: Otherwise, perform
    //         ! ReadableStreamDefaultControllerCallPullIfNeeded(this).
    else {
      if (!ReadableStreamControllerCallPullIfNeeded(cx, unwrappedController)) {
        return nullptr;
      }
    }

    // Step d: Return a promise resolved with
    //         ! ReadableStreamCreateReadResult(chunk, false, forAuthorCode).
    cx->check(chunk);
    ReadableStreamReader* unwrappedReader =
        UnwrapReaderFromStream(cx, unwrappedStream);
    if (!unwrappedReader) {
      return nullptr;
    }
    RootedObject readResultObj(
        cx, ReadableStreamCreateReadResult(cx, chunk, false,
                                           unwrappedReader->forAuthorCode()));
    if (!readResultObj) {
      return nullptr;
    }
    RootedValue readResult(cx, ObjectValue(*readResultObj));
    return PromiseObject::unforgeableResolve(cx, readResult);
  }

  // Step 3: Let pendingPromise be
  //         ! ReadableStreamAddReadRequest(stream, forAuthorCode).
  RootedObject pendingPromise(
      cx, ReadableStreamAddReadOrReadIntoRequest(cx, unwrappedStream));
  if (!pendingPromise) {
    return nullptr;
  }

  // Step 4: Perform ! ReadableStreamDefaultControllerCallPullIfNeeded(this).
  if (!ReadableStreamControllerCallPullIfNeeded(cx, unwrappedController)) {
    return nullptr;
  }

  // Step 5: Return pendingPromise.
  return pendingPromise;
}

/*** 3.9. Readable stream default controller abstract operations ************/

// Streams spec, 3.9.1. IsReadableStreamDefaultController ( x )
// Implemented via is<ReadableStreamDefaultController>()

/**
 * Streams spec, 3.9.2 and 3.12.3. step 7:
 * Upon fulfillment of pullPromise,
 */
static bool ControllerPullHandler(JSContext* cx, unsigned argc, Value* vp) {
  CallArgs args = CallArgsFromVp(argc, vp);

  Rooted<ReadableStreamController*> controller(
      cx, UnwrapCalleeSlot<ReadableStreamController>(cx, args, 0));
  if (!controller) {
    return false;
  }
||||||| merged common ancestors
static JSObject*
ReadableStreamDefaultControllerPullSteps(JSContext* cx,
                                         Handle<ReadableStreamController*> controller)
{
    MOZ_ASSERT(controller->is<ReadableStreamDefaultController>());

    // Step 1: Let stream be this.[[controlledReadableStream]].
    Rooted<ReadableStream*> stream(cx, StreamFromController(controller));

    // Step 2: If this.[[queue]] is not empty,
    RootedNativeObject queue(cx);
    RootedValue val(cx, controller->getFixedSlot(QueueContainerSlot_Queue));
    if (val.isObject()) {
        queue = &val.toObject().as<NativeObject>();
    }

    if (queue && queue->getDenseInitializedLength() != 0) {
        // Step a: Let chunk be ! DequeueValue(this.[[queue]]).
        RootedValue chunk(cx);
        if (!DequeueValue(cx, controller, &chunk)) {
            return nullptr;
        }

        // Step b: If this.[[closeRequested]] is true and this.[[queue]] is empty,
        //         perform ! ReadableStreamClose(stream).
        bool closeRequested = ControllerFlags(controller) & ControllerFlag_CloseRequested;
        if (closeRequested && queue->getDenseInitializedLength() == 0) {
            if (!ReadableStreamCloseInternal(cx, stream)) {
                return nullptr;
            }
        }

        // Step c: Otherwise, perform ! ReadableStreamDefaultControllerCallPullIfNeeded(this).
        else {
        if (!ReadableStreamControllerCallPullIfNeeded(cx, controller)) {
            return nullptr;
        }
        }

        // Step d: Return a promise resolved with ! CreateIterResultObject(chunk, false).
        if (!cx->compartment()->wrap(cx, &chunk)) {
            return nullptr;
        }
        RootedObject iterResultObj(cx, CreateIterResultObject(cx, chunk, false));
        if (!iterResultObj) {
            return nullptr;
        }
        RootedValue iterResult(cx, ObjectValue(*iterResultObj));
        return PromiseObject::unforgeableResolve(cx, iterResult);
    }

    // Step 3: Let pendingPromise be ! ReadableStreamAddReadRequest(stream).
    RootedObject pendingPromise(cx, ReadableStreamAddReadOrReadIntoRequest(cx, stream));
    if (!pendingPromise) {
        return nullptr;
    }
=======
static JSObject* ReadableStreamDefaultControllerPullSteps(
    JSContext* cx,
    Handle<ReadableStreamDefaultController*> unwrappedController) {
  // Step 1: Let stream be this.[[controlledReadableStream]].
  Rooted<ReadableStream*> unwrappedStream(cx, unwrappedController->stream());

  // Step 2: If this.[[queue]] is not empty,
  Rooted<ListObject*> unwrappedQueue(cx);
  RootedValue val(
      cx, unwrappedController->getFixedSlot(StreamController::Slot_Queue));
  if (val.isObject()) {
    unwrappedQueue = &val.toObject().as<ListObject>();
  }

  if (unwrappedQueue && unwrappedQueue->length() != 0) {
    // Step a: Let chunk be ! DequeueValue(this).
    RootedValue chunk(cx);
    if (!DequeueValue(cx, unwrappedController, &chunk)) {
      return nullptr;
    }

    // Step b: If this.[[closeRequested]] is true and this.[[queue]] is empty,
    if (unwrappedController->closeRequested() &&
        unwrappedQueue->length() == 0) {
      // Step i: Perform ! ReadableStreamDefaultControllerClearAlgorithms(this).
      ReadableStreamControllerClearAlgorithms(unwrappedController);

      // Step ii: Perform ! ReadableStreamClose(stream).
      if (!ReadableStreamCloseInternal(cx, unwrappedStream)) {
        return nullptr;
      }
    }
>>>>>>> upstream-releases

<<<<<<< HEAD
  bool pullAgain = controller->pullAgain();

  // Step a: Set controller.[[pulling]] to false.
  // Step b.i: Set controller.[[pullAgain]] to false.
  controller->clearPullFlags();

  // Step b: If controller.[[pullAgain]] is true,
  if (pullAgain) {
    // Step ii: Perform
    //          ! ReadableByteStreamControllerCallPullIfNeeded(controller).
    if (!ReadableStreamControllerCallPullIfNeeded(cx, controller)) {
      return false;
    }
  }
||||||| merged common ancestors
    // Step 4: Perform ! ReadableStreamDefaultControllerCallPullIfNeeded(this).
    if (!ReadableStreamControllerCallPullIfNeeded(cx, controller)) {
        return nullptr;
    }
=======
    // Step c: Otherwise, perform
    //         ! ReadableStreamDefaultControllerCallPullIfNeeded(this).
    else {
      if (!ReadableStreamControllerCallPullIfNeeded(cx, unwrappedController)) {
        return nullptr;
      }
    }

    // Step d: Return a promise resolved with
    //         ! ReadableStreamCreateReadResult(chunk, false, forAuthorCode).
    cx->check(chunk);
    ReadableStreamReader* unwrappedReader =
        UnwrapReaderFromStream(cx, unwrappedStream);
    if (!unwrappedReader) {
      return nullptr;
    }
    RootedObject readResultObj(
        cx, ReadableStreamCreateReadResult(cx, chunk, false,
                                           unwrappedReader->forAuthorCode()));
    if (!readResultObj) {
      return nullptr;
    }
    RootedValue readResult(cx, ObjectValue(*readResultObj));
    return PromiseObject::unforgeableResolve(cx, readResult);
  }

  // Step 3: Let pendingPromise be
  //         ! ReadableStreamAddReadRequest(stream, forAuthorCode).
  RootedObject pendingPromise(
      cx, ReadableStreamAddReadOrReadIntoRequest(cx, unwrappedStream));
  if (!pendingPromise) {
    return nullptr;
  }

  // Step 4: Perform ! ReadableStreamDefaultControllerCallPullIfNeeded(this).
  if (!ReadableStreamControllerCallPullIfNeeded(cx, unwrappedController)) {
    return nullptr;
  }
>>>>>>> upstream-releases

<<<<<<< HEAD
  args.rval().setUndefined();
  return true;
||||||| merged common ancestors
    // Step 5: Return pendingPromise.
    return pendingPromise;
=======
  // Step 5: Return pendingPromise.
  return pendingPromise;
>>>>>>> upstream-releases
}

<<<<<<< HEAD
/**
 * Streams spec, 3.9.2 and 3.12.3. step 8:
 * Upon rejection of pullPromise with reason e,
 */
static bool ControllerPullFailedHandler(JSContext* cx, unsigned argc,
                                        Value* vp) {
  CallArgs args = CallArgsFromVp(argc, vp);
  HandleValue e = args.get(0);

  Rooted<ReadableStreamController*> controller(
      cx, UnwrapCalleeSlot<ReadableStreamController>(cx, args, 0));
  if (!controller) {
    return false;
  }
||||||| merged common ancestors
// Streams spec, 3.9.2 and 3.12.3. step 7:
// Upon fulfillment of pullPromise,
static bool
ControllerPullHandler(JSContext* cx, unsigned argc, Value* vp)
{
    CallArgs args = CallArgsFromVp(argc, vp);
=======
/*** 3.9. Readable stream default controller abstract operations ************/
>>>>>>> upstream-releases

<<<<<<< HEAD
  // Step a: Perform ! ReadableStreamDefaultControllerError(controller, e).
  //         (ReadableByteStreamControllerError in 3.12.3.)
  if (!ReadableStreamControllerError(cx, controller, e)) {
    return false;
  }
||||||| merged common ancestors
    RootedValue controllerVal(cx, args.callee().as<JSFunction>().getExtendedSlot(0));
    Rooted<ReadableStreamController*> controller(cx);
    controller = ToUnwrapped<ReadableStreamController>(cx, controllerVal);
    if (!controller) {
        return false;
    }
=======
// Streams spec, 3.9.1. IsReadableStreamDefaultController ( x )
// Implemented via is<ReadableStreamDefaultController>()

/**
 * Streams spec, 3.9.2 and 3.12.3. step 7:
 *      Upon fulfillment of pullPromise, [...]
 */
static bool ControllerPullHandler(JSContext* cx, unsigned argc, Value* vp) {
  CallArgs args = CallArgsFromVp(argc, vp);
>>>>>>> upstream-releases

<<<<<<< HEAD
  args.rval().setUndefined();
  return true;
}
||||||| merged common ancestors
    uint32_t flags = ControllerFlags(controller);
=======
  Rooted<ReadableStreamController*> unwrappedController(
      cx, UnwrapCalleeSlot<ReadableStreamController>(cx, args, 0));
  if (!unwrappedController) {
    return false;
  }
>>>>>>> upstream-releases

<<<<<<< HEAD
static bool ReadableStreamControllerShouldCallPull(
    ReadableStreamController* unwrappedController);
||||||| merged common ancestors
    // Step a: Set controller.[[pulling]] to false.
    // Step b.i: Set controller.[[pullAgain]] to false.
    RemoveControllerFlags(controller, ControllerFlag_Pulling | ControllerFlag_PullAgain);
=======
  bool pullAgain = unwrappedController->pullAgain();
>>>>>>> upstream-releases

<<<<<<< HEAD
static MOZ_MUST_USE double ReadableStreamControllerGetDesiredSizeUnchecked(
    ReadableStreamController* unwrappedController);
||||||| merged common ancestors
    // Step b: If controller.[[pullAgain]] is true,
    if (flags & ControllerFlag_PullAgain) {
        // Step ii: Perform ! ReadableByteStreamControllerCallPullIfNeeded(controller).
        if (!ReadableStreamControllerCallPullIfNeeded(cx, controller)) {
            return false;
        }
    }
=======
  // Step a: Set controller.[[pulling]] to false.
  // Step b.i: Set controller.[[pullAgain]] to false.
  unwrappedController->clearPullFlags();

  // Step b: If controller.[[pullAgain]] is true,
  if (pullAgain) {
    // Step ii: Perform
    //          ! ReadableStreamDefaultControllerCallPullIfNeeded(controller)
    //          (or ReadableByteStreamControllerCallPullIfNeeded(controller)).
    if (!ReadableStreamControllerCallPullIfNeeded(cx, unwrappedController)) {
      return false;
    }
  }
>>>>>>> upstream-releases

<<<<<<< HEAD
/**
 * Streams spec, 3.9.2
 *      ReadableStreamDefaultControllerCallPullIfNeeded ( controller )
 * Streams spec, 3.12.3.
 *      ReadableByteStreamControllerCallPullIfNeeded ( controller )
 */
inline static MOZ_MUST_USE bool ReadableStreamControllerCallPullIfNeeded(
    JSContext* cx, Handle<ReadableStreamController*> unwrappedController) {
  // Step 1: Let shouldPull be
  //         ! ReadableStreamDefaultControllerShouldCallPull(controller).
  // (ReadableByteStreamDefaultControllerShouldCallPull in 3.12.3.)
  bool shouldPull = ReadableStreamControllerShouldCallPull(unwrappedController);

  // Step 2: If shouldPull is false, return.
  if (!shouldPull) {
    return true;
  }
||||||| merged common ancestors
    args.rval().setUndefined();
    return true;
}
=======
  args.rval().setUndefined();
  return true;
}
>>>>>>> upstream-releases

<<<<<<< HEAD
  // Step 3: If controller.[[pulling]] is true,
  if (unwrappedController->pulling()) {
    // Step a: Set controller.[[pullAgain]] to true.
    unwrappedController->setPullAgain();

    // Step b: Return.
    return true;
  }
||||||| merged common ancestors
// Streams spec, 3.9.2 and 3.12.3. step 8:
// Upon rejection of pullPromise with reason e,
static bool
ControllerPullFailedHandler(JSContext* cx, unsigned argc, Value* vp)
{
    CallArgs args = CallArgsFromVp(argc, vp);
    HandleValue e = args.get(0);

    RootedValue controllerVal(cx, args.callee().as<JSFunction>().getExtendedSlot(0));
    Rooted<ReadableStreamController*> controller(cx);
    controller = ToUnwrapped<ReadableStreamController>(cx, controllerVal);
    if (!controller) {
        return false;
    }
=======
/**
 * Streams spec, 3.9.2 and 3.12.3. step 8:
 * Upon rejection of pullPromise with reason e,
 */
static bool ControllerPullFailedHandler(JSContext* cx, unsigned argc,
                                        Value* vp) {
  CallArgs args = CallArgsFromVp(argc, vp);
  HandleValue e = args.get(0);

  Rooted<ReadableStreamController*> controller(
      cx, UnwrapCalleeSlot<ReadableStreamController>(cx, args, 0));
  if (!controller) {
    return false;
  }
>>>>>>> upstream-releases

<<<<<<< HEAD
  // Step 4: Assert: controller.[[pullAgain]] is false.
  MOZ_ASSERT(!unwrappedController->pullAgain());

  // Step 5: Set controller.[[pulling]] to true.
  unwrappedController->setPulling();

  // We use this variable in step 7. For ease of error-handling, we wrap it
  // early.
  RootedObject wrappedController(cx, unwrappedController);
  if (!cx->compartment()->wrap(cx, &wrappedController)) {
    return false;
  }

  // Step 6: Let pullPromise be the result of performing
  //         controller.[[pullAlgorithm]].
  // Our representation of pull algorithms is a bit awkward, for performance,
  // so we must figure out which algorithm is being invoked.
  RootedObject pullPromise(cx);
  RootedValue unwrappedUnderlyingSource(
      cx, unwrappedController->underlyingSource());

  if (IsMaybeWrapped<TeeState>(unwrappedUnderlyingSource)) {
    // The pull algorithm given in ReadableStreamTee step 12.
    MOZ_ASSERT(unwrappedUnderlyingSource.toObject().is<TeeState>(),
               "tee streams and controllers are always same-compartment with "
               "the TeeState object");
    Rooted<TeeState*> unwrappedTeeState(
        cx, &unwrappedUnderlyingSource.toObject().as<TeeState>());
    pullPromise = ReadableStreamTee_Pull(cx, unwrappedTeeState);
  } else if (unwrappedController->hasExternalSource()) {
    // An embedding-provided pull algorithm.
    {
      AutoRealm ar(cx, unwrappedController);
      JS::ReadableStreamUnderlyingSource* source =
          unwrappedController->externalSource();
      Rooted<ReadableStream*> stream(cx, unwrappedController->stream());
      double desiredSize =
          ReadableStreamControllerGetDesiredSizeUnchecked(unwrappedController);
      source->requestData(cx, stream, desiredSize);
    }
    pullPromise = PromiseObject::unforgeableResolve(cx, UndefinedHandleValue);
  } else {
    // The pull algorithm created in
    // SetUpReadableStreamDefaultControllerFromUnderlyingSource step 4.
    RootedValue unwrappedPullMethod(cx, unwrappedController->pullMethod());
    if (unwrappedPullMethod.isUndefined()) {
      // CreateAlgorithmFromUnderlyingMethod step 7.
      pullPromise = PromiseObject::unforgeableResolve(cx, UndefinedHandleValue);
    } else {
      // CreateAlgorithmFromUnderlyingMethod step 6.b.i.
      {
        AutoRealm ar(cx, &unwrappedPullMethod.toObject());
        RootedValue underlyingSource(cx, unwrappedUnderlyingSource);
        if (!cx->compartment()->wrap(cx, &underlyingSource)) {
          return false;
        }
        RootedValue controller(cx, ObjectValue(*unwrappedController));
        if (!cx->compartment()->wrap(cx, &controller)) {
          return false;
        }
        pullPromise =
            PromiseCall(cx, unwrappedPullMethod, underlyingSource, controller);
        if (!pullPromise) {
          return false;
        }
      }
      if (!cx->compartment()->wrap(cx, &pullPromise)) {
        return false;
      }
    }
  }
  if (!pullPromise) {
    return false;
  }
||||||| merged common ancestors
    // Step a: If controller.[[controlledReadableStream]].[[state]] is "readable",
    //         perform ! ReadableByteStreamControllerError(controller, e).
    if (StreamFromController(controller)->readable()) {
        if (!ReadableStreamControllerError(cx, controller, e)) {
            return false;
        }
    }
=======
  // Step a: Perform ! ReadableStreamDefaultControllerError(controller, e).
  //         (ReadableByteStreamControllerError in 3.12.3.)
  if (!ReadableStreamControllerError(cx, controller, e)) {
    return false;
  }
>>>>>>> upstream-releases

<<<<<<< HEAD
  RootedObject onPullFulfilled(
      cx, NewHandler(cx, ControllerPullHandler, wrappedController));
  if (!onPullFulfilled) {
    return false;
  }

  RootedObject onPullRejected(
      cx, NewHandler(cx, ControllerPullFailedHandler, wrappedController));
  if (!onPullRejected) {
    return false;
  }
||||||| merged common ancestors
    args.rval().setUndefined();
    return true;
}
=======
  args.rval().setUndefined();
  return true;
}
>>>>>>> upstream-releases

<<<<<<< HEAD
  return JS::AddPromiseReactions(cx, pullPromise, onPullFulfilled,
                                 onPullRejected);
||||||| merged common ancestors
static bool
ReadableStreamControllerShouldCallPull(ReadableStreamController* controller);
=======
static bool ReadableStreamControllerShouldCallPull(
    ReadableStreamController* unwrappedController);
>>>>>>> upstream-releases

<<<<<<< HEAD
  // Steps 7-8 implemented in functions above.
}
||||||| merged common ancestors
static MOZ_MUST_USE double
ReadableStreamControllerGetDesiredSizeUnchecked(ReadableStreamController* controller);
=======
static MOZ_MUST_USE double ReadableStreamControllerGetDesiredSizeUnchecked(
    ReadableStreamController* unwrappedController);
>>>>>>> upstream-releases

/**
<<<<<<< HEAD
 * Streams spec, 3.9.3.
 *      ReadableStreamDefaultControllerShouldCallPull ( controller )
 * Streams spec, 3.12.25.
 *      ReadableByteStreamControllerShouldCallPull ( controller )
||||||| merged common ancestors
 * Streams spec, 3.9.2 ReadableStreamDefaultControllerCallPullIfNeeded ( controller )
 * Streams spec, 3.12.3. ReadableByteStreamControllerCallPullIfNeeded ( controller )
 *
 * Note: can operate on unwrapped instances from other compartments for
 * |controller|.
=======
 * Streams spec, 3.9.2
 *      ReadableStreamDefaultControllerCallPullIfNeeded ( controller )
 * Streams spec, 3.12.3.
 *      ReadableByteStreamControllerCallPullIfNeeded ( controller )
>>>>>>> upstream-releases
 */
<<<<<<< HEAD
static bool ReadableStreamControllerShouldCallPull(
    ReadableStreamController* unwrappedController) {
  // Step 1: Let stream be controller.[[controlledReadableStream]].
  ReadableStream* unwrappedStream = unwrappedController->stream();

  // Step 2: If stream.[[state]] is "closed" or stream.[[state]] is "errored",
  //         return false.
  // or, equivalently
  // Step 2: If stream.[[state]] is not "readable", return false.
  if (!unwrappedStream->readable()) {
    return false;
  }

  // Step 3: If controller.[[closeRequested]] is true, return false.
  if (unwrappedController->closeRequested()) {
    return false;
  }
||||||| merged common ancestors
inline static MOZ_MUST_USE bool
ReadableStreamControllerCallPullIfNeeded(JSContext* cx,
                                         Handle<ReadableStreamController*> controller)
{
    // Step 1: Let shouldPull be
    //         ! ReadableByteStreamControllerShouldCallPull(controller).
    bool shouldPull = ReadableStreamControllerShouldCallPull(controller);

    // Step 2: If shouldPull is false, return.
    if (!shouldPull) {
        return true;
    }
=======
inline static MOZ_MUST_USE bool ReadableStreamControllerCallPullIfNeeded(
    JSContext* cx, Handle<ReadableStreamController*> unwrappedController) {
  // Step 1: Let shouldPull be
  //         ! ReadableStreamDefaultControllerShouldCallPull(controller).
  // (ReadableByteStreamDefaultControllerShouldCallPull in 3.12.3.)
  bool shouldPull = ReadableStreamControllerShouldCallPull(unwrappedController);

  // Step 2: If shouldPull is false, return.
  if (!shouldPull) {
    return true;
  }
>>>>>>> upstream-releases

<<<<<<< HEAD
  // Step 4: If controller.[[started]] is false, return false.
  if (!unwrappedController->started()) {
    return false;
  }
||||||| merged common ancestors
    // Step 3: If controller.[[pulling]] is true,
    if (ControllerFlags(controller) & ControllerFlag_Pulling) {
        // Step a: Set controller.[[pullAgain]] to true.
        AddControllerFlags(controller, ControllerFlag_PullAgain);
=======
  // Step 3: If controller.[[pulling]] is true,
  if (unwrappedController->pulling()) {
    // Step a: Set controller.[[pullAgain]] to true.
    unwrappedController->setPullAgain();
>>>>>>> upstream-releases

<<<<<<< HEAD
  // Step 5: If ! IsReadableStreamLocked(stream) is true and
  //         ! ReadableStreamGetNumReadRequests(stream) > 0, return true.
  // Steps 5-6 of 3.12.24 are equivalent in our implementation.
  if (unwrappedStream->locked() &&
      ReadableStreamGetNumReadRequests(unwrappedStream) > 0) {
    return true;
  }
||||||| merged common ancestors
        // Step b: Return.
        return true;
    }
=======
    // Step b: Return.
    return true;
  }
>>>>>>> upstream-releases

<<<<<<< HEAD
  // Step 6: Let desiredSize be
  //         ReadableStreamDefaultControllerGetDesiredSize(controller).
  double desiredSize =
      ReadableStreamControllerGetDesiredSizeUnchecked(unwrappedController);
||||||| merged common ancestors
    // Step 4: Assert: controller.[[pullAgain]] is false.
    MOZ_ASSERT(!(ControllerFlags(controller) & ControllerFlag_PullAgain));
=======
  // Step 4: Assert: controller.[[pullAgain]] is false.
  MOZ_ASSERT(!unwrappedController->pullAgain());
>>>>>>> upstream-releases

<<<<<<< HEAD
  // Step 7: If desiredSize > 0, return true.
  // Step 8: Return false.
  // Steps 7-8 of 3.12.24 are equivalent in our implementation.
  return desiredSize > 0;
}
||||||| merged common ancestors
    // Step 5: Set controller.[[pulling]] to true.
    AddControllerFlags(controller, ControllerFlag_Pulling);
=======
  // Step 5: Set controller.[[pulling]] to true.
  unwrappedController->setPulling();
>>>>>>> upstream-releases

<<<<<<< HEAD
/**
 * Streams spec, 3.9.4.
 *      ReadableStreamDefaultControllerClearAlgorithms ( controller )
 * and 3.12.4.
 *      ReadableByteStreamControllerClearAlgorithms ( controller )
 */
static void ReadableStreamControllerClearAlgorithms(
    ReadableStreamController* controller) {
  // Step 1: Set controller.[[pullAlgorithm]] to undefined.
  controller->setPullMethod(UndefinedHandleValue);

  // Step 2: Set controller.[[cancelAlgorithm]] to undefined.
  controller->setCancelMethod(UndefinedHandleValue);

  // Step 3 (of 3.9.4 only) : Set controller.[[strategySizeAlgorithm]] to
  // undefined.
  if (controller->is<ReadableStreamDefaultController>()) {
    controller->as<ReadableStreamDefaultController>().setStrategySize(
        UndefinedHandleValue);
  }
}

/**
 * Streams spec, 3.9.5. ReadableStreamDefaultControllerClose ( controller )
 */
static MOZ_MUST_USE bool ReadableStreamDefaultControllerClose(
    JSContext* cx,
    Handle<ReadableStreamDefaultController*> unwrappedController) {
  // Step 1: Let stream be controller.[[controlledReadableStream]].
  Rooted<ReadableStream*> unwrappedStream(cx, unwrappedController->stream());

  // Step 2: Assert: controller.[[closeRequested]] is false.
  MOZ_ASSERT(!unwrappedController->closeRequested());

  // Step 3: Assert: stream.[[state]] is "readable".
  MOZ_ASSERT(unwrappedStream->readable());

  // Step 4: Set controller.[[closeRequested]] to true.
  unwrappedController->setCloseRequested();
||||||| merged common ancestors
    // Step 6: Let pullPromise be
    //         ! PromiseInvokeOrNoop(controller.[[underlyingByteSource]], "pull", controller).
    RootedObject wrappedController(cx, controller);
    if (!cx->compartment()->wrap(cx, &wrappedController)) {
        return false;
    }
    RootedValue controllerVal(cx, ObjectValue(*wrappedController));
    RootedValue underlyingSource(cx);
    underlyingSource = controller->getFixedSlot(ControllerSlot_UnderlyingSource);
    RootedObject pullPromise(cx);

    if (IsMaybeWrapped<TeeState>(underlyingSource)) {
        Rooted<TeeState*> teeState(cx);
        teeState = &UncheckedUnwrap(&underlyingSource.toObject())->as<TeeState>();
        Rooted<ReadableStream*> stream(cx, StreamFromController(controller));
        pullPromise = ReadableStreamTee_Pull(cx, teeState, stream);
    } else if (ControllerFlags(controller) & ControllerFlag_ExternalSource) {
        void* source = underlyingSource.toPrivate();
        Rooted<ReadableStream*> stream(cx, StreamFromController(controller));
        double desiredSize = ReadableStreamControllerGetDesiredSizeUnchecked(controller);
        cx->runtime()->readableStreamDataRequestCallback(cx, stream, source,
                                                         stream->embeddingFlags(), desiredSize);
        pullPromise = PromiseObject::unforgeableResolve(cx, UndefinedHandleValue);
    } else {
        pullPromise = PromiseInvokeOrNoop(cx, underlyingSource, cx->names().pull, controllerVal);
    }
    if (!pullPromise) {
        return false;
    }

    RootedObject onPullFulfilled(cx, NewHandler(cx, ControllerPullHandler, wrappedController));
    if (!onPullFulfilled) {
        return false;
    }

    RootedObject onPullRejected(cx, NewHandler(cx, ControllerPullFailedHandler, wrappedController));
    if (!onPullRejected) {
        return false;
    }
=======
  // We use this variable in step 7. For ease of error-handling, we wrap it
  // early.
  RootedObject wrappedController(cx, unwrappedController);
  if (!cx->compartment()->wrap(cx, &wrappedController)) {
    return false;
  }

  // Step 6: Let pullPromise be the result of performing
  //         controller.[[pullAlgorithm]].
  // Our representation of pull algorithms is a bit awkward, for performance,
  // so we must figure out which algorithm is being invoked.
  RootedObject pullPromise(cx);
  RootedValue unwrappedUnderlyingSource(
      cx, unwrappedController->underlyingSource());

  if (IsMaybeWrapped<TeeState>(unwrappedUnderlyingSource)) {
    // The pull algorithm given in ReadableStreamTee step 12.
    MOZ_ASSERT(unwrappedUnderlyingSource.toObject().is<TeeState>(),
               "tee streams and controllers are always same-compartment with "
               "the TeeState object");
    Rooted<TeeState*> unwrappedTeeState(
        cx, &unwrappedUnderlyingSource.toObject().as<TeeState>());
    pullPromise = ReadableStreamTee_Pull(cx, unwrappedTeeState);
  } else if (unwrappedController->hasExternalSource()) {
    // An embedding-provided pull algorithm.
    {
      AutoRealm ar(cx, unwrappedController);
      JS::ReadableStreamUnderlyingSource* source =
          unwrappedController->externalSource();
      Rooted<ReadableStream*> stream(cx, unwrappedController->stream());
      double desiredSize =
          ReadableStreamControllerGetDesiredSizeUnchecked(unwrappedController);
      source->requestData(cx, stream, desiredSize);
    }
    pullPromise = PromiseObject::unforgeableResolve(cx, UndefinedHandleValue);
  } else {
    // The pull algorithm created in
    // SetUpReadableStreamDefaultControllerFromUnderlyingSource step 4.
    RootedValue unwrappedPullMethod(cx, unwrappedController->pullMethod());
    if (unwrappedPullMethod.isUndefined()) {
      // CreateAlgorithmFromUnderlyingMethod step 7.
      pullPromise = PromiseObject::unforgeableResolve(cx, UndefinedHandleValue);
    } else {
      // CreateAlgorithmFromUnderlyingMethod step 6.b.i.
      {
        AutoRealm ar(cx, &unwrappedPullMethod.toObject());
        RootedValue underlyingSource(cx, unwrappedUnderlyingSource);
        if (!cx->compartment()->wrap(cx, &underlyingSource)) {
          return false;
        }
        RootedValue controller(cx, ObjectValue(*unwrappedController));
        if (!cx->compartment()->wrap(cx, &controller)) {
          return false;
        }
        pullPromise =
            PromiseCall(cx, unwrappedPullMethod, underlyingSource, controller);
        if (!pullPromise) {
          return false;
        }
      }
      if (!cx->compartment()->wrap(cx, &pullPromise)) {
        return false;
      }
    }
  }
  if (!pullPromise) {
    return false;
  }
>>>>>>> upstream-releases

<<<<<<< HEAD
  // Step 5: If controller.[[queue]] is empty, perform
  //         ! ReadableStreamClose(stream).
  Rooted<ListObject*> unwrappedQueue(cx, unwrappedController->queue());
  if (unwrappedQueue->length() == 0) {
    return ReadableStreamCloseInternal(cx, unwrappedStream);
  }

  return true;
||||||| merged common ancestors
    return JS::AddPromiseReactions(cx, pullPromise, onPullFulfilled, onPullRejected);

    // Steps 7-8 implemented in functions above.
=======
  // Step 7: Upon fulfillment of pullPromise, [...]
  // Step 8. Upon rejection of pullPromise with reason e, [...]
  RootedObject onPullFulfilled(
      cx, NewHandler(cx, ControllerPullHandler, wrappedController));
  if (!onPullFulfilled) {
    return false;
  }
  RootedObject onPullRejected(
      cx, NewHandler(cx, ControllerPullFailedHandler, wrappedController));
  if (!onPullRejected) {
    return false;
  }
  return JS::AddPromiseReactions(cx, pullPromise, onPullFulfilled,
                                 onPullRejected);
>>>>>>> upstream-releases
}

static MOZ_MUST_USE bool EnqueueValueWithSize(
    JSContext* cx, Handle<ReadableStreamController*> unwrappedContainer,
    HandleValue value, HandleValue sizeVal);

/**
<<<<<<< HEAD
 * Streams spec, 3.9.6.
 *      ReadableStreamDefaultControllerEnqueue ( controller, chunk )
||||||| merged common ancestors
 * Streams spec, 3.9.3. ReadableStreamDefaultControllerShouldCallPull ( controller )
 * Streams spec, 3.12.24. ReadableByteStreamControllerShouldCallPull ( controller )
 *
 * Note: can operate on unwrapped ReadableStream controller instances from
 * another compartment.
=======
 * Streams spec, 3.9.3.
 *      ReadableStreamDefaultControllerShouldCallPull ( controller )
 * Streams spec, 3.12.25.
 *      ReadableByteStreamControllerShouldCallPull ( controller )
>>>>>>> upstream-releases
 */
<<<<<<< HEAD
static MOZ_MUST_USE bool ReadableStreamDefaultControllerEnqueue(
    JSContext* cx, Handle<ReadableStreamDefaultController*> unwrappedController,
    HandleValue chunk) {
  AssertSameCompartment(cx, chunk);

  // Step 1: Let stream be controller.[[controlledReadableStream]].
  Rooted<ReadableStream*> unwrappedStream(cx, unwrappedController->stream());

  // Step 2: Assert: controller.[[closeRequested]] is false.
  MOZ_ASSERT(!unwrappedController->closeRequested());

  // Step 3: If ! IsReadableStreamLocked(stream) is true and
  //         ! ReadableStreamGetNumReadRequests(stream) > 0, perform
  //         ! ReadableStreamFulfillReadRequest(stream, chunk, false).
  if (unwrappedStream->locked() &&
      ReadableStreamGetNumReadRequests(unwrappedStream) > 0) {
    if (!ReadableStreamFulfillReadOrReadIntoRequest(cx, unwrappedStream, chunk,
                                                    false)) {
      return false;
    }
  } else {
    // Step 4: Otherwise,
    // Step a: Let result be the result of performing
    //         controller.[[strategySizeAlgorithm]], passing in chunk, and
    //         interpreting the result as an ECMAScript completion value.
    // Step c: (on success) Let chunkSize be result.[[Value]].
    RootedValue chunkSize(cx, NumberValue(1));
    bool success = true;
    RootedValue strategySize(cx, unwrappedController->strategySize());
    if (!strategySize.isUndefined()) {
      if (!cx->compartment()->wrap(cx, &strategySize)) {
        return false;
      }
      success = Call(cx, strategySize, UndefinedHandleValue, chunk, &chunkSize);
    }
||||||| merged common ancestors
static bool
ReadableStreamControllerShouldCallPull(ReadableStreamController* controller)
{
    // Step 1: Let stream be controller.[[controlledReadableStream]].
    ReadableStream* stream = StreamFromController(controller);

    // Step 2: If stream.[[state]] is "closed" or stream.[[state]] is "errored",
    //         return false.
    // or, equivalently
    // Step 2: If stream.[[state]] is not "readable", return false.
    if (!stream->readable()) {
        return false;
    }
=======
static bool ReadableStreamControllerShouldCallPull(
    ReadableStreamController* unwrappedController) {
  // Step 1: Let stream be controller.[[controlledReadableStream]]
  //         (or [[controlledReadableByteStream]]).
  ReadableStream* unwrappedStream = unwrappedController->stream();

  // 3.9.3. Step 2:
  //      If ! ReadableStreamDefaultControllerCanCloseOrEnqueue(controller)
  //      is false, return false.
  // This turns out to be the same as 3.12.25 steps 2-3.

  // 3.12.25 Step 2: If stream.[[state]] is not "readable", return false.
  if (!unwrappedStream->readable()) {
    return false;
  }
>>>>>>> upstream-releases

<<<<<<< HEAD
    // Step d: Let enqueueResult be
    //         EnqueueValueWithSize(controller, chunk, chunkSize).
    if (success) {
      success = EnqueueValueWithSize(cx, unwrappedController, chunk, chunkSize);
    }
||||||| merged common ancestors
    // Step 3: If controller.[[closeRequested]] is true, return false.
    uint32_t flags = ControllerFlags(controller);
    if (flags & ControllerFlag_CloseRequested) {
        return false;
    }
=======
  // 3.12.25 Step 3: If controller.[[closeRequested]] is true, return false.
  if (unwrappedController->closeRequested()) {
    return false;
  }
>>>>>>> upstream-releases

<<<<<<< HEAD
    if (!success) {
      // Step b: If result is an abrupt completion,
      // and
      // Step e: If enqueueResult is an abrupt completion,
      RootedValue exn(cx);
      if (!cx->isExceptionPending() || !GetAndClearException(cx, &exn)) {
        // Uncatchable error. Die immediately without erroring the
        // stream.
        return false;
      }
||||||| merged common ancestors
    // Step 4: If controller.[[started]] is false, return false.
    if (!(flags & ControllerFlag_Started)) {
        return false;
    }
=======
  // Step 3 (or 4):
  //      If controller.[[started]] is false, return false.
  if (!unwrappedController->started()) {
    return false;
  }

  // 3.9.3.
  // Step 4: If ! IsReadableStreamLocked(stream) is true and
  //      ! ReadableStreamGetNumReadRequests(stream) > 0, return true.
  //
  // 3.12.25.
  // Step 5: If ! ReadableStreamHasDefaultReader(stream) is true and
  //         ! ReadableStreamGetNumReadRequests(stream) > 0, return true.
  // Step 6: If ! ReadableStreamHasBYOBReader(stream) is true and
  //         ! ReadableStreamGetNumReadIntoRequests(stream) > 0, return true.
  //
  // All of these amount to the same thing in this implementation:
  if (unwrappedStream->locked() &&
      ReadableStreamGetNumReadRequests(unwrappedStream) > 0) {
    return true;
  }
>>>>>>> upstream-releases

<<<<<<< HEAD
      // Step b.i: Perform ! ReadableStreamDefaultControllerError(
      //           controller, result.[[Value]]).
      // Step e.i: Perform ! ReadableStreamDefaultControllerError(
      //           controller, enqueueResult.[[Value]]).
      if (!ReadableStreamControllerError(cx, unwrappedController, exn)) {
        return false;
      }
||||||| merged common ancestors
    // Step 5: If ! IsReadableStreamLocked(stream) is true and
    //         ! ReadableStreamGetNumReadRequests(stream) > 0, return true.
    // Steps 5-6 of 3.12.24 are equivalent in our implementation.
    if (stream->locked() && ReadableStreamGetNumReadRequests(stream) > 0) {
        return true;
    }
=======
  // Step 5 (or 7):
  //      Let desiredSize be
  //      ! ReadableStreamDefaultControllerGetDesiredSize(controller).
  //      (ReadableByteStreamControllerGetDesiredSize in 3.12.25.)
  double desiredSize =
      ReadableStreamControllerGetDesiredSizeUnchecked(unwrappedController);
>>>>>>> upstream-releases

<<<<<<< HEAD
      // Step b.ii: Return result.
      // Step e.ii: Return enqueueResult.
      // (I.e., propagate the exception.)
      cx->setPendingException(exn);
      return false;
    }
  }
||||||| merged common ancestors
    // Step 6: Let desiredSize be ReadableStreamDefaultControllerGetDesiredSize(controller).
    double desiredSize = ReadableStreamControllerGetDesiredSizeUnchecked(controller);
=======
  // Step 6 (or 8): Assert: desiredSize is not null (implicit).
  // Step 7 (or 9): If desiredSize > 0, return true.
  // Step 8 (or 10): Return false.
  return desiredSize > 0;
}
>>>>>>> upstream-releases

<<<<<<< HEAD
  // Step 6: Perform
  //         ! ReadableStreamDefaultControllerCallPullIfNeeded(controller).
  // Step 7: Return.
  return ReadableStreamControllerCallPullIfNeeded(cx, unwrappedController);
||||||| merged common ancestors
    // Step 7: If desiredSize > 0, return true.
    // Step 8: Return false.
    // Steps 7-8 of 3.12.24 are equivalent in our implementation.
    return desiredSize > 0;
=======
/**
 * Streams spec, 3.9.4.
 *      ReadableStreamDefaultControllerClearAlgorithms ( controller )
 * and 3.12.4.
 *      ReadableByteStreamControllerClearAlgorithms ( controller )
 */
static void ReadableStreamControllerClearAlgorithms(
    Handle<ReadableStreamController*> controller) {
  // Step 1: Set controller.[[pullAlgorithm]] to undefined.
  // Step 2: Set controller.[[cancelAlgorithm]] to undefined.
  // (In this implementation, the UnderlyingSource slot is part of the
  // representation of these algorithms.)
  controller->setPullMethod(UndefinedHandleValue);
  controller->setCancelMethod(UndefinedHandleValue);
  ReadableStreamController::clearUnderlyingSource(controller);

  // Step 3 (of 3.9.4 only) : Set controller.[[strategySizeAlgorithm]] to
  // undefined.
  if (controller->is<ReadableStreamDefaultController>()) {
    controller->as<ReadableStreamDefaultController>().setStrategySize(
        UndefinedHandleValue);
  }
>>>>>>> upstream-releases
}

static MOZ_MUST_USE bool ReadableByteStreamControllerClearPendingPullIntos(
    JSContext* cx, Handle<ReadableByteStreamController*> unwrappedController);

/**
<<<<<<< HEAD
 * Streams spec, 3.9.7. ReadableStreamDefaultControllerError ( controller, e )
 * Streams spec, 3.12.11. ReadableByteStreamControllerError ( controller, e )
||||||| merged common ancestors
 * Streams spec, 3.9.4. ReadableStreamDefaultControllerClose ( controller )
 *
 * Note: can operate on unwrapped ReadableStream controller instances from
 * another compartment.
=======
 * Streams spec, 3.9.5. ReadableStreamDefaultControllerClose ( controller )
>>>>>>> upstream-releases
 */
<<<<<<< HEAD
static MOZ_MUST_USE bool ReadableStreamControllerError(
    JSContext* cx, Handle<ReadableStreamController*> unwrappedController,
    HandleValue e) {
  MOZ_ASSERT(!cx->isExceptionPending());
  AssertSameCompartment(cx, e);
||||||| merged common ancestors
static MOZ_MUST_USE bool
ReadableStreamDefaultControllerClose(JSContext* cx,
                                     Handle<ReadableStreamDefaultController*> controller)
{
    // Step 1: Let stream be controller.[[controlledReadableStream]].
    Rooted<ReadableStream*> stream(cx, StreamFromController(controller));

    // Step 2: Assert: controller.[[closeRequested]] is false.
    MOZ_ASSERT(!(ControllerFlags(controller) & ControllerFlag_CloseRequested));
=======
static MOZ_MUST_USE bool ReadableStreamDefaultControllerClose(
    JSContext* cx,
    Handle<ReadableStreamDefaultController*> unwrappedController) {
  // Step 1: Let stream be controller.[[controlledReadableStream]].
  Rooted<ReadableStream*> unwrappedStream(cx, unwrappedController->stream());

  // Step 2: Assert:
  //         ! ReadableStreamDefaultControllerCanCloseOrEnqueue(controller)
  //         is true.
  MOZ_ASSERT(!unwrappedController->closeRequested());
  MOZ_ASSERT(unwrappedStream->readable());
>>>>>>> upstream-releases

<<<<<<< HEAD
  // Step 1: Let stream be controller.[[controlledReadableStream]].
  Rooted<ReadableStream*> unwrappedStream(cx, unwrappedController->stream());
||||||| merged common ancestors
    // Step 3: Assert: stream.[[state]] is "readable".
    MOZ_ASSERT(stream->readable());
=======
  // Step 3: Set controller.[[closeRequested]] to true.
  unwrappedController->setCloseRequested();
>>>>>>> upstream-releases

<<<<<<< HEAD
  // Step 2: If stream.[[state]] is not "readable", return.
  if (!unwrappedStream->readable()) {
    return true;
  }

  // Step 3 of 3.12.10:
  // Perform ! ReadableByteStreamControllerClearPendingPullIntos(controller).
  if (unwrappedController->is<ReadableByteStreamController>()) {
    Rooted<ReadableByteStreamController*> unwrappedByteStreamController(
        cx, &unwrappedController->as<ReadableByteStreamController>());
    if (!ReadableByteStreamControllerClearPendingPullIntos(
            cx, unwrappedByteStreamController)) {
      return false;
    }
  }
||||||| merged common ancestors
    // Step 4: Set controller.[[closeRequested]] to true.
    AddControllerFlags(controller, ControllerFlag_CloseRequested);

    // Step 5: If controller.[[queue]] is empty, perform ! ReadableStreamClose(stream).
    RootedNativeObject queue(cx);
    queue = &controller->getFixedSlot(QueueContainerSlot_Queue).toObject().as<NativeObject>();
    if (queue->getDenseInitializedLength() == 0) {
        return ReadableStreamCloseInternal(cx, stream);
    }
=======
  // Step 4: If controller.[[queue]] is empty,
  Rooted<ListObject*> unwrappedQueue(cx, unwrappedController->queue());
  if (unwrappedQueue->length() == 0) {
    // Step a: Perform
    //         ! ReadableStreamDefaultControllerClearAlgorithms(controller).
    ReadableStreamControllerClearAlgorithms(unwrappedController);

    // Step b: Perform ! ReadableStreamClose(stream).
    return ReadableStreamCloseInternal(cx, unwrappedStream);
  }
>>>>>>> upstream-releases

<<<<<<< HEAD
  // Step 3 (or 4): Perform ! ResetQueue(controller).
  if (!ResetQueue(cx, unwrappedController)) {
    return false;
  }
||||||| merged common ancestors
    return true;
}
=======
  return true;
}
>>>>>>> upstream-releases

<<<<<<< HEAD
  // Step 4 (or 5): Perform ! ReadableStreamError(stream, e).
  return ReadableStreamErrorInternal(cx, unwrappedStream, e);
}
||||||| merged common ancestors
static MOZ_MUST_USE bool
EnqueueValueWithSize(JSContext* cx, Handle<ReadableStreamController*> container, HandleValue value,
                     HandleValue sizeVal);
=======
static MOZ_MUST_USE bool EnqueueValueWithSize(
    JSContext* cx, Handle<ReadableStreamController*> unwrappedContainer,
    HandleValue value, HandleValue sizeVal);
>>>>>>> upstream-releases

/**
<<<<<<< HEAD
 * Streams spec, 3.9.8.
 *      ReadableStreamDefaultControllerGetDesiredSize ( controller )
 * Streams spec 3.12.14.
 *      ReadableByteStreamControllerGetDesiredSize ( controller )
||||||| merged common ancestors
 * Streams spec, 3.9.5. ReadableStreamDefaultControllerEnqueue ( controller, chunk )
 *
 * Note: can operate on unwrapped instances from other compartments for
 * |controller|. |chunk| must be in the current cx compartment.
=======
 * Streams spec, 3.9.6.
 *      ReadableStreamDefaultControllerEnqueue ( controller, chunk )
>>>>>>> upstream-releases
 */
<<<<<<< HEAD
static MOZ_MUST_USE double ReadableStreamControllerGetDesiredSizeUnchecked(
    ReadableStreamController* controller) {
  // Steps 1-4 done at callsites, so only assert that they have been done.
#if DEBUG
  ReadableStream* stream = controller->stream();
  MOZ_ASSERT(!(stream->errored() || stream->closed()));
#endif  // DEBUG

  // Step 5: Return controller.[[strategyHWM]] − controller.[[queueTotalSize]].
  return controller->strategyHWM() - controller->queueTotalSize();
}

/**
 * Streams spec, 3.9.11.
 *      SetUpReadableStreamDefaultController(stream, controller,
 *          startAlgorithm, pullAlgorithm, cancelAlgorithm, highWaterMark,
 *          sizeAlgorithm )
 *
 * The standard algorithm takes a `controller` argument which must be a new,
 * blank object. This implementation creates a new controller instead.
 *
 * In the spec, three algorithms (startAlgorithm, pullAlgorithm,
 * cancelAlgorithm) are passed as arguments to this routine. This
 * implementation passes these "algorithms" as data, using four arguments:
 * sourceAlgorithms, underlyingSource, pullMethod, and cancelMethod. The
 * sourceAlgorithms argument tells how to interpret the other three:
 *
 * -   SourceAlgorithms::Script - We're creating a stream from a JS source.
 *     The caller is `new ReadableStream(underlyingSource)` or
 *     `JS::NewReadableDefaultStreamObject`. `underlyingSource` is the
 *     source; `pullMethod` and `cancelMethod` are its .pull and
 *     .cancel methods, which the caller has already extracted and
 *     type-checked: each one must be either a callable JS object or undefined.
 *
 *     Script streams use the start/pull/cancel algorithms defined in
 *     3.9.12. SetUpReadableStreamDefaultControllerFromUnderlyingSource, which
 *     call JS methods of the underlyingSource.
 *
 * -   SourceAlgorithms::Tee - We're creating a tee stream. `underlyingSource`
 *     is a TeeState object. `pullMethod` and `cancelMethod` are undefined.
 *
 *     Tee streams use the start/pull/cancel algorithms given in
 *     3.3.9. ReadableStreamTee.
 *
 * Note: All arguments must be same-compartment with cx. ReadableStream
 * controllers are always created in the same compartment as the stream.
 */
static MOZ_MUST_USE bool SetUpReadableStreamDefaultController(
    JSContext* cx, Handle<ReadableStream*> stream,
    SourceAlgorithms sourceAlgorithms, HandleValue underlyingSource,
    HandleValue pullMethod, HandleValue cancelMethod, double highWaterMark,
    HandleValue size) {
  cx->check(stream, underlyingSource, size);
  MOZ_ASSERT(pullMethod.isUndefined() || IsCallable(pullMethod));
  MOZ_ASSERT(cancelMethod.isUndefined() || IsCallable(cancelMethod));
  MOZ_ASSERT_IF(sourceAlgorithms != SourceAlgorithms::Script,
                pullMethod.isUndefined());
  MOZ_ASSERT_IF(sourceAlgorithms != SourceAlgorithms::Script,
                cancelMethod.isUndefined());
  MOZ_ASSERT(highWaterMark >= 0);
  MOZ_ASSERT(size.isUndefined() || IsCallable(size));

  // Done elsewhere in the standard: Create the new controller.
  Rooted<ReadableStreamDefaultController*> controller(
      cx, NewBuiltinClassInstance<ReadableStreamDefaultController>(cx));
  if (!controller) {
    return false;
  }

  // Step 1: Assert: stream.[[readableStreamController]] is undefined.
  MOZ_ASSERT(!stream->hasController());
||||||| merged common ancestors
static MOZ_MUST_USE bool
ReadableStreamDefaultControllerEnqueue(JSContext* cx,
                                       Handle<ReadableStreamDefaultController*> controller,
                                       HandleValue chunk)
{
    AssertSameCompartment(cx, chunk);

    // Step 1: Let stream be controller.[[controlledReadableStream]].
    Rooted<ReadableStream*> stream(cx, StreamFromController(controller));

    // Step 2: Assert: controller.[[closeRequested]] is false.
    MOZ_ASSERT(!(ControllerFlags(controller) & ControllerFlag_CloseRequested));

    // Step 3: Assert: stream.[[state]] is "readable".
    MOZ_ASSERT(stream->readable());
=======
static MOZ_MUST_USE bool ReadableStreamDefaultControllerEnqueue(
    JSContext* cx, Handle<ReadableStreamDefaultController*> unwrappedController,
    HandleValue chunk) {
  AssertSameCompartment(cx, chunk);

  // Step 1: Let stream be controller.[[controlledReadableStream]].
  Rooted<ReadableStream*> unwrappedStream(cx, unwrappedController->stream());

  // Step 2: Assert:
  //      ! ReadableStreamDefaultControllerCanCloseOrEnqueue(controller) is
  //      true.
  MOZ_ASSERT(!unwrappedController->closeRequested());
  MOZ_ASSERT(unwrappedStream->readable());

  // Step 3: If ! IsReadableStreamLocked(stream) is true and
  //         ! ReadableStreamGetNumReadRequests(stream) > 0, perform
  //         ! ReadableStreamFulfillReadRequest(stream, chunk, false).
  if (unwrappedStream->locked() &&
      ReadableStreamGetNumReadRequests(unwrappedStream) > 0) {
    if (!ReadableStreamFulfillReadOrReadIntoRequest(cx, unwrappedStream, chunk,
                                                    false)) {
      return false;
    }
  } else {
    // Step 4: Otherwise,
    // Step a: Let result be the result of performing
    //         controller.[[strategySizeAlgorithm]], passing in chunk, and
    //         interpreting the result as an ECMAScript completion value.
    // Step c: (on success) Let chunkSize be result.[[Value]].
    RootedValue chunkSize(cx, NumberValue(1));
    bool success = true;
    RootedValue strategySize(cx, unwrappedController->strategySize());
    if (!strategySize.isUndefined()) {
      if (!cx->compartment()->wrap(cx, &strategySize)) {
        return false;
      }
      success = Call(cx, strategySize, UndefinedHandleValue, chunk, &chunkSize);
    }

    // Step d: Let enqueueResult be
    //         EnqueueValueWithSize(controller, chunk, chunkSize).
    if (success) {
      success = EnqueueValueWithSize(cx, unwrappedController, chunk, chunkSize);
    }

    // Step b: If result is an abrupt completion,
    // and
    // Step e: If enqueueResult is an abrupt completion,
    if (!success) {
      RootedValue exn(cx);
      RootedSavedFrame stack(cx);
      if (!cx->isExceptionPending() ||
          !GetAndClearExceptionAndStack(cx, &exn, &stack)) {
        // Uncatchable error. Die immediately without erroring the
        // stream.
        return false;
      }
>>>>>>> upstream-releases

<<<<<<< HEAD
  // Step 2: Set controller.[[controlledReadableStream]] to stream.
  controller->setStream(stream);
||||||| merged common ancestors
    // Step 4: If ! IsReadableStreamLocked(stream) is true and
    //         ! ReadableStreamGetNumReadRequests(stream) > 0, perform
    //         ! ReadableStreamFulfillReadRequest(stream, chunk, false).
    if (stream->locked() && ReadableStreamGetNumReadRequests(stream) > 0) {
        if (!ReadableStreamFulfillReadOrReadIntoRequest(cx, stream, chunk, false)) {
            return false;
        }
    } else {
        // Step 5: Otherwise,
        // Step a: Let chunkSize be 1.
        RootedValue chunkSize(cx, NumberValue(1));
        bool success = true;

        // Step b: If controller.[[strategySize]] is not undefined,
        RootedValue strategySize(cx);
        strategySize = controller->getFixedSlot(DefaultControllerSlot_StrategySize);
        if (!strategySize.isUndefined()) {
            // Step i: Set chunkSize to Call(stream.[[strategySize]], undefined, chunk).
            if (!cx->compartment()->wrap(cx, &strategySize)) {
                return false;
            }
            success = Call(cx, strategySize, UndefinedHandleValue, chunk, &chunkSize);
        }
=======
      // Step b.i: Perform ! ReadableStreamDefaultControllerError(
      //           controller, result.[[Value]]).
      // Step e.i: Perform ! ReadableStreamDefaultControllerError(
      //           controller, enqueueResult.[[Value]]).
      if (!ReadableStreamControllerError(cx, unwrappedController, exn)) {
        return false;
      }
>>>>>>> upstream-releases

<<<<<<< HEAD
  // Step 3: Set controller.[[queue]] and controller.[[queueTotalSize]] to
  //         undefined (implicit), then perform ! ResetQueue(controller).
  if (!ResetQueue(cx, controller)) {
    return false;
  }
||||||| merged common ancestors
        // Step c: Let enqueueResult be
        //         EnqueueValueWithSize(controller, chunk, chunkSize).
        if (success) {
            success = EnqueueValueWithSize(cx, controller, chunk, chunkSize);
        }
=======
      // Step b.ii: Return result.
      // Step e.ii: Return enqueueResult.
      // (I.e., propagate the exception.)
      cx->setPendingException(exn, stack);
      return false;
    }
  }
>>>>>>> upstream-releases

<<<<<<< HEAD
  // Step 4: Set controller.[[started]], controller.[[closeRequested]],
  //         controller.[[pullAgain]], and controller.[[pulling]] to false.
  controller->setFlags(0);
||||||| merged common ancestors
        if (!success) {
            // Step b.ii: If chunkSize is an abrupt completion,
            // and
            // Step d: If enqueueResult is an abrupt completion,
            RootedValue exn(cx);
            if (!cx->getPendingException(&exn)) {
                return false;
            }
=======
  // Step 5: Perform
  //         ! ReadableStreamDefaultControllerCallPullIfNeeded(controller).
  return ReadableStreamControllerCallPullIfNeeded(cx, unwrappedController);
}
>>>>>>> upstream-releases

<<<<<<< HEAD
  // Step 5: Set controller.[[strategySizeAlgorithm]] to sizeAlgorithm
  //         and controller.[[strategyHWM]] to highWaterMark.
  controller->setStrategySize(size);
  controller->setStrategyHWM(highWaterMark);
||||||| merged common ancestors
            // Step b.ii.1: Perform
            //         ! ReadableStreamDefaultControllerErrorIfNeeded(controller,
            //                                                        chunkSize.[[Value]]).
            if (!ReadableStreamDefaultControllerErrorIfNeeded(cx, controller, exn)) {
                return false;
            }
=======
static MOZ_MUST_USE bool ReadableByteStreamControllerClearPendingPullIntos(
    JSContext* cx, Handle<ReadableByteStreamController*> unwrappedController);
>>>>>>> upstream-releases

<<<<<<< HEAD
  // Step 6: Set controller.[[pullAlgorithm]] to pullAlgorithm.
  // (In this implementation, the pullAlgorithm is determined by the
  // underlyingSource in combination with the pullMethod field.)
  controller->setUnderlyingSource(underlyingSource);
  controller->setPullMethod(pullMethod);
||||||| merged common ancestors
            // Step b.ii.2: Return chunkSize.
            return false;
        }
    }
=======
/**
 * Streams spec, 3.9.7. ReadableStreamDefaultControllerError ( controller, e )
 * Streams spec, 3.12.11. ReadableByteStreamControllerError ( controller, e )
 */
static MOZ_MUST_USE bool ReadableStreamControllerError(
    JSContext* cx, Handle<ReadableStreamController*> unwrappedController,
    HandleValue e) {
  MOZ_ASSERT(!cx->isExceptionPending());
  AssertSameCompartment(cx, e);

  // Step 1: Let stream be controller.[[controlledReadableStream]]
  //         (or controller.[[controlledReadableByteStream]]).
  Rooted<ReadableStream*> unwrappedStream(cx, unwrappedController->stream());

  // Step 2: If stream.[[state]] is not "readable", return.
  if (!unwrappedStream->readable()) {
    return true;
  }

  // Step 3 of 3.12.10:
  // Perform ! ReadableByteStreamControllerClearPendingPullIntos(controller).
  if (unwrappedController->is<ReadableByteStreamController>()) {
    Rooted<ReadableByteStreamController*> unwrappedByteStreamController(
        cx, &unwrappedController->as<ReadableByteStreamController>());
    if (!ReadableByteStreamControllerClearPendingPullIntos(
            cx, unwrappedByteStreamController)) {
      return false;
    }
  }

  // Step 3 (or 4): Perform ! ResetQueue(controller).
  if (!ResetQueue(cx, unwrappedController)) {
    return false;
  }
>>>>>>> upstream-releases

<<<<<<< HEAD
  // Step 7: Set controller.[[cancelAlgorithm]] to cancelAlgorithm.
  controller->setCancelMethod(cancelMethod);
||||||| merged common ancestors
    // Step 6: Perform ! ReadableStreamDefaultControllerCallPullIfNeeded(controller).
    // Step 7: Return.
    return ReadableStreamControllerCallPullIfNeeded(cx, controller);
}
=======
  // Step 4 (or 5):
  //      Perform ! ReadableStreamDefaultControllerClearAlgorithms(controller)
  //      (or ReadableByteStreamControllerClearAlgorithms(controller)).
  ReadableStreamControllerClearAlgorithms(unwrappedController);

  // Step 5 (or 6): Perform ! ReadableStreamError(stream, e).
  return ReadableStreamErrorInternal(cx, unwrappedStream, e);
}
>>>>>>> upstream-releases

<<<<<<< HEAD
  // Step 8: Set stream.[[readableStreamController]] to controller.
  stream->setController(controller);
||||||| merged common ancestors
static MOZ_MUST_USE bool
ReadableByteStreamControllerClearPendingPullIntos(JSContext* cx,
                                                  Handle<ReadableStreamController*> controller);
=======
/**
 * Streams spec, 3.9.8.
 *      ReadableStreamDefaultControllerGetDesiredSize ( controller )
 * Streams spec 3.12.14.
 *      ReadableByteStreamControllerGetDesiredSize ( controller )
 */
static MOZ_MUST_USE double ReadableStreamControllerGetDesiredSizeUnchecked(
    ReadableStreamController* controller) {
  // Steps 1-4 done at callsites, so only assert that they have been done.
#if DEBUG
  ReadableStream* stream = controller->stream();
  MOZ_ASSERT(!(stream->errored() || stream->closed()));
#endif  // DEBUG

  // Step 5: Return controller.[[strategyHWM]] − controller.[[queueTotalSize]].
  return controller->strategyHWM() - controller->queueTotalSize();
}
>>>>>>> upstream-releases

<<<<<<< HEAD
  // Step 9: Let startResult be the result of performing startAlgorithm.
  RootedValue startResult(cx);
  if (sourceAlgorithms == SourceAlgorithms::Script) {
    RootedValue controllerVal(cx, ObjectValue(*controller));
    if (!InvokeOrNoop(cx, underlyingSource, cx->names().start, controllerVal,
                      &startResult)) {
      return false;
    }
  }
||||||| merged common ancestors
/**
 * Streams spec, 3.9.6. ReadableStreamDefaultControllerError ( controller, e )
 * Streams spec, 3.12.10. ReadableByteStreamControllerError ( controller, e )
 *
 * Note: can operate on unwrapped ReadableStream controller instances from
 * another compartment.
 */
static MOZ_MUST_USE bool
ReadableStreamControllerError(JSContext* cx, Handle<ReadableStreamController*> controller,
                              HandleValue e)
{
    AssertSameCompartment(cx, e);
=======
/**
 * Streams spec, 3.9.11.
 *      SetUpReadableStreamDefaultController(stream, controller,
 *          startAlgorithm, pullAlgorithm, cancelAlgorithm, highWaterMark,
 *          sizeAlgorithm )
 *
 * The standard algorithm takes a `controller` argument which must be a new,
 * blank object. This implementation creates a new controller instead.
 *
 * In the spec, three algorithms (startAlgorithm, pullAlgorithm,
 * cancelAlgorithm) are passed as arguments to this routine. This
 * implementation passes these "algorithms" as data, using four arguments:
 * sourceAlgorithms, underlyingSource, pullMethod, and cancelMethod. The
 * sourceAlgorithms argument tells how to interpret the other three:
 *
 * -   SourceAlgorithms::Script - We're creating a stream from a JS source.
 *     The caller is `new ReadableStream(underlyingSource)` or
 *     `JS::NewReadableDefaultStreamObject`. `underlyingSource` is the
 *     source; `pullMethod` and `cancelMethod` are its .pull and
 *     .cancel methods, which the caller has already extracted and
 *     type-checked: each one must be either a callable JS object or undefined.
 *
 *     Script streams use the start/pull/cancel algorithms defined in
 *     3.9.12. SetUpReadableStreamDefaultControllerFromUnderlyingSource, which
 *     call JS methods of the underlyingSource.
 *
 * -   SourceAlgorithms::Tee - We're creating a tee stream. `underlyingSource`
 *     is a TeeState object. `pullMethod` and `cancelMethod` are undefined.
 *
 *     Tee streams use the start/pull/cancel algorithms given in
 *     3.3.9. ReadableStreamTee.
 *
 * Note: All arguments must be same-compartment with cx. ReadableStream
 * controllers are always created in the same compartment as the stream.
 */
static MOZ_MUST_USE bool SetUpReadableStreamDefaultController(
    JSContext* cx, Handle<ReadableStream*> stream,
    SourceAlgorithms sourceAlgorithms, HandleValue underlyingSource,
    HandleValue pullMethod, HandleValue cancelMethod, double highWaterMark,
    HandleValue size) {
  cx->check(stream, underlyingSource, size);
  MOZ_ASSERT(pullMethod.isUndefined() || IsCallable(pullMethod));
  MOZ_ASSERT(cancelMethod.isUndefined() || IsCallable(cancelMethod));
  MOZ_ASSERT_IF(sourceAlgorithms != SourceAlgorithms::Script,
                pullMethod.isUndefined());
  MOZ_ASSERT_IF(sourceAlgorithms != SourceAlgorithms::Script,
                cancelMethod.isUndefined());
  MOZ_ASSERT(highWaterMark >= 0);
  MOZ_ASSERT(size.isUndefined() || IsCallable(size));

  // Done elsewhere in the standard: Create the new controller.
  Rooted<ReadableStreamDefaultController*> controller(
      cx, NewBuiltinClassInstance<ReadableStreamDefaultController>(cx));
  if (!controller) {
    return false;
  }
>>>>>>> upstream-releases

<<<<<<< HEAD
  // Step 10: Let startPromise be a promise resolved with startResult.
  RootedObject startPromise(cx,
                            PromiseObject::unforgeableResolve(cx, startResult));
  if (!startPromise) {
    return false;
  }
||||||| merged common ancestors
    // Step 1: Let stream be controller.[[controlledReadableStream]].
    Rooted<ReadableStream*> stream(cx, StreamFromController(controller));
=======
  // Step 1: Assert: stream.[[readableStreamController]] is undefined.
  MOZ_ASSERT(!stream->hasController());
>>>>>>> upstream-releases

<<<<<<< HEAD
  // Step 11: Upon fulfillment of startPromise, [...]
  // Step 12: Upon rejection of startPromise with reason r, [...]
  RootedObject onStartFulfilled(
      cx, NewHandler(cx, ControllerStartHandler, controller));
  if (!onStartFulfilled) {
    return false;
  }
||||||| merged common ancestors
    // Step 2: Assert: stream.[[state]] is "readable".
    MOZ_ASSERT(stream->readable());
=======
  // Step 2: Set controller.[[controlledReadableStream]] to stream.
  controller->setStream(stream);
>>>>>>> upstream-releases

<<<<<<< HEAD
  RootedObject onStartRejected(
      cx, NewHandler(cx, ControllerStartFailedHandler, controller));
  if (!onStartRejected) {
    return false;
  }
||||||| merged common ancestors
    // Step 3 of 3.12.10:
    // Perform ! ReadableByteStreamControllerClearPendingPullIntos(controller).
    if (controller->is<ReadableByteStreamController>()) {
        Rooted<ReadableByteStreamController*> byteStreamController(cx);
        byteStreamController = &controller->as<ReadableByteStreamController>();
        if (!ReadableByteStreamControllerClearPendingPullIntos(cx, byteStreamController)) {
            return false;
        }
    }
=======
  // Step 3: Set controller.[[queue]] and controller.[[queueTotalSize]] to
  //         undefined (implicit), then perform ! ResetQueue(controller).
  if (!ResetQueue(cx, controller)) {
    return false;
  }
>>>>>>> upstream-releases

<<<<<<< HEAD
  if (!JS::AddPromiseReactions(cx, startPromise, onStartFulfilled,
                               onStartRejected)) {
    return false;
  }
||||||| merged common ancestors
    // Step 3 (or 4): Perform ! ResetQueue(controller).
    if (!ResetQueue(cx, controller)) {
        return false;
    }
=======
  // Step 4: Set controller.[[started]], controller.[[closeRequested]],
  //         controller.[[pullAgain]], and controller.[[pulling]] to false.
  controller->setFlags(0);

  // Step 5: Set controller.[[strategySizeAlgorithm]] to sizeAlgorithm
  //         and controller.[[strategyHWM]] to highWaterMark.
  controller->setStrategySize(size);
  controller->setStrategyHWM(highWaterMark);

  // Step 6: Set controller.[[pullAlgorithm]] to pullAlgorithm.
  // (In this implementation, the pullAlgorithm is determined by the
  // underlyingSource in combination with the pullMethod field.)
  controller->setUnderlyingSource(underlyingSource);
  controller->setPullMethod(pullMethod);

  // Step 7: Set controller.[[cancelAlgorithm]] to cancelAlgorithm.
  controller->setCancelMethod(cancelMethod);

  // Step 8: Set stream.[[readableStreamController]] to controller.
  stream->setController(controller);

  // Step 9: Let startResult be the result of performing startAlgorithm.
  RootedValue startResult(cx);
  if (sourceAlgorithms == SourceAlgorithms::Script) {
    RootedValue controllerVal(cx, ObjectValue(*controller));
    if (!InvokeOrNoop(cx, underlyingSource, cx->names().start, controllerVal,
                      &startResult)) {
      return false;
    }
  }

  // Step 10: Let startPromise be a promise resolved with startResult.
  RootedObject startPromise(cx,
                            PromiseObject::unforgeableResolve(cx, startResult));
  if (!startPromise) {
    return false;
  }

  // Step 11: Upon fulfillment of startPromise, [...]
  // Step 12: Upon rejection of startPromise with reason r, [...]
  RootedObject onStartFulfilled(
      cx, NewHandler(cx, ControllerStartHandler, controller));
  if (!onStartFulfilled) {
    return false;
  }
  RootedObject onStartRejected(
      cx, NewHandler(cx, ControllerStartFailedHandler, controller));
  if (!onStartRejected) {
    return false;
  }
  if (!JS::AddPromiseReactions(cx, startPromise, onStartFulfilled,
                               onStartRejected)) {
    return false;
  }
>>>>>>> upstream-releases

  return true;
}

static MOZ_MUST_USE bool CreateAlgorithmFromUnderlyingMethod(
    JSContext* cx, HandleValue underlyingObject,
    const char* methodNameForErrorMessage, HandlePropertyName methodName,
    MutableHandleValue method);

/**
 * Streams spec, 3.9.12.
 *      SetUpReadableStreamDefaultControllerFromUnderlyingSource( stream,
 *          underlyingSource, highWaterMark, sizeAlgorithm )
 */
static MOZ_MUST_USE bool
SetUpReadableStreamDefaultControllerFromUnderlyingSource(
    JSContext* cx, Handle<ReadableStream*> stream, HandleValue underlyingSource,
    double highWaterMark, HandleValue sizeAlgorithm) {
  // Step 1: Assert: underlyingSource is not undefined.
  MOZ_ASSERT(!underlyingSource.isUndefined());

  // Step 2: Let controller be ObjectCreate(the original value of
  //         ReadableStreamDefaultController's prototype property).
  // (Deferred to SetUpReadableStreamDefaultController.)

  // Step 3: Let startAlgorithm be the following steps:
  //         a. Return ? InvokeOrNoop(underlyingSource, "start",
  //                                  « controller »).
  SourceAlgorithms sourceAlgorithms = SourceAlgorithms::Script;

  // Step 4: Let pullAlgorithm be
  //         ? CreateAlgorithmFromUnderlyingMethod(underlyingSource, "pull",
  //                                               0, « controller »).
  RootedValue pullMethod(cx);
  if (!CreateAlgorithmFromUnderlyingMethod(cx, underlyingSource,
                                           "ReadableStream source.pull method",
                                           cx->names().pull, &pullMethod)) {
    return false;
  }

  // Step 5. Let cancelAlgorithm be
  //         ? CreateAlgorithmFromUnderlyingMethod(underlyingSource,
  //                                               "cancel", 1, « »).
  RootedValue cancelMethod(cx);
  if (!CreateAlgorithmFromUnderlyingMethod(
          cx, underlyingSource, "ReadableStream source.cancel method",
          cx->names().cancel, &cancelMethod)) {
    return false;
  }

<<<<<<< HEAD
  // Step 6. Perform ? SetUpReadableStreamDefaultController(stream,
  //             controller, startAlgorithm, pullAlgorithm, cancelAlgorithm,
  //             highWaterMark, sizeAlgorithm).

  return SetUpReadableStreamDefaultController(
      cx, stream, sourceAlgorithms, underlyingSource, pullMethod, cancelMethod,
      highWaterMark, sizeAlgorithm);
||||||| merged common ancestors
/**
 * Streams spec, 3.9.8. ReadableStreamDefaultControllerGetDesiredSize ( controller )
 * Streams spec 3.12.13. ReadableByteStreamControllerGetDesiredSize ( controller )
 */
static MOZ_MUST_USE double
ReadableStreamControllerGetDesiredSizeUnchecked(ReadableStreamController* controller)
{
    // Steps 1-4 done at callsites, so only assert that they have been done.
#if DEBUG
    ReadableStream* stream = StreamFromController(controller);
    MOZ_ASSERT(!(stream->errored() || stream->closed()));
#endif // DEBUG

    // Step 5: Return controller.[[strategyHWM]] − controller.[[queueTotalSize]].
    double strategyHWM = controller->getFixedSlot(ControllerSlot_StrategyHWM).toNumber();
    double queueSize = QueueSize(controller);
    return strategyHWM - queueSize;
=======
  // Step 6. Perform ? SetUpReadableStreamDefaultController(stream,
  //             controller, startAlgorithm, pullAlgorithm, cancelAlgorithm,
  //             highWaterMark, sizeAlgorithm).
  return SetUpReadableStreamDefaultController(
      cx, stream, sourceAlgorithms, underlyingSource, pullMethod, cancelMethod,
      highWaterMark, sizeAlgorithm);
>>>>>>> upstream-releases
}

/*** 3.10. Class ReadableByteStreamController *******************************/

#if 0  // disable user-defined byte streams

/**
 * Streams spec, 3.10.3
 *      new ReadableByteStreamController ( stream, underlyingSource,
 *                                         highWaterMark )
 * Steps 3 - 16.
 *
 * Note: All arguments must be same-compartment with cx. ReadableStream
 * controllers are always created in the same compartment as the stream.
 */
static MOZ_MUST_USE ReadableByteStreamController*
CreateReadableByteStreamController(JSContext* cx,
                                   Handle<ReadableStream*> stream,
                                   HandleValue underlyingByteSource,
                                   HandleValue highWaterMarkVal)
{
    cx->check(stream, underlyingByteSource, highWaterMarkVal);

    Rooted<ReadableByteStreamController*> controller(cx,
        NewBuiltinClassInstance<ReadableByteStreamController>(cx));
    if (!controller) {
        return nullptr;
    }

    // Step 3: Set this.[[controlledReadableStream]] to stream.
    controller->setStream(stream);

    // Step 4: Set this.[[underlyingByteSource]] to underlyingByteSource.
    controller->setUnderlyingSource(underlyingByteSource);

    // Step 5: Set this.[[pullAgain]], and this.[[pulling]] to false.
    controller->setFlags(0);

    // Step 6: Perform ! ReadableByteStreamControllerClearPendingPullIntos(this).
    if (!ReadableByteStreamControllerClearPendingPullIntos(cx, controller)) {
        return nullptr;
    }

    // Step 7: Perform ! ResetQueue(this).
    if (!ResetQueue(cx, controller)) {
        return nullptr;
    }

    // Step 8: Set this.[[started]] and this.[[closeRequested]] to false.
    // These should be false by default, unchanged since step 5.
    MOZ_ASSERT(controller->flags() == 0);

    // Step 9: Set this.[[strategyHWM]] to
    //         ? ValidateAndNormalizeHighWaterMark(highWaterMark).
    double highWaterMark;
    if (!ValidateAndNormalizeHighWaterMark(cx, highWaterMarkVal, &highWaterMark)) {
        return nullptr;
    }
    controller->setStrategyHWM(highWaterMark);

    // Step 10: Let autoAllocateChunkSize be
    //          ? GetV(underlyingByteSource, "autoAllocateChunkSize").
    RootedValue autoAllocateChunkSize(cx);
    if (!GetProperty(cx, underlyingByteSource, cx->names().autoAllocateChunkSize,
                     &autoAllocateChunkSize))
    {
        return nullptr;
    }

    // Step 11: If autoAllocateChunkSize is not undefined,
    if (!autoAllocateChunkSize.isUndefined()) {
        // Step a: If ! IsInteger(autoAllocateChunkSize) is false, or if
        //         autoAllocateChunkSize ≤ 0, throw a RangeError exception.
        if (!IsInteger(autoAllocateChunkSize) || autoAllocateChunkSize.toNumber() <= 0) {
            JS_ReportErrorNumberASCII(cx, GetErrorMessage, nullptr,
                                      JSMSG_READABLEBYTESTREAMCONTROLLER_BAD_CHUNKSIZE);
            return nullptr;
        }
    }

    // Step 12: Set this.[[autoAllocateChunkSize]] to autoAllocateChunkSize.
    controller->setAutoAllocateChunkSize(autoAllocateChunkSize);

    // Step 13: Set this.[[pendingPullIntos]] to a new empty List.
    if (!SetNewList(cx, controller, ReadableByteStreamController::Slot_PendingPullIntos)) {
        return nullptr;
    }

    // Step 14: Let controller be this (implicit).

    // Step 15: Let startResult be
    //          ? InvokeOrNoop(underlyingSource, "start", « this »).
    RootedValue startResult(cx);
    RootedValue controllerVal(cx, ObjectValue(*controller));
    if (!InvokeOrNoop(cx, underlyingByteSource, cx->names().start, controllerVal, &startResult)) {
        return nullptr;
    }

    // Step 16: Let startPromise be a promise resolved with startResult:
    RootedObject startPromise(cx, PromiseObject::unforgeableResolve(cx, startResult));
    if (!startPromise) {
        return nullptr;
    }

    RootedObject onStartFulfilled(cx, NewHandler(cx, ControllerStartHandler, controller));
    if (!onStartFulfilled) {
        return nullptr;
    }

    RootedObject onStartRejected(cx, NewHandler(cx, ControllerStartFailedHandler, controller));
    if (!onStartRejected) {
        return nullptr;
    }

    if (!JS::AddPromiseReactions(cx, startPromise, onStartFulfilled, onStartRejected)) {
        return nullptr;
    }

    return controller;
}

<<<<<<< HEAD
#endif  // user-defined byte streams
||||||| merged common ancestors
bool
ReadableByteStreamController::hasExternalSource() {
    return ControllerFlags(this) & ControllerFlag_ExternalSource;
}

// Streams spec, 3.10.3.
// new ReadableByteStreamController ( stream, underlyingByteSource,
//                                    highWaterMark )
bool
ReadableByteStreamController::constructor(JSContext* cx, unsigned argc, Value* vp)
{
    CallArgs args = CallArgsFromVp(argc, vp);

    if (!ThrowIfNotConstructing(cx, args, "ReadableByteStreamController")) {
        return false;
    }

    // Step 1: If ! IsReadableStream(stream) is false, throw a TypeError exception.
    HandleValue streamVal = args.get(0);
    if (!Is<ReadableStream>(streamVal)) {
        ReportArgTypeError(cx, "ReadableStreamDefaultController", "ReadableStream",
                           args.get(0));
        return false;
    }

    Rooted<ReadableStream*> stream(cx, &streamVal.toObject().as<ReadableStream>());

    // Step 2: If stream.[[readableStreamController]] is not undefined, throw a
    //         TypeError exception.
    if (HasController(stream)) {
        JS_ReportErrorNumberASCII(cx, GetErrorMessage, nullptr,
                                  JSMSG_READABLESTREAM_CONTROLLER_SET);
        return false;
    }

    RootedObject controller(cx, CreateReadableByteStreamController(cx, stream, args.get(1),
                                                                   args.get(2)));
    if (!controller) {
        return false;
    }
=======
#endif  // user-defined byte streams

/**
 * Streams spec, 3.10.3.
 * new ReadableByteStreamController ( stream, underlyingByteSource,
 *                                    highWaterMark )
 */
bool ReadableByteStreamController::constructor(JSContext* cx, unsigned argc,
                                               Value* vp) {
  // Step 1: Throw a TypeError exception.
  JS_ReportErrorNumberASCII(cx, GetErrorMessage, nullptr,
                            JSMSG_BOGUS_CONSTRUCTOR,
                            "ReadableByteStreamController");
  return false;
}

// Disconnect the source from a controller without calling finalize() on it,
// unless this class is reset(). This ensures that finalize() will not be called
// on the source if setting up the controller fails.
class MOZ_RAII AutoClearUnderlyingSource {
  Rooted<ReadableStreamController*> controller_;

 public:
  AutoClearUnderlyingSource(JSContext* cx, ReadableStreamController* controller)
      : controller_(cx, controller) {}

  ~AutoClearUnderlyingSource() {
    if (controller_) {
      ReadableStreamController::clearUnderlyingSource(
          controller_, /* finalizeSource */ false);
    }
  }

  void reset() { controller_ = nullptr; }
};

/**
 * Version of SetUpReadableByteStreamController that's specialized for handling
 * external, embedding-provided, underlying sources.
 */
static MOZ_MUST_USE bool SetUpExternalReadableByteStreamController(
    JSContext* cx, Handle<ReadableStream*> stream,
    JS::ReadableStreamUnderlyingSource* source) {
  // Done elsewhere in the standard: Create the controller object.
  Rooted<ReadableByteStreamController*> controller(
      cx, NewBuiltinClassInstance<ReadableByteStreamController>(cx));
  if (!controller) {
    return false;
  }
>>>>>>> upstream-releases

<<<<<<< HEAD
/**
 * Streams spec, 3.10.3.
 * new ReadableByteStreamController ( stream, underlyingByteSource,
 *                                    highWaterMark )
 */
bool ReadableByteStreamController::constructor(JSContext* cx, unsigned argc,
                                               Value* vp) {
  // Step 1: Throw a TypeError exception.
  JS_ReportErrorNumberASCII(cx, GetErrorMessage, nullptr,
                            JSMSG_BOGUS_CONSTRUCTOR,
                            "ReadableByteStreamController");
  return false;
}
||||||| merged common ancestors
    args.rval().setObject(*controller);
    return true;
}
=======
  AutoClearUnderlyingSource autoClear(cx, controller);
>>>>>>> upstream-releases

<<<<<<< HEAD
/**
 * Version of the ReadableByteStreamConstructor that's specialized for
 * handling external, embedding-provided, underlying sources.
 */
static MOZ_MUST_USE ReadableByteStreamController*
CreateExternalReadableByteStreamController(
    JSContext* cx, Handle<ReadableStream*> stream,
    JS::ReadableStreamUnderlyingSource* source) {
  Rooted<ReadableByteStreamController*> controller(
      cx, NewBuiltinClassInstance<ReadableByteStreamController>(cx));
  if (!controller) {
    return nullptr;
  }
||||||| merged common ancestors
// Version of the ReadableByteStreamConstructor that's specialized for
// handling external, embedding-provided, underlying sources.
static MOZ_MUST_USE ReadableByteStreamController*
CreateReadableByteStreamController(JSContext* cx, Handle<ReadableStream*> stream,
                                   void* underlyingSource)
{
    Rooted<ReadableByteStreamController*> controller(cx);
    controller = NewBuiltinClassInstance<ReadableByteStreamController>(cx);
    if (!controller) {
        return nullptr;
    }
=======
  // Step 1: Assert: stream.[[readableStreamController]] is undefined.
  MOZ_ASSERT(!stream->hasController());
>>>>>>> upstream-releases

<<<<<<< HEAD
  // Step 3: Set this.[[controlledReadableStream]] to stream.
  controller->setStream(stream);
||||||| merged common ancestors
    // Step 3: Set this.[[controlledReadableStream]] to stream.
    controller->setFixedSlot(ControllerSlot_Stream, ObjectValue(*stream));
=======
  // Step 2: If autoAllocateChunkSize is not undefined, [...]
  // (It's treated as undefined.)
>>>>>>> upstream-releases

<<<<<<< HEAD
  // Step 4: Set this.[[underlyingByteSource]] to underlyingByteSource.
  controller->setExternalSource(source);
||||||| merged common ancestors
    // Step 4: Set this.[[underlyingByteSource]] to underlyingByteSource.
    controller->setFixedSlot(ControllerSlot_UnderlyingSource, PrivateValue(underlyingSource));
=======
  // Step 3: Set controller.[[controlledReadableByteStream]] to stream.
  controller->setStream(stream);
>>>>>>> upstream-releases

<<<<<<< HEAD
  // Step 5: Set this.[[pullAgain]] and this.[[pulling]] to false (implicit).
  MOZ_ASSERT(!controller->pullAgain());
  MOZ_ASSERT(!controller->pulling());
||||||| merged common ancestors
    // Step 5: Set this.[[pullAgain]], and this.[[pulling]] to false.
    controller->setFixedSlot(ControllerSlot_Flags, Int32Value(ControllerFlag_ExternalSource));
=======
  // Step 4: Set controller.[[pullAgain]] and controller.[[pulling]] to false.
  controller->setFlags(0);
  MOZ_ASSERT(!controller->pullAgain());
  MOZ_ASSERT(!controller->pulling());
>>>>>>> upstream-releases

<<<<<<< HEAD
  // Step 6: Perform ! ReadableByteStreamControllerClearPendingPullIntos(this).
  // Omitted.
||||||| merged common ancestors
    // Step 6: Perform ! ReadableByteStreamControllerClearPendingPullIntos(this).
    // Omitted.
=======
  // Step 5: Perform
  //         ! ReadableByteStreamControllerClearPendingPullIntos(controller).
  // Omitted. This step is apparently redundant; see
  // <https://github.com/whatwg/streams/issues/975>.
>>>>>>> upstream-releases

<<<<<<< HEAD
  // Step 7: Perform ! ResetQueue(this).
  controller->setQueueTotalSize(0);
||||||| merged common ancestors
    // Step 7: Perform ! ResetQueue(this).
    SetQueueSize(controller, 0);
=======
  // Step 6: Perform ! ResetQueue(this).
  controller->setQueueTotalSize(0);
>>>>>>> upstream-releases

<<<<<<< HEAD
  // Step 8: Set this.[[started]] and this.[[closeRequested]] to false.
  // Step 9: Set this.[[strategyHWM]] to
  //         ? ValidateAndNormalizeHighWaterMark(highWaterMark).
  controller->setStrategyHWM(0);
||||||| merged common ancestors
    // Step 8: Set this.[[started]] and this.[[closeRequested]] to false.
    // Step 9: Set this.[[strategyHWM]] to
    //         ? ValidateAndNormalizeHighWaterMark(highWaterMark).
    controller->setFixedSlot(ControllerSlot_StrategyHWM, Int32Value(0));
=======
  // Step 7: Set controller.[[closeRequested]] and controller.[[started]] to
  //         false (implicit).
  MOZ_ASSERT(!controller->closeRequested());
  MOZ_ASSERT(!controller->started());
>>>>>>> upstream-releases

<<<<<<< HEAD
  // Step 10: Let autoAllocateChunkSize be
  //          ? GetV(underlyingByteSource, "autoAllocateChunkSize").
  // Step 11: If autoAllocateChunkSize is not undefined,
  // Step 12: Set this.[[autoAllocateChunkSize]] to autoAllocateChunkSize.
  // Omitted.
||||||| merged common ancestors
    // Step 10: Let autoAllocateChunkSize be
    //          ? GetV(underlyingByteSource, "autoAllocateChunkSize").
    // Step 11: If autoAllocateChunkSize is not undefined,
    // Step 12: Set this.[[autoAllocateChunkSize]] to autoAllocateChunkSize.
    // Omitted.
=======
  // Step 8: Set controller.[[strategyHWM]] to
  //         ? ValidateAndNormalizeHighWaterMark(highWaterMark).
  controller->setStrategyHWM(0);
>>>>>>> upstream-releases

<<<<<<< HEAD
  // Step 13: Set this.[[pendingPullIntos]] to a new empty List.
  if (!SetNewList(cx, controller,
                  ReadableByteStreamController::Slot_PendingPullIntos)) {
    return nullptr;
  }
||||||| merged common ancestors
    // Step 13: Set this.[[pendingPullIntos]] to a new empty List.
    if (!SetNewList(cx, controller, ByteControllerSlot_PendingPullIntos)) {
        return nullptr;
    }
=======
  // Step 9: Set controller.[[pullAlgorithm]] to pullAlgorithm.
  // Step 10: Set controller.[[cancelAlgorithm]] to cancelAlgorithm.
  // (These algorithms are given by source's virtual methods.)
  controller->setExternalSource(source);
>>>>>>> upstream-releases

<<<<<<< HEAD
  // Step 14: Let controller be this (implicit).
  // Step 15: Let startResult be
  //          ? InvokeOrNoop(underlyingSource, "start", « this »).
  // Omitted.
||||||| merged common ancestors
    // Step 14: Let controller be this (implicit).
    // Step 15: Let startResult be
    //          ? InvokeOrNoop(underlyingSource, "start", « this »).
    // Omitted.
=======
  // Step 11: Set controller.[[autoAllocateChunkSize]] to
  //          autoAllocateChunkSize (implicit).
  MOZ_ASSERT(controller->autoAllocateChunkSize().isUndefined());
>>>>>>> upstream-releases

<<<<<<< HEAD
  // Step 16: Let startPromise be a promise resolved with startResult:
  RootedObject startPromise(
      cx, PromiseObject::unforgeableResolve(cx, UndefinedHandleValue));
  if (!startPromise) {
    return nullptr;
  }
||||||| merged common ancestors
    // Step 16: Let startPromise be a promise resolved with startResult:
    RootedObject startPromise(cx, PromiseObject::unforgeableResolve(cx, UndefinedHandleValue));
    if (!startPromise) {
        return nullptr;
    }
=======
  // Step 12: Set this.[[pendingPullIntos]] to a new empty List.
  if (!SetNewList(cx, controller,
                  ReadableByteStreamController::Slot_PendingPullIntos)) {
    return false;
  }
>>>>>>> upstream-releases

<<<<<<< HEAD
  RootedObject onStartFulfilled(
      cx, NewHandler(cx, ControllerStartHandler, controller));
  if (!onStartFulfilled) {
    return nullptr;
  }
||||||| merged common ancestors
    RootedObject onStartFulfilled(cx, NewHandler(cx, ControllerStartHandler, controller));
    if (!onStartFulfilled) {
        return nullptr;
    }
=======
  // Step 13: Set stream.[[readableStreamController]] to controller.
  stream->setController(controller);
>>>>>>> upstream-releases

<<<<<<< HEAD
  RootedObject onStartRejected(
      cx, NewHandler(cx, ControllerStartFailedHandler, controller));
  if (!onStartRejected) {
    return nullptr;
  }
||||||| merged common ancestors
    RootedObject onStartRejected(cx, NewHandler(cx, ControllerStartFailedHandler, controller));
    if (!onStartRejected) {
        return nullptr;
    }
=======
  // Step 14: Let startResult be the result of performing startAlgorithm.
  // (For external sources, this algorithm does nothing and returns undefined.)
  // Step 15: Let startPromise be a promise resolved with startResult.
  RootedObject startPromise(
      cx, PromiseObject::unforgeableResolve(cx, UndefinedHandleValue));
  if (!startPromise) {
    return false;
  }
>>>>>>> upstream-releases

<<<<<<< HEAD
  if (!JS::AddPromiseReactions(cx, startPromise, onStartFulfilled,
                               onStartRejected)) {
    return nullptr;
  }
||||||| merged common ancestors
    if (!JS::AddPromiseReactions(cx, startPromise, onStartFulfilled, onStartRejected)) {
        return nullptr;
    }
=======
  // Step 16: Upon fulfillment of startPromise, [...]
  // Step 17: Upon rejection of startPromise with reason r, [...]
  RootedObject onStartFulfilled(
      cx, NewHandler(cx, ControllerStartHandler, controller));
  if (!onStartFulfilled) {
    return false;
  }
  RootedObject onStartRejected(
      cx, NewHandler(cx, ControllerStartFailedHandler, controller));
  if (!onStartRejected) {
    return false;
  }
  if (!JS::AddPromiseReactions(cx, startPromise, onStartFulfilled,
                               onStartRejected)) {
    return false;
  }
>>>>>>> upstream-releases

<<<<<<< HEAD
  return controller;
||||||| merged common ancestors
    return controller;
=======
  autoClear.reset();
  return true;
>>>>>>> upstream-releases
}

static const JSPropertySpec ReadableByteStreamController_properties[] = {
    JS_PS_END};

static const JSFunctionSpec ReadableByteStreamController_methods[] = {
    JS_FS_END};

static void ReadableByteStreamControllerFinalize(FreeOp* fop, JSObject* obj) {
  ReadableByteStreamController& controller =
      obj->as<ReadableByteStreamController>();

  if (controller.getFixedSlot(ReadableStreamController::Slot_Flags)
          .isUndefined()) {
    return;
  }

  if (!controller.hasExternalSource()) {
    return;
  }

  controller.externalSource()->finalize();
}

static const ClassOps ReadableByteStreamControllerClassOps = {
    nullptr, /* addProperty */
    nullptr, /* delProperty */
    nullptr, /* enumerate */
    nullptr, /* newEnumerate */
    nullptr, /* resolve */
    nullptr, /* mayResolve */
    ReadableByteStreamControllerFinalize,
    nullptr, /* call        */
    nullptr, /* hasInstance */
    nullptr, /* construct   */
    nullptr, /* trace   */
};

CLASS_SPEC(ReadableByteStreamController, 0, SlotCount,
           ClassSpec::DontDefineConstructor, JSCLASS_BACKGROUND_FINALIZE,
           &ReadableByteStreamControllerClassOps);

// Streams spec, 3.10.5.1. [[CancelSteps]] ()
// Unified with 3.8.5.1 above.

static MOZ_MUST_USE bool ReadableByteStreamControllerHandleQueueDrain(
    JSContext* cx, Handle<ReadableStreamController*> unwrappedController);

/**
 * Streams spec, 3.10.5.2. [[PullSteps]] ( forAuthorCode )
 */
<<<<<<< HEAD
static MOZ_MUST_USE JSObject* ReadableByteStreamControllerPullSteps(
    JSContext* cx, Handle<ReadableByteStreamController*> unwrappedController) {
  // Step 1: Let stream be this.[[controlledReadableStream]].
  Rooted<ReadableStream*> unwrappedStream(cx, unwrappedController->stream());
||||||| merged common ancestors
static MOZ_MUST_USE JSObject*
ReadableByteStreamControllerPullSteps(JSContext* cx, Handle<ReadableStreamController*> controller)
{
    // Step 1: Let stream be this.[[controlledReadableStream]].
    Rooted<ReadableStream*> stream(cx, StreamFromController(controller));
=======
static MOZ_MUST_USE JSObject* ReadableByteStreamControllerPullSteps(
    JSContext* cx, Handle<ReadableByteStreamController*> unwrappedController) {
  // Step 1: Let stream be this.[[controlledReadableByteStream]].
  Rooted<ReadableStream*> unwrappedStream(cx, unwrappedController->stream());
>>>>>>> upstream-releases

  // Step 2: Assert: ! ReadableStreamHasDefaultReader(stream) is true.
#ifdef DEBUG
  bool result;
  if (!ReadableStreamHasDefaultReader(cx, unwrappedStream, &result)) {
    return nullptr;
  }
  MOZ_ASSERT(result);
#endif

<<<<<<< HEAD
  RootedValue val(cx);
  // Step 3: If this.[[queueTotalSize]] > 0,
  double queueTotalSize = unwrappedController->queueTotalSize();
  if (queueTotalSize > 0) {
    // Step 3.a: Assert: ! ReadableStreamGetNumReadRequests(_stream_) is 0.
    MOZ_ASSERT(ReadableStreamGetNumReadRequests(unwrappedStream) == 0);

    RootedObject view(cx);
||||||| merged common ancestors
    RootedValue val(cx);
    // Step 3: If this.[[queueTotalSize]] > 0,
    double queueTotalSize = QueueSize(controller);
    if (queueTotalSize > 0) {
        // Step 3.a: Assert: ! ReadableStreamGetNumReadRequests(_stream_) is 0.
        MOZ_ASSERT(ReadableStreamGetNumReadRequests(stream) == 0);
=======
  RootedValue val(cx);
  // Step 3: If this.[[queueTotalSize]] > 0,
  double queueTotalSize = unwrappedController->queueTotalSize();
  if (queueTotalSize > 0) {
    // Step 3.a: Assert: ! ReadableStreamGetNumReadRequests(_stream_) is 0.
    MOZ_ASSERT(ReadableStreamGetNumReadRequests(unwrappedStream) == 0);
>>>>>>> upstream-releases

<<<<<<< HEAD
    MOZ_RELEASE_ASSERT(unwrappedStream->mode() ==
                       JS::ReadableStreamMode::ExternalSource);
#if 0   // disable user-defined byte streams
        if (unwrappedStream->mode() == JS::ReadableStreamMode::ExternalSource)
#endif  // user-defined byte streams
    {
      JS::ReadableStreamUnderlyingSource* source =
          unwrappedController->externalSource();
||||||| merged common ancestors
        RootedObject view(cx);
=======
    RootedObject view(cx);
>>>>>>> upstream-releases

<<<<<<< HEAD
      view = JS_NewUint8Array(cx, queueTotalSize);
      if (!view) {
        return nullptr;
      }
||||||| merged common ancestors
        if (stream->mode() == JS::ReadableStreamMode::ExternalSource) {
            val = controller->getFixedSlot(ControllerSlot_UnderlyingSource);
            void* underlyingSource = val.toPrivate();
=======
    MOZ_RELEASE_ASSERT(unwrappedStream->mode() ==
                       JS::ReadableStreamMode::ExternalSource);
#if 0   // disable user-defined byte streams
        if (unwrappedStream->mode() == JS::ReadableStreamMode::ExternalSource)
#endif  // user-defined byte streams
    {
      JS::ReadableStreamUnderlyingSource* source =
          unwrappedController->externalSource();
>>>>>>> upstream-releases

<<<<<<< HEAD
      size_t bytesWritten;
      {
        AutoRealm ar(cx, unwrappedStream);
        JS::AutoSuppressGCAnalysis suppressGC(cx);
        JS::AutoCheckCannotGC noGC;
        bool dummy;
        void* buffer = JS_GetArrayBufferViewData(view, &dummy, noGC);
||||||| merged common ancestors
            view = JS_NewUint8Array(cx, queueTotalSize);
            if (!view) {
                return nullptr;
            }
=======
      view = JS_NewUint8Array(cx, queueTotalSize);
      if (!view) {
        return nullptr;
      }
>>>>>>> upstream-releases

<<<<<<< HEAD
        source->writeIntoReadRequestBuffer(cx, unwrappedStream, buffer,
                                           queueTotalSize, &bytesWritten);
      }

      queueTotalSize = queueTotalSize - bytesWritten;
    }
||||||| merged common ancestors
            size_t bytesWritten;
            {
                JS::AutoSuppressGCAnalysis suppressGC(cx);
                JS::AutoCheckCannotGC noGC;
                bool dummy;
                void* buffer = JS_GetArrayBufferViewData(view, &dummy, noGC);
                auto cb = cx->runtime()->readableStreamWriteIntoReadRequestCallback;
                MOZ_ASSERT(cb);
                // TODO: use bytesWritten to correctly update the request's state.
                // TODO: make this compartment-safe.
                cb(cx, stream, underlyingSource, stream->embeddingFlags(), buffer,
                   queueTotalSize, &bytesWritten);
            }
=======
      size_t bytesWritten;
      {
        AutoRealm ar(cx, unwrappedStream);
        JS::AutoSuppressGCAnalysis suppressGC(cx);
        JS::AutoCheckCannotGC noGC;
        bool dummy;
        void* buffer = JS_GetArrayBufferViewData(view, &dummy, noGC);

        source->writeIntoReadRequestBuffer(cx, unwrappedStream, buffer,
                                           queueTotalSize, &bytesWritten);
      }
>>>>>>> upstream-releases

<<<<<<< HEAD
#if 0   // disable user-defined byte streams
        else {
||||||| merged common ancestors
            queueTotalSize = queueTotalSize - bytesWritten;
        } else {
=======
      queueTotalSize = queueTotalSize - bytesWritten;
    }

#if 0   // disable user-defined byte streams
        else {
>>>>>>> upstream-releases
            // Step 3.b: Let entry be the first element of this.[[queue]].
            // Step 3.c: Remove entry from this.[[queue]], shifting all other
            //           elements downward (so that the second becomes the
            //           first, and so on).
            Rooted<ListObject*> unwrappedQueue(cx, unwrappedController->queue());
            Rooted<ByteStreamChunk*> unwrappedEntry(cx,
                UnwrapAndDowncastObject<ByteStreamChunk>(
                    cx, &unwrappedQueue->popFirstAs<JSObject>(cx)));
            if (!unwrappedEntry) {
                return nullptr;
            }

            queueTotalSize = queueTotalSize - unwrappedEntry->byteLength();

            // Step 3.f: Let view be ! Construct(%Uint8Array%,
            //                                   « entry.[[buffer]],
            //                                     entry.[[byteOffset]],
            //                                     entry.[[byteLength]] »).
            // (reordered)
            RootedObject buffer(cx, unwrappedEntry->buffer());
            if (!cx->compartment()->wrap(cx, &buffer)) {
                return nullptr;
            }

            uint32_t byteOffset = unwrappedEntry->byteOffset();
            view = JS_NewUint8ArrayWithBuffer(cx, buffer, byteOffset, unwrappedEntry->byteLength());
            if (!view) {
                return nullptr;
            }
        }
<<<<<<< HEAD
#endif  // user-defined byte streams

    // Step 3.d: Set this.[[queueTotalSize]] to
    //           this.[[queueTotalSize]] − entry.[[byteLength]].
    // (reordered)
    unwrappedController->setQueueTotalSize(queueTotalSize);

    // Step 3.e: Perform ! ReadableByteStreamControllerHandleQueueDrain(this).
    // (reordered)
    if (!ReadableByteStreamControllerHandleQueueDrain(cx,
                                                      unwrappedController)) {
      return nullptr;
    }

    // Step 3.g: Return a promise resolved with
    //           ! ReadableStreamCreateReadResult(view, false, forAuthorCode).
    val.setObject(*view);
    ReadableStreamReader* unwrappedReader =
        UnwrapReaderFromStream(cx, unwrappedStream);
    if (!unwrappedReader) {
      return nullptr;
    }
    RootedObject readResult(
        cx, ReadableStreamCreateReadResult(cx, val, false,
                                           unwrappedReader->forAuthorCode()));
    if (!readResult) {
      return nullptr;
    }
    val.setObject(*readResult);

    return PromiseObject::unforgeableResolve(cx, val);
  }

  // Step 4: Let autoAllocateChunkSize be this.[[autoAllocateChunkSize]].
  val = unwrappedController->autoAllocateChunkSize();

  // Step 5: If autoAllocateChunkSize is not undefined,
  if (!val.isUndefined()) {
    double autoAllocateChunkSize = val.toNumber();

    // Step 5.a: Let buffer be
    //           Construct(%ArrayBuffer%, « autoAllocateChunkSize »).
    JSObject* bufferObj = JS_NewArrayBuffer(cx, autoAllocateChunkSize);

    // Step 5.b: If buffer is an abrupt completion,
    //           return a promise rejected with buffer.[[Value]].
    if (!bufferObj) {
      return PromiseRejectedWithPendingError(cx);
    }

    RootedArrayBufferObject buffer(cx, &bufferObj->as<ArrayBufferObject>());

    // Step 5.c: Let pullIntoDescriptor be
    //           Record {[[buffer]]: buffer.[[Value]],
    //                   [[byteOffset]]: 0,
    //                   [[byteLength]]: autoAllocateChunkSize,
    //                   [[bytesFilled]]: 0, [[elementSize]]: 1,
    //                   [[ctor]]: %Uint8Array%,
    //                   [[readerType]]: `"default"`}.
    RootedObject pullIntoDescriptor(
        cx, PullIntoDescriptor::create(cx, buffer, 0, autoAllocateChunkSize, 0,
                                       1, nullptr, ReaderType_Default));
    if (!pullIntoDescriptor) {
      return PromiseRejectedWithPendingError(cx);
    }

    // Step 5.d: Append pullIntoDescriptor as the last element of
    //           this.[[pendingPullIntos]].
    if (!AppendToListAtSlot(cx, unwrappedController,
                            ReadableByteStreamController::Slot_PendingPullIntos,
                            pullIntoDescriptor)) {
      return nullptr;
    }
  }

  // Step 6: Let promise be ! ReadableStreamAddReadRequest(stream,
  // forAuthorCode).
  RootedObject promise(
      cx, ReadableStreamAddReadOrReadIntoRequest(cx, unwrappedStream));
  if (!promise) {
    return nullptr;
  }
||||||| merged common ancestors
=======
#endif  // user-defined byte streams

    // Step 3.d: Set this.[[queueTotalSize]] to
    //           this.[[queueTotalSize]] − entry.[[byteLength]].
    // (reordered)
    unwrappedController->setQueueTotalSize(queueTotalSize);

    // Step 3.e: Perform ! ReadableByteStreamControllerHandleQueueDrain(this).
    // (reordered)
    if (!ReadableByteStreamControllerHandleQueueDrain(cx,
                                                      unwrappedController)) {
      return nullptr;
    }

    // Step 3.g: Return a promise resolved with
    //           ! ReadableStreamCreateReadResult(view, false, forAuthorCode).
    val.setObject(*view);
    ReadableStreamReader* unwrappedReader =
        UnwrapReaderFromStream(cx, unwrappedStream);
    if (!unwrappedReader) {
      return nullptr;
    }
    RootedObject readResult(
        cx, ReadableStreamCreateReadResult(cx, val, false,
                                           unwrappedReader->forAuthorCode()));
    if (!readResult) {
      return nullptr;
    }
    val.setObject(*readResult);

    return PromiseObject::unforgeableResolve(cx, val);
  }

  // Step 4: Let autoAllocateChunkSize be this.[[autoAllocateChunkSize]].
  val = unwrappedController->autoAllocateChunkSize();

  // Step 5: If autoAllocateChunkSize is not undefined,
  if (!val.isUndefined()) {
    double autoAllocateChunkSize = val.toNumber();

    // Step 5.a: Let buffer be
    //           Construct(%ArrayBuffer%, « autoAllocateChunkSize »).
    JSObject* bufferObj = JS::NewArrayBuffer(cx, autoAllocateChunkSize);

    // Step 5.b: If buffer is an abrupt completion,
    //           return a promise rejected with buffer.[[Value]].
    if (!bufferObj) {
      return PromiseRejectedWithPendingError(cx);
    }

    RootedArrayBufferObject buffer(cx, &bufferObj->as<ArrayBufferObject>());

    // Step 5.c: Let pullIntoDescriptor be
    //           Record {[[buffer]]: buffer.[[Value]],
    //                   [[byteOffset]]: 0,
    //                   [[byteLength]]: autoAllocateChunkSize,
    //                   [[bytesFilled]]: 0,
    //                   [[elementSize]]: 1,
    //                   [[ctor]]: %Uint8Array%,
    //                   [[readerType]]: `"default"`}.
    RootedObject pullIntoDescriptor(
        cx, PullIntoDescriptor::create(cx, buffer, 0, autoAllocateChunkSize, 0,
                                       1, nullptr, ReaderType_Default));
    if (!pullIntoDescriptor) {
      return PromiseRejectedWithPendingError(cx);
    }

    // Step 5.d: Append pullIntoDescriptor as the last element of
    //           this.[[pendingPullIntos]].
    if (!AppendToListAtSlot(cx, unwrappedController,
                            ReadableByteStreamController::Slot_PendingPullIntos,
                            pullIntoDescriptor)) {
      return nullptr;
    }
  }

  // Step 6: Let promise be ! ReadableStreamAddReadRequest(stream,
  //                                                       forAuthorCode).
  RootedObject promise(
      cx, ReadableStreamAddReadOrReadIntoRequest(cx, unwrappedStream));
  if (!promise) {
    return nullptr;
  }
>>>>>>> upstream-releases

  // Step 7: Perform ! ReadableByteStreamControllerCallPullIfNeeded(this).
  if (!ReadableStreamControllerCallPullIfNeeded(cx, unwrappedController)) {
    return nullptr;
  }

  // Step 8: Return promise.
  return promise;
}

/**
 * Unified implementation of ReadableStream controllers' [[PullSteps]] internal
 * methods.
 * Streams spec, 3.8.5.2. [[PullSteps]] ( forAuthorCode )
 * and
 * Streams spec, 3.10.5.2. [[PullSteps]] ( forAuthorCode )
 */
static MOZ_MUST_USE JSObject* ReadableStreamControllerPullSteps(
    JSContext* cx, Handle<ReadableStreamController*> unwrappedController) {
  if (unwrappedController->is<ReadableStreamDefaultController>()) {
    Rooted<ReadableStreamDefaultController*> unwrappedDefaultController(
        cx, &unwrappedController->as<ReadableStreamDefaultController>());
    return ReadableStreamDefaultControllerPullSteps(cx,
                                                    unwrappedDefaultController);
  }

  Rooted<ReadableByteStreamController*> unwrappedByteController(
      cx, &unwrappedController->as<ReadableByteStreamController>());
  return ReadableByteStreamControllerPullSteps(cx, unwrappedByteController);
}

/*** 3.12. Readable stream BYOB controller abstract operations **************/

// Streams spec, 3.12.1. IsReadableStreamBYOBRequest ( x )
// Implemented via is<ReadableStreamBYOBRequest>()

// Streams spec, 3.12.2. IsReadableByteStreamController ( x )
// Implemented via is<ReadableByteStreamController>()

// Streams spec, 3.12.3.
//      ReadableByteStreamControllerCallPullIfNeeded ( controller )
// Unified with 3.9.2 above.

static MOZ_MUST_USE bool ReadableByteStreamControllerInvalidateBYOBRequest(
    JSContext* cx, Handle<ReadableByteStreamController*> unwrappedController);

/**
 * Streams spec, 3.12.5.
 *      ReadableByteStreamControllerClearPendingPullIntos ( controller )
 */
<<<<<<< HEAD
static MOZ_MUST_USE bool ReadableByteStreamControllerClearPendingPullIntos(
    JSContext* cx, Handle<ReadableByteStreamController*> unwrappedController) {
  // Step 1: Perform
  //         ! ReadableByteStreamControllerInvalidateBYOBRequest(controller).
  if (!ReadableByteStreamControllerInvalidateBYOBRequest(cx,
                                                         unwrappedController)) {
    return false;
  }
||||||| merged common ancestors
static MOZ_MUST_USE bool
ReadableByteStreamControllerClearPendingPullIntos(JSContext* cx,
                                                  Handle<ReadableStreamController*> controller)
{
    MOZ_ASSERT(controller->is<ReadableByteStreamController>());

    // Step 1: Perform ! ReadableByteStreamControllerInvalidateBYOBRequest(controller).
    if (!ReadableByteStreamControllerInvalidateBYOBRequest(cx, controller)) {
        return false;
    }
=======
static MOZ_MUST_USE bool ReadableByteStreamControllerClearPendingPullIntos(
    JSContext* cx, Handle<ReadableByteStreamController*> unwrappedController) {
  // Step 1: Perform
  //         ! ReadableByteStreamControllerInvalidateBYOBRequest(controller).
  if (!ReadableByteStreamControllerInvalidateBYOBRequest(cx,
                                                         unwrappedController)) {
    return false;
  }

  // Step 2: Set controller.[[pendingPullIntos]] to a new empty List.
  return SetNewList(cx, unwrappedController,
                    ReadableByteStreamController::Slot_PendingPullIntos);
}

/**
 * Streams spec, 3.12.6. ReadableByteStreamControllerClose ( controller )
 */
static MOZ_MUST_USE bool ReadableByteStreamControllerClose(
    JSContext* cx, Handle<ReadableByteStreamController*> unwrappedController) {
  // Step 1: Let stream be controller.[[controlledReadableByteStream]].
  Rooted<ReadableStream*> unwrappedStream(cx, unwrappedController->stream());

  // Step 2: Assert: controller.[[closeRequested]] is false.
  MOZ_ASSERT(!unwrappedController->closeRequested());

  // Step 3: Assert: stream.[[state]] is "readable".
  MOZ_ASSERT(unwrappedStream->readable());

  // Step 4: If controller.[[queueTotalSize]] > 0,
  if (unwrappedController->queueTotalSize() > 0) {
    // Step a: Set controller.[[closeRequested]] to true.
    unwrappedController->setCloseRequested();

    // Step b: Return.
    return true;
  }

  // Step 5: If controller.[[pendingPullIntos]] is not empty,
  Rooted<ListObject*> unwrappedPendingPullIntos(
      cx, unwrappedController->pendingPullIntos());
  if (unwrappedPendingPullIntos->length() != 0) {
    // Step a: Let firstPendingPullInto be the first element of
    //         controller.[[pendingPullIntos]].
    Rooted<PullIntoDescriptor*> unwrappedFirstPendingPullInto(
        cx, UnwrapAndDowncastObject<PullIntoDescriptor>(
                cx, &unwrappedPendingPullIntos->get(0).toObject()));
    if (!unwrappedFirstPendingPullInto) {
      return false;
    }

    // Step b: If firstPendingPullInto.[[bytesFilled]] > 0,
    if (unwrappedFirstPendingPullInto->bytesFilled() > 0) {
      // Step i: Let e be a new TypeError exception.
      JS_ReportErrorNumberASCII(
          cx, GetErrorMessage, nullptr,
          JSMSG_READABLEBYTESTREAMCONTROLLER_CLOSE_PENDING_PULL);
      RootedValue e(cx);
      RootedSavedFrame stack(cx);
      if (!cx->isExceptionPending() ||
          !GetAndClearExceptionAndStack(cx, &e, &stack)) {
        // Uncatchable error. Die immediately without erroring the
        // stream.
        return false;
      }

      // Step ii: Perform ! ReadableByteStreamControllerError(controller, e).
      if (!ReadableStreamControllerError(cx, unwrappedController, e)) {
        return false;
      }

      // Step iii: Throw e.
      cx->setPendingException(e, stack);
      return false;
    }
  }
>>>>>>> upstream-releases

<<<<<<< HEAD
  // Step 2: Set controller.[[pendingPullIntos]] to a new empty List.
  return SetNewList(cx, unwrappedController,
                    ReadableByteStreamController::Slot_PendingPullIntos);
||||||| merged common ancestors
    // Step 2: Set controller.[[pendingPullIntos]] to a new empty List.
    return SetNewList(cx, controller, ByteControllerSlot_PendingPullIntos);
=======
  // Step 6: Perform ! ReadableByteStreamControllerClearAlgorithms(controller).
  ReadableStreamControllerClearAlgorithms(unwrappedController);

  // Step 7: Perform ! ReadableStreamClose(stream).
  return ReadableStreamCloseInternal(cx, unwrappedStream);
>>>>>>> upstream-releases
}

// Streams spec, 3.12.11. ReadableByteStreamControllerError ( controller, e )
// Unified with 3.9.7 above.

// Streams spec 3.12.14.
//      ReadableByteStreamControllerGetDesiredSize ( controller )
// Unified with 3.9.8 above.

/**
<<<<<<< HEAD
 * Streams spec, 3.12.6. ReadableByteStreamControllerClose ( controller )
||||||| merged common ancestors
 * Streams spec, 3.12.5. ReadableByteStreamControllerClose ( controller )
 *
 * Note: can operate on unwrapped ReadableByteStreamController instances from
 * another compartment.
=======
 * Streams spec, 3.12.15.
 *      ReadableByteStreamControllerHandleQueueDrain ( controller )
>>>>>>> upstream-releases
 */
<<<<<<< HEAD
static MOZ_MUST_USE bool ReadableByteStreamControllerClose(
    JSContext* cx, Handle<ReadableByteStreamController*> unwrappedController) {
  // Step 1: Let stream be controller.[[controlledReadableStream]].
  Rooted<ReadableStream*> unwrappedStream(cx, unwrappedController->stream());
||||||| merged common ancestors
static MOZ_MUST_USE bool
ReadableByteStreamControllerClose(JSContext* cx, Handle<ReadableByteStreamController*> controller)
{
    // Step 1: Let stream be controller.[[controlledReadableStream]].
    Rooted<ReadableStream*> stream(cx, StreamFromController(controller));
=======
static MOZ_MUST_USE bool ReadableByteStreamControllerHandleQueueDrain(
    JSContext* cx, Handle<ReadableStreamController*> unwrappedController) {
  MOZ_ASSERT(unwrappedController->is<ReadableByteStreamController>());
>>>>>>> upstream-releases

<<<<<<< HEAD
  // Step 2: Assert: controller.[[closeRequested]] is false.
  MOZ_ASSERT(!unwrappedController->closeRequested());
||||||| merged common ancestors
    // Step 2: Assert: controller.[[closeRequested]] is false.
    MOZ_ASSERT(!(ControllerFlags(controller) & ControllerFlag_CloseRequested));
=======
  // Step 1: Assert: controller.[[controlledReadableStream]].[[state]]
  //                 is "readable".
  Rooted<ReadableStream*> unwrappedStream(cx, unwrappedController->stream());
  MOZ_ASSERT(unwrappedStream->readable());
>>>>>>> upstream-releases

<<<<<<< HEAD
  // Step 3: Assert: stream.[[state]] is "readable".
  MOZ_ASSERT(unwrappedStream->readable());
||||||| merged common ancestors
    // Step 3: Assert: stream.[[state]] is "readable".
    MOZ_ASSERT(stream->readable());
=======
  // Step 2: If controller.[[queueTotalSize]] is 0 and
  //         controller.[[closeRequested]] is true,
  if (unwrappedController->queueTotalSize() == 0 &&
      unwrappedController->closeRequested()) {
    // Step a: Perform
    //         ! ReadableByteStreamControllerClearAlgorithms(controller).
    ReadableStreamControllerClearAlgorithms(unwrappedController);
>>>>>>> upstream-releases

<<<<<<< HEAD
  // Step 4: If controller.[[queueTotalSize]] > 0,
  if (unwrappedController->queueTotalSize() > 0) {
    // Step a: Set controller.[[closeRequested]] to true.
    unwrappedController->setCloseRequested();
||||||| merged common ancestors
    // Step 4: If controller.[[queueTotalSize]] > 0,
    double queueTotalSize = QueueSize(controller);
    if (queueTotalSize > 0) {
        // Step a: Set controller.[[closeRequested]] to true.
        AddControllerFlags(controller, ControllerFlag_CloseRequested);
=======
    // Step b: Perform
    //         ! ReadableStreamClose(controller.[[controlledReadableStream]]).
    return ReadableStreamCloseInternal(cx, unwrappedStream);
  }
>>>>>>> upstream-releases

<<<<<<< HEAD
    // Step b: Return.
    return true;
  }

  // Step 5: If controller.[[pendingPullIntos]] is not empty,
  Rooted<ListObject*> unwrappedPendingPullIntos(
      cx, unwrappedController->pendingPullIntos());
  if (unwrappedPendingPullIntos->length() != 0) {
    // Step a: Let firstPendingPullInto be the first element of
    //         controller.[[pendingPullIntos]].
    Rooted<PullIntoDescriptor*> unwrappedFirstPendingPullInto(
        cx, UnwrapAndDowncastObject<PullIntoDescriptor>(
                cx, &unwrappedPendingPullIntos->get(0).toObject()));
    if (!unwrappedFirstPendingPullInto) {
      return false;
    }
||||||| merged common ancestors
        // Step b: Return
        return true;
    }
=======
  // Step 3: Otherwise,
  // Step a: Perform ! ReadableByteStreamControllerCallPullIfNeeded(controller).
  return ReadableStreamControllerCallPullIfNeeded(cx, unwrappedController);
}
>>>>>>> upstream-releases

<<<<<<< HEAD
    // Step b: If firstPendingPullInto.[[bytesFilled]] > 0,
    if (unwrappedFirstPendingPullInto->bytesFilled() > 0) {
      // Step i: Let e be a new TypeError exception.
      JS_ReportErrorNumberASCII(
          cx, GetErrorMessage, nullptr,
          JSMSG_READABLEBYTESTREAMCONTROLLER_CLOSE_PENDING_PULL);
      RootedValue e(cx);
      if (!cx->isExceptionPending() || !GetAndClearException(cx, &e)) {
        // Uncatchable error. Die immediately without erroring the
        // stream.
        return false;
      }
||||||| merged common ancestors
    // Step 5: If controller.[[pendingPullIntos]] is not empty,
    RootedValue val(cx, controller->getFixedSlot(ByteControllerSlot_PendingPullIntos));
    RootedNativeObject pendingPullIntos(cx, &val.toObject().as<NativeObject>());
    if (pendingPullIntos->getDenseInitializedLength() != 0) {
        // Step a: Let firstPendingPullInto be the first element of
        //         controller.[[pendingPullIntos]].
        Rooted<PullIntoDescriptor*> firstPendingPullInto(cx);
        firstPendingPullInto = ToUnwrapped<PullIntoDescriptor>(cx,
                                                               PeekList<JSObject>(pendingPullIntos));
        if (!firstPendingPullInto) {
            return false;
        }

        // Step b: If firstPendingPullInto.[[bytesFilled]] > 0,
        if (firstPendingPullInto->bytesFilled() > 0) {
            // Step i: Let e be a new TypeError exception.
            JS_ReportErrorNumberASCII(cx, GetErrorMessage, nullptr,
                                      JSMSG_READABLEBYTESTREAMCONTROLLER_CLOSE_PENDING_PULL);
            RootedValue e(cx);
            // Not much we can do about uncatchable exceptions, just bail.
            if (!cx->getPendingException(&e)) {
                return false;
            }
=======
enum BYOBRequestSlots {
  BYOBRequestSlot_Controller,
  BYOBRequestSlot_View,
  BYOBRequestSlotCount
};

/**
 * Streams spec 3.12.16.
 *      ReadableByteStreamControllerInvalidateBYOBRequest ( controller )
 */
static MOZ_MUST_USE bool ReadableByteStreamControllerInvalidateBYOBRequest(
    JSContext* cx, Handle<ReadableByteStreamController*> unwrappedController) {
  // Step 1: If controller.[[byobRequest]] is undefined, return.
  RootedValue unwrappedBYOBRequestVal(cx, unwrappedController->byobRequest());
  if (unwrappedBYOBRequestVal.isUndefined()) {
    return true;
  }
>>>>>>> upstream-releases

<<<<<<< HEAD
      // Step ii: Perform ! ReadableByteStreamControllerError(controller, e).
      if (!ReadableStreamControllerError(cx, unwrappedController, e)) {
        return false;
      }
||||||| merged common ancestors
            // Step ii: Perform ! ReadableByteStreamControllerError(controller, e).
            if (!ReadableStreamControllerError(cx, controller, e)) {
                return false;
            }
=======
  RootedNativeObject unwrappedBYOBRequest(
      cx, UnwrapAndDowncastValue<NativeObject>(cx, unwrappedBYOBRequestVal));
  if (!unwrappedBYOBRequest) {
    return false;
  }
>>>>>>> upstream-releases

<<<<<<< HEAD
      // Step iii: Throw e.
      cx->setPendingException(e);
      return false;
    }
  }
||||||| merged common ancestors
            // Step iii: Throw e.
            return false;
        }
    }
=======
  // Step 2: Set controller.[[byobRequest]]
  //                       .[[associatedReadableByteStreamController]]
  //         to undefined.
  unwrappedBYOBRequest->setFixedSlot(BYOBRequestSlot_Controller,
                                     UndefinedValue());

  // Step 3: Set controller.[[byobRequest]].[[view]] to undefined.
  unwrappedBYOBRequest->setFixedSlot(BYOBRequestSlot_View, UndefinedValue());
>>>>>>> upstream-releases

<<<<<<< HEAD
  // Step 6: Perform ! ReadableStreamClose(stream).
  return ReadableStreamCloseInternal(cx, unwrappedStream);
||||||| merged common ancestors
    // Step 6: Perform ! ReadableStreamClose(stream).
    return ReadableStreamCloseInternal(cx, stream);
=======
  // Step 4: Set controller.[[byobRequest]] to undefined.
  unwrappedController->clearBYOBRequest();

  return true;
>>>>>>> upstream-releases
}

<<<<<<< HEAD
// Streams spec, 3.12.11. ReadableByteStreamControllerError ( controller, e )
// Unified with 3.9.7 above.
||||||| merged common ancestors
// Streams spec, 3.12.10. ReadableByteStreamControllerError ( controller, e )
// Unified with 3.9.6 above.
=======
// Streams spec, 3.12.25.
//      ReadableByteStreamControllerShouldCallPull ( controller )
// Unified with 3.9.3 above.
>>>>>>> upstream-releases

<<<<<<< HEAD
// Streams spec 3.12.14.
//      ReadableByteStreamControllerGetDesiredSize ( controller )
// Unified with 3.9.8 above.
||||||| merged common ancestors
// Streams spec 3.12.13. ReadableByteStreamControllerGetDesiredSize ( controller )
// Unified with 3.9.8 above.
=======
/*** 6.1. Queuing strategies ************************************************/
>>>>>>> upstream-releases

/**
<<<<<<< HEAD
 * Streams spec, 3.12.15.
 *      ReadableByteStreamControllerHandleQueueDrain ( controller )
||||||| merged common ancestors
 * Streams spec, 3.12.14. ReadableByteStreamControllerHandleQueueDrain ( controller )
 *
 * Note: can operate on unwrapped instances from other compartments for
 * |controller|.
=======
 * ECMA-262 7.3.4 CreateDataProperty(O, P, V)
>>>>>>> upstream-releases
 */
<<<<<<< HEAD
static MOZ_MUST_USE bool ReadableByteStreamControllerHandleQueueDrain(
    JSContext* cx, Handle<ReadableStreamController*> unwrappedController) {
  MOZ_ASSERT(unwrappedController->is<ReadableByteStreamController>());

  // Step 1: Assert: controller.[[controlledReadableStream]].[[state]]
  //                 is "readable".
  Rooted<ReadableStream*> unwrappedStream(cx, unwrappedController->stream());
  MOZ_ASSERT(unwrappedStream->readable());
||||||| merged common ancestors
static MOZ_MUST_USE bool
ReadableByteStreamControllerHandleQueueDrain(JSContext* cx,
                                             Handle<ReadableStreamController*> controller)
{
    MOZ_ASSERT(controller->is<ReadableByteStreamController>());

    // Step 1: Assert: controller.[[controlledReadableStream]].[[state]] is "readable".
    Rooted<ReadableStream*> stream(cx, StreamFromController(controller));
    MOZ_ASSERT(stream->readable());

    // Step 2: If controller.[[queueTotalSize]] is 0 and
    //         controller.[[closeRequested]] is true,
    double totalSize = QueueSize(controller);
    bool closeRequested = ControllerFlags(controller) & ControllerFlag_CloseRequested;
    if (totalSize == 0 && closeRequested) {
      // Step a: Perform ! ReadableStreamClose(controller.[[controlledReadableStream]]).
      return ReadableStreamCloseInternal(cx, stream);
    }
=======
static MOZ_MUST_USE bool CreateDataProperty(JSContext* cx, HandleObject obj,
                                            HandlePropertyName key,
                                            HandleValue value,
                                            ObjectOpResult& result) {
  RootedId id(cx, NameToId(key));
  Rooted<PropertyDescriptor> desc(cx);
  desc.setDataDescriptor(value, JSPROP_ENUMERATE);
  return DefineProperty(cx, obj, id, desc, result);
}

// Streams spec, 6.1.2.2. new ByteLengthQueuingStrategy({ highWaterMark })
bool js::ByteLengthQueuingStrategy::constructor(JSContext* cx, unsigned argc,
                                                Value* vp) {
  CallArgs args = CallArgsFromVp(argc, vp);

  if (!ThrowIfNotConstructing(cx, args, "ByteLengthQueuingStrategy")) {
    return false;
  }

  // Implicit in the spec: Create the new strategy object.
  RootedObject proto(cx);
  if (!GetPrototypeFromBuiltinConstructor(
          cx, args, JSProto_ByteLengthQueuingStrategy, &proto)) {
    return false;
  }
  RootedObject strategy(
      cx, NewObjectWithClassProto<ByteLengthQueuingStrategy>(cx, proto));
  if (!strategy) {
    return false;
  }

  // Implicit in the spec: Argument destructuring.
  RootedObject argObj(cx, ToObject(cx, args.get(0)));
  if (!argObj) {
    return false;
  }
  RootedValue highWaterMark(cx);
  if (!GetProperty(cx, argObj, argObj, cx->names().highWaterMark,
                   &highWaterMark)) {
    return false;
  }

  // Step 1: Perform ! CreateDataProperty(this, "highWaterMark",
  //                                      highWaterMark).
  ObjectOpResult ignored;
  if (!CreateDataProperty(cx, strategy, cx->names().highWaterMark,
                          highWaterMark, ignored)) {
    return false;
  }

  args.rval().setObject(*strategy);
  return true;
}

// Streams spec 6.1.2.3.1. size ( chunk )
bool ByteLengthQueuingStrategy_size(JSContext* cx, unsigned argc, Value* vp) {
  CallArgs args = CallArgsFromVp(argc, vp);

  // Step 1: Return ? GetV(chunk, "byteLength").
  return GetProperty(cx, args.get(0), cx->names().byteLength, args.rval());
}

static const JSPropertySpec ByteLengthQueuingStrategy_properties[] = {
    JS_PS_END};

static const JSFunctionSpec ByteLengthQueuingStrategy_methods[] = {
    JS_FN("size", ByteLengthQueuingStrategy_size, 1, 0), JS_FS_END};

CLASS_SPEC(ByteLengthQueuingStrategy, 1, 0, 0, 0, JS_NULL_CLASS_OPS);

// Streams spec, 6.1.3.2. new CountQueuingStrategy({ highWaterMark })
bool js::CountQueuingStrategy::constructor(JSContext* cx, unsigned argc,
                                           Value* vp) {
  CallArgs args = CallArgsFromVp(argc, vp);

  if (!ThrowIfNotConstructing(cx, args, "CountQueuingStrategy")) {
    return false;
  }

  // Implicit in the spec: Create the new strategy object.
  RootedObject proto(cx);
  if (!GetPrototypeFromBuiltinConstructor(
          cx, args, JSProto_CountQueuingStrategy, &proto)) {
    return false;
  }
  Rooted<CountQueuingStrategy*> strategy(
      cx, NewObjectWithClassProto<CountQueuingStrategy>(cx, proto));
  if (!strategy) {
    return false;
  }

  // Implicit in the spec: Argument destructuring.
  RootedObject argObj(cx, ToObject(cx, args.get(0)));
  if (!argObj) {
    return false;
  }
  RootedValue highWaterMark(cx);
  if (!GetProperty(cx, argObj, argObj, cx->names().highWaterMark,
                   &highWaterMark)) {
    return false;
  }

  // Step 1: Perform ! CreateDataProperty(this, "highWaterMark", highWaterMark).
  ObjectOpResult ignored;
  if (!CreateDataProperty(cx, strategy, cx->names().highWaterMark,
                          highWaterMark, ignored)) {
    return false;
  }
>>>>>>> upstream-releases

<<<<<<< HEAD
  // Step 2: If controller.[[queueTotalSize]] is 0 and
  //         controller.[[closeRequested]] is true,
  if (unwrappedController->queueTotalSize() == 0 &&
      unwrappedController->closeRequested()) {
    // Step a: Perform
    //         ! ReadableStreamClose(controller.[[controlledReadableStream]]).
    return ReadableStreamCloseInternal(cx, unwrappedStream);
  }

  // Step 3: Otherwise,
  // Step a: Perform ! ReadableByteStreamControllerCallPullIfNeeded(controller).
  return ReadableStreamControllerCallPullIfNeeded(cx, unwrappedController);
||||||| merged common ancestors
    // Step 3: Otherwise,
    // Step a: Perform ! ReadableByteStreamControllerCallPullIfNeeded(controller).
    return ReadableStreamControllerCallPullIfNeeded(cx, controller);
=======
  args.rval().setObject(*strategy);
  return true;
>>>>>>> upstream-releases
}

<<<<<<< HEAD
enum BYOBRequestSlots {
  BYOBRequestSlot_Controller,
  BYOBRequestSlot_View,
  BYOBRequestSlotCount
};

/**
 * Streams spec 3.12.16.
 *      ReadableByteStreamControllerInvalidateBYOBRequest ( controller )
 */
static MOZ_MUST_USE bool ReadableByteStreamControllerInvalidateBYOBRequest(
    JSContext* cx, Handle<ReadableByteStreamController*> unwrappedController) {
  // Step 1: If controller.[[byobRequest]] is undefined, return.
  RootedValue unwrappedBYOBRequestVal(cx, unwrappedController->byobRequest());
  if (unwrappedBYOBRequestVal.isUndefined()) {
    return true;
  }
||||||| merged common ancestors
/**
 * Streams spec 3.12.15. ReadableByteStreamControllerInvalidateBYOBRequest ( controller )
 *
 * Note: can operate on unwrapped instances from other compartments for
 * |controller|.
 */
static MOZ_MUST_USE bool
ReadableByteStreamControllerInvalidateBYOBRequest(JSContext* cx,
                                                  Handle<ReadableStreamController*> controller)
{
    MOZ_ASSERT(controller->is<ReadableByteStreamController>());

    // Step 1: If controller.[[byobRequest]] is undefined, return.
    RootedValue byobRequestVal(cx, controller->getFixedSlot(ByteControllerSlot_BYOBRequest));
    if (byobRequestVal.isUndefined()) {
        return true;
    }
=======
// Streams spec 6.2.3.3.1. size ( chunk )
bool CountQueuingStrategy_size(JSContext* cx, unsigned argc, Value* vp) {
  CallArgs args = CallArgsFromVp(argc, vp);

  // Step 1: Return 1.
  args.rval().setInt32(1);
  return true;
}
>>>>>>> upstream-releases

<<<<<<< HEAD
  RootedNativeObject unwrappedBYOBRequest(
      cx, UnwrapAndDowncastValue<NativeObject>(cx, unwrappedBYOBRequestVal));
  if (!unwrappedBYOBRequest) {
    return false;
  }
||||||| merged common ancestors
    RootedNativeObject byobRequest(cx, ToUnwrapped<NativeObject>(cx, byobRequestVal));
    if (!byobRequest) {
        return false;
    }
=======
static const JSPropertySpec CountQueuingStrategy_properties[] = {JS_PS_END};
>>>>>>> upstream-releases

<<<<<<< HEAD
  // Step 2: Set controller.[[byobRequest]]
  //                       .[[associatedReadableByteStreamController]]
  //         to undefined.
  unwrappedBYOBRequest->setFixedSlot(BYOBRequestSlot_Controller,
                                     UndefinedValue());
||||||| merged common ancestors
    // Step 2: Set controller.[[byobRequest]].[[associatedReadableByteStreamController]]
    //         to undefined.
    byobRequest->setFixedSlot(BYOBRequestSlot_Controller, UndefinedValue());
=======
static const JSFunctionSpec CountQueuingStrategy_methods[] = {
    JS_FN("size", CountQueuingStrategy_size, 0, 0), JS_FS_END};
>>>>>>> upstream-releases

<<<<<<< HEAD
  // Step 3: Set controller.[[byobRequest]].[[view]] to undefined.
  unwrappedBYOBRequest->setFixedSlot(BYOBRequestSlot_View, UndefinedValue());
||||||| merged common ancestors
    // Step 3: Set controller.[[byobRequest]].[[view]] to undefined.
    byobRequest->setFixedSlot(BYOBRequestSlot_View, UndefinedValue());
=======
CLASS_SPEC(CountQueuingStrategy, 1, 0, 0, 0, JS_NULL_CLASS_OPS);
>>>>>>> upstream-releases

<<<<<<< HEAD
  // Step 4: Set controller.[[byobRequest]] to undefined.
  unwrappedController->clearBYOBRequest();
||||||| merged common ancestors
    // Step 4: Set controller.[[byobRequest]] to undefined.
    controller->setFixedSlot(ByteControllerSlot_BYOBRequest, UndefinedValue());
=======
#undef CLASS_SPEC
>>>>>>> upstream-releases

<<<<<<< HEAD
  return true;
}
||||||| merged common ancestors
    return true;
}
=======
/*** 6.2. Queue-with-sizes operations ***************************************/
>>>>>>> upstream-releases

<<<<<<< HEAD
// Streams spec, 3.12.25.
//      ReadableByteStreamControllerShouldCallPull ( controller )
// Unified with 3.9.3 above.
||||||| merged common ancestors
// Streams spec, 3.12.24. ReadableByteStreamControllerShouldCallPull ( controller )
// Unified with 3.9.3 above.
=======
/**
 * Streams spec, 6.2.1. DequeueValue ( container ) nothrow
 */
inline static MOZ_MUST_USE bool DequeueValue(
    JSContext* cx, Handle<ReadableStreamController*> unwrappedContainer,
    MutableHandleValue chunk) {
  // Step 1: Assert: container has [[queue]] and [[queueTotalSize]] internal
  //         slots (implicit).
  // Step 2: Assert: queue is not empty.
  Rooted<ListObject*> unwrappedQueue(cx, unwrappedContainer->queue());
  MOZ_ASSERT(unwrappedQueue->length() > 0);

  // Step 3. Let pair be the first element of queue.
  // Step 4. Remove pair from queue, shifting all other elements downward
  //         (so that the second becomes the first, and so on).
  Rooted<QueueEntry*> unwrappedPair(
      cx, &unwrappedQueue->popFirstAs<QueueEntry>(cx));
  MOZ_ASSERT(unwrappedPair);

  // Step 5: Set container.[[queueTotalSize]] to
  //         container.[[queueTotalSize]] − pair.[[size]].
  // Step 6: If container.[[queueTotalSize]] < 0, set
  //         container.[[queueTotalSize]] to 0.
  //         (This can occur due to rounding errors.)
  double totalSize = unwrappedContainer->queueTotalSize();
  totalSize -= unwrappedPair->size();
  if (totalSize < 0) {
    totalSize = 0;
  }
  unwrappedContainer->setQueueTotalSize(totalSize);

  // Step 7: Return pair.[[value]].
  RootedValue val(cx, unwrappedPair->value());
  if (!cx->compartment()->wrap(cx, &val)) {
    return false;
  }
  chunk.set(val);
  return true;
}
>>>>>>> upstream-releases

<<<<<<< HEAD
/*** 6.1. Queuing strategies ************************************************/
||||||| merged common ancestors
// Streams spec, 6.1.2. new ByteLengthQueuingStrategy({ highWaterMark })
bool
js::ByteLengthQueuingStrategy::constructor(JSContext* cx, unsigned argc, Value* vp)
{
    CallArgs args = CallArgsFromVp(argc, vp);
=======
/**
 * Streams spec, 6.2.2. EnqueueValueWithSize ( container, value, size ) throws
 */
static MOZ_MUST_USE bool EnqueueValueWithSize(
    JSContext* cx, Handle<ReadableStreamController*> unwrappedContainer,
    HandleValue value, HandleValue sizeVal) {
  cx->check(value, sizeVal);

  // Step 1: Assert: container has [[queue]] and [[queueTotalSize]] internal
  //         slots (implicit).
  // Step 2: Let size be ? ToNumber(size).
  double size;
  if (!ToNumber(cx, sizeVal, &size)) {
    return false;
  }
>>>>>>> upstream-releases

<<<<<<< HEAD
/**
 * ECMA-262 7.3.4 CreateDataProperty(O, P, V)
 */
static MOZ_MUST_USE bool CreateDataProperty(JSContext* cx, HandleObject obj,
                                            HandlePropertyName key,
                                            HandleValue value,
                                            ObjectOpResult& result) {
  RootedId id(cx, NameToId(key));
  Rooted<PropertyDescriptor> desc(cx);
  desc.setDataDescriptor(value, JSPROP_ENUMERATE);
  return DefineProperty(cx, obj, id, desc, result);
}

// Streams spec, 6.1.2.2. new ByteLengthQueuingStrategy({ highWaterMark })
bool js::ByteLengthQueuingStrategy::constructor(JSContext* cx, unsigned argc,
                                                Value* vp) {
  CallArgs args = CallArgsFromVp(argc, vp);

  if (!ThrowIfNotConstructing(cx, args, "ByteLengthQueuingStrategy")) {
    return false;
  }
||||||| merged common ancestors
    RootedObject strategy(cx, NewBuiltinClassInstance<ByteLengthQueuingStrategy>(cx));
    if (!strategy) {
        return false;
    }
=======
  // Step 3: If ! IsFiniteNonNegativeNumber(size) is false, throw a RangeError
  //         exception.
  if (size < 0 || mozilla::IsNaN(size) || mozilla::IsInfinite(size)) {
    JS_ReportErrorNumberASCII(cx, GetErrorMessage, nullptr,
                              JSMSG_NUMBER_MUST_BE_FINITE_NON_NEGATIVE, "size");
    return false;
  }
>>>>>>> upstream-releases

<<<<<<< HEAD
  // Implicit in the spec: Create the new strategy object.
  RootedObject proto(cx);
  if (!GetPrototypeFromBuiltinConstructor(cx, args, &proto)) {
    return false;
  }
  RootedObject strategy(
      cx, NewObjectWithClassProto<ByteLengthQueuingStrategy>(cx, proto));
  if (!strategy) {
    return false;
  }
||||||| merged common ancestors
    RootedObject argObj(cx, ToObject(cx, args.get(0)));
    if (!argObj) {
      return false;
    }
=======
  // Step 4: Append Record {[[value]]: value, [[size]]: size} as the last
  //         element of container.[[queue]].
  {
    AutoRealm ar(cx, unwrappedContainer);
    Rooted<ListObject*> queue(cx, unwrappedContainer->queue());
    RootedValue wrappedVal(cx, value);
    if (!cx->compartment()->wrap(cx, &wrappedVal)) {
      return false;
    }
>>>>>>> upstream-releases

<<<<<<< HEAD
  // Implicit in the spec: Argument destructuring.
  RootedObject argObj(cx, ToObject(cx, args.get(0)));
  if (!argObj) {
    return false;
  }
  RootedValue highWaterMark(cx);
  if (!GetProperty(cx, argObj, argObj, cx->names().highWaterMark,
                   &highWaterMark)) {
    return false;
  }

  // Step 1: Perform ! CreateDataProperty(this, "highWaterMark",
  //                                      highWaterMark).
  ObjectOpResult ignored;
  if (!CreateDataProperty(cx, strategy, cx->names().highWaterMark,
                          highWaterMark, ignored)) {
    return false;
  }
||||||| merged common ancestors
    RootedValue highWaterMark(cx);
    if (!GetProperty(cx, argObj, argObj, cx->names().highWaterMark, &highWaterMark)) {
      return false;
    }

    if (!SetProperty(cx, strategy, cx->names().highWaterMark, highWaterMark)) {
      return false;
    }
=======
    QueueEntry* entry = QueueEntry::create(cx, wrappedVal, size);
    if (!entry) {
      return false;
    }
    RootedValue val(cx, ObjectValue(*entry));
    if (!queue->append(cx, val)) {
      return false;
    }
  }
>>>>>>> upstream-releases

<<<<<<< HEAD
  args.rval().setObject(*strategy);
  return true;
}

// Streams spec 6.1.2.3.1. size ( chunk )
bool ByteLengthQueuingStrategy_size(JSContext* cx, unsigned argc, Value* vp) {
  CallArgs args = CallArgsFromVp(argc, vp);
||||||| merged common ancestors
    args.rval().setObject(*strategy);
    return true;
}

// Streams spec 6.1.3.1. size ( chunk )
bool
ByteLengthQueuingStrategy_size(JSContext* cx, unsigned argc, Value* vp)
{
    CallArgs args = CallArgsFromVp(argc, vp);
=======
  // Step 5: Set container.[[queueTotalSize]] to
  //         container.[[queueTotalSize]] + size.
  unwrappedContainer->setQueueTotalSize(unwrappedContainer->queueTotalSize() +
                                        size);
>>>>>>> upstream-releases

<<<<<<< HEAD
  // Step 1: Return ? GetV(chunk, "byteLength").
  return GetProperty(cx, args.get(0), cx->names().byteLength, args.rval());
||||||| merged common ancestors
    // Step 1: Return ? GetV(chunk, "byteLength").
    return GetProperty(cx, args.get(0), cx->names().byteLength, args.rval());
=======
  return true;
>>>>>>> upstream-releases
}

<<<<<<< HEAD
static const JSPropertySpec ByteLengthQueuingStrategy_properties[] = {
    JS_PS_END};

static const JSFunctionSpec ByteLengthQueuingStrategy_methods[] = {
    JS_FN("size", ByteLengthQueuingStrategy_size, 1, 0), JS_FS_END};
||||||| merged common ancestors
static const JSPropertySpec ByteLengthQueuingStrategy_properties[] = {
    JS_PS_END
};

static const JSFunctionSpec ByteLengthQueuingStrategy_methods[] = {
    JS_FN("size", ByteLengthQueuingStrategy_size, 1, 0),
    JS_FS_END
};
=======
/**
 * Streams spec, 6.2.4. ResetQueue ( container ) nothrow
 */
inline static MOZ_MUST_USE bool ResetQueue(
    JSContext* cx, Handle<ReadableStreamController*> unwrappedContainer) {
  // Step 1: Assert: container has [[queue]] and [[queueTotalSize]] internal
  //         slots (implicit).
  // Step 2: Set container.[[queue]] to a new empty List.
  if (!SetNewList(cx, unwrappedContainer, StreamController::Slot_Queue)) {
    return false;
  }
>>>>>>> upstream-releases

  // Step 3: Set container.[[queueTotalSize]] to 0.
  unwrappedContainer->setQueueTotalSize(0);

<<<<<<< HEAD
// Streams spec, 6.1.3.2. new CountQueuingStrategy({ highWaterMark })
bool js::CountQueuingStrategy::constructor(JSContext* cx, unsigned argc,
                                           Value* vp) {
  CallArgs args = CallArgsFromVp(argc, vp);
||||||| merged common ancestors
// Streams spec, 6.2.2. new CountQueuingStrategy({ highWaterMark })
bool
js::CountQueuingStrategy::constructor(JSContext* cx, unsigned argc, Value* vp)
{
    CallArgs args = CallArgsFromVp(argc, vp);
=======
  return true;
}
>>>>>>> upstream-releases

<<<<<<< HEAD
  if (!ThrowIfNotConstructing(cx, args, "CountQueuingStrategy")) {
    return false;
  }
||||||| merged common ancestors
    Rooted<CountQueuingStrategy*> strategy(cx, NewBuiltinClassInstance<CountQueuingStrategy>(cx));
    if (!strategy) {
        return false;
    }
=======
/*** 6.3. Miscellaneous operations ******************************************/
>>>>>>> upstream-releases

<<<<<<< HEAD
  // Implicit in the spec: Create the new strategy object.
  RootedObject proto(cx);
  if (!GetPrototypeFromBuiltinConstructor(cx, args, &proto)) {
    return false;
  }
  Rooted<CountQueuingStrategy*> strategy(
      cx, NewObjectWithClassProto<CountQueuingStrategy>(cx, proto));
  if (!strategy) {
    return false;
  }
||||||| merged common ancestors
    RootedObject argObj(cx, ToObject(cx, args.get(0)));
    if (!argObj) {
      return false;
    }
=======
/**
 * Appends the given |obj| to the given list |container|'s list.
 */
inline static MOZ_MUST_USE bool AppendToListAtSlot(
    JSContext* cx, HandleNativeObject unwrappedContainer, uint32_t slot,
    HandleObject obj) {
  Rooted<ListObject*> list(
      cx, &unwrappedContainer->getFixedSlot(slot).toObject().as<ListObject>());

  AutoRealm ar(cx, list);
  RootedValue val(cx, ObjectValue(*obj));
  if (!cx->compartment()->wrap(cx, &val)) {
    return false;
  }
  return list->append(cx, val);
}
>>>>>>> upstream-releases

<<<<<<< HEAD
  // Implicit in the spec: Argument destructuring.
  RootedObject argObj(cx, ToObject(cx, args.get(0)));
  if (!argObj) {
    return false;
  }
  RootedValue highWaterMark(cx);
  if (!GetProperty(cx, argObj, argObj, cx->names().highWaterMark,
                   &highWaterMark)) {
    return false;
  }
||||||| merged common ancestors
    RootedValue highWaterMark(cx);
    if (!GetProperty(cx, argObj, argObj, cx->names().highWaterMark, &highWaterMark)) {
      return false;
    }
=======
/**
 * Streams spec, 6.3.1.
 *      CreateAlgorithmFromUnderlyingMethod ( underlyingObject, methodName,
 *                                            algoArgCount, extraArgs )
 *
 * This function only partly implements the standard algorithm. We do not
 * actually create a new JSFunction completely encapsulating the new algorithm.
 * Instead, this just gets the specified method and checks for errors. It's the
 * caller's responsibility to make sure that later, when the algorithm is
 * "performed", the appropriate steps are carried out.
 */
static MOZ_MUST_USE bool CreateAlgorithmFromUnderlyingMethod(
    JSContext* cx, HandleValue underlyingObject,
    const char* methodNameForErrorMessage, HandlePropertyName methodName,
    MutableHandleValue method) {
  // Step 1: Assert: underlyingObject is not undefined.
  MOZ_ASSERT(!underlyingObject.isUndefined());

  // Step 2: Assert: ! IsPropertyKey(methodName) is true (implicit).
  // Step 3: Assert: algoArgCount is 0 or 1 (omitted).
  // Step 4: Assert: extraArgs is a List (omitted).

  // Step 5: Let method be ? GetV(underlyingObject, methodName).
  if (!GetProperty(cx, underlyingObject, methodName, method)) {
    return false;
  }
>>>>>>> upstream-releases

<<<<<<< HEAD
  // Step 1: Perform ! CreateDataProperty(this, "highWaterMark", highWaterMark).
  ObjectOpResult ignored;
  if (!CreateDataProperty(cx, strategy, cx->names().highWaterMark,
                          highWaterMark, ignored)) {
    return false;
  }
||||||| merged common ancestors
    if (!SetProperty(cx, strategy, cx->names().highWaterMark, highWaterMark)) {
      return false;
    }
=======
  // Step 6: If method is not undefined,
  if (!method.isUndefined()) {
    // Step a: If ! IsCallable(method) is false, throw a TypeError
    //         exception.
    if (!IsCallable(method)) {
      JS_ReportErrorNumberASCII(cx, GetErrorMessage, nullptr,
                                JSMSG_NOT_FUNCTION, methodNameForErrorMessage);
      return false;
    }
>>>>>>> upstream-releases

<<<<<<< HEAD
  args.rval().setObject(*strategy);
  return true;
||||||| merged common ancestors
    args.rval().setObject(*strategy);
    return true;
=======
    // Step b: If algoArgCount is 0, return an algorithm that performs the
    //         following steps:
    //     Step i: Return ! PromiseCall(method, underlyingObject,
    //             extraArgs).
    // Step c: Otherwise, return an algorithm that performs the following
    //         steps, taking an arg argument:
    //     Step i: Let fullArgs be a List consisting of arg followed by the
    //             elements of extraArgs in order.
    //     Step ii: Return ! PromiseCall(method, underlyingObject,
    //                                   fullArgs).
    // (These steps are deferred to the code that performs the algorithm.
    // See ReadableStreamControllerCancelSteps and
    // ReadableStreamControllerCallPullIfNeeded.)
    return true;
  }

  // Step 7: Return an algorithm which returns a promise resolved with
  //         undefined (implicit).
  return true;
>>>>>>> upstream-releases
}

<<<<<<< HEAD
// Streams spec 6.2.3.3.1. size ( chunk )
bool CountQueuingStrategy_size(JSContext* cx, unsigned argc, Value* vp) {
  CallArgs args = CallArgsFromVp(argc, vp);
||||||| merged common ancestors
// Streams spec 6.2.3.1. size ( chunk )
bool
CountQueuingStrategy_size(JSContext* cx, unsigned argc, Value* vp)
{
    CallArgs args = CallArgsFromVp(argc, vp);
=======
/**
 * Streams spec, 6.3.2. InvokeOrNoop ( O, P, args )
 * As it happens, all callers pass exactly one argument.
 */
inline static MOZ_MUST_USE bool InvokeOrNoop(JSContext* cx, HandleValue O,
                                             HandlePropertyName P,
                                             HandleValue arg,
                                             MutableHandleValue rval) {
  cx->check(O, P, arg);

  // Step 1: Assert: O is not undefined.
  MOZ_ASSERT(!O.isUndefined());

  // Step 2: Assert: ! IsPropertyKey(P) is true (implicit).
  // Step 3: Assert: args is a List (implicit).
  // Step 4: Let method be ? GetV(O, P).
  RootedValue method(cx);
  if (!GetProperty(cx, O, P, &method)) {
    return false;
  }
>>>>>>> upstream-releases

<<<<<<< HEAD
  // Step 1: Return 1.
  args.rval().setInt32(1);
  return true;
||||||| merged common ancestors
    // Step 1: Return 1.
    args.rval().setInt32(1);
    return true;
=======
  // Step 5: If method is undefined, return.
  if (method.isUndefined()) {
    return true;
  }

  // Step 6: Return ? Call(method, O, args).
  return Call(cx, method, O, arg, rval);
>>>>>>> upstream-releases
}

<<<<<<< HEAD
static const JSPropertySpec CountQueuingStrategy_properties[] = {JS_PS_END};
||||||| merged common ancestors
static const JSPropertySpec CountQueuingStrategy_properties[] = {
    JS_PS_END
};
=======
/**
 * Streams spec, 6.3.5. PromiseCall ( F, V, args )
 * As it happens, all callers pass exactly one argument.
 */
static MOZ_MUST_USE JSObject* PromiseCall(JSContext* cx, HandleValue F,
                                          HandleValue V, HandleValue arg) {
  cx->check(F, V, arg);
>>>>>>> upstream-releases

<<<<<<< HEAD
static const JSFunctionSpec CountQueuingStrategy_methods[] = {
    JS_FN("size", CountQueuingStrategy_size, 0, 0), JS_FS_END};
||||||| merged common ancestors
static const JSFunctionSpec CountQueuingStrategy_methods[] = {
    JS_FN("size", CountQueuingStrategy_size, 0, 0),
    JS_FS_END
};
=======
  // Step 1: Assert: ! IsCallable(F) is true.
  MOZ_ASSERT(IsCallable(F));
>>>>>>> upstream-releases

  // Step 2: Assert: V is not undefined.
  MOZ_ASSERT(!V.isUndefined());

  // Step 3: Assert: args is a List (implicit).
  // Step 4: Let returnValue be Call(F, V, args).
  RootedValue rval(cx);
  if (!Call(cx, F, V, arg, &rval)) {
    // Step 5: If returnValue is an abrupt completion, return a promise rejected
    // with returnValue.[[Value]].
    return PromiseRejectedWithPendingError(cx);
  }

  // Step 6: Otherwise, return a promise resolved with returnValue.[[Value]].
  return PromiseObject::unforgeableResolve(cx, rval);
}

/*** 6.2. Queue-with-sizes operations ***************************************/

/**
<<<<<<< HEAD
 * Streams spec, 6.2.1. DequeueValue ( container ) nothrow
||||||| merged common ancestors
 * Streams spec, 6.3.1. DequeueValue ( container ) nothrow
 *
 * Note: can operate on unwrapped queue container instances from another
 * compartment. In that case, the returned chunk will be wrapped into the
 * current compartment.
=======
 * Streams spec, 6.3.7. ValidateAndNormalizeHighWaterMark ( highWaterMark )
>>>>>>> upstream-releases
 */
<<<<<<< HEAD
inline static MOZ_MUST_USE bool DequeueValue(
    JSContext* cx, Handle<ReadableStreamController*> unwrappedContainer,
    MutableHandleValue chunk) {
  // Step 1: Assert: container has [[queue]] and [[queueTotalSize]] internal
  //         slots (implicit).
  // Step 2: Assert: queue is not empty.
  Rooted<ListObject*> unwrappedQueue(cx, unwrappedContainer->queue());
  MOZ_ASSERT(unwrappedQueue->length() > 0);

  // Step 3. Let pair be the first element of queue.
  // Step 4. Remove pair from queue, shifting all other elements downward
  //         (so that the second becomes the first, and so on).
  Rooted<QueueEntry*> unwrappedPair(
      cx, &unwrappedQueue->popFirstAs<QueueEntry>(cx));
  MOZ_ASSERT(unwrappedPair);

  // Step 5: Set container.[[queueTotalSize]] to
  //         container.[[queueTotalSize]] − pair.[[size]].
  // Step 6: If container.[[queueTotalSize]] < 0, set
  //         container.[[queueTotalSize]] to 0.
  //         (This can occur due to rounding errors.)
  double totalSize = unwrappedContainer->queueTotalSize();

  totalSize -= unwrappedPair->size();
  if (totalSize < 0) {
    totalSize = 0;
  }
  unwrappedContainer->setQueueTotalSize(totalSize);

  RootedValue val(cx, unwrappedPair->value());
  if (!cx->compartment()->wrap(cx, &val)) {
    return false;
  }
||||||| merged common ancestors
inline static MOZ_MUST_USE bool
DequeueValue(JSContext* cx, Handle<ReadableStreamController*> container, MutableHandleValue chunk)
{
    // Step 1: Assert: container has [[queue]] and [[queueTotalSize]] internal
    //         slots (implicit).
    // Step 2: Assert: queue is not empty.
    RootedValue val(cx, container->getFixedSlot(QueueContainerSlot_Queue));
    RootedNativeObject queue(cx, &val.toObject().as<NativeObject>());
    MOZ_ASSERT(queue->getDenseInitializedLength() > 0);

    // Step 3. Let pair be the first element of queue.
    // Step 4. Remove pair from queue, shifting all other elements downward
    //         (so that the second becomes the first, and so on).
    Rooted<QueueEntry*> pair(cx, ShiftFromList<QueueEntry>(cx, queue));
    MOZ_ASSERT(pair);

    // Step 5: Set container.[[queueTotalSize]] to
    //         container.[[queueTotalSize]] − pair.[[size]].
    // Step 6: If container.[[queueTotalSize]] < 0, set
    //         container.[[queueTotalSize]] to 0.
    //         (This can occur due to rounding errors.)
    double totalSize = QueueSize(container);

    totalSize -= pair->size();
    if (totalSize < 0) {
        totalSize = 0;
    }
    SetQueueSize(container, totalSize);

    val = pair->value();
    if (container->compartment() != cx->compartment() && !cx->compartment()->wrap(cx, &val)) {
        return false;
    }
=======
static MOZ_MUST_USE bool ValidateAndNormalizeHighWaterMark(
    JSContext* cx, HandleValue highWaterMarkVal, double* highWaterMark) {
  // Step 1: Set highWaterMark to ? ToNumber(highWaterMark).
  if (!ToNumber(cx, highWaterMarkVal, highWaterMark)) {
    return false;
  }

  // Step 2: If highWaterMark is NaN or highWaterMark < 0, throw a RangeError
  // exception.
  if (mozilla::IsNaN(*highWaterMark) || *highWaterMark < 0) {
    JS_ReportErrorNumberASCII(cx, GetErrorMessage, nullptr,
                              JSMSG_STREAM_INVALID_HIGHWATERMARK);
    return false;
  }
>>>>>>> upstream-releases

<<<<<<< HEAD
  // Step 7: Return pair.[[value]].
  chunk.set(val);
  return true;
||||||| merged common ancestors
    // Step 7: Return pair.[[value]].
    chunk.set(val);
    return true;
=======
  // Step 3: Return highWaterMark.
  return true;
>>>>>>> upstream-releases
}

/**
<<<<<<< HEAD
 * Streams spec, 6.2.2. EnqueueValueWithSize ( container, value, size ) throws
||||||| merged common ancestors
 * Streams spec, 6.3.2. EnqueueValueWithSize ( container, value, size ) throws
 *
 * Note: can operate on unwrapped queue container instances from another
 * compartment than the current one. In that case, the given value will be
 * wrapped into the container compartment.
=======
 * Streams spec, 6.3.8. MakeSizeAlgorithmFromSizeFunction ( size )
 *
 * The standard makes a big deal of turning JavaScript functions (grubby,
 * touched by users, covered with germs) into algorithms (pristine,
 * respectable, purposeful). We don't bother. Here we only check for errors and
 * leave `size` unchanged. Then, in ReadableStreamDefaultControllerEnqueue,
 * where this value is used, we have to check for undefined and behave as if we
 * had "made" an "algorithm" as described below.
>>>>>>> upstream-releases
 */
<<<<<<< HEAD
static MOZ_MUST_USE bool EnqueueValueWithSize(
    JSContext* cx, Handle<ReadableStreamController*> unwrappedContainer,
    HandleValue value, HandleValue sizeVal) {
  cx->check(value, sizeVal);

  // Step 1: Assert: container has [[queue]] and [[queueTotalSize]] internal
  //         slots (implicit).
  // Step 2: Let size be ? ToNumber(size).
  double size;
  if (!ToNumber(cx, sizeVal, &size)) {
    return false;
  }
||||||| merged common ancestors
static MOZ_MUST_USE bool
EnqueueValueWithSize(JSContext* cx, Handle<ReadableStreamController*> container, HandleValue value,
                     HandleValue sizeVal)
{
    // Step 1: Assert: container has [[queue]] and [[queueTotalSize]] internal
    //         slots (implicit).
    // Step 2: Let size be ? ToNumber(size).
    double size;
    if (!ToNumber(cx, sizeVal, &size)) {
        return false;
    }

    // Step 3: If ! IsFiniteNonNegativeNumber(size) is false, throw a RangeError
    //         exception.
    if (size < 0 || mozilla::IsNaN(size) || mozilla::IsInfinite(size)) {
        JS_ReportErrorNumberASCII(cx, GetErrorMessage, nullptr,
                                  JSMSG_NUMBER_MUST_BE_FINITE_NON_NEGATIVE, "size");
        return false;
    }
=======
static MOZ_MUST_USE bool MakeSizeAlgorithmFromSizeFunction(JSContext* cx,
                                                           HandleValue size) {
  // Step 1: If size is undefined, return an algorithm that returns 1.
  if (size.isUndefined()) {
    // Deferred. Size algorithm users must check for undefined.
    return true;
  }

  // Step 2: If ! IsCallable(size) is false, throw a TypeError exception.
  if (!IsCallable(size)) {
    JS_ReportErrorNumberASCII(cx, GetErrorMessage, nullptr, JSMSG_NOT_FUNCTION,
                              "ReadableStream argument options.size");
    return false;
  }
>>>>>>> upstream-releases

<<<<<<< HEAD
  // Step 3: If ! IsFiniteNonNegativeNumber(size) is false, throw a RangeError
  //         exception.
  if (size < 0 || mozilla::IsNaN(size) || mozilla::IsInfinite(size)) {
    JS_ReportErrorNumberASCII(cx, GetErrorMessage, nullptr,
                              JSMSG_NUMBER_MUST_BE_FINITE_NON_NEGATIVE, "size");
    return false;
  }
||||||| merged common ancestors
    // Step 4: Append Record {[[value]]: value, [[size]]: size} as the last element
    //         of container.[[queue]].
    RootedValue val(cx, container->getFixedSlot(QueueContainerSlot_Queue));
    RootedNativeObject queue(cx, &val.toObject().as<NativeObject>());
=======
  // Step 3: Return an algorithm that performs the following steps, taking a
  //         chunk argument:
  //     a. Return ? Call(size, undefined, « chunk »).
  // Deferred. Size algorithm users must know how to call the size function.
  return true;
}
>>>>>>> upstream-releases

<<<<<<< HEAD
  // Step 4: Append Record {[[value]]: value, [[size]]: size} as the last
  //         element of container.[[queue]].
  {
    AutoRealm ar(cx, unwrappedContainer);
    Rooted<ListObject*> queue(cx, unwrappedContainer->queue());
    RootedValue wrappedVal(cx, value);
    if (!cx->compartment()->wrap(cx, &wrappedVal)) {
      return false;
    }
||||||| merged common ancestors
    RootedValue wrappedVal(cx, value);
    {
        mozilla::Maybe<AutoRealm> ar;
        if (container->compartment() != cx->compartment()) {
            ar.emplace(cx, container);
            if (!cx->compartment()->wrap(cx, &wrappedVal)) {
                return false;
            }
        }
=======
/*** API entry points *******************************************************/
>>>>>>> upstream-releases

<<<<<<< HEAD
    QueueEntry* entry = QueueEntry::create(cx, wrappedVal, size);
    if (!entry) {
      return false;
    }
    RootedValue val(cx, ObjectValue(*entry));
    if (!queue->append(cx, val)) {
      return false;
    }
  }
||||||| merged common ancestors
        QueueEntry* entry = QueueEntry::create(cx, wrappedVal, size);
        if (!entry) {
            return false;
        }
        val = ObjectValue(*entry);
        if (!AppendToList(cx, queue, val)) {
            return false;
        }
    }
=======
JS_FRIEND_API JSObject* js::UnwrapReadableStream(JSObject* obj) {
  return obj->maybeUnwrapIf<ReadableStream>();
}
>>>>>>> upstream-releases

<<<<<<< HEAD
  // Step 5: Set container.[[queueTotalSize]] to
  //         container.[[queueTotalSize]] + size.
  unwrappedContainer->setQueueTotalSize(unwrappedContainer->queueTotalSize() +
                                        size);
||||||| merged common ancestors
    // Step 5: Set container.[[queueTotalSize]] to
    //         container.[[queueTotalSize]] + size.
    SetQueueSize(container, QueueSize(container) + size);
=======
JS_PUBLIC_API JSObject* JS::NewReadableDefaultStreamObject(
    JSContext* cx, JS::HandleObject underlyingSource /* = nullptr */,
    JS::HandleFunction size /* = nullptr */, double highWaterMark /* = 1 */,
    JS::HandleObject proto /* = nullptr */) {
  MOZ_ASSERT(!cx->zone()->isAtomsZone());
  AssertHeapIsIdle();
  CHECK_THREAD(cx);
  cx->check(underlyingSource, size, proto);
  MOZ_ASSERT(highWaterMark >= 0);
>>>>>>> upstream-releases

<<<<<<< HEAD
  return true;
||||||| merged common ancestors
    return true;
=======
  // A copy of ReadableStream::constructor, with most of the
  // argument-checking done implicitly by C++ type checking.
  Rooted<ReadableStream*> stream(cx, ReadableStream::create(cx));
  if (!stream) {
    return nullptr;
  }
  RootedValue sourceVal(cx);
  if (underlyingSource) {
    sourceVal.setObject(*underlyingSource);
  } else {
    JSObject* source = NewBuiltinClassInstance<PlainObject>(cx);
    if (!source) {
      return nullptr;
    }
    sourceVal.setObject(*source);
  }
  RootedValue sizeVal(cx, size ? ObjectValue(*size) : UndefinedValue());

  if (!SetUpReadableStreamDefaultControllerFromUnderlyingSource(
          cx, stream, sourceVal, highWaterMark, sizeVal)) {
    return nullptr;
  }

  return stream;
>>>>>>> upstream-releases
}

<<<<<<< HEAD
/**
 * Streams spec, 6.2.4. ResetQueue ( container ) nothrow
 *
 * Note: can operate on unwrapped container instances from another
 * compartment.
 */
inline static MOZ_MUST_USE bool ResetQueue(
    JSContext* cx, Handle<ReadableStreamController*> unwrappedContainer) {
  // Step 1: Assert: container has [[queue]] and [[queueTotalSize]] internal
  //         slots (implicit).
  // Step 2: Set container.[[queue]] to a new empty List.
  if (!SetNewList(cx, unwrappedContainer, StreamController::Slot_Queue)) {
    return false;
  }
||||||| merged common ancestors
// Streams spec, 6.3.3. PeekQueueValue ( container ) nothrow
// Used by WritableStream.
// static MOZ_MUST_USE Value
// PeekQueueValue(ReadableStreamController* container)
// {
//     // Step 1: Assert: container has [[queue]] and [[queueTotalSize]] internal
//     //         slots (implicit).
//     // Step 2: Assert: queue is not empty.
//     Value val = container->getFixedSlot(QueueContainerSlot_Queue);
//     NativeObject* queue = &val.toObject().as<NativeObject>();
//     MOZ_ASSERT(queue->getDenseInitializedLength() > 0);

//     // Step 3: Let pair be the first element of container.[[queue]].
//     QueueEntry* pair = PeekList<QueueEntry>(queue);

//     // Step 4: Return pair.[[value]].
//     return pair->value();
// }

/**
 * Streams spec, 6.3.4. ResetQueue ( container ) nothrow
 */
inline static MOZ_MUST_USE bool
ResetQueue(JSContext* cx, Handle<ReadableStreamController*> container)
{
    // Step 1: Assert: container has [[queue]] and [[queueTotalSize]] internal
    //         slots (implicit).
    // Step 2: Set container.[[queue]] to a new empty List.
    if (!SetNewList(cx, container, QueueContainerSlot_Queue)) {
        return false;
    }
=======
JS_PUBLIC_API JSObject* JS::NewReadableExternalSourceStreamObject(
    JSContext* cx, JS::ReadableStreamUnderlyingSource* underlyingSource,
    void* nsISupportsObject_alreadyAddreffed /* = nullptr */,
    HandleObject proto /* = nullptr */) {
  MOZ_ASSERT(!cx->zone()->isAtomsZone());
  AssertHeapIsIdle();
  CHECK_THREAD(cx);
  MOZ_ASSERT(underlyingSource);
  MOZ_ASSERT((uintptr_t(underlyingSource) & 1) == 0,
             "external underlying source pointers must be aligned");
  cx->check(proto);

  return ReadableStream::createExternalSourceStream(
      cx, underlyingSource, nsISupportsObject_alreadyAddreffed, proto);
}
>>>>>>> upstream-releases

<<<<<<< HEAD
  // Step 3: Set container.[[queueTotalSize]] to 0.
  unwrappedContainer->setQueueTotalSize(0);
||||||| merged common ancestors
    // Step 3: Set container.[[queueTotalSize]] to 0.
    SetQueueSize(container, 0);
=======
JS_PUBLIC_API bool JS::IsReadableStream(JSObject* obj) {
  return obj->canUnwrapAs<ReadableStream>();
}
>>>>>>> upstream-releases

<<<<<<< HEAD
  return true;
||||||| merged common ancestors
    return true;
=======
JS_PUBLIC_API bool JS::IsReadableStreamReader(JSObject* obj) {
  return obj->canUnwrapAs<ReadableStreamDefaultReader>();
>>>>>>> upstream-releases
}

<<<<<<< HEAD
/*** 6.3. Miscellaneous operations ******************************************/
||||||| merged common ancestors
inline static double
QueueSize(const NativeObject* container)
{
    return container->getFixedSlot(QueueContainerSlot_TotalSize).toNumber();
}
=======
JS_PUBLIC_API bool JS::IsReadableStreamDefaultReader(JSObject* obj) {
  return obj->canUnwrapAs<ReadableStreamDefaultReader>();
}
>>>>>>> upstream-releases

<<<<<<< HEAD
/**
 * Appends the given |obj| to the given list |container|'s list.
 */
inline static MOZ_MUST_USE bool AppendToListAtSlot(
    JSContext* cx, HandleNativeObject unwrappedContainer, uint32_t slot,
    HandleObject obj) {
  Rooted<ListObject*> list(
      cx, &unwrappedContainer->getFixedSlot(slot).toObject().as<ListObject>());

  AutoRealm ar(cx, list);
  RootedValue val(cx, ObjectValue(*obj));
  if (!cx->compartment()->wrap(cx, &val)) {
    return false;
  }
  return list->append(cx, val);
||||||| merged common ancestors
inline static void
SetQueueSize(NativeObject* container, double size)
{
    container->setFixedSlot(QueueContainerSlot_TotalSize, NumberValue(size));
=======
template <class T>
static MOZ_MUST_USE T* APIUnwrapAndDowncast(JSContext* cx, JSObject* obj) {
  cx->check(obj);
  return UnwrapAndDowncastObject<T>(cx, obj);
>>>>>>> upstream-releases
}

<<<<<<< HEAD
/**
 * Streams spec, 6.3.1.
 *      CreateAlgorithmFromUnderlyingMethod ( underlyingObject, methodName,
 *                                            algoArgCount, extraArgs )
 *
 * This function only partly implements the standard algorithm. We do not
 * actually create a new JSFunction completely encapsulating the new algorithm.
 * Instead, this just gets the specified method and checks for errors. It's the
 * caller's responsibility to make sure that later, when the algorithm is
 * "performed", the appropriate steps are carried out.
 */
static MOZ_MUST_USE bool CreateAlgorithmFromUnderlyingMethod(
    JSContext* cx, HandleValue underlyingObject,
    const char* methodNameForErrorMessage, HandlePropertyName methodName,
    MutableHandleValue method) {
  // Step 1: Assert: underlyingObject is not undefined.
  MOZ_ASSERT(!underlyingObject.isUndefined());

  // Step 2: Assert: ! IsPropertyKey(methodName) is true (implicit).
  // Step 3: Assert: algoArgCount is 0 or 1 (omitted).
  // Step 4: Assert: extraArgs is a List (omitted).

  // Step 5: Let method be ? GetV(underlyingObject, methodName).
  if (!GetProperty(cx, underlyingObject, methodName, method)) {
    return false;
  }
||||||| merged common ancestors
/**
 * Appends the given |obj| to the given list |container|'s list.
 *
 * Note: can operate on |container| and |obj| combinations from different
 * compartments, in which case |obj| is wrapped before storing it.
 */
inline static MOZ_MUST_USE bool
AppendToListAtSlot(JSContext* cx, HandleNativeObject container, uint32_t slot, HandleObject obj)
{
    RootedValue val(cx, container->getFixedSlot(slot));
    RootedNativeObject list(cx, &val.toObject().as<NativeObject>());

    val = ObjectValue(*obj);
=======
JS_PUBLIC_API bool JS::ReadableStreamIsReadable(JSContext* cx,
                                                HandleObject streamObj,
                                                bool* result) {
  ReadableStream* unwrappedStream =
      APIUnwrapAndDowncast<ReadableStream>(cx, streamObj);
  if (!unwrappedStream) {
    return false;
  }

  *result = unwrappedStream->readable();
  return true;
}

JS_PUBLIC_API bool JS::ReadableStreamIsLocked(JSContext* cx,
                                              HandleObject streamObj,
                                              bool* result) {
  ReadableStream* unwrappedStream =
      APIUnwrapAndDowncast<ReadableStream>(cx, streamObj);
  if (!unwrappedStream) {
    return false;
  }
>>>>>>> upstream-releases

<<<<<<< HEAD
  // Step 6: If method is not undefined,
  if (!method.isUndefined()) {
    // Step a: If ! IsCallable(method) is false, throw a TypeError
    //         exception.
    if (!IsCallable(method)) {
      JS_ReportErrorNumberASCII(cx, GetErrorMessage, nullptr,
                                JSMSG_NOT_FUNCTION, methodNameForErrorMessage);
      return false;
    }
||||||| merged common ancestors
    AutoRealm ar(cx, list);
    if (!cx->compartment()->wrap(cx, &val)) {
        return false;
    }
    return AppendToList(cx, list, val);
}
=======
  *result = unwrappedStream->locked();
  return true;
}
>>>>>>> upstream-releases

<<<<<<< HEAD
    // Step b: If algoArgCount is 0, return an algorithm that performs the
    //         following steps:
    //     Step i: Return ! PromiseCall(method, underlyingObject,
    //             extraArgs).
    // Step c: Otherwise, return an algorithm that performs the following
    //         steps, taking an arg argument:
    //     Step i: Let fullArgs be a List consisting of arg followed by the
    //             elements of extraArgs in order.
    //     Step ii: Return ! PromiseCall(method, underlyingObject,
    //                                   fullArgs).
    // (These steps are deferred to the code that performs the algorithm.
    // See ReadableStreamControllerCancelSteps and
    // ReadableStreamControllerCallPullIfNeeded.)
    return true;
  }

  // Step 7: Return an algorithm which returns a promise resolved with
  //         undefined (implicit).
  return true;
}
||||||| merged common ancestors
=======
JS_PUBLIC_API bool JS::ReadableStreamIsDisturbed(JSContext* cx,
                                                 HandleObject streamObj,
                                                 bool* result) {
  ReadableStream* unwrappedStream =
      APIUnwrapAndDowncast<ReadableStream>(cx, streamObj);
  if (!unwrappedStream) {
    return false;
  }
>>>>>>> upstream-releases

<<<<<<< HEAD
/**
 * Streams spec, 6.3.2. InvokeOrNoop ( O, P, args )
 */
inline static MOZ_MUST_USE bool InvokeOrNoop(JSContext* cx, HandleValue O,
                                             HandlePropertyName P,
                                             HandleValue arg,
                                             MutableHandleValue rval) {
  cx->check(O, P, arg);

  // Step 1: Assert: P is a valid property key (omitted).
  // Step 2: If args was not passed, let args be a new empty List (omitted).
  // Step 3: Let method be ? GetV(O, P).
  RootedValue method(cx);
  if (!GetProperty(cx, O, P, &method)) {
    return false;
  }
||||||| merged common ancestors
/**
 * Streams spec, 6.4.1. InvokeOrNoop ( O, P, args )
 */
inline static MOZ_MUST_USE bool
InvokeOrNoop(JSContext* cx, HandleValue O, HandlePropertyName P, HandleValue arg,
             MutableHandleValue rval)
{
    // Step 1: Assert: P is a valid property key (omitted).
    // Step 2: If args was not passed, let args be a new empty List (omitted).
    // Step 3: Let method be ? GetV(O, P).
    RootedValue method(cx);
    if (!GetProperty(cx, O, P, &method)) {
        return false;
    }
=======
  *result = unwrappedStream->disturbed();
  return true;
}
>>>>>>> upstream-releases

<<<<<<< HEAD
  // Step 4: If method is undefined, return.
  if (method.isUndefined()) {
    return true;
  }
||||||| merged common ancestors
    // Step 4: If method is undefined, return.
    if (method.isUndefined()) {
        return true;
    }
=======
JS_PUBLIC_API JSObject* JS::ReadableStreamCancel(JSContext* cx,
                                                 HandleObject streamObj,
                                                 HandleValue reason) {
  AssertHeapIsIdle();
  CHECK_THREAD(cx);
  cx->check(reason);
>>>>>>> upstream-releases

<<<<<<< HEAD
  // Step 5: Return ? Call(method, O, args).
  return Call(cx, method, O, arg, rval);
||||||| merged common ancestors
    // Step 5: Return ? Call(method, O, args).
    return Call(cx, method, O, arg, rval);
=======
  Rooted<ReadableStream*> unwrappedStream(
      cx, APIUnwrapAndDowncast<ReadableStream>(cx, streamObj));
  if (!unwrappedStream) {
    return nullptr;
  }

  return ::ReadableStreamCancel(cx, unwrappedStream, reason);
>>>>>>> upstream-releases
}

<<<<<<< HEAD
/**
 * Streams spec, 6.3.5. PromiseCall ( F, V, args )
 * As it happens, all callers pass exactly one argument.
 */
static MOZ_MUST_USE JSObject* PromiseCall(JSContext* cx, HandleValue F,
                                          HandleValue V, HandleValue arg) {
  cx->check(F, V, arg);

  // Step 1: Assert: ! IsCallable(F) is true.
  MOZ_ASSERT(IsCallable(F));

  // Step 2: Assert: V is not undefined.
  MOZ_ASSERT(!V.isUndefined());
||||||| merged common ancestors
/**
 * Streams spec, 6.4.3. PromiseInvokeOrNoop ( O, P, args )
 * Specialized to one arg, because that's what all stream related callers use.
 */
static MOZ_MUST_USE JSObject*
PromiseInvokeOrNoop(JSContext* cx, HandleValue O, HandlePropertyName P, HandleValue arg)
{
    // Step 1: Assert: O is not undefined.
    MOZ_ASSERT(!O.isUndefined());

    // Step 2: Assert: ! IsPropertyKey(P) is true (implicit).
    // Step 3: Assert: args is a List (omitted).

    // Step 4: Let returnValue be InvokeOrNoop(O, P, args).
    // Step 5: If returnValue is an abrupt completion, return a promise
    //         rejected with returnValue.[[Value]].
    RootedValue returnValue(cx);
    if (!InvokeOrNoop(cx, O, P, arg, &returnValue)) {
        return PromiseRejectedWithPendingError(cx);
    }
=======
JS_PUBLIC_API bool JS::ReadableStreamGetMode(JSContext* cx,
                                             HandleObject streamObj,
                                             JS::ReadableStreamMode* mode) {
  ReadableStream* unwrappedStream =
      APIUnwrapAndDowncast<ReadableStream>(cx, streamObj);
  if (!unwrappedStream) {
    return false;
  }
>>>>>>> upstream-releases

<<<<<<< HEAD
  // Step 3: Assert: args is a List (implicit).
  // Step 4: Let returnValue be Call(F, V, args).
  RootedValue rval(cx);
  if (!Call(cx, F, V, arg, &rval)) {
    // Step 5: If returnValue is an abrupt completion, return a promise rejected
    // with returnValue.[[Value]].
    return PromiseRejectedWithPendingError(cx);
  }

  // Step 6: Otherwise, return a promise resolved with returnValue.[[Value]].
  return PromiseObject::unforgeableResolve(cx, rval);
||||||| merged common ancestors
    // Step 6: Otherwise, return a promise resolved with returnValue.[[Value]].
    return PromiseObject::unforgeableResolve(cx, returnValue);
=======
  *mode = unwrappedStream->mode();
  return true;
>>>>>>> upstream-releases
}

<<<<<<< HEAD
/**
 * Streams spec, 6.3.7. ValidateAndNormalizeHighWaterMark ( highWaterMark )
 */
static MOZ_MUST_USE bool ValidateAndNormalizeHighWaterMark(
    JSContext* cx, HandleValue highWaterMarkVal, double* highWaterMark) {
  // Step 1: Set highWaterMark to ? ToNumber(highWaterMark).
  if (!ToNumber(cx, highWaterMarkVal, highWaterMark)) {
    return false;
  }
||||||| merged common ancestors
// Streams spec, 6.4.5. ValidateAndNormalizeHighWaterMark ( highWaterMark )
static MOZ_MUST_USE bool
ValidateAndNormalizeHighWaterMark(JSContext* cx, HandleValue highWaterMarkVal, double* highWaterMark)
{
    // Step 1: Set highWaterMark to ? ToNumber(highWaterMark).
    if (!ToNumber(cx, highWaterMarkVal, highWaterMark)) {
        return false;
    }
=======
JS_PUBLIC_API JSObject* JS::ReadableStreamGetReader(
    JSContext* cx, HandleObject streamObj, ReadableStreamReaderMode mode) {
  AssertHeapIsIdle();
  CHECK_THREAD(cx);
>>>>>>> upstream-releases

<<<<<<< HEAD
  // Step 2: If highWaterMark is NaN or highWaterMark < 0, throw a RangeError
  // exception.
  if (mozilla::IsNaN(*highWaterMark) || *highWaterMark < 0) {
    JS_ReportErrorNumberASCII(cx, GetErrorMessage, nullptr,
                              JSMSG_STREAM_INVALID_HIGHWATERMARK);
    return false;
  }
||||||| merged common ancestors
    // Step 2: If highWaterMark is NaN, throw a TypeError exception.
    // Step 3: If highWaterMark < 0, throw a RangeError exception.
    if (mozilla::IsNaN(*highWaterMark) || *highWaterMark < 0) {
        JS_ReportErrorNumberASCII(cx, GetErrorMessage, nullptr, JSMSG_STREAM_INVALID_HIGHWATERMARK);
        return false;
    }
=======
  Rooted<ReadableStream*> unwrappedStream(
      cx, APIUnwrapAndDowncast<ReadableStream>(cx, streamObj));
  if (!unwrappedStream) {
    return nullptr;
  }
>>>>>>> upstream-releases

<<<<<<< HEAD
  // Step 3: Return highWaterMark.
  return true;
}

/**
 * Streams spec, 6.3.8. MakeSizeAlgorithmFromSizeFunction ( size )
 *
 * The standard makes a big deal of turning JavaScript functions (grubby,
 * touched by users, covered with germs) into algorithms (pristine,
 * respectable, purposeful). We don't bother. Here we only check for errors and
 * leave `size` unchanged. Then, in ReadableStreamDefaultControllerEnqueue,
 * where this value is used, we have to check for undefined and behave as if we
 * had "made" an "algorithm" as described below.
 */
static MOZ_MUST_USE bool MakeSizeAlgorithmFromSizeFunction(JSContext* cx,
                                                           HandleValue size) {
  // Step 1: If size is undefined, return an algorithm that returns 1.
  if (size.isUndefined()) {
    // Deferred. Size algorithm users must check for undefined.
    return true;
  }

  // Step 2: If ! IsCallable(size) is false, throw a TypeError exception.
  if (!IsCallable(size)) {
    JS_ReportErrorNumberASCII(cx, GetErrorMessage, nullptr, JSMSG_NOT_FUNCTION,
                              "ReadableStream argument options.size");
    return false;
  }

  // Step 3: Return an algorithm that performs the following steps, taking a
  //         chunk argument:
  //     a. Return ? Call(size, undefined, « chunk »).
  // Deferred. Size algorithm users must know how to call the size function.
  return true;
}

/*** API entry points *******************************************************/

JS_FRIEND_API JSObject* js::UnwrapReadableStream(JSObject* obj) {
  if (JSObject* unwrapped = CheckedUnwrap(obj)) {
    return unwrapped->is<ReadableStream>() ? unwrapped : nullptr;
  }
  return nullptr;
}

JS_PUBLIC_API JSObject* JS::NewReadableDefaultStreamObject(
    JSContext* cx, JS::HandleObject underlyingSource /* = nullptr */,
    JS::HandleFunction size /* = nullptr */, double highWaterMark /* = 1 */,
    JS::HandleObject proto /* = nullptr */) {
  MOZ_ASSERT(!cx->zone()->isAtomsZone());
  AssertHeapIsIdle();
  CHECK_THREAD(cx);
  cx->check(underlyingSource, size, proto);
  MOZ_ASSERT(highWaterMark >= 0);

  // A copy of ReadableStream::constructor, with most of the
  // argument-checking done implicitly by C++ type checking.
  Rooted<ReadableStream*> stream(cx, ReadableStream::create(cx));
  if (!stream) {
    return nullptr;
  }
  RootedValue sourceVal(cx);
  if (underlyingSource) {
    sourceVal.setObject(*underlyingSource);
  } else {
    JSObject* source = NewBuiltinClassInstance<PlainObject>(cx);
    if (!source) {
      return nullptr;
    }
    sourceVal.setObject(*source);
  }
  RootedValue sizeVal(cx, size ? ObjectValue(*size) : UndefinedValue());

  if (!SetUpReadableStreamDefaultControllerFromUnderlyingSource(
          cx, stream, sourceVal, highWaterMark, sizeVal)) {
    return nullptr;
  }

  return stream;
||||||| merged common ancestors
    // Step 4: Return highWaterMark.
    return true;
=======
  JSObject* result = CreateReadableStreamDefaultReader(cx, unwrappedStream);
  MOZ_ASSERT_IF(result, IsObjectInContextCompartment(result, cx));
  return result;
>>>>>>> upstream-releases
}

<<<<<<< HEAD
JS_PUBLIC_API JSObject* JS::NewReadableExternalSourceStreamObject(
    JSContext* cx, JS::ReadableStreamUnderlyingSource* underlyingSource,
    HandleObject proto /* = nullptr */) {
  MOZ_ASSERT(!cx->zone()->isAtomsZone());
  AssertHeapIsIdle();
  CHECK_THREAD(cx);
  MOZ_ASSERT(underlyingSource);
  MOZ_ASSERT((uintptr_t(underlyingSource) & 1) == 0,
             "external underlying source pointers must be aligned");
  cx->check(proto);
||||||| merged common ancestors
// Streams spec, 6.4.6. ValidateAndNormalizeQueuingStrategy ( size, highWaterMark )
static MOZ_MUST_USE bool
ValidateAndNormalizeQueuingStrategy(JSContext* cx, HandleValue size,
                                    HandleValue highWaterMarkVal, double* highWaterMark)
{
    // Step 1: If size is not undefined and ! IsCallable(size) is false, throw a
    //         TypeError exception.
    if (!size.isUndefined() && !IsCallable(size)) {
        JS_ReportErrorNumberASCII(cx, GetErrorMessage, nullptr, JSMSG_NOT_FUNCTION,
                                  "ReadableStream argument options.size");
        return false;
    }
=======
JS_PUBLIC_API bool JS::ReadableStreamGetExternalUnderlyingSource(
    JSContext* cx, HandleObject streamObj,
    JS::ReadableStreamUnderlyingSource** source) {
  AssertHeapIsIdle();
  CHECK_THREAD(cx);
>>>>>>> upstream-releases

<<<<<<< HEAD
  return ReadableStream::createExternalSourceStream(cx, underlyingSource,
                                                    proto);
}
||||||| merged common ancestors
    // Step 2: Let highWaterMark be ? ValidateAndNormalizeHighWaterMark(highWaterMark).
    if (!ValidateAndNormalizeHighWaterMark(cx, highWaterMarkVal, highWaterMark)) {
        return false;
    }
=======
  Rooted<ReadableStream*> unwrappedStream(
      cx, APIUnwrapAndDowncast<ReadableStream>(cx, streamObj));
  if (!unwrappedStream) {
    return false;
  }
>>>>>>> upstream-releases

<<<<<<< HEAD
JS_PUBLIC_API bool JS::IsReadableStream(JSObject* obj) {
  return obj->canUnwrapAs<ReadableStream>();
}
||||||| merged common ancestors
    // Step 3: Return Record {[[size]]: size, [[highWaterMark]]: highWaterMark}.
    return true;
}
=======
  MOZ_ASSERT(unwrappedStream->mode() == JS::ReadableStreamMode::ExternalSource);
  if (unwrappedStream->locked()) {
    JS_ReportErrorNumberASCII(cx, GetErrorMessage, nullptr,
                              JSMSG_READABLESTREAM_LOCKED);
    return false;
  }
  if (!unwrappedStream->readable()) {
    JS_ReportErrorNumberASCII(cx, GetErrorMessage, nullptr,
                              JSMSG_READABLESTREAMCONTROLLER_NOT_READABLE,
                              "ReadableStreamGetExternalUnderlyingSource");
    return false;
  }
>>>>>>> upstream-releases

<<<<<<< HEAD
JS_PUBLIC_API bool JS::IsReadableStreamReader(JSObject* obj) {
  return obj->canUnwrapAs<ReadableStreamDefaultReader>();
||||||| merged common ancestors
MOZ_MUST_USE bool
js::ReadableStreamReaderCancel(JSContext* cx, HandleObject readerObj, HandleValue reason)
{
    Rooted<ReadableStreamReader*> reader(cx, &readerObj->as<ReadableStreamReader>());
    ReadableStream* stream = StreamFromReader(cx, reader);
    if (!stream) {
        return false;
    }
    return ReadableStreamReaderGenericCancel(cx, reader, reason);
=======
  auto unwrappedController =
      &unwrappedStream->controller()->as<ReadableByteStreamController>();
  unwrappedController->setSourceLocked();
  *source = unwrappedController->externalSource();
  return true;
>>>>>>> upstream-releases
}

<<<<<<< HEAD
JS_PUBLIC_API bool JS::IsReadableStreamDefaultReader(JSObject* obj) {
  return obj->canUnwrapAs<ReadableStreamDefaultReader>();
||||||| merged common ancestors
MOZ_MUST_USE bool
js::ReadableStreamReaderReleaseLock(JSContext* cx, HandleObject readerObj)
{
    Rooted<ReadableStreamReader*> reader(cx, &readerObj->as<ReadableStreamReader>());
    ReadableStream* stream = StreamFromReader(cx, reader);
    if (!stream) {
        return false;
    }
    MOZ_ASSERT(ReadableStreamGetNumReadRequests(stream) == 0);
    return ReadableStreamReaderGenericRelease(cx, reader);
=======
JS_PUBLIC_API bool JS::ReadableStreamReleaseExternalUnderlyingSource(
    JSContext* cx, HandleObject streamObj) {
  ReadableStream* unwrappedStream =
      APIUnwrapAndDowncast<ReadableStream>(cx, streamObj);
  if (!unwrappedStream) {
    return false;
  }

  MOZ_ASSERT(unwrappedStream->mode() == JS::ReadableStreamMode::ExternalSource);
  MOZ_ASSERT(unwrappedStream->locked());
  MOZ_ASSERT(unwrappedStream->controller()->sourceLocked());
  unwrappedStream->controller()->clearSourceLocked();
  return true;
>>>>>>> upstream-releases
}

<<<<<<< HEAD
template <class T>
static MOZ_MUST_USE T* APIUnwrapAndDowncast(JSContext* cx, JSObject* obj) {
  cx->check(obj);
  return UnwrapAndDowncastObject<T>(cx, obj);
}
||||||| merged common ancestors
MOZ_MUST_USE bool
ReadableStream::enqueue(JSContext* cx, Handle<ReadableStream*> stream, HandleValue chunk)
{
    Rooted<ReadableStreamDefaultController*> controller(cx);
    controller = &ControllerFromStream(stream)->as<ReadableStreamDefaultController>();
=======
JS_PUBLIC_API bool JS::ReadableStreamUpdateDataAvailableFromSource(
    JSContext* cx, JS::HandleObject streamObj, uint32_t availableData) {
  AssertHeapIsIdle();
  CHECK_THREAD(cx);
>>>>>>> upstream-releases

<<<<<<< HEAD
JS_PUBLIC_API bool JS::ReadableStreamIsReadable(JSContext* cx,
                                                HandleObject streamObj,
                                                bool* result) {
  ReadableStream* unwrappedStream =
      APIUnwrapAndDowncast<ReadableStream>(cx, streamObj);
  if (!unwrappedStream) {
    return false;
  }
||||||| merged common ancestors
    MOZ_ASSERT(!(ControllerFlags(controller) & ControllerFlag_CloseRequested));
    MOZ_ASSERT(stream->readable());
=======
  Rooted<ReadableStream*> unwrappedStream(
      cx, APIUnwrapAndDowncast<ReadableStream>(cx, streamObj));
  if (!unwrappedStream) {
    return false;
  }

  // This is based on Streams spec 3.10.4.4. enqueue(chunk) steps 1-3 and
  // 3.12.9. ReadableByteStreamControllerEnqueue(controller, chunk) steps
  // 8-9.
  //
  // Adapted to handling updates signaled by the embedding for streams with
  // external underlying sources.
  //
  // The remaining steps of those two functions perform checks and asserts
  // that don't apply to streams with external underlying sources.

  Rooted<ReadableByteStreamController*> unwrappedController(
      cx, &unwrappedStream->controller()->as<ReadableByteStreamController>());

  // Step 2: If this.[[closeRequested]] is true, throw a TypeError exception.
  if (unwrappedController->closeRequested()) {
    JS_ReportErrorNumberASCII(cx, GetErrorMessage, nullptr,
                              JSMSG_READABLESTREAMCONTROLLER_CLOSED, "enqueue");
    return false;
  }
>>>>>>> upstream-releases

<<<<<<< HEAD
  *result = unwrappedStream->readable();
  return true;
}
||||||| merged common ancestors
    return ReadableStreamDefaultControllerEnqueue(cx, controller, chunk);
}
=======
  // Step 3: If this.[[controlledReadableStream]].[[state]] is not "readable",
  //         throw a TypeError exception.
  if (!unwrappedController->stream()->readable()) {
    JS_ReportErrorNumberASCII(cx, GetErrorMessage, nullptr,
                              JSMSG_READABLESTREAMCONTROLLER_NOT_READABLE,
                              "enqueue");
    return false;
  }
>>>>>>> upstream-releases

<<<<<<< HEAD
JS_PUBLIC_API bool JS::ReadableStreamIsLocked(JSContext* cx,
                                              HandleObject streamObj,
                                              bool* result) {
  ReadableStream* unwrappedStream =
      APIUnwrapAndDowncast<ReadableStream>(cx, streamObj);
  if (!unwrappedStream) {
    return false;
  }
||||||| merged common ancestors
void
ReadableStream::desiredSize(bool* hasSize, double* size) const
{
    if (errored()) {
        *hasSize = false;
        return;
    }
=======
  unwrappedController->clearPullFlags();
>>>>>>> upstream-releases

<<<<<<< HEAD
  *result = unwrappedStream->locked();
  return true;
}
||||||| merged common ancestors
    *hasSize = true;
=======
#if DEBUG
  uint32_t oldAvailableData =
      unwrappedController->getFixedSlot(StreamController::Slot_TotalSize)
          .toInt32();
#endif  // DEBUG
  unwrappedController->setQueueTotalSize(availableData);

  // 3.12.9. ReadableByteStreamControllerEnqueue
  // Step 8.a: If ! ReadableStreamGetNumReadRequests(stream) is 0,
  // Reordered because for externally-sourced streams it applies regardless
  // of reader type.
  if (ReadableStreamGetNumReadRequests(unwrappedStream) == 0) {
    return true;
  }
>>>>>>> upstream-releases

<<<<<<< HEAD
JS_PUBLIC_API bool JS::ReadableStreamIsDisturbed(JSContext* cx,
                                                 HandleObject streamObj,
                                                 bool* result) {
  ReadableStream* unwrappedStream =
      APIUnwrapAndDowncast<ReadableStream>(cx, streamObj);
  if (!unwrappedStream) {
    return false;
  }
||||||| merged common ancestors
    if (closed()) {
        *size = 0;
        return;
    }
=======
  // Step 8: If ! ReadableStreamHasDefaultReader(stream) is true
  bool hasDefaultReader;
  if (!ReadableStreamHasDefaultReader(cx, unwrappedStream, &hasDefaultReader)) {
    return false;
  }
  if (hasDefaultReader) {
    // Step b: Otherwise,
    // Step i: Assert: controller.[[queue]] is empty.
    MOZ_ASSERT(oldAvailableData == 0);

    // Step ii: Let transferredView be
    //          ! Construct(%Uint8Array%, transferredBuffer,
    //                      byteOffset, byteLength).
    JSObject* viewObj = JS_NewUint8Array(cx, availableData);
    if (!viewObj) {
      return false;
    }
    Rooted<ArrayBufferViewObject*> transferredView(
        cx, &viewObj->as<ArrayBufferViewObject>());
    if (!transferredView) {
      return false;
    }
>>>>>>> upstream-releases

<<<<<<< HEAD
  *result = unwrappedStream->disturbed();
  return true;
}
||||||| merged common ancestors
    ReadableStreamController* controller = ControllerFromStream(this);
    *size = ReadableStreamControllerGetDesiredSizeUnchecked(controller);
}
=======
    JS::ReadableStreamUnderlyingSource* source =
        unwrappedController->externalSource();
>>>>>>> upstream-releases

<<<<<<< HEAD
JS_PUBLIC_API JSObject* JS::ReadableStreamCancel(JSContext* cx,
                                                 HandleObject streamObj,
                                                 HandleValue reason) {
  AssertHeapIsIdle();
  CHECK_THREAD(cx);
  cx->check(reason);
||||||| merged common ancestors
/*static */ bool
ReadableStream::getExternalSource(JSContext* cx, Handle<ReadableStream*> stream, void** source)
{
    MOZ_ASSERT(stream->mode() == JS::ReadableStreamMode::ExternalSource);
    if (stream->locked()) {
        JS_ReportErrorNumberASCII(cx, GetErrorMessage, nullptr, JSMSG_READABLESTREAM_LOCKED);
        return false;
    }
    if (!stream->readable()) {
        JS_ReportErrorNumberASCII(cx, GetErrorMessage, nullptr,
                                  JSMSG_READABLESTREAMCONTROLLER_NOT_READABLE,
                                  "ReadableStreamGetExternalUnderlyingSource");
        return false;
    }
=======
    size_t bytesWritten;
    {
      AutoRealm ar(cx, unwrappedStream);
      JS::AutoSuppressGCAnalysis suppressGC(cx);
      JS::AutoCheckCannotGC noGC;
      bool dummy;
      void* buffer = JS_GetArrayBufferViewData(transferredView, &dummy, noGC);
      source->writeIntoReadRequestBuffer(cx, unwrappedStream, buffer,
                                         availableData, &bytesWritten);
    }

    // Step iii: Perform ! ReadableStreamFulfillReadRequest(stream,
    //                                                      transferredView,
    //                                                      false).
    RootedValue chunk(cx, ObjectValue(*transferredView));
    if (!ReadableStreamFulfillReadOrReadIntoRequest(cx, unwrappedStream, chunk,
                                                    false)) {
      return false;
    }
>>>>>>> upstream-releases

<<<<<<< HEAD
  Rooted<ReadableStream*> unwrappedStream(
      cx, APIUnwrapAndDowncast<ReadableStream>(cx, streamObj));
  if (!unwrappedStream) {
    return nullptr;
  }

  return ::ReadableStreamCancel(cx, unwrappedStream, reason);
||||||| merged common ancestors
    auto controller = &ControllerFromStream(stream)->as<ReadableByteStreamController>();
    AddControllerFlags(controller, ControllerFlag_SourceLocked);
    *source = controller->getFixedSlot(ControllerSlot_UnderlyingSource).toPrivate();
    return true;
=======
    unwrappedController->setQueueTotalSize(availableData - bytesWritten);
  } else {
    // Step 9: Otherwise, if ! ReadableStreamHasBYOBReader(stream) is true,
    //         [...]
    // (Omitted. BYOB readers are not implemented.)

    // Step 10: Otherwise,
    // Step a: Assert: ! IsReadableStreamLocked(stream) is false.
    MOZ_ASSERT(!unwrappedStream->locked());

    // Step b: Perform ! ReadableByteStreamControllerEnqueueChunkToQueue(
    //         controller, transferredBuffer, byteOffset, byteLength).
    // (Not needed for external underlying sources.)
  }

  return true;
>>>>>>> upstream-releases
}

<<<<<<< HEAD
JS_PUBLIC_API bool JS::ReadableStreamGetMode(JSContext* cx,
                                             HandleObject streamObj,
                                             JS::ReadableStreamMode* mode) {
  ReadableStream* unwrappedStream =
      APIUnwrapAndDowncast<ReadableStream>(cx, streamObj);
  if (!unwrappedStream) {
    return false;
  }

  *mode = unwrappedStream->mode();
  return true;
||||||| merged common ancestors
void
ReadableStream::releaseExternalSource()
{
    MOZ_ASSERT(mode() == JS::ReadableStreamMode::ExternalSource);
    MOZ_ASSERT(locked());
    auto controller = ControllerFromStream(this);
    MOZ_ASSERT(ControllerFlags(controller) & ControllerFlag_SourceLocked);
    RemoveControllerFlags(controller, ControllerFlag_SourceLocked);
=======
JS_PUBLIC_API void JS::ReadableStreamReleaseCCObject(JSObject* streamObj) {
  MOZ_ASSERT(JS::IsReadableStream(streamObj));
  JS_SetPrivate(streamObj, nullptr);
>>>>>>> upstream-releases
}

<<<<<<< HEAD
JS_PUBLIC_API JSObject* JS::ReadableStreamGetReader(
    JSContext* cx, HandleObject streamObj, ReadableStreamReaderMode mode) {
  AssertHeapIsIdle();
  CHECK_THREAD(cx);

  Rooted<ReadableStream*> unwrappedStream(
      cx, APIUnwrapAndDowncast<ReadableStream>(cx, streamObj));
  if (!unwrappedStream) {
    return nullptr;
  }

  JSObject* result = CreateReadableStreamDefaultReader(cx, unwrappedStream);
  MOZ_ASSERT_IF(result, IsObjectInContextCompartment(result, cx));
  return result;
||||||| merged common ancestors
uint8_t
ReadableStream::embeddingFlags() const
{
    uint8_t flags = ControllerFlags(ControllerFromStream(this)) >> ControllerEmbeddingFlagsOffset;
    MOZ_ASSERT_IF(flags, mode() == JS::ReadableStreamMode::ExternalSource);
    return flags;
=======
JS_PUBLIC_API bool JS::ReadableStreamTee(JSContext* cx, HandleObject streamObj,
                                         MutableHandleObject branch1Obj,
                                         MutableHandleObject branch2Obj) {
  AssertHeapIsIdle();
  CHECK_THREAD(cx);

  Rooted<ReadableStream*> unwrappedStream(
      cx, APIUnwrapAndDowncast<ReadableStream>(cx, streamObj));
  if (!unwrappedStream) {
    return false;
  }

  Rooted<ReadableStream*> branch1Stream(cx);
  Rooted<ReadableStream*> branch2Stream(cx);
  if (!ReadableStreamTee(cx, unwrappedStream, false, &branch1Stream,
                         &branch2Stream)) {
    return false;
  }

  branch1Obj.set(branch1Stream);
  branch2Obj.set(branch2Stream);
  return true;
>>>>>>> upstream-releases
}

<<<<<<< HEAD
JS_PUBLIC_API bool JS::ReadableStreamGetExternalUnderlyingSource(
    JSContext* cx, HandleObject streamObj,
    JS::ReadableStreamUnderlyingSource** source) {
  AssertHeapIsIdle();
  CHECK_THREAD(cx);
||||||| merged common ancestors
// Streams spec, 3.10.4.4. steps 1-3
// and
// Streams spec, 3.12.8. steps 8-9
//
// Adapted to handling updates signaled by the embedding for streams with
// external underlying sources.
//
// The remaining steps of those two functions perform checks and asserts that
// don't apply to streams with external underlying sources.
MOZ_MUST_USE bool
ReadableStream::updateDataAvailableFromSource(JSContext* cx, Handle<ReadableStream*> stream,
                                              uint32_t availableData)
{
    Rooted<ReadableByteStreamController*> controller(cx);
    controller = &ControllerFromStream(stream)->as<ReadableByteStreamController>();
=======
JS_PUBLIC_API bool JS::ReadableStreamGetDesiredSize(JSContext* cx,
                                                    JSObject* streamObj,
                                                    bool* hasValue,
                                                    double* value) {
  ReadableStream* unwrappedStream =
      APIUnwrapAndDowncast<ReadableStream>(cx, streamObj);
  if (!unwrappedStream) {
    return false;
  }
>>>>>>> upstream-releases

<<<<<<< HEAD
  Rooted<ReadableStream*> unwrappedStream(
      cx, APIUnwrapAndDowncast<ReadableStream>(cx, streamObj));
  if (!unwrappedStream) {
    return false;
  }
||||||| merged common ancestors
    // Step 2: If this.[[closeRequested]] is true, throw a TypeError exception.
    if (ControllerFlags(controller) & ControllerFlag_CloseRequested) {
        JS_ReportErrorNumberASCII(cx, GetErrorMessage, nullptr,
                                  JSMSG_READABLESTREAMCONTROLLER_CLOSED, "enqueue");
        return false;
    }
=======
  if (unwrappedStream->errored()) {
    *hasValue = false;
    return true;
  }
>>>>>>> upstream-releases

<<<<<<< HEAD
  MOZ_ASSERT(unwrappedStream->mode() == JS::ReadableStreamMode::ExternalSource);
  if (unwrappedStream->locked()) {
    JS_ReportErrorNumberASCII(cx, GetErrorMessage, nullptr,
                              JSMSG_READABLESTREAM_LOCKED);
    return false;
  }
  if (!unwrappedStream->readable()) {
    JS_ReportErrorNumberASCII(cx, GetErrorMessage, nullptr,
                              JSMSG_READABLESTREAMCONTROLLER_NOT_READABLE,
                              "ReadableStreamGetExternalUnderlyingSource");
    return false;
  }
||||||| merged common ancestors
    // Step 3: If this.[[controlledReadableStream]].[[state]] is not "readable",
    //         throw a TypeError exception.
    if (!StreamFromController(controller)->readable()) {
        JS_ReportErrorNumberASCII(cx, GetErrorMessage, nullptr,
                                  JSMSG_READABLESTREAMCONTROLLER_NOT_READABLE, "enqueue");
        return false;
    }
=======
  *hasValue = true;
>>>>>>> upstream-releases

<<<<<<< HEAD
  auto unwrappedController =
      &unwrappedStream->controller()->as<ReadableByteStreamController>();
  unwrappedController->setSourceLocked();
  *source = unwrappedController->externalSource();
  return true;
}

JS_PUBLIC_API bool JS::ReadableStreamReleaseExternalUnderlyingSource(
    JSContext* cx, HandleObject streamObj) {
  ReadableStream* unwrappedStream =
      APIUnwrapAndDowncast<ReadableStream>(cx, streamObj);
  if (!unwrappedStream) {
    return false;
  }

  MOZ_ASSERT(unwrappedStream->mode() == JS::ReadableStreamMode::ExternalSource);
  MOZ_ASSERT(unwrappedStream->locked());
  MOZ_ASSERT(unwrappedStream->controller()->sourceLocked());
  unwrappedStream->controller()->clearSourceLocked();
  return true;
}

JS_PUBLIC_API bool JS::ReadableStreamUpdateDataAvailableFromSource(
    JSContext* cx, JS::HandleObject streamObj, uint32_t availableData) {
  AssertHeapIsIdle();
  CHECK_THREAD(cx);

  Rooted<ReadableStream*> unwrappedStream(
      cx, APIUnwrapAndDowncast<ReadableStream>(cx, streamObj));
  if (!unwrappedStream) {
    return false;
  }

  // This is based on Streams spec 3.10.4.4. enqueue(chunk) steps 1-3 and
  // 3.12.9. ReadableByteStreamControllerEnqueue(controller, chunk) steps
  // 8-9.
  //
  // Adapted to handling updates signaled by the embedding for streams with
  // external underlying sources.
  //
  // The remaining steps of those two functions perform checks and asserts
  // that don't apply to streams with external underlying sources.

  Rooted<ReadableByteStreamController*> unwrappedController(
      cx, &unwrappedStream->controller()->as<ReadableByteStreamController>());

  // Step 2: If this.[[closeRequested]] is true, throw a TypeError exception.
  if (unwrappedController->closeRequested()) {
    JS_ReportErrorNumberASCII(cx, GetErrorMessage, nullptr,
                              JSMSG_READABLESTREAMCONTROLLER_CLOSED, "enqueue");
    return false;
  }

  // Step 3: If this.[[controlledReadableStream]].[[state]] is not "readable",
  //         throw a TypeError exception.
  if (!unwrappedController->stream()->readable()) {
    JS_ReportErrorNumberASCII(cx, GetErrorMessage, nullptr,
                              JSMSG_READABLESTREAMCONTROLLER_NOT_READABLE,
                              "enqueue");
    return false;
  }

  unwrappedController->clearPullFlags();
||||||| merged common ancestors
    RemoveControllerFlags(controller, ControllerFlag_Pulling | ControllerFlag_PullAgain);
=======
  if (unwrappedStream->closed()) {
    *value = 0;
    return true;
  }
>>>>>>> upstream-releases

<<<<<<< HEAD
#if DEBUG
  uint32_t oldAvailableData =
      unwrappedController->getFixedSlot(StreamController::Slot_TotalSize)
          .toInt32();
#endif  // DEBUG
  unwrappedController->setQueueTotalSize(availableData);

  // 3.12.9. ReadableByteStreamControllerEnqueue
  // Step 8.a: If ! ReadableStreamGetNumReadRequests(stream) is 0,
  // Reordered because for externally-sourced streams it applies regardless
  // of reader type.
  if (ReadableStreamGetNumReadRequests(unwrappedStream) == 0) {
    return true;
  }

  // Step 8: If ! ReadableStreamHasDefaultReader(stream) is true
  bool hasDefaultReader;
  if (!ReadableStreamHasDefaultReader(cx, unwrappedStream, &hasDefaultReader)) {
    return false;
  }
  if (hasDefaultReader) {
    // Step b: Otherwise,
    // Step i: Assert: controller.[[queue]] is empty.
    MOZ_ASSERT(oldAvailableData == 0);

    // Step ii: Let transferredView be
    //          ! Construct(%Uint8Array%, transferredBuffer,
    //                      byteOffset, byteLength).
    JSObject* viewObj = JS_NewUint8Array(cx, availableData);
    if (!viewObj) {
      return false;
    }
    Rooted<ArrayBufferViewObject*> transferredView(
        cx, &viewObj->as<ArrayBufferViewObject>());
    if (!transferredView) {
      return false;
    }
||||||| merged common ancestors
#if DEBUG
    uint32_t oldAvailableData = controller->getFixedSlot(QueueContainerSlot_TotalSize).toInt32();
#endif // DEBUG
    SetQueueSize(controller, availableData);

    // Step 8.a: If ! ReadableStreamGetNumReadRequests(stream) is 0,
    // Reordered because for externally-sourced streams it applies regardless
    // of reader type.
    if (ReadableStreamGetNumReadRequests(stream) == 0) {
        return true;
    }
=======
  *value = ReadableStreamControllerGetDesiredSizeUnchecked(
      unwrappedStream->controller());
  return true;
}
>>>>>>> upstream-releases

<<<<<<< HEAD
    JS::ReadableStreamUnderlyingSource* source =
        unwrappedController->externalSource();

    size_t bytesWritten;
    {
      AutoRealm ar(cx, unwrappedStream);
      JS::AutoSuppressGCAnalysis suppressGC(cx);
      JS::AutoCheckCannotGC noGC;
      bool dummy;
      void* buffer = JS_GetArrayBufferViewData(transferredView, &dummy, noGC);
      source->writeIntoReadRequestBuffer(cx, unwrappedStream, buffer,
                                         availableData, &bytesWritten);
    }

    // Step iii: Perform ! ReadableStreamFulfillReadRequest(stream,
    //                                                      transferredView,
    //                                                      false).
    RootedValue chunk(cx, ObjectValue(*transferredView));
    if (!ReadableStreamFulfillReadOrReadIntoRequest(cx, unwrappedStream, chunk,
                                                    false)) {
      return false;
    }
||||||| merged common ancestors
    // Step 8: If ! ReadableStreamHasDefaultReader(stream) is true
    ReaderMode readerMode;
    if (!ReadableStreamGetReaderMode(cx, stream, &readerMode)) {
        return false;
    }
=======
JS_PUBLIC_API bool JS::ReadableStreamClose(JSContext* cx,
                                           HandleObject streamObj) {
  AssertHeapIsIdle();
  CHECK_THREAD(cx);
>>>>>>> upstream-releases

<<<<<<< HEAD
    unwrappedController->setQueueTotalSize(availableData - bytesWritten);
  } else {
    // Step b: Otherwise,
    // Step i: Assert: ! IsReadableStreamLocked(stream) is false.
    MOZ_ASSERT(!unwrappedStream->locked());
||||||| merged common ancestors
    if (readerMode == ReaderMode::Default) {
        // Step b: Otherwise,
        // Step i: Assert: controller.[[queue]] is empty.
        MOZ_ASSERT(oldAvailableData == 0);

        // Step ii: Let transferredView be
        //          ! Construct(%Uint8Array%, transferredBuffer, byteOffset, byteLength).
        JSObject* viewObj = JS_NewUint8Array(cx, availableData);
        Rooted<ArrayBufferViewObject*> transferredView(cx, &viewObj->as<ArrayBufferViewObject>());
        if (!transferredView) {
            return false;
        }
=======
  Rooted<ReadableStream*> unwrappedStream(
      cx, APIUnwrapAndDowncast<ReadableStream>(cx, streamObj));
  if (!unwrappedStream) {
    return false;
  }
>>>>>>> upstream-releases

<<<<<<< HEAD
    // Step ii: Perform ! ReadableByteStreamControllerEnqueueChunkToQueue(
    //              controller, transferredBuffer, byteOffset, byteLength).
    // (Not needed for external underlying sources.)
  }
||||||| merged common ancestors
        Value val = controller->getFixedSlot(ControllerSlot_UnderlyingSource);
        void* underlyingSource = val.toPrivate();

        size_t bytesWritten;
        {
            JS::AutoSuppressGCAnalysis suppressGC(cx);
            JS::AutoCheckCannotGC noGC;
            bool dummy;
            void* buffer = JS_GetArrayBufferViewData(transferredView, &dummy, noGC);
            auto cb = cx->runtime()->readableStreamWriteIntoReadRequestCallback;
            MOZ_ASSERT(cb);
            // TODO: use bytesWritten to correctly update the request's state.
            // TODO: make cross-compartment safe.
            cb(cx, stream, underlyingSource, stream->embeddingFlags(), buffer,
               availableData, &bytesWritten);
        }
=======
  Rooted<ReadableStreamController*> unwrappedControllerObj(
      cx, unwrappedStream->controller());
  if (!CheckReadableStreamControllerCanCloseOrEnqueue(
          cx, unwrappedControllerObj, "close")) {
    return false;
  }

  if (unwrappedControllerObj->is<ReadableStreamDefaultController>()) {
    Rooted<ReadableStreamDefaultController*> unwrappedController(cx);
    unwrappedController =
        &unwrappedControllerObj->as<ReadableStreamDefaultController>();
    return ReadableStreamDefaultControllerClose(cx, unwrappedController);
  }

  Rooted<ReadableByteStreamController*> unwrappedController(cx);
  unwrappedController =
      &unwrappedControllerObj->as<ReadableByteStreamController>();
  return ReadableByteStreamControllerClose(cx, unwrappedController);
}

JS_PUBLIC_API bool JS::ReadableStreamEnqueue(JSContext* cx,
                                             HandleObject streamObj,
                                             HandleValue chunk) {
  AssertHeapIsIdle();
  CHECK_THREAD(cx);
  cx->check(chunk);

  Rooted<ReadableStream*> unwrappedStream(
      cx, APIUnwrapAndDowncast<ReadableStream>(cx, streamObj));
  if (!unwrappedStream) {
    return false;
  }
>>>>>>> upstream-releases

<<<<<<< HEAD
  return true;
}
||||||| merged common ancestors
        // Step iii: Perform ! ReadableStreamFulfillReadRequest(stream, transferredView, false).
        RootedValue chunk(cx, ObjectValue(*transferredView));
        if (!ReadableStreamFulfillReadOrReadIntoRequest(cx, stream, chunk, false)) {
            return false;
        }
=======
  if (unwrappedStream->mode() != JS::ReadableStreamMode::Default) {
    JS_ReportErrorNumberASCII(cx, GetErrorMessage, nullptr,
                              JSMSG_READABLESTREAM_NOT_DEFAULT_CONTROLLER,
                              "JS::ReadableStreamEnqueue");
    return false;
  }
>>>>>>> upstream-releases

<<<<<<< HEAD
JS_PUBLIC_API bool JS::ReadableStreamTee(JSContext* cx, HandleObject streamObj,
                                         MutableHandleObject branch1Obj,
                                         MutableHandleObject branch2Obj) {
  AssertHeapIsIdle();
  CHECK_THREAD(cx);

  Rooted<ReadableStream*> unwrappedStream(
      cx, APIUnwrapAndDowncast<ReadableStream>(cx, streamObj));
  if (!unwrappedStream) {
    return false;
  }
||||||| merged common ancestors
        SetQueueSize(controller, availableData - bytesWritten);
    } else {
        // Step b: Otherwise,
        // Step i: Assert: ! IsReadableStreamLocked(stream) is false.
        MOZ_ASSERT(!stream->locked());

        // Step ii: Perform
        //          ! ReadableByteStreamControllerEnqueueChunkToQueue(controller,
        //                                                            transferredBuffer,
        //                                                            byteOffset,
        //                                                            byteLength).
        // (Not needed for external underlying sources.)
    }
=======
  Rooted<ReadableStreamDefaultController*> unwrappedController(cx);
  unwrappedController =
      &unwrappedStream->controller()->as<ReadableStreamDefaultController>();
>>>>>>> upstream-releases

<<<<<<< HEAD
  Rooted<ReadableStream*> branch1Stream(cx);
  Rooted<ReadableStream*> branch2Stream(cx);
  if (!ReadableStreamTee(cx, unwrappedStream, false, &branch1Stream,
                         &branch2Stream)) {
    return false;
  }

  branch1Obj.set(branch1Stream);
  branch2Obj.set(branch2Stream);

  return true;
}

JS_PUBLIC_API bool JS::ReadableStreamGetDesiredSize(JSContext* cx,
                                                    JSObject* streamObj,
                                                    bool* hasValue,
                                                    double* value) {
  ReadableStream* unwrappedStream =
      APIUnwrapAndDowncast<ReadableStream>(cx, streamObj);
  if (!unwrappedStream) {
    return false;
  }

  if (unwrappedStream->errored()) {
    *hasValue = false;
    return true;
  }

  *hasValue = true;

  if (unwrappedStream->closed()) {
    *value = 0;
    return true;
  }

  *value = ReadableStreamControllerGetDesiredSizeUnchecked(
      unwrappedStream->controller());
  return true;
||||||| merged common ancestors
    return true;
=======
  MOZ_ASSERT(!unwrappedController->closeRequested());
  MOZ_ASSERT(unwrappedStream->readable());

  return ReadableStreamDefaultControllerEnqueue(cx, unwrappedController, chunk);
>>>>>>> upstream-releases
}

<<<<<<< HEAD
JS_PUBLIC_API bool JS::ReadableStreamClose(JSContext* cx,
                                           HandleObject streamObj) {
  AssertHeapIsIdle();
  CHECK_THREAD(cx);
||||||| merged common ancestors
MOZ_MUST_USE bool
ReadableStream::close(JSContext* cx, Handle<ReadableStream*> stream)
{
    Rooted<ReadableStreamController*> controllerObj(cx, ControllerFromStream(stream));
    if (!VerifyControllerStateForClosing(cx, controllerObj)) {
        return false;
    }
=======
JS_PUBLIC_API bool JS::ReadableStreamError(JSContext* cx,
                                           HandleObject streamObj,
                                           HandleValue error) {
  AssertHeapIsIdle();
  CHECK_THREAD(cx);
  cx->check(error);
>>>>>>> upstream-releases

  Rooted<ReadableStream*> unwrappedStream(
      cx, APIUnwrapAndDowncast<ReadableStream>(cx, streamObj));
  if (!unwrappedStream) {
    return false;
  }

<<<<<<< HEAD
  Rooted<ReadableStreamController*> unwrappedControllerObj(
      cx, unwrappedStream->controller());
  if (!VerifyControllerStateForClosing(cx, unwrappedControllerObj)) {
    return false;
  }

  if (unwrappedControllerObj->is<ReadableStreamDefaultController>()) {
    Rooted<ReadableStreamDefaultController*> unwrappedController(cx);
    unwrappedController =
        &unwrappedControllerObj->as<ReadableStreamDefaultController>();
    return ReadableStreamDefaultControllerClose(cx, unwrappedController);
  }

  Rooted<ReadableByteStreamController*> unwrappedController(cx);
  unwrappedController =
      &unwrappedControllerObj->as<ReadableByteStreamController>();
  return ReadableByteStreamControllerClose(cx, unwrappedController);
}

JS_PUBLIC_API bool JS::ReadableStreamEnqueue(JSContext* cx,
                                             HandleObject streamObj,
                                             HandleValue chunk) {
  AssertHeapIsIdle();
  CHECK_THREAD(cx);
  cx->check(chunk);

  Rooted<ReadableStream*> unwrappedStream(
      cx, APIUnwrapAndDowncast<ReadableStream>(cx, streamObj));
  if (!unwrappedStream) {
    return false;
  }

  if (unwrappedStream->mode() != JS::ReadableStreamMode::Default) {
    JS_ReportErrorNumberASCII(cx, GetErrorMessage, nullptr,
                              JSMSG_READABLESTREAM_NOT_DEFAULT_CONTROLLER,
                              "JS::ReadableStreamEnqueue");
    return false;
  }

  Rooted<ReadableStreamDefaultController*> unwrappedController(cx);
  unwrappedController =
      &unwrappedStream->controller()->as<ReadableStreamDefaultController>();

  MOZ_ASSERT(!unwrappedController->closeRequested());
  MOZ_ASSERT(unwrappedStream->readable());

  return ReadableStreamDefaultControllerEnqueue(cx, unwrappedController, chunk);
||||||| merged common ancestors
    Rooted<ReadableByteStreamController*> controller(cx);
    controller = &controllerObj->as<ReadableByteStreamController>();
    return ReadableByteStreamControllerClose(cx, controller);
=======
  Rooted<ReadableStreamController*> unwrappedController(
      cx, unwrappedStream->controller());
  return ReadableStreamControllerError(cx, unwrappedController, error);
>>>>>>> upstream-releases
}

<<<<<<< HEAD
JS_PUBLIC_API bool JS::ReadableStreamError(JSContext* cx,
                                           HandleObject streamObj,
                                           HandleValue error) {
  AssertHeapIsIdle();
  CHECK_THREAD(cx);
  cx->check(error);

  Rooted<ReadableStream*> unwrappedStream(
      cx, APIUnwrapAndDowncast<ReadableStream>(cx, streamObj));
  if (!unwrappedStream) {
    return false;
  }
||||||| merged common ancestors
MOZ_MUST_USE bool
ReadableStream::error(JSContext* cx, Handle<ReadableStream*> stream, HandleValue reason)
{
    // Step 3: If stream.[[state]] is not "readable", throw a TypeError exception.
    if (!stream->readable()) {
        JS_ReportErrorNumberASCII(cx, GetErrorMessage, nullptr,
                                  JSMSG_READABLESTREAMCONTROLLER_NOT_READABLE, "error");
        return false;
    }
=======
JS_PUBLIC_API bool JS::ReadableStreamReaderIsClosed(JSContext* cx,
                                                    HandleObject readerObj,
                                                    bool* result) {
  Rooted<ReadableStreamReader*> unwrappedReader(
      cx, APIUnwrapAndDowncast<ReadableStreamReader>(cx, readerObj));
  if (!unwrappedReader) {
    return false;
  }
>>>>>>> upstream-releases

<<<<<<< HEAD
  Rooted<ReadableStreamController*> unwrappedController(
      cx, unwrappedStream->controller());
  return ReadableStreamControllerError(cx, unwrappedController, error);
||||||| merged common ancestors
    // Step 4: Perform ! ReadableStreamDefaultControllerError(this, e).
    Rooted<ReadableStreamController*> controller(cx, ControllerFromStream(stream));
    return ReadableStreamControllerError(cx, controller, reason);
=======
  *result = unwrappedReader->isClosed();
  return true;
>>>>>>> upstream-releases
}

<<<<<<< HEAD
JS_PUBLIC_API bool JS::ReadableStreamReaderIsClosed(JSContext* cx,
                                                    HandleObject readerObj,
                                                    bool* result) {
  Rooted<ReadableStreamReader*> unwrappedReader(
      cx, APIUnwrapAndDowncast<ReadableStreamReader>(cx, readerObj));
  if (!unwrappedReader) {
    return false;
  }

  *result = unwrappedReader->isClosed();
  return true;
||||||| merged common ancestors
MOZ_MUST_USE bool
ReadableStream::tee(JSContext* cx, Handle<ReadableStream*> stream, bool cloneForBranch2,
                    MutableHandle<ReadableStream*> branch1Stream,
                    MutableHandle<ReadableStream*> branch2Stream)
{
    return ReadableStreamTee(cx, stream, false, branch1Stream, branch2Stream);
=======
JS_PUBLIC_API bool JS::ReadableStreamReaderCancel(JSContext* cx,
                                                  HandleObject readerObj,
                                                  HandleValue reason) {
  AssertHeapIsIdle();
  CHECK_THREAD(cx);
  cx->check(reason);

  Rooted<ReadableStreamReader*> unwrappedReader(
      cx, APIUnwrapAndDowncast<ReadableStreamReader>(cx, readerObj));
  if (!unwrappedReader) {
    return false;
  }
  MOZ_ASSERT(unwrappedReader->forAuthorCode() == ForAuthorCodeBool::No,
             "C++ code should not touch readers created by scripts");

  return ReadableStreamReaderGenericCancel(cx, unwrappedReader, reason);
>>>>>>> upstream-releases
}

<<<<<<< HEAD
JS_PUBLIC_API bool JS::ReadableStreamReaderCancel(JSContext* cx,
                                                  HandleObject readerObj,
                                                  HandleValue reason) {
  AssertHeapIsIdle();
  CHECK_THREAD(cx);
  cx->check(reason);

  Rooted<ReadableStreamReader*> unwrappedReader(
      cx, APIUnwrapAndDowncast<ReadableStreamReader>(cx, readerObj));
  if (!unwrappedReader) {
    return false;
  }
  MOZ_ASSERT(unwrappedReader->forAuthorCode() == ForAuthorCodeBool::No,
             "C++ code should not touch readers created by scripts");

  return ReadableStreamReaderGenericCancel(cx, unwrappedReader, reason);
||||||| merged common ancestors
MOZ_MUST_USE ReadableStreamReader*
ReadableStream::getReader(JSContext* cx, Handle<ReadableStream*> stream,
                          JS::ReadableStreamReaderMode mode)
{
    return CreateReadableStreamDefaultReader(cx, stream);
=======
JS_PUBLIC_API bool JS::ReadableStreamReaderReleaseLock(JSContext* cx,
                                                       HandleObject readerObj) {
  AssertHeapIsIdle();
  CHECK_THREAD(cx);

  Rooted<ReadableStreamReader*> unwrappedReader(
      cx, APIUnwrapAndDowncast<ReadableStreamReader>(cx, readerObj));
  if (!unwrappedReader) {
    return false;
  }
  MOZ_ASSERT(unwrappedReader->forAuthorCode() == ForAuthorCodeBool::No,
             "C++ code should not touch readers created by scripts");

#ifdef DEBUG
  Rooted<ReadableStream*> unwrappedStream(
      cx, UnwrapStreamFromReader(cx, unwrappedReader));
  if (!unwrappedStream) {
    return false;
  }
  MOZ_ASSERT(ReadableStreamGetNumReadRequests(unwrappedStream) == 0);
#endif  // DEBUG

  return ReadableStreamReaderGenericRelease(cx, unwrappedReader);
>>>>>>> upstream-releases
}

<<<<<<< HEAD
JS_PUBLIC_API bool JS::ReadableStreamReaderReleaseLock(JSContext* cx,
                                                       HandleObject readerObj) {
  AssertHeapIsIdle();
  CHECK_THREAD(cx);

  Rooted<ReadableStreamReader*> unwrappedReader(
      cx, APIUnwrapAndDowncast<ReadableStreamReader>(cx, readerObj));
  if (!unwrappedReader) {
    return false;
  }
  MOZ_ASSERT(unwrappedReader->forAuthorCode() == ForAuthorCodeBool::No,
             "C++ code should not touch readers created by scripts");

#ifdef DEBUG
  Rooted<ReadableStream*> unwrappedStream(
      cx, UnwrapStreamFromReader(cx, unwrappedReader));
  if (!unwrappedStream) {
    return false;
  }
  MOZ_ASSERT(ReadableStreamGetNumReadRequests(unwrappedStream) == 0);
#endif  // DEBUG

  return ReadableStreamReaderGenericRelease(cx, unwrappedReader);
}

JS_PUBLIC_API JSObject* JS::ReadableStreamDefaultReaderRead(
    JSContext* cx, HandleObject readerObj) {
  AssertHeapIsIdle();
  CHECK_THREAD(cx);

  Rooted<ReadableStreamDefaultReader*> unwrappedReader(
      cx, APIUnwrapAndDowncast<ReadableStreamDefaultReader>(cx, readerObj));
  if (!unwrappedReader) {
||||||| merged common ancestors
JS_FRIEND_API(JSObject*)
js::UnwrapReadableStream(JSObject* obj)
{
    if (JSObject* unwrapped = CheckedUnwrap(obj)) {
        return unwrapped->is<ReadableStream>() ? unwrapped : nullptr;
    }
=======
JS_PUBLIC_API JSObject* JS::ReadableStreamDefaultReaderRead(
    JSContext* cx, HandleObject readerObj) {
  AssertHeapIsIdle();
  CHECK_THREAD(cx);

  Rooted<ReadableStreamDefaultReader*> unwrappedReader(
      cx, APIUnwrapAndDowncast<ReadableStreamDefaultReader>(cx, readerObj));
  if (!unwrappedReader) {
>>>>>>> upstream-releases
    return nullptr;
  }
  MOZ_ASSERT(unwrappedReader->forAuthorCode() == ForAuthorCodeBool::No,
             "C++ code should not touch readers created by scripts");

  return ::ReadableStreamDefaultReaderRead(cx, unwrappedReader);
}
