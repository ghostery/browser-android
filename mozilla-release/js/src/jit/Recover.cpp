/* -*- Mode: C++; tab-width: 8; indent-tabs-mode: nil; c-basic-offset: 2 -*-
 * vim: set ts=8 sts=2 et sw=2 tw=80:
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/. */

#include "jit/Recover.h"

#include "jsapi.h"
#include "jsmath.h"

#include "builtin/RegExp.h"
#include "builtin/String.h"
#include "builtin/TypedObject.h"
#include "gc/Heap.h"
#include "jit/Ion.h"
#include "jit/JitSpewer.h"
#include "jit/JSJitFrameIter.h"
#include "jit/MIR.h"
#include "jit/MIRGraph.h"
#include "jit/VMFunctions.h"
#include "vm/Interpreter.h"
#include "vm/Iteration.h"
#include "vm/JSContext.h"
#include "vm/JSObject.h"
#include "vm/StringType.h"

#include "vm/Interpreter-inl.h"
#include "vm/NativeObject-inl.h"

using namespace js;
using namespace js::jit;

bool MNode::writeRecoverData(CompactBufferWriter& writer) const {
  MOZ_CRASH("This instruction is not serializable");
}

void RInstruction::readRecoverData(CompactBufferReader& reader,
                                   RInstructionStorage* raw) {
  uint32_t op = reader.readUnsigned();
  switch (Opcode(op)) {
#define MATCH_OPCODES_(op)                                                  \
  case Recover_##op:                                                        \
    static_assert(sizeof(R##op) <= sizeof(RInstructionStorage),             \
                  "storage space must be big enough to store R" #op);       \
    static_assert(alignof(R##op) <= alignof(RInstructionStorage),           \
                  "storage space must be aligned adequate to store R" #op); \
    new (raw->addr()) R##op(reader);                                        \
    break;

    RECOVER_OPCODE_LIST(MATCH_OPCODES_)
#undef MATCH_OPCODES_

    case Recover_Invalid:
    default:
      MOZ_CRASH("Bad decoding of the previous instruction?");
  }
}

bool MResumePoint::writeRecoverData(CompactBufferWriter& writer) const {
  writer.writeUnsigned(uint32_t(RInstruction::Recover_ResumePoint));

  MBasicBlock* bb = block();
  JSFunction* fun = bb->info().funMaybeLazy();
  JSScript* script = bb->info().script();
  uint32_t exprStack = stackDepth() - bb->info().ninvoke();

#ifdef DEBUG
<<<<<<< HEAD
  // Ensure that all snapshot which are encoded can safely be used for
  // bailouts.
  if (GetJitContext()->cx) {
    uint32_t stackDepth;
    bool reachablePC;
    jsbytecode* bailPC = pc();

    if (mode() == MResumePoint::ResumeAfter) {
      bailPC = GetNextPc(pc());
    }

    if (!ReconstructStackDepth(GetJitContext()->cx, script, bailPC, &stackDepth,
                               &reachablePC)) {
      return false;
    }

    if (reachablePC) {
      if (JSOp(*bailPC) == JSOP_FUNCALL) {
        // For fun.call(this, ...); the reconstructStackDepth will
        // include the this. When inlining that is not included.  So the
        // exprStackSlots will be one less.
        MOZ_ASSERT(stackDepth - exprStack <= 1);
      } else if (JSOp(*bailPC) != JSOP_FUNAPPLY && !IsGetPropPC(bailPC) &&
                 !IsSetPropPC(bailPC)) {
        // For fun.apply({}, arguments) the reconstructStackDepth will
        // have stackdepth 4, but it could be that we inlined the
        // funapply. In that case exprStackSlots, will have the real
        // arguments in the slots and not be 4.

        // With accessors, we have different stack depths depending on
        // whether or not we inlined the accessor, as the inlined stack
        // contains a callee function that should never have been there
        // and we might just be capturing an uneventful property site,
        // in which case there won't have been any violence.
        MOZ_ASSERT(exprStack == stackDepth);
      }
||||||| merged common ancestors
    // Ensure that all snapshot which are encoded can safely be used for
    // bailouts.
    if (GetJitContext()->cx) {
        uint32_t stackDepth;
        bool reachablePC;
        jsbytecode* bailPC = pc();

        if (mode() == MResumePoint::ResumeAfter) {
            bailPC = GetNextPc(pc());
        }

        if (!ReconstructStackDepth(GetJitContext()->cx, script,
                                   bailPC, &stackDepth, &reachablePC))
        {
            return false;
        }

        if (reachablePC) {
            if (JSOp(*bailPC) == JSOP_FUNCALL) {
                // For fun.call(this, ...); the reconstructStackDepth will
                // include the this. When inlining that is not included.  So the
                // exprStackSlots will be one less.
                MOZ_ASSERT(stackDepth - exprStack <= 1);
            } else if (JSOp(*bailPC) != JSOP_FUNAPPLY &&
                       !IsGetPropPC(bailPC) && !IsSetPropPC(bailPC))
            {
                // For fun.apply({}, arguments) the reconstructStackDepth will
                // have stackdepth 4, but it could be that we inlined the
                // funapply. In that case exprStackSlots, will have the real
                // arguments in the slots and not be 4.

                // With accessors, we have different stack depths depending on
                // whether or not we inlined the accessor, as the inlined stack
                // contains a callee function that should never have been there
                // and we might just be capturing an uneventful property site,
                // in which case there won't have been any violence.
                MOZ_ASSERT(exprStack == stackDepth);
            }
        }
=======
  // Ensure that all snapshot which are encoded can safely be used for
  // bailouts.
  if (GetJitContext()->cx) {
    uint32_t stackDepth;
    bool reachablePC;
    jsbytecode* bailPC = pc();

    if (mode() == MResumePoint::ResumeAfter) {
      bailPC = GetNextPc(pc());
>>>>>>> upstream-releases
    }
<<<<<<< HEAD
  }
||||||| merged common ancestors
=======

    if (!ReconstructStackDepth(GetJitContext()->cx, script, bailPC, &stackDepth,
                               &reachablePC)) {
      return false;
    }

    if (reachablePC) {
      if (JSOp(*bailPC) == JSOP_FUNCALL) {
        // For fun.call(this, ...); the reconstructStackDepth will
        // include the this. When inlining that is not included.  So the
        // exprStackSlots will be one less.
        MOZ_ASSERT(stackDepth - exprStack <= 1);
      } else if (JSOp(*bailPC) != JSOP_FUNAPPLY &&
                 !IsIonInlinableGetterOrSetterPC(bailPC)) {
        // For fun.apply({}, arguments) the reconstructStackDepth will
        // have stackdepth 4, but it could be that we inlined the
        // funapply. In that case exprStackSlots, will have the real
        // arguments in the slots and not be 4.

        // With accessors, we have different stack depths depending on
        // whether or not we inlined the accessor, as the inlined stack
        // contains a callee function that should never have been there
        // and we might just be capturing an uneventful property site,
        // in which case there won't have been any violence.
        MOZ_ASSERT(exprStack == stackDepth);
      }
    }
  }
>>>>>>> upstream-releases
#endif

  // Test if we honor the maximum of arguments at all times.  This is a sanity
  // check and not an algorithm limit. So check might be a bit too loose.  +4
  // to account for scope chain, return value, this value and maybe
  // arguments_object.
  MOZ_ASSERT(CountArgSlots(script, fun) < SNAPSHOT_MAX_NARGS + 4);

#ifdef JS_JITSPEW
  uint32_t implicit = StartArgSlot(script);
#endif
  uint32_t formalArgs = CountArgSlots(script, fun);
  uint32_t nallocs = formalArgs + script->nfixed() + exprStack;

  JitSpew(JitSpew_IonSnapshots,
          "Starting frame; implicit %u, formals %u, fixed %zu, exprs %u",
          implicit, formalArgs - implicit, script->nfixed(), exprStack);

  uint32_t pcoff = script->pcToOffset(pc());
  JitSpew(JitSpew_IonSnapshots, "Writing pc offset %u, nslots %u", pcoff,
          nallocs);
  writer.writeUnsigned(pcoff);
  writer.writeUnsigned(nallocs);
  return true;
}

RResumePoint::RResumePoint(CompactBufferReader& reader) {
  pcOffset_ = reader.readUnsigned();
  numOperands_ = reader.readUnsigned();
  JitSpew(JitSpew_IonSnapshots, "Read RResumePoint (pc offset %u, nslots %u)",
          pcOffset_, numOperands_);
}

bool RResumePoint::recover(JSContext* cx, SnapshotIterator& iter) const {
  MOZ_CRASH("This instruction is not recoverable.");
}

bool MBitNot::writeRecoverData(CompactBufferWriter& writer) const {
  MOZ_ASSERT(canRecoverOnBailout());
  writer.writeUnsigned(uint32_t(RInstruction::Recover_BitNot));
  return true;
}

RBitNot::RBitNot(CompactBufferReader& reader) {}

bool RBitNot::recover(JSContext* cx, SnapshotIterator& iter) const {
  RootedValue operand(cx, iter.read());
  RootedValue result(cx);

  if (!js::BitNot(cx, &operand, &result)) {
    return false;
  }

  iter.storeInstructionResult(result);
  return true;
}

bool MBitAnd::writeRecoverData(CompactBufferWriter& writer) const {
  MOZ_ASSERT(canRecoverOnBailout());
  writer.writeUnsigned(uint32_t(RInstruction::Recover_BitAnd));
  return true;
}

RBitAnd::RBitAnd(CompactBufferReader& reader) {}

bool RBitAnd::recover(JSContext* cx, SnapshotIterator& iter) const {
  RootedValue lhs(cx, iter.read());
  RootedValue rhs(cx, iter.read());
  RootedValue result(cx);
  MOZ_ASSERT(!lhs.isObject() && !rhs.isObject());

  if (!js::BitAnd(cx, &lhs, &rhs, &result)) {
    return false;
  }

  iter.storeInstructionResult(result);
  return true;
}

bool MBitOr::writeRecoverData(CompactBufferWriter& writer) const {
  MOZ_ASSERT(canRecoverOnBailout());
  writer.writeUnsigned(uint32_t(RInstruction::Recover_BitOr));
  return true;
}

RBitOr::RBitOr(CompactBufferReader& reader) {}

bool RBitOr::recover(JSContext* cx, SnapshotIterator& iter) const {
  RootedValue lhs(cx, iter.read());
  RootedValue rhs(cx, iter.read());
  RootedValue result(cx);
  MOZ_ASSERT(!lhs.isObject() && !rhs.isObject());

  if (!js::BitOr(cx, &lhs, &rhs, &result)) {
    return false;
  }

  iter.storeInstructionResult(result);
  return true;
}

bool MBitXor::writeRecoverData(CompactBufferWriter& writer) const {
  MOZ_ASSERT(canRecoverOnBailout());
  writer.writeUnsigned(uint32_t(RInstruction::Recover_BitXor));
  return true;
}

RBitXor::RBitXor(CompactBufferReader& reader) {}

bool RBitXor::recover(JSContext* cx, SnapshotIterator& iter) const {
  RootedValue lhs(cx, iter.read());
  RootedValue rhs(cx, iter.read());
  RootedValue result(cx);

  if (!js::BitXor(cx, &lhs, &rhs, &result)) {
    return false;
  }

  iter.storeInstructionResult(result);
  return true;
}

bool MLsh::writeRecoverData(CompactBufferWriter& writer) const {
  MOZ_ASSERT(canRecoverOnBailout());
  writer.writeUnsigned(uint32_t(RInstruction::Recover_Lsh));
  return true;
}

RLsh::RLsh(CompactBufferReader& reader) {}

bool RLsh::recover(JSContext* cx, SnapshotIterator& iter) const {
  RootedValue lhs(cx, iter.read());
  RootedValue rhs(cx, iter.read());
  RootedValue result(cx);
  MOZ_ASSERT(!lhs.isObject() && !rhs.isObject());

  if (!js::BitLsh(cx, &lhs, &rhs, &result)) {
    return false;
  }

  iter.storeInstructionResult(result);
  return true;
}

bool MRsh::writeRecoverData(CompactBufferWriter& writer) const {
  MOZ_ASSERT(canRecoverOnBailout());
  writer.writeUnsigned(uint32_t(RInstruction::Recover_Rsh));
  return true;
}

RRsh::RRsh(CompactBufferReader& reader) {}

bool RRsh::recover(JSContext* cx, SnapshotIterator& iter) const {
  RootedValue lhs(cx, iter.read());
  RootedValue rhs(cx, iter.read());
  RootedValue result(cx);
  MOZ_ASSERT(!lhs.isObject() && !rhs.isObject());

<<<<<<< HEAD
  if (!js::BitRsh(cx, &lhs, &rhs, &result)) {
    return false;
  }

  iter.storeInstructionResult(result);
  return true;
}

bool MUrsh::writeRecoverData(CompactBufferWriter& writer) const {
  MOZ_ASSERT(canRecoverOnBailout());
  writer.writeUnsigned(uint32_t(RInstruction::Recover_Ursh));
  return true;
}

RUrsh::RUrsh(CompactBufferReader& reader) {}

bool RUrsh::recover(JSContext* cx, SnapshotIterator& iter) const {
  RootedValue lhs(cx, iter.read());
  RootedValue rhs(cx, iter.read());
  MOZ_ASSERT(!lhs.isObject() && !rhs.isObject());

  RootedValue result(cx);
  if (!js::UrshOperation(cx, &lhs, &rhs, &result)) {
    return false;
  }
||||||| merged common ancestors
    if (!js::BitRsh(cx, &lhs, &rhs, &result)) {
        return false;
    }
=======
  if (!js::BitRsh(cx, &lhs, &rhs, &result)) {
    return false;
  }
>>>>>>> upstream-releases

  iter.storeInstructionResult(result);
  return true;
}

<<<<<<< HEAD
bool MSignExtendInt32::writeRecoverData(CompactBufferWriter& writer) const {
  MOZ_ASSERT(canRecoverOnBailout());
  writer.writeUnsigned(uint32_t(RInstruction::Recover_SignExtendInt32));
  MOZ_ASSERT(Mode(uint8_t(mode_)) == mode_);
  writer.writeByte(uint8_t(mode_));
  return true;
||||||| merged common ancestors
bool
MUrsh::writeRecoverData(CompactBufferWriter& writer) const
{
    MOZ_ASSERT(canRecoverOnBailout());
    writer.writeUnsigned(uint32_t(RInstruction::Recover_Ursh));
    return true;
=======
bool MUrsh::writeRecoverData(CompactBufferWriter& writer) const {
  MOZ_ASSERT(canRecoverOnBailout());
  writer.writeUnsigned(uint32_t(RInstruction::Recover_Ursh));
  return true;
>>>>>>> upstream-releases
}

<<<<<<< HEAD
RSignExtendInt32::RSignExtendInt32(CompactBufferReader& reader) {
  mode_ = reader.readByte();
}
||||||| merged common ancestors
RUrsh::RUrsh(CompactBufferReader& reader)
{ }
=======
RUrsh::RUrsh(CompactBufferReader& reader) {}
>>>>>>> upstream-releases

<<<<<<< HEAD
bool RSignExtendInt32::recover(JSContext* cx, SnapshotIterator& iter) const {
  RootedValue operand(cx, iter.read());
||||||| merged common ancestors
bool
RUrsh::recover(JSContext* cx, SnapshotIterator& iter) const
{
    RootedValue lhs(cx, iter.read());
    RootedValue rhs(cx, iter.read());
    MOZ_ASSERT(!lhs.isObject() && !rhs.isObject());
=======
bool RUrsh::recover(JSContext* cx, SnapshotIterator& iter) const {
  RootedValue lhs(cx, iter.read());
  RootedValue rhs(cx, iter.read());
  MOZ_ASSERT(!lhs.isObject() && !rhs.isObject());
>>>>>>> upstream-releases

<<<<<<< HEAD
  int32_t result;
  switch (MSignExtendInt32::Mode(mode_)) {
    case MSignExtendInt32::Byte:
      if (!js::SignExtendOperation<int8_t>(cx, operand, &result)) {
||||||| merged common ancestors
    RootedValue result(cx);
    if (!js::UrshOperation(cx, &lhs, &rhs, &result)) {
=======
  RootedValue result(cx);
  if (!js::UrshOperation(cx, &lhs, &rhs, &result)) {
    return false;
  }

  iter.storeInstructionResult(result);
  return true;
}

bool MSignExtendInt32::writeRecoverData(CompactBufferWriter& writer) const {
  MOZ_ASSERT(canRecoverOnBailout());
  writer.writeUnsigned(uint32_t(RInstruction::Recover_SignExtendInt32));
  MOZ_ASSERT(Mode(uint8_t(mode_)) == mode_);
  writer.writeByte(uint8_t(mode_));
  return true;
}

RSignExtendInt32::RSignExtendInt32(CompactBufferReader& reader) {
  mode_ = reader.readByte();
}

bool RSignExtendInt32::recover(JSContext* cx, SnapshotIterator& iter) const {
  RootedValue operand(cx, iter.read());

  int32_t result;
  switch (MSignExtendInt32::Mode(mode_)) {
    case MSignExtendInt32::Byte:
      if (!js::SignExtendOperation<int8_t>(cx, operand, &result)) {
>>>>>>> upstream-releases
        return false;
      }
      break;
    case MSignExtendInt32::Half:
      if (!js::SignExtendOperation<int16_t>(cx, operand, &result)) {
        return false;
      }
      break;
  }

  RootedValue rootedResult(cx, js::Int32Value(result));
  iter.storeInstructionResult(rootedResult);
  return true;
}

bool MAdd::writeRecoverData(CompactBufferWriter& writer) const {
  MOZ_ASSERT(canRecoverOnBailout());
  writer.writeUnsigned(uint32_t(RInstruction::Recover_Add));
  writer.writeByte(specialization_ == MIRType::Float32);
  return true;
}

RAdd::RAdd(CompactBufferReader& reader) {
  isFloatOperation_ = reader.readByte();
}

bool RAdd::recover(JSContext* cx, SnapshotIterator& iter) const {
  RootedValue lhs(cx, iter.read());
  RootedValue rhs(cx, iter.read());
  RootedValue result(cx);

  MOZ_ASSERT(!lhs.isObject() && !rhs.isObject());
  if (!js::AddValues(cx, &lhs, &rhs, &result)) {
    return false;
  }

  // MIRType::Float32 is a specialization embedding the fact that the result is
  // rounded to a Float32.
  if (isFloatOperation_ && !RoundFloat32(cx, result, &result)) {
    return false;
  }

  iter.storeInstructionResult(result);
  return true;
}

bool MSub::writeRecoverData(CompactBufferWriter& writer) const {
  MOZ_ASSERT(canRecoverOnBailout());
  writer.writeUnsigned(uint32_t(RInstruction::Recover_Sub));
  writer.writeByte(specialization_ == MIRType::Float32);
  return true;
}

RSub::RSub(CompactBufferReader& reader) {
  isFloatOperation_ = reader.readByte();
}

<<<<<<< HEAD
bool RSub::recover(JSContext* cx, SnapshotIterator& iter) const {
  RootedValue lhs(cx, iter.read());
  RootedValue rhs(cx, iter.read());
  RootedValue result(cx);
||||||| merged common ancestors
    // MIRType::Float32 is a specialization embedding the fact that the result is
    // rounded to a Float32.
    if (isFloatOperation_ && !RoundFloat32(cx, result, &result)) {
        return false;
    }
=======
bool RSub::recover(JSContext* cx, SnapshotIterator& iter) const {
  RootedValue lhs(cx, iter.read());
  RootedValue rhs(cx, iter.read());
  RootedValue result(cx);

  MOZ_ASSERT(!lhs.isObject() && !rhs.isObject());
  if (!js::SubValues(cx, &lhs, &rhs, &result)) {
    return false;
  }
>>>>>>> upstream-releases

<<<<<<< HEAD
  MOZ_ASSERT(!lhs.isObject() && !rhs.isObject());
  if (!js::SubValues(cx, &lhs, &rhs, &result)) {
    return false;
  }

  // MIRType::Float32 is a specialization embedding the fact that the result is
  // rounded to a Float32.
  if (isFloatOperation_ && !RoundFloat32(cx, result, &result)) {
    return false;
  }

  iter.storeInstructionResult(result);
  return true;
||||||| merged common ancestors
    iter.storeInstructionResult(result);
    return true;
=======
  // MIRType::Float32 is a specialization embedding the fact that the result is
  // rounded to a Float32.
  if (isFloatOperation_ && !RoundFloat32(cx, result, &result)) {
    return false;
  }

  iter.storeInstructionResult(result);
  return true;
>>>>>>> upstream-releases
}

bool MMul::writeRecoverData(CompactBufferWriter& writer) const {
  MOZ_ASSERT(canRecoverOnBailout());
  writer.writeUnsigned(uint32_t(RInstruction::Recover_Mul));
  writer.writeByte(specialization_ == MIRType::Float32);
  MOZ_ASSERT(Mode(uint8_t(mode_)) == mode_);
  writer.writeByte(uint8_t(mode_));
  return true;
}

RMul::RMul(CompactBufferReader& reader) {
  isFloatOperation_ = reader.readByte();
  mode_ = reader.readByte();
}

bool RMul::recover(JSContext* cx, SnapshotIterator& iter) const {
  RootedValue lhs(cx, iter.read());
  RootedValue rhs(cx, iter.read());
  RootedValue result(cx);

  if (MMul::Mode(mode_) == MMul::Normal) {
    if (!js::MulValues(cx, &lhs, &rhs, &result)) {
      return false;
    }

    // MIRType::Float32 is a specialization embedding the fact that the
    // result is rounded to a Float32.
    if (isFloatOperation_ && !RoundFloat32(cx, result, &result)) {
      return false;
    }
  } else {
    MOZ_ASSERT(MMul::Mode(mode_) == MMul::Integer);
    if (!js::math_imul_handle(cx, lhs, rhs, &result)) {
      return false;
    }
  }

  iter.storeInstructionResult(result);
  return true;
}

bool MDiv::writeRecoverData(CompactBufferWriter& writer) const {
  MOZ_ASSERT(canRecoverOnBailout());
  writer.writeUnsigned(uint32_t(RInstruction::Recover_Div));
  writer.writeByte(specialization_ == MIRType::Float32);
  return true;
}

RDiv::RDiv(CompactBufferReader& reader) {
  isFloatOperation_ = reader.readByte();
}

bool RDiv::recover(JSContext* cx, SnapshotIterator& iter) const {
  RootedValue lhs(cx, iter.read());
  RootedValue rhs(cx, iter.read());
  RootedValue result(cx);

  if (!js::DivValues(cx, &lhs, &rhs, &result)) {
    return false;
  }

  // MIRType::Float32 is a specialization embedding the fact that the result is
  // rounded to a Float32.
  if (isFloatOperation_ && !RoundFloat32(cx, result, &result)) {
    return false;
  }

  iter.storeInstructionResult(result);
  return true;
}

bool MMod::writeRecoverData(CompactBufferWriter& writer) const {
  MOZ_ASSERT(canRecoverOnBailout());
  writer.writeUnsigned(uint32_t(RInstruction::Recover_Mod));
  return true;
}

RMod::RMod(CompactBufferReader& reader) {}

bool RMod::recover(JSContext* cx, SnapshotIterator& iter) const {
  RootedValue lhs(cx, iter.read());
  RootedValue rhs(cx, iter.read());
  RootedValue result(cx);

  MOZ_ASSERT(!lhs.isObject() && !rhs.isObject());
  if (!js::ModValues(cx, &lhs, &rhs, &result)) {
    return false;
  }

  iter.storeInstructionResult(result);
  return true;
}

bool MNot::writeRecoverData(CompactBufferWriter& writer) const {
  MOZ_ASSERT(canRecoverOnBailout());
  writer.writeUnsigned(uint32_t(RInstruction::Recover_Not));
  return true;
}

RNot::RNot(CompactBufferReader& reader) {}

bool RNot::recover(JSContext* cx, SnapshotIterator& iter) const {
  RootedValue v(cx, iter.read());
  RootedValue result(cx);

  result.setBoolean(!ToBoolean(v));

  iter.storeInstructionResult(result);
  return true;
}

bool MConcat::writeRecoverData(CompactBufferWriter& writer) const {
  MOZ_ASSERT(canRecoverOnBailout());
  writer.writeUnsigned(uint32_t(RInstruction::Recover_Concat));
  return true;
}

RConcat::RConcat(CompactBufferReader& reader) {}

bool RConcat::recover(JSContext* cx, SnapshotIterator& iter) const {
  RootedValue lhs(cx, iter.read());
  RootedValue rhs(cx, iter.read());
  RootedValue result(cx);

  MOZ_ASSERT(!lhs.isObject() && !rhs.isObject());
  if (!js::AddValues(cx, &lhs, &rhs, &result)) {
    return false;
  }

  iter.storeInstructionResult(result);
  return true;
}

RStringLength::RStringLength(CompactBufferReader& reader) {}

bool RStringLength::recover(JSContext* cx, SnapshotIterator& iter) const {
  RootedValue operand(cx, iter.read());
  RootedValue result(cx);

  MOZ_ASSERT(!operand.isObject());
  if (!js::GetLengthProperty(operand, &result)) {
    return false;
  }

  iter.storeInstructionResult(result);
  return true;
}

bool MStringLength::writeRecoverData(CompactBufferWriter& writer) const {
  MOZ_ASSERT(canRecoverOnBailout());
  writer.writeUnsigned(uint32_t(RInstruction::Recover_StringLength));
  return true;
}

bool MArgumentsLength::writeRecoverData(CompactBufferWriter& writer) const {
  MOZ_ASSERT(canRecoverOnBailout());
  writer.writeUnsigned(uint32_t(RInstruction::Recover_ArgumentsLength));
  return true;
}

RArgumentsLength::RArgumentsLength(CompactBufferReader& reader) {}

bool RArgumentsLength::recover(JSContext* cx, SnapshotIterator& iter) const {
  RootedValue result(cx);

  result.setInt32(iter.readOuterNumActualArgs());

  iter.storeInstructionResult(result);
  return true;
}

bool MFloor::writeRecoverData(CompactBufferWriter& writer) const {
  MOZ_ASSERT(canRecoverOnBailout());
  writer.writeUnsigned(uint32_t(RInstruction::Recover_Floor));
  return true;
}

RFloor::RFloor(CompactBufferReader& reader) {}

bool RFloor::recover(JSContext* cx, SnapshotIterator& iter) const {
  RootedValue v(cx, iter.read());
  RootedValue result(cx);

  if (!js::math_floor_handle(cx, v, &result)) {
    return false;
  }

  iter.storeInstructionResult(result);
  return true;
}

bool MCeil::writeRecoverData(CompactBufferWriter& writer) const {
  MOZ_ASSERT(canRecoverOnBailout());
  writer.writeUnsigned(uint32_t(RInstruction::Recover_Ceil));
  return true;
}

RCeil::RCeil(CompactBufferReader& reader) {}

bool RCeil::recover(JSContext* cx, SnapshotIterator& iter) const {
  RootedValue v(cx, iter.read());
  RootedValue result(cx);

<<<<<<< HEAD
  if (!js::math_ceil_handle(cx, v, &result)) {
    return false;
  }

  iter.storeInstructionResult(result);
  return true;
||||||| merged common ancestors
bool
RCeil::recover(JSContext* cx, SnapshotIterator& iter) const
{
    RootedValue v(cx, iter.read());
    RootedValue result(cx);

    if (!js::math_ceil_handle(cx, v, &result)) {
        return false;
    }

    iter.storeInstructionResult(result);
    return true;
=======
  if (!js::math_ceil_handle(cx, v, &result)) {
    return false;
  }

  iter.storeInstructionResult(result);
  return true;
>>>>>>> upstream-releases
}

bool MRound::writeRecoverData(CompactBufferWriter& writer) const {
  MOZ_ASSERT(canRecoverOnBailout());
  writer.writeUnsigned(uint32_t(RInstruction::Recover_Round));
  return true;
}

RRound::RRound(CompactBufferReader& reader) {}

bool RRound::recover(JSContext* cx, SnapshotIterator& iter) const {
  RootedValue arg(cx, iter.read());
  RootedValue result(cx);

  MOZ_ASSERT(!arg.isObject());
  if (!js::math_round_handle(cx, arg, &result)) return false;

  iter.storeInstructionResult(result);
  return true;
}

bool MTrunc::writeRecoverData(CompactBufferWriter& writer) const {
  MOZ_ASSERT(canRecoverOnBailout());
  writer.writeUnsigned(uint32_t(RInstruction::Recover_Trunc));
  return true;
}

RTrunc::RTrunc(CompactBufferReader& reader) {}

bool RTrunc::recover(JSContext* cx, SnapshotIterator& iter) const {
  RootedValue arg(cx, iter.read());
  RootedValue result(cx);

  MOZ_ASSERT(!arg.isObject());
  if (!js::math_trunc_handle(cx, arg, &result)) return false;

  iter.storeInstructionResult(result);
  return true;
}

bool MCharCodeAt::writeRecoverData(CompactBufferWriter& writer) const {
  MOZ_ASSERT(canRecoverOnBailout());
  writer.writeUnsigned(uint32_t(RInstruction::Recover_CharCodeAt));
  return true;
}

RCharCodeAt::RCharCodeAt(CompactBufferReader& reader) {}

bool RCharCodeAt::recover(JSContext* cx, SnapshotIterator& iter) const {
  RootedString lhs(cx, iter.read().toString());
  RootedValue rhs(cx, iter.read());
  RootedValue result(cx);

  if (!js::str_charCodeAt_impl(cx, lhs, rhs, &result)) {
    return false;
  }

  iter.storeInstructionResult(result);
  return true;
}

bool MFromCharCode::writeRecoverData(CompactBufferWriter& writer) const {
  MOZ_ASSERT(canRecoverOnBailout());
  writer.writeUnsigned(uint32_t(RInstruction::Recover_FromCharCode));
  return true;
}

RFromCharCode::RFromCharCode(CompactBufferReader& reader) {}

bool RFromCharCode::recover(JSContext* cx, SnapshotIterator& iter) const {
  RootedValue operand(cx, iter.read());
  RootedValue result(cx);

  MOZ_ASSERT(!operand.isObject());
  if (!js::str_fromCharCode_one_arg(cx, operand, &result)) {
    return false;
  }

  iter.storeInstructionResult(result);
  return true;
}

bool MPow::writeRecoverData(CompactBufferWriter& writer) const {
  MOZ_ASSERT(canRecoverOnBailout());
  writer.writeUnsigned(uint32_t(RInstruction::Recover_Pow));
  return true;
}

RPow::RPow(CompactBufferReader& reader) {}

bool RPow::recover(JSContext* cx, SnapshotIterator& iter) const {
  RootedValue base(cx, iter.read());
  RootedValue power(cx, iter.read());
  RootedValue result(cx);

  MOZ_ASSERT(base.isNumber() && power.isNumber());
  if (!js::PowValues(cx, &base, &power, &result)) {
    return false;
  }

  iter.storeInstructionResult(result);
  return true;
}

bool MPowHalf::writeRecoverData(CompactBufferWriter& writer) const {
  MOZ_ASSERT(canRecoverOnBailout());
  writer.writeUnsigned(uint32_t(RInstruction::Recover_PowHalf));
  return true;
}

RPowHalf::RPowHalf(CompactBufferReader& reader) {}

bool RPowHalf::recover(JSContext* cx, SnapshotIterator& iter) const {
  RootedValue base(cx, iter.read());
  RootedValue power(cx);
  RootedValue result(cx);
  power.setNumber(0.5);

  MOZ_ASSERT(base.isNumber());
  if (!js::PowValues(cx, &base, &power, &result)) {
    return false;
  }

  iter.storeInstructionResult(result);
  return true;
}

bool MMinMax::writeRecoverData(CompactBufferWriter& writer) const {
  MOZ_ASSERT(canRecoverOnBailout());
  writer.writeUnsigned(uint32_t(RInstruction::Recover_MinMax));
  writer.writeByte(isMax_);
  return true;
}

RMinMax::RMinMax(CompactBufferReader& reader) { isMax_ = reader.readByte(); }

bool RMinMax::recover(JSContext* cx, SnapshotIterator& iter) const {
  RootedValue a(cx, iter.read());
  RootedValue b(cx, iter.read());
  RootedValue result(cx);

  if (!js::minmax_impl(cx, isMax_, a, b, &result)) {
    return false;
  }

  iter.storeInstructionResult(result);
  return true;
}

bool MAbs::writeRecoverData(CompactBufferWriter& writer) const {
  MOZ_ASSERT(canRecoverOnBailout());
  writer.writeUnsigned(uint32_t(RInstruction::Recover_Abs));
  return true;
}

RAbs::RAbs(CompactBufferReader& reader) {}

bool RAbs::recover(JSContext* cx, SnapshotIterator& iter) const {
  RootedValue v(cx, iter.read());
  RootedValue result(cx);

  if (!js::math_abs_handle(cx, v, &result)) {
    return false;
  }

  iter.storeInstructionResult(result);
  return true;
}

bool MSqrt::writeRecoverData(CompactBufferWriter& writer) const {
  MOZ_ASSERT(canRecoverOnBailout());
  writer.writeUnsigned(uint32_t(RInstruction::Recover_Sqrt));
  writer.writeByte(type() == MIRType::Float32);
  return true;
}

RSqrt::RSqrt(CompactBufferReader& reader) {
  isFloatOperation_ = reader.readByte();
}

bool RSqrt::recover(JSContext* cx, SnapshotIterator& iter) const {
  RootedValue num(cx, iter.read());
  RootedValue result(cx);

  MOZ_ASSERT(num.isNumber());
  if (!math_sqrt_handle(cx, num, &result)) {
    return false;
  }

  // MIRType::Float32 is a specialization embedding the fact that the result is
  // rounded to a Float32.
  if (isFloatOperation_ && !RoundFloat32(cx, result, &result)) {
    return false;
  }

  iter.storeInstructionResult(result);
  return true;
}

bool MAtan2::writeRecoverData(CompactBufferWriter& writer) const {
  MOZ_ASSERT(canRecoverOnBailout());
  writer.writeUnsigned(uint32_t(RInstruction::Recover_Atan2));
  return true;
}

RAtan2::RAtan2(CompactBufferReader& reader) {}

bool RAtan2::recover(JSContext* cx, SnapshotIterator& iter) const {
  RootedValue y(cx, iter.read());
  RootedValue x(cx, iter.read());
  RootedValue result(cx);

  if (!math_atan2_handle(cx, y, x, &result)) return false;

  iter.storeInstructionResult(result);
  return true;
}

bool MHypot::writeRecoverData(CompactBufferWriter& writer) const {
  MOZ_ASSERT(canRecoverOnBailout());
  writer.writeUnsigned(uint32_t(RInstruction::Recover_Hypot));
  writer.writeUnsigned(uint32_t(numOperands()));
  return true;
}

RHypot::RHypot(CompactBufferReader& reader)
    : numOperands_(reader.readUnsigned()) {}

<<<<<<< HEAD
bool RHypot::recover(JSContext* cx, SnapshotIterator& iter) const {
  JS::AutoValueVector vec(cx);
||||||| merged common ancestors
bool
RHypot::recover(JSContext* cx, SnapshotIterator& iter) const
{
    JS::AutoValueVector vec(cx);
=======
bool RHypot::recover(JSContext* cx, SnapshotIterator& iter) const {
  JS::RootedValueVector vec(cx);
>>>>>>> upstream-releases

  if (!vec.reserve(numOperands_)) {
    return false;
  }

  for (uint32_t i = 0; i < numOperands_; ++i) {
    vec.infallibleAppend(iter.read());
  }

  RootedValue result(cx);

  if (!js::math_hypot_handle(cx, vec, &result)) return false;

<<<<<<< HEAD
  iter.storeInstructionResult(result);
  return true;
}

bool MNearbyInt::writeRecoverData(CompactBufferWriter& writer) const {
  MOZ_ASSERT(canRecoverOnBailout());
  switch (roundingMode_) {
    case RoundingMode::Up:
      writer.writeUnsigned(uint32_t(RInstruction::Recover_Ceil));
      return true;
    case RoundingMode::Down:
      writer.writeUnsigned(uint32_t(RInstruction::Recover_Floor));
      return true;
    case RoundingMode::TowardsZero:
      writer.writeUnsigned(uint32_t(RInstruction::Recover_Trunc));
      return true;
    default:
      MOZ_CRASH("Unsupported rounding mode.");
  }
||||||| merged common ancestors
    iter.storeInstructionResult(result);
    return true;
}

bool
MNearbyInt::writeRecoverData(CompactBufferWriter& writer) const
{
    MOZ_ASSERT(canRecoverOnBailout());
    switch (roundingMode_) {
      case RoundingMode::Up:
        writer.writeUnsigned(uint32_t(RInstruction::Recover_Ceil));
        return true;
      case RoundingMode::Down:
        writer.writeUnsigned(uint32_t(RInstruction::Recover_Floor));
        return true;
      case RoundingMode::TowardsZero:
        writer.writeUnsigned(uint32_t(RInstruction::Recover_Trunc));
        return true;
      default:
        MOZ_CRASH("Unsupported rounding mode.");
    }
=======
  iter.storeInstructionResult(result);
  return true;
>>>>>>> upstream-releases
}

<<<<<<< HEAD
RNearbyInt::RNearbyInt(CompactBufferReader& reader) {
  roundingMode_ = reader.readByte();
||||||| merged common ancestors
RNearbyInt::RNearbyInt(CompactBufferReader& reader)
{
    roundingMode_ = reader.readByte();
=======
bool MNearbyInt::writeRecoverData(CompactBufferWriter& writer) const {
  MOZ_ASSERT(canRecoverOnBailout());
  switch (roundingMode_) {
    case RoundingMode::Up:
      writer.writeUnsigned(uint32_t(RInstruction::Recover_Ceil));
      return true;
    case RoundingMode::Down:
      writer.writeUnsigned(uint32_t(RInstruction::Recover_Floor));
      return true;
    case RoundingMode::TowardsZero:
      writer.writeUnsigned(uint32_t(RInstruction::Recover_Trunc));
      return true;
    default:
      MOZ_CRASH("Unsupported rounding mode.");
  }
>>>>>>> upstream-releases
}

<<<<<<< HEAD
bool RNearbyInt::recover(JSContext* cx, SnapshotIterator& iter) const {
  MOZ_CRASH("Unsupported rounding mode.");
||||||| merged common ancestors
bool
RNearbyInt::recover(JSContext* cx, SnapshotIterator& iter) const
{
    MOZ_CRASH("Unsupported rounding mode.");
=======
RNearbyInt::RNearbyInt(CompactBufferReader& reader) {
  roundingMode_ = reader.readByte();
>>>>>>> upstream-releases
}

<<<<<<< HEAD
bool MSign::writeRecoverData(CompactBufferWriter& writer) const {
  MOZ_ASSERT(canRecoverOnBailout());
  writer.writeUnsigned(uint32_t(RInstruction::Recover_Sign));
  return true;
||||||| merged common ancestors
bool
MSign::writeRecoverData(CompactBufferWriter& writer) const
{
    MOZ_ASSERT(canRecoverOnBailout());
    writer.writeUnsigned(uint32_t(RInstruction::Recover_Sign));
    return true;
=======
bool RNearbyInt::recover(JSContext* cx, SnapshotIterator& iter) const {
  MOZ_CRASH("Unsupported rounding mode.");
>>>>>>> upstream-releases
}

<<<<<<< HEAD
RSign::RSign(CompactBufferReader& reader) {}
||||||| merged common ancestors
RSign::RSign(CompactBufferReader& reader)
{ }
=======
bool MSign::writeRecoverData(CompactBufferWriter& writer) const {
  MOZ_ASSERT(canRecoverOnBailout());
  writer.writeUnsigned(uint32_t(RInstruction::Recover_Sign));
  return true;
}
>>>>>>> upstream-releases

<<<<<<< HEAD
bool RSign::recover(JSContext* cx, SnapshotIterator& iter) const {
  RootedValue arg(cx, iter.read());
  RootedValue result(cx);
||||||| merged common ancestors
bool
RSign::recover(JSContext* cx, SnapshotIterator& iter) const
{
    RootedValue arg(cx, iter.read());
    RootedValue result(cx);
=======
RSign::RSign(CompactBufferReader& reader) {}
>>>>>>> upstream-releases

<<<<<<< HEAD
  MOZ_ASSERT(!arg.isObject());
  if (!js::math_sign_handle(cx, arg, &result)) return false;
||||||| merged common ancestors
    MOZ_ASSERT(!arg.isObject());
    if(!js::math_sign_handle(cx, arg, &result))
        return false;
=======
bool RSign::recover(JSContext* cx, SnapshotIterator& iter) const {
  RootedValue arg(cx, iter.read());
  RootedValue result(cx);
>>>>>>> upstream-releases

<<<<<<< HEAD
  iter.storeInstructionResult(result);
  return true;
||||||| merged common ancestors
    iter.storeInstructionResult(result);
    return true;
}

bool
MMathFunction::writeRecoverData(CompactBufferWriter& writer) const
{
    MOZ_ASSERT(canRecoverOnBailout());
    switch (function_) {
      case Ceil:
        writer.writeUnsigned(uint32_t(RInstruction::Recover_Ceil));
        return true;
      case Floor:
        writer.writeUnsigned(uint32_t(RInstruction::Recover_Floor));
        return true;
      case Round:
        writer.writeUnsigned(uint32_t(RInstruction::Recover_Round));
        return true;
      case Trunc:
        writer.writeUnsigned(uint32_t(RInstruction::Recover_Trunc));
        return true;
      case Sin:
      case Log:
        writer.writeUnsigned(uint32_t(RInstruction::Recover_MathFunction));
        writer.writeByte(function_);
        return true;
      default:
        MOZ_CRASH("Unknown math function.");
    }
=======
  MOZ_ASSERT(!arg.isObject());
  if (!js::math_sign_handle(cx, arg, &result)) return false;

  iter.storeInstructionResult(result);
  return true;
>>>>>>> upstream-releases
}

bool MMathFunction::writeRecoverData(CompactBufferWriter& writer) const {
  MOZ_ASSERT(canRecoverOnBailout());
  switch (function_) {
    case Ceil:
      writer.writeUnsigned(uint32_t(RInstruction::Recover_Ceil));
      return true;
    case Floor:
      writer.writeUnsigned(uint32_t(RInstruction::Recover_Floor));
      return true;
    case Round:
      writer.writeUnsigned(uint32_t(RInstruction::Recover_Round));
      return true;
    case Trunc:
      writer.writeUnsigned(uint32_t(RInstruction::Recover_Trunc));
      return true;
    case Sin:
    case Log:
      writer.writeUnsigned(uint32_t(RInstruction::Recover_MathFunction));
      writer.writeByte(function_);
      return true;
    default:
      MOZ_CRASH("Unknown math function.");
  }
}

RMathFunction::RMathFunction(CompactBufferReader& reader) {
  function_ = reader.readByte();
}

bool RMathFunction::recover(JSContext* cx, SnapshotIterator& iter) const {
  switch (function_) {
    case MMathFunction::Sin: {
      RootedValue arg(cx, iter.read());
      RootedValue result(cx);

      if (!js::math_sin_handle(cx, arg, &result)) {
        return false;
      }

      iter.storeInstructionResult(result);
      return true;
    }
    case MMathFunction::Log: {
      RootedValue arg(cx, iter.read());
      RootedValue result(cx);

      if (!js::math_log_handle(cx, arg, &result)) {
        return false;
      }

      iter.storeInstructionResult(result);
      return true;
    }
    default:
      MOZ_CRASH("Unknown math function.");
  }
}

bool MRandom::writeRecoverData(CompactBufferWriter& writer) const {
  MOZ_ASSERT(this->canRecoverOnBailout());
  writer.writeUnsigned(uint32_t(RInstruction::Recover_Random));
  return true;
}

RRandom::RRandom(CompactBufferReader& reader) {}

bool RRandom::recover(JSContext* cx, SnapshotIterator& iter) const {
  iter.storeInstructionResult(DoubleValue(math_random_impl(cx)));
  return true;
}

bool MStringSplit::writeRecoverData(CompactBufferWriter& writer) const {
  MOZ_ASSERT(canRecoverOnBailout());
  writer.writeUnsigned(uint32_t(RInstruction::Recover_StringSplit));
  return true;
}

RStringSplit::RStringSplit(CompactBufferReader& reader) {}

bool RStringSplit::recover(JSContext* cx, SnapshotIterator& iter) const {
  RootedString str(cx, iter.read().toString());
  RootedString sep(cx, iter.read().toString());
  RootedObjectGroup group(cx, ObjectGroupRealm::getStringSplitStringGroup(cx));
  if (!group) {
    return false;
  }
  RootedValue result(cx);

<<<<<<< HEAD
  JSObject* res = str_split_string(cx, group, str, sep, INT32_MAX);
  if (!res) {
    return false;
  }
||||||| merged common ancestors
    JSObject* res = str_split_string(cx, group, str, sep, INT32_MAX);
    if (!res) {
        return false;
    }
=======
  JSObject* res = StringSplitString(cx, group, str, sep, INT32_MAX);
  if (!res) {
    return false;
  }
>>>>>>> upstream-releases

  result.setObject(*res);
  iter.storeInstructionResult(result);
  return true;
}

bool MNaNToZero::writeRecoverData(CompactBufferWriter& writer) const {
  MOZ_ASSERT(canRecoverOnBailout());
  writer.writeUnsigned(uint32_t(RInstruction::Recover_NaNToZero));
  return true;
}

<<<<<<< HEAD
RNaNToZero::RNaNToZero(CompactBufferReader& reader) {}

bool RNaNToZero::recover(JSContext* cx, SnapshotIterator& iter) const {
  RootedValue v(cx, iter.read());
  RootedValue result(cx);
  MOZ_ASSERT(v.isDouble() || v.isInt32());
||||||| merged common ancestors
RNaNToZero::RNaNToZero(CompactBufferReader& reader)
{ }

=======
RNaNToZero::RNaNToZero(CompactBufferReader& reader) {}
>>>>>>> upstream-releases

<<<<<<< HEAD
  // x ? x : 0.0
  if (ToBoolean(v)) {
    result = v;
  } else {
    result.setDouble(0.0);
  }
||||||| merged common ancestors
bool
RNaNToZero::recover(JSContext* cx, SnapshotIterator& iter) const
{
    RootedValue v(cx, iter.read());
    RootedValue result(cx);
    MOZ_ASSERT(v.isDouble() || v.isInt32());
=======
bool RNaNToZero::recover(JSContext* cx, SnapshotIterator& iter) const {
  RootedValue v(cx, iter.read());
  RootedValue result(cx);
  MOZ_ASSERT(v.isDouble() || v.isInt32());
>>>>>>> upstream-releases

<<<<<<< HEAD
  iter.storeInstructionResult(result);
  return true;
||||||| merged common ancestors
    // x ? x : 0.0
    if (ToBoolean(v)) {
        result = v;
    } else {
        result.setDouble(0.0);
    }

    iter.storeInstructionResult(result);
    return true;
=======
  // x ? x : 0.0
  if (ToBoolean(v)) {
    result = v;
  } else {
    result.setDouble(0.0);
  }

  iter.storeInstructionResult(result);
  return true;
>>>>>>> upstream-releases
}

bool MRegExpMatcher::writeRecoverData(CompactBufferWriter& writer) const {
  MOZ_ASSERT(canRecoverOnBailout());
  writer.writeUnsigned(uint32_t(RInstruction::Recover_RegExpMatcher));
  return true;
}

RRegExpMatcher::RRegExpMatcher(CompactBufferReader& reader) {}

bool RRegExpMatcher::recover(JSContext* cx, SnapshotIterator& iter) const {
  RootedObject regexp(cx, &iter.read().toObject());
  RootedString input(cx, iter.read().toString());
  int32_t lastIndex = iter.read().toInt32();

  RootedValue result(cx);
  if (!RegExpMatcherRaw(cx, regexp, input, lastIndex, nullptr, &result)) {
    return false;
  }

  iter.storeInstructionResult(result);
  return true;
}

bool MRegExpSearcher::writeRecoverData(CompactBufferWriter& writer) const {
  MOZ_ASSERT(canRecoverOnBailout());
  writer.writeUnsigned(uint32_t(RInstruction::Recover_RegExpSearcher));
  return true;
}

RRegExpSearcher::RRegExpSearcher(CompactBufferReader& reader) {}

bool RRegExpSearcher::recover(JSContext* cx, SnapshotIterator& iter) const {
  RootedObject regexp(cx, &iter.read().toObject());
  RootedString input(cx, iter.read().toString());
  int32_t lastIndex = iter.read().toInt32();

  int32_t result;
  if (!RegExpSearcherRaw(cx, regexp, input, lastIndex, nullptr, &result)) {
    return false;
  }

  RootedValue resultVal(cx);
  resultVal.setInt32(result);
  iter.storeInstructionResult(resultVal);
  return true;
}

bool MRegExpTester::writeRecoverData(CompactBufferWriter& writer) const {
  MOZ_ASSERT(canRecoverOnBailout());
  writer.writeUnsigned(uint32_t(RInstruction::Recover_RegExpTester));
  return true;
}

RRegExpTester::RRegExpTester(CompactBufferReader& reader) {}

bool RRegExpTester::recover(JSContext* cx, SnapshotIterator& iter) const {
  RootedString string(cx, iter.read().toString());
  RootedObject regexp(cx, &iter.read().toObject());
  int32_t lastIndex = iter.read().toInt32();
  int32_t endIndex;

  if (!js::RegExpTesterRaw(cx, regexp, string, lastIndex, &endIndex)) {
    return false;
  }

  RootedValue result(cx);
  result.setInt32(endIndex);
  iter.storeInstructionResult(result);
  return true;
}

bool MTypeOf::writeRecoverData(CompactBufferWriter& writer) const {
  MOZ_ASSERT(canRecoverOnBailout());
  writer.writeUnsigned(uint32_t(RInstruction::Recover_TypeOf));
  return true;
}

RTypeOf::RTypeOf(CompactBufferReader& reader) {}

bool RTypeOf::recover(JSContext* cx, SnapshotIterator& iter) const {
  RootedValue v(cx, iter.read());

  RootedValue result(cx, StringValue(TypeOfOperation(v, cx->runtime())));
  iter.storeInstructionResult(result);
  return true;
}

bool MToDouble::writeRecoverData(CompactBufferWriter& writer) const {
  MOZ_ASSERT(canRecoverOnBailout());
  writer.writeUnsigned(uint32_t(RInstruction::Recover_ToDouble));
  return true;
}

RToDouble::RToDouble(CompactBufferReader& reader) {}

bool RToDouble::recover(JSContext* cx, SnapshotIterator& iter) const {
  RootedValue v(cx, iter.read());
  RootedValue result(cx);

  MOZ_ASSERT(!v.isObject());
  MOZ_ASSERT(!v.isSymbol());

  double dbl;
  if (!ToNumber(cx, v, &dbl)) {
    return false;
  }

  result.setDouble(dbl);
  iter.storeInstructionResult(result);
  return true;
}

bool MToFloat32::writeRecoverData(CompactBufferWriter& writer) const {
  MOZ_ASSERT(canRecoverOnBailout());
  writer.writeUnsigned(uint32_t(RInstruction::Recover_ToFloat32));
  return true;
}

RToFloat32::RToFloat32(CompactBufferReader& reader) {}

bool RToFloat32::recover(JSContext* cx, SnapshotIterator& iter) const {
  RootedValue v(cx, iter.read());
  RootedValue result(cx);

  MOZ_ASSERT(!v.isObject());
  if (!RoundFloat32(cx, v, &result)) {
    return false;
  }

  iter.storeInstructionResult(result);
  return true;
}

bool MTruncateToInt32::writeRecoverData(CompactBufferWriter& writer) const {
  MOZ_ASSERT(canRecoverOnBailout());
  writer.writeUnsigned(uint32_t(RInstruction::Recover_TruncateToInt32));
  return true;
}

RTruncateToInt32::RTruncateToInt32(CompactBufferReader& reader) {}

bool RTruncateToInt32::recover(JSContext* cx, SnapshotIterator& iter) const {
  RootedValue value(cx, iter.read());
  RootedValue result(cx);

  int32_t trunc;
  if (!JS::ToInt32(cx, value, &trunc)) {
    return false;
  }

  result.setInt32(trunc);
  iter.storeInstructionResult(result);
  return true;
}

bool MNewObject::writeRecoverData(CompactBufferWriter& writer) const {
  MOZ_ASSERT(canRecoverOnBailout());
  writer.writeUnsigned(uint32_t(RInstruction::Recover_NewObject));
  MOZ_ASSERT(Mode(uint8_t(mode_)) == mode_);
  writer.writeByte(uint8_t(mode_));
  return true;
}

RNewObject::RNewObject(CompactBufferReader& reader) {
  mode_ = MNewObject::Mode(reader.readByte());
}

bool RNewObject::recover(JSContext* cx, SnapshotIterator& iter) const {
  RootedObject templateObject(cx, &iter.read().toObject());
  RootedValue result(cx);
  JSObject* resultObject = nullptr;

  // See CodeGenerator::visitNewObjectVMCall
  switch (mode_) {
    case MNewObject::ObjectLiteral:
      resultObject = NewObjectOperationWithTemplate(cx, templateObject);
      break;
    case MNewObject::ObjectCreate:
      resultObject =
          ObjectCreateWithTemplate(cx, templateObject.as<PlainObject>());
      break;
  }

  if (!resultObject) {
    return false;
  }

  result.setObject(*resultObject);
  iter.storeInstructionResult(result);
  return true;
}

bool MNewTypedArray::writeRecoverData(CompactBufferWriter& writer) const {
  MOZ_ASSERT(canRecoverOnBailout());
  writer.writeUnsigned(uint32_t(RInstruction::Recover_NewTypedArray));
  return true;
}

RNewTypedArray::RNewTypedArray(CompactBufferReader& reader) {}

<<<<<<< HEAD
bool RNewTypedArray::recover(JSContext* cx, SnapshotIterator& iter) const {
  RootedObject templateObject(cx, &iter.read().toObject());
  RootedValue result(cx);

  uint32_t length = templateObject.as<TypedArrayObject>()->length();
  JSObject* resultObject =
      TypedArrayCreateWithTemplate(cx, templateObject, length);
  if (!resultObject) {
    return false;
  }
||||||| merged common ancestors
    uint32_t length = templateObject.as<TypedArrayObject>()->length();
    JSObject* resultObject = TypedArrayCreateWithTemplate(cx, templateObject, length);
    if (!resultObject) {
        return false;
    }
=======
bool RNewTypedArray::recover(JSContext* cx, SnapshotIterator& iter) const {
  RootedObject templateObject(cx, &iter.read().toObject());
  RootedValue result(cx);
>>>>>>> upstream-releases

<<<<<<< HEAD
  result.setObject(*resultObject);
  iter.storeInstructionResult(result);
  return true;
||||||| merged common ancestors
    result.setObject(*resultObject);
    iter.storeInstructionResult(result);
    return true;
=======
  uint32_t length = templateObject.as<TypedArrayObject>()->length();
  JSObject* resultObject =
      NewTypedArrayWithTemplateAndLength(cx, templateObject, length);
  if (!resultObject) {
    return false;
  }

  result.setObject(*resultObject);
  iter.storeInstructionResult(result);
  return true;
>>>>>>> upstream-releases
}

<<<<<<< HEAD
bool MNewArray::writeRecoverData(CompactBufferWriter& writer) const {
  MOZ_ASSERT(canRecoverOnBailout());
  writer.writeUnsigned(uint32_t(RInstruction::Recover_NewArray));
  writer.writeUnsigned(length());
  return true;
||||||| merged common ancestors
bool
MNewArray::writeRecoverData(CompactBufferWriter& writer) const
{
    MOZ_ASSERT(canRecoverOnBailout());
    writer.writeUnsigned(uint32_t(RInstruction::Recover_NewArray));
    writer.writeUnsigned(length());
    return true;
=======
bool MNewArray::writeRecoverData(CompactBufferWriter& writer) const {
  MOZ_ASSERT(canRecoverOnBailout());
  writer.writeUnsigned(uint32_t(RInstruction::Recover_NewArray));
  writer.writeUnsigned(length());
  writer.writeByte(uint8_t(convertDoubleElements()));
  return true;
>>>>>>> upstream-releases
}

<<<<<<< HEAD
RNewArray::RNewArray(CompactBufferReader& reader) {
  count_ = reader.readUnsigned();
||||||| merged common ancestors
RNewArray::RNewArray(CompactBufferReader& reader)
{
    count_ = reader.readUnsigned();
=======
RNewArray::RNewArray(CompactBufferReader& reader) {
  count_ = reader.readUnsigned();
  convertDoubleElements_ = reader.readByte();
>>>>>>> upstream-releases
}

bool RNewArray::recover(JSContext* cx, SnapshotIterator& iter) const {
  RootedObject templateObject(cx, &iter.read().toObject());
  RootedValue result(cx);
  RootedObjectGroup group(cx, templateObject->group());

<<<<<<< HEAD
  ArrayObject* resultObject =
      NewFullyAllocatedArrayTryUseGroup(cx, group, count_);
  if (!resultObject) {
    return false;
  }
||||||| merged common ancestors
    ArrayObject* resultObject = NewFullyAllocatedArrayTryUseGroup(cx, group, count_);
    if (!resultObject) {
        return false;
    }
=======
  ArrayObject* resultObject =
      NewArrayWithGroup(cx, count_, group, convertDoubleElements_);
  if (!resultObject) {
    return false;
  }
>>>>>>> upstream-releases

  result.setObject(*resultObject);
  iter.storeInstructionResult(result);
  return true;
}

<<<<<<< HEAD
bool MNewArrayCopyOnWrite::writeRecoverData(CompactBufferWriter& writer) const {
  MOZ_ASSERT(canRecoverOnBailout());
  writer.writeUnsigned(uint32_t(RInstruction::Recover_NewArrayCopyOnWrite));
  writer.writeByte(initialHeap());
  return true;
||||||| merged common ancestors
bool
MNewArrayCopyOnWrite::writeRecoverData(CompactBufferWriter& writer) const
{
    MOZ_ASSERT(canRecoverOnBailout());
    writer.writeUnsigned(uint32_t(RInstruction::Recover_NewArrayCopyOnWrite));
    writer.writeByte(initialHeap());
    return true;
=======
bool MNewArrayCopyOnWrite::writeRecoverData(CompactBufferWriter& writer) const {
  MOZ_ASSERT(canRecoverOnBailout());
  writer.writeUnsigned(uint32_t(RInstruction::Recover_NewArrayCopyOnWrite));
  return true;
>>>>>>> upstream-releases
}

<<<<<<< HEAD
RNewArrayCopyOnWrite::RNewArrayCopyOnWrite(CompactBufferReader& reader) {
  initialHeap_ = gc::InitialHeap(reader.readByte());
}
||||||| merged common ancestors
RNewArrayCopyOnWrite::RNewArrayCopyOnWrite(CompactBufferReader& reader)
{
    initialHeap_ = gc::InitialHeap(reader.readByte());
}
=======
RNewArrayCopyOnWrite::RNewArrayCopyOnWrite(CompactBufferReader& reader) {}
>>>>>>> upstream-releases

bool RNewArrayCopyOnWrite::recover(JSContext* cx,
                                   SnapshotIterator& iter) const {
  RootedArrayObject templateObject(cx,
                                   &iter.read().toObject().as<ArrayObject>());
  RootedValue result(cx);

<<<<<<< HEAD
  ArrayObject* resultObject =
      NewDenseCopyOnWriteArray(cx, templateObject, initialHeap_);
  if (!resultObject) {
    return false;
  }
||||||| merged common ancestors
    ArrayObject* resultObject = NewDenseCopyOnWriteArray(cx, templateObject, initialHeap_);
    if (!resultObject) {
        return false;
    }
=======
  ArrayObject* resultObject = NewDenseCopyOnWriteArray(cx, templateObject);
  if (!resultObject) {
    return false;
  }
>>>>>>> upstream-releases

  result.setObject(*resultObject);
  iter.storeInstructionResult(result);
  return true;
}

bool MNewIterator::writeRecoverData(CompactBufferWriter& writer) const {
  MOZ_ASSERT(canRecoverOnBailout());
  writer.writeUnsigned(uint32_t(RInstruction::Recover_NewIterator));
  writer.writeByte(type_);
  return true;
}

RNewIterator::RNewIterator(CompactBufferReader& reader) {
  type_ = reader.readByte();
}

bool RNewIterator::recover(JSContext* cx, SnapshotIterator& iter) const {
  RootedObject templateObject(cx, &iter.read().toObject());
  RootedValue result(cx);

<<<<<<< HEAD
  JSObject* resultObject = nullptr;
  switch (MNewIterator::Type(type_)) {
    case MNewIterator::ArrayIterator:
      resultObject = NewArrayIteratorObject(cx);
      break;
    case MNewIterator::StringIterator:
      resultObject = NewStringIteratorObject(cx);
      break;
  }
||||||| merged common ancestors
    JSObject* resultObject = nullptr;
    switch (MNewIterator::Type(type_)) {
      case MNewIterator::ArrayIterator:
        resultObject = NewArrayIteratorObject(cx);
        break;
      case MNewIterator::StringIterator:
        resultObject = NewStringIteratorObject(cx);
        break;
    }
=======
  JSObject* resultObject = nullptr;
  switch (MNewIterator::Type(type_)) {
    case MNewIterator::ArrayIterator:
      resultObject = NewArrayIteratorObject(cx);
      break;
    case MNewIterator::StringIterator:
      resultObject = NewStringIteratorObject(cx);
      break;
    case MNewIterator::RegExpStringIterator:
      resultObject = NewRegExpStringIteratorObject(cx);
      break;
  }
>>>>>>> upstream-releases

  if (!resultObject) {
    return false;
  }

  result.setObject(*resultObject);
  iter.storeInstructionResult(result);
  return true;
}

bool MNewDerivedTypedObject::writeRecoverData(
    CompactBufferWriter& writer) const {
  MOZ_ASSERT(canRecoverOnBailout());
  writer.writeUnsigned(uint32_t(RInstruction::Recover_NewDerivedTypedObject));
  return true;
}

RNewDerivedTypedObject::RNewDerivedTypedObject(CompactBufferReader& reader) {}

bool RNewDerivedTypedObject::recover(JSContext* cx,
                                     SnapshotIterator& iter) const {
  Rooted<TypeDescr*> descr(cx, &iter.read().toObject().as<TypeDescr>());
  Rooted<TypedObject*> owner(cx, &iter.read().toObject().as<TypedObject>());
  int32_t offset = iter.read().toInt32();

<<<<<<< HEAD
  JSObject* obj = OutlineTypedObject::createDerived(cx, descr, owner, offset);
  if (!obj) {
    return false;
  }
||||||| merged common ancestors
    JSObject* obj = OutlineTypedObject::createDerived(cx, descr, owner, offset);
    if (!obj) {
        return false;
    }
=======
  JSObject* obj = OutlineTypedObject::createDerived(cx, descr, owner, offset);
  if (!obj) {
    return false;
  }

  RootedValue result(cx, ObjectValue(*obj));
  iter.storeInstructionResult(result);
  return true;
}
>>>>>>> upstream-releases

<<<<<<< HEAD
  RootedValue result(cx, ObjectValue(*obj));
  iter.storeInstructionResult(result);
  return true;
||||||| merged common ancestors
    RootedValue result(cx, ObjectValue(*obj));
    iter.storeInstructionResult(result);
    return true;
=======
bool MCreateThisWithTemplate::writeRecoverData(
    CompactBufferWriter& writer) const {
  MOZ_ASSERT(canRecoverOnBailout());
  writer.writeUnsigned(uint32_t(RInstruction::Recover_CreateThisWithTemplate));
  return true;
>>>>>>> upstream-releases
}

<<<<<<< HEAD
bool MCreateThisWithTemplate::writeRecoverData(
    CompactBufferWriter& writer) const {
  MOZ_ASSERT(canRecoverOnBailout());
  writer.writeUnsigned(uint32_t(RInstruction::Recover_CreateThisWithTemplate));
  return true;
||||||| merged common ancestors
bool
MCreateThisWithTemplate::writeRecoverData(CompactBufferWriter& writer) const
{
    MOZ_ASSERT(canRecoverOnBailout());
    writer.writeUnsigned(uint32_t(RInstruction::Recover_CreateThisWithTemplate));
    return true;
=======
RCreateThisWithTemplate::RCreateThisWithTemplate(CompactBufferReader& reader) {}

bool RCreateThisWithTemplate::recover(JSContext* cx,
                                      SnapshotIterator& iter) const {
  RootedObject templateObject(cx, &iter.read().toObject());

  // See CodeGenerator::visitCreateThisWithTemplate
  JSObject* resultObject = CreateThisWithTemplate(cx, templateObject);
  if (!resultObject) {
    return false;
  }

  RootedValue result(cx);
  result.setObject(*resultObject);
  iter.storeInstructionResult(result);
  return true;
>>>>>>> upstream-releases
}

<<<<<<< HEAD
RCreateThisWithTemplate::RCreateThisWithTemplate(CompactBufferReader& reader) {}
||||||| merged common ancestors
RCreateThisWithTemplate::RCreateThisWithTemplate(CompactBufferReader& reader)
{
}
=======
bool MLambda::writeRecoverData(CompactBufferWriter& writer) const {
  MOZ_ASSERT(canRecoverOnBailout());
  writer.writeUnsigned(uint32_t(RInstruction::Recover_Lambda));
  return true;
}
>>>>>>> upstream-releases

<<<<<<< HEAD
bool RCreateThisWithTemplate::recover(JSContext* cx,
                                      SnapshotIterator& iter) const {
  RootedObject templateObject(cx, &iter.read().toObject());
||||||| merged common ancestors
bool
RCreateThisWithTemplate::recover(JSContext* cx, SnapshotIterator& iter) const
{
    RootedObject templateObject(cx, &iter.read().toObject());
=======
RLambda::RLambda(CompactBufferReader& reader) {}
>>>>>>> upstream-releases

<<<<<<< HEAD
  // See CodeGenerator::visitCreateThisWithTemplate
  JSObject* resultObject = NewObjectOperationWithTemplate(cx, templateObject);
  if (!resultObject) {
    return false;
  }
||||||| merged common ancestors
    // See CodeGenerator::visitCreateThisWithTemplate
    JSObject* resultObject = NewObjectOperationWithTemplate(cx, templateObject);
    if (!resultObject) {
        return false;
    }
=======
bool RLambda::recover(JSContext* cx, SnapshotIterator& iter) const {
  RootedObject scopeChain(cx, &iter.read().toObject());
  RootedFunction fun(cx, &iter.read().toObject().as<JSFunction>());

  JSObject* resultObject = js::Lambda(cx, fun, scopeChain);
  if (!resultObject) {
    return false;
  }
>>>>>>> upstream-releases

  RootedValue result(cx);
  result.setObject(*resultObject);
  iter.storeInstructionResult(result);
  return true;
}

<<<<<<< HEAD
bool MLambda::writeRecoverData(CompactBufferWriter& writer) const {
  MOZ_ASSERT(canRecoverOnBailout());
  writer.writeUnsigned(uint32_t(RInstruction::Recover_Lambda));
  return true;
||||||| merged common ancestors
bool
MLambda::writeRecoverData(CompactBufferWriter& writer) const
{
    MOZ_ASSERT(canRecoverOnBailout());
    writer.writeUnsigned(uint32_t(RInstruction::Recover_Lambda));
    return true;
=======
bool MLambdaArrow::writeRecoverData(CompactBufferWriter& writer) const {
  MOZ_ASSERT(canRecoverOnBailout());
  writer.writeUnsigned(uint32_t(RInstruction::Recover_LambdaArrow));
  return true;
>>>>>>> upstream-releases
}

<<<<<<< HEAD
RLambda::RLambda(CompactBufferReader& reader) {}
||||||| merged common ancestors
RLambda::RLambda(CompactBufferReader& reader)
{
}
=======
RLambdaArrow::RLambdaArrow(CompactBufferReader& reader) {}

bool RLambdaArrow::recover(JSContext* cx, SnapshotIterator& iter) const {
  RootedObject scopeChain(cx, &iter.read().toObject());
  RootedValue newTarget(cx, iter.read());
  RootedFunction fun(cx, &iter.read().toObject().as<JSFunction>());

  JSObject* resultObject = js::LambdaArrow(cx, fun, scopeChain, newTarget);
  if (!resultObject) {
    return false;
  }

  RootedValue result(cx);
  result.setObject(*resultObject);
  iter.storeInstructionResult(result);
  return true;
}
>>>>>>> upstream-releases

<<<<<<< HEAD
bool RLambda::recover(JSContext* cx, SnapshotIterator& iter) const {
  RootedObject scopeChain(cx, &iter.read().toObject());
  RootedFunction fun(cx, &iter.read().toObject().as<JSFunction>());
||||||| merged common ancestors
bool
RLambda::recover(JSContext* cx, SnapshotIterator& iter) const
{
    RootedObject scopeChain(cx, &iter.read().toObject());
    RootedFunction fun(cx, &iter.read().toObject().as<JSFunction>());
=======
bool MNewCallObject::writeRecoverData(CompactBufferWriter& writer) const {
  MOZ_ASSERT(canRecoverOnBailout());
  writer.writeUnsigned(uint32_t(RInstruction::Recover_NewCallObject));
  return true;
}
>>>>>>> upstream-releases

<<<<<<< HEAD
  JSObject* resultObject = js::Lambda(cx, fun, scopeChain);
  if (!resultObject) {
    return false;
  }
||||||| merged common ancestors
    JSObject* resultObject = js::Lambda(cx, fun, scopeChain);
    if (!resultObject) {
        return false;
    }
=======
RNewCallObject::RNewCallObject(CompactBufferReader& reader) {}

bool RNewCallObject::recover(JSContext* cx, SnapshotIterator& iter) const {
  Rooted<CallObject*> templateObj(cx, &iter.read().toObject().as<CallObject>());

  RootedShape shape(cx, templateObj->lastProperty());
  RootedObjectGroup group(cx, templateObj->group());
  JSObject* resultObject = NewCallObject(cx, shape, group);
  if (!resultObject) {
    return false;
  }
>>>>>>> upstream-releases

  RootedValue result(cx);
  result.setObject(*resultObject);
  iter.storeInstructionResult(result);
  return true;
}

<<<<<<< HEAD
bool MLambdaArrow::writeRecoverData(CompactBufferWriter& writer) const {
  MOZ_ASSERT(canRecoverOnBailout());
  writer.writeUnsigned(uint32_t(RInstruction::Recover_LambdaArrow));
  return true;
||||||| merged common ancestors
bool
MLambdaArrow::writeRecoverData(CompactBufferWriter& writer) const
{
    MOZ_ASSERT(canRecoverOnBailout());
    writer.writeUnsigned(uint32_t(RInstruction::Recover_LambdaArrow));
    return true;
=======
bool MObjectState::writeRecoverData(CompactBufferWriter& writer) const {
  MOZ_ASSERT(canRecoverOnBailout());
  writer.writeUnsigned(uint32_t(RInstruction::Recover_ObjectState));
  writer.writeUnsigned(numSlots());
  return true;
>>>>>>> upstream-releases
}

<<<<<<< HEAD
RLambdaArrow::RLambdaArrow(CompactBufferReader& reader) {}

bool RLambdaArrow::recover(JSContext* cx, SnapshotIterator& iter) const {
  RootedObject scopeChain(cx, &iter.read().toObject());
  RootedValue newTarget(cx, iter.read());
  RootedFunction fun(cx, &iter.read().toObject().as<JSFunction>());

  JSObject* resultObject = js::LambdaArrow(cx, fun, scopeChain, newTarget);
  if (!resultObject) {
    return false;
  }

  RootedValue result(cx);
  result.setObject(*resultObject);
  iter.storeInstructionResult(result);
  return true;
||||||| merged common ancestors
RLambdaArrow::RLambdaArrow(CompactBufferReader& reader)
{
=======
RObjectState::RObjectState(CompactBufferReader& reader) {
  numSlots_ = reader.readUnsigned();
>>>>>>> upstream-releases
}

<<<<<<< HEAD
bool MNewCallObject::writeRecoverData(CompactBufferWriter& writer) const {
  MOZ_ASSERT(canRecoverOnBailout());
  writer.writeUnsigned(uint32_t(RInstruction::Recover_NewCallObject));
  return true;
}
||||||| merged common ancestors
bool
RLambdaArrow::recover(JSContext* cx, SnapshotIterator& iter) const
{
    RootedObject scopeChain(cx, &iter.read().toObject());
    RootedValue newTarget(cx, iter.read());
    RootedFunction fun(cx, &iter.read().toObject().as<JSFunction>());
=======
bool RObjectState::recover(JSContext* cx, SnapshotIterator& iter) const {
  RootedObject object(cx, &iter.read().toObject());
  RootedValue val(cx);
  RootedNativeObject nativeObject(cx, &object->as<NativeObject>());
  MOZ_ASSERT(nativeObject->slotSpan() == numSlots());
>>>>>>> upstream-releases

<<<<<<< HEAD
RNewCallObject::RNewCallObject(CompactBufferReader& reader) {}

bool RNewCallObject::recover(JSContext* cx, SnapshotIterator& iter) const {
  Rooted<CallObject*> templateObj(cx, &iter.read().toObject().as<CallObject>());

  RootedShape shape(cx, templateObj->lastProperty());
  RootedObjectGroup group(cx, templateObj->group());
  JSObject* resultObject = NewCallObject(cx, shape, group);
  if (!resultObject) {
    return false;
  }
||||||| merged common ancestors
    JSObject* resultObject = js::LambdaArrow(cx, fun, scopeChain, newTarget);
    if (!resultObject) {
        return false;
    }
=======
  for (size_t i = 0; i < numSlots(); i++) {
    val = iter.read();
    nativeObject->setSlot(i, val);
  }
>>>>>>> upstream-releases

<<<<<<< HEAD
  RootedValue result(cx);
  result.setObject(*resultObject);
  iter.storeInstructionResult(result);
  return true;
||||||| merged common ancestors
    RootedValue result(cx);
    result.setObject(*resultObject);
    iter.storeInstructionResult(result);
    return true;
=======
  val.setObject(*object);
  iter.storeInstructionResult(val);
  return true;
>>>>>>> upstream-releases
}

<<<<<<< HEAD
bool MObjectState::writeRecoverData(CompactBufferWriter& writer) const {
  MOZ_ASSERT(canRecoverOnBailout());
  writer.writeUnsigned(uint32_t(RInstruction::Recover_ObjectState));
  writer.writeUnsigned(numSlots());
  return true;
||||||| merged common ancestors
bool
MNewCallObject::writeRecoverData(CompactBufferWriter& writer) const
{
    MOZ_ASSERT(canRecoverOnBailout());
    writer.writeUnsigned(uint32_t(RInstruction::Recover_NewCallObject));
    return true;
=======
bool MArrayState::writeRecoverData(CompactBufferWriter& writer) const {
  MOZ_ASSERT(canRecoverOnBailout());
  writer.writeUnsigned(uint32_t(RInstruction::Recover_ArrayState));
  writer.writeUnsigned(numElements());
  return true;
>>>>>>> upstream-releases
}

<<<<<<< HEAD
RObjectState::RObjectState(CompactBufferReader& reader) {
  numSlots_ = reader.readUnsigned();
||||||| merged common ancestors
RNewCallObject::RNewCallObject(CompactBufferReader& reader)
{
=======
RArrayState::RArrayState(CompactBufferReader& reader) {
  numElements_ = reader.readUnsigned();
>>>>>>> upstream-releases
}

<<<<<<< HEAD
bool RObjectState::recover(JSContext* cx, SnapshotIterator& iter) const {
  RootedObject object(cx, &iter.read().toObject());
  RootedValue val(cx);
||||||| merged common ancestors
bool
RNewCallObject::recover(JSContext* cx, SnapshotIterator& iter) const
{
    Rooted<CallObject*> templateObj(cx, &iter.read().toObject().as<CallObject>());
=======
bool RArrayState::recover(JSContext* cx, SnapshotIterator& iter) const {
  RootedValue result(cx);
  ArrayObject* object = &iter.read().toObject().as<ArrayObject>();
  uint32_t initLength = iter.read().toInt32();
>>>>>>> upstream-releases

<<<<<<< HEAD
  if (object->is<UnboxedPlainObject>()) {
    const UnboxedLayout& layout = object->as<UnboxedPlainObject>().layout();

    RootedId id(cx);
    RootedValue receiver(cx, ObjectValue(*object));
    const UnboxedLayout::PropertyVector& properties = layout.properties();
    for (size_t i = 0; i < properties.length(); i++) {
      val = iter.read();

      // This is the default placeholder value of MObjectState, when no
      // properties are defined yet.
      if (val.isUndefined()) {
        continue;
      }
||||||| merged common ancestors
    RootedShape shape(cx, templateObj->lastProperty());
    RootedObjectGroup group(cx, templateObj->group());
    JSObject* resultObject = NewCallObject(cx, shape, group);
    if (!resultObject) {
        return false;
    }
=======
  if (!object->denseElementsAreCopyOnWrite()) {
    MOZ_ASSERT(object->getDenseInitializedLength() == 0,
               "initDenseElement call below relies on this");
    object->setDenseInitializedLength(initLength);
>>>>>>> upstream-releases

<<<<<<< HEAD
      id = NameToId(properties[i].name);
      ObjectOpResult result;

      // SetProperty can only fail due to OOM.
      if (!SetProperty(cx, object, id, val, receiver, result)) {
        return false;
      }
      if (!result) {
        return result.reportError(cx, object, id);
      }
    }
  } else {
    RootedNativeObject nativeObject(cx, &object->as<NativeObject>());
    MOZ_ASSERT(nativeObject->slotSpan() == numSlots());
||||||| merged common ancestors
    RootedValue result(cx);
    result.setObject(*resultObject);
    iter.storeInstructionResult(result);
    return true;
}

bool
MObjectState::writeRecoverData(CompactBufferWriter& writer) const
{
    MOZ_ASSERT(canRecoverOnBailout());
    writer.writeUnsigned(uint32_t(RInstruction::Recover_ObjectState));
    writer.writeUnsigned(numSlots());
    return true;
}

RObjectState::RObjectState(CompactBufferReader& reader)
{
    numSlots_ = reader.readUnsigned();
}

bool
RObjectState::recover(JSContext* cx, SnapshotIterator& iter) const
{
    RootedObject object(cx, &iter.read().toObject());
    RootedValue val(cx);

    if (object->is<UnboxedPlainObject>()) {
        const UnboxedLayout& layout = object->as<UnboxedPlainObject>().layout();

        RootedId id(cx);
        RootedValue receiver(cx, ObjectValue(*object));
        const UnboxedLayout::PropertyVector& properties = layout.properties();
        for (size_t i = 0; i < properties.length(); i++) {
            val = iter.read();

            // This is the default placeholder value of MObjectState, when no
            // properties are defined yet.
            if (val.isUndefined()) {
                continue;
            }

            id = NameToId(properties[i].name);
            ObjectOpResult result;

            // SetProperty can only fail due to OOM.
            if (!SetProperty(cx, object, id, val, receiver, result)) {
                return false;
            }
            if (!result) {
                return result.reportError(cx, object, id);
            }
        }
    } else {
        RootedNativeObject nativeObject(cx, &object->as<NativeObject>());
        MOZ_ASSERT(nativeObject->slotSpan() == numSlots());

        for (size_t i = 0; i < numSlots(); i++) {
            val = iter.read();
            nativeObject->setSlot(i, val);
        }
    }
=======
    for (size_t index = 0; index < numElements(); index++) {
      Value val = iter.read();
>>>>>>> upstream-releases

<<<<<<< HEAD
    for (size_t i = 0; i < numSlots(); i++) {
      val = iter.read();
      nativeObject->setSlot(i, val);
||||||| merged common ancestors
    val.setObject(*object);
    iter.storeInstructionResult(val);
    return true;
}

bool
MArrayState::writeRecoverData(CompactBufferWriter& writer) const
{
    MOZ_ASSERT(canRecoverOnBailout());
    writer.writeUnsigned(uint32_t(RInstruction::Recover_ArrayState));
    writer.writeUnsigned(numElements());
    return true;
}

RArrayState::RArrayState(CompactBufferReader& reader)
{
    numElements_ = reader.readUnsigned();
}

bool
RArrayState::recover(JSContext* cx, SnapshotIterator& iter) const
{
    RootedValue result(cx);
    ArrayObject* object = &iter.read().toObject().as<ArrayObject>();
    uint32_t initLength = iter.read().toInt32();

    if (!object->denseElementsAreCopyOnWrite()) {
        MOZ_ASSERT(object->getDenseInitializedLength() == 0,
                   "initDenseElement call below relies on this");
        object->setDenseInitializedLength(initLength);

        for (size_t index = 0; index < numElements(); index++) {
            Value val = iter.read();

            if (index >= initLength) {
                MOZ_ASSERT(val.isUndefined());
                continue;
            }

            object->initDenseElement(index, val);
        }
    } else {
        MOZ_RELEASE_ASSERT(object->getDenseInitializedLength() == numElements());
        MOZ_RELEASE_ASSERT(initLength == numElements());

        for (size_t index = 0; index < numElements(); index++) {
            Value val = iter.read();
            if (object->getDenseElement(index) == val) {
                continue;
            }
            if (!object->maybeCopyElementsForWrite(cx)) {
                return false;
            }
            object->setDenseElement(index, val);
        }
=======
      if (index >= initLength) {
        MOZ_ASSERT(val.isUndefined());
        continue;
      }

      object->initDenseElement(index, val);
>>>>>>> upstream-releases
    }
<<<<<<< HEAD
  }
||||||| merged common ancestors
=======
  } else {
    MOZ_RELEASE_ASSERT(object->getDenseInitializedLength() == numElements());
    MOZ_RELEASE_ASSERT(initLength == numElements());

    for (size_t index = 0; index < numElements(); index++) {
      Value val = iter.read();
      if (object->getDenseElement(index) == val) {
        continue;
      }
      if (!object->maybeCopyElementsForWrite(cx)) {
        return false;
      }
      object->setDenseElement(index, val);
    }
  }
>>>>>>> upstream-releases

<<<<<<< HEAD
  val.setObject(*object);
  iter.storeInstructionResult(val);
  return true;
||||||| merged common ancestors
    result.setObject(*object);
    iter.storeInstructionResult(result);
    return true;
=======
  result.setObject(*object);
  iter.storeInstructionResult(result);
  return true;
>>>>>>> upstream-releases
}

<<<<<<< HEAD
bool MArrayState::writeRecoverData(CompactBufferWriter& writer) const {
  MOZ_ASSERT(canRecoverOnBailout());
  writer.writeUnsigned(uint32_t(RInstruction::Recover_ArrayState));
  writer.writeUnsigned(numElements());
  return true;
||||||| merged common ancestors
bool
MSetArrayLength::writeRecoverData(CompactBufferWriter& writer) const
{
    MOZ_ASSERT(canRecoverOnBailout());
    // For simplicity, we capture directly the object instead of the elements
    // pointer.
    MOZ_ASSERT(elements()->type() != MIRType::Elements);
    writer.writeUnsigned(uint32_t(RInstruction::Recover_SetArrayLength));
    return true;
=======
bool MSetArrayLength::writeRecoverData(CompactBufferWriter& writer) const {
  MOZ_ASSERT(canRecoverOnBailout());
  // For simplicity, we capture directly the object instead of the elements
  // pointer.
  MOZ_ASSERT(elements()->type() != MIRType::Elements);
  writer.writeUnsigned(uint32_t(RInstruction::Recover_SetArrayLength));
  return true;
>>>>>>> upstream-releases
}

<<<<<<< HEAD
RArrayState::RArrayState(CompactBufferReader& reader) {
  numElements_ = reader.readUnsigned();
}
||||||| merged common ancestors
RSetArrayLength::RSetArrayLength(CompactBufferReader& reader)
{
}
=======
RSetArrayLength::RSetArrayLength(CompactBufferReader& reader) {}
>>>>>>> upstream-releases

<<<<<<< HEAD
bool RArrayState::recover(JSContext* cx, SnapshotIterator& iter) const {
  RootedValue result(cx);
  ArrayObject* object = &iter.read().toObject().as<ArrayObject>();
  uint32_t initLength = iter.read().toInt32();

  if (!object->denseElementsAreCopyOnWrite()) {
    MOZ_ASSERT(object->getDenseInitializedLength() == 0,
               "initDenseElement call below relies on this");
    object->setDenseInitializedLength(initLength);

    for (size_t index = 0; index < numElements(); index++) {
      Value val = iter.read();
||||||| merged common ancestors
bool
RSetArrayLength::recover(JSContext* cx, SnapshotIterator& iter) const
{
    RootedValue result(cx);
    RootedArrayObject obj(cx, &iter.read().toObject().as<ArrayObject>());
    RootedValue len(cx, iter.read());
=======
bool RSetArrayLength::recover(JSContext* cx, SnapshotIterator& iter) const {
  RootedValue result(cx);
  RootedArrayObject obj(cx, &iter.read().toObject().as<ArrayObject>());
  RootedValue len(cx, iter.read());
>>>>>>> upstream-releases

<<<<<<< HEAD
      if (index >= initLength) {
        MOZ_ASSERT(val.isUndefined());
        continue;
      }

      object->initDenseElement(index, val);
    }
  } else {
    MOZ_RELEASE_ASSERT(object->getDenseInitializedLength() == numElements());
    MOZ_RELEASE_ASSERT(initLength == numElements());

    for (size_t index = 0; index < numElements(); index++) {
      Value val = iter.read();
      if (object->getDenseElement(index) == val) {
        continue;
      }
      if (!object->maybeCopyElementsForWrite(cx)) {
        return false;
      }
      object->setDenseElement(index, val);
    }
  }
||||||| merged common ancestors
    RootedId id(cx, NameToId(cx->names().length));
    ObjectOpResult error;
    if (!ArraySetLength(cx, obj, id, JSPROP_PERMANENT, len, error)) {
        return false;
    }
=======
  RootedId id(cx, NameToId(cx->names().length));
  ObjectOpResult error;
  if (!ArraySetLength(cx, obj, id, JSPROP_PERMANENT, len, error)) {
    return false;
  }
>>>>>>> upstream-releases

<<<<<<< HEAD
  result.setObject(*object);
  iter.storeInstructionResult(result);
  return true;
||||||| merged common ancestors
    result.setObject(*obj);
    iter.storeInstructionResult(result);
    return true;
=======
  result.setObject(*obj);
  iter.storeInstructionResult(result);
  return true;
>>>>>>> upstream-releases
}

<<<<<<< HEAD
bool MSetArrayLength::writeRecoverData(CompactBufferWriter& writer) const {
  MOZ_ASSERT(canRecoverOnBailout());
  // For simplicity, we capture directly the object instead of the elements
  // pointer.
  MOZ_ASSERT(elements()->type() != MIRType::Elements);
  writer.writeUnsigned(uint32_t(RInstruction::Recover_SetArrayLength));
  return true;
||||||| merged common ancestors
bool
MAssertRecoveredOnBailout::writeRecoverData(CompactBufferWriter& writer) const
{
    MOZ_ASSERT(canRecoverOnBailout());
    MOZ_RELEASE_ASSERT(input()->isRecoveredOnBailout() == mustBeRecovered_,
        "assertRecoveredOnBailout failed during compilation");
    writer.writeUnsigned(uint32_t(RInstruction::Recover_AssertRecoveredOnBailout));
    return true;
=======
bool MAssertRecoveredOnBailout::writeRecoverData(
    CompactBufferWriter& writer) const {
  MOZ_ASSERT(canRecoverOnBailout());
  MOZ_RELEASE_ASSERT(input()->isRecoveredOnBailout() == mustBeRecovered_,
                     "assertRecoveredOnBailout failed during compilation");
  writer.writeUnsigned(
      uint32_t(RInstruction::Recover_AssertRecoveredOnBailout));
  return true;
>>>>>>> upstream-releases
}

<<<<<<< HEAD
RSetArrayLength::RSetArrayLength(CompactBufferReader& reader) {}
||||||| merged common ancestors
RAssertRecoveredOnBailout::RAssertRecoveredOnBailout(CompactBufferReader& reader)
{ }
=======
RAssertRecoveredOnBailout::RAssertRecoveredOnBailout(
    CompactBufferReader& reader) {}
>>>>>>> upstream-releases

<<<<<<< HEAD
bool RSetArrayLength::recover(JSContext* cx, SnapshotIterator& iter) const {
  RootedValue result(cx);
  RootedArrayObject obj(cx, &iter.read().toObject().as<ArrayObject>());
  RootedValue len(cx, iter.read());

  RootedId id(cx, NameToId(cx->names().length));
  ObjectOpResult error;
  if (!ArraySetLength(cx, obj, id, JSPROP_PERMANENT, len, error)) {
    return false;
  }

  result.setObject(*obj);
  iter.storeInstructionResult(result);
  return true;
||||||| merged common ancestors
bool RAssertRecoveredOnBailout::recover(JSContext* cx, SnapshotIterator& iter) const
{
    RootedValue result(cx);
    iter.read(); // skip the unused operand.
    result.setUndefined();
    iter.storeInstructionResult(result);
    return true;
=======
bool RAssertRecoveredOnBailout::recover(JSContext* cx,
                                        SnapshotIterator& iter) const {
  RootedValue result(cx);
  iter.read();  // skip the unused operand.
  result.setUndefined();
  iter.storeInstructionResult(result);
  return true;
>>>>>>> upstream-releases
}

<<<<<<< HEAD
bool MAssertRecoveredOnBailout::writeRecoverData(
    CompactBufferWriter& writer) const {
  MOZ_ASSERT(canRecoverOnBailout());
  MOZ_RELEASE_ASSERT(input()->isRecoveredOnBailout() == mustBeRecovered_,
                     "assertRecoveredOnBailout failed during compilation");
  writer.writeUnsigned(
      uint32_t(RInstruction::Recover_AssertRecoveredOnBailout));
  return true;
||||||| merged common ancestors
bool
MStringReplace::writeRecoverData(CompactBufferWriter& writer) const
{
    MOZ_ASSERT(canRecoverOnBailout());
    writer.writeUnsigned(uint32_t(RInstruction::Recover_StringReplace));
    writer.writeByte(isFlatReplacement_);
    return true;
=======
bool MStringReplace::writeRecoverData(CompactBufferWriter& writer) const {
  MOZ_ASSERT(canRecoverOnBailout());
  writer.writeUnsigned(uint32_t(RInstruction::Recover_StringReplace));
  writer.writeByte(isFlatReplacement_);
  return true;
>>>>>>> upstream-releases
}

<<<<<<< HEAD
RAssertRecoveredOnBailout::RAssertRecoveredOnBailout(
    CompactBufferReader& reader) {}

bool RAssertRecoveredOnBailout::recover(JSContext* cx,
                                        SnapshotIterator& iter) const {
  RootedValue result(cx);
  iter.read();  // skip the unused operand.
  result.setUndefined();
  iter.storeInstructionResult(result);
  return true;
||||||| merged common ancestors
RStringReplace::RStringReplace(CompactBufferReader& reader)
{
    isFlatReplacement_ = reader.readByte();
=======
RStringReplace::RStringReplace(CompactBufferReader& reader) {
  isFlatReplacement_ = reader.readByte();
>>>>>>> upstream-releases
}

<<<<<<< HEAD
bool MStringReplace::writeRecoverData(CompactBufferWriter& writer) const {
  MOZ_ASSERT(canRecoverOnBailout());
  writer.writeUnsigned(uint32_t(RInstruction::Recover_StringReplace));
  writer.writeByte(isFlatReplacement_);
  return true;
}
||||||| merged common ancestors
bool RStringReplace::recover(JSContext* cx, SnapshotIterator& iter) const
{
    RootedString string(cx, iter.read().toString());
    RootedString pattern(cx, iter.read().toString());
    RootedString replace(cx, iter.read().toString());
=======
bool RStringReplace::recover(JSContext* cx, SnapshotIterator& iter) const {
  RootedString string(cx, iter.read().toString());
  RootedString pattern(cx, iter.read().toString());
  RootedString replace(cx, iter.read().toString());
>>>>>>> upstream-releases

<<<<<<< HEAD
RStringReplace::RStringReplace(CompactBufferReader& reader) {
  isFlatReplacement_ = reader.readByte();
}
||||||| merged common ancestors
    JSString* result = isFlatReplacement_ ? js::str_flat_replace_string(cx, string, pattern, replace) :
                                            js::str_replace_string_raw(cx, string, pattern, replace);
=======
  JSString* result =
      isFlatReplacement_
          ? js::StringFlatReplaceString(cx, string, pattern, replace)
          : js::str_replace_string_raw(cx, string, pattern, replace);
>>>>>>> upstream-releases

<<<<<<< HEAD
bool RStringReplace::recover(JSContext* cx, SnapshotIterator& iter) const {
  RootedString string(cx, iter.read().toString());
  RootedString pattern(cx, iter.read().toString());
  RootedString replace(cx, iter.read().toString());

  JSString* result =
      isFlatReplacement_
          ? js::str_flat_replace_string(cx, string, pattern, replace)
          : js::str_replace_string_raw(cx, string, pattern, replace);

  if (!result) {
    return false;
  }
||||||| merged common ancestors
    if (!result) {
        return false;
    }
=======
  if (!result) {
    return false;
  }
>>>>>>> upstream-releases

  iter.storeInstructionResult(StringValue(result));
  return true;
}

bool MAtomicIsLockFree::writeRecoverData(CompactBufferWriter& writer) const {
  MOZ_ASSERT(canRecoverOnBailout());
  writer.writeUnsigned(uint32_t(RInstruction::Recover_AtomicIsLockFree));
  return true;
}

RAtomicIsLockFree::RAtomicIsLockFree(CompactBufferReader& reader) {}

bool RAtomicIsLockFree::recover(JSContext* cx, SnapshotIterator& iter) const {
  RootedValue operand(cx, iter.read());
  MOZ_ASSERT(operand.isInt32());

  int32_t result;
  if (!js::AtomicIsLockFree(cx, operand, &result)) {
    return false;
  }

  RootedValue rootedResult(cx, js::Int32Value(result));
  iter.storeInstructionResult(rootedResult);
  return true;
}
