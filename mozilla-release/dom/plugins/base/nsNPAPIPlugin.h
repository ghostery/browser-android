/* -*- Mode: C++; tab-width: 2; indent-tabs-mode: nil; c-basic-offset: 2 -*- */
/* This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/. */

#ifndef nsNPAPIPlugin_h_
#define nsNPAPIPlugin_h_

#include "prlink.h"
#include "npfunctions.h"
#include "nsPluginHost.h"

#include "mozilla/dom/ScriptSettings.h"
#include "mozilla/PluginLibrary.h"
#include "mozilla/RefCounted.h"

// nsNPAPIPlugin is held alive both by active nsPluginTag instances and
// by active nsNPAPIPluginInstance.
class nsNPAPIPlugin final {
 private:
  typedef mozilla::PluginLibrary PluginLibrary;

 public:
  nsNPAPIPlugin();

  NS_INLINE_DECL_REFCOUNTING(nsNPAPIPlugin)

  // Constructs and initializes an nsNPAPIPlugin object. A nullptr file path
  // will prevent this from calling NP_Initialize.
<<<<<<< HEAD
  static nsresult CreatePlugin(nsPluginTag *aPluginTag,
                               nsNPAPIPlugin **aResult);
||||||| merged common ancestors
  static nsresult CreatePlugin(nsPluginTag *aPluginTag, nsNPAPIPlugin** aResult);
=======
  static nsresult CreatePlugin(nsPluginTag* aPluginTag,
                               nsNPAPIPlugin** aResult);
>>>>>>> upstream-releases

<<<<<<< HEAD
  PluginLibrary *GetLibrary();
  // PluginFuncs() can't fail but results are only valid if GetLibrary()
  // succeeds
  NPPluginFuncs *PluginFuncs();
||||||| merged common ancestors
  PluginLibrary* GetLibrary();
  // PluginFuncs() can't fail but results are only valid if GetLibrary() succeeds
  NPPluginFuncs* PluginFuncs();
=======
  PluginLibrary* GetLibrary();
  // PluginFuncs() can't fail but results are only valid if GetLibrary()
  // succeeds
  NPPluginFuncs* PluginFuncs();
>>>>>>> upstream-releases

#if defined(XP_MACOSX) && !defined(__LP64__)
  void SetPluginRefNum(short aRefNum);
#endif

  // The IPC mechanism notifies the nsNPAPIPlugin if the plugin
  // crashes and is no longer usable. pluginDumpID/browserDumpID are
  // the IDs of respective minidumps that were written, or empty if no
  // minidump was written.
  void PluginCrashed(const nsAString &pluginDumpID,
                     const nsAString &browserDumpID);

  nsresult Shutdown();

  static nsresult RetainStream(NPStream* pstream, nsISupports** aRetainedPeer);

 private:
  ~nsNPAPIPlugin();

  NPPluginFuncs mPluginFuncs;
  PluginLibrary *mLibrary;
};

namespace mozilla {
namespace plugins {
namespace parent {

static_assert(sizeof(NPIdentifier) == sizeof(jsid),
              "NPIdentifier must be binary compatible with jsid.");

inline jsid NPIdentifierToJSId(NPIdentifier id) {
  jsid tmp;
  JSID_BITS(tmp) = (size_t)id;
  return tmp;
}

inline NPIdentifier JSIdToNPIdentifier(jsid id) {
  return (NPIdentifier)JSID_BITS(id);
}

inline bool NPIdentifierIsString(NPIdentifier id) {
  return JSID_IS_STRING(NPIdentifierToJSId(id));
}

<<<<<<< HEAD
inline JSString *NPIdentifierToString(NPIdentifier id) {
  return JSID_TO_STRING(NPIdentifierToJSId(id));
||||||| merged common ancestors
inline JSString *
NPIdentifierToString(NPIdentifier id)
{
    return JSID_TO_STRING(NPIdentifierToJSId(id));
=======
inline JSString* NPIdentifierToString(NPIdentifier id) {
  return JSID_TO_STRING(NPIdentifierToJSId(id));
>>>>>>> upstream-releases
}

<<<<<<< HEAD
inline NPIdentifier StringToNPIdentifier(JSContext *cx, JSString *str) {
  return JSIdToNPIdentifier(INTERNED_STRING_TO_JSID(cx, str));
||||||| merged common ancestors
inline NPIdentifier
StringToNPIdentifier(JSContext *cx, JSString *str)
{
    return JSIdToNPIdentifier(INTERNED_STRING_TO_JSID(cx, str));
=======
inline NPIdentifier StringToNPIdentifier(JSContext* cx, JSString* str) {
  return JSIdToNPIdentifier(INTERNED_STRING_TO_JSID(cx, str));
>>>>>>> upstream-releases
}

inline bool NPIdentifierIsInt(NPIdentifier id) {
  return JSID_IS_INT(NPIdentifierToJSId(id));
}

inline int NPIdentifierToInt(NPIdentifier id) {
  return JSID_TO_INT(NPIdentifierToJSId(id));
}

inline NPIdentifier IntToNPIdentifier(int i) {
  return JSIdToNPIdentifier(INT_TO_JSID(i));
}

JSContext *GetJSContext(NPP npp);

inline bool NPStringIdentifierIsPermanent(NPIdentifier id) {
  AutoSafeJSContext cx;
  return JS_StringHasBeenPinned(cx, NPIdentifierToString(id));
}

#define NPIdentifier_VOID (JSIdToNPIdentifier(JSID_VOID))

<<<<<<< HEAD
NPObject *_getwindowobject(NPP npp);
||||||| merged common ancestors
NPObject*
_getwindowobject(NPP npp);
=======
NPObject* _getwindowobject(NPP npp);
>>>>>>> upstream-releases

<<<<<<< HEAD
NPObject *_getpluginelement(NPP npp);
||||||| merged common ancestors
NPObject*
_getpluginelement(NPP npp);
=======
NPObject* _getpluginelement(NPP npp);
>>>>>>> upstream-releases

<<<<<<< HEAD
NPIdentifier _getstringidentifier(const NPUTF8 *name);
||||||| merged common ancestors
NPIdentifier
_getstringidentifier(const NPUTF8* name);
=======
NPIdentifier _getstringidentifier(const NPUTF8* name);
>>>>>>> upstream-releases

<<<<<<< HEAD
void _getstringidentifiers(const NPUTF8 **names, int32_t nameCount,
                           NPIdentifier *identifiers);
||||||| merged common ancestors
void
_getstringidentifiers(const NPUTF8** names, int32_t nameCount,
                      NPIdentifier *identifiers);
=======
void _getstringidentifiers(const NPUTF8** names, int32_t nameCount,
                           NPIdentifier* identifiers);
>>>>>>> upstream-releases

bool _identifierisstring(NPIdentifier identifiers);

NPIdentifier _getintidentifier(int32_t intid);

<<<<<<< HEAD
NPUTF8 *_utf8fromidentifier(NPIdentifier identifier);
||||||| merged common ancestors
NPUTF8*
_utf8fromidentifier(NPIdentifier identifier);
=======
NPUTF8* _utf8fromidentifier(NPIdentifier identifier);
>>>>>>> upstream-releases

int32_t _intfromidentifier(NPIdentifier identifier);

<<<<<<< HEAD
NPObject *_createobject(NPP npp, NPClass *aClass);
||||||| merged common ancestors
NPObject*
_createobject(NPP npp, NPClass* aClass);
=======
NPObject* _createobject(NPP npp, NPClass* aClass);
>>>>>>> upstream-releases

<<<<<<< HEAD
NPObject *_retainobject(NPObject *npobj);
||||||| merged common ancestors
NPObject*
_retainobject(NPObject* npobj);
=======
NPObject* _retainobject(NPObject* npobj);
>>>>>>> upstream-releases

<<<<<<< HEAD
void _releaseobject(NPObject *npobj);
||||||| merged common ancestors
void
_releaseobject(NPObject* npobj);
=======
void _releaseobject(NPObject* npobj);
>>>>>>> upstream-releases

<<<<<<< HEAD
bool _invoke(NPP npp, NPObject *npobj, NPIdentifier method,
             const NPVariant *args, uint32_t argCount, NPVariant *result);
||||||| merged common ancestors
bool
_invoke(NPP npp, NPObject* npobj, NPIdentifier method, const NPVariant *args,
        uint32_t argCount, NPVariant *result);
=======
bool _invoke(NPP npp, NPObject* npobj, NPIdentifier method,
             const NPVariant* args, uint32_t argCount, NPVariant* result);
>>>>>>> upstream-releases

<<<<<<< HEAD
bool _invokeDefault(NPP npp, NPObject *npobj, const NPVariant *args,
                    uint32_t argCount, NPVariant *result);
||||||| merged common ancestors
bool
_invokeDefault(NPP npp, NPObject* npobj, const NPVariant *args,
               uint32_t argCount, NPVariant *result);
=======
bool _invokeDefault(NPP npp, NPObject* npobj, const NPVariant* args,
                    uint32_t argCount, NPVariant* result);
>>>>>>> upstream-releases

<<<<<<< HEAD
bool _evaluate(NPP npp, NPObject *npobj, NPString *script, NPVariant *result);
||||||| merged common ancestors
bool
_evaluate(NPP npp, NPObject* npobj, NPString *script, NPVariant *result);
=======
bool _evaluate(NPP npp, NPObject* npobj, NPString* script, NPVariant* result);
>>>>>>> upstream-releases

<<<<<<< HEAD
bool _getproperty(NPP npp, NPObject *npobj, NPIdentifier property,
                  NPVariant *result);
||||||| merged common ancestors
bool
_getproperty(NPP npp, NPObject* npobj, NPIdentifier property,
             NPVariant *result);
=======
bool _getproperty(NPP npp, NPObject* npobj, NPIdentifier property,
                  NPVariant* result);
>>>>>>> upstream-releases

<<<<<<< HEAD
bool _setproperty(NPP npp, NPObject *npobj, NPIdentifier property,
                  const NPVariant *value);
||||||| merged common ancestors
bool
_setproperty(NPP npp, NPObject* npobj, NPIdentifier property,
             const NPVariant *value);
=======
bool _setproperty(NPP npp, NPObject* npobj, NPIdentifier property,
                  const NPVariant* value);
>>>>>>> upstream-releases

<<<<<<< HEAD
bool _removeproperty(NPP npp, NPObject *npobj, NPIdentifier property);
||||||| merged common ancestors
bool
_removeproperty(NPP npp, NPObject* npobj, NPIdentifier property);
=======
bool _removeproperty(NPP npp, NPObject* npobj, NPIdentifier property);
>>>>>>> upstream-releases

<<<<<<< HEAD
bool _hasproperty(NPP npp, NPObject *npobj, NPIdentifier propertyName);
||||||| merged common ancestors
bool
_hasproperty(NPP npp, NPObject* npobj, NPIdentifier propertyName);
=======
bool _hasproperty(NPP npp, NPObject* npobj, NPIdentifier propertyName);
>>>>>>> upstream-releases

<<<<<<< HEAD
bool _hasmethod(NPP npp, NPObject *npobj, NPIdentifier methodName);
||||||| merged common ancestors
bool
_hasmethod(NPP npp, NPObject* npobj, NPIdentifier methodName);
=======
bool _hasmethod(NPP npp, NPObject* npobj, NPIdentifier methodName);
>>>>>>> upstream-releases

<<<<<<< HEAD
bool _enumerate(NPP npp, NPObject *npobj, NPIdentifier **identifier,
                uint32_t *count);
||||||| merged common ancestors
bool
_enumerate(NPP npp, NPObject *npobj, NPIdentifier **identifier,
           uint32_t *count);
=======
bool _enumerate(NPP npp, NPObject* npobj, NPIdentifier** identifier,
                uint32_t* count);
>>>>>>> upstream-releases

<<<<<<< HEAD
bool _construct(NPP npp, NPObject *npobj, const NPVariant *args,
                uint32_t argCount, NPVariant *result);
||||||| merged common ancestors
bool
_construct(NPP npp, NPObject* npobj, const NPVariant *args,
           uint32_t argCount, NPVariant *result);
=======
bool _construct(NPP npp, NPObject* npobj, const NPVariant* args,
                uint32_t argCount, NPVariant* result);
>>>>>>> upstream-releases

<<<<<<< HEAD
void _releasevariantvalue(NPVariant *variant);
||||||| merged common ancestors
void
_releasevariantvalue(NPVariant *variant);
=======
void _releasevariantvalue(NPVariant* variant);
>>>>>>> upstream-releases

<<<<<<< HEAD
void _setexception(NPObject *npobj, const NPUTF8 *message);
||||||| merged common ancestors
void
_setexception(NPObject* npobj, const NPUTF8 *message);
=======
void _setexception(NPObject* npobj, const NPUTF8* message);
>>>>>>> upstream-releases

void _pushpopupsenabledstate(NPP npp, NPBool enabled);

void _poppopupsenabledstate(NPP npp);

<<<<<<< HEAD
NPError _getvalueforurl(NPP instance, NPNURLVariable variable, const char *url,
                        char **value, uint32_t *len);
||||||| merged common ancestors
NPError
_getvalueforurl(NPP instance, NPNURLVariable variable, const char *url,
                char **value, uint32_t *len);
=======
NPError _getvalueforurl(NPP instance, NPNURLVariable variable, const char* url,
                        char** value, uint32_t* len);
>>>>>>> upstream-releases

<<<<<<< HEAD
NPError _setvalueforurl(NPP instance, NPNURLVariable variable, const char *url,
                        const char *value, uint32_t len);
||||||| merged common ancestors
NPError
_setvalueforurl(NPP instance, NPNURLVariable variable, const char *url,
                const char *value, uint32_t len);
=======
NPError _setvalueforurl(NPP instance, NPNURLVariable variable, const char* url,
                        const char* value, uint32_t len);
>>>>>>> upstream-releases

typedef void (*PluginTimerFunc)(NPP npp, uint32_t timerID);

uint32_t _scheduletimer(NPP instance, uint32_t interval, NPBool repeat,
                        PluginTimerFunc timerFunc);

void _unscheduletimer(NPP instance, uint32_t timerID);

<<<<<<< HEAD
NPError _popupcontextmenu(NPP instance, NPMenu *menu);
||||||| merged common ancestors
NPError
_popupcontextmenu(NPP instance, NPMenu* menu);
=======
NPError _popupcontextmenu(NPP instance, NPMenu* menu);
>>>>>>> upstream-releases

<<<<<<< HEAD
NPBool _convertpoint(NPP instance, double sourceX, double sourceY,
                     NPCoordinateSpace sourceSpace, double *destX,
                     double *destY, NPCoordinateSpace destSpace);
||||||| merged common ancestors
NPBool
_convertpoint(NPP instance, double sourceX, double sourceY, NPCoordinateSpace sourceSpace, double *destX, double *destY, NPCoordinateSpace destSpace);
=======
NPBool _convertpoint(NPP instance, double sourceX, double sourceY,
                     NPCoordinateSpace sourceSpace, double* destX,
                     double* destY, NPCoordinateSpace destSpace);
>>>>>>> upstream-releases

<<<<<<< HEAD
NPError _requestread(NPStream *pstream, NPByteRange *rangeList);
||||||| merged common ancestors
NPError
_requestread(NPStream *pstream, NPByteRange *rangeList);
=======
NPError _requestread(NPStream* pstream, NPByteRange* rangeList);
>>>>>>> upstream-releases

<<<<<<< HEAD
NPError _geturlnotify(NPP npp, const char *relativeURL, const char *target,
                      void *notifyData);
||||||| merged common ancestors
NPError
_geturlnotify(NPP npp, const char* relativeURL, const char* target,
              void* notifyData);
=======
NPError _geturlnotify(NPP npp, const char* relativeURL, const char* target,
                      void* notifyData);
>>>>>>> upstream-releases

<<<<<<< HEAD
NPError _getvalue(NPP npp, NPNVariable variable, void *r_value);
||||||| merged common ancestors
NPError
_getvalue(NPP npp, NPNVariable variable, void *r_value);
=======
NPError _getvalue(NPP npp, NPNVariable variable, void* r_value);
>>>>>>> upstream-releases

<<<<<<< HEAD
NPError _setvalue(NPP npp, NPPVariable variable, void *r_value);
||||||| merged common ancestors
NPError
_setvalue(NPP npp, NPPVariable variable, void *r_value);
=======
NPError _setvalue(NPP npp, NPPVariable variable, void* r_value);
>>>>>>> upstream-releases

<<<<<<< HEAD
NPError _geturl(NPP npp, const char *relativeURL, const char *target);
||||||| merged common ancestors
NPError
_geturl(NPP npp, const char* relativeURL, const char* target);
=======
NPError _geturl(NPP npp, const char* relativeURL, const char* target);
>>>>>>> upstream-releases

<<<<<<< HEAD
NPError _posturlnotify(NPP npp, const char *relativeURL, const char *target,
                       uint32_t len, const char *buf, NPBool file,
                       void *notifyData);
||||||| merged common ancestors
NPError
_posturlnotify(NPP npp, const char* relativeURL, const char *target,
               uint32_t len, const char *buf, NPBool file, void* notifyData);
=======
NPError _posturlnotify(NPP npp, const char* relativeURL, const char* target,
                       uint32_t len, const char* buf, NPBool file,
                       void* notifyData);
>>>>>>> upstream-releases

<<<<<<< HEAD
NPError _posturl(NPP npp, const char *relativeURL, const char *target,
                 uint32_t len, const char *buf, NPBool file);
||||||| merged common ancestors
NPError
_posturl(NPP npp, const char* relativeURL, const char *target, uint32_t len,
            const char *buf, NPBool file);
=======
NPError _posturl(NPP npp, const char* relativeURL, const char* target,
                 uint32_t len, const char* buf, NPBool file);
>>>>>>> upstream-releases

<<<<<<< HEAD
void _status(NPP npp, const char *message);
||||||| merged common ancestors
void
_status(NPP npp, const char *message);
=======
void _status(NPP npp, const char* message);
>>>>>>> upstream-releases

<<<<<<< HEAD
void _memfree(void *ptr);
||||||| merged common ancestors
void
_memfree (void *ptr);
=======
void _memfree(void* ptr);
>>>>>>> upstream-releases

uint32_t _memflush(uint32_t size);

void _reloadplugins(NPBool reloadPages);

<<<<<<< HEAD
void _invalidaterect(NPP npp, NPRect *invalidRect);
||||||| merged common ancestors
void
_invalidaterect(NPP npp, NPRect *invalidRect);
=======
void _invalidaterect(NPP npp, NPRect* invalidRect);
>>>>>>> upstream-releases

void _invalidateregion(NPP npp, NPRegion invalidRegion);

void _forceredraw(NPP npp);

<<<<<<< HEAD
const char *_useragent(NPP npp);
||||||| merged common ancestors
const char*
_useragent(NPP npp);
=======
const char* _useragent(NPP npp);
>>>>>>> upstream-releases

<<<<<<< HEAD
void *_memalloc(uint32_t size);
||||||| merged common ancestors
void*
_memalloc (uint32_t size);
=======
void* _memalloc(uint32_t size);
>>>>>>> upstream-releases

// Deprecated entry points for the old Java plugin.
void * /* OJI type: JRIEnv* */
_getJavaEnv();

void * /* OJI type: jref */
_getJavaPeer(NPP npp);

<<<<<<< HEAD
void _urlredirectresponse(NPP instance, void *notifyData, NPBool allow);
||||||| merged common ancestors
void
_urlredirectresponse(NPP instance, void* notifyData, NPBool allow);
=======
void _urlredirectresponse(NPP instance, void* notifyData, NPBool allow);
>>>>>>> upstream-releases

<<<<<<< HEAD
NPError _initasyncsurface(NPP instance, NPSize *size, NPImageFormat format,
                          void *initData, NPAsyncSurface *surface);
||||||| merged common ancestors
NPError
_initasyncsurface(NPP instance, NPSize *size, NPImageFormat format, void *initData, NPAsyncSurface *surface);
=======
NPError _initasyncsurface(NPP instance, NPSize* size, NPImageFormat format,
                          void* initData, NPAsyncSurface* surface);
>>>>>>> upstream-releases

<<<<<<< HEAD
NPError _finalizeasyncsurface(NPP instance, NPAsyncSurface *surface);
||||||| merged common ancestors
NPError
_finalizeasyncsurface(NPP instance, NPAsyncSurface *surface);
=======
NPError _finalizeasyncsurface(NPP instance, NPAsyncSurface* surface);
>>>>>>> upstream-releases

<<<<<<< HEAD
void _setcurrentasyncsurface(NPP instance, NPAsyncSurface *surface,
                             NPRect *changed);
||||||| merged common ancestors
void
_setcurrentasyncsurface(NPP instance, NPAsyncSurface *surface, NPRect *changed);
=======
void _setcurrentasyncsurface(NPP instance, NPAsyncSurface* surface,
                             NPRect* changed);
>>>>>>> upstream-releases

} /* namespace parent */
} /* namespace plugins */
} /* namespace mozilla */

<<<<<<< HEAD
const char *PeekException();
||||||| merged common ancestors
const char *
PeekException();
=======
const char* PeekException();
>>>>>>> upstream-releases

void PopException();

class NPPStack {
 public:
  static NPP Peek() { return sCurrentNPP; }

 protected:
  static NPP sCurrentNPP;
};

// XXXjst: The NPPAutoPusher stack is a bit redundant now that
// PluginDestructionGuard exists, and could thus be replaced by code
// that uses the PluginDestructionGuard list of plugins on the
// stack. But they're not identical, and to minimize code changes
// we're keeping both for the moment, and making NPPAutoPusher inherit
// the PluginDestructionGuard class to avoid having to keep two
// separate objects on the stack since we always want a
// PluginDestructionGuard where we use an NPPAutoPusher.

class MOZ_STACK_CLASS NPPAutoPusher : public NPPStack,
                                      protected PluginDestructionGuard {
 public:
  explicit NPPAutoPusher(NPP aNpp)
      : PluginDestructionGuard(aNpp), mOldNPP(sCurrentNPP) {
    NS_ASSERTION(aNpp, "Uh, null aNpp passed to NPPAutoPusher!");

    sCurrentNPP = aNpp;
  }

  ~NPPAutoPusher() { sCurrentNPP = mOldNPP; }

 private:
  NPP mOldNPP;
};

class NPPExceptionAutoHolder {
 public:
  NPPExceptionAutoHolder();
  ~NPPExceptionAutoHolder();

<<<<<<< HEAD
 protected:
  char *mOldException;
||||||| merged common ancestors
protected:
  char *mOldException;
=======
 protected:
  char* mOldException;
>>>>>>> upstream-releases
};

#endif  // nsNPAPIPlugin_h_
