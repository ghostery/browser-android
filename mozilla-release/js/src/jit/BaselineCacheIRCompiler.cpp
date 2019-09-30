/* -*- Mode: C++; tab-width: 8; indent-tabs-mode: nil; c-basic-offset: 2 -*-
 * vim: set ts=8 sts=2 et sw=2 tw=80:
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/. */

#include "jit/BaselineCacheIRCompiler.h"

#include "jit/CacheIR.h"
#include "jit/Linker.h"
#include "jit/SharedICHelpers.h"
#include "jit/VMFunctions.h"
#include "proxy/DeadObjectProxy.h"
#include "proxy/Proxy.h"

#include "jit/MacroAssembler-inl.h"
#include "jit/SharedICHelpers-inl.h"
#include "jit/VMFunctionList-inl.h"
#include "vm/JSContext-inl.h"
#include "vm/Realm-inl.h"

using namespace js;
using namespace js::jit;

using mozilla::Maybe;

namespace js {
namespace jit {

class AutoStubFrame;

<<<<<<< HEAD
Address CacheRegisterAllocator::addressOf(MacroAssembler& masm,
                                          BaselineFrameSlot slot) const {
  uint32_t offset =
      stackPushed_ + ICStackValueOffset + slot.slot() * sizeof(JS::Value);
  return Address(masm.getStackPointer(), offset);
||||||| merged common ancestors
Address
CacheRegisterAllocator::addressOf(MacroAssembler& masm, BaselineFrameSlot slot) const
{
    uint32_t offset = stackPushed_ + ICStackValueOffset + slot.slot() * sizeof(JS::Value);
    return Address(masm.getStackPointer(), offset);
=======
Address CacheRegisterAllocator::addressOf(MacroAssembler& masm,
                                          BaselineFrameSlot slot) const {
  uint32_t offset =
      stackPushed_ + ICStackValueOffset + slot.slot() * sizeof(JS::Value);
  return Address(masm.getStackPointer(), offset);
}
BaseValueIndex CacheRegisterAllocator::addressOf(MacroAssembler& masm,
                                                 Register argcReg,
                                                 BaselineFrameSlot slot) const {
  uint32_t offset =
      stackPushed_ + ICStackValueOffset + slot.slot() * sizeof(JS::Value);
  return BaseValueIndex(masm.getStackPointer(), argcReg, offset);
>>>>>>> upstream-releases
}

// BaselineCacheIRCompiler compiles CacheIR to BaselineIC native code.
<<<<<<< HEAD
class MOZ_RAII BaselineCacheIRCompiler : public CacheIRCompiler {
  bool inStubFrame_;
  bool makesGCCalls_;
  BaselineCacheIRStubKind kind_;

  MOZ_MUST_USE bool callVM(MacroAssembler& masm, const VMFunction& fun);
  MOZ_MUST_USE bool tailCallVM(MacroAssembler& masm, const VMFunction& fun);

  MOZ_MUST_USE bool callTypeUpdateIC(Register obj, ValueOperand val,
                                     Register scratch,
                                     LiveGeneralRegisterSet saveRegs);

  MOZ_MUST_USE bool emitStoreSlotShared(bool isFixed);
  MOZ_MUST_USE bool emitAddAndStoreSlotShared(CacheOp op);

 public:
  friend class AutoStubFrame;

  BaselineCacheIRCompiler(JSContext* cx, const CacheIRWriter& writer,
                          uint32_t stubDataOffset,
                          BaselineCacheIRStubKind stubKind)
      : CacheIRCompiler(cx, writer, stubDataOffset, Mode::Baseline,
                        StubFieldPolicy::Address),
        inStubFrame_(false),
        makesGCCalls_(false),
        kind_(stubKind) {}

  MOZ_MUST_USE bool init(CacheKind kind);

  JitCode* compile();

  bool makesGCCalls() const { return makesGCCalls_; }

 private:
#define DEFINE_OP(op) MOZ_MUST_USE bool emit##op();
  CACHE_IR_OPS(DEFINE_OP)
#undef DEFINE_OP

  Address stubAddress(uint32_t offset) const {
    return Address(ICStubReg, stubDataOffset_ + offset);
  }
};

#define DEFINE_SHARED_OP(op)                 \
  bool BaselineCacheIRCompiler::emit##op() { \
    return CacheIRCompiler::emit##op();      \
  }
CACHE_IR_SHARED_OPS(DEFINE_SHARED_OP)
||||||| merged common ancestors
class MOZ_RAII BaselineCacheIRCompiler : public CacheIRCompiler
{

    bool inStubFrame_;
    bool makesGCCalls_;

    MOZ_MUST_USE bool callVM(MacroAssembler& masm, const VMFunction& fun);
    MOZ_MUST_USE bool tailCallVM(MacroAssembler& masm, const VMFunction& fun);

    MOZ_MUST_USE bool callTypeUpdateIC(Register obj, ValueOperand val, Register scratch,
                                       LiveGeneralRegisterSet saveRegs);

    MOZ_MUST_USE bool emitStoreSlotShared(bool isFixed);
    MOZ_MUST_USE bool emitAddAndStoreSlotShared(CacheOp op);

  public:
    friend class AutoStubFrame;

    BaselineCacheIRCompiler(JSContext* cx, const CacheIRWriter& writer,
                            uint32_t stubDataOffset)
      : CacheIRCompiler(cx, writer, stubDataOffset, Mode::Baseline, StubFieldPolicy::Address),
        inStubFrame_(false),
        makesGCCalls_(false)
    {}

    MOZ_MUST_USE bool init(CacheKind kind);

    JitCode* compile();

    bool makesGCCalls() const { return makesGCCalls_; }

  private:
#define DEFINE_OP(op) MOZ_MUST_USE bool emit##op();
    CACHE_IR_OPS(DEFINE_OP)
#undef DEFINE_OP

    Address stubAddress(uint32_t offset) const {
        return Address(ICStubReg, stubDataOffset_ + offset);
    }
};

#define DEFINE_SHARED_OP(op) \
    bool BaselineCacheIRCompiler::emit##op() { return CacheIRCompiler::emit##op(); }
    CACHE_IR_SHARED_OPS(DEFINE_SHARED_OP)
=======
BaselineCacheIRCompiler::BaselineCacheIRCompiler(
    JSContext* cx, const CacheIRWriter& writer, uint32_t stubDataOffset,
    BaselineCacheIRStubKind stubKind)
    : CacheIRCompiler(cx, writer, stubDataOffset, Mode::Baseline,
                      StubFieldPolicy::Address),
      makesGCCalls_(false),
      kind_(stubKind) {}

#define DEFINE_SHARED_OP(op)                 \
  bool BaselineCacheIRCompiler::emit##op() { \
    return CacheIRCompiler::emit##op();      \
  }
CACHE_IR_SHARED_OPS(DEFINE_SHARED_OP)
>>>>>>> upstream-releases
#undef DEFINE_SHARED_OP

<<<<<<< HEAD
enum class CallCanGC { CanGC, CanNotGC };

// Instructions that have to perform a callVM require a stub frame. Call its
// enter() and leave() methods to enter/leave the stub frame.
class MOZ_RAII AutoStubFrame {
  BaselineCacheIRCompiler& compiler;
||||||| merged common ancestors
enum class CallCanGC { CanGC, CanNotGC };

// Instructions that have to perform a callVM require a stub frame. Call its
// enter() and leave() methods to enter/leave the stub frame.
class MOZ_RAII AutoStubFrame
{
    BaselineCacheIRCompiler& compiler;
=======
// AutoStubFrame methods
AutoStubFrame::AutoStubFrame(BaselineCacheIRCompiler& compiler)
    : compiler(compiler)
>>>>>>> upstream-releases
#ifdef DEBUG
<<<<<<< HEAD
  uint32_t framePushedAtEnterStubFrame_;
||||||| merged common ancestors
    uint32_t framePushedAtEnterStubFrame_;
=======
      ,
      framePushedAtEnterStubFrame_(0)
>>>>>>> upstream-releases
#endif
{
}
void AutoStubFrame::enter(MacroAssembler& masm, Register scratch,
                          CallCanGC canGC) {
  MOZ_ASSERT(compiler.allocator.stackPushed() == 0);

<<<<<<< HEAD
  AutoStubFrame(const AutoStubFrame&) = delete;
  void operator=(const AutoStubFrame&) = delete;
||||||| merged common ancestors
    AutoStubFrame(const AutoStubFrame&) = delete;
    void operator=(const AutoStubFrame&) = delete;
=======
  EmitBaselineEnterStubFrame(masm, scratch);
>>>>>>> upstream-releases

<<<<<<< HEAD
 public:
  explicit AutoStubFrame(BaselineCacheIRCompiler& compiler)
      : compiler(compiler)
||||||| merged common ancestors
  public:
    explicit AutoStubFrame(BaselineCacheIRCompiler& compiler)
      : compiler(compiler)
=======
>>>>>>> upstream-releases
#ifdef DEBUG
<<<<<<< HEAD
        ,
        framePushedAtEnterStubFrame_(0)
||||||| merged common ancestors
        , framePushedAtEnterStubFrame_(0)
=======
  framePushedAtEnterStubFrame_ = masm.framePushed();
>>>>>>> upstream-releases
#endif
<<<<<<< HEAD
  {
  }

  void enter(MacroAssembler& masm, Register scratch,
             CallCanGC canGC = CallCanGC::CanGC) {
    MOZ_ASSERT(compiler.allocator.stackPushed() == 0);
||||||| merged common ancestors
    { }

    void enter(MacroAssembler& masm, Register scratch, CallCanGC canGC = CallCanGC::CanGC) {
        MOZ_ASSERT(compiler.allocator.stackPushed() == 0);
=======
>>>>>>> upstream-releases

<<<<<<< HEAD
    EmitBaselineEnterStubFrame(masm, scratch);
||||||| merged common ancestors
        EmitBaselineEnterStubFrame(masm, scratch);
=======
  MOZ_ASSERT(!compiler.preparedForVMCall_);
  compiler.preparedForVMCall_ = true;
  if (canGC == CallCanGC::CanGC) {
    compiler.makesGCCalls_ = true;
  }
}
void AutoStubFrame::leave(MacroAssembler& masm, bool calledIntoIon) {
  MOZ_ASSERT(compiler.preparedForVMCall_);
  compiler.preparedForVMCall_ = false;
>>>>>>> upstream-releases

#ifdef DEBUG
<<<<<<< HEAD
    framePushedAtEnterStubFrame_ = masm.framePushed();
||||||| merged common ancestors
        framePushedAtEnterStubFrame_ = masm.framePushed();
=======
  masm.setFramePushed(framePushedAtEnterStubFrame_);
  if (calledIntoIon) {
    masm.adjustFrame(sizeof(intptr_t));  // Calls into ion have this extra.
  }
>>>>>>> upstream-releases
#endif

<<<<<<< HEAD
    MOZ_ASSERT(!compiler.inStubFrame_);
    compiler.inStubFrame_ = true;
    if (canGC == CallCanGC::CanGC) {
      compiler.makesGCCalls_ = true;
    }
  }
  void leave(MacroAssembler& masm, bool calledIntoIon = false) {
    MOZ_ASSERT(compiler.inStubFrame_);
    compiler.inStubFrame_ = false;
||||||| merged common ancestors
        MOZ_ASSERT(!compiler.inStubFrame_);
        compiler.inStubFrame_ = true;
        if (canGC == CallCanGC::CanGC) {
            compiler.makesGCCalls_ = true;
        }
    }
    void leave(MacroAssembler& masm, bool calledIntoIon = false) {
        MOZ_ASSERT(compiler.inStubFrame_);
        compiler.inStubFrame_ = false;
=======
  EmitBaselineLeaveStubFrame(masm, calledIntoIon);
}
>>>>>>> upstream-releases

#ifdef DEBUG
<<<<<<< HEAD
    masm.setFramePushed(framePushedAtEnterStubFrame_);
    if (calledIntoIon) {
      masm.adjustFrame(sizeof(intptr_t));  // Calls into ion have this extra.
    }
||||||| merged common ancestors
        masm.setFramePushed(framePushedAtEnterStubFrame_);
        if (calledIntoIon) {
            masm.adjustFrame(sizeof(intptr_t)); // Calls into ion have this extra.
        }
=======
AutoStubFrame::~AutoStubFrame() { MOZ_ASSERT(!compiler.preparedForVMCall_); }
>>>>>>> upstream-releases
#endif

<<<<<<< HEAD
    EmitBaselineLeaveStubFrame(masm, calledIntoIon);
  }
||||||| merged common ancestors
        EmitBaselineLeaveStubFrame(masm, calledIntoIon);
    }
=======
}  // namespace jit
}  // namespace js
>>>>>>> upstream-releases

<<<<<<< HEAD
#ifdef DEBUG
  ~AutoStubFrame() { MOZ_ASSERT(!compiler.inStubFrame_); }
#endif
};
||||||| merged common ancestors
#ifdef DEBUG
    ~AutoStubFrame() {
        MOZ_ASSERT(!compiler.inStubFrame_);
    }
#endif
};
=======
bool BaselineCacheIRCompiler::makesGCCalls() const { return makesGCCalls_; }
>>>>>>> upstream-releases

<<<<<<< HEAD
bool BaselineCacheIRCompiler::callVM(MacroAssembler& masm,
                                     const VMFunction& fun) {
  MOZ_ASSERT(inStubFrame_);
||||||| merged common ancestors
bool
BaselineCacheIRCompiler::callVM(MacroAssembler& masm, const VMFunction& fun)
{
    MOZ_ASSERT(inStubFrame_);
=======
Address BaselineCacheIRCompiler::stubAddress(uint32_t offset) const {
  return Address(ICStubReg, stubDataOffset_ + offset);
}
>>>>>>> upstream-releases

<<<<<<< HEAD
  TrampolinePtr code = cx_->runtime()->jitRuntime()->getVMWrapper(fun);
  MOZ_ASSERT(fun.expectTailCall == NonTailCall);
||||||| merged common ancestors
    TrampolinePtr code = cx_->runtime()->jitRuntime()->getVMWrapper(fun);
    MOZ_ASSERT(fun.expectTailCall == NonTailCall);
=======
template <typename Fn, Fn fn>
void BaselineCacheIRCompiler::callVM(MacroAssembler& masm) {
  VMFunctionId id = VMFunctionToId<Fn, fn>::id;
  callVMInternal(masm, id);
}
>>>>>>> upstream-releases

<<<<<<< HEAD
  EmitBaselineCallVM(code, masm);
  return true;
||||||| merged common ancestors
    EmitBaselineCallVM(code, masm);
    return true;
=======
template <typename Fn, Fn fn>
void BaselineCacheIRCompiler::tailCallVM(MacroAssembler& masm) {
  TailCallVMFunctionId id = TailCallVMFunctionToId<Fn, fn>::id;
  tailCallVMInternal(masm, id);
>>>>>>> upstream-releases
}

<<<<<<< HEAD
bool BaselineCacheIRCompiler::tailCallVM(MacroAssembler& masm,
                                         const VMFunction& fun) {
  MOZ_ASSERT(!inStubFrame_);
||||||| merged common ancestors
bool
BaselineCacheIRCompiler::tailCallVM(MacroAssembler& masm, const VMFunction& fun)
{
    MOZ_ASSERT(!inStubFrame_);
=======
void BaselineCacheIRCompiler::tailCallVMInternal(MacroAssembler& masm,
                                                 TailCallVMFunctionId id) {
  MOZ_ASSERT(!preparedForVMCall_);
>>>>>>> upstream-releases

<<<<<<< HEAD
  TrampolinePtr code = cx_->runtime()->jitRuntime()->getVMWrapper(fun);
  MOZ_ASSERT(fun.expectTailCall == TailCall);
  size_t argSize = fun.explicitStackSlots() * sizeof(void*);
||||||| merged common ancestors
    TrampolinePtr code = cx_->runtime()->jitRuntime()->getVMWrapper(fun);
    MOZ_ASSERT(fun.expectTailCall == TailCall);
    size_t argSize = fun.explicitStackSlots() * sizeof(void*);
=======
  TrampolinePtr code = cx_->runtime()->jitRuntime()->getVMWrapper(id);
  const VMFunctionData& fun = GetVMFunction(id);
  MOZ_ASSERT(fun.expectTailCall == TailCall);
  size_t argSize = fun.explicitStackSlots() * sizeof(void*);
>>>>>>> upstream-releases

<<<<<<< HEAD
  EmitBaselineTailCallVM(code, masm, argSize);
  return true;
||||||| merged common ancestors
    EmitBaselineTailCallVM(code, masm, argSize);
    return true;
=======
  EmitBaselineTailCallVM(code, masm, argSize);
>>>>>>> upstream-releases
}

static size_t GetEnteredOffset(BaselineCacheIRStubKind kind) {
  switch (kind) {
    case BaselineCacheIRStubKind::Regular:
      return ICCacheIR_Regular::offsetOfEnteredCount();
    case BaselineCacheIRStubKind::Updated:
      return ICCacheIR_Updated::offsetOfEnteredCount();
    case BaselineCacheIRStubKind::Monitored:
      return ICCacheIR_Monitored::offsetOfEnteredCount();
  }
  MOZ_CRASH("unhandled BaselineCacheIRStubKind");
}

JitCode* BaselineCacheIRCompiler::compile() {
#ifndef JS_USE_LINK_REGISTER
  // The first value contains the return addres,
  // which we pull into ICTailCallReg for tail calls.
  masm.adjustFrame(sizeof(intptr_t));
#endif
#ifdef JS_CODEGEN_ARM
  masm.setSecondScratchReg(BaselineSecondScratchReg);
#endif
<<<<<<< HEAD
  // Count stub entries: We count entries rather than successes as it much
  // easier to ensure ICStubReg is valid at entry than at exit.
  Address enteredCount(ICStubReg, GetEnteredOffset(kind_));
  masm.add32(Imm32(1), enteredCount);

  do {
    switch (reader.readOp()) {
#define DEFINE_OP(op)                \
  case CacheOp::op:                  \
    if (!emit##op()) return nullptr; \
    break;
      CACHE_IR_OPS(DEFINE_OP)
||||||| merged common ancestors

    do {
        switch (reader.readOp()) {
#define DEFINE_OP(op)                   \
          case CacheOp::op:             \
            if (!emit##op())            \
                return nullptr;         \
            break;
    CACHE_IR_OPS(DEFINE_OP)
=======
  // Count stub entries: We count entries rather than successes as it much
  // easier to ensure ICStubReg is valid at entry than at exit.
  Address enteredCount(ICStubReg, GetEnteredOffset(kind_));
  masm.add32(Imm32(1), enteredCount);

  do {
    switch (reader.readOp()) {
#define DEFINE_OP(op, ...)           \
  case CacheOp::op:                  \
    if (!emit##op()) return nullptr; \
    break;
      CACHE_IR_OPS(DEFINE_OP)
>>>>>>> upstream-releases
#undef DEFINE_OP

<<<<<<< HEAD
      default:
        MOZ_CRASH("Invalid op");
    }

    allocator.nextOp();
  } while (reader.more());

  MOZ_ASSERT(!inStubFrame_);
  masm.assumeUnreachable("Should have returned from IC");

  // Done emitting the main IC code. Now emit the failure paths.
  for (size_t i = 0; i < failurePaths.length(); i++) {
    if (!emitFailurePath(i)) {
      return nullptr;
    }
    EmitStubGuardFailure(masm);
  }

  Linker linker(masm);
  AutoFlushICache afc("getStubCode");
  Rooted<JitCode*> newStubCode(cx_, linker.newCode(cx_, CodeKind::Baseline));
  if (!newStubCode) {
    cx_->recoverFromOutOfMemory();
    return nullptr;
  }

  return newStubCode;
}

bool BaselineCacheIRCompiler::emitGuardShape() {
  ObjOperandId objId = reader.objOperandId();
  Register obj = allocator.useRegister(masm, objId);
  AutoScratchRegister scratch1(allocator, masm);

  bool needSpectreMitigations = objectGuardNeedsSpectreMitigations(objId);

  Maybe<AutoScratchRegister> maybeScratch2;
  if (needSpectreMitigations) {
    maybeScratch2.emplace(allocator, masm);
  }

  FailurePath* failure;
  if (!addFailurePath(&failure)) {
    return false;
  }

  Address addr(stubAddress(reader.stubOffset()));
  masm.loadPtr(addr, scratch1);
  if (needSpectreMitigations) {
    masm.branchTestObjShape(Assembler::NotEqual, obj, scratch1, *maybeScratch2,
                            obj, failure->label());
  } else {
    masm.branchTestObjShapeNoSpectreMitigations(Assembler::NotEqual, obj,
                                                scratch1, failure->label());
  }

  return true;
}

bool BaselineCacheIRCompiler::emitGuardGroup() {
  ObjOperandId objId = reader.objOperandId();
  Register obj = allocator.useRegister(masm, objId);
  AutoScratchRegister scratch1(allocator, masm);

  bool needSpectreMitigations = objectGuardNeedsSpectreMitigations(objId);

  Maybe<AutoScratchRegister> maybeScratch2;
  if (needSpectreMitigations) {
    maybeScratch2.emplace(allocator, masm);
  }

  FailurePath* failure;
  if (!addFailurePath(&failure)) {
    return false;
  }
||||||| merged common ancestors
          default:
            MOZ_CRASH("Invalid op");
        }

        allocator.nextOp();
    } while (reader.more());

    MOZ_ASSERT(!inStubFrame_);
    masm.assumeUnreachable("Should have returned from IC");

    // Done emitting the main IC code. Now emit the failure paths.
    for (size_t i = 0; i < failurePaths.length(); i++) {
        if (!emitFailurePath(i)) {
            return nullptr;
        }
        EmitStubGuardFailure(masm);
    }

    Linker linker(masm);
    AutoFlushICache afc("getStubCode");
    Rooted<JitCode*> newStubCode(cx_, linker.newCode(cx_, CodeKind::Baseline));
    if (!newStubCode) {
        cx_->recoverFromOutOfMemory();
        return nullptr;
    }

    return newStubCode;
}

bool
BaselineCacheIRCompiler::emitGuardShape()
{
    ObjOperandId objId = reader.objOperandId();
    Register obj = allocator.useRegister(masm, objId);
    AutoScratchRegister scratch1(allocator, masm);

    bool needSpectreMitigations = objectGuardNeedsSpectreMitigations(objId);

    Maybe<AutoScratchRegister> maybeScratch2;
    if (needSpectreMitigations) {
        maybeScratch2.emplace(allocator, masm);
    }

    FailurePath* failure;
    if (!addFailurePath(&failure)) {
        return false;
    }

    Address addr(stubAddress(reader.stubOffset()));
    masm.loadPtr(addr, scratch1);
    if (needSpectreMitigations) {
        masm.branchTestObjShape(Assembler::NotEqual, obj, scratch1, *maybeScratch2, obj,
                                failure->label());
    } else {
        masm.branchTestObjShapeNoSpectreMitigations(Assembler::NotEqual, obj, scratch1,
                                                    failure->label());
    }

    return true;
}

bool
BaselineCacheIRCompiler::emitGuardGroup()
{
    ObjOperandId objId = reader.objOperandId();
    Register obj = allocator.useRegister(masm, objId);
    AutoScratchRegister scratch1(allocator, masm);

    bool needSpectreMitigations = objectGuardNeedsSpectreMitigations(objId);

    Maybe<AutoScratchRegister> maybeScratch2;
    if (needSpectreMitigations) {
        maybeScratch2.emplace(allocator, masm);
    }

    FailurePath* failure;
    if (!addFailurePath(&failure)) {
        return false;
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
>>>>>>> upstream-releases

<<<<<<< HEAD
  Address addr(stubAddress(reader.stubOffset()));
  masm.loadPtr(addr, scratch1);
  if (needSpectreMitigations) {
    masm.branchTestObjGroup(Assembler::NotEqual, obj, scratch1, *maybeScratch2,
                            obj, failure->label());
  } else {
    masm.branchTestObjGroupNoSpectreMitigations(Assembler::NotEqual, obj,
                                                scratch1, failure->label());
  }

  return true;
}

bool BaselineCacheIRCompiler::emitGuardProto() {
  Register obj = allocator.useRegister(masm, reader.objOperandId());
  AutoScratchRegister scratch(allocator, masm);

  FailurePath* failure;
  if (!addFailurePath(&failure)) {
    return false;
  }

  Address addr(stubAddress(reader.stubOffset()));
  masm.loadObjProto(obj, scratch);
  masm.branchPtr(Assembler::NotEqual, addr, scratch, failure->label());
  return true;
}

bool BaselineCacheIRCompiler::emitGuardCompartment() {
  Register obj = allocator.useRegister(masm, reader.objOperandId());
  AutoScratchRegister scratch(allocator, masm);

  FailurePath* failure;
  if (!addFailurePath(&failure)) {
    return false;
  }

  // Verify that the global wrapper is still valid, as
  // it is pre-requisite for doing the compartment check.
  Address globalWrapper(stubAddress(reader.stubOffset()));
  masm.loadPtr(globalWrapper, scratch);
  Address handlerAddr(scratch, ProxyObject::offsetOfHandler());
  masm.branchPtr(Assembler::Equal, handlerAddr,
                 ImmPtr(&DeadObjectProxy::singleton), failure->label());

  Address addr(stubAddress(reader.stubOffset()));
  masm.branchTestObjCompartment(Assembler::NotEqual, obj, addr, scratch,
                                failure->label());
  return true;
||||||| merged common ancestors
    Address addr(stubAddress(reader.stubOffset()));
    masm.loadPtr(addr, scratch1);
    if (needSpectreMitigations) {
        masm.branchTestObjGroup(Assembler::NotEqual, obj, scratch1, *maybeScratch2, obj,
                                failure->label());
    } else {
        masm.branchTestObjGroupNoSpectreMitigations(Assembler::NotEqual, obj, scratch1,
                                                    failure->label());
    }

    return true;
}

bool
BaselineCacheIRCompiler::emitGuardProto()
{
    Register obj = allocator.useRegister(masm, reader.objOperandId());
    AutoScratchRegister scratch(allocator, masm);

    FailurePath* failure;
    if (!addFailurePath(&failure)) {
        return false;
    }

    Address addr(stubAddress(reader.stubOffset()));
    masm.loadObjProto(obj, scratch);
    masm.branchPtr(Assembler::NotEqual, addr, scratch, failure->label());
    return true;
}

bool
BaselineCacheIRCompiler::emitGuardCompartment()
{
    Register obj = allocator.useRegister(masm, reader.objOperandId());
    AutoScratchRegister scratch(allocator, masm);

    FailurePath* failure;
    if (!addFailurePath(&failure)) {
        return false;
    }

    // Verify that the global wrapper is still valid, as
    // it is pre-requisite for doing the compartment check.
    Address globalWrapper(stubAddress(reader.stubOffset()));
    masm.loadPtr(globalWrapper, scratch);
    Address handlerAddr(scratch, ProxyObject::offsetOfHandler());
    masm.branchPtr(Assembler::Equal, handlerAddr, ImmPtr(&DeadObjectProxy::singleton), failure->label());

    Address addr(stubAddress(reader.stubOffset()));
    masm.branchTestObjCompartment(Assembler::NotEqual, obj, addr, scratch, failure->label());
    return true;
=======
  MOZ_ASSERT(!preparedForVMCall_);
  masm.assumeUnreachable("Should have returned from IC");

  // Done emitting the main IC code. Now emit the failure paths.
  for (size_t i = 0; i < failurePaths.length(); i++) {
    if (!emitFailurePath(i)) {
      return nullptr;
    }
    EmitStubGuardFailure(masm);
  }

  Linker linker(masm, "getStubCode");
  Rooted<JitCode*> newStubCode(cx_, linker.newCode(cx_, CodeKind::Baseline));
  if (!newStubCode) {
    cx_->recoverFromOutOfMemory();
    return nullptr;
  }

  return newStubCode;
}

bool BaselineCacheIRCompiler::emitGuardShape() {
  JitSpew(JitSpew_Codegen, __FUNCTION__);
  ObjOperandId objId = reader.objOperandId();
  Register obj = allocator.useRegister(masm, objId);
  AutoScratchRegister scratch1(allocator, masm);

  bool needSpectreMitigations = objectGuardNeedsSpectreMitigations(objId);

  Maybe<AutoScratchRegister> maybeScratch2;
  if (needSpectreMitigations) {
    maybeScratch2.emplace(allocator, masm);
  }

  FailurePath* failure;
  if (!addFailurePath(&failure)) {
    return false;
  }

  Address addr(stubAddress(reader.stubOffset()));
  masm.loadPtr(addr, scratch1);
  if (needSpectreMitigations) {
    masm.branchTestObjShape(Assembler::NotEqual, obj, scratch1, *maybeScratch2,
                            obj, failure->label());
  } else {
    masm.branchTestObjShapeNoSpectreMitigations(Assembler::NotEqual, obj,
                                                scratch1, failure->label());
  }

  return true;
}

bool BaselineCacheIRCompiler::emitGuardGroup() {
  JitSpew(JitSpew_Codegen, __FUNCTION__);
  ObjOperandId objId = reader.objOperandId();
  Register obj = allocator.useRegister(masm, objId);
  AutoScratchRegister scratch1(allocator, masm);

  bool needSpectreMitigations = objectGuardNeedsSpectreMitigations(objId);

  Maybe<AutoScratchRegister> maybeScratch2;
  if (needSpectreMitigations) {
    maybeScratch2.emplace(allocator, masm);
  }

  FailurePath* failure;
  if (!addFailurePath(&failure)) {
    return false;
  }

  Address addr(stubAddress(reader.stubOffset()));
  masm.loadPtr(addr, scratch1);
  if (needSpectreMitigations) {
    masm.branchTestObjGroup(Assembler::NotEqual, obj, scratch1, *maybeScratch2,
                            obj, failure->label());
  } else {
    masm.branchTestObjGroupNoSpectreMitigations(Assembler::NotEqual, obj,
                                                scratch1, failure->label());
  }

  return true;
}

bool BaselineCacheIRCompiler::emitGuardProto() {
  JitSpew(JitSpew_Codegen, __FUNCTION__);
  Register obj = allocator.useRegister(masm, reader.objOperandId());
  AutoScratchRegister scratch(allocator, masm);

  FailurePath* failure;
  if (!addFailurePath(&failure)) {
    return false;
  }

  Address addr(stubAddress(reader.stubOffset()));
  masm.loadObjProto(obj, scratch);
  masm.branchPtr(Assembler::NotEqual, addr, scratch, failure->label());
  return true;
}

bool BaselineCacheIRCompiler::emitGuardCompartment() {
  JitSpew(JitSpew_Codegen, __FUNCTION__);
  Register obj = allocator.useRegister(masm, reader.objOperandId());
  AutoScratchRegister scratch(allocator, masm);

  FailurePath* failure;
  if (!addFailurePath(&failure)) {
    return false;
  }

  // Verify that the global wrapper is still valid, as
  // it is pre-requisite for doing the compartment check.
  Address globalWrapper(stubAddress(reader.stubOffset()));
  masm.loadPtr(globalWrapper, scratch);
  Address handlerAddr(scratch, ProxyObject::offsetOfHandler());
  masm.branchPtr(Assembler::Equal, handlerAddr,
                 ImmPtr(&DeadObjectProxy::singleton), failure->label());

  Address addr(stubAddress(reader.stubOffset()));
  masm.branchTestObjCompartment(Assembler::NotEqual, obj, addr, scratch,
                                failure->label());
  return true;
>>>>>>> upstream-releases
}

<<<<<<< HEAD
bool BaselineCacheIRCompiler::emitGuardAnyClass() {
  ObjOperandId objId = reader.objOperandId();
  Register obj = allocator.useRegister(masm, objId);
  AutoScratchRegister scratch(allocator, masm);
||||||| merged common ancestors
bool
BaselineCacheIRCompiler::emitGuardAnyClass()
{
    ObjOperandId objId = reader.objOperandId();
    Register obj = allocator.useRegister(masm, objId);
    AutoScratchRegister scratch(allocator, masm);
=======
bool BaselineCacheIRCompiler::emitGuardAnyClass() {
  JitSpew(JitSpew_Codegen, __FUNCTION__);
  ObjOperandId objId = reader.objOperandId();
  Register obj = allocator.useRegister(masm, objId);
  AutoScratchRegister scratch(allocator, masm);
>>>>>>> upstream-releases

  FailurePath* failure;
  if (!addFailurePath(&failure)) {
    return false;
  }

  Address testAddr(stubAddress(reader.stubOffset()));
  if (objectGuardNeedsSpectreMitigations(objId)) {
    masm.branchTestObjClass(Assembler::NotEqual, obj, testAddr, scratch, obj,
                            failure->label());
  } else {
    masm.branchTestObjClassNoSpectreMitigations(
        Assembler::NotEqual, obj, testAddr, scratch, failure->label());
  }

  return true;
}

<<<<<<< HEAD
bool BaselineCacheIRCompiler::emitGuardHasProxyHandler() {
  Register obj = allocator.useRegister(masm, reader.objOperandId());
  AutoScratchRegister scratch(allocator, masm);
||||||| merged common ancestors
bool
BaselineCacheIRCompiler::emitGuardHasProxyHandler()
{
    Register obj = allocator.useRegister(masm, reader.objOperandId());
    AutoScratchRegister scratch(allocator, masm);
=======
bool BaselineCacheIRCompiler::emitGuardHasProxyHandler() {
  JitSpew(JitSpew_Codegen, __FUNCTION__);
  Register obj = allocator.useRegister(masm, reader.objOperandId());
  AutoScratchRegister scratch(allocator, masm);
>>>>>>> upstream-releases

  FailurePath* failure;
  if (!addFailurePath(&failure)) {
    return false;
  }

  Address testAddr(stubAddress(reader.stubOffset()));
  masm.loadPtr(testAddr, scratch);

  Address handlerAddr(obj, ProxyObject::offsetOfHandler());
  masm.branchPtr(Assembler::NotEqual, handlerAddr, scratch, failure->label());
  return true;
}

<<<<<<< HEAD
bool BaselineCacheIRCompiler::emitGuardSpecificObject() {
  Register obj = allocator.useRegister(masm, reader.objOperandId());
||||||| merged common ancestors
bool
BaselineCacheIRCompiler::emitGuardSpecificObject()
{
    Register obj = allocator.useRegister(masm, reader.objOperandId());
=======
bool BaselineCacheIRCompiler::emitGuardSpecificObject() {
  JitSpew(JitSpew_Codegen, __FUNCTION__);
  Register obj = allocator.useRegister(masm, reader.objOperandId());
>>>>>>> upstream-releases

  FailurePath* failure;
  if (!addFailurePath(&failure)) {
    return false;
  }

  Address addr(stubAddress(reader.stubOffset()));
  masm.branchPtr(Assembler::NotEqual, addr, obj, failure->label());
  return true;
}

<<<<<<< HEAD
bool BaselineCacheIRCompiler::emitGuardSpecificAtom() {
  Register str = allocator.useRegister(masm, reader.stringOperandId());
  AutoScratchRegister scratch(allocator, masm);

  FailurePath* failure;
  if (!addFailurePath(&failure)) {
    return false;
  }

  Address atomAddr(stubAddress(reader.stubOffset()));
||||||| merged common ancestors
bool
BaselineCacheIRCompiler::emitGuardSpecificAtom()
{
    Register str = allocator.useRegister(masm, reader.stringOperandId());
    AutoScratchRegister scratch(allocator, masm);

    FailurePath* failure;
    if (!addFailurePath(&failure)) {
        return false;
    }

    Address atomAddr(stubAddress(reader.stubOffset()));
=======
bool BaselineCacheIRCompiler::emitGuardSpecificAtom() {
  JitSpew(JitSpew_Codegen, __FUNCTION__);
  Register str = allocator.useRegister(masm, reader.stringOperandId());
  AutoScratchRegister scratch(allocator, masm);
>>>>>>> upstream-releases

<<<<<<< HEAD
  Label done;
  masm.branchPtr(Assembler::Equal, atomAddr, str, &done);
||||||| merged common ancestors
    Label done;
    masm.branchPtr(Assembler::Equal, atomAddr, str, &done);
=======
  FailurePath* failure;
  if (!addFailurePath(&failure)) {
    return false;
  }
>>>>>>> upstream-releases

<<<<<<< HEAD
  // The pointers are not equal, so if the input string is also an atom it
  // must be a different string.
  masm.branchTest32(Assembler::Zero, Address(str, JSString::offsetOfFlags()),
                    Imm32(JSString::NON_ATOM_BIT), failure->label());
||||||| merged common ancestors
    // The pointers are not equal, so if the input string is also an atom it
    // must be a different string.
    masm.branchTest32(Assembler::Zero, Address(str, JSString::offsetOfFlags()),
                      Imm32(JSString::NON_ATOM_BIT), failure->label());
=======
  Address atomAddr(stubAddress(reader.stubOffset()));
>>>>>>> upstream-releases

<<<<<<< HEAD
  // Check the length.
  masm.loadPtr(atomAddr, scratch);
  masm.loadStringLength(scratch, scratch);
  masm.branch32(Assembler::NotEqual, Address(str, JSString::offsetOfLength()),
                scratch, failure->label());
||||||| merged common ancestors
    // Check the length.
    masm.loadPtr(atomAddr, scratch);
    masm.loadStringLength(scratch, scratch);
    masm.branch32(Assembler::NotEqual, Address(str, JSString::offsetOfLength()),
                  scratch, failure->label());
=======
  Label done;
  masm.branchPtr(Assembler::Equal, atomAddr, str, &done);
>>>>>>> upstream-releases

<<<<<<< HEAD
  // We have a non-atomized string with the same length. Call a helper
  // function to do the comparison.
  LiveRegisterSet volatileRegs(GeneralRegisterSet::Volatile(),
                               liveVolatileFloatRegs());
  masm.PushRegsInMask(volatileRegs);

  masm.setupUnalignedABICall(scratch);
  masm.loadPtr(atomAddr, scratch);
  masm.passABIArg(scratch);
  masm.passABIArg(str);
  masm.callWithABI(JS_FUNC_TO_DATA_PTR(void*, EqualStringsHelperPure));
  masm.mov(ReturnReg, scratch);
||||||| merged common ancestors
    // We have a non-atomized string with the same length. Call a helper
    // function to do the comparison.
    LiveRegisterSet volatileRegs(GeneralRegisterSet::Volatile(), liveVolatileFloatRegs());
    masm.PushRegsInMask(volatileRegs);

    masm.setupUnalignedABICall(scratch);
    masm.loadPtr(atomAddr, scratch);
    masm.passABIArg(scratch);
    masm.passABIArg(str);
    masm.callWithABI(JS_FUNC_TO_DATA_PTR(void*, EqualStringsHelperPure));
    masm.mov(ReturnReg, scratch);
=======
  // The pointers are not equal, so if the input string is also an atom it
  // must be a different string.
  masm.branchTest32(Assembler::Zero, Address(str, JSString::offsetOfFlags()),
                    Imm32(JSString::NON_ATOM_BIT), failure->label());

  // Check the length.
  masm.loadPtr(atomAddr, scratch);
  masm.loadStringLength(scratch, scratch);
  masm.branch32(Assembler::NotEqual, Address(str, JSString::offsetOfLength()),
                scratch, failure->label());
>>>>>>> upstream-releases

<<<<<<< HEAD
  LiveRegisterSet ignore;
  ignore.add(scratch);
  masm.PopRegsInMaskIgnore(volatileRegs, ignore);
  masm.branchIfFalseBool(scratch, failure->label());
||||||| merged common ancestors
    LiveRegisterSet ignore;
    ignore.add(scratch);
    masm.PopRegsInMaskIgnore(volatileRegs, ignore);
    masm.branchIfFalseBool(scratch, failure->label());
=======
  // We have a non-atomized string with the same length. Call a helper
  // function to do the comparison.
  LiveRegisterSet volatileRegs(GeneralRegisterSet::Volatile(),
                               liveVolatileFloatRegs());
  masm.PushRegsInMask(volatileRegs);
>>>>>>> upstream-releases

<<<<<<< HEAD
  masm.bind(&done);
  return true;
}
||||||| merged common ancestors
    masm.bind(&done);
    return true;
}
=======
  masm.setupUnalignedABICall(scratch);
  masm.loadPtr(atomAddr, scratch);
  masm.passABIArg(scratch);
  masm.passABIArg(str);
  masm.callWithABI(JS_FUNC_TO_DATA_PTR(void*, EqualStringsHelperPure));
  masm.mov(ReturnReg, scratch);
>>>>>>> upstream-releases

<<<<<<< HEAD
bool BaselineCacheIRCompiler::emitGuardSpecificSymbol() {
  Register sym = allocator.useRegister(masm, reader.symbolOperandId());
||||||| merged common ancestors
bool
BaselineCacheIRCompiler::emitGuardSpecificSymbol()
{
    Register sym = allocator.useRegister(masm, reader.symbolOperandId());
=======
  LiveRegisterSet ignore;
  ignore.add(scratch);
  masm.PopRegsInMaskIgnore(volatileRegs, ignore);
  masm.branchIfFalseBool(scratch, failure->label());
>>>>>>> upstream-releases

<<<<<<< HEAD
  FailurePath* failure;
  if (!addFailurePath(&failure)) {
    return false;
  }

  Address addr(stubAddress(reader.stubOffset()));
  masm.branchPtr(Assembler::NotEqual, addr, sym, failure->label());
  return true;
||||||| merged common ancestors
    FailurePath* failure;
    if (!addFailurePath(&failure)) {
        return false;
    }

    Address addr(stubAddress(reader.stubOffset()));
    masm.branchPtr(Assembler::NotEqual, addr, sym, failure->label());
    return true;
=======
  masm.bind(&done);
  return true;
}

bool BaselineCacheIRCompiler::emitGuardSpecificSymbol() {
  JitSpew(JitSpew_Codegen, __FUNCTION__);
  Register sym = allocator.useRegister(masm, reader.symbolOperandId());

  FailurePath* failure;
  if (!addFailurePath(&failure)) {
    return false;
  }

  Address addr(stubAddress(reader.stubOffset()));
  masm.branchPtr(Assembler::NotEqual, addr, sym, failure->label());
  return true;
>>>>>>> upstream-releases
}

<<<<<<< HEAD
bool BaselineCacheIRCompiler::emitLoadValueResult() {
  AutoOutputRegister output(*this);
  masm.loadValue(stubAddress(reader.stubOffset()), output.valueReg());
  return true;
||||||| merged common ancestors
bool
BaselineCacheIRCompiler::emitLoadValueResult()
{
    AutoOutputRegister output(*this);
    masm.loadValue(stubAddress(reader.stubOffset()), output.valueReg());
    return true;
=======
bool BaselineCacheIRCompiler::emitLoadValueResult() {
  JitSpew(JitSpew_Codegen, __FUNCTION__);
  AutoOutputRegister output(*this);
  masm.loadValue(stubAddress(reader.stubOffset()), output.valueReg());
  return true;
>>>>>>> upstream-releases
}

<<<<<<< HEAD
bool BaselineCacheIRCompiler::emitLoadFixedSlotResult() {
  AutoOutputRegister output(*this);
  Register obj = allocator.useRegister(masm, reader.objOperandId());
  AutoScratchRegisterMaybeOutput scratch(allocator, masm, output);

  masm.load32(stubAddress(reader.stubOffset()), scratch);
  masm.loadValue(BaseIndex(obj, scratch, TimesOne), output.valueReg());
  return true;
||||||| merged common ancestors
bool
BaselineCacheIRCompiler::emitLoadFixedSlotResult()
{
    AutoOutputRegister output(*this);
    Register obj = allocator.useRegister(masm, reader.objOperandId());
    AutoScratchRegisterMaybeOutput scratch(allocator, masm, output);

    masm.load32(stubAddress(reader.stubOffset()), scratch);
    masm.loadValue(BaseIndex(obj, scratch, TimesOne), output.valueReg());
    return true;
=======
bool BaselineCacheIRCompiler::emitLoadFixedSlotResult() {
  JitSpew(JitSpew_Codegen, __FUNCTION__);
  AutoOutputRegister output(*this);
  Register obj = allocator.useRegister(masm, reader.objOperandId());
  AutoScratchRegisterMaybeOutput scratch(allocator, masm, output);

  masm.load32(stubAddress(reader.stubOffset()), scratch);
  masm.loadValue(BaseIndex(obj, scratch, TimesOne), output.valueReg());
  return true;
>>>>>>> upstream-releases
}

<<<<<<< HEAD
bool BaselineCacheIRCompiler::emitLoadDynamicSlotResult() {
  AutoOutputRegister output(*this);
  Register obj = allocator.useRegister(masm, reader.objOperandId());
  AutoScratchRegisterMaybeOutput scratch(allocator, masm, output);
  AutoScratchRegister scratch2(allocator, masm);
||||||| merged common ancestors
bool
BaselineCacheIRCompiler::emitLoadDynamicSlotResult()
{
    AutoOutputRegister output(*this);
    Register obj = allocator.useRegister(masm, reader.objOperandId());
    AutoScratchRegisterMaybeOutput scratch(allocator, masm, output);
    AutoScratchRegister scratch2(allocator, masm);
=======
bool BaselineCacheIRCompiler::emitLoadDynamicSlotResult() {
  JitSpew(JitSpew_Codegen, __FUNCTION__);
  AutoOutputRegister output(*this);
  Register obj = allocator.useRegister(masm, reader.objOperandId());
  AutoScratchRegisterMaybeOutput scratch(allocator, masm, output);
  AutoScratchRegister scratch2(allocator, masm);

  masm.load32(stubAddress(reader.stubOffset()), scratch);
  masm.loadPtr(Address(obj, NativeObject::offsetOfSlots()), scratch2);
  masm.loadValue(BaseIndex(scratch2, scratch, TimesOne), output.valueReg());
  return true;
}

bool BaselineCacheIRCompiler::emitGuardHasGetterSetter() {
  JitSpew(JitSpew_Codegen, __FUNCTION__);
  Register obj = allocator.useRegister(masm, reader.objOperandId());
  Address shapeAddr = stubAddress(reader.stubOffset());

  AutoScratchRegister scratch1(allocator, masm);
  AutoScratchRegister scratch2(allocator, masm);

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
  masm.loadPtr(shapeAddr, scratch2);
  masm.passABIArg(scratch2);
  masm.callWithABI(JS_FUNC_TO_DATA_PTR(void*, ObjectHasGetterSetterPure));
  masm.mov(ReturnReg, scratch1);
  masm.PopRegsInMask(volatileRegs);

  masm.branchIfFalseBool(scratch1, failure->label());
  return true;
}

bool BaselineCacheIRCompiler::emitCallScriptedGetterResultShared(
    TypedOrValueRegister receiver) {
  Address getterAddr(stubAddress(reader.stubOffset()));
  bool isSameRealm = reader.readBool();
>>>>>>> upstream-releases

<<<<<<< HEAD
  masm.load32(stubAddress(reader.stubOffset()), scratch);
  masm.loadPtr(Address(obj, NativeObject::offsetOfSlots()), scratch2);
  masm.loadValue(BaseIndex(scratch2, scratch, TimesOne), output.valueReg());
  return true;
}

bool BaselineCacheIRCompiler::emitGuardHasGetterSetter() {
  Register obj = allocator.useRegister(masm, reader.objOperandId());
  Address shapeAddr = stubAddress(reader.stubOffset());

  AutoScratchRegister scratch1(allocator, masm);
  AutoScratchRegister scratch2(allocator, masm);
||||||| merged common ancestors
    masm.load32(stubAddress(reader.stubOffset()), scratch);
    masm.loadPtr(Address(obj, NativeObject::offsetOfSlots()), scratch2);
    masm.loadValue(BaseIndex(scratch2, scratch, TimesOne), output.valueReg());
    return true;
}

bool
BaselineCacheIRCompiler::emitGuardHasGetterSetter()
{
    Register obj = allocator.useRegister(masm, reader.objOperandId());
    Address shapeAddr = stubAddress(reader.stubOffset());

    AutoScratchRegister scratch1(allocator, masm);
    AutoScratchRegister scratch2(allocator, masm);

    FailurePath* failure;
    if (!addFailurePath(&failure)) {
        return false;
    }
=======
  AutoScratchRegister code(allocator, masm);
  AutoScratchRegister callee(allocator, masm);
  AutoScratchRegister scratch(allocator, masm);

  // First, ensure our getter is non-lazy.
  {
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
    LiveRegisterSet volatileRegs(GeneralRegisterSet::Volatile(), liveVolatileFloatRegs());
    volatileRegs.takeUnchecked(scratch1);
    volatileRegs.takeUnchecked(scratch2);
    masm.PushRegsInMask(volatileRegs);
=======
    masm.loadPtr(getterAddr, callee);
    masm.branchIfFunctionHasNoJitEntry(callee, /* constructing */ false,
                                       failure->label());
    masm.loadJitCodeRaw(callee, code);
  }
>>>>>>> upstream-releases

<<<<<<< HEAD
  LiveRegisterSet volatileRegs(GeneralRegisterSet::Volatile(),
                               liveVolatileFloatRegs());
  volatileRegs.takeUnchecked(scratch1);
  volatileRegs.takeUnchecked(scratch2);
  masm.PushRegsInMask(volatileRegs);
||||||| merged common ancestors
    masm.setupUnalignedABICall(scratch1);
    masm.loadJSContext(scratch1);
    masm.passABIArg(scratch1);
    masm.passABIArg(obj);
    masm.loadPtr(shapeAddr, scratch2);
    masm.passABIArg(scratch2);
    masm.callWithABI(JS_FUNC_TO_DATA_PTR(void*, ObjectHasGetterSetterPure));
    masm.mov(ReturnReg, scratch1);
    masm.PopRegsInMask(volatileRegs);
=======
  allocator.discardStack(masm);
>>>>>>> upstream-releases

<<<<<<< HEAD
  masm.setupUnalignedABICall(scratch1);
  masm.loadJSContext(scratch1);
  masm.passABIArg(scratch1);
  masm.passABIArg(obj);
  masm.loadPtr(shapeAddr, scratch2);
  masm.passABIArg(scratch2);
  masm.callWithABI(JS_FUNC_TO_DATA_PTR(void*, ObjectHasGetterSetterPure));
  masm.mov(ReturnReg, scratch1);
  masm.PopRegsInMask(volatileRegs);

  masm.branchIfFalseBool(scratch1, failure->label());
  return true;
}
||||||| merged common ancestors
    masm.branchIfFalseBool(scratch1, failure->label());
    return true;
}
=======
  AutoStubFrame stubFrame(*this);
  stubFrame.enter(masm, scratch);
>>>>>>> upstream-releases

<<<<<<< HEAD
bool BaselineCacheIRCompiler::emitCallScriptedGetterResult() {
  Register obj = allocator.useRegister(masm, reader.objOperandId());
  Address getterAddr(stubAddress(reader.stubOffset()));
  bool isCrossRealm = reader.readBool();

  AutoScratchRegister code(allocator, masm);
  AutoScratchRegister callee(allocator, masm);
  AutoScratchRegister scratch(allocator, masm);

  // First, ensure our getter is non-lazy.
  {
    FailurePath* failure;
    if (!addFailurePath(&failure)) {
      return false;
    }

    masm.loadPtr(getterAddr, callee);
    masm.branchIfFunctionHasNoJitEntry(callee, /* constructing */ false,
                                       failure->label());
    masm.loadJitCodeRaw(callee, code);
  }
||||||| merged common ancestors
bool
BaselineCacheIRCompiler::emitCallScriptedGetterResult()
{
    Register obj = allocator.useRegister(masm, reader.objOperandId());
    Address getterAddr(stubAddress(reader.stubOffset()));
    bool isCrossRealm = reader.readBool();

    AutoScratchRegister code(allocator, masm);
    AutoScratchRegister callee(allocator, masm);
    AutoScratchRegister scratch(allocator, masm);

    // First, ensure our getter is non-lazy.
    {
        FailurePath* failure;
        if (!addFailurePath(&failure)) {
            return false;
        }
=======
  if (!isSameRealm) {
    masm.switchToObjectRealm(callee, scratch);
  }
>>>>>>> upstream-releases

<<<<<<< HEAD
  allocator.discardStack(masm);
||||||| merged common ancestors
        masm.loadPtr(getterAddr, callee);
        masm.branchIfFunctionHasNoJitEntry(callee, /* constructing */ false, failure->label());
        masm.loadJitCodeRaw(callee, code);
    }
=======
  // Align the stack such that the JitFrameLayout is aligned on
  // JitStackAlignment.
  masm.alignJitStackBasedOnNArgs(0);
>>>>>>> upstream-releases

<<<<<<< HEAD
  AutoStubFrame stubFrame(*this);
  stubFrame.enter(masm, scratch);
||||||| merged common ancestors
    allocator.discardStack(masm);
=======
  // Getter is called with 0 arguments, just |receiver| as thisv.
  // Note that we use Push, not push, so that callJit will align the stack
  // properly on ARM.
  masm.Push(receiver);
>>>>>>> upstream-releases

<<<<<<< HEAD
  if (isCrossRealm) {
    masm.switchToObjectRealm(callee, scratch);
  }

  // Align the stack such that the JitFrameLayout is aligned on
  // JitStackAlignment.
  masm.alignJitStackBasedOnNArgs(0);
||||||| merged common ancestors
    AutoStubFrame stubFrame(*this);
    stubFrame.enter(masm, scratch);

    if (isCrossRealm) {
        masm.switchToObjectRealm(callee, scratch);
    }
=======
  EmitBaselineCreateStubFrameDescriptor(masm, scratch, JitFrameLayout::Size());
  masm.Push(Imm32(0));  // ActualArgc is 0
  masm.Push(callee);
  masm.Push(scratch);
>>>>>>> upstream-releases

<<<<<<< HEAD
  // Getter is called with 0 arguments, just |obj| as thisv.
  // Note that we use Push, not push, so that callJit will align the stack
  // properly on ARM.
  masm.Push(TypedOrValueRegister(MIRType::Object, AnyRegister(obj)));
||||||| merged common ancestors
    // Align the stack such that the JitFrameLayout is aligned on
    // JitStackAlignment.
    masm.alignJitStackBasedOnNArgs(0);

    // Getter is called with 0 arguments, just |obj| as thisv.
    // Note that we use Push, not push, so that callJit will align the stack
    // properly on ARM.
    masm.Push(TypedOrValueRegister(MIRType::Object, AnyRegister(obj)));

    EmitBaselineCreateStubFrameDescriptor(masm, scratch, JitFrameLayout::Size());
    masm.Push(Imm32(0));  // ActualArgc is 0
    masm.Push(callee);
    masm.Push(scratch);

    // Handle arguments underflow.
    Label noUnderflow;
    masm.load16ZeroExtend(Address(callee, JSFunction::offsetOfNargs()), callee);
    masm.branch32(Assembler::Equal, callee, Imm32(0), &noUnderflow);
    {
        // Call the arguments rectifier.
        TrampolinePtr argumentsRectifier = cx_->runtime()->jitRuntime()->getArgumentsRectifier();
        masm.movePtr(argumentsRectifier, code);
    }
=======
  // Handle arguments underflow.
  Label noUnderflow;
  masm.load16ZeroExtend(Address(callee, JSFunction::offsetOfNargs()), callee);
  masm.branch32(Assembler::Equal, callee, Imm32(0), &noUnderflow);
  {
    // Call the arguments rectifier.
    TrampolinePtr argumentsRectifier =
        cx_->runtime()->jitRuntime()->getArgumentsRectifier();
    masm.movePtr(argumentsRectifier, code);
  }

  masm.bind(&noUnderflow);
  masm.callJit(code);
>>>>>>> upstream-releases

<<<<<<< HEAD
  EmitBaselineCreateStubFrameDescriptor(masm, scratch, JitFrameLayout::Size());
  masm.Push(Imm32(0));  // ActualArgc is 0
  masm.Push(callee);
  masm.Push(scratch);
||||||| merged common ancestors
    masm.bind(&noUnderflow);
    masm.callJit(code);
=======
  stubFrame.leave(masm, true);
>>>>>>> upstream-releases

<<<<<<< HEAD
  // Handle arguments underflow.
  Label noUnderflow;
  masm.load16ZeroExtend(Address(callee, JSFunction::offsetOfNargs()), callee);
  masm.branch32(Assembler::Equal, callee, Imm32(0), &noUnderflow);
  {
    // Call the arguments rectifier.
    TrampolinePtr argumentsRectifier =
        cx_->runtime()->jitRuntime()->getArgumentsRectifier();
    masm.movePtr(argumentsRectifier, code);
  }
||||||| merged common ancestors
    stubFrame.leave(masm, true);
=======
  if (!isSameRealm) {
    masm.switchToBaselineFrameRealm(R1.scratchReg());
  }
>>>>>>> upstream-releases

<<<<<<< HEAD
  masm.bind(&noUnderflow);
  masm.callJit(code);

  stubFrame.leave(masm, true);

  if (isCrossRealm) {
    masm.switchToBaselineFrameRealm(R1.scratchReg());
  }

  return true;
||||||| merged common ancestors
    if (isCrossRealm) {
        masm.switchToBaselineFrameRealm(R1.scratchReg());
    }

    return true;
=======
  return true;
>>>>>>> upstream-releases
}

<<<<<<< HEAD
typedef bool (*CallNativeGetterFn)(JSContext*, HandleFunction, HandleObject,
                                   MutableHandleValue);
static const VMFunction CallNativeGetterInfo =
    FunctionInfo<CallNativeGetterFn>(CallNativeGetter, "CallNativeGetter");

bool BaselineCacheIRCompiler::emitCallNativeGetterResult() {
  Register obj = allocator.useRegister(masm, reader.objOperandId());
  Address getterAddr(stubAddress(reader.stubOffset()));

  AutoScratchRegister scratch(allocator, masm);
||||||| merged common ancestors
typedef bool (*CallNativeGetterFn)(JSContext*, HandleFunction, HandleObject, MutableHandleValue);
static const VMFunction CallNativeGetterInfo =
    FunctionInfo<CallNativeGetterFn>(CallNativeGetter, "CallNativeGetter");

bool
BaselineCacheIRCompiler::emitCallNativeGetterResult()
{
    Register obj = allocator.useRegister(masm, reader.objOperandId());
    Address getterAddr(stubAddress(reader.stubOffset()));

    AutoScratchRegister scratch(allocator, masm);
=======
bool BaselineCacheIRCompiler::emitCallScriptedGetterResult() {
  JitSpew(JitSpew_Codegen, __FUNCTION__);
  Register obj = allocator.useRegister(masm, reader.objOperandId());
>>>>>>> upstream-releases

<<<<<<< HEAD
  allocator.discardStack(masm);

  AutoStubFrame stubFrame(*this);
  stubFrame.enter(masm, scratch);

  // Load the callee in the scratch register.
  masm.loadPtr(getterAddr, scratch);

  masm.Push(obj);
  masm.Push(scratch);

  if (!callVM(masm, CallNativeGetterInfo)) {
    return false;
  }

  stubFrame.leave(masm);
  return true;
||||||| merged common ancestors
    allocator.discardStack(masm);

    AutoStubFrame stubFrame(*this);
    stubFrame.enter(masm, scratch);

    // Load the callee in the scratch register.
    masm.loadPtr(getterAddr, scratch);

    masm.Push(obj);
    masm.Push(scratch);

    if (!callVM(masm, CallNativeGetterInfo)) {
        return false;
    }

    stubFrame.leave(masm);
    return true;
=======
  return emitCallScriptedGetterResultShared(
      TypedOrValueRegister(MIRType::Object, AnyRegister(obj)));
>>>>>>> upstream-releases
}

<<<<<<< HEAD
bool BaselineCacheIRCompiler::emitCallProxyGetResult() {
  Register obj = allocator.useRegister(masm, reader.objOperandId());
  Address idAddr(stubAddress(reader.stubOffset()));

  AutoScratchRegister scratch(allocator, masm);
||||||| merged common ancestors
bool
BaselineCacheIRCompiler::emitCallProxyGetResult()
{
    Register obj = allocator.useRegister(masm, reader.objOperandId());
    Address idAddr(stubAddress(reader.stubOffset()));

    AutoScratchRegister scratch(allocator, masm);
=======
bool BaselineCacheIRCompiler::emitCallScriptedGetterByValueResult() {
  JitSpew(JitSpew_Codegen, __FUNCTION__);
  ValueOperand val = allocator.useValueRegister(masm, reader.valOperandId());
>>>>>>> upstream-releases

<<<<<<< HEAD
  allocator.discardStack(masm);

  AutoStubFrame stubFrame(*this);
  stubFrame.enter(masm, scratch);

  // Load the jsid in the scratch register.
  masm.loadPtr(idAddr, scratch);

  masm.Push(scratch);
  masm.Push(obj);

  if (!callVM(masm, ProxyGetPropertyInfo)) {
    return false;
  }

  stubFrame.leave(masm);
  return true;
||||||| merged common ancestors
    allocator.discardStack(masm);

    AutoStubFrame stubFrame(*this);
    stubFrame.enter(masm, scratch);

    // Load the jsid in the scratch register.
    masm.loadPtr(idAddr, scratch);

    masm.Push(scratch);
    masm.Push(obj);

    if (!callVM(masm, ProxyGetPropertyInfo)) {
        return false;
    }

    stubFrame.leave(masm);
    return true;
=======
  return emitCallScriptedGetterResultShared(val);
>>>>>>> upstream-releases
}

<<<<<<< HEAD
bool BaselineCacheIRCompiler::emitCallProxyGetByValueResult() {
  Register obj = allocator.useRegister(masm, reader.objOperandId());
  ValueOperand idVal = allocator.useValueRegister(masm, reader.valOperandId());
||||||| merged common ancestors
bool
BaselineCacheIRCompiler::emitCallProxyGetByValueResult()
{
    Register obj = allocator.useRegister(masm, reader.objOperandId());
    ValueOperand idVal = allocator.useValueRegister(masm, reader.valOperandId());
=======
template <typename T, typename CallVM>
bool BaselineCacheIRCompiler::emitCallNativeGetterResultShared(
    T receiver, const CallVM& emitCallVM) {
  Address getterAddr(stubAddress(reader.stubOffset()));
>>>>>>> upstream-releases

  AutoScratchRegister scratch(allocator, masm);

  allocator.discardStack(masm);

<<<<<<< HEAD
  AutoStubFrame stubFrame(*this);
  stubFrame.enter(masm, scratch);

  masm.Push(idVal);
  masm.Push(obj);

  if (!callVM(masm, ProxyGetPropertyByValueInfo)) {
    return false;
  }

  stubFrame.leave(masm);
  return true;
}

bool BaselineCacheIRCompiler::emitCallProxyHasPropResult() {
  Register obj = allocator.useRegister(masm, reader.objOperandId());
  ValueOperand idVal = allocator.useValueRegister(masm, reader.valOperandId());
  bool hasOwn = reader.readBool();

  AutoScratchRegister scratch(allocator, masm);

  allocator.discardStack(masm);

  AutoStubFrame stubFrame(*this);
  stubFrame.enter(masm, scratch);

  masm.Push(idVal);
  masm.Push(obj);

  if (hasOwn) {
    if (!callVM(masm, ProxyHasOwnInfo)) {
      return false;
    }
  } else {
    if (!callVM(masm, ProxyHasInfo)) {
      return false;
    }
  }

  stubFrame.leave(masm);
  return true;
}
||||||| merged common ancestors
    AutoStubFrame stubFrame(*this);
    stubFrame.enter(masm, scratch);

    masm.Push(idVal);
    masm.Push(obj);

    if (!callVM(masm, ProxyGetPropertyByValueInfo)) {
        return false;
    }

    stubFrame.leave(masm);
    return true;
}

bool
BaselineCacheIRCompiler::emitCallProxyHasPropResult()
{
    Register obj = allocator.useRegister(masm, reader.objOperandId());
    ValueOperand idVal = allocator.useValueRegister(masm, reader.valOperandId());
    bool hasOwn = reader.readBool();

    AutoScratchRegister scratch(allocator, masm);

    allocator.discardStack(masm);

    AutoStubFrame stubFrame(*this);
    stubFrame.enter(masm, scratch);

    masm.Push(idVal);
    masm.Push(obj);

    if (hasOwn) {
        if (!callVM(masm, ProxyHasOwnInfo)) {
            return false;
        }
    } else {
        if (!callVM(masm, ProxyHasInfo)) {
            return false;
        }
    }

    stubFrame.leave(masm);
    return true;
}
=======
  AutoStubFrame stubFrame(*this);
  stubFrame.enter(masm, scratch);
>>>>>>> upstream-releases

<<<<<<< HEAD
bool BaselineCacheIRCompiler::emitCallNativeGetElementResult() {
  Register obj = allocator.useRegister(masm, reader.objOperandId());
  Register index = allocator.useRegister(masm, reader.int32OperandId());

  AutoScratchRegister scratch(allocator, masm);

  allocator.discardStack(masm);

  AutoStubFrame stubFrame(*this);
  stubFrame.enter(masm, scratch);
||||||| merged common ancestors
bool
BaselineCacheIRCompiler::emitCallNativeGetElementResult()
{
    Register obj = allocator.useRegister(masm, reader.objOperandId());
    Register index = allocator.useRegister(masm, reader.int32OperandId());

    AutoScratchRegister scratch(allocator, masm);

    allocator.discardStack(masm);

    AutoStubFrame stubFrame(*this);
    stubFrame.enter(masm, scratch);
=======
  // Load the callee in the scratch register.
  masm.loadPtr(getterAddr, scratch);
>>>>>>> upstream-releases

<<<<<<< HEAD
  masm.Push(index);
  masm.Push(TypedOrValueRegister(MIRType::Object, AnyRegister(obj)));
  masm.Push(obj);
||||||| merged common ancestors
    masm.Push(index);
    masm.Push(TypedOrValueRegister(MIRType::Object, AnyRegister(obj)));
    masm.Push(obj);
=======
  masm.Push(receiver);
  masm.Push(scratch);
>>>>>>> upstream-releases

<<<<<<< HEAD
  if (!callVM(masm, NativeGetElementInfo)) {
    return false;
  }
||||||| merged common ancestors
    if (!callVM(masm, NativeGetElementInfo)) {
        return false;
    }
=======
  emitCallVM();
>>>>>>> upstream-releases

  stubFrame.leave(masm);
  return true;
}

<<<<<<< HEAD
bool BaselineCacheIRCompiler::emitLoadUnboxedPropertyResult() {
  AutoOutputRegister output(*this);
  Register obj = allocator.useRegister(masm, reader.objOperandId());
  AutoScratchRegisterMaybeOutput scratch(allocator, masm, output);
||||||| merged common ancestors
bool
BaselineCacheIRCompiler::emitLoadUnboxedPropertyResult()
{
    AutoOutputRegister output(*this);
    Register obj = allocator.useRegister(masm, reader.objOperandId());
    AutoScratchRegisterMaybeOutput scratch(allocator, masm, output);
=======
bool BaselineCacheIRCompiler::emitCallNativeGetterResult() {
  JitSpew(JitSpew_Codegen, __FUNCTION__);
  Register obj = allocator.useRegister(masm, reader.objOperandId());
>>>>>>> upstream-releases

<<<<<<< HEAD
  JSValueType fieldType = reader.valueType();
  Address fieldOffset(stubAddress(reader.stubOffset()));
  masm.load32(fieldOffset, scratch);
  masm.loadUnboxedProperty(BaseIndex(obj, scratch, TimesOne), fieldType,
                           output);
  return true;
||||||| merged common ancestors
    JSValueType fieldType = reader.valueType();
    Address fieldOffset(stubAddress(reader.stubOffset()));
    masm.load32(fieldOffset, scratch);
    masm.loadUnboxedProperty(BaseIndex(obj, scratch, TimesOne), fieldType, output);
    return true;
=======
  return emitCallNativeGetterResultShared(obj, [this]() {
    using Fn =
        bool (*)(JSContext*, HandleFunction, HandleObject, MutableHandleValue);
    callVM<Fn, CallNativeGetter>(masm);
  });
>>>>>>> upstream-releases
}

<<<<<<< HEAD
bool BaselineCacheIRCompiler::emitGuardFrameHasNoArgumentsObject() {
  FailurePath* failure;
  if (!addFailurePath(&failure)) {
    return false;
  }
||||||| merged common ancestors
bool
BaselineCacheIRCompiler::emitGuardFrameHasNoArgumentsObject()
{
    FailurePath* failure;
    if (!addFailurePath(&failure)) {
        return false;
    }
=======
bool BaselineCacheIRCompiler::emitCallNativeGetterByValueResult() {
  JitSpew(JitSpew_Codegen, __FUNCTION__);
  ValueOperand val = allocator.useValueRegister(masm, reader.valOperandId());
>>>>>>> upstream-releases

<<<<<<< HEAD
  masm.branchTest32(
      Assembler::NonZero,
      Address(BaselineFrameReg, BaselineFrame::reverseOffsetOfFlags()),
      Imm32(BaselineFrame::HAS_ARGS_OBJ), failure->label());
  return true;
||||||| merged common ancestors
    masm.branchTest32(Assembler::NonZero,
                      Address(BaselineFrameReg, BaselineFrame::reverseOffsetOfFlags()),
                      Imm32(BaselineFrame::HAS_ARGS_OBJ),
                      failure->label());
    return true;
=======
  return emitCallNativeGetterResultShared(val, [this]() {
    using Fn =
        bool (*)(JSContext*, HandleFunction, HandleValue, MutableHandleValue);
    callVM<Fn, CallNativeGetterByValue>(masm);
  });
>>>>>>> upstream-releases
}

<<<<<<< HEAD
bool BaselineCacheIRCompiler::emitLoadFrameCalleeResult() {
  AutoOutputRegister output(*this);
  AutoScratchRegisterMaybeOutput scratch(allocator, masm, output);
||||||| merged common ancestors
bool
BaselineCacheIRCompiler::emitLoadFrameCalleeResult()
{
    AutoOutputRegister output(*this);
    AutoScratchRegisterMaybeOutput scratch(allocator, masm, output);
=======
bool BaselineCacheIRCompiler::emitCallProxyGetResult() {
  JitSpew(JitSpew_Codegen, __FUNCTION__);
  Register obj = allocator.useRegister(masm, reader.objOperandId());
  Address idAddr(stubAddress(reader.stubOffset()));
>>>>>>> upstream-releases

<<<<<<< HEAD
  Address callee(BaselineFrameReg, BaselineFrame::offsetOfCalleeToken());
  masm.loadFunctionFromCalleeToken(callee, scratch);
  masm.tagValue(JSVAL_TYPE_OBJECT, scratch, output.valueReg());
  return true;
}
||||||| merged common ancestors
    Address callee(BaselineFrameReg, BaselineFrame::offsetOfCalleeToken());
    masm.loadFunctionFromCalleeToken(callee, scratch);
    masm.tagValue(JSVAL_TYPE_OBJECT, scratch, output.valueReg());
    return true;
}
=======
  AutoScratchRegister scratch(allocator, masm);
>>>>>>> upstream-releases

<<<<<<< HEAD
bool BaselineCacheIRCompiler::emitLoadFrameNumActualArgsResult() {
  AutoOutputRegister output(*this);
  AutoScratchRegisterMaybeOutput scratch(allocator, masm, output);
||||||| merged common ancestors
bool
BaselineCacheIRCompiler::emitLoadFrameNumActualArgsResult()
{
    AutoOutputRegister output(*this);
    AutoScratchRegisterMaybeOutput scratch(allocator, masm, output);
=======
  allocator.discardStack(masm);
>>>>>>> upstream-releases

<<<<<<< HEAD
  Address actualArgs(BaselineFrameReg, BaselineFrame::offsetOfNumActualArgs());
  masm.loadPtr(actualArgs, scratch);
  masm.tagValue(JSVAL_TYPE_INT32, scratch, output.valueReg());
  return true;
}
||||||| merged common ancestors
    Address actualArgs(BaselineFrameReg, BaselineFrame::offsetOfNumActualArgs());
    masm.loadPtr(actualArgs, scratch);
    masm.tagValue(JSVAL_TYPE_INT32, scratch, output.valueReg());
    return true;
}
=======
  AutoStubFrame stubFrame(*this);
  stubFrame.enter(masm, scratch);
>>>>>>> upstream-releases

<<<<<<< HEAD
bool BaselineCacheIRCompiler::emitLoadTypedObjectResult() {
  AutoOutputRegister output(*this);
  Register obj = allocator.useRegister(masm, reader.objOperandId());
  AutoScratchRegister scratch1(allocator, masm);
  AutoScratchRegister scratch2(allocator, masm);

  TypedThingLayout layout = reader.typedThingLayout();
  uint32_t typeDescr = reader.typeDescrKey();
  Address fieldOffset(stubAddress(reader.stubOffset()));
||||||| merged common ancestors
bool
BaselineCacheIRCompiler::emitLoadTypedObjectResult()
{
    AutoOutputRegister output(*this);
    Register obj = allocator.useRegister(masm, reader.objOperandId());
    AutoScratchRegister scratch1(allocator, masm);
    AutoScratchRegister scratch2(allocator, masm);

    TypedThingLayout layout = reader.typedThingLayout();
    uint32_t typeDescr = reader.typeDescrKey();
    Address fieldOffset(stubAddress(reader.stubOffset()));
=======
  // Load the jsid in the scratch register.
  masm.loadPtr(idAddr, scratch);
>>>>>>> upstream-releases

<<<<<<< HEAD
  // Get the object's data pointer.
  LoadTypedThingData(masm, layout, obj, scratch1);
||||||| merged common ancestors
    // Get the object's data pointer.
    LoadTypedThingData(masm, layout, obj, scratch1);
=======
  masm.Push(scratch);
  masm.Push(obj);
>>>>>>> upstream-releases

<<<<<<< HEAD
  // Get the address being written to.
  masm.load32(fieldOffset, scratch2);
  masm.addPtr(scratch2, scratch1);
||||||| merged common ancestors
    // Get the address being written to.
    masm.load32(fieldOffset, scratch2);
    masm.addPtr(scratch2, scratch1);
=======
  using Fn = bool (*)(JSContext*, HandleObject, HandleId, MutableHandleValue);
  callVM<Fn, ProxyGetProperty>(masm);
>>>>>>> upstream-releases

<<<<<<< HEAD
  Address fieldAddr(scratch1, 0);
  emitLoadTypedObjectResultShared(fieldAddr, scratch2, typeDescr, output);
  return true;
||||||| merged common ancestors
    Address fieldAddr(scratch1, 0);
    emitLoadTypedObjectResultShared(fieldAddr, scratch2, typeDescr, output);
    return true;
=======
  stubFrame.leave(masm);
  return true;
>>>>>>> upstream-releases
}

<<<<<<< HEAD
bool BaselineCacheIRCompiler::emitLoadFrameArgumentResult() {
  AutoOutputRegister output(*this);
  Register index = allocator.useRegister(masm, reader.int32OperandId());
  AutoScratchRegister scratch1(allocator, masm);
  AutoScratchRegisterMaybeOutput scratch2(allocator, masm, output);
||||||| merged common ancestors
bool
BaselineCacheIRCompiler::emitLoadFrameArgumentResult()
{
    AutoOutputRegister output(*this);
    Register index = allocator.useRegister(masm, reader.int32OperandId());
    AutoScratchRegister scratch1(allocator, masm);
    AutoScratchRegisterMaybeOutput scratch2(allocator, masm, output);
=======
bool BaselineCacheIRCompiler::emitGuardFrameHasNoArgumentsObject() {
  JitSpew(JitSpew_Codegen, __FUNCTION__);
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

  // Bounds check.
  masm.loadPtr(
      Address(BaselineFrameReg, BaselineFrame::offsetOfNumActualArgs()),
      scratch1);
  masm.spectreBoundsCheck32(index, scratch1, scratch2, failure->label());

  // Load the argument.
  masm.loadValue(
      BaseValueIndex(BaselineFrameReg, index, BaselineFrame::offsetOfArg(0)),
      output.valueReg());
  return true;
||||||| merged common ancestors
    FailurePath* failure;
    if (!addFailurePath(&failure)) {
        return false;
    }

    // Bounds check.
    masm.loadPtr(Address(BaselineFrameReg, BaselineFrame::offsetOfNumActualArgs()), scratch1);
    masm.spectreBoundsCheck32(index, scratch1, scratch2, failure->label());

    // Load the argument.
    masm.loadValue(BaseValueIndex(BaselineFrameReg, index, BaselineFrame::offsetOfArg(0)),
                   output.valueReg());
    return true;
=======
  masm.branchTest32(
      Assembler::NonZero,
      Address(BaselineFrameReg, BaselineFrame::reverseOffsetOfFlags()),
      Imm32(BaselineFrame::HAS_ARGS_OBJ), failure->label());
  return true;
>>>>>>> upstream-releases
}

<<<<<<< HEAD
bool BaselineCacheIRCompiler::emitLoadEnvironmentFixedSlotResult() {
  AutoOutputRegister output(*this);
  Register obj = allocator.useRegister(masm, reader.objOperandId());
  AutoScratchRegisterMaybeOutput scratch(allocator, masm, output);

  FailurePath* failure;
  if (!addFailurePath(&failure)) {
    return false;
  }
||||||| merged common ancestors
bool
BaselineCacheIRCompiler::emitLoadEnvironmentFixedSlotResult()
{
    AutoOutputRegister output(*this);
    Register obj = allocator.useRegister(masm, reader.objOperandId());
    AutoScratchRegisterMaybeOutput scratch(allocator, masm, output);

    FailurePath* failure;
    if (!addFailurePath(&failure)) {
        return false;
    }
=======
bool BaselineCacheIRCompiler::emitLoadFrameCalleeResult() {
  JitSpew(JitSpew_Codegen, __FUNCTION__);
  AutoOutputRegister output(*this);
  AutoScratchRegisterMaybeOutput scratch(allocator, masm, output);
>>>>>>> upstream-releases

<<<<<<< HEAD
  masm.load32(stubAddress(reader.stubOffset()), scratch);
  BaseIndex slot(obj, scratch, TimesOne);

  // Check for uninitialized lexicals.
  masm.branchTestMagic(Assembler::Equal, slot, failure->label());

  // Load the value.
  masm.loadValue(slot, output.valueReg());
  return true;
||||||| merged common ancestors
    masm.load32(stubAddress(reader.stubOffset()), scratch);
    BaseIndex slot(obj, scratch, TimesOne);

    // Check for uninitialized lexicals.
    masm.branchTestMagic(Assembler::Equal, slot, failure->label());

    // Load the value.
    masm.loadValue(slot, output.valueReg());
    return true;
=======
  Address callee(BaselineFrameReg, BaselineFrame::offsetOfCalleeToken());
  masm.loadFunctionFromCalleeToken(callee, scratch);
  masm.tagValue(JSVAL_TYPE_OBJECT, scratch, output.valueReg());
  return true;
>>>>>>> upstream-releases
}

<<<<<<< HEAD
bool BaselineCacheIRCompiler::emitLoadEnvironmentDynamicSlotResult() {
  AutoOutputRegister output(*this);
  Register obj = allocator.useRegister(masm, reader.objOperandId());
  AutoScratchRegister scratch(allocator, masm);
  AutoScratchRegisterMaybeOutput scratch2(allocator, masm, output);

  FailurePath* failure;
  if (!addFailurePath(&failure)) {
    return false;
  }

  masm.load32(stubAddress(reader.stubOffset()), scratch);
  masm.loadPtr(Address(obj, NativeObject::offsetOfSlots()), scratch2);

  // Check for uninitialized lexicals.
  BaseIndex slot(scratch2, scratch, TimesOne);
  masm.branchTestMagic(Assembler::Equal, slot, failure->label());

  // Load the value.
  masm.loadValue(slot, output.valueReg());
  return true;
}

bool BaselineCacheIRCompiler::emitLoadStringResult() {
  AutoOutputRegister output(*this);
  AutoScratchRegisterMaybeOutput scratch(allocator, masm, output);

  masm.loadPtr(stubAddress(reader.stubOffset()), scratch);
  masm.tagValue(JSVAL_TYPE_STRING, scratch, output.valueReg());
  return true;
}

bool BaselineCacheIRCompiler::emitCallStringSplitResult() {
  Register str = allocator.useRegister(masm, reader.stringOperandId());
  Register sep = allocator.useRegister(masm, reader.stringOperandId());
  Address groupAddr(stubAddress(reader.stubOffset()));

  AutoScratchRegister scratch(allocator, masm);
  allocator.discardStack(masm);

  AutoStubFrame stubFrame(*this);
  stubFrame.enter(masm, scratch);

  // Load the group in the scratch register.
  masm.loadPtr(groupAddr, scratch);

  masm.Push(Imm32(INT32_MAX));
  masm.Push(scratch);
  masm.Push(sep);
  masm.Push(str);

  if (!callVM(masm, StringSplitHelperInfo)) {
    return false;
  }

  stubFrame.leave(masm);
  return true;
}

bool BaselineCacheIRCompiler::emitCompareStringResult() {
  AutoOutputRegister output(*this);

  Register left = allocator.useRegister(masm, reader.stringOperandId());
  Register right = allocator.useRegister(masm, reader.stringOperandId());
  JSOp op = reader.jsop();

  AutoScratchRegisterMaybeOutput scratch(allocator, masm, output);

  FailurePath* failure;
  if (!addFailurePath(&failure)) {
    return false;
  }

  allocator.discardStack(masm);

  Label slow, done;
  masm.compareStrings(op, left, right, scratch, &slow);
  masm.jump(&done);
  masm.bind(&slow);
  {
    AutoStubFrame stubFrame(*this);
    stubFrame.enter(masm, scratch);

    masm.Push(right);
    masm.Push(left);

    if (!callVM(masm, (op == JSOP_EQ || op == JSOP_STRICTEQ)
                          ? StringsEqualInfo
                          : StringsNotEqualInfo)) {
      return false;
    }
    stubFrame.leave(masm);
    masm.mov(ReturnReg, scratch);
  }
  masm.bind(&done);
  masm.tagValue(JSVAL_TYPE_BOOLEAN, scratch, output.valueReg());
  return true;
}

bool BaselineCacheIRCompiler::callTypeUpdateIC(
    Register obj, ValueOperand val, Register scratch,
    LiveGeneralRegisterSet saveRegs) {
  // Ensure the stack is empty for the VM call below.
  allocator.discardStack(masm);

  // R0 contains the value that needs to be typechecked.
  MOZ_ASSERT(val == R0);
  MOZ_ASSERT(scratch == R1.scratchReg());

#if defined(JS_CODEGEN_X86) || defined(JS_CODEGEN_X64)
  static const bool CallClobbersTailReg = false;
#else
  static const bool CallClobbersTailReg = true;
#endif

  // Call the first type update stub.
  if (CallClobbersTailReg) {
    masm.push(ICTailCallReg);
  }
  masm.push(ICStubReg);
  masm.loadPtr(Address(ICStubReg, ICUpdatedStub::offsetOfFirstUpdateStub()),
               ICStubReg);
  masm.call(Address(ICStubReg, ICStub::offsetOfStubCode()));
  masm.pop(ICStubReg);
  if (CallClobbersTailReg) {
    masm.pop(ICTailCallReg);
  }

  // The update IC will store 0 or 1 in |scratch|, R1.scratchReg(), reflecting
  // if the value in R0 type-checked properly or not.
  Label done;
  masm.branch32(Assembler::Equal, scratch, Imm32(1), &done);

  AutoStubFrame stubFrame(*this);
  stubFrame.enter(masm, scratch, CallCanGC::CanNotGC);

  masm.PushRegsInMask(saveRegs);

  masm.Push(val);
  masm.Push(TypedOrValueRegister(MIRType::Object, AnyRegister(obj)));
  masm.Push(ICStubReg);

  // Load previous frame pointer, push BaselineFrame*.
  masm.loadPtr(Address(BaselineFrameReg, 0), scratch);
  masm.pushBaselineFramePtr(scratch, scratch);

  if (!callVM(masm, DoTypeUpdateFallbackInfo)) {
    return false;
  }

  masm.PopRegsInMask(saveRegs);

  stubFrame.leave(masm);

  masm.bind(&done);
  return true;
}

bool BaselineCacheIRCompiler::emitStoreSlotShared(bool isFixed) {
  ObjOperandId objId = reader.objOperandId();
  Address offsetAddr = stubAddress(reader.stubOffset());

  // Allocate the fixed registers first. These need to be fixed for
  // callTypeUpdateIC.
  AutoScratchRegister scratch1(allocator, masm, R1.scratchReg());
  ValueOperand val =
      allocator.useFixedValueRegister(masm, reader.valOperandId(), R0);

  Register obj = allocator.useRegister(masm, objId);
  Maybe<AutoScratchRegister> scratch2;
  if (!isFixed) {
    scratch2.emplace(allocator, masm);
  }

  LiveGeneralRegisterSet saveRegs;
  saveRegs.add(obj);
  saveRegs.add(val);
  if (!callTypeUpdateIC(obj, val, scratch1, saveRegs)) {
    return false;
  }

  masm.load32(offsetAddr, scratch1);

  if (isFixed) {
    BaseIndex slot(obj, scratch1, TimesOne);
    EmitPreBarrier(masm, slot, MIRType::Value);
    masm.storeValue(val, slot);
  } else {
    masm.loadPtr(Address(obj, NativeObject::offsetOfSlots()), scratch2.ref());
    BaseIndex slot(scratch2.ref(), scratch1, TimesOne);
    EmitPreBarrier(masm, slot, MIRType::Value);
    masm.storeValue(val, slot);
  }

  emitPostBarrierSlot(obj, val, scratch1);
  return true;
}

bool BaselineCacheIRCompiler::emitStoreFixedSlot() {
  return emitStoreSlotShared(true);
}

bool BaselineCacheIRCompiler::emitStoreDynamicSlot() {
  return emitStoreSlotShared(false);
}

bool BaselineCacheIRCompiler::emitAddAndStoreSlotShared(CacheOp op) {
  ObjOperandId objId = reader.objOperandId();
  Address offsetAddr = stubAddress(reader.stubOffset());

  // Allocate the fixed registers first. These need to be fixed for
  // callTypeUpdateIC.
  AutoScratchRegister scratch1(allocator, masm, R1.scratchReg());
  ValueOperand val =
      allocator.useFixedValueRegister(masm, reader.valOperandId(), R0);

  Register obj = allocator.useRegister(masm, objId);
  AutoScratchRegister scratch2(allocator, masm);

  bool changeGroup = reader.readBool();
  Address newGroupAddr = stubAddress(reader.stubOffset());
  Address newShapeAddr = stubAddress(reader.stubOffset());

  if (op == CacheOp::AllocateAndStoreDynamicSlot) {
    // We have to (re)allocate dynamic slots. Do this first, as it's the
    // only fallible operation here. This simplifies the callTypeUpdateIC
    // call below: it does not have to worry about saving registers used by
    // failure paths. Note that growSlotsPure is fallible but does
    // not GC.
    Address numNewSlotsAddr = stubAddress(reader.stubOffset());

    FailurePath* failure;
    if (!addFailurePath(&failure)) {
      return false;
    }

    LiveRegisterSet save(GeneralRegisterSet::Volatile(),
                         liveVolatileFloatRegs());
    masm.PushRegsInMask(save);

    masm.setupUnalignedABICall(scratch1);
    masm.loadJSContext(scratch1);
    masm.passABIArg(scratch1);
    masm.passABIArg(obj);
    masm.load32(numNewSlotsAddr, scratch2);
    masm.passABIArg(scratch2);
    masm.callWithABI(JS_FUNC_TO_DATA_PTR(void*, NativeObject::growSlotsPure));
    masm.mov(ReturnReg, scratch1);

    LiveRegisterSet ignore;
    ignore.add(scratch1);
    masm.PopRegsInMaskIgnore(save, ignore);

    masm.branchIfFalseBool(scratch1, failure->label());
  }

  LiveGeneralRegisterSet saveRegs;
  saveRegs.add(obj);
  saveRegs.add(val);
  if (!callTypeUpdateIC(obj, val, scratch1, saveRegs)) {
    return false;
  }

  if (changeGroup) {
    // Changing object's group from a partially to fully initialized group,
    // per the acquired properties analysis. Only change the group if the
    // old group still has a newScript. This only applies to PlainObjects.
    Label noGroupChange;
    masm.branchIfObjGroupHasNoAddendum(obj, scratch1, &noGroupChange);

    // Update the object's group.
    masm.loadPtr(newGroupAddr, scratch1);
    masm.storeObjGroup(scratch1, obj,
                       [](MacroAssembler& masm, const Address& addr) {
                         EmitPreBarrier(masm, addr, MIRType::ObjectGroup);
                       });

    masm.bind(&noGroupChange);
  }

  // Update the object's shape.
  masm.loadPtr(newShapeAddr, scratch1);
  masm.storeObjShape(scratch1, obj,
                     [](MacroAssembler& masm, const Address& addr) {
                       EmitPreBarrier(masm, addr, MIRType::Shape);
                     });

  // Perform the store. No pre-barrier required since this is a new
  // initialization.
  masm.load32(offsetAddr, scratch1);
  if (op == CacheOp::AddAndStoreFixedSlot) {
    BaseIndex slot(obj, scratch1, TimesOne);
    masm.storeValue(val, slot);
  } else {
    MOZ_ASSERT(op == CacheOp::AddAndStoreDynamicSlot ||
               op == CacheOp::AllocateAndStoreDynamicSlot);
    masm.loadPtr(Address(obj, NativeObject::offsetOfSlots()), scratch2);
    BaseIndex slot(scratch2, scratch1, TimesOne);
    masm.storeValue(val, slot);
  }

  emitPostBarrierSlot(obj, val, scratch1);
  return true;
}

bool BaselineCacheIRCompiler::emitAddAndStoreFixedSlot() {
  return emitAddAndStoreSlotShared(CacheOp::AddAndStoreFixedSlot);
}

bool BaselineCacheIRCompiler::emitAddAndStoreDynamicSlot() {
  return emitAddAndStoreSlotShared(CacheOp::AddAndStoreDynamicSlot);
||||||| merged common ancestors
bool
BaselineCacheIRCompiler::emitLoadEnvironmentDynamicSlotResult()
{
    AutoOutputRegister output(*this);
    Register obj = allocator.useRegister(masm, reader.objOperandId());
    AutoScratchRegister scratch(allocator, masm);
    AutoScratchRegisterMaybeOutput scratch2(allocator, masm, output);

    FailurePath* failure;
    if (!addFailurePath(&failure)) {
        return false;
    }

    masm.load32(stubAddress(reader.stubOffset()), scratch);
    masm.loadPtr(Address(obj, NativeObject::offsetOfSlots()), scratch2);

    // Check for uninitialized lexicals.
    BaseIndex slot(scratch2, scratch, TimesOne);
    masm.branchTestMagic(Assembler::Equal, slot, failure->label());

    // Load the value.
    masm.loadValue(slot, output.valueReg());
    return true;
}

bool
BaselineCacheIRCompiler::emitLoadStringResult()
{
    AutoOutputRegister output(*this);
    AutoScratchRegisterMaybeOutput scratch(allocator, masm, output);

    masm.loadPtr(stubAddress(reader.stubOffset()), scratch);
    masm.tagValue(JSVAL_TYPE_STRING, scratch, output.valueReg());
    return true;
}

bool
BaselineCacheIRCompiler::emitCallStringSplitResult()
{
    Register str = allocator.useRegister(masm, reader.stringOperandId());
    Register sep = allocator.useRegister(masm, reader.stringOperandId());
    Address groupAddr(stubAddress(reader.stubOffset()));

    AutoScratchRegister scratch(allocator, masm);
    allocator.discardStack(masm);

    AutoStubFrame stubFrame(*this);
    stubFrame.enter(masm, scratch);

    // Load the group in the scratch register.
    masm.loadPtr(groupAddr, scratch);

    masm.Push(Imm32(INT32_MAX));
    masm.Push(scratch);
    masm.Push(sep);
    masm.Push(str);

    if (!callVM(masm, StringSplitHelperInfo)) {
        return false;
    }

    stubFrame.leave(masm);
    return true;
}

bool
BaselineCacheIRCompiler::emitCompareStringResult()
{
    AutoOutputRegister output(*this);

    Register left = allocator.useRegister(masm, reader.stringOperandId());
    Register right = allocator.useRegister(masm, reader.stringOperandId());
    JSOp op = reader.jsop();

    AutoScratchRegisterMaybeOutput scratch(allocator, masm, output);

    FailurePath* failure;
    if (!addFailurePath(&failure)) {
        return false;
    }

    allocator.discardStack(masm);

    Label slow, done;
    masm.compareStrings(op, left, right, scratch, &slow);
    masm.jump(&done);
    masm.bind(&slow);
    {
        AutoStubFrame stubFrame(*this);
        stubFrame.enter(masm, scratch);

        masm.Push(right);
        masm.Push(left);

        if (!callVM(masm, (op == JSOP_EQ || op == JSOP_STRICTEQ) ?
                             StringsEqualInfo :
                             StringsNotEqualInfo))
        {
            return false;
        }
        stubFrame.leave(masm);
        masm.mov(ReturnReg, scratch);
    }
    masm.bind(&done);
    masm.tagValue(JSVAL_TYPE_BOOLEAN, scratch, output.valueReg());
    return true;
}

bool
BaselineCacheIRCompiler::callTypeUpdateIC(Register obj, ValueOperand val, Register scratch,
                                          LiveGeneralRegisterSet saveRegs)
{
    // Ensure the stack is empty for the VM call below.
    allocator.discardStack(masm);

    // R0 contains the value that needs to be typechecked.
    MOZ_ASSERT(val == R0);
    MOZ_ASSERT(scratch == R1.scratchReg());

#if defined(JS_CODEGEN_X86) || defined(JS_CODEGEN_X64)
    static const bool CallClobbersTailReg = false;
#else
    static const bool CallClobbersTailReg = true;
#endif

    // Call the first type update stub.
    if (CallClobbersTailReg) {
        masm.push(ICTailCallReg);
    }
    masm.push(ICStubReg);
    masm.loadPtr(Address(ICStubReg, ICUpdatedStub::offsetOfFirstUpdateStub()),
                 ICStubReg);
    masm.call(Address(ICStubReg, ICStub::offsetOfStubCode()));
    masm.pop(ICStubReg);
    if (CallClobbersTailReg) {
        masm.pop(ICTailCallReg);
    }

    // The update IC will store 0 or 1 in |scratch|, R1.scratchReg(), reflecting
    // if the value in R0 type-checked properly or not.
    Label done;
    masm.branch32(Assembler::Equal, scratch, Imm32(1), &done);

    AutoStubFrame stubFrame(*this);
    stubFrame.enter(masm, scratch, CallCanGC::CanNotGC);

    masm.PushRegsInMask(saveRegs);

    masm.Push(val);
    masm.Push(TypedOrValueRegister(MIRType::Object, AnyRegister(obj)));
    masm.Push(ICStubReg);

    // Load previous frame pointer, push BaselineFrame*.
    masm.loadPtr(Address(BaselineFrameReg, 0), scratch);
    masm.pushBaselineFramePtr(scratch, scratch);

    if (!callVM(masm, DoTypeUpdateFallbackInfo)) {
        return false;
    }

    masm.PopRegsInMask(saveRegs);

    stubFrame.leave(masm);

    masm.bind(&done);
    return true;
}

bool
BaselineCacheIRCompiler::emitStoreSlotShared(bool isFixed)
{
    ObjOperandId objId = reader.objOperandId();
    Address offsetAddr = stubAddress(reader.stubOffset());

    // Allocate the fixed registers first. These need to be fixed for
    // callTypeUpdateIC.
    AutoScratchRegister scratch1(allocator, masm, R1.scratchReg());
    ValueOperand val = allocator.useFixedValueRegister(masm, reader.valOperandId(), R0);

    Register obj = allocator.useRegister(masm, objId);
    Maybe<AutoScratchRegister> scratch2;
    if (!isFixed) {
        scratch2.emplace(allocator, masm);
    }

    LiveGeneralRegisterSet saveRegs;
    saveRegs.add(obj);
    saveRegs.add(val);
    if (!callTypeUpdateIC(obj, val, scratch1, saveRegs)) {
        return false;
    }

    masm.load32(offsetAddr, scratch1);

    if (isFixed) {
        BaseIndex slot(obj, scratch1, TimesOne);
        EmitPreBarrier(masm, slot, MIRType::Value);
        masm.storeValue(val, slot);
    } else {
        masm.loadPtr(Address(obj, NativeObject::offsetOfSlots()), scratch2.ref());
        BaseIndex slot(scratch2.ref(), scratch1, TimesOne);
        EmitPreBarrier(masm, slot, MIRType::Value);
        masm.storeValue(val, slot);
    }

    emitPostBarrierSlot(obj, val, scratch1);
    return true;
}

bool
BaselineCacheIRCompiler::emitStoreFixedSlot()
{
    return emitStoreSlotShared(true);
}

bool
BaselineCacheIRCompiler::emitStoreDynamicSlot()
{
    return emitStoreSlotShared(false);
}

bool
BaselineCacheIRCompiler::emitAddAndStoreSlotShared(CacheOp op)
{
    ObjOperandId objId = reader.objOperandId();
    Address offsetAddr = stubAddress(reader.stubOffset());

    // Allocate the fixed registers first. These need to be fixed for
    // callTypeUpdateIC.
    AutoScratchRegister scratch1(allocator, masm, R1.scratchReg());
    ValueOperand val = allocator.useFixedValueRegister(masm, reader.valOperandId(), R0);

    Register obj = allocator.useRegister(masm, objId);
    AutoScratchRegister scratch2(allocator, masm);

    bool changeGroup = reader.readBool();
    Address newGroupAddr = stubAddress(reader.stubOffset());
    Address newShapeAddr = stubAddress(reader.stubOffset());

    if (op == CacheOp::AllocateAndStoreDynamicSlot) {
        // We have to (re)allocate dynamic slots. Do this first, as it's the
        // only fallible operation here. This simplifies the callTypeUpdateIC
        // call below: it does not have to worry about saving registers used by
        // failure paths. Note that growSlotsPure is fallible but does
        // not GC.
        Address numNewSlotsAddr = stubAddress(reader.stubOffset());

        FailurePath* failure;
        if (!addFailurePath(&failure)) {
            return false;
        }

        LiveRegisterSet save(GeneralRegisterSet::Volatile(), liveVolatileFloatRegs());
        masm.PushRegsInMask(save);

        masm.setupUnalignedABICall(scratch1);
        masm.loadJSContext(scratch1);
        masm.passABIArg(scratch1);
        masm.passABIArg(obj);
        masm.load32(numNewSlotsAddr, scratch2);
        masm.passABIArg(scratch2);
        masm.callWithABI(JS_FUNC_TO_DATA_PTR(void*, NativeObject::growSlotsPure));
        masm.mov(ReturnReg, scratch1);

        LiveRegisterSet ignore;
        ignore.add(scratch1);
        masm.PopRegsInMaskIgnore(save, ignore);

        masm.branchIfFalseBool(scratch1, failure->label());
    }

    LiveGeneralRegisterSet saveRegs;
    saveRegs.add(obj);
    saveRegs.add(val);
    if (!callTypeUpdateIC(obj, val, scratch1, saveRegs)) {
        return false;
    }

    if (changeGroup) {
        // Changing object's group from a partially to fully initialized group,
        // per the acquired properties analysis. Only change the group if the
        // old group still has a newScript. This only applies to PlainObjects.
        Label noGroupChange;
        masm.branchIfObjGroupHasNoAddendum(obj, scratch1, &noGroupChange);

        // Update the object's group.
        masm.loadPtr(newGroupAddr, scratch1);
        masm.storeObjGroup(scratch1, obj, [](MacroAssembler& masm, const Address& addr) {
            EmitPreBarrier(masm, addr, MIRType::ObjectGroup);
        });

        masm.bind(&noGroupChange);
    }

    // Update the object's shape.
    masm.loadPtr(newShapeAddr, scratch1);
    masm.storeObjShape(scratch1, obj, [](MacroAssembler& masm, const Address& addr) {
        EmitPreBarrier(masm, addr, MIRType::Shape);
    });

    // Perform the store. No pre-barrier required since this is a new
    // initialization.
    masm.load32(offsetAddr, scratch1);
    if (op == CacheOp::AddAndStoreFixedSlot) {
        BaseIndex slot(obj, scratch1, TimesOne);
        masm.storeValue(val, slot);
    } else {
        MOZ_ASSERT(op == CacheOp::AddAndStoreDynamicSlot ||
                   op == CacheOp::AllocateAndStoreDynamicSlot);
        masm.loadPtr(Address(obj, NativeObject::offsetOfSlots()), scratch2);
        BaseIndex slot(scratch2, scratch1, TimesOne);
        masm.storeValue(val, slot);
    }

    emitPostBarrierSlot(obj, val, scratch1);
    return true;
}

bool
BaselineCacheIRCompiler::emitAddAndStoreFixedSlot()
{
    return emitAddAndStoreSlotShared(CacheOp::AddAndStoreFixedSlot);
}

bool
BaselineCacheIRCompiler::emitAddAndStoreDynamicSlot()
{
    return emitAddAndStoreSlotShared(CacheOp::AddAndStoreDynamicSlot);
}

bool
BaselineCacheIRCompiler::emitAllocateAndStoreDynamicSlot()
{
    return emitAddAndStoreSlotShared(CacheOp::AllocateAndStoreDynamicSlot);
}

bool
BaselineCacheIRCompiler::emitStoreUnboxedProperty()
{
    ObjOperandId objId = reader.objOperandId();
    JSValueType fieldType = reader.valueType();
    Address offsetAddr = stubAddress(reader.stubOffset());

    // Allocate the fixed registers first. These need to be fixed for
    // callTypeUpdateIC.
    AutoScratchRegister scratch(allocator, masm, R1.scratchReg());
    ValueOperand val = allocator.useFixedValueRegister(masm, reader.valOperandId(), R0);

    Register obj = allocator.useRegister(masm, objId);

    // We only need the type update IC if we are storing an object.
    if (fieldType == JSVAL_TYPE_OBJECT) {
        LiveGeneralRegisterSet saveRegs;
        saveRegs.add(obj);
        saveRegs.add(val);
        if (!callTypeUpdateIC(obj, val, scratch, saveRegs)) {
            return false;
        }
    }

    masm.load32(offsetAddr, scratch);
    BaseIndex fieldAddr(obj, scratch, TimesOne);

    // Note that the storeUnboxedProperty call here is infallible, as the
    // IR emitter is responsible for guarding on |val|'s type.
    EmitICUnboxedPreBarrier(masm, fieldAddr, fieldType);
    masm.storeUnboxedProperty(fieldAddr, fieldType,
                              ConstantOrRegister(TypedOrValueRegister(val)),
                              /* failure = */ nullptr);

    if (UnboxedTypeNeedsPostBarrier(fieldType)) {
        emitPostBarrierSlot(obj, val, scratch);
    }
    return true;
}

bool
BaselineCacheIRCompiler::emitStoreTypedObjectReferenceProperty()
{
    ObjOperandId objId = reader.objOperandId();
    Address offsetAddr = stubAddress(reader.stubOffset());
    TypedThingLayout layout = reader.typedThingLayout();
    ReferenceType type = reader.referenceTypeDescrType();

    // Allocate the fixed registers first. These need to be fixed for
    // callTypeUpdateIC.
    AutoScratchRegister scratch1(allocator, masm, R1.scratchReg());
    ValueOperand val = allocator.useFixedValueRegister(masm, reader.valOperandId(), R0);

    Register obj = allocator.useRegister(masm, objId);
    AutoScratchRegister scratch2(allocator, masm);

    // We don't need a type update IC if the property is always a string.
    if (type != ReferenceType::TYPE_STRING) {
        LiveGeneralRegisterSet saveRegs;
        saveRegs.add(obj);
        saveRegs.add(val);
        if (!callTypeUpdateIC(obj, val, scratch1, saveRegs)) {
            return false;
        }
    }

    // Compute the address being written to.
    LoadTypedThingData(masm, layout, obj, scratch1);
    masm.addPtr(offsetAddr, scratch1);
    Address dest(scratch1, 0);

    emitStoreTypedObjectReferenceProp(val, type, dest, scratch2);
    emitPostBarrierSlot(obj, val, scratch1);

    return true;
=======
bool BaselineCacheIRCompiler::emitLoadFrameNumActualArgsResult() {
  JitSpew(JitSpew_Codegen, __FUNCTION__);
  AutoOutputRegister output(*this);
  AutoScratchRegisterMaybeOutput scratch(allocator, masm, output);

  Address actualArgs(BaselineFrameReg, BaselineFrame::offsetOfNumActualArgs());
  masm.loadPtr(actualArgs, scratch);
  masm.tagValue(JSVAL_TYPE_INT32, scratch, output.valueReg());
  return true;
}

bool BaselineCacheIRCompiler::emitLoadTypedObjectResult() {
  JitSpew(JitSpew_Codegen, __FUNCTION__);
  AutoOutputRegister output(*this);
  Register obj = allocator.useRegister(masm, reader.objOperandId());
  AutoScratchRegister scratch1(allocator, masm);
  AutoScratchRegister scratch2(allocator, masm);

  TypedThingLayout layout = reader.typedThingLayout();
  uint32_t typeDescr = reader.typeDescrKey();
  Address fieldOffset(stubAddress(reader.stubOffset()));

  // Get the object's data pointer.
  LoadTypedThingData(masm, layout, obj, scratch1);

  // Get the address being written to.
  masm.load32(fieldOffset, scratch2);
  masm.addPtr(scratch2, scratch1);

  Address fieldAddr(scratch1, 0);
  emitLoadTypedObjectResultShared(fieldAddr, scratch2, typeDescr, output);
  return true;
>>>>>>> upstream-releases
}

<<<<<<< HEAD
bool BaselineCacheIRCompiler::emitAllocateAndStoreDynamicSlot() {
  return emitAddAndStoreSlotShared(CacheOp::AllocateAndStoreDynamicSlot);
||||||| merged common ancestors
bool
BaselineCacheIRCompiler::emitStoreTypedObjectScalarProperty()
{
    Register obj = allocator.useRegister(masm, reader.objOperandId());
    Address offsetAddr = stubAddress(reader.stubOffset());
    TypedThingLayout layout = reader.typedThingLayout();
    Scalar::Type type = reader.scalarType();
    ValueOperand val = allocator.useValueRegister(masm, reader.valOperandId());
    AutoScratchRegister scratch1(allocator, masm);
    AutoScratchRegister scratch2(allocator, masm);

    FailurePath* failure;
    if (!addFailurePath(&failure)) {
        return false;
    }

    // Compute the address being written to.
    LoadTypedThingData(masm, layout, obj, scratch1);
    masm.addPtr(offsetAddr, scratch1);
    Address dest(scratch1, 0);

    StoreToTypedArray(cx_, masm, type, val, dest, scratch2, failure->label());
    return true;
=======
bool BaselineCacheIRCompiler::emitLoadFrameArgumentResult() {
  JitSpew(JitSpew_Codegen, __FUNCTION__);
  AutoOutputRegister output(*this);
  Register index = allocator.useRegister(masm, reader.int32OperandId());
  AutoScratchRegister scratch1(allocator, masm);
  AutoScratchRegisterMaybeOutput scratch2(allocator, masm, output);

  FailurePath* failure;
  if (!addFailurePath(&failure)) {
    return false;
  }

  // Bounds check.
  masm.loadPtr(
      Address(BaselineFrameReg, BaselineFrame::offsetOfNumActualArgs()),
      scratch1);
  masm.spectreBoundsCheck32(index, scratch1, scratch2, failure->label());

  // Load the argument.
  masm.loadValue(
      BaseValueIndex(BaselineFrameReg, index, BaselineFrame::offsetOfArg(0)),
      output.valueReg());
  return true;
>>>>>>> upstream-releases
}

<<<<<<< HEAD
bool BaselineCacheIRCompiler::emitStoreUnboxedProperty() {
  ObjOperandId objId = reader.objOperandId();
  JSValueType fieldType = reader.valueType();
  Address offsetAddr = stubAddress(reader.stubOffset());
||||||| merged common ancestors
bool
BaselineCacheIRCompiler::emitStoreDenseElement()
{
    ObjOperandId objId = reader.objOperandId();
    Int32OperandId indexId = reader.int32OperandId();

    // Allocate the fixed registers first. These need to be fixed for
    // callTypeUpdateIC.
    AutoScratchRegister scratch(allocator, masm, R1.scratchReg());
    ValueOperand val = allocator.useFixedValueRegister(masm, reader.valOperandId(), R0);

    Register obj = allocator.useRegister(masm, objId);
    Register index = allocator.useRegister(masm, indexId);

    FailurePath* failure;
    if (!addFailurePath(&failure)) {
        return false;
    }

    // Load obj->elements in scratch.
    masm.loadPtr(Address(obj, NativeObject::offsetOfElements()), scratch);

    // Bounds check. Unfortunately we don't have more registers available on
    // x86, so use InvalidReg and emit slightly slower code on x86.
    Register spectreTemp = InvalidReg;
    Address initLength(scratch, ObjectElements::offsetOfInitializedLength());
    masm.spectreBoundsCheck32(index, initLength, spectreTemp, failure->label());
=======
bool BaselineCacheIRCompiler::emitLoadEnvironmentFixedSlotResult() {
  JitSpew(JitSpew_Codegen, __FUNCTION__);
  AutoOutputRegister output(*this);
  Register obj = allocator.useRegister(masm, reader.objOperandId());
  AutoScratchRegisterMaybeOutput scratch(allocator, masm, output);

  FailurePath* failure;
  if (!addFailurePath(&failure)) {
    return false;
  }

  masm.load32(stubAddress(reader.stubOffset()), scratch);
  BaseIndex slot(obj, scratch, TimesOne);

  // Check for uninitialized lexicals.
  masm.branchTestMagic(Assembler::Equal, slot, failure->label());
>>>>>>> upstream-releases

<<<<<<< HEAD
  // Allocate the fixed registers first. These need to be fixed for
  // callTypeUpdateIC.
  AutoScratchRegister scratch(allocator, masm, R1.scratchReg());
  ValueOperand val =
      allocator.useFixedValueRegister(masm, reader.valOperandId(), R0);
||||||| merged common ancestors
    // Hole check.
    BaseObjectElementIndex element(scratch, index);
    masm.branchTestMagic(Assembler::Equal, element, failure->label());

    // Perform a single test to see if we either need to convert double
    // elements, clone the copy on write elements in the object or fail
    // due to a frozen element.
    Label noSpecialHandling;
    Address elementsFlags(scratch, ObjectElements::offsetOfFlags());
    masm.branchTest32(Assembler::Zero, elementsFlags,
                      Imm32(ObjectElements::CONVERT_DOUBLE_ELEMENTS |
                            ObjectElements::COPY_ON_WRITE |
                            ObjectElements::FROZEN),
                      &noSpecialHandling);

    // Fail if we need to clone copy on write elements or to throw due
    // to a frozen element.
    masm.branchTest32(Assembler::NonZero, elementsFlags,
                      Imm32(ObjectElements::COPY_ON_WRITE |
                            ObjectElements::FROZEN),
                      failure->label());

    // We need to convert int32 values being stored into doubles. Note that
    // double arrays are only created by IonMonkey, so if we have no FP support
    // Ion is disabled and there should be no double arrays.
    if (cx_->runtime()->jitSupportsFloatingPoint) {
        // It's fine to convert the value in place in Baseline. We can't do
        // this in Ion.
        masm.convertInt32ValueToDouble(val);
    } else {
        masm.assumeUnreachable("There shouldn't be double arrays when there is no FP support.");
    }
=======
  // Load the value.
  masm.loadValue(slot, output.valueReg());
  return true;
}

bool BaselineCacheIRCompiler::emitLoadEnvironmentDynamicSlotResult() {
  JitSpew(JitSpew_Codegen, __FUNCTION__);
  AutoOutputRegister output(*this);
  Register obj = allocator.useRegister(masm, reader.objOperandId());
  AutoScratchRegister scratch(allocator, masm);
  AutoScratchRegisterMaybeOutput scratch2(allocator, masm, output);
>>>>>>> upstream-releases

<<<<<<< HEAD
  Register obj = allocator.useRegister(masm, objId);
||||||| merged common ancestors
    masm.bind(&noSpecialHandling);
=======
  FailurePath* failure;
  if (!addFailurePath(&failure)) {
    return false;
  }
>>>>>>> upstream-releases

<<<<<<< HEAD
  // We only need the type update IC if we are storing an object.
  if (fieldType == JSVAL_TYPE_OBJECT) {
    LiveGeneralRegisterSet saveRegs;
    saveRegs.add(obj);
    saveRegs.add(val);
    if (!callTypeUpdateIC(obj, val, scratch, saveRegs)) {
      return false;
    }
  }
||||||| merged common ancestors
    // Call the type update IC. After this everything must be infallible as we
    // don't save all registers here.
    LiveGeneralRegisterSet saveRegs;
    saveRegs.add(obj);
    saveRegs.add(index);
    saveRegs.add(val);
    if (!callTypeUpdateIC(obj, val, scratch, saveRegs)) {
        return false;
    }

    // Perform the store. Reload obj->elements because callTypeUpdateIC
    // used the scratch register.
    masm.loadPtr(Address(obj, NativeObject::offsetOfElements()), scratch);
    EmitPreBarrier(masm, element, MIRType::Value);
    masm.storeValue(val, element);

    emitPostBarrierElement(obj, val, scratch, index);
    return true;
}

bool
BaselineCacheIRCompiler::emitStoreDenseElementHole()
{
    ObjOperandId objId = reader.objOperandId();
    Int32OperandId indexId = reader.int32OperandId();

    // Allocate the fixed registers first. These need to be fixed for
    // callTypeUpdateIC.
    AutoScratchRegister scratch(allocator, masm, R1.scratchReg());
    ValueOperand val = allocator.useFixedValueRegister(masm, reader.valOperandId(), R0);

    Register obj = allocator.useRegister(masm, objId);
    Register index = allocator.useRegister(masm, indexId);

    bool handleAdd = reader.readBool();

    FailurePath* failure;
    if (!addFailurePath(&failure)) {
        return false;
    }
=======
  masm.load32(stubAddress(reader.stubOffset()), scratch);
  masm.loadPtr(Address(obj, NativeObject::offsetOfSlots()), scratch2);

  // Check for uninitialized lexicals.
  BaseIndex slot(scratch2, scratch, TimesOne);
  masm.branchTestMagic(Assembler::Equal, slot, failure->label());

  // Load the value.
  masm.loadValue(slot, output.valueReg());
  return true;
}

bool BaselineCacheIRCompiler::emitLoadStringResult() {
  JitSpew(JitSpew_Codegen, __FUNCTION__);
  AutoOutputRegister output(*this);
  AutoScratchRegisterMaybeOutput scratch(allocator, masm, output);

  masm.loadPtr(stubAddress(reader.stubOffset()), scratch);
  masm.tagValue(JSVAL_TYPE_STRING, scratch, output.valueReg());
  return true;
}

bool BaselineCacheIRCompiler::emitCompareStringResult() {
  JitSpew(JitSpew_Codegen, __FUNCTION__);
  AutoOutputRegister output(*this);

  Register left = allocator.useRegister(masm, reader.stringOperandId());
  Register right = allocator.useRegister(masm, reader.stringOperandId());
  JSOp op = reader.jsop();

  AutoScratchRegisterMaybeOutput scratch(allocator, masm, output);
>>>>>>> upstream-releases

<<<<<<< HEAD
  masm.load32(offsetAddr, scratch);
  BaseIndex fieldAddr(obj, scratch, TimesOne);
||||||| merged common ancestors
    // Load obj->elements in scratch.
    masm.loadPtr(Address(obj, NativeObject::offsetOfElements()), scratch);
=======
  FailurePath* failure;
  if (!addFailurePath(&failure)) {
    return false;
  }
>>>>>>> upstream-releases

<<<<<<< HEAD
  // Note that the storeUnboxedProperty call here is infallible, as the
  // IR emitter is responsible for guarding on |val|'s type.
  EmitICUnboxedPreBarrier(masm, fieldAddr, fieldType);
  masm.storeUnboxedProperty(fieldAddr, fieldType,
                            ConstantOrRegister(TypedOrValueRegister(val)),
                            /* failure = */ nullptr);
||||||| merged common ancestors
    BaseObjectElementIndex element(scratch, index);
    Address initLength(scratch, ObjectElements::offsetOfInitializedLength());
    Address elementsFlags(scratch, ObjectElements::offsetOfFlags());
=======
  allocator.discardStack(masm);
>>>>>>> upstream-releases

<<<<<<< HEAD
  if (UnboxedTypeNeedsPostBarrier(fieldType)) {
    emitPostBarrierSlot(obj, val, scratch);
  }
  return true;
}
||||||| merged common ancestors
    // Check for copy-on-write elements. Note that this stub is not attached for
    // non-extensible objects, so the shape guard ensures there are no sealed or
    // frozen elements.
    masm.branchTest32(Assembler::NonZero, elementsFlags,
                      Imm32(ObjectElements::COPY_ON_WRITE),
                      failure->label());
=======
  Label slow, done;
  masm.compareStrings(op, left, right, scratch, &slow);
  masm.jump(&done);
  masm.bind(&slow);
  {
    AutoStubFrame stubFrame(*this);
    stubFrame.enter(masm, scratch);
>>>>>>> upstream-releases

<<<<<<< HEAD
bool BaselineCacheIRCompiler::emitStoreTypedObjectReferenceProperty() {
  ObjOperandId objId = reader.objOperandId();
  Address offsetAddr = stubAddress(reader.stubOffset());
  TypedThingLayout layout = reader.typedThingLayout();
  ReferenceType type = reader.referenceTypeDescrType();
||||||| merged common ancestors
    // We don't have enough registers on x86 so use InvalidReg. This will emit
    // slightly less efficient code on x86.
    Register spectreTemp = InvalidReg;

    if (handleAdd) {
        // Bounds check.
        Label capacityOk, outOfBounds;
        masm.spectreBoundsCheck32(index, initLength, spectreTemp, &outOfBounds);
        masm.jump(&capacityOk);

        // If we're out-of-bounds, only handle the index == initLength case.
        masm.bind(&outOfBounds);
        masm.branch32(Assembler::NotEqual, initLength, index, failure->label());

        // If index < capacity, we can add a dense element inline. If not we
        // need to allocate more elements.
        Label allocElement;
        Address capacity(scratch, ObjectElements::offsetOfCapacity());
        masm.spectreBoundsCheck32(index, capacity, spectreTemp, &allocElement);
        masm.jump(&capacityOk);

        // Check for non-writable array length. We only have to do this if
        // index >= capacity.
        masm.bind(&allocElement);
        masm.branchTest32(Assembler::NonZero, elementsFlags,
                          Imm32(ObjectElements::NONWRITABLE_ARRAY_LENGTH),
                          failure->label());

        LiveRegisterSet save(GeneralRegisterSet::Volatile(), liveVolatileFloatRegs());
        save.takeUnchecked(scratch);
        masm.PushRegsInMask(save);

        masm.setupUnalignedABICall(scratch);
        masm.loadJSContext(scratch);
        masm.passABIArg(scratch);
        masm.passABIArg(obj);
        masm.callWithABI(JS_FUNC_TO_DATA_PTR(void*, NativeObject::addDenseElementPure));
        masm.mov(ReturnReg, scratch);

        masm.PopRegsInMask(save);
        masm.branchIfFalseBool(scratch, failure->label());

        // Load the reallocated elements pointer.
        masm.loadPtr(Address(obj, NativeObject::offsetOfElements()), scratch);

        masm.bind(&capacityOk);

        // We increment initLength after the callTypeUpdateIC call, to ensure
        // the type update code doesn't read uninitialized memory.
    } else {
        // Fail if index >= initLength.
        masm.spectreBoundsCheck32(index, initLength, spectreTemp, failure->label());
    }
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
>>>>>>> upstream-releases

<<<<<<< HEAD
  // Allocate the fixed registers first. These need to be fixed for
  // callTypeUpdateIC.
  AutoScratchRegister scratch1(allocator, masm, R1.scratchReg());
  ValueOperand val =
      allocator.useFixedValueRegister(masm, reader.valOperandId(), R0);
||||||| merged common ancestors
    // Check if we have to convert a double element.
    Label noConversion;
    masm.branchTest32(Assembler::Zero, elementsFlags,
                      Imm32(ObjectElements::CONVERT_DOUBLE_ELEMENTS),
                      &noConversion);

    // We need to convert int32 values being stored into doubles. Note that
    // double arrays are only created by IonMonkey, so if we have no FP support
    // Ion is disabled and there should be no double arrays.
    if (cx_->runtime()->jitSupportsFloatingPoint) {
        // It's fine to convert the value in place in Baseline. We can't do
        // this in Ion.
        masm.convertInt32ValueToDouble(val);
    } else {
        masm.assumeUnreachable("There shouldn't be double arrays when there is no FP support.");
    }
=======
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
>>>>>>> upstream-releases

<<<<<<< HEAD
  Register obj = allocator.useRegister(masm, objId);
  AutoScratchRegister scratch2(allocator, masm);
||||||| merged common ancestors
    masm.bind(&noConversion);
=======
    stubFrame.leave(masm);
    masm.mov(ReturnReg, scratch);
  }
  masm.bind(&done);
  masm.tagValue(JSVAL_TYPE_BOOLEAN, scratch, output.valueReg());
  return true;
}

bool BaselineCacheIRCompiler::callTypeUpdateIC(
    Register obj, ValueOperand val, Register scratch,
    LiveGeneralRegisterSet saveRegs) {
  // Ensure the stack is empty for the VM call below.
  allocator.discardStack(masm);
>>>>>>> upstream-releases

<<<<<<< HEAD
  // We don't need a type update IC if the property is always a string.
  if (type != ReferenceType::TYPE_STRING) {
    LiveGeneralRegisterSet saveRegs;
    saveRegs.add(obj);
    saveRegs.add(val);
    if (!callTypeUpdateIC(obj, val, scratch1, saveRegs)) {
      return false;
    }
  }

  // Compute the address being written to.
  LoadTypedThingData(masm, layout, obj, scratch1);
  masm.addPtr(offsetAddr, scratch1);
  Address dest(scratch1, 0);

  emitStoreTypedObjectReferenceProp(val, type, dest, scratch2);
  emitPostBarrierSlot(obj, val, scratch1);

  return true;
}

bool BaselineCacheIRCompiler::emitStoreTypedObjectScalarProperty() {
  Register obj = allocator.useRegister(masm, reader.objOperandId());
  Address offsetAddr = stubAddress(reader.stubOffset());
  TypedThingLayout layout = reader.typedThingLayout();
  Scalar::Type type = reader.scalarType();
  ValueOperand val = allocator.useValueRegister(masm, reader.valOperandId());
  AutoScratchRegister scratch1(allocator, masm);
  AutoScratchRegister scratch2(allocator, masm);

  FailurePath* failure;
  if (!addFailurePath(&failure)) {
    return false;
  }

  // Compute the address being written to.
  LoadTypedThingData(masm, layout, obj, scratch1);
  masm.addPtr(offsetAddr, scratch1);
  Address dest(scratch1, 0);

  StoreToTypedArray(cx_, masm, type, val, dest, scratch2, failure->label());
  return true;
}

bool BaselineCacheIRCompiler::emitStoreDenseElement() {
  ObjOperandId objId = reader.objOperandId();
  Int32OperandId indexId = reader.int32OperandId();

  // Allocate the fixed registers first. These need to be fixed for
  // callTypeUpdateIC.
  AutoScratchRegister scratch(allocator, masm, R1.scratchReg());
  ValueOperand val =
      allocator.useFixedValueRegister(masm, reader.valOperandId(), R0);

  Register obj = allocator.useRegister(masm, objId);
  Register index = allocator.useRegister(masm, indexId);

  FailurePath* failure;
  if (!addFailurePath(&failure)) {
    return false;
  }

  // Load obj->elements in scratch.
  masm.loadPtr(Address(obj, NativeObject::offsetOfElements()), scratch);

  // Bounds check. Unfortunately we don't have more registers available on
  // x86, so use InvalidReg and emit slightly slower code on x86.
  Register spectreTemp = InvalidReg;
  Address initLength(scratch, ObjectElements::offsetOfInitializedLength());
  masm.spectreBoundsCheck32(index, initLength, spectreTemp, failure->label());

  // Hole check.
  BaseObjectElementIndex element(scratch, index);
  masm.branchTestMagic(Assembler::Equal, element, failure->label());

  // Perform a single test to see if we either need to convert double
  // elements, clone the copy on write elements in the object or fail
  // due to a frozen element.
  Label noSpecialHandling;
  Address elementsFlags(scratch, ObjectElements::offsetOfFlags());
  masm.branchTest32(
      Assembler::Zero, elementsFlags,
      Imm32(ObjectElements::CONVERT_DOUBLE_ELEMENTS |
            ObjectElements::COPY_ON_WRITE | ObjectElements::FROZEN),
      &noSpecialHandling);

  // Fail if we need to clone copy on write elements or to throw due
  // to a frozen element.
  masm.branchTest32(
      Assembler::NonZero, elementsFlags,
      Imm32(ObjectElements::COPY_ON_WRITE | ObjectElements::FROZEN),
      failure->label());

  // We need to convert int32 values being stored into doubles. Note
  // that double arrays are only created by IonMonkey. It's fine to
  // convert the value in place in Baseline. We can't do this in
  // Ion.
  masm.convertInt32ValueToDouble(val);

  masm.bind(&noSpecialHandling);

  // Call the type update IC. After this everything must be infallible as we
  // don't save all registers here.
  LiveGeneralRegisterSet saveRegs;
  saveRegs.add(obj);
  saveRegs.add(index);
  saveRegs.add(val);
  if (!callTypeUpdateIC(obj, val, scratch, saveRegs)) {
    return false;
  }

  // Perform the store. Reload obj->elements because callTypeUpdateIC
  // used the scratch register.
  masm.loadPtr(Address(obj, NativeObject::offsetOfElements()), scratch);
  EmitPreBarrier(masm, element, MIRType::Value);
  masm.storeValue(val, element);

  emitPostBarrierElement(obj, val, scratch, index);
  return true;
}

bool BaselineCacheIRCompiler::emitStoreDenseElementHole() {
  ObjOperandId objId = reader.objOperandId();
  Int32OperandId indexId = reader.int32OperandId();

  // Allocate the fixed registers first. These need to be fixed for
  // callTypeUpdateIC.
  AutoScratchRegister scratch(allocator, masm, R1.scratchReg());
  ValueOperand val =
      allocator.useFixedValueRegister(masm, reader.valOperandId(), R0);

  Register obj = allocator.useRegister(masm, objId);
  Register index = allocator.useRegister(masm, indexId);

  bool handleAdd = reader.readBool();

  FailurePath* failure;
  if (!addFailurePath(&failure)) {
    return false;
  }

  // Load obj->elements in scratch.
  masm.loadPtr(Address(obj, NativeObject::offsetOfElements()), scratch);

  BaseObjectElementIndex element(scratch, index);
  Address initLength(scratch, ObjectElements::offsetOfInitializedLength());
  Address elementsFlags(scratch, ObjectElements::offsetOfFlags());

  // Check for copy-on-write elements. Note that this stub is not attached for
  // non-extensible objects, so the shape guard ensures there are no sealed or
  // frozen elements.
  masm.branchTest32(Assembler::NonZero, elementsFlags,
                    Imm32(ObjectElements::COPY_ON_WRITE), failure->label());

  // We don't have enough registers on x86 so use InvalidReg. This will emit
  // slightly less efficient code on x86.
  Register spectreTemp = InvalidReg;

  if (handleAdd) {
    // Bounds check.
    Label capacityOk, outOfBounds;
    masm.spectreBoundsCheck32(index, initLength, spectreTemp, &outOfBounds);
    masm.jump(&capacityOk);
||||||| merged common ancestors
    // Call the type update IC. After this everything must be infallible as we
    // don't save all registers here.
    LiveGeneralRegisterSet saveRegs;
    saveRegs.add(obj);
    saveRegs.add(index);
    saveRegs.add(val);
    if (!callTypeUpdateIC(obj, val, scratch, saveRegs)) {
        return false;
    }

    // Reload obj->elements as callTypeUpdateIC used the scratch register.
    masm.loadPtr(Address(obj, NativeObject::offsetOfElements()), scratch);

    Label doStore;
    if (handleAdd) {
        // If index == initLength, increment initLength.
        Label inBounds;
        masm.branch32(Assembler::NotEqual, initLength, index, &inBounds);

        // Increment initLength.
        masm.add32(Imm32(1), initLength);

        // If length is now <= index, increment length too.
        Label skipIncrementLength;
        Address length(scratch, ObjectElements::offsetOfLength());
        masm.branch32(Assembler::Above, length, index, &skipIncrementLength);
        masm.add32(Imm32(1), length);
        masm.bind(&skipIncrementLength);

        // Skip EmitPreBarrier as the memory is uninitialized.
        masm.jump(&doStore);

        masm.bind(&inBounds);
    }

    EmitPreBarrier(masm, element, MIRType::Value);

    masm.bind(&doStore);
    masm.storeValue(val, element);

    emitPostBarrierElement(obj, val, scratch, index);
    return true;
}

bool
BaselineCacheIRCompiler::emitArrayPush()
{
    ObjOperandId objId = reader.objOperandId();
    ValOperandId rhsId = reader.valOperandId();

    // Allocate the fixed registers first. These need to be fixed for
    // callTypeUpdateIC.
    AutoScratchRegister scratch(allocator, masm, R1.scratchReg());
    ValueOperand val = allocator.useFixedValueRegister(masm, rhsId, R0);

    Register obj = allocator.useRegister(masm, objId);
    AutoScratchRegister scratchLength(allocator, masm);

    FailurePath* failure;
    if (!addFailurePath(&failure)) {
        return false;
    }

    // Load obj->elements in scratch.
    masm.loadPtr(Address(obj, NativeObject::offsetOfElements()), scratch);

    Address elementsInitLength(scratch, ObjectElements::offsetOfInitializedLength());
    Address elementsLength(scratch, ObjectElements::offsetOfLength());
    Address elementsFlags(scratch, ObjectElements::offsetOfFlags());

    // Check for copy-on-write elements. Note that this stub is not attached for
    // non-extensible objects, so the shape guard ensures there are no sealed or
    // frozen elements.
    masm.branchTest32(Assembler::NonZero, elementsFlags,
                      Imm32(ObjectElements::COPY_ON_WRITE),
                      failure->label());
=======
  // R0 contains the value that needs to be typechecked.
  MOZ_ASSERT(val == R0);
  MOZ_ASSERT(scratch == R1.scratchReg());

#if defined(JS_CODEGEN_X86) || defined(JS_CODEGEN_X64)
  static const bool CallClobbersTailReg = false;
#else
  static const bool CallClobbersTailReg = true;
#endif

  // Call the first type update stub.
  if (CallClobbersTailReg) {
    masm.push(ICTailCallReg);
  }
  masm.push(ICStubReg);
  masm.loadPtr(Address(ICStubReg, ICCacheIR_Updated::offsetOfFirstUpdateStub()),
               ICStubReg);
  masm.call(Address(ICStubReg, ICStub::offsetOfStubCode()));
  masm.pop(ICStubReg);
  if (CallClobbersTailReg) {
    masm.pop(ICTailCallReg);
  }

  // The update IC will store 0 or 1 in |scratch|, R1.scratchReg(), reflecting
  // if the value in R0 type-checked properly or not.
  Label done;
  masm.branch32(Assembler::Equal, scratch, Imm32(1), &done);

  AutoStubFrame stubFrame(*this);
  stubFrame.enter(masm, scratch, CallCanGC::CanNotGC);

  masm.PushRegsInMask(saveRegs);

  masm.Push(val);
  masm.Push(TypedOrValueRegister(MIRType::Object, AnyRegister(obj)));
  masm.Push(ICStubReg);

  // Load previous frame pointer, push BaselineFrame*.
  masm.loadPtr(Address(BaselineFrameReg, 0), scratch);
  masm.pushBaselineFramePtr(scratch, scratch);

  using Fn = bool (*)(JSContext*, BaselineFrame*, ICCacheIR_Updated*,
                      HandleValue, HandleValue);
  callVM<Fn, DoTypeUpdateFallback>(masm);

  masm.PopRegsInMask(saveRegs);

  stubFrame.leave(masm);

  masm.bind(&done);
  return true;
}

bool BaselineCacheIRCompiler::emitStoreSlotShared(bool isFixed) {
  JitSpew(JitSpew_Codegen, __FUNCTION__);
  ObjOperandId objId = reader.objOperandId();
  Address offsetAddr = stubAddress(reader.stubOffset());

  // Allocate the fixed registers first. These need to be fixed for
  // callTypeUpdateIC.
  AutoScratchRegister scratch1(allocator, masm, R1.scratchReg());
  ValueOperand val =
      allocator.useFixedValueRegister(masm, reader.valOperandId(), R0);

  Register obj = allocator.useRegister(masm, objId);
  Maybe<AutoScratchRegister> scratch2;
  if (!isFixed) {
    scratch2.emplace(allocator, masm);
  }

  LiveGeneralRegisterSet saveRegs;
  saveRegs.add(obj);
  saveRegs.add(val);
  if (!callTypeUpdateIC(obj, val, scratch1, saveRegs)) {
    return false;
  }

  masm.load32(offsetAddr, scratch1);

  if (isFixed) {
    BaseIndex slot(obj, scratch1, TimesOne);
    EmitPreBarrier(masm, slot, MIRType::Value);
    masm.storeValue(val, slot);
  } else {
    masm.loadPtr(Address(obj, NativeObject::offsetOfSlots()), scratch2.ref());
    BaseIndex slot(scratch2.ref(), scratch1, TimesOne);
    EmitPreBarrier(masm, slot, MIRType::Value);
    masm.storeValue(val, slot);
  }

  emitPostBarrierSlot(obj, val, scratch1);
  return true;
}

bool BaselineCacheIRCompiler::emitStoreFixedSlot() {
  JitSpew(JitSpew_Codegen, __FUNCTION__);
  return emitStoreSlotShared(true);
}

bool BaselineCacheIRCompiler::emitStoreDynamicSlot() {
  JitSpew(JitSpew_Codegen, __FUNCTION__);
  return emitStoreSlotShared(false);
}

bool BaselineCacheIRCompiler::emitAddAndStoreSlotShared(CacheOp op) {
  JitSpew(JitSpew_Codegen, __FUNCTION__);
  ObjOperandId objId = reader.objOperandId();
  Address offsetAddr = stubAddress(reader.stubOffset());

  // Allocate the fixed registers first. These need to be fixed for
  // callTypeUpdateIC.
  AutoScratchRegister scratch1(allocator, masm, R1.scratchReg());
  ValueOperand val =
      allocator.useFixedValueRegister(masm, reader.valOperandId(), R0);

  Register obj = allocator.useRegister(masm, objId);
  AutoScratchRegister scratch2(allocator, masm);

  bool changeGroup = reader.readBool();
  Address newGroupAddr = stubAddress(reader.stubOffset());
  Address newShapeAddr = stubAddress(reader.stubOffset());

  if (op == CacheOp::AllocateAndStoreDynamicSlot) {
    // We have to (re)allocate dynamic slots. Do this first, as it's the
    // only fallible operation here. This simplifies the callTypeUpdateIC
    // call below: it does not have to worry about saving registers used by
    // failure paths. Note that growSlotsPure is fallible but does
    // not GC.
    Address numNewSlotsAddr = stubAddress(reader.stubOffset());

    FailurePath* failure;
    if (!addFailurePath(&failure)) {
      return false;
    }

    LiveRegisterSet save(GeneralRegisterSet::Volatile(),
                         liveVolatileFloatRegs());
    masm.PushRegsInMask(save);

    masm.setupUnalignedABICall(scratch1);
    masm.loadJSContext(scratch1);
    masm.passABIArg(scratch1);
    masm.passABIArg(obj);
    masm.load32(numNewSlotsAddr, scratch2);
    masm.passABIArg(scratch2);
    masm.callWithABI(JS_FUNC_TO_DATA_PTR(void*, NativeObject::growSlotsPure));
    masm.mov(ReturnReg, scratch1);

    LiveRegisterSet ignore;
    ignore.add(scratch1);
    masm.PopRegsInMaskIgnore(save, ignore);

    masm.branchIfFalseBool(scratch1, failure->label());
  }

  LiveGeneralRegisterSet saveRegs;
  saveRegs.add(obj);
  saveRegs.add(val);
  if (!callTypeUpdateIC(obj, val, scratch1, saveRegs)) {
    return false;
  }

  if (changeGroup) {
    // Changing object's group from a partially to fully initialized group,
    // per the acquired properties analysis. Only change the group if the
    // old group still has a newScript. This only applies to PlainObjects.
    Label noGroupChange;
    masm.branchIfObjGroupHasNoAddendum(obj, scratch1, &noGroupChange);

    // Update the object's group.
    masm.loadPtr(newGroupAddr, scratch1);
    masm.storeObjGroup(scratch1, obj,
                       [](MacroAssembler& masm, const Address& addr) {
                         EmitPreBarrier(masm, addr, MIRType::ObjectGroup);
                       });

    masm.bind(&noGroupChange);
  }

  // Update the object's shape.
  masm.loadPtr(newShapeAddr, scratch1);
  masm.storeObjShape(scratch1, obj,
                     [](MacroAssembler& masm, const Address& addr) {
                       EmitPreBarrier(masm, addr, MIRType::Shape);
                     });

  // Perform the store. No pre-barrier required since this is a new
  // initialization.
  masm.load32(offsetAddr, scratch1);
  if (op == CacheOp::AddAndStoreFixedSlot) {
    BaseIndex slot(obj, scratch1, TimesOne);
    masm.storeValue(val, slot);
  } else {
    MOZ_ASSERT(op == CacheOp::AddAndStoreDynamicSlot ||
               op == CacheOp::AllocateAndStoreDynamicSlot);
    masm.loadPtr(Address(obj, NativeObject::offsetOfSlots()), scratch2);
    BaseIndex slot(scratch2, scratch1, TimesOne);
    masm.storeValue(val, slot);
  }

  emitPostBarrierSlot(obj, val, scratch1);
  return true;
}

bool BaselineCacheIRCompiler::emitAddAndStoreFixedSlot() {
  JitSpew(JitSpew_Codegen, __FUNCTION__);
  return emitAddAndStoreSlotShared(CacheOp::AddAndStoreFixedSlot);
}

bool BaselineCacheIRCompiler::emitAddAndStoreDynamicSlot() {
  JitSpew(JitSpew_Codegen, __FUNCTION__);
  return emitAddAndStoreSlotShared(CacheOp::AddAndStoreDynamicSlot);
}

bool BaselineCacheIRCompiler::emitAllocateAndStoreDynamicSlot() {
  JitSpew(JitSpew_Codegen, __FUNCTION__);
  return emitAddAndStoreSlotShared(CacheOp::AllocateAndStoreDynamicSlot);
}

bool BaselineCacheIRCompiler::emitStoreTypedObjectReferenceProperty() {
  JitSpew(JitSpew_Codegen, __FUNCTION__);
  ObjOperandId objId = reader.objOperandId();
  Address offsetAddr = stubAddress(reader.stubOffset());
  TypedThingLayout layout = reader.typedThingLayout();
  ReferenceType type = reader.referenceTypeDescrType();

  // Allocate the fixed registers first. These need to be fixed for
  // callTypeUpdateIC.
  AutoScratchRegister scratch1(allocator, masm, R1.scratchReg());
  ValueOperand val =
      allocator.useFixedValueRegister(masm, reader.valOperandId(), R0);

  Register obj = allocator.useRegister(masm, objId);
  AutoScratchRegister scratch2(allocator, masm);

  // We don't need a type update IC if the property is always a string.
  if (type != ReferenceType::TYPE_STRING) {
    LiveGeneralRegisterSet saveRegs;
    saveRegs.add(obj);
    saveRegs.add(val);
    if (!callTypeUpdateIC(obj, val, scratch1, saveRegs)) {
      return false;
    }
  }

  // Compute the address being written to.
  LoadTypedThingData(masm, layout, obj, scratch1);
  masm.addPtr(offsetAddr, scratch1);
  Address dest(scratch1, 0);

  emitStoreTypedObjectReferenceProp(val, type, dest, scratch2);
  emitPostBarrierSlot(obj, val, scratch1);

  return true;
}

bool BaselineCacheIRCompiler::emitStoreTypedObjectScalarProperty() {
  JitSpew(JitSpew_Codegen, __FUNCTION__);
  Register obj = allocator.useRegister(masm, reader.objOperandId());
  Address offsetAddr = stubAddress(reader.stubOffset());
  TypedThingLayout layout = reader.typedThingLayout();
  Scalar::Type type = reader.scalarType();
  ValueOperand val = allocator.useValueRegister(masm, reader.valOperandId());
  AutoScratchRegister scratch1(allocator, masm);
  AutoScratchRegister scratch2(allocator, masm);

  FailurePath* failure;
  if (!addFailurePath(&failure)) {
    return false;
  }

  // Compute the address being written to.
  LoadTypedThingData(masm, layout, obj, scratch1);
  masm.addPtr(offsetAddr, scratch1);
  Address dest(scratch1, 0);

  StoreToTypedArray(cx_, masm, type, val, dest, scratch2, failure->label());
  return true;
}

bool BaselineCacheIRCompiler::emitStoreDenseElement() {
  JitSpew(JitSpew_Codegen, __FUNCTION__);
  ObjOperandId objId = reader.objOperandId();
  Int32OperandId indexId = reader.int32OperandId();

  // Allocate the fixed registers first. These need to be fixed for
  // callTypeUpdateIC.
  AutoScratchRegister scratch(allocator, masm, R1.scratchReg());
  ValueOperand val =
      allocator.useFixedValueRegister(masm, reader.valOperandId(), R0);

  Register obj = allocator.useRegister(masm, objId);
  Register index = allocator.useRegister(masm, indexId);

  FailurePath* failure;
  if (!addFailurePath(&failure)) {
    return false;
  }

  // Load obj->elements in scratch.
  masm.loadPtr(Address(obj, NativeObject::offsetOfElements()), scratch);

  // Bounds check. Unfortunately we don't have more registers available on
  // x86, so use InvalidReg and emit slightly slower code on x86.
  Register spectreTemp = InvalidReg;
  Address initLength(scratch, ObjectElements::offsetOfInitializedLength());
  masm.spectreBoundsCheck32(index, initLength, spectreTemp, failure->label());

  // Hole check.
  BaseObjectElementIndex element(scratch, index);
  masm.branchTestMagic(Assembler::Equal, element, failure->label());

  // Perform a single test to see if we either need to convert double
  // elements, clone the copy on write elements in the object or fail
  // due to a frozen element.
  Label noSpecialHandling;
  Address elementsFlags(scratch, ObjectElements::offsetOfFlags());
  masm.branchTest32(
      Assembler::Zero, elementsFlags,
      Imm32(ObjectElements::CONVERT_DOUBLE_ELEMENTS |
            ObjectElements::COPY_ON_WRITE | ObjectElements::FROZEN),
      &noSpecialHandling);

  // Fail if we need to clone copy on write elements or to throw due
  // to a frozen element.
  masm.branchTest32(
      Assembler::NonZero, elementsFlags,
      Imm32(ObjectElements::COPY_ON_WRITE | ObjectElements::FROZEN),
      failure->label());

  // We need to convert int32 values being stored into doubles. Note
  // that double arrays are only created by IonMonkey. It's fine to
  // convert the value in place in Baseline. We can't do this in
  // Ion.
  masm.convertInt32ValueToDouble(val);

  masm.bind(&noSpecialHandling);

  // Call the type update IC. After this everything must be infallible as we
  // don't save all registers here.
  LiveGeneralRegisterSet saveRegs;
  saveRegs.add(obj);
  saveRegs.add(index);
  saveRegs.add(val);
  if (!callTypeUpdateIC(obj, val, scratch, saveRegs)) {
    return false;
  }

  // Perform the store. Reload obj->elements because callTypeUpdateIC
  // used the scratch register.
  masm.loadPtr(Address(obj, NativeObject::offsetOfElements()), scratch);
  EmitPreBarrier(masm, element, MIRType::Value);
  masm.storeValue(val, element);

  emitPostBarrierElement(obj, val, scratch, index);
  return true;
}

bool BaselineCacheIRCompiler::emitStoreDenseElementHole() {
  JitSpew(JitSpew_Codegen, __FUNCTION__);
  ObjOperandId objId = reader.objOperandId();
  Int32OperandId indexId = reader.int32OperandId();

  // Allocate the fixed registers first. These need to be fixed for
  // callTypeUpdateIC.
  AutoScratchRegister scratch(allocator, masm, R1.scratchReg());
  ValueOperand val =
      allocator.useFixedValueRegister(masm, reader.valOperandId(), R0);

  Register obj = allocator.useRegister(masm, objId);
  Register index = allocator.useRegister(masm, indexId);

  bool handleAdd = reader.readBool();

  FailurePath* failure;
  if (!addFailurePath(&failure)) {
    return false;
  }

  // Load obj->elements in scratch.
  masm.loadPtr(Address(obj, NativeObject::offsetOfElements()), scratch);

  BaseObjectElementIndex element(scratch, index);
  Address initLength(scratch, ObjectElements::offsetOfInitializedLength());
  Address elementsFlags(scratch, ObjectElements::offsetOfFlags());

  // Check for copy-on-write elements. Note that this stub is not attached for
  // non-extensible objects, so the shape guard ensures there are no sealed or
  // frozen elements.
  masm.branchTest32(Assembler::NonZero, elementsFlags,
                    Imm32(ObjectElements::COPY_ON_WRITE), failure->label());

  // We don't have enough registers on x86 so use InvalidReg. This will emit
  // slightly less efficient code on x86.
  Register spectreTemp = InvalidReg;

  if (handleAdd) {
    // Bounds check.
    Label capacityOk, outOfBounds;
    masm.spectreBoundsCheck32(index, initLength, spectreTemp, &outOfBounds);
    masm.jump(&capacityOk);
>>>>>>> upstream-releases

    // If we're out-of-bounds, only handle the index == initLength case.
    masm.bind(&outOfBounds);
    masm.branch32(Assembler::NotEqual, initLength, index, failure->label());

    // If index < capacity, we can add a dense element inline. If not we
    // need to allocate more elements.
    Label allocElement;
    Address capacity(scratch, ObjectElements::offsetOfCapacity());
    masm.spectreBoundsCheck32(index, capacity, spectreTemp, &allocElement);
    masm.jump(&capacityOk);

    // Check for non-writable array length. We only have to do this if
    // index >= capacity.
    masm.bind(&allocElement);
    masm.branchTest32(Assembler::NonZero, elementsFlags,
                      Imm32(ObjectElements::NONWRITABLE_ARRAY_LENGTH),
                      failure->label());

    LiveRegisterSet save(GeneralRegisterSet::Volatile(),
                         liveVolatileFloatRegs());
    save.takeUnchecked(scratch);
    masm.PushRegsInMask(save);

    masm.setupUnalignedABICall(scratch);
    masm.loadJSContext(scratch);
    masm.passABIArg(scratch);
    masm.passABIArg(obj);
    masm.callWithABI(
        JS_FUNC_TO_DATA_PTR(void*, NativeObject::addDenseElementPure));
    masm.mov(ReturnReg, scratch);

    masm.PopRegsInMask(save);
    masm.branchIfFalseBool(scratch, failure->label());

    // Load the reallocated elements pointer.
    masm.loadPtr(Address(obj, NativeObject::offsetOfElements()), scratch);

    masm.bind(&capacityOk);

<<<<<<< HEAD
    // We increment initLength after the callTypeUpdateIC call, to ensure
    // the type update code doesn't read uninitialized memory.
  } else {
    // Fail if index >= initLength.
    masm.spectreBoundsCheck32(index, initLength, spectreTemp, failure->label());
  }

  // Check if we have to convert a double element.
  Label noConversion;
  masm.branchTest32(Assembler::Zero, elementsFlags,
                    Imm32(ObjectElements::CONVERT_DOUBLE_ELEMENTS),
                    &noConversion);

  // We need to convert int32 values being stored into doubles. Note
  // that double arrays are only created by IonMonkey. It's fine to
  // convert the value in place in Baseline. We can't do this in
  // Ion.
  masm.convertInt32ValueToDouble(val);

  masm.bind(&noConversion);

  // Call the type update IC. After this everything must be infallible as we
  // don't save all registers here.
  LiveGeneralRegisterSet saveRegs;
  saveRegs.add(obj);
  saveRegs.add(index);
  saveRegs.add(val);
  if (!callTypeUpdateIC(obj, val, scratch, saveRegs)) {
    return false;
  }

  // Reload obj->elements as callTypeUpdateIC used the scratch register.
  masm.loadPtr(Address(obj, NativeObject::offsetOfElements()), scratch);

  Label doStore;
  if (handleAdd) {
    // If index == initLength, increment initLength.
    Label inBounds;
    masm.branch32(Assembler::NotEqual, initLength, index, &inBounds);

    // Increment initLength.
    masm.add32(Imm32(1), initLength);

    // If length is now <= index, increment length too.
    Label skipIncrementLength;
    Address length(scratch, ObjectElements::offsetOfLength());
    masm.branch32(Assembler::Above, length, index, &skipIncrementLength);
    masm.add32(Imm32(1), length);
    masm.bind(&skipIncrementLength);

    // Skip EmitPreBarrier as the memory is uninitialized.
    masm.jump(&doStore);

    masm.bind(&inBounds);
  }

  EmitPreBarrier(masm, element, MIRType::Value);

  masm.bind(&doStore);
  masm.storeValue(val, element);

  emitPostBarrierElement(obj, val, scratch, index);
  return true;
}

bool BaselineCacheIRCompiler::emitArrayPush() {
  ObjOperandId objId = reader.objOperandId();
  ValOperandId rhsId = reader.valOperandId();

  // Allocate the fixed registers first. These need to be fixed for
  // callTypeUpdateIC.
  AutoScratchRegister scratch(allocator, masm, R1.scratchReg());
  ValueOperand val = allocator.useFixedValueRegister(masm, rhsId, R0);

  Register obj = allocator.useRegister(masm, objId);
  AutoScratchRegister scratchLength(allocator, masm);

  FailurePath* failure;
  if (!addFailurePath(&failure)) {
    return false;
  }

  // Load obj->elements in scratch.
  masm.loadPtr(Address(obj, NativeObject::offsetOfElements()), scratch);

  Address elementsInitLength(scratch,
                             ObjectElements::offsetOfInitializedLength());
  Address elementsLength(scratch, ObjectElements::offsetOfLength());
  Address elementsFlags(scratch, ObjectElements::offsetOfFlags());

  // Check for copy-on-write elements. Note that this stub is not attached for
  // non-extensible objects, so the shape guard ensures there are no sealed or
  // frozen elements.
  masm.branchTest32(Assembler::NonZero, elementsFlags,
                    Imm32(ObjectElements::COPY_ON_WRITE), failure->label());

  // Fail if length != initLength.
  masm.load32(elementsInitLength, scratchLength);
  masm.branch32(Assembler::NotEqual, elementsLength, scratchLength,
                failure->label());

  // If scratchLength < capacity, we can add a dense element inline. If not we
  // need to allocate more elements.
  Label capacityOk, allocElement;
  Address capacity(scratch, ObjectElements::offsetOfCapacity());
  masm.spectreBoundsCheck32(scratchLength, capacity, InvalidReg, &allocElement);
  masm.jump(&capacityOk);

  // Check for non-writable array length. We only have to do this if
  // index >= capacity.
  masm.bind(&allocElement);
  masm.branchTest32(Assembler::NonZero, elementsFlags,
                    Imm32(ObjectElements::NONWRITABLE_ARRAY_LENGTH),
                    failure->label());

  LiveRegisterSet save(GeneralRegisterSet::Volatile(), liveVolatileFloatRegs());
  save.takeUnchecked(scratch);
  masm.PushRegsInMask(save);

  masm.setupUnalignedABICall(scratch);
  masm.loadJSContext(scratch);
  masm.passABIArg(scratch);
  masm.passABIArg(obj);
  masm.callWithABI(
      JS_FUNC_TO_DATA_PTR(void*, NativeObject::addDenseElementPure));
  masm.mov(ReturnReg, scratch);

  masm.PopRegsInMask(save);
  masm.branchIfFalseBool(scratch, failure->label());

  // Load the reallocated elements pointer.
  masm.loadPtr(Address(obj, NativeObject::offsetOfElements()), scratch);

  masm.bind(&capacityOk);

  // Check if we have to convert a double element.
  Label noConversion;
  masm.branchTest32(Assembler::Zero, elementsFlags,
                    Imm32(ObjectElements::CONVERT_DOUBLE_ELEMENTS),
                    &noConversion);

  // We need to convert int32 values being stored into doubles. Note
  // that double arrays are only created by IonMonkey. It's fine to
  // convert the value in place in Baseline. We can't do this in
  // Ion.
  masm.convertInt32ValueToDouble(val);

  masm.bind(&noConversion);

  // Call the type update IC. After this everything must be infallible as we
  // don't save all registers here.
  LiveGeneralRegisterSet saveRegs;
  saveRegs.add(obj);
  saveRegs.add(val);
  if (!callTypeUpdateIC(obj, val, scratch, saveRegs)) {
    return false;
  }

  // Reload obj->elements as callTypeUpdateIC used the scratch register.
  masm.loadPtr(Address(obj, NativeObject::offsetOfElements()), scratch);

  // Increment initLength and length.
  masm.add32(Imm32(1), elementsInitLength);
  masm.load32(elementsLength, scratchLength);
  masm.add32(Imm32(1), elementsLength);

  // Store the value.
  BaseObjectElementIndex element(scratch, scratchLength);
  masm.storeValue(val, element);
  emitPostBarrierElement(obj, val, scratch, scratchLength);

  // Return value is new length.
  masm.add32(Imm32(1), scratchLength);
  masm.tagValue(JSVAL_TYPE_INT32, scratchLength, val);

  return true;
}

bool BaselineCacheIRCompiler::emitStoreTypedElement() {
  Register obj = allocator.useRegister(masm, reader.objOperandId());
  Register index = allocator.useRegister(masm, reader.int32OperandId());
  ValueOperand val = allocator.useValueRegister(masm, reader.valOperandId());

  TypedThingLayout layout = reader.typedThingLayout();
  Scalar::Type type = reader.scalarType();
  bool handleOOB = reader.readBool();

  AutoScratchRegister scratch1(allocator, masm);

  FailurePath* failure;
  if (!addFailurePath(&failure)) {
    return false;
  }

  // Bounds check.
  Label done;
  LoadTypedThingLength(masm, layout, obj, scratch1);

  // Unfortunately we don't have more registers available on x86, so use
  // InvalidReg and emit slightly slower code on x86.
  Register spectreTemp = InvalidReg;
  masm.spectreBoundsCheck32(index, scratch1, spectreTemp,
                            handleOOB ? &done : failure->label());

  // Load the elements vector.
  LoadTypedThingData(masm, layout, obj, scratch1);

  BaseIndex dest(scratch1, index, ScaleFromElemWidth(Scalar::byteSize(type)));

  // Use ICStubReg as second scratch register. TODO: consider doing the RHS
  // type check/conversion as a separate IR instruction so we can simplify
  // this.
  Register scratch2 = ICStubReg;
  masm.push(scratch2);

  Label fail;
  StoreToTypedArray(cx_, masm, type, val, dest, scratch2, &fail);
  masm.pop(scratch2);
  masm.jump(&done);

  masm.bind(&fail);
  masm.pop(scratch2);
  masm.jump(failure->label());

  masm.bind(&done);
  return true;
}
||||||| merged common ancestors
    // Check if we have to convert a double element.
    Label noConversion;
    masm.branchTest32(Assembler::Zero, elementsFlags,
                      Imm32(ObjectElements::CONVERT_DOUBLE_ELEMENTS),
                      &noConversion);

    // We need to convert int32 values being stored into doubles. Note that
    // double arrays are only created by IonMonkey, so if we have no FP support
    // Ion is disabled and there should be no double arrays.
    if (cx_->runtime()->jitSupportsFloatingPoint) {
        // It's fine to convert the value in place in Baseline. We can't do
        // this in Ion.
        masm.convertInt32ValueToDouble(val);
    } else {
        masm.assumeUnreachable("There shouldn't be double arrays when there is no FP support.");
    }

    masm.bind(&noConversion);

    // Call the type update IC. After this everything must be infallible as we
    // don't save all registers here.
    LiveGeneralRegisterSet saveRegs;
    saveRegs.add(obj);
    saveRegs.add(val);
    if (!callTypeUpdateIC(obj, val, scratch, saveRegs)) {
        return false;
    }

    // Reload obj->elements as callTypeUpdateIC used the scratch register.
    masm.loadPtr(Address(obj, NativeObject::offsetOfElements()), scratch);

    // Increment initLength and length.
    masm.add32(Imm32(1), elementsInitLength);
    masm.load32(elementsLength, scratchLength);
    masm.add32(Imm32(1), elementsLength);

    // Store the value.
    BaseObjectElementIndex element(scratch, scratchLength);
    masm.storeValue(val, element);
    emitPostBarrierElement(obj, val, scratch, scratchLength);

    // Return value is new length.
    masm.add32(Imm32(1), scratchLength);
    masm.tagValue(JSVAL_TYPE_INT32, scratchLength, val);

    return true;
}
=======
    // We increment initLength after the callTypeUpdateIC call, to ensure
    // the type update code doesn't read uninitialized memory.
  } else {
    // Fail if index >= initLength.
    masm.spectreBoundsCheck32(index, initLength, spectreTemp, failure->label());
  }

  // Check if we have to convert a double element.
  Label noConversion;
  masm.branchTest32(Assembler::Zero, elementsFlags,
                    Imm32(ObjectElements::CONVERT_DOUBLE_ELEMENTS),
                    &noConversion);

  // We need to convert int32 values being stored into doubles. Note
  // that double arrays are only created by IonMonkey. It's fine to
  // convert the value in place in Baseline. We can't do this in
  // Ion.
  masm.convertInt32ValueToDouble(val);

  masm.bind(&noConversion);

  // Call the type update IC. After this everything must be infallible as we
  // don't save all registers here.
  LiveGeneralRegisterSet saveRegs;
  saveRegs.add(obj);
  saveRegs.add(index);
  saveRegs.add(val);
  if (!callTypeUpdateIC(obj, val, scratch, saveRegs)) {
    return false;
  }

  // Reload obj->elements as callTypeUpdateIC used the scratch register.
  masm.loadPtr(Address(obj, NativeObject::offsetOfElements()), scratch);

  Label doStore;
  if (handleAdd) {
    // If index == initLength, increment initLength.
    Label inBounds;
    masm.branch32(Assembler::NotEqual, initLength, index, &inBounds);

    // Increment initLength.
    masm.add32(Imm32(1), initLength);

    // If length is now <= index, increment length too.
    Label skipIncrementLength;
    Address length(scratch, ObjectElements::offsetOfLength());
    masm.branch32(Assembler::Above, length, index, &skipIncrementLength);
    masm.add32(Imm32(1), length);
    masm.bind(&skipIncrementLength);

    // Skip EmitPreBarrier as the memory is uninitialized.
    masm.jump(&doStore);

    masm.bind(&inBounds);
  }

  EmitPreBarrier(masm, element, MIRType::Value);

  masm.bind(&doStore);
  masm.storeValue(val, element);

  emitPostBarrierElement(obj, val, scratch, index);
  return true;
}

bool BaselineCacheIRCompiler::emitArrayPush() {
  JitSpew(JitSpew_Codegen, __FUNCTION__);
  ObjOperandId objId = reader.objOperandId();
  ValOperandId rhsId = reader.valOperandId();

  // Allocate the fixed registers first. These need to be fixed for
  // callTypeUpdateIC.
  AutoScratchRegister scratch(allocator, masm, R1.scratchReg());
  ValueOperand val = allocator.useFixedValueRegister(masm, rhsId, R0);

  Register obj = allocator.useRegister(masm, objId);
  AutoScratchRegister scratchLength(allocator, masm);

  FailurePath* failure;
  if (!addFailurePath(&failure)) {
    return false;
  }

  // Load obj->elements in scratch.
  masm.loadPtr(Address(obj, NativeObject::offsetOfElements()), scratch);

  Address elementsInitLength(scratch,
                             ObjectElements::offsetOfInitializedLength());
  Address elementsLength(scratch, ObjectElements::offsetOfLength());
  Address elementsFlags(scratch, ObjectElements::offsetOfFlags());

  // Check for copy-on-write elements. Note that this stub is not attached for
  // non-extensible objects, so the shape guard ensures there are no sealed or
  // frozen elements.
  masm.branchTest32(Assembler::NonZero, elementsFlags,
                    Imm32(ObjectElements::COPY_ON_WRITE), failure->label());

  // Fail if length != initLength.
  masm.load32(elementsInitLength, scratchLength);
  masm.branch32(Assembler::NotEqual, elementsLength, scratchLength,
                failure->label());

  // If scratchLength < capacity, we can add a dense element inline. If not we
  // need to allocate more elements.
  Label capacityOk, allocElement;
  Address capacity(scratch, ObjectElements::offsetOfCapacity());
  masm.spectreBoundsCheck32(scratchLength, capacity, InvalidReg, &allocElement);
  masm.jump(&capacityOk);

  // Check for non-writable array length. We only have to do this if
  // index >= capacity.
  masm.bind(&allocElement);
  masm.branchTest32(Assembler::NonZero, elementsFlags,
                    Imm32(ObjectElements::NONWRITABLE_ARRAY_LENGTH),
                    failure->label());

  LiveRegisterSet save(GeneralRegisterSet::Volatile(), liveVolatileFloatRegs());
  save.takeUnchecked(scratch);
  masm.PushRegsInMask(save);

  masm.setupUnalignedABICall(scratch);
  masm.loadJSContext(scratch);
  masm.passABIArg(scratch);
  masm.passABIArg(obj);
  masm.callWithABI(
      JS_FUNC_TO_DATA_PTR(void*, NativeObject::addDenseElementPure));
  masm.mov(ReturnReg, scratch);

  masm.PopRegsInMask(save);
  masm.branchIfFalseBool(scratch, failure->label());

  // Load the reallocated elements pointer.
  masm.loadPtr(Address(obj, NativeObject::offsetOfElements()), scratch);

  masm.bind(&capacityOk);

  // Check if we have to convert a double element.
  Label noConversion;
  masm.branchTest32(Assembler::Zero, elementsFlags,
                    Imm32(ObjectElements::CONVERT_DOUBLE_ELEMENTS),
                    &noConversion);

  // We need to convert int32 values being stored into doubles. Note
  // that double arrays are only created by IonMonkey. It's fine to
  // convert the value in place in Baseline. We can't do this in
  // Ion.
  masm.convertInt32ValueToDouble(val);
>>>>>>> upstream-releases

<<<<<<< HEAD
typedef bool (*CallNativeSetterFn)(JSContext*, HandleFunction, HandleObject,
                                   HandleValue);
static const VMFunction CallNativeSetterInfo =
    FunctionInfo<CallNativeSetterFn>(CallNativeSetter, "CallNativeSetter");
||||||| merged common ancestors
bool
BaselineCacheIRCompiler::emitStoreTypedElement()
{
    Register obj = allocator.useRegister(masm, reader.objOperandId());
    Register index = allocator.useRegister(masm, reader.int32OperandId());
    ValueOperand val = allocator.useValueRegister(masm, reader.valOperandId());

    TypedThingLayout layout = reader.typedThingLayout();
    Scalar::Type type = reader.scalarType();
    bool handleOOB = reader.readBool();

    AutoScratchRegister scratch1(allocator, masm);

    FailurePath* failure;
    if (!addFailurePath(&failure)) {
        return false;
    }
=======
  masm.bind(&noConversion);

  // Call the type update IC. After this everything must be infallible as we
  // don't save all registers here.
  LiveGeneralRegisterSet saveRegs;
  saveRegs.add(obj);
  saveRegs.add(val);
  if (!callTypeUpdateIC(obj, val, scratch, saveRegs)) {
    return false;
  }

  // Reload obj->elements as callTypeUpdateIC used the scratch register.
  masm.loadPtr(Address(obj, NativeObject::offsetOfElements()), scratch);

  // Increment initLength and length.
  masm.add32(Imm32(1), elementsInitLength);
  masm.load32(elementsLength, scratchLength);
  masm.add32(Imm32(1), elementsLength);

  // Store the value.
  BaseObjectElementIndex element(scratch, scratchLength);
  masm.storeValue(val, element);
  emitPostBarrierElement(obj, val, scratch, scratchLength);

  // Return value is new length.
  masm.add32(Imm32(1), scratchLength);
  masm.tagValue(JSVAL_TYPE_INT32, scratchLength, val);

  return true;
}

bool BaselineCacheIRCompiler::emitStoreTypedElement() {
  JitSpew(JitSpew_Codegen, __FUNCTION__);
  Register obj = allocator.useRegister(masm, reader.objOperandId());
  Register index = allocator.useRegister(masm, reader.int32OperandId());
  ValueOperand val = allocator.useValueRegister(masm, reader.valOperandId());

  TypedThingLayout layout = reader.typedThingLayout();
  Scalar::Type type = reader.scalarType();
  bool handleOOB = reader.readBool();

  AutoScratchRegister scratch1(allocator, masm);

  FailurePath* failure;
  if (!addFailurePath(&failure)) {
    return false;
  }
>>>>>>> upstream-releases

<<<<<<< HEAD
bool BaselineCacheIRCompiler::emitCallNativeSetter() {
  Register obj = allocator.useRegister(masm, reader.objOperandId());
  Address setterAddr(stubAddress(reader.stubOffset()));
  ValueOperand val = allocator.useValueRegister(masm, reader.valOperandId());
||||||| merged common ancestors
    // Bounds check.
    Label done;
    LoadTypedThingLength(masm, layout, obj, scratch1);
=======
  // Bounds check.
  Label done;
  LoadTypedThingLength(masm, layout, obj, scratch1);
>>>>>>> upstream-releases

<<<<<<< HEAD
  AutoScratchRegister scratch(allocator, masm);
||||||| merged common ancestors
    // Unfortunately we don't have more registers available on x86, so use
    // InvalidReg and emit slightly slower code on x86.
    Register spectreTemp = InvalidReg;
    masm.spectreBoundsCheck32(index, scratch1, spectreTemp, handleOOB ? &done : failure->label());
=======
  // Unfortunately we don't have more registers available on x86, so use
  // InvalidReg and emit slightly slower code on x86.
  Register spectreTemp = InvalidReg;
  masm.spectreBoundsCheck32(index, scratch1, spectreTemp,
                            handleOOB ? &done : failure->label());
>>>>>>> upstream-releases

<<<<<<< HEAD
  allocator.discardStack(masm);
||||||| merged common ancestors
    // Load the elements vector.
    LoadTypedThingData(masm, layout, obj, scratch1);
=======
  // Load the elements vector.
  LoadTypedThingData(masm, layout, obj, scratch1);
>>>>>>> upstream-releases

<<<<<<< HEAD
  AutoStubFrame stubFrame(*this);
  stubFrame.enter(masm, scratch);
||||||| merged common ancestors
    BaseIndex dest(scratch1, index, ScaleFromElemWidth(Scalar::byteSize(type)));
=======
  BaseIndex dest(scratch1, index, ScaleFromElemWidth(Scalar::byteSize(type)));
>>>>>>> upstream-releases

<<<<<<< HEAD
  // Load the callee in the scratch register.
  masm.loadPtr(setterAddr, scratch);
||||||| merged common ancestors
    // Use ICStubReg as second scratch register. TODO: consider doing the RHS
    // type check/conversion as a separate IR instruction so we can simplify
    // this.
    Register scratch2 = ICStubReg;
    masm.push(scratch2);
=======
  // Use ICStubReg as second scratch register. TODO: consider doing the RHS
  // type check/conversion as a separate IR instruction so we can simplify
  // this.
  Register scratch2 = ICStubReg;
  masm.push(scratch2);
>>>>>>> upstream-releases

<<<<<<< HEAD
  masm.Push(val);
  masm.Push(obj);
  masm.Push(scratch);
||||||| merged common ancestors
    Label fail;
    StoreToTypedArray(cx_, masm, type, val, dest, scratch2, &fail);
    masm.pop(scratch2);
    masm.jump(&done);
=======
  Label fail;
  StoreToTypedArray(cx_, masm, type, val, dest, scratch2, &fail);
  masm.pop(scratch2);
  masm.jump(&done);
>>>>>>> upstream-releases

<<<<<<< HEAD
  if (!callVM(masm, CallNativeSetterInfo)) {
    return false;
  }
||||||| merged common ancestors
    masm.bind(&fail);
    masm.pop(scratch2);
    masm.jump(failure->label());
=======
  masm.bind(&fail);
  masm.pop(scratch2);
  masm.jump(failure->label());
>>>>>>> upstream-releases

<<<<<<< HEAD
  stubFrame.leave(masm);
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
bool BaselineCacheIRCompiler::emitCallScriptedSetter() {
  AutoScratchRegister scratch1(allocator, masm);
  AutoScratchRegister scratch2(allocator, masm);
||||||| merged common ancestors
typedef bool (*CallNativeSetterFn)(JSContext*, HandleFunction, HandleObject, HandleValue);
static const VMFunction CallNativeSetterInfo =
    FunctionInfo<CallNativeSetterFn>(CallNativeSetter, "CallNativeSetter");
=======
bool BaselineCacheIRCompiler::emitCallNativeSetter() {
  JitSpew(JitSpew_Codegen, __FUNCTION__);
  Register obj = allocator.useRegister(masm, reader.objOperandId());
  Address setterAddr(stubAddress(reader.stubOffset()));
  ValueOperand val = allocator.useValueRegister(masm, reader.valOperandId());
>>>>>>> upstream-releases

<<<<<<< HEAD
  Register obj = allocator.useRegister(masm, reader.objOperandId());
  Address setterAddr(stubAddress(reader.stubOffset()));
  ValueOperand val = allocator.useValueRegister(masm, reader.valOperandId());
  bool isCrossRealm = reader.readBool();

  // First, ensure our setter is non-lazy. This also loads the callee in
  // scratch1.
  {
    FailurePath* failure;
    if (!addFailurePath(&failure)) {
      return false;
    }
||||||| merged common ancestors
bool
BaselineCacheIRCompiler::emitCallNativeSetter()
{
    Register obj = allocator.useRegister(masm, reader.objOperandId());
    Address setterAddr(stubAddress(reader.stubOffset()));
    ValueOperand val = allocator.useValueRegister(masm, reader.valOperandId());

    AutoScratchRegister scratch(allocator, masm);
=======
  AutoScratchRegister scratch(allocator, masm);
>>>>>>> upstream-releases

<<<<<<< HEAD
    masm.loadPtr(setterAddr, scratch1);
    masm.branchIfFunctionHasNoJitEntry(scratch1, /* constructing */ false,
                                       failure->label());
  }
||||||| merged common ancestors
    allocator.discardStack(masm);
=======
  allocator.discardStack(masm);
>>>>>>> upstream-releases

<<<<<<< HEAD
  allocator.discardStack(masm);
||||||| merged common ancestors
    AutoStubFrame stubFrame(*this);
    stubFrame.enter(masm, scratch);
=======
  AutoStubFrame stubFrame(*this);
  stubFrame.enter(masm, scratch);
>>>>>>> upstream-releases

<<<<<<< HEAD
  AutoStubFrame stubFrame(*this);
  stubFrame.enter(masm, scratch2);
||||||| merged common ancestors
    // Load the callee in the scratch register.
    masm.loadPtr(setterAddr, scratch);
=======
  // Load the callee in the scratch register.
  masm.loadPtr(setterAddr, scratch);
>>>>>>> upstream-releases

<<<<<<< HEAD
  if (isCrossRealm) {
    masm.switchToObjectRealm(scratch1, scratch2);
  }
||||||| merged common ancestors
    masm.Push(val);
    masm.Push(obj);
    masm.Push(scratch);
=======
  masm.Push(val);
  masm.Push(obj);
  masm.Push(scratch);
>>>>>>> upstream-releases

<<<<<<< HEAD
  // Align the stack such that the JitFrameLayout is aligned on
  // JitStackAlignment.
  masm.alignJitStackBasedOnNArgs(1);
||||||| merged common ancestors
    if (!callVM(masm, CallNativeSetterInfo)) {
        return false;
    }
=======
  using Fn = bool (*)(JSContext*, HandleFunction, HandleObject, HandleValue);
  callVM<Fn, CallNativeSetter>(masm);
>>>>>>> upstream-releases

<<<<<<< HEAD
  // Setter is called with 1 argument, and |obj| as thisv. Note that we use
  // Push, not push, so that callJit will align the stack properly on ARM.
  masm.Push(val);
  masm.Push(TypedOrValueRegister(MIRType::Object, AnyRegister(obj)));
||||||| merged common ancestors
    stubFrame.leave(masm);
    return true;
}

bool
BaselineCacheIRCompiler::emitCallScriptedSetter()
{
    AutoScratchRegister scratch1(allocator, masm);
    AutoScratchRegister scratch2(allocator, masm);

    Register obj = allocator.useRegister(masm, reader.objOperandId());
    Address setterAddr(stubAddress(reader.stubOffset()));
    ValueOperand val = allocator.useValueRegister(masm, reader.valOperandId());
    bool isCrossRealm = reader.readBool();

    // First, ensure our setter is non-lazy. This also loads the callee in
    // scratch1.
    {
        FailurePath* failure;
        if (!addFailurePath(&failure)) {
            return false;
        }
=======
  stubFrame.leave(masm);
  return true;
}

bool BaselineCacheIRCompiler::emitCallScriptedSetter() {
  JitSpew(JitSpew_Codegen, __FUNCTION__);
  AutoScratchRegister scratch1(allocator, masm);
  AutoScratchRegister scratch2(allocator, masm);
>>>>>>> upstream-releases

<<<<<<< HEAD
  // Now that the object register is no longer needed, use it as second
  // scratch.
  EmitBaselineCreateStubFrameDescriptor(masm, scratch2, JitFrameLayout::Size());
  masm.Push(Imm32(1));  // ActualArgc
||||||| merged common ancestors
        masm.loadPtr(setterAddr, scratch1);
        masm.branchIfFunctionHasNoJitEntry(scratch1, /* constructing */ false, failure->label());
    }
=======
  Register obj = allocator.useRegister(masm, reader.objOperandId());
  Address setterAddr(stubAddress(reader.stubOffset()));
  ValueOperand val = allocator.useValueRegister(masm, reader.valOperandId());
  bool isSameRealm = reader.readBool();

  // First, ensure our setter is non-lazy. This also loads the callee in
  // scratch1.
  {
    FailurePath* failure;
    if (!addFailurePath(&failure)) {
      return false;
    }
>>>>>>> upstream-releases

<<<<<<< HEAD
  // Push callee.
  masm.Push(scratch1);
||||||| merged common ancestors
    allocator.discardStack(masm);
=======
    masm.loadPtr(setterAddr, scratch1);
    masm.branchIfFunctionHasNoJitEntry(scratch1, /* constructing */ false,
                                       failure->label());
  }
>>>>>>> upstream-releases

<<<<<<< HEAD
  // Push frame descriptor.
  masm.Push(scratch2);
||||||| merged common ancestors
    AutoStubFrame stubFrame(*this);
    stubFrame.enter(masm, scratch2);
=======
  allocator.discardStack(masm);
>>>>>>> upstream-releases

<<<<<<< HEAD
  // Load callee->nargs in scratch2 and the JIT code in scratch.
  Label noUnderflow;
  masm.load16ZeroExtend(Address(scratch1, JSFunction::offsetOfNargs()),
                        scratch2);
  masm.loadJitCodeRaw(scratch1, scratch1);
||||||| merged common ancestors
    if (isCrossRealm) {
        masm.switchToObjectRealm(scratch1, scratch2);
    }
=======
  AutoStubFrame stubFrame(*this);
  stubFrame.enter(masm, scratch2);
>>>>>>> upstream-releases

<<<<<<< HEAD
  // Handle arguments underflow.
  masm.branch32(Assembler::BelowOrEqual, scratch2, Imm32(1), &noUnderflow);
  {
    // Call the arguments rectifier.
    TrampolinePtr argumentsRectifier =
        cx_->runtime()->jitRuntime()->getArgumentsRectifier();
    masm.movePtr(argumentsRectifier, scratch1);
  }
||||||| merged common ancestors
    // Align the stack such that the JitFrameLayout is aligned on
    // JitStackAlignment.
    masm.alignJitStackBasedOnNArgs(1);

    // Setter is called with 1 argument, and |obj| as thisv. Note that we use
    // Push, not push, so that callJit will align the stack properly on ARM.
    masm.Push(val);
    masm.Push(TypedOrValueRegister(MIRType::Object, AnyRegister(obj)));

    // Now that the object register is no longer needed, use it as second
    // scratch.
    EmitBaselineCreateStubFrameDescriptor(masm, scratch2, JitFrameLayout::Size());
    masm.Push(Imm32(1));  // ActualArgc

    // Push callee.
    masm.Push(scratch1);

    // Push frame descriptor.
    masm.Push(scratch2);

    // Load callee->nargs in scratch2 and the JIT code in scratch.
    Label noUnderflow;
    masm.load16ZeroExtend(Address(scratch1, JSFunction::offsetOfNargs()), scratch2);
    masm.loadJitCodeRaw(scratch1, scratch1);

    // Handle arguments underflow.
    masm.branch32(Assembler::BelowOrEqual, scratch2, Imm32(1), &noUnderflow);
    {
        // Call the arguments rectifier.
        TrampolinePtr argumentsRectifier = cx_->runtime()->jitRuntime()->getArgumentsRectifier();
        masm.movePtr(argumentsRectifier, scratch1);
    }
=======
  if (!isSameRealm) {
    masm.switchToObjectRealm(scratch1, scratch2);
  }
>>>>>>> upstream-releases

<<<<<<< HEAD
  masm.bind(&noUnderflow);
  masm.callJit(scratch1);
||||||| merged common ancestors
    masm.bind(&noUnderflow);
    masm.callJit(scratch1);
=======
  // Align the stack such that the JitFrameLayout is aligned on
  // JitStackAlignment.
  masm.alignJitStackBasedOnNArgs(1);
>>>>>>> upstream-releases

<<<<<<< HEAD
  stubFrame.leave(masm, true);
||||||| merged common ancestors
    stubFrame.leave(masm, true);
=======
  // Setter is called with 1 argument, and |obj| as thisv. Note that we use
  // Push, not push, so that callJit will align the stack properly on ARM.
  masm.Push(val);
  masm.Push(TypedOrValueRegister(MIRType::Object, AnyRegister(obj)));
>>>>>>> upstream-releases

<<<<<<< HEAD
  if (isCrossRealm) {
    masm.switchToBaselineFrameRealm(R1.scratchReg());
  }
||||||| merged common ancestors
    if (isCrossRealm) {
        masm.switchToBaselineFrameRealm(R1.scratchReg());
    }
=======
  // Now that the object register is no longer needed, use it as second
  // scratch.
  EmitBaselineCreateStubFrameDescriptor(masm, scratch2, JitFrameLayout::Size());
  masm.Push(Imm32(1));  // ActualArgc
>>>>>>> upstream-releases

<<<<<<< HEAD
  return true;
}
||||||| merged common ancestors
    return true;
}
=======
  // Push callee.
  masm.Push(scratch1);
>>>>>>> upstream-releases

<<<<<<< HEAD
bool BaselineCacheIRCompiler::emitCallSetArrayLength() {
  Register obj = allocator.useRegister(masm, reader.objOperandId());
  bool strict = reader.readBool();
  ValueOperand val = allocator.useValueRegister(masm, reader.valOperandId());
||||||| merged common ancestors
bool
BaselineCacheIRCompiler::emitCallSetArrayLength()
{
    Register obj = allocator.useRegister(masm, reader.objOperandId());
    bool strict = reader.readBool();
    ValueOperand val = allocator.useValueRegister(masm, reader.valOperandId());
=======
  // Push frame descriptor.
  masm.Push(scratch2);
>>>>>>> upstream-releases

<<<<<<< HEAD
  AutoScratchRegister scratch(allocator, masm);
||||||| merged common ancestors
    AutoScratchRegister scratch(allocator, masm);
=======
  // Load callee->nargs in scratch2 and the JIT code in scratch.
  Label noUnderflow;
  masm.load16ZeroExtend(Address(scratch1, JSFunction::offsetOfNargs()),
                        scratch2);
  masm.loadJitCodeRaw(scratch1, scratch1);
>>>>>>> upstream-releases

<<<<<<< HEAD
  allocator.discardStack(masm);
||||||| merged common ancestors
    allocator.discardStack(masm);
=======
  // Handle arguments underflow.
  masm.branch32(Assembler::BelowOrEqual, scratch2, Imm32(1), &noUnderflow);
  {
    // Call the arguments rectifier.
    TrampolinePtr argumentsRectifier =
        cx_->runtime()->jitRuntime()->getArgumentsRectifier();
    masm.movePtr(argumentsRectifier, scratch1);
  }
>>>>>>> upstream-releases

<<<<<<< HEAD
  AutoStubFrame stubFrame(*this);
  stubFrame.enter(masm, scratch);
||||||| merged common ancestors
    AutoStubFrame stubFrame(*this);
    stubFrame.enter(masm, scratch);
=======
  masm.bind(&noUnderflow);
  masm.callJit(scratch1);
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
  stubFrame.leave(masm, true);
>>>>>>> upstream-releases

<<<<<<< HEAD
  if (!callVM(masm, SetArrayLengthInfo)) {
    return false;
  }
||||||| merged common ancestors
    if (!callVM(masm, SetArrayLengthInfo)) {
        return false;
    }
=======
  if (!isSameRealm) {
    masm.switchToBaselineFrameRealm(R1.scratchReg());
  }
>>>>>>> upstream-releases

<<<<<<< HEAD
  stubFrame.leave(masm);
  return true;
||||||| merged common ancestors
    stubFrame.leave(masm);
    return true;
=======
  return true;
>>>>>>> upstream-releases
}

<<<<<<< HEAD
bool BaselineCacheIRCompiler::emitCallProxySet() {
  Register obj = allocator.useRegister(masm, reader.objOperandId());
  ValueOperand val = allocator.useValueRegister(masm, reader.valOperandId());
  Address idAddr(stubAddress(reader.stubOffset()));
  bool strict = reader.readBool();

  AutoScratchRegister scratch(allocator, masm);
||||||| merged common ancestors
bool
BaselineCacheIRCompiler::emitCallProxySet()
{
    Register obj = allocator.useRegister(masm, reader.objOperandId());
    ValueOperand val = allocator.useValueRegister(masm, reader.valOperandId());
    Address idAddr(stubAddress(reader.stubOffset()));
    bool strict = reader.readBool();

    AutoScratchRegister scratch(allocator, masm);
=======
bool BaselineCacheIRCompiler::emitCallSetArrayLength() {
  JitSpew(JitSpew_Codegen, __FUNCTION__);
  Register obj = allocator.useRegister(masm, reader.objOperandId());
  bool strict = reader.readBool();
  ValueOperand val = allocator.useValueRegister(masm, reader.valOperandId());
>>>>>>> upstream-releases

<<<<<<< HEAD
  allocator.discardStack(masm);
||||||| merged common ancestors
    allocator.discardStack(masm);
=======
  AutoScratchRegister scratch(allocator, masm);
>>>>>>> upstream-releases

<<<<<<< HEAD
  AutoStubFrame stubFrame(*this);
  stubFrame.enter(masm, scratch);
||||||| merged common ancestors
    AutoStubFrame stubFrame(*this);
    stubFrame.enter(masm, scratch);
=======
  allocator.discardStack(masm);
>>>>>>> upstream-releases

<<<<<<< HEAD
  // Load the jsid in the scratch register.
  masm.loadPtr(idAddr, scratch);
||||||| merged common ancestors
    // Load the jsid in the scratch register.
    masm.loadPtr(idAddr, scratch);
=======
  AutoStubFrame stubFrame(*this);
  stubFrame.enter(masm, scratch);
>>>>>>> upstream-releases

<<<<<<< HEAD
  masm.Push(Imm32(strict));
  masm.Push(val);
  masm.Push(scratch);
  masm.Push(obj);
||||||| merged common ancestors
    masm.Push(Imm32(strict));
    masm.Push(val);
    masm.Push(scratch);
    masm.Push(obj);
=======
  masm.Push(Imm32(strict));
  masm.Push(val);
  masm.Push(obj);
>>>>>>> upstream-releases

<<<<<<< HEAD
  if (!callVM(masm, ProxySetPropertyInfo)) {
    return false;
  }
||||||| merged common ancestors
    if (!callVM(masm, ProxySetPropertyInfo)) {
        return false;
    }
=======
  using Fn = bool (*)(JSContext*, HandleObject, HandleValue, bool);
  callVM<Fn, jit::SetArrayLength>(masm);
>>>>>>> upstream-releases

  stubFrame.leave(masm);
  return true;
}

<<<<<<< HEAD
bool BaselineCacheIRCompiler::emitCallProxySetByValue() {
  Register obj = allocator.useRegister(masm, reader.objOperandId());
  ValueOperand idVal = allocator.useValueRegister(masm, reader.valOperandId());
  ValueOperand val = allocator.useValueRegister(masm, reader.valOperandId());
  bool strict = reader.readBool();
||||||| merged common ancestors
bool
BaselineCacheIRCompiler::emitCallProxySetByValue()
{
    Register obj = allocator.useRegister(masm, reader.objOperandId());
    ValueOperand idVal = allocator.useValueRegister(masm, reader.valOperandId());
    ValueOperand val = allocator.useValueRegister(masm, reader.valOperandId());
    bool strict = reader.readBool();
=======
bool BaselineCacheIRCompiler::emitCallProxySet() {
  JitSpew(JitSpew_Codegen, __FUNCTION__);
  Register obj = allocator.useRegister(masm, reader.objOperandId());
  ValueOperand val = allocator.useValueRegister(masm, reader.valOperandId());
  Address idAddr(stubAddress(reader.stubOffset()));
  bool strict = reader.readBool();
>>>>>>> upstream-releases

<<<<<<< HEAD
  allocator.discardStack(masm);
||||||| merged common ancestors
    allocator.discardStack(masm);
=======
  AutoScratchRegister scratch(allocator, masm);
>>>>>>> upstream-releases

<<<<<<< HEAD
  // We need a scratch register but we don't have any registers available on
  // x86, so temporarily store |obj| in the frame's scratch slot.
  int scratchOffset = BaselineFrame::reverseOffsetOfScratchValue();
  masm.storePtr(obj, Address(BaselineFrameReg, scratchOffset));
||||||| merged common ancestors
    // We need a scratch register but we don't have any registers available on
    // x86, so temporarily store |obj| in the frame's scratch slot.
    int scratchOffset = BaselineFrame::reverseOffsetOfScratchValue();
    masm.storePtr(obj, Address(BaselineFrameReg, scratchOffset));
=======
  allocator.discardStack(masm);
>>>>>>> upstream-releases

<<<<<<< HEAD
  AutoStubFrame stubFrame(*this);
  stubFrame.enter(masm, obj);
||||||| merged common ancestors
    AutoStubFrame stubFrame(*this);
    stubFrame.enter(masm, obj);
=======
  AutoStubFrame stubFrame(*this);
  stubFrame.enter(masm, scratch);
>>>>>>> upstream-releases

<<<<<<< HEAD
  // Restore |obj|. Because we entered a stub frame we first have to load
  // the original frame pointer.
  masm.loadPtr(Address(BaselineFrameReg, 0), obj);
  masm.loadPtr(Address(obj, scratchOffset), obj);
||||||| merged common ancestors
    // Restore |obj|. Because we entered a stub frame we first have to load
    // the original frame pointer.
    masm.loadPtr(Address(BaselineFrameReg, 0), obj);
    masm.loadPtr(Address(obj, scratchOffset), obj);
=======
  // Load the jsid in the scratch register.
  masm.loadPtr(idAddr, scratch);
>>>>>>> upstream-releases

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
  masm.Push(scratch);
  masm.Push(obj);
>>>>>>> upstream-releases

<<<<<<< HEAD
  if (!callVM(masm, ProxySetPropertyByValueInfo)) {
    return false;
  }
||||||| merged common ancestors
    if (!callVM(masm, ProxySetPropertyByValueInfo)) {
        return false;
    }
=======
  using Fn = bool (*)(JSContext*, HandleObject, HandleId, HandleValue, bool);
  callVM<Fn, ProxySetProperty>(masm);
>>>>>>> upstream-releases

  stubFrame.leave(masm);
  return true;
}

<<<<<<< HEAD
bool BaselineCacheIRCompiler::emitCallAddOrUpdateSparseElementHelper() {
  Register obj = allocator.useRegister(masm, reader.objOperandId());
  Register id = allocator.useRegister(masm, reader.int32OperandId());
  ValueOperand val = allocator.useValueRegister(masm, reader.valOperandId());
  bool strict = reader.readBool();
  AutoScratchRegister scratch(allocator, masm);

  allocator.discardStack(masm);
||||||| merged common ancestors
bool
BaselineCacheIRCompiler::emitMegamorphicSetElement()
{
    Register obj = allocator.useRegister(masm, reader.objOperandId());
    ValueOperand idVal = allocator.useValueRegister(masm, reader.valOperandId());
    ValueOperand val = allocator.useValueRegister(masm, reader.valOperandId());
    bool strict = reader.readBool();

    allocator.discardStack(masm);
=======
bool BaselineCacheIRCompiler::emitCallProxySetByValue() {
  JitSpew(JitSpew_Codegen, __FUNCTION__);
  Register obj = allocator.useRegister(masm, reader.objOperandId());
  ValueOperand idVal = allocator.useValueRegister(masm, reader.valOperandId());
  ValueOperand val = allocator.useValueRegister(masm, reader.valOperandId());
  bool strict = reader.readBool();
>>>>>>> upstream-releases

<<<<<<< HEAD
  AutoStubFrame stubFrame(*this);
  stubFrame.enter(masm, scratch);
||||||| merged common ancestors
    // We need a scratch register but we don't have any registers available on
    // x86, so temporarily store |obj| in the frame's scratch slot.
    int scratchOffset = BaselineFrame::reverseOffsetOfScratchValue();
    masm.storePtr(obj, Address(BaselineFrameReg, scratchOffset));
=======
  allocator.discardStack(masm);
>>>>>>> upstream-releases

<<<<<<< HEAD
  masm.Push(Imm32(strict));
  masm.Push(val);
  masm.Push(id);
  masm.Push(obj);
||||||| merged common ancestors
    AutoStubFrame stubFrame(*this);
    stubFrame.enter(masm, obj);
=======
  // We need a scratch register but we don't have any registers available on
  // x86, so temporarily store |obj| in the frame's scratch slot.
  int scratchOffset = BaselineFrame::reverseOffsetOfScratchValue();
  masm.storePtr(obj, Address(BaselineFrameReg, scratchOffset));
>>>>>>> upstream-releases

<<<<<<< HEAD
  if (!callVM(masm, AddOrUpdateSparseElementHelperInfo)) {
    return false;
  }
  stubFrame.leave(masm);
  return true;
}
||||||| merged common ancestors
    // Restore |obj|. Because we entered a stub frame we first have to load
    // the original frame pointer.
    masm.loadPtr(Address(BaselineFrameReg, 0), obj);
    masm.loadPtr(Address(obj, scratchOffset), obj);

    masm.Push(Imm32(strict));
    masm.Push(TypedOrValueRegister(MIRType::Object, AnyRegister(obj)));
    masm.Push(val);
    masm.Push(idVal);
    masm.Push(obj);

    if (!callVM(masm, SetObjectElementInfo)) {
        return false;
    }

    stubFrame.leave(masm);
    return true;
}
=======
  AutoStubFrame stubFrame(*this);
  stubFrame.enter(masm, obj);

  // Restore |obj|. Because we entered a stub frame we first have to load
  // the original frame pointer.
  masm.loadPtr(Address(BaselineFrameReg, 0), obj);
  masm.loadPtr(Address(obj, scratchOffset), obj);

  masm.Push(Imm32(strict));
  masm.Push(val);
  masm.Push(idVal);
  masm.Push(obj);

  using Fn = bool (*)(JSContext*, HandleObject, HandleValue, HandleValue, bool);
  callVM<Fn, ProxySetPropertyByValue>(masm);
>>>>>>> upstream-releases

<<<<<<< HEAD
bool BaselineCacheIRCompiler::emitCallGetSparseElementResult() {
  Register obj = allocator.useRegister(masm, reader.objOperandId());
  Register id = allocator.useRegister(masm, reader.int32OperandId());
  AutoScratchRegister scratch(allocator, masm);
||||||| merged common ancestors
bool
BaselineCacheIRCompiler::emitTypeMonitorResult()
{
    allocator.discardStack(masm);
    EmitEnterTypeMonitorIC(masm);
    return true;
}
=======
  stubFrame.leave(masm);
  return true;
}
>>>>>>> upstream-releases

<<<<<<< HEAD
  allocator.discardStack(masm);
||||||| merged common ancestors
bool
BaselineCacheIRCompiler::emitReturnFromIC()
{
    allocator.discardStack(masm);
    EmitReturnFromIC(masm);
    return true;
}
=======
bool BaselineCacheIRCompiler::emitCallAddOrUpdateSparseElementHelper() {
  JitSpew(JitSpew_Codegen, __FUNCTION__);
  Register obj = allocator.useRegister(masm, reader.objOperandId());
  Register id = allocator.useRegister(masm, reader.int32OperandId());
  ValueOperand val = allocator.useValueRegister(masm, reader.valOperandId());
  bool strict = reader.readBool();
  AutoScratchRegister scratch(allocator, masm);
>>>>>>> upstream-releases

<<<<<<< HEAD
  AutoStubFrame stubFrame(*this);
  stubFrame.enter(masm, scratch);
||||||| merged common ancestors
bool
BaselineCacheIRCompiler::emitLoadStackValue()
{
    ValueOperand val = allocator.defineValueRegister(masm, reader.valOperandId());
    Address addr = allocator.addressOf(masm, BaselineFrameSlot(reader.uint32Immediate()));
    masm.loadValue(addr, val);
    return true;
}

bool
BaselineCacheIRCompiler::emitGuardAndGetIterator()
{
    Register obj = allocator.useRegister(masm, reader.objOperandId());

    AutoScratchRegister scratch1(allocator, masm);
    AutoScratchRegister scratch2(allocator, masm);
    AutoScratchRegister niScratch(allocator, masm);
=======
  allocator.discardStack(masm);

  AutoStubFrame stubFrame(*this);
  stubFrame.enter(masm, scratch);

  masm.Push(Imm32(strict));
  masm.Push(val);
  masm.Push(id);
  masm.Push(obj);
>>>>>>> upstream-releases

<<<<<<< HEAD
  masm.Push(id);
  masm.Push(obj);
||||||| merged common ancestors
    Address iterAddr(stubAddress(reader.stubOffset()));
    Address enumeratorsAddr(stubAddress(reader.stubOffset()));
=======
  using Fn = bool (*)(JSContext * cx, HandleArrayObject obj, int32_t int_id,
                      HandleValue v, bool strict);
  callVM<Fn, AddOrUpdateSparseElementHelper>(masm);
>>>>>>> upstream-releases

<<<<<<< HEAD
  if (!callVM(masm, GetSparseElementHelperInfo)) {
    return false;
  }
  stubFrame.leave(masm);
  return true;
}
||||||| merged common ancestors
    Register output = allocator.defineRegister(masm, reader.objOperandId());
=======
  stubFrame.leave(masm);
  return true;
}
>>>>>>> upstream-releases

<<<<<<< HEAD
bool BaselineCacheIRCompiler::emitMegamorphicSetElement() {
  Register obj = allocator.useRegister(masm, reader.objOperandId());
  ValueOperand idVal = allocator.useValueRegister(masm, reader.valOperandId());
  ValueOperand val = allocator.useValueRegister(masm, reader.valOperandId());
  bool strict = reader.readBool();
||||||| merged common ancestors
    FailurePath* failure;
    if (!addFailurePath(&failure)) {
        return false;
    }
=======
bool BaselineCacheIRCompiler::emitMegamorphicSetElement() {
  JitSpew(JitSpew_Codegen, __FUNCTION__);
  Register obj = allocator.useRegister(masm, reader.objOperandId());
  ValueOperand idVal = allocator.useValueRegister(masm, reader.valOperandId());
  ValueOperand val = allocator.useValueRegister(masm, reader.valOperandId());
  bool strict = reader.readBool();
>>>>>>> upstream-releases

  allocator.discardStack(masm);

  // We need a scratch register but we don't have any registers available on
  // x86, so temporarily store |obj| in the frame's scratch slot.
  int scratchOffset = BaselineFrame::reverseOffsetOfScratchValue();
  masm.storePtr(obj, Address(BaselineFrameReg, scratchOffset));

  AutoStubFrame stubFrame(*this);
  stubFrame.enter(masm, obj);

  // Restore |obj|. Because we entered a stub frame we first have to load
  // the original frame pointer.
  masm.loadPtr(Address(BaselineFrameReg, 0), obj);
  masm.loadPtr(Address(obj, scratchOffset), obj);

  masm.Push(Imm32(strict));
  masm.Push(TypedOrValueRegister(MIRType::Object, AnyRegister(obj)));
  masm.Push(val);
  masm.Push(idVal);
  masm.Push(obj);

<<<<<<< HEAD
  if (!callVM(masm, SetObjectElementInfo)) {
    return false;
  }
||||||| merged common ancestors
    // Chain onto the active iterator stack. Note that Baseline CacheIR stub
    // code is shared across compartments within a Zone, so we can't bake in
    // compartment->enumerators here.
    masm.loadPtr(enumeratorsAddr, scratch1);
    masm.loadPtr(Address(scratch1, 0), scratch1);
    emitRegisterEnumerator(scratch1, niScratch, scratch2);
=======
  using Fn = bool (*)(JSContext*, HandleObject, HandleValue, HandleValue,
                      HandleValue, bool);
  callVM<Fn, SetObjectElementWithReceiver>(masm);
>>>>>>> upstream-releases

  stubFrame.leave(masm);
  return true;
}

<<<<<<< HEAD
bool BaselineCacheIRCompiler::emitTypeMonitorResult() {
  allocator.discardStack(masm);
  EmitEnterTypeMonitorIC(masm);
  return true;
}

bool BaselineCacheIRCompiler::emitReturnFromIC() {
  allocator.discardStack(masm);
  EmitReturnFromIC(masm);
  return true;
}

bool BaselineCacheIRCompiler::emitLoadStackValue() {
  ValueOperand val = allocator.defineValueRegister(masm, reader.valOperandId());
  Address addr =
      allocator.addressOf(masm, BaselineFrameSlot(reader.uint32Immediate()));
  masm.loadValue(addr, val);
  return true;
||||||| merged common ancestors
bool
BaselineCacheIRCompiler::emitGuardDOMExpandoMissingOrGuardShape()
{
    ValueOperand val = allocator.useValueRegister(masm, reader.valOperandId());
    AutoScratchRegister shapeScratch(allocator, masm);
    AutoScratchRegister objScratch(allocator, masm);
    Address shapeAddr(stubAddress(reader.stubOffset()));

    FailurePath* failure;
    if (!addFailurePath(&failure)) {
        return false;
    }

    Label done;
    masm.branchTestUndefined(Assembler::Equal, val, &done);

    masm.debugAssertIsObject(val);
    masm.loadPtr(shapeAddr, shapeScratch);
    masm.unboxObject(val, objScratch);
    // The expando object is not used in this case, so we don't need Spectre
    // mitigations.
    masm.branchTestObjShapeNoSpectreMitigations(Assembler::NotEqual, objScratch, shapeScratch,
                                                failure->label());

    masm.bind(&done);
    return true;
=======
bool BaselineCacheIRCompiler::emitTypeMonitorResult() {
  JitSpew(JitSpew_Codegen, __FUNCTION__);
  allocator.discardStack(masm);
  EmitEnterTypeMonitorIC(masm);
  return true;
}

bool BaselineCacheIRCompiler::emitReturnFromIC() {
  JitSpew(JitSpew_Codegen, __FUNCTION__);
  allocator.discardStack(masm);
  EmitReturnFromIC(masm);
  return true;
>>>>>>> upstream-releases
}

<<<<<<< HEAD
bool BaselineCacheIRCompiler::emitGuardAndGetIterator() {
  Register obj = allocator.useRegister(masm, reader.objOperandId());

  AutoScratchRegister scratch1(allocator, masm);
  AutoScratchRegister scratch2(allocator, masm);
  AutoScratchRegister niScratch(allocator, masm);

  Address iterAddr(stubAddress(reader.stubOffset()));
  Address enumeratorsAddr(stubAddress(reader.stubOffset()));

  Register output = allocator.defineRegister(masm, reader.objOperandId());

  FailurePath* failure;
  if (!addFailurePath(&failure)) {
    return false;
  }

  // Load our PropertyIteratorObject* and its NativeIterator.
  masm.loadPtr(iterAddr, output);
  masm.loadObjPrivate(output, JSObject::ITER_CLASS_NFIXED_SLOTS, niScratch);

  // Ensure the iterator is reusable: see NativeIterator::isReusable.
  masm.branchIfNativeIteratorNotReusable(niScratch, failure->label());
||||||| merged common ancestors
bool
BaselineCacheIRCompiler::emitLoadDOMExpandoValueGuardGeneration()
{
    Register obj = allocator.useRegister(masm, reader.objOperandId());
    Address expandoAndGenerationAddr(stubAddress(reader.stubOffset()));
    Address generationAddr(stubAddress(reader.stubOffset()));

    AutoScratchRegister scratch(allocator, masm);
    ValueOperand output = allocator.defineValueRegister(masm, reader.valOperandId());

    FailurePath* failure;
    if (!addFailurePath(&failure)) {
        return false;
    }

    masm.loadPtr(Address(obj, ProxyObject::offsetOfReservedSlots()), scratch);
    Address expandoAddr(scratch, js::detail::ProxyReservedSlots::offsetOfPrivateSlot());

    // Load the ExpandoAndGeneration* in the output scratch register and guard
    // it matches the proxy's ExpandoAndGeneration.
    masm.loadPtr(expandoAndGenerationAddr, output.scratchReg());
    masm.branchPrivatePtr(Assembler::NotEqual, expandoAddr, output.scratchReg(), failure->label());

    // Guard expandoAndGeneration->generation matches the expected generation.
    masm.branch64(Assembler::NotEqual,
                  Address(output.scratchReg(), ExpandoAndGeneration::offsetOfGeneration()),
                  generationAddr,
                  scratch, failure->label());

    // Load expandoAndGeneration->expando into the output Value register.
    masm.loadValue(Address(output.scratchReg(), ExpandoAndGeneration::offsetOfExpando()), output);
    return true;
}
=======
bool BaselineCacheIRCompiler::emitLoadArgumentFixedSlot() {
  JitSpew(JitSpew_Codegen, __FUNCTION__);
  ValueOperand resultReg =
      allocator.defineValueRegister(masm, reader.valOperandId());
  Address addr =
      allocator.addressOf(masm, BaselineFrameSlot(reader.readByte()));
  masm.loadValue(addr, resultReg);
  return true;
}
>>>>>>> upstream-releases

<<<<<<< HEAD
  // Pre-write barrier for store to 'objectBeingIterated_'.
  Address iterObjAddr(niScratch, NativeIterator::offsetOfObjectBeingIterated());
  EmitPreBarrier(masm, iterObjAddr, MIRType::Object);

  // Mark iterator as active.
  Address iterFlagsAddr(niScratch, NativeIterator::offsetOfFlags());
  masm.storePtr(obj, iterObjAddr);
  masm.or32(Imm32(NativeIterator::Flags::Active), iterFlagsAddr);

  // Post-write barrier for stores to 'objectBeingIterated_'.
  emitPostBarrierSlot(output,
                      TypedOrValueRegister(MIRType::Object, AnyRegister(obj)),
                      scratch1);

  // Chain onto the active iterator stack. Note that Baseline CacheIR stub
  // code is shared across compartments within a Zone, so we can't bake in
  // compartment->enumerators here.
  masm.loadPtr(enumeratorsAddr, scratch1);
  masm.loadPtr(Address(scratch1, 0), scratch1);
  emitRegisterEnumerator(scratch1, niScratch, scratch2);

  return true;
}

bool BaselineCacheIRCompiler::emitGuardDOMExpandoMissingOrGuardShape() {
  ValueOperand val = allocator.useValueRegister(masm, reader.valOperandId());
  AutoScratchRegister shapeScratch(allocator, masm);
  AutoScratchRegister objScratch(allocator, masm);
  Address shapeAddr(stubAddress(reader.stubOffset()));

  FailurePath* failure;
  if (!addFailurePath(&failure)) {
    return false;
  }

  Label done;
  masm.branchTestUndefined(Assembler::Equal, val, &done);

  masm.debugAssertIsObject(val);
  masm.loadPtr(shapeAddr, shapeScratch);
  masm.unboxObject(val, objScratch);
  // The expando object is not used in this case, so we don't need Spectre
  // mitigations.
  masm.branchTestObjShapeNoSpectreMitigations(Assembler::NotEqual, objScratch,
                                              shapeScratch, failure->label());

  masm.bind(&done);
  return true;
}

bool BaselineCacheIRCompiler::emitLoadDOMExpandoValueGuardGeneration() {
  Register obj = allocator.useRegister(masm, reader.objOperandId());
  Address expandoAndGenerationAddr(stubAddress(reader.stubOffset()));
  Address generationAddr(stubAddress(reader.stubOffset()));

  AutoScratchRegister scratch(allocator, masm);
  ValueOperand output =
      allocator.defineValueRegister(masm, reader.valOperandId());

  FailurePath* failure;
  if (!addFailurePath(&failure)) {
    return false;
  }

  masm.loadPtr(Address(obj, ProxyObject::offsetOfReservedSlots()), scratch);
  Address expandoAddr(scratch,
                      js::detail::ProxyReservedSlots::offsetOfPrivateSlot());

  // Load the ExpandoAndGeneration* in the output scratch register and guard
  // it matches the proxy's ExpandoAndGeneration.
  masm.loadPtr(expandoAndGenerationAddr, output.scratchReg());
  masm.branchPrivatePtr(Assembler::NotEqual, expandoAddr, output.scratchReg(),
                        failure->label());

  // Guard expandoAndGeneration->generation matches the expected generation.
  masm.branch64(
      Assembler::NotEqual,
      Address(output.scratchReg(), ExpandoAndGeneration::offsetOfGeneration()),
      generationAddr, scratch, failure->label());

  // Load expandoAndGeneration->expando into the output Value register.
  masm.loadValue(
      Address(output.scratchReg(), ExpandoAndGeneration::offsetOfExpando()),
      output);
  return true;
}

bool BaselineCacheIRCompiler::init(CacheKind kind) {
  if (!allocator.init()) {
    return false;
  }

  // Baseline ICs monitor values when needed, so returning doubles is fine.
  allowDoubleResult_.emplace(true);

  size_t numInputs = writer_.numInputOperands();

  // Baseline passes the first 2 inputs in R0/R1, other Values are stored on
  // the stack.
  size_t numInputsInRegs = std::min(numInputs, size_t(2));
  AllocatableGeneralRegisterSet available(
      ICStubCompiler::availableGeneralRegs(numInputsInRegs));

  switch (kind) {
    case CacheKind::NewObject:
    case CacheKind::GetIntrinsic:
      MOZ_ASSERT(numInputs == 0);
      break;
    case CacheKind::GetProp:
    case CacheKind::TypeOf:
    case CacheKind::GetIterator:
    case CacheKind::ToBool:
    case CacheKind::UnaryArith:
      MOZ_ASSERT(numInputs == 1);
      allocator.initInputLocation(0, R0);
      break;
    case CacheKind::Compare:
    case CacheKind::GetElem:
    case CacheKind::GetPropSuper:
    case CacheKind::SetProp:
    case CacheKind::In:
    case CacheKind::HasOwn:
    case CacheKind::InstanceOf:
    case CacheKind::BinaryArith:
      MOZ_ASSERT(numInputs == 2);
      allocator.initInputLocation(0, R0);
      allocator.initInputLocation(1, R1);
      break;
    case CacheKind::GetElemSuper:
      MOZ_ASSERT(numInputs == 3);
      allocator.initInputLocation(0, BaselineFrameSlot(0));
      allocator.initInputLocation(1, R0);
      allocator.initInputLocation(2, R1);
      break;
    case CacheKind::SetElem:
      MOZ_ASSERT(numInputs == 3);
      allocator.initInputLocation(0, R0);
      allocator.initInputLocation(1, R1);
      allocator.initInputLocation(2, BaselineFrameSlot(0));
      break;
    case CacheKind::GetName:
    case CacheKind::BindName:
      MOZ_ASSERT(numInputs == 1);
      allocator.initInputLocation(0, R0.scratchReg(), JSVAL_TYPE_OBJECT);
||||||| merged common ancestors
bool
BaselineCacheIRCompiler::init(CacheKind kind)
{
    if (!allocator.init()) {
        return false;
    }

    // Baseline ICs monitor values when needed, so returning doubles is fine.
    allowDoubleResult_.emplace(true);

    size_t numInputs = writer_.numInputOperands();

    // Baseline passes the first 2 inputs in R0/R1, other Values are stored on
    // the stack.
    size_t numInputsInRegs = std::min(numInputs, size_t(2));
    AllocatableGeneralRegisterSet available(ICStubCompiler::availableGeneralRegs(numInputsInRegs));

    switch (kind) {
      case CacheKind::NewObject:
      case CacheKind::GetIntrinsic:
        MOZ_ASSERT(numInputs == 0);
        break;
      case CacheKind::GetProp:
      case CacheKind::TypeOf:
      case CacheKind::GetIterator:
      case CacheKind::ToBool:
      case CacheKind::UnaryArith:
        MOZ_ASSERT(numInputs == 1);
        allocator.initInputLocation(0, R0);
        break;
      case CacheKind::Compare:
      case CacheKind::GetElem:
      case CacheKind::GetPropSuper:
      case CacheKind::SetProp:
      case CacheKind::In:
      case CacheKind::HasOwn:
      case CacheKind::InstanceOf:
      case CacheKind::BinaryArith:
        MOZ_ASSERT(numInputs == 2);
        allocator.initInputLocation(0, R0);
        allocator.initInputLocation(1, R1);
        break;
      case CacheKind::GetElemSuper:
        MOZ_ASSERT(numInputs == 3);
        allocator.initInputLocation(0, BaselineFrameSlot(0));
        allocator.initInputLocation(1, R0);
        allocator.initInputLocation(2, R1);
        break;
      case CacheKind::SetElem:
        MOZ_ASSERT(numInputs == 3);
        allocator.initInputLocation(0, R0);
        allocator.initInputLocation(1, R1);
        allocator.initInputLocation(2, BaselineFrameSlot(0));
        break;
      case CacheKind::GetName:
      case CacheKind::BindName:
        MOZ_ASSERT(numInputs == 1);
        allocator.initInputLocation(0, R0.scratchReg(), JSVAL_TYPE_OBJECT);
=======
bool BaselineCacheIRCompiler::emitLoadArgumentDynamicSlot() {
  JitSpew(JitSpew_Codegen, __FUNCTION__);
  ValueOperand resultReg =
      allocator.defineValueRegister(masm, reader.valOperandId());
  Register argcReg = allocator.useRegister(masm, reader.int32OperandId());
  BaseValueIndex addr =
      allocator.addressOf(masm, argcReg, BaselineFrameSlot(reader.readByte()));
  masm.loadValue(addr, resultReg);
  return true;
}

bool BaselineCacheIRCompiler::emitGuardAndGetIterator() {
  JitSpew(JitSpew_Codegen, __FUNCTION__);
  Register obj = allocator.useRegister(masm, reader.objOperandId());

  AutoScratchRegister scratch1(allocator, masm);
  AutoScratchRegister scratch2(allocator, masm);
  AutoScratchRegister niScratch(allocator, masm);

  Address iterAddr(stubAddress(reader.stubOffset()));
  Address enumeratorsAddr(stubAddress(reader.stubOffset()));

  Register output = allocator.defineRegister(masm, reader.objOperandId());

  FailurePath* failure;
  if (!addFailurePath(&failure)) {
    return false;
  }

  // Load our PropertyIteratorObject* and its NativeIterator.
  masm.loadPtr(iterAddr, output);
  masm.loadObjPrivate(output, PropertyIteratorObject::NUM_FIXED_SLOTS,
                      niScratch);

  // Ensure the iterator is reusable: see NativeIterator::isReusable.
  masm.branchIfNativeIteratorNotReusable(niScratch, failure->label());

  // Pre-write barrier for store to 'objectBeingIterated_'.
  Address iterObjAddr(niScratch, NativeIterator::offsetOfObjectBeingIterated());
  EmitPreBarrier(masm, iterObjAddr, MIRType::Object);

  // Mark iterator as active.
  Address iterFlagsAddr(niScratch, NativeIterator::offsetOfFlagsAndCount());
  masm.storePtr(obj, iterObjAddr);
  masm.or32(Imm32(NativeIterator::Flags::Active), iterFlagsAddr);

  // Post-write barrier for stores to 'objectBeingIterated_'.
  emitPostBarrierSlot(output,
                      TypedOrValueRegister(MIRType::Object, AnyRegister(obj)),
                      scratch1);

  // Chain onto the active iterator stack. Note that Baseline CacheIR stub
  // code is shared across compartments within a Zone, so we can't bake in
  // compartment->enumerators here.
  masm.loadPtr(enumeratorsAddr, scratch1);
  masm.loadPtr(Address(scratch1, 0), scratch1);
  emitRegisterEnumerator(scratch1, niScratch, scratch2);

  return true;
}

bool BaselineCacheIRCompiler::emitGuardDOMExpandoMissingOrGuardShape() {
  JitSpew(JitSpew_Codegen, __FUNCTION__);
  ValueOperand val = allocator.useValueRegister(masm, reader.valOperandId());
  AutoScratchRegister shapeScratch(allocator, masm);
  AutoScratchRegister objScratch(allocator, masm);
  Address shapeAddr(stubAddress(reader.stubOffset()));

  FailurePath* failure;
  if (!addFailurePath(&failure)) {
    return false;
  }

  Label done;
  masm.branchTestUndefined(Assembler::Equal, val, &done);

  masm.debugAssertIsObject(val);
  masm.loadPtr(shapeAddr, shapeScratch);
  masm.unboxObject(val, objScratch);
  // The expando object is not used in this case, so we don't need Spectre
  // mitigations.
  masm.branchTestObjShapeNoSpectreMitigations(Assembler::NotEqual, objScratch,
                                              shapeScratch, failure->label());

  masm.bind(&done);
  return true;
}

bool BaselineCacheIRCompiler::emitLoadDOMExpandoValueGuardGeneration() {
  JitSpew(JitSpew_Codegen, __FUNCTION__);
  Register obj = allocator.useRegister(masm, reader.objOperandId());
  Address expandoAndGenerationAddr(stubAddress(reader.stubOffset()));
  Address generationAddr(stubAddress(reader.stubOffset()));

  AutoScratchRegister scratch(allocator, masm);
  ValueOperand output =
      allocator.defineValueRegister(masm, reader.valOperandId());

  FailurePath* failure;
  if (!addFailurePath(&failure)) {
    return false;
  }

  masm.loadPtr(Address(obj, ProxyObject::offsetOfReservedSlots()), scratch);
  Address expandoAddr(scratch,
                      js::detail::ProxyReservedSlots::offsetOfPrivateSlot());

  // Load the ExpandoAndGeneration* in the output scratch register and guard
  // it matches the proxy's ExpandoAndGeneration.
  masm.loadPtr(expandoAndGenerationAddr, output.scratchReg());
  masm.branchPrivatePtr(Assembler::NotEqual, expandoAddr, output.scratchReg(),
                        failure->label());

  // Guard expandoAndGeneration->generation matches the expected generation.
  masm.branch64(
      Assembler::NotEqual,
      Address(output.scratchReg(), ExpandoAndGeneration::offsetOfGeneration()),
      generationAddr, scratch, failure->label());

  // Load expandoAndGeneration->expando into the output Value register.
  masm.loadValue(
      Address(output.scratchReg(), ExpandoAndGeneration::offsetOfExpando()),
      output);
  return true;
}

bool BaselineCacheIRCompiler::init(CacheKind kind) {
  if (!allocator.init()) {
    return false;
  }

  // Baseline ICs monitor values when needed, so returning doubles is fine.
  allowDoubleResult_.emplace(true);

  size_t numInputs = writer_.numInputOperands();

  // Baseline passes the first 2 inputs in R0/R1, other Values are stored on
  // the stack.
  size_t numInputsInRegs = std::min(numInputs, size_t(2));
  AllocatableGeneralRegisterSet available(
      ICStubCompiler::availableGeneralRegs(numInputsInRegs));

  switch (kind) {
    case CacheKind::NewObject:
    case CacheKind::GetIntrinsic:
      MOZ_ASSERT(numInputs == 0);
      break;
    case CacheKind::GetProp:
    case CacheKind::TypeOf:
    case CacheKind::GetIterator:
    case CacheKind::ToBool:
    case CacheKind::UnaryArith:
      MOZ_ASSERT(numInputs == 1);
      allocator.initInputLocation(0, R0);
      break;
    case CacheKind::Compare:
    case CacheKind::GetElem:
    case CacheKind::GetPropSuper:
    case CacheKind::SetProp:
    case CacheKind::In:
    case CacheKind::HasOwn:
    case CacheKind::InstanceOf:
    case CacheKind::BinaryArith:
      MOZ_ASSERT(numInputs == 2);
      allocator.initInputLocation(0, R0);
      allocator.initInputLocation(1, R1);
      break;
    case CacheKind::GetElemSuper:
      MOZ_ASSERT(numInputs == 3);
      allocator.initInputLocation(0, BaselineFrameSlot(0));
      allocator.initInputLocation(1, R0);
      allocator.initInputLocation(2, R1);
      break;
    case CacheKind::SetElem:
      MOZ_ASSERT(numInputs == 3);
      allocator.initInputLocation(0, R0);
      allocator.initInputLocation(1, R1);
      allocator.initInputLocation(2, BaselineFrameSlot(0));
      break;
    case CacheKind::GetName:
    case CacheKind::BindName:
      MOZ_ASSERT(numInputs == 1);
      allocator.initInputLocation(0, R0.scratchReg(), JSVAL_TYPE_OBJECT);
>>>>>>> upstream-releases
#if defined(JS_NUNBOX32)
      // availableGeneralRegs can't know that GetName/BindName is only using
      // the payloadReg and not typeReg on x86.
      available.add(R0.typeReg());
#endif
<<<<<<< HEAD
      break;
    case CacheKind::Call:
      // Calls pass argc in R0. CallICs may or may not use it.
      MOZ_ASSERT(numInputs == 0 || numInputs == 1);
      if (numInputs == 1) {
        allocator.initInputLocation(0, R0.scratchReg(), JSVAL_TYPE_INT32);
||||||| merged common ancestors
        break;
      case CacheKind::Call:
        MOZ_ASSERT(numInputs == 1);
        allocator.initInputLocation(0, R0.scratchReg(), JSVAL_TYPE_INT32);
=======
      break;
    case CacheKind::Call:
      MOZ_ASSERT(numInputs == 1);
      allocator.initInputLocation(0, R0.scratchReg(), JSVAL_TYPE_INT32);
>>>>>>> upstream-releases
#if defined(JS_NUNBOX32)
      // availableGeneralRegs can't know that Call is only using
      // the payloadReg and not typeReg on x86.
      available.add(R0.typeReg());
#endif
<<<<<<< HEAD
      }
      break;
  }
||||||| merged common ancestors
        break;
    }
=======
      break;
  }
>>>>>>> upstream-releases

  // Baseline doesn't allocate float registers so none of them are live.
  liveFloatRegs_ = LiveFloatRegisterSet(FloatRegisterSet());

  allocator.initAvailableRegs(available);
  outputUnchecked_.emplace(R0);
  return true;
}

static const size_t MaxOptimizedCacheIRStubs = 16;

<<<<<<< HEAD
static void ResetEnteredCounts(ICFallbackStub* stub) {
  for (ICStubIterator iter = stub->beginChain(); !iter.atEnd(); iter++) {
    switch (iter->kind()) {
      case ICStub::CacheIR_Regular:
        iter->toCacheIR_Regular()->resetEnteredCount();
        break;
      case ICStub::CacheIR_Updated:
        iter->toCacheIR_Updated()->resetEnteredCount();
||||||| merged common ancestors
ICStub*
js::jit::AttachBaselineCacheIRStub(JSContext* cx, const CacheIRWriter& writer,
                                   CacheKind kind, BaselineCacheIRStubKind stubKind,
                                   JSScript* outerScript, ICFallbackStub* stub,
                                   bool* attached)
{
    // We shouldn't GC or report OOM (or any other exception) here.
    AutoAssertNoPendingException aanpe(cx);
    JS::AutoCheckCannotGC nogc;

    MOZ_ASSERT(!*attached);

    if (writer.failed()) {
        return nullptr;
    }

    // Just a sanity check: the caller should ensure we don't attach an
    // unlimited number of stubs.
    MOZ_ASSERT(stub->numOptimizedStubs() < MaxOptimizedCacheIRStubs);

    uint32_t stubDataOffset = 0;
    switch (stubKind) {
      case BaselineCacheIRStubKind::Monitored:
        stubDataOffset = sizeof(ICCacheIR_Monitored);
=======
static void ResetEnteredCounts(ICFallbackStub* stub) {
  for (ICStubIterator iter = stub->beginChain(); !iter.atEnd(); iter++) {
    switch (iter->kind()) {
      case ICStub::CacheIR_Regular:
        iter->toCacheIR_Regular()->resetEnteredCount();
>>>>>>> upstream-releases
        break;
<<<<<<< HEAD
      case ICStub::CacheIR_Monitored:
        iter->toCacheIR_Monitored()->resetEnteredCount();
||||||| merged common ancestors
      case BaselineCacheIRStubKind::Regular:
        stubDataOffset = sizeof(ICCacheIR_Regular);
=======
      case ICStub::CacheIR_Updated:
        iter->toCacheIR_Updated()->resetEnteredCount();
>>>>>>> upstream-releases
        break;
<<<<<<< HEAD
      default:
||||||| merged common ancestors
      case BaselineCacheIRStubKind::Updated:
        stubDataOffset = sizeof(ICCacheIR_Updated);
=======
      case ICStub::CacheIR_Monitored:
        iter->toCacheIR_Monitored()->resetEnteredCount();
        break;
      default:
>>>>>>> upstream-releases
        break;
    }
  }
  stub->resetEnteredCount();
}

ICStub* js::jit::AttachBaselineCacheIRStub(
    JSContext* cx, const CacheIRWriter& writer, CacheKind kind,
    BaselineCacheIRStubKind stubKind, JSScript* outerScript,
    ICFallbackStub* stub, bool* attached) {
  // We shouldn't GC or report OOM (or any other exception) here.
  AutoAssertNoPendingException aanpe(cx);
  JS::AutoCheckCannotGC nogc;

  MOZ_ASSERT(!*attached);

  if (writer.failed()) {
    return nullptr;
  }

  // Just a sanity check: the caller should ensure we don't attach an
  // unlimited number of stubs.
  MOZ_ASSERT(stub->numOptimizedStubs() < MaxOptimizedCacheIRStubs);

  uint32_t stubDataOffset = 0;
  switch (stubKind) {
    case BaselineCacheIRStubKind::Monitored:
      stubDataOffset = sizeof(ICCacheIR_Monitored);
      break;
    case BaselineCacheIRStubKind::Regular:
      stubDataOffset = sizeof(ICCacheIR_Regular);
      break;
    case BaselineCacheIRStubKind::Updated:
      stubDataOffset = sizeof(ICCacheIR_Updated);
      break;
  }

  JitZone* jitZone = cx->zone()->jitZone();

  // Check if we already have JitCode for this stub.
  CacheIRStubInfo* stubInfo;
  CacheIRStubKey::Lookup lookup(kind, ICStubEngine::Baseline,
                                writer.codeStart(), writer.codeLength());
  JitCode* code = jitZone->getBaselineCacheIRStubCode(lookup, &stubInfo);
  if (!code) {
    // We have to generate stub code.
    JitContext jctx(cx, nullptr);
    BaselineCacheIRCompiler comp(cx, writer, stubDataOffset, stubKind);
    if (!comp.init(kind)) {
      return nullptr;
    }

    code = comp.compile();
    if (!code) {
      return nullptr;
    }

    // Allocate the shared CacheIRStubInfo. Note that the
    // putBaselineCacheIRStubCode call below will transfer ownership
    // to the stub code HashMap, so we don't have to worry about freeing
    // it below.
    MOZ_ASSERT(!stubInfo);
    stubInfo =
        CacheIRStubInfo::New(kind, ICStubEngine::Baseline, comp.makesGCCalls(),
                             stubDataOffset, writer);
    if (!stubInfo) {
      return nullptr;
    }

    CacheIRStubKey key(stubInfo);
    if (!jitZone->putBaselineCacheIRStubCode(lookup, key, code)) {
      return nullptr;
    }
  }

  MOZ_ASSERT(code);
  MOZ_ASSERT(stubInfo);
  MOZ_ASSERT(stubInfo->stubDataSize() == writer.stubDataSize());

  // Ensure we don't attach duplicate stubs. This can happen if a stub failed
  // for some reason and the IR generator doesn't check for exactly the same
  // conditions.
  for (ICStubConstIterator iter = stub->beginChainConst(); !iter.atEnd();
       iter++) {
    bool updated = false;
    switch (stubKind) {
      case BaselineCacheIRStubKind::Regular: {
        if (!iter->isCacheIR_Regular()) {
          continue;
        }
        auto otherStub = iter->toCacheIR_Regular();
        if (otherStub->stubInfo() != stubInfo) {
          continue;
        }
        if (!writer.stubDataEqualsMaybeUpdate(otherStub->stubDataStart(),
                                              &updated)) {
          continue;
        }
        break;
      }
      case BaselineCacheIRStubKind::Monitored: {
        if (!iter->isCacheIR_Monitored()) {
          continue;
        }
        auto otherStub = iter->toCacheIR_Monitored();
        if (otherStub->stubInfo() != stubInfo) {
          continue;
        }
        if (!writer.stubDataEqualsMaybeUpdate(otherStub->stubDataStart(),
                                              &updated)) {
          continue;
        }
        break;
      }
      case BaselineCacheIRStubKind::Updated: {
<<<<<<< HEAD
        if (!iter->isCacheIR_Updated()) {
          continue;
        }
        auto otherStub = iter->toCacheIR_Updated();
        if (otherStub->stubInfo() != stubInfo) {
          continue;
        }
        if (!writer.stubDataEqualsMaybeUpdate(otherStub->stubDataStart(),
                                              &updated)) {
          continue;
||||||| merged common ancestors
        auto newStub = new(newStubMem) ICCacheIR_Updated(code, stubInfo);
        if (!newStub->initUpdatingChain(cx, stubSpace)) {
            cx->recoverFromOutOfMemory();
            return nullptr;
=======
        if (!iter->isCacheIR_Updated()) {
          continue;
        }
        auto otherStub = iter->toCacheIR_Updated();
        if (otherStub->stubInfo() != stubInfo) {
          continue;
>>>>>>> upstream-releases
        }
<<<<<<< HEAD
        break;
||||||| merged common ancestors
        writer.copyStubData(newStub->stubDataStart());
        stub->addNewStub(newStub);
        *attached = true;
        return newStub;
=======
        if (!writer.stubDataEqualsMaybeUpdate(otherStub->stubDataStart(),
                                              &updated)) {
          continue;
        }
        break;
>>>>>>> upstream-releases
      }
    }

<<<<<<< HEAD
    // We found a stub that's exactly the same as the stub we're about to
    // attach. Just return nullptr, the caller should do nothing in this
    // case.
    if (updated) {
      *attached = true;
    }
    return nullptr;
  }

  // Time to allocate and attach a new stub.

  size_t bytesNeeded = stubInfo->stubDataOffset() + stubInfo->stubDataSize();

  ICStubSpace* stubSpace =
      ICStubCompiler::StubSpaceForStub(stubInfo->makesGCCalls(), outerScript);
  void* newStubMem = stubSpace->alloc(bytesNeeded);
  if (!newStubMem) {
    return nullptr;
  }

  // Resetting the entered counts on the IC chain makes subsequent reasoning
  // about the chain much easier.
  ResetEnteredCounts(stub);

  switch (stubKind) {
    case BaselineCacheIRStubKind::Regular: {
      auto newStub = new (newStubMem) ICCacheIR_Regular(code, stubInfo);
      writer.copyStubData(newStub->stubDataStart());
      stub->addNewStub(newStub);
      *attached = true;
      return newStub;
    }
    case BaselineCacheIRStubKind::Monitored: {
      ICTypeMonitor_Fallback* typeMonitorFallback =
          stub->toMonitoredFallbackStub()->getFallbackMonitorStub(cx,
                                                                  outerScript);
      if (!typeMonitorFallback) {
        cx->recoverFromOutOfMemory();
        return nullptr;
      }
      ICStub* monitorStub = typeMonitorFallback->firstMonitorStub();
      auto newStub =
          new (newStubMem) ICCacheIR_Monitored(code, monitorStub, stubInfo);
      writer.copyStubData(newStub->stubDataStart());
      stub->addNewStub(newStub);
      *attached = true;
      return newStub;
    }
    case BaselineCacheIRStubKind::Updated: {
      auto newStub = new (newStubMem) ICCacheIR_Updated(code, stubInfo);
      if (!newStub->initUpdatingChain(cx, stubSpace)) {
        cx->recoverFromOutOfMemory();
        return nullptr;
      }
      writer.copyStubData(newStub->stubDataStart());
      stub->addNewStub(newStub);
      *attached = true;
      return newStub;
    }
  }
||||||| merged common ancestors
    MOZ_CRASH("Invalid kind");
}

uint8_t*
ICCacheIR_Regular::stubDataStart()
{
    return reinterpret_cast<uint8_t*>(this) + stubInfo_->stubDataOffset();
}

uint8_t*
ICCacheIR_Monitored::stubDataStart()
{
    return reinterpret_cast<uint8_t*>(this) + stubInfo_->stubDataOffset();
}

uint8_t*
ICCacheIR_Updated::stubDataStart()
{
    return reinterpret_cast<uint8_t*>(this) + stubInfo_->stubDataOffset();
}

/* static */ ICCacheIR_Regular*
ICCacheIR_Regular::Clone(JSContext* cx, ICStubSpace* space, ICStub* firstMonitorStub,
                         ICCacheIR_Regular& other)
{
    const CacheIRStubInfo* stubInfo = other.stubInfo();
    MOZ_ASSERT(stubInfo->makesGCCalls());

    size_t bytesNeeded = stubInfo->stubDataOffset() + stubInfo->stubDataSize();
    void* newStub = space->alloc(bytesNeeded);
    if (!newStub) {
        return nullptr;
    }

    ICCacheIR_Regular* res = new(newStub) ICCacheIR_Regular(other.jitCode(), stubInfo);
    stubInfo->copyStubData(&other, res);
    return res;
}


/* static */ ICCacheIR_Monitored*
ICCacheIR_Monitored::Clone(JSContext* cx, ICStubSpace* space, ICStub* firstMonitorStub,
                           ICCacheIR_Monitored& other)
{
    const CacheIRStubInfo* stubInfo = other.stubInfo();
    MOZ_ASSERT(stubInfo->makesGCCalls());

    size_t bytesNeeded = stubInfo->stubDataOffset() + stubInfo->stubDataSize();
    void* newStub = space->alloc(bytesNeeded);
    if (!newStub) {
        return nullptr;
    }
=======
    // We found a stub that's exactly the same as the stub we're about to
    // attach. Just return nullptr, the caller should do nothing in this
    // case.
    if (updated) {
      *attached = true;
    } else {
      JitSpew(JitSpew_BaselineICFallback,
              "Tried attaching identical stub for (%s:%u:%u)",
              outerScript->filename(), outerScript->lineno(),
              outerScript->column());
    }
    return nullptr;
  }

  // Time to allocate and attach a new stub.

  size_t bytesNeeded = stubInfo->stubDataOffset() + stubInfo->stubDataSize();

  ICStubSpace* stubSpace =
      ICStubCompiler::StubSpaceForStub(stubInfo->makesGCCalls(), outerScript);
  void* newStubMem = stubSpace->alloc(bytesNeeded);
  if (!newStubMem) {
    return nullptr;
  }

  // Resetting the entered counts on the IC chain makes subsequent reasoning
  // about the chain much easier.
  ResetEnteredCounts(stub);

  switch (stubKind) {
    case BaselineCacheIRStubKind::Regular: {
      auto newStub = new (newStubMem) ICCacheIR_Regular(code, stubInfo);
      writer.copyStubData(newStub->stubDataStart());
      stub->addNewStub(newStub);
      *attached = true;
      return newStub;
    }
    case BaselineCacheIRStubKind::Monitored: {
      ICTypeMonitor_Fallback* typeMonitorFallback =
          stub->toMonitoredFallbackStub()->getFallbackMonitorStub(cx,
                                                                  outerScript);
      if (!typeMonitorFallback) {
        cx->recoverFromOutOfMemory();
        return nullptr;
      }
      ICStub* monitorStub = typeMonitorFallback->firstMonitorStub();
      auto newStub =
          new (newStubMem) ICCacheIR_Monitored(code, monitorStub, stubInfo);
      writer.copyStubData(newStub->stubDataStart());
      stub->addNewStub(newStub);
      *attached = true;
      return newStub;
    }
    case BaselineCacheIRStubKind::Updated: {
      auto newStub = new (newStubMem) ICCacheIR_Updated(code, stubInfo);
      if (!newStub->initUpdatingChain(cx, stubSpace)) {
        cx->recoverFromOutOfMemory();
        return nullptr;
      }
      writer.copyStubData(newStub->stubDataStart());
      stub->addNewStub(newStub);
      *attached = true;
      return newStub;
    }
  }

  MOZ_CRASH("Invalid kind");
}

uint8_t* ICCacheIR_Regular::stubDataStart() {
  return reinterpret_cast<uint8_t*>(this) + stubInfo_->stubDataOffset();
}

uint8_t* ICCacheIR_Monitored::stubDataStart() {
  return reinterpret_cast<uint8_t*>(this) + stubInfo_->stubDataOffset();
}

uint8_t* ICCacheIR_Updated::stubDataStart() {
  return reinterpret_cast<uint8_t*>(this) + stubInfo_->stubDataOffset();
}

bool BaselineCacheIRCompiler::emitCallStringConcatResult() {
  JitSpew(JitSpew_Codegen, __FUNCTION__);
  AutoOutputRegister output(*this);
  Register lhs = allocator.useRegister(masm, reader.stringOperandId());
  Register rhs = allocator.useRegister(masm, reader.stringOperandId());
  AutoScratchRegisterMaybeOutput scratch(allocator, masm, output);

  allocator.discardStack(masm);

  AutoStubFrame stubFrame(*this);
  stubFrame.enter(masm, scratch);

  masm.push(rhs);
  masm.push(lhs);

  using Fn = JSString* (*)(JSContext*, HandleString, HandleString);
  callVM<Fn, ConcatStrings<CanGC>>(masm);

  masm.tagValue(JSVAL_TYPE_STRING, ReturnReg, output.valueReg());

  stubFrame.leave(masm);
  return true;
}

bool BaselineCacheIRCompiler::emitCallStringObjectConcatResult() {
  JitSpew(JitSpew_Codegen, __FUNCTION__);
  ValueOperand lhs = allocator.useValueRegister(masm, reader.valOperandId());
  ValueOperand rhs = allocator.useValueRegister(masm, reader.valOperandId());

  allocator.discardStack(masm);

  // For the expression decompiler
  EmitRestoreTailCallReg(masm);
  masm.pushValue(lhs);
  masm.pushValue(rhs);

  masm.pushValue(rhs);
  masm.pushValue(lhs);

  using Fn = bool (*)(JSContext*, HandleValue, HandleValue, MutableHandleValue);
  tailCallVM<Fn, DoConcatStringObject>(masm);

  return true;
}

// The value of argc entering the call IC is not always the value of
// argc entering the callee. (For example, argc for a spread call IC
// is always 1, but argc for the callee is the length of the array.)
// In these cases, we update argc as part of the call op itself, to
// avoid modifying input operands while it is still possible to fail a
// guard. We also limit callee argc to a reasonable value to avoid
// blowing the stack limit.
bool BaselineCacheIRCompiler::updateArgc(CallFlags flags, Register argcReg,
                                         Register scratch) {
  static_assert(CacheIRCompiler::MAX_ARGS_ARRAY_LENGTH <= ARGS_LENGTH_MAX,
                "maximum arguments length for optimized stub should be <= "
                "ARGS_LENGTH_MAX");

  CallFlags::ArgFormat format = flags.getArgFormat();
  switch (format) {
    case CallFlags::Standard:
      // Standard calls have no extra guards, and argc is already correct.
      return true;
    case CallFlags::FunCall:
      // fun_call has no extra guards, and argc will be corrected in
      // pushFunCallArguments.
      return true;
    case CallFlags::FunApplyArray: {
      // GuardFunApply array already guarded argc while checking for
      // holes in the array, so we don't need to guard again here. We
      // do still need to update argc.
      BaselineFrameSlot slot(0);
      masm.unboxObject(allocator.addressOf(masm, slot), argcReg);
      masm.loadPtr(Address(argcReg, NativeObject::offsetOfElements()), argcReg);
      masm.load32(Address(argcReg, ObjectElements::offsetOfLength()), argcReg);
      return true;
    }
    default:
      break;
  }

  // We need to guard the length of the arguments.
  FailurePath* failure;
  if (!addFailurePath(&failure)) {
    return false;
  }

  // Load callee argc into scratch.
  switch (flags.getArgFormat()) {
    case CallFlags::Spread: {
      // Load the length of the elements.
      BaselineFrameSlot slot(flags.isConstructing());
      masm.unboxObject(allocator.addressOf(masm, slot), scratch);
      masm.loadPtr(Address(scratch, NativeObject::offsetOfElements()), scratch);
      masm.load32(Address(scratch, ObjectElements::offsetOfLength()), scratch);
    } break;
    case CallFlags::FunApplyArgs: {
      // The length of |arguments| is stored in the baseline frame.
      Address numActualArgsAddr(BaselineFrameReg,
                                BaselineFrame::offsetOfNumActualArgs());
      masm.load32(numActualArgsAddr, scratch);
    } break;
    default:
      MOZ_CRASH("Unknown arg format");
  }

  // Ensure that callee argc does not exceed the limit.
  masm.branch32(Assembler::Above, scratch,
                Imm32(CacheIRCompiler::MAX_ARGS_ARRAY_LENGTH),
                failure->label());

  // We're past the final guard. Update argc with the new value.
  masm.move32(scratch, argcReg);

  return true;
}

bool BaselineCacheIRCompiler::emitGuardFunApply() {
  JitSpew(JitSpew_Codegen, __FUNCTION__);
  Register argcReg = allocator.useRegister(masm, reader.int32OperandId());
  AutoScratchRegister scratch(allocator, masm);
  AutoScratchRegister scratch2(allocator, masm);
  CallFlags flags = reader.callFlags();

  FailurePath* failure;
  if (!addFailurePath(&failure)) {
    return false;
  }

  // Ensure argc == 2
  masm.branch32(Assembler::NotEqual, argcReg, Imm32(2), failure->label());

  // Stack layout is (bottom to top):
  //   Callee (fun_apply)
  //   ThisValue (target)
  //   Arg0 (new this)
  //   Arg1 (argument array)

  Address argsAddr = allocator.addressOf(masm, BaselineFrameSlot(0));
  switch (flags.getArgFormat()) {
    case CallFlags::FunApplyArgs: {
      // Ensure that args is magic |arguments|.
      masm.branchTestMagic(Assembler::NotEqual, argsAddr, failure->label());

      // Ensure that this frame doesn't have an arguments object.
      Address flagAddr(BaselineFrameReg, BaselineFrame::reverseOffsetOfFlags());
      masm.branchTest32(Assembler::NonZero, flagAddr,
                        Imm32(BaselineFrame::HAS_ARGS_OBJ), failure->label());
    } break;
    case CallFlags::FunApplyArray: {
      // Ensure that args is an array object.
      masm.branchTestObject(Assembler::NotEqual, argsAddr, failure->label());
      masm.unboxObject(argsAddr, scratch);
      const Class* clasp = &ArrayObject::class_;
      masm.branchTestObjClass(Assembler::NotEqual, scratch, clasp, scratch2,
                              scratch, failure->label());

      // Get the array elements and length
      Register elementsReg = scratch;
      masm.loadPtr(Address(scratch, NativeObject::offsetOfElements()),
                   elementsReg);
      Register calleeArgcReg = scratch2;
      masm.load32(Address(elementsReg, ObjectElements::offsetOfLength()),
                  calleeArgcReg);

      // Ensure that callee argc does not exceed the limit.  Note that
      // we do this earlier for FunApplyArray than for FunApplyArgs,
      // because we don't want to loop over every element of the array
      // looking for holes if we already know it is too long.
      masm.branch32(Assembler::Above, calleeArgcReg,
                    Imm32(CacheIRCompiler::MAX_ARGS_ARRAY_LENGTH),
                    failure->label());

      // Ensure that length == initializedLength
      Address initLenAddr(elementsReg,
                          ObjectElements::offsetOfInitializedLength());
      masm.branch32(Assembler::NotEqual, initLenAddr, calleeArgcReg,
                    failure->label());

      // Ensure no holes. Loop through array and verify no elements are magic.
      Register start = elementsReg;
      Register end = scratch2;
      BaseValueIndex endAddr(elementsReg, calleeArgcReg);
      masm.computeEffectiveAddress(endAddr, end);

      Label loop;
      Label endLoop;
      masm.bind(&loop);
      masm.branchPtr(Assembler::AboveOrEqual, start, end, &endLoop);
      masm.branchTestMagic(Assembler::Equal, Address(start, 0),
                           failure->label());
      masm.addPtr(Imm32(sizeof(Value)), start);
      masm.jump(&loop);
      masm.bind(&endLoop);
    } break;
    default:
      MOZ_CRASH("Invalid arg format");
      break;
  }
  return true;
}

void BaselineCacheIRCompiler::pushArguments(Register argcReg,
                                            Register calleeReg,
                                            Register scratch, Register scratch2,
                                            CallFlags flags, bool isJitCall) {
  switch (flags.getArgFormat()) {
    case CallFlags::Standard:
      pushStandardArguments(argcReg, scratch, scratch2, isJitCall,
                            flags.isConstructing());
      break;
    case CallFlags::Spread:
      pushArrayArguments(argcReg, scratch, scratch2, isJitCall,
                         flags.isConstructing());
      break;
    case CallFlags::FunCall:
      pushFunCallArguments(argcReg, calleeReg, scratch, scratch2, isJitCall);
      break;
    case CallFlags::FunApplyArgs:
      pushFunApplyArgs(argcReg, calleeReg, scratch, scratch2, isJitCall);
      break;
    case CallFlags::FunApplyArray:
      pushArrayArguments(argcReg, scratch, scratch2, isJitCall,
                         /*isConstructing =*/false);
      break;
    default:
      MOZ_CRASH("Invalid arg format");
  }
}

void BaselineCacheIRCompiler::pushStandardArguments(Register argcReg,
                                                    Register scratch,
                                                    Register scratch2,
                                                    bool isJitCall,
                                                    bool isConstructing) {
  // The arguments to the call IC are pushed on the stack left-to-right.
  // Our calling conventions want them right-to-left in the callee, so
  // we duplicate them on the stack in reverse order.

  // countReg contains the total number of arguments to copy.
  // In addition to the actual arguments, we have to copy hidden arguments.
  // We always have to copy |this|.
  // If we are constructing, we have to copy |newTarget|.
  // If we are not a jit call, we have to copy |callee|.
  // We use a scratch register to avoid clobbering argc, which is an input reg.
  Register countReg = scratch;
  masm.move32(argcReg, countReg);
  masm.add32(Imm32(1 + !isJitCall + isConstructing), countReg);

  // argPtr initially points to the last argument. Skip the stub frame.
  Register argPtr = scratch2;
  Address argAddress(masm.getStackPointer(), STUB_FRAME_SIZE);
  masm.computeEffectiveAddress(argAddress, argPtr);

  // Align the stack such that the JitFrameLayout is aligned on the
  // JitStackAlignment.
  if (isJitCall) {
    masm.alignJitStackBasedOnNArgs(countReg, /*countIncludesThis = */ true);
  }

  // Push all values, starting at the last one.
  Label loop, done;
  masm.bind(&loop);
  masm.branchTest32(Assembler::Zero, countReg, countReg, &done);
  {
    masm.pushValue(Address(argPtr, 0));
    masm.addPtr(Imm32(sizeof(Value)), argPtr);

    masm.sub32(Imm32(1), countReg);
    masm.jump(&loop);
  }
  masm.bind(&done);
}

void BaselineCacheIRCompiler::pushArrayArguments(Register argcReg,
                                                 Register scratch,
                                                 Register scratch2,
                                                 bool isJitCall,
                                                 bool isConstructing) {
  // Pull the array off the stack before aligning.
  Register startReg = scratch;
  masm.unboxObject(Address(masm.getStackPointer(),
                           (isConstructing * sizeof(Value)) + STUB_FRAME_SIZE),
                   startReg);
  masm.loadPtr(Address(startReg, NativeObject::offsetOfElements()), startReg);

  // Align the stack such that the JitFrameLayout is aligned on the
  // JitStackAlignment.
  if (isJitCall) {
    Register alignReg = argcReg;
    if (isConstructing) {
      // If we are constructing, we must take newTarget into account.
      alignReg = scratch2;
      masm.computeEffectiveAddress(Address(argcReg, 1), alignReg);
    }
    masm.alignJitStackBasedOnNArgs(alignReg, /*countIncludesThis =*/false);
  }

  // Push newTarget, if necessary
  if (isConstructing) {
    masm.pushValue(Address(BaselineFrameReg, STUB_FRAME_SIZE));
  }

  // Push arguments: set up endReg to point to &array[argc]
  Register endReg = scratch2;
  BaseValueIndex endAddr(startReg, argcReg);
  masm.computeEffectiveAddress(endAddr, endReg);

  // Copying pre-decrements endReg by 8 until startReg is reached
  Label copyDone;
  Label copyStart;
  masm.bind(&copyStart);
  masm.branchPtr(Assembler::Equal, endReg, startReg, &copyDone);
  masm.subPtr(Imm32(sizeof(Value)), endReg);
  masm.pushValue(Address(endReg, 0));
  masm.jump(&copyStart);
  masm.bind(&copyDone);

  // Push |this|.
  masm.pushValue(
      Address(BaselineFrameReg,
              STUB_FRAME_SIZE + (1 + isConstructing) * sizeof(Value)));

  // Push |callee| if needed.
  if (!isJitCall) {
    masm.pushValue(
        Address(BaselineFrameReg,
                STUB_FRAME_SIZE + (2 + isConstructing) * sizeof(Value)));
  }
}

void BaselineCacheIRCompiler::pushFunCallArguments(Register argcReg,
                                                   Register calleeReg,
                                                   Register scratch,
                                                   Register scratch2,
                                                   bool isJitCall) {
  Label zeroArgs, done;
  masm.branchTest32(Assembler::Zero, argcReg, argcReg, &zeroArgs);

  // When we call fun_call, the stack looks like the left column (note
  // that newTarget will not be present, because fun_call cannot be a
  // constructor call):
  //
  // ***Arguments to fun_call***
  // callee (fun_call)               ***Arguments to target***
  // this (target function)   -----> callee
  // arg0 (this of target)    -----> this
  // arg1 (arg0 of target)    -----> arg0
  // argN (argN-1 of target)  -----> arg1
  //
  // As demonstrated in the right column, this is exactly what we need
  // the stack to look like when calling pushStandardArguments for target,
  // except with one more argument. If we subtract 1 from argc,
  // everything works out correctly.
  masm.sub32(Imm32(1), argcReg);

  pushStandardArguments(argcReg, scratch, scratch2, isJitCall,
                        /*isConstructing =*/false);

  masm.jump(&done);
  masm.bind(&zeroArgs);

  // The exception is the case where argc == 0:
  //
  // ***Arguments to fun_call***
  // callee (fun_call)               ***Arguments to target***
  // this (target function)   -----> callee
  // <nothing>                -----> this
  //
  // In this case, we push |undefined| for |this|.

  if (isJitCall) {
    // Align the stack to 0 args.
    masm.alignJitStackBasedOnNArgs(0);
  }

  // Store the new |this|.
  masm.pushValue(UndefinedValue());

  // Store |callee| if needed.
  if (!isJitCall) {
    masm.Push(TypedOrValueRegister(MIRType::Object, AnyRegister(calleeReg)));
  }

  masm.bind(&done);
}

void BaselineCacheIRCompiler::pushFunApplyArgs(Register argcReg,
                                               Register calleeReg,
                                               Register scratch,
                                               Register scratch2,
                                               bool isJitCall) {
  // Push the caller's arguments onto the stack.

  // Find the start of the caller's arguments.
  Register startReg = scratch;
  masm.loadPtr(Address(BaselineFrameReg, 0), startReg);
  masm.addPtr(Imm32(BaselineFrame::offsetOfArg(0)), startReg);

  if (isJitCall) {
    masm.alignJitStackBasedOnNArgs(argcReg, /*countIncludesThis =*/false);
  }

  Register endReg = scratch2;
  BaseValueIndex endAddr(startReg, argcReg);
  masm.computeEffectiveAddress(endAddr, endReg);

  // Copying pre-decrements endReg by 8 until startReg is reached
  Label copyDone;
  Label copyStart;
  masm.bind(&copyStart);
  masm.branchPtr(Assembler::Equal, endReg, startReg, &copyDone);
  masm.subPtr(Imm32(sizeof(Value)), endReg);
  masm.pushValue(Address(endReg, 0));
  masm.jump(&copyStart);
  masm.bind(&copyDone);

  // Push arg0 as |this| for call
  masm.pushValue(Address(BaselineFrameReg, STUB_FRAME_SIZE + sizeof(Value)));

  // Push |callee| if needed.
  if (!isJitCall) {
    masm.Push(TypedOrValueRegister(MIRType::Object, AnyRegister(calleeReg)));
  }
}

bool BaselineCacheIRCompiler::emitCallNativeShared(NativeCallType callType) {
  AutoOutputRegister output(*this);
  AutoScratchRegisterMaybeOutput scratch(allocator, masm, output);
  AutoScratchRegister scratch2(allocator, masm);

  Register calleeReg = allocator.useRegister(masm, reader.objOperandId());
  Register argcReg = allocator.useRegister(masm, reader.int32OperandId());

  CallFlags flags = reader.callFlags();
  bool isConstructing = flags.isConstructing();
  bool isSameRealm = flags.isSameRealm();

  if (!updateArgc(flags, argcReg, scratch)) {
    return false;
  }

  allocator.discardStack(masm);

  // Push a stub frame so that we can perform a non-tail call.
  // Note that this leaves the return address in TailCallReg.
  AutoStubFrame stubFrame(*this);
  stubFrame.enter(masm, scratch);

  if (!isSameRealm) {
    masm.switchToObjectRealm(calleeReg, scratch);
  }

  pushArguments(argcReg, calleeReg, scratch, scratch2, flags,
                /*isJitCall =*/false);

  // Native functions have the signature:
  //
  //    bool (*)(JSContext*, unsigned, Value* vp)
  //
  // Where vp[0] is space for callee/return value, vp[1] is |this|, and vp[2]
  // onward are the function arguments.

  // Initialize vp.
  masm.moveStackPtrTo(scratch2.get());

  // Construct a native exit frame.
  masm.push(argcReg);

  EmitBaselineCreateStubFrameDescriptor(masm, scratch, ExitFrameLayout::Size());
  masm.push(scratch);
  masm.push(ICTailCallReg);
  masm.loadJSContext(scratch);
  masm.enterFakeExitFrameForNative(scratch, scratch, isConstructing);

  // Execute call.
  masm.setupUnalignedABICall(scratch);
  masm.loadJSContext(scratch);
  masm.passABIArg(scratch);
  masm.passABIArg(argcReg);
  masm.passABIArg(scratch2);

  switch (callType) {
    case NativeCallType::Native: {
#ifdef JS_SIMULATOR
      // The simulator requires VM calls to be redirected to a special
      // swi instruction to handle them, so we store the redirected
      // pointer in the stub and use that instead of the original one.
      // (See CacheIRWriter::callNativeFunction.)
      Address redirectedAddr(stubAddress(reader.stubOffset()));
      masm.callWithABI(redirectedAddr);
#else
      bool ignoresReturnValue = reader.readBool();
      if (ignoresReturnValue) {
        masm.loadPtr(Address(calleeReg, JSFunction::offsetOfJitInfo()),
                     calleeReg);
        masm.callWithABI(
            Address(calleeReg, JSJitInfo::offsetOfIgnoresReturnValueNative()));
      } else {
        masm.callWithABI(Address(calleeReg, JSFunction::offsetOfNative()));
      }
#endif
    } break;
    case NativeCallType::ClassHook: {
      Address nativeAddr(stubAddress(reader.stubOffset()));
      masm.callWithABI(nativeAddr);
    } break;
  }

  // Test for failure.
  masm.branchIfFalseBool(ReturnReg, masm.exceptionLabel());

  // Load the return value.
  masm.loadValue(
      Address(masm.getStackPointer(), NativeExitFrameLayout::offsetOfResult()),
      output.valueReg());

  stubFrame.leave(masm);

  if (!isSameRealm) {
    masm.switchToBaselineFrameRealm(scratch2);
  }

  return true;
}

bool BaselineCacheIRCompiler::emitCallNativeFunction() {
  JitSpew(JitSpew_Codegen, __FUNCTION__);
  return emitCallNativeShared(NativeCallType::Native);
}

bool BaselineCacheIRCompiler::emitCallClassHook() {
  JitSpew(JitSpew_Codegen, __FUNCTION__);
  return emitCallNativeShared(NativeCallType::ClassHook);
}

// Helper function for loading call arguments from the stack.  Loads
// and unboxes an object from a specific slot. |stackPushed| is the
// size of the data pushed on top of the call arguments in the current
// frame. It must be tracked manually by the caller. (createThis is
// currently the only caller. If more callers are added, it might be
// worth improving the stack depth story.)
void BaselineCacheIRCompiler::loadStackObject(ArgumentKind kind,
                                              CallFlags flags,
                                              size_t stackPushed,
                                              Register argcReg, Register dest) {
  bool addArgc = false;
  int32_t slotIndex = GetIndexOfArgument(kind, flags, &addArgc);

  if (addArgc) {
    int32_t slotOffset = slotIndex * sizeof(JS::Value) + stackPushed;
    BaseValueIndex slotAddr(masm.getStackPointer(), argcReg, slotOffset);
    masm.unboxObject(slotAddr, dest);
  } else {
    int32_t slotOffset = slotIndex * sizeof(JS::Value) + stackPushed;
    Address slotAddr(masm.getStackPointer(), slotOffset);
    masm.unboxObject(slotAddr, dest);
  }
}

/*
 * Scripted constructors require a |this| object to be created prior to the
 * call. When this function is called, the stack looks like (bottom->top):
 *
 * [..., Callee, ThisV, Arg0V, ..., ArgNV, NewTarget, StubFrameHeader]
 *
 * At this point, |ThisV| is JSWhyMagic::JS_IS_CONSTRUCTING.
 *
 * This function calls CreateThis to generate a new |this| object, then
 * overwrites the magic ThisV on the stack.
 */
void BaselineCacheIRCompiler::createThis(Register argcReg, Register calleeReg,
                                         Register scratch, CallFlags flags) {
  MOZ_ASSERT(flags.isConstructing());

  size_t depth = STUB_FRAME_SIZE;

  // Save argc before call.
  masm.push(argcReg);
  depth += sizeof(size_t);

  // CreateThis takes two arguments: callee, and newTarget.

  // Push newTarget:
  loadStackObject(ArgumentKind::NewTarget, flags, depth, argcReg, scratch);
  masm.push(scratch);
  depth += sizeof(JSObject*);

  // Push callee:
  loadStackObject(ArgumentKind::Callee, flags, depth, argcReg, scratch);
  masm.push(scratch);

  // Call CreateThis
  using Fn =
      bool (*)(JSContext*, HandleObject, HandleObject, MutableHandleValue);
  callVM<Fn, CreateThis>(masm);

#ifdef DEBUG
  Label createdThisOK;
  masm.branchTestObject(Assembler::Equal, JSReturnOperand, &createdThisOK);
  masm.branchTestMagic(Assembler::Equal, JSReturnOperand, &createdThisOK);
  masm.assumeUnreachable(
      "The return of CreateThis must be an object or uninitialized.");
  masm.bind(&createdThisOK);
#endif

  // Restore argc
  masm.pop(argcReg);

  // Save |this| value back into pushed arguments on stack.
  switch (flags.getArgFormat()) {
    case CallFlags::Standard: {
      BaseValueIndex thisAddress(masm.getStackPointer(),
                                 argcReg,               // Arguments
                                 1 * sizeof(Value) +    // NewTarget
                                     STUB_FRAME_SIZE);  // Stub frame
      masm.storeValue(JSReturnOperand, thisAddress);
    } break;
    case CallFlags::Spread: {
      Address thisAddress(masm.getStackPointer(),
                          2 * sizeof(Value) +    // Arg array, NewTarget
                              STUB_FRAME_SIZE);  // Stub frame
      masm.storeValue(JSReturnOperand, thisAddress);
    } break;
    default:
      MOZ_CRASH("Invalid arg format for scripted constructor");
  }

  // Restore the stub register from the baseline stub frame.
  Address stubRegAddress(masm.getStackPointer(), STUB_FRAME_SAVED_STUB_OFFSET);
  masm.loadPtr(stubRegAddress, ICStubReg);

  // Restore calleeReg.
  depth = STUB_FRAME_SIZE;
  loadStackObject(ArgumentKind::Callee, flags, depth, argcReg, calleeReg);
}

void BaselineCacheIRCompiler::updateReturnValue() {
  Label skipThisReplace;
  masm.branchTestObject(Assembler::Equal, JSReturnOperand, &skipThisReplace);

  // If a constructor does not explicitly return an object, the return value
  // of the constructor is |this|. We load it out of the baseline stub frame.

  // At this point, the stack looks like this:
  //  newTarget
  //  ArgN
  //  ...
  //  Arg0
  //  ThisVal         <---- We want this value.
  //  argc                  ^
  //  Callee token          | Skip three stack slots.
  //  Frame descriptor      v
  //  [Top of stack]
  Address thisAddress(masm.getStackPointer(), 3 * sizeof(size_t));
  masm.loadValue(thisAddress, JSReturnOperand);
>>>>>>> upstream-releases

<<<<<<< HEAD
  MOZ_CRASH("Invalid kind");
||||||| merged common ancestors
    ICCacheIR_Monitored* res = new(newStub) ICCacheIR_Monitored(other.jitCode(), firstMonitorStub,
                                                                stubInfo);
    stubInfo->copyStubData(&other, res);
    return res;
=======
#ifdef DEBUG
  masm.branchTestObject(Assembler::Equal, JSReturnOperand, &skipThisReplace);
  masm.assumeUnreachable("Return of constructing call should be an object.");
#endif
  masm.bind(&skipThisReplace);
>>>>>>> upstream-releases
}

<<<<<<< HEAD
uint8_t* ICCacheIR_Regular::stubDataStart() {
  return reinterpret_cast<uint8_t*>(this) + stubInfo_->stubDataOffset();
}
||||||| merged common ancestors
/* static */ ICCacheIR_Updated*
ICCacheIR_Updated::Clone(JSContext* cx, ICStubSpace* space, ICStub* firstMonitorStub,
                         ICCacheIR_Updated& other)
{
    const CacheIRStubInfo* stubInfo = other.stubInfo();
    MOZ_ASSERT(stubInfo->makesGCCalls());

    size_t bytesNeeded = stubInfo->stubDataOffset() + stubInfo->stubDataSize();
    void* newStub = space->alloc(bytesNeeded);
    if (!newStub) {
        return nullptr;
    }
=======
bool BaselineCacheIRCompiler::emitCallScriptedFunction() {
  JitSpew(JitSpew_Codegen, __FUNCTION__);
  AutoOutputRegister output(*this);
  AutoScratchRegisterMaybeOutput scratch(allocator, masm, output);
  AutoScratchRegister scratch2(allocator, masm);

  Register calleeReg = allocator.useRegister(masm, reader.objOperandId());
  Register argcReg = allocator.useRegister(masm, reader.int32OperandId());
>>>>>>> upstream-releases

<<<<<<< HEAD
uint8_t* ICCacheIR_Monitored::stubDataStart() {
  return reinterpret_cast<uint8_t*>(this) + stubInfo_->stubDataOffset();
}
||||||| merged common ancestors
    ICCacheIR_Updated* res = new(newStub) ICCacheIR_Updated(other.jitCode(), stubInfo);
    res->updateStubGroup() = other.updateStubGroup();
    res->updateStubId() = other.updateStubId();
=======
  CallFlags flags = reader.callFlags();
  bool isConstructing = flags.isConstructing();
  bool isSameRealm = flags.isSameRealm();
>>>>>>> upstream-releases

<<<<<<< HEAD
uint8_t* ICCacheIR_Updated::stubDataStart() {
  return reinterpret_cast<uint8_t*>(this) + stubInfo_->stubDataOffset();
}
||||||| merged common ancestors
    stubInfo->copyStubData(&other, res);
    return res;
}
=======
  if (!updateArgc(flags, argcReg, scratch)) {
    return false;
  }
>>>>>>> upstream-releases

<<<<<<< HEAD
bool BaselineCacheIRCompiler::emitCallStringConcatResult() {
  AutoOutputRegister output(*this);
  Register lhs = allocator.useRegister(masm, reader.stringOperandId());
  Register rhs = allocator.useRegister(masm, reader.stringOperandId());
  AutoScratchRegisterMaybeOutput scratch(allocator, masm, output);
||||||| merged common ancestors
bool
BaselineCacheIRCompiler::emitCallStringConcatResult()
{
    AutoOutputRegister output(*this);
    Register lhs = allocator.useRegister(masm, reader.stringOperandId());
    Register rhs = allocator.useRegister(masm, reader.stringOperandId());
    AutoScratchRegisterMaybeOutput scratch(allocator, masm, output);
=======
  allocator.discardStack(masm);
>>>>>>> upstream-releases

<<<<<<< HEAD
  allocator.discardStack(masm);
||||||| merged common ancestors
    allocator.discardStack(masm);
=======
  // Push a stub frame so that we can perform a non-tail call.
  // Note that this leaves the return address in TailCallReg.
  AutoStubFrame stubFrame(*this);
  stubFrame.enter(masm, scratch);
>>>>>>> upstream-releases

<<<<<<< HEAD
  AutoStubFrame stubFrame(*this);
  stubFrame.enter(masm, scratch);
||||||| merged common ancestors
    AutoStubFrame stubFrame(*this);
    stubFrame.enter(masm, scratch);
=======
  if (!isSameRealm) {
    masm.switchToObjectRealm(calleeReg, scratch);
  }
>>>>>>> upstream-releases

<<<<<<< HEAD
  masm.push(rhs);
  masm.push(lhs);
||||||| merged common ancestors
    masm.push(rhs);
    masm.push(lhs);
=======
  if (isConstructing) {
    createThis(argcReg, calleeReg, scratch, flags);
  }
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
  pushArguments(argcReg, calleeReg, scratch, scratch2, flags,
                /*isJitCall =*/true);
>>>>>>> upstream-releases

<<<<<<< HEAD
  masm.tagValue(JSVAL_TYPE_STRING, ReturnReg, output.valueReg());
||||||| merged common ancestors
    masm.tagValue(JSVAL_TYPE_STRING, ReturnReg, output.valueReg());
=======
  // Load the start of the target JitCode.
  Register code = scratch2;
  masm.loadJitCodeRaw(calleeReg, code);
>>>>>>> upstream-releases

<<<<<<< HEAD
  stubFrame.leave(masm);
  return true;
}
||||||| merged common ancestors
    stubFrame.leave(masm);
    return true;
}
=======
  EmitBaselineCreateStubFrameDescriptor(masm, scratch, JitFrameLayout::Size());
>>>>>>> upstream-releases

<<<<<<< HEAD
bool BaselineCacheIRCompiler::emitCallStringObjectConcatResult() {
  ValueOperand lhs = allocator.useValueRegister(masm, reader.valOperandId());
  ValueOperand rhs = allocator.useValueRegister(masm, reader.valOperandId());
||||||| merged common ancestors
bool
BaselineCacheIRCompiler::emitCallStringObjectConcatResult()
{
    ValueOperand lhs = allocator.useValueRegister(masm, reader.valOperandId());
    ValueOperand rhs = allocator.useValueRegister(masm, reader.valOperandId());
=======
  // Note that we use Push, not push, so that callJit will align the stack
  // properly on ARM.
  masm.Push(argcReg);
  masm.PushCalleeToken(calleeReg, isConstructing);
  masm.Push(scratch);
>>>>>>> upstream-releases

<<<<<<< HEAD
  allocator.discardStack(masm);
||||||| merged common ancestors
    allocator.discardStack(masm);
=======
  // Handle arguments underflow.
  Label noUnderflow;
  masm.load16ZeroExtend(Address(calleeReg, JSFunction::offsetOfNargs()),
                        calleeReg);
  masm.branch32(Assembler::AboveOrEqual, argcReg, calleeReg, &noUnderflow);
  {
    // Call the arguments rectifier.
    TrampolinePtr argumentsRectifier =
        cx_->runtime()->jitRuntime()->getArgumentsRectifier();
    masm.movePtr(argumentsRectifier, code);
  }
>>>>>>> upstream-releases

<<<<<<< HEAD
  // For the expression decompiler
  EmitRestoreTailCallReg(masm);
  masm.pushValue(lhs);
  masm.pushValue(rhs);
||||||| merged common ancestors
    // For the expression decompiler
    EmitRestoreTailCallReg(masm);
    masm.pushValue(lhs);
    masm.pushValue(rhs);
=======
  masm.bind(&noUnderflow);
  masm.callJit(code);
>>>>>>> upstream-releases

<<<<<<< HEAD
  masm.pushValue(rhs);
  masm.pushValue(lhs);
||||||| merged common ancestors
    masm.pushValue(rhs);
    masm.pushValue(lhs);
=======
  // If this is a constructing call, and the callee returns a non-object,
  // replace it with the |this| object passed in.
  if (isConstructing) {
    updateReturnValue();
  }
>>>>>>> upstream-releases

<<<<<<< HEAD
  if (!tailCallVM(masm, DoConcatStringObjectInfo)) {
    return false;
  }
||||||| merged common ancestors
    if (!tailCallVM(masm, DoConcatStringObjectInfo)) {
        return false;
    }
=======
  stubFrame.leave(masm, true);

  if (!isSameRealm) {
    masm.switchToBaselineFrameRealm(scratch2);
  }
>>>>>>> upstream-releases

  return true;
}
