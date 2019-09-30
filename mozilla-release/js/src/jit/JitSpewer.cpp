/* -*- Mode: C++; tab-width: 8; indent-tabs-mode: nil; c-basic-offset: 2 -*-
 * vim: set ts=8 sts=2 et sw=2 tw=80:
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/. */

#ifdef JS_JITSPEW

<<<<<<< HEAD
#include "jit/JitSpewer.h"

#include "mozilla/Atomics.h"
#include "mozilla/Sprintf.h"

#ifdef XP_WIN
#include <process.h>
#define getpid _getpid
#else
#include <unistd.h>
#endif
#include "jit/Ion.h"
#include "jit/MIR.h"
#include "jit/MIRGenerator.h"
#include "jit/MIRGraph.h"
#include "threading/LockGuard.h"
#include "util/Text.h"
#include "vm/HelperThreads.h"
#include "vm/MutexIDs.h"

#include "vm/Realm-inl.h"

#ifndef JIT_SPEW_DIR
#if defined(_WIN32)
#define JIT_SPEW_DIR "."
#elif defined(__ANDROID__)
#define JIT_SPEW_DIR "/data/local/tmp"
#else
#define JIT_SPEW_DIR "/tmp"
#endif
#endif
||||||| merged common ancestors
#include "jit/JitSpewer.h"

#include "mozilla/Atomics.h"

#ifdef XP_WIN
#include <process.h>
#define getpid _getpid
#else
#include <unistd.h>
#endif
#include "jit/Ion.h"
#include "jit/MIR.h"
#include "jit/MIRGenerator.h"
#include "jit/MIRGraph.h"
#include "threading/LockGuard.h"
#include "vm/HelperThreads.h"
#include "vm/MutexIDs.h"

#include "vm/Realm-inl.h"

#ifndef JIT_SPEW_DIR
# if defined(_WIN32)
#  define JIT_SPEW_DIR "."
# elif defined(__ANDROID__)
#  define JIT_SPEW_DIR "/data/local/tmp"
# else
#  define JIT_SPEW_DIR "/tmp"
# endif
#endif
=======
#  include "jit/JitSpewer.h"

#  include "mozilla/Atomics.h"
#  include "mozilla/Sprintf.h"

#  ifdef XP_WIN
#    include <process.h>
#    define getpid _getpid
#  else
#    include <unistd.h>
#  endif
#  include "jit/Ion.h"
#  include "jit/MIR.h"
#  include "jit/MIRGenerator.h"
#  include "jit/MIRGraph.h"
#  include "threading/LockGuard.h"
#  include "util/Text.h"
#  include "vm/HelperThreads.h"
#  include "vm/MutexIDs.h"

#  include "vm/Realm-inl.h"

#  ifndef JIT_SPEW_DIR
#    if defined(_WIN32)
#      define JIT_SPEW_DIR "."
#    elif defined(__ANDROID__)
#      define JIT_SPEW_DIR "/data/local/tmp"
#    else
#      define JIT_SPEW_DIR "/tmp"
#    endif
#  endif
>>>>>>> upstream-releases

using namespace js;
using namespace js::jit;

class IonSpewer {
 private:
  Mutex outputLock_;
  Fprinter jsonOutput_;
  bool firstFunction_;
  bool asyncLogging_;
  bool inited_;

  void release();

 public:
  IonSpewer()
      : outputLock_(mutexid::IonSpewer),
        firstFunction_(false),
        asyncLogging_(false),
        inited_(false) {}

  // File output is terminated safely upon destruction.
  ~IonSpewer();

  bool init();
  bool isEnabled() { return inited_; }
  void setAsyncLogging(bool incremental) { asyncLogging_ = incremental; }
  bool getAsyncLogging() { return asyncLogging_; }

  void beginFunction();
  void spewPass(GraphSpewer* gs);
  void endFunction(GraphSpewer* gs);
};

// IonSpewer singleton.
static IonSpewer ionspewer;

static bool LoggingChecked = false;
static_assert(JitSpew_Terminator <= 64,
              "Increase the size of the LoggingBits global.");
static uint64_t LoggingBits = 0;
static mozilla::Atomic<uint32_t, mozilla::Relaxed> filteredOutCompilations(0);

<<<<<<< HEAD
static const char* const ChannelNames[] = {
#define JITSPEW_CHANNEL(name) #name,
||||||| merged common ancestors
static const char* const ChannelNames[] =
{
#define JITSPEW_CHANNEL(name) #name,
=======
static const char* const ChannelNames[] = {
#  define JITSPEW_CHANNEL(name) #  name,
>>>>>>> upstream-releases
    JITSPEW_CHANNEL_LIST(JITSPEW_CHANNEL)
#  undef JITSPEW_CHANNEL
};

<<<<<<< HEAD
static size_t ChannelIndentLevel[] = {
#define JITSPEW_CHANNEL(name) 0,
||||||| merged common ancestors
static size_t ChannelIndentLevel[] =
{
#define JITSPEW_CHANNEL(name) 0,
=======
static size_t ChannelIndentLevel[] = {
#  define JITSPEW_CHANNEL(name) 0,
>>>>>>> upstream-releases
    JITSPEW_CHANNEL_LIST(JITSPEW_CHANNEL)
#  undef JITSPEW_CHANNEL
};

// The IONFILTER environment variable specifies an expression to select only
// certain functions for spewing to reduce amount of log data generated.
static const char* gSpewFilter = nullptr;

static bool FilterContainsLocation(JSScript* function) {
  // If there is no filter we accept all outputs.
  if (!gSpewFilter || !gSpewFilter[0]) {
    return true;
  }

<<<<<<< HEAD
  // Disable wasm output when filter is set.
  if (!function) {
    return false;
  }

  const char* filename = function->filename();
  const size_t line = function->lineno();
  const size_t filelen = strlen(filename);
  const char* index = strstr(gSpewFilter, filename);
  while (index) {
    if (index == gSpewFilter || index[-1] == ',') {
      if (index[filelen] == 0 || index[filelen] == ',') {
        return true;
      }
      if (index[filelen] == ':' && line != size_t(-1)) {
        size_t read_line = strtoul(&index[filelen + 1], nullptr, 10);
        if (read_line == line) {
          return true;
        }
      }
    }
    index = strstr(index + filelen, filename);
  }
  return false;
||||||| merged common ancestors
    const char* filename = function->filename();
    const size_t line = function->lineno();
    const size_t filelen = strlen(filename);
    const char* index = strstr(gSpewFilter, filename);
    while (index) {
        if (index == gSpewFilter || index[-1] == ',') {
            if (index[filelen] == 0 || index[filelen] == ',') {
                return true;
            }
            if (index[filelen] == ':' && line != size_t(-1)) {
                size_t read_line = strtoul(&index[filelen + 1], nullptr, 10);
                if (read_line == line) {
                    return true;
                }
            }
        }
        index = strstr(index + filelen, filename);
    }
    return false;
}

void
jit::EnableIonDebugSyncLogging()
{
    ionspewer.init();
    ionspewer.setAsyncLogging(false);
    EnableChannel(JitSpew_IonSyncLogs);
}

void
jit::EnableIonDebugAsyncLogging()
{
    ionspewer.init();
    ionspewer.setAsyncLogging(true);
}

void
IonSpewer::release()
{
    if (c1Output_.isInitialized()) {
        c1Output_.finish();
    }
    if (jsonOutput_.isInitialized()) {
        jsonOutput_.finish();
    }
    inited_ = false;
}

bool
IonSpewer::init()
{
    if (inited_) {
        return true;
    }

    // Filter expression for spewing
    gSpewFilter = getenv("IONFILTER");

    const size_t bufferLength = 256;
    char c1Buffer[bufferLength];
    char jsonBuffer[bufferLength];
    const char *c1Filename = JIT_SPEW_DIR "/ion.cfg";
    const char *jsonFilename = JIT_SPEW_DIR "/ion.json";

    const char* usePid = getenv("ION_SPEW_BY_PID");
    if (usePid && *usePid != 0) {
        uint32_t pid = getpid();
        size_t len;
        len = snprintf(jsonBuffer, bufferLength, JIT_SPEW_DIR "/ion%" PRIu32 ".json", pid);
        if (bufferLength <= len) {
            fprintf(stderr, "Warning: IonSpewer::init: Cannot serialize file name.");
            return false;
        }
        jsonFilename = jsonBuffer;

        len = snprintf(c1Buffer, bufferLength, JIT_SPEW_DIR "/ion%" PRIu32 ".cfg", pid);
        if (bufferLength <= len) {
            fprintf(stderr, "Warning: IonSpewer::init: Cannot serialize file name.");
            return false;
        }
        c1Filename = c1Buffer;
    }

    if (!c1Output_.init(c1Filename) ||
        !jsonOutput_.init(jsonFilename))
    {
        release();
        return false;
    }

    jsonOutput_.printf("{\n  \"functions\": [\n");
    firstFunction_ = true;

    inited_ = true;
    return true;
}

void
IonSpewer::beginFunction()
{
    // If we are doing a synchronous logging then we spew everything as we go,
    // as this is useful in case of failure during the compilation. On the other
    // hand, it is recommended to disable off thread compilation.
    if (!getAsyncLogging() && !firstFunction_) {
        LockGuard<Mutex> guard(outputLock_);
        jsonOutput_.put(","); // separate functions
    }
}

void
IonSpewer::spewPass(GraphSpewer* gs)
{
    if (!getAsyncLogging()) {
        LockGuard<Mutex> guard(outputLock_);
        gs->dump(c1Output_, jsonOutput_);
    }
}

void
IonSpewer::endFunction(GraphSpewer* gs)
{
    LockGuard<Mutex> guard(outputLock_);
    if (getAsyncLogging() && !firstFunction_) {
        jsonOutput_.put(","); // separate functions
    }

    gs->dump(c1Output_, jsonOutput_);
    firstFunction_ = false;
}

IonSpewer::~IonSpewer()
{
    if (!inited_) {
        return;
    }

    jsonOutput_.printf("\n]}\n");
    release();
=======
  // Disable wasm output when filter is set.
  if (!function) {
    return false;
  }

  const char* filename = function->filename();
  const size_t line = function->lineno();
  const size_t filelen = strlen(filename);
  const char* index = strstr(gSpewFilter, filename);
  while (index) {
    if (index == gSpewFilter || index[-1] == ',') {
      if (index[filelen] == 0 || index[filelen] == ',') {
        return true;
      }
      if (index[filelen] == ':' && line != size_t(-1)) {
        size_t read_line = strtoul(&index[filelen + 1], nullptr, 10);
        if (read_line == line) {
          return true;
        }
      }
    }
    index = strstr(index + filelen, filename);
  }
  return false;
}

void jit::EnableIonDebugSyncLogging() {
  ionspewer.init();
  ionspewer.setAsyncLogging(false);
  EnableChannel(JitSpew_IonSyncLogs);
}

void jit::EnableIonDebugAsyncLogging() {
  ionspewer.init();
  ionspewer.setAsyncLogging(true);
}

void IonSpewer::release() {
  if (jsonOutput_.isInitialized()) {
    jsonOutput_.finish();
  }
  inited_ = false;
}

bool IonSpewer::init() {
  if (inited_) {
    return true;
  }

  // Filter expression for spewing
  gSpewFilter = getenv("IONFILTER");

  const size_t bufferLength = 256;
  char jsonBuffer[bufferLength];
  const char* jsonFilename = JIT_SPEW_DIR "/ion.json";

  const char* usePid = getenv("ION_SPEW_BY_PID");
  if (usePid && *usePid != 0) {
    uint32_t pid = getpid();
    size_t len;
    len = snprintf(jsonBuffer, bufferLength,
                   JIT_SPEW_DIR "/ion%" PRIu32 ".json", pid);
    if (bufferLength <= len) {
      fprintf(stderr, "Warning: IonSpewer::init: Cannot serialize file name.");
      return false;
    }
    jsonFilename = jsonBuffer;
  }

  if (!jsonOutput_.init(jsonFilename)) {
    release();
    return false;
  }

  jsonOutput_.printf("{\n  \"functions\": [\n");
  firstFunction_ = true;

  inited_ = true;
  return true;
}

void IonSpewer::beginFunction() {
  // If we are doing a synchronous logging then we spew everything as we go,
  // as this is useful in case of failure during the compilation. On the other
  // hand, it is recommended to disable off thread compilation.
  if (!getAsyncLogging() && !firstFunction_) {
    LockGuard<Mutex> guard(outputLock_);
    jsonOutput_.put(",");  // separate functions
  }
>>>>>>> upstream-releases
}

<<<<<<< HEAD
void jit::EnableIonDebugSyncLogging() {
  ionspewer.init();
  ionspewer.setAsyncLogging(false);
  EnableChannel(JitSpew_IonSyncLogs);
||||||| merged common ancestors
GraphSpewer::GraphSpewer(TempAllocator *alloc)
  : graph_(nullptr),
    c1Printer_(alloc->lifoAlloc()),
    jsonPrinter_(alloc->lifoAlloc()),
    c1Spewer_(c1Printer_),
    jsonSpewer_(jsonPrinter_)
{
}

void
GraphSpewer::init(MIRGraph* graph, JSScript* function)
{
    MOZ_ASSERT(!isSpewing());
    if (!ionspewer.isEnabled()) {
        return;
    }

    if (!FilterContainsLocation(function)) {
        // filter out logs during the compilation.
        filteredOutCompilations++;
        MOZ_ASSERT(!isSpewing());
        return;
    }

    graph_ = graph;
    MOZ_ASSERT(isSpewing());
=======
void IonSpewer::spewPass(GraphSpewer* gs) {
  if (!getAsyncLogging()) {
    LockGuard<Mutex> guard(outputLock_);
    gs->dump(jsonOutput_);
  }
}

void IonSpewer::endFunction(GraphSpewer* gs) {
  LockGuard<Mutex> guard(outputLock_);
  if (getAsyncLogging() && !firstFunction_) {
    jsonOutput_.put(",");  // separate functions
  }

  gs->dump(jsonOutput_);
  firstFunction_ = false;
>>>>>>> upstream-releases
}

<<<<<<< HEAD
void jit::EnableIonDebugAsyncLogging() {
  ionspewer.init();
  ionspewer.setAsyncLogging(true);
||||||| merged common ancestors
void
GraphSpewer::beginFunction(JSScript* function)
{
    if (!isSpewing()) {
        return;
    }

    c1Spewer_.beginFunction(graph_, function);
    jsonSpewer_.beginFunction(function);

    ionspewer.beginFunction();
=======
IonSpewer::~IonSpewer() {
  if (!inited_) {
    return;
  }

  jsonOutput_.printf("\n]}\n");
  release();
>>>>>>> upstream-releases
}

<<<<<<< HEAD
void IonSpewer::release() {
  if (jsonOutput_.isInitialized()) {
    jsonOutput_.finish();
  }
  inited_ = false;
||||||| merged common ancestors
void
GraphSpewer::spewPass(const char* pass)
{
    if (!isSpewing()) {
        return;
    }

    c1Spewer_.spewPass(pass);

    jsonSpewer_.beginPass(pass);
    jsonSpewer_.spewMIR(graph_);
    jsonSpewer_.spewLIR(graph_);
    jsonSpewer_.endPass();

    ionspewer.spewPass(this);

    // As this function is used for debugging, we ignore any of the previous
    // failures and ensure there is enough ballast space, such that we do not
    // exhaust the ballast space before running the next phase.
    AutoEnterOOMUnsafeRegion oomUnsafe;
    if (!graph_->alloc().ensureBallast()) {
        oomUnsafe.crash("Could not ensure enough ballast space after spewing graph information.");
    }
=======
GraphSpewer::GraphSpewer(TempAllocator* alloc)
    : graph_(nullptr),
      jsonPrinter_(alloc->lifoAlloc()),
      jsonSpewer_(jsonPrinter_) {}

void GraphSpewer::init(MIRGraph* graph, JSScript* function) {
  MOZ_ASSERT(!isSpewing());
  if (!ionspewer.isEnabled()) {
    return;
  }

  if (!FilterContainsLocation(function)) {
    // filter out logs during the compilation.
    filteredOutCompilations++;
    MOZ_ASSERT(!isSpewing());
    return;
  }

  graph_ = graph;
  MOZ_ASSERT(isSpewing());
>>>>>>> upstream-releases
}

<<<<<<< HEAD
bool IonSpewer::init() {
  if (inited_) {
    return true;
  }

  // Filter expression for spewing
  gSpewFilter = getenv("IONFILTER");

  const size_t bufferLength = 256;
  char jsonBuffer[bufferLength];
  const char* jsonFilename = JIT_SPEW_DIR "/ion.json";

  const char* usePid = getenv("ION_SPEW_BY_PID");
  if (usePid && *usePid != 0) {
    uint32_t pid = getpid();
    size_t len;
    len = snprintf(jsonBuffer, bufferLength,
                   JIT_SPEW_DIR "/ion%" PRIu32 ".json", pid);
    if (bufferLength <= len) {
      fprintf(stderr, "Warning: IonSpewer::init: Cannot serialize file name.");
      return false;
    }
    jsonFilename = jsonBuffer;
  }

  if (!jsonOutput_.init(jsonFilename)) {
    release();
    return false;
  }

  jsonOutput_.printf("{\n  \"functions\": [\n");
  firstFunction_ = true;
||||||| merged common ancestors
void
GraphSpewer::spewPass(const char* pass, BacktrackingAllocator* ra)
{
    if (!isSpewing()) {
        return;
    }

    c1Spewer_.spewPass(pass);
    c1Spewer_.spewRanges(pass, ra);

    jsonSpewer_.beginPass(pass);
    jsonSpewer_.spewMIR(graph_);
    jsonSpewer_.spewLIR(graph_);
    jsonSpewer_.spewRanges(ra);
    jsonSpewer_.endPass();

    ionspewer.spewPass(this);
}

void
GraphSpewer::endFunction()
{
    if (!ionspewer.isEnabled()) {
        return;
    }

    if (!isSpewing()) {
        MOZ_ASSERT(filteredOutCompilations != 0);
        filteredOutCompilations--;
        return;
    }

    c1Spewer_.endFunction();
    jsonSpewer_.endFunction();

    ionspewer.endFunction(this);
    graph_ = nullptr;
}

void
GraphSpewer::dump(Fprinter& c1Out, Fprinter& jsonOut)
{
    if (!c1Printer_.hadOutOfMemory()) {
        c1Printer_.exportInto(c1Out);
        c1Out.flush();
    }
    c1Printer_.clear();

    if (!jsonPrinter_.hadOutOfMemory()) {
        jsonPrinter_.exportInto(jsonOut);
    } else {
        jsonOut.put("{}");
    }
    jsonOut.flush();
    jsonPrinter_.clear();
}
=======
void GraphSpewer::beginFunction(JSScript* function) {
  if (!isSpewing()) {
    return;
  }
  jsonSpewer_.beginFunction(function);
  ionspewer.beginFunction();
}

void GraphSpewer::beginWasmFunction(unsigned funcIndex) {
  if (!isSpewing()) {
    return;
  }
  jsonSpewer_.beginWasmFunction(funcIndex);
  ionspewer.beginFunction();
}

void GraphSpewer::spewPass(const char* pass) {
  if (!isSpewing()) {
    return;
  }

  jsonSpewer_.beginPass(pass);
  jsonSpewer_.spewMIR(graph_);
  jsonSpewer_.spewLIR(graph_);
  jsonSpewer_.endPass();

  ionspewer.spewPass(this);

  // As this function is used for debugging, we ignore any of the previous
  // failures and ensure there is enough ballast space, such that we do not
  // exhaust the ballast space before running the next phase.
  AutoEnterOOMUnsafeRegion oomUnsafe;
  if (!graph_->alloc().ensureBallast()) {
    oomUnsafe.crash(
        "Could not ensure enough ballast space after spewing graph "
        "information.");
  }
}

void GraphSpewer::spewPass(const char* pass, BacktrackingAllocator* ra) {
  if (!isSpewing()) {
    return;
  }

  jsonSpewer_.beginPass(pass);
  jsonSpewer_.spewMIR(graph_);
  jsonSpewer_.spewLIR(graph_);
  jsonSpewer_.spewRanges(ra);
  jsonSpewer_.endPass();

  ionspewer.spewPass(this);
}

void GraphSpewer::endFunction() {
  if (!ionspewer.isEnabled()) {
    return;
  }

  if (!isSpewing()) {
    MOZ_ASSERT(filteredOutCompilations != 0);
    filteredOutCompilations--;
    return;
  }

  jsonSpewer_.endFunction();

  ionspewer.endFunction(this);
  graph_ = nullptr;
}

void GraphSpewer::dump(Fprinter& jsonOut) {
  if (!jsonPrinter_.hadOutOfMemory()) {
    jsonPrinter_.exportInto(jsonOut);
  } else {
    jsonOut.put("{}");
  }
  jsonOut.flush();
  jsonPrinter_.clear();
}

void jit::SpewBeginFunction(MIRGenerator* mir, JSScript* function) {
  MIRGraph* graph = &mir->graph();
  mir->graphSpewer().init(graph, function);
  mir->graphSpewer().beginFunction(function);
}

void jit::SpewBeginWasmFunction(MIRGenerator* mir, unsigned funcIndex) {
  MIRGraph* graph = &mir->graph();
  mir->graphSpewer().init(graph, nullptr);
  mir->graphSpewer().beginWasmFunction(funcIndex);
}

AutoSpewEndFunction::~AutoSpewEndFunction() {
  mir_->graphSpewer().endFunction();
}

Fprinter& jit::JitSpewPrinter() {
  static Fprinter out;
  return out;
}

static void PrintHelpAndExit(int status = 0) {
  fflush(nullptr);
  printf(
      "\n"
      "usage: IONFLAGS=option,option,option,... where options can be:\n"
      "\n"
      "  aborts        Compilation abort messages\n"
      "  scripts       Compiled scripts\n"
      "  mir           MIR information\n"
      "  prune         Prune unused branches\n"
      "  escape        Escape analysis\n"
      "  alias         Alias analysis\n"
      "  alias-sum     Alias analysis: shows summaries for every block\n"
      "  gvn           Global Value Numbering\n"
      "  licm          Loop invariant code motion\n"
      "  flac          Fold linear arithmetic constants\n"
      "  eaa           Effective address analysis\n"
      "  sink          Sink transformation\n"
      "  regalloc      Register allocation\n"
      "  inline        Inlining\n"
      "  snapshots     Snapshot information\n"
      "  codegen       Native code generation\n"
      "  bailouts      Bailouts\n"
      "  caches        Inline caches\n"
      "  osi           Invalidation\n"
      "  safepoints    Safepoints\n"
      "  pools         Literal Pools (ARM only for now)\n"
      "  cacheflush    Instruction Cache flushes (ARM only for now)\n"
      "  range         Range Analysis\n"
      "  logs          JSON visualization logging\n"
      "  logs-sync     Same as logs, but flushes between each pass (sync. "
      "compiled functions only).\n"
      "  profiling     Profiling-related information\n"
      "  trackopts     Optimization tracking information gathered by the "
      "Gecko profiler. "
      "(Note: call enableGeckoProfiling() in your script to enable it).\n"
      "  trackopts-ext Encoding information about optimization tracking\n"
      "  dump-mir-expr Dump the MIR expressions\n"
      "  cfg           Control flow graph generation\n"
      "  scriptstats   Tracelogger summary stats\n"
      "  all           Everything\n"
      "\n"
      "  bl-aborts     Baseline compiler abort messages\n"
      "  bl-scripts    Baseline script-compilation\n"
      "  bl-op         Baseline compiler detailed op-specific messages\n"
      "  bl-ic         Baseline inline-cache messages\n"
      "  bl-ic-fb      Baseline IC fallback stub messages\n"
      "  bl-osr        Baseline IC OSR messages\n"
      "  bl-bails      Baseline bailouts\n"
      "  bl-dbg-osr    Baseline debug mode on stack recompile messages\n"
      "  bl-all        All baseline spew\n"
      "\n"
      "See also SPEW=help for information on the Structured Spewer."
      "\n");
  exit(status);
}

static bool IsFlag(const char* found, const char* flag) {
  return strlen(found) == strlen(flag) && strcmp(found, flag) == 0;
}

void jit::CheckLogging() {
  if (LoggingChecked) {
    return;
  }

  LoggingChecked = true;

  char* env = getenv("IONFLAGS");
  if (!env) {
    return;
  }

  const char* found = strtok(env, ",");
  while (found) {
    fprintf(stderr, "found tag: %s\n", found);
    // We're at the end of a flag; check if the previous substring was a
    // known flag (i-1 is the last character of the flag we just read).
    if (IsFlag(found, "help")) {
      PrintHelpAndExit();
    } else if (IsFlag(found, "aborts")) {
      EnableChannel(JitSpew_IonAbort);
    } else if (IsFlag(found, "prune")) {
      EnableChannel(JitSpew_Prune);
    } else if (IsFlag(found, "escape")) {
      EnableChannel(JitSpew_Escape);
    } else if (IsFlag(found, "alias")) {
      EnableChannel(JitSpew_Alias);
    } else if (IsFlag(found, "alias-sum")) {
      EnableChannel(JitSpew_AliasSummaries);
    } else if (IsFlag(found, "scripts")) {
      EnableChannel(JitSpew_IonScripts);
    } else if (IsFlag(found, "mir")) {
      EnableChannel(JitSpew_IonMIR);
    } else if (IsFlag(found, "gvn")) {
      EnableChannel(JitSpew_GVN);
    } else if (IsFlag(found, "range")) {
      EnableChannel(JitSpew_Range);
    } else if (IsFlag(found, "licm")) {
      EnableChannel(JitSpew_LICM);
    } else if (IsFlag(found, "flac")) {
      EnableChannel(JitSpew_FLAC);
    } else if (IsFlag(found, "eaa")) {
      EnableChannel(JitSpew_EAA);
    } else if (IsFlag(found, "sink")) {
      EnableChannel(JitSpew_Sink);
    } else if (IsFlag(found, "regalloc")) {
      EnableChannel(JitSpew_RegAlloc);
    } else if (IsFlag(found, "inline")) {
      EnableChannel(JitSpew_Inlining);
    } else if (IsFlag(found, "snapshots")) {
      EnableChannel(JitSpew_IonSnapshots);
    } else if (IsFlag(found, "codegen")) {
      EnableChannel(JitSpew_Codegen);
    } else if (IsFlag(found, "bailouts")) {
      EnableChannel(JitSpew_IonBailouts);
    } else if (IsFlag(found, "osi")) {
      EnableChannel(JitSpew_IonInvalidate);
    } else if (IsFlag(found, "caches")) {
      EnableChannel(JitSpew_IonIC);
    } else if (IsFlag(found, "safepoints")) {
      EnableChannel(JitSpew_Safepoints);
    } else if (IsFlag(found, "pools")) {
      EnableChannel(JitSpew_Pools);
    } else if (IsFlag(found, "cacheflush")) {
      EnableChannel(JitSpew_CacheFlush);
    } else if (IsFlag(found, "logs")) {
      EnableIonDebugAsyncLogging();
    } else if (IsFlag(found, "logs-sync")) {
      EnableIonDebugSyncLogging();
    } else if (IsFlag(found, "profiling")) {
      EnableChannel(JitSpew_Profiling);
    } else if (IsFlag(found, "trackopts")) {
      JitOptions.disableOptimizationTracking = false;
      EnableChannel(JitSpew_OptimizationTracking);
    } else if (IsFlag(found, "trackopts-ext")) {
      EnableChannel(JitSpew_OptimizationTrackingExtended);
    } else if (IsFlag(found, "dump-mir-expr")) {
      EnableChannel(JitSpew_MIRExpressions);
    } else if (IsFlag(found, "cfg")) {
      EnableChannel(JitSpew_CFG);
    } else if (IsFlag(found, "scriptstats")) {
      EnableChannel(JitSpew_ScriptStats);
    } else if (IsFlag(found, "all")) {
      LoggingBits = uint64_t(-1);
    } else if (IsFlag(found, "bl-aborts")) {
      EnableChannel(JitSpew_BaselineAbort);
    } else if (IsFlag(found, "bl-scripts")) {
      EnableChannel(JitSpew_BaselineScripts);
    } else if (IsFlag(found, "bl-op")) {
      EnableChannel(JitSpew_BaselineOp);
    } else if (IsFlag(found, "bl-ic")) {
      EnableChannel(JitSpew_BaselineIC);
    } else if (IsFlag(found, "bl-ic-fb")) {
      EnableChannel(JitSpew_BaselineICFallback);
    } else if (IsFlag(found, "bl-osr")) {
      EnableChannel(JitSpew_BaselineOSR);
    } else if (IsFlag(found, "bl-bails")) {
      EnableChannel(JitSpew_BaselineBailouts);
    } else if (IsFlag(found, "bl-dbg-osr")) {
      EnableChannel(JitSpew_BaselineDebugModeOSR);
    } else if (IsFlag(found, "bl-all")) {
      EnableChannel(JitSpew_BaselineAbort);
      EnableChannel(JitSpew_BaselineScripts);
      EnableChannel(JitSpew_BaselineOp);
      EnableChannel(JitSpew_BaselineIC);
      EnableChannel(JitSpew_BaselineICFallback);
      EnableChannel(JitSpew_BaselineOSR);
      EnableChannel(JitSpew_BaselineBailouts);
      EnableChannel(JitSpew_BaselineDebugModeOSR);
    } else {
      fprintf(stderr, "Unknown flag.\n");
      PrintHelpAndExit(64);
    }
    found = strtok(nullptr, ",");
  }
>>>>>>> upstream-releases

<<<<<<< HEAD
  inited_ = true;
  return true;
||||||| merged common ancestors
void
jit::SpewBeginFunction(MIRGenerator* mir, JSScript* function)
{
    MIRGraph* graph = &mir->graph();
    mir->graphSpewer().init(graph, function);
    mir->graphSpewer().beginFunction(function);
=======
  FILE* spewfh = stderr;
  const char* filename = getenv("ION_SPEW_FILENAME");
  if (filename && *filename) {
    char actual_filename[2048] = {0};
    SprintfLiteral(actual_filename, "%s.%d", filename, getpid());
    spewfh = fopen(actual_filename, "w");
    MOZ_RELEASE_ASSERT(spewfh);
    setbuf(spewfh, nullptr);  // Make unbuffered
  }
  JitSpewPrinter().init(spewfh);
>>>>>>> upstream-releases
}

<<<<<<< HEAD
void IonSpewer::beginFunction() {
  // If we are doing a synchronous logging then we spew everything as we go,
  // as this is useful in case of failure during the compilation. On the other
  // hand, it is recommended to disable off thread compilation.
  if (!getAsyncLogging() && !firstFunction_) {
    LockGuard<Mutex> guard(outputLock_);
    jsonOutput_.put(",");  // separate functions
  }
||||||| merged common ancestors
AutoSpewEndFunction::~AutoSpewEndFunction()
{
    mir_->graphSpewer().endFunction();
=======
JitSpewIndent::JitSpewIndent(JitSpewChannel channel) : channel_(channel) {
  ChannelIndentLevel[channel]++;
>>>>>>> upstream-releases
}

<<<<<<< HEAD
void IonSpewer::spewPass(GraphSpewer* gs) {
  if (!getAsyncLogging()) {
    LockGuard<Mutex> guard(outputLock_);
    gs->dump(jsonOutput_);
  }
}
||||||| merged common ancestors
Fprinter&
jit::JitSpewPrinter()
{
    static Fprinter out;
    return out;
}
=======
JitSpewIndent::~JitSpewIndent() { ChannelIndentLevel[channel_]--; }
>>>>>>> upstream-releases

<<<<<<< HEAD
void IonSpewer::endFunction(GraphSpewer* gs) {
  LockGuard<Mutex> guard(outputLock_);
  if (getAsyncLogging() && !firstFunction_) {
    jsonOutput_.put(",");  // separate functions
  }
||||||| merged common ancestors
=======
void jit::JitSpewStartVA(JitSpewChannel channel, const char* fmt, va_list ap) {
  if (!JitSpewEnabled(channel)) {
    return;
  }
>>>>>>> upstream-releases

<<<<<<< HEAD
  gs->dump(jsonOutput_);
  firstFunction_ = false;
||||||| merged common ancestors
static bool
ContainsFlag(const char* str, const char* flag)
{
    size_t flaglen = strlen(flag);
    const char* index = strstr(str, flag);
    while (index) {
        if ((index == str || index[-1] == ',') && (index[flaglen] == 0 || index[flaglen] == ',')) {
            return true;
        }
        index = strstr(index + flaglen, flag);
    }
    return false;
=======
  JitSpewHeader(channel);
  Fprinter& out = JitSpewPrinter();
  out.vprintf(fmt, ap);
>>>>>>> upstream-releases
}

<<<<<<< HEAD
IonSpewer::~IonSpewer() {
  if (!inited_) {
    return;
  }

  jsonOutput_.printf("\n]}\n");
  release();
||||||| merged common ancestors
void
jit::CheckLogging()
{
    if (LoggingChecked) {
        return;
    }
    LoggingChecked = true;
    const char* env = getenv("IONFLAGS");
    if (!env) {
        return;
    }
    if (strstr(env, "help")) {
        fflush(nullptr);
        printf(
            "\n"
            "usage: IONFLAGS=option,option,option,... where options can be:\n"
            "\n"
            "  aborts        Compilation abort messages\n"
            "  scripts       Compiled scripts\n"
            "  mir           MIR information\n"
            "  prune         Prune unused branches\n"
            "  escape        Escape analysis\n"
            "  alias         Alias analysis\n"
            "  alias-sum     Alias analysis: shows summaries for every block\n"
            "  gvn           Global Value Numbering\n"
            "  licm          Loop invariant code motion\n"
            "  flac          Fold linear arithmetic constants\n"
            "  eaa           Effective address analysis\n"
            "  sincos        Replace sin/cos by sincos\n"
            "  sink          Sink transformation\n"
            "  regalloc      Register allocation\n"
            "  inline        Inlining\n"
            "  snapshots     Snapshot information\n"
            "  codegen       Native code generation\n"
            "  bailouts      Bailouts\n"
            "  caches        Inline caches\n"
            "  osi           Invalidation\n"
            "  safepoints    Safepoints\n"
            "  pools         Literal Pools (ARM only for now)\n"
            "  cacheflush    Instruction Cache flushes (ARM only for now)\n"
            "  range         Range Analysis\n"
            "  unroll        Loop unrolling\n"
            "  logs          C1 and JSON visualization logging\n"
            "  logs-sync     Same as logs, but flushes between each pass (sync. compiled functions only).\n"
            "  profiling     Profiling-related information\n"
            "  trackopts     Optimization tracking information gathered by the Gecko profiler. "
                            "(Note: call enableGeckoProfiling() in your script to enable it).\n"
            "  trackopts-ext Encoding information about optimization tracking\n"
            "  dump-mir-expr Dump the MIR expressions\n"
            "  cfg           Control flow graph generation\n"
            "  all           Everything\n"
            "\n"
            "  bl-aborts     Baseline compiler abort messages\n"
            "  bl-scripts    Baseline script-compilation\n"
            "  bl-op         Baseline compiler detailed op-specific messages\n"
            "  bl-ic         Baseline inline-cache messages\n"
            "  bl-ic-fb      Baseline IC fallback stub messages\n"
            "  bl-osr        Baseline IC OSR messages\n"
            "  bl-bails      Baseline bailouts\n"
            "  bl-dbg-osr    Baseline debug mode on stack recompile messages\n"
            "  bl-all        All baseline spew\n"
            "\n"
        );
        exit(0);
        /*NOTREACHED*/
    }
    if (ContainsFlag(env, "aborts")) {
        EnableChannel(JitSpew_IonAbort);
    }
    if (ContainsFlag(env, "prune")) {
        EnableChannel(JitSpew_Prune);
    }
    if (ContainsFlag(env, "escape")) {
        EnableChannel(JitSpew_Escape);
    }
    if (ContainsFlag(env, "alias")) {
        EnableChannel(JitSpew_Alias);
    }
    if (ContainsFlag(env, "alias-sum")) {
        EnableChannel(JitSpew_AliasSummaries);
    }
    if (ContainsFlag(env, "scripts")) {
        EnableChannel(JitSpew_IonScripts);
    }
    if (ContainsFlag(env, "mir")) {
        EnableChannel(JitSpew_IonMIR);
    }
    if (ContainsFlag(env, "gvn")) {
        EnableChannel(JitSpew_GVN);
    }
    if (ContainsFlag(env, "range")) {
        EnableChannel(JitSpew_Range);
    }
    if (ContainsFlag(env, "unroll")) {
        EnableChannel(JitSpew_Unrolling);
    }
    if (ContainsFlag(env, "licm")) {
        EnableChannel(JitSpew_LICM);
    }
    if (ContainsFlag(env, "flac")) {
        EnableChannel(JitSpew_FLAC);
    }
    if (ContainsFlag(env, "eaa")) {
        EnableChannel(JitSpew_EAA);
    }
    if (ContainsFlag(env, "sincos")) {
        EnableChannel(JitSpew_Sincos);
    }
    if (ContainsFlag(env, "sink")) {
        EnableChannel(JitSpew_Sink);
    }
    if (ContainsFlag(env, "regalloc")) {
        EnableChannel(JitSpew_RegAlloc);
    }
    if (ContainsFlag(env, "inline")) {
        EnableChannel(JitSpew_Inlining);
    }
    if (ContainsFlag(env, "snapshots")) {
        EnableChannel(JitSpew_IonSnapshots);
    }
    if (ContainsFlag(env, "codegen")) {
        EnableChannel(JitSpew_Codegen);
    }
    if (ContainsFlag(env, "bailouts")) {
        EnableChannel(JitSpew_IonBailouts);
    }
    if (ContainsFlag(env, "osi")) {
        EnableChannel(JitSpew_IonInvalidate);
    }
    if (ContainsFlag(env, "caches")) {
        EnableChannel(JitSpew_IonIC);
    }
    if (ContainsFlag(env, "safepoints")) {
        EnableChannel(JitSpew_Safepoints);
    }
    if (ContainsFlag(env, "pools")) {
        EnableChannel(JitSpew_Pools);
    }
    if (ContainsFlag(env, "cacheflush")) {
        EnableChannel(JitSpew_CacheFlush);
    }
    if (ContainsFlag(env, "logs")) {
        EnableIonDebugAsyncLogging();
    }
    if (ContainsFlag(env, "logs-sync")) {
        EnableIonDebugSyncLogging();
    }
    if (ContainsFlag(env, "profiling")) {
        EnableChannel(JitSpew_Profiling);
    }
    if (ContainsFlag(env, "trackopts")) {
        JitOptions.disableOptimizationTracking = false;
        EnableChannel(JitSpew_OptimizationTracking);
    }
    if (ContainsFlag(env, "trackopts-ext")) {
        EnableChannel(JitSpew_OptimizationTrackingExtended);
    }
    if (ContainsFlag(env, "dump-mir-expr")) {
        EnableChannel(JitSpew_MIRExpressions);
    }
    if (ContainsFlag(env, "cfg")) {
        EnableChannel(JitSpew_CFG);
    }
    if (ContainsFlag(env, "all")) {
        LoggingBits = uint64_t(-1);
    }

    if (ContainsFlag(env, "bl-aborts")) {
        EnableChannel(JitSpew_BaselineAbort);
    }
    if (ContainsFlag(env, "bl-scripts")) {
        EnableChannel(JitSpew_BaselineScripts);
    }
    if (ContainsFlag(env, "bl-op")) {
        EnableChannel(JitSpew_BaselineOp);
    }
    if (ContainsFlag(env, "bl-ic")) {
        EnableChannel(JitSpew_BaselineIC);
    }
    if (ContainsFlag(env, "bl-ic-fb")) {
        EnableChannel(JitSpew_BaselineICFallback);
    }
    if (ContainsFlag(env, "bl-osr")) {
        EnableChannel(JitSpew_BaselineOSR);
    }
    if (ContainsFlag(env, "bl-bails")) {
        EnableChannel(JitSpew_BaselineBailouts);
    }
    if (ContainsFlag(env, "bl-dbg-osr")) {
        EnableChannel(JitSpew_BaselineDebugModeOSR);
    }
    if (ContainsFlag(env, "bl-all")) {
        EnableChannel(JitSpew_BaselineAbort);
        EnableChannel(JitSpew_BaselineScripts);
        EnableChannel(JitSpew_BaselineOp);
        EnableChannel(JitSpew_BaselineIC);
        EnableChannel(JitSpew_BaselineICFallback);
        EnableChannel(JitSpew_BaselineOSR);
        EnableChannel(JitSpew_BaselineBailouts);
        EnableChannel(JitSpew_BaselineDebugModeOSR);
    }

    FILE* spewfh = stderr;
    const char* filename = getenv("ION_SPEW_FILENAME");
    if (filename && *filename) {
        spewfh = fopen(filename, "w");
        MOZ_RELEASE_ASSERT(spewfh);
        setbuf(spewfh, nullptr); // Make unbuffered
    }
    JitSpewPrinter().init(spewfh);
=======
void jit::JitSpewContVA(JitSpewChannel channel, const char* fmt, va_list ap) {
  if (!JitSpewEnabled(channel)) {
    return;
  }

  Fprinter& out = JitSpewPrinter();
  out.vprintf(fmt, ap);
>>>>>>> upstream-releases
}

<<<<<<< HEAD
GraphSpewer::GraphSpewer(TempAllocator* alloc)
    : graph_(nullptr),
      jsonPrinter_(alloc->lifoAlloc()),
      jsonSpewer_(jsonPrinter_) {}
||||||| merged common ancestors
JitSpewIndent::JitSpewIndent(JitSpewChannel channel)
  : channel_(channel)
{
    ChannelIndentLevel[channel]++;
}
=======
void jit::JitSpewFin(JitSpewChannel channel) {
  if (!JitSpewEnabled(channel)) {
    return;
  }
>>>>>>> upstream-releases

<<<<<<< HEAD
void GraphSpewer::init(MIRGraph* graph, JSScript* function) {
  MOZ_ASSERT(!isSpewing());
  if (!ionspewer.isEnabled()) {
    return;
  }
||||||| merged common ancestors
JitSpewIndent::~JitSpewIndent()
{
    ChannelIndentLevel[channel_]--;
}
=======
  Fprinter& out = JitSpewPrinter();
  out.put("\n");
}
>>>>>>> upstream-releases

<<<<<<< HEAD
  if (!FilterContainsLocation(function)) {
    // filter out logs during the compilation.
    filteredOutCompilations++;
    MOZ_ASSERT(!isSpewing());
    return;
  }

  graph_ = graph;
  MOZ_ASSERT(isSpewing());
||||||| merged common ancestors
void
jit::JitSpewStartVA(JitSpewChannel channel, const char* fmt, va_list ap)
{
    if (!JitSpewEnabled(channel)) {
        return;
    }

    JitSpewHeader(channel);
    Fprinter& out = JitSpewPrinter();
    out.vprintf(fmt, ap);
=======
void jit::JitSpewVA(JitSpewChannel channel, const char* fmt, va_list ap) {
  JitSpewStartVA(channel, fmt, ap);
  JitSpewFin(channel);
>>>>>>> upstream-releases
}

<<<<<<< HEAD
void GraphSpewer::beginFunction(JSScript* function) {
  if (!isSpewing()) {
    return;
  }

  jsonSpewer_.beginFunction(function);
||||||| merged common ancestors
void
jit::JitSpewContVA(JitSpewChannel channel, const char* fmt, va_list ap)
{
    if (!JitSpewEnabled(channel)) {
        return;
    }

    Fprinter& out = JitSpewPrinter();
    out.vprintf(fmt, ap);
}
=======
void jit::JitSpew(JitSpewChannel channel, const char* fmt, ...) {
  va_list ap;
  va_start(ap, fmt);
  JitSpewVA(channel, fmt, ap);
  va_end(ap);
}
>>>>>>> upstream-releases

<<<<<<< HEAD
  ionspewer.beginFunction();
||||||| merged common ancestors
void
jit::JitSpewFin(JitSpewChannel channel)
{
    if (!JitSpewEnabled(channel)) {
        return;
    }

    Fprinter& out = JitSpewPrinter();
    out.put("\n");
=======
void jit::JitSpewDef(JitSpewChannel channel, const char* str,
                     MDefinition* def) {
  if (!JitSpewEnabled(channel)) {
    return;
  }

  JitSpewHeader(channel);
  Fprinter& out = JitSpewPrinter();
  out.put(str);
  def->dump(out);
  def->dumpLocation(out);
>>>>>>> upstream-releases
}

<<<<<<< HEAD
void GraphSpewer::spewPass(const char* pass) {
  if (!isSpewing()) {
    return;
  }

  jsonSpewer_.beginPass(pass);
  jsonSpewer_.spewMIR(graph_);
  jsonSpewer_.spewLIR(graph_);
  jsonSpewer_.endPass();

  ionspewer.spewPass(this);

  // As this function is used for debugging, we ignore any of the previous
  // failures and ensure there is enough ballast space, such that we do not
  // exhaust the ballast space before running the next phase.
  AutoEnterOOMUnsafeRegion oomUnsafe;
  if (!graph_->alloc().ensureBallast()) {
    oomUnsafe.crash(
        "Could not ensure enough ballast space after spewing graph "
        "information.");
  }
}

void GraphSpewer::spewPass(const char* pass, BacktrackingAllocator* ra) {
  if (!isSpewing()) {
    return;
  }

  jsonSpewer_.beginPass(pass);
  jsonSpewer_.spewMIR(graph_);
  jsonSpewer_.spewLIR(graph_);
  jsonSpewer_.spewRanges(ra);
  jsonSpewer_.endPass();

  ionspewer.spewPass(this);
}

void GraphSpewer::endFunction() {
  if (!ionspewer.isEnabled()) {
    return;
  }

  if (!isSpewing()) {
    MOZ_ASSERT(filteredOutCompilations != 0);
    filteredOutCompilations--;
    return;
  }

  jsonSpewer_.endFunction();

  ionspewer.endFunction(this);
  graph_ = nullptr;
}

void GraphSpewer::dump(Fprinter& jsonOut) {
  if (!jsonPrinter_.hadOutOfMemory()) {
    jsonPrinter_.exportInto(jsonOut);
  } else {
    jsonOut.put("{}");
  }
  jsonOut.flush();
  jsonPrinter_.clear();
}

void jit::SpewBeginFunction(MIRGenerator* mir, JSScript* function) {
  MIRGraph* graph = &mir->graph();
  mir->graphSpewer().init(graph, function);
  mir->graphSpewer().beginFunction(function);
}

AutoSpewEndFunction::~AutoSpewEndFunction() {
  mir_->graphSpewer().endFunction();
}

Fprinter& jit::JitSpewPrinter() {
  static Fprinter out;
  return out;
}

void jit::CheckLogging() {
  if (LoggingChecked) {
    return;
  }
  LoggingChecked = true;
  const char* env = getenv("IONFLAGS");
  if (!env) {
    return;
  }
  if (strstr(env, "help")) {
    fflush(nullptr);
    printf(
        "\n"
        "usage: IONFLAGS=option,option,option,... where options can be:\n"
        "\n"
        "  aborts        Compilation abort messages\n"
        "  scripts       Compiled scripts\n"
        "  mir           MIR information\n"
        "  prune         Prune unused branches\n"
        "  escape        Escape analysis\n"
        "  alias         Alias analysis\n"
        "  alias-sum     Alias analysis: shows summaries for every block\n"
        "  gvn           Global Value Numbering\n"
        "  licm          Loop invariant code motion\n"
        "  flac          Fold linear arithmetic constants\n"
        "  eaa           Effective address analysis\n"
        "  sincos        Replace sin/cos by sincos\n"
        "  sink          Sink transformation\n"
        "  regalloc      Register allocation\n"
        "  inline        Inlining\n"
        "  snapshots     Snapshot information\n"
        "  codegen       Native code generation\n"
        "  bailouts      Bailouts\n"
        "  caches        Inline caches\n"
        "  osi           Invalidation\n"
        "  safepoints    Safepoints\n"
        "  pools         Literal Pools (ARM only for now)\n"
        "  cacheflush    Instruction Cache flushes (ARM only for now)\n"
        "  range         Range Analysis\n"
        "  unroll        Loop unrolling\n"
        "  logs          JSON visualization logging\n"
        "  logs-sync     Same as logs, but flushes between each pass (sync. "
        "compiled functions only).\n"
        "  profiling     Profiling-related information\n"
        "  trackopts     Optimization tracking information gathered by the "
        "Gecko profiler. "
        "(Note: call enableGeckoProfiling() in your script to enable it).\n"
        "  trackopts-ext Encoding information about optimization tracking\n"
        "  dump-mir-expr Dump the MIR expressions\n"
        "  cfg           Control flow graph generation\n"
        "  all           Everything\n"
        "\n"
        "  bl-aborts     Baseline compiler abort messages\n"
        "  bl-scripts    Baseline script-compilation\n"
        "  bl-op         Baseline compiler detailed op-specific messages\n"
        "  bl-ic         Baseline inline-cache messages\n"
        "  bl-ic-fb      Baseline IC fallback stub messages\n"
        "  bl-osr        Baseline IC OSR messages\n"
        "  bl-bails      Baseline bailouts\n"
        "  bl-dbg-osr    Baseline debug mode on stack recompile messages\n"
        "  bl-all        All baseline spew\n"
        "\n"
        "See also SPEW=help for information on the Structured Spewer."
        "\n");
    exit(0);
    /*NOTREACHED*/
  }
  if (ContainsFlag(env, "aborts")) {
    EnableChannel(JitSpew_IonAbort);
  }
  if (ContainsFlag(env, "prune")) {
    EnableChannel(JitSpew_Prune);
  }
  if (ContainsFlag(env, "escape")) {
    EnableChannel(JitSpew_Escape);
  }
  if (ContainsFlag(env, "alias")) {
    EnableChannel(JitSpew_Alias);
  }
  if (ContainsFlag(env, "alias-sum")) {
    EnableChannel(JitSpew_AliasSummaries);
  }
  if (ContainsFlag(env, "scripts")) {
    EnableChannel(JitSpew_IonScripts);
  }
  if (ContainsFlag(env, "mir")) {
    EnableChannel(JitSpew_IonMIR);
  }
  if (ContainsFlag(env, "gvn")) {
    EnableChannel(JitSpew_GVN);
  }
  if (ContainsFlag(env, "range")) {
    EnableChannel(JitSpew_Range);
  }
  if (ContainsFlag(env, "unroll")) {
    EnableChannel(JitSpew_Unrolling);
  }
  if (ContainsFlag(env, "licm")) {
    EnableChannel(JitSpew_LICM);
  }
  if (ContainsFlag(env, "flac")) {
    EnableChannel(JitSpew_FLAC);
  }
  if (ContainsFlag(env, "eaa")) {
    EnableChannel(JitSpew_EAA);
  }
  if (ContainsFlag(env, "sincos")) {
    EnableChannel(JitSpew_Sincos);
  }
  if (ContainsFlag(env, "sink")) {
    EnableChannel(JitSpew_Sink);
  }
  if (ContainsFlag(env, "regalloc")) {
    EnableChannel(JitSpew_RegAlloc);
  }
  if (ContainsFlag(env, "inline")) {
    EnableChannel(JitSpew_Inlining);
  }
  if (ContainsFlag(env, "snapshots")) {
    EnableChannel(JitSpew_IonSnapshots);
  }
  if (ContainsFlag(env, "codegen")) {
    EnableChannel(JitSpew_Codegen);
  }
  if (ContainsFlag(env, "bailouts")) {
    EnableChannel(JitSpew_IonBailouts);
  }
  if (ContainsFlag(env, "osi")) {
    EnableChannel(JitSpew_IonInvalidate);
  }
  if (ContainsFlag(env, "caches")) {
    EnableChannel(JitSpew_IonIC);
  }
  if (ContainsFlag(env, "safepoints")) {
    EnableChannel(JitSpew_Safepoints);
  }
  if (ContainsFlag(env, "pools")) {
    EnableChannel(JitSpew_Pools);
  }
  if (ContainsFlag(env, "cacheflush")) {
    EnableChannel(JitSpew_CacheFlush);
  }
  if (ContainsFlag(env, "logs")) {
    EnableIonDebugAsyncLogging();
  }
  if (ContainsFlag(env, "logs-sync")) {
    EnableIonDebugSyncLogging();
  }
  if (ContainsFlag(env, "profiling")) {
    EnableChannel(JitSpew_Profiling);
  }
  if (ContainsFlag(env, "trackopts")) {
    JitOptions.disableOptimizationTracking = false;
    EnableChannel(JitSpew_OptimizationTracking);
  }
  if (ContainsFlag(env, "trackopts-ext")) {
    EnableChannel(JitSpew_OptimizationTrackingExtended);
  }
  if (ContainsFlag(env, "dump-mir-expr")) {
    EnableChannel(JitSpew_MIRExpressions);
  }
  if (ContainsFlag(env, "cfg")) {
    EnableChannel(JitSpew_CFG);
  }
  if (ContainsFlag(env, "all")) {
    LoggingBits = uint64_t(-1);
  }

  if (ContainsFlag(env, "bl-aborts")) {
    EnableChannel(JitSpew_BaselineAbort);
  }
  if (ContainsFlag(env, "bl-scripts")) {
    EnableChannel(JitSpew_BaselineScripts);
  }
  if (ContainsFlag(env, "bl-op")) {
    EnableChannel(JitSpew_BaselineOp);
  }
  if (ContainsFlag(env, "bl-ic")) {
    EnableChannel(JitSpew_BaselineIC);
  }
  if (ContainsFlag(env, "bl-ic-fb")) {
    EnableChannel(JitSpew_BaselineICFallback);
  }
  if (ContainsFlag(env, "bl-osr")) {
    EnableChannel(JitSpew_BaselineOSR);
  }
  if (ContainsFlag(env, "bl-bails")) {
    EnableChannel(JitSpew_BaselineBailouts);
  }
  if (ContainsFlag(env, "bl-dbg-osr")) {
    EnableChannel(JitSpew_BaselineDebugModeOSR);
  }
  if (ContainsFlag(env, "bl-all")) {
    EnableChannel(JitSpew_BaselineAbort);
    EnableChannel(JitSpew_BaselineScripts);
    EnableChannel(JitSpew_BaselineOp);
    EnableChannel(JitSpew_BaselineIC);
    EnableChannel(JitSpew_BaselineICFallback);
    EnableChannel(JitSpew_BaselineOSR);
    EnableChannel(JitSpew_BaselineBailouts);
    EnableChannel(JitSpew_BaselineDebugModeOSR);
  }

  FILE* spewfh = stderr;
  const char* filename = getenv("ION_SPEW_FILENAME");
  if (filename && *filename) {
    char actual_filename[2048] = {0};
    SprintfLiteral(actual_filename, "%s.%d", filename, getpid());
    spewfh = fopen(actual_filename, "w");
    MOZ_RELEASE_ASSERT(spewfh);
    setbuf(spewfh, nullptr);  // Make unbuffered
  }
  JitSpewPrinter().init(spewfh);
}

JitSpewIndent::JitSpewIndent(JitSpewChannel channel) : channel_(channel) {
  ChannelIndentLevel[channel]++;
}

JitSpewIndent::~JitSpewIndent() { ChannelIndentLevel[channel_]--; }

void jit::JitSpewStartVA(JitSpewChannel channel, const char* fmt, va_list ap) {
  if (!JitSpewEnabled(channel)) {
    return;
  }

  JitSpewHeader(channel);
  Fprinter& out = JitSpewPrinter();
  out.vprintf(fmt, ap);
}

void jit::JitSpewContVA(JitSpewChannel channel, const char* fmt, va_list ap) {
  if (!JitSpewEnabled(channel)) {
    return;
  }

  Fprinter& out = JitSpewPrinter();
  out.vprintf(fmt, ap);
}

void jit::JitSpewFin(JitSpewChannel channel) {
  if (!JitSpewEnabled(channel)) {
    return;
  }

  Fprinter& out = JitSpewPrinter();
  out.put("\n");
}

void jit::JitSpewVA(JitSpewChannel channel, const char* fmt, va_list ap) {
  JitSpewStartVA(channel, fmt, ap);
  JitSpewFin(channel);
}

void jit::JitSpew(JitSpewChannel channel, const char* fmt, ...) {
  va_list ap;
  va_start(ap, fmt);
  JitSpewVA(channel, fmt, ap);
  va_end(ap);
}

void jit::JitSpewDef(JitSpewChannel channel, const char* str,
                     MDefinition* def) {
  if (!JitSpewEnabled(channel)) {
    return;
  }

  JitSpewHeader(channel);
  Fprinter& out = JitSpewPrinter();
  out.put(str);
  def->dump(out);
  def->dumpLocation(out);
}

void jit::JitSpewStart(JitSpewChannel channel, const char* fmt, ...) {
  va_list ap;
  va_start(ap, fmt);
  JitSpewStartVA(channel, fmt, ap);
  va_end(ap);
||||||| merged common ancestors
void
jit::JitSpewVA(JitSpewChannel channel, const char* fmt, va_list ap)
{
    JitSpewStartVA(channel, fmt, ap);
    JitSpewFin(channel);
=======
void jit::JitSpewStart(JitSpewChannel channel, const char* fmt, ...) {
  va_list ap;
  va_start(ap, fmt);
  JitSpewStartVA(channel, fmt, ap);
  va_end(ap);
>>>>>>> upstream-releases
}
void jit::JitSpewCont(JitSpewChannel channel, const char* fmt, ...) {
  va_list ap;
  va_start(ap, fmt);
  JitSpewContVA(channel, fmt, ap);
  va_end(ap);
}

<<<<<<< HEAD
void jit::JitSpewHeader(JitSpewChannel channel) {
  if (!JitSpewEnabled(channel)) {
    return;
  }

  Fprinter& out = JitSpewPrinter();
  out.printf("[%s] ", ChannelNames[channel]);
  for (size_t i = ChannelIndentLevel[channel]; i != 0; i--) {
    out.put("  ");
  }
||||||| merged common ancestors
void
jit::JitSpewDef(JitSpewChannel channel, const char* str, MDefinition* def)
{
    if (!JitSpewEnabled(channel)) {
        return;
    }

    JitSpewHeader(channel);
    Fprinter& out = JitSpewPrinter();
    out.put(str);
    def->dump(out);
    def->dumpLocation(out);
}

void
jit::JitSpewStart(JitSpewChannel channel, const char* fmt, ...)
{
    va_list ap;
    va_start(ap, fmt);
    JitSpewStartVA(channel, fmt, ap);
    va_end(ap);
}
void
jit::JitSpewCont(JitSpewChannel channel, const char* fmt, ...)
{
    va_list ap;
    va_start(ap, fmt);
    JitSpewContVA(channel, fmt, ap);
    va_end(ap);
}

void
jit::JitSpewHeader(JitSpewChannel channel)
{
    if (!JitSpewEnabled(channel)) {
        return;
    }

    Fprinter& out = JitSpewPrinter();
    out.printf("[%s] ", ChannelNames[channel]);
    for (size_t i = ChannelIndentLevel[channel]; i != 0; i--) {
        out.put("  ");
    }
=======
void jit::JitSpewHeader(JitSpewChannel channel) {
  if (!JitSpewEnabled(channel)) {
    return;
  }

  Fprinter& out = JitSpewPrinter();
  out.printf("[%s] ", ChannelNames[channel]);
  for (size_t i = ChannelIndentLevel[channel]; i != 0; i--) {
    out.put("  ");
  }
>>>>>>> upstream-releases
}

bool jit::JitSpewEnabled(JitSpewChannel channel) {
  MOZ_ASSERT(LoggingChecked);
  return (LoggingBits & (uint64_t(1) << uint32_t(channel))) &&
         !filteredOutCompilations;
}

void jit::EnableChannel(JitSpewChannel channel) {
  MOZ_ASSERT(LoggingChecked);
  LoggingBits |= uint64_t(1) << uint32_t(channel);
}

void jit::DisableChannel(JitSpewChannel channel) {
  MOZ_ASSERT(LoggingChecked);
  LoggingBits &= ~(uint64_t(1) << uint32_t(channel));
}

#endif /* JS_JITSPEW */
