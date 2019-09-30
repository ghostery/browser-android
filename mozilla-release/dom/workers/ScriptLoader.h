/* -*- Mode: C++; tab-width: 8; indent-tabs-mode: nil; c-basic-offset: 2 -*- */
/* vim: set ts=8 sts=2 et sw=2 tw=80: */
/* This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/. */

#ifndef mozilla_dom_workers_scriptloader_h__
#define mozilla_dom_workers_scriptloader_h__

#include "mozilla/dom/WorkerCommon.h"
#include "nsIContentPolicy.h"
#include "nsStringFwd.h"

class nsIPrincipal;
class nsIURI;

class nsILoadGroup;
class nsIChannel;
class nsICookieSettings;

namespace mozilla {

class ErrorResult;

namespace dom {

struct WorkerLoadInfo;
class WorkerPrivate;
class SerializedStackHolder;

enum WorkerScriptType { WorkerScript, DebuggerScript };

namespace workerinternals {

<<<<<<< HEAD
nsresult ChannelFromScriptURLMainThread(
    nsIPrincipal* aPrincipal, nsIDocument* aParentDoc, nsILoadGroup* aLoadGroup,
    nsIURI* aScriptURL, const Maybe<ClientInfo>& aClientInfo,
    nsContentPolicyType aContentPolicyType, nsIChannel** aChannel);

nsresult ChannelFromScriptURLWorkerThread(JSContext* aCx,
                                          WorkerPrivate* aParent,
                                          const nsAString& aScriptURL,
                                          WorkerLoadInfo& aLoadInfo);
||||||| merged common ancestors
nsresult
ChannelFromScriptURLMainThread(nsIPrincipal* aPrincipal,
                               nsIURI* aBaseURI,
                               nsIDocument* aParentDoc,
                               nsILoadGroup* aLoadGroup,
                               const nsAString& aScriptURL,
                               const Maybe<ClientInfo>& aClientInfo,
                               nsContentPolicyType aContentPolicyType,
                               bool aDefaultURIEncoding,
                               nsIChannel** aChannel);

nsresult
ChannelFromScriptURLWorkerThread(JSContext* aCx,
                                 WorkerPrivate* aParent,
                                 const nsAString& aScriptURL,
                                 WorkerLoadInfo& aLoadInfo);
=======
nsresult ChannelFromScriptURLMainThread(
    nsIPrincipal* aPrincipal, Document* aParentDoc, nsILoadGroup* aLoadGroup,
    nsIURI* aScriptURL, const Maybe<ClientInfo>& aClientInfo,
    nsContentPolicyType aContentPolicyType, nsICookieSettings* aCookieSettings,
    nsIReferrerInfo* aReferrerInfo, nsIChannel** aChannel);

nsresult ChannelFromScriptURLWorkerThread(JSContext* aCx,
                                          WorkerPrivate* aParent,
                                          const nsAString& aScriptURL,
                                          WorkerLoadInfo& aLoadInfo);
>>>>>>> upstream-releases

void ReportLoadError(ErrorResult& aRv, nsresult aLoadResult,
                     const nsAString& aScriptURL);

<<<<<<< HEAD
void LoadMainScript(WorkerPrivate* aWorkerPrivate, const nsAString& aScriptURL,
                    WorkerScriptType aWorkerScriptType, ErrorResult& aRv);
||||||| merged common ancestors
void LoadMainScript(WorkerPrivate* aWorkerPrivate,
                    const nsAString& aScriptURL,
                    WorkerScriptType aWorkerScriptType,
                    ErrorResult& aRv);
=======
void LoadMainScript(WorkerPrivate* aWorkerPrivate,
                    UniquePtr<SerializedStackHolder> aOriginStack,
                    const nsAString& aScriptURL,
                    WorkerScriptType aWorkerScriptType, ErrorResult& aRv);
>>>>>>> upstream-releases

<<<<<<< HEAD
void Load(WorkerPrivate* aWorkerPrivate, const nsTArray<nsString>& aScriptURLs,
          WorkerScriptType aWorkerScriptType, ErrorResult& aRv);
||||||| merged common ancestors
void Load(WorkerPrivate* aWorkerPrivate,
          const nsTArray<nsString>& aScriptURLs,
          WorkerScriptType aWorkerScriptType,
          ErrorResult& aRv);
=======
void Load(WorkerPrivate* aWorkerPrivate,
          UniquePtr<SerializedStackHolder> aOriginStack,
          const nsTArray<nsString>& aScriptURLs,
          WorkerScriptType aWorkerScriptType, ErrorResult& aRv);
>>>>>>> upstream-releases

}  // namespace workerinternals

}  // namespace dom
}  // namespace mozilla

#endif /* mozilla_dom_workers_scriptloader_h__ */
