/* -*- Mode: C++; tab-width: 8; indent-tabs-mode: nil; c-basic-offset: 2 -*- */
/* vim: set ts=8 sts=2 et sw=2 tw=80: */
/* This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/. */

#include <functional>

#include "nsExceptionHandler.h"
#include "nsExceptionHandlerUtils.h"

namespace CrashReporter {

void AnnotateOOMAllocationSize(size_t size) {}

void AnnotateTexturesSize(size_t size) {}

void AnnotatePendingIPC(size_t aNumOfPendingIPC, uint32_t aTopPendingIPCCount,
                        const char* aTopPendingIPCName,
                        uint32_t aTopPendingIPCType) {}

nsresult SetExceptionHandler(nsIFile* aXREDirectory, bool force /*=false*/) {
  return NS_ERROR_NOT_IMPLEMENTED;
}

bool GetEnabled() { return false; }

bool GetMinidumpPath(nsAString& aPath) { return false; }

nsresult SetMinidumpPath(const nsAString& aPath) {
  return NS_ERROR_NOT_IMPLEMENTED;
}

nsresult SetupExtraData(nsIFile* aAppDataDirectory,
                        const nsACString& aBuildID) {
  return NS_ERROR_NOT_IMPLEMENTED;
}

nsresult UnsetExceptionHandler() { return NS_ERROR_NOT_IMPLEMENTED; }

void NotifyCrashReporterClientCreated() {}

nsresult AnnotateCrashReport(Annotation key, bool data) {
  return NS_ERROR_NOT_IMPLEMENTED;
}

nsresult AnnotateCrashReport(Annotation key, int data) {
  return NS_ERROR_NOT_IMPLEMENTED;
}

nsresult AnnotateCrashReport(Annotation key, unsigned int data) {
  return NS_ERROR_NOT_IMPLEMENTED;
}

nsresult AnnotateCrashReport(Annotation key, const nsACString& data) {
  return NS_ERROR_NOT_IMPLEMENTED;
}

nsresult RemoveCrashReportAnnotation(Annotation key) {
  return NS_ERROR_NOT_IMPLEMENTED;
}

<<<<<<< HEAD
nsresult SetGarbageCollecting(bool collecting) {
  return NS_ERROR_NOT_IMPLEMENTED;
||||||| merged common ancestors
nsresult
SetGarbageCollecting(bool collecting)
{
  return NS_ERROR_NOT_IMPLEMENTED;
=======
void MergeCrashAnnotations(AnnotationTable& aDst, const AnnotationTable& aSrc) {
>>>>>>> upstream-releases
}

<<<<<<< HEAD
void SetEventloopNestingLevel(uint32_t level) {}
||||||| merged common ancestors
void
SetEventloopNestingLevel(uint32_t level)
{
}
=======
nsresult SetGarbageCollecting(bool collecting) {
  return NS_ERROR_NOT_IMPLEMENTED;
}
>>>>>>> upstream-releases

<<<<<<< HEAD
void SetMinidumpAnalysisAllThreads() {}
||||||| merged common ancestors
void
SetMinidumpAnalysisAllThreads()
{
}
=======
void SetEventloopNestingLevel(uint32_t level) {}

void SetMinidumpAnalysisAllThreads() {}
>>>>>>> upstream-releases

nsresult AppendAppNotesToCrashReport(const nsACString& data) {
  return NS_ERROR_NOT_IMPLEMENTED;
}

bool GetAnnotation(const nsACString& key, nsACString& data) { return false; }

nsresult RegisterAppMemory(void* ptr, size_t length) {
  return NS_ERROR_NOT_IMPLEMENTED;
}

nsresult UnregisterAppMemory(void* ptr) { return NS_ERROR_NOT_IMPLEMENTED; }

void SetIncludeContextHeap(bool aValue) {}

bool GetServerURL(nsACString& aServerURL) { return false; }

nsresult SetServerURL(const nsACString& aServerURL) {
  return NS_ERROR_NOT_IMPLEMENTED;
}

nsresult SetRestartArgs(int argc, char** argv) {
  return NS_ERROR_NOT_IMPLEMENTED;
}

#if !defined(XP_WIN)
int GetAnnotationTimeCrashFd() { return 7; }
#endif

void RegisterChildCrashAnnotationFileDescriptor(ProcessId aProcess,
                                                PRFileDesc* aFd) {}

void DeregisterChildCrashAnnotationFileDescriptor(ProcessId aProcess) {}

<<<<<<< HEAD
#ifdef XP_WIN32
nsresult WriteMinidumpForException(EXCEPTION_POINTERS* aExceptionInfo) {
||||||| merged common ancestors
#ifdef XP_WIN32
nsresult
WriteMinidumpForException(EXCEPTION_POINTERS* aExceptionInfo)
{
=======
#ifdef XP_WIN
nsresult WriteMinidumpForException(EXCEPTION_POINTERS* aExceptionInfo) {
>>>>>>> upstream-releases
  return NS_ERROR_NOT_IMPLEMENTED;
}
#endif

#ifdef XP_LINUX
bool WriteMinidumpForSigInfo(int signo, siginfo_t* info, void* uc) {
  return false;
}
#endif

#ifdef XP_MACOSX
nsresult AppendObjCExceptionInfoToAppNotes(void* inException) {
  return NS_ERROR_NOT_IMPLEMENTED;
}
#endif

nsresult GetSubmitReports(bool* aSubmitReports) {
  return NS_ERROR_NOT_IMPLEMENTED;
}

nsresult SetSubmitReports(bool aSubmitReports) {
  return NS_ERROR_NOT_IMPLEMENTED;
}

void SetProfileDirectory(nsIFile* aDir) {}

void SetUserAppDataDirectory(nsIFile* aDir) {}

void UpdateCrashEventsDir() {}

bool GetCrashEventsDir(nsAString& aPath) { return false; }

void SetMemoryReportFile(nsIFile* aFile) {}

nsresult GetDefaultMemoryReportFile(nsIFile** aFile) {
  return NS_ERROR_NOT_IMPLEMENTED;
}

<<<<<<< HEAD
void SetTelemetrySessionId(const nsACString& id) {}

void DeleteMinidumpFilesForID(const nsAString& id) {}
||||||| merged common ancestors
void
SetTelemetrySessionId(const nsACString& id)
{
}

void
DeleteMinidumpFilesForID(const nsAString& id)
{
}
=======
void SetTelemetrySessionId(const nsACString& id) {}
>>>>>>> upstream-releases

<<<<<<< HEAD
bool GetMinidumpForID(const nsAString& id, nsIFile** minidump) { return false; }
||||||| merged common ancestors
bool
GetMinidumpForID(const nsAString& id, nsIFile** minidump)
{
  return false;
}
=======
void DeleteMinidumpFilesForID(const nsAString& id) {}
>>>>>>> upstream-releases

<<<<<<< HEAD
bool GetIDFromMinidump(nsIFile* minidump, nsAString& id) { return false; }
||||||| merged common ancestors
bool
GetIDFromMinidump(nsIFile* minidump, nsAString& id)
{
  return false;
}
=======
bool GetMinidumpForID(const nsAString& id, nsIFile** minidump) { return false; }
>>>>>>> upstream-releases

<<<<<<< HEAD
bool GetExtraFileForID(const nsAString& id, nsIFile** extraFile) {
  return false;
}
||||||| merged common ancestors
bool
GetExtraFileForID(const nsAString& id,
                  nsIFile** extraFile)
{
  return false;
}
=======
bool GetIDFromMinidump(nsIFile* minidump, nsAString& id) { return false; }
>>>>>>> upstream-releases

<<<<<<< HEAD
bool GetExtraFileForMinidump(nsIFile* minidump, nsIFile** extraFile) {
||||||| merged common ancestors
bool
GetExtraFileForMinidump(nsIFile* minidump,
                        nsIFile** extraFile)
{
=======
bool GetExtraFileForID(const nsAString& id, nsIFile** extraFile) {
>>>>>>> upstream-releases
  return false;
}

<<<<<<< HEAD
bool AppendExtraData(const nsAString& id, const AnnotationTable& data) {
||||||| merged common ancestors
bool
AppendExtraData(const nsAString& id,
                const AnnotationTable& data)
{
=======
bool GetExtraFileForMinidump(nsIFile* minidump, nsIFile** extraFile) {
>>>>>>> upstream-releases
  return false;
}

<<<<<<< HEAD
bool AppendExtraData(nsIFile* extraFile, const AnnotationTable& data) {
||||||| merged common ancestors
bool
AppendExtraData(nsIFile* extraFile,
                const AnnotationTable& data)
{
=======
bool WriteExtraFile(const nsAString& id, const AnnotationTable& annotations) {
>>>>>>> upstream-releases
  return false;
}

<<<<<<< HEAD
void OOPInit() {}

void GetChildProcessTmpDir(nsIFile** aOutTmpDir) {}
||||||| merged common ancestors
void
OOPInit()
{
}

void
GetChildProcessTmpDir(nsIFile** aOutTmpDir)
{
}
=======
void OOPInit() {}
>>>>>>> upstream-releases

#if defined(XP_WIN) || defined(XP_MACOSX)
const char* GetChildNotificationPipe() { return nullptr; }
#endif

#ifdef MOZ_CRASHREPORTER_INJECTOR
void InjectCrashReporterIntoProcess(DWORD processID,
                                    InjectorCrashCallback* cb) {}

void UnregisterInjectorCallback(DWORD processID) {}

#endif  // MOZ_CRASHREPORTER_INJECTOR

bool GetLastRunCrashID(nsAString& id) { return false; }

#if defined(XP_WIN)
bool SetRemoteExceptionHandler(const nsACString& crashPipe,
                               uintptr_t aCrashTimeAnnotationFile) {
  return false;
}

#elif defined(XP_MACOSX)

bool SetRemoteExceptionHandler(const nsACString& crashPipe) { return false; }

#else

bool CreateNotificationPipeForChild(int* childCrashFd, int* childCrashRemapFd) {
  return false;
}

bool SetRemoteExceptionHandler() { return false; }
#endif  // XP_WIN

<<<<<<< HEAD
bool TakeMinidumpForChild(uint32_t childPid, nsIFile** dump,
                          uint32_t* aSequence) {
||||||| merged common ancestors
bool
TakeMinidumpForChild(uint32_t childPid, nsIFile** dump, uint32_t* aSequence)
{
=======
bool TakeMinidumpForChild(uint32_t childPid, nsIFile** dump,
                          AnnotationTable& aAnnotations, uint32_t* aSequence) {
>>>>>>> upstream-releases
  return false;
}

<<<<<<< HEAD
void RenameAdditionalHangMinidump(nsIFile* minidump, nsIFile* childMinidump,
                                  const nsACString& name) {}
||||||| merged common ancestors
void
RenameAdditionalHangMinidump(nsIFile* minidump, nsIFile* childMinidump,
                             const nsACString& name)
{
}
=======
bool FinalizeOrphanedMinidump(uint32_t aChildPid, GeckoProcessType aType) {
  return false;
}
>>>>>>> upstream-releases

ThreadId CurrentThreadId() { return -1; }

bool TakeMinidump(nsIFile** aResult, bool aMoveToPending) { return false; }

<<<<<<< HEAD
bool CreateMinidumpsAndPair(ProcessHandle aTargetPid,
                            ThreadId aTargetBlamedThread,
                            const nsACString& aIncomingPairName,
                            nsIFile* aIncomingDumpToPair,
                            nsIFile** aTargetDumpOut) {
||||||| merged common ancestors
bool
CreateMinidumpsAndPair(ProcessHandle aTargetPid,
                       ThreadId aTargetBlamedThread,
                       const nsACString& aIncomingPairName,
                       nsIFile* aIncomingDumpToPair,
                       nsIFile** aTargetDumpOut)
{
=======
bool CreateMinidumpsAndPair(ProcessHandle aTargetPid,
                            ThreadId aTargetBlamedThread,
                            const nsACString& aIncomingPairName,
                            nsIFile* aIncomingDumpToPair,
                            AnnotationTable& aTargetAnnotations,
                            nsIFile** aTargetDumpOut) {
>>>>>>> upstream-releases
  return false;
}

bool CreateAdditionalChildMinidump(ProcessHandle childPid,
                                   ThreadId childBlamedThread,
                                   nsIFile* parentMinidump,
                                   const nsACString& name) {
  return false;
}

bool UnsetRemoteExceptionHandler() { return false; }

#if defined(MOZ_WIDGET_ANDROID)
void SetNotificationPipeForChild(int childCrashFd) {}

void SetCrashAnnotationPipeForChild(int childCrashAnnotationFd) {}

void AddLibraryMapping(const char* library_name, uintptr_t start_address,
                       size_t mapping_length, size_t file_offset) {}
#endif

// From ThreadAnnotation.cpp

void InitThreadAnnotation() {}

void SetCurrentThreadName(const char* aName) {}

void GetFlatThreadAnnotation(
    const std::function<void(const char*)>& aCallback) {}

void ShutdownThreadAnnotation() {}

}  // namespace CrashReporter
