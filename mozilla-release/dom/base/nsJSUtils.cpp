/* -*- Mode: C++; tab-width: 8; indent-tabs-mode: nil; c-basic-offset: 2 -*- */
/* vim: set ts=8 sts=2 et sw=2 tw=80: */
/* This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/. */

/**
 * This is not a generated file. It contains common utility functions
 * invoked from the JavaScript code generated from IDL interfaces.
 * The goal of the utility functions is to cut down on the size of
 * the generated code itself.
 */

#include "nsJSUtils.h"
#include "jsapi.h"
#include "jsfriendapi.h"
#include "js/CompilationAndEvaluation.h"
#include "js/Modules.h"  // JS::CompileModule{,DontInflate}, JS::GetModuleScript, JS::Module{Instantiate,Evaluate}
#include "js/OffThreadScriptCompilation.h"
#include "js/SourceText.h"
#include "nsIScriptContext.h"
#include "nsIScriptElement.h"
#include "nsIScriptGlobalObject.h"
#include "nsIXPConnect.h"
#include "nsCOMPtr.h"
#include "nsIScriptSecurityManager.h"
#include "nsPIDOMWindow.h"
#include "GeckoProfiler.h"
#include "nsJSPrincipals.h"
#include "xpcpublic.h"
#include "nsContentUtils.h"
#include "nsGlobalWindow.h"
#include "nsXBLPrototypeBinding.h"
#include "mozilla/CycleCollectedJSContext.h"
#include "mozilla/StaticPrefs.h"
#include "mozilla/dom/BindingUtils.h"
#include "mozilla/dom/Date.h"
#include "mozilla/dom/Element.h"
#include "mozilla/dom/ScriptSettings.h"
#include "mozilla/Utf8.h"  // mozilla::Utf8Unit

using namespace mozilla;
using namespace mozilla::dom;

bool nsJSUtils::GetCallingLocation(JSContext* aContext, nsACString& aFilename,
                                   uint32_t* aLineno, uint32_t* aColumn) {
  JS::AutoFilename filename;
  if (!JS::DescribeScriptedCaller(aContext, &filename, aLineno, aColumn)) {
    return false;
  }

  return aFilename.Assign(filename.get(), fallible);
}

bool nsJSUtils::GetCallingLocation(JSContext* aContext, nsAString& aFilename,
                                   uint32_t* aLineno, uint32_t* aColumn) {
  JS::AutoFilename filename;
  if (!JS::DescribeScriptedCaller(aContext, &filename, aLineno, aColumn)) {
    return false;
  }

  return aFilename.Assign(NS_ConvertUTF8toUTF16(filename.get()), fallible);
}

uint64_t nsJSUtils::GetCurrentlyRunningCodeInnerWindowID(JSContext* aContext) {
  if (!aContext) return 0;

  nsGlobalWindowInner* win = xpc::CurrentWindowOrNull(aContext);
  return win ? win->WindowID() : 0;
}

<<<<<<< HEAD
nsresult nsJSUtils::CompileFunction(AutoJSAPI& jsapi,
                                    JS::AutoObjectVector& aScopeChain,
                                    JS::CompileOptions& aOptions,
                                    const nsACString& aName, uint32_t aArgCount,
                                    const char** aArgArray,
                                    const nsAString& aBody,
                                    JSObject** aFunctionObject) {
||||||| merged common ancestors
nsresult
nsJSUtils::CompileFunction(AutoJSAPI& jsapi,
                           JS::AutoObjectVector& aScopeChain,
                           JS::CompileOptions& aOptions,
                           const nsACString& aName,
                           uint32_t aArgCount,
                           const char** aArgArray,
                           const nsAString& aBody,
                           JSObject** aFunctionObject)
{
=======
nsresult nsJSUtils::CompileFunction(AutoJSAPI& jsapi,
                                    JS::HandleVector<JSObject*> aScopeChain,
                                    JS::CompileOptions& aOptions,
                                    const nsACString& aName, uint32_t aArgCount,
                                    const char** aArgArray,
                                    const nsAString& aBody,
                                    JSObject** aFunctionObject) {
>>>>>>> upstream-releases
  JSContext* cx = jsapi.cx();
  MOZ_ASSERT(js::GetContextRealm(cx));
  MOZ_ASSERT_IF(aScopeChain.length() != 0,
                js::IsObjectInContextCompartment(aScopeChain[0], cx));

  // Do the junk Gecko is supposed to do before calling into JSAPI.
  for (size_t i = 0; i < aScopeChain.length(); ++i) {
    JS::ExposeObjectToActiveJS(aScopeChain[i]);
  }

  // Compile.
<<<<<<< HEAD
  const nsPromiseFlatString& flatBody = PromiseFlatString(aBody);

  JS::SourceText<char16_t> source;
  if (!source.init(cx, flatBody.get(), flatBody.Length(),
                   JS::SourceOwnership::Borrowed)) {
    return NS_ERROR_FAILURE;
  }

  JS::Rooted<JSFunction*> fun(cx);
  if (!JS::CompileFunction(cx, aScopeChain, aOptions,
                           PromiseFlatCString(aName).get(), aArgCount,
                           aArgArray, source, &fun)) {
||||||| merged common ancestors
  JS::Rooted<JSFunction*> fun(cx);
  const nsString& bodyString = PromiseFlatString(aBody);
  JS::SourceBufferHolder source(bodyString.get(), aBody.Length(),
                                JS::SourceBufferHolder::NoOwnership);
  if (!JS::CompileFunction(cx, aScopeChain, aOptions,
                           PromiseFlatCString(aName).get(),
                           aArgCount, aArgArray,
                           source, &fun))
  {
=======
  const nsPromiseFlatString& flatBody = PromiseFlatString(aBody);

  JS::SourceText<char16_t> source;
  if (!source.init(cx, flatBody.get(), flatBody.Length(),
                   JS::SourceOwnership::Borrowed)) {
    return NS_ERROR_FAILURE;
  }

  JS::Rooted<JSFunction*> fun(
      cx, JS::CompileFunction(cx, aScopeChain, aOptions,
                              PromiseFlatCString(aName).get(), aArgCount,
                              aArgArray, source));
  if (!fun) {
>>>>>>> upstream-releases
    return NS_ERROR_FAILURE;
  }

  *aFunctionObject = JS_GetFunctionObject(fun);
  return NS_OK;
}

static nsresult EvaluationExceptionToNSResult(JSContext* aCx) {
  if (JS_IsExceptionPending(aCx)) {
    return NS_SUCCESS_DOM_SCRIPT_EVALUATION_THREW;
  }
  return NS_SUCCESS_DOM_SCRIPT_EVALUATION_THREW_UNCATCHABLE;
}

nsJSUtils::ExecutionContext::ExecutionContext(JSContext* aCx,
                                              JS::Handle<JSObject*> aGlobal)
    :
#ifdef MOZ_GECKO_PROFILER
<<<<<<< HEAD
      mAutoProfilerLabel("nsJSUtils::ExecutionContext",
                         /* dynamicStr */ nullptr,
                         js::ProfilingStackFrame::Category::JS),
||||||| merged common ancestors
    mAutoProfilerLabel("nsJSUtils::ExecutionContext", /* dynamicStr */ nullptr,
                       __LINE__, js::ProfilingStackFrame::Category::JS),
=======
      mAutoProfilerLabel("nsJSUtils::ExecutionContext",
                         /* dynamicStr */ nullptr,
                         JS::ProfilingCategoryPair::JS),
>>>>>>> upstream-releases
#endif
<<<<<<< HEAD
      mCx(aCx),
      mRealm(aCx, aGlobal),
      mRetValue(aCx),
      mScopeChain(aCx),
      mRv(NS_OK),
      mSkip(false),
      mCoerceToString(false),
      mEncodeBytecode(false)
||||||| merged common ancestors
    mCx(aCx)
  , mRealm(aCx, aGlobal)
  , mRetValue(aCx)
  , mScopeChain(aCx)
  , mRv(NS_OK)
  , mSkip(false)
  , mCoerceToString(false)
  , mEncodeBytecode(false)
=======
      mCx(aCx),
      mRealm(aCx, aGlobal),
      mRetValue(aCx),
      mScopeChain(aCx),
      mScript(aCx),
      mRv(NS_OK),
      mSkip(false),
      mCoerceToString(false),
      mEncodeBytecode(false)
>>>>>>> upstream-releases
#ifdef DEBUG
<<<<<<< HEAD
      ,
      mWantsReturnValue(false),
      mExpectScopeChain(false)
||||||| merged common ancestors
  , mWantsReturnValue(false)
  , mExpectScopeChain(false)
=======
      ,
      mWantsReturnValue(false),
      mExpectScopeChain(false),
      mScriptUsed(false)
>>>>>>> upstream-releases
#endif
{
  MOZ_ASSERT(aCx == nsContentUtils::GetCurrentJSContext());
  MOZ_ASSERT(NS_IsMainThread());
  MOZ_ASSERT(CycleCollectedJSContext::Get() &&
             CycleCollectedJSContext::Get()->MicroTaskLevel());
  MOZ_ASSERT(mRetValue.isUndefined());

  MOZ_ASSERT(JS_IsGlobalObject(aGlobal));
  if (MOZ_UNLIKELY(!xpc::Scriptability::Get(aGlobal).Allowed())) {
    mSkip = true;
    mRv = NS_OK;
  }
}

<<<<<<< HEAD
void nsJSUtils::ExecutionContext::SetScopeChain(
    const JS::AutoObjectVector& aScopeChain) {
||||||| merged common ancestors
void
nsJSUtils::ExecutionContext::SetScopeChain(
  const JS::AutoObjectVector& aScopeChain)
{
=======
void nsJSUtils::ExecutionContext::SetScopeChain(
    JS::HandleVector<JSObject*> aScopeChain) {
>>>>>>> upstream-releases
  if (mSkip) {
    return;
  }

#ifdef DEBUG
  mExpectScopeChain = true;
#endif
  // Now make sure to wrap the scope chain into the right compartment.
  if (!mScopeChain.reserve(aScopeChain.length())) {
    mSkip = true;
    mRv = NS_ERROR_OUT_OF_MEMORY;
    return;
  }

  for (size_t i = 0; i < aScopeChain.length(); ++i) {
    JS::ExposeObjectToActiveJS(aScopeChain[i]);
    mScopeChain.infallibleAppend(aScopeChain[i]);
    if (!JS_WrapObject(mCx, mScopeChain[i])) {
      mSkip = true;
      mRv = NS_ERROR_OUT_OF_MEMORY;
      return;
    }
  }
}

<<<<<<< HEAD
nsresult nsJSUtils::ExecutionContext::JoinAndExec(
    JS::OffThreadToken** aOffThreadToken,
    JS::MutableHandle<JSScript*> aScript) {
||||||| merged common ancestors
nsresult
nsJSUtils::ExecutionContext::JoinAndExec(JS::OffThreadToken** aOffThreadToken,
                                         JS::MutableHandle<JSScript*> aScript)
{
=======
nsresult nsJSUtils::ExecutionContext::JoinCompile(
    JS::OffThreadToken** aOffThreadToken) {
>>>>>>> upstream-releases
  if (mSkip) {
    return mRv;
  }

  MOZ_ASSERT(!mWantsReturnValue);
  MOZ_ASSERT(!mExpectScopeChain);
<<<<<<< HEAD
  aScript.set(JS::FinishOffThreadScript(mCx, *aOffThreadToken));
  *aOffThreadToken = nullptr;  // Mark the token as having been finished.
  if (!aScript) {
    mSkip = true;
    mRv = EvaluationExceptionToNSResult(mCx);
    return mRv;
  }

  if (mEncodeBytecode && !StartIncrementalEncoding(mCx, aScript)) {
||||||| merged common ancestors
  aScript.set(JS::FinishOffThreadScript(mCx, *aOffThreadToken));
  *aOffThreadToken = nullptr; // Mark the token as having been finished.
  if (!aScript) {
    mSkip = true;
    mRv = EvaluationExceptionToNSResult(mCx);
    return mRv;
  }

  if (mEncodeBytecode && !StartIncrementalEncoding(mCx, aScript)) {
=======
  MOZ_ASSERT(!mScript);
  mScript.set(JS::FinishOffThreadScript(mCx, *aOffThreadToken));
  *aOffThreadToken = nullptr;  // Mark the token as having been finished.
  if (!mScript) {
>>>>>>> upstream-releases
    mSkip = true;
    mRv = EvaluationExceptionToNSResult(mCx);
    return mRv;
  }

  if (mEncodeBytecode && !StartIncrementalEncoding(mCx, mScript)) {
    mSkip = true;
    mRv = EvaluationExceptionToNSResult(mCx);
    return mRv;
  }

  return NS_OK;
}

<<<<<<< HEAD
nsresult nsJSUtils::ExecutionContext::CompileAndExec(
    JS::CompileOptions& aCompileOptions, JS::SourceText<char16_t>& aSrcBuf,
    JS::MutableHandle<JSScript*> aScript) {
||||||| merged common ancestors
nsresult
nsJSUtils::ExecutionContext::CompileAndExec(JS::CompileOptions& aCompileOptions,
                                            JS::SourceBufferHolder& aSrcBuf,
                                            JS::MutableHandle<JSScript*> aScript)
{
=======
static JSScript* CompileScript(
    JSContext* aCx, JS::Handle<JS::StackGCVector<JSObject*>> aScopeChain,
    JS::CompileOptions& aCompileOptions, JS::SourceText<char16_t>& aSrcBuf) {
  return aScopeChain.length() == 0
             ? JS::Compile(aCx, aCompileOptions, aSrcBuf)
             : JS::CompileForNonSyntacticScope(aCx, aCompileOptions, aSrcBuf);
}

static JSScript* CompileScript(
    JSContext* aCx, JS::Handle<JS::StackGCVector<JSObject*>> aScopeChain,
    JS::CompileOptions& aCompileOptions, JS::SourceText<Utf8Unit>& aSrcBuf) {
  // Once the UTF-8 overloads don't inflate, we can get rid of these two
  // |CompileScript| overloads and just call the JSAPI directly in the one
  // caller.
  return aScopeChain.length() == 0
             ? JS::CompileDontInflate(aCx, aCompileOptions, aSrcBuf)
             : JS::CompileForNonSyntacticScopeDontInflate(aCx, aCompileOptions,
                                                          aSrcBuf);
}

template <typename Unit>
nsresult nsJSUtils::ExecutionContext::InternalCompile(
    JS::CompileOptions& aCompileOptions, JS::SourceText<Unit>& aSrcBuf) {
>>>>>>> upstream-releases
  if (mSkip) {
    return mRv;
  }

  MOZ_ASSERT(aSrcBuf.get());
  MOZ_ASSERT(mRetValue.isUndefined());
#ifdef DEBUG
  mWantsReturnValue = !aCompileOptions.noScriptRval;
#endif

<<<<<<< HEAD
  bool compiled = true;
  if (mScopeChain.length() == 0) {
    compiled = JS::Compile(mCx, aCompileOptions, aSrcBuf, aScript);
  } else {
    compiled =
        JS::CompileForNonSyntacticScope(mCx, aCompileOptions, aSrcBuf, aScript);
  }

  MOZ_ASSERT_IF(compiled, aScript);
  if (!compiled) {
||||||| merged common ancestors
  bool compiled = true;
  if (mScopeChain.length() == 0) {
    compiled = JS::Compile(mCx, aCompileOptions, aSrcBuf, aScript);
  } else {
    compiled = JS::CompileForNonSyntacticScope(mCx, aCompileOptions, aSrcBuf, aScript);
  }

  MOZ_ASSERT_IF(compiled, aScript);
  if (!compiled) {
=======
  MOZ_ASSERT(!mScript);
  mScript = CompileScript(mCx, mScopeChain, aCompileOptions, aSrcBuf);
  if (!mScript) {
>>>>>>> upstream-releases
    mSkip = true;
    mRv = EvaluationExceptionToNSResult(mCx);
    return mRv;
  }

  if (mEncodeBytecode && !StartIncrementalEncoding(mCx, mScript)) {
    mSkip = true;
    mRv = EvaluationExceptionToNSResult(mCx);
    return mRv;
  }

  return NS_OK;
}

<<<<<<< HEAD
nsresult nsJSUtils::ExecutionContext::CompileAndExec(
    JS::CompileOptions& aCompileOptions, const nsAString& aScript) {
  MOZ_ASSERT(!mEncodeBytecode,
             "A JSScript is needed for calling FinishIncrementalEncoding");
||||||| merged common ancestors
nsresult
nsJSUtils::ExecutionContext::CompileAndExec(JS::CompileOptions& aCompileOptions,
                                            const nsAString& aScript)
{
  MOZ_ASSERT(!mEncodeBytecode, "A JSScript is needed for calling FinishIncrementalEncoding");
=======
nsresult nsJSUtils::ExecutionContext::Compile(
    JS::CompileOptions& aCompileOptions, JS::SourceText<char16_t>& aSrcBuf) {
  return InternalCompile(aCompileOptions, aSrcBuf);
}

nsresult nsJSUtils::ExecutionContext::Compile(
    JS::CompileOptions& aCompileOptions, JS::SourceText<Utf8Unit>& aSrcBuf) {
  return InternalCompile(aCompileOptions, aSrcBuf);
}

nsresult nsJSUtils::ExecutionContext::Compile(
    JS::CompileOptions& aCompileOptions, const nsAString& aScript) {
>>>>>>> upstream-releases
  if (mSkip) {
    return mRv;
  }

  const nsPromiseFlatString& flatScript = PromiseFlatString(aScript);
<<<<<<< HEAD
  JS::SourceText<char16_t> srcBuf;
  if (!srcBuf.init(mCx, flatScript.get(), flatScript.Length(),
                   JS::SourceOwnership::Borrowed)) {
    mSkip = true;
    mRv = EvaluationExceptionToNSResult(mCx);
    return mRv;
  }

  JS::Rooted<JSScript*> script(mCx);
  return CompileAndExec(aCompileOptions, srcBuf, &script);
||||||| merged common ancestors
  JS::SourceBufferHolder srcBuf(flatScript.get(), aScript.Length(),
                                JS::SourceBufferHolder::NoOwnership);
  JS::Rooted<JSScript*> script(mCx);
  return CompileAndExec(aCompileOptions, srcBuf, &script);
=======
  JS::SourceText<char16_t> srcBuf;
  if (!srcBuf.init(mCx, flatScript.get(), flatScript.Length(),
                   JS::SourceOwnership::Borrowed)) {
    mSkip = true;
    mRv = EvaluationExceptionToNSResult(mCx);
    return mRv;
  }

  return Compile(aCompileOptions, srcBuf);
>>>>>>> upstream-releases
}

<<<<<<< HEAD
nsresult nsJSUtils::ExecutionContext::DecodeAndExec(
    JS::CompileOptions& aCompileOptions, mozilla::Vector<uint8_t>& aBytecodeBuf,
    size_t aBytecodeIndex) {
  MOZ_ASSERT(!mEncodeBytecode,
             "A JSScript is needed for calling FinishIncrementalEncoding");
||||||| merged common ancestors
nsresult
nsJSUtils::ExecutionContext::DecodeAndExec(JS::CompileOptions& aCompileOptions,
                                           mozilla::Vector<uint8_t>& aBytecodeBuf,
                                           size_t aBytecodeIndex)
{
  MOZ_ASSERT(!mEncodeBytecode, "A JSScript is needed for calling FinishIncrementalEncoding");
=======
nsresult nsJSUtils::ExecutionContext::Decode(
    JS::CompileOptions& aCompileOptions, mozilla::Vector<uint8_t>& aBytecodeBuf,
    size_t aBytecodeIndex) {
>>>>>>> upstream-releases
  if (mSkip) {
    return mRv;
  }

  MOZ_ASSERT(!mWantsReturnValue);
<<<<<<< HEAD
  JS::Rooted<JSScript*> script(mCx);
  JS::TranscodeResult tr =
      JS::DecodeScript(mCx, aBytecodeBuf, &script, aBytecodeIndex);
||||||| merged common ancestors
  JS::Rooted<JSScript*> script(mCx);
  JS::TranscodeResult tr = JS::DecodeScript(mCx, aBytecodeBuf, &script, aBytecodeIndex);
=======
  JS::TranscodeResult tr =
      JS::DecodeScript(mCx, aBytecodeBuf, &mScript, aBytecodeIndex);
>>>>>>> upstream-releases
  // These errors are external parameters which should be handled before the
  // decoding phase, and which are the only reasons why you might want to
  // fallback on decoding failures.
  MOZ_ASSERT(tr != JS::TranscodeResult_Failure_BadBuildId &&
             tr != JS::TranscodeResult_Failure_WrongCompileOption);
  if (tr != JS::TranscodeResult_Ok) {
    mSkip = true;
    mRv = NS_ERROR_DOM_JS_DECODING_ERROR;
    return mRv;
  }

  return mRv;
}

<<<<<<< HEAD
nsresult nsJSUtils::ExecutionContext::DecodeJoinAndExec(
    JS::OffThreadToken** aOffThreadToken) {
||||||| merged common ancestors
nsresult
nsJSUtils::ExecutionContext::DecodeJoinAndExec(JS::OffThreadToken** aOffThreadToken)
{
=======
nsresult nsJSUtils::ExecutionContext::JoinDecode(
    JS::OffThreadToken** aOffThreadToken) {
>>>>>>> upstream-releases
  if (mSkip) {
    return mRv;
  }

  MOZ_ASSERT(!mWantsReturnValue);
  MOZ_ASSERT(!mExpectScopeChain);
<<<<<<< HEAD
  JS::Rooted<JSScript*> script(mCx);
  script.set(JS::FinishOffThreadScriptDecoder(mCx, *aOffThreadToken));
  *aOffThreadToken = nullptr;  // Mark the token as having been finished.
  if (!script || !JS_ExecuteScript(mCx, mScopeChain, script)) {
||||||| merged common ancestors
  JS::Rooted<JSScript*> script(mCx);
  script.set(JS::FinishOffThreadScriptDecoder(mCx, *aOffThreadToken));
  *aOffThreadToken = nullptr; // Mark the token as having been finished.
  if (!script || !JS_ExecuteScript(mCx, mScopeChain, script)) {
=======
  mScript.set(JS::FinishOffThreadScriptDecoder(mCx, *aOffThreadToken));
  *aOffThreadToken = nullptr;  // Mark the token as having been finished.
  if (!mScript) {
>>>>>>> upstream-releases
    mSkip = true;
    mRv = EvaluationExceptionToNSResult(mCx);
    return mRv;
  }

  return NS_OK;
}

<<<<<<< HEAD
nsresult nsJSUtils::ExecutionContext::DecodeBinASTJoinAndExec(
    JS::OffThreadToken** aOffThreadToken,
    JS::MutableHandle<JSScript*> aScript) {
||||||| merged common ancestors
nsresult
nsJSUtils::ExecutionContext::DecodeBinASTJoinAndExec(JS::OffThreadToken** aOffThreadToken,
                                                     JS::MutableHandle<JSScript*> aScript)
{
=======
nsresult nsJSUtils::ExecutionContext::JoinDecodeBinAST(
    JS::OffThreadToken** aOffThreadToken) {
>>>>>>> upstream-releases
#ifdef JS_BUILD_BINAST
  if (mSkip) {
    return mRv;
  }

  MOZ_ASSERT(!mWantsReturnValue);
  MOZ_ASSERT(!mExpectScopeChain);

<<<<<<< HEAD
  aScript.set(JS::FinishOffThreadBinASTDecode(mCx, *aOffThreadToken));
  *aOffThreadToken = nullptr;  // Mark the token as having been finished.
||||||| merged common ancestors
  aScript.set(JS::FinishOffThreadBinASTDecode(mCx, *aOffThreadToken));
  *aOffThreadToken = nullptr; // Mark the token as having been finished.
=======
  mScript.set(JS::FinishOffThreadBinASTDecode(mCx, *aOffThreadToken));
  *aOffThreadToken = nullptr;  // Mark the token as having been finished.
>>>>>>> upstream-releases

  if (!mScript) {
    mSkip = true;
    mRv = EvaluationExceptionToNSResult(mCx);
    return mRv;
  }

  if (mEncodeBytecode && !StartIncrementalEncoding(mCx, mScript)) {
    mSkip = true;
    mRv = EvaluationExceptionToNSResult(mCx);
    return mRv;
  }

  return NS_OK;
#else
  return NS_ERROR_NOT_IMPLEMENTED;
#endif
}

<<<<<<< HEAD
nsresult nsJSUtils::ExecutionContext::DecodeBinASTAndExec(
    JS::CompileOptions& aCompileOptions, const uint8_t* aBuf, size_t aLength,
    JS::MutableHandle<JSScript*> aScript) {
||||||| merged common ancestors
nsresult
nsJSUtils::ExecutionContext::DecodeBinASTAndExec(JS::CompileOptions& aCompileOptions,
                                                 const uint8_t* aBuf, size_t aLength,
                                                 JS::MutableHandle<JSScript*> aScript)
{
=======
nsresult nsJSUtils::ExecutionContext::DecodeBinAST(
    JS::CompileOptions& aCompileOptions, const uint8_t* aBuf, size_t aLength) {
>>>>>>> upstream-releases
#ifdef JS_BUILD_BINAST
  MOZ_ASSERT(mScopeChain.length() == 0,
             "BinAST decoding is not supported in non-syntactic scopes");

  if (mSkip) {
    return mRv;
  }

  MOZ_ASSERT(aBuf);
  MOZ_ASSERT(mRetValue.isUndefined());
#  ifdef DEBUG
  mWantsReturnValue = !aCompileOptions.noScriptRval;
#  endif

  mScript.set(JS::DecodeBinAST(mCx, aCompileOptions, aBuf, aLength));

  if (!mScript) {
    mSkip = true;
    mRv = EvaluationExceptionToNSResult(mCx);
    return mRv;
  }

  if (mEncodeBytecode && !StartIncrementalEncoding(mCx, mScript)) {
    mSkip = true;
    mRv = EvaluationExceptionToNSResult(mCx);
    return mRv;
  }

  return NS_OK;
#else
  return NS_ERROR_NOT_IMPLEMENTED;
#endif
}

JSScript* nsJSUtils::ExecutionContext::GetScript() {
#ifdef DEBUG
  MOZ_ASSERT(!mSkip);
  MOZ_ASSERT(mScript);
  mScriptUsed = true;
#endif

  return MaybeGetScript();
}

JSScript* nsJSUtils::ExecutionContext::MaybeGetScript() { return mScript; }

nsresult nsJSUtils::ExecutionContext::ExecScript() {
  if (mSkip) {
    return mRv;
  }

  MOZ_ASSERT(mScript);

  if (!JS_ExecuteScript(mCx, mScopeChain, mScript)) {
    mSkip = true;
    mRv = EvaluationExceptionToNSResult(mCx);
    return mRv;
  }

  return NS_OK;
}

static bool IsPromiseValue(JSContext* aCx, JS::Handle<JS::Value> aValue) {
  if (!aValue.isObject()) {
    return false;
  }

  // We only care about Promise here, so CheckedUnwrapStatic is fine.
  JS::Rooted<JSObject*> obj(aCx, js::CheckedUnwrapStatic(&aValue.toObject()));
  if (!obj) {
    return false;
  }

  return JS::IsPromiseObject(obj);
}

<<<<<<< HEAD
nsresult nsJSUtils::ExecutionContext::ExtractReturnValue(
    JS::MutableHandle<JS::Value> aRetValue) {
  MOZ_ASSERT(aRetValue.isUndefined());
||||||| merged common ancestors
nsresult
nsJSUtils::ExecutionContext::ExtractReturnValue(JS::MutableHandle<JS::Value> aRetValue)
{
  MOZ_ASSERT(aRetValue.isUndefined());
=======
nsresult nsJSUtils::ExecutionContext::ExecScript(
    JS::MutableHandle<JS::Value> aRetValue) {
>>>>>>> upstream-releases
  if (mSkip) {
    aRetValue.setUndefined();
    return mRv;
  }

  MOZ_ASSERT(mScript);
  MOZ_ASSERT(mWantsReturnValue);

  if (!JS_ExecuteScript(mCx, mScopeChain, mScript, aRetValue)) {
    mSkip = true;
    mRv = EvaluationExceptionToNSResult(mCx);
    return mRv;
  }

#ifdef DEBUG
  mWantsReturnValue = false;
#endif
  if (mCoerceToString && IsPromiseValue(mCx, aRetValue)) {
    // We're a javascript: url and we should treat Promise return values as
    // undefined.
    //
    // Once bug 1477821 is fixed this code might be able to go away, or will
    // become enshrined in the spec, depending.
    aRetValue.setUndefined();
  }

  if (mCoerceToString && !aRetValue.isUndefined()) {
    JSString* str = JS::ToString(mCx, aRetValue);
    if (!str) {
      // ToString can be a function call, so an exception can be raised while
      // executing the function.
      mSkip = true;
      return EvaluationExceptionToNSResult(mCx);
    }
    aRetValue.set(JS::StringValue(str));
  }

  return NS_OK;
}

<<<<<<< HEAD
nsresult nsJSUtils::CompileModule(JSContext* aCx,
                                  JS::SourceText<char16_t>& aSrcBuf,
                                  JS::Handle<JSObject*> aEvaluationGlobal,
                                  JS::CompileOptions& aCompileOptions,
                                  JS::MutableHandle<JSObject*> aModule) {
  AUTO_PROFILER_LABEL("nsJSUtils::CompileModule", JS);
||||||| merged common ancestors
nsresult
nsJSUtils::CompileModule(JSContext* aCx,
                       JS::SourceBufferHolder& aSrcBuf,
                       JS::Handle<JSObject*> aEvaluationGlobal,
                       JS::CompileOptions &aCompileOptions,
                       JS::MutableHandle<JSObject*> aModule)
{
  AUTO_PROFILER_LABEL("nsJSUtils::CompileModule", JS);
=======
static JSObject* CompileModule(JSContext* aCx,
                               JS::CompileOptions& aCompileOptions,
                               JS::SourceText<char16_t>& aSrcBuf) {
  return JS::CompileModule(aCx, aCompileOptions, aSrcBuf);
}

static JSObject* CompileModule(JSContext* aCx,
                               JS::CompileOptions& aCompileOptions,
                               JS::SourceText<Utf8Unit>& aSrcBuf) {
  // Once compile-UTF-8-without-inflating is stable, it'll be renamed to remove
  // the "DontInflate" suffix, these two overloads can be removed, and
  // |JS::CompileModule| can be used in the sole caller below.
  return JS::CompileModuleDontInflate(aCx, aCompileOptions, aSrcBuf);
}
>>>>>>> upstream-releases

template <typename Unit>
static nsresult CompileJSModule(JSContext* aCx, JS::SourceText<Unit>& aSrcBuf,
                                JS::Handle<JSObject*> aEvaluationGlobal,
                                JS::CompileOptions& aCompileOptions,
                                JS::MutableHandle<JSObject*> aModule) {
  AUTO_PROFILER_LABEL("nsJSUtils::CompileModule", JS);
  MOZ_ASSERT(aCx == nsContentUtils::GetCurrentJSContext());
  MOZ_ASSERT(aSrcBuf.get());
  MOZ_ASSERT(JS_IsGlobalObject(aEvaluationGlobal));
  MOZ_ASSERT(JS::CurrentGlobalOrNull(aCx) == aEvaluationGlobal);
  MOZ_ASSERT(NS_IsMainThread());
  MOZ_ASSERT(CycleCollectedJSContext::Get() &&
             CycleCollectedJSContext::Get()->MicroTaskLevel());

  NS_ENSURE_TRUE(xpc::Scriptability::Get(aEvaluationGlobal).Allowed(), NS_OK);

  JSObject* module = CompileModule(aCx, aCompileOptions, aSrcBuf);
  if (!module) {
    return NS_ERROR_FAILURE;
  }

  aModule.set(module);
  return NS_OK;
}

<<<<<<< HEAD
nsresult nsJSUtils::InitModuleSourceElement(JSContext* aCx,
                                            JS::Handle<JSObject*> aModule,
                                            nsIScriptElement* aElement) {
||||||| merged common ancestors
nsresult
nsJSUtils::InitModuleSourceElement(JSContext* aCx,
                                   JS::Handle<JSObject*> aModule,
                                   nsIScriptElement* aElement)
{
=======
nsresult nsJSUtils::CompileModule(JSContext* aCx,
                                  JS::SourceText<char16_t>& aSrcBuf,
                                  JS::Handle<JSObject*> aEvaluationGlobal,
                                  JS::CompileOptions& aCompileOptions,
                                  JS::MutableHandle<JSObject*> aModule) {
  return CompileJSModule(aCx, aSrcBuf, aEvaluationGlobal, aCompileOptions,
                         aModule);
}

nsresult nsJSUtils::CompileModule(JSContext* aCx,
                                  JS::SourceText<Utf8Unit>& aSrcBuf,
                                  JS::Handle<JSObject*> aEvaluationGlobal,
                                  JS::CompileOptions& aCompileOptions,
                                  JS::MutableHandle<JSObject*> aModule) {
  return CompileJSModule(aCx, aSrcBuf, aEvaluationGlobal, aCompileOptions,
                         aModule);
}

nsresult nsJSUtils::InitModuleSourceElement(JSContext* aCx,
                                            JS::Handle<JSObject*> aModule,
                                            nsIScriptElement* aElement) {
>>>>>>> upstream-releases
  JS::Rooted<JS::Value> value(aCx);
  nsresult rv = nsContentUtils::WrapNative(aCx, aElement, &value,
                                           /* aAllowWrapping = */ true);
  if (NS_FAILED(rv)) {
    return rv;
  }

  MOZ_ASSERT(value.isObject());
  JS::Rooted<JSObject*> object(aCx, &value.toObject());

  JS::Rooted<JSScript*> script(aCx, JS::GetModuleScript(aModule));
  if (!JS::InitScriptSourceElement(aCx, script, object, nullptr)) {
    return NS_ERROR_FAILURE;
  }

  return NS_OK;
}

nsresult nsJSUtils::ModuleInstantiate(JSContext* aCx,
                                      JS::Handle<JSObject*> aModule) {
  AUTO_PROFILER_LABEL("nsJSUtils::ModuleInstantiate", JS);

  MOZ_ASSERT(aCx == nsContentUtils::GetCurrentJSContext());
  MOZ_ASSERT(NS_IsMainThread());
  MOZ_ASSERT(CycleCollectedJSContext::Get() &&
             CycleCollectedJSContext::Get()->MicroTaskLevel());

  NS_ENSURE_TRUE(xpc::Scriptability::Get(aModule).Allowed(), NS_OK);

  if (!JS::ModuleInstantiate(aCx, aModule)) {
    return NS_ERROR_FAILURE;
  }

  return NS_OK;
}

nsresult nsJSUtils::ModuleEvaluate(JSContext* aCx,
                                   JS::Handle<JSObject*> aModule) {
  AUTO_PROFILER_LABEL("nsJSUtils::ModuleEvaluate", JS);

  MOZ_ASSERT(aCx == nsContentUtils::GetCurrentJSContext());
  MOZ_ASSERT(NS_IsMainThread());
  MOZ_ASSERT(CycleCollectedJSContext::Get() &&
             CycleCollectedJSContext::Get()->MicroTaskLevel());

  NS_ENSURE_TRUE(xpc::Scriptability::Get(aModule).Allowed(), NS_OK);

  if (!JS::ModuleEvaluate(aCx, aModule)) {
    return NS_ERROR_FAILURE;
  }

  return NS_OK;
}

<<<<<<< HEAD
static bool AddScopeChainItem(JSContext* aCx, nsINode* aNode,
                              JS::AutoObjectVector& aScopeChain) {
||||||| merged common ancestors
static bool
AddScopeChainItem(JSContext* aCx,
                  nsINode* aNode,
                  JS::AutoObjectVector& aScopeChain)
{
=======
static bool AddScopeChainItem(JSContext* aCx, nsINode* aNode,
                              JS::MutableHandleVector<JSObject*> aScopeChain) {
>>>>>>> upstream-releases
  JS::RootedValue val(aCx);
  if (!GetOrCreateDOMReflector(aCx, aNode, &val)) {
    return false;
  }

  if (!aScopeChain.append(&val.toObject())) {
    return false;
  }

  return true;
}

/* static */
<<<<<<< HEAD
bool nsJSUtils::GetScopeChainForElement(JSContext* aCx, Element* aElement,
                                        JS::AutoObjectVector& aScopeChain) {
||||||| merged common ancestors
bool
nsJSUtils::GetScopeChainForElement(JSContext* aCx,
                                   Element* aElement,
                                   JS::AutoObjectVector& aScopeChain)
{
=======
bool nsJSUtils::GetScopeChainForElement(
    JSContext* aCx, Element* aElement,
    JS::MutableHandleVector<JSObject*> aScopeChain) {
>>>>>>> upstream-releases
  for (nsINode* cur = aElement; cur; cur = cur->GetScopeChainParent()) {
    if (!AddScopeChainItem(aCx, cur, aScopeChain)) {
      return false;
    }
  }

  return true;
}

/* static */
<<<<<<< HEAD
bool nsJSUtils::GetScopeChainForXBL(JSContext* aCx, Element* aElement,
                                    const nsXBLPrototypeBinding& aProtoBinding,
                                    JS::AutoObjectVector& aScopeChain) {
||||||| merged common ancestors
bool
nsJSUtils::GetScopeChainForXBL(JSContext* aCx,
                               Element* aElement,
                               const nsXBLPrototypeBinding& aProtoBinding,
                               JS::AutoObjectVector& aScopeChain)
{
=======
bool nsJSUtils::GetScopeChainForXBL(
    JSContext* aCx, Element* aElement,
    const nsXBLPrototypeBinding& aProtoBinding,
    JS::MutableHandleVector<JSObject*> aScopeChain) {
>>>>>>> upstream-releases
  if (!aElement) {
    return true;
  }

  if (!aProtoBinding.SimpleScopeChain()) {
    return GetScopeChainForElement(aCx, aElement, aScopeChain);
  }

  if (!AddScopeChainItem(aCx, aElement, aScopeChain)) {
    return false;
  }

  if (!AddScopeChainItem(aCx, aElement->OwnerDoc(), aScopeChain)) {
    return false;
  }
  return true;
}

/* static */
<<<<<<< HEAD
void nsJSUtils::ResetTimeZone() { JS::ResetTimeZone(); }
||||||| merged common ancestors
void
nsJSUtils::ResetTimeZone()
{
  JS::ResetTimeZone();
}
=======
void nsJSUtils::ResetTimeZone() { JS::ResetTimeZone(); }

/* static */
bool nsJSUtils::DumpEnabled() {
#if defined(DEBUG) || defined(MOZ_ENABLE_JS_DUMP)
  return true;
#else
  return StaticPrefs::browser_dom_window_dump_enabled();
#endif
}
>>>>>>> upstream-releases

//
// nsDOMJSUtils.h
//

bool nsAutoJSString::init(const JS::Value& v) {
  // Note: it's okay to use danger::GetJSContext here instead of AutoJSAPI,
  // because the init() call below is careful not to run script (for instance,
  // it only calls JS::ToString for non-object values).
  JSContext* cx = danger::GetJSContext();
  if (!init(cx, v)) {
    JS_ClearPendingException(cx);
    return false;
  }

  return true;
}
