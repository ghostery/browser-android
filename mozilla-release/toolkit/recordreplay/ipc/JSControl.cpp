/* -*- Mode: C++; tab-width: 8; indent-tabs-mode: nil; c-basic-offset: 2 -*-
 * vim: set ts=8 sts=2 et sw=2 tw=80:
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/. */

#include "JSControl.h"

#include "mozilla/ClearOnShutdown.h"
#include "mozilla/StaticPtr.h"
#include "js/CharacterEncoding.h"
#include "js/Conversions.h"
#include "js/JSON.h"
#include "js/PropertySpec.h"
#include "ChildInternal.h"
#include "ParentInternal.h"
#include "nsImportModule.h"
#include "rrIControl.h"
#include "rrIReplay.h"
#include "xpcprivate.h"

using namespace JS;

namespace mozilla {
namespace recordreplay {
namespace js {

// Callback for filling CharBuffers when converting objects to JSON.
static bool FillCharBufferCallback(const char16_t* buf, uint32_t len,
                                   void* data) {
  CharBuffer* buffer = (CharBuffer*)data;
  MOZ_RELEASE_ASSERT(buffer->length() == 0);
  buffer->append(buf, len);
  return true;
}

<<<<<<< HEAD
static JSObject* NonNullObject(JSContext* aCx, HandleValue aValue) {
||||||| merged common ancestors
static JSObject*
NonNullObject(JSContext* aCx, HandleValue aValue)
{
=======
static JSObject* RequireObject(JSContext* aCx, HandleValue aValue) {
>>>>>>> upstream-releases
  if (!aValue.isObject()) {
    JS_ReportErrorASCII(aCx, "Expected object");
    return nullptr;
  }
  return &aValue.toObject();
}

<<<<<<< HEAD
template <typename T>
static bool MaybeGetNumberProperty(JSContext* aCx, HandleObject aObject,
                                   const char* aProperty, T* aResult) {
  RootedValue v(aCx);
  if (!JS_GetProperty(aCx, aObject, aProperty, &v)) {
    return false;
  }
  if (v.isNumber()) {
    *aResult = v.toNumber();
  }
  return true;
}

template <typename T>
static bool GetNumberProperty(JSContext* aCx, HandleObject aObject,
                              const char* aProperty, T* aResult) {
  RootedValue v(aCx);
  if (!JS_GetProperty(aCx, aObject, aProperty, &v)) {
    return false;
||||||| merged common ancestors
template <typename T>
static bool
MaybeGetNumberProperty(JSContext* aCx, HandleObject aObject, const char* aProperty, T* aResult)
{
  RootedValue v(aCx);
  if (!JS_GetProperty(aCx, aObject, aProperty, &v)) {
    return false;
  }
  if (v.isNumber()) {
    *aResult = v.toNumber();
  }
  return true;
}

template <typename T>
static bool
GetNumberProperty(JSContext* aCx, HandleObject aObject, const char* aProperty, T* aResult)
{
  RootedValue v(aCx);
  if (!JS_GetProperty(aCx, aObject, aProperty, &v)) {
    return false;
=======
static parent::ChildProcessInfo* GetChildById(JSContext* aCx,
                                              const Value& aValue,
                                              bool aAllowUnpaused = false) {
  if (!aValue.isNumber()) {
    JS_ReportErrorASCII(aCx, "Expected child ID");
    return nullptr;
>>>>>>> upstream-releases
  }
  parent::ChildProcessInfo* child = parent::GetChildProcess(aValue.toNumber());
  if (!child || (!aAllowUnpaused && !child->IsPaused())) {
    JS_ReportErrorASCII(aCx, "Unpaused or bad child ID");
    return nullptr;
  }
  return child;
}

///////////////////////////////////////////////////////////////////////////////
// Middleman Control
///////////////////////////////////////////////////////////////////////////////

<<<<<<< HEAD
// Names of properties which JS code uses to specify the contents of a
// BreakpointPosition.
static const char gKindProperty[] = "kind";
static const char gScriptProperty[] = "script";
static const char gOffsetProperty[] = "offset";
static const char gFrameIndexProperty[] = "frameIndex";
||||||| merged common ancestors
// Names of properties which JS code uses to specify the contents of a BreakpointPosition.
static const char gKindProperty[] = "kind";
static const char gScriptProperty[] = "script";
static const char gOffsetProperty[] = "offset";
static const char gFrameIndexProperty[] = "frameIndex";
=======
static StaticRefPtr<rrIControl> gControl;
>>>>>>> upstream-releases

<<<<<<< HEAD
JSObject* BreakpointPosition::Encode(JSContext* aCx) const {
  RootedString kindString(aCx, JS_NewStringCopyZ(aCx, KindString()));
  RootedObject obj(aCx, JS_NewObject(aCx, nullptr));
  if (!kindString || !obj ||
      !JS_DefineProperty(aCx, obj, gKindProperty, kindString,
                         JSPROP_ENUMERATE) ||
      (mScript != BreakpointPosition::EMPTY_SCRIPT &&
       !JS_DefineProperty(aCx, obj, gScriptProperty, mScript,
                          JSPROP_ENUMERATE)) ||
      (mOffset != BreakpointPosition::EMPTY_OFFSET &&
       !JS_DefineProperty(aCx, obj, gOffsetProperty, mOffset,
                          JSPROP_ENUMERATE)) ||
      (mFrameIndex != BreakpointPosition::EMPTY_FRAME_INDEX &&
       !JS_DefineProperty(aCx, obj, gFrameIndexProperty, mFrameIndex,
                          JSPROP_ENUMERATE))) {
    return nullptr;
  }
  return obj;
}
||||||| merged common ancestors
JSObject*
BreakpointPosition::Encode(JSContext* aCx) const
{
  RootedString kindString(aCx, JS_NewStringCopyZ(aCx, KindString()));
  RootedObject obj(aCx, JS_NewObject(aCx, nullptr));
  if (!kindString || !obj ||
      !JS_DefineProperty(aCx, obj, gKindProperty, kindString, JSPROP_ENUMERATE) ||
      (mScript != BreakpointPosition::EMPTY_SCRIPT &&
       !JS_DefineProperty(aCx, obj, gScriptProperty, mScript, JSPROP_ENUMERATE)) ||
      (mOffset != BreakpointPosition::EMPTY_OFFSET &&
       !JS_DefineProperty(aCx, obj, gOffsetProperty, mOffset, JSPROP_ENUMERATE)) ||
      (mFrameIndex != BreakpointPosition::EMPTY_FRAME_INDEX &&
       !JS_DefineProperty(aCx, obj, gFrameIndexProperty, mFrameIndex, JSPROP_ENUMERATE)))
  {
    return nullptr;
  }
  return obj;
}
=======
void SetupMiddlemanControl(const Maybe<size_t>& aRecordingChildId) {
  MOZ_RELEASE_ASSERT(!gControl);
>>>>>>> upstream-releases

<<<<<<< HEAD
bool BreakpointPosition::Decode(JSContext* aCx, HandleObject aObject) {
  RootedValue v(aCx);
  if (!JS_GetProperty(aCx, aObject, gKindProperty, &v)) {
    return false;
  }
||||||| merged common ancestors
bool
BreakpointPosition::Decode(JSContext* aCx, HandleObject aObject)
{
  RootedValue v(aCx);
  if (!JS_GetProperty(aCx, aObject, gKindProperty, &v)) {
    return false;
  }
=======
  nsCOMPtr<rrIControl> control =
      do_ImportModule("resource://devtools/server/actors/replay/control.js");
  gControl = control.forget();
  ClearOnShutdown(&gControl);
>>>>>>> upstream-releases

<<<<<<< HEAD
  RootedString str(aCx, ToString(aCx, v));
  for (size_t i = BreakpointPosition::Invalid + 1;
       i < BreakpointPosition::sKindCount; i++) {
    BreakpointPosition::Kind kind = (BreakpointPosition::Kind)i;
    bool match;
    if (!JS_StringEqualsAscii(
            aCx, str, BreakpointPosition::StaticKindString(kind), &match))
      return false;
    if (match) {
      mKind = kind;
      break;
    }
||||||| merged common ancestors
  RootedString str(aCx, ToString(aCx, v));
  for (size_t i = BreakpointPosition::Invalid + 1; i < BreakpointPosition::sKindCount; i++) {
    BreakpointPosition::Kind kind = (BreakpointPosition::Kind) i;
    bool match;
    if (!JS_StringEqualsAscii(aCx, str, BreakpointPosition::StaticKindString(kind), &match))
      return false;
    if (match) {
      mKind = kind;
      break;
    }
=======
  MOZ_RELEASE_ASSERT(gControl);

  AutoSafeJSContext cx;
  JSAutoRealm ar(cx, xpc::PrivilegedJunkScope());

  RootedValue recordingChildValue(cx);
  if (aRecordingChildId.isSome()) {
    recordingChildValue.setInt32(aRecordingChildId.ref());
>>>>>>> upstream-releases
  }
  if (NS_FAILED(gControl->Initialize(recordingChildValue))) {
    MOZ_CRASH("SetupMiddlemanControl");
  }
}

<<<<<<< HEAD
  if (!MaybeGetNumberProperty(aCx, aObject, gScriptProperty, &mScript) ||
      !MaybeGetNumberProperty(aCx, aObject, gOffsetProperty, &mOffset) ||
      !MaybeGetNumberProperty(aCx, aObject, gFrameIndexProperty,
                              &mFrameIndex)) {
    return false;
||||||| merged common ancestors
  if (!MaybeGetNumberProperty(aCx, aObject, gScriptProperty, &mScript) ||
      !MaybeGetNumberProperty(aCx, aObject, gOffsetProperty, &mOffset) ||
      !MaybeGetNumberProperty(aCx, aObject, gFrameIndexProperty, &mFrameIndex))
  {
    return false;
=======
void ForwardManifestFinished(parent::ChildProcessInfo* aChild,
                             const Message& aMsg) {
  MOZ_RELEASE_ASSERT(gControl);
  const auto& nmsg = static_cast<const ManifestFinishedMessage&>(aMsg);

  AutoSafeJSContext cx;
  JSAutoRealm ar(cx, xpc::PrivilegedJunkScope());

  RootedValue value(cx);
  if (nmsg.BufferSize() &&
      !JS_ParseJSON(cx, nmsg.Buffer(), nmsg.BufferSize(), &value)) {
    MOZ_CRASH("ForwardManifestFinished");
>>>>>>> upstream-releases
  }

  if (NS_FAILED(gControl->ManifestFinished(aChild->GetId(), value))) {
    MOZ_CRASH("ForwardManifestFinished");
  }
}

void BeforeSaveRecording() {
  MOZ_RELEASE_ASSERT(gControl);

<<<<<<< HEAD
// Names of properties which JS code uses to specify the contents of an
// ExecutionPoint.
static const char gCheckpointProperty[] = "checkpoint";
static const char gProgressProperty[] = "progress";
static const char gPositionProperty[] = "position";
||||||| merged common ancestors
// Names of properties which JS code uses to specify the contents of an ExecutionPoint.
static const char gCheckpointProperty[] = "checkpoint";
static const char gProgressProperty[] = "progress";
static const char gPositionProperty[] = "position";
=======
  AutoSafeJSContext cx;
  JSAutoRealm ar(cx, xpc::PrivilegedJunkScope());
>>>>>>> upstream-releases

<<<<<<< HEAD
JSObject* ExecutionPoint::Encode(JSContext* aCx) const {
  RootedObject obj(aCx, JS_NewObject(aCx, nullptr));
  RootedObject position(aCx, mPosition.Encode(aCx));
  if (!obj || !position ||
      !JS_DefineProperty(aCx, obj, gCheckpointProperty, (double)mCheckpoint,
                         JSPROP_ENUMERATE) ||
      !JS_DefineProperty(aCx, obj, gProgressProperty, (double)mProgress,
                         JSPROP_ENUMERATE) ||
      !JS_DefineProperty(aCx, obj, gPositionProperty, position,
                         JSPROP_ENUMERATE)) {
    return nullptr;
||||||| merged common ancestors
JSObject*
ExecutionPoint::Encode(JSContext* aCx) const
{
  RootedObject obj(aCx, JS_NewObject(aCx, nullptr));
  RootedObject position(aCx, mPosition.Encode(aCx));
  if (!obj || !position ||
      !JS_DefineProperty(aCx, obj, gCheckpointProperty,
                         (double) mCheckpoint, JSPROP_ENUMERATE) ||
      !JS_DefineProperty(aCx, obj, gProgressProperty,
                         (double) mProgress, JSPROP_ENUMERATE) ||
      !JS_DefineProperty(aCx, obj, gPositionProperty, position, JSPROP_ENUMERATE))
  {
    return nullptr;
=======
  if (NS_FAILED(gControl->BeforeSaveRecording())) {
    MOZ_CRASH("BeforeSaveRecording");
>>>>>>> upstream-releases
  }
}

<<<<<<< HEAD
bool ExecutionPoint::Decode(JSContext* aCx, HandleObject aObject) {
  RootedValue v(aCx);
  if (!JS_GetProperty(aCx, aObject, gPositionProperty, &v)) {
    return false;
  }
||||||| merged common ancestors
bool
ExecutionPoint::Decode(JSContext* aCx, HandleObject aObject)
{
  RootedValue v(aCx);
  if (!JS_GetProperty(aCx, aObject, gPositionProperty, &v)) {
    return false;
  }
=======
void AfterSaveRecording() {
  MOZ_RELEASE_ASSERT(gControl);

  AutoSafeJSContext cx;
  JSAutoRealm ar(cx, xpc::PrivilegedJunkScope());
>>>>>>> upstream-releases

<<<<<<< HEAD
  RootedObject positionObject(aCx, NonNullObject(aCx, v));
  return positionObject && mPosition.Decode(aCx, positionObject) &&
         GetNumberProperty(aCx, aObject, gCheckpointProperty, &mCheckpoint) &&
         GetNumberProperty(aCx, aObject, gProgressProperty, &mProgress);
||||||| merged common ancestors
  RootedObject positionObject(aCx, NonNullObject(aCx, v));
  return positionObject
      && mPosition.Decode(aCx, positionObject)
      && GetNumberProperty(aCx, aObject, gCheckpointProperty, &mCheckpoint)
      && GetNumberProperty(aCx, aObject, gProgressProperty, &mProgress);
=======
  if (NS_FAILED(gControl->AfterSaveRecording())) {
    MOZ_CRASH("AfterSaveRecording");
  }
>>>>>>> upstream-releases
}

///////////////////////////////////////////////////////////////////////////////
// Middleman Methods
///////////////////////////////////////////////////////////////////////////////

// There can be at most one replay debugger in existence.
static PersistentRootedObject* gReplayDebugger;

static bool Middleman_RegisterReplayDebugger(JSContext* aCx, unsigned aArgc,
                                             Value* aVp) {
  CallArgs args = CallArgsFromVp(aArgc, aVp);
<<<<<<< HEAD

  if (gReplayDebugger) {
    args.rval().setObject(**gReplayDebugger);
    return true;
  }

  RootedObject obj(aCx, NonNullObject(aCx, args.get(0)));
  if (!obj) {
    return false;
||||||| merged common ancestors
  RootedObject obj(aCx, NonNullObject(aCx, args.get(0)));
  if (!obj) {
    return false;
=======

  if (gReplayDebugger) {
    args.rval().setObject(**gReplayDebugger);
    return JS_WrapValue(aCx, args.rval());
>>>>>>> upstream-releases
  }

  RootedObject obj(aCx, RequireObject(aCx, args.get(0)));
  if (!obj) {
    return false;
  }

<<<<<<< HEAD
  gReplayDebugger = new PersistentRootedObject(aCx);
  *gReplayDebugger = obj;
||||||| merged common ancestors
  PersistentRootedObject* root = new PersistentRootedObject(aCx);
  *root = obj;
  gReplayDebuggers.append(root);
=======
  {
    JSAutoRealm ar(aCx, xpc::PrivilegedJunkScope());
>>>>>>> upstream-releases

    RootedValue debuggerValue(aCx, ObjectValue(*obj));
    if (!JS_WrapValue(aCx, &debuggerValue)) {
      return false;
    }

<<<<<<< HEAD
static bool CallReplayDebuggerHook(const char* aMethod) {
  if (!gReplayDebugger) {
    return false;
  }

  AutoSafeJSContext cx;
  JSAutoRealm ar(cx, *gReplayDebugger);
  RootedValue rval(cx);
  if (!JS_CallFunctionName(cx, *gReplayDebugger, aMethod,
                           HandleValueArray::empty(), &rval)) {
    Print("Warning: ReplayDebugger hook %s threw an exception\n", aMethod);
||||||| merged common ancestors
static bool
InvalidateReplayDebuggersAfterUnpause(JSContext* aCx)
{
  RootedValue rval(aCx);
  for (auto root : gReplayDebuggers) {
    JSAutoRealm ar(aCx, *root);
    if (!JS_CallFunctionName(aCx, *root, "invalidateAfterUnpause",
                             HandleValueArray::empty(), &rval))
    {
      return false;
    }
=======
    if (NS_FAILED(gControl->ConnectDebugger(debuggerValue))) {
      JS_ReportErrorASCII(aCx, "ConnectDebugger failed\n");
      return false;
    }
>>>>>>> upstream-releases
  }
<<<<<<< HEAD
  return true;
}

bool DebuggerOnPause() { return CallReplayDebuggerHook("_onPause"); }

void DebuggerOnSwitchChild() { CallReplayDebuggerHook("_onSwitchChild"); }

static bool Middleman_CanRewind(JSContext* aCx, unsigned aArgc, Value* aVp) {
  CallArgs args = CallArgsFromVp(aArgc, aVp);
  args.rval().setBoolean(parent::CanRewind());
  return true;
}
||||||| merged common ancestors
  return true;
}

static bool
Middleman_CanRewind(JSContext* aCx, unsigned aArgc, Value* aVp)
{
  CallArgs args = CallArgsFromVp(aArgc, aVp);
  args.rval().setBoolean(parent::CanRewind());
  return true;
}
=======
>>>>>>> upstream-releases

<<<<<<< HEAD
static bool Middleman_Resume(JSContext* aCx, unsigned aArgc, Value* aVp) {
  CallArgs args = CallArgsFromVp(aArgc, aVp);
  bool forward = ToBoolean(args.get(0));

  parent::Resume(forward);
||||||| merged common ancestors
static bool
Middleman_Resume(JSContext* aCx, unsigned aArgc, Value* aVp)
{
  CallArgs args = CallArgsFromVp(aArgc, aVp);
  bool forward = ToBoolean(args.get(0));

  if (!InvalidateReplayDebuggersAfterUnpause(aCx)) {
    return false;
  }

  parent::Resume(forward);
=======
  // Who knows what values are being passed here.  Play it safe and do
  // CheckedUnwrapDynamic.
  obj = ::js::CheckedUnwrapDynamic(obj, aCx);
  if (!obj) {
    ::js::ReportAccessDenied(aCx);
    return false;
  }

  gReplayDebugger = new PersistentRootedObject(aCx);
  *gReplayDebugger = obj;
>>>>>>> upstream-releases

  args.rval().setUndefined();
  return true;
}

<<<<<<< HEAD
static bool Middleman_TimeWarp(JSContext* aCx, unsigned aArgc, Value* aVp) {
||||||| merged common ancestors
static bool
Middleman_TimeWarp(JSContext* aCx, unsigned aArgc, Value* aVp)
{
=======
static bool Middleman_CanRewind(JSContext* aCx, unsigned aArgc, Value* aVp) {
>>>>>>> upstream-releases
  CallArgs args = CallArgsFromVp(aArgc, aVp);
<<<<<<< HEAD
  RootedObject targetObject(aCx, NonNullObject(aCx, args.get(0)));
  if (!targetObject) {
    return false;
  }

  ExecutionPoint target;
  if (!target.Decode(aCx, targetObject)) {
    return false;
  }

  parent::TimeWarp(target);

  args.rval().setUndefined();
||||||| merged common ancestors
  RootedObject targetObject(aCx, NonNullObject(aCx, args.get(0)));
  if (!targetObject) {
    return false;
  }

  ExecutionPoint target;
  if (!target.Decode(aCx, targetObject)) {
    return false;
  }

  if (!InvalidateReplayDebuggersAfterUnpause(aCx)) {
    return false;
  }

  parent::TimeWarp(target);

  args.rval().setUndefined();
=======
  args.rval().setBoolean(parent::CanRewind());
>>>>>>> upstream-releases
  return true;
}

<<<<<<< HEAD
static bool Middleman_SendRequest(JSContext* aCx, unsigned aArgc, Value* aVp) {
  CallArgs args = CallArgsFromVp(aArgc, aVp);
  RootedObject requestObject(aCx, NonNullObject(aCx, args.get(0)));
  if (!requestObject) {
    return false;
  }

  CharBuffer requestBuffer;
  if (!ToJSONMaybeSafely(aCx, requestObject, FillCharBufferCallback,
                         &requestBuffer)) {
    return false;
  }

  CharBuffer responseBuffer;
  parent::SendRequest(requestBuffer, &responseBuffer);

  return JS_ParseJSON(aCx, responseBuffer.begin(), responseBuffer.length(),
                      args.rval());
}

static bool Middleman_AddBreakpoint(JSContext* aCx, unsigned aArgc,
                                    Value* aVp) {
||||||| merged common ancestors
static bool
Middleman_Pause(JSContext* aCx, unsigned aArgc, Value* aVp)
{
  CallArgs args = CallArgsFromVp(aArgc, aVp);

  parent::Pause();

  args.rval().setUndefined();
  return true;
}

static bool
Middleman_SendRequest(JSContext* aCx, unsigned aArgc, Value* aVp)
{
  CallArgs args = CallArgsFromVp(aArgc, aVp);
  RootedObject requestObject(aCx, NonNullObject(aCx, args.get(0)));
  if (!requestObject) {
    return false;
  }

  CharBuffer requestBuffer;
  if (!ToJSONMaybeSafely(aCx, requestObject, FillCharBufferCallback, &requestBuffer)) {
    return false;
  }

  CharBuffer responseBuffer;
  parent::SendRequest(requestBuffer, &responseBuffer);

  return JS_ParseJSON(aCx, responseBuffer.begin(), responseBuffer.length(), args.rval());
}

struct InstalledBreakpoint
{
  PersistentRootedObject mHandler;
  BreakpointPosition mPosition;

  InstalledBreakpoint(JSContext* aCx, JSObject* aHandler, const BreakpointPosition& aPosition)
    : mHandler(aCx, aHandler), mPosition(aPosition)
  {}
};
static StaticInfallibleVector<InstalledBreakpoint*> gBreakpoints;

static bool
Middleman_SetBreakpoint(JSContext* aCx, unsigned aArgc, Value* aVp)
{
=======
static bool Middleman_SpawnReplayingChild(JSContext* aCx, unsigned aArgc,
                                          Value* aVp) {
  CallArgs args = CallArgsFromVp(aArgc, aVp);

  size_t id = parent::SpawnReplayingChild();
  args.rval().setInt32(id);
  return true;
}

static bool Middleman_SendManifest(JSContext* aCx, unsigned aArgc, Value* aVp) {
>>>>>>> upstream-releases
  CallArgs args = CallArgsFromVp(aArgc, aVp);

<<<<<<< HEAD
  RootedObject positionObject(aCx, NonNullObject(aCx, args.get(0)));
  if (!positionObject) {
||||||| merged common ancestors
  RootedObject handler(aCx, NonNullObject(aCx, args.get(0)));
  RootedObject positionObject(aCx, NonNullObject(aCx, args.get(1)));
  if (!handler || !positionObject) {
    return false;
  }

  handler = ::js::CheckedUnwrap(handler);
  if (!handler) {
    ::js::ReportAccessDenied(aCx);
=======
  RootedObject manifestObject(aCx, RequireObject(aCx, args.get(1)));
  if (!manifestObject) {
    return false;
  }

  CharBuffer manifestBuffer;
  if (!ToJSONMaybeSafely(aCx, manifestObject, FillCharBufferCallback,
                         &manifestBuffer)) {
>>>>>>> upstream-releases
    return false;
  }

  parent::ChildProcessInfo* child = GetChildById(aCx, args.get(0));
  if (!child) {
    return false;
  }

<<<<<<< HEAD
  parent::AddBreakpoint(position);

  args.rval().setUndefined();
  return true;
}

/* static */ bool Middleman_ClearBreakpoints(JSContext* aCx, unsigned aArgc,
                                             Value* aVp) {
  CallArgs args = CallArgsFromVp(aArgc, aVp);

  parent::ClearBreakpoints();
||||||| merged common ancestors
  size_t breakpointId;
  for (breakpointId = 0; breakpointId < gBreakpoints.length(); breakpointId++) {
    if (!gBreakpoints[breakpointId]) {
      break;
    }
  }
  if (breakpointId == gBreakpoints.length()) {
    gBreakpoints.append(nullptr);
  }

  gBreakpoints[breakpointId] = new InstalledBreakpoint(aCx, handler, position);

  parent::SetBreakpoint(breakpointId, position);

  args.rval().setInt32(breakpointId);
  return true;
}

bool
HitBreakpoint(JSContext* aCx, size_t aId)
{
  InstalledBreakpoint* breakpoint = gBreakpoints[aId];
  MOZ_RELEASE_ASSERT(breakpoint);

  JSAutoRealm ar(aCx, breakpoint->mHandler);

  RootedValue handlerValue(aCx, ObjectValue(*breakpoint->mHandler));
  RootedValue rval(aCx);
  return JS_CallFunctionValue(aCx, nullptr, handlerValue,
                              HandleValueArray::empty(), &rval)
      // The replaying process will resume after this hook returns, if it
      // hasn't already been explicitly resumed.
      && InvalidateReplayDebuggersAfterUnpause(aCx);
}

/* static */ bool
Middleman_ClearBreakpoint(JSContext* aCx, unsigned aArgc, Value* aVp)
{
  CallArgs args = CallArgsFromVp(aArgc, aVp);
  if (!args.get(0).isNumber()) {
    JS_ReportErrorASCII(aCx, "Bad breakpoint ID");
    return false;
  }

  size_t breakpointId = (size_t) args.get(0).toNumber();
  if (breakpointId >= gBreakpoints.length() || !gBreakpoints[breakpointId]) {
    JS_ReportErrorASCII(aCx, "Bad breakpoint ID");
    return false;
  }

  delete gBreakpoints[breakpointId];
  gBreakpoints[breakpointId] = nullptr;

  parent::SetBreakpoint(breakpointId, BreakpointPosition());
=======
  ManifestStartMessage* msg = ManifestStartMessage::New(
      manifestBuffer.begin(), manifestBuffer.length());
  child->SendMessage(std::move(*msg));
  free(msg);
>>>>>>> upstream-releases

  args.rval().setUndefined();
  return true;
}

<<<<<<< HEAD
static bool Middleman_MaybeSwitchToReplayingChild(JSContext* aCx,
                                                  unsigned aArgc, Value* aVp) {
  CallArgs args = CallArgsFromVp(aArgc, aVp);

  parent::MaybeSwitchToReplayingChild();

  args.rval().setUndefined();
  return true;
}

static bool Middleman_HadRepaint(JSContext* aCx, unsigned aArgc, Value* aVp) {
||||||| merged common ancestors
static bool
Middleman_MaybeSwitchToReplayingChild(JSContext* aCx, unsigned aArgc, Value* aVp)
{
  CallArgs args = CallArgsFromVp(aArgc, aVp);

  parent::MaybeSwitchToReplayingChild();

  args.rval().setUndefined();
  return true;
}

static bool
Middleman_HadRepaint(JSContext* aCx, unsigned aArgc, Value* aVp)
{
=======
static bool Middleman_HadRepaint(JSContext* aCx, unsigned aArgc, Value* aVp) {
>>>>>>> upstream-releases
  CallArgs args = CallArgsFromVp(aArgc, aVp);

  if (!args.get(0).isNumber() || !args.get(1).isNumber()) {
    JS_ReportErrorASCII(aCx, "Bad width/height");
    return false;
  }

  size_t width = args.get(0).toNumber();
  size_t height = args.get(1).toNumber();

  PaintMessage message(InvalidCheckpointId, width, height);
  parent::UpdateGraphicsInUIProcess(&message);

  args.rval().setUndefined();
  return true;
}

static bool Middleman_HadRepaintFailure(JSContext* aCx, unsigned aArgc,
                                        Value* aVp) {
  CallArgs args = CallArgsFromVp(aArgc, aVp);

  parent::UpdateGraphicsInUIProcess(nullptr);

  args.rval().setUndefined();
  return true;
}

<<<<<<< HEAD
static bool Middleman_ChildIsRecording(JSContext* aCx, unsigned aArgc,
                                       Value* aVp) {
||||||| merged common ancestors
static bool
Middleman_RecordingPosition(JSContext* aCx, unsigned aArgc, Value* aVp)
{
=======
static bool Middleman_InRepaintStressMode(JSContext* aCx, unsigned aArgc,
                                          Value* aVp) {
>>>>>>> upstream-releases
  CallArgs args = CallArgsFromVp(aArgc, aVp);
  args.rval().setBoolean(parent::ActiveChildIsRecording());
  return true;
}

<<<<<<< HEAD
static bool Middleman_MarkExplicitPause(JSContext* aCx, unsigned aArgc,
                                        Value* aVp) {
  CallArgs args = CallArgsFromVp(aArgc, aVp);
||||||| merged common ancestors
  Maybe<double> recordingPosition = parent::GetRecordingPosition();
=======
  args.rval().setBoolean(parent::InRepaintStressMode());
  return true;
}
>>>>>>> upstream-releases

<<<<<<< HEAD
  parent::MarkActiveChildExplicitPause();

  args.rval().setUndefined();
  return true;
}

static bool Middleman_WaitUntilPaused(JSContext* aCx, unsigned aArgc,
                                      Value* aVp) {
  CallArgs args = CallArgsFromVp(aArgc, aVp);

  parent::WaitUntilActiveChildIsPaused();

  args.rval().setUndefined();
  return true;
}

static bool Middleman_PositionSubsumes(JSContext* aCx, unsigned aArgc,
                                       Value* aVp) {
  CallArgs args = CallArgsFromVp(aArgc, aVp);

  RootedObject firstPositionObject(aCx, NonNullObject(aCx, args.get(0)));
  if (!firstPositionObject) {
    return false;
  }

  BreakpointPosition firstPosition;
  if (!firstPosition.Decode(aCx, firstPositionObject)) {
    return false;
  }

  RootedObject secondPositionObject(aCx, NonNullObject(aCx, args.get(1)));
  if (!secondPositionObject) {
    return false;
||||||| merged common ancestors
  if (recordingPosition.isSome()) {
    args.rval().setNumber(recordingPosition.ref());
  } else {
    args.rval().setUndefined();
=======
// Recording children can idle indefinitely while waiting for input, without
// creating a checkpoint. If this might be a problem, this method induces the
// child to create a new checkpoint and pause.
static void MaybeCreateCheckpointInChild(parent::ChildProcessInfo* aChild) {
  if (aChild->IsRecording() && !aChild->IsPaused()) {
    aChild->SendMessage(CreateCheckpointMessage());
>>>>>>> upstream-releases
  }
<<<<<<< HEAD

  BreakpointPosition secondPosition;
  if (!secondPosition.Decode(aCx, secondPositionObject)) {
    return false;
  }

  args.rval().setBoolean(firstPosition.Subsumes(secondPosition));
||||||| merged common ancestors
=======
}

static bool Middleman_WaitUntilPaused(JSContext* aCx, unsigned aArgc,
                                      Value* aVp) {
  CallArgs args = CallArgsFromVp(aArgc, aVp);

  parent::ChildProcessInfo* child = GetChildById(aCx, args.get(0),
                                                 /* aAllowUnpaused = */ true);
  if (!child) {
    return false;
  }

  if (ToBoolean(args.get(1))) {
    MaybeCreateCheckpointInChild(child);
  }

  child->WaitUntilPaused();

  args.rval().setUndefined();
>>>>>>> upstream-releases
  return true;
}

///////////////////////////////////////////////////////////////////////////////
// Devtools Sandbox
///////////////////////////////////////////////////////////////////////////////

static StaticRefPtr<rrIReplay> gReplay;

// URL of the root script that runs when recording/replaying.
#define ReplayScriptURL "resource://devtools/server/actors/replay/replay.js"

<<<<<<< HEAD
// Whether to expose chrome:// and resource:// scripts to the debugger.
static bool gIncludeSystemScripts;

void SetupDevtoolsSandbox() {
  MOZ_RELEASE_ASSERT(!gDevtoolsSandbox);
||||||| merged common ancestors
void
SetupDevtoolsSandbox()
{
  MOZ_RELEASE_ASSERT(!gDevtoolsSandbox);
=======
// Whether to expose chrome:// and resource:// scripts to the debugger.
static bool gIncludeSystemScripts;
>>>>>>> upstream-releases

static void InitializeScriptHits();

void SetupDevtoolsSandbox() {
  MOZ_RELEASE_ASSERT(!gReplay);

<<<<<<< HEAD
  xpc::SandboxOptions options;
  options.sandboxName.AssignLiteral("Record/Replay Devtools Sandbox");
  options.invisibleToDebugger = true;
  RootedValue v(cx);
  nsresult rv =
      CreateSandboxObject(cx, &v, nsXPConnect::SystemPrincipal(), options);
  MOZ_RELEASE_ASSERT(NS_SUCCEEDED(rv));
||||||| merged common ancestors
  xpc::SandboxOptions options;
  options.sandboxName.AssignLiteral("Record/Replay Devtools Sandbox");
  options.invisibleToDebugger = true;
  RootedValue v(cx);
  nsresult rv = CreateSandboxObject(cx, &v, nsXPConnect::SystemPrincipal(), options);
  MOZ_RELEASE_ASSERT(NS_SUCCEEDED(rv));
=======
  nsCOMPtr<rrIReplay> replay = do_ImportModule(ReplayScriptURL);
  gReplay = replay.forget();
  ClearOnShutdown(&gReplay);
>>>>>>> upstream-releases

  MOZ_RELEASE_ASSERT(gReplay);

  gIncludeSystemScripts =
      Preferences::GetBool("devtools.recordreplay.includeSystemScripts");

<<<<<<< HEAD
  ErrorResult er;
  dom::GlobalObject global(cx, *gDevtoolsSandbox);
  RootedObject obj(cx);
  dom::ChromeUtils::Import(global, NS_LITERAL_STRING(ReplayScriptURL),
                           dom::Optional<HandleObject>(), &obj, er);
  MOZ_RELEASE_ASSERT(!er.Failed());

  gIncludeSystemScripts =
      Preferences::GetBool("devtools.recordreplay.includeSystemScripts");
||||||| merged common ancestors
  ErrorResult er;
  dom::GlobalObject global(cx, *gDevtoolsSandbox);
  RootedObject obj(cx);
  dom::ChromeUtils::Import(global, NS_LITERAL_STRING(ReplayScriptURL),
                           dom::Optional<HandleObject>(), &obj, er);
  MOZ_RELEASE_ASSERT(!er.Failed());
=======
  InitializeScriptHits();
>>>>>>> upstream-releases
}

bool IsInitialized() { return !!gReplay; }

extern "C" {

MOZ_EXPORT bool RecordReplayInterface_ShouldUpdateProgressCounter(
    const char* aURL) {
  // Progress counters are only updated for scripts which are exposed to the
  // debugger. The devtools timeline is based on progress values and we don't
  // want gaps on the timeline which users can't seek to.
  if (gIncludeSystemScripts) {
    // Always exclude ReplayScriptURL. Scripts in this file are internal to the
    // record/replay infrastructure and run non-deterministically between
    // recording and replaying.
    return aURL && strcmp(aURL, ReplayScriptURL);
  } else {
    return aURL && strncmp(aURL, "resource:", 9) && strncmp(aURL, "chrome:", 7);
  }
}

}  // extern "C"

#undef ReplayScriptURL

<<<<<<< HEAD
void ProcessRequest(const char16_t* aRequest, size_t aRequestLength,
                    CharBuffer* aResponse) {
||||||| merged common ancestors
void
ProcessRequest(const char16_t* aRequest, size_t aRequestLength, CharBuffer* aResponse)
{
=======
void ManifestStart(const CharBuffer& aContents) {
>>>>>>> upstream-releases
  AutoDisallowThreadEvents disallow;
  AutoSafeJSContext cx;
  JSAutoRealm ar(cx, xpc::PrivilegedJunkScope());

  RootedValue value(cx);
  if (!JS_ParseJSON(cx, aContents.begin(), aContents.length(), &value)) {
    MOZ_CRASH("ManifestStart: ParseJSON failed");
  }

  if (NS_FAILED(gReplay->ManifestStart(value))) {
    MOZ_CRASH("ManifestStart: Handler failed");
  }

  // Processing the manifest may have called into MaybeDivergeFromRecording.
  // If it did so, we should already have finished any processing that required
  // diverging from the recording. Don't tolerate future events that
  // would otherwise cause us to rewind to the last checkpoint.
  DisallowUnhandledDivergeFromRecording();
}

void BeforeCheckpoint() {
  if (!IsInitialized()) {
    SetupDevtoolsSandbox();
  }

<<<<<<< HEAD
  RootedObject responseObject(cx, &responseValue.toObject());
  if (!ToJSONMaybeSafely(cx, responseObject, FillCharBufferCallback,
                         aResponse)) {
    MOZ_CRASH("ProcessRequest: ToJSONMaybeSafely failed");
  }
}

void EnsurePositionHandler(const BreakpointPosition& aPosition) {
||||||| merged common ancestors
  RootedObject responseObject(cx, &responseValue.toObject());
  if (!ToJSONMaybeSafely(cx, responseObject, FillCharBufferCallback, aResponse)) {
    MOZ_CRASH("ProcessRequest: ToJSONMaybeSafely failed");
  }
}

void
EnsurePositionHandler(const BreakpointPosition& aPosition)
{
=======
>>>>>>> upstream-releases
  AutoDisallowThreadEvents disallow;
  AutoSafeJSContext cx;
  JSAutoRealm ar(cx, xpc::PrivilegedJunkScope());

  if (NS_FAILED(gReplay->BeforeCheckpoint())) {
    MOZ_CRASH("BeforeCheckpoint");
  }
}

<<<<<<< HEAD
void ClearPositionHandlers() {
||||||| merged common ancestors
void
ClearPositionHandlers()
{
=======
void AfterCheckpoint(size_t aCheckpoint, bool aRestoredCheckpoint) {
>>>>>>> upstream-releases
  AutoDisallowThreadEvents disallow;
  AutoSafeJSContext cx;
  JSAutoRealm ar(cx, xpc::PrivilegedJunkScope());

  if (NS_FAILED(gReplay->AfterCheckpoint(aCheckpoint, aRestoredCheckpoint))) {
    MOZ_CRASH("AfterCheckpoint");
  }
}

<<<<<<< HEAD
void ClearPausedState() {
  AutoDisallowThreadEvents disallow;
  AutoSafeJSContext cx;
  JSAutoRealm ar(cx, *gDevtoolsSandbox);
||||||| merged common ancestors
void
ClearPausedState()
{
  AutoDisallowThreadEvents disallow;
  AutoSafeJSContext cx;
  JSAutoRealm ar(cx, *gDevtoolsSandbox);
=======
static ProgressCounter gProgressCounter;
>>>>>>> upstream-releases

extern "C" {

<<<<<<< HEAD
Maybe<BreakpointPosition> GetEntryPosition(
    const BreakpointPosition& aPosition) {
  AutoDisallowThreadEvents disallow;
  AutoSafeJSContext cx;
  JSAutoRealm ar(cx, *gDevtoolsSandbox);
||||||| merged common ancestors
Maybe<BreakpointPosition>
GetEntryPosition(const BreakpointPosition& aPosition)
{
  AutoDisallowThreadEvents disallow;
  AutoSafeJSContext cx;
  JSAutoRealm ar(cx, *gDevtoolsSandbox);
=======
MOZ_EXPORT ProgressCounter* RecordReplayInterface_ExecutionProgressCounter() {
  return &gProgressCounter;
}
>>>>>>> upstream-releases

MOZ_EXPORT ProgressCounter RecordReplayInterface_NewTimeWarpTarget() {
  if (AreThreadEventsDisallowed()) {
    return 0;
  }

  // NewTimeWarpTarget() must be called at consistent points between recording
  // and replaying.
  RecordReplayAssert("NewTimeWarpTarget");

  if (!IsInitialized()) {
    return 0;
  }

  AutoDisallowThreadEvents disallow;
  AutoSafeJSContext cx;
  JSAutoRealm ar(cx, xpc::PrivilegedJunkScope());

  int32_t counter = 0;
  if (NS_FAILED(gReplay->NewTimeWarpTarget(&counter))) {
    MOZ_CRASH("NewTimeWarpTarget");
  }

  return counter;
}

}  // extern "C"

///////////////////////////////////////////////////////////////////////////////
// Replaying process content
///////////////////////////////////////////////////////////////////////////////

struct ContentInfo {
  const void* mToken;
  char* mURL;
  char* mContentType;
  InfallibleVector<char> mContent8;
  InfallibleVector<char16_t> mContent16;

  ContentInfo(const void* aToken, const char* aURL, const char* aContentType)
      : mToken(aToken),
        mURL(strdup(aURL)),
        mContentType(strdup(aContentType)) {}

  ContentInfo(ContentInfo&& aOther)
      : mToken(aOther.mToken),
        mURL(aOther.mURL),
        mContentType(aOther.mContentType),
        mContent8(std::move(aOther.mContent8)),
        mContent16(std::move(aOther.mContent16)) {
    aOther.mURL = nullptr;
    aOther.mContentType = nullptr;
  }

  ~ContentInfo() {
    free(mURL);
    free(mContentType);
  }

  size_t Length() {
    MOZ_RELEASE_ASSERT(!mContent8.length() || !mContent16.length());
    return mContent8.length() ? mContent8.length() : mContent16.length();
  }
};

// All content that has been parsed so far. Protected by child::gMonitor.
static StaticInfallibleVector<ContentInfo> gContent;

extern "C" {

MOZ_EXPORT void RecordReplayInterface_BeginContentParse(
    const void* aToken, const char* aURL, const char* aContentType) {
  MOZ_RELEASE_ASSERT(IsRecordingOrReplaying());
  MOZ_RELEASE_ASSERT(aToken);

  RecordReplayAssert("BeginContentParse %s", aURL);

  MonitorAutoLock lock(*child::gMonitor);
  for (ContentInfo& info : gContent) {
    MOZ_RELEASE_ASSERT(info.mToken != aToken);
  }
  gContent.emplaceBack(aToken, aURL, aContentType);
}

MOZ_EXPORT void RecordReplayInterface_AddContentParseData8(
    const void* aToken, const Utf8Unit* aUtf8Buffer, size_t aLength) {
  MOZ_RELEASE_ASSERT(IsRecordingOrReplaying());
  MOZ_RELEASE_ASSERT(aToken);

  RecordReplayAssert("AddContentParseData8ForRecordReplay %d", (int)aLength);

  MonitorAutoLock lock(*child::gMonitor);
  for (ContentInfo& info : gContent) {
    if (info.mToken == aToken) {
      info.mContent8.append(reinterpret_cast<const char*>(aUtf8Buffer),
                            aLength);
      return;
    }
  }
  MOZ_CRASH("Unknown content parse token");
}

MOZ_EXPORT void RecordReplayInterface_AddContentParseData16(
    const void* aToken, const char16_t* aBuffer, size_t aLength) {
  MOZ_RELEASE_ASSERT(IsRecordingOrReplaying());
  MOZ_RELEASE_ASSERT(aToken);

  RecordReplayAssert("AddContentParseData16ForRecordReplay %d", (int)aLength);

  MonitorAutoLock lock(*child::gMonitor);
  for (ContentInfo& info : gContent) {
    if (info.mToken == aToken) {
      info.mContent16.append(aBuffer, aLength);
      return;
    }
  }
  MOZ_CRASH("Unknown content parse token");
}

MOZ_EXPORT void RecordReplayInterface_EndContentParse(const void* aToken) {
  MOZ_RELEASE_ASSERT(IsRecordingOrReplaying());
  MOZ_RELEASE_ASSERT(aToken);

  MonitorAutoLock lock(*child::gMonitor);
  for (ContentInfo& info : gContent) {
    if (info.mToken == aToken) {
      info.mToken = nullptr;
      return;
    }
  }
  MOZ_CRASH("Unknown content parse token");
}

}  // extern "C"

static bool FetchContent(JSContext* aCx, HandleString aURL,
                         MutableHandleString aContentType,
                         MutableHandleString aContent) {
  MonitorAutoLock lock(*child::gMonitor);

  // Find the longest content parse data with this URL. This is to handle inline
  // script elements in HTML pages, where we will see content parses for both
  // the HTML itself and for each inline script.
  ContentInfo* best = nullptr;
  for (ContentInfo& info : gContent) {
    if (JS_FlatStringEqualsAscii(JS_ASSERT_STRING_IS_FLAT(aURL), info.mURL)) {
      if (!best || info.Length() > best->Length()) {
        best = &info;
      }
    }
  }

  if (best) {
    aContentType.set(JS_NewStringCopyZ(aCx, best->mContentType));

    MOZ_ASSERT(best->mContent8.length() == 0 || best->mContent16.length() == 0,
               "should have content data of only one type");

    aContent.set(best->mContent8.length() > 0
                     ? JS_NewStringCopyUTF8N(
                           aCx, JS::UTF8Chars(best->mContent8.begin(),
                                              best->mContent8.length()))
                     : JS_NewUCStringCopyN(aCx, best->mContent16.begin(),
                                           best->mContent16.length()));
  } else {
    aContentType.set(JS_NewStringCopyZ(aCx, "text/plain"));
    aContent.set(
        JS_NewStringCopyZ(aCx, "Could not find record/replay content"));
  }

  return aContentType && aContent;
}

///////////////////////////////////////////////////////////////////////////////
// Recording/Replaying Methods
///////////////////////////////////////////////////////////////////////////////

static bool RecordReplay_AreThreadEventsDisallowed(JSContext* aCx,
                                                   unsigned aArgc, Value* aVp) {
  CallArgs args = CallArgsFromVp(aArgc, aVp);
  args.rval().setBoolean(AreThreadEventsDisallowed());
  return true;
}

<<<<<<< HEAD
static bool RecordReplay_MaybeDivergeFromRecording(JSContext* aCx,
                                                   unsigned aArgc, Value* aVp) {
||||||| merged common ancestors
static bool
RecordReplay_MaybeDivergeFromRecording(JSContext* aCx, unsigned aArgc, Value* aVp)
{
=======
static bool RecordReplay_DivergeFromRecording(JSContext* aCx, unsigned aArgc,
                                              Value* aVp) {
>>>>>>> upstream-releases
  CallArgs args = CallArgsFromVp(aArgc, aVp);
  DivergeFromRecording();
  args.rval().setUndefined();
  return true;
}

<<<<<<< HEAD
static bool RecordReplay_AdvanceProgressCounter(JSContext* aCx, unsigned aArgc,
                                                Value* aVp) {
||||||| merged common ancestors
static bool
RecordReplay_AdvanceProgressCounter(JSContext* aCx, unsigned aArgc, Value* aVp)
{
=======
static bool RecordReplay_ProgressCounter(JSContext* aCx, unsigned aArgc,
                                         Value* aVp) {
  CallArgs args = CallArgsFromVp(aArgc, aVp);
  args.rval().setNumber((double)gProgressCounter);
  return true;
}

static bool RecordReplay_AdvanceProgressCounter(JSContext* aCx, unsigned aArgc,
                                                Value* aVp) {
>>>>>>> upstream-releases
  CallArgs args = CallArgsFromVp(aArgc, aVp);
  AdvanceExecutionProgressCounter();
  args.rval().setUndefined();
  return true;
}

<<<<<<< HEAD
static bool RecordReplay_ShouldUpdateProgressCounter(JSContext* aCx,
                                                     unsigned aArgc,
                                                     Value* aVp) {
  CallArgs args = CallArgsFromVp(aArgc, aVp);

  if (args.get(0).isNull()) {
    args.rval().setBoolean(ShouldUpdateProgressCounter(nullptr));
  } else {
    if (!args.get(0).isString()) {
      JS_ReportErrorASCII(aCx, "Expected string or null as first argument");
      return false;
    }

    JSString* str = args.get(0).toString();
    size_t len = JS_GetStringLength(str);

    nsAutoString chars;
    chars.SetLength(len);
    if (!JS_CopyStringChars(aCx, Range<char16_t>(chars.BeginWriting(), len),
                            str)) {
      return false;
    }

    NS_ConvertUTF16toUTF8 utf8(chars);
    args.rval().setBoolean(ShouldUpdateProgressCounter(utf8.get()));
  }

  return true;
}

static bool RecordReplay_PositionHit(JSContext* aCx, unsigned aArgc,
                                     Value* aVp) {
||||||| merged common ancestors
static bool
RecordReplay_PositionHit(JSContext* aCx, unsigned aArgc, Value* aVp)
{
=======
static bool RecordReplay_ShouldUpdateProgressCounter(JSContext* aCx,
                                                     unsigned aArgc,
                                                     Value* aVp) {
>>>>>>> upstream-releases
  CallArgs args = CallArgsFromVp(aArgc, aVp);

  if (args.get(0).isNull()) {
    args.rval().setBoolean(ShouldUpdateProgressCounter(nullptr));
  } else {
    if (!args.get(0).isString()) {
      JS_ReportErrorASCII(aCx, "Expected string or null as first argument");
      return false;
    }

    JSString* str = args.get(0).toString();
    size_t len = JS_GetStringLength(str);

    nsAutoString chars;
    chars.SetLength(len);
    if (!JS_CopyStringChars(aCx, Range<char16_t>(chars.BeginWriting(), len),
                            str)) {
      return false;
    }

    NS_ConvertUTF16toUTF8 utf8(chars);
    args.rval().setBoolean(ShouldUpdateProgressCounter(utf8.get()));
  }

  return true;
}

static bool RecordReplay_ManifestFinished(JSContext* aCx, unsigned aArgc,
                                          Value* aVp) {
  CallArgs args = CallArgsFromVp(aArgc, aVp);

  CharBuffer responseBuffer;
  if (args.hasDefined(0)) {
    RootedObject responseObject(aCx, RequireObject(aCx, args.get(0)));
    if (!responseObject) {
      return false;
    }

    if (!ToJSONMaybeSafely(aCx, responseObject, FillCharBufferCallback,
                           &responseBuffer)) {
      return false;
    }
  }

  child::ManifestFinished(responseBuffer);

  args.rval().setUndefined();
  return true;
}

static bool RecordReplay_ResumeExecution(JSContext* aCx, unsigned aArgc,
                                         Value* aVp) {
  CallArgs args = CallArgsFromVp(aArgc, aVp);

  ResumeExecution();

  args.rval().setUndefined();
  return true;
}

static bool RecordReplay_RestoreCheckpoint(JSContext* aCx, unsigned aArgc,
                                           Value* aVp) {
  CallArgs args = CallArgsFromVp(aArgc, aVp);

  if (!args.get(0).isNumber()) {
    JS_ReportErrorASCII(aCx, "Bad checkpoint ID");
    return false;
  }

  size_t checkpoint = args.get(0).toNumber();
  if (!HasSavedCheckpoint(checkpoint)) {
    JS_ReportErrorASCII(aCx, "Only saved checkpoints can be restored");
    return false;
  }

  RestoreCheckpointAndResume(checkpoint);

  JS_ReportErrorASCII(aCx, "Unreachable!");
  return false;
}

// The total amount of time this process has spent idling.
static double gIdleTimeTotal;

// When recording and we are idle, the time when we became idle.
static double gIdleTimeStart;

void BeginIdleTime() {
  if (IsRecording() && Thread::CurrentIsMainThread()) {
    MOZ_RELEASE_ASSERT(!gIdleTimeStart);
    gIdleTimeStart = CurrentTime();
  }
}

void EndIdleTime() {
  if (IsRecording() && Thread::CurrentIsMainThread()) {
    MOZ_RELEASE_ASSERT(!!gIdleTimeStart);
    gIdleTimeTotal += CurrentTime() - gIdleTimeStart;
    gIdleTimeStart = 0;
  }
}

static bool RecordReplay_CurrentExecutionTime(JSContext* aCx, unsigned aArgc,
                                              Value* aVp) {
  CallArgs args = CallArgsFromVp(aArgc, aVp);

  // Get a current timestamp biased by the amount of time the process has spent
  // idling. Comparing these timestamps gives the elapsed non-idle time between
  // them.
  args.rval().setNumber((CurrentTime() - gIdleTimeTotal) / 1000.0);
  return true;
}

static bool RecordReplay_FlushRecording(JSContext* aCx, unsigned aArgc,
                                        Value* aVp) {
  CallArgs args = CallArgsFromVp(aArgc, aVp);

  FlushRecording();

  args.rval().setUndefined();
  return true;
}

<<<<<<< HEAD
static bool RecordReplay_GetContent(JSContext* aCx, unsigned aArgc,
                                    Value* aVp) {
||||||| merged common ancestors
static bool
RecordReplay_GetContent(JSContext* aCx, unsigned aArgc, Value* aVp)
{
=======
static bool RecordReplay_SetMainChild(JSContext* aCx, unsigned aArgc,
                                      Value* aVp) {
>>>>>>> upstream-releases
  CallArgs args = CallArgsFromVp(aArgc, aVp);

  SetMainChild();
  size_t endpoint = RecordingEndpoint();

  args.rval().setInt32(endpoint);
  return true;
}

static bool RecordReplay_SaveCheckpoint(JSContext* aCx, unsigned aArgc,
                                        Value* aVp) {
  CallArgs args = CallArgsFromVp(aArgc, aVp);

  if (!args.get(0).isNumber()) {
    JS_ReportErrorASCII(aCx, "Bad checkpoint ID");
    return false;
  }

<<<<<<< HEAD
  RootedObject obj(aCx, JS_NewObject(aCx, nullptr));
  if (!obj ||
      !JS_DefineProperty(aCx, obj, "contentType", contentType,
                         JSPROP_ENUMERATE) ||
      !JS_DefineProperty(aCx, obj, "content", content, JSPROP_ENUMERATE)) {
||||||| merged common ancestors
  RootedObject obj(aCx, JS_NewObject(aCx, nullptr));
  if (!obj ||
      !JS_DefineProperty(aCx, obj, "contentType", contentType, JSPROP_ENUMERATE) ||
      !JS_DefineProperty(aCx, obj, "content", content, JSPROP_ENUMERATE))
  {
=======
  size_t checkpoint = args.get(0).toNumber();
  if (checkpoint <= GetLastCheckpoint()) {
    JS_ReportErrorASCII(aCx, "Can't save checkpoint in the past");
>>>>>>> upstream-releases
    return false;
  }

  SetSaveCheckpoint(checkpoint, true);

  args.rval().setUndefined();
  return true;
}

<<<<<<< HEAD
static bool RecordReplay_CurrentExecutionPoint(JSContext* aCx, unsigned aArgc,
                                               Value* aVp) {
||||||| merged common ancestors
static bool
RecordReplay_CurrentExecutionPoint(JSContext* aCx, unsigned aArgc, Value* aVp)
{
=======
// How many hits on a script location we will precisely track for a checkpoint.
static const size_t MaxHitsPerCheckpoint = 10;

struct ScriptHitInfo {
  // Information about a location where a script offset has been hit, or an
  // aggregate set of hits.
  struct ScriptHit {
    // The most recent checkpoint prior to the hit.
    uint32_t mCheckpoint;

    // Index of the frame where the hit occurred, or UINT32_MAX if this
    // represents an aggregate set of hits after the checkpoint.
    uint32_t mFrameIndex;

    // Progress counter when the hit occurred, invalid if this represents an
    // aggregate set of hits.
    ProgressCounter mProgress;

    explicit ScriptHit(uint32_t aCheckpoint)
        : mCheckpoint(aCheckpoint), mFrameIndex(UINT32_MAX), mProgress(0) {}

    ScriptHit(uint32_t aCheckpoint, uint32_t aFrameIndex,
              ProgressCounter aProgress)
        : mCheckpoint(aCheckpoint),
          mFrameIndex(aFrameIndex),
          mProgress(aProgress) {}
  };

  struct ScriptHitChunk {
    ScriptHit mHits[7];
    ScriptHitChunk* mPrevious;
  };

  struct ScriptHitKey {
    uint32_t mScript;
    uint32_t mOffset;

    ScriptHitKey(uint32_t aScript, uint32_t aOffset)
        : mScript(aScript), mOffset(aOffset) {}

    typedef ScriptHitKey Lookup;

    static HashNumber hash(const ScriptHitKey& aKey) {
      return HashGeneric(aKey.mScript, aKey.mOffset);
    }

    static bool match(const ScriptHitKey& aFirst, const ScriptHitKey& aSecond) {
      return aFirst.mScript == aSecond.mScript &&
             aFirst.mOffset == aSecond.mOffset;
    }
  };

  typedef HashMap<ScriptHitKey, ScriptHitChunk*, ScriptHitKey,
                  AllocPolicy<MemoryKind::ScriptHits>>
      ScriptHitMap;
  ScriptHitMap mTable;
  ScriptHitChunk* mFreeChunk;

  ScriptHitInfo() : mFreeChunk(nullptr) {}

  ScriptHitChunk* FindHits(uint32_t aScript, uint32_t aOffset) {
    ScriptHitKey key(aScript, aOffset);
    ScriptHitMap::Ptr p = mTable.lookup(key);
    return p ? p->value() : nullptr;
  }

  void AddHit(uint32_t aScript, uint32_t aOffset, uint32_t aCheckpoint,
              uint32_t aFrameIndex, ProgressCounter aProgress) {
    ScriptHitKey key(aScript, aOffset);
    ScriptHitMap::AddPtr p = mTable.lookupForAdd(key);
    if (!p && !mTable.add(p, key, NewChunk(nullptr))) {
      MOZ_CRASH("ScriptHitInfo::AddScriptHit");
    }

    ScriptHitChunk* chunk = p->value();
    p->value() = AddHit(chunk, ScriptHit(aCheckpoint, aFrameIndex, aProgress));
  }

  ScriptHitChunk* AddHit(ScriptHitChunk* aChunk, const ScriptHit& aHit) {
    for (int i = ArrayLength(aChunk->mHits) - 1; i >= 0; i--) {
      if (!aChunk->mHits[i].mCheckpoint) {
        aChunk->mHits[i] = aHit;
        return aChunk;
      }
    }
    ScriptHitChunk* newChunk = NewChunk(aChunk);
    newChunk->mHits[ArrayLength(newChunk->mHits) - 1] = aHit;
    return newChunk;
  }

  ScriptHitChunk* NewChunk(ScriptHitChunk* aPrevious) {
    if (!mFreeChunk) {
      void* mem = AllocateMemory(PageSize, MemoryKind::ScriptHits);
      ScriptHitChunk* chunks = reinterpret_cast<ScriptHitChunk*>(mem);
      size_t numChunks = PageSize / sizeof(ScriptHitChunk);
      for (size_t i = 0; i < numChunks - 1; i++) {
        chunks[i].mPrevious = &chunks[i + 1];
      }
      mFreeChunk = chunks;
    }
    ScriptHitChunk* result = mFreeChunk;
    mFreeChunk = mFreeChunk->mPrevious;
    result->mPrevious = aPrevious;
    return result;
  }
};

static ScriptHitInfo* gScriptHits;

static void InitializeScriptHits() {
  void* mem = AllocateMemory(sizeof(ScriptHitInfo), MemoryKind::ScriptHits);
  gScriptHits = new (mem) ScriptHitInfo();
}

static bool RecordReplay_AddScriptHit(JSContext* aCx, unsigned aArgc,
                                      Value* aVp) {
>>>>>>> upstream-releases
  CallArgs args = CallArgsFromVp(aArgc, aVp);
<<<<<<< HEAD
||||||| merged common ancestors
  RootedObject obj(aCx, NonNullObject(aCx, args.get(0)));
  if (!obj) {
    return false;
  }
=======

  if (!args.get(0).isNumber() || !args.get(1).isNumber() ||
      !args.get(2).isNumber()) {
    JS_ReportErrorASCII(aCx, "Bad parameters");
    return false;
  }
>>>>>>> upstream-releases

<<<<<<< HEAD
  Maybe<BreakpointPosition> position;
  if (!args.get(0).isUndefined()) {
    RootedObject obj(aCx, NonNullObject(aCx, args.get(0)));
    if (!obj) {
      return false;
    }

    position.emplace();
    if (!position.ref().Decode(aCx, obj)) {
      return false;
    }
||||||| merged common ancestors
  BreakpointPosition position;
  if (!position.Decode(aCx, obj)) {
    return false;
=======
  uint32_t script = args.get(0).toNumber();
  uint32_t offset = args.get(1).toNumber();
  uint32_t frameIndex = args.get(2).toNumber();

  gScriptHits->AddHit(script, offset, GetLastCheckpoint(), frameIndex,
                      gProgressCounter);
  args.rval().setUndefined();
  return true;
}

static bool RecordReplay_FindScriptHits(JSContext* aCx, unsigned aArgc,
                                        Value* aVp) {
  CallArgs args = CallArgsFromVp(aArgc, aVp);

  if (!args.get(0).isNumber() || !args.get(1).isNumber()) {
    JS_ReportErrorASCII(aCx, "Bad parameters");
    return false;
>>>>>>> upstream-releases
  }

  uint32_t script = args.get(0).toNumber();
  uint32_t offset = args.get(1).toNumber();

  RootedValueVector values(aCx);

  ScriptHitInfo::ScriptHitChunk* chunk =
      gScriptHits ? gScriptHits->FindHits(script, offset) : nullptr;
  while (chunk) {
    for (const auto& hit : chunk->mHits) {
      if (hit.mCheckpoint) {
        RootedObject hitObject(aCx, JS_NewObject(aCx, nullptr));
        if (!hitObject ||
            !JS_DefineProperty(aCx, hitObject, "checkpoint", hit.mCheckpoint,
                               JSPROP_ENUMERATE) ||
            !JS_DefineProperty(aCx, hitObject, "progress",
                               (double)hit.mProgress, JSPROP_ENUMERATE) ||
            !JS_DefineProperty(aCx, hitObject, "frameIndex", hit.mFrameIndex,
                               JSPROP_ENUMERATE) ||
            !values.append(ObjectValue(*hitObject))) {
          return false;
        }
      }
    }
    chunk = chunk->mPrevious;
  }

  JSObject* array = JS_NewArrayObject(aCx, values);
  if (!array) {
    return false;
  }

  args.rval().setObject(*array);
  return true;
}

<<<<<<< HEAD
static bool RecordReplay_TimeWarpTargetExecutionPoint(JSContext* aCx,
                                                      unsigned aArgc,
                                                      Value* aVp) {
||||||| merged common ancestors
static bool
RecordReplay_TimeWarpTargetExecutionPoint(JSContext* aCx, unsigned aArgc, Value* aVp)
{
=======
static bool RecordReplay_GetContent(JSContext* aCx, unsigned aArgc,
                                    Value* aVp) {
>>>>>>> upstream-releases
  CallArgs args = CallArgsFromVp(aArgc, aVp);
  RootedString url(aCx, ToString(aCx, args.get(0)));

  RootedString contentType(aCx), content(aCx);
  if (!FetchContent(aCx, url, &contentType, &content)) {
    return false;
  }

<<<<<<< HEAD
  ExecutionPoint point =
      navigation::TimeWarpTargetExecutionPoint((ProgressCounter)timeWarpTarget);
  RootedObject result(aCx, point.Encode(aCx));
  if (!result) {
    return false;
  }

  args.rval().setObject(*result);
  return true;
}

static bool RecordReplay_RecordingEndpoint(JSContext* aCx, unsigned aArgc,
                                           Value* aVp) {
  CallArgs args = CallArgsFromVp(aArgc, aVp);

  ExecutionPoint point = navigation::GetRecordingEndpoint();
  RootedObject result(aCx, point.Encode(aCx));
  if (!result) {
||||||| merged common ancestors
  ExecutionPoint point = navigation::TimeWarpTargetExecutionPoint((ProgressCounter) timeWarpTarget);
  RootedObject result(aCx, point.Encode(aCx));
  if (!result) {
=======
  RootedObject obj(aCx, JS_NewObject(aCx, nullptr));
  if (!obj ||
      !JS_DefineProperty(aCx, obj, "contentType", contentType,
                         JSPROP_ENUMERATE) ||
      !JS_DefineProperty(aCx, obj, "content", content, JSPROP_ENUMERATE)) {
>>>>>>> upstream-releases
    return false;
  }

  args.rval().setObject(*obj);
  return true;
}

static bool RecordReplay_Repaint(JSContext* aCx, unsigned aArgc, Value* aVp) {
  CallArgs args = CallArgsFromVp(aArgc, aVp);

  size_t width, height;
  child::Repaint(&width, &height);

  RootedObject obj(aCx, JS_NewObject(aCx, nullptr));
  if (!obj ||
      !JS_DefineProperty(aCx, obj, "width", (double)width, JSPROP_ENUMERATE) ||
      !JS_DefineProperty(aCx, obj, "height", (double)height,
                         JSPROP_ENUMERATE)) {
    return false;
  }

  args.rval().setObject(*obj);
  return true;
}

static bool RecordReplay_Dump(JSContext* aCx, unsigned aArgc, Value* aVp) {
  // This method is an alternative to dump() that can be used in places where
  // thread events are disallowed.
  CallArgs args = CallArgsFromVp(aArgc, aVp);
  for (size_t i = 0; i < args.length(); i++) {
    RootedString str(aCx, ToString(aCx, args[i]));
    if (!str) {
      return false;
    }
    JS::UniqueChars cstr = JS_EncodeStringToLatin1(aCx, str);
    if (!cstr) {
      return false;
    }
    Print("%s", cstr.get());
  }

  args.rval().setUndefined();
  return true;
}

///////////////////////////////////////////////////////////////////////////////
// Plumbing
///////////////////////////////////////////////////////////////////////////////

static const JSFunctionSpec gMiddlemanMethods[] = {
<<<<<<< HEAD
    JS_FN("registerReplayDebugger", Middleman_RegisterReplayDebugger, 1, 0),
    JS_FN("canRewind", Middleman_CanRewind, 0, 0),
    JS_FN("resume", Middleman_Resume, 1, 0),
    JS_FN("timeWarp", Middleman_TimeWarp, 1, 0),
    JS_FN("sendRequest", Middleman_SendRequest, 1, 0),
    JS_FN("addBreakpoint", Middleman_AddBreakpoint, 1, 0),
    JS_FN("clearBreakpoints", Middleman_ClearBreakpoints, 0, 0),
    JS_FN("maybeSwitchToReplayingChild", Middleman_MaybeSwitchToReplayingChild,
          0, 0),
    JS_FN("hadRepaint", Middleman_HadRepaint, 2, 0),
    JS_FN("hadRepaintFailure", Middleman_HadRepaintFailure, 0, 0),
    JS_FN("childIsRecording", Middleman_ChildIsRecording, 0, 0),
    JS_FN("markExplicitPause", Middleman_MarkExplicitPause, 0, 0),
    JS_FN("waitUntilPaused", Middleman_WaitUntilPaused, 0, 0),
    JS_FN("positionSubsumes", Middleman_PositionSubsumes, 2, 0),
    JS_FS_END};
||||||| merged common ancestors
  JS_FN("registerReplayDebugger", Middleman_RegisterReplayDebugger, 1, 0),
  JS_FN("canRewind", Middleman_CanRewind, 0, 0),
  JS_FN("resume", Middleman_Resume, 1, 0),
  JS_FN("timeWarp", Middleman_TimeWarp, 1, 0),
  JS_FN("pause", Middleman_Pause, 0, 0),
  JS_FN("sendRequest", Middleman_SendRequest, 1, 0),
  JS_FN("setBreakpoint", Middleman_SetBreakpoint, 2, 0),
  JS_FN("clearBreakpoint", Middleman_ClearBreakpoint, 1, 0),
  JS_FN("maybeSwitchToReplayingChild", Middleman_MaybeSwitchToReplayingChild, 0, 0),
  JS_FN("hadRepaint", Middleman_HadRepaint, 2, 0),
  JS_FN("hadRepaintFailure", Middleman_HadRepaintFailure, 0, 0),
  JS_FN("recordingPosition", Middleman_RecordingPosition, 0, 0),
  JS_FS_END
};
=======
    JS_FN("registerReplayDebugger", Middleman_RegisterReplayDebugger, 1, 0),
    JS_FN("canRewind", Middleman_CanRewind, 0, 0),
    JS_FN("spawnReplayingChild", Middleman_SpawnReplayingChild, 0, 0),
    JS_FN("sendManifest", Middleman_SendManifest, 2, 0),
    JS_FN("hadRepaint", Middleman_HadRepaint, 2, 0),
    JS_FN("hadRepaintFailure", Middleman_HadRepaintFailure, 0, 0),
    JS_FN("inRepaintStressMode", Middleman_InRepaintStressMode, 0, 0),
    JS_FN("waitUntilPaused", Middleman_WaitUntilPaused, 1, 0),
    JS_FS_END};
>>>>>>> upstream-releases

static const JSFunctionSpec gRecordReplayMethods[] = {
<<<<<<< HEAD
    JS_FN("areThreadEventsDisallowed", RecordReplay_AreThreadEventsDisallowed,
          0, 0),
    JS_FN("maybeDivergeFromRecording", RecordReplay_MaybeDivergeFromRecording,
          0, 0),
    JS_FN("advanceProgressCounter", RecordReplay_AdvanceProgressCounter, 0, 0),
    JS_FN("shouldUpdateProgressCounter",
          RecordReplay_ShouldUpdateProgressCounter, 1, 0),
    JS_FN("positionHit", RecordReplay_PositionHit, 1, 0),
    JS_FN("getContent", RecordReplay_GetContent, 1, 0),
    JS_FN("currentExecutionPoint", RecordReplay_CurrentExecutionPoint, 1, 0),
    JS_FN("timeWarpTargetExecutionPoint",
          RecordReplay_TimeWarpTargetExecutionPoint, 1, 0),
    JS_FN("recordingEndpoint", RecordReplay_RecordingEndpoint, 0, 0),
    JS_FN("repaint", RecordReplay_Repaint, 0, 0),
    JS_FN("dump", RecordReplay_Dump, 1, 0),
    JS_FS_END};
||||||| merged common ancestors
  JS_FN("areThreadEventsDisallowed", RecordReplay_AreThreadEventsDisallowed, 0, 0),
  JS_FN("maybeDivergeFromRecording", RecordReplay_MaybeDivergeFromRecording, 0, 0),
  JS_FN("advanceProgressCounter", RecordReplay_AdvanceProgressCounter, 0, 0),
  JS_FN("positionHit", RecordReplay_PositionHit, 1, 0),
  JS_FN("getContent", RecordReplay_GetContent, 1, 0),
  JS_FN("currentExecutionPoint", RecordReplay_CurrentExecutionPoint, 1, 0),
  JS_FN("timeWarpTargetExecutionPoint", RecordReplay_TimeWarpTargetExecutionPoint, 1, 0),
  JS_FN("repaint", RecordReplay_Repaint, 0, 0),
  JS_FN("dump", RecordReplay_Dump, 1, 0),
  JS_FS_END
};
=======
    JS_FN("areThreadEventsDisallowed", RecordReplay_AreThreadEventsDisallowed,
          0, 0),
    JS_FN("divergeFromRecording", RecordReplay_DivergeFromRecording, 0, 0),
    JS_FN("progressCounter", RecordReplay_ProgressCounter, 0, 0),
    JS_FN("advanceProgressCounter", RecordReplay_AdvanceProgressCounter, 0, 0),
    JS_FN("shouldUpdateProgressCounter",
          RecordReplay_ShouldUpdateProgressCounter, 1, 0),
    JS_FN("manifestFinished", RecordReplay_ManifestFinished, 1, 0),
    JS_FN("resumeExecution", RecordReplay_ResumeExecution, 0, 0),
    JS_FN("restoreCheckpoint", RecordReplay_RestoreCheckpoint, 1, 0),
    JS_FN("currentExecutionTime", RecordReplay_CurrentExecutionTime, 0, 0),
    JS_FN("flushRecording", RecordReplay_FlushRecording, 0, 0),
    JS_FN("setMainChild", RecordReplay_SetMainChild, 0, 0),
    JS_FN("saveCheckpoint", RecordReplay_SaveCheckpoint, 1, 0),
    JS_FN("addScriptHit", RecordReplay_AddScriptHit, 3, 0),
    JS_FN("findScriptHits", RecordReplay_FindScriptHits, 2, 0),
    JS_FN("getContent", RecordReplay_GetContent, 1, 0),
    JS_FN("repaint", RecordReplay_Repaint, 0, 0),
    JS_FN("dump", RecordReplay_Dump, 1, 0),
    JS_FS_END};
>>>>>>> upstream-releases

extern "C" {

MOZ_EXPORT bool RecordReplayInterface_DefineRecordReplayControlObject(
    JSContext* aCx, JSObject* aObjectArg) {
  RootedObject object(aCx, aObjectArg);

  RootedObject staticObject(aCx, JS_NewObject(aCx, nullptr));
  if (!staticObject ||
      !JS_DefineProperty(aCx, object, "RecordReplayControl", staticObject, 0)) {
    return false;
  }

  // FIXME Bug 1475901 Define this interface via WebIDL instead of raw JSAPI.
  if (IsMiddleman()) {
    if (!JS_DefineFunctions(aCx, staticObject, gMiddlemanMethods)) {
      return false;
    }
  } else if (IsRecordingOrReplaying()) {
    if (!JS_DefineFunctions(aCx, staticObject, gRecordReplayMethods)) {
      return false;
    }
  } else {
    // Leave RecordReplayControl as an empty object. We still define the object
    // to avoid reference errors in scripts that run in normal processes.
  }

  return true;
}

}  // extern "C"

}  // namespace js
}  // namespace recordreplay
}  // namespace mozilla
