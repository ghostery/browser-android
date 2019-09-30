/* -*- Mode: C++; tab-width: 8; indent-tabs-mode: nil; c-basic-offset: 2 -*- */
/* vim: set ts=8 sts=2 et sw=2 tw=80: */
/* This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/. */

#ifndef gfxCrashReporterUtils_h__
#define gfxCrashReporterUtils_h__

#include "nsString.h"

namespace mozilla {

/** \class ScopedGfxFeatureReporter
<<<<<<< HEAD
 *
 * On creation, adds "FeatureName?" to AppNotes
 * On destruction, adds "FeatureName-", or "FeatureName+" if you called
 * SetSuccessful().
 *
 * Any such string is added at most once to AppNotes, and is subsequently
 * skipped.
 *
 * This ScopedGfxFeatureReporter class is designed to be fool-proof to use in
 * functions that have many exit points. We don't want to encourage having
 * function with many exit points. It just happens that our graphics features
 * initialization functions are like that.
 */
class ScopedGfxFeatureReporter {
 public:
  explicit ScopedGfxFeatureReporter(const char *aFeature, bool aForce = false)
      : mFeature(aFeature), mStatusChar('-'), mStatusNumber(0) {
||||||| merged common ancestors
  *
  * On creation, adds "FeatureName?" to AppNotes
  * On destruction, adds "FeatureName-", or "FeatureName+" if you called SetSuccessful().
  *
  * Any such string is added at most once to AppNotes, and is subsequently skipped.
  *
  * This ScopedGfxFeatureReporter class is designed to be fool-proof to use in functions that
  * have many exit points. We don't want to encourage having function with many exit points.
  * It just happens that our graphics features initialization functions are like that.
  */
class ScopedGfxFeatureReporter
{
public:
  explicit ScopedGfxFeatureReporter(const char *aFeature, bool aForce = false)
    : mFeature(aFeature), mStatusChar('-'), mStatusNumber(0)
  {
=======
 *
 * On creation, adds "FeatureName?" to AppNotes
 * On destruction, adds "FeatureName-", or "FeatureName+" if you called
 * SetSuccessful().
 *
 * Any such string is added at most once to AppNotes, and is subsequently
 * skipped.
 *
 * This ScopedGfxFeatureReporter class is designed to be fool-proof to use in
 * functions that have many exit points. We don't want to encourage having
 * function with many exit points. It just happens that our graphics features
 * initialization functions are like that.
 */
class ScopedGfxFeatureReporter {
 public:
  explicit ScopedGfxFeatureReporter(const char* aFeature, bool aForce = false)
      : mFeature(aFeature), mStatusChar('-'), mStatusNumber(0) {
>>>>>>> upstream-releases
    WriteAppNote(aForce ? '!' : '?', 0);
  }
  ~ScopedGfxFeatureReporter() { WriteAppNote(mStatusChar, mStatusNumber); }
  void SetSuccessful() { mStatusChar = '+'; }
  void SetSuccessful(int32_t aNumber) {
    mStatusChar = '+';
    mStatusNumber = aNumber;
  }

  static void AppNote(const nsACString &aMessage);

  class AppNoteWritingRunnable;

<<<<<<< HEAD
 protected:
  const char *mFeature;
||||||| merged common ancestors
protected:
  const char *mFeature;
=======
 protected:
  const char* mFeature;
>>>>>>> upstream-releases
  char mStatusChar;
  int32_t mStatusNumber;

 private:
  void WriteAppNote(char statusChar, int32_t statusNumber);
};

}  // end namespace mozilla

#endif  // gfxCrashReporterUtils_h__
