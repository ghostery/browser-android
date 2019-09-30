/* -*- Mode: C++; tab-width: 8; indent-tabs-mode: nil; c-basic-offset: 2 -*- */
/* vim: set ts=8 sts=2 et sw=2 tw=80: */
/* This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at http://mozilla.org/MPL/2.0/. */

#include "ParentInternal.h"

#include "base/task.h"
#include "mozilla/dom/ContentChild.h"
#include "Thread.h"

namespace mozilla {
namespace recordreplay {
namespace parent {

// A saved introduction message for sending to all children.
static IntroductionMessage* gIntroductionMessage;

// How many channels have been constructed so far.
static size_t gNumChannels;

// Whether children might be debugged and should not be treated as hung.
static bool gChildrenAreDebugging;

<<<<<<< HEAD
/* static */ void ChildProcessInfo::SetIntroductionMessage(
    IntroductionMessage* aMessage) {
||||||| merged common ancestors
/* static */ void
ChildProcessInfo::SetIntroductionMessage(IntroductionMessage* aMessage)
{
=======
/* static */
void ChildProcessInfo::SetIntroductionMessage(IntroductionMessage* aMessage) {
>>>>>>> upstream-releases
  gIntroductionMessage = aMessage;
}

<<<<<<< HEAD
ChildProcessInfo::ChildProcessInfo(
    UniquePtr<ChildRole> aRole,
    const Maybe<RecordingProcessData>& aRecordingProcessData)
    : mChannel(nullptr),
      mRecording(aRecordingProcessData.isSome()),
      mRecoveryStage(RecoveryStage::None),
      mPaused(false),
      mPausedMessage(nullptr),
      mLastCheckpoint(CheckpointId::Invalid),
      mNumRecoveredMessages(0),
      mRole(std::move(aRole)),
      mPauseNeeded(false),
      mHasBegunFatalError(false),
      mHasFatalError(false) {
||||||| merged common ancestors
ChildProcessInfo::ChildProcessInfo(UniquePtr<ChildRole> aRole,
                                   const Maybe<RecordingProcessData>& aRecordingProcessData)
  : mChannel(nullptr)
  , mRecording(aRecordingProcessData.isSome())
  , mRecoveryStage(RecoveryStage::None)
  , mPaused(false)
  , mPausedMessage(nullptr)
  , mLastCheckpoint(CheckpointId::Invalid)
  , mNumRecoveredMessages(0)
  , mRole(std::move(aRole))
  , mPauseNeeded(false)
{
=======
ChildProcessInfo::ChildProcessInfo(
    const Maybe<RecordingProcessData>& aRecordingProcessData)
    : mChannel(nullptr),
      mRecording(aRecordingProcessData.isSome()),
      mPaused(false),
      mHasBegunFatalError(false),
      mHasFatalError(false) {
>>>>>>> upstream-releases
  MOZ_RELEASE_ASSERT(NS_IsMainThread());

  static bool gFirst = false;
  if (!gFirst) {
    gFirst = true;
    gChildrenAreDebugging = !!getenv("WAIT_AT_START");
  }

  LaunchSubprocess(aRecordingProcessData);
}

ChildProcessInfo::~ChildProcessInfo() {
  MOZ_RELEASE_ASSERT(NS_IsMainThread());
  if (IsRecording()) {
    SendMessage(TerminateMessage());
  }
}

<<<<<<< HEAD
ChildProcessInfo::Disposition ChildProcessInfo::GetDisposition() {
  // We can determine the disposition of the child by looking at the first
  // resume message sent since the last time it reached a checkpoint.
  for (Message* msg : mMessages) {
    if (msg->mType == MessageType::Resume) {
      const ResumeMessage& nmsg = static_cast<const ResumeMessage&>(*msg);
      return nmsg.mForward ? AfterLastCheckpoint : BeforeLastCheckpoint;
    }
  }
  return AtLastCheckpoint;
}

bool ChildProcessInfo::IsPausedAtCheckpoint() {
  return IsPaused() && mPausedMessage->mType == MessageType::HitCheckpoint;
}

bool ChildProcessInfo::IsPausedAtRecordingEndpoint() {
  if (!IsPaused()) {
    return false;
  }
  if (mPausedMessage->mType == MessageType::HitCheckpoint) {
    return static_cast<HitCheckpointMessage*>(mPausedMessage)
        ->mRecordingEndpoint;
  }
  if (mPausedMessage->mType == MessageType::HitBreakpoint) {
    return static_cast<HitBreakpointMessage*>(mPausedMessage)
        ->mRecordingEndpoint;
  }
  return false;
}

void ChildProcessInfo::GetInstalledBreakpoints(
    InfallibleVector<AddBreakpointMessage*>& aBreakpoints) {
  MOZ_RELEASE_ASSERT(aBreakpoints.empty());
  for (Message* msg : mMessages) {
    if (msg->mType == MessageType::AddBreakpoint) {
      aBreakpoints.append(static_cast<AddBreakpointMessage*>(msg));
    } else if (msg->mType == MessageType::ClearBreakpoints) {
      aBreakpoints.clear();
    }
  }
}

void ChildProcessInfo::AddMajorCheckpoint(size_t aId) {
  // Major checkpoints should be listed in order.
  MOZ_RELEASE_ASSERT(mMajorCheckpoints.empty() ||
                     aId > mMajorCheckpoints.back());
  mMajorCheckpoints.append(aId);
}

void ChildProcessInfo::SetRole(UniquePtr<ChildRole> aRole) {
  MOZ_RELEASE_ASSERT(!IsRecovering());

  PrintSpew("SetRole:%d %s\n", (int)GetId(),
            ChildRole::TypeString(aRole->GetType()));

  mRole = std::move(aRole);
  mRole->SetProcess(this);
  mRole->Initialize();
}

void ChildProcessInfo::OnIncomingMessage(size_t aChannelId,
                                         const Message& aMsg) {
||||||| merged common ancestors
ChildProcessInfo::Disposition
ChildProcessInfo::GetDisposition()
{
  // We can determine the disposition of the child by looking at the first
  // resume message sent since the last time it reached a checkpoint.
  for (Message* msg : mMessages) {
    if (msg->mType == MessageType::Resume) {
      const ResumeMessage& nmsg = static_cast<const ResumeMessage&>(*msg);
      return nmsg.mForward ? AfterLastCheckpoint : BeforeLastCheckpoint;
    }
  }
  return AtLastCheckpoint;
}

bool
ChildProcessInfo::IsPausedAtCheckpoint()
{
  return IsPaused() && mPausedMessage->mType == MessageType::HitCheckpoint;
}

bool
ChildProcessInfo::IsPausedAtRecordingEndpoint()
{
  if (!IsPaused()) {
    return false;
  }
  if (mPausedMessage->mType == MessageType::HitCheckpoint) {
    return static_cast<HitCheckpointMessage*>(mPausedMessage)->mRecordingEndpoint;
  }
  if (mPausedMessage->mType == MessageType::HitBreakpoint) {
    return static_cast<HitBreakpointMessage*>(mPausedMessage)->mRecordingEndpoint;
  }
  return false;
}

void
ChildProcessInfo::GetInstalledBreakpoints(Vector<SetBreakpointMessage*>& aBreakpoints)
{
  for (Message* msg : mMessages) {
    if (msg->mType == MessageType::SetBreakpoint) {
      SetBreakpointMessage* nmsg = static_cast<SetBreakpointMessage*>(msg);
      for (SetBreakpointMessage*& existing : aBreakpoints) {
        if (existing->mId == nmsg->mId) {
          aBreakpoints.erase(&existing);
          break;
        }
      }
      if (nmsg->mPosition.IsValid()) {
        if (!aBreakpoints.append(nmsg)) {
          MOZ_CRASH("OOM");
        }
      }
    }
  }
}

bool
ChildProcessInfo::IsPausedAtMatchingBreakpoint(const BreakpointFilter& aFilter)
{
  if (!IsPaused() || mPausedMessage->mType != MessageType::HitBreakpoint) {
    return false;
  }

  Vector<SetBreakpointMessage*> installed;
  GetInstalledBreakpoints(installed);

  HitBreakpointMessage* npaused = static_cast<HitBreakpointMessage*>(mPausedMessage);
  for (size_t i = 0; i < npaused->NumBreakpoints(); i++) {
    uint32_t breakpointId = npaused->Breakpoints()[i];

    // Note: this test isn't quite right if new breakpoints have been installed
    // since the child paused, though this does not affect current callers.
    for (SetBreakpointMessage* msg : installed) {
      if (msg->mId == breakpointId && aFilter(msg->mPosition.mKind)) {
        return true;
      }
    }
  }

  return false;
}

void
ChildProcessInfo::GetMatchingInstalledBreakpoints(const BreakpointFilter& aFilter,
                                                  Vector<uint32_t>& aBreakpointIds)
{
  Vector<SetBreakpointMessage*> installed;
  GetInstalledBreakpoints(installed);

  for (SetBreakpointMessage* msg : installed) {
    if (aFilter(msg->mPosition.mKind) && !aBreakpointIds.append(msg->mId)) {
      MOZ_CRASH("OOM");
    }
  }
}

void
ChildProcessInfo::AddMajorCheckpoint(size_t aId)
{
  // Major checkpoints should be listed in order.
  MOZ_RELEASE_ASSERT(mMajorCheckpoints.empty() || aId > mMajorCheckpoints.back());
  mMajorCheckpoints.append(aId);
}

void
ChildProcessInfo::SetRole(UniquePtr<ChildRole> aRole)
{
  MOZ_RELEASE_ASSERT(!IsRecovering());

  PrintSpew("SetRole:%d %s\n", (int) GetId(), ChildRole::TypeString(aRole->GetType()));

  mRole = std::move(aRole);
  mRole->SetProcess(this);
  mRole->Initialize();
}

void
ChildProcessInfo::OnIncomingMessage(size_t aChannelId, const Message& aMsg)
{
=======
void ChildProcessInfo::OnIncomingMessage(const Message& aMsg) {
>>>>>>> upstream-releases
  MOZ_RELEASE_ASSERT(NS_IsMainThread());
<<<<<<< HEAD

  // Ignore messages from channels for subprocesses we terminated already.
  if (aChannelId != mChannel->GetId()) {
    return;
  }

  // Always handle fatal errors in the same way.
  if (aMsg.mType == MessageType::BeginFatalError) {
    mHasBegunFatalError = true;
    return;
  } else if (aMsg.mType == MessageType::FatalError) {
    mHasFatalError = true;
    const FatalErrorMessage& nmsg = static_cast<const FatalErrorMessage&>(aMsg);
    OnCrash(nmsg.Error());
    return;
  }

||||||| merged common ancestors

  // Ignore messages from channels for subprocesses we terminated already.
  if (aChannelId != mChannel->GetId()) {
    return;
  }

  // Always handle fatal errors in the same way.
  if (aMsg.mType == MessageType::FatalError) {
    const FatalErrorMessage& nmsg = static_cast<const FatalErrorMessage&>(aMsg);
    OnCrash(nmsg.Error());
    return;
  }

=======
>>>>>>> upstream-releases
  mLastMessageTime = TimeStamp::Now();

  switch (aMsg.mType) {
<<<<<<< HEAD
    case MessageType::HitCheckpoint:
    case MessageType::HitBreakpoint:
      MOZ_RELEASE_ASSERT(!mPausedMessage);
      mPausedMessage = aMsg.Clone();
      MOZ_FALLTHROUGH;
    case MessageType::DebuggerResponse:
    case MessageType::RecordingFlushed:
      MOZ_RELEASE_ASSERT(mPausedMessage);
      mPaused = true;
      break;
    default:
      break;
  }

  if (aMsg.mType == MessageType::HitCheckpoint) {
    const HitCheckpointMessage& nmsg =
        static_cast<const HitCheckpointMessage&>(aMsg);
    mLastCheckpoint = nmsg.mCheckpointId;

    // All messages sent since the last checkpoint are now obsolete, except
    // those which establish the set of installed breakpoints.
    InfallibleVector<Message*> newMessages;
    for (Message* msg : mMessages) {
      if (msg->mType == MessageType::AddBreakpoint) {
        newMessages.append(msg);
      } else {
        if (msg->mType == MessageType::ClearBreakpoints) {
          for (Message* existing : newMessages) {
            free(existing);
          }
          newMessages.clear();
        }
        free(msg);
      }
||||||| merged common ancestors
  case MessageType::HitCheckpoint:
  case MessageType::HitBreakpoint:
    MOZ_RELEASE_ASSERT(!mPausedMessage);
    mPausedMessage = aMsg.Clone();
    MOZ_FALLTHROUGH;
  case MessageType::DebuggerResponse:
  case MessageType::RecordingFlushed:
    MOZ_RELEASE_ASSERT(mPausedMessage);
    mPaused = true;
    break;
  default:
    break;
  }

  if (aMsg.mType == MessageType::HitCheckpoint) {
    const HitCheckpointMessage& nmsg = static_cast<const HitCheckpointMessage&>(aMsg);
    mLastCheckpoint = nmsg.mCheckpointId;

    // All messages sent since the last checkpoint are now obsolete, except
    // SetBreakpoint messages.
    InfallibleVector<Message*> newMessages;
    for (Message* msg : mMessages) {
      if (msg->mType == MessageType::SetBreakpoint) {
        // Look for an older SetBreakpoint on the same ID to overwrite.
        bool found = false;
        for (Message*& older : newMessages) {
          if (static_cast<SetBreakpointMessage*>(msg)->mId ==
              static_cast<SetBreakpointMessage*>(older)->mId) {
            free(older);
            older = msg;
            found = true;
          }
        }
        if (!found) {
          newMessages.emplaceBack(msg);
        }
      } else {
        free(msg);
      }
=======
    case MessageType::BeginFatalError:
      mHasBegunFatalError = true;
      return;
    case MessageType::FatalError: {
      mHasFatalError = true;
      const FatalErrorMessage& nmsg =
          static_cast<const FatalErrorMessage&>(aMsg);
      OnCrash(nmsg.Error());
      return;
>>>>>>> upstream-releases
    }
    case MessageType::Paint:
      UpdateGraphicsInUIProcess(&static_cast<const PaintMessage&>(aMsg));
      break;
    case MessageType::ManifestFinished:
      mPaused = true;
      js::ForwardManifestFinished(this, aMsg);
      break;
    case MessageType::MiddlemanCallRequest: {
      const MiddlemanCallRequestMessage& nmsg =
          static_cast<const MiddlemanCallRequestMessage&>(aMsg);
      InfallibleVector<char> outputData;
      ProcessMiddlemanCall(GetId(), nmsg.BinaryData(), nmsg.BinaryDataSize(),
                           &outputData);
      Message::UniquePtr response(MiddlemanCallResponseMessage::New(
          outputData.begin(), outputData.length()));
      SendMessage(std::move(*response));
      break;
    }
    case MessageType::ResetMiddlemanCalls:
      ResetMiddlemanCalls(GetId());
      break;
    default:
      break;
  }
}

<<<<<<< HEAD
void ChildProcessInfo::SendMessage(const Message& aMsg) {
  MOZ_RELEASE_ASSERT(!IsRecovering());
||||||| merged common ancestors
void
ChildProcessInfo::SendMessage(const Message& aMsg)
{
  MOZ_RELEASE_ASSERT(!IsRecovering());
=======
void ChildProcessInfo::SendMessage(Message&& aMsg) {
>>>>>>> upstream-releases
  MOZ_RELEASE_ASSERT(NS_IsMainThread());

  // Update paused state.
  MOZ_RELEASE_ASSERT(IsPaused() || aMsg.CanBeSentWhileUnpaused());
<<<<<<< HEAD
  switch (aMsg.mType) {
    case MessageType::Resume:
    case MessageType::RestoreCheckpoint:
    case MessageType::RunToPoint:
      free(mPausedMessage);
      mPausedMessage = nullptr;
      MOZ_FALLTHROUGH;
    case MessageType::DebuggerRequest:
    case MessageType::FlushRecording:
      mPaused = false;
      break;
    default:
      break;
  }

  // Keep track of messages which affect the child's behavior.
  switch (aMsg.mType) {
    case MessageType::Resume:
    case MessageType::RestoreCheckpoint:
    case MessageType::RunToPoint:
    case MessageType::DebuggerRequest:
    case MessageType::AddBreakpoint:
    case MessageType::ClearBreakpoints:
      mMessages.emplaceBack(aMsg.Clone());
      break;
    default:
      break;
  }

  // Keep track of the checkpoints the process will save.
  if (aMsg.mType == MessageType::SetSaveCheckpoint) {
    const SetSaveCheckpointMessage& nmsg =
        static_cast<const SetSaveCheckpointMessage&>(aMsg);
    MOZ_RELEASE_ASSERT(nmsg.mCheckpoint > MostRecentCheckpoint());
    VectorAddOrRemoveEntry(mShouldSaveCheckpoints, nmsg.mCheckpoint,
                           nmsg.mSave);
||||||| merged common ancestors
  switch (aMsg.mType) {
  case MessageType::Resume:
  case MessageType::RestoreCheckpoint:
  case MessageType::RunToPoint:
    free(mPausedMessage);
    mPausedMessage = nullptr;
    MOZ_FALLTHROUGH;
  case MessageType::DebuggerRequest:
  case MessageType::FlushRecording:
    mPaused = false;
    break;
  default:
    break;
  }

  // Keep track of messages which affect the child's behavior.
  switch (aMsg.mType) {
  case MessageType::Resume:
  case MessageType::RestoreCheckpoint:
  case MessageType::RunToPoint:
  case MessageType::DebuggerRequest:
  case MessageType::SetBreakpoint:
  case MessageType::MiddlemanCallResponse:
    mMessages.emplaceBack(aMsg.Clone());
    break;
  default:
    break;
  }

  // Keep track of the checkpoints the process will save.
  if (aMsg.mType == MessageType::SetSaveCheckpoint) {
    const SetSaveCheckpointMessage& nmsg = static_cast<const SetSaveCheckpointMessage&>(aMsg);
    MOZ_RELEASE_ASSERT(nmsg.mCheckpoint > MostRecentCheckpoint());
    VectorAddOrRemoveEntry(mShouldSaveCheckpoints, nmsg.mCheckpoint, nmsg.mSave);
=======
  if (aMsg.mType == MessageType::ManifestStart) {
    mPaused = false;
>>>>>>> upstream-releases
  }

<<<<<<< HEAD
  SendMessageRaw(aMsg);
}

void ChildProcessInfo::SendMessageRaw(const Message& aMsg) {
  MOZ_RELEASE_ASSERT(NS_IsMainThread());
||||||| merged common ancestors
  SendMessageRaw(aMsg);
}

void
ChildProcessInfo::SendMessageRaw(const Message& aMsg)
{
  MOZ_RELEASE_ASSERT(NS_IsMainThread());
=======
>>>>>>> upstream-releases
  mLastMessageTime = TimeStamp::Now();
<<<<<<< HEAD
  mChannel->SendMessage(aMsg);
}

void ChildProcessInfo::Recover(bool aPaused, Message* aPausedMessage,
                               size_t aLastCheckpoint, Message** aMessages,
                               size_t aNumMessages) {
  MOZ_RELEASE_ASSERT(IsPaused());

  SendMessageRaw(SetIsActiveMessage(false));

  size_t mostRecentCheckpoint = MostRecentCheckpoint();
  bool pausedAtCheckpoint = IsPausedAtCheckpoint();

  // Clear out all messages that have been sent to this process.
  for (Message* msg : mMessages) {
    free(msg);
  }
  mMessages.clear();
  SendMessageRaw(ClearBreakpointsMessage());

  mPaused = aPaused;
  mPausedMessage = aPausedMessage;
  mLastCheckpoint = aLastCheckpoint;
  for (size_t i = 0; i < aNumMessages; i++) {
    mMessages.append(aMessages[i]->Clone());
  }

  mNumRecoveredMessages = 0;

  if (mostRecentCheckpoint < mLastCheckpoint) {
    mRecoveryStage = RecoveryStage::ReachingCheckpoint;
    SendMessageRaw(ResumeMessage(/* aForward = */ true));
  } else if (mostRecentCheckpoint > mLastCheckpoint || !pausedAtCheckpoint) {
    mRecoveryStage = RecoveryStage::ReachingCheckpoint;
    // Rewind to the last saved checkpoint at or prior to the target.
    size_t targetCheckpoint = CheckpointId::Invalid;
    for (size_t saved : mShouldSaveCheckpoints) {
      if (saved <= mLastCheckpoint && saved > targetCheckpoint) {
        targetCheckpoint = saved;
      }
    }
    MOZ_RELEASE_ASSERT(targetCheckpoint != CheckpointId::Invalid);
    SendMessageRaw(RestoreCheckpointMessage(targetCheckpoint));
  } else {
    mRecoveryStage = RecoveryStage::PlayingMessages;
    SendNextRecoveryMessage();
  }

  WaitUntil([=]() { return !IsRecovering(); });
}

void ChildProcessInfo::Recover(ChildProcessInfo* aTargetProcess) {
  MOZ_RELEASE_ASSERT(aTargetProcess->IsPaused());
  Recover(true, aTargetProcess->mPausedMessage->Clone(),
          aTargetProcess->mLastCheckpoint, aTargetProcess->mMessages.begin(),
          aTargetProcess->mMessages.length());
}

void ChildProcessInfo::RecoverToCheckpoint(size_t aCheckpoint) {
  HitCheckpointMessage pausedMessage(aCheckpoint,
                                     /* aRecordingEndpoint = */ false,
                                     /* aDuration = */ 0);
  Recover(true, pausedMessage.Clone(), aCheckpoint, nullptr, 0);
}

void ChildProcessInfo::OnIncomingRecoveryMessage(const Message& aMsg) {
  switch (aMsg.mType) {
    case MessageType::HitCheckpoint: {
      MOZ_RELEASE_ASSERT(mRecoveryStage == RecoveryStage::ReachingCheckpoint);
      const HitCheckpointMessage& nmsg =
          static_cast<const HitCheckpointMessage&>(aMsg);
      if (nmsg.mCheckpointId < mLastCheckpoint) {
        SendMessageRaw(ResumeMessage(/* aForward = */ true));
      } else {
        MOZ_RELEASE_ASSERT(nmsg.mCheckpointId == mLastCheckpoint);
        mRecoveryStage = RecoveryStage::PlayingMessages;
        SendNextRecoveryMessage();
      }
      break;
    }
    case MessageType::HitBreakpoint:
    case MessageType::DebuggerResponse:
      SendNextRecoveryMessage();
      break;
    case MessageType::MiddlemanCallRequest: {
      // Middleman call messages can arrive in different orders when recovering
      // than they originally did in the original process, so handle them afresh
      // even when recovering.
      MiddlemanCallResponseMessage* response =
          ProcessMiddlemanCallMessage((MiddlemanCallRequestMessage&)aMsg);
      SendMessageRaw(*response);
      free(response);
      break;
    }
    case MessageType::ResetMiddlemanCalls:
      ResetMiddlemanCalls();
      break;
    default:
      MOZ_CRASH("Unexpected message during recovery");
  }
}

void ChildProcessInfo::SendNextRecoveryMessage() {
  MOZ_RELEASE_ASSERT(mRecoveryStage == RecoveryStage::PlayingMessages);

  // Keep sending messages to the child as long as it stays paused.
  Message* msg;
  do {
    // Check if we have recovered to the desired paused state.
    if (mNumRecoveredMessages == mMessages.length()) {
      MOZ_RELEASE_ASSERT(IsPaused());
      mRecoveryStage = RecoveryStage::None;
      return;
    }
    msg = mMessages[mNumRecoveredMessages++];
    SendMessageRaw(*msg);

    // Messages operating on breakpoints preserve the paused state of the
    // child, so keep sending more messages.
  } while (msg->mType == MessageType::AddBreakpoint ||
           msg->mType == MessageType::ClearBreakpoints);

  // If we have sent all messages and are in an unpaused state, we are done
  // recovering.
  if (mNumRecoveredMessages == mMessages.length() && !IsPaused()) {
    mRecoveryStage = RecoveryStage::None;
  }
||||||| merged common ancestors
  mChannel->SendMessage(aMsg);
}

void
ChildProcessInfo::Recover(bool aPaused, Message* aPausedMessage, size_t aLastCheckpoint,
                          Message** aMessages, size_t aNumMessages)
{
  MOZ_RELEASE_ASSERT(IsPaused());

  SendMessageRaw(SetIsActiveMessage(false));

  size_t mostRecentCheckpoint = MostRecentCheckpoint();
  bool pausedAtCheckpoint = IsPausedAtCheckpoint();

  // Clear out all messages that have been sent to this process.
  for (Message* msg : mMessages) {
    if (msg->mType == MessageType::SetBreakpoint) {
      SetBreakpointMessage* nmsg = static_cast<SetBreakpointMessage*>(msg);
      SendMessageRaw(SetBreakpointMessage(nmsg->mId, js::BreakpointPosition()));
    }
    free(msg);
  }
  mMessages.clear();

  mPaused = aPaused;
  mPausedMessage = aPausedMessage;
  mLastCheckpoint = aLastCheckpoint;
  for (size_t i = 0; i < aNumMessages; i++) {
    mMessages.append(aMessages[i]->Clone());
  }

  mNumRecoveredMessages = 0;

  if (mostRecentCheckpoint < mLastCheckpoint) {
    mRecoveryStage = RecoveryStage::ReachingCheckpoint;
    SendMessageRaw(ResumeMessage(/* aForward = */ true));
  } else if (mostRecentCheckpoint > mLastCheckpoint || !pausedAtCheckpoint) {
    mRecoveryStage = RecoveryStage::ReachingCheckpoint;
    // Rewind to the last saved checkpoint at or prior to the target.
    size_t targetCheckpoint = CheckpointId::Invalid;
    for (size_t saved : mShouldSaveCheckpoints) {
      if (saved <= mLastCheckpoint && saved > targetCheckpoint) {
        targetCheckpoint = saved;
      }
    }
    MOZ_RELEASE_ASSERT(targetCheckpoint != CheckpointId::Invalid);
    SendMessageRaw(RestoreCheckpointMessage(targetCheckpoint));
  } else {
    mRecoveryStage = RecoveryStage::PlayingMessages;
    SendNextRecoveryMessage();
  }

  WaitUntil([=]() { return !IsRecovering(); });
}

void
ChildProcessInfo::Recover(ChildProcessInfo* aTargetProcess)
{
  MOZ_RELEASE_ASSERT(aTargetProcess->IsPaused());
  Recover(true, aTargetProcess->mPausedMessage->Clone(),
          aTargetProcess->mLastCheckpoint,
          aTargetProcess->mMessages.begin(), aTargetProcess->mMessages.length());
}

void
ChildProcessInfo::RecoverToCheckpoint(size_t aCheckpoint)
{
  HitCheckpointMessage pausedMessage(aCheckpoint,
                                     /* aRecordingEndpoint = */ false,
                                     /* aDuration = */ 0);
  Recover(true, pausedMessage.Clone(), aCheckpoint, nullptr, 0);
}

void
ChildProcessInfo::OnIncomingRecoveryMessage(const Message& aMsg)
{
  switch (aMsg.mType) {
  case MessageType::HitCheckpoint: {
    MOZ_RELEASE_ASSERT(mRecoveryStage == RecoveryStage::ReachingCheckpoint);
    const HitCheckpointMessage& nmsg = static_cast<const HitCheckpointMessage&>(aMsg);
    if (nmsg.mCheckpointId < mLastCheckpoint) {
      SendMessageRaw(ResumeMessage(/* aForward = */ true));
    } else {
      MOZ_RELEASE_ASSERT(nmsg.mCheckpointId == mLastCheckpoint);
      mRecoveryStage = RecoveryStage::PlayingMessages;
      SendNextRecoveryMessage();
    }
    break;
  }
  case MessageType::HitBreakpoint:
  case MessageType::DebuggerResponse:
  case MessageType::MiddlemanCallRequest:
    SendNextRecoveryMessage();
    break;
  case MessageType::ResetMiddlemanCalls:
    break;
  default:
    MOZ_CRASH("Unexpected message during recovery");
  }
}

void
ChildProcessInfo::SendNextRecoveryMessage()
{
  MOZ_RELEASE_ASSERT(mRecoveryStage == RecoveryStage::PlayingMessages);

  // Keep sending messages to the child as long as it stays paused.
  Message* msg;
  do {
    // Check if we have recovered to the desired paused state.
    if (mNumRecoveredMessages == mMessages.length()) {
      MOZ_RELEASE_ASSERT(IsPaused());
      mRecoveryStage = RecoveryStage::None;
      return;
    }
    msg = mMessages[mNumRecoveredMessages++];
    SendMessageRaw(*msg);

    // If we just sent a SetBreakpoint message then the child process is still
    // paused, so keep sending more messages.
  } while (msg->mType == MessageType::SetBreakpoint);

  // If we have sent all messages and are in an unpaused state, we are done
  // recovering.
  if (mNumRecoveredMessages == mMessages.length() && !IsPaused()) {
    mRecoveryStage = RecoveryStage::None;
  }
=======
  mChannel->SendMessage(std::move(aMsg));
>>>>>>> upstream-releases
}

///////////////////////////////////////////////////////////////////////////////
// Subprocess Management
///////////////////////////////////////////////////////////////////////////////

ipc::GeckoChildProcessHost* gRecordingProcess;

void GetArgumentsForChildProcess(base::ProcessId aMiddlemanPid,
                                 uint32_t aChannelId,
                                 const char* aRecordingFile, bool aRecording,
                                 std::vector<std::string>& aExtraArgs) {
  MOZ_RELEASE_ASSERT(IsMiddleman() || XRE_IsParentProcess());

  aExtraArgs.push_back(gMiddlemanPidOption);
  aExtraArgs.push_back(nsPrintfCString("%d", aMiddlemanPid).get());

  aExtraArgs.push_back(gChannelIDOption);
  aExtraArgs.push_back(nsPrintfCString("%d", (int)aChannelId).get());

  aExtraArgs.push_back(gProcessKindOption);
  aExtraArgs.push_back(nsPrintfCString("%d", aRecording
                                                 ? (int)ProcessKind::Recording
                                                 : (int)ProcessKind::Replaying)
                           .get());

  aExtraArgs.push_back(gRecordingFileOption);
  aExtraArgs.push_back(aRecordingFile);
}

void ChildProcessInfo::LaunchSubprocess(
    const Maybe<RecordingProcessData>& aRecordingProcessData) {
  size_t channelId = gNumChannels++;

  // Create a new channel every time we launch a new subprocess, without
  // deleting or tearing down the old one's state. This is pretty lame and it
  // would be nice if we could do something better here, especially because
  // with restarts we could create any number of channels over time.
<<<<<<< HEAD
  mChannel = new Channel(channelId, IsRecording(), [=](Message* aMsg) {
    ReceiveChildMessageOnMainThread(channelId, aMsg);
  });
||||||| merged common ancestors
  mChannel = new Channel(channelId, IsRecording(), [=](Message* aMsg) {
      ReceiveChildMessageOnMainThread(channelId, aMsg);
    });
=======
  mChannel =
      new Channel(channelId, IsRecording(), [=](Message::UniquePtr aMsg) {
        ReceiveChildMessageOnMainThread(std::move(aMsg));
      });
>>>>>>> upstream-releases

  MOZ_RELEASE_ASSERT(IsRecording() == aRecordingProcessData.isSome());
  if (IsRecording()) {
    std::vector<std::string> extraArgs;
    GetArgumentsForChildProcess(base::GetCurrentProcId(), channelId,
                                gRecordingFilename, /* aRecording = */ true,
                                extraArgs);

    MOZ_RELEASE_ASSERT(!gRecordingProcess);
    gRecordingProcess =
        new ipc::GeckoChildProcessHost(GeckoProcessType_Content);

    // Preferences data is conveyed to the recording process via fixed file
    // descriptors on macOS.
    gRecordingProcess->AddFdToRemap(aRecordingProcessData.ref().mPrefsHandle.fd,
                                    kPrefsFileDescriptor);
    ipc::FileDescriptor::UniquePlatformHandle prefMapHandle =
        aRecordingProcessData.ref().mPrefMapHandle.ClonePlatformHandle();
    gRecordingProcess->AddFdToRemap(prefMapHandle.get(),
                                    kPrefMapFileDescriptor);

    if (!gRecordingProcess->LaunchAndWaitForProcessHandle(extraArgs)) {
      MOZ_CRASH("ChildProcessInfo::LaunchSubprocess");
    }
  } else {
    dom::ContentChild::GetSingleton()->SendCreateReplayingProcess(channelId);
  }

  mLastMessageTime = TimeStamp::Now();

  SendGraphicsMemoryToChild();

  MOZ_RELEASE_ASSERT(gIntroductionMessage);
  SendMessage(std::move(*gIntroductionMessage));
}

void ChildProcessInfo::OnCrash(const char* aWhy) {
  MOZ_RELEASE_ASSERT(NS_IsMainThread());

  // If a child process crashes or hangs then annotate the crash report.
  CrashReporter::AnnotateCrashReport(
      CrashReporter::Annotation::RecordReplayError, nsAutoCString(aWhy));

  // If we received a FatalError message then the child generated a minidump.
  // Shut down cleanly so that we don't mask the report with our own crash.
  if (mHasFatalError) {
    Shutdown();
  }

  // Indicate when we crash if the child tried to send us a fatal error message
  // but had a problem either unprotecting system memory or generating the
  // minidump.
  MOZ_RELEASE_ASSERT(!mHasBegunFatalError);

  // The child crashed without producing a minidump, produce one ourselves.
  MOZ_CRASH("Unexpected child crash");
}

///////////////////////////////////////////////////////////////////////////////
// Handling Channel Messages
///////////////////////////////////////////////////////////////////////////////

// When messages are received from child processes, we want their handler to
// execute on the main thread. The main thread might be blocked in WaitUntil,
// so runnables associated with child processes have special handling.

// All messages received on a channel thread which the main thread has not
// processed yet. This is protected by gMonitor.
struct PendingMessage {
  ChildProcessInfo* mProcess;
  Message::UniquePtr mMsg;

  PendingMessage() : mProcess(nullptr) {}

  PendingMessage& operator=(PendingMessage&& aOther) {
    mProcess = aOther.mProcess;
    mMsg = std::move(aOther.mMsg);
    return *this;
  }

  PendingMessage(PendingMessage&& aOther) { *this = std::move(aOther); }
};
static StaticInfallibleVector<PendingMessage> gPendingMessages;

<<<<<<< HEAD
// Whether there is a pending task on the main thread's message loop to handle
// all pending messages.
static bool gHasPendingMessageRunnable;

// Process a pending message from aProcess (or any process if aProcess is null)
// and return whether such a message was found. This must be called on the main
// thread with gMonitor held.
/* static */ bool ChildProcessInfo::MaybeProcessPendingMessage(
    ChildProcessInfo* aProcess) {
||||||| merged common ancestors
// Whether there is a pending task on the main thread's message loop to handle
// all pending messages.
static bool gHasPendingMessageRunnable;

// Process a pending message from aProcess (or any process if aProcess is null)
// and return whether such a message was found. This must be called on the main
// thread with gMonitor held.
/* static */ bool
ChildProcessInfo::MaybeProcessPendingMessage(ChildProcessInfo* aProcess)
{
=======
static Message::UniquePtr ExtractChildMessage(ChildProcessInfo** aProcess) {
>>>>>>> upstream-releases
  MOZ_RELEASE_ASSERT(NS_IsMainThread());

  for (size_t i = 0; i < gPendingMessages.length(); i++) {
    PendingMessage& pending = gPendingMessages[i];
    if (!*aProcess || pending.mProcess == *aProcess) {
      *aProcess = pending.mProcess;
      Message::UniquePtr msg = std::move(pending.mMsg);
      gPendingMessages.erase(&pending);
      return msg;
    }
  }

  return nullptr;
}

// Whether there is a pending task on the main thread's message loop to handle
// all pending messages.
static bool gHasPendingMessageRunnable;

// How many seconds to wait without hearing from an unpaused child before
// considering that child to be hung.
static const size_t HangSeconds = 30;

<<<<<<< HEAD
void ChildProcessInfo::WaitUntil(const std::function<bool()>& aCallback) {
||||||| merged common ancestors
void
ChildProcessInfo::WaitUntil(const std::function<bool()>& aCallback)
{
=======
void ChildProcessInfo::WaitUntilPaused() {
>>>>>>> upstream-releases
  MOZ_RELEASE_ASSERT(NS_IsMainThread());

  if (IsPaused()) {
    return;
  }

  bool sentTerminateMessage = false;
  while (true) {
    MonitorAutoLock lock(*gMonitor);

    // Search for the first message received from this process.
    ChildProcessInfo* process = this;
    Message::UniquePtr msg = ExtractChildMessage(&process);

    if (msg) {
      OnIncomingMessage(*msg);
      if (IsPaused()) {
        return;
      }
    } else {
      if (gChildrenAreDebugging || IsRecording()) {
        // Don't watch for hangs when children are being debugged. Recording
        // children are never treated as hanged both because they cannot be
        // restarted and because they may just be idling.
        gMonitor->Wait();
      } else {
        TimeStamp deadline =
            mLastMessageTime + TimeDuration::FromSeconds(HangSeconds);
        if (TimeStamp::Now() >= deadline) {
          MonitorAutoUnlock unlock(*gMonitor);
          if (!sentTerminateMessage) {
            // Try to get the child to crash, so that we can get a minidump.
            // Sending the message will reset mLastMessageTime so we get to
            // wait another HangSeconds before hitting the restart case below.
            // Use SendMessageRaw to avoid problems if we are recovering.
<<<<<<< HEAD
            CrashReporter::AnnotateCrashReport(
                CrashReporter::Annotation::RecordReplayHang, true);
            SendMessageRaw(TerminateMessage());
||||||| merged common ancestors
            CrashReporter::AnnotateCrashReport(CrashReporter::Annotation::RecordReplayHang,
                                               true);
            SendMessageRaw(TerminateMessage());
=======
            CrashReporter::AnnotateCrashReport(
                CrashReporter::Annotation::RecordReplayHang, true);
            SendMessage(TerminateMessage());
>>>>>>> upstream-releases
            sentTerminateMessage = true;
          } else {
            // The child is still non-responsive after sending the terminate
            // message.
            OnCrash("Child process non-responsive");
          }
        }
        gMonitor->WaitUntil(deadline);
      }
    }
  }
}

// Runnable created on the main thread to handle any tasks sent by the replay
// message loop thread which were not handled while the main thread was blocked.
<<<<<<< HEAD
/* static */ void ChildProcessInfo::MaybeProcessPendingMessageRunnable() {
||||||| merged common ancestors
/* static */ void
ChildProcessInfo::MaybeProcessPendingMessageRunnable()
{
=======
/* static */
void ChildProcessInfo::MaybeProcessPendingMessageRunnable() {
>>>>>>> upstream-releases
  MOZ_RELEASE_ASSERT(NS_IsMainThread());
  MonitorAutoLock lock(*gMonitor);
  MOZ_RELEASE_ASSERT(gHasPendingMessageRunnable);
  gHasPendingMessageRunnable = false;
<<<<<<< HEAD
  while (MaybeProcessPendingMessage(nullptr)) {
  }
||||||| merged common ancestors
  while (MaybeProcessPendingMessage(nullptr)) {}
=======
  while (true) {
    ChildProcessInfo* process = nullptr;
    Message::UniquePtr msg = ExtractChildMessage(&process);

    if (msg) {
      MonitorAutoUnlock unlock(*gMonitor);
      process->OnIncomingMessage(*msg);
    } else {
      break;
    }
  }
>>>>>>> upstream-releases
}

// Execute a task that processes a message received from the child. This is
// called on a channel thread, and the function executes asynchronously on
// the main thread.
<<<<<<< HEAD
void ChildProcessInfo::ReceiveChildMessageOnMainThread(size_t aChannelId,
                                                       Message* aMsg) {
||||||| merged common ancestors
void
ChildProcessInfo::ReceiveChildMessageOnMainThread(size_t aChannelId, Message* aMsg)
{
=======
void ChildProcessInfo::ReceiveChildMessageOnMainThread(
    Message::UniquePtr aMsg) {
>>>>>>> upstream-releases
  MOZ_RELEASE_ASSERT(!NS_IsMainThread());

  MonitorAutoLock lock(*gMonitor);

  PendingMessage pending;
  pending.mProcess = this;
  pending.mMsg = std::move(aMsg);
  gPendingMessages.append(std::move(pending));

  // Notify the main thread, if it is waiting in WaitUntilPaused.
  gMonitor->NotifyAll();

  // Make sure there is a task on the main thread's message loop that can
  // process this task if necessary.
  if (!gHasPendingMessageRunnable) {
    gHasPendingMessageRunnable = true;
    MainThreadMessageLoop()->PostTask(
        NewRunnableFunction("MaybeProcessPendingMessageRunnable",
                            MaybeProcessPendingMessageRunnable));
  }
}

}  // namespace parent
}  // namespace recordreplay
}  // namespace mozilla
