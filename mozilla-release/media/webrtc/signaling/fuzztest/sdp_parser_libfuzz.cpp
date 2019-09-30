/* -*- Mode: C++; tab-width: 8; indent-tabs-mode: nil; c-basic-offset: 2 -*- */
/* vim: set ts=2 et sw=2 tw=80: */
/* This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at http://mozilla.org/MPL/2.0/. */

#include <string>

#include "gtest/gtest.h"

#include "FuzzingInterface.h"

#include "signaling/src/sdp/SipccSdpParser.h"

using namespace mozilla;

static mozilla::UniquePtr<Sdp> sdpPtr;
static SipccSdpParser mParser;

<<<<<<< HEAD
int FuzzingInitSdpParser(int *argc, char ***argv) { return 0; }
||||||| merged common ancestors
int FuzzingInitSdpParser(int *argc, char ***argv) {
  return 0;
}
=======
int FuzzingInitSdpParser(int* argc, char*** argv) { return 0; }
>>>>>>> upstream-releases

<<<<<<< HEAD
static int RunSdpParserFuzzing(const uint8_t *data, size_t size) {
  std::string message(reinterpret_cast<const char *>(data), size);
||||||| merged common ancestors
static int
RunSdpParserFuzzing(const uint8_t* data, size_t size) {
  std::string message(reinterpret_cast<const char*>(data), size);
=======
static int RunSdpParserFuzzing(const uint8_t* data, size_t size) {
  std::string message(reinterpret_cast<const char*>(data), size);
>>>>>>> upstream-releases

  sdpPtr = mParser.Parse(message);

  return 0;
}

MOZ_FUZZING_INTERFACE_RAW(FuzzingInitSdpParser, RunSdpParserFuzzing, SdpParser);
