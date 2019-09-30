/* -*- Mode: C++; tab-width: 8; indent-tabs-mode: nil; c-basic-offset: 2 -*- */
/* vim: set ts=8 sts=2 et sw=2 tw=80: */
/* This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/. */

/* Class used to manage the wrapped native objects within a JS scope. */

#include "xpcprivate.h"
#include "XPCWrapper.h"
#include "nsContentUtils.h"
#include "nsCycleCollectionNoteRootCallback.h"
#include "ExpandedPrincipal.h"
#include "mozilla/BasePrincipal.h"
#include "mozilla/MemoryReporting.h"
#include "mozilla/Preferences.h"
#include "mozilla/Unused.h"
#include "nsIXULRuntime.h"
#include "mozJSComponentLoader.h"

#include "mozilla/dom/BindingUtils.h"

using namespace mozilla;
using namespace xpc;
using namespace JS;

/***************************************************************************/

<<<<<<< HEAD
XPCWrappedNativeScope* XPCWrappedNativeScope::gScopes = nullptr;
XPCWrappedNativeScope* XPCWrappedNativeScope::gDyingScopes = nullptr;

static bool RemoteXULForbidsXBLScope(nsIPrincipal* aPrincipal,
                                     HandleObject aGlobal) {
  MOZ_ASSERT(aPrincipal);

  // Certain singleton sandoxes are created very early in startup - too early
  // to call into AllowXULXBLForPrincipal. We never create XBL scopes for
  // sandboxes anway, and certainly not for these singleton scopes. So we just
  // short-circuit here.
  if (IsSandbox(aGlobal)) {
    return false;
  }
||||||| merged common ancestors
XPCWrappedNativeScope* XPCWrappedNativeScope::gScopes = nullptr;
XPCWrappedNativeScope* XPCWrappedNativeScope::gDyingScopes = nullptr;

static bool
RemoteXULForbidsXBLScope(nsIPrincipal* aPrincipal, HandleObject aGlobal)
{
  MOZ_ASSERT(aPrincipal);

  // Certain singleton sandoxes are created very early in startup - too early
  // to call into AllowXULXBLForPrincipal. We never create XBL scopes for
  // sandboxes anway, and certainly not for these singleton scopes. So we just
  // short-circuit here.
  if (IsSandbox(aGlobal)) {
      return false;
  }
=======
static XPCWrappedNativeScopeList& AllScopes() {
  return XPCJSRuntime::Get()->GetWrappedNativeScopes();
}
>>>>>>> upstream-releases

static bool RemoteXULForbidsXBLScopeForPrincipal(nsIPrincipal* aPrincipal) {
  // AllowXULXBLForPrincipal will return true for system principal, but we
  // don't want that here.
  MOZ_ASSERT(nsContentUtils::IsInitialized());
<<<<<<< HEAD
  if (nsContentUtils::IsSystemPrincipal(aPrincipal)) {
    return false;
||||||| merged common ancestors
  if (nsContentUtils::IsSystemPrincipal(aPrincipal)) {
      return false;
=======
  if (aPrincipal->IsSystemPrincipal()) {
    return false;
>>>>>>> upstream-releases
  }

  // If this domain isn't whitelisted, we're done.
  if (!nsContentUtils::AllowXULXBLForPrincipal(aPrincipal)) {
    return false;
  }

  // Check the pref to determine how we should behave.
  return !Preferences::GetBool("dom.use_xbl_scopes_for_remote_xul", false);
}

<<<<<<< HEAD
XPCWrappedNativeScope::XPCWrappedNativeScope(
    JSContext* cx, JS::HandleObject aGlobal,
    const mozilla::SiteIdentifier& aSite)
    : mWrappedNativeMap(Native2WrappedNativeMap::newMap(XPC_NATIVE_MAP_LENGTH)),
      mWrappedNativeProtoMap(
          ClassInfo2WrappedNativeProtoMap::newMap(XPC_NATIVE_PROTO_MAP_LENGTH)),
      mComponents(nullptr),
      mNext(nullptr),
      mGlobalJSObject(aGlobal) {
  // add ourselves to the scopes list
  {
    MOZ_ASSERT(aGlobal);
    DebugOnly<const js::Class*> clasp = js::GetObjectClass(aGlobal);
    MOZ_ASSERT(clasp->flags &
                   (JSCLASS_PRIVATE_IS_NSISUPPORTS | JSCLASS_HAS_PRIVATE) ||
               mozilla::dom::IsDOMClass(clasp));
||||||| merged common ancestors
XPCWrappedNativeScope::XPCWrappedNativeScope(JSContext* cx,
                                             JS::HandleObject aGlobal,
                                             const mozilla::SiteIdentifier& aSite)
      : mWrappedNativeMap(Native2WrappedNativeMap::newMap(XPC_NATIVE_MAP_LENGTH)),
        mWrappedNativeProtoMap(ClassInfo2WrappedNativeProtoMap::newMap(XPC_NATIVE_PROTO_MAP_LENGTH)),
        mComponents(nullptr),
        mNext(nullptr),
        mGlobalJSObject(aGlobal)
{
    // add ourselves to the scopes list
    {
        MOZ_ASSERT(aGlobal);
        DebugOnly<const js::Class*> clasp = js::GetObjectClass(aGlobal);
        MOZ_ASSERT(clasp->flags & (JSCLASS_PRIVATE_IS_NSISUPPORTS |
                                   JSCLASS_HAS_PRIVATE) ||
                   mozilla::dom::IsDOMClass(clasp));
=======
static bool RemoteXULForbidsXBLScope(HandleObject aFirstGlobal) {
  MOZ_ASSERT(aFirstGlobal);

  // Certain singleton sandoxes are created very early in startup - too early
  // to call into AllowXULXBLForPrincipal. We never create XBL scopes for
  // sandboxes anway, and certainly not for these singleton scopes. So we just
  // short-circuit here.
  if (IsSandbox(aFirstGlobal)) {
    return false;
  }

  nsIPrincipal* principal = xpc::GetObjectPrincipal(aFirstGlobal);
  return RemoteXULForbidsXBLScopeForPrincipal(principal);
}

XPCWrappedNativeScope::XPCWrappedNativeScope(JS::Compartment* aCompartment,
                                             JS::HandleObject aFirstGlobal)
    : mWrappedNativeMap(Native2WrappedNativeMap::newMap(XPC_NATIVE_MAP_LENGTH)),
      mWrappedNativeProtoMap(
          ClassInfo2WrappedNativeProtoMap::newMap(XPC_NATIVE_PROTO_MAP_LENGTH)),
      mComponents(nullptr),
      mCompartment(aCompartment) {
>>>>>>> upstream-releases
#ifdef DEBUG
<<<<<<< HEAD
    for (XPCWrappedNativeScope* cur = gScopes; cur; cur = cur->mNext) {
      MOZ_ASSERT(aGlobal != cur->GetGlobalJSObjectPreserveColor(),
                 "dup object");
    }
||||||| merged common ancestors
        for (XPCWrappedNativeScope* cur = gScopes; cur; cur = cur->mNext) {
            MOZ_ASSERT(aGlobal != cur->GetGlobalJSObjectPreserveColor(), "dup object");
        }
=======
  for (XPCWrappedNativeScope* cur : AllScopes()) {
    MOZ_ASSERT(aCompartment != cur->Compartment(), "dup object");
  }
>>>>>>> upstream-releases
#endif

<<<<<<< HEAD
    mNext = gScopes;
    gScopes = this;
  }
||||||| merged common ancestors
        mNext = gScopes;
        gScopes = this;
    }
=======
  AllScopes().insertBack(this);
>>>>>>> upstream-releases

<<<<<<< HEAD
  MOZ_COUNT_CTOR(XPCWrappedNativeScope);

  nsIPrincipal* principal = GetPrincipal();

  // Create the compartment private.
  JS::Compartment* c = js::GetObjectCompartment(aGlobal);
  MOZ_ASSERT(!JS_GetCompartmentPrivate(c));
  CompartmentPrivate* priv =
      new CompartmentPrivate(c, BasePrincipal::Cast(principal), aSite);
  JS_SetCompartmentPrivate(c, priv);

  // Attach ourselves to the realm private.
  Realm* realm = JS::GetObjectRealmOrNull(aGlobal);
  RealmPrivate* realmPriv = new RealmPrivate(realm);
  realmPriv->scope = this;
  JS::SetRealmPrivate(realm, realmPriv);

  // Determine whether we would allow an XBL scope in this situation.
  // In addition to being pref-controlled, we also disable XBL scopes for
  // remote XUL domains, _except_ if we have an additional pref override set.
  mAllowContentXBLScope = !RemoteXULForbidsXBLScope(principal, aGlobal);

  // Determine whether to use an XBL scope.
  mUseContentXBLScope = mAllowContentXBLScope;
  if (mUseContentXBLScope) {
    const js::Class* clasp = js::GetObjectClass(mGlobalJSObject);
    mUseContentXBLScope = !strcmp(clasp->name, "Window");
  }
  if (mUseContentXBLScope) {
    mUseContentXBLScope =
        principal && !nsContentUtils::IsSystemPrincipal(principal);
  }
}
||||||| merged common ancestors
    MOZ_COUNT_CTOR(XPCWrappedNativeScope);

    nsIPrincipal* principal = GetPrincipal();

    // Create the compartment private.
    JS::Compartment* c = js::GetObjectCompartment(aGlobal);
    MOZ_ASSERT(!JS_GetCompartmentPrivate(c));
    CompartmentPrivate* priv =
        new CompartmentPrivate(c, BasePrincipal::Cast(principal), aSite);
    JS_SetCompartmentPrivate(c, priv);

    // Attach ourselves to the realm private.
    Realm* realm = JS::GetObjectRealmOrNull(aGlobal);
    RealmPrivate* realmPriv = new RealmPrivate(realm);
    realmPriv->scope = this;
    JS::SetRealmPrivate(realm, realmPriv);

    // Determine whether we would allow an XBL scope in this situation.
    // In addition to being pref-controlled, we also disable XBL scopes for
    // remote XUL domains, _except_ if we have an additional pref override set.
    mAllowContentXBLScope = !RemoteXULForbidsXBLScope(principal, aGlobal);

    // Determine whether to use an XBL scope.
    mUseContentXBLScope = mAllowContentXBLScope;
    if (mUseContentXBLScope) {
        const js::Class* clasp = js::GetObjectClass(mGlobalJSObject);
        mUseContentXBLScope = !strcmp(clasp->name, "Window");
    }
    if (mUseContentXBLScope) {
        mUseContentXBLScope = principal && !nsContentUtils::IsSystemPrincipal(principal);
    }
}
=======
  MOZ_COUNT_CTOR(XPCWrappedNativeScope);
>>>>>>> upstream-releases

<<<<<<< HEAD
// static
bool XPCWrappedNativeScope::IsDyingScope(XPCWrappedNativeScope* scope) {
  for (XPCWrappedNativeScope* cur = gDyingScopes; cur; cur = cur->mNext) {
    if (scope == cur) {
      return true;
    }
  }
  return false;
||||||| merged common ancestors
// static
bool
XPCWrappedNativeScope::IsDyingScope(XPCWrappedNativeScope* scope)
{
    for (XPCWrappedNativeScope* cur = gDyingScopes; cur; cur = cur->mNext) {
        if (scope == cur) {
            return true;
        }
    }
    return false;
=======
  // Determine whether we would allow an XBL scope in this situation.
  // In addition to being pref-controlled, we also disable XBL scopes for
  // remote XUL domains, _except_ if we have an additional pref override set.
  //
  // Note that we can't quite remove this yet, even though we never actually
  // use XBL scopes, because some code (including the security manager) uses
  // this boolean to make decisions that we rely on in our test infrastructure.
  mAllowContentXBLScope = !RemoteXULForbidsXBLScope(aFirstGlobal);
>>>>>>> upstream-releases
}

<<<<<<< HEAD
bool XPCWrappedNativeScope::GetComponentsJSObject(JS::MutableHandleObject obj) {
  AutoJSContext cx;
  if (!mComponents) {
    nsIPrincipal* p = GetPrincipal();
    bool system = nsXPConnect::SecurityManager()->IsSystemPrincipal(p);
    mComponents =
        system ? new nsXPCComponents(this) : new nsXPCComponentsBase(this);
  }
||||||| merged common ancestors
bool
XPCWrappedNativeScope::GetComponentsJSObject(JS::MutableHandleObject obj)
{
    AutoJSContext cx;
    if (!mComponents) {
        nsIPrincipal* p = GetPrincipal();
        bool system = nsXPConnect::SecurityManager()->IsSystemPrincipal(p);
        mComponents = system ? new nsXPCComponents(this)
                             : new nsXPCComponentsBase(this);
    }
=======
bool XPCWrappedNativeScope::GetComponentsJSObject(JSContext* cx,
                                                  JS::MutableHandleObject obj) {
  if (!mComponents) {
    bool system = AccessCheck::isChrome(mCompartment);
    mComponents =
        system ? new nsXPCComponents(this) : new nsXPCComponentsBase(this);
  }
>>>>>>> upstream-releases

<<<<<<< HEAD
  RootedValue val(cx);
  xpcObjectHelper helper(mComponents);
  bool ok = XPCConvert::NativeInterface2JSObject(&val, helper, nullptr, false,
                                                 nullptr);
  if (NS_WARN_IF(!ok)) {
    return false;
  }
||||||| merged common ancestors
    RootedValue val(cx);
    xpcObjectHelper helper(mComponents);
    bool ok = XPCConvert::NativeInterface2JSObject(&val, helper, nullptr,
                                                   false, nullptr);
    if (NS_WARN_IF(!ok)) {
        return false;
    }
=======
  RootedValue val(cx);
  xpcObjectHelper helper(mComponents);
  bool ok = XPCConvert::NativeInterface2JSObject(cx, &val, helper, nullptr,
                                                 false, nullptr);
  if (NS_WARN_IF(!ok)) {
    return false;
  }
>>>>>>> upstream-releases

  if (NS_WARN_IF(!val.isObject())) {
    return false;
  }

  // The call to wrap() here is necessary even though the object is same-
  // compartment, because it applies our security wrapper.
  obj.set(&val.toObject());
  if (NS_WARN_IF(!JS_WrapObject(cx, obj))) {
    return false;
  }
  return true;
}

void XPCWrappedNativeScope::ForcePrivilegedComponents() {
  nsCOMPtr<nsIXPCComponents> c = do_QueryInterface(mComponents);
  if (!c) {
    mComponents = new nsXPCComponents(this);
  }
}

<<<<<<< HEAD
static bool DefineSubcomponentProperty(JSContext* aCx, HandleObject aGlobal,
                                       nsISupports* aSubcomponent,
                                       const nsID* aIID,
                                       unsigned int aStringIndex) {
  RootedValue subcompVal(aCx);
  xpcObjectHelper helper(aSubcomponent);
  if (!XPCConvert::NativeInterface2JSObject(&subcompVal, helper, aIID, false,
                                            nullptr))
    return false;
  if (NS_WARN_IF(!subcompVal.isObject())) {
    return false;
  }
  RootedId id(aCx, XPCJSContext::Get()->GetStringID(aStringIndex));
  return JS_DefinePropertyById(aCx, aGlobal, id, subcompVal, 0);
||||||| merged common ancestors
static bool
DefineSubcomponentProperty(JSContext* aCx,
                           HandleObject aGlobal,
                           nsISupports* aSubcomponent,
                           const nsID* aIID,
                           unsigned int aStringIndex)
{
    RootedValue subcompVal(aCx);
    xpcObjectHelper helper(aSubcomponent);
    if (!XPCConvert::NativeInterface2JSObject(&subcompVal, helper,
                                              aIID, false, nullptr))
        return false;
    if (NS_WARN_IF(!subcompVal.isObject())) {
        return false;
    }
    RootedId id(aCx, XPCJSContext::Get()->GetStringID(aStringIndex));
    return JS_DefinePropertyById(aCx, aGlobal, id, subcompVal, 0);
=======
static bool DefineSubcomponentProperty(JSContext* aCx, HandleObject aGlobal,
                                       nsISupports* aSubcomponent,
                                       const nsID* aIID,
                                       unsigned int aStringIndex) {
  RootedValue subcompVal(aCx);
  xpcObjectHelper helper(aSubcomponent);
  if (!XPCConvert::NativeInterface2JSObject(aCx, &subcompVal, helper, aIID,
                                            false, nullptr))
    return false;
  if (NS_WARN_IF(!subcompVal.isObject())) {
    return false;
  }
  RootedId id(aCx, XPCJSContext::Get()->GetStringID(aStringIndex));
  return JS_DefinePropertyById(aCx, aGlobal, id, subcompVal, 0);
>>>>>>> upstream-releases
}

<<<<<<< HEAD
bool XPCWrappedNativeScope::AttachComponentsObject(JSContext* aCx) {
  RootedObject components(aCx);
  if (!GetComponentsJSObject(&components)) {
    return false;
  }
||||||| merged common ancestors
bool
XPCWrappedNativeScope::AttachComponentsObject(JSContext* aCx)
{
    RootedObject components(aCx);
    if (!GetComponentsJSObject(&components)) {
        return false;
    }
=======
bool XPCWrappedNativeScope::AttachComponentsObject(JSContext* aCx) {
  RootedObject components(aCx);
  if (!GetComponentsJSObject(aCx, &components)) {
    return false;
  }
>>>>>>> upstream-releases

<<<<<<< HEAD
  RootedObject global(aCx, GetGlobalJSObject());
  MOZ_ASSERT(js::IsObjectInContextCompartment(global, aCx));
||||||| merged common ancestors
    RootedObject global(aCx, GetGlobalJSObject());
    MOZ_ASSERT(js::IsObjectInContextCompartment(global, aCx));
=======
  RootedObject global(aCx, CurrentGlobalOrNull(aCx));
>>>>>>> upstream-releases

  // The global Components property is non-configurable if it's a full
  // nsXPCComponents object. That way, if it's an nsXPCComponentsBase,
  // enableUniversalXPConnect can upgrade it later.
  unsigned attrs = JSPROP_READONLY | JSPROP_RESOLVING;
  nsCOMPtr<nsIXPCComponents> c = do_QueryInterface(mComponents);
  if (c) {
    attrs |= JSPROP_PERMANENT;
  }

  RootedId id(aCx,
              XPCJSContext::Get()->GetStringID(XPCJSContext::IDX_COMPONENTS));
  if (!JS_DefinePropertyById(aCx, global, id, components, attrs)) {
    return false;
  }

// _iid can be nullptr if the object implements classinfo.
#define DEFINE_SUBCOMPONENT_PROPERTY(_comp, _type, _iid, _id)                 \
  nsCOMPtr<nsIXPCComponents_##_type> obj##_type;                              \
  if (NS_FAILED(_comp->Get##_type(getter_AddRefs(obj##_type)))) return false; \
  if (!DefineSubcomponentProperty(aCx, global, obj##_type, _iid,              \
                                  XPCJSContext::IDX_##_id))                   \
    return false;

  DEFINE_SUBCOMPONENT_PROPERTY(mComponents, Interfaces, nullptr, CI)
  DEFINE_SUBCOMPONENT_PROPERTY(mComponents, Results, nullptr, CR)

  if (!c) {
    return true;
  }

  DEFINE_SUBCOMPONENT_PROPERTY(c, Classes, nullptr, CC)
  DEFINE_SUBCOMPONENT_PROPERTY(c, Utils, &NS_GET_IID(nsIXPCComponents_Utils),
                               CU)

#undef DEFINE_SUBCOMPONENT_PROPERTY

  return true;
}

<<<<<<< HEAD
JSObject* XPCWrappedNativeScope::EnsureContentXBLScope(JSContext* cx) {
  JS::RootedObject global(cx, GetGlobalJSObject());
  MOZ_ASSERT(js::IsObjectInContextCompartment(global, cx));
  MOZ_ASSERT(!IsContentXBLScope());
  MOZ_ASSERT(strcmp(js::GetObjectClass(global)->name,
                    "nsXBLPrototypeScript compilation scope"));

  // If we already have a special XBL scope object, we know what to use.
  if (mContentXBLScope) {
    return mContentXBLScope;
  }

  // If this scope doesn't need an XBL scope, just return the global.
  if (!mUseContentXBLScope) {
    return global;
  }
||||||| merged common ancestors
JSObject*
XPCWrappedNativeScope::EnsureContentXBLScope(JSContext* cx)
{
    JS::RootedObject global(cx, GetGlobalJSObject());
    MOZ_ASSERT(js::IsObjectInContextCompartment(global, cx));
    MOZ_ASSERT(!IsContentXBLScope());
    MOZ_ASSERT(strcmp(js::GetObjectClass(global)->name,
                      "nsXBLPrototypeScript compilation scope"));

    // If we already have a special XBL scope object, we know what to use.
    if (mContentXBLScope) {
        return mContentXBLScope;
    }

    // If this scope doesn't need an XBL scope, just return the global.
    if (!mUseContentXBLScope) {
        return global;
    }
=======
JSObject* XPCWrappedNativeScope::EnsureContentXBLScope(JSContext* cx) {
  JS::RootedObject global(cx, CurrentGlobalOrNull(cx));
  MOZ_ASSERT(js::IsObjectInContextCompartment(global, cx));
  MOZ_ASSERT(!IsContentXBLScope());
  MOZ_ASSERT(strcmp(js::GetObjectClass(global)->name,
                    "nsXBLPrototypeScript compilation scope"));
>>>>>>> upstream-releases

<<<<<<< HEAD
  // Set up the sandbox options. Note that we use the DOM global as the
  // sandboxPrototype so that the XBL scope can access all the DOM objects
  // it's accustomed to accessing.
  //
  // In general wantXrays shouldn't matter much here, but there are weird
  // cases when adopting bound content between same-origin globals where a
  // <destructor> in one content XBL scope sees anonymous content in another
  // content XBL scope. When that happens, we hit LookupBindingMember for an
  // anonymous element that lives in a content XBL scope, which isn't a tested
  // or audited codepath. So let's avoid hitting that case by opting out of
  // same-origin Xrays.
  SandboxOptions options;
  options.wantXrays = false;
  options.wantComponents = false;
  options.proto = global;
  options.sameZoneAs = global;
  options.isContentXBLScope = true;

  // Use an ExpandedPrincipal to create asymmetric security.
  nsIPrincipal* principal = GetPrincipal();
  MOZ_ASSERT(!nsContentUtils::IsExpandedPrincipal(principal));
  nsTArray<nsCOMPtr<nsIPrincipal>> principalAsArray(1);
  principalAsArray.AppendElement(principal);
  RefPtr<ExpandedPrincipal> ep = ExpandedPrincipal::Create(
      principalAsArray, principal->OriginAttributesRef());

  // Create the sandbox.
  RootedValue v(cx);
  nsresult rv = CreateSandboxObject(
      cx, &v, static_cast<nsIExpandedPrincipal*>(ep), options);
  NS_ENSURE_SUCCESS(rv, nullptr);
  mContentXBLScope = &v.toObject();

  MOZ_ASSERT(xpc::IsInContentXBLScope(js::UncheckedUnwrap(mContentXBLScope)));

  // Good to go!
  return mContentXBLScope;
}

bool XPCWrappedNativeScope::AllowContentXBLScope() {
  // We only disallow XBL scopes in remote XUL situations.
  MOZ_ASSERT_IF(!mAllowContentXBLScope,
                nsContentUtils::AllowXULXBLForPrincipal(GetPrincipal()));
  return mAllowContentXBLScope;
||||||| merged common ancestors
    // Set up the sandbox options. Note that we use the DOM global as the
    // sandboxPrototype so that the XBL scope can access all the DOM objects
    // it's accustomed to accessing.
    //
    // In general wantXrays shouldn't matter much here, but there are weird
    // cases when adopting bound content between same-origin globals where a
    // <destructor> in one content XBL scope sees anonymous content in another
    // content XBL scope. When that happens, we hit LookupBindingMember for an
    // anonymous element that lives in a content XBL scope, which isn't a tested
    // or audited codepath. So let's avoid hitting that case by opting out of
    // same-origin Xrays.
    SandboxOptions options;
    options.wantXrays = false;
    options.wantComponents = false;
    options.proto = global;
    options.sameZoneAs = global;
    options.isContentXBLScope = true;

    // Use an ExpandedPrincipal to create asymmetric security.
    nsIPrincipal* principal = GetPrincipal();
    MOZ_ASSERT(!nsContentUtils::IsExpandedPrincipal(principal));
    nsTArray<nsCOMPtr<nsIPrincipal>> principalAsArray(1);
    principalAsArray.AppendElement(principal);
    RefPtr<ExpandedPrincipal> ep =
        ExpandedPrincipal::Create(principalAsArray,
                                  principal->OriginAttributesRef());

    // Create the sandbox.
    RootedValue v(cx);
    nsresult rv = CreateSandboxObject(cx, &v,
                                      static_cast<nsIExpandedPrincipal*>(ep),
                                      options);
    NS_ENSURE_SUCCESS(rv, nullptr);
    mContentXBLScope = &v.toObject();

    MOZ_ASSERT(xpc::IsInContentXBLScope(js::UncheckedUnwrap(mContentXBLScope)));

    // Good to go!
    return mContentXBLScope;
=======
  // We can probably remove EnsureContentXBLScope and clean up all its callers,
  // but a bunch (all?) of those callers will just go away when we remove XBL
  // support, so it's simpler to just leave it here as a no-op.
  return global;
>>>>>>> upstream-releases
}

<<<<<<< HEAD
namespace xpc {
JSObject* GetXBLScope(JSContext* cx, JSObject* contentScopeArg) {
  JS::RootedObject contentScope(cx, contentScopeArg);
  JSAutoRealm ar(cx, contentScope);
  XPCWrappedNativeScope* nativeScope = RealmPrivate::Get(contentScope)->scope;

  RootedObject scope(cx, nativeScope->EnsureContentXBLScope(cx));
  NS_ENSURE_TRUE(scope, nullptr);  // See bug 858642.

  scope = js::UncheckedUnwrap(scope);
  JS::ExposeObjectToActiveJS(scope);
  return scope;
||||||| merged common ancestors
bool
XPCWrappedNativeScope::AllowContentXBLScope()
{
    // We only disallow XBL scopes in remote XUL situations.
    MOZ_ASSERT_IF(!mAllowContentXBLScope,
                  nsContentUtils::AllowXULXBLForPrincipal(GetPrincipal()));
    return mAllowContentXBLScope;
=======
bool XPCWrappedNativeScope::XBLScopeStateMatches(nsIPrincipal* aPrincipal) {
  return mAllowContentXBLScope ==
         !RemoteXULForbidsXBLScopeForPrincipal(aPrincipal);
>>>>>>> upstream-releases
}

<<<<<<< HEAD
JSObject* GetUAWidgetScope(JSContext* cx, JSObject* contentScopeArg) {
  JS::RootedObject contentScope(cx, contentScopeArg);
  JSAutoRealm ar(cx, contentScope);
  nsIPrincipal* principal = GetObjectPrincipal(contentScope);
||||||| merged common ancestors
namespace xpc {
JSObject*
GetXBLScope(JSContext* cx, JSObject* contentScopeArg)
{
    JS::RootedObject contentScope(cx, contentScopeArg);
    JSAutoRealm ar(cx, contentScope);
    XPCWrappedNativeScope* nativeScope = RealmPrivate::Get(contentScope)->scope;

    RootedObject scope(cx, nativeScope->EnsureContentXBLScope(cx));
    NS_ENSURE_TRUE(scope, nullptr); // See bug 858642.

    scope = js::UncheckedUnwrap(scope);
    JS::ExposeObjectToActiveJS(scope);
    return scope;
}

JSObject*
GetUAWidgetScope(JSContext* cx, JSObject* contentScopeArg)
{
    JS::RootedObject contentScope(cx, contentScopeArg);
    JSAutoRealm ar(cx, contentScope);
    nsIPrincipal* principal = GetObjectPrincipal(contentScope);

    if (nsContentUtils::IsSystemPrincipal(principal)) {
        return JS::GetNonCCWObjectGlobal(contentScope);
    }
=======
bool XPCWrappedNativeScope::AllowContentXBLScope(Realm* aRealm) {
  // We only disallow XBL scopes in remote XUL situations.
  MOZ_ASSERT_IF(!mAllowContentXBLScope, nsContentUtils::AllowXULXBLForPrincipal(
                                            xpc::GetRealmPrincipal(aRealm)));
  return mAllowContentXBLScope;
}

namespace xpc {
JSObject* GetXBLScope(JSContext* cx, JSObject* contentScopeArg) {
  JS::RootedObject contentScope(cx, contentScopeArg);
  JSAutoRealm ar(cx, contentScope);
  XPCWrappedNativeScope* nativeScope = ObjectScope(contentScope);

  RootedObject scope(cx, nativeScope->EnsureContentXBLScope(cx));
  NS_ENSURE_TRUE(scope, nullptr);  // See bug 858642.
>>>>>>> upstream-releases

<<<<<<< HEAD
  if (nsContentUtils::IsSystemPrincipal(principal)) {
    return JS::GetNonCCWObjectGlobal(contentScope);
  }

  return GetUAWidgetScope(cx, principal);
||||||| merged common ancestors
    return GetUAWidgetScope(cx, principal);
=======
  scope = js::UncheckedUnwrap(scope);
  JS::ExposeObjectToActiveJS(scope);
  return scope;
>>>>>>> upstream-releases
}

<<<<<<< HEAD
JSObject* GetUAWidgetScope(JSContext* cx, nsIPrincipal* principal) {
  RootedObject scope(cx, XPCJSRuntime::Get()->GetUAWidgetScope(cx, principal));
  NS_ENSURE_TRUE(scope, nullptr);  // See bug 858642.
||||||| merged common ancestors
JSObject*
GetUAWidgetScope(JSContext* cx, nsIPrincipal* principal)
{
    RootedObject scope(cx, XPCJSRuntime::Get()->GetUAWidgetScope(cx, principal));
    NS_ENSURE_TRUE(scope, nullptr); // See bug 858642.
=======
JSObject* GetUAWidgetScope(JSContext* cx, JSObject* contentScopeArg) {
  JS::RootedObject contentScope(cx, contentScopeArg);
  JSAutoRealm ar(cx, contentScope);
  nsIPrincipal* principal = GetObjectPrincipal(contentScope);
>>>>>>> upstream-releases

<<<<<<< HEAD
  scope = js::UncheckedUnwrap(scope);
  JS::ExposeObjectToActiveJS(scope);
  return scope;
}
||||||| merged common ancestors
    scope = js::UncheckedUnwrap(scope);
    JS::ExposeObjectToActiveJS(scope);
    return scope;
}
=======
  if (nsContentUtils::IsSystemPrincipal(principal)) {
    return JS::GetNonCCWObjectGlobal(contentScope);
  }
>>>>>>> upstream-releases

<<<<<<< HEAD
bool AllowContentXBLScope(JS::Realm* realm) {
  XPCWrappedNativeScope* scope = RealmPrivate::Get(realm)->scope;
  return scope && scope->AllowContentXBLScope();
||||||| merged common ancestors
bool
AllowContentXBLScope(JS::Realm* realm)
{
    XPCWrappedNativeScope* scope = RealmPrivate::Get(realm)->scope;
    return scope && scope->AllowContentXBLScope();
=======
  return GetUAWidgetScope(cx, principal);
>>>>>>> upstream-releases
}

<<<<<<< HEAD
bool UseContentXBLScope(JS::Realm* realm) {
  XPCWrappedNativeScope* scope = RealmPrivate::Get(realm)->scope;
  return scope && scope->UseContentXBLScope();
||||||| merged common ancestors
bool
UseContentXBLScope(JS::Realm* realm)
{
    XPCWrappedNativeScope* scope = RealmPrivate::Get(realm)->scope;
    return scope && scope->UseContentXBLScope();
=======
JSObject* GetUAWidgetScope(JSContext* cx, nsIPrincipal* principal) {
  RootedObject scope(cx, XPCJSRuntime::Get()->GetUAWidgetScope(cx, principal));
  NS_ENSURE_TRUE(scope, nullptr);  // See bug 858642.

  scope = js::UncheckedUnwrap(scope);
  JS::ExposeObjectToActiveJS(scope);
  return scope;
>>>>>>> upstream-releases
}

<<<<<<< HEAD
void ClearContentXBLScope(JSObject* global) {
  RealmPrivate::Get(global)->scope->ClearContentXBLScope();
||||||| merged common ancestors
void
ClearContentXBLScope(JSObject* global)
{
    RealmPrivate::Get(global)->scope->ClearContentXBLScope();
=======
bool AllowContentXBLScope(JS::Realm* realm) {
  JS::Compartment* comp = GetCompartmentForRealm(realm);
  XPCWrappedNativeScope* scope = CompartmentPrivate::Get(comp)->GetScope();
  MOZ_ASSERT(scope);
  return scope->AllowContentXBLScope(realm);
>>>>>>> upstream-releases
}

} /* namespace xpc */

XPCWrappedNativeScope::~XPCWrappedNativeScope() {
  MOZ_COUNT_DTOR(XPCWrappedNativeScope);

  // We can do additional cleanup assertions here...

  MOZ_ASSERT(0 == mWrappedNativeMap->Count(), "scope has non-empty map");
  delete mWrappedNativeMap;

  MOZ_ASSERT(0 == mWrappedNativeProtoMap->Count(), "scope has non-empty map");
  delete mWrappedNativeProtoMap;

  // This should not be necessary, since the Components object should die
  // with the scope but just in case.
  if (mComponents) {
    mComponents->mScope = nullptr;
  }

  // XXX we should assert that we are dead or that xpconnect has shutdown
  // XXX might not want to do this at xpconnect shutdown time???
  mComponents = nullptr;

<<<<<<< HEAD
  if (mXrayExpandos.initialized()) {
    mXrayExpandos.destroy();
  }
||||||| merged common ancestors
    if (mXrayExpandos.initialized()) {
        mXrayExpandos.destroy();
    }
=======
  MOZ_RELEASE_ASSERT(!mXrayExpandos.initialized());
>>>>>>> upstream-releases

<<<<<<< HEAD
  JSContext* cx = dom::danger::GetJSContext();
  mGlobalJSObject.finalize(cx);
}

// static
void XPCWrappedNativeScope::TraceWrappedNativesInAllScopes(JSTracer* trc) {
  // Do JS::TraceEdge for all wrapped natives with external references, as
  // well as any DOM expando objects.
  for (XPCWrappedNativeScope* cur = gScopes; cur; cur = cur->mNext) {
    for (auto i = cur->mWrappedNativeMap->Iter(); !i.Done(); i.Next()) {
      auto entry = static_cast<Native2WrappedNativeMap::Entry*>(i.Get());
      XPCWrappedNative* wrapper = entry->value;
      if (wrapper->HasExternalReference() && !wrapper->IsWrapperExpired()) {
        wrapper->TraceSelf(trc);
      }
    }
  }
||||||| merged common ancestors
    JSContext* cx = dom::danger::GetJSContext();
    mGlobalJSObject.finalize(cx);
}

// static
void
XPCWrappedNativeScope::TraceWrappedNativesInAllScopes(JSTracer* trc)
{
    // Do JS::TraceEdge for all wrapped natives with external references, as
    // well as any DOM expando objects.
    for (XPCWrappedNativeScope* cur = gScopes; cur; cur = cur->mNext) {
        for (auto i = cur->mWrappedNativeMap->Iter(); !i.Done(); i.Next()) {
            auto entry = static_cast<Native2WrappedNativeMap::Entry*>(i.Get());
            XPCWrappedNative* wrapper = entry->value;
            if (wrapper->HasExternalReference() && !wrapper->IsWrapperExpired()) {
                wrapper->TraceSelf(trc);
            }
        }
    }
=======
  mCompartment = nullptr;
>>>>>>> upstream-releases
}

// static
<<<<<<< HEAD
void XPCWrappedNativeScope::SuspectAllWrappers(
    nsCycleCollectionNoteRootCallback& cb) {
  for (XPCWrappedNativeScope* cur = gScopes; cur; cur = cur->mNext) {
    for (auto i = cur->mWrappedNativeMap->Iter(); !i.Done(); i.Next()) {
      static_cast<Native2WrappedNativeMap::Entry*>(i.Get())->value->Suspect(cb);
||||||| merged common ancestors
void
XPCWrappedNativeScope::SuspectAllWrappers(nsCycleCollectionNoteRootCallback& cb)
{
    for (XPCWrappedNativeScope* cur = gScopes; cur; cur = cur->mNext) {
        for (auto i = cur->mWrappedNativeMap->Iter(); !i.Done(); i.Next()) {
            static_cast<Native2WrappedNativeMap::Entry*>(i.Get())->value->Suspect(cb);
        }
=======
void XPCWrappedNativeScope::TraceWrappedNativesInAllScopes(XPCJSRuntime* xpcrt,
                                                           JSTracer* trc) {
  // Do JS::TraceEdge for all wrapped natives with external references, as
  // well as any DOM expando objects.
  //
  // Note: the GC can call this from a JS helper thread. We don't use
  // AllScopes() because that asserts we're on the main thread.

  for (XPCWrappedNativeScope* cur : xpcrt->GetWrappedNativeScopes()) {
    for (auto i = cur->mWrappedNativeMap->Iter(); !i.Done(); i.Next()) {
      auto entry = static_cast<Native2WrappedNativeMap::Entry*>(i.Get());
      XPCWrappedNative* wrapper = entry->value;
      if (wrapper->HasExternalReference() && !wrapper->IsWrapperExpired()) {
        wrapper->TraceSelf(trc);
      }
>>>>>>> upstream-releases
    }
  }
}

// static
<<<<<<< HEAD
void XPCWrappedNativeScope::UpdateWeakPointersInAllScopesAfterGC() {
  // If this is called from the finalization callback in JSGC_MARK_END then
  // JSGC_FINALIZE_END must always follow it calling
  // FinishedFinalizationPhaseOfGC and clearing gDyingScopes in
  // KillDyingScopes.
  MOZ_ASSERT(!gDyingScopes, "JSGC_MARK_END without JSGC_FINALIZE_END");

  XPCWrappedNativeScope** scopep = &gScopes;
  while (*scopep) {
    XPCWrappedNativeScope* cur = *scopep;
    cur->UpdateWeakPointersAfterGC();
    if (cur->mGlobalJSObject) {
      scopep = &cur->mNext;
    } else {
      // The scope's global is dead so move it to the dying scopes list.
      *scopep = cur->mNext;
      cur->mNext = gDyingScopes;
      gDyingScopes = cur;
||||||| merged common ancestors
void
XPCWrappedNativeScope::UpdateWeakPointersInAllScopesAfterGC()
{
    // If this is called from the finalization callback in JSGC_MARK_END then
    // JSGC_FINALIZE_END must always follow it calling
    // FinishedFinalizationPhaseOfGC and clearing gDyingScopes in
    // KillDyingScopes.
    MOZ_ASSERT(!gDyingScopes, "JSGC_MARK_END without JSGC_FINALIZE_END");

    XPCWrappedNativeScope** scopep = &gScopes;
    while (*scopep) {
        XPCWrappedNativeScope* cur = *scopep;
        cur->UpdateWeakPointersAfterGC();
        if (cur->mGlobalJSObject) {
            scopep = &cur->mNext;
        } else {
            // The scope's global is dead so move it to the dying scopes list.
            *scopep = cur->mNext;
            cur->mNext = gDyingScopes;
            gDyingScopes = cur;
        }
=======
void XPCWrappedNativeScope::SuspectAllWrappers(
    nsCycleCollectionNoteRootCallback& cb) {
  for (XPCWrappedNativeScope* cur : AllScopes()) {
    for (auto i = cur->mWrappedNativeMap->Iter(); !i.Done(); i.Next()) {
      static_cast<Native2WrappedNativeMap::Entry*>(i.Get())->value->Suspect(cb);
>>>>>>> upstream-releases
    }
  }
}

<<<<<<< HEAD
static inline void AssertSameCompartment(DebugOnly<JS::Compartment*>& comp,
                                         JSObject* obj) {
  MOZ_ASSERT_IF(obj, js::GetObjectCompartment(obj) == comp);
}

static inline void AssertSameCompartment(DebugOnly<JS::Compartment*>& comp,
                                         const JS::ObjectPtr& obj) {
#ifdef DEBUG
  AssertSameCompartment(comp, obj.unbarrieredGet());
#endif
}

void XPCWrappedNativeScope::UpdateWeakPointersAfterGC() {
  // Sweep waivers.
  if (mWaiverWrapperMap) {
    mWaiverWrapperMap->Sweep();
  }

  if (!js::IsObjectZoneSweepingOrCompacting(mGlobalJSObject.unbarrieredGet())) {
    return;
  }
||||||| merged common ancestors
static inline void
AssertSameCompartment(DebugOnly<JS::Compartment*>& comp, JSObject* obj)
{
    MOZ_ASSERT_IF(obj, js::GetObjectCompartment(obj) == comp);
}

static inline void
AssertSameCompartment(DebugOnly<JS::Compartment*>& comp, const JS::ObjectPtr& obj)
{
#ifdef DEBUG
    AssertSameCompartment(comp, obj.unbarrieredGet());
#endif
}

void
XPCWrappedNativeScope::UpdateWeakPointersAfterGC()
{
    // Sweep waivers.
    if (mWaiverWrapperMap) {
        mWaiverWrapperMap->Sweep();
    }

    if (!js::IsObjectZoneSweepingOrCompacting(mGlobalJSObject.unbarrieredGet())) {
        return;
    }
=======
void XPCWrappedNativeScope::UpdateWeakPointersAfterGC() {
  // Sweep waivers.
  if (mWaiverWrapperMap) {
    mWaiverWrapperMap->Sweep();
  }
>>>>>>> upstream-releases

<<<<<<< HEAD
  // Update our pointer to the global object in case it was moved or
  // finalized.
  mGlobalJSObject.updateWeakPointerAfterGC();
  if (!mGlobalJSObject) {
    JSContext* cx = dom::danger::GetJSContext();
    mContentXBLScope.finalize(cx);
    GetWrappedNativeMap()->Clear();
    mWrappedNativeProtoMap->Clear();
    return;
  }
||||||| merged common ancestors
    // Update our pointer to the global object in case it was moved or
    // finalized.
    mGlobalJSObject.updateWeakPointerAfterGC();
    if (!mGlobalJSObject) {
        JSContext* cx = dom::danger::GetJSContext();
        mContentXBLScope.finalize(cx);
        GetWrappedNativeMap()->Clear();
        mWrappedNativeProtoMap->Clear();
        return;
    }
=======
  if (!js::IsCompartmentZoneSweepingOrCompacting(mCompartment)) {
    return;
  }
>>>>>>> upstream-releases

<<<<<<< HEAD
  DebugOnly<JS::Compartment*> comp =
      js::GetObjectCompartment(mGlobalJSObject.unbarrieredGet());
||||||| merged common ancestors
    DebugOnly<JS::Compartment*> comp =
        js::GetObjectCompartment(mGlobalJSObject.unbarrieredGet());
=======
  if (!js::CompartmentHasLiveGlobal(mCompartment)) {
    GetWrappedNativeMap()->Clear();
    mWrappedNativeProtoMap->Clear();
>>>>>>> upstream-releases

<<<<<<< HEAD
#ifdef DEBUG
  // These are traced, so no updates are necessary.
  if (mContentXBLScope) {
    JSObject* prev = mContentXBLScope.unbarrieredGet();
    mContentXBLScope.updateWeakPointerAfterGC();
    MOZ_ASSERT(prev == mContentXBLScope.unbarrieredGet());
    AssertSameCompartment(comp, mContentXBLScope);
  }
#endif
||||||| merged common ancestors
#ifdef DEBUG
    // These are traced, so no updates are necessary.
    if (mContentXBLScope) {
        JSObject* prev = mContentXBLScope.unbarrieredGet();
        mContentXBLScope.updateWeakPointerAfterGC();
        MOZ_ASSERT(prev == mContentXBLScope.unbarrieredGet());
        AssertSameCompartment(comp, mContentXBLScope);
    }
#endif
=======
    // The fields below are traced only if there's a live global in the
    // compartment, see TraceXPCGlobal. The compartment has no live globals so
    // clear these pointers here.
    if (mXrayExpandos.initialized()) {
      mXrayExpandos.destroy();
    }
    mIDProto = nullptr;
    mIIDProto = nullptr;
    mCIDProto = nullptr;
    return;
  }
>>>>>>> upstream-releases

<<<<<<< HEAD
  // Sweep mWrappedNativeMap for dying flat JS objects. Moving has already
  // been handled by XPCWrappedNative::FlatJSObjectMoved.
  for (auto iter = GetWrappedNativeMap()->Iter(); !iter.Done(); iter.Next()) {
    auto entry = static_cast<Native2WrappedNativeMap::Entry*>(iter.Get());
    XPCWrappedNative* wrapper = entry->value;
    JSObject* obj = wrapper->GetFlatJSObjectPreserveColor();
    JS_UpdateWeakPointerAfterGCUnbarriered(&obj);
    MOZ_ASSERT(!obj || obj == wrapper->GetFlatJSObjectPreserveColor());
    AssertSameCompartment(comp, obj);
    if (!obj) {
      iter.Remove();
||||||| merged common ancestors
    // Sweep mWrappedNativeMap for dying flat JS objects. Moving has already
    // been handled by XPCWrappedNative::FlatJSObjectMoved.
    for (auto iter = GetWrappedNativeMap()->Iter(); !iter.Done(); iter.Next()) {
        auto entry = static_cast<Native2WrappedNativeMap::Entry*>(iter.Get());
        XPCWrappedNative* wrapper = entry->value;
        JSObject* obj = wrapper->GetFlatJSObjectPreserveColor();
        JS_UpdateWeakPointerAfterGCUnbarriered(&obj);
        MOZ_ASSERT(!obj || obj == wrapper->GetFlatJSObjectPreserveColor());
        AssertSameCompartment(comp, obj);
        if (!obj) {
            iter.Remove();
        }
=======
  // Sweep mWrappedNativeMap for dying flat JS objects. Moving has already
  // been handled by XPCWrappedNative::FlatJSObjectMoved.
  for (auto iter = GetWrappedNativeMap()->Iter(); !iter.Done(); iter.Next()) {
    auto entry = static_cast<Native2WrappedNativeMap::Entry*>(iter.Get());
    XPCWrappedNative* wrapper = entry->value;
    JSObject* obj = wrapper->GetFlatJSObjectPreserveColor();
    JS_UpdateWeakPointerAfterGCUnbarriered(&obj);
    MOZ_ASSERT(!obj || obj == wrapper->GetFlatJSObjectPreserveColor());
    MOZ_ASSERT_IF(obj, js::GetObjectCompartment(obj) == mCompartment);
    if (!obj) {
      iter.Remove();
>>>>>>> upstream-releases
    }
  }

<<<<<<< HEAD
  // Sweep mWrappedNativeProtoMap for dying prototype JSObjects. Moving has
  // already been handled by XPCWrappedNativeProto::JSProtoObjectMoved.
  for (auto i = mWrappedNativeProtoMap->Iter(); !i.Done(); i.Next()) {
    auto entry = static_cast<ClassInfo2WrappedNativeProtoMap::Entry*>(i.Get());
    JSObject* obj = entry->value->GetJSProtoObjectPreserveColor();
    JS_UpdateWeakPointerAfterGCUnbarriered(&obj);
    AssertSameCompartment(comp, obj);
    MOZ_ASSERT(!obj || obj == entry->value->GetJSProtoObjectPreserveColor());
    if (!obj) {
      i.Remove();
||||||| merged common ancestors
    // Sweep mWrappedNativeProtoMap for dying prototype JSObjects. Moving has
    // already been handled by XPCWrappedNativeProto::JSProtoObjectMoved.
    for (auto i = mWrappedNativeProtoMap->Iter(); !i.Done(); i.Next()) {
        auto entry = static_cast<ClassInfo2WrappedNativeProtoMap::Entry*>(i.Get());
        JSObject* obj = entry->value->GetJSProtoObjectPreserveColor();
        JS_UpdateWeakPointerAfterGCUnbarriered(&obj);
        AssertSameCompartment(comp, obj);
        MOZ_ASSERT(!obj || obj == entry->value->GetJSProtoObjectPreserveColor());
        if (!obj) {
            i.Remove();
        }
=======
  // Sweep mWrappedNativeProtoMap for dying prototype JSObjects. Moving has
  // already been handled by XPCWrappedNativeProto::JSProtoObjectMoved.
  for (auto i = mWrappedNativeProtoMap->Iter(); !i.Done(); i.Next()) {
    auto entry = static_cast<ClassInfo2WrappedNativeProtoMap::Entry*>(i.Get());
    JSObject* obj = entry->value->GetJSProtoObjectPreserveColor();
    JS_UpdateWeakPointerAfterGCUnbarriered(&obj);
    MOZ_ASSERT_IF(obj, js::GetObjectCompartment(obj) == mCompartment);
    MOZ_ASSERT(!obj || obj == entry->value->GetJSProtoObjectPreserveColor());
    if (!obj) {
      i.Remove();
>>>>>>> upstream-releases
    }
  }
}

// static
<<<<<<< HEAD
void XPCWrappedNativeScope::SweepAllWrappedNativeTearOffs() {
  for (XPCWrappedNativeScope* cur = gScopes; cur; cur = cur->mNext) {
    for (auto i = cur->mWrappedNativeMap->Iter(); !i.Done(); i.Next()) {
      auto entry = static_cast<Native2WrappedNativeMap::Entry*>(i.Get());
      entry->value->SweepTearOffs();
||||||| merged common ancestors
void
XPCWrappedNativeScope::SweepAllWrappedNativeTearOffs()
{
    for (XPCWrappedNativeScope* cur = gScopes; cur; cur = cur->mNext) {
        for (auto i = cur->mWrappedNativeMap->Iter(); !i.Done(); i.Next()) {
            auto entry = static_cast<Native2WrappedNativeMap::Entry*>(i.Get());
            entry->value->SweepTearOffs();
        }
=======
void XPCWrappedNativeScope::SweepAllWrappedNativeTearOffs() {
  for (XPCWrappedNativeScope* cur : AllScopes()) {
    for (auto i = cur->mWrappedNativeMap->Iter(); !i.Done(); i.Next()) {
      auto entry = static_cast<Native2WrappedNativeMap::Entry*>(i.Get());
      entry->value->SweepTearOffs();
>>>>>>> upstream-releases
    }
  }
}

// static
<<<<<<< HEAD
void XPCWrappedNativeScope::KillDyingScopes() {
  XPCWrappedNativeScope* cur = gDyingScopes;
  while (cur) {
    XPCWrappedNativeScope* next = cur->mNext;
    if (cur->mGlobalJSObject) {
      RealmPrivate::Get(cur->mGlobalJSObject)->scope = nullptr;
    }
    delete cur;
    cur = next;
  }
  gDyingScopes = nullptr;
}

// static
void XPCWrappedNativeScope::SystemIsBeingShutDown() {
  int liveScopeCount = 0;

  XPCWrappedNativeScope* cur;

  // First move all the scopes to the dying list.

  cur = gScopes;
  while (cur) {
    XPCWrappedNativeScope* next = cur->mNext;
    cur->mNext = gDyingScopes;
    gDyingScopes = cur;
    cur = next;
    liveScopeCount++;
  }
  gScopes = nullptr;

  // We're forcibly killing scopes, rather than allowing them to go away
  // when they're ready. As such, we need to do some cleanup before they
  // can safely be destroyed.

  for (cur = gDyingScopes; cur; cur = cur->mNext) {
    // Give the Components object a chance to try to clean up.
    if (cur->mComponents) {
      cur->mComponents->SystemIsBeingShutDown();
    }

    // Walk the protos first. Wrapper shutdown can leave dangling
    // proto pointers in the proto map.
    for (auto i = cur->mWrappedNativeProtoMap->Iter(); !i.Done(); i.Next()) {
      auto entry =
          static_cast<ClassInfo2WrappedNativeProtoMap::Entry*>(i.Get());
      entry->value->SystemIsBeingShutDown();
      i.Remove();
    }
    for (auto i = cur->mWrappedNativeMap->Iter(); !i.Done(); i.Next()) {
      auto entry = static_cast<Native2WrappedNativeMap::Entry*>(i.Get());
      XPCWrappedNative* wrapper = entry->value;
      if (wrapper->IsValid()) {
        wrapper->SystemIsBeingShutDown();
      }
      i.Remove();
    }

    CompartmentPrivate* priv = CompartmentPrivate::Get(cur->Compartment());
    priv->SystemIsBeingShutDown();
  }
||||||| merged common ancestors
void
XPCWrappedNativeScope::KillDyingScopes()
{
    XPCWrappedNativeScope* cur = gDyingScopes;
    while (cur) {
        XPCWrappedNativeScope* next = cur->mNext;
        if (cur->mGlobalJSObject) {
            RealmPrivate::Get(cur->mGlobalJSObject)->scope = nullptr;
        }
        delete cur;
        cur = next;
    }
    gDyingScopes = nullptr;
}

//static
void
XPCWrappedNativeScope::SystemIsBeingShutDown()
{
    int liveScopeCount = 0;

    XPCWrappedNativeScope* cur;

    // First move all the scopes to the dying list.

    cur = gScopes;
    while (cur) {
        XPCWrappedNativeScope* next = cur->mNext;
        cur->mNext = gDyingScopes;
        gDyingScopes = cur;
        cur = next;
        liveScopeCount++;
    }
    gScopes = nullptr;

    // We're forcibly killing scopes, rather than allowing them to go away
    // when they're ready. As such, we need to do some cleanup before they
    // can safely be destroyed.

    for (cur = gDyingScopes; cur; cur = cur->mNext) {
        // Give the Components object a chance to try to clean up.
        if (cur->mComponents) {
            cur->mComponents->SystemIsBeingShutDown();
        }

        // Walk the protos first. Wrapper shutdown can leave dangling
        // proto pointers in the proto map.
        for (auto i = cur->mWrappedNativeProtoMap->Iter(); !i.Done(); i.Next()) {
            auto entry = static_cast<ClassInfo2WrappedNativeProtoMap::Entry*>(i.Get());
            entry->value->SystemIsBeingShutDown();
            i.Remove();
        }
        for (auto i = cur->mWrappedNativeMap->Iter(); !i.Done(); i.Next()) {
            auto entry = static_cast<Native2WrappedNativeMap::Entry*>(i.Get());
            XPCWrappedNative* wrapper = entry->value;
            if (wrapper->IsValid()) {
                wrapper->SystemIsBeingShutDown();
            }
            i.Remove();
        }

        CompartmentPrivate* priv = CompartmentPrivate::Get(cur->Compartment());
        priv->SystemIsBeingShutDown();
    }

    // Now it is safe to kill all the scopes.
    KillDyingScopes();
}
=======
void XPCWrappedNativeScope::SystemIsBeingShutDown() {
  // We're forcibly killing scopes, rather than allowing them to go away
  // when they're ready. As such, we need to do some cleanup before they
  // can safely be destroyed.

  for (XPCWrappedNativeScope* cur : AllScopes()) {
    // Give the Components object a chance to try to clean up.
    if (cur->mComponents) {
      cur->mComponents->SystemIsBeingShutDown();
    }

    // Null out these pointers to prevent ~ObjectPtr assertion failures if we
    // leaked things at shutdown.
    cur->mIDProto = nullptr;
    cur->mIIDProto = nullptr;
    cur->mCIDProto = nullptr;

    // Similarly, destroy mXrayExpandos to prevent assertion failures.
    if (cur->mXrayExpandos.initialized()) {
      cur->mXrayExpandos.destroy();
    }

    // Walk the protos first. Wrapper shutdown can leave dangling
    // proto pointers in the proto map.
    for (auto i = cur->mWrappedNativeProtoMap->Iter(); !i.Done(); i.Next()) {
      auto entry =
          static_cast<ClassInfo2WrappedNativeProtoMap::Entry*>(i.Get());
      entry->value->SystemIsBeingShutDown();
      i.Remove();
    }
    for (auto i = cur->mWrappedNativeMap->Iter(); !i.Done(); i.Next()) {
      auto entry = static_cast<Native2WrappedNativeMap::Entry*>(i.Get());
      XPCWrappedNative* wrapper = entry->value;
      if (wrapper->IsValid()) {
        wrapper->SystemIsBeingShutDown();
      }
      i.Remove();
    }

    CompartmentPrivate* priv = CompartmentPrivate::Get(cur->Compartment());
    priv->SystemIsBeingShutDown();
  }
}
>>>>>>> upstream-releases

<<<<<<< HEAD
  // Now it is safe to kill all the scopes.
  KillDyingScopes();
}

||||||| merged common ancestors

=======
>>>>>>> upstream-releases
/***************************************************************************/

JSObject* XPCWrappedNativeScope::GetExpandoChain(HandleObject target) {
  MOZ_ASSERT(ObjectScope(target) == this);
  if (!mXrayExpandos.initialized()) {
    return nullptr;
  }
  return mXrayExpandos.lookup(target);
}

<<<<<<< HEAD
JSObject* XPCWrappedNativeScope::DetachExpandoChain(HandleObject target) {
  MOZ_ASSERT(ObjectScope(target) == this);
  if (!mXrayExpandos.initialized()) {
    return nullptr;
  }
  return mXrayExpandos.removeValue(target);
||||||| merged common ancestors
JSObject*
XPCWrappedNativeScope::DetachExpandoChain(HandleObject target)
{
    MOZ_ASSERT(ObjectScope(target) == this);
    if (!mXrayExpandos.initialized()) {
        return nullptr;
    }
    return mXrayExpandos.removeValue(target);
}

bool
XPCWrappedNativeScope::SetExpandoChain(JSContext* cx, HandleObject target,
                                       HandleObject chain)
{
    MOZ_ASSERT(ObjectScope(target) == this);
    MOZ_ASSERT(js::IsObjectInContextCompartment(target, cx));
    MOZ_ASSERT_IF(chain, ObjectScope(chain) == this);
    if (!mXrayExpandos.initialized() && !mXrayExpandos.init(cx)) {
        return false;
    }
    return mXrayExpandos.put(cx, target, chain);
=======
JSObject* XPCWrappedNativeScope::DetachExpandoChain(HandleObject target) {
  MOZ_ASSERT(ObjectScope(target) == this);
  if (!mXrayExpandos.initialized()) {
    return nullptr;
  }
  return mXrayExpandos.removeValue(target);
}

bool XPCWrappedNativeScope::SetExpandoChain(JSContext* cx, HandleObject target,
                                            HandleObject chain) {
  MOZ_ASSERT(ObjectScope(target) == this);
  MOZ_ASSERT(js::IsObjectInContextCompartment(target, cx));
  MOZ_ASSERT_IF(chain, ObjectScope(chain) == this);
  if (!mXrayExpandos.initialized() && !mXrayExpandos.init(cx)) {
    return false;
  }
  return mXrayExpandos.put(cx, target, chain);
>>>>>>> upstream-releases
}

<<<<<<< HEAD
bool XPCWrappedNativeScope::SetExpandoChain(JSContext* cx, HandleObject target,
                                            HandleObject chain) {
  MOZ_ASSERT(ObjectScope(target) == this);
  MOZ_ASSERT(js::IsObjectInContextCompartment(target, cx));
  MOZ_ASSERT_IF(chain, ObjectScope(chain) == this);
  if (!mXrayExpandos.initialized() && !mXrayExpandos.init(cx)) {
    return false;
  }
  return mXrayExpandos.put(cx, target, chain);
}

||||||| merged common ancestors

=======
>>>>>>> upstream-releases
/***************************************************************************/

// static
void XPCWrappedNativeScope::DebugDumpAllScopes(int16_t depth) {
#ifdef DEBUG
  depth--;

<<<<<<< HEAD
  // get scope count.
  int count = 0;
  XPCWrappedNativeScope* cur;
  for (cur = gScopes; cur; cur = cur->mNext) {
    count++;
  }

  XPC_LOG_ALWAYS(("chain of %d XPCWrappedNativeScope(s)", count));
  XPC_LOG_INDENT();
  XPC_LOG_ALWAYS(("gDyingScopes @ %p", gDyingScopes));
  if (depth) {
    for (cur = gScopes; cur; cur = cur->mNext) {
      cur->DebugDump(depth);
    }
  }
  XPC_LOG_OUTDENT();
||||||| merged common ancestors
    // get scope count.
    int count = 0;
    XPCWrappedNativeScope* cur;
    for (cur = gScopes; cur; cur = cur->mNext) {
        count++ ;
    }

    XPC_LOG_ALWAYS(("chain of %d XPCWrappedNativeScope(s)", count));
    XPC_LOG_INDENT();
        XPC_LOG_ALWAYS(("gDyingScopes @ %p", gDyingScopes));
        if (depth) {
            for (cur = gScopes; cur; cur = cur->mNext) {
                cur->DebugDump(depth);
            }
        }
    XPC_LOG_OUTDENT();
=======
  // get scope count.
  int count = 0;
  for (XPCWrappedNativeScope* cur : AllScopes()) {
    mozilla::Unused << cur;
    count++;
  }

  XPC_LOG_ALWAYS(("chain of %d XPCWrappedNativeScope(s)", count));
  XPC_LOG_INDENT();
  if (depth) {
    for (XPCWrappedNativeScope* cur : AllScopes()) {
      cur->DebugDump(depth);
    }
  }
  XPC_LOG_OUTDENT();
>>>>>>> upstream-releases
#endif
}

void XPCWrappedNativeScope::DebugDump(int16_t depth) {
#ifdef DEBUG
<<<<<<< HEAD
  depth--;
  XPC_LOG_ALWAYS(("XPCWrappedNativeScope @ %p", this));
  XPC_LOG_INDENT();
  XPC_LOG_ALWAYS(("mNext @ %p", mNext));
  XPC_LOG_ALWAYS(("mComponents @ %p", mComponents.get()));
  XPC_LOG_ALWAYS(("mGlobalJSObject @ %p", mGlobalJSObject.get()));

  XPC_LOG_ALWAYS(("mWrappedNativeMap @ %p with %d wrappers(s)",
                  mWrappedNativeMap, mWrappedNativeMap->Count()));
  // iterate contexts...
  if (depth && mWrappedNativeMap->Count()) {
||||||| merged common ancestors
    depth-- ;
    XPC_LOG_ALWAYS(("XPCWrappedNativeScope @ %p", this));
=======
  depth--;
  XPC_LOG_ALWAYS(("XPCWrappedNativeScope @ %p", this));
  XPC_LOG_INDENT();
  XPC_LOG_ALWAYS(("next @ %p", getNext()));
  XPC_LOG_ALWAYS(("mComponents @ %p", mComponents.get()));
  XPC_LOG_ALWAYS(("mCompartment @ %p", mCompartment));

  XPC_LOG_ALWAYS(("mWrappedNativeMap @ %p with %d wrappers(s)",
                  mWrappedNativeMap, mWrappedNativeMap->Count()));
  // iterate contexts...
  if (depth && mWrappedNativeMap->Count()) {
>>>>>>> upstream-releases
    XPC_LOG_INDENT();
    for (auto i = mWrappedNativeMap->Iter(); !i.Done(); i.Next()) {
      auto entry = static_cast<Native2WrappedNativeMap::Entry*>(i.Get());
      entry->value->DebugDump(depth);
    }
    XPC_LOG_OUTDENT();
<<<<<<< HEAD
  }

  XPC_LOG_ALWAYS(("mWrappedNativeProtoMap @ %p with %d protos(s)",
                  mWrappedNativeProtoMap, mWrappedNativeProtoMap->Count()));
  // iterate contexts...
  if (depth && mWrappedNativeProtoMap->Count()) {
    XPC_LOG_INDENT();
    for (auto i = mWrappedNativeProtoMap->Iter(); !i.Done(); i.Next()) {
      auto entry =
          static_cast<ClassInfo2WrappedNativeProtoMap::Entry*>(i.Get());
      entry->value->DebugDump(depth);
    }
    XPC_LOG_OUTDENT();
  }
  XPC_LOG_OUTDENT();
#endif
}
||||||| merged common ancestors
#endif
}
=======
  }
>>>>>>> upstream-releases

<<<<<<< HEAD
void XPCWrappedNativeScope::AddSizeOfAllScopesIncludingThis(
    ScopeSizeInfo* scopeSizeInfo) {
  for (XPCWrappedNativeScope* cur = gScopes; cur; cur = cur->mNext) {
    cur->AddSizeOfIncludingThis(scopeSizeInfo);
  }
||||||| merged common ancestors
void
XPCWrappedNativeScope::AddSizeOfAllScopesIncludingThis(ScopeSizeInfo* scopeSizeInfo)
{
    for (XPCWrappedNativeScope* cur = gScopes; cur; cur = cur->mNext) {
        cur->AddSizeOfIncludingThis(scopeSizeInfo);
    }
=======
  XPC_LOG_ALWAYS(("mWrappedNativeProtoMap @ %p with %d protos(s)",
                  mWrappedNativeProtoMap, mWrappedNativeProtoMap->Count()));
  // iterate contexts...
  if (depth && mWrappedNativeProtoMap->Count()) {
    XPC_LOG_INDENT();
    for (auto i = mWrappedNativeProtoMap->Iter(); !i.Done(); i.Next()) {
      auto entry =
          static_cast<ClassInfo2WrappedNativeProtoMap::Entry*>(i.Get());
      entry->value->DebugDump(depth);
    }
    XPC_LOG_OUTDENT();
  }
  XPC_LOG_OUTDENT();
#endif
>>>>>>> upstream-releases
}

<<<<<<< HEAD
void XPCWrappedNativeScope::AddSizeOfIncludingThis(
    ScopeSizeInfo* scopeSizeInfo) {
  scopeSizeInfo->mScopeAndMapSize += scopeSizeInfo->mMallocSizeOf(this);
  scopeSizeInfo->mScopeAndMapSize +=
      mWrappedNativeMap->SizeOfIncludingThis(scopeSizeInfo->mMallocSizeOf);
  scopeSizeInfo->mScopeAndMapSize +=
      mWrappedNativeProtoMap->SizeOfIncludingThis(scopeSizeInfo->mMallocSizeOf);

  if (dom::HasProtoAndIfaceCache(mGlobalJSObject)) {
    dom::ProtoAndIfaceCache* cache =
        dom::GetProtoAndIfaceCache(mGlobalJSObject);
    scopeSizeInfo->mProtoAndIfaceCacheSize +=
        cache->SizeOfIncludingThis(scopeSizeInfo->mMallocSizeOf);
  }
||||||| merged common ancestors
void
XPCWrappedNativeScope::AddSizeOfIncludingThis(ScopeSizeInfo* scopeSizeInfo)
{
    scopeSizeInfo->mScopeAndMapSize += scopeSizeInfo->mMallocSizeOf(this);
    scopeSizeInfo->mScopeAndMapSize +=
        mWrappedNativeMap->SizeOfIncludingThis(scopeSizeInfo->mMallocSizeOf);
    scopeSizeInfo->mScopeAndMapSize +=
        mWrappedNativeProtoMap->SizeOfIncludingThis(scopeSizeInfo->mMallocSizeOf);

    if (dom::HasProtoAndIfaceCache(mGlobalJSObject)) {
        dom::ProtoAndIfaceCache* cache = dom::GetProtoAndIfaceCache(mGlobalJSObject);
        scopeSizeInfo->mProtoAndIfaceCacheSize +=
            cache->SizeOfIncludingThis(scopeSizeInfo->mMallocSizeOf);
    }
=======
void XPCWrappedNativeScope::AddSizeOfAllScopesIncludingThis(
    JSContext* cx, ScopeSizeInfo* scopeSizeInfo) {
  for (XPCWrappedNativeScope* cur : AllScopes()) {
    cur->AddSizeOfIncludingThis(cx, scopeSizeInfo);
  }
}
>>>>>>> upstream-releases

<<<<<<< HEAD
  // There are other XPCWrappedNativeScope members that could be measured;
  // the above ones have been seen by DMD to be worth measuring.  More stuff
  // may be added later.
||||||| merged common ancestors
    // There are other XPCWrappedNativeScope members that could be measured;
    // the above ones have been seen by DMD to be worth measuring.  More stuff
    // may be added later.
=======
void XPCWrappedNativeScope::AddSizeOfIncludingThis(
    JSContext* cx, ScopeSizeInfo* scopeSizeInfo) {
  scopeSizeInfo->mScopeAndMapSize += scopeSizeInfo->mMallocSizeOf(this);
  scopeSizeInfo->mScopeAndMapSize +=
      mWrappedNativeMap->SizeOfIncludingThis(scopeSizeInfo->mMallocSizeOf);
  scopeSizeInfo->mScopeAndMapSize +=
      mWrappedNativeProtoMap->SizeOfIncludingThis(scopeSizeInfo->mMallocSizeOf);

  auto realmCb = [](JSContext*, void* aData, JS::Handle<JS::Realm*> aRealm) {
    auto* scopeSizeInfo = static_cast<ScopeSizeInfo*>(aData);
    JSObject* global = GetRealmGlobalOrNull(aRealm);
    if (global && dom::HasProtoAndIfaceCache(global)) {
      dom::ProtoAndIfaceCache* cache = dom::GetProtoAndIfaceCache(global);
      scopeSizeInfo->mProtoAndIfaceCacheSize +=
          cache->SizeOfIncludingThis(scopeSizeInfo->mMallocSizeOf);
    }
  };
  IterateRealmsInCompartment(cx, Compartment(), scopeSizeInfo, realmCb);

  // There are other XPCWrappedNativeScope members that could be measured;
  // the above ones have been seen by DMD to be worth measuring.  More stuff
  // may be added later.
>>>>>>> upstream-releases
}
