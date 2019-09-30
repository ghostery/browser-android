/* This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/. */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "gdb-tests.h"
#include "jsapi.h"
#include "jsfriendapi.h"
#include "js/Initialization.h"
#include "js/Warnings.h"  // JS::SetWarningReporter

using namespace JS;

<<<<<<< HEAD
static const JSClassOps global_classOps = {nullptr,
                                           nullptr,
                                           nullptr,
                                           nullptr,
                                           nullptr,
                                           nullptr,
                                           nullptr,
                                           nullptr,
                                           nullptr,
                                           nullptr,
                                           JS_GlobalObjectTraceHook};

||||||| merged common ancestors
static const JSClassOps global_classOps = {
    nullptr, nullptr, nullptr, nullptr,
    nullptr, nullptr, nullptr, nullptr,
    nullptr, nullptr,
    JS_GlobalObjectTraceHook
};

=======
>>>>>>> upstream-releases
/* The class of the global object. */
<<<<<<< HEAD
static const JSClass global_class = {"global", JSCLASS_GLOBAL_FLAGS,
                                     &global_classOps};
||||||| merged common ancestors
static const JSClass global_class = {
    "global", JSCLASS_GLOBAL_FLAGS,
    &global_classOps
};
=======
static const JSClass global_class = {"global", JSCLASS_GLOBAL_FLAGS,
                                     &DefaultGlobalClassOps};
>>>>>>> upstream-releases

static volatile int dontOptimizeMeAway = 0;

void usePointer(const void* ptr) { dontOptimizeMeAway++; }

template <typename T>
static inline T* checkPtr(T* ptr) {
  if (!ptr) {
    abort();
  }
  return ptr;
}

static void checkBool(bool success) {
  if (!success) {
    abort();
  }
}

/* The warning reporter callback. */
void reportWarning(JSContext* cx, JSErrorReport* report) {
  fprintf(stderr, "%s:%u: %s\n",
          report->filename ? report->filename : "<no filename>",
          (unsigned int)report->lineno, report->message().c_str());
}

// prologue.py sets a breakpoint on this function; test functions can call it
// to easily return control to GDB where desired.
void breakpoint() {
  // If we leave this function empty, the linker will unify it with other
  // empty functions throughout SpiderMonkey. If we then set a GDB
  // breakpoint on it, that breakpoint will hit at all sorts of random
  // times. So make it perform a distinctive side effect.
  fprintf(stderr, "Called " __FILE__ ":breakpoint\n");
}

GDBFragment* GDBFragment::allFragments = nullptr;

<<<<<<< HEAD
int main(int argc, const char** argv) {
  if (!JS_Init()) return 1;
  JSContext* cx = checkPtr(JS_NewContext(1024 * 1024));

  JS_SetGCParameter(cx, JSGC_MAX_BYTES, 0xffffffff);
  JS_SetNativeStackQuota(cx, 5000000);

  checkBool(JS::InitSelfHostedCode(cx));
  JS::SetWarningReporter(cx, reportWarning);

  /* Create the global object. */
  JS::RealmOptions options;
  RootedObject global(
      cx, checkPtr(JS_NewGlobalObject(cx, &global_class, nullptr,
                                      JS::FireOnNewGlobalHook, options)));
  JSAutoRealm ar(cx, global);

  /* Populate the global object with the standard globals,
     like Object and Array. */
  checkBool(JS::InitRealmStandardClasses(cx));

  argv++;
  while (*argv) {
    const char* name = *argv++;
    GDBFragment* fragment;
    for (fragment = GDBFragment::allFragments; fragment;
         fragment = fragment->next) {
      if (strcmp(fragment->name(), name) == 0) {
        fragment->run(cx, argv);
        break;
      }
    }
    if (!fragment) {
      fprintf(stderr, "Unrecognized fragment name: %s\n", name);
      exit(1);
||||||| merged common ancestors
int
main(int argc, const char** argv)
{
    if (!JS_Init()) return 1;
    JSContext* cx = checkPtr(JS_NewContext(1024 * 1024));

    JS_SetGCParameter(cx, JSGC_MAX_BYTES, 0xffffffff);
    JS_SetNativeStackQuota(cx, 5000000);

    checkBool(JS::InitSelfHostedCode(cx));
    JS::SetWarningReporter(cx, reportWarning);

    /* Create the global object. */
    JS::RealmOptions options;
    RootedObject global(cx, checkPtr(JS_NewGlobalObject(cx, &global_class,
                        nullptr, JS::FireOnNewGlobalHook, options)));
    JSAutoRealm ar(cx, global);

    /* Populate the global object with the standard globals,
       like Object and Array. */
    checkBool(JS::InitRealmStandardClasses(cx));

    argv++;
    while (*argv) {
        const char* name = *argv++;
        GDBFragment* fragment;
        for (fragment = GDBFragment::allFragments; fragment; fragment = fragment->next) {
            if (strcmp(fragment->name(), name) == 0) {
                fragment->run(cx, argv);
                break;
            }
        }
        if (!fragment) {
            fprintf(stderr, "Unrecognized fragment name: %s\n", name);
            exit(1);
        }
=======
int main(int argc, const char** argv) {
  if (!JS_Init()) return 1;
  JSContext* cx = checkPtr(JS_NewContext(1024 * 1024));

  JS_SetGCParameter(cx, JSGC_MAX_BYTES, 0xffffffff);
  JS_SetNativeStackQuota(cx, 5000000);

  checkBool(JS::InitSelfHostedCode(cx));
  JS::SetWarningReporter(cx, reportWarning);

  /* Create the global object. */
  JS::RealmOptions options;
  RootedObject global(
      cx, checkPtr(JS_NewGlobalObject(cx, &global_class, nullptr,
                                      JS::FireOnNewGlobalHook, options)));
  JSAutoRealm ar(cx, global);

  argv++;
  while (*argv) {
    const char* name = *argv++;
    GDBFragment* fragment;
    for (fragment = GDBFragment::allFragments; fragment;
         fragment = fragment->next) {
      if (strcmp(fragment->name(), name) == 0) {
        fragment->run(cx, argv);
        break;
      }
>>>>>>> upstream-releases
    }
<<<<<<< HEAD
  }
||||||| merged common ancestors
=======
    if (!fragment) {
      fprintf(stderr, "Unrecognized fragment name: %s\n", name);
      exit(1);
    }
  }
>>>>>>> upstream-releases

  return 0;
}
