/* -*- Mode: C++; tab-width: 8; indent-tabs-mode: nil; c-basic-offset: 2 -*- */
/* vim: set ts=8 sts=2 et sw=2 tw=80: */
/* This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/. */

#include "mozilla/dom/PushUtil.h"

namespace mozilla {
namespace dom {

<<<<<<< HEAD
/* static */ bool PushUtil::CopyArrayBufferToArray(const ArrayBuffer& aBuffer,
                                                   nsTArray<uint8_t>& aArray) {
||||||| merged common ancestors
/* static */ bool
PushUtil::CopyArrayBufferToArray(const ArrayBuffer& aBuffer,
                                 nsTArray<uint8_t>& aArray)
{
=======
/* static */
bool PushUtil::CopyArrayBufferToArray(const ArrayBuffer& aBuffer,
                                      nsTArray<uint8_t>& aArray) {
>>>>>>> upstream-releases
  MOZ_ASSERT(aArray.IsEmpty());
  aBuffer.ComputeLengthAndData();
  return aArray.SetCapacity(aBuffer.Length(), fallible) &&
         aArray.InsertElementsAt(0, aBuffer.Data(), aBuffer.Length(), fallible);
}

<<<<<<< HEAD
/* static */ bool PushUtil::CopyArrayBufferViewToArray(
    const ArrayBufferView& aView, nsTArray<uint8_t>& aArray) {
||||||| merged common ancestors
/* static */ bool
PushUtil::CopyArrayBufferViewToArray(const ArrayBufferView& aView,
                                     nsTArray<uint8_t>& aArray)
{
=======
/* static */
bool PushUtil::CopyArrayBufferViewToArray(const ArrayBufferView& aView,
                                          nsTArray<uint8_t>& aArray) {
>>>>>>> upstream-releases
  MOZ_ASSERT(aArray.IsEmpty());
  aView.ComputeLengthAndData();
  return aArray.SetCapacity(aView.Length(), fallible) &&
         aArray.InsertElementsAt(0, aView.Data(), aView.Length(), fallible);
}

<<<<<<< HEAD
/* static */ bool PushUtil::CopyBufferSourceToArray(
    const OwningArrayBufferViewOrArrayBuffer& aSource,
    nsTArray<uint8_t>& aArray) {
||||||| merged common ancestors
/* static */ bool
PushUtil::CopyBufferSourceToArray(
  const OwningArrayBufferViewOrArrayBuffer& aSource, nsTArray<uint8_t>& aArray)
{
=======
/* static */
bool PushUtil::CopyBufferSourceToArray(
    const OwningArrayBufferViewOrArrayBuffer& aSource,
    nsTArray<uint8_t>& aArray) {
>>>>>>> upstream-releases
  if (aSource.IsArrayBuffer()) {
    return CopyArrayBufferToArray(aSource.GetAsArrayBuffer(), aArray);
  }
  if (aSource.IsArrayBufferView()) {
    return CopyArrayBufferViewToArray(aSource.GetAsArrayBufferView(), aArray);
  }
  MOZ_CRASH("Uninitialized union: expected buffer or view");
}

<<<<<<< HEAD
/* static */ void PushUtil::CopyArrayToArrayBuffer(
    JSContext* aCx, const nsTArray<uint8_t>& aArray,
    JS::MutableHandle<JSObject*> aValue, ErrorResult& aRv) {
||||||| merged common ancestors
/* static */ void
PushUtil::CopyArrayToArrayBuffer(JSContext* aCx,
                                 const nsTArray<uint8_t>& aArray,
                                 JS::MutableHandle<JSObject*> aValue,
                                 ErrorResult& aRv)
{
=======
/* static */
void PushUtil::CopyArrayToArrayBuffer(JSContext* aCx,
                                      const nsTArray<uint8_t>& aArray,
                                      JS::MutableHandle<JSObject*> aValue,
                                      ErrorResult& aRv) {
>>>>>>> upstream-releases
  if (aArray.IsEmpty()) {
    aValue.set(nullptr);
    return;
  }
  JS::Rooted<JSObject*> buffer(
      aCx, ArrayBuffer::Create(aCx, aArray.Length(), aArray.Elements()));
  if (NS_WARN_IF(!buffer)) {
    aRv.Throw(NS_ERROR_OUT_OF_MEMORY);
    return;
  }
  aValue.set(buffer);
}

}  // namespace dom
}  // namespace mozilla
