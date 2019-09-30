/* -*- Mode: C++; tab-width: 2; indent-tabs-mode: nil; c-basic-offset: 2 -*- */
/* This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/. */

#ifndef nsJSNPRuntime_h_
#define nsJSNPRuntime_h_

#include "nscore.h"
#include "npapi.h"
#include "npruntime.h"
#include "PLDHashTable.h"
#include "js/RootingAPI.h"

class nsJSNPRuntime {
 public:
  static void OnPluginDestroy(NPP npp);
  static void OnPluginDestroyPending(NPP npp);
};

<<<<<<< HEAD
class nsJSObjWrapperKey {
 public:
  nsJSObjWrapperKey(JSObject *obj, NPP npp) : mJSObj(obj), mNpp(npp) {}
||||||| merged common ancestors
class nsJSObjWrapperKey
{
public:
  nsJSObjWrapperKey(JSObject *obj, NPP npp)
    : mJSObj(obj), mNpp(npp)
  {
  }
=======
class nsJSObjWrapperKey {
 public:
  nsJSObjWrapperKey(JSObject* obj, NPP npp) : mJSObj(obj), mNpp(npp) {}
>>>>>>> upstream-releases

  bool operator==(const nsJSObjWrapperKey &other) const {
    return mJSObj == other.mJSObj && mNpp == other.mNpp;
  }
  bool operator!=(const nsJSObjWrapperKey &other) const {
    return !(*this == other);
  }

<<<<<<< HEAD
  void trace(JSTracer *trc) {
    JS::TraceEdge(trc, &mJSObj, "nsJSObjWrapperKey");
||||||| merged common ancestors
  void trace(JSTracer* trc) {
      JS::TraceEdge(trc, &mJSObj, "nsJSObjWrapperKey");
=======
  void trace(JSTracer* trc) {
    JS::TraceEdge(trc, &mJSObj, "nsJSObjWrapperKey");
>>>>>>> upstream-releases
  }

<<<<<<< HEAD
  nsJSObjWrapperKey(const nsJSObjWrapperKey &other)
      : mJSObj(other.mJSObj), mNpp(other.mNpp) {}
  void operator=(const nsJSObjWrapperKey &other) {
||||||| merged common ancestors
  nsJSObjWrapperKey(const nsJSObjWrapperKey& other)
    : mJSObj(other.mJSObj),
      mNpp(other.mNpp)
  {}
  void operator=(const nsJSObjWrapperKey& other) {
=======
  nsJSObjWrapperKey(const nsJSObjWrapperKey& other)
      : mJSObj(other.mJSObj), mNpp(other.mNpp) {}
  void operator=(const nsJSObjWrapperKey& other) {
>>>>>>> upstream-releases
    mJSObj = other.mJSObj;
    mNpp = other.mNpp;
  }

  JS::Heap<JSObject *> mJSObj;
  NPP mNpp;
};

<<<<<<< HEAD
class nsJSObjWrapper : public NPObject {
 public:
  JS::Heap<JSObject *> mJSObj;
||||||| merged common ancestors
class nsJSObjWrapper : public NPObject
{
public:
  JS::Heap<JSObject*> mJSObj;
=======
class nsJSObjWrapper : public NPObject {
 public:
  JS::Heap<JSObject*> mJSObj;
>>>>>>> upstream-releases
  // Because mJSObj might be a cross-compartment wrapper, we can't use it to
  // enter the target realm. We use this global instead (it's always
  // same-compartment with mJSObj).
  JS::Heap<JSObject *> mJSObjGlobal;
  const NPP mNpp;
  bool mDestroyPending;

  static NPObject *GetNewOrUsed(NPP npp, JS::Handle<JSObject *> obj,
                                JS::Handle<JSObject *> objGlobal);

<<<<<<< HEAD
  void trace(JSTracer *trc) {
    JS::TraceEdge(trc, &mJSObj, "nsJSObjWrapper::mJSObj");
    JS::TraceEdge(trc, &mJSObjGlobal, "nsJSObjWrapper::mJSObjGlobal");
||||||| merged common ancestors
  void trace(JSTracer* trc) {
      JS::TraceEdge(trc, &mJSObj, "nsJSObjWrapper::mJSObj");
      JS::TraceEdge(trc, &mJSObjGlobal, "nsJSObjWrapper::mJSObjGlobal");
=======
  void trace(JSTracer* trc) {
    JS::TraceEdge(trc, &mJSObj, "nsJSObjWrapper::mJSObj");
    JS::TraceEdge(trc, &mJSObjGlobal, "nsJSObjWrapper::mJSObjGlobal");
>>>>>>> upstream-releases
  }

 protected:
  explicit nsJSObjWrapper(NPP npp);
  ~nsJSObjWrapper();

<<<<<<< HEAD
  static NPObject *NP_Allocate(NPP npp, NPClass *aClass);
  static void NP_Deallocate(NPObject *obj);
  static void NP_Invalidate(NPObject *obj);
  static bool NP_HasMethod(NPObject *, NPIdentifier identifier);
  static bool NP_Invoke(NPObject *obj, NPIdentifier method,
                        const NPVariant *args, uint32_t argCount,
                        NPVariant *result);
  static bool NP_InvokeDefault(NPObject *obj, const NPVariant *args,
                               uint32_t argCount, NPVariant *result);
  static bool NP_HasProperty(NPObject *obj, NPIdentifier property);
  static bool NP_GetProperty(NPObject *obj, NPIdentifier property,
                             NPVariant *result);
  static bool NP_SetProperty(NPObject *obj, NPIdentifier property,
                             const NPVariant *value);
  static bool NP_RemoveProperty(NPObject *obj, NPIdentifier property);
  static bool NP_Enumerate(NPObject *npobj, NPIdentifier **identifier,
                           uint32_t *count);
  static bool NP_Construct(NPObject *obj, const NPVariant *args,
                           uint32_t argCount, NPVariant *result);

 public:
||||||| merged common ancestors
  static NPObject * NP_Allocate(NPP npp, NPClass *aClass);
  static void NP_Deallocate(NPObject *obj);
  static void NP_Invalidate(NPObject *obj);
  static bool NP_HasMethod(NPObject *, NPIdentifier identifier);
  static bool NP_Invoke(NPObject *obj, NPIdentifier method,
                        const NPVariant *args, uint32_t argCount,
                        NPVariant *result);
  static bool NP_InvokeDefault(NPObject *obj, const NPVariant *args,
                               uint32_t argCount, NPVariant *result);
  static bool NP_HasProperty(NPObject * obj, NPIdentifier property);
  static bool NP_GetProperty(NPObject *obj, NPIdentifier property,
                             NPVariant *result);
  static bool NP_SetProperty(NPObject *obj, NPIdentifier property,
                             const NPVariant *value);
  static bool NP_RemoveProperty(NPObject *obj, NPIdentifier property);
  static bool NP_Enumerate(NPObject *npobj, NPIdentifier **identifier,
                           uint32_t *count);
  static bool NP_Construct(NPObject *obj, const NPVariant *args,
                           uint32_t argCount, NPVariant *result);

public:
=======
  static NPObject* NP_Allocate(NPP npp, NPClass* aClass);
  static void NP_Deallocate(NPObject* obj);
  static void NP_Invalidate(NPObject* obj);
  static bool NP_HasMethod(NPObject*, NPIdentifier identifier);
  static bool NP_Invoke(NPObject* obj, NPIdentifier method,
                        const NPVariant* args, uint32_t argCount,
                        NPVariant* result);
  static bool NP_InvokeDefault(NPObject* obj, const NPVariant* args,
                               uint32_t argCount, NPVariant* result);
  static bool NP_HasProperty(NPObject* obj, NPIdentifier property);
  static bool NP_GetProperty(NPObject* obj, NPIdentifier property,
                             NPVariant* result);
  static bool NP_SetProperty(NPObject* obj, NPIdentifier property,
                             const NPVariant* value);
  static bool NP_RemoveProperty(NPObject* obj, NPIdentifier property);
  static bool NP_Enumerate(NPObject* npobj, NPIdentifier** identifier,
                           uint32_t* count);
  static bool NP_Construct(NPObject* obj, const NPVariant* args,
                           uint32_t argCount, NPVariant* result);

 public:
>>>>>>> upstream-releases
  static NPClass sJSObjWrapperNPClass;
};

<<<<<<< HEAD
class nsNPObjWrapper {
 public:
  static bool IsWrapper(JSObject *obj);
  static void OnDestroy(NPObject *npobj);
  static JSObject *GetNewOrUsed(NPP npp, JSContext *cx, NPObject *npobj);
||||||| merged common ancestors
class nsNPObjWrapper
{
public:
  static bool IsWrapper(JSObject *obj);
  static void OnDestroy(NPObject *npobj);
  static JSObject *GetNewOrUsed(NPP npp, JSContext *cx, NPObject *npobj);
=======
class nsNPObjWrapper {
 public:
  static bool IsWrapper(JSObject* obj);
  static void OnDestroy(NPObject* npobj);
  static JSObject* GetNewOrUsed(NPP npp, JSContext* cx, NPObject* npobj);
>>>>>>> upstream-releases
};

<<<<<<< HEAD
bool JSValToNPVariant(NPP npp, JSContext *cx, const JS::Value &val,
                      NPVariant *variant);
||||||| merged common ancestors
bool
JSValToNPVariant(NPP npp, JSContext *cx, const JS::Value& val, NPVariant *variant);

=======
bool JSValToNPVariant(NPP npp, JSContext* cx, const JS::Value& val,
                      NPVariant* variant);
>>>>>>> upstream-releases

#endif  // nsJSNPRuntime_h_
