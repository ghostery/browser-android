/* -*- Mode: C++; tab-width: 8; indent-tabs-mode: nil; c-basic-offset: 2 -*- */
/* vim: set ts=8 sts=2 et sw=2 tw=80: */
/* This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at http://mozilla.org/MPL/2.0/. */

#ifndef mozilla_recordreplay_Channel_h
#define mozilla_recordreplay_Channel_h

#include "base/process.h"

#include "mozilla/gfx/Types.h"
#include "mozilla/Maybe.h"
#include "mozilla/UniquePtr.h"

#include "File.h"
#include "JSControl.h"
#include "MiddlemanCall.h"
#include "Monitor.h"

namespace mozilla {
namespace recordreplay {

// This file has definitions for creating and communicating on a special
// bidirectional channel between a middleman process and a recording or
// replaying process. This communication is not included in the recording, and
// when replaying this is the only mechanism the child can use to communicate
// with the middleman process.
//
// Replaying processes can rewind themselves, restoring execution state and the
// contents of all heap memory to that at an earlier point. To keep the
// replaying process and middleman from getting out of sync with each other,
// there are tight constraints on when messages may be sent across the channel
// by one process or the other. At any given time the child process may be
// either paused or unpaused. If it is paused, it is not doing any execution
// and cannot rewind itself. If it is unpaused, it may execute content and may
// rewind itself.
//
// Messages can be sent from the child process to the middleman only when the
// child process is unpaused, and messages can only be sent from the middleman
// to the child process when the child process is paused. This prevents
// messages from being lost when they are sent from the middleman as the
// replaying process rewinds itself. A few exceptions to this rule are noted
// below.

#define ForEachMessageType(_Macro)                             \
  /* Messages sent from the middleman to the child process. */ \
                                                               \
  /* Sent at startup. */                                       \
  _Macro(Introduction)                                         \
                                                               \
  /* Sent to recording processes to indicate that the middleman will be running */ \
  /* developer tools server-side code instead of the recording process itself. */ \
  _Macro(SetDebuggerRunsInMiddleman)                           \
                                                               \
  /* Sent to recording processes when exiting, or to force a hanged replaying */ \
  /* process to crash. */                                      \
  _Macro(Terminate)                                            \
                                                               \
  /* Poke a child that is recording to create an artificial checkpoint, rather than */ \
  /* (potentially) idling indefinitely. This has no effect on a replaying process. */ \
  _Macro(CreateCheckpoint)                                     \
                                                               \
<<<<<<< HEAD
  /* Debugger JSON messages are initially sent from the parent. The child unpauses */ \
  /* after receiving the message and will pause after it sends a DebuggerResponse. */ \
  _Macro(DebuggerRequest)                                      \
                                                               \
  /* Add a breakpoint position to stop at. Because a single entry point is used for */ \
  /* calling into the ReplayDebugger after pausing, the set of breakpoints is simply */ \
  /* a set of positions at which the child process should pause and send a HitBreakpoint */ \
  /* message. */                                               \
  _Macro(AddBreakpoint)                                        \
                                                               \
  /* Clear all installed breakpoints. */                       \
  _Macro(ClearBreakpoints)                                     \
                                                               \
  /* Unpause the child and play execution either to the next point when a */ \
  /* breakpoint is hit, or to the next checkpoint. Resumption may be either */ \
  /* forward or backward. */                                   \
  _Macro(Resume)                                               \
                                                               \
  /* Rewind to a particular saved checkpoint in the past. */   \
  _Macro(RestoreCheckpoint)                                    \
                                                               \
  /* Run forward to a particular execution point between the current checkpoint */ \
  /* and the next one. */                                      \
  _Macro(RunToPoint)                                           \
                                                               \
  /* Notify the child whether it is the active child and should send paint and similar */ \
  /* messages to the middleman. */                             \
  _Macro(SetIsActive)                                          \
                                                               \
  /* Set whether to perform intentional crashes, for testing. */ \
  _Macro(SetAllowIntentionalCrashes)                           \
                                                               \
  /* Set whether to save a particular checkpoint. */           \
  _Macro(SetSaveCheckpoint)                                    \
||||||| merged common ancestors
  /* Debugger JSON messages are initially sent from the parent. The child unpauses */ \
  /* after receiving the message and will pause after it sends a DebuggerResponse. */ \
  _Macro(DebuggerRequest)                                      \
                                                               \
  /* Set or clear a JavaScript breakpoint. */                  \
  _Macro(SetBreakpoint)                                        \
                                                               \
  /* Unpause the child and play execution either to the next point when a */ \
  /* breakpoint is hit, or to the next checkpoint. Resumption may be either */ \
  /* forward or backward. */                                   \
  _Macro(Resume)                                               \
                                                               \
  /* Rewind to a particular saved checkpoint in the past. */   \
  _Macro(RestoreCheckpoint)                                    \
                                                               \
  /* Run forward to a particular execution point between the current checkpoint */ \
  /* and the next one. */                                      \
  _Macro(RunToPoint)                                           \
                                                               \
  /* Notify the child whether it is the active child and should send paint and similar */ \
  /* messages to the middleman. */                             \
  _Macro(SetIsActive)                                          \
                                                               \
  /* Set whether to perform intentional crashes, for testing. */ \
  _Macro(SetAllowIntentionalCrashes)                           \
                                                               \
  /* Set whether to save a particular checkpoint. */           \
  _Macro(SetSaveCheckpoint)                                    \
=======
  /* Unpause the child and perform a debugger-defined operation. */ \
  _Macro(ManifestStart)                                             \
>>>>>>> upstream-releases
                                                               \
  /* Respond to a MiddlemanCallRequest message. */             \
  _Macro(MiddlemanCallResponse)                                \
                                                               \
  /* Messages sent from the child process to the middleman. */ \
                                                               \
  /* Pause after executing a manifest, specifying its response. */ \
  _Macro(ManifestFinished)                                     \
                                                               \
  /* A critical error occurred and execution cannot continue. The child will */ \
  /* stop executing after sending this message and will wait to be terminated. */ \
  /* A minidump for the child has been generated. */           \
  _Macro(FatalError)                                           \
                                                               \
  /* Sent when a fatal error has occurred, but before the minidump has been */ \
  /* generated. */                                             \
  _Macro(BeginFatalError)                                      \
                                                               \
  /* The child's graphics were repainted. */                   \
  _Macro(Paint)                                                \
                                                               \
  /* Call a system function from the middleman process which the child has */ \
  /* encountered after diverging from the recording. */        \
  _Macro(MiddlemanCallRequest)                                 \
                                                               \
  /* Reset all information generated by previous MiddlemanCallRequest messages. */ \
  _Macro(ResetMiddlemanCalls)

enum class MessageType {
#define DefineEnum(Kind) Kind,
  ForEachMessageType(DefineEnum)
#undef DefineEnum
};

struct Message {
  MessageType mType;

  // When simulating message delays, the time this message should be received,
  // relative to when the channel was opened.
  uint32_t mReceiveTime;

  // Total message size, including the header.
  uint32_t mSize;

<<<<<<< HEAD
 protected:
  Message(MessageType aType, uint32_t aSize) : mType(aType), mSize(aSize) {
||||||| merged common ancestors
protected:
  Message(MessageType aType, uint32_t aSize)
    : mType(aType), mSize(aSize)
  {
=======
 protected:
  Message(MessageType aType, uint32_t aSize)
      : mType(aType), mReceiveTime(0), mSize(aSize) {
>>>>>>> upstream-releases
    MOZ_RELEASE_ASSERT(mSize >= sizeof(*this));
  }

<<<<<<< HEAD
 public:
  Message* Clone() const {
    char* res = (char*)malloc(mSize);
||||||| merged common ancestors
public:
  Message* Clone() const {
    char* res = (char*) malloc(mSize);
=======
 public:
  struct FreePolicy {
    void operator()(Message* msg) { /*free(msg);*/
    }
  };
  typedef UniquePtr<Message, FreePolicy> UniquePtr;

  UniquePtr Clone() const {
    Message* res = static_cast<Message*>(malloc(mSize));
>>>>>>> upstream-releases
    memcpy(res, this, mSize);
<<<<<<< HEAD
    return (Message*)res;
||||||| merged common ancestors
    return (Message*) res;
=======
    return UniquePtr(res);
>>>>>>> upstream-releases
  }

  const char* TypeString() const {
    switch (mType) {
#define EnumToString(Kind) \
  case MessageType::Kind:  \
    return #Kind;
      ForEachMessageType(EnumToString)
#undef EnumToString
          default : return "Unknown";
    }
  }

  // Return whether this is a middleman->child message that can be sent while
  // the child is unpaused.
  bool CanBeSentWhileUnpaused() const {
<<<<<<< HEAD
    return mType == MessageType::CreateCheckpoint ||
           mType == MessageType::SetDebuggerRunsInMiddleman ||
           mType == MessageType::MiddlemanCallResponse ||
           mType == MessageType::Terminate;
||||||| merged common ancestors
    return mType == MessageType::CreateCheckpoint
        || mType == MessageType::SetDebuggerRunsInMiddleman
        || mType == MessageType::MiddlemanCallResponse
        || mType == MessageType::Terminate;
=======
    return mType == MessageType::CreateCheckpoint ||
           mType == MessageType::SetDebuggerRunsInMiddleman ||
           mType == MessageType::MiddlemanCallResponse ||
           mType == MessageType::Terminate ||
           mType == MessageType::Introduction;
>>>>>>> upstream-releases
  }

 protected:
  template <typename T, typename Elem>
  Elem* Data() {
    return (Elem*)(sizeof(T) + (char*)this);
  }

  template <typename T, typename Elem>
  const Elem* Data() const {
    return (const Elem*)(sizeof(T) + (const char*)this);
  }

  template <typename T, typename Elem>
  size_t DataSize() const {
    return (mSize - sizeof(T)) / sizeof(Elem);
  }

  template <typename T, typename Elem, typename... Args>
  static T* NewWithData(size_t aBufferSize, Args&&... aArgs) {
    size_t size = sizeof(T) + aBufferSize * sizeof(Elem);
    void* ptr = malloc(size);
    return new (ptr) T(size, std::forward<Args>(aArgs)...);
  }
};

struct IntroductionMessage : public Message {
  base::ProcessId mParentPid;
  uint32_t mArgc;

  IntroductionMessage(uint32_t aSize, base::ProcessId aParentPid,
                      uint32_t aArgc)
      : Message(MessageType::Introduction, aSize),
        mParentPid(aParentPid),
        mArgc(aArgc) {}

  char* ArgvString() { return Data<IntroductionMessage, char>(); }
  const char* ArgvString() const { return Data<IntroductionMessage, char>(); }

  static IntroductionMessage* New(base::ProcessId aParentPid, int aArgc,
                                  char* aArgv[]) {
    size_t argsLen = 0;
    for (int i = 0; i < aArgc; i++) {
      argsLen += strlen(aArgv[i]) + 1;
    }

    IntroductionMessage* res =
        NewWithData<IntroductionMessage, char>(argsLen, aParentPid, aArgc);

    size_t offset = 0;
    for (int i = 0; i < aArgc; i++) {
      memcpy(&res->ArgvString()[offset], aArgv[i], strlen(aArgv[i]) + 1);
      offset += strlen(aArgv[i]) + 1;
    }
    MOZ_RELEASE_ASSERT(offset == argsLen);

    return res;
  }

  static IntroductionMessage* RecordReplay(const IntroductionMessage& aMsg) {
    size_t introductionSize = RecordReplayValue(aMsg.mSize);
    IntroductionMessage* msg = (IntroductionMessage*)malloc(introductionSize);
    if (IsRecording()) {
      memcpy(msg, &aMsg, introductionSize);
    }
    RecordReplayBytes(msg, introductionSize);
    return msg;
  }
};

template <MessageType Type>
struct EmptyMessage : public Message {
  EmptyMessage() : Message(Type, sizeof(*this)) {}
};

typedef EmptyMessage<MessageType::SetDebuggerRunsInMiddleman>
    SetDebuggerRunsInMiddlemanMessage;
typedef EmptyMessage<MessageType::Terminate> TerminateMessage;
typedef EmptyMessage<MessageType::CreateCheckpoint> CreateCheckpointMessage;

template <MessageType Type>
struct JSONMessage : public Message {
  explicit JSONMessage(uint32_t aSize) : Message(Type, aSize) {}

  const char16_t* Buffer() const { return Data<JSONMessage<Type>, char16_t>(); }
  size_t BufferSize() const { return DataSize<JSONMessage<Type>, char16_t>(); }

  static JSONMessage<Type>* New(const char16_t* aBuffer, size_t aBufferSize) {
    JSONMessage<Type>* res =
        NewWithData<JSONMessage<Type>, char16_t>(aBufferSize);
    MOZ_RELEASE_ASSERT(res->BufferSize() == aBufferSize);
    PodCopy(res->Data<JSONMessage<Type>, char16_t>(), aBuffer, aBufferSize);
    return res;
  }
};

<<<<<<< HEAD
typedef JSONMessage<MessageType::DebuggerRequest> DebuggerRequestMessage;
typedef JSONMessage<MessageType::DebuggerResponse> DebuggerResponseMessage;

struct AddBreakpointMessage : public Message {
  js::BreakpointPosition mPosition;

  explicit AddBreakpointMessage(const js::BreakpointPosition& aPosition)
      : Message(MessageType::AddBreakpoint, sizeof(*this)),
        mPosition(aPosition) {}
};

typedef EmptyMessage<MessageType::ClearBreakpoints> ClearBreakpointsMessage;

struct ResumeMessage : public Message {
  // Whether to travel forwards or backwards.
  bool mForward;

  explicit ResumeMessage(bool aForward)
      : Message(MessageType::Resume, sizeof(*this)), mForward(aForward) {}
};

struct RestoreCheckpointMessage : public Message {
  // The checkpoint to restore.
  size_t mCheckpoint;

  explicit RestoreCheckpointMessage(size_t aCheckpoint)
      : Message(MessageType::RestoreCheckpoint, sizeof(*this)),
        mCheckpoint(aCheckpoint) {}
};

struct RunToPointMessage : public Message {
  // The target execution point.
  js::ExecutionPoint mTarget;

  explicit RunToPointMessage(const js::ExecutionPoint& aTarget)
      : Message(MessageType::RunToPoint, sizeof(*this)), mTarget(aTarget) {}
};

struct SetIsActiveMessage : public Message {
  // Whether this is the active child process (see ParentIPC.cpp).
  bool mActive;
||||||| merged common ancestors
typedef JSONMessage<MessageType::DebuggerRequest> DebuggerRequestMessage;
typedef JSONMessage<MessageType::DebuggerResponse> DebuggerResponseMessage;

struct SetBreakpointMessage : public Message
{
  // ID of the breakpoint to change.
  size_t mId;

  // New position of the breakpoint. If this is invalid then the breakpoint is
  // being cleared.
  js::BreakpointPosition mPosition;

  SetBreakpointMessage(size_t aId, const js::BreakpointPosition& aPosition)
    : Message(MessageType::SetBreakpoint, sizeof(*this))
    , mId(aId)
    , mPosition(aPosition)
  {}
};

struct ResumeMessage : public Message
{
  // Whether to travel forwards or backwards.
  bool mForward;

  explicit ResumeMessage(bool aForward)
    : Message(MessageType::Resume, sizeof(*this))
    , mForward(aForward)
  {}
};

struct RestoreCheckpointMessage : public Message
{
  // The checkpoint to restore.
  size_t mCheckpoint;

  explicit RestoreCheckpointMessage(size_t aCheckpoint)
    : Message(MessageType::RestoreCheckpoint, sizeof(*this))
    , mCheckpoint(aCheckpoint)
  {}
};

struct RunToPointMessage : public Message
{
  // The target execution point.
  js::ExecutionPoint mTarget;

  explicit RunToPointMessage(const js::ExecutionPoint& aTarget)
    : Message(MessageType::RunToPoint, sizeof(*this))
    , mTarget(aTarget)
  {}
};

struct SetIsActiveMessage : public Message
{
  // Whether this is the active child process (see ParentIPC.cpp).
  bool mActive;
=======
typedef JSONMessage<MessageType::ManifestStart> ManifestStartMessage;
typedef JSONMessage<MessageType::ManifestFinished> ManifestFinishedMessage;
>>>>>>> upstream-releases

<<<<<<< HEAD
  explicit SetIsActiveMessage(bool aActive)
      : Message(MessageType::SetIsActive, sizeof(*this)), mActive(aActive) {}
};

struct SetAllowIntentionalCrashesMessage : public Message {
  // Whether to allow intentional crashes in the future or not.
  bool mAllowed;

  explicit SetAllowIntentionalCrashesMessage(bool aAllowed)
      : Message(MessageType::SetAllowIntentionalCrashes, sizeof(*this)),
        mAllowed(aAllowed) {}
};

struct SetSaveCheckpointMessage : public Message {
  // The checkpoint in question.
  size_t mCheckpoint;

  // Whether to save this checkpoint whenever it is encountered.
  bool mSave;

  SetSaveCheckpointMessage(size_t aCheckpoint, bool aSave)
      : Message(MessageType::SetSaveCheckpoint, sizeof(*this)),
        mCheckpoint(aCheckpoint),
        mSave(aSave) {}
};

typedef EmptyMessage<MessageType::RecordingFlushed> RecordingFlushedMessage;

struct FatalErrorMessage : public Message {
||||||| merged common ancestors
  explicit SetIsActiveMessage(bool aActive)
    : Message(MessageType::SetIsActive, sizeof(*this))
    , mActive(aActive)
  {}
};

struct SetAllowIntentionalCrashesMessage : public Message
{
  // Whether to allow intentional crashes in the future or not.
  bool mAllowed;

  explicit SetAllowIntentionalCrashesMessage(bool aAllowed)
    : Message(MessageType::SetAllowIntentionalCrashes, sizeof(*this))
    , mAllowed(aAllowed)
  {}
};

struct SetSaveCheckpointMessage : public Message
{
  // The checkpoint in question.
  size_t mCheckpoint;

  // Whether to save this checkpoint whenever it is encountered.
  bool mSave;

  SetSaveCheckpointMessage(size_t aCheckpoint, bool aSave)
    : Message(MessageType::SetSaveCheckpoint, sizeof(*this))
    , mCheckpoint(aCheckpoint)
    , mSave(aSave)
  {}
};

typedef EmptyMessage<MessageType::RecordingFlushed> RecordingFlushedMessage;

struct FatalErrorMessage : public Message
{
=======
struct FatalErrorMessage : public Message {
>>>>>>> upstream-releases
  explicit FatalErrorMessage(uint32_t aSize)
      : Message(MessageType::FatalError, aSize) {}

  const char* Error() const { return Data<FatalErrorMessage, const char>(); }
};

typedef EmptyMessage<MessageType::BeginFatalError> BeginFatalErrorMessage;

// The format for graphics data which will be sent to the middleman process.
// This needs to match the format expected for canvas image data, to avoid
// transforming the data before rendering it in the middleman process.
static const gfx::SurfaceFormat gSurfaceFormat = gfx::SurfaceFormat::R8G8B8X8;

struct PaintMessage : public Message {
  // Checkpoint whose state is being painted.
  uint32_t mCheckpointId;

  uint32_t mWidth;
  uint32_t mHeight;

  PaintMessage(uint32_t aCheckpointId, uint32_t aWidth, uint32_t aHeight)
      : Message(MessageType::Paint, sizeof(*this)),
        mCheckpointId(aCheckpointId),
        mWidth(aWidth),
        mHeight(aHeight) {}
};

<<<<<<< HEAD
struct HitCheckpointMessage : public Message {
  uint32_t mCheckpointId;
  bool mRecordingEndpoint;

  // When recording, the amount of non-idle time taken to get to this
  // checkpoint from the previous one.
  double mDurationMicroseconds;

  HitCheckpointMessage(uint32_t aCheckpointId, bool aRecordingEndpoint,
                       double aDurationMicroseconds)
      : Message(MessageType::HitCheckpoint, sizeof(*this)),
        mCheckpointId(aCheckpointId),
        mRecordingEndpoint(aRecordingEndpoint),
        mDurationMicroseconds(aDurationMicroseconds) {}
};

struct HitBreakpointMessage : public Message {
  bool mRecordingEndpoint;

  explicit HitBreakpointMessage(bool aRecordingEndpoint)
      : Message(MessageType::HitBreakpoint, sizeof(*this)),
        mRecordingEndpoint(aRecordingEndpoint) {}
};

typedef EmptyMessage<MessageType::AlwaysMarkMajorCheckpoints>
    AlwaysMarkMajorCheckpointsMessage;

||||||| merged common ancestors
struct HitCheckpointMessage : public Message
{
  uint32_t mCheckpointId;
  bool mRecordingEndpoint;

  // When recording, the amount of non-idle time taken to get to this
  // checkpoint from the previous one.
  double mDurationMicroseconds;

  HitCheckpointMessage(uint32_t aCheckpointId, bool aRecordingEndpoint, double aDurationMicroseconds)
    : Message(MessageType::HitCheckpoint, sizeof(*this))
    , mCheckpointId(aCheckpointId)
    , mRecordingEndpoint(aRecordingEndpoint)
    , mDurationMicroseconds(aDurationMicroseconds)
  {}
};

struct HitBreakpointMessage : public Message
{
  bool mRecordingEndpoint;

  HitBreakpointMessage(uint32_t aSize, bool aRecordingEndpoint)
    : Message(MessageType::HitBreakpoint, aSize)
    , mRecordingEndpoint(aRecordingEndpoint)
  {}

  const uint32_t* Breakpoints() const { return Data<HitBreakpointMessage, uint32_t>(); }
  uint32_t NumBreakpoints() const { return DataSize<HitBreakpointMessage, uint32_t>(); }

  static HitBreakpointMessage* New(bool aRecordingEndpoint,
                                   const uint32_t* aBreakpoints, size_t aNumBreakpoints) {
    HitBreakpointMessage* res =
      NewWithData<HitBreakpointMessage, uint32_t>(aNumBreakpoints, aRecordingEndpoint);
    MOZ_RELEASE_ASSERT(res->NumBreakpoints() == aNumBreakpoints);
    PodCopy(res->Data<HitBreakpointMessage, uint32_t>(), aBreakpoints, aNumBreakpoints);
    return res;
  }
};

typedef EmptyMessage<MessageType::AlwaysMarkMajorCheckpoints> AlwaysMarkMajorCheckpointsMessage;

=======
>>>>>>> upstream-releases
template <MessageType Type>
struct BinaryMessage : public Message {
  explicit BinaryMessage(uint32_t aSize) : Message(Type, aSize) {}

  const char* BinaryData() const { return Data<BinaryMessage<Type>, char>(); }
  size_t BinaryDataSize() const {
    return DataSize<BinaryMessage<Type>, char>();
  }

  static BinaryMessage<Type>* New(const char* aData, size_t aDataSize) {
    BinaryMessage<Type>* res =
        NewWithData<BinaryMessage<Type>, char>(aDataSize);
    MOZ_RELEASE_ASSERT(res->BinaryDataSize() == aDataSize);
    PodCopy(res->Data<BinaryMessage<Type>, char>(), aData, aDataSize);
    return res;
  }
};

<<<<<<< HEAD
typedef BinaryMessage<MessageType::MiddlemanCallRequest>
    MiddlemanCallRequestMessage;
typedef BinaryMessage<MessageType::MiddlemanCallResponse>
    MiddlemanCallResponseMessage;
typedef EmptyMessage<MessageType::ResetMiddlemanCalls>
    ResetMiddlemanCallsMessage;

static inline MiddlemanCallResponseMessage* ProcessMiddlemanCallMessage(
    const MiddlemanCallRequestMessage& aMsg) {
  InfallibleVector<char> outputData;
  ProcessMiddlemanCall(aMsg.BinaryData(), aMsg.BinaryDataSize(), &outputData);
  return MiddlemanCallResponseMessage::New(outputData.begin(),
                                           outputData.length());
}

class Channel {
 public:
||||||| merged common ancestors
typedef BinaryMessage<MessageType::MiddlemanCallRequest> MiddlemanCallRequestMessage;
typedef BinaryMessage<MessageType::MiddlemanCallResponse> MiddlemanCallResponseMessage;
typedef EmptyMessage<MessageType::ResetMiddlemanCalls> ResetMiddlemanCallsMessage;

class Channel
{
public:
=======
typedef BinaryMessage<MessageType::MiddlemanCallRequest>
    MiddlemanCallRequestMessage;
typedef BinaryMessage<MessageType::MiddlemanCallResponse>
    MiddlemanCallResponseMessage;
typedef EmptyMessage<MessageType::ResetMiddlemanCalls>
    ResetMiddlemanCallsMessage;

class Channel {
 public:
>>>>>>> upstream-releases
  // Note: the handler is responsible for freeing its input message. It will be
  // called on the channel's message thread.
  typedef std::function<void(Message::UniquePtr)> MessageHandler;

 private:
  // ID for this channel, unique for the middleman.
  size_t mId;

  // Callback to invoke off thread on incoming messages.
  MessageHandler mHandler;

  // Whether the channel is initialized and ready for outgoing messages.
  Atomic<bool, SequentiallyConsistent, Behavior::DontPreserve> mInitialized;

  // Descriptor used to accept connections on the parent side.
  int mConnectionFd;

  // Descriptor used to communicate with the other side.
  int mFd;

  // For synchronizing initialization of the channel.
  Monitor mMonitor;

  // Buffer for message data received from the other side of the channel.
  typedef InfallibleVector<char, 0, AllocPolicy<MemoryKind::Generic>>
      MessageBuffer;
  MessageBuffer* mMessageBuffer;

  // The number of bytes of data already in the message buffer.
  size_t mMessageBytes;

  // Whether this channel is subject to message delays during simulation.
  bool mSimulateDelays;

  // The time this channel was opened, for use in simulating message delays.
  TimeStamp mStartTime;

  // When simulating message delays, the time at which old messages will have
  // finished sending and new messages may be sent.
  TimeStamp mAvailableTime;

  // If spew is enabled, print a message and associated info to stderr.
  void PrintMessage(const char* aPrefix, const Message& aMsg);

  // Block until a complete message is received from the other side of the
  // channel.
  Message::UniquePtr WaitForMessage();

  // Main routine for the channel's thread.
  static void ThreadMain(void* aChannel);

 public:
  // Initialize this channel, connect to the other side, and spin up a thread
  // to process incoming messages by calling aHandler.
  Channel(size_t aId, bool aMiddlemanRecording, const MessageHandler& aHandler);

  size_t GetId() { return mId; }

  // Send a message to the other side of the channel. This must be called on
  // the main thread, except for fatal error messages.
  void SendMessage(Message&& aMsg);
};

// Command line option used to specify the middleman pid for a child process.
static const char* gMiddlemanPidOption = "-middlemanPid";

// Command line option used to specify the channel ID for a child process.
static const char* gChannelIDOption = "-recordReplayChannelID";

}  // namespace recordreplay
}  // namespace mozilla

#endif  // mozilla_recordreplay_Channel_h
