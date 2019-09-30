/* -*- Mode: C++; tab-width: 8; indent-tabs-mode: nil; c-basic-offset: 2 -*-
 * vim: set ts=8 sts=2 et sw=2 tw=80:
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/. */

#include "frontend/TryEmitter.h"

#include "mozilla/Assertions.h"  // MOZ_ASSERT

#include "frontend/BytecodeEmitter.h"  // BytecodeEmitter
#include "frontend/SharedContext.h"    // StatementKind
#include "frontend/SourceNotes.h"      // SrcNote, SRC_*
#include "vm/JSScript.h"               // JSTRY_CATCH, JSTRY_FINALLY
#include "vm/Opcodes.h"                // JSOP_*

using namespace js;
using namespace js::frontend;

using mozilla::Maybe;

TryEmitter::TryEmitter(BytecodeEmitter* bce, Kind kind, ControlKind controlKind)
    : bce_(bce),
      kind_(kind),
      controlKind_(controlKind),
      depth_(0),
      noteIndex_(0),
      tryStart_(0)
#ifdef DEBUG
      ,
      state_(State::Start)
#endif
{
<<<<<<< HEAD
  if (controlKind_ == ControlKind::Syntactic) {
    controlInfo_.emplace(
        bce_, hasFinally() ? StatementKind::Finally : StatementKind::Try);
  }
  finallyStart_.offset = 0;
||||||| merged common ancestors
    if (controlKind_ == ControlKind::Syntactic) {
        controlInfo_.emplace(bce_, hasFinally() ? StatementKind::Finally : StatementKind::Try);
    }
    finallyStart_.offset = 0;
=======
  if (controlKind_ == ControlKind::Syntactic) {
    controlInfo_.emplace(
        bce_, hasFinally() ? StatementKind::Finally : StatementKind::Try);
  }
>>>>>>> upstream-releases
}

// Emits JSOP_GOTO to the end of try-catch-finally.
// Used in `yield*`.
bool TryEmitter::emitJumpOverCatchAndFinally() {
  if (!bce_->emitJump(JSOP_GOTO, &catchAndFinallyJump_)) {
    return false;
  }
  return true;
}

<<<<<<< HEAD
bool TryEmitter::emitTry() {
  MOZ_ASSERT(state_ == State::Start);

  // Since an exception can be thrown at any place inside the try block,
  // we need to restore the stack and the scope chain before we transfer
  // the control to the exception handler.
  //
  // For that we store in a try note associated with the catch or
  // finally block the stack depth upon the try entry. The interpreter
  // uses this depth to properly unwind the stack and the scope chain.
  depth_ = bce_->stackDepth;

  // Record the try location, then emit the try block.
  if (!bce_->newSrcNote(SRC_TRY, &noteIndex_)) {
    return false;
  }
  if (!bce_->emit1(JSOP_TRY)) {
    return false;
  }
  tryStart_ = bce_->offset();
||||||| merged common ancestors
bool
TryEmitter::emitTry()
{
    MOZ_ASSERT(state_ == State::Start);

    // Since an exception can be thrown at any place inside the try block,
    // we need to restore the stack and the scope chain before we transfer
    // the control to the exception handler.
    //
    // For that we store in a try note associated with the catch or
    // finally block the stack depth upon the try entry. The interpreter
    // uses this depth to properly unwind the stack and the scope chain.
    depth_ = bce_->stackDepth;

    // Record the try location, then emit the try block.
    if (!bce_->newSrcNote(SRC_TRY, &noteIndex_)) {
        return false;
    }
    if (!bce_->emit1(JSOP_TRY)) {
        return false;
    }
    tryStart_ = bce_->offset();
=======
bool TryEmitter::emitTry() {
  MOZ_ASSERT(state_ == State::Start);

  // Since an exception can be thrown at any place inside the try block,
  // we need to restore the stack and the scope chain before we transfer
  // the control to the exception handler.
  //
  // For that we store in a try note associated with the catch or
  // finally block the stack depth upon the try entry. The interpreter
  // uses this depth to properly unwind the stack and the scope chain.
  depth_ = bce_->bytecodeSection().stackDepth();

  // Record the try location, then emit the try block.
  if (!bce_->newSrcNote(SRC_TRY, &noteIndex_)) {
    return false;
  }
  if (!bce_->emit1(JSOP_TRY)) {
    return false;
  }
  tryStart_ = bce_->bytecodeSection().offset();
>>>>>>> upstream-releases

#ifdef DEBUG
  state_ = State::Try;
#endif
  return true;
}

<<<<<<< HEAD
bool TryEmitter::emitTryEnd() {
  MOZ_ASSERT(state_ == State::Try);
  MOZ_ASSERT(depth_ == bce_->stackDepth);
||||||| merged common ancestors
bool
TryEmitter::emitTryEnd()
{
    MOZ_ASSERT(state_ == State::Try);
    MOZ_ASSERT(depth_ == bce_->stackDepth);
=======
bool TryEmitter::emitTryEnd() {
  MOZ_ASSERT(state_ == State::Try);
  MOZ_ASSERT(depth_ == bce_->bytecodeSection().stackDepth());
>>>>>>> upstream-releases

  // GOSUB to finally, if present.
  if (hasFinally() && controlInfo_) {
    if (!bce_->emitGoSub(&controlInfo_->gosubs)) {
      return false;
    }
  }

<<<<<<< HEAD
  // Source note points to the jump at the end of the try block.
  if (!bce_->setSrcNoteOffset(noteIndex_, SrcNote::Try::EndOfTryJumpOffset,
                              bce_->offset() - tryStart_ + JSOP_TRY_LENGTH)) {
    return false;
  }
||||||| merged common ancestors
    // Source note points to the jump at the end of the try block.
    if (!bce_->setSrcNoteOffset(noteIndex_, SrcNote::Try::EndOfTryJumpOffset,
                                bce_->offset() - tryStart_ + JSOP_TRY_LENGTH))
    {
        return false;
    }
=======
  // Source note points to the jump at the end of the try block.
  if (!bce_->setSrcNoteOffset(noteIndex_, SrcNote::Try::EndOfTryJumpOffset,
                              bce_->bytecodeSection().offset() - tryStart_ +
                                  BytecodeOffsetDiff(JSOP_TRY_LENGTH))) {
    return false;
  }
>>>>>>> upstream-releases

  // Emit jump over catch and/or finally.
  if (!bce_->emitJump(JSOP_GOTO, &catchAndFinallyJump_)) {
    return false;
  }

  if (!bce_->emitJumpTarget(&tryEnd_)) {
    return false;
  }

  return true;
}

<<<<<<< HEAD
bool TryEmitter::emitCatch() {
  MOZ_ASSERT(state_ == State::Try);
  if (!emitTryEnd()) {
    return false;
  }
||||||| merged common ancestors
bool
TryEmitter::emitCatch()
{
    MOZ_ASSERT(state_ == State::Try);
    if (!emitTryEnd()) {
        return false;
    }
=======
bool TryEmitter::emitCatch() {
  MOZ_ASSERT(state_ == State::Try);
  if (!emitTryEnd()) {
    return false;
  }

  MOZ_ASSERT(bce_->bytecodeSection().stackDepth() == depth_);
>>>>>>> upstream-releases

<<<<<<< HEAD
  MOZ_ASSERT(bce_->stackDepth == depth_);

  if (controlKind_ == ControlKind::Syntactic) {
    // Clear the frame's return value that might have been set by the
    // try block:
    //
    //   eval("try { 1; throw 2 } catch(e) {}"); // undefined, not 1
    if (!bce_->emit1(JSOP_UNDEFINED)) {
      return false;
    }
    if (!bce_->emit1(JSOP_SETRVAL)) {
      return false;
||||||| merged common ancestors
    MOZ_ASSERT(bce_->stackDepth == depth_);

    if (controlKind_ == ControlKind::Syntactic) {
        // Clear the frame's return value that might have been set by the
        // try block:
        //
        //   eval("try { 1; throw 2 } catch(e) {}"); // undefined, not 1
        if (!bce_->emit1(JSOP_UNDEFINED)) {
            return false;
        }
        if (!bce_->emit1(JSOP_SETRVAL)) {
            return false;
        }
=======
  if (controlKind_ == ControlKind::Syntactic) {
    // Clear the frame's return value that might have been set by the
    // try block:
    //
    //   eval("try { 1; throw 2 } catch(e) {}"); // undefined, not 1
    if (!bce_->emit1(JSOP_UNDEFINED)) {
      return false;
    }
    if (!bce_->emit1(JSOP_SETRVAL)) {
      return false;
>>>>>>> upstream-releases
    }
  }

#ifdef DEBUG
  state_ = State::Catch;
#endif
  return true;
}

<<<<<<< HEAD
bool TryEmitter::emitCatchEnd() {
  MOZ_ASSERT(state_ == State::Catch);

  if (!controlInfo_) {
    return true;
  }
||||||| merged common ancestors
bool
TryEmitter::emitCatchEnd()
{
    MOZ_ASSERT(state_ == State::Catch);
=======
bool TryEmitter::emitCatchEnd() {
  MOZ_ASSERT(state_ == State::Catch);
>>>>>>> upstream-releases

<<<<<<< HEAD
  // gosub <finally>, if required.
  if (hasFinally()) {
    if (!bce_->emitGoSub(&controlInfo_->gosubs)) {
      return false;
||||||| merged common ancestors
    if (!controlInfo_) {
        return true;
=======
  if (!controlInfo_) {
    return true;
  }

  // gosub <finally>, if required.
  if (hasFinally()) {
    if (!bce_->emitGoSub(&controlInfo_->gosubs)) {
      return false;
>>>>>>> upstream-releases
    }
<<<<<<< HEAD
    MOZ_ASSERT(bce_->stackDepth == depth_);
||||||| merged common ancestors
=======
    MOZ_ASSERT(bce_->bytecodeSection().stackDepth() == depth_);
>>>>>>> upstream-releases

    // Jump over the finally block.
    if (!bce_->emitJump(JSOP_GOTO, &catchAndFinallyJump_)) {
      return false;
    }
  }

  return true;
}

bool TryEmitter::emitFinally(
    const Maybe<uint32_t>& finallyPos /* = Nothing() */) {
  // If we are using controlInfo_ (i.e., emitting a syntactic try
  // blocks), we must have specified up front if there will be a finally
  // close. For internal non-syntactic try blocks, like those emitted for
  // yield* and IteratorClose inside for-of loops, we can emitFinally even
  // without specifying up front, since the internal non-syntactic try
  // blocks emit no GOSUBs.
  if (!controlInfo_) {
    if (kind_ == Kind::TryCatch) {
      kind_ = Kind::TryCatchFinally;
    }
  } else {
    MOZ_ASSERT(hasFinally());
  }

<<<<<<< HEAD
  if (!hasCatch()) {
    MOZ_ASSERT(state_ == State::Try);
    if (!emitTryEnd()) {
      return false;
    }
  } else {
    MOZ_ASSERT(state_ == State::Catch);
    if (!emitCatchEnd()) {
      return false;
||||||| merged common ancestors
    if (!hasCatch()) {
        MOZ_ASSERT(state_ == State::Try);
        if (!emitTryEnd()) {
            return false;
        }
    } else {
        MOZ_ASSERT(state_ == State::Catch);
        if (!emitCatchEnd()) {
            return false;
        }
=======
  if (!hasCatch()) {
    MOZ_ASSERT(state_ == State::Try);
    if (!emitTryEnd()) {
      return false;
>>>>>>> upstream-releases
    }
<<<<<<< HEAD
  }
||||||| merged common ancestors
=======
  } else {
    MOZ_ASSERT(state_ == State::Catch);
    if (!emitCatchEnd()) {
      return false;
    }
  }
>>>>>>> upstream-releases

<<<<<<< HEAD
  MOZ_ASSERT(bce_->stackDepth == depth_);
||||||| merged common ancestors
    MOZ_ASSERT(bce_->stackDepth == depth_);
=======
  MOZ_ASSERT(bce_->bytecodeSection().stackDepth() == depth_);
>>>>>>> upstream-releases

  if (!bce_->emitJumpTarget(&finallyStart_)) {
    return false;
  }

  if (controlInfo_) {
    // Fix up the gosubs that might have been emitted before non-local
    // jumps to the finally code.
    bce_->patchJumpsToTarget(controlInfo_->gosubs, finallyStart_);

<<<<<<< HEAD
    // Indicate that we're emitting a subroutine body.
    controlInfo_->setEmittingSubroutine();
  }
  if (finallyPos) {
    if (!bce_->updateSourceCoordNotes(finallyPos.value())) {
      return false;
    }
  }
  if (!bce_->emit1(JSOP_FINALLY)) {
    return false;
  }

  if (controlKind_ == ControlKind::Syntactic) {
    if (!bce_->emit1(JSOP_GETRVAL)) {
      return false;
||||||| merged common ancestors
        // Indicate that we're emitting a subroutine body.
        controlInfo_->setEmittingSubroutine();
    }
    if (finallyPos) {
        if (!bce_->updateSourceCoordNotes(finallyPos.value())) {
            return false;
        }
    }
    if (!bce_->emit1(JSOP_FINALLY)) {
        return false;
=======
    // Indicate that we're emitting a subroutine body.
    controlInfo_->setEmittingSubroutine();
  }
  if (finallyPos) {
    if (!bce_->updateSourceCoordNotes(finallyPos.value())) {
      return false;
    }
  }
  if (!bce_->emit1(JSOP_FINALLY)) {
    return false;
  }

  if (controlKind_ == ControlKind::Syntactic) {
    if (!bce_->emit1(JSOP_GETRVAL)) {
      return false;
>>>>>>> upstream-releases
    }

<<<<<<< HEAD
    // Clear the frame's return value to make break/continue return
    // correct value even if there's no other statement before them:
    //
    //   eval("x: try { 1 } finally { break x; }"); // undefined, not 1
    if (!bce_->emit1(JSOP_UNDEFINED)) {
      return false;
||||||| merged common ancestors
    if (controlKind_ == ControlKind::Syntactic) {
        if (!bce_->emit1(JSOP_GETRVAL)) {
            return false;
        }

        // Clear the frame's return value to make break/continue return
        // correct value even if there's no other statement before them:
        //
        //   eval("x: try { 1 } finally { break x; }"); // undefined, not 1
        if (!bce_->emit1(JSOP_UNDEFINED)) {
            return false;
        }
        if (!bce_->emit1(JSOP_SETRVAL)) {
            return false;
        }
=======
    // Clear the frame's return value to make break/continue return
    // correct value even if there's no other statement before them:
    //
    //   eval("x: try { 1 } finally { break x; }"); // undefined, not 1
    if (!bce_->emit1(JSOP_UNDEFINED)) {
      return false;
    }
    if (!bce_->emit1(JSOP_SETRVAL)) {
      return false;
>>>>>>> upstream-releases
    }
<<<<<<< HEAD
    if (!bce_->emit1(JSOP_SETRVAL)) {
      return false;
    }
  }
||||||| merged common ancestors
=======
  }
>>>>>>> upstream-releases

#ifdef DEBUG
  state_ = State::Finally;
#endif
  return true;
}

bool TryEmitter::emitFinallyEnd() {
  MOZ_ASSERT(state_ == State::Finally);

  if (controlKind_ == ControlKind::Syntactic) {
    if (!bce_->emit1(JSOP_SETRVAL)) {
      return false;
    }
  }

  if (!bce_->emit1(JSOP_RETSUB)) {
    return false;
  }

  bce_->hasTryFinally = true;
  return true;
}

bool TryEmitter::emitEnd() {
  if (!hasFinally()) {
    MOZ_ASSERT(state_ == State::Catch);
    if (!emitCatchEnd()) {
      return false;
    }
  } else {
    MOZ_ASSERT(state_ == State::Finally);
    if (!emitFinallyEnd()) {
      return false;
    }
<<<<<<< HEAD
  }

  MOZ_ASSERT(bce_->stackDepth == depth_);

  // ReconstructPCStack needs a NOP here to mark the end of the last
  // catch block.
  if (!bce_->emit1(JSOP_NOP)) {
    return false;
  }

  // Fix up the end-of-try/catch jumps to come here.
  if (!bce_->emitJumpTargetAndPatch(catchAndFinallyJump_)) {
    return false;
  }

  // Add the try note last, to let post-order give us the right ordering
  // (first to last for a given nesting level, inner to outer by level).
  if (hasCatch()) {
    if (!bce_->addTryNote(JSTRY_CATCH, depth_, tryStart_, tryEnd_.offset)) {
      return false;
||||||| merged common ancestors

    // Add the try note last, to let post-order give us the right ordering
    // (first to last for a given nesting level, inner to outer by level).
    if (hasCatch()) {
        if (!bce_->tryNoteList.append(JSTRY_CATCH, depth_, tryStart_, tryEnd_.offset)) {
            return false;
        }
=======
  }

  MOZ_ASSERT(bce_->bytecodeSection().stackDepth() == depth_);

  // ReconstructPCStack needs a NOP here to mark the end of the last
  // catch block.
  if (!bce_->emit1(JSOP_NOP)) {
    return false;
  }

  // Fix up the end-of-try/catch jumps to come here.
  if (!bce_->emitJumpTargetAndPatch(catchAndFinallyJump_)) {
    return false;
  }

  // Add the try note last, to let post-order give us the right ordering
  // (first to last for a given nesting level, inner to outer by level).
  if (hasCatch()) {
    if (!bce_->addTryNote(JSTRY_CATCH, depth_, tryStart_, tryEnd_.offset)) {
      return false;
>>>>>>> upstream-releases
    }
  }

  // If we've got a finally, mark try+catch region with additional
  // trynote to catch exceptions (re)thrown from a catch block or
  // for the try{}finally{} case.
  if (hasFinally()) {
    if (!bce_->addTryNote(JSTRY_FINALLY, depth_, tryStart_,
                          finallyStart_.offset)) {
      return false;
    }
  }

#ifdef DEBUG
  state_ = State::End;
#endif
  return true;
}
