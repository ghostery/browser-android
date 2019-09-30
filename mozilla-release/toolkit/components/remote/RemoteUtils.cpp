/* -*- Mode: C++; tab-width: 8; indent-tabs-mode: nil; c-basic-offset: 2 -*- */
/* vim:expandtab:shiftwidth=2:tabstop=8:
 */
/* vim:set ts=8 sw=2 et cindent: */
/* This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/. */

#include <stdlib.h>
#include <string.h>
#include <strings.h>
#include <unistd.h>
#include <limits.h>

#include "RemoteUtils.h"

#ifdef IS_BIG_ENDIAN
<<<<<<< HEAD:mozilla-release/widget/xremoteclient/RemoteUtils.cpp
#define TO_LITTLE_ENDIAN32(x)                           \
  ((((x)&0xff000000) >> 24) | (((x)&0x00ff0000) >> 8) | \
   (((x)&0x0000ff00) << 8) | (((x)&0x000000ff) << 24))
||||||| merged common ancestors
#define TO_LITTLE_ENDIAN32(x) \
    ((((x) & 0xff000000) >> 24) | (((x) & 0x00ff0000) >> 8) | \
    (((x) & 0x0000ff00) << 8) | (((x) & 0x000000ff) << 24))
=======
#  define TO_LITTLE_ENDIAN32(x)                           \
    ((((x)&0xff000000) >> 24) | (((x)&0x00ff0000) >> 8) | \
     (((x)&0x0000ff00) << 8) | (((x)&0x000000ff) << 24))
>>>>>>> upstream-releases:mozilla-release/toolkit/components/remote/RemoteUtils.cpp
#else
#  define TO_LITTLE_ENDIAN32(x) (x)
#endif

#ifndef MAX_PATH
#  ifdef PATH_MAX
#    define MAX_PATH PATH_MAX
#  else
#    define MAX_PATH 1024
#  endif
#endif

/* like strcpy, but return the char after the final null */
<<<<<<< HEAD:mozilla-release/widget/xremoteclient/RemoteUtils.cpp
static char *estrcpy(const char *s, char *d) {
  while (*s) *d++ = *s++;
||||||| merged common ancestors
static char*
estrcpy(const char* s, char* d)
{
  while (*s)
    *d++ = *s++;
=======
static char* estrcpy(const char* s, char* d) {
  while (*s) *d++ = *s++;
>>>>>>> upstream-releases:mozilla-release/toolkit/components/remote/RemoteUtils.cpp

  *d++ = '\0';
  return d;
}

/* Construct a command line from given args and desktop startup ID.
 * Returned buffer must be released by free().
 */
<<<<<<< HEAD:mozilla-release/widget/xremoteclient/RemoteUtils.cpp
char *ConstructCommandLine(int32_t argc, char **argv,
                           const char *aDesktopStartupID,
                           int *aCommandLineLength) {
||||||| merged common ancestors
char*
ConstructCommandLine(int32_t argc, char **argv,
                     const char* aDesktopStartupID,
                     int *aCommandLineLength)
{
=======
char* ConstructCommandLine(int32_t argc, char** argv,
                           const char* aDesktopStartupID,
                           int* aCommandLineLength) {
>>>>>>> upstream-releases:mozilla-release/toolkit/components/remote/RemoteUtils.cpp
  char cwdbuf[MAX_PATH];
  if (!getcwd(cwdbuf, MAX_PATH)) return nullptr;

  // the commandline property is constructed as an array of int32_t
  // followed by a series of null-terminated strings:
  //
  // [argc][offsetargv0][offsetargv1...]<workingdir>\0<argv[0]>\0argv[1]...\0
  // (offset is from the beginning of the buffer)

  static char desktopStartupPrefix[] = " DESKTOP_STARTUP_ID=";

  int32_t argvlen = strlen(cwdbuf);
  for (int i = 0; i < argc; ++i) {
    int32_t len = strlen(argv[i]);
    if (i == 0 && aDesktopStartupID) {
      len += sizeof(desktopStartupPrefix) - 1 + strlen(aDesktopStartupID);
    }
    argvlen += len;
  }

<<<<<<< HEAD:mozilla-release/widget/xremoteclient/RemoteUtils.cpp
  auto *buffer =
      (int32_t *)malloc(argvlen + argc + 1 + sizeof(int32_t) * (argc + 1));
  if (!buffer) return nullptr;
||||||| merged common ancestors
  auto* buffer = (int32_t*) malloc(argvlen + argc + 1 +
                                   sizeof(int32_t) * (argc + 1));
  if (!buffer)
    return nullptr;
=======
  auto* buffer =
      (int32_t*)malloc(argvlen + argc + 1 + sizeof(int32_t) * (argc + 1));
  if (!buffer) return nullptr;
>>>>>>> upstream-releases:mozilla-release/toolkit/components/remote/RemoteUtils.cpp

  buffer[0] = TO_LITTLE_ENDIAN32(argc);

<<<<<<< HEAD:mozilla-release/widget/xremoteclient/RemoteUtils.cpp
  auto *bufend = (char *)(buffer + argc + 1);
||||||| merged common ancestors
  auto *bufend = (char*) (buffer + argc + 1);
=======
  auto* bufend = (char*)(buffer + argc + 1);
>>>>>>> upstream-releases:mozilla-release/toolkit/components/remote/RemoteUtils.cpp

  bufend = estrcpy(cwdbuf, bufend);

  for (int i = 0; i < argc; ++i) {
<<<<<<< HEAD:mozilla-release/widget/xremoteclient/RemoteUtils.cpp
    buffer[i + 1] = TO_LITTLE_ENDIAN32(bufend - ((char *)buffer));
||||||| merged common ancestors
    buffer[i + 1] = TO_LITTLE_ENDIAN32(bufend - ((char*) buffer));
=======
    buffer[i + 1] = TO_LITTLE_ENDIAN32(bufend - ((char*)buffer));
>>>>>>> upstream-releases:mozilla-release/toolkit/components/remote/RemoteUtils.cpp
    bufend = estrcpy(argv[i], bufend);
    if (i == 0 && aDesktopStartupID) {
      bufend = estrcpy(desktopStartupPrefix, bufend - 1);
      bufend = estrcpy(aDesktopStartupID, bufend - 1);
    }
  }

#ifdef DEBUG_command_line
<<<<<<< HEAD:mozilla-release/widget/xremoteclient/RemoteUtils.cpp
  int32_t debug_argc = TO_LITTLE_ENDIAN32(*buffer);
  char *debug_workingdir = (char *)(buffer + argc + 1);
||||||| merged common ancestors
  int32_t   debug_argc   = TO_LITTLE_ENDIAN32(*buffer);
  char *debug_workingdir = (char*) (buffer + argc + 1);
=======
  int32_t debug_argc = TO_LITTLE_ENDIAN32(*buffer);
  char* debug_workingdir = (char*)(buffer + argc + 1);
>>>>>>> upstream-releases:mozilla-release/toolkit/components/remote/RemoteUtils.cpp

  printf(
      "Sending command line:\n"
      "  working dir: %s\n"
      "  argc:\t%i",
      debug_workingdir, debug_argc);

<<<<<<< HEAD:mozilla-release/widget/xremoteclient/RemoteUtils.cpp
  int32_t *debug_offset = buffer + 1;
||||||| merged common ancestors
  int32_t  *debug_offset = buffer + 1;
=======
  int32_t* debug_offset = buffer + 1;
>>>>>>> upstream-releases:mozilla-release/toolkit/components/remote/RemoteUtils.cpp
  for (int debug_i = 0; debug_i < debug_argc; ++debug_i)
    printf("  argv[%i]:\t%s\n", debug_i,
<<<<<<< HEAD:mozilla-release/widget/xremoteclient/RemoteUtils.cpp
           ((char *)buffer) + TO_LITTLE_ENDIAN32(debug_offset[debug_i]));
||||||| merged common ancestors
           ((char*) buffer) + TO_LITTLE_ENDIAN32(debug_offset[debug_i]));
=======
           ((char*)buffer) + TO_LITTLE_ENDIAN32(debug_offset[debug_i]));
>>>>>>> upstream-releases:mozilla-release/toolkit/components/remote/RemoteUtils.cpp
#endif

  *aCommandLineLength = bufend - reinterpret_cast<char*>(buffer);
  return reinterpret_cast<char*>(buffer);
}
