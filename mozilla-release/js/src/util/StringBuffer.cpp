/* -*- Mode: C++; tab-width: 8; indent-tabs-mode: nil; c-basic-offset: 2 -*-
 * vim: set ts=8 sts=2 et sw=2 tw=80:
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/. */

#include "util/StringBuffer.h"

#include "mozilla/Range.h"
#include "mozilla/Unused.h"

#include "vm/JSObject-inl.h"
#include "vm/StringType-inl.h"

using namespace js;

template <typename CharT, class Buffer>
<<<<<<< HEAD
static CharT* ExtractWellSized(JSContext* cx, Buffer& cb) {
  size_t capacity = cb.capacity();
  size_t length = cb.length();

  CharT* buf = cb.extractOrCopyRawBuffer();
  if (!buf) {
    return nullptr;
  }

  /* For medium/big buffers, avoid wasting more than 1/4 of the memory. */
  MOZ_ASSERT(capacity >= length);
  if (length > Buffer::sMaxInlineStorage && capacity - length > length / 4) {
    CharT* tmp = cx->pod_realloc<CharT>(buf, capacity, length + 1);
    if (!tmp) {
      js_free(buf);
      return nullptr;
    }
    buf = tmp;
  }

  return buf;
}
||||||| merged common ancestors
static CharT*
ExtractWellSized(JSContext* cx, Buffer& cb)
{
    size_t capacity = cb.capacity();
    size_t length = cb.length();

    CharT* buf = cb.extractOrCopyRawBuffer();
    if (!buf) {
        return nullptr;
    }
=======
static CharT* ExtractWellSized(Buffer& cb) {
  size_t capacity = cb.capacity();
  size_t length = cb.length();
  StringBufferAllocPolicy allocPolicy = cb.allocPolicy();

  CharT* buf = cb.extractOrCopyRawBuffer();
  if (!buf) {
    return nullptr;
  }

  /* For medium/big buffers, avoid wasting more than 1/4 of the memory. */
  MOZ_ASSERT(capacity >= length);
  if (length > Buffer::sMaxInlineStorage && capacity - length > length / 4) {
    CharT* tmp = allocPolicy.pod_realloc<CharT>(buf, capacity, length + 1);
    if (!tmp) {
      allocPolicy.free_(buf);
      return nullptr;
    }
    buf = tmp;
  }

  return buf;
}
>>>>>>> upstream-releases

char16_t* StringBuffer::stealChars() {
  if (isLatin1() && !inflateChars()) {
    return nullptr;
  }

<<<<<<< HEAD
  return ExtractWellSized<char16_t>(cx, twoByteChars());
||||||| merged common ancestors
    return buf;
=======
  return ExtractWellSized<char16_t>(twoByteChars());
>>>>>>> upstream-releases
}

bool StringBuffer::inflateChars() {
  MOZ_ASSERT(isLatin1());

<<<<<<< HEAD
  TwoByteCharBuffer twoByte(cx);
||||||| merged common ancestors
    return ExtractWellSized<char16_t>(cx, twoByteChars());
}
=======
  TwoByteCharBuffer twoByte(StringBufferAllocPolicy{cx_, arenaId_});
>>>>>>> upstream-releases

  /*
   * Note: we don't use Vector::capacity() because it always returns a
   * value >= sInlineCapacity. Since Latin1CharBuffer::sInlineCapacity >
   * TwoByteCharBuffer::sInlineCapacitychars, we'd always malloc here.
   */
  size_t capacity = Max(reserved_, latin1Chars().length());
  if (!twoByte.reserve(capacity)) {
    return false;
  }

  twoByte.infallibleAppend(latin1Chars().begin(), latin1Chars().length());

  cb.destroy();
  cb.construct<TwoByteCharBuffer>(std::move(twoByte));
  return true;
}

<<<<<<< HEAD
template <typename CharT, class Buffer>
static JSFlatString* FinishStringFlat(JSContext* cx, StringBuffer& sb,
                                      Buffer& cb) {
  size_t len = sb.length();
  if (!sb.append('\0')) {
    return nullptr;
  }

  UniquePtr<CharT[], JS::FreePolicy> buf(ExtractWellSized<CharT>(cx, cb));
  if (!buf) {
    return nullptr;
  }

  JSFlatString* str = NewStringDontDeflate<CanGC>(cx, std::move(buf), len);
  if (!str) {
    return nullptr;
  }

  /*
   * The allocation was made on a TempAllocPolicy, so account for the string
   * data on the string's zone.
   */
  cx->updateMallocCounter(sizeof(CharT) * len);

  return str;
}
||||||| merged common ancestors
template <typename CharT, class Buffer>
static JSFlatString*
FinishStringFlat(JSContext* cx, StringBuffer& sb, Buffer& cb)
{
    size_t len = sb.length();
    if (!sb.append('\0')) {
        return nullptr;
    }

    UniquePtr<CharT[], JS::FreePolicy> buf(ExtractWellSized<CharT>(cx, cb));
    if (!buf) {
        return nullptr;
    }

    JSFlatString* str = NewStringDontDeflate<CanGC>(cx, std::move(buf), len);
    if (!str) {
        return nullptr;
    }
=======
template <typename CharT>
JSFlatString* StringBuffer::finishStringInternal(JSContext* cx) {
  size_t len = length();

  if (JSInlineString::lengthFits<CharT>(len)) {
    mozilla::Range<const CharT> range(begin<CharT>(), len);
    return NewInlineString<CanGC>(cx, range);
  }

  if (!append('\0')) {
    return nullptr;
  }

  UniquePtr<CharT[], JS::FreePolicy> buf(
      ExtractWellSized<CharT>(chars<CharT>()));
>>>>>>> upstream-releases

<<<<<<< HEAD
JSFlatString* StringBuffer::finishString() {
  size_t len = length();
  if (len == 0) {
    return cx->names().empty;
  }
||||||| merged common ancestors
    /*
     * The allocation was made on a TempAllocPolicy, so account for the string
     * data on the string's zone.
     */
    cx->updateMallocCounter(sizeof(CharT) * len);
=======
  if (!buf) {
    return nullptr;
  }
>>>>>>> upstream-releases

<<<<<<< HEAD
  if (!JSString::validateLength(cx, len)) {
    return nullptr;
  }
||||||| merged common ancestors
    return str;
}
=======
  JSFlatString* str = NewStringDontDeflate<CanGC>(cx, std::move(buf), len);
  if (!str) {
    return nullptr;
  }

  /*
   * The allocation was made on a TempAllocPolicy, so account for the string
   * data on the string's zone.
   */
  cx->updateMallocCounter(sizeof(CharT) * len);

  return str;
}
>>>>>>> upstream-releases

<<<<<<< HEAD
  JS_STATIC_ASSERT(JSFatInlineString::MAX_LENGTH_TWO_BYTE <
                   TwoByteCharBuffer::InlineLength);
  JS_STATIC_ASSERT(JSFatInlineString::MAX_LENGTH_LATIN1 <
                   Latin1CharBuffer::InlineLength);
||||||| merged common ancestors
JSFlatString*
StringBuffer::finishString()
{
    size_t len = length();
    if (len == 0) {
        return cx->names().empty;
    }
=======
JSFlatString* JSStringBuilder::finishString() {
  size_t len = length();
  if (len == 0) {
    return cx_->names().empty;
  }
>>>>>>> upstream-releases

<<<<<<< HEAD
  if (isLatin1()) {
    if (JSInlineString::lengthFits<Latin1Char>(len)) {
      mozilla::Range<const Latin1Char> range(latin1Chars().begin(), len);
      return NewInlineString<CanGC>(cx, range);
    }
  } else {
    if (JSInlineString::lengthFits<char16_t>(len)) {
      mozilla::Range<const char16_t> range(twoByteChars().begin(), len);
      return NewInlineString<CanGC>(cx, range);
    }
  }
||||||| merged common ancestors
    if (!JSString::validateLength(cx, len)) {
        return nullptr;
    }

    JS_STATIC_ASSERT(JSFatInlineString::MAX_LENGTH_TWO_BYTE < TwoByteCharBuffer::InlineLength);
    JS_STATIC_ASSERT(JSFatInlineString::MAX_LENGTH_LATIN1 < Latin1CharBuffer::InlineLength);

    if (isLatin1()) {
        if (JSInlineString::lengthFits<Latin1Char>(len)) {
            mozilla::Range<const Latin1Char> range(latin1Chars().begin(), len);
            return NewInlineString<CanGC>(cx, range);
        }
    } else {
        if (JSInlineString::lengthFits<char16_t>(len)) {
            mozilla::Range<const char16_t> range(twoByteChars().begin(), len);
            return NewInlineString<CanGC>(cx, range);
        }
    }
=======
  if (!JSString::validateLength(cx_, len)) {
    return nullptr;
  }

  JS_STATIC_ASSERT(JSFatInlineString::MAX_LENGTH_TWO_BYTE <
                   TwoByteCharBuffer::InlineLength);
  JS_STATIC_ASSERT(JSFatInlineString::MAX_LENGTH_LATIN1 <
                   Latin1CharBuffer::InlineLength);
>>>>>>> upstream-releases

<<<<<<< HEAD
  return isLatin1() ? FinishStringFlat<Latin1Char>(cx, *this, latin1Chars())
                    : FinishStringFlat<char16_t>(cx, *this, twoByteChars());
||||||| merged common ancestors
    return isLatin1()
        ? FinishStringFlat<Latin1Char>(cx, *this, latin1Chars())
        : FinishStringFlat<char16_t>(cx, *this, twoByteChars());
=======
  return isLatin1() ? finishStringInternal<Latin1Char>(cx_)
                    : finishStringInternal<char16_t>(cx_);
>>>>>>> upstream-releases
}

<<<<<<< HEAD
JSAtom* StringBuffer::finishAtom() {
  size_t len = length();
  if (len == 0) {
    return cx->names().empty;
  }
||||||| merged common ancestors
JSAtom*
StringBuffer::finishAtom()
{
    size_t len = length();
    if (len == 0) {
        return cx->names().empty;
    }
=======
JSAtom* StringBuffer::finishAtom() {
  size_t len = length();
  if (len == 0) {
    return cx_->names().empty;
  }
>>>>>>> upstream-releases

<<<<<<< HEAD
  if (isLatin1()) {
    JSAtom* atom = AtomizeChars(cx, latin1Chars().begin(), len);
    latin1Chars().clear();
||||||| merged common ancestors
    if (isLatin1()) {
        JSAtom* atom = AtomizeChars(cx, latin1Chars().begin(), len);
        latin1Chars().clear();
        return atom;
    }

    JSAtom* atom = AtomizeChars(cx, twoByteChars().begin(), len);
    twoByteChars().clear();
=======
  if (isLatin1()) {
    JSAtom* atom = AtomizeChars(cx_, latin1Chars().begin(), len);
    latin1Chars().clear();
>>>>>>> upstream-releases
    return atom;
  }

<<<<<<< HEAD
  JSAtom* atom = AtomizeChars(cx, twoByteChars().begin(), len);
  twoByteChars().clear();
  return atom;
}
||||||| merged common ancestors
bool
js::ValueToStringBufferSlow(JSContext* cx, const Value& arg, StringBuffer& sb)
{
    RootedValue v(cx, arg);
    if (!ToPrimitive(cx, JSTYPE_STRING, &v)) {
        return false;
    }
=======
  JSAtom* atom = AtomizeChars(cx_, twoByteChars().begin(), len);
  twoByteChars().clear();
  return atom;
}
>>>>>>> upstream-releases

<<<<<<< HEAD
bool js::ValueToStringBufferSlow(JSContext* cx, const Value& arg,
                                 StringBuffer& sb) {
  RootedValue v(cx, arg);
  if (!ToPrimitive(cx, JSTYPE_STRING, &v)) {
    return false;
  }

  if (v.isString()) {
    return sb.append(v.toString());
  }
  if (v.isNumber()) {
    return NumberValueToStringBuffer(cx, v, sb);
  }
  if (v.isBoolean()) {
    return BooleanToStringBuffer(v.toBoolean(), sb);
  }
  if (v.isNull()) {
    return sb.append(cx->names().null);
  }
  if (v.isSymbol()) {
    JS_ReportErrorNumberASCII(cx, GetErrorMessage, nullptr,
                              JSMSG_SYMBOL_TO_STRING);
    return false;
  }
#ifdef ENABLE_BIGINT
  if (v.isBigInt()) {
    JSLinearString* str = BigInt::toString(cx, v.toBigInt(), 10);
    if (!str) {
      return false;
||||||| merged common ancestors
    if (v.isString()) {
        return sb.append(v.toString());
    }
    if (v.isNumber()) {
        return NumberValueToStringBuffer(cx, v, sb);
    }
    if (v.isBoolean()) {
        return BooleanToStringBuffer(v.toBoolean(), sb);
    }
    if (v.isNull()) {
        return sb.append(cx->names().null);
    }
    if (v.isSymbol()) {
        JS_ReportErrorNumberASCII(cx, GetErrorMessage, nullptr, JSMSG_SYMBOL_TO_STRING);
        return false;
    }
#ifdef ENABLE_BIGINT
    if (v.isBigInt()) {
        JSLinearString* str = BigInt::toString(cx, v.toBigInt(), 10);
        if (!str) {
            return false;
        }
        return sb.append(str);
=======
bool js::ValueToStringBufferSlow(JSContext* cx, const Value& arg,
                                 StringBuffer& sb) {
  RootedValue v(cx, arg);
  if (!ToPrimitive(cx, JSTYPE_STRING, &v)) {
    return false;
  }

  if (v.isString()) {
    return sb.append(v.toString());
  }
  if (v.isNumber()) {
    return NumberValueToStringBuffer(cx, v, sb);
  }
  if (v.isBoolean()) {
    return BooleanToStringBuffer(v.toBoolean(), sb);
  }
  if (v.isNull()) {
    return sb.append(cx->names().null);
  }
  if (v.isSymbol()) {
    JS_ReportErrorNumberASCII(cx, GetErrorMessage, nullptr,
                              JSMSG_SYMBOL_TO_STRING);
    return false;
  }
  if (v.isBigInt()) {
    RootedBigInt i(cx, v.toBigInt());
    JSLinearString* str = BigInt::toString<CanGC>(cx, i, 10);
    if (!str) {
      return false;
>>>>>>> upstream-releases
    }
<<<<<<< HEAD
    return sb.append(str);
  }
#endif
  MOZ_ASSERT(v.isUndefined());
  return sb.append(cx->names().undefined);
||||||| merged common ancestors
#endif
    MOZ_ASSERT(v.isUndefined());
    return sb.append(cx->names().undefined);
=======
    return sb.append(str);
  }
  MOZ_ASSERT(v.isUndefined());
  return sb.append(cx->names().undefined);
>>>>>>> upstream-releases
}
