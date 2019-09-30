/* -*- Mode: C++; tab-width: 8; indent-tabs-mode: nil; c-basic-offset: 2 -*- */
/* vim: set ts=8 sts=2 et sw=2 tw=80: */
/* This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/. */

#ifndef mozilla_dom_ConsoleInstance_h
#define mozilla_dom_ConsoleInstance_h

#include "mozilla/dom/Console.h"

namespace mozilla {
namespace dom {

class ConsoleInstance final : public nsISupports, public nsWrapperCache {
 public:
  NS_DECL_CYCLE_COLLECTING_ISUPPORTS
  NS_DECL_CYCLE_COLLECTION_SCRIPT_HOLDER_CLASS(ConsoleInstance)

  explicit ConsoleInstance(JSContext* aCx,
                           const ConsoleInstanceOptions& aOptions);

  // WebIDL methods
  JSObject* WrapObject(JSContext* aCx,
                       JS::Handle<JSObject*> aGivenProto) override;

  nsPIDOMWindowInner* GetParentObject() const { return nullptr; }

<<<<<<< HEAD
  void Log(JSContext* aCx, const Sequence<JS::Value>& aData);
||||||| merged common ancestors
  void
  Log(JSContext* aCx, const Sequence<JS::Value>& aData);
=======
  MOZ_CAN_RUN_SCRIPT
  void Log(JSContext* aCx, const Sequence<JS::Value>& aData);
>>>>>>> upstream-releases

<<<<<<< HEAD
  void Info(JSContext* aCx, const Sequence<JS::Value>& aData);
||||||| merged common ancestors
  void
  Info(JSContext* aCx, const Sequence<JS::Value>& aData);
=======
  MOZ_CAN_RUN_SCRIPT
  void Info(JSContext* aCx, const Sequence<JS::Value>& aData);
>>>>>>> upstream-releases

<<<<<<< HEAD
  void Warn(JSContext* aCx, const Sequence<JS::Value>& aData);
||||||| merged common ancestors
  void
  Warn(JSContext* aCx, const Sequence<JS::Value>& aData);
=======
  MOZ_CAN_RUN_SCRIPT
  void Warn(JSContext* aCx, const Sequence<JS::Value>& aData);
>>>>>>> upstream-releases

<<<<<<< HEAD
  void Error(JSContext* aCx, const Sequence<JS::Value>& aData);
||||||| merged common ancestors
  void
  Error(JSContext* aCx, const Sequence<JS::Value>& aData);
=======
  MOZ_CAN_RUN_SCRIPT
  void Error(JSContext* aCx, const Sequence<JS::Value>& aData);
>>>>>>> upstream-releases

<<<<<<< HEAD
  void Exception(JSContext* aCx, const Sequence<JS::Value>& aData);
||||||| merged common ancestors
  void
  Exception(JSContext* aCx, const Sequence<JS::Value>& aData);
=======
  MOZ_CAN_RUN_SCRIPT
  void Exception(JSContext* aCx, const Sequence<JS::Value>& aData);
>>>>>>> upstream-releases

<<<<<<< HEAD
  void Debug(JSContext* aCx, const Sequence<JS::Value>& aData);
||||||| merged common ancestors
  void
  Debug(JSContext* aCx, const Sequence<JS::Value>& aData);
=======
  MOZ_CAN_RUN_SCRIPT
  void Debug(JSContext* aCx, const Sequence<JS::Value>& aData);
>>>>>>> upstream-releases

<<<<<<< HEAD
  void Table(JSContext* aCx, const Sequence<JS::Value>& aData);
||||||| merged common ancestors
  void
  Table(JSContext* aCx, const Sequence<JS::Value>& aData);
=======
  MOZ_CAN_RUN_SCRIPT
  void Table(JSContext* aCx, const Sequence<JS::Value>& aData);
>>>>>>> upstream-releases

<<<<<<< HEAD
  void Trace(JSContext* aCx, const Sequence<JS::Value>& aData);
||||||| merged common ancestors
  void
  Trace(JSContext* aCx, const Sequence<JS::Value>& aData);
=======
  MOZ_CAN_RUN_SCRIPT
  void Trace(JSContext* aCx, const Sequence<JS::Value>& aData);
>>>>>>> upstream-releases

<<<<<<< HEAD
  void Dir(JSContext* aCx, const Sequence<JS::Value>& aData);
||||||| merged common ancestors
  void
  Dir(JSContext* aCx, const Sequence<JS::Value>& aData);
=======
  MOZ_CAN_RUN_SCRIPT
  void Dir(JSContext* aCx, const Sequence<JS::Value>& aData);
>>>>>>> upstream-releases

<<<<<<< HEAD
  void Dirxml(JSContext* aCx, const Sequence<JS::Value>& aData);
||||||| merged common ancestors
  void
  Dirxml(JSContext* aCx, const Sequence<JS::Value>& aData);
=======
  MOZ_CAN_RUN_SCRIPT
  void Dirxml(JSContext* aCx, const Sequence<JS::Value>& aData);
>>>>>>> upstream-releases

<<<<<<< HEAD
  void Group(JSContext* aCx, const Sequence<JS::Value>& aData);
||||||| merged common ancestors
  void
  Group(JSContext* aCx, const Sequence<JS::Value>& aData);
=======
  MOZ_CAN_RUN_SCRIPT
  void Group(JSContext* aCx, const Sequence<JS::Value>& aData);
>>>>>>> upstream-releases

<<<<<<< HEAD
  void GroupCollapsed(JSContext* aCx, const Sequence<JS::Value>& aData);
||||||| merged common ancestors
  void
  GroupCollapsed(JSContext* aCx, const Sequence<JS::Value>& aData);
=======
  MOZ_CAN_RUN_SCRIPT
  void GroupCollapsed(JSContext* aCx, const Sequence<JS::Value>& aData);
>>>>>>> upstream-releases

<<<<<<< HEAD
  void GroupEnd(JSContext* aCx);
||||||| merged common ancestors
  void
  GroupEnd(JSContext* aCx);
=======
  MOZ_CAN_RUN_SCRIPT
  void GroupEnd(JSContext* aCx);
>>>>>>> upstream-releases

<<<<<<< HEAD
  void Time(JSContext* aCx, const nsAString& aLabel);
||||||| merged common ancestors
  void
  Time(JSContext* aCx, const nsAString& aLabel);
=======
  MOZ_CAN_RUN_SCRIPT
  void Time(JSContext* aCx, const nsAString& aLabel);
>>>>>>> upstream-releases

<<<<<<< HEAD
  void TimeLog(JSContext* aCx, const nsAString& aLabel,
               const Sequence<JS::Value>& aData);
||||||| merged common ancestors
  void
  TimeLog(JSContext* aCx, const nsAString& aLabel,
          const Sequence<JS::Value>& aData);
=======
  MOZ_CAN_RUN_SCRIPT
  void TimeLog(JSContext* aCx, const nsAString& aLabel,
               const Sequence<JS::Value>& aData);
>>>>>>> upstream-releases

<<<<<<< HEAD
  void TimeEnd(JSContext* aCx, const nsAString& aLabel);
||||||| merged common ancestors
  void
  TimeEnd(JSContext* aCx, const nsAString& aLabel);
=======
  MOZ_CAN_RUN_SCRIPT
  void TimeEnd(JSContext* aCx, const nsAString& aLabel);
>>>>>>> upstream-releases

<<<<<<< HEAD
  void TimeStamp(JSContext* aCx, const JS::Handle<JS::Value> aData);
||||||| merged common ancestors
  void
  TimeStamp(JSContext* aCx, const JS::Handle<JS::Value> aData);
=======
  MOZ_CAN_RUN_SCRIPT
  void TimeStamp(JSContext* aCx, const JS::Handle<JS::Value> aData);
>>>>>>> upstream-releases

<<<<<<< HEAD
  void Profile(JSContext* aCx, const Sequence<JS::Value>& aData);
||||||| merged common ancestors
  void
  Profile(JSContext* aCx, const Sequence<JS::Value>& aData);
=======
  MOZ_CAN_RUN_SCRIPT
  void Profile(JSContext* aCx, const Sequence<JS::Value>& aData);
>>>>>>> upstream-releases

<<<<<<< HEAD
  void ProfileEnd(JSContext* aCx, const Sequence<JS::Value>& aData);
||||||| merged common ancestors
  void
  ProfileEnd(JSContext* aCx, const Sequence<JS::Value>& aData);
=======
  MOZ_CAN_RUN_SCRIPT
  void ProfileEnd(JSContext* aCx, const Sequence<JS::Value>& aData);
>>>>>>> upstream-releases

<<<<<<< HEAD
  void Assert(JSContext* aCx, bool aCondition,
              const Sequence<JS::Value>& aData);
||||||| merged common ancestors
  void
  Assert(JSContext* aCx, bool aCondition, const Sequence<JS::Value>& aData);
=======
  MOZ_CAN_RUN_SCRIPT
  void Assert(JSContext* aCx, bool aCondition,
              const Sequence<JS::Value>& aData);
>>>>>>> upstream-releases

<<<<<<< HEAD
  void Count(JSContext* aCx, const nsAString& aLabel);
||||||| merged common ancestors
  void
  Count(JSContext* aCx, const nsAString& aLabel);
=======
  MOZ_CAN_RUN_SCRIPT
  void Count(JSContext* aCx, const nsAString& aLabel);
>>>>>>> upstream-releases

<<<<<<< HEAD
  void CountReset(JSContext* aCx, const nsAString& aLabel);
||||||| merged common ancestors
  void
  CountReset(JSContext* aCx, const nsAString& aLabel);
=======
  MOZ_CAN_RUN_SCRIPT
  void CountReset(JSContext* aCx, const nsAString& aLabel);
>>>>>>> upstream-releases

<<<<<<< HEAD
  void Clear(JSContext* aCx);
||||||| merged common ancestors
  void
  Clear(JSContext* aCx);
=======
  MOZ_CAN_RUN_SCRIPT
  void Clear(JSContext* aCx);
>>>>>>> upstream-releases

  // For testing only.
  void ReportForServiceWorkerScope(const nsAString& aScope,
                                   const nsAString& aMessage,
                                   const nsAString& aFilename,
                                   uint32_t aLineNumber, uint32_t aColumnNumber,
                                   ConsoleLevel aLevel);

 private:
  ~ConsoleInstance();

  RefPtr<Console> mConsole;
};

}  // namespace dom
}  // namespace mozilla

#endif  // mozilla_dom_ConsoleInstance_h
