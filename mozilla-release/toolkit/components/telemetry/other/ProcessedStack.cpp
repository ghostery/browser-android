/* -*- Mode: C++; tab-width: 8; indent-tabs-mode: nil; c-basic-offset: 2 -*- */
/* vim: set ts=8 sts=2 et sw=2 tw=80: */
/* This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/. */

#include "ProcessedStack.h"

namespace {

struct StackFrame {
  uintptr_t mPC;    // The program counter at this position in the call stack.
  uint16_t mIndex;  // The number of this frame in the call stack.
  uint16_t mModIndex;  // The index of module that has this program counter.
};

#ifdef MOZ_GECKO_PROFILER
<<<<<<< HEAD
static bool CompareByPC(const StackFrame &a, const StackFrame &b) {
||||||| merged common ancestors
static bool CompareByPC(const StackFrame &a, const StackFrame &b)
{
=======
static bool CompareByPC(const StackFrame& a, const StackFrame& b) {
>>>>>>> upstream-releases
  return a.mPC < b.mPC;
}

<<<<<<< HEAD
static bool CompareByIndex(const StackFrame &a, const StackFrame &b) {
||||||| merged common ancestors
static bool CompareByIndex(const StackFrame &a, const StackFrame &b)
{
=======
static bool CompareByIndex(const StackFrame& a, const StackFrame& b) {
>>>>>>> upstream-releases
  return a.mIndex < b.mIndex;
}
#endif

}  // namespace

namespace mozilla {
namespace Telemetry {

const size_t kMaxChromeStackDepth = 50;

ProcessedStack::ProcessedStack() = default;

size_t ProcessedStack::GetStackSize() const { return mStack.size(); }

size_t ProcessedStack::GetNumModules() const { return mModules.size(); }

<<<<<<< HEAD
bool ProcessedStack::Module::operator==(const Module &aOther) const {
  return mName == aOther.mName && mBreakpadId == aOther.mBreakpadId;
||||||| merged common ancestors
bool ProcessedStack::Module::operator==(const Module& aOther) const {
  return  mName == aOther.mName &&
    mBreakpadId == aOther.mBreakpadId;
=======
bool ProcessedStack::Module::operator==(const Module& aOther) const {
  return mName == aOther.mName && mBreakpadId == aOther.mBreakpadId;
>>>>>>> upstream-releases
}

<<<<<<< HEAD
const ProcessedStack::Frame &ProcessedStack::GetFrame(unsigned aIndex) const {
||||||| merged common ancestors
const ProcessedStack::Frame &ProcessedStack::GetFrame(unsigned aIndex) const
{
=======
const ProcessedStack::Frame& ProcessedStack::GetFrame(unsigned aIndex) const {
>>>>>>> upstream-releases
  MOZ_ASSERT(aIndex < mStack.size());
  return mStack[aIndex];
}

<<<<<<< HEAD
void ProcessedStack::AddFrame(const Frame &aFrame) { mStack.push_back(aFrame); }
||||||| merged common ancestors
void ProcessedStack::AddFrame(const Frame &aFrame)
{
  mStack.push_back(aFrame);
}
=======
void ProcessedStack::AddFrame(const Frame& aFrame) { mStack.push_back(aFrame); }
>>>>>>> upstream-releases

<<<<<<< HEAD
const ProcessedStack::Module &ProcessedStack::GetModule(unsigned aIndex) const {
||||||| merged common ancestors
const ProcessedStack::Module &ProcessedStack::GetModule(unsigned aIndex) const
{
=======
const ProcessedStack::Module& ProcessedStack::GetModule(unsigned aIndex) const {
>>>>>>> upstream-releases
  MOZ_ASSERT(aIndex < mModules.size());
  return mModules[aIndex];
}

<<<<<<< HEAD
void ProcessedStack::AddModule(const Module &aModule) {
||||||| merged common ancestors
void ProcessedStack::AddModule(const Module &aModule)
{
=======
void ProcessedStack::AddModule(const Module& aModule) {
>>>>>>> upstream-releases
  mModules.push_back(aModule);
}

void ProcessedStack::Clear() {
  mModules.clear();
  mStack.clear();
}

<<<<<<< HEAD
ProcessedStack GetStackAndModules(const std::vector<uintptr_t> &aPCs) {
  return BatchProcessedStackGenerator().GetStackAndModules(aPCs);
}

BatchProcessedStackGenerator::BatchProcessedStackGenerator()
#ifdef MOZ_GECKO_PROFILER
    : mSortedRawModules(SharedLibraryInfo::GetInfoForSelf())
#endif
||||||| merged common ancestors
ProcessedStack
GetStackAndModules(const std::vector<uintptr_t>& aPCs)
=======
ProcessedStack GetStackAndModules(const std::vector<uintptr_t>& aPCs) {
  return BatchProcessedStackGenerator().GetStackAndModules(aPCs);
}

BatchProcessedStackGenerator::BatchProcessedStackGenerator()
#ifdef MOZ_GECKO_PROFILER
    : mSortedRawModules(SharedLibraryInfo::GetInfoForSelf())
#endif
>>>>>>> upstream-releases
{
<<<<<<< HEAD
#ifdef MOZ_GECKO_PROFILER
  mSortedRawModules.SortByAddress();
#endif
}

ProcessedStack BatchProcessedStackGenerator::GetStackAndModules(
    const std::vector<uintptr_t> &aPCs) {
||||||| merged common ancestors
=======
#ifdef MOZ_GECKO_PROFILER
  mSortedRawModules.SortByAddress();
#endif
}

ProcessedStack BatchProcessedStackGenerator::GetStackAndModules(
    const std::vector<uintptr_t>& aPCs) {
>>>>>>> upstream-releases
  std::vector<StackFrame> rawStack;
  auto stackEnd = aPCs.begin() + std::min(aPCs.size(), kMaxChromeStackDepth);
  for (auto i = aPCs.begin(); i != stackEnd; ++i) {
    uintptr_t aPC = *i;
    StackFrame Frame = {aPC, static_cast<uint16_t>(rawStack.size()),
                        std::numeric_limits<uint16_t>::max()};
    rawStack.push_back(Frame);
  }

#ifdef MOZ_GECKO_PROFILER
  SharedLibraryInfo rawModules(mSortedRawModules);
  // Remove all modules not referenced by a PC on the stack
  std::sort(rawStack.begin(), rawStack.end(), CompareByPC);

  size_t moduleIndex = 0;
  size_t stackIndex = 0;
  size_t stackSize = rawStack.size();

  while (moduleIndex < rawModules.GetSize()) {
    const SharedLibrary &module = rawModules.GetEntry(moduleIndex);
    uintptr_t moduleStart = module.GetStart();
    uintptr_t moduleEnd = module.GetEnd() - 1;
    // the interval is [moduleStart, moduleEnd)

    bool moduleReferenced = false;
    for (; stackIndex < stackSize; ++stackIndex) {
      uintptr_t pc = rawStack[stackIndex].mPC;
      if (pc >= moduleEnd) break;

      if (pc >= moduleStart) {
        // If the current PC is within the current module, mark
        // module as used
        moduleReferenced = true;
        rawStack[stackIndex].mPC -= moduleStart;
        rawStack[stackIndex].mModIndex = moduleIndex;
      } else {
        // PC does not belong to any module. It is probably from
        // the JIT. Use a fixed mPC so that we don't get different
        // stacks on different runs.
        rawStack[stackIndex].mPC = std::numeric_limits<uintptr_t>::max();
      }
    }

    if (moduleReferenced) {
      ++moduleIndex;
    } else {
      // Remove module if no PCs within its address range
      rawModules.RemoveEntries(moduleIndex, moduleIndex + 1);
    }
  }

  for (; stackIndex < stackSize; ++stackIndex) {
    // These PCs are past the last module.
    rawStack[stackIndex].mPC = std::numeric_limits<uintptr_t>::max();
  }

  std::sort(rawStack.begin(), rawStack.end(), CompareByIndex);
#endif

  // Copy the information to the return value.
  ProcessedStack Ret;
<<<<<<< HEAD
  for (auto &rawFrame : rawStack) {
    mozilla::Telemetry::ProcessedStack::Frame frame = {rawFrame.mPC,
                                                       rawFrame.mModIndex};
||||||| merged common ancestors
  for (auto & rawFrame : rawStack) {
    mozilla::Telemetry::ProcessedStack::Frame frame = { rawFrame.mPC, rawFrame.mModIndex };
=======
  for (auto& rawFrame : rawStack) {
    mozilla::Telemetry::ProcessedStack::Frame frame = {rawFrame.mPC,
                                                       rawFrame.mModIndex};
>>>>>>> upstream-releases
    Ret.AddFrame(frame);
  }

#ifdef MOZ_GECKO_PROFILER
  for (unsigned i = 0, n = rawModules.GetSize(); i != n; ++i) {
<<<<<<< HEAD
    const SharedLibrary &info = rawModules.GetEntry(i);
    mozilla::Telemetry::ProcessedStack::Module module = {info.GetDebugName(),
                                                         info.GetBreakpadId()};
||||||| merged common ancestors
    const SharedLibrary &info = rawModules.GetEntry(i);
    mozilla::Telemetry::ProcessedStack::Module module = {
      info.GetDebugName(),
      info.GetBreakpadId()
    };
=======
    const SharedLibrary& info = rawModules.GetEntry(i);
    mozilla::Telemetry::ProcessedStack::Module module = {info.GetDebugName(),
                                                         info.GetBreakpadId()};
>>>>>>> upstream-releases
    Ret.AddModule(module);
  }
#endif

  return Ret;
}

}  // namespace Telemetry
}  // namespace mozilla
