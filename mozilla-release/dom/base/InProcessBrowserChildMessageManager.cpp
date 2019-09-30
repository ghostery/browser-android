/* -*- Mode: C++; tab-width: 8; indent-tabs-mode: nil; c-basic-offset: 2 -*- */
/* vim: set ts=8 sts=2 et sw=2 tw=80: */
/* This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/. */

#include "InProcessBrowserChildMessageManager.h"
#include "nsContentUtils.h"
#include "nsDocShell.h"
#include "nsIScriptSecurityManager.h"
#include "nsIInterfaceRequestorUtils.h"
#include "nsIComponentManager.h"
#include "nsIServiceManager.h"
#include "nsComponentManagerUtils.h"
#include "nsFrameLoader.h"
#include "nsFrameLoaderOwner.h"
#include "nsQueryObject.h"
#include "xpcpublic.h"
#include "nsIMozBrowserFrame.h"
#include "mozilla/EventDispatcher.h"
#include "mozilla/dom/ChromeMessageSender.h"
#include "mozilla/dom/MessageManagerBinding.h"
#include "mozilla/dom/SameProcessMessageQueue.h"
#include "mozilla/dom/ScriptLoader.h"
#include "mozilla/dom/WindowProxyHolder.h"

using namespace mozilla;
using namespace mozilla::dom;
using namespace mozilla::dom::ipc;

<<<<<<< HEAD:mozilla-release/dom/base/InProcessTabChildMessageManager.cpp
bool InProcessTabChildMessageManager::DoSendBlockingMessage(
    JSContext* aCx, const nsAString& aMessage, StructuredCloneData& aData,
    JS::Handle<JSObject*> aCpows, nsIPrincipal* aPrincipal,
    nsTArray<StructuredCloneData>* aRetVal, bool aIsSync) {
||||||| merged common ancestors
bool
InProcessTabChildMessageManager::DoSendBlockingMessage(JSContext* aCx,
                                                       const nsAString& aMessage,
                                                       StructuredCloneData& aData,
                                                       JS::Handle<JSObject *> aCpows,
                                                       nsIPrincipal* aPrincipal,
                                                       nsTArray<StructuredCloneData>* aRetVal,
                                                       bool aIsSync)
{
=======
bool InProcessBrowserChildMessageManager::DoSendBlockingMessage(
    JSContext* aCx, const nsAString& aMessage, StructuredCloneData& aData,
    JS::Handle<JSObject*> aCpows, nsIPrincipal* aPrincipal,
    nsTArray<StructuredCloneData>* aRetVal, bool aIsSync) {
>>>>>>> upstream-releases:mozilla-release/dom/base/InProcessBrowserChildMessageManager.cpp
  SameProcessMessageQueue* queue = SameProcessMessageQueue::Get();
  queue->Flush();

  if (mChromeMessageManager) {
    SameProcessCpowHolder cpows(JS::RootingContext::get(aCx), aCpows);
    RefPtr<nsFrameMessageManager> mm = mChromeMessageManager;
    RefPtr<nsFrameLoader> fl = GetFrameLoader();
    mm->ReceiveMessage(mOwner, fl, aMessage, true, &aData, &cpows, aPrincipal,
                       aRetVal, IgnoreErrors());
  }
  return true;
}

class nsAsyncMessageToParent : public nsSameProcessAsyncMessageBase,
                               public SameProcessMessageQueue::Runnable {
 public:
  nsAsyncMessageToParent(JS::RootingContext* aRootingCx,
                         JS::Handle<JSObject*> aCpows,
<<<<<<< HEAD:mozilla-release/dom/base/InProcessTabChildMessageManager.cpp
                         InProcessTabChildMessageManager* aTabChild)
      : nsSameProcessAsyncMessageBase(aRootingCx, aCpows),
        mTabChild(aTabChild) {}

  virtual nsresult HandleMessage() override {
    RefPtr<nsFrameLoader> fl = mTabChild->GetFrameLoader();
    ReceiveMessage(mTabChild->mOwner, fl, mTabChild->mChromeMessageManager);
||||||| merged common ancestors
                         InProcessTabChildMessageManager* aTabChild)
    : nsSameProcessAsyncMessageBase(aRootingCx, aCpows)
    , mTabChild(aTabChild)
  { }

  virtual nsresult HandleMessage() override
  {
    RefPtr<nsFrameLoader> fl = mTabChild->GetFrameLoader();
    ReceiveMessage(mTabChild->mOwner, fl, mTabChild->mChromeMessageManager);
=======
                         InProcessBrowserChildMessageManager* aBrowserChild)
      : nsSameProcessAsyncMessageBase(aRootingCx, aCpows),
        mBrowserChild(aBrowserChild) {}

  virtual nsresult HandleMessage() override {
    RefPtr<nsFrameLoader> fl = mBrowserChild->GetFrameLoader();
    ReceiveMessage(mBrowserChild->mOwner, fl,
                   mBrowserChild->mChromeMessageManager);
>>>>>>> upstream-releases:mozilla-release/dom/base/InProcessBrowserChildMessageManager.cpp
    return NS_OK;
  }
  RefPtr<InProcessBrowserChildMessageManager> mBrowserChild;
};

<<<<<<< HEAD:mozilla-release/dom/base/InProcessTabChildMessageManager.cpp
nsresult InProcessTabChildMessageManager::DoSendAsyncMessage(
    JSContext* aCx, const nsAString& aMessage, StructuredCloneData& aData,
    JS::Handle<JSObject*> aCpows, nsIPrincipal* aPrincipal) {
||||||| merged common ancestors
nsresult
InProcessTabChildMessageManager::DoSendAsyncMessage(JSContext* aCx,
                                                    const nsAString& aMessage,
                                                    StructuredCloneData& aData,
                                                    JS::Handle<JSObject *> aCpows,
                                                    nsIPrincipal* aPrincipal)
{
=======
nsresult InProcessBrowserChildMessageManager::DoSendAsyncMessage(
    JSContext* aCx, const nsAString& aMessage, StructuredCloneData& aData,
    JS::Handle<JSObject*> aCpows, nsIPrincipal* aPrincipal) {
>>>>>>> upstream-releases:mozilla-release/dom/base/InProcessBrowserChildMessageManager.cpp
  SameProcessMessageQueue* queue = SameProcessMessageQueue::Get();
  JS::RootingContext* rcx = JS::RootingContext::get(aCx);
  RefPtr<nsAsyncMessageToParent> ev =
      new nsAsyncMessageToParent(rcx, aCpows, this);

  nsresult rv = ev->Init(aMessage, aData, aPrincipal);
  if (NS_FAILED(rv)) {
    return rv;
  }

  queue->Push(ev);
  return NS_OK;
}

<<<<<<< HEAD:mozilla-release/dom/base/InProcessTabChildMessageManager.cpp
InProcessTabChildMessageManager::InProcessTabChildMessageManager(
    nsIDocShell* aShell, nsIContent* aOwner, nsFrameMessageManager* aChrome)
    : ContentFrameMessageManager(new nsFrameMessageManager(this)),
      mDocShell(aShell),
      mLoadingScript(false),
      mPreventEventsEscaping(false),
      mOwner(aOwner),
      mChromeMessageManager(aChrome) {
||||||| merged common ancestors
InProcessTabChildMessageManager::InProcessTabChildMessageManager(nsIDocShell* aShell,
                                                                 nsIContent* aOwner,
                                                                 nsFrameMessageManager* aChrome)
: ContentFrameMessageManager(new nsFrameMessageManager(this)),
  mDocShell(aShell), mLoadingScript(false),
  mPreventEventsEscaping(false),
  mOwner(aOwner), mChromeMessageManager(aChrome)
{
=======
InProcessBrowserChildMessageManager::InProcessBrowserChildMessageManager(
    nsDocShell* aShell, nsIContent* aOwner, nsFrameMessageManager* aChrome)
    : ContentFrameMessageManager(new nsFrameMessageManager(this)),
      mDocShell(aShell),
      mLoadingScript(false),
      mPreventEventsEscaping(false),
      mOwner(aOwner),
      mChromeMessageManager(aChrome) {
>>>>>>> upstream-releases:mozilla-release/dom/base/InProcessBrowserChildMessageManager.cpp
  mozilla::HoldJSObjects(this);

  // If owner corresponds to an <iframe mozbrowser>, we'll have to tweak our
  // GetEventTargetParent implementation.
  nsCOMPtr<nsIMozBrowserFrame> browserFrame = do_QueryInterface(mOwner);
  if (browserFrame) {
    mIsBrowserFrame = browserFrame->GetReallyIsBrowser();
  } else {
    mIsBrowserFrame = false;
  }
}

<<<<<<< HEAD:mozilla-release/dom/base/InProcessTabChildMessageManager.cpp
InProcessTabChildMessageManager::~InProcessTabChildMessageManager() {
||||||| merged common ancestors
InProcessTabChildMessageManager::~InProcessTabChildMessageManager()
{
=======
InProcessBrowserChildMessageManager::~InProcessBrowserChildMessageManager() {
>>>>>>> upstream-releases:mozilla-release/dom/base/InProcessBrowserChildMessageManager.cpp
  mAnonymousGlobalScopes.Clear();
  mozilla::DropJSObjects(this);
}

// This method isn't automatically forwarded safely because it's notxpcom, so
// the IDL binding doesn't know what value to return.
<<<<<<< HEAD:mozilla-release/dom/base/InProcessTabChildMessageManager.cpp
void InProcessTabChildMessageManager::MarkForCC() {
||||||| merged common ancestors
void
InProcessTabChildMessageManager::MarkForCC()
{
=======
void InProcessBrowserChildMessageManager::MarkForCC() {
>>>>>>> upstream-releases:mozilla-release/dom/base/InProcessBrowserChildMessageManager.cpp
  MarkScopesForCC();
  MessageManagerGlobal::MarkForCC();
}

NS_IMPL_CYCLE_COLLECTION_CLASS(InProcessBrowserChildMessageManager)

<<<<<<< HEAD:mozilla-release/dom/base/InProcessTabChildMessageManager.cpp
NS_IMPL_CYCLE_COLLECTION_TRAVERSE_BEGIN_INHERITED(
    InProcessTabChildMessageManager, DOMEventTargetHelper)
  NS_IMPL_CYCLE_COLLECTION_TRAVERSE(mMessageManager)
  NS_IMPL_CYCLE_COLLECTION_TRAVERSE(mDocShell)
||||||| merged common ancestors

NS_IMPL_CYCLE_COLLECTION_TRAVERSE_BEGIN_INHERITED(InProcessTabChildMessageManager,
                                                  DOMEventTargetHelper)
   NS_IMPL_CYCLE_COLLECTION_TRAVERSE(mMessageManager)
   NS_IMPL_CYCLE_COLLECTION_TRAVERSE(mDocShell)
=======
NS_IMPL_CYCLE_COLLECTION_TRAVERSE_BEGIN_INHERITED(
    InProcessBrowserChildMessageManager, DOMEventTargetHelper)
  NS_IMPL_CYCLE_COLLECTION_TRAVERSE(mMessageManager)
  NS_IMPL_CYCLE_COLLECTION_TRAVERSE(mDocShell)
>>>>>>> upstream-releases:mozilla-release/dom/base/InProcessBrowserChildMessageManager.cpp
NS_IMPL_CYCLE_COLLECTION_TRAVERSE_END

NS_IMPL_CYCLE_COLLECTION_TRACE_BEGIN_INHERITED(
    InProcessBrowserChildMessageManager, DOMEventTargetHelper)
  tmp->nsMessageManagerScriptExecutor::Trace(aCallbacks, aClosure);
NS_IMPL_CYCLE_COLLECTION_TRACE_END

NS_IMPL_CYCLE_COLLECTION_UNLINK_BEGIN_INHERITED(
    InProcessBrowserChildMessageManager, DOMEventTargetHelper)
  NS_IMPL_CYCLE_COLLECTION_UNLINK(mMessageManager)
  NS_IMPL_CYCLE_COLLECTION_UNLINK(mDocShell)
  tmp->nsMessageManagerScriptExecutor::Unlink();
NS_IMPL_CYCLE_COLLECTION_UNLINK_END

NS_INTERFACE_MAP_BEGIN_CYCLE_COLLECTION(InProcessBrowserChildMessageManager)
  NS_INTERFACE_MAP_ENTRY(nsIMessageSender)
  NS_INTERFACE_MAP_ENTRY(nsIInProcessContentFrameMessageManager)
  NS_INTERFACE_MAP_ENTRY(ContentFrameMessageManager)
  NS_INTERFACE_MAP_ENTRY(nsISupportsWeakReference)
NS_INTERFACE_MAP_END_INHERITING(DOMEventTargetHelper)

NS_IMPL_ADDREF_INHERITED(InProcessBrowserChildMessageManager,
                         DOMEventTargetHelper)
NS_IMPL_RELEASE_INHERITED(InProcessBrowserChildMessageManager,
                          DOMEventTargetHelper)

<<<<<<< HEAD:mozilla-release/dom/base/InProcessTabChildMessageManager.cpp
JSObject* InProcessTabChildMessageManager::WrapObject(
    JSContext* aCx, JS::Handle<JSObject*> aGivenProto) {
||||||| merged common ancestors
JSObject*
InProcessTabChildMessageManager::WrapObject(JSContext* aCx,
                                            JS::Handle<JSObject*> aGivenProto)
{
=======
JSObject* InProcessBrowserChildMessageManager::WrapObject(
    JSContext* aCx, JS::Handle<JSObject*> aGivenProto) {
>>>>>>> upstream-releases:mozilla-release/dom/base/InProcessBrowserChildMessageManager.cpp
  return ContentFrameMessageManager_Binding::Wrap(aCx, this, aGivenProto);
}

<<<<<<< HEAD:mozilla-release/dom/base/InProcessTabChildMessageManager.cpp
void InProcessTabChildMessageManager::CacheFrameLoader(
    nsFrameLoader* aFrameLoader) {
||||||| merged common ancestors
void
InProcessTabChildMessageManager::CacheFrameLoader(nsFrameLoader* aFrameLoader)
{
=======
void InProcessBrowserChildMessageManager::CacheFrameLoader(
    nsFrameLoader* aFrameLoader) {
>>>>>>> upstream-releases:mozilla-release/dom/base/InProcessBrowserChildMessageManager.cpp
  mFrameLoader = aFrameLoader;
}

<<<<<<< HEAD:mozilla-release/dom/base/InProcessTabChildMessageManager.cpp
already_AddRefed<nsPIDOMWindowOuter>
InProcessTabChildMessageManager::GetContent(ErrorResult& aError) {
  nsCOMPtr<nsPIDOMWindowOuter> content;
  if (mDocShell) {
    content = mDocShell->GetWindow();
||||||| merged common ancestors
already_AddRefed<nsPIDOMWindowOuter>
InProcessTabChildMessageManager::GetContent(ErrorResult& aError)
{
  nsCOMPtr<nsPIDOMWindowOuter> content;
  if (mDocShell) {
    content = mDocShell->GetWindow();
=======
Nullable<WindowProxyHolder> InProcessBrowserChildMessageManager::GetContent(
    ErrorResult& aError) {
  if (!mDocShell) {
    return nullptr;
>>>>>>> upstream-releases:mozilla-release/dom/base/InProcessBrowserChildMessageManager.cpp
  }
  return WindowProxyHolder(mDocShell->GetBrowsingContext());
}

already_AddRefed<nsIEventTarget>
<<<<<<< HEAD:mozilla-release/dom/base/InProcessTabChildMessageManager.cpp
InProcessTabChildMessageManager::GetTabEventTarget() {
||||||| merged common ancestors
InProcessTabChildMessageManager::GetTabEventTarget()
{
=======
InProcessBrowserChildMessageManager::GetTabEventTarget() {
>>>>>>> upstream-releases:mozilla-release/dom/base/InProcessBrowserChildMessageManager.cpp
  nsCOMPtr<nsIEventTarget> target = GetMainThreadEventTarget();
  return target.forget();
}

<<<<<<< HEAD:mozilla-release/dom/base/InProcessTabChildMessageManager.cpp
uint64_t InProcessTabChildMessageManager::ChromeOuterWindowID() {
||||||| merged common ancestors
uint64_t
InProcessTabChildMessageManager::ChromeOuterWindowID()
{
=======
uint64_t InProcessBrowserChildMessageManager::ChromeOuterWindowID() {
>>>>>>> upstream-releases:mozilla-release/dom/base/InProcessBrowserChildMessageManager.cpp
  if (!mDocShell) {
    return 0;
  }

  nsCOMPtr<nsIDocShellTreeItem> root;
  nsresult rv = mDocShell->GetRootTreeItem(getter_AddRefs(root));
  if (NS_WARN_IF(NS_FAILED(rv))) {
    return 0;
  }

  nsPIDOMWindowOuter* topWin = root->GetWindow();
  if (!topWin) {
    return 0;
  }

  return topWin->WindowID();
}

<<<<<<< HEAD:mozilla-release/dom/base/InProcessTabChildMessageManager.cpp
void InProcessTabChildMessageManager::FireUnloadEvent() {
  // We're called from nsDocument::MaybeInitializeFinalizeFrameLoaders, so it
||||||| merged common ancestors
void
InProcessTabChildMessageManager::FireUnloadEvent()
{
  // We're called from nsDocument::MaybeInitializeFinalizeFrameLoaders, so it
=======
void InProcessBrowserChildMessageManager::FireUnloadEvent() {
  // We're called from Document::MaybeInitializeFinalizeFrameLoaders, so it
>>>>>>> upstream-releases:mozilla-release/dom/base/InProcessBrowserChildMessageManager.cpp
  // should be safe to run script.
  MOZ_ASSERT(nsContentUtils::IsSafeToRunScript());

  // Don't let the unload event propagate to chrome event handlers.
  mPreventEventsEscaping = true;
  DOMEventTargetHelper::DispatchTrustedEvent(NS_LITERAL_STRING("unload"));

  // Allow events fired during docshell destruction (pagehide, unload) to
  // propagate to the <browser> element since chrome code depends on this.
  mPreventEventsEscaping = false;
}

<<<<<<< HEAD:mozilla-release/dom/base/InProcessTabChildMessageManager.cpp
void InProcessTabChildMessageManager::DisconnectEventListeners() {
||||||| merged common ancestors
void
InProcessTabChildMessageManager::DisconnectEventListeners()
{
=======
void InProcessBrowserChildMessageManager::DisconnectEventListeners() {
>>>>>>> upstream-releases:mozilla-release/dom/base/InProcessBrowserChildMessageManager.cpp
  if (mDocShell) {
    if (nsCOMPtr<nsPIDOMWindowOuter> win = mDocShell->GetWindow()) {
      win->SetChromeEventHandler(win->GetChromeEventHandler());
    }
  }
  if (mListenerManager) {
    mListenerManager->Disconnect();
  }

  mDocShell = nullptr;
}

<<<<<<< HEAD:mozilla-release/dom/base/InProcessTabChildMessageManager.cpp
void InProcessTabChildMessageManager::Disconnect() {
||||||| merged common ancestors
void
InProcessTabChildMessageManager::Disconnect()
{
=======
void InProcessBrowserChildMessageManager::Disconnect() {
>>>>>>> upstream-releases:mozilla-release/dom/base/InProcessBrowserChildMessageManager.cpp
  mChromeMessageManager = nullptr;
  mOwner = nullptr;
  if (mMessageManager) {
    static_cast<nsFrameMessageManager*>(mMessageManager.get())->Disconnect();
    mMessageManager = nullptr;
  }
}

<<<<<<< HEAD:mozilla-release/dom/base/InProcessTabChildMessageManager.cpp
NS_IMETHODIMP_(nsIContent*)
InProcessTabChildMessageManager::GetOwnerContent() { return mOwner; }
||||||| merged common ancestors
NS_IMETHODIMP_(nsIContent *)
InProcessTabChildMessageManager::GetOwnerContent()
{
  return mOwner;
}
=======
NS_IMETHODIMP_(nsIContent*)
InProcessBrowserChildMessageManager::GetOwnerContent() { return mOwner; }
>>>>>>> upstream-releases:mozilla-release/dom/base/InProcessBrowserChildMessageManager.cpp

<<<<<<< HEAD:mozilla-release/dom/base/InProcessTabChildMessageManager.cpp
void InProcessTabChildMessageManager::GetEventTargetParent(
    EventChainPreVisitor& aVisitor) {
||||||| merged common ancestors
void
InProcessTabChildMessageManager::GetEventTargetParent(EventChainPreVisitor& aVisitor)
{
=======
void InProcessBrowserChildMessageManager::GetEventTargetParent(
    EventChainPreVisitor& aVisitor) {
>>>>>>> upstream-releases:mozilla-release/dom/base/InProcessBrowserChildMessageManager.cpp
  aVisitor.mForceContentDispatch = true;
  aVisitor.mCanHandle = true;

#ifdef DEBUG
  if (mOwner) {
    RefPtr<nsFrameLoaderOwner> owner = do_QueryObject(mOwner);
    RefPtr<nsFrameLoader> fl = owner->GetFrameLoader();
    if (fl) {
      NS_ASSERTION(this == fl->GetBrowserChildMessageManager(),
                   "Wrong event target!");
      NS_ASSERTION(fl->mMessageManager == mChromeMessageManager,
                   "Wrong message manager!");
    }
  }
#endif

  if (mPreventEventsEscaping) {
    aVisitor.SetParentTarget(nullptr, false);
    return;
  }

  if (mIsBrowserFrame &&
      (!mOwner || !nsContentUtils::IsInChromeDocshell(mOwner->OwnerDoc()))) {
    if (mOwner) {
      if (nsPIDOMWindowInner* innerWindow =
              mOwner->OwnerDoc()->GetInnerWindow()) {
        // 'this' is already a "chrome handler", so we consider window's
        // parent target to be part of that same part of the event path.
        aVisitor.SetParentTarget(innerWindow->GetParentTarget(), false);
      }
    }
  } else {
    aVisitor.SetParentTarget(mOwner, false);
  }
}

<<<<<<< HEAD:mozilla-release/dom/base/InProcessTabChildMessageManager.cpp
class nsAsyncScriptLoad : public Runnable {
 public:
  nsAsyncScriptLoad(InProcessTabChildMessageManager* aTabChild,
                    const nsAString& aURL, bool aRunInGlobalScope)
      : mozilla::Runnable("nsAsyncScriptLoad"),
        mTabChild(aTabChild),
        mURL(aURL),
        mRunInGlobalScope(aRunInGlobalScope) {}

  NS_IMETHOD Run() override {
    mTabChild->LoadFrameScript(mURL, mRunInGlobalScope);
||||||| merged common ancestors
class nsAsyncScriptLoad : public Runnable
{
public:
  nsAsyncScriptLoad(InProcessTabChildMessageManager* aTabChild,
                    const nsAString& aURL,
                    bool aRunInGlobalScope)
    : mozilla::Runnable("nsAsyncScriptLoad")
    , mTabChild(aTabChild)
    , mURL(aURL)
    , mRunInGlobalScope(aRunInGlobalScope)
  {
  }

  NS_IMETHOD Run() override
  {
    mTabChild->LoadFrameScript(mURL, mRunInGlobalScope);
=======
class nsAsyncScriptLoad : public Runnable {
 public:
  nsAsyncScriptLoad(InProcessBrowserChildMessageManager* aBrowserChild,
                    const nsAString& aURL, bool aRunInGlobalScope)
      : mozilla::Runnable("nsAsyncScriptLoad"),
        mBrowserChild(aBrowserChild),
        mURL(aURL),
        mRunInGlobalScope(aRunInGlobalScope) {}

  NS_IMETHOD Run() override {
    mBrowserChild->LoadFrameScript(mURL, mRunInGlobalScope);
>>>>>>> upstream-releases:mozilla-release/dom/base/InProcessBrowserChildMessageManager.cpp
    return NS_OK;
  }
  RefPtr<InProcessBrowserChildMessageManager> mBrowserChild;
  nsString mURL;
  bool mRunInGlobalScope;
};

<<<<<<< HEAD:mozilla-release/dom/base/InProcessTabChildMessageManager.cpp
void InProcessTabChildMessageManager::LoadFrameScript(const nsAString& aURL,
                                                      bool aRunInGlobalScope) {
||||||| merged common ancestors
void
InProcessTabChildMessageManager::LoadFrameScript(const nsAString& aURL, bool aRunInGlobalScope)
{
=======
void InProcessBrowserChildMessageManager::LoadFrameScript(
    const nsAString& aURL, bool aRunInGlobalScope) {
>>>>>>> upstream-releases:mozilla-release/dom/base/InProcessBrowserChildMessageManager.cpp
  if (!nsContentUtils::IsSafeToRunScript()) {
    nsContentUtils::AddScriptRunner(
        new nsAsyncScriptLoad(this, aURL, aRunInGlobalScope));
    return;
  }
  bool tmp = mLoadingScript;
  mLoadingScript = true;
  JS::Rooted<JSObject*> mm(mozilla::dom::RootingCx(), GetOrCreateWrapper());
  LoadScriptInternal(mm, aURL, !aRunInGlobalScope);
  mLoadingScript = tmp;
}

already_AddRefed<nsFrameLoader>
<<<<<<< HEAD:mozilla-release/dom/base/InProcessTabChildMessageManager.cpp
InProcessTabChildMessageManager::GetFrameLoader() {
  nsCOMPtr<nsIFrameLoaderOwner> owner = do_QueryInterface(mOwner);
||||||| merged common ancestors
InProcessTabChildMessageManager::GetFrameLoader()
{
  nsCOMPtr<nsIFrameLoaderOwner> owner = do_QueryInterface(mOwner);
=======
InProcessBrowserChildMessageManager::GetFrameLoader() {
  RefPtr<nsFrameLoaderOwner> owner = do_QueryObject(mOwner);
>>>>>>> upstream-releases:mozilla-release/dom/base/InProcessBrowserChildMessageManager.cpp
  RefPtr<nsFrameLoader> fl = owner ? owner->GetFrameLoader() : nullptr;
  if (!fl) {
    fl = mFrameLoader;
  }
  return fl.forget();
}
