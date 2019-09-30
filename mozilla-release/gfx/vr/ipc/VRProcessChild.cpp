/* -*- Mode: C++; tab-width: 8; indent-tabs-mode: nil; c-basic-offset: 2 -*- */
/* vim: set ts=8 sts=2 et sw=2 tw=80: */
/* This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/. */

#include "VRProcessChild.h"

#include "mozilla/BackgroundHangMonitor.h"
#include "mozilla/ipc/IOThreadChild.h"
<<<<<<< HEAD

||||||| merged common ancestors


=======
#include "ProcessUtils.h"

>>>>>>> upstream-releases
using namespace mozilla;
using namespace mozilla::gfx;
using mozilla::ipc::IOThreadChild;

<<<<<<< HEAD
||||||| merged common ancestors

=======
StaticRefPtr<VRParent> sVRParent;

>>>>>>> upstream-releases
VRProcessChild::VRProcessChild(ProcessId aParentPid)
<<<<<<< HEAD
    : ProcessChild(aParentPid)
#if defined(aParentPid)
      ,
      mVR(nullptr)
#endif
{
}
||||||| merged common ancestors
  : ProcessChild(aParentPid)
#if defined(aParentPid)
  , mVR(nullptr)
#endif
{
}
=======
    : ProcessChild(aParentPid) {}
>>>>>>> upstream-releases

<<<<<<< HEAD
VRProcessChild::~VRProcessChild() {}
||||||| merged common ancestors
VRProcessChild::~VRProcessChild()
{
}
=======
VRProcessChild::~VRProcessChild() { sVRParent = nullptr; }
>>>>>>> upstream-releases

<<<<<<< HEAD
bool VRProcessChild::Init(int aArgc, char* aArgv[]) {
  BackgroundHangMonitor::Startup();
||||||| merged common ancestors
bool
VRProcessChild::Init(int aArgc, char* aArgv[])
{
  BackgroundHangMonitor::Startup();
=======
/*static*/
VRParent* VRProcessChild::GetVRParent() {
  MOZ_ASSERT(sVRParent);
  return sVRParent;
}
>>>>>>> upstream-releases

bool VRProcessChild::Init(int aArgc, char* aArgv[]) {
  char* parentBuildID = nullptr;
  char* prefsHandle = nullptr;
  char* prefMapHandle = nullptr;
  char* prefsLen = nullptr;
  char* prefMapSize = nullptr;
  for (int i = 1; i < aArgc; i++) {
    if (!aArgv[i]) {
      continue;
    }
    if (strcmp(aArgv[i], "-parentBuildID") == 0) {
      parentBuildID = aArgv[i + 1];

#ifdef XP_WIN
    } else if (strcmp(aArgv[i], "-prefsHandle") == 0) {
      if (++i == aArgc) {
        return false;
      }
      prefsHandle = aArgv[i];
    } else if (strcmp(aArgv[i], "-prefMapHandle") == 0) {
      if (++i == aArgc) {
        return false;
      }
      prefMapHandle = aArgv[i];
#endif
    } else if (strcmp(aArgv[i], "-prefsLen") == 0) {
      if (++i == aArgc) {
        return false;
      }
      prefsLen = aArgv[i];
    } else if (strcmp(aArgv[i], "-prefMapSize") == 0) {
      if (++i == aArgc) {
        return false;
      }
      prefMapSize = aArgv[i];
    }
  }

<<<<<<< HEAD
  mVR.Init(ParentPid(), parentBuildID, IOThreadChild::message_loop(),
           IOThreadChild::channel());
||||||| merged common ancestors
  mVR.Init(ParentPid(), parentBuildID,
           IOThreadChild::message_loop(),
           IOThreadChild::channel());
=======
  SharedPreferenceDeserializer deserializer;
  if (!deserializer.DeserializeFromSharedMemory(prefsHandle, prefMapHandle,
                                                prefsLen, prefMapSize)) {
    return false;
  }

  sVRParent = new VRParent();
  sVRParent->Init(ParentPid(), parentBuildID, IOThreadChild::message_loop(),
                  IOThreadChild::channel());
>>>>>>> upstream-releases

  return true;
}

<<<<<<< HEAD
void VRProcessChild::CleanUp() { NS_ShutdownXPCOM(nullptr); }
||||||| merged common ancestors
void
VRProcessChild::CleanUp()
{
  NS_ShutdownXPCOM(nullptr);
}
=======
void VRProcessChild::CleanUp() {
  sVRParent = nullptr;
  NS_ShutdownXPCOM(nullptr);
}
>>>>>>> upstream-releases
