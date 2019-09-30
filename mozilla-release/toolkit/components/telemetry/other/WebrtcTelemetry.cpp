/* -*- Mode: C++; tab-width: 8; indent-tabs-mode: nil; c-basic-offset: 2 -*- */
/* vim: set ts=8 sts=2 et sw=2 tw=80: */
/* This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/. */

#include "WebrtcTelemetry.h"

#include "jsapi.h"
#include "mozilla/Telemetry.h"
#include "nsPrintfCString.h"
#include "nsTHashtable.h"
<<<<<<< HEAD
void WebrtcTelemetry::RecordIceCandidateMask(const uint32_t iceCandidateBitmask,
                                             const bool success) {
  WebrtcIceCandidateType *entry =
      mWebrtcIceCandidates.GetEntry(iceCandidateBitmask);
||||||| merged common ancestors
void
WebrtcTelemetry::RecordIceCandidateMask(const uint32_t iceCandidateBitmask,
                                        const bool success)
{
  WebrtcIceCandidateType *entry = mWebrtcIceCandidates.GetEntry(iceCandidateBitmask);
=======
void WebrtcTelemetry::RecordIceCandidateMask(const uint32_t iceCandidateBitmask,
                                             const bool success) {
  WebrtcIceCandidateType* entry =
      mWebrtcIceCandidates.GetEntry(iceCandidateBitmask);
>>>>>>> upstream-releases
  if (!entry) {
    entry = mWebrtcIceCandidates.PutEntry(iceCandidateBitmask);
    if (MOZ_UNLIKELY(!entry)) return;
  }

  if (success) {
    entry->mData.webrtc.successCount++;
  } else {
    entry->mData.webrtc.failureCount++;
  }
}

<<<<<<< HEAD
bool ReflectIceEntry(const WebrtcTelemetry::WebrtcIceCandidateType *entry,
                     const WebrtcTelemetry::WebrtcIceCandidateStats *stat,
                     JSContext *cx, JS::Handle<JSObject *> obj) {
  if ((stat->successCount == 0) && (stat->failureCount == 0)) return true;
||||||| merged common ancestors
bool
ReflectIceEntry(const WebrtcTelemetry::WebrtcIceCandidateType *entry,
                const WebrtcTelemetry::WebrtcIceCandidateStats *stat, JSContext *cx,
                JS::Handle<JSObject*> obj)
{
  if ((stat->successCount == 0) && (stat->failureCount == 0))
    return true;
=======
bool ReflectIceEntry(const WebrtcTelemetry::WebrtcIceCandidateType* entry,
                     const WebrtcTelemetry::WebrtcIceCandidateStats* stat,
                     JSContext* cx, JS::Handle<JSObject*> obj) {
  if ((stat->successCount == 0) && (stat->failureCount == 0)) return true;
>>>>>>> upstream-releases

  const uint32_t& bitmask = entry->GetKey();

<<<<<<< HEAD
  JS::Rooted<JSObject *> statsObj(cx, JS_NewPlainObject(cx));
  if (!statsObj) return false;
||||||| merged common ancestors
  JS::Rooted<JSObject*> statsObj(cx, JS_NewPlainObject(cx));
  if (!statsObj)
    return false;
=======
  JS::Rooted<JSObject*> statsObj(cx, JS_NewPlainObject(cx));
  if (!statsObj) return false;
>>>>>>> upstream-releases
  if (!JS_DefineProperty(cx, obj,
                         nsPrintfCString("%" PRIu32, bitmask).BeginReading(),
                         statsObj, JSPROP_ENUMERATE)) {
    return false;
  }
  if (stat->successCount &&
      !JS_DefineProperty(cx, statsObj, "successCount", stat->successCount,
                         JSPROP_ENUMERATE)) {
    return false;
  }
  if (stat->failureCount &&
      !JS_DefineProperty(cx, statsObj, "failureCount", stat->failureCount,
                         JSPROP_ENUMERATE)) {
    return false;
  }
  return true;
}

<<<<<<< HEAD
bool ReflectIceWebrtc(WebrtcTelemetry::WebrtcIceCandidateType *entry,
                      JSContext *cx, JS::Handle<JSObject *> obj) {
||||||| merged common ancestors
bool
ReflectIceWebrtc(WebrtcTelemetry::WebrtcIceCandidateType *entry, JSContext *cx,
                 JS::Handle<JSObject*> obj)
{
=======
bool ReflectIceWebrtc(WebrtcTelemetry::WebrtcIceCandidateType* entry,
                      JSContext* cx, JS::Handle<JSObject*> obj) {
>>>>>>> upstream-releases
  return ReflectIceEntry(entry, &entry->mData.webrtc, cx, obj);
}

<<<<<<< HEAD
bool WebrtcTelemetry::AddIceInfo(JSContext *cx, JS::Handle<JSObject *> iceObj) {
  JS::Rooted<JSObject *> statsObj(cx, JS_NewPlainObject(cx));
  if (!statsObj) return false;
||||||| merged common ancestors
bool
WebrtcTelemetry::AddIceInfo(JSContext *cx, JS::Handle<JSObject*> iceObj)
{
  JS::Rooted<JSObject*> statsObj(cx, JS_NewPlainObject(cx));
  if (!statsObj)
    return false;
=======
bool WebrtcTelemetry::AddIceInfo(JSContext* cx, JS::Handle<JSObject*> iceObj) {
  JS::Rooted<JSObject*> statsObj(cx, JS_NewPlainObject(cx));
  if (!statsObj) return false;
>>>>>>> upstream-releases

  if (!mWebrtcIceCandidates.ReflectIntoJS(ReflectIceWebrtc, cx, statsObj)) {
    return false;
  }

  return JS_DefineProperty(cx, iceObj, "webrtc", statsObj, JSPROP_ENUMERATE);
}

<<<<<<< HEAD
bool WebrtcTelemetry::GetWebrtcStats(JSContext *cx,
                                     JS::MutableHandle<JS::Value> ret) {
  JS::Rooted<JSObject *> root_obj(cx, JS_NewPlainObject(cx));
  if (!root_obj) return false;
||||||| merged common ancestors
bool
WebrtcTelemetry::GetWebrtcStats(JSContext *cx, JS::MutableHandle<JS::Value> ret)
{
  JS::Rooted<JSObject*> root_obj(cx, JS_NewPlainObject(cx));
  if (!root_obj)
    return false;
=======
bool WebrtcTelemetry::GetWebrtcStats(JSContext* cx,
                                     JS::MutableHandle<JS::Value> ret) {
  JS::Rooted<JSObject*> root_obj(cx, JS_NewPlainObject(cx));
  if (!root_obj) return false;
>>>>>>> upstream-releases
  ret.setObject(*root_obj);

<<<<<<< HEAD
  JS::Rooted<JSObject *> ice_obj(cx, JS_NewPlainObject(cx));
  if (!ice_obj) return false;
||||||| merged common ancestors
  JS::Rooted<JSObject*> ice_obj(cx, JS_NewPlainObject(cx));
  if (!ice_obj)
    return false;
=======
  JS::Rooted<JSObject*> ice_obj(cx, JS_NewPlainObject(cx));
  if (!ice_obj) return false;
>>>>>>> upstream-releases
  JS_DefineProperty(cx, root_obj, "IceCandidatesStats", ice_obj,
                    JSPROP_ENUMERATE);

  if (!AddIceInfo(cx, ice_obj)) return false;

  return true;
}

size_t WebrtcTelemetry::SizeOfExcludingThis(
    mozilla::MallocSizeOf aMallocSizeOf) const {
  return mWebrtcIceCandidates.ShallowSizeOfExcludingThis(aMallocSizeOf);
}
