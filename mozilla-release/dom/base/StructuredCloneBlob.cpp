/* -*- Mode: C++; tab-width: 8; indent-tabs-mode: nil; c-basic-offset: 2 -*- */
/* vim: set ts=8 sts=2 et sw=2 tw=80: */
/* This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/. */

#include "mozilla/dom/StructuredCloneBlob.h"

#include "js/StructuredClone.h"
#include "js/Utility.h"
#include "js/Wrapper.h"
#include "mozilla/dom/BlobImpl.h"
#include "mozilla/dom/StructuredCloneTags.h"
#include "mozilla/Maybe.h"
#include "mozilla/UniquePtr.h"
#include "xpcpublic.h"

namespace mozilla {
namespace dom {

<<<<<<< HEAD
StructuredCloneBlob::StructuredCloneBlob()
    : StructuredCloneHolder(CloningSupported, TransferringNotSupported,
                            StructuredCloneScope::DifferentProcess) {}
||||||| merged common ancestors
StructuredCloneBlob::StructuredCloneBlob()
    : StructuredCloneHolder(CloningSupported, TransferringNotSupported,
                            StructuredCloneScope::DifferentProcess)
{}
=======
StructuredCloneBlob::StructuredCloneBlob() {
  mHolder.emplace(Holder::CloningSupported, Holder::TransferringNotSupported,
                  Holder::StructuredCloneScope::DifferentProcess);
}
>>>>>>> upstream-releases

StructuredCloneBlob::~StructuredCloneBlob() {
  UnregisterWeakMemoryReporter(this);
}

<<<<<<< HEAD
/* static */ already_AddRefed<StructuredCloneBlob>
StructuredCloneBlob::Constructor(GlobalObject& aGlobal, JS::HandleValue aValue,
                                 JS::HandleObject aTargetGlobal,
                                 ErrorResult& aRv) {
||||||| merged common ancestors

/* static */ already_AddRefed<StructuredCloneBlob>
StructuredCloneBlob::Constructor(GlobalObject& aGlobal, JS::HandleValue aValue,
                                 JS::HandleObject aTargetGlobal,
                                 ErrorResult& aRv)
{
=======
/* static */
already_AddRefed<StructuredCloneBlob> StructuredCloneBlob::Constructor(
    GlobalObject& aGlobal, JS::HandleValue aValue,
    JS::HandleObject aTargetGlobal, ErrorResult& aRv) {
>>>>>>> upstream-releases
  JSContext* cx = aGlobal.Context();

  RefPtr<StructuredCloneBlob> holder = StructuredCloneBlob::Create();

  Maybe<JSAutoRealm> ar;
  JS::RootedValue value(cx, aValue);

  if (aTargetGlobal) {
    // OK to unwrap if our caller (represented by cx's Realm) can do it.
    JS::RootedObject targetGlobal(cx,
                                  js::CheckedUnwrapDynamic(aTargetGlobal, cx));
    if (!targetGlobal) {
      js::ReportAccessDenied(cx);
      aRv.NoteJSContextException(cx);
      return nullptr;
    }

    ar.emplace(cx, targetGlobal);

    if (!JS_WrapValue(cx, &value)) {
      aRv.NoteJSContextException(cx);
      return nullptr;
    }
  } else if (value.isObject()) {
    // OK to unwrap if our caller (represented by cx's Realm) can do it.
    JS::RootedObject obj(cx, js::CheckedUnwrapDynamic(&value.toObject(), cx));
    if (!obj) {
      js::ReportAccessDenied(cx);
      aRv.NoteJSContextException(cx);
      return nullptr;
    }

    ar.emplace(cx, obj);
    value = JS::ObjectValue(*obj);
  }

  holder->mHolder->Write(cx, value, aRv);
  if (aRv.Failed()) {
    return nullptr;
  }

  return holder.forget();
}

<<<<<<< HEAD
void StructuredCloneBlob::Deserialize(JSContext* aCx,
                                      JS::HandleObject aTargetScope,
                                      JS::MutableHandleValue aResult,
                                      ErrorResult& aRv) {
  JS::RootedObject scope(aCx, js::CheckedUnwrap(aTargetScope));
||||||| merged common ancestors
void
StructuredCloneBlob::Deserialize(JSContext* aCx, JS::HandleObject aTargetScope,
                                      JS::MutableHandleValue aResult, ErrorResult& aRv)
{
  JS::RootedObject scope(aCx, js::CheckedUnwrap(aTargetScope));
=======
void StructuredCloneBlob::Deserialize(JSContext* aCx,
                                      JS::HandleObject aTargetScope,
                                      bool aKeepData,
                                      JS::MutableHandleValue aResult,
                                      ErrorResult& aRv) {
  // OK to unwrap if our caller (represented by aCx's Realm) can do it.
  JS::RootedObject scope(aCx, js::CheckedUnwrapDynamic(aTargetScope, aCx));
>>>>>>> upstream-releases
  if (!scope) {
    js::ReportAccessDenied(aCx);
    aRv.NoteJSContextException(aCx);
    return;
  }

  if (!mHolder.isSome()) {
    aRv.Throw(NS_ERROR_NOT_INITIALIZED);
    return;
  }

  {
    JSAutoRealm ar(aCx, scope);

    mHolder->Read(xpc::NativeGlobal(scope), aCx, aResult, aRv);
    if (aRv.Failed()) {
      return;
    }
  }

  if (!aKeepData) {
    mHolder.reset();
  }

  if (!JS_WrapValue(aCx, aResult)) {
    aResult.set(JS::UndefinedValue());
    aRv.NoteJSContextException(aCx);
  }
}

<<<<<<< HEAD
/* static */ JSObject* StructuredCloneBlob::ReadStructuredClone(
    JSContext* aCx, JSStructuredCloneReader* aReader,
    StructuredCloneHolder* aHolder) {
||||||| merged common ancestors

/* static */ JSObject*
StructuredCloneBlob::ReadStructuredClone(JSContext* aCx, JSStructuredCloneReader* aReader,
                                         StructuredCloneHolder* aHolder)
{
=======
/* static */
JSObject* StructuredCloneBlob::ReadStructuredClone(
    JSContext* aCx, JSStructuredCloneReader* aReader,
    StructuredCloneHolder* aHolder) {
>>>>>>> upstream-releases
  JS::RootedObject obj(aCx);
  {
    RefPtr<StructuredCloneBlob> holder = StructuredCloneBlob::Create();

    if (!holder->mHolder->ReadStructuredCloneInternal(aCx, aReader, aHolder) ||
        !holder->WrapObject(aCx, nullptr, &obj)) {
      return nullptr;
    }
  }
  return obj.get();
}

<<<<<<< HEAD
bool StructuredCloneBlob::ReadStructuredCloneInternal(
    JSContext* aCx, JSStructuredCloneReader* aReader,
    StructuredCloneHolder* aHolder) {
||||||| merged common ancestors
bool
StructuredCloneBlob::ReadStructuredCloneInternal(JSContext* aCx, JSStructuredCloneReader* aReader,
                                                 StructuredCloneHolder* aHolder)
{
=======
bool StructuredCloneBlob::Holder::ReadStructuredCloneInternal(
    JSContext* aCx, JSStructuredCloneReader* aReader,
    StructuredCloneHolder* aHolder) {
>>>>>>> upstream-releases
  uint32_t length;
  uint32_t version;
  if (!JS_ReadUint32Pair(aReader, &length, &version)) {
    return false;
  }

  uint32_t blobOffset;
  uint32_t blobCount;
  if (!JS_ReadUint32Pair(aReader, &blobOffset, &blobCount)) {
    return false;
  }
  if (blobCount) {
#ifdef FUZZING
    if (blobOffset >= aHolder->BlobImpls().Length()) {
      return false;
    }
#endif
    BlobImpls().AppendElements(&aHolder->BlobImpls()[blobOffset], blobCount);
  }

  JSStructuredCloneData data(mStructuredCloneScope);
  while (length) {
    size_t size;
    char* buffer = data.AllocateBytes(length, &size);
    if (!buffer || !JS_ReadBytes(aReader, buffer, size)) {
      return false;
    }
    length -= size;
  }

  mBuffer = MakeUnique<JSAutoStructuredCloneBuffer>(
      mStructuredCloneScope, &StructuredCloneHolder::sCallbacks, this);
  mBuffer->adopt(std::move(data), version, &StructuredCloneHolder::sCallbacks);

  return true;
}

<<<<<<< HEAD
bool StructuredCloneBlob::WriteStructuredClone(JSContext* aCx,
                                               JSStructuredCloneWriter* aWriter,
                                               StructuredCloneHolder* aHolder) {
||||||| merged common ancestors
bool
StructuredCloneBlob::WriteStructuredClone(JSContext* aCx, JSStructuredCloneWriter* aWriter,
                                          StructuredCloneHolder* aHolder)
{
=======
bool StructuredCloneBlob::WriteStructuredClone(JSContext* aCx,
                                               JSStructuredCloneWriter* aWriter,
                                               StructuredCloneHolder* aHolder) {
  if (mHolder.isNothing()) {
    return false;
  }
  return mHolder->WriteStructuredClone(aCx, aWriter, aHolder);
}

bool StructuredCloneBlob::Holder::WriteStructuredClone(
    JSContext* aCx, JSStructuredCloneWriter* aWriter,
    StructuredCloneHolder* aHolder) {
>>>>>>> upstream-releases
  auto& data = mBuffer->data();
  if (!JS_WriteUint32Pair(aWriter, SCTAG_DOM_STRUCTURED_CLONE_HOLDER, 0) ||
      !JS_WriteUint32Pair(aWriter, data.Size(), JS_STRUCTURED_CLONE_VERSION) ||
      !JS_WriteUint32Pair(aWriter, aHolder->BlobImpls().Length(),
                          BlobImpls().Length())) {
    return false;
  }

  aHolder->BlobImpls().AppendElements(BlobImpls());

  return data.ForEachDataChunk([&](const char* aData, size_t aSize) {
    return JS_WriteBytes(aWriter, aData, aSize);
  });
}

bool StructuredCloneBlob::WrapObject(JSContext* aCx,
                                     JS::HandleObject aGivenProto,
                                     JS::MutableHandleObject aResult) {
  return StructuredCloneHolder_Binding::Wrap(aCx, this, aGivenProto, aResult);
}

NS_IMETHODIMP
StructuredCloneBlob::CollectReports(nsIHandleReportCallback* aHandleReport,
<<<<<<< HEAD
                                    nsISupports* aData, bool aAnonymize) {
  MOZ_COLLECT_REPORT("explicit/dom/structured-clone-holder", KIND_HEAP,
                     UNITS_BYTES,
                     MallocSizeOf(this) + SizeOfExcludingThis(MallocSizeOf),
                     "Memory used by StructuredCloneHolder DOM objects.");
||||||| merged common ancestors
                                    nsISupports* aData, bool aAnonymize)
{
  MOZ_COLLECT_REPORT(
    "explicit/dom/structured-clone-holder", KIND_HEAP, UNITS_BYTES,
    MallocSizeOf(this) + SizeOfExcludingThis(MallocSizeOf),
    "Memory used by StructuredCloneHolder DOM objects.");
=======
                                    nsISupports* aData, bool aAnonymize) {
  size_t size = MallocSizeOf(this);
  if (mHolder.isSome()) {
    size += mHolder->SizeOfExcludingThis(MallocSizeOf);
  }

  MOZ_COLLECT_REPORT("explicit/dom/structured-clone-holder", KIND_HEAP,
                     UNITS_BYTES, size,
                     "Memory used by StructuredCloneHolder DOM objects.");
>>>>>>> upstream-releases

  return NS_OK;
}

NS_IMPL_ISUPPORTS(StructuredCloneBlob, nsIMemoryReporter)

}  // namespace dom
}  // namespace mozilla
