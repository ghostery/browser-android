/* -*- Mode: C++; tab-width: 8; indent-tabs-mode: nil; c-basic-offset: 2 -*- */
/* vim: set ts=8 sts=2 et sw=2 tw=80: */
/* This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/. */

#ifndef mozilla_dom_FeaturePolicyUtils_h
#define mozilla_dom_FeaturePolicyUtils_h

#include "nsString.h"
#include <functional>

namespace mozilla {
namespace dom {

<<<<<<< HEAD
class FeaturePolicyUtils final {
 public:
  enum FeaturePolicyValue {
||||||| merged common ancestors
class FeaturePolicyUtils final
{
public:
  enum FeaturePolicyValue
  {
=======
class Document;

class FeaturePolicyUtils final {
 public:
  enum FeaturePolicyValue {
>>>>>>> upstream-releases
    // Feature always allowed.
    eAll,

    // Feature allowed for documents that are same-origin with this one.
    eSelf,

    // Feature denied.
    eNone,
  };

  // This method returns true if aFeatureName is allowed for aDocument.
  // Use this method everywhere you need to check feature-policy directives.
<<<<<<< HEAD
  static bool IsFeatureAllowed(nsIDocument* aDocument,
                               const nsAString& aFeatureName);
||||||| merged common ancestors
  static bool
  IsFeatureAllowed(nsIDocument* aDocument,
                   const nsAString& aFeatureName);
=======
  static bool IsFeatureAllowed(Document* aDocument,
                               const nsAString& aFeatureName);
>>>>>>> upstream-releases

  // Returns true if aFeatureName is a known feature policy name.
  static bool IsSupportedFeature(const nsAString& aFeatureName);

  // Runs aCallback for each known feature policy, with the feature name as
  // argument.
  static void ForEachFeature(const std::function<void(const char*)>& aCallback);

  // Returns the default policy value for aFeatureName.
<<<<<<< HEAD
  static FeaturePolicyValue DefaultAllowListFeature(
      const nsAString& aFeatureName);

 private:
  static void ReportViolation(nsIDocument* aDocument,
                              const nsAString& aFeatureName);
||||||| merged common ancestors
  static FeaturePolicyValue
  DefaultAllowListFeature(const nsAString& aFeatureName);
=======
  static FeaturePolicyValue DefaultAllowListFeature(
      const nsAString& aFeatureName);

 private:
  static void ReportViolation(Document* aDocument,
                              const nsAString& aFeatureName);
>>>>>>> upstream-releases
};

}  // namespace dom
}  // namespace mozilla

#endif  // mozilla_dom_FeaturePolicyUtils_h
