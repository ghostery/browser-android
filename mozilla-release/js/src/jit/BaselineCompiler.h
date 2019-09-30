/* -*- Mode: C++; tab-width: 8; indent-tabs-mode: nil; c-basic-offset: 2 -*-
 * vim: set ts=8 sts=2 et sw=2 tw=80:
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/. */

#ifndef jit_BaselineCompiler_h
#define jit_BaselineCompiler_h

#include "jit/BaselineFrameInfo.h"
#include "jit/BaselineIC.h"
#include "jit/BytecodeAnalysis.h"
#include "jit/FixedList.h"
#include "jit/MacroAssembler.h"

namespace js {

enum class GeneratorResumeKind;

namespace jit {

<<<<<<< HEAD
#define OPCODE_LIST(_)                \
  _(JSOP_NOP)                         \
  _(JSOP_NOP_DESTRUCTURING)           \
  _(JSOP_LABEL)                       \
  _(JSOP_ITERNEXT)                    \
  _(JSOP_POP)                         \
  _(JSOP_POPN)                        \
  _(JSOP_DUPAT)                       \
  _(JSOP_ENTERWITH)                   \
  _(JSOP_LEAVEWITH)                   \
  _(JSOP_DUP)                         \
  _(JSOP_DUP2)                        \
  _(JSOP_SWAP)                        \
  _(JSOP_PICK)                        \
  _(JSOP_UNPICK)                      \
  _(JSOP_GOTO)                        \
  _(JSOP_IFEQ)                        \
  _(JSOP_IFNE)                        \
  _(JSOP_AND)                         \
  _(JSOP_OR)                          \
  _(JSOP_NOT)                         \
  _(JSOP_POS)                         \
  _(JSOP_LOOPHEAD)                    \
  _(JSOP_LOOPENTRY)                   \
  _(JSOP_VOID)                        \
  _(JSOP_UNDEFINED)                   \
  _(JSOP_HOLE)                        \
  _(JSOP_NULL)                        \
  _(JSOP_TRUE)                        \
  _(JSOP_FALSE)                       \
  _(JSOP_ZERO)                        \
  _(JSOP_ONE)                         \
  _(JSOP_INT8)                        \
  _(JSOP_INT32)                       \
  _(JSOP_UINT16)                      \
  _(JSOP_UINT24)                      \
  _(JSOP_RESUMEINDEX)                 \
  _(JSOP_DOUBLE)                      \
  IF_BIGINT(_(JSOP_BIGINT), )         \
  _(JSOP_STRING)                      \
  _(JSOP_SYMBOL)                      \
  _(JSOP_OBJECT)                      \
  _(JSOP_CALLSITEOBJ)                 \
  _(JSOP_REGEXP)                      \
  _(JSOP_LAMBDA)                      \
  _(JSOP_LAMBDA_ARROW)                \
  _(JSOP_SETFUNNAME)                  \
  _(JSOP_BITOR)                       \
  _(JSOP_BITXOR)                      \
  _(JSOP_BITAND)                      \
  _(JSOP_LSH)                         \
  _(JSOP_RSH)                         \
  _(JSOP_URSH)                        \
  _(JSOP_ADD)                         \
  _(JSOP_SUB)                         \
  _(JSOP_MUL)                         \
  _(JSOP_DIV)                         \
  _(JSOP_MOD)                         \
  _(JSOP_POW)                         \
  _(JSOP_LT)                          \
  _(JSOP_LE)                          \
  _(JSOP_GT)                          \
  _(JSOP_GE)                          \
  _(JSOP_EQ)                          \
  _(JSOP_NE)                          \
  _(JSOP_STRICTEQ)                    \
  _(JSOP_STRICTNE)                    \
  _(JSOP_CONDSWITCH)                  \
  _(JSOP_CASE)                        \
  _(JSOP_DEFAULT)                     \
  _(JSOP_LINENO)                      \
  _(JSOP_BITNOT)                      \
  _(JSOP_NEG)                         \
  _(JSOP_NEWARRAY)                    \
  _(JSOP_NEWARRAY_COPYONWRITE)        \
  _(JSOP_INITELEM_ARRAY)              \
  _(JSOP_NEWOBJECT)                   \
  _(JSOP_NEWINIT)                     \
  _(JSOP_INITELEM)                    \
  _(JSOP_INITELEM_GETTER)             \
  _(JSOP_INITELEM_SETTER)             \
  _(JSOP_INITELEM_INC)                \
  _(JSOP_MUTATEPROTO)                 \
  _(JSOP_INITPROP)                    \
  _(JSOP_INITLOCKEDPROP)              \
  _(JSOP_INITHIDDENPROP)              \
  _(JSOP_INITPROP_GETTER)             \
  _(JSOP_INITPROP_SETTER)             \
  _(JSOP_GETELEM)                     \
  _(JSOP_SETELEM)                     \
  _(JSOP_STRICTSETELEM)               \
  _(JSOP_CALLELEM)                    \
  _(JSOP_DELELEM)                     \
  _(JSOP_STRICTDELELEM)               \
  _(JSOP_GETELEM_SUPER)               \
  _(JSOP_SETELEM_SUPER)               \
  _(JSOP_STRICTSETELEM_SUPER)         \
  _(JSOP_IN)                          \
  _(JSOP_HASOWN)                      \
  _(JSOP_GETGNAME)                    \
  _(JSOP_BINDGNAME)                   \
  _(JSOP_SETGNAME)                    \
  _(JSOP_STRICTSETGNAME)              \
  _(JSOP_SETNAME)                     \
  _(JSOP_STRICTSETNAME)               \
  _(JSOP_GETPROP)                     \
  _(JSOP_SETPROP)                     \
  _(JSOP_STRICTSETPROP)               \
  _(JSOP_CALLPROP)                    \
  _(JSOP_DELPROP)                     \
  _(JSOP_STRICTDELPROP)               \
  _(JSOP_GETPROP_SUPER)               \
  _(JSOP_SETPROP_SUPER)               \
  _(JSOP_STRICTSETPROP_SUPER)         \
  _(JSOP_LENGTH)                      \
  _(JSOP_GETBOUNDNAME)                \
  _(JSOP_GETALIASEDVAR)               \
  _(JSOP_SETALIASEDVAR)               \
  _(JSOP_GETNAME)                     \
  _(JSOP_BINDNAME)                    \
  _(JSOP_DELNAME)                     \
  _(JSOP_GETIMPORT)                   \
  _(JSOP_GETINTRINSIC)                \
  _(JSOP_BINDVAR)                     \
  _(JSOP_DEFVAR)                      \
  _(JSOP_DEFCONST)                    \
  _(JSOP_DEFLET)                      \
  _(JSOP_DEFFUN)                      \
  _(JSOP_GETLOCAL)                    \
  _(JSOP_SETLOCAL)                    \
  _(JSOP_GETARG)                      \
  _(JSOP_SETARG)                      \
  _(JSOP_CHECKLEXICAL)                \
  _(JSOP_INITLEXICAL)                 \
  _(JSOP_INITGLEXICAL)                \
  _(JSOP_CHECKALIASEDLEXICAL)         \
  _(JSOP_INITALIASEDLEXICAL)          \
  _(JSOP_UNINITIALIZED)               \
  _(JSOP_CALL)                        \
  _(JSOP_CALL_IGNORES_RV)             \
  _(JSOP_CALLITER)                    \
  _(JSOP_FUNCALL)                     \
  _(JSOP_FUNAPPLY)                    \
  _(JSOP_NEW)                         \
  _(JSOP_EVAL)                        \
  _(JSOP_STRICTEVAL)                  \
  _(JSOP_SPREADCALL)                  \
  _(JSOP_SPREADNEW)                   \
  _(JSOP_SPREADEVAL)                  \
  _(JSOP_STRICTSPREADEVAL)            \
  _(JSOP_OPTIMIZE_SPREADCALL)         \
  _(JSOP_IMPLICITTHIS)                \
  _(JSOP_GIMPLICITTHIS)               \
  _(JSOP_INSTANCEOF)                  \
  _(JSOP_TYPEOF)                      \
  _(JSOP_TYPEOFEXPR)                  \
  _(JSOP_THROWMSG)                    \
  _(JSOP_THROW)                       \
  _(JSOP_TRY)                         \
  _(JSOP_FINALLY)                     \
  _(JSOP_GOSUB)                       \
  _(JSOP_RETSUB)                      \
  _(JSOP_PUSHLEXICALENV)              \
  _(JSOP_POPLEXICALENV)               \
  _(JSOP_FRESHENLEXICALENV)           \
  _(JSOP_RECREATELEXICALENV)          \
  _(JSOP_DEBUGLEAVELEXICALENV)        \
  _(JSOP_PUSHVARENV)                  \
  _(JSOP_POPVARENV)                   \
  _(JSOP_EXCEPTION)                   \
  _(JSOP_DEBUGGER)                    \
  _(JSOP_ARGUMENTS)                   \
  _(JSOP_RUNONCE)                     \
  _(JSOP_REST)                        \
  _(JSOP_TOASYNC)                     \
  _(JSOP_TOASYNCGEN)                  \
  _(JSOP_TOASYNCITER)                 \
  _(JSOP_TOID)                        \
  _(JSOP_TOSTRING)                    \
  _(JSOP_TABLESWITCH)                 \
  _(JSOP_ITER)                        \
  _(JSOP_MOREITER)                    \
  _(JSOP_ISNOITER)                    \
  _(JSOP_ENDITER)                     \
  _(JSOP_ISGENCLOSING)                \
  _(JSOP_GENERATOR)                   \
  _(JSOP_INITIALYIELD)                \
  _(JSOP_YIELD)                       \
  _(JSOP_AWAIT)                       \
  _(JSOP_TRYSKIPAWAIT)                \
  _(JSOP_DEBUGAFTERYIELD)             \
  _(JSOP_FINALYIELDRVAL)              \
  _(JSOP_RESUME)                      \
  _(JSOP_CALLEE)                      \
  _(JSOP_ENVCALLEE)                   \
  _(JSOP_SUPERBASE)                   \
  _(JSOP_SUPERFUN)                    \
  _(JSOP_GETRVAL)                     \
  _(JSOP_SETRVAL)                     \
  _(JSOP_RETRVAL)                     \
  _(JSOP_RETURN)                      \
  _(JSOP_FUNCTIONTHIS)                \
  _(JSOP_GLOBALTHIS)                  \
  _(JSOP_CHECKISOBJ)                  \
  _(JSOP_CHECKISCALLABLE)             \
  _(JSOP_CHECKTHIS)                   \
  _(JSOP_CHECKTHISREINIT)             \
  _(JSOP_CHECKRETURN)                 \
  _(JSOP_NEWTARGET)                   \
  _(JSOP_SUPERCALL)                   \
  _(JSOP_SPREADSUPERCALL)             \
  _(JSOP_THROWSETCONST)               \
  _(JSOP_THROWSETALIASEDCONST)        \
  _(JSOP_THROWSETCALLEE)              \
  _(JSOP_INITHIDDENPROP_GETTER)       \
  _(JSOP_INITHIDDENPROP_SETTER)       \
  _(JSOP_INITHIDDENELEM)              \
  _(JSOP_INITHIDDENELEM_GETTER)       \
  _(JSOP_INITHIDDENELEM_SETTER)       \
  _(JSOP_CHECKOBJCOERCIBLE)           \
  _(JSOP_DEBUGCHECKSELFHOSTED)        \
  _(JSOP_JUMPTARGET)                  \
  _(JSOP_IS_CONSTRUCTING)             \
  _(JSOP_TRY_DESTRUCTURING_ITERCLOSE) \
  _(JSOP_CHECKCLASSHERITAGE)          \
  _(JSOP_INITHOMEOBJECT)              \
  _(JSOP_BUILTINPROTO)                \
  _(JSOP_OBJWITHPROTO)                \
  _(JSOP_FUNWITHPROTO)                \
  _(JSOP_CLASSCONSTRUCTOR)            \
  _(JSOP_DERIVEDCONSTRUCTOR)          \
  _(JSOP_IMPORTMETA)                  \
  _(JSOP_DYNAMIC_IMPORT)

// Base class for BaselineCompiler and BaselineInterpreterGenerator. The Handler
// template is a class storing fields/methods that are interpreter or compiler
// specific. This can be combined with template specialization of methods in
// this class to specialize behavior.
template <typename Handler>
class BaselineCodeGen {
 protected:
  Handler handler;

  JSContext* cx;
  JSScript* script;
  jsbytecode* pc;
  StackMacroAssembler masm;
  bool ionCompileable_;

  TempAllocator& alloc_;
  BytecodeAnalysis analysis_;
  FrameInfo frame;

  js::Vector<RetAddrEntry, 16, SystemAllocPolicy> retAddrEntries_;
  js::Vector<CodeOffset> traceLoggerToggleOffsets_;

  NonAssertingLabel return_;
  NonAssertingLabel postBarrierSlot_;

  // Index of the current ICEntry in the script's ICScript.
  uint32_t icEntryIndex_;

  uint32_t pushedBeforeCall_;
||||||| merged common ancestors
#define OPCODE_LIST(_)         \
    _(JSOP_NOP)                \
    _(JSOP_NOP_DESTRUCTURING)  \
    _(JSOP_LABEL)              \
    _(JSOP_ITERNEXT)           \
    _(JSOP_POP)                \
    _(JSOP_POPN)               \
    _(JSOP_DUPAT)              \
    _(JSOP_ENTERWITH)          \
    _(JSOP_LEAVEWITH)          \
    _(JSOP_DUP)                \
    _(JSOP_DUP2)               \
    _(JSOP_SWAP)               \
    _(JSOP_PICK)               \
    _(JSOP_UNPICK)             \
    _(JSOP_GOTO)               \
    _(JSOP_IFEQ)               \
    _(JSOP_IFNE)               \
    _(JSOP_AND)                \
    _(JSOP_OR)                 \
    _(JSOP_NOT)                \
    _(JSOP_POS)                \
    _(JSOP_LOOPHEAD)           \
    _(JSOP_LOOPENTRY)          \
    _(JSOP_VOID)               \
    _(JSOP_UNDEFINED)          \
    _(JSOP_HOLE)               \
    _(JSOP_NULL)               \
    _(JSOP_TRUE)               \
    _(JSOP_FALSE)              \
    _(JSOP_ZERO)               \
    _(JSOP_ONE)                \
    _(JSOP_INT8)               \
    _(JSOP_INT32)              \
    _(JSOP_UINT16)             \
    _(JSOP_UINT24)             \
    _(JSOP_DOUBLE)             \
    _(JSOP_STRING)             \
    _(JSOP_SYMBOL)             \
    _(JSOP_OBJECT)             \
    _(JSOP_CALLSITEOBJ)        \
    _(JSOP_REGEXP)             \
    _(JSOP_LAMBDA)             \
    _(JSOP_LAMBDA_ARROW)       \
    _(JSOP_SETFUNNAME)         \
    _(JSOP_BITOR)              \
    _(JSOP_BITXOR)             \
    _(JSOP_BITAND)             \
    _(JSOP_LSH)                \
    _(JSOP_RSH)                \
    _(JSOP_URSH)               \
    _(JSOP_ADD)                \
    _(JSOP_SUB)                \
    _(JSOP_MUL)                \
    _(JSOP_DIV)                \
    _(JSOP_MOD)                \
    _(JSOP_POW)                \
    _(JSOP_LT)                 \
    _(JSOP_LE)                 \
    _(JSOP_GT)                 \
    _(JSOP_GE)                 \
    _(JSOP_EQ)                 \
    _(JSOP_NE)                 \
    _(JSOP_STRICTEQ)           \
    _(JSOP_STRICTNE)           \
    _(JSOP_CONDSWITCH)         \
    _(JSOP_CASE)               \
    _(JSOP_DEFAULT)            \
    _(JSOP_LINENO)             \
    _(JSOP_BITNOT)             \
    _(JSOP_NEG)                \
    _(JSOP_NEWARRAY)           \
    _(JSOP_NEWARRAY_COPYONWRITE) \
    _(JSOP_INITELEM_ARRAY)     \
    _(JSOP_NEWOBJECT)          \
    _(JSOP_NEWINIT)            \
    _(JSOP_INITELEM)           \
    _(JSOP_INITELEM_GETTER)    \
    _(JSOP_INITELEM_SETTER)    \
    _(JSOP_INITELEM_INC)       \
    _(JSOP_MUTATEPROTO)        \
    _(JSOP_INITPROP)           \
    _(JSOP_INITLOCKEDPROP)     \
    _(JSOP_INITHIDDENPROP)     \
    _(JSOP_INITPROP_GETTER)    \
    _(JSOP_INITPROP_SETTER)    \
    _(JSOP_GETELEM)            \
    _(JSOP_SETELEM)            \
    _(JSOP_STRICTSETELEM)      \
    _(JSOP_CALLELEM)           \
    _(JSOP_DELELEM)            \
    _(JSOP_STRICTDELELEM)      \
    _(JSOP_GETELEM_SUPER)      \
    _(JSOP_SETELEM_SUPER)      \
    _(JSOP_STRICTSETELEM_SUPER) \
    _(JSOP_IN)                 \
    _(JSOP_HASOWN)             \
    _(JSOP_GETGNAME)           \
    _(JSOP_BINDGNAME)          \
    _(JSOP_SETGNAME)           \
    _(JSOP_STRICTSETGNAME)     \
    _(JSOP_SETNAME)            \
    _(JSOP_STRICTSETNAME)      \
    _(JSOP_GETPROP)            \
    _(JSOP_SETPROP)            \
    _(JSOP_STRICTSETPROP)      \
    _(JSOP_CALLPROP)           \
    _(JSOP_DELPROP)            \
    _(JSOP_STRICTDELPROP)      \
    _(JSOP_GETPROP_SUPER)      \
    _(JSOP_SETPROP_SUPER)      \
    _(JSOP_STRICTSETPROP_SUPER) \
    _(JSOP_LENGTH)             \
    _(JSOP_GETBOUNDNAME)       \
    _(JSOP_GETALIASEDVAR)      \
    _(JSOP_SETALIASEDVAR)      \
    _(JSOP_GETNAME)            \
    _(JSOP_BINDNAME)           \
    _(JSOP_DELNAME)            \
    _(JSOP_GETIMPORT)          \
    _(JSOP_GETINTRINSIC)       \
    _(JSOP_BINDVAR)            \
    _(JSOP_DEFVAR)             \
    _(JSOP_DEFCONST)           \
    _(JSOP_DEFLET)             \
    _(JSOP_DEFFUN)             \
    _(JSOP_GETLOCAL)           \
    _(JSOP_SETLOCAL)           \
    _(JSOP_GETARG)             \
    _(JSOP_SETARG)             \
    _(JSOP_CHECKLEXICAL)       \
    _(JSOP_INITLEXICAL)        \
    _(JSOP_INITGLEXICAL)       \
    _(JSOP_CHECKALIASEDLEXICAL) \
    _(JSOP_INITALIASEDLEXICAL) \
    _(JSOP_UNINITIALIZED)      \
    _(JSOP_CALL)               \
    _(JSOP_CALL_IGNORES_RV)    \
    _(JSOP_CALLITER)           \
    _(JSOP_FUNCALL)            \
    _(JSOP_FUNAPPLY)           \
    _(JSOP_NEW)                \
    _(JSOP_EVAL)               \
    _(JSOP_STRICTEVAL)         \
    _(JSOP_SPREADCALL)         \
    _(JSOP_SPREADNEW)          \
    _(JSOP_SPREADEVAL)         \
    _(JSOP_STRICTSPREADEVAL)   \
    _(JSOP_OPTIMIZE_SPREADCALL)\
    _(JSOP_IMPLICITTHIS)       \
    _(JSOP_GIMPLICITTHIS)      \
    _(JSOP_INSTANCEOF)         \
    _(JSOP_TYPEOF)             \
    _(JSOP_TYPEOFEXPR)         \
    _(JSOP_THROWMSG)           \
    _(JSOP_THROW)              \
    _(JSOP_THROWING)           \
    _(JSOP_TRY)                \
    _(JSOP_FINALLY)            \
    _(JSOP_GOSUB)              \
    _(JSOP_RETSUB)             \
    _(JSOP_PUSHLEXICALENV)     \
    _(JSOP_POPLEXICALENV)      \
    _(JSOP_FRESHENLEXICALENV)  \
    _(JSOP_RECREATELEXICALENV) \
    _(JSOP_DEBUGLEAVELEXICALENV) \
    _(JSOP_PUSHVARENV)         \
    _(JSOP_POPVARENV)          \
    _(JSOP_EXCEPTION)          \
    _(JSOP_DEBUGGER)           \
    _(JSOP_ARGUMENTS)          \
    _(JSOP_RUNONCE)            \
    _(JSOP_REST)               \
    _(JSOP_TOASYNC)            \
    _(JSOP_TOASYNCGEN)         \
    _(JSOP_TOASYNCITER)        \
    _(JSOP_TOID)               \
    _(JSOP_TOSTRING)           \
    _(JSOP_TABLESWITCH)        \
    _(JSOP_ITER)               \
    _(JSOP_MOREITER)           \
    _(JSOP_ISNOITER)           \
    _(JSOP_ENDITER)            \
    _(JSOP_ISGENCLOSING)       \
    _(JSOP_GENERATOR)          \
    _(JSOP_INITIALYIELD)       \
    _(JSOP_YIELD)              \
    _(JSOP_AWAIT)              \
    _(JSOP_TRYSKIPAWAIT)       \
    _(JSOP_DEBUGAFTERYIELD)    \
    _(JSOP_FINALYIELDRVAL)     \
    _(JSOP_RESUME)             \
    _(JSOP_CALLEE)             \
    _(JSOP_SUPERBASE)          \
    _(JSOP_SUPERFUN)           \
    _(JSOP_GETRVAL)            \
    _(JSOP_SETRVAL)            \
    _(JSOP_RETRVAL)            \
    _(JSOP_RETURN)             \
    _(JSOP_FUNCTIONTHIS)       \
    _(JSOP_GLOBALTHIS)         \
    _(JSOP_CHECKISOBJ)         \
    _(JSOP_CHECKISCALLABLE)    \
    _(JSOP_CHECKTHIS)          \
    _(JSOP_CHECKTHISREINIT)    \
    _(JSOP_CHECKRETURN)        \
    _(JSOP_NEWTARGET)          \
    _(JSOP_SUPERCALL)          \
    _(JSOP_SPREADSUPERCALL)    \
    _(JSOP_THROWSETCONST)      \
    _(JSOP_THROWSETALIASEDCONST) \
    _(JSOP_THROWSETCALLEE)     \
    _(JSOP_INITHIDDENPROP_GETTER) \
    _(JSOP_INITHIDDENPROP_SETTER) \
    _(JSOP_INITHIDDENELEM)     \
    _(JSOP_INITHIDDENELEM_GETTER) \
    _(JSOP_INITHIDDENELEM_SETTER) \
    _(JSOP_CHECKOBJCOERCIBLE)  \
    _(JSOP_DEBUGCHECKSELFHOSTED) \
    _(JSOP_JUMPTARGET)         \
    _(JSOP_IS_CONSTRUCTING)    \
    _(JSOP_TRY_DESTRUCTURING_ITERCLOSE) \
    _(JSOP_CHECKCLASSHERITAGE) \
    _(JSOP_INITHOMEOBJECT)     \
    _(JSOP_BUILTINPROTO)       \
    _(JSOP_OBJWITHPROTO)       \
    _(JSOP_FUNWITHPROTO)       \
    _(JSOP_CLASSCONSTRUCTOR)   \
    _(JSOP_DERIVEDCONSTRUCTOR) \
    _(JSOP_IMPORTMETA)

class BaselineCompiler final
{
    JSContext* cx;
    JSScript* script;
    jsbytecode* pc;
    StackMacroAssembler masm;
    bool ionCompileable_;
    bool compileDebugInstrumentation_;

    TempAllocator& alloc_;
    BytecodeAnalysis analysis_;
    FrameInfo frame;

    FallbackICStubSpace stubSpace_;
    js::Vector<ICEntry, 16, SystemAllocPolicy> icEntries_;

    // Stores the native code offset for a bytecode pc.
    struct PCMappingEntry
    {
        uint32_t pcOffset;
        uint32_t nativeOffset;
        PCMappingSlotInfo slotInfo;

        // If set, insert a PCMappingIndexEntry before encoding the
        // current entry.
        bool addIndexEntry;
    };

    js::Vector<PCMappingEntry, 16, SystemAllocPolicy> pcMappingEntries_;

    // Labels for the 'movWithPatch' for loading IC entry pointers in
    // the generated IC-calling code in the main jitcode.  These need
    // to be patched with the actual icEntry offsets after the BaselineScript
    // has been allocated.
    struct ICLoadLabel {
        size_t icEntry;
        CodeOffset label;
    };
    js::Vector<ICLoadLabel, 16, SystemAllocPolicy> icLoadLabels_;

    uint32_t pushedBeforeCall_;
=======
#define OPCODE_LIST(_)          \
  _(JSOP_NOP)                   \
  _(JSOP_NOP_DESTRUCTURING)     \
  _(JSOP_LABEL)                 \
  _(JSOP_ITERNEXT)              \
  _(JSOP_POP)                   \
  _(JSOP_POPN)                  \
  _(JSOP_DUPAT)                 \
  _(JSOP_ENTERWITH)             \
  _(JSOP_LEAVEWITH)             \
  _(JSOP_DUP)                   \
  _(JSOP_DUP2)                  \
  _(JSOP_SWAP)                  \
  _(JSOP_PICK)                  \
  _(JSOP_UNPICK)                \
  _(JSOP_GOTO)                  \
  _(JSOP_IFEQ)                  \
  _(JSOP_IFNE)                  \
  _(JSOP_AND)                   \
  _(JSOP_OR)                    \
  _(JSOP_NOT)                   \
  _(JSOP_POS)                   \
  _(JSOP_TONUMERIC)             \
  _(JSOP_LOOPHEAD)              \
  _(JSOP_LOOPENTRY)             \
  _(JSOP_VOID)                  \
  _(JSOP_UNDEFINED)             \
  _(JSOP_HOLE)                  \
  _(JSOP_NULL)                  \
  _(JSOP_TRUE)                  \
  _(JSOP_FALSE)                 \
  _(JSOP_ZERO)                  \
  _(JSOP_ONE)                   \
  _(JSOP_INT8)                  \
  _(JSOP_INT32)                 \
  _(JSOP_UINT16)                \
  _(JSOP_UINT24)                \
  _(JSOP_RESUMEINDEX)           \
  _(JSOP_DOUBLE)                \
  _(JSOP_BIGINT)                \
  _(JSOP_STRING)                \
  _(JSOP_SYMBOL)                \
  _(JSOP_OBJECT)                \
  _(JSOP_CALLSITEOBJ)           \
  _(JSOP_REGEXP)                \
  _(JSOP_LAMBDA)                \
  _(JSOP_LAMBDA_ARROW)          \
  _(JSOP_SETFUNNAME)            \
  _(JSOP_BITOR)                 \
  _(JSOP_BITXOR)                \
  _(JSOP_BITAND)                \
  _(JSOP_LSH)                   \
  _(JSOP_RSH)                   \
  _(JSOP_URSH)                  \
  _(JSOP_ADD)                   \
  _(JSOP_SUB)                   \
  _(JSOP_MUL)                   \
  _(JSOP_DIV)                   \
  _(JSOP_MOD)                   \
  _(JSOP_POW)                   \
  _(JSOP_LT)                    \
  _(JSOP_LE)                    \
  _(JSOP_GT)                    \
  _(JSOP_GE)                    \
  _(JSOP_EQ)                    \
  _(JSOP_NE)                    \
  _(JSOP_STRICTEQ)              \
  _(JSOP_STRICTNE)              \
  _(JSOP_CONDSWITCH)            \
  _(JSOP_CASE)                  \
  _(JSOP_DEFAULT)               \
  _(JSOP_LINENO)                \
  _(JSOP_BITNOT)                \
  _(JSOP_NEG)                   \
  _(JSOP_NEWARRAY)              \
  _(JSOP_NEWARRAY_COPYONWRITE)  \
  _(JSOP_INITELEM_ARRAY)        \
  _(JSOP_NEWOBJECT)             \
  _(JSOP_NEWINIT)               \
  _(JSOP_INITELEM)              \
  _(JSOP_INITELEM_GETTER)       \
  _(JSOP_INITELEM_SETTER)       \
  _(JSOP_INITELEM_INC)          \
  _(JSOP_MUTATEPROTO)           \
  _(JSOP_INITPROP)              \
  _(JSOP_INITLOCKEDPROP)        \
  _(JSOP_INITHIDDENPROP)        \
  _(JSOP_INITPROP_GETTER)       \
  _(JSOP_INITPROP_SETTER)       \
  _(JSOP_GETELEM)               \
  _(JSOP_SETELEM)               \
  _(JSOP_STRICTSETELEM)         \
  _(JSOP_CALLELEM)              \
  _(JSOP_DELELEM)               \
  _(JSOP_STRICTDELELEM)         \
  _(JSOP_GETELEM_SUPER)         \
  _(JSOP_SETELEM_SUPER)         \
  _(JSOP_STRICTSETELEM_SUPER)   \
  _(JSOP_IN)                    \
  _(JSOP_HASOWN)                \
  _(JSOP_GETGNAME)              \
  _(JSOP_BINDGNAME)             \
  _(JSOP_SETGNAME)              \
  _(JSOP_STRICTSETGNAME)        \
  _(JSOP_SETNAME)               \
  _(JSOP_STRICTSETNAME)         \
  _(JSOP_GETPROP)               \
  _(JSOP_SETPROP)               \
  _(JSOP_STRICTSETPROP)         \
  _(JSOP_CALLPROP)              \
  _(JSOP_DELPROP)               \
  _(JSOP_STRICTDELPROP)         \
  _(JSOP_GETPROP_SUPER)         \
  _(JSOP_SETPROP_SUPER)         \
  _(JSOP_STRICTSETPROP_SUPER)   \
  _(JSOP_LENGTH)                \
  _(JSOP_GETBOUNDNAME)          \
  _(JSOP_GETALIASEDVAR)         \
  _(JSOP_SETALIASEDVAR)         \
  _(JSOP_GETNAME)               \
  _(JSOP_BINDNAME)              \
  _(JSOP_DELNAME)               \
  _(JSOP_GETIMPORT)             \
  _(JSOP_GETINTRINSIC)          \
  _(JSOP_SETINTRINSIC)          \
  _(JSOP_BINDVAR)               \
  _(JSOP_DEFVAR)                \
  _(JSOP_DEFCONST)              \
  _(JSOP_DEFLET)                \
  _(JSOP_DEFFUN)                \
  _(JSOP_GETLOCAL)              \
  _(JSOP_SETLOCAL)              \
  _(JSOP_GETARG)                \
  _(JSOP_SETARG)                \
  _(JSOP_CHECKLEXICAL)          \
  _(JSOP_INITLEXICAL)           \
  _(JSOP_INITGLEXICAL)          \
  _(JSOP_CHECKALIASEDLEXICAL)   \
  _(JSOP_INITALIASEDLEXICAL)    \
  _(JSOP_UNINITIALIZED)         \
  _(JSOP_CALL)                  \
  _(JSOP_CALL_IGNORES_RV)       \
  _(JSOP_CALLITER)              \
  _(JSOP_FUNCALL)               \
  _(JSOP_FUNAPPLY)              \
  _(JSOP_NEW)                   \
  _(JSOP_EVAL)                  \
  _(JSOP_STRICTEVAL)            \
  _(JSOP_SPREADCALL)            \
  _(JSOP_SPREADNEW)             \
  _(JSOP_SPREADEVAL)            \
  _(JSOP_STRICTSPREADEVAL)      \
  _(JSOP_OPTIMIZE_SPREADCALL)   \
  _(JSOP_IMPLICITTHIS)          \
  _(JSOP_GIMPLICITTHIS)         \
  _(JSOP_INSTANCEOF)            \
  _(JSOP_TYPEOF)                \
  _(JSOP_TYPEOFEXPR)            \
  _(JSOP_THROWMSG)              \
  _(JSOP_THROW)                 \
  _(JSOP_TRY)                   \
  _(JSOP_FINALLY)               \
  _(JSOP_GOSUB)                 \
  _(JSOP_RETSUB)                \
  _(JSOP_PUSHLEXICALENV)        \
  _(JSOP_POPLEXICALENV)         \
  _(JSOP_FRESHENLEXICALENV)     \
  _(JSOP_RECREATELEXICALENV)    \
  _(JSOP_DEBUGLEAVELEXICALENV)  \
  _(JSOP_PUSHVARENV)            \
  _(JSOP_POPVARENV)             \
  _(JSOP_EXCEPTION)             \
  _(JSOP_DEBUGGER)              \
  _(JSOP_ARGUMENTS)             \
  _(JSOP_REST)                  \
  _(JSOP_TOASYNCITER)           \
  _(JSOP_TOID)                  \
  _(JSOP_TOSTRING)              \
  _(JSOP_TABLESWITCH)           \
  _(JSOP_ITER)                  \
  _(JSOP_MOREITER)              \
  _(JSOP_ISNOITER)              \
  _(JSOP_ENDITER)               \
  _(JSOP_ISGENCLOSING)          \
  _(JSOP_GENERATOR)             \
  _(JSOP_INITIALYIELD)          \
  _(JSOP_YIELD)                 \
  _(JSOP_AWAIT)                 \
  _(JSOP_TRYSKIPAWAIT)          \
  _(JSOP_AFTERYIELD)            \
  _(JSOP_FINALYIELDRVAL)        \
  _(JSOP_RESUME)                \
  _(JSOP_ASYNCAWAIT)            \
  _(JSOP_ASYNCRESOLVE)          \
  _(JSOP_CALLEE)                \
  _(JSOP_ENVCALLEE)             \
  _(JSOP_SUPERBASE)             \
  _(JSOP_SUPERFUN)              \
  _(JSOP_GETRVAL)               \
  _(JSOP_SETRVAL)               \
  _(JSOP_RETRVAL)               \
  _(JSOP_RETURN)                \
  _(JSOP_FUNCTIONTHIS)          \
  _(JSOP_GLOBALTHIS)            \
  _(JSOP_CHECKISOBJ)            \
  _(JSOP_CHECKISCALLABLE)       \
  _(JSOP_CHECKTHIS)             \
  _(JSOP_CHECKTHISREINIT)       \
  _(JSOP_CHECKRETURN)           \
  _(JSOP_NEWTARGET)             \
  _(JSOP_SUPERCALL)             \
  _(JSOP_SPREADSUPERCALL)       \
  _(JSOP_THROWSETCONST)         \
  _(JSOP_THROWSETALIASEDCONST)  \
  _(JSOP_THROWSETCALLEE)        \
  _(JSOP_INITHIDDENPROP_GETTER) \
  _(JSOP_INITHIDDENPROP_SETTER) \
  _(JSOP_INITHIDDENELEM)        \
  _(JSOP_INITHIDDENELEM_GETTER) \
  _(JSOP_INITHIDDENELEM_SETTER) \
  _(JSOP_CHECKOBJCOERCIBLE)     \
  _(JSOP_DEBUGCHECKSELFHOSTED)  \
  _(JSOP_JUMPTARGET)            \
  _(JSOP_IS_CONSTRUCTING)       \
  _(JSOP_TRY_DESTRUCTURING)     \
  _(JSOP_CHECKCLASSHERITAGE)    \
  _(JSOP_INITHOMEOBJECT)        \
  _(JSOP_BUILTINPROTO)          \
  _(JSOP_OBJWITHPROTO)          \
  _(JSOP_FUNWITHPROTO)          \
  _(JSOP_CLASSCONSTRUCTOR)      \
  _(JSOP_DERIVEDCONSTRUCTOR)    \
  _(JSOP_IMPORTMETA)            \
  _(JSOP_DYNAMIC_IMPORT)        \
  _(JSOP_INC)                   \
  _(JSOP_DEC)

enum class ScriptGCThingType { RegExp, Function, Scope, BigInt };

// Base class for BaselineCompiler and BaselineInterpreterGenerator. The Handler
// template is a class storing fields/methods that are interpreter or compiler
// specific. This can be combined with template specialization of methods in
// this class to specialize behavior.
template <typename Handler>
class BaselineCodeGen {
 protected:
  Handler handler;

  JSContext* cx;
  StackMacroAssembler masm;

  typename Handler::FrameInfoT& frame;

  js::Vector<CodeOffset> traceLoggerToggleOffsets_;

  NonAssertingLabel return_;
  NonAssertingLabel postBarrierSlot_;

  CodeOffset profilerEnterFrameToggleOffset_;
  CodeOffset profilerExitFrameToggleOffset_;

  // Early Ion bailouts will enter at this address. This is after frame
  // construction and before environment chain is initialized.
  CodeOffset bailoutPrologueOffset_;

  // Baseline Interpreter can enter Baseline Compiler code at this address. This
  // is right after the warm-up counter check in the prologue.
  CodeOffset warmUpCheckPrologueOffset_;

  // Baseline Debug OSR during prologue will enter at this address. This is
  // right after where a debug prologue VM call would have returned.
  CodeOffset debugOsrPrologueOffset_;

  // Baseline Debug OSR during epilogue will enter at this address. This is
  // right after where a debug epilogue VM call would have returned.
  CodeOffset debugOsrEpilogueOffset_;

  uint32_t pushedBeforeCall_;
>>>>>>> upstream-releases
#ifdef DEBUG
  bool inCall_;
#endif

  // Whether any on stack arguments are modified.
  bool modifiesArguments_;

<<<<<<< HEAD
  template <typename... HandlerArgs>
  BaselineCodeGen(JSContext* cx, TempAllocator& alloc, JSScript* script,
                  HandlerArgs&&... args);
||||||| merged common ancestors
    Vector<CodeOffset> traceLoggerToggleOffsets_;
    CodeOffset traceLoggerScriptTextIdOffset_;
=======
  template <typename... HandlerArgs>
  explicit BaselineCodeGen(JSContext* cx, HandlerArgs&&... args);
>>>>>>> upstream-releases

<<<<<<< HEAD
  MOZ_MUST_USE bool appendRetAddrEntry(RetAddrEntry::Kind kind,
                                       uint32_t retOffset) {
    if (!retAddrEntries_.emplaceBack(script->pcToOffset(pc), kind,
                                     CodeOffset(retOffset))) {
      ReportOutOfMemory(cx);
      return false;
    }
    return true;
  }
||||||| merged common ancestors
    FixedList<Label> labels_;
    NonAssertingLabel return_;
    NonAssertingLabel postBarrierSlot_;

    // Native code offset right before the scope chain is initialized.
    CodeOffset prologueOffset_;

    // Native code offset right before the frame is popped and the method
    // returned from.
    CodeOffset epilogueOffset_;

    // Native code offset right after debug prologue and epilogue, or
    // equivalent positions when debug mode is off.
    CodeOffset postDebugPrologueOffset_;

    // For each INITIALYIELD or YIELD or AWAIT op, this Vector maps the yield
    // index to the bytecode offset of the next op.
    Vector<uint32_t>            yieldAndAwaitOffsets_;

    // Whether any on stack arguments are modified.
    bool modifiesArguments_;

    Label* labelOf(jsbytecode* pc) {
        return &labels_[script->pcToOffset(pc)];
    }
=======
  template <typename T>
  void pushArg(const T& t) {
    masm.Push(t);
  }

  // Pushes the current script as argument for a VM function.
  void pushScriptArg();

  // Pushes the bytecode pc as argument for a VM function.
  void pushBytecodePCArg();

  // Pushes a name/object/scope associated with the current bytecode op (and
  // stored in the script) as argument for a VM function.
  void loadScriptGCThing(ScriptGCThingType type, Register dest,
                         Register scratch);
  void pushScriptGCThingArg(ScriptGCThingType type, Register scratch1,
                            Register scratch2);
  void pushScriptNameArg(Register scratch1, Register scratch2);

  // Pushes a bytecode operand as argument for a VM function.
  void pushUint8BytecodeOperandArg(Register scratch);
  void pushUint16BytecodeOperandArg(Register scratch);

  void loadInt32LengthBytecodeOperand(Register dest);
  void loadInt32IndexBytecodeOperand(ValueOperand dest);
  void loadNumFormalArguments(Register dest);

  // Loads the current JSScript* in dest.
  void loadScript(Register dest);
>>>>>>> upstream-releases

<<<<<<< HEAD
  JSFunction* function() const {
    // Not delazifying here is ok as the function is guaranteed to have
    // been delazified before compilation started.
    return script->functionNonDelazifying();
  }
||||||| merged common ancestors
    // If a script has more |nslots| than this, then emit code to do an
    // early stack check.
    static const unsigned EARLY_STACK_CHECK_SLOT_COUNT = 128;
    bool needsEarlyStackCheck() const {
        return script->nslots() > EARLY_STACK_CHECK_SLOT_COUNT;
    }
=======
  void saveInterpreterPCReg();
  void restoreInterpreterPCReg();
>>>>>>> upstream-releases

<<<<<<< HEAD
  ModuleObject* module() const { return script->module(); }
||||||| merged common ancestors
  public:
    BaselineCompiler(JSContext* cx, TempAllocator& alloc, JSScript* script);
    MOZ_MUST_USE bool init();
=======
  // Subtracts |script->nslots() * sizeof(Value)| from reg.
  void subtractScriptSlotsSize(Register reg, Register scratch);
>>>>>>> upstream-releases

<<<<<<< HEAD
  template <typename T>
  void pushArg(const T& t) {
    masm.Push(t);
  }
  void prepareVMCall();
||||||| merged common ancestors
    MethodStatus compile();
=======
  // Jump to the script's resume entry indicated by resumeIndex.
  void jumpToResumeEntry(Register resumeIndex, Register scratch1,
                         Register scratch2);
>>>>>>> upstream-releases

<<<<<<< HEAD
  enum CallVMPhase { POST_INITIALIZE, CHECK_OVER_RECURSED };
  bool callVM(const VMFunction& fun, CallVMPhase phase = POST_INITIALIZE);

  bool callVMNonOp(const VMFunction& fun, CallVMPhase phase = POST_INITIALIZE) {
    if (!callVM(fun, phase)) {
      return false;
    }
    retAddrEntries_.back().setKind(RetAddrEntry::Kind::NonOpCallVM);
    return true;
  }

  // ifDebuggee should be a function emitting code for when the script is a
  // debuggee script. ifNotDebuggee (if present) is called to emit code for
  // non-debuggee scripts.
  template <typename F1, typename F2>
  MOZ_MUST_USE bool emitDebugInstrumentation(
      const F1& ifDebuggee, const mozilla::Maybe<F2>& ifNotDebuggee);
  template <typename F>
  MOZ_MUST_USE bool emitDebugInstrumentation(const F& ifDebuggee) {
    return emitDebugInstrumentation(ifDebuggee, mozilla::Maybe<F>());
  }

  MOZ_MUST_USE bool emitCheckThis(ValueOperand val, bool reinit = false);
  void emitLoadReturnValue(ValueOperand val);

  MOZ_MUST_USE bool emitNextIC();
  MOZ_MUST_USE bool emitInterruptCheck();
  MOZ_MUST_USE bool emitWarmUpCounterIncrement(bool allowOsr = true);
  MOZ_MUST_USE bool emitTraceLoggerResume(Register script,
                                          AllocatableGeneralRegisterSet& regs);

  void storeValue(const StackValue* source, const Address& dest,
                  const ValueOperand& scratch);
||||||| merged common ancestors
    void setCompileDebugInstrumentation() {
        compileDebugInstrumentation_ = true;
    }
=======
  // Load the global's lexical environment.
  void loadGlobalLexicalEnvironment(Register dest);
  void pushGlobalLexicalEnvironmentValue(ValueOperand scratch);
>>>>>>> upstream-releases

<<<<<<< HEAD
#define EMIT_OP(op) bool emit_##op();
  OPCODE_LIST(EMIT_OP)
#undef EMIT_OP
||||||| merged common ancestors
  private:
    ICEntry* allocateICEntry(ICStub* stub, ICEntry::Kind kind) {
        if (!stub) {
            return nullptr;
        }
=======
  // Load the |this|-value from the global's lexical environment.
  void loadGlobalThisValue(ValueOperand dest);
>>>>>>> upstream-releases

<<<<<<< HEAD
  // JSOP_NEG, JSOP_BITNOT
  MOZ_MUST_USE bool emitUnaryArith();
||||||| merged common ancestors
        // Create the entry and add it to the vector.
        if (!icEntries_.append(ICEntry(script->pcToOffset(pc), kind))) {
            ReportOutOfMemory(cx);
            return nullptr;
        }
        ICEntry& vecEntry = icEntries_.back();
=======
  // Load script atom |index| into |dest|.
  void loadScriptAtom(Register index, Register dest);
>>>>>>> upstream-releases

<<<<<<< HEAD
  // JSOP_BITXOR, JSOP_LSH, JSOP_ADD etc.
  MOZ_MUST_USE bool emitBinaryArith();
||||||| merged common ancestors
        // Set the first stub for the IC entry to the fallback stub
        vecEntry.setFirstStub(stub);
=======
  void prepareVMCall();
>>>>>>> upstream-releases

<<<<<<< HEAD
  // Handles JSOP_LT, JSOP_GT, and friends
  MOZ_MUST_USE bool emitCompare();
||||||| merged common ancestors
        // Return pointer to the IC entry
        return &vecEntry;
    }
=======
  void storeFrameSizeAndPushDescriptor(uint32_t frameBaseSize, uint32_t argSize,
                                       const Address& frameSizeAddr,
                                       Register scratch1, Register scratch2);
>>>>>>> upstream-releases

<<<<<<< HEAD
  // For a JOF_JUMP op, jumps to the op's jump target.
  void emitJump();
||||||| merged common ancestors
    // Append an ICEntry without a stub.
    bool appendICEntry(ICEntry::Kind kind, uint32_t returnOffset) {
        ICEntry entry(script->pcToOffset(pc), kind);
        entry.setReturnOffset(CodeOffset(returnOffset));
        if (!icEntries_.append(entry)) {
            ReportOutOfMemory(cx);
            return false;
        }
        return true;
    }
=======
  enum CallVMPhase { POST_INITIALIZE, CHECK_OVER_RECURSED };
  bool callVMInternal(VMFunctionId id, CallVMPhase phase);
>>>>>>> upstream-releases

<<<<<<< HEAD
  // For a JOF_JUMP op, jumps to the op's jump target depending on the Value
  // in |val|.
  void emitTestBooleanTruthy(bool branchIfTrue, ValueOperand val);
||||||| merged common ancestors
    bool addICLoadLabel(CodeOffset label) {
        MOZ_ASSERT(!icEntries_.empty());
        ICLoadLabel loadLabel;
        loadLabel.label = label;
        loadLabel.icEntry = icEntries_.length() - 1;
        if (!icLoadLabels_.append(loadLabel)) {
            ReportOutOfMemory(cx);
            return false;
        }
        return true;
    }
=======
  template <typename Fn, Fn fn>
  bool callVM(CallVMPhase phase = POST_INITIALIZE);
>>>>>>> upstream-releases

<<<<<<< HEAD
  // Converts |val| to an index in the jump table and stores this in |dest|
  // or branches to the default pc if not int32 or out-of-range.
  void emitGetTableSwitchIndex(ValueOperand val, Register dest);
||||||| merged common ancestors
    JSFunction* function() const {
        // Not delazifying here is ok as the function is guaranteed to have
        // been delazified before compilation started.
        return script->functionNonDelazifying();
    }
=======
  template <typename Fn, Fn fn>
  bool callVMNonOp(CallVMPhase phase = POST_INITIALIZE) {
    if (!callVM<Fn, fn>(phase)) {
      return false;
    }
    handler.markLastRetAddrEntryKind(RetAddrEntry::Kind::NonOpCallVM);
    return true;
  }

  // ifDebuggee should be a function emitting code for when the script is a
  // debuggee script. ifNotDebuggee (if present) is called to emit code for
  // non-debuggee scripts.
  template <typename F1, typename F2>
  MOZ_MUST_USE bool emitDebugInstrumentation(
      const F1& ifDebuggee, const mozilla::Maybe<F2>& ifNotDebuggee);
  template <typename F>
  MOZ_MUST_USE bool emitDebugInstrumentation(const F& ifDebuggee) {
    return emitDebugInstrumentation(ifDebuggee, mozilla::Maybe<F>());
  }

  template <typename F>
  MOZ_MUST_USE bool emitAfterYieldDebugInstrumentation(const F& ifDebuggee,
                                                       Register scratch);

  // ifSet should be a function emitting code for when the script has |flag|
  // set. ifNotSet emits code for when the flag isn't set.
  template <typename F1, typename F2>
  MOZ_MUST_USE bool emitTestScriptFlag(JSScript::ImmutableFlags flag,
                                       const F1& ifSet, const F2& ifNotSet,
                                       Register scratch);

  // If |script->hasFlag(flag) == value|, execute the code emitted by |emit|.
  template <typename F>
  MOZ_MUST_USE bool emitTestScriptFlag(JSScript::ImmutableFlags flag,
                                       bool value, const F& emit,
                                       Register scratch);
  template <typename F>
  MOZ_MUST_USE bool emitTestScriptFlag(JSScript::MutableFlags flag, bool value,
                                       const F& emit, Register scratch);

  MOZ_MUST_USE bool emitGeneratorResume(GeneratorResumeKind resumeKind);
  MOZ_MUST_USE bool emitEnterGeneratorCode(Register script,
                                           Register resumeIndex,
                                           Register scratch);
  void emitInterpJumpToResumeEntry(Register script, Register resumeIndex,
                                   Register scratch);
  void emitJumpToInterpretOpLabel();

  MOZ_MUST_USE bool emitIncExecutionProgressCounter(Register scratch);

  MOZ_MUST_USE bool emitCheckThis(ValueOperand val, bool reinit = false);
  void emitLoadReturnValue(ValueOperand val);
  void emitPushNonArrowFunctionNewTarget();
  void emitGetAliasedVar(ValueOperand dest);

  MOZ_MUST_USE bool emitNextIC();
  MOZ_MUST_USE bool emitInterruptCheck();
  MOZ_MUST_USE bool emitWarmUpCounterIncrement();
  MOZ_MUST_USE bool emitTraceLoggerResume(Register script,
                                          AllocatableGeneralRegisterSet& regs);
>>>>>>> upstream-releases

<<<<<<< HEAD
  MOZ_MUST_USE bool emitReturn();
||||||| merged common ancestors
    ModuleObject* module() const {
        return script->module();
    }
=======
#define EMIT_OP(op) bool emit_##op();
  OPCODE_LIST(EMIT_OP)
#undef EMIT_OP
>>>>>>> upstream-releases

<<<<<<< HEAD
  MOZ_MUST_USE bool emitToBoolean();
  MOZ_MUST_USE bool emitTest(bool branchIfTrue);
  MOZ_MUST_USE bool emitAndOr(bool branchIfTrue);
||||||| merged common ancestors
    PCMappingSlotInfo getStackTopSlotInfo() {
        MOZ_ASSERT(frame.numUnsyncedSlots() <= 2);
        switch (frame.numUnsyncedSlots()) {
          case 0:
            return PCMappingSlotInfo::MakeSlotInfo();
          case 1:
            return PCMappingSlotInfo::MakeSlotInfo(PCMappingSlotInfo::ToSlotLocation(frame.peek(-1)));
          case 2:
          default:
            return PCMappingSlotInfo::MakeSlotInfo(PCMappingSlotInfo::ToSlotLocation(frame.peek(-1)),
                                                   PCMappingSlotInfo::ToSlotLocation(frame.peek(-2)));
        }
    }
=======
  // JSOP_NEG, JSOP_BITNOT, JSOP_INC, JSOP_DEC
  MOZ_MUST_USE bool emitUnaryArith();
>>>>>>> upstream-releases

<<<<<<< HEAD
  MOZ_MUST_USE bool emitCall(JSOp op);
  MOZ_MUST_USE bool emitSpreadCall(JSOp op);
||||||| merged common ancestors
    template <typename T>
    void pushArg(const T& t) {
        masm.Push(t);
    }
    void prepareVMCall();

    enum CallVMPhase {
        POST_INITIALIZE,
        CHECK_OVER_RECURSED
    };
    bool callVM(const VMFunction& fun, CallVMPhase phase=POST_INITIALIZE);

    bool callVMNonOp(const VMFunction& fun, CallVMPhase phase=POST_INITIALIZE) {
        if (!callVM(fun, phase)) {
            return false;
        }
        icEntries_.back().setFakeKind(ICEntry::Kind_NonOpCallVM);
        return true;
    }
=======
  // JSOP_BITXOR, JSOP_LSH, JSOP_ADD etc.
  MOZ_MUST_USE bool emitBinaryArith();
>>>>>>> upstream-releases

<<<<<<< HEAD
  MOZ_MUST_USE bool emitDelElem(bool strict);
  MOZ_MUST_USE bool emitDelProp(bool strict);
  MOZ_MUST_USE bool emitSetElemSuper(bool strict);
  MOZ_MUST_USE bool emitSetPropSuper(bool strict);
||||||| merged common ancestors
    BytecodeAnalysis& analysis() {
        return analysis_;
    }
=======
  // Handles JSOP_LT, JSOP_GT, and friends
  MOZ_MUST_USE bool emitCompare();

  // For a JOF_JUMP op, jumps to the op's jump target.
  void emitJump();

  // For a JOF_JUMP op, jumps to the op's jump target depending on the Value
  // in |val|.
  void emitTestBooleanTruthy(bool branchIfTrue, ValueOperand val);

  // Converts |val| to an index in the jump table and stores this in |dest|
  // or branches to the default pc if not int32 or out-of-range.
  void emitGetTableSwitchIndex(ValueOperand val, Register dest,
                               Register scratch1, Register scratch2);

  // Jumps to the target of a table switch based on |key| and the
  // firstResumeIndex stored in JSOP_TABLESWITCH.
  void emitTableSwitchJump(Register key, Register scratch1, Register scratch2);

  MOZ_MUST_USE bool emitReturn();

  MOZ_MUST_USE bool emitToBoolean();
  MOZ_MUST_USE bool emitTest(bool branchIfTrue);
  MOZ_MUST_USE bool emitAndOr(bool branchIfTrue);

  MOZ_MUST_USE bool emitCall(JSOp op);
  MOZ_MUST_USE bool emitSpreadCall(JSOp op);

  MOZ_MUST_USE bool emitDelElem(bool strict);
  MOZ_MUST_USE bool emitDelProp(bool strict);
  MOZ_MUST_USE bool emitSetElemSuper(bool strict);
  MOZ_MUST_USE bool emitSetPropSuper(bool strict);

  MOZ_MUST_USE bool emitBindName(JSOp op);
  MOZ_MUST_USE bool emitDefLexical(JSOp op);

  // Try to bake in the result of GETGNAME/BINDGNAME instead of using an IC.
  // Return true if we managed to optimize the op.
  bool tryOptimizeGetGlobalName();
  bool tryOptimizeBindGlobalName();

  MOZ_MUST_USE bool emitInitPropGetterSetter();
  MOZ_MUST_USE bool emitInitElemGetterSetter();

  MOZ_MUST_USE bool emitFormalArgAccess(JSOp op);

  MOZ_MUST_USE bool emitThrowConstAssignment();
  MOZ_MUST_USE bool emitUninitializedLexicalCheck(const ValueOperand& val);

  MOZ_MUST_USE bool emitIsMagicValue();

  void getEnvironmentCoordinateObject(Register reg);
  Address getEnvironmentCoordinateAddressFromObject(Register objReg,
                                                    Register reg);
  Address getEnvironmentCoordinateAddress(Register reg);

  MOZ_MUST_USE bool emitPrologue();
  MOZ_MUST_USE bool emitEpilogue();
  MOZ_MUST_USE bool emitOutOfLinePostBarrierSlot();
  MOZ_MUST_USE bool emitStackCheck();
  MOZ_MUST_USE bool emitArgumentTypeChecks();
  MOZ_MUST_USE bool emitDebugPrologue();

  template <typename F1, typename F2>
  MOZ_MUST_USE bool initEnvironmentChainHelper(const F1& initFunctionEnv,
                                               const F2& initGlobalOrEvalEnv,
                                               Register scratch);
  MOZ_MUST_USE bool initEnvironmentChain();

  MOZ_MUST_USE bool emitTraceLoggerEnter();
  MOZ_MUST_USE bool emitTraceLoggerExit();

  MOZ_MUST_USE bool emitHandleCodeCoverageAtPrologue();

  void emitInitFrameFields();
  void emitIsDebuggeeCheck();
  void emitInitializeLocals();
  void emitPreInitEnvironmentChain(Register nonFunctionEnv);

  void emitProfilerEnterFrame();
  void emitProfilerExitFrame();
};

using RetAddrEntryVector = js::Vector<RetAddrEntry, 16, SystemAllocPolicy>;

// Interface used by BaselineCodeGen for BaselineCompiler.
class BaselineCompilerHandler {
  CompilerFrameInfo frame_;
  TempAllocator& alloc_;
  BytecodeAnalysis analysis_;
  FixedList<Label> labels_;
  RetAddrEntryVector retAddrEntries_;
  JSScript* script_;
  jsbytecode* pc_;

  // Index of the current ICEntry in the script's JitScript.
  uint32_t icEntryIndex_;

  bool compileDebugInstrumentation_;
  bool ionCompileable_;

 public:
  using FrameInfoT = CompilerFrameInfo;

  BaselineCompilerHandler(JSContext* cx, MacroAssembler& masm,
                          TempAllocator& alloc, JSScript* script);

  MOZ_MUST_USE bool init(JSContext* cx);

  CompilerFrameInfo& frame() { return frame_; }

  jsbytecode* pc() const { return pc_; }
  jsbytecode* maybePC() const { return pc_; }
>>>>>>> upstream-releases

<<<<<<< HEAD
  MOZ_MUST_USE bool emitBindName(JSOp op);
  MOZ_MUST_USE bool emitDefLexical(JSOp op);
||||||| merged common ancestors
    MethodStatus emitBody();
=======
  void moveToNextPC() { pc_ += GetBytecodeLength(pc_); }
  Label* labelOf(jsbytecode* pc) { return &labels_[script_->pcToOffset(pc)]; }
>>>>>>> upstream-releases

<<<<<<< HEAD
  MOZ_MUST_USE bool emitInitPropGetterSetter();
  MOZ_MUST_USE bool emitInitElemGetterSetter();
||||||| merged common ancestors
    MOZ_MUST_USE bool emitCheckThis(ValueOperand val, bool reinit=false);
    void emitLoadReturnValue(ValueOperand val);
=======
  bool isDefinitelyLastOp() const { return pc_ == script_->lastPC(); }
>>>>>>> upstream-releases

<<<<<<< HEAD
  MOZ_MUST_USE bool emitFormalArgAccess(uint32_t arg, bool get);
||||||| merged common ancestors
    void emitInitializeLocals();
    MOZ_MUST_USE bool emitPrologue();
    MOZ_MUST_USE bool emitEpilogue();
    MOZ_MUST_USE bool emitOutOfLinePostBarrierSlot();
    MOZ_MUST_USE bool emitIC(ICStub* stub, ICEntry::Kind kind);
    MOZ_MUST_USE bool emitOpIC(ICStub* stub) {
        return emitIC(stub, ICEntry::Kind_Op);
    }
    MOZ_MUST_USE bool emitNonOpIC(ICStub* stub) {
        return emitIC(stub, ICEntry::Kind_NonOp);
    }
=======
  JSScript* script() const { return script_; }
  JSScript* maybeScript() const { return script_; }

  JSFunction* function() const {
    // Not delazifying here is ok as the function is guaranteed to have
    // been delazified before compilation started.
    return script_->functionNonDelazifying();
  }
  JSFunction* maybeFunction() const { return function(); }

  ModuleObject* module() const { return script_->module(); }

  void setCompileDebugInstrumentation() { compileDebugInstrumentation_ = true; }
  bool compileDebugInstrumentation() const {
    return compileDebugInstrumentation_;
  }

  bool maybeIonCompileable() const { return ionCompileable_; }

  uint32_t icEntryIndex() const { return icEntryIndex_; }
  void moveToNextICEntry() { icEntryIndex_++; }

  BytecodeAnalysis& analysis() { return analysis_; }

  RetAddrEntryVector& retAddrEntries() { return retAddrEntries_; }

  MOZ_MUST_USE bool appendRetAddrEntry(JSContext* cx, RetAddrEntry::Kind kind,
                                       uint32_t retOffset) {
    if (!retAddrEntries_.emplaceBack(script_->pcToOffset(pc_), kind,
                                     CodeOffset(retOffset))) {
      ReportOutOfMemory(cx);
      return false;
    }
    return true;
  }
  void markLastRetAddrEntryKind(RetAddrEntry::Kind kind) {
    retAddrEntries_.back().setKind(kind);
  }

  // If a script has more |nslots| than this, then emit code to do an
  // early stack check.
  bool needsEarlyStackCheck() const {
    static const unsigned EARLY_STACK_CHECK_SLOT_COUNT = 128;
    return script()->nslots() > EARLY_STACK_CHECK_SLOT_COUNT;
  }

  JSObject* maybeNoCloneSingletonObject();
};

using BaselineCompilerCodeGen = BaselineCodeGen<BaselineCompilerHandler>;

class BaselineCompiler final : private BaselineCompilerCodeGen {
  // Stores the native code offset for a bytecode pc.
  struct PCMappingEntry {
    uint32_t pcOffset;
    uint32_t nativeOffset;
    PCMappingSlotInfo slotInfo;

    // If set, insert a PCMappingIndexEntry before encoding the
    // current entry.
    bool addIndexEntry;
  };

  js::Vector<PCMappingEntry, 16, SystemAllocPolicy> pcMappingEntries_;

  CodeOffset profilerPushToggleOffset_;

  CodeOffset traceLoggerScriptTextIdOffset_;

 public:
  BaselineCompiler(JSContext* cx, TempAllocator& alloc, JSScript* script);
  MOZ_MUST_USE bool init();

  MethodStatus compile();

  bool compileDebugInstrumentation() const {
    return handler.compileDebugInstrumentation();
  }
  void setCompileDebugInstrumentation() {
    handler.setCompileDebugInstrumentation();
  }

 private:
  PCMappingSlotInfo getStackTopSlotInfo() {
    MOZ_ASSERT(frame.numUnsyncedSlots() <= 2);
    switch (frame.numUnsyncedSlots()) {
      case 0:
        return PCMappingSlotInfo::MakeSlotInfo();
      case 1: {
        PCMappingSlotInfo::SlotLocation loc = frame.stackValueSlotLocation(-1);
        return PCMappingSlotInfo::MakeSlotInfo(loc);
      }
      case 2:
      default: {
        PCMappingSlotInfo::SlotLocation loc1 = frame.stackValueSlotLocation(-1);
        PCMappingSlotInfo::SlotLocation loc2 = frame.stackValueSlotLocation(-2);
        return PCMappingSlotInfo::MakeSlotInfo(loc1, loc2);
      }
    }
  }
>>>>>>> upstream-releases

<<<<<<< HEAD
  MOZ_MUST_USE bool emitThrowConstAssignment();
  MOZ_MUST_USE bool emitUninitializedLexicalCheck(const ValueOperand& val);
||||||| merged common ancestors
    MOZ_MUST_USE bool emitStackCheck();
    MOZ_MUST_USE bool emitInterruptCheck();
    MOZ_MUST_USE bool emitWarmUpCounterIncrement(bool allowOsr=true);
    MOZ_MUST_USE bool emitArgumentTypeChecks();
    void emitIsDebuggeeCheck();
    MOZ_MUST_USE bool emitDebugPrologue();
    MOZ_MUST_USE bool emitDebugTrap();
    MOZ_MUST_USE bool emitTraceLoggerEnter();
    MOZ_MUST_USE bool emitTraceLoggerExit();
    MOZ_MUST_USE bool emitTraceLoggerResume(Register script, AllocatableGeneralRegisterSet& regs);
=======
  MethodStatus emitBody();
>>>>>>> upstream-releases

<<<<<<< HEAD
  MOZ_MUST_USE bool emitIsMagicValue();
||||||| merged common ancestors
    void emitProfilerEnterFrame();
    void emitProfilerExitFrame();
=======
  MOZ_MUST_USE bool emitDebugTrap();
>>>>>>> upstream-releases

<<<<<<< HEAD
  void getEnvironmentCoordinateObject(Register reg);
  Address getEnvironmentCoordinateAddressFromObject(Register objReg,
                                                    Register reg);
  Address getEnvironmentCoordinateAddress(Register reg);
};
||||||| merged common ancestors
    MOZ_MUST_USE bool initEnvironmentChain();
=======
  MOZ_MUST_USE bool addPCMappingEntry(bool addIndexEntry);
};
>>>>>>> upstream-releases

<<<<<<< HEAD
// Interface used by BaselineCodeGen for BaselineCompiler.
class BaselineCompilerHandler {
  TempAllocator& alloc_;
  FixedList<Label> labels_;
  JSScript* script_;
  bool compileDebugInstrumentation_;
||||||| merged common ancestors
    void storeValue(const StackValue* source, const Address& dest,
                    const ValueOperand& scratch);
=======
// Interface used by BaselineCodeGen for BaselineInterpreterGenerator.
class BaselineInterpreterHandler {
  InterpreterFrameInfo frame_;
>>>>>>> upstream-releases

<<<<<<< HEAD
 public:
  BaselineCompilerHandler(TempAllocator& alloc, JSScript* script);
||||||| merged common ancestors
#define EMIT_OP(op) bool emit_##op();
    OPCODE_LIST(EMIT_OP)
#undef EMIT_OP
=======
  // Entry point to start interpreting a bytecode op. No registers are live. PC
  // is loaded from the frame.
  Label interpretOp_;

  // Like interpretOp_ but at this point the PC is expected to be in
  // InterpreterPCReg.
  Label interpretOpWithPCReg_;
>>>>>>> upstream-releases

<<<<<<< HEAD
  MOZ_MUST_USE bool init();
||||||| merged common ancestors
    // JSOP_NEG, JSOP_BITNOT
    MOZ_MUST_USE bool emitUnaryArith();
=======
  // Offset of toggled jump for prologue debugger instrumentation.
  CodeOffset debuggeeCheckOffset_;
>>>>>>> upstream-releases

<<<<<<< HEAD
  Label* labelOf(jsbytecode* pc) { return &labels_[script_->pcToOffset(pc)]; }
||||||| merged common ancestors
    // JSOP_BITXOR, JSOP_LSH, JSOP_ADD etc.
    MOZ_MUST_USE bool emitBinaryArith();
=======
  // Offsets of toggled jumps for code coverage instrumentation.
  using CodeOffsetVector = Vector<uint32_t, 0, SystemAllocPolicy>;
  CodeOffsetVector codeCoverageOffsets_;
  Label codeCoverageAtPrologueLabel_;
  Label codeCoverageAtPCLabel_;
>>>>>>> upstream-releases

<<<<<<< HEAD
  void setCompileDebugInstrumentation() { compileDebugInstrumentation_ = true; }
  bool compileDebugInstrumentation() const {
    return compileDebugInstrumentation_;
  }
};
||||||| merged common ancestors
    // Handles JSOP_LT, JSOP_GT, and friends
    MOZ_MUST_USE bool emitCompare();
=======
 public:
  using FrameInfoT = InterpreterFrameInfo;
>>>>>>> upstream-releases

<<<<<<< HEAD
using BaselineCompilerCodeGen = BaselineCodeGen<BaselineCompilerHandler>;

class BaselineCompiler final : private BaselineCompilerCodeGen {
  // Stores the native code offset for a bytecode pc.
  struct PCMappingEntry {
    uint32_t pcOffset;
    uint32_t nativeOffset;
    PCMappingSlotInfo slotInfo;

    // If set, insert a PCMappingIndexEntry before encoding the
    // current entry.
    bool addIndexEntry;
  };

  js::Vector<PCMappingEntry, 16, SystemAllocPolicy> pcMappingEntries_;

  CodeOffset profilerPushToggleOffset_;
  CodeOffset profilerEnterFrameToggleOffset_;
  CodeOffset profilerExitFrameToggleOffset_;

  CodeOffset traceLoggerScriptTextIdOffset_;

  // Early Ion bailouts will enter at this address. This is after frame
  // construction and before environment chain is initialized.
  CodeOffset bailoutPrologueOffset_;

  // Baseline Debug OSR during prologue will enter at this address. This is
  // right after where a debug prologue VM call would have returned.
  CodeOffset debugOsrPrologueOffset_;

  // Baseline Debug OSR during epilogue will enter at this address. This is
  // right after where a debug epilogue VM call would have returned.
  CodeOffset debugOsrEpilogueOffset_;

  // If a script has more |nslots| than this, then emit code to do an
  // early stack check.
  static const unsigned EARLY_STACK_CHECK_SLOT_COUNT = 128;
  bool needsEarlyStackCheck() const {
    return script->nslots() > EARLY_STACK_CHECK_SLOT_COUNT;
  }

 public:
  BaselineCompiler(JSContext* cx, TempAllocator& alloc, JSScript* script);
  MOZ_MUST_USE bool init();

  MethodStatus compile();

  bool compileDebugInstrumentation() const {
    return handler.compileDebugInstrumentation();
  }
  void setCompileDebugInstrumentation() {
    handler.setCompileDebugInstrumentation();
  }

 private:
  PCMappingSlotInfo getStackTopSlotInfo() {
    MOZ_ASSERT(frame.numUnsyncedSlots() <= 2);
    switch (frame.numUnsyncedSlots()) {
      case 0:
        return PCMappingSlotInfo::MakeSlotInfo();
      case 1:
        return PCMappingSlotInfo::MakeSlotInfo(
            PCMappingSlotInfo::ToSlotLocation(frame.peek(-1)));
      case 2:
      default:
        return PCMappingSlotInfo::MakeSlotInfo(
            PCMappingSlotInfo::ToSlotLocation(frame.peek(-1)),
            PCMappingSlotInfo::ToSlotLocation(frame.peek(-2)));
    }
  }
||||||| merged common ancestors
    MOZ_MUST_USE bool emitReturn();
=======
  explicit BaselineInterpreterHandler(JSContext* cx, MacroAssembler& masm);
>>>>>>> upstream-releases

<<<<<<< HEAD
  BytecodeAnalysis& analysis() { return analysis_; }
||||||| merged common ancestors
    MOZ_MUST_USE bool emitToBoolean();
    MOZ_MUST_USE bool emitTest(bool branchIfTrue);
    MOZ_MUST_USE bool emitAndOr(bool branchIfTrue);
    MOZ_MUST_USE bool emitCall();
    MOZ_MUST_USE bool emitSpreadCall();
=======
  InterpreterFrameInfo& frame() { return frame_; }
>>>>>>> upstream-releases

<<<<<<< HEAD
  MethodStatus emitBody();
||||||| merged common ancestors
    MOZ_MUST_USE bool emitInitPropGetterSetter();
    MOZ_MUST_USE bool emitInitElemGetterSetter();
=======
  Label* interpretOpLabel() { return &interpretOp_; }
  Label* interpretOpWithPCRegLabel() { return &interpretOpWithPCReg_; }
>>>>>>> upstream-releases

<<<<<<< HEAD
  void emitInitializeLocals();
  MOZ_MUST_USE bool emitPrologue();
  MOZ_MUST_USE bool emitEpilogue();
  MOZ_MUST_USE bool emitOutOfLinePostBarrierSlot();
  MOZ_MUST_USE bool emitStackCheck();
  MOZ_MUST_USE bool emitArgumentTypeChecks();
  void emitIsDebuggeeCheck();
  MOZ_MUST_USE bool emitDebugPrologue();
  MOZ_MUST_USE bool emitDebugTrap();
  MOZ_MUST_USE bool emitTraceLoggerEnter();
  MOZ_MUST_USE bool emitTraceLoggerExit();
||||||| merged common ancestors
    MOZ_MUST_USE bool emitFormalArgAccess(uint32_t arg, bool get);
=======
  Label* codeCoverageAtPrologueLabel() { return &codeCoverageAtPrologueLabel_; }
  Label* codeCoverageAtPCLabel() { return &codeCoverageAtPCLabel_; }
>>>>>>> upstream-releases

<<<<<<< HEAD
  void emitProfilerEnterFrame();
  void emitProfilerExitFrame();
||||||| merged common ancestors
    MOZ_MUST_USE bool emitThrowConstAssignment();
    MOZ_MUST_USE bool emitUninitializedLexicalCheck(const ValueOperand& val);
=======
  CodeOffsetVector& codeCoverageOffsets() { return codeCoverageOffsets_; }
>>>>>>> upstream-releases

<<<<<<< HEAD
  MOZ_MUST_USE bool initEnvironmentChain();
||||||| merged common ancestors
    MOZ_MUST_USE bool emitIsMagicValue();
=======
  // Interpreter doesn't know the script and pc statically.
  jsbytecode* maybePC() const { return nullptr; }
  bool isDefinitelyLastOp() const { return false; }
  JSScript* maybeScript() const { return nullptr; }
  JSFunction* maybeFunction() const { return nullptr; }
>>>>>>> upstream-releases

<<<<<<< HEAD
  MOZ_MUST_USE bool addPCMappingEntry(bool addIndexEntry);
};
||||||| merged common ancestors
    MOZ_MUST_USE bool addPCMappingEntry(bool addIndexEntry);
=======
  void setDebuggeeCheckOffset(CodeOffset offset) {
    debuggeeCheckOffset_ = offset;
  }
  CodeOffset debuggeeCheckOffset() const { return debuggeeCheckOffset_; }
>>>>>>> upstream-releases

<<<<<<< HEAD
// Interface used by BaselineCodeGen for BaselineInterpreterGenerator.
class BaselineInterpreterHandler {
 public:
  explicit BaselineInterpreterHandler();
};
||||||| merged common ancestors
    MOZ_MUST_USE bool addYieldAndAwaitOffset();
=======
  // Interpreter doesn't need to keep track of RetAddrEntries, so these methods
  // are no-ops.
  MOZ_MUST_USE bool appendRetAddrEntry(JSContext* cx, RetAddrEntry::Kind kind,
                                       uint32_t retOffset) {
    return true;
  }
  void markLastRetAddrEntryKind(RetAddrEntry::Kind) {}
>>>>>>> upstream-releases

<<<<<<< HEAD
using BaselineInterpreterCodeGen = BaselineCodeGen<BaselineInterpreterHandler>;
||||||| merged common ancestors
    void getEnvironmentCoordinateObject(Register reg);
    Address getEnvironmentCoordinateAddressFromObject(Register objReg, Register reg);
    Address getEnvironmentCoordinateAddress(Register reg);
=======
  bool maybeIonCompileable() const { return true; }
>>>>>>> upstream-releases

<<<<<<< HEAD
class BaselineInterpreterGenerator final : private BaselineInterpreterCodeGen {
 public:
||||||| merged common ancestors
    void getThisEnvironmentCallee(Register reg);
=======
  // The interpreter always does the early stack check because we don't know the
  // frame size statically.
  bool needsEarlyStackCheck() const { return true; }

  JSObject* maybeNoCloneSingletonObject() { return nullptr; }
>>>>>>> upstream-releases
};

using BaselineInterpreterCodeGen = BaselineCodeGen<BaselineInterpreterHandler>;

class BaselineInterpreterGenerator final : private BaselineInterpreterCodeGen {
  // Offsets of patchable call instructions for debugger breakpoints/stepping.
  Vector<uint32_t, 0, SystemAllocPolicy> debugTrapOffsets_;

  // Offsets of move instructions for tableswitch base address.
  Vector<CodeOffset, 0, SystemAllocPolicy> tableLabels_;

  // Offset of the first tableswitch entry.
  uint32_t tableOffset_ = 0;

  // Offset of the code to start interpreting a bytecode op.
  uint32_t interpretOpOffset_ = 0;

  // Like interpretOpOffset_ but skips the debug trap for the current op.
  uint32_t interpretOpNoDebugTrapOffset_ = 0;

 public:
  explicit BaselineInterpreterGenerator(JSContext* cx);

  MOZ_MUST_USE bool generate(BaselineInterpreter& interpreter);

 private:
  MOZ_MUST_USE bool emitInterpreterLoop();
  MOZ_MUST_USE bool emitDebugTrap();

  void emitOutOfLineCodeCoverageInstrumentation();
};

}  // namespace jit
}  // namespace js

#endif /* jit_BaselineCompiler_h */
