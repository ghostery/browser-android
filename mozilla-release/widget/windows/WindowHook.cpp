/* -*- Mode: C++; tab-width: 2; indent-tabs-mode: nil; c-basic-offset: 2 -*-
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/. */

#include "WindowHook.h"
#include "nsWindow.h"
#include "nsWindowDefs.h"

namespace mozilla {
namespace widget {

<<<<<<< HEAD
nsresult WindowHook::AddHook(UINT nMsg, Callback callback, void *context) {
  MessageData *data = LookupOrCreate(nMsg);
||||||| merged common ancestors
nsresult
WindowHook::AddHook(UINT nMsg, Callback callback, void *context) {
  MessageData *data = LookupOrCreate(nMsg);
=======
nsresult WindowHook::AddHook(UINT nMsg, Callback callback, void* context) {
  MessageData* data = LookupOrCreate(nMsg);
>>>>>>> upstream-releases

  if (!data) return NS_ERROR_OUT_OF_MEMORY;

  // Ensure we don't overwrite another hook
  NS_ENSURE_TRUE(nullptr == data->hook.cb, NS_ERROR_UNEXPECTED);

  data->hook = CallbackData(callback, context);

  return NS_OK;
}

<<<<<<< HEAD
nsresult WindowHook::RemoveHook(UINT nMsg, Callback callback, void *context) {
||||||| merged common ancestors
nsresult
WindowHook::RemoveHook(UINT nMsg, Callback callback, void *context) {
=======
nsresult WindowHook::RemoveHook(UINT nMsg, Callback callback, void* context) {
>>>>>>> upstream-releases
  CallbackData cbdata(callback, context);
<<<<<<< HEAD
  MessageData *data = Lookup(nMsg);
  if (!data) return NS_ERROR_UNEXPECTED;
  if (data->hook != cbdata) return NS_ERROR_UNEXPECTED;
||||||| merged common ancestors
  MessageData *data = Lookup(nMsg);
  if (!data)
    return NS_ERROR_UNEXPECTED;
  if (data->hook != cbdata)
    return NS_ERROR_UNEXPECTED;
=======
  MessageData* data = Lookup(nMsg);
  if (!data) return NS_ERROR_UNEXPECTED;
  if (data->hook != cbdata) return NS_ERROR_UNEXPECTED;
>>>>>>> upstream-releases
  data->hook = CallbackData();

  DeleteIfEmpty(data);
  return NS_OK;
}

<<<<<<< HEAD
nsresult WindowHook::AddMonitor(UINT nMsg, Callback callback, void *context) {
  MessageData *data = LookupOrCreate(nMsg);
||||||| merged common ancestors
nsresult
WindowHook::AddMonitor(UINT nMsg, Callback callback, void *context) {
  MessageData *data = LookupOrCreate(nMsg);
=======
nsresult WindowHook::AddMonitor(UINT nMsg, Callback callback, void* context) {
  MessageData* data = LookupOrCreate(nMsg);
>>>>>>> upstream-releases
  return (data && data->monitors.AppendElement(CallbackData(callback, context)))
             ? NS_OK
             : NS_ERROR_OUT_OF_MEMORY;
}

<<<<<<< HEAD
nsresult WindowHook::RemoveMonitor(UINT nMsg, Callback callback,
                                   void *context) {
||||||| merged common ancestors
nsresult
WindowHook::RemoveMonitor(UINT nMsg, Callback callback, void *context) {
=======
nsresult WindowHook::RemoveMonitor(UINT nMsg, Callback callback,
                                   void* context) {
>>>>>>> upstream-releases
  CallbackData cbdata(callback, context);
<<<<<<< HEAD
  MessageData *data = Lookup(nMsg);
  if (!data) return NS_ERROR_UNEXPECTED;
||||||| merged common ancestors
  MessageData *data = Lookup(nMsg);
  if (!data)
    return NS_ERROR_UNEXPECTED;
=======
  MessageData* data = Lookup(nMsg);
  if (!data) return NS_ERROR_UNEXPECTED;
>>>>>>> upstream-releases
  CallbackDataArray::index_type idx = data->monitors.IndexOf(cbdata);
  if (idx == CallbackDataArray::NoIndex) return NS_ERROR_UNEXPECTED;
  data->monitors.RemoveElementAt(idx);
  DeleteIfEmpty(data);
  return NS_OK;
}

<<<<<<< HEAD
WindowHook::MessageData *WindowHook::Lookup(UINT nMsg) {
||||||| merged common ancestors
WindowHook::MessageData *
WindowHook::Lookup(UINT nMsg) {
=======
WindowHook::MessageData* WindowHook::Lookup(UINT nMsg) {
>>>>>>> upstream-releases
  MessageDataArray::index_type idx;
  for (idx = 0; idx < mMessageData.Length(); idx++) {
<<<<<<< HEAD
    MessageData &data = mMessageData[idx];
    if (data.nMsg == nMsg) return &data;
||||||| merged common ancestors
    MessageData &data = mMessageData[idx];
    if (data.nMsg == nMsg)
      return &data;
=======
    MessageData& data = mMessageData[idx];
    if (data.nMsg == nMsg) return &data;
>>>>>>> upstream-releases
  }
  return nullptr;
}

<<<<<<< HEAD
WindowHook::MessageData *WindowHook::LookupOrCreate(UINT nMsg) {
  MessageData *data = Lookup(nMsg);
||||||| merged common ancestors
WindowHook::MessageData *
WindowHook::LookupOrCreate(UINT nMsg) {
  MessageData *data = Lookup(nMsg);
=======
WindowHook::MessageData* WindowHook::LookupOrCreate(UINT nMsg) {
  MessageData* data = Lookup(nMsg);
>>>>>>> upstream-releases
  if (!data) {
    data = mMessageData.AppendElement();

    if (!data) return nullptr;

    data->nMsg = nMsg;
  }
  return data;
}

<<<<<<< HEAD
void WindowHook::DeleteIfEmpty(MessageData *data) {
||||||| merged common ancestors
void
WindowHook::DeleteIfEmpty(MessageData *data) {
=======
void WindowHook::DeleteIfEmpty(MessageData* data) {
>>>>>>> upstream-releases
  // Never remove a MessageData that has still a hook or monitor entries.
  if (data->hook || !data->monitors.IsEmpty()) return;

  MessageDataArray::index_type idx;
  idx = data - mMessageData.Elements();
  NS_ASSERTION(
      idx < mMessageData.Length(),
      "Attempted to delete MessageData that doesn't belong to this array!");
  mMessageData.RemoveElementAt(idx);
}

<<<<<<< HEAD
bool WindowHook::Notify(HWND hWnd, UINT nMsg, WPARAM wParam, LPARAM lParam,
                        MSGResult &aResult) {
  MessageData *data = Lookup(nMsg);
  if (!data) return false;
||||||| merged common ancestors
bool
WindowHook::Notify(HWND hWnd, UINT nMsg, WPARAM wParam, LPARAM lParam,
                   MSGResult& aResult)
{
  MessageData *data = Lookup(nMsg);
  if (!data)
    return false;
=======
bool WindowHook::Notify(HWND hWnd, UINT nMsg, WPARAM wParam, LPARAM lParam,
                        MSGResult& aResult) {
  MessageData* data = Lookup(nMsg);
  if (!data) return false;
>>>>>>> upstream-releases

  uint32_t length = data->monitors.Length();
  for (uint32_t midx = 0; midx < length; midx++) {
    data->monitors[midx].Invoke(hWnd, nMsg, wParam, lParam, &aResult.mResult);
  }

  aResult.mConsumed =
      data->hook.Invoke(hWnd, nMsg, wParam, lParam, &aResult.mResult);
  return aResult.mConsumed;
}

<<<<<<< HEAD
bool WindowHook::CallbackData::Invoke(HWND hWnd, UINT msg, WPARAM wParam,
                                      LPARAM lParam, LRESULT *aResult) {
  if (!cb) return false;
||||||| merged common ancestors
bool
WindowHook::CallbackData::Invoke(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam,
                                 LRESULT *aResult) {
  if (!cb)
    return false;
=======
bool WindowHook::CallbackData::Invoke(HWND hWnd, UINT msg, WPARAM wParam,
                                      LPARAM lParam, LRESULT* aResult) {
  if (!cb) return false;
>>>>>>> upstream-releases
  return cb(context, hWnd, msg, wParam, lParam, aResult);
}

}  // namespace widget
}  // namespace mozilla
