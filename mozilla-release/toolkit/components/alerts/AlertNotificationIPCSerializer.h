/* This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/. */

#ifndef mozilla_AlertNotificationIPCSerializer_h__
#define mozilla_AlertNotificationIPCSerializer_h__

#include "nsComponentManagerUtils.h"
#include "nsCOMPtr.h"
#include "nsIAlertsService.h"
#include "nsIPrincipal.h"
#include "nsString.h"

#include "ipc/IPCMessageUtils.h"

#include "mozilla/dom/PermissionMessageUtils.h"

namespace mozilla {
namespace ipc {

template <>
<<<<<<< HEAD
struct ParamTraits<nsIAlertNotification> {
  static void Write(Message* aMsg, nsIAlertNotification* aParam) {
||||||| merged common ancestors
struct ParamTraits<nsIAlertNotification>
{
  static void Write(Message* aMsg, nsIAlertNotification* aParam)
  {
=======
struct IPDLParamTraits<nsIAlertNotification*> {
  static void Write(IPC::Message* aMsg, IProtocol* aActor,
                    nsIAlertNotification* aParam) {
>>>>>>> upstream-releases
    bool isNull = !aParam;
    if (isNull) {
      WriteIPDLParam(aMsg, aActor, isNull);
      return;
    }

    nsString name, imageURL, title, text, cookie, dir, lang, data;
    bool textClickable, inPrivateBrowsing, requireInteraction;
    nsCOMPtr<nsIPrincipal> principal;

    if (NS_WARN_IF(NS_FAILED(aParam->GetName(name))) ||
        NS_WARN_IF(NS_FAILED(aParam->GetImageURL(imageURL))) ||
        NS_WARN_IF(NS_FAILED(aParam->GetTitle(title))) ||
        NS_WARN_IF(NS_FAILED(aParam->GetText(text))) ||
        NS_WARN_IF(NS_FAILED(aParam->GetTextClickable(&textClickable))) ||
        NS_WARN_IF(NS_FAILED(aParam->GetCookie(cookie))) ||
        NS_WARN_IF(NS_FAILED(aParam->GetDir(dir))) ||
        NS_WARN_IF(NS_FAILED(aParam->GetLang(lang))) ||
        NS_WARN_IF(NS_FAILED(aParam->GetData(data))) ||
        NS_WARN_IF(
            NS_FAILED(aParam->GetPrincipal(getter_AddRefs(principal)))) ||
        NS_WARN_IF(
            NS_FAILED(aParam->GetInPrivateBrowsing(&inPrivateBrowsing))) ||
        NS_WARN_IF(
            NS_FAILED(aParam->GetRequireInteraction(&requireInteraction)))) {
      // Write a `null` object if any getter returns an error. Otherwise, the
      // receiver will try to deserialize an incomplete object and crash.
      WriteIPDLParam(aMsg, aActor, /* isNull */ true);
      return;
    }

    WriteIPDLParam(aMsg, aActor, isNull);
    WriteIPDLParam(aMsg, aActor, name);
    WriteIPDLParam(aMsg, aActor, imageURL);
    WriteIPDLParam(aMsg, aActor, title);
    WriteIPDLParam(aMsg, aActor, text);
    WriteIPDLParam(aMsg, aActor, textClickable);
    WriteIPDLParam(aMsg, aActor, cookie);
    WriteIPDLParam(aMsg, aActor, dir);
    WriteIPDLParam(aMsg, aActor, lang);
    WriteIPDLParam(aMsg, aActor, data);
    WriteIPDLParam(aMsg, aActor, IPC::Principal(principal));
    WriteIPDLParam(aMsg, aActor, inPrivateBrowsing);
    WriteIPDLParam(aMsg, aActor, requireInteraction);
  }

<<<<<<< HEAD
  static bool Read(const Message* aMsg, PickleIterator* aIter,
                   RefPtr<nsIAlertNotification>* aResult) {
||||||| merged common ancestors
  static bool Read(const Message* aMsg, PickleIterator* aIter, RefPtr<nsIAlertNotification>* aResult)
  {
=======
  static bool Read(const IPC::Message* aMsg, PickleIterator* aIter,
                   IProtocol* aActor, RefPtr<nsIAlertNotification>* aResult) {
>>>>>>> upstream-releases
    bool isNull;
    NS_ENSURE_TRUE(ReadIPDLParam(aMsg, aIter, aActor, &isNull), false);
    if (isNull) {
      *aResult = nullptr;
      return true;
    }

    nsString name, imageURL, title, text, cookie, dir, lang, data;
    bool textClickable, inPrivateBrowsing, requireInteraction;
    IPC::Principal principal;

<<<<<<< HEAD
    if (!ReadParam(aMsg, aIter, &name) || !ReadParam(aMsg, aIter, &imageURL) ||
        !ReadParam(aMsg, aIter, &title) || !ReadParam(aMsg, aIter, &text) ||
        !ReadParam(aMsg, aIter, &textClickable) ||
        !ReadParam(aMsg, aIter, &cookie) || !ReadParam(aMsg, aIter, &dir) ||
        !ReadParam(aMsg, aIter, &lang) || !ReadParam(aMsg, aIter, &data) ||
        !ReadParam(aMsg, aIter, &principal) ||
        !ReadParam(aMsg, aIter, &inPrivateBrowsing) ||
        !ReadParam(aMsg, aIter, &requireInteraction)) {
||||||| merged common ancestors
    if (!ReadParam(aMsg, aIter, &name) ||
        !ReadParam(aMsg, aIter, &imageURL) ||
        !ReadParam(aMsg, aIter, &title) ||
        !ReadParam(aMsg, aIter, &text) ||
        !ReadParam(aMsg, aIter, &textClickable) ||
        !ReadParam(aMsg, aIter, &cookie) ||
        !ReadParam(aMsg, aIter, &dir) ||
        !ReadParam(aMsg, aIter, &lang) ||
        !ReadParam(aMsg, aIter, &data) ||
        !ReadParam(aMsg, aIter, &principal) ||
        !ReadParam(aMsg, aIter, &inPrivateBrowsing) ||
        !ReadParam(aMsg, aIter, &requireInteraction)) {

=======
    if (!ReadIPDLParam(aMsg, aIter, aActor, &name) ||
        !ReadIPDLParam(aMsg, aIter, aActor, &imageURL) ||
        !ReadIPDLParam(aMsg, aIter, aActor, &title) ||
        !ReadIPDLParam(aMsg, aIter, aActor, &text) ||
        !ReadIPDLParam(aMsg, aIter, aActor, &textClickable) ||
        !ReadIPDLParam(aMsg, aIter, aActor, &cookie) ||
        !ReadIPDLParam(aMsg, aIter, aActor, &dir) ||
        !ReadIPDLParam(aMsg, aIter, aActor, &lang) ||
        !ReadIPDLParam(aMsg, aIter, aActor, &data) ||
        !ReadIPDLParam(aMsg, aIter, aActor, &principal) ||
        !ReadIPDLParam(aMsg, aIter, aActor, &inPrivateBrowsing) ||
        !ReadIPDLParam(aMsg, aIter, aActor, &requireInteraction)) {
>>>>>>> upstream-releases
      return false;
    }

    nsCOMPtr<nsIAlertNotification> alert =
        do_CreateInstance(ALERT_NOTIFICATION_CONTRACTID);
    if (NS_WARN_IF(!alert)) {
      *aResult = nullptr;
      return true;
    }
    nsresult rv = alert->Init(name, imageURL, title, text, textClickable,
                              cookie, dir, lang, data, principal,
                              inPrivateBrowsing, requireInteraction);
    if (NS_WARN_IF(NS_FAILED(rv))) {
      *aResult = nullptr;
      return true;
    }
    *aResult = alert.forget();
    return true;
  }
};

<<<<<<< HEAD
}  // namespace IPC
||||||| merged common ancestors
} // namespace IPC
=======
}  // namespace ipc
}  // namespace mozilla
>>>>>>> upstream-releases

#endif /* mozilla_AlertNotificationIPCSerializer_h__ */
