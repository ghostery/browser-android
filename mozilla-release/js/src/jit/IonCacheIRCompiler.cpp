/* -*- Mode: C++; tab-width: 8; indent-tabs-mode: nil; c-basic-offset: 2 -*-
 * vim: set ts=8 sts=2 et sw=2 tw=80:
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/. */

#include "jit/IonCacheIRCompiler.h"
#include "mozilla/DebugOnly.h"
#include "mozilla/Maybe.h"

#include "jit/BaselineIC.h"
#include "jit/CacheIRCompiler.h"
#include "jit/IonIC.h"
#include "jit/JSJitFrameIter.h"
#include "jit/Linker.h"
#include "jit/SharedICHelpers.h"
#include "jit/VMFunctions.h"
#include "proxy/DeadObjectProxy.h"
#include "proxy/Proxy.h"

#include "jit/JSJitFrameIter-inl.h"
#include "jit/MacroAssembler-inl.h"
#include "jit/VMFunctionList-inl.h"
#include "vm/Realm-inl.h"
#include "vm/TypeInference-inl.h"

using namespace js;
using namespace js::jit;

using mozilla::DebugOnly;
using mozilla::Maybe;

namespace js {
namespace jit {

// IonCacheIRCompiler compiles CacheIR to IonIC native code.
<<<<<<< HEAD
class MOZ_RAII IonCacheIRCompiler : public CacheIRCompiler {
 public:
  friend class AutoSaveLiveRegisters;

  IonCacheIRCompiler(JSContext* cx, const CacheIRWriter& writer, IonIC* ic,
                     IonScript* ionScript, IonICStub* stub,
                     const PropertyTypeCheckInfo* typeCheckInfo,
                     uint32_t stubDataOffset)
      : CacheIRCompiler(cx, writer, stubDataOffset, Mode::Ion,
                        StubFieldPolicy::Constant),
        writer_(writer),
        ic_(ic),
        ionScript_(ionScript),
        stub_(stub),
        typeCheckInfo_(typeCheckInfo),
#ifdef DEBUG
        calledPrepareVMCall_(false),
#endif
        savedLiveRegs_(false) {
    MOZ_ASSERT(ic_);
    MOZ_ASSERT(ionScript_);
  }

  MOZ_MUST_USE bool init();
  JitCode* compile();

 private:
  const CacheIRWriter& writer_;
  IonIC* ic_;
  IonScript* ionScript_;

  // The stub we're generating code for.
  IonICStub* stub_;

  // Information necessary to generate property type checks. Non-null iff
  // this is a SetProp/SetElem stub.
  const PropertyTypeCheckInfo* typeCheckInfo_;

  CodeOffsetJump rejoinOffset_;
  Vector<CodeOffset, 4, SystemAllocPolicy> nextCodeOffsets_;
  Maybe<LiveRegisterSet> liveRegs_;
  Maybe<CodeOffset> stubJitCodeOffset_;

#ifdef DEBUG
  bool calledPrepareVMCall_;
#endif
  bool savedLiveRegs_;

  template <typename T>
  T rawWordStubField(uint32_t offset) {
    static_assert(sizeof(T) == sizeof(uintptr_t), "T must have word size");
    return (T)readStubWord(offset, StubField::Type::RawWord);
  }
  template <typename T>
  T rawInt64StubField(uint32_t offset) {
    static_assert(sizeof(T) == sizeof(int64_t), "T musthave int64 size");
    return (T)readStubInt64(offset, StubField::Type::RawInt64);
  }

  uint64_t* expandoGenerationStubFieldPtr(uint32_t offset) {
    DebugOnly<uint64_t> generation =
        readStubInt64(offset, StubField::Type::DOMExpandoGeneration);
    uint64_t* ptr =
        reinterpret_cast<uint64_t*>(stub_->stubDataStart() + offset);
    MOZ_ASSERT(*ptr == generation);
    return ptr;
  }

  void prepareVMCall(MacroAssembler& masm, const AutoSaveLiveRegisters&);
  MOZ_MUST_USE bool callVM(MacroAssembler& masm, const VMFunction& fun);

  MOZ_MUST_USE bool emitAddAndStoreSlotShared(CacheOp op);

  bool needsPostBarrier() const {
    return ic_->asSetPropertyIC()->needsPostBarrier();
  }

  void pushStubCodePointer() {
    stubJitCodeOffset_.emplace(masm.PushWithPatch(ImmPtr((void*)-1)));
  }
||||||| merged common ancestors
class MOZ_RAII IonCacheIRCompiler : public CacheIRCompiler
{
  public:
    friend class AutoSaveLiveRegisters;

    IonCacheIRCompiler(JSContext* cx, const CacheIRWriter& writer, IonIC* ic, IonScript* ionScript,
                       IonICStub* stub, const PropertyTypeCheckInfo* typeCheckInfo, uint32_t stubDataOffset)
      : CacheIRCompiler(cx, writer, stubDataOffset, Mode::Ion, StubFieldPolicy::Constant),
        writer_(writer),
        ic_(ic),
        ionScript_(ionScript),
        stub_(stub),
        typeCheckInfo_(typeCheckInfo),
#ifdef DEBUG
        calledPrepareVMCall_(false),
#endif
        savedLiveRegs_(false)
    {
        MOZ_ASSERT(ic_);
        MOZ_ASSERT(ionScript_);
    }

    MOZ_MUST_USE bool init();
    JitCode* compile();

  private:
    const CacheIRWriter& writer_;
    IonIC* ic_;
    IonScript* ionScript_;

    // The stub we're generating code for.
    IonICStub* stub_;

    // Information necessary to generate property type checks. Non-null iff
    // this is a SetProp/SetElem stub.
    const PropertyTypeCheckInfo* typeCheckInfo_;

    CodeOffsetJump rejoinOffset_;
    Vector<CodeOffset, 4, SystemAllocPolicy> nextCodeOffsets_;
    Maybe<LiveRegisterSet> liveRegs_;
    Maybe<CodeOffset> stubJitCodeOffset_;

#ifdef DEBUG
    bool calledPrepareVMCall_;
#endif
    bool savedLiveRegs_;


    template <typename T>
    T rawWordStubField(uint32_t offset) {
        static_assert(sizeof(T) == sizeof(uintptr_t), "T must have word size");
        return (T)readStubWord(offset, StubField::Type::RawWord);
    }
    template <typename T>
    T rawInt64StubField(uint32_t offset) {
        static_assert(sizeof(T) == sizeof(int64_t), "T musthave int64 size");
        return (T)readStubInt64(offset, StubField::Type::RawInt64);
    }

    uint64_t* expandoGenerationStubFieldPtr(uint32_t offset) {
        DebugOnly<uint64_t> generation =
            readStubInt64(offset, StubField::Type::DOMExpandoGeneration);
        uint64_t* ptr = reinterpret_cast<uint64_t*>(stub_->stubDataStart() + offset);
        MOZ_ASSERT(*ptr == generation);
        return ptr;
    }

    void prepareVMCall(MacroAssembler& masm, const AutoSaveLiveRegisters&);
    MOZ_MUST_USE bool callVM(MacroAssembler& masm, const VMFunction& fun);

    MOZ_MUST_USE bool emitAddAndStoreSlotShared(CacheOp op);

    bool needsPostBarrier() const {
        return ic_->asSetPropertyIC()->needsPostBarrier();
    }

    void pushStubCodePointer() {
        stubJitCodeOffset_.emplace(masm.PushWithPatch(ImmPtr((void*)-1)));
    }
=======
IonCacheIRCompiler::IonCacheIRCompiler(
    JSContext* cx, const CacheIRWriter& writer, IonIC* ic, IonScript* ionScript,
    IonICStub* stub, const PropertyTypeCheckInfo* typeCheckInfo,
    uint32_t stubDataOffset)
    : CacheIRCompiler(cx, writer, stubDataOffset, Mode::Ion,
                      StubFieldPolicy::Constant),
      writer_(writer),
      ic_(ic),
      ionScript_(ionScript),
      stub_(stub),
      typeCheckInfo_(typeCheckInfo),
      savedLiveRegs_(false) {
  MOZ_ASSERT(ic_);
  MOZ_ASSERT(ionScript_);
}

template <typename T>
T IonCacheIRCompiler::rawWordStubField(uint32_t offset) {
  static_assert(sizeof(T) == sizeof(uintptr_t), "T must have word size");
  return (T)readStubWord(offset, StubField::Type::RawWord);
}
template <typename T>
T IonCacheIRCompiler::rawInt64StubField(uint32_t offset) {
  static_assert(sizeof(T) == sizeof(int64_t), "T musthave int64 size");
  return (T)readStubInt64(offset, StubField::Type::RawInt64);
}

uint64_t* IonCacheIRCompiler::expandoGenerationStubFieldPtr(uint32_t offset) {
  DebugOnly<uint64_t> generation =
      readStubInt64(offset, StubField::Type::DOMExpandoGeneration);
  uint64_t* ptr = reinterpret_cast<uint64_t*>(stub_->stubDataStart() + offset);
  MOZ_ASSERT(*ptr == generation);
  return ptr;
}

template <typename Fn, Fn fn>
void IonCacheIRCompiler::callVM(MacroAssembler& masm) {
  VMFunctionId id = VMFunctionToId<Fn, fn>::id;
  callVMInternal(masm, id);
}

bool IonCacheIRCompiler::needsPostBarrier() const {
  return ic_->asSetPropertyIC()->needsPostBarrier();
}
>>>>>>> upstream-releases

<<<<<<< HEAD
#define DEFINE_OP(op) MOZ_MUST_USE bool emit##op();
  CACHE_IR_OPS(DEFINE_OP)
#undef DEFINE_OP
};
||||||| merged common ancestors
#define DEFINE_OP(op) MOZ_MUST_USE bool emit##op();
    CACHE_IR_OPS(DEFINE_OP)
#undef DEFINE_OP
};
=======
void IonCacheIRCompiler::pushStubCodePointer() {
  stubJitCodeOffset_.emplace(masm.PushWithPatch(ImmPtr((void*)-1)));
}
>>>>>>> upstream-releases

// AutoSaveLiveRegisters must be used when we make a call that can GC. The
// constructor ensures all live registers are stored on the stack (where the GC
// expects them) and the destructor restores these registers.
<<<<<<< HEAD
class MOZ_RAII AutoSaveLiveRegisters {
  IonCacheIRCompiler& compiler_;

  AutoSaveLiveRegisters(const AutoSaveLiveRegisters&) = delete;
  void operator=(const AutoSaveLiveRegisters&) = delete;

 public:
  explicit AutoSaveLiveRegisters(IonCacheIRCompiler& compiler)
      : compiler_(compiler) {
    MOZ_ASSERT(compiler_.liveRegs_.isSome());
    compiler_.allocator.saveIonLiveRegisters(
        compiler_.masm, compiler_.liveRegs_.ref(),
        compiler_.ic_->scratchRegisterForEntryJump(), compiler_.ionScript_);
    compiler_.savedLiveRegs_ = true;
  }
  ~AutoSaveLiveRegisters() {
    MOZ_ASSERT(compiler_.stubJitCodeOffset_.isSome(),
               "Must have pushed JitCode* pointer");
    compiler_.allocator.restoreIonLiveRegisters(compiler_.masm,
                                                compiler_.liveRegs_.ref());
    MOZ_ASSERT(compiler_.masm.framePushed() ==
               compiler_.ionScript_->frameSize());
  }
};

}  // namespace jit
}  // namespace js
||||||| merged common ancestors
class MOZ_RAII AutoSaveLiveRegisters
{
    IonCacheIRCompiler& compiler_;

    AutoSaveLiveRegisters(const AutoSaveLiveRegisters&) = delete;
    void operator=(const AutoSaveLiveRegisters&) = delete;

  public:
    explicit AutoSaveLiveRegisters(IonCacheIRCompiler& compiler)
      : compiler_(compiler)
    {
        MOZ_ASSERT(compiler_.liveRegs_.isSome());
        compiler_.allocator.saveIonLiveRegisters(compiler_.masm,
                                                 compiler_.liveRegs_.ref(),
                                                 compiler_.ic_->scratchRegisterForEntryJump(),
                                                 compiler_.ionScript_);
        compiler_.savedLiveRegs_ = true;
    }
    ~AutoSaveLiveRegisters() {
        MOZ_ASSERT(compiler_.stubJitCodeOffset_.isSome(), "Must have pushed JitCode* pointer");
        compiler_.allocator.restoreIonLiveRegisters(compiler_.masm, compiler_.liveRegs_.ref());
        MOZ_ASSERT(compiler_.masm.framePushed() == compiler_.ionScript_->frameSize());
    }
};

} // namespace jit
} // namespace js
=======
AutoSaveLiveRegisters::AutoSaveLiveRegisters(IonCacheIRCompiler& compiler)
    : compiler_(compiler) {
  MOZ_ASSERT(compiler_.liveRegs_.isSome());
  MOZ_ASSERT(compiler_.ic_);
  compiler_.allocator.saveIonLiveRegisters(
      compiler_.masm, compiler_.liveRegs_.ref(),
      compiler_.ic_->scratchRegisterForEntryJump(), compiler_.ionScript_);
  compiler_.savedLiveRegs_ = true;
}
AutoSaveLiveRegisters::~AutoSaveLiveRegisters() {
  MOZ_ASSERT(compiler_.stubJitCodeOffset_.isSome(),
             "Must have pushed JitCode* pointer");
  compiler_.allocator.restoreIonLiveRegisters(compiler_.masm,
                                              compiler_.liveRegs_.ref());
  MOZ_ASSERT(compiler_.masm.framePushed() == compiler_.ionScript_->frameSize());
}

}  // namespace jit
}  // namespace js
>>>>>>> upstream-releases

#define DEFINE_SHARED_OP(op) \
  bool IonCacheIRCompiler::emit##op() { return CacheIRCompiler::emit##op(); }
CACHE_IR_SHARED_OPS(DEFINE_SHARED_OP)
#undef DEFINE_SHARED_OP

void CacheRegisterAllocator::saveIonLiveRegisters(MacroAssembler& masm,
                                                  LiveRegisterSet liveRegs,
                                                  Register scratch,
                                                  IonScript* ionScript) {
  // We have to push all registers in liveRegs on the stack. It's possible we
  // stored other values in our live registers and stored operands on the
  // stack (where our live registers should go), so this requires some careful
  // work. Try to keep it simple by taking one small step at a time.

  // Step 1. Discard any dead operands so we can reuse their registers.
  freeDeadOperandLocations(masm);

  // Step 2. Figure out the size of our live regs.
  size_t sizeOfLiveRegsInBytes = liveRegs.gprs().size() * sizeof(intptr_t) +
                                 liveRegs.fpus().getPushSizeInBytes();

  MOZ_ASSERT(sizeOfLiveRegsInBytes > 0);

  // Step 3. Ensure all non-input operands are on the stack.
  size_t numInputs = writer_.numInputOperands();
  for (size_t i = numInputs; i < operandLocations_.length(); i++) {
    OperandLocation& loc = operandLocations_[i];
    if (loc.isInRegister()) {
      spillOperandToStack(masm, &loc);
    }
  }

  // Step 4. Restore the register state, but don't discard the stack as
  // non-input operands are stored there.
  restoreInputState(masm, /* shouldDiscardStack = */ false);

  // We just restored the input state, so no input operands should be stored
  // on the stack.
#ifdef DEBUG
  for (size_t i = 0; i < numInputs; i++) {
    const OperandLocation& loc = operandLocations_[i];
    MOZ_ASSERT(!loc.isOnStack());
  }
#endif

  // Step 5. At this point our register state is correct. Stack values,
  // however, may cover the space where we have to store the live registers.
  // Move them out of the way.

  bool hasOperandOnStack = false;
  for (size_t i = numInputs; i < operandLocations_.length(); i++) {
    OperandLocation& loc = operandLocations_[i];
    if (!loc.isOnStack()) {
      continue;
    }

    hasOperandOnStack = true;

    size_t operandSize = loc.stackSizeInBytes();
    size_t operandStackPushed = loc.stackPushed();
    MOZ_ASSERT(operandSize > 0);
    MOZ_ASSERT(stackPushed_ >= operandStackPushed);
    MOZ_ASSERT(operandStackPushed >= operandSize);

    // If this operand doesn't cover the live register space, there's
    // nothing to do.
    if (operandStackPushed - operandSize >= sizeOfLiveRegsInBytes) {
      MOZ_ASSERT(stackPushed_ > sizeOfLiveRegsInBytes);
      continue;
    }

    // Reserve stack space for the live registers if needed.
    if (sizeOfLiveRegsInBytes > stackPushed_) {
      size_t extraBytes = sizeOfLiveRegsInBytes - stackPushed_;
      MOZ_ASSERT((extraBytes % sizeof(uintptr_t)) == 0);
      masm.subFromStackPtr(Imm32(extraBytes));
      stackPushed_ += extraBytes;
    }

    // Push the operand below the live register space.
    if (loc.kind() == OperandLocation::PayloadStack) {
      masm.push(
          Address(masm.getStackPointer(), stackPushed_ - operandStackPushed));
      stackPushed_ += operandSize;
      loc.setPayloadStack(stackPushed_, loc.payloadType());
      continue;
    }
    MOZ_ASSERT(loc.kind() == OperandLocation::ValueStack);
    masm.pushValue(
        Address(masm.getStackPointer(), stackPushed_ - operandStackPushed));
    stackPushed_ += operandSize;
    loc.setValueStack(stackPushed_);
  }

  // Step 6. If we have any operands on the stack, adjust their stackPushed
  // values to not include sizeOfLiveRegsInBytes (this simplifies code down
  // the line). Then push/store the live registers.
  if (hasOperandOnStack) {
    MOZ_ASSERT(stackPushed_ > sizeOfLiveRegsInBytes);
    stackPushed_ -= sizeOfLiveRegsInBytes;

    for (size_t i = numInputs; i < operandLocations_.length(); i++) {
      OperandLocation& loc = operandLocations_[i];
      if (loc.isOnStack()) {
        loc.adjustStackPushed(-int32_t(sizeOfLiveRegsInBytes));
      }
    }

    size_t stackBottom = stackPushed_ + sizeOfLiveRegsInBytes;
    masm.storeRegsInMask(liveRegs, Address(masm.getStackPointer(), stackBottom),
                         scratch);
    masm.setFramePushed(masm.framePushed() + sizeOfLiveRegsInBytes);
  } else {
    // If no operands are on the stack, discard the unused stack space.
    if (stackPushed_ > 0) {
      masm.addToStackPtr(Imm32(stackPushed_));
      stackPushed_ = 0;
    }
    masm.PushRegsInMask(liveRegs);
  }
  freePayloadSlots_.clear();
  freeValueSlots_.clear();

  MOZ_ASSERT(masm.framePushed() ==
             ionScript->frameSize() + sizeOfLiveRegsInBytes);

  // Step 7. All live registers and non-input operands are stored on the stack
  // now, so at this point all registers except for the input registers are
  // available.
  availableRegs_.set() = GeneralRegisterSet::Not(inputRegisterSet());
  availableRegsAfterSpill_.set() = GeneralRegisterSet();

  // Step 8. We restored our input state, so we have to fix up aliased input
  // registers again.
  fixupAliasedInputs(masm);
}

void CacheRegisterAllocator::restoreIonLiveRegisters(MacroAssembler& masm,
                                                     LiveRegisterSet liveRegs) {
  masm.PopRegsInMask(liveRegs);

  availableRegs_.set() = GeneralRegisterSet();
  availableRegsAfterSpill_.set() = GeneralRegisterSet::All();
}

static void* GetReturnAddressToIonCode(JSContext* cx) {
  JSJitFrameIter frame(cx->activation()->asJit());
  MOZ_ASSERT(frame.type() == FrameType::Exit,
             "An exit frame is expected as update functions are called with a "
             "VMFunction.");

  void* returnAddr = frame.returnAddress();
#ifdef DEBUG
  ++frame;
  MOZ_ASSERT(frame.isIonJS());
#endif
  return returnAddr;
}

// The AutoSaveLiveRegisters parameter is used to ensure registers were saved
void IonCacheIRCompiler::prepareVMCall(MacroAssembler& masm,
                                       const AutoSaveLiveRegisters&) {
  uint32_t descriptor = MakeFrameDescriptor(
      masm.framePushed(), FrameType::IonJS, IonICCallFrameLayout::Size());
  pushStubCodePointer();
  masm.Push(Imm32(descriptor));
  masm.Push(ImmPtr(GetReturnAddressToIonCode(cx_)));

<<<<<<< HEAD
#ifdef DEBUG
  calledPrepareVMCall_ = true;
#endif
||||||| merged common ancestors
#ifdef DEBUG
    calledPrepareVMCall_ = true;
#endif
=======
  preparedForVMCall_ = true;
>>>>>>> upstream-releases
}

<<<<<<< HEAD
bool IonCacheIRCompiler::callVM(MacroAssembler& masm, const VMFunction& fun) {
  MOZ_ASSERT(calledPrepareVMCall_);

  TrampolinePtr code = cx_->runtime()->jitRuntime()->getVMWrapper(fun);

  uint32_t frameSize = fun.explicitStackSlots() * sizeof(void*);
  uint32_t descriptor = MakeFrameDescriptor(frameSize, FrameType::IonICCall,
                                            ExitFrameLayout::Size());
  masm.Push(Imm32(descriptor));
  masm.callJit(code);

  // Remove rest of the frame left on the stack. We remove the return address
  // which is implicitly poped when returning.
  int framePop = sizeof(ExitFrameLayout) - sizeof(void*);

  // Pop arguments from framePushed.
  masm.implicitPop(frameSize + framePop);
  masm.freeStack(IonICCallFrameLayout::Size());
  return true;
}

bool IonCacheIRCompiler::init() {
  if (!allocator.init()) {
    return false;
  }
||||||| merged common ancestors
bool
IonCacheIRCompiler::callVM(MacroAssembler& masm, const VMFunction& fun)
{
    MOZ_ASSERT(calledPrepareVMCall_);

    TrampolinePtr code = cx_->runtime()->jitRuntime()->getVMWrapper(fun);

    uint32_t frameSize = fun.explicitStackSlots() * sizeof(void*);
    uint32_t descriptor = MakeFrameDescriptor(frameSize, FrameType::IonICCall,
                                              ExitFrameLayout::Size());
    masm.Push(Imm32(descriptor));
    masm.callJit(code);

    // Remove rest of the frame left on the stack. We remove the return address
    // which is implicitly poped when returning.
    int framePop = sizeof(ExitFrameLayout) - sizeof(void*);

    // Pop arguments from framePushed.
    masm.implicitPop(frameSize + framePop);
    masm.freeStack(IonICCallFrameLayout::Size());
    return true;
}

bool
IonCacheIRCompiler::init()
{
    if (!allocator.init()) {
        return false;
    }

    size_t numInputs = writer_.numInputOperands();

    AllocatableGeneralRegisterSet available;

    switch (ic_->kind()) {
      case CacheKind::GetProp:
      case CacheKind::GetElem: {
        IonGetPropertyIC* ic = ic_->asGetPropertyIC();
        TypedOrValueRegister output = ic->output();

        if (output.hasValue()) {
            available.add(output.valueReg());
        } else if (!output.typedReg().isFloat()) {
            available.add(output.typedReg().gpr());
        }

        if (ic->maybeTemp() != InvalidReg) {
            available.add(ic->maybeTemp());
        }

        liveRegs_.emplace(ic->liveRegs());
        outputUnchecked_.emplace(output);
=======
bool IonCacheIRCompiler::init() {
  if (!allocator.init()) {
    return false;
  }
>>>>>>> upstream-releases

  size_t numInputs = writer_.numInputOperands();

  AllocatableGeneralRegisterSet available;

  switch (ic_->kind()) {
    case CacheKind::GetProp:
    case CacheKind::GetElem: {
      IonGetPropertyIC* ic = ic_->asGetPropertyIC();
      TypedOrValueRegister output = ic->output();

      if (output.hasValue()) {
        available.add(output.valueReg());
      } else if (!output.typedReg().isFloat()) {
        available.add(output.typedReg().gpr());
      }
<<<<<<< HEAD

      if (ic->maybeTemp() != InvalidReg) {
        available.add(ic->maybeTemp());
||||||| merged common ancestors
      case CacheKind::SetProp:
      case CacheKind::SetElem: {
        IonSetPropertyIC* ic = ic_->asSetPropertyIC();

        available.add(ic->temp());

        liveRegs_.emplace(ic->liveRegs());

        allocator.initInputLocation(0, ic->object(), JSVAL_TYPE_OBJECT);

        if (ic->kind() == CacheKind::SetProp) {
            MOZ_ASSERT(numInputs == 2);
            allocator.initInputLocation(1, ic->rhs());
        } else {
            MOZ_ASSERT(numInputs == 3);
            allocator.initInputLocation(1, ic->id());
            allocator.initInputLocation(2, ic->rhs());
        }
        break;
=======

      if (ic->maybeTemp() != InvalidReg) {
        available.add(ic->maybeTemp());
>>>>>>> upstream-releases
      }
<<<<<<< HEAD

      liveRegs_.emplace(ic->liveRegs());
      outputUnchecked_.emplace(output);
||||||| merged common ancestors
      case CacheKind::GetName: {
        IonGetNameIC* ic = ic_->asGetNameIC();
        ValueOperand output = ic->output();

        available.add(output);
        available.add(ic->temp());
=======
>>>>>>> upstream-releases

<<<<<<< HEAD
      allowDoubleResult_.emplace(ic->allowDoubleResult());

      MOZ_ASSERT(numInputs == 1 || numInputs == 2);
||||||| merged common ancestors
        liveRegs_.emplace(ic->liveRegs());
        outputUnchecked_.emplace(output);

        MOZ_ASSERT(numInputs == 1);
        allocator.initInputLocation(0, ic->environment(), JSVAL_TYPE_OBJECT);
        break;
      }
      case CacheKind::BindName: {
        IonBindNameIC* ic = ic_->asBindNameIC();
        Register output = ic->output();
=======
      liveRegs_.emplace(ic->liveRegs());
      outputUnchecked_.emplace(output);
>>>>>>> upstream-releases

<<<<<<< HEAD
      allocator.initInputLocation(0, ic->value());
      if (numInputs > 1) {
        allocator.initInputLocation(1, ic->id());
||||||| merged common ancestors
        available.add(output);
        available.add(ic->temp());

        liveRegs_.emplace(ic->liveRegs());
        outputUnchecked_.emplace(TypedOrValueRegister(MIRType::Object, AnyRegister(output)));

        MOZ_ASSERT(numInputs == 1);
        allocator.initInputLocation(0, ic->environment(), JSVAL_TYPE_OBJECT);
        break;
=======
      allowDoubleResult_.emplace(ic->allowDoubleResult());

      MOZ_ASSERT(numInputs == 1 || numInputs == 2);

      allocator.initInputLocation(0, ic->value());
      if (numInputs > 1) {
        allocator.initInputLocation(1, ic->id());
>>>>>>> upstream-releases
      }
<<<<<<< HEAD
      break;
    }
    case CacheKind::GetPropSuper:
    case CacheKind::GetElemSuper: {
      IonGetPropSuperIC* ic = ic_->asGetPropSuperIC();
      TypedOrValueRegister output = ic->output();
||||||| merged common ancestors
      case CacheKind::GetIterator: {
        IonGetIteratorIC* ic = ic_->asGetIteratorIC();
        Register output = ic->output();

        available.add(output);
        available.add(ic->temp1());
        available.add(ic->temp2());
=======
      break;
    }
    case CacheKind::GetPropSuper:
    case CacheKind::GetElemSuper: {
      IonGetPropSuperIC* ic = ic_->asGetPropSuperIC();
      TypedOrValueRegister output = ic->output();

      available.add(output.valueReg());
>>>>>>> upstream-releases

<<<<<<< HEAD
      available.add(output.valueReg());
||||||| merged common ancestors
        liveRegs_.emplace(ic->liveRegs());
        outputUnchecked_.emplace(TypedOrValueRegister(MIRType::Object, AnyRegister(output)));
=======
      liveRegs_.emplace(ic->liveRegs());
      outputUnchecked_.emplace(output);
>>>>>>> upstream-releases

<<<<<<< HEAD
      liveRegs_.emplace(ic->liveRegs());
      outputUnchecked_.emplace(output);
||||||| merged common ancestors
        MOZ_ASSERT(numInputs == 1);
        allocator.initInputLocation(0, ic->value());
        break;
      }
      case CacheKind::In: {
        IonInIC* ic = ic_->asInIC();
        Register output = ic->output();
=======
      allowDoubleResult_.emplace(true);
>>>>>>> upstream-releases

<<<<<<< HEAD
      allowDoubleResult_.emplace(true);
||||||| merged common ancestors
        available.add(output);
=======
      MOZ_ASSERT(numInputs == 2 || numInputs == 3);
>>>>>>> upstream-releases

<<<<<<< HEAD
      MOZ_ASSERT(numInputs == 2 || numInputs == 3);
||||||| merged common ancestors
        liveRegs_.emplace(ic->liveRegs());
        outputUnchecked_.emplace(TypedOrValueRegister(MIRType::Boolean, AnyRegister(output)));
=======
      allocator.initInputLocation(0, ic->object(), JSVAL_TYPE_OBJECT);
>>>>>>> upstream-releases

<<<<<<< HEAD
      allocator.initInputLocation(0, ic->object(), JSVAL_TYPE_OBJECT);
||||||| merged common ancestors
        MOZ_ASSERT(numInputs == 2);
        allocator.initInputLocation(0, ic->key());
        allocator.initInputLocation(1, TypedOrValueRegister(MIRType::Object,
                                                            AnyRegister(ic->object())));
        break;
      }
      case CacheKind::HasOwn: {
        IonHasOwnIC* ic = ic_->asHasOwnIC();
        Register output = ic->output();

        available.add(output);

        liveRegs_.emplace(ic->liveRegs());
        outputUnchecked_.emplace(TypedOrValueRegister(MIRType::Boolean, AnyRegister(output)));
=======
      if (ic->kind() == CacheKind::GetPropSuper) {
        MOZ_ASSERT(numInputs == 2);
        allocator.initInputLocation(1, ic->receiver());
      } else {
        MOZ_ASSERT(numInputs == 3);
        allocator.initInputLocation(1, ic->id());
        allocator.initInputLocation(2, ic->receiver());
      }
      break;
    }
    case CacheKind::SetProp:
    case CacheKind::SetElem: {
      IonSetPropertyIC* ic = ic_->asSetPropertyIC();

      available.add(ic->temp());

      liveRegs_.emplace(ic->liveRegs());
>>>>>>> upstream-releases

<<<<<<< HEAD
      if (ic->kind() == CacheKind::GetPropSuper) {
        MOZ_ASSERT(numInputs == 2);
        allocator.initInputLocation(1, ic->receiver());
      } else {
        MOZ_ASSERT(numInputs == 3);
        allocator.initInputLocation(1, ic->id());
        allocator.initInputLocation(2, ic->receiver());
      }
      break;
    }
    case CacheKind::SetProp:
    case CacheKind::SetElem: {
      IonSetPropertyIC* ic = ic_->asSetPropertyIC();

      available.add(ic->temp());
||||||| merged common ancestors
        MOZ_ASSERT(numInputs == 2);
        allocator.initInputLocation(0, ic->id());
        allocator.initInputLocation(1, ic->value());
        break;
      }
      case CacheKind::InstanceOf: {
        IonInstanceOfIC* ic = ic_->asInstanceOfIC();
        Register output = ic->output();
        available.add(output);
        liveRegs_.emplace(ic->liveRegs());
        outputUnchecked_.emplace(TypedOrValueRegister(MIRType::Boolean, AnyRegister(output)));

        MOZ_ASSERT(numInputs == 2);
        allocator.initInputLocation(0, ic->lhs());
        allocator.initInputLocation(1, TypedOrValueRegister(MIRType::Object,
                                                            AnyRegister(ic->rhs())));
        break;
      }
      case CacheKind::UnaryArith: {
        IonUnaryArithIC *ic = ic_->asUnaryArithIC();
        ValueOperand output = ic->output();

        available.add(output);

        liveRegs_.emplace(ic->liveRegs());
        outputUnchecked_.emplace(TypedOrValueRegister(output));

        MOZ_ASSERT(numInputs == 1);
        allocator.initInputLocation(0, ic->input());
        break;
      }
      case CacheKind::BinaryArith: {
        IonBinaryArithIC* ic = ic_->asBinaryArithIC();
        ValueOperand output = ic->output();
=======
      allocator.initInputLocation(0, ic->object(), JSVAL_TYPE_OBJECT);

      if (ic->kind() == CacheKind::SetProp) {
        MOZ_ASSERT(numInputs == 2);
        allocator.initInputLocation(1, ic->rhs());
      } else {
        MOZ_ASSERT(numInputs == 3);
        allocator.initInputLocation(1, ic->id());
        allocator.initInputLocation(2, ic->rhs());
      }
      break;
    }
    case CacheKind::GetName: {
      IonGetNameIC* ic = ic_->asGetNameIC();
      ValueOperand output = ic->output();
>>>>>>> upstream-releases

<<<<<<< HEAD
      liveRegs_.emplace(ic->liveRegs());
||||||| merged common ancestors
        available.add(output);
=======
      available.add(output);
      available.add(ic->temp());
>>>>>>> upstream-releases

<<<<<<< HEAD
      allocator.initInputLocation(0, ic->object(), JSVAL_TYPE_OBJECT);
||||||| merged common ancestors
        liveRegs_.emplace(ic->liveRegs());
        outputUnchecked_.emplace(TypedOrValueRegister(output));
=======
      liveRegs_.emplace(ic->liveRegs());
      outputUnchecked_.emplace(output);
>>>>>>> upstream-releases

<<<<<<< HEAD
      if (ic->kind() == CacheKind::SetProp) {
        MOZ_ASSERT(numInputs == 2);
        allocator.initInputLocation(1, ic->rhs());
      } else {
        MOZ_ASSERT(numInputs == 3);
        allocator.initInputLocation(1, ic->id());
        allocator.initInputLocation(2, ic->rhs());
      }
      break;
    }
    case CacheKind::GetName: {
      IonGetNameIC* ic = ic_->asGetNameIC();
      ValueOperand output = ic->output();
||||||| merged common ancestors
        MOZ_ASSERT(numInputs == 2);
        allocator.initInputLocation(0, ic->lhs());
        allocator.initInputLocation(1, ic->rhs());
        break;
      }
      case CacheKind::Compare: {
        IonCompareIC *ic = ic_->asCompareIC();
        Register output = ic->output();
=======
      MOZ_ASSERT(numInputs == 1);
      allocator.initInputLocation(0, ic->environment(), JSVAL_TYPE_OBJECT);
      break;
    }
    case CacheKind::BindName: {
      IonBindNameIC* ic = ic_->asBindNameIC();
      Register output = ic->output();
>>>>>>> upstream-releases

      available.add(output);
      available.add(ic->temp());

<<<<<<< HEAD
      liveRegs_.emplace(ic->liveRegs());
      outputUnchecked_.emplace(output);
||||||| merged common ancestors
        liveRegs_.emplace(ic->liveRegs());
        outputUnchecked_.emplace(TypedOrValueRegister(MIRType::Boolean, AnyRegister(output)));
=======
      liveRegs_.emplace(ic->liveRegs());
      outputUnchecked_.emplace(
          TypedOrValueRegister(MIRType::Object, AnyRegister(output)));
>>>>>>> upstream-releases

      MOZ_ASSERT(numInputs == 1);
      allocator.initInputLocation(0, ic->environment(), JSVAL_TYPE_OBJECT);
      break;
    }
<<<<<<< HEAD
    case CacheKind::BindName: {
      IonBindNameIC* ic = ic_->asBindNameIC();
      Register output = ic->output();
||||||| merged common ancestors
=======
    case CacheKind::GetIterator: {
      IonGetIteratorIC* ic = ic_->asGetIteratorIC();
      Register output = ic->output();
>>>>>>> upstream-releases

<<<<<<< HEAD
      available.add(output);
      available.add(ic->temp());
||||||| merged common ancestors
    if (liveRegs_) {
        liveFloatRegs_ = LiveFloatRegisterSet(liveRegs_->fpus());
    }
=======
      available.add(output);
      available.add(ic->temp1());
      available.add(ic->temp2());
>>>>>>> upstream-releases

      liveRegs_.emplace(ic->liveRegs());
      outputUnchecked_.emplace(
          TypedOrValueRegister(MIRType::Object, AnyRegister(output)));

<<<<<<< HEAD
      MOZ_ASSERT(numInputs == 1);
      allocator.initInputLocation(0, ic->environment(), JSVAL_TYPE_OBJECT);
      break;
||||||| merged common ancestors
JitCode*
IonCacheIRCompiler::compile()
{
    masm.setFramePushed(ionScript_->frameSize());
    if (cx_->runtime()->geckoProfiler().enabled()) {
        masm.enableProfilingInstrumentation();
=======
      MOZ_ASSERT(numInputs == 1);
      allocator.initInputLocation(0, ic->value());
      break;
>>>>>>> upstream-releases
    }
<<<<<<< HEAD
    case CacheKind::GetIterator: {
      IonGetIteratorIC* ic = ic_->asGetIteratorIC();
      Register output = ic->output();
||||||| merged common ancestors
=======
    case CacheKind::In: {
      IonInIC* ic = ic_->asInIC();
      Register output = ic->output();
>>>>>>> upstream-releases

<<<<<<< HEAD
      available.add(output);
      available.add(ic->temp1());
      available.add(ic->temp2());
||||||| merged common ancestors
    allocator.fixupAliasedInputs(masm);
=======
      available.add(output);
>>>>>>> upstream-releases

<<<<<<< HEAD
      liveRegs_.emplace(ic->liveRegs());
      outputUnchecked_.emplace(
          TypedOrValueRegister(MIRType::Object, AnyRegister(output)));
||||||| merged common ancestors
    do {
        switch (reader.readOp()) {
#define DEFINE_OP(op)                   \
          case CacheOp::op:             \
            if (!emit##op())            \
                return nullptr;         \
            break;
    CACHE_IR_OPS(DEFINE_OP)
#undef DEFINE_OP
=======
      liveRegs_.emplace(ic->liveRegs());
      outputUnchecked_.emplace(
          TypedOrValueRegister(MIRType::Boolean, AnyRegister(output)));
>>>>>>> upstream-releases

<<<<<<< HEAD
      MOZ_ASSERT(numInputs == 1);
      allocator.initInputLocation(0, ic->value());
      break;
||||||| merged common ancestors
          default:
            MOZ_CRASH("Invalid op");
        }

        allocator.nextOp();
    } while (reader.more());

    masm.assumeUnreachable("Should have returned from IC");

    // Done emitting the main IC code. Now emit the failure paths.
    for (size_t i = 0; i < failurePaths.length(); i++) {
        if (!emitFailurePath(i)) {
            return nullptr;
        }
        Register scratch = ic_->scratchRegisterForEntryJump();
        CodeOffset offset = masm.movWithPatch(ImmWord(-1), scratch);
        masm.jump(Address(scratch, 0));
        if (!nextCodeOffsets_.append(offset)) {
            return nullptr;
        }
=======
      MOZ_ASSERT(numInputs == 2);
      allocator.initInputLocation(0, ic->key());
      allocator.initInputLocation(
          1, TypedOrValueRegister(MIRType::Object, AnyRegister(ic->object())));
      break;
>>>>>>> upstream-releases
    }
<<<<<<< HEAD
    case CacheKind::In: {
      IonInIC* ic = ic_->asInIC();
      Register output = ic->output();
||||||| merged common ancestors
=======
    case CacheKind::HasOwn: {
      IonHasOwnIC* ic = ic_->asHasOwnIC();
      Register output = ic->output();
>>>>>>> upstream-releases

      available.add(output);

      liveRegs_.emplace(ic->liveRegs());
      outputUnchecked_.emplace(
          TypedOrValueRegister(MIRType::Boolean, AnyRegister(output)));

<<<<<<< HEAD
      MOZ_ASSERT(numInputs == 2);
      allocator.initInputLocation(0, ic->key());
      allocator.initInputLocation(
          1, TypedOrValueRegister(MIRType::Object, AnyRegister(ic->object())));
      break;
||||||| merged common ancestors
    for (CodeOffset offset : nextCodeOffsets_) {
        Assembler::PatchDataWithValueCheck(CodeLocationLabel(newStubCode, offset),
                                           ImmPtr(stub_->nextCodeRawPtr()),
                                           ImmPtr((void*)-1));
    }
    if (stubJitCodeOffset_) {
        Assembler::PatchDataWithValueCheck(CodeLocationLabel(newStubCode, *stubJitCodeOffset_),
                                           ImmPtr(newStubCode.get()),
                                           ImmPtr((void*)-1));
=======
      MOZ_ASSERT(numInputs == 2);
      allocator.initInputLocation(0, ic->id());
      allocator.initInputLocation(1, ic->value());
      break;
>>>>>>> upstream-releases
    }
<<<<<<< HEAD
    case CacheKind::HasOwn: {
      IonHasOwnIC* ic = ic_->asHasOwnIC();
      Register output = ic->output();
||||||| merged common ancestors
=======
    case CacheKind::InstanceOf: {
      IonInstanceOfIC* ic = ic_->asInstanceOfIC();
      Register output = ic->output();
      available.add(output);
      liveRegs_.emplace(ic->liveRegs());
      outputUnchecked_.emplace(
          TypedOrValueRegister(MIRType::Boolean, AnyRegister(output)));
>>>>>>> upstream-releases

<<<<<<< HEAD
      available.add(output);
||||||| merged common ancestors
    return newStubCode;
}
=======
      MOZ_ASSERT(numInputs == 2);
      allocator.initInputLocation(0, ic->lhs());
      allocator.initInputLocation(
          1, TypedOrValueRegister(MIRType::Object, AnyRegister(ic->rhs())));
      break;
    }
    case CacheKind::UnaryArith: {
      IonUnaryArithIC* ic = ic_->asUnaryArithIC();
      ValueOperand output = ic->output();
>>>>>>> upstream-releases

<<<<<<< HEAD
      liveRegs_.emplace(ic->liveRegs());
      outputUnchecked_.emplace(
          TypedOrValueRegister(MIRType::Boolean, AnyRegister(output)));
||||||| merged common ancestors
bool
IonCacheIRCompiler::emitGuardShape()
{
    ObjOperandId objId = reader.objOperandId();
    Register obj = allocator.useRegister(masm, objId);
    Shape* shape = shapeStubField(reader.stubOffset());
=======
      available.add(output);
>>>>>>> upstream-releases

<<<<<<< HEAD
      MOZ_ASSERT(numInputs == 2);
      allocator.initInputLocation(0, ic->id());
      allocator.initInputLocation(1, ic->value());
      break;
||||||| merged common ancestors
    bool needSpectreMitigations = objectGuardNeedsSpectreMitigations(objId);

    Maybe<AutoScratchRegister> maybeScratch;
    if (needSpectreMitigations) {
        maybeScratch.emplace(allocator, masm);
=======
      liveRegs_.emplace(ic->liveRegs());
      outputUnchecked_.emplace(TypedOrValueRegister(output));

      MOZ_ASSERT(numInputs == 1);
      allocator.initInputLocation(0, ic->input());
      break;
>>>>>>> upstream-releases
    }
<<<<<<< HEAD
    case CacheKind::InstanceOf: {
      IonInstanceOfIC* ic = ic_->asInstanceOfIC();
      Register output = ic->output();
      available.add(output);
      liveRegs_.emplace(ic->liveRegs());
      outputUnchecked_.emplace(
          TypedOrValueRegister(MIRType::Boolean, AnyRegister(output)));
||||||| merged common ancestors
=======
    case CacheKind::BinaryArith: {
      IonBinaryArithIC* ic = ic_->asBinaryArithIC();
      ValueOperand output = ic->output();
>>>>>>> upstream-releases

<<<<<<< HEAD
      MOZ_ASSERT(numInputs == 2);
      allocator.initInputLocation(0, ic->lhs());
      allocator.initInputLocation(
          1, TypedOrValueRegister(MIRType::Object, AnyRegister(ic->rhs())));
      break;
    }
    case CacheKind::UnaryArith: {
      IonUnaryArithIC* ic = ic_->asUnaryArithIC();
      ValueOperand output = ic->output();
||||||| merged common ancestors
    FailurePath* failure;
    if (!addFailurePath(&failure)) {
        return false;
    }
=======
      available.add(output);

      liveRegs_.emplace(ic->liveRegs());
      outputUnchecked_.emplace(TypedOrValueRegister(output));

      MOZ_ASSERT(numInputs == 2);
      allocator.initInputLocation(0, ic->lhs());
      allocator.initInputLocation(1, ic->rhs());
      break;
    }
    case CacheKind::Compare: {
      IonCompareIC* ic = ic_->asCompareIC();
      Register output = ic->output();

      available.add(output);

      liveRegs_.emplace(ic->liveRegs());
      outputUnchecked_.emplace(
          TypedOrValueRegister(MIRType::Boolean, AnyRegister(output)));

      MOZ_ASSERT(numInputs == 2);
      allocator.initInputLocation(0, ic->lhs());
      allocator.initInputLocation(1, ic->rhs());
      break;
    }
    case CacheKind::Call:
    case CacheKind::TypeOf:
    case CacheKind::ToBool:
    case CacheKind::GetIntrinsic:
    case CacheKind::NewObject:
      MOZ_CRASH("Unsupported IC");
  }

  if (liveRegs_) {
    liveFloatRegs_ = LiveFloatRegisterSet(liveRegs_->fpus());
  }

  allocator.initAvailableRegs(available);
  allocator.initAvailableRegsAfterSpill();
  return true;
}

JitCode* IonCacheIRCompiler::compile() {
  masm.setFramePushed(ionScript_->frameSize());
  if (cx_->runtime()->geckoProfiler().enabled()) {
    masm.enableProfilingInstrumentation();
  }

  allocator.fixupAliasedInputs(masm);

  do {
    switch (reader.readOp()) {
#define DEFINE_OP(op, ...)           \
  case CacheOp::op:                  \
    if (!emit##op()) return nullptr; \
    break;
      CACHE_IR_OPS(DEFINE_OP)
#undef DEFINE_OP
>>>>>>> upstream-releases

<<<<<<< HEAD
      available.add(output);
||||||| merged common ancestors
    if (needSpectreMitigations) {
        masm.branchTestObjShape(Assembler::NotEqual, obj, shape, *maybeScratch, obj,
                                failure->label());
    } else {
        masm.branchTestObjShapeNoSpectreMitigations(Assembler::NotEqual, obj, shape,
                                                    failure->label());
    }

    return true;
}
=======
      default:
        MOZ_CRASH("Invalid op");
    }
#ifdef DEBUG
    assertAllArgumentsConsumed();
#endif
    allocator.nextOp();
  } while (reader.more());

  masm.assumeUnreachable("Should have returned from IC");

  // Done emitting the main IC code. Now emit the failure paths.
  for (size_t i = 0; i < failurePaths.length(); i++) {
    if (!emitFailurePath(i)) {
      return nullptr;
    }
    Register scratch = ic_->scratchRegisterForEntryJump();
    CodeOffset offset = masm.movWithPatch(ImmWord(-1), scratch);
    masm.jump(Address(scratch, 0));
    if (!nextCodeOffsets_.append(offset)) {
      return nullptr;
    }
  }

  Linker linker(masm, "getStubCode");
  Rooted<JitCode*> newStubCode(cx_, linker.newCode(cx_, CodeKind::Ion));
  if (!newStubCode) {
    cx_->recoverFromOutOfMemory();
    return nullptr;
  }

  rejoinOffset_.fixup(&masm);
  CodeLocationJump rejoinJump(newStubCode, rejoinOffset_);
  PatchJump(rejoinJump, ic_->rejoinLabel());

  for (CodeOffset offset : nextCodeOffsets_) {
    Assembler::PatchDataWithValueCheck(CodeLocationLabel(newStubCode, offset),
                                       ImmPtr(stub_->nextCodeRawPtr()),
                                       ImmPtr((void*)-1));
  }
  if (stubJitCodeOffset_) {
    Assembler::PatchDataWithValueCheck(
        CodeLocationLabel(newStubCode, *stubJitCodeOffset_),
        ImmPtr(newStubCode.get()), ImmPtr((void*)-1));
  }

  return newStubCode;
}

bool IonCacheIRCompiler::emitGuardShape() {
  JitSpew(JitSpew_Codegen, __FUNCTION__);
  ObjOperandId objId = reader.objOperandId();
  Register obj = allocator.useRegister(masm, objId);
  Shape* shape = shapeStubField(reader.stubOffset());

  bool needSpectreMitigations = objectGuardNeedsSpectreMitigations(objId);

  Maybe<AutoScratchRegister> maybeScratch;
  if (needSpectreMitigations) {
    maybeScratch.emplace(allocator, masm);
  }

  FailurePath* failure;
  if (!addFailurePath(&failure)) {
    return false;
  }

  if (needSpectreMitigations) {
    masm.branchTestObjShape(Assembler::NotEqual, obj, shape, *maybeScratch, obj,
                            failure->label());
  } else {
    masm.branchTestObjShapeNoSpectreMitigations(Assembler::NotEqual, obj, shape,
                                                failure->label());
  }

  return true;
}
>>>>>>> upstream-releases

<<<<<<< HEAD
      liveRegs_.emplace(ic->liveRegs());
      outputUnchecked_.emplace(TypedOrValueRegister(output));
||||||| merged common ancestors
bool
IonCacheIRCompiler::emitGuardGroup()
{
    ObjOperandId objId = reader.objOperandId();
    Register obj = allocator.useRegister(masm, objId);
    ObjectGroup* group = groupStubField(reader.stubOffset());
=======
bool IonCacheIRCompiler::emitGuardGroup() {
  JitSpew(JitSpew_Codegen, __FUNCTION__);
  ObjOperandId objId = reader.objOperandId();
  Register obj = allocator.useRegister(masm, objId);
  ObjectGroup* group = groupStubField(reader.stubOffset());
>>>>>>> upstream-releases

<<<<<<< HEAD
      MOZ_ASSERT(numInputs == 1);
      allocator.initInputLocation(0, ic->input());
      break;
    }
    case CacheKind::BinaryArith: {
      IonBinaryArithIC* ic = ic_->asBinaryArithIC();
      ValueOperand output = ic->output();
||||||| merged common ancestors
    bool needSpectreMitigations = objectGuardNeedsSpectreMitigations(objId);

    Maybe<AutoScratchRegister> maybeScratch;
    if (needSpectreMitigations) {
        maybeScratch.emplace(allocator, masm);
    }
=======
  bool needSpectreMitigations = objectGuardNeedsSpectreMitigations(objId);

  Maybe<AutoScratchRegister> maybeScratch;
  if (needSpectreMitigations) {
    maybeScratch.emplace(allocator, masm);
  }
>>>>>>> upstream-releases

<<<<<<< HEAD
      available.add(output);

      liveRegs_.emplace(ic->liveRegs());
      outputUnchecked_.emplace(TypedOrValueRegister(output));

      MOZ_ASSERT(numInputs == 2);
      allocator.initInputLocation(0, ic->lhs());
      allocator.initInputLocation(1, ic->rhs());
      break;
    }
    case CacheKind::Compare: {
      IonCompareIC* ic = ic_->asCompareIC();
      Register output = ic->output();

      available.add(output);

      liveRegs_.emplace(ic->liveRegs());
      outputUnchecked_.emplace(
          TypedOrValueRegister(MIRType::Boolean, AnyRegister(output)));

      MOZ_ASSERT(numInputs == 2);
      allocator.initInputLocation(0, ic->lhs());
      allocator.initInputLocation(1, ic->rhs());
      break;
    }
    case CacheKind::Call:
    case CacheKind::TypeOf:
    case CacheKind::ToBool:
    case CacheKind::GetIntrinsic:
    case CacheKind::NewObject:
      MOZ_CRASH("Unsupported IC");
  }

  if (liveRegs_) {
    liveFloatRegs_ = LiveFloatRegisterSet(liveRegs_->fpus());
  }

  allocator.initAvailableRegs(available);
  allocator.initAvailableRegsAfterSpill();
  return true;
}

JitCode* IonCacheIRCompiler::compile() {
  masm.setFramePushed(ionScript_->frameSize());
  if (cx_->runtime()->geckoProfiler().enabled()) {
    masm.enableProfilingInstrumentation();
  }

  allocator.fixupAliasedInputs(masm);

  do {
    switch (reader.readOp()) {
#define DEFINE_OP(op)                \
  case CacheOp::op:                  \
    if (!emit##op()) return nullptr; \
    break;
      CACHE_IR_OPS(DEFINE_OP)
#undef DEFINE_OP
||||||| merged common ancestors
    FailurePath* failure;
    if (!addFailurePath(&failure)) {
        return false;
    }
=======
  FailurePath* failure;
  if (!addFailurePath(&failure)) {
    return false;
  }
>>>>>>> upstream-releases

<<<<<<< HEAD
      default:
        MOZ_CRASH("Invalid op");
    }
||||||| merged common ancestors
    if (needSpectreMitigations) {
        masm.branchTestObjGroup(Assembler::NotEqual, obj, group, *maybeScratch, obj,
                                failure->label());
    } else {
        masm.branchTestObjGroupNoSpectreMitigations(Assembler::NotEqual, obj, group,
                                                    failure->label());
    }
=======
  if (needSpectreMitigations) {
    masm.branchTestObjGroup(Assembler::NotEqual, obj, group, *maybeScratch, obj,
                            failure->label());
  } else {
    masm.branchTestObjGroupNoSpectreMitigations(Assembler::NotEqual, obj, group,
                                                failure->label());
  }
>>>>>>> upstream-releases

<<<<<<< HEAD
    allocator.nextOp();
  } while (reader.more());
||||||| merged common ancestors
    return true;
}
=======
  return true;
}
>>>>>>> upstream-releases

<<<<<<< HEAD
  masm.assumeUnreachable("Should have returned from IC");
||||||| merged common ancestors
bool
IonCacheIRCompiler::emitGuardProto()
{
    Register obj = allocator.useRegister(masm, reader.objOperandId());
    JSObject* proto = objectStubField(reader.stubOffset());
=======
bool IonCacheIRCompiler::emitGuardProto() {
  JitSpew(JitSpew_Codegen, __FUNCTION__);
  Register obj = allocator.useRegister(masm, reader.objOperandId());
  JSObject* proto = objectStubField(reader.stubOffset());
>>>>>>> upstream-releases

<<<<<<< HEAD
  // Done emitting the main IC code. Now emit the failure paths.
  for (size_t i = 0; i < failurePaths.length(); i++) {
    if (!emitFailurePath(i)) {
      return nullptr;
    }
    Register scratch = ic_->scratchRegisterForEntryJump();
    CodeOffset offset = masm.movWithPatch(ImmWord(-1), scratch);
    masm.jump(Address(scratch, 0));
    if (!nextCodeOffsets_.append(offset)) {
      return nullptr;
    }
  }
||||||| merged common ancestors
    AutoScratchRegister scratch(allocator, masm);

    FailurePath* failure;
    if (!addFailurePath(&failure)) {
        return false;
    }

    masm.loadObjProto(obj, scratch);
    masm.branchPtr(Assembler::NotEqual, scratch, ImmGCPtr(proto), failure->label());
    return true;
}

bool
IonCacheIRCompiler::emitGuardCompartment()
{
    Register obj = allocator.useRegister(masm, reader.objOperandId());
    JSObject* globalWrapper = objectStubField(reader.stubOffset());
    JS::Compartment* compartment = compartmentStubField(reader.stubOffset());
    AutoScratchRegister scratch(allocator, masm);

    FailurePath* failure;
    if (!addFailurePath(&failure)) {
        return false;
    }
=======
  AutoScratchRegister scratch(allocator, masm);

  FailurePath* failure;
  if (!addFailurePath(&failure)) {
    return false;
  }

  masm.loadObjProto(obj, scratch);
  masm.branchPtr(Assembler::NotEqual, scratch, ImmGCPtr(proto),
                 failure->label());
  return true;
}

bool IonCacheIRCompiler::emitGuardCompartment() {
  JitSpew(JitSpew_Codegen, __FUNCTION__);
  Register obj = allocator.useRegister(masm, reader.objOperandId());
  JSObject* globalWrapper = objectStubField(reader.stubOffset());
  JS::Compartment* compartment = compartmentStubField(reader.stubOffset());
  AutoScratchRegister scratch(allocator, masm);

  FailurePath* failure;
  if (!addFailurePath(&failure)) {
    return false;
  }
>>>>>>> upstream-releases

<<<<<<< HEAD
  Linker linker(masm);
  AutoFlushICache afc("getStubCode");
  Rooted<JitCode*> newStubCode(cx_, linker.newCode(cx_, CodeKind::Ion));
  if (!newStubCode) {
    cx_->recoverFromOutOfMemory();
    return nullptr;
  }
||||||| merged common ancestors
    // Verify that the global wrapper is still valid, as
    // it is pre-requisite for doing the compartment check.
    masm.movePtr(ImmGCPtr(globalWrapper), scratch);
    Address handlerAddr(scratch, ProxyObject::offsetOfHandler());
    masm.branchPtr(Assembler::Equal, handlerAddr, ImmPtr(&DeadObjectProxy::singleton), failure->label());
=======
  // Verify that the global wrapper is still valid, as
  // it is pre-requisite for doing the compartment check.
  masm.movePtr(ImmGCPtr(globalWrapper), scratch);
  Address handlerAddr(scratch, ProxyObject::offsetOfHandler());
  masm.branchPtr(Assembler::Equal, handlerAddr,
                 ImmPtr(&DeadObjectProxy::singleton), failure->label());
>>>>>>> upstream-releases

<<<<<<< HEAD
  rejoinOffset_.fixup(&masm);
  CodeLocationJump rejoinJump(newStubCode, rejoinOffset_);
  PatchJump(rejoinJump, ic_->rejoinLabel());
||||||| merged common ancestors
    masm.branchTestObjCompartment(Assembler::NotEqual, obj, compartment, scratch,
                                  failure->label());
    return true;
}
=======
  masm.branchTestObjCompartment(Assembler::NotEqual, obj, compartment, scratch,
                                failure->label());
  return true;
}
>>>>>>> upstream-releases

<<<<<<< HEAD
  for (CodeOffset offset : nextCodeOffsets_) {
    Assembler::PatchDataWithValueCheck(CodeLocationLabel(newStubCode, offset),
                                       ImmPtr(stub_->nextCodeRawPtr()),
                                       ImmPtr((void*)-1));
  }
  if (stubJitCodeOffset_) {
    Assembler::PatchDataWithValueCheck(
        CodeLocationLabel(newStubCode, *stubJitCodeOffset_),
        ImmPtr(newStubCode.get()), ImmPtr((void*)-1));
  }
||||||| merged common ancestors
bool
IonCacheIRCompiler::emitGuardAnyClass()
{
    ObjOperandId objId = reader.objOperandId();
    Register obj = allocator.useRegister(masm, objId);
    AutoScratchRegister scratch(allocator, masm);

    const Class* clasp = classStubField(reader.stubOffset());

    FailurePath* failure;
    if (!addFailurePath(&failure)) {
        return false;
    }

    if (objectGuardNeedsSpectreMitigations(objId)) {
        masm.branchTestObjClass(Assembler::NotEqual, obj, clasp, scratch, obj, failure->label());
    } else {
        masm.branchTestObjClassNoSpectreMitigations(Assembler::NotEqual, obj, clasp, scratch,
                                                    failure->label());
    }
=======
bool IonCacheIRCompiler::emitGuardAnyClass() {
  JitSpew(JitSpew_Codegen, __FUNCTION__);
  ObjOperandId objId = reader.objOperandId();
  Register obj = allocator.useRegister(masm, objId);
  AutoScratchRegister scratch(allocator, masm);

  const Class* clasp = classStubField(reader.stubOffset());

  FailurePath* failure;
  if (!addFailurePath(&failure)) {
    return false;
  }

  if (objectGuardNeedsSpectreMitigations(objId)) {
    masm.branchTestObjClass(Assembler::NotEqual, obj, clasp, scratch, obj,
                            failure->label());
  } else {
    masm.branchTestObjClassNoSpectreMitigations(Assembler::NotEqual, obj, clasp,
                                                scratch, failure->label());
  }
>>>>>>> upstream-releases

<<<<<<< HEAD
  return newStubCode;
||||||| merged common ancestors
    return true;
=======
  return true;
>>>>>>> upstream-releases
}

<<<<<<< HEAD
bool IonCacheIRCompiler::emitGuardShape() {
  ObjOperandId objId = reader.objOperandId();
  Register obj = allocator.useRegister(masm, objId);
  Shape* shape = shapeStubField(reader.stubOffset());
||||||| merged common ancestors
bool
IonCacheIRCompiler::emitGuardHasProxyHandler()
{
    Register obj = allocator.useRegister(masm, reader.objOperandId());
    const void* handler = proxyHandlerStubField(reader.stubOffset());
=======
bool IonCacheIRCompiler::emitGuardHasProxyHandler() {
  JitSpew(JitSpew_Codegen, __FUNCTION__);
  Register obj = allocator.useRegister(masm, reader.objOperandId());
  const void* handler = proxyHandlerStubField(reader.stubOffset());
>>>>>>> upstream-releases

<<<<<<< HEAD
  bool needSpectreMitigations = objectGuardNeedsSpectreMitigations(objId);
||||||| merged common ancestors
    FailurePath* failure;
    if (!addFailurePath(&failure)) {
        return false;
    }
=======
  FailurePath* failure;
  if (!addFailurePath(&failure)) {
    return false;
  }
>>>>>>> upstream-releases

<<<<<<< HEAD
  Maybe<AutoScratchRegister> maybeScratch;
  if (needSpectreMitigations) {
    maybeScratch.emplace(allocator, masm);
  }
||||||| merged common ancestors
    Address handlerAddr(obj, ProxyObject::offsetOfHandler());
    masm.branchPtr(Assembler::NotEqual, handlerAddr, ImmPtr(handler), failure->label());
    return true;
}
=======
  Address handlerAddr(obj, ProxyObject::offsetOfHandler());
  masm.branchPtr(Assembler::NotEqual, handlerAddr, ImmPtr(handler),
                 failure->label());
  return true;
}
>>>>>>> upstream-releases

<<<<<<< HEAD
  FailurePath* failure;
  if (!addFailurePath(&failure)) {
    return false;
  }
||||||| merged common ancestors
bool
IonCacheIRCompiler::emitGuardSpecificObject()
{
    Register obj = allocator.useRegister(masm, reader.objOperandId());
    JSObject* expected = objectStubField(reader.stubOffset());
=======
bool IonCacheIRCompiler::emitGuardSpecificObject() {
  JitSpew(JitSpew_Codegen, __FUNCTION__);
  Register obj = allocator.useRegister(masm, reader.objOperandId());
  JSObject* expected = objectStubField(reader.stubOffset());
>>>>>>> upstream-releases

<<<<<<< HEAD
  if (needSpectreMitigations) {
    masm.branchTestObjShape(Assembler::NotEqual, obj, shape, *maybeScratch, obj,
                            failure->label());
  } else {
    masm.branchTestObjShapeNoSpectreMitigations(Assembler::NotEqual, obj, shape,
                                                failure->label());
  }
||||||| merged common ancestors
    FailurePath* failure;
    if (!addFailurePath(&failure)) {
        return false;
    }
=======
  FailurePath* failure;
  if (!addFailurePath(&failure)) {
    return false;
  }
>>>>>>> upstream-releases

<<<<<<< HEAD
  return true;
||||||| merged common ancestors
    masm.branchPtr(Assembler::NotEqual, obj, ImmGCPtr(expected), failure->label());
    return true;
=======
  masm.branchPtr(Assembler::NotEqual, obj, ImmGCPtr(expected),
                 failure->label());
  return true;
>>>>>>> upstream-releases
}

<<<<<<< HEAD
bool IonCacheIRCompiler::emitGuardGroup() {
  ObjOperandId objId = reader.objOperandId();
  Register obj = allocator.useRegister(masm, objId);
  ObjectGroup* group = groupStubField(reader.stubOffset());
||||||| merged common ancestors
bool
IonCacheIRCompiler::emitGuardSpecificAtom()
{
    Register str = allocator.useRegister(masm, reader.stringOperandId());
    AutoScratchRegister scratch(allocator, masm);
=======
bool IonCacheIRCompiler::emitGuardSpecificAtom() {
  JitSpew(JitSpew_Codegen, __FUNCTION__);
  Register str = allocator.useRegister(masm, reader.stringOperandId());
  AutoScratchRegister scratch(allocator, masm);
>>>>>>> upstream-releases

<<<<<<< HEAD
  bool needSpectreMitigations = objectGuardNeedsSpectreMitigations(objId);
||||||| merged common ancestors
    JSAtom* atom = &stringStubField(reader.stubOffset())->asAtom();
=======
  JSAtom* atom = &stringStubField(reader.stubOffset())->asAtom();
>>>>>>> upstream-releases

<<<<<<< HEAD
  Maybe<AutoScratchRegister> maybeScratch;
  if (needSpectreMitigations) {
    maybeScratch.emplace(allocator, masm);
  }
||||||| merged common ancestors
    FailurePath* failure;
    if (!addFailurePath(&failure)) {
        return false;
    }
=======
  FailurePath* failure;
  if (!addFailurePath(&failure)) {
    return false;
  }
>>>>>>> upstream-releases

<<<<<<< HEAD
  FailurePath* failure;
  if (!addFailurePath(&failure)) {
    return false;
  }
||||||| merged common ancestors
    Label done;
    masm.branchPtr(Assembler::Equal, str, ImmGCPtr(atom), &done);
=======
  Label done;
  masm.branchPtr(Assembler::Equal, str, ImmGCPtr(atom), &done);
>>>>>>> upstream-releases

<<<<<<< HEAD
  if (needSpectreMitigations) {
    masm.branchTestObjGroup(Assembler::NotEqual, obj, group, *maybeScratch, obj,
                            failure->label());
  } else {
    masm.branchTestObjGroupNoSpectreMitigations(Assembler::NotEqual, obj, group,
                                                failure->label());
  }
||||||| merged common ancestors
    // The pointers are not equal, so if the input string is also an atom it
    // must be a different string.
    masm.branchTest32(Assembler::Zero, Address(str, JSString::offsetOfFlags()),
                      Imm32(JSString::NON_ATOM_BIT), failure->label());
=======
  // The pointers are not equal, so if the input string is also an atom it
  // must be a different string.
  masm.branchTest32(Assembler::Zero, Address(str, JSString::offsetOfFlags()),
                    Imm32(JSString::NON_ATOM_BIT), failure->label());
>>>>>>> upstream-releases

<<<<<<< HEAD
  return true;
}
||||||| merged common ancestors
    // Check the length.
    masm.branch32(Assembler::NotEqual, Address(str, JSString::offsetOfLength()),
                  Imm32(atom->length()), failure->label());
=======
  // Check the length.
  masm.branch32(Assembler::NotEqual, Address(str, JSString::offsetOfLength()),
                Imm32(atom->length()), failure->label());
>>>>>>> upstream-releases

<<<<<<< HEAD
bool IonCacheIRCompiler::emitGuardProto() {
  Register obj = allocator.useRegister(masm, reader.objOperandId());
  JSObject* proto = objectStubField(reader.stubOffset());
||||||| merged common ancestors
    // We have a non-atomized string with the same length. Call a helper
    // function to do the comparison.
    LiveRegisterSet volatileRegs(GeneralRegisterSet::Volatile(), liveVolatileFloatRegs());
    masm.PushRegsInMask(volatileRegs);
=======
  // We have a non-atomized string with the same length. Call a helper
  // function to do the comparison.
  LiveRegisterSet volatileRegs(GeneralRegisterSet::Volatile(),
                               liveVolatileFloatRegs());
  masm.PushRegsInMask(volatileRegs);
>>>>>>> upstream-releases

<<<<<<< HEAD
  AutoScratchRegister scratch(allocator, masm);
||||||| merged common ancestors
    masm.setupUnalignedABICall(scratch);
    masm.movePtr(ImmGCPtr(atom), scratch);
    masm.passABIArg(scratch);
    masm.passABIArg(str);
    masm.callWithABI(JS_FUNC_TO_DATA_PTR(void*, EqualStringsHelperPure));
    masm.mov(ReturnReg, scratch);
=======
  masm.setupUnalignedABICall(scratch);
  masm.movePtr(ImmGCPtr(atom), scratch);
  masm.passABIArg(scratch);
  masm.passABIArg(str);
  masm.callWithABI(JS_FUNC_TO_DATA_PTR(void*, EqualStringsHelperPure));
  masm.mov(ReturnReg, scratch);
>>>>>>> upstream-releases

<<<<<<< HEAD
  FailurePath* failure;
  if (!addFailurePath(&failure)) {
    return false;
  }
||||||| merged common ancestors
    LiveRegisterSet ignore;
    ignore.add(scratch);
    masm.PopRegsInMaskIgnore(volatileRegs, ignore);
    masm.branchIfFalseBool(scratch, failure->label());
=======
  LiveRegisterSet ignore;
  ignore.add(scratch);
  masm.PopRegsInMaskIgnore(volatileRegs, ignore);
  masm.branchIfFalseBool(scratch, failure->label());
>>>>>>> upstream-releases

<<<<<<< HEAD
  masm.loadObjProto(obj, scratch);
  masm.branchPtr(Assembler::NotEqual, scratch, ImmGCPtr(proto),
                 failure->label());
  return true;
||||||| merged common ancestors
    masm.bind(&done);
    return true;
=======
  masm.bind(&done);
  return true;
>>>>>>> upstream-releases
}

<<<<<<< HEAD
bool IonCacheIRCompiler::emitGuardCompartment() {
  Register obj = allocator.useRegister(masm, reader.objOperandId());
  JSObject* globalWrapper = objectStubField(reader.stubOffset());
  JS::Compartment* compartment = compartmentStubField(reader.stubOffset());
  AutoScratchRegister scratch(allocator, masm);
||||||| merged common ancestors
bool
IonCacheIRCompiler::emitGuardSpecificSymbol()
{
    Register sym = allocator.useRegister(masm, reader.symbolOperandId());
    JS::Symbol* expected = symbolStubField(reader.stubOffset());
=======
bool IonCacheIRCompiler::emitGuardSpecificSymbol() {
  JitSpew(JitSpew_Codegen, __FUNCTION__);
  Register sym = allocator.useRegister(masm, reader.symbolOperandId());
  JS::Symbol* expected = symbolStubField(reader.stubOffset());
>>>>>>> upstream-releases

  FailurePath* failure;
  if (!addFailurePath(&failure)) {
    return false;
  }

<<<<<<< HEAD
  // Verify that the global wrapper is still valid, as
  // it is pre-requisite for doing the compartment check.
  masm.movePtr(ImmGCPtr(globalWrapper), scratch);
  Address handlerAddr(scratch, ProxyObject::offsetOfHandler());
  masm.branchPtr(Assembler::Equal, handlerAddr,
                 ImmPtr(&DeadObjectProxy::singleton), failure->label());
||||||| merged common ancestors
    masm.branchPtr(Assembler::NotEqual, sym, ImmGCPtr(expected), failure->label());
    return true;
}
=======
  masm.branchPtr(Assembler::NotEqual, sym, ImmGCPtr(expected),
                 failure->label());
  return true;
}
>>>>>>> upstream-releases

<<<<<<< HEAD
  masm.branchTestObjCompartment(Assembler::NotEqual, obj, compartment, scratch,
                                failure->label());
  return true;
||||||| merged common ancestors
bool
IonCacheIRCompiler::emitLoadValueResult()
{
   MOZ_CRASH("Baseline-specific op");
=======
bool IonCacheIRCompiler::emitLoadValueResult() {
  MOZ_CRASH("Baseline-specific op");
>>>>>>> upstream-releases
}

<<<<<<< HEAD
bool IonCacheIRCompiler::emitGuardAnyClass() {
  ObjOperandId objId = reader.objOperandId();
  Register obj = allocator.useRegister(masm, objId);
  AutoScratchRegister scratch(allocator, masm);

  const Class* clasp = classStubField(reader.stubOffset());
||||||| merged common ancestors

bool
IonCacheIRCompiler::emitLoadFixedSlotResult()
{
    AutoOutputRegister output(*this);
    Register obj = allocator.useRegister(masm, reader.objOperandId());
    int32_t offset = int32StubField(reader.stubOffset());
    masm.loadTypedOrValue(Address(obj, offset), output);
    return true;
}
=======
bool IonCacheIRCompiler::emitLoadFixedSlotResult() {
  JitSpew(JitSpew_Codegen, __FUNCTION__);
  AutoOutputRegister output(*this);
  Register obj = allocator.useRegister(masm, reader.objOperandId());
  int32_t offset = int32StubField(reader.stubOffset());
  masm.loadTypedOrValue(Address(obj, offset), output);
  return true;
}
>>>>>>> upstream-releases

<<<<<<< HEAD
  FailurePath* failure;
  if (!addFailurePath(&failure)) {
    return false;
  }
||||||| merged common ancestors
bool
IonCacheIRCompiler::emitLoadDynamicSlotResult()
{
    AutoOutputRegister output(*this);
    Register obj = allocator.useRegister(masm, reader.objOperandId());
    int32_t offset = int32StubField(reader.stubOffset());
=======
bool IonCacheIRCompiler::emitLoadDynamicSlotResult() {
  JitSpew(JitSpew_Codegen, __FUNCTION__);
  AutoOutputRegister output(*this);
  Register obj = allocator.useRegister(masm, reader.objOperandId());
  int32_t offset = int32StubField(reader.stubOffset());
>>>>>>> upstream-releases

<<<<<<< HEAD
  if (objectGuardNeedsSpectreMitigations(objId)) {
    masm.branchTestObjClass(Assembler::NotEqual, obj, clasp, scratch, obj,
                            failure->label());
  } else {
    masm.branchTestObjClassNoSpectreMitigations(Assembler::NotEqual, obj, clasp,
                                                scratch, failure->label());
  }

  return true;
||||||| merged common ancestors
    AutoScratchRegisterMaybeOutput scratch(allocator, masm, output);
    masm.loadPtr(Address(obj, NativeObject::offsetOfSlots()), scratch);
    masm.loadTypedOrValue(Address(scratch, offset), output);
    return true;
=======
  AutoScratchRegisterMaybeOutput scratch(allocator, masm, output);
  masm.loadPtr(Address(obj, NativeObject::offsetOfSlots()), scratch);
  masm.loadTypedOrValue(Address(scratch, offset), output);
  return true;
>>>>>>> upstream-releases
}

<<<<<<< HEAD
bool IonCacheIRCompiler::emitGuardHasProxyHandler() {
  Register obj = allocator.useRegister(masm, reader.objOperandId());
  const void* handler = proxyHandlerStubField(reader.stubOffset());

  FailurePath* failure;
  if (!addFailurePath(&failure)) {
    return false;
  }

  Address handlerAddr(obj, ProxyObject::offsetOfHandler());
  masm.branchPtr(Assembler::NotEqual, handlerAddr, ImmPtr(handler),
                 failure->label());
  return true;
}

bool IonCacheIRCompiler::emitGuardSpecificObject() {
  Register obj = allocator.useRegister(masm, reader.objOperandId());
  JSObject* expected = objectStubField(reader.stubOffset());

  FailurePath* failure;
  if (!addFailurePath(&failure)) {
    return false;
  }

  masm.branchPtr(Assembler::NotEqual, obj, ImmGCPtr(expected),
                 failure->label());
  return true;
}
||||||| merged common ancestors
bool
IonCacheIRCompiler::emitGuardHasGetterSetter()
{
    Register obj = allocator.useRegister(masm, reader.objOperandId());
    Shape* shape = shapeStubField(reader.stubOffset());

    AutoScratchRegister scratch1(allocator, masm);
    AutoScratchRegister scratch2(allocator, masm);

    FailurePath* failure;
    if (!addFailurePath(&failure)) {
        return false;
    }

    LiveRegisterSet volatileRegs(GeneralRegisterSet::Volatile(), liveVolatileFloatRegs());
    volatileRegs.takeUnchecked(scratch1);
    volatileRegs.takeUnchecked(scratch2);
    masm.PushRegsInMask(volatileRegs);

    masm.setupUnalignedABICall(scratch1);
    masm.loadJSContext(scratch1);
    masm.passABIArg(scratch1);
    masm.passABIArg(obj);
    masm.movePtr(ImmGCPtr(shape), scratch2);
    masm.passABIArg(scratch2);
    masm.callWithABI(JS_FUNC_TO_DATA_PTR(void*, ObjectHasGetterSetterPure));
    masm.mov(ReturnReg, scratch1);
    masm.PopRegsInMask(volatileRegs);

    masm.branchIfFalseBool(scratch1, failure->label());
    return true;
}
=======
bool IonCacheIRCompiler::emitGuardHasGetterSetter() {
  JitSpew(JitSpew_Codegen, __FUNCTION__);
  Register obj = allocator.useRegister(masm, reader.objOperandId());
  Shape* shape = shapeStubField(reader.stubOffset());
>>>>>>> upstream-releases

<<<<<<< HEAD
bool IonCacheIRCompiler::emitGuardSpecificAtom() {
  Register str = allocator.useRegister(masm, reader.stringOperandId());
  AutoScratchRegister scratch(allocator, masm);
||||||| merged common ancestors
bool
IonCacheIRCompiler::emitCallScriptedGetterResult()
{
    AutoSaveLiveRegisters save(*this);
    AutoOutputRegister output(*this);
=======
  AutoScratchRegister scratch1(allocator, masm);
  AutoScratchRegister scratch2(allocator, masm);
>>>>>>> upstream-releases

<<<<<<< HEAD
  JSAtom* atom = &stringStubField(reader.stubOffset())->asAtom();
||||||| merged common ancestors
    Register obj = allocator.useRegister(masm, reader.objOperandId());
    JSFunction* target = &objectStubField(reader.stubOffset())->as<JSFunction>();
    AutoScratchRegister scratch(allocator, masm);
=======
  FailurePath* failure;
  if (!addFailurePath(&failure)) {
    return false;
  }

  LiveRegisterSet volatileRegs(GeneralRegisterSet::Volatile(),
                               liveVolatileFloatRegs());
  volatileRegs.takeUnchecked(scratch1);
  volatileRegs.takeUnchecked(scratch2);
  masm.PushRegsInMask(volatileRegs);

  masm.setupUnalignedABICall(scratch1);
  masm.loadJSContext(scratch1);
  masm.passABIArg(scratch1);
  masm.passABIArg(obj);
  masm.movePtr(ImmGCPtr(shape), scratch2);
  masm.passABIArg(scratch2);
  masm.callWithABI(JS_FUNC_TO_DATA_PTR(void*, ObjectHasGetterSetterPure));
  masm.mov(ReturnReg, scratch1);
  masm.PopRegsInMask(volatileRegs);

  masm.branchIfFalseBool(scratch1, failure->label());
  return true;
}

bool IonCacheIRCompiler::emitCallScriptedGetterResultShared(
    TypedOrValueRegister receiver, TypedOrValueRegister output) {
  JSFunction* target = &objectStubField(reader.stubOffset())->as<JSFunction>();
  AutoScratchRegister scratch(allocator, masm);

  bool isSameRealm = reader.readBool();
  MOZ_ASSERT(isSameRealm == (cx_->realm() == target->realm()));

  allocator.discardStack(masm);

  uint32_t framePushedBefore = masm.framePushed();

  // Construct IonICCallFrameLayout.
  uint32_t descriptor = MakeFrameDescriptor(
      masm.framePushed(), FrameType::IonJS, IonICCallFrameLayout::Size());
  pushStubCodePointer();
  masm.Push(Imm32(descriptor));
  masm.Push(ImmPtr(GetReturnAddressToIonCode(cx_)));

  // The JitFrameLayout pushed below will be aligned to JitStackAlignment,
  // so we just have to make sure the stack is aligned after we push the
  // |this| + argument Values.
  uint32_t argSize = (target->nargs() + 1) * sizeof(Value);
  uint32_t padding =
      ComputeByteAlignment(masm.framePushed() + argSize, JitStackAlignment);
  MOZ_ASSERT(padding % sizeof(uintptr_t) == 0);
  MOZ_ASSERT(padding < JitStackAlignment);
  masm.reserveStack(padding);

  for (size_t i = 0; i < target->nargs(); i++) {
    masm.Push(UndefinedValue());
  }
  masm.Push(receiver);
>>>>>>> upstream-releases

<<<<<<< HEAD
  FailurePath* failure;
  if (!addFailurePath(&failure)) {
    return false;
  }
||||||| merged common ancestors
    bool isCrossRealm = reader.readBool();
    MOZ_ASSERT(isCrossRealm == (cx_->realm() != target->realm()));
=======
  if (!isSameRealm) {
    masm.switchToRealm(target->realm(), scratch);
  }
>>>>>>> upstream-releases

<<<<<<< HEAD
  Label done;
  masm.branchPtr(Assembler::Equal, str, ImmGCPtr(atom), &done);
||||||| merged common ancestors
    allocator.discardStack(masm);
=======
  masm.movePtr(ImmGCPtr(target), scratch);
>>>>>>> upstream-releases

<<<<<<< HEAD
  // The pointers are not equal, so if the input string is also an atom it
  // must be a different string.
  masm.branchTest32(Assembler::Zero, Address(str, JSString::offsetOfFlags()),
                    Imm32(JSString::NON_ATOM_BIT), failure->label());
||||||| merged common ancestors
    uint32_t framePushedBefore = masm.framePushed();
=======
  descriptor = MakeFrameDescriptor(argSize + padding, FrameType::IonICCall,
                                   JitFrameLayout::Size());
  masm.Push(Imm32(0));  // argc
  masm.Push(scratch);
  masm.Push(Imm32(descriptor));
>>>>>>> upstream-releases

<<<<<<< HEAD
  // Check the length.
  masm.branch32(Assembler::NotEqual, Address(str, JSString::offsetOfLength()),
                Imm32(atom->length()), failure->label());
||||||| merged common ancestors
    // Construct IonICCallFrameLayout.
    uint32_t descriptor = MakeFrameDescriptor(masm.framePushed(), FrameType::IonJS,
                                              IonICCallFrameLayout::Size());
    pushStubCodePointer();
    masm.Push(Imm32(descriptor));
    masm.Push(ImmPtr(GetReturnAddressToIonCode(cx_)));
=======
  // Check stack alignment. Add sizeof(uintptr_t) for the return address.
  MOZ_ASSERT(((masm.framePushed() + sizeof(uintptr_t)) % JitStackAlignment) ==
             0);
>>>>>>> upstream-releases

<<<<<<< HEAD
  // We have a non-atomized string with the same length. Call a helper
  // function to do the comparison.
  LiveRegisterSet volatileRegs(GeneralRegisterSet::Volatile(),
                               liveVolatileFloatRegs());
  masm.PushRegsInMask(volatileRegs);
||||||| merged common ancestors
    // The JitFrameLayout pushed below will be aligned to JitStackAlignment,
    // so we just have to make sure the stack is aligned after we push the
    // |this| + argument Values.
    uint32_t argSize = (target->nargs() + 1) * sizeof(Value);
    uint32_t padding = ComputeByteAlignment(masm.framePushed() + argSize, JitStackAlignment);
    MOZ_ASSERT(padding % sizeof(uintptr_t) == 0);
    MOZ_ASSERT(padding < JitStackAlignment);
    masm.reserveStack(padding);
=======
  // The getter currently has a jit entry or a non-lazy script. We will only
  // relazify when we do a shrinking GC and when that happens we will also
  // purge IC stubs.
  MOZ_ASSERT(target->hasJitEntry());
  masm.loadJitCodeRaw(scratch, scratch);
  masm.callJit(scratch);
>>>>>>> upstream-releases

<<<<<<< HEAD
  masm.setupUnalignedABICall(scratch);
  masm.movePtr(ImmGCPtr(atom), scratch);
  masm.passABIArg(scratch);
  masm.passABIArg(str);
  masm.callWithABI(JS_FUNC_TO_DATA_PTR(void*, EqualStringsHelperPure));
  masm.mov(ReturnReg, scratch);
||||||| merged common ancestors
    for (size_t i = 0; i < target->nargs(); i++) {
        masm.Push(UndefinedValue());
    }
    masm.Push(TypedOrValueRegister(MIRType::Object, AnyRegister(obj)));
=======
  if (!isSameRealm) {
    static_assert(!JSReturnOperand.aliases(ReturnReg),
                  "ReturnReg available as scratch after scripted calls");
    masm.switchToRealm(cx_->realm(), ReturnReg);
  }
>>>>>>> upstream-releases

<<<<<<< HEAD
  LiveRegisterSet ignore;
  ignore.add(scratch);
  masm.PopRegsInMaskIgnore(volatileRegs, ignore);
  masm.branchIfFalseBool(scratch, failure->label());
||||||| merged common ancestors
    if (isCrossRealm) {
        masm.switchToRealm(target->realm(), scratch);
    }
=======
  masm.storeCallResultValue(output);
  masm.freeStack(masm.framePushed() - framePushedBefore);
  return true;
}
>>>>>>> upstream-releases

<<<<<<< HEAD
  masm.bind(&done);
  return true;
}
||||||| merged common ancestors
    masm.movePtr(ImmGCPtr(target), scratch);
=======
bool IonCacheIRCompiler::emitCallScriptedGetterResult() {
  JitSpew(JitSpew_Codegen, __FUNCTION__);
  AutoSaveLiveRegisters save(*this);
  AutoOutputRegister output(*this);
>>>>>>> upstream-releases

<<<<<<< HEAD
bool IonCacheIRCompiler::emitGuardSpecificSymbol() {
  Register sym = allocator.useRegister(masm, reader.symbolOperandId());
  JS::Symbol* expected = symbolStubField(reader.stubOffset());
||||||| merged common ancestors
    descriptor = MakeFrameDescriptor(argSize + padding, FrameType::IonICCall,
                                     JitFrameLayout::Size());
    masm.Push(Imm32(0)); // argc
    masm.Push(scratch);
    masm.Push(Imm32(descriptor));
=======
  Register obj = allocator.useRegister(masm, reader.objOperandId());
>>>>>>> upstream-releases

<<<<<<< HEAD
  FailurePath* failure;
  if (!addFailurePath(&failure)) {
    return false;
  }
||||||| merged common ancestors
    // Check stack alignment. Add sizeof(uintptr_t) for the return address.
    MOZ_ASSERT(((masm.framePushed() + sizeof(uintptr_t)) % JitStackAlignment) == 0);
=======
  return emitCallScriptedGetterResultShared(
      TypedOrValueRegister(MIRType::Object, AnyRegister(obj)), output);
}
>>>>>>> upstream-releases

<<<<<<< HEAD
  masm.branchPtr(Assembler::NotEqual, sym, ImmGCPtr(expected),
                 failure->label());
  return true;
}
||||||| merged common ancestors
    // The getter currently has a jit entry or a non-lazy script. We will only
    // relazify when we do a shrinking GC and when that happens we will also
    // purge IC stubs.
    MOZ_ASSERT(target->hasJitEntry());
    masm.loadJitCodeRaw(scratch, scratch);
    masm.callJit(scratch);
=======
bool IonCacheIRCompiler::emitCallScriptedGetterByValueResult() {
  JitSpew(JitSpew_Codegen, __FUNCTION__);
  AutoSaveLiveRegisters save(*this);
  AutoOutputRegister output(*this);
>>>>>>> upstream-releases

<<<<<<< HEAD
bool IonCacheIRCompiler::emitLoadValueResult() {
  MOZ_CRASH("Baseline-specific op");
}
||||||| merged common ancestors
    if (isCrossRealm) {
        static_assert(!JSReturnOperand.aliases(ReturnReg),
                      "ReturnReg available as scratch after scripted calls");
        masm.switchToRealm(cx_->realm(), ReturnReg);
    }
=======
  ValueOperand val = allocator.useValueRegister(masm, reader.valOperandId());
>>>>>>> upstream-releases

<<<<<<< HEAD
bool IonCacheIRCompiler::emitLoadFixedSlotResult() {
  AutoOutputRegister output(*this);
  Register obj = allocator.useRegister(masm, reader.objOperandId());
  int32_t offset = int32StubField(reader.stubOffset());
  masm.loadTypedOrValue(Address(obj, offset), output);
  return true;
||||||| merged common ancestors
    masm.storeCallResultValue(output);
    masm.freeStack(masm.framePushed() - framePushedBefore);
    return true;
=======
  return emitCallScriptedGetterResultShared(val, output);
>>>>>>> upstream-releases
}

<<<<<<< HEAD
bool IonCacheIRCompiler::emitLoadDynamicSlotResult() {
  AutoOutputRegister output(*this);
  Register obj = allocator.useRegister(masm, reader.objOperandId());
  int32_t offset = int32StubField(reader.stubOffset());

  AutoScratchRegisterMaybeOutput scratch(allocator, masm, output);
  masm.loadPtr(Address(obj, NativeObject::offsetOfSlots()), scratch);
  masm.loadTypedOrValue(Address(scratch, offset), output);
  return true;
}
||||||| merged common ancestors
bool
IonCacheIRCompiler::emitCallNativeGetterResult()
{
    AutoSaveLiveRegisters save(*this);
    AutoOutputRegister output(*this);

    Register obj = allocator.useRegister(masm, reader.objOperandId());
    JSFunction* target = &objectStubField(reader.stubOffset())->as<JSFunction>();
    MOZ_ASSERT(target->isNative());
=======
bool IonCacheIRCompiler::emitCallNativeGetterResultShared(
    TypedOrValueRegister receiver, const AutoOutputRegister& output,
    AutoSaveLiveRegisters& save) {
  JSFunction* target = &objectStubField(reader.stubOffset())->as<JSFunction>();
  MOZ_ASSERT(target->isNative());
>>>>>>> upstream-releases

<<<<<<< HEAD
bool IonCacheIRCompiler::emitGuardHasGetterSetter() {
  Register obj = allocator.useRegister(masm, reader.objOperandId());
  Shape* shape = shapeStubField(reader.stubOffset());
||||||| merged common ancestors
    AutoScratchRegister argJSContext(allocator, masm);
    AutoScratchRegister argUintN(allocator, masm);
    AutoScratchRegister argVp(allocator, masm);
    AutoScratchRegister scratch(allocator, masm);
=======
  AutoScratchRegisterMaybeOutput argJSContext(allocator, masm, output);
  AutoScratchRegister argUintN(allocator, masm);
  AutoScratchRegister argVp(allocator, masm);
  AutoScratchRegister scratch(allocator, masm);
>>>>>>> upstream-releases

<<<<<<< HEAD
  AutoScratchRegister scratch1(allocator, masm);
  AutoScratchRegister scratch2(allocator, masm);
||||||| merged common ancestors
    allocator.discardStack(masm);
=======
  allocator.discardStack(masm);
>>>>>>> upstream-releases

<<<<<<< HEAD
  FailurePath* failure;
  if (!addFailurePath(&failure)) {
    return false;
  }

  LiveRegisterSet volatileRegs(GeneralRegisterSet::Volatile(),
                               liveVolatileFloatRegs());
  volatileRegs.takeUnchecked(scratch1);
  volatileRegs.takeUnchecked(scratch2);
  masm.PushRegsInMask(volatileRegs);

  masm.setupUnalignedABICall(scratch1);
  masm.loadJSContext(scratch1);
  masm.passABIArg(scratch1);
  masm.passABIArg(obj);
  masm.movePtr(ImmGCPtr(shape), scratch2);
  masm.passABIArg(scratch2);
  masm.callWithABI(JS_FUNC_TO_DATA_PTR(void*, ObjectHasGetterSetterPure));
  masm.mov(ReturnReg, scratch1);
  masm.PopRegsInMask(volatileRegs);

  masm.branchIfFalseBool(scratch1, failure->label());
  return true;
}

bool IonCacheIRCompiler::emitCallScriptedGetterResult() {
  AutoSaveLiveRegisters save(*this);
  AutoOutputRegister output(*this);

  Register obj = allocator.useRegister(masm, reader.objOperandId());
  JSFunction* target = &objectStubField(reader.stubOffset())->as<JSFunction>();
  AutoScratchRegister scratch(allocator, masm);

  bool isCrossRealm = reader.readBool();
  MOZ_ASSERT(isCrossRealm == (cx_->realm() != target->realm()));

  allocator.discardStack(masm);

  uint32_t framePushedBefore = masm.framePushed();

  // Construct IonICCallFrameLayout.
  uint32_t descriptor = MakeFrameDescriptor(
      masm.framePushed(), FrameType::IonJS, IonICCallFrameLayout::Size());
  pushStubCodePointer();
  masm.Push(Imm32(descriptor));
  masm.Push(ImmPtr(GetReturnAddressToIonCode(cx_)));

  // The JitFrameLayout pushed below will be aligned to JitStackAlignment,
  // so we just have to make sure the stack is aligned after we push the
  // |this| + argument Values.
  uint32_t argSize = (target->nargs() + 1) * sizeof(Value);
  uint32_t padding =
      ComputeByteAlignment(masm.framePushed() + argSize, JitStackAlignment);
  MOZ_ASSERT(padding % sizeof(uintptr_t) == 0);
  MOZ_ASSERT(padding < JitStackAlignment);
  masm.reserveStack(padding);

  for (size_t i = 0; i < target->nargs(); i++) {
    masm.Push(UndefinedValue());
  }
  masm.Push(TypedOrValueRegister(MIRType::Object, AnyRegister(obj)));

  if (isCrossRealm) {
    masm.switchToRealm(target->realm(), scratch);
  }

  masm.movePtr(ImmGCPtr(target), scratch);

  descriptor = MakeFrameDescriptor(argSize + padding, FrameType::IonICCall,
                                   JitFrameLayout::Size());
  masm.Push(Imm32(0));  // argc
  masm.Push(scratch);
  masm.Push(Imm32(descriptor));

  // Check stack alignment. Add sizeof(uintptr_t) for the return address.
  MOZ_ASSERT(((masm.framePushed() + sizeof(uintptr_t)) % JitStackAlignment) ==
             0);

  // The getter currently has a jit entry or a non-lazy script. We will only
  // relazify when we do a shrinking GC and when that happens we will also
  // purge IC stubs.
  MOZ_ASSERT(target->hasJitEntry());
  masm.loadJitCodeRaw(scratch, scratch);
  masm.callJit(scratch);

  if (isCrossRealm) {
    static_assert(!JSReturnOperand.aliases(ReturnReg),
                  "ReturnReg available as scratch after scripted calls");
    masm.switchToRealm(cx_->realm(), ReturnReg);
  }

  masm.storeCallResultValue(output);
  masm.freeStack(masm.framePushed() - framePushedBefore);
  return true;
}

bool IonCacheIRCompiler::emitCallNativeGetterResult() {
  AutoSaveLiveRegisters save(*this);
  AutoOutputRegister output(*this);

  Register obj = allocator.useRegister(masm, reader.objOperandId());
  JSFunction* target = &objectStubField(reader.stubOffset())->as<JSFunction>();
  MOZ_ASSERT(target->isNative());

  AutoScratchRegister argJSContext(allocator, masm);
  AutoScratchRegister argUintN(allocator, masm);
  AutoScratchRegister argVp(allocator, masm);
  AutoScratchRegister scratch(allocator, masm);

  allocator.discardStack(masm);

  // Native functions have the signature:
  //  bool (*)(JSContext*, unsigned, Value* vp)
  // Where vp[0] is space for an outparam, vp[1] is |this|, and vp[2] onward
  // are the function arguments.

  // Construct vp array:
  // Push object value for |this|
  masm.Push(TypedOrValueRegister(MIRType::Object, AnyRegister(obj)));
  // Push callee/outparam.
  masm.Push(ObjectValue(*target));

  // Preload arguments into registers.
  masm.loadJSContext(argJSContext);
  masm.move32(Imm32(0), argUintN);
  masm.moveStackPtrTo(argVp.get());

  // Push marking data for later use.
  masm.Push(argUintN);
  pushStubCodePointer();

  if (!masm.icBuildOOLFakeExitFrame(GetReturnAddressToIonCode(cx_), save)) {
    return false;
  }
  masm.enterFakeExitFrame(argJSContext, scratch, ExitFrameType::IonOOLNative);
||||||| merged common ancestors
    // Native functions have the signature:
    //  bool (*)(JSContext*, unsigned, Value* vp)
    // Where vp[0] is space for an outparam, vp[1] is |this|, and vp[2] onward
    // are the function arguments.
=======
  // Native functions have the signature:
  //  bool (*)(JSContext*, unsigned, Value* vp)
  // Where vp[0] is space for an outparam, vp[1] is |this|, and vp[2] onward
  // are the function arguments.
>>>>>>> upstream-releases

<<<<<<< HEAD
  if (target->realm() != cx_->realm()) {
    masm.switchToRealm(target->realm(), scratch);
  }
||||||| merged common ancestors
    // Construct vp array:
    // Push object value for |this|
    masm.Push(TypedOrValueRegister(MIRType::Object, AnyRegister(obj)));
    // Push callee/outparam.
    masm.Push(ObjectValue(*target));
=======
  // Construct vp array:
  // Push receiver value for |this|
  masm.Push(receiver);
  // Push callee/outparam.
  masm.Push(ObjectValue(*target));
>>>>>>> upstream-releases

<<<<<<< HEAD
  // Construct and execute call.
  masm.setupUnalignedABICall(scratch);
  masm.passABIArg(argJSContext);
  masm.passABIArg(argUintN);
  masm.passABIArg(argVp);
  masm.callWithABI(JS_FUNC_TO_DATA_PTR(void*, target->native()),
                   MoveOp::GENERAL,
                   CheckUnsafeCallWithABI::DontCheckHasExitFrame);
||||||| merged common ancestors
    // Preload arguments into registers.
    masm.loadJSContext(argJSContext);
    masm.move32(Imm32(0), argUintN);
    masm.moveStackPtrTo(argVp.get());
=======
  // Preload arguments into registers.
  masm.loadJSContext(argJSContext);
  masm.move32(Imm32(0), argUintN);
  masm.moveStackPtrTo(argVp.get());
>>>>>>> upstream-releases

<<<<<<< HEAD
  // Test for failure.
  masm.branchIfFalseBool(ReturnReg, masm.exceptionLabel());
||||||| merged common ancestors
    // Push marking data for later use.
    masm.Push(argUintN);
    pushStubCodePointer();
=======
  // Push marking data for later use.
  masm.Push(argUintN);
  pushStubCodePointer();
>>>>>>> upstream-releases

<<<<<<< HEAD
  if (target->realm() != cx_->realm()) {
    masm.switchToRealm(cx_->realm(), ReturnReg);
  }
||||||| merged common ancestors
    if (!masm.icBuildOOLFakeExitFrame(GetReturnAddressToIonCode(cx_), save)) {
        return false;
    }
    masm.enterFakeExitFrame(argJSContext, scratch, ExitFrameType::IonOOLNative);
=======
  if (!masm.icBuildOOLFakeExitFrame(GetReturnAddressToIonCode(cx_), save)) {
    return false;
  }
  masm.enterFakeExitFrame(argJSContext, scratch, ExitFrameType::IonOOLNative);
>>>>>>> upstream-releases

<<<<<<< HEAD
  // Load the outparam vp[0] into output register(s).
  Address outparam(masm.getStackPointer(),
                   IonOOLNativeExitFrameLayout::offsetOfResult());
  masm.loadValue(outparam, output.valueReg());
||||||| merged common ancestors
    if (target->realm() != cx_->realm()) {
        masm.switchToRealm(target->realm(), scratch);
    }
=======
  if (target->realm() != cx_->realm()) {
    masm.switchToRealm(target->realm(), scratch);
  }
>>>>>>> upstream-releases

<<<<<<< HEAD
  if (JitOptions.spectreJitToCxxCalls) {
    masm.speculationBarrier();
  }
||||||| merged common ancestors
    // Construct and execute call.
    masm.setupUnalignedABICall(scratch);
    masm.passABIArg(argJSContext);
    masm.passABIArg(argUintN);
    masm.passABIArg(argVp);
    masm.callWithABI(JS_FUNC_TO_DATA_PTR(void*, target->native()), MoveOp::GENERAL,
                     CheckUnsafeCallWithABI::DontCheckHasExitFrame);
=======
  // Construct and execute call.
  masm.setupUnalignedABICall(scratch);
  masm.passABIArg(argJSContext);
  masm.passABIArg(argUintN);
  masm.passABIArg(argVp);
  masm.callWithABI(JS_FUNC_TO_DATA_PTR(void*, target->native()),
                   MoveOp::GENERAL,
                   CheckUnsafeCallWithABI::DontCheckHasExitFrame);
>>>>>>> upstream-releases

<<<<<<< HEAD
  masm.adjustStack(IonOOLNativeExitFrameLayout::Size(0));
  return true;
}
||||||| merged common ancestors
    // Test for failure.
    masm.branchIfFalseBool(ReturnReg, masm.exceptionLabel());
=======
  // Test for failure.
  masm.branchIfFalseBool(ReturnReg, masm.exceptionLabel());
>>>>>>> upstream-releases

<<<<<<< HEAD
bool IonCacheIRCompiler::emitCallProxyGetResult() {
  AutoSaveLiveRegisters save(*this);
  AutoOutputRegister output(*this);
||||||| merged common ancestors
    if (target->realm() != cx_->realm()) {
        masm.switchToRealm(cx_->realm(), ReturnReg);
    }
=======
  if (target->realm() != cx_->realm()) {
    masm.switchToRealm(cx_->realm(), ReturnReg);
  }
>>>>>>> upstream-releases

<<<<<<< HEAD
  Register obj = allocator.useRegister(masm, reader.objOperandId());
  jsid id = idStubField(reader.stubOffset());
||||||| merged common ancestors
    // Load the outparam vp[0] into output register(s).
    Address outparam(masm.getStackPointer(), IonOOLNativeExitFrameLayout::offsetOfResult());
    masm.loadValue(outparam, output.valueReg());
=======
  // Load the outparam vp[0] into output register(s).
  Address outparam(masm.getStackPointer(),
                   IonOOLNativeExitFrameLayout::offsetOfResult());
  masm.loadValue(outparam, output.valueReg());
>>>>>>> upstream-releases

<<<<<<< HEAD
  // ProxyGetProperty(JSContext* cx, HandleObject proxy, HandleId id,
  //                  MutableHandleValue vp)
  AutoScratchRegisterMaybeOutput argJSContext(allocator, masm, output);
  AutoScratchRegister argProxy(allocator, masm);
  AutoScratchRegister argId(allocator, masm);
  AutoScratchRegister argVp(allocator, masm);
  AutoScratchRegister scratch(allocator, masm);
||||||| merged common ancestors
    if (JitOptions.spectreJitToCxxCalls) {
        masm.speculationBarrier();
    }
=======
  if (JitOptions.spectreJitToCxxCalls) {
    masm.speculationBarrier();
  }
>>>>>>> upstream-releases

<<<<<<< HEAD
  allocator.discardStack(masm);
||||||| merged common ancestors
    masm.adjustStack(IonOOLNativeExitFrameLayout::Size(0));
    return true;
}
=======
  masm.adjustStack(IonOOLNativeExitFrameLayout::Size(0));
  return true;
}
>>>>>>> upstream-releases

<<<<<<< HEAD
  // Push stubCode for marking.
  pushStubCodePointer();

  // Push args on stack first so we can take pointers to make handles.
  masm.Push(UndefinedValue());
  masm.moveStackPtrTo(argVp.get());

  masm.Push(id, scratch);
  masm.moveStackPtrTo(argId.get());

  // Push the proxy. Also used as receiver.
  masm.Push(obj);
  masm.moveStackPtrTo(argProxy.get());

  masm.loadJSContext(argJSContext);

  if (!masm.icBuildOOLFakeExitFrame(GetReturnAddressToIonCode(cx_), save)) {
    return false;
  }
  masm.enterFakeExitFrame(argJSContext, scratch, ExitFrameType::IonOOLProxy);

  // Make the call.
  masm.setupUnalignedABICall(scratch);
  masm.passABIArg(argJSContext);
  masm.passABIArg(argProxy);
  masm.passABIArg(argId);
  masm.passABIArg(argVp);
  masm.callWithABI(JS_FUNC_TO_DATA_PTR(void*, ProxyGetProperty),
                   MoveOp::GENERAL,
                   CheckUnsafeCallWithABI::DontCheckHasExitFrame);
||||||| merged common ancestors
bool
IonCacheIRCompiler::emitCallProxyGetResult()
{
    AutoSaveLiveRegisters save(*this);
    AutoOutputRegister output(*this);

    Register obj = allocator.useRegister(masm, reader.objOperandId());
    jsid id = idStubField(reader.stubOffset());

    // ProxyGetProperty(JSContext* cx, HandleObject proxy, HandleId id,
    //                  MutableHandleValue vp)
    AutoScratchRegisterMaybeOutput argJSContext(allocator, masm, output);
    AutoScratchRegister argProxy(allocator, masm);
    AutoScratchRegister argId(allocator, masm);
    AutoScratchRegister argVp(allocator, masm);
    AutoScratchRegister scratch(allocator, masm);

    allocator.discardStack(masm);

    // Push stubCode for marking.
    pushStubCodePointer();

    // Push args on stack first so we can take pointers to make handles.
    masm.Push(UndefinedValue());
    masm.moveStackPtrTo(argVp.get());

    masm.Push(id, scratch);
    masm.moveStackPtrTo(argId.get());
=======
bool IonCacheIRCompiler::emitCallNativeGetterResult() {
  JitSpew(JitSpew_Codegen, __FUNCTION__);
  AutoSaveLiveRegisters save(*this);
  AutoOutputRegister output(*this);
>>>>>>> upstream-releases

<<<<<<< HEAD
  // Test for failure.
  masm.branchIfFalseBool(ReturnReg, masm.exceptionLabel());
||||||| merged common ancestors
    // Push the proxy. Also used as receiver.
    masm.Push(obj);
    masm.moveStackPtrTo(argProxy.get());
=======
  Register obj = allocator.useRegister(masm, reader.objOperandId());
>>>>>>> upstream-releases

<<<<<<< HEAD
  // Load the outparam vp[0] into output register(s).
  Address outparam(masm.getStackPointer(),
                   IonOOLProxyExitFrameLayout::offsetOfResult());
  masm.loadValue(outparam, output.valueReg());

  // Spectre mitigation in case of speculative execution within C++ code.
  if (JitOptions.spectreJitToCxxCalls) {
    masm.speculationBarrier();
  }

  // masm.leaveExitFrame & pop locals
  masm.adjustStack(IonOOLProxyExitFrameLayout::Size());
  return true;
||||||| merged common ancestors
    masm.loadJSContext(argJSContext);

    if (!masm.icBuildOOLFakeExitFrame(GetReturnAddressToIonCode(cx_), save)) {
        return false;
    }
    masm.enterFakeExitFrame(argJSContext, scratch, ExitFrameType::IonOOLProxy);

    // Make the call.
    masm.setupUnalignedABICall(scratch);
    masm.passABIArg(argJSContext);
    masm.passABIArg(argProxy);
    masm.passABIArg(argId);
    masm.passABIArg(argVp);
    masm.callWithABI(JS_FUNC_TO_DATA_PTR(void*, ProxyGetProperty), MoveOp::GENERAL,
                     CheckUnsafeCallWithABI::DontCheckHasExitFrame);

    // Test for failure.
    masm.branchIfFalseBool(ReturnReg, masm.exceptionLabel());

    // Load the outparam vp[0] into output register(s).
    Address outparam(masm.getStackPointer(), IonOOLProxyExitFrameLayout::offsetOfResult());
    masm.loadValue(outparam, output.valueReg());

    // Spectre mitigation in case of speculative execution within C++ code.
    if (JitOptions.spectreJitToCxxCalls) {
        masm.speculationBarrier();
    }

    // masm.leaveExitFrame & pop locals
    masm.adjustStack(IonOOLProxyExitFrameLayout::Size());
    return true;
=======
  return emitCallNativeGetterResultShared(
      TypedOrValueRegister(MIRType::Object, AnyRegister(obj)), output, save);
>>>>>>> upstream-releases
}

<<<<<<< HEAD
bool IonCacheIRCompiler::emitCallProxyGetByValueResult() {
  AutoSaveLiveRegisters save(*this);
  AutoOutputRegister output(*this);

  Register obj = allocator.useRegister(masm, reader.objOperandId());
  ValueOperand idVal = allocator.useValueRegister(masm, reader.valOperandId());
||||||| merged common ancestors
bool
IonCacheIRCompiler::emitCallProxyGetByValueResult()
{
    AutoSaveLiveRegisters save(*this);
    AutoOutputRegister output(*this);

    Register obj = allocator.useRegister(masm, reader.objOperandId());
    ValueOperand idVal = allocator.useValueRegister(masm, reader.valOperandId());
=======
bool IonCacheIRCompiler::emitCallNativeGetterByValueResult() {
  JitSpew(JitSpew_Codegen, __FUNCTION__);
  AutoSaveLiveRegisters save(*this);
  AutoOutputRegister output(*this);
>>>>>>> upstream-releases

<<<<<<< HEAD
  allocator.discardStack(masm);
||||||| merged common ancestors
    allocator.discardStack(masm);
=======
  ValueOperand val = allocator.useValueRegister(masm, reader.valOperandId());
>>>>>>> upstream-releases

<<<<<<< HEAD
  prepareVMCall(masm, save);

  masm.Push(idVal);
  masm.Push(obj);

  if (!callVM(masm, ProxyGetPropertyByValueInfo)) {
    return false;
  }

  masm.storeCallResultValue(output);
  return true;
||||||| merged common ancestors
    prepareVMCall(masm, save);

    masm.Push(idVal);
    masm.Push(obj);

    if (!callVM(masm, ProxyGetPropertyByValueInfo)) {
        return false;
    }

    masm.storeCallResultValue(output);
    return true;
=======
  return emitCallNativeGetterResultShared(val, output, save);
>>>>>>> upstream-releases
}

<<<<<<< HEAD
bool IonCacheIRCompiler::emitCallProxyHasPropResult() {
  AutoSaveLiveRegisters save(*this);
  AutoOutputRegister output(*this);
||||||| merged common ancestors
bool
IonCacheIRCompiler::emitCallProxyHasPropResult()
{
    AutoSaveLiveRegisters save(*this);
    AutoOutputRegister output(*this);
=======
bool IonCacheIRCompiler::emitCallProxyGetResult() {
  JitSpew(JitSpew_Codegen, __FUNCTION__);
  AutoSaveLiveRegisters save(*this);
  AutoOutputRegister output(*this);
>>>>>>> upstream-releases

<<<<<<< HEAD
  Register obj = allocator.useRegister(masm, reader.objOperandId());
  ValueOperand idVal = allocator.useValueRegister(masm, reader.valOperandId());
  bool hasOwn = reader.readBool();
||||||| merged common ancestors
    Register obj = allocator.useRegister(masm, reader.objOperandId());
    ValueOperand idVal = allocator.useValueRegister(masm, reader.valOperandId());
    bool hasOwn = reader.readBool();
=======
  Register obj = allocator.useRegister(masm, reader.objOperandId());
  jsid id = idStubField(reader.stubOffset());
>>>>>>> upstream-releases

<<<<<<< HEAD
  allocator.discardStack(masm);
||||||| merged common ancestors
    allocator.discardStack(masm);
=======
  // ProxyGetProperty(JSContext* cx, HandleObject proxy, HandleId id,
  //                  MutableHandleValue vp)
  AutoScratchRegisterMaybeOutput argJSContext(allocator, masm, output);
  AutoScratchRegister argProxy(allocator, masm);
  AutoScratchRegister argId(allocator, masm);
  AutoScratchRegister argVp(allocator, masm);
  AutoScratchRegister scratch(allocator, masm);
>>>>>>> upstream-releases

<<<<<<< HEAD
  prepareVMCall(masm, save);
||||||| merged common ancestors
    prepareVMCall(masm, save);
=======
  allocator.discardStack(masm);
>>>>>>> upstream-releases

<<<<<<< HEAD
  masm.Push(idVal);
  masm.Push(obj);
||||||| merged common ancestors
    masm.Push(idVal);
    masm.Push(obj);
=======
  // Push stubCode for marking.
  pushStubCodePointer();
>>>>>>> upstream-releases

<<<<<<< HEAD
  if (hasOwn) {
    if (!callVM(masm, ProxyHasOwnInfo)) {
      return false;
    }
  } else {
    if (!callVM(masm, ProxyHasInfo)) {
      return false;
    }
  }
||||||| merged common ancestors
    if (hasOwn) {
        if (!callVM(masm, ProxyHasOwnInfo)) {
            return false;
        }
    } else {
        if (!callVM(masm, ProxyHasInfo)) {
            return false;
        }
    }
=======
  // Push args on stack first so we can take pointers to make handles.
  masm.Push(UndefinedValue());
  masm.moveStackPtrTo(argVp.get());
>>>>>>> upstream-releases

<<<<<<< HEAD
  masm.storeCallResultValue(output);
  return true;
}

bool IonCacheIRCompiler::emitCallNativeGetElementResult() {
  AutoSaveLiveRegisters save(*this);
  AutoOutputRegister output(*this);

  Register obj = allocator.useRegister(masm, reader.objOperandId());
  Register index = allocator.useRegister(masm, reader.int32OperandId());
||||||| merged common ancestors
    masm.storeCallResultValue(output);
    return true;
}


bool
IonCacheIRCompiler::emitCallNativeGetElementResult()
{
    AutoSaveLiveRegisters save(*this);
    AutoOutputRegister output(*this);
=======
  masm.Push(id, scratch);
  masm.moveStackPtrTo(argId.get());
>>>>>>> upstream-releases

<<<<<<< HEAD
  allocator.discardStack(masm);
||||||| merged common ancestors
    Register obj = allocator.useRegister(masm, reader.objOperandId());
    Register index = allocator.useRegister(masm, reader.int32OperandId());
=======
  // Push the proxy. Also used as receiver.
  masm.Push(obj);
  masm.moveStackPtrTo(argProxy.get());
>>>>>>> upstream-releases

<<<<<<< HEAD
  prepareVMCall(masm, save);
||||||| merged common ancestors
    allocator.discardStack(masm);
=======
  masm.loadJSContext(argJSContext);
>>>>>>> upstream-releases

<<<<<<< HEAD
  masm.Push(index);
  masm.Push(TypedOrValueRegister(MIRType::Object, AnyRegister(obj)));
  masm.Push(obj);

  if (!callVM(masm, NativeGetElementInfo)) {
    return false;
  }
||||||| merged common ancestors
    prepareVMCall(masm, save);

    masm.Push(index);
    masm.Push(TypedOrValueRegister(MIRType::Object, AnyRegister(obj)));
    masm.Push(obj);

    if (!callVM(masm, NativeGetElementInfo)) {
        return false;
    }
=======
  if (!masm.icBuildOOLFakeExitFrame(GetReturnAddressToIonCode(cx_), save)) {
    return false;
  }
  masm.enterFakeExitFrame(argJSContext, scratch, ExitFrameType::IonOOLProxy);

  // Make the call.
  masm.setupUnalignedABICall(scratch);
  masm.passABIArg(argJSContext);
  masm.passABIArg(argProxy);
  masm.passABIArg(argId);
  masm.passABIArg(argVp);
  masm.callWithABI(JS_FUNC_TO_DATA_PTR(void*, ProxyGetProperty),
                   MoveOp::GENERAL,
                   CheckUnsafeCallWithABI::DontCheckHasExitFrame);
>>>>>>> upstream-releases

<<<<<<< HEAD
  masm.storeCallResultValue(output);
  return true;
}
||||||| merged common ancestors
    masm.storeCallResultValue(output);
    return true;
}
=======
  // Test for failure.
  masm.branchIfFalseBool(ReturnReg, masm.exceptionLabel());
>>>>>>> upstream-releases

<<<<<<< HEAD
bool IonCacheIRCompiler::emitLoadUnboxedPropertyResult() {
  AutoOutputRegister output(*this);
  Register obj = allocator.useRegister(masm, reader.objOperandId());
||||||| merged common ancestors
=======
  // Load the outparam vp[0] into output register(s).
  Address outparam(masm.getStackPointer(),
                   IonOOLProxyExitFrameLayout::offsetOfResult());
  masm.loadValue(outparam, output.valueReg());
>>>>>>> upstream-releases

<<<<<<< HEAD
  JSValueType fieldType = reader.valueType();
  int32_t fieldOffset = int32StubField(reader.stubOffset());
  masm.loadUnboxedProperty(Address(obj, fieldOffset), fieldType, output);
  return true;
||||||| merged common ancestors
bool
IonCacheIRCompiler::emitLoadUnboxedPropertyResult()
{
    AutoOutputRegister output(*this);
    Register obj = allocator.useRegister(masm, reader.objOperandId());

    JSValueType fieldType = reader.valueType();
    int32_t fieldOffset = int32StubField(reader.stubOffset());
    masm.loadUnboxedProperty(Address(obj, fieldOffset), fieldType, output);
    return true;
=======
  // Spectre mitigation in case of speculative execution within C++ code.
  if (JitOptions.spectreJitToCxxCalls) {
    masm.speculationBarrier();
  }

  // masm.leaveExitFrame & pop locals
  masm.adjustStack(IonOOLProxyExitFrameLayout::Size());
  return true;
>>>>>>> upstream-releases
}

bool IonCacheIRCompiler::emitGuardFrameHasNoArgumentsObject() {
  MOZ_CRASH("Baseline-specific op");
}

bool IonCacheIRCompiler::emitLoadFrameCalleeResult() {
  MOZ_CRASH("Baseline-specific op");
}

bool IonCacheIRCompiler::emitLoadFrameNumActualArgsResult() {
  MOZ_CRASH("Baseline-specific op");
}

bool IonCacheIRCompiler::emitLoadFrameArgumentResult() {
  MOZ_CRASH("Baseline-specific op");
}

<<<<<<< HEAD
bool IonCacheIRCompiler::emitLoadEnvironmentFixedSlotResult() {
  AutoOutputRegister output(*this);
  Register obj = allocator.useRegister(masm, reader.objOperandId());
  int32_t offset = int32StubField(reader.stubOffset());
||||||| merged common ancestors
bool
IonCacheIRCompiler::emitLoadEnvironmentFixedSlotResult()
{
    AutoOutputRegister output(*this);
    Register obj = allocator.useRegister(masm, reader.objOperandId());
    int32_t offset = int32StubField(reader.stubOffset());
=======
bool IonCacheIRCompiler::emitLoadEnvironmentFixedSlotResult() {
  JitSpew(JitSpew_Codegen, __FUNCTION__);
  AutoOutputRegister output(*this);
  Register obj = allocator.useRegister(masm, reader.objOperandId());
  int32_t offset = int32StubField(reader.stubOffset());
>>>>>>> upstream-releases

  FailurePath* failure;
  if (!addFailurePath(&failure)) {
    return false;
  }

  // Check for uninitialized lexicals.
  Address slot(obj, offset);
  masm.branchTestMagic(Assembler::Equal, slot, failure->label());

  // Load the value.
  masm.loadTypedOrValue(slot, output);
  return true;
}

<<<<<<< HEAD
bool IonCacheIRCompiler::emitLoadEnvironmentDynamicSlotResult() {
  AutoOutputRegister output(*this);
  Register obj = allocator.useRegister(masm, reader.objOperandId());
  int32_t offset = int32StubField(reader.stubOffset());
  AutoScratchRegisterMaybeOutput scratch(allocator, masm, output);
||||||| merged common ancestors
bool
IonCacheIRCompiler::emitLoadEnvironmentDynamicSlotResult()
{
    AutoOutputRegister output(*this);
    Register obj = allocator.useRegister(masm, reader.objOperandId());
    int32_t offset = int32StubField(reader.stubOffset());
    AutoScratchRegisterMaybeOutput scratch(allocator, masm, output);
=======
bool IonCacheIRCompiler::emitLoadEnvironmentDynamicSlotResult() {
  JitSpew(JitSpew_Codegen, __FUNCTION__);
  AutoOutputRegister output(*this);
  Register obj = allocator.useRegister(masm, reader.objOperandId());
  int32_t offset = int32StubField(reader.stubOffset());
  AutoScratchRegisterMaybeOutput scratch(allocator, masm, output);
>>>>>>> upstream-releases

  FailurePath* failure;
  if (!addFailurePath(&failure)) {
    return false;
  }

  masm.loadPtr(Address(obj, NativeObject::offsetOfSlots()), scratch);

  // Check for uninitialized lexicals.
  Address slot(scratch, offset);
  masm.branchTestMagic(Assembler::Equal, slot, failure->label());

  // Load the value.
  masm.loadTypedOrValue(slot, output);
  return true;
}

<<<<<<< HEAD
bool IonCacheIRCompiler::emitLoadStringResult() {
  MOZ_CRASH("not used in ion");
||||||| merged common ancestors

bool
IonCacheIRCompiler::emitLoadStringResult()
{
    MOZ_CRASH("not used in ion");
=======
bool IonCacheIRCompiler::emitLoadStringResult() {
  JitSpew(JitSpew_Codegen, __FUNCTION__);
  MOZ_CRASH("not used in ion");
>>>>>>> upstream-releases
}

<<<<<<< HEAD
bool IonCacheIRCompiler::emitCallStringSplitResult() {
  AutoSaveLiveRegisters save(*this);
  AutoOutputRegister output(*this);

  Register str = allocator.useRegister(masm, reader.stringOperandId());
  Register sep = allocator.useRegister(masm, reader.stringOperandId());
  ObjectGroup* group = groupStubField(reader.stubOffset());

  allocator.discardStack(masm);

  prepareVMCall(masm, save);

  masm.Push(str);
  masm.Push(sep);
  masm.Push(ImmGCPtr(group));
  masm.Push(Imm32(INT32_MAX));

  if (!callVM(masm, StringSplitHelperInfo)) {
    return false;
  }

  masm.storeCallResultValue(output);
  return true;
}
||||||| merged common ancestors
bool
IonCacheIRCompiler::emitCallStringSplitResult()
{
    AutoSaveLiveRegisters save(*this);
    AutoOutputRegister output(*this);

    Register str = allocator.useRegister(masm, reader.stringOperandId());
    Register sep = allocator.useRegister(masm, reader.stringOperandId());
    ObjectGroup* group = groupStubField(reader.stubOffset());

    allocator.discardStack(masm);

    prepareVMCall(masm, save);

    masm.Push(str);
    masm.Push(sep);
    masm.Push(ImmGCPtr(group));
    masm.Push(Imm32(INT32_MAX));

    if (!callVM(masm, StringSplitHelperInfo)) {
        return false;
    }

    masm.storeCallResultValue(output);
    return true;
}
=======
bool IonCacheIRCompiler::emitCompareStringResult() {
  JitSpew(JitSpew_Codegen, __FUNCTION__);
  AutoSaveLiveRegisters save(*this);
  AutoOutputRegister output(*this);
>>>>>>> upstream-releases

<<<<<<< HEAD
bool IonCacheIRCompiler::emitCompareStringResult() {
  AutoSaveLiveRegisters save(*this);
  AutoOutputRegister output(*this);
||||||| merged common ancestors
bool
IonCacheIRCompiler::emitCompareStringResult()
{
    AutoSaveLiveRegisters save(*this);
    AutoOutputRegister output(*this);
=======
  Register left = allocator.useRegister(masm, reader.stringOperandId());
  Register right = allocator.useRegister(masm, reader.stringOperandId());
  JSOp op = reader.jsop();
>>>>>>> upstream-releases

<<<<<<< HEAD
  Register left = allocator.useRegister(masm, reader.stringOperandId());
  Register right = allocator.useRegister(masm, reader.stringOperandId());
  JSOp op = reader.jsop();
||||||| merged common ancestors
    Register left = allocator.useRegister(masm, reader.stringOperandId());
    Register right = allocator.useRegister(masm, reader.stringOperandId());
    JSOp op = reader.jsop();
=======
  allocator.discardStack(masm);
>>>>>>> upstream-releases

<<<<<<< HEAD
  allocator.discardStack(masm);
||||||| merged common ancestors
    allocator.discardStack(masm);
=======
  Label slow, done;
  MOZ_ASSERT(!output.hasValue());
  masm.compareStrings(op, left, right, output.typedReg().gpr(), &slow);
>>>>>>> upstream-releases

<<<<<<< HEAD
  Label slow, done;
  MOZ_ASSERT(!output.hasValue());
  masm.compareStrings(op, left, right, output.typedReg().gpr(), &slow);
||||||| merged common ancestors
    Label slow, done;
    MOZ_ASSERT(!output.hasValue());
    masm.compareStrings(op, left, right, output.typedReg().gpr(), &slow);
=======
  masm.jump(&done);
  masm.bind(&slow);
>>>>>>> upstream-releases

<<<<<<< HEAD
  masm.jump(&done);
  masm.bind(&slow);
||||||| merged common ancestors
    masm.jump(&done);
    masm.bind(&slow);
=======
  prepareVMCall(masm, save);
>>>>>>> upstream-releases

<<<<<<< HEAD
  prepareVMCall(masm, save);
  masm.Push(right);
  masm.Push(left);

  if (!callVM(masm, (op == JSOP_EQ || op == JSOP_STRICTEQ)
                        ? StringsEqualInfo
                        : StringsNotEqualInfo)) {
    return false;
  }

  masm.storeCallBoolResult(output.typedReg().gpr());
  masm.bind(&done);
  return true;
}

static bool GroupHasPropertyTypes(ObjectGroup* group, jsid* id, Value* v) {
  AutoUnsafeCallWithABI unsafe;
  if (group->unknownPropertiesDontCheckGeneration()) {
    return true;
  }
  HeapTypeSet* propTypes = group->maybeGetPropertyDontCheckGeneration(*id);
  if (!propTypes) {
    return true;
  }
  if (!propTypes->nonConstantProperty()) {
    return false;
  }
  return propTypes->hasType(TypeSet::GetValueType(*v));
}

static void EmitCheckPropertyTypes(MacroAssembler& masm,
                                   const PropertyTypeCheckInfo* typeCheckInfo,
                                   Register obj, const ConstantOrRegister& val,
                                   const LiveRegisterSet& liveRegs,
                                   Label* failures) {
  // Emit code to check |val| is part of the property's HeapTypeSet.

  if (!typeCheckInfo->isSet()) {
    return;
  }

  ObjectGroup* group = typeCheckInfo->group();
  AutoSweepObjectGroup sweep(group);
  if (group->unknownProperties(sweep)) {
    return;
  }

  jsid id = typeCheckInfo->id();
  HeapTypeSet* propTypes = group->maybeGetProperty(sweep, id);
  if (propTypes && propTypes->unknown()) {
    return;
  }

  // Use the object register as scratch, as we don't need it here.
  masm.Push(obj);
  Register scratch1 = obj;

  // We may also need a scratch register for guardTypeSet. Additionally,
  // spectreRegToZero is the register that may be zeroed on speculatively
  // executed paths.
  Register objScratch = InvalidReg;
  Register spectreRegToZero = InvalidReg;
  if (propTypes && !propTypes->unknownObject() &&
      propTypes->getObjectCount() > 0) {
    AllocatableGeneralRegisterSet regs(GeneralRegisterSet::All());
    if (!val.constant()) {
      TypedOrValueRegister valReg = val.reg();
      if (valReg.hasValue()) {
        regs.take(valReg.valueReg());
        spectreRegToZero = valReg.valueReg().payloadOrValueReg();
      } else if (!valReg.typedReg().isFloat()) {
        regs.take(valReg.typedReg().gpr());
        spectreRegToZero = valReg.typedReg().gpr();
      }
||||||| merged common ancestors
    prepareVMCall(masm, save);
    masm.Push(right);
    masm.Push(left);

    if (!callVM(masm, (op == JSOP_EQ || op == JSOP_STRICTEQ) ?
                            StringsEqualInfo :
                            StringsNotEqualInfo))
    {
        return false;
    }

    masm.storeCallBoolResult(output.typedReg().gpr());
    masm.bind(&done);
    return true;
}

static bool
GroupHasPropertyTypes(ObjectGroup* group, jsid* id, Value* v)
{
    AutoUnsafeCallWithABI unsafe;
    if (group->unknownPropertiesDontCheckGeneration()) {
        return true;
    }
    HeapTypeSet* propTypes = group->maybeGetPropertyDontCheckGeneration(*id);
    if (!propTypes) {
        return true;
    }
    if (!propTypes->nonConstantProperty()) {
        return false;
=======
  // Push the operands in reverse order for JSOP_LE and JSOP_GT:
  // - |left <= right| is implemented as |right >= left|.
  // - |left > right| is implemented as |right < left|.
  if (op == JSOP_LE || op == JSOP_GT) {
    masm.Push(left);
    masm.Push(right);
  } else {
    masm.Push(right);
    masm.Push(left);
  }

  using Fn = bool (*)(JSContext*, HandleString, HandleString, bool*);
  if (op == JSOP_EQ || op == JSOP_STRICTEQ) {
    callVM<Fn, jit::StringsEqual<EqualityKind::Equal>>(masm);
  } else if (op == JSOP_NE || op == JSOP_STRICTNE) {
    callVM<Fn, jit::StringsEqual<EqualityKind::NotEqual>>(masm);
  } else if (op == JSOP_LT || op == JSOP_GT) {
    callVM<Fn, jit::StringsCompare<ComparisonKind::LessThan>>(masm);
  } else {
    MOZ_ASSERT(op == JSOP_LE || op == JSOP_GE);
    callVM<Fn, jit::StringsCompare<ComparisonKind::GreaterThanOrEqual>>(masm);
  }

  masm.storeCallBoolResult(output.typedReg().gpr());
  masm.bind(&done);
  return true;
}

static bool GroupHasPropertyTypes(ObjectGroup* group, jsid* id, Value* v) {
  AutoUnsafeCallWithABI unsafe;
  if (group->unknownPropertiesDontCheckGeneration()) {
    return true;
  }
  HeapTypeSet* propTypes = group->maybeGetPropertyDontCheckGeneration(*id);
  if (!propTypes) {
    return true;
  }
  if (!propTypes->nonConstantProperty()) {
    return false;
  }
  return propTypes->hasType(TypeSet::GetValueType(*v));
}

static void EmitCheckPropertyTypes(MacroAssembler& masm,
                                   const PropertyTypeCheckInfo* typeCheckInfo,
                                   Register obj, const ConstantOrRegister& val,
                                   const LiveRegisterSet& liveRegs,
                                   Label* failures) {
  // Emit code to check |val| is part of the property's HeapTypeSet.

  if (!typeCheckInfo->isSet()) {
    return;
  }

  ObjectGroup* group = typeCheckInfo->group();
  AutoSweepObjectGroup sweep(group);
  if (group->unknownProperties(sweep)) {
    return;
  }

  jsid id = typeCheckInfo->id();
  HeapTypeSet* propTypes = group->maybeGetProperty(sweep, id);
  if (propTypes && propTypes->unknown()) {
    return;
  }

  // Use the object register as scratch, as we don't need it here.
  masm.Push(obj);
  Register scratch1 = obj;

  // We may also need a scratch register for guardTypeSet. Additionally,
  // spectreRegToZero is the register that may be zeroed on speculatively
  // executed paths.
  Register objScratch = InvalidReg;
  Register spectreRegToZero = InvalidReg;
  if (propTypes && !propTypes->unknownObject() &&
      propTypes->getObjectCount() > 0) {
    AllocatableGeneralRegisterSet regs(GeneralRegisterSet::All());
    if (!val.constant()) {
      TypedOrValueRegister valReg = val.reg();
      if (valReg.hasValue()) {
        regs.take(valReg.valueReg());
        spectreRegToZero = valReg.valueReg().payloadOrValueReg();
      } else if (!valReg.typedReg().isFloat()) {
        regs.take(valReg.typedReg().gpr());
        spectreRegToZero = valReg.typedReg().gpr();
      }
>>>>>>> upstream-releases
    }
    regs.take(scratch1);
    objScratch = regs.takeAny();
    masm.Push(objScratch);
  }

  bool checkTypeSet = true;
  Label failedFastPath;

  if (propTypes && !propTypes->nonConstantProperty()) {
    masm.jump(&failedFastPath);
  }

  if (val.constant()) {
    // If the input is a constant, then don't bother if the barrier will always
    // fail.
    if (!propTypes || !propTypes->hasType(TypeSet::GetValueType(val.value()))) {
      masm.jump(&failedFastPath);
    }
    checkTypeSet = false;
  } else {
    // We can do the same trick as above for primitive types of specialized
    // registers.
    TypedOrValueRegister reg = val.reg();
    if (reg.hasTyped() && reg.type() != MIRType::Object) {
      JSValueType valType = ValueTypeFromMIRType(reg.type());
      if (!propTypes || !propTypes->hasType(TypeSet::PrimitiveType(valType))) {
        masm.jump(&failedFastPath);
      }
      checkTypeSet = false;
    }
  }

  Label done;
  if (checkTypeSet) {
    TypedOrValueRegister valReg = val.reg();
    if (propTypes) {
      // guardTypeSet can read from type sets without triggering read barriers.
      TypeSet::readBarrier(propTypes);
      masm.guardTypeSet(valReg, propTypes, BarrierKind::TypeSet, scratch1,
                        objScratch, spectreRegToZero, &failedFastPath);
      masm.jump(&done);
    } else {
      masm.jump(&failedFastPath);
    }
  }

  if (failedFastPath.used()) {
    // The inline type check failed. Do a callWithABI to check the current
    // TypeSet in case the type was added after we generated this stub.
    masm.bind(&failedFastPath);

    AllocatableRegisterSet regs(GeneralRegisterSet::Volatile(),
                                liveRegs.fpus());
    LiveRegisterSet save(regs.asLiveSet());
    masm.PushRegsInMask(save);

    regs.takeUnchecked(scratch1);

    // Push |val| first to make sure everything is fine if |val| aliases
    // scratch2.
    Register scratch2 = regs.takeAnyGeneral();
    masm.Push(val);
    masm.moveStackPtrTo(scratch2);

    Register scratch3 = regs.takeAnyGeneral();
    masm.Push(id, scratch3);
    masm.moveStackPtrTo(scratch3);

    masm.setupUnalignedABICall(scratch1);
    masm.movePtr(ImmGCPtr(group), scratch1);
    masm.passABIArg(scratch1);
    masm.passABIArg(scratch3);
    masm.passABIArg(scratch2);
    masm.callWithABI(JS_FUNC_TO_DATA_PTR(void*, GroupHasPropertyTypes));
    masm.mov(ReturnReg, scratch1);

    masm.adjustStack(sizeof(Value) + sizeof(jsid));

    LiveRegisterSet ignore;
    ignore.add(scratch1);
    masm.PopRegsInMaskIgnore(save, ignore);

    masm.branchIfTrueBool(scratch1, &done);
    if (objScratch != InvalidReg) {
<<<<<<< HEAD
      masm.pop(objScratch);
    }
    masm.pop(obj);
    masm.jump(failures);
  }

  masm.bind(&done);
  if (objScratch != InvalidReg) {
    masm.Pop(objScratch);
  }
  masm.Pop(obj);
||||||| merged common ancestors
        masm.Pop(objScratch);
    }
    masm.Pop(obj);
}

bool
IonCacheIRCompiler::emitStoreFixedSlot()
{
    Register obj = allocator.useRegister(masm, reader.objOperandId());
    int32_t offset = int32StubField(reader.stubOffset());
    ConstantOrRegister val = allocator.useConstantOrRegister(masm, reader.valOperandId());

    Maybe<AutoScratchRegister> scratch;
    if (needsPostBarrier()) {
        scratch.emplace(allocator, masm);
    }

    if (typeCheckInfo_->isSet()) {
        FailurePath* failure;
        if (!addFailurePath(&failure)) {
            return false;
        }

        EmitCheckPropertyTypes(masm, typeCheckInfo_, obj, val, *liveRegs_, failure->label());
    }

    Address slot(obj, offset);
    EmitPreBarrier(masm, slot, MIRType::Value);
    masm.storeConstantOrRegister(val, slot);
    if (needsPostBarrier()) {
        emitPostBarrierSlot(obj, val, scratch.ref());
    }
    return true;
=======
      masm.pop(objScratch);
    }
    masm.pop(obj);
    masm.jump(failures);
  }

  masm.bind(&done);
  if (objScratch != InvalidReg) {
    masm.Pop(objScratch);
  }
  masm.Pop(obj);
>>>>>>> upstream-releases
}

<<<<<<< HEAD
bool IonCacheIRCompiler::emitStoreFixedSlot() {
  Register obj = allocator.useRegister(masm, reader.objOperandId());
  int32_t offset = int32StubField(reader.stubOffset());
  ConstantOrRegister val =
      allocator.useConstantOrRegister(masm, reader.valOperandId());

  Maybe<AutoScratchRegister> scratch;
  if (needsPostBarrier()) {
    scratch.emplace(allocator, masm);
  }
||||||| merged common ancestors
bool
IonCacheIRCompiler::emitStoreDynamicSlot()
{
    Register obj = allocator.useRegister(masm, reader.objOperandId());
    int32_t offset = int32StubField(reader.stubOffset());
    ConstantOrRegister val = allocator.useConstantOrRegister(masm, reader.valOperandId());
    AutoScratchRegister scratch(allocator, masm);

    if (typeCheckInfo_->isSet()) {
        FailurePath* failure;
        if (!addFailurePath(&failure)) {
            return false;
        }

        EmitCheckPropertyTypes(masm, typeCheckInfo_, obj, val, *liveRegs_, failure->label());
    }
=======
bool IonCacheIRCompiler::emitStoreFixedSlot() {
  JitSpew(JitSpew_Codegen, __FUNCTION__);
  Register obj = allocator.useRegister(masm, reader.objOperandId());
  int32_t offset = int32StubField(reader.stubOffset());
  ConstantOrRegister val =
      allocator.useConstantOrRegister(masm, reader.valOperandId());

  Maybe<AutoScratchRegister> scratch;
  if (needsPostBarrier()) {
    scratch.emplace(allocator, masm);
  }
>>>>>>> upstream-releases

  if (typeCheckInfo_->isSet()) {
    FailurePath* failure;
    if (!addFailurePath(&failure)) {
      return false;
    }

    EmitCheckPropertyTypes(masm, typeCheckInfo_, obj, val, *liveRegs_,
                           failure->label());
  }

  Address slot(obj, offset);
  EmitPreBarrier(masm, slot, MIRType::Value);
  masm.storeConstantOrRegister(val, slot);
  if (needsPostBarrier()) {
    emitPostBarrierSlot(obj, val, scratch.ref());
  }
  return true;
}

<<<<<<< HEAD
bool IonCacheIRCompiler::emitStoreDynamicSlot() {
  Register obj = allocator.useRegister(masm, reader.objOperandId());
  int32_t offset = int32StubField(reader.stubOffset());
  ConstantOrRegister val =
      allocator.useConstantOrRegister(masm, reader.valOperandId());
  AutoScratchRegister scratch(allocator, masm);
||||||| merged common ancestors
    bool changeGroup = reader.readBool();
    ObjectGroup* newGroup = groupStubField(reader.stubOffset());
    Shape* newShape = shapeStubField(reader.stubOffset());
=======
bool IonCacheIRCompiler::emitStoreDynamicSlot() {
  JitSpew(JitSpew_Codegen, __FUNCTION__);
  Register obj = allocator.useRegister(masm, reader.objOperandId());
  int32_t offset = int32StubField(reader.stubOffset());
  ConstantOrRegister val =
      allocator.useConstantOrRegister(masm, reader.valOperandId());
  AutoScratchRegister scratch(allocator, masm);
>>>>>>> upstream-releases

  if (typeCheckInfo_->isSet()) {
    FailurePath* failure;
    if (!addFailurePath(&failure)) {
      return false;
    }

    EmitCheckPropertyTypes(masm, typeCheckInfo_, obj, val, *liveRegs_,
                           failure->label());
  }

  masm.loadPtr(Address(obj, NativeObject::offsetOfSlots()), scratch);
  Address slot(scratch, offset);
  EmitPreBarrier(masm, slot, MIRType::Value);
  masm.storeConstantOrRegister(val, slot);
  if (needsPostBarrier()) {
    emitPostBarrierSlot(obj, val, scratch);
  }
  return true;
}

<<<<<<< HEAD
bool IonCacheIRCompiler::emitAddAndStoreSlotShared(CacheOp op) {
  Register obj = allocator.useRegister(masm, reader.objOperandId());
  int32_t offset = int32StubField(reader.stubOffset());
  ConstantOrRegister val =
      allocator.useConstantOrRegister(masm, reader.valOperandId());
||||||| merged common ancestors
        LiveRegisterSet save(GeneralRegisterSet::Volatile(), liveVolatileFloatRegs());
        masm.PushRegsInMask(save);
=======
bool IonCacheIRCompiler::emitAddAndStoreSlotShared(CacheOp op) {
  JitSpew(JitSpew_Codegen, __FUNCTION__);
  Register obj = allocator.useRegister(masm, reader.objOperandId());
  int32_t offset = int32StubField(reader.stubOffset());
  ConstantOrRegister val =
      allocator.useConstantOrRegister(masm, reader.valOperandId());
>>>>>>> upstream-releases

  AutoScratchRegister scratch1(allocator, masm);

  Maybe<AutoScratchRegister> scratch2;
  if (op == CacheOp::AllocateAndStoreDynamicSlot) {
    scratch2.emplace(allocator, masm);
  }

  bool changeGroup = reader.readBool();
  ObjectGroup* newGroup = groupStubField(reader.stubOffset());
  Shape* newShape = shapeStubField(reader.stubOffset());

  FailurePath* failure;
  if (!addFailurePath(&failure)) {
    return false;
  }

  EmitCheckPropertyTypes(masm, typeCheckInfo_, obj, val, *liveRegs_,
                         failure->label());

  if (op == CacheOp::AllocateAndStoreDynamicSlot) {
    // We have to (re)allocate dynamic slots. Do this first, as it's the
    // only fallible operation here. Note that growSlotsPure is
    // fallible but does not GC.
    int32_t numNewSlots = int32StubField(reader.stubOffset());
    MOZ_ASSERT(numNewSlots > 0);

    LiveRegisterSet save(GeneralRegisterSet::Volatile(),
                         liveVolatileFloatRegs());
    masm.PushRegsInMask(save);

    masm.setupUnalignedABICall(scratch1);
    masm.loadJSContext(scratch1);
    masm.passABIArg(scratch1);
    masm.passABIArg(obj);
    masm.move32(Imm32(numNewSlots), scratch2.ref());
    masm.passABIArg(scratch2.ref());
    masm.callWithABI(JS_FUNC_TO_DATA_PTR(void*, NativeObject::growSlotsPure));
    masm.mov(ReturnReg, scratch1);

    LiveRegisterSet ignore;
    ignore.add(scratch1);
    masm.PopRegsInMaskIgnore(save, ignore);

<<<<<<< HEAD
    masm.branchIfFalseBool(scratch1, failure->label());
  }

  if (changeGroup) {
    // Changing object's group from a partially to fully initialized group,
    // per the acquired properties analysis. Only change the group if the
    // old group still has a newScript. This only applies to PlainObjects.
    Label noGroupChange;
    masm.branchIfObjGroupHasNoAddendum(obj, scratch1, &noGroupChange);

    // Update the object's group.
    masm.storeObjGroup(newGroup, obj,
                       [](MacroAssembler& masm, const Address& addr) {
                         EmitPreBarrier(masm, addr, MIRType::ObjectGroup);
                       });

    masm.bind(&noGroupChange);
  }

  // Update the object's shape.
  masm.storeObjShape(newShape, obj,
                     [](MacroAssembler& masm, const Address& addr) {
                       EmitPreBarrier(masm, addr, MIRType::Shape);
                     });

  // Perform the store. No pre-barrier required since this is a new
  // initialization.
  if (op == CacheOp::AddAndStoreFixedSlot) {
    Address slot(obj, offset);
    masm.storeConstantOrRegister(val, slot);
  } else {
    MOZ_ASSERT(op == CacheOp::AddAndStoreDynamicSlot ||
               op == CacheOp::AllocateAndStoreDynamicSlot);
    masm.loadPtr(Address(obj, NativeObject::offsetOfSlots()), scratch1);
    Address slot(scratch1, offset);
    masm.storeConstantOrRegister(val, slot);
  }

  if (needsPostBarrier()) {
    emitPostBarrierSlot(obj, val, scratch1);
  }

  return true;
}
||||||| merged common ancestors
    return true;
}
=======
    masm.branchIfFalseBool(scratch1, failure->label());
  }

  if (changeGroup) {
    // Changing object's group from a partially to fully initialized group,
    // per the acquired properties analysis. Only change the group if the
    // old group still has a newScript. This only applies to PlainObjects.
    Label noGroupChange;
    masm.branchIfObjGroupHasNoAddendum(obj, scratch1, &noGroupChange);

    // Update the object's group.
    masm.storeObjGroup(newGroup, obj,
                       [](MacroAssembler& masm, const Address& addr) {
                         EmitPreBarrier(masm, addr, MIRType::ObjectGroup);
                       });

    masm.bind(&noGroupChange);
  }

  // Update the object's shape.
  masm.storeObjShape(newShape, obj,
                     [](MacroAssembler& masm, const Address& addr) {
                       EmitPreBarrier(masm, addr, MIRType::Shape);
                     });

  // Perform the store. No pre-barrier required since this is a new
  // initialization.
  if (op == CacheOp::AddAndStoreFixedSlot) {
    Address slot(obj, offset);
    masm.storeConstantOrRegister(val, slot);
  } else {
    MOZ_ASSERT(op == CacheOp::AddAndStoreDynamicSlot ||
               op == CacheOp::AllocateAndStoreDynamicSlot);
    masm.loadPtr(Address(obj, NativeObject::offsetOfSlots()), scratch1);
    Address slot(scratch1, offset);
    masm.storeConstantOrRegister(val, slot);
  }
>>>>>>> upstream-releases

<<<<<<< HEAD
bool IonCacheIRCompiler::emitAddAndStoreFixedSlot() {
  return emitAddAndStoreSlotShared(CacheOp::AddAndStoreFixedSlot);
}
||||||| merged common ancestors
bool
IonCacheIRCompiler::emitAddAndStoreFixedSlot()
{
    return emitAddAndStoreSlotShared(CacheOp::AddAndStoreFixedSlot);
}
=======
  if (needsPostBarrier()) {
    emitPostBarrierSlot(obj, val, scratch1);
  }
>>>>>>> upstream-releases

<<<<<<< HEAD
bool IonCacheIRCompiler::emitAddAndStoreDynamicSlot() {
  return emitAddAndStoreSlotShared(CacheOp::AddAndStoreDynamicSlot);
||||||| merged common ancestors
bool
IonCacheIRCompiler::emitAddAndStoreDynamicSlot()
{
    return emitAddAndStoreSlotShared(CacheOp::AddAndStoreDynamicSlot);
=======
  return true;
>>>>>>> upstream-releases
}

<<<<<<< HEAD
bool IonCacheIRCompiler::emitAllocateAndStoreDynamicSlot() {
  return emitAddAndStoreSlotShared(CacheOp::AllocateAndStoreDynamicSlot);
||||||| merged common ancestors
bool
IonCacheIRCompiler::emitAllocateAndStoreDynamicSlot()
{
    return emitAddAndStoreSlotShared(CacheOp::AllocateAndStoreDynamicSlot);
=======
bool IonCacheIRCompiler::emitAddAndStoreFixedSlot() {
  JitSpew(JitSpew_Codegen, __FUNCTION__);
  return emitAddAndStoreSlotShared(CacheOp::AddAndStoreFixedSlot);
>>>>>>> upstream-releases
}

<<<<<<< HEAD
bool IonCacheIRCompiler::emitStoreUnboxedProperty() {
  Register obj = allocator.useRegister(masm, reader.objOperandId());
  JSValueType fieldType = reader.valueType();
  int32_t offset = int32StubField(reader.stubOffset());
  ConstantOrRegister val =
      allocator.useConstantOrRegister(masm, reader.valOperandId());

  Maybe<AutoScratchRegister> scratch;
  if (needsPostBarrier() && UnboxedTypeNeedsPostBarrier(fieldType)) {
    scratch.emplace(allocator, masm);
  }

  if (fieldType == JSVAL_TYPE_OBJECT && typeCheckInfo_->isSet()) {
    FailurePath* failure;
    if (!addFailurePath(&failure)) {
      return false;
    }
    EmitCheckPropertyTypes(masm, typeCheckInfo_, obj, val, *liveRegs_,
                           failure->label());
  }

  // Note that the storeUnboxedProperty call here is infallible, as the
  // IR emitter is responsible for guarding on |val|'s type.
  Address fieldAddr(obj, offset);
  EmitICUnboxedPreBarrier(masm, fieldAddr, fieldType);
  masm.storeUnboxedProperty(fieldAddr, fieldType, val, /* failure = */ nullptr);
  if (needsPostBarrier() && UnboxedTypeNeedsPostBarrier(fieldType)) {
    emitPostBarrierSlot(obj, val, scratch.ref());
  }
  return true;
||||||| merged common ancestors
bool
IonCacheIRCompiler::emitStoreUnboxedProperty()
{
    Register obj = allocator.useRegister(masm, reader.objOperandId());
    JSValueType fieldType = reader.valueType();
    int32_t offset = int32StubField(reader.stubOffset());
    ConstantOrRegister val = allocator.useConstantOrRegister(masm, reader.valOperandId());

    Maybe<AutoScratchRegister> scratch;
    if (needsPostBarrier() && UnboxedTypeNeedsPostBarrier(fieldType)) {
        scratch.emplace(allocator, masm);
    }

    if (fieldType == JSVAL_TYPE_OBJECT && typeCheckInfo_->isSet()) {
        FailurePath* failure;
        if (!addFailurePath(&failure)) {
            return false;
        }
        EmitCheckPropertyTypes(masm, typeCheckInfo_, obj, val, *liveRegs_, failure->label());
    }

    // Note that the storeUnboxedProperty call here is infallible, as the
    // IR emitter is responsible for guarding on |val|'s type.
    Address fieldAddr(obj, offset);
    EmitICUnboxedPreBarrier(masm, fieldAddr, fieldType);
    masm.storeUnboxedProperty(fieldAddr, fieldType, val, /* failure = */ nullptr);
    if (needsPostBarrier() && UnboxedTypeNeedsPostBarrier(fieldType)) {
        emitPostBarrierSlot(obj, val, scratch.ref());
    }
    return true;
=======
bool IonCacheIRCompiler::emitAddAndStoreDynamicSlot() {
  JitSpew(JitSpew_Codegen, __FUNCTION__);
  return emitAddAndStoreSlotShared(CacheOp::AddAndStoreDynamicSlot);
>>>>>>> upstream-releases
}

<<<<<<< HEAD
bool IonCacheIRCompiler::emitStoreTypedObjectReferenceProperty() {
  Register obj = allocator.useRegister(masm, reader.objOperandId());
  int32_t offset = int32StubField(reader.stubOffset());
  TypedThingLayout layout = reader.typedThingLayout();
  ReferenceType type = reader.referenceTypeDescrType();
||||||| merged common ancestors
bool
IonCacheIRCompiler::emitStoreTypedObjectReferenceProperty()
{
    Register obj = allocator.useRegister(masm, reader.objOperandId());
    int32_t offset = int32StubField(reader.stubOffset());
    TypedThingLayout layout = reader.typedThingLayout();
    ReferenceType type = reader.referenceTypeDescrType();

    ValueOperand val = allocator.useValueRegister(masm, reader.valOperandId());

    AutoScratchRegister scratch1(allocator, masm);
    AutoScratchRegister scratch2(allocator, masm);

    // We don't need to check property types if the property is always a
    // string.
    if (type != ReferenceType::TYPE_STRING) {
        FailurePath* failure;
        if (!addFailurePath(&failure)) {
            return false;
        }
        EmitCheckPropertyTypes(masm, typeCheckInfo_, obj, TypedOrValueRegister(val),
                               *liveRegs_, failure->label());
    }

    // Compute the address being written to.
    LoadTypedThingData(masm, layout, obj, scratch1);
    Address dest(scratch1, offset);
=======
bool IonCacheIRCompiler::emitAllocateAndStoreDynamicSlot() {
  JitSpew(JitSpew_Codegen, __FUNCTION__);
  return emitAddAndStoreSlotShared(CacheOp::AllocateAndStoreDynamicSlot);
}
>>>>>>> upstream-releases

<<<<<<< HEAD
  ValueOperand val = allocator.useValueRegister(masm, reader.valOperandId());
||||||| merged common ancestors
    emitStoreTypedObjectReferenceProp(val, type, dest, scratch2);
=======
bool IonCacheIRCompiler::emitStoreTypedObjectReferenceProperty() {
  JitSpew(JitSpew_Codegen, __FUNCTION__);
  Register obj = allocator.useRegister(masm, reader.objOperandId());
  int32_t offset = int32StubField(reader.stubOffset());
  TypedThingLayout layout = reader.typedThingLayout();
  ReferenceType type = reader.referenceTypeDescrType();
>>>>>>> upstream-releases

<<<<<<< HEAD
  AutoScratchRegister scratch1(allocator, masm);
  AutoScratchRegister scratch2(allocator, masm);
||||||| merged common ancestors
    if (needsPostBarrier() && type != ReferenceType::TYPE_STRING) {
        emitPostBarrierSlot(obj, val, scratch1);
    }
    return true;
}

bool
IonCacheIRCompiler::emitStoreTypedObjectScalarProperty()
{
    Register obj = allocator.useRegister(masm, reader.objOperandId());
    int32_t offset = int32StubField(reader.stubOffset());
    TypedThingLayout layout = reader.typedThingLayout();
    Scalar::Type type = reader.scalarType();
    ValueOperand val = allocator.useValueRegister(masm, reader.valOperandId());
    AutoScratchRegister scratch1(allocator, masm);
    AutoScratchRegister scratch2(allocator, masm);
=======
  ValueOperand val = allocator.useValueRegister(masm, reader.valOperandId());

  AutoScratchRegister scratch1(allocator, masm);
  AutoScratchRegister scratch2(allocator, masm);
>>>>>>> upstream-releases

  // We don't need to check property types if the property is always a
  // string.
  if (type != ReferenceType::TYPE_STRING) {
    FailurePath* failure;
    if (!addFailurePath(&failure)) {
      return false;
    }
    EmitCheckPropertyTypes(masm, typeCheckInfo_, obj, TypedOrValueRegister(val),
                           *liveRegs_, failure->label());
  }

  // Compute the address being written to.
  LoadTypedThingData(masm, layout, obj, scratch1);
  Address dest(scratch1, offset);

  emitStoreTypedObjectReferenceProp(val, type, dest, scratch2);

  if (needsPostBarrier() && type != ReferenceType::TYPE_STRING) {
    emitPostBarrierSlot(obj, val, scratch1);
  }
  return true;
}

<<<<<<< HEAD
bool IonCacheIRCompiler::emitStoreTypedObjectScalarProperty() {
  Register obj = allocator.useRegister(masm, reader.objOperandId());
  int32_t offset = int32StubField(reader.stubOffset());
  TypedThingLayout layout = reader.typedThingLayout();
  Scalar::Type type = reader.scalarType();
  ValueOperand val = allocator.useValueRegister(masm, reader.valOperandId());
  AutoScratchRegister scratch1(allocator, masm);
  AutoScratchRegister scratch2(allocator, masm);
||||||| merged common ancestors
static void
EmitStoreDenseElement(MacroAssembler& masm, const ConstantOrRegister& value,
                      Register elements, BaseObjectElementIndex target)
{
    // If the ObjectElements::CONVERT_DOUBLE_ELEMENTS flag is set, int32 values
    // have to be converted to double first. If the value is not int32, it can
    // always be stored directly.

    Address elementsFlags(elements, ObjectElements::offsetOfFlags());
    if (value.constant()) {
        Value v = value.value();
        Label done;
        if (v.isInt32()) {
            Label dontConvert;
            masm.branchTest32(Assembler::Zero, elementsFlags,
                              Imm32(ObjectElements::CONVERT_DOUBLE_ELEMENTS),
                              &dontConvert);
            masm.storeValue(DoubleValue(v.toInt32()), target);
            masm.jump(&done);
            masm.bind(&dontConvert);
        }
        masm.storeValue(v, target);
        masm.bind(&done);
        return;
    }
=======
bool IonCacheIRCompiler::emitStoreTypedObjectScalarProperty() {
  JitSpew(JitSpew_Codegen, __FUNCTION__);
  Register obj = allocator.useRegister(masm, reader.objOperandId());
  int32_t offset = int32StubField(reader.stubOffset());
  TypedThingLayout layout = reader.typedThingLayout();
  Scalar::Type type = reader.scalarType();
  ValueOperand val = allocator.useValueRegister(masm, reader.valOperandId());
  AutoScratchRegister scratch1(allocator, masm);
  AutoScratchRegister scratch2(allocator, masm);
>>>>>>> upstream-releases

  FailurePath* failure;
  if (!addFailurePath(&failure)) {
    return false;
  }

  // Compute the address being written to.
  LoadTypedThingData(masm, layout, obj, scratch1);
  Address dest(scratch1, offset);

<<<<<<< HEAD
  StoreToTypedArray(cx_, masm, type, val, dest, scratch2, failure->label());
  return true;
}

static void EmitStoreDenseElement(MacroAssembler& masm,
                                  const ConstantOrRegister& value,
                                  Register elements,
                                  BaseObjectElementIndex target) {
  // If the ObjectElements::CONVERT_DOUBLE_ELEMENTS flag is set, int32 values
  // have to be converted to double first. If the value is not int32, it can
  // always be stored directly.
||||||| merged common ancestors
    masm.bind(&convert);
    if (reg.hasValue()) {
        masm.branchTestInt32(Assembler::NotEqual, reg.valueReg(), &storeValue);
        masm.int32ValueToDouble(reg.valueReg(), ScratchDoubleReg);
        masm.storeDouble(ScratchDoubleReg, target);
    } else {
        MOZ_ASSERT(reg.type() == MIRType::Int32);
        masm.convertInt32ToDouble(reg.typedReg().gpr(), ScratchDoubleReg);
        masm.storeDouble(ScratchDoubleReg, target);
    }
=======
  StoreToTypedArray(cx_, masm, type, val, dest, scratch2, failure->label());
  return true;
}
>>>>>>> upstream-releases

<<<<<<< HEAD
  Address elementsFlags(elements, ObjectElements::offsetOfFlags());
  if (value.constant()) {
    Value v = value.value();
    Label done;
    if (v.isInt32()) {
      Label dontConvert;
      masm.branchTest32(Assembler::Zero, elementsFlags,
                        Imm32(ObjectElements::CONVERT_DOUBLE_ELEMENTS),
                        &dontConvert);
      masm.storeValue(DoubleValue(v.toInt32()), target);
      masm.jump(&done);
      masm.bind(&dontConvert);
    }
    masm.storeValue(v, target);
||||||| merged common ancestors
=======
static void EmitStoreDenseElement(MacroAssembler& masm,
                                  const ConstantOrRegister& value,
                                  Register elements,
                                  BaseObjectElementIndex target) {
  // If the ObjectElements::CONVERT_DOUBLE_ELEMENTS flag is set, int32 values
  // have to be converted to double first. If the value is not int32, it can
  // always be stored directly.

  Address elementsFlags(elements, ObjectElements::offsetOfFlags());
  if (value.constant()) {
    Value v = value.value();
    Label done;
    if (v.isInt32()) {
      Label dontConvert;
      masm.branchTest32(Assembler::Zero, elementsFlags,
                        Imm32(ObjectElements::CONVERT_DOUBLE_ELEMENTS),
                        &dontConvert);
      masm.storeValue(DoubleValue(v.toInt32()), target);
      masm.jump(&done);
      masm.bind(&dontConvert);
    }
    masm.storeValue(v, target);
>>>>>>> upstream-releases
    masm.bind(&done);
    return;
  }

<<<<<<< HEAD
  TypedOrValueRegister reg = value.reg();
  if (reg.hasTyped() && reg.type() != MIRType::Int32) {
    masm.storeTypedOrValue(reg, target);
    return;
  }

  Label convert, storeValue, done;
  masm.branchTest32(Assembler::NonZero, elementsFlags,
                    Imm32(ObjectElements::CONVERT_DOUBLE_ELEMENTS), &convert);
  masm.bind(&storeValue);
  masm.storeTypedOrValue(reg, target);
  masm.jump(&done);

  masm.bind(&convert);
  if (reg.hasValue()) {
    masm.branchTestInt32(Assembler::NotEqual, reg.valueReg(), &storeValue);
    masm.int32ValueToDouble(reg.valueReg(), ScratchDoubleReg);
    masm.storeDouble(ScratchDoubleReg, target);
  } else {
    MOZ_ASSERT(reg.type() == MIRType::Int32);
    masm.convertInt32ToDouble(reg.typedReg().gpr(), ScratchDoubleReg);
    masm.storeDouble(ScratchDoubleReg, target);
  }

  masm.bind(&done);
}

static void EmitAssertNoCopyOnWriteElements(MacroAssembler& masm,
                                            Register elementsReg) {
||||||| merged common ancestors
static void
EmitAssertNoCopyOnWriteElements(MacroAssembler& masm, Register elementsReg)
{
=======
  TypedOrValueRegister reg = value.reg();
  if (reg.hasTyped() && reg.type() != MIRType::Int32) {
    masm.storeTypedOrValue(reg, target);
    return;
  }

  Label convert, storeValue, done;
  masm.branchTest32(Assembler::NonZero, elementsFlags,
                    Imm32(ObjectElements::CONVERT_DOUBLE_ELEMENTS), &convert);
  masm.bind(&storeValue);
  masm.storeTypedOrValue(reg, target);
  masm.jump(&done);

  masm.bind(&convert);
  ScratchDoubleScope fpscratch(masm);
  if (reg.hasValue()) {
    masm.branchTestInt32(Assembler::NotEqual, reg.valueReg(), &storeValue);
    masm.int32ValueToDouble(reg.valueReg(), fpscratch);
    masm.boxDouble(fpscratch, target);
  } else {
    MOZ_ASSERT(reg.type() == MIRType::Int32);
    masm.convertInt32ToDouble(reg.typedReg().gpr(), fpscratch);
    masm.boxDouble(fpscratch, target);
  }

  masm.bind(&done);
}

static void EmitAssertNoCopyOnWriteElements(MacroAssembler& masm,
                                            Register elementsReg) {
>>>>>>> upstream-releases
#ifdef DEBUG
  // IonBuilder::initOrSetElemTryCache ensures we have no copy-on-write
  // elements. Assert this in debug builds.
  Address elementsFlags(elementsReg, ObjectElements::offsetOfFlags());
  Label ok;
  masm.branchTest32(Assembler::Zero, elementsFlags,
                    Imm32(ObjectElements::COPY_ON_WRITE), &ok);
  masm.assumeUnreachable("Unexpected copy-on-write elements in Ion IC!");
  masm.bind(&ok);
#endif
}

<<<<<<< HEAD
bool IonCacheIRCompiler::emitStoreDenseElement() {
  Register obj = allocator.useRegister(masm, reader.objOperandId());
  Register index = allocator.useRegister(masm, reader.int32OperandId());
  ConstantOrRegister val =
      allocator.useConstantOrRegister(masm, reader.valOperandId());
||||||| merged common ancestors
bool
IonCacheIRCompiler::emitStoreDenseElement()
{
    Register obj = allocator.useRegister(masm, reader.objOperandId());
    Register index = allocator.useRegister(masm, reader.int32OperandId());
    ConstantOrRegister val = allocator.useConstantOrRegister(masm, reader.valOperandId());
=======
bool IonCacheIRCompiler::emitStoreDenseElement() {
  JitSpew(JitSpew_Codegen, __FUNCTION__);
  Register obj = allocator.useRegister(masm, reader.objOperandId());
  Register index = allocator.useRegister(masm, reader.int32OperandId());
  ConstantOrRegister val =
      allocator.useConstantOrRegister(masm, reader.valOperandId());
>>>>>>> upstream-releases

  AutoScratchRegister scratch1(allocator, masm);
  AutoScratchRegister scratch2(allocator, masm);

  FailurePath* failure;
  if (!addFailurePath(&failure)) {
    return false;
  }

  EmitCheckPropertyTypes(masm, typeCheckInfo_, obj, val, *liveRegs_,
                         failure->label());

  // Load obj->elements in scratch1.
  masm.loadPtr(Address(obj, NativeObject::offsetOfElements()), scratch1);

  EmitAssertNoCopyOnWriteElements(masm, scratch1);

  // Bounds check.
  Address initLength(scratch1, ObjectElements::offsetOfInitializedLength());
  masm.spectreBoundsCheck32(index, initLength, scratch2, failure->label());

  // Hole check.
  BaseObjectElementIndex element(scratch1, index);
  masm.branchTestMagic(Assembler::Equal, element, failure->label());

  // Check for frozen elements. We have to check this here because we attach
  // this stub also for non-extensible objects, and these can become frozen
  // without triggering a Shape change.
  Address flags(scratch1, ObjectElements::offsetOfFlags());
  masm.branchTest32(Assembler::NonZero, flags, Imm32(ObjectElements::FROZEN),
                    failure->label());

  EmitPreBarrier(masm, element, MIRType::Value);
  EmitStoreDenseElement(masm, val, scratch1, element);
  if (needsPostBarrier()) {
    emitPostBarrierElement(obj, val, scratch1, index);
  }
  return true;
}

<<<<<<< HEAD
bool IonCacheIRCompiler::emitStoreDenseElementHole() {
  Register obj = allocator.useRegister(masm, reader.objOperandId());
  Register index = allocator.useRegister(masm, reader.int32OperandId());
  ConstantOrRegister val =
      allocator.useConstantOrRegister(masm, reader.valOperandId());
||||||| merged common ancestors
bool
IonCacheIRCompiler::emitStoreDenseElementHole()
{
    Register obj = allocator.useRegister(masm, reader.objOperandId());
    Register index = allocator.useRegister(masm, reader.int32OperandId());
    ConstantOrRegister val = allocator.useConstantOrRegister(masm, reader.valOperandId());
=======
bool IonCacheIRCompiler::emitStoreDenseElementHole() {
  JitSpew(JitSpew_Codegen, __FUNCTION__);
  Register obj = allocator.useRegister(masm, reader.objOperandId());
  Register index = allocator.useRegister(masm, reader.int32OperandId());
  ConstantOrRegister val =
      allocator.useConstantOrRegister(masm, reader.valOperandId());
>>>>>>> upstream-releases

  // handleAdd boolean is only relevant for Baseline. Ion ICs can always
  // handle adds as we don't have to set any flags on the fallback stub to
  // track this.
  reader.readBool();

  AutoScratchRegister scratch1(allocator, masm);
  AutoScratchRegister scratch2(allocator, masm);

  FailurePath* failure;
  if (!addFailurePath(&failure)) {
    return false;
  }

  EmitCheckPropertyTypes(masm, typeCheckInfo_, obj, val, *liveRegs_,
                         failure->label());

  // Load obj->elements in scratch1.
  masm.loadPtr(Address(obj, NativeObject::offsetOfElements()), scratch1);

  EmitAssertNoCopyOnWriteElements(masm, scratch1);

  Address initLength(scratch1, ObjectElements::offsetOfInitializedLength());
  BaseObjectElementIndex element(scratch1, index);

  Label inBounds, outOfBounds;
  Register spectreTemp = scratch2;
  masm.spectreBoundsCheck32(index, initLength, spectreTemp, &outOfBounds);
  masm.jump(&inBounds);

  masm.bind(&outOfBounds);
  masm.branch32(Assembler::NotEqual, initLength, index, failure->label());

  // If index < capacity, we can add a dense element inline. If not we
  // need to allocate more elements.
  Label capacityOk, allocElement;
  Address capacity(scratch1, ObjectElements::offsetOfCapacity());
  masm.spectreBoundsCheck32(index, capacity, spectreTemp, &allocElement);
  masm.jump(&capacityOk);

  // Check for non-writable array length. We only have to do this if
  // index >= capacity.
  masm.bind(&allocElement);
  Address elementsFlags(scratch1, ObjectElements::offsetOfFlags());
  masm.branchTest32(Assembler::NonZero, elementsFlags,
                    Imm32(ObjectElements::NONWRITABLE_ARRAY_LENGTH),
                    failure->label());

  LiveRegisterSet save(GeneralRegisterSet::Volatile(), liveVolatileFloatRegs());
  save.takeUnchecked(scratch1);
  masm.PushRegsInMask(save);

  masm.setupUnalignedABICall(scratch1);
  masm.loadJSContext(scratch1);
  masm.passABIArg(scratch1);
  masm.passABIArg(obj);
  masm.callWithABI(
      JS_FUNC_TO_DATA_PTR(void*, NativeObject::addDenseElementPure));
  masm.mov(ReturnReg, scratch1);

  masm.PopRegsInMask(save);
  masm.branchIfFalseBool(scratch1, failure->label());

  // Load the reallocated elements pointer.
  masm.loadPtr(Address(obj, NativeObject::offsetOfElements()), scratch1);

  masm.bind(&capacityOk);

  // Increment initLength.
  masm.add32(Imm32(1), initLength);

  // If length is now <= index, increment length too.
  Label skipIncrementLength;
  Address length(scratch1, ObjectElements::offsetOfLength());
  masm.branch32(Assembler::Above, length, index, &skipIncrementLength);
  masm.add32(Imm32(1), length);
  masm.bind(&skipIncrementLength);

  // Skip EmitPreBarrier as the memory is uninitialized.
  Label doStore;
  masm.jump(&doStore);

  masm.bind(&inBounds);

  EmitPreBarrier(masm, element, MIRType::Value);

  masm.bind(&doStore);
  EmitStoreDenseElement(masm, val, scratch1, element);
  if (needsPostBarrier()) {
    emitPostBarrierElement(obj, val, scratch1, index);
  }
  return true;
}

bool IonCacheIRCompiler::emitArrayPush() {
  MOZ_ASSERT_UNREACHABLE("emitArrayPush not supported for IonCaches.");
  return false;
}

<<<<<<< HEAD
bool IonCacheIRCompiler::emitStoreTypedElement() {
  Register obj = allocator.useRegister(masm, reader.objOperandId());
  Register index = allocator.useRegister(masm, reader.int32OperandId());
  ConstantOrRegister val =
      allocator.useConstantOrRegister(masm, reader.valOperandId());
||||||| merged common ancestors
bool
IonCacheIRCompiler::emitStoreTypedElement()
{
    Register obj = allocator.useRegister(masm, reader.objOperandId());
    Register index = allocator.useRegister(masm, reader.int32OperandId());
    ConstantOrRegister val = allocator.useConstantOrRegister(masm, reader.valOperandId());
=======
bool IonCacheIRCompiler::emitStoreTypedElement() {
  JitSpew(JitSpew_Codegen, __FUNCTION__);
  Register obj = allocator.useRegister(masm, reader.objOperandId());
  Register index = allocator.useRegister(masm, reader.int32OperandId());
  ConstantOrRegister val =
      allocator.useConstantOrRegister(masm, reader.valOperandId());
>>>>>>> upstream-releases

  TypedThingLayout layout = reader.typedThingLayout();
  Scalar::Type arrayType = reader.scalarType();
  bool handleOOB = reader.readBool();

  AutoScratchRegister scratch1(allocator, masm);
  AutoScratchRegister scratch2(allocator, masm);

  FailurePath* failure;
  if (!addFailurePath(&failure)) {
    return false;
  }

  // Bounds check.
  Label done;
  LoadTypedThingLength(masm, layout, obj, scratch1);
  masm.spectreBoundsCheck32(index, scratch1, scratch2,
                            handleOOB ? &done : failure->label());

  // Load the elements vector.
  LoadTypedThingData(masm, layout, obj, scratch1);

  BaseIndex dest(scratch1, index,
                 ScaleFromElemWidth(Scalar::byteSize(arrayType)));

  FloatRegister maybeTempDouble = ic_->asSetPropertyIC()->maybeTempDouble();
  FloatRegister maybeTempFloat32 = ic_->asSetPropertyIC()->maybeTempFloat32();
  MOZ_ASSERT(maybeTempDouble != InvalidFloatReg);
  MOZ_ASSERT_IF(jit::hasUnaliasedDouble(), maybeTempFloat32 != InvalidFloatReg);

  if (arrayType == Scalar::Float32) {
    FloatRegister tempFloat =
        hasUnaliasedDouble() ? maybeTempFloat32 : maybeTempDouble;
    if (!masm.convertConstantOrRegisterToFloat(cx_, val, tempFloat,
                                               failure->label())) {
      return false;
    }
    masm.storeToTypedFloatArray(arrayType, tempFloat, dest);
  } else if (arrayType == Scalar::Float64) {
    if (!masm.convertConstantOrRegisterToDouble(cx_, val, maybeTempDouble,
                                                failure->label())) {
      return false;
    }
    masm.storeToTypedFloatArray(arrayType, maybeTempDouble, dest);
  } else {
    Register valueToStore = scratch2;
    if (arrayType == Scalar::Uint8Clamped) {
      if (!masm.clampConstantOrRegisterToUint8(
              cx_, val, maybeTempDouble, valueToStore, failure->label())) {
        return false;
      }
    } else {
      if (!masm.truncateConstantOrRegisterToInt32(
              cx_, val, maybeTempDouble, valueToStore, failure->label())) {
        return false;
      }
    }
    masm.storeToTypedIntArray(arrayType, valueToStore, dest);
  }

  masm.bind(&done);
  return true;
}

<<<<<<< HEAD
bool IonCacheIRCompiler::emitCallNativeSetter() {
  AutoSaveLiveRegisters save(*this);

  Register obj = allocator.useRegister(masm, reader.objOperandId());
  JSFunction* target = &objectStubField(reader.stubOffset())->as<JSFunction>();
  MOZ_ASSERT(target->isNative());
  ConstantOrRegister val =
      allocator.useConstantOrRegister(masm, reader.valOperandId());

  AutoScratchRegister argJSContext(allocator, masm);
  AutoScratchRegister argVp(allocator, masm);
  AutoScratchRegister argUintN(allocator, masm);
  AutoScratchRegister scratch(allocator, masm);

  allocator.discardStack(masm);

  // Set up the call:
  //  bool (*)(JSContext*, unsigned, Value* vp)
  // vp[0] is callee/outparam
  // vp[1] is |this|
  // vp[2] is the value

  // Build vp and move the base into argVpReg.
  masm.Push(val);
  masm.Push(TypedOrValueRegister(MIRType::Object, AnyRegister(obj)));
  masm.Push(ObjectValue(*target));
  masm.moveStackPtrTo(argVp.get());

  // Preload other regs.
  masm.loadJSContext(argJSContext);
  masm.move32(Imm32(1), argUintN);

  // Push marking data for later use.
  masm.Push(argUintN);
  pushStubCodePointer();

  if (!masm.icBuildOOLFakeExitFrame(GetReturnAddressToIonCode(cx_), save)) {
    return false;
  }
  masm.enterFakeExitFrame(argJSContext, scratch, ExitFrameType::IonOOLNative);

  if (target->realm() != cx_->realm()) {
    masm.switchToRealm(target->realm(), scratch);
  }
||||||| merged common ancestors
bool
IonCacheIRCompiler::emitCallNativeSetter()
{
    AutoSaveLiveRegisters save(*this);

    Register obj = allocator.useRegister(masm, reader.objOperandId());
    JSFunction* target = &objectStubField(reader.stubOffset())->as<JSFunction>();
    MOZ_ASSERT(target->isNative());
    ConstantOrRegister val = allocator.useConstantOrRegister(masm, reader.valOperandId());

    AutoScratchRegister argJSContext(allocator, masm);
    AutoScratchRegister argVp(allocator, masm);
    AutoScratchRegister argUintN(allocator, masm);
    AutoScratchRegister scratch(allocator, masm);

    allocator.discardStack(masm);

    // Set up the call:
    //  bool (*)(JSContext*, unsigned, Value* vp)
    // vp[0] is callee/outparam
    // vp[1] is |this|
    // vp[2] is the value

    // Build vp and move the base into argVpReg.
    masm.Push(val);
    masm.Push(TypedOrValueRegister(MIRType::Object, AnyRegister(obj)));
    masm.Push(ObjectValue(*target));
    masm.moveStackPtrTo(argVp.get());

    // Preload other regs.
    masm.loadJSContext(argJSContext);
    masm.move32(Imm32(1), argUintN);

    // Push marking data for later use.
    masm.Push(argUintN);
    pushStubCodePointer();

    if (!masm.icBuildOOLFakeExitFrame(GetReturnAddressToIonCode(cx_), save)) {
        return false;
    }
    masm.enterFakeExitFrame(argJSContext, scratch, ExitFrameType::IonOOLNative);

    if (target->realm() != cx_->realm()) {
        masm.switchToRealm(target->realm(), scratch);
    }
=======
bool IonCacheIRCompiler::emitCallNativeSetter() {
  JitSpew(JitSpew_Codegen, __FUNCTION__);
  AutoSaveLiveRegisters save(*this);
>>>>>>> upstream-releases

<<<<<<< HEAD
  // Make the call.
  masm.setupUnalignedABICall(scratch);
  masm.passABIArg(argJSContext);
  masm.passABIArg(argUintN);
  masm.passABIArg(argVp);
  masm.callWithABI(JS_FUNC_TO_DATA_PTR(void*, target->native()),
                   MoveOp::GENERAL,
                   CheckUnsafeCallWithABI::DontCheckHasExitFrame);
||||||| merged common ancestors
    // Make the call.
    masm.setupUnalignedABICall(scratch);
    masm.passABIArg(argJSContext);
    masm.passABIArg(argUintN);
    masm.passABIArg(argVp);
    masm.callWithABI(JS_FUNC_TO_DATA_PTR(void*, target->native()), MoveOp::GENERAL,
                     CheckUnsafeCallWithABI::DontCheckHasExitFrame);
=======
  Register obj = allocator.useRegister(masm, reader.objOperandId());
  JSFunction* target = &objectStubField(reader.stubOffset())->as<JSFunction>();
  MOZ_ASSERT(target->isNative());
  ConstantOrRegister val =
      allocator.useConstantOrRegister(masm, reader.valOperandId());
>>>>>>> upstream-releases

<<<<<<< HEAD
  // Test for failure.
  masm.branchIfFalseBool(ReturnReg, masm.exceptionLabel());
||||||| merged common ancestors
    // Test for failure.
    masm.branchIfFalseBool(ReturnReg, masm.exceptionLabel());
=======
  AutoScratchRegister argJSContext(allocator, masm);
  AutoScratchRegister argVp(allocator, masm);
  AutoScratchRegister argUintN(allocator, masm);
  AutoScratchRegister scratch(allocator, masm);
>>>>>>> upstream-releases

<<<<<<< HEAD
  if (target->realm() != cx_->realm()) {
    masm.switchToRealm(cx_->realm(), ReturnReg);
  }
||||||| merged common ancestors
    if (target->realm() != cx_->realm()) {
        masm.switchToRealm(cx_->realm(), ReturnReg);
    }
=======
  allocator.discardStack(masm);
>>>>>>> upstream-releases

<<<<<<< HEAD
  masm.adjustStack(IonOOLNativeExitFrameLayout::Size(1));
  return true;
}
||||||| merged common ancestors
    masm.adjustStack(IonOOLNativeExitFrameLayout::Size(1));
    return true;
}
=======
  // Set up the call:
  //  bool (*)(JSContext*, unsigned, Value* vp)
  // vp[0] is callee/outparam
  // vp[1] is |this|
  // vp[2] is the value
>>>>>>> upstream-releases

<<<<<<< HEAD
bool IonCacheIRCompiler::emitCallScriptedSetter() {
  AutoSaveLiveRegisters save(*this);
||||||| merged common ancestors
bool
IonCacheIRCompiler::emitCallScriptedSetter()
{
    AutoSaveLiveRegisters save(*this);
=======
  // Build vp and move the base into argVpReg.
  masm.Push(val);
  masm.Push(TypedOrValueRegister(MIRType::Object, AnyRegister(obj)));
  masm.Push(ObjectValue(*target));
  masm.moveStackPtrTo(argVp.get());
>>>>>>> upstream-releases

<<<<<<< HEAD
  Register obj = allocator.useRegister(masm, reader.objOperandId());
  JSFunction* target = &objectStubField(reader.stubOffset())->as<JSFunction>();
  ConstantOrRegister val =
      allocator.useConstantOrRegister(masm, reader.valOperandId());
||||||| merged common ancestors
    Register obj = allocator.useRegister(masm, reader.objOperandId());
    JSFunction* target = &objectStubField(reader.stubOffset())->as<JSFunction>();
    ConstantOrRegister val = allocator.useConstantOrRegister(masm, reader.valOperandId());
=======
  // Preload other regs.
  masm.loadJSContext(argJSContext);
  masm.move32(Imm32(1), argUintN);
>>>>>>> upstream-releases

<<<<<<< HEAD
  bool isCrossRealm = reader.readBool();
  MOZ_ASSERT(isCrossRealm == (cx_->realm() != target->realm()));
||||||| merged common ancestors
    bool isCrossRealm = reader.readBool();
    MOZ_ASSERT(isCrossRealm == (cx_->realm() != target->realm()));
=======
  // Push marking data for later use.
  masm.Push(argUintN);
  pushStubCodePointer();
>>>>>>> upstream-releases

<<<<<<< HEAD
  AutoScratchRegister scratch(allocator, masm);
||||||| merged common ancestors
    AutoScratchRegister scratch(allocator, masm);
=======
  if (!masm.icBuildOOLFakeExitFrame(GetReturnAddressToIonCode(cx_), save)) {
    return false;
  }
  masm.enterFakeExitFrame(argJSContext, scratch, ExitFrameType::IonOOLNative);

  if (target->realm() != cx_->realm()) {
    masm.switchToRealm(target->realm(), scratch);
  }

  // Make the call.
  masm.setupUnalignedABICall(scratch);
  masm.passABIArg(argJSContext);
  masm.passABIArg(argUintN);
  masm.passABIArg(argVp);
  masm.callWithABI(JS_FUNC_TO_DATA_PTR(void*, target->native()),
                   MoveOp::GENERAL,
                   CheckUnsafeCallWithABI::DontCheckHasExitFrame);

  // Test for failure.
  masm.branchIfFalseBool(ReturnReg, masm.exceptionLabel());

  if (target->realm() != cx_->realm()) {
    masm.switchToRealm(cx_->realm(), ReturnReg);
  }

  masm.adjustStack(IonOOLNativeExitFrameLayout::Size(1));
  return true;
}

bool IonCacheIRCompiler::emitCallScriptedSetter() {
  JitSpew(JitSpew_Codegen, __FUNCTION__);
  AutoSaveLiveRegisters save(*this);

  Register obj = allocator.useRegister(masm, reader.objOperandId());
  JSFunction* target = &objectStubField(reader.stubOffset())->as<JSFunction>();
  ConstantOrRegister val =
      allocator.useConstantOrRegister(masm, reader.valOperandId());

  bool isSameRealm = reader.readBool();
  MOZ_ASSERT(isSameRealm == (cx_->realm() == target->realm()));

  AutoScratchRegister scratch(allocator, masm);

  allocator.discardStack(masm);

  uint32_t framePushedBefore = masm.framePushed();

  // Construct IonICCallFrameLayout.
  uint32_t descriptor = MakeFrameDescriptor(
      masm.framePushed(), FrameType::IonJS, IonICCallFrameLayout::Size());
  pushStubCodePointer();
  masm.Push(Imm32(descriptor));
  masm.Push(ImmPtr(GetReturnAddressToIonCode(cx_)));

  // The JitFrameLayout pushed below will be aligned to JitStackAlignment,
  // so we just have to make sure the stack is aligned after we push the
  // |this| + argument Values.
  size_t numArgs = Max<size_t>(1, target->nargs());
  uint32_t argSize = (numArgs + 1) * sizeof(Value);
  uint32_t padding =
      ComputeByteAlignment(masm.framePushed() + argSize, JitStackAlignment);
  MOZ_ASSERT(padding % sizeof(uintptr_t) == 0);
  MOZ_ASSERT(padding < JitStackAlignment);
  masm.reserveStack(padding);

  for (size_t i = 1; i < target->nargs(); i++) {
    masm.Push(UndefinedValue());
  }
  masm.Push(val);
  masm.Push(TypedOrValueRegister(MIRType::Object, AnyRegister(obj)));
>>>>>>> upstream-releases

<<<<<<< HEAD
  allocator.discardStack(masm);
||||||| merged common ancestors
    allocator.discardStack(masm);
=======
  if (!isSameRealm) {
    masm.switchToRealm(target->realm(), scratch);
  }
>>>>>>> upstream-releases

<<<<<<< HEAD
  uint32_t framePushedBefore = masm.framePushed();
||||||| merged common ancestors
    uint32_t framePushedBefore = masm.framePushed();
=======
  masm.movePtr(ImmGCPtr(target), scratch);
>>>>>>> upstream-releases

<<<<<<< HEAD
  // Construct IonICCallFrameLayout.
  uint32_t descriptor = MakeFrameDescriptor(
      masm.framePushed(), FrameType::IonJS, IonICCallFrameLayout::Size());
  pushStubCodePointer();
  masm.Push(Imm32(descriptor));
  masm.Push(ImmPtr(GetReturnAddressToIonCode(cx_)));
||||||| merged common ancestors
    // Construct IonICCallFrameLayout.
    uint32_t descriptor = MakeFrameDescriptor(masm.framePushed(), FrameType::IonJS,
                                              IonICCallFrameLayout::Size());
    pushStubCodePointer();
    masm.Push(Imm32(descriptor));
    masm.Push(ImmPtr(GetReturnAddressToIonCode(cx_)));
=======
  descriptor = MakeFrameDescriptor(argSize + padding, FrameType::IonICCall,
                                   JitFrameLayout::Size());
  masm.Push(Imm32(1));  // argc
  masm.Push(scratch);
  masm.Push(Imm32(descriptor));
>>>>>>> upstream-releases

<<<<<<< HEAD
  // The JitFrameLayout pushed below will be aligned to JitStackAlignment,
  // so we just have to make sure the stack is aligned after we push the
  // |this| + argument Values.
  size_t numArgs = Max<size_t>(1, target->nargs());
  uint32_t argSize = (numArgs + 1) * sizeof(Value);
  uint32_t padding =
      ComputeByteAlignment(masm.framePushed() + argSize, JitStackAlignment);
  MOZ_ASSERT(padding % sizeof(uintptr_t) == 0);
  MOZ_ASSERT(padding < JitStackAlignment);
  masm.reserveStack(padding);
||||||| merged common ancestors
    // The JitFrameLayout pushed below will be aligned to JitStackAlignment,
    // so we just have to make sure the stack is aligned after we push the
    // |this| + argument Values.
    size_t numArgs = Max<size_t>(1, target->nargs());
    uint32_t argSize = (numArgs + 1) * sizeof(Value);
    uint32_t padding = ComputeByteAlignment(masm.framePushed() + argSize, JitStackAlignment);
    MOZ_ASSERT(padding % sizeof(uintptr_t) == 0);
    MOZ_ASSERT(padding < JitStackAlignment);
    masm.reserveStack(padding);
=======
  // Check stack alignment. Add sizeof(uintptr_t) for the return address.
  MOZ_ASSERT(((masm.framePushed() + sizeof(uintptr_t)) % JitStackAlignment) ==
             0);
>>>>>>> upstream-releases

<<<<<<< HEAD
  for (size_t i = 1; i < target->nargs(); i++) {
    masm.Push(UndefinedValue());
  }
  masm.Push(val);
  masm.Push(TypedOrValueRegister(MIRType::Object, AnyRegister(obj)));
||||||| merged common ancestors
    for (size_t i = 1; i < target->nargs(); i++) {
        masm.Push(UndefinedValue());
    }
    masm.Push(val);
    masm.Push(TypedOrValueRegister(MIRType::Object, AnyRegister(obj)));
=======
  // The setter currently has a jit entry or a non-lazy script. We will only
  // relazify when we do a shrinking GC and when that happens we will also
  // purge IC stubs.
  MOZ_ASSERT(target->hasJitEntry());
  masm.loadJitCodeRaw(scratch, scratch);
  masm.callJit(scratch);
>>>>>>> upstream-releases

<<<<<<< HEAD
  if (isCrossRealm) {
    masm.switchToRealm(target->realm(), scratch);
  }
||||||| merged common ancestors
    if (isCrossRealm) {
        masm.switchToRealm(target->realm(), scratch);
    }
=======
  if (!isSameRealm) {
    masm.switchToRealm(cx_->realm(), ReturnReg);
  }
>>>>>>> upstream-releases

<<<<<<< HEAD
  masm.movePtr(ImmGCPtr(target), scratch);
||||||| merged common ancestors
    masm.movePtr(ImmGCPtr(target), scratch);
=======
  masm.freeStack(masm.framePushed() - framePushedBefore);
  return true;
}
>>>>>>> upstream-releases

<<<<<<< HEAD
  descriptor = MakeFrameDescriptor(argSize + padding, FrameType::IonICCall,
                                   JitFrameLayout::Size());
  masm.Push(Imm32(1));  // argc
  masm.Push(scratch);
  masm.Push(Imm32(descriptor));
||||||| merged common ancestors
    descriptor = MakeFrameDescriptor(argSize + padding, FrameType::IonICCall,
                                     JitFrameLayout::Size());
    masm.Push(Imm32(1)); // argc
    masm.Push(scratch);
    masm.Push(Imm32(descriptor));
=======
bool IonCacheIRCompiler::emitCallSetArrayLength() {
  JitSpew(JitSpew_Codegen, __FUNCTION__);
  AutoSaveLiveRegisters save(*this);
>>>>>>> upstream-releases

<<<<<<< HEAD
  // Check stack alignment. Add sizeof(uintptr_t) for the return address.
  MOZ_ASSERT(((masm.framePushed() + sizeof(uintptr_t)) % JitStackAlignment) ==
             0);
||||||| merged common ancestors
    // Check stack alignment. Add sizeof(uintptr_t) for the return address.
    MOZ_ASSERT(((masm.framePushed() + sizeof(uintptr_t)) % JitStackAlignment) == 0);
=======
  Register obj = allocator.useRegister(masm, reader.objOperandId());
  bool strict = reader.readBool();
  ConstantOrRegister val =
      allocator.useConstantOrRegister(masm, reader.valOperandId());
>>>>>>> upstream-releases

<<<<<<< HEAD
  // The setter currently has a jit entry or a non-lazy script. We will only
  // relazify when we do a shrinking GC and when that happens we will also
  // purge IC stubs.
  MOZ_ASSERT(target->hasJitEntry());
  masm.loadJitCodeRaw(scratch, scratch);
  masm.callJit(scratch);
||||||| merged common ancestors
    // The setter currently has a jit entry or a non-lazy script. We will only
    // relazify when we do a shrinking GC and when that happens we will also
    // purge IC stubs.
    MOZ_ASSERT(target->hasJitEntry());
    masm.loadJitCodeRaw(scratch, scratch);
    masm.callJit(scratch);
=======
  allocator.discardStack(masm);
  prepareVMCall(masm, save);
>>>>>>> upstream-releases

<<<<<<< HEAD
  if (isCrossRealm) {
    masm.switchToRealm(cx_->realm(), ReturnReg);
  }
||||||| merged common ancestors
    if (isCrossRealm) {
        masm.switchToRealm(cx_->realm(), ReturnReg);
    }
=======
  masm.Push(Imm32(strict));
  masm.Push(val);
  masm.Push(obj);
>>>>>>> upstream-releases

<<<<<<< HEAD
  masm.freeStack(masm.framePushed() - framePushedBefore);
  return true;
||||||| merged common ancestors
    masm.freeStack(masm.framePushed() - framePushedBefore);
    return true;
=======
  using Fn = bool (*)(JSContext*, HandleObject, HandleValue, bool);
  callVM<Fn, jit::SetArrayLength>(masm);
  return true;
>>>>>>> upstream-releases
}

<<<<<<< HEAD
bool IonCacheIRCompiler::emitCallSetArrayLength() {
  AutoSaveLiveRegisters save(*this);
||||||| merged common ancestors
bool
IonCacheIRCompiler::emitCallSetArrayLength()
{
    AutoSaveLiveRegisters save(*this);
=======
bool IonCacheIRCompiler::emitCallProxySet() {
  JitSpew(JitSpew_Codegen, __FUNCTION__);
  AutoSaveLiveRegisters save(*this);
>>>>>>> upstream-releases

<<<<<<< HEAD
  Register obj = allocator.useRegister(masm, reader.objOperandId());
  bool strict = reader.readBool();
  ConstantOrRegister val =
      allocator.useConstantOrRegister(masm, reader.valOperandId());
||||||| merged common ancestors
    Register obj = allocator.useRegister(masm, reader.objOperandId());
    bool strict = reader.readBool();
    ConstantOrRegister val = allocator.useConstantOrRegister(masm, reader.valOperandId());
=======
  Register obj = allocator.useRegister(masm, reader.objOperandId());
  ConstantOrRegister val =
      allocator.useConstantOrRegister(masm, reader.valOperandId());
  jsid id = idStubField(reader.stubOffset());
  bool strict = reader.readBool();
>>>>>>> upstream-releases

<<<<<<< HEAD
  allocator.discardStack(masm);
  prepareVMCall(masm, save);
||||||| merged common ancestors
    allocator.discardStack(masm);
    prepareVMCall(masm, save);
=======
  AutoScratchRegister scratch(allocator, masm);
>>>>>>> upstream-releases

<<<<<<< HEAD
  masm.Push(Imm32(strict));
  masm.Push(val);
  masm.Push(obj);
||||||| merged common ancestors
    masm.Push(Imm32(strict));
    masm.Push(val);
    masm.Push(obj);
=======
  allocator.discardStack(masm);
  prepareVMCall(masm, save);
>>>>>>> upstream-releases

<<<<<<< HEAD
  return callVM(masm, SetArrayLengthInfo);
}
||||||| merged common ancestors
    return callVM(masm, SetArrayLengthInfo);
}
=======
  masm.Push(Imm32(strict));
  masm.Push(val);
  masm.Push(id, scratch);
  masm.Push(obj);
>>>>>>> upstream-releases

<<<<<<< HEAD
bool IonCacheIRCompiler::emitCallProxySet() {
  AutoSaveLiveRegisters save(*this);
||||||| merged common ancestors
bool
IonCacheIRCompiler::emitCallProxySet()
{
    AutoSaveLiveRegisters save(*this);
=======
  using Fn = bool (*)(JSContext*, HandleObject, HandleId, HandleValue, bool);
  callVM<Fn, ProxySetProperty>(masm);
  return true;
}
>>>>>>> upstream-releases

<<<<<<< HEAD
  Register obj = allocator.useRegister(masm, reader.objOperandId());
  ConstantOrRegister val =
      allocator.useConstantOrRegister(masm, reader.valOperandId());
  jsid id = idStubField(reader.stubOffset());
  bool strict = reader.readBool();
||||||| merged common ancestors
    Register obj = allocator.useRegister(masm, reader.objOperandId());
    ConstantOrRegister val = allocator.useConstantOrRegister(masm, reader.valOperandId());
    jsid id = idStubField(reader.stubOffset());
    bool strict = reader.readBool();
=======
bool IonCacheIRCompiler::emitCallProxySetByValue() {
  JitSpew(JitSpew_Codegen, __FUNCTION__);
  AutoSaveLiveRegisters save(*this);
>>>>>>> upstream-releases

<<<<<<< HEAD
  AutoScratchRegister scratch(allocator, masm);
||||||| merged common ancestors
    AutoScratchRegister scratch(allocator, masm);
=======
  Register obj = allocator.useRegister(masm, reader.objOperandId());
  ConstantOrRegister idVal =
      allocator.useConstantOrRegister(masm, reader.valOperandId());
  ConstantOrRegister val =
      allocator.useConstantOrRegister(masm, reader.valOperandId());
  bool strict = reader.readBool();
>>>>>>> upstream-releases

  allocator.discardStack(masm);
  prepareVMCall(masm, save);

<<<<<<< HEAD
  masm.Push(Imm32(strict));
  masm.Push(val);
  masm.Push(id, scratch);
  masm.Push(obj);
||||||| merged common ancestors
    masm.Push(Imm32(strict));
    masm.Push(val);
    masm.Push(id, scratch);
    masm.Push(obj);
=======
  masm.Push(Imm32(strict));
  masm.Push(val);
  masm.Push(idVal);
  masm.Push(obj);
>>>>>>> upstream-releases

<<<<<<< HEAD
  return callVM(masm, ProxySetPropertyInfo);
||||||| merged common ancestors
    return callVM(masm, ProxySetPropertyInfo);
=======
  using Fn = bool (*)(JSContext*, HandleObject, HandleValue, HandleValue, bool);
  callVM<Fn, ProxySetPropertyByValue>(masm);
  return true;
>>>>>>> upstream-releases
}

<<<<<<< HEAD
bool IonCacheIRCompiler::emitCallProxySetByValue() {
  AutoSaveLiveRegisters save(*this);
||||||| merged common ancestors
bool
IonCacheIRCompiler::emitCallProxySetByValue()
{
    AutoSaveLiveRegisters save(*this);
=======
bool IonCacheIRCompiler::emitCallAddOrUpdateSparseElementHelper() {
  JitSpew(JitSpew_Codegen, __FUNCTION__);
  AutoSaveLiveRegisters save(*this);
>>>>>>> upstream-releases

<<<<<<< HEAD
  Register obj = allocator.useRegister(masm, reader.objOperandId());
  ConstantOrRegister idVal =
      allocator.useConstantOrRegister(masm, reader.valOperandId());
  ConstantOrRegister val =
      allocator.useConstantOrRegister(masm, reader.valOperandId());
  bool strict = reader.readBool();
||||||| merged common ancestors
    Register obj = allocator.useRegister(masm, reader.objOperandId());
    ConstantOrRegister idVal = allocator.useConstantOrRegister(masm, reader.valOperandId());
    ConstantOrRegister val = allocator.useConstantOrRegister(masm, reader.valOperandId());
    bool strict = reader.readBool();
=======
  Register obj = allocator.useRegister(masm, reader.objOperandId());
  Register id = allocator.useRegister(masm, reader.int32OperandId());
  ValueOperand val = allocator.useValueRegister(masm, reader.valOperandId());
  bool strict = reader.readBool();
>>>>>>> upstream-releases

  allocator.discardStack(masm);
  prepareVMCall(masm, save);

<<<<<<< HEAD
  masm.Push(Imm32(strict));
  masm.Push(val);
  masm.Push(idVal);
  masm.Push(obj);
||||||| merged common ancestors
    masm.Push(Imm32(strict));
    masm.Push(val);
    masm.Push(idVal);
    masm.Push(obj);
=======
  masm.Push(Imm32(strict));
  masm.Push(val);
  masm.Push(id);
  masm.Push(obj);
>>>>>>> upstream-releases

<<<<<<< HEAD
  return callVM(masm, ProxySetPropertyByValueInfo);
||||||| merged common ancestors
    return callVM(masm, ProxySetPropertyByValueInfo);
=======
  using Fn = bool (*)(JSContext * cx, HandleArrayObject obj, int32_t int_id,
                      HandleValue v, bool strict);
  callVM<Fn, AddOrUpdateSparseElementHelper>(masm);
  return true;
>>>>>>> upstream-releases
}

<<<<<<< HEAD
bool IonCacheIRCompiler::emitCallAddOrUpdateSparseElementHelper() {
  AutoSaveLiveRegisters save(*this);
||||||| merged common ancestors
bool
IonCacheIRCompiler::emitMegamorphicSetElement()
{
    AutoSaveLiveRegisters save(*this);
=======
bool IonCacheIRCompiler::emitMegamorphicSetElement() {
  JitSpew(JitSpew_Codegen, __FUNCTION__);
  AutoSaveLiveRegisters save(*this);
>>>>>>> upstream-releases

<<<<<<< HEAD
  Register obj = allocator.useRegister(masm, reader.objOperandId());
  Register id = allocator.useRegister(masm, reader.int32OperandId());
  ValueOperand val = allocator.useValueRegister(masm, reader.valOperandId());
  bool strict = reader.readBool();
||||||| merged common ancestors
    Register obj = allocator.useRegister(masm, reader.objOperandId());
    ConstantOrRegister idVal = allocator.useConstantOrRegister(masm, reader.valOperandId());
    ConstantOrRegister val = allocator.useConstantOrRegister(masm, reader.valOperandId());
    bool strict = reader.readBool();
=======
  Register obj = allocator.useRegister(masm, reader.objOperandId());
  ConstantOrRegister idVal =
      allocator.useConstantOrRegister(masm, reader.valOperandId());
  ConstantOrRegister val =
      allocator.useConstantOrRegister(masm, reader.valOperandId());
  bool strict = reader.readBool();
>>>>>>> upstream-releases

  allocator.discardStack(masm);
  prepareVMCall(masm, save);

<<<<<<< HEAD
  masm.Push(Imm32(strict));
  masm.Push(val);
  masm.Push(id);
  masm.Push(obj);
||||||| merged common ancestors
    masm.Push(Imm32(strict));
    masm.Push(TypedOrValueRegister(MIRType::Object, AnyRegister(obj)));
    masm.Push(val);
    masm.Push(idVal);
    masm.Push(obj);
=======
  masm.Push(Imm32(strict));
  masm.Push(TypedOrValueRegister(MIRType::Object, AnyRegister(obj)));
  masm.Push(val);
  masm.Push(idVal);
  masm.Push(obj);
>>>>>>> upstream-releases

<<<<<<< HEAD
  return callVM(masm, AddOrUpdateSparseElementHelperInfo);
||||||| merged common ancestors
    return callVM(masm, SetObjectElementInfo);
=======
  using Fn = bool (*)(JSContext*, HandleObject, HandleValue, HandleValue,
                      HandleValue, bool);
  callVM<Fn, SetObjectElementWithReceiver>(masm);
  return true;
>>>>>>> upstream-releases
}

<<<<<<< HEAD
bool IonCacheIRCompiler::emitCallGetSparseElementResult() {
  AutoSaveLiveRegisters save(*this);
  AutoOutputRegister output(*this);
||||||| merged common ancestors
bool
IonCacheIRCompiler::emitLoadTypedObjectResult()
{
    AutoOutputRegister output(*this);
    Register obj = allocator.useRegister(masm, reader.objOperandId());
    AutoScratchRegister scratch1(allocator, masm);
    AutoScratchRegister scratch2(allocator, masm);
=======
bool IonCacheIRCompiler::emitLoadTypedObjectResult() {
  JitSpew(JitSpew_Codegen, __FUNCTION__);
  AutoOutputRegister output(*this);
  Register obj = allocator.useRegister(masm, reader.objOperandId());
  AutoScratchRegister scratch1(allocator, masm);
  AutoScratchRegister scratch2(allocator, masm);
>>>>>>> upstream-releases

<<<<<<< HEAD
  Register obj = allocator.useRegister(masm, reader.objOperandId());
  Register id = allocator.useRegister(masm, reader.int32OperandId());
||||||| merged common ancestors
    TypedThingLayout layout = reader.typedThingLayout();
    uint32_t typeDescr = reader.typeDescrKey();
    uint32_t fieldOffset = int32StubField(reader.stubOffset());
=======
  TypedThingLayout layout = reader.typedThingLayout();
  uint32_t typeDescr = reader.typeDescrKey();
  uint32_t fieldOffset = int32StubField(reader.stubOffset());
>>>>>>> upstream-releases

<<<<<<< HEAD
  allocator.discardStack(masm);
  prepareVMCall(masm, save);
  masm.Push(id);
  masm.Push(obj);
||||||| merged common ancestors
    // Get the object's data pointer.
    LoadTypedThingData(masm, layout, obj, scratch1);
=======
  // Get the object's data pointer.
  LoadTypedThingData(masm, layout, obj, scratch1);
>>>>>>> upstream-releases

<<<<<<< HEAD
  if (!callVM(masm, GetSparseElementHelperInfo)) {
    return false;
  }
||||||| merged common ancestors
    Address fieldAddr(scratch1, fieldOffset);
    emitLoadTypedObjectResultShared(fieldAddr, scratch2, typeDescr, output);
    return true;
}
=======
  Address fieldAddr(scratch1, fieldOffset);
  emitLoadTypedObjectResultShared(fieldAddr, scratch2, typeDescr, output);
  return true;
}
>>>>>>> upstream-releases

<<<<<<< HEAD
  masm.storeCallResultValue(output);
  return true;
||||||| merged common ancestors
bool
IonCacheIRCompiler::emitTypeMonitorResult()
{
    return emitReturnFromIC();
=======
bool IonCacheIRCompiler::emitTypeMonitorResult() {
  JitSpew(JitSpew_Codegen, __FUNCTION__);
  return emitReturnFromIC();
>>>>>>> upstream-releases
}

<<<<<<< HEAD
bool IonCacheIRCompiler::emitMegamorphicSetElement() {
  AutoSaveLiveRegisters save(*this);
||||||| merged common ancestors
bool
IonCacheIRCompiler::emitReturnFromIC()
{
    if (!savedLiveRegs_) {
        allocator.restoreInputState(masm);
    }
=======
bool IonCacheIRCompiler::emitReturnFromIC() {
  JitSpew(JitSpew_Codegen, __FUNCTION__);
  if (!savedLiveRegs_) {
    allocator.restoreInputState(masm);
  }
>>>>>>> upstream-releases

<<<<<<< HEAD
  Register obj = allocator.useRegister(masm, reader.objOperandId());
  ConstantOrRegister idVal =
      allocator.useConstantOrRegister(masm, reader.valOperandId());
  ConstantOrRegister val =
      allocator.useConstantOrRegister(masm, reader.valOperandId());
  bool strict = reader.readBool();
||||||| merged common ancestors
    RepatchLabel rejoin;
    rejoinOffset_ = masm.jumpWithPatch(&rejoin);
    masm.bind(&rejoin);
    return true;
}
=======
  RepatchLabel rejoin;
  rejoinOffset_ = masm.jumpWithPatch(&rejoin);
  masm.bind(&rejoin);
  return true;
}
>>>>>>> upstream-releases

<<<<<<< HEAD
  allocator.discardStack(masm);
  prepareVMCall(masm, save);

  masm.Push(Imm32(strict));
  masm.Push(TypedOrValueRegister(MIRType::Object, AnyRegister(obj)));
  masm.Push(val);
  masm.Push(idVal);
  masm.Push(obj);

  return callVM(masm, SetObjectElementInfo);
}
||||||| merged common ancestors
bool
IonCacheIRCompiler::emitLoadStackValue()
{
    MOZ_ASSERT_UNREACHABLE("emitLoadStackValue not supported for IonCaches.");
    return false;
}
=======
bool IonCacheIRCompiler::emitGuardAndGetIterator() {
  JitSpew(JitSpew_Codegen, __FUNCTION__);
  Register obj = allocator.useRegister(masm, reader.objOperandId());
>>>>>>> upstream-releases

<<<<<<< HEAD
bool IonCacheIRCompiler::emitLoadTypedObjectResult() {
  AutoOutputRegister output(*this);
  Register obj = allocator.useRegister(masm, reader.objOperandId());
  AutoScratchRegister scratch1(allocator, masm);
  AutoScratchRegister scratch2(allocator, masm);
||||||| merged common ancestors
bool
IonCacheIRCompiler::emitGuardAndGetIterator()
{
    Register obj = allocator.useRegister(masm, reader.objOperandId());
=======
  AutoScratchRegister scratch1(allocator, masm);
  AutoScratchRegister scratch2(allocator, masm);
  AutoScratchRegister niScratch(allocator, masm);
>>>>>>> upstream-releases

<<<<<<< HEAD
  TypedThingLayout layout = reader.typedThingLayout();
  uint32_t typeDescr = reader.typeDescrKey();
  uint32_t fieldOffset = int32StubField(reader.stubOffset());
||||||| merged common ancestors
    AutoScratchRegister scratch1(allocator, masm);
    AutoScratchRegister scratch2(allocator, masm);
    AutoScratchRegister niScratch(allocator, masm);
=======
  PropertyIteratorObject* iterobj =
      &objectStubField(reader.stubOffset())->as<PropertyIteratorObject>();
  NativeIterator** enumerators =
      rawWordStubField<NativeIterator**>(reader.stubOffset());
>>>>>>> upstream-releases

<<<<<<< HEAD
  // Get the object's data pointer.
  LoadTypedThingData(masm, layout, obj, scratch1);
||||||| merged common ancestors
    PropertyIteratorObject* iterobj =
        &objectStubField(reader.stubOffset())->as<PropertyIteratorObject>();
    NativeIterator** enumerators = rawWordStubField<NativeIterator**>(reader.stubOffset());
=======
  Register output = allocator.defineRegister(masm, reader.objOperandId());
>>>>>>> upstream-releases

<<<<<<< HEAD
  Address fieldAddr(scratch1, fieldOffset);
  emitLoadTypedObjectResultShared(fieldAddr, scratch2, typeDescr, output);
  return true;
}

bool IonCacheIRCompiler::emitTypeMonitorResult() { return emitReturnFromIC(); }
||||||| merged common ancestors
    Register output = allocator.defineRegister(masm, reader.objOperandId());

    FailurePath* failure;
    if (!addFailurePath(&failure)) {
        return false;
    }
=======
  FailurePath* failure;
  if (!addFailurePath(&failure)) {
    return false;
  }
>>>>>>> upstream-releases

<<<<<<< HEAD
bool IonCacheIRCompiler::emitReturnFromIC() {
  if (!savedLiveRegs_) {
    allocator.restoreInputState(masm);
  }
||||||| merged common ancestors
    // Load our PropertyIteratorObject* and its NativeIterator.
    masm.movePtr(ImmGCPtr(iterobj), output);
    masm.loadObjPrivate(output, JSObject::ITER_CLASS_NFIXED_SLOTS, niScratch);
=======
  // Load our PropertyIteratorObject* and its NativeIterator.
  masm.movePtr(ImmGCPtr(iterobj), output);
  masm.loadObjPrivate(output, PropertyIteratorObject::NUM_FIXED_SLOTS,
                      niScratch);
>>>>>>> upstream-releases

<<<<<<< HEAD
  RepatchLabel rejoin;
  rejoinOffset_ = masm.jumpWithPatch(&rejoin);
  masm.bind(&rejoin);
  return true;
}
||||||| merged common ancestors
    // Ensure the iterator is reusable: see NativeIterator::isReusable.
    masm.branchIfNativeIteratorNotReusable(niScratch, failure->label());
=======
  // Ensure the iterator is reusable: see NativeIterator::isReusable.
  masm.branchIfNativeIteratorNotReusable(niScratch, failure->label());
>>>>>>> upstream-releases

<<<<<<< HEAD
bool IonCacheIRCompiler::emitLoadStackValue() {
  MOZ_ASSERT_UNREACHABLE("emitLoadStackValue not supported for IonCaches.");
  return false;
}
||||||| merged common ancestors
    // Pre-write barrier for store to 'objectBeingIterated_'.
    Address iterObjAddr(niScratch, NativeIterator::offsetOfObjectBeingIterated());
    EmitPreBarrier(masm, iterObjAddr, MIRType::Object);
=======
  // Pre-write barrier for store to 'objectBeingIterated_'.
  Address iterObjAddr(niScratch, NativeIterator::offsetOfObjectBeingIterated());
  EmitPreBarrier(masm, iterObjAddr, MIRType::Object);
>>>>>>> upstream-releases

<<<<<<< HEAD
bool IonCacheIRCompiler::emitGuardAndGetIterator() {
  Register obj = allocator.useRegister(masm, reader.objOperandId());
||||||| merged common ancestors
    // Mark iterator as active.
    Address iterFlagsAddr(niScratch, NativeIterator::offsetOfFlags());
    masm.storePtr(obj, iterObjAddr);
    masm.or32(Imm32(NativeIterator::Flags::Active), iterFlagsAddr);
=======
  // Mark iterator as active.
  Address iterFlagsAddr(niScratch, NativeIterator::offsetOfFlagsAndCount());
  masm.storePtr(obj, iterObjAddr);
  masm.or32(Imm32(NativeIterator::Flags::Active), iterFlagsAddr);
>>>>>>> upstream-releases

<<<<<<< HEAD
  AutoScratchRegister scratch1(allocator, masm);
  AutoScratchRegister scratch2(allocator, masm);
  AutoScratchRegister niScratch(allocator, masm);
||||||| merged common ancestors
    // Post-write barrier for stores to 'objectBeingIterated_'.
    emitPostBarrierSlot(output, TypedOrValueRegister(MIRType::Object, AnyRegister(obj)), scratch1);
=======
  // Post-write barrier for stores to 'objectBeingIterated_'.
  emitPostBarrierSlot(output,
                      TypedOrValueRegister(MIRType::Object, AnyRegister(obj)),
                      scratch1);
>>>>>>> upstream-releases

<<<<<<< HEAD
  PropertyIteratorObject* iterobj =
      &objectStubField(reader.stubOffset())->as<PropertyIteratorObject>();
  NativeIterator** enumerators =
      rawWordStubField<NativeIterator**>(reader.stubOffset());
||||||| merged common ancestors
    // Chain onto the active iterator stack.
    masm.loadPtr(AbsoluteAddress(enumerators), scratch1);
    emitRegisterEnumerator(scratch1, niScratch, scratch2);
=======
  // Chain onto the active iterator stack.
  masm.loadPtr(AbsoluteAddress(enumerators), scratch1);
  emitRegisterEnumerator(scratch1, niScratch, scratch2);
>>>>>>> upstream-releases

<<<<<<< HEAD
  Register output = allocator.defineRegister(masm, reader.objOperandId());
||||||| merged common ancestors
    return true;
}
=======
  return true;
}
>>>>>>> upstream-releases

<<<<<<< HEAD
  FailurePath* failure;
  if (!addFailurePath(&failure)) {
    return false;
  }
||||||| merged common ancestors
bool
IonCacheIRCompiler::emitGuardDOMExpandoMissingOrGuardShape()
{
    ValueOperand val = allocator.useValueRegister(masm, reader.valOperandId());
    Shape* shape = shapeStubField(reader.stubOffset());
=======
bool IonCacheIRCompiler::emitGuardDOMExpandoMissingOrGuardShape() {
  JitSpew(JitSpew_Codegen, __FUNCTION__);
  ValueOperand val = allocator.useValueRegister(masm, reader.valOperandId());
  Shape* shape = shapeStubField(reader.stubOffset());
>>>>>>> upstream-releases

<<<<<<< HEAD
  // Load our PropertyIteratorObject* and its NativeIterator.
  masm.movePtr(ImmGCPtr(iterobj), output);
  masm.loadObjPrivate(output, JSObject::ITER_CLASS_NFIXED_SLOTS, niScratch);
||||||| merged common ancestors
    AutoScratchRegister objScratch(allocator, masm);
=======
  AutoScratchRegister objScratch(allocator, masm);
>>>>>>> upstream-releases

<<<<<<< HEAD
  // Ensure the iterator is reusable: see NativeIterator::isReusable.
  masm.branchIfNativeIteratorNotReusable(niScratch, failure->label());
||||||| merged common ancestors
    FailurePath* failure;
    if (!addFailurePath(&failure)) {
        return false;
    }
=======
  FailurePath* failure;
  if (!addFailurePath(&failure)) {
    return false;
  }
>>>>>>> upstream-releases

<<<<<<< HEAD
  // Pre-write barrier for store to 'objectBeingIterated_'.
  Address iterObjAddr(niScratch, NativeIterator::offsetOfObjectBeingIterated());
  EmitPreBarrier(masm, iterObjAddr, MIRType::Object);
||||||| merged common ancestors
    Label done;
    masm.branchTestUndefined(Assembler::Equal, val, &done);
=======
  Label done;
  masm.branchTestUndefined(Assembler::Equal, val, &done);
>>>>>>> upstream-releases

<<<<<<< HEAD
  // Mark iterator as active.
  Address iterFlagsAddr(niScratch, NativeIterator::offsetOfFlags());
  masm.storePtr(obj, iterObjAddr);
  masm.or32(Imm32(NativeIterator::Flags::Active), iterFlagsAddr);
||||||| merged common ancestors
    masm.debugAssertIsObject(val);
    masm.unboxObject(val, objScratch);
    // The expando object is not used in this case, so we don't need Spectre
    // mitigations.
    masm.branchTestObjShapeNoSpectreMitigations(Assembler::NotEqual, objScratch, shape,
                                                failure->label());
=======
  masm.debugAssertIsObject(val);
  masm.unboxObject(val, objScratch);
  // The expando object is not used in this case, so we don't need Spectre
  // mitigations.
  masm.branchTestObjShapeNoSpectreMitigations(Assembler::NotEqual, objScratch,
                                              shape, failure->label());
>>>>>>> upstream-releases

<<<<<<< HEAD
  // Post-write barrier for stores to 'objectBeingIterated_'.
  emitPostBarrierSlot(output,
                      TypedOrValueRegister(MIRType::Object, AnyRegister(obj)),
                      scratch1);

  // Chain onto the active iterator stack.
  masm.loadPtr(AbsoluteAddress(enumerators), scratch1);
  emitRegisterEnumerator(scratch1, niScratch, scratch2);

  return true;
||||||| merged common ancestors
    masm.bind(&done);
    return true;
=======
  masm.bind(&done);
  return true;
>>>>>>> upstream-releases
}

<<<<<<< HEAD
bool IonCacheIRCompiler::emitGuardDOMExpandoMissingOrGuardShape() {
  ValueOperand val = allocator.useValueRegister(masm, reader.valOperandId());
  Shape* shape = shapeStubField(reader.stubOffset());
||||||| merged common ancestors
bool
IonCacheIRCompiler::emitLoadDOMExpandoValueGuardGeneration()
{
    Register obj = allocator.useRegister(masm, reader.objOperandId());
    ExpandoAndGeneration* expandoAndGeneration =
        rawWordStubField<ExpandoAndGeneration*>(reader.stubOffset());
    uint64_t* generationFieldPtr = expandoGenerationStubFieldPtr(reader.stubOffset());
=======
bool IonCacheIRCompiler::emitLoadDOMExpandoValueGuardGeneration() {
  JitSpew(JitSpew_Codegen, __FUNCTION__);
  Register obj = allocator.useRegister(masm, reader.objOperandId());
  ExpandoAndGeneration* expandoAndGeneration =
      rawWordStubField<ExpandoAndGeneration*>(reader.stubOffset());
  uint64_t* generationFieldPtr =
      expandoGenerationStubFieldPtr(reader.stubOffset());
>>>>>>> upstream-releases

<<<<<<< HEAD
  AutoScratchRegister objScratch(allocator, masm);
||||||| merged common ancestors
    AutoScratchRegister scratch1(allocator, masm);
    AutoScratchRegister scratch2(allocator, masm);
    ValueOperand output = allocator.defineValueRegister(masm, reader.valOperandId());
=======
  AutoScratchRegister scratch1(allocator, masm);
  AutoScratchRegister scratch2(allocator, masm);
  ValueOperand output =
      allocator.defineValueRegister(masm, reader.valOperandId());
>>>>>>> upstream-releases

<<<<<<< HEAD
  FailurePath* failure;
  if (!addFailurePath(&failure)) {
    return false;
  }
||||||| merged common ancestors
    FailurePath* failure;
    if (!addFailurePath(&failure)) {
        return false;
    }
=======
  FailurePath* failure;
  if (!addFailurePath(&failure)) {
    return false;
  }

  masm.loadPtr(Address(obj, ProxyObject::offsetOfReservedSlots()), scratch1);
  Address expandoAddr(scratch1,
                      detail::ProxyReservedSlots::offsetOfPrivateSlot());

  // Guard the ExpandoAndGeneration* matches the proxy's ExpandoAndGeneration.
  masm.loadValue(expandoAddr, output);
  masm.branchTestValue(Assembler::NotEqual, output,
                       PrivateValue(expandoAndGeneration), failure->label());

  // Guard expandoAndGeneration->generation matches the expected generation.
  masm.movePtr(ImmPtr(expandoAndGeneration), output.scratchReg());
  masm.movePtr(ImmPtr(generationFieldPtr), scratch1);
  masm.branch64(
      Assembler::NotEqual,
      Address(output.scratchReg(), ExpandoAndGeneration::offsetOfGeneration()),
      Address(scratch1, 0), scratch2, failure->label());

  // Load expandoAndGeneration->expando into the output Value register.
  masm.loadValue(
      Address(output.scratchReg(), ExpandoAndGeneration::offsetOfExpando()),
      output);
  return true;
}

void IonIC::attachCacheIRStub(JSContext* cx, const CacheIRWriter& writer,
                              CacheKind kind, IonScript* ionScript,
                              bool* attached,
                              const PropertyTypeCheckInfo* typeCheckInfo) {
  // We shouldn't GC or report OOM (or any other exception) here.
  AutoAssertNoPendingException aanpe(cx);
  JS::AutoCheckCannotGC nogc;

  MOZ_ASSERT(!*attached);

  // SetProp/SetElem stubs must have non-null typeCheckInfo.
  MOZ_ASSERT(!!typeCheckInfo ==
             (kind == CacheKind::SetProp || kind == CacheKind::SetElem));

  // Do nothing if the IR generator failed or triggered a GC that invalidated
  // the script.
  if (writer.failed() || ionScript->invalidated()) {
    return;
  }

  JitZone* jitZone = cx->zone()->jitZone();
  uint32_t stubDataOffset = sizeof(IonICStub);

  // Try to reuse a previously-allocated CacheIRStubInfo.
  CacheIRStubKey::Lookup lookup(kind, ICStubEngine::IonIC, writer.codeStart(),
                                writer.codeLength());
  CacheIRStubInfo* stubInfo = jitZone->getIonCacheIRStubInfo(lookup);
  if (!stubInfo) {
    // Allocate the shared CacheIRStubInfo. Note that the
    // putIonCacheIRStubInfo call below will transfer ownership to
    // the stub info HashSet, so we don't have to worry about freeing
    // it below.

    // For Ion ICs, we don't track/use the makesGCCalls flag, so just pass true.
    bool makesGCCalls = true;
    stubInfo = CacheIRStubInfo::New(kind, ICStubEngine::IonIC, makesGCCalls,
                                    stubDataOffset, writer);
    if (!stubInfo) {
      return;
    }
>>>>>>> upstream-releases

<<<<<<< HEAD
  Label done;
  masm.branchTestUndefined(Assembler::Equal, val, &done);
||||||| merged common ancestors
    masm.loadPtr(Address(obj, ProxyObject::offsetOfReservedSlots()), scratch1);
    Address expandoAddr(scratch1, detail::ProxyReservedSlots::offsetOfPrivateSlot());
=======
    CacheIRStubKey key(stubInfo);
    if (!jitZone->putIonCacheIRStubInfo(lookup, key)) {
      return;
    }
  }
>>>>>>> upstream-releases

<<<<<<< HEAD
  masm.debugAssertIsObject(val);
  masm.unboxObject(val, objScratch);
  // The expando object is not used in this case, so we don't need Spectre
  // mitigations.
  masm.branchTestObjShapeNoSpectreMitigations(Assembler::NotEqual, objScratch,
                                              shape, failure->label());
||||||| merged common ancestors
    // Guard the ExpandoAndGeneration* matches the proxy's ExpandoAndGeneration.
    masm.loadValue(expandoAddr, output);
    masm.branchTestValue(Assembler::NotEqual, output, PrivateValue(expandoAndGeneration),
                         failure->label());
=======
  MOZ_ASSERT(stubInfo);
>>>>>>> upstream-releases

<<<<<<< HEAD
  masm.bind(&done);
  return true;
}
||||||| merged common ancestors
    // Guard expandoAndGeneration->generation matches the expected generation.
    masm.movePtr(ImmPtr(expandoAndGeneration), output.scratchReg());
    masm.movePtr(ImmPtr(generationFieldPtr), scratch1);
    masm.branch64(Assembler::NotEqual,
                  Address(output.scratchReg(), ExpandoAndGeneration::offsetOfGeneration()),
                  Address(scratch1, 0),
                  scratch2,
                  failure->label());

    // Load expandoAndGeneration->expando into the output Value register.
    masm.loadValue(Address(output.scratchReg(), ExpandoAndGeneration::offsetOfExpando()), output);
    return true;
}
=======
  // Ensure we don't attach duplicate stubs. This can happen if a stub failed
  // for some reason and the IR generator doesn't check for exactly the same
  // conditions.
  for (IonICStub* stub = firstStub_; stub; stub = stub->next()) {
    if (stub->stubInfo() != stubInfo) {
      continue;
    }
    bool updated = false;
    if (!writer.stubDataEqualsMaybeUpdate(stub->stubDataStart(), &updated)) {
      continue;
    }
    if (updated || (typeCheckInfo && typeCheckInfo->needsTypeBarrier())) {
      // We updated a stub or have a stub that requires property type
      // checks. In this case the stub will likely handle more cases in
      // the future and we shouldn't deoptimize.
      *attached = true;
    }
    return;
  }
>>>>>>> upstream-releases

<<<<<<< HEAD
bool IonCacheIRCompiler::emitLoadDOMExpandoValueGuardGeneration() {
  Register obj = allocator.useRegister(masm, reader.objOperandId());
  ExpandoAndGeneration* expandoAndGeneration =
      rawWordStubField<ExpandoAndGeneration*>(reader.stubOffset());
  uint64_t* generationFieldPtr =
      expandoGenerationStubFieldPtr(reader.stubOffset());
||||||| merged common ancestors
void
IonIC::attachCacheIRStub(JSContext* cx, const CacheIRWriter& writer, CacheKind kind,
                         IonScript* ionScript, bool* attached,
                         const PropertyTypeCheckInfo* typeCheckInfo)
{
    // We shouldn't GC or report OOM (or any other exception) here.
    AutoAssertNoPendingException aanpe(cx);
    JS::AutoCheckCannotGC nogc;
=======
  size_t bytesNeeded = stubInfo->stubDataOffset() + stubInfo->stubDataSize();
>>>>>>> upstream-releases

<<<<<<< HEAD
  AutoScratchRegister scratch1(allocator, masm);
  AutoScratchRegister scratch2(allocator, masm);
  ValueOperand output =
      allocator.defineValueRegister(masm, reader.valOperandId());
||||||| merged common ancestors
    MOZ_ASSERT(!*attached);
=======
  // Allocate the IonICStub in the optimized stub space. Ion stubs and
  // CacheIRStubInfo instances for Ion stubs can be purged on GC. That's okay
  // because the stub code is rooted separately when we make a VM call, and
  // stub code should never access the IonICStub after making a VM call. The
  // IonICStub::poison method poisons the stub to catch bugs in this area.
  ICStubSpace* stubSpace = cx->zone()->jitZone()->optimizedStubSpace();
  void* newStubMem = stubSpace->alloc(bytesNeeded);
  if (!newStubMem) {
    return;
  }
>>>>>>> upstream-releases

<<<<<<< HEAD
  FailurePath* failure;
  if (!addFailurePath(&failure)) {
    return false;
  }

  masm.loadPtr(Address(obj, ProxyObject::offsetOfReservedSlots()), scratch1);
  Address expandoAddr(scratch1,
                      detail::ProxyReservedSlots::offsetOfPrivateSlot());

  // Guard the ExpandoAndGeneration* matches the proxy's ExpandoAndGeneration.
  masm.loadValue(expandoAddr, output);
  masm.branchTestValue(Assembler::NotEqual, output,
                       PrivateValue(expandoAndGeneration), failure->label());

  // Guard expandoAndGeneration->generation matches the expected generation.
  masm.movePtr(ImmPtr(expandoAndGeneration), output.scratchReg());
  masm.movePtr(ImmPtr(generationFieldPtr), scratch1);
  masm.branch64(
      Assembler::NotEqual,
      Address(output.scratchReg(), ExpandoAndGeneration::offsetOfGeneration()),
      Address(scratch1, 0), scratch2, failure->label());

  // Load expandoAndGeneration->expando into the output Value register.
  masm.loadValue(
      Address(output.scratchReg(), ExpandoAndGeneration::offsetOfExpando()),
      output);
  return true;
}

void IonIC::attachCacheIRStub(JSContext* cx, const CacheIRWriter& writer,
                              CacheKind kind, IonScript* ionScript,
                              bool* attached,
                              const PropertyTypeCheckInfo* typeCheckInfo) {
  // We shouldn't GC or report OOM (or any other exception) here.
  AutoAssertNoPendingException aanpe(cx);
  JS::AutoCheckCannotGC nogc;

  MOZ_ASSERT(!*attached);

  // SetProp/SetElem stubs must have non-null typeCheckInfo.
  MOZ_ASSERT(!!typeCheckInfo ==
             (kind == CacheKind::SetProp || kind == CacheKind::SetElem));

  // Do nothing if the IR generator failed or triggered a GC that invalidated
  // the script.
  if (writer.failed() || ionScript->invalidated()) {
    return;
  }

  JitZone* jitZone = cx->zone()->jitZone();
  uint32_t stubDataOffset = sizeof(IonICStub);

  // Try to reuse a previously-allocated CacheIRStubInfo.
  CacheIRStubKey::Lookup lookup(kind, ICStubEngine::IonIC, writer.codeStart(),
                                writer.codeLength());
  CacheIRStubInfo* stubInfo = jitZone->getIonCacheIRStubInfo(lookup);
  if (!stubInfo) {
    // Allocate the shared CacheIRStubInfo. Note that the
    // putIonCacheIRStubInfo call below will transfer ownership to
    // the stub info HashSet, so we don't have to worry about freeing
    // it below.

    // For Ion ICs, we don't track/use the makesGCCalls flag, so just pass true.
    bool makesGCCalls = true;
    stubInfo = CacheIRStubInfo::New(kind, ICStubEngine::IonIC, makesGCCalls,
                                    stubDataOffset, writer);
    if (!stubInfo) {
      return;
    }
||||||| merged common ancestors
    // SetProp/SetElem stubs must have non-null typeCheckInfo.
    MOZ_ASSERT(!!typeCheckInfo == (kind == CacheKind::SetProp || kind == CacheKind::SetElem));
=======
  IonICStub* newStub =
      new (newStubMem) IonICStub(fallbackLabel_.raw(), stubInfo);
  writer.copyStubData(newStub->stubDataStart());
>>>>>>> upstream-releases

<<<<<<< HEAD
    CacheIRStubKey key(stubInfo);
    if (!jitZone->putIonCacheIRStubInfo(lookup, key)) {
      return;
    }
  }
||||||| merged common ancestors
    // Do nothing if the IR generator failed or triggered a GC that invalidated
    // the script.
    if (writer.failed() || ionScript->invalidated()) {
        return;
    }
=======
  JitContext jctx(cx, nullptr);
  IonCacheIRCompiler compiler(cx, writer, this, ionScript, newStub,
                              typeCheckInfo, stubDataOffset);
  if (!compiler.init()) {
    return;
  }
>>>>>>> upstream-releases

<<<<<<< HEAD
  MOZ_ASSERT(stubInfo);
||||||| merged common ancestors
    JitZone* jitZone = cx->zone()->jitZone();
    uint32_t stubDataOffset = sizeof(IonICStub);
=======
  JitCode* code = compiler.compile();
  if (!code) {
    return;
  }
>>>>>>> upstream-releases

<<<<<<< HEAD
  // Ensure we don't attach duplicate stubs. This can happen if a stub failed
  // for some reason and the IR generator doesn't check for exactly the same
  // conditions.
  for (IonICStub* stub = firstStub_; stub; stub = stub->next()) {
    if (stub->stubInfo() != stubInfo) {
      continue;
    }
    bool updated = false;
    if (!writer.stubDataEqualsMaybeUpdate(stub->stubDataStart(), &updated)) {
      continue;
    }
    if (updated || (typeCheckInfo && typeCheckInfo->needsTypeBarrier())) {
      // We updated a stub or have a stub that requires property type
      // checks. In this case the stub will likely handle more cases in
      // the future and we shouldn't deoptimize.
      *attached = true;
    }
    return;
  }
||||||| merged common ancestors
    // Try to reuse a previously-allocated CacheIRStubInfo.
    CacheIRStubKey::Lookup lookup(kind, ICStubEngine::IonIC,
                                  writer.codeStart(), writer.codeLength());
    CacheIRStubInfo* stubInfo = jitZone->getIonCacheIRStubInfo(lookup);
    if (!stubInfo) {
        // Allocate the shared CacheIRStubInfo. Note that the
        // putIonCacheIRStubInfo call below will transfer ownership to
        // the stub info HashSet, so we don't have to worry about freeing
        // it below.

        // For Ion ICs, we don't track/use the makesGCCalls flag, so just pass true.
        bool makesGCCalls = true;
        stubInfo = CacheIRStubInfo::New(kind, ICStubEngine::IonIC, makesGCCalls,
                                        stubDataOffset, writer);
        if (!stubInfo) {
            return;
        }

        CacheIRStubKey key(stubInfo);
        if (!jitZone->putIonCacheIRStubInfo(lookup, key)) {
            return;
        }
    }

    MOZ_ASSERT(stubInfo);

    // Ensure we don't attach duplicate stubs. This can happen if a stub failed
    // for some reason and the IR generator doesn't check for exactly the same
    // conditions.
    for (IonICStub* stub = firstStub_; stub; stub = stub->next()) {
        if (stub->stubInfo() != stubInfo) {
            continue;
        }
        bool updated = false;
        if (!writer.stubDataEqualsMaybeUpdate(stub->stubDataStart(), &updated)) {
            continue;
        }
        if (updated || (typeCheckInfo && typeCheckInfo->needsTypeBarrier())) {
            // We updated a stub or have a stub that requires property type
            // checks. In this case the stub will likely handle more cases in
            // the future and we shouldn't deoptimize.
            *attached = true;
        }
        return;
    }

    size_t bytesNeeded = stubInfo->stubDataOffset() + stubInfo->stubDataSize();

    // Allocate the IonICStub in the optimized stub space. Ion stubs and
    // CacheIRStubInfo instances for Ion stubs can be purged on GC. That's okay
    // because the stub code is rooted separately when we make a VM call, and
    // stub code should never access the IonICStub after making a VM call. The
    // IonICStub::poison method poisons the stub to catch bugs in this area.
    ICStubSpace* stubSpace = cx->zone()->jitZone()->optimizedStubSpace();
    void* newStubMem = stubSpace->alloc(bytesNeeded);
    if (!newStubMem) {
        return;
    }
=======
  attachStub(newStub, code);
  *attached = true;
}

bool IonCacheIRCompiler::emitCallStringConcatResult() {
  JitSpew(JitSpew_Codegen, __FUNCTION__);
  AutoSaveLiveRegisters save(*this);
  AutoOutputRegister output(*this);

  Register lhs = allocator.useRegister(masm, reader.stringOperandId());
  Register rhs = allocator.useRegister(masm, reader.stringOperandId());
>>>>>>> upstream-releases

<<<<<<< HEAD
  size_t bytesNeeded = stubInfo->stubDataOffset() + stubInfo->stubDataSize();
||||||| merged common ancestors
    IonICStub* newStub = new(newStubMem) IonICStub(fallbackLabel_.raw(), stubInfo);
    writer.copyStubData(newStub->stubDataStart());
=======
  allocator.discardStack(masm);
>>>>>>> upstream-releases

<<<<<<< HEAD
  // Allocate the IonICStub in the optimized stub space. Ion stubs and
  // CacheIRStubInfo instances for Ion stubs can be purged on GC. That's okay
  // because the stub code is rooted separately when we make a VM call, and
  // stub code should never access the IonICStub after making a VM call. The
  // IonICStub::poison method poisons the stub to catch bugs in this area.
  ICStubSpace* stubSpace = cx->zone()->jitZone()->optimizedStubSpace();
  void* newStubMem = stubSpace->alloc(bytesNeeded);
  if (!newStubMem) {
    return;
  }
||||||| merged common ancestors
    JitContext jctx(cx, nullptr);
    IonCacheIRCompiler compiler(cx, writer, this, ionScript, newStub, typeCheckInfo, stubDataOffset);
    if (!compiler.init()) {
        return;
    }
=======
  prepareVMCall(masm, save);
>>>>>>> upstream-releases

<<<<<<< HEAD
  IonICStub* newStub =
      new (newStubMem) IonICStub(fallbackLabel_.raw(), stubInfo);
  writer.copyStubData(newStub->stubDataStart());

  JitContext jctx(cx, nullptr);
  IonCacheIRCompiler compiler(cx, writer, this, ionScript, newStub,
                              typeCheckInfo, stubDataOffset);
  if (!compiler.init()) {
    return;
  }
||||||| merged common ancestors
    JitCode* code = compiler.compile();
    if (!code) {
        return;
    }
=======
  masm.Push(rhs);
  masm.Push(lhs);
>>>>>>> upstream-releases

<<<<<<< HEAD
  JitCode* code = compiler.compile();
  if (!code) {
    return;
  }

  attachStub(newStub, code);
  *attached = true;
}
||||||| merged common ancestors
    attachStub(newStub, code);
    *attached = true;
}
=======
  using Fn = JSString* (*)(JSContext*, HandleString, HandleString);
  callVM<Fn, ConcatStrings<CanGC>>(masm);
>>>>>>> upstream-releases

<<<<<<< HEAD
bool IonCacheIRCompiler::emitCallStringConcatResult() {
  AutoSaveLiveRegisters save(*this);
  AutoOutputRegister output(*this);
||||||| merged common ancestors
bool
IonCacheIRCompiler::emitCallStringConcatResult()
{
    AutoSaveLiveRegisters save(*this);
    AutoOutputRegister output(*this);
=======
  masm.tagValue(JSVAL_TYPE_STRING, ReturnReg, output.valueReg());
  return true;
}
>>>>>>> upstream-releases

<<<<<<< HEAD
  Register lhs = allocator.useRegister(masm, reader.stringOperandId());
  Register rhs = allocator.useRegister(masm, reader.stringOperandId());
||||||| merged common ancestors
    Register lhs = allocator.useRegister(masm, reader.stringOperandId());
    Register rhs = allocator.useRegister(masm, reader.stringOperandId());
=======
bool IonCacheIRCompiler::emitCallStringObjectConcatResult() {
  JitSpew(JitSpew_Codegen, __FUNCTION__);
  AutoSaveLiveRegisters save(*this);
  AutoOutputRegister output(*this);
>>>>>>> upstream-releases

<<<<<<< HEAD
  allocator.discardStack(masm);
||||||| merged common ancestors
    allocator.discardStack(masm);
=======
  ValueOperand lhs = allocator.useValueRegister(masm, reader.valOperandId());
  ValueOperand rhs = allocator.useValueRegister(masm, reader.valOperandId());
>>>>>>> upstream-releases

<<<<<<< HEAD
  prepareVMCall(masm, save);
||||||| merged common ancestors
    prepareVMCall(masm, save);
=======
  allocator.discardStack(masm);
>>>>>>> upstream-releases

<<<<<<< HEAD
  masm.Push(rhs);
  masm.Push(lhs);
||||||| merged common ancestors
    masm.Push(rhs);
    masm.Push(lhs);
=======
  prepareVMCall(masm, save);
  masm.Push(rhs);
  masm.Push(lhs);
>>>>>>> upstream-releases

<<<<<<< HEAD
  if (!callVM(masm, ConcatStringsInfo)) {
    return false;
  }
||||||| merged common ancestors
    if (!callVM(masm, ConcatStringsInfo)) {
        return false;
    }
=======
  using Fn = bool (*)(JSContext*, HandleValue, HandleValue, MutableHandleValue);
  callVM<Fn, DoConcatStringObject>(masm);
>>>>>>> upstream-releases

<<<<<<< HEAD
  masm.tagValue(JSVAL_TYPE_STRING, ReturnReg, output.valueReg());
  return true;
||||||| merged common ancestors
    masm.tagValue(JSVAL_TYPE_STRING, ReturnReg, output.valueReg());
    return true;
=======
  masm.storeCallResultValue(output);
  return true;
>>>>>>> upstream-releases
}

<<<<<<< HEAD
typedef bool (*DoConcatStringObjectFn)(JSContext*, HandleValue, HandleValue,
                                       MutableHandleValue);
const VMFunction DoIonConcatStringObjectInfo =
    FunctionInfo<DoConcatStringObjectFn>(DoConcatStringObject,
                                         "DoIonConcatStringObject");

bool IonCacheIRCompiler::emitCallStringObjectConcatResult() {
  AutoSaveLiveRegisters save(*this);
  AutoOutputRegister output(*this);
||||||| merged common ancestors
typedef bool (*DoConcatStringObjectFn)(JSContext*, HandleValue, HandleValue,
                                       MutableHandleValue);
const VMFunction DoIonConcatStringObjectInfo =
    FunctionInfo<DoConcatStringObjectFn>(DoConcatStringObject, "DoIonConcatStringObject");

bool
IonCacheIRCompiler::emitCallStringObjectConcatResult()
{
    AutoSaveLiveRegisters save(*this);
    AutoOutputRegister output(*this);
=======
bool IonCacheIRCompiler::emitCallScriptedFunction() {
  MOZ_CRASH("Call ICs not used in ion");
}
>>>>>>> upstream-releases

<<<<<<< HEAD
  ValueOperand lhs = allocator.useValueRegister(masm, reader.valOperandId());
  ValueOperand rhs = allocator.useValueRegister(masm, reader.valOperandId());
||||||| merged common ancestors
    ValueOperand lhs = allocator.useValueRegister(masm, reader.valOperandId());
    ValueOperand rhs = allocator.useValueRegister(masm, reader.valOperandId());
=======
bool IonCacheIRCompiler::emitCallNativeFunction() {
  MOZ_CRASH("Call ICs not used in ion");
}
>>>>>>> upstream-releases

<<<<<<< HEAD
  allocator.discardStack(masm);
||||||| merged common ancestors
    allocator.discardStack(masm);
=======
bool IonCacheIRCompiler::emitCallClassHook() {
  MOZ_CRASH("Call ICs not used in ion");
}
>>>>>>> upstream-releases

<<<<<<< HEAD
  prepareVMCall(masm, save);
  masm.Push(rhs);
  masm.Push(lhs);
||||||| merged common ancestors
    prepareVMCall(masm, save);
    masm.Push(rhs);
    masm.Push(lhs);
=======
bool IonCacheIRCompiler::emitLoadArgumentFixedSlot() {
  MOZ_CRASH("Call ICs not used in ion");
}
>>>>>>> upstream-releases

<<<<<<< HEAD
  if (!callVM(masm, DoIonConcatStringObjectInfo)) {
    return false;
  }
||||||| merged common ancestors
    if (!callVM(masm, DoIonConcatStringObjectInfo)) {
        return false;
    }
=======
bool IonCacheIRCompiler::emitLoadArgumentDynamicSlot() {
  MOZ_CRASH("Call ICs not used in ion");
}
>>>>>>> upstream-releases

<<<<<<< HEAD
  masm.storeCallResultValue(output);
  return true;
||||||| merged common ancestors
    masm.storeCallResultValue(output);
    return true;
=======
bool IonCacheIRCompiler::emitGuardFunApply() {
  MOZ_CRASH("Call ICs not used in ion");
>>>>>>> upstream-releases
}
