/* -*- Mode: C++; tab-width: 8; indent-tabs-mode: nil; c-basic-offset: 2 -*-
 * vim: set ts=8 sts=2 et sw=2 tw=80:
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/. */

/* A higher-order macro for enumerating all cached property names. */

#ifndef vm_CommonPropertyNames_h
#define vm_CommonPropertyNames_h

#include "js/ProtoKey.h"

<<<<<<< HEAD
#define FOR_EACH_COMMON_PROPERTYNAME(MACRO)                                    \
  MACRO(add, add, "add")                                                       \
  MACRO(allowContentIter, allowContentIter, "allowContentIter")                \
  MACRO(anonymous, anonymous, "anonymous")                                     \
  MACRO(Any, Any, "Any")                                                       \
  MACRO(apply, apply, "apply")                                                 \
  MACRO(arguments, arguments, "arguments")                                     \
  MACRO(ArrayBufferSpecies, ArrayBufferSpecies, "ArrayBufferSpecies")          \
  MACRO(ArrayIterator, ArrayIterator, "Array Iterator")                        \
  MACRO(ArrayIteratorNext, ArrayIteratorNext, "ArrayIteratorNext")             \
  MACRO(ArraySort, ArraySort, "ArraySort")                                     \
  MACRO(ArraySpecies, ArraySpecies, "ArraySpecies")                            \
  MACRO(ArraySpeciesCreate, ArraySpeciesCreate, "ArraySpeciesCreate")          \
  MACRO(ArrayToLocaleString, ArrayToLocaleString, "ArrayToLocaleString")       \
  MACRO(ArrayType, ArrayType, "ArrayType")                                     \
  MACRO(ArrayValues, ArrayValues, "ArrayValues")                               \
  MACRO(as, as, "as")                                                          \
  MACRO(Async, Async, "Async")                                                 \
  MACRO(AsyncFromSyncIterator, AsyncFromSyncIterator,                          \
        "Async-from-Sync Iterator")                                            \
  MACRO(AsyncFunction, AsyncFunction, "AsyncFunction")                         \
  MACRO(AsyncGenerator, AsyncGenerator, "AsyncGenerator")                      \
  MACRO(AsyncGeneratorFunction, AsyncGeneratorFunction,                        \
        "AsyncGeneratorFunction")                                              \
  MACRO(AsyncWrapped, AsyncWrapped, "AsyncWrapped")                            \
  MACRO(async, async, "async")                                                 \
  MACRO(autoAllocateChunkSize, autoAllocateChunkSize, "autoAllocateChunkSize") \
  MACRO(await, await, "await")                                                 \
  MACRO(Bool8x16, Bool8x16, "Bool8x16")                                        \
  MACRO(Bool16x8, Bool16x8, "Bool16x8")                                        \
  MACRO(Bool32x4, Bool32x4, "Bool32x4")                                        \
  MACRO(Bool64x2, Bool64x2, "Bool64x2")                                        \
  MACRO(boundWithSpace, boundWithSpace, "bound ")                              \
  MACRO(break, break_, "break")                                                \
  MACRO(breakdown, breakdown, "breakdown")                                     \
  MACRO(buffer, buffer, "buffer")                                              \
  MACRO(builder, builder, "builder")                                           \
  MACRO(by, by, "by")                                                          \
  MACRO(byob, byob, "byob")                                                    \
  MACRO(byteAlignment, byteAlignment, "byteAlignment")                         \
  MACRO(byteLength, byteLength, "byteLength")                                  \
  MACRO(byteOffset, byteOffset, "byteOffset")                                  \
  MACRO(bytes, bytes, "bytes")                                                 \
  MACRO(BYTES_PER_ELEMENT, BYTES_PER_ELEMENT, "BYTES_PER_ELEMENT")             \
  MACRO(call, call, "call")                                                    \
  MACRO(callContentFunction, callContentFunction, "callContentFunction")       \
  MACRO(callee, callee, "callee")                                              \
  MACRO(caller, caller, "caller")                                              \
  MACRO(callFunction, callFunction, "callFunction")                            \
  MACRO(cancel, cancel, "cancel")                                              \
  MACRO(case, case_, "case")                                                   \
  MACRO(caseFirst, caseFirst, "caseFirst")                                     \
  MACRO(catch, catch_, "catch")                                                \
  MACRO(class, class_, "class")                                                \
  MACRO(Collator, Collator, "Collator")                                        \
  MACRO(collections, collections, "collections")                               \
  MACRO(columnNumber, columnNumber, "columnNumber")                            \
  MACRO(comma, comma, ",")                                                     \
  MACRO(compare, compare, "compare")                                           \
  MACRO(configurable, configurable, "configurable")                            \
  MACRO(const, const_, "const")                                                \
  MACRO(construct, construct, "construct")                                     \
  MACRO(constructContentFunction, constructContentFunction,                    \
        "constructContentFunction")                                            \
  MACRO(constructor, constructor, "constructor")                               \
  MACRO(continue, continue_, "continue")                                       \
  MACRO(ConvertAndCopyTo, ConvertAndCopyTo, "ConvertAndCopyTo")                \
  MACRO(CopyDataProperties, CopyDataProperties, "CopyDataProperties")          \
  MACRO(CopyDataPropertiesUnfiltered, CopyDataPropertiesUnfiltered,            \
        "CopyDataPropertiesUnfiltered")                                        \
  MACRO(copyWithin, copyWithin, "copyWithin")                                  \
  MACRO(count, count, "count")                                                 \
  MACRO(CreateResolvingFunctions, CreateResolvingFunctions,                    \
        "CreateResolvingFunctions")                                            \
  MACRO(currency, currency, "currency")                                        \
  MACRO(currencyDisplay, currencyDisplay, "currencyDisplay")                   \
  MACRO(DateTimeFormat, DateTimeFormat, "DateTimeFormat")                      \
  MACRO(day, day, "day")                                                       \
  MACRO(dayPeriod, dayPeriod, "dayPeriod")                                     \
  MACRO(debugger, debugger, "debugger")                                        \
  MACRO(decimal, decimal, "decimal")                                           \
  MACRO(decodeURI, decodeURI, "decodeURI")                                     \
  MACRO(decodeURIComponent, decodeURIComponent, "decodeURIComponent")          \
  MACRO(DefaultBaseClassConstructor, DefaultBaseClassConstructor,              \
        "DefaultBaseClassConstructor")                                         \
  MACRO(DefaultDerivedClassConstructor, DefaultDerivedClassConstructor,        \
        "DefaultDerivedClassConstructor")                                      \
  MACRO(default, default_, "default")                                          \
  MACRO(defineGetter, defineGetter, "__defineGetter__")                        \
  MACRO(defineProperty, defineProperty, "defineProperty")                      \
  MACRO(defineSetter, defineSetter, "__defineSetter__")                        \
  MACRO(delete, delete_, "delete")                                             \
  MACRO(deleteProperty, deleteProperty, "deleteProperty")                      \
  MACRO(direction, direction, "direction")                                     \
  MACRO(displayURL, displayURL, "displayURL")                                  \
  MACRO(do, do_, "do")                                                         \
  MACRO(domNode, domNode, "domNode")                                           \
  MACRO(done, done, "done")                                                    \
  MACRO(dotGenerator, dotGenerator, ".generator")                              \
  MACRO(dotThis, dotThis, ".this")                                             \
  MACRO(each, each, "each")                                                    \
  MACRO(elementType, elementType, "elementType")                               \
  MACRO(else, else_, "else")                                                   \
  MACRO(empty, empty, "")                                                      \
  MACRO(emptyRegExp, emptyRegExp, "(?:)")                                      \
  MACRO(encodeURI, encodeURI, "encodeURI")                                     \
  MACRO(encodeURIComponent, encodeURIComponent, "encodeURIComponent")          \
  MACRO(endTimestamp, endTimestamp, "endTimestamp")                            \
  MACRO(entries, entries, "entries")                                           \
  MACRO(enum, enum_, "enum")                                                   \
  MACRO(enumerable, enumerable, "enumerable")                                  \
  MACRO(enumerate, enumerate, "enumerate")                                     \
  MACRO(era, era, "era")                                                       \
  MACRO(ErrorToStringWithTrailingNewline, ErrorToStringWithTrailingNewline,    \
        "ErrorToStringWithTrailingNewline")                                    \
  MACRO(escape, escape, "escape")                                              \
  MACRO(eval, eval, "eval")                                                    \
  MACRO(exec, exec, "exec")                                                    \
  MACRO(export, export_, "export")                                             \
  MACRO(extends, extends, "extends")                                           \
  MACRO(false, false_, "false")                                                \
  MACRO(fieldOffsets, fieldOffsets, "fieldOffsets")                            \
  MACRO(fieldTypes, fieldTypes, "fieldTypes")                                  \
  MACRO(fileName, fileName, "fileName")                                        \
  MACRO(fill, fill, "fill")                                                    \
  MACRO(finally, finally_, "finally")                                          \
  MACRO(find, find, "find")                                                    \
  MACRO(findIndex, findIndex, "findIndex")                                     \
  MACRO(firstDayOfWeek, firstDayOfWeek, "firstDayOfWeek")                      \
  MACRO(fix, fix, "fix")                                                       \
  MACRO(flags, flags, "flags")                                                 \
  MACRO(float32, float32, "float32")                                           \
  MACRO(Float32x4, Float32x4, "Float32x4")                                     \
  MACRO(float64, float64, "float64")                                           \
  MACRO(Float64x2, Float64x2, "Float64x2")                                     \
    MACRO(for, for_, "for")                                                    \
  MACRO(forceInterpreter, forceInterpreter, "forceInterpreter")                \
  MACRO(forEach, forEach, "forEach")                                           \
  MACRO(format, format, "format")                                              \
  MACRO(fraction, fraction, "fraction")                                        \
  MACRO(frame, frame, "frame")                                                 \
  MACRO(from, from, "from")                                                    \
  MACRO(fulfilled, fulfilled, "fulfilled")                                     \
  MACRO(futexNotEqual, futexNotEqual, "not-equal")                             \
  MACRO(futexOK, futexOK, "ok")                                                \
  MACRO(futexTimedOut, futexTimedOut, "timed-out")                             \
  MACRO(gcCycleNumber, gcCycleNumber, "gcCycleNumber")                         \
  MACRO(Generator, Generator, "Generator")                                     \
  MACRO(GeneratorFunction, GeneratorFunction, "GeneratorFunction")             \
  MACRO(GeneratorNext, GeneratorNext, "GeneratorNext")                         \
  MACRO(GeneratorReturn, GeneratorReturn, "GeneratorReturn")                   \
  MACRO(GeneratorThrow, GeneratorThrow, "GeneratorThrow")                      \
  MACRO(get, get, "get")                                                       \
  MACRO(GetInternalError, GetInternalError, "GetInternalError")                \
  MACRO(getInternals, getInternals, "getInternals")                            \
  MACRO(GetModuleNamespace, GetModuleNamespace, "GetModuleNamespace")          \
  MACRO(getOwnPropertyDescriptor, getOwnPropertyDescriptor,                    \
        "getOwnPropertyDescriptor")                                            \
  MACRO(getOwnPropertyNames, getOwnPropertyNames, "getOwnPropertyNames")       \
  MACRO(getPrefix, getPrefix, "get ")                                          \
  MACRO(getPropertyDescriptor, getPropertyDescriptor, "getPropertyDescriptor") \
  MACRO(getPropertySuper, getPropertySuper, "getPropertySuper")                \
  MACRO(getPrototypeOf, getPrototypeOf, "getPrototypeOf")                      \
  MACRO(GetTypeError, GetTypeError, "GetTypeError")                            \
  MACRO(global, global, "global")                                              \
  MACRO(globalThis, globalThis, "globalThis")                                  \
  MACRO(group, group, "group")                                                 \
  MACRO(Handle, Handle, "Handle")                                              \
  MACRO(has, has, "has")                                                       \
  MACRO(hasOwn, hasOwn, "hasOwn")                                              \
  MACRO(hasOwnProperty, hasOwnProperty, "hasOwnProperty")                      \
  MACRO(highWaterMark, highWaterMark, "highWaterMark")                         \
  MACRO(hour, hour, "hour")                                                    \
  MACRO(if, if_, "if")                                                         \
  MACRO(ignoreCase, ignoreCase, "ignoreCase")                                  \
  MACRO(ignorePunctuation, ignorePunctuation, "ignorePunctuation")             \
  MACRO(implements, implements, "implements")                                  \
  MACRO(import, import, "import")                                              \
  MACRO(in, in, "in")                                                          \
  MACRO(includes, includes, "includes")                                        \
  MACRO(incumbentGlobal, incumbentGlobal, "incumbentGlobal")                   \
  MACRO(index, index, "index")                                                 \
  MACRO(infinity, infinity, "infinity")                                        \
  MACRO(Infinity, Infinity, "Infinity")                                        \
  MACRO(InitializeCollator, InitializeCollator, "InitializeCollator")          \
  MACRO(InitializeDateTimeFormat, InitializeDateTimeFormat,                    \
        "InitializeDateTimeFormat")                                            \
  MACRO(InitializeNumberFormat, InitializeNumberFormat,                        \
        "InitializeNumberFormat")                                              \
  MACRO(InitializePluralRules, InitializePluralRules, "InitializePluralRules") \
  MACRO(InitializeRelativeTimeFormat, InitializeRelativeTimeFormat,            \
        "InitializeRelativeTimeFormat")                                        \
  MACRO(innermost, innermost, "innermost")                                     \
  MACRO(inNursery, inNursery, "inNursery")                                     \
  MACRO(input, input, "input")                                                 \
  MACRO(instanceof, instanceof, "instanceof")                                  \
  MACRO(int8, int8, "int8")                                                    \
  MACRO(int16, int16, "int16")                                                 \
  MACRO(int32, int32, "int32")                                                 \
  MACRO(Int8x16, Int8x16, "Int8x16")                                           \
  MACRO(Int16x8, Int16x8, "Int16x8")                                           \
  MACRO(Int32x4, Int32x4, "Int32x4")                                           \
  MACRO(integer, integer, "integer")                                           \
  MACRO(interface, interface, "interface")                                     \
  MACRO(InterpretGeneratorResume, InterpretGeneratorResume,                    \
        "InterpretGeneratorResume")                                            \
  MACRO(InvalidDate, InvalidDate, "Invalid Date")                              \
  MACRO(isEntryPoint, isEntryPoint, "isEntryPoint")                            \
  MACRO(isExtensible, isExtensible, "isExtensible")                            \
  MACRO(isFinite, isFinite, "isFinite")                                        \
  MACRO(isNaN, isNaN, "isNaN")                                                 \
  MACRO(isPrototypeOf, isPrototypeOf, "isPrototypeOf")                         \
  MACRO(IterableToList, IterableToList, "IterableToList")                      \
  MACRO(iterate, iterate, "iterate")                                           \
  MACRO(join, join, "join")                                                    \
  MACRO(js, js, "js")                                                          \
  MACRO(keys, keys, "keys")                                                    \
  MACRO(label, label, "label")                                                 \
  MACRO(lastIndex, lastIndex, "lastIndex")                                     \
  MACRO(length, length, "length")                                              \
  MACRO(let, let, "let")                                                       \
  MACRO(line, line, "line")                                                    \
  MACRO(lineNumber, lineNumber, "lineNumber")                                  \
  MACRO(literal, literal, "literal")                                           \
  MACRO(loc, loc, "loc")                                                       \
  MACRO(locale, locale, "locale")                                              \
  MACRO(lookupGetter, lookupGetter, "__lookupGetter__")                        \
  MACRO(lookupSetter, lookupSetter, "__lookupSetter__")                        \
  MACRO(ltr, ltr, "ltr")                                                       \
  MACRO(MapConstructorInit, MapConstructorInit, "MapConstructorInit")          \
  MACRO(MapIterator, MapIterator, "Map Iterator")                              \
  MACRO(maximumFractionDigits, maximumFractionDigits, "maximumFractionDigits") \
  MACRO(maximumSignificantDigits, maximumSignificantDigits,                    \
        "maximumSignificantDigits")                                            \
  MACRO(message, message, "message")                                           \
  MACRO(meta, meta, "meta")                                                    \
  MACRO(minDays, minDays, "minDays")                                           \
  MACRO(minimumFractionDigits, minimumFractionDigits, "minimumFractionDigits") \
  MACRO(minimumIntegerDigits, minimumIntegerDigits, "minimumIntegerDigits")    \
  MACRO(minimumSignificantDigits, minimumSignificantDigits,                    \
        "minimumSignificantDigits")                                            \
  MACRO(minusSign, minusSign, "minusSign")                                     \
  MACRO(minute, minute, "minute")                                              \
  MACRO(missingArguments, missingArguments, "missingArguments")                \
  MACRO(mode, mode, "mode")                                                    \
  MACRO(module, module, "module")                                              \
  MACRO(Module, Module, "Module")                                              \
  MACRO(ModuleInstantiate, ModuleInstantiate, "ModuleInstantiate")             \
  MACRO(ModuleEvaluate, ModuleEvaluate, "ModuleEvaluate")                      \
  MACRO(month, month, "month")                                                 \
  MACRO(multiline, multiline, "multiline")                                     \
  MACRO(name, name, "name")                                                    \
  MACRO(nan, nan, "nan")                                                       \
  MACRO(NaN, NaN, "NaN")                                                       \
  MACRO(NegativeInfinity, NegativeInfinity, "-Infinity")                       \
  MACRO(new, new_, "new")                                                      \
  MACRO(next, next, "next")                                                    \
  MACRO(NFC, NFC, "NFC")                                                       \
  MACRO(NFD, NFD, "NFD")                                                       \
  MACRO(NFKC, NFKC, "NFKC")                                                    \
  MACRO(NFKD, NFKD, "NFKD")                                                    \
  MACRO(noFilename, noFilename, "noFilename")                                  \
  MACRO(nonincrementalReason, nonincrementalReason, "nonincrementalReason")    \
  MACRO(noStack, noStack, "noStack")                                           \
  MACRO(notes, notes, "notes")                                                 \
  MACRO(NumberFormat, NumberFormat, "NumberFormat")                            \
  MACRO(numeric, numeric, "numeric")                                           \
  MACRO(objectArguments, objectArguments, "[object Arguments]")                \
  MACRO(objectArray, objectArray, "[object Array]")                            \
  MACRO(objectBigInt, objectBigInt, "[object BigInt]")                         \
  MACRO(objectBoolean, objectBoolean, "[object Boolean]")                      \
  MACRO(objectDate, objectDate, "[object Date]")                               \
  MACRO(objectError, objectError, "[object Error]")                            \
  MACRO(objectFunction, objectFunction, "[object Function]")                   \
  MACRO(objectNull, objectNull, "[object Null]")                               \
  MACRO(objectNumber, objectNumber, "[object Number]")                         \
  MACRO(objectObject, objectObject, "[object Object]")                         \
  MACRO(objectRegExp, objectRegExp, "[object RegExp]")                         \
  MACRO(objects, objects, "objects")                                           \
  MACRO(objectString, objectString, "[object String]")                         \
  MACRO(objectUndefined, objectUndefined, "[object Undefined]")                \
  MACRO(of, of, "of")                                                          \
  MACRO(offset, offset, "offset")                                              \
  MACRO(optimizedOut, optimizedOut, "optimizedOut")                            \
  MACRO(other, other, "other")                                                 \
  MACRO(outOfMemory, outOfMemory, "out of memory")                             \
  MACRO(ownKeys, ownKeys, "ownKeys")                                           \
  MACRO(Object_valueOf, Object_valueOf, "Object_valueOf")                      \
  MACRO(package, package, "package")                                           \
  MACRO(parseFloat, parseFloat, "parseFloat")                                  \
  MACRO(parseInt, parseInt, "parseInt")                                        \
  MACRO(pattern, pattern, "pattern")                                           \
  MACRO(pending, pending, "pending")                                           \
  MACRO(PluralRules, PluralRules, "PluralRules")                               \
  MACRO(percentSign, percentSign, "percentSign")                               \
  MACRO(plusSign, plusSign, "plusSign")                                        \
  MACRO(public, public_, "public")                                             \
  MACRO(pull, pull, "pull")                                                    \
  MACRO(preventExtensions, preventExtensions, "preventExtensions")             \
  MACRO(private, private_, "private")                                          \
  MACRO(promise, promise, "promise")                                           \
  MACRO(propertyIsEnumerable, propertyIsEnumerable, "propertyIsEnumerable")    \
  MACRO(protected, protected_, "protected")                                    \
  MACRO(proto, proto, "__proto__")                                             \
  MACRO(prototype, prototype, "prototype")                                     \
  MACRO(proxy, proxy, "proxy")                                                 \
  MACRO(raw, raw, "raw")                                                       \
  MACRO(reason, reason, "reason")                                              \
  MACRO(RegExpBuiltinExec, RegExpBuiltinExec, "RegExpBuiltinExec")             \
  MACRO(RegExpFlagsGetter, RegExpFlagsGetter, "RegExpFlagsGetter")             \
  MACRO(RegExpMatcher, RegExpMatcher, "RegExpMatcher")                         \
  MACRO(RegExpSearcher, RegExpSearcher, "RegExpSearcher")                      \
  MACRO(RegExpTester, RegExpTester, "RegExpTester")                            \
  MACRO(RegExp_prototype_Exec, RegExp_prototype_Exec, "RegExp_prototype_Exec") \
  MACRO(Reify, Reify, "Reify")                                                 \
  MACRO(reject, reject, "reject")                                              \
  MACRO(rejected, rejected, "rejected")                                        \
  MACRO(RelativeTimeFormat, RelativeTimeFormat, "RelativeTimeFormat")          \
  MACRO(RelativeTimeFormatFormat, RelativeTimeFormatFormat,                    \
        "Intl_RelativeTimeFormat_Format")                                      \
  MACRO(RequireObjectCoercible, RequireObjectCoercible,                        \
        "RequireObjectCoercible")                                              \
  MACRO(resolve, resolve, "resolve")                                           \
  MACRO(resumeGenerator, resumeGenerator, "resumeGenerator")                   \
  MACRO(return, return_, "return")                                             \
  MACRO(revoke, revoke, "revoke")                                              \
  MACRO(rtl, rtl, "rtl")                                                       \
  MACRO(script, script, "script")                                              \
  MACRO(scripts, scripts, "scripts")                                           \
  MACRO(second, second, "second")                                              \
  MACRO(selfHosted, selfHosted, "self-hosted")                                 \
  MACRO(sensitivity, sensitivity, "sensitivity")                               \
  MACRO(set, set, "set")                                                       \
  MACRO(SetConstructorInit, SetConstructorInit, "SetConstructorInit")          \
  MACRO(SetIterator, SetIterator, "Set Iterator")                              \
  MACRO(setPrefix, setPrefix, "set ")                                          \
  MACRO(setPrototypeOf, setPrototypeOf, "setPrototypeOf")                      \
  MACRO(shape, shape, "shape")                                                 \
  MACRO(size, size, "size")                                                    \
  MACRO(source, source, "source")                                              \
  MACRO(SpeciesConstructor, SpeciesConstructor, "SpeciesConstructor")          \
  MACRO(stack, stack, "stack")                                                 \
  MACRO(star, star, "*")                                                       \
  MACRO(start, start, "start")                                                 \
  MACRO(startTimestamp, startTimestamp, "startTimestamp")                      \
  MACRO(state, state, "state")                                                 \
  MACRO(static, static_, "static")                                             \
  MACRO(std_Function_apply, std_Function_apply, "std_Function_apply")          \
  MACRO(sticky, sticky, "sticky")                                              \
  MACRO(StringIterator, StringIterator, "String Iterator")                     \
  MACRO(strings, strings, "strings")                                           \
  MACRO(StructType, StructType, "StructType")                                  \
  MACRO(style, style, "style")                                                 \
  MACRO(super, super, "super")                                                 \
  MACRO(switch, switch_, "switch")                                             \
  MACRO(Symbol_iterator_fun, Symbol_iterator_fun, "[Symbol.iterator]")         \
  MACRO(target, target, "target")                                              \
  MACRO(test, test, "test")                                                    \
  MACRO(then, then, "then")                                                    \
  MACRO(this, this_, "this")                                                   \
  MACRO(throw, throw_, "throw")                                                \
  MACRO(timestamp, timestamp, "timestamp")                                     \
  MACRO(timeZone, timeZone, "timeZone")                                        \
  MACRO(timeZoneName, timeZoneName, "timeZoneName")                            \
  MACRO(trimEnd, trimEnd, "trimEnd")                                           \
  MACRO(trimLeft, trimLeft, "trimLeft")                                        \
  MACRO(trimRight, trimRight, "trimRight")                                     \
  MACRO(trimStart, trimStart, "trimStart")                                     \
  MACRO(toGMTString, toGMTString, "toGMTString")                               \
  MACRO(toISOString, toISOString, "toISOString")                               \
  MACRO(toJSON, toJSON, "toJSON")                                              \
  MACRO(toLocaleString, toLocaleString, "toLocaleString")                      \
  MACRO(toSource, toSource, "toSource")                                        \
  MACRO(toString, toString, "toString")                                        \
  MACRO(toUTCString, toUTCString, "toUTCString")                               \
  MACRO(true, true_, "true")                                                   \
  MACRO(try, try_, "try")                                                      \
  MACRO(type, type, "type")                                                    \
  MACRO(typeof, typeof_, "typeof")                                             \
  MACRO(uint8, uint8, "uint8")                                                 \
  MACRO(uint8Clamped, uint8Clamped, "uint8Clamped")                            \
  MACRO(uint16, uint16, "uint16")                                              \
  MACRO(uint32, uint32, "uint32")                                              \
  MACRO(Uint8x16, Uint8x16, "Uint8x16")                                        \
  MACRO(Uint16x8, Uint16x8, "Uint16x8")                                        \
  MACRO(Uint32x4, Uint32x4, "Uint32x4")                                        \
  MACRO(unescape, unescape, "unescape")                                        \
  MACRO(uneval, uneval, "uneval")                                              \
  MACRO(unicode, unicode, "unicode")                                           \
  MACRO(unit, unit, "unit")                                                    \
  MACRO(uninitialized, uninitialized, "uninitialized")                         \
  MACRO(unsized, unsized, "unsized")                                           \
  MACRO(unwatch, unwatch, "unwatch")                                           \
  MACRO(UnwrapAndCallRegExpBuiltinExec, UnwrapAndCallRegExpBuiltinExec,        \
        "UnwrapAndCallRegExpBuiltinExec")                                      \
  MACRO(url, url, "url")                                                       \
  MACRO(usage, usage, "usage")                                                 \
  MACRO(useAsm, useAsm, "use asm")                                             \
  MACRO(useGrouping, useGrouping, "useGrouping")                               \
  MACRO(useStrict, useStrict, "use strict")                                    \
  MACRO(void, void_, "void")                                                   \
  MACRO(value, value, "value")                                                 \
  MACRO(valueOf, valueOf, "valueOf")                                           \
  MACRO(values, values, "values")                                              \
  MACRO(var, var, "var")                                                       \
  MACRO(variable, variable, "variable")                                        \
  MACRO(void0, void0, "(void 0)")                                              \
  MACRO(wasm, wasm, "wasm")                                                    \
  MACRO(wasmcall, wasmcall, "wasmcall")                                        \
  MACRO(watch, watch, "watch")                                                 \
  MACRO(WeakMapConstructorInit, WeakMapConstructorInit,                        \
        "WeakMapConstructorInit")                                              \
  MACRO(WeakSetConstructorInit, WeakSetConstructorInit,                        \
        "WeakSetConstructorInit")                                              \
  MACRO(WeakSet_add, WeakSet_add, "WeakSet_add")                               \
  MACRO(weekday, weekday, "weekday")                                           \
  MACRO(weekendEnd, weekendEnd, "weekendEnd")                                  \
  MACRO(weekendStart, weekendStart, "weekendStart")                            \
  MACRO(while, while_, "while")                                                \
  MACRO(with, with, "with")                                                    \
  MACRO(writable, writable, "writable")                                        \
  MACRO(year, year, "year")                                                    \
  MACRO(yield, yield, "yield")                                                 \
  /* Type names must be contiguous and ordered; see js::TypeName. */           \
  MACRO(undefined, undefined, "undefined")                                     \
  MACRO(object, object, "object")                                              \
  MACRO(function, function, "function")                                        \
  MACRO(string, string, "string")                                              \
  MACRO(number, number, "number")                                              \
  MACRO(boolean, boolean, "boolean")                                           \
  MACRO(null, null, "null")                                                    \
  MACRO(symbol, symbol, "symbol")                                              \
  MACRO(bigint, bigint, "bigint")                                              \
  MACRO(defineDataPropertyIntrinsic, defineDataPropertyIntrinsic,              \
        "_DefineDataProperty")
||||||| merged common ancestors
#define FOR_EACH_COMMON_PROPERTYNAME(macro) \
    macro(add, add, "add") \
    macro(allowContentIter, allowContentIter, "allowContentIter") \
    macro(anonymous, anonymous, "anonymous") \
    macro(Any, Any, "Any") \
    macro(apply, apply, "apply") \
    macro(arguments, arguments, "arguments") \
    macro(ArrayBufferSpecies, ArrayBufferSpecies, "ArrayBufferSpecies") \
    macro(ArrayIterator, ArrayIterator, "Array Iterator") \
    macro(ArrayIteratorNext, ArrayIteratorNext, "ArrayIteratorNext") \
    macro(ArraySort, ArraySort, "ArraySort") \
    macro(ArraySpecies, ArraySpecies, "ArraySpecies") \
    macro(ArraySpeciesCreate, ArraySpeciesCreate, "ArraySpeciesCreate") \
    macro(ArrayToLocaleString, ArrayToLocaleString, "ArrayToLocaleString") \
    macro(ArrayType, ArrayType, "ArrayType") \
    macro(ArrayValues, ArrayValues, "ArrayValues") \
    macro(as, as, "as") \
    macro(Async, Async, "Async") \
    macro(AsyncFromSyncIterator, AsyncFromSyncIterator, "Async-from-Sync Iterator") \
    macro(AsyncFunction, AsyncFunction, "AsyncFunction") \
    macro(AsyncGenerator, AsyncGenerator, "AsyncGenerator") \
    macro(AsyncGeneratorFunction, AsyncGeneratorFunction, "AsyncGeneratorFunction") \
    macro(AsyncWrapped, AsyncWrapped, "AsyncWrapped") \
    macro(async, async, "async") \
    macro(autoAllocateChunkSize, autoAllocateChunkSize, "autoAllocateChunkSize") \
    macro(await, await, "await") \
    macro(Bool8x16, Bool8x16, "Bool8x16") \
    macro(Bool16x8, Bool16x8, "Bool16x8") \
    macro(Bool32x4, Bool32x4, "Bool32x4") \
    macro(Bool64x2, Bool64x2, "Bool64x2") \
    macro(boundWithSpace, boundWithSpace, "bound ") \
    macro(break, break_, "break") \
    macro(breakdown, breakdown, "breakdown") \
    macro(buffer, buffer, "buffer") \
    macro(builder, builder, "builder") \
    macro(by, by, "by") \
    macro(byob, byob, "byob") \
    macro(byteAlignment, byteAlignment, "byteAlignment") \
    macro(byteLength, byteLength, "byteLength") \
    macro(byteOffset, byteOffset, "byteOffset") \
    macro(bytes, bytes, "bytes") \
    macro(BYTES_PER_ELEMENT, BYTES_PER_ELEMENT, "BYTES_PER_ELEMENT") \
    macro(call, call, "call") \
    macro(callContentFunction, callContentFunction, "callContentFunction") \
    macro(callee, callee, "callee") \
    macro(caller, caller, "caller") \
    macro(callFunction, callFunction, "callFunction") \
    macro(cancel, cancel, "cancel") \
    macro(case, case_, "case") \
    macro(caseFirst, caseFirst, "caseFirst") \
    macro(catch, catch_, "catch") \
    macro(class, class_, "class") \
    macro(Collator, Collator, "Collator") \
    macro(collections, collections, "collections") \
    macro(columnNumber, columnNumber, "columnNumber") \
    macro(comma, comma, ",") \
    macro(compare, compare, "compare") \
    macro(configurable, configurable, "configurable") \
    macro(const, const_, "const") \
    macro(construct, construct, "construct") \
    macro(constructContentFunction, constructContentFunction, "constructContentFunction") \
    macro(constructor, constructor, "constructor") \
    macro(continue, continue_, "continue") \
    macro(ConvertAndCopyTo, ConvertAndCopyTo, "ConvertAndCopyTo") \
    macro(CopyDataProperties, CopyDataProperties, "CopyDataProperties") \
    macro(CopyDataPropertiesUnfiltered, CopyDataPropertiesUnfiltered, "CopyDataPropertiesUnfiltered") \
    macro(copyWithin, copyWithin, "copyWithin") \
    macro(count, count, "count") \
    macro(CreateResolvingFunctions, CreateResolvingFunctions, "CreateResolvingFunctions") \
    macro(currency, currency, "currency") \
    macro(currencyDisplay, currencyDisplay, "currencyDisplay") \
    macro(DateTimeFormat, DateTimeFormat, "DateTimeFormat") \
    macro(day, day, "day") \
    macro(dayPeriod, dayPeriod, "dayPeriod") \
    macro(debugger, debugger, "debugger") \
    macro(decimal, decimal, "decimal") \
    macro(decodeURI, decodeURI, "decodeURI") \
    macro(decodeURIComponent, decodeURIComponent, "decodeURIComponent") \
    macro(DefaultBaseClassConstructor, DefaultBaseClassConstructor, "DefaultBaseClassConstructor") \
    macro(DefaultDerivedClassConstructor, DefaultDerivedClassConstructor, "DefaultDerivedClassConstructor") \
    macro(default, default_, "default") \
    macro(defineGetter, defineGetter, "__defineGetter__") \
    macro(defineProperty, defineProperty, "defineProperty") \
    macro(defineSetter, defineSetter, "__defineSetter__") \
    macro(delete, delete_, "delete") \
    macro(deleteProperty, deleteProperty, "deleteProperty") \
    macro(direction, direction, "direction") \
    macro(displayURL, displayURL, "displayURL") \
    macro(do, do_, "do") \
    macro(domNode, domNode, "domNode") \
    macro(done, done, "done") \
    macro(dotGenerator, dotGenerator, ".generator") \
    macro(dotThis, dotThis, ".this") \
    macro(each, each, "each") \
    macro(elementType, elementType, "elementType") \
    macro(else, else_, "else") \
    macro(empty, empty, "") \
    macro(emptyRegExp, emptyRegExp, "(?:)") \
    macro(encodeURI, encodeURI, "encodeURI") \
    macro(encodeURIComponent, encodeURIComponent, "encodeURIComponent") \
    macro(endTimestamp, endTimestamp, "endTimestamp") \
    macro(entries, entries, "entries") \
    macro(enum, enum_, "enum") \
    macro(enumerable, enumerable, "enumerable") \
    macro(enumerate, enumerate, "enumerate") \
    macro(era, era, "era") \
    macro(ErrorToStringWithTrailingNewline, ErrorToStringWithTrailingNewline, "ErrorToStringWithTrailingNewline") \
    macro(escape, escape, "escape") \
    macro(eval, eval, "eval") \
    macro(exec, exec, "exec") \
    macro(export, export_, "export") \
    macro(extends, extends, "extends") \
    macro(false, false_, "false") \
    macro(fieldOffsets, fieldOffsets, "fieldOffsets") \
    macro(fieldTypes, fieldTypes, "fieldTypes") \
    macro(fileName, fileName, "fileName") \
    macro(fill, fill, "fill") \
    macro(finally, finally_, "finally") \
    macro(find, find, "find") \
    macro(findIndex, findIndex, "findIndex") \
    macro(firstDayOfWeek, firstDayOfWeek, "firstDayOfWeek") \
    macro(fix, fix, "fix") \
    macro(flags, flags, "flags") \
    macro(float32, float32, "float32") \
    macro(Float32x4, Float32x4, "Float32x4") \
    macro(float64, float64, "float64") \
    macro(Float64x2, Float64x2, "Float64x2") \
    macro(for, for_, "for") \
    macro(forceInterpreter, forceInterpreter, "forceInterpreter") \
    macro(forEach, forEach, "forEach") \
    macro(format, format, "format") \
    macro(fraction, fraction, "fraction") \
    macro(frame, frame, "frame") \
    macro(from, from, "from") \
    macro(fulfilled, fulfilled, "fulfilled") \
    macro(futexNotEqual, futexNotEqual, "not-equal") \
    macro(futexOK, futexOK, "ok") \
    macro(futexTimedOut, futexTimedOut, "timed-out") \
    macro(gcCycleNumber, gcCycleNumber, "gcCycleNumber") \
    macro(Generator, Generator, "Generator") \
    macro(GeneratorFunction, GeneratorFunction, "GeneratorFunction") \
    macro(GeneratorNext, GeneratorNext, "GeneratorNext") \
    macro(GeneratorReturn, GeneratorReturn, "GeneratorReturn") \
    macro(GeneratorThrow, GeneratorThrow, "GeneratorThrow") \
    macro(get, get, "get") \
    macro(GetInternalError, GetInternalError, "GetInternalError") \
    macro(getInternals, getInternals, "getInternals") \
    macro(GetModuleNamespace, GetModuleNamespace, "GetModuleNamespace") \
    macro(getOwnPropertyDescriptor, getOwnPropertyDescriptor, "getOwnPropertyDescriptor") \
    macro(getOwnPropertyNames, getOwnPropertyNames, "getOwnPropertyNames") \
    macro(getPrefix, getPrefix, "get ") \
    macro(getPropertyDescriptor, getPropertyDescriptor, "getPropertyDescriptor") \
    macro(getPropertySuper, getPropertySuper, "getPropertySuper") \
    macro(getPrototypeOf, getPrototypeOf, "getPrototypeOf") \
    macro(GetTypeError, GetTypeError, "GetTypeError") \
    macro(global, global, "global") \
    macro(group, group, "group") \
    macro(Handle, Handle, "Handle") \
    macro(has, has, "has") \
    macro(hasOwn, hasOwn, "hasOwn") \
    macro(hasOwnProperty, hasOwnProperty, "hasOwnProperty") \
    macro(highWaterMark, highWaterMark, "highWaterMark") \
    macro(hour, hour, "hour") \
    macro(if, if_, "if") \
    macro(ignoreCase, ignoreCase, "ignoreCase") \
    macro(ignorePunctuation, ignorePunctuation, "ignorePunctuation") \
    macro(implements, implements, "implements") \
    macro(import, import, "import") \
    macro(in, in, "in") \
    macro(includes, includes, "includes") \
    macro(incumbentGlobal, incumbentGlobal, "incumbentGlobal") \
    macro(index, index, "index") \
    macro(infinity, infinity, "infinity") \
    macro(Infinity, Infinity, "Infinity") \
    macro(InitializeCollator, InitializeCollator, "InitializeCollator") \
    macro(InitializeDateTimeFormat, InitializeDateTimeFormat, "InitializeDateTimeFormat") \
    macro(InitializeNumberFormat, InitializeNumberFormat, "InitializeNumberFormat") \
    macro(InitializePluralRules, InitializePluralRules, "InitializePluralRules") \
    macro(InitializeRelativeTimeFormat, InitializeRelativeTimeFormat, "InitializeRelativeTimeFormat") \
    macro(innermost, innermost, "innermost") \
    macro(inNursery, inNursery, "inNursery") \
    macro(input, input, "input") \
    macro(instanceof, instanceof, "instanceof") \
    macro(int8, int8, "int8") \
    macro(int16, int16, "int16") \
    macro(int32, int32, "int32") \
    macro(Int8x16, Int8x16, "Int8x16") \
    macro(Int16x8, Int16x8, "Int16x8") \
    macro(Int32x4, Int32x4, "Int32x4") \
    macro(integer, integer, "integer") \
    macro(interface, interface, "interface") \
    macro(InterpretGeneratorResume, InterpretGeneratorResume, "InterpretGeneratorResume") \
    macro(InvalidDate, InvalidDate, "Invalid Date") \
    macro(isEntryPoint, isEntryPoint, "isEntryPoint") \
    macro(isExtensible, isExtensible, "isExtensible") \
    macro(isFinite, isFinite, "isFinite") \
    macro(isNaN, isNaN, "isNaN") \
    macro(isPrototypeOf, isPrototypeOf, "isPrototypeOf") \
    macro(IterableToList, IterableToList, "IterableToList") \
    macro(iterate, iterate, "iterate") \
    macro(join, join, "join") \
    macro(js, js, "js") \
    macro(keys, keys, "keys") \
    macro(label, label, "label") \
    macro(lastIndex, lastIndex, "lastIndex") \
    macro(length, length, "length") \
    macro(let, let, "let") \
    macro(line, line, "line") \
    macro(lineNumber, lineNumber, "lineNumber") \
    macro(literal, literal, "literal") \
    macro(loc, loc, "loc") \
    macro(locale, locale, "locale") \
    macro(lookupGetter, lookupGetter, "__lookupGetter__") \
    macro(lookupSetter, lookupSetter, "__lookupSetter__") \
    macro(ltr, ltr, "ltr") \
    macro(MapConstructorInit, MapConstructorInit, "MapConstructorInit") \
    macro(MapIterator, MapIterator, "Map Iterator") \
    macro(maximumFractionDigits, maximumFractionDigits, "maximumFractionDigits") \
    macro(maximumSignificantDigits, maximumSignificantDigits, "maximumSignificantDigits") \
    macro(message, message, "message") \
    macro(meta, meta, "meta") \
    macro(minDays, minDays, "minDays") \
    macro(minimumFractionDigits, minimumFractionDigits, "minimumFractionDigits") \
    macro(minimumIntegerDigits, minimumIntegerDigits, "minimumIntegerDigits") \
    macro(minimumSignificantDigits, minimumSignificantDigits, "minimumSignificantDigits") \
    macro(minusSign, minusSign, "minusSign") \
    macro(minute, minute, "minute") \
    macro(missingArguments, missingArguments, "missingArguments") \
    macro(mode, mode, "mode") \
    macro(module, module, "module") \
    macro(Module, Module, "Module") \
    macro(ModuleInstantiate, ModuleInstantiate, "ModuleInstantiate") \
    macro(ModuleEvaluate, ModuleEvaluate, "ModuleEvaluate") \
    macro(month, month, "month") \
    macro(multiline, multiline, "multiline") \
    macro(name, name, "name") \
    macro(nan, nan, "nan") \
    macro(NaN, NaN, "NaN") \
    macro(NegativeInfinity, NegativeInfinity, "-Infinity") \
    macro(new, new_, "new") \
    macro(next, next, "next") \
    macro(NFC, NFC, "NFC") \
    macro(NFD, NFD, "NFD") \
    macro(NFKC, NFKC, "NFKC") \
    macro(NFKD, NFKD, "NFKD") \
    macro(noFilename, noFilename, "noFilename") \
    macro(nonincrementalReason, nonincrementalReason, "nonincrementalReason") \
    macro(noStack, noStack, "noStack") \
    macro(notes, notes, "notes") \
    macro(NumberFormat, NumberFormat, "NumberFormat") \
    macro(numeric, numeric, "numeric") \
    macro(objectArguments, objectArguments, "[object Arguments]") \
    macro(objectArray, objectArray, "[object Array]") \
    macro(objectBigInt, objectBigInt, "[object BigInt]") \
    macro(objectBoolean, objectBoolean, "[object Boolean]") \
    macro(objectDate, objectDate, "[object Date]") \
    macro(objectError, objectError, "[object Error]") \
    macro(objectFunction, objectFunction, "[object Function]") \
    macro(objectNull, objectNull, "[object Null]") \
    macro(objectNumber, objectNumber, "[object Number]") \
    macro(objectObject, objectObject, "[object Object]") \
    macro(objectRegExp, objectRegExp, "[object RegExp]") \
    macro(objects, objects, "objects") \
    macro(objectString, objectString, "[object String]") \
    macro(objectUndefined, objectUndefined, "[object Undefined]") \
    macro(of, of, "of") \
    macro(offset, offset, "offset") \
    macro(optimizedOut, optimizedOut, "optimizedOut") \
    macro(other, other, "other") \
    macro(outOfMemory, outOfMemory, "out of memory") \
    macro(ownKeys, ownKeys, "ownKeys") \
    macro(Object_valueOf, Object_valueOf, "Object_valueOf") \
    macro(package, package, "package") \
    macro(parseFloat, parseFloat, "parseFloat") \
    macro(parseInt, parseInt, "parseInt") \
    macro(pattern, pattern, "pattern") \
    macro(pending, pending, "pending") \
    macro(PluralRules, PluralRules, "PluralRules") \
    macro(percentSign, percentSign, "percentSign") \
    macro(plusSign, plusSign, "plusSign") \
    macro(public, public_, "public") \
    macro(pull, pull, "pull") \
    macro(preventExtensions, preventExtensions, "preventExtensions") \
    macro(private, private_, "private") \
    macro(promise, promise, "promise") \
    macro(propertyIsEnumerable, propertyIsEnumerable, "propertyIsEnumerable") \
    macro(protected, protected_, "protected") \
    macro(proto, proto, "__proto__") \
    macro(prototype, prototype, "prototype") \
    macro(proxy, proxy, "proxy") \
    macro(raw, raw, "raw") \
    macro(reason, reason, "reason") \
    macro(RegExpBuiltinExec, RegExpBuiltinExec, "RegExpBuiltinExec") \
    macro(RegExpFlagsGetter, RegExpFlagsGetter, "RegExpFlagsGetter") \
    macro(RegExpMatcher, RegExpMatcher, "RegExpMatcher") \
    macro(RegExpSearcher, RegExpSearcher, "RegExpSearcher") \
    macro(RegExpTester, RegExpTester, "RegExpTester") \
    macro(RegExp_prototype_Exec, RegExp_prototype_Exec, "RegExp_prototype_Exec") \
    macro(Reify, Reify, "Reify") \
    macro(reject, reject, "reject") \
    macro(rejected, rejected, "rejected") \
    macro(RelativeTimeFormat, RelativeTimeFormat, "RelativeTimeFormat") \
    macro(RelativeTimeFormatFormat, RelativeTimeFormatFormat, "Intl_RelativeTimeFormat_Format") \
    macro(RequireObjectCoercible, RequireObjectCoercible, "RequireObjectCoercible") \
    macro(resolve, resolve, "resolve") \
    macro(resumeGenerator, resumeGenerator, "resumeGenerator") \
    macro(return, return_, "return") \
    macro(revoke, revoke, "revoke") \
    macro(rtl, rtl, "rtl") \
    macro(script, script, "script") \
    macro(scripts, scripts, "scripts") \
    macro(second, second, "second") \
    macro(selfHosted, selfHosted, "self-hosted") \
    macro(sensitivity, sensitivity, "sensitivity") \
    macro(set, set, "set") \
    macro(SetConstructorInit, SetConstructorInit, "SetConstructorInit") \
    macro(SetIterator, SetIterator, "Set Iterator") \
    macro(setPrefix, setPrefix, "set ") \
    macro(setPrototypeOf, setPrototypeOf, "setPrototypeOf") \
    macro(shape, shape, "shape") \
    macro(size, size, "size") \
    macro(source, source, "source") \
    macro(SpeciesConstructor, SpeciesConstructor, "SpeciesConstructor") \
    macro(stack, stack, "stack") \
    macro(star, star, "*") \
    macro(start, start, "start") \
    macro(startTimestamp, startTimestamp, "startTimestamp") \
    macro(state, state, "state") \
    macro(static, static_, "static") \
    macro(std_Function_apply, std_Function_apply, "std_Function_apply") \
    macro(sticky, sticky, "sticky") \
    macro(StringIterator, StringIterator, "String Iterator") \
    macro(strings, strings, "strings") \
    macro(StructType, StructType, "StructType") \
    macro(style, style, "style") \
    macro(super, super, "super") \
    macro(switch, switch_, "switch") \
    macro(Symbol_iterator_fun, Symbol_iterator_fun, "[Symbol.iterator]") \
    macro(target, target, "target") \
    macro(test, test, "test") \
    macro(then, then, "then") \
    macro(this, this_, "this") \
    macro(throw, throw_, "throw") \
    macro(timestamp, timestamp, "timestamp") \
    macro(timeZone, timeZone, "timeZone") \
    macro(timeZoneName, timeZoneName, "timeZoneName") \
    macro(trimEnd, trimEnd, "trimEnd") \
    macro(trimLeft, trimLeft, "trimLeft") \
    macro(trimRight, trimRight, "trimRight") \
    macro(trimStart, trimStart, "trimStart") \
    macro(toGMTString, toGMTString, "toGMTString") \
    macro(toISOString, toISOString, "toISOString") \
    macro(toJSON, toJSON, "toJSON") \
    macro(toLocaleString, toLocaleString, "toLocaleString") \
    macro(toSource, toSource, "toSource") \
    macro(toString, toString, "toString") \
    macro(toUTCString, toUTCString, "toUTCString") \
    macro(true, true_, "true") \
    macro(try, try_, "try") \
    macro(type, type, "type") \
    macro(typeof, typeof_, "typeof") \
    macro(uint8, uint8, "uint8") \
    macro(uint8Clamped, uint8Clamped, "uint8Clamped") \
    macro(uint16, uint16, "uint16") \
    macro(uint32, uint32, "uint32") \
    macro(Uint8x16, Uint8x16, "Uint8x16") \
    macro(Uint16x8, Uint16x8, "Uint16x8") \
    macro(Uint32x4, Uint32x4, "Uint32x4") \
    macro(unescape, unescape, "unescape") \
    macro(uneval, uneval, "uneval") \
    macro(unicode, unicode, "unicode") \
    macro(unit, unit, "unit") \
    macro(uninitialized, uninitialized, "uninitialized") \
    macro(unsized, unsized, "unsized") \
    macro(unwatch, unwatch, "unwatch") \
    macro(UnwrapAndCallRegExpBuiltinExec, UnwrapAndCallRegExpBuiltinExec, "UnwrapAndCallRegExpBuiltinExec") \
    macro(url, url, "url") \
    macro(usage, usage, "usage") \
    macro(useAsm, useAsm, "use asm") \
    macro(useGrouping, useGrouping, "useGrouping") \
    macro(useStrict, useStrict, "use strict") \
    macro(void, void_, "void") \
    macro(value, value, "value") \
    macro(valueOf, valueOf, "valueOf") \
    macro(values, values, "values") \
    macro(var, var, "var") \
    macro(variable, variable, "variable") \
    macro(void0, void0, "(void 0)") \
    macro(wasm, wasm, "wasm") \
    macro(wasmcall, wasmcall, "wasmcall") \
    macro(watch, watch, "watch") \
    macro(WeakMapConstructorInit, WeakMapConstructorInit, "WeakMapConstructorInit") \
    macro(WeakSetConstructorInit, WeakSetConstructorInit, "WeakSetConstructorInit") \
    macro(WeakSet_add, WeakSet_add, "WeakSet_add") \
    macro(weekday, weekday, "weekday") \
    macro(weekendEnd, weekendEnd, "weekendEnd") \
    macro(weekendStart, weekendStart, "weekendStart") \
    macro(while, while_, "while") \
    macro(with, with, "with") \
    macro(writable, writable, "writable") \
    macro(year, year, "year") \
    macro(yield, yield, "yield") \
    /* Type names must be contiguous and ordered; see js::TypeName. */ \
    macro(undefined, undefined, "undefined") \
    macro(object, object, "object") \
    macro(function, function, "function") \
    macro(string, string, "string") \
    macro(number, number, "number") \
    macro(boolean, boolean, "boolean") \
    macro(null, null, "null") \
    macro(symbol, symbol, "symbol") \
    macro(bigint, bigint, "bigint") \
    macro(defineDataPropertyIntrinsic, defineDataPropertyIntrinsic, "_DefineDataProperty")
=======
#define FOR_EACH_COMMON_PROPERTYNAME(MACRO)                                    \
  MACRO(add, add, "add")                                                       \
  MACRO(allowContentIter, allowContentIter, "allowContentIter")                \
  MACRO(anonymous, anonymous, "anonymous")                                     \
  MACRO(Any, Any, "Any")                                                       \
  MACRO(apply, apply, "apply")                                                 \
  MACRO(args, args, "args")                                                    \
  MACRO(arguments, arguments, "arguments")                                     \
  MACRO(ArrayBufferSpecies, ArrayBufferSpecies, "$ArrayBufferSpecies")         \
  MACRO(ArrayIterator, ArrayIterator, "Array Iterator")                        \
  MACRO(ArrayIteratorNext, ArrayIteratorNext, "ArrayIteratorNext")             \
  MACRO(ArraySort, ArraySort, "ArraySort")                                     \
  MACRO(ArraySpecies, ArraySpecies, "$ArraySpecies")                           \
  MACRO(ArraySpeciesCreate, ArraySpeciesCreate, "ArraySpeciesCreate")          \
  MACRO(ArrayToLocaleString, ArrayToLocaleString, "ArrayToLocaleString")       \
  MACRO(ArrayType, ArrayType, "ArrayType")                                     \
  MACRO(ArrayValues, ArrayValues, "$ArrayValues")                              \
  MACRO(as, as, "as")                                                          \
  MACRO(Async, Async, "Async")                                                 \
  MACRO(AsyncFromSyncIterator, AsyncFromSyncIterator,                          \
        "Async-from-Sync Iterator")                                            \
  MACRO(AsyncFunction, AsyncFunction, "AsyncFunction")                         \
  MACRO(AsyncFunctionNext, AsyncFunctionNext, "AsyncFunctionNext")             \
  MACRO(AsyncFunctionThrow, AsyncFunctionThrow, "AsyncFunctionThrow")          \
  MACRO(AsyncGenerator, AsyncGenerator, "AsyncGenerator")                      \
  MACRO(AsyncGeneratorFunction, AsyncGeneratorFunction,                        \
        "AsyncGeneratorFunction")                                              \
  MACRO(AsyncGeneratorNext, AsyncGeneratorNext, "AsyncGeneratorNext")          \
  MACRO(AsyncGeneratorReturn, AsyncGeneratorReturn, "AsyncGeneratorReturn")    \
  MACRO(AsyncGeneratorThrow, AsyncGeneratorThrow, "AsyncGeneratorThrow")       \
  MACRO(AsyncWrapped, AsyncWrapped, "AsyncWrapped")                            \
  MACRO(async, async, "async")                                                 \
  MACRO(autoAllocateChunkSize, autoAllocateChunkSize, "autoAllocateChunkSize") \
  MACRO(await, await, "await")                                                 \
  MACRO(bigint64, bigint64, "bigint64")                                        \
  MACRO(biguint64, biguint64, "biguint64")                                     \
  MACRO(Bool8x16, Bool8x16, "Bool8x16")                                        \
  MACRO(Bool16x8, Bool16x8, "Bool16x8")                                        \
  MACRO(Bool32x4, Bool32x4, "Bool32x4")                                        \
  MACRO(Bool64x2, Bool64x2, "Bool64x2")                                        \
  MACRO(boundWithSpace, boundWithSpace, "bound ")                              \
  MACRO(break, break_, "break")                                                \
  MACRO(breakdown, breakdown, "breakdown")                                     \
  MACRO(buffer, buffer, "buffer")                                              \
  MACRO(builder, builder, "builder")                                           \
  MACRO(by, by, "by")                                                          \
  MACRO(byob, byob, "byob")                                                    \
  MACRO(byteAlignment, byteAlignment, "byteAlignment")                         \
  MACRO(byteLength, byteLength, "byteLength")                                  \
  MACRO(byteOffset, byteOffset, "byteOffset")                                  \
  MACRO(bytes, bytes, "bytes")                                                 \
  MACRO(BYTES_PER_ELEMENT, BYTES_PER_ELEMENT, "BYTES_PER_ELEMENT")             \
  MACRO(call, call, "call")                                                    \
  MACRO(callContentFunction, callContentFunction, "callContentFunction")       \
  MACRO(callee, callee, "callee")                                              \
  MACRO(caller, caller, "caller")                                              \
  MACRO(callFunction, callFunction, "callFunction")                            \
  MACRO(CallRegExpMethodIfWrapped, CallRegExpMethodIfWrapped,                  \
        "CallRegExpMethodIfWrapped")                                           \
  MACRO(cancel, cancel, "cancel")                                              \
  MACRO(case, case_, "case")                                                   \
  MACRO(caseFirst, caseFirst, "caseFirst")                                     \
  MACRO(catch, catch_, "catch")                                                \
  MACRO(class, class_, "class")                                                \
  MACRO(Collator, Collator, "Collator")                                        \
  MACRO(collections, collections, "collections")                               \
  MACRO(columnNumber, columnNumber, "columnNumber")                            \
  MACRO(comma, comma, ",")                                                     \
  MACRO(compare, compare, "compare")                                           \
  MACRO(configurable, configurable, "configurable")                            \
  MACRO(const, const_, "const")                                                \
  MACRO(construct, construct, "construct")                                     \
  MACRO(constructContentFunction, constructContentFunction,                    \
        "constructContentFunction")                                            \
  MACRO(constructor, constructor, "constructor")                               \
  MACRO(continue, continue_, "continue")                                       \
  MACRO(ConvertAndCopyTo, ConvertAndCopyTo, "ConvertAndCopyTo")                \
  MACRO(CopyDataProperties, CopyDataProperties, "CopyDataProperties")          \
  MACRO(CopyDataPropertiesUnfiltered, CopyDataPropertiesUnfiltered,            \
        "CopyDataPropertiesUnfiltered")                                        \
  MACRO(copyWithin, copyWithin, "copyWithin")                                  \
  MACRO(count, count, "count")                                                 \
  MACRO(CreateResolvingFunctions, CreateResolvingFunctions,                    \
        "CreateResolvingFunctions")                                            \
  MACRO(currency, currency, "currency")                                        \
  MACRO(currencyDisplay, currencyDisplay, "currencyDisplay")                   \
  MACRO(DateTimeFormat, DateTimeFormat, "DateTimeFormat")                      \
  MACRO(day, day, "day")                                                       \
  MACRO(dayPeriod, dayPeriod, "dayPeriod")                                     \
  MACRO(debugger, debugger, "debugger")                                        \
  MACRO(decimal, decimal, "decimal")                                           \
  MACRO(decodeURI, decodeURI, "decodeURI")                                     \
  MACRO(decodeURIComponent, decodeURIComponent, "decodeURIComponent")          \
  MACRO(DefaultBaseClassConstructor, DefaultBaseClassConstructor,              \
        "DefaultBaseClassConstructor")                                         \
  MACRO(DefaultDerivedClassConstructor, DefaultDerivedClassConstructor,        \
        "DefaultDerivedClassConstructor")                                      \
  MACRO(default, default_, "default")                                          \
  MACRO(defineGetter, defineGetter, "__defineGetter__")                        \
  MACRO(defineProperty, defineProperty, "defineProperty")                      \
  MACRO(defineSetter, defineSetter, "__defineSetter__")                        \
  MACRO(delete, delete_, "delete")                                             \
  MACRO(deleteProperty, deleteProperty, "deleteProperty")                      \
  MACRO(direction, direction, "direction")                                     \
  MACRO(displayURL, displayURL, "displayURL")                                  \
  MACRO(do, do_, "do")                                                         \
  MACRO(domNode, domNode, "domNode")                                           \
  MACRO(done, done, "done")                                                    \
  MACRO(dotGenerator, dotGenerator, ".generator")                              \
  MACRO(dotThis, dotThis, ".this")                                             \
  MACRO(dotInitializers, dotInitializers, ".initializers")                     \
  MACRO(dotFieldKeys, dotFieldKeys, ".fieldKeys")                              \
  MACRO(each, each, "each")                                                    \
  MACRO(elementType, elementType, "elementType")                               \
  MACRO(else, else_, "else")                                                   \
  MACRO(empty, empty, "")                                                      \
  MACRO(emptyRegExp, emptyRegExp, "(?:)")                                      \
  MACRO(encodeURI, encodeURI, "encodeURI")                                     \
  MACRO(encodeURIComponent, encodeURIComponent, "encodeURIComponent")          \
  MACRO(endTimestamp, endTimestamp, "endTimestamp")                            \
  MACRO(entries, entries, "entries")                                           \
  MACRO(enum, enum_, "enum")                                                   \
  MACRO(enumerable, enumerable, "enumerable")                                  \
  MACRO(enumerate, enumerate, "enumerate")                                     \
  MACRO(era, era, "era")                                                       \
  MACRO(ErrorToStringWithTrailingNewline, ErrorToStringWithTrailingNewline,    \
        "ErrorToStringWithTrailingNewline")                                    \
  MACRO(escape, escape, "escape")                                              \
  MACRO(eval, eval, "eval")                                                    \
  MACRO(exec, exec, "exec")                                                    \
  MACRO(export, export_, "export")                                             \
  MACRO(extends, extends, "extends")                                           \
  MACRO(false, false_, "false")                                                \
  MACRO(fieldOffsets, fieldOffsets, "fieldOffsets")                            \
  MACRO(fieldTypes, fieldTypes, "fieldTypes")                                  \
  MACRO(fileName, fileName, "fileName")                                        \
  MACRO(fill, fill, "fill")                                                    \
  MACRO(finally, finally_, "finally")                                          \
  MACRO(find, find, "find")                                                    \
  MACRO(findIndex, findIndex, "findIndex")                                     \
  MACRO(firstDayOfWeek, firstDayOfWeek, "firstDayOfWeek")                      \
  MACRO(fix, fix, "fix")                                                       \
  MACRO(flags, flags, "flags")                                                 \
  MACRO(flat, flat, "flat")                                                    \
  MACRO(flatMap, flatMap, "flatMap")                                           \
  MACRO(float32, float32, "float32")                                           \
  MACRO(Float32x4, Float32x4, "Float32x4")                                     \
  MACRO(float64, float64, "float64")                                           \
  MACRO(Float64x2, Float64x2, "Float64x2")                                     \
    MACRO(for, for_, "for")                                                    \
  MACRO(forceInterpreter, forceInterpreter, "forceInterpreter")                \
  MACRO(forEach, forEach, "forEach")                                           \
  MACRO(format, format, "format")                                              \
  MACRO(fraction, fraction, "fraction")                                        \
  MACRO(frame, frame, "frame")                                                 \
  MACRO(from, from, "from")                                                    \
  MACRO(fulfilled, fulfilled, "fulfilled")                                     \
  MACRO(futexNotEqual, futexNotEqual, "not-equal")                             \
  MACRO(futexOK, futexOK, "ok")                                                \
  MACRO(futexTimedOut, futexTimedOut, "timed-out")                             \
  MACRO(gcCycleNumber, gcCycleNumber, "gcCycleNumber")                         \
  MACRO(Generator, Generator, "Generator")                                     \
  MACRO(GeneratorFunction, GeneratorFunction, "GeneratorFunction")             \
  MACRO(GeneratorNext, GeneratorNext, "GeneratorNext")                         \
  MACRO(GeneratorReturn, GeneratorReturn, "GeneratorReturn")                   \
  MACRO(GeneratorThrow, GeneratorThrow, "GeneratorThrow")                      \
  MACRO(get, get, "get")                                                       \
  MACRO(GetInternalError, GetInternalError, "GetInternalError")                \
  MACRO(getBigInt64, getBigInt64, "getBigInt64")                               \
  MACRO(getBigUint64, getBigUint64, "getBigUint64")                            \
  MACRO(getInternals, getInternals, "getInternals")                            \
  MACRO(GetModuleNamespace, GetModuleNamespace, "GetModuleNamespace")          \
  MACRO(getOwnPropertyDescriptor, getOwnPropertyDescriptor,                    \
        "getOwnPropertyDescriptor")                                            \
  MACRO(getOwnPropertyNames, getOwnPropertyNames, "getOwnPropertyNames")       \
  MACRO(getPrefix, getPrefix, "get ")                                          \
  MACRO(getPropertySuper, getPropertySuper, "getPropertySuper")                \
  MACRO(getPrototypeOf, getPrototypeOf, "getPrototypeOf")                      \
  MACRO(GetTypeError, GetTypeError, "GetTypeError")                            \
  MACRO(global, global, "global")                                              \
  MACRO(globalThis, globalThis, "globalThis")                                  \
  MACRO(group, group, "group")                                                 \
  MACRO(Handle, Handle, "Handle")                                              \
  MACRO(has, has, "has")                                                       \
  MACRO(hasOwn, hasOwn, "hasOwn")                                              \
  MACRO(hasOwnProperty, hasOwnProperty, "hasOwnProperty")                      \
  MACRO(highWaterMark, highWaterMark, "highWaterMark")                         \
  MACRO(hour, hour, "hour")                                                    \
  MACRO(if, if_, "if")                                                         \
  MACRO(ignoreCase, ignoreCase, "ignoreCase")                                  \
  MACRO(ignorePunctuation, ignorePunctuation, "ignorePunctuation")             \
  MACRO(implements, implements, "implements")                                  \
  MACRO(import, import, "import")                                              \
  MACRO(in, in, "in")                                                          \
  MACRO(includes, includes, "includes")                                        \
  MACRO(incumbentGlobal, incumbentGlobal, "incumbentGlobal")                   \
  MACRO(index, index, "index")                                                 \
  MACRO(infinity, infinity, "infinity")                                        \
  MACRO(Infinity, Infinity, "Infinity")                                        \
  MACRO(initial, initial, "initial")                                           \
  MACRO(InitializeCollator, InitializeCollator, "InitializeCollator")          \
  MACRO(InitializeDateTimeFormat, InitializeDateTimeFormat,                    \
        "InitializeDateTimeFormat")                                            \
  MACRO(InitializeNumberFormat, InitializeNumberFormat,                        \
        "InitializeNumberFormat")                                              \
  MACRO(InitializePluralRules, InitializePluralRules, "InitializePluralRules") \
  MACRO(InitializeRelativeTimeFormat, InitializeRelativeTimeFormat,            \
        "InitializeRelativeTimeFormat")                                        \
  MACRO(innermost, innermost, "innermost")                                     \
  MACRO(inNursery, inNursery, "inNursery")                                     \
  MACRO(input, input, "input")                                                 \
  MACRO(instanceof, instanceof, "instanceof")                                  \
  MACRO(int8, int8, "int8")                                                    \
  MACRO(int16, int16, "int16")                                                 \
  MACRO(int32, int32, "int32")                                                 \
  MACRO(Int8x16, Int8x16, "Int8x16")                                           \
  MACRO(Int16x8, Int16x8, "Int16x8")                                           \
  MACRO(Int32x4, Int32x4, "Int32x4")                                           \
  MACRO(integer, integer, "integer")                                           \
  MACRO(interface, interface, "interface")                                     \
  MACRO(InterpretGeneratorResume, InterpretGeneratorResume,                    \
        "InterpretGeneratorResume")                                            \
  MACRO(InvalidDate, InvalidDate, "Invalid Date")                              \
  MACRO(isBreakpoint, isBreakpoint, "isBreakpoint")                            \
  MACRO(isEntryPoint, isEntryPoint, "isEntryPoint")                            \
  MACRO(isExtensible, isExtensible, "isExtensible")                            \
  MACRO(isFinite, isFinite, "isFinite")                                        \
  MACRO(isNaN, isNaN, "isNaN")                                                 \
  MACRO(isPrototypeOf, isPrototypeOf, "isPrototypeOf")                         \
  MACRO(isStepStart, isStepStart, "isStepStart")                               \
  MACRO(IterableToList, IterableToList, "IterableToList")                      \
  MACRO(iterate, iterate, "iterate")                                           \
  MACRO(join, join, "join")                                                    \
  MACRO(js, js, "js")                                                          \
  MACRO(keys, keys, "keys")                                                    \
  MACRO(label, label, "label")                                                 \
  MACRO(lastIndex, lastIndex, "lastIndex")                                     \
  MACRO(length, length, "length")                                              \
  MACRO(let, let, "let")                                                       \
  MACRO(line, line, "line")                                                    \
  MACRO(lineNumber, lineNumber, "lineNumber")                                  \
  MACRO(literal, literal, "literal")                                           \
  MACRO(loc, loc, "loc")                                                       \
  MACRO(locale, locale, "locale")                                              \
  MACRO(lookupGetter, lookupGetter, "__lookupGetter__")                        \
  MACRO(lookupSetter, lookupSetter, "__lookupSetter__")                        \
  MACRO(ltr, ltr, "ltr")                                                       \
  MACRO(MapConstructorInit, MapConstructorInit, "MapConstructorInit")          \
  MACRO(MapIterator, MapIterator, "Map Iterator")                              \
  MACRO(maxColumn, maxColumn, "maxColumn")                                     \
  MACRO(maximumFractionDigits, maximumFractionDigits, "maximumFractionDigits") \
  MACRO(maximumSignificantDigits, maximumSignificantDigits,                    \
        "maximumSignificantDigits")                                            \
  MACRO(maxLine, maxLine, "maxLine")                                           \
  MACRO(maxOffset, maxOffset, "maxOffset")                                     \
  MACRO(message, message, "message")                                           \
  MACRO(meta, meta, "meta")                                                    \
  MACRO(minColumn, minColumn, "minColumn")                                     \
  MACRO(minDays, minDays, "minDays")                                           \
  MACRO(minimumFractionDigits, minimumFractionDigits, "minimumFractionDigits") \
  MACRO(minimumIntegerDigits, minimumIntegerDigits, "minimumIntegerDigits")    \
  MACRO(minimumSignificantDigits, minimumSignificantDigits,                    \
        "minimumSignificantDigits")                                            \
  MACRO(minLine, minLine, "minLine")                                           \
  MACRO(minOffset, minOffset, "minOffset")                                     \
  MACRO(minusSign, minusSign, "minusSign")                                     \
  MACRO(minute, minute, "minute")                                              \
  MACRO(missingArguments, missingArguments, "missingArguments")                \
  MACRO(mode, mode, "mode")                                                    \
  MACRO(module, module, "module")                                              \
  MACRO(Module, Module, "Module")                                              \
  MACRO(ModuleInstantiate, ModuleInstantiate, "ModuleInstantiate")             \
  MACRO(ModuleEvaluate, ModuleEvaluate, "ModuleEvaluate")                      \
  MACRO(month, month, "month")                                                 \
  MACRO(multiline, multiline, "multiline")                                     \
  MACRO(name, name, "name")                                                    \
  MACRO(nan, nan, "nan")                                                       \
  MACRO(NaN, NaN, "NaN")                                                       \
  MACRO(NegativeInfinity, NegativeInfinity, "-Infinity")                       \
  MACRO(new, new_, "new")                                                      \
  MACRO(next, next, "next")                                                    \
  MACRO(NFC, NFC, "NFC")                                                       \
  MACRO(NFD, NFD, "NFD")                                                       \
  MACRO(NFKC, NFKC, "NFKC")                                                    \
  MACRO(NFKD, NFKD, "NFKD")                                                    \
  MACRO(noFilename, noFilename, "noFilename")                                  \
  MACRO(nonincrementalReason, nonincrementalReason, "nonincrementalReason")    \
  MACRO(noStack, noStack, "noStack")                                           \
  MACRO(notes, notes, "notes")                                                 \
  MACRO(NumberFormat, NumberFormat, "NumberFormat")                            \
  MACRO(numeric, numeric, "numeric")                                           \
  MACRO(objectArguments, objectArguments, "[object Arguments]")                \
  MACRO(objectArray, objectArray, "[object Array]")                            \
  MACRO(objectBigInt, objectBigInt, "[object BigInt]")                         \
  MACRO(objectBoolean, objectBoolean, "[object Boolean]")                      \
  MACRO(objectDate, objectDate, "[object Date]")                               \
  MACRO(objectError, objectError, "[object Error]")                            \
  MACRO(objectFunction, objectFunction, "[object Function]")                   \
  MACRO(objectNull, objectNull, "[object Null]")                               \
  MACRO(objectNumber, objectNumber, "[object Number]")                         \
  MACRO(objectObject, objectObject, "[object Object]")                         \
  MACRO(objectRegExp, objectRegExp, "[object RegExp]")                         \
  MACRO(objects, objects, "objects")                                           \
  MACRO(objectString, objectString, "[object String]")                         \
  MACRO(objectUndefined, objectUndefined, "[object Undefined]")                \
  MACRO(of, of, "of")                                                          \
  MACRO(offset, offset, "offset")                                              \
  MACRO(optimizedOut, optimizedOut, "optimizedOut")                            \
  MACRO(other, other, "other")                                                 \
  MACRO(outOfMemory, outOfMemory, "out of memory")                             \
  MACRO(ownKeys, ownKeys, "ownKeys")                                           \
  MACRO(Object_valueOf, Object_valueOf, "Object_valueOf")                      \
  MACRO(package, package, "package")                                           \
  MACRO(parseFloat, parseFloat, "parseFloat")                                  \
  MACRO(parseInt, parseInt, "parseInt")                                        \
  MACRO(pattern, pattern, "pattern")                                           \
  MACRO(pending, pending, "pending")                                           \
  MACRO(PluralRules, PluralRules, "PluralRules")                               \
  MACRO(percentSign, percentSign, "percentSign")                               \
  MACRO(plusSign, plusSign, "plusSign")                                        \
  MACRO(public, public_, "public")                                             \
  MACRO(pull, pull, "pull")                                                    \
  MACRO(preventExtensions, preventExtensions, "preventExtensions")             \
  MACRO(private, private_, "private")                                          \
  MACRO(promise, promise, "promise")                                           \
  MACRO(propertyIsEnumerable, propertyIsEnumerable, "propertyIsEnumerable")    \
  MACRO(protected, protected_, "protected")                                    \
  MACRO(proto, proto, "__proto__")                                             \
  MACRO(prototype, prototype, "prototype")                                     \
  MACRO(proxy, proxy, "proxy")                                                 \
  MACRO(raw, raw, "raw")                                                       \
  MACRO(reason, reason, "reason")                                              \
  MACRO(RegExpBuiltinExec, RegExpBuiltinExec, "RegExpBuiltinExec")             \
  MACRO(RegExpFlagsGetter, RegExpFlagsGetter, "$RegExpFlagsGetter")            \
  MACRO(RegExpMatcher, RegExpMatcher, "RegExpMatcher")                         \
  MACRO(RegExpSearcher, RegExpSearcher, "RegExpSearcher")                      \
  MACRO(RegExpStringIterator, RegExpStringIterator, "RegExp String Iterator")  \
  MACRO(RegExpTester, RegExpTester, "RegExpTester")                            \
  MACRO(RegExp_prototype_Exec, RegExp_prototype_Exec, "RegExp_prototype_Exec") \
  MACRO(Reify, Reify, "Reify")                                                 \
  MACRO(reject, reject, "reject")                                              \
  MACRO(rejected, rejected, "rejected")                                        \
  MACRO(RelativeTimeFormat, RelativeTimeFormat, "RelativeTimeFormat")          \
  MACRO(RelativeTimeFormatFormat, RelativeTimeFormatFormat,                    \
        "Intl_RelativeTimeFormat_Format")                                      \
  MACRO(RequireObjectCoercible, RequireObjectCoercible,                        \
        "RequireObjectCoercible")                                              \
  MACRO(resolve, resolve, "resolve")                                           \
  MACRO(resumeGenerator, resumeGenerator, "resumeGenerator")                   \
  MACRO(return, return_, "return")                                             \
  MACRO(revoke, revoke, "revoke")                                              \
  MACRO(rtl, rtl, "rtl")                                                       \
  MACRO(script, script, "script")                                              \
  MACRO(scripts, scripts, "scripts")                                           \
  MACRO(second, second, "second")                                              \
  MACRO(selfHosted, selfHosted, "self-hosted")                                 \
  MACRO(sensitivity, sensitivity, "sensitivity")                               \
  MACRO(set, set, "set")                                                       \
  MACRO(setBigInt64, setBigInt64, "setBigInt64")                               \
  MACRO(setBigUint64, setBigUint64, "setBigUint64")                            \
  MACRO(SetConstructorInit, SetConstructorInit, "SetConstructorInit")          \
  MACRO(SetIterator, SetIterator, "Set Iterator")                              \
  MACRO(setPrefix, setPrefix, "set ")                                          \
  MACRO(setPrototypeOf, setPrototypeOf, "setPrototypeOf")                      \
  MACRO(shape, shape, "shape")                                                 \
  MACRO(size, size, "size")                                                    \
  MACRO(source, source, "source")                                              \
  MACRO(SpeciesConstructor, SpeciesConstructor, "SpeciesConstructor")          \
  MACRO(stack, stack, "stack")                                                 \
  MACRO(star, star, "*")                                                       \
  MACRO(start, start, "start")                                                 \
  MACRO(startTimestamp, startTimestamp, "startTimestamp")                      \
  MACRO(state, state, "state")                                                 \
  MACRO(static, static_, "static")                                             \
  MACRO(status, status, "status")                                              \
  MACRO(std_Function_apply, std_Function_apply, "std_Function_apply")          \
  MACRO(sticky, sticky, "sticky")                                              \
  MACRO(StringIterator, StringIterator, "String Iterator")                     \
  MACRO(strings, strings, "strings")                                           \
  MACRO(String_split, String_split, "String_split")                            \
  MACRO(StructType, StructType, "StructType")                                  \
  MACRO(style, style, "style")                                                 \
  MACRO(super, super, "super")                                                 \
  MACRO(switch, switch_, "switch")                                             \
  MACRO(Symbol_iterator_fun, Symbol_iterator_fun, "[Symbol.iterator]")         \
  MACRO(target, target, "target")                                              \
  MACRO(test, test, "test")                                                    \
  MACRO(then, then, "then")                                                    \
  MACRO(this, this_, "this")                                                   \
  MACRO(throw, throw_, "throw")                                                \
  MACRO(timestamp, timestamp, "timestamp")                                     \
  MACRO(timeZone, timeZone, "timeZone")                                        \
  MACRO(timeZoneName, timeZoneName, "timeZoneName")                            \
  MACRO(trimEnd, trimEnd, "trimEnd")                                           \
  MACRO(trimLeft, trimLeft, "trimLeft")                                        \
  MACRO(trimRight, trimRight, "trimRight")                                     \
  MACRO(trimStart, trimStart, "trimStart")                                     \
  MACRO(toGMTString, toGMTString, "toGMTString")                               \
  MACRO(toISOString, toISOString, "toISOString")                               \
  MACRO(toJSON, toJSON, "toJSON")                                              \
  MACRO(toLocaleString, toLocaleString, "toLocaleString")                      \
  MACRO(toSource, toSource, "toSource")                                        \
  MACRO(toString, toString, "toString")                                        \
  MACRO(toUTCString, toUTCString, "toUTCString")                               \
  MACRO(true, true_, "true")                                                   \
  MACRO(try, try_, "try")                                                      \
  MACRO(type, type, "type")                                                    \
  MACRO(typeof, typeof_, "typeof")                                             \
  MACRO(uint8, uint8, "uint8")                                                 \
  MACRO(uint8Clamped, uint8Clamped, "uint8Clamped")                            \
  MACRO(uint16, uint16, "uint16")                                              \
  MACRO(uint32, uint32, "uint32")                                              \
  MACRO(Uint8x16, Uint8x16, "Uint8x16")                                        \
  MACRO(Uint16x8, Uint16x8, "Uint16x8")                                        \
  MACRO(Uint32x4, Uint32x4, "Uint32x4")                                        \
  MACRO(unescape, unescape, "unescape")                                        \
  MACRO(uneval, uneval, "uneval")                                              \
  MACRO(unicode, unicode, "unicode")                                           \
  MACRO(unit, unit, "unit")                                                    \
  MACRO(uninitialized, uninitialized, "uninitialized")                         \
  MACRO(unsized, unsized, "unsized")                                           \
  MACRO(unwatch, unwatch, "unwatch")                                           \
  MACRO(UnwrapAndCallRegExpBuiltinExec, UnwrapAndCallRegExpBuiltinExec,        \
        "UnwrapAndCallRegExpBuiltinExec")                                      \
  MACRO(url, url, "url")                                                       \
  MACRO(usage, usage, "usage")                                                 \
  MACRO(useAsm, useAsm, "use asm")                                             \
  MACRO(useGrouping, useGrouping, "useGrouping")                               \
  MACRO(useStrict, useStrict, "use strict")                                    \
  MACRO(void, void_, "void")                                                   \
  MACRO(value, value, "value")                                                 \
  MACRO(valueOf, valueOf, "valueOf")                                           \
  MACRO(values, values, "values")                                              \
  MACRO(var, var, "var")                                                       \
  MACRO(variable, variable, "variable")                                        \
  MACRO(void0, void0, "(void 0)")                                              \
  MACRO(wasm, wasm, "wasm")                                                    \
  MACRO(WasmAnyRef, WasmAnyRef, "WasmAnyRef")                                  \
  MACRO(wasmcall, wasmcall, "wasmcall")                                        \
  MACRO(watch, watch, "watch")                                                 \
  MACRO(WeakMapConstructorInit, WeakMapConstructorInit,                        \
        "WeakMapConstructorInit")                                              \
  MACRO(WeakSetConstructorInit, WeakSetConstructorInit,                        \
        "WeakSetConstructorInit")                                              \
  MACRO(WeakSet_add, WeakSet_add, "WeakSet_add")                               \
  MACRO(weekday, weekday, "weekday")                                           \
  MACRO(weekendEnd, weekendEnd, "weekendEnd")                                  \
  MACRO(weekendStart, weekendStart, "weekendStart")                            \
  MACRO(while, while_, "while")                                                \
  MACRO(with, with, "with")                                                    \
  MACRO(writable, writable, "writable")                                        \
  MACRO(year, year, "year")                                                    \
  MACRO(yield, yield, "yield")                                                 \
  /* Type names must be contiguous and ordered; see js::TypeName. */           \
  MACRO(undefined, undefined, "undefined")                                     \
  MACRO(object, object, "object")                                              \
  MACRO(function, function, "function")                                        \
  MACRO(string, string, "string")                                              \
  MACRO(number, number, "number")                                              \
  MACRO(boolean, boolean, "boolean")                                           \
  MACRO(null, null, "null")                                                    \
  MACRO(symbol, symbol, "symbol")                                              \
  MACRO(bigint, bigint, "bigint")                                              \
  MACRO(defineDataPropertyIntrinsic, defineDataPropertyIntrinsic,              \
        "_DefineDataProperty")
>>>>>>> upstream-releases

#endif /* vm_CommonPropertyNames_h */
