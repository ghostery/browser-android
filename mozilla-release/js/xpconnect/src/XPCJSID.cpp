/* -*- Mode: C++; tab-width: 8; indent-tabs-mode: nil; c-basic-offset: 2 -*- */
/* vim: set ts=8 sts=2 et sw=2 tw=80: */
/* This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/. */

/* An xpcom implementation of the JavaScript nsIID and nsCID objects. */

#include "xpcprivate.h"
#include "mozilla/dom/BindingUtils.h"
#include "mozilla/Attributes.h"
#include "mozilla/jsipc/CrossProcessObjectWrappers.h"
<<<<<<< HEAD
#include "js/Symbol.h"
#include "mozilla/StaticPtr.h"
||||||| merged common ancestors
#include "mozilla/StaticPtr.h"
=======
#include "js/Symbol.h"
>>>>>>> upstream-releases

using namespace mozilla::dom;
using namespace JS;

namespace xpc {

<<<<<<< HEAD
NS_IMPL_CLASSINFO(nsJSID, nullptr, 0, NS_JS_ID_CID)
NS_IMPL_ISUPPORTS_CI(nsJSID, nsIJSID)

const char nsJSID::gNoString[] = "";

nsJSID::nsJSID()
    : mID(GetInvalidIID()),
      mNumber(const_cast<char*>(gNoString)),
      mName(const_cast<char*>(gNoString)) {}

nsJSID::~nsJSID() {
  if (mNumber && mNumber != gNoString) {
    free(mNumber);
  }
  if (mName && mName != gNoString) {
    free(mName);
  }
}

void nsJSID::Reset() {
  mID = GetInvalidIID();

  if (mNumber && mNumber != gNoString) {
    free(mNumber);
  }
  if (mName && mName != gNoString) {
    free(mName);
  }

  mNumber = mName = nullptr;
}

void nsJSID::SetName(const char* name) {
  MOZ_ASSERT(!mName || mName == gNoString, "name already set");
  MOZ_ASSERT(name, "null name");
  mName = NS_xstrdup(name);
}

NS_IMETHODIMP
nsJSID::GetName(char** aName) {
  if (!aName) {
    return NS_ERROR_NULL_POINTER;
  }

  if (!NameIsSet()) {
    SetNameToNoString();
  }
  MOZ_ASSERT(mName, "name not set");
  *aName = NS_xstrdup(mName);
  return NS_OK;
}

NS_IMETHODIMP
nsJSID::GetNumber(char** aNumber) {
  if (!aNumber) {
    return NS_ERROR_NULL_POINTER;
  }

  if (!mNumber) {
    if (!(mNumber = mID.ToString())) {
      mNumber = const_cast<char*>(gNoString);
    }
  }

  *aNumber = NS_xstrdup(mNumber);
  return NS_OK;
}

NS_IMETHODIMP_(const nsID*)
nsJSID::GetID() { return &mID; }

NS_IMETHODIMP
nsJSID::GetValid(bool* aValid) {
  if (!aValid) {
    return NS_ERROR_NULL_POINTER;
  }

  *aValid = IsValid();
  return NS_OK;
}

NS_IMETHODIMP
nsJSID::Equals(nsIJSID* other, bool* _retval) {
  if (!_retval) {
    return NS_ERROR_NULL_POINTER;
  }

  if (!other || mID.Equals(GetInvalidIID())) {
    *_retval = false;
    return NS_OK;
  }

  *_retval = other->GetID()->Equals(mID);
  return NS_OK;
}

NS_IMETHODIMP
nsJSID::Initialize(const char* idString) {
  if (!idString) {
    return NS_ERROR_NULL_POINTER;
  }

  if (*idString != '\0' && mID.Equals(GetInvalidIID())) {
    Reset();

    if (idString[0] == '{') {
      if (mID.Parse(idString)) {
        return NS_OK;
      }

      // error - reset to invalid state
      mID = GetInvalidIID();
    }
  }
  return NS_ERROR_FAILURE;
}

void nsJSID::InitWithName(const nsID& id, const char* nameString) {
  MOZ_ASSERT(nameString, "no name");
  Reset();
  mID = id;
  SetName(nameString);
}

// try to use the name, if no name, then use the number
NS_IMETHODIMP
nsJSID::ToString(char** _retval) {
  if (mName && mName != gNoString) {
    return GetName(_retval);
  }

  return GetNumber(_retval);
}

const nsID& nsJSID::GetInvalidIID() const {
  // {BB1F47B0-D137-11d2-9841-006008962422}
  static const nsID invalid = {0xbb1f47b0,
                               0xd137,
                               0x11d2,
                               {0x98, 0x41, 0x0, 0x60, 0x8, 0x96, 0x24, 0x22}};
  return invalid;
}

// static
already_AddRefed<nsJSID> nsJSID::NewID(const char* str) {
  if (!str) {
    NS_ERROR("no string");
    return nullptr;
  }

  RefPtr<nsJSID> idObj = new nsJSID();
  NS_ENSURE_SUCCESS(idObj->Initialize(str), nullptr);
  return idObj.forget();
}

// static
already_AddRefed<nsJSID> nsJSID::NewID(const nsID& id) {
  RefPtr<nsJSID> idObj = new nsJSID();
  idObj->mID = id;
  idObj->mName = nullptr;
  idObj->mNumber = nullptr;
  return idObj.forget();
}

/***************************************************************************/
// Class object support so that we can share prototypes of wrapper

// This class exists just so we can have a shared scriptable helper for
// the nsJSIID class. The instances implement their own helpers. But we
// needed to be able to indicate to the shared prototypes this single flag:
// XPC_SCRIPTABLE_DONT_ENUM_STATIC_PROPS. And having a class to do it is
// the only means we have. Setting this flag on any given instance scriptable
// helper is not sufficient to convey the information that we don't want
// static properties enumerated on the shared proto.

class SharedScriptableHelperForJSIID final : public nsIXPCScriptable {
  ~SharedScriptableHelperForJSIID() {}

 public:
  NS_DECL_ISUPPORTS
  NS_DECL_NSIXPCSCRIPTABLE
  SharedScriptableHelperForJSIID() {}
};

NS_IMPL_ISUPPORTS(SharedScriptableHelperForJSIID, nsIXPCScriptable)

// The nsIXPCScriptable map declaration that will generate stubs for us...
#define XPC_MAP_CLASSNAME SharedScriptableHelperForJSIID
#define XPC_MAP_QUOTED_CLASSNAME "JSIID"
#define XPC_MAP_FLAGS XPC_SCRIPTABLE_ALLOW_PROP_MODS_DURING_RESOLVE
#include "xpc_map_end.h" /* This will #undef the above */

static mozilla::StaticRefPtr<nsIXPCScriptable> gSharedScriptableHelperForJSIID;
static bool gClassObjectsWereInited = false;

static void EnsureClassObjectsInitialized() {
  if (!gClassObjectsWereInited) {
    gSharedScriptableHelperForJSIID = new SharedScriptableHelperForJSIID();

    gClassObjectsWereInited = true;
  }
}

static nsresult GetSharedScriptableHelperForJSIID(nsIXPCScriptable** helper) {
  EnsureClassObjectsInitialized();
  nsCOMPtr<nsIXPCScriptable> temp = gSharedScriptableHelperForJSIID.get();
  temp.forget(helper);
  return NS_OK;
}

/******************************************************/

#define NULL_CID                                     \
  {                                                  \
    0x00000000, 0x0000, 0x0000, {                    \
      0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 \
    }                                                \
  }

NS_DECL_CI_INTERFACE_GETTER(nsJSIID)
NS_IMPL_CLASSINFO(nsJSIID, GetSharedScriptableHelperForJSIID, 0, NULL_CID)

NS_DECL_CI_INTERFACE_GETTER(nsJSCID)
NS_IMPL_CLASSINFO(nsJSCID, nullptr, 0, NULL_CID)

void xpc_DestroyJSxIDClassObjects() {
  if (gClassObjectsWereInited) {
    NS_IF_RELEASE(NS_CLASSINFO_NAME(nsJSIID));
    NS_IF_RELEASE(NS_CLASSINFO_NAME(nsJSCID));
    gSharedScriptableHelperForJSIID = nullptr;

    gClassObjectsWereInited = false;
  }
}

/***************************************************************************/

NS_IMPL_QUERY_INTERFACE_CI(nsJSIID, nsIJSID, nsIJSIID, nsIXPCScriptable)

NS_IMPL_ADDREF(nsJSIID)
NS_IMPL_RELEASE(nsJSIID)
NS_IMPL_CI_INTERFACE_GETTER(nsJSIID, nsIJSID, nsIJSIID)

// The nsIXPCScriptable map declaration that will generate stubs for us...
#define XPC_MAP_CLASSNAME nsJSIID
#define XPC_MAP_QUOTED_CLASSNAME "nsJSIID"
#define XPC_MAP_FLAGS                                            \
  (XPC_SCRIPTABLE_WANT_RESOLVE | XPC_SCRIPTABLE_WANT_ENUMERATE | \
   XPC_SCRIPTABLE_WANT_HASINSTANCE |                             \
   XPC_SCRIPTABLE_ALLOW_PROP_MODS_DURING_RESOLVE)
#include "xpc_map_end.h" /* This will #undef the above */

nsJSIID::nsJSIID(const nsXPTInterfaceInfo* aInfo) : mInfo(aInfo) {}

nsJSIID::~nsJSIID() {}

// If mInfo is present we use it and ignore mDetails, else we use mDetails.

NS_IMETHODIMP nsJSIID::GetName(char** aName) {
  *aName = moz_xstrdup(mInfo->Name());
  return NS_OK;
}

NS_IMETHODIMP nsJSIID::GetNumber(char** aNumber) {
  char str[NSID_LENGTH];
  mInfo->IID().ToProvidedString(str);
  *aNumber = (char*)moz_xmemdup(str, NSID_LENGTH);
  return NS_OK;
}

NS_IMETHODIMP_(const nsID*) nsJSIID::GetID() { return &mInfo->IID(); }

NS_IMETHODIMP nsJSIID::GetValid(bool* aValid) {
  *aValid = true;
  return NS_OK;
}

NS_IMETHODIMP nsJSIID::Equals(nsIJSID* other, bool* _retval) {
  if (!_retval) {
    return NS_ERROR_NULL_POINTER;
  }

  if (!other) {
    *_retval = false;
    return NS_OK;
  }

  *_retval = mInfo->IID() == *other->GetID();
  return NS_OK;
}

NS_IMETHODIMP nsJSIID::Initialize(const char* idString) {
  return NS_ERROR_FAILURE;
}

NS_IMETHODIMP nsJSIID::ToString(char** _retval) {
  *_retval = moz_xstrdup(mInfo->Name());
  return NS_OK;
}

// static
already_AddRefed<nsJSIID> nsJSIID::NewID(const nsXPTInterfaceInfo* aInfo) {
  if (!aInfo) {
    NS_ERROR("no info");
    return nullptr;
  }

  RefPtr<nsJSIID> idObj = new nsJSIID(aInfo);
  return idObj.forget();
}
||||||| merged common ancestors
NS_IMPL_CLASSINFO(nsJSID, nullptr, 0, NS_JS_ID_CID)
NS_IMPL_ISUPPORTS_CI(nsJSID, nsIJSID)

const char nsJSID::gNoString[] = "";

nsJSID::nsJSID()
    : mID(GetInvalidIID()),
      mNumber(const_cast<char*>(gNoString)),
      mName(const_cast<char*>(gNoString))
{
}

nsJSID::~nsJSID()
{
    if (mNumber && mNumber != gNoString) {
        free(mNumber);
    }
    if (mName && mName != gNoString) {
        free(mName);
    }
}

void nsJSID::Reset()
{
    mID = GetInvalidIID();

    if (mNumber && mNumber != gNoString) {
        free(mNumber);
    }
    if (mName && mName != gNoString) {
        free(mName);
    }

    mNumber = mName = nullptr;
}

void
nsJSID::SetName(const char* name)
{
    MOZ_ASSERT(!mName || mName == gNoString, "name already set");
    MOZ_ASSERT(name, "null name");
    mName = NS_xstrdup(name);
}

NS_IMETHODIMP
nsJSID::GetName(char * *aName)
{
    if (!aName) {
        return NS_ERROR_NULL_POINTER;
    }

    if (!NameIsSet()) {
        SetNameToNoString();
    }
    MOZ_ASSERT(mName, "name not set");
    *aName = NS_xstrdup(mName);
    return NS_OK;
}

NS_IMETHODIMP
nsJSID::GetNumber(char * *aNumber)
{
    if (!aNumber) {
        return NS_ERROR_NULL_POINTER;
    }

    if (!mNumber) {
        if (!(mNumber = mID.ToString())) {
            mNumber = const_cast<char*>(gNoString);
        }
    }

    *aNumber = NS_xstrdup(mNumber);
    return NS_OK;
}

NS_IMETHODIMP_(const nsID*)
nsJSID::GetID()
{
    return &mID;
}

NS_IMETHODIMP
nsJSID::GetValid(bool* aValid)
{
    if (!aValid) {
        return NS_ERROR_NULL_POINTER;
    }

    *aValid = IsValid();
    return NS_OK;
}

NS_IMETHODIMP
nsJSID::Equals(nsIJSID* other, bool* _retval)
{
    if (!_retval) {
        return NS_ERROR_NULL_POINTER;
    }

    if (!other || mID.Equals(GetInvalidIID())) {
        *_retval = false;
        return NS_OK;
    }

    *_retval = other->GetID()->Equals(mID);
    return NS_OK;
}

NS_IMETHODIMP
nsJSID::Initialize(const char* idString)
{
    if (!idString) {
        return NS_ERROR_NULL_POINTER;
    }

    if (*idString != '\0' && mID.Equals(GetInvalidIID())) {
        Reset();

        if (idString[0] == '{') {
            if (mID.Parse(idString)) {
                return NS_OK;
            }

            // error - reset to invalid state
            mID = GetInvalidIID();
        }
    }
    return NS_ERROR_FAILURE;
}

void
nsJSID::InitWithName(const nsID& id, const char* nameString)
{
    MOZ_ASSERT(nameString, "no name");
    Reset();
    mID = id;
    SetName(nameString);
}

// try to use the name, if no name, then use the number
NS_IMETHODIMP
nsJSID::ToString(char** _retval)
{
    if (mName && mName != gNoString) {
        return GetName(_retval);
    }

    return GetNumber(_retval);
}

const nsID&
nsJSID::GetInvalidIID() const
{
    // {BB1F47B0-D137-11d2-9841-006008962422}
    static const nsID invalid = {0xbb1f47b0, 0xd137, 0x11d2,
                                  {0x98, 0x41, 0x0, 0x60, 0x8, 0x96, 0x24, 0x22}};
    return invalid;
}

//static
already_AddRefed<nsJSID>
nsJSID::NewID(const char* str)
{
    if (!str) {
        NS_ERROR("no string");
        return nullptr;
    }

    RefPtr<nsJSID> idObj = new nsJSID();
    NS_ENSURE_SUCCESS(idObj->Initialize(str), nullptr);
    return idObj.forget();
}

//static
already_AddRefed<nsJSID>
nsJSID::NewID(const nsID& id)
{
    RefPtr<nsJSID> idObj = new nsJSID();
    idObj->mID = id;
    idObj->mName = nullptr;
    idObj->mNumber = nullptr;
    return idObj.forget();
}


/***************************************************************************/
// Class object support so that we can share prototypes of wrapper

// This class exists just so we can have a shared scriptable helper for
// the nsJSIID class. The instances implement their own helpers. But we
// needed to be able to indicate to the shared prototypes this single flag:
// XPC_SCRIPTABLE_DONT_ENUM_STATIC_PROPS. And having a class to do it is
// the only means we have. Setting this flag on any given instance scriptable
// helper is not sufficient to convey the information that we don't want
// static properties enumerated on the shared proto.

class SharedScriptableHelperForJSIID final : public nsIXPCScriptable
{
    ~SharedScriptableHelperForJSIID() {}
public:
    NS_DECL_ISUPPORTS
    NS_DECL_NSIXPCSCRIPTABLE
    SharedScriptableHelperForJSIID() {}
};

NS_IMPL_ISUPPORTS(SharedScriptableHelperForJSIID,
                  nsIXPCScriptable)

// The nsIXPCScriptable map declaration that will generate stubs for us...
#define XPC_MAP_CLASSNAME SharedScriptableHelperForJSIID
#define XPC_MAP_QUOTED_CLASSNAME "JSIID"
#define XPC_MAP_FLAGS XPC_SCRIPTABLE_ALLOW_PROP_MODS_DURING_RESOLVE
#include "xpc_map_end.h" /* This will #undef the above */

static mozilla::StaticRefPtr<nsIXPCScriptable> gSharedScriptableHelperForJSIID;
static bool gClassObjectsWereInited = false;

static void EnsureClassObjectsInitialized()
{
    if (!gClassObjectsWereInited) {
        gSharedScriptableHelperForJSIID = new SharedScriptableHelperForJSIID();

        gClassObjectsWereInited = true;
    }
}

static nsresult GetSharedScriptableHelperForJSIID(nsIXPCScriptable** helper)
{
    EnsureClassObjectsInitialized();
    nsCOMPtr<nsIXPCScriptable> temp = gSharedScriptableHelperForJSIID.get();
    temp.forget(helper);
    return NS_OK;
}

/******************************************************/

#define NULL_CID                                                              \
{ 0x00000000, 0x0000, 0x0000,                                                 \
  { 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 } }

NS_DECL_CI_INTERFACE_GETTER(nsJSIID)
NS_IMPL_CLASSINFO(nsJSIID, GetSharedScriptableHelperForJSIID, 0, NULL_CID)

NS_DECL_CI_INTERFACE_GETTER(nsJSCID)
NS_IMPL_CLASSINFO(nsJSCID, nullptr, 0, NULL_CID)

void xpc_DestroyJSxIDClassObjects()
{
    if (gClassObjectsWereInited) {
        NS_IF_RELEASE(NS_CLASSINFO_NAME(nsJSIID));
        NS_IF_RELEASE(NS_CLASSINFO_NAME(nsJSCID));
        gSharedScriptableHelperForJSIID = nullptr;

        gClassObjectsWereInited = false;
    }
}

/***************************************************************************/

NS_IMPL_QUERY_INTERFACE_CI(nsJSIID,
                           nsIJSID,
                           nsIJSIID,
                           nsIXPCScriptable)

NS_IMPL_ADDREF(nsJSIID)
NS_IMPL_RELEASE(nsJSIID)
NS_IMPL_CI_INTERFACE_GETTER(nsJSIID, nsIJSID, nsIJSIID)

// The nsIXPCScriptable map declaration that will generate stubs for us...
#define XPC_MAP_CLASSNAME         nsJSIID
#define XPC_MAP_QUOTED_CLASSNAME "nsJSIID"
#define XPC_MAP_FLAGS (XPC_SCRIPTABLE_WANT_RESOLVE | \
                       XPC_SCRIPTABLE_WANT_ENUMERATE | \
                       XPC_SCRIPTABLE_WANT_HASINSTANCE | \
                       XPC_SCRIPTABLE_ALLOW_PROP_MODS_DURING_RESOLVE)
#include "xpc_map_end.h" /* This will #undef the above */


nsJSIID::nsJSIID(const nsXPTInterfaceInfo* aInfo)
    : mInfo(aInfo)
{
}

nsJSIID::~nsJSIID() {}

// If mInfo is present we use it and ignore mDetails, else we use mDetails.

NS_IMETHODIMP nsJSIID::GetName(char * *aName)
{
    return mInfo->GetName(aName);
}

NS_IMETHODIMP nsJSIID::GetNumber(char * *aNumber)
{
    char str[NSID_LENGTH];
    const nsIID* id;
    mInfo->GetIIDShared(&id);
    id->ToProvidedString(str);
    *aNumber = (char*) moz_xmemdup(str, NSID_LENGTH);
    return NS_OK;
}

NS_IMETHODIMP_(const nsID*) nsJSIID::GetID()
{
    const nsIID* id;
    mInfo->GetIIDShared(&id);
    return id;
}

NS_IMETHODIMP nsJSIID::GetValid(bool* aValid)
{
    *aValid = true;
    return NS_OK;
}

NS_IMETHODIMP nsJSIID::Equals(nsIJSID* other, bool* _retval)
{
    if (!_retval) {
        return NS_ERROR_NULL_POINTER;
    }

    if (!other) {
        *_retval = false;
        return NS_OK;
    }

    mInfo->IsIID(other->GetID(), _retval);
    return NS_OK;
}

NS_IMETHODIMP nsJSIID::Initialize(const char* idString)
{
    return NS_ERROR_FAILURE;
}

NS_IMETHODIMP nsJSIID::ToString(char** _retval)
{
    return mInfo->GetName(_retval);
}

// static
already_AddRefed<nsJSIID>
nsJSIID::NewID(const nsXPTInterfaceInfo* aInfo)
{
    if (!aInfo) {
        NS_ERROR("no info");
        return nullptr;
    }

    if (!aInfo->IsScriptable()) {
        return nullptr;
    }

    RefPtr<nsJSIID> idObj = new nsJSIID(aInfo);
    return idObj.forget();
}
=======
/******************************************************************************
 * # Generic IDs #
 *
 * Generic IDs are the type of JS object created by most code which passes nsID
 * objects to JavaScript code. They provide no special methods, and only store
 * the raw nsID value.
 *
 * The nsID value is stored in 4 reserved slots, with 32 bits of the 128-bit
 * value stored in each slot. Getter code extracts this data, and combines them
 * back into the nsID value.
 */
static bool ID_Equals(JSContext* aCx, unsigned aArgc, Value* aVp);
static bool ID_GetNumber(JSContext* aCx, unsigned aArgc, Value* aVp);

// Generic ID objects contain 4 reserved slots, each containing a uint32_t with
// 1/4 of the representation of the nsID value. This allows us to avoid an extra
// allocation for the nsID object, and eliminates the need for a finalizer.
enum { kID_Slot0, kID_Slot1, kID_Slot2, kID_Slot3, kID_SlotCount };
static const js::Class sID_Class = {
    "nsJSID", JSCLASS_HAS_RESERVED_SLOTS(kID_SlotCount), JS_NULL_CLASS_OPS};

/******************************************************************************
 * # Interface IDs #
 *
 * In addition to the properties exposed by Generic ID objects, IID supports
 * 'instanceof', exposes constant properties defined on the class, and exposes
 * the interface name as the 'name' and 'toString()' values.
 */
static bool IID_HasInstance(JSContext* aCx, unsigned aArgc, Value* aVp);
static bool IID_GetName(JSContext* aCx, unsigned aArgc, Value* aVp);
>>>>>>> upstream-releases

<<<<<<< HEAD
NS_IMETHODIMP
nsJSIID::Resolve(nsIXPConnectWrappedNative* wrapper, JSContext* cx,
                 JSObject* objArg, jsid idArg, bool* resolvedp, bool* _retval) {
  RootedObject obj(cx, objArg);
  RootedId id(cx, idArg);
  XPCCallContext ccx(cx);
||||||| merged common ancestors

NS_IMETHODIMP
nsJSIID::Resolve(nsIXPConnectWrappedNative* wrapper,
                 JSContext * cx, JSObject * objArg,
                 jsid idArg, bool* resolvedp,
                 bool* _retval)
{
    RootedObject obj(cx, objArg);
    RootedId id(cx, idArg);
    XPCCallContext ccx(cx);

    RefPtr<XPCNativeInterface> iface =
        XPCNativeInterface::GetNewOrUsed(mInfo);

    if (!iface) {
        return NS_OK;
    }
=======
// Interface ID objects use a single reserved slot containing a pointer to the
// nsXPTInterfaceInfo object for the interface in question.
enum { kIID_InfoSlot, kIID_SlotCount };
static const js::Class sIID_Class = {
    "nsJSIID", JSCLASS_HAS_RESERVED_SLOTS(kIID_SlotCount), JS_NULL_CLASS_OPS};

/******************************************************************************
 * # Contract IDs #
 *
 * In addition to the properties exposed by Generic ID objects, Contract IDs
 * expose 'getService' and 'createInstance' methods, and expose the contractID
 * string as '.name' and '.toString()'.
 */
static bool CID_CreateInstance(JSContext* aCx, unsigned aArgc, Value* aVp);
static bool CID_GetService(JSContext* aCx, unsigned aArgc, Value* aVp);
static bool CID_GetName(JSContext* aCx, unsigned aArgc, Value* aVp);

// ContractID objects use a single reserved slot, containing the ContractID. The
// nsCID value for this object is looked up when the object is being unwrapped.
enum { kCID_ContractSlot, kCID_SlotCount };
static const js::Class sCID_Class = {
    "nsJSCID", JSCLASS_HAS_RESERVED_SLOTS(kCID_SlotCount), JS_NULL_CLASS_OPS};

/**
 * Ensure that the nsID prototype objects have been created for the current
 * global, and extract the prototype values.
 */
static JSObject* GetIDPrototype(JSContext* aCx, const js::Class* aClass) {
  XPCWrappedNativeScope* scope = ObjectScope(CurrentGlobalOrNull(aCx));
  if (NS_WARN_IF(!scope)) {
    return nullptr;
  }

  // Create prototype objects for the JSID objects if they haven't been
  // created for this scope yet.
  if (!scope->mIDProto) {
    MOZ_ASSERT(!scope->mIIDProto && !scope->mCIDProto);

    RootedObject idProto(aCx, JS_NewPlainObject(aCx));
    RootedObject iidProto(aCx,
                          JS_NewObjectWithGivenProto(aCx, nullptr, idProto));
    RootedObject cidProto(aCx,
                          JS_NewObjectWithGivenProto(aCx, nullptr, idProto));
    RootedId hasInstance(
        aCx, SYMBOL_TO_JSID(GetWellKnownSymbol(aCx, SymbolCode::hasInstance)));

    const uint32_t kFlags =
        JSPROP_READONLY | JSPROP_ENUMERATE | JSPROP_PERMANENT;
    const uint32_t kNoEnum = JSPROP_READONLY | JSPROP_PERMANENT;

    bool ok =
        idProto && iidProto && cidProto &&
        // Methods and properties on all ID Objects:
        JS_DefineFunction(aCx, idProto, "equals", ID_Equals, 1, kFlags) &&
        JS_DefineProperty(aCx, idProto, "number", ID_GetNumber, nullptr,
                          kFlags) &&

        // Methods for IfaceID objects, which also inherit ID properties:
        JS_DefineFunctionById(aCx, iidProto, hasInstance, IID_HasInstance, 1,
                              kNoEnum) &&
        JS_DefineProperty(aCx, iidProto, "name", IID_GetName, nullptr,
                          kFlags) &&

        // Methods for ContractID objects, which also inherit ID properties:
        JS_DefineFunction(aCx, cidProto, "createInstance", CID_CreateInstance,
                          1, kFlags) &&
        JS_DefineFunction(aCx, cidProto, "getService", CID_GetService, 1,
                          kFlags) &&
        JS_DefineProperty(aCx, cidProto, "name", CID_GetName, nullptr,
                          kFlags) &&

        // ToString returns '.number' on generic IDs, while returning
        // '.name' on other ID types.
        JS_DefineFunction(aCx, idProto, "toString", ID_GetNumber, 0, kFlags) &&
        JS_DefineFunction(aCx, iidProto, "toString", IID_GetName, 0, kFlags) &&
        JS_DefineFunction(aCx, cidProto, "toString", CID_GetName, 0, kFlags);
    if (!ok) {
      return nullptr;
    }

    scope->mIDProto = idProto;
    scope->mIIDProto = iidProto;
    scope->mCIDProto = cidProto;
  }

  if (aClass == &sID_Class) {
    return scope->mIDProto;
  } else if (aClass == &sIID_Class) {
    return scope->mIIDProto;
  } else if (aClass == &sCID_Class) {
    return scope->mCIDProto;
  }

  MOZ_CRASH("Unrecognized ID Object Class");
}

// Unwrap the given value to an object with the correct class, or nullptr.
static JSObject* GetIDObject(HandleValue aVal, const Class* aClass) {
  if (aVal.isObject()) {
    // We care only about IID/CID objects here, so CheckedUnwrapStatic is fine.
    JSObject* obj = js::CheckedUnwrapStatic(&aVal.toObject());
    if (obj && js::GetObjectClass(obj) == aClass) {
      return obj;
    }
  }
  return nullptr;
}

/**
 * Unwrap an nsID object from a JSValue.
 *
 * For Generic ID objects, this function will extract the nsID from reserved
 * slots. For IfaceID objects, it will be extracted from the nsXPTInterfaceInfo,
 * and for ContractID objects, the ContractID's corresponding CID will be looked
 * up.
 */
Maybe<nsID> JSValue2ID(JSContext* aCx, HandleValue aVal) {
  if (!aVal.isObject()) {
    return Nothing();
  }

  // We only care about ID objects here, so CheckedUnwrapStatic is fine.
  RootedObject obj(aCx, js::CheckedUnwrapStatic(&aVal.toObject()));
  if (!obj) {
    return Nothing();
  }

  mozilla::Maybe<nsID> id;
  if (js::GetObjectClass(obj) == &sID_Class) {
    // Extract the raw bytes of the nsID from reserved slots.
    uint32_t rawid[] = {js::GetReservedSlot(obj, kID_Slot0).toPrivateUint32(),
                        js::GetReservedSlot(obj, kID_Slot1).toPrivateUint32(),
                        js::GetReservedSlot(obj, kID_Slot2).toPrivateUint32(),
                        js::GetReservedSlot(obj, kID_Slot3).toPrivateUint32()};

    // Construct a nsID inside the Maybe, and copy the rawid into it.
    id.emplace();
    memcpy(id.ptr(), &rawid, sizeof(nsID));
  } else if (js::GetObjectClass(obj) == &sIID_Class) {
    // IfaceID objects store a nsXPTInterfaceInfo* pointer.
    auto* info = static_cast<const nsXPTInterfaceInfo*>(
        js::GetReservedSlot(obj, kIID_InfoSlot).toPrivate());
    id.emplace(info->IID());
  } else if (js::GetObjectClass(obj) == &sCID_Class) {
    // ContractID objects store a ContractID string.
    JS::UniqueChars contractId = JS_EncodeStringToLatin1(
        aCx, js::GetReservedSlot(obj, kCID_ContractSlot).toString());

    // NOTE(nika): If we directly access the nsComponentManager, we can do
    // this with a more-basic pointer lookup:
    //     nsFactoryEntry* entry = nsComponentManagerImpl::gComponentManager->
    //         GetFactoryEntry(contractId.ptr(), contractId.length());
    //     if (entry) id.emplace(entry->mCIDEntry->cid);

    nsCOMPtr<nsIComponentRegistrar> registrar;
    nsresult rv = NS_GetComponentRegistrar(getter_AddRefs(registrar));
    if (NS_FAILED(rv) || !registrar) {
      return Nothing();
    }

    nsCID* cid = nullptr;
    if (NS_SUCCEEDED(registrar->ContractIDToCID(contractId.get(), &cid))) {
      id.emplace(*cid);
      free(cid);
    }
  }
  return id;
}

/**
 * Public ID Object Constructor Methods
 */
static JSObject* NewIDObjectHelper(JSContext* aCx, const js::Class* aClass) {
  RootedObject proto(aCx, GetIDPrototype(aCx, aClass));
  if (proto) {
    return JS_NewObjectWithGivenProto(aCx, Jsvalify(aClass), proto);
  }
  return nullptr;
}

bool ID2JSValue(JSContext* aCx, const nsID& aId, MutableHandleValue aVal) {
  RootedObject obj(aCx, NewIDObjectHelper(aCx, &sID_Class));
  if (!obj) {
    return false;
  }

  // Get the data in nsID as 4 uint32_ts, and store them in slots.
  uint32_t rawid[4];
  memcpy(&rawid, &aId, sizeof(nsID));
  static_assert(sizeof(nsID) == sizeof(rawid), "Wrong size of nsID");
  js::SetReservedSlot(obj, kID_Slot0, PrivateUint32Value(rawid[0]));
  js::SetReservedSlot(obj, kID_Slot1, PrivateUint32Value(rawid[1]));
  js::SetReservedSlot(obj, kID_Slot2, PrivateUint32Value(rawid[2]));
  js::SetReservedSlot(obj, kID_Slot3, PrivateUint32Value(rawid[3]));

  aVal.setObject(*obj);
  return true;
}

bool IfaceID2JSValue(JSContext* aCx, const nsXPTInterfaceInfo& aInfo,
                     MutableHandleValue aVal) {
  RootedObject obj(aCx, NewIDObjectHelper(aCx, &sIID_Class));
  if (!obj) {
    return false;
  }

  // Define any constants defined on the interface on the ID object.
  //
  // NOTE: When InterfaceIDs were implemented using nsIXPCScriptable and
  // XPConnect, this was implemented using a 'resolve' hook. It has been
  // changed to happen at creation-time as most interfaces shouldn't have many
  // constants, and this is likely to turn out cheaper.
  RootedValue constant(aCx);
  for (uint16_t i = 0; i < aInfo.ConstantCount(); ++i) {
    constant.set(aInfo.Constant(i).JSValue());
    if (!JS_DefineProperty(
            aCx, obj, aInfo.Constant(i).Name(), constant,
            JSPROP_READONLY | JSPROP_ENUMERATE | JSPROP_PERMANENT)) {
      return false;
    }
  }

  // The InterfaceInfo is stored in a reserved slot.
  js::SetReservedSlot(obj, kIID_InfoSlot, PrivateValue((void*)&aInfo));
  aVal.setObject(*obj);
  return true;
}

bool ContractID2JSValue(JSContext* aCx, JSString* aContract,
                        MutableHandleValue aVal) {
  RootedString jsContract(aCx, aContract);

  {
    // It is perfectly safe to have a ContractID object with an invalid
    // ContractID, but is usually a bug.
    nsCOMPtr<nsIComponentRegistrar> registrar;
    NS_GetComponentRegistrar(getter_AddRefs(registrar));
    if (!registrar) {
      return false;
    }

    bool registered = false;
    JS::UniqueChars contract = JS_EncodeStringToLatin1(aCx, jsContract);
    registrar->IsContractIDRegistered(contract.get(), &registered);
    if (!registered) {
      return false;
    }
  }

  RootedObject obj(aCx, NewIDObjectHelper(aCx, &sCID_Class));
  if (!obj) {
    return false;
  }

  // The Contract is stored in a reserved slot.
  js::SetReservedSlot(obj, kCID_ContractSlot, StringValue(jsContract));
  aVal.setObject(*obj);
  return true;
}

/******************************************************************************
 * # Method & Property Getter Implementations #
 */

// NOTE: This method is used both for 'get ID.prototype.number' and
// 'ID.prototype.toString'.
static bool ID_GetNumber(JSContext* aCx, unsigned aArgc, Value* aVp) {
  CallArgs args = CallArgsFromVp(aArgc, aVp);

  Maybe<nsID> id = JSValue2ID(aCx, args.thisv());
  if (!id) {
    return Throw(aCx, NS_ERROR_XPC_BAD_CONVERT_JS);
  }
>>>>>>> upstream-releases

<<<<<<< HEAD
  RefPtr<XPCNativeInterface> iface = XPCNativeInterface::GetNewOrUsed(mInfo);

  if (!iface) {
    return NS_OK;
  }

  XPCNativeMember* member = iface->FindMember(id);
  if (member && member->IsConstant()) {
    RootedValue val(cx);
    if (!member->GetConstantValue(ccx, iface, val.address())) {
      return NS_ERROR_OUT_OF_MEMORY;
    }
||||||| merged common ancestors
    XPCNativeMember* member = iface->FindMember(id);
    if (member && member->IsConstant()) {
        RootedValue val(cx);
        if (!member->GetConstantValue(ccx, iface, val.address())) {
            return NS_ERROR_OUT_OF_MEMORY;
        }

        *resolvedp = true;
        *_retval = JS_DefinePropertyById(cx, obj, id, val,
                                         JSPROP_ENUMERATE | JSPROP_READONLY |
                                         JSPROP_PERMANENT | JSPROP_RESOLVING);
    }
=======
  char buf[NSID_LENGTH];
  id->ToProvidedString(buf);
  JSString* jsnum = JS_NewStringCopyZ(aCx, buf);
  if (!jsnum) {
    return Throw(aCx, NS_ERROR_OUT_OF_MEMORY);
  }
>>>>>>> upstream-releases

<<<<<<< HEAD
    *resolvedp = true;
    *_retval = JS_DefinePropertyById(cx, obj, id, val,
                                     JSPROP_ENUMERATE | JSPROP_READONLY |
                                         JSPROP_PERMANENT | JSPROP_RESOLVING);
  }

  return NS_OK;
||||||| merged common ancestors
    return NS_OK;
=======
  args.rval().setString(jsnum);
  return true;
>>>>>>> upstream-releases
}

<<<<<<< HEAD
NS_IMETHODIMP
nsJSIID::Enumerate(nsIXPConnectWrappedNative* wrapper, JSContext* cx,
                   JSObject* objArg, bool* _retval) {
  // In this case, let's just eagerly resolve...

  RootedObject obj(cx, objArg);
  XPCCallContext ccx(cx);

  RefPtr<XPCNativeInterface> iface = XPCNativeInterface::GetNewOrUsed(mInfo);
||||||| merged common ancestors
NS_IMETHODIMP
nsJSIID::Enumerate(nsIXPConnectWrappedNative* wrapper,
                   JSContext * cx, JSObject * objArg, bool* _retval)
{
    // In this case, let's just eagerly resolve...

    RootedObject obj(cx, objArg);
    XPCCallContext ccx(cx);

    RefPtr<XPCNativeInterface> iface =
        XPCNativeInterface::GetNewOrUsed(mInfo);
=======
static bool ID_Equals(JSContext* aCx, unsigned aArgc, Value* aVp) {
  CallArgs args = CallArgsFromVp(aArgc, aVp);
  if (!args.requireAtLeast(aCx, "nsID.equals", 1)) {
    return false;
  }
>>>>>>> upstream-releases

<<<<<<< HEAD
  if (!iface) {
    return NS_OK;
  }
||||||| merged common ancestors
    if (!iface) {
        return NS_OK;
    }
=======
  Maybe<nsID> id = JSValue2ID(aCx, args.thisv());
  Maybe<nsID> id2 = JSValue2ID(aCx, args[0]);
  if (!id || !id2) {
    return Throw(aCx, NS_ERROR_XPC_BAD_CONVERT_JS);
  }
>>>>>>> upstream-releases

<<<<<<< HEAD
  uint16_t count = iface->GetMemberCount();
  for (uint16_t i = 0; i < count; i++) {
    XPCNativeMember* member = iface->GetMemberAt(i);
    if (member && member->IsConstant() &&
        !xpc_ForcePropertyResolve(cx, obj, member->GetName())) {
      return NS_ERROR_UNEXPECTED;
    }
  }
  return NS_OK;
||||||| merged common ancestors
    uint16_t count = iface->GetMemberCount();
    for (uint16_t i = 0; i < count; i++) {
        XPCNativeMember* member = iface->GetMemberAt(i);
        if (member && member->IsConstant() &&
            !xpc_ForcePropertyResolve(cx, obj, member->GetName())) {
            return NS_ERROR_UNEXPECTED;
        }
    }
    return NS_OK;
=======
  args.rval().setBoolean(id->Equals(*id2));
  return true;
>>>>>>> upstream-releases
}

/*
 * HasInstance hooks need to find an appropriate reflector in order to function
 * properly. There are two complexities that we need to handle:
 *
 * 1 - Cross-compartment wrappers. Chrome uses over 100 compartments, all with
 *     system principal. The success of an instanceof check should not depend
 *     on which compartment an object comes from. At the same time, we want to
 *     make sure we don't unwrap important security wrappers.
 *     CheckedUnwrap does the right thing here.
 *
 * 2 - Prototype chains. Suppose someone creates a vanilla JS object |a| and
 *     sets its __proto__ to some WN |b|. If |b instanceof nsIFoo| returns true,
 *     one would expect |a instanceof nsIFoo| to return true as well, since
 *     instanceof is transitive up the prototype chain in ECMAScript. Moreover,
 *     there's chrome code that relies on this.
 *
 * This static method handles both complexities, returning either an XPCWN, a
 * DOM object, or null. The object may well be cross-compartment from |cx|.
 */
<<<<<<< HEAD
static nsresult FindObjectForHasInstance(JSContext* cx, HandleObject objArg,
                                         MutableHandleObject target) {
  using namespace mozilla::jsipc;
  RootedObject obj(cx, objArg), proto(cx);
  while (true) {
    // Try the object, or the wrappee if allowed.
    JSObject* o = js::IsWrapper(obj) ? js::CheckedUnwrap(obj, false) : obj;
    if (o && (IS_WN_REFLECTOR(o) || IsDOMObject(o) || IsCPOW(o))) {
      target.set(o);
      return NS_OK;
    }

    // Walk the prototype chain from the perspective of the callee (i.e.
    // respecting Xrays if they exist).
    if (!js::GetObjectProto(cx, obj, &proto)) {
      return NS_ERROR_FAILURE;
    }
    if (!proto) {
      target.set(nullptr);
      return NS_OK;
    }
    obj = proto;
  }
}

nsresult xpc::HasInstance(JSContext* cx, HandleObject objArg, const nsID* iid,
                          bool* bp) {
  *bp = false;

  RootedObject obj(cx);
  nsresult rv = FindObjectForHasInstance(cx, objArg, &obj);
  if (NS_WARN_IF(NS_FAILED(rv))) {
    return rv;
  }

  if (!obj) {
    return NS_OK;
  }

  if (mozilla::jsipc::IsCPOW(obj)) {
    return mozilla::jsipc::InstanceOf(obj, iid, bp);
  }

  nsCOMPtr<nsISupports> identity = UnwrapReflectorToISupports(obj);
  if (!identity) {
    return NS_OK;
  }

  nsCOMPtr<nsISupports> supp;
  identity->QueryInterface(*iid, getter_AddRefs(supp));
  *bp = supp;

  // Our old HasInstance implementation operated by invoking FindTearOff on
  // XPCWrappedNatives, and various bits of chrome JS came to depend on
  // |instanceof| doing an implicit QI if it succeeds. Do a drive-by QI to
  // preserve that behavior. This is just a compatibility hack, so we don't
  // really care if it fails.
  if (IS_WN_REFLECTOR(obj)) {
    (void)XPCWrappedNative::Get(obj)->FindTearOff(*iid);
  }

  return NS_OK;
}

NS_IMETHODIMP
nsJSIID::HasInstance(nsIXPConnectWrappedNative* wrapper, JSContext* cx,
                     JSObject* /* unused */, HandleValue val, bool* bp,
                     bool* _retval) {
  *bp = false;
||||||| merged common ancestors
static nsresult
FindObjectForHasInstance(JSContext* cx, HandleObject objArg, MutableHandleObject target)
{
    using namespace mozilla::jsipc;
    RootedObject obj(cx, objArg), proto(cx);
    while (true) {
        // Try the object, or the wrappee if allowed.
        JSObject* o = js::IsWrapper(obj) ? js::CheckedUnwrap(obj, false) : obj;
        if (o && (IS_WN_REFLECTOR(o) || IsDOMObject(o) || IsCPOW(o))) {
            target.set(o);
            return NS_OK;
        }

        // Walk the prototype chain from the perspective of the callee (i.e.
        // respecting Xrays if they exist).
        if (!js::GetObjectProto(cx, obj, &proto)) {
            return NS_ERROR_FAILURE;
        }
        if (!proto) {
            target.set(nullptr);
            return NS_OK;
        }
        obj = proto;
    }
}

nsresult
xpc::HasInstance(JSContext* cx, HandleObject objArg, const nsID* iid, bool* bp)
{
    *bp = false;

    RootedObject obj(cx);
    nsresult rv = FindObjectForHasInstance(cx, objArg, &obj);
    if (NS_WARN_IF(NS_FAILED(rv))) {
        return rv;
    }

    if (!obj) {
        return NS_OK;
    }

    if (mozilla::jsipc::IsCPOW(obj)) {
        return mozilla::jsipc::InstanceOf(obj, iid, bp);
    }

    nsCOMPtr<nsISupports> identity = UnwrapReflectorToISupports(obj);
    if (!identity) {
        return NS_OK;
    }

    nsCOMPtr<nsISupports> supp;
    identity->QueryInterface(*iid, getter_AddRefs(supp));
    *bp = supp;

    // Our old HasInstance implementation operated by invoking FindTearOff on
    // XPCWrappedNatives, and various bits of chrome JS came to depend on
    // |instanceof| doing an implicit QI if it succeeds. Do a drive-by QI to
    // preserve that behavior. This is just a compatibility hack, so we don't
    // really care if it fails.
    if (IS_WN_REFLECTOR(obj)) {
        (void) XPCWrappedNative::Get(obj)->FindTearOff(*iid);
    }

    return NS_OK;
}

NS_IMETHODIMP
nsJSIID::HasInstance(nsIXPConnectWrappedNative* wrapper,
                     JSContext* cx, JSObject * /* unused */,
                     HandleValue val, bool* bp, bool* _retval)
{
    *bp = false;
=======
static nsresult FindObjectForHasInstance(JSContext* cx, HandleObject objArg,
                                         MutableHandleObject target) {
  using namespace mozilla::jsipc;
  RootedObject obj(cx, objArg), proto(cx);
  while (true) {
    // Try the object, or the wrappee if allowed.  We want CheckedUnwrapDynamic
    // here, because we might in fact be looking for a Window.  "cx" represents
    // our current global.
    JSObject* o =
        js::IsWrapper(obj) ? js::CheckedUnwrapDynamic(obj, cx, false) : obj;
    if (o && (IS_WN_REFLECTOR(o) || IsDOMObject(o) || IsCPOW(o))) {
      target.set(o);
      return NS_OK;
    }

    // Walk the prototype chain from the perspective of the callee (i.e.
    // respecting Xrays if they exist).
    if (!js::GetObjectProto(cx, obj, &proto)) {
      return NS_ERROR_FAILURE;
    }
    if (!proto) {
      target.set(nullptr);
      return NS_OK;
    }
    obj = proto;
  }
}

nsresult HasInstance(JSContext* cx, HandleObject objArg, const nsID* iid,
                     bool* bp) {
  *bp = false;

  RootedObject obj(cx);
  nsresult rv = FindObjectForHasInstance(cx, objArg, &obj);
  if (NS_WARN_IF(NS_FAILED(rv))) {
    return rv;
  }

  if (!obj) {
    return NS_OK;
  }
>>>>>>> upstream-releases

<<<<<<< HEAD
  if (val.isPrimitive()) {
    return NS_OK;
  }

  // we have a JSObject
  RootedObject obj(cx, &val.toObject());

  const nsIID* iid = &mInfo->IID();
  return xpc::HasInstance(cx, obj, iid, bp);
}
||||||| merged common ancestors
    if (val.isPrimitive()) {
        return NS_OK;
    }

    // we have a JSObject
    RootedObject obj(cx, &val.toObject());

    const nsIID* iid;
    mInfo->GetIIDShared(&iid);
    return xpc::HasInstance(cx, obj, iid, bp);
}
=======
  if (mozilla::jsipc::IsCPOW(obj)) {
    return mozilla::jsipc::InstanceOf(obj, iid, bp);
  }
>>>>>>> upstream-releases

<<<<<<< HEAD
/***************************************************************************/

NS_IMPL_QUERY_INTERFACE_CI(nsJSCID, nsIJSID, nsIJSCID, nsIXPCScriptable)

NS_IMPL_ADDREF(nsJSCID)
NS_IMPL_RELEASE(nsJSCID)
NS_IMPL_CI_INTERFACE_GETTER(nsJSCID, nsIJSID, nsIJSCID)

// The nsIXPCScriptable map declaration that will generate stubs for us...
#define XPC_MAP_CLASSNAME nsJSCID
#define XPC_MAP_QUOTED_CLASSNAME "nsJSCID"
#define XPC_MAP_FLAGS \
  (XPC_SCRIPTABLE_WANT_CONSTRUCT | XPC_SCRIPTABLE_WANT_HASINSTANCE)
#include "xpc_map_end.h" /* This will #undef the above */

nsJSCID::nsJSCID() { mDetails = new nsJSID(); }
nsJSCID::~nsJSCID() {}

NS_IMETHODIMP nsJSCID::GetName(char** aName) {
  ResolveName();
  return mDetails->GetName(aName);
}

NS_IMETHODIMP nsJSCID::GetNumber(char** aNumber) {
  return mDetails->GetNumber(aNumber);
}

NS_IMETHODIMP_(const nsID*) nsJSCID::GetID() { return &mDetails->ID(); }

NS_IMETHODIMP nsJSCID::GetValid(bool* aValid) {
  return mDetails->GetValid(aValid);
}

NS_IMETHODIMP nsJSCID::Equals(nsIJSID* other, bool* _retval) {
  return mDetails->Equals(other, _retval);
}

NS_IMETHODIMP
nsJSCID::Initialize(const char* str) {
  if (str[0] == '{') {
    NS_ENSURE_SUCCESS(mDetails->Initialize(str), NS_ERROR_FAILURE);
  } else {
    nsCOMPtr<nsIComponentRegistrar> registrar;
    NS_GetComponentRegistrar(getter_AddRefs(registrar));
    NS_ENSURE_TRUE(registrar, NS_ERROR_FAILURE);

    nsCID* cid;
    if (NS_FAILED(registrar->ContractIDToCID(str, &cid))) {
      return NS_ERROR_FAILURE;
    }
    mDetails->InitWithName(*cid, str);
    free(cid);
  }
  return NS_OK;
}

NS_IMETHODIMP nsJSCID::ToString(char** _retval) {
  ResolveName();
  return mDetails->ToString(_retval);
}

void nsJSCID::ResolveName() {
  if (!mDetails->NameIsSet()) {
    mDetails->SetNameToNoString();
  }
}

// static
already_AddRefed<nsJSCID> nsJSCID::NewID(const char* str) {
  if (!str) {
    NS_ERROR("no string");
    return nullptr;
  }

  RefPtr<nsJSCID> idObj = new nsJSCID();
  if (NS_FAILED(idObj->Initialize(str))) {
    return nullptr;
  }
  return idObj.forget();
}

static const nsID* GetIIDArg(uint32_t argc, const JS::Value& val,
                             JSContext* cx) {
  const nsID* iid;

  // If an IID was passed in then use it
  if (argc) {
    JSObject* iidobj;
    if (val.isPrimitive() || !(iidobj = val.toObjectOrNull()) ||
        !(iid = xpc_JSObjectToID(cx, iidobj))) {
      return nullptr;
    }
  } else
    iid = &NS_GET_IID(nsISupports);

  return iid;
}

NS_IMETHODIMP
nsJSCID::CreateInstance(HandleValue iidval, JSContext* cx, uint8_t optionalArgc,
                        MutableHandleValue retval) {
  if (!mDetails->IsValid()) {
    return NS_ERROR_XPC_BAD_CID;
  }

  if (NS_FAILED(nsXPConnect::SecurityManager()->CanCreateInstance(
          cx, mDetails->ID()))) {
    NS_ERROR("how are we not being called from chrome here?");
    return NS_OK;
  }

  // If an IID was passed in then use it
  const nsID* iid = GetIIDArg(optionalArgc, iidval, cx);
  if (!iid) {
    return NS_ERROR_XPC_BAD_IID;
  }

  nsCOMPtr<nsIComponentManager> compMgr;
  nsresult rv = NS_GetComponentManager(getter_AddRefs(compMgr));
  if (NS_FAILED(rv)) {
    return NS_ERROR_UNEXPECTED;
  }

  nsCOMPtr<nsISupports> inst;
  rv = compMgr->CreateInstance(mDetails->ID(), nullptr, *iid,
                               getter_AddRefs(inst));
  MOZ_ASSERT(NS_FAILED(rv) || inst,
             "component manager returned success, but instance is null!");

  NS_ENSURE_SUCCESS(rv, NS_ERROR_XPC_CI_RETURNED_FAILURE);
  if (!inst) {
    return NS_ERROR_XPC_CI_RETURNED_FAILURE;
  }

  rv = nsContentUtils::WrapNative(cx, inst, iid, retval);
  if (NS_FAILED(rv) || retval.isPrimitive()) {
    return NS_ERROR_XPC_CANT_CREATE_WN;
  }
  return NS_OK;
}

NS_IMETHODIMP
nsJSCID::GetService(HandleValue iidval, JSContext* cx, uint8_t optionalArgc,
                    MutableHandleValue retval) {
  if (!mDetails->IsValid()) {
    return NS_ERROR_XPC_BAD_CID;
  }

  if (NS_FAILED(nsXPConnect::SecurityManager()->CanCreateInstance(
          cx, mDetails->ID()))) {
    MOZ_ASSERT(JS_IsExceptionPending(cx),
               "security manager vetoed GetService without setting exception");
    return NS_OK;
  }

  // If an IID was passed in then use it
  const nsID* iid = GetIIDArg(optionalArgc, iidval, cx);
  if (!iid) {
    return NS_ERROR_XPC_BAD_IID;
  }

  nsCOMPtr<nsIServiceManager> svcMgr;
  nsresult rv = NS_GetServiceManager(getter_AddRefs(svcMgr));
  if (NS_FAILED(rv)) {
    return rv;
  }

  nsCOMPtr<nsISupports> srvc;
  rv = svcMgr->GetService(mDetails->ID(), *iid, getter_AddRefs(srvc));
  MOZ_ASSERT(NS_FAILED(rv) || srvc,
             "service manager returned success, but service is null!");

  NS_ENSURE_SUCCESS(rv, NS_ERROR_XPC_GS_RETURNED_FAILURE);
  if (!srvc) {
    return NS_ERROR_XPC_GS_RETURNED_FAILURE;
  }

  RootedValue v(cx);
  rv = nsContentUtils::WrapNative(cx, srvc, iid, &v);
  if (NS_FAILED(rv) || !v.isObject()) {
    return NS_ERROR_XPC_CANT_CREATE_WN;
  }

  retval.set(v);
  return NS_OK;
}

NS_IMETHODIMP
nsJSCID::Construct(nsIXPConnectWrappedNative* wrapper, JSContext* cx,
                   JSObject* objArg, const CallArgs& args, bool* _retval) {
  RootedObject obj(cx, objArg);
  XPCJSRuntime* xpcrt = nsXPConnect::GetRuntimeInstance();
  if (!xpcrt) {
    return NS_ERROR_FAILURE;
  }

  // 'push' a call context and call on it
  RootedId name(cx, xpcrt->GetStringID(XPCJSContext::IDX_CREATE_INSTANCE));
  XPCCallContext ccx(cx, obj, nullptr, name, args.length(), args.array(),
                     args.rval().address());

  *_retval = XPCWrappedNative::CallMethod(ccx);
  return NS_OK;
}

NS_IMETHODIMP
nsJSCID::HasInstance(nsIXPConnectWrappedNative* wrapper, JSContext* cx,
                     JSObject* /* unused */, HandleValue val, bool* bp,
                     bool* _retval) {
  *bp = false;

  if (!val.isObject()) {
    return NS_OK;
  }

  RootedObject obj(cx, &val.toObject());

  // is this really a native xpcom object with a wrapper?
  RootedObject target(cx);
  nsresult rv = FindObjectForHasInstance(cx, obj, &target);
  if (NS_WARN_IF(NS_FAILED(rv))) {
    return rv;
  }

  if (!target || !IS_WN_REFLECTOR(target)) {
    return NS_OK;
  }

  if (XPCWrappedNative* other_wrapper = XPCWrappedNative::Get(target)) {
    if (nsIClassInfo* ci = other_wrapper->GetClassInfo()) {
      // We consider CID equality to be the thing that matters here.
      // This is perhaps debatable.
      nsID cid;
      if (NS_SUCCEEDED(ci->GetClassIDNoAlloc(&cid))) {
        *bp = cid.Equals(mDetails->ID());
      }
    }
  }

  return NS_OK;
}

/***************************************************************************/
// additional utilities...

JSObject* xpc_NewIDObject(JSContext* cx, HandleObject scope, const nsID& aID) {
  RootedObject obj(cx);

  nsCOMPtr<nsIJSID> iid = nsJSID::NewID(aID);
  if (iid) {
    nsIXPConnect* xpc = nsIXPConnect::XPConnect();
    if (xpc) {
      xpc->WrapNative(cx, scope, static_cast<nsISupports*>(iid),
                      NS_GET_IID(nsIJSID), obj.address());
    }
  }
  return obj;
}

// note: returned pointer is only valid while |obj| remains alive!
const nsID* xpc_JSObjectToID(JSContext* cx, JSObject* obj) {
  if (!cx || !obj) {
    return nullptr;
  }

  // NOTE: this call does NOT addref
  XPCWrappedNative* wrapper = nullptr;
  obj = js::CheckedUnwrap(obj);
  if (obj && IS_WN_REFLECTOR(obj)) {
    wrapper = XPCWrappedNative::Get(obj);
  }
  if (wrapper && (wrapper->HasInterfaceNoQI(NS_GET_IID(nsIJSID)) ||
                  wrapper->HasInterfaceNoQI(NS_GET_IID(nsIJSIID)) ||
                  wrapper->HasInterfaceNoQI(NS_GET_IID(nsIJSCID)))) {
    return ((nsIJSID*)wrapper->GetIdentityObject())->GetID();
  }
  return nullptr;
}

bool xpc_JSObjectIsID(JSContext* cx, JSObject* obj) {
  MOZ_ASSERT(cx && obj, "bad param");
  // NOTE: this call does NOT addref
  XPCWrappedNative* wrapper = nullptr;
  obj = js::CheckedUnwrap(obj);
  if (obj && IS_WN_REFLECTOR(obj)) {
    wrapper = XPCWrappedNative::Get(obj);
  }
  return wrapper && (wrapper->HasInterfaceNoQI(NS_GET_IID(nsIJSID)) ||
                     wrapper->HasInterfaceNoQI(NS_GET_IID(nsIJSIID)) ||
                     wrapper->HasInterfaceNoQI(NS_GET_IID(nsIJSCID)));
}
||||||| merged common ancestors
/***************************************************************************/

NS_IMPL_QUERY_INTERFACE_CI(nsJSCID,
                           nsIJSID,
                           nsIJSCID,
                           nsIXPCScriptable)

NS_IMPL_ADDREF(nsJSCID)
NS_IMPL_RELEASE(nsJSCID)
NS_IMPL_CI_INTERFACE_GETTER(nsJSCID, nsIJSID, nsIJSCID)

// The nsIXPCScriptable map declaration that will generate stubs for us...
#define XPC_MAP_CLASSNAME         nsJSCID
#define XPC_MAP_QUOTED_CLASSNAME "nsJSCID"
#define XPC_MAP_FLAGS (XPC_SCRIPTABLE_WANT_CONSTRUCT | \
                       XPC_SCRIPTABLE_WANT_HASINSTANCE)
#include "xpc_map_end.h" /* This will #undef the above */

nsJSCID::nsJSCID()  { mDetails = new nsJSID(); }
nsJSCID::~nsJSCID() {}

NS_IMETHODIMP nsJSCID::GetName(char * *aName)
    {ResolveName(); return mDetails->GetName(aName);}

NS_IMETHODIMP nsJSCID::GetNumber(char * *aNumber)
    {return mDetails->GetNumber(aNumber);}

NS_IMETHODIMP_(const nsID*) nsJSCID::GetID()
    {return &mDetails->ID();}

NS_IMETHODIMP nsJSCID::GetValid(bool* aValid)
    {return mDetails->GetValid(aValid);}

NS_IMETHODIMP nsJSCID::Equals(nsIJSID* other, bool* _retval)
    {return mDetails->Equals(other, _retval);}

NS_IMETHODIMP
nsJSCID::Initialize(const char* str)
{
    if (str[0] == '{') {
        NS_ENSURE_SUCCESS(mDetails->Initialize(str), NS_ERROR_FAILURE);
    } else {
        nsCOMPtr<nsIComponentRegistrar> registrar;
        NS_GetComponentRegistrar(getter_AddRefs(registrar));
        NS_ENSURE_TRUE(registrar, NS_ERROR_FAILURE);

        nsCID* cid;
        if (NS_FAILED(registrar->ContractIDToCID(str, &cid))) {
            return NS_ERROR_FAILURE;
        }
        mDetails->InitWithName(*cid, str);
        free(cid);
    }
    return NS_OK;
}

NS_IMETHODIMP nsJSCID::ToString(char** _retval)
    {ResolveName(); return mDetails->ToString(_retval);}

void
nsJSCID::ResolveName()
{
    if (!mDetails->NameIsSet()) {
        mDetails->SetNameToNoString();
    }
}

//static
already_AddRefed<nsJSCID>
nsJSCID::NewID(const char* str)
{
    if (!str) {
        NS_ERROR("no string");
        return nullptr;
    }

    RefPtr<nsJSCID> idObj = new nsJSCID();
    if (NS_FAILED(idObj->Initialize(str))) {
        return nullptr;
    }
    return idObj.forget();
}

static const nsID*
GetIIDArg(uint32_t argc, const JS::Value& val, JSContext* cx)
{
    const nsID* iid;

    // If an IID was passed in then use it
    if (argc) {
        JSObject* iidobj;
        if (val.isPrimitive() ||
            !(iidobj = val.toObjectOrNull()) ||
            !(iid = xpc_JSObjectToID(cx, iidobj))) {
            return nullptr;
        }
    } else
        iid = &NS_GET_IID(nsISupports);

    return iid;
}

NS_IMETHODIMP
nsJSCID::CreateInstance(HandleValue iidval, JSContext* cx,
                        uint8_t optionalArgc, MutableHandleValue retval)
{
    if (!mDetails->IsValid()) {
        return NS_ERROR_XPC_BAD_CID;
    }

    if (NS_FAILED(nsXPConnect::SecurityManager()->CanCreateInstance(cx, mDetails->ID()))) {
        NS_ERROR("how are we not being called from chrome here?");
        return NS_OK;
    }

    // If an IID was passed in then use it
    const nsID* iid = GetIIDArg(optionalArgc, iidval, cx);
    if (!iid) {
        return NS_ERROR_XPC_BAD_IID;
    }

    nsCOMPtr<nsIComponentManager> compMgr;
    nsresult rv = NS_GetComponentManager(getter_AddRefs(compMgr));
    if (NS_FAILED(rv)) {
        return NS_ERROR_UNEXPECTED;
    }

    nsCOMPtr<nsISupports> inst;
    rv = compMgr->CreateInstance(mDetails->ID(), nullptr, *iid, getter_AddRefs(inst));
    MOZ_ASSERT(NS_FAILED(rv) || inst, "component manager returned success, but instance is null!");

    NS_ENSURE_SUCCESS(rv, NS_ERROR_XPC_CI_RETURNED_FAILURE);
    if (!inst) {
      return NS_ERROR_XPC_CI_RETURNED_FAILURE;
    }

    rv = nsContentUtils::WrapNative(cx, inst, iid, retval);
    if (NS_FAILED(rv) || retval.isPrimitive()) {
        return NS_ERROR_XPC_CANT_CREATE_WN;
    }
    return NS_OK;
}

NS_IMETHODIMP
nsJSCID::GetService(HandleValue iidval, JSContext* cx, uint8_t optionalArgc,
                    MutableHandleValue retval)
{
    if (!mDetails->IsValid()) {
        return NS_ERROR_XPC_BAD_CID;
    }

    if (NS_FAILED(nsXPConnect::SecurityManager()->CanCreateInstance(cx, mDetails->ID()))) {
        MOZ_ASSERT(JS_IsExceptionPending(cx),
                   "security manager vetoed GetService without setting exception");
        return NS_OK;
    }

    // If an IID was passed in then use it
    const nsID* iid = GetIIDArg(optionalArgc, iidval, cx);
    if (!iid) {
        return NS_ERROR_XPC_BAD_IID;
    }

    nsCOMPtr<nsIServiceManager> svcMgr;
    nsresult rv = NS_GetServiceManager(getter_AddRefs(svcMgr));
    if (NS_FAILED(rv)) {
        return rv;
    }

    nsCOMPtr<nsISupports> srvc;
    rv = svcMgr->GetService(mDetails->ID(), *iid, getter_AddRefs(srvc));
    MOZ_ASSERT(NS_FAILED(rv) || srvc, "service manager returned success, but service is null!");

    NS_ENSURE_SUCCESS(rv, NS_ERROR_XPC_GS_RETURNED_FAILURE);
    if (!srvc) {
        return NS_ERROR_XPC_GS_RETURNED_FAILURE;
    }

    RootedValue v(cx);
    rv = nsContentUtils::WrapNative(cx, srvc, iid, &v);
    if (NS_FAILED(rv) || !v.isObject()) {
        return NS_ERROR_XPC_CANT_CREATE_WN;
    }

    retval.set(v);
    return NS_OK;
}

NS_IMETHODIMP
nsJSCID::Construct(nsIXPConnectWrappedNative* wrapper,
                   JSContext* cx, JSObject* objArg,
                   const CallArgs& args, bool* _retval)
{
    RootedObject obj(cx, objArg);
    XPCJSRuntime* xpcrt = nsXPConnect::GetRuntimeInstance();
    if (!xpcrt) {
        return NS_ERROR_FAILURE;
    }

    // 'push' a call context and call on it
    RootedId name(cx, xpcrt->GetStringID(XPCJSContext::IDX_CREATE_INSTANCE));
    XPCCallContext ccx(cx, obj, nullptr, name, args.length(), args.array(),
                       args.rval().address());

    *_retval = XPCWrappedNative::CallMethod(ccx);
    return NS_OK;
}

NS_IMETHODIMP
nsJSCID::HasInstance(nsIXPConnectWrappedNative* wrapper,
                     JSContext* cx, JSObject * /* unused */,
                     HandleValue val, bool* bp, bool* _retval)
{
    *bp = false;

    if (!val.isObject()) {
        return NS_OK;
    }

    RootedObject obj(cx, &val.toObject());

    // is this really a native xpcom object with a wrapper?
    RootedObject target(cx);
    nsresult rv = FindObjectForHasInstance(cx, obj, &target);
    if (NS_WARN_IF(NS_FAILED(rv))) {
        return rv;
    }

    if (!target || !IS_WN_REFLECTOR(target)) {
        return NS_OK;
    }

    if (XPCWrappedNative* other_wrapper = XPCWrappedNative::Get(target)) {
        if (nsIClassInfo* ci = other_wrapper->GetClassInfo()) {
            // We consider CID equality to be the thing that matters here.
            // This is perhaps debatable.
            nsID cid;
            if (NS_SUCCEEDED(ci->GetClassIDNoAlloc(&cid))) {
                *bp = cid.Equals(mDetails->ID());
            }
        }
    }

    return NS_OK;
}

/***************************************************************************/
// additional utilities...

JSObject*
xpc_NewIDObject(JSContext* cx, HandleObject scope, const nsID& aID)
{
    RootedObject obj(cx);

    nsCOMPtr<nsIJSID> iid = nsJSID::NewID(aID);
    if (iid) {
        nsIXPConnect* xpc = nsIXPConnect::XPConnect();
        if (xpc) {
            xpc->WrapNative(cx, scope, static_cast<nsISupports*>(iid),
                            NS_GET_IID(nsIJSID), obj.address());
        }
    }
    return obj;
}

// note: returned pointer is only valid while |obj| remains alive!
const nsID*
xpc_JSObjectToID(JSContext* cx, JSObject* obj)
{
    if (!cx || !obj) {
        return nullptr;
    }

    // NOTE: this call does NOT addref
    XPCWrappedNative* wrapper = nullptr;
    obj = js::CheckedUnwrap(obj);
    if (obj && IS_WN_REFLECTOR(obj)) {
        wrapper = XPCWrappedNative::Get(obj);
    }
    if (wrapper &&
        (wrapper->HasInterfaceNoQI(NS_GET_IID(nsIJSID))  ||
         wrapper->HasInterfaceNoQI(NS_GET_IID(nsIJSIID)) ||
         wrapper->HasInterfaceNoQI(NS_GET_IID(nsIJSCID)))) {
        return ((nsIJSID*)wrapper->GetIdentityObject())->GetID();
    }
    return nullptr;
}

bool
xpc_JSObjectIsID(JSContext* cx, JSObject* obj)
{
    MOZ_ASSERT(cx && obj, "bad param");
    // NOTE: this call does NOT addref
    XPCWrappedNative* wrapper = nullptr;
    obj = js::CheckedUnwrap(obj);
    if (obj && IS_WN_REFLECTOR(obj)) {
        wrapper = XPCWrappedNative::Get(obj);
    }
    return wrapper &&
           (wrapper->HasInterfaceNoQI(NS_GET_IID(nsIJSID))  ||
            wrapper->HasInterfaceNoQI(NS_GET_IID(nsIJSIID)) ||
            wrapper->HasInterfaceNoQI(NS_GET_IID(nsIJSCID)));
}


=======
  // Need to unwrap Window correctly here, so use ReflectorToISupportsDynamic.
  nsCOMPtr<nsISupports> identity = ReflectorToISupportsDynamic(obj, cx);
  if (!identity) {
    return NS_OK;
  }

  nsCOMPtr<nsISupports> supp;
  identity->QueryInterface(*iid, getter_AddRefs(supp));
  *bp = supp;

  // Our old HasInstance implementation operated by invoking FindTearOff on
  // XPCWrappedNatives, and various bits of chrome JS came to depend on
  // |instanceof| doing an implicit QI if it succeeds. Do a drive-by QI to
  // preserve that behavior. This is just a compatibility hack, so we don't
  // really care if it fails.
  if (IS_WN_REFLECTOR(obj)) {
    (void)XPCWrappedNative::Get(obj)->FindTearOff(cx, *iid);
  }

  return NS_OK;
}

static bool IID_HasInstance(JSContext* aCx, unsigned aArgc, Value* aVp) {
  CallArgs args = CallArgsFromVp(aArgc, aVp);
  if (!args.requireAtLeast(aCx, "nsIID[Symbol.hasInstance]", 1)) {
    return false;
  }

  Maybe<nsID> id = JSValue2ID(aCx, args.thisv());
  if (!id) {
    return Throw(aCx, NS_ERROR_XPC_BAD_CONVERT_JS);
  }

  bool hasInstance = false;
  if (args[0].isObject()) {
    RootedObject target(aCx, &args[0].toObject());
    nsresult rv = HasInstance(aCx, target, id.ptr(), &hasInstance);
    if (NS_FAILED(rv)) {
      return Throw(aCx, rv);
    }
  }
  args.rval().setBoolean(hasInstance);
  return true;
}

// NOTE: This method is used both for 'get IID.prototype.name' and
// 'IID.prototype.toString'.
static bool IID_GetName(JSContext* aCx, unsigned aArgc, Value* aVp) {
  CallArgs args = CallArgsFromVp(aArgc, aVp);

  RootedObject obj(aCx, GetIDObject(args.thisv(), &sIID_Class));
  if (!obj) {
    return Throw(aCx, NS_ERROR_XPC_BAD_CONVERT_JS);
  }

  auto* info =
      (const nsXPTInterfaceInfo*)js::GetReservedSlot(obj, kIID_InfoSlot)
          .toPrivate();

  // Name property is the name of the interface this nsIID was created from.
  JSString* name = JS_NewStringCopyZ(aCx, info->Name());
  if (!name) {
    return Throw(aCx, NS_ERROR_OUT_OF_MEMORY);
  }

  args.rval().setString(name);
  return true;
}

// Common code for CID_CreateInstance and CID_GetService
static bool CIGSHelper(JSContext* aCx, unsigned aArgc, Value* aVp,
                       bool aGetService) {
  CallArgs args = CallArgsFromVp(aArgc, aVp);

  // Extract the ContractID string from our reserved slot. Don't use
  // JSValue2ID as this method should only be defined on Contract ID objects,
  // and it allows us to avoid a duplicate hashtable lookup.
  RootedObject obj(aCx, GetIDObject(args.thisv(), &sCID_Class));
  if (!obj) {
    return Throw(aCx, NS_ERROR_XPC_BAD_CONVERT_JS);
  }
  JS::UniqueChars contractID = JS_EncodeStringToLatin1(
      aCx, js::GetReservedSlot(obj, kCID_ContractSlot).toString());

  // Extract the IID from the first argument, if passed. Default: nsISupports.
  Maybe<nsIID> iid = args.length() >= 1 ? JSValue2ID(aCx, args[0])
                                        : Some(NS_GET_IID(nsISupports));
  if (!iid) {
    return Throw(aCx, NS_ERROR_XPC_BAD_CONVERT_JS);
  }

  // Invoke CreateInstance or GetService with our ContractID.
  nsresult rv;
  nsCOMPtr<nsISupports> result;
  if (aGetService) {
    rv = CallGetService(contractID.get(), *iid, getter_AddRefs(result));
    if (NS_FAILED(rv) || !result) {
      return Throw(aCx, NS_ERROR_XPC_GS_RETURNED_FAILURE);
    }
  } else {
    rv = CallCreateInstance(contractID.get(), nullptr, *iid,
                            getter_AddRefs(result));
    if (NS_FAILED(rv) || !result) {
      return Throw(aCx, NS_ERROR_XPC_CI_RETURNED_FAILURE);
    }
  }

  // Wrap the created object and return it.
  rv = nsContentUtils::WrapNative(aCx, result, iid.ptr(), args.rval());
  if (NS_FAILED(rv) || args.rval().isPrimitive()) {
    return Throw(aCx, NS_ERROR_XPC_CANT_CREATE_WN);
  }
  return true;
}

static bool CID_CreateInstance(JSContext* aCx, unsigned aArgc, Value* aVp) {
  return CIGSHelper(aCx, aArgc, aVp, /* aGetService = */ false);
}

static bool CID_GetService(JSContext* aCx, unsigned aArgc, Value* aVp) {
  return CIGSHelper(aCx, aArgc, aVp, /* aGetService = */ true);
}

// NOTE: This method is used both for 'get CID.prototype.name' and
// 'CID.prototype.toString'.
static bool CID_GetName(JSContext* aCx, unsigned aArgc, Value* aVp) {
  CallArgs args = CallArgsFromVp(aArgc, aVp);
  RootedObject obj(aCx, GetIDObject(args.thisv(), &sCID_Class));
  if (!obj) {
    return Throw(aCx, NS_ERROR_XPC_BAD_CONVERT_JS);
  }

  // Return the string stored in our reserved ContractID slot.
  args.rval().set(js::GetReservedSlot(obj, kCID_ContractSlot));
  return true;
}

}  // namespace xpc
>>>>>>> upstream-releases
