/* -*- Mode: C++; tab-width: 8; indent-tabs-mode: nil; c-basic-offset: 2 -*- */
/* vim: set ts=8 sts=2 et sw=2 tw=80: */
/* This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/. */

#include "StructuredCloneTester.h"

#include "js/StructuredClone.h"
#include "mozilla/RefPtr.h"
#include "mozilla/dom/StructuredCloneTags.h"
#include "mozilla/dom/StructuredCloneTesterBinding.h"
#include "xpcpublic.h"

namespace mozilla {

class ErrorResult;

namespace dom {

NS_IMPL_CYCLE_COLLECTION_WRAPPERCACHE_0(StructuredCloneTester)
NS_IMPL_CYCLE_COLLECTING_ADDREF(StructuredCloneTester)
NS_IMPL_CYCLE_COLLECTING_RELEASE(StructuredCloneTester)
NS_INTERFACE_MAP_BEGIN_CYCLE_COLLECTION(StructuredCloneTester)
  NS_WRAPPERCACHE_INTERFACE_MAP_ENTRY
  NS_INTERFACE_MAP_ENTRY(nsISupports)
NS_INTERFACE_MAP_END

StructuredCloneTester::StructuredCloneTester(nsISupports* aParent,
                                             const bool aSerializable,
                                             const bool aDeserializable)
<<<<<<< HEAD
    : mParent(aParent),
      mSerializable(aSerializable),
      mDeserializable(aDeserializable) {}

/* static */ already_AddRefed<StructuredCloneTester>
StructuredCloneTester::Constructor(const GlobalObject& aGlobal,
                                   const bool aSerializable,
                                   const bool aDeserializable,
                                   ErrorResult& aRv) {
  RefPtr<StructuredCloneTester> sct = new StructuredCloneTester(
      aGlobal.GetAsSupports(), aSerializable, aDeserializable);
||||||| merged common ancestors
  : mParent(aParent),
    mSerializable(aSerializable),
    mDeserializable(aDeserializable)
{
}

/* static */ already_AddRefed<StructuredCloneTester>
StructuredCloneTester::Constructor(const GlobalObject& aGlobal,
                                   const bool aSerializable,
                                   const bool aDeserializable,
                                   ErrorResult& aRv)
{
  RefPtr<StructuredCloneTester> sct = new StructuredCloneTester(aGlobal.GetAsSupports(),
                                                                aSerializable,
                                                                aDeserializable);
=======
    : mParent(aParent),
      mSerializable(aSerializable),
      mDeserializable(aDeserializable) {}

/* static */
already_AddRefed<StructuredCloneTester> StructuredCloneTester::Constructor(
    const GlobalObject& aGlobal, const bool aSerializable,
    const bool aDeserializable, ErrorResult& aRv) {
  RefPtr<StructuredCloneTester> sct = new StructuredCloneTester(
      aGlobal.GetAsSupports(), aSerializable, aDeserializable);
>>>>>>> upstream-releases
  return sct.forget();
}

bool StructuredCloneTester::Serializable() const { return mSerializable; }

bool StructuredCloneTester::Deserializable() const { return mDeserializable; }

<<<<<<< HEAD
/* static */ JSObject* StructuredCloneTester::ReadStructuredClone(
    JSContext* aCx, JSStructuredCloneReader* aReader) {
||||||| merged common ancestors
/* static */ JSObject*
StructuredCloneTester::ReadStructuredClone(JSContext* aCx,
                                           JSStructuredCloneReader* aReader)
{
=======
/* static */
already_AddRefed<StructuredCloneTester>
StructuredCloneTester::ReadStructuredClone(JSContext* aCx,
                                           nsIGlobalObject* aGlobal,
                                           JSStructuredCloneReader* aReader) {
>>>>>>> upstream-releases
  uint32_t serializable = 0;
  uint32_t deserializable = 0;

  if (!JS_ReadUint32Pair(aReader, &serializable, &deserializable)) {
    return nullptr;
  }

  RefPtr<StructuredCloneTester> sct =
      new StructuredCloneTester(aGlobal, static_cast<bool>(serializable),
                                static_cast<bool>(deserializable));

  // "Fail" deserialization
  if (!sct->Deserializable()) {
    xpc::Throw(aCx, NS_ERROR_DOM_DATA_CLONE_ERR);
    return nullptr;
  }

<<<<<<< HEAD
  // Prevent the return value from being trashed by a GC during ~RefPtr
  JS::Rooted<JSObject*> result(aCx);
  {
    RefPtr<StructuredCloneTester> sct =
        new StructuredCloneTester(global, static_cast<bool>(serializable),
                                  static_cast<bool>(deserializable));

    // "Fail" deserialization
    if (!sct->Deserializable()) {
      xpc::Throw(aCx, NS_ERROR_DOM_DATA_CLONE_ERR);
      return nullptr;
    }

    result = sct->WrapObject(aCx, nullptr);
  }

  return result;
||||||| merged common ancestors
  // Prevent the return value from being trashed by a GC during ~RefPtr
  JS::Rooted<JSObject*> result(aCx);
  {
    RefPtr<StructuredCloneTester> sct = new StructuredCloneTester(
      global,
      static_cast<bool>(serializable),
      static_cast<bool>(deserializable)
    );

    // "Fail" deserialization
    if (!sct->Deserializable()) {
      xpc::Throw(aCx, NS_ERROR_DOM_DATA_CLONE_ERR);
      return nullptr;
    }

    result = sct->WrapObject(aCx, nullptr);
  }

  return result;
=======
  return sct.forget();
>>>>>>> upstream-releases
}

<<<<<<< HEAD
bool StructuredCloneTester::WriteStructuredClone(
    JSStructuredCloneWriter* aWriter) const {
  return JS_WriteUint32Pair(aWriter, SCTAG_DOM_STRUCTURED_CLONE_TESTER, 0) &&
         JS_WriteUint32Pair(aWriter, static_cast<uint32_t>(Serializable()),
||||||| merged common ancestors
bool
StructuredCloneTester::WriteStructuredClone(JSStructuredCloneWriter* aWriter) const
{
  return JS_WriteUint32Pair(aWriter, SCTAG_DOM_STRUCTURED_CLONE_TESTER, 0) &&
         JS_WriteUint32Pair(aWriter, static_cast<uint32_t>(Serializable()),
=======
bool StructuredCloneTester::WriteStructuredClone(
    JSContext* aCx, JSStructuredCloneWriter* aWriter) const {
  if (!Serializable()) {
    return xpc::Throw(aCx, NS_ERROR_DOM_DATA_CLONE_ERR);
  }
  return JS_WriteUint32Pair(aWriter, static_cast<uint32_t>(Serializable()),
>>>>>>> upstream-releases
                            static_cast<uint32_t>(Deserializable()));
}

nsISupports* StructuredCloneTester::GetParentObject() const { return mParent; }

JSObject* StructuredCloneTester::WrapObject(JSContext* aCx,
                                            JS::Handle<JSObject*> aGivenProto) {
  return StructuredCloneTester_Binding::Wrap(aCx, this, aGivenProto);
}

}  // namespace dom
}  // namespace mozilla
