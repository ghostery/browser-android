/* -*- Mode: C++; tab-width: 8; indent-tabs-mode: nil; c-basic-offset: 2 -*- */
/* vim: set ts=8 sts=2 et sw=2 tw=80: */
/* This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/. */

#include "base/process_util.h"
#include "base/task.h"

#ifdef OS_POSIX
#  include <errno.h>
#endif

#include "mozilla/IntegerPrintfMacros.h"

#include "mozilla/ipc/ProtocolUtils.h"

#include "mozilla/ipc/MessageChannel.h"
#include "mozilla/ipc/Transport.h"
#include "mozilla/recordreplay/ChildIPC.h"
#include "mozilla/recordreplay/ParentIPC.h"
#include "mozilla/StaticMutex.h"
#include "mozilla/SystemGroup.h"
#include "mozilla/Unused.h"
#include "nsPrintfCString.h"

#if defined(MOZ_SANDBOX) && defined(XP_WIN)
#  include "mozilla/sandboxTarget.h"
#endif

#if defined(XP_WIN)
#  include "aclapi.h"
#  include "sddl.h"

#  include "mozilla/TypeTraits.h"
#endif

#include "nsAutoPtr.h"

using namespace IPC;

using base::GetCurrentProcId;
using base::ProcessHandle;
using base::ProcessId;

namespace mozilla {

#if defined(XP_WIN)
// Generate RAII classes for LPTSTR and PSECURITY_DESCRIPTOR.
MOZ_TYPE_SPECIFIC_SCOPED_POINTER_TEMPLATE(ScopedLPTStr,
                                          RemovePointer<LPTSTR>::Type,
                                          ::LocalFree)
MOZ_TYPE_SPECIFIC_SCOPED_POINTER_TEMPLATE(
    ScopedPSecurityDescriptor, RemovePointer<PSECURITY_DESCRIPTOR>::Type,
    ::LocalFree)
#endif

namespace ipc {

IPCResult IPCResult::Fail(NotNull<IProtocol*> actor, const char* where,
                          const char* why) {
  // Calls top-level protocol to handle the error.
  nsPrintfCString errorMsg("%s %s\n", where, why);
  actor->GetIPCChannel()->Listener()->ProcessingError(
      HasResultCodes::MsgProcessingError, errorMsg.get());
  return IPCResult(false);
}

class ChannelOpened : public IPC::Message {
 public:
  ChannelOpened(TransportDescriptor aDescriptor, ProcessId aOtherProcess,
                ProtocolId aProtocol,
                NestedLevel aNestedLevel = NOT_NESTED)
      : IPC::Message(MSG_ROUTING_CONTROL,  // these only go to top-level actors
                     CHANNEL_OPENED_MESSAGE_TYPE, 0,
                     HeaderFlags(aNestedLevel)) {
    IPC::WriteParam(this, aDescriptor);
    IPC::WriteParam(this, aOtherProcess);
    IPC::WriteParam(this, static_cast<uint32_t>(aProtocol));
  }

  static bool Read(const IPC::Message& aMsg, TransportDescriptor* aDescriptor,
                   ProcessId* aOtherProcess, ProtocolId* aProtocol) {
    PickleIterator iter(aMsg);
    if (!IPC::ReadParam(&aMsg, &iter, aDescriptor) ||
        !IPC::ReadParam(&aMsg, &iter, aOtherProcess) ||
        !IPC::ReadParam(&aMsg, &iter, reinterpret_cast<uint32_t*>(aProtocol))) {
      return false;
    }
    aMsg.EndRead(iter);
    return true;
  }
};

nsresult Bridge(const PrivateIPDLInterface&, MessageChannel* aParentChannel,
                ProcessId aParentPid, MessageChannel* aChildChannel,
                ProcessId aChildPid, ProtocolId aProtocol,
                ProtocolId aChildProtocol) {
  if (!aParentPid || !aChildPid) {
    return NS_ERROR_INVALID_ARG;
  }

  TransportDescriptor parentSide, childSide;
  nsresult rv;
  if (NS_FAILED(rv = CreateTransport(aParentPid, &parentSide, &childSide))) {
    return rv;
  }

  if (!aParentChannel->Send(
          new ChannelOpened(parentSide, aChildPid, aProtocol,
                            IPC::Message::NESTED_INSIDE_CPOW))) {
    CloseDescriptor(parentSide);
    CloseDescriptor(childSide);
    return NS_ERROR_BRIDGE_OPEN_PARENT;
  }

  if (!aChildChannel->Send(
          new ChannelOpened(childSide, aParentPid, aChildProtocol,
                            IPC::Message::NESTED_INSIDE_CPOW))) {
    CloseDescriptor(parentSide);
    CloseDescriptor(childSide);
    return NS_ERROR_BRIDGE_OPEN_CHILD;
  }

  return NS_OK;
}

bool Open(const PrivateIPDLInterface&, MessageChannel* aOpenerChannel,
          ProcessId aOtherProcessId, Transport::Mode aOpenerMode,
          ProtocolId aProtocol, ProtocolId aChildProtocol) {
  bool isParent = (Transport::MODE_SERVER == aOpenerMode);
  ProcessId thisPid = GetCurrentProcId();
  ProcessId parentId = isParent ? thisPid : aOtherProcessId;
  ProcessId childId = !isParent ? thisPid : aOtherProcessId;
  if (!parentId || !childId) {
    return false;
  }

  TransportDescriptor parentSide, childSide;
  if (NS_FAILED(CreateTransport(parentId, &parentSide, &childSide))) {
    return false;
  }

  Message* parentMsg = new ChannelOpened(parentSide, childId, aProtocol);
  Message* childMsg = new ChannelOpened(childSide, parentId, aChildProtocol);
  nsAutoPtr<Message> messageForUs(isParent ? parentMsg : childMsg);
  nsAutoPtr<Message> messageForOtherSide(!isParent ? parentMsg : childMsg);
  if (!aOpenerChannel->Echo(messageForUs.forget()) ||
      !aOpenerChannel->Send(messageForOtherSide.forget())) {
    CloseDescriptor(parentSide);
    CloseDescriptor(childSide);
    return false;
  }
  return true;
}

bool UnpackChannelOpened(const PrivateIPDLInterface&, const Message& aMsg,
                         TransportDescriptor* aTransport,
                         ProcessId* aOtherProcess, ProtocolId* aProtocol) {
  return ChannelOpened::Read(aMsg, aTransport, aOtherProcess, aProtocol);
}

#if defined(XP_WIN)
bool DuplicateHandle(HANDLE aSourceHandle, DWORD aTargetProcessId,
                     HANDLE* aTargetHandle, DWORD aDesiredAccess,
                     DWORD aOptions) {
  // If our process is the target just duplicate the handle.
  if (aTargetProcessId == base::GetCurrentProcId()) {
    return !!::DuplicateHandle(::GetCurrentProcess(), aSourceHandle,
                               ::GetCurrentProcess(), aTargetHandle,
                               aDesiredAccess, false, aOptions);
  }

#  if defined(MOZ_SANDBOX)
  // Try the broker next (will fail if not sandboxed).
  if (SandboxTarget::Instance()->BrokerDuplicateHandle(
          aSourceHandle, aTargetProcessId, aTargetHandle, aDesiredAccess,
          aOptions)) {
    return true;
  }
#  endif

  // Finally, see if we already have access to the process.
  ScopedProcessHandle targetProcess(
      OpenProcess(PROCESS_DUP_HANDLE, FALSE, aTargetProcessId));
  if (!targetProcess) {
    CrashReporter::AnnotateCrashReport(
        CrashReporter::Annotation::IPCTransportFailureReason,
        NS_LITERAL_CSTRING("Failed to open target process."));
    return false;
  }

  return !!::DuplicateHandle(::GetCurrentProcess(), aSourceHandle,
                             targetProcess, aTargetHandle, aDesiredAccess,
                             FALSE, aOptions);
}
#endif

void AnnotateSystemError() {
  int64_t error = 0;
#if defined(XP_WIN)
  error = ::GetLastError();
#elif defined(OS_POSIX)
  error = errno;
#endif
  if (error) {
    CrashReporter::AnnotateCrashReport(
        CrashReporter::Annotation::IPCSystemError,
        nsPrintfCString("%" PRId64, error));
  }
}

#if defined(XP_MACOSX)
void AnnotateCrashReportWithErrno(CrashReporter::Annotation tag, int error) {
  CrashReporter::AnnotateCrashReport(tag, error);
}
#endif  // defined(XP_MACOSX)

void LogMessageForProtocol(const char* aTopLevelProtocol,
                           base::ProcessId aOtherPid,
                           const char* aContextDescription, uint32_t aMessageId,
                           MessageDirection aDirection) {
  nsPrintfCString logMessage(
      "[time: %" PRId64 "][%d%s%d] [%s] %s %s\n", PR_Now(),
      base::GetCurrentProcId(),
      aDirection == MessageDirection::eReceiving ? "<-" : "->", aOtherPid,
      aTopLevelProtocol, aContextDescription,
      StringFromIPCMessageType(aMessageId));
#ifdef ANDROID
  __android_log_write(ANDROID_LOG_INFO, "GeckoIPC", logMessage.get());
#endif
  fputs(logMessage.get(), stderr);
}

void ProtocolErrorBreakpoint(const char* aMsg) {
  // Bugs that generate these error messages can be tough to
  // reproduce.  Log always in the hope that someone finds the error
  // message.
  printf_stderr("IPDL protocol error: %s\n", aMsg);
}

void FatalError(const char* aMsg, bool aIsParent) {
#ifndef FUZZING
  ProtocolErrorBreakpoint(aMsg);
#endif

  nsAutoCString formattedMessage("IPDL error: \"");
  formattedMessage.AppendASCII(aMsg);
  if (aIsParent) {
    // We're going to crash the parent process because at this time
    // there's no other really nice way of getting a minidump out of
    // this process if we're off the main thread.
    formattedMessage.AppendLiteral("\". Intentionally crashing.");
    NS_ERROR(formattedMessage.get());
    CrashReporter::AnnotateCrashReport(
        CrashReporter::Annotation::IPCFatalErrorMsg, nsDependentCString(aMsg));
    AnnotateSystemError();
#ifndef FUZZING
    MOZ_CRASH("IPC FatalError in the parent process!");
#endif
  } else {
    formattedMessage.AppendLiteral("\". abort()ing as a result.");
#ifndef FUZZING
    MOZ_CRASH_UNSAFE(formattedMessage.get());
#endif
  }
}

<<<<<<< HEAD
void LogicError(const char* aMsg) { MOZ_CRASH_UNSAFE_OOL(aMsg); }
||||||| merged common ancestors
void
LogicError(const char* aMsg)
{
  MOZ_CRASH_UNSAFE_OOL(aMsg);
}
=======
void LogicError(const char* aMsg) { MOZ_CRASH_UNSAFE(aMsg); }
>>>>>>> upstream-releases

void ActorIdReadError(const char* aActorDescription) {
#ifndef FUZZING
  MOZ_CRASH_UNSAFE_PRINTF("Error deserializing id for %s", aActorDescription);
#endif
}

void BadActorIdError(const char* aActorDescription) {
  nsPrintfCString message("bad id for %s", aActorDescription);
  ProtocolErrorBreakpoint(message.get());
}

void ActorLookupError(const char* aActorDescription) {
  nsPrintfCString message("could not lookup id for %s", aActorDescription);
  ProtocolErrorBreakpoint(message.get());
}

void MismatchedActorTypeError(const char* aActorDescription) {
  nsPrintfCString message("actor that should be of type %s has different type",
                          aActorDescription);
  ProtocolErrorBreakpoint(message.get());
}

void UnionTypeReadError(const char* aUnionName) {
  MOZ_CRASH_UNSAFE_PRINTF("error deserializing type of union %s", aUnionName);
}

void ArrayLengthReadError(const char* aElementName) {
  MOZ_CRASH_UNSAFE_PRINTF("error deserializing length of %s[]", aElementName);
}

void SentinelReadError(const char* aClassName) {
  MOZ_CRASH_UNSAFE_PRINTF("incorrect sentinel when reading %s", aClassName);
}

<<<<<<< HEAD
bool StateTransition(bool aIsDelete, LivenessState* aNext) {
  switch (*aNext) {
    case LivenessState::Null:
      if (aIsDelete) {
        *aNext = LivenessState::Dead;
      }
      break;
    case LivenessState::Dead:
      return false;
    default:
      return false;
  }
  return true;
}

bool ReEntrantDeleteStateTransition(bool aIsDelete, bool aIsDeleteReply,
                                    ReEntrantDeleteLivenessState* aNext) {
  switch (*aNext) {
    case ReEntrantDeleteLivenessState::Null:
      if (aIsDelete) {
        *aNext = ReEntrantDeleteLivenessState::Dying;
      }
      break;
    case ReEntrantDeleteLivenessState::Dead:
      return false;
    case ReEntrantDeleteLivenessState::Dying:
      if (aIsDeleteReply) {
        *aNext = ReEntrantDeleteLivenessState::Dead;
      }
      break;
    default:
      return false;
  }
  return true;
}

void TableToArray(const nsTHashtable<nsPtrHashKey<void>>& aTable,
                  nsTArray<void*>& aArray) {
||||||| merged common ancestors
bool
StateTransition(bool aIsDelete, LivenessState* aNext)
{
  switch (*aNext) {
    case LivenessState::Null:
      if (aIsDelete) {
        *aNext = LivenessState::Dead;
      }
      break;
    case LivenessState::Dead:
      return false;
    default:
      return false;
  }
  return true;
}

bool
ReEntrantDeleteStateTransition(bool aIsDelete,
                               bool aIsDeleteReply,
                               ReEntrantDeleteLivenessState* aNext)
{
  switch (*aNext) {
    case ReEntrantDeleteLivenessState::Null:
      if (aIsDelete) {
        *aNext = ReEntrantDeleteLivenessState::Dying;
      }
      break;
    case ReEntrantDeleteLivenessState::Dead:
      return false;
    case ReEntrantDeleteLivenessState::Dying:
      if (aIsDeleteReply) {
        *aNext = ReEntrantDeleteLivenessState::Dead;
      }
      break;
    default:
      return false;
  }
  return true;
}

void
TableToArray(const nsTHashtable<nsPtrHashKey<void>>& aTable,
             nsTArray<void*>& aArray)
{
=======
void TableToArray(const nsTHashtable<nsPtrHashKey<void>>& aTable,
                  nsTArray<void*>& aArray) {
>>>>>>> upstream-releases
  uint32_t i = 0;
  void** elements = aArray.AppendElements(aTable.Count());
  for (auto iter = aTable.ConstIter(); !iter.Done(); iter.Next()) {
    elements[i] = iter.Get()->GetKey();
    ++i;
  }
}

<<<<<<< HEAD
Maybe<IProtocol*> IProtocol::ReadActor(const IPC::Message* aMessage,
                                       PickleIterator* aIter, bool aNullable,
                                       const char* aActorDescription,
                                       int32_t aProtocolTypeId) {
  int32_t id;
  if (!IPC::ReadParam(aMessage, aIter, &id)) {
    ActorIdReadError(aActorDescription);
    return Nothing();
  }
||||||| merged common ancestors
Maybe<IProtocol*>
IProtocol::ReadActor(const IPC::Message* aMessage, PickleIterator* aIter, bool aNullable,
                     const char* aActorDescription, int32_t aProtocolTypeId)
{
    int32_t id;
    if (!IPC::ReadParam(aMessage, aIter, &id)) {
        ActorIdReadError(aActorDescription);
        return Nothing();
    }
=======
ActorLifecycleProxy::ActorLifecycleProxy(IProtocol* aActor) : mActor(aActor) {
  MOZ_ASSERT(mActor);
  MOZ_ASSERT(mActor->CanSend(),
             "Cannot create LifecycleProxy for non-connected actor!");
>>>>>>> upstream-releases

<<<<<<< HEAD
  if (id == 1 || (id == 0 && !aNullable)) {
    BadActorIdError(aActorDescription);
    return Nothing();
  }

  if (id == 0) {
    return Some(static_cast<IProtocol*>(nullptr));
  }
||||||| merged common ancestors
    if (id == 1 || (id == 0 && !aNullable)) {
        BadActorIdError(aActorDescription);
        return Nothing();
    }

    if (id == 0) {
        return Some(static_cast<IProtocol*>(nullptr));
    }
=======
  // Take a reference to our manager's lifecycle proxy to try to hold it &
  // ensure it doesn't die before us.
  if (mActor->mManager) {
    mManager = mActor->mManager->mLifecycleProxy;
  }
}
>>>>>>> upstream-releases

<<<<<<< HEAD
  IProtocol* listener = this->Lookup(id);
  if (!listener) {
    ActorLookupError(aActorDescription);
    return Nothing();
  }
||||||| merged common ancestors
    IProtocol* listener = this->Lookup(id);
    if (!listener) {
        ActorLookupError(aActorDescription);
        return Nothing();
    }
=======
ActorLifecycleProxy::~ActorLifecycleProxy() {
  // When the LifecycleProxy's lifetime has come to an end, it means that the
  // actor should have its `Dealloc` method called on it. In a well-behaved
  // actor, this will release the IPC-held reference to the actor.
  //
  // If the actor has already died before the `LifecycleProxy`, the `IProtocol`
  // destructor below will clear our reference to it, preventing us from
  // performing a use-after-free here.
  if (!mActor) {
    return;
  }
>>>>>>> upstream-releases

<<<<<<< HEAD
  if (listener->GetProtocolTypeId() != aProtocolTypeId) {
    MismatchedActorTypeError(aActorDescription);
    return Nothing();
  }

  return Some(listener);
||||||| merged common ancestors
    if (listener->GetProtocolTypeId() != aProtocolTypeId) {
        MismatchedActorTypeError(aActorDescription);
        return Nothing();
    }

    return Some(listener);
=======
  // Clear our actor's state back to inactive, and then invoke ActorDealloc.
  MOZ_ASSERT(mActor->mLinkStatus == LinkStatus::Destroyed,
             "Deallocating non-destroyed actor!");
  mActor->mLifecycleProxy = nullptr;
  mActor->mLinkStatus = LinkStatus::Inactive;
  mActor->ActorDealloc();
  mActor = nullptr;
}

IProtocol::~IProtocol() {
  // If the actor still has a lifecycle proxy when it is being torn down, it
  // means that IPC was not given control over the lifecycle of the actor
  // correctly. Usually this means that the actor was destroyed while IPC is
  // calling a message handler for it, and the actor incorrectly frees itself
  // during that operation.
  //
  // As this happens unfortunately frequently, due to many odd protocols in
  // Gecko, simply emit a warning and clear the weak backreference from our
  // LifecycleProxy back to us.
  if (mLifecycleProxy) {
    // FIXME: It would be nice to have this print out the name of the
    // misbehaving actor, to help people notice it's their fault!
    NS_WARNING(
        "Actor destructor called before IPC lifecycle complete!\n"
        "References to this actor may unexpectedly dangle!");

    mLifecycleProxy->mActor = nullptr;

    // If we are somehow being destroyed while active, make sure that the
    // existing IPC reference has been freed. If the status of the actor is
    // `Destroyed`, the reference has already been freed, and we shouldn't free
    // it a second time.
    MOZ_ASSERT(mLinkStatus != LinkStatus::Inactive);
    if (mLinkStatus != LinkStatus::Destroyed) {
      NS_IF_RELEASE(mLifecycleProxy);
    }
    mLifecycleProxy = nullptr;
  }
>>>>>>> upstream-releases
}

<<<<<<< HEAD
int32_t IProtocol::ManagedState::Register(IProtocol* aRouted) {
  return mProtocol->Manager()->Register(aRouted);
||||||| merged common ancestors
int32_t
IProtocol::ManagedState::Register(IProtocol* aRouted)
{
  return mProtocol->Manager()->Register(aRouted);
=======
// The following methods either directly forward to the toplevel protocol, or
// almost directly do.
int32_t IProtocol::Register(IProtocol* aRouted) {
  return mToplevel->Register(aRouted);
>>>>>>> upstream-releases
}
<<<<<<< HEAD

int32_t IProtocol::ManagedState::RegisterID(IProtocol* aRouted, int32_t aId) {
  return mProtocol->Manager()->RegisterID(aRouted, aId);
||||||| merged common ancestors

int32_t
IProtocol::ManagedState::RegisterID(IProtocol* aRouted, int32_t aId)
{
  return mProtocol->Manager()->RegisterID(aRouted, aId);
=======
int32_t IProtocol::RegisterID(IProtocol* aRouted, int32_t aId) {
  return mToplevel->RegisterID(aRouted, aId);
>>>>>>> upstream-releases
}
<<<<<<< HEAD

IProtocol* IProtocol::ManagedState::Lookup(int32_t aId) {
  return mProtocol->Manager()->Lookup(aId);
||||||| merged common ancestors

IProtocol*
IProtocol::ManagedState::Lookup(int32_t aId)
{
  return mProtocol->Manager()->Lookup(aId);
=======
IProtocol* IProtocol::Lookup(int32_t aId) { return mToplevel->Lookup(aId); }
void IProtocol::Unregister(int32_t aId) {
  if (aId == mId) {
    mId = kFreedActorId;
  }
  return mToplevel->Unregister(aId);
>>>>>>> upstream-releases
}

<<<<<<< HEAD
void IProtocol::ManagedState::Unregister(int32_t aId) {
  if (mProtocol->mId == aId) {
    mProtocol->mId = kFreedActorId;
  }
  mProtocol->Manager()->Unregister(aId);
||||||| merged common ancestors
void
IProtocol::ManagedState::Unregister(int32_t aId)
{
  if (mProtocol->mId == aId) {
    mProtocol->mId = kFreedActorId;
  }
  mProtocol->Manager()->Unregister(aId);
=======
Shmem::SharedMemory* IProtocol::CreateSharedMemory(
    size_t aSize, SharedMemory::SharedMemoryType aType, bool aUnsafe,
    int32_t* aId) {
  return mToplevel->CreateSharedMemory(aSize, aType, aUnsafe, aId);
}
Shmem::SharedMemory* IProtocol::LookupSharedMemory(int32_t aId) {
  return mToplevel->LookupSharedMemory(aId);
}
bool IProtocol::IsTrackingSharedMemory(Shmem::SharedMemory* aSegment) {
  return mToplevel->IsTrackingSharedMemory(aSegment);
}
bool IProtocol::DestroySharedMemory(Shmem& aShmem) {
  return mToplevel->DestroySharedMemory(aShmem);
>>>>>>> upstream-releases
}

<<<<<<< HEAD
Shmem::SharedMemory* IProtocol::ManagedState::CreateSharedMemory(
    size_t aSize, SharedMemory::SharedMemoryType aType, bool aUnsafe,
    int32_t* aId) {
  return mProtocol->Manager()->CreateSharedMemory(aSize, aType, aUnsafe, aId);
||||||| merged common ancestors
Shmem::SharedMemory*
IProtocol::ManagedState::CreateSharedMemory(size_t aSize,
                                            SharedMemory::SharedMemoryType aType,
                                            bool aUnsafe,
                                            int32_t* aId)
{
  return mProtocol->Manager()->CreateSharedMemory(aSize, aType, aUnsafe, aId);
=======
MessageChannel* IProtocol::GetIPCChannel() {
  return mToplevel->GetIPCChannel();
}
const MessageChannel* IProtocol::GetIPCChannel() const {
  return mToplevel->GetIPCChannel();
>>>>>>> upstream-releases
}

<<<<<<< HEAD
Shmem::SharedMemory* IProtocol::ManagedState::LookupSharedMemory(int32_t aId) {
  return mProtocol->Manager()->LookupSharedMemory(aId);
||||||| merged common ancestors
Shmem::SharedMemory*
IProtocol::ManagedState::LookupSharedMemory(int32_t aId)
{
  return mProtocol->Manager()->LookupSharedMemory(aId);
=======
void IProtocol::SetEventTargetForActor(IProtocol* aActor,
                                       nsIEventTarget* aEventTarget) {
  // Make sure we have a manager for the internal method to access.
  aActor->SetManager(this);
  mToplevel->SetEventTargetForActorInternal(aActor, aEventTarget);
>>>>>>> upstream-releases
}

<<<<<<< HEAD
bool IProtocol::ManagedState::IsTrackingSharedMemory(
    Shmem::SharedMemory* aSegment) {
  return mProtocol->Manager()->IsTrackingSharedMemory(aSegment);
||||||| merged common ancestors
bool
IProtocol::ManagedState::IsTrackingSharedMemory(Shmem::SharedMemory* aSegment)
{
  return mProtocol->Manager()->IsTrackingSharedMemory(aSegment);
=======
void IProtocol::ReplaceEventTargetForActor(IProtocol* aActor,
                                           nsIEventTarget* aEventTarget) {
  MOZ_ASSERT(aActor->Manager());
  mToplevel->ReplaceEventTargetForActor(aActor, aEventTarget);
>>>>>>> upstream-releases
}

<<<<<<< HEAD
bool IProtocol::ManagedState::DestroySharedMemory(Shmem& aShmem) {
  return mProtocol->Manager()->DestroySharedMemory(aShmem);
||||||| merged common ancestors
bool
IProtocol::ManagedState::DestroySharedMemory(Shmem& aShmem)
{
  return mProtocol->Manager()->DestroySharedMemory(aShmem);
=======
void IProtocol::SetEventTargetForRoute(int32_t aRoute,
                                       nsIEventTarget* aEventTarget) {
  mToplevel->SetEventTargetForRoute(aRoute, aEventTarget);
>>>>>>> upstream-releases
}

<<<<<<< HEAD
const MessageChannel* IProtocol::ManagedState::GetIPCChannel() const {
  return mChannel;
||||||| merged common ancestors
const MessageChannel*
IProtocol::ManagedState::GetIPCChannel() const
{
  return mChannel;
=======
nsIEventTarget* IProtocol::GetActorEventTarget() {
  // FIXME: It's a touch sketchy that we don't return a strong reference here.
  RefPtr<nsIEventTarget> target = GetActorEventTarget(this);
  return target;
>>>>>>> upstream-releases
}
already_AddRefed<nsIEventTarget> IProtocol::GetActorEventTarget(
    IProtocol* aActor) {
  return mToplevel->GetActorEventTarget(aActor);
}

ProcessId IProtocol::OtherPid() const { return mToplevel->OtherPid(); }

<<<<<<< HEAD
MessageChannel* IProtocol::ManagedState::GetIPCChannel() { return mChannel; }
||||||| merged common ancestors
MessageChannel*
IProtocol::ManagedState::GetIPCChannel()
{
  return mChannel;
}
=======
void IProtocol::SetId(int32_t aId) {
  MOZ_ASSERT(mId == aId || mLinkStatus == LinkStatus::Inactive);
  mId = aId;
}
>>>>>>> upstream-releases

<<<<<<< HEAD
ProcessId IProtocol::OtherPid() const { return Manager()->OtherPid(); }
||||||| merged common ancestors
ProcessId
IProtocol::OtherPid() const
{
  return Manager()->OtherPid();
}
=======
Maybe<IProtocol*> IProtocol::ReadActor(const IPC::Message* aMessage,
                                       PickleIterator* aIter, bool aNullable,
                                       const char* aActorDescription,
                                       int32_t aProtocolTypeId) {
  int32_t id;
  if (!IPC::ReadParam(aMessage, aIter, &id)) {
    ActorIdReadError(aActorDescription);
    return Nothing();
  }

  if (id == 1 || (id == 0 && !aNullable)) {
    BadActorIdError(aActorDescription);
    return Nothing();
  }

  if (id == 0) {
    return Some(static_cast<IProtocol*>(nullptr));
  }

  IProtocol* listener = this->Lookup(id);
  if (!listener) {
    ActorLookupError(aActorDescription);
    return Nothing();
  }

  if (listener->GetProtocolId() != aProtocolTypeId) {
    MismatchedActorTypeError(aActorDescription);
    return Nothing();
  }

  return Some(listener);
}
>>>>>>> upstream-releases

void IProtocol::FatalError(const char* const aErrorMsg) const {
  HandleFatalError(aErrorMsg);
}

void IProtocol::HandleFatalError(const char* aErrorMsg) const {
  if (IProtocol* manager = Manager()) {
    manager->HandleFatalError(aErrorMsg);
    return;
  }

  mozilla::ipc::FatalError(aErrorMsg, mSide == ParentSide);
}

bool IProtocol::AllocShmem(size_t aSize,
                           Shmem::SharedMemory::SharedMemoryType aType,
                           Shmem* aOutMem) {
  Shmem::id_t id;
  Shmem::SharedMemory* rawmem(CreateSharedMemory(aSize, aType, false, &id));
  if (!rawmem) {
    return false;
  }

  *aOutMem = Shmem(Shmem::PrivateIPDLCaller(), rawmem, id);
  return true;
}

bool IProtocol::AllocUnsafeShmem(size_t aSize,
                                 Shmem::SharedMemory::SharedMemoryType aType,
                                 Shmem* aOutMem) {
  Shmem::id_t id;
  Shmem::SharedMemory* rawmem(CreateSharedMemory(aSize, aType, true, &id));
  if (!rawmem) {
    return false;
  }

  *aOutMem = Shmem(Shmem::PrivateIPDLCaller(), rawmem, id);
  return true;
}

bool IProtocol::DeallocShmem(Shmem& aMem) {
  bool ok = DestroySharedMemory(aMem);
#ifdef DEBUG
  if (!ok) {
    if (mSide == ChildSide) {
      FatalError("bad Shmem");
    } else {
      NS_WARNING("bad Shmem");
    }
    return false;
  }
#endif  // DEBUG
  aMem.forget(Shmem::PrivateIPDLCaller());
  return ok;
}

void IProtocol::SetManager(IProtocol* aManager) {
  MOZ_RELEASE_ASSERT(!mManager || mManager == aManager);
  mManager = aManager;
  mToplevel = aManager->mToplevel;
}

void IProtocol::SetManagerAndRegister(IProtocol* aManager) {
  // Set the manager prior to registering so registering properly inherits
  // the manager's event target.
  SetManager(aManager);

  aManager->Register(this);
}

void IProtocol::SetManagerAndRegister(IProtocol* aManager, int32_t aId) {
  // Set the manager prior to registering so registering properly inherits
  // the manager's event target.
  SetManager(aManager);

  aManager->RegisterID(this, aId);
}

<<<<<<< HEAD
void IProtocol::SetEventTargetForActor(IProtocol* aActor,
                                       nsIEventTarget* aEventTarget) {
  // Make sure we have a manager for the internal method to access.
  aActor->SetManager(this);
  mState->SetEventTargetForActor(aActor, aEventTarget);
}
||||||| merged common ancestors
void
IProtocol::SetEventTargetForActor(IProtocol* aActor, nsIEventTarget* aEventTarget)
{
  // Make sure we have a manager for the internal method to access.
  aActor->SetManager(this);
  mState->SetEventTargetForActor(aActor, aEventTarget);
}
=======
bool IProtocol::ChannelSend(IPC::Message* aMsg) {
  UniquePtr<IPC::Message> msg(aMsg);
  if (CanSend()) {
    return GetIPCChannel()->Send(msg.release());
  }
>>>>>>> upstream-releases

<<<<<<< HEAD
void IProtocol::ReplaceEventTargetForActor(IProtocol* aActor,
                                           nsIEventTarget* aEventTarget) {
  // Ensure the actor has been registered.
  MOZ_ASSERT(aActor->Manager());
  mState->ReplaceEventTargetForActor(aActor, aEventTarget);
||||||| merged common ancestors
void
IProtocol::ReplaceEventTargetForActor(IProtocol* aActor,
                                      nsIEventTarget* aEventTarget)
{
  // Ensure the actor has been registered.
  MOZ_ASSERT(aActor->Manager());
  mState->ReplaceEventTargetForActor(aActor, aEventTarget);
=======
  NS_WARNING("IPC message discarded: actor cannot send");
  return false;
>>>>>>> upstream-releases
}

<<<<<<< HEAD
nsIEventTarget* IProtocol::GetActorEventTarget() {
  return mState->GetActorEventTarget();
}
||||||| merged common ancestors
nsIEventTarget*
IProtocol::GetActorEventTarget()
{
  return mState->GetActorEventTarget();
}
=======
bool IProtocol::ChannelSend(IPC::Message* aMsg, IPC::Message* aReply) {
  UniquePtr<IPC::Message> msg(aMsg);
  if (CanSend()) {
    return GetIPCChannel()->Send(msg.release(), aReply);
  }
>>>>>>> upstream-releases

<<<<<<< HEAD
already_AddRefed<nsIEventTarget> IProtocol::GetActorEventTarget(
    IProtocol* aActor) {
  return mState->GetActorEventTarget(aActor);
||||||| merged common ancestors
already_AddRefed<nsIEventTarget>
IProtocol::GetActorEventTarget(IProtocol* aActor)
{
  return mState->GetActorEventTarget(aActor);
=======
  NS_WARNING("IPC message discarded: actor cannot send");
  return false;
>>>>>>> upstream-releases
}

<<<<<<< HEAD
nsIEventTarget* IProtocol::ManagedState::GetActorEventTarget() {
  // We should only call this function when this actor has been registered and
  // is not unregistered yet.
  MOZ_RELEASE_ASSERT(mProtocol->mId != kNullActorId &&
                     mProtocol->mId != kFreedActorId);
  RefPtr<nsIEventTarget> target = GetActorEventTarget(mProtocol);
  return target;
||||||| merged common ancestors
nsIEventTarget*
IProtocol::ManagedState::GetActorEventTarget()
{
  // We should only call this function when this actor has been registered and
  // is not unregistered yet.
  MOZ_RELEASE_ASSERT(mProtocol->mId != kNullActorId && mProtocol->mId != kFreedActorId);
  RefPtr<nsIEventTarget> target = GetActorEventTarget(mProtocol);
  return target;
=======
bool IProtocol::ChannelCall(IPC::Message* aMsg, IPC::Message* aReply) {
  UniquePtr<IPC::Message> msg(aMsg);
  if (CanSend()) {
    return GetIPCChannel()->Call(msg.release(), aReply);
  }

  NS_WARNING("IPC message discarded: actor cannot send");
  return false;
>>>>>>> upstream-releases
}

<<<<<<< HEAD
void IProtocol::ManagedState::SetEventTargetForActor(
    IProtocol* aActor, nsIEventTarget* aEventTarget) {
  // Go directly through the state so we don't try to redundantly (and
  // wrongly) call SetManager() on aActor.
  mProtocol->Manager()->mState->SetEventTargetForActor(aActor, aEventTarget);
||||||| merged common ancestors
void
IProtocol::ManagedState::SetEventTargetForActor(IProtocol* aActor,
                                                        nsIEventTarget* aEventTarget)
{
  // Go directly through the state so we don't try to redundantly (and
  // wrongly) call SetManager() on aActor.
  mProtocol->Manager()->mState->SetEventTargetForActor(aActor, aEventTarget);
=======
void IProtocol::ActorConnected() {
  if (mLinkStatus != LinkStatus::Inactive) {
    return;
  }

  mLinkStatus = LinkStatus::Connected;

  MOZ_ASSERT(!mLifecycleProxy, "double-connecting live actor");
  mLifecycleProxy = new ActorLifecycleProxy(this);
  NS_ADDREF(mLifecycleProxy);  // Reference freed in DestroySubtree();
>>>>>>> upstream-releases
}

<<<<<<< HEAD
void IProtocol::ManagedState::ReplaceEventTargetForActor(
    IProtocol* aActor, nsIEventTarget* aEventTarget) {
  mProtocol->Manager()->ReplaceEventTargetForActor(aActor, aEventTarget);
||||||| merged common ancestors
void
IProtocol::ManagedState::ReplaceEventTargetForActor(IProtocol* aActor,
                                                            nsIEventTarget* aEventTarget)
{
  mProtocol->Manager()->ReplaceEventTargetForActor(aActor, aEventTarget);
=======
void IProtocol::DoomSubtree() {
  MOZ_ASSERT(CanSend(), "dooming non-connected actor");
  MOZ_ASSERT(mLifecycleProxy, "dooming zombie actor");

  nsTArray<RefPtr<ActorLifecycleProxy>> managed;
  AllManagedActors(managed);
  for (ActorLifecycleProxy* proxy : managed) {
    // Guard against actor being disconnected or destroyed during previous Doom
    IProtocol* actor = proxy->Get();
    if (actor && actor->CanSend()) {
      actor->DoomSubtree();
    }
  }

  // ActorDoom is called immediately before changing state, this allows messages
  // to be sent during ActorDoom immediately before the channel is closed and
  // sending messages is disabled.
  ActorDoom();
  mLinkStatus = LinkStatus::Doomed;
>>>>>>> upstream-releases
}

<<<<<<< HEAD
already_AddRefed<nsIEventTarget> IProtocol::ManagedState::GetActorEventTarget(
    IProtocol* aActor) {
  return mProtocol->Manager()->GetActorEventTarget(aActor);
||||||| merged common ancestors
already_AddRefed<nsIEventTarget>
IProtocol::ManagedState::GetActorEventTarget(IProtocol* aActor)
{
  return mProtocol->Manager()->GetActorEventTarget(aActor);
=======
void IProtocol::DestroySubtree(ActorDestroyReason aWhy) {
  MOZ_ASSERT(CanRecv(), "destroying non-connected actor");
  MOZ_ASSERT(mLifecycleProxy, "destroying zombie actor");

  // If we're a managed actor, unregister from our manager
  if (Manager()) {
    Unregister(Id());
  }

  // Destroy subtree
  ActorDestroyReason subtreeWhy = aWhy;
  if (aWhy == Deletion || aWhy == FailedConstructor) {
    subtreeWhy = AncestorDeletion;
  }

  nsTArray<RefPtr<ActorLifecycleProxy>> managed;
  AllManagedActors(managed);
  for (ActorLifecycleProxy* proxy : managed) {
    // Guard against actor being disconnected or destroyed during previous
    // Destroy
    IProtocol* actor = proxy->Get();
    if (actor && actor->CanRecv()) {
      actor->DestroySubtree(subtreeWhy);
    }
  }

  // Ensure that we don't send any messages while we're calling `ActorDestroy`
  // by setting our state to `Doomed`.
  mLinkStatus = LinkStatus::Doomed;

  // The actor is being destroyed, reject any pending responses, invoke
  // `ActorDestroy` to destroy it, and then clear our status to
  // `LinkStatus::Destroyed`.
  GetIPCChannel()->RejectPendingResponsesForActor(this);
  ActorDestroy(aWhy);
  mLinkStatus = LinkStatus::Destroyed;
>>>>>>> upstream-releases
}

IToplevelProtocol::IToplevelProtocol(const char* aName, ProtocolId aProtoId,
                                     Side aSide)
<<<<<<< HEAD
    : IProtocol(aSide, MakeUnique<ToplevelState>(aName, this, aSide)),
      mProtocolId(aProtoId),
      mOtherPid(mozilla::ipc::kInvalidProcessId),
      mIsMainThreadProtocol(false) {}

IToplevelProtocol::~IToplevelProtocol() {
  mState = nullptr;
||||||| merged common ancestors
  : IProtocol(aSide, MakeUnique<ToplevelState>(aName, this, aSide))
  , mMonitor("mozilla.ipc.IToplevelProtocol.mMonitor")
  , mProtocolId(aProtoId)
  , mOtherPid(mozilla::ipc::kInvalidProcessId)
  , mOtherPidState(ProcessIdState::eUnstarted)
  , mIsMainThreadProtocol(false)
{
}

IToplevelProtocol::~IToplevelProtocol()
{
  mState = nullptr;
=======
    : IProtocol(aProtoId, aSide),
      mOtherPid(mozilla::ipc::kInvalidProcessId),
      mLastLocalId(0),
      mEventTargetMutex("ProtocolEventTargetMutex"),
      mMiddlemanChannelOverride(nullptr),
      mChannel(aName, this) {
  mToplevel = this;
}

IToplevelProtocol::~IToplevelProtocol() {
>>>>>>> upstream-releases
  if (mTrans) {
    RefPtr<DeleteTask<Transport>> task =
        new DeleteTask<Transport>(mTrans.release());
    XRE_GetIOMessageLoop()->PostTask(task.forget());
  }
}

base::ProcessId IToplevelProtocol::OtherPid() const {
  base::ProcessId pid = OtherPidMaybeInvalid();
  MOZ_RELEASE_ASSERT(pid != kInvalidProcessId);
  return pid;
}

<<<<<<< HEAD
base::ProcessId IToplevelProtocol::OtherPidMaybeInvalid() const {
  return mOtherPid;
}

void IToplevelProtocol::SetOtherProcessId(base::ProcessId aOtherPid) {
||||||| merged common ancestors
base::ProcessId
IToplevelProtocol::OtherPidMaybeInvalid() const
{
  MonitorAutoLock lock(mMonitor);

  if (mOtherPidState == ProcessIdState::eUnstarted) {
    // If you're asking for the pid of a process we haven't even tried to
    // start, you get an invalid pid back immediately.
    return kInvalidProcessId;
  }

  while (mOtherPidState < ProcessIdState::eReady) {
    lock.Wait();
  }
  MOZ_RELEASE_ASSERT(mOtherPidState == ProcessIdState::eReady);

  return mOtherPid;
}

void
IToplevelProtocol::SetOtherProcessId(base::ProcessId aOtherPid,
                                     ProcessIdState aState)
{
  MonitorAutoLock lock(mMonitor);
=======
void IToplevelProtocol::SetOtherProcessId(base::ProcessId aOtherPid) {
>>>>>>> upstream-releases
  // When recording an execution, all communication we do is forwarded from
  // the middleman to the parent process, so use its pid instead of the
  // middleman's pid.
  if (recordreplay::IsRecordingOrReplaying() &&
      aOtherPid == recordreplay::child::MiddlemanProcessId()) {
    mOtherPid = recordreplay::child::ParentProcessId();
  } else {
    mOtherPid = aOtherPid;
  }
<<<<<<< HEAD
}

bool IToplevelProtocol::TakeMinidump(nsIFile** aDump, uint32_t* aSequence) {
  MOZ_RELEASE_ASSERT(GetSide() == ParentSide);
  return XRE_TakeMinidumpForChild(OtherPid(), aDump, aSequence);
||||||| merged common ancestors
  mOtherPidState = aState;
  lock.NotifyAll();
}

bool
IToplevelProtocol::TakeMinidump(nsIFile** aDump, uint32_t* aSequence)
{
  MOZ_RELEASE_ASSERT(GetSide() == ParentSide);
  return XRE_TakeMinidumpForChild(OtherPid(), aDump, aSequence);
=======
>>>>>>> upstream-releases
}

bool IToplevelProtocol::Open(mozilla::ipc::Transport* aTransport,
                             base::ProcessId aOtherPid, MessageLoop* aThread,
                             mozilla::ipc::Side aSide) {
  SetOtherProcessId(aOtherPid);
  return GetIPCChannel()->Open(aTransport, aThread, aSide);
}

bool IToplevelProtocol::Open(MessageChannel* aChannel,
                             MessageLoop* aMessageLoop,
                             mozilla::ipc::Side aSide) {
  SetOtherProcessId(base::GetCurrentProcId());
  return GetIPCChannel()->Open(aChannel, aMessageLoop->SerialEventTarget(),
                               aSide);
}

bool IToplevelProtocol::Open(MessageChannel* aChannel,
                             nsIEventTarget* aEventTarget,
                             mozilla::ipc::Side aSide) {
  SetOtherProcessId(base::GetCurrentProcId());
  return GetIPCChannel()->Open(aChannel, aEventTarget, aSide);
}

bool IToplevelProtocol::OpenWithAsyncPid(mozilla::ipc::Transport* aTransport,
                                         MessageLoop* aThread,
                                         mozilla::ipc::Side aSide) {
  return GetIPCChannel()->Open(aTransport, aThread, aSide);
}

bool IToplevelProtocol::OpenOnSameThread(MessageChannel* aChannel, Side aSide) {
  SetOtherProcessId(base::GetCurrentProcId());
  return GetIPCChannel()->OpenOnSameThread(aChannel, aSide);
}

void IToplevelProtocol::Close() { GetIPCChannel()->Close(); }

void IToplevelProtocol::SetReplyTimeoutMs(int32_t aTimeoutMs) {
  GetIPCChannel()->SetReplyTimeoutMs(aTimeoutMs);
}

bool IToplevelProtocol::IsOnCxxStack() const {
  return GetIPCChannel()->IsOnCxxStack();
}

<<<<<<< HEAD
int32_t IToplevelProtocol::ToplevelState::NextId() {
  // Genreate the next ID to use for a shared memory or protocol. Parent and
  // Child sides of the protocol use different pools, and actors created in the
  // middleman need to use a distinct pool as well.
  int32_t tag = 0;
  if (recordreplay::IsMiddleman()) {
    tag |= 1 << 0;
  }
  if (mProtocol->GetSide() == ParentSide) {
    tag |= 1 << 1;
  }

  // Compute the ID to use with the low two bits as our tag, and the remaining
  // bits as a monotonic.
  return (++mLastLocalId << 2) | tag;
}

int32_t IToplevelProtocol::ToplevelState::Register(IProtocol* aRouted) {
||||||| merged common ancestors
int32_t
IToplevelProtocol::ToplevelState::Register(IProtocol* aRouted)
{
=======
int32_t IToplevelProtocol::NextId() {
  // Genreate the next ID to use for a shared memory or protocol. Parent and
  // Child sides of the protocol use different pools, and actors created in the
  // middleman need to use a distinct pool as well.
  int32_t tag = 0;
  if (recordreplay::IsMiddleman()) {
    tag |= 1 << 0;
  }
  if (GetSide() == ParentSide) {
    tag |= 1 << 1;
  }

  // Check any overflow
  MOZ_RELEASE_ASSERT(mLastLocalId < (1 << 29));

  // Compute the ID to use with the low two bits as our tag, and the remaining
  // bits as a monotonic.
  return (++mLastLocalId << 2) | tag;
}

int32_t IToplevelProtocol::Register(IProtocol* aRouted) {
>>>>>>> upstream-releases
  if (aRouted->Id() != kNullActorId && aRouted->Id() != kFreedActorId) {
    // If there's already an ID, just return that.
    return aRouted->Id();
  }
<<<<<<< HEAD
  int32_t id = NextId();
  mActorMap.AddWithID(aRouted, id);
  aRouted->SetId(id);
||||||| merged common ancestors
  int32_t id = mProtocol->GetSide() == ParentSide ? ++mLastRouteId : --mLastRouteId;
  mActorMap.AddWithID(aRouted, id);
  aRouted->SetId(id);
=======
  int32_t id = RegisterID(aRouted, NextId());
>>>>>>> upstream-releases

  // Inherit our event target from our manager.
  if (IProtocol* manager = aRouted->Manager()) {
    MutexAutoLock lock(mEventTargetMutex);
    if (nsCOMPtr<nsIEventTarget> target =
            mEventTargetMap.Lookup(manager->Id())) {
      mEventTargetMap.AddWithID(target, id);
    }
  }

  return id;
}

<<<<<<< HEAD
int32_t IToplevelProtocol::ToplevelState::RegisterID(IProtocol* aRouted,
                                                     int32_t aId) {
  mActorMap.AddWithID(aRouted, aId);
||||||| merged common ancestors
int32_t
IToplevelProtocol::ToplevelState::RegisterID(IProtocol* aRouted,
                                     int32_t aId)
{
  mActorMap.AddWithID(aRouted, aId);
=======
int32_t IToplevelProtocol::RegisterID(IProtocol* aRouted, int32_t aId) {
>>>>>>> upstream-releases
  aRouted->SetId(aId);
  aRouted->ActorConnected();
  mActorMap.AddWithID(aRouted, aId);
  return aId;
}

<<<<<<< HEAD
IProtocol* IToplevelProtocol::ToplevelState::Lookup(int32_t aId) {
||||||| merged common ancestors
IProtocol*
IToplevelProtocol::ToplevelState::Lookup(int32_t aId)
{
=======
IProtocol* IToplevelProtocol::Lookup(int32_t aId) {
>>>>>>> upstream-releases
  return mActorMap.Lookup(aId);
}

<<<<<<< HEAD
void IToplevelProtocol::ToplevelState::Unregister(int32_t aId) {
||||||| merged common ancestors
void
IToplevelProtocol::ToplevelState::Unregister(int32_t aId)
{
=======
void IToplevelProtocol::Unregister(int32_t aId) {
>>>>>>> upstream-releases
  mActorMap.Remove(aId);

  MutexAutoLock lock(mEventTargetMutex);
  mEventTargetMap.RemoveIfPresent(aId);
}

<<<<<<< HEAD
IToplevelProtocol::ToplevelState::ToplevelState(const char* aName,
                                                IToplevelProtocol* aProtocol,
                                                Side aSide)
    : ProtocolState(),
      mProtocol(aProtocol),
      mLastLocalId(0),
      mEventTargetMutex("ProtocolEventTargetMutex"),
      mChannel(aName, aProtocol) {}

Shmem::SharedMemory* IToplevelProtocol::ToplevelState::CreateSharedMemory(
    size_t aSize, Shmem::SharedMemory::SharedMemoryType aType, bool aUnsafe,
    Shmem::id_t* aId) {
  // XXX the mProtocol uses here should go away!
||||||| merged common ancestors
IToplevelProtocol::ToplevelState::ToplevelState(const char* aName,
                                                IToplevelProtocol* aProtocol,
                                                Side aSide)
  : ProtocolState()
  , mProtocol(aProtocol)
  , mLastRouteId(aSide == ParentSide ? kFreedActorId : kNullActorId)
  , mLastShmemId(aSide == ParentSide ? kFreedActorId : kNullActorId)
  , mEventTargetMutex("ProtocolEventTargetMutex")
  , mChannel(aName, aProtocol)
{
}

Shmem::SharedMemory*
IToplevelProtocol::ToplevelState::CreateSharedMemory(size_t aSize,
                                                     Shmem::SharedMemory::SharedMemoryType aType,
                                                     bool aUnsafe,
                                                     Shmem::id_t* aId)
{
  // XXX the mProtocol uses here should go away!
=======
Shmem::SharedMemory* IToplevelProtocol::CreateSharedMemory(
    size_t aSize, Shmem::SharedMemory::SharedMemoryType aType, bool aUnsafe,
    Shmem::id_t* aId) {
>>>>>>> upstream-releases
  RefPtr<Shmem::SharedMemory> segment(
      Shmem::Alloc(Shmem::PrivateIPDLCaller(), aSize, aType, aUnsafe));
  if (!segment) {
    return nullptr;
  }
  int32_t id = NextId();
  Shmem shmem(Shmem::PrivateIPDLCaller(), segment.get(), id);

  base::ProcessId pid =
#ifdef ANDROID
<<<<<<< HEAD
      // We use OtherPidMaybeInvalid() because on Android this method is
      // actually called on an unconnected protocol, but Android's shared memory
      // implementation doesn't actually use the PID.
      mProtocol->OtherPidMaybeInvalid();
||||||| merged common ancestors
    // We use OtherPidMaybeInvalid() because on Android this method is actually
    // called on an unconnected protocol, but Android's shared memory
    // implementation doesn't actually use the PID.
    mProtocol->OtherPidMaybeInvalid();
=======
      // We use OtherPidMaybeInvalid() because on Android this method is
      // actually called on an unconnected protocol, but Android's shared memory
      // implementation doesn't actually use the PID.
      OtherPidMaybeInvalid();
>>>>>>> upstream-releases
#else
<<<<<<< HEAD
      mProtocol->OtherPid();
||||||| merged common ancestors
    mProtocol->OtherPid();
=======
      OtherPid();
>>>>>>> upstream-releases
#endif

  Message* descriptor =
      shmem.ShareTo(Shmem::PrivateIPDLCaller(), pid, MSG_ROUTING_CONTROL);
  if (!descriptor) {
    return nullptr;
  }
  Unused << GetIPCChannel()->Send(descriptor);

  *aId = shmem.Id(Shmem::PrivateIPDLCaller());
  Shmem::SharedMemory* rawSegment = segment.get();
  mShmemMap.AddWithID(segment.forget().take(), *aId);
  return rawSegment;
}

<<<<<<< HEAD
Shmem::SharedMemory* IToplevelProtocol::ToplevelState::LookupSharedMemory(
    Shmem::id_t aId) {
||||||| merged common ancestors
Shmem::SharedMemory*
IToplevelProtocol::ToplevelState::LookupSharedMemory(Shmem::id_t aId)
{
=======
Shmem::SharedMemory* IToplevelProtocol::LookupSharedMemory(Shmem::id_t aId) {
>>>>>>> upstream-releases
  return mShmemMap.Lookup(aId);
}

<<<<<<< HEAD
bool IToplevelProtocol::ToplevelState::IsTrackingSharedMemory(
    Shmem::SharedMemory* segment) {
||||||| merged common ancestors
bool
IToplevelProtocol::ToplevelState::IsTrackingSharedMemory(Shmem::SharedMemory* segment)
{
=======
bool IToplevelProtocol::IsTrackingSharedMemory(Shmem::SharedMemory* segment) {
>>>>>>> upstream-releases
  return mShmemMap.HasData(segment);
}

<<<<<<< HEAD
bool IToplevelProtocol::ToplevelState::DestroySharedMemory(Shmem& shmem) {
||||||| merged common ancestors
bool
IToplevelProtocol::ToplevelState::DestroySharedMemory(Shmem& shmem)
{
=======
bool IToplevelProtocol::DestroySharedMemory(Shmem& shmem) {
>>>>>>> upstream-releases
  Shmem::id_t aId = shmem.Id(Shmem::PrivateIPDLCaller());
  Shmem::SharedMemory* segment = LookupSharedMemory(aId);
  if (!segment) {
    return false;
  }

  Message* descriptor =
      shmem.UnshareFrom(Shmem::PrivateIPDLCaller(), MSG_ROUTING_CONTROL);

  mShmemMap.Remove(aId);
  Shmem::Dealloc(Shmem::PrivateIPDLCaller(), segment);

  MessageChannel* channel = GetIPCChannel();
  if (!channel->CanSend()) {
    delete descriptor;
    return true;
  }

  return descriptor && channel->Send(descriptor);
}

<<<<<<< HEAD
void IToplevelProtocol::ToplevelState::DeallocShmems() {
  for (IDMap<SharedMemory*>::const_iterator cit = mShmemMap.begin();
       cit != mShmemMap.end(); ++cit) {
||||||| merged common ancestors
void
IToplevelProtocol::ToplevelState::DeallocShmems()
{
  for (IDMap<SharedMemory*>::const_iterator cit = mShmemMap.begin(); cit != mShmemMap.end(); ++cit) {
=======
void IToplevelProtocol::DeallocShmems() {
  for (IDMap<SharedMemory*>::const_iterator cit = mShmemMap.begin();
       cit != mShmemMap.end(); ++cit) {
>>>>>>> upstream-releases
    Shmem::Dealloc(Shmem::PrivateIPDLCaller(), cit->second);
  }
  mShmemMap.Clear();
}

<<<<<<< HEAD
bool IToplevelProtocol::ToplevelState::ShmemCreated(const Message& aMsg) {
||||||| merged common ancestors
bool
IToplevelProtocol::ToplevelState::ShmemCreated(const Message& aMsg)
{
=======
bool IToplevelProtocol::ShmemCreated(const Message& aMsg) {
>>>>>>> upstream-releases
  Shmem::id_t id;
  RefPtr<Shmem::SharedMemory> rawmem(
      Shmem::OpenExisting(Shmem::PrivateIPDLCaller(), aMsg, &id, true));
  if (!rawmem) {
    return false;
  }
  mShmemMap.AddWithID(rawmem.forget().take(), id);
  return true;
}

<<<<<<< HEAD
bool IToplevelProtocol::ToplevelState::ShmemDestroyed(const Message& aMsg) {
||||||| merged common ancestors
bool
IToplevelProtocol::ToplevelState::ShmemDestroyed(const Message& aMsg)
{
=======
bool IToplevelProtocol::ShmemDestroyed(const Message& aMsg) {
>>>>>>> upstream-releases
  Shmem::id_t id;
  PickleIterator iter = PickleIterator(aMsg);
  if (!IPC::ReadParam(&aMsg, &iter, &id)) {
    return false;
  }
  aMsg.EndRead(iter);

  Shmem::SharedMemory* rawmem = LookupSharedMemory(id);
  if (rawmem) {
    mShmemMap.Remove(id);
    Shmem::Dealloc(Shmem::PrivateIPDLCaller(), rawmem);
  }
  return true;
}

<<<<<<< HEAD
already_AddRefed<nsIEventTarget>
IToplevelProtocol::ToplevelState::GetMessageEventTarget(const Message& aMsg) {
||||||| merged common ancestors
already_AddRefed<nsIEventTarget>
IToplevelProtocol::ToplevelState::GetMessageEventTarget(const Message& aMsg)
{
=======
already_AddRefed<nsIEventTarget> IToplevelProtocol::GetMessageEventTarget(
    const Message& aMsg) {
>>>>>>> upstream-releases
  int32_t route = aMsg.routing_id();

  Maybe<MutexAutoLock> lock;
  lock.emplace(mEventTargetMutex);

  nsCOMPtr<nsIEventTarget> target = mEventTargetMap.Lookup(route);

  if (aMsg.is_constructor()) {
    ActorHandle handle;
    PickleIterator iter = PickleIterator(aMsg);
    if (!IPC::ReadParam(&aMsg, &iter, &handle)) {
      return nullptr;
    }

    // Normally a new actor inherits its event target from its manager. If the
    // manager has no event target, we give the subclass a chance to make a new
    // one.
    if (!target) {
      MutexAutoUnlock unlock(mEventTargetMutex);
      target = GetConstructedEventTarget(aMsg);
    }

    mEventTargetMap.AddWithID(target, handle.mId);
  } else if (!target) {
    // We don't need the lock after this point.
    lock.reset();

    target = GetSpecificMessageEventTarget(aMsg);
  }

  return target.forget();
}

<<<<<<< HEAD
already_AddRefed<nsIEventTarget>
IToplevelProtocol::ToplevelState::GetActorEventTarget(IProtocol* aActor) {
  MOZ_RELEASE_ASSERT(aActor->Id() != kNullActorId &&
                     aActor->Id() != kFreedActorId);
||||||| merged common ancestors
already_AddRefed<nsIEventTarget>
IToplevelProtocol::ToplevelState::GetActorEventTarget(IProtocol* aActor)
{
  MOZ_RELEASE_ASSERT(aActor->Id() != kNullActorId && aActor->Id() != kFreedActorId);
=======
already_AddRefed<nsIEventTarget> IToplevelProtocol::GetActorEventTarget(
    IProtocol* aActor) {
  MOZ_RELEASE_ASSERT(aActor->Id() != kNullActorId &&
                     aActor->Id() != kFreedActorId);
>>>>>>> upstream-releases

  MutexAutoLock lock(mEventTargetMutex);
  nsCOMPtr<nsIEventTarget> target = mEventTargetMap.Lookup(aActor->Id());
  return target.forget();
}

<<<<<<< HEAD
nsIEventTarget* IToplevelProtocol::ToplevelState::GetActorEventTarget() {
||||||| merged common ancestors
nsIEventTarget*
IToplevelProtocol::ToplevelState::GetActorEventTarget()
{
=======
nsIEventTarget* IToplevelProtocol::GetActorEventTarget() {
>>>>>>> upstream-releases
  // The EventTarget of a ToplevelProtocol shall never be set.
  return nullptr;
}

<<<<<<< HEAD
void IToplevelProtocol::ToplevelState::SetEventTargetForActor(
    IProtocol* aActor, nsIEventTarget* aEventTarget) {
||||||| merged common ancestors
void
IToplevelProtocol::ToplevelState::SetEventTargetForActor(IProtocol* aActor,
                                                 nsIEventTarget* aEventTarget)
{
=======
void IToplevelProtocol::SetEventTargetForActorInternal(
    IProtocol* aActor, nsIEventTarget* aEventTarget) {
>>>>>>> upstream-releases
  // The EventTarget of a ToplevelProtocol shall never be set.
  MOZ_RELEASE_ASSERT(aActor != this);

  // We should only call this function on actors that haven't been used for IPC
  // code yet. Otherwise we'll be posting stuff to the wrong event target before
  // we're called.
<<<<<<< HEAD
  MOZ_RELEASE_ASSERT(aActor->Id() == kNullActorId ||
                     aActor->Id() == kFreedActorId);
||||||| merged common ancestors
  MOZ_RELEASE_ASSERT(aActor->Id() == kNullActorId || aActor->Id() == kFreedActorId);
=======
  MOZ_RELEASE_ASSERT(aActor->Id() == kNullActorId ||
                     aActor->Id() == kFreedActorId);

  MOZ_ASSERT(aActor->Manager() && aActor->ToplevelProtocol() == this);
>>>>>>> upstream-releases

  // Register the actor early. When it's registered again, it will keep the same
  // ID.
  int32_t id = Register(aActor);
  aActor->SetId(id);

  MutexAutoLock lock(mEventTargetMutex);
  // FIXME bug 1445121 - sometimes the id is already mapped.
  // (IDMap debug-asserts that the existing state is as expected.)
  bool replace = false;
#ifdef DEBUG
  replace = mEventTargetMap.Lookup(id) != nullptr;
#endif
  if (replace) {
    mEventTargetMap.ReplaceWithID(aEventTarget, id);
  } else {
    mEventTargetMap.AddWithID(aEventTarget, id);
  }
}

<<<<<<< HEAD
void IToplevelProtocol::ToplevelState::ReplaceEventTargetForActor(
    IProtocol* aActor, nsIEventTarget* aEventTarget) {
||||||| merged common ancestors
void
IToplevelProtocol::ToplevelState::ReplaceEventTargetForActor(
  IProtocol* aActor,
  nsIEventTarget* aEventTarget)
{
=======
void IToplevelProtocol::ReplaceEventTargetForActor(
    IProtocol* aActor, nsIEventTarget* aEventTarget) {
>>>>>>> upstream-releases
  // The EventTarget of a ToplevelProtocol shall never be set.
  MOZ_RELEASE_ASSERT(aActor != this);

  int32_t id = aActor->Id();
  // The ID of the actor should have existed.
  MOZ_RELEASE_ASSERT(id != kNullActorId && id != kFreedActorId);

  MutexAutoLock lock(mEventTargetMutex);
  mEventTargetMap.ReplaceWithID(aEventTarget, id);
}

<<<<<<< HEAD
const MessageChannel* IToplevelProtocol::ToplevelState::GetIPCChannel() const {
  return ProtocolState::mChannel ? ProtocolState::mChannel : &mChannel;
}
||||||| merged common ancestors
const MessageChannel*
IToplevelProtocol::ToplevelState::GetIPCChannel() const
{
  return ProtocolState::mChannel ? ProtocolState::mChannel : &mChannel;
}
=======
void IToplevelProtocol::SetEventTargetForRoute(int32_t aRoute,
                                               nsIEventTarget* aEventTarget) {
  MOZ_RELEASE_ASSERT(aRoute != Id());
  MOZ_RELEASE_ASSERT(aRoute != kNullActorId && aRoute != kFreedActorId);
>>>>>>> upstream-releases

<<<<<<< HEAD
MessageChannel* IToplevelProtocol::ToplevelState::GetIPCChannel() {
  return ProtocolState::mChannel ? ProtocolState::mChannel : &mChannel;
||||||| merged common ancestors
MessageChannel*
IToplevelProtocol::ToplevelState::GetIPCChannel()
{
  return ProtocolState::mChannel ? ProtocolState::mChannel : &mChannel;
=======
  MutexAutoLock lock(mEventTargetMutex);
  MOZ_ASSERT(!mEventTargetMap.Lookup(aRoute));
  mEventTargetMap.AddWithID(aEventTarget, aRoute);
>>>>>>> upstream-releases
}

}  // namespace ipc
}  // namespace mozilla
