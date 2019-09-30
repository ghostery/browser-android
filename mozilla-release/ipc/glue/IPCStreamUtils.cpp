/* -*- Mode: C++; tab-width: 8; indent-tabs-mode: nil; c-basic-offset: 2 -*- */
/* vim: set ts=8 sts=2 et sw=2 tw=80: */
/* This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/. */

#include "IPCStreamUtils.h"

#include "nsIIPCSerializableInputStream.h"

#include "mozilla/Assertions.h"
#include "mozilla/dom/ContentChild.h"
#include "mozilla/dom/ContentParent.h"
#include "mozilla/dom/File.h"
#include "mozilla/ipc/FileDescriptorSetChild.h"
#include "mozilla/ipc/FileDescriptorSetParent.h"
#include "mozilla/ipc/InputStreamUtils.h"
#include "mozilla/ipc/PBackgroundChild.h"
#include "mozilla/ipc/PBackgroundParent.h"
#include "mozilla/Unused.h"
#include "nsNetCID.h"

using namespace mozilla::dom;

namespace mozilla {
namespace ipc {

namespace {

<<<<<<< HEAD
void AssertValidValueToTake(const IPCStream& aVal) {
  MOZ_ASSERT(aVal.type() == IPCStream::TIPCRemoteStream ||
             aVal.type() == IPCStream::TInputStreamParamsWithFds);
}

void AssertValidValueToTake(const OptionalIPCStream& aVal) {
  MOZ_ASSERT(aVal.type() == OptionalIPCStream::Tvoid_t ||
             aVal.type() == OptionalIPCStream::TIPCStream);
  if (aVal.type() == OptionalIPCStream::TIPCStream) {
    AssertValidValueToTake(aVal.get_IPCStream());
  }
}

||||||| merged common ancestors
void
AssertValidValueToTake(const IPCStream& aVal)
{
  MOZ_ASSERT(aVal.type() == IPCStream::TIPCRemoteStream ||
             aVal.type() == IPCStream::TInputStreamParamsWithFds);
}

void
AssertValidValueToTake(const OptionalIPCStream& aVal)
{
  MOZ_ASSERT(aVal.type() == OptionalIPCStream::Tvoid_t ||
             aVal.type() == OptionalIPCStream::TIPCStream);
  if (aVal.type() == OptionalIPCStream::TIPCStream) {
    AssertValidValueToTake(aVal.get_IPCStream());
  }
}

=======
>>>>>>> upstream-releases
// These serialization and cleanup functions could be externally exposed.  For
// now, though, keep them private to encourage use of the safer RAII
// AutoIPCStream class.

<<<<<<< HEAD
template <typename M>
bool SerializeInputStreamWithFdsChild(nsIIPCSerializableInputStream* aStream,
                                      IPCStream& aValue, M* aManager) {
||||||| merged common ancestors
template<typename M>
bool
SerializeInputStreamWithFdsChild(nsIIPCSerializableInputStream* aStream,
                                 IPCStream& aValue,
                                 M* aManager)
{
=======
template <typename M>
bool SerializeInputStreamWithFdsChild(nsIIPCSerializableInputStream* aStream,
                                      IPCStream& aValue, bool aDelayedStart,
                                      M* aManager) {
>>>>>>> upstream-releases
  MOZ_RELEASE_ASSERT(aStream);
  MOZ_ASSERT(aManager);

<<<<<<< HEAD
  aValue = InputStreamParamsWithFds();
  InputStreamParamsWithFds& streamWithFds =
      aValue.get_InputStreamParamsWithFds();
||||||| merged common ancestors
  aValue = InputStreamParamsWithFds();
  InputStreamParamsWithFds& streamWithFds =
    aValue.get_InputStreamParamsWithFds();
=======
  const uint64_t kTooLargeStream = 1024 * 1024;
>>>>>>> upstream-releases

  uint32_t sizeUsed = 0;
  AutoTArray<FileDescriptor, 4> fds;
  aStream->Serialize(aValue.stream(), fds, aDelayedStart, kTooLargeStream,
                     &sizeUsed, aManager);

  MOZ_ASSERT(sizeUsed <= kTooLargeStream);

  if (aValue.stream().type() == InputStreamParams::T__None) {
    MOZ_CRASH("Serialize failed!");
  }

  if (fds.IsEmpty()) {
    aValue.optionalFds() = void_t();
  } else {
    PFileDescriptorSetChild* fdSet =
        aManager->SendPFileDescriptorSetConstructor(fds[0]);
    for (uint32_t i = 1; i < fds.Length(); ++i) {
      Unused << fdSet->SendAddFileDescriptor(fds[i]);
    }

    aValue.optionalFds() = fdSet;
  }

  return true;
}

<<<<<<< HEAD
template <typename M>
bool SerializeInputStreamWithFdsParent(nsIIPCSerializableInputStream* aStream,
                                       IPCStream& aValue, M* aManager) {
||||||| merged common ancestors
template<typename M>
bool
SerializeInputStreamWithFdsParent(nsIIPCSerializableInputStream* aStream,
                                  IPCStream& aValue,
                                  M* aManager)
{
=======
template <typename M>
bool SerializeInputStreamWithFdsParent(nsIIPCSerializableInputStream* aStream,
                                       IPCStream& aValue, bool aDelayedStart,
                                       M* aManager) {
>>>>>>> upstream-releases
  MOZ_RELEASE_ASSERT(aStream);
  MOZ_ASSERT(aManager);

<<<<<<< HEAD
  aValue = InputStreamParamsWithFds();
  InputStreamParamsWithFds& streamWithFds =
      aValue.get_InputStreamParamsWithFds();
||||||| merged common ancestors
  aValue = InputStreamParamsWithFds();
  InputStreamParamsWithFds& streamWithFds =
    aValue.get_InputStreamParamsWithFds();
=======
  const uint64_t kTooLargeStream = 1024 * 1024;
>>>>>>> upstream-releases

  uint32_t sizeUsed = 0;
  AutoTArray<FileDescriptor, 4> fds;
  aStream->Serialize(aValue.stream(), fds, aDelayedStart, kTooLargeStream,
                     &sizeUsed, aManager);

  MOZ_ASSERT(sizeUsed <= kTooLargeStream);

  if (aValue.stream().type() == InputStreamParams::T__None) {
    MOZ_CRASH("Serialize failed!");
  }

  aValue.optionalFds() = void_t();
  if (!fds.IsEmpty()) {
    PFileDescriptorSetParent* fdSet =
        aManager->SendPFileDescriptorSetConstructor(fds[0]);
    for (uint32_t i = 1; i < fds.Length(); ++i) {
      if (NS_WARN_IF(!fdSet->SendAddFileDescriptor(fds[i]))) {
        Unused << PFileDescriptorSetParent::Send__delete__(fdSet);
        fdSet = nullptr;
        break;
      }
    }

    if (fdSet) {
      aValue.optionalFds() = fdSet;
    }
  }

  return true;
}

template <typename M>
bool SerializeInputStream(nsIInputStream* aStream, IPCStream& aValue,
                          M* aManager, bool aDelayedStart) {
  MOZ_ASSERT(aStream);
  MOZ_ASSERT(aManager);

  InputStreamParams params;
  InputStreamHelper::SerializeInputStreamAsPipe(aStream, params, aDelayedStart,
                                                aManager);

<<<<<<< HEAD
  // As a fallback, attempt to stream the data across using a IPCStream
  // actor. For blocking streams, create a nonblocking pipe instead,
  nsCOMPtr<nsIAsyncInputStream> asyncStream = do_QueryInterface(aStream);
  if (!asyncStream) {
    const uint32_t kBufferSize = 32768;  // matches IPCStream buffer size.
    nsCOMPtr<nsIAsyncOutputStream> sink;
    nsresult rv = NS_NewPipe2(getter_AddRefs(asyncStream), getter_AddRefs(sink),
                              true, false, kBufferSize, UINT32_MAX);
    if (NS_WARN_IF(NS_FAILED(rv))) {
      return false;
    }

    nsCOMPtr<nsIEventTarget> target =
        do_GetService(NS_STREAMTRANSPORTSERVICE_CONTRACTID);

    rv = NS_AsyncCopy(aStream, sink, target, NS_ASYNCCOPY_VIA_READSEGMENTS,
                      kBufferSize);
    if (NS_WARN_IF(NS_FAILED(rv))) {
      return false;
    }
||||||| merged common ancestors
  // As a fallback, attempt to stream the data across using a IPCStream
  // actor. For blocking streams, create a nonblocking pipe instead,
  nsCOMPtr<nsIAsyncInputStream> asyncStream = do_QueryInterface(aStream);
  if (!asyncStream) {
    const uint32_t kBufferSize = 32768; // matches IPCStream buffer size.
    nsCOMPtr<nsIAsyncOutputStream> sink;
    nsresult rv = NS_NewPipe2(getter_AddRefs(asyncStream),
                              getter_AddRefs(sink),
                              true,
                              false,
                              kBufferSize,
                              UINT32_MAX);
    if (NS_WARN_IF(NS_FAILED(rv))) {
      return false;
    }

    nsCOMPtr<nsIEventTarget> target =
      do_GetService(NS_STREAMTRANSPORTSERVICE_CONTRACTID);

    rv = NS_AsyncCopy(aStream, sink, target, NS_ASYNCCOPY_VIA_READSEGMENTS,
                      kBufferSize);
    if (NS_WARN_IF(NS_FAILED(rv))) {
      return false;
    }
=======
  if (params.type() == InputStreamParams::T__None) {
    return false;
>>>>>>> upstream-releases
  }

  aValue.stream() = params;
  aValue.optionalFds() = void_t();

  return true;
}

<<<<<<< HEAD
template <typename M>
bool SerializeInputStreamChild(nsIInputStream* aStream, M* aManager,
                               IPCStream* aValue,
                               OptionalIPCStream* aOptionalValue,
                               bool aDelayedStart) {
||||||| merged common ancestors
template<typename M>
bool
SerializeInputStreamChild(nsIInputStream* aStream, M* aManager,
                          IPCStream* aValue,
                          OptionalIPCStream* aOptionalValue,
                          bool aDelayedStart)
{
=======
template <typename M>
bool SerializeInputStreamChild(nsIInputStream* aStream, M* aManager,
                               IPCStream* aValue,
                               Maybe<IPCStream>* aOptionalValue,
                               bool aDelayedStart) {
>>>>>>> upstream-releases
  MOZ_ASSERT(aStream);
  MOZ_ASSERT(aManager);
  MOZ_ASSERT(aValue || aOptionalValue);

  nsCOMPtr<nsIIPCSerializableInputStream> serializable =
<<<<<<< HEAD
      do_QueryInterface(aStream);

  // ExpectedSerializedLength() returns the length of the stream if serialized.
  // This is useful to decide if we want to continue using the serialization
  // directly, or if it's better to use IPCStream.
  uint64_t expectedLength =
      serializable ? serializable->ExpectedSerializedLength().valueOr(0) : 0;
  if (serializable && expectedLength < kTooLargeStream) {
||||||| merged common ancestors
    do_QueryInterface(aStream);

  // ExpectedSerializedLength() returns the length of the stream if serialized.
  // This is useful to decide if we want to continue using the serialization
  // directly, or if it's better to use IPCStream.
  uint64_t expectedLength =
    serializable ? serializable->ExpectedSerializedLength().valueOr(0) : 0;
  if (serializable && expectedLength < kTooLargeStream) {
=======
      do_QueryInterface(aStream);

  if (serializable) {
>>>>>>> upstream-releases
    if (aValue) {
      return SerializeInputStreamWithFdsChild(serializable, *aValue,
                                              aDelayedStart, aManager);
    }

    return SerializeInputStreamWithFdsChild(serializable, aOptionalValue->ref(),
                                            aDelayedStart, aManager);
  }

  if (aValue) {
    return SerializeInputStream(aStream, *aValue, aManager, aDelayedStart);
  }

<<<<<<< HEAD
  return SerializeInputStream(aStream, *aOptionalValue, aManager,
                              aDelayedStart);
||||||| merged common ancestors
  return SerializeInputStream(aStream, *aOptionalValue, aManager, aDelayedStart);
=======
  return SerializeInputStream(aStream, aOptionalValue->ref(), aManager,
                              aDelayedStart);
>>>>>>> upstream-releases
}

<<<<<<< HEAD
template <typename M>
bool SerializeInputStreamParent(nsIInputStream* aStream, M* aManager,
                                IPCStream* aValue,
                                OptionalIPCStream* aOptionalValue,
                                bool aDelayedStart) {
||||||| merged common ancestors
template<typename M>
bool
SerializeInputStreamParent(nsIInputStream* aStream, M* aManager,
                           IPCStream* aValue,
                           OptionalIPCStream* aOptionalValue,
                           bool aDelayedStart)
{
=======
template <typename M>
bool SerializeInputStreamParent(nsIInputStream* aStream, M* aManager,
                                IPCStream* aValue,
                                Maybe<IPCStream>* aOptionalValue,
                                bool aDelayedStart) {
>>>>>>> upstream-releases
  MOZ_ASSERT(aStream);
  MOZ_ASSERT(aManager);
  MOZ_ASSERT(aValue || aOptionalValue);

  nsCOMPtr<nsIIPCSerializableInputStream> serializable =
<<<<<<< HEAD
      do_QueryInterface(aStream);
  uint64_t expectedLength =
      serializable ? serializable->ExpectedSerializedLength().valueOr(0) : 0;
||||||| merged common ancestors
    do_QueryInterface(aStream);
  uint64_t expectedLength =
    serializable ? serializable->ExpectedSerializedLength().valueOr(0) : 0;
=======
      do_QueryInterface(aStream);
>>>>>>> upstream-releases

  if (serializable) {
    if (aValue) {
      return SerializeInputStreamWithFdsParent(serializable, *aValue,
                                               aDelayedStart, aManager);
    }

    return SerializeInputStreamWithFdsParent(
        serializable, aOptionalValue->ref(), aDelayedStart, aManager);
  }

  if (aValue) {
    return SerializeInputStream(aStream, *aValue, aManager, aDelayedStart);
  }

<<<<<<< HEAD
  return SerializeInputStream(aStream, *aOptionalValue, aManager,
                              aDelayedStart);
||||||| merged common ancestors
  return SerializeInputStream(aStream, *aOptionalValue, aManager, aDelayedStart);
=======
  return SerializeInputStream(aStream, aOptionalValue->ref(), aManager,
                              aDelayedStart);
>>>>>>> upstream-releases
}

<<<<<<< HEAD
void CleanupIPCStream(IPCStream& aValue, bool aConsumedByIPC,
                      bool aDelayedStart) {
  if (aValue.type() == IPCStream::T__None) {
    return;
  }

  if (aValue.type() == IPCStream::TInputStreamParamsWithFds) {
    InputStreamParamsWithFds& streamWithFds =
        aValue.get_InputStreamParamsWithFds();

    // Cleanup file descriptors if necessary
    if (streamWithFds.optionalFds().type() ==
        OptionalFileDescriptorSet::TPFileDescriptorSetChild) {
      AutoTArray<FileDescriptor, 4> fds;

      auto fdSetActor = static_cast<FileDescriptorSetChild*>(
          streamWithFds.optionalFds().get_PFileDescriptorSetChild());
      MOZ_ASSERT(fdSetActor);

      // FileDescriptorSet doesn't clear its fds in its ActorDestroy, so we
      // unconditionally forget them here.  The fds themselves are auto-closed
      // in ~FileDescriptor since they originated in this process.
      fdSetActor->ForgetFileDescriptors(fds);
||||||| merged common ancestors
void
CleanupIPCStream(IPCStream& aValue, bool aConsumedByIPC, bool aDelayedStart)
{
  if (aValue.type() == IPCStream::T__None) {
    return;
  }

  if (aValue.type() == IPCStream::TInputStreamParamsWithFds) {

    InputStreamParamsWithFds& streamWithFds =
      aValue.get_InputStreamParamsWithFds();

    // Cleanup file descriptors if necessary
    if (streamWithFds.optionalFds().type() ==
        OptionalFileDescriptorSet::TPFileDescriptorSetChild) {

      AutoTArray<FileDescriptor, 4> fds;

      auto fdSetActor = static_cast<FileDescriptorSetChild*>(
        streamWithFds.optionalFds().get_PFileDescriptorSetChild());
      MOZ_ASSERT(fdSetActor);

      // FileDescriptorSet doesn't clear its fds in its ActorDestroy, so we
      // unconditionally forget them here.  The fds themselves are auto-closed in
      // ~FileDescriptor since they originated in this process.
      fdSetActor->ForgetFileDescriptors(fds);
=======
void ActivateAndCleanupIPCStream(IPCStream& aValue, bool aConsumedByIPC,
                                 bool aDelayedStart) {
  // Cleanup file descriptors if necessary
  if (aValue.optionalFds().type() ==
      OptionalFileDescriptorSet::TPFileDescriptorSetChild) {
    AutoTArray<FileDescriptor, 4> fds;
>>>>>>> upstream-releases

<<<<<<< HEAD
      if (!aConsumedByIPC) {
        Unused << FileDescriptorSetChild::Send__delete__(fdSetActor);
      }

    } else if (streamWithFds.optionalFds().type() ==
               OptionalFileDescriptorSet::TPFileDescriptorSetParent) {
      AutoTArray<FileDescriptor, 4> fds;

      auto fdSetActor = static_cast<FileDescriptorSetParent*>(
          streamWithFds.optionalFds().get_PFileDescriptorSetParent());
      MOZ_ASSERT(fdSetActor);
||||||| merged common ancestors
      if (!aConsumedByIPC) {
        Unused << FileDescriptorSetChild::Send__delete__(fdSetActor);
      }

    } else if (streamWithFds.optionalFds().type() ==
               OptionalFileDescriptorSet::TPFileDescriptorSetParent) {

      AutoTArray<FileDescriptor, 4> fds;

      auto fdSetActor = static_cast<FileDescriptorSetParent*>(
        streamWithFds.optionalFds().get_PFileDescriptorSetParent());
      MOZ_ASSERT(fdSetActor);
=======
    auto fdSetActor = static_cast<FileDescriptorSetChild*>(
        aValue.optionalFds().get_PFileDescriptorSetChild());
    MOZ_ASSERT(fdSetActor);
>>>>>>> upstream-releases

<<<<<<< HEAD
      // FileDescriptorSet doesn't clear its fds in its ActorDestroy, so we
      // unconditionally forget them here.  The fds themselves are auto-closed
      // in ~FileDescriptor since they originated in this process.
      fdSetActor->ForgetFileDescriptors(fds);
||||||| merged common ancestors
      // FileDescriptorSet doesn't clear its fds in its ActorDestroy, so we
      // unconditionally forget them here.  The fds themselves are auto-closed in
      // ~FileDescriptor since they originated in this process.
      fdSetActor->ForgetFileDescriptors(fds);
=======
    // FileDescriptorSet doesn't clear its fds in its ActorDestroy, so we
    // unconditionally forget them here.  The fds themselves are auto-closed
    // in ~FileDescriptor since they originated in this process.
    fdSetActor->ForgetFileDescriptors(fds);
>>>>>>> upstream-releases

    if (!aConsumedByIPC) {
      Unused << FileDescriptorSetChild::Send__delete__(fdSetActor);
    }

<<<<<<< HEAD
    return;
  }

  MOZ_ASSERT(aValue.type() == IPCStream::TIPCRemoteStream);
  IPCRemoteStreamType& remoteInputStream =
      aValue.get_IPCRemoteStream().stream();
||||||| merged common ancestors
    return;
  }

  MOZ_ASSERT(aValue.type() == IPCStream::TIPCRemoteStream);
  IPCRemoteStreamType& remoteInputStream =
    aValue.get_IPCRemoteStream().stream();
=======
  } else if (aValue.optionalFds().type() ==
             OptionalFileDescriptorSet::TPFileDescriptorSetParent) {
    AutoTArray<FileDescriptor, 4> fds;
>>>>>>> upstream-releases

<<<<<<< HEAD
  IPCStreamSource* source = nullptr;
  if (remoteInputStream.type() ==
      IPCRemoteStreamType::TPChildToParentStreamChild) {
    source = IPCStreamSource::Cast(
        remoteInputStream.get_PChildToParentStreamChild());
  } else {
    MOZ_ASSERT(remoteInputStream.type() ==
               IPCRemoteStreamType::TPParentToChildStreamParent);
    source = IPCStreamSource::Cast(
        remoteInputStream.get_PParentToChildStreamParent());
  }
||||||| merged common ancestors
  IPCStreamSource* source = nullptr;
  if (remoteInputStream.type() == IPCRemoteStreamType::TPChildToParentStreamChild) {
    source = IPCStreamSource::Cast(remoteInputStream.get_PChildToParentStreamChild());
  } else {
    MOZ_ASSERT(remoteInputStream.type() == IPCRemoteStreamType::TPParentToChildStreamParent);
    source = IPCStreamSource::Cast(remoteInputStream.get_PParentToChildStreamParent());
  }
=======
    auto fdSetActor = static_cast<FileDescriptorSetParent*>(
        aValue.optionalFds().get_PFileDescriptorSetParent());
    MOZ_ASSERT(fdSetActor);
>>>>>>> upstream-releases

    // FileDescriptorSet doesn't clear its fds in its ActorDestroy, so we
    // unconditionally forget them here.  The fds themselves are auto-closed
    // in ~FileDescriptor since they originated in this process.
    fdSetActor->ForgetFileDescriptors(fds);

    if (!aConsumedByIPC) {
      Unused << FileDescriptorSetParent::Send__delete__(fdSetActor);
    }
  }

  // Activate IPCRemoteStreamParams.
  InputStreamHelper::PostSerializationActivation(aValue.stream(),
                                                 aConsumedByIPC, aDelayedStart);
}

<<<<<<< HEAD
void CleanupIPCStream(OptionalIPCStream& aValue, bool aConsumedByIPC,
                      bool aDelayedStart) {
  if (aValue.type() == OptionalIPCStream::Tvoid_t) {
||||||| merged common ancestors
void
CleanupIPCStream(OptionalIPCStream& aValue, bool aConsumedByIPC, bool aDelayedStart)
{
  if (aValue.type() == OptionalIPCStream::Tvoid_t) {
=======
void ActivateAndCleanupIPCStream(Maybe<IPCStream>& aValue, bool aConsumedByIPC,
                                 bool aDelayedStart) {
  if (aValue.isNothing()) {
>>>>>>> upstream-releases
    return;
  }

  ActivateAndCleanupIPCStream(aValue.ref(), aConsumedByIPC, aDelayedStart);
}

// Returns false if the serialization should not proceed. This means that the
// inputStream is null.
<<<<<<< HEAD
bool NormalizeOptionalValue(nsIInputStream* aStream, IPCStream* aValue,
                            OptionalIPCStream* aOptionalValue) {
||||||| merged common ancestors
bool
NormalizeOptionalValue(nsIInputStream* aStream,
                       IPCStream* aValue,
                       OptionalIPCStream* aOptionalValue)
{
=======
bool NormalizeOptionalValue(nsIInputStream* aStream, IPCStream* aValue,
                            Maybe<IPCStream>* aOptionalValue) {
>>>>>>> upstream-releases
  if (aValue) {
    // if aStream is null, we will crash when serializing.
    return true;
  }

  if (!aStream) {
    aOptionalValue->reset();
    return false;
  }

  aOptionalValue->emplace();
  return true;
}

<<<<<<< HEAD
}  // anonymous namespace

already_AddRefed<nsIInputStream> DeserializeIPCStream(const IPCStream& aValue) {
  if (aValue.type() == IPCStream::TIPCRemoteStream) {
    const IPCRemoteStream& remoteStream = aValue.get_IPCRemoteStream();
    const IPCRemoteStreamType& remoteStreamType = remoteStream.stream();
    IPCStreamDestination* destinationStream;

    if (remoteStreamType.type() ==
        IPCRemoteStreamType::TPChildToParentStreamParent) {
      destinationStream = IPCStreamDestination::Cast(
          remoteStreamType.get_PChildToParentStreamParent());
    } else {
      MOZ_ASSERT(remoteStreamType.type() ==
                 IPCRemoteStreamType::TPParentToChildStreamChild);
      destinationStream = IPCStreamDestination::Cast(
          remoteStreamType.get_PParentToChildStreamChild());
    }

    destinationStream->SetDelayedStart(remoteStream.delayedStart());
    destinationStream->SetLength(remoteStream.length());
    return destinationStream->TakeReader();
  }
||||||| merged common ancestors
} // anonymous namespace

already_AddRefed<nsIInputStream>
DeserializeIPCStream(const IPCStream& aValue)
{
  if (aValue.type() == IPCStream::TIPCRemoteStream) {
    const IPCRemoteStream& remoteStream = aValue.get_IPCRemoteStream();
    const IPCRemoteStreamType& remoteStreamType = remoteStream.stream();
    IPCStreamDestination* destinationStream;

    if (remoteStreamType.type() == IPCRemoteStreamType::TPChildToParentStreamParent) {
      destinationStream =
        IPCStreamDestination::Cast(remoteStreamType.get_PChildToParentStreamParent());
    } else {
      MOZ_ASSERT(remoteStreamType.type() == IPCRemoteStreamType::TPParentToChildStreamChild);
      destinationStream =
        IPCStreamDestination::Cast(remoteStreamType.get_PParentToChildStreamChild());
    }

    destinationStream->SetDelayedStart(remoteStream.delayedStart());
    destinationStream->SetLength(remoteStream.length());
    return destinationStream->TakeReader();
  }
=======
}  // anonymous namespace
>>>>>>> upstream-releases

<<<<<<< HEAD
  // Note, we explicitly do not support deserializing the PChildToParentStream
  // actor on the child side nor the PParentToChildStream actor on the parent
  // side.
  MOZ_ASSERT(aValue.type() == IPCStream::TInputStreamParamsWithFds);

  const InputStreamParamsWithFds& streamWithFds =
      aValue.get_InputStreamParamsWithFds();
||||||| merged common ancestors
  // Note, we explicitly do not support deserializing the PChildToParentStream actor on
  // the child side nor the PParentToChildStream actor on the parent side.
  MOZ_ASSERT(aValue.type() == IPCStream::TInputStreamParamsWithFds);

  const InputStreamParamsWithFds& streamWithFds =
    aValue.get_InputStreamParamsWithFds();
=======
already_AddRefed<nsIInputStream> DeserializeIPCStream(const IPCStream& aValue) {
  // Note, we explicitly do not support deserializing the PChildToParentStream
  // actor on the child side nor the PParentToChildStream actor on the parent
  // side.
>>>>>>> upstream-releases

  AutoTArray<FileDescriptor, 4> fds;
  if (aValue.optionalFds().type() ==
      OptionalFileDescriptorSet::TPFileDescriptorSetParent) {
    auto fdSetActor = static_cast<FileDescriptorSetParent*>(
<<<<<<< HEAD
        streamWithFds.optionalFds().get_PFileDescriptorSetParent());
||||||| merged common ancestors
      streamWithFds.optionalFds().get_PFileDescriptorSetParent());
=======
        aValue.optionalFds().get_PFileDescriptorSetParent());
>>>>>>> upstream-releases
    MOZ_ASSERT(fdSetActor);

    fdSetActor->ForgetFileDescriptors(fds);
    MOZ_ASSERT(!fds.IsEmpty());

    if (!FileDescriptorSetParent::Send__delete__(fdSetActor)) {
      // child process is gone, warn and allow actor to clean up normally
      NS_WARNING("Failed to delete fd set actor.");
    }
  } else if (aValue.optionalFds().type() ==
             OptionalFileDescriptorSet::TPFileDescriptorSetChild) {
    auto fdSetActor = static_cast<FileDescriptorSetChild*>(
<<<<<<< HEAD
        streamWithFds.optionalFds().get_PFileDescriptorSetChild());
||||||| merged common ancestors
      streamWithFds.optionalFds().get_PFileDescriptorSetChild());
=======
        aValue.optionalFds().get_PFileDescriptorSetChild());
>>>>>>> upstream-releases
    MOZ_ASSERT(fdSetActor);

    fdSetActor->ForgetFileDescriptors(fds);
    MOZ_ASSERT(!fds.IsEmpty());

    Unused << FileDescriptorSetChild::Send__delete__(fdSetActor);
  }

  return InputStreamHelper::DeserializeInputStream(aValue.stream(), fds);
}

<<<<<<< HEAD
already_AddRefed<nsIInputStream> DeserializeIPCStream(
    const OptionalIPCStream& aValue) {
  if (aValue.type() == OptionalIPCStream::Tvoid_t) {
||||||| merged common ancestors
already_AddRefed<nsIInputStream>
DeserializeIPCStream(const OptionalIPCStream& aValue)
{
  if (aValue.type() == OptionalIPCStream::Tvoid_t) {
=======
already_AddRefed<nsIInputStream> DeserializeIPCStream(
    const Maybe<IPCStream>& aValue) {
  if (aValue.isNothing()) {
>>>>>>> upstream-releases
    return nullptr;
  }

  return DeserializeIPCStream(aValue.ref());
}

AutoIPCStream::AutoIPCStream(bool aDelayedStart)
<<<<<<< HEAD
    : mInlineValue(void_t()),
      mValue(nullptr),
      mOptionalValue(&mInlineValue),
      mTaken(false),
      mDelayedStart(aDelayedStart) {}
||||||| merged common ancestors
  : mInlineValue(void_t())
  , mValue(nullptr)
  , mOptionalValue(&mInlineValue)
  , mTaken(false)
  , mDelayedStart(aDelayedStart)
{
}
=======
    : mValue(nullptr),
      mOptionalValue(&mInlineValue),
      mTaken(false),
      mDelayedStart(aDelayedStart) {}
>>>>>>> upstream-releases

AutoIPCStream::AutoIPCStream(IPCStream& aTarget, bool aDelayedStart)
<<<<<<< HEAD
    : mInlineValue(void_t()),
      mValue(&aTarget),
      mOptionalValue(nullptr),
      mTaken(false),
      mDelayedStart(aDelayedStart) {}

AutoIPCStream::AutoIPCStream(OptionalIPCStream& aTarget, bool aDelayedStart)
    : mInlineValue(void_t()),
      mValue(nullptr),
      mOptionalValue(&aTarget),
      mTaken(false),
      mDelayedStart(aDelayedStart) {
  *mOptionalValue = void_t();
||||||| merged common ancestors
  : mInlineValue(void_t())
  , mValue(&aTarget)
  , mOptionalValue(nullptr)
  , mTaken(false)
  , mDelayedStart(aDelayedStart)
{
}

AutoIPCStream::AutoIPCStream(OptionalIPCStream& aTarget, bool aDelayedStart)
  : mInlineValue(void_t())
  , mValue(nullptr)
  , mOptionalValue(&aTarget)
  , mTaken(false)
  , mDelayedStart(aDelayedStart)
{
  *mOptionalValue = void_t();
=======
    : mValue(&aTarget),
      mOptionalValue(nullptr),
      mTaken(false),
      mDelayedStart(aDelayedStart) {}

AutoIPCStream::AutoIPCStream(Maybe<IPCStream>& aTarget, bool aDelayedStart)
    : mValue(nullptr),
      mOptionalValue(&aTarget),
      mTaken(false),
      mDelayedStart(aDelayedStart) {
  mOptionalValue->reset();
>>>>>>> upstream-releases
}

AutoIPCStream::~AutoIPCStream() {
  MOZ_ASSERT(mValue || mOptionalValue);
  if (mValue && IsSet()) {
    ActivateAndCleanupIPCStream(*mValue, mTaken, mDelayedStart);
  } else {
    ActivateAndCleanupIPCStream(*mOptionalValue, mTaken, mDelayedStart);
  }
}

<<<<<<< HEAD
bool AutoIPCStream::Serialize(nsIInputStream* aStream,
                              dom::nsIContentChild* aManager) {
||||||| merged common ancestors
bool
AutoIPCStream::Serialize(nsIInputStream* aStream, dom::nsIContentChild* aManager)
{
=======
bool AutoIPCStream::Serialize(nsIInputStream* aStream,
                              dom::ContentChild* aManager) {
>>>>>>> upstream-releases
  MOZ_ASSERT(aStream || !mValue);
  MOZ_ASSERT(aManager);
  MOZ_ASSERT(mValue || mOptionalValue);
  MOZ_ASSERT(!mTaken);
  MOZ_ASSERT(!IsSet());

  // If NormalizeOptionalValue returns false, we don't have to proceed.
  if (!NormalizeOptionalValue(aStream, mValue, mOptionalValue)) {
    return true;
  }

  if (!SerializeInputStreamChild(aStream, aManager, mValue, mOptionalValue,
                                 mDelayedStart)) {
    MOZ_CRASH("IPCStream creation failed!");
  }

  return true;
}

bool AutoIPCStream::Serialize(nsIInputStream* aStream,
                              PBackgroundChild* aManager) {
  MOZ_ASSERT(aStream || !mValue);
  MOZ_ASSERT(aManager);
  MOZ_ASSERT(mValue || mOptionalValue);
  MOZ_ASSERT(!mTaken);
  MOZ_ASSERT(!IsSet());

  // If NormalizeOptionalValue returns false, we don't have to proceed.
  if (!NormalizeOptionalValue(aStream, mValue, mOptionalValue)) {
    return true;
  }

  if (!SerializeInputStreamChild(aStream, aManager, mValue, mOptionalValue,
                                 mDelayedStart)) {
    MOZ_CRASH("IPCStream creation failed!");
  }

  return true;
}

<<<<<<< HEAD
bool AutoIPCStream::Serialize(nsIInputStream* aStream,
                              dom::nsIContentParent* aManager) {
||||||| merged common ancestors
bool
AutoIPCStream::Serialize(nsIInputStream* aStream,
                         dom::nsIContentParent* aManager)
{
=======
bool AutoIPCStream::Serialize(nsIInputStream* aStream,
                              dom::ContentParent* aManager) {
>>>>>>> upstream-releases
  MOZ_ASSERT(aStream || !mValue);
  MOZ_ASSERT(aManager);
  MOZ_ASSERT(mValue || mOptionalValue);
  MOZ_ASSERT(!mTaken);
  MOZ_ASSERT(!IsSet());

  // If NormalizeOptionalValue returns false, we don't have to proceed.
  if (!NormalizeOptionalValue(aStream, mValue, mOptionalValue)) {
    return true;
  }

  if (!SerializeInputStreamParent(aStream, aManager, mValue, mOptionalValue,
                                  mDelayedStart)) {
    return false;
  }

  return true;
}

bool AutoIPCStream::Serialize(nsIInputStream* aStream,
                              PBackgroundParent* aManager) {
  MOZ_ASSERT(aStream || !mValue);
  MOZ_ASSERT(aManager);
  MOZ_ASSERT(mValue || mOptionalValue);
  MOZ_ASSERT(!mTaken);
  MOZ_ASSERT(!IsSet());

  // If NormalizeOptionalValue returns false, we don't have to proceed.
  if (!NormalizeOptionalValue(aStream, mValue, mOptionalValue)) {
    return true;
  }

  if (!SerializeInputStreamParent(aStream, aManager, mValue, mOptionalValue,
                                  mDelayedStart)) {
    return false;
  }

  return true;
}

bool AutoIPCStream::IsSet() const {
  MOZ_ASSERT(mValue || mOptionalValue);
  if (mValue) {
    return mValue->stream().type() != InputStreamParams::T__None;
  } else {
    return mOptionalValue->isSome() &&
           mOptionalValue->ref().stream().type() != InputStreamParams::T__None;
  }
}

IPCStream& AutoIPCStream::TakeValue() {
  MOZ_ASSERT(mValue || mOptionalValue);
  MOZ_ASSERT(!mTaken);
  MOZ_ASSERT(IsSet());

  mTaken = true;

  if (mValue) {
    return *mValue;
  }

<<<<<<< HEAD
  IPCStream& value = mOptionalValue->get_IPCStream();

  AssertValidValueToTake(value);
||||||| merged common ancestors
  IPCStream& value =
    mOptionalValue->get_IPCStream();

  AssertValidValueToTake(value);
=======
  IPCStream& value = mOptionalValue->ref();
>>>>>>> upstream-releases
  return value;
}

<<<<<<< HEAD
OptionalIPCStream& AutoIPCStream::TakeOptionalValue() {
||||||| merged common ancestors
OptionalIPCStream&
AutoIPCStream::TakeOptionalValue()
{
=======
Maybe<IPCStream>& AutoIPCStream::TakeOptionalValue() {
>>>>>>> upstream-releases
  MOZ_ASSERT(!mTaken);
  MOZ_ASSERT(!mValue);
  MOZ_ASSERT(mOptionalValue);
  mTaken = true;
  return *mOptionalValue;
}

<<<<<<< HEAD
void IPDLParamTraits<nsIInputStream>::Write(IPC::Message* aMsg,
                                            IProtocol* aActor,
                                            nsIInputStream* aParam) {
||||||| merged common ancestors
void
IPDLParamTraits<nsIInputStream>::Write(IPC::Message* aMsg,
                                       IProtocol* aActor,
                                       nsIInputStream* aParam)
{
=======
void IPDLParamTraits<nsIInputStream*>::Write(IPC::Message* aMsg,
                                             IProtocol* aActor,
                                             nsIInputStream* aParam) {
>>>>>>> upstream-releases
  mozilla::ipc::AutoIPCStream autoStream;
  bool ok = false;
  bool found = false;

  // We can only serialize our nsIInputStream if it's going to be sent over one
  // of the protocols we support, or a protocol which is managed by one of the
  // protocols we support.
  IProtocol* actor = aActor;
  while (!found && actor) {
    switch (actor->GetProtocolId()) {
      case PContentMsgStart:
        if (actor->GetSide() == mozilla::ipc::ParentSide) {
          ok = autoStream.Serialize(
              aParam, static_cast<mozilla::dom::ContentParent*>(actor));
        } else {
          MOZ_RELEASE_ASSERT(actor->GetSide() == mozilla::ipc::ChildSide);
          ok = autoStream.Serialize(
              aParam, static_cast<mozilla::dom::ContentChild*>(actor));
        }
        found = true;
        break;
      case PBackgroundMsgStart:
        if (actor->GetSide() == mozilla::ipc::ParentSide) {
          ok = autoStream.Serialize(
              aParam, static_cast<mozilla::ipc::PBackgroundParent*>(actor));
        } else {
          MOZ_RELEASE_ASSERT(actor->GetSide() == mozilla::ipc::ChildSide);
          ok = autoStream.Serialize(
              aParam, static_cast<mozilla::ipc::PBackgroundChild*>(actor));
        }
        found = true;
        break;
      default:
        break;
    }

    // Try the actor's manager.
    actor = actor->Manager();
  }

  if (!found) {
    aActor->FatalError(
        "Attempt to send nsIInputStream over an unsupported ipdl protocol");
  }
  MOZ_RELEASE_ASSERT(ok, "Failed to serialize nsIInputStream");

  WriteIPDLParam(aMsg, aActor, autoStream.TakeOptionalValue());
}

<<<<<<< HEAD
bool IPDLParamTraits<nsIInputStream>::Read(const IPC::Message* aMsg,
                                           PickleIterator* aIter,
                                           IProtocol* aActor,
                                           RefPtr<nsIInputStream>* aResult) {
  mozilla::ipc::OptionalIPCStream ipcStream;
||||||| merged common ancestors
bool
IPDLParamTraits<nsIInputStream>::Read(const IPC::Message* aMsg, PickleIterator* aIter,
                                      IProtocol* aActor, RefPtr<nsIInputStream>* aResult)
{
  mozilla::ipc::OptionalIPCStream ipcStream;
=======
bool IPDLParamTraits<nsIInputStream*>::Read(const IPC::Message* aMsg,
                                            PickleIterator* aIter,
                                            IProtocol* aActor,
                                            RefPtr<nsIInputStream>* aResult) {
  mozilla::Maybe<mozilla::ipc::IPCStream> ipcStream;
>>>>>>> upstream-releases
  if (!ReadIPDLParam(aMsg, aIter, aActor, &ipcStream)) {
    return false;
  }

  *aResult = mozilla::ipc::DeserializeIPCStream(ipcStream);
  return true;
}

}  // namespace ipc
}  // namespace mozilla
