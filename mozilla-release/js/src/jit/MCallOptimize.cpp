/* -*- Mode: C++; tab-width: 8; indent-tabs-mode: nil; c-basic-offset: 2 -*-
 * vim: set ts=8 sts=2 et sw=2 tw=80:
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/. */

#include "mozilla/Casting.h"

#include "jsmath.h"

#include "builtin/AtomicsObject.h"
#include "builtin/intl/Collator.h"
#include "builtin/intl/DateTimeFormat.h"
#include "builtin/intl/NumberFormat.h"
#include "builtin/intl/PluralRules.h"
#include "builtin/intl/RelativeTimeFormat.h"
#include "builtin/MapObject.h"
#include "builtin/String.h"
#include "builtin/TestingFunctions.h"
#include "builtin/TypedObject.h"
#include "jit/BaselineInspector.h"
#include "jit/InlinableNatives.h"
#include "jit/IonBuilder.h"
#include "jit/Lowering.h"
#include "jit/MIR.h"
#include "jit/MIRGraph.h"
#include "js/RegExpFlags.h"  // JS::RegExpFlag, JS::RegExpFlags
#include "vm/ArgumentsObject.h"
#include "vm/ArrayBufferObject.h"
#include "vm/JSObject.h"
#include "vm/ProxyObject.h"
#include "vm/SelfHosting.h"
#include "vm/SharedArrayObject.h"
#include "vm/TypedArrayObject.h"
#include "wasm/WasmInstance.h"

#include "jit/shared/Lowering-shared-inl.h"
#include "vm/JSScript-inl.h"
#include "vm/NativeObject-inl.h"
#include "vm/StringObject-inl.h"

using mozilla::ArrayLength;
using mozilla::AssertedCast;
using mozilla::Maybe;

using JS::RegExpFlag;
using JS::RegExpFlags;
using JS::TrackedOutcome;

namespace js {
namespace jit {

<<<<<<< HEAD
IonBuilder::InliningResult IonBuilder::inlineNativeCall(CallInfo& callInfo,
                                                        JSFunction* target) {
  MOZ_ASSERT(target->isNative());
||||||| merged common ancestors
IonBuilder::InliningResult
IonBuilder::inlineNativeCall(CallInfo& callInfo, JSFunction* target)
{
    MOZ_ASSERT(target->isNative());
=======
// Returns true if |native| can be inlined cross-realm. Especially inlined
// natives that can allocate objects or throw exceptions shouldn't be inlined
// cross-realm without a careful analysis because we might use the wrong realm!
//
// Note that self-hosting intrinsics are never called cross-realm. See the
// MOZ_CRASH below.
//
// If you are adding a new inlinable native, the safe thing is to |return false|
// here.
static bool CanInlineCrossRealm(InlinableNative native) {
  switch (native) {
    case InlinableNative::MathAbs:
    case InlinableNative::MathFloor:
    case InlinableNative::MathCeil:
    case InlinableNative::MathRound:
    case InlinableNative::MathClz32:
    case InlinableNative::MathSqrt:
    case InlinableNative::MathATan2:
    case InlinableNative::MathHypot:
    case InlinableNative::MathMax:
    case InlinableNative::MathMin:
    case InlinableNative::MathPow:
    case InlinableNative::MathImul:
    case InlinableNative::MathFRound:
    case InlinableNative::MathTrunc:
    case InlinableNative::MathSign:
    case InlinableNative::MathSin:
    case InlinableNative::MathTan:
    case InlinableNative::MathCos:
    case InlinableNative::MathExp:
    case InlinableNative::MathLog:
    case InlinableNative::MathASin:
    case InlinableNative::MathATan:
    case InlinableNative::MathACos:
    case InlinableNative::MathLog10:
    case InlinableNative::MathLog2:
    case InlinableNative::MathLog1P:
    case InlinableNative::MathExpM1:
    case InlinableNative::MathCosH:
    case InlinableNative::MathSinH:
    case InlinableNative::MathTanH:
    case InlinableNative::MathACosH:
    case InlinableNative::MathASinH:
    case InlinableNative::MathATanH:
    case InlinableNative::MathCbrt:
    case InlinableNative::Boolean:
      return true;

    case InlinableNative::Array:
      // Cross-realm case handled by inlineArray.
      return true;

    case InlinableNative::MathRandom:
      // RNG state is per-realm.
      return false;

    case InlinableNative::IntlGuardToCollator:
    case InlinableNative::IntlGuardToDateTimeFormat:
    case InlinableNative::IntlGuardToNumberFormat:
    case InlinableNative::IntlGuardToPluralRules:
    case InlinableNative::IntlGuardToRelativeTimeFormat:
    case InlinableNative::IsRegExpObject:
    case InlinableNative::RegExpMatcher:
    case InlinableNative::RegExpSearcher:
    case InlinableNative::RegExpTester:
    case InlinableNative::RegExpPrototypeOptimizable:
    case InlinableNative::RegExpInstanceOptimizable:
    case InlinableNative::GetFirstDollarIndex:
    case InlinableNative::IntrinsicNewArrayIterator:
    case InlinableNative::IntrinsicNewStringIterator:
    case InlinableNative::IntrinsicNewRegExpStringIterator:
    case InlinableNative::IntrinsicStringReplaceString:
    case InlinableNative::IntrinsicStringSplitString:
    case InlinableNative::IntrinsicUnsafeSetReservedSlot:
    case InlinableNative::IntrinsicUnsafeGetReservedSlot:
    case InlinableNative::IntrinsicUnsafeGetObjectFromReservedSlot:
    case InlinableNative::IntrinsicUnsafeGetInt32FromReservedSlot:
    case InlinableNative::IntrinsicUnsafeGetStringFromReservedSlot:
    case InlinableNative::IntrinsicUnsafeGetBooleanFromReservedSlot:
    case InlinableNative::IntrinsicIsCallable:
    case InlinableNative::IntrinsicIsConstructor:
    case InlinableNative::IntrinsicToObject:
    case InlinableNative::IntrinsicIsObject:
    case InlinableNative::IntrinsicIsCrossRealmArrayConstructor:
    case InlinableNative::IntrinsicToInteger:
    case InlinableNative::IntrinsicToString:
    case InlinableNative::IntrinsicIsConstructing:
    case InlinableNative::IntrinsicSubstringKernel:
    case InlinableNative::IntrinsicGuardToArrayIterator:
    case InlinableNative::IntrinsicGuardToMapIterator:
    case InlinableNative::IntrinsicGuardToSetIterator:
    case InlinableNative::IntrinsicGuardToStringIterator:
    case InlinableNative::IntrinsicGuardToRegExpStringIterator:
    case InlinableNative::IntrinsicObjectHasPrototype:
    case InlinableNative::IntrinsicFinishBoundFunctionInit:
    case InlinableNative::IntrinsicIsPackedArray:
    case InlinableNative::IntrinsicGuardToMapObject:
    case InlinableNative::IntrinsicGetNextMapEntryForIterator:
    case InlinableNative::IntrinsicGuardToSetObject:
    case InlinableNative::IntrinsicGetNextSetEntryForIterator:
    case InlinableNative::IntrinsicGuardToArrayBuffer:
    case InlinableNative::IntrinsicArrayBufferByteLength:
    case InlinableNative::IntrinsicPossiblyWrappedArrayBufferByteLength:
    case InlinableNative::IntrinsicGuardToSharedArrayBuffer:
    case InlinableNative::IntrinsicIsTypedArrayConstructor:
    case InlinableNative::IntrinsicIsTypedArray:
    case InlinableNative::IntrinsicIsPossiblyWrappedTypedArray:
    case InlinableNative::IntrinsicPossiblyWrappedTypedArrayLength:
    case InlinableNative::IntrinsicTypedArrayLength:
    case InlinableNative::IntrinsicTypedArrayByteOffset:
    case InlinableNative::IntrinsicTypedArrayElementShift:
    case InlinableNative::IntrinsicSetDisjointTypedElements:
    case InlinableNative::IntrinsicObjectIsTypedObject:
    case InlinableNative::IntrinsicObjectIsTransparentTypedObject:
    case InlinableNative::IntrinsicObjectIsOpaqueTypedObject:
    case InlinableNative::IntrinsicObjectIsTypeDescr:
    case InlinableNative::IntrinsicTypeDescrIsSimpleType:
    case InlinableNative::IntrinsicTypeDescrIsArrayType:
    case InlinableNative::IntrinsicSetTypedObjectOffset:
    case InlinableNative::IntrinsicArrayIteratorPrototypeOptimizable:
      MOZ_CRASH("Unexpected cross-realm intrinsic call");

    case InlinableNative::TestBailout:
    case InlinableNative::TestAssertFloat32:
    case InlinableNative::TestAssertRecoveredOnBailout:
      // Testing functions, not worth inlining cross-realm.
      return false;

    case InlinableNative::ArrayIsArray:
    case InlinableNative::ArrayJoin:
    case InlinableNative::ArrayPop:
    case InlinableNative::ArrayShift:
    case InlinableNative::ArrayPush:
    case InlinableNative::ArraySlice:
    case InlinableNative::AtomicsCompareExchange:
    case InlinableNative::AtomicsExchange:
    case InlinableNative::AtomicsLoad:
    case InlinableNative::AtomicsStore:
    case InlinableNative::AtomicsAdd:
    case InlinableNative::AtomicsSub:
    case InlinableNative::AtomicsAnd:
    case InlinableNative::AtomicsOr:
    case InlinableNative::AtomicsXor:
    case InlinableNative::AtomicsIsLockFree:
    case InlinableNative::ReflectGetPrototypeOf:
    case InlinableNative::String:
    case InlinableNative::StringCharCodeAt:
    case InlinableNative::StringFromCharCode:
    case InlinableNative::StringFromCodePoint:
    case InlinableNative::StringCharAt:
    case InlinableNative::StringToLowerCase:
    case InlinableNative::StringToUpperCase:
    case InlinableNative::Object:
    case InlinableNative::ObjectCreate:
    case InlinableNative::ObjectIs:
    case InlinableNative::ObjectToString:
    case InlinableNative::TypedArrayConstructor:
      // Default to false for most natives.
      return false;

    case InlinableNative::Limit:
      break;
  }
  MOZ_CRASH("Unknown native");
}
>>>>>>> upstream-releases

<<<<<<< HEAD
  if (!optimizationInfo().inlineNative()) {
    trackOptimizationOutcome(TrackedOutcome::CantInlineDisabledIon);
    return InliningStatus_NotInlined;
  }

  bool isWasmCall = target->isWasmOptimized();
  if (!isWasmCall &&
      (!target->hasJitInfo() ||
       target->jitInfo()->type() != JSJitInfo::InlinableNative)) {
    // Reaching here means we tried to inline a native for which there is no
    // Ion specialization.
    trackOptimizationOutcome(TrackedOutcome::CantInlineNativeNoSpecialization);
    return InliningStatus_NotInlined;
  }
||||||| merged common ancestors
    if (!optimizationInfo().inlineNative()) {
        trackOptimizationOutcome(TrackedOutcome::CantInlineDisabledIon);
        return InliningStatus_NotInlined;
    }
=======
IonBuilder::InliningResult IonBuilder::inlineNativeCall(CallInfo& callInfo,
                                                        JSFunction* target) {
  MOZ_ASSERT(target->isNative());
>>>>>>> upstream-releases

<<<<<<< HEAD
  // Don't inline if we're constructing and new.target != callee. This can
  // happen with Reflect.construct or derived class constructors.
  if (callInfo.constructing() && callInfo.getNewTarget() != callInfo.fun()) {
    trackOptimizationOutcome(TrackedOutcome::CantInlineUnexpectedNewTarget);
    return InliningStatus_NotInlined;
  }
||||||| merged common ancestors
    bool isWasmCall = target->isWasmOptimized();
    if (!isWasmCall &&
        (!target->hasJitInfo() ||
        target->jitInfo()->type() != JSJitInfo::InlinableNative))
    {
        // Reaching here means we tried to inline a native for which there is no
        // Ion specialization.
        trackOptimizationOutcome(TrackedOutcome::CantInlineNativeNoSpecialization);
        return InliningStatus_NotInlined;
    }
=======
  if (!optimizationInfo().inlineNative()) {
    trackOptimizationOutcome(TrackedOutcome::CantInlineDisabledIon);
    return InliningStatus_NotInlined;
  }

  bool isWasmCall = target->isWasmWithJitEntry();
  if (!isWasmCall &&
      (!target->hasJitInfo() ||
       target->jitInfo()->type() != JSJitInfo::InlinableNative)) {
    // Reaching here means we tried to inline a native for which there is no
    // Ion specialization.
    trackOptimizationOutcome(TrackedOutcome::CantInlineNativeNoSpecialization);
    return InliningStatus_NotInlined;
  }
>>>>>>> upstream-releases

<<<<<<< HEAD
  // Default failure reason is observing an unsupported type.
  trackOptimizationOutcome(TrackedOutcome::CantInlineNativeBadType);
||||||| merged common ancestors
    // Don't inline if we're constructing and new.target != callee. This can
    // happen with Reflect.construct or derived class constructors.
    if (callInfo.constructing() && callInfo.getNewTarget() != callInfo.fun()) {
        trackOptimizationOutcome(TrackedOutcome::CantInlineUnexpectedNewTarget);
        return InliningStatus_NotInlined;
    }
=======
  // Don't inline if we're constructing and new.target != callee. This can
  // happen with Reflect.construct or derived class constructors.
  if (callInfo.constructing() && callInfo.getNewTarget() != callInfo.fun()) {
    trackOptimizationOutcome(TrackedOutcome::CantInlineUnexpectedNewTarget);
    return InliningStatus_NotInlined;
  }
>>>>>>> upstream-releases

<<<<<<< HEAD
  if (shouldAbortOnPreliminaryGroups(callInfo.thisArg())) {
    return InliningStatus_NotInlined;
  }
  for (size_t i = 0; i < callInfo.argc(); i++) {
    if (shouldAbortOnPreliminaryGroups(callInfo.getArg(i))) {
      return InliningStatus_NotInlined;
    }
  }

  if (isWasmCall) {
    return inlineWasmCall(callInfo, target);
  }

  switch (InlinableNative inlNative = target->jitInfo()->inlinableNative) {
    // Array natives.
    case InlinableNative::Array:
      return inlineArray(callInfo);
    case InlinableNative::ArrayIsArray:
      return inlineArrayIsArray(callInfo);
    case InlinableNative::ArrayJoin:
      return inlineArrayJoin(callInfo);
    case InlinableNative::ArrayPop:
      return inlineArrayPopShift(callInfo, MArrayPopShift::Pop);
    case InlinableNative::ArrayShift:
      return inlineArrayPopShift(callInfo, MArrayPopShift::Shift);
    case InlinableNative::ArrayPush:
      return inlineArrayPush(callInfo);
    case InlinableNative::ArraySlice:
      return inlineArraySlice(callInfo);

    // Array intrinsics.
    case InlinableNative::IntrinsicNewArrayIterator:
      return inlineNewIterator(callInfo, MNewIterator::ArrayIterator);

    // Atomic natives.
    case InlinableNative::AtomicsCompareExchange:
      return inlineAtomicsCompareExchange(callInfo);
    case InlinableNative::AtomicsExchange:
      return inlineAtomicsExchange(callInfo);
    case InlinableNative::AtomicsLoad:
      return inlineAtomicsLoad(callInfo);
    case InlinableNative::AtomicsStore:
      return inlineAtomicsStore(callInfo);
    case InlinableNative::AtomicsAdd:
    case InlinableNative::AtomicsSub:
    case InlinableNative::AtomicsAnd:
    case InlinableNative::AtomicsOr:
    case InlinableNative::AtomicsXor:
      return inlineAtomicsBinop(callInfo, inlNative);
    case InlinableNative::AtomicsIsLockFree:
      return inlineAtomicsIsLockFree(callInfo);

    // Boolean natives.
    case InlinableNative::Boolean:
      return inlineBoolean(callInfo);

    // Intl natives.
    case InlinableNative::IntlGuardToCollator:
      return inlineGuardToClass(callInfo, &CollatorObject::class_);
    case InlinableNative::IntlGuardToDateTimeFormat:
      return inlineGuardToClass(callInfo, &DateTimeFormatObject::class_);
    case InlinableNative::IntlGuardToNumberFormat:
      return inlineGuardToClass(callInfo, &NumberFormatObject::class_);
    case InlinableNative::IntlGuardToPluralRules:
      return inlineGuardToClass(callInfo, &PluralRulesObject::class_);
    case InlinableNative::IntlGuardToRelativeTimeFormat:
      return inlineGuardToClass(callInfo, &RelativeTimeFormatObject::class_);

    // Math natives.
    case InlinableNative::MathAbs:
      return inlineMathAbs(callInfo);
    case InlinableNative::MathFloor:
      return inlineMathFloor(callInfo);
    case InlinableNative::MathCeil:
      return inlineMathCeil(callInfo);
    case InlinableNative::MathRound:
      return inlineMathRound(callInfo);
    case InlinableNative::MathClz32:
      return inlineMathClz32(callInfo);
    case InlinableNative::MathSqrt:
      return inlineMathSqrt(callInfo);
    case InlinableNative::MathATan2:
      return inlineMathAtan2(callInfo);
    case InlinableNative::MathHypot:
      return inlineMathHypot(callInfo);
    case InlinableNative::MathMax:
      return inlineMathMinMax(callInfo, true /* max */);
    case InlinableNative::MathMin:
      return inlineMathMinMax(callInfo, false /* max */);
    case InlinableNative::MathPow:
      return inlineMathPow(callInfo);
    case InlinableNative::MathRandom:
      return inlineMathRandom(callInfo);
    case InlinableNative::MathImul:
      return inlineMathImul(callInfo);
    case InlinableNative::MathFRound:
      return inlineMathFRound(callInfo);
    case InlinableNative::MathTrunc:
      return inlineMathTrunc(callInfo);
    case InlinableNative::MathSign:
      return inlineMathSign(callInfo);
    case InlinableNative::MathSin:
      return inlineMathFunction(callInfo, MMathFunction::Sin);
    case InlinableNative::MathTan:
      return inlineMathFunction(callInfo, MMathFunction::Tan);
    case InlinableNative::MathCos:
      return inlineMathFunction(callInfo, MMathFunction::Cos);
    case InlinableNative::MathExp:
      return inlineMathFunction(callInfo, MMathFunction::Exp);
    case InlinableNative::MathLog:
      return inlineMathFunction(callInfo, MMathFunction::Log);
    case InlinableNative::MathASin:
      return inlineMathFunction(callInfo, MMathFunction::ASin);
    case InlinableNative::MathATan:
      return inlineMathFunction(callInfo, MMathFunction::ATan);
    case InlinableNative::MathACos:
      return inlineMathFunction(callInfo, MMathFunction::ACos);
    case InlinableNative::MathLog10:
      return inlineMathFunction(callInfo, MMathFunction::Log10);
    case InlinableNative::MathLog2:
      return inlineMathFunction(callInfo, MMathFunction::Log2);
    case InlinableNative::MathLog1P:
      return inlineMathFunction(callInfo, MMathFunction::Log1P);
    case InlinableNative::MathExpM1:
      return inlineMathFunction(callInfo, MMathFunction::ExpM1);
    case InlinableNative::MathCosH:
      return inlineMathFunction(callInfo, MMathFunction::CosH);
    case InlinableNative::MathSinH:
      return inlineMathFunction(callInfo, MMathFunction::SinH);
    case InlinableNative::MathTanH:
      return inlineMathFunction(callInfo, MMathFunction::TanH);
    case InlinableNative::MathACosH:
      return inlineMathFunction(callInfo, MMathFunction::ACosH);
    case InlinableNative::MathASinH:
      return inlineMathFunction(callInfo, MMathFunction::ASinH);
    case InlinableNative::MathATanH:
      return inlineMathFunction(callInfo, MMathFunction::ATanH);
    case InlinableNative::MathCbrt:
      return inlineMathFunction(callInfo, MMathFunction::Cbrt);

    // Reflect natives.
    case InlinableNative::ReflectGetPrototypeOf:
      return inlineReflectGetPrototypeOf(callInfo);

    // RegExp natives.
    case InlinableNative::RegExpMatcher:
      return inlineRegExpMatcher(callInfo);
    case InlinableNative::RegExpSearcher:
      return inlineRegExpSearcher(callInfo);
    case InlinableNative::RegExpTester:
      return inlineRegExpTester(callInfo);
    case InlinableNative::IsRegExpObject:
      return inlineIsRegExpObject(callInfo);
    case InlinableNative::RegExpPrototypeOptimizable:
      return inlineRegExpPrototypeOptimizable(callInfo);
    case InlinableNative::RegExpInstanceOptimizable:
      return inlineRegExpInstanceOptimizable(callInfo);
    case InlinableNative::GetFirstDollarIndex:
      return inlineGetFirstDollarIndex(callInfo);

    // String natives.
    case InlinableNative::String:
      return inlineStringObject(callInfo);
    case InlinableNative::StringCharCodeAt:
      return inlineStrCharCodeAt(callInfo);
    case InlinableNative::StringFromCharCode:
      return inlineStrFromCharCode(callInfo);
    case InlinableNative::StringFromCodePoint:
      return inlineStrFromCodePoint(callInfo);
    case InlinableNative::StringCharAt:
      return inlineStrCharAt(callInfo);
    case InlinableNative::StringToLowerCase:
      return inlineStringConvertCase(callInfo, MStringConvertCase::LowerCase);
    case InlinableNative::StringToUpperCase:
      return inlineStringConvertCase(callInfo, MStringConvertCase::UpperCase);

    // String intrinsics.
    case InlinableNative::IntrinsicStringReplaceString:
      return inlineStringReplaceString(callInfo);
    case InlinableNative::IntrinsicStringSplitString:
      return inlineStringSplitString(callInfo);
    case InlinableNative::IntrinsicNewStringIterator:
      return inlineNewIterator(callInfo, MNewIterator::StringIterator);

    // Object natives.
    case InlinableNative::Object:
      return inlineObject(callInfo);
    case InlinableNative::ObjectCreate:
      return inlineObjectCreate(callInfo);
    case InlinableNative::ObjectIs:
      return inlineObjectIs(callInfo);
    case InlinableNative::ObjectToString:
      return inlineObjectToString(callInfo);

    // Testing functions.
    case InlinableNative::TestBailout:
      return inlineBailout(callInfo);
    case InlinableNative::TestAssertFloat32:
      return inlineAssertFloat32(callInfo);
    case InlinableNative::TestAssertRecoveredOnBailout:
      return inlineAssertRecoveredOnBailout(callInfo);

    // Slot intrinsics.
    case InlinableNative::IntrinsicUnsafeSetReservedSlot:
      return inlineUnsafeSetReservedSlot(callInfo);
    case InlinableNative::IntrinsicUnsafeGetReservedSlot:
      return inlineUnsafeGetReservedSlot(callInfo, MIRType::Value);
    case InlinableNative::IntrinsicUnsafeGetObjectFromReservedSlot:
      return inlineUnsafeGetReservedSlot(callInfo, MIRType::Object);
    case InlinableNative::IntrinsicUnsafeGetInt32FromReservedSlot:
      return inlineUnsafeGetReservedSlot(callInfo, MIRType::Int32);
    case InlinableNative::IntrinsicUnsafeGetStringFromReservedSlot:
      return inlineUnsafeGetReservedSlot(callInfo, MIRType::String);
    case InlinableNative::IntrinsicUnsafeGetBooleanFromReservedSlot:
      return inlineUnsafeGetReservedSlot(callInfo, MIRType::Boolean);

    // Utility intrinsics.
    case InlinableNative::IntrinsicIsCallable:
      return inlineIsCallable(callInfo);
    case InlinableNative::IntrinsicIsConstructor:
      return inlineIsConstructor(callInfo);
    case InlinableNative::IntrinsicToObject:
      return inlineToObject(callInfo);
    case InlinableNative::IntrinsicIsObject:
      return inlineIsObject(callInfo);
    case InlinableNative::IntrinsicIsCrossRealmArrayConstructor:
      return inlineIsCrossRealmArrayConstructor(callInfo);
    case InlinableNative::IntrinsicToInteger:
      return inlineToInteger(callInfo);
    case InlinableNative::IntrinsicToString:
      return inlineToString(callInfo);
    case InlinableNative::IntrinsicIsConstructing:
      return inlineIsConstructing(callInfo);
    case InlinableNative::IntrinsicSubstringKernel:
      return inlineSubstringKernel(callInfo);
    case InlinableNative::IntrinsicGuardToArrayIterator:
      return inlineGuardToClass(callInfo, &ArrayIteratorObject::class_);
    case InlinableNative::IntrinsicGuardToMapIterator:
      return inlineGuardToClass(callInfo, &MapIteratorObject::class_);
    case InlinableNative::IntrinsicGuardToSetIterator:
      return inlineGuardToClass(callInfo, &SetIteratorObject::class_);
    case InlinableNative::IntrinsicGuardToStringIterator:
      return inlineGuardToClass(callInfo, &StringIteratorObject::class_);
    case InlinableNative::IntrinsicObjectHasPrototype:
      return inlineObjectHasPrototype(callInfo);
    case InlinableNative::IntrinsicFinishBoundFunctionInit:
      return inlineFinishBoundFunctionInit(callInfo);
    case InlinableNative::IntrinsicIsPackedArray:
      return inlineIsPackedArray(callInfo);

    // Map intrinsics.
    case InlinableNative::IntrinsicGuardToMapObject:
      return inlineGuardToClass(callInfo, &MapObject::class_);
    case InlinableNative::IntrinsicGetNextMapEntryForIterator:
      return inlineGetNextEntryForIterator(callInfo,
                                           MGetNextEntryForIterator::Map);

    // Set intrinsics.
    case InlinableNative::IntrinsicGuardToSetObject:
      return inlineGuardToClass(callInfo, &SetObject::class_);
    case InlinableNative::IntrinsicGetNextSetEntryForIterator:
      return inlineGetNextEntryForIterator(callInfo,
                                           MGetNextEntryForIterator::Set);

    // ArrayBuffer intrinsics.
    case InlinableNative::IntrinsicGuardToArrayBuffer:
      return inlineGuardToClass(callInfo, &ArrayBufferObject::class_);
    case InlinableNative::IntrinsicArrayBufferByteLength:
      return inlineArrayBufferByteLength(callInfo);
    case InlinableNative::IntrinsicPossiblyWrappedArrayBufferByteLength:
      return inlinePossiblyWrappedArrayBufferByteLength(callInfo);

    // SharedArrayBuffer intrinsics.
    case InlinableNative::IntrinsicGuardToSharedArrayBuffer:
      return inlineGuardToClass(callInfo, &SharedArrayBufferObject::class_);

    // TypedArray intrinsics.
    case InlinableNative::TypedArrayConstructor:
      return inlineTypedArray(callInfo, target->native());
    case InlinableNative::IntrinsicIsTypedArray:
      return inlineIsTypedArray(callInfo);
    case InlinableNative::IntrinsicIsPossiblyWrappedTypedArray:
      return inlineIsPossiblyWrappedTypedArray(callInfo);
    case InlinableNative::IntrinsicPossiblyWrappedTypedArrayLength:
      return inlinePossiblyWrappedTypedArrayLength(callInfo);
    case InlinableNative::IntrinsicTypedArrayLength:
      return inlineTypedArrayLength(callInfo);
    case InlinableNative::IntrinsicSetDisjointTypedElements:
      return inlineSetDisjointTypedElements(callInfo);

    // TypedObject intrinsics.
    case InlinableNative::IntrinsicObjectIsTypedObject:
      return inlineHasClass(callInfo, &OutlineTransparentTypedObject::class_,
                            &OutlineOpaqueTypedObject::class_,
                            &InlineTransparentTypedObject::class_,
                            &InlineOpaqueTypedObject::class_);
    case InlinableNative::IntrinsicObjectIsTransparentTypedObject:
      return inlineHasClass(callInfo, &OutlineTransparentTypedObject::class_,
                            &InlineTransparentTypedObject::class_);
    case InlinableNative::IntrinsicObjectIsOpaqueTypedObject:
      return inlineHasClass(callInfo, &OutlineOpaqueTypedObject::class_,
                            &InlineOpaqueTypedObject::class_);
    case InlinableNative::IntrinsicObjectIsTypeDescr:
      return inlineObjectIsTypeDescr(callInfo);
    case InlinableNative::IntrinsicTypeDescrIsSimpleType:
      return inlineHasClass(callInfo, &ScalarTypeDescr::class_,
                            &ReferenceTypeDescr::class_);
    case InlinableNative::IntrinsicTypeDescrIsArrayType:
      return inlineHasClass(callInfo, &ArrayTypeDescr::class_);
    case InlinableNative::IntrinsicSetTypedObjectOffset:
      return inlineSetTypedObjectOffset(callInfo);
    case InlinableNative::Limit:
      break;
  }

  MOZ_CRASH("Shouldn't get here");
}

IonBuilder::InliningResult IonBuilder::inlineNativeGetter(CallInfo& callInfo,
                                                          JSFunction* target) {
  MOZ_ASSERT(target->isNative());
  JSNative native = target->native();

  if (!optimizationInfo().inlineNative()) {
    return InliningStatus_NotInlined;
  }
||||||| merged common ancestors
    // Default failure reason is observing an unsupported type.
    trackOptimizationOutcome(TrackedOutcome::CantInlineNativeBadType);
=======
  // Default failure reason is observing an unsupported type.
  trackOptimizationOutcome(TrackedOutcome::CantInlineNativeBadType);
>>>>>>> upstream-releases

<<<<<<< HEAD
  MDefinition* thisArg = callInfo.thisArg();
  TemporaryTypeSet* thisTypes = thisArg->resultTypeSet();
  MOZ_ASSERT(callInfo.argc() == 0);

  if (!thisTypes) {
    return InliningStatus_NotInlined;
  }
||||||| merged common ancestors
    if (shouldAbortOnPreliminaryGroups(callInfo.thisArg())) {
        return InliningStatus_NotInlined;
    }
    for (size_t i = 0; i < callInfo.argc(); i++) {
        if (shouldAbortOnPreliminaryGroups(callInfo.getArg(i))) {
            return InliningStatus_NotInlined;
        }
    }
=======
  if (shouldAbortOnPreliminaryGroups(callInfo.thisArg())) {
    return InliningStatus_NotInlined;
  }
  for (size_t i = 0; i < callInfo.argc(); i++) {
    if (shouldAbortOnPreliminaryGroups(callInfo.getArg(i))) {
      return InliningStatus_NotInlined;
    }
  }
>>>>>>> upstream-releases

<<<<<<< HEAD
  // Try to optimize typed array lengths.
  if (TypedArrayObject::isOriginalLengthGetter(native)) {
    Scalar::Type type = thisTypes->getTypedArrayType(constraints());
    if (type == Scalar::MaxTypedArrayViewType) {
      return InliningStatus_NotInlined;
    }
||||||| merged common ancestors
    if (isWasmCall) {
        return inlineWasmCall(callInfo, target);
    }
=======
  if (isWasmCall) {
    return inlineWasmCall(callInfo, target);
  }
>>>>>>> upstream-releases

<<<<<<< HEAD
    MInstruction* length = addTypedArrayLength(thisArg);
    current->push(length);
    return InliningStatus_Inlined;
  }

  // Try to optimize RegExp getters.
  RegExpFlag mask = NoFlags;
  if (RegExpObject::isOriginalFlagGetter(native, &mask)) {
    const Class* clasp = thisTypes->getKnownClass(constraints());
    if (clasp != &RegExpObject::class_) {
      return InliningStatus_NotInlined;
    }
||||||| merged common ancestors
    switch (InlinableNative inlNative = target->jitInfo()->inlinableNative) {
      // Array natives.
      case InlinableNative::Array:
        return inlineArray(callInfo);
      case InlinableNative::ArrayIsArray:
        return inlineArrayIsArray(callInfo);
      case InlinableNative::ArrayJoin:
        return inlineArrayJoin(callInfo);
      case InlinableNative::ArrayPop:
        return inlineArrayPopShift(callInfo, MArrayPopShift::Pop);
      case InlinableNative::ArrayShift:
        return inlineArrayPopShift(callInfo, MArrayPopShift::Shift);
      case InlinableNative::ArrayPush:
        return inlineArrayPush(callInfo);
      case InlinableNative::ArraySlice:
        return inlineArraySlice(callInfo);

      // Array intrinsics.
      case InlinableNative::IntrinsicNewArrayIterator:
        return inlineNewIterator(callInfo, MNewIterator::ArrayIterator);

      // Atomic natives.
      case InlinableNative::AtomicsCompareExchange:
        return inlineAtomicsCompareExchange(callInfo);
      case InlinableNative::AtomicsExchange:
        return inlineAtomicsExchange(callInfo);
      case InlinableNative::AtomicsLoad:
        return inlineAtomicsLoad(callInfo);
      case InlinableNative::AtomicsStore:
        return inlineAtomicsStore(callInfo);
      case InlinableNative::AtomicsAdd:
      case InlinableNative::AtomicsSub:
      case InlinableNative::AtomicsAnd:
      case InlinableNative::AtomicsOr:
      case InlinableNative::AtomicsXor:
        return inlineAtomicsBinop(callInfo, inlNative);
      case InlinableNative::AtomicsIsLockFree:
        return inlineAtomicsIsLockFree(callInfo);

      // Boolean natives.
      case InlinableNative::Boolean:
        return inlineBoolean(callInfo);

      // Intl natives.
      case InlinableNative::IntlGuardToCollator:
        return inlineGuardToClass(callInfo, &CollatorObject::class_);
      case InlinableNative::IntlGuardToDateTimeFormat:
        return inlineGuardToClass(callInfo, &DateTimeFormatObject::class_);
      case InlinableNative::IntlGuardToNumberFormat:
        return inlineGuardToClass(callInfo, &NumberFormatObject::class_);
      case InlinableNative::IntlGuardToPluralRules:
        return inlineGuardToClass(callInfo, &PluralRulesObject::class_);
      case InlinableNative::IntlGuardToRelativeTimeFormat:
        return inlineGuardToClass(callInfo, &RelativeTimeFormatObject::class_);

      // Math natives.
      case InlinableNative::MathAbs:
        return inlineMathAbs(callInfo);
      case InlinableNative::MathFloor:
        return inlineMathFloor(callInfo);
      case InlinableNative::MathCeil:
        return inlineMathCeil(callInfo);
      case InlinableNative::MathRound:
        return inlineMathRound(callInfo);
      case InlinableNative::MathClz32:
        return inlineMathClz32(callInfo);
      case InlinableNative::MathSqrt:
        return inlineMathSqrt(callInfo);
      case InlinableNative::MathATan2:
        return inlineMathAtan2(callInfo);
      case InlinableNative::MathHypot:
        return inlineMathHypot(callInfo);
      case InlinableNative::MathMax:
        return inlineMathMinMax(callInfo, true /* max */);
      case InlinableNative::MathMin:
        return inlineMathMinMax(callInfo, false /* max */);
      case InlinableNative::MathPow:
        return inlineMathPow(callInfo);
      case InlinableNative::MathRandom:
        return inlineMathRandom(callInfo);
      case InlinableNative::MathImul:
        return inlineMathImul(callInfo);
      case InlinableNative::MathFRound:
        return inlineMathFRound(callInfo);
      case InlinableNative::MathTrunc:
        return inlineMathTrunc(callInfo);
      case InlinableNative::MathSign:
        return inlineMathSign(callInfo);
      case InlinableNative::MathSin:
        return inlineMathFunction(callInfo, MMathFunction::Sin);
      case InlinableNative::MathTan:
        return inlineMathFunction(callInfo, MMathFunction::Tan);
      case InlinableNative::MathCos:
        return inlineMathFunction(callInfo, MMathFunction::Cos);
      case InlinableNative::MathExp:
        return inlineMathFunction(callInfo, MMathFunction::Exp);
      case InlinableNative::MathLog:
        return inlineMathFunction(callInfo, MMathFunction::Log);
      case InlinableNative::MathASin:
        return inlineMathFunction(callInfo, MMathFunction::ASin);
      case InlinableNative::MathATan:
        return inlineMathFunction(callInfo, MMathFunction::ATan);
      case InlinableNative::MathACos:
        return inlineMathFunction(callInfo, MMathFunction::ACos);
      case InlinableNative::MathLog10:
        return inlineMathFunction(callInfo, MMathFunction::Log10);
      case InlinableNative::MathLog2:
        return inlineMathFunction(callInfo, MMathFunction::Log2);
      case InlinableNative::MathLog1P:
        return inlineMathFunction(callInfo, MMathFunction::Log1P);
      case InlinableNative::MathExpM1:
        return inlineMathFunction(callInfo, MMathFunction::ExpM1);
      case InlinableNative::MathCosH:
        return inlineMathFunction(callInfo, MMathFunction::CosH);
      case InlinableNative::MathSinH:
        return inlineMathFunction(callInfo, MMathFunction::SinH);
      case InlinableNative::MathTanH:
        return inlineMathFunction(callInfo, MMathFunction::TanH);
      case InlinableNative::MathACosH:
        return inlineMathFunction(callInfo, MMathFunction::ACosH);
      case InlinableNative::MathASinH:
        return inlineMathFunction(callInfo, MMathFunction::ASinH);
      case InlinableNative::MathATanH:
        return inlineMathFunction(callInfo, MMathFunction::ATanH);
      case InlinableNative::MathCbrt:
        return inlineMathFunction(callInfo, MMathFunction::Cbrt);

      // Reflect natives.
      case InlinableNative::ReflectGetPrototypeOf:
        return inlineReflectGetPrototypeOf(callInfo);

      // RegExp natives.
      case InlinableNative::RegExpMatcher:
        return inlineRegExpMatcher(callInfo);
      case InlinableNative::RegExpSearcher:
        return inlineRegExpSearcher(callInfo);
      case InlinableNative::RegExpTester:
        return inlineRegExpTester(callInfo);
      case InlinableNative::IsRegExpObject:
        return inlineIsRegExpObject(callInfo);
      case InlinableNative::RegExpPrototypeOptimizable:
        return inlineRegExpPrototypeOptimizable(callInfo);
      case InlinableNative::RegExpInstanceOptimizable:
        return inlineRegExpInstanceOptimizable(callInfo);
      case InlinableNative::GetFirstDollarIndex:
        return inlineGetFirstDollarIndex(callInfo);

      // String natives.
      case InlinableNative::String:
        return inlineStringObject(callInfo);
      case InlinableNative::StringCharCodeAt:
        return inlineStrCharCodeAt(callInfo);
      case InlinableNative::StringFromCharCode:
        return inlineStrFromCharCode(callInfo);
      case InlinableNative::StringFromCodePoint:
        return inlineStrFromCodePoint(callInfo);
      case InlinableNative::StringCharAt:
        return inlineStrCharAt(callInfo);
      case InlinableNative::StringToLowerCase:
        return inlineStringConvertCase(callInfo, MStringConvertCase::LowerCase);
      case InlinableNative::StringToUpperCase:
        return inlineStringConvertCase(callInfo, MStringConvertCase::UpperCase);

      // String intrinsics.
      case InlinableNative::IntrinsicStringReplaceString:
        return inlineStringReplaceString(callInfo);
      case InlinableNative::IntrinsicStringSplitString:
        return inlineStringSplitString(callInfo);
      case InlinableNative::IntrinsicNewStringIterator:
        return inlineNewIterator(callInfo, MNewIterator::StringIterator);

      // Object natives.
      case InlinableNative::Object:
        return inlineObject(callInfo);
      case InlinableNative::ObjectCreate:
        return inlineObjectCreate(callInfo);
      case InlinableNative::ObjectIs:
        return inlineObjectIs(callInfo);
      case InlinableNative::ObjectToString:
        return inlineObjectToString(callInfo);

      // Testing functions.
      case InlinableNative::TestBailout:
        return inlineBailout(callInfo);
      case InlinableNative::TestAssertFloat32:
        return inlineAssertFloat32(callInfo);
      case InlinableNative::TestAssertRecoveredOnBailout:
        return inlineAssertRecoveredOnBailout(callInfo);

      // Slot intrinsics.
      case InlinableNative::IntrinsicUnsafeSetReservedSlot:
        return inlineUnsafeSetReservedSlot(callInfo);
      case InlinableNative::IntrinsicUnsafeGetReservedSlot:
        return inlineUnsafeGetReservedSlot(callInfo, MIRType::Value);
      case InlinableNative::IntrinsicUnsafeGetObjectFromReservedSlot:
        return inlineUnsafeGetReservedSlot(callInfo, MIRType::Object);
      case InlinableNative::IntrinsicUnsafeGetInt32FromReservedSlot:
        return inlineUnsafeGetReservedSlot(callInfo, MIRType::Int32);
      case InlinableNative::IntrinsicUnsafeGetStringFromReservedSlot:
        return inlineUnsafeGetReservedSlot(callInfo, MIRType::String);
      case InlinableNative::IntrinsicUnsafeGetBooleanFromReservedSlot:
        return inlineUnsafeGetReservedSlot(callInfo, MIRType::Boolean);

      // Utility intrinsics.
      case InlinableNative::IntrinsicIsCallable:
        return inlineIsCallable(callInfo);
      case InlinableNative::IntrinsicIsConstructor:
        return inlineIsConstructor(callInfo);
      case InlinableNative::IntrinsicToObject:
        return inlineToObject(callInfo);
      case InlinableNative::IntrinsicIsObject:
        return inlineIsObject(callInfo);
      case InlinableNative::IntrinsicIsCrossRealmArrayConstructor:
        return inlineIsCrossRealmArrayConstructor(callInfo);
      case InlinableNative::IntrinsicToInteger:
        return inlineToInteger(callInfo);
      case InlinableNative::IntrinsicToString:
        return inlineToString(callInfo);
      case InlinableNative::IntrinsicIsConstructing:
        return inlineIsConstructing(callInfo);
      case InlinableNative::IntrinsicSubstringKernel:
        return inlineSubstringKernel(callInfo);
      case InlinableNative::IntrinsicGuardToArrayIterator:
        return inlineGuardToClass(callInfo, &ArrayIteratorObject::class_);
      case InlinableNative::IntrinsicGuardToMapIterator:
        return inlineGuardToClass(callInfo, &MapIteratorObject::class_);
      case InlinableNative::IntrinsicGuardToSetIterator:
        return inlineGuardToClass(callInfo, &SetIteratorObject::class_);
      case InlinableNative::IntrinsicGuardToStringIterator:
        return inlineGuardToClass(callInfo, &StringIteratorObject::class_);
      case InlinableNative::IntrinsicObjectHasPrototype:
        return inlineObjectHasPrototype(callInfo);
      case InlinableNative::IntrinsicFinishBoundFunctionInit:
        return inlineFinishBoundFunctionInit(callInfo);
      case InlinableNative::IntrinsicIsPackedArray:
        return inlineIsPackedArray(callInfo);

      // Map intrinsics.
      case InlinableNative::IntrinsicGuardToMapObject:
        return inlineGuardToClass(callInfo, &MapObject::class_);
      case InlinableNative::IntrinsicGetNextMapEntryForIterator:
        return inlineGetNextEntryForIterator(callInfo, MGetNextEntryForIterator::Map);

      // Set intrinsics.
      case InlinableNative::IntrinsicGuardToSetObject:
        return inlineGuardToClass(callInfo, &SetObject::class_);
      case InlinableNative::IntrinsicGetNextSetEntryForIterator:
        return inlineGetNextEntryForIterator(callInfo, MGetNextEntryForIterator::Set);

      // ArrayBuffer intrinsics.
      case InlinableNative::IntrinsicGuardToArrayBuffer:
        return inlineGuardToClass(callInfo, &ArrayBufferObject::class_);
      case InlinableNative::IntrinsicArrayBufferByteLength:
        return inlineArrayBufferByteLength(callInfo);
      case InlinableNative::IntrinsicPossiblyWrappedArrayBufferByteLength:
        return inlinePossiblyWrappedArrayBufferByteLength(callInfo);

      // SharedArrayBuffer intrinsics.
      case InlinableNative::IntrinsicGuardToSharedArrayBuffer:
        return inlineGuardToClass(callInfo, &SharedArrayBufferObject::class_);

      // TypedArray intrinsics.
      case InlinableNative::TypedArrayConstructor:
        return inlineTypedArray(callInfo, target->native());
      case InlinableNative::IntrinsicIsTypedArray:
        return inlineIsTypedArray(callInfo);
      case InlinableNative::IntrinsicIsPossiblyWrappedTypedArray:
        return inlineIsPossiblyWrappedTypedArray(callInfo);
      case InlinableNative::IntrinsicPossiblyWrappedTypedArrayLength:
        return inlinePossiblyWrappedTypedArrayLength(callInfo);
      case InlinableNative::IntrinsicTypedArrayLength:
        return inlineTypedArrayLength(callInfo);
      case InlinableNative::IntrinsicSetDisjointTypedElements:
        return inlineSetDisjointTypedElements(callInfo);

      // TypedObject intrinsics.
      case InlinableNative::IntrinsicObjectIsTypedObject:
        return inlineHasClass(callInfo,
                              &OutlineTransparentTypedObject::class_,
                              &OutlineOpaqueTypedObject::class_,
                              &InlineTransparentTypedObject::class_,
                              &InlineOpaqueTypedObject::class_);
      case InlinableNative::IntrinsicObjectIsTransparentTypedObject:
        return inlineHasClass(callInfo,
                              &OutlineTransparentTypedObject::class_,
                              &InlineTransparentTypedObject::class_);
      case InlinableNative::IntrinsicObjectIsOpaqueTypedObject:
        return inlineHasClass(callInfo,
                              &OutlineOpaqueTypedObject::class_,
                              &InlineOpaqueTypedObject::class_);
      case InlinableNative::IntrinsicObjectIsTypeDescr:
        return inlineObjectIsTypeDescr(callInfo);
      case InlinableNative::IntrinsicTypeDescrIsSimpleType:
        return inlineHasClass(callInfo,
                              &ScalarTypeDescr::class_, &ReferenceTypeDescr::class_);
      case InlinableNative::IntrinsicTypeDescrIsArrayType:
        return inlineHasClass(callInfo, &ArrayTypeDescr::class_);
      case InlinableNative::IntrinsicSetTypedObjectOffset:
        return inlineSetTypedObjectOffset(callInfo);
      case InlinableNative::Limit:
        break;
    }
=======
  InlinableNative inlNative = target->jitInfo()->inlinableNative;
>>>>>>> upstream-releases

<<<<<<< HEAD
    MLoadFixedSlot* flags =
        MLoadFixedSlot::New(alloc(), thisArg, RegExpObject::flagsSlot());
    current->add(flags);
    flags->setResultType(MIRType::Int32);
    MConstant* maskConst = MConstant::New(alloc(), Int32Value(mask));
    current->add(maskConst);
    MBitAnd* maskedFlag = MBitAnd::New(alloc(), flags, maskConst);
    maskedFlag->setInt32Specialization();
    current->add(maskedFlag);

    MDefinition* result = convertToBoolean(maskedFlag);
    current->push(result);
    return InliningStatus_Inlined;
  }

  return InliningStatus_NotInlined;
||||||| merged common ancestors
    MOZ_CRASH("Shouldn't get here");
=======
  if (target->realm() != script()->realm() && !CanInlineCrossRealm(inlNative)) {
    trackOptimizationOutcome(TrackedOutcome::CantInlineCrossRealm);
    return InliningStatus_NotInlined;
  }

  switch (inlNative) {
    // Array natives.
    case InlinableNative::Array:
      return inlineArray(callInfo, target->realm());
    case InlinableNative::ArrayIsArray:
      return inlineArrayIsArray(callInfo);
    case InlinableNative::ArrayJoin:
      return inlineArrayJoin(callInfo);
    case InlinableNative::ArrayPop:
      return inlineArrayPopShift(callInfo, MArrayPopShift::Pop);
    case InlinableNative::ArrayShift:
      return inlineArrayPopShift(callInfo, MArrayPopShift::Shift);
    case InlinableNative::ArrayPush:
      return inlineArrayPush(callInfo);
    case InlinableNative::ArraySlice:
      return inlineArraySlice(callInfo);

    // Array intrinsics.
    case InlinableNative::IntrinsicNewArrayIterator:
      return inlineNewIterator(callInfo, MNewIterator::ArrayIterator);
    case InlinableNative::IntrinsicArrayIteratorPrototypeOptimizable:
      return inlineArrayIteratorPrototypeOptimizable(callInfo);

    // Atomic natives.
    case InlinableNative::AtomicsCompareExchange:
      return inlineAtomicsCompareExchange(callInfo);
    case InlinableNative::AtomicsExchange:
      return inlineAtomicsExchange(callInfo);
    case InlinableNative::AtomicsLoad:
      return inlineAtomicsLoad(callInfo);
    case InlinableNative::AtomicsStore:
      return inlineAtomicsStore(callInfo);
    case InlinableNative::AtomicsAdd:
    case InlinableNative::AtomicsSub:
    case InlinableNative::AtomicsAnd:
    case InlinableNative::AtomicsOr:
    case InlinableNative::AtomicsXor:
      return inlineAtomicsBinop(callInfo, inlNative);
    case InlinableNative::AtomicsIsLockFree:
      return inlineAtomicsIsLockFree(callInfo);

    // Boolean natives.
    case InlinableNative::Boolean:
      return inlineBoolean(callInfo);

    // Intl natives.
    case InlinableNative::IntlGuardToCollator:
      return inlineGuardToClass(callInfo, &CollatorObject::class_);
    case InlinableNative::IntlGuardToDateTimeFormat:
      return inlineGuardToClass(callInfo, &DateTimeFormatObject::class_);
    case InlinableNative::IntlGuardToNumberFormat:
      return inlineGuardToClass(callInfo, &NumberFormatObject::class_);
    case InlinableNative::IntlGuardToPluralRules:
      return inlineGuardToClass(callInfo, &PluralRulesObject::class_);
    case InlinableNative::IntlGuardToRelativeTimeFormat:
      return inlineGuardToClass(callInfo, &RelativeTimeFormatObject::class_);

    // Math natives.
    case InlinableNative::MathAbs:
      return inlineMathAbs(callInfo);
    case InlinableNative::MathFloor:
      return inlineMathFloor(callInfo);
    case InlinableNative::MathCeil:
      return inlineMathCeil(callInfo);
    case InlinableNative::MathRound:
      return inlineMathRound(callInfo);
    case InlinableNative::MathClz32:
      return inlineMathClz32(callInfo);
    case InlinableNative::MathSqrt:
      return inlineMathSqrt(callInfo);
    case InlinableNative::MathATan2:
      return inlineMathAtan2(callInfo);
    case InlinableNative::MathHypot:
      return inlineMathHypot(callInfo);
    case InlinableNative::MathMax:
      return inlineMathMinMax(callInfo, true /* max */);
    case InlinableNative::MathMin:
      return inlineMathMinMax(callInfo, false /* max */);
    case InlinableNative::MathPow:
      return inlineMathPow(callInfo);
    case InlinableNative::MathRandom:
      return inlineMathRandom(callInfo);
    case InlinableNative::MathImul:
      return inlineMathImul(callInfo);
    case InlinableNative::MathFRound:
      return inlineMathFRound(callInfo);
    case InlinableNative::MathTrunc:
      return inlineMathTrunc(callInfo);
    case InlinableNative::MathSign:
      return inlineMathSign(callInfo);
    case InlinableNative::MathSin:
      return inlineMathFunction(callInfo, MMathFunction::Sin);
    case InlinableNative::MathTan:
      return inlineMathFunction(callInfo, MMathFunction::Tan);
    case InlinableNative::MathCos:
      return inlineMathFunction(callInfo, MMathFunction::Cos);
    case InlinableNative::MathExp:
      return inlineMathFunction(callInfo, MMathFunction::Exp);
    case InlinableNative::MathLog:
      return inlineMathFunction(callInfo, MMathFunction::Log);
    case InlinableNative::MathASin:
      return inlineMathFunction(callInfo, MMathFunction::ASin);
    case InlinableNative::MathATan:
      return inlineMathFunction(callInfo, MMathFunction::ATan);
    case InlinableNative::MathACos:
      return inlineMathFunction(callInfo, MMathFunction::ACos);
    case InlinableNative::MathLog10:
      return inlineMathFunction(callInfo, MMathFunction::Log10);
    case InlinableNative::MathLog2:
      return inlineMathFunction(callInfo, MMathFunction::Log2);
    case InlinableNative::MathLog1P:
      return inlineMathFunction(callInfo, MMathFunction::Log1P);
    case InlinableNative::MathExpM1:
      return inlineMathFunction(callInfo, MMathFunction::ExpM1);
    case InlinableNative::MathCosH:
      return inlineMathFunction(callInfo, MMathFunction::CosH);
    case InlinableNative::MathSinH:
      return inlineMathFunction(callInfo, MMathFunction::SinH);
    case InlinableNative::MathTanH:
      return inlineMathFunction(callInfo, MMathFunction::TanH);
    case InlinableNative::MathACosH:
      return inlineMathFunction(callInfo, MMathFunction::ACosH);
    case InlinableNative::MathASinH:
      return inlineMathFunction(callInfo, MMathFunction::ASinH);
    case InlinableNative::MathATanH:
      return inlineMathFunction(callInfo, MMathFunction::ATanH);
    case InlinableNative::MathCbrt:
      return inlineMathFunction(callInfo, MMathFunction::Cbrt);

    // Reflect natives.
    case InlinableNative::ReflectGetPrototypeOf:
      return inlineReflectGetPrototypeOf(callInfo);

    // RegExp natives.
    case InlinableNative::RegExpMatcher:
      return inlineRegExpMatcher(callInfo);
    case InlinableNative::RegExpSearcher:
      return inlineRegExpSearcher(callInfo);
    case InlinableNative::RegExpTester:
      return inlineRegExpTester(callInfo);
    case InlinableNative::IsRegExpObject:
      return inlineIsRegExpObject(callInfo);
    case InlinableNative::RegExpPrototypeOptimizable:
      return inlineRegExpPrototypeOptimizable(callInfo);
    case InlinableNative::RegExpInstanceOptimizable:
      return inlineRegExpInstanceOptimizable(callInfo);
    case InlinableNative::GetFirstDollarIndex:
      return inlineGetFirstDollarIndex(callInfo);
    case InlinableNative::IntrinsicNewRegExpStringIterator:
      return inlineNewIterator(callInfo, MNewIterator::RegExpStringIterator);

    // String natives.
    case InlinableNative::String:
      return inlineStringObject(callInfo);
    case InlinableNative::StringCharCodeAt:
      return inlineStrCharCodeAt(callInfo);
    case InlinableNative::StringFromCharCode:
      return inlineStrFromCharCode(callInfo);
    case InlinableNative::StringFromCodePoint:
      return inlineStrFromCodePoint(callInfo);
    case InlinableNative::StringCharAt:
      return inlineStrCharAt(callInfo);
    case InlinableNative::StringToLowerCase:
      return inlineStringConvertCase(callInfo, MStringConvertCase::LowerCase);
    case InlinableNative::StringToUpperCase:
      return inlineStringConvertCase(callInfo, MStringConvertCase::UpperCase);

    // String intrinsics.
    case InlinableNative::IntrinsicStringReplaceString:
      return inlineStringReplaceString(callInfo);
    case InlinableNative::IntrinsicStringSplitString:
      return inlineStringSplitString(callInfo);
    case InlinableNative::IntrinsicNewStringIterator:
      return inlineNewIterator(callInfo, MNewIterator::StringIterator);

    // Object natives.
    case InlinableNative::Object:
      return inlineObject(callInfo);
    case InlinableNative::ObjectCreate:
      return inlineObjectCreate(callInfo);
    case InlinableNative::ObjectIs:
      return inlineObjectIs(callInfo);
    case InlinableNative::ObjectToString:
      return inlineObjectToString(callInfo);

    // Testing functions.
    case InlinableNative::TestBailout:
      return inlineBailout(callInfo);
    case InlinableNative::TestAssertFloat32:
      return inlineAssertFloat32(callInfo);
    case InlinableNative::TestAssertRecoveredOnBailout:
      return inlineAssertRecoveredOnBailout(callInfo);

    // Slot intrinsics.
    case InlinableNative::IntrinsicUnsafeSetReservedSlot:
      return inlineUnsafeSetReservedSlot(callInfo);
    case InlinableNative::IntrinsicUnsafeGetReservedSlot:
      return inlineUnsafeGetReservedSlot(callInfo, MIRType::Value);
    case InlinableNative::IntrinsicUnsafeGetObjectFromReservedSlot:
      return inlineUnsafeGetReservedSlot(callInfo, MIRType::Object);
    case InlinableNative::IntrinsicUnsafeGetInt32FromReservedSlot:
      return inlineUnsafeGetReservedSlot(callInfo, MIRType::Int32);
    case InlinableNative::IntrinsicUnsafeGetStringFromReservedSlot:
      return inlineUnsafeGetReservedSlot(callInfo, MIRType::String);
    case InlinableNative::IntrinsicUnsafeGetBooleanFromReservedSlot:
      return inlineUnsafeGetReservedSlot(callInfo, MIRType::Boolean);

    // Utility intrinsics.
    case InlinableNative::IntrinsicIsCallable:
      return inlineIsCallable(callInfo);
    case InlinableNative::IntrinsicIsConstructor:
      return inlineIsConstructor(callInfo);
    case InlinableNative::IntrinsicToObject:
      return inlineToObject(callInfo);
    case InlinableNative::IntrinsicIsObject:
      return inlineIsObject(callInfo);
    case InlinableNative::IntrinsicIsCrossRealmArrayConstructor:
      return inlineIsCrossRealmArrayConstructor(callInfo);
    case InlinableNative::IntrinsicToInteger:
      return inlineToInteger(callInfo);
    case InlinableNative::IntrinsicToString:
      return inlineToString(callInfo);
    case InlinableNative::IntrinsicIsConstructing:
      return inlineIsConstructing(callInfo);
    case InlinableNative::IntrinsicSubstringKernel:
      return inlineSubstringKernel(callInfo);
    case InlinableNative::IntrinsicGuardToArrayIterator:
      return inlineGuardToClass(callInfo, &ArrayIteratorObject::class_);
    case InlinableNative::IntrinsicGuardToMapIterator:
      return inlineGuardToClass(callInfo, &MapIteratorObject::class_);
    case InlinableNative::IntrinsicGuardToSetIterator:
      return inlineGuardToClass(callInfo, &SetIteratorObject::class_);
    case InlinableNative::IntrinsicGuardToStringIterator:
      return inlineGuardToClass(callInfo, &StringIteratorObject::class_);
    case InlinableNative::IntrinsicGuardToRegExpStringIterator:
      return inlineGuardToClass(callInfo, &RegExpStringIteratorObject::class_);
    case InlinableNative::IntrinsicObjectHasPrototype:
      return inlineObjectHasPrototype(callInfo);
    case InlinableNative::IntrinsicFinishBoundFunctionInit:
      return inlineFinishBoundFunctionInit(callInfo);
    case InlinableNative::IntrinsicIsPackedArray:
      return inlineIsPackedArray(callInfo);

    // Map intrinsics.
    case InlinableNative::IntrinsicGuardToMapObject:
      return inlineGuardToClass(callInfo, &MapObject::class_);
    case InlinableNative::IntrinsicGetNextMapEntryForIterator:
      return inlineGetNextEntryForIterator(callInfo,
                                           MGetNextEntryForIterator::Map);

    // Set intrinsics.
    case InlinableNative::IntrinsicGuardToSetObject:
      return inlineGuardToClass(callInfo, &SetObject::class_);
    case InlinableNative::IntrinsicGetNextSetEntryForIterator:
      return inlineGetNextEntryForIterator(callInfo,
                                           MGetNextEntryForIterator::Set);

    // ArrayBuffer intrinsics.
    case InlinableNative::IntrinsicGuardToArrayBuffer:
      return inlineGuardToClass(callInfo, &ArrayBufferObject::class_);
    case InlinableNative::IntrinsicArrayBufferByteLength:
      return inlineArrayBufferByteLength(callInfo);
    case InlinableNative::IntrinsicPossiblyWrappedArrayBufferByteLength:
      return inlinePossiblyWrappedArrayBufferByteLength(callInfo);

    // SharedArrayBuffer intrinsics.
    case InlinableNative::IntrinsicGuardToSharedArrayBuffer:
      return inlineGuardToClass(callInfo, &SharedArrayBufferObject::class_);

    // TypedArray intrinsics.
    case InlinableNative::TypedArrayConstructor:
      return inlineTypedArray(callInfo, target->native());
    case InlinableNative::IntrinsicIsTypedArrayConstructor:
      return inlineIsTypedArrayConstructor(callInfo);
    case InlinableNative::IntrinsicIsTypedArray:
      return inlineIsTypedArray(callInfo);
    case InlinableNative::IntrinsicIsPossiblyWrappedTypedArray:
      return inlineIsPossiblyWrappedTypedArray(callInfo);
    case InlinableNative::IntrinsicPossiblyWrappedTypedArrayLength:
      return inlinePossiblyWrappedTypedArrayLength(callInfo);
    case InlinableNative::IntrinsicTypedArrayLength:
      return inlineTypedArrayLength(callInfo);
    case InlinableNative::IntrinsicTypedArrayByteOffset:
      return inlineTypedArrayByteOffset(callInfo);
    case InlinableNative::IntrinsicTypedArrayElementShift:
      return inlineTypedArrayElementShift(callInfo);
    case InlinableNative::IntrinsicSetDisjointTypedElements:
      return inlineSetDisjointTypedElements(callInfo);

    // TypedObject intrinsics.
    case InlinableNative::IntrinsicObjectIsTypedObject:
      return inlineHasClass(callInfo, &OutlineTransparentTypedObject::class_,
                            &OutlineOpaqueTypedObject::class_,
                            &InlineTransparentTypedObject::class_,
                            &InlineOpaqueTypedObject::class_);
    case InlinableNative::IntrinsicObjectIsTransparentTypedObject:
      return inlineHasClass(callInfo, &OutlineTransparentTypedObject::class_,
                            &InlineTransparentTypedObject::class_);
    case InlinableNative::IntrinsicObjectIsOpaqueTypedObject:
      return inlineHasClass(callInfo, &OutlineOpaqueTypedObject::class_,
                            &InlineOpaqueTypedObject::class_);
    case InlinableNative::IntrinsicObjectIsTypeDescr:
      return inlineObjectIsTypeDescr(callInfo);
    case InlinableNative::IntrinsicTypeDescrIsSimpleType:
      return inlineHasClass(callInfo, &ScalarTypeDescr::class_,
                            &ReferenceTypeDescr::class_);
    case InlinableNative::IntrinsicTypeDescrIsArrayType:
      return inlineHasClass(callInfo, &ArrayTypeDescr::class_);
    case InlinableNative::IntrinsicSetTypedObjectOffset:
      return inlineSetTypedObjectOffset(callInfo);
    case InlinableNative::Limit:
      break;
  }

  MOZ_CRASH("Shouldn't get here");
>>>>>>> upstream-releases
}

<<<<<<< HEAD
IonBuilder::InliningResult IonBuilder::inlineNonFunctionCall(CallInfo& callInfo,
                                                             JSObject* target) {
  // Inline a call to a non-function object, invoking the object's call or
  // construct hook.
||||||| merged common ancestors
IonBuilder::InliningResult
IonBuilder::inlineNativeGetter(CallInfo& callInfo, JSFunction* target)
{
    MOZ_ASSERT(target->isNative());
    JSNative native = target->native();
=======
IonBuilder::InliningResult IonBuilder::inlineNativeGetter(CallInfo& callInfo,
                                                          JSFunction* target) {
  MOZ_ASSERT(target->isNative());
  JSNative native = target->native();
>>>>>>> upstream-releases

<<<<<<< HEAD
  MOZ_ASSERT(target->nonCCWRealm() == script()->realm());
||||||| merged common ancestors
    if (!optimizationInfo().inlineNative()) {
        return InliningStatus_NotInlined;
    }
=======
  if (!optimizationInfo().inlineNative()) {
    return InliningStatus_NotInlined;
  }
>>>>>>> upstream-releases

<<<<<<< HEAD
  // Don't inline if we're constructing and new.target != callee. This can
  // happen with Reflect.construct or derived class constructors.
  if (callInfo.constructing() && callInfo.getNewTarget() != callInfo.fun()) {
    trackOptimizationOutcome(TrackedOutcome::CantInlineUnexpectedNewTarget);
    return InliningStatus_NotInlined;
  }
||||||| merged common ancestors
    MDefinition* thisArg = callInfo.thisArg();
    TemporaryTypeSet* thisTypes = thisArg->resultTypeSet();
    MOZ_ASSERT(callInfo.argc() == 0);
=======
  MDefinition* thisArg = callInfo.thisArg();
  TemporaryTypeSet* thisTypes = thisArg->resultTypeSet();
  MOZ_ASSERT(callInfo.argc() == 0);
>>>>>>> upstream-releases

<<<<<<< HEAD
  if (callInfo.constructing() &&
      target->constructHook() == TypedObject::construct) {
    return inlineConstructTypedObject(callInfo, &target->as<TypeDescr>());
  }
||||||| merged common ancestors
    if (!thisTypes) {
        return InliningStatus_NotInlined;
    }
=======
  if (!thisTypes) {
    return InliningStatus_NotInlined;
  }

  // Note: target might be a cross-realm native!

  // Try to optimize typed array lengths.
  if (TypedArrayObject::isOriginalLengthGetter(native)) {
    if (thisTypes->forAllClasses(constraints(), IsTypedArrayClass) !=
        TemporaryTypeSet::ForAllResult::ALL_TRUE) {
      return InliningStatus_NotInlined;
    }
>>>>>>> upstream-releases

<<<<<<< HEAD
  return InliningStatus_NotInlined;
}
||||||| merged common ancestors
    // Try to optimize typed array lengths.
    if (TypedArrayObject::isOriginalLengthGetter(native)) {
        Scalar::Type type = thisTypes->getTypedArrayType(constraints());
        if (type == Scalar::MaxTypedArrayViewType) {
            return InliningStatus_NotInlined;
        }
=======
    MInstruction* length = addTypedArrayLength(thisArg);
    current->push(length);
    return InliningStatus_Inlined;
  }
>>>>>>> upstream-releases

<<<<<<< HEAD
TemporaryTypeSet* IonBuilder::getInlineReturnTypeSet() {
  return bytecodeTypes(pc);
}
||||||| merged common ancestors
        MInstruction* length = addTypedArrayLength(thisArg);
        current->push(length);
        return InliningStatus_Inlined;
    }
=======
  // Try to optimize typed array byteOffsets.
  if (TypedArrayObject::isOriginalByteOffsetGetter(native)) {
    if (thisTypes->forAllClasses(constraints(), IsTypedArrayClass) !=
        TemporaryTypeSet::ForAllResult::ALL_TRUE) {
      return InliningStatus_NotInlined;
    }
>>>>>>> upstream-releases

<<<<<<< HEAD
MIRType IonBuilder::getInlineReturnType() {
  TemporaryTypeSet* returnTypes = getInlineReturnTypeSet();
  return returnTypes->getKnownMIRType();
}
||||||| merged common ancestors
    // Try to optimize RegExp getters.
    RegExpFlag mask = NoFlags;
    if (RegExpObject::isOriginalFlagGetter(native, &mask)) {
        const Class* clasp = thisTypes->getKnownClass(constraints());
        if (clasp != &RegExpObject::class_) {
            return InliningStatus_NotInlined;
        }
=======
    MInstruction* byteOffset = addTypedArrayByteOffset(thisArg);
    current->push(byteOffset);
    return InliningStatus_Inlined;
  }
>>>>>>> upstream-releases

<<<<<<< HEAD
IonBuilder::InliningResult IonBuilder::inlineMathFunction(
    CallInfo& callInfo, MMathFunction::Function function) {
  if (callInfo.constructing()) {
    return InliningStatus_NotInlined;
  }
||||||| merged common ancestors
        MLoadFixedSlot* flags = MLoadFixedSlot::New(alloc(), thisArg, RegExpObject::flagsSlot());
        current->add(flags);
        flags->setResultType(MIRType::Int32);
        MConstant* maskConst = MConstant::New(alloc(), Int32Value(mask));
        current->add(maskConst);
        MBitAnd* maskedFlag = MBitAnd::New(alloc(), flags, maskConst);
        maskedFlag->setInt32Specialization();
        current->add(maskedFlag);

        MDefinition* result = convertToBoolean(maskedFlag);
        current->push(result);
        return InliningStatus_Inlined;
    }
=======
  // Try to optimize RegExp getters.
  RegExpFlags mask = RegExpFlag::NoFlags;
  if (RegExpObject::isOriginalFlagGetter(native, &mask)) {
    const Class* clasp = thisTypes->getKnownClass(constraints());
    if (clasp != &RegExpObject::class_) {
      return InliningStatus_NotInlined;
    }
>>>>>>> upstream-releases

<<<<<<< HEAD
  if (callInfo.argc() != 1) {
    return InliningStatus_NotInlined;
  }
||||||| merged common ancestors
    return InliningStatus_NotInlined;
}
=======
    MLoadFixedSlot* flags =
        MLoadFixedSlot::New(alloc(), thisArg, RegExpObject::flagsSlot());
    current->add(flags);
    flags->setResultType(MIRType::Int32);
    MConstant* maskConst = MConstant::New(alloc(), Int32Value(mask.value()));
    current->add(maskConst);
    MBitAnd* maskedFlag = MBitAnd::New(alloc(), flags, maskConst);
    maskedFlag->setInt32Specialization();
    current->add(maskedFlag);
>>>>>>> upstream-releases

<<<<<<< HEAD
  if (getInlineReturnType() != MIRType::Double) {
    return InliningStatus_NotInlined;
  }
  if (!IsNumberType(callInfo.getArg(0)->type())) {
    return InliningStatus_NotInlined;
  }
||||||| merged common ancestors
IonBuilder::InliningResult
IonBuilder::inlineNonFunctionCall(CallInfo& callInfo, JSObject* target)
{
    // Inline a call to a non-function object, invoking the object's call or
    // construct hook.
=======
    MDefinition* result = convertToBoolean(maskedFlag);
    current->push(result);
    return InliningStatus_Inlined;
  }
>>>>>>> upstream-releases

<<<<<<< HEAD
  callInfo.fun()->setImplicitlyUsedUnchecked();
  callInfo.thisArg()->setImplicitlyUsedUnchecked();
||||||| merged common ancestors
    MOZ_ASSERT(target->nonCCWRealm() == script()->realm());
=======
  return InliningStatus_NotInlined;
}
>>>>>>> upstream-releases

<<<<<<< HEAD
  MMathFunction* ins =
      MMathFunction::New(alloc(), callInfo.getArg(0), function);
  current->add(ins);
  current->push(ins);
  return InliningStatus_Inlined;
}
||||||| merged common ancestors
    // Don't inline if we're constructing and new.target != callee. This can
    // happen with Reflect.construct or derived class constructors.
    if (callInfo.constructing() && callInfo.getNewTarget() != callInfo.fun()) {
        trackOptimizationOutcome(TrackedOutcome::CantInlineUnexpectedNewTarget);
        return InliningStatus_NotInlined;
    }
=======
IonBuilder::InliningResult IonBuilder::inlineNonFunctionCall(CallInfo& callInfo,
                                                             JSObject* target) {
  // Inline a call to a non-function object, invoking the object's call or
  // construct hook.
>>>>>>> upstream-releases

<<<<<<< HEAD
IonBuilder::InliningResult IonBuilder::inlineArray(CallInfo& callInfo) {
  uint32_t initLength = 0;
||||||| merged common ancestors
    if (callInfo.constructing() && target->constructHook() == TypedObject::construct) {
        return inlineConstructTypedObject(callInfo, &target->as<TypeDescr>());
    }
=======
  // Don't inline if we're constructing and new.target != callee. This can
  // happen with Reflect.construct or derived class constructors.
  if (callInfo.constructing() && callInfo.getNewTarget() != callInfo.fun()) {
    trackOptimizationOutcome(TrackedOutcome::CantInlineUnexpectedNewTarget);
    return InliningStatus_NotInlined;
  }
>>>>>>> upstream-releases

<<<<<<< HEAD
  JSObject* templateObject =
      inspector->getTemplateObjectForNative(pc, ArrayConstructor);
  // This is shared by ArrayConstructor and array_construct (std_Array).
  if (!templateObject) {
    templateObject = inspector->getTemplateObjectForNative(pc, array_construct);
  }

  if (!templateObject) {
    trackOptimizationOutcome(TrackedOutcome::CantInlineNativeNoTemplateObj);
||||||| merged common ancestors
=======
  Realm* targetRealm = JS::GetObjectRealmOrNull(target);
  if (!targetRealm || targetRealm != script()->realm()) {
    trackOptimizationOutcome(TrackedOutcome::CantInlineCrossRealm);
>>>>>>> upstream-releases
    return InliningStatus_NotInlined;
<<<<<<< HEAD
  }
||||||| merged common ancestors
}
=======
  }

  if (callInfo.constructing() &&
      target->constructHook() == TypedObject::construct) {
    return inlineConstructTypedObject(callInfo, &target->as<TypeDescr>());
  }

  return InliningStatus_NotInlined;
}
>>>>>>> upstream-releases

<<<<<<< HEAD
  // Multiple arguments imply array initialization, not just construction.
  if (callInfo.argc() >= 2) {
    initLength = callInfo.argc();
||||||| merged common ancestors
TemporaryTypeSet*
IonBuilder::getInlineReturnTypeSet()
{
    return bytecodeTypes(pc);
}
=======
TemporaryTypeSet* IonBuilder::getInlineReturnTypeSet() {
  return bytecodeTypes(pc);
}
>>>>>>> upstream-releases

<<<<<<< HEAD
    TypeSet::ObjectKey* key = TypeSet::ObjectKey::get(templateObject);
    if (!key->unknownProperties()) {
      HeapTypeSetKey elemTypes = key->property(JSID_VOID);
||||||| merged common ancestors
MIRType
IonBuilder::getInlineReturnType()
{
    TemporaryTypeSet* returnTypes = getInlineReturnTypeSet();
    return returnTypes->getKnownMIRType();
}
=======
MIRType IonBuilder::getInlineReturnType() {
  TemporaryTypeSet* returnTypes = getInlineReturnTypeSet();
  return returnTypes->getKnownMIRType();
}
>>>>>>> upstream-releases

<<<<<<< HEAD
      for (uint32_t i = 0; i < initLength; i++) {
        MDefinition* value = callInfo.getArg(i);
        if (!TypeSetIncludes(elemTypes.maybeTypes(), value->type(),
                             value->resultTypeSet())) {
          elemTypes.freeze(constraints());
          return InliningStatus_NotInlined;
        }
      }
    }
  }
||||||| merged common ancestors
IonBuilder::InliningResult
IonBuilder::inlineMathFunction(CallInfo& callInfo, MMathFunction::Function function)
{
    if (callInfo.constructing()) {
        return InliningStatus_NotInlined;
    }
=======
IonBuilder::InliningResult IonBuilder::inlineMathFunction(
    CallInfo& callInfo, MMathFunction::Function function) {
  if (callInfo.constructing()) {
    return InliningStatus_NotInlined;
  }
>>>>>>> upstream-releases

<<<<<<< HEAD
  // A single integer argument denotes initial length.
  if (callInfo.argc() == 1) {
    MDefinition* arg = callInfo.getArg(0);
    if (arg->type() != MIRType::Int32) {
      return InliningStatus_NotInlined;
    }
||||||| merged common ancestors
    if (callInfo.argc() != 1) {
        return InliningStatus_NotInlined;
    }
=======
  if (callInfo.argc() != 1) {
    return InliningStatus_NotInlined;
  }
>>>>>>> upstream-releases

<<<<<<< HEAD
    if (!arg->isConstant()) {
      callInfo.setImplicitlyUsedUnchecked();
      MNewArrayDynamicLength* ins = MNewArrayDynamicLength::New(
          alloc(), constraints(), templateObject,
          templateObject->group()->initialHeap(constraints()), arg);
      current->add(ins);
      current->push(ins);
||||||| merged common ancestors
    if (getInlineReturnType() != MIRType::Double) {
        return InliningStatus_NotInlined;
    }
    if (!IsNumberType(callInfo.getArg(0)->type())) {
        return InliningStatus_NotInlined;
    }
=======
  if (getInlineReturnType() != MIRType::Double) {
    return InliningStatus_NotInlined;
  }
  if (!IsNumberType(callInfo.getArg(0)->type())) {
    return InliningStatus_NotInlined;
  }
>>>>>>> upstream-releases

<<<<<<< HEAD
      // This may throw, so we need a resume point.
      MOZ_TRY(resumeAfter(ins));
||||||| merged common ancestors
    callInfo.fun()->setImplicitlyUsedUnchecked();
    callInfo.thisArg()->setImplicitlyUsedUnchecked();
=======
  callInfo.fun()->setImplicitlyUsedUnchecked();
  callInfo.thisArg()->setImplicitlyUsedUnchecked();
>>>>>>> upstream-releases

<<<<<<< HEAD
      return InliningStatus_Inlined;
    }
||||||| merged common ancestors
    MMathFunction* ins = MMathFunction::New(alloc(), callInfo.getArg(0), function);
    current->add(ins);
    current->push(ins);
    return InliningStatus_Inlined;
}
=======
  MMathFunction* ins =
      MMathFunction::New(alloc(), callInfo.getArg(0), function);
  current->add(ins);
  current->push(ins);
  return InliningStatus_Inlined;
}
>>>>>>> upstream-releases

<<<<<<< HEAD
    // The next several checks all may fail due to range conditions.
    trackOptimizationOutcome(TrackedOutcome::ArrayRange);

    // Negative lengths generate a RangeError, unhandled by the inline path.
    initLength = arg->toConstant()->toInt32();
    if (initLength > NativeObject::MAX_DENSE_ELEMENTS_COUNT) {
      return InliningStatus_NotInlined;
    }
    MOZ_ASSERT(initLength <= INT32_MAX);

    // Make sure initLength matches the template object's length. This is
    // not guaranteed to be the case, for instance if we're inlining the
    // MConstant may come from an outer script.
    if (initLength != templateObject->as<ArrayObject>().length()) {
      return InliningStatus_NotInlined;
    }

    // Don't inline large allocations.
    if (initLength > ArrayObject::EagerAllocationMaxLength) {
      return InliningStatus_NotInlined;
    }
  }
||||||| merged common ancestors
IonBuilder::InliningResult
IonBuilder::inlineArray(CallInfo& callInfo)
{
    uint32_t initLength = 0;

    JSObject* templateObject = inspector->getTemplateObjectForNative(pc, ArrayConstructor);
    // This is shared by ArrayConstructor and array_construct (std_Array).
    if (!templateObject) {
        templateObject = inspector->getTemplateObjectForNative(pc, array_construct);
    }

    if (!templateObject) {
        trackOptimizationOutcome(TrackedOutcome::CantInlineNativeNoTemplateObj);
        return InliningStatus_NotInlined;
    }

    // Multiple arguments imply array initialization, not just construction.
    if (callInfo.argc() >= 2) {
        initLength = callInfo.argc();

        TypeSet::ObjectKey* key = TypeSet::ObjectKey::get(templateObject);
        if (!key->unknownProperties()) {
            HeapTypeSetKey elemTypes = key->property(JSID_VOID);

            for (uint32_t i = 0; i < initLength; i++) {
                MDefinition* value = callInfo.getArg(i);
                if (!TypeSetIncludes(elemTypes.maybeTypes(), value->type(), value->resultTypeSet())) {
                    elemTypes.freeze(constraints());
                    return InliningStatus_NotInlined;
                }
            }
        }
    }
=======
IonBuilder::InliningResult IonBuilder::inlineArray(CallInfo& callInfo,
                                                   Realm* targetRealm) {
  uint32_t initLength = 0;
>>>>>>> upstream-releases

<<<<<<< HEAD
  callInfo.setImplicitlyUsedUnchecked();

  MOZ_TRY(jsop_newarray(templateObject, initLength));

  MDefinition* array = current->peek(-1);
  if (callInfo.argc() >= 2) {
    for (uint32_t i = 0; i < initLength; i++) {
      if (!alloc().ensureBallast()) {
        return abort(AbortReason::Alloc);
      }
      MDefinition* value = callInfo.getArg(i);
      MOZ_TRY(initializeArrayElement(array, i, value,
                                     /* addResumePoint = */ false));
    }
||||||| merged common ancestors
    // A single integer argument denotes initial length.
    if (callInfo.argc() == 1) {
        MDefinition* arg = callInfo.getArg(0);
        if (arg->type() != MIRType::Int32) {
            return InliningStatus_NotInlined;
        }

        if (!arg->isConstant()) {
            callInfo.setImplicitlyUsedUnchecked();
            MNewArrayDynamicLength* ins =
                MNewArrayDynamicLength::New(alloc(), constraints(), templateObject,
                                            templateObject->group()->initialHeap(constraints()),
                                            arg);
            current->add(ins);
            current->push(ins);
=======
  JSObject* templateObject =
      inspector->getTemplateObjectForNative(pc, ArrayConstructor);
  // This is shared by ArrayConstructor and array_construct (std_Array).
  if (!templateObject) {
    templateObject = inspector->getTemplateObjectForNative(pc, array_construct);
  }
>>>>>>> upstream-releases

<<<<<<< HEAD
    MInstruction* setLength = setInitializedLength(array, initLength);
    MOZ_TRY(resumeAfter(setLength));
  }

  return InliningStatus_Inlined;
}
||||||| merged common ancestors
            // This may throw, so we need a resume point.
            MOZ_TRY(resumeAfter(ins));

            return InliningStatus_Inlined;
        }
=======
  if (!templateObject) {
    trackOptimizationOutcome(TrackedOutcome::CantInlineNativeNoTemplateObj);
    return InliningStatus_NotInlined;
  }
>>>>>>> upstream-releases

<<<<<<< HEAD
static bool IsArrayClass(const Class* clasp) {
  return clasp == &ArrayObject::class_;
}
||||||| merged common ancestors
        // The next several checks all may fail due to range conditions.
        trackOptimizationOutcome(TrackedOutcome::ArrayRange);
=======
  if (templateObject->nonCCWRealm() != targetRealm) {
    trackOptimizationOutcome(TrackedOutcome::CantInlineCrossRealm);
    return InliningStatus_NotInlined;
  }
>>>>>>> upstream-releases

<<<<<<< HEAD
IonBuilder::InliningResult IonBuilder::inlineArrayIsArray(CallInfo& callInfo) {
  if (callInfo.constructing() || callInfo.argc() != 1) {
    trackOptimizationOutcome(TrackedOutcome::CantInlineNativeBadForm);
    return InliningStatus_NotInlined;
  }
||||||| merged common ancestors
        // Negative lengths generate a RangeError, unhandled by the inline path.
        initLength = arg->toConstant()->toInt32();
        if (initLength > NativeObject::MAX_DENSE_ELEMENTS_COUNT) {
            return InliningStatus_NotInlined;
        }
        MOZ_ASSERT(initLength <= INT32_MAX);
=======
  // Multiple arguments imply array initialization, not just construction.
  if (callInfo.argc() >= 2) {
    initLength = callInfo.argc();
>>>>>>> upstream-releases

<<<<<<< HEAD
  if (getInlineReturnType() != MIRType::Boolean) {
    return InliningStatus_NotInlined;
  }
||||||| merged common ancestors
        // Make sure initLength matches the template object's length. This is
        // not guaranteed to be the case, for instance if we're inlining the
        // MConstant may come from an outer script.
        if (initLength != templateObject->as<ArrayObject>().length()) {
            return InliningStatus_NotInlined;
        }
=======
    TypeSet::ObjectKey* key = TypeSet::ObjectKey::get(templateObject);
    if (!key->unknownProperties()) {
      HeapTypeSetKey elemTypes = key->property(JSID_VOID);
>>>>>>> upstream-releases

<<<<<<< HEAD
  MDefinition* arg = callInfo.getArg(0);
||||||| merged common ancestors
        // Don't inline large allocations.
        if (initLength > ArrayObject::EagerAllocationMaxLength) {
            return InliningStatus_NotInlined;
        }
    }
=======
      for (uint32_t i = 0; i < initLength; i++) {
        MDefinition* value = callInfo.getArg(i);
        if (!TypeSetIncludes(elemTypes.maybeTypes(), value->type(),
                             value->resultTypeSet())) {
          elemTypes.freeze(constraints());
          return InliningStatus_NotInlined;
        }
      }
    }
  }
>>>>>>> upstream-releases

<<<<<<< HEAD
  if (!arg->mightBeType(MIRType::Object)) {
    pushConstant(BooleanValue(false));
    callInfo.setImplicitlyUsedUnchecked();
    return InliningStatus_Inlined;
  }
||||||| merged common ancestors
    callInfo.setImplicitlyUsedUnchecked();
=======
  // A single integer argument denotes initial length.
  if (callInfo.argc() == 1) {
    MDefinition* arg = callInfo.getArg(0);
    if (arg->type() != MIRType::Int32) {
      return InliningStatus_NotInlined;
    }
>>>>>>> upstream-releases

<<<<<<< HEAD
  using ForAllResult = TemporaryTypeSet::ForAllResult;
||||||| merged common ancestors
    MOZ_TRY(jsop_newarray(templateObject, initLength));
=======
    if (!arg->isConstant()) {
      callInfo.setImplicitlyUsedUnchecked();
      MNewArrayDynamicLength* ins = MNewArrayDynamicLength::New(
          alloc(), constraints(), templateObject,
          templateObject->group()->initialHeap(constraints()), arg);
      current->add(ins);
      current->push(ins);
>>>>>>> upstream-releases

<<<<<<< HEAD
  TemporaryTypeSet* types = arg->resultTypeSet();
||||||| merged common ancestors
    MDefinition* array = current->peek(-1);
    if (callInfo.argc() >= 2) {
        for (uint32_t i = 0; i < initLength; i++) {
            if (!alloc().ensureBallast()) {
                return abort(AbortReason::Alloc);
            }
            MDefinition* value = callInfo.getArg(i);
            MOZ_TRY(initializeArrayElement(array, i, value, /* addResumePoint = */ false));
        }
=======
      // This may throw, so we need a resume point.
      MOZ_TRY(resumeAfter(ins));
>>>>>>> upstream-releases

<<<<<<< HEAD
  // Fast path for non-proxy objects.
  if (arg->type() == MIRType::Object && types &&
      types->forAllClasses(constraints(), IsProxyClass) ==
          ForAllResult::ALL_FALSE) {
    // Definitely not a proxy. Now check for the array classes.
    ForAllResult result = types->forAllClasses(constraints(), IsArrayClass);

    if (result == ForAllResult::ALL_FALSE || result == ForAllResult::ALL_TRUE) {
      // Definitely an array or definitely not an array, so we can
      // constant fold.
      pushConstant(BooleanValue(result == ForAllResult::ALL_TRUE));
      callInfo.setImplicitlyUsedUnchecked();
      return InliningStatus_Inlined;
||||||| merged common ancestors
        MInstruction* setLength = setInitializedLength(array, initLength);
        MOZ_TRY(resumeAfter(setLength));
=======
      return InliningStatus_Inlined;
>>>>>>> upstream-releases
    }

<<<<<<< HEAD
    // We have some array classes and some non-array classes, so we have to
    // check at runtime.
    MOZ_ASSERT(result == ForAllResult::MIXED);

    MHasClass* hasClass = MHasClass::New(alloc(), arg, &ArrayObject::class_);
    current->add(hasClass);
    current->push(hasClass);
||||||| merged common ancestors
    return InliningStatus_Inlined;
}

static bool
IsArrayClass(const Class* clasp)
{
    return clasp == &ArrayObject::class_;
}
=======
    // The next several checks all may fail due to range conditions.
    trackOptimizationOutcome(TrackedOutcome::ArrayRange);
>>>>>>> upstream-releases

<<<<<<< HEAD
    callInfo.setImplicitlyUsedUnchecked();
    return InliningStatus_Inlined;
  }
||||||| merged common ancestors
IonBuilder::InliningResult
IonBuilder::inlineArrayIsArray(CallInfo& callInfo)
{
    if (callInfo.constructing() || callInfo.argc() != 1) {
        trackOptimizationOutcome(TrackedOutcome::CantInlineNativeBadForm);
        return InliningStatus_NotInlined;
    }
=======
    // Negative lengths generate a RangeError, unhandled by the inline path.
    initLength = arg->toConstant()->toInt32();
    if (initLength > NativeObject::MAX_DENSE_ELEMENTS_COUNT) {
      return InliningStatus_NotInlined;
    }
    MOZ_ASSERT(initLength <= INT32_MAX);
>>>>>>> upstream-releases

<<<<<<< HEAD
  // The value might be a primitive or a proxy. MIsArray handles these cases.
  MIsArray* isArray = MIsArray::New(alloc(), arg);
  current->add(isArray);
  current->push(isArray);
||||||| merged common ancestors
    if (getInlineReturnType() != MIRType::Boolean) {
        return InliningStatus_NotInlined;
    }
=======
    // Make sure initLength matches the template object's length. This is
    // not guaranteed to be the case, for instance if we're inlining the
    // MConstant may come from an outer script.
    if (initLength != templateObject->as<ArrayObject>().length()) {
      return InliningStatus_NotInlined;
    }
>>>>>>> upstream-releases

<<<<<<< HEAD
  MOZ_TRY(resumeAfter(isArray));

  callInfo.setImplicitlyUsedUnchecked();
  return InliningStatus_Inlined;
}
||||||| merged common ancestors
    MDefinition* arg = callInfo.getArg(0);

    if (!arg->mightBeType(MIRType::Object)) {
        pushConstant(BooleanValue(false));
        callInfo.setImplicitlyUsedUnchecked();
        return InliningStatus_Inlined;
    }
=======
    // Don't inline large allocations.
    if (initLength > ArrayObject::EagerAllocationMaxLength) {
      return InliningStatus_NotInlined;
    }
  }
>>>>>>> upstream-releases

<<<<<<< HEAD
IonBuilder::InliningResult IonBuilder::inlineArrayPopShift(
    CallInfo& callInfo, MArrayPopShift::Mode mode) {
  if (callInfo.constructing()) {
    trackOptimizationOutcome(TrackedOutcome::CantInlineNativeBadForm);
    return InliningStatus_NotInlined;
  }
||||||| merged common ancestors
    using ForAllResult = TemporaryTypeSet::ForAllResult;
=======
  callInfo.setImplicitlyUsedUnchecked();
>>>>>>> upstream-releases

<<<<<<< HEAD
  MIRType returnType = getInlineReturnType();
  if (returnType == MIRType::Undefined || returnType == MIRType::Null) {
    return InliningStatus_NotInlined;
  }
  if (callInfo.thisArg()->type() != MIRType::Object) {
    return InliningStatus_NotInlined;
  }

  // Pop and shift are only handled for dense arrays that have never been
  // used in an iterator: popping elements does not account for suppressing
  // deleted properties in active iterators.
  // Don't optimize shift if the array may be non-extensible (this matters
  // when there are holes). Don't optimize pop if the array may be
  // non-extensible, so we don't need to adjust the capacity for
  // non-extensible arrays (non-extensible objects always have a capacity
  // equal to their initialized length). We check this here because there's
  // no non-extensible ObjectElements flag so we would need an extra guard
  // on the BaseShape flags.
  ObjectGroupFlags unhandledFlags =
      OBJECT_FLAG_SPARSE_INDEXES | OBJECT_FLAG_LENGTH_OVERFLOW |
      OBJECT_FLAG_ITERATED | OBJECT_FLAG_NON_EXTENSIBLE_ELEMENTS;

  MDefinition* obj = convertUnboxedObjects(callInfo.thisArg());
  TemporaryTypeSet* thisTypes = obj->resultTypeSet();
  if (!thisTypes) {
    return InliningStatus_NotInlined;
  }
  const Class* clasp = thisTypes->getKnownClass(constraints());
  if (clasp != &ArrayObject::class_) {
    return InliningStatus_NotInlined;
  }
  if (thisTypes->hasObjectFlags(constraints(), unhandledFlags)) {
    trackOptimizationOutcome(TrackedOutcome::ArrayBadFlags);
    return InliningStatus_NotInlined;
  }
||||||| merged common ancestors
    TemporaryTypeSet* types = arg->resultTypeSet();
=======
  MOZ_TRY(jsop_newarray(templateObject, initLength));
>>>>>>> upstream-releases

<<<<<<< HEAD
  bool hasIndexedProperty;
  MOZ_TRY_VAR(hasIndexedProperty,
              ArrayPrototypeHasIndexedProperty(this, script()));
  if (hasIndexedProperty) {
    trackOptimizationOutcome(TrackedOutcome::ProtoIndexedProps);
    return InliningStatus_NotInlined;
  }
||||||| merged common ancestors
    // Fast path for non-proxy objects.
    if (arg->type() == MIRType::Object &&
        types &&
        types->forAllClasses(constraints(), IsProxyClass) == ForAllResult::ALL_FALSE)
    {
        // Definitely not a proxy. Now check for the array classes.
        ForAllResult result = types->forAllClasses(constraints(), IsArrayClass);

        if (result == ForAllResult::ALL_FALSE || result == ForAllResult::ALL_TRUE) {
            // Definitely an array or definitely not an array, so we can
            // constant fold.
            pushConstant(BooleanValue(result == ForAllResult::ALL_TRUE));
            callInfo.setImplicitlyUsedUnchecked();
            return InliningStatus_Inlined;
        }
=======
  MDefinition* array = current->peek(-1);
  if (callInfo.argc() >= 2) {
    for (uint32_t i = 0; i < initLength; i++) {
      if (!alloc().ensureBallast()) {
        return abort(AbortReason::Alloc);
      }
      MDefinition* value = callInfo.getArg(i);
      MOZ_TRY(initializeArrayElement(array, i, value,
                                     /* addResumePoint = */ false));
    }
>>>>>>> upstream-releases

<<<<<<< HEAD
  callInfo.setImplicitlyUsedUnchecked();
||||||| merged common ancestors
        // We have some array classes and some non-array classes, so we have to
        // check at runtime.
        MOZ_ASSERT(result == ForAllResult::MIXED);
=======
    MInstruction* setLength = setInitializedLength(array, initLength);
    MOZ_TRY(resumeAfter(setLength));
  }
>>>>>>> upstream-releases

<<<<<<< HEAD
  obj = addMaybeCopyElementsForWrite(obj, /* checkNative = */ false);
||||||| merged common ancestors
        MHasClass* hasClass = MHasClass::New(alloc(), arg, &ArrayObject::class_);
        current->add(hasClass);
        current->push(hasClass);
=======
  return InliningStatus_Inlined;
}
>>>>>>> upstream-releases

<<<<<<< HEAD
  TemporaryTypeSet* returnTypes = getInlineReturnTypeSet();
  bool needsHoleCheck =
      thisTypes->hasObjectFlags(constraints(), OBJECT_FLAG_NON_PACKED);
  bool maybeUndefined = returnTypes->hasType(TypeSet::UndefinedType());
||||||| merged common ancestors
        callInfo.setImplicitlyUsedUnchecked();
        return InliningStatus_Inlined;
    }
=======
static bool IsArrayClass(const Class* clasp) {
  return clasp == &ArrayObject::class_;
}

IonBuilder::InliningResult IonBuilder::inlineArrayIsArray(CallInfo& callInfo) {
  if (callInfo.constructing() || callInfo.argc() != 1) {
    trackOptimizationOutcome(TrackedOutcome::CantInlineNativeBadForm);
    return InliningStatus_NotInlined;
  }
>>>>>>> upstream-releases

<<<<<<< HEAD
  BarrierKind barrier = PropertyReadNeedsTypeBarrier(
      analysisContext, alloc(), constraints(), obj, nullptr, returnTypes);
  if (barrier != BarrierKind::NoBarrier) {
    returnType = MIRType::Value;
  }
||||||| merged common ancestors
    // The value might be a primitive or a proxy. MIsArray handles these cases.
    MIsArray* isArray = MIsArray::New(alloc(), arg);
    current->add(isArray);
    current->push(isArray);
=======
  if (getInlineReturnType() != MIRType::Boolean) {
    return InliningStatus_NotInlined;
  }
>>>>>>> upstream-releases

<<<<<<< HEAD
  MArrayPopShift* ins =
      MArrayPopShift::New(alloc(), obj, mode, needsHoleCheck, maybeUndefined);
  current->add(ins);
  current->push(ins);
  ins->setResultType(returnType);
||||||| merged common ancestors
    MOZ_TRY(resumeAfter(isArray));
=======
  MDefinition* arg = callInfo.getArg(0);
>>>>>>> upstream-releases

<<<<<<< HEAD
  MOZ_TRY(resumeAfter(ins));
  MOZ_TRY(pushTypeBarrier(ins, returnTypes, barrier));
  return InliningStatus_Inlined;
}
||||||| merged common ancestors
    callInfo.setImplicitlyUsedUnchecked();
    return InliningStatus_Inlined;
}
=======
  if (!arg->mightBeType(MIRType::Object)) {
    pushConstant(BooleanValue(false));
    callInfo.setImplicitlyUsedUnchecked();
    return InliningStatus_Inlined;
  }
>>>>>>> upstream-releases

<<<<<<< HEAD
IonBuilder::InliningResult IonBuilder::inlineArrayJoin(CallInfo& callInfo) {
  if (callInfo.argc() != 1 || callInfo.constructing()) {
    trackOptimizationOutcome(TrackedOutcome::CantInlineNativeBadForm);
    return InliningStatus_NotInlined;
  }
||||||| merged common ancestors
IonBuilder::InliningResult
IonBuilder::inlineArrayPopShift(CallInfo& callInfo, MArrayPopShift::Mode mode)
{
    if (callInfo.constructing()) {
        trackOptimizationOutcome(TrackedOutcome::CantInlineNativeBadForm);
        return InliningStatus_NotInlined;
    }
=======
  using ForAllResult = TemporaryTypeSet::ForAllResult;
>>>>>>> upstream-releases

<<<<<<< HEAD
  if (getInlineReturnType() != MIRType::String) {
    return InliningStatus_NotInlined;
  }
  if (callInfo.thisArg()->type() != MIRType::Object) {
    return InliningStatus_NotInlined;
  }
  if (callInfo.getArg(0)->type() != MIRType::String) {
    return InliningStatus_NotInlined;
  }
||||||| merged common ancestors
    MIRType returnType = getInlineReturnType();
    if (returnType == MIRType::Undefined || returnType == MIRType::Null) {
        return InliningStatus_NotInlined;
    }
    if (callInfo.thisArg()->type() != MIRType::Object) {
        return InliningStatus_NotInlined;
    }
=======
  TemporaryTypeSet* types = arg->resultTypeSet();
>>>>>>> upstream-releases

<<<<<<< HEAD
  // If we can confirm that the class is an array, the codegen
  // for MArrayJoin can be notified to check for common empty and one-item
  // arrays.
  bool optimizeForArray = ([&]() {
    TemporaryTypeSet* thisTypes = callInfo.thisArg()->resultTypeSet();
    if (!thisTypes) {
      return false;
    }

    const Class* clasp = thisTypes->getKnownClass(constraints());
    if (clasp != &ArrayObject::class_) {
      return false;
    }
||||||| merged common ancestors
    // Pop and shift are only handled for dense arrays that have never been
    // used in an iterator: popping elements does not account for suppressing
    // deleted properties in active iterators.
    // Don't optimize shift if the array may be non-extensible (this matters
    // when there are holes). Don't optimize pop if the array may be
    // non-extensible, so we don't need to adjust the capacity for
    // non-extensible arrays (non-extensible objects always have a capacity
    // equal to their initialized length). We check this here because there's
    // no non-extensible ObjectElements flag so we would need an extra guard
    // on the BaseShape flags.
    ObjectGroupFlags unhandledFlags =
        OBJECT_FLAG_SPARSE_INDEXES |
        OBJECT_FLAG_LENGTH_OVERFLOW |
        OBJECT_FLAG_ITERATED |
        OBJECT_FLAG_NON_EXTENSIBLE_ELEMENTS;

    MDefinition* obj = convertUnboxedObjects(callInfo.thisArg());
    TemporaryTypeSet* thisTypes = obj->resultTypeSet();
    if (!thisTypes) {
        return InliningStatus_NotInlined;
    }
    const Class* clasp = thisTypes->getKnownClass(constraints());
    if (clasp != &ArrayObject::class_) {
        return InliningStatus_NotInlined;
    }
    if (thisTypes->hasObjectFlags(constraints(), unhandledFlags)) {
        trackOptimizationOutcome(TrackedOutcome::ArrayBadFlags);
        return InliningStatus_NotInlined;
    }
=======
  // Fast path for non-proxy objects.
  if (arg->type() == MIRType::Object && types &&
      types->forAllClasses(constraints(), IsProxyClass) ==
          ForAllResult::ALL_FALSE) {
    // Definitely not a proxy. Now check for the array classes.
    ForAllResult result = types->forAllClasses(constraints(), IsArrayClass);
>>>>>>> upstream-releases

<<<<<<< HEAD
    return true;
  })();
||||||| merged common ancestors
    bool hasIndexedProperty;
    MOZ_TRY_VAR(hasIndexedProperty, ArrayPrototypeHasIndexedProperty(this, script()));
    if (hasIndexedProperty) {
        trackOptimizationOutcome(TrackedOutcome::ProtoIndexedProps);
        return InliningStatus_NotInlined;
    }
=======
    if (result == ForAllResult::ALL_FALSE || result == ForAllResult::ALL_TRUE) {
      // Definitely an array or definitely not an array, so we can
      // constant fold.
      pushConstant(BooleanValue(result == ForAllResult::ALL_TRUE));
      callInfo.setImplicitlyUsedUnchecked();
      return InliningStatus_Inlined;
    }
>>>>>>> upstream-releases

<<<<<<< HEAD
  callInfo.setImplicitlyUsedUnchecked();
||||||| merged common ancestors
    callInfo.setImplicitlyUsedUnchecked();
=======
    // We have some array classes and some non-array classes, so we have to
    // check at runtime.
    MOZ_ASSERT(result == ForAllResult::MIXED);
>>>>>>> upstream-releases

<<<<<<< HEAD
  MArrayJoin* ins = MArrayJoin::New(alloc(), callInfo.thisArg(),
                                    callInfo.getArg(0), optimizeForArray);
||||||| merged common ancestors
    obj = addMaybeCopyElementsForWrite(obj, /* checkNative = */ false);
=======
    MHasClass* hasClass = MHasClass::New(alloc(), arg, &ArrayObject::class_);
    current->add(hasClass);
    current->push(hasClass);
>>>>>>> upstream-releases

<<<<<<< HEAD
  current->add(ins);
  current->push(ins);
||||||| merged common ancestors
    TemporaryTypeSet* returnTypes = getInlineReturnTypeSet();
    bool needsHoleCheck = thisTypes->hasObjectFlags(constraints(), OBJECT_FLAG_NON_PACKED);
    bool maybeUndefined = returnTypes->hasType(TypeSet::UndefinedType());
=======
    callInfo.setImplicitlyUsedUnchecked();
    return InliningStatus_Inlined;
  }
>>>>>>> upstream-releases

<<<<<<< HEAD
  MOZ_TRY(resumeAfter(ins));
  return InliningStatus_Inlined;
}
||||||| merged common ancestors
    BarrierKind barrier = PropertyReadNeedsTypeBarrier(analysisContext, alloc(), constraints(),
                                                       obj, nullptr, returnTypes);
    if (barrier != BarrierKind::NoBarrier) {
        returnType = MIRType::Value;
    }
=======
  // The value might be a primitive or a proxy. MIsArray handles these cases.
  MIsArray* isArray = MIsArray::New(alloc(), arg);
  current->add(isArray);
  current->push(isArray);
>>>>>>> upstream-releases

<<<<<<< HEAD
IonBuilder::InliningResult IonBuilder::inlineArrayPush(CallInfo& callInfo) {
  const uint32_t inlineArgsLimit = 10;
  if (callInfo.argc() < 1 || callInfo.argc() > inlineArgsLimit ||
      callInfo.constructing()) {
    trackOptimizationOutcome(TrackedOutcome::CantInlineNativeBadForm);
    return InliningStatus_NotInlined;
  }
||||||| merged common ancestors
    MArrayPopShift* ins = MArrayPopShift::New(alloc(), obj, mode, needsHoleCheck, maybeUndefined);
    current->add(ins);
    current->push(ins);
    ins->setResultType(returnType);
=======
  MOZ_TRY(resumeAfter(isArray));
>>>>>>> upstream-releases

<<<<<<< HEAD
  // XXX bug 1493903.
  if (callInfo.argc() != 1) {
    trackOptimizationOutcome(TrackedOutcome::CantInlineNativeBadForm);
    return InliningStatus_NotInlined;
  }
||||||| merged common ancestors
    MOZ_TRY(resumeAfter(ins));
    MOZ_TRY(pushTypeBarrier(ins, returnTypes, barrier));
    return InliningStatus_Inlined;
}
=======
  callInfo.setImplicitlyUsedUnchecked();
  return InliningStatus_Inlined;
}
>>>>>>> upstream-releases

<<<<<<< HEAD
  MDefinition* obj = convertUnboxedObjects(callInfo.thisArg());
  for (uint32_t i = 0; i < callInfo.argc(); i++) {
    MDefinition* value = callInfo.getArg(i);
    if (PropertyWriteNeedsTypeBarrier(alloc(), constraints(), current, &obj,
                                      nullptr, &value,
                                      /* canModify = */ false)) {
      trackOptimizationOutcome(TrackedOutcome::NeedsTypeBarrier);
      return InliningStatus_NotInlined;
    }
  }

  if (getInlineReturnType() != MIRType::Int32) {
    return InliningStatus_NotInlined;
  }
  if (obj->type() != MIRType::Object) {
    return InliningStatus_NotInlined;
  }
||||||| merged common ancestors
IonBuilder::InliningResult
IonBuilder::inlineArrayJoin(CallInfo& callInfo)
{
    if (callInfo.argc() != 1 || callInfo.constructing()) {
        trackOptimizationOutcome(TrackedOutcome::CantInlineNativeBadForm);
        return InliningStatus_NotInlined;
    }

    if (getInlineReturnType() != MIRType::String) {
        return InliningStatus_NotInlined;
    }
    if (callInfo.thisArg()->type() != MIRType::Object) {
        return InliningStatus_NotInlined;
    }
    if (callInfo.getArg(0)->type() != MIRType::String) {
        return InliningStatus_NotInlined;
    }
=======
IonBuilder::InliningResult IonBuilder::inlineArrayPopShift(
    CallInfo& callInfo, MArrayPopShift::Mode mode) {
  if (callInfo.constructing()) {
    trackOptimizationOutcome(TrackedOutcome::CantInlineNativeBadForm);
    return InliningStatus_NotInlined;
  }
>>>>>>> upstream-releases

<<<<<<< HEAD
  TemporaryTypeSet* thisTypes = obj->resultTypeSet();
  if (!thisTypes) {
    return InliningStatus_NotInlined;
  }
  const Class* clasp = thisTypes->getKnownClass(constraints());
  if (clasp != &ArrayObject::class_) {
    return InliningStatus_NotInlined;
  }
  if (thisTypes->hasObjectFlags(
          constraints(),
          OBJECT_FLAG_SPARSE_INDEXES | OBJECT_FLAG_LENGTH_OVERFLOW)) {
    trackOptimizationOutcome(TrackedOutcome::ArrayBadFlags);
    return InliningStatus_NotInlined;
  }
||||||| merged common ancestors
    // If we can confirm that the class is an array, the codegen
    // for MArrayJoin can be notified to check for common empty and one-item arrays.
    bool optimizeForArray = ([&](){
        TemporaryTypeSet* thisTypes = callInfo.thisArg()->resultTypeSet();
        if (!thisTypes) {
            return false;
        }
=======
  MIRType returnType = getInlineReturnType();
  if (returnType == MIRType::Undefined || returnType == MIRType::Null) {
    return InliningStatus_NotInlined;
  }
  if (callInfo.thisArg()->type() != MIRType::Object) {
    return InliningStatus_NotInlined;
  }

  // Pop and shift are only handled for dense arrays that have never been
  // used in an iterator: popping elements does not account for suppressing
  // deleted properties in active iterators.
  // Don't optimize shift if the array may be non-extensible (this matters
  // when there are holes). Don't optimize pop if the array may be
  // non-extensible, so we don't need to adjust the capacity for
  // non-extensible arrays (non-extensible objects always have a capacity
  // equal to their initialized length). We check this here because there's
  // no non-extensible ObjectElements flag so we would need an extra guard
  // on the BaseShape flags.
  ObjectGroupFlags unhandledFlags =
      OBJECT_FLAG_SPARSE_INDEXES | OBJECT_FLAG_LENGTH_OVERFLOW |
      OBJECT_FLAG_ITERATED | OBJECT_FLAG_NON_EXTENSIBLE_ELEMENTS;

  MDefinition* obj = callInfo.thisArg();
  TemporaryTypeSet* thisTypes = obj->resultTypeSet();
  if (!thisTypes) {
    return InliningStatus_NotInlined;
  }
  const Class* clasp = thisTypes->getKnownClass(constraints());
  if (clasp != &ArrayObject::class_) {
    return InliningStatus_NotInlined;
  }
  if (thisTypes->hasObjectFlags(constraints(), unhandledFlags)) {
    trackOptimizationOutcome(TrackedOutcome::ArrayBadFlags);
    return InliningStatus_NotInlined;
  }

  // Watch out for extra indexed properties on the object or its prototype.
  bool hasIndexedProperty;
  MOZ_TRY_VAR(hasIndexedProperty,
              ElementAccessHasExtraIndexedProperty(this, obj));
  if (hasIndexedProperty) {
    trackOptimizationOutcome(TrackedOutcome::ProtoIndexedProps);
    return InliningStatus_NotInlined;
  }
>>>>>>> upstream-releases

<<<<<<< HEAD
  bool hasIndexedProperty;
  MOZ_TRY_VAR(hasIndexedProperty,
              ArrayPrototypeHasIndexedProperty(this, script()));
  if (hasIndexedProperty) {
    trackOptimizationOutcome(TrackedOutcome::ProtoIndexedProps);
    return InliningStatus_NotInlined;
  }
||||||| merged common ancestors
        const Class* clasp = thisTypes->getKnownClass(constraints());
        if (clasp != &ArrayObject::class_) {
            return false;
        }
=======
  callInfo.setImplicitlyUsedUnchecked();
>>>>>>> upstream-releases

<<<<<<< HEAD
  TemporaryTypeSet::DoubleConversion conversion =
      thisTypes->convertDoubleElements(constraints());
  if (conversion == TemporaryTypeSet::AmbiguousDoubleConversion) {
    trackOptimizationOutcome(TrackedOutcome::ArrayDoubleConversion);
    return InliningStatus_NotInlined;
  }

  callInfo.setImplicitlyUsedUnchecked();

  bool toDouble = conversion == TemporaryTypeSet::AlwaysConvertToDoubles ||
                  conversion == TemporaryTypeSet::MaybeConvertToDoubles;

  obj = addMaybeCopyElementsForWrite(obj, /* checkNative = */ false);

  // If we have more than one argument, we are splitting the function into
  // multiple inlined calls to Array.push.
  //
  // Still, in case of bailouts, we have to keep the atomicity of the call, as
  // we cannot resume within Array.push function. To do so, we register a
  // resume point which would be captured by the upcoming MArrayPush
  // instructions, and this resume point contains an instruction which
  // truncates the length of the Array, to its original length in case of
  // bailouts, and resume before the Array.push call.
  MResumePoint* lastRp = nullptr;
  MInstruction* truncate = nullptr;
  if (callInfo.argc() > 1) {
    MInstruction* elements = MElements::New(alloc(), obj);
    MInstruction* length = MArrayLength::New(alloc(), elements);
    truncate = MSetArrayLength::New(alloc(), obj, length);
    truncate->setRecoveredOnBailout();

    current->add(elements);
    current->add(length);
    current->add(truncate);
||||||| merged common ancestors
        return true;
    })();
=======
  obj = addMaybeCopyElementsForWrite(obj, /* checkNative = */ false);
>>>>>>> upstream-releases

<<<<<<< HEAD
    // Restore the stack, such that resume points are created with the stack
    // as it was before the call.
    MOZ_TRY(callInfo.pushPriorCallStack(this, current));
  }

  MInstruction* ins = nullptr;
  for (uint32_t i = 0; i < callInfo.argc(); i++) {
    MDefinition* value = callInfo.getArg(i);
    if (toDouble) {
      MInstruction* valueDouble = MToDouble::New(alloc(), value);
      current->add(valueDouble);
      value = valueDouble;
    }
||||||| merged common ancestors
    callInfo.setImplicitlyUsedUnchecked();
=======
  TemporaryTypeSet* returnTypes = getInlineReturnTypeSet();
  bool needsHoleCheck =
      thisTypes->hasObjectFlags(constraints(), OBJECT_FLAG_NON_PACKED);
  bool maybeUndefined = returnTypes->hasType(TypeSet::UndefinedType());
>>>>>>> upstream-releases

<<<<<<< HEAD
    if (needsPostBarrier(value)) {
      MInstruction* elements = MElements::New(alloc(), obj);
      current->add(elements);
      MInstruction* initLength = MInitializedLength::New(alloc(), elements);
      current->add(initLength);
      current->add(
          MPostWriteElementBarrier::New(alloc(), obj, value, initLength));
    }
||||||| merged common ancestors
    MArrayJoin* ins = MArrayJoin::New(alloc(), callInfo.thisArg(), callInfo.getArg(0),
                                      optimizeForArray);
=======
  BarrierKind barrier = PropertyReadNeedsTypeBarrier(
      analysisContext, alloc(), constraints(), obj, nullptr, returnTypes);
  if (barrier != BarrierKind::NoBarrier) {
    returnType = MIRType::Value;
  }
>>>>>>> upstream-releases

<<<<<<< HEAD
    ins = MArrayPush::New(alloc(), obj, value);
    current->add(ins);
||||||| merged common ancestors
    current->add(ins);
    current->push(ins);
=======
  MArrayPopShift* ins =
      MArrayPopShift::New(alloc(), obj, mode, needsHoleCheck, maybeUndefined);
  current->add(ins);
  current->push(ins);
  ins->setResultType(returnType);
>>>>>>> upstream-releases

<<<<<<< HEAD
    if (callInfo.argc() > 1) {
      // Restore that call stack and the array length.
      MOZ_TRY(resumeAt(ins, pc));
      ins->resumePoint()->addStore(alloc(), truncate, lastRp);
      lastRp = ins->resumePoint();
    }
  }

  if (callInfo.argc() > 1) {
    // Fix the stack to represent the state after the call execution.
    callInfo.popPriorCallStack(current);
  }
  current->push(ins);

  if (callInfo.argc() > 1) {
    ins = MNop::New(alloc());
    current->add(ins);
  }

  MOZ_TRY(resumeAfter(ins));
  return InliningStatus_Inlined;
||||||| merged common ancestors
    MOZ_TRY(resumeAfter(ins));
    return InliningStatus_Inlined;
=======
  MOZ_TRY(resumeAfter(ins));
  MOZ_TRY(pushTypeBarrier(ins, returnTypes, barrier));
  return InliningStatus_Inlined;
>>>>>>> upstream-releases
}

<<<<<<< HEAD
IonBuilder::InliningResult IonBuilder::inlineArraySlice(CallInfo& callInfo) {
  if (callInfo.constructing()) {
    trackOptimizationOutcome(TrackedOutcome::CantInlineNativeBadForm);
    return InliningStatus_NotInlined;
  }

  MDefinition* obj = convertUnboxedObjects(callInfo.thisArg());

  // Ensure |this| and result are objects.
  if (getInlineReturnType() != MIRType::Object) {
    return InliningStatus_NotInlined;
  }
  if (obj->type() != MIRType::Object) {
    return InliningStatus_NotInlined;
  }
||||||| merged common ancestors
IonBuilder::InliningResult
IonBuilder::inlineArrayPush(CallInfo& callInfo)
{
    const uint32_t inlineArgsLimit = 10;
    if (callInfo.argc() < 1 || callInfo.argc() > inlineArgsLimit || callInfo.constructing()) {
        trackOptimizationOutcome(TrackedOutcome::CantInlineNativeBadForm);
        return InliningStatus_NotInlined;
    }

    // XXX bug 1493903.
    if (callInfo.argc() != 1) {
        trackOptimizationOutcome(TrackedOutcome::CantInlineNativeBadForm);
        return InliningStatus_NotInlined;
    }

    MDefinition* obj = convertUnboxedObjects(callInfo.thisArg());
    for (uint32_t i = 0; i < callInfo.argc(); i++) {
        MDefinition* value = callInfo.getArg(i);
        if (PropertyWriteNeedsTypeBarrier(alloc(), constraints(), current,
                                          &obj, nullptr, &value, /* canModify = */ false))
        {
            trackOptimizationOutcome(TrackedOutcome::NeedsTypeBarrier);
            return InliningStatus_NotInlined;
        }
    }
=======
IonBuilder::InliningResult IonBuilder::inlineArrayJoin(CallInfo& callInfo) {
  if (callInfo.argc() != 1 || callInfo.constructing()) {
    trackOptimizationOutcome(TrackedOutcome::CantInlineNativeBadForm);
    return InliningStatus_NotInlined;
  }
>>>>>>> upstream-releases

<<<<<<< HEAD
  // Arguments for the sliced region must be integers.
  if (callInfo.argc() > 0) {
    if (callInfo.getArg(0)->type() != MIRType::Int32) {
      return InliningStatus_NotInlined;
    }
    if (callInfo.argc() > 1) {
      if (callInfo.getArg(1)->type() != MIRType::Int32) {
        return InliningStatus_NotInlined;
      }
    }
  }
||||||| merged common ancestors
    if (getInlineReturnType() != MIRType::Int32) {
        return InliningStatus_NotInlined;
    }
    if (obj->type() != MIRType::Object) {
        return InliningStatus_NotInlined;
    }
=======
  if (getInlineReturnType() != MIRType::String) {
    return InliningStatus_NotInlined;
  }
  if (callInfo.thisArg()->type() != MIRType::Object) {
    return InliningStatus_NotInlined;
  }
  if (callInfo.getArg(0)->type() != MIRType::String) {
    return InliningStatus_NotInlined;
  }
>>>>>>> upstream-releases

<<<<<<< HEAD
  // |this| must be a dense array.
  TemporaryTypeSet* thisTypes = obj->resultTypeSet();
  if (!thisTypes) {
    return InliningStatus_NotInlined;
  }
||||||| merged common ancestors
    TemporaryTypeSet* thisTypes = obj->resultTypeSet();
    if (!thisTypes) {
        return InliningStatus_NotInlined;
    }
    const Class* clasp = thisTypes->getKnownClass(constraints());
    if (clasp != &ArrayObject::class_) {
        return InliningStatus_NotInlined;
    }
    if (thisTypes->hasObjectFlags(constraints(), OBJECT_FLAG_SPARSE_INDEXES |
                                  OBJECT_FLAG_LENGTH_OVERFLOW))
    {
        trackOptimizationOutcome(TrackedOutcome::ArrayBadFlags);
        return InliningStatus_NotInlined;
    }
=======
  // If we can confirm that the class is an array, the codegen
  // for MArrayJoin can be notified to check for common empty and one-item
  // arrays.
  bool optimizeForArray = ([&]() {
    TemporaryTypeSet* thisTypes = callInfo.thisArg()->resultTypeSet();
    if (!thisTypes) {
      return false;
    }

    const Class* clasp = thisTypes->getKnownClass(constraints());
    if (clasp != &ArrayObject::class_) {
      return false;
    }
>>>>>>> upstream-releases

<<<<<<< HEAD
  const Class* clasp = thisTypes->getKnownClass(constraints());
  if (clasp != &ArrayObject::class_) {
    return InliningStatus_NotInlined;
  }
  if (thisTypes->hasObjectFlags(
          constraints(),
          OBJECT_FLAG_SPARSE_INDEXES | OBJECT_FLAG_LENGTH_OVERFLOW)) {
    trackOptimizationOutcome(TrackedOutcome::ArrayBadFlags);
    return InliningStatus_NotInlined;
  }

  // Watch out for indexed properties on the prototype.
  bool hasIndexedProperty;
  MOZ_TRY_VAR(hasIndexedProperty,
              ArrayPrototypeHasIndexedProperty(this, script()));
  if (hasIndexedProperty) {
    trackOptimizationOutcome(TrackedOutcome::ProtoIndexedProps);
    return InliningStatus_NotInlined;
  }

  // The group of the result will be dynamically fixed up to match the input
  // object, allowing us to handle 'this' objects that might have more than
  // one group. Make sure that no singletons can be sliced here.
  for (unsigned i = 0; i < thisTypes->getObjectCount(); i++) {
    TypeSet::ObjectKey* key = thisTypes->getObject(i);
    if (key && key->isSingleton()) {
      return InliningStatus_NotInlined;
    }
  }

  // Inline the call.
  JSObject* templateObj =
      inspector->getTemplateObjectForNative(pc, js::array_slice);
  if (!templateObj) {
    return InliningStatus_NotInlined;
  }
||||||| merged common ancestors
    bool hasIndexedProperty;
    MOZ_TRY_VAR(hasIndexedProperty, ArrayPrototypeHasIndexedProperty(this, script()));
    if (hasIndexedProperty) {
        trackOptimizationOutcome(TrackedOutcome::ProtoIndexedProps);
        return InliningStatus_NotInlined;
    }
=======
    return true;
  })();
>>>>>>> upstream-releases

<<<<<<< HEAD
  if (!templateObj->is<ArrayObject>()) {
    return InliningStatus_NotInlined;
  }

  callInfo.setImplicitlyUsedUnchecked();

  MDefinition* begin;
  if (callInfo.argc() > 0) {
    begin = callInfo.getArg(0);
  } else {
    begin = constant(Int32Value(0));
  }

  MDefinition* end;
  if (callInfo.argc() > 1) {
    end = callInfo.getArg(1);
  } else if (clasp == &ArrayObject::class_) {
    MElements* elements = MElements::New(alloc(), obj);
    current->add(elements);

    end = MArrayLength::New(alloc(), elements);
    current->add(end->toInstruction());
  }

  MArraySlice* ins =
      MArraySlice::New(alloc(), obj, begin, end, templateObj,
                       templateObj->group()->initialHeap(constraints()));
  current->add(ins);
  current->push(ins);

  MOZ_TRY(resumeAfter(ins));
  MOZ_TRY(pushTypeBarrier(ins, getInlineReturnTypeSet(), BarrierKind::TypeSet));
  return InliningStatus_Inlined;
}

IonBuilder::InliningResult IonBuilder::inlineBoolean(CallInfo& callInfo) {
  if (callInfo.constructing()) {
    trackOptimizationOutcome(TrackedOutcome::CantInlineNativeBadForm);
    return InliningStatus_NotInlined;
  }
||||||| merged common ancestors
    TemporaryTypeSet::DoubleConversion conversion =
        thisTypes->convertDoubleElements(constraints());
    if (conversion == TemporaryTypeSet::AmbiguousDoubleConversion) {
        trackOptimizationOutcome(TrackedOutcome::ArrayDoubleConversion);
        return InliningStatus_NotInlined;
    }
=======
  callInfo.setImplicitlyUsedUnchecked();
>>>>>>> upstream-releases

<<<<<<< HEAD
  if (getInlineReturnType() != MIRType::Boolean) {
    return InliningStatus_NotInlined;
  }
||||||| merged common ancestors
    callInfo.setImplicitlyUsedUnchecked();
=======
  MArrayJoin* ins = MArrayJoin::New(alloc(), callInfo.thisArg(),
                                    callInfo.getArg(0), optimizeForArray);
>>>>>>> upstream-releases

<<<<<<< HEAD
  callInfo.setImplicitlyUsedUnchecked();
||||||| merged common ancestors
    bool toDouble =
        conversion == TemporaryTypeSet::AlwaysConvertToDoubles ||
        conversion == TemporaryTypeSet::MaybeConvertToDoubles;

    obj = addMaybeCopyElementsForWrite(obj, /* checkNative = */ false);

    // If we have more than one argument, we are splitting the function into
    // multiple inlined calls to Array.push.
    //
    // Still, in case of bailouts, we have to keep the atomicity of the call, as
    // we cannot resume within Array.push function. To do so, we register a
    // resume point which would be captured by the upcoming MArrayPush
    // instructions, and this resume point contains an instruction which
    // truncates the length of the Array, to its original length in case of
    // bailouts, and resume before the Array.push call.
    MResumePoint* lastRp = nullptr;
    MInstruction* truncate = nullptr;
    if (callInfo.argc() > 1) {
        MInstruction* elements = MElements::New(alloc(), obj);
        MInstruction* length = MArrayLength::New(alloc(), elements);
        truncate = MSetArrayLength::New(alloc(), obj, length);
        truncate->setRecoveredOnBailout();

        current->add(elements);
        current->add(length);
        current->add(truncate);

        // Restore the stack, such that resume points are created with the stack
        // as it was before the call.
        MOZ_TRY(callInfo.pushPriorCallStack(this, current));
    }
=======
  current->add(ins);
  current->push(ins);
>>>>>>> upstream-releases

<<<<<<< HEAD
  if (callInfo.argc() > 0) {
    MDefinition* result = convertToBoolean(callInfo.getArg(0));
    current->push(result);
  } else {
    pushConstant(BooleanValue(false));
  }
  return InliningStatus_Inlined;
}

IonBuilder::InliningResult IonBuilder::inlineNewIterator(
    CallInfo& callInfo, MNewIterator::Type type) {
  MOZ_ASSERT(!callInfo.constructing());
  MOZ_ASSERT(callInfo.argc() == 0);

  JSObject* templateObject = nullptr;
  switch (type) {
    case MNewIterator::ArrayIterator:
      templateObject = inspector->getTemplateObjectForNative(
          pc, js::intrinsic_NewArrayIterator);
      MOZ_ASSERT_IF(templateObject, templateObject->is<ArrayIteratorObject>());
      break;
    case MNewIterator::StringIterator:
      templateObject = inspector->getTemplateObjectForNative(
          pc, js::intrinsic_NewStringIterator);
      MOZ_ASSERT_IF(templateObject, templateObject->is<StringIteratorObject>());
      break;
  }

  if (!templateObject) {
    return InliningStatus_NotInlined;
  }
||||||| merged common ancestors
    MInstruction* ins = nullptr;
    for (uint32_t i = 0; i < callInfo.argc(); i++) {
        MDefinition* value = callInfo.getArg(i);
        if (toDouble) {
            MInstruction* valueDouble = MToDouble::New(alloc(), value);
            current->add(valueDouble);
            value = valueDouble;
        }
=======
  MOZ_TRY(resumeAfter(ins));
  return InliningStatus_Inlined;
}
>>>>>>> upstream-releases

<<<<<<< HEAD
  callInfo.setImplicitlyUsedUnchecked();
||||||| merged common ancestors
        if (needsPostBarrier(value)) {
            MInstruction* elements = MElements::New(alloc(), obj);
            current->add(elements);
            MInstruction* initLength = MInitializedLength::New(alloc(), elements);
            current->add(initLength);
            current->add(MPostWriteElementBarrier::New(alloc(), obj, value, initLength));
        }
=======
IonBuilder::InliningResult IonBuilder::inlineArrayPush(CallInfo& callInfo) {
  const uint32_t inlineArgsLimit = 10;
  if (callInfo.argc() < 1 || callInfo.argc() > inlineArgsLimit ||
      callInfo.constructing()) {
    trackOptimizationOutcome(TrackedOutcome::CantInlineNativeBadForm);
    return InliningStatus_NotInlined;
  }
>>>>>>> upstream-releases

<<<<<<< HEAD
  MConstant* templateConst =
      MConstant::NewConstraintlessObject(alloc(), templateObject);
  current->add(templateConst);
||||||| merged common ancestors
        ins = MArrayPush::New(alloc(), obj, value);
        current->add(ins);
=======
  // XXX bug 1493903.
  if (callInfo.argc() != 1) {
    trackOptimizationOutcome(TrackedOutcome::CantInlineNativeBadForm);
    return InliningStatus_NotInlined;
  }
>>>>>>> upstream-releases

<<<<<<< HEAD
  MNewIterator* ins =
      MNewIterator::New(alloc(), constraints(), templateConst, type);
  current->add(ins);
  current->push(ins);
||||||| merged common ancestors
        if (callInfo.argc() > 1) {
            // Restore that call stack and the array length.
            MOZ_TRY(resumeAt(ins, pc));
            ins->resumePoint()->addStore(alloc(), truncate, lastRp);
            lastRp = ins->resumePoint();
        }
    }
=======
  MDefinition* obj = callInfo.thisArg();
  for (uint32_t i = 0; i < callInfo.argc(); i++) {
    MDefinition* value = callInfo.getArg(i);
    if (PropertyWriteNeedsTypeBarrier(alloc(), constraints(), current, &obj,
                                      nullptr, &value,
                                      /* canModify = */ false)) {
      trackOptimizationOutcome(TrackedOutcome::NeedsTypeBarrier);
      return InliningStatus_NotInlined;
    }
  }
>>>>>>> upstream-releases

<<<<<<< HEAD
  MOZ_TRY(resumeAfter(ins));
  return InliningStatus_Inlined;
}
||||||| merged common ancestors
    if (callInfo.argc() > 1) {
        // Fix the stack to represent the state after the call execution.
        callInfo.popPriorCallStack(current);
    }
    current->push(ins);
=======
  if (getInlineReturnType() != MIRType::Int32) {
    return InliningStatus_NotInlined;
  }
  if (obj->type() != MIRType::Object) {
    return InliningStatus_NotInlined;
  }
>>>>>>> upstream-releases

<<<<<<< HEAD
IonBuilder::InliningResult IonBuilder::inlineMathAbs(CallInfo& callInfo) {
  if (callInfo.argc() != 1 || callInfo.constructing()) {
    trackOptimizationOutcome(TrackedOutcome::CantInlineNativeBadForm);
    return InliningStatus_NotInlined;
  }
||||||| merged common ancestors
    if (callInfo.argc() > 1) {
        ins = MNop::New(alloc());
        current->add(ins);
    }
=======
  TemporaryTypeSet* thisTypes = obj->resultTypeSet();
  if (!thisTypes) {
    return InliningStatus_NotInlined;
  }
  const Class* clasp = thisTypes->getKnownClass(constraints());
  if (clasp != &ArrayObject::class_) {
    return InliningStatus_NotInlined;
  }
>>>>>>> upstream-releases

<<<<<<< HEAD
  MIRType returnType = getInlineReturnType();
  MIRType argType = callInfo.getArg(0)->type();
  if (!IsNumberType(argType)) {
    return InliningStatus_NotInlined;
  }

  // Either argType == returnType, or
  //        argType == Double or Float32, returnType == Int, or
  //        argType == Float32, returnType == Double
  if (argType != returnType &&
      !(IsFloatingPointType(argType) && returnType == MIRType::Int32) &&
      !(argType == MIRType::Float32 && returnType == MIRType::Double)) {
    return InliningStatus_NotInlined;
  }

  callInfo.setImplicitlyUsedUnchecked();

  // If the arg is a Float32, we specialize the op as double, it will be
  // specialized as float32 if necessary later.
  MIRType absType = (argType == MIRType::Float32) ? MIRType::Double : argType;
  MInstruction* ins = MAbs::New(alloc(), callInfo.getArg(0), absType);
  current->add(ins);

  current->push(ins);
  return InliningStatus_Inlined;
}
||||||| merged common ancestors
    MOZ_TRY(resumeAfter(ins));
    return InliningStatus_Inlined;
}
=======
  bool hasIndexedProperty;
  MOZ_TRY_VAR(hasIndexedProperty,
              ElementAccessHasExtraIndexedProperty(this, obj));
  if (hasIndexedProperty) {
    trackOptimizationOutcome(TrackedOutcome::ProtoIndexedProps);
    return InliningStatus_NotInlined;
  }
>>>>>>> upstream-releases

<<<<<<< HEAD
IonBuilder::InliningResult IonBuilder::inlineMathFloor(CallInfo& callInfo) {
  if (callInfo.argc() != 1 || callInfo.constructing()) {
    trackOptimizationOutcome(TrackedOutcome::CantInlineNativeBadForm);
    return InliningStatus_NotInlined;
  }
||||||| merged common ancestors
IonBuilder::InliningResult
IonBuilder::inlineArraySlice(CallInfo& callInfo)
{
    if (callInfo.constructing()) {
        trackOptimizationOutcome(TrackedOutcome::CantInlineNativeBadForm);
        return InliningStatus_NotInlined;
    }
=======
  TemporaryTypeSet::DoubleConversion conversion =
      thisTypes->convertDoubleElements(constraints());
  if (conversion == TemporaryTypeSet::AmbiguousDoubleConversion) {
    trackOptimizationOutcome(TrackedOutcome::ArrayDoubleConversion);
    return InliningStatus_NotInlined;
  }

  callInfo.setImplicitlyUsedUnchecked();

  bool toDouble = conversion == TemporaryTypeSet::AlwaysConvertToDoubles ||
                  conversion == TemporaryTypeSet::MaybeConvertToDoubles;

  obj = addMaybeCopyElementsForWrite(obj, /* checkNative = */ false);

  // If we have more than one argument, we are splitting the function into
  // multiple inlined calls to Array.push.
  //
  // Still, in case of bailouts, we have to keep the atomicity of the call, as
  // we cannot resume within Array.push function. To do so, we register a
  // resume point which would be captured by the upcoming MArrayPush
  // instructions, and this resume point contains an instruction which
  // truncates the length of the Array, to its original length in case of
  // bailouts, and resume before the Array.push call.
  MResumePoint* lastRp = nullptr;
  MInstruction* truncate = nullptr;
  if (callInfo.argc() > 1) {
    MInstruction* elements = MElements::New(alloc(), obj);
    MInstruction* length = MArrayLength::New(alloc(), elements);
    truncate = MSetArrayLength::New(alloc(), obj, length);
    truncate->setRecoveredOnBailout();

    current->add(elements);
    current->add(length);
    current->add(truncate);
>>>>>>> upstream-releases

<<<<<<< HEAD
  MIRType argType = callInfo.getArg(0)->type();
  MIRType returnType = getInlineReturnType();
||||||| merged common ancestors
    MDefinition* obj = convertUnboxedObjects(callInfo.thisArg());
=======
    // Restore the stack, such that resume points are created with the stack
    // as it was before the call.
    MOZ_TRY(callInfo.pushPriorCallStack(this, current));
  }
>>>>>>> upstream-releases

<<<<<<< HEAD
  // Math.floor(int(x)) == int(x)
  if (argType == MIRType::Int32 && returnType == MIRType::Int32) {
    callInfo.setImplicitlyUsedUnchecked();
    // The int operand may be something which bails out if the actual value
    // is not in the range of the result type of the MIR. We need to tell
    // the optimizer to preserve this bailout even if the final result is
    // fully truncated.
    MLimitedTruncate* ins = MLimitedTruncate::New(
        alloc(), callInfo.getArg(0), MDefinition::IndirectTruncate);
    current->add(ins);
    current->push(ins);
    return InliningStatus_Inlined;
  }
||||||| merged common ancestors
    // Ensure |this| and result are objects.
    if (getInlineReturnType() != MIRType::Object) {
        return InliningStatus_NotInlined;
    }
    if (obj->type() != MIRType::Object) {
        return InliningStatus_NotInlined;
    }
=======
  MInstruction* ins = nullptr;
  for (uint32_t i = 0; i < callInfo.argc(); i++) {
    MDefinition* value = callInfo.getArg(i);
    if (toDouble) {
      MInstruction* valueDouble = MToDouble::New(alloc(), value);
      current->add(valueDouble);
      value = valueDouble;
    }
>>>>>>> upstream-releases

<<<<<<< HEAD
  if (IsFloatingPointType(argType)) {
    if (returnType == MIRType::Int32) {
      callInfo.setImplicitlyUsedUnchecked();
      MFloor* ins = MFloor::New(alloc(), callInfo.getArg(0));
      current->add(ins);
      current->push(ins);
      return InliningStatus_Inlined;
||||||| merged common ancestors
    // Arguments for the sliced region must be integers.
    if (callInfo.argc() > 0) {
        if (callInfo.getArg(0)->type() != MIRType::Int32) {
            return InliningStatus_NotInlined;
        }
        if (callInfo.argc() > 1) {
            if (callInfo.getArg(1)->type() != MIRType::Int32) {
                return InliningStatus_NotInlined;
            }
        }
=======
    if (needsPostBarrier(value)) {
      MInstruction* elements = MElements::New(alloc(), obj);
      current->add(elements);
      MInstruction* initLength = MInitializedLength::New(alloc(), elements);
      current->add(initLength);
      current->add(
          MPostWriteElementBarrier::New(alloc(), obj, value, initLength));
>>>>>>> upstream-releases
    }

<<<<<<< HEAD
    if (returnType == MIRType::Double) {
      callInfo.setImplicitlyUsedUnchecked();
||||||| merged common ancestors
    // |this| must be a dense array.
    TemporaryTypeSet* thisTypes = obj->resultTypeSet();
    if (!thisTypes) {
        return InliningStatus_NotInlined;
    }
=======
    ins = MArrayPush::New(alloc(), obj, value);
    current->add(ins);
>>>>>>> upstream-releases

<<<<<<< HEAD
      MInstruction* ins = nullptr;
      if (MNearbyInt::HasAssemblerSupport(RoundingMode::Down)) {
        ins = MNearbyInt::New(alloc(), callInfo.getArg(0), argType,
                              RoundingMode::Down);
      } else {
        ins = MMathFunction::New(alloc(), callInfo.getArg(0),
                                 MMathFunction::Floor);
      }
||||||| merged common ancestors
    const Class* clasp = thisTypes->getKnownClass(constraints());
    if (clasp != &ArrayObject::class_) {
        return InliningStatus_NotInlined;
    }
    if (thisTypes->hasObjectFlags(constraints(), OBJECT_FLAG_SPARSE_INDEXES |
                                  OBJECT_FLAG_LENGTH_OVERFLOW))
    {
        trackOptimizationOutcome(TrackedOutcome::ArrayBadFlags);
        return InliningStatus_NotInlined;
    }
=======
    if (callInfo.argc() > 1) {
      // Restore that call stack and the array length.
      MOZ_TRY(resumeAt(ins, pc));
      ins->resumePoint()->addStore(alloc(), truncate, lastRp);
      lastRp = ins->resumePoint();
    }
  }
>>>>>>> upstream-releases

<<<<<<< HEAD
      current->add(ins);
      current->push(ins);
      return InliningStatus_Inlined;
    }
  }
||||||| merged common ancestors
    // Watch out for indexed properties on the prototype.
    bool hasIndexedProperty;
    MOZ_TRY_VAR(hasIndexedProperty, ArrayPrototypeHasIndexedProperty(this, script()));
    if (hasIndexedProperty) {
        trackOptimizationOutcome(TrackedOutcome::ProtoIndexedProps);
        return InliningStatus_NotInlined;
    }
=======
  if (callInfo.argc() > 1) {
    // Fix the stack to represent the state after the call execution.
    callInfo.popPriorCallStack(current);
  }
  current->push(ins);
>>>>>>> upstream-releases

<<<<<<< HEAD
  return InliningStatus_NotInlined;
}
||||||| merged common ancestors
    // The group of the result will be dynamically fixed up to match the input
    // object, allowing us to handle 'this' objects that might have more than
    // one group. Make sure that no singletons can be sliced here.
    for (unsigned i = 0; i < thisTypes->getObjectCount(); i++) {
        TypeSet::ObjectKey* key = thisTypes->getObject(i);
        if (key && key->isSingleton()) {
            return InliningStatus_NotInlined;
        }
    }
=======
  if (callInfo.argc() > 1) {
    ins = MNop::New(alloc());
    current->add(ins);
  }
>>>>>>> upstream-releases

<<<<<<< HEAD
IonBuilder::InliningResult IonBuilder::inlineMathCeil(CallInfo& callInfo) {
  if (callInfo.argc() != 1 || callInfo.constructing()) {
    trackOptimizationOutcome(TrackedOutcome::CantInlineNativeBadForm);
    return InliningStatus_NotInlined;
  }
||||||| merged common ancestors
    // Inline the call.
    JSObject* templateObj = inspector->getTemplateObjectForNative(pc, js::array_slice);
    if (!templateObj) {
        return InliningStatus_NotInlined;
    }
=======
  MOZ_TRY(resumeAfter(ins));
  return InliningStatus_Inlined;
}
>>>>>>> upstream-releases

<<<<<<< HEAD
  MIRType argType = callInfo.getArg(0)->type();
  MIRType returnType = getInlineReturnType();
||||||| merged common ancestors
    if (!templateObj->is<ArrayObject>()) {
        return InliningStatus_NotInlined;
    }
=======
IonBuilder::InliningResult IonBuilder::inlineArraySlice(CallInfo& callInfo) {
  if (callInfo.constructing()) {
    trackOptimizationOutcome(TrackedOutcome::CantInlineNativeBadForm);
    return InliningStatus_NotInlined;
  }
>>>>>>> upstream-releases

<<<<<<< HEAD
  // Math.ceil(int(x)) == int(x)
  if (argType == MIRType::Int32 && returnType == MIRType::Int32) {
    callInfo.setImplicitlyUsedUnchecked();
    // The int operand may be something which bails out if the actual value
    // is not in the range of the result type of the MIR. We need to tell
    // the optimizer to preserve this bailout even if the final result is
    // fully truncated.
    MLimitedTruncate* ins = MLimitedTruncate::New(
        alloc(), callInfo.getArg(0), MDefinition::IndirectTruncate);
    current->add(ins);
    current->push(ins);
    return InliningStatus_Inlined;
  }
||||||| merged common ancestors
    callInfo.setImplicitlyUsedUnchecked();
=======
  MDefinition* obj = callInfo.thisArg();
>>>>>>> upstream-releases

<<<<<<< HEAD
  if (IsFloatingPointType(argType)) {
    if (returnType == MIRType::Int32) {
      callInfo.setImplicitlyUsedUnchecked();
      MCeil* ins = MCeil::New(alloc(), callInfo.getArg(0));
      current->add(ins);
      current->push(ins);
      return InliningStatus_Inlined;
    }
||||||| merged common ancestors
    MDefinition* begin;
    if (callInfo.argc() > 0) {
        begin = callInfo.getArg(0);
    } else {
        begin = constant(Int32Value(0));
    }
=======
  // Ensure |this| and result are objects.
  if (getInlineReturnType() != MIRType::Object) {
    return InliningStatus_NotInlined;
  }
  if (obj->type() != MIRType::Object) {
    return InliningStatus_NotInlined;
  }
>>>>>>> upstream-releases

<<<<<<< HEAD
    if (returnType == MIRType::Double) {
      callInfo.setImplicitlyUsedUnchecked();

      MInstruction* ins = nullptr;
      if (MNearbyInt::HasAssemblerSupport(RoundingMode::Up)) {
        ins = MNearbyInt::New(alloc(), callInfo.getArg(0), argType,
                              RoundingMode::Up);
      } else {
        ins = MMathFunction::New(alloc(), callInfo.getArg(0),
                                 MMathFunction::Ceil);
      }
||||||| merged common ancestors
    MDefinition* end;
    if (callInfo.argc() > 1) {
        end = callInfo.getArg(1);
    } else if (clasp == &ArrayObject::class_) {
        MElements* elements = MElements::New(alloc(), obj);
        current->add(elements);

        end = MArrayLength::New(alloc(), elements);
        current->add(end->toInstruction());
    }
=======
  // Arguments for the sliced region must be integers.
  if (callInfo.argc() > 0) {
    if (callInfo.getArg(0)->type() != MIRType::Int32) {
      return InliningStatus_NotInlined;
    }
    if (callInfo.argc() > 1) {
      if (callInfo.getArg(1)->type() != MIRType::Int32) {
        return InliningStatus_NotInlined;
      }
    }
  }
>>>>>>> upstream-releases

<<<<<<< HEAD
      current->add(ins);
      current->push(ins);
      return InliningStatus_Inlined;
    }
  }
||||||| merged common ancestors
    MArraySlice* ins = MArraySlice::New(alloc(),
                                        obj, begin, end,
                                        templateObj,
                                        templateObj->group()->initialHeap(constraints()));
    current->add(ins);
    current->push(ins);
=======
  // |this| must be a dense array.
  TemporaryTypeSet* thisTypes = obj->resultTypeSet();
  if (!thisTypes) {
    return InliningStatus_NotInlined;
  }
>>>>>>> upstream-releases

<<<<<<< HEAD
  return InliningStatus_NotInlined;
||||||| merged common ancestors
    MOZ_TRY(resumeAfter(ins));
    MOZ_TRY(pushTypeBarrier(ins, getInlineReturnTypeSet(), BarrierKind::TypeSet));
    return InliningStatus_Inlined;
=======
  const Class* clasp = thisTypes->getKnownClass(constraints());
  if (clasp != &ArrayObject::class_) {
    return InliningStatus_NotInlined;
  }

  // Watch out for extra indexed properties on the object or its prototype.
  bool hasIndexedProperty;
  MOZ_TRY_VAR(hasIndexedProperty,
              ElementAccessHasExtraIndexedProperty(this, obj));
  if (hasIndexedProperty) {
    trackOptimizationOutcome(TrackedOutcome::ProtoIndexedProps);
    return InliningStatus_NotInlined;
  }

  // The group of the result will be dynamically fixed up to match the input
  // object, allowing us to handle 'this' objects that might have more than
  // one group. Make sure that no singletons can be sliced here.
  for (unsigned i = 0; i < thisTypes->getObjectCount(); i++) {
    TypeSet::ObjectKey* key = thisTypes->getObject(i);
    if (key && key->isSingleton()) {
      return InliningStatus_NotInlined;
    }
  }

  // Inline the call.
  JSObject* templateObj =
      inspector->getTemplateObjectForNative(pc, js::array_slice);
  if (!templateObj) {
    return InliningStatus_NotInlined;
  }

  if (!templateObj->is<ArrayObject>()) {
    return InliningStatus_NotInlined;
  }

  callInfo.setImplicitlyUsedUnchecked();

  MDefinition* begin;
  if (callInfo.argc() > 0) {
    begin = callInfo.getArg(0);
  } else {
    begin = constant(Int32Value(0));
  }

  MDefinition* end;
  if (callInfo.argc() > 1) {
    end = callInfo.getArg(1);
  } else if (clasp == &ArrayObject::class_) {
    MElements* elements = MElements::New(alloc(), obj);
    current->add(elements);

    end = MArrayLength::New(alloc(), elements);
    current->add(end->toInstruction());
  }

  MArraySlice* ins =
      MArraySlice::New(alloc(), obj, begin, end, templateObj,
                       templateObj->group()->initialHeap(constraints()));
  current->add(ins);
  current->push(ins);

  MOZ_TRY(resumeAfter(ins));
  MOZ_TRY(pushTypeBarrier(ins, getInlineReturnTypeSet(), BarrierKind::TypeSet));
  return InliningStatus_Inlined;
>>>>>>> upstream-releases
}

<<<<<<< HEAD
IonBuilder::InliningResult IonBuilder::inlineMathClz32(CallInfo& callInfo) {
  if (callInfo.argc() != 1 || callInfo.constructing()) {
    trackOptimizationOutcome(TrackedOutcome::CantInlineNativeBadForm);
    return InliningStatus_NotInlined;
  }
||||||| merged common ancestors
IonBuilder::InliningResult
IonBuilder::inlineBoolean(CallInfo& callInfo)
{
    if (callInfo.constructing()) {
        trackOptimizationOutcome(TrackedOutcome::CantInlineNativeBadForm);
        return InliningStatus_NotInlined;
    }
=======
IonBuilder::InliningResult IonBuilder::inlineBoolean(CallInfo& callInfo) {
  if (callInfo.constructing()) {
    trackOptimizationOutcome(TrackedOutcome::CantInlineNativeBadForm);
    return InliningStatus_NotInlined;
  }
>>>>>>> upstream-releases

<<<<<<< HEAD
  MIRType returnType = getInlineReturnType();
  if (returnType != MIRType::Int32) {
    return InliningStatus_NotInlined;
  }
||||||| merged common ancestors
    if (getInlineReturnType() != MIRType::Boolean) {
        return InliningStatus_NotInlined;
    }
=======
  if (getInlineReturnType() != MIRType::Boolean) {
    return InliningStatus_NotInlined;
  }
>>>>>>> upstream-releases

<<<<<<< HEAD
  if (!IsNumberType(callInfo.getArg(0)->type())) {
    return InliningStatus_NotInlined;
  }
||||||| merged common ancestors
    callInfo.setImplicitlyUsedUnchecked();
=======
  callInfo.setImplicitlyUsedUnchecked();
>>>>>>> upstream-releases

<<<<<<< HEAD
  callInfo.setImplicitlyUsedUnchecked();

  MClz* ins = MClz::New(alloc(), callInfo.getArg(0), MIRType::Int32);
  current->add(ins);
  current->push(ins);
  return InliningStatus_Inlined;
||||||| merged common ancestors
    if (callInfo.argc() > 0) {
        MDefinition* result = convertToBoolean(callInfo.getArg(0));
        current->push(result);
    } else {
        pushConstant(BooleanValue(false));
    }
    return InliningStatus_Inlined;
=======
  if (callInfo.argc() > 0) {
    MDefinition* result = convertToBoolean(callInfo.getArg(0));
    current->push(result);
  } else {
    pushConstant(BooleanValue(false));
  }
  return InliningStatus_Inlined;
>>>>>>> upstream-releases
}

<<<<<<< HEAD
IonBuilder::InliningResult IonBuilder::inlineMathRound(CallInfo& callInfo) {
  if (callInfo.argc() != 1 || callInfo.constructing()) {
    trackOptimizationOutcome(TrackedOutcome::CantInlineNativeBadForm);
    return InliningStatus_NotInlined;
  }
||||||| merged common ancestors
IonBuilder::InliningResult
IonBuilder::inlineNewIterator(CallInfo& callInfo, MNewIterator::Type type)
{
    MOZ_ASSERT(!callInfo.constructing());
    MOZ_ASSERT(callInfo.argc() == 0);

    JSObject* templateObject = nullptr;
    switch (type) {
      case MNewIterator::ArrayIterator:
        templateObject = inspector->getTemplateObjectForNative(pc, js::intrinsic_NewArrayIterator);
        MOZ_ASSERT_IF(templateObject, templateObject->is<ArrayIteratorObject>());
        break;
      case MNewIterator::StringIterator:
        templateObject = inspector->getTemplateObjectForNative(pc, js::intrinsic_NewStringIterator);
        MOZ_ASSERT_IF(templateObject, templateObject->is<StringIteratorObject>());
        break;
    }
=======
IonBuilder::InliningResult IonBuilder::inlineNewIterator(
    CallInfo& callInfo, MNewIterator::Type type) {
  MOZ_ASSERT(!callInfo.constructing());
  MOZ_ASSERT(callInfo.argc() == 0);

  JSObject* templateObject = nullptr;
  switch (type) {
    case MNewIterator::ArrayIterator:
      templateObject = inspector->getTemplateObjectForNative(
          pc, js::intrinsic_NewArrayIterator);
      MOZ_ASSERT_IF(templateObject, templateObject->is<ArrayIteratorObject>());
      break;
    case MNewIterator::StringIterator:
      templateObject = inspector->getTemplateObjectForNative(
          pc, js::intrinsic_NewStringIterator);
      MOZ_ASSERT_IF(templateObject, templateObject->is<StringIteratorObject>());
      break;
    case MNewIterator::RegExpStringIterator:
      templateObject = inspector->getTemplateObjectForNative(
          pc, js::intrinsic_NewRegExpStringIterator);
      MOZ_ASSERT_IF(templateObject,
                    templateObject->is<RegExpStringIteratorObject>());
      break;
  }

  if (!templateObject) {
    return InliningStatus_NotInlined;
  }
>>>>>>> upstream-releases

<<<<<<< HEAD
  MIRType returnType = getInlineReturnType();
  MIRType argType = callInfo.getArg(0)->type();
||||||| merged common ancestors
    if (!templateObject) {
        return InliningStatus_NotInlined;
    }
=======
  callInfo.setImplicitlyUsedUnchecked();
>>>>>>> upstream-releases

<<<<<<< HEAD
  // Math.round(int(x)) == int(x)
  if (argType == MIRType::Int32 && returnType == MIRType::Int32) {
    callInfo.setImplicitlyUsedUnchecked();
    // The int operand may be something which bails out if the actual value
    // is not in the range of the result type of the MIR. We need to tell
    // the optimizer to preserve this bailout even if the final result is
    // fully truncated.
    MLimitedTruncate* ins = MLimitedTruncate::New(
        alloc(), callInfo.getArg(0), MDefinition::IndirectTruncate);
    current->add(ins);
    current->push(ins);
    return InliningStatus_Inlined;
  }
||||||| merged common ancestors
    callInfo.setImplicitlyUsedUnchecked();
=======
  MConstant* templateConst =
      MConstant::NewConstraintlessObject(alloc(), templateObject);
  current->add(templateConst);
>>>>>>> upstream-releases

<<<<<<< HEAD
  if (IsFloatingPointType(argType) && returnType == MIRType::Int32) {
    callInfo.setImplicitlyUsedUnchecked();
    MRound* ins = MRound::New(alloc(), callInfo.getArg(0));
    current->add(ins);
    current->push(ins);
    return InliningStatus_Inlined;
  }
||||||| merged common ancestors
    MConstant* templateConst = MConstant::NewConstraintlessObject(alloc(), templateObject);
    current->add(templateConst);

    MNewIterator* ins = MNewIterator::New(alloc(), constraints(), templateConst, type);
    current->add(ins);
    current->push(ins);
=======
  MNewIterator* ins =
      MNewIterator::New(alloc(), constraints(), templateConst, type);
  current->add(ins);
  current->push(ins);

  MOZ_TRY(resumeAfter(ins));
  return InliningStatus_Inlined;
}
>>>>>>> upstream-releases

<<<<<<< HEAD
  if (IsFloatingPointType(argType) && returnType == MIRType::Double) {
    callInfo.setImplicitlyUsedUnchecked();
    MMathFunction* ins =
        MMathFunction::New(alloc(), callInfo.getArg(0), MMathFunction::Round);
    current->add(ins);
    current->push(ins);
    return InliningStatus_Inlined;
  }

  return InliningStatus_NotInlined;
||||||| merged common ancestors
    MOZ_TRY(resumeAfter(ins));
    return InliningStatus_Inlined;
=======
IonBuilder::InliningResult IonBuilder::inlineArrayIteratorPrototypeOptimizable(
    CallInfo& callInfo) {
  MOZ_ASSERT(!callInfo.constructing());
  MOZ_ASSERT(callInfo.argc() == 0);

  if (!ensureArrayIteratorPrototypeNextNotModified()) {
    return InliningStatus_NotInlined;
  }

  callInfo.setImplicitlyUsedUnchecked();
  pushConstant(BooleanValue(true));
  return InliningStatus_Inlined;
>>>>>>> upstream-releases
}

<<<<<<< HEAD
IonBuilder::InliningResult IonBuilder::inlineMathSqrt(CallInfo& callInfo) {
  if (callInfo.argc() != 1 || callInfo.constructing()) {
    trackOptimizationOutcome(TrackedOutcome::CantInlineNativeBadForm);
    return InliningStatus_NotInlined;
  }
||||||| merged common ancestors
IonBuilder::InliningResult
IonBuilder::inlineMathAbs(CallInfo& callInfo)
{
    if (callInfo.argc() != 1 || callInfo.constructing()) {
        trackOptimizationOutcome(TrackedOutcome::CantInlineNativeBadForm);
        return InliningStatus_NotInlined;
    }
=======
IonBuilder::InliningResult IonBuilder::inlineMathAbs(CallInfo& callInfo) {
  if (callInfo.argc() != 1 || callInfo.constructing()) {
    trackOptimizationOutcome(TrackedOutcome::CantInlineNativeBadForm);
    return InliningStatus_NotInlined;
  }
>>>>>>> upstream-releases

<<<<<<< HEAD
  MIRType argType = callInfo.getArg(0)->type();
  if (getInlineReturnType() != MIRType::Double) {
    return InliningStatus_NotInlined;
  }
  if (!IsNumberType(argType)) {
    return InliningStatus_NotInlined;
  }
||||||| merged common ancestors
    MIRType returnType = getInlineReturnType();
    MIRType argType = callInfo.getArg(0)->type();
    if (!IsNumberType(argType)) {
        return InliningStatus_NotInlined;
    }
=======
  MIRType returnType = getInlineReturnType();
  MIRType argType = callInfo.getArg(0)->type();
  if (!IsNumberType(argType)) {
    return InliningStatus_NotInlined;
  }

  // Either argType == returnType, or
  //        argType == Double or Float32, returnType == Int, or
  //        argType == Float32, returnType == Double
  if (argType != returnType &&
      !(IsFloatingPointType(argType) && returnType == MIRType::Int32) &&
      !(argType == MIRType::Float32 && returnType == MIRType::Double)) {
    return InliningStatus_NotInlined;
  }
>>>>>>> upstream-releases

  callInfo.setImplicitlyUsedUnchecked();

<<<<<<< HEAD
  MSqrt* sqrt = MSqrt::New(alloc(), callInfo.getArg(0), MIRType::Double);
  current->add(sqrt);
  current->push(sqrt);
  return InliningStatus_Inlined;
}
||||||| merged common ancestors
    callInfo.setImplicitlyUsedUnchecked();
=======
  // If the arg is a Float32, we specialize the op as double, it will be
  // specialized as float32 if necessary later.
  MIRType absType = (argType == MIRType::Float32) ? MIRType::Double : argType;
  MInstruction* ins = MAbs::New(alloc(), callInfo.getArg(0), absType);
  current->add(ins);
>>>>>>> upstream-releases

<<<<<<< HEAD
IonBuilder::InliningResult IonBuilder::inlineMathAtan2(CallInfo& callInfo) {
  if (callInfo.argc() != 2 || callInfo.constructing()) {
    trackOptimizationOutcome(TrackedOutcome::CantInlineNativeBadForm);
    return InliningStatus_NotInlined;
  }
||||||| merged common ancestors
    // If the arg is a Float32, we specialize the op as double, it will be specialized
    // as float32 if necessary later.
    MIRType absType = (argType == MIRType::Float32) ? MIRType::Double : argType;
    MInstruction* ins = MAbs::New(alloc(), callInfo.getArg(0), absType);
    current->add(ins);
=======
  current->push(ins);
  return InliningStatus_Inlined;
}

IonBuilder::InliningResult IonBuilder::inlineMathFloor(CallInfo& callInfo) {
  if (callInfo.argc() != 1 || callInfo.constructing()) {
    trackOptimizationOutcome(TrackedOutcome::CantInlineNativeBadForm);
    return InliningStatus_NotInlined;
  }

  MIRType argType = callInfo.getArg(0)->type();
  MIRType returnType = getInlineReturnType();
>>>>>>> upstream-releases

<<<<<<< HEAD
  if (getInlineReturnType() != MIRType::Double) {
    return InliningStatus_NotInlined;
  }
||||||| merged common ancestors
    current->push(ins);
    return InliningStatus_Inlined;
}
=======
  // Math.floor(int(x)) == int(x)
  if (argType == MIRType::Int32 && returnType == MIRType::Int32) {
    callInfo.setImplicitlyUsedUnchecked();
    // The int operand may be something which bails out if the actual value
    // is not in the range of the result type of the MIR. We need to tell
    // the optimizer to preserve this bailout even if the final result is
    // fully truncated.
    MLimitedTruncate* ins = MLimitedTruncate::New(
        alloc(), callInfo.getArg(0), MDefinition::IndirectTruncate);
    current->add(ins);
    current->push(ins);
    return InliningStatus_Inlined;
  }
>>>>>>> upstream-releases

<<<<<<< HEAD
  MIRType argType0 = callInfo.getArg(0)->type();
  MIRType argType1 = callInfo.getArg(1)->type();
||||||| merged common ancestors
IonBuilder::InliningResult
IonBuilder::inlineMathFloor(CallInfo& callInfo)
{
    if (callInfo.argc() != 1 || callInfo.constructing()) {
        trackOptimizationOutcome(TrackedOutcome::CantInlineNativeBadForm);
        return InliningStatus_NotInlined;
    }
=======
  if (IsFloatingPointType(argType)) {
    if (returnType == MIRType::Int32) {
      callInfo.setImplicitlyUsedUnchecked();
      MFloor* ins = MFloor::New(alloc(), callInfo.getArg(0));
      current->add(ins);
      current->push(ins);
      return InliningStatus_Inlined;
    }
>>>>>>> upstream-releases

<<<<<<< HEAD
  if (!IsNumberType(argType0) || !IsNumberType(argType1)) {
    return InliningStatus_NotInlined;
  }
||||||| merged common ancestors
    MIRType argType = callInfo.getArg(0)->type();
    MIRType returnType = getInlineReturnType();
=======
    if (returnType == MIRType::Double) {
      callInfo.setImplicitlyUsedUnchecked();
>>>>>>> upstream-releases

<<<<<<< HEAD
  callInfo.setImplicitlyUsedUnchecked();
||||||| merged common ancestors
    // Math.floor(int(x)) == int(x)
    if (argType == MIRType::Int32 && returnType == MIRType::Int32) {
        callInfo.setImplicitlyUsedUnchecked();
        // The int operand may be something which bails out if the actual value
        // is not in the range of the result type of the MIR. We need to tell
        // the optimizer to preserve this bailout even if the final result is
        // fully truncated.
        MLimitedTruncate* ins = MLimitedTruncate::New(alloc(), callInfo.getArg(0),
                                                      MDefinition::IndirectTruncate);
        current->add(ins);
        current->push(ins);
        return InliningStatus_Inlined;
    }
=======
      MInstruction* ins = nullptr;
      if (MNearbyInt::HasAssemblerSupport(RoundingMode::Down)) {
        ins = MNearbyInt::New(alloc(), callInfo.getArg(0), argType,
                              RoundingMode::Down);
      } else {
        ins = MMathFunction::New(alloc(), callInfo.getArg(0),
                                 MMathFunction::Floor);
      }
>>>>>>> upstream-releases

<<<<<<< HEAD
  MAtan2* atan2 = MAtan2::New(alloc(), callInfo.getArg(0), callInfo.getArg(1));
  current->add(atan2);
  current->push(atan2);
  return InliningStatus_Inlined;
}
||||||| merged common ancestors
    if (IsFloatingPointType(argType)) {
        if (returnType == MIRType::Int32) {
            callInfo.setImplicitlyUsedUnchecked();
            MFloor* ins = MFloor::New(alloc(), callInfo.getArg(0));
            current->add(ins);
            current->push(ins);
            return InliningStatus_Inlined;
        }
=======
      current->add(ins);
      current->push(ins);
      return InliningStatus_Inlined;
    }
  }
>>>>>>> upstream-releases

<<<<<<< HEAD
IonBuilder::InliningResult IonBuilder::inlineMathHypot(CallInfo& callInfo) {
  if (callInfo.constructing()) {
    trackOptimizationOutcome(TrackedOutcome::CantInlineNativeBadForm);
    return InliningStatus_NotInlined;
  }
||||||| merged common ancestors
        if (returnType == MIRType::Double) {
            callInfo.setImplicitlyUsedUnchecked();
=======
  return InliningStatus_NotInlined;
}
>>>>>>> upstream-releases

<<<<<<< HEAD
  uint32_t argc = callInfo.argc();
  if (argc < 2 || argc > 4) {
    trackOptimizationOutcome(TrackedOutcome::CantInlineNativeBadForm);
    return InliningStatus_NotInlined;
  }
||||||| merged common ancestors
            MInstruction* ins = nullptr;
            if (MNearbyInt::HasAssemblerSupport(RoundingMode::Down)) {
                ins = MNearbyInt::New(alloc(), callInfo.getArg(0), argType, RoundingMode::Down);
            } else {
                ins = MMathFunction::New(alloc(), callInfo.getArg(0), MMathFunction::Floor);
            }
=======
IonBuilder::InliningResult IonBuilder::inlineMathCeil(CallInfo& callInfo) {
  if (callInfo.argc() != 1 || callInfo.constructing()) {
    trackOptimizationOutcome(TrackedOutcome::CantInlineNativeBadForm);
    return InliningStatus_NotInlined;
  }
>>>>>>> upstream-releases

<<<<<<< HEAD
  if (getInlineReturnType() != MIRType::Double) {
    return InliningStatus_NotInlined;
  }
||||||| merged common ancestors
            current->add(ins);
            current->push(ins);
            return InliningStatus_Inlined;
        }
    }
=======
  MIRType argType = callInfo.getArg(0)->type();
  MIRType returnType = getInlineReturnType();
>>>>>>> upstream-releases

<<<<<<< HEAD
  MDefinitionVector vector(alloc());
  if (!vector.reserve(argc)) {
    return InliningStatus_NotInlined;
  }
||||||| merged common ancestors
    return InliningStatus_NotInlined;
}
=======
  // Math.ceil(int(x)) == int(x)
  if (argType == MIRType::Int32 && returnType == MIRType::Int32) {
    callInfo.setImplicitlyUsedUnchecked();
    // The int operand may be something which bails out if the actual value
    // is not in the range of the result type of the MIR. We need to tell
    // the optimizer to preserve this bailout even if the final result is
    // fully truncated.
    MLimitedTruncate* ins = MLimitedTruncate::New(
        alloc(), callInfo.getArg(0), MDefinition::IndirectTruncate);
    current->add(ins);
    current->push(ins);
    return InliningStatus_Inlined;
  }
>>>>>>> upstream-releases

<<<<<<< HEAD
  for (uint32_t i = 0; i < argc; ++i) {
    MDefinition* arg = callInfo.getArg(i);
    if (!IsNumberType(arg->type())) {
      return InliningStatus_NotInlined;
||||||| merged common ancestors
IonBuilder::InliningResult
IonBuilder::inlineMathCeil(CallInfo& callInfo)
{
    if (callInfo.argc() != 1 || callInfo.constructing()) {
        trackOptimizationOutcome(TrackedOutcome::CantInlineNativeBadForm);
        return InliningStatus_NotInlined;
=======
  if (IsFloatingPointType(argType)) {
    if (returnType == MIRType::Int32) {
      callInfo.setImplicitlyUsedUnchecked();
      MCeil* ins = MCeil::New(alloc(), callInfo.getArg(0));
      current->add(ins);
      current->push(ins);
      return InliningStatus_Inlined;
>>>>>>> upstream-releases
    }
    vector.infallibleAppend(arg);
  }

<<<<<<< HEAD
  callInfo.setImplicitlyUsedUnchecked();
  MHypot* hypot = MHypot::New(alloc(), vector);
||||||| merged common ancestors
    MIRType argType = callInfo.getArg(0)->type();
    MIRType returnType = getInlineReturnType();
=======
    if (returnType == MIRType::Double) {
      callInfo.setImplicitlyUsedUnchecked();
>>>>>>> upstream-releases

<<<<<<< HEAD
  if (!hypot) {
    return InliningStatus_NotInlined;
  }
||||||| merged common ancestors
    // Math.ceil(int(x)) == int(x)
    if (argType == MIRType::Int32 && returnType == MIRType::Int32) {
        callInfo.setImplicitlyUsedUnchecked();
        // The int operand may be something which bails out if the actual value
        // is not in the range of the result type of the MIR. We need to tell
        // the optimizer to preserve this bailout even if the final result is
        // fully truncated.
        MLimitedTruncate* ins = MLimitedTruncate::New(alloc(), callInfo.getArg(0),
                                                      MDefinition::IndirectTruncate);
        current->add(ins);
        current->push(ins);
        return InliningStatus_Inlined;
    }
=======
      MInstruction* ins = nullptr;
      if (MNearbyInt::HasAssemblerSupport(RoundingMode::Up)) {
        ins = MNearbyInt::New(alloc(), callInfo.getArg(0), argType,
                              RoundingMode::Up);
      } else {
        ins = MMathFunction::New(alloc(), callInfo.getArg(0),
                                 MMathFunction::Ceil);
      }
>>>>>>> upstream-releases

<<<<<<< HEAD
  current->add(hypot);
  current->push(hypot);
  return InliningStatus_Inlined;
}
||||||| merged common ancestors
    if (IsFloatingPointType(argType)) {
        if (returnType == MIRType::Int32) {
            callInfo.setImplicitlyUsedUnchecked();
            MCeil* ins = MCeil::New(alloc(), callInfo.getArg(0));
            current->add(ins);
            current->push(ins);
            return InliningStatus_Inlined;
        }
=======
      current->add(ins);
      current->push(ins);
      return InliningStatus_Inlined;
    }
  }
>>>>>>> upstream-releases

<<<<<<< HEAD
IonBuilder::InliningResult IonBuilder::inlineMathPow(CallInfo& callInfo) {
  if (callInfo.argc() != 2 || callInfo.constructing()) {
    trackOptimizationOutcome(TrackedOutcome::CantInlineNativeBadForm);
    return InliningStatus_NotInlined;
  }
||||||| merged common ancestors
        if (returnType == MIRType::Double) {
            callInfo.setImplicitlyUsedUnchecked();
=======
  return InliningStatus_NotInlined;
}
>>>>>>> upstream-releases

<<<<<<< HEAD
  bool emitted = false;
  MOZ_TRY(powTrySpecialized(&emitted, callInfo.getArg(0), callInfo.getArg(1),
                            getInlineReturnType()));
||||||| merged common ancestors
            MInstruction* ins = nullptr;
            if (MNearbyInt::HasAssemblerSupport(RoundingMode::Up)) {
                ins = MNearbyInt::New(alloc(), callInfo.getArg(0), argType, RoundingMode::Up);
            } else {
                ins = MMathFunction::New(alloc(), callInfo.getArg(0), MMathFunction::Ceil);
            }
=======
IonBuilder::InliningResult IonBuilder::inlineMathClz32(CallInfo& callInfo) {
  if (callInfo.argc() != 1 || callInfo.constructing()) {
    trackOptimizationOutcome(TrackedOutcome::CantInlineNativeBadForm);
    return InliningStatus_NotInlined;
  }
>>>>>>> upstream-releases

<<<<<<< HEAD
  if (!emitted) {
    return InliningStatus_NotInlined;
  }

  callInfo.setImplicitlyUsedUnchecked();
  return InliningStatus_Inlined;
}
||||||| merged common ancestors
            current->add(ins);
            current->push(ins);
            return InliningStatus_Inlined;
        }
    }
=======
  MIRType returnType = getInlineReturnType();
  if (returnType != MIRType::Int32) {
    return InliningStatus_NotInlined;
  }
>>>>>>> upstream-releases

<<<<<<< HEAD
IonBuilder::InliningResult IonBuilder::inlineMathRandom(CallInfo& callInfo) {
  if (callInfo.constructing()) {
    trackOptimizationOutcome(TrackedOutcome::CantInlineNativeBadForm);
||||||| merged common ancestors
=======
  if (!IsNumberType(callInfo.getArg(0)->type())) {
>>>>>>> upstream-releases
    return InliningStatus_NotInlined;
<<<<<<< HEAD
  }

  if (getInlineReturnType() != MIRType::Double) {
    return InliningStatus_NotInlined;
  }

  // MRandom JIT code directly accesses the RNG. It's (barely) possible to
  // inline Math.random without it having been called yet, so ensure RNG
  // state that isn't guaranteed to be initialized already.
  script()->realm()->getOrCreateRandomNumberGenerator();

  callInfo.setImplicitlyUsedUnchecked();

  MRandom* rand = MRandom::New(alloc());
  current->add(rand);
  current->push(rand);
  return InliningStatus_Inlined;
||||||| merged common ancestors
=======
  }

  callInfo.setImplicitlyUsedUnchecked();

  MClz* ins = MClz::New(alloc(), callInfo.getArg(0), MIRType::Int32);
  current->add(ins);
  current->push(ins);
  return InliningStatus_Inlined;
>>>>>>> upstream-releases
}

<<<<<<< HEAD
IonBuilder::InliningResult IonBuilder::inlineMathImul(CallInfo& callInfo) {
  if (callInfo.argc() != 2 || callInfo.constructing()) {
    trackOptimizationOutcome(TrackedOutcome::CantInlineNativeBadForm);
    return InliningStatus_NotInlined;
  }
||||||| merged common ancestors
IonBuilder::InliningResult
IonBuilder::inlineMathClz32(CallInfo& callInfo)
{
    if (callInfo.argc() != 1 || callInfo.constructing()) {
        trackOptimizationOutcome(TrackedOutcome::CantInlineNativeBadForm);
        return InliningStatus_NotInlined;
    }
=======
IonBuilder::InliningResult IonBuilder::inlineMathRound(CallInfo& callInfo) {
  if (callInfo.argc() != 1 || callInfo.constructing()) {
    trackOptimizationOutcome(TrackedOutcome::CantInlineNativeBadForm);
    return InliningStatus_NotInlined;
  }
>>>>>>> upstream-releases

<<<<<<< HEAD
  MIRType returnType = getInlineReturnType();
  if (returnType != MIRType::Int32) {
    return InliningStatus_NotInlined;
  }
||||||| merged common ancestors
    MIRType returnType = getInlineReturnType();
    if (returnType != MIRType::Int32) {
        return InliningStatus_NotInlined;
    }
=======
  MIRType returnType = getInlineReturnType();
  MIRType argType = callInfo.getArg(0)->type();
>>>>>>> upstream-releases

<<<<<<< HEAD
  if (!IsNumberType(callInfo.getArg(0)->type())) {
    return InliningStatus_NotInlined;
  }
  if (!IsNumberType(callInfo.getArg(1)->type())) {
    return InliningStatus_NotInlined;
  }
||||||| merged common ancestors
    if (!IsNumberType(callInfo.getArg(0)->type())) {
        return InliningStatus_NotInlined;
    }
=======
  // Math.round(int(x)) == int(x)
  if (argType == MIRType::Int32 && returnType == MIRType::Int32) {
    callInfo.setImplicitlyUsedUnchecked();
    // The int operand may be something which bails out if the actual value
    // is not in the range of the result type of the MIR. We need to tell
    // the optimizer to preserve this bailout even if the final result is
    // fully truncated.
    MLimitedTruncate* ins = MLimitedTruncate::New(
        alloc(), callInfo.getArg(0), MDefinition::IndirectTruncate);
    current->add(ins);
    current->push(ins);
    return InliningStatus_Inlined;
  }
>>>>>>> upstream-releases

<<<<<<< HEAD
  callInfo.setImplicitlyUsedUnchecked();
||||||| merged common ancestors
    callInfo.setImplicitlyUsedUnchecked();
=======
  if (IsFloatingPointType(argType) && returnType == MIRType::Int32) {
    callInfo.setImplicitlyUsedUnchecked();
    MRound* ins = MRound::New(alloc(), callInfo.getArg(0));
    current->add(ins);
    current->push(ins);
    return InliningStatus_Inlined;
  }
>>>>>>> upstream-releases

<<<<<<< HEAD
  MInstruction* first = MTruncateToInt32::New(alloc(), callInfo.getArg(0));
  current->add(first);
||||||| merged common ancestors
    MClz* ins = MClz::New(alloc(), callInfo.getArg(0), MIRType::Int32);
    current->add(ins);
    current->push(ins);
    return InliningStatus_Inlined;
=======
  if (IsFloatingPointType(argType) && returnType == MIRType::Double) {
    callInfo.setImplicitlyUsedUnchecked();
    MMathFunction* ins =
        MMathFunction::New(alloc(), callInfo.getArg(0), MMathFunction::Round);
    current->add(ins);
    current->push(ins);
    return InliningStatus_Inlined;
  }
>>>>>>> upstream-releases

<<<<<<< HEAD
  MInstruction* second = MTruncateToInt32::New(alloc(), callInfo.getArg(1));
  current->add(second);
||||||| merged common ancestors
}
=======
  return InliningStatus_NotInlined;
}
>>>>>>> upstream-releases

<<<<<<< HEAD
  MMul* ins = MMul::New(alloc(), first, second, MIRType::Int32, MMul::Integer);
  current->add(ins);
  current->push(ins);
  return InliningStatus_Inlined;
}

IonBuilder::InliningResult IonBuilder::inlineMathFRound(CallInfo& callInfo) {
  if (callInfo.argc() != 1 || callInfo.constructing()) {
    trackOptimizationOutcome(TrackedOutcome::CantInlineNativeBadForm);
    return InliningStatus_NotInlined;
  }

  // MIRType can't be Float32, as this point, as getInlineReturnType uses JSVal
  // types to infer the returned MIR type.
  TemporaryTypeSet* returned = getInlineReturnTypeSet();
  if (returned->empty()) {
    // As there's only one possible returned type, just add it to the observed
    // returned typeset
    returned->addType(TypeSet::DoubleType(), alloc_->lifoAlloc());
  } else {
    MIRType returnType = getInlineReturnType();
    if (!IsNumberType(returnType)) {
      return InliningStatus_NotInlined;
    }
  }

  MIRType arg = callInfo.getArg(0)->type();
  if (!IsNumberType(arg)) {
    return InliningStatus_NotInlined;
  }

  callInfo.setImplicitlyUsedUnchecked();

  MToFloat32* ins = MToFloat32::New(alloc(), callInfo.getArg(0));
  current->add(ins);
  current->push(ins);
  return InliningStatus_Inlined;
}

IonBuilder::InliningResult IonBuilder::inlineMathTrunc(CallInfo& callInfo) {
  if (callInfo.argc() != 1 || callInfo.constructing()) {
    trackOptimizationOutcome(TrackedOutcome::CantInlineNativeBadForm);
    return InliningStatus_NotInlined;
  }
||||||| merged common ancestors
IonBuilder::InliningResult
IonBuilder::inlineMathRound(CallInfo& callInfo)
{
    if (callInfo.argc() != 1 || callInfo.constructing()) {
        trackOptimizationOutcome(TrackedOutcome::CantInlineNativeBadForm);
        return InliningStatus_NotInlined;
    }

    MIRType returnType = getInlineReturnType();
    MIRType argType = callInfo.getArg(0)->type();

    // Math.round(int(x)) == int(x)
    if (argType == MIRType::Int32 && returnType == MIRType::Int32) {
        callInfo.setImplicitlyUsedUnchecked();
        // The int operand may be something which bails out if the actual value
        // is not in the range of the result type of the MIR. We need to tell
        // the optimizer to preserve this bailout even if the final result is
        // fully truncated.
        MLimitedTruncate* ins = MLimitedTruncate::New(alloc(), callInfo.getArg(0),
                                                      MDefinition::IndirectTruncate);
        current->add(ins);
        current->push(ins);
        return InliningStatus_Inlined;
    }

    if (IsFloatingPointType(argType) && returnType == MIRType::Int32) {
        callInfo.setImplicitlyUsedUnchecked();
        MRound* ins = MRound::New(alloc(), callInfo.getArg(0));
        current->add(ins);
        current->push(ins);
        return InliningStatus_Inlined;
    }

    if (IsFloatingPointType(argType) && returnType == MIRType::Double) {
        callInfo.setImplicitlyUsedUnchecked();
        MMathFunction* ins = MMathFunction::New(alloc(), callInfo.getArg(0), MMathFunction::Round);
        current->add(ins);
        current->push(ins);
        return InliningStatus_Inlined;
    }

    return InliningStatus_NotInlined;
}

IonBuilder::InliningResult
IonBuilder::inlineMathSqrt(CallInfo& callInfo)
{
    if (callInfo.argc() != 1 || callInfo.constructing()) {
        trackOptimizationOutcome(TrackedOutcome::CantInlineNativeBadForm);
        return InliningStatus_NotInlined;
    }
=======
IonBuilder::InliningResult IonBuilder::inlineMathSqrt(CallInfo& callInfo) {
  if (callInfo.argc() != 1 || callInfo.constructing()) {
    trackOptimizationOutcome(TrackedOutcome::CantInlineNativeBadForm);
    return InliningStatus_NotInlined;
  }
>>>>>>> upstream-releases

<<<<<<< HEAD
  MIRType argType = callInfo.getArg(0)->type();
  MIRType returnType = getInlineReturnType();
||||||| merged common ancestors
    MIRType argType = callInfo.getArg(0)->type();
    if (getInlineReturnType() != MIRType::Double) {
        return InliningStatus_NotInlined;
    }
    if (!IsNumberType(argType)) {
        return InliningStatus_NotInlined;
    }
=======
  MIRType argType = callInfo.getArg(0)->type();
  if (getInlineReturnType() != MIRType::Double) {
    return InliningStatus_NotInlined;
  }
  if (!IsNumberType(argType)) {
    return InliningStatus_NotInlined;
  }
>>>>>>> upstream-releases

<<<<<<< HEAD
  // Math.trunc(int(x)) == int(x)
  if (argType == MIRType::Int32 && returnType == MIRType::Int32) {
    callInfo.setImplicitlyUsedUnchecked();
    // The int operand may be something which bails out if the actual value
    // is not in the range of the result type of the MIR. We need to tell
    // the optimizer to preserve this bailout even if the final result is
    // fully truncated.
    MLimitedTruncate* ins = MLimitedTruncate::New(
        alloc(), callInfo.getArg(0), MDefinition::IndirectTruncate);
    current->add(ins);
    current->push(ins);
    return InliningStatus_Inlined;
  }
||||||| merged common ancestors
    callInfo.setImplicitlyUsedUnchecked();

    MSqrt* sqrt = MSqrt::New(alloc(), callInfo.getArg(0), MIRType::Double);
    current->add(sqrt);
    current->push(sqrt);
    return InliningStatus_Inlined;
}
=======
  callInfo.setImplicitlyUsedUnchecked();

  MSqrt* sqrt = MSqrt::New(alloc(), callInfo.getArg(0), MIRType::Double);
  current->add(sqrt);
  current->push(sqrt);
  return InliningStatus_Inlined;
}
>>>>>>> upstream-releases

<<<<<<< HEAD
  if (IsFloatingPointType(argType)) {
    if (returnType == MIRType::Int32) {
      callInfo.setImplicitlyUsedUnchecked();
      MTrunc* ins = MTrunc::New(alloc(), callInfo.getArg(0));
      current->add(ins);
      current->push(ins);
      return InliningStatus_Inlined;
    }
||||||| merged common ancestors
IonBuilder::InliningResult
IonBuilder::inlineMathAtan2(CallInfo& callInfo)
{
    if (callInfo.argc() != 2 || callInfo.constructing()) {
        trackOptimizationOutcome(TrackedOutcome::CantInlineNativeBadForm);
        return InliningStatus_NotInlined;
    }
=======
IonBuilder::InliningResult IonBuilder::inlineMathAtan2(CallInfo& callInfo) {
  if (callInfo.argc() != 2 || callInfo.constructing()) {
    trackOptimizationOutcome(TrackedOutcome::CantInlineNativeBadForm);
    return InliningStatus_NotInlined;
  }
>>>>>>> upstream-releases

<<<<<<< HEAD
    if (returnType == MIRType::Double) {
      callInfo.setImplicitlyUsedUnchecked();
||||||| merged common ancestors
    if (getInlineReturnType() != MIRType::Double) {
        return InliningStatus_NotInlined;
    }
=======
  if (getInlineReturnType() != MIRType::Double) {
    return InliningStatus_NotInlined;
  }
>>>>>>> upstream-releases

<<<<<<< HEAD
      MInstruction* ins = nullptr;
      if (MNearbyInt::HasAssemblerSupport(RoundingMode::TowardsZero)) {
        ins = MNearbyInt::New(alloc(), callInfo.getArg(0), argType,
                              RoundingMode::TowardsZero);
      } else {
        ins = MMathFunction::New(alloc(), callInfo.getArg(0),
                                 MMathFunction::Trunc);
      }
||||||| merged common ancestors
    MIRType argType0 = callInfo.getArg(0)->type();
    MIRType argType1 = callInfo.getArg(1)->type();
=======
  MIRType argType0 = callInfo.getArg(0)->type();
  MIRType argType1 = callInfo.getArg(1)->type();
>>>>>>> upstream-releases

<<<<<<< HEAD
      current->add(ins);
      current->push(ins);
      return InliningStatus_Inlined;
    }
  }
||||||| merged common ancestors
    if (!IsNumberType(argType0) || !IsNumberType(argType1)) {
        return InliningStatus_NotInlined;
    }
=======
  if (!IsNumberType(argType0) || !IsNumberType(argType1)) {
    return InliningStatus_NotInlined;
  }
>>>>>>> upstream-releases

<<<<<<< HEAD
  return InliningStatus_NotInlined;
||||||| merged common ancestors
    callInfo.setImplicitlyUsedUnchecked();

    MAtan2* atan2 = MAtan2::New(alloc(), callInfo.getArg(0), callInfo.getArg(1));
    current->add(atan2);
    current->push(atan2);
    return InliningStatus_Inlined;
=======
  callInfo.setImplicitlyUsedUnchecked();

  MAtan2* atan2 = MAtan2::New(alloc(), callInfo.getArg(0), callInfo.getArg(1));
  current->add(atan2);
  current->push(atan2);
  return InliningStatus_Inlined;
>>>>>>> upstream-releases
}

<<<<<<< HEAD
IonBuilder::InliningResult IonBuilder::inlineMathSign(CallInfo& callInfo) {
  if (callInfo.argc() != 1 || callInfo.constructing()) {
    trackOptimizationOutcome(TrackedOutcome::CantInlineNativeBadForm);
    return InliningStatus_NotInlined;
  }
||||||| merged common ancestors
IonBuilder::InliningResult
IonBuilder::inlineMathHypot(CallInfo& callInfo)
{
    if (callInfo.constructing()) {
        trackOptimizationOutcome(TrackedOutcome::CantInlineNativeBadForm);
        return InliningStatus_NotInlined;
    }

    uint32_t argc = callInfo.argc();
    if (argc < 2 || argc > 4) {
        trackOptimizationOutcome(TrackedOutcome::CantInlineNativeBadForm);
        return InliningStatus_NotInlined;
    }
=======
IonBuilder::InliningResult IonBuilder::inlineMathHypot(CallInfo& callInfo) {
  if (callInfo.constructing()) {
    trackOptimizationOutcome(TrackedOutcome::CantInlineNativeBadForm);
    return InliningStatus_NotInlined;
  }

  uint32_t argc = callInfo.argc();
  if (argc < 2 || argc > 4) {
    trackOptimizationOutcome(TrackedOutcome::CantInlineNativeBadForm);
    return InliningStatus_NotInlined;
  }
>>>>>>> upstream-releases

<<<<<<< HEAD
  MIRType argType = callInfo.getArg(0)->type();
  MIRType returnType = getInlineReturnType();
||||||| merged common ancestors
    if (getInlineReturnType() != MIRType::Double) {
        return InliningStatus_NotInlined;
    }
=======
  if (getInlineReturnType() != MIRType::Double) {
    return InliningStatus_NotInlined;
  }
>>>>>>> upstream-releases

<<<<<<< HEAD
  if (returnType != MIRType::Int32 && returnType != MIRType::Double) {
    return InliningStatus_NotInlined;
  }
||||||| merged common ancestors
    MDefinitionVector vector(alloc());
    if (!vector.reserve(argc)) {
        return InliningStatus_NotInlined;
    }
=======
  MDefinitionVector vector(alloc());
  if (!vector.reserve(argc)) {
    return InliningStatus_NotInlined;
  }
>>>>>>> upstream-releases

<<<<<<< HEAD
  if (!IsFloatingPointType(argType) &&
      !(argType == MIRType::Int32 && returnType == MIRType::Int32)) {
    return InliningStatus_NotInlined;
  }
||||||| merged common ancestors
    for (uint32_t i = 0; i < argc; ++i) {
        MDefinition * arg = callInfo.getArg(i);
        if (!IsNumberType(arg->type())) {
            return InliningStatus_NotInlined;
        }
        vector.infallibleAppend(arg);
    }
=======
  for (uint32_t i = 0; i < argc; ++i) {
    MDefinition* arg = callInfo.getArg(i);
    if (!IsNumberType(arg->type())) {
      return InliningStatus_NotInlined;
    }
    vector.infallibleAppend(arg);
  }
>>>>>>> upstream-releases

<<<<<<< HEAD
  callInfo.setImplicitlyUsedUnchecked();
||||||| merged common ancestors
    callInfo.setImplicitlyUsedUnchecked();
    MHypot* hypot = MHypot::New(alloc(), vector);
=======
  callInfo.setImplicitlyUsedUnchecked();
  MHypot* hypot = MHypot::New(alloc(), vector);
>>>>>>> upstream-releases

<<<<<<< HEAD
  auto* ins = MSign::New(alloc(), callInfo.getArg(0), returnType);
  current->add(ins);
  current->push(ins);
||||||| merged common ancestors
    if (!hypot) {
        return InliningStatus_NotInlined;
    }
=======
  if (!hypot) {
    return InliningStatus_NotInlined;
  }
>>>>>>> upstream-releases

<<<<<<< HEAD
  return InliningStatus_Inlined;
||||||| merged common ancestors
    current->add(hypot);
    current->push(hypot);
    return InliningStatus_Inlined;
=======
  current->add(hypot);
  current->push(hypot);
  return InliningStatus_Inlined;
>>>>>>> upstream-releases
}

<<<<<<< HEAD
IonBuilder::InliningResult IonBuilder::inlineMathMinMax(CallInfo& callInfo,
                                                        bool max) {
  if (callInfo.argc() < 1 || callInfo.constructing()) {
    trackOptimizationOutcome(TrackedOutcome::CantInlineNativeBadForm);
    return InliningStatus_NotInlined;
  }
||||||| merged common ancestors
IonBuilder::InliningResult
IonBuilder::inlineMathPow(CallInfo& callInfo)
{
    if (callInfo.argc() != 2 || callInfo.constructing()) {
        trackOptimizationOutcome(TrackedOutcome::CantInlineNativeBadForm);
        return InliningStatus_NotInlined;
    }
=======
IonBuilder::InliningResult IonBuilder::inlineMathPow(CallInfo& callInfo) {
  if (callInfo.argc() != 2 || callInfo.constructing()) {
    trackOptimizationOutcome(TrackedOutcome::CantInlineNativeBadForm);
    return InliningStatus_NotInlined;
  }
>>>>>>> upstream-releases

<<<<<<< HEAD
  MIRType returnType = getInlineReturnType();
  if (!IsNumberType(returnType)) {
    return InliningStatus_NotInlined;
  }
||||||| merged common ancestors
    bool emitted = false;
    MOZ_TRY(powTrySpecialized(&emitted, callInfo.getArg(0), callInfo.getArg(1),
                              getInlineReturnType()));
=======
  bool emitted = false;
  MOZ_TRY(powTrySpecialized(&emitted, callInfo.getArg(0), callInfo.getArg(1),
                            getInlineReturnType()));
>>>>>>> upstream-releases

<<<<<<< HEAD
  MDefinitionVector int32_cases(alloc());
  for (unsigned i = 0; i < callInfo.argc(); i++) {
    MDefinition* arg = callInfo.getArg(i);
||||||| merged common ancestors
    if (!emitted) {
        return InliningStatus_NotInlined;
    }
=======
  if (!emitted) {
    return InliningStatus_NotInlined;
  }
>>>>>>> upstream-releases

<<<<<<< HEAD
    switch (arg->type()) {
      case MIRType::Int32:
        if (!int32_cases.append(arg)) {
          return abort(AbortReason::Alloc);
        }
        break;
      case MIRType::Double:
      case MIRType::Float32:
        // Don't force a double MMinMax for arguments that would be a NOP
        // when doing an integer MMinMax.
        if (arg->isConstant()) {
          double cte = arg->toConstant()->numberToDouble();
          // min(int32, cte >= INT32_MAX) = int32
          if (cte >= INT32_MAX && !max) {
            break;
          }
          // max(int32, cte <= INT32_MIN) = int32
          if (cte <= INT32_MIN && max) {
            break;
          }
        }
||||||| merged common ancestors
    callInfo.setImplicitlyUsedUnchecked();
    return InliningStatus_Inlined;
}
=======
  callInfo.setImplicitlyUsedUnchecked();
  return InliningStatus_Inlined;
}
>>>>>>> upstream-releases

<<<<<<< HEAD
        // Force double MMinMax if argument is a "effectfull" double.
        returnType = MIRType::Double;
        break;
      default:
        return InliningStatus_NotInlined;
    }
  }
||||||| merged common ancestors
IonBuilder::InliningResult
IonBuilder::inlineMathRandom(CallInfo& callInfo)
{
    if (callInfo.constructing()) {
        trackOptimizationOutcome(TrackedOutcome::CantInlineNativeBadForm);
        return InliningStatus_NotInlined;
    }
=======
IonBuilder::InliningResult IonBuilder::inlineMathRandom(CallInfo& callInfo) {
  if (callInfo.constructing()) {
    trackOptimizationOutcome(TrackedOutcome::CantInlineNativeBadForm);
    return InliningStatus_NotInlined;
  }
>>>>>>> upstream-releases

<<<<<<< HEAD
  if (int32_cases.length() == 0) {
    returnType = MIRType::Double;
  }
||||||| merged common ancestors
    if (getInlineReturnType() != MIRType::Double) {
        return InliningStatus_NotInlined;
    }
=======
  if (getInlineReturnType() != MIRType::Double) {
    return InliningStatus_NotInlined;
  }
>>>>>>> upstream-releases

<<<<<<< HEAD
  callInfo.setImplicitlyUsedUnchecked();
||||||| merged common ancestors
    // MRandom JIT code directly accesses the RNG. It's (barely) possible to
    // inline Math.random without it having been called yet, so ensure RNG
    // state that isn't guaranteed to be initialized already.
    script()->realm()->getOrCreateRandomNumberGenerator();
=======
  // MRandom JIT code directly accesses the RNG. It's (barely) possible to
  // inline Math.random without it having been called yet, so ensure RNG
  // state that isn't guaranteed to be initialized already.
  script()->realm()->getOrCreateRandomNumberGenerator();
>>>>>>> upstream-releases

<<<<<<< HEAD
  MDefinitionVector& cases =
      (returnType == MIRType::Int32) ? int32_cases : callInfo.argv();
||||||| merged common ancestors
    callInfo.setImplicitlyUsedUnchecked();
=======
  callInfo.setImplicitlyUsedUnchecked();
>>>>>>> upstream-releases

<<<<<<< HEAD
  if (cases.length() == 1) {
    MLimitedTruncate* limit =
        MLimitedTruncate::New(alloc(), cases[0], MDefinition::NoTruncate);
    current->add(limit);
    current->push(limit);
    return InliningStatus_Inlined;
  }
||||||| merged common ancestors
    MRandom* rand = MRandom::New(alloc());
    current->add(rand);
    current->push(rand);
    return InliningStatus_Inlined;
}
=======
  MRandom* rand = MRandom::New(alloc());
  current->add(rand);
  current->push(rand);
  return InliningStatus_Inlined;
}
>>>>>>> upstream-releases

<<<<<<< HEAD
  // Chain N-1 MMinMax instructions to compute the MinMax.
  MMinMax* last = MMinMax::New(alloc(), cases[0], cases[1], returnType, max);
  current->add(last);
||||||| merged common ancestors
IonBuilder::InliningResult
IonBuilder::inlineMathImul(CallInfo& callInfo)
{
    if (callInfo.argc() != 2 || callInfo.constructing()) {
        trackOptimizationOutcome(TrackedOutcome::CantInlineNativeBadForm);
        return InliningStatus_NotInlined;
    }
=======
IonBuilder::InliningResult IonBuilder::inlineMathImul(CallInfo& callInfo) {
  if (callInfo.argc() != 2 || callInfo.constructing()) {
    trackOptimizationOutcome(TrackedOutcome::CantInlineNativeBadForm);
    return InliningStatus_NotInlined;
  }
>>>>>>> upstream-releases

<<<<<<< HEAD
  for (unsigned i = 2; i < cases.length(); i++) {
    MMinMax* ins =
        MMinMax::New(alloc().fallible(), last, cases[i], returnType, max);
    if (!ins) {
      return abort(AbortReason::Alloc);
    }
    current->add(ins);
    last = ins;
  }
||||||| merged common ancestors
    MIRType returnType = getInlineReturnType();
    if (returnType != MIRType::Int32) {
        return InliningStatus_NotInlined;
    }
=======
  MIRType returnType = getInlineReturnType();
  if (returnType != MIRType::Int32) {
    return InliningStatus_NotInlined;
  }
>>>>>>> upstream-releases

<<<<<<< HEAD
  current->push(last);
  return InliningStatus_Inlined;
}
||||||| merged common ancestors
    if (!IsNumberType(callInfo.getArg(0)->type())) {
        return InliningStatus_NotInlined;
    }
    if (!IsNumberType(callInfo.getArg(1)->type())) {
        return InliningStatus_NotInlined;
    }
=======
  if (!IsNumberType(callInfo.getArg(0)->type())) {
    return InliningStatus_NotInlined;
  }
  if (!IsNumberType(callInfo.getArg(1)->type())) {
    return InliningStatus_NotInlined;
  }
>>>>>>> upstream-releases

<<<<<<< HEAD
IonBuilder::InliningResult IonBuilder::inlineStringObject(CallInfo& callInfo) {
  if (callInfo.argc() != 1 || !callInfo.constructing()) {
    trackOptimizationOutcome(TrackedOutcome::CantInlineNativeBadForm);
    return InliningStatus_NotInlined;
  }
||||||| merged common ancestors
    callInfo.setImplicitlyUsedUnchecked();
=======
  callInfo.setImplicitlyUsedUnchecked();
>>>>>>> upstream-releases

<<<<<<< HEAD
  // ConvertToString doesn't support objects.
  if (callInfo.getArg(0)->mightBeType(MIRType::Object)) {
    return InliningStatus_NotInlined;
  }
||||||| merged common ancestors
    MInstruction* first = MTruncateToInt32::New(alloc(), callInfo.getArg(0));
    current->add(first);
=======
  MInstruction* first = MTruncateToInt32::New(alloc(), callInfo.getArg(0));
  current->add(first);
>>>>>>> upstream-releases

<<<<<<< HEAD
  JSObject* templateObj =
      inspector->getTemplateObjectForNative(pc, StringConstructor);
  if (!templateObj) {
    return InliningStatus_NotInlined;
  }
  MOZ_ASSERT(templateObj->is<StringObject>());
||||||| merged common ancestors
    MInstruction* second = MTruncateToInt32::New(alloc(), callInfo.getArg(1));
    current->add(second);
=======
  MInstruction* second = MTruncateToInt32::New(alloc(), callInfo.getArg(1));
  current->add(second);
>>>>>>> upstream-releases

<<<<<<< HEAD
  callInfo.setImplicitlyUsedUnchecked();
||||||| merged common ancestors
    MMul* ins = MMul::New(alloc(), first, second, MIRType::Int32, MMul::Integer);
    current->add(ins);
    current->push(ins);
    return InliningStatus_Inlined;
}
=======
  MMul* ins = MMul::New(alloc(), first, second, MIRType::Int32, MMul::Integer);
  current->add(ins);
  current->push(ins);
  return InliningStatus_Inlined;
}
>>>>>>> upstream-releases

<<<<<<< HEAD
  MNewStringObject* ins =
      MNewStringObject::New(alloc(), callInfo.getArg(0), templateObj);
  current->add(ins);
  current->push(ins);
||||||| merged common ancestors
IonBuilder::InliningResult
IonBuilder::inlineMathFRound(CallInfo& callInfo)
{
    if (callInfo.argc() != 1 || callInfo.constructing()) {
        trackOptimizationOutcome(TrackedOutcome::CantInlineNativeBadForm);
        return InliningStatus_NotInlined;
    }
=======
IonBuilder::InliningResult IonBuilder::inlineMathFRound(CallInfo& callInfo) {
  if (callInfo.argc() != 1 || callInfo.constructing()) {
    trackOptimizationOutcome(TrackedOutcome::CantInlineNativeBadForm);
    return InliningStatus_NotInlined;
  }

  // MIRType can't be Float32, as this point, as getInlineReturnType uses JSVal
  // types to infer the returned MIR type.
  TemporaryTypeSet* returned = getInlineReturnTypeSet();
  if (returned->empty()) {
    // As there's only one possible returned type, just add it to the observed
    // returned typeset
    returned->addType(TypeSet::DoubleType(), alloc_->lifoAlloc());
  } else {
    MIRType returnType = getInlineReturnType();
    if (!IsNumberType(returnType)) {
      return InliningStatus_NotInlined;
    }
  }
>>>>>>> upstream-releases

<<<<<<< HEAD
  MOZ_TRY(resumeAfter(ins));
  return InliningStatus_Inlined;
}
||||||| merged common ancestors
    // MIRType can't be Float32, as this point, as getInlineReturnType uses JSVal types
    // to infer the returned MIR type.
    TemporaryTypeSet* returned = getInlineReturnTypeSet();
    if (returned->empty()) {
        // As there's only one possible returned type, just add it to the observed
        // returned typeset
        returned->addType(TypeSet::DoubleType(), alloc_->lifoAlloc());
    } else {
        MIRType returnType = getInlineReturnType();
        if (!IsNumberType(returnType)) {
            return InliningStatus_NotInlined;
        }
    }
=======
  MIRType arg = callInfo.getArg(0)->type();
  if (!IsNumberType(arg)) {
    return InliningStatus_NotInlined;
  }
>>>>>>> upstream-releases

<<<<<<< HEAD
IonBuilder::InliningResult IonBuilder::inlineConstantStringSplitString(
    CallInfo& callInfo) {
  if (!callInfo.getArg(0)->isConstant()) {
    return InliningStatus_NotInlined;
  }
||||||| merged common ancestors
    MIRType arg = callInfo.getArg(0)->type();
    if (!IsNumberType(arg)) {
        return InliningStatus_NotInlined;
    }
=======
  callInfo.setImplicitlyUsedUnchecked();
>>>>>>> upstream-releases

<<<<<<< HEAD
  if (!callInfo.getArg(1)->isConstant()) {
    return InliningStatus_NotInlined;
  }
||||||| merged common ancestors
    callInfo.setImplicitlyUsedUnchecked();
=======
  MToFloat32* ins = MToFloat32::New(alloc(), callInfo.getArg(0));
  current->add(ins);
  current->push(ins);
  return InliningStatus_Inlined;
}

IonBuilder::InliningResult IonBuilder::inlineMathTrunc(CallInfo& callInfo) {
  if (callInfo.argc() != 1 || callInfo.constructing()) {
    trackOptimizationOutcome(TrackedOutcome::CantInlineNativeBadForm);
    return InliningStatus_NotInlined;
  }

  MIRType argType = callInfo.getArg(0)->type();
  MIRType returnType = getInlineReturnType();
>>>>>>> upstream-releases

<<<<<<< HEAD
  MConstant* strval = callInfo.getArg(0)->toConstant();
  if (strval->type() != MIRType::String) {
    return InliningStatus_NotInlined;
  }
||||||| merged common ancestors
    MToFloat32* ins = MToFloat32::New(alloc(), callInfo.getArg(0));
    current->add(ins);
    current->push(ins);
    return InliningStatus_Inlined;
}
=======
  // Math.trunc(int(x)) == int(x)
  if (argType == MIRType::Int32 && returnType == MIRType::Int32) {
    callInfo.setImplicitlyUsedUnchecked();
    // The int operand may be something which bails out if the actual value
    // is not in the range of the result type of the MIR. We need to tell
    // the optimizer to preserve this bailout even if the final result is
    // fully truncated.
    MLimitedTruncate* ins = MLimitedTruncate::New(
        alloc(), callInfo.getArg(0), MDefinition::IndirectTruncate);
    current->add(ins);
    current->push(ins);
    return InliningStatus_Inlined;
  }
>>>>>>> upstream-releases

<<<<<<< HEAD
  MConstant* sepval = callInfo.getArg(1)->toConstant();
  if (strval->type() != MIRType::String) {
    return InliningStatus_NotInlined;
  }

  // Check if exist a template object in stub.
  JSString* stringStr = nullptr;
  JSString* stringSep = nullptr;
  ArrayObject* templateObject = nullptr;
  if (!inspector->isOptimizableConstStringSplit(pc, &stringStr, &stringSep,
                                                &templateObject)) {
    return InliningStatus_NotInlined;
  }

  MOZ_ASSERT(stringStr);
  MOZ_ASSERT(stringSep);
  MOZ_ASSERT(templateObject);

  if (strval->toString() != stringStr) {
    return InliningStatus_NotInlined;
  }
||||||| merged common ancestors
IonBuilder::InliningResult
IonBuilder::inlineMathTrunc(CallInfo& callInfo)
{
    if (callInfo.argc() != 1 || callInfo.constructing()) {
        trackOptimizationOutcome(TrackedOutcome::CantInlineNativeBadForm);
        return InliningStatus_NotInlined;
    }

    MIRType argType = callInfo.getArg(0)->type();
    MIRType returnType = getInlineReturnType();

    // Math.trunc(int(x)) == int(x)
    if (argType == MIRType::Int32 && returnType == MIRType::Int32) {
        callInfo.setImplicitlyUsedUnchecked();
        // The int operand may be something which bails out if the actual value
        // is not in the range of the result type of the MIR. We need to tell
        // the optimizer to preserve this bailout even if the final result is
        // fully truncated.
        MLimitedTruncate* ins = MLimitedTruncate::New(alloc(), callInfo.getArg(0),
                                                      MDefinition::IndirectTruncate);
        current->add(ins);
        current->push(ins);
        return InliningStatus_Inlined;
    }
=======
  if (IsFloatingPointType(argType)) {
    if (returnType == MIRType::Int32) {
      callInfo.setImplicitlyUsedUnchecked();
      MTrunc* ins = MTrunc::New(alloc(), callInfo.getArg(0));
      current->add(ins);
      current->push(ins);
      return InliningStatus_Inlined;
    }
>>>>>>> upstream-releases

<<<<<<< HEAD
  if (sepval->toString() != stringSep) {
    return InliningStatus_NotInlined;
  }

  // Check if |templateObject| is valid.
  TypeSet::ObjectKey* retType = TypeSet::ObjectKey::get(templateObject);
  if (retType->unknownProperties()) {
    return InliningStatus_NotInlined;
  }
||||||| merged common ancestors
    if (IsFloatingPointType(argType)) {
        if (returnType == MIRType::Int32) {
            callInfo.setImplicitlyUsedUnchecked();
            MTrunc* ins = MTrunc::New(alloc(), callInfo.getArg(0));
            current->add(ins);
            current->push(ins);
            return InliningStatus_Inlined;
        }

        if (returnType == MIRType::Double) {
            callInfo.setImplicitlyUsedUnchecked();
=======
    if (returnType == MIRType::Double) {
      callInfo.setImplicitlyUsedUnchecked();
>>>>>>> upstream-releases

<<<<<<< HEAD
  HeapTypeSetKey key = retType->property(JSID_VOID);
  if (!key.maybeTypes()) {
    return InliningStatus_NotInlined;
  }
||||||| merged common ancestors
            MInstruction* ins = nullptr;
            if (MNearbyInt::HasAssemblerSupport(RoundingMode::TowardsZero)) {
                ins = MNearbyInt::New(alloc(), callInfo.getArg(0), argType,
                                      RoundingMode::TowardsZero);
            } else {
                ins = MMathFunction::New(alloc(), callInfo.getArg(0), MMathFunction::Trunc);
            }
=======
      MInstruction* ins = nullptr;
      if (MNearbyInt::HasAssemblerSupport(RoundingMode::TowardsZero)) {
        ins = MNearbyInt::New(alloc(), callInfo.getArg(0), argType,
                              RoundingMode::TowardsZero);
      } else {
        ins = MMathFunction::New(alloc(), callInfo.getArg(0),
                                 MMathFunction::Trunc);
      }
>>>>>>> upstream-releases

<<<<<<< HEAD
  if (!key.maybeTypes()->hasType(TypeSet::StringType())) {
    return InliningStatus_NotInlined;
  }
||||||| merged common ancestors
            current->add(ins);
            current->push(ins);
            return InliningStatus_Inlined;
        }
    }
=======
      current->add(ins);
      current->push(ins);
      return InliningStatus_Inlined;
    }
  }
>>>>>>> upstream-releases

<<<<<<< HEAD
  uint32_t initLength = templateObject->length();
  if (templateObject->getDenseInitializedLength() != initLength) {
    return InliningStatus_NotInlined;
  }
||||||| merged common ancestors
    return InliningStatus_NotInlined;
}
=======
  return InliningStatus_NotInlined;
}
>>>>>>> upstream-releases

<<<<<<< HEAD
  Vector<MConstant*, 0, SystemAllocPolicy> arrayValues;
  for (uint32_t i = 0; i < initLength; i++) {
    Value str = templateObject->getDenseElement(i);
    MOZ_ASSERT(str.toString()->isAtom());
    MConstant* value = MConstant::New(alloc().fallible(), str, constraints());
    if (!value) {
      return abort(AbortReason::Alloc);
    }
    if (!TypeSetIncludes(key.maybeTypes(), value->type(),
                         value->resultTypeSet())) {
      return InliningStatus_NotInlined;
    }
||||||| merged common ancestors
IonBuilder::InliningResult
IonBuilder::inlineMathSign(CallInfo& callInfo)
{
    if (callInfo.argc() != 1 || callInfo.constructing()) {
        trackOptimizationOutcome(TrackedOutcome::CantInlineNativeBadForm);
        return InliningStatus_NotInlined;
    }
=======
IonBuilder::InliningResult IonBuilder::inlineMathSign(CallInfo& callInfo) {
  if (callInfo.argc() != 1 || callInfo.constructing()) {
    trackOptimizationOutcome(TrackedOutcome::CantInlineNativeBadForm);
    return InliningStatus_NotInlined;
  }
>>>>>>> upstream-releases

<<<<<<< HEAD
    if (!arrayValues.append(value)) {
      return abort(AbortReason::Alloc);
    }
  }
  callInfo.setImplicitlyUsedUnchecked();
||||||| merged common ancestors
    MIRType argType = callInfo.getArg(0)->type();
    MIRType returnType = getInlineReturnType();

    if (returnType != MIRType::Int32 && returnType != MIRType::Double) {
        return InliningStatus_NotInlined;
    }
=======
  MIRType argType = callInfo.getArg(0)->type();
  MIRType returnType = getInlineReturnType();

  if (returnType != MIRType::Int32 && returnType != MIRType::Double) {
    return InliningStatus_NotInlined;
  }
>>>>>>> upstream-releases

<<<<<<< HEAD
  TemporaryTypeSet::DoubleConversion conversion =
      getInlineReturnTypeSet()->convertDoubleElements(constraints());
  if (conversion == TemporaryTypeSet::AlwaysConvertToDoubles) {
    return InliningStatus_NotInlined;
  }

  MOZ_TRY(jsop_newarray(templateObject, initLength));

  MDefinition* array = current->peek(-1);

  if (!initLength) {
    MOZ_TRY(resumeAfter(array->toNewArray()));
    return InliningStatus_Inlined;
  }
||||||| merged common ancestors
    if (!IsFloatingPointType(argType) &&
        !(argType == MIRType::Int32 && returnType == MIRType::Int32))
    {
        return InliningStatus_NotInlined;
    }

    callInfo.setImplicitlyUsedUnchecked();

    auto* ins = MSign::New(alloc(), callInfo.getArg(0), returnType);
    current->add(ins);
    current->push(ins);

    return InliningStatus_Inlined;
}

IonBuilder::InliningResult
IonBuilder::inlineMathMinMax(CallInfo& callInfo, bool max)
{
    if (callInfo.argc() < 1 || callInfo.constructing()) {
        trackOptimizationOutcome(TrackedOutcome::CantInlineNativeBadForm);
        return InliningStatus_NotInlined;
    }

    MIRType returnType = getInlineReturnType();
    if (!IsNumberType(returnType)) {
        return InliningStatus_NotInlined;
    }

    MDefinitionVector int32_cases(alloc());
    for (unsigned i = 0; i < callInfo.argc(); i++) {
        MDefinition* arg = callInfo.getArg(i);

        switch (arg->type()) {
          case MIRType::Int32:
            if (!int32_cases.append(arg)) {
                return abort(AbortReason::Alloc);
            }
            break;
          case MIRType::Double:
          case MIRType::Float32:
            // Don't force a double MMinMax for arguments that would be a NOP
            // when doing an integer MMinMax.
            if (arg->isConstant()) {
                double cte = arg->toConstant()->numberToDouble();
                // min(int32, cte >= INT32_MAX) = int32
                if (cte >= INT32_MAX && !max) {
                    break;
                }
                // max(int32, cte <= INT32_MIN) = int32
                if (cte <= INT32_MIN && max) {
                    break;
                }
            }

            // Force double MMinMax if argument is a "effectfull" double.
            returnType = MIRType::Double;
            break;
          default:
            return InliningStatus_NotInlined;
        }
    }

    if (int32_cases.length() == 0) {
        returnType = MIRType::Double;
    }

    callInfo.setImplicitlyUsedUnchecked();

    MDefinitionVector& cases = (returnType == MIRType::Int32) ? int32_cases : callInfo.argv();

    if (cases.length() == 1) {
        MLimitedTruncate* limit = MLimitedTruncate::New(alloc(), cases[0], MDefinition::NoTruncate);
        current->add(limit);
        current->push(limit);
        return InliningStatus_Inlined;
    }

    // Chain N-1 MMinMax instructions to compute the MinMax.
    MMinMax* last = MMinMax::New(alloc(), cases[0], cases[1], returnType, max);
    current->add(last);

    for (unsigned i = 2; i < cases.length(); i++) {
        MMinMax* ins = MMinMax::New(alloc().fallible(), last, cases[i], returnType, max);
        if (!ins) {
            return abort(AbortReason::Alloc);
        }
        current->add(ins);
        last = ins;
    }

    current->push(last);
    return InliningStatus_Inlined;
}

IonBuilder::InliningResult
IonBuilder::inlineStringObject(CallInfo& callInfo)
{
    if (callInfo.argc() != 1 || !callInfo.constructing()) {
        trackOptimizationOutcome(TrackedOutcome::CantInlineNativeBadForm);
        return InliningStatus_NotInlined;
    }

    // ConvertToString doesn't support objects.
    if (callInfo.getArg(0)->mightBeType(MIRType::Object)) {
        return InliningStatus_NotInlined;
    }

    JSObject* templateObj = inspector->getTemplateObjectForNative(pc, StringConstructor);
    if (!templateObj) {
        return InliningStatus_NotInlined;
    }
    MOZ_ASSERT(templateObj->is<StringObject>());

    callInfo.setImplicitlyUsedUnchecked();

    MNewStringObject* ins = MNewStringObject::New(alloc(), callInfo.getArg(0), templateObj);
    current->add(ins);
    current->push(ins);

    MOZ_TRY(resumeAfter(ins));
    return InliningStatus_Inlined;
}

IonBuilder::InliningResult
IonBuilder::inlineConstantStringSplitString(CallInfo& callInfo)
{
    if (!callInfo.getArg(0)->isConstant()) {
        return InliningStatus_NotInlined;
    }

    if (!callInfo.getArg(1)->isConstant()) {
        return InliningStatus_NotInlined;
    }

    MConstant* strval = callInfo.getArg(0)->toConstant();
    if (strval->type() != MIRType::String) {
        return InliningStatus_NotInlined;
    }

    MConstant* sepval = callInfo.getArg(1)->toConstant();
    if (strval->type() != MIRType::String) {
        return InliningStatus_NotInlined;
    }

    // Check if exist a template object in stub.
    JSString* stringStr = nullptr;
    JSString* stringSep = nullptr;
    ArrayObject* templateObject = nullptr;
    if (!inspector->isOptimizableConstStringSplit(pc, &stringStr, &stringSep, &templateObject)) {
        return InliningStatus_NotInlined;
    }

    MOZ_ASSERT(stringStr);
    MOZ_ASSERT(stringSep);
    MOZ_ASSERT(templateObject);

    if (strval->toString() != stringStr) {
        return InliningStatus_NotInlined;
    }

    if (sepval->toString() != stringSep) {
        return InliningStatus_NotInlined;
    }

    // Check if |templateObject| is valid.
    TypeSet::ObjectKey* retType = TypeSet::ObjectKey::get(templateObject);
    if (retType->unknownProperties()) {
        return InliningStatus_NotInlined;
    }

    HeapTypeSetKey key = retType->property(JSID_VOID);
    if (!key.maybeTypes()) {
        return InliningStatus_NotInlined;
    }

    if (!key.maybeTypes()->hasType(TypeSet::StringType())) {
        return InliningStatus_NotInlined;
    }

    uint32_t initLength = templateObject->length();
    if (templateObject->getDenseInitializedLength() != initLength) {
        return InliningStatus_NotInlined;
    }

    Vector<MConstant*, 0, SystemAllocPolicy> arrayValues;
    for (uint32_t i = 0; i < initLength; i++) {
        Value str = templateObject->getDenseElement(i);
        MOZ_ASSERT(str.toString()->isAtom());
        MConstant* value = MConstant::New(alloc().fallible(), str, constraints());
        if (!value) {
            return abort(AbortReason::Alloc);
        }
        if (!TypeSetIncludes(key.maybeTypes(), value->type(), value->resultTypeSet())) {
            return InliningStatus_NotInlined;
        }

        if (!arrayValues.append(value)) {
            return abort(AbortReason::Alloc);
        }
    }
    callInfo.setImplicitlyUsedUnchecked();

    TemporaryTypeSet::DoubleConversion conversion =
            getInlineReturnTypeSet()->convertDoubleElements(constraints());
    if (conversion == TemporaryTypeSet::AlwaysConvertToDoubles) {
        return InliningStatus_NotInlined;
    }

    MOZ_TRY(jsop_newarray(templateObject, initLength));

    MDefinition* array = current->peek(-1);

    if (!initLength) {
        MOZ_TRY(resumeAfter(array->toNewArray()));
        return InliningStatus_Inlined;
    }

    // Store all values, no need to initialize the length after each as
    // jsop_initelem_array is doing because we do not expect to bailout
    // because the memory is supposed to be allocated by now.
    for (uint32_t i = 0; i < initLength; i++) {
        if (!alloc().ensureBallast()) {
            return abort(AbortReason::Alloc);
        }

        MConstant* value = arrayValues[i];
        current->add(value);

        MOZ_TRY(initializeArrayElement(array, i, value, /* addResumePoint = */ false));
    }

    MInstruction* setLength = setInitializedLength(array, initLength);
    MOZ_TRY(resumeAfter(setLength));
    return InliningStatus_Inlined;
}

IonBuilder::InliningResult
IonBuilder::inlineStringSplitString(CallInfo& callInfo)
{
    MOZ_ASSERT(!callInfo.constructing());
    MOZ_ASSERT(callInfo.argc() == 2);

    MDefinition* strArg = callInfo.getArg(0);
    MDefinition* sepArg = callInfo.getArg(1);

    if (strArg->type() != MIRType::String) {
        return InliningStatus_NotInlined;
    }

    if (sepArg->type() != MIRType::String) {
        return InliningStatus_NotInlined;
    }

    IonBuilder::InliningStatus resultConstStringSplit;
    MOZ_TRY_VAR(resultConstStringSplit, inlineConstantStringSplitString(callInfo));
    if (resultConstStringSplit != InliningStatus_NotInlined) {
        return resultConstStringSplit;
    }

    JSContext* cx = TlsContext.get();
    ObjectGroup* group = ObjectGroupRealm::getStringSplitStringGroup(cx);
    if (!group) {
        return InliningStatus_NotInlined;
    }
    AutoSweepObjectGroup sweep(group);
    if (group->maybePreliminaryObjects(sweep)) {
        return InliningStatus_NotInlined;
    }

    TypeSet::ObjectKey* retKey = TypeSet::ObjectKey::get(group);
    if (retKey->unknownProperties()) {
        return InliningStatus_NotInlined;
    }

    HeapTypeSetKey key = retKey->property(JSID_VOID);
    if (!key.maybeTypes()) {
        return InliningStatus_NotInlined;
    }

    if (!key.maybeTypes()->hasType(TypeSet::StringType())) {
        key.freeze(constraints());
        return InliningStatus_NotInlined;
    }

    callInfo.setImplicitlyUsedUnchecked();
    MStringSplit* ins = MStringSplit::New(alloc(), constraints(), strArg, sepArg, group);
    current->add(ins);
    current->push(ins);

    return InliningStatus_Inlined;
}

IonBuilder::InliningResult
IonBuilder::inlineObjectHasPrototype(CallInfo& callInfo)
{
    MOZ_ASSERT(!callInfo.constructing());
    MOZ_ASSERT(callInfo.argc() == 2);

    MDefinition* objArg = callInfo.getArg(0);
    MDefinition* protoArg = callInfo.getArg(1);

    if (objArg->type() != MIRType::Object) {
        return InliningStatus_NotInlined;
    }
    if (protoArg->type() != MIRType::Object) {
        return InliningStatus_NotInlined;
    }

    // Inline only when both obj and proto are singleton objects and
    // obj does not have uncacheable proto and obj.__proto__ is proto.
    TemporaryTypeSet* objTypes = objArg->resultTypeSet();
    if (!objTypes || objTypes->unknownObject() || objTypes->getObjectCount() != 1) {
        return InliningStatus_NotInlined;
    }

    TypeSet::ObjectKey* objKey = objTypes->getObject(0);
    if (!objKey || !objKey->hasStableClassAndProto(constraints())) {
        return InliningStatus_NotInlined;
    }
    if (!objKey->isSingleton() || !objKey->singleton()->is<NativeObject>()) {
        return InliningStatus_NotInlined;
    }

    JSObject* obj = &objKey->singleton()->as<NativeObject>();
    if (obj->hasUncacheableProto()) {
        return InliningStatus_NotInlined;
    }

    JSObject* actualProto = checkNurseryObject(objKey->proto().toObjectOrNull());
    if (actualProto == nullptr) {
        return InliningStatus_NotInlined;
    }

    TemporaryTypeSet* protoTypes = protoArg->resultTypeSet();
    if (!protoTypes || protoTypes->unknownObject() || protoTypes->getObjectCount() != 1) {
        return InliningStatus_NotInlined;
    }

    TypeSet::ObjectKey* protoKey = protoTypes->getObject(0);
    if (!protoKey || !protoKey->hasStableClassAndProto(constraints())) {
        return InliningStatus_NotInlined;
    }
    if (!protoKey->isSingleton() || !protoKey->singleton()->is<NativeObject>()) {
        return InliningStatus_NotInlined;
    }

    JSObject* proto = &protoKey->singleton()->as<NativeObject>();
    pushConstant(BooleanValue(proto == actualProto));
    callInfo.setImplicitlyUsedUnchecked();
    return InliningStatus_Inlined;
}

IonBuilder::InliningResult
IonBuilder::inlineFinishBoundFunctionInit(CallInfo& callInfo)
{
    MOZ_ASSERT(!callInfo.constructing());
    MOZ_ASSERT(callInfo.argc() == 3);
    MOZ_ASSERT(BytecodeIsPopped(pc));

    MDefinition* boundFunction = callInfo.getArg(0);
    MDefinition* targetFunction = callInfo.getArg(1);
    MDefinition* argCount = callInfo.getArg(2);

    if (boundFunction->type() != MIRType::Object) {
        return InliningStatus_NotInlined;
    }
    if (targetFunction->type() != MIRType::Object) {
        return InliningStatus_NotInlined;
    }
    if (argCount->type() != MIRType::Int32) {
        return InliningStatus_NotInlined;
    }

    callInfo.setImplicitlyUsedUnchecked();

    auto* ins = MFinishBoundFunctionInit::New(alloc(), boundFunction, targetFunction, argCount);
    current->add(ins);

    pushConstant(UndefinedValue());

    MOZ_TRY(resumeAfter(ins));
    return InliningStatus_Inlined;
}

IonBuilder::InliningResult
IonBuilder::inlineIsPackedArray(CallInfo& callInfo)
{
    MOZ_ASSERT(!callInfo.constructing());
    MOZ_ASSERT(callInfo.argc() == 1);

    if (getInlineReturnType() != MIRType::Boolean) {
        return InliningStatus_NotInlined;
    }

    MDefinition* array = callInfo.getArg(0);

    if (array->type() != MIRType::Object) {
        return InliningStatus_NotInlined;
    }

    TemporaryTypeSet* arrayTypes = array->resultTypeSet();
    if (!arrayTypes) {
        return InliningStatus_NotInlined;
    }

    const Class* clasp = arrayTypes->getKnownClass(constraints());
    if (clasp != &ArrayObject::class_) {
        return InliningStatus_NotInlined;
    }

    // Only inline if the array uses dense storage.
    ObjectGroupFlags unhandledFlags = OBJECT_FLAG_SPARSE_INDEXES |
                                      OBJECT_FLAG_LENGTH_OVERFLOW |
                                      OBJECT_FLAG_NON_PACKED;

    if (arrayTypes->hasObjectFlags(constraints(), unhandledFlags)) {
        trackOptimizationOutcome(TrackedOutcome::ArrayBadFlags);
        return InliningStatus_NotInlined;
    }

    callInfo.setImplicitlyUsedUnchecked();

    auto* ins = MIsPackedArray::New(alloc(), array);
    current->add(ins);
    current->push(ins);

    return InliningStatus_Inlined;
}

IonBuilder::InliningResult
IonBuilder::inlineReflectGetPrototypeOf(CallInfo& callInfo)
{
    if (callInfo.argc() != 1 || callInfo.constructing()) {
        trackOptimizationOutcome(TrackedOutcome::CantInlineNativeBadForm);
        return InliningStatus_NotInlined;
    }
=======
  if (!IsFloatingPointType(argType) &&
      !(argType == MIRType::Int32 && returnType == MIRType::Int32)) {
    return InliningStatus_NotInlined;
  }

  callInfo.setImplicitlyUsedUnchecked();

  auto* ins = MSign::New(alloc(), callInfo.getArg(0), returnType);
  current->add(ins);
  current->push(ins);

  return InliningStatus_Inlined;
}

IonBuilder::InliningResult IonBuilder::inlineMathMinMax(CallInfo& callInfo,
                                                        bool max) {
  if (callInfo.argc() < 1 || callInfo.constructing()) {
    trackOptimizationOutcome(TrackedOutcome::CantInlineNativeBadForm);
    return InliningStatus_NotInlined;
  }

  MIRType returnType = getInlineReturnType();
  if (!IsNumberType(returnType)) {
    return InliningStatus_NotInlined;
  }

  MDefinitionVector int32_cases(alloc());
  for (unsigned i = 0; i < callInfo.argc(); i++) {
    MDefinition* arg = callInfo.getArg(i);

    switch (arg->type()) {
      case MIRType::Int32:
        if (!int32_cases.append(arg)) {
          return abort(AbortReason::Alloc);
        }
        break;
      case MIRType::Double:
      case MIRType::Float32:
        // Don't force a double MMinMax for arguments that would be a NOP
        // when doing an integer MMinMax.
        if (arg->isConstant()) {
          double cte = arg->toConstant()->numberToDouble();
          // min(int32, cte >= INT32_MAX) = int32
          if (cte >= INT32_MAX && !max) {
            break;
          }
          // max(int32, cte <= INT32_MIN) = int32
          if (cte <= INT32_MIN && max) {
            break;
          }
        }

        // Force double MMinMax if argument is a "effectfull" double.
        returnType = MIRType::Double;
        break;
      default:
        return InliningStatus_NotInlined;
    }
  }

  if (int32_cases.length() == 0) {
    returnType = MIRType::Double;
  }

  callInfo.setImplicitlyUsedUnchecked();

  MDefinitionVector& cases =
      (returnType == MIRType::Int32) ? int32_cases : callInfo.argv();

  if (cases.length() == 1) {
    MLimitedTruncate* limit =
        MLimitedTruncate::New(alloc(), cases[0], MDefinition::NoTruncate);
    current->add(limit);
    current->push(limit);
    return InliningStatus_Inlined;
  }

  // Chain N-1 MMinMax instructions to compute the MinMax.
  MMinMax* last = MMinMax::New(alloc(), cases[0], cases[1], returnType, max);
  current->add(last);

  for (unsigned i = 2; i < cases.length(); i++) {
    MMinMax* ins =
        MMinMax::New(alloc().fallible(), last, cases[i], returnType, max);
    if (!ins) {
      return abort(AbortReason::Alloc);
    }
    current->add(ins);
    last = ins;
  }

  current->push(last);
  return InliningStatus_Inlined;
}

IonBuilder::InliningResult IonBuilder::inlineStringObject(CallInfo& callInfo) {
  if (callInfo.argc() != 1 || !callInfo.constructing()) {
    trackOptimizationOutcome(TrackedOutcome::CantInlineNativeBadForm);
    return InliningStatus_NotInlined;
  }

  // ConvertToString doesn't support objects.
  if (callInfo.getArg(0)->mightBeType(MIRType::Object)) {
    return InliningStatus_NotInlined;
  }
>>>>>>> upstream-releases

<<<<<<< HEAD
  // Store all values, no need to initialize the length after each as
  // jsop_initelem_array is doing because we do not expect to bailout
  // because the memory is supposed to be allocated by now.
  for (uint32_t i = 0; i < initLength; i++) {
    if (!alloc().ensureBallast()) {
      return abort(AbortReason::Alloc);
    }
||||||| merged common ancestors
    MDefinition* target = callInfo.getArg(0);
    if (target->type() != MIRType::Object) {
        return InliningStatus_NotInlined;
    }
=======
  JSObject* templateObj =
      inspector->getTemplateObjectForNative(pc, StringConstructor);
  if (!templateObj) {
    return InliningStatus_NotInlined;
  }
  MOZ_ASSERT(templateObj->is<StringObject>());
>>>>>>> upstream-releases

<<<<<<< HEAD
    MConstant* value = arrayValues[i];
    current->add(value);
||||||| merged common ancestors
    callInfo.setImplicitlyUsedUnchecked();
=======
  callInfo.setImplicitlyUsedUnchecked();
>>>>>>> upstream-releases

<<<<<<< HEAD
    MOZ_TRY(
        initializeArrayElement(array, i, value, /* addResumePoint = */ false));
  }
||||||| merged common ancestors
    auto* ins = MGetPrototypeOf::New(alloc(), target);
    current->add(ins);
    current->push(ins);
=======
  MNewStringObject* ins =
      MNewStringObject::New(alloc(), callInfo.getArg(0), templateObj);
  current->add(ins);
  current->push(ins);
>>>>>>> upstream-releases

<<<<<<< HEAD
  MInstruction* setLength = setInitializedLength(array, initLength);
  MOZ_TRY(resumeAfter(setLength));
  return InliningStatus_Inlined;
||||||| merged common ancestors
    MOZ_TRY(resumeAfter(ins));
    MOZ_TRY(pushTypeBarrier(ins, getInlineReturnTypeSet(), BarrierKind::TypeSet));
    return InliningStatus_Inlined;
=======
  MOZ_TRY(resumeAfter(ins));
  return InliningStatus_Inlined;
>>>>>>> upstream-releases
}

<<<<<<< HEAD
IonBuilder::InliningResult IonBuilder::inlineStringSplitString(
    CallInfo& callInfo) {
  MOZ_ASSERT(!callInfo.constructing());
  MOZ_ASSERT(callInfo.argc() == 2);
||||||| merged common ancestors
IonBuilder::InliningResult
IonBuilder::inlineStrCharCodeAt(CallInfo& callInfo)
{
    if (callInfo.argc() != 1 || callInfo.constructing()) {
        trackOptimizationOutcome(TrackedOutcome::CantInlineNativeBadForm);
        return InliningStatus_NotInlined;
    }

    if (getInlineReturnType() != MIRType::Int32) {
        return InliningStatus_NotInlined;
    }
    if (callInfo.thisArg()->type() != MIRType::String && callInfo.thisArg()->type() != MIRType::Value) {
        return InliningStatus_NotInlined;
    }
    MIRType argType = callInfo.getArg(0)->type();
    if (argType != MIRType::Int32 && argType != MIRType::Double) {
        return InliningStatus_NotInlined;
    }

    // Check for STR.charCodeAt(IDX) where STR is a constant string and IDX is a
    // constant integer.
    InliningStatus constInlineStatus;
    MOZ_TRY_VAR(constInlineStatus, inlineConstantCharCodeAt(callInfo));
    if (constInlineStatus != InliningStatus_NotInlined) {
        return constInlineStatus;
    }

    callInfo.setImplicitlyUsedUnchecked();

    MInstruction* index = MToNumberInt32::New(alloc(), callInfo.getArg(0));
    current->add(index);

    MStringLength* length = MStringLength::New(alloc(), callInfo.thisArg());
    current->add(length);

    index = addBoundsCheck(index, length);
=======
IonBuilder::InliningResult IonBuilder::inlineStringSplitString(
    CallInfo& callInfo) {
  MOZ_ASSERT(!callInfo.constructing());
  MOZ_ASSERT(callInfo.argc() == 2);

  MDefinition* strArg = callInfo.getArg(0);
  MDefinition* sepArg = callInfo.getArg(1);
>>>>>>> upstream-releases

<<<<<<< HEAD
  MDefinition* strArg = callInfo.getArg(0);
  MDefinition* sepArg = callInfo.getArg(1);
||||||| merged common ancestors
    MCharCodeAt* charCode = MCharCodeAt::New(alloc(), callInfo.thisArg(), index);
    current->add(charCode);
    current->push(charCode);
    return InliningStatus_Inlined;
}
=======
  if (strArg->type() != MIRType::String) {
    return InliningStatus_NotInlined;
  }
>>>>>>> upstream-releases

<<<<<<< HEAD
  if (strArg->type() != MIRType::String) {
    return InliningStatus_NotInlined;
  }
||||||| merged common ancestors
IonBuilder::InliningResult
IonBuilder::inlineConstantCharCodeAt(CallInfo& callInfo)
{
    if (!callInfo.thisArg()->maybeConstantValue() || !callInfo.getArg(0)->maybeConstantValue()) {
        trackOptimizationOutcome(TrackedOutcome::CantInlineGeneric);
        return InliningStatus_NotInlined;
    }
=======
  if (sepArg->type() != MIRType::String) {
    return InliningStatus_NotInlined;
  }
>>>>>>> upstream-releases

<<<<<<< HEAD
  if (sepArg->type() != MIRType::String) {
    return InliningStatus_NotInlined;
  }

  IonBuilder::InliningStatus resultConstStringSplit;
  MOZ_TRY_VAR(resultConstStringSplit,
              inlineConstantStringSplitString(callInfo));
  if (resultConstStringSplit != InliningStatus_NotInlined) {
    return resultConstStringSplit;
  }

  JSContext* cx = TlsContext.get();
  ObjectGroup* group = ObjectGroupRealm::getStringSplitStringGroup(cx);
  if (!group) {
    return InliningStatus_NotInlined;
  }
  AutoSweepObjectGroup sweep(group);
  if (group->maybePreliminaryObjects(sweep)) {
    return InliningStatus_NotInlined;
  }
||||||| merged common ancestors
    MConstant* strval = callInfo.thisArg()->maybeConstantValue();
    MConstant* idxval = callInfo.getArg(0)->maybeConstantValue();
=======
  JSContext* cx = TlsContext.get();
  ObjectGroup* group = ObjectGroupRealm::getStringSplitStringGroup(cx);
  if (!group) {
    return InliningStatus_NotInlined;
  }
>>>>>>> upstream-releases

  TypeSet::ObjectKey* retKey = TypeSet::ObjectKey::get(group);
  if (retKey->unknownProperties()) {
    return InliningStatus_NotInlined;
  }

  HeapTypeSetKey key = retKey->property(JSID_VOID);
  if (!key.maybeTypes()) {
    return InliningStatus_NotInlined;
  }

  if (!key.maybeTypes()->hasType(TypeSet::StringType())) {
    key.freeze(constraints());
    return InliningStatus_NotInlined;
  }

  callInfo.setImplicitlyUsedUnchecked();
  MStringSplit* ins =
      MStringSplit::New(alloc(), constraints(), strArg, sepArg, group);
  current->add(ins);
  current->push(ins);

  return InliningStatus_Inlined;
}

IonBuilder::InliningResult IonBuilder::inlineObjectHasPrototype(
    CallInfo& callInfo) {
  MOZ_ASSERT(!callInfo.constructing());
  MOZ_ASSERT(callInfo.argc() == 2);

  MDefinition* objArg = callInfo.getArg(0);
  MDefinition* protoArg = callInfo.getArg(1);

  if (objArg->type() != MIRType::Object) {
    return InliningStatus_NotInlined;
  }
  if (protoArg->type() != MIRType::Object) {
    return InliningStatus_NotInlined;
  }

  // Inline only when both obj and proto are singleton objects and
  // obj does not have uncacheable proto and obj.__proto__ is proto.
  TemporaryTypeSet* objTypes = objArg->resultTypeSet();
  if (!objTypes || objTypes->unknownObject() ||
      objTypes->getObjectCount() != 1) {
    return InliningStatus_NotInlined;
  }

  TypeSet::ObjectKey* objKey = objTypes->getObject(0);
  if (!objKey || !objKey->hasStableClassAndProto(constraints())) {
    return InliningStatus_NotInlined;
  }
  if (!objKey->isSingleton() || !objKey->singleton()->is<NativeObject>()) {
    return InliningStatus_NotInlined;
  }

  JSObject* obj = &objKey->singleton()->as<NativeObject>();
  if (obj->hasUncacheableProto()) {
    return InliningStatus_NotInlined;
  }

  JSObject* actualProto = checkNurseryObject(objKey->proto().toObjectOrNull());
  if (actualProto == nullptr) {
    return InliningStatus_NotInlined;
  }

  TemporaryTypeSet* protoTypes = protoArg->resultTypeSet();
  if (!protoTypes || protoTypes->unknownObject() ||
      protoTypes->getObjectCount() != 1) {
    return InliningStatus_NotInlined;
  }

  TypeSet::ObjectKey* protoKey = protoTypes->getObject(0);
  if (!protoKey || !protoKey->hasStableClassAndProto(constraints())) {
    return InliningStatus_NotInlined;
  }
  if (!protoKey->isSingleton() || !protoKey->singleton()->is<NativeObject>()) {
    return InliningStatus_NotInlined;
  }

  JSObject* proto = &protoKey->singleton()->as<NativeObject>();
  pushConstant(BooleanValue(proto == actualProto));
  callInfo.setImplicitlyUsedUnchecked();
  return InliningStatus_Inlined;
}

<<<<<<< HEAD
IonBuilder::InliningResult IonBuilder::inlineFinishBoundFunctionInit(
    CallInfo& callInfo) {
  MOZ_ASSERT(!callInfo.constructing());
  MOZ_ASSERT(callInfo.argc() == 3);
  MOZ_ASSERT(BytecodeIsPopped(pc));
||||||| merged common ancestors
IonBuilder::InliningResult
IonBuilder::inlineStrCharAt(CallInfo& callInfo)
{
    if (callInfo.argc() != 1 || callInfo.constructing()) {
        trackOptimizationOutcome(TrackedOutcome::CantInlineNativeBadForm);
        return InliningStatus_NotInlined;
    }

    if (getInlineReturnType() != MIRType::String) {
        return InliningStatus_NotInlined;
    }
    if (callInfo.thisArg()->type() != MIRType::String) {
        return InliningStatus_NotInlined;
    }
    MIRType argType = callInfo.getArg(0)->type();
    if (argType != MIRType::Int32 && argType != MIRType::Double) {
        return InliningStatus_NotInlined;
    }

    callInfo.setImplicitlyUsedUnchecked();

    MInstruction* index = MToNumberInt32::New(alloc(), callInfo.getArg(0));
    current->add(index);

    MStringLength* length = MStringLength::New(alloc(), callInfo.thisArg());
    current->add(length);

    index = addBoundsCheck(index, length);
=======
IonBuilder::InliningResult IonBuilder::inlineFinishBoundFunctionInit(
    CallInfo& callInfo) {
  MOZ_ASSERT(!callInfo.constructing());
  MOZ_ASSERT(callInfo.argc() == 3);
  MOZ_ASSERT(BytecodeIsPopped(pc));

  MDefinition* boundFunction = callInfo.getArg(0);
  MDefinition* targetFunction = callInfo.getArg(1);
  MDefinition* argCount = callInfo.getArg(2);

  if (boundFunction->type() != MIRType::Object) {
    return InliningStatus_NotInlined;
  }
  if (targetFunction->type() != MIRType::Object) {
    return InliningStatus_NotInlined;
  }
  if (argCount->type() != MIRType::Int32) {
    return InliningStatus_NotInlined;
  }

  callInfo.setImplicitlyUsedUnchecked();

  auto* ins = MFinishBoundFunctionInit::New(alloc(), boundFunction,
                                            targetFunction, argCount);
  current->add(ins);

  pushConstant(UndefinedValue());
>>>>>>> upstream-releases

<<<<<<< HEAD
  MDefinition* boundFunction = callInfo.getArg(0);
  MDefinition* targetFunction = callInfo.getArg(1);
  MDefinition* argCount = callInfo.getArg(2);

  if (boundFunction->type() != MIRType::Object) {
    return InliningStatus_NotInlined;
  }
  if (targetFunction->type() != MIRType::Object) {
    return InliningStatus_NotInlined;
  }
  if (argCount->type() != MIRType::Int32) {
    return InliningStatus_NotInlined;
  }
||||||| merged common ancestors
    // String.charAt(x) = String.fromCharCode(String.charCodeAt(x))
    MCharCodeAt* charCode = MCharCodeAt::New(alloc(), callInfo.thisArg(), index);
    current->add(charCode);

    MFromCharCode* string = MFromCharCode::New(alloc(), charCode);
    current->add(string);
    current->push(string);
    return InliningStatus_Inlined;
}
=======
  MOZ_TRY(resumeAfter(ins));
  return InliningStatus_Inlined;
}
>>>>>>> upstream-releases

<<<<<<< HEAD
  callInfo.setImplicitlyUsedUnchecked();
||||||| merged common ancestors
IonBuilder::InliningResult
IonBuilder::inlineStringConvertCase(CallInfo& callInfo, MStringConvertCase::Mode mode)
{
    if (callInfo.argc() != 0 || callInfo.constructing()) {
        trackOptimizationOutcome(TrackedOutcome::CantInlineNativeBadForm);
        return InliningStatus_NotInlined;
    }
=======
IonBuilder::InliningResult IonBuilder::inlineIsPackedArray(CallInfo& callInfo) {
  MOZ_ASSERT(!callInfo.constructing());
  MOZ_ASSERT(callInfo.argc() == 1);
>>>>>>> upstream-releases

<<<<<<< HEAD
  auto* ins = MFinishBoundFunctionInit::New(alloc(), boundFunction,
                                            targetFunction, argCount);
  current->add(ins);

  pushConstant(UndefinedValue());

  MOZ_TRY(resumeAfter(ins));
  return InliningStatus_Inlined;
}

IonBuilder::InliningResult IonBuilder::inlineIsPackedArray(CallInfo& callInfo) {
  MOZ_ASSERT(!callInfo.constructing());
  MOZ_ASSERT(callInfo.argc() == 1);
||||||| merged common ancestors
    if (getInlineReturnType() != MIRType::String) {
        return InliningStatus_NotInlined;
    }
    if (callInfo.thisArg()->type() != MIRType::String) {
        return InliningStatus_NotInlined;
    }

    callInfo.setImplicitlyUsedUnchecked();

    auto* ins = MStringConvertCase::New(alloc(), callInfo.thisArg(), mode);
    current->add(ins);
    current->push(ins);
    return InliningStatus_Inlined;
}

IonBuilder::InliningResult
IonBuilder::inlineRegExpMatcher(CallInfo& callInfo)
{
    // This is called from Self-hosted JS, after testing each argument,
    // most of following tests should be passed.

    MOZ_ASSERT(!callInfo.constructing());
    MOZ_ASSERT(callInfo.argc() == 3);
=======
  if (getInlineReturnType() != MIRType::Boolean) {
    return InliningStatus_NotInlined;
  }
>>>>>>> upstream-releases

<<<<<<< HEAD
  if (getInlineReturnType() != MIRType::Boolean) {
    return InliningStatus_NotInlined;
  }
||||||| merged common ancestors
    MDefinition* rxArg = callInfo.getArg(0);
    MDefinition* strArg = callInfo.getArg(1);
    MDefinition* lastIndexArg = callInfo.getArg(2);
=======
  MDefinition* array = callInfo.getArg(0);
>>>>>>> upstream-releases

<<<<<<< HEAD
  MDefinition* array = callInfo.getArg(0);
||||||| merged common ancestors
    if (rxArg->type() != MIRType::Object && rxArg->type() != MIRType::Value) {
        return InliningStatus_NotInlined;
    }
=======
  if (array->type() != MIRType::Object) {
    return InliningStatus_NotInlined;
  }
>>>>>>> upstream-releases

<<<<<<< HEAD
  if (array->type() != MIRType::Object) {
    return InliningStatus_NotInlined;
  }
||||||| merged common ancestors
    TemporaryTypeSet* rxTypes = rxArg->resultTypeSet();
    const Class* clasp = rxTypes ? rxTypes->getKnownClass(constraints()) : nullptr;
    if (clasp != &RegExpObject::class_) {
        return InliningStatus_NotInlined;
    }
=======
  TemporaryTypeSet* arrayTypes = array->resultTypeSet();
  if (!arrayTypes) {
    return InliningStatus_NotInlined;
  }
>>>>>>> upstream-releases

<<<<<<< HEAD
  TemporaryTypeSet* arrayTypes = array->resultTypeSet();
  if (!arrayTypes) {
    return InliningStatus_NotInlined;
  }
||||||| merged common ancestors
    if (strArg->type() != MIRType::String && strArg->type() != MIRType::Value) {
        return InliningStatus_NotInlined;
    }
=======
  const Class* clasp = arrayTypes->getKnownClass(constraints());
  if (clasp != &ArrayObject::class_) {
    return InliningStatus_NotInlined;
  }
>>>>>>> upstream-releases

<<<<<<< HEAD
  const Class* clasp = arrayTypes->getKnownClass(constraints());
  if (clasp != &ArrayObject::class_) {
    return InliningStatus_NotInlined;
  }
||||||| merged common ancestors
    if (lastIndexArg->type() != MIRType::Int32) {
        return InliningStatus_NotInlined;
    }
=======
  // Only inline if the array uses dense storage.
  ObjectGroupFlags unhandledFlags = OBJECT_FLAG_SPARSE_INDEXES |
                                    OBJECT_FLAG_LENGTH_OVERFLOW |
                                    OBJECT_FLAG_NON_PACKED;
>>>>>>> upstream-releases

<<<<<<< HEAD
  // Only inline if the array uses dense storage.
  ObjectGroupFlags unhandledFlags = OBJECT_FLAG_SPARSE_INDEXES |
                                    OBJECT_FLAG_LENGTH_OVERFLOW |
                                    OBJECT_FLAG_NON_PACKED;
||||||| merged common ancestors
    JSContext* cx = TlsContext.get();
    if (!cx->realm()->jitRealm()->ensureRegExpMatcherStubExists(cx)) {
        cx->clearPendingException(); // OOM or overrecursion.
        return InliningStatus_NotInlined;
    }
=======
  if (arrayTypes->hasObjectFlags(constraints(), unhandledFlags)) {
    trackOptimizationOutcome(TrackedOutcome::ArrayBadFlags);
    return InliningStatus_NotInlined;
  }
>>>>>>> upstream-releases

<<<<<<< HEAD
  if (arrayTypes->hasObjectFlags(constraints(), unhandledFlags)) {
    trackOptimizationOutcome(TrackedOutcome::ArrayBadFlags);
    return InliningStatus_NotInlined;
  }
||||||| merged common ancestors
    callInfo.setImplicitlyUsedUnchecked();
=======
  callInfo.setImplicitlyUsedUnchecked();
>>>>>>> upstream-releases

<<<<<<< HEAD
  callInfo.setImplicitlyUsedUnchecked();
||||||| merged common ancestors
    MInstruction* matcher = MRegExpMatcher::New(alloc(), rxArg, strArg, lastIndexArg);
    current->add(matcher);
    current->push(matcher);
=======
  auto* ins = MIsPackedArray::New(alloc(), array);
  current->add(ins);
  current->push(ins);
>>>>>>> upstream-releases

<<<<<<< HEAD
  auto* ins = MIsPackedArray::New(alloc(), array);
  current->add(ins);
  current->push(ins);

  return InliningStatus_Inlined;
||||||| merged common ancestors
    MOZ_TRY(resumeAfter(matcher));
    MOZ_TRY(pushTypeBarrier(matcher, getInlineReturnTypeSet(), BarrierKind::TypeSet));
    return InliningStatus_Inlined;
=======
  return InliningStatus_Inlined;
>>>>>>> upstream-releases
}

<<<<<<< HEAD
IonBuilder::InliningResult IonBuilder::inlineReflectGetPrototypeOf(
    CallInfo& callInfo) {
  if (callInfo.argc() != 1 || callInfo.constructing()) {
    trackOptimizationOutcome(TrackedOutcome::CantInlineNativeBadForm);
    return InliningStatus_NotInlined;
  }

  MDefinition* target = callInfo.getArg(0);
  if (target->type() != MIRType::Object) {
    return InliningStatus_NotInlined;
  }

  callInfo.setImplicitlyUsedUnchecked();

  auto* ins = MGetPrototypeOf::New(alloc(), target);
  current->add(ins);
  current->push(ins);

  MOZ_TRY(resumeAfter(ins));
  MOZ_TRY(pushTypeBarrier(ins, getInlineReturnTypeSet(), BarrierKind::TypeSet));
  return InliningStatus_Inlined;
}

IonBuilder::InliningResult IonBuilder::inlineStrCharCodeAt(CallInfo& callInfo) {
  if (callInfo.argc() != 1 || callInfo.constructing()) {
    trackOptimizationOutcome(TrackedOutcome::CantInlineNativeBadForm);
    return InliningStatus_NotInlined;
  }

  if (getInlineReturnType() != MIRType::Int32) {
    return InliningStatus_NotInlined;
  }
  if (callInfo.thisArg()->type() != MIRType::String &&
      callInfo.thisArg()->type() != MIRType::Value) {
    return InliningStatus_NotInlined;
  }
  MIRType argType = callInfo.getArg(0)->type();
  if (argType != MIRType::Int32 && argType != MIRType::Double) {
    return InliningStatus_NotInlined;
  }
||||||| merged common ancestors
IonBuilder::InliningResult
IonBuilder::inlineRegExpSearcher(CallInfo& callInfo)
{
    // This is called from Self-hosted JS, after testing each argument,
    // most of following tests should be passed.

    MOZ_ASSERT(!callInfo.constructing());
    MOZ_ASSERT(callInfo.argc() == 3);

    MDefinition* rxArg = callInfo.getArg(0);
    MDefinition* strArg = callInfo.getArg(1);
    MDefinition* lastIndexArg = callInfo.getArg(2);

    if (rxArg->type() != MIRType::Object && rxArg->type() != MIRType::Value) {
        return InliningStatus_NotInlined;
    }

    TemporaryTypeSet* regexpTypes = rxArg->resultTypeSet();
    const Class* clasp = regexpTypes ? regexpTypes->getKnownClass(constraints()) : nullptr;
    if (clasp != &RegExpObject::class_) {
        return InliningStatus_NotInlined;
    }

    if (strArg->type() != MIRType::String && strArg->type() != MIRType::Value) {
        return InliningStatus_NotInlined;
    }

    if (lastIndexArg->type() != MIRType::Int32) {
        return InliningStatus_NotInlined;
    }
=======
IonBuilder::InliningResult IonBuilder::inlineReflectGetPrototypeOf(
    CallInfo& callInfo) {
  if (callInfo.argc() != 1 || callInfo.constructing()) {
    trackOptimizationOutcome(TrackedOutcome::CantInlineNativeBadForm);
    return InliningStatus_NotInlined;
  }
>>>>>>> upstream-releases

<<<<<<< HEAD
  // Check for STR.charCodeAt(IDX) where STR is a constant string and IDX is a
  // constant integer.
  InliningStatus constInlineStatus;
  MOZ_TRY_VAR(constInlineStatus, inlineConstantCharCodeAt(callInfo));
  if (constInlineStatus != InliningStatus_NotInlined) {
    return constInlineStatus;
  }
||||||| merged common ancestors
    JSContext* cx = TlsContext.get();
    if (!cx->realm()->jitRealm()->ensureRegExpSearcherStubExists(cx)) {
        cx->clearPendingException(); // OOM or overrecursion.
        return abort(AbortReason::Error);
    }
=======
  MDefinition* target = callInfo.getArg(0);
  if (target->type() != MIRType::Object) {
    return InliningStatus_NotInlined;
  }
>>>>>>> upstream-releases

  callInfo.setImplicitlyUsedUnchecked();

<<<<<<< HEAD
  MInstruction* index = MToNumberInt32::New(alloc(), callInfo.getArg(0));
  current->add(index);
||||||| merged common ancestors
    MInstruction* searcher = MRegExpSearcher::New(alloc(), rxArg, strArg, lastIndexArg);
    current->add(searcher);
    current->push(searcher);
=======
  auto* ins = MGetPrototypeOf::New(alloc(), target);
  current->add(ins);
  current->push(ins);
>>>>>>> upstream-releases

<<<<<<< HEAD
  MStringLength* length = MStringLength::New(alloc(), callInfo.thisArg());
  current->add(length);

  index = addBoundsCheck(index, length);

  MCharCodeAt* charCode = MCharCodeAt::New(alloc(), callInfo.thisArg(), index);
  current->add(charCode);
  current->push(charCode);
  return InliningStatus_Inlined;
||||||| merged common ancestors
    MOZ_TRY(resumeAfter(searcher));
    MOZ_TRY(pushTypeBarrier(searcher, getInlineReturnTypeSet(), BarrierKind::TypeSet));
    return InliningStatus_Inlined;
=======
  MOZ_TRY(resumeAfter(ins));
  MOZ_TRY(pushTypeBarrier(ins, getInlineReturnTypeSet(), BarrierKind::TypeSet));
  return InliningStatus_Inlined;
>>>>>>> upstream-releases
}

<<<<<<< HEAD
IonBuilder::InliningResult IonBuilder::inlineConstantCharCodeAt(
    CallInfo& callInfo) {
  if (!callInfo.thisArg()->maybeConstantValue() ||
      !callInfo.getArg(0)->maybeConstantValue()) {
    trackOptimizationOutcome(TrackedOutcome::CantInlineGeneric);
    return InliningStatus_NotInlined;
  }

  MConstant* strval = callInfo.thisArg()->maybeConstantValue();
  MConstant* idxval = callInfo.getArg(0)->maybeConstantValue();

  if (strval->type() != MIRType::String || idxval->type() != MIRType::Int32) {
    return InliningStatus_NotInlined;
  }

  JSString* str = strval->toString();
  if (!str->isLinear()) {
    trackOptimizationOutcome(TrackedOutcome::CantInlineGeneric);
    return InliningStatus_NotInlined;
  }
||||||| merged common ancestors
IonBuilder::InliningResult
IonBuilder::inlineRegExpTester(CallInfo& callInfo)
{
    // This is called from Self-hosted JS, after testing each argument,
    // most of following tests should be passed.

    MOZ_ASSERT(!callInfo.constructing());
    MOZ_ASSERT(callInfo.argc() == 3);

    MDefinition* rxArg = callInfo.getArg(0);
    MDefinition* strArg = callInfo.getArg(1);
    MDefinition* lastIndexArg = callInfo.getArg(2);

    if (rxArg->type() != MIRType::Object && rxArg->type() != MIRType::Value) {
        return InliningStatus_NotInlined;
    }
=======
IonBuilder::InliningResult IonBuilder::inlineStrCharCodeAt(CallInfo& callInfo) {
  if (callInfo.argc() != 1 || callInfo.constructing()) {
    trackOptimizationOutcome(TrackedOutcome::CantInlineNativeBadForm);
    return InliningStatus_NotInlined;
  }
>>>>>>> upstream-releases

<<<<<<< HEAD
  int32_t idx = idxval->toInt32();
  if (idx < 0 || (uint32_t(idx) >= str->length())) {
    trackOptimizationOutcome(TrackedOutcome::OutOfBounds);
    return InliningStatus_NotInlined;
  }
||||||| merged common ancestors
    TemporaryTypeSet* rxTypes = rxArg->resultTypeSet();
    const Class* clasp = rxTypes ? rxTypes->getKnownClass(constraints()) : nullptr;
    if (clasp != &RegExpObject::class_) {
        return InliningStatus_NotInlined;
    }
=======
  if (getInlineReturnType() != MIRType::Int32) {
    return InliningStatus_NotInlined;
  }
  if (callInfo.thisArg()->type() != MIRType::String &&
      callInfo.thisArg()->type() != MIRType::Value) {
    return InliningStatus_NotInlined;
  }
  MIRType argType = callInfo.getArg(0)->type();
  if (argType != MIRType::Int32 && argType != MIRType::Double) {
    return InliningStatus_NotInlined;
  }
>>>>>>> upstream-releases

<<<<<<< HEAD
  callInfo.setImplicitlyUsedUnchecked();
||||||| merged common ancestors
    if (strArg->type() != MIRType::String && strArg->type() != MIRType::Value) {
        return InliningStatus_NotInlined;
    }
=======
  // Check for STR.charCodeAt(IDX) where STR is a constant string and IDX is a
  // constant integer.
  InliningStatus constInlineStatus;
  MOZ_TRY_VAR(constInlineStatus, inlineConstantCharCodeAt(callInfo));
  if (constInlineStatus != InliningStatus_NotInlined) {
    return constInlineStatus;
  }
>>>>>>> upstream-releases

<<<<<<< HEAD
  JSLinearString& linstr = str->asLinear();
  char16_t ch = linstr.latin1OrTwoByteChar(idx);
  MConstant* result = MConstant::New(alloc(), Int32Value(ch));
  current->add(result);
  current->push(result);
  return InliningStatus_Inlined;
}
||||||| merged common ancestors
    if (lastIndexArg->type() != MIRType::Int32) {
        return InliningStatus_NotInlined;
    }
=======
  callInfo.setImplicitlyUsedUnchecked();
>>>>>>> upstream-releases

<<<<<<< HEAD
IonBuilder::InliningResult IonBuilder::inlineStrFromCharCode(
    CallInfo& callInfo) {
  if (callInfo.argc() != 1 || callInfo.constructing()) {
    trackOptimizationOutcome(TrackedOutcome::CantInlineNativeBadForm);
    return InliningStatus_NotInlined;
  }
||||||| merged common ancestors
    JSContext* cx = TlsContext.get();
    if (!cx->realm()->jitRealm()->ensureRegExpTesterStubExists(cx)) {
        cx->clearPendingException(); // OOM or overrecursion.
        return InliningStatus_NotInlined;
    }
=======
  MInstruction* index = MToNumberInt32::New(alloc(), callInfo.getArg(0));
  current->add(index);
>>>>>>> upstream-releases

<<<<<<< HEAD
  if (getInlineReturnType() != MIRType::String) {
    return InliningStatus_NotInlined;
  }
||||||| merged common ancestors
    callInfo.setImplicitlyUsedUnchecked();
=======
  MStringLength* length = MStringLength::New(alloc(), callInfo.thisArg());
  current->add(length);
>>>>>>> upstream-releases

<<<<<<< HEAD
  MDefinition* codeUnit = callInfo.getArg(0);
  if (codeUnit->type() != MIRType::Int32) {
    // MTruncateToInt32 will always bail for objects and symbols, so don't
    // try to inline String.fromCharCode() for these two value types.
    if (codeUnit->mightBeType(MIRType::Object) ||
        codeUnit->mightBeType(MIRType::Symbol)) {
      return InliningStatus_NotInlined;
    }
||||||| merged common ancestors
    MInstruction* tester = MRegExpTester::New(alloc(), rxArg, strArg, lastIndexArg);
    current->add(tester);
    current->push(tester);
=======
  index = addBoundsCheck(index, length);
>>>>>>> upstream-releases

<<<<<<< HEAD
    codeUnit = MTruncateToInt32::New(alloc(), codeUnit);
    current->add(codeUnit->toInstruction());
  }
||||||| merged common ancestors
    MOZ_TRY(resumeAfter(tester));
    return InliningStatus_Inlined;
}
=======
  MCharCodeAt* charCode = MCharCodeAt::New(alloc(), callInfo.thisArg(), index);
  current->add(charCode);
  current->push(charCode);
  return InliningStatus_Inlined;
}
>>>>>>> upstream-releases

<<<<<<< HEAD
  callInfo.setImplicitlyUsedUnchecked();
||||||| merged common ancestors
IonBuilder::InliningResult
IonBuilder::inlineIsRegExpObject(CallInfo& callInfo)
{
    MOZ_ASSERT(!callInfo.constructing());
    MOZ_ASSERT(callInfo.argc() == 1);
=======
IonBuilder::InliningResult IonBuilder::inlineConstantCharCodeAt(
    CallInfo& callInfo) {
  if (!callInfo.thisArg()->maybeConstantValue() ||
      !callInfo.getArg(0)->maybeConstantValue()) {
    trackOptimizationOutcome(TrackedOutcome::CantInlineGeneric);
    return InliningStatus_NotInlined;
  }
>>>>>>> upstream-releases

<<<<<<< HEAD
  MFromCharCode* string = MFromCharCode::New(alloc(), codeUnit);
  current->add(string);
  current->push(string);
  return InliningStatus_Inlined;
}
||||||| merged common ancestors
    if (getInlineReturnType() != MIRType::Boolean) {
        return InliningStatus_NotInlined;
    }
=======
  MConstant* strval = callInfo.thisArg()->maybeConstantValue();
  MConstant* idxval = callInfo.getArg(0)->maybeConstantValue();
>>>>>>> upstream-releases

<<<<<<< HEAD
IonBuilder::InliningResult IonBuilder::inlineStrFromCodePoint(
    CallInfo& callInfo) {
  if (callInfo.argc() != 1 || callInfo.constructing()) {
    trackOptimizationOutcome(TrackedOutcome::CantInlineNativeBadForm);
    return InliningStatus_NotInlined;
  }
||||||| merged common ancestors
    MDefinition* arg = callInfo.getArg(0);
=======
  if (strval->type() != MIRType::String || idxval->type() != MIRType::Int32) {
    return InliningStatus_NotInlined;
  }
>>>>>>> upstream-releases

<<<<<<< HEAD
  if (getInlineReturnType() != MIRType::String) {
    return InliningStatus_NotInlined;
  }
  if (callInfo.getArg(0)->type() != MIRType::Int32) {
    return InliningStatus_NotInlined;
  }
||||||| merged common ancestors
    bool isRegExpObjectKnown = false;
    bool isRegExpObjectConstant;
    if (arg->type() == MIRType::Object) {
        TemporaryTypeSet* types = arg->resultTypeSet();
        const Class* clasp = types ? types->getKnownClass(constraints()) : nullptr;
        if (clasp) {
            isRegExpObjectKnown = true;
            isRegExpObjectConstant = (clasp == &RegExpObject::class_);
        }
    } else if (!arg->mightBeType(MIRType::Object)) {
        // NB: This case only happens when Phi-nodes flow into IsRegExpObject.
        // IsRegExpObject itself will never be called with a non-Object value.
        isRegExpObjectKnown = true;
        isRegExpObjectConstant = false;
    } else if (arg->type() != MIRType::Value) {
        return InliningStatus_NotInlined;
    }
=======
  JSString* str = strval->toString();
  if (!str->isLinear()) {
    trackOptimizationOutcome(TrackedOutcome::CantInlineGeneric);
    return InliningStatus_NotInlined;
  }
>>>>>>> upstream-releases

<<<<<<< HEAD
  callInfo.setImplicitlyUsedUnchecked();
||||||| merged common ancestors
    if (isRegExpObjectKnown) {
        pushConstant(BooleanValue(isRegExpObjectConstant));
    } else {
        MHasClass* hasClass = MHasClass::New(alloc(), arg, &RegExpObject::class_);
        current->add(hasClass);
        current->push(hasClass);
    }
=======
  int32_t idx = idxval->toInt32();
  if (idx < 0 || (uint32_t(idx) >= str->length())) {
    trackOptimizationOutcome(TrackedOutcome::OutOfBounds);
    return InliningStatus_NotInlined;
  }
>>>>>>> upstream-releases

<<<<<<< HEAD
  MFromCodePoint* string = MFromCodePoint::New(alloc(), callInfo.getArg(0));
  current->add(string);
  current->push(string);
  return InliningStatus_Inlined;
||||||| merged common ancestors
    callInfo.setImplicitlyUsedUnchecked();
    return InliningStatus_Inlined;
=======
  callInfo.setImplicitlyUsedUnchecked();

  JSLinearString& linstr = str->asLinear();
  char16_t ch = linstr.latin1OrTwoByteChar(idx);
  MConstant* result = MConstant::New(alloc(), Int32Value(ch));
  current->add(result);
  current->push(result);
  return InliningStatus_Inlined;
>>>>>>> upstream-releases
}

<<<<<<< HEAD
IonBuilder::InliningResult IonBuilder::inlineStrCharAt(CallInfo& callInfo) {
  if (callInfo.argc() != 1 || callInfo.constructing()) {
    trackOptimizationOutcome(TrackedOutcome::CantInlineNativeBadForm);
    return InliningStatus_NotInlined;
  }

  if (getInlineReturnType() != MIRType::String) {
    return InliningStatus_NotInlined;
  }
  if (callInfo.thisArg()->type() != MIRType::String) {
    return InliningStatus_NotInlined;
  }
  MIRType argType = callInfo.getArg(0)->type();
  if (argType != MIRType::Int32 && argType != MIRType::Double) {
    return InliningStatus_NotInlined;
  }
||||||| merged common ancestors
IonBuilder::InliningResult
IonBuilder::inlineRegExpPrototypeOptimizable(CallInfo& callInfo)
{
    MOZ_ASSERT(!callInfo.constructing());
    MOZ_ASSERT(callInfo.argc() == 1);
=======
IonBuilder::InliningResult IonBuilder::inlineStrFromCharCode(
    CallInfo& callInfo) {
  if (callInfo.argc() != 1 || callInfo.constructing()) {
    trackOptimizationOutcome(TrackedOutcome::CantInlineNativeBadForm);
    return InliningStatus_NotInlined;
  }
>>>>>>> upstream-releases

<<<<<<< HEAD
  callInfo.setImplicitlyUsedUnchecked();
||||||| merged common ancestors
    MDefinition* protoArg = callInfo.getArg(0);
=======
  if (getInlineReturnType() != MIRType::String) {
    return InliningStatus_NotInlined;
  }
>>>>>>> upstream-releases

<<<<<<< HEAD
  MInstruction* index = MToNumberInt32::New(alloc(), callInfo.getArg(0));
  current->add(index);
||||||| merged common ancestors
    if (protoArg->type() != MIRType::Object) {
        return InliningStatus_NotInlined;
    }
=======
  MDefinition* codeUnit = callInfo.getArg(0);
  if (codeUnit->type() != MIRType::Int32) {
    // MTruncateToInt32 will always bail for objects, symbols and BigInts, so
    // don't try to inline String.fromCharCode() for these value types.
    if (codeUnit->mightBeType(MIRType::Object) ||
        codeUnit->mightBeType(MIRType::Symbol) ||
        codeUnit->mightBeType(MIRType::BigInt)) {
      return InliningStatus_NotInlined;
    }
>>>>>>> upstream-releases

<<<<<<< HEAD
  MStringLength* length = MStringLength::New(alloc(), callInfo.thisArg());
  current->add(length);
||||||| merged common ancestors
    if (getInlineReturnType() != MIRType::Boolean) {
        return InliningStatus_NotInlined;
    }
=======
    codeUnit = MTruncateToInt32::New(alloc(), codeUnit);
    current->add(codeUnit->toInstruction());
  }
>>>>>>> upstream-releases

<<<<<<< HEAD
  index = addBoundsCheck(index, length);
||||||| merged common ancestors
    callInfo.setImplicitlyUsedUnchecked();
=======
  callInfo.setImplicitlyUsedUnchecked();
>>>>>>> upstream-releases

<<<<<<< HEAD
  // String.charAt(x) = String.fromCharCode(String.charCodeAt(x))
  MCharCodeAt* charCode = MCharCodeAt::New(alloc(), callInfo.thisArg(), index);
  current->add(charCode);
||||||| merged common ancestors
    MInstruction* opt = MRegExpPrototypeOptimizable::New(alloc(), protoArg);
    current->add(opt);
    current->push(opt);
=======
  MFromCharCode* string = MFromCharCode::New(alloc(), codeUnit);
  current->add(string);
  current->push(string);
  return InliningStatus_Inlined;
}
>>>>>>> upstream-releases

<<<<<<< HEAD
  MFromCharCode* string = MFromCharCode::New(alloc(), charCode);
  current->add(string);
  current->push(string);
  return InliningStatus_Inlined;
||||||| merged common ancestors
    return InliningStatus_Inlined;
=======
IonBuilder::InliningResult IonBuilder::inlineStrFromCodePoint(
    CallInfo& callInfo) {
  if (callInfo.argc() != 1 || callInfo.constructing()) {
    trackOptimizationOutcome(TrackedOutcome::CantInlineNativeBadForm);
    return InliningStatus_NotInlined;
  }

  if (getInlineReturnType() != MIRType::String) {
    return InliningStatus_NotInlined;
  }
  if (callInfo.getArg(0)->type() != MIRType::Int32) {
    return InliningStatus_NotInlined;
  }

  callInfo.setImplicitlyUsedUnchecked();

  MFromCodePoint* string = MFromCodePoint::New(alloc(), callInfo.getArg(0));
  current->add(string);
  current->push(string);
  return InliningStatus_Inlined;
>>>>>>> upstream-releases
}

<<<<<<< HEAD
IonBuilder::InliningResult IonBuilder::inlineStringConvertCase(
    CallInfo& callInfo, MStringConvertCase::Mode mode) {
  if (callInfo.argc() != 0 || callInfo.constructing()) {
    trackOptimizationOutcome(TrackedOutcome::CantInlineNativeBadForm);
    return InliningStatus_NotInlined;
  }
||||||| merged common ancestors
IonBuilder::InliningResult
IonBuilder::inlineRegExpInstanceOptimizable(CallInfo& callInfo)
{
    MOZ_ASSERT(!callInfo.constructing());
    MOZ_ASSERT(callInfo.argc() == 2);
=======
IonBuilder::InliningResult IonBuilder::inlineStrCharAt(CallInfo& callInfo) {
  if (callInfo.argc() != 1 || callInfo.constructing()) {
    trackOptimizationOutcome(TrackedOutcome::CantInlineNativeBadForm);
    return InliningStatus_NotInlined;
  }
>>>>>>> upstream-releases

<<<<<<< HEAD
  if (getInlineReturnType() != MIRType::String) {
    return InliningStatus_NotInlined;
  }
  if (callInfo.thisArg()->type() != MIRType::String) {
    return InliningStatus_NotInlined;
  }
||||||| merged common ancestors
    MDefinition* rxArg = callInfo.getArg(0);
    MDefinition* protoArg = callInfo.getArg(1);
=======
  if (getInlineReturnType() != MIRType::String) {
    return InliningStatus_NotInlined;
  }
  if (callInfo.thisArg()->type() != MIRType::String) {
    return InliningStatus_NotInlined;
  }
  MIRType argType = callInfo.getArg(0)->type();
  if (argType != MIRType::Int32 && argType != MIRType::Double) {
    return InliningStatus_NotInlined;
  }
>>>>>>> upstream-releases

  callInfo.setImplicitlyUsedUnchecked();

<<<<<<< HEAD
  auto* ins = MStringConvertCase::New(alloc(), callInfo.thisArg(), mode);
  current->add(ins);
  current->push(ins);
  return InliningStatus_Inlined;
}
||||||| merged common ancestors
    if (protoArg->type() != MIRType::Object) {
        return InliningStatus_NotInlined;
    }
=======
  MInstruction* index = MToNumberInt32::New(alloc(), callInfo.getArg(0));
  current->add(index);
>>>>>>> upstream-releases

<<<<<<< HEAD
IonBuilder::InliningResult IonBuilder::inlineRegExpMatcher(CallInfo& callInfo) {
  // This is called from Self-hosted JS, after testing each argument,
  // most of following tests should be passed.
||||||| merged common ancestors
    if (getInlineReturnType() != MIRType::Boolean) {
        return InliningStatus_NotInlined;
    }
=======
  MStringLength* length = MStringLength::New(alloc(), callInfo.thisArg());
  current->add(length);
>>>>>>> upstream-releases

<<<<<<< HEAD
  MOZ_ASSERT(!callInfo.constructing());
  MOZ_ASSERT(callInfo.argc() == 3);
||||||| merged common ancestors
    callInfo.setImplicitlyUsedUnchecked();
=======
  index = addBoundsCheck(index, length);
>>>>>>> upstream-releases

<<<<<<< HEAD
  MDefinition* rxArg = callInfo.getArg(0);
  MDefinition* strArg = callInfo.getArg(1);
  MDefinition* lastIndexArg = callInfo.getArg(2);
||||||| merged common ancestors
    MInstruction* opt = MRegExpInstanceOptimizable::New(alloc(), rxArg, protoArg);
    current->add(opt);
    current->push(opt);
=======
  // String.charAt(x) = String.fromCharCode(String.charCodeAt(x))
  MCharCodeAt* charCode = MCharCodeAt::New(alloc(), callInfo.thisArg(), index);
  current->add(charCode);
>>>>>>> upstream-releases

<<<<<<< HEAD
  if (rxArg->type() != MIRType::Object && rxArg->type() != MIRType::Value) {
    return InliningStatus_NotInlined;
  }
||||||| merged common ancestors
    return InliningStatus_Inlined;
}
=======
  MFromCharCode* string = MFromCharCode::New(alloc(), charCode);
  current->add(string);
  current->push(string);
  return InliningStatus_Inlined;
}
>>>>>>> upstream-releases

<<<<<<< HEAD
  TemporaryTypeSet* rxTypes = rxArg->resultTypeSet();
  const Class* clasp =
      rxTypes ? rxTypes->getKnownClass(constraints()) : nullptr;
  if (clasp != &RegExpObject::class_) {
    return InliningStatus_NotInlined;
  }
||||||| merged common ancestors
IonBuilder::InliningResult
IonBuilder::inlineGetFirstDollarIndex(CallInfo& callInfo)
{
    MOZ_ASSERT(!callInfo.constructing());
    MOZ_ASSERT(callInfo.argc() == 1);
=======
IonBuilder::InliningResult IonBuilder::inlineStringConvertCase(
    CallInfo& callInfo, MStringConvertCase::Mode mode) {
  if (callInfo.argc() != 0 || callInfo.constructing()) {
    trackOptimizationOutcome(TrackedOutcome::CantInlineNativeBadForm);
    return InliningStatus_NotInlined;
  }
>>>>>>> upstream-releases

<<<<<<< HEAD
  if (strArg->type() != MIRType::String && strArg->type() != MIRType::Value) {
    return InliningStatus_NotInlined;
  }
||||||| merged common ancestors
    MDefinition* strArg = callInfo.getArg(0);
=======
  if (getInlineReturnType() != MIRType::String) {
    return InliningStatus_NotInlined;
  }
  if (callInfo.thisArg()->type() != MIRType::String) {
    return InliningStatus_NotInlined;
  }
>>>>>>> upstream-releases

<<<<<<< HEAD
  if (lastIndexArg->type() != MIRType::Int32) {
    return InliningStatus_NotInlined;
  }
||||||| merged common ancestors
    if (strArg->type() != MIRType::String) {
        return InliningStatus_NotInlined;
    }
=======
  callInfo.setImplicitlyUsedUnchecked();
>>>>>>> upstream-releases

<<<<<<< HEAD
  JSContext* cx = TlsContext.get();
  if (!cx->realm()->jitRealm()->ensureRegExpMatcherStubExists(cx)) {
    cx->clearPendingException();  // OOM or overrecursion.
    return InliningStatus_NotInlined;
  }
||||||| merged common ancestors
    if (getInlineReturnType() != MIRType::Int32) {
        return InliningStatus_NotInlined;
    }
=======
  auto* ins = MStringConvertCase::New(alloc(), callInfo.thisArg(), mode);
  current->add(ins);
  current->push(ins);
  return InliningStatus_Inlined;
}
>>>>>>> upstream-releases

<<<<<<< HEAD
  callInfo.setImplicitlyUsedUnchecked();
||||||| merged common ancestors
    callInfo.setImplicitlyUsedUnchecked();
=======
IonBuilder::InliningResult IonBuilder::inlineRegExpMatcher(CallInfo& callInfo) {
  // This is called from Self-hosted JS, after testing each argument,
  // most of following tests should be passed.
>>>>>>> upstream-releases

<<<<<<< HEAD
  MInstruction* matcher =
      MRegExpMatcher::New(alloc(), rxArg, strArg, lastIndexArg);
  current->add(matcher);
  current->push(matcher);
||||||| merged common ancestors
    MInstruction* ins = MGetFirstDollarIndex::New(alloc(), strArg);
    current->add(ins);
    current->push(ins);
=======
  MOZ_ASSERT(!callInfo.constructing());
  MOZ_ASSERT(callInfo.argc() == 3);
>>>>>>> upstream-releases

<<<<<<< HEAD
  MOZ_TRY(resumeAfter(matcher));
  MOZ_TRY(
      pushTypeBarrier(matcher, getInlineReturnTypeSet(), BarrierKind::TypeSet));
  return InliningStatus_Inlined;
}
||||||| merged common ancestors
    return InliningStatus_Inlined;
}
=======
  MDefinition* rxArg = callInfo.getArg(0);
  MDefinition* strArg = callInfo.getArg(1);
  MDefinition* lastIndexArg = callInfo.getArg(2);
>>>>>>> upstream-releases

<<<<<<< HEAD
IonBuilder::InliningResult IonBuilder::inlineRegExpSearcher(
    CallInfo& callInfo) {
  // This is called from Self-hosted JS, after testing each argument,
  // most of following tests should be passed.
||||||| merged common ancestors
IonBuilder::InliningResult
IonBuilder::inlineStringReplaceString(CallInfo& callInfo)
{
    MOZ_ASSERT(!callInfo.constructing());
    MOZ_ASSERT(callInfo.argc() == 3);
=======
  if (rxArg->type() != MIRType::Object && rxArg->type() != MIRType::Value) {
    return InliningStatus_NotInlined;
  }
>>>>>>> upstream-releases

<<<<<<< HEAD
  MOZ_ASSERT(!callInfo.constructing());
  MOZ_ASSERT(callInfo.argc() == 3);
||||||| merged common ancestors
    if (getInlineReturnType() != MIRType::String) {
        return InliningStatus_NotInlined;
    }
=======
  TemporaryTypeSet* rxTypes = rxArg->resultTypeSet();
  const Class* clasp =
      rxTypes ? rxTypes->getKnownClass(constraints()) : nullptr;
  if (clasp != &RegExpObject::class_) {
    return InliningStatus_NotInlined;
  }
>>>>>>> upstream-releases

<<<<<<< HEAD
  MDefinition* rxArg = callInfo.getArg(0);
  MDefinition* strArg = callInfo.getArg(1);
  MDefinition* lastIndexArg = callInfo.getArg(2);
||||||| merged common ancestors
    MDefinition* strArg = callInfo.getArg(0);
    MDefinition* patArg = callInfo.getArg(1);
    MDefinition* replArg = callInfo.getArg(2);
=======
  if (strArg->type() != MIRType::String && strArg->type() != MIRType::Value) {
    return InliningStatus_NotInlined;
  }
>>>>>>> upstream-releases

<<<<<<< HEAD
  if (rxArg->type() != MIRType::Object && rxArg->type() != MIRType::Value) {
    return InliningStatus_NotInlined;
  }
||||||| merged common ancestors
    if (strArg->type() != MIRType::String) {
        return InliningStatus_NotInlined;
    }
=======
  if (lastIndexArg->type() != MIRType::Int32) {
    return InliningStatus_NotInlined;
  }
>>>>>>> upstream-releases

<<<<<<< HEAD
  TemporaryTypeSet* regexpTypes = rxArg->resultTypeSet();
  const Class* clasp =
      regexpTypes ? regexpTypes->getKnownClass(constraints()) : nullptr;
  if (clasp != &RegExpObject::class_) {
    return InliningStatus_NotInlined;
  }
||||||| merged common ancestors
    if (patArg->type() != MIRType::String) {
        return InliningStatus_NotInlined;
    }
=======
  JSContext* cx = TlsContext.get();
  if (!cx->realm()->jitRealm()->ensureRegExpMatcherStubExists(cx)) {
    cx->clearPendingException();  // OOM or overrecursion.
    return InliningStatus_NotInlined;
  }
>>>>>>> upstream-releases

<<<<<<< HEAD
  if (strArg->type() != MIRType::String && strArg->type() != MIRType::Value) {
    return InliningStatus_NotInlined;
  }
||||||| merged common ancestors
    if (replArg->type() != MIRType::String) {
        return InliningStatus_NotInlined;
    }
=======
  callInfo.setImplicitlyUsedUnchecked();
>>>>>>> upstream-releases

<<<<<<< HEAD
  if (lastIndexArg->type() != MIRType::Int32) {
    return InliningStatus_NotInlined;
  }
||||||| merged common ancestors
    callInfo.setImplicitlyUsedUnchecked();
=======
  MInstruction* matcher =
      MRegExpMatcher::New(alloc(), rxArg, strArg, lastIndexArg);
  current->add(matcher);
  current->push(matcher);
>>>>>>> upstream-releases

<<<<<<< HEAD
  JSContext* cx = TlsContext.get();
  if (!cx->realm()->jitRealm()->ensureRegExpSearcherStubExists(cx)) {
    cx->clearPendingException();  // OOM or overrecursion.
    return InliningStatus_NotInlined;
  }
||||||| merged common ancestors
    MInstruction* cte = MStringReplace::New(alloc(), strArg, patArg, replArg);
    current->add(cte);
    current->push(cte);
    if (cte->isEffectful()) {
        MOZ_TRY(resumeAfter(cte));
    }
    return InliningStatus_Inlined;
}
=======
  MOZ_TRY(resumeAfter(matcher));
  MOZ_TRY(
      pushTypeBarrier(matcher, getInlineReturnTypeSet(), BarrierKind::TypeSet));
  return InliningStatus_Inlined;
}
>>>>>>> upstream-releases

<<<<<<< HEAD
  callInfo.setImplicitlyUsedUnchecked();
||||||| merged common ancestors
IonBuilder::InliningResult
IonBuilder::inlineSubstringKernel(CallInfo& callInfo)
{
    MOZ_ASSERT(!callInfo.constructing());
    MOZ_ASSERT(callInfo.argc() == 3);
=======
IonBuilder::InliningResult IonBuilder::inlineRegExpSearcher(
    CallInfo& callInfo) {
  // This is called from Self-hosted JS, after testing each argument,
  // most of following tests should be passed.
>>>>>>> upstream-releases

<<<<<<< HEAD
  MInstruction* searcher =
      MRegExpSearcher::New(alloc(), rxArg, strArg, lastIndexArg);
  current->add(searcher);
  current->push(searcher);
||||||| merged common ancestors
    // Return: String.
    if (getInlineReturnType() != MIRType::String) {
        return InliningStatus_NotInlined;
    }
=======
  MOZ_ASSERT(!callInfo.constructing());
  MOZ_ASSERT(callInfo.argc() == 3);
>>>>>>> upstream-releases

<<<<<<< HEAD
  MOZ_TRY(resumeAfter(searcher));
  MOZ_TRY(pushTypeBarrier(searcher, getInlineReturnTypeSet(),
                          BarrierKind::TypeSet));
  return InliningStatus_Inlined;
}
||||||| merged common ancestors
    // Arg 0: String.
    if (callInfo.getArg(0)->type() != MIRType::String) {
        return InliningStatus_NotInlined;
    }
=======
  MDefinition* rxArg = callInfo.getArg(0);
  MDefinition* strArg = callInfo.getArg(1);
  MDefinition* lastIndexArg = callInfo.getArg(2);
>>>>>>> upstream-releases

<<<<<<< HEAD
IonBuilder::InliningResult IonBuilder::inlineRegExpTester(CallInfo& callInfo) {
  // This is called from Self-hosted JS, after testing each argument,
  // most of following tests should be passed.
||||||| merged common ancestors
    // Arg 1: Int.
    if (callInfo.getArg(1)->type() != MIRType::Int32) {
        return InliningStatus_NotInlined;
    }
=======
  if (rxArg->type() != MIRType::Object && rxArg->type() != MIRType::Value) {
    return InliningStatus_NotInlined;
  }
>>>>>>> upstream-releases

<<<<<<< HEAD
  MOZ_ASSERT(!callInfo.constructing());
  MOZ_ASSERT(callInfo.argc() == 3);
||||||| merged common ancestors
    // Arg 2: Int.
    if (callInfo.getArg(2)->type() != MIRType::Int32) {
        return InliningStatus_NotInlined;
    }
=======
  TemporaryTypeSet* regexpTypes = rxArg->resultTypeSet();
  const Class* clasp =
      regexpTypes ? regexpTypes->getKnownClass(constraints()) : nullptr;
  if (clasp != &RegExpObject::class_) {
    return InliningStatus_NotInlined;
  }
>>>>>>> upstream-releases

<<<<<<< HEAD
  MDefinition* rxArg = callInfo.getArg(0);
  MDefinition* strArg = callInfo.getArg(1);
  MDefinition* lastIndexArg = callInfo.getArg(2);
||||||| merged common ancestors
    callInfo.setImplicitlyUsedUnchecked();
=======
  if (strArg->type() != MIRType::String && strArg->type() != MIRType::Value) {
    return InliningStatus_NotInlined;
  }
>>>>>>> upstream-releases

<<<<<<< HEAD
  if (rxArg->type() != MIRType::Object && rxArg->type() != MIRType::Value) {
    return InliningStatus_NotInlined;
  }
||||||| merged common ancestors
    MSubstr* substr = MSubstr::New(alloc(), callInfo.getArg(0), callInfo.getArg(1),
                                            callInfo.getArg(2));
    current->add(substr);
    current->push(substr);
=======
  if (lastIndexArg->type() != MIRType::Int32) {
    return InliningStatus_NotInlined;
  }
>>>>>>> upstream-releases

<<<<<<< HEAD
  TemporaryTypeSet* rxTypes = rxArg->resultTypeSet();
  const Class* clasp =
      rxTypes ? rxTypes->getKnownClass(constraints()) : nullptr;
  if (clasp != &RegExpObject::class_) {
    return InliningStatus_NotInlined;
  }
||||||| merged common ancestors
    return InliningStatus_Inlined;
}
=======
  JSContext* cx = TlsContext.get();
  if (!cx->realm()->jitRealm()->ensureRegExpSearcherStubExists(cx)) {
    cx->clearPendingException();  // OOM or overrecursion.
    return InliningStatus_NotInlined;
  }
>>>>>>> upstream-releases

<<<<<<< HEAD
  if (strArg->type() != MIRType::String && strArg->type() != MIRType::Value) {
    return InliningStatus_NotInlined;
  }
||||||| merged common ancestors
IonBuilder::InliningResult
IonBuilder::inlineObject(CallInfo& callInfo)
{
    if (callInfo.argc() != 1 || callInfo.constructing()) {
        return InliningStatus_NotInlined;
    }
=======
  callInfo.setImplicitlyUsedUnchecked();
>>>>>>> upstream-releases

<<<<<<< HEAD
  if (lastIndexArg->type() != MIRType::Int32) {
    return InliningStatus_NotInlined;
  }
||||||| merged common ancestors
    if (getInlineReturnType() != MIRType::Object) {
        return InliningStatus_NotInlined;
    }
=======
  MInstruction* searcher =
      MRegExpSearcher::New(alloc(), rxArg, strArg, lastIndexArg);
  current->add(searcher);
  current->push(searcher);
>>>>>>> upstream-releases

<<<<<<< HEAD
  JSContext* cx = TlsContext.get();
  if (!cx->realm()->jitRealm()->ensureRegExpTesterStubExists(cx)) {
    cx->clearPendingException();  // OOM or overrecursion.
    return InliningStatus_NotInlined;
  }
||||||| merged common ancestors
    MDefinition* arg = callInfo.getArg(0);
    if (arg->type() != MIRType::Object) {
        return InliningStatus_NotInlined;
    }
=======
  MOZ_TRY(resumeAfter(searcher));
  MOZ_TRY(pushTypeBarrier(searcher, getInlineReturnTypeSet(),
                          BarrierKind::TypeSet));
  return InliningStatus_Inlined;
}
>>>>>>> upstream-releases

<<<<<<< HEAD
  callInfo.setImplicitlyUsedUnchecked();
||||||| merged common ancestors
    callInfo.setImplicitlyUsedUnchecked();
    current->push(arg);
=======
IonBuilder::InliningResult IonBuilder::inlineRegExpTester(CallInfo& callInfo) {
  // This is called from Self-hosted JS, after testing each argument,
  // most of following tests should be passed.
>>>>>>> upstream-releases

<<<<<<< HEAD
  MInstruction* tester =
      MRegExpTester::New(alloc(), rxArg, strArg, lastIndexArg);
  current->add(tester);
  current->push(tester);

  MOZ_TRY(resumeAfter(tester));
  return InliningStatus_Inlined;
}
||||||| merged common ancestors
    return InliningStatus_Inlined;
}
=======
  MOZ_ASSERT(!callInfo.constructing());
  MOZ_ASSERT(callInfo.argc() == 3);
>>>>>>> upstream-releases

<<<<<<< HEAD
IonBuilder::InliningResult IonBuilder::inlineIsRegExpObject(
    CallInfo& callInfo) {
  MOZ_ASSERT(!callInfo.constructing());
  MOZ_ASSERT(callInfo.argc() == 1);
||||||| merged common ancestors
IonBuilder::InliningResult
IonBuilder::inlineObjectCreate(CallInfo& callInfo)
{
    if (callInfo.argc() != 1 || callInfo.constructing()) {
        return InliningStatus_NotInlined;
    }
=======
  MDefinition* rxArg = callInfo.getArg(0);
  MDefinition* strArg = callInfo.getArg(1);
  MDefinition* lastIndexArg = callInfo.getArg(2);
>>>>>>> upstream-releases

<<<<<<< HEAD
  if (getInlineReturnType() != MIRType::Boolean) {
    return InliningStatus_NotInlined;
  }
||||||| merged common ancestors
    JSObject* templateObject = inspector->getTemplateObjectForNative(pc, obj_create);
    if (!templateObject) {
        return InliningStatus_NotInlined;
    }
=======
  if (rxArg->type() != MIRType::Object && rxArg->type() != MIRType::Value) {
    return InliningStatus_NotInlined;
  }
>>>>>>> upstream-releases

<<<<<<< HEAD
  MDefinition* arg = callInfo.getArg(0);
||||||| merged common ancestors
    MOZ_ASSERT(templateObject->is<PlainObject>());
    MOZ_ASSERT(!templateObject->isSingleton());
=======
  TemporaryTypeSet* rxTypes = rxArg->resultTypeSet();
  const Class* clasp =
      rxTypes ? rxTypes->getKnownClass(constraints()) : nullptr;
  if (clasp != &RegExpObject::class_) {
    return InliningStatus_NotInlined;
  }
>>>>>>> upstream-releases

<<<<<<< HEAD
  bool isRegExpObjectKnown = false;
  bool isRegExpObjectConstant;
  if (arg->type() == MIRType::Object) {
    TemporaryTypeSet* types = arg->resultTypeSet();
    const Class* clasp = types ? types->getKnownClass(constraints()) : nullptr;
    if (clasp) {
      isRegExpObjectKnown = true;
      isRegExpObjectConstant = (clasp == &RegExpObject::class_);
    }
  } else if (!arg->mightBeType(MIRType::Object)) {
    // NB: This case only happens when Phi-nodes flow into IsRegExpObject.
    // IsRegExpObject itself will never be called with a non-Object value.
    isRegExpObjectKnown = true;
    isRegExpObjectConstant = false;
  } else if (arg->type() != MIRType::Value) {
    return InliningStatus_NotInlined;
  }
||||||| merged common ancestors
    // Ensure the argument matches the template object's prototype.
    MDefinition* arg = callInfo.getArg(0);
    if (JSObject* proto = templateObject->staticPrototype()) {
        if (IsInsideNursery(proto)) {
            return InliningStatus_NotInlined;
        }
=======
  if (strArg->type() != MIRType::String && strArg->type() != MIRType::Value) {
    return InliningStatus_NotInlined;
  }
>>>>>>> upstream-releases

<<<<<<< HEAD
  if (isRegExpObjectKnown) {
    pushConstant(BooleanValue(isRegExpObjectConstant));
  } else {
    MHasClass* hasClass = MHasClass::New(alloc(), arg, &RegExpObject::class_);
    current->add(hasClass);
    current->push(hasClass);
  }
||||||| merged common ancestors
        TemporaryTypeSet* types = arg->resultTypeSet();
        if (!types || types->maybeSingleton() != proto) {
            return InliningStatus_NotInlined;
        }
=======
  if (lastIndexArg->type() != MIRType::Int32) {
    return InliningStatus_NotInlined;
  }
>>>>>>> upstream-releases

<<<<<<< HEAD
  callInfo.setImplicitlyUsedUnchecked();
  return InliningStatus_Inlined;
}
||||||| merged common ancestors
        MOZ_ASSERT(types->getKnownMIRType() == MIRType::Object);
    } else {
        if (arg->type() != MIRType::Null) {
            return InliningStatus_NotInlined;
        }
    }
=======
  JSContext* cx = TlsContext.get();
  if (!cx->realm()->jitRealm()->ensureRegExpTesterStubExists(cx)) {
    cx->clearPendingException();  // OOM or overrecursion.
    return InliningStatus_NotInlined;
  }
>>>>>>> upstream-releases

<<<<<<< HEAD
IonBuilder::InliningResult IonBuilder::inlineRegExpPrototypeOptimizable(
    CallInfo& callInfo) {
  MOZ_ASSERT(!callInfo.constructing());
  MOZ_ASSERT(callInfo.argc() == 1);
||||||| merged common ancestors
    callInfo.setImplicitlyUsedUnchecked();
=======
  callInfo.setImplicitlyUsedUnchecked();
>>>>>>> upstream-releases

<<<<<<< HEAD
  MDefinition* protoArg = callInfo.getArg(0);
||||||| merged common ancestors
    bool emitted = false;
    MOZ_TRY(newObjectTryTemplateObject(&emitted, templateObject));
=======
  MInstruction* tester =
      MRegExpTester::New(alloc(), rxArg, strArg, lastIndexArg);
  current->add(tester);
  current->push(tester);
>>>>>>> upstream-releases

<<<<<<< HEAD
  if (protoArg->type() != MIRType::Object) {
    return InliningStatus_NotInlined;
  }

  if (getInlineReturnType() != MIRType::Boolean) {
    return InliningStatus_NotInlined;
  }

  callInfo.setImplicitlyUsedUnchecked();

  MInstruction* opt = MRegExpPrototypeOptimizable::New(alloc(), protoArg);
  current->add(opt);
  current->push(opt);

  return InliningStatus_Inlined;
||||||| merged common ancestors
    MOZ_ASSERT(emitted);
    return InliningStatus_Inlined;
=======
  MOZ_TRY(resumeAfter(tester));
  return InliningStatus_Inlined;
>>>>>>> upstream-releases
}

<<<<<<< HEAD
IonBuilder::InliningResult IonBuilder::inlineRegExpInstanceOptimizable(
    CallInfo& callInfo) {
  MOZ_ASSERT(!callInfo.constructing());
  MOZ_ASSERT(callInfo.argc() == 2);
||||||| merged common ancestors
IonBuilder::InliningResult
IonBuilder::inlineObjectIs(CallInfo& callInfo)
{
    if (callInfo.argc() < 2 || callInfo.constructing()) {
        return InliningStatus_NotInlined;
    }
=======
IonBuilder::InliningResult IonBuilder::inlineIsRegExpObject(
    CallInfo& callInfo) {
  MOZ_ASSERT(!callInfo.constructing());
  MOZ_ASSERT(callInfo.argc() == 1);
>>>>>>> upstream-releases

<<<<<<< HEAD
  MDefinition* rxArg = callInfo.getArg(0);
  MDefinition* protoArg = callInfo.getArg(1);
||||||| merged common ancestors
    if (getInlineReturnType() != MIRType::Boolean) {
        return InliningStatus_NotInlined;
    }
=======
  if (getInlineReturnType() != MIRType::Boolean) {
    return InliningStatus_NotInlined;
  }
>>>>>>> upstream-releases

<<<<<<< HEAD
  if (rxArg->type() != MIRType::Object && rxArg->type() != MIRType::Value) {
    return InliningStatus_NotInlined;
  }
||||||| merged common ancestors
    MDefinition* left = callInfo.getArg(0);
    MDefinition* right = callInfo.getArg(1);
    MIRType leftType = left->type();
    MIRType rightType = right->type();

    auto mightBeFloatingPointType = [](MDefinition* def) {
        return def->mightBeType(MIRType::Double) || def->mightBeType(MIRType::Float32);
    };

    bool strictEq;
    bool incompatibleTypes = false;
    if (leftType == rightType) {
        // We can only compare the arguments with strict-equals semantics if
        // they aren't floating-point types or values which might be floating-
        // point types. Otherwise we need to use MSameValue.
        strictEq = leftType != MIRType::Value
                   ? !IsFloatingPointType(leftType)
                   : (!mightBeFloatingPointType(left) && !mightBeFloatingPointType(right));
    } else if (leftType == MIRType::Value) {
        // Also use strict-equals when comparing a value with a non-number or
        // the value cannot be a floating-point type.
        strictEq = !IsNumberType(rightType) || !mightBeFloatingPointType(left);
    } else if (rightType == MIRType::Value) {
        // Dual case to the previous one, only with reversed operands.
        strictEq = !IsNumberType(leftType) || !mightBeFloatingPointType(right);
    } else if (IsNumberType(leftType) && IsNumberType(rightType)) {
        // Both arguments are numbers, but with different representations. We
        // can't use strict-equals semantics to compare the operands, but
        // instead need to use MSameValue.
        strictEq = false;
    } else {
        incompatibleTypes = true;
    }
=======
  MDefinition* arg = callInfo.getArg(0);

  bool isRegExpObjectKnown = false;
  bool isRegExpObjectConstant;
  if (arg->type() == MIRType::Object) {
    TemporaryTypeSet* types = arg->resultTypeSet();
    const Class* clasp = types ? types->getKnownClass(constraints()) : nullptr;
    if (clasp) {
      isRegExpObjectKnown = true;
      isRegExpObjectConstant = (clasp == &RegExpObject::class_);
    }
  } else if (!arg->mightBeType(MIRType::Object)) {
    // NB: This case only happens when Phi-nodes flow into IsRegExpObject.
    // IsRegExpObject itself will never be called with a non-Object value.
    isRegExpObjectKnown = true;
    isRegExpObjectConstant = false;
  } else if (arg->type() != MIRType::Value) {
    return InliningStatus_NotInlined;
  }

  if (isRegExpObjectKnown) {
    pushConstant(BooleanValue(isRegExpObjectConstant));
  } else {
    MHasClass* hasClass = MHasClass::New(alloc(), arg, &RegExpObject::class_);
    current->add(hasClass);
    current->push(hasClass);
  }

  callInfo.setImplicitlyUsedUnchecked();
  return InliningStatus_Inlined;
}

IonBuilder::InliningResult IonBuilder::inlineRegExpPrototypeOptimizable(
    CallInfo& callInfo) {
  MOZ_ASSERT(!callInfo.constructing());
  MOZ_ASSERT(callInfo.argc() == 1);
>>>>>>> upstream-releases

<<<<<<< HEAD
  if (protoArg->type() != MIRType::Object) {
    return InliningStatus_NotInlined;
  }
||||||| merged common ancestors
    if (incompatibleTypes) {
        // The result is always |false| when comparing incompatible types.
        pushConstant(BooleanValue(false));
    } else if (strictEq) {
        // Specialize |Object.is(lhs, rhs)| as |lhs === rhs|.
        MOZ_TRY(jsop_compare(JSOP_STRICTEQ, left, right));
    } else {
        MSameValue* ins = MSameValue::New(alloc(), left, right);
=======
  MDefinition* protoArg = callInfo.getArg(0);
>>>>>>> upstream-releases

<<<<<<< HEAD
  if (getInlineReturnType() != MIRType::Boolean) {
    return InliningStatus_NotInlined;
  }
||||||| merged common ancestors
        // The more specific operand is expected to be in the rhs.
        if (IsNumberType(leftType) && rightType == MIRType::Value) {
            ins->swapOperands();
        }
=======
  if (protoArg->type() != MIRType::Object) {
    return InliningStatus_NotInlined;
  }
>>>>>>> upstream-releases

<<<<<<< HEAD
  callInfo.setImplicitlyUsedUnchecked();
||||||| merged common ancestors
        current->add(ins);
        current->push(ins);
    }
=======
  if (getInlineReturnType() != MIRType::Boolean) {
    return InliningStatus_NotInlined;
  }
>>>>>>> upstream-releases

<<<<<<< HEAD
  MInstruction* opt = MRegExpInstanceOptimizable::New(alloc(), rxArg, protoArg);
  current->add(opt);
  current->push(opt);

  return InliningStatus_Inlined;
}
||||||| merged common ancestors
    callInfo.setImplicitlyUsedUnchecked();
    return InliningStatus_Inlined;
}
=======
  callInfo.setImplicitlyUsedUnchecked();
>>>>>>> upstream-releases

<<<<<<< HEAD
IonBuilder::InliningResult IonBuilder::inlineGetFirstDollarIndex(
    CallInfo& callInfo) {
  MOZ_ASSERT(!callInfo.constructing());
  MOZ_ASSERT(callInfo.argc() == 1);
||||||| merged common ancestors
IonBuilder::InliningResult
IonBuilder::inlineObjectToString(CallInfo& callInfo)
{
    if (callInfo.constructing() || callInfo.argc() != 0) {
        trackOptimizationOutcome(TrackedOutcome::CantInlineNativeBadForm);
        return InliningStatus_NotInlined;
    }
=======
  MInstruction* opt = MRegExpPrototypeOptimizable::New(alloc(), protoArg);
  current->add(opt);
  current->push(opt);
>>>>>>> upstream-releases

<<<<<<< HEAD
  MDefinition* strArg = callInfo.getArg(0);
||||||| merged common ancestors
    if (getInlineReturnType() != MIRType::String) {
        return InliningStatus_NotInlined;
    }
=======
  return InliningStatus_Inlined;
}
>>>>>>> upstream-releases

<<<<<<< HEAD
  if (strArg->type() != MIRType::String) {
    return InliningStatus_NotInlined;
  }
||||||| merged common ancestors
    MDefinition* arg = callInfo.thisArg();
    if (arg->type() != MIRType::Object) {
        return InliningStatus_NotInlined;
    }
=======
IonBuilder::InliningResult IonBuilder::inlineRegExpInstanceOptimizable(
    CallInfo& callInfo) {
  MOZ_ASSERT(!callInfo.constructing());
  MOZ_ASSERT(callInfo.argc() == 2);
>>>>>>> upstream-releases

<<<<<<< HEAD
  if (getInlineReturnType() != MIRType::Int32) {
    return InliningStatus_NotInlined;
  }
||||||| merged common ancestors
    TemporaryTypeSet* types = arg->resultTypeSet();
    if (!types || types->unknownObject()) {
        return InliningStatus_NotInlined;
    }
=======
  MDefinition* rxArg = callInfo.getArg(0);
  MDefinition* protoArg = callInfo.getArg(1);
>>>>>>> upstream-releases

<<<<<<< HEAD
  callInfo.setImplicitlyUsedUnchecked();
||||||| merged common ancestors
    // Don't optimize if this might be a proxy.
    using ForAllResult = TemporaryTypeSet::ForAllResult;
    if (types->forAllClasses(constraints(), IsProxyClass) != ForAllResult::ALL_FALSE) {
        return InliningStatus_NotInlined;
    }
=======
  if (rxArg->type() != MIRType::Object && rxArg->type() != MIRType::Value) {
    return InliningStatus_NotInlined;
  }
>>>>>>> upstream-releases

<<<<<<< HEAD
  MInstruction* ins = MGetFirstDollarIndex::New(alloc(), strArg);
  current->add(ins);
  current->push(ins);
||||||| merged common ancestors
    // Make sure there's no Symbol.toStringTag property.
    jsid toStringTag = SYMBOL_TO_JSID(realm->runtime()->wellKnownSymbols().toStringTag);
    bool res;
    MOZ_TRY_VAR(res, testNotDefinedProperty(arg, toStringTag));
    if (!res) {
        return InliningStatus_NotInlined;
    }
=======
  if (protoArg->type() != MIRType::Object) {
    return InliningStatus_NotInlined;
  }
>>>>>>> upstream-releases

<<<<<<< HEAD
  return InliningStatus_Inlined;
}
||||||| merged common ancestors
    // At this point we know we're going to inline this.
    callInfo.setImplicitlyUsedUnchecked();
=======
  if (getInlineReturnType() != MIRType::Boolean) {
    return InliningStatus_NotInlined;
  }
>>>>>>> upstream-releases

<<<<<<< HEAD
IonBuilder::InliningResult IonBuilder::inlineStringReplaceString(
    CallInfo& callInfo) {
  MOZ_ASSERT(!callInfo.constructing());
  MOZ_ASSERT(callInfo.argc() == 3);
||||||| merged common ancestors
    // Try to constant fold some common cases.
    if (const Class* knownClass = types->getKnownClass(constraints())) {
        if (knownClass == &PlainObject::class_ || knownClass == &UnboxedPlainObject::class_) {
            pushConstant(StringValue(names().objectObject));
            return InliningStatus_Inlined;
        }
        if (IsArrayClass(knownClass)) {
            pushConstant(StringValue(names().objectArray));
            return InliningStatus_Inlined;
        }
        if (knownClass == &JSFunction::class_) {
            pushConstant(StringValue(names().objectFunction));
            return InliningStatus_Inlined;
        }
    }
=======
  callInfo.setImplicitlyUsedUnchecked();
>>>>>>> upstream-releases

<<<<<<< HEAD
  if (getInlineReturnType() != MIRType::String) {
    return InliningStatus_NotInlined;
  }
||||||| merged common ancestors
    MObjectClassToString* toString = MObjectClassToString::New(alloc(), arg);
    current->add(toString);
    current->push(toString);
=======
  MInstruction* opt = MRegExpInstanceOptimizable::New(alloc(), rxArg, protoArg);
  current->add(opt);
  current->push(opt);
>>>>>>> upstream-releases

<<<<<<< HEAD
  MDefinition* strArg = callInfo.getArg(0);
  MDefinition* patArg = callInfo.getArg(1);
  MDefinition* replArg = callInfo.getArg(2);
||||||| merged common ancestors
    return InliningStatus_Inlined;
}
=======
  return InliningStatus_Inlined;
}
>>>>>>> upstream-releases

<<<<<<< HEAD
  if (strArg->type() != MIRType::String) {
    return InliningStatus_NotInlined;
  }
||||||| merged common ancestors
IonBuilder::InliningResult
IonBuilder::inlineHasClass(CallInfo& callInfo,
                           const Class* clasp1, const Class* clasp2,
                           const Class* clasp3, const Class* clasp4)
{
    MOZ_ASSERT(!callInfo.constructing());
    MOZ_ASSERT(callInfo.argc() == 1);

    if (callInfo.getArg(0)->type() != MIRType::Object) {
        return InliningStatus_NotInlined;
    }
    if (getInlineReturnType() != MIRType::Boolean) {
        return InliningStatus_NotInlined;
    }
=======
IonBuilder::InliningResult IonBuilder::inlineGetFirstDollarIndex(
    CallInfo& callInfo) {
  MOZ_ASSERT(!callInfo.constructing());
  MOZ_ASSERT(callInfo.argc() == 1);
>>>>>>> upstream-releases

<<<<<<< HEAD
  if (patArg->type() != MIRType::String) {
    return InliningStatus_NotInlined;
  }
||||||| merged common ancestors
    TemporaryTypeSet* types = callInfo.getArg(0)->resultTypeSet();
    const Class* knownClass = types ? types->getKnownClass(constraints()) : nullptr;
    if (knownClass) {
        pushConstant(BooleanValue(knownClass == clasp1 ||
                                  knownClass == clasp2 ||
                                  knownClass == clasp3 ||
                                  knownClass == clasp4));
    } else {
        MHasClass* hasClass1 = MHasClass::New(alloc(), callInfo.getArg(0), clasp1);
        current->add(hasClass1);

        if (!clasp2 && !clasp3 && !clasp4) {
            current->push(hasClass1);
        } else {
            const Class* remaining[] = { clasp2, clasp3, clasp4 };
            MDefinition* last = hasClass1;
            for (size_t i = 0; i < ArrayLength(remaining); i++) {
                MHasClass* hasClass = MHasClass::New(alloc(), callInfo.getArg(0), remaining[i]);
                current->add(hasClass);
                MBitOr* either = MBitOr::New(alloc(), last, hasClass);
                either->infer(inspector, pc);
                current->add(either);
                last = either;
            }

            MDefinition* result = convertToBoolean(last);
            current->push(result);
        }
    }
=======
  MDefinition* strArg = callInfo.getArg(0);

  if (strArg->type() != MIRType::String) {
    return InliningStatus_NotInlined;
  }
>>>>>>> upstream-releases

<<<<<<< HEAD
  if (replArg->type() != MIRType::String) {
    return InliningStatus_NotInlined;
  }
||||||| merged common ancestors
    callInfo.setImplicitlyUsedUnchecked();
    return InliningStatus_Inlined;
}
=======
  if (getInlineReturnType() != MIRType::Int32) {
    return InliningStatus_NotInlined;
  }

  callInfo.setImplicitlyUsedUnchecked();

  MInstruction* ins = MGetFirstDollarIndex::New(alloc(), strArg);
  current->add(ins);
  current->push(ins);

  return InliningStatus_Inlined;
}
>>>>>>> upstream-releases

<<<<<<< HEAD
  callInfo.setImplicitlyUsedUnchecked();
||||||| merged common ancestors
IonBuilder::InliningResult
IonBuilder::inlineGuardToClass(CallInfo& callInfo, const Class* clasp)
{
    MOZ_ASSERT(!callInfo.constructing());
    MOZ_ASSERT(callInfo.argc() == 1);
=======
IonBuilder::InliningResult IonBuilder::inlineStringReplaceString(
    CallInfo& callInfo) {
  MOZ_ASSERT(!callInfo.constructing());
  MOZ_ASSERT(callInfo.argc() == 3);
>>>>>>> upstream-releases

<<<<<<< HEAD
  MInstruction* cte = MStringReplace::New(alloc(), strArg, patArg, replArg);
  current->add(cte);
  current->push(cte);
  if (cte->isEffectful()) {
    MOZ_TRY(resumeAfter(cte));
  }
  return InliningStatus_Inlined;
}
||||||| merged common ancestors
    if (callInfo.getArg(0)->type() != MIRType::Object) {
        return InliningStatus_NotInlined;
    }
=======
  if (getInlineReturnType() != MIRType::String) {
    return InliningStatus_NotInlined;
  }
>>>>>>> upstream-releases

<<<<<<< HEAD
IonBuilder::InliningResult IonBuilder::inlineSubstringKernel(
    CallInfo& callInfo) {
  MOZ_ASSERT(!callInfo.constructing());
  MOZ_ASSERT(callInfo.argc() == 3);
||||||| merged common ancestors
    if (getInlineReturnType() != MIRType::ObjectOrNull &&
        getInlineReturnType() != MIRType::Object)
    {
        return InliningStatus_NotInlined;
    }
=======
  MDefinition* strArg = callInfo.getArg(0);
  MDefinition* patArg = callInfo.getArg(1);
  MDefinition* replArg = callInfo.getArg(2);

  if (strArg->type() != MIRType::String) {
    return InliningStatus_NotInlined;
  }
>>>>>>> upstream-releases

<<<<<<< HEAD
  // Return: String.
  if (getInlineReturnType() != MIRType::String) {
    return InliningStatus_NotInlined;
  }
||||||| merged common ancestors
    TemporaryTypeSet* types = callInfo.getArg(0)->resultTypeSet();
    const Class* knownClass = types ? types->getKnownClass(constraints()) : nullptr;
=======
  if (patArg->type() != MIRType::String) {
    return InliningStatus_NotInlined;
  }
>>>>>>> upstream-releases

<<<<<<< HEAD
  // Arg 0: String.
  if (callInfo.getArg(0)->type() != MIRType::String) {
    return InliningStatus_NotInlined;
  }
||||||| merged common ancestors
    if (knownClass && knownClass == clasp) {
        current->push(callInfo.getArg(0));
    } else {
        MGuardToClass* guardToClass = MGuardToClass::New(alloc(), callInfo.getArg(0),
                                                         clasp, getInlineReturnType());
        current->add(guardToClass);
        current->push(guardToClass);
    }
=======
  if (replArg->type() != MIRType::String) {
    return InliningStatus_NotInlined;
  }
>>>>>>> upstream-releases

<<<<<<< HEAD
  // Arg 1: Int.
  if (callInfo.getArg(1)->type() != MIRType::Int32) {
    return InliningStatus_NotInlined;
  }
||||||| merged common ancestors
    callInfo.setImplicitlyUsedUnchecked();
    return InliningStatus_Inlined;
}
=======
  callInfo.setImplicitlyUsedUnchecked();

  MInstruction* cte = MStringReplace::New(alloc(), strArg, patArg, replArg);
  current->add(cte);
  current->push(cte);
  if (cte->isEffectful()) {
    MOZ_TRY(resumeAfter(cte));
  }
  return InliningStatus_Inlined;
}
>>>>>>> upstream-releases

<<<<<<< HEAD
  // Arg 2: Int.
  if (callInfo.getArg(2)->type() != MIRType::Int32) {
    return InliningStatus_NotInlined;
  }
||||||| merged common ancestors
IonBuilder::InliningResult
IonBuilder::inlineGetNextEntryForIterator(CallInfo& callInfo, MGetNextEntryForIterator::Mode mode)
{
    MOZ_ASSERT(!callInfo.constructing());
    MOZ_ASSERT(callInfo.argc() == 2);
=======
IonBuilder::InliningResult IonBuilder::inlineSubstringKernel(
    CallInfo& callInfo) {
  MOZ_ASSERT(!callInfo.constructing());
  MOZ_ASSERT(callInfo.argc() == 3);
>>>>>>> upstream-releases

<<<<<<< HEAD
  callInfo.setImplicitlyUsedUnchecked();
||||||| merged common ancestors
    MDefinition* iterArg = callInfo.getArg(0);
    MDefinition* resultArg = callInfo.getArg(1);
=======
  // Return: String.
  if (getInlineReturnType() != MIRType::String) {
    return InliningStatus_NotInlined;
  }
>>>>>>> upstream-releases

<<<<<<< HEAD
  MSubstr* substr = MSubstr::New(alloc(), callInfo.getArg(0),
                                 callInfo.getArg(1), callInfo.getArg(2));
  current->add(substr);
  current->push(substr);
||||||| merged common ancestors
    // Self-hosted code has already validated |iterArg| is a (possibly boxed)
    // Map- or SetIterator object.
    if (iterArg->type() != MIRType::Object) {
        return InliningStatus_NotInlined;
    }
=======
  // Arg 0: String.
  if (callInfo.getArg(0)->type() != MIRType::String) {
    return InliningStatus_NotInlined;
  }
>>>>>>> upstream-releases

<<<<<<< HEAD
  return InliningStatus_Inlined;
}
||||||| merged common ancestors
    if (resultArg->type() != MIRType::Object) {
        return InliningStatus_NotInlined;
    }
=======
  // Arg 1: Int.
  if (callInfo.getArg(1)->type() != MIRType::Int32) {
    return InliningStatus_NotInlined;
  }
>>>>>>> upstream-releases

<<<<<<< HEAD
IonBuilder::InliningResult IonBuilder::inlineObject(CallInfo& callInfo) {
  if (callInfo.argc() != 1 || callInfo.constructing()) {
    return InliningStatus_NotInlined;
  }
||||||| merged common ancestors
    TemporaryTypeSet* resultTypes = resultArg->resultTypeSet();
    const Class* resultClasp = resultTypes ? resultTypes->getKnownClass(constraints()) : nullptr;
    if (resultClasp != &ArrayObject::class_) {
        return InliningStatus_NotInlined;
    }
=======
  // Arg 2: Int.
  if (callInfo.getArg(2)->type() != MIRType::Int32) {
    return InliningStatus_NotInlined;
  }
>>>>>>> upstream-releases

<<<<<<< HEAD
  if (getInlineReturnType() != MIRType::Object) {
    return InliningStatus_NotInlined;
  }
||||||| merged common ancestors
    callInfo.setImplicitlyUsedUnchecked();
=======
  callInfo.setImplicitlyUsedUnchecked();
>>>>>>> upstream-releases

<<<<<<< HEAD
  MDefinition* arg = callInfo.getArg(0);
  if (arg->type() != MIRType::Object) {
    return InliningStatus_NotInlined;
  }
||||||| merged common ancestors
    MInstruction* next = MGetNextEntryForIterator::New(alloc(), iterArg, resultArg, mode);
    current->add(next);
    current->push(next);
=======
  MSubstr* substr = MSubstr::New(alloc(), callInfo.getArg(0),
                                 callInfo.getArg(1), callInfo.getArg(2));
  current->add(substr);
  current->push(substr);
>>>>>>> upstream-releases

<<<<<<< HEAD
  callInfo.setImplicitlyUsedUnchecked();
  current->push(arg);

  return InliningStatus_Inlined;
||||||| merged common ancestors
    MOZ_TRY(resumeAfter(next));
    return InliningStatus_Inlined;
=======
  return InliningStatus_Inlined;
>>>>>>> upstream-releases
}

<<<<<<< HEAD
IonBuilder::InliningResult IonBuilder::inlineObjectCreate(CallInfo& callInfo) {
  if (callInfo.argc() != 1 || callInfo.constructing()) {
    return InliningStatus_NotInlined;
  }
||||||| merged common ancestors
static bool
IsArrayBufferObject(CompilerConstraintList* constraints, MDefinition* def)
{
    MOZ_ASSERT(def->type() == MIRType::Object);
=======
IonBuilder::InliningResult IonBuilder::inlineObject(CallInfo& callInfo) {
  if (callInfo.argc() != 1 || callInfo.constructing()) {
    return InliningStatus_NotInlined;
  }
>>>>>>> upstream-releases

<<<<<<< HEAD
  JSObject* templateObject =
      inspector->getTemplateObjectForNative(pc, obj_create);
  if (!templateObject) {
    return InliningStatus_NotInlined;
  }
||||||| merged common ancestors
    TemporaryTypeSet* types = def->resultTypeSet();
    if (!types) {
        return false;
    }
=======
  if (getInlineReturnType() != MIRType::Object) {
    return InliningStatus_NotInlined;
  }
>>>>>>> upstream-releases

<<<<<<< HEAD
  MOZ_ASSERT(templateObject->is<PlainObject>());
  MOZ_ASSERT(!templateObject->isSingleton());
||||||| merged common ancestors
    return types->getKnownClass(constraints) == &ArrayBufferObject::class_;
}
=======
  MDefinition* arg = callInfo.getArg(0);
  if (arg->type() != MIRType::Object) {
    return InliningStatus_NotInlined;
  }
>>>>>>> upstream-releases

<<<<<<< HEAD
  // Ensure the argument matches the template object's prototype.
  MDefinition* arg = callInfo.getArg(0);
  if (JSObject* proto = templateObject->staticPrototype()) {
    if (IsInsideNursery(proto)) {
      return InliningStatus_NotInlined;
    }
||||||| merged common ancestors
IonBuilder::InliningResult
IonBuilder::inlineArrayBufferByteLength(CallInfo& callInfo)
{
    MOZ_ASSERT(!callInfo.constructing());
    MOZ_ASSERT(callInfo.argc() == 1);
=======
  callInfo.setImplicitlyUsedUnchecked();
  current->push(arg);
>>>>>>> upstream-releases

<<<<<<< HEAD
    TemporaryTypeSet* types = arg->resultTypeSet();
    if (!types || types->maybeSingleton() != proto) {
      return InliningStatus_NotInlined;
    }

    MOZ_ASSERT(types->getKnownMIRType() == MIRType::Object);
  } else {
    if (arg->type() != MIRType::Null) {
      return InliningStatus_NotInlined;
    }
  }
||||||| merged common ancestors
    MDefinition* objArg = callInfo.getArg(0);
    if (objArg->type() != MIRType::Object) {
        return InliningStatus_NotInlined;
    }
    if (getInlineReturnType() != MIRType::Int32) {
        return InliningStatus_NotInlined;
    }
=======
  return InliningStatus_Inlined;
}
>>>>>>> upstream-releases

<<<<<<< HEAD
  callInfo.setImplicitlyUsedUnchecked();
||||||| merged common ancestors
    MInstruction* ins = addArrayBufferByteLength(objArg);
    current->push(ins);
=======
IonBuilder::InliningResult IonBuilder::inlineObjectCreate(CallInfo& callInfo) {
  if (callInfo.argc() != 1 || callInfo.constructing()) {
    return InliningStatus_NotInlined;
  }
>>>>>>> upstream-releases

<<<<<<< HEAD
  bool emitted = false;
  MOZ_TRY(newObjectTryTemplateObject(&emitted, templateObject));

  MOZ_ASSERT(emitted);
  return InliningStatus_Inlined;
}
||||||| merged common ancestors
    callInfo.setImplicitlyUsedUnchecked();
    return InliningStatus_Inlined;
}
=======
  JSObject* templateObject =
      inspector->getTemplateObjectForNative(pc, obj_create);
  if (!templateObject) {
    return InliningStatus_NotInlined;
  }
>>>>>>> upstream-releases

<<<<<<< HEAD
IonBuilder::InliningResult IonBuilder::inlineObjectIs(CallInfo& callInfo) {
  if (callInfo.argc() < 2 || callInfo.constructing()) {
    return InliningStatus_NotInlined;
  }
||||||| merged common ancestors
IonBuilder::InliningResult
IonBuilder::inlinePossiblyWrappedArrayBufferByteLength(CallInfo& callInfo)
{
    MOZ_ASSERT(!callInfo.constructing());
    MOZ_ASSERT(callInfo.argc() == 1);
=======
  MOZ_ASSERT(templateObject->is<PlainObject>());
  MOZ_ASSERT(!templateObject->isSingleton());
>>>>>>> upstream-releases

<<<<<<< HEAD
  if (getInlineReturnType() != MIRType::Boolean) {
    return InliningStatus_NotInlined;
  }

  MDefinition* left = callInfo.getArg(0);
  MDefinition* right = callInfo.getArg(1);
  MIRType leftType = left->type();
  MIRType rightType = right->type();

  auto mightBeFloatingPointType = [](MDefinition* def) {
    return def->mightBeType(MIRType::Double) ||
           def->mightBeType(MIRType::Float32);
  };

  bool strictEq;
  bool incompatibleTypes = false;
  if (leftType == rightType) {
    // We can only compare the arguments with strict-equals semantics if
    // they aren't floating-point types or values which might be floating-
    // point types. Otherwise we need to use MSameValue.
    strictEq = leftType != MIRType::Value ? !IsFloatingPointType(leftType)
                                          : (!mightBeFloatingPointType(left) &&
                                             !mightBeFloatingPointType(right));
  } else if (leftType == MIRType::Value) {
    // Also use strict-equals when comparing a value with a non-number or
    // the value cannot be a floating-point type.
    strictEq = !IsNumberType(rightType) || !mightBeFloatingPointType(left);
  } else if (rightType == MIRType::Value) {
    // Dual case to the previous one, only with reversed operands.
    strictEq = !IsNumberType(leftType) || !mightBeFloatingPointType(right);
  } else if (IsNumberType(leftType) && IsNumberType(rightType)) {
    // Both arguments are numbers, but with different representations. We
    // can't use strict-equals semantics to compare the operands, but
    // instead need to use MSameValue.
    strictEq = false;
  } else {
    incompatibleTypes = true;
  }

  if (incompatibleTypes) {
    // The result is always |false| when comparing incompatible types.
    pushConstant(BooleanValue(false));
  } else if (strictEq) {
    // Specialize |Object.is(lhs, rhs)| as |lhs === rhs|.
    MOZ_TRY(jsop_compare(JSOP_STRICTEQ, left, right));
  } else {
    MSameValue* ins = MSameValue::New(alloc(), left, right);
||||||| merged common ancestors
    MDefinition* objArg = callInfo.getArg(0);
    if (objArg->type() != MIRType::Object) {
        return InliningStatus_NotInlined;
    }
    if (getInlineReturnType() != MIRType::Int32) {
        return InliningStatus_NotInlined;
    }
=======
  // Ensure the argument matches the template object's prototype.
  MDefinition* arg = callInfo.getArg(0);
  if (JSObject* proto = templateObject->staticPrototype()) {
    if (IsInsideNursery(proto)) {
      return InliningStatus_NotInlined;
    }
>>>>>>> upstream-releases

<<<<<<< HEAD
    // The more specific operand is expected to be in the rhs.
    if (IsNumberType(leftType) && rightType == MIRType::Value) {
      ins->swapOperands();
||||||| merged common ancestors
    if (!IsArrayBufferObject(constraints(), objArg)) {
        return InliningStatus_NotInlined;
=======
    TemporaryTypeSet* types = arg->resultTypeSet();
    if (!types || types->maybeSingleton() != proto) {
      return InliningStatus_NotInlined;
>>>>>>> upstream-releases
    }

<<<<<<< HEAD
    current->add(ins);
    current->push(ins);
  }
||||||| merged common ancestors
    MInstruction* ins = addArrayBufferByteLength(objArg);
    current->push(ins);
=======
    MOZ_ASSERT(types->getKnownMIRType() == MIRType::Object);
  } else {
    if (arg->type() != MIRType::Null) {
      return InliningStatus_NotInlined;
    }
  }
>>>>>>> upstream-releases

<<<<<<< HEAD
  callInfo.setImplicitlyUsedUnchecked();
  return InliningStatus_Inlined;
}
||||||| merged common ancestors
    callInfo.setImplicitlyUsedUnchecked();
    return InliningStatus_Inlined;
}
=======
  callInfo.setImplicitlyUsedUnchecked();
>>>>>>> upstream-releases

<<<<<<< HEAD
IonBuilder::InliningResult IonBuilder::inlineObjectToString(
    CallInfo& callInfo) {
  if (callInfo.constructing() || callInfo.argc() != 0) {
    trackOptimizationOutcome(TrackedOutcome::CantInlineNativeBadForm);
    return InliningStatus_NotInlined;
  }
||||||| merged common ancestors
IonBuilder::InliningResult
IonBuilder::inlineTypedArray(CallInfo& callInfo, Native native)
{
    if (!callInfo.constructing()) {
        trackOptimizationOutcome(TrackedOutcome::CantInlineNativeBadForm);
        return InliningStatus_NotInlined;
    }

    if (getInlineReturnType() != MIRType::Object) {
        return InliningStatus_NotInlined;
    }
    if (callInfo.argc() != 1) {
        return InliningStatus_NotInlined;
    }
=======
  bool emitted = false;
  MOZ_TRY(newObjectTryTemplateObject(&emitted, templateObject));

  MOZ_ASSERT(emitted);
  return InliningStatus_Inlined;
}
>>>>>>> upstream-releases

<<<<<<< HEAD
  if (getInlineReturnType() != MIRType::String) {
    return InliningStatus_NotInlined;
  }
||||||| merged common ancestors
    MDefinition* arg = callInfo.getArg(0);
=======
IonBuilder::InliningResult IonBuilder::inlineObjectIs(CallInfo& callInfo) {
  if (callInfo.argc() < 2 || callInfo.constructing()) {
    return InliningStatus_NotInlined;
  }
>>>>>>> upstream-releases

<<<<<<< HEAD
  MDefinition* arg = callInfo.thisArg();
  if (arg->type() != MIRType::Object) {
    return InliningStatus_NotInlined;
  }
||||||| merged common ancestors
    if (arg->type() != MIRType::Int32) {
        return InliningStatus_NotInlined;
    }
=======
  if (getInlineReturnType() != MIRType::Boolean) {
    return InliningStatus_NotInlined;
  }

  MDefinition* left = callInfo.getArg(0);
  MDefinition* right = callInfo.getArg(1);
  MIRType leftType = left->type();
  MIRType rightType = right->type();

  auto mightBeFloatingPointType = [](MDefinition* def) {
    return def->mightBeType(MIRType::Double) ||
           def->mightBeType(MIRType::Float32);
  };

  bool strictEq;
  bool incompatibleTypes = false;
  if (leftType == rightType) {
    // We can only compare the arguments with strict-equals semantics if
    // they aren't floating-point types or values which might be floating-
    // point types. Otherwise we need to use MSameValue.
    strictEq = leftType != MIRType::Value ? !IsFloatingPointType(leftType)
                                          : (!mightBeFloatingPointType(left) &&
                                             !mightBeFloatingPointType(right));
  } else if (leftType == MIRType::Value) {
    // Also use strict-equals when comparing a value with a non-number or
    // the value cannot be a floating-point type.
    strictEq = !IsNumberType(rightType) || !mightBeFloatingPointType(left);
  } else if (rightType == MIRType::Value) {
    // Dual case to the previous one, only with reversed operands.
    strictEq = !IsNumberType(leftType) || !mightBeFloatingPointType(right);
  } else if (IsNumberType(leftType) && IsNumberType(rightType)) {
    // Both arguments are numbers, but with different representations. We
    // can't use strict-equals semantics to compare the operands, but
    // instead need to use MSameValue.
    strictEq = false;
  } else {
    incompatibleTypes = true;
  }

  if (incompatibleTypes) {
    // The result is always |false| when comparing incompatible types.
    pushConstant(BooleanValue(false));
  } else if (strictEq) {
    // Specialize |Object.is(lhs, rhs)| as |lhs === rhs|.
    MOZ_TRY(jsop_compare(JSOP_STRICTEQ, left, right));
  } else {
    MSameValue* ins = MSameValue::New(alloc(), left, right);

    // The more specific operand is expected to be in the rhs.
    if (IsNumberType(leftType) && rightType == MIRType::Value) {
      ins->swapOperands();
    }
>>>>>>> upstream-releases

<<<<<<< HEAD
  TemporaryTypeSet* types = arg->resultTypeSet();
  if (!types || types->unknownObject()) {
    return InliningStatus_NotInlined;
  }
||||||| merged common ancestors
    JSObject* templateObject = inspector->getTemplateObjectForNative(pc, native);
=======
    current->add(ins);
    current->push(ins);
  }
>>>>>>> upstream-releases

<<<<<<< HEAD
  // Don't optimize if this might be a proxy.
  using ForAllResult = TemporaryTypeSet::ForAllResult;
  if (types->forAllClasses(constraints(), IsProxyClass) !=
      ForAllResult::ALL_FALSE) {
    return InliningStatus_NotInlined;
  }

  // Make sure there's no Symbol.toStringTag property.
  jsid toStringTag =
      SYMBOL_TO_JSID(realm->runtime()->wellKnownSymbols().toStringTag);
  bool res;
  MOZ_TRY_VAR(res, testNotDefinedProperty(arg, toStringTag));
  if (!res) {
    return InliningStatus_NotInlined;
  }
||||||| merged common ancestors
    if (!templateObject) {
        trackOptimizationOutcome(TrackedOutcome::CantInlineNativeNoTemplateObj);
        return InliningStatus_NotInlined;
    }
=======
  callInfo.setImplicitlyUsedUnchecked();
  return InliningStatus_Inlined;
}
>>>>>>> upstream-releases

<<<<<<< HEAD
  // At this point we know we're going to inline this.
  callInfo.setImplicitlyUsedUnchecked();
||||||| merged common ancestors
    MOZ_ASSERT(templateObject->is<TypedArrayObject>());
    TypedArrayObject* obj = &templateObject->as<TypedArrayObject>();
=======
IonBuilder::InliningResult IonBuilder::inlineObjectToString(
    CallInfo& callInfo) {
  if (callInfo.constructing() || callInfo.argc() != 0) {
    trackOptimizationOutcome(TrackedOutcome::CantInlineNativeBadForm);
    return InliningStatus_NotInlined;
  }
>>>>>>> upstream-releases

<<<<<<< HEAD
  // Try to constant fold some common cases.
  if (const Class* knownClass = types->getKnownClass(constraints())) {
    if (knownClass == &PlainObject::class_ ||
        knownClass == &UnboxedPlainObject::class_) {
      pushConstant(StringValue(names().objectObject));
      return InliningStatus_Inlined;
    }
    if (IsArrayClass(knownClass)) {
      pushConstant(StringValue(names().objectArray));
      return InliningStatus_Inlined;
    }
    if (knownClass == &JSFunction::class_) {
      pushConstant(StringValue(names().objectFunction));
      return InliningStatus_Inlined;
    }
  }
||||||| merged common ancestors
    // Do not optimize when we see a template object with a singleton type,
    // since it hits at most once.
    if (templateObject->isSingleton()) {
        return InliningStatus_NotInlined;
    }
=======
  if (getInlineReturnType() != MIRType::String) {
    return InliningStatus_NotInlined;
  }
>>>>>>> upstream-releases

<<<<<<< HEAD
  MObjectClassToString* toString = MObjectClassToString::New(alloc(), arg);
  current->add(toString);
  current->push(toString);
||||||| merged common ancestors
    MInstruction* ins = nullptr;
=======
  MDefinition* arg = callInfo.thisArg();
  if (arg->type() != MIRType::Object) {
    return InliningStatus_NotInlined;
  }
>>>>>>> upstream-releases

<<<<<<< HEAD
  return InliningStatus_Inlined;
}
||||||| merged common ancestors
    if (!arg->isConstant()) {
        callInfo.setImplicitlyUsedUnchecked();
        ins = MNewTypedArrayDynamicLength::New(alloc(), constraints(), templateObject,
                                               templateObject->group()->initialHeap(constraints()),
                                               arg);
    } else {
        // Negative lengths must throw a RangeError.  (We don't track that this
        // might have previously thrown, when determining whether to inline, so we
        // have to deal with this error case when inlining.)
        int32_t providedLen = arg->maybeConstantValue()->toInt32();
        if (providedLen <= 0) {
            return InliningStatus_NotInlined;
        }
=======
  TemporaryTypeSet* types = arg->resultTypeSet();
  if (!types || types->unknownObject()) {
    return InliningStatus_NotInlined;
  }
>>>>>>> upstream-releases

<<<<<<< HEAD
IonBuilder::InliningResult IonBuilder::inlineHasClass(CallInfo& callInfo,
                                                      const Class* clasp1,
                                                      const Class* clasp2,
                                                      const Class* clasp3,
                                                      const Class* clasp4) {
  MOZ_ASSERT(!callInfo.constructing());
  MOZ_ASSERT(callInfo.argc() == 1);
||||||| merged common ancestors
        uint32_t len = AssertedCast<uint32_t>(providedLen);
=======
  // Don't optimize if this might be a proxy.
  using ForAllResult = TemporaryTypeSet::ForAllResult;
  if (types->forAllClasses(constraints(), IsProxyClass) !=
      ForAllResult::ALL_FALSE) {
    return InliningStatus_NotInlined;
  }

  // Make sure there's no Symbol.toStringTag property.
  jsid toStringTag =
      SYMBOL_TO_JSID(realm->runtime()->wellKnownSymbols().toStringTag);
  bool res;
  MOZ_TRY_VAR(res, testNotDefinedProperty(arg, toStringTag));
  if (!res) {
    return InliningStatus_NotInlined;
  }
>>>>>>> upstream-releases

<<<<<<< HEAD
  if (callInfo.getArg(0)->type() != MIRType::Object) {
    return InliningStatus_NotInlined;
  }
  if (getInlineReturnType() != MIRType::Boolean) {
    return InliningStatus_NotInlined;
  }

  TemporaryTypeSet* types = callInfo.getArg(0)->resultTypeSet();
  const Class* knownClass =
      types ? types->getKnownClass(constraints()) : nullptr;
  if (knownClass) {
    pushConstant(BooleanValue(knownClass == clasp1 || knownClass == clasp2 ||
                              knownClass == clasp3 || knownClass == clasp4));
  } else {
    MHasClass* hasClass1 = MHasClass::New(alloc(), callInfo.getArg(0), clasp1);
    current->add(hasClass1);

    if (!clasp2 && !clasp3 && !clasp4) {
      current->push(hasClass1);
    } else {
      const Class* remaining[] = {clasp2, clasp3, clasp4};
      MDefinition* last = hasClass1;
      for (size_t i = 0; i < ArrayLength(remaining); i++) {
        MHasClass* hasClass =
            MHasClass::New(alloc(), callInfo.getArg(0), remaining[i]);
        current->add(hasClass);
        MBitOr* either = MBitOr::New(alloc(), last, hasClass);
        either->infer(inspector, pc);
        current->add(either);
        last = either;
      }
||||||| merged common ancestors
        if (obj->length() != len) {
            return InliningStatus_NotInlined;
        }
=======
  // At this point we know we're going to inline this.
  callInfo.setImplicitlyUsedUnchecked();
>>>>>>> upstream-releases

<<<<<<< HEAD
      MDefinition* result = convertToBoolean(last);
      current->push(result);
||||||| merged common ancestors
        callInfo.setImplicitlyUsedUnchecked();
        MConstant* templateConst = MConstant::NewConstraintlessObject(alloc(), obj);
        current->add(templateConst);
        ins = MNewTypedArray::New(alloc(), constraints(), templateConst,
                                  obj->group()->initialHeap(constraints()));
=======
  // Try to constant fold some common cases.
  if (const Class* knownClass = types->getKnownClass(constraints())) {
    if (knownClass == &PlainObject::class_) {
      pushConstant(StringValue(names().objectObject));
      return InliningStatus_Inlined;
    }
    if (IsArrayClass(knownClass)) {
      pushConstant(StringValue(names().objectArray));
      return InliningStatus_Inlined;
    }
    if (knownClass == &JSFunction::class_) {
      pushConstant(StringValue(names().objectFunction));
      return InliningStatus_Inlined;
>>>>>>> upstream-releases
    }
  }

<<<<<<< HEAD
  callInfo.setImplicitlyUsedUnchecked();
  return InliningStatus_Inlined;
||||||| merged common ancestors
    current->add(ins);
    current->push(ins);
    MOZ_TRY(resumeAfter(ins));
    return InliningStatus_Inlined;
=======
  MObjectClassToString* toString = MObjectClassToString::New(alloc(), arg);
  current->add(toString);
  current->push(toString);

  return InliningStatus_Inlined;
>>>>>>> upstream-releases
}

<<<<<<< HEAD
IonBuilder::InliningResult IonBuilder::inlineGuardToClass(CallInfo& callInfo,
                                                          const Class* clasp) {
  MOZ_ASSERT(!callInfo.constructing());
  MOZ_ASSERT(callInfo.argc() == 1);
||||||| merged common ancestors
IonBuilder::InliningResult
IonBuilder::inlineIsTypedArrayHelper(CallInfo& callInfo, WrappingBehavior wrappingBehavior)
{
    MOZ_ASSERT(!callInfo.constructing());
    MOZ_ASSERT(callInfo.argc() == 1);
=======
IonBuilder::InliningResult IonBuilder::inlineHasClass(CallInfo& callInfo,
                                                      const Class* clasp1,
                                                      const Class* clasp2,
                                                      const Class* clasp3,
                                                      const Class* clasp4) {
  MOZ_ASSERT(!callInfo.constructing());
  MOZ_ASSERT(callInfo.argc() == 1);
>>>>>>> upstream-releases

<<<<<<< HEAD
  if (callInfo.getArg(0)->type() != MIRType::Object) {
    return InliningStatus_NotInlined;
  }
||||||| merged common ancestors
    if (callInfo.getArg(0)->type() != MIRType::Object) {
        return InliningStatus_NotInlined;
    }
    if (getInlineReturnType() != MIRType::Boolean) {
        return InliningStatus_NotInlined;
    }
=======
  if (callInfo.getArg(0)->type() != MIRType::Object) {
    return InliningStatus_NotInlined;
  }
  if (getInlineReturnType() != MIRType::Boolean) {
    return InliningStatus_NotInlined;
  }

  TemporaryTypeSet* types = callInfo.getArg(0)->resultTypeSet();
  const Class* knownClass =
      types ? types->getKnownClass(constraints()) : nullptr;
  if (knownClass) {
    pushConstant(BooleanValue(knownClass == clasp1 || knownClass == clasp2 ||
                              knownClass == clasp3 || knownClass == clasp4));
  } else {
    MHasClass* hasClass1 = MHasClass::New(alloc(), callInfo.getArg(0), clasp1);
    current->add(hasClass1);

    if (!clasp2 && !clasp3 && !clasp4) {
      current->push(hasClass1);
    } else {
      const Class* remaining[] = {clasp2, clasp3, clasp4};
      MDefinition* last = hasClass1;
      for (size_t i = 0; i < ArrayLength(remaining); i++) {
        MHasClass* hasClass =
            MHasClass::New(alloc(), callInfo.getArg(0), remaining[i]);
        current->add(hasClass);
        MBitOr* either = MBitOr::New(alloc(), last, hasClass);
        either->infer(inspector, pc);
        current->add(either);
        last = either;
      }

      MDefinition* result = convertToBoolean(last);
      current->push(result);
    }
  }
>>>>>>> upstream-releases

<<<<<<< HEAD
  if (getInlineReturnType() != MIRType::ObjectOrNull &&
      getInlineReturnType() != MIRType::Object) {
    return InliningStatus_NotInlined;
  }
||||||| merged common ancestors
    // The test is elaborate: in-line only if there is exact
    // information.
=======
  callInfo.setImplicitlyUsedUnchecked();
  return InliningStatus_Inlined;
}
>>>>>>> upstream-releases

<<<<<<< HEAD
  TemporaryTypeSet* types = callInfo.getArg(0)->resultTypeSet();
  const Class* knownClass =
      types ? types->getKnownClass(constraints()) : nullptr;
||||||| merged common ancestors
    TemporaryTypeSet* types = callInfo.getArg(0)->resultTypeSet();
    if (!types) {
        return InliningStatus_NotInlined;
    }
=======
IonBuilder::InliningResult IonBuilder::inlineGuardToClass(CallInfo& callInfo,
                                                          const Class* clasp) {
  MOZ_ASSERT(!callInfo.constructing());
  MOZ_ASSERT(callInfo.argc() == 1);
>>>>>>> upstream-releases

<<<<<<< HEAD
  if (knownClass && knownClass == clasp) {
    current->push(callInfo.getArg(0));
  } else {
    MGuardToClass* guardToClass = MGuardToClass::New(
        alloc(), callInfo.getArg(0), clasp, getInlineReturnType());
    current->add(guardToClass);
    current->push(guardToClass);
  }
||||||| merged common ancestors
    // Wrapped typed arrays won't appear to be typed arrays per a
    // |forAllClasses| query.  If wrapped typed arrays are to be considered
    // typed arrays, a negative answer is not conclusive.  Don't inline in
    // that case.
    auto isPossiblyWrapped = [this, wrappingBehavior, types]() {
        if (wrappingBehavior != AllowWrappedTypedArrays) {
            return false;
        }
=======
  if (callInfo.getArg(0)->type() != MIRType::Object) {
    return InliningStatus_NotInlined;
  }
>>>>>>> upstream-releases

<<<<<<< HEAD
  callInfo.setImplicitlyUsedUnchecked();
  return InliningStatus_Inlined;
}
||||||| merged common ancestors
        switch (types->forAllClasses(constraints(), IsProxyClass)) {
          case TemporaryTypeSet::ForAllResult::ALL_FALSE:
          case TemporaryTypeSet::ForAllResult::EMPTY:
            break;
          case TemporaryTypeSet::ForAllResult::ALL_TRUE:
          case TemporaryTypeSet::ForAllResult::MIXED:
            return true;
        }
        return false;
    };
=======
  if (getInlineReturnType() != MIRType::Object) {
    return InliningStatus_NotInlined;
  }

  TemporaryTypeSet* types = callInfo.getArg(0)->resultTypeSet();
  const Class* knownClass =
      types ? types->getKnownClass(constraints()) : nullptr;

  if (knownClass && knownClass == clasp) {
    current->push(callInfo.getArg(0));
  } else {
    MGuardToClass* guardToClass =
        MGuardToClass::New(alloc(), callInfo.getArg(0), clasp);
    current->add(guardToClass);
    current->push(guardToClass);
  }

  callInfo.setImplicitlyUsedUnchecked();
  return InliningStatus_Inlined;
}
>>>>>>> upstream-releases

IonBuilder::InliningResult IonBuilder::inlineGetNextEntryForIterator(
    CallInfo& callInfo, MGetNextEntryForIterator::Mode mode) {
  MOZ_ASSERT(!callInfo.constructing());
  MOZ_ASSERT(callInfo.argc() == 2);

  MDefinition* iterArg = callInfo.getArg(0);
  MDefinition* resultArg = callInfo.getArg(1);

  // Self-hosted code has already validated |iterArg| is a (possibly boxed)
  // Map- or SetIterator object.
  if (iterArg->type() != MIRType::Object) {
    return InliningStatus_NotInlined;
  }

  if (resultArg->type() != MIRType::Object) {
    return InliningStatus_NotInlined;
  }

  TemporaryTypeSet* resultTypes = resultArg->resultTypeSet();
  const Class* resultClasp =
      resultTypes ? resultTypes->getKnownClass(constraints()) : nullptr;
  if (resultClasp != &ArrayObject::class_) {
    return InliningStatus_NotInlined;
  }

  callInfo.setImplicitlyUsedUnchecked();

  MInstruction* next =
      MGetNextEntryForIterator::New(alloc(), iterArg, resultArg, mode);
  current->add(next);
  current->push(next);

  MOZ_TRY(resumeAfter(next));
  return InliningStatus_Inlined;
}

static bool IsArrayBufferObject(CompilerConstraintList* constraints,
                                MDefinition* def) {
  MOZ_ASSERT(def->type() == MIRType::Object);

  TemporaryTypeSet* types = def->resultTypeSet();
  if (!types) {
    return false;
  }

  return types->getKnownClass(constraints) == &ArrayBufferObject::class_;
}

IonBuilder::InliningResult IonBuilder::inlineArrayBufferByteLength(
    CallInfo& callInfo) {
  MOZ_ASSERT(!callInfo.constructing());
  MOZ_ASSERT(callInfo.argc() == 1);

  MDefinition* objArg = callInfo.getArg(0);
  if (objArg->type() != MIRType::Object) {
    return InliningStatus_NotInlined;
  }
  if (getInlineReturnType() != MIRType::Int32) {
    return InliningStatus_NotInlined;
  }

  MInstruction* ins = addArrayBufferByteLength(objArg);
  current->push(ins);

  callInfo.setImplicitlyUsedUnchecked();
  return InliningStatus_Inlined;
}

IonBuilder::InliningResult
IonBuilder::inlinePossiblyWrappedArrayBufferByteLength(CallInfo& callInfo) {
  MOZ_ASSERT(!callInfo.constructing());
  MOZ_ASSERT(callInfo.argc() == 1);

  MDefinition* objArg = callInfo.getArg(0);
  if (objArg->type() != MIRType::Object) {
    return InliningStatus_NotInlined;
  }
  if (getInlineReturnType() != MIRType::Int32) {
    return InliningStatus_NotInlined;
  }

  if (!IsArrayBufferObject(constraints(), objArg)) {
    return InliningStatus_NotInlined;
  }

  MInstruction* ins = addArrayBufferByteLength(objArg);
  current->push(ins);

  callInfo.setImplicitlyUsedUnchecked();
  return InliningStatus_Inlined;
}

<<<<<<< HEAD
IonBuilder::InliningResult IonBuilder::inlineTypedArray(CallInfo& callInfo,
                                                        Native native) {
  if (!callInfo.constructing()) {
    trackOptimizationOutcome(TrackedOutcome::CantInlineNativeBadForm);
    return InliningStatus_NotInlined;
  }

  if (getInlineReturnType() != MIRType::Object) {
    return InliningStatus_NotInlined;
  }
  if (callInfo.argc() != 1) {
    return InliningStatus_NotInlined;
  }
||||||| merged common ancestors
IonBuilder::InliningResult
IonBuilder::inlineSetDisjointTypedElements(CallInfo& callInfo)
{
    MOZ_ASSERT(!callInfo.constructing());
    MOZ_ASSERT(callInfo.argc() == 3);

    // Initial argument requirements.
=======
IonBuilder::InliningResult IonBuilder::inlineTypedArray(CallInfo& callInfo,
                                                        Native native) {
  if (!callInfo.constructing()) {
    trackOptimizationOutcome(TrackedOutcome::CantInlineNativeBadForm);
    return InliningStatus_NotInlined;
  }
>>>>>>> upstream-releases

<<<<<<< HEAD
  MDefinition* arg = callInfo.getArg(0);
||||||| merged common ancestors
    MDefinition* target = callInfo.getArg(0);
    if (target->type() != MIRType::Object) {
        return InliningStatus_NotInlined;
    }
=======
  if (getInlineReturnType() != MIRType::Object) {
    return InliningStatus_NotInlined;
  }
  if (callInfo.argc() == 0 || callInfo.argc() > 3) {
    return InliningStatus_NotInlined;
  }
>>>>>>> upstream-releases

<<<<<<< HEAD
  if (arg->type() != MIRType::Int32) {
    return InliningStatus_NotInlined;
  }
||||||| merged common ancestors
    if (getInlineReturnType() != MIRType::Undefined) {
        return InliningStatus_NotInlined;
    }
=======
  JSObject* templateObject = inspector->getTemplateObjectForNative(pc, native);
  if (!templateObject) {
    trackOptimizationOutcome(TrackedOutcome::CantInlineNativeNoTemplateObj);
    return InliningStatus_NotInlined;
  }
  MOZ_ASSERT(templateObject->is<TypedArrayObject>());
  TypedArrayObject* obj = &templateObject->as<TypedArrayObject>();
>>>>>>> upstream-releases

<<<<<<< HEAD
  JSObject* templateObject = inspector->getTemplateObjectForNative(pc, native);
||||||| merged common ancestors
    MDefinition* targetOffset = callInfo.getArg(1);
    MOZ_ASSERT(targetOffset->type() == MIRType::Int32);
=======
  // Do not optimize when we see a template object with a singleton type,
  // since it hits at most once.
  if (templateObject->isSingleton()) {
    return InliningStatus_NotInlined;
  }
>>>>>>> upstream-releases

<<<<<<< HEAD
  if (!templateObject) {
    trackOptimizationOutcome(TrackedOutcome::CantInlineNativeNoTemplateObj);
    return InliningStatus_NotInlined;
  }
||||||| merged common ancestors
    MDefinition* sourceTypedArray = callInfo.getArg(2);
    if (sourceTypedArray->type() != MIRType::Object) {
        return InliningStatus_NotInlined;
    }
=======
  MDefinition* arg = callInfo.getArg(0);
  MInstruction* ins;
  if (arg->type() == MIRType::Int32) {
    if (!arg->isConstant()) {
      ins = MNewTypedArrayDynamicLength::New(
          alloc(), constraints(), templateObject,
          templateObject->group()->initialHeap(constraints()), arg);
    } else {
      // Negative lengths must throw a RangeError.  (We don't track that this
      // might have previously thrown, when determining whether to inline, so we
      // have to deal with this error case when inlining.)
      int32_t providedLen = arg->maybeConstantValue()->toInt32();
      if (providedLen <= 0) {
        return InliningStatus_NotInlined;
      }
>>>>>>> upstream-releases

<<<<<<< HEAD
  MOZ_ASSERT(templateObject->is<TypedArrayObject>());
  TypedArrayObject* obj = &templateObject->as<TypedArrayObject>();
||||||| merged common ancestors
    // Only attempt to optimize if |target| and |sourceTypedArray| are both
    // definitely typed arrays.  (The former always is.  The latter is not,
    // necessarily, because of wrappers.)
    if (!IsTypedArrayObject(constraints(), target) ||
        !IsTypedArrayObject(constraints(), sourceTypedArray))
    {
        return InliningStatus_NotInlined;
    }
=======
      uint32_t len = AssertedCast<uint32_t>(providedLen);
      if (obj->length() != len) {
        return InliningStatus_NotInlined;
      }
>>>>>>> upstream-releases

<<<<<<< HEAD
  // Do not optimize when we see a template object with a singleton type,
  // since it hits at most once.
  if (templateObject->isSingleton()) {
    return InliningStatus_NotInlined;
  }
||||||| merged common ancestors
    auto sets = MSetDisjointTypedElements::New(alloc(), target, targetOffset, sourceTypedArray);
    current->add(sets);
=======
      MConstant* templateConst =
          MConstant::NewConstraintlessObject(alloc(), obj);
      current->add(templateConst);
      ins = MNewTypedArray::New(alloc(), constraints(), templateConst,
                                obj->group()->initialHeap(constraints()));
    }
  } else if (arg->type() == MIRType::Object) {
    TemporaryTypeSet* types = arg->resultTypeSet();
    if (!types) {
      return InliningStatus_NotInlined;
    }
>>>>>>> upstream-releases

<<<<<<< HEAD
  MInstruction* ins = nullptr;
||||||| merged common ancestors
    pushConstant(UndefinedValue());
=======
    // Don't inline if the argument might be a wrapper.
    if (types->forAllClasses(constraints(), IsProxyClass) !=
        TemporaryTypeSet::ForAllResult::ALL_FALSE) {
      return InliningStatus_NotInlined;
    }
>>>>>>> upstream-releases

<<<<<<< HEAD
  if (!arg->isConstant()) {
    callInfo.setImplicitlyUsedUnchecked();
    ins = MNewTypedArrayDynamicLength::New(
        alloc(), constraints(), templateObject,
        templateObject->group()->initialHeap(constraints()), arg);
  } else {
    // Negative lengths must throw a RangeError.  (We don't track that this
    // might have previously thrown, when determining whether to inline, so we
    // have to deal with this error case when inlining.)
    int32_t providedLen = arg->maybeConstantValue()->toInt32();
    if (providedLen <= 0) {
      return InliningStatus_NotInlined;
||||||| merged common ancestors
    MOZ_TRY(resumeAfter(sets));
    callInfo.setImplicitlyUsedUnchecked();
    return InliningStatus_Inlined;
}

IonBuilder::InliningResult
IonBuilder::inlineObjectIsTypeDescr(CallInfo& callInfo)
{
    MOZ_ASSERT(!callInfo.constructing());
    MOZ_ASSERT(callInfo.argc() == 1);

    if (callInfo.getArg(0)->type() != MIRType::Object) {
        return InliningStatus_NotInlined;
    }
    if (getInlineReturnType() != MIRType::Boolean) {
        return InliningStatus_NotInlined;
=======
    // Don't inline if we saw mixed use of (Shared)ArrayBuffers and other
    // objects.
    auto IsArrayBufferMaybeSharedClass = [](const Class* clasp) {
      return clasp == &ArrayBufferObject::class_ ||
             clasp == &SharedArrayBufferObject::class_;
    };
    switch (
        types->forAllClasses(constraints(), IsArrayBufferMaybeSharedClass)) {
      case TemporaryTypeSet::ForAllResult::ALL_FALSE:
        ins = MNewTypedArrayFromArray::New(
            alloc(), constraints(), templateObject,
            templateObject->group()->initialHeap(constraints()), arg);
        break;
      case TemporaryTypeSet::ForAllResult::ALL_TRUE:
        MDefinition* byteOffset;
        if (callInfo.argc() > 1) {
          byteOffset = callInfo.getArg(1);
        } else {
          byteOffset = constant(UndefinedValue());
        }

        MDefinition* length;
        if (callInfo.argc() > 2) {
          length = callInfo.getArg(2);
        } else {
          length = constant(UndefinedValue());
        }

        ins = MNewTypedArrayFromArrayBuffer::New(
            alloc(), constraints(), templateObject,
            templateObject->group()->initialHeap(constraints()), arg,
            byteOffset, length);
        break;
      case TemporaryTypeSet::ForAllResult::EMPTY:
      case TemporaryTypeSet::ForAllResult::MIXED:
        return InliningStatus_NotInlined;
>>>>>>> upstream-releases
    }
  } else {
    return InliningStatus_NotInlined;
  }

<<<<<<< HEAD
    uint32_t len = AssertedCast<uint32_t>(providedLen);
||||||| merged common ancestors
    // The test is elaborate: in-line only if there is exact
    // information.

    TemporaryTypeSet* types = callInfo.getArg(0)->resultTypeSet();
    if (!types) {
        return InliningStatus_NotInlined;
    }
=======
  callInfo.setImplicitlyUsedUnchecked();
  current->add(ins);
  current->push(ins);
  MOZ_TRY(resumeAfter(ins));
  return InliningStatus_Inlined;
}

IonBuilder::InliningResult IonBuilder::inlineIsTypedArrayConstructor(
    CallInfo& callInfo) {
  MOZ_ASSERT(!callInfo.constructing());
  MOZ_ASSERT(callInfo.argc() == 1);
>>>>>>> upstream-releases

<<<<<<< HEAD
    if (obj->length() != len) {
      return InliningStatus_NotInlined;
||||||| merged common ancestors
    bool result = false;
    switch (types->forAllClasses(constraints(), IsTypeDescrClass)) {
    case TemporaryTypeSet::ForAllResult::ALL_FALSE:
    case TemporaryTypeSet::ForAllResult::EMPTY:
        result = false;
        break;
    case TemporaryTypeSet::ForAllResult::ALL_TRUE:
        result = true;
        break;
    case TemporaryTypeSet::ForAllResult::MIXED:
        return InliningStatus_NotInlined;
=======
  if (getInlineReturnType() != MIRType::Boolean) {
    return InliningStatus_NotInlined;
  }
  if (callInfo.getArg(0)->type() != MIRType::Object) {
    return InliningStatus_NotInlined;
  }

  // Try inlining with a constant if the argument is definitely a TypedArray
  // constructor.
  TemporaryTypeSet* types = callInfo.getArg(0)->resultTypeSet();
  if (!types || types->unknownObject() || types->getObjectCount() == 0) {
    return InliningStatus_NotInlined;
  }
  for (unsigned i = 0; i < types->getObjectCount(); i++) {
    JSObject* singleton = types->getSingleton(i);
    if (!singleton || !IsTypedArrayConstructor(singleton)) {
      return InliningStatus_NotInlined;
>>>>>>> upstream-releases
    }
  }

<<<<<<< HEAD
    callInfo.setImplicitlyUsedUnchecked();
    MConstant* templateConst = MConstant::NewConstraintlessObject(alloc(), obj);
    current->add(templateConst);
    ins = MNewTypedArray::New(alloc(), constraints(), templateConst,
                              obj->group()->initialHeap(constraints()));
  }

  current->add(ins);
  current->push(ins);
  MOZ_TRY(resumeAfter(ins));
  return InliningStatus_Inlined;
||||||| merged common ancestors
    pushConstant(BooleanValue(result));

    callInfo.setImplicitlyUsedUnchecked();
    return InliningStatus_Inlined;
=======
  callInfo.setImplicitlyUsedUnchecked();

  pushConstant(BooleanValue(true));
  return InliningStatus_Inlined;
>>>>>>> upstream-releases
}

<<<<<<< HEAD
IonBuilder::InliningResult IonBuilder::inlineIsTypedArrayHelper(
    CallInfo& callInfo, WrappingBehavior wrappingBehavior) {
  MOZ_ASSERT(!callInfo.constructing());
  MOZ_ASSERT(callInfo.argc() == 1);
||||||| merged common ancestors
IonBuilder::InliningResult
IonBuilder::inlineSetTypedObjectOffset(CallInfo& callInfo)
{
    MOZ_ASSERT(!callInfo.constructing());
    MOZ_ASSERT(callInfo.argc() == 2);
=======
IonBuilder::InliningResult IonBuilder::inlineIsTypedArrayHelper(
    CallInfo& callInfo, WrappingBehavior wrappingBehavior) {
  MOZ_ASSERT(!callInfo.constructing());
  MOZ_ASSERT(callInfo.argc() == 1);

  if (callInfo.getArg(0)->type() != MIRType::Object) {
    return InliningStatus_NotInlined;
  }
  if (getInlineReturnType() != MIRType::Boolean) {
    return InliningStatus_NotInlined;
  }
>>>>>>> upstream-releases

<<<<<<< HEAD
  if (callInfo.getArg(0)->type() != MIRType::Object) {
    return InliningStatus_NotInlined;
  }
  if (getInlineReturnType() != MIRType::Boolean) {
    return InliningStatus_NotInlined;
  }
||||||| merged common ancestors
    MDefinition* typedObj = callInfo.getArg(0);
    MDefinition* offset = callInfo.getArg(1);
=======
  // The test is elaborate: in-line only if there is exact
  // information.
>>>>>>> upstream-releases

<<<<<<< HEAD
  // The test is elaborate: in-line only if there is exact
  // information.
||||||| merged common ancestors
    // Return type should be undefined or something wacky is going on.
    if (getInlineReturnType() != MIRType::Undefined) {
        return InliningStatus_NotInlined;
    }
=======
  TemporaryTypeSet* types = callInfo.getArg(0)->resultTypeSet();
  if (!types) {
    return InliningStatus_NotInlined;
  }
>>>>>>> upstream-releases

<<<<<<< HEAD
  TemporaryTypeSet* types = callInfo.getArg(0)->resultTypeSet();
  if (!types) {
    return InliningStatus_NotInlined;
  }

  // Wrapped typed arrays won't appear to be typed arrays per a
  // |forAllClasses| query.  If wrapped typed arrays are to be considered
  // typed arrays, a negative answer is not conclusive.  Don't inline in
  // that case.
  auto isPossiblyWrapped = [this, wrappingBehavior, types]() {
    if (wrappingBehavior != AllowWrappedTypedArrays) {
      return false;
||||||| merged common ancestors
    // Check typedObj is a, well, typed object. Go ahead and use TI
    // data. If this check should fail, that is almost certainly a bug
    // in self-hosted code -- either because it's not being careful
    // with TI or because of something else -- but we'll just let it
    // fall through to the SetTypedObjectOffset intrinsic in such
    // cases.
    TemporaryTypeSet* types = typedObj->resultTypeSet();
    if (typedObj->type() != MIRType::Object || !types) {
        return InliningStatus_NotInlined;
=======
  // Wrapped typed arrays won't appear to be typed arrays per a
  // |forAllClasses| query.  If wrapped typed arrays are to be considered
  // typed arrays, a negative answer is not conclusive.
  auto isPossiblyWrapped = [this, wrappingBehavior, types]() {
    if (wrappingBehavior != AllowWrappedTypedArrays) {
      return false;
>>>>>>> upstream-releases
    }

    switch (types->forAllClasses(constraints(), IsProxyClass)) {
      case TemporaryTypeSet::ForAllResult::ALL_FALSE:
      case TemporaryTypeSet::ForAllResult::EMPTY:
        break;
      case TemporaryTypeSet::ForAllResult::ALL_TRUE:
      case TemporaryTypeSet::ForAllResult::MIXED:
        return true;
    }
    return false;
  };

<<<<<<< HEAD
  bool result = false;
  bool isConstant = true;
  switch (types->forAllClasses(constraints(), IsTypedArrayClass)) {
    case TemporaryTypeSet::ForAllResult::ALL_FALSE:
      if (isPossiblyWrapped()) {
||||||| merged common ancestors
    // Check type of offset argument is an integer.
    if (offset->type() != MIRType::Int32) {
=======
  bool result = false;
  bool isConstant = true;
  bool possiblyWrapped = false;
  switch (types->forAllClasses(constraints(), IsTypedArrayClass)) {
    case TemporaryTypeSet::ForAllResult::ALL_FALSE:
      if (isPossiblyWrapped()) {
        // Don't inline if we never saw typed arrays, but always only proxies.
>>>>>>> upstream-releases
        return InliningStatus_NotInlined;
      }

<<<<<<< HEAD
      MOZ_FALLTHROUGH;

    case TemporaryTypeSet::ForAllResult::EMPTY:
      result = false;
      break;

    case TemporaryTypeSet::ForAllResult::ALL_TRUE:
      result = true;
      break;

    case TemporaryTypeSet::ForAllResult::MIXED:
      if (isPossiblyWrapped()) {
        return InliningStatus_NotInlined;
      }

      isConstant = false;
      break;
  }

  if (isConstant) {
    pushConstant(BooleanValue(result));
  } else {
    auto* ins = MIsTypedArray::New(alloc(), callInfo.getArg(0));
||||||| merged common ancestors
    callInfo.setImplicitlyUsedUnchecked();
    MInstruction* ins = MSetTypedObjectOffset::New(alloc(), typedObj, offset);
=======
      MOZ_FALLTHROUGH;

    case TemporaryTypeSet::ForAllResult::EMPTY:
      result = false;
      break;

    case TemporaryTypeSet::ForAllResult::ALL_TRUE:
      result = true;
      break;

    case TemporaryTypeSet::ForAllResult::MIXED:
      isConstant = false;
      possiblyWrapped = isPossiblyWrapped();
      break;
  }

  if (isConstant) {
    pushConstant(BooleanValue(result));
  } else {
    auto* ins =
        MIsTypedArray::New(alloc(), callInfo.getArg(0), possiblyWrapped);
>>>>>>> upstream-releases
    current->add(ins);
    current->push(ins);
<<<<<<< HEAD
  }

  callInfo.setImplicitlyUsedUnchecked();
  return InliningStatus_Inlined;
||||||| merged common ancestors
    return InliningStatus_Inlined;
=======

    if (possiblyWrapped) {
      MOZ_TRY(resumeAfter(ins));
    }
  }

  callInfo.setImplicitlyUsedUnchecked();
  return InliningStatus_Inlined;
>>>>>>> upstream-releases
}

IonBuilder::InliningResult IonBuilder::inlineIsTypedArray(CallInfo& callInfo) {
  return inlineIsTypedArrayHelper(callInfo, RejectWrappedTypedArrays);
}

IonBuilder::InliningResult IonBuilder::inlineIsPossiblyWrappedTypedArray(
    CallInfo& callInfo) {
  return inlineIsTypedArrayHelper(callInfo, AllowWrappedTypedArrays);
}

static bool IsTypedArrayObject(CompilerConstraintList* constraints,
                               MDefinition* def) {
  MOZ_ASSERT(def->type() == MIRType::Object);

  TemporaryTypeSet* types = def->resultTypeSet();
  if (!types) {
    return false;
  }

  return types->forAllClasses(constraints, IsTypedArrayClass) ==
         TemporaryTypeSet::ForAllResult::ALL_TRUE;
}

IonBuilder::InliningResult IonBuilder::inlinePossiblyWrappedTypedArrayLength(
    CallInfo& callInfo) {
  MOZ_ASSERT(!callInfo.constructing());
  MOZ_ASSERT(callInfo.argc() == 1);
  if (callInfo.getArg(0)->type() != MIRType::Object) {
    return InliningStatus_NotInlined;
  }
  if (getInlineReturnType() != MIRType::Int32) {
    return InliningStatus_NotInlined;
  }

  if (!IsTypedArrayObject(constraints(), callInfo.getArg(0))) {
    return InliningStatus_NotInlined;
  }

  MInstruction* length = addTypedArrayLength(callInfo.getArg(0));
  current->push(length);

  callInfo.setImplicitlyUsedUnchecked();
  return InliningStatus_Inlined;
}

IonBuilder::InliningResult IonBuilder::inlineTypedArrayLength(
    CallInfo& callInfo) {
  return inlinePossiblyWrappedTypedArrayLength(callInfo);
}

<<<<<<< HEAD
IonBuilder::InliningResult IonBuilder::inlineSetDisjointTypedElements(
    CallInfo& callInfo) {
  MOZ_ASSERT(!callInfo.constructing());
  MOZ_ASSERT(callInfo.argc() == 3);
||||||| merged common ancestors
IonBuilder::InliningResult
IonBuilder::inlineUnsafeGetReservedSlot(CallInfo& callInfo, MIRType knownValueType)
{
    MOZ_ASSERT(!callInfo.constructing());
    MOZ_ASSERT(callInfo.argc() == 2);
=======
IonBuilder::InliningResult IonBuilder::inlineTypedArrayByteOffset(
    CallInfo& callInfo) {
  MOZ_ASSERT(!callInfo.constructing());
  MOZ_ASSERT(callInfo.argc() == 1);
  if (callInfo.getArg(0)->type() != MIRType::Object) {
    return InliningStatus_NotInlined;
  }
  if (getInlineReturnType() != MIRType::Int32) {
    return InliningStatus_NotInlined;
  }
>>>>>>> upstream-releases

<<<<<<< HEAD
  // Initial argument requirements.
||||||| merged common ancestors
    MDefinition* obj = callInfo.getArg(0);
    if (obj->type() != MIRType::Object && obj->type() != MIRType::Value) {
        return InliningStatus_NotInlined;
    }
=======
  if (!IsTypedArrayObject(constraints(), callInfo.getArg(0))) {
    return InliningStatus_NotInlined;
  }
>>>>>>> upstream-releases

<<<<<<< HEAD
  MDefinition* target = callInfo.getArg(0);
  if (target->type() != MIRType::Object) {
    return InliningStatus_NotInlined;
  }
||||||| merged common ancestors
    MDefinition* arg = callInfo.getArg(1);
    if (arg->type() != MIRType::Int32) {
        return InliningStatus_NotInlined;
    }
=======
  MInstruction* byteOffset = addTypedArrayByteOffset(callInfo.getArg(0));
  current->push(byteOffset);
>>>>>>> upstream-releases

<<<<<<< HEAD
  if (getInlineReturnType() != MIRType::Undefined) {
    return InliningStatus_NotInlined;
  }
||||||| merged common ancestors
    // Don't inline if we don't have a constant slot.
    if (!arg->isConstant()) {
        return InliningStatus_NotInlined;
    }
    uint32_t slot = uint32_t(arg->toConstant()->toInt32());
=======
  callInfo.setImplicitlyUsedUnchecked();
  return InliningStatus_Inlined;
}
>>>>>>> upstream-releases

<<<<<<< HEAD
  MDefinition* targetOffset = callInfo.getArg(1);
  MOZ_ASSERT(targetOffset->type() == MIRType::Int32);
||||||| merged common ancestors
    // Don't inline if it's not a fixed slot.
    if (slot >= NativeObject::MAX_FIXED_SLOTS) {
        return InliningStatus_NotInlined;
    }
=======
IonBuilder::InliningResult IonBuilder::inlineTypedArrayElementShift(
    CallInfo& callInfo) {
  MOZ_ASSERT(!callInfo.constructing());
  MOZ_ASSERT(callInfo.argc() == 1);
  if (callInfo.getArg(0)->type() != MIRType::Object) {
    return InliningStatus_NotInlined;
  }
  if (getInlineReturnType() != MIRType::Int32) {
    return InliningStatus_NotInlined;
  }
>>>>>>> upstream-releases

<<<<<<< HEAD
  MDefinition* sourceTypedArray = callInfo.getArg(2);
  if (sourceTypedArray->type() != MIRType::Object) {
    return InliningStatus_NotInlined;
  }
||||||| merged common ancestors
    callInfo.setImplicitlyUsedUnchecked();
=======
  if (!IsTypedArrayObject(constraints(), callInfo.getArg(0))) {
    return InliningStatus_NotInlined;
  }
>>>>>>> upstream-releases

<<<<<<< HEAD
  // Only attempt to optimize if |target| and |sourceTypedArray| are both
  // definitely typed arrays.  (The former always is.  The latter is not,
  // necessarily, because of wrappers.)
  if (!IsTypedArrayObject(constraints(), target) ||
      !IsTypedArrayObject(constraints(), sourceTypedArray)) {
    return InliningStatus_NotInlined;
  }
||||||| merged common ancestors
    MLoadFixedSlot* load = MLoadFixedSlot::New(alloc(), obj, slot);
    current->add(load);
    current->push(load);
    if (knownValueType != MIRType::Value) {
        // We know what type we have in this slot.  Assert that this is in fact
        // what we've seen coming from this slot in the past, then tell the
        // MLoadFixedSlot about its result type.  That will make us do an
        // infallible unbox as part of the slot load and then we'll barrier on
        // the unbox result.  That way the type barrier code won't end up doing
        // MIRType checks and conditional unboxing.
        MOZ_ASSERT_IF(!getInlineReturnTypeSet()->empty(),
                      getInlineReturnType() == knownValueType);
        load->setResultType(knownValueType);
    }
=======
  auto* ins = MTypedArrayElementShift::New(alloc(), callInfo.getArg(0));
  current->add(ins);
  current->push(ins);
>>>>>>> upstream-releases

<<<<<<< HEAD
  auto sets = MSetDisjointTypedElements::New(alloc(), target, targetOffset,
                                             sourceTypedArray);
  current->add(sets);
||||||| merged common ancestors
    // We don't track reserved slot types, so always emit a barrier.
    MOZ_TRY(pushTypeBarrier(load, getInlineReturnTypeSet(), BarrierKind::TypeSet));
    return InliningStatus_Inlined;
}
=======
  callInfo.setImplicitlyUsedUnchecked();
  return InliningStatus_Inlined;
}
>>>>>>> upstream-releases

<<<<<<< HEAD
  pushConstant(UndefinedValue());
||||||| merged common ancestors
IonBuilder::InliningResult
IonBuilder::inlineIsCallable(CallInfo& callInfo)
{
    MOZ_ASSERT(!callInfo.constructing());
    MOZ_ASSERT(callInfo.argc() == 1);
=======
IonBuilder::InliningResult IonBuilder::inlineSetDisjointTypedElements(
    CallInfo& callInfo) {
  MOZ_ASSERT(!callInfo.constructing());
  MOZ_ASSERT(callInfo.argc() == 3);
>>>>>>> upstream-releases

<<<<<<< HEAD
  MOZ_TRY(resumeAfter(sets));
  callInfo.setImplicitlyUsedUnchecked();
  return InliningStatus_Inlined;
}
||||||| merged common ancestors
    if (getInlineReturnType() != MIRType::Boolean) {
        return InliningStatus_NotInlined;
    }
=======
  // Initial argument requirements.
>>>>>>> upstream-releases

<<<<<<< HEAD
IonBuilder::InliningResult IonBuilder::inlineObjectIsTypeDescr(
    CallInfo& callInfo) {
  MOZ_ASSERT(!callInfo.constructing());
  MOZ_ASSERT(callInfo.argc() == 1);
||||||| merged common ancestors
    MDefinition* arg = callInfo.getArg(0);
=======
  MDefinition* target = callInfo.getArg(0);
  if (target->type() != MIRType::Object) {
    return InliningStatus_NotInlined;
  }
>>>>>>> upstream-releases

<<<<<<< HEAD
  if (callInfo.getArg(0)->type() != MIRType::Object) {
    return InliningStatus_NotInlined;
  }
  if (getInlineReturnType() != MIRType::Boolean) {
    return InliningStatus_NotInlined;
  }
||||||| merged common ancestors
    // Try inlining with constant true/false: only objects may be callable at
    // all, and if we know the class check if it is callable.
    bool isCallableKnown = false;
    bool isCallableConstant;
    if (arg->type() == MIRType::Object) {
        TemporaryTypeSet* types = arg->resultTypeSet();
        const Class* clasp = types ? types->getKnownClass(constraints()) : nullptr;
        if (clasp && !clasp->isProxy()) {
            isCallableKnown = true;
            isCallableConstant = clasp->nonProxyCallable();
        }
    } else if (!arg->mightBeType(MIRType::Object)) {
        // Primitive (including undefined and null).
        isCallableKnown = true;
        isCallableConstant = false;
    } else if (arg->type() != MIRType::Value) {
        return InliningStatus_NotInlined;
    }
=======
  if (getInlineReturnType() != MIRType::Undefined) {
    return InliningStatus_NotInlined;
  }
>>>>>>> upstream-releases

<<<<<<< HEAD
  // The test is elaborate: in-line only if there is exact
  // information.
||||||| merged common ancestors
    callInfo.setImplicitlyUsedUnchecked();
=======
  MDefinition* targetOffset = callInfo.getArg(1);
  MOZ_ASSERT(targetOffset->type() == MIRType::Int32);
>>>>>>> upstream-releases

<<<<<<< HEAD
  TemporaryTypeSet* types = callInfo.getArg(0)->resultTypeSet();
  if (!types) {
    return InliningStatus_NotInlined;
  }

  bool result = false;
  switch (types->forAllClasses(constraints(), IsTypeDescrClass)) {
    case TemporaryTypeSet::ForAllResult::ALL_FALSE:
    case TemporaryTypeSet::ForAllResult::EMPTY:
      result = false;
      break;
    case TemporaryTypeSet::ForAllResult::ALL_TRUE:
      result = true;
      break;
    case TemporaryTypeSet::ForAllResult::MIXED:
      return InliningStatus_NotInlined;
  }
||||||| merged common ancestors
    if (isCallableKnown) {
        MConstant* constant = MConstant::New(alloc(), BooleanValue(isCallableConstant));
        current->add(constant);
        current->push(constant);
        return InliningStatus_Inlined;
    }
=======
  MDefinition* sourceTypedArray = callInfo.getArg(2);
  if (sourceTypedArray->type() != MIRType::Object) {
    return InliningStatus_NotInlined;
  }

  // Only attempt to optimize if |target| and |sourceTypedArray| are both
  // definitely typed arrays.  (The former always is.  The latter is not,
  // necessarily, because of wrappers.)
  if (!IsTypedArrayObject(constraints(), target) ||
      !IsTypedArrayObject(constraints(), sourceTypedArray)) {
    return InliningStatus_NotInlined;
  }
>>>>>>> upstream-releases

<<<<<<< HEAD
  pushConstant(BooleanValue(result));
||||||| merged common ancestors
    MIsCallable* isCallable = MIsCallable::New(alloc(), arg);
    current->add(isCallable);
    current->push(isCallable);
=======
  auto sets = MSetDisjointTypedElements::New(alloc(), target, targetOffset,
                                             sourceTypedArray);
  current->add(sets);
>>>>>>> upstream-releases

<<<<<<< HEAD
  callInfo.setImplicitlyUsedUnchecked();
  return InliningStatus_Inlined;
||||||| merged common ancestors
    return InliningStatus_Inlined;
=======
  pushConstant(UndefinedValue());

  MOZ_TRY(resumeAfter(sets));
  callInfo.setImplicitlyUsedUnchecked();
  return InliningStatus_Inlined;
>>>>>>> upstream-releases
}

<<<<<<< HEAD
IonBuilder::InliningResult IonBuilder::inlineSetTypedObjectOffset(
    CallInfo& callInfo) {
  MOZ_ASSERT(!callInfo.constructing());
  MOZ_ASSERT(callInfo.argc() == 2);
||||||| merged common ancestors
IonBuilder::InliningResult
IonBuilder::inlineIsConstructor(CallInfo& callInfo)
{
    MOZ_ASSERT(!callInfo.constructing());
    MOZ_ASSERT(callInfo.argc() == 1);
=======
IonBuilder::InliningResult IonBuilder::inlineObjectIsTypeDescr(
    CallInfo& callInfo) {
  MOZ_ASSERT(!callInfo.constructing());
  MOZ_ASSERT(callInfo.argc() == 1);
>>>>>>> upstream-releases

<<<<<<< HEAD
  MDefinition* typedObj = callInfo.getArg(0);
  MDefinition* offset = callInfo.getArg(1);
||||||| merged common ancestors
    if (getInlineReturnType() != MIRType::Boolean) {
        return InliningStatus_NotInlined;
    }
    if (callInfo.getArg(0)->type() != MIRType::Object) {
        return InliningStatus_NotInlined;
    }
=======
  if (callInfo.getArg(0)->type() != MIRType::Object) {
    return InliningStatus_NotInlined;
  }
  if (getInlineReturnType() != MIRType::Boolean) {
    return InliningStatus_NotInlined;
  }
>>>>>>> upstream-releases

<<<<<<< HEAD
  // Return type should be undefined or something wacky is going on.
  if (getInlineReturnType() != MIRType::Undefined) {
    return InliningStatus_NotInlined;
  }

  // Check typedObj is a, well, typed object. Go ahead and use TI
  // data. If this check should fail, that is almost certainly a bug
  // in self-hosted code -- either because it's not being careful
  // with TI or because of something else -- but we'll just let it
  // fall through to the SetTypedObjectOffset intrinsic in such
  // cases.
  TemporaryTypeSet* types = typedObj->resultTypeSet();
  if (typedObj->type() != MIRType::Object || !types) {
    return InliningStatus_NotInlined;
  }
  switch (types->forAllClasses(constraints(), IsTypedObjectClass)) {
    case TemporaryTypeSet::ForAllResult::ALL_FALSE:
    case TemporaryTypeSet::ForAllResult::EMPTY:
    case TemporaryTypeSet::ForAllResult::MIXED:
      return InliningStatus_NotInlined;
    case TemporaryTypeSet::ForAllResult::ALL_TRUE:
      break;
  }
||||||| merged common ancestors
    callInfo.setImplicitlyUsedUnchecked();
=======
  // The test is elaborate: in-line only if there is exact
  // information.

  TemporaryTypeSet* types = callInfo.getArg(0)->resultTypeSet();
  if (!types) {
    return InliningStatus_NotInlined;
  }

  bool result = false;
  switch (types->forAllClasses(constraints(), IsTypeDescrClass)) {
    case TemporaryTypeSet::ForAllResult::ALL_FALSE:
    case TemporaryTypeSet::ForAllResult::EMPTY:
      result = false;
      break;
    case TemporaryTypeSet::ForAllResult::ALL_TRUE:
      result = true;
      break;
    case TemporaryTypeSet::ForAllResult::MIXED:
      return InliningStatus_NotInlined;
  }

  pushConstant(BooleanValue(result));

  callInfo.setImplicitlyUsedUnchecked();
  return InliningStatus_Inlined;
}

IonBuilder::InliningResult IonBuilder::inlineSetTypedObjectOffset(
    CallInfo& callInfo) {
  MOZ_ASSERT(!callInfo.constructing());
  MOZ_ASSERT(callInfo.argc() == 2);

  MDefinition* typedObj = callInfo.getArg(0);
  MDefinition* offset = callInfo.getArg(1);

  // Return type should be undefined or something wacky is going on.
  if (getInlineReturnType() != MIRType::Undefined) {
    return InliningStatus_NotInlined;
  }

  // Check typedObj is a, well, typed object. Go ahead and use TI
  // data. If this check should fail, that is almost certainly a bug
  // in self-hosted code -- either because it's not being careful
  // with TI or because of something else -- but we'll just let it
  // fall through to the SetTypedObjectOffset intrinsic in such
  // cases.
  TemporaryTypeSet* types = typedObj->resultTypeSet();
  if (typedObj->type() != MIRType::Object || !types) {
    return InliningStatus_NotInlined;
  }
  switch (types->forAllClasses(constraints(), IsTypedObjectClass)) {
    case TemporaryTypeSet::ForAllResult::ALL_FALSE:
    case TemporaryTypeSet::ForAllResult::EMPTY:
    case TemporaryTypeSet::ForAllResult::MIXED:
      return InliningStatus_NotInlined;
    case TemporaryTypeSet::ForAllResult::ALL_TRUE:
      break;
  }
>>>>>>> upstream-releases

  // Check type of offset argument is an integer.
  if (offset->type() != MIRType::Int32) {
    return InliningStatus_NotInlined;
  }

  callInfo.setImplicitlyUsedUnchecked();
  MInstruction* ins = MSetTypedObjectOffset::New(alloc(), typedObj, offset);
  current->add(ins);
  current->push(ins);
  return InliningStatus_Inlined;
}

IonBuilder::InliningResult IonBuilder::inlineUnsafeSetReservedSlot(
    CallInfo& callInfo) {
  MOZ_ASSERT(!callInfo.constructing());
  MOZ_ASSERT(callInfo.argc() == 3);

  if (getInlineReturnType() != MIRType::Undefined) {
    return InliningStatus_NotInlined;
  }

  MDefinition* obj = callInfo.getArg(0);
  if (obj->type() != MIRType::Object && obj->type() != MIRType::Value) {
    return InliningStatus_NotInlined;
  }

  MDefinition* arg = callInfo.getArg(1);
  if (arg->type() != MIRType::Int32) {
    return InliningStatus_NotInlined;
  }

  // Don't inline if we don't have a constant slot.
  if (!arg->isConstant()) {
    return InliningStatus_NotInlined;
  }
  uint32_t slot = uint32_t(arg->toConstant()->toInt32());

  // Don't inline if it's not a fixed slot.
  if (slot >= NativeObject::MAX_FIXED_SLOTS) {
    return InliningStatus_NotInlined;
  }

  callInfo.setImplicitlyUsedUnchecked();

  MStoreFixedSlot* store =
      MStoreFixedSlot::NewBarriered(alloc(), obj, slot, callInfo.getArg(2));
  current->add(store);
  current->push(store);

  if (needsPostBarrier(callInfo.getArg(2))) {
    current->add(MPostWriteBarrier::New(alloc(), obj, callInfo.getArg(2)));
  }

  return InliningStatus_Inlined;
}

<<<<<<< HEAD
IonBuilder::InliningResult IonBuilder::inlineUnsafeGetReservedSlot(
    CallInfo& callInfo, MIRType knownValueType) {
  MOZ_ASSERT(!callInfo.constructing());
  MOZ_ASSERT(callInfo.argc() == 2);
||||||| merged common ancestors
IonBuilder::InliningResult
IonBuilder::inlineIsCrossRealmArrayConstructor(CallInfo& callInfo)
{
    MOZ_ASSERT(!callInfo.constructing());
    MOZ_ASSERT(callInfo.argc() == 1);

    if (getInlineReturnType() != MIRType::Boolean) {
        return InliningStatus_NotInlined;
    }
    MDefinition* arg = callInfo.getArg(0);
    if (arg->type() != MIRType::Object) {
        return InliningStatus_NotInlined;
    }
=======
IonBuilder::InliningResult IonBuilder::inlineUnsafeGetReservedSlot(
    CallInfo& callInfo, MIRType knownValueType) {
  MOZ_ASSERT(!callInfo.constructing());
  MOZ_ASSERT(callInfo.argc() == 2);

  MDefinition* obj = callInfo.getArg(0);
  if (obj->type() != MIRType::Object && obj->type() != MIRType::Value) {
    return InliningStatus_NotInlined;
  }
>>>>>>> upstream-releases

<<<<<<< HEAD
  MDefinition* obj = callInfo.getArg(0);
  if (obj->type() != MIRType::Object && obj->type() != MIRType::Value) {
    return InliningStatus_NotInlined;
  }
||||||| merged common ancestors
    TemporaryTypeSet* types = arg->resultTypeSet();
    Realm* realm = types->getKnownRealm(constraints());
    if (!realm || realm != script()->realm()) {
        return InliningStatus_NotInlined;
    }
=======
  MDefinition* arg = callInfo.getArg(1);
  if (arg->type() != MIRType::Int32) {
    return InliningStatus_NotInlined;
  }
>>>>>>> upstream-releases

<<<<<<< HEAD
  MDefinition* arg = callInfo.getArg(1);
  if (arg->type() != MIRType::Int32) {
    return InliningStatus_NotInlined;
  }
||||||| merged common ancestors
    callInfo.setImplicitlyUsedUnchecked();
=======
  // Don't inline if we don't have a constant slot.
  if (!arg->isConstant()) {
    return InliningStatus_NotInlined;
  }
  uint32_t slot = uint32_t(arg->toConstant()->toInt32());
>>>>>>> upstream-releases

<<<<<<< HEAD
  // Don't inline if we don't have a constant slot.
  if (!arg->isConstant()) {
    return InliningStatus_NotInlined;
  }
  uint32_t slot = uint32_t(arg->toConstant()->toInt32());
||||||| merged common ancestors
    // Inline only if argument is absolutely *not* a wrapper or a cross-realm
    // object.
    pushConstant(BooleanValue(false));
    return InliningStatus_Inlined;
}
=======
  // Don't inline if it's not a fixed slot.
  if (slot >= NativeObject::MAX_FIXED_SLOTS) {
    return InliningStatus_NotInlined;
  }

  callInfo.setImplicitlyUsedUnchecked();

  MLoadFixedSlot* load = MLoadFixedSlot::New(alloc(), obj, slot);
  current->add(load);
  current->push(load);
  if (knownValueType != MIRType::Value) {
    // We know what type we have in this slot.  Assert that this is in fact
    // what we've seen coming from this slot in the past, then tell the
    // MLoadFixedSlot about its result type.  That will make us do an
    // infallible unbox as part of the slot load and then we'll barrier on
    // the unbox result.  That way the type barrier code won't end up doing
    // MIRType checks and conditional unboxing.
    MOZ_ASSERT_IF(!getInlineReturnTypeSet()->empty(),
                  getInlineReturnType() == knownValueType);
    load->setResultType(knownValueType);
  }

  // We don't track reserved slot types, so always emit a barrier.
  MOZ_TRY(
      pushTypeBarrier(load, getInlineReturnTypeSet(), BarrierKind::TypeSet));
  return InliningStatus_Inlined;
}
>>>>>>> upstream-releases

<<<<<<< HEAD
  // Don't inline if it's not a fixed slot.
  if (slot >= NativeObject::MAX_FIXED_SLOTS) {
    return InliningStatus_NotInlined;
  }

  callInfo.setImplicitlyUsedUnchecked();

  MLoadFixedSlot* load = MLoadFixedSlot::New(alloc(), obj, slot);
  current->add(load);
  current->push(load);
  if (knownValueType != MIRType::Value) {
    // We know what type we have in this slot.  Assert that this is in fact
    // what we've seen coming from this slot in the past, then tell the
    // MLoadFixedSlot about its result type.  That will make us do an
    // infallible unbox as part of the slot load and then we'll barrier on
    // the unbox result.  That way the type barrier code won't end up doing
    // MIRType checks and conditional unboxing.
    MOZ_ASSERT_IF(!getInlineReturnTypeSet()->empty(),
                  getInlineReturnType() == knownValueType);
    load->setResultType(knownValueType);
  }

  // We don't track reserved slot types, so always emit a barrier.
  MOZ_TRY(
      pushTypeBarrier(load, getInlineReturnTypeSet(), BarrierKind::TypeSet));
  return InliningStatus_Inlined;
}

IonBuilder::InliningResult IonBuilder::inlineIsCallable(CallInfo& callInfo) {
  MOZ_ASSERT(!callInfo.constructing());
  MOZ_ASSERT(callInfo.argc() == 1);

  if (getInlineReturnType() != MIRType::Boolean) {
    return InliningStatus_NotInlined;
  }
||||||| merged common ancestors
IonBuilder::InliningResult
IonBuilder::inlineToInteger(CallInfo& callInfo)
{
    MOZ_ASSERT(!callInfo.constructing());
    MOZ_ASSERT(callInfo.argc() == 1);

    MDefinition* input = callInfo.getArg(0);

    // Only optimize cases where input contains only number, null or boolean
    if (input->mightBeType(MIRType::Object) ||
        input->mightBeType(MIRType::String) ||
        input->mightBeType(MIRType::Symbol) ||
        input->mightBeType(MIRType::Undefined) ||
        input->mightBeMagicType())
    {
        return InliningStatus_NotInlined;
    }
=======
IonBuilder::InliningResult IonBuilder::inlineIsCallable(CallInfo& callInfo) {
  MOZ_ASSERT(!callInfo.constructing());
  MOZ_ASSERT(callInfo.argc() == 1);

  if (getInlineReturnType() != MIRType::Boolean) {
    return InliningStatus_NotInlined;
  }
>>>>>>> upstream-releases

  MDefinition* arg = callInfo.getArg(0);

  // Try inlining with constant true/false: only objects may be callable at
  // all, and if we know the class check if it is callable.
  bool isCallableKnown = false;
  bool isCallableConstant;
  if (arg->type() == MIRType::Object) {
    TemporaryTypeSet* types = arg->resultTypeSet();
    const Class* clasp = types ? types->getKnownClass(constraints()) : nullptr;
    if (clasp && !clasp->isProxy()) {
      isCallableKnown = true;
      isCallableConstant = clasp->nonProxyCallable();
    }
  } else if (!arg->mightBeType(MIRType::Object)) {
    // Primitive (including undefined and null).
    isCallableKnown = true;
    isCallableConstant = false;
  } else if (arg->type() != MIRType::Value) {
    return InliningStatus_NotInlined;
  }

  callInfo.setImplicitlyUsedUnchecked();

  if (isCallableKnown) {
    MConstant* constant =
        MConstant::New(alloc(), BooleanValue(isCallableConstant));
    current->add(constant);
    current->push(constant);
    return InliningStatus_Inlined;
  }

  MIsCallable* isCallable = MIsCallable::New(alloc(), arg);
  current->add(isCallable);
  current->push(isCallable);

  return InliningStatus_Inlined;
}

IonBuilder::InliningResult IonBuilder::inlineIsConstructor(CallInfo& callInfo) {
  MOZ_ASSERT(!callInfo.constructing());
  MOZ_ASSERT(callInfo.argc() == 1);

  if (getInlineReturnType() != MIRType::Boolean) {
    return InliningStatus_NotInlined;
  }
  if (callInfo.getArg(0)->type() != MIRType::Object) {
    return InliningStatus_NotInlined;
  }

<<<<<<< HEAD
  callInfo.setImplicitlyUsedUnchecked();

  MIsConstructor* ins = MIsConstructor::New(alloc(), callInfo.getArg(0));
  current->add(ins);
  current->push(ins);

  return InliningStatus_Inlined;
}
||||||| merged common ancestors
    callInfo.setImplicitlyUsedUnchecked();
    MToString* toString = MToString::New(alloc(), callInfo.getArg(0));
    current->add(toString);
    current->push(toString);
    return InliningStatus_Inlined;
}
=======
  callInfo.setImplicitlyUsedUnchecked();
>>>>>>> upstream-releases

<<<<<<< HEAD
IonBuilder::InliningResult IonBuilder::inlineIsObject(CallInfo& callInfo) {
  MOZ_ASSERT(!callInfo.constructing());
  MOZ_ASSERT(callInfo.argc() == 1);
||||||| merged common ancestors
IonBuilder::InliningResult
IonBuilder::inlineBailout(CallInfo& callInfo)
{
    callInfo.setImplicitlyUsedUnchecked();
=======
  MIsConstructor* ins = MIsConstructor::New(alloc(), callInfo.getArg(0));
  current->add(ins);
  current->push(ins);
>>>>>>> upstream-releases

<<<<<<< HEAD
  if (getInlineReturnType() != MIRType::Boolean) {
    return InliningStatus_NotInlined;
  }
||||||| merged common ancestors
    current->add(MBail::New(alloc()));
=======
  return InliningStatus_Inlined;
}
>>>>>>> upstream-releases

<<<<<<< HEAD
  callInfo.setImplicitlyUsedUnchecked();
  if (callInfo.getArg(0)->type() == MIRType::Object) {
    pushConstant(BooleanValue(true));
  } else {
    MIsObject* isObject = MIsObject::New(alloc(), callInfo.getArg(0));
    current->add(isObject);
    current->push(isObject);
  }
  return InliningStatus_Inlined;
||||||| merged common ancestors
    MConstant* undefined = MConstant::New(alloc(), UndefinedValue());
    current->add(undefined);
    current->push(undefined);
    return InliningStatus_Inlined;
=======
IonBuilder::InliningResult IonBuilder::inlineIsObject(CallInfo& callInfo) {
  MOZ_ASSERT(!callInfo.constructing());
  MOZ_ASSERT(callInfo.argc() == 1);

  if (getInlineReturnType() != MIRType::Boolean) {
    return InliningStatus_NotInlined;
  }

  callInfo.setImplicitlyUsedUnchecked();
  if (callInfo.getArg(0)->type() == MIRType::Object) {
    pushConstant(BooleanValue(true));
  } else {
    MIsObject* isObject = MIsObject::New(alloc(), callInfo.getArg(0));
    current->add(isObject);
    current->push(isObject);
  }
  return InliningStatus_Inlined;
>>>>>>> upstream-releases
}

<<<<<<< HEAD
IonBuilder::InliningResult IonBuilder::inlineToObject(CallInfo& callInfo) {
  MOZ_ASSERT(!callInfo.constructing());
  MOZ_ASSERT(callInfo.argc() == 1);
||||||| merged common ancestors
IonBuilder::InliningResult
IonBuilder::inlineAssertFloat32(CallInfo& callInfo)
{
    if (callInfo.argc() != 2) {
        return InliningStatus_NotInlined;
    }

    MDefinition* secondArg = callInfo.getArg(1);
=======
IonBuilder::InliningResult IonBuilder::inlineToObject(CallInfo& callInfo) {
  MOZ_ASSERT(!callInfo.constructing());
  MOZ_ASSERT(callInfo.argc() == 1);

  if (getInlineReturnType() != MIRType::Object) {
    return InliningStatus_NotInlined;
  }
>>>>>>> upstream-releases

<<<<<<< HEAD
  if (getInlineReturnType() != MIRType::Object) {
    return InliningStatus_NotInlined;
  }
||||||| merged common ancestors
    MOZ_ASSERT(secondArg->type() == MIRType::Boolean);
    MOZ_ASSERT(secondArg->isConstant());
=======
  MDefinition* object = callInfo.getArg(0);
  if (object->type() != MIRType::Object && object->type() != MIRType::Value) {
    return InliningStatus_NotInlined;
  }
>>>>>>> upstream-releases

<<<<<<< HEAD
  MDefinition* object = callInfo.getArg(0);
  if (object->type() != MIRType::Object && object->type() != MIRType::Value) {
    return InliningStatus_NotInlined;
  }
||||||| merged common ancestors
    bool mustBeFloat32 = secondArg->toConstant()->toBoolean();
    current->add(MAssertFloat32::New(alloc(), callInfo.getArg(0), mustBeFloat32));
=======
  callInfo.setImplicitlyUsedUnchecked();
>>>>>>> upstream-releases

<<<<<<< HEAD
  callInfo.setImplicitlyUsedUnchecked();
||||||| merged common ancestors
    MConstant* undefined = MConstant::New(alloc(), UndefinedValue());
    current->add(undefined);
    current->push(undefined);
    callInfo.setImplicitlyUsedUnchecked();
    return InliningStatus_Inlined;
}
=======
  // If we know the input type is an object, nop ToObject.
  if (object->type() == MIRType::Object) {
    current->push(object);
  } else {
    auto* ins = MToObject::New(alloc(), object);
    current->add(ins);
    current->push(ins);

    MOZ_TRY(
        pushTypeBarrier(ins, getInlineReturnTypeSet(), BarrierKind::TypeSet));
  }

  return InliningStatus_Inlined;
}
>>>>>>> upstream-releases

<<<<<<< HEAD
  // If we know the input type is an object, nop ToObject.
  if (object->type() == MIRType::Object) {
    current->push(object);
  } else {
    auto* ins = MToObject::New(alloc(), object);
    current->add(ins);
    current->push(ins);
||||||| merged common ancestors
IonBuilder::InliningResult
IonBuilder::inlineAssertRecoveredOnBailout(CallInfo& callInfo)
{
    if (callInfo.argc() != 2) {
        return InliningStatus_NotInlined;
    }
=======
IonBuilder::InliningResult IonBuilder::inlineIsCrossRealmArrayConstructor(
    CallInfo& callInfo) {
  MOZ_ASSERT(!callInfo.constructing());
  MOZ_ASSERT(callInfo.argc() == 1);
>>>>>>> upstream-releases

<<<<<<< HEAD
    MOZ_TRY(
        pushTypeBarrier(ins, getInlineReturnTypeSet(), BarrierKind::TypeSet));
  }
||||||| merged common ancestors
    // Don't assert for recovered instructions when recovering is disabled.
    if (JitOptions.disableRecoverIns) {
        return InliningStatus_NotInlined;
    }
=======
  if (getInlineReturnType() != MIRType::Boolean) {
    return InliningStatus_NotInlined;
  }
  MDefinition* arg = callInfo.getArg(0);
  if (arg->type() != MIRType::Object) {
    return InliningStatus_NotInlined;
  }
>>>>>>> upstream-releases

<<<<<<< HEAD
  return InliningStatus_Inlined;
}
||||||| merged common ancestors
    if (JitOptions.checkRangeAnalysis) {
        // If we are checking the range of all instructions, then the guards
        // inserted by Range Analysis prevent the use of recover
        // instruction. Thus, we just disable these checks.
        current->push(constant(UndefinedValue()));
        callInfo.setImplicitlyUsedUnchecked();
        return InliningStatus_Inlined;
    }
=======
  TemporaryTypeSet* types = arg->resultTypeSet();
  Realm* realm = types->getKnownRealm(constraints());
  if (!realm || realm != script()->realm()) {
    return InliningStatus_NotInlined;
  }
>>>>>>> upstream-releases

<<<<<<< HEAD
IonBuilder::InliningResult IonBuilder::inlineIsCrossRealmArrayConstructor(
    CallInfo& callInfo) {
  MOZ_ASSERT(!callInfo.constructing());
  MOZ_ASSERT(callInfo.argc() == 1);
||||||| merged common ancestors
    MDefinition* secondArg = callInfo.getArg(1);
=======
  callInfo.setImplicitlyUsedUnchecked();
>>>>>>> upstream-releases

<<<<<<< HEAD
  if (getInlineReturnType() != MIRType::Boolean) {
    return InliningStatus_NotInlined;
  }
  MDefinition* arg = callInfo.getArg(0);
  if (arg->type() != MIRType::Object) {
    return InliningStatus_NotInlined;
  }
||||||| merged common ancestors
    MOZ_ASSERT(secondArg->type() == MIRType::Boolean);
    MOZ_ASSERT(secondArg->isConstant());
=======
  // Inline only if argument is absolutely *not* a wrapper or a cross-realm
  // object.
  pushConstant(BooleanValue(false));
  return InliningStatus_Inlined;
}
>>>>>>> upstream-releases

<<<<<<< HEAD
  TemporaryTypeSet* types = arg->resultTypeSet();
  Realm* realm = types->getKnownRealm(constraints());
  if (!realm || realm != script()->realm()) {
    return InliningStatus_NotInlined;
  }
||||||| merged common ancestors
    bool mustBeRecovered = secondArg->toConstant()->toBoolean();
    MAssertRecoveredOnBailout* assert =
        MAssertRecoveredOnBailout::New(alloc(), callInfo.getArg(0), mustBeRecovered);
    current->add(assert);
    current->push(assert);
=======
IonBuilder::InliningResult IonBuilder::inlineToInteger(CallInfo& callInfo) {
  MOZ_ASSERT(!callInfo.constructing());
  MOZ_ASSERT(callInfo.argc() == 1);
>>>>>>> upstream-releases

<<<<<<< HEAD
  callInfo.setImplicitlyUsedUnchecked();
||||||| merged common ancestors
    // Create an instruction sequence which implies that the argument of the
    // assertRecoveredOnBailout function would be encoded at least in one
    // Snapshot.
    MNop* nop = MNop::New(alloc());
    current->add(nop);
    MOZ_TRY(resumeAfter(nop));
    current->add(MEncodeSnapshot::New(alloc()));
=======
  MDefinition* input = callInfo.getArg(0);
>>>>>>> upstream-releases

<<<<<<< HEAD
  // Inline only if argument is absolutely *not* a wrapper or a cross-realm
  // object.
  pushConstant(BooleanValue(false));
  return InliningStatus_Inlined;
}
||||||| merged common ancestors
    current->pop();
    current->push(constant(UndefinedValue()));
    callInfo.setImplicitlyUsedUnchecked();
    return InliningStatus_Inlined;
}
=======
  // Only optimize cases where input contains only number, null or boolean
  if (input->mightBeType(MIRType::Object) ||
      input->mightBeType(MIRType::String) ||
      input->mightBeType(MIRType::Symbol) ||
      input->mightBeType(MIRType::BigInt) ||
      input->mightBeType(MIRType::Undefined) || input->mightBeMagicType()) {
    return InliningStatus_NotInlined;
  }
>>>>>>> upstream-releases

<<<<<<< HEAD
IonBuilder::InliningResult IonBuilder::inlineToInteger(CallInfo& callInfo) {
  MOZ_ASSERT(!callInfo.constructing());
  MOZ_ASSERT(callInfo.argc() == 1);
||||||| merged common ancestors
IonBuilder::InliningResult
IonBuilder::inlineAtomicsCompareExchange(CallInfo& callInfo)
{
    if (callInfo.argc() != 4 || callInfo.constructing()) {
        trackOptimizationOutcome(TrackedOutcome::CantInlineNativeBadForm);
        return InliningStatus_NotInlined;
    }
=======
  MOZ_ASSERT(input->type() == MIRType::Value ||
             input->type() == MIRType::Null ||
             input->type() == MIRType::Boolean || IsNumberType(input->type()));
>>>>>>> upstream-releases

<<<<<<< HEAD
  MDefinition* input = callInfo.getArg(0);
||||||| merged common ancestors
    // These guards are desirable here and in subsequent atomics to
    // avoid bad bailouts with MTruncateToInt32, see https://bugzilla.mozilla.org/show_bug.cgi?id=1141986#c20.
    MDefinition* oldval = callInfo.getArg(2);
    if (oldval->mightBeType(MIRType::Object) || oldval->mightBeType(MIRType::Symbol)) {
        return InliningStatus_NotInlined;
    }
=======
  // Only optimize cases where output is int32
  if (getInlineReturnType() != MIRType::Int32) {
    return InliningStatus_NotInlined;
  }
>>>>>>> upstream-releases

<<<<<<< HEAD
  // Only optimize cases where input contains only number, null or boolean
  if (input->mightBeType(MIRType::Object) ||
      input->mightBeType(MIRType::String) ||
      input->mightBeType(MIRType::Symbol) ||
      input->mightBeType(MIRType::Undefined) || input->mightBeMagicType()) {
    return InliningStatus_NotInlined;
  }
||||||| merged common ancestors
    MDefinition* newval = callInfo.getArg(3);
    if (newval->mightBeType(MIRType::Object) || newval->mightBeType(MIRType::Symbol)) {
        return InliningStatus_NotInlined;
    }
=======
  callInfo.setImplicitlyUsedUnchecked();
>>>>>>> upstream-releases

<<<<<<< HEAD
  MOZ_ASSERT(input->type() == MIRType::Value ||
             input->type() == MIRType::Null ||
             input->type() == MIRType::Boolean || IsNumberType(input->type()));
||||||| merged common ancestors
    Scalar::Type arrayType;
    bool requiresCheck = false;
    if (!atomicsMeetsPreconditions(callInfo, &arrayType, &requiresCheck)) {
        return InliningStatus_NotInlined;
    }
=======
  auto* toInt32 = MToNumberInt32::New(alloc(), callInfo.getArg(0));
  current->add(toInt32);
  current->push(toInt32);
  return InliningStatus_Inlined;
}
>>>>>>> upstream-releases

<<<<<<< HEAD
  // Only optimize cases where output is int32
  if (getInlineReturnType() != MIRType::Int32) {
    return InliningStatus_NotInlined;
  }
||||||| merged common ancestors
    callInfo.setImplicitlyUsedUnchecked();
=======
IonBuilder::InliningResult IonBuilder::inlineToString(CallInfo& callInfo) {
  MOZ_ASSERT(!callInfo.constructing());
  MOZ_ASSERT(callInfo.argc() == 1);
>>>>>>> upstream-releases

<<<<<<< HEAD
  callInfo.setImplicitlyUsedUnchecked();
||||||| merged common ancestors
    MInstruction* elements;
    MDefinition* index;
    atomicsCheckBounds(callInfo, &elements, &index);
=======
  if (getInlineReturnType() != MIRType::String) {
    return InliningStatus_NotInlined;
  }
>>>>>>> upstream-releases

<<<<<<< HEAD
  auto* toInt32 = MToNumberInt32::New(alloc(), callInfo.getArg(0));
  current->add(toInt32);
  current->push(toInt32);
  return InliningStatus_Inlined;
}
||||||| merged common ancestors
    if (requiresCheck) {
        addSharedTypedArrayGuard(callInfo.getArg(0));
    }
=======
  callInfo.setImplicitlyUsedUnchecked();
  MToString* toString = MToString::New(alloc(), callInfo.getArg(0));
  current->add(toString);
  current->push(toString);
  return InliningStatus_Inlined;
}
>>>>>>> upstream-releases

<<<<<<< HEAD
IonBuilder::InliningResult IonBuilder::inlineToString(CallInfo& callInfo) {
  MOZ_ASSERT(!callInfo.constructing());
  MOZ_ASSERT(callInfo.argc() == 1);
||||||| merged common ancestors
    MCompareExchangeTypedArrayElement* cas =
        MCompareExchangeTypedArrayElement::New(alloc(), elements, index, arrayType, oldval, newval);
    cas->setResultType(getInlineReturnType());
    current->add(cas);
    current->push(cas);
=======
IonBuilder::InliningResult IonBuilder::inlineBailout(CallInfo& callInfo) {
  callInfo.setImplicitlyUsedUnchecked();
>>>>>>> upstream-releases

<<<<<<< HEAD
  if (getInlineReturnType() != MIRType::String) {
    return InliningStatus_NotInlined;
  }

  callInfo.setImplicitlyUsedUnchecked();
  MToString* toString = MToString::New(alloc(), callInfo.getArg(0));
  current->add(toString);
  current->push(toString);
  return InliningStatus_Inlined;
||||||| merged common ancestors
    MOZ_TRY(resumeAfter(cas));
    return InliningStatus_Inlined;
=======
  current->add(MBail::New(alloc()));

  MConstant* undefined = MConstant::New(alloc(), UndefinedValue());
  current->add(undefined);
  current->push(undefined);
  return InliningStatus_Inlined;
>>>>>>> upstream-releases
}

<<<<<<< HEAD
IonBuilder::InliningResult IonBuilder::inlineBailout(CallInfo& callInfo) {
  callInfo.setImplicitlyUsedUnchecked();
||||||| merged common ancestors
IonBuilder::InliningResult
IonBuilder::inlineAtomicsExchange(CallInfo& callInfo)
{
    if (callInfo.argc() != 3 || callInfo.constructing()) {
        trackOptimizationOutcome(TrackedOutcome::CantInlineNativeBadForm);
        return InliningStatus_NotInlined;
    }
=======
IonBuilder::InliningResult IonBuilder::inlineAssertFloat32(CallInfo& callInfo) {
  if (callInfo.argc() != 2) {
    return InliningStatus_NotInlined;
  }
>>>>>>> upstream-releases

<<<<<<< HEAD
  current->add(MBail::New(alloc()));
||||||| merged common ancestors
    MDefinition* value = callInfo.getArg(2);
    if (value->mightBeType(MIRType::Object) || value->mightBeType(MIRType::Symbol)) {
        return InliningStatus_NotInlined;
    }
=======
  MDefinition* secondArg = callInfo.getArg(1);
>>>>>>> upstream-releases

<<<<<<< HEAD
  MConstant* undefined = MConstant::New(alloc(), UndefinedValue());
  current->add(undefined);
  current->push(undefined);
  return InliningStatus_Inlined;
}
||||||| merged common ancestors
    Scalar::Type arrayType;
    bool requiresCheck = false;
    if (!atomicsMeetsPreconditions(callInfo, &arrayType, &requiresCheck)) {
        return InliningStatus_NotInlined;
    }
=======
  MOZ_ASSERT(secondArg->type() == MIRType::Boolean);
  MOZ_ASSERT(secondArg->isConstant());
>>>>>>> upstream-releases

<<<<<<< HEAD
IonBuilder::InliningResult IonBuilder::inlineAssertFloat32(CallInfo& callInfo) {
  if (callInfo.argc() != 2) {
    return InliningStatus_NotInlined;
  }
||||||| merged common ancestors
    callInfo.setImplicitlyUsedUnchecked();
=======
  bool mustBeFloat32 = secondArg->toConstant()->toBoolean();
  current->add(MAssertFloat32::New(alloc(), callInfo.getArg(0), mustBeFloat32));
>>>>>>> upstream-releases

<<<<<<< HEAD
  MDefinition* secondArg = callInfo.getArg(1);
||||||| merged common ancestors
    MInstruction* elements;
    MDefinition* index;
    atomicsCheckBounds(callInfo, &elements, &index);
=======
  MConstant* undefined = MConstant::New(alloc(), UndefinedValue());
  current->add(undefined);
  current->push(undefined);
  callInfo.setImplicitlyUsedUnchecked();
  return InliningStatus_Inlined;
}
>>>>>>> upstream-releases

<<<<<<< HEAD
  MOZ_ASSERT(secondArg->type() == MIRType::Boolean);
  MOZ_ASSERT(secondArg->isConstant());
||||||| merged common ancestors
    if (requiresCheck) {
        addSharedTypedArrayGuard(callInfo.getArg(0));
    }
=======
IonBuilder::InliningResult IonBuilder::inlineAssertRecoveredOnBailout(
    CallInfo& callInfo) {
  if (callInfo.argc() != 2) {
    return InliningStatus_NotInlined;
  }
>>>>>>> upstream-releases

<<<<<<< HEAD
  bool mustBeFloat32 = secondArg->toConstant()->toBoolean();
  current->add(MAssertFloat32::New(alloc(), callInfo.getArg(0), mustBeFloat32));
||||||| merged common ancestors
    MInstruction* exchange =
        MAtomicExchangeTypedArrayElement::New(alloc(), elements, index, value, arrayType);
    exchange->setResultType(getInlineReturnType());
    current->add(exchange);
    current->push(exchange);
=======
  // Don't assert for recovered instructions when recovering is disabled.
  if (JitOptions.disableRecoverIns) {
    return InliningStatus_NotInlined;
  }
>>>>>>> upstream-releases

<<<<<<< HEAD
  MConstant* undefined = MConstant::New(alloc(), UndefinedValue());
  current->add(undefined);
  current->push(undefined);
  callInfo.setImplicitlyUsedUnchecked();
  return InliningStatus_Inlined;
||||||| merged common ancestors
    MOZ_TRY(resumeAfter(exchange));
    return InliningStatus_Inlined;
=======
  if (JitOptions.checkRangeAnalysis) {
    // If we are checking the range of all instructions, then the guards
    // inserted by Range Analysis prevent the use of recover
    // instruction. Thus, we just disable these checks.
    current->push(constant(UndefinedValue()));
    callInfo.setImplicitlyUsedUnchecked();
    return InliningStatus_Inlined;
  }

  MDefinition* secondArg = callInfo.getArg(1);

  MOZ_ASSERT(secondArg->type() == MIRType::Boolean);
  MOZ_ASSERT(secondArg->isConstant());

  bool mustBeRecovered = secondArg->toConstant()->toBoolean();
  MAssertRecoveredOnBailout* assert = MAssertRecoveredOnBailout::New(
      alloc(), callInfo.getArg(0), mustBeRecovered);
  current->add(assert);
  current->push(assert);

  // Create an instruction sequence which implies that the argument of the
  // assertRecoveredOnBailout function would be encoded at least in one
  // Snapshot.
  MNop* nop = MNop::New(alloc());
  current->add(nop);
  MOZ_TRY(resumeAfter(nop));
  current->add(MEncodeSnapshot::New(alloc()));

  current->pop();
  current->push(constant(UndefinedValue()));
  callInfo.setImplicitlyUsedUnchecked();
  return InliningStatus_Inlined;
>>>>>>> upstream-releases
}

<<<<<<< HEAD
IonBuilder::InliningResult IonBuilder::inlineAssertRecoveredOnBailout(
    CallInfo& callInfo) {
  if (callInfo.argc() != 2) {
    return InliningStatus_NotInlined;
  }
||||||| merged common ancestors
IonBuilder::InliningResult
IonBuilder::inlineAtomicsLoad(CallInfo& callInfo)
{
    if (callInfo.argc() != 2 || callInfo.constructing()) {
        trackOptimizationOutcome(TrackedOutcome::CantInlineNativeBadForm);
        return InliningStatus_NotInlined;
    }
=======
IonBuilder::InliningResult IonBuilder::inlineAtomicsCompareExchange(
    CallInfo& callInfo) {
  if (callInfo.argc() != 4 || callInfo.constructing()) {
    trackOptimizationOutcome(TrackedOutcome::CantInlineNativeBadForm);
    return InliningStatus_NotInlined;
  }

  // These guards are desirable here and in subsequent atomics to
  // avoid bad bailouts with MTruncateToInt32, see
  // https://bugzilla.mozilla.org/show_bug.cgi?id=1141986#c20.
  MDefinition* oldval = callInfo.getArg(2);
  if (oldval->mightBeType(MIRType::Object) ||
      oldval->mightBeType(MIRType::Symbol) ||
      oldval->mightBeType(MIRType::BigInt)) {
    return InliningStatus_NotInlined;
  }
>>>>>>> upstream-releases

<<<<<<< HEAD
  // Don't assert for recovered instructions when recovering is disabled.
  if (JitOptions.disableRecoverIns) {
    return InliningStatus_NotInlined;
  }
||||||| merged common ancestors
    Scalar::Type arrayType;
    bool requiresCheck = false;
    if (!atomicsMeetsPreconditions(callInfo, &arrayType, &requiresCheck)) {
        return InliningStatus_NotInlined;
    }
=======
  MDefinition* newval = callInfo.getArg(3);
  if (newval->mightBeType(MIRType::Object) ||
      newval->mightBeType(MIRType::Symbol) ||
      newval->mightBeType(MIRType::BigInt)) {
    return InliningStatus_NotInlined;
  }
>>>>>>> upstream-releases

<<<<<<< HEAD
  if (JitOptions.checkRangeAnalysis) {
    // If we are checking the range of all instructions, then the guards
    // inserted by Range Analysis prevent the use of recover
    // instruction. Thus, we just disable these checks.
    current->push(constant(UndefinedValue()));
    callInfo.setImplicitlyUsedUnchecked();
    return InliningStatus_Inlined;
  }
||||||| merged common ancestors
    callInfo.setImplicitlyUsedUnchecked();
=======
  Scalar::Type arrayType;
  bool requiresCheck = false;
  if (!atomicsMeetsPreconditions(callInfo, &arrayType, &requiresCheck)) {
    return InliningStatus_NotInlined;
  }
>>>>>>> upstream-releases

<<<<<<< HEAD
  MDefinition* secondArg = callInfo.getArg(1);
||||||| merged common ancestors
    MInstruction* elements;
    MDefinition* index;
    atomicsCheckBounds(callInfo, &elements, &index);
=======
  callInfo.setImplicitlyUsedUnchecked();
>>>>>>> upstream-releases

<<<<<<< HEAD
  MOZ_ASSERT(secondArg->type() == MIRType::Boolean);
  MOZ_ASSERT(secondArg->isConstant());
||||||| merged common ancestors
    if (requiresCheck) {
        addSharedTypedArrayGuard(callInfo.getArg(0));
    }
=======
  MInstruction* elements;
  MDefinition* index;
  atomicsCheckBounds(callInfo, &elements, &index);
>>>>>>> upstream-releases

<<<<<<< HEAD
  bool mustBeRecovered = secondArg->toConstant()->toBoolean();
  MAssertRecoveredOnBailout* assert = MAssertRecoveredOnBailout::New(
      alloc(), callInfo.getArg(0), mustBeRecovered);
  current->add(assert);
  current->push(assert);
||||||| merged common ancestors
    MLoadUnboxedScalar* load =
        MLoadUnboxedScalar::New(alloc(), elements, index, arrayType,
                                DoesRequireMemoryBarrier);
    load->setResultType(getInlineReturnType());
    current->add(load);
    current->push(load);
=======
  if (requiresCheck) {
    addSharedTypedArrayGuard(callInfo.getArg(0));
  }
>>>>>>> upstream-releases

<<<<<<< HEAD
  // Create an instruction sequence which implies that the argument of the
  // assertRecoveredOnBailout function would be encoded at least in one
  // Snapshot.
  MNop* nop = MNop::New(alloc());
  current->add(nop);
  MOZ_TRY(resumeAfter(nop));
  current->add(MEncodeSnapshot::New(alloc()));
||||||| merged common ancestors
    // Loads are considered effectful (they execute a memory barrier).
    MOZ_TRY(resumeAfter(load));
    return InliningStatus_Inlined;
}
=======
  MCompareExchangeTypedArrayElement* cas =
      MCompareExchangeTypedArrayElement::New(alloc(), elements, index,
                                             arrayType, oldval, newval);
  cas->setResultType(getInlineReturnType());
  current->add(cas);
  current->push(cas);
>>>>>>> upstream-releases

<<<<<<< HEAD
  current->pop();
  current->push(constant(UndefinedValue()));
  callInfo.setImplicitlyUsedUnchecked();
  return InliningStatus_Inlined;
}
||||||| merged common ancestors
IonBuilder::InliningResult
IonBuilder::inlineAtomicsStore(CallInfo& callInfo)
{
    if (callInfo.argc() != 3 || callInfo.constructing()) {
        trackOptimizationOutcome(TrackedOutcome::CantInlineNativeBadForm);
        return InliningStatus_NotInlined;
    }
=======
  MOZ_TRY(resumeAfter(cas));
  return InliningStatus_Inlined;
}
>>>>>>> upstream-releases

<<<<<<< HEAD
IonBuilder::InliningResult IonBuilder::inlineAtomicsCompareExchange(
    CallInfo& callInfo) {
  if (callInfo.argc() != 4 || callInfo.constructing()) {
    trackOptimizationOutcome(TrackedOutcome::CantInlineNativeBadForm);
    return InliningStatus_NotInlined;
  }

  // These guards are desirable here and in subsequent atomics to
  // avoid bad bailouts with MTruncateToInt32, see
  // https://bugzilla.mozilla.org/show_bug.cgi?id=1141986#c20.
  MDefinition* oldval = callInfo.getArg(2);
  if (oldval->mightBeType(MIRType::Object) ||
      oldval->mightBeType(MIRType::Symbol)) {
    return InliningStatus_NotInlined;
  }
||||||| merged common ancestors
    // Atomics.store() is annoying because it returns the result of converting
    // the value by ToInteger(), not the input value, nor the result of
    // converting the value by ToInt32().  It is especially annoying because
    // almost nobody uses the result value.
    //
    // As an expedient compromise, therefore, we inline only if the result is
    // obviously unused or if the argument is already Int32 and thus requires no
    // conversion.

    MDefinition* value = callInfo.getArg(2);
    if (!BytecodeIsPopped(pc) && value->type() != MIRType::Int32) {
        trackOptimizationOutcome(TrackedOutcome::CantInlineNativeBadType);
        return InliningStatus_NotInlined;
    }
=======
IonBuilder::InliningResult IonBuilder::inlineAtomicsExchange(
    CallInfo& callInfo) {
  if (callInfo.argc() != 3 || callInfo.constructing()) {
    trackOptimizationOutcome(TrackedOutcome::CantInlineNativeBadForm);
    return InliningStatus_NotInlined;
  }
>>>>>>> upstream-releases

<<<<<<< HEAD
  MDefinition* newval = callInfo.getArg(3);
  if (newval->mightBeType(MIRType::Object) ||
      newval->mightBeType(MIRType::Symbol)) {
    return InliningStatus_NotInlined;
  }
||||||| merged common ancestors
    if (value->mightBeType(MIRType::Object) || value->mightBeType(MIRType::Symbol)) {
        return InliningStatus_NotInlined;
    }
=======
  MDefinition* value = callInfo.getArg(2);
  if (value->mightBeType(MIRType::Object) ||
      value->mightBeType(MIRType::Symbol) ||
      value->mightBeType(MIRType::BigInt)) {
    return InliningStatus_NotInlined;
  }
>>>>>>> upstream-releases

  Scalar::Type arrayType;
  bool requiresCheck = false;
  if (!atomicsMeetsPreconditions(callInfo, &arrayType, &requiresCheck)) {
    return InliningStatus_NotInlined;
  }

  callInfo.setImplicitlyUsedUnchecked();

  MInstruction* elements;
  MDefinition* index;
  atomicsCheckBounds(callInfo, &elements, &index);

  if (requiresCheck) {
    addSharedTypedArrayGuard(callInfo.getArg(0));
  }

<<<<<<< HEAD
  MCompareExchangeTypedArrayElement* cas =
      MCompareExchangeTypedArrayElement::New(alloc(), elements, index,
                                             arrayType, oldval, newval);
  cas->setResultType(getInlineReturnType());
  current->add(cas);
  current->push(cas);
||||||| merged common ancestors
    MDefinition* toWrite = value;
    if (toWrite->type() != MIRType::Int32) {
        toWrite = MTruncateToInt32::New(alloc(), toWrite);
        current->add(toWrite->toInstruction());
    }
    MStoreUnboxedScalar* store =
        MStoreUnboxedScalar::New(alloc(), elements, index, toWrite, arrayType,
                                 MStoreUnboxedScalar::TruncateInput, DoesRequireMemoryBarrier);
    current->add(store);
    current->push(value);       // Either Int32 or not used; in either case correct
=======
  MInstruction* exchange = MAtomicExchangeTypedArrayElement::New(
      alloc(), elements, index, value, arrayType);
  exchange->setResultType(getInlineReturnType());
  current->add(exchange);
  current->push(exchange);
>>>>>>> upstream-releases

<<<<<<< HEAD
  MOZ_TRY(resumeAfter(cas));
  return InliningStatus_Inlined;
||||||| merged common ancestors
    MOZ_TRY(resumeAfter(store));
    return InliningStatus_Inlined;
=======
  MOZ_TRY(resumeAfter(exchange));
  return InliningStatus_Inlined;
>>>>>>> upstream-releases
}

<<<<<<< HEAD
IonBuilder::InliningResult IonBuilder::inlineAtomicsExchange(
    CallInfo& callInfo) {
  if (callInfo.argc() != 3 || callInfo.constructing()) {
    trackOptimizationOutcome(TrackedOutcome::CantInlineNativeBadForm);
    return InliningStatus_NotInlined;
  }
||||||| merged common ancestors
IonBuilder::InliningResult
IonBuilder::inlineAtomicsBinop(CallInfo& callInfo, InlinableNative target)
{
    if (callInfo.argc() != 3 || callInfo.constructing()) {
        trackOptimizationOutcome(TrackedOutcome::CantInlineNativeBadForm);
        return InliningStatus_NotInlined;
    }

    MDefinition* value = callInfo.getArg(2);
    if (value->mightBeType(MIRType::Object) || value->mightBeType(MIRType::Symbol)) {
        return InliningStatus_NotInlined;
    }
=======
IonBuilder::InliningResult IonBuilder::inlineAtomicsLoad(CallInfo& callInfo) {
  if (callInfo.argc() != 2 || callInfo.constructing()) {
    trackOptimizationOutcome(TrackedOutcome::CantInlineNativeBadForm);
    return InliningStatus_NotInlined;
  }

  Scalar::Type arrayType;
  bool requiresCheck = false;
  if (!atomicsMeetsPreconditions(callInfo, &arrayType, &requiresCheck)) {
    return InliningStatus_NotInlined;
  }
>>>>>>> upstream-releases

<<<<<<< HEAD
  MDefinition* value = callInfo.getArg(2);
  if (value->mightBeType(MIRType::Object) ||
      value->mightBeType(MIRType::Symbol)) {
    return InliningStatus_NotInlined;
  }
||||||| merged common ancestors
    Scalar::Type arrayType;
    bool requiresCheck = false;
    if (!atomicsMeetsPreconditions(callInfo, &arrayType, &requiresCheck)) {
        return InliningStatus_NotInlined;
    }
=======
  callInfo.setImplicitlyUsedUnchecked();
>>>>>>> upstream-releases

<<<<<<< HEAD
  Scalar::Type arrayType;
  bool requiresCheck = false;
  if (!atomicsMeetsPreconditions(callInfo, &arrayType, &requiresCheck)) {
    return InliningStatus_NotInlined;
  }
||||||| merged common ancestors
    callInfo.setImplicitlyUsedUnchecked();
=======
  MInstruction* elements;
  MDefinition* index;
  atomicsCheckBounds(callInfo, &elements, &index);
>>>>>>> upstream-releases

<<<<<<< HEAD
  callInfo.setImplicitlyUsedUnchecked();
||||||| merged common ancestors
    if (requiresCheck) {
        addSharedTypedArrayGuard(callInfo.getArg(0));
    }
=======
  if (requiresCheck) {
    addSharedTypedArrayGuard(callInfo.getArg(0));
  }
>>>>>>> upstream-releases

<<<<<<< HEAD
  MInstruction* elements;
  MDefinition* index;
  atomicsCheckBounds(callInfo, &elements, &index);
||||||| merged common ancestors
    MInstruction* elements;
    MDefinition* index;
    atomicsCheckBounds(callInfo, &elements, &index);
=======
  MLoadUnboxedScalar* load = MLoadUnboxedScalar::New(
      alloc(), elements, index, arrayType, DoesRequireMemoryBarrier);
  load->setResultType(getInlineReturnType());
  current->add(load);
  current->push(load);
>>>>>>> upstream-releases

<<<<<<< HEAD
  if (requiresCheck) {
    addSharedTypedArrayGuard(callInfo.getArg(0));
  }
||||||| merged common ancestors
    AtomicOp k = AtomicFetchAddOp;
    switch (target) {
      case InlinableNative::AtomicsAdd:
        k = AtomicFetchAddOp;
        break;
      case InlinableNative::AtomicsSub:
        k = AtomicFetchSubOp;
        break;
      case InlinableNative::AtomicsAnd:
        k = AtomicFetchAndOp;
        break;
      case InlinableNative::AtomicsOr:
        k = AtomicFetchOrOp;
        break;
      case InlinableNative::AtomicsXor:
        k = AtomicFetchXorOp;
        break;
      default:
        MOZ_CRASH("Bad atomic operation");
    }
=======
  // Loads are considered effectful (they execute a memory barrier).
  MOZ_TRY(resumeAfter(load));
  return InliningStatus_Inlined;
}
>>>>>>> upstream-releases

<<<<<<< HEAD
  MInstruction* exchange = MAtomicExchangeTypedArrayElement::New(
      alloc(), elements, index, value, arrayType);
  exchange->setResultType(getInlineReturnType());
  current->add(exchange);
  current->push(exchange);
||||||| merged common ancestors
    MAtomicTypedArrayElementBinop* binop =
        MAtomicTypedArrayElementBinop::New(alloc(), k, elements, index, arrayType, value);
    binop->setResultType(getInlineReturnType());
    current->add(binop);
    current->push(binop);
=======
IonBuilder::InliningResult IonBuilder::inlineAtomicsStore(CallInfo& callInfo) {
  if (callInfo.argc() != 3 || callInfo.constructing()) {
    trackOptimizationOutcome(TrackedOutcome::CantInlineNativeBadForm);
    return InliningStatus_NotInlined;
  }

  // Atomics.store() is annoying because it returns the result of converting
  // the value by ToInteger(), not the input value, nor the result of
  // converting the value by ToInt32().  It is especially annoying because
  // almost nobody uses the result value.
  //
  // As an expedient compromise, therefore, we inline only if the result is
  // obviously unused or if the argument is already Int32 and thus requires no
  // conversion.

  MDefinition* value = callInfo.getArg(2);
  if (!BytecodeIsPopped(pc) && value->type() != MIRType::Int32) {
    trackOptimizationOutcome(TrackedOutcome::CantInlineNativeBadType);
    return InliningStatus_NotInlined;
  }
>>>>>>> upstream-releases

<<<<<<< HEAD
  MOZ_TRY(resumeAfter(exchange));
  return InliningStatus_Inlined;
}
||||||| merged common ancestors
    MOZ_TRY(resumeAfter(binop));
    return InliningStatus_Inlined;
}
=======
  if (value->mightBeType(MIRType::Object) ||
      value->mightBeType(MIRType::Symbol) ||
      value->mightBeType(MIRType::BigInt)) {
    return InliningStatus_NotInlined;
  }
>>>>>>> upstream-releases

<<<<<<< HEAD
IonBuilder::InliningResult IonBuilder::inlineAtomicsLoad(CallInfo& callInfo) {
  if (callInfo.argc() != 2 || callInfo.constructing()) {
    trackOptimizationOutcome(TrackedOutcome::CantInlineNativeBadForm);
    return InliningStatus_NotInlined;
  }
||||||| merged common ancestors
IonBuilder::InliningResult
IonBuilder::inlineAtomicsIsLockFree(CallInfo& callInfo)
{
    if (callInfo.argc() != 1 || callInfo.constructing()) {
        trackOptimizationOutcome(TrackedOutcome::CantInlineNativeBadForm);
        return InliningStatus_NotInlined;
    }
=======
  Scalar::Type arrayType;
  bool requiresCheck = false;
  if (!atomicsMeetsPreconditions(callInfo, &arrayType, &requiresCheck,
                                 DontCheckAtomicResult)) {
    return InliningStatus_NotInlined;
  }

  callInfo.setImplicitlyUsedUnchecked();

  MInstruction* elements;
  MDefinition* index;
  atomicsCheckBounds(callInfo, &elements, &index);

  if (requiresCheck) {
    addSharedTypedArrayGuard(callInfo.getArg(0));
  }

  MDefinition* toWrite = value;
  if (toWrite->type() != MIRType::Int32) {
    toWrite = MTruncateToInt32::New(alloc(), toWrite);
    current->add(toWrite->toInstruction());
  }
  MStoreUnboxedScalar* store = MStoreUnboxedScalar::New(
      alloc(), elements, index, toWrite, arrayType,
      MStoreUnboxedScalar::TruncateInput, DoesRequireMemoryBarrier);
  current->add(store);
  current->push(value);  // Either Int32 or not used; in either case correct

  MOZ_TRY(resumeAfter(store));
  return InliningStatus_Inlined;
}
>>>>>>> upstream-releases

<<<<<<< HEAD
  Scalar::Type arrayType;
  bool requiresCheck = false;
  if (!atomicsMeetsPreconditions(callInfo, &arrayType, &requiresCheck)) {
    return InliningStatus_NotInlined;
  }
||||||| merged common ancestors
    callInfo.setImplicitlyUsedUnchecked();
=======
IonBuilder::InliningResult IonBuilder::inlineAtomicsBinop(
    CallInfo& callInfo, InlinableNative target) {
  if (callInfo.argc() != 3 || callInfo.constructing()) {
    trackOptimizationOutcome(TrackedOutcome::CantInlineNativeBadForm);
    return InliningStatus_NotInlined;
  }
>>>>>>> upstream-releases

<<<<<<< HEAD
  callInfo.setImplicitlyUsedUnchecked();
||||||| merged common ancestors
    MAtomicIsLockFree* ilf =
        MAtomicIsLockFree::New(alloc(), callInfo.getArg(0));
    current->add(ilf);
    current->push(ilf);
=======
  MDefinition* value = callInfo.getArg(2);
  if (value->mightBeType(MIRType::Object) ||
      value->mightBeType(MIRType::Symbol) ||
      value->mightBeType(MIRType::BigInt)) {
    return InliningStatus_NotInlined;
  }
>>>>>>> upstream-releases

<<<<<<< HEAD
  MInstruction* elements;
  MDefinition* index;
  atomicsCheckBounds(callInfo, &elements, &index);

  if (requiresCheck) {
    addSharedTypedArrayGuard(callInfo.getArg(0));
  }

  MLoadUnboxedScalar* load = MLoadUnboxedScalar::New(
      alloc(), elements, index, arrayType, DoesRequireMemoryBarrier);
  load->setResultType(getInlineReturnType());
  current->add(load);
  current->push(load);

  // Loads are considered effectful (they execute a memory barrier).
  MOZ_TRY(resumeAfter(load));
  return InliningStatus_Inlined;
||||||| merged common ancestors
    return InliningStatus_Inlined;
=======
  Scalar::Type arrayType;
  bool requiresCheck = false;
  if (!atomicsMeetsPreconditions(callInfo, &arrayType, &requiresCheck)) {
    return InliningStatus_NotInlined;
  }

  callInfo.setImplicitlyUsedUnchecked();

  if (requiresCheck) {
    addSharedTypedArrayGuard(callInfo.getArg(0));
  }

  MInstruction* elements;
  MDefinition* index;
  atomicsCheckBounds(callInfo, &elements, &index);

  AtomicOp k = AtomicFetchAddOp;
  switch (target) {
    case InlinableNative::AtomicsAdd:
      k = AtomicFetchAddOp;
      break;
    case InlinableNative::AtomicsSub:
      k = AtomicFetchSubOp;
      break;
    case InlinableNative::AtomicsAnd:
      k = AtomicFetchAndOp;
      break;
    case InlinableNative::AtomicsOr:
      k = AtomicFetchOrOp;
      break;
    case InlinableNative::AtomicsXor:
      k = AtomicFetchXorOp;
      break;
    default:
      MOZ_CRASH("Bad atomic operation");
  }

  MAtomicTypedArrayElementBinop* binop = MAtomicTypedArrayElementBinop::New(
      alloc(), k, elements, index, arrayType, value);
  binop->setResultType(getInlineReturnType());
  current->add(binop);
  current->push(binop);

  MOZ_TRY(resumeAfter(binop));
  return InliningStatus_Inlined;
>>>>>>> upstream-releases
}

<<<<<<< HEAD
IonBuilder::InliningResult IonBuilder::inlineAtomicsStore(CallInfo& callInfo) {
  if (callInfo.argc() != 3 || callInfo.constructing()) {
    trackOptimizationOutcome(TrackedOutcome::CantInlineNativeBadForm);
    return InliningStatus_NotInlined;
  }

  // Atomics.store() is annoying because it returns the result of converting
  // the value by ToInteger(), not the input value, nor the result of
  // converting the value by ToInt32().  It is especially annoying because
  // almost nobody uses the result value.
  //
  // As an expedient compromise, therefore, we inline only if the result is
  // obviously unused or if the argument is already Int32 and thus requires no
  // conversion.

  MDefinition* value = callInfo.getArg(2);
  if (!BytecodeIsPopped(pc) && value->type() != MIRType::Int32) {
    trackOptimizationOutcome(TrackedOutcome::CantInlineNativeBadType);
    return InliningStatus_NotInlined;
  }

  if (value->mightBeType(MIRType::Object) ||
      value->mightBeType(MIRType::Symbol)) {
    return InliningStatus_NotInlined;
  }
||||||| merged common ancestors
bool
IonBuilder::atomicsMeetsPreconditions(CallInfo& callInfo, Scalar::Type* arrayType,
                                      bool* requiresTagCheck, AtomicCheckResult checkResult)
{
    if (!JitSupportsAtomics()) {
        return false;
    }

    if (callInfo.getArg(0)->type() != MIRType::Object) {
        return false;
    }
=======
IonBuilder::InliningResult IonBuilder::inlineAtomicsIsLockFree(
    CallInfo& callInfo) {
  if (callInfo.argc() != 1 || callInfo.constructing()) {
    trackOptimizationOutcome(TrackedOutcome::CantInlineNativeBadForm);
    return InliningStatus_NotInlined;
  }
>>>>>>> upstream-releases

<<<<<<< HEAD
  Scalar::Type arrayType;
  bool requiresCheck = false;
  if (!atomicsMeetsPreconditions(callInfo, &arrayType, &requiresCheck,
                                 DontCheckAtomicResult)) {
    return InliningStatus_NotInlined;
  }
||||||| merged common ancestors
    if (callInfo.getArg(1)->type() != MIRType::Int32) {
        return false;
    }
=======
  callInfo.setImplicitlyUsedUnchecked();
>>>>>>> upstream-releases

<<<<<<< HEAD
  callInfo.setImplicitlyUsedUnchecked();
||||||| merged common ancestors
    // Ensure that the first argument is a TypedArray that maps shared
    // memory.
    //
    // Then check both that the element type is something we can
    // optimize and that the return type is suitable for that element
    // type.
=======
  MAtomicIsLockFree* ilf = MAtomicIsLockFree::New(alloc(), callInfo.getArg(0));
  current->add(ilf);
  current->push(ilf);
>>>>>>> upstream-releases

<<<<<<< HEAD
  MInstruction* elements;
  MDefinition* index;
  atomicsCheckBounds(callInfo, &elements, &index);
||||||| merged common ancestors
    TemporaryTypeSet* arg0Types = callInfo.getArg(0)->resultTypeSet();
    if (!arg0Types) {
        return false;
    }
=======
  return InliningStatus_Inlined;
}
>>>>>>> upstream-releases

<<<<<<< HEAD
  if (requiresCheck) {
    addSharedTypedArrayGuard(callInfo.getArg(0));
  }

  MDefinition* toWrite = value;
  if (toWrite->type() != MIRType::Int32) {
    toWrite = MTruncateToInt32::New(alloc(), toWrite);
    current->add(toWrite->toInstruction());
  }
  MStoreUnboxedScalar* store = MStoreUnboxedScalar::New(
      alloc(), elements, index, toWrite, arrayType,
      MStoreUnboxedScalar::TruncateInput, DoesRequireMemoryBarrier);
  current->add(store);
  current->push(value);  // Either Int32 or not used; in either case correct
||||||| merged common ancestors
    TemporaryTypeSet::TypedArraySharedness sharedness = TemporaryTypeSet::UnknownSharedness;
    *arrayType = arg0Types->getTypedArrayType(constraints(), &sharedness);
    *requiresTagCheck = sharedness != TemporaryTypeSet::KnownShared;
    switch (*arrayType) {
      case Scalar::Int8:
      case Scalar::Uint8:
      case Scalar::Int16:
      case Scalar::Uint16:
      case Scalar::Int32:
        return checkResult == DontCheckAtomicResult || getInlineReturnType() == MIRType::Int32;
      case Scalar::Uint32:
        // Bug 1077305: it would be attractive to allow inlining even
        // if the inline return type is Int32, which it will frequently
        // be.
        return checkResult == DontCheckAtomicResult || getInlineReturnType() == MIRType::Double;
      default:
        // Excludes floating types and Uint8Clamped.
        return false;
    }
}
=======
bool IonBuilder::atomicsMeetsPreconditions(CallInfo& callInfo,
                                           Scalar::Type* arrayType,
                                           bool* requiresTagCheck,
                                           AtomicCheckResult checkResult) {
  if (!JitSupportsAtomics()) {
    return false;
  }

  if (callInfo.getArg(0)->type() != MIRType::Object) {
    return false;
  }

  if (callInfo.getArg(1)->type() != MIRType::Int32) {
    return false;
  }

  // Ensure that the first argument is a TypedArray that maps shared
  // memory.
  //
  // Then check both that the element type is something we can
  // optimize and that the return type is suitable for that element
  // type.

  TemporaryTypeSet* arg0Types = callInfo.getArg(0)->resultTypeSet();
  if (!arg0Types) {
    return false;
  }

  TemporaryTypeSet::TypedArraySharedness sharedness =
      TemporaryTypeSet::UnknownSharedness;
  *arrayType = arg0Types->getTypedArrayType(constraints(), &sharedness);
  *requiresTagCheck = sharedness != TemporaryTypeSet::KnownShared;
  switch (*arrayType) {
    case Scalar::Int8:
    case Scalar::Uint8:
    case Scalar::Int16:
    case Scalar::Uint16:
    case Scalar::Int32:
      return checkResult == DontCheckAtomicResult ||
             getInlineReturnType() == MIRType::Int32;
    case Scalar::Uint32:
      // Bug 1077305: it would be attractive to allow inlining even
      // if the inline return type is Int32, which it will frequently
      // be.
      return checkResult == DontCheckAtomicResult ||
             getInlineReturnType() == MIRType::Double;
    default:
      // Excludes floating types and Uint8Clamped.
      return false;
  }
}
>>>>>>> upstream-releases

<<<<<<< HEAD
  MOZ_TRY(resumeAfter(store));
  return InliningStatus_Inlined;
||||||| merged common ancestors
void
IonBuilder::atomicsCheckBounds(CallInfo& callInfo, MInstruction** elements, MDefinition** index)
{
    // Perform bounds checking and extract the elements vector.
    MDefinition* obj = callInfo.getArg(0);
    MInstruction* length = nullptr;
    *index = callInfo.getArg(1);
    *elements = nullptr;
    addTypedArrayLengthAndData(obj, DoBoundsCheck, index, &length, elements);
=======
void IonBuilder::atomicsCheckBounds(CallInfo& callInfo, MInstruction** elements,
                                    MDefinition** index) {
  // Perform bounds checking and extract the elements vector.
  MDefinition* obj = callInfo.getArg(0);
  MInstruction* length = nullptr;
  *index = callInfo.getArg(1);
  *elements = nullptr;
  addTypedArrayLengthAndData(obj, DoBoundsCheck, index, &length, elements);
>>>>>>> upstream-releases
}

<<<<<<< HEAD
IonBuilder::InliningResult IonBuilder::inlineAtomicsBinop(
    CallInfo& callInfo, InlinableNative target) {
  if (callInfo.argc() != 3 || callInfo.constructing()) {
    trackOptimizationOutcome(TrackedOutcome::CantInlineNativeBadForm);
    return InliningStatus_NotInlined;
  }
||||||| merged common ancestors
IonBuilder::InliningResult
IonBuilder::inlineIsConstructing(CallInfo& callInfo)
{
    MOZ_ASSERT(!callInfo.constructing());
    MOZ_ASSERT(callInfo.argc() == 0);
    MOZ_ASSERT(script()->functionNonDelazifying(),
               "isConstructing() should only be called in function scripts");

    if (getInlineReturnType() != MIRType::Boolean) {
        return InliningStatus_NotInlined;
    }
=======
IonBuilder::InliningResult IonBuilder::inlineIsConstructing(
    CallInfo& callInfo) {
  MOZ_ASSERT(!callInfo.constructing());
  MOZ_ASSERT(callInfo.argc() == 0);
  MOZ_ASSERT(script()->functionNonDelazifying(),
             "isConstructing() should only be called in function scripts");
>>>>>>> upstream-releases

<<<<<<< HEAD
  MDefinition* value = callInfo.getArg(2);
  if (value->mightBeType(MIRType::Object) ||
      value->mightBeType(MIRType::Symbol)) {
    return InliningStatus_NotInlined;
  }
||||||| merged common ancestors
    callInfo.setImplicitlyUsedUnchecked();
=======
  if (getInlineReturnType() != MIRType::Boolean) {
    return InliningStatus_NotInlined;
  }
>>>>>>> upstream-releases

<<<<<<< HEAD
  Scalar::Type arrayType;
  bool requiresCheck = false;
  if (!atomicsMeetsPreconditions(callInfo, &arrayType, &requiresCheck)) {
    return InliningStatus_NotInlined;
  }

  callInfo.setImplicitlyUsedUnchecked();

  if (requiresCheck) {
    addSharedTypedArrayGuard(callInfo.getArg(0));
  }

  MInstruction* elements;
  MDefinition* index;
  atomicsCheckBounds(callInfo, &elements, &index);

  AtomicOp k = AtomicFetchAddOp;
  switch (target) {
    case InlinableNative::AtomicsAdd:
      k = AtomicFetchAddOp;
      break;
    case InlinableNative::AtomicsSub:
      k = AtomicFetchSubOp;
      break;
    case InlinableNative::AtomicsAnd:
      k = AtomicFetchAndOp;
      break;
    case InlinableNative::AtomicsOr:
      k = AtomicFetchOrOp;
      break;
    case InlinableNative::AtomicsXor:
      k = AtomicFetchXorOp;
      break;
    default:
      MOZ_CRASH("Bad atomic operation");
  }

  MAtomicTypedArrayElementBinop* binop = MAtomicTypedArrayElementBinop::New(
      alloc(), k, elements, index, arrayType, value);
  binop->setResultType(getInlineReturnType());
  current->add(binop);
  current->push(binop);

  MOZ_TRY(resumeAfter(binop));
  return InliningStatus_Inlined;
}

IonBuilder::InliningResult IonBuilder::inlineAtomicsIsLockFree(
    CallInfo& callInfo) {
  if (callInfo.argc() != 1 || callInfo.constructing()) {
    trackOptimizationOutcome(TrackedOutcome::CantInlineNativeBadForm);
    return InliningStatus_NotInlined;
  }

  callInfo.setImplicitlyUsedUnchecked();

  MAtomicIsLockFree* ilf = MAtomicIsLockFree::New(alloc(), callInfo.getArg(0));
  current->add(ilf);
  current->push(ilf);

  return InliningStatus_Inlined;
}

bool IonBuilder::atomicsMeetsPreconditions(CallInfo& callInfo,
                                           Scalar::Type* arrayType,
                                           bool* requiresTagCheck,
                                           AtomicCheckResult checkResult) {
  if (!JitSupportsAtomics()) {
    return false;
  }

  if (callInfo.getArg(0)->type() != MIRType::Object) {
    return false;
  }

  if (callInfo.getArg(1)->type() != MIRType::Int32) {
    return false;
  }

  // Ensure that the first argument is a TypedArray that maps shared
  // memory.
  //
  // Then check both that the element type is something we can
  // optimize and that the return type is suitable for that element
  // type.

  TemporaryTypeSet* arg0Types = callInfo.getArg(0)->resultTypeSet();
  if (!arg0Types) {
    return false;
  }

  TemporaryTypeSet::TypedArraySharedness sharedness =
      TemporaryTypeSet::UnknownSharedness;
  *arrayType = arg0Types->getTypedArrayType(constraints(), &sharedness);
  *requiresTagCheck = sharedness != TemporaryTypeSet::KnownShared;
  switch (*arrayType) {
    case Scalar::Int8:
    case Scalar::Uint8:
    case Scalar::Int16:
    case Scalar::Uint16:
    case Scalar::Int32:
      return checkResult == DontCheckAtomicResult ||
             getInlineReturnType() == MIRType::Int32;
    case Scalar::Uint32:
      // Bug 1077305: it would be attractive to allow inlining even
      // if the inline return type is Int32, which it will frequently
      // be.
      return checkResult == DontCheckAtomicResult ||
             getInlineReturnType() == MIRType::Double;
    default:
      // Excludes floating types and Uint8Clamped.
      return false;
  }
}

void IonBuilder::atomicsCheckBounds(CallInfo& callInfo, MInstruction** elements,
                                    MDefinition** index) {
  // Perform bounds checking and extract the elements vector.
  MDefinition* obj = callInfo.getArg(0);
  MInstruction* length = nullptr;
  *index = callInfo.getArg(1);
  *elements = nullptr;
  addTypedArrayLengthAndData(obj, DoBoundsCheck, index, &length, elements);
}

IonBuilder::InliningResult IonBuilder::inlineIsConstructing(
    CallInfo& callInfo) {
  MOZ_ASSERT(!callInfo.constructing());
  MOZ_ASSERT(callInfo.argc() == 0);
  MOZ_ASSERT(script()->functionNonDelazifying(),
             "isConstructing() should only be called in function scripts");

  if (getInlineReturnType() != MIRType::Boolean) {
    return InliningStatus_NotInlined;
  }
||||||| merged common ancestors
    if (inliningDepth_ == 0) {
        MInstruction* ins = MIsConstructing::New(alloc());
        current->add(ins);
        current->push(ins);
        return InliningStatus_Inlined;
    }
=======
  callInfo.setImplicitlyUsedUnchecked();
>>>>>>> upstream-releases

<<<<<<< HEAD
  callInfo.setImplicitlyUsedUnchecked();

  if (inliningDepth_ == 0) {
    MInstruction* ins = MIsConstructing::New(alloc());
    current->add(ins);
    current->push(ins);
||||||| merged common ancestors
    bool constructing = inlineCallInfo_->constructing();
    pushConstant(BooleanValue(constructing));
=======
  if (inliningDepth_ == 0) {
    MInstruction* ins = MIsConstructing::New(alloc());
    current->add(ins);
    current->push(ins);
>>>>>>> upstream-releases
    return InliningStatus_Inlined;
  }

  bool constructing = inlineCallInfo_->constructing();
  pushConstant(BooleanValue(constructing));
  return InliningStatus_Inlined;
}

IonBuilder::InliningResult IonBuilder::inlineConstructTypedObject(
    CallInfo& callInfo, TypeDescr* descr) {
  // Only inline default constructors for now.
  if (callInfo.argc() != 0) {
    trackOptimizationOutcome(TrackedOutcome::CantInlineNativeBadForm);
    return InliningStatus_NotInlined;
  }

  if (!InlineTypedObject::canAccommodateType(descr)) {
    return InliningStatus_NotInlined;
  }

  JSObject* obj =
      inspector->getTemplateObjectForClassHook(pc, descr->getClass());
  if (!obj || !obj->is<InlineTypedObject>()) {
    return InliningStatus_NotInlined;
  }

  InlineTypedObject* templateObject = &obj->as<InlineTypedObject>();
  if (&templateObject->typeDescr() != descr) {
    return InliningStatus_NotInlined;
  }

  callInfo.setImplicitlyUsedUnchecked();

  MNewTypedObject* ins =
      MNewTypedObject::New(alloc(), constraints(), templateObject,
                           templateObject->group()->initialHeap(constraints()));
  current->add(ins);
  current->push(ins);

  return InliningStatus_Inlined;
}

<<<<<<< HEAD
IonBuilder::InliningResult IonBuilder::inlineWasmCall(CallInfo& callInfo,
                                                      JSFunction* target) {
  MOZ_ASSERT(target->isWasmOptimized());
  MOZ_ASSERT(target->realm() == script()->realm());
||||||| merged common ancestors
IonBuilder::InliningResult
IonBuilder::inlineWasmCall(CallInfo& callInfo, JSFunction* target)
{
    MOZ_ASSERT(target->isWasmOptimized());
    MOZ_ASSERT(target->realm() == script()->realm());

    // Don't inline wasm constructors.
    if (callInfo.constructing()) {
        trackOptimizationOutcome(TrackedOutcome::CantInlineNativeBadForm);
        return InliningStatus_NotInlined;
    }

    wasm::Instance& inst = wasm::ExportedFunctionToInstance(target);
    uint32_t funcIndex = inst.code().getFuncIndex(target);
=======
IonBuilder::InliningResult IonBuilder::inlineWasmCall(CallInfo& callInfo,
                                                      JSFunction* target) {
  MOZ_ASSERT(target->isWasmWithJitEntry());

  // Don't inline wasm constructors.
  if (callInfo.constructing()) {
    trackOptimizationOutcome(TrackedOutcome::CantInlineNativeBadForm);
    return InliningStatus_NotInlined;
  }
>>>>>>> upstream-releases

<<<<<<< HEAD
  // Don't inline wasm constructors.
  if (callInfo.constructing()) {
    trackOptimizationOutcome(TrackedOutcome::CantInlineNativeBadForm);
    return InliningStatus_NotInlined;
  }
||||||| merged common ancestors
    auto bestTier = inst.code().bestTier();
    const wasm::FuncExport& funcExport = inst.metadata(bestTier).lookupFuncExport(funcIndex);
    const wasm::FuncType& sig = funcExport.funcType();
=======
  if (target->realm() != script()->realm()) {
    trackOptimizationOutcome(TrackedOutcome::CantInlineCrossRealm);
    return InliningStatus_NotInlined;
  }
>>>>>>> upstream-releases

  wasm::Instance& inst = wasm::ExportedFunctionToInstance(target);
  uint32_t funcIndex = inst.code().getFuncIndex(target);

  auto bestTier = inst.code().bestTier();
  const wasm::FuncExport& funcExport =
      inst.metadata(bestTier).lookupFuncExport(funcIndex);
  const wasm::FuncType& sig = funcExport.funcType();

<<<<<<< HEAD
  // Check that the function doesn't take or return non-compatible JS
  // argument types before adding nodes to the MIR graph, otherwise they'd be
  // dead code.
  if (sig.hasI64ArgOrRet() || sig.temporarilyUnsupportedAnyRef()) {
    return InliningStatus_NotInlined;
  }

  // If there are too many arguments, don't inline (we won't be able to store
  // the arguments in the LIR node).
  static constexpr size_t MaxNumInlinedArgs = 8;
  static_assert(MaxNumInlinedArgs <= MaxNumLInstructionOperands,
                "inlined arguments can all be LIR operands");
  if (sig.args().length() > MaxNumInlinedArgs) {
    return InliningStatus_NotInlined;
  }
||||||| merged common ancestors
    auto* call = MIonToWasmCall::New(alloc(), inst.object(), funcExport);
    if (!call) {
        return abort(AbortReason::Alloc);
    }
=======
  // Check that the function doesn't take or return non-compatible JS
  // argument types before adding nodes to the MIR graph, otherwise they'd be
  // dead code.
  if (sig.hasI64ArgOrRet() || sig.temporarilyUnsupportedAnyRef() ||
      !JitOptions.enableWasmIonFastCalls) {
    return InliningStatus_NotInlined;
  }

  // If there are too many arguments, don't inline (we won't be able to store
  // the arguments in the LIR node).
  static constexpr size_t MaxNumInlinedArgs = 8;
  static_assert(MaxNumInlinedArgs <= MaxNumLInstructionOperands,
                "inlined arguments can all be LIR operands");
  if (sig.args().length() > MaxNumInlinedArgs) {
    return InliningStatus_NotInlined;
  }
>>>>>>> upstream-releases

  auto* call = MIonToWasmCall::New(alloc(), inst.object(), funcExport);
  if (!call) {
    return abort(AbortReason::Alloc);
  }

  Maybe<MDefinition*> undefined;
  for (size_t i = 0; i < sig.args().length(); i++) {
    if (!alloc().ensureBallast()) {
      return abort(AbortReason::Alloc);
    }

    // Add undefined if an argument is missing.
    if (i >= callInfo.argc() && !undefined) {
      undefined.emplace(constant(UndefinedValue()));
    }

    MDefinition* arg = i >= callInfo.argc() ? *undefined : callInfo.getArg(i);

<<<<<<< HEAD
    MInstruction* conversion = nullptr;
    switch (sig.args()[i].code()) {
      case wasm::ValType::I32:
        conversion = MTruncateToInt32::New(alloc(), arg);
        break;
      case wasm::ValType::F32:
        conversion = MToFloat32::New(alloc(), arg);
        break;
      case wasm::ValType::F64:
        conversion = MToDouble::New(alloc(), arg);
        break;
      case wasm::ValType::I64:
      case wasm::ValType::AnyRef:
      case wasm::ValType::Ref:
        MOZ_CRASH("impossible per above check");
      case wasm::ValType::NullRef:
        MOZ_CRASH("NullRef not expressible");
||||||| merged common ancestors
        current->add(conversion);
        call->initArg(i, conversion);
=======
    MInstruction* conversion = nullptr;
    switch (sig.args()[i].code()) {
      case wasm::ValType::I32:
        conversion = MTruncateToInt32::New(alloc(), arg);
        break;
      case wasm::ValType::F32:
        conversion = MToFloat32::New(alloc(), arg);
        break;
      case wasm::ValType::F64:
        conversion = MToDouble::New(alloc(), arg);
        break;
      case wasm::ValType::I64:
      case wasm::ValType::AnyRef:
      case wasm::ValType::FuncRef:
      case wasm::ValType::Ref:
        MOZ_CRASH("impossible per above check");
      case wasm::ValType::NullRef:
        MOZ_CRASH("NullRef not expressible");
>>>>>>> upstream-releases
    }

    current->add(conversion);
    call->initArg(i, conversion);
  }

  current->push(call);
  current->add(call);

<<<<<<< HEAD
  callInfo.setImplicitlyUsedUnchecked();

  return InliningStatus_Inlined;
||||||| merged common ancestors
    return InliningStatus_Inlined;
=======
  MOZ_TRY(resumeAfter(call));

  callInfo.setImplicitlyUsedUnchecked();

  return InliningStatus_Inlined;
>>>>>>> upstream-releases
}

#define ADD_NATIVE(native)                                              \
  const JSJitInfo JitInfo_##native{{nullptr},                           \
                                   {uint16_t(InlinableNative::native)}, \
                                   {0},                                 \
                                   JSJitInfo::InlinableNative};
INLINABLE_NATIVE_LIST(ADD_NATIVE)
#undef ADD_NATIVE

}  // namespace jit
}  // namespace js
