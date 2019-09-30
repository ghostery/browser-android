/* -*- Mode: C++; tab-width: 8; indent-tabs-mode: nil; c-basic-offset: 2 -*- */
/* vim: set ts=8 sts=2 et sw=2 tw=80: */
/* This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/. */

#ifndef mozilla_dom_TemporaryIPCBlobParent_h
#define mozilla_dom_TemporaryIPCBlobParent_h

#include "mozilla/dom/PTemporaryIPCBlob.h"
#include "mozilla/dom/PTemporaryIPCBlobParent.h"

class nsIFile;

namespace mozilla {
namespace dom {

<<<<<<< HEAD
class TemporaryIPCBlobParent final
    : public mozilla::ipc::PTemporaryIPCBlobParent {
 public:
||||||| merged common ancestors
class TemporaryIPCBlobParent final : public mozilla::ipc::PTemporaryIPCBlobParent
{
public:
=======
class TemporaryIPCBlobParent final : public PTemporaryIPCBlobParent {
  friend class PTemporaryIPCBlobParent;

 public:
>>>>>>> upstream-releases
  explicit TemporaryIPCBlobParent();

  mozilla::ipc::IPCResult CreateAndShareFile();

 private:
  ~TemporaryIPCBlobParent();

<<<<<<< HEAD
  mozilla::ipc::IPCResult RecvOperationFailed() override;
||||||| merged common ancestors
  mozilla::ipc::IPCResult
  RecvOperationFailed() override;
=======
  mozilla::ipc::IPCResult RecvOperationFailed();
>>>>>>> upstream-releases

<<<<<<< HEAD
  mozilla::ipc::IPCResult RecvOperationDone(const nsCString& aContentType,
                                            const FileDescriptor& aFD) override;
||||||| merged common ancestors
  mozilla::ipc::IPCResult
  RecvOperationDone(const nsCString& aContentType,
                    const FileDescriptor& aFD) override;
=======
  mozilla::ipc::IPCResult RecvOperationDone(const nsCString& aContentType,
                                            const FileDescriptor& aFD);
>>>>>>> upstream-releases

  void ActorDestroy(ActorDestroyReason aWhy) override;

  mozilla::ipc::IPCResult SendDeleteError(nsresult aRv);

  nsCOMPtr<nsIFile> mFile;
  bool mActive;
};

}  // namespace dom
}  // namespace mozilla

<<<<<<< HEAD
#endif  // mozilla_dom_ipc_TemporaryIPCBlobParent_h
||||||| merged common ancestors
#endif // mozilla_dom_ipc_TemporaryIPCBlobParent_h
=======
#endif  // mozilla_dom_TemporaryIPCBlobParent_h
>>>>>>> upstream-releases
