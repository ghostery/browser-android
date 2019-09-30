/* -*- Mode: C++; tab-width: 8; indent-tabs-mode: nil; c-basic-offset: 2 -*-
 * vim: set ts=8 sts=2 et sw=2 tw=80:
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/. */

#ifndef jit_arm64_LIR_arm64_h
#define jit_arm64_LIR_arm64_h

namespace js {
namespace jit {

class LUnboxBase : public LInstructionHelper<1, 1, 0> {
 public:
  LUnboxBase(LNode::Opcode opcode, const LAllocation& input)
      : LInstructionHelper(opcode) {
    setOperand(0, input);
  }

  static const size_t Input = 0;

  MUnbox* mir() const { return mir_->toUnbox(); }
};

class LUnbox : public LUnboxBase {
 public:
  LIR_HEADER(Unbox);

  explicit LUnbox(const LAllocation& input) : LUnboxBase(classOpcode, input) {}

  const char* extraName() const { return StringFromMIRType(mir()->type()); }
};

class LUnboxFloatingPoint : public LUnboxBase {
  MIRType type_;

 public:
  LIR_HEADER(UnboxFloatingPoint);

  LUnboxFloatingPoint(const LAllocation& input, MIRType type)
      : LUnboxBase(classOpcode, input), type_(type) {}

  MIRType type() const { return type_; }
  const char* extraName() const { return StringFromMIRType(type_); }
};

// Convert a 32-bit unsigned integer to a double.
class LWasmUint32ToDouble : public LInstructionHelper<1, 1, 0> {
 public:
  LIR_HEADER(WasmUint32ToDouble)

  explicit LWasmUint32ToDouble(const LAllocation& input)
      : LInstructionHelper(classOpcode) {
    setOperand(0, input);
  }
};

// Convert a 32-bit unsigned integer to a float32.
class LWasmUint32ToFloat32 : public LInstructionHelper<1, 1, 0> {
 public:
  LIR_HEADER(WasmUint32ToFloat32)

  explicit LWasmUint32ToFloat32(const LAllocation& input)
      : LInstructionHelper(classOpcode) {
    setOperand(0, input);
  }
};

<<<<<<< HEAD
class LDivI : public LBinaryMath<1> {
 public:
  LIR_HEADER(DivI);

  LDivI(const LAllocation& lhs, const LAllocation& rhs, const LDefinition& temp)
      : LBinaryMath(classOpcode) {
    setOperand(0, lhs);
    setOperand(1, rhs);
    setTemp(0, temp);
  }

  MDiv* mir() const { return mir_->toDiv(); }
};
||||||| merged common ancestors
class LDivI : public LBinaryMath<1>
{
  public:
    LIR_HEADER(DivI);

    LDivI(const LAllocation& lhs, const LAllocation& rhs,
          const LDefinition& temp)
      : LBinaryMath(classOpcode)
    {
        setOperand(0, lhs);
        setOperand(1, rhs);
        setTemp(0, temp);
    }

    MDiv* mir() const {
        return mir_->toDiv();
    }
};
=======
class LDivI : public LBinaryMath<1> {
 public:
  LIR_HEADER(DivI);

  LDivI(const LAllocation& lhs, const LAllocation& rhs, const LDefinition& temp)
      : LBinaryMath(classOpcode) {
    setOperand(0, lhs);
    setOperand(1, rhs);
    setTemp(0, temp);
  }
>>>>>>> upstream-releases

<<<<<<< HEAD
// LSoftDivI is a software divide for ARM cores that don't support a hardware
// divide instruction.
//
// It is implemented as a proper C function so it trashes r0, r1, r2 and r3.
// The call also trashes lr, and has the ability to trash ip. The function also
// takes two arguments (dividend in r0, divisor in r1). The LInstruction gets
// encoded such that the divisor and dividend are passed in their apropriate
// registers and end their life at the start of the instruction by the use of
// useFixedAtStart.  The result is returned in r0 and the other three registers
// that can be trashed are marked as temps.  For the time being, the link
// register is not marked as trashed because we never allocate to the link
// register.  The FP registers are not trashed.
class LSoftDivI : public LBinaryMath<3> {
 public:
  LIR_HEADER(SoftDivI);

  LSoftDivI(const LAllocation& lhs, const LAllocation& rhs,
            const LDefinition& temp1, const LDefinition& temp2,
            const LDefinition& temp3)
      : LBinaryMath(classOpcode) {
    setOperand(0, lhs);
    setOperand(1, rhs);
    setTemp(0, temp1);
    setTemp(1, temp2);
    setTemp(2, temp3);
  }

  MDiv* mir() const { return mir_->toDiv(); }
||||||| merged common ancestors
// LSoftDivI is a software divide for ARM cores that don't support a hardware
// divide instruction.
//
// It is implemented as a proper C function so it trashes r0, r1, r2 and r3.
// The call also trashes lr, and has the ability to trash ip. The function also
// takes two arguments (dividend in r0, divisor in r1). The LInstruction gets
// encoded such that the divisor and dividend are passed in their apropriate
// registers and end their life at the start of the instruction by the use of
// useFixedAtStart.  The result is returned in r0 and the other three registers
// that can be trashed are marked as temps.  For the time being, the link
// register is not marked as trashed because we never allocate to the link
// register.  The FP registers are not trashed.
class LSoftDivI : public LBinaryMath<3>
{
  public:
    LIR_HEADER(SoftDivI);

    LSoftDivI(const LAllocation& lhs, const LAllocation& rhs,
              const LDefinition& temp1, const LDefinition& temp2, const LDefinition& temp3)
      : LBinaryMath(classOpcode)
    {
        setOperand(0, lhs);
        setOperand(1, rhs);
        setTemp(0, temp1);
        setTemp(1, temp2);
        setTemp(2, temp3);
    }

    MDiv* mir() const {
        return mir_->toDiv();
    }
=======
  MDiv* mir() const { return mir_->toDiv(); }
>>>>>>> upstream-releases
};

<<<<<<< HEAD
class LDivPowTwoI : public LInstructionHelper<1, 1, 0> {
  const int32_t shift_;
||||||| merged common ancestors
class LDivPowTwoI : public LInstructionHelper<1, 1, 0>
{
    const int32_t shift_;
=======
class LDivPowTwoI : public LInstructionHelper<1, 1, 0> {
  const int32_t shift_;
  const bool negativeDivisor_;
>>>>>>> upstream-releases

 public:
  LIR_HEADER(DivPowTwoI)

<<<<<<< HEAD
  LDivPowTwoI(const LAllocation& lhs, int32_t shift)
      : LInstructionHelper(classOpcode), shift_(shift) {
    setOperand(0, lhs);
  }

  const LAllocation* numerator() { return getOperand(0); }

  int32_t shift() { return shift_; }

  MDiv* mir() const { return mir_->toDiv(); }
||||||| merged common ancestors
    LDivPowTwoI(const LAllocation& lhs, int32_t shift)
      : LInstructionHelper(classOpcode),
        shift_(shift)
    {
        setOperand(0, lhs);
    }

    const LAllocation* numerator() {
        return getOperand(0);
    }

    int32_t shift() {
        return shift_;
    }

    MDiv* mir() const {
        return mir_->toDiv();
    }
=======
  LDivPowTwoI(const LAllocation& lhs, int32_t shift, bool negativeDivisor)
      : LInstructionHelper(classOpcode),
        shift_(shift),
        negativeDivisor_(negativeDivisor) {
    setOperand(0, lhs);
  }

  const LAllocation* numerator() { return getOperand(0); }

  int32_t shift() { return shift_; }
  bool negativeDivisor() { return negativeDivisor_; }

  MDiv* mir() const { return mir_->toDiv(); }
>>>>>>> upstream-releases
};

<<<<<<< HEAD
class LModI : public LBinaryMath<1> {
 public:
  LIR_HEADER(ModI);

  LModI(const LAllocation& lhs, const LAllocation& rhs,
        const LDefinition& callTemp)
      : LBinaryMath(classOpcode) {
    setOperand(0, lhs);
    setOperand(1, rhs);
    setTemp(0, callTemp);
  }

  const LDefinition* callTemp() { return getTemp(0); }

  MMod* mir() const { return mir_->toMod(); }
||||||| merged common ancestors
class LModI : public LBinaryMath<1>
{
  public:
    LIR_HEADER(ModI);

    LModI(const LAllocation& lhs, const LAllocation& rhs,
          const LDefinition& callTemp)
      : LBinaryMath(classOpcode)
    {
        setOperand(0, lhs);
        setOperand(1, rhs);
        setTemp(0, callTemp);
    }

    const LDefinition* callTemp() {
        return getTemp(0);
    }

    MMod* mir() const {
        return mir_->toMod();
    }
=======
class LDivConstantI : public LInstructionHelper<1, 1, 1> {
  const int32_t denominator_;

 public:
  LIR_HEADER(DivConstantI)

  LDivConstantI(const LAllocation& lhs, int32_t denominator,
                const LDefinition& temp)
      : LInstructionHelper(classOpcode), denominator_(denominator) {
    setOperand(0, lhs);
    setTemp(0, temp);
  }

  const LAllocation* numerator() { return getOperand(0); }
  const LDefinition* temp() { return getTemp(0); }
  int32_t denominator() const { return denominator_; }
  MDiv* mir() const { return mir_->toDiv(); }
  bool canBeNegativeDividend() const { return mir()->canBeNegativeDividend(); }
>>>>>>> upstream-releases
};

<<<<<<< HEAD
class LSoftModI : public LBinaryMath<4> {
 public:
  LIR_HEADER(SoftModI);

  LSoftModI(const LAllocation& lhs, const LAllocation& rhs,
            const LDefinition& temp1, const LDefinition& temp2,
            const LDefinition& temp3, const LDefinition& callTemp)
      : LBinaryMath(classOpcode) {
    setOperand(0, lhs);
    setOperand(1, rhs);
    setTemp(0, temp1);
    setTemp(1, temp2);
    setTemp(2, temp3);
    setTemp(3, callTemp);
  }

  const LDefinition* callTemp() { return getTemp(3); }

  MMod* mir() const { return mir_->toMod(); }
||||||| merged common ancestors
class LSoftModI : public LBinaryMath<4>
{
  public:
    LIR_HEADER(SoftModI);

    LSoftModI(const LAllocation& lhs, const LAllocation& rhs,
              const LDefinition& temp1, const LDefinition& temp2, const LDefinition& temp3,
              const LDefinition& callTemp)
      : LBinaryMath(classOpcode)
    {
        setOperand(0, lhs);
        setOperand(1, rhs);
        setTemp(0, temp1);
        setTemp(1, temp2);
        setTemp(2, temp3);
        setTemp(3, callTemp);
    }

    const LDefinition* callTemp() {
        return getTemp(3);
    }

    MMod* mir() const {
        return mir_->toMod();
    }
=======
class LUDivConstantI : public LInstructionHelper<1, 1, 1> {
  const int32_t denominator_;

 public:
  LIR_HEADER(UDivConstantI)

  LUDivConstantI(const LAllocation& lhs, int32_t denominator,
                 const LDefinition& temp)
      : LInstructionHelper(classOpcode), denominator_(denominator) {
    setOperand(0, lhs);
    setTemp(0, temp);
  }

  const LAllocation* numerator() { return getOperand(0); }
  const LDefinition* temp() { return getTemp(0); }
  int32_t denominator() const { return denominator_; }
  MDiv* mir() const { return mir_->toDiv(); }
>>>>>>> upstream-releases
};

<<<<<<< HEAD
class LModPowTwoI : public LInstructionHelper<1, 1, 0> {
  const int32_t shift_;

 public:
  LIR_HEADER(ModPowTwoI);
  int32_t shift() { return shift_; }

  LModPowTwoI(const LAllocation& lhs, int32_t shift)
      : LInstructionHelper(classOpcode), shift_(shift) {
    setOperand(0, lhs);
  }

  MMod* mir() const { return mir_->toMod(); }
||||||| merged common ancestors
class LModPowTwoI : public LInstructionHelper<1, 1, 0>
{
    const int32_t shift_;

  public:
    LIR_HEADER(ModPowTwoI);
    int32_t shift()
    {
        return shift_;
    }

    LModPowTwoI(const LAllocation& lhs, int32_t shift)
      : LInstructionHelper(classOpcode),
        shift_(shift)
    {
        setOperand(0, lhs);
    }

    MMod* mir() const {
        return mir_->toMod();
    }
=======
class LModI : public LBinaryMath<1> {
 public:
  LIR_HEADER(ModI);

  LModI(const LAllocation& lhs, const LAllocation& rhs,
        const LDefinition& callTemp)
      : LBinaryMath(classOpcode) {
    setOperand(0, lhs);
    setOperand(1, rhs);
    setTemp(0, callTemp);
  }

  const LDefinition* callTemp() { return getTemp(0); }

  MMod* mir() const { return mir_->toMod(); }
>>>>>>> upstream-releases
};

<<<<<<< HEAD
class LModMaskI : public LInstructionHelper<1, 1, 1> {
  const int32_t shift_;
||||||| merged common ancestors
class LModMaskI : public LInstructionHelper<1, 1, 1>
{
    const int32_t shift_;
=======
class LModPowTwoI : public LInstructionHelper<1, 1, 0> {
  const int32_t shift_;
>>>>>>> upstream-releases

<<<<<<< HEAD
 public:
  LIR_HEADER(ModMaskI);
||||||| merged common ancestors
  public:
    LIR_HEADER(ModMaskI);
=======
 public:
  LIR_HEADER(ModPowTwoI);
  int32_t shift() { return shift_; }
>>>>>>> upstream-releases

<<<<<<< HEAD
  LModMaskI(const LAllocation& lhs, const LDefinition& temp1, int32_t shift)
      : LInstructionHelper(classOpcode), shift_(shift) {
    setOperand(0, lhs);
    setTemp(0, temp1);
  }

  int32_t shift() const { return shift_; }

  MMod* mir() const { return mir_->toMod(); }
||||||| merged common ancestors
    LModMaskI(const LAllocation& lhs, const LDefinition& temp1, int32_t shift)
      : LInstructionHelper(classOpcode),
        shift_(shift)
    {
        setOperand(0, lhs);
        setTemp(0, temp1);
    }

    int32_t shift() const {
        return shift_;
    }

    MMod* mir() const {
        return mir_->toMod();
    }
=======
  LModPowTwoI(const LAllocation& lhs, int32_t shift)
      : LInstructionHelper(classOpcode), shift_(shift) {
    setOperand(0, lhs);
  }

  MMod* mir() const { return mir_->toMod(); }
};

class LModMaskI : public LInstructionHelper<1, 1, 2> {
  const int32_t shift_;

 public:
  LIR_HEADER(ModMaskI);

  LModMaskI(const LAllocation& lhs, const LDefinition& temp1,
            const LDefinition& temp2, int32_t shift)
      : LInstructionHelper(classOpcode), shift_(shift) {
    setOperand(0, lhs);
    setTemp(0, temp1);
    setTemp(1, temp2);
  }

  int32_t shift() const { return shift_; }

  MMod* mir() const { return mir_->toMod(); }
>>>>>>> upstream-releases
};

// Takes a tableswitch with an integer to decide
<<<<<<< HEAD
class LTableSwitch : public LInstructionHelper<0, 1, 1> {
 public:
  LIR_HEADER(TableSwitch);

  LTableSwitch(const LAllocation& in, const LDefinition& inputCopy,
               MTableSwitch* ins)
      : LInstructionHelper(classOpcode) {
    setOperand(0, in);
    setTemp(0, inputCopy);
    setMir(ins);
  }

  MTableSwitch* mir() const { return mir_->toTableSwitch(); }

  const LAllocation* index() { return getOperand(0); }
  const LDefinition* tempInt() { return getTemp(0); }
  // This is added to share the same CodeGenerator prefixes.
  const LDefinition* tempPointer() { return nullptr; }
||||||| merged common ancestors
class LTableSwitch : public LInstructionHelper<0, 1, 1>
{
  public:
    LIR_HEADER(TableSwitch);

    LTableSwitch(const LAllocation& in, const LDefinition& inputCopy, MTableSwitch* ins)
      : LInstructionHelper(classOpcode)
    {
        setOperand(0, in);
        setTemp(0, inputCopy);
        setMir(ins);
    }

    MTableSwitch* mir() const {
        return mir_->toTableSwitch();
    }

    const LAllocation* index() {
        return getOperand(0);
    }
    const LDefinition* tempInt() {
        return getTemp(0);
    }
    // This is added to share the same CodeGenerator prefixes.
    const LDefinition* tempPointer() {
        return nullptr;
    }
=======
class LTableSwitch : public LInstructionHelper<0, 1, 2> {
 public:
  LIR_HEADER(TableSwitch);

  LTableSwitch(const LAllocation& in, const LDefinition& inputCopy,
               const LDefinition& jumpTablePointer, MTableSwitch* ins)
      : LInstructionHelper(classOpcode) {
    setOperand(0, in);
    setTemp(0, inputCopy);
    setTemp(1, jumpTablePointer);
    setMir(ins);
  }

  MTableSwitch* mir() const { return mir_->toTableSwitch(); }

  const LAllocation* index() { return getOperand(0); }
  const LDefinition* tempInt() { return getTemp(0); }
  // This is added to share the same CodeGenerator prefixes.
  const LDefinition* tempPointer() { return getTemp(1); }
>>>>>>> upstream-releases
};

// Takes a tableswitch with an integer to decide
<<<<<<< HEAD
class LTableSwitchV : public LInstructionHelper<0, BOX_PIECES, 2> {
 public:
  LIR_HEADER(TableSwitchV);

  LTableSwitchV(const LBoxAllocation& input, const LDefinition& inputCopy,
                const LDefinition& floatCopy, MTableSwitch* ins)
      : LInstructionHelper(classOpcode) {
    setBoxOperand(InputValue, input);
    setTemp(0, inputCopy);
    setTemp(1, floatCopy);
    setMir(ins);
  }

  MTableSwitch* mir() const { return mir_->toTableSwitch(); }

  static const size_t InputValue = 0;

  const LDefinition* tempInt() { return getTemp(0); }
  const LDefinition* tempFloat() { return getTemp(1); }
  const LDefinition* tempPointer() { return nullptr; }
||||||| merged common ancestors
class LTableSwitchV : public LInstructionHelper<0, BOX_PIECES, 2>
{
  public:
    LIR_HEADER(TableSwitchV);

    LTableSwitchV(const LBoxAllocation& input, const LDefinition& inputCopy,
                  const LDefinition& floatCopy, MTableSwitch* ins)
      : LInstructionHelper(classOpcode)
    {
        setBoxOperand(InputValue, input);
        setTemp(0, inputCopy);
        setTemp(1, floatCopy);
        setMir(ins);
    }

    MTableSwitch* mir() const {
        return mir_->toTableSwitch();
    }

    static const size_t InputValue = 0;

    const LDefinition* tempInt() {
        return getTemp(0);
    }
    const LDefinition* tempFloat() {
        return getTemp(1);
    }
    const LDefinition* tempPointer() {
        return nullptr;
    }
=======
class LTableSwitchV : public LInstructionHelper<0, BOX_PIECES, 3> {
 public:
  LIR_HEADER(TableSwitchV);

  LTableSwitchV(const LBoxAllocation& input, const LDefinition& inputCopy,
                const LDefinition& floatCopy,
                const LDefinition& jumpTablePointer, MTableSwitch* ins)
      : LInstructionHelper(classOpcode) {
    setBoxOperand(InputValue, input);
    setTemp(0, inputCopy);
    setTemp(1, floatCopy);
    setTemp(2, jumpTablePointer);
    setMir(ins);
  }

  MTableSwitch* mir() const { return mir_->toTableSwitch(); }

  static const size_t InputValue = 0;

  const LDefinition* tempInt() { return getTemp(0); }
  const LDefinition* tempFloat() { return getTemp(1); }
  const LDefinition* tempPointer() { return getTemp(2); }
>>>>>>> upstream-releases
};

class LMulI : public LBinaryMath<0> {
 public:
  LIR_HEADER(MulI);

  LMulI() : LBinaryMath(classOpcode) {}

  MMul* mir() { return mir_->toMul(); }
};

<<<<<<< HEAD
class LUDiv : public LBinaryMath<0> {
 public:
  LIR_HEADER(UDiv);
||||||| merged common ancestors
class LUDiv : public LBinaryMath<0>
{
  public:
    LIR_HEADER(UDiv);
=======
class LUDiv : public LBinaryMath<1> {
 public:
  LIR_HEADER(UDiv);
>>>>>>> upstream-releases

<<<<<<< HEAD
  MDiv* mir() { return mir_->toDiv(); }
};
||||||| merged common ancestors
    MDiv* mir() {
        return mir_->toDiv();
    }
};
=======
  LUDiv(const LAllocation& lhs, const LAllocation& rhs,
        const LDefinition& remainder)
      : LBinaryMath(classOpcode) {
    setOperand(0, lhs);
    setOperand(1, rhs);
    setTemp(0, remainder);
  }
>>>>>>> upstream-releases

<<<<<<< HEAD
class LUMod : public LBinaryMath<0> {
 public:
  LIR_HEADER(UMod);
||||||| merged common ancestors
class LUMod : public LBinaryMath<0>
{
  public:
    LIR_HEADER(UMod);
=======
  const LDefinition* remainder() { return getTemp(0); }
>>>>>>> upstream-releases

<<<<<<< HEAD
  MMod* mir() { return mir_->toMod(); }
||||||| merged common ancestors
    MMod* mir() {
        return mir_->toMod();
    }
=======
  MDiv* mir() { return mir_->toDiv(); }
>>>>>>> upstream-releases
};

<<<<<<< HEAD
// This class performs a simple x86 'div', yielding either a quotient or
// remainder depending on whether this instruction is defined to output eax
// (quotient) or edx (remainder).
class LSoftUDivOrMod : public LBinaryMath<3> {
 public:
  LIR_HEADER(SoftUDivOrMod);

  LSoftUDivOrMod(const LAllocation& lhs, const LAllocation& rhs,
                 const LDefinition& temp1, const LDefinition& temp2,
                 const LDefinition& temp3)
      : LBinaryMath(classOpcode) {
    setOperand(0, lhs);
    setOperand(1, rhs);
    setTemp(0, temp1);
    setTemp(1, temp2);
    setTemp(2, temp3);
  }
||||||| merged common ancestors
// This class performs a simple x86 'div', yielding either a quotient or remainder depending on
// whether this instruction is defined to output eax (quotient) or edx (remainder).
class LSoftUDivOrMod : public LBinaryMath<3>
{
  public:
    LIR_HEADER(SoftUDivOrMod);

    LSoftUDivOrMod(const LAllocation& lhs, const LAllocation& rhs, const LDefinition& temp1,
                   const LDefinition& temp2, const LDefinition& temp3)
      : LBinaryMath(classOpcode)
    {
        setOperand(0, lhs);
        setOperand(1, rhs);
        setTemp(0, temp1);
        setTemp(1, temp2);
        setTemp(2, temp3);
    }
=======
class LUMod : public LBinaryMath<0> {
 public:
  LIR_HEADER(UMod);

  LUMod(const LAllocation& lhs, const LAllocation& rhs)
      : LBinaryMath(classOpcode) {
    setOperand(0, lhs);
    setOperand(1, rhs);
  }

  MMod* mir() { return mir_->toMod(); }
>>>>>>> upstream-releases
};

}  // namespace jit
}  // namespace js

#endif /* jit_arm64_LIR_arm64_h */
