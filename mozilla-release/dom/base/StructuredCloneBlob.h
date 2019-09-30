/* -*- Mode: C++; tab-width: 8; indent-tabs-mode: nil; c-basic-offset: 2 -*- */
/* vim: set ts=8 sts=2 et sw=2 tw=80: */
/* This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/. */

#ifndef mozilla_dom_StructuredCloneBlob_h
#define mozilla_dom_StructuredCloneBlob_h

#include "mozilla/dom/BindingDeclarations.h"
#include "mozilla/dom/StructuredCloneHolder.h"
#include "mozilla/dom/StructuredCloneHolderBinding.h"

#include "jsapi.h"

#include "mozilla/Maybe.h"
#include "nsIMemoryReporter.h"
#include "nsISupports.h"

namespace mozilla {
namespace dom {

<<<<<<< HEAD
class StructuredCloneBlob final : public nsIMemoryReporter,
                                  public StructuredCloneHolder {
||||||| merged common ancestors
class StructuredCloneBlob final : public nsIMemoryReporter
                                , public StructuredCloneHolder
{
=======
class StructuredCloneBlob final : public nsIMemoryReporter {
>>>>>>> upstream-releases
  MOZ_DEFINE_MALLOC_SIZE_OF(MallocSizeOf)

 public:
  NS_DECL_ISUPPORTS
  NS_DECL_NSIMEMORYREPORTER

  static JSObject* ReadStructuredClone(JSContext* aCx,
                                       JSStructuredCloneReader* aReader,
                                       StructuredCloneHolder* aHolder);
  bool WriteStructuredClone(JSContext* aCx, JSStructuredCloneWriter* aWriter,
                            StructuredCloneHolder* aHolder);

  static already_AddRefed<StructuredCloneBlob> Constructor(
      GlobalObject& aGlobal, JS::HandleValue aValue,
      JS::HandleObject aTargetGlobal, ErrorResult& aRv);

  void Deserialize(JSContext* aCx, JS::HandleObject aTargetScope,
                   bool aKeepData, JS::MutableHandleValue aResult,
                   ErrorResult& aRv);

  nsISupports* GetParentObject() const { return nullptr; }
  JSObject* GetWrapper() const { return nullptr; }

  bool WrapObject(JSContext* aCx, JS::Handle<JSObject*> aGivenProto,
                  JS::MutableHandleObject aResult);

 protected:
  virtual ~StructuredCloneBlob();

 private:
  explicit StructuredCloneBlob();

<<<<<<< HEAD
  static already_AddRefed<StructuredCloneBlob> Create() {
||||||| merged common ancestors
  static already_AddRefed<StructuredCloneBlob> Create()
  {
=======
  class Holder : public StructuredCloneHolder {
   public:
    using StructuredCloneHolder::StructuredCloneHolder;

    bool ReadStructuredCloneInternal(JSContext* aCx,
                                     JSStructuredCloneReader* aReader,
                                     StructuredCloneHolder* aHolder);

    bool WriteStructuredClone(JSContext* aCx, JSStructuredCloneWriter* aWriter,
                              StructuredCloneHolder* aHolder);
  };

  Maybe<Holder> mHolder;

  static already_AddRefed<StructuredCloneBlob> Create() {
>>>>>>> upstream-releases
    RefPtr<StructuredCloneBlob> holder = new StructuredCloneBlob();
    RegisterWeakMemoryReporter(holder);
    return holder.forget();
  }
<<<<<<< HEAD

  bool ReadStructuredCloneInternal(JSContext* aCx,
                                   JSStructuredCloneReader* aReader,
                                   StructuredCloneHolder* aHolder);
||||||| merged common ancestors

  bool ReadStructuredCloneInternal(JSContext* aCx, JSStructuredCloneReader* aReader,
                                   StructuredCloneHolder* aHolder);
=======
>>>>>>> upstream-releases
};

}  // namespace dom
}  // namespace mozilla

#endif  // mozilla_dom_StructuredCloneBlob_h
