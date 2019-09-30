/* -*- Mode: C++; tab-width: 8; indent-tabs-mode: nil; c-basic-offset: 2 -*-
 * vim: set ts=8 sts=2 et sw=2 tw=80:
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/. */

/*
 * JS parser.
 *
 * This is a recursive-descent parser for the JavaScript language specified by
 * "The ECMAScript Language Specification" (Standard ECMA-262).  It uses
 * lexical and semantic feedback to disambiguate non-LL(1) structures.  It
 * generates trees of nodes induced by the recursive parsing (not precise
 * syntax trees, see Parser.h).  After tree construction, it rewrites trees to
 * fold constants and evaluate compile-time expressions.
 *
 * This parser attempts no error recovery.
 */

#include "frontend/Parser.h"

#include "mozilla/ArrayUtils.h"
#include "mozilla/Casting.h"
#include "mozilla/Range.h"
#include "mozilla/Sprintf.h"
#include "mozilla/TypeTraits.h"
#include "mozilla/Unused.h"
#include "mozilla/Utf8.h"
#include "mozilla/Variant.h"

#include <memory>
#include <new>

#include "jsnum.h"
#include "jstypes.h"

#include "builtin/ModuleObject.h"
#include "builtin/SelfHostingDefines.h"
#include "frontend/BytecodeCompiler.h"
#include "frontend/FoldConstants.h"
#include "frontend/ModuleSharedContext.h"
#include "frontend/ParseNode.h"
#include "frontend/ParseNodeVerify.h"
#include "frontend/TokenStream.h"
#include "irregexp/RegExpParser.h"
#include "js/RegExpFlags.h"  // JS::RegExpFlags
#include "vm/BytecodeUtil.h"
#include "vm/JSAtom.h"
#include "vm/JSContext.h"
#include "vm/JSFunction.h"
#include "vm/JSScript.h"
#include "vm/ModuleBuilder.h"  // js::ModuleBuilder
#include "vm/RegExpObject.h"
#include "vm/SelfHosting.h"
#include "vm/StringType.h"
#include "wasm/AsmJS.h"

#include "frontend/ParseContext-inl.h"
#include "frontend/SharedContext-inl.h"
#include "vm/EnvironmentObject-inl.h"

using namespace js;

using mozilla::AssertedCast;
using mozilla::AsVariant;
using mozilla::Maybe;
using mozilla::Nothing;
using mozilla::PodCopy;
using mozilla::PodZero;
using mozilla::PointerRangeSize;
using mozilla::Some;
using mozilla::Unused;
using mozilla::Utf8Unit;

using JS::AutoGCRooter;
using JS::ReadOnlyCompileOptions;
using JS::RegExpFlags;

namespace js {
namespace frontend {

using DeclaredNamePtr = ParseContext::Scope::DeclaredNamePtr;
using AddDeclaredNamePtr = ParseContext::Scope::AddDeclaredNamePtr;
using BindingIter = ParseContext::Scope::BindingIter;
using UsedNamePtr = UsedNameTracker::UsedNameMap::Ptr;

using BindingNameVector = Vector<BindingName, 6>;

<<<<<<< HEAD
template <class T, class U>
static inline void PropagateTransitiveParseFlags(const T* inner, U* outer) {
  if (inner->bindingsAccessedDynamically()) {
    outer->setBindingsAccessedDynamically();
  }
  if (inner->hasDebuggerStatement()) {
    outer->setHasDebuggerStatement();
  }
  if (inner->hasDirectEval()) {
    outer->setHasDirectEval();
  }
||||||| merged common ancestors
// Read a token. Report an error and return null() if that token doesn't match
// to the condition.  Do not use MUST_MATCH_TOKEN_INTERNAL directly.
#define MUST_MATCH_TOKEN_INTERNAL(cond, modifier, errorReport, failureValue)                \
    JS_BEGIN_MACRO                                                                          \
        TokenKind token;                                                                    \
        if (!tokenStream.getToken(&token, modifier))                                        \
            return failureValue;                                                            \
        if (!(cond)) {                                                                      \
            errorReport;                                                                    \
            return failureValue;                                                            \
        }                                                                                   \
    JS_END_MACRO

#define MUST_MATCH_TOKEN_MOD_OR(tt, modifier, errorNumber, failureValue) \
    MUST_MATCH_TOKEN_INTERNAL(token == tt, modifier, error(errorNumber), failureValue)

#define MUST_MATCH_TOKEN_MOD(tt, modifier, errorNumber) \
    MUST_MATCH_TOKEN_MOD_OR(tt, modifier, errorNumber, null())

#define MUST_MATCH_TOKEN_OR(tt, errorNumber, failureValue) \
    MUST_MATCH_TOKEN_MOD_OR(tt, TokenStream::None, errorNumber, failureValue)

#define MUST_MATCH_TOKEN(tt, errorNumber) \
    MUST_MATCH_TOKEN_OR(tt, errorNumber, null())

#define MUST_MATCH_TOKEN_FUNC_MOD_OR(func, modifier, errorNumber, failureValue) \
    MUST_MATCH_TOKEN_INTERNAL((func)(token), modifier, error(errorNumber), failureValue)

#define MUST_MATCH_TOKEN_FUNC_OR(func, errorNumber, failureValue) \
    MUST_MATCH_TOKEN_FUNC_MOD_OR(func, TokenStream::None, errorNumber, failureValue)

#define MUST_MATCH_TOKEN_FUNC(func, errorNumber) \
    MUST_MATCH_TOKEN_FUNC_OR(func, errorNumber, null())

#define MUST_MATCH_TOKEN_MOD_WITH_REPORT_OR(tt, modifier, errorReport, failureValue) \
    MUST_MATCH_TOKEN_INTERNAL(token == tt, modifier, errorReport, failureValue)

#define MUST_MATCH_TOKEN_MOD_WITH_REPORT(tt, modifier, errorReport) \
    MUST_MATCH_TOKEN_MOD_WITH_REPORT_OR(tt, modifier, errorReport, null())

template <class T, class U>
static inline void
PropagateTransitiveParseFlags(const T* inner, U* outer)
{
    if (inner->bindingsAccessedDynamically()) {
        outer->setBindingsAccessedDynamically();
    }
    if (inner->hasDebuggerStatement()) {
        outer->setHasDebuggerStatement();
    }
    if (inner->hasDirectEval()) {
        outer->setHasDirectEval();
    }
}

static const char*
DeclarationKindString(DeclarationKind kind)
{
    switch (kind) {
      case DeclarationKind::PositionalFormalParameter:
      case DeclarationKind::FormalParameter:
        return "formal parameter";
      case DeclarationKind::CoverArrowParameter:
        return "cover arrow parameter";
      case DeclarationKind::Var:
        return "var";
      case DeclarationKind::Let:
        return "let";
      case DeclarationKind::Const:
        return "const";
      case DeclarationKind::Class:
        return "class";
      case DeclarationKind::Import:
        return "import";
      case DeclarationKind::BodyLevelFunction:
      case DeclarationKind::ModuleBodyLevelFunction:
      case DeclarationKind::LexicalFunction:
      case DeclarationKind::SloppyLexicalFunction:
        return "function";
      case DeclarationKind::VarForAnnexBLexicalFunction:
        return "annex b var";
      case DeclarationKind::ForOfVar:
        return "var in for-of";
      case DeclarationKind::SimpleCatchParameter:
      case DeclarationKind::CatchParameter:
        return "catch parameter";
    }

    MOZ_CRASH("Bad DeclarationKind");
}

static bool
StatementKindIsBraced(StatementKind kind)
{
    return kind == StatementKind::Block ||
           kind == StatementKind::Switch ||
           kind == StatementKind::Try ||
           kind == StatementKind::Catch ||
           kind == StatementKind::Finally ||
           kind == StatementKind::Class;
}

void
ParseContext::Scope::dump(ParseContext* pc)
{
    JSContext* cx = pc->sc()->context;

    fprintf(stdout, "ParseScope %p", this);

    fprintf(stdout, "\n  decls:\n");
    for (DeclaredNameMap::Range r = declared_->all(); !r.empty(); r.popFront()) {
        UniqueChars bytes = AtomToPrintableString(cx, r.front().key());
        if (!bytes) {
            return;
        }
        DeclaredNameInfo& info = r.front().value().wrapped;
        fprintf(stdout, "    %s %s%s\n",
                DeclarationKindString(info.kind()),
                bytes.get(),
                info.closedOver() ? " (closed over)" : "");
    }

    fprintf(stdout, "\n");
}

bool
ParseContext::Scope::addPossibleAnnexBFunctionBox(ParseContext* pc, FunctionBox* funbox)
{
    if (!possibleAnnexBFunctionBoxes_) {
        if (!possibleAnnexBFunctionBoxes_.acquire(pc->sc()->context)) {
            return false;
        }
    }

    return maybeReportOOM(pc, possibleAnnexBFunctionBoxes_->append(funbox));
}

bool
ParseContext::Scope::propagateAndMarkAnnexBFunctionBoxes(ParseContext* pc)
{
    // Strict mode doesn't have wack Annex B function semantics.
    if (pc->sc()->strict() ||
        !possibleAnnexBFunctionBoxes_ ||
        possibleAnnexBFunctionBoxes_->empty())
    {
        return true;
    }

    if (this == &pc->varScope()) {
        // Base case: actually declare the Annex B vars and mark applicable
        // function boxes as Annex B.
        RootedPropertyName name(pc->sc()->context);
        Maybe<DeclarationKind> redeclaredKind;
        uint32_t unused;
        for (FunctionBox* funbox : *possibleAnnexBFunctionBoxes_) {
            if (pc->annexBAppliesToLexicalFunctionInInnermostScope(funbox)) {
                name = funbox->function()->explicitName()->asPropertyName();
                if (!pc->tryDeclareVar(name,
                                       DeclarationKind::VarForAnnexBLexicalFunction,
                                       DeclaredNameInfo::npos, &redeclaredKind, &unused))
                {
                    return false;
                }

                MOZ_ASSERT(!redeclaredKind);
                funbox->isAnnexB = true;
            }
        }
    } else {
        // Inner scope case: propagate still applicable function boxes to the
        // enclosing scope.
        for (FunctionBox* funbox : *possibleAnnexBFunctionBoxes_) {
            if (pc->annexBAppliesToLexicalFunctionInInnermostScope(funbox)) {
                if (!enclosing()->addPossibleAnnexBFunctionBox(pc, funbox)) {
                    return false;
                }
            }
        }
    }

    return true;
}

static bool
DeclarationKindIsCatchParameter(DeclarationKind kind)
{
    return kind == DeclarationKind::SimpleCatchParameter ||
           kind == DeclarationKind::CatchParameter;
}

bool
ParseContext::Scope::addCatchParameters(ParseContext* pc, Scope& catchParamScope)
{
    if (pc->useAsmOrInsideUseAsm()) {
        return true;
    }

    for (DeclaredNameMap::Range r = catchParamScope.declared_->all(); !r.empty(); r.popFront()) {
        DeclarationKind kind = r.front().value()->kind();
        uint32_t pos = r.front().value()->pos();
        MOZ_ASSERT(DeclarationKindIsCatchParameter(kind));
        JSAtom* name = r.front().key();
        AddDeclaredNamePtr p = lookupDeclaredNameForAdd(name);
        MOZ_ASSERT(!p);
        if (!addDeclaredName(pc, p, name, kind, pos)) {
            return false;
        }
    }

    return true;
}

void
ParseContext::Scope::removeCatchParameters(ParseContext* pc, Scope& catchParamScope)
{
    if (pc->useAsmOrInsideUseAsm()) {
        return;
    }

    for (DeclaredNameMap::Range r = catchParamScope.declared_->all(); !r.empty(); r.popFront()) {
        DeclaredNamePtr p = declared_->lookup(r.front().key());
        MOZ_ASSERT(p);

        // This check is needed because the catch body could have declared
        // vars, which would have been added to catchParamScope.
        if (DeclarationKindIsCatchParameter(r.front().value()->kind())) {
            declared_->remove(p);
        }
    }
}

void
SharedContext::computeAllowSyntax(Scope* scope)
{
    for (ScopeIter si(scope); si; si++) {
        if (si.kind() == ScopeKind::Function) {
            JSFunction* fun = si.scope()->as<FunctionScope>().canonicalFunction();
            if (fun->isArrow()) {
                continue;
            }
            allowNewTarget_ = true;
            allowSuperProperty_ = fun->allowSuperProperty();
            allowSuperCall_ = fun->isDerivedClassConstructor();
            return;
        }
    }
}

void
SharedContext::computeThisBinding(Scope* scope)
{
    for (ScopeIter si(scope); si; si++) {
        if (si.kind() == ScopeKind::Module) {
            thisBinding_ = ThisBinding::Module;
            return;
        }

        if (si.kind() == ScopeKind::Function) {
            JSFunction* fun = si.scope()->as<FunctionScope>().canonicalFunction();

            // Arrow functions don't have their own `this` binding.
            if (fun->isArrow()) {
                continue;
            }

            // Derived class constructors (including nested arrow functions and
            // eval) need TDZ checks when accessing |this|.
            if (fun->isDerivedClassConstructor()) {
                needsThisTDZChecks_ = true;
            }

            thisBinding_ = ThisBinding::Function;
            return;
        }
    }

    thisBinding_ = ThisBinding::Global;
}

void
SharedContext::computeInWith(Scope* scope)
{
    for (ScopeIter si(scope); si; si++) {
        if (si.kind() == ScopeKind::With) {
            inWith_ = true;
            break;
        }
    }
}

EvalSharedContext::EvalSharedContext(JSContext* cx, JSObject* enclosingEnv,
                                     Scope* enclosingScope, Directives directives,
                                     bool extraWarnings)
  : SharedContext(cx, Kind::Eval, directives, extraWarnings),
    enclosingScope_(cx, enclosingScope),
    bindings(cx)
{
    computeAllowSyntax(enclosingScope);
    computeInWith(enclosingScope);
    computeThisBinding(enclosingScope);

    // If this eval is in response to Debugger.Frame.eval, we may have been
    // passed an incomplete scope chain. In order to better determine the 'this'
    // binding type, we traverse the environment chain, looking for a CallObject
    // and recompute the binding type based on its body scope.
    //
    // NOTE: A non-debug eval in a non-syntactic environment will also trigger
    // this code. In that case, we should still compute the same binding type.
    if (enclosingEnv && enclosingScope->hasOnChain(ScopeKind::NonSyntactic)) {
        JSObject* env = enclosingEnv;
        while (env) {
            // Look at target of any DebugEnvironmentProxy, but be sure to use
            // enclosingEnvironment() of the proxy itself.
            JSObject* unwrapped = env;
            if (env->is<DebugEnvironmentProxy>()) {
                unwrapped = &env->as<DebugEnvironmentProxy>().environment();
            }

            if (unwrapped->is<CallObject>()) {
                JSFunction* callee = &unwrapped->as<CallObject>().callee();
                computeThisBinding(callee->nonLazyScript()->bodyScope());
                break;
            }

            env = env->enclosingEnvironment();
        }
    }
}

ParseContext::ParseContext(JSContext* cx, ParseContext*& parent, SharedContext* sc,
                           ErrorReporter& errorReporter, class UsedNameTracker& usedNames,
                           Directives* newDirectives, bool isFull)
  : Nestable<ParseContext>(&parent),
    traceLog_(sc->context,
              isFull
              ? TraceLogger_ParsingFull
              : TraceLogger_ParsingSyntax,
              errorReporter),
    sc_(sc),
    errorReporter_(errorReporter),
    innermostStatement_(nullptr),
    innermostScope_(nullptr),
    varScope_(nullptr),
    positionalFormalParameterNames_(cx->frontendCollectionPool()),
    closedOverBindingsForLazy_(cx->frontendCollectionPool()),
    innerFunctionsForLazy(cx, GCVector<JSFunction*, 8>(cx)),
    newDirectives(newDirectives),
    lastYieldOffset(NoYieldOffset),
    lastAwaitOffset(NoAwaitOffset),
    scriptId_(usedNames.nextScriptId()),
    isStandaloneFunctionBody_(false),
    superScopeNeedsHomeObject_(false)
{
    if (isFunctionBox()) {
        if (functionBox()->function()->isNamedLambda()) {
            namedLambdaScope_.emplace(cx, parent, usedNames);
        }
        functionScope_.emplace(cx, parent, usedNames);
    }
}

bool
ParseContext::init()
{
    if (scriptId_ == UINT32_MAX) {
        errorReporter_.reportErrorNoOffset(JSMSG_NEED_DIET, js_script_str);
        return false;
    }

    JSContext* cx = sc()->context;

    if (isFunctionBox()) {
        // Named lambdas always need a binding for their own name. If this
        // binding is closed over when we finish parsing the function in
        // finishExtraFunctionScopes, the function box needs to be marked as
        // needing a dynamic DeclEnv object.
        RootedFunction fun(cx, functionBox()->function());
        if (fun->isNamedLambda()) {
            if (!namedLambdaScope_->init(this)) {
                return false;
            }
            AddDeclaredNamePtr p =
                namedLambdaScope_->lookupDeclaredNameForAdd(fun->explicitName());
            MOZ_ASSERT(!p);
            if (!namedLambdaScope_->addDeclaredName(this, p, fun->explicitName(),
                                                    DeclarationKind::Const,
                                                    DeclaredNameInfo::npos))
            {
                return false;
            }
        }

        if (!functionScope_->init(this)) {
            return false;
        }

        if (!positionalFormalParameterNames_.acquire(cx)) {
            return false;
        }
    }

    if (!closedOverBindingsForLazy_.acquire(cx)) {
        return false;
    }

    return true;
}

bool
UsedNameTracker::noteUse(JSContext* cx, JSAtom* name, uint32_t scriptId, uint32_t scopeId)
{
    if (UsedNameMap::AddPtr p = map_.lookupForAdd(name)) {
        if (!p || !p->value().noteUsedInScope(scriptId, scopeId)) {
            return false;
        }
    } else {
        UsedNameInfo info(cx);
        if (!info.noteUsedInScope(scriptId, scopeId)) {
            return false;
        }
        if (!map_.add(p, name, std::move(info))) {
            return false;
        }
    }

    return true;
}

void
UsedNameTracker::UsedNameInfo::resetToScope(uint32_t scriptId, uint32_t scopeId)
{
    while (!uses_.empty()) {
        Use& innermost = uses_.back();
        if (innermost.scopeId < scopeId) {
            break;
        }
        MOZ_ASSERT(innermost.scriptId >= scriptId);
        uses_.popBack();
    }
}

void
UsedNameTracker::rewind(RewindToken token)
{
    scriptCounter_ = token.scriptId;
    scopeCounter_ = token.scopeId;

    for (UsedNameMap::Range r = map_.all(); !r.empty(); r.popFront()) {
        r.front().value().resetToScope(token.scriptId, token.scopeId);
    }
}

#ifdef DEBUG
bool
FunctionBox::atomsAreKept()
{
    return context->zone()->hasKeptAtoms();
}
#endif

FunctionBox::FunctionBox(JSContext* cx, ObjectBox* traceListHead,
                         JSFunction* fun, uint32_t toStringStart,
                         Directives directives, bool extraWarnings,
                         GeneratorKind generatorKind, FunctionAsyncKind asyncKind)
  : ObjectBox(fun, traceListHead),
    SharedContext(cx, Kind::FunctionBox, directives, extraWarnings),
    enclosingScope_(nullptr),
    namedLambdaBindings_(nullptr),
    functionScopeBindings_(nullptr),
    extraVarScopeBindings_(nullptr),
    functionNode(nullptr),
    bufStart(0),
    bufEnd(0),
    startLine(1),
    startColumn(0),
    toStringStart(toStringStart),
    toStringEnd(0),
    length(0),
    isGenerator_(generatorKind == GeneratorKind::Generator),
    isAsync_(asyncKind == FunctionAsyncKind::AsyncFunction),
    hasDestructuringArgs(false),
    hasParameterExprs(false),
    hasDirectEvalInParameterExpr(false),
    hasDuplicateParameters(false),
    useAsm(false),
    isAnnexB(false),
    wasEmitted(false),
    declaredArguments(false),
    usesArguments(false),
    usesApply(false),
    usesThis(false),
    usesReturn(false),
    hasRest_(false),
    hasExprBody_(false),
    hasExtensibleScope_(false),
    argumentsHasLocalBinding_(false),
    definitelyNeedsArgsObj_(false),
    needsHomeObject_(false),
    isDerivedClassConstructor_(false),
    hasThisBinding_(false),
    hasInnerFunctions_(false)
{
    // Functions created at parse time may be set singleton after parsing and
    // baked into JIT code, so they must be allocated tenured. They are held by
    // the JSScript so cannot be collected during a minor GC anyway.
    MOZ_ASSERT(fun->isTenured());
}

void
FunctionBox::initFromLazyFunction()
{
    JSFunction* fun = function();
    if (fun->lazyScript()->isDerivedClassConstructor()) {
        setDerivedClassConstructor();
    }
    if (fun->lazyScript()->needsHomeObject()) {
        setNeedsHomeObject();
    }
    if (fun->lazyScript()->hasEnclosingScope()) {
        enclosingScope_ = fun->lazyScript()->enclosingScope();
    } else {
        enclosingScope_ = nullptr;
    }
    initWithEnclosingScope(enclosingScope_);
}

void
FunctionBox::initStandaloneFunction(Scope* enclosingScope)
{
    // Standalone functions are Function or Generator constructors and are
    // always scoped to the global.
    MOZ_ASSERT(enclosingScope->is<GlobalScope>());
    enclosingScope_ = enclosingScope;
    allowNewTarget_ = true;
    thisBinding_ = ThisBinding::Function;
}

void
FunctionBox::initWithEnclosingParseContext(ParseContext* enclosing, FunctionSyntaxKind kind)
{
    SharedContext* sc = enclosing->sc();
    useAsm = sc->isFunctionBox() && sc->asFunctionBox()->useAsmOrInsideUseAsm();

    JSFunction* fun = function();

    // Arrow functions don't have their own `this` binding.
    if (fun->isArrow()) {
        allowNewTarget_ = sc->allowNewTarget();
        allowSuperProperty_ = sc->allowSuperProperty();
        allowSuperCall_ = sc->allowSuperCall();
        needsThisTDZChecks_ = sc->needsThisTDZChecks();
        thisBinding_ = sc->thisBinding();
    } else {
        allowNewTarget_ = true;
        allowSuperProperty_ = fun->allowSuperProperty();

        if (IsConstructorKind(kind)) {
            auto stmt = enclosing->findInnermostStatement<ParseContext::ClassStatement>();
            MOZ_ASSERT(stmt);
            stmt->constructorBox = this;

            if (kind == FunctionSyntaxKind::DerivedClassConstructor) {
                setDerivedClassConstructor();
                allowSuperCall_ = true;
                needsThisTDZChecks_ = true;
            }
        }

        thisBinding_ = ThisBinding::Function;
    }

    if (sc->inWith()) {
        inWith_ = true;
    } else {
        auto isWith = [](ParseContext::Statement* stmt) {
            return stmt->kind() == StatementKind::With;
        };

        inWith_ = enclosing->findInnermostStatement(isWith);
    }
}

void
FunctionBox::initWithEnclosingScope(Scope* enclosingScope)
{
    if (!function()->isArrow()) {
        allowNewTarget_ = true;
        allowSuperProperty_ = function()->allowSuperProperty();

        if (isDerivedClassConstructor()) {
            setDerivedClassConstructor();
            allowSuperCall_ = true;
            needsThisTDZChecks_ = true;
        }

        thisBinding_ = ThisBinding::Function;
    } else {
        computeAllowSyntax(enclosingScope);
        computeThisBinding(enclosingScope);
    }

    computeInWith(enclosingScope);
}

void
FunctionBox::setEnclosingScopeForInnerLazyFunction(Scope* enclosingScope)
{
    MOZ_ASSERT(isLazyFunctionWithoutEnclosingScope());

    // For lazy functions inside a function which is being compiled, we cache
    // the incomplete scope object while compiling, and store it to the
    // LazyScript once the enclosing script successfully finishes compilation
    // in FunctionBox::finish.
    enclosingScope_ = enclosingScope;
=======
template <class T, class U>
static inline void PropagateTransitiveParseFlags(const T* inner, U* outer) {
  if (inner->bindingsAccessedDynamically()) {
    outer->setBindingsAccessedDynamically();
  }
  if (inner->hasDirectEval()) {
    outer->setHasDirectEval();
  }
}

static bool StatementKindIsBraced(StatementKind kind) {
  return kind == StatementKind::Block || kind == StatementKind::Switch ||
         kind == StatementKind::Try || kind == StatementKind::Catch ||
         kind == StatementKind::Finally || kind == StatementKind::Class;
}

template <class ParseHandler, typename Unit>
inline typename GeneralParser<ParseHandler, Unit>::FinalParser*
GeneralParser<ParseHandler, Unit>::asFinalParser() {
  static_assert(
      mozilla::IsBaseOf<GeneralParser<ParseHandler, Unit>, FinalParser>::value,
      "inheritance relationship required by the static_cast<> below");

  return static_cast<FinalParser*>(this);
}

template <class ParseHandler, typename Unit>
inline const typename GeneralParser<ParseHandler, Unit>::FinalParser*
GeneralParser<ParseHandler, Unit>::asFinalParser() const {
  static_assert(
      mozilla::IsBaseOf<GeneralParser<ParseHandler, Unit>, FinalParser>::value,
      "inheritance relationship required by the static_cast<> below");

  return static_cast<const FinalParser*>(this);
}

template <class ParseHandler, typename Unit>
template <typename ConditionT, typename ErrorReportT>
bool GeneralParser<ParseHandler, Unit>::mustMatchTokenInternal(
    ConditionT condition, ErrorReportT errorReport) {
  MOZ_ASSERT(condition(TokenKind::Div) == false);
  MOZ_ASSERT(condition(TokenKind::DivAssign) == false);
  MOZ_ASSERT(condition(TokenKind::RegExp) == false);

  TokenKind actual;
  if (!tokenStream.getToken(&actual, TokenStream::SlashIsInvalid)) {
    return false;
  }
  if (!condition(actual)) {
    errorReport(actual);
    return false;
  }
  return true;
}

ParserSharedBase::ParserSharedBase(JSContext* cx, LifoAlloc& alloc,
                                   UsedNameTracker& usedNames,
                                   ScriptSourceObject* sourceObject, Kind kind)
    : JS::AutoGCRooter(
          cx,
#ifdef JS_BUILD_BINAST
          kind == Kind::Parser ? JS::AutoGCRooter::Tag::Parser
                               : JS::AutoGCRooter::Tag::BinASTParser
#else
          JS::AutoGCRooter::Tag::Parser
#endif
          ),
      cx_(cx),
      alloc_(alloc),
      traceListHead_(nullptr),
      pc_(nullptr),
      usedNames_(usedNames),
      sourceObject_(cx, sourceObject),
      keepAtoms_(cx) {
  cx->frontendCollectionPool().addActiveCompilation();
  tempPoolMark_ = alloc_.mark();
}

ParserSharedBase::~ParserSharedBase() {
  alloc_.release(tempPoolMark_);

  /*
   * The parser can allocate enormous amounts of memory for large functions.
   * Eagerly free the memory now (which otherwise won't be freed until the
   * next GC) to avoid unnecessary OOMs.
   */
  alloc_.freeAllIfHugeAndUnused();

  cx_->frontendCollectionPool().removeActiveCompilation();
}

ParserBase::ParserBase(JSContext* cx, LifoAlloc& alloc,
                       const ReadOnlyCompileOptions& options,
                       bool foldConstants, UsedNameTracker& usedNames,
                       ScriptSourceObject* sourceObject, ParseGoal parseGoal)
    : ParserSharedBase(cx, alloc, usedNames, sourceObject,
                       ParserSharedBase::Kind::Parser),
      anyChars(cx, options, this),
      ss(nullptr),
      foldConstants_(foldConstants),
#ifdef DEBUG
      checkOptionsCalled_(false),
#endif
      isUnexpectedEOF_(false),
      awaitHandling_(AwaitIsName),
      inParametersOfAsyncFunction_(false),
      parseGoal_(uint8_t(parseGoal)) {
}

bool ParserBase::checkOptions() {
#ifdef DEBUG
  checkOptionsCalled_ = true;
#endif

  return anyChars.checkOptions();
}

ParserBase::~ParserBase() { MOZ_ASSERT(checkOptionsCalled_); }

template <class ParseHandler>
PerHandlerParser<ParseHandler>::PerHandlerParser(
    JSContext* cx, LifoAlloc& alloc, const ReadOnlyCompileOptions& options,
    bool foldConstants, UsedNameTracker& usedNames,
    LazyScript* lazyOuterFunction, ScriptSourceObject* sourceObject,
    ParseGoal parseGoal, void* internalSyntaxParser)
    : ParserBase(cx, alloc, options, foldConstants, usedNames, sourceObject,
                 parseGoal),
      handler_(cx, alloc, lazyOuterFunction),
      internalSyntaxParser_(internalSyntaxParser) {}

template <class ParseHandler, typename Unit>
GeneralParser<ParseHandler, Unit>::GeneralParser(
    JSContext* cx, LifoAlloc& alloc, const ReadOnlyCompileOptions& options,
    const Unit* units, size_t length, bool foldConstants,
    UsedNameTracker& usedNames, SyntaxParser* syntaxParser,
    LazyScript* lazyOuterFunction, ScriptSourceObject* sourceObject,
    ParseGoal parseGoal)
    : Base(cx, alloc, options, foldConstants, usedNames, syntaxParser,
           lazyOuterFunction, sourceObject, parseGoal),
      tokenStream(cx, options, units, length) {}

template <typename Unit>
void Parser<SyntaxParseHandler, Unit>::setAwaitHandling(
    AwaitHandling awaitHandling) {
  this->awaitHandling_ = awaitHandling;
}

template <typename Unit>
void Parser<FullParseHandler, Unit>::setAwaitHandling(
    AwaitHandling awaitHandling) {
  this->awaitHandling_ = awaitHandling;
  if (SyntaxParser* syntaxParser = getSyntaxParser()) {
    syntaxParser->setAwaitHandling(awaitHandling);
  }
}

template <class ParseHandler, typename Unit>
inline void GeneralParser<ParseHandler, Unit>::setAwaitHandling(
    AwaitHandling awaitHandling) {
  asFinalParser()->setAwaitHandling(awaitHandling);
}

template <typename Unit>
void Parser<SyntaxParseHandler, Unit>::setInParametersOfAsyncFunction(
    bool inParameters) {
  this->inParametersOfAsyncFunction_ = inParameters;
}

template <typename Unit>
void Parser<FullParseHandler, Unit>::setInParametersOfAsyncFunction(
    bool inParameters) {
  this->inParametersOfAsyncFunction_ = inParameters;
  if (SyntaxParser* syntaxParser = getSyntaxParser()) {
    syntaxParser->setInParametersOfAsyncFunction(inParameters);
  }
}

template <class ParseHandler, typename Unit>
inline void GeneralParser<ParseHandler, Unit>::setInParametersOfAsyncFunction(
    bool inParameters) {
  asFinalParser()->setInParametersOfAsyncFunction(inParameters);
}

template <typename BoxT, typename ArgT>
BoxT* ParserSharedBase::newTraceListNode(ArgT* arg) {
  MOZ_ASSERT(arg);

  /*
   * We use JSContext.tempLifoAlloc to allocate parsed objects and place them
   * on a list in this Parser to ensure GC safety. Thus the tempLifoAlloc
   * arenas containing the entries must be alive until we are done with
   * scanning, parsing and code generation for the whole script or top-level
   * function.
   */

  BoxT* box = alloc_.template new_<BoxT>(arg, traceListHead_);
  if (!box) {
    ReportOutOfMemory(cx_);
    return nullptr;
  }

  traceListHead_ = box;

  return box;
}

ObjectBox* ParserSharedBase::newObjectBox(JSObject* obj) {
  return newTraceListNode<ObjectBox, JSObject>(obj);
}

BigIntBox* ParserSharedBase::newBigIntBox(BigInt* val) {
  return newTraceListNode<BigIntBox, BigInt>(val);
}

template <class ParseHandler>
FunctionBox* PerHandlerParser<ParseHandler>::newFunctionBox(
    FunctionNodeType funNode, JSFunction* fun, uint32_t toStringStart,
    Directives inheritedDirectives, GeneratorKind generatorKind,
    FunctionAsyncKind asyncKind) {
  MOZ_ASSERT(fun);

  /*
   * We use JSContext.tempLifoAlloc to allocate parsed objects and place them
   * on a list in this Parser to ensure GC safety. Thus the tempLifoAlloc
   * arenas containing the entries must be alive until we are done with
   * scanning, parsing and code generation for the whole script or top-level
   * function.
   */
  FunctionBox* funbox = alloc_.new_<FunctionBox>(
      cx_, traceListHead_, fun, toStringStart, inheritedDirectives,
      options().extraWarningsOption, generatorKind, asyncKind);
  if (!funbox) {
    ReportOutOfMemory(cx_);
    return nullptr;
  }

  traceListHead_ = funbox;
  if (funNode) {
    handler_.setFunctionBox(funNode, funbox);
  }

  return funbox;
}

void ParserBase::trace(JSTracer* trc) {
  TraceListNode::TraceList(trc, traceListHead_);
}

void TraceParser(JSTracer* trc, AutoGCRooter* parser) {
  static_cast<ParserBase*>(parser)->trace(trc);
}

bool ParserBase::setSourceMapInfo() {
  // Not all clients initialize ss. Can't update info to an object that isn't
  // there.
  if (!ss) {
    return true;
  }

  if (anyChars.hasDisplayURL()) {
    if (!ss->setDisplayURL(cx_, anyChars.displayURL())) {
      return false;
    }
  }

  if (anyChars.hasSourceMapURL()) {
    MOZ_ASSERT(!ss->hasSourceMapURL());
    if (!ss->setSourceMapURL(cx_, anyChars.sourceMapURL())) {
      return false;
    }
  }

  /*
   * Source map URLs passed as a compile option (usually via a HTTP source map
   * header) override any source map urls passed as comment pragmas.
   */
  if (options().sourceMapURL()) {
    // Warn about the replacement, but use the new one.
    if (ss->hasSourceMapURL()) {
      if (!warningNoOffset(JSMSG_ALREADY_HAS_PRAGMA, ss->filename(),
                           "//# sourceMappingURL")) {
        return false;
      }
    }

    if (!ss->setSourceMapURL(cx_, options().sourceMapURL())) {
      return false;
    }
  }

  return true;
}

/*
 * Parse a top-level JS script.
 */
template <class ParseHandler, typename Unit>
typename ParseHandler::ListNodeType GeneralParser<ParseHandler, Unit>::parse() {
  MOZ_ASSERT(checkOptionsCalled_);

  Directives directives(options().strictOption);
  GlobalSharedContext globalsc(cx_, ScopeKind::Global, directives,
                               options().extraWarningsOption);
  SourceParseContext globalpc(this, &globalsc, /* newDirectives = */ nullptr);
  if (!globalpc.init()) {
    return null();
  }

  ParseContext::VarScope varScope(this);
  if (!varScope.init(pc_)) {
    return null();
  }

  ListNodeType stmtList = statementList(YieldIsName);
  if (!stmtList) {
    return null();
  }

  TokenKind tt;
  if (!tokenStream.getToken(&tt, TokenStream::SlashIsRegExp)) {
    return null();
  }
  if (tt != TokenKind::Eof) {
    error(JSMSG_GARBAGE_AFTER_INPUT, "script", TokenKindToDesc(tt));
    return null();
  }

  if (!CheckParseTree(cx_, alloc_, stmtList)) {
    return null();
  }

  if (foldConstants_) {
    Node node = stmtList;
    // Don't constant-fold inside "use asm" code, as this could create a parse
    // tree that doesn't type-check as asm.js.
    if (!pc_->useAsmOrInsideUseAsm()) {
      if (!FoldConstants(cx_, &node, &handler_)) {
        return null();
      }
    }
    stmtList = handler_.asList(node);
  }

  return stmtList;
}

/*
 * Strict mode forbids introducing new definitions for 'eval', 'arguments',
 * 'let', 'static', 'yield', or for any strict mode reserved word.
 */
bool ParserBase::isValidStrictBinding(PropertyName* name) {
  TokenKind tt = ReservedWordTokenKind(name);
  if (tt == TokenKind::Name) {
    return name != cx_->names().eval && name != cx_->names().arguments;
  }
  return tt != TokenKind::Let && tt != TokenKind::Static &&
         tt != TokenKind::Yield && !TokenKindIsStrictReservedWord(tt);
}

/*
 * Returns true if all parameter names are valid strict mode binding names and
 * no duplicate parameter names are present.
 */
bool ParserBase::hasValidSimpleStrictParameterNames() {
  MOZ_ASSERT(pc_->isFunctionBox() &&
             pc_->functionBox()->hasSimpleParameterList());

  if (pc_->functionBox()->hasDuplicateParameters) {
    return false;
  }

  for (auto* name : pc_->positionalFormalParameterNames()) {
    MOZ_ASSERT(name);
    if (!isValidStrictBinding(name->asPropertyName())) {
      return false;
    }
  }
  return true;
}

template <class ParseHandler, typename Unit>
void GeneralParser<ParseHandler, Unit>::reportMissingClosing(
    unsigned errorNumber, unsigned noteNumber, uint32_t openedPos) {
  auto notes = MakeUnique<JSErrorNotes>();
  if (!notes) {
    ReportOutOfMemory(pc_->sc()->cx_);
    return;
  }

  uint32_t line, column;
  tokenStream.computeLineAndColumn(openedPos, &line, &column);

  const size_t MaxWidth = sizeof("4294967295");
  char columnNumber[MaxWidth];
  SprintfLiteral(columnNumber, "%" PRIu32, column);
  char lineNumber[MaxWidth];
  SprintfLiteral(lineNumber, "%" PRIu32, line);

  if (!notes->addNoteASCII(pc_->sc()->cx_, getFilename(), 0, line, column,
                           GetErrorMessage, nullptr, noteNumber, lineNumber,
                           columnNumber)) {
    return;
  }

  errorWithNotes(std::move(notes), errorNumber);
}

template <class ParseHandler, typename Unit>
void GeneralParser<ParseHandler, Unit>::reportRedeclaration(
    HandlePropertyName name, DeclarationKind prevKind, TokenPos pos,
    uint32_t prevPos) {
  UniqueChars bytes = AtomToPrintableString(cx_, name);
  if (!bytes) {
    return;
  }

  if (prevPos == DeclaredNameInfo::npos) {
    errorAt(pos.begin, JSMSG_REDECLARED_VAR, DeclarationKindString(prevKind),
            bytes.get());
    return;
  }

  auto notes = MakeUnique<JSErrorNotes>();
  if (!notes) {
    ReportOutOfMemory(pc_->sc()->cx_);
    return;
  }

  uint32_t line, column;
  tokenStream.computeLineAndColumn(prevPos, &line, &column);

  const size_t MaxWidth = sizeof("4294967295");
  char columnNumber[MaxWidth];
  SprintfLiteral(columnNumber, "%" PRIu32, column);
  char lineNumber[MaxWidth];
  SprintfLiteral(lineNumber, "%" PRIu32, line);

  if (!notes->addNoteASCII(pc_->sc()->cx_, getFilename(), 0, line, column,
                           GetErrorMessage, nullptr, JSMSG_REDECLARED_PREV,
                           lineNumber, columnNumber)) {
    return;
  }

  errorWithNotesAt(std::move(notes), pos.begin, JSMSG_REDECLARED_VAR,
                   DeclarationKindString(prevKind), bytes.get());
}

// notePositionalFormalParameter is called for both the arguments of a regular
// function definition and the arguments specified by the Function
// constructor.
//
// The 'disallowDuplicateParams' bool indicates whether the use of another
// feature (destructuring or default arguments) disables duplicate arguments.
// (ECMA-262 requires us to support duplicate parameter names, but, for newer
// features, we consider the code to have "opted in" to higher standards and
// forbid duplicates.)
template <class ParseHandler, typename Unit>
bool GeneralParser<ParseHandler, Unit>::notePositionalFormalParameter(
    FunctionNodeType funNode, HandlePropertyName name, uint32_t beginPos,
    bool disallowDuplicateParams, bool* duplicatedParam) {
  if (AddDeclaredNamePtr p =
          pc_->functionScope().lookupDeclaredNameForAdd(name)) {
    if (disallowDuplicateParams) {
      error(JSMSG_BAD_DUP_ARGS);
      return false;
    }

    // Strict-mode disallows duplicate args. We may not know whether we are
    // in strict mode or not (since the function body hasn't been parsed).
    // In such cases, report will queue up the potential error and return
    // 'true'.
    if (pc_->sc()->needStrictChecks()) {
      UniqueChars bytes = AtomToPrintableString(cx_, name);
      if (!bytes) {
        return false;
      }
      if (!strictModeError(JSMSG_DUPLICATE_FORMAL, bytes.get())) {
        return false;
      }
    }

    *duplicatedParam = true;
  } else {
    DeclarationKind kind = DeclarationKind::PositionalFormalParameter;
    if (!pc_->functionScope().addDeclaredName(pc_, p, name, kind, beginPos)) {
      return false;
    }
  }

  if (!pc_->positionalFormalParameterNames().append(name)) {
    ReportOutOfMemory(cx_);
    return false;
  }

  NameNodeType paramNode = newName(name);
  if (!paramNode) {
    return false;
  }

  handler_.addFunctionFormalParameter(funNode, paramNode);
  return true;
>>>>>>> upstream-releases
}

<<<<<<< HEAD
static bool StatementKindIsBraced(StatementKind kind) {
  return kind == StatementKind::Block || kind == StatementKind::Switch ||
         kind == StatementKind::Try || kind == StatementKind::Catch ||
         kind == StatementKind::Finally || kind == StatementKind::Class;
||||||| merged common ancestors
void
FunctionBox::finish()
{
    if (!isLazyFunctionWithoutEnclosingScope()) {
        return;
    }
    MOZ_ASSERT(enclosingScope_);
    function()->lazyScript()->setEnclosingScope(enclosingScope_);
=======
template <class ParseHandler>
bool PerHandlerParser<ParseHandler>::noteDestructuredPositionalFormalParameter(
    FunctionNodeType funNode, Node destruct) {
  // Append an empty name to the positional formals vector to keep track of
  // argument slots when making FunctionScope::Data.
  if (!pc_->positionalFormalParameterNames().append(nullptr)) {
    ReportOutOfMemory(cx_);
    return false;
  }

  handler_.addFunctionFormalParameter(funNode, destruct);
  return true;
>>>>>>> upstream-releases
}

template <class ParseHandler, typename Unit>
<<<<<<< HEAD
inline typename GeneralParser<ParseHandler, Unit>::FinalParser*
GeneralParser<ParseHandler, Unit>::asFinalParser() {
  static_assert(
      mozilla::IsBaseOf<GeneralParser<ParseHandler, Unit>, FinalParser>::value,
      "inheritance relationship required by the static_cast<> below");
||||||| merged common ancestors
inline typename GeneralParser<ParseHandler, Unit>::FinalParser*
GeneralParser<ParseHandler, Unit>::asFinalParser()
{
    static_assert(mozilla::IsBaseOf<GeneralParser<ParseHandler, Unit>, FinalParser>::value,
                  "inheritance relationship required by the static_cast<> below");
=======
bool GeneralParser<ParseHandler, Unit>::
    checkLexicalDeclarationDirectlyWithinBlock(ParseContext::Statement& stmt,
                                               DeclarationKind kind,
                                               TokenPos pos) {
  MOZ_ASSERT(DeclarationKindIsLexical(kind));

  // It is an early error to declare a lexical binding not directly
  // within a block.
  if (!StatementKindIsBraced(stmt.kind()) &&
      stmt.kind() != StatementKind::ForLoopLexicalHead) {
    errorAt(pos.begin,
            stmt.kind() == StatementKind::Label
                ? JSMSG_LEXICAL_DECL_LABEL
                : JSMSG_LEXICAL_DECL_NOT_IN_BLOCK,
            DeclarationKindString(kind));
    return false;
  }
>>>>>>> upstream-releases

<<<<<<< HEAD
  return static_cast<FinalParser*>(this);
||||||| merged common ancestors
    return static_cast<FinalParser*>(this);
=======
  return true;
>>>>>>> upstream-releases
}

template <class ParseHandler, typename Unit>
<<<<<<< HEAD
inline const typename GeneralParser<ParseHandler, Unit>::FinalParser*
GeneralParser<ParseHandler, Unit>::asFinalParser() const {
  static_assert(
      mozilla::IsBaseOf<GeneralParser<ParseHandler, Unit>, FinalParser>::value,
      "inheritance relationship required by the static_cast<> below");

  return static_cast<const FinalParser*>(this);
}
||||||| merged common ancestors
inline const typename GeneralParser<ParseHandler, Unit>::FinalParser*
GeneralParser<ParseHandler, Unit>::asFinalParser() const
{
    static_assert(mozilla::IsBaseOf<GeneralParser<ParseHandler, Unit>, FinalParser>::value,
                  "inheritance relationship required by the static_cast<> below");
=======
bool GeneralParser<ParseHandler, Unit>::noteDeclaredName(
    HandlePropertyName name, DeclarationKind kind, TokenPos pos) {
  // The asm.js validator does all its own symbol-table management so, as an
  // optimization, avoid doing any work here.
  if (pc_->useAsmOrInsideUseAsm()) {
    return true;
  }

  switch (kind) {
    case DeclarationKind::Var:
    case DeclarationKind::BodyLevelFunction: {
      Maybe<DeclarationKind> redeclaredKind;
      uint32_t prevPos;
      if (!pc_->tryDeclareVar(name, kind, pos.begin, &redeclaredKind,
                              &prevPos)) {
        return false;
      }
>>>>>>> upstream-releases

<<<<<<< HEAD
template <class ParseHandler, typename Unit>
template <typename ConditionT, typename ErrorReportT>
bool GeneralParser<ParseHandler, Unit>::mustMatchTokenInternal(
    ConditionT condition, Modifier modifier, ErrorReportT errorReport) {
  TokenKind actual;
  if (!tokenStream.getToken(&actual, modifier)) {
    return false;
  }
  if (!condition(actual)) {
    errorReport(actual);
    return false;
  }
  return true;
}
||||||| merged common ancestors
    return static_cast<const FinalParser*>(this);
}
=======
      if (redeclaredKind) {
        reportRedeclaration(name, *redeclaredKind, pos, prevPos);
        return false;
      }
>>>>>>> upstream-releases

<<<<<<< HEAD
template <class ParseHandler, typename Unit>
void GeneralParser<ParseHandler, Unit>::error(unsigned errorNumber, ...) {
  va_list args;
  va_start(args, errorNumber);
||||||| merged common ancestors
template <class ParseHandler, typename Unit>
void
GeneralParser<ParseHandler, Unit>::error(unsigned errorNumber, ...)
{
    va_list args;
    va_start(args, errorNumber);
=======
      break;
    }

    case DeclarationKind::ModuleBodyLevelFunction: {
      MOZ_ASSERT(pc_->atModuleLevel());

      AddDeclaredNamePtr p = pc_->varScope().lookupDeclaredNameForAdd(name);
      if (p) {
        reportRedeclaration(name, p->value()->kind(), pos, p->value()->pos());
        return false;
      }

      if (!pc_->varScope().addDeclaredName(pc_, p, name, kind, pos.begin)) {
        return false;
      }

      // Body-level functions in modules are always closed over.
      pc_->varScope().lookupDeclaredName(name)->value()->setClosedOver();
>>>>>>> upstream-releases

<<<<<<< HEAD
  ErrorMetadata metadata;
  if (tokenStream.computeErrorMetadata(&metadata, pos().begin)) {
    ReportCompileError(context, std::move(metadata), nullptr, JSREPORT_ERROR,
                       errorNumber, args);
  }
||||||| merged common ancestors
    ErrorMetadata metadata;
    if (tokenStream.computeErrorMetadata(&metadata, pos().begin)) {
        ReportCompileError(context, std::move(metadata), nullptr, JSREPORT_ERROR, errorNumber, args);
    }
=======
      break;
    }
>>>>>>> upstream-releases

<<<<<<< HEAD
  va_end(args);
}
||||||| merged common ancestors
    va_end(args);
}
=======
    case DeclarationKind::FormalParameter: {
      // It is an early error if any non-positional formal parameter name
      // (e.g., destructuring formal parameter) is duplicated.
>>>>>>> upstream-releases

<<<<<<< HEAD
template <class ParseHandler, typename Unit>
void GeneralParser<ParseHandler, Unit>::errorWithNotes(
    UniquePtr<JSErrorNotes> notes, unsigned errorNumber, ...) {
  va_list args;
  va_start(args, errorNumber);
||||||| merged common ancestors
template <class ParseHandler, typename Unit>
void
GeneralParser<ParseHandler, Unit>::errorWithNotes(UniquePtr<JSErrorNotes> notes,
                                                  unsigned errorNumber, ...)
{
    va_list args;
    va_start(args, errorNumber);
=======
      AddDeclaredNamePtr p =
          pc_->functionScope().lookupDeclaredNameForAdd(name);
      if (p) {
        error(JSMSG_BAD_DUP_ARGS);
        return false;
      }

      if (!pc_->functionScope().addDeclaredName(pc_, p, name, kind,
                                                pos.begin)) {
        return false;
      }
>>>>>>> upstream-releases

<<<<<<< HEAD
  ErrorMetadata metadata;
  if (tokenStream.computeErrorMetadata(&metadata, pos().begin)) {
    ReportCompileError(context, std::move(metadata), std::move(notes),
                       JSREPORT_ERROR, errorNumber, args);
  }
||||||| merged common ancestors
    ErrorMetadata metadata;
    if (tokenStream.computeErrorMetadata(&metadata, pos().begin)) {
        ReportCompileError(context, std::move(metadata), std::move(notes), JSREPORT_ERROR, errorNumber,
                           args);
    }
=======
      break;
    }
>>>>>>> upstream-releases

<<<<<<< HEAD
  va_end(args);
}
||||||| merged common ancestors
    va_end(args);
}
=======
    case DeclarationKind::LexicalFunction: {
      ParseContext::Scope* scope = pc_->innermostScope();
      AddDeclaredNamePtr p = scope->lookupDeclaredNameForAdd(name);
      if (p) {
        reportRedeclaration(name, p->value()->kind(), pos, p->value()->pos());
        return false;
      }
>>>>>>> upstream-releases

<<<<<<< HEAD
template <class ParseHandler, typename Unit>
void GeneralParser<ParseHandler, Unit>::errorAt(uint32_t offset,
                                                unsigned errorNumber, ...) {
  va_list args;
  va_start(args, errorNumber);
||||||| merged common ancestors
template <class ParseHandler, typename Unit>
void
GeneralParser<ParseHandler, Unit>::errorAt(uint32_t offset, unsigned errorNumber, ...)
{
    va_list args;
    va_start(args, errorNumber);
=======
      if (!scope->addDeclaredName(pc_, p, name, kind, pos.begin)) {
        return false;
      }
>>>>>>> upstream-releases

<<<<<<< HEAD
  ErrorMetadata metadata;
  if (tokenStream.computeErrorMetadata(&metadata, offset)) {
    ReportCompileError(context, std::move(metadata), nullptr, JSREPORT_ERROR,
                       errorNumber, args);
  }
||||||| merged common ancestors
    ErrorMetadata metadata;
    if (tokenStream.computeErrorMetadata(&metadata, offset)) {
        ReportCompileError(context, std::move(metadata), nullptr, JSREPORT_ERROR, errorNumber, args);
    }
=======
      break;
    }
>>>>>>> upstream-releases

<<<<<<< HEAD
  va_end(args);
}
||||||| merged common ancestors
    va_end(args);
}
=======
    case DeclarationKind::SloppyLexicalFunction: {
      // Functions in block have complex allowances in sloppy mode for being
      // labelled that other lexical declarations do not have. Those checks
      // are more complex than calling checkLexicalDeclarationDirectlyWithin-
      // Block and are done in checkFunctionDefinition.
>>>>>>> upstream-releases

<<<<<<< HEAD
template <class ParseHandler, typename Unit>
void GeneralParser<ParseHandler, Unit>::errorWithNotesAt(
    UniquePtr<JSErrorNotes> notes, uint32_t offset, unsigned errorNumber, ...) {
  va_list args;
  va_start(args, errorNumber);
||||||| merged common ancestors
template <class ParseHandler, typename Unit>
void
GeneralParser<ParseHandler, Unit>::errorWithNotesAt(UniquePtr<JSErrorNotes> notes,
                                                    uint32_t offset, unsigned errorNumber, ...)
{
    va_list args;
    va_start(args, errorNumber);
=======
      ParseContext::Scope* scope = pc_->innermostScope();
      if (AddDeclaredNamePtr p = scope->lookupDeclaredNameForAdd(name)) {
        // It is usually an early error if there is another declaration
        // with the same name in the same scope.
        //
        // Sloppy lexical functions may redeclare other sloppy lexical
        // functions for web compatibility reasons.
        if (p->value()->kind() != DeclarationKind::SloppyLexicalFunction) {
          reportRedeclaration(name, p->value()->kind(), pos, p->value()->pos());
          return false;
        }
      } else {
        if (!scope->addDeclaredName(pc_, p, name, kind, pos.begin)) {
          return false;
        }
      }
>>>>>>> upstream-releases

<<<<<<< HEAD
  ErrorMetadata metadata;
  if (tokenStream.computeErrorMetadata(&metadata, offset)) {
    ReportCompileError(context, std::move(metadata), std::move(notes),
                       JSREPORT_ERROR, errorNumber, args);
  }
||||||| merged common ancestors
    ErrorMetadata metadata;
    if (tokenStream.computeErrorMetadata(&metadata, offset)) {
        ReportCompileError(context, std::move(metadata), std::move(notes), JSREPORT_ERROR, errorNumber,
                           args);
    }
=======
      break;
    }
>>>>>>> upstream-releases

<<<<<<< HEAD
  va_end(args);
}
||||||| merged common ancestors
    va_end(args);
}
=======
    case DeclarationKind::Let:
    case DeclarationKind::Const:
    case DeclarationKind::Class:
      // The BoundNames of LexicalDeclaration and ForDeclaration must not
      // contain 'let'. (CatchParameter is the only lexical binding form
      // without this restriction.)
      if (name == cx_->names().let) {
        errorAt(pos.begin, JSMSG_LEXICAL_DECL_DEFINES_LET);
        return false;
      }
>>>>>>> upstream-releases

<<<<<<< HEAD
template <class ParseHandler, typename Unit>
bool GeneralParser<ParseHandler, Unit>::warning(unsigned errorNumber, ...) {
  va_list args;
  va_start(args, errorNumber);
||||||| merged common ancestors
template <class ParseHandler, typename Unit>
bool
GeneralParser<ParseHandler, Unit>::warning(unsigned errorNumber, ...)
{
    va_list args;
    va_start(args, errorNumber);
=======
      MOZ_FALLTHROUGH;
>>>>>>> upstream-releases

<<<<<<< HEAD
  ErrorMetadata metadata;
  bool result = tokenStream.computeErrorMetadata(&metadata, pos().begin) &&
                anyChars.compileWarning(std::move(metadata), nullptr,
                                        JSREPORT_WARNING, errorNumber, args);
||||||| merged common ancestors
    ErrorMetadata metadata;
    bool result =
        tokenStream.computeErrorMetadata(&metadata, pos().begin) &&
        anyChars.compileWarning(std::move(metadata), nullptr, JSREPORT_WARNING, errorNumber, args);
=======
    case DeclarationKind::Import:
      // Module code is always strict, so 'let' is always a keyword and never a
      // name.
      MOZ_ASSERT(name != cx_->names().let);
      MOZ_FALLTHROUGH;
>>>>>>> upstream-releases

<<<<<<< HEAD
  va_end(args);
  return result;
}
||||||| merged common ancestors
    va_end(args);
    return result;
}
=======
    case DeclarationKind::SimpleCatchParameter:
    case DeclarationKind::CatchParameter: {
      if (ParseContext::Statement* stmt = pc_->innermostStatement()) {
        if (!checkLexicalDeclarationDirectlyWithinBlock(*stmt, kind, pos)) {
          return false;
        }
      }
>>>>>>> upstream-releases

<<<<<<< HEAD
template <class ParseHandler, typename Unit>
bool GeneralParser<ParseHandler, Unit>::warningAt(uint32_t offset,
                                                  unsigned errorNumber, ...) {
  va_list args;
  va_start(args, errorNumber);
||||||| merged common ancestors
template <class ParseHandler, typename Unit>
bool
GeneralParser<ParseHandler, Unit>::warningAt(uint32_t offset, unsigned errorNumber, ...)
{
    va_list args;
    va_start(args, errorNumber);
=======
      ParseContext::Scope* scope = pc_->innermostScope();
>>>>>>> upstream-releases

<<<<<<< HEAD
  ErrorMetadata metadata;
  bool result = tokenStream.computeErrorMetadata(&metadata, offset);
  if (result) {
    result = anyChars.compileWarning(std::move(metadata), nullptr,
                                     JSREPORT_WARNING, errorNumber, args);
  }
||||||| merged common ancestors
    ErrorMetadata metadata;
    bool result = tokenStream.computeErrorMetadata(&metadata, offset);
    if (result) {
        result =
            anyChars.compileWarning(std::move(metadata), nullptr, JSREPORT_WARNING, errorNumber, args);
    }
=======
      // For body-level lexically declared names in a function, it is an
      // early error if there is a formal parameter of the same name. This
      // needs a special check if there is an extra var scope due to
      // parameter expressions.
      if (pc_->isFunctionExtraBodyVarScopeInnermost()) {
        DeclaredNamePtr p = pc_->functionScope().lookupDeclaredName(name);
        if (p && DeclarationKindIsParameter(p->value()->kind())) {
          reportRedeclaration(name, p->value()->kind(), pos, p->value()->pos());
          return false;
        }
      }
>>>>>>> upstream-releases

<<<<<<< HEAD
  va_end(args);
  return result;
}
||||||| merged common ancestors
    va_end(args);
    return result;
}
=======
      // It is an early error if there is another declaration with the same
      // name in the same scope.
      AddDeclaredNamePtr p = scope->lookupDeclaredNameForAdd(name);
      if (p) {
        reportRedeclaration(name, p->value()->kind(), pos, p->value()->pos());
        return false;
      }
>>>>>>> upstream-releases

<<<<<<< HEAD
template <class ParseHandler, typename Unit>
bool GeneralParser<ParseHandler, Unit>::extraWarning(unsigned errorNumber,
                                                     ...) {
  va_list args;
  va_start(args, errorNumber);
||||||| merged common ancestors
template <class ParseHandler, typename Unit>
bool
GeneralParser<ParseHandler, Unit>::extraWarning(unsigned errorNumber, ...)
{
    va_list args;
    va_start(args, errorNumber);
=======
      if (!scope->addDeclaredName(pc_, p, name, kind, pos.begin)) {
        return false;
      }
>>>>>>> upstream-releases

<<<<<<< HEAD
  bool result = tokenStream.reportExtraWarningErrorNumberVA(
      nullptr, pos().begin, errorNumber, &args);
||||||| merged common ancestors
    bool result =
        tokenStream.reportExtraWarningErrorNumberVA(nullptr, pos().begin, errorNumber, &args);
=======
      break;
    }
>>>>>>> upstream-releases

<<<<<<< HEAD
  va_end(args);
  return result;
}
||||||| merged common ancestors
    va_end(args);
    return result;
}
=======
    case DeclarationKind::CoverArrowParameter:
      // CoverArrowParameter is only used as a placeholder declaration kind.
      break;
>>>>>>> upstream-releases

<<<<<<< HEAD
template <class ParseHandler, typename Unit>
bool GeneralParser<ParseHandler, Unit>::extraWarningAt(uint32_t offset,
                                                       unsigned errorNumber,
                                                       ...) {
  va_list args;
  va_start(args, errorNumber);
||||||| merged common ancestors
template <class ParseHandler, typename Unit>
bool
GeneralParser<ParseHandler, Unit>::extraWarningAt(uint32_t offset, unsigned errorNumber, ...)
{
    va_list args;
    va_start(args, errorNumber);
=======
    case DeclarationKind::PositionalFormalParameter:
      MOZ_CRASH(
          "Positional formal parameter names should use "
          "notePositionalFormalParameter");
      break;
>>>>>>> upstream-releases

<<<<<<< HEAD
  bool result = tokenStream.reportExtraWarningErrorNumberVA(nullptr, offset,
                                                            errorNumber, &args);
||||||| merged common ancestors
    bool result =
        tokenStream.reportExtraWarningErrorNumberVA(nullptr, offset, errorNumber, &args);
=======
    case DeclarationKind::VarForAnnexBLexicalFunction:
      MOZ_CRASH(
          "Synthesized Annex B vars should go through "
          "tryDeclareVarForAnnexBLexicalFunction");
      break;
  }
>>>>>>> upstream-releases

<<<<<<< HEAD
  va_end(args);
  return result;
||||||| merged common ancestors
    va_end(args);
    return result;
=======
  return true;
>>>>>>> upstream-releases
}

<<<<<<< HEAD
template <class ParseHandler, typename Unit>
bool GeneralParser<ParseHandler, Unit>::strictModeError(unsigned errorNumber,
                                                        ...) {
  va_list args;
  va_start(args, errorNumber);

  bool res = tokenStream.reportStrictModeErrorNumberVA(
      nullptr, pos().begin, pc->sc()->strict(), errorNumber, &args);
||||||| merged common ancestors
template <class ParseHandler, typename Unit>
bool
GeneralParser<ParseHandler, Unit>::strictModeError(unsigned errorNumber, ...)
{
    va_list args;
    va_start(args, errorNumber);

    bool res =
        tokenStream.reportStrictModeErrorNumberVA(nullptr, pos().begin, pc->sc()->strict(),
                                                  errorNumber, &args);
=======
bool ParserBase::noteUsedNameInternal(HandlePropertyName name) {
  // The asm.js validator does all its own symbol-table management so, as an
  // optimization, avoid doing any work here.
  if (pc_->useAsmOrInsideUseAsm()) {
    return true;
  }

  // Global bindings are properties and not actual bindings; we don't need
  // to know if they are closed over. So no need to track used name at the
  // global scope. It is not incorrect to track them, this is an
  // optimization.
  ParseContext::Scope* scope = pc_->innermostScope();
  if (pc_->sc()->isGlobalContext() && scope == &pc_->varScope()) {
    return true;
  }
>>>>>>> upstream-releases

<<<<<<< HEAD
  va_end(args);
  return res;
||||||| merged common ancestors
    va_end(args);
    return res;
=======
  return usedNames_.noteUse(cx_, name, pc_->scriptId(), scope->id());
>>>>>>> upstream-releases
}

<<<<<<< HEAD
template <class ParseHandler, typename Unit>
bool GeneralParser<ParseHandler, Unit>::strictModeErrorAt(uint32_t offset,
                                                          unsigned errorNumber,
                                                          ...) {
  va_list args;
  va_start(args, errorNumber);
||||||| merged common ancestors
template <class ParseHandler, typename Unit>
bool
GeneralParser<ParseHandler, Unit>::strictModeErrorAt(uint32_t offset, unsigned errorNumber, ...)
{
    va_list args;
    va_start(args, errorNumber);
=======
template <class ParseHandler>
bool PerHandlerParser<ParseHandler>::
    propagateFreeNamesAndMarkClosedOverBindings(ParseContext::Scope& scope) {
  // Now that we have all the declared names in the scope, check which
  // functions should exhibit Annex B semantics.
  if (!scope.propagateAndMarkAnnexBFunctionBoxes(pc_)) {
    return false;
  }
>>>>>>> upstream-releases

<<<<<<< HEAD
  bool res = tokenStream.reportStrictModeErrorNumberVA(
      nullptr, offset, pc->sc()->strict(), errorNumber, &args);
||||||| merged common ancestors
    bool res =
        tokenStream.reportStrictModeErrorNumberVA(nullptr, offset, pc->sc()->strict(),
                                                  errorNumber, &args);
=======
  if (handler_.canSkipLazyClosedOverBindings()) {
    // Scopes are nullptr-delimited in the LazyScript closed over bindings
    // array.
    while (JSAtom* name = handler_.nextLazyClosedOverBinding()) {
      scope.lookupDeclaredName(name)->value()->setClosedOver();
    }
    return true;
  }

  bool isSyntaxParser =
      mozilla::IsSame<ParseHandler, SyntaxParseHandler>::value;
  uint32_t scriptId = pc_->scriptId();
  uint32_t scopeId = scope.id();
  for (BindingIter bi = scope.bindings(pc_); bi; bi++) {
    if (UsedNamePtr p = usedNames_.lookup(bi.name())) {
      bool closedOver;
      p->value().noteBoundInScope(scriptId, scopeId, &closedOver);
      if (closedOver) {
        bi.setClosedOver();

        if (isSyntaxParser &&
            !pc_->closedOverBindingsForLazy().append(bi.name())) {
          ReportOutOfMemory(cx_);
          return false;
        }
      }
    }
  }

  // Append a nullptr to denote end-of-scope.
  if (isSyntaxParser && !pc_->closedOverBindingsForLazy().append(nullptr)) {
    ReportOutOfMemory(cx_);
    return false;
  }
>>>>>>> upstream-releases

<<<<<<< HEAD
  va_end(args);
  return res;
||||||| merged common ancestors
    va_end(args);
    return res;
=======
  return true;
>>>>>>> upstream-releases
}

<<<<<<< HEAD
bool ParserBase::warningNoOffset(unsigned errorNumber, ...) {
  va_list args;
  va_start(args, errorNumber);
||||||| merged common ancestors
bool
ParserBase::warningNoOffset(unsigned errorNumber, ...)
{
    va_list args;
    va_start(args, errorNumber);
=======
template <typename Unit>
bool Parser<FullParseHandler, Unit>::checkStatementsEOF() {
  // This is designed to be paired with parsing a statement list at the top
  // level.
  //
  // The statementList() call breaks on TokenKind::RightCurly, so make sure
  // we've reached EOF here.
  TokenKind tt;
  if (!tokenStream.peekToken(&tt, TokenStream::SlashIsRegExp)) {
    return false;
  }
  if (tt != TokenKind::Eof) {
    error(JSMSG_UNEXPECTED_TOKEN, "expression", TokenKindToDesc(tt));
    return false;
  }
  return true;
}
>>>>>>> upstream-releases

<<<<<<< HEAD
  ErrorMetadata metadata;
  anyChars.computeErrorMetadataNoOffset(&metadata);
||||||| merged common ancestors
    ErrorMetadata metadata;
    anyChars.computeErrorMetadataNoOffset(&metadata);
=======
template <typename Scope>
typename Scope::Data* NewEmptyBindingData(JSContext* cx, LifoAlloc& alloc,
                                          uint32_t numBindings) {
  using Data = typename Scope::Data;
  size_t allocSize = SizeOfData<typename Scope::Data>(numBindings);
  auto* bindings = alloc.newWithSize<Data>(allocSize, numBindings);
  if (!bindings) {
    ReportOutOfMemory(cx);
  }
  return bindings;
}
>>>>>>> upstream-releases

<<<<<<< HEAD
  bool result = anyChars.compileWarning(std::move(metadata), nullptr,
                                        JSREPORT_WARNING, errorNumber, args);
||||||| merged common ancestors
    bool result =
        anyChars.compileWarning(std::move(metadata), nullptr, JSREPORT_WARNING, errorNumber, args);
=======
namespace detail {
>>>>>>> upstream-releases

<<<<<<< HEAD
  va_end(args);
  return result;
||||||| merged common ancestors
    va_end(args);
    return result;
=======
template <class Data>
static MOZ_ALWAYS_INLINE BindingName* InitializeIndexedBindings(
    Data* data, BindingName* start, BindingName* cursor) {
  return cursor;
>>>>>>> upstream-releases
}

<<<<<<< HEAD
void ParserBase::errorNoOffset(unsigned errorNumber, ...) {
  va_list args;
  va_start(args, errorNumber);
||||||| merged common ancestors
void
ParserBase::errorNoOffset(unsigned errorNumber, ...)
{
    va_list args;
    va_start(args, errorNumber);
=======
template <class Data, typename UnsignedInteger, typename... Step>
static MOZ_ALWAYS_INLINE BindingName* InitializeIndexedBindings(
    Data* data, BindingName* start, BindingName* cursor,
    UnsignedInteger Data::*field, const BindingNameVector& bindings,
    Step&&... step) {
  data->*field = AssertedCast<UnsignedInteger>(PointerRangeSize(start, cursor));
>>>>>>> upstream-releases

<<<<<<< HEAD
  ErrorMetadata metadata;
  anyChars.computeErrorMetadataNoOffset(&metadata);
||||||| merged common ancestors
    ErrorMetadata metadata;
    anyChars.computeErrorMetadataNoOffset(&metadata);
=======
  BindingName* newCursor =
      std::uninitialized_copy(bindings.begin(), bindings.end(), cursor);
>>>>>>> upstream-releases

<<<<<<< HEAD
  ReportCompileError(context, std::move(metadata), nullptr, JSREPORT_ERROR,
                     errorNumber, args);

  va_end(args);
||||||| merged common ancestors
    ReportCompileError(context, std::move(metadata), nullptr, JSREPORT_ERROR, errorNumber, args);

    va_end(args);
=======
  return InitializeIndexedBindings(data, start, newCursor,
                                   std::forward<Step>(step)...);
>>>>>>> upstream-releases
}

<<<<<<< HEAD
ParserBase::ParserBase(JSContext* cx, LifoAlloc& alloc,
                       const ReadOnlyCompileOptions& options,
                       bool foldConstants, UsedNameTracker& usedNames,
                       ScriptSourceObject* sourceObject, ParseGoal parseGoal)
    : AutoGCRooter(cx, AutoGCRooter::Tag::Parser),
      context(cx),
      alloc(alloc),
      anyChars(cx, options, thisForCtor()),
      traceListHead(nullptr),
      pc(nullptr),
      usedNames(usedNames),
      ss(nullptr),
      sourceObject(cx, sourceObject),
      keepAtoms(cx),
      foldConstants(foldConstants),
#ifdef DEBUG
      checkOptionsCalled(false),
#endif
      isUnexpectedEOF_(false),
      awaitHandling_(AwaitIsName),
      inParametersOfAsyncFunction_(false),
      parseGoal_(uint8_t(parseGoal)) {
  cx->frontendCollectionPool().addActiveCompilation();
  tempPoolMark = alloc.mark();
}

bool ParserBase::checkOptions() {
||||||| merged common ancestors
ParserBase::ParserBase(JSContext* cx, LifoAlloc& alloc,
                       const ReadOnlyCompileOptions& options,
                       bool foldConstants,
                       UsedNameTracker& usedNames,
                       ScriptSourceObject* sourceObject,
                       ParseGoal parseGoal)
  : AutoGCRooter(cx, AutoGCRooter::Tag::Parser),
    context(cx),
    alloc(alloc),
    anyChars(cx, options, thisForCtor()),
    traceListHead(nullptr),
    pc(nullptr),
    usedNames(usedNames),
    ss(nullptr),
    sourceObject(cx, sourceObject),
    keepAtoms(cx),
    foldConstants(foldConstants),
#ifdef DEBUG
    checkOptionsCalled(false),
#endif
    isUnexpectedEOF_(false),
    awaitHandling_(AwaitIsName),
    inParametersOfAsyncFunction_(false),
    parseGoal_(uint8_t(parseGoal))
{
    cx->frontendCollectionPool().addActiveCompilation();
    tempPoolMark = alloc.mark();
}

bool
ParserBase::checkOptions()
{
=======
}  // namespace detail

// Initialize |data->trailingNames| bindings, then set |data->length| to the
// count of bindings added (which must equal |count|).
//
// First, |firstBindings| are added to |data->trailingNames|.  Then any "steps"
// present are performed first to last.  Each step is 1) a pointer to a member
// of |data| to be set to the current number of bindings added, and 2) a vector
// of |BindingName|s to then copy into |data->trailingNames|.  (Thus each
// |data| member field indicates where the corresponding vector's names start.)
template <class Data, typename... Step>
static MOZ_ALWAYS_INLINE void InitializeBindingData(
    Data* data, uint32_t count, const BindingNameVector& firstBindings,
    Step&&... step) {
  MOZ_ASSERT(data->length == 0, "data shouldn't be filled yet");

  BindingName* start = data->trailingNames.start();
  BindingName* cursor = std::uninitialized_copy(firstBindings.begin(),
                                                firstBindings.end(), start);

>>>>>>> upstream-releases
#ifdef DEBUG
<<<<<<< HEAD
  checkOptionsCalled = true;
||||||| merged common ancestors
    checkOptionsCalled = true;
=======
  BindingName* end =
>>>>>>> upstream-releases
#endif
      detail::InitializeIndexedBindings(data, start, cursor,
                                        std::forward<Step>(step)...);

  MOZ_ASSERT(PointerRangeSize(start, end) == count);
  data->length = count;
}

Maybe<GlobalScope::Data*> NewGlobalScopeData(JSContext* cx,
                                             ParseContext::Scope& scope,
                                             LifoAlloc& alloc,
                                             ParseContext* pc) {
  BindingNameVector vars(cx);
  BindingNameVector lets(cx);
  BindingNameVector consts(cx);

  bool allBindingsClosedOver = pc->sc()->allBindingsClosedOver();
  for (BindingIter bi = scope.bindings(pc); bi; bi++) {
    bool closedOver = allBindingsClosedOver || bi.closedOver();

    switch (bi.kind()) {
      case BindingKind::Var: {
        bool isTopLevelFunction =
            bi.declarationKind() == DeclarationKind::BodyLevelFunction;
        BindingName binding(bi.name(), closedOver, isTopLevelFunction);
        if (!vars.append(binding)) {
          return Nothing();
        }
        break;
      }
      case BindingKind::Let: {
        BindingName binding(bi.name(), closedOver);
        if (!lets.append(binding)) {
          return Nothing();
        }
        break;
      }
      case BindingKind::Const: {
        BindingName binding(bi.name(), closedOver);
        if (!consts.append(binding)) {
          return Nothing();
        }
        break;
      }
      default:
        MOZ_CRASH("Bad global scope BindingKind");
    }
  }

<<<<<<< HEAD
  return anyChars.checkOptions();
}
||||||| merged common ancestors
    return anyChars.checkOptions();
}
=======
  GlobalScope::Data* bindings = nullptr;
  uint32_t numBindings = vars.length() + lets.length() + consts.length();
>>>>>>> upstream-releases

<<<<<<< HEAD
ParserBase::~ParserBase() {
  MOZ_ASSERT(checkOptionsCalled);
||||||| merged common ancestors
ParserBase::~ParserBase()
{
    MOZ_ASSERT(checkOptionsCalled);
=======
  if (numBindings > 0) {
    bindings = NewEmptyBindingData<GlobalScope>(cx, alloc, numBindings);
    if (!bindings) {
      return Nothing();
    }
>>>>>>> upstream-releases

<<<<<<< HEAD
  alloc.release(tempPoolMark);
||||||| merged common ancestors
    alloc.release(tempPoolMark);
=======
    // The ordering here is important. See comments in GlobalScope.
    InitializeBindingData(bindings, numBindings, vars,
                          &GlobalScope::Data::letStart, lets,
                          &GlobalScope::Data::constStart, consts);
  }
>>>>>>> upstream-releases

<<<<<<< HEAD
  /*
   * The parser can allocate enormous amounts of memory for large functions.
   * Eagerly free the memory now (which otherwise won't be freed until the
   * next GC) to avoid unnecessary OOMs.
   */
  alloc.freeAllIfHugeAndUnused();
||||||| merged common ancestors
    /*
     * The parser can allocate enormous amounts of memory for large functions.
     * Eagerly free the memory now (which otherwise won't be freed until the
     * next GC) to avoid unnecessary OOMs.
     */
    alloc.freeAllIfHugeAndUnused();
=======
  return Some(bindings);
}
>>>>>>> upstream-releases

<<<<<<< HEAD
  context->frontendCollectionPool().removeActiveCompilation();
||||||| merged common ancestors
    context->frontendCollectionPool().removeActiveCompilation();
=======
Maybe<GlobalScope::Data*> ParserBase::newGlobalScopeData(
    ParseContext::Scope& scope) {
  return NewGlobalScopeData(cx_, scope, alloc_, pc_);
>>>>>>> upstream-releases
}

<<<<<<< HEAD
template <class ParseHandler>
PerHandlerParser<ParseHandler>::PerHandlerParser(
    JSContext* cx, LifoAlloc& alloc, const ReadOnlyCompileOptions& options,
    bool foldConstants, UsedNameTracker& usedNames,
    LazyScript* lazyOuterFunction, ScriptSourceObject* sourceObject,
    ParseGoal parseGoal, void* internalSyntaxParser)
    : ParserBase(cx, alloc, options, foldConstants, usedNames, sourceObject,
                 parseGoal),
      handler(cx, alloc, lazyOuterFunction),
      internalSyntaxParser_(internalSyntaxParser) {}

template <class ParseHandler, typename Unit>
GeneralParser<ParseHandler, Unit>::GeneralParser(
    JSContext* cx, LifoAlloc& alloc, const ReadOnlyCompileOptions& options,
    const Unit* units, size_t length, bool foldConstants,
    UsedNameTracker& usedNames, SyntaxParser* syntaxParser,
    LazyScript* lazyOuterFunction, ScriptSourceObject* sourceObject,
    ParseGoal parseGoal)
    : Base(cx, alloc, options, foldConstants, usedNames, syntaxParser,
           lazyOuterFunction, sourceObject, parseGoal),
      tokenStream(cx, options, units, length) {}
||||||| merged common ancestors
template <class ParseHandler>
PerHandlerParser<ParseHandler>::PerHandlerParser(JSContext* cx, LifoAlloc& alloc,
                                                 const ReadOnlyCompileOptions& options,
                                                 bool foldConstants, UsedNameTracker& usedNames,
                                                 LazyScript* lazyOuterFunction,
                                                 ScriptSourceObject* sourceObject,
                                                 ParseGoal parseGoal, void* internalSyntaxParser)
  : ParserBase(cx, alloc, options, foldConstants, usedNames, sourceObject, parseGoal),
    handler(cx, alloc, lazyOuterFunction),
    internalSyntaxParser_(internalSyntaxParser)
{

}

template <class ParseHandler, typename Unit>
GeneralParser<ParseHandler, Unit>::GeneralParser(JSContext* cx, LifoAlloc& alloc,
                                                 const ReadOnlyCompileOptions& options,
                                                 const Unit* units, size_t length,
                                                 bool foldConstants,
                                                 UsedNameTracker& usedNames,
                                                 SyntaxParser* syntaxParser,
                                                 LazyScript* lazyOuterFunction,
                                                 ScriptSourceObject* sourceObject,
                                                 ParseGoal parseGoal)
  : Base(cx, alloc, options, foldConstants, usedNames, syntaxParser, lazyOuterFunction,
         sourceObject, parseGoal),
    tokenStream(cx, options, units, length)
{}
=======
Maybe<ModuleScope::Data*> NewModuleScopeData(JSContext* cx,
                                             ParseContext::Scope& scope,
                                             LifoAlloc& alloc,
                                             ParseContext* pc) {
  BindingNameVector imports(cx);
  BindingNameVector vars(cx);
  BindingNameVector lets(cx);
  BindingNameVector consts(cx);
>>>>>>> upstream-releases

<<<<<<< HEAD
template <typename Unit>
void Parser<SyntaxParseHandler, Unit>::setAwaitHandling(
    AwaitHandling awaitHandling) {
  this->awaitHandling_ = awaitHandling;
}
||||||| merged common ancestors
template <typename Unit>
void
Parser<SyntaxParseHandler, Unit>::setAwaitHandling(AwaitHandling awaitHandling)
{
    this->awaitHandling_ = awaitHandling;
}
=======
  bool allBindingsClosedOver = pc->sc()->allBindingsClosedOver();
  for (BindingIter bi = scope.bindings(pc); bi; bi++) {
    // Imports are indirect bindings and must not be given known slots.
    BindingName binding(bi.name(), (allBindingsClosedOver || bi.closedOver()) &&
                                       bi.kind() != BindingKind::Import);
    switch (bi.kind()) {
      case BindingKind::Import:
        if (!imports.append(binding)) {
          return Nothing();
        }
        break;
      case BindingKind::Var:
        if (!vars.append(binding)) {
          return Nothing();
        }
        break;
      case BindingKind::Let:
        if (!lets.append(binding)) {
          return Nothing();
        }
        break;
      case BindingKind::Const:
        if (!consts.append(binding)) {
          return Nothing();
        }
        break;
      default:
        MOZ_CRASH("Bad module scope BindingKind");
    }
  }
>>>>>>> upstream-releases

<<<<<<< HEAD
template <typename Unit>
void Parser<FullParseHandler, Unit>::setAwaitHandling(
    AwaitHandling awaitHandling) {
  this->awaitHandling_ = awaitHandling;
  if (SyntaxParser* syntaxParser = getSyntaxParser()) {
    syntaxParser->setAwaitHandling(awaitHandling);
  }
}
||||||| merged common ancestors
template <typename Unit>
void
Parser<FullParseHandler, Unit>::setAwaitHandling(AwaitHandling awaitHandling)
{
    this->awaitHandling_ = awaitHandling;
    if (SyntaxParser* syntaxParser = getSyntaxParser()) {
        syntaxParser->setAwaitHandling(awaitHandling);
    }
}
=======
  ModuleScope::Data* bindings = nullptr;
  uint32_t numBindings =
      imports.length() + vars.length() + lets.length() + consts.length();

  if (numBindings > 0) {
    bindings = NewEmptyBindingData<ModuleScope>(cx, alloc, numBindings);
    if (!bindings) {
      return Nothing();
    }
>>>>>>> upstream-releases

<<<<<<< HEAD
template <class ParseHandler, typename Unit>
inline void GeneralParser<ParseHandler, Unit>::setAwaitHandling(
    AwaitHandling awaitHandling) {
  asFinalParser()->setAwaitHandling(awaitHandling);
||||||| merged common ancestors
template <class ParseHandler, typename Unit>
inline void
GeneralParser<ParseHandler, Unit>::setAwaitHandling(AwaitHandling awaitHandling)
{
    asFinalParser()->setAwaitHandling(awaitHandling);
=======
    // The ordering here is important. See comments in ModuleScope.
    InitializeBindingData(bindings, numBindings, imports,
                          &ModuleScope::Data::varStart, vars,
                          &ModuleScope::Data::letStart, lets,
                          &ModuleScope::Data::constStart, consts);
  }

  return Some(bindings);
>>>>>>> upstream-releases
}

<<<<<<< HEAD
template <typename Unit>
void Parser<SyntaxParseHandler, Unit>::setInParametersOfAsyncFunction(
    bool inParameters) {
  this->inParametersOfAsyncFunction_ = inParameters;
||||||| merged common ancestors
template <typename Unit>
void
Parser<SyntaxParseHandler, Unit>::setInParametersOfAsyncFunction(bool inParameters)
{
    this->inParametersOfAsyncFunction_ = inParameters;
=======
Maybe<ModuleScope::Data*> ParserBase::newModuleScopeData(
    ParseContext::Scope& scope) {
  return NewModuleScopeData(cx_, scope, alloc_, pc_);
>>>>>>> upstream-releases
}

<<<<<<< HEAD
template <typename Unit>
void Parser<FullParseHandler, Unit>::setInParametersOfAsyncFunction(
    bool inParameters) {
  this->inParametersOfAsyncFunction_ = inParameters;
  if (SyntaxParser* syntaxParser = getSyntaxParser()) {
    syntaxParser->setInParametersOfAsyncFunction(inParameters);
  }
||||||| merged common ancestors
template <typename Unit>
void
Parser<FullParseHandler, Unit>::setInParametersOfAsyncFunction(bool inParameters)
{
    this->inParametersOfAsyncFunction_ = inParameters;
    if (SyntaxParser* syntaxParser = getSyntaxParser()) {
        syntaxParser->setInParametersOfAsyncFunction(inParameters);
    }
=======
Maybe<EvalScope::Data*> NewEvalScopeData(JSContext* cx,
                                         ParseContext::Scope& scope,
                                         LifoAlloc& alloc, ParseContext* pc) {
  BindingNameVector vars(cx);

  for (BindingIter bi = scope.bindings(pc); bi; bi++) {
    // Eval scopes only contain 'var' bindings. Make all bindings aliased
    // for now.
    MOZ_ASSERT(bi.kind() == BindingKind::Var);
    bool isTopLevelFunction =
        bi.declarationKind() == DeclarationKind::BodyLevelFunction;
    BindingName binding(bi.name(), true, isTopLevelFunction);
    if (!vars.append(binding)) {
      return Nothing();
    }
  }

  EvalScope::Data* bindings = nullptr;
  uint32_t numBindings = vars.length();

  if (numBindings > 0) {
    bindings = NewEmptyBindingData<EvalScope>(cx, alloc, numBindings);
    if (!bindings) {
      return Nothing();
    }

    InitializeBindingData(bindings, numBindings, vars);
  }

  return Some(bindings);
>>>>>>> upstream-releases
}

<<<<<<< HEAD
template <class ParseHandler, typename Unit>
inline void GeneralParser<ParseHandler, Unit>::setInParametersOfAsyncFunction(
    bool inParameters) {
  asFinalParser()->setInParametersOfAsyncFunction(inParameters);
||||||| merged common ancestors
template <class ParseHandler, typename Unit>
inline void
GeneralParser<ParseHandler, Unit>::setInParametersOfAsyncFunction(bool inParameters)
{
    asFinalParser()->setInParametersOfAsyncFunction(inParameters);
=======
Maybe<EvalScope::Data*> ParserBase::newEvalScopeData(
    ParseContext::Scope& scope) {
  return NewEvalScopeData(cx_, scope, alloc_, pc_);
>>>>>>> upstream-releases
}

<<<<<<< HEAD
template <typename BoxT, typename ArgT>
BoxT* ParserBase::newTraceListNode(ArgT* arg) {
  MOZ_ASSERT(arg);
||||||| merged common ancestors
ObjectBox*
ParserBase::newObjectBox(JSObject* obj)
{
    MOZ_ASSERT(obj);
=======
Maybe<FunctionScope::Data*> NewFunctionScopeData(
    JSContext* cx, ParseContext::Scope& scope, bool hasParameterExprs,
    IsFieldInitializer isFieldInitializer, LifoAlloc& alloc, ParseContext* pc) {
  BindingNameVector positionalFormals(cx);
  BindingNameVector formals(cx);
  BindingNameVector vars(cx);
>>>>>>> upstream-releases

<<<<<<< HEAD
  /*
   * We use JSContext.tempLifoAlloc to allocate parsed objects and place them
   * on a list in this Parser to ensure GC safety. Thus the tempLifoAlloc
   * arenas containing the entries must be alive until we are done with
   * scanning, parsing and code generation for the whole script or top-level
   * function.
   */
||||||| merged common ancestors
    /*
     * We use JSContext.tempLifoAlloc to allocate parsed objects and place them
     * on a list in this Parser to ensure GC safety. Thus the tempLifoAlloc
     * arenas containing the entries must be alive until we are done with
     * scanning, parsing and code generation for the whole script or top-level
     * function.
     */
=======
  bool allBindingsClosedOver = pc->sc()->allBindingsClosedOver();
  bool hasDuplicateParams = pc->functionBox()->hasDuplicateParameters;
>>>>>>> upstream-releases

<<<<<<< HEAD
  BoxT* box = alloc.template new_<BoxT>(arg, traceListHead);
  if (!box) {
    ReportOutOfMemory(context);
    return nullptr;
  }
||||||| merged common ancestors
    ObjectBox* objbox = alloc.template new_<ObjectBox>(obj, traceListHead);
    if (!objbox) {
        ReportOutOfMemory(context);
        return nullptr;
    }
=======
  // Positional parameter names must be added in order of appearance as they are
  // referenced using argument slots.
  for (size_t i = 0; i < pc->positionalFormalParameterNames().length(); i++) {
    JSAtom* name = pc->positionalFormalParameterNames()[i];
>>>>>>> upstream-releases

<<<<<<< HEAD
  traceListHead = box;
||||||| merged common ancestors
    traceListHead = objbox;
=======
    BindingName bindName;
    if (name) {
      DeclaredNamePtr p = scope.lookupDeclaredName(name);

      // Do not consider any positional formal parameters closed over if
      // there are parameter defaults. It is the binding in the defaults
      // scope that is closed over instead.
      bool closedOver =
          allBindingsClosedOver || (p && p->value()->closedOver());

      // If the parameter name has duplicates, only the final parameter
      // name should be on the environment, as otherwise the environment
      // object would have multiple, same-named properties.
      if (hasDuplicateParams) {
        for (size_t j = pc->positionalFormalParameterNames().length() - 1;
             j > i; j--) {
          if (pc->positionalFormalParameterNames()[j] == name) {
            closedOver = false;
            break;
          }
        }
      }
>>>>>>> upstream-releases

<<<<<<< HEAD
  return box;
}
||||||| merged common ancestors
    return objbox;
}
=======
      bindName = BindingName(name, closedOver);
    }
>>>>>>> upstream-releases

<<<<<<< HEAD
ObjectBox* ParserBase::newObjectBox(JSObject* obj) {
  return newTraceListNode<ObjectBox, JSObject>(obj);
}
||||||| merged common ancestors
template <class ParseHandler>
FunctionBox*
PerHandlerParser<ParseHandler>::newFunctionBox(CodeNodeType funNode, JSFunction* fun,
                                               uint32_t toStringStart,
                                               Directives inheritedDirectives,
                                               GeneratorKind generatorKind,
                                               FunctionAsyncKind asyncKind)
{
    MOZ_ASSERT(fun);
=======
    if (!positionalFormals.append(bindName)) {
      return Nothing();
    }
  }
>>>>>>> upstream-releases

<<<<<<< HEAD
#ifdef ENABLE_BIGINT
BigIntBox* ParserBase::newBigIntBox(BigInt* val) {
  return newTraceListNode<BigIntBox, BigInt>(val);
}
#endif
||||||| merged common ancestors
    /*
     * We use JSContext.tempLifoAlloc to allocate parsed objects and place them
     * on a list in this Parser to ensure GC safety. Thus the tempLifoAlloc
     * arenas containing the entries must be alive until we are done with
     * scanning, parsing and code generation for the whole script or top-level
     * function.
     */
    FunctionBox* funbox =
        alloc.new_<FunctionBox>(context, traceListHead, fun, toStringStart, inheritedDirectives,
                                options().extraWarningsOption, generatorKind, asyncKind);
    if (!funbox) {
        ReportOutOfMemory(context);
        return nullptr;
    }
=======
  for (BindingIter bi = scope.bindings(pc); bi; bi++) {
    BindingName binding(bi.name(), allBindingsClosedOver || bi.closedOver());
    switch (bi.kind()) {
      case BindingKind::FormalParameter:
        // Positional parameter names are already handled above.
        if (bi.declarationKind() == DeclarationKind::FormalParameter) {
          if (!formals.append(binding)) {
            return Nothing();
          }
        }
        break;
      case BindingKind::Var:
        // The only vars in the function scope when there are parameter
        // exprs, which induces a separate var environment, should be the
        // special bindings.
        MOZ_ASSERT_IF(hasParameterExprs,
                      FunctionScope::isSpecialName(cx, bi.name()));
        if (!vars.append(binding)) {
          return Nothing();
        }
        break;
      default:
        break;
    }
  }
>>>>>>> upstream-releases

<<<<<<< HEAD
template <class ParseHandler>
FunctionBox* PerHandlerParser<ParseHandler>::newFunctionBox(
    CodeNodeType funNode, JSFunction* fun, uint32_t toStringStart,
    Directives inheritedDirectives, GeneratorKind generatorKind,
    FunctionAsyncKind asyncKind) {
  MOZ_ASSERT(fun);

  /*
   * We use JSContext.tempLifoAlloc to allocate parsed objects and place them
   * on a list in this Parser to ensure GC safety. Thus the tempLifoAlloc
   * arenas containing the entries must be alive until we are done with
   * scanning, parsing and code generation for the whole script or top-level
   * function.
   */
  FunctionBox* funbox = alloc.new_<FunctionBox>(
      context, traceListHead, fun, toStringStart, inheritedDirectives,
      options().extraWarningsOption, generatorKind, asyncKind);
  if (!funbox) {
    ReportOutOfMemory(context);
    return nullptr;
  }
||||||| merged common ancestors
    traceListHead = funbox;
    if (funNode) {
        handler.setFunctionBox(funNode, funbox);
    }
=======
  FunctionScope::Data* bindings = nullptr;
  uint32_t numBindings =
      positionalFormals.length() + formals.length() + vars.length();

  if (numBindings > 0) {
    bindings = NewEmptyBindingData<FunctionScope>(cx, alloc, numBindings);
    if (!bindings) {
      return Nothing();
    }
>>>>>>> upstream-releases

<<<<<<< HEAD
  traceListHead = funbox;
  if (funNode) {
    handler.setFunctionBox(funNode, funbox);
  }
||||||| merged common ancestors
    return funbox;
}
=======
    bindings->isFieldInitializer = isFieldInitializer;
>>>>>>> upstream-releases

<<<<<<< HEAD
  return funbox;
}
||||||| merged common ancestors
ModuleSharedContext::ModuleSharedContext(JSContext* cx, ModuleObject* module,
                                         Scope* enclosingScope, ModuleBuilder& builder)
  : SharedContext(cx, Kind::Module, Directives(true), false),
    module_(cx, module),
    enclosingScope_(cx, enclosingScope),
    bindings(cx),
    builder(builder)
{
    thisBinding_ = ThisBinding::Module;
}
=======
    // The ordering here is important. See comments in FunctionScope.
    InitializeBindingData(bindings, numBindings, positionalFormals,
                          &FunctionScope::Data::nonPositionalFormalStart,
                          formals, &FunctionScope::Data::varStart, vars);
  }
>>>>>>> upstream-releases

<<<<<<< HEAD
void ParserBase::trace(JSTracer* trc) {
  TraceListNode::TraceList(trc, traceListHead);
||||||| merged common ancestors
void
ParserBase::trace(JSTracer* trc)
{
    ObjectBox::TraceList(trc, traceListHead);
=======
  return Some(bindings);
>>>>>>> upstream-releases
}

<<<<<<< HEAD
void TraceParser(JSTracer* trc, AutoGCRooter* parser) {
  static_cast<ParserBase*>(parser)->trace(trc);
||||||| merged common ancestors
void
TraceParser(JSTracer* trc, AutoGCRooter* parser)
{
    static_cast<ParserBase*>(parser)->trace(trc);
=======
Maybe<FunctionScope::Data*> ParserBase::newFunctionScopeData(
    ParseContext::Scope& scope, bool hasParameterExprs,
    IsFieldInitializer isFieldInitializer) {
  return NewFunctionScopeData(cx_, scope, hasParameterExprs, isFieldInitializer,
                              alloc_, pc_);
>>>>>>> upstream-releases
}

<<<<<<< HEAD
bool ParserBase::setSourceMapInfo() {
  // Not all clients initialize ss. Can't update info to an object that isn't
  // there.
  if (!ss) {
    return true;
  }

  if (anyChars.hasDisplayURL()) {
    if (!ss->setDisplayURL(context, anyChars.displayURL())) {
      return false;
    }
  }

  if (anyChars.hasSourceMapURL()) {
    MOZ_ASSERT(!ss->hasSourceMapURL());
    if (!ss->setSourceMapURL(context, anyChars.sourceMapURL())) {
      return false;
    }
  }

  /*
   * Source map URLs passed as a compile option (usually via a HTTP source map
   * header) override any source map urls passed as comment pragmas.
   */
  if (options().sourceMapURL()) {
    // Warn about the replacement, but use the new one.
    if (ss->hasSourceMapURL()) {
      if (!warningNoOffset(JSMSG_ALREADY_HAS_PRAGMA, ss->filename(),
                           "//# sourceMappingURL")) {
        return false;
      }
||||||| merged common ancestors
bool
ParserBase::setSourceMapInfo()
{
    // Not all clients initialize ss. Can't update info to an object that isn't there.
    if (!ss) {
        return true;
    }

    if (anyChars.hasDisplayURL()) {
        if (!ss->setDisplayURL(context, anyChars.displayURL())) {
            return false;
        }
    }

    if (anyChars.hasSourceMapURL()) {
        MOZ_ASSERT(!ss->hasSourceMapURL());
        if (!ss->setSourceMapURL(context, anyChars.sourceMapURL())) {
            return false;
        }
=======
Maybe<VarScope::Data*> NewVarScopeData(JSContext* cx,
                                       ParseContext::Scope& scope,
                                       LifoAlloc& alloc, ParseContext* pc) {
  BindingNameVector vars(cx);

  bool allBindingsClosedOver = pc->sc()->allBindingsClosedOver();

  for (BindingIter bi = scope.bindings(pc); bi; bi++) {
    if (bi.kind() == BindingKind::Var) {
      BindingName binding(bi.name(), allBindingsClosedOver || bi.closedOver());
      if (!vars.append(binding)) {
        return Nothing();
      }
>>>>>>> upstream-releases
    }
  }

<<<<<<< HEAD
    if (!ss->setSourceMapURL(context, options().sourceMapURL())) {
      return false;
||||||| merged common ancestors
    /*
     * Source map URLs passed as a compile option (usually via a HTTP source map
     * header) override any source map urls passed as comment pragmas.
     */
    if (options().sourceMapURL()) {
        // Warn about the replacement, but use the new one.
        if (ss->hasSourceMapURL()) {
            if (!warningNoOffset(JSMSG_ALREADY_HAS_PRAGMA,
                                 ss->filename(), "//# sourceMappingURL"))
            {
                return false;
            }
        }

        if (!ss->setSourceMapURL(context, options().sourceMapURL())) {
            return false;
        }
=======
  VarScope::Data* bindings = nullptr;
  uint32_t numBindings = vars.length();

  if (numBindings > 0) {
    bindings = NewEmptyBindingData<VarScope>(cx, alloc, numBindings);
    if (!bindings) {
      return Nothing();
>>>>>>> upstream-releases
    }
  }

<<<<<<< HEAD
  return true;
||||||| merged common ancestors
    return true;
=======
    InitializeBindingData(bindings, numBindings, vars);
  }

  return Some(bindings);
>>>>>>> upstream-releases
}

<<<<<<< HEAD
/*
 * Parse a top-level JS script.
 */
template <class ParseHandler, typename Unit>
typename ParseHandler::ListNodeType GeneralParser<ParseHandler, Unit>::parse() {
  MOZ_ASSERT(checkOptionsCalled);
||||||| merged common ancestors

/*
 * Parse a top-level JS script.
 */
template <class ParseHandler, typename Unit>
typename ParseHandler::ListNodeType
GeneralParser<ParseHandler, Unit>::parse()
{
    MOZ_ASSERT(checkOptionsCalled);

    Directives directives(options().strictOption);
    GlobalSharedContext globalsc(context, ScopeKind::Global,
                                 directives, options().extraWarningsOption);
    SourceParseContext globalpc(this, &globalsc, /* newDirectives = */ nullptr);
    if (!globalpc.init()) {
        return null();
    }
=======
Maybe<VarScope::Data*> ParserBase::newVarScopeData(ParseContext::Scope& scope) {
  return NewVarScopeData(cx_, scope, alloc_, pc_);
}

Maybe<LexicalScope::Data*> NewLexicalScopeData(JSContext* cx,
                                               ParseContext::Scope& scope,
                                               LifoAlloc& alloc,
                                               ParseContext* pc) {
  BindingNameVector lets(cx);
  BindingNameVector consts(cx);
>>>>>>> upstream-releases

<<<<<<< HEAD
  Directives directives(options().strictOption);
  GlobalSharedContext globalsc(context, ScopeKind::Global, directives,
                               options().extraWarningsOption);
  SourceParseContext globalpc(this, &globalsc, /* newDirectives = */ nullptr);
  if (!globalpc.init()) {
    return null();
  }
||||||| merged common ancestors
    ParseContext::VarScope varScope(this);
    if (!varScope.init(pc)) {
        return null();
    }
=======
  // Unlike other scopes with bindings which are body-level, it is unknown
  // if pc->sc()->allBindingsClosedOver() is correct at the time of
  // finishing parsing a lexical scope.
  //
  // Instead, pc->sc()->allBindingsClosedOver() is checked in
  // EmitterScope::enterLexical. Also see comment there.
  for (BindingIter bi = scope.bindings(pc); bi; bi++) {
    BindingName binding(bi.name(), bi.closedOver());
    switch (bi.kind()) {
      case BindingKind::Let:
        if (!lets.append(binding)) {
          return Nothing();
        }
        break;
      case BindingKind::Const:
        if (!consts.append(binding)) {
          return Nothing();
        }
        break;
      default:
        break;
    }
  }
>>>>>>> upstream-releases

<<<<<<< HEAD
  ParseContext::VarScope varScope(this);
  if (!varScope.init(pc)) {
    return null();
  }
||||||| merged common ancestors
    ListNodeType stmtList = statementList(YieldIsName);
    if (!stmtList) {
        return null();
    }
=======
  LexicalScope::Data* bindings = nullptr;
  uint32_t numBindings = lets.length() + consts.length();
>>>>>>> upstream-releases

<<<<<<< HEAD
  ListNodeType stmtList = statementList(YieldIsName);
  if (!stmtList) {
    return null();
  }

  TokenKind tt;
  if (!tokenStream.getToken(&tt, TokenStream::Operand)) {
    return null();
  }
  if (tt != TokenKind::Eof) {
    error(JSMSG_GARBAGE_AFTER_INPUT, "script", TokenKindToDesc(tt));
    return null();
  }
  if (foldConstants) {
    Node node = stmtList;
    if (!FoldConstants(context, &node, this)) {
      return null();
||||||| merged common ancestors
    TokenKind tt;
    if (!tokenStream.getToken(&tt, TokenStream::Operand)) {
        return null();
    }
    if (tt != TokenKind::Eof) {
        error(JSMSG_GARBAGE_AFTER_INPUT, "script", TokenKindToDesc(tt));
        return null();
    }
    if (foldConstants) {
        Node node = stmtList;
        if (!FoldConstants(context, &node, this)) {
            return null();
        }
        stmtList = handler.asList(node);
=======
  if (numBindings > 0) {
    bindings = NewEmptyBindingData<LexicalScope>(cx, alloc, numBindings);
    if (!bindings) {
      return Nothing();
>>>>>>> upstream-releases
    }
    stmtList = handler.asList(node);
  }

<<<<<<< HEAD
  return stmtList;
||||||| merged common ancestors
    return stmtList;
=======
    // The ordering here is important. See comments in LexicalScope.
    InitializeBindingData(bindings, numBindings, lets,
                          &LexicalScope::Data::constStart, consts);
  }

  return Some(bindings);
>>>>>>> upstream-releases
}

<<<<<<< HEAD
/*
 * Strict mode forbids introducing new definitions for 'eval', 'arguments',
 * 'let', 'static', 'yield', or for any strict mode reserved word.
 */
bool ParserBase::isValidStrictBinding(PropertyName* name) {
  TokenKind tt = ReservedWordTokenKind(name);
  if (tt == TokenKind::Name) {
    return name != context->names().eval && name != context->names().arguments;
  }
  return tt != TokenKind::Let && tt != TokenKind::Static &&
         tt != TokenKind::Yield && !TokenKindIsStrictReservedWord(tt);
||||||| merged common ancestors
/*
 * Strict mode forbids introducing new definitions for 'eval', 'arguments',
 * 'let', 'static', 'yield', or for any strict mode reserved word.
 */
bool
ParserBase::isValidStrictBinding(PropertyName* name)
{
    TokenKind tt = ReservedWordTokenKind(name);
    if (tt == TokenKind::Name) {
        return name != context->names().eval &&
               name != context->names().arguments;
    }
    return tt != TokenKind::Let &&
           tt != TokenKind::Static &&
           tt != TokenKind::Yield &&
           !TokenKindIsStrictReservedWord(tt);
=======
Maybe<LexicalScope::Data*> ParserBase::newLexicalScopeData(
    ParseContext::Scope& scope) {
  return NewLexicalScopeData(cx_, scope, alloc_, pc_);
>>>>>>> upstream-releases
}

<<<<<<< HEAD
/*
 * Returns true if all parameter names are valid strict mode binding names and
 * no duplicate parameter names are present.
 */
bool ParserBase::hasValidSimpleStrictParameterNames() {
  MOZ_ASSERT(pc->isFunctionBox() &&
             pc->functionBox()->hasSimpleParameterList());
||||||| merged common ancestors
/*
 * Returns true if all parameter names are valid strict mode binding names and
 * no duplicate parameter names are present.
 */
bool
ParserBase::hasValidSimpleStrictParameterNames()
{
    MOZ_ASSERT(pc->isFunctionBox() && pc->functionBox()->hasSimpleParameterList());
=======
template <>
SyntaxParseHandler::LexicalScopeNodeType
PerHandlerParser<SyntaxParseHandler>::finishLexicalScope(
    ParseContext::Scope& scope, Node body) {
  if (!propagateFreeNamesAndMarkClosedOverBindings(scope)) {
    return null();
  }
>>>>>>> upstream-releases

<<<<<<< HEAD
  if (pc->functionBox()->hasDuplicateParameters) {
    return false;
  }
||||||| merged common ancestors
    if (pc->functionBox()->hasDuplicateParameters) {
        return false;
    }
=======
  return handler_.newLexicalScope(body);
}
>>>>>>> upstream-releases

<<<<<<< HEAD
  for (auto* name : pc->positionalFormalParameterNames()) {
    MOZ_ASSERT(name);
    if (!isValidStrictBinding(name->asPropertyName())) {
      return false;
    }
  }
  return true;
||||||| merged common ancestors
    for (auto* name : pc->positionalFormalParameterNames()) {
        MOZ_ASSERT(name);
        if (!isValidStrictBinding(name->asPropertyName())) {
            return false;
        }
    }
    return true;
=======
template <>
LexicalScopeNode* PerHandlerParser<FullParseHandler>::finishLexicalScope(
    ParseContext::Scope& scope, ParseNode* body) {
  if (!propagateFreeNamesAndMarkClosedOverBindings(scope)) {
    return nullptr;
  }

  Maybe<LexicalScope::Data*> bindings = newLexicalScopeData(scope);
  if (!bindings) {
    return nullptr;
  }

  return handler_.newLexicalScope(*bindings, body);
>>>>>>> upstream-releases
}

<<<<<<< HEAD
template <class ParseHandler, typename Unit>
void GeneralParser<ParseHandler, Unit>::reportMissingClosing(
    unsigned errorNumber, unsigned noteNumber, uint32_t openedPos) {
  auto notes = MakeUnique<JSErrorNotes>();
  if (!notes) {
    ReportOutOfMemory(pc->sc()->context);
    return;
  }
||||||| merged common ancestors
template <class ParseHandler, typename Unit>
void
GeneralParser<ParseHandler, Unit>::reportMissingClosing(unsigned errorNumber, unsigned noteNumber,
                                                        uint32_t openedPos)
{
    auto notes = MakeUnique<JSErrorNotes>();
    if (!notes) {
        ReportOutOfMemory(pc->sc()->context);
        return;
    }
=======
template <typename Unit>
LexicalScopeNode* Parser<FullParseHandler, Unit>::evalBody(
    EvalSharedContext* evalsc) {
  SourceParseContext evalpc(this, evalsc, /* newDirectives = */ nullptr);
  if (!evalpc.init()) {
    return nullptr;
  }
>>>>>>> upstream-releases

<<<<<<< HEAD
  uint32_t line, column;
  anyChars.srcCoords.lineNumAndColumnIndex(openedPos, &line, &column);
||||||| merged common ancestors
    uint32_t line, column;
    anyChars.srcCoords.lineNumAndColumnIndex(openedPos, &line, &column);
=======
  ParseContext::VarScope varScope(this);
  if (!varScope.init(pc_)) {
    return nullptr;
  }
>>>>>>> upstream-releases

<<<<<<< HEAD
  const size_t MaxWidth = sizeof("4294967295");
  char columnNumber[MaxWidth];
  SprintfLiteral(columnNumber, "%" PRIu32, column);
  char lineNumber[MaxWidth];
  SprintfLiteral(lineNumber, "%" PRIu32, line);
||||||| merged common ancestors
    const size_t MaxWidth = sizeof("4294967295");
    char columnNumber[MaxWidth];
    SprintfLiteral(columnNumber, "%" PRIu32, column);
    char lineNumber[MaxWidth];
    SprintfLiteral(lineNumber, "%" PRIu32, line);
=======
  LexicalScopeNode* body;
  {
    // All evals have an implicit non-extensible lexical scope.
    ParseContext::Scope lexicalScope(this);
    if (!lexicalScope.init(pc_)) {
      return nullptr;
    }
>>>>>>> upstream-releases

<<<<<<< HEAD
  if (!notes->addNoteASCII(pc->sc()->context, getFilename(), line, column,
                           GetErrorMessage, nullptr, noteNumber, lineNumber,
                           columnNumber)) {
    return;
  }
||||||| merged common ancestors
    if (!notes->addNoteASCII(pc->sc()->context,
                             getFilename(), line, column,
                             GetErrorMessage, nullptr,
                             noteNumber, lineNumber, columnNumber))
    {
        return;
    }
=======
    ListNode* list = statementList(YieldIsName);
    if (!list) {
      return nullptr;
    }
>>>>>>> upstream-releases

<<<<<<< HEAD
  errorWithNotes(std::move(notes), errorNumber);
}
||||||| merged common ancestors
    errorWithNotes(std::move(notes), errorNumber);
}
=======
    if (!checkStatementsEOF()) {
      return nullptr;
    }
>>>>>>> upstream-releases

<<<<<<< HEAD
template <class ParseHandler, typename Unit>
void GeneralParser<ParseHandler, Unit>::reportRedeclaration(
    HandlePropertyName name, DeclarationKind prevKind, TokenPos pos,
    uint32_t prevPos) {
  UniqueChars bytes = AtomToPrintableString(context, name);
  if (!bytes) {
    return;
  }
||||||| merged common ancestors
template <class ParseHandler, typename Unit>
void
GeneralParser<ParseHandler, Unit>::reportRedeclaration(HandlePropertyName name,
                                                       DeclarationKind prevKind,
                                                       TokenPos pos, uint32_t prevPos)
{
    UniqueChars bytes = AtomToPrintableString(context, name);
    if (!bytes) {
        return;
    }
=======
    body = finishLexicalScope(lexicalScope, list);
    if (!body) {
      return nullptr;
    }
  }
>>>>>>> upstream-releases

<<<<<<< HEAD
  if (prevPos == DeclaredNameInfo::npos) {
    errorAt(pos.begin, JSMSG_REDECLARED_VAR, DeclarationKindString(prevKind),
            bytes.get());
    return;
  }
||||||| merged common ancestors
    if (prevPos == DeclaredNameInfo::npos) {
        errorAt(pos.begin, JSMSG_REDECLARED_VAR, DeclarationKindString(prevKind), bytes.get());
        return;
    }
=======
#ifdef DEBUG
  if (evalpc.superScopeNeedsHomeObject() &&
      evalsc->compilationEnclosingScope()) {
    // If superScopeNeedsHomeObject_ is set and we are an entry-point
    // ParseContext, then we must be emitting an eval script, and the
    // outer function must already be marked as needing a home object
    // since it contains an eval.
    ScopeIter si(evalsc->compilationEnclosingScope());
    for (; si; si++) {
      if (si.kind() == ScopeKind::Function) {
        JSFunction* fun = si.scope()->as<FunctionScope>().canonicalFunction();
        if (fun->isArrow()) {
          continue;
        }
        MOZ_ASSERT(fun->allowSuperProperty());
        MOZ_ASSERT(fun->nonLazyScript()->needsHomeObject());
        break;
      }
    }
    MOZ_ASSERT(!si.done(),
               "Eval must have found an enclosing function box scope that "
               "allows super.property");
  }
#endif

  if (!CheckParseTree(cx_, alloc_, body)) {
    return null();
  }
>>>>>>> upstream-releases

<<<<<<< HEAD
  auto notes = MakeUnique<JSErrorNotes>();
  if (!notes) {
    ReportOutOfMemory(pc->sc()->context);
    return;
  }
||||||| merged common ancestors
    auto notes = MakeUnique<JSErrorNotes>();
    if (!notes) {
        ReportOutOfMemory(pc->sc()->context);
        return;
    }
=======
  ParseNode* node = body;
  // Don't constant-fold inside "use asm" code, as this could create a parse
  // tree that doesn't type-check as asm.js.
  if (!pc_->useAsmOrInsideUseAsm()) {
    if (!FoldConstants(cx_, &node, &handler_)) {
      return null();
    }
  }
  body = handler_.asLexicalScope(node);
>>>>>>> upstream-releases

<<<<<<< HEAD
  uint32_t line, column;
  anyChars.srcCoords.lineNumAndColumnIndex(prevPos, &line, &column);
||||||| merged common ancestors
    uint32_t line, column;
    anyChars.srcCoords.lineNumAndColumnIndex(prevPos, &line, &column);
=======
  if (!this->setSourceMapInfo()) {
    return nullptr;
  }
>>>>>>> upstream-releases

<<<<<<< HEAD
  const size_t MaxWidth = sizeof("4294967295");
  char columnNumber[MaxWidth];
  SprintfLiteral(columnNumber, "%" PRIu32, column);
  char lineNumber[MaxWidth];
  SprintfLiteral(lineNumber, "%" PRIu32, line);
||||||| merged common ancestors
    const size_t MaxWidth = sizeof("4294967295");
    char columnNumber[MaxWidth];
    SprintfLiteral(columnNumber, "%" PRIu32, column);
    char lineNumber[MaxWidth];
    SprintfLiteral(lineNumber, "%" PRIu32, line);
=======
  // For eval scripts, since all bindings are automatically considered
  // closed over, we don't need to call propagateFreeNamesAndMarkClosed-
  // OverBindings. However, Annex B.3.3 functions still need to be marked.
  if (!varScope.propagateAndMarkAnnexBFunctionBoxes(pc_)) {
    return nullptr;
  }
>>>>>>> upstream-releases

<<<<<<< HEAD
  if (!notes->addNoteASCII(pc->sc()->context, getFilename(), line, column,
                           GetErrorMessage, nullptr, JSMSG_REDECLARED_PREV,
                           lineNumber, columnNumber)) {
    return;
  }
||||||| merged common ancestors
    if (!notes->addNoteASCII(pc->sc()->context,
                             getFilename(), line, column,
                             GetErrorMessage, nullptr,
                             JSMSG_REDECLARED_PREV,
                             lineNumber, columnNumber))
    {
        return;
    }
=======
  Maybe<EvalScope::Data*> bindings = newEvalScopeData(pc_->varScope());
  if (!bindings) {
    return nullptr;
  }
  evalsc->bindings = *bindings;
>>>>>>> upstream-releases

<<<<<<< HEAD
  errorWithNotesAt(std::move(notes), pos.begin, JSMSG_REDECLARED_VAR,
                   DeclarationKindString(prevKind), bytes.get());
||||||| merged common ancestors
    errorWithNotesAt(std::move(notes), pos.begin, JSMSG_REDECLARED_VAR,
                     DeclarationKindString(prevKind), bytes.get());
=======
  return body;
>>>>>>> upstream-releases
}

<<<<<<< HEAD
// notePositionalFormalParameter is called for both the arguments of a regular
// function definition and the arguments specified by the Function
// constructor.
//
// The 'disallowDuplicateParams' bool indicates whether the use of another
// feature (destructuring or default arguments) disables duplicate arguments.
// (ECMA-262 requires us to support duplicate parameter names, but, for newer
// features, we consider the code to have "opted in" to higher standards and
// forbid duplicates.)
template <class ParseHandler, typename Unit>
bool GeneralParser<ParseHandler, Unit>::notePositionalFormalParameter(
    CodeNodeType funNode, HandlePropertyName name, uint32_t beginPos,
    bool disallowDuplicateParams, bool* duplicatedParam) {
  if (AddDeclaredNamePtr p =
          pc->functionScope().lookupDeclaredNameForAdd(name)) {
    if (disallowDuplicateParams) {
      error(JSMSG_BAD_DUP_ARGS);
      return false;
    }
||||||| merged common ancestors
// notePositionalFormalParameter is called for both the arguments of a regular
// function definition and the arguments specified by the Function
// constructor.
//
// The 'disallowDuplicateParams' bool indicates whether the use of another
// feature (destructuring or default arguments) disables duplicate arguments.
// (ECMA-262 requires us to support duplicate parameter names, but, for newer
// features, we consider the code to have "opted in" to higher standards and
// forbid duplicates.)
template <class ParseHandler, typename Unit>
bool
GeneralParser<ParseHandler, Unit>::notePositionalFormalParameter(CodeNodeType funNode,
                                                                 HandlePropertyName name,
                                                                 uint32_t beginPos,
                                                                 bool disallowDuplicateParams,
                                                                 bool* duplicatedParam)
{
    if (AddDeclaredNamePtr p = pc->functionScope().lookupDeclaredNameForAdd(name)) {
        if (disallowDuplicateParams) {
            error(JSMSG_BAD_DUP_ARGS);
            return false;
        }

        // Strict-mode disallows duplicate args. We may not know whether we are
        // in strict mode or not (since the function body hasn't been parsed).
        // In such cases, report will queue up the potential error and return
        // 'true'.
        if (pc->sc()->needStrictChecks()) {
            UniqueChars bytes = AtomToPrintableString(context, name);
            if (!bytes) {
                return false;
            }
            if (!strictModeError(JSMSG_DUPLICATE_FORMAL, bytes.get())) {
                return false;
            }
        }

        *duplicatedParam = true;
    } else {
        DeclarationKind kind = DeclarationKind::PositionalFormalParameter;
        if (!pc->functionScope().addDeclaredName(pc, p, name, kind, beginPos)) {
            return false;
        }
    }
=======
template <typename Unit>
ListNode* Parser<FullParseHandler, Unit>::globalBody(
    GlobalSharedContext* globalsc) {
  SourceParseContext globalpc(this, globalsc, /* newDirectives = */ nullptr);
  if (!globalpc.init()) {
    return nullptr;
  }

  ParseContext::VarScope varScope(this);
  if (!varScope.init(pc_)) {
    return nullptr;
  }

  ListNode* body = statementList(YieldIsName);
  if (!body) {
    return nullptr;
  }
>>>>>>> upstream-releases

<<<<<<< HEAD
    // Strict-mode disallows duplicate args. We may not know whether we are
    // in strict mode or not (since the function body hasn't been parsed).
    // In such cases, report will queue up the potential error and return
    // 'true'.
    if (pc->sc()->needStrictChecks()) {
      UniqueChars bytes = AtomToPrintableString(context, name);
      if (!bytes) {
        return false;
      }
      if (!strictModeError(JSMSG_DUPLICATE_FORMAL, bytes.get())) {
        return false;
      }
    }
||||||| merged common ancestors
    if (!pc->positionalFormalParameterNames().append(name)) {
        ReportOutOfMemory(context);
        return false;
    }
=======
  if (!checkStatementsEOF()) {
    return nullptr;
  }
>>>>>>> upstream-releases

<<<<<<< HEAD
    *duplicatedParam = true;
  } else {
    DeclarationKind kind = DeclarationKind::PositionalFormalParameter;
    if (!pc->functionScope().addDeclaredName(pc, p, name, kind, beginPos)) {
      return false;
||||||| merged common ancestors
    NameNodeType paramNode = newName(name);
    if (!paramNode) {
        return false;
=======
  if (!CheckParseTree(cx_, alloc_, body)) {
    return null();
  }

  ParseNode* node = body;
  // Don't constant-fold inside "use asm" code, as this could create a parse
  // tree that doesn't type-check as asm.js.
  if (!pc_->useAsmOrInsideUseAsm()) {
    if (!FoldConstants(cx_, &node, &handler_)) {
      return null();
>>>>>>> upstream-releases
    }
<<<<<<< HEAD
  }
||||||| merged common ancestors
=======
  }
  body = &node->as<ListNode>();
>>>>>>> upstream-releases

<<<<<<< HEAD
  if (!pc->positionalFormalParameterNames().append(name)) {
    ReportOutOfMemory(context);
    return false;
  }
||||||| merged common ancestors
    handler.addFunctionFormalParameter(funNode, paramNode);
    return true;
}

template <class ParseHandler>
bool
PerHandlerParser<ParseHandler>::noteDestructuredPositionalFormalParameter(CodeNodeType funNode,
                                                                          Node destruct)
{
    // Append an empty name to the positional formals vector to keep track of
    // argument slots when making FunctionScope::Data.
    if (!pc->positionalFormalParameterNames().append(nullptr)) {
        ReportOutOfMemory(context);
        return false;
    }
=======
  if (!this->setSourceMapInfo()) {
    return nullptr;
  }

  // For global scripts, whether bindings are closed over or not doesn't
  // matter, so no need to call propagateFreeNamesAndMarkClosedOver-
  // Bindings. However, Annex B.3.3 functions still need to be marked.
  if (!varScope.propagateAndMarkAnnexBFunctionBoxes(pc_)) {
    return nullptr;
  }
>>>>>>> upstream-releases

<<<<<<< HEAD
  NameNodeType paramNode = newName(name);
  if (!paramNode) {
    return false;
  }
||||||| merged common ancestors
    handler.addFunctionFormalParameter(funNode, destruct);
    return true;
}
=======
  Maybe<GlobalScope::Data*> bindings = newGlobalScopeData(pc_->varScope());
  if (!bindings) {
    return nullptr;
  }
  globalsc->bindings = *bindings;
>>>>>>> upstream-releases

<<<<<<< HEAD
  handler.addFunctionFormalParameter(funNode, paramNode);
  return true;
||||||| merged common ancestors
bool
DeclarationKindIsVar(DeclarationKind kind)
{
    return kind == DeclarationKind::Var ||
           kind == DeclarationKind::BodyLevelFunction ||
           kind == DeclarationKind::VarForAnnexBLexicalFunction ||
           kind == DeclarationKind::ForOfVar;
=======
  return body;
>>>>>>> upstream-releases
}

<<<<<<< HEAD
template <class ParseHandler>
bool PerHandlerParser<ParseHandler>::noteDestructuredPositionalFormalParameter(
    CodeNodeType funNode, Node destruct) {
  // Append an empty name to the positional formals vector to keep track of
  // argument slots when making FunctionScope::Data.
  if (!pc->positionalFormalParameterNames().append(nullptr)) {
    ReportOutOfMemory(context);
    return false;
  }
||||||| merged common ancestors
Maybe<DeclarationKind>
ParseContext::isVarRedeclaredInEval(HandlePropertyName name, DeclarationKind kind)
{
    MOZ_ASSERT(DeclarationKindIsVar(kind));
    MOZ_ASSERT(sc()->isEvalContext());

    // In the case of eval, we also need to check enclosing VM scopes to see
    // if the var declaration is allowed in the context.
    //
    // This check is necessary in addition to
    // js::CheckEvalDeclarationConflicts because we only know during parsing
    // if a var is bound by for-of.
    js::Scope* enclosingScope = sc()->compilationEnclosingScope();
    js::Scope* varScope = EvalScope::nearestVarScopeForDirectEval(enclosingScope);
    MOZ_ASSERT(varScope);
    for (ScopeIter si(enclosingScope); si; si++) {
        for (js::BindingIter bi(si.scope()); bi; bi++) {
            if (bi.name() != name) {
                continue;
            }

            switch (bi.kind()) {
              case BindingKind::Let: {
                  // Annex B.3.5 allows redeclaring simple (non-destructured)
                  // catch parameters with var declarations, except when it
                  // appears in a for-of.
                  bool annexB35Allowance = si.kind() == ScopeKind::SimpleCatch &&
                                           kind != DeclarationKind::ForOfVar;
                  if (!annexB35Allowance) {
                      return Some(ScopeKindIsCatch(si.kind())
                                  ? DeclarationKind::CatchParameter
                                  : DeclarationKind::Let);
                  }
                  break;
              }

              case BindingKind::Const:
                return Some(DeclarationKind::Const);

              case BindingKind::Import:
              case BindingKind::FormalParameter:
              case BindingKind::Var:
              case BindingKind::NamedLambdaCallee:
                break;
            }
        }

        if (si.scope() == varScope) {
            break;
        }
    }
=======
template <typename Unit>
ModuleNode* Parser<FullParseHandler, Unit>::moduleBody(
    ModuleSharedContext* modulesc) {
  MOZ_ASSERT(checkOptionsCalled_);

  SourceParseContext modulepc(this, modulesc, nullptr);
  if (!modulepc.init()) {
    return null();
  }

  ParseContext::VarScope varScope(this);
  if (!varScope.init(pc_)) {
    return nullptr;
  }

  ModuleNodeType moduleNode = handler_.newModule(pos());
  if (!moduleNode) {
    return null();
  }
>>>>>>> upstream-releases

<<<<<<< HEAD
  handler.addFunctionFormalParameter(funNode, destruct);
  return true;
}
||||||| merged common ancestors
    return Nothing();
}
=======
  AutoAwaitIsKeyword<FullParseHandler, Unit> awaitIsKeyword(
      this, AwaitIsModuleKeyword);
  ListNode* stmtList = statementList(YieldIsName);
  if (!stmtList) {
    return null();
  }
>>>>>>> upstream-releases

<<<<<<< HEAD
template <class ParseHandler, typename Unit>
bool GeneralParser<ParseHandler, Unit>::
    checkLexicalDeclarationDirectlyWithinBlock(ParseContext::Statement& stmt,
                                               DeclarationKind kind,
                                               TokenPos pos) {
  MOZ_ASSERT(DeclarationKindIsLexical(kind));
||||||| merged common ancestors
Maybe<DeclarationKind>
ParseContext::isVarRedeclaredInInnermostScope(HandlePropertyName name, DeclarationKind kind)
{
    Maybe<DeclarationKind> redeclaredKind;
    uint32_t unused;
    MOZ_ALWAYS_TRUE(tryDeclareVarHelper<DryRunInnermostScopeOnly>(name, kind,
                                                                  DeclaredNameInfo::npos,
                                                                  &redeclaredKind, &unused));
    return redeclaredKind;
}
=======
  MOZ_ASSERT(stmtList->isKind(ParseNodeKind::StatementList));
  moduleNode->setBody(&stmtList->as<ListNode>());
>>>>>>> upstream-releases

<<<<<<< HEAD
  // It is an early error to declare a lexical binding not directly
  // within a block.
  if (!StatementKindIsBraced(stmt.kind()) &&
      stmt.kind() != StatementKind::ForLoopLexicalHead) {
    errorAt(pos.begin,
            stmt.kind() == StatementKind::Label
                ? JSMSG_LEXICAL_DECL_LABEL
                : JSMSG_LEXICAL_DECL_NOT_IN_BLOCK,
            DeclarationKindString(kind));
    return false;
  }
||||||| merged common ancestors
bool
ParseContext::tryDeclareVar(HandlePropertyName name, DeclarationKind kind,
                            uint32_t beginPos, Maybe<DeclarationKind>* redeclaredKind,
                            uint32_t* prevPos)
{
    return tryDeclareVarHelper<NotDryRun>(name, kind, beginPos, redeclaredKind, prevPos);
}
=======
  TokenKind tt;
  if (!tokenStream.getToken(&tt, TokenStream::SlashIsRegExp)) {
    return null();
  }
  if (tt != TokenKind::Eof) {
    error(JSMSG_GARBAGE_AFTER_INPUT, "module", TokenKindToDesc(tt));
    return null();
  }
>>>>>>> upstream-releases

<<<<<<< HEAD
  return true;
}
||||||| merged common ancestors
static bool
DeclarationKindIsParameter(DeclarationKind kind)
{
    return kind == DeclarationKind::PositionalFormalParameter ||
           kind == DeclarationKind::FormalParameter;
}
=======
  if (!modulesc->builder.buildTables()) {
    return null();
  }
>>>>>>> upstream-releases

<<<<<<< HEAD
template <class ParseHandler, typename Unit>
bool GeneralParser<ParseHandler, Unit>::noteDeclaredName(
    HandlePropertyName name, DeclarationKind kind, TokenPos pos) {
  // The asm.js validator does all its own symbol-table management so, as an
  // optimization, avoid doing any work here.
  if (pc->useAsmOrInsideUseAsm()) {
    return true;
  }

  switch (kind) {
    case DeclarationKind::Var:
    case DeclarationKind::BodyLevelFunction:
    case DeclarationKind::ForOfVar: {
      Maybe<DeclarationKind> redeclaredKind;
      uint32_t prevPos;
      if (!pc->tryDeclareVar(name, kind, pos.begin, &redeclaredKind,
                             &prevPos)) {
        return false;
      }
||||||| merged common ancestors
template <ParseContext::DryRunOption dryRunOption>
bool
ParseContext::tryDeclareVarHelper(HandlePropertyName name, DeclarationKind kind,
                                  uint32_t beginPos, Maybe<DeclarationKind>* redeclaredKind,
                                  uint32_t* prevPos)
{
    MOZ_ASSERT(DeclarationKindIsVar(kind));

    // It is an early error if a 'var' declaration appears inside a
    // scope contour that has a lexical declaration of the same name. For
    // example, the following are early errors:
    //
    //   { let x; var x; }
    //   { { var x; } let x; }
    //
    // And the following are not:
    //
    //   { var x; var x; }
    //   { { let x; } var x; }

    for (ParseContext::Scope* scope = innermostScope();
         scope != varScope().enclosing();
         scope = scope->enclosing())
    {
        if (AddDeclaredNamePtr p = scope->lookupDeclaredNameForAdd(name)) {
            DeclarationKind declaredKind = p->value()->kind();
            if (DeclarationKindIsVar(declaredKind)) {
                // Any vars that are redeclared as body-level functions must
                // be recorded as body-level functions.
                //
                // In the case of global and eval scripts, GlobalDeclaration-
                // Instantiation [1] and EvalDeclarationInstantiation [2]
                // check for the declarability of global var and function
                // bindings via CanDeclareVar [3] and CanDeclareGlobal-
                // Function [4]. CanDeclareGlobalFunction is strictly more
                // restrictive than CanDeclareGlobalVar, so record the more
                // restrictive kind. These semantics are implemented in
                // CheckCanDeclareGlobalBinding.
                //
                // For a var previously declared as ForOfVar, this previous
                // DeclarationKind is used only to check for if the
                // 'arguments' binding should be declared. Since body-level
                // functions shadow 'arguments' [5], it is correct to alter
                // the kind to BodyLevelFunction. See
                // declareFunctionArgumentsObject.
                //
                // VarForAnnexBLexicalFunction declarations are declared when
                // the var scope exits. It is not possible for a var to be
                // previously declared as VarForAnnexBLexicalFunction and
                // checked for redeclaration.
                //
                // [1] ES 15.1.11
                // [2] ES 18.2.1.3
                // [3] ES 8.1.1.4.15
                // [4] ES 8.1.1.4.16
                // [5] ES 9.2.12
                if (dryRunOption == NotDryRun && kind == DeclarationKind::BodyLevelFunction) {
                    MOZ_ASSERT(declaredKind != DeclarationKind::VarForAnnexBLexicalFunction);
                    p->value()->alterKind(kind);
                }
            } else if (!DeclarationKindIsParameter(declaredKind)) {
                // Annex B.3.5 allows redeclaring simple (non-destructured)
                // catch parameters with var declarations, except when it
                // appears in a for-of.
                bool annexB35Allowance = declaredKind == DeclarationKind::SimpleCatchParameter &&
                                         kind != DeclarationKind::ForOfVar;

                // Annex B.3.3 allows redeclaring functions in the same block.
                bool annexB33Allowance = declaredKind == DeclarationKind::SloppyLexicalFunction &&
                                         kind == DeclarationKind::VarForAnnexBLexicalFunction &&
                                         scope == innermostScope();

                if (!annexB35Allowance && !annexB33Allowance) {
                    *redeclaredKind = Some(declaredKind);
                    *prevPos = p->value()->pos();
                    return true;
                }
            } else if (kind == DeclarationKind::VarForAnnexBLexicalFunction) {
                MOZ_ASSERT(DeclarationKindIsParameter(declaredKind));

                // Annex B.3.3.1 disallows redeclaring parameter names.
                // We don't need to set *prevPos here since this case is not
                // an error.
                *redeclaredKind = Some(declaredKind);
                return true;
            }
        } else if (dryRunOption == NotDryRun) {
            if (!scope->addDeclaredName(this, p, name, kind, beginPos)) {
                return false;
            }
        }

        // DryRunOption is used for propagating Annex B functions: we don't
        // want to declare the synthesized Annex B vars until we exit the var
        // scope and know that no early errors would have occurred. In order
        // to avoid quadratic search, we only check for var redeclarations in
        // the innermost scope when doing a dry run.
        if (dryRunOption == DryRunInnermostScopeOnly) {
            break;
        }
    }

    if (!sc()->strict() && sc()->isEvalContext() &&
        (dryRunOption == NotDryRun || innermostScope() == &varScope()))
    {
        *redeclaredKind = isVarRedeclaredInEval(name, kind);
        // We don't have position information at runtime.
        *prevPos = DeclaredNameInfo::npos;
    }

    return true;
}
=======
  // Check exported local bindings exist and mark them as closed over.
  for (auto entry : modulesc->builder.localExportEntries()) {
    JSAtom* name = entry->localName();
    MOZ_ASSERT(name);

    DeclaredNamePtr p = modulepc.varScope().lookupDeclaredName(name);
    if (!p) {
      UniqueChars str = AtomToPrintableString(cx_, name);
      if (!str) {
        return null();
      }

      errorNoOffset(JSMSG_MISSING_EXPORT, str.get());
      return null();
    }

    p->value()->setClosedOver();
  }

  if (!CheckParseTree(cx_, alloc_, stmtList)) {
    return null();
  }
>>>>>>> upstream-releases

<<<<<<< HEAD
      if (redeclaredKind) {
        reportRedeclaration(name, *redeclaredKind, pos, prevPos);
        return false;
      }

      break;
||||||| merged common ancestors
bool
ParseContext::annexBAppliesToLexicalFunctionInInnermostScope(FunctionBox* funbox)
{
    MOZ_ASSERT(!sc()->strict());

    RootedPropertyName name(sc()->context, funbox->function()->explicitName()->asPropertyName());
    Maybe<DeclarationKind> redeclaredKind =
        isVarRedeclaredInInnermostScope(name, DeclarationKind::VarForAnnexBLexicalFunction);

    if (!redeclaredKind && isFunctionBox()) {
        Scope& funScope = functionScope();
        if (&funScope != &varScope()) {
            // Annex B.3.3.1 disallows redeclaring parameter names. In the
            // presence of parameter expressions, parameter names are on the
            // function scope, which encloses the var scope. This means the
            // isVarRedeclaredInInnermostScope call above would not catch this
            // case, so test it manually.
            if (AddDeclaredNamePtr p = funScope.lookupDeclaredNameForAdd(name)) {
                DeclarationKind declaredKind = p->value()->kind();
                if (DeclarationKindIsParameter(declaredKind)) {
                    redeclaredKind = Some(declaredKind);
                } else {
                    MOZ_ASSERT(FunctionScope::isSpecialName(sc()->context, name));
                }
            }
        }
=======
  ParseNode* node = stmtList;
  // Don't constant-fold inside "use asm" code, as this could create a parse
  // tree that doesn't type-check as asm.js.
  if (!pc_->useAsmOrInsideUseAsm()) {
    if (!FoldConstants(cx_, &node, &handler_)) {
      return null();
>>>>>>> upstream-releases
    }
  }
  stmtList = &node->as<ListNode>();

<<<<<<< HEAD
    case DeclarationKind::ModuleBodyLevelFunction: {
      MOZ_ASSERT(pc->atModuleLevel());

      AddDeclaredNamePtr p = pc->varScope().lookupDeclaredNameForAdd(name);
      if (p) {
        reportRedeclaration(name, p->value()->kind(), pos, p->value()->pos());
        return false;
      }

      if (!pc->varScope().addDeclaredName(pc, p, name, kind, pos.begin)) {
        return false;
      }

      // Body-level functions in modules are always closed over.
      pc->varScope().lookupDeclaredName(name)->value()->setClosedOver();

      break;
    }

    case DeclarationKind::FormalParameter: {
      // It is an early error if any non-positional formal parameter name
      // (e.g., destructuring formal parameter) is duplicated.

      AddDeclaredNamePtr p = pc->functionScope().lookupDeclaredNameForAdd(name);
      if (p) {
        error(JSMSG_BAD_DUP_ARGS);
        return false;
      }

      if (!pc->functionScope().addDeclaredName(pc, p, name, kind, pos.begin)) {
        return false;
      }

      break;
    }

    case DeclarationKind::LexicalFunction: {
      ParseContext::Scope* scope = pc->innermostScope();
      AddDeclaredNamePtr p = scope->lookupDeclaredNameForAdd(name);
      if (p) {
        reportRedeclaration(name, p->value()->kind(), pos, p->value()->pos());
        return false;
      }

      if (!scope->addDeclaredName(pc, p, name, kind, pos.begin)) {
        return false;
      }

      break;
    }

    case DeclarationKind::SloppyLexicalFunction: {
      // Functions in block have complex allowances in sloppy mode for being
      // labelled that other lexical declarations do not have. Those checks
      // are more complex than calling checkLexicalDeclarationDirectlyWithin-
      // Block and are done in checkFunctionDefinition.

      ParseContext::Scope* scope = pc->innermostScope();
      if (AddDeclaredNamePtr p = scope->lookupDeclaredNameForAdd(name)) {
        // It is usually an early error if there is another declaration
        // with the same name in the same scope.
        //
        // Sloppy lexical functions may redeclare other sloppy lexical
        // functions for web compatibility reasons.
        if (p->value()->kind() != DeclarationKind::SloppyLexicalFunction) {
          reportRedeclaration(name, p->value()->kind(), pos, p->value()->pos());
          return false;
        }
      } else {
        if (!scope->addDeclaredName(pc, p, name, kind, pos.begin)) {
          return false;
        }
      }

      break;
    }

    case DeclarationKind::Let:
    case DeclarationKind::Const:
    case DeclarationKind::Class:
      // The BoundNames of LexicalDeclaration and ForDeclaration must not
      // contain 'let'. (CatchParameter is the only lexical binding form
      // without this restriction.)
      if (name == context->names().let) {
        errorAt(pos.begin, JSMSG_LEXICAL_DECL_DEFINES_LET);
        return false;
      }

      MOZ_FALLTHROUGH;

    case DeclarationKind::Import:
      // Module code is always strict, so 'let' is always a keyword and never a
      // name.
      MOZ_ASSERT(name != context->names().let);
      MOZ_FALLTHROUGH;

    case DeclarationKind::SimpleCatchParameter:
    case DeclarationKind::CatchParameter: {
      if (ParseContext::Statement* stmt = pc->innermostStatement()) {
        if (!checkLexicalDeclarationDirectlyWithinBlock(*stmt, kind, pos)) {
          return false;
        }
      }

      ParseContext::Scope* scope = pc->innermostScope();

      // For body-level lexically declared names in a function, it is an
      // early error if there is a formal parameter of the same name. This
      // needs a special check if there is an extra var scope due to
      // parameter expressions.
      if (pc->isFunctionExtraBodyVarScopeInnermost()) {
        DeclaredNamePtr p = pc->functionScope().lookupDeclaredName(name);
        if (p && DeclarationKindIsParameter(p->value()->kind())) {
          reportRedeclaration(name, p->value()->kind(), pos, p->value()->pos());
          return false;
        }
      }

      // It is an early error if there is another declaration with the same
      // name in the same scope.
      AddDeclaredNamePtr p = scope->lookupDeclaredNameForAdd(name);
      if (p) {
        reportRedeclaration(name, p->value()->kind(), pos, p->value()->pos());
        return false;
      }

      if (!scope->addDeclaredName(pc, p, name, kind, pos.begin)) {
        return false;
      }

      break;
    }

    case DeclarationKind::CoverArrowParameter:
      // CoverArrowParameter is only used as a placeholder declaration kind.
      break;

    case DeclarationKind::PositionalFormalParameter:
      MOZ_CRASH(
          "Positional formal parameter names should use "
          "notePositionalFormalParameter");
      break;

    case DeclarationKind::VarForAnnexBLexicalFunction:
      MOZ_CRASH(
          "Synthesized Annex B vars should go through "
          "tryDeclareVarForAnnexBLexicalFunction");
      break;
  }

  return true;
}

bool ParserBase::noteUsedNameInternal(HandlePropertyName name) {
  // The asm.js validator does all its own symbol-table management so, as an
  // optimization, avoid doing any work here.
  if (pc->useAsmOrInsideUseAsm()) {
    return true;
  }

  // Global bindings are properties and not actual bindings; we don't need
  // to know if they are closed over. So no need to track used name at the
  // global scope. It is not incorrect to track them, this is an
  // optimization.
  ParseContext::Scope* scope = pc->innermostScope();
  if (pc->sc()->isGlobalContext() && scope == &pc->varScope()) {
    return true;
  }

  return usedNames.noteUse(context, name, pc->scriptId(), scope->id());
}

bool ParserBase::hasUsedName(HandlePropertyName name) {
  if (UsedNamePtr p = usedNames.lookup(name)) {
    return p->value().isUsedInScript(pc->scriptId());
  }
  return false;
}

template <class ParseHandler>
bool PerHandlerParser<ParseHandler>::
    propagateFreeNamesAndMarkClosedOverBindings(ParseContext::Scope& scope) {
  // Now that we have all the declared names in the scope, check which
  // functions should exhibit Annex B semantics.
  if (!scope.propagateAndMarkAnnexBFunctionBoxes(pc)) {
    return false;
  }

  if (handler.canSkipLazyClosedOverBindings()) {
    // Scopes are nullptr-delimited in the LazyScript closed over bindings
    // array.
    while (JSAtom* name = handler.nextLazyClosedOverBinding()) {
      scope.lookupDeclaredName(name)->value()->setClosedOver();
    }
    return true;
  }

  bool isSyntaxParser =
      mozilla::IsSame<ParseHandler, SyntaxParseHandler>::value;
  uint32_t scriptId = pc->scriptId();
  uint32_t scopeId = scope.id();
  for (BindingIter bi = scope.bindings(pc); bi; bi++) {
    if (UsedNamePtr p = usedNames.lookup(bi.name())) {
      bool closedOver;
      p->value().noteBoundInScope(scriptId, scopeId, &closedOver);
      if (closedOver) {
        bi.setClosedOver();

        if (isSyntaxParser &&
            !pc->closedOverBindingsForLazy().append(bi.name())) {
          ReportOutOfMemory(context);
          return false;
        }
      }
    }
  }

  // Append a nullptr to denote end-of-scope.
  if (isSyntaxParser && !pc->closedOverBindingsForLazy().append(nullptr)) {
    ReportOutOfMemory(context);
    return false;
  }

  return true;
}

template <typename Unit>
bool Parser<FullParseHandler, Unit>::checkStatementsEOF() {
  // This is designed to be paired with parsing a statement list at the top
  // level.
  //
  // The statementList() call breaks on TokenKind::RightCurly, so make sure
  // we've reached EOF here.
  TokenKind tt;
  if (!tokenStream.peekToken(&tt, TokenStream::Operand)) {
    return false;
  }
  if (tt != TokenKind::Eof) {
    error(JSMSG_UNEXPECTED_TOKEN, "expression", TokenKindToDesc(tt));
    return false;
  }
  return true;
}

template <typename Scope>
typename Scope::Data* NewEmptyBindingData(JSContext* cx, LifoAlloc& alloc,
                                          uint32_t numBindings) {
  using Data = typename Scope::Data;
  size_t allocSize = SizeOfData<typename Scope::Data>(numBindings);
  auto* bindings = alloc.allocInSize<Data>(allocSize, numBindings);
  if (!bindings) {
    ReportOutOfMemory(cx);
  }
  return bindings;
}

namespace detail {

template <class Data>
static MOZ_ALWAYS_INLINE BindingName* InitializeIndexedBindings(
    Data* data, BindingName* start, BindingName* cursor) {
  return cursor;
}

template <class Data, typename UnsignedInteger, typename... Step>
static MOZ_ALWAYS_INLINE BindingName* InitializeIndexedBindings(
    Data* data, BindingName* start, BindingName* cursor,
    UnsignedInteger Data::*field, const BindingNameVector& bindings,
    Step&&... step) {
  data->*field = AssertedCast<UnsignedInteger>(PointerRangeSize(start, cursor));

  BindingName* newCursor =
      std::uninitialized_copy(bindings.begin(), bindings.end(), cursor);

  return InitializeIndexedBindings(data, start, newCursor,
                                   std::forward<Step>(step)...);
}

}  // namespace detail

// Initialize |data->trailingNames| bindings, then set |data->length| to the
// count of bindings added (which must equal |count|).
//
// First, |firstBindings| are added to |data->trailingNames|.  Then any "steps"
// present are performed first to last.  Each step is 1) a pointer to a member
// of |data| to be set to the current number of bindings added, and 2) a vector
// of |BindingName|s to then copy into |data->trailingNames|.  (Thus each
// |data| member field indicates where the corresponding vector's names start.)
template <class Data, typename... Step>
static MOZ_ALWAYS_INLINE void InitializeBindingData(
    Data* data, uint32_t count, const BindingNameVector& firstBindings,
    Step&&... step) {
  MOZ_ASSERT(data->length == 0, "data shouldn't be filled yet");

  BindingName* start = data->trailingNames.start();
  BindingName* cursor = std::uninitialized_copy(firstBindings.begin(),
                                                firstBindings.end(), start);

#ifdef DEBUG
  BindingName* end =
#endif
      detail::InitializeIndexedBindings(data, start, cursor,
                                        std::forward<Step>(step)...);

  MOZ_ASSERT(PointerRangeSize(start, end) == count);
  data->length = count;
}

Maybe<GlobalScope::Data*> NewGlobalScopeData(JSContext* context,
                                             ParseContext::Scope& scope,
                                             LifoAlloc& alloc,
                                             ParseContext* pc) {
  BindingNameVector vars(context);
  BindingNameVector lets(context);
  BindingNameVector consts(context);

  bool allBindingsClosedOver = pc->sc()->allBindingsClosedOver();
  for (BindingIter bi = scope.bindings(pc); bi; bi++) {
    bool closedOver = allBindingsClosedOver || bi.closedOver();

    switch (bi.kind()) {
      case BindingKind::Var: {
        bool isTopLevelFunction =
            bi.declarationKind() == DeclarationKind::BodyLevelFunction;
        BindingName binding(bi.name(), closedOver, isTopLevelFunction);
        if (!vars.append(binding)) {
          return Nothing();
        }
        break;
      }
      case BindingKind::Let: {
        BindingName binding(bi.name(), closedOver);
        if (!lets.append(binding)) {
          return Nothing();
        }
        break;
      }
      case BindingKind::Const: {
        BindingName binding(bi.name(), closedOver);
        if (!consts.append(binding)) {
          return Nothing();
        }
        break;
      }
      default:
        MOZ_CRASH("Bad global scope BindingKind");
    }
  }

  GlobalScope::Data* bindings = nullptr;
  uint32_t numBindings = vars.length() + lets.length() + consts.length();

  if (numBindings > 0) {
    bindings = NewEmptyBindingData<GlobalScope>(context, alloc, numBindings);
    if (!bindings) {
      return Nothing();
    }

    // The ordering here is important. See comments in GlobalScope.
    InitializeBindingData(bindings, numBindings, vars,
                          &GlobalScope::Data::letStart, lets,
                          &GlobalScope::Data::constStart, consts);
  }

  return Some(bindings);
}

Maybe<GlobalScope::Data*> ParserBase::newGlobalScopeData(
    ParseContext::Scope& scope) {
  return NewGlobalScopeData(context, scope, alloc, pc);
}

Maybe<ModuleScope::Data*> NewModuleScopeData(JSContext* context,
                                             ParseContext::Scope& scope,
                                             LifoAlloc& alloc,
                                             ParseContext* pc) {
  BindingNameVector imports(context);
  BindingNameVector vars(context);
  BindingNameVector lets(context);
  BindingNameVector consts(context);

  bool allBindingsClosedOver = pc->sc()->allBindingsClosedOver();
  for (BindingIter bi = scope.bindings(pc); bi; bi++) {
    // Imports are indirect bindings and must not be given known slots.
    BindingName binding(bi.name(), (allBindingsClosedOver || bi.closedOver()) &&
                                       bi.kind() != BindingKind::Import);
    switch (bi.kind()) {
      case BindingKind::Import:
        if (!imports.append(binding)) {
          return Nothing();
        }
        break;
      case BindingKind::Var:
        if (!vars.append(binding)) {
          return Nothing();
        }
        break;
      case BindingKind::Let:
        if (!lets.append(binding)) {
          return Nothing();
        }
        break;
      case BindingKind::Const:
        if (!consts.append(binding)) {
          return Nothing();
        }
        break;
      default:
        MOZ_CRASH("Bad module scope BindingKind");
    }
  }

  ModuleScope::Data* bindings = nullptr;
  uint32_t numBindings =
      imports.length() + vars.length() + lets.length() + consts.length();

  if (numBindings > 0) {
    bindings = NewEmptyBindingData<ModuleScope>(context, alloc, numBindings);
    if (!bindings) {
      return Nothing();
    }

    // The ordering here is important. See comments in ModuleScope.
    InitializeBindingData(bindings, numBindings, imports,
                          &ModuleScope::Data::varStart, vars,
                          &ModuleScope::Data::letStart, lets,
                          &ModuleScope::Data::constStart, consts);
  }

  return Some(bindings);
}

Maybe<ModuleScope::Data*> ParserBase::newModuleScopeData(
    ParseContext::Scope& scope) {
  return NewModuleScopeData(context, scope, alloc, pc);
}

Maybe<EvalScope::Data*> NewEvalScopeData(JSContext* context,
                                         ParseContext::Scope& scope,
                                         LifoAlloc& alloc, ParseContext* pc) {
  BindingNameVector vars(context);

  for (BindingIter bi = scope.bindings(pc); bi; bi++) {
    // Eval scopes only contain 'var' bindings. Make all bindings aliased
    // for now.
    MOZ_ASSERT(bi.kind() == BindingKind::Var);
    bool isTopLevelFunction =
        bi.declarationKind() == DeclarationKind::BodyLevelFunction;
    BindingName binding(bi.name(), true, isTopLevelFunction);
    if (!vars.append(binding)) {
      return Nothing();
    }
  }

  EvalScope::Data* bindings = nullptr;
  uint32_t numBindings = vars.length();

  if (numBindings > 0) {
    bindings = NewEmptyBindingData<EvalScope>(context, alloc, numBindings);
    if (!bindings) {
      return Nothing();
    }

    InitializeBindingData(bindings, numBindings, vars);
  }

  return Some(bindings);
}

Maybe<EvalScope::Data*> ParserBase::newEvalScopeData(
    ParseContext::Scope& scope) {
  return NewEvalScopeData(context, scope, alloc, pc);
}

Maybe<FunctionScope::Data*> NewFunctionScopeData(JSContext* context,
                                                 ParseContext::Scope& scope,
                                                 bool hasParameterExprs,
                                                 LifoAlloc& alloc,
                                                 ParseContext* pc) {
  BindingNameVector positionalFormals(context);
  BindingNameVector formals(context);
  BindingNameVector vars(context);

  bool allBindingsClosedOver = pc->sc()->allBindingsClosedOver();
  bool hasDuplicateParams = pc->functionBox()->hasDuplicateParameters;

  // Positional parameter names must be added in order of appearance as they are
  // referenced using argument slots.
  for (size_t i = 0; i < pc->positionalFormalParameterNames().length(); i++) {
    JSAtom* name = pc->positionalFormalParameterNames()[i];

    BindingName bindName;
    if (name) {
      DeclaredNamePtr p = scope.lookupDeclaredName(name);

      // Do not consider any positional formal parameters closed over if
      // there are parameter defaults. It is the binding in the defaults
      // scope that is closed over instead.
      bool closedOver =
          allBindingsClosedOver || (p && p->value()->closedOver());

      // If the parameter name has duplicates, only the final parameter
      // name should be on the environment, as otherwise the environment
      // object would have multiple, same-named properties.
      if (hasDuplicateParams) {
        for (size_t j = pc->positionalFormalParameterNames().length() - 1;
             j > i; j--) {
          if (pc->positionalFormalParameterNames()[j] == name) {
            closedOver = false;
            break;
          }
        }
      }

      bindName = BindingName(name, closedOver);
    }

    if (!positionalFormals.append(bindName)) {
      return Nothing();
    }
  }

  for (BindingIter bi = scope.bindings(pc); bi; bi++) {
    BindingName binding(bi.name(), allBindingsClosedOver || bi.closedOver());
    switch (bi.kind()) {
      case BindingKind::FormalParameter:
        // Positional parameter names are already handled above.
        if (bi.declarationKind() == DeclarationKind::FormalParameter) {
          if (!formals.append(binding)) {
            return Nothing();
          }
        }
        break;
      case BindingKind::Var:
        // The only vars in the function scope when there are parameter
        // exprs, which induces a separate var environment, should be the
        // special bindings.
        MOZ_ASSERT_IF(hasParameterExprs,
                      FunctionScope::isSpecialName(context, bi.name()));
        if (!vars.append(binding)) {
          return Nothing();
        }
        break;
      default:
        break;
    }
  }

  FunctionScope::Data* bindings = nullptr;
  uint32_t numBindings =
      positionalFormals.length() + formals.length() + vars.length();

  if (numBindings > 0) {
    bindings = NewEmptyBindingData<FunctionScope>(context, alloc, numBindings);
    if (!bindings) {
      return Nothing();
    }

    // The ordering here is important. See comments in FunctionScope.
    InitializeBindingData(bindings, numBindings, positionalFormals,
                          &FunctionScope::Data::nonPositionalFormalStart,
                          formals, &FunctionScope::Data::varStart, vars);
  }

  return Some(bindings);
}

Maybe<FunctionScope::Data*> ParserBase::newFunctionScopeData(
    ParseContext::Scope& scope, bool hasParameterExprs) {
  return NewFunctionScopeData(context, scope, hasParameterExprs, alloc, pc);
}

Maybe<VarScope::Data*> NewVarScopeData(JSContext* context,
                                       ParseContext::Scope& scope,
                                       LifoAlloc& alloc, ParseContext* pc) {
  BindingNameVector vars(context);

  bool allBindingsClosedOver = pc->sc()->allBindingsClosedOver();

  for (BindingIter bi = scope.bindings(pc); bi; bi++) {
    if (bi.kind() == BindingKind::Var) {
      BindingName binding(bi.name(), allBindingsClosedOver || bi.closedOver());
      if (!vars.append(binding)) {
        return Nothing();
      }
    }
  }

  VarScope::Data* bindings = nullptr;
  uint32_t numBindings = vars.length();

  if (numBindings > 0) {
    bindings = NewEmptyBindingData<VarScope>(context, alloc, numBindings);
    if (!bindings) {
      return Nothing();
    }

    InitializeBindingData(bindings, numBindings, vars);
  }

  return Some(bindings);
}

Maybe<VarScope::Data*> ParserBase::newVarScopeData(ParseContext::Scope& scope) {
  return NewVarScopeData(context, scope, alloc, pc);
}

Maybe<LexicalScope::Data*> NewLexicalScopeData(JSContext* context,
                                               ParseContext::Scope& scope,
                                               LifoAlloc& alloc,
                                               ParseContext* pc) {
  BindingNameVector lets(context);
  BindingNameVector consts(context);

  // Unlike other scopes with bindings which are body-level, it is unknown
  // if pc->sc()->allBindingsClosedOver() is correct at the time of
  // finishing parsing a lexical scope.
  //
  // Instead, pc->sc()->allBindingsClosedOver() is checked in
  // EmitterScope::enterLexical. Also see comment there.
  for (BindingIter bi = scope.bindings(pc); bi; bi++) {
    BindingName binding(bi.name(), bi.closedOver());
    switch (bi.kind()) {
      case BindingKind::Let:
        if (!lets.append(binding)) {
          return Nothing();
        }
        break;
      case BindingKind::Const:
        if (!consts.append(binding)) {
          return Nothing();
        }
        break;
      default:
        break;
    }
  }

  LexicalScope::Data* bindings = nullptr;
  uint32_t numBindings = lets.length() + consts.length();

  if (numBindings > 0) {
    bindings = NewEmptyBindingData<LexicalScope>(context, alloc, numBindings);
    if (!bindings) {
      return Nothing();
    }

    // The ordering here is important. See comments in LexicalScope.
    InitializeBindingData(bindings, numBindings, lets,
                          &LexicalScope::Data::constStart, consts);
  }

  return Some(bindings);
}

Maybe<LexicalScope::Data*> ParserBase::newLexicalScopeData(
    ParseContext::Scope& scope) {
  return NewLexicalScopeData(context, scope, alloc, pc);
}

template <>
SyntaxParseHandler::LexicalScopeNodeType
PerHandlerParser<SyntaxParseHandler>::finishLexicalScope(
    ParseContext::Scope& scope, Node body) {
  if (!propagateFreeNamesAndMarkClosedOverBindings(scope)) {
    return null();
  }

  return handler.newLexicalScope(body);
}

template <>
LexicalScopeNode* PerHandlerParser<FullParseHandler>::finishLexicalScope(
    ParseContext::Scope& scope, ParseNode* body) {
  if (!propagateFreeNamesAndMarkClosedOverBindings(scope)) {
    return nullptr;
  }

  Maybe<LexicalScope::Data*> bindings = newLexicalScopeData(scope);
  if (!bindings) {
    return nullptr;
  }

  return handler.newLexicalScope(*bindings, body);
}

template <typename Unit>
LexicalScopeNode* Parser<FullParseHandler, Unit>::evalBody(
    EvalSharedContext* evalsc) {
  SourceParseContext evalpc(this, evalsc, /* newDirectives = */ nullptr);
  if (!evalpc.init()) {
    return nullptr;
  }

  ParseContext::VarScope varScope(this);
  if (!varScope.init(pc)) {
    return nullptr;
  }

  LexicalScopeNode* body;
  {
    // All evals have an implicit non-extensible lexical scope.
    ParseContext::Scope lexicalScope(this);
    if (!lexicalScope.init(pc)) {
      return nullptr;
    }

    ParseNode* list = statementList(YieldIsName);
    if (!list) {
      return nullptr;
    }

    if (!checkStatementsEOF()) {
      return nullptr;
    }

    body = finishLexicalScope(lexicalScope, list);
    if (!body) {
      return nullptr;
    }
  }

#ifdef DEBUG
  if (evalpc.superScopeNeedsHomeObject() &&
      evalsc->compilationEnclosingScope()) {
    // If superScopeNeedsHomeObject_ is set and we are an entry-point
    // ParseContext, then we must be emitting an eval script, and the
    // outer function must already be marked as needing a home object
    // since it contains an eval.
    ScopeIter si(evalsc->compilationEnclosingScope());
    for (; si; si++) {
      if (si.kind() == ScopeKind::Function) {
        JSFunction* fun = si.scope()->as<FunctionScope>().canonicalFunction();
        if (fun->isArrow()) {
          continue;
        }
        MOZ_ASSERT(fun->allowSuperProperty());
        MOZ_ASSERT(fun->nonLazyScript()->needsHomeObject());
        break;
      }
    }
    MOZ_ASSERT(!si.done(),
               "Eval must have found an enclosing function box scope that "
               "allows super.property");
  }
#endif

  ParseNode* node = body;
  if (!FoldConstants(context, &node, this)) {
    return nullptr;
  }
  body = handler.asLexicalScope(node);

  if (!this->setSourceMapInfo()) {
    return nullptr;
  }

  // For eval scripts, since all bindings are automatically considered
  // closed over, we don't need to call propagateFreeNamesAndMarkClosed-
  // OverBindings. However, Annex B.3.3 functions still need to be marked.
  if (!varScope.propagateAndMarkAnnexBFunctionBoxes(pc)) {
    return nullptr;
  }

  Maybe<EvalScope::Data*> bindings = newEvalScopeData(pc->varScope());
  if (!bindings) {
    return nullptr;
  }
  evalsc->bindings = *bindings;

  return body;
}

template <typename Unit>
ListNode* Parser<FullParseHandler, Unit>::globalBody(
    GlobalSharedContext* globalsc) {
  SourceParseContext globalpc(this, globalsc, /* newDirectives = */ nullptr);
  if (!globalpc.init()) {
    return nullptr;
  }

  ParseContext::VarScope varScope(this);
  if (!varScope.init(pc)) {
    return nullptr;
  }

  ListNode* body = statementList(YieldIsName);
  if (!body) {
    return nullptr;
  }

  if (!checkStatementsEOF()) {
    return nullptr;
  }

  ParseNode* node = body;
  if (!FoldConstants(context, &node, this)) {
    return null();
  }
  body = &node->as<ListNode>();

  if (!this->setSourceMapInfo()) {
    return nullptr;
  }

  // For global scripts, whether bindings are closed over or not doesn't
  // matter, so no need to call propagateFreeNamesAndMarkClosedOver-
  // Bindings. However, Annex B.3.3 functions still need to be marked.
  if (!varScope.propagateAndMarkAnnexBFunctionBoxes(pc)) {
    return nullptr;
  }

  Maybe<GlobalScope::Data*> bindings = newGlobalScopeData(pc->varScope());
  if (!bindings) {
    return nullptr;
  }
  globalsc->bindings = *bindings;

  return body;
}

template <typename Unit>
CodeNode* Parser<FullParseHandler, Unit>::moduleBody(
    ModuleSharedContext* modulesc) {
  MOZ_ASSERT(checkOptionsCalled);

  SourceParseContext modulepc(this, modulesc, nullptr);
  if (!modulepc.init()) {
    return null();
  }

  ParseContext::VarScope varScope(this);
  if (!varScope.init(pc)) {
    return nullptr;
  }

  CodeNodeType moduleNode = handler.newModule(pos());
  if (!moduleNode) {
    return null();
  }

  AutoAwaitIsKeyword<FullParseHandler, Unit> awaitIsKeyword(
      this, AwaitIsModuleKeyword);
  ListNode* stmtList = statementList(YieldIsName);
  if (!stmtList) {
    return null();
  }

  MOZ_ASSERT(stmtList->isKind(ParseNodeKind::StatementList));
  moduleNode->setBody(&stmtList->as<ListNode>());

  TokenKind tt;
  if (!tokenStream.getToken(&tt, TokenStream::Operand)) {
    return null();
  }
  if (tt != TokenKind::Eof) {
    error(JSMSG_GARBAGE_AFTER_INPUT, "module", TokenKindToDesc(tt));
    return null();
  }

  if (!modulesc->builder.buildTables()) {
    return null();
  }

  // Check exported local bindings exist and mark them as closed over.
  for (auto entry : modulesc->builder.localExportEntries()) {
    JSAtom* name = entry->localName();
    MOZ_ASSERT(name);

    DeclaredNamePtr p = modulepc.varScope().lookupDeclaredName(name);
    if (!p) {
      UniqueChars str = AtomToPrintableString(context, name);
      if (!str) {
        return null();
      }

      errorAt(TokenStream::NoOffset, JSMSG_MISSING_EXPORT, str.get());
      return null();
    }

    p->value()->setClosedOver();
  }

  ParseNode* node = stmtList;
  if (!FoldConstants(context, &node, this)) {
    return null();
  }
  stmtList = &node->as<ListNode>();
||||||| merged common ancestors
    // If an early error would have occurred already, this function should not
    // exhibit Annex B.3.3 semantics.
    return !redeclaredKind;
}

template <class ParseHandler, typename Unit>
bool
GeneralParser<ParseHandler, Unit>::checkLexicalDeclarationDirectlyWithinBlock(ParseContext::Statement& stmt,
                                                                              DeclarationKind kind,
                                                                              TokenPos pos)
{
    MOZ_ASSERT(DeclarationKindIsLexical(kind));

    // It is an early error to declare a lexical binding not directly
    // within a block.
    if (!StatementKindIsBraced(stmt.kind()) &&
        stmt.kind() != StatementKind::ForLoopLexicalHead)
    {
        errorAt(pos.begin,
                stmt.kind() == StatementKind::Label
                ? JSMSG_LEXICAL_DECL_LABEL
                : JSMSG_LEXICAL_DECL_NOT_IN_BLOCK,
                DeclarationKindString(kind));
        return false;
    }

    return true;
}

template <class ParseHandler, typename Unit>
bool
GeneralParser<ParseHandler, Unit>::noteDeclaredName(HandlePropertyName name, DeclarationKind kind,
                                                    TokenPos pos)
{
    // The asm.js validator does all its own symbol-table management so, as an
    // optimization, avoid doing any work here.
    if (pc->useAsmOrInsideUseAsm()) {
        return true;
    }

    switch (kind) {
      case DeclarationKind::Var:
      case DeclarationKind::BodyLevelFunction:
      case DeclarationKind::ForOfVar: {
        Maybe<DeclarationKind> redeclaredKind;
        uint32_t prevPos;
        if (!pc->tryDeclareVar(name, kind, pos.begin, &redeclaredKind, &prevPos)) {
            return false;
        }

        if (redeclaredKind) {
            reportRedeclaration(name, *redeclaredKind, pos, prevPos);
            return false;
        }

        break;
      }

      case DeclarationKind::ModuleBodyLevelFunction: {
          MOZ_ASSERT(pc->atModuleLevel());

          AddDeclaredNamePtr p = pc->varScope().lookupDeclaredNameForAdd(name);
          if (p) {
              reportRedeclaration(name, p->value()->kind(), pos, p->value()->pos());
              return false;
          }

          if (!pc->varScope().addDeclaredName(pc, p, name, kind, pos.begin)) {
              return false;
          }

          // Body-level functions in modules are always closed over.
          pc->varScope().lookupDeclaredName(name)->value()->setClosedOver();

          break;
      }

      case DeclarationKind::FormalParameter: {
        // It is an early error if any non-positional formal parameter name
        // (e.g., destructuring formal parameter) is duplicated.

        AddDeclaredNamePtr p = pc->functionScope().lookupDeclaredNameForAdd(name);
        if (p) {
            error(JSMSG_BAD_DUP_ARGS);
            return false;
        }

        if (!pc->functionScope().addDeclaredName(pc, p, name, kind, pos.begin)) {
            return false;
        }

        break;
      }

      case DeclarationKind::LexicalFunction: {
        ParseContext::Scope* scope = pc->innermostScope();
        AddDeclaredNamePtr p = scope->lookupDeclaredNameForAdd(name);
        if (p) {
            reportRedeclaration(name, p->value()->kind(), pos, p->value()->pos());
            return false;
        }

        if (!scope->addDeclaredName(pc, p, name, kind, pos.begin)) {
            return false;
        }

        break;
      }

      case DeclarationKind::SloppyLexicalFunction: {
        // Functions in block have complex allowances in sloppy mode for being
        // labelled that other lexical declarations do not have. Those checks
        // are more complex than calling checkLexicalDeclarationDirectlyWithin-
        // Block and are done in checkFunctionDefinition.

        ParseContext::Scope* scope = pc->innermostScope();
        if (AddDeclaredNamePtr p = scope->lookupDeclaredNameForAdd(name)) {
            // It is usually an early error if there is another declaration
            // with the same name in the same scope.
            //
            // Sloppy lexical functions may redeclare other sloppy lexical
            // functions for web compatibility reasons.
            if (p->value()->kind() != DeclarationKind::SloppyLexicalFunction) {
                reportRedeclaration(name, p->value()->kind(), pos, p->value()->pos());
                return false;
            }
        } else {
            if (!scope->addDeclaredName(pc, p, name, kind, pos.begin)) {
                return false;
            }
        }

        break;
      }

      case DeclarationKind::Let:
      case DeclarationKind::Const:
      case DeclarationKind::Class:
        // The BoundNames of LexicalDeclaration and ForDeclaration must not
        // contain 'let'. (CatchParameter is the only lexical binding form
        // without this restriction.)
        if (name == context->names().let) {
            errorAt(pos.begin, JSMSG_LEXICAL_DECL_DEFINES_LET);
            return false;
        }

        MOZ_FALLTHROUGH;

      case DeclarationKind::Import:
        // Module code is always strict, so 'let' is always a keyword and never a name.
        MOZ_ASSERT(name != context->names().let);
        MOZ_FALLTHROUGH;

      case DeclarationKind::SimpleCatchParameter:
      case DeclarationKind::CatchParameter: {
        if (ParseContext::Statement* stmt = pc->innermostStatement()) {
            if (!checkLexicalDeclarationDirectlyWithinBlock(*stmt, kind, pos)) {
                return false;
            }
        }

        ParseContext::Scope* scope = pc->innermostScope();

        // For body-level lexically declared names in a function, it is an
        // early error if there is a formal parameter of the same name. This
        // needs a special check if there is an extra var scope due to
        // parameter expressions.
        if (pc->isFunctionExtraBodyVarScopeInnermost()) {
            DeclaredNamePtr p = pc->functionScope().lookupDeclaredName(name);
            if (p && DeclarationKindIsParameter(p->value()->kind())) {
                reportRedeclaration(name, p->value()->kind(), pos, p->value()->pos());
                return false;
            }
        }

        // It is an early error if there is another declaration with the same
        // name in the same scope.
        AddDeclaredNamePtr p = scope->lookupDeclaredNameForAdd(name);
        if (p) {
            reportRedeclaration(name, p->value()->kind(), pos, p->value()->pos());
            return false;
        }

        if (!scope->addDeclaredName(pc, p, name, kind, pos.begin)) {
            return false;
        }

        break;
      }

      case DeclarationKind::CoverArrowParameter:
        // CoverArrowParameter is only used as a placeholder declaration kind.
        break;

      case DeclarationKind::PositionalFormalParameter:
        MOZ_CRASH("Positional formal parameter names should use "
                  "notePositionalFormalParameter");
        break;

      case DeclarationKind::VarForAnnexBLexicalFunction:
        MOZ_CRASH("Synthesized Annex B vars should go through "
                  "tryDeclareVarForAnnexBLexicalFunction");
        break;
    }

    return true;
}

bool
ParserBase::noteUsedNameInternal(HandlePropertyName name)
{
    // The asm.js validator does all its own symbol-table management so, as an
    // optimization, avoid doing any work here.
    if (pc->useAsmOrInsideUseAsm()) {
        return true;
    }

    // Global bindings are properties and not actual bindings; we don't need
    // to know if they are closed over. So no need to track used name at the
    // global scope. It is not incorrect to track them, this is an
    // optimization.
    ParseContext::Scope* scope = pc->innermostScope();
    if (pc->sc()->isGlobalContext() && scope == &pc->varScope()) {
        return true;
    }

    return usedNames.noteUse(context, name, pc->scriptId(), scope->id());
}

bool
ParserBase::hasUsedName(HandlePropertyName name)
{
    if (UsedNamePtr p = usedNames.lookup(name)) {
        return p->value().isUsedInScript(pc->scriptId());
    }
    return false;
}

template <class ParseHandler>
bool
PerHandlerParser<ParseHandler>::propagateFreeNamesAndMarkClosedOverBindings(ParseContext::Scope& scope)
{
    // Now that we have all the declared names in the scope, check which
    // functions should exhibit Annex B semantics.
    if (!scope.propagateAndMarkAnnexBFunctionBoxes(pc)) {
        return false;
    }

    if (handler.canSkipLazyClosedOverBindings()) {
        // Scopes are nullptr-delimited in the LazyScript closed over bindings
        // array.
        while (JSAtom* name = handler.nextLazyClosedOverBinding()) {
            scope.lookupDeclaredName(name)->value()->setClosedOver();
        }
        return true;
    }

    bool isSyntaxParser = mozilla::IsSame<ParseHandler, SyntaxParseHandler>::value;
    uint32_t scriptId = pc->scriptId();
    uint32_t scopeId = scope.id();
    for (BindingIter bi = scope.bindings(pc); bi; bi++) {
        if (UsedNamePtr p = usedNames.lookup(bi.name())) {
            bool closedOver;
            p->value().noteBoundInScope(scriptId, scopeId, &closedOver);
            if (closedOver) {
                bi.setClosedOver();

                if (isSyntaxParser && !pc->closedOverBindingsForLazy().append(bi.name())) {
                    ReportOutOfMemory(context);
                    return false;
                }
            }
        }
    }

    // Append a nullptr to denote end-of-scope.
    if (isSyntaxParser && !pc->closedOverBindingsForLazy().append(nullptr)) {
        ReportOutOfMemory(context);
        return false;
    }

    return true;
}

template <typename Unit>
bool
Parser<FullParseHandler, Unit>::checkStatementsEOF()
{
    // This is designed to be paired with parsing a statement list at the top
    // level.
    //
    // The statementList() call breaks on TokenKind::RightCurly, so make sure
    // we've reached EOF here.
    TokenKind tt;
    if (!tokenStream.peekToken(&tt, TokenStream::Operand)) {
        return false;
    }
    if (tt != TokenKind::Eof) {
        error(JSMSG_UNEXPECTED_TOKEN, "expression", TokenKindToDesc(tt));
        return false;
    }
    return true;
}

template <typename Scope>
typename Scope::Data*
NewEmptyBindingData(JSContext* cx, LifoAlloc& alloc, uint32_t numBindings)
{
    using Data = typename Scope::Data;
    size_t allocSize = SizeOfData<typename Scope::Data>(numBindings);
    auto* bindings = alloc.allocInSize<Data>(allocSize, numBindings);
    if (!bindings) {
        ReportOutOfMemory(cx);
    }
    return bindings;
}

namespace detail {

template<class Data>
static MOZ_ALWAYS_INLINE BindingName*
InitializeIndexedBindings(Data* data, BindingName* start, BindingName* cursor)
{
    return cursor;
}

template<class Data, typename UnsignedInteger, typename... Step>
static MOZ_ALWAYS_INLINE BindingName*
InitializeIndexedBindings(Data* data, BindingName* start, BindingName* cursor,
                          UnsignedInteger Data::* field, const BindingNameVector& bindings,
                          Step&&... step)
{
    data->*field = AssertedCast<UnsignedInteger>(PointerRangeSize(start, cursor));

    BindingName* newCursor = std::uninitialized_copy(bindings.begin(), bindings.end(), cursor);

    return InitializeIndexedBindings(data, start, newCursor, std::forward<Step>(step)...);
}

} // namespace detail

// Initialize |data->trailingNames| bindings, then set |data->length| to the
// count of bindings added (which must equal |count|).
//
// First, |firstBindings| are added to |data->trailingNames|.  Then any "steps"
// present are performed first to last.  Each step is 1) a pointer to a member
// of |data| to be set to the current number of bindings added, and 2) a vector
// of |BindingName|s to then copy into |data->trailingNames|.  (Thus each
// |data| member field indicates where the corresponding vector's names start.)
template<class Data, typename... Step>
static MOZ_ALWAYS_INLINE void
InitializeBindingData(Data* data, uint32_t count,
                      const BindingNameVector& firstBindings,
                      Step&&... step)
{
    MOZ_ASSERT(data->length == 0, "data shouldn't be filled yet");

    BindingName* start = data->trailingNames.start();
    BindingName* cursor =
        std::uninitialized_copy(firstBindings.begin(), firstBindings.end(), start);

#ifdef DEBUG
    BindingName* end =
#endif
        detail::InitializeIndexedBindings(data, start, cursor, std::forward<Step>(step)...);

    MOZ_ASSERT(PointerRangeSize(start, end) == count);
    data->length = count;
}

Maybe<GlobalScope::Data*>
NewGlobalScopeData(JSContext* context, ParseContext::Scope& scope, LifoAlloc& alloc, ParseContext* pc)
{
    BindingNameVector vars(context);
    BindingNameVector lets(context);
    BindingNameVector consts(context);

    bool allBindingsClosedOver = pc->sc()->allBindingsClosedOver();
    for (BindingIter bi = scope.bindings(pc); bi; bi++) {
        bool closedOver = allBindingsClosedOver || bi.closedOver();

        switch (bi.kind()) {
          case BindingKind::Var: {
            bool isTopLevelFunction = bi.declarationKind() == DeclarationKind::BodyLevelFunction;
            BindingName binding(bi.name(), closedOver, isTopLevelFunction);
            if (!vars.append(binding)) {
                return Nothing();
            }
            break;
          }
          case BindingKind::Let: {
            BindingName binding(bi.name(), closedOver);
            if (!lets.append(binding)) {
                return Nothing();
            }
            break;
          }
          case BindingKind::Const: {
            BindingName binding(bi.name(), closedOver);
            if (!consts.append(binding)) {
                return Nothing();
            }
            break;
          }
          default:
            MOZ_CRASH("Bad global scope BindingKind");
        }
    }

    GlobalScope::Data* bindings = nullptr;
    uint32_t numBindings = vars.length() + lets.length() + consts.length();

    if (numBindings > 0) {
        bindings = NewEmptyBindingData<GlobalScope>(context, alloc, numBindings);
        if (!bindings) {
            return Nothing();
        }

        // The ordering here is important. See comments in GlobalScope.
        InitializeBindingData(bindings, numBindings,
                              vars,
                              &GlobalScope::Data::letStart, lets,
                              &GlobalScope::Data::constStart, consts);
    }

    return Some(bindings);
}

Maybe<GlobalScope::Data*>
ParserBase::newGlobalScopeData(ParseContext::Scope& scope)
{
    return NewGlobalScopeData(context, scope, alloc, pc);
}

Maybe<ModuleScope::Data*>
NewModuleScopeData(JSContext* context, ParseContext::Scope& scope, LifoAlloc& alloc, ParseContext* pc)
{
    BindingNameVector imports(context);
    BindingNameVector vars(context);
    BindingNameVector lets(context);
    BindingNameVector consts(context);

    bool allBindingsClosedOver = pc->sc()->allBindingsClosedOver();
    for (BindingIter bi = scope.bindings(pc); bi; bi++) {
        // Imports are indirect bindings and must not be given known slots.
        BindingName binding(bi.name(), (allBindingsClosedOver || bi.closedOver()) &&
                                       bi.kind() != BindingKind::Import);
        switch (bi.kind()) {
          case BindingKind::Import:
            if (!imports.append(binding)) {
                return Nothing();
            }
            break;
          case BindingKind::Var:
            if (!vars.append(binding)) {
                return Nothing();
            }
            break;
          case BindingKind::Let:
            if (!lets.append(binding)) {
                return Nothing();
            }
            break;
          case BindingKind::Const:
            if (!consts.append(binding)) {
                return Nothing();
            }
            break;
          default:
            MOZ_CRASH("Bad module scope BindingKind");
        }
    }

    ModuleScope::Data* bindings = nullptr;
    uint32_t numBindings = imports.length() + vars.length() + lets.length() + consts.length();

    if (numBindings > 0) {
        bindings = NewEmptyBindingData<ModuleScope>(context, alloc, numBindings);
        if (!bindings) {
            return Nothing();
        }

        // The ordering here is important. See comments in ModuleScope.
        InitializeBindingData(bindings, numBindings,
                              imports,
                              &ModuleScope::Data::varStart, vars,
                              &ModuleScope::Data::letStart, lets,
                              &ModuleScope::Data::constStart, consts);
    }

    return Some(bindings);
}

Maybe<ModuleScope::Data*>
ParserBase::newModuleScopeData(ParseContext::Scope& scope)
{
    return NewModuleScopeData(context, scope, alloc, pc);
}

Maybe<EvalScope::Data*>
NewEvalScopeData(JSContext* context, ParseContext::Scope& scope, LifoAlloc& alloc, ParseContext* pc)
{
    BindingNameVector vars(context);

    for (BindingIter bi = scope.bindings(pc); bi; bi++) {
        // Eval scopes only contain 'var' bindings. Make all bindings aliased
        // for now.
        MOZ_ASSERT(bi.kind() == BindingKind::Var);
        bool isTopLevelFunction = bi.declarationKind() == DeclarationKind::BodyLevelFunction;
        BindingName binding(bi.name(), true, isTopLevelFunction);
        if (!vars.append(binding)) {
            return Nothing();
        }
    }

    EvalScope::Data* bindings = nullptr;
    uint32_t numBindings = vars.length();

    if (numBindings > 0) {
        bindings = NewEmptyBindingData<EvalScope>(context, alloc, numBindings);
        if (!bindings) {
            return Nothing();
        }

        InitializeBindingData(bindings, numBindings, vars);
    }

    return Some(bindings);
}

Maybe<EvalScope::Data*>
ParserBase::newEvalScopeData(ParseContext::Scope& scope)
{
    return NewEvalScopeData(context, scope, alloc, pc);
}

Maybe<FunctionScope::Data*>
NewFunctionScopeData(JSContext* context, ParseContext::Scope& scope, bool hasParameterExprs, LifoAlloc& alloc, ParseContext* pc)
{
    BindingNameVector positionalFormals(context);
    BindingNameVector formals(context);
    BindingNameVector vars(context);

    bool allBindingsClosedOver = pc->sc()->allBindingsClosedOver();
    bool hasDuplicateParams = pc->functionBox()->hasDuplicateParameters;

    // Positional parameter names must be added in order of appearance as they are
    // referenced using argument slots.
    for (size_t i = 0; i < pc->positionalFormalParameterNames().length(); i++) {
        JSAtom* name = pc->positionalFormalParameterNames()[i];

        BindingName bindName;
        if (name) {
            DeclaredNamePtr p = scope.lookupDeclaredName(name);

            // Do not consider any positional formal parameters closed over if
            // there are parameter defaults. It is the binding in the defaults
            // scope that is closed over instead.
            bool closedOver = allBindingsClosedOver ||
                              (p && p->value()->closedOver());

            // If the parameter name has duplicates, only the final parameter
            // name should be on the environment, as otherwise the environment
            // object would have multiple, same-named properties.
            if (hasDuplicateParams) {
                for (size_t j = pc->positionalFormalParameterNames().length() - 1; j > i; j--) {
                    if (pc->positionalFormalParameterNames()[j] == name) {
                        closedOver = false;
                        break;
                    }
                }
            }

            bindName = BindingName(name, closedOver);
        }

        if (!positionalFormals.append(bindName)) {
            return Nothing();
        }
    }

    for (BindingIter bi = scope.bindings(pc); bi; bi++) {
        BindingName binding(bi.name(), allBindingsClosedOver || bi.closedOver());
        switch (bi.kind()) {
          case BindingKind::FormalParameter:
            // Positional parameter names are already handled above.
            if (bi.declarationKind() == DeclarationKind::FormalParameter) {
                if (!formals.append(binding)) {
                    return Nothing();
                }
            }
            break;
          case BindingKind::Var:
            // The only vars in the function scope when there are parameter
            // exprs, which induces a separate var environment, should be the
            // special bindings.
            MOZ_ASSERT_IF(hasParameterExprs, FunctionScope::isSpecialName(context, bi.name()));
            if (!vars.append(binding)) {
                return Nothing();
            }
            break;
          default:
            break;
        }
    }

    FunctionScope::Data* bindings = nullptr;
    uint32_t numBindings = positionalFormals.length() + formals.length() + vars.length();

    if (numBindings > 0) {
        bindings = NewEmptyBindingData<FunctionScope>(context, alloc, numBindings);
        if (!bindings) {
            return Nothing();
        }

        // The ordering here is important. See comments in FunctionScope.
        InitializeBindingData(bindings, numBindings,
                              positionalFormals,
                              &FunctionScope::Data::nonPositionalFormalStart, formals,
                              &FunctionScope::Data::varStart, vars);
    }

    return Some(bindings);
}

Maybe<FunctionScope::Data*>
ParserBase::newFunctionScopeData(ParseContext::Scope& scope, bool hasParameterExprs)
{
    return NewFunctionScopeData(context, scope, hasParameterExprs, alloc, pc);
}

Maybe<VarScope::Data*>
NewVarScopeData(JSContext* context, ParseContext::Scope& scope, LifoAlloc& alloc, ParseContext* pc)
{
    BindingNameVector vars(context);

    bool allBindingsClosedOver = pc->sc()->allBindingsClosedOver();

    for (BindingIter bi = scope.bindings(pc); bi; bi++) {
        if (bi.kind() == BindingKind::Var) {
            BindingName binding(bi.name(), allBindingsClosedOver || bi.closedOver());
            if (!vars.append(binding)) {
                return Nothing();
            }
        }
    }

    VarScope::Data* bindings = nullptr;
    uint32_t numBindings = vars.length();

    if (numBindings > 0) {
        bindings = NewEmptyBindingData<VarScope>(context, alloc, numBindings);
        if (!bindings) {
            return Nothing();
        }

        InitializeBindingData(bindings, numBindings, vars);
    }

    return Some(bindings);
}

Maybe<VarScope::Data*>
ParserBase::newVarScopeData(ParseContext::Scope& scope)
{
    return NewVarScopeData(context, scope, alloc, pc);
}

Maybe<LexicalScope::Data*>
NewLexicalScopeData(JSContext* context, ParseContext::Scope& scope, LifoAlloc& alloc, ParseContext* pc)
{
    BindingNameVector lets(context);
    BindingNameVector consts(context);

    // Unlike other scopes with bindings which are body-level, it is unknown
    // if pc->sc()->allBindingsClosedOver() is correct at the time of
    // finishing parsing a lexical scope.
    //
    // Instead, pc->sc()->allBindingsClosedOver() is checked in
    // EmitterScope::enterLexical. Also see comment there.
    for (BindingIter bi = scope.bindings(pc); bi; bi++) {
        BindingName binding(bi.name(), bi.closedOver());
        switch (bi.kind()) {
          case BindingKind::Let:
            if (!lets.append(binding)) {
                return Nothing();
            }
            break;
          case BindingKind::Const:
            if (!consts.append(binding)) {
                return Nothing();
            }
            break;
          default:
            break;
        }
    }

    LexicalScope::Data* bindings = nullptr;
    uint32_t numBindings = lets.length() + consts.length();

    if (numBindings > 0) {
        bindings = NewEmptyBindingData<LexicalScope>(context, alloc, numBindings);
        if (!bindings) {
            return Nothing();
        }

        // The ordering here is important. See comments in LexicalScope.
        InitializeBindingData(bindings, numBindings,
                              lets,
                              &LexicalScope::Data::constStart, consts);
    }

    return Some(bindings);
}

Maybe<LexicalScope::Data*>
ParserBase::newLexicalScopeData(ParseContext::Scope& scope)
{
    return NewLexicalScopeData(context, scope, alloc, pc);
}

template <>
SyntaxParseHandler::LexicalScopeNodeType
PerHandlerParser<SyntaxParseHandler>::finishLexicalScope(ParseContext::Scope& scope, Node body)
{
    if (!propagateFreeNamesAndMarkClosedOverBindings(scope)) {
        return null();
    }

    return handler.newLexicalScope(body);
}

template <>
LexicalScopeNode*
PerHandlerParser<FullParseHandler>::finishLexicalScope(ParseContext::Scope& scope, ParseNode* body)
{
    if (!propagateFreeNamesAndMarkClosedOverBindings(scope)) {
        return nullptr;
    }

    Maybe<LexicalScope::Data*> bindings = newLexicalScopeData(scope);
    if (!bindings) {
        return nullptr;
    }

    return handler.newLexicalScope(*bindings, body);
}

template <typename Unit>
LexicalScopeNode*
Parser<FullParseHandler, Unit>::evalBody(EvalSharedContext* evalsc)
{
    SourceParseContext evalpc(this, evalsc, /* newDirectives = */ nullptr);
    if (!evalpc.init()) {
        return nullptr;
    }

    ParseContext::VarScope varScope(this);
    if (!varScope.init(pc)) {
        return nullptr;
    }

    LexicalScopeNode* body;
    {
        // All evals have an implicit non-extensible lexical scope.
        ParseContext::Scope lexicalScope(this);
        if (!lexicalScope.init(pc)) {
            return nullptr;
        }

        ParseNode* list = statementList(YieldIsName);
        if (!list) {
            return nullptr;
        }

        if (!checkStatementsEOF()) {
            return nullptr;
        }

        body = finishLexicalScope(lexicalScope, list);
        if (!body) {
            return nullptr;
        }
    }

#ifdef DEBUG
    if (evalpc.superScopeNeedsHomeObject() && evalsc->compilationEnclosingScope()) {
        // If superScopeNeedsHomeObject_ is set and we are an entry-point
        // ParseContext, then we must be emitting an eval script, and the
        // outer function must already be marked as needing a home object
        // since it contains an eval.
        ScopeIter si(evalsc->compilationEnclosingScope());
        for (; si; si++) {
            if (si.kind() == ScopeKind::Function) {
                JSFunction* fun = si.scope()->as<FunctionScope>().canonicalFunction();
                if (fun->isArrow()) {
                    continue;
                }
                MOZ_ASSERT(fun->allowSuperProperty());
                MOZ_ASSERT(fun->nonLazyScript()->needsHomeObject());
                break;
            }
        }
        MOZ_ASSERT(!si.done(),
                   "Eval must have found an enclosing function box scope that allows super.property");
    }
#endif

    ParseNode* node = body;
    if (!FoldConstants(context, &node, this)) {
        return nullptr;
    }
    body = handler.asLexicalScope(node);

    if (!this->setSourceMapInfo()) {
        return nullptr;
    }

    // For eval scripts, since all bindings are automatically considered
    // closed over, we don't need to call propagateFreeNamesAndMarkClosed-
    // OverBindings. However, Annex B.3.3 functions still need to be marked.
    if (!varScope.propagateAndMarkAnnexBFunctionBoxes(pc)) {
        return nullptr;
    }

    Maybe<EvalScope::Data*> bindings = newEvalScopeData(pc->varScope());
    if (!bindings) {
        return nullptr;
    }
    evalsc->bindings = *bindings;

    return body;
}

template <typename Unit>
ListNode*
Parser<FullParseHandler, Unit>::globalBody(GlobalSharedContext* globalsc)
{
    SourceParseContext globalpc(this, globalsc, /* newDirectives = */ nullptr);
    if (!globalpc.init()) {
        return nullptr;
    }

    ParseContext::VarScope varScope(this);
    if (!varScope.init(pc)) {
        return nullptr;
    }

    ListNode* body = statementList(YieldIsName);
    if (!body) {
        return nullptr;
    }

    if (!checkStatementsEOF()) {
        return nullptr;
    }

    ParseNode* node = body;
    if (!FoldConstants(context, &node, this)) {
        return null();
    }
    body = &node->as<ListNode>();

    if (!this->setSourceMapInfo()) {
        return nullptr;
    }

    // For global scripts, whether bindings are closed over or not doesn't
    // matter, so no need to call propagateFreeNamesAndMarkClosedOver-
    // Bindings. However, Annex B.3.3 functions still need to be marked.
    if (!varScope.propagateAndMarkAnnexBFunctionBoxes(pc)) {
        return nullptr;
    }

    Maybe<GlobalScope::Data*> bindings = newGlobalScopeData(pc->varScope());
    if (!bindings) {
        return nullptr;
    }
    globalsc->bindings = *bindings;

    return body;
}

template <typename Unit>
CodeNode*
Parser<FullParseHandler, Unit>::moduleBody(ModuleSharedContext* modulesc)
{
    MOZ_ASSERT(checkOptionsCalled);

    SourceParseContext modulepc(this, modulesc, nullptr);
    if (!modulepc.init()) {
        return null();
    }

    ParseContext::VarScope varScope(this);
    if (!varScope.init(pc)) {
        return nullptr;
    }

    CodeNodeType moduleNode = handler.newModule(pos());
    if (!moduleNode) {
        return null();
    }

    AutoAwaitIsKeyword<FullParseHandler, Unit> awaitIsKeyword(this, AwaitIsModuleKeyword);
    ListNode* stmtList = statementList(YieldIsName);
    if (!stmtList) {
        return null();
    }

    MOZ_ASSERT(stmtList->isKind(ParseNodeKind::StatementList));
    moduleNode->setBody(&stmtList->as<ListNode>());

    TokenKind tt;
    if (!tokenStream.getToken(&tt, TokenStream::Operand)) {
        return null();
    }
    if (tt != TokenKind::Eof) {
        error(JSMSG_GARBAGE_AFTER_INPUT, "module", TokenKindToDesc(tt));
        return null();
    }

    if (!modulesc->builder.buildTables()) {
        return null();
    }

    // Check exported local bindings exist and mark them as closed over.
    for (auto entry : modulesc->builder.localExportEntries()) {
        JSAtom* name = entry->localName();
        MOZ_ASSERT(name);

        DeclaredNamePtr p = modulepc.varScope().lookupDeclaredName(name);
        if (!p) {
            UniqueChars str = AtomToPrintableString(context, name);
            if (!str) {
                return null();
            }

            errorAt(TokenStream::NoOffset, JSMSG_MISSING_EXPORT, str.get());
            return null();
        }

        p->value()->setClosedOver();
    }

    ParseNode* node = stmtList;
    if (!FoldConstants(context, &node, this)) {
        return null();
    }
    stmtList = &node->as<ListNode>();

    if (!this->setSourceMapInfo()) {
        return null();
    }
=======
  if (!this->setSourceMapInfo()) {
    return null();
  }
>>>>>>> upstream-releases

<<<<<<< HEAD
  if (!this->setSourceMapInfo()) {
    return null();
  }
||||||| merged common ancestors
    if (!propagateFreeNamesAndMarkClosedOverBindings(modulepc.varScope())) {
        return null();
    }
=======
  if (!propagateFreeNamesAndMarkClosedOverBindings(modulepc.varScope())) {
    return null();
  }
>>>>>>> upstream-releases

<<<<<<< HEAD
  if (!propagateFreeNamesAndMarkClosedOverBindings(modulepc.varScope())) {
    return null();
  }

  Maybe<ModuleScope::Data*> bindings = newModuleScopeData(modulepc.varScope());
  if (!bindings) {
    return nullptr;
  }
||||||| merged common ancestors
    Maybe<ModuleScope::Data*> bindings = newModuleScopeData(modulepc.varScope());
    if (!bindings) {
        return nullptr;
    }
=======
  Maybe<ModuleScope::Data*> bindings = newModuleScopeData(modulepc.varScope());
  if (!bindings) {
    return nullptr;
  }
>>>>>>> upstream-releases

  modulesc->bindings = *bindings;
  return moduleNode;
}

template <typename Unit>
<<<<<<< HEAD
SyntaxParseHandler::CodeNodeType Parser<SyntaxParseHandler, Unit>::moduleBody(
    ModuleSharedContext* modulesc) {
  MOZ_ALWAYS_FALSE(abortIfSyntaxParser());
  return SyntaxParseHandler::NodeFailure;
}

bool ParserBase::hasUsedFunctionSpecialName(HandlePropertyName name) {
  MOZ_ASSERT(name == context->names().arguments ||
             name == context->names().dotThis);
  return hasUsedName(name) || pc->functionBox()->bindingsAccessedDynamically();
}

template <class ParseHandler>
bool PerHandlerParser<ParseHandler>::declareFunctionThis() {
  // The asm.js validator does all its own symbol-table management so, as an
  // optimization, avoid doing any work here.
  if (pc->useAsmOrInsideUseAsm()) {
    return true;
  }

  // Derived class constructors emit JSOP_CHECKRETURN, which requires
  // '.this' to be bound.
  FunctionBox* funbox = pc->functionBox();
  HandlePropertyName dotThis = context->names().dotThis;

  bool declareThis;
  if (handler.canSkipLazyClosedOverBindings()) {
    declareThis = funbox->function()->lazyScript()->hasThisBinding();
  } else {
    declareThis = hasUsedFunctionSpecialName(dotThis) ||
                  funbox->isDerivedClassConstructor();
  }

  if (declareThis) {
    ParseContext::Scope& funScope = pc->functionScope();
    AddDeclaredNamePtr p = funScope.lookupDeclaredNameForAdd(dotThis);
    MOZ_ASSERT(!p);
    if (!funScope.addDeclaredName(pc, p, dotThis, DeclarationKind::Var,
                                  DeclaredNameInfo::npos)) {
      return false;
    }
    funbox->setHasThisBinding();
  }

  return true;
||||||| merged common ancestors
SyntaxParseHandler::CodeNodeType
Parser<SyntaxParseHandler, Unit>::moduleBody(ModuleSharedContext* modulesc)
{
    MOZ_ALWAYS_FALSE(abortIfSyntaxParser());
    return SyntaxParseHandler::NodeFailure;
}

bool
ParserBase::hasUsedFunctionSpecialName(HandlePropertyName name)
{
    MOZ_ASSERT(name == context->names().arguments || name == context->names().dotThis);
    return hasUsedName(name) || pc->functionBox()->bindingsAccessedDynamically();
}

template <class ParseHandler>
bool
PerHandlerParser<ParseHandler>::declareFunctionThis()
{
    // The asm.js validator does all its own symbol-table management so, as an
    // optimization, avoid doing any work here.
    if (pc->useAsmOrInsideUseAsm()) {
        return true;
    }

    // Derived class constructors emit JSOP_CHECKRETURN, which requires
    // '.this' to be bound.
    FunctionBox* funbox = pc->functionBox();
    HandlePropertyName dotThis = context->names().dotThis;

    bool declareThis;
    if (handler.canSkipLazyClosedOverBindings()) {
        declareThis = funbox->function()->lazyScript()->hasThisBinding();
    } else {
        declareThis = hasUsedFunctionSpecialName(dotThis) || funbox->isDerivedClassConstructor();
    }

    if (declareThis) {
        ParseContext::Scope& funScope = pc->functionScope();
        AddDeclaredNamePtr p = funScope.lookupDeclaredNameForAdd(dotThis);
        MOZ_ASSERT(!p);
        if (!funScope.addDeclaredName(pc, p, dotThis, DeclarationKind::Var,
                                      DeclaredNameInfo::npos))
        {
            return false;
        }
        funbox->setHasThisBinding();
    }

    return true;
=======
SyntaxParseHandler::ModuleNodeType Parser<SyntaxParseHandler, Unit>::moduleBody(
    ModuleSharedContext* modulesc) {
  MOZ_ALWAYS_FALSE(abortIfSyntaxParser());
  return SyntaxParseHandler::NodeFailure;
>>>>>>> upstream-releases
}

template <class ParseHandler>
typename ParseHandler::NameNodeType
PerHandlerParser<ParseHandler>::newInternalDotName(HandlePropertyName name) {
  NameNodeType nameNode = newName(name);
  if (!nameNode) {
    return null();
  }
  if (!noteUsedName(name)) {
    return null();
  }
  return nameNode;
}

template <class ParseHandler>
typename ParseHandler::NameNodeType
<<<<<<< HEAD
PerHandlerParser<ParseHandler>::newThisName() {
  return newInternalDotName(context->names().dotThis);
||||||| merged common ancestors
PerHandlerParser<ParseHandler>::newThisName()
{
    return newInternalDotName(context->names().dotThis);
=======
PerHandlerParser<ParseHandler>::newThisName() {
  return newInternalDotName(cx_->names().dotThis);
>>>>>>> upstream-releases
}

template <class ParseHandler>
typename ParseHandler::NameNodeType
<<<<<<< HEAD
PerHandlerParser<ParseHandler>::newDotGeneratorName() {
  return newInternalDotName(context->names().dotGenerator);
}

bool ParserBase::declareDotGeneratorName() {
  // The special '.generator' binding must be on the function scope, as
  // generators expect to find it on the CallObject.
  ParseContext::Scope& funScope = pc->functionScope();
  HandlePropertyName dotGenerator = context->names().dotGenerator;
  AddDeclaredNamePtr p = funScope.lookupDeclaredNameForAdd(dotGenerator);
  if (!p && !funScope.addDeclaredName(pc, p, dotGenerator, DeclarationKind::Var,
                                      DeclaredNameInfo::npos)) {
    return false;
  }
  return true;
||||||| merged common ancestors
PerHandlerParser<ParseHandler>::newDotGeneratorName()
{
    return newInternalDotName(context->names().dotGenerator);
}

bool
ParserBase::declareDotGeneratorName()
{
    // The special '.generator' binding must be on the function scope, as
    // generators expect to find it on the CallObject.
    ParseContext::Scope& funScope = pc->functionScope();
    HandlePropertyName dotGenerator = context->names().dotGenerator;
    AddDeclaredNamePtr p = funScope.lookupDeclaredNameForAdd(dotGenerator);
    if (!p && !funScope.addDeclaredName(pc, p, dotGenerator, DeclarationKind::Var,
                                        DeclaredNameInfo::npos))
    {
        return false;
    }
    return true;
=======
PerHandlerParser<ParseHandler>::newDotGeneratorName() {
  return newInternalDotName(cx_->names().dotGenerator);
>>>>>>> upstream-releases
}

template <class ParseHandler>
<<<<<<< HEAD
bool PerHandlerParser<ParseHandler>::finishFunctionScopes(
    bool isStandaloneFunction) {
  FunctionBox* funbox = pc->functionBox();

  if (funbox->hasParameterExprs) {
    if (!propagateFreeNamesAndMarkClosedOverBindings(pc->functionScope())) {
      return false;
    }
  }

  if (funbox->function()->isNamedLambda() && !isStandaloneFunction) {
    if (!propagateFreeNamesAndMarkClosedOverBindings(pc->namedLambdaScope())) {
      return false;
    }
  }

  return true;
}

template <>
bool PerHandlerParser<FullParseHandler>::finishFunction(
    bool isStandaloneFunction /* = false */) {
  if (!finishFunctionScopes(isStandaloneFunction)) {
    return false;
  }

  FunctionBox* funbox = pc->functionBox();
  bool hasParameterExprs = funbox->hasParameterExprs;

  if (hasParameterExprs) {
    Maybe<VarScope::Data*> bindings = newVarScopeData(pc->varScope());
    if (!bindings) {
      return false;
    }
    funbox->extraVarScopeBindings().set(*bindings);
  }
||||||| merged common ancestors
bool
PerHandlerParser<ParseHandler>::finishFunctionScopes(bool isStandaloneFunction)
{
    FunctionBox* funbox = pc->functionBox();

    if (funbox->hasParameterExprs) {
        if (!propagateFreeNamesAndMarkClosedOverBindings(pc->functionScope())) {
            return false;
        }
    }

    if (funbox->function()->isNamedLambda() && !isStandaloneFunction) {
        if (!propagateFreeNamesAndMarkClosedOverBindings(pc->namedLambdaScope())) {
            return false;
        }
    }

    return true;
}

template <>
bool
PerHandlerParser<FullParseHandler>::finishFunction(bool isStandaloneFunction /* = false */)
{
    if (!finishFunctionScopes(isStandaloneFunction)) {
        return false;
    }

    FunctionBox* funbox = pc->functionBox();
    bool hasParameterExprs = funbox->hasParameterExprs;

    if (hasParameterExprs) {
        Maybe<VarScope::Data*> bindings = newVarScopeData(pc->varScope());
        if (!bindings) {
            return false;
        }
        funbox->extraVarScopeBindings().set(*bindings);
    }
=======
bool PerHandlerParser<ParseHandler>::finishFunctionScopes(
    bool isStandaloneFunction) {
  FunctionBox* funbox = pc_->functionBox();
>>>>>>> upstream-releases

<<<<<<< HEAD
  {
    Maybe<FunctionScope::Data*> bindings =
        newFunctionScopeData(pc->functionScope(), hasParameterExprs);
    if (!bindings) {
      return false;
||||||| merged common ancestors
    {
        Maybe<FunctionScope::Data*> bindings = newFunctionScopeData(pc->functionScope(),
                                                                    hasParameterExprs);
        if (!bindings) {
            return false;
        }
        funbox->functionScopeBindings().set(*bindings);
=======
  if (funbox->hasParameterExprs) {
    if (!propagateFreeNamesAndMarkClosedOverBindings(pc_->functionScope())) {
      return false;
>>>>>>> upstream-releases
    }
<<<<<<< HEAD
    funbox->functionScopeBindings().set(*bindings);
  }
||||||| merged common ancestors
=======
  }
>>>>>>> upstream-releases

<<<<<<< HEAD
  if (funbox->function()->isNamedLambda() && !isStandaloneFunction) {
    Maybe<LexicalScope::Data*> bindings =
        newLexicalScopeData(pc->namedLambdaScope());
    if (!bindings) {
      return false;
||||||| merged common ancestors
    if (funbox->function()->isNamedLambda() && !isStandaloneFunction) {
        Maybe<LexicalScope::Data*> bindings = newLexicalScopeData(pc->namedLambdaScope());
        if (!bindings) {
            return false;
        }
        funbox->namedLambdaBindings().set(*bindings);
=======
  if (funbox->isNamedLambda() && !isStandaloneFunction) {
    if (!propagateFreeNamesAndMarkClosedOverBindings(pc_->namedLambdaScope())) {
      return false;
>>>>>>> upstream-releases
    }
<<<<<<< HEAD
    funbox->namedLambdaBindings().set(*bindings);
  }
||||||| merged common ancestors
=======
  }
>>>>>>> upstream-releases

  return true;
}

template <>
<<<<<<< HEAD
bool PerHandlerParser<SyntaxParseHandler>::finishFunction(
    bool isStandaloneFunction /* = false */) {
  // The LazyScript for a lazily parsed function needs to know its set of
  // free variables and inner functions so that when it is fully parsed, we
  // can skip over any already syntax parsed inner functions and still
  // retain correct scope information.

  if (!finishFunctionScopes(isStandaloneFunction)) {
    return false;
  }

  // There are too many bindings or inner functions to be saved into the
  // LazyScript. Do a full parse.
  if (pc->closedOverBindingsForLazy().length() >=
          LazyScript::NumClosedOverBindingsLimit ||
      pc->innerFunctionsForLazy.length() >=
          LazyScript::NumInnerFunctionsLimit) {
    MOZ_ALWAYS_FALSE(abortIfSyntaxParser());
    return false;
  }

  FunctionBox* funbox = pc->functionBox();
  RootedFunction fun(context, funbox->function());
  LazyScript* lazy = LazyScript::Create(
      context, fun, sourceObject, pc->closedOverBindingsForLazy(),
      pc->innerFunctionsForLazy, funbox->bufStart, funbox->bufEnd,
      funbox->toStringStart, funbox->startLine, funbox->startColumn,
      parseGoal());
  if (!lazy) {
    return false;
  }

  // Flags that need to be copied into the JSScript when we do the full
  // parse.
  if (pc->sc()->strict()) {
    lazy->setStrict();
  }
  lazy->setGeneratorKind(funbox->generatorKind());
  lazy->setAsyncKind(funbox->asyncKind());
  if (funbox->hasRest()) {
    lazy->setHasRest();
  }
  if (funbox->isLikelyConstructorWrapper()) {
    lazy->setLikelyConstructorWrapper();
  }
  if (funbox->isDerivedClassConstructor()) {
    lazy->setIsDerivedClassConstructor();
  }
  if (funbox->needsHomeObject()) {
    lazy->setNeedsHomeObject();
  }
  if (funbox->declaredArguments) {
    lazy->setShouldDeclareArguments();
  }
  if (funbox->hasThisBinding()) {
    lazy->setHasThisBinding();
  }

  // Flags that need to copied back into the parser when we do the full
  // parse.
  PropagateTransitiveParseFlags(funbox, lazy);

  fun->initLazyScript(lazy);
  return true;
}

static YieldHandling GetYieldHandling(GeneratorKind generatorKind) {
  if (generatorKind == GeneratorKind::NotGenerator) {
    return YieldIsName;
  }
  return YieldIsKeyword;
}

static AwaitHandling GetAwaitHandling(FunctionAsyncKind asyncKind) {
  if (asyncKind == FunctionAsyncKind::SyncFunction) {
    return AwaitIsName;
  }
  return AwaitIsKeyword;
}

template <typename Unit>
CodeNode* Parser<FullParseHandler, Unit>::standaloneFunction(
    HandleFunction fun, HandleScope enclosingScope,
    const Maybe<uint32_t>& parameterListEnd, GeneratorKind generatorKind,
    FunctionAsyncKind asyncKind, Directives inheritedDirectives,
    Directives* newDirectives) {
  MOZ_ASSERT(checkOptionsCalled);

  // Skip prelude.
  TokenKind tt;
  if (!tokenStream.getToken(&tt, TokenStream::Operand)) {
    return null();
  }
  if (asyncKind == FunctionAsyncKind::AsyncFunction) {
    MOZ_ASSERT(tt == TokenKind::Async);
    if (!tokenStream.getToken(&tt, TokenStream::Operand)) {
      return null();
    }
  }
  MOZ_ASSERT(tt == TokenKind::Function);

  if (!tokenStream.getToken(&tt)) {
    return null();
  }
  if (generatorKind == GeneratorKind::Generator) {
    MOZ_ASSERT(tt == TokenKind::Mul);
    if (!tokenStream.getToken(&tt)) {
      return null();
    }
  }

  // Skip function name, if present.
  if (TokenKindIsPossibleIdentifierName(tt)) {
    MOZ_ASSERT(anyChars.currentName() == fun->explicitName());
  } else {
    MOZ_ASSERT(fun->explicitName() == nullptr);
    anyChars.ungetToken();
  }

  CodeNodeType funNode = handler.newFunctionStatement(pos());
  if (!funNode) {
    return null();
  }

  ListNodeType argsbody = handler.newList(ParseNodeKind::ParamsBody, pos());
  if (!argsbody) {
    return null();
  }
  funNode->setBody(argsbody);

  FunctionBox* funbox =
      newFunctionBox(funNode, fun, /* toStringStart = */ 0, inheritedDirectives,
                     generatorKind, asyncKind);
  if (!funbox) {
    return null();
  }
  funbox->initStandaloneFunction(enclosingScope);

  SourceParseContext funpc(this, funbox, newDirectives);
  if (!funpc.init()) {
    return null();
  }
  funpc.setIsStandaloneFunctionBody();

  YieldHandling yieldHandling = GetYieldHandling(generatorKind);
  AwaitHandling awaitHandling = GetAwaitHandling(asyncKind);
  AutoAwaitIsKeyword<FullParseHandler, Unit> awaitIsKeyword(this,
                                                            awaitHandling);
  if (!functionFormalParametersAndBody(
          InAllowed, yieldHandling, &funNode, FunctionSyntaxKind::Statement,
          parameterListEnd, /* isStandaloneFunction = */ true)) {
    return null();
  }

  if (!tokenStream.getToken(&tt, TokenStream::Operand)) {
    return null();
  }
  if (tt != TokenKind::Eof) {
    error(JSMSG_GARBAGE_AFTER_INPUT, "function body", TokenKindToDesc(tt));
    return null();
  }

  ParseNode* node = funNode;
  if (!FoldConstants(context, &node, this)) {
    return null();
  }
  funNode = &node->as<CodeNode>();

  if (!this->setSourceMapInfo()) {
    return null();
  }

  return funNode;
}

template <class ParseHandler>
bool PerHandlerParser<ParseHandler>::declareFunctionArgumentsObject() {
  FunctionBox* funbox = pc->functionBox();
  ParseContext::Scope& funScope = pc->functionScope();
  ParseContext::Scope& varScope = pc->varScope();

  bool hasExtraBodyVarScope = &funScope != &varScope;

  // Time to implement the odd semantics of 'arguments'.
  HandlePropertyName argumentsName = context->names().arguments;

  bool tryDeclareArguments;
  if (handler.canSkipLazyClosedOverBindings()) {
    tryDeclareArguments =
        funbox->function()->lazyScript()->shouldDeclareArguments();
  } else {
    tryDeclareArguments = hasUsedFunctionSpecialName(argumentsName);
  }

  // ES 9.2.12 steps 19 and 20 say formal parameters, lexical bindings,
  // and body-level functions named 'arguments' shadow the arguments
  // object.
  //
  // So even if there wasn't a free use of 'arguments' but there is a var
  // binding of 'arguments', we still might need the arguments object.
  //
  // If we have an extra var scope due to parameter expressions and the body
  // declared 'var arguments', we still need to declare 'arguments' in the
  // function scope.
  DeclaredNamePtr p = varScope.lookupDeclaredName(argumentsName);
  if (p && (p->value()->kind() == DeclarationKind::Var ||
            p->value()->kind() == DeclarationKind::ForOfVar)) {
    if (hasExtraBodyVarScope) {
      tryDeclareArguments = true;
    } else {
      funbox->usesArguments = true;
    }
  }

  if (tryDeclareArguments) {
    AddDeclaredNamePtr p = funScope.lookupDeclaredNameForAdd(argumentsName);
    if (!p) {
      if (!funScope.addDeclaredName(pc, p, argumentsName, DeclarationKind::Var,
                                    DeclaredNameInfo::npos)) {
        return false;
      }
      funbox->declaredArguments = true;
      funbox->usesArguments = true;
    } else if (hasExtraBodyVarScope) {
      // Formal parameters shadow the arguments object.
      return true;
    }
  }

  // Compute if we need an arguments object.
  if (funbox->usesArguments) {
    // There is an 'arguments' binding. Is the arguments object definitely
    // needed?
    //
    // Also see the flags' comments in ContextFlags.
    funbox->setArgumentsHasLocalBinding();

    // Dynamic scope access destroys all hope of optimization.
    if (pc->sc()->bindingsAccessedDynamically()) {
      funbox->setDefinitelyNeedsArgsObj();
    }

    // If a script contains the debugger statement either directly or
    // within an inner function, the arguments object should be created
    // eagerly so the Debugger API may observe bindings.
    if (pc->sc()->hasDebuggerStatement()) {
      funbox->setDefinitelyNeedsArgsObj();
    }
  }

  return true;
}

template <class ParseHandler, typename Unit>
typename ParseHandler::LexicalScopeNodeType
GeneralParser<ParseHandler, Unit>::functionBody(InHandling inHandling,
                                                YieldHandling yieldHandling,
                                                FunctionSyntaxKind kind,
                                                FunctionBodyType type) {
  MOZ_ASSERT(pc->isFunctionBox());

#ifdef DEBUG
  uint32_t startYieldOffset = pc->lastYieldOffset;
#endif

  Node body;
  if (type == StatementListBody) {
    bool inheritedStrict = pc->sc()->strict();
    body = statementList(yieldHandling);
    if (!body) {
      return null();
    }

    // When we transitioned from non-strict to strict mode, we need to
    // validate that all parameter names are valid strict mode names.
    if (!inheritedStrict && pc->sc()->strict()) {
      MOZ_ASSERT(pc->sc()->hasExplicitUseStrict(),
                 "strict mode should only change when a 'use strict' directive "
                 "is present");
      if (!hasValidSimpleStrictParameterNames()) {
        // Request that this function be reparsed as strict to report
        // the invalid parameter name at the correct source location.
        pc->newDirectives->setStrict();
        return null();
      }
    }
  } else {
    MOZ_ASSERT(type == ExpressionBody);

    // Async functions are implemented as generators, and generators are
    // assumed to be statement lists, to prepend initial `yield`.
    ListNodeType stmtList = null();
    if (pc->isAsync()) {
      stmtList = handler.newStatementList(pos());
      if (!stmtList) {
        return null();
      }
    }

    Node kid = assignExpr(inHandling, yieldHandling, TripledotProhibited);
    if (!kid) {
      return null();
    }
||||||| merged common ancestors
bool
PerHandlerParser<SyntaxParseHandler>::finishFunction(bool isStandaloneFunction /* = false */)
{
    // The LazyScript for a lazily parsed function needs to know its set of
    // free variables and inner functions so that when it is fully parsed, we
    // can skip over any already syntax parsed inner functions and still
    // retain correct scope information.

    if (!finishFunctionScopes(isStandaloneFunction)) {
        return false;
    }

    // There are too many bindings or inner functions to be saved into the
    // LazyScript. Do a full parse.
    if (pc->closedOverBindingsForLazy().length() >= LazyScript::NumClosedOverBindingsLimit ||
        pc->innerFunctionsForLazy.length() >= LazyScript::NumInnerFunctionsLimit)
    {
        MOZ_ALWAYS_FALSE(abortIfSyntaxParser());
        return false;
    }

    FunctionBox* funbox = pc->functionBox();
    RootedFunction fun(context, funbox->function());
    LazyScript* lazy = LazyScript::Create(context, fun, sourceObject,
                                          pc->closedOverBindingsForLazy(),
                                          pc->innerFunctionsForLazy,
                                          funbox->bufStart, funbox->bufEnd,
                                          funbox->toStringStart,
                                          funbox->startLine, funbox->startColumn,
                                          parseGoal());
    if (!lazy) {
        return false;
    }

    // Flags that need to be copied into the JSScript when we do the full
    // parse.
    if (pc->sc()->strict()) {
        lazy->setStrict();
    }
    lazy->setGeneratorKind(funbox->generatorKind());
    lazy->setAsyncKind(funbox->asyncKind());
    if (funbox->hasRest()) {
        lazy->setHasRest();
    }
    if (funbox->isLikelyConstructorWrapper()) {
        lazy->setLikelyConstructorWrapper();
    }
    if (funbox->isDerivedClassConstructor()) {
        lazy->setIsDerivedClassConstructor();
    }
    if (funbox->needsHomeObject()) {
        lazy->setNeedsHomeObject();
    }
    if (funbox->declaredArguments) {
        lazy->setShouldDeclareArguments();
    }
    if (funbox->hasThisBinding()) {
        lazy->setHasThisBinding();
    }

    // Flags that need to copied back into the parser when we do the full
    // parse.
    PropagateTransitiveParseFlags(funbox, lazy);

    fun->initLazyScript(lazy);
    return true;
}

static YieldHandling
GetYieldHandling(GeneratorKind generatorKind)
{
    if (generatorKind == GeneratorKind::NotGenerator) {
        return YieldIsName;
    }
    return YieldIsKeyword;
}

static AwaitHandling
GetAwaitHandling(FunctionAsyncKind asyncKind)
{
    if (asyncKind == FunctionAsyncKind::SyncFunction) {
        return AwaitIsName;
    }
    return AwaitIsKeyword;
}

template <typename Unit>
CodeNode*
Parser<FullParseHandler, Unit>::standaloneFunction(HandleFunction fun, HandleScope enclosingScope,
                                                   const Maybe<uint32_t>& parameterListEnd,
                                                   GeneratorKind generatorKind,
                                                   FunctionAsyncKind asyncKind,
                                                   Directives inheritedDirectives,
                                                   Directives* newDirectives)
{
    MOZ_ASSERT(checkOptionsCalled);

    // Skip prelude.
    TokenKind tt;
    if (!tokenStream.getToken(&tt, TokenStream::Operand)) {
        return null();
    }
    if (asyncKind == FunctionAsyncKind::AsyncFunction) {
        MOZ_ASSERT(tt == TokenKind::Async);
        if (!tokenStream.getToken(&tt, TokenStream::Operand)) {
            return null();
        }
    }
    MOZ_ASSERT(tt == TokenKind::Function);

    if (!tokenStream.getToken(&tt)) {
        return null();
    }
    if (generatorKind == GeneratorKind::Generator) {
        MOZ_ASSERT(tt == TokenKind::Mul);
        if (!tokenStream.getToken(&tt)) {
            return null();
        }
    }

    // Skip function name, if present.
    if (TokenKindIsPossibleIdentifierName(tt)) {
        MOZ_ASSERT(anyChars.currentName() == fun->explicitName());
    } else {
        MOZ_ASSERT(fun->explicitName() == nullptr);
        anyChars.ungetToken();
    }

    CodeNodeType funNode = handler.newFunctionStatement(pos());
    if (!funNode) {
        return null();
    }

    ListNodeType argsbody = handler.newList(ParseNodeKind::ParamsBody, pos());
    if (!argsbody) {
        return null();
    }
    funNode->setBody(argsbody);

    FunctionBox* funbox = newFunctionBox(funNode, fun, /* toStringStart = */ 0,
                                         inheritedDirectives, generatorKind, asyncKind);
    if (!funbox) {
        return null();
    }
    funbox->initStandaloneFunction(enclosingScope);
=======
bool PerHandlerParser<FullParseHandler>::finishFunction(
    bool isStandaloneFunction /* = false */,
    IsFieldInitializer isFieldInitializer /* = IsFieldInitializer::No */) {
  if (!finishFunctionScopes(isStandaloneFunction)) {
    return false;
  }
>>>>>>> upstream-releases

<<<<<<< HEAD
    body = handler.newExpressionBody(kid);
    if (!body) {
      return null();
    }
||||||| merged common ancestors
    SourceParseContext funpc(this, funbox, newDirectives);
    if (!funpc.init()) {
        return null();
    }
    funpc.setIsStandaloneFunctionBody();
=======
  FunctionBox* funbox = pc_->functionBox();
  bool hasParameterExprs = funbox->hasParameterExprs;
>>>>>>> upstream-releases

<<<<<<< HEAD
    if (pc->isAsync()) {
      handler.addStatementToList(stmtList, body);
      body = stmtList;
||||||| merged common ancestors
    YieldHandling yieldHandling = GetYieldHandling(generatorKind);
    AwaitHandling awaitHandling = GetAwaitHandling(asyncKind);
    AutoAwaitIsKeyword<FullParseHandler, Unit> awaitIsKeyword(this, awaitHandling);
    if (!functionFormalParametersAndBody(InAllowed, yieldHandling, &funNode,
                                         FunctionSyntaxKind::Statement,
                                         parameterListEnd, /* isStandaloneFunction = */ true))
    {
        return null();
=======
  if (hasParameterExprs) {
    Maybe<VarScope::Data*> bindings = newVarScopeData(pc_->varScope());
    if (!bindings) {
      return false;
>>>>>>> upstream-releases
    }
<<<<<<< HEAD
  }
||||||| merged common ancestors
=======
    funbox->extraVarScopeBindings().set(*bindings);
  }
>>>>>>> upstream-releases

<<<<<<< HEAD
  MOZ_ASSERT_IF(!pc->isGenerator() && !pc->isAsync(),
                pc->lastYieldOffset == startYieldOffset);
  MOZ_ASSERT_IF(pc->isGenerator(), kind != FunctionSyntaxKind::Arrow);
  MOZ_ASSERT_IF(pc->isGenerator(), type == StatementListBody);

  if (pc->needsDotGeneratorName()) {
    MOZ_ASSERT_IF(!pc->isAsync(), type == StatementListBody);
    if (!declareDotGeneratorName()) {
      return null();
    }
    NameNodeType generator = newDotGeneratorName();
    if (!generator) {
      return null();
||||||| merged common ancestors
    if (!tokenStream.getToken(&tt, TokenStream::Operand)) {
        return null();
    }
    if (tt != TokenKind::Eof) {
        error(JSMSG_GARBAGE_AFTER_INPUT, "function body", TokenKindToDesc(tt));
        return null();
=======
  {
    Maybe<FunctionScope::Data*> bindings = newFunctionScopeData(
        pc_->functionScope(), hasParameterExprs, isFieldInitializer);
    if (!bindings) {
      return false;
>>>>>>> upstream-releases
    }
<<<<<<< HEAD
    if (!handler.prependInitialYield(handler.asList(body), generator)) {
      return null();
||||||| merged common ancestors

    ParseNode* node = funNode;
    if (!FoldConstants(context, &node, this)) {
        return null();
=======
    funbox->functionScopeBindings().set(*bindings);
  }

  if (funbox->isNamedLambda() && !isStandaloneFunction) {
    Maybe<LexicalScope::Data*> bindings =
        newLexicalScopeData(pc_->namedLambdaScope());
    if (!bindings) {
      return false;
>>>>>>> upstream-releases
    }
<<<<<<< HEAD
  }
||||||| merged common ancestors
    funNode = &node->as<CodeNode>();
=======
    funbox->namedLambdaBindings().set(*bindings);
  }
>>>>>>> upstream-releases

<<<<<<< HEAD
  // Declare the 'arguments' and 'this' bindings if necessary before
  // finishing up the scope so these special bindings get marked as closed
  // over if necessary. Arrow functions don't have these bindings.
  if (kind != FunctionSyntaxKind::Arrow) {
    if (!declareFunctionArgumentsObject()) {
      return null();
    }
    if (!declareFunctionThis()) {
      return null();
    }
  }
||||||| merged common ancestors
    if (!this->setSourceMapInfo()) {
        return null();
    }
=======
  return true;
}
>>>>>>> upstream-releases

<<<<<<< HEAD
  return finishLexicalScope(pc->varScope(), body);
||||||| merged common ancestors
    return funNode;
=======
template <>
bool PerHandlerParser<SyntaxParseHandler>::finishFunction(
    bool isStandaloneFunction /* = false */,
    IsFieldInitializer isFieldInitializer /* = IsFieldInitializer::Yes */) {
  // The LazyScript for a lazily parsed function needs to know its set of
  // free variables and inner functions so that when it is fully parsed, we
  // can skip over any already syntax parsed inner functions and still
  // retain correct scope information.

  if (!finishFunctionScopes(isStandaloneFunction)) {
    return false;
  }

  // There are too many bindings or inner functions to be saved into the
  // LazyScript. Do a full parse.
  if (pc_->closedOverBindingsForLazy().length() >=
          LazyScript::NumClosedOverBindingsLimit ||
      pc_->innerFunctionsForLazy.length() >=
          LazyScript::NumInnerFunctionsLimit) {
    MOZ_ALWAYS_FALSE(abortIfSyntaxParser());
    return false;
  }

  FunctionBox* funbox = pc_->functionBox();
  RootedFunction fun(cx_, funbox->function());
  LazyScript* lazy = LazyScript::Create(
      cx_, fun, sourceObject_, pc_->closedOverBindingsForLazy(),
      pc_->innerFunctionsForLazy, funbox->bufStart, funbox->bufEnd,
      funbox->toStringStart, funbox->startLine, funbox->startColumn,
      parseGoal());
  if (!lazy) {
    return false;
  }

  // Flags that need to be copied into the JSScript when we do the full
  // parse.
  if (pc_->sc()->strict()) {
    lazy->setStrict();
  }
  lazy->setGeneratorKind(funbox->generatorKind());
  lazy->setAsyncKind(funbox->asyncKind());
  if (funbox->hasRest()) {
    lazy->setHasRest();
  }
  if (funbox->isLikelyConstructorWrapper()) {
    lazy->setLikelyConstructorWrapper();
  }
  if (funbox->isDerivedClassConstructor()) {
    lazy->setIsDerivedClassConstructor();
  }
  if (funbox->needsHomeObject()) {
    lazy->setNeedsHomeObject();
  }
  if (funbox->declaredArguments) {
    lazy->setShouldDeclareArguments();
  }
  if (funbox->hasThisBinding()) {
    lazy->setHasThisBinding();
  }

  // Flags that need to copied back into the parser when we do the full
  // parse.
  PropagateTransitiveParseFlags(funbox, lazy);

  fun->initLazyScript(lazy);
  return true;
}

static YieldHandling GetYieldHandling(GeneratorKind generatorKind) {
  if (generatorKind == GeneratorKind::NotGenerator) {
    return YieldIsName;
  }
  return YieldIsKeyword;
}

static AwaitHandling GetAwaitHandling(FunctionAsyncKind asyncKind) {
  if (asyncKind == FunctionAsyncKind::SyncFunction) {
    return AwaitIsName;
  }
  return AwaitIsKeyword;
>>>>>>> upstream-releases
}

<<<<<<< HEAD
JSFunction* AllocNewFunction(JSContext* cx, HandleAtom atom,
                             FunctionSyntaxKind kind,
                             GeneratorKind generatorKind,
                             FunctionAsyncKind asyncKind, HandleObject proto,
                             bool isSelfHosting /* = false */,
                             bool inFunctionBox /* = false */) {
  MOZ_ASSERT_IF(kind == FunctionSyntaxKind::Statement, atom != nullptr);
||||||| merged common ancestors
template <class ParseHandler>
bool
PerHandlerParser<ParseHandler>::declareFunctionArgumentsObject()
{
    FunctionBox* funbox = pc->functionBox();
    ParseContext::Scope& funScope = pc->functionScope();
    ParseContext::Scope& varScope = pc->varScope();

    bool hasExtraBodyVarScope = &funScope != &varScope;
=======
template <typename Unit>
FunctionNode* Parser<FullParseHandler, Unit>::standaloneFunction(
    HandleFunction fun, HandleScope enclosingScope,
    const Maybe<uint32_t>& parameterListEnd, GeneratorKind generatorKind,
    FunctionAsyncKind asyncKind, Directives inheritedDirectives,
    Directives* newDirectives) {
  MOZ_ASSERT(checkOptionsCalled_);

  // Skip prelude.
  TokenKind tt;
  if (!tokenStream.getToken(&tt, TokenStream::SlashIsRegExp)) {
    return null();
  }
  if (asyncKind == FunctionAsyncKind::AsyncFunction) {
    MOZ_ASSERT(tt == TokenKind::Async);
    if (!tokenStream.getToken(&tt, TokenStream::SlashIsRegExp)) {
      return null();
    }
  }
  MOZ_ASSERT(tt == TokenKind::Function);

  if (!tokenStream.getToken(&tt)) {
    return null();
  }
  if (generatorKind == GeneratorKind::Generator) {
    MOZ_ASSERT(tt == TokenKind::Mul);
    if (!tokenStream.getToken(&tt)) {
      return null();
    }
  }
>>>>>>> upstream-releases

<<<<<<< HEAD
  RootedFunction fun(cx);
||||||| merged common ancestors
    // Time to implement the odd semantics of 'arguments'.
    HandlePropertyName argumentsName = context->names().arguments;
=======
  // Skip function name, if present.
  if (TokenKindIsPossibleIdentifierName(tt)) {
    MOZ_ASSERT(anyChars.currentName() == fun->explicitName());
  } else {
    MOZ_ASSERT(fun->explicitName() == nullptr);
    anyChars.ungetToken();
  }
>>>>>>> upstream-releases

<<<<<<< HEAD
  gc::AllocKind allocKind = gc::AllocKind::FUNCTION;
  JSFunction::Flags flags;
#ifdef DEBUG
  bool isGlobalSelfHostedBuiltin = false;
#endif
  switch (kind) {
    case FunctionSyntaxKind::Expression:
      flags = (generatorKind == GeneratorKind::NotGenerator &&
                       asyncKind == FunctionAsyncKind::SyncFunction
                   ? JSFunction::INTERPRETED_LAMBDA
                   : JSFunction::INTERPRETED_LAMBDA_GENERATOR_OR_ASYNC);
      break;
    case FunctionSyntaxKind::Arrow:
      flags = JSFunction::INTERPRETED_LAMBDA_ARROW;
      allocKind = gc::AllocKind::FUNCTION_EXTENDED;
      break;
    case FunctionSyntaxKind::Method:
      flags = (generatorKind == GeneratorKind::NotGenerator &&
                       asyncKind == FunctionAsyncKind::SyncFunction
                   ? JSFunction::INTERPRETED_METHOD
                   : JSFunction::INTERPRETED_METHOD_GENERATOR_OR_ASYNC);
      allocKind = gc::AllocKind::FUNCTION_EXTENDED;
      break;
    case FunctionSyntaxKind::ClassConstructor:
    case FunctionSyntaxKind::DerivedClassConstructor:
      flags = JSFunction::INTERPRETED_CLASS_CONSTRUCTOR;
      allocKind = gc::AllocKind::FUNCTION_EXTENDED;
      break;
    case FunctionSyntaxKind::Getter:
      flags = JSFunction::INTERPRETED_GETTER;
      allocKind = gc::AllocKind::FUNCTION_EXTENDED;
      break;
    case FunctionSyntaxKind::Setter:
      flags = JSFunction::INTERPRETED_SETTER;
      allocKind = gc::AllocKind::FUNCTION_EXTENDED;
      break;
    default:
      MOZ_ASSERT(kind == FunctionSyntaxKind::Statement);
#ifdef DEBUG
      if (isSelfHosting && !inFunctionBox) {
        isGlobalSelfHostedBuiltin = true;
        allocKind = gc::AllocKind::FUNCTION_EXTENDED;
      }
#endif
      flags = (generatorKind == GeneratorKind::NotGenerator &&
                       asyncKind == FunctionAsyncKind::SyncFunction
                   ? JSFunction::INTERPRETED_NORMAL
                   : JSFunction::INTERPRETED_GENERATOR_OR_ASYNC);
  }

  // We store the async wrapper in a slot for later access.
  if (asyncKind == FunctionAsyncKind::AsyncFunction) {
    allocKind = gc::AllocKind::FUNCTION_EXTENDED;
  }

  fun = NewFunctionWithProto(cx, nullptr, 0, flags, nullptr, atom, proto,
                             allocKind, TenuredObject);
  if (!fun) {
    return nullptr;
  }
  if (isSelfHosting) {
    fun->setIsSelfHostedBuiltin();
#ifdef DEBUG
    if (isGlobalSelfHostedBuiltin) {
      fun->setExtendedSlot(HAS_SELFHOSTED_CANONICAL_NAME_SLOT,
                           BooleanValue(false));
    }
#endif
  }
  return fun;
}
||||||| merged common ancestors
    bool tryDeclareArguments;
    if (handler.canSkipLazyClosedOverBindings()) {
        tryDeclareArguments = funbox->function()->lazyScript()->shouldDeclareArguments();
    } else {
        tryDeclareArguments = hasUsedFunctionSpecialName(argumentsName);
    }
=======
  FunctionSyntaxKind syntaxKind = FunctionSyntaxKind::Statement;
  FunctionNodeType funNode = handler_.newFunction(syntaxKind, pos());
  if (!funNode) {
    return null();
  }
>>>>>>> upstream-releases

<<<<<<< HEAD
JSFunction* ParserBase::newFunction(HandleAtom atom, FunctionSyntaxKind kind,
                                    GeneratorKind generatorKind,
                                    FunctionAsyncKind asyncKind,
                                    HandleObject proto) {
  return AllocNewFunction(context, atom, kind, generatorKind, asyncKind, proto,
                          options().selfHostingMode, pc->isFunctionBox());
}
||||||| merged common ancestors
    // ES 9.2.12 steps 19 and 20 say formal parameters, lexical bindings,
    // and body-level functions named 'arguments' shadow the arguments
    // object.
    //
    // So even if there wasn't a free use of 'arguments' but there is a var
    // binding of 'arguments', we still might need the arguments object.
    //
    // If we have an extra var scope due to parameter expressions and the body
    // declared 'var arguments', we still need to declare 'arguments' in the
    // function scope.
    DeclaredNamePtr p = varScope.lookupDeclaredName(argumentsName);
    if (p && (p->value()->kind() == DeclarationKind::Var ||
              p->value()->kind() == DeclarationKind::ForOfVar))
    {
        if (hasExtraBodyVarScope) {
            tryDeclareArguments = true;
        } else {
            funbox->usesArguments = true;
        }
    }
=======
  ListNodeType argsbody = handler_.newList(ParseNodeKind::ParamsBody, pos());
  if (!argsbody) {
    return null();
  }
  funNode->setBody(argsbody);
>>>>>>> upstream-releases

<<<<<<< HEAD
template <class ParseHandler, typename Unit>
bool GeneralParser<ParseHandler, Unit>::matchOrInsertSemicolon() {
  TokenKind tt = TokenKind::Eof;
  if (!tokenStream.peekTokenSameLine(&tt, TokenStream::Operand)) {
    return false;
  }
  if (tt != TokenKind::Eof && tt != TokenKind::Eol && tt != TokenKind::Semi &&
      tt != TokenKind::RightCurly) {
    /*
     * When current token is `await` and it's outside of async function,
     * it's possibly intended to be an await expression.
     *
     *   await f();
     *        ^
     *        |
     *        tried to insert semicolon here
     *
     * Detect this situation and throw an understandable error.  Otherwise
     * we'd throw a confusing "unexpected token: (unexpected token)" error.
     */
    if (!pc->isAsync() && anyChars.currentToken().type == TokenKind::Await) {
      error(JSMSG_AWAIT_OUTSIDE_ASYNC);
      return false;
    }
    if (!yieldExpressionsSupported() &&
        anyChars.currentToken().type == TokenKind::Yield) {
      error(JSMSG_YIELD_OUTSIDE_GENERATOR);
      return false;
||||||| merged common ancestors
    if (tryDeclareArguments) {
        AddDeclaredNamePtr p = funScope.lookupDeclaredNameForAdd(argumentsName);
        if (!p) {
            if (!funScope.addDeclaredName(pc, p, argumentsName, DeclarationKind::Var,
                                          DeclaredNameInfo::npos))
            {
                return false;
            }
            funbox->declaredArguments = true;
            funbox->usesArguments = true;
        } else if (hasExtraBodyVarScope) {
            // Formal parameters shadow the arguments object.
            return true;
        }
    }

    // Compute if we need an arguments object.
    if (funbox->usesArguments) {
        // There is an 'arguments' binding. Is the arguments object definitely
        // needed?
        //
        // Also see the flags' comments in ContextFlags.
        funbox->setArgumentsHasLocalBinding();

        // Dynamic scope access destroys all hope of optimization.
        if (pc->sc()->bindingsAccessedDynamically()) {
            funbox->setDefinitelyNeedsArgsObj();
        }

        // If a script contains the debugger statement either directly or
        // within an inner function, the arguments object should be created
        // eagerly so the Debugger API may observe bindings.
        if (pc->sc()->hasDebuggerStatement()) {
            funbox->setDefinitelyNeedsArgsObj();
        }
=======
  FunctionBox* funbox =
      newFunctionBox(funNode, fun, /* toStringStart = */ 0, inheritedDirectives,
                     generatorKind, asyncKind);
  if (!funbox) {
    return null();
  }
  funbox->initStandaloneFunction(enclosingScope);

  SourceParseContext funpc(this, funbox, newDirectives);
  if (!funpc.init()) {
    return null();
  }
  funpc.setIsStandaloneFunctionBody();

  YieldHandling yieldHandling = GetYieldHandling(generatorKind);
  AwaitHandling awaitHandling = GetAwaitHandling(asyncKind);
  AutoAwaitIsKeyword<FullParseHandler, Unit> awaitIsKeyword(this,
                                                            awaitHandling);
  if (!functionFormalParametersAndBody(InAllowed, yieldHandling, &funNode,
                                       syntaxKind, parameterListEnd,
                                       /* isStandaloneFunction = */ true)) {
    return null();
  }

  if (!tokenStream.getToken(&tt, TokenStream::SlashIsRegExp)) {
    return null();
  }
  if (tt != TokenKind::Eof) {
    error(JSMSG_GARBAGE_AFTER_INPUT, "function body", TokenKindToDesc(tt));
    return null();
  }

  if (!CheckParseTree(cx_, alloc_, funNode)) {
    return null();
  }

  ParseNode* node = funNode;
  // Don't constant-fold inside "use asm" code, as this could create a parse
  // tree that doesn't type-check as asm.js.
  if (!pc_->useAsmOrInsideUseAsm()) {
    if (!FoldConstants(cx_, &node, &handler_)) {
      return null();
>>>>>>> upstream-releases
    }
  }
  funNode = &node->as<FunctionNode>();

<<<<<<< HEAD
    /* Advance the scanner for proper error location reporting. */
    tokenStream.consumeKnownToken(tt, TokenStream::Operand);
    error(JSMSG_UNEXPECTED_TOKEN_NO_EXPECT, TokenKindToDesc(tt));
    return false;
  }
  bool matched;
  return tokenStream.matchToken(&matched, TokenKind::Semi,
                                TokenStream::Operand);
||||||| merged common ancestors
    return true;
=======
  if (!this->setSourceMapInfo()) {
    return null();
  }

  return funNode;
>>>>>>> upstream-releases
}

<<<<<<< HEAD
bool ParserBase::leaveInnerFunction(ParseContext* outerpc) {
  MOZ_ASSERT(pc != outerpc);
||||||| merged common ancestors
template <class ParseHandler, typename Unit>
typename ParseHandler::LexicalScopeNodeType
GeneralParser<ParseHandler, Unit>::functionBody(InHandling inHandling, YieldHandling yieldHandling,
                                                FunctionSyntaxKind kind, FunctionBodyType type)
{
    MOZ_ASSERT(pc->isFunctionBox());
=======
template <class ParseHandler, typename Unit>
typename ParseHandler::LexicalScopeNodeType
GeneralParser<ParseHandler, Unit>::functionBody(InHandling inHandling,
                                                YieldHandling yieldHandling,
                                                FunctionSyntaxKind kind,
                                                FunctionBodyType type) {
  MOZ_ASSERT(pc_->isFunctionBox());
>>>>>>> upstream-releases

<<<<<<< HEAD
  // If the current function allows super.property but cannot have a home
  // object, i.e., it is an arrow function, we need to propagate the flag to
  // the outer ParseContext.
  if (pc->superScopeNeedsHomeObject()) {
    if (!pc->isArrowFunction()) {
      MOZ_ASSERT(pc->functionBox()->needsHomeObject());
    } else {
      outerpc->setSuperScopeNeedsHomeObject();
    }
  }

  // Lazy functions inner to another lazy function need to be remembered by
  // the inner function so that if the outer function is eventually parsed
  // we do not need any further parsing or processing of the inner function.
  //
  // Append the inner function here unconditionally; the vector is only used
  // if the Parser using outerpc is a syntax parsing. See
  // GeneralParser<SyntaxParseHandler>::finishFunction.
  if (!outerpc->innerFunctionsForLazy.append(pc->functionBox()->function())) {
    return false;
  }

  PropagateTransitiveParseFlags(pc->functionBox(), outerpc->sc());
||||||| merged common ancestors
#ifdef DEBUG
    uint32_t startYieldOffset = pc->lastYieldOffset;
#endif

    Node body;
    if (type == StatementListBody) {
        bool inheritedStrict = pc->sc()->strict();
        body = statementList(yieldHandling);
        if (!body) {
            return null();
        }

        // When we transitioned from non-strict to strict mode, we need to
        // validate that all parameter names are valid strict mode names.
        if (!inheritedStrict && pc->sc()->strict()) {
            MOZ_ASSERT(pc->sc()->hasExplicitUseStrict(),
                       "strict mode should only change when a 'use strict' directive is present");
            if (!hasValidSimpleStrictParameterNames()) {
                // Request that this function be reparsed as strict to report
                // the invalid parameter name at the correct source location.
                pc->newDirectives->setStrict();
                return null();
            }
        }
    } else {
        MOZ_ASSERT(type == ExpressionBody);

        // Async functions are implemented as generators, and generators are
        // assumed to be statement lists, to prepend initial `yield`.
        ListNodeType stmtList = null();
        if (pc->isAsync()) {
            stmtList = handler.newStatementList(pos());
            if (!stmtList) {
                return null();
            }
        }

        Node kid = assignExpr(inHandling, yieldHandling, TripledotProhibited);
        if (!kid) {
            return null();
        }

        body = handler.newExpressionBody(kid);
        if (!body) {
            return null();
        }
=======
#ifdef DEBUG
  uint32_t startYieldOffset = pc_->lastYieldOffset;
#endif

  // One might expect noteUsedName(".initializers") here when parsing a
  // constructor. See GeneralParser<ParseHandler, Unit>::classDefinition on why
  // it's not here.
>>>>>>> upstream-releases

<<<<<<< HEAD
  return true;
}
||||||| merged common ancestors
        if (pc->isAsync()) {
            handler.addStatementToList(stmtList, body);
            body = stmtList;
        }
    }
=======
  Node body;
  if (type == StatementListBody) {
    bool inheritedStrict = pc_->sc()->strict();
    body = statementList(yieldHandling);
    if (!body) {
      return null();
    }
>>>>>>> upstream-releases

<<<<<<< HEAD
JSAtom* ParserBase::prefixAccessorName(PropertyType propType,
                                       HandleAtom propAtom) {
  RootedAtom prefix(context);
  if (propType == PropertyType::Setter) {
    prefix = context->names().setPrefix;
  } else {
    MOZ_ASSERT(propType == PropertyType::Getter);
    prefix = context->names().getPrefix;
  }

  RootedString str(context, ConcatStrings<CanGC>(context, prefix, propAtom));
  if (!str) {
    return nullptr;
  }

  return AtomizeString(context, str);
}

template <class ParseHandler, typename Unit>
bool GeneralParser<ParseHandler, Unit>::functionArguments(
    YieldHandling yieldHandling, FunctionSyntaxKind kind,
    CodeNodeType funNode) {
  FunctionBox* funbox = pc->functionBox();

  bool parenFreeArrow = false;
  // Modifier for the following tokens.
  // TokenStream::None for the following cases:
  //   async a => 1
  //         ^
  //
  //   (a) => 1
  //   ^
  //
  //   async (a) => 1
  //         ^
  //
  //   function f(a) {}
  //             ^
  //
  // TokenStream::Operand for the following case:
  //   a => 1
  //   ^
  Modifier firstTokenModifier = TokenStream::None;

  // Modifier for the the first token in each argument.
  // can be changed to TokenStream::None for the following case:
  //   async a => 1
  //         ^
  Modifier argModifier = TokenStream::Operand;
  if (kind == FunctionSyntaxKind::Arrow) {
    TokenKind tt;
    // In async function, the first token after `async` is already gotten
    // with TokenStream::None.
    // In sync function, the first token is already gotten with
    // TokenStream::Operand.
    firstTokenModifier =
        funbox->isAsync() ? TokenStream::None : TokenStream::Operand;
    if (!tokenStream.peekToken(&tt, firstTokenModifier)) {
      return false;
||||||| merged common ancestors
    MOZ_ASSERT_IF(!pc->isGenerator() && !pc->isAsync(), pc->lastYieldOffset == startYieldOffset);
    MOZ_ASSERT_IF(pc->isGenerator(), kind != FunctionSyntaxKind::Arrow);
    MOZ_ASSERT_IF(pc->isGenerator(), type == StatementListBody);

    if (pc->needsDotGeneratorName()) {
        MOZ_ASSERT_IF(!pc->isAsync(), type == StatementListBody);
        if (!declareDotGeneratorName()) {
            return null();
        }
        NameNodeType generator = newDotGeneratorName();
        if (!generator) {
            return null();
        }
        if (!handler.prependInitialYield(handler.asList(body), generator)) {
            return null();
        }
=======
    // When we transitioned from non-strict to strict mode, we need to
    // validate that all parameter names are valid strict mode names.
    if (!inheritedStrict && pc_->sc()->strict()) {
      MOZ_ASSERT(pc_->sc()->hasExplicitUseStrict(),
                 "strict mode should only change when a 'use strict' directive "
                 "is present");
      if (!hasValidSimpleStrictParameterNames()) {
        // Request that this function be reparsed as strict to report
        // the invalid parameter name at the correct source location.
        pc_->newDirectives->setStrict();
        return null();
      }
>>>>>>> upstream-releases
    }
<<<<<<< HEAD
    if (TokenKindIsPossibleIdentifier(tt)) {
      parenFreeArrow = true;
      argModifier = firstTokenModifier;
||||||| merged common ancestors

    // Declare the 'arguments' and 'this' bindings if necessary before
    // finishing up the scope so these special bindings get marked as closed
    // over if necessary. Arrow functions don't have these bindings.
    if (kind != FunctionSyntaxKind::Arrow) {
        if (!declareFunctionArgumentsObject()) {
            return null();
        }
        if (!declareFunctionThis()) {
            return null();
        }
=======
  } else {
    MOZ_ASSERT(type == ExpressionBody);

    // Async functions are implemented as generators, and generators are
    // assumed to be statement lists, to prepend initial `yield`.
    ListNodeType stmtList = null();
    if (pc_->isAsync()) {
      stmtList = handler_.newStatementList(pos());
      if (!stmtList) {
        return null();
      }
>>>>>>> upstream-releases
    }
  }

<<<<<<< HEAD
  TokenPos firstTokenPos;
  if (!parenFreeArrow) {
    TokenKind tt;
    if (!tokenStream.getToken(&tt, firstTokenModifier)) {
      return false;
    }
    if (tt != TokenKind::LeftParen) {
      error(kind == FunctionSyntaxKind::Arrow ? JSMSG_BAD_ARROW_ARGS
                                              : JSMSG_PAREN_BEFORE_FORMAL);
      return false;
    }

    firstTokenPos = pos();
||||||| merged common ancestors
    return finishLexicalScope(pc->varScope(), body);
}

JSFunction*
AllocNewFunction(JSContext* cx, HandleAtom atom, FunctionSyntaxKind kind, GeneratorKind generatorKind,
                 FunctionAsyncKind asyncKind, HandleObject proto,
                 bool isSelfHosting /* = false */, bool inFunctionBox /* = false */)
{
    MOZ_ASSERT_IF(kind == FunctionSyntaxKind::Statement, atom != nullptr);

    RootedFunction fun(cx);
=======
    Node kid = assignExpr(inHandling, yieldHandling, TripledotProhibited);
    if (!kid) {
      return null();
    }
>>>>>>> upstream-releases

<<<<<<< HEAD
    // Record the start of function source (for FunctionToString). If we
    // are parenFreeArrow, we will set this below, after consuming the NAME.
    funbox->setStart(anyChars);
  } else {
    // When delazifying, we may not have a current token and pos() is
    // garbage. In that case, substitute the first token's position.
    if (!tokenStream.peekTokenPos(&firstTokenPos, firstTokenModifier)) {
      return false;
||||||| merged common ancestors
    gc::AllocKind allocKind = gc::AllocKind::FUNCTION;
    JSFunction::Flags flags;
#ifdef DEBUG
    bool isGlobalSelfHostedBuiltin = false;
#endif
    switch (kind) {
      case FunctionSyntaxKind::Expression:
        flags = (generatorKind == GeneratorKind::NotGenerator &&
                 asyncKind == FunctionAsyncKind::SyncFunction
                 ? JSFunction::INTERPRETED_LAMBDA
                 : JSFunction::INTERPRETED_LAMBDA_GENERATOR_OR_ASYNC);
        break;
      case FunctionSyntaxKind::Arrow:
        flags = JSFunction::INTERPRETED_LAMBDA_ARROW;
        allocKind = gc::AllocKind::FUNCTION_EXTENDED;
        break;
      case FunctionSyntaxKind::Method:
        flags = (generatorKind == GeneratorKind::NotGenerator &&
                 asyncKind == FunctionAsyncKind::SyncFunction
                 ? JSFunction::INTERPRETED_METHOD
                 : JSFunction::INTERPRETED_METHOD_GENERATOR_OR_ASYNC);
        allocKind = gc::AllocKind::FUNCTION_EXTENDED;
        break;
      case FunctionSyntaxKind::ClassConstructor:
      case FunctionSyntaxKind::DerivedClassConstructor:
        flags = JSFunction::INTERPRETED_CLASS_CONSTRUCTOR;
        allocKind = gc::AllocKind::FUNCTION_EXTENDED;
        break;
      case FunctionSyntaxKind::Getter:
        flags = JSFunction::INTERPRETED_GETTER;
        allocKind = gc::AllocKind::FUNCTION_EXTENDED;
        break;
      case FunctionSyntaxKind::Setter:
        flags = JSFunction::INTERPRETED_SETTER;
        allocKind = gc::AllocKind::FUNCTION_EXTENDED;
        break;
      default:
        MOZ_ASSERT(kind == FunctionSyntaxKind::Statement);
#ifdef DEBUG
        if (isSelfHosting && !inFunctionBox) {
            isGlobalSelfHostedBuiltin = true;
            allocKind = gc::AllocKind::FUNCTION_EXTENDED;
        }
#endif
        flags = (generatorKind == GeneratorKind::NotGenerator &&
                 asyncKind == FunctionAsyncKind::SyncFunction
                 ? JSFunction::INTERPRETED_NORMAL
                 : JSFunction::INTERPRETED_GENERATOR_OR_ASYNC);
=======
    body = handler_.newExpressionBody(kid);
    if (!body) {
      return null();
>>>>>>> upstream-releases
    }
  }

<<<<<<< HEAD
  ListNodeType argsbody =
      handler.newList(ParseNodeKind::ParamsBody, firstTokenPos);
  if (!argsbody) {
    return false;
  }
  handler.setFunctionFormalParametersAndBody(funNode, argsbody);
||||||| merged common ancestors
    // We store the async wrapper in a slot for later access.
    if (asyncKind == FunctionAsyncKind::AsyncFunction) {
        allocKind = gc::AllocKind::FUNCTION_EXTENDED;
    }
=======
    if (pc_->isAsync()) {
      handler_.addStatementToList(stmtList, body);
      body = stmtList;
    }
  }
>>>>>>> upstream-releases

<<<<<<< HEAD
  bool hasArguments = false;
  if (parenFreeArrow) {
    hasArguments = true;
  } else {
    bool matched;
    if (!tokenStream.matchToken(&matched, TokenKind::RightParen,
                                TokenStream::Operand)) {
      return false;
||||||| merged common ancestors
    fun = NewFunctionWithProto(cx, nullptr, 0, flags, nullptr, atom, proto,
                               allocKind, TenuredObject);
    if (!fun) {
        return nullptr;
=======
  MOZ_ASSERT_IF(!pc_->isGenerator() && !pc_->isAsync(),
                pc_->lastYieldOffset == startYieldOffset);
  MOZ_ASSERT_IF(pc_->isGenerator(), kind != FunctionSyntaxKind::Arrow);
  MOZ_ASSERT_IF(pc_->isGenerator(), type == StatementListBody);

  if (pc_->needsDotGeneratorName()) {
    MOZ_ASSERT_IF(!pc_->isAsync(), type == StatementListBody);
    if (!pc_->declareDotGeneratorName()) {
      return null();
>>>>>>> upstream-releases
    }
<<<<<<< HEAD
    if (!matched) {
      hasArguments = true;
||||||| merged common ancestors
    if (isSelfHosting) {
        fun->setIsSelfHostedBuiltin();
#ifdef DEBUG
        if (isGlobalSelfHostedBuiltin) {
            fun->setExtendedSlot(HAS_SELFHOSTED_CANONICAL_NAME_SLOT, BooleanValue(false));
        }
#endif
=======
    if (pc_->isGenerator()) {
      NameNodeType generator = newDotGeneratorName();
      if (!generator) {
        return null();
      }
      if (!handler_.prependInitialYield(handler_.asList(body), generator)) {
        return null();
      }
>>>>>>> upstream-releases
    }
<<<<<<< HEAD
  }
  if (hasArguments) {
    bool hasRest = false;
    bool hasDefault = false;
    bool duplicatedParam = false;
    bool disallowDuplicateParams = kind == FunctionSyntaxKind::Arrow ||
                                   kind == FunctionSyntaxKind::Method ||
                                   kind == FunctionSyntaxKind::ClassConstructor;
    AtomVector& positionalFormals = pc->positionalFormalParameterNames();
||||||| merged common ancestors
    return fun;
}

JSFunction*
ParserBase::newFunction(HandleAtom atom, FunctionSyntaxKind kind,
                        GeneratorKind generatorKind, FunctionAsyncKind asyncKind,
                        HandleObject proto)
{
    return AllocNewFunction(context, atom, kind, generatorKind, asyncKind, proto,
                            options().selfHostingMode, pc->isFunctionBox());
}
=======
  }

  // Declare the 'arguments' and 'this' bindings if necessary before
  // finishing up the scope so these special bindings get marked as closed
  // over if necessary. Arrow functions don't have these bindings.
  if (kind != FunctionSyntaxKind::Arrow) {
    bool canSkipLazyClosedOverBindings =
        handler_.canSkipLazyClosedOverBindings();
    if (!pc_->declareFunctionArgumentsObject(usedNames_,
                                             canSkipLazyClosedOverBindings)) {
      return null();
    }
    if (!pc_->declareFunctionThis(usedNames_, canSkipLazyClosedOverBindings)) {
      return null();
    }
  }

  return finishLexicalScope(pc_->varScope(), body);
}

JSFunction* AllocNewFunction(JSContext* cx, HandleAtom atom,
                             FunctionSyntaxKind kind,
                             GeneratorKind generatorKind,
                             FunctionAsyncKind asyncKind, HandleObject proto,
                             bool isSelfHosting /* = false */,
                             bool inFunctionBox /* = false */) {
  MOZ_ASSERT_IF(kind == FunctionSyntaxKind::Statement, atom != nullptr);

  RootedFunction fun(cx);

  gc::AllocKind allocKind = gc::AllocKind::FUNCTION;
  JSFunction::Flags flags;
  bool isExtendedUnclonedSelfHostedFunctionName =
      isSelfHosting && atom && IsExtendedUnclonedSelfHostedFunctionName(atom);
  MOZ_ASSERT_IF(isExtendedUnclonedSelfHostedFunctionName, !inFunctionBox);

  switch (kind) {
    case FunctionSyntaxKind::Expression:
      flags = (generatorKind == GeneratorKind::NotGenerator &&
                       asyncKind == FunctionAsyncKind::SyncFunction
                   ? JSFunction::INTERPRETED_LAMBDA
                   : JSFunction::INTERPRETED_LAMBDA_GENERATOR_OR_ASYNC);
      break;
    case FunctionSyntaxKind::Arrow:
      flags = JSFunction::INTERPRETED_LAMBDA_ARROW;
      allocKind = gc::AllocKind::FUNCTION_EXTENDED;
      break;
    case FunctionSyntaxKind::Method:
      flags = JSFunction::INTERPRETED_METHOD;
      allocKind = gc::AllocKind::FUNCTION_EXTENDED;
      break;
    case FunctionSyntaxKind::ClassConstructor:
    case FunctionSyntaxKind::DerivedClassConstructor:
      flags = JSFunction::INTERPRETED_CLASS_CONSTRUCTOR;
      allocKind = gc::AllocKind::FUNCTION_EXTENDED;
      break;
    case FunctionSyntaxKind::Getter:
      flags = JSFunction::INTERPRETED_GETTER;
      allocKind = gc::AllocKind::FUNCTION_EXTENDED;
      break;
    case FunctionSyntaxKind::Setter:
      flags = JSFunction::INTERPRETED_SETTER;
      allocKind = gc::AllocKind::FUNCTION_EXTENDED;
      break;
    default:
      MOZ_ASSERT(kind == FunctionSyntaxKind::Statement);
      if (isExtendedUnclonedSelfHostedFunctionName) {
        allocKind = gc::AllocKind::FUNCTION_EXTENDED;
      }
      flags = (generatorKind == GeneratorKind::NotGenerator &&
                       asyncKind == FunctionAsyncKind::SyncFunction
                   ? JSFunction::INTERPRETED_NORMAL
                   : JSFunction::INTERPRETED_GENERATOR_OR_ASYNC);
  }

  fun = NewFunctionWithProto(cx, nullptr, 0, flags, nullptr, atom, proto,
                             allocKind, TenuredObject);
  if (!fun) {
    return nullptr;
  }
  if (isSelfHosting) {
    fun->setIsSelfHostedBuiltin();
  }
  return fun;
}

JSFunction* ParserBase::newFunction(HandleAtom atom, FunctionSyntaxKind kind,
                                    GeneratorKind generatorKind,
                                    FunctionAsyncKind asyncKind,
                                    HandleObject proto /* = nullptr */) {
  return AllocNewFunction(cx_, atom, kind, generatorKind, asyncKind, proto,
                          options().selfHostingMode, pc_->isFunctionBox());
}
>>>>>>> upstream-releases

<<<<<<< HEAD
    if (kind == FunctionSyntaxKind::Getter) {
      error(JSMSG_ACCESSOR_WRONG_ARGS, "getter", "no", "s");
      return false;
||||||| merged common ancestors
template <class ParseHandler, typename Unit>
bool
GeneralParser<ParseHandler, Unit>::matchOrInsertSemicolon()
{
    TokenKind tt = TokenKind::Eof;
    if (!tokenStream.peekTokenSameLine(&tt, TokenStream::Operand)) {
        return false;
=======
template <class ParseHandler, typename Unit>
bool GeneralParser<ParseHandler, Unit>::matchOrInsertSemicolon(
    Modifier modifier /* = TokenStream::SlashIsRegExp */) {
  TokenKind tt = TokenKind::Eof;
  if (!tokenStream.peekTokenSameLine(&tt, modifier)) {
    return false;
  }
  if (tt != TokenKind::Eof && tt != TokenKind::Eol && tt != TokenKind::Semi &&
      tt != TokenKind::RightCurly) {
    /*
     * When current token is `await` and it's outside of async function,
     * it's possibly intended to be an await expression.
     *
     *   await f();
     *        ^
     *        |
     *        tried to insert semicolon here
     *
     * Detect this situation and throw an understandable error.  Otherwise
     * we'd throw a confusing "unexpected token: (unexpected token)" error.
     */
    if (!pc_->isAsync() && anyChars.currentToken().type == TokenKind::Await) {
      error(JSMSG_AWAIT_OUTSIDE_ASYNC);
      return false;
>>>>>>> upstream-releases
    }
<<<<<<< HEAD

    while (true) {
      if (hasRest) {
        error(JSMSG_PARAMETER_AFTER_REST);
        return false;
      }
||||||| merged common ancestors
    if (tt != TokenKind::Eof &&
        tt != TokenKind::Eol &&
        tt != TokenKind::Semi &&
        tt != TokenKind::RightCurly)
    {
        /*
         * When current token is `await` and it's outside of async function,
         * it's possibly intended to be an await expression.
         *
         *   await f();
         *        ^
         *        |
         *        tried to insert semicolon here
         *
         * Detect this situation and throw an understandable error.  Otherwise
         * we'd throw a confusing "unexpected token: (unexpected token)" error.
         */
        if (!pc->isAsync() && anyChars.currentToken().type == TokenKind::Await) {
            error(JSMSG_AWAIT_OUTSIDE_ASYNC);
            return false;
        }
        if (!yieldExpressionsSupported() && anyChars.currentToken().type == TokenKind::Yield) {
            error(JSMSG_YIELD_OUTSIDE_GENERATOR);
            return false;
        }

        /* Advance the scanner for proper error location reporting. */
        tokenStream.consumeKnownToken(tt, TokenStream::Operand);
        error(JSMSG_UNEXPECTED_TOKEN_NO_EXPECT, TokenKindToDesc(tt));
        return false;
    }
    bool matched;
    return tokenStream.matchToken(&matched, TokenKind::Semi, TokenStream::Operand);
}
=======
    if (!yieldExpressionsSupported() &&
        anyChars.currentToken().type == TokenKind::Yield) {
      error(JSMSG_YIELD_OUTSIDE_GENERATOR);
      return false;
    }

    /* Advance the scanner for proper error location reporting. */
    tokenStream.consumeKnownToken(tt, modifier);
    error(JSMSG_UNEXPECTED_TOKEN_NO_EXPECT, TokenKindToDesc(tt));
    return false;
  }
  bool matched;
  return tokenStream.matchToken(&matched, TokenKind::Semi, modifier);
}
>>>>>>> upstream-releases

<<<<<<< HEAD
      TokenKind tt;
      if (!tokenStream.getToken(&tt, argModifier)) {
        return false;
      }
      argModifier = TokenStream::Operand;
      MOZ_ASSERT_IF(parenFreeArrow, TokenKindIsPossibleIdentifier(tt));
||||||| merged common ancestors
bool
ParserBase::leaveInnerFunction(ParseContext* outerpc)
{
    MOZ_ASSERT(pc != outerpc);
=======
bool ParserBase::leaveInnerFunction(ParseContext* outerpc) {
  MOZ_ASSERT(pc_ != outerpc);
>>>>>>> upstream-releases

<<<<<<< HEAD
      if (tt == TokenKind::TripleDot) {
        if (kind == FunctionSyntaxKind::Setter) {
          error(JSMSG_ACCESSOR_WRONG_ARGS, "setter", "one", "");
          return false;
        }
||||||| merged common ancestors
    // If the current function allows super.property but cannot have a home
    // object, i.e., it is an arrow function, we need to propagate the flag to
    // the outer ParseContext.
    if (pc->superScopeNeedsHomeObject()) {
        if (!pc->isArrowFunction()) {
            MOZ_ASSERT(pc->functionBox()->needsHomeObject());
        } else {
            outerpc->setSuperScopeNeedsHomeObject();
        }
    }

    // Lazy functions inner to another lazy function need to be remembered by
    // the inner function so that if the outer function is eventually parsed
    // we do not need any further parsing or processing of the inner function.
    //
    // Append the inner function here unconditionally; the vector is only used
    // if the Parser using outerpc is a syntax parsing. See
    // GeneralParser<SyntaxParseHandler>::finishFunction.
    if (!outerpc->innerFunctionsForLazy.append(pc->functionBox()->function())) {
        return false;
    }
=======
  // If the current function allows super.property but cannot have a home
  // object, i.e., it is an arrow function, we need to propagate the flag to
  // the outer ParseContext.
  if (pc_->superScopeNeedsHomeObject()) {
    if (!pc_->isArrowFunction()) {
      MOZ_ASSERT(pc_->functionBox()->needsHomeObject());
    } else {
      outerpc->setSuperScopeNeedsHomeObject();
    }
  }

  // Lazy functions inner to another lazy function need to be remembered by
  // the inner function so that if the outer function is eventually parsed
  // we do not need any further parsing or processing of the inner function.
  //
  // Append the inner function here unconditionally; the vector is only used
  // if the Parser using outerpc is a syntax parsing. See
  // GeneralParser<SyntaxParseHandler>::finishFunction.
  if (!outerpc->innerFunctionsForLazy.append(pc_->functionBox()->function())) {
    return false;
  }

  PropagateTransitiveParseFlags(pc_->functionBox(), outerpc->sc());

  return true;
}

JSAtom* ParserBase::prefixAccessorName(PropertyType propType,
                                       HandleAtom propAtom) {
  RootedAtom prefix(cx_);
  if (propType == PropertyType::Setter) {
    prefix = cx_->names().setPrefix;
  } else {
    MOZ_ASSERT(propType == PropertyType::Getter);
    prefix = cx_->names().getPrefix;
  }

  RootedString str(cx_, ConcatStrings<CanGC>(cx_, prefix, propAtom));
  if (!str) {
    return nullptr;
  }

  return AtomizeString(cx_, str);
}

template <class ParseHandler, typename Unit>
void GeneralParser<ParseHandler, Unit>::setFunctionStartAtCurrentToken(
    FunctionBox* funbox) const {
  uint32_t bufStart = anyChars.currentToken().pos.begin;

  uint32_t startLine, startColumn;
  tokenStream.computeLineAndColumn(bufStart, &startLine, &startColumn);

  funbox->setStart(bufStart, startLine, startColumn);
}

template <class ParseHandler, typename Unit>
bool GeneralParser<ParseHandler, Unit>::functionArguments(
    YieldHandling yieldHandling, FunctionSyntaxKind kind,
    FunctionNodeType funNode) {
  FunctionBox* funbox = pc_->functionBox();

  bool parenFreeArrow = false;
  // Modifier for the following tokens.
  // TokenStream::SlashIsDiv for the following cases:
  //   async a => 1
  //         ^
  //
  //   (a) => 1
  //   ^
  //
  //   async (a) => 1
  //         ^
  //
  //   function f(a) {}
  //             ^
  //
  // TokenStream::SlashIsRegExp for the following case:
  //   a => 1
  //   ^
  Modifier firstTokenModifier = TokenStream::SlashIsDiv;

  // Modifier for the the first token in each argument.
  // can be changed to TokenStream::SlashIsDiv for the following case:
  //   async a => 1
  //         ^
  Modifier argModifier = TokenStream::SlashIsRegExp;
  if (kind == FunctionSyntaxKind::Arrow) {
    TokenKind tt;
    // In async function, the first token after `async` is already gotten
    // with TokenStream::SlashIsDiv.
    // In sync function, the first token is already gotten with
    // TokenStream::SlashIsRegExp.
    firstTokenModifier = funbox->isAsync() ? TokenStream::SlashIsDiv
                                           : TokenStream::SlashIsRegExp;
    if (!tokenStream.peekToken(&tt, firstTokenModifier)) {
      return false;
    }
    if (TokenKindIsPossibleIdentifier(tt)) {
      parenFreeArrow = true;
      argModifier = firstTokenModifier;
    }
  }

  TokenPos firstTokenPos;
  if (!parenFreeArrow) {
    TokenKind tt;
    if (!tokenStream.getToken(&tt, firstTokenModifier)) {
      return false;
    }
    if (tt != TokenKind::LeftParen) {
      error(kind == FunctionSyntaxKind::Arrow ? JSMSG_BAD_ARROW_ARGS
                                              : JSMSG_PAREN_BEFORE_FORMAL);
      return false;
    }
>>>>>>> upstream-releases

<<<<<<< HEAD
        disallowDuplicateParams = true;
        if (duplicatedParam) {
          // Has duplicated args before the rest parameter.
          error(JSMSG_BAD_DUP_ARGS);
          return false;
        }
||||||| merged common ancestors
    PropagateTransitiveParseFlags(pc->functionBox(), outerpc->sc());
=======
    firstTokenPos = pos();
>>>>>>> upstream-releases

<<<<<<< HEAD
        hasRest = true;
        funbox->setHasRest();
||||||| merged common ancestors
    return true;
}
=======
    // Record the start of function source (for FunctionToString). If we
    // are parenFreeArrow, we will set this below, after consuming the NAME.
    setFunctionStartAtCurrentToken(funbox);
  } else {
    // When delazifying, we may not have a current token and pos() is
    // garbage. In that case, substitute the first token's position.
    if (!tokenStream.peekTokenPos(&firstTokenPos, firstTokenModifier)) {
      return false;
    }
  }
>>>>>>> upstream-releases

<<<<<<< HEAD
        if (!tokenStream.getToken(&tt)) {
          return false;
        }
||||||| merged common ancestors
JSAtom*
ParserBase::prefixAccessorName(PropertyType propType, HandleAtom propAtom)
{
    RootedAtom prefix(context);
    if (propType == PropertyType::Setter) {
        prefix = context->names().setPrefix;
    } else {
        MOZ_ASSERT(propType == PropertyType::Getter);
        prefix = context->names().getPrefix;
    }
=======
  ListNodeType argsbody =
      handler_.newList(ParseNodeKind::ParamsBody, firstTokenPos);
  if (!argsbody) {
    return false;
  }
  handler_.setFunctionFormalParametersAndBody(funNode, argsbody);

  bool hasArguments = false;
  if (parenFreeArrow) {
    hasArguments = true;
  } else {
    bool matched;
    if (!tokenStream.matchToken(&matched, TokenKind::RightParen,
                                TokenStream::SlashIsRegExp)) {
      return false;
    }
    if (!matched) {
      hasArguments = true;
    }
  }
  if (hasArguments) {
    bool hasRest = false;
    bool hasDefault = false;
    bool duplicatedParam = false;
    bool disallowDuplicateParams = kind == FunctionSyntaxKind::Arrow ||
                                   kind == FunctionSyntaxKind::Method ||
                                   kind == FunctionSyntaxKind::ClassConstructor;
    AtomVector& positionalFormals = pc_->positionalFormalParameterNames();
>>>>>>> upstream-releases

<<<<<<< HEAD
        if (!TokenKindIsPossibleIdentifier(tt) &&
            tt != TokenKind::LeftBracket && tt != TokenKind::LeftCurly) {
          error(JSMSG_NO_REST_NAME);
          return false;
        }
      }
||||||| merged common ancestors
    RootedString str(context, ConcatStrings<CanGC>(context, prefix, propAtom));
    if (!str) {
        return nullptr;
    }
=======
    if (kind == FunctionSyntaxKind::Getter) {
      error(JSMSG_ACCESSOR_WRONG_ARGS, "getter", "no", "s");
      return false;
    }
>>>>>>> upstream-releases

<<<<<<< HEAD
      switch (tt) {
        case TokenKind::LeftBracket:
        case TokenKind::LeftCurly: {
          disallowDuplicateParams = true;
          if (duplicatedParam) {
            // Has duplicated args before the destructuring parameter.
            error(JSMSG_BAD_DUP_ARGS);
            return false;
          }
||||||| merged common ancestors
    return AtomizeString(context, str);
}
=======
    while (true) {
      if (hasRest) {
        error(JSMSG_PARAMETER_AFTER_REST);
        return false;
      }
>>>>>>> upstream-releases

<<<<<<< HEAD
          funbox->hasDestructuringArgs = true;
||||||| merged common ancestors
template <class ParseHandler, typename Unit>
bool
GeneralParser<ParseHandler, Unit>::functionArguments(YieldHandling yieldHandling,
                                                     FunctionSyntaxKind kind, CodeNodeType funNode)
{
    FunctionBox* funbox = pc->functionBox();
=======
      TokenKind tt;
      if (!tokenStream.getToken(&tt, argModifier)) {
        return false;
      }
      argModifier = TokenStream::SlashIsRegExp;
      MOZ_ASSERT_IF(parenFreeArrow, TokenKindIsPossibleIdentifier(tt));
>>>>>>> upstream-releases

<<<<<<< HEAD
          Node destruct = destructuringDeclarationWithoutYieldOrAwait(
              DeclarationKind::FormalParameter, yieldHandling, tt);
          if (!destruct) {
            return false;
          }
||||||| merged common ancestors
    bool parenFreeArrow = false;
    // Modifier for the following tokens.
    // TokenStream::None for the following cases:
    //   async a => 1
    //         ^
    //
    //   (a) => 1
    //   ^
    //
    //   async (a) => 1
    //         ^
    //
    //   function f(a) {}
    //             ^
    //
    // TokenStream::Operand for the following case:
    //   a => 1
    //   ^
    Modifier firstTokenModifier = TokenStream::None;

    // Modifier for the the first token in each argument.
    // can be changed to TokenStream::None for the following case:
    //   async a => 1
    //         ^
    Modifier argModifier = TokenStream::Operand;
    if (kind == FunctionSyntaxKind::Arrow) {
        TokenKind tt;
        // In async function, the first token after `async` is already gotten
        // with TokenStream::None.
        // In sync function, the first token is already gotten with
        // TokenStream::Operand.
        firstTokenModifier = funbox->isAsync() ? TokenStream::None : TokenStream::Operand;
        if (!tokenStream.peekToken(&tt, firstTokenModifier)) {
            return false;
        }
        if (TokenKindIsPossibleIdentifier(tt)) {
            parenFreeArrow = true;
            argModifier = firstTokenModifier;
        }
    }
=======
      if (tt == TokenKind::TripleDot) {
        if (kind == FunctionSyntaxKind::Setter) {
          error(JSMSG_ACCESSOR_WRONG_ARGS, "setter", "one", "");
          return false;
        }
>>>>>>> upstream-releases

<<<<<<< HEAD
          if (!noteDestructuredPositionalFormalParameter(funNode, destruct)) {
            return false;
          }
||||||| merged common ancestors
    TokenPos firstTokenPos;
    if (!parenFreeArrow) {
        TokenKind tt;
        if (!tokenStream.getToken(&tt, firstTokenModifier)) {
            return false;
        }
        if (tt != TokenKind::LeftParen) {
            error(kind == FunctionSyntaxKind::Arrow
                  ? JSMSG_BAD_ARROW_ARGS
                  : JSMSG_PAREN_BEFORE_FORMAL);
            return false;
        }
=======
        disallowDuplicateParams = true;
        if (duplicatedParam) {
          // Has duplicated args before the rest parameter.
          error(JSMSG_BAD_DUP_ARGS);
          return false;
        }

        hasRest = true;
        funbox->setHasRest();

        if (!tokenStream.getToken(&tt)) {
          return false;
        }
>>>>>>> upstream-releases

<<<<<<< HEAD
          break;
        }
||||||| merged common ancestors
        firstTokenPos = pos();
=======
        if (!TokenKindIsPossibleIdentifier(tt) &&
            tt != TokenKind::LeftBracket && tt != TokenKind::LeftCurly) {
          error(JSMSG_NO_REST_NAME);
          return false;
        }
      }
>>>>>>> upstream-releases

<<<<<<< HEAD
        default: {
          if (!TokenKindIsPossibleIdentifier(tt)) {
            error(JSMSG_MISSING_FORMAL);
||||||| merged common ancestors
        // Record the start of function source (for FunctionToString). If we
        // are parenFreeArrow, we will set this below, after consuming the NAME.
        funbox->setStart(anyChars);
    } else {
        // When delazifying, we may not have a current token and pos() is
        // garbage. In that case, substitute the first token's position.
        if (!tokenStream.peekTokenPos(&firstTokenPos, firstTokenModifier)) {
=======
      switch (tt) {
        case TokenKind::LeftBracket:
        case TokenKind::LeftCurly: {
          disallowDuplicateParams = true;
          if (duplicatedParam) {
            // Has duplicated args before the destructuring parameter.
            error(JSMSG_BAD_DUP_ARGS);
>>>>>>> upstream-releases
            return false;
          }

<<<<<<< HEAD
          if (parenFreeArrow) {
            funbox->setStart(anyChars);
          }
||||||| merged common ancestors
    ListNodeType argsbody = handler.newList(ParseNodeKind::ParamsBody, firstTokenPos);
    if (!argsbody) {
        return false;
    }
    handler.setFunctionFormalParametersAndBody(funNode, argsbody);
=======
          funbox->hasDestructuringArgs = true;
>>>>>>> upstream-releases

<<<<<<< HEAD
          RootedPropertyName name(context, bindingIdentifier(yieldHandling));
          if (!name) {
||||||| merged common ancestors
    bool hasArguments = false;
    if (parenFreeArrow) {
        hasArguments = true;
    } else {
        bool matched;
        if (!tokenStream.matchToken(&matched, TokenKind::RightParen, TokenStream::Operand)) {
=======
          Node destruct = destructuringDeclarationWithoutYieldOrAwait(
              DeclarationKind::FormalParameter, yieldHandling, tt);
          if (!destruct) {
>>>>>>> upstream-releases
            return false;
<<<<<<< HEAD
          }

          if (!notePositionalFormalParameter(funNode, name, pos().begin,
                                             disallowDuplicateParams,
                                             &duplicatedParam)) {
||||||| merged common ancestors
        }
        if (!matched) {
            hasArguments = true;
        }
    }
    if (hasArguments) {
        bool hasRest = false;
        bool hasDefault = false;
        bool duplicatedParam = false;
        bool disallowDuplicateParams = kind == FunctionSyntaxKind::Arrow ||
                                       kind == FunctionSyntaxKind::Method ||
                                       kind == FunctionSyntaxKind::ClassConstructor;
        AtomVector& positionalFormals = pc->positionalFormalParameterNames();

        if (kind == FunctionSyntaxKind::Getter) {
            error(JSMSG_ACCESSOR_WRONG_ARGS, "getter", "no", "s");
=======
          }

          if (!noteDestructuredPositionalFormalParameter(funNode, destruct)) {
>>>>>>> upstream-releases
            return false;
<<<<<<< HEAD
          }
          if (duplicatedParam) {
            funbox->hasDuplicateParameters = true;
          }

          break;
||||||| merged common ancestors
=======
          }

          break;
>>>>>>> upstream-releases
        }
      }

<<<<<<< HEAD
      if (positionalFormals.length() >= ARGNO_LIMIT) {
        error(JSMSG_TOO_MANY_FUN_ARGS);
        return false;
      }
||||||| merged common ancestors
        while (true) {
            if (hasRest) {
                error(JSMSG_PARAMETER_AFTER_REST);
                return false;
            }
=======
        default: {
          if (!TokenKindIsPossibleIdentifier(tt)) {
            error(JSMSG_MISSING_FORMAL);
            return false;
          }
>>>>>>> upstream-releases

<<<<<<< HEAD
      // The next step is to detect arguments with default expressions,
      // e.g. |function parseInt(str, radix = 10) {}|.  But if we have a
      // parentheses-free arrow function, |a => ...|, the '=' necessary
      // for a default expression would really be an assignment operator:
      // that is, |a = b => 42;| would parse as |a = (b => 42);|.  So we
      // should stop parsing arguments here.
      if (parenFreeArrow) {
        break;
      }
||||||| merged common ancestors
            TokenKind tt;
            if (!tokenStream.getToken(&tt, argModifier)) {
                return false;
            }
            argModifier = TokenStream::Operand;
            MOZ_ASSERT_IF(parenFreeArrow, TokenKindIsPossibleIdentifier(tt));

            if (tt == TokenKind::TripleDot) {
                if (kind == FunctionSyntaxKind::Setter) {
                    error(JSMSG_ACCESSOR_WRONG_ARGS, "setter", "one", "");
                    return false;
                }

                disallowDuplicateParams = true;
                if (duplicatedParam) {
                    // Has duplicated args before the rest parameter.
                    error(JSMSG_BAD_DUP_ARGS);
                    return false;
                }

                hasRest = true;
                funbox->setHasRest();

                if (!tokenStream.getToken(&tt)) {
                    return false;
                }

                if (!TokenKindIsPossibleIdentifier(tt) &&
                    tt != TokenKind::LeftBracket &&
                    tt != TokenKind::LeftCurly)
                {
                    error(JSMSG_NO_REST_NAME);
                    return false;
                }
            }
=======
          if (parenFreeArrow) {
            setFunctionStartAtCurrentToken(funbox);
          }
>>>>>>> upstream-releases

<<<<<<< HEAD
      bool matched;
      if (!tokenStream.matchToken(&matched, TokenKind::Assign,
                                  TokenStream::Operand)) {
        return false;
      }
      if (matched) {
        // A default argument without parentheses would look like:
        // a = expr => body, but both operators are right-associative, so
        // that would have been parsed as a = (expr => body) instead.
        // Therefore it's impossible to get here with parenFreeArrow.
        MOZ_ASSERT(!parenFreeArrow);
||||||| merged common ancestors
            switch (tt) {
              case TokenKind::LeftBracket:
              case TokenKind::LeftCurly: {
                disallowDuplicateParams = true;
                if (duplicatedParam) {
                    // Has duplicated args before the destructuring parameter.
                    error(JSMSG_BAD_DUP_ARGS);
                    return false;
                }

                funbox->hasDestructuringArgs = true;

                Node destruct = destructuringDeclarationWithoutYieldOrAwait(
                    DeclarationKind::FormalParameter,
                    yieldHandling, tt);
                if (!destruct) {
                    return false;
                }

                if (!noteDestructuredPositionalFormalParameter(funNode, destruct)) {
                    return false;
                }

                break;
              }

              default: {
                if (!TokenKindIsPossibleIdentifier(tt)) {
                    error(JSMSG_MISSING_FORMAL);
                    return false;
                }

                if (parenFreeArrow) {
                    funbox->setStart(anyChars);
                }

                RootedPropertyName name(context, bindingIdentifier(yieldHandling));
                if (!name) {
                    return false;
                }

                if (!notePositionalFormalParameter(funNode, name, pos().begin,
                                                   disallowDuplicateParams, &duplicatedParam))
                {
                    return false;
                }
                if (duplicatedParam) {
                    funbox->hasDuplicateParameters = true;
                }

                break;
              }
            }
=======
          RootedPropertyName name(cx_, bindingIdentifier(yieldHandling));
          if (!name) {
            return false;
          }
>>>>>>> upstream-releases

<<<<<<< HEAD
        if (hasRest) {
          error(JSMSG_REST_WITH_DEFAULT);
          return false;
        }
        disallowDuplicateParams = true;
        if (duplicatedParam) {
          error(JSMSG_BAD_DUP_ARGS);
          return false;
        }
||||||| merged common ancestors
            if (positionalFormals.length() >= ARGNO_LIMIT) {
                error(JSMSG_TOO_MANY_FUN_ARGS);
                return false;
            }
=======
          if (!notePositionalFormalParameter(funNode, name, pos().begin,
                                             disallowDuplicateParams,
                                             &duplicatedParam)) {
            return false;
          }
          if (duplicatedParam) {
            funbox->hasDuplicateParameters = true;
          }
>>>>>>> upstream-releases

<<<<<<< HEAD
        if (!hasDefault) {
          hasDefault = true;
||||||| merged common ancestors
            // The next step is to detect arguments with default expressions,
            // e.g. |function parseInt(str, radix = 10) {}|.  But if we have a
            // parentheses-free arrow function, |a => ...|, the '=' necessary
            // for a default expression would really be an assignment operator:
            // that is, |a = b => 42;| would parse as |a = (b => 42);|.  So we
            // should stop parsing arguments here.
            if (parenFreeArrow) {
                break;
            }
=======
          break;
        }
      }
>>>>>>> upstream-releases

<<<<<<< HEAD
          // The Function.length property is the number of formals
          // before the first default argument.
          funbox->length = positionalFormals.length() - 1;
        }
        funbox->hasParameterExprs = true;
||||||| merged common ancestors
            bool matched;
            if (!tokenStream.matchToken(&matched, TokenKind::Assign, TokenStream::Operand)) {
                return false;
            }
            if (matched) {
                // A default argument without parentheses would look like:
                // a = expr => body, but both operators are right-associative, so
                // that would have been parsed as a = (expr => body) instead.
                // Therefore it's impossible to get here with parenFreeArrow.
                MOZ_ASSERT(!parenFreeArrow);

                if (hasRest) {
                    error(JSMSG_REST_WITH_DEFAULT);
                    return false;
                }
                disallowDuplicateParams = true;
                if (duplicatedParam) {
                    error(JSMSG_BAD_DUP_ARGS);
                    return false;
                }

                if (!hasDefault) {
                    hasDefault = true;

                    // The Function.length property is the number of formals
                    // before the first default argument.
                    funbox->length = positionalFormals.length() - 1;
                }
                funbox->hasParameterExprs = true;

                Node def_expr = assignExprWithoutYieldOrAwait(yieldHandling);
                if (!def_expr) {
                    return false;
                }
                if (!handler.setLastFunctionFormalParameterDefault(funNode, def_expr)) {
                    return false;
                }
            }
=======
      if (positionalFormals.length() >= ARGNO_LIMIT) {
        error(JSMSG_TOO_MANY_FUN_ARGS);
        return false;
      }
>>>>>>> upstream-releases

<<<<<<< HEAD
        Node def_expr = assignExprWithoutYieldOrAwait(yieldHandling);
        if (!def_expr) {
          return false;
        }
        if (!handler.setLastFunctionFormalParameterDefault(funNode, def_expr)) {
          return false;
        }
      }
||||||| merged common ancestors
            // Setter syntax uniquely requires exactly one argument.
            if (kind == FunctionSyntaxKind::Setter) {
                break;
            }
=======
      // The next step is to detect arguments with default expressions,
      // e.g. |function parseInt(str, radix = 10) {}|.  But if we have a
      // parentheses-free arrow function, |a => ...|, the '=' necessary
      // for a default expression would really be an assignment operator:
      // that is, |a = b => 42;| would parse as |a = (b => 42);|.  So we
      // should stop parsing arguments here.
      if (parenFreeArrow) {
        break;
      }
>>>>>>> upstream-releases

<<<<<<< HEAD
      // Setter syntax uniquely requires exactly one argument.
      if (kind == FunctionSyntaxKind::Setter) {
        break;
      }
||||||| merged common ancestors
            if (!tokenStream.matchToken(&matched, TokenKind::Comma, TokenStream::Operand)) {
                return false;
            }
            if (!matched) {
                break;
            }
=======
      bool matched;
      if (!tokenStream.matchToken(&matched, TokenKind::Assign,
                                  TokenStream::SlashIsRegExp)) {
        return false;
      }
      if (matched) {
        // A default argument without parentheses would look like:
        // a = expr => body, but both operators are right-associative, so
        // that would have been parsed as a = (expr => body) instead.
        // Therefore it's impossible to get here with parenFreeArrow.
        MOZ_ASSERT(!parenFreeArrow);
>>>>>>> upstream-releases

<<<<<<< HEAD
      if (!tokenStream.matchToken(&matched, TokenKind::Comma,
                                  TokenStream::Operand)) {
        return false;
      }
      if (!matched) {
        break;
      }

      if (!hasRest) {
        if (!tokenStream.peekToken(&tt, TokenStream::Operand)) {
          return false;
||||||| merged common ancestors
            if (!hasRest) {
                if (!tokenStream.peekToken(&tt, TokenStream::Operand)) {
                    return false;
                }
                if (tt == TokenKind::RightParen) {
                    break;
                }
            }
        }

        if (!parenFreeArrow) {
            TokenKind tt;
            if (!tokenStream.getToken(&tt, TokenStream::Operand)) {
                return false;
            }
            if (tt != TokenKind::RightParen) {
                if (kind == FunctionSyntaxKind::Setter) {
                    error(JSMSG_ACCESSOR_WRONG_ARGS, "setter", "one", "");
                    return false;
                }

                error(JSMSG_PAREN_AFTER_FORMAL);
                return false;
            }
=======
        if (hasRest) {
          error(JSMSG_REST_WITH_DEFAULT);
          return false;
        }
        disallowDuplicateParams = true;
        if (duplicatedParam) {
          error(JSMSG_BAD_DUP_ARGS);
          return false;
>>>>>>> upstream-releases
        }
<<<<<<< HEAD
        if (tt == TokenKind::RightParen) {
          break;
||||||| merged common ancestors

        if (!hasDefault) {
            funbox->length = positionalFormals.length() - hasRest;
=======

        if (!hasDefault) {
          hasDefault = true;

          // The Function.length property is the number of formals
          // before the first default argument.
          funbox->length = positionalFormals.length() - 1;
        }
        funbox->hasParameterExprs = true;

        Node def_expr = assignExprWithoutYieldOrAwait(yieldHandling);
        if (!def_expr) {
          return false;
        }
        if (!handler_.setLastFunctionFormalParameterDefault(funNode,
                                                            def_expr)) {
          return false;
        }
      }

      // Setter syntax uniquely requires exactly one argument.
      if (kind == FunctionSyntaxKind::Setter) {
        break;
      }

      if (!tokenStream.matchToken(&matched, TokenKind::Comma,
                                  TokenStream::SlashIsRegExp)) {
        return false;
      }
      if (!matched) {
        break;
      }

      if (!hasRest) {
        if (!tokenStream.peekToken(&tt, TokenStream::SlashIsRegExp)) {
          return false;
        }
        if (tt == TokenKind::RightParen) {
          break;
>>>>>>> upstream-releases
        }
      }
    }

<<<<<<< HEAD
    if (!parenFreeArrow) {
      TokenKind tt;
      if (!tokenStream.getToken(&tt, TokenStream::Operand)) {
        return false;
      }
      if (tt != TokenKind::RightParen) {
        if (kind == FunctionSyntaxKind::Setter) {
          error(JSMSG_ACCESSOR_WRONG_ARGS, "setter", "one", "");
          return false;
||||||| merged common ancestors
        if (funbox->hasParameterExprs && funbox->hasDirectEval()) {
            funbox->hasDirectEvalInParameterExpr = true;
=======
    if (!parenFreeArrow) {
      TokenKind tt;
      if (!tokenStream.getToken(&tt, TokenStream::SlashIsRegExp)) {
        return false;
      }
      if (tt != TokenKind::RightParen) {
        if (kind == FunctionSyntaxKind::Setter) {
          error(JSMSG_ACCESSOR_WRONG_ARGS, "setter", "one", "");
          return false;
>>>>>>> upstream-releases
        }

        error(JSMSG_PAREN_AFTER_FORMAL);
        return false;
      }
    }

    if (!hasDefault) {
      funbox->length = positionalFormals.length() - hasRest;
    }

    if (funbox->hasParameterExprs && funbox->hasDirectEval()) {
      funbox->hasDirectEvalInParameterExpr = true;
    }

    funbox->function()->setArgCount(positionalFormals.length());
  } else if (kind == FunctionSyntaxKind::Setter) {
    error(JSMSG_ACCESSOR_WRONG_ARGS, "setter", "one", "");
    return false;
  }

  return true;
}

template <typename Unit>
<<<<<<< HEAD
bool Parser<FullParseHandler, Unit>::skipLazyInnerFunction(
    CodeNode* funNode, uint32_t toStringStart, FunctionSyntaxKind kind,
    bool tryAnnexB) {
  // When a lazily-parsed function is called, we only fully parse (and emit)
  // that function, not any of its nested children. The initial syntax-only
  // parse recorded the free variables of nested functions and their extents,
  // so we can skip over them after accounting for their free variables.

  RootedFunction fun(context, handler.nextLazyInnerFunction());
  FunctionBox* funbox = newFunctionBox(funNode, fun, toStringStart,
                                       Directives(/* strict = */ false),
                                       fun->generatorKind(), fun->asyncKind());
  if (!funbox) {
    return false;
  }
||||||| merged common ancestors
bool
Parser<FullParseHandler, Unit>::skipLazyInnerFunction(CodeNode* funNode, uint32_t toStringStart,
                                                      FunctionSyntaxKind kind, bool tryAnnexB)
{
    // When a lazily-parsed function is called, we only fully parse (and emit)
    // that function, not any of its nested children. The initial syntax-only
    // parse recorded the free variables of nested functions and their extents,
    // so we can skip over them after accounting for their free variables.

    RootedFunction fun(context, handler.nextLazyInnerFunction());
    FunctionBox* funbox =
        newFunctionBox(funNode, fun, toStringStart, Directives(/* strict = */ false),
                       fun->generatorKind(), fun->asyncKind());
    if (!funbox) {
        return false;
    }
=======
bool Parser<FullParseHandler, Unit>::skipLazyInnerFunction(
    FunctionNode* funNode, uint32_t toStringStart, FunctionSyntaxKind kind,
    bool tryAnnexB) {
  // When a lazily-parsed function is called, we only fully parse (and emit)
  // that function, not any of its nested children. The initial syntax-only
  // parse recorded the free variables of nested functions and their extents,
  // so we can skip over them after accounting for their free variables.

  RootedFunction fun(cx_, handler_.nextLazyInnerFunction());
  FunctionBox* funbox = newFunctionBox(funNode, fun, toStringStart,
                                       Directives(/* strict = */ false),
                                       fun->generatorKind(), fun->asyncKind());
  if (!funbox) {
    return false;
  }
>>>>>>> upstream-releases

  LazyScript* lazy = fun->lazyScript();
  if (lazy->needsHomeObject()) {
    funbox->setNeedsHomeObject();
  }

<<<<<<< HEAD
  PropagateTransitiveParseFlags(lazy, pc->sc());
||||||| merged common ancestors
    PropagateTransitiveParseFlags(lazy, pc->sc());
=======
  PropagateTransitiveParseFlags(lazy, pc_->sc());
>>>>>>> upstream-releases

  if (!tokenStream.advance(fun->lazyScript()->sourceEnd())) {
    return false;
  }

<<<<<<< HEAD
  // Append possible Annex B function box only upon successfully parsing.
  if (tryAnnexB &&
      !pc->innermostScope()->addPossibleAnnexBFunctionBox(pc, funbox)) {
    return false;
  }
||||||| merged common ancestors
    // Append possible Annex B function box only upon successfully parsing.
    if (tryAnnexB && !pc->innermostScope()->addPossibleAnnexBFunctionBox(pc, funbox)) {
        return false;
    }
=======
  // Append possible Annex B function box only upon successfully parsing.
  if (tryAnnexB &&
      !pc_->innermostScope()->addPossibleAnnexBFunctionBox(pc_, funbox)) {
    return false;
  }
>>>>>>> upstream-releases

  return true;
}

template <typename Unit>
<<<<<<< HEAD
bool Parser<SyntaxParseHandler, Unit>::skipLazyInnerFunction(
    CodeNodeType funNode, uint32_t toStringStart, FunctionSyntaxKind kind,
    bool tryAnnexB) {
  MOZ_CRASH("Cannot skip lazy inner functions when syntax parsing");
||||||| merged common ancestors
bool
Parser<SyntaxParseHandler, Unit>::skipLazyInnerFunction(CodeNodeType funNode,
                                                        uint32_t toStringStart,
                                                        FunctionSyntaxKind kind, bool tryAnnexB)
{
    MOZ_CRASH("Cannot skip lazy inner functions when syntax parsing");
=======
bool Parser<SyntaxParseHandler, Unit>::skipLazyInnerFunction(
    FunctionNodeType funNode, uint32_t toStringStart, FunctionSyntaxKind kind,
    bool tryAnnexB) {
  MOZ_CRASH("Cannot skip lazy inner functions when syntax parsing");
>>>>>>> upstream-releases
}

template <class ParseHandler, typename Unit>
<<<<<<< HEAD
bool GeneralParser<ParseHandler, Unit>::skipLazyInnerFunction(
    CodeNodeType funNode, uint32_t toStringStart, FunctionSyntaxKind kind,
    bool tryAnnexB) {
  return asFinalParser()->skipLazyInnerFunction(funNode, toStringStart, kind,
                                                tryAnnexB);
||||||| merged common ancestors
bool
GeneralParser<ParseHandler, Unit>::skipLazyInnerFunction(CodeNodeType funNode,
                                                         uint32_t toStringStart,
                                                         FunctionSyntaxKind kind, bool tryAnnexB)
{
    return asFinalParser()->skipLazyInnerFunction(funNode, toStringStart, kind, tryAnnexB);
=======
bool GeneralParser<ParseHandler, Unit>::skipLazyInnerFunction(
    FunctionNodeType funNode, uint32_t toStringStart, FunctionSyntaxKind kind,
    bool tryAnnexB) {
  return asFinalParser()->skipLazyInnerFunction(funNode, toStringStart, kind,
                                                tryAnnexB);
>>>>>>> upstream-releases
}

template <class ParseHandler, typename Unit>
<<<<<<< HEAD
bool GeneralParser<ParseHandler, Unit>::addExprAndGetNextTemplStrToken(
    YieldHandling yieldHandling, ListNodeType nodeList, TokenKind* ttp) {
  Node pn = expr(InAllowed, yieldHandling, TripledotProhibited);
  if (!pn) {
    return false;
  }
  handler.addList(nodeList, pn);
||||||| merged common ancestors
bool
GeneralParser<ParseHandler, Unit>::addExprAndGetNextTemplStrToken(YieldHandling yieldHandling,
                                                                  ListNodeType nodeList,
                                                                  TokenKind* ttp)
{
    Node pn = expr(InAllowed, yieldHandling, TripledotProhibited);
    if (!pn) {
        return false;
    }
    handler.addList(nodeList, pn);
=======
bool GeneralParser<ParseHandler, Unit>::addExprAndGetNextTemplStrToken(
    YieldHandling yieldHandling, ListNodeType nodeList, TokenKind* ttp) {
  Node pn = expr(InAllowed, yieldHandling, TripledotProhibited);
  if (!pn) {
    return false;
  }
  handler_.addList(nodeList, pn);
>>>>>>> upstream-releases

<<<<<<< HEAD
  TokenKind tt;
  if (!tokenStream.getToken(&tt, TokenStream::Operand)) {
    return false;
  }
  if (tt != TokenKind::RightCurly) {
    error(JSMSG_TEMPLSTR_UNTERM_EXPR);
    return false;
  }
||||||| merged common ancestors
    TokenKind tt;
    if (!tokenStream.getToken(&tt, TokenStream::Operand)) {
        return false;
    }
    if (tt != TokenKind::RightCurly) {
        error(JSMSG_TEMPLSTR_UNTERM_EXPR);
        return false;
    }
=======
  TokenKind tt;
  if (!tokenStream.getToken(&tt, TokenStream::SlashIsRegExp)) {
    return false;
  }
  if (tt != TokenKind::RightCurly) {
    error(JSMSG_TEMPLSTR_UNTERM_EXPR);
    return false;
  }
>>>>>>> upstream-releases

<<<<<<< HEAD
  return tokenStream.getToken(ttp, TokenStream::TemplateTail);
||||||| merged common ancestors
    return tokenStream.getToken(ttp, TokenStream::TemplateTail);
=======
  return tokenStream.getTemplateToken(ttp);
>>>>>>> upstream-releases
}

template <class ParseHandler, typename Unit>
<<<<<<< HEAD
bool GeneralParser<ParseHandler, Unit>::taggedTemplate(
    YieldHandling yieldHandling, ListNodeType tagArgsList, TokenKind tt) {
  CallSiteNodeType callSiteObjNode = handler.newCallSiteObject(pos().begin);
  if (!callSiteObjNode) {
    return false;
  }
  handler.addList(tagArgsList, callSiteObjNode);
||||||| merged common ancestors
bool
GeneralParser<ParseHandler, Unit>::taggedTemplate(YieldHandling yieldHandling,
                                                  ListNodeType tagArgsList, TokenKind tt)
{
    CallSiteNodeType callSiteObjNode = handler.newCallSiteObject(pos().begin);
    if (!callSiteObjNode) {
        return false;
    }
    handler.addList(tagArgsList, callSiteObjNode);
=======
bool GeneralParser<ParseHandler, Unit>::taggedTemplate(
    YieldHandling yieldHandling, ListNodeType tagArgsList, TokenKind tt) {
  CallSiteNodeType callSiteObjNode = handler_.newCallSiteObject(pos().begin);
  if (!callSiteObjNode) {
    return false;
  }
  handler_.addList(tagArgsList, callSiteObjNode);
>>>>>>> upstream-releases

  while (true) {
    if (!appendToCallSiteObj(callSiteObjNode)) {
      return false;
    }
    if (tt != TokenKind::TemplateHead) {
      break;
    }

    if (!addExprAndGetNextTemplStrToken(yieldHandling, tagArgsList, &tt)) {
      return false;
    }
<<<<<<< HEAD
  }
  handler.setEndPosition(tagArgsList, callSiteObjNode);
  return true;
||||||| merged common ancestors
    handler.setEndPosition(tagArgsList, callSiteObjNode);
    return true;
=======
  }
  handler_.setEndPosition(tagArgsList, callSiteObjNode);
  return true;
>>>>>>> upstream-releases
}

template <class ParseHandler, typename Unit>
typename ParseHandler::ListNodeType
GeneralParser<ParseHandler, Unit>::templateLiteral(
    YieldHandling yieldHandling) {
  NameNodeType literal = noSubstitutionUntaggedTemplate();
  if (!literal) {
    return null();
  }

<<<<<<< HEAD
  ListNodeType nodeList =
      handler.newList(ParseNodeKind::TemplateStringList, literal);
  if (!nodeList) {
    return null();
  }
||||||| merged common ancestors
    ListNodeType nodeList = handler.newList(ParseNodeKind::TemplateStringList, literal);
    if (!nodeList) {
        return null();
    }
=======
  ListNodeType nodeList =
      handler_.newList(ParseNodeKind::TemplateStringListExpr, literal);
  if (!nodeList) {
    return null();
  }
>>>>>>> upstream-releases

  TokenKind tt;
  do {
    if (!addExprAndGetNextTemplStrToken(yieldHandling, nodeList, &tt)) {
      return null();
    }

    literal = noSubstitutionUntaggedTemplate();
    if (!literal) {
      return null();
    }

<<<<<<< HEAD
    handler.addList(nodeList, literal);
  } while (tt == TokenKind::TemplateHead);
  return nodeList;
||||||| merged common ancestors
        handler.addList(nodeList, literal);
    } while (tt == TokenKind::TemplateHead);
    return nodeList;
=======
    handler_.addList(nodeList, literal);
  } while (tt == TokenKind::TemplateHead);
  return nodeList;
>>>>>>> upstream-releases
}

template <class ParseHandler, typename Unit>
<<<<<<< HEAD
typename ParseHandler::CodeNodeType
GeneralParser<ParseHandler, Unit>::functionDefinition(
    CodeNodeType funNode, uint32_t toStringStart, InHandling inHandling,
    YieldHandling yieldHandling, HandleAtom funName, FunctionSyntaxKind kind,
    GeneratorKind generatorKind, FunctionAsyncKind asyncKind,
    bool tryAnnexB /* = false */) {
  MOZ_ASSERT_IF(kind == FunctionSyntaxKind::Statement, funName);

  // When fully parsing a LazyScript, we do not fully reparse its inner
  // functions, which are also lazy. Instead, their free variables and
  // source extents are recorded and may be skipped.
  if (handler.canSkipLazyInnerFunctions()) {
    if (!skipLazyInnerFunction(funNode, toStringStart, kind, tryAnnexB)) {
      return null();
    }
||||||| merged common ancestors
typename ParseHandler::CodeNodeType
GeneralParser<ParseHandler, Unit>::functionDefinition(CodeNodeType funNode, uint32_t toStringStart,
                                                      InHandling inHandling,
                                                      YieldHandling yieldHandling,
                                                      HandleAtom funName, FunctionSyntaxKind kind,
                                                      GeneratorKind generatorKind,
                                                      FunctionAsyncKind asyncKind,
                                                      bool tryAnnexB /* = false */)
{
    MOZ_ASSERT_IF(kind == FunctionSyntaxKind::Statement, funName);

    // When fully parsing a LazyScript, we do not fully reparse its inner
    // functions, which are also lazy. Instead, their free variables and
    // source extents are recorded and may be skipped.
    if (handler.canSkipLazyInnerFunctions()) {
        if (!skipLazyInnerFunction(funNode, toStringStart, kind, tryAnnexB)) {
            return null();
        }

        return funNode;
    }
=======
typename ParseHandler::FunctionNodeType
GeneralParser<ParseHandler, Unit>::functionDefinition(
    FunctionNodeType funNode, uint32_t toStringStart, InHandling inHandling,
    YieldHandling yieldHandling, HandleAtom funName, FunctionSyntaxKind kind,
    GeneratorKind generatorKind, FunctionAsyncKind asyncKind,
    bool tryAnnexB /* = false */) {
  MOZ_ASSERT_IF(kind == FunctionSyntaxKind::Statement, funName);
>>>>>>> upstream-releases

<<<<<<< HEAD
    return funNode;
  }

  RootedObject proto(context);
  if (generatorKind == GeneratorKind::Generator ||
      asyncKind == FunctionAsyncKind::AsyncFunction) {
    proto = GlobalObject::getOrCreateGeneratorFunctionPrototype(
        context, context->global());
    if (!proto) {
      return null();
    }
  }
  RootedFunction fun(
      context, newFunction(funName, kind, generatorKind, asyncKind, proto));
  if (!fun) {
    return null();
  }
||||||| merged common ancestors
    RootedObject proto(context);
    if (generatorKind == GeneratorKind::Generator ||
        asyncKind == FunctionAsyncKind::AsyncFunction)
    {
        proto = GlobalObject::getOrCreateGeneratorFunctionPrototype(context, context->global());
        if (!proto) {
            return null();
        }
    }
    RootedFunction fun(context, newFunction(funName, kind, generatorKind, asyncKind, proto));
    if (!fun) {
        return null();
    }
=======
  // When fully parsing a LazyScript, we do not fully reparse its inner
  // functions, which are also lazy. Instead, their free variables and
  // source extents are recorded and may be skipped.
  if (handler_.canSkipLazyInnerFunctions()) {
    if (!skipLazyInnerFunction(funNode, toStringStart, kind, tryAnnexB)) {
      return null();
    }
>>>>>>> upstream-releases

<<<<<<< HEAD
  // Speculatively parse using the directives of the parent parsing context.
  // If a directive is encountered (e.g., "use strict") that changes how the
  // function should have been parsed, we backup and reparse with the new set
  // of directives.
  Directives directives(pc);
  Directives newDirectives = directives;

  Position start(keepAtoms, tokenStream);

  // Parse the inner function. The following is a loop as we may attempt to
  // reparse a function due to failed syntax parsing and encountering new
  // "use foo" directives.
  while (true) {
    if (trySyntaxParseInnerFunction(
            &funNode, fun, toStringStart, inHandling, yieldHandling, kind,
            generatorKind, asyncKind, tryAnnexB, directives, &newDirectives)) {
      break;
    }
||||||| merged common ancestors
    // Speculatively parse using the directives of the parent parsing context.
    // If a directive is encountered (e.g., "use strict") that changes how the
    // function should have been parsed, we backup and reparse with the new set
    // of directives.
    Directives directives(pc);
    Directives newDirectives = directives;

    Position start(keepAtoms, tokenStream);

    // Parse the inner function. The following is a loop as we may attempt to
    // reparse a function due to failed syntax parsing and encountering new
    // "use foo" directives.
    while (true) {
        if (trySyntaxParseInnerFunction(&funNode, fun, toStringStart, inHandling, yieldHandling,
                                        kind, generatorKind, asyncKind, tryAnnexB, directives,
                                        &newDirectives))
        {
            break;
        }
=======
    return funNode;
  }
>>>>>>> upstream-releases

<<<<<<< HEAD
    // Return on error.
    if (anyChars.hadError() || directives == newDirectives) {
      return null();
    }
||||||| merged common ancestors
        // Return on error.
        if (anyChars.hadError() || directives == newDirectives) {
            return null();
        }
=======
  RootedObject proto(cx_);
  if (!GetFunctionPrototype(cx_, generatorKind, asyncKind, &proto)) {
    return null();
  }
  RootedFunction fun(
      cx_, newFunction(funName, kind, generatorKind, asyncKind, proto));
  if (!fun) {
    return null();
  }
>>>>>>> upstream-releases

<<<<<<< HEAD
    // Assignment must be monotonic to prevent infinitely attempting to
    // reparse.
    MOZ_ASSERT_IF(directives.strict(), newDirectives.strict());
    MOZ_ASSERT_IF(directives.asmJS(), newDirectives.asmJS());
    directives = newDirectives;
||||||| merged common ancestors
        // Assignment must be monotonic to prevent infinitely attempting to
        // reparse.
        MOZ_ASSERT_IF(directives.strict(), newDirectives.strict());
        MOZ_ASSERT_IF(directives.asmJS(), newDirectives.asmJS());
        directives = newDirectives;
=======
  // Speculatively parse using the directives of the parent parsing context.
  // If a directive is encountered (e.g., "use strict") that changes how the
  // function should have been parsed, we backup and reparse with the new set
  // of directives.
  Directives directives(pc_);
  Directives newDirectives = directives;
>>>>>>> upstream-releases

<<<<<<< HEAD
    tokenStream.seek(start);
||||||| merged common ancestors
        tokenStream.seek(start);
=======
  Position start(keepAtoms_, tokenStream);
>>>>>>> upstream-releases

<<<<<<< HEAD
    // functionFormalParametersAndBody may have already set body before
    // failing.
    handler.setFunctionFormalParametersAndBody(funNode, null());
  }
||||||| merged common ancestors
        // functionFormalParametersAndBody may have already set body before
        // failing.
        handler.setFunctionFormalParametersAndBody(funNode, null());
    }
=======
  // Parse the inner function. The following is a loop as we may attempt to
  // reparse a function due to failed syntax parsing and encountering new
  // "use foo" directives.
  while (true) {
    if (trySyntaxParseInnerFunction(
            &funNode, fun, toStringStart, inHandling, yieldHandling, kind,
            generatorKind, asyncKind, tryAnnexB, directives, &newDirectives)) {
      break;
    }
>>>>>>> upstream-releases

<<<<<<< HEAD
  return funNode;
}

template <typename Unit>
bool Parser<FullParseHandler, Unit>::trySyntaxParseInnerFunction(
    CodeNode** funNode, HandleFunction fun, uint32_t toStringStart,
    InHandling inHandling, YieldHandling yieldHandling, FunctionSyntaxKind kind,
    GeneratorKind generatorKind, FunctionAsyncKind asyncKind, bool tryAnnexB,
    Directives inheritedDirectives, Directives* newDirectives) {
  // Try a syntax parse for this inner function.
  do {
    // If we're assuming this function is an IIFE, always perform a full
    // parse to avoid the overhead of a lazy syntax-only parse. Although
    // the prediction may be incorrect, IIFEs are common enough that it
    // pays off for lots of code.
    if ((*funNode)->isLikelyIIFE() &&
        generatorKind == GeneratorKind::NotGenerator &&
        asyncKind == FunctionAsyncKind::SyncFunction) {
      break;
    }

    SyntaxParser* syntaxParser = getSyntaxParser();
    if (!syntaxParser) {
      break;
    }

    UsedNameTracker::RewindToken token = usedNames.getRewindToken();

    // Move the syntax parser to the current position in the stream.
    Position currentPosition(keepAtoms, tokenStream);
    if (!syntaxParser->tokenStream.seek(currentPosition, anyChars)) {
      return false;
    }

    // Make a FunctionBox before we enter the syntax parser, because |pn|
    // still expects a FunctionBox to be attached to it during BCE, and
    // the syntax parser cannot attach one to it.
    FunctionBox* funbox =
        newFunctionBox(*funNode, fun, toStringStart, inheritedDirectives,
                       generatorKind, asyncKind);
    if (!funbox) {
      return false;
    }
    funbox->initWithEnclosingParseContext(pc, kind);

    SyntaxParseHandler::Node syntaxNode =
        syntaxParser->innerFunctionForFunctionBox(
            SyntaxParseHandler::NodeGeneric, pc, funbox, inHandling,
            yieldHandling, kind, newDirectives);
    if (!syntaxNode) {
      if (syntaxParser->hadAbortedSyntaxParse()) {
        // Try again with a full parse. UsedNameTracker needs to be
        // rewound to just before we tried the syntax parse for
        // correctness.
        syntaxParser->clearAbortedSyntaxParse();
        usedNames.rewind(token);
        MOZ_ASSERT_IF(!syntaxParser->context->helperThread(),
                      !syntaxParser->context->isExceptionPending());
        break;
      }
      return false;
    }
||||||| merged common ancestors
    return funNode;
}

template <typename Unit>
bool
Parser<FullParseHandler, Unit>::trySyntaxParseInnerFunction(CodeNode** funNode, HandleFunction fun,
                                                            uint32_t toStringStart,
                                                            InHandling inHandling,
                                                            YieldHandling yieldHandling,
                                                            FunctionSyntaxKind kind,
                                                            GeneratorKind generatorKind,
                                                            FunctionAsyncKind asyncKind,
                                                            bool tryAnnexB,
                                                            Directives inheritedDirectives,
                                                            Directives* newDirectives)
{
    // Try a syntax parse for this inner function.
    do {
        // If we're assuming this function is an IIFE, always perform a full
        // parse to avoid the overhead of a lazy syntax-only parse. Although
        // the prediction may be incorrect, IIFEs are common enough that it
        // pays off for lots of code.
        if ((*funNode)->isLikelyIIFE() &&
            generatorKind == GeneratorKind::NotGenerator &&
            asyncKind == FunctionAsyncKind::SyncFunction)
        {
            break;
        }

        SyntaxParser* syntaxParser = getSyntaxParser();
        if (!syntaxParser) {
            break;
        }

        UsedNameTracker::RewindToken token = usedNames.getRewindToken();

        // Move the syntax parser to the current position in the stream.
        Position currentPosition(keepAtoms, tokenStream);
        if (!syntaxParser->tokenStream.seek(currentPosition, anyChars)) {
            return false;
        }

        // Make a FunctionBox before we enter the syntax parser, because |pn|
        // still expects a FunctionBox to be attached to it during BCE, and
        // the syntax parser cannot attach one to it.
        FunctionBox* funbox = newFunctionBox(*funNode, fun, toStringStart, inheritedDirectives,
                                             generatorKind, asyncKind);
        if (!funbox) {
            return false;
        }
        funbox->initWithEnclosingParseContext(pc, kind);

        SyntaxParseHandler::Node syntaxNode =
            syntaxParser->innerFunctionForFunctionBox(SyntaxParseHandler::NodeGeneric, pc, funbox,
                                                      inHandling, yieldHandling, kind,
                                                      newDirectives);
        if (!syntaxNode) {
            if (syntaxParser->hadAbortedSyntaxParse()) {
                // Try again with a full parse. UsedNameTracker needs to be
                // rewound to just before we tried the syntax parse for
                // correctness.
                syntaxParser->clearAbortedSyntaxParse();
                usedNames.rewind(token);
                MOZ_ASSERT_IF(!syntaxParser->context->helperThread(),
                              !syntaxParser->context->isExceptionPending());
                break;
            }
            return false;
        }

        // Advance this parser over tokens processed by the syntax parser.
        Position currentSyntaxPosition(keepAtoms, syntaxParser->tokenStream);
        if (!tokenStream.seek(currentSyntaxPosition, syntaxParser->anyChars)) {
            return false;
        }

        // Update the end position of the parse node.
        (*funNode)->pn_pos.end = anyChars.currentToken().pos.end;
=======
    // Return on error.
    if (anyChars.hadError() || directives == newDirectives) {
      return null();
    }

    // Assignment must be monotonic to prevent infinitely attempting to
    // reparse.
    MOZ_ASSERT_IF(directives.strict(), newDirectives.strict());
    MOZ_ASSERT_IF(directives.asmJS(), newDirectives.asmJS());
    directives = newDirectives;

    tokenStream.seek(start);

    // functionFormalParametersAndBody may have already set body before
    // failing.
    handler_.setFunctionFormalParametersAndBody(funNode, null());
  }

  return funNode;
}

template <typename Unit>
bool Parser<FullParseHandler, Unit>::trySyntaxParseInnerFunction(
    FunctionNode** funNode, HandleFunction fun, uint32_t toStringStart,
    InHandling inHandling, YieldHandling yieldHandling, FunctionSyntaxKind kind,
    GeneratorKind generatorKind, FunctionAsyncKind asyncKind, bool tryAnnexB,
    Directives inheritedDirectives, Directives* newDirectives) {
  // Try a syntax parse for this inner function.
  do {
    // If we're assuming this function is an IIFE, always perform a full
    // parse to avoid the overhead of a lazy syntax-only parse. Although
    // the prediction may be incorrect, IIFEs are common enough that it
    // pays off for lots of code.
    if ((*funNode)->isLikelyIIFE() &&
        generatorKind == GeneratorKind::NotGenerator &&
        asyncKind == FunctionAsyncKind::SyncFunction) {
      break;
    }

    SyntaxParser* syntaxParser = getSyntaxParser();
    if (!syntaxParser) {
      break;
    }

    UsedNameTracker::RewindToken token = usedNames_.getRewindToken();

    // Move the syntax parser to the current position in the stream.
    Position currentPosition(keepAtoms_, tokenStream);
    if (!syntaxParser->tokenStream.seek(currentPosition, anyChars)) {
      return false;
    }

    // Make a FunctionBox before we enter the syntax parser, because |pn|
    // still expects a FunctionBox to be attached to it during BCE, and
    // the syntax parser cannot attach one to it.
    FunctionBox* funbox =
        newFunctionBox(*funNode, fun, toStringStart, inheritedDirectives,
                       generatorKind, asyncKind);
    if (!funbox) {
      return false;
    }
    funbox->initWithEnclosingParseContext(pc_, fun, kind);

    SyntaxParseHandler::Node syntaxNode =
        syntaxParser->innerFunctionForFunctionBox(
            SyntaxParseHandler::NodeGeneric, pc_, funbox, inHandling,
            yieldHandling, kind, newDirectives);
    if (!syntaxNode) {
      if (syntaxParser->hadAbortedSyntaxParse()) {
        // Try again with a full parse. UsedNameTracker needs to be
        // rewound to just before we tried the syntax parse for
        // correctness.
        syntaxParser->clearAbortedSyntaxParse();
        usedNames_.rewind(token);
        MOZ_ASSERT_IF(!syntaxParser->cx_->isHelperThreadContext(),
                      !syntaxParser->cx_->isExceptionPending());
        break;
      }
      return false;
    }
>>>>>>> upstream-releases

<<<<<<< HEAD
    // Advance this parser over tokens processed by the syntax parser.
    Position currentSyntaxPosition(keepAtoms, syntaxParser->tokenStream);
    if (!tokenStream.seek(currentSyntaxPosition, syntaxParser->anyChars)) {
      return false;
    }
||||||| merged common ancestors
        // Append possible Annex B function box only upon successfully parsing.
        if (tryAnnexB) {
            if (!pc->innermostScope()->addPossibleAnnexBFunctionBox(pc, funbox)) {
                return false;
            }
        }
=======
    // Advance this parser over tokens processed by the syntax parser.
    Position currentSyntaxPosition(keepAtoms_, syntaxParser->tokenStream);
    if (!tokenStream.seek(currentSyntaxPosition, syntaxParser->anyChars)) {
      return false;
    }
>>>>>>> upstream-releases

    // Update the end position of the parse node.
    (*funNode)->pn_pos.end = anyChars.currentToken().pos.end;

<<<<<<< HEAD
    // Append possible Annex B function box only upon successfully parsing.
    if (tryAnnexB) {
      if (!pc->innermostScope()->addPossibleAnnexBFunctionBox(pc, funbox)) {
||||||| merged common ancestors
    // We failed to do a syntax parse above, so do the full parse.
    CodeNodeType innerFunc =
        innerFunction(*funNode, pc, fun, toStringStart, inHandling, yieldHandling, kind,
                      generatorKind, asyncKind, tryAnnexB, inheritedDirectives, newDirectives);
    if (!innerFunc) {
=======
    // Append possible Annex B function box only upon successfully parsing.
    if (tryAnnexB) {
      if (!pc_->innermostScope()->addPossibleAnnexBFunctionBox(pc_, funbox)) {
>>>>>>> upstream-releases
        return false;
      }
    }

    return true;
<<<<<<< HEAD
  } while (false);

  // We failed to do a syntax parse above, so do the full parse.
  CodeNodeType innerFunc = innerFunction(
      *funNode, pc, fun, toStringStart, inHandling, yieldHandling, kind,
      generatorKind, asyncKind, tryAnnexB, inheritedDirectives, newDirectives);
  if (!innerFunc) {
    return false;
  }

  *funNode = innerFunc;
  return true;
||||||| merged common ancestors
=======
  } while (false);

  // We failed to do a syntax parse above, so do the full parse.
  FunctionNodeType innerFunc = innerFunction(
      *funNode, pc_, fun, toStringStart, inHandling, yieldHandling, kind,
      generatorKind, asyncKind, tryAnnexB, inheritedDirectives, newDirectives);
  if (!innerFunc) {
    return false;
  }

  *funNode = innerFunc;
  return true;
>>>>>>> upstream-releases
}

template <typename Unit>
<<<<<<< HEAD
bool Parser<SyntaxParseHandler, Unit>::trySyntaxParseInnerFunction(
    CodeNodeType* funNode, HandleFunction fun, uint32_t toStringStart,
    InHandling inHandling, YieldHandling yieldHandling, FunctionSyntaxKind kind,
    GeneratorKind generatorKind, FunctionAsyncKind asyncKind, bool tryAnnexB,
    Directives inheritedDirectives, Directives* newDirectives) {
  // This is already a syntax parser, so just parse the inner function.
  CodeNodeType innerFunc = innerFunction(
      *funNode, pc, fun, toStringStart, inHandling, yieldHandling, kind,
      generatorKind, asyncKind, tryAnnexB, inheritedDirectives, newDirectives);

  if (!innerFunc) {
    return false;
  }
||||||| merged common ancestors
bool
Parser<SyntaxParseHandler, Unit>::trySyntaxParseInnerFunction(CodeNodeType* funNode,
                                                              HandleFunction fun,
                                                              uint32_t toStringStart,
                                                              InHandling inHandling,
                                                              YieldHandling yieldHandling,
                                                              FunctionSyntaxKind kind,
                                                              GeneratorKind generatorKind,
                                                              FunctionAsyncKind asyncKind,
                                                              bool tryAnnexB,
                                                              Directives inheritedDirectives,
                                                              Directives* newDirectives)
{
    // This is already a syntax parser, so just parse the inner function.
    CodeNodeType innerFunc =
        innerFunction(*funNode, pc, fun, toStringStart, inHandling, yieldHandling, kind,
                      generatorKind, asyncKind, tryAnnexB, inheritedDirectives, newDirectives);

    if (!innerFunc) {
        return false;
    }
=======
bool Parser<SyntaxParseHandler, Unit>::trySyntaxParseInnerFunction(
    FunctionNodeType* funNode, HandleFunction fun, uint32_t toStringStart,
    InHandling inHandling, YieldHandling yieldHandling, FunctionSyntaxKind kind,
    GeneratorKind generatorKind, FunctionAsyncKind asyncKind, bool tryAnnexB,
    Directives inheritedDirectives, Directives* newDirectives) {
  // This is already a syntax parser, so just parse the inner function.
  FunctionNodeType innerFunc = innerFunction(
      *funNode, pc_, fun, toStringStart, inHandling, yieldHandling, kind,
      generatorKind, asyncKind, tryAnnexB, inheritedDirectives, newDirectives);

  if (!innerFunc) {
    return false;
  }
>>>>>>> upstream-releases

  *funNode = innerFunc;
  return true;
}

template <class ParseHandler, typename Unit>
<<<<<<< HEAD
inline bool GeneralParser<ParseHandler, Unit>::trySyntaxParseInnerFunction(
    CodeNodeType* funNode, HandleFunction fun, uint32_t toStringStart,
    InHandling inHandling, YieldHandling yieldHandling, FunctionSyntaxKind kind,
    GeneratorKind generatorKind, FunctionAsyncKind asyncKind, bool tryAnnexB,
    Directives inheritedDirectives, Directives* newDirectives) {
  return asFinalParser()->trySyntaxParseInnerFunction(
      funNode, fun, toStringStart, inHandling, yieldHandling, kind,
      generatorKind, asyncKind, tryAnnexB, inheritedDirectives, newDirectives);
}

template <class ParseHandler, typename Unit>
typename ParseHandler::CodeNodeType
GeneralParser<ParseHandler, Unit>::innerFunctionForFunctionBox(
    CodeNodeType funNode, ParseContext* outerpc, FunctionBox* funbox,
    InHandling inHandling, YieldHandling yieldHandling, FunctionSyntaxKind kind,
    Directives* newDirectives) {
  // Note that it is possible for outerpc != this->pc, as we may be
  // attempting to syntax parse an inner function from an outer full
  // parser. In that case, outerpc is a SourceParseContext from the full parser
  // instead of the current top of the stack of the syntax parser.

  // Push a new ParseContext.
  SourceParseContext funpc(this, funbox, newDirectives);
  if (!funpc.init()) {
    return null();
  }
||||||| merged common ancestors
inline bool
GeneralParser<ParseHandler, Unit>::trySyntaxParseInnerFunction(CodeNodeType* funNode,
                                                               HandleFunction fun,
                                                               uint32_t toStringStart,
                                                               InHandling inHandling,
                                                               YieldHandling yieldHandling,
                                                               FunctionSyntaxKind kind,
                                                               GeneratorKind generatorKind,
                                                               FunctionAsyncKind asyncKind,
                                                               bool tryAnnexB,
                                                               Directives inheritedDirectives,
                                                               Directives* newDirectives)
{
    return asFinalParser()->trySyntaxParseInnerFunction(funNode, fun, toStringStart, inHandling,
                                                        yieldHandling, kind, generatorKind,
                                                        asyncKind, tryAnnexB, inheritedDirectives,
                                                        newDirectives);
}

template <class ParseHandler, typename Unit>
typename ParseHandler::CodeNodeType
GeneralParser<ParseHandler, Unit>::innerFunctionForFunctionBox(CodeNodeType funNode,
                                                               ParseContext* outerpc,
                                                               FunctionBox* funbox,
                                                               InHandling inHandling,
                                                               YieldHandling yieldHandling,
                                                               FunctionSyntaxKind kind,
                                                               Directives* newDirectives)
{
    // Note that it is possible for outerpc != this->pc, as we may be
    // attempting to syntax parse an inner function from an outer full
    // parser. In that case, outerpc is a SourceParseContext from the full parser
    // instead of the current top of the stack of the syntax parser.

    // Push a new ParseContext.
    SourceParseContext funpc(this, funbox, newDirectives);
    if (!funpc.init()) {
        return null();
    }
=======
inline bool GeneralParser<ParseHandler, Unit>::trySyntaxParseInnerFunction(
    FunctionNodeType* funNode, HandleFunction fun, uint32_t toStringStart,
    InHandling inHandling, YieldHandling yieldHandling, FunctionSyntaxKind kind,
    GeneratorKind generatorKind, FunctionAsyncKind asyncKind, bool tryAnnexB,
    Directives inheritedDirectives, Directives* newDirectives) {
  return asFinalParser()->trySyntaxParseInnerFunction(
      funNode, fun, toStringStart, inHandling, yieldHandling, kind,
      generatorKind, asyncKind, tryAnnexB, inheritedDirectives, newDirectives);
}
>>>>>>> upstream-releases

<<<<<<< HEAD
  if (!functionFormalParametersAndBody(inHandling, yieldHandling, &funNode,
                                       kind)) {
    return null();
  }
||||||| merged common ancestors
    if (!functionFormalParametersAndBody(inHandling, yieldHandling, &funNode, kind)) {
        return null();
    }
=======
template <class ParseHandler, typename Unit>
typename ParseHandler::FunctionNodeType
GeneralParser<ParseHandler, Unit>::innerFunctionForFunctionBox(
    FunctionNodeType funNode, ParseContext* outerpc, FunctionBox* funbox,
    InHandling inHandling, YieldHandling yieldHandling, FunctionSyntaxKind kind,
    Directives* newDirectives) {
  // Note that it is possible for outerpc != this->pc_, as we may be
  // attempting to syntax parse an inner function from an outer full
  // parser. In that case, outerpc is a SourceParseContext from the full parser
  // instead of the current top of the stack of the syntax parser.
>>>>>>> upstream-releases

<<<<<<< HEAD
  if (!leaveInnerFunction(outerpc)) {
    return null();
  }
||||||| merged common ancestors
    if (!leaveInnerFunction(outerpc)) {
        return null();
    }
=======
  // Push a new ParseContext.
  SourceParseContext funpc(this, funbox, newDirectives);
  if (!funpc.init()) {
    return null();
  }
>>>>>>> upstream-releases

<<<<<<< HEAD
  return funNode;
||||||| merged common ancestors
    return funNode;
=======
  if (!functionFormalParametersAndBody(inHandling, yieldHandling, &funNode,
                                       kind)) {
    return null();
  }

  if (!leaveInnerFunction(outerpc)) {
    return null();
  }

  return funNode;
>>>>>>> upstream-releases
}

template <class ParseHandler, typename Unit>
<<<<<<< HEAD
typename ParseHandler::CodeNodeType
GeneralParser<ParseHandler, Unit>::innerFunction(
    CodeNodeType funNode, ParseContext* outerpc, HandleFunction fun,
    uint32_t toStringStart, InHandling inHandling, YieldHandling yieldHandling,
    FunctionSyntaxKind kind, GeneratorKind generatorKind,
    FunctionAsyncKind asyncKind, bool tryAnnexB, Directives inheritedDirectives,
    Directives* newDirectives) {
  // Note that it is possible for outerpc != this->pc, as we may be
  // attempting to syntax parse an inner function from an outer full
  // parser. In that case, outerpc is a SourceParseContext from the full parser
  // instead of the current top of the stack of the syntax parser.

  FunctionBox* funbox =
      newFunctionBox(funNode, fun, toStringStart, inheritedDirectives,
                     generatorKind, asyncKind);
  if (!funbox) {
    return null();
  }
  funbox->initWithEnclosingParseContext(outerpc, kind);
||||||| merged common ancestors
typename ParseHandler::CodeNodeType
GeneralParser<ParseHandler, Unit>::innerFunction(CodeNodeType funNode, ParseContext* outerpc,
                                                 HandleFunction fun, uint32_t toStringStart,
                                                 InHandling inHandling,
                                                 YieldHandling yieldHandling,
                                                 FunctionSyntaxKind kind,
                                                 GeneratorKind generatorKind,
                                                 FunctionAsyncKind asyncKind, bool tryAnnexB,
                                                 Directives inheritedDirectives,
                                                 Directives* newDirectives)
{
    // Note that it is possible for outerpc != this->pc, as we may be
    // attempting to syntax parse an inner function from an outer full
    // parser. In that case, outerpc is a SourceParseContext from the full parser
    // instead of the current top of the stack of the syntax parser.

    FunctionBox* funbox = newFunctionBox(funNode, fun, toStringStart, inheritedDirectives,
                                         generatorKind, asyncKind);
    if (!funbox) {
        return null();
    }
    funbox->initWithEnclosingParseContext(outerpc, kind);
=======
typename ParseHandler::FunctionNodeType
GeneralParser<ParseHandler, Unit>::innerFunction(
    FunctionNodeType funNode, ParseContext* outerpc, HandleFunction fun,
    uint32_t toStringStart, InHandling inHandling, YieldHandling yieldHandling,
    FunctionSyntaxKind kind, GeneratorKind generatorKind,
    FunctionAsyncKind asyncKind, bool tryAnnexB, Directives inheritedDirectives,
    Directives* newDirectives) {
  // Note that it is possible for outerpc != this->pc_, as we may be
  // attempting to syntax parse an inner function from an outer full
  // parser. In that case, outerpc is a SourceParseContext from the full parser
  // instead of the current top of the stack of the syntax parser.
>>>>>>> upstream-releases

<<<<<<< HEAD
  CodeNodeType innerFunc = innerFunctionForFunctionBox(
      funNode, outerpc, funbox, inHandling, yieldHandling, kind, newDirectives);
  if (!innerFunc) {
    return null();
  }
||||||| merged common ancestors
    CodeNodeType innerFunc =
        innerFunctionForFunctionBox(funNode, outerpc, funbox, inHandling, yieldHandling, kind,
                                     newDirectives);
    if (!innerFunc) {
        return null();
    }
=======
  FunctionBox* funbox =
      newFunctionBox(funNode, fun, toStringStart, inheritedDirectives,
                     generatorKind, asyncKind);
  if (!funbox) {
    return null();
  }
  funbox->initWithEnclosingParseContext(outerpc, fun, kind);
>>>>>>> upstream-releases

<<<<<<< HEAD
  // Append possible Annex B function box only upon successfully parsing.
  if (tryAnnexB) {
    if (!pc->innermostScope()->addPossibleAnnexBFunctionBox(pc, funbox)) {
      return null();
||||||| merged common ancestors
    // Append possible Annex B function box only upon successfully parsing.
    if (tryAnnexB) {
        if (!pc->innermostScope()->addPossibleAnnexBFunctionBox(pc, funbox)) {
            return null();
        }
=======
  FunctionNodeType innerFunc = innerFunctionForFunctionBox(
      funNode, outerpc, funbox, inHandling, yieldHandling, kind, newDirectives);
  if (!innerFunc) {
    return null();
  }

  // Append possible Annex B function box only upon successfully parsing.
  if (tryAnnexB) {
    if (!pc_->innermostScope()->addPossibleAnnexBFunctionBox(pc_, funbox)) {
      return null();
>>>>>>> upstream-releases
    }
  }

  return innerFunc;
}

template <class ParseHandler, typename Unit>
bool GeneralParser<ParseHandler, Unit>::appendToCallSiteObj(
    CallSiteNodeType callSiteObj) {
  Node cookedNode = noSubstitutionTaggedTemplate();
  if (!cookedNode) {
    return false;
  }

<<<<<<< HEAD
  JSAtom* atom = tokenStream.getRawTemplateStringAtom();
  if (!atom) {
    return false;
  }
  NameNodeType rawNode = handler.newTemplateStringLiteral(atom, pos());
  if (!rawNode) {
    return false;
  }
||||||| merged common ancestors
    JSAtom* atom = tokenStream.getRawTemplateStringAtom();
    if (!atom) {
        return false;
    }
    NameNodeType rawNode = handler.newTemplateStringLiteral(atom, pos());
    if (!rawNode) {
        return false;
    }
=======
  JSAtom* atom = tokenStream.getRawTemplateStringAtom();
  if (!atom) {
    return false;
  }
  NameNodeType rawNode = handler_.newTemplateStringLiteral(atom, pos());
  if (!rawNode) {
    return false;
  }
>>>>>>> upstream-releases

<<<<<<< HEAD
  handler.addToCallSiteObject(callSiteObj, rawNode, cookedNode);
  return true;
||||||| merged common ancestors
    handler.addToCallSiteObject(callSiteObj, rawNode, cookedNode);
    return true;
=======
  handler_.addToCallSiteObject(callSiteObj, rawNode, cookedNode);
  return true;
>>>>>>> upstream-releases
}

template <typename Unit>
<<<<<<< HEAD
CodeNode* Parser<FullParseHandler, Unit>::standaloneLazyFunction(
    HandleFunction fun, uint32_t toStringStart, bool strict,
    GeneratorKind generatorKind, FunctionAsyncKind asyncKind) {
  MOZ_ASSERT(checkOptionsCalled);
||||||| merged common ancestors
CodeNode*
Parser<FullParseHandler, Unit>::standaloneLazyFunction(HandleFunction fun, uint32_t toStringStart,
                                                       bool strict, GeneratorKind generatorKind,
                                                       FunctionAsyncKind asyncKind)
{
    MOZ_ASSERT(checkOptionsCalled);

    CodeNodeType funNode = handler.newFunctionStatement(pos());
    if (!funNode) {
        return null();
    }

    Directives directives(strict);
    FunctionBox* funbox = newFunctionBox(funNode, fun, toStringStart, directives, generatorKind,
                                         asyncKind);
    if (!funbox) {
        return null();
    }
    funbox->initFromLazyFunction();

    Directives newDirectives = directives;
    SourceParseContext funpc(this, funbox, &newDirectives);
    if (!funpc.init()) {
        return null();
    }
=======
FunctionNode* Parser<FullParseHandler, Unit>::standaloneLazyFunction(
    HandleFunction fun, uint32_t toStringStart, bool strict,
    GeneratorKind generatorKind, FunctionAsyncKind asyncKind) {
  MOZ_ASSERT(checkOptionsCalled_);

  FunctionSyntaxKind syntaxKind = FunctionSyntaxKind::Statement;
  if (fun->isClassConstructor()) {
    if (fun->isDerivedClassConstructor()) {
      syntaxKind = FunctionSyntaxKind::DerivedClassConstructor;
    } else {
      syntaxKind = FunctionSyntaxKind::ClassConstructor;
    }
  } else if (fun->isMethod()) {
    syntaxKind = FunctionSyntaxKind::Method;
  } else if (fun->isGetter()) {
    syntaxKind = FunctionSyntaxKind::Getter;
  } else if (fun->isSetter()) {
    syntaxKind = FunctionSyntaxKind::Setter;
  } else if (fun->isArrow()) {
    syntaxKind = FunctionSyntaxKind::Arrow;
  }

  FunctionNodeType funNode = handler_.newFunction(syntaxKind, pos());
  if (!funNode) {
    return null();
  }

  Directives directives(strict);
  FunctionBox* funbox = newFunctionBox(funNode, fun, toStringStart, directives,
                                       generatorKind, asyncKind);
  if (!funbox) {
    return null();
  }
  funbox->initFromLazyFunction(fun);

  Directives newDirectives = directives;
  SourceParseContext funpc(this, funbox, &newDirectives);
  if (!funpc.init()) {
    return null();
  }

  // Our tokenStream has no current token, so funNode's position is garbage.
  // Substitute the position of the first token in our source.  If the
  // function is a not-async arrow, use TokenStream::SlashIsRegExp to keep
  // verifyConsistentModifier from complaining (we will use
  // TokenStream::SlashIsRegExp in functionArguments).
  Modifier modifier =
      (fun->isArrow() && asyncKind == FunctionAsyncKind::SyncFunction)
          ? TokenStream::SlashIsRegExp
          : TokenStream::SlashIsDiv;
  if (!tokenStream.peekTokenPos(&funNode->pn_pos, modifier)) {
    return null();
  }
>>>>>>> upstream-releases

<<<<<<< HEAD
  CodeNodeType funNode = handler.newFunctionStatement(pos());
  if (!funNode) {
    return null();
  }
||||||| merged common ancestors
    // Our tokenStream has no current token, so funNode's position is garbage.
    // Substitute the position of the first token in our source.  If the
    // function is a not-async arrow, use TokenStream::Operand to keep
    // verifyConsistentModifier from complaining (we will use
    // TokenStream::Operand in functionArguments).
    Modifier modifier = (fun->isArrow() && asyncKind == FunctionAsyncKind::SyncFunction)
                        ? TokenStream::Operand
                        : TokenStream::None;
    if (!tokenStream.peekTokenPos(&funNode->pn_pos, modifier)) {
        return null();
    }
=======
  YieldHandling yieldHandling = GetYieldHandling(generatorKind);
>>>>>>> upstream-releases

<<<<<<< HEAD
  Directives directives(strict);
  FunctionBox* funbox = newFunctionBox(funNode, fun, toStringStart, directives,
                                       generatorKind, asyncKind);
  if (!funbox) {
    return null();
  }
  funbox->initFromLazyFunction();
||||||| merged common ancestors
    YieldHandling yieldHandling = GetYieldHandling(generatorKind);
    FunctionSyntaxKind syntaxKind = FunctionSyntaxKind::Statement;
    if (fun->isClassConstructor()) {
        syntaxKind = FunctionSyntaxKind::ClassConstructor;
    } else if (fun->isMethod()) {
        syntaxKind = FunctionSyntaxKind::Method;
    } else if (fun->isGetter()) {
        syntaxKind = FunctionSyntaxKind::Getter;
    } else if (fun->isSetter()) {
        syntaxKind = FunctionSyntaxKind::Setter;
    } else if (fun->isArrow()) {
        syntaxKind = FunctionSyntaxKind::Arrow;
    }
=======
  if (!functionFormalParametersAndBody(InAllowed, yieldHandling, &funNode,
                                       syntaxKind)) {
    MOZ_ASSERT(directives == newDirectives);
    return null();
  }
>>>>>>> upstream-releases

<<<<<<< HEAD
  Directives newDirectives = directives;
  SourceParseContext funpc(this, funbox, &newDirectives);
  if (!funpc.init()) {
    return null();
  }

  // Our tokenStream has no current token, so funNode's position is garbage.
  // Substitute the position of the first token in our source.  If the
  // function is a not-async arrow, use TokenStream::Operand to keep
  // verifyConsistentModifier from complaining (we will use
  // TokenStream::Operand in functionArguments).
  Modifier modifier =
      (fun->isArrow() && asyncKind == FunctionAsyncKind::SyncFunction)
          ? TokenStream::Operand
          : TokenStream::None;
  if (!tokenStream.peekTokenPos(&funNode->pn_pos, modifier)) {
    return null();
  }

  YieldHandling yieldHandling = GetYieldHandling(generatorKind);
  FunctionSyntaxKind syntaxKind = FunctionSyntaxKind::Statement;
  if (fun->isClassConstructor()) {
    syntaxKind = FunctionSyntaxKind::ClassConstructor;
  } else if (fun->isMethod()) {
    syntaxKind = FunctionSyntaxKind::Method;
  } else if (fun->isGetter()) {
    syntaxKind = FunctionSyntaxKind::Getter;
  } else if (fun->isSetter()) {
    syntaxKind = FunctionSyntaxKind::Setter;
  } else if (fun->isArrow()) {
    syntaxKind = FunctionSyntaxKind::Arrow;
  }

  if (!functionFormalParametersAndBody(InAllowed, yieldHandling, &funNode,
                                       syntaxKind)) {
    MOZ_ASSERT(directives == newDirectives);
    return null();
  }
||||||| merged common ancestors
    if (!functionFormalParametersAndBody(InAllowed, yieldHandling, &funNode, syntaxKind)) {
        MOZ_ASSERT(directives == newDirectives);
        return null();
    }
=======
  if (!CheckParseTree(cx_, alloc_, funNode)) {
    return null();
  }
>>>>>>> upstream-releases

<<<<<<< HEAD
  ParseNode* node = funNode;
  if (!FoldConstants(context, &node, this)) {
    return null();
  }
  funNode = &node->as<CodeNode>();
||||||| merged common ancestors
    ParseNode* node = funNode;
    if (!FoldConstants(context, &node, this)) {
        return null();
    }
    funNode = &node->as<CodeNode>();
=======
  ParseNode* node = funNode;
  // Don't constant-fold inside "use asm" code, as this could create a parse
  // tree that doesn't type-check as asm.js.
  if (!pc_->useAsmOrInsideUseAsm()) {
    if (!FoldConstants(cx_, &node, &handler_)) {
      return null();
    }
  }
  funNode = &node->as<FunctionNode>();
>>>>>>> upstream-releases

<<<<<<< HEAD
  return funNode;
||||||| merged common ancestors
    return funNode;
=======
  return funNode;
}

void ParserBase::setFunctionEndFromCurrentToken(FunctionBox* funbox) const {
  funbox->setEnd(anyChars.currentToken().pos.end);
>>>>>>> upstream-releases
}

template <class ParseHandler, typename Unit>
<<<<<<< HEAD
bool GeneralParser<ParseHandler, Unit>::functionFormalParametersAndBody(
    InHandling inHandling, YieldHandling yieldHandling, CodeNodeType* funNode,
    FunctionSyntaxKind kind,
    const Maybe<uint32_t>& parameterListEnd /* = Nothing() */,
    bool isStandaloneFunction /* = false */) {
  // Given a properly initialized parse context, try to parse an actual
  // function without concern for conversion to strict mode, use of lazy
  // parsing and such.
||||||| merged common ancestors
bool
GeneralParser<ParseHandler, Unit>::functionFormalParametersAndBody(InHandling inHandling,
                                                                   YieldHandling yieldHandling,
                                                                   CodeNodeType* funNode,
                                                                   FunctionSyntaxKind kind,
                                                                   const Maybe<uint32_t>& parameterListEnd /* = Nothing() */,
                                                                   bool isStandaloneFunction /* = false */)
{
    // Given a properly initialized parse context, try to parse an actual
    // function without concern for conversion to strict mode, use of lazy
    // parsing and such.
=======
bool GeneralParser<ParseHandler, Unit>::functionFormalParametersAndBody(
    InHandling inHandling, YieldHandling yieldHandling,
    FunctionNodeType* funNode, FunctionSyntaxKind kind,
    const Maybe<uint32_t>& parameterListEnd /* = Nothing() */,
    bool isStandaloneFunction /* = false */) {
  // Given a properly initialized parse context, try to parse an actual
  // function without concern for conversion to strict mode, use of lazy
  // parsing and such.
>>>>>>> upstream-releases

<<<<<<< HEAD
  FunctionBox* funbox = pc->functionBox();
  RootedFunction fun(context, funbox->function());
||||||| merged common ancestors
    FunctionBox* funbox = pc->functionBox();
    RootedFunction fun(context, funbox->function());
=======
  FunctionBox* funbox = pc_->functionBox();
  RootedFunction fun(cx_, funbox->function());
>>>>>>> upstream-releases

<<<<<<< HEAD
  // See below for an explanation why arrow function parameters and arrow
  // function bodies are parsed with different yield/await settings.
  {
    AwaitHandling awaitHandling =
        (funbox->isAsync() ||
         (kind == FunctionSyntaxKind::Arrow && awaitIsKeyword()))
||||||| merged common ancestors
    // See below for an explanation why arrow function parameters and arrow
    // function bodies are parsed with different yield/await settings.
    {
        AwaitHandling awaitHandling =
            (funbox->isAsync() || (kind == FunctionSyntaxKind::Arrow && awaitIsKeyword()))
=======
  if (kind == FunctionSyntaxKind::ClassConstructor ||
      kind == FunctionSyntaxKind::DerivedClassConstructor) {
    if (!noteUsedName(cx_->names().dotInitializers)) {
      return false;
    }
  }

  // See below for an explanation why arrow function parameters and arrow
  // function bodies are parsed with different yield/await settings.
  {
    AwaitHandling awaitHandling =
        (funbox->isAsync() ||
         (kind == FunctionSyntaxKind::Arrow && awaitIsKeyword()))
>>>>>>> upstream-releases
            ? AwaitIsKeyword
            : AwaitIsName;
    AutoAwaitIsKeyword<ParseHandler, Unit> awaitIsKeyword(this, awaitHandling);
    AutoInParametersOfAsyncFunction<ParseHandler, Unit> inParameters(
        this, funbox->isAsync());
    if (!functionArguments(yieldHandling, kind, *funNode)) {
      return false;
    }
  }

<<<<<<< HEAD
  Maybe<ParseContext::VarScope> varScope;
  if (funbox->hasParameterExprs) {
    varScope.emplace(this);
    if (!varScope->init(pc)) {
      return false;
||||||| merged common ancestors
    Maybe<ParseContext::VarScope> varScope;
    if (funbox->hasParameterExprs) {
        varScope.emplace(this);
        if (!varScope->init(pc)) {
            return false;
        }
    } else {
        pc->functionScope().useAsVarScope(pc);
=======
  Maybe<ParseContext::VarScope> varScope;
  if (funbox->hasParameterExprs) {
    varScope.emplace(this);
    if (!varScope->init(pc_)) {
      return false;
>>>>>>> upstream-releases
    }
<<<<<<< HEAD
  } else {
    pc->functionScope().useAsVarScope(pc);
  }
||||||| merged common ancestors
=======
  } else {
    pc_->functionScope().useAsVarScope(pc_);
  }
>>>>>>> upstream-releases

  if (kind == FunctionSyntaxKind::Arrow) {
    bool matched;
    if (!tokenStream.matchToken(&matched, TokenKind::Arrow)) {
      return false;
    }
    if (!matched) {
      error(JSMSG_BAD_ARROW_ARGS);
      return false;
    }
  }

<<<<<<< HEAD
  // When parsing something for new Function() we have to make sure to
  // only treat a certain part of the source as a parameter list.
  if (parameterListEnd.isSome() && parameterListEnd.value() != pos().begin) {
    error(JSMSG_UNEXPECTED_PARAMLIST_END);
    return false;
  }
||||||| merged common ancestors
    // Parse the function body.
    FunctionBodyType bodyType = StatementListBody;
    TokenKind tt;
    if (!tokenStream.getToken(&tt, TokenStream::Operand)) {
        return false;
    }
    uint32_t openedPos = 0;
    if (tt != TokenKind::LeftCurly) {
        if (kind != FunctionSyntaxKind::Arrow) {
            error(JSMSG_CURLY_BEFORE_BODY);
            return false;
        }
=======
  // When parsing something for new Function() we have to make sure to
  // only treat a certain part of the source as a parameter list.
  if (parameterListEnd.isSome() && parameterListEnd.value() != pos().begin) {
    error(JSMSG_UNEXPECTED_PARAMLIST_END);
    return false;
  }

  // Parse the function body.
  FunctionBodyType bodyType = StatementListBody;
  TokenKind tt;
  if (!tokenStream.getToken(&tt, TokenStream::SlashIsRegExp)) {
    return false;
  }
  uint32_t openedPos = 0;
  if (tt != TokenKind::LeftCurly) {
    if (kind != FunctionSyntaxKind::Arrow) {
      error(JSMSG_CURLY_BEFORE_BODY);
      return false;
    }
>>>>>>> upstream-releases

<<<<<<< HEAD
  // Parse the function body.
  FunctionBodyType bodyType = StatementListBody;
  TokenKind tt;
  if (!tokenStream.getToken(&tt, TokenStream::Operand)) {
    return false;
  }
  uint32_t openedPos = 0;
  if (tt != TokenKind::LeftCurly) {
    if (kind != FunctionSyntaxKind::Arrow) {
      error(JSMSG_CURLY_BEFORE_BODY);
      return false;
    }

    anyChars.ungetToken();
    bodyType = ExpressionBody;
    funbox->setHasExprBody();
  } else {
    openedPos = pos().begin;
  }

  // Arrow function parameters inherit yieldHandling from the enclosing
  // context, but the arrow body doesn't. E.g. in |(a = yield) => yield|,
  // |yield| in the parameters is either a name or keyword, depending on
  // whether the arrow function is enclosed in a generator function or not.
  // Whereas the |yield| in the function body is always parsed as a name.
  // The same goes when parsing |await| in arrow functions.
  YieldHandling bodyYieldHandling = GetYieldHandling(pc->generatorKind());
  AwaitHandling bodyAwaitHandling = GetAwaitHandling(pc->asyncKind());
  bool inheritedStrict = pc->sc()->strict();
  LexicalScopeNodeType body;
  {
    AutoAwaitIsKeyword<ParseHandler, Unit> awaitIsKeyword(this,
                                                          bodyAwaitHandling);
    AutoInParametersOfAsyncFunction<ParseHandler, Unit> inParameters(this,
                                                                     false);
    body = functionBody(inHandling, bodyYieldHandling, kind, bodyType);
    if (!body) {
      return false;
    }
  }

  // Revalidate the function name when we transitioned to strict mode.
  if ((kind == FunctionSyntaxKind::Statement ||
       kind == FunctionSyntaxKind::Expression) &&
      fun->explicitName() && !inheritedStrict && pc->sc()->strict()) {
    MOZ_ASSERT(pc->sc()->hasExplicitUseStrict(),
               "strict mode should only change when a 'use strict' directive "
               "is present");

    PropertyName* propertyName = fun->explicitName()->asPropertyName();
    YieldHandling nameYieldHandling;
    if (kind == FunctionSyntaxKind::Expression) {
      // Named lambda has binding inside it.
      nameYieldHandling = bodyYieldHandling;
    } else {
      // Otherwise YieldHandling cannot be checked at this point
      // because of different context.
      // It should already be checked before this point.
      nameYieldHandling = YieldIsName;
||||||| merged common ancestors
        anyChars.ungetToken();
        bodyType = ExpressionBody;
        funbox->setHasExprBody();
    } else {
        openedPos = pos().begin;
    }

    // Arrow function parameters inherit yieldHandling from the enclosing
    // context, but the arrow body doesn't. E.g. in |(a = yield) => yield|,
    // |yield| in the parameters is either a name or keyword, depending on
    // whether the arrow function is enclosed in a generator function or not.
    // Whereas the |yield| in the function body is always parsed as a name.
    // The same goes when parsing |await| in arrow functions.
    YieldHandling bodyYieldHandling = GetYieldHandling(pc->generatorKind());
    AwaitHandling bodyAwaitHandling = GetAwaitHandling(pc->asyncKind());
    bool inheritedStrict = pc->sc()->strict();
    LexicalScopeNodeType body;
    {
        AutoAwaitIsKeyword<ParseHandler, Unit> awaitIsKeyword(this, bodyAwaitHandling);
        AutoInParametersOfAsyncFunction<ParseHandler, Unit> inParameters(this, false);
        body = functionBody(inHandling, bodyYieldHandling, kind, bodyType);
        if (!body) {
            return false;
        }
=======
    anyChars.ungetToken();
    bodyType = ExpressionBody;
    funbox->setHasExprBody();
  } else {
    openedPos = pos().begin;
  }

  // Arrow function parameters inherit yieldHandling from the enclosing
  // context, but the arrow body doesn't. E.g. in |(a = yield) => yield|,
  // |yield| in the parameters is either a name or keyword, depending on
  // whether the arrow function is enclosed in a generator function or not.
  // Whereas the |yield| in the function body is always parsed as a name.
  // The same goes when parsing |await| in arrow functions.
  YieldHandling bodyYieldHandling = GetYieldHandling(pc_->generatorKind());
  AwaitHandling bodyAwaitHandling = GetAwaitHandling(pc_->asyncKind());
  bool inheritedStrict = pc_->sc()->strict();
  LexicalScopeNodeType body;
  {
    AutoAwaitIsKeyword<ParseHandler, Unit> awaitIsKeyword(this,
                                                          bodyAwaitHandling);
    AutoInParametersOfAsyncFunction<ParseHandler, Unit> inParameters(this,
                                                                     false);
    body = functionBody(inHandling, bodyYieldHandling, kind, bodyType);
    if (!body) {
      return false;
    }
  }

  // Revalidate the function name when we transitioned to strict mode.
  if ((kind == FunctionSyntaxKind::Statement ||
       kind == FunctionSyntaxKind::Expression) &&
      fun->explicitName() && !inheritedStrict && pc_->sc()->strict()) {
    MOZ_ASSERT(pc_->sc()->hasExplicitUseStrict(),
               "strict mode should only change when a 'use strict' directive "
               "is present");

    PropertyName* propertyName = fun->explicitName()->asPropertyName();
    YieldHandling nameYieldHandling;
    if (kind == FunctionSyntaxKind::Expression) {
      // Named lambda has binding inside it.
      nameYieldHandling = bodyYieldHandling;
    } else {
      // Otherwise YieldHandling cannot be checked at this point
      // because of different context.
      // It should already be checked before this point.
      nameYieldHandling = YieldIsName;
>>>>>>> upstream-releases
    }

    // We already use the correct await-handling at this point, therefore
    // we don't need call AutoAwaitIsKeyword here.

<<<<<<< HEAD
    uint32_t nameOffset = handler.getFunctionNameOffset(*funNode, anyChars);
    if (!checkBindingIdentifier(propertyName, nameOffset, nameYieldHandling)) {
      return false;
||||||| merged common ancestors
        // We already use the correct await-handling at this point, therefore
        // we don't need call AutoAwaitIsKeyword here.

        uint32_t nameOffset = handler.getFunctionNameOffset(*funNode, anyChars);
        if (!checkBindingIdentifier(propertyName, nameOffset, nameYieldHandling)) {
            return false;
        }
=======
    uint32_t nameOffset = handler_.getFunctionNameOffset(*funNode, anyChars);
    if (!checkBindingIdentifier(propertyName, nameOffset, nameYieldHandling)) {
      return false;
    }
  }

  if (bodyType == StatementListBody) {
    // Cannot use mustMatchToken here because of internal compiler error on
    // gcc 6.4.0, with linux 64 SM hazard build.
    TokenKind actual;
    if (!tokenStream.getToken(&actual, TokenStream::SlashIsRegExp)) {
      return false;
    }
    if (actual != TokenKind::RightCurly) {
      reportMissingClosing(JSMSG_CURLY_AFTER_BODY, JSMSG_CURLY_OPENED,
                           openedPos);
      return false;
>>>>>>> upstream-releases
    }
  }

<<<<<<< HEAD
  if (bodyType == StatementListBody) {
    // Cannot use mustMatchToken here because of internal compiler error on
    // gcc 6.4.0, with linux 64 SM hazard build.
    TokenKind actual;
    if (!tokenStream.getToken(&actual, TokenStream::Operand)) {
      return false;
||||||| merged common ancestors
    if (bodyType == StatementListBody) {
        MUST_MATCH_TOKEN_MOD_WITH_REPORT_OR(TokenKind::RightCurly, TokenStream::Operand,
                                            reportMissingClosing(JSMSG_CURLY_AFTER_BODY,
                                                                 JSMSG_CURLY_OPENED, openedPos),
                                            false);
        funbox->setEnd(anyChars);
    } else {
        MOZ_ASSERT(kind == FunctionSyntaxKind::Arrow);

        if (anyChars.hadError()) {
            return false;
        }
        funbox->setEnd(anyChars);
        if (kind == FunctionSyntaxKind::Statement && !matchOrInsertSemicolon()) {
            return false;
        }
=======
    setFunctionEndFromCurrentToken(funbox);
  } else {
    MOZ_ASSERT(kind == FunctionSyntaxKind::Arrow);

    if (anyChars.hadError()) {
      return false;
>>>>>>> upstream-releases
    }
<<<<<<< HEAD
    if (actual != TokenKind::RightCurly) {
      reportMissingClosing(JSMSG_CURLY_AFTER_BODY, JSMSG_CURLY_OPENED,
                           openedPos);
      return false;
    }
    funbox->setEnd(anyChars);
  } else {
    MOZ_ASSERT(kind == FunctionSyntaxKind::Arrow);
||||||| merged common ancestors

    if (IsMethodDefinitionKind(kind) && pc->superScopeNeedsHomeObject()) {
        funbox->setNeedsHomeObject();
    }
=======

    setFunctionEndFromCurrentToken(funbox);
>>>>>>> upstream-releases

<<<<<<< HEAD
    if (anyChars.hadError()) {
      return false;
||||||| merged common ancestors
    if (!finishFunction(isStandaloneFunction)) {
        return false;
=======
    if (kind == FunctionSyntaxKind::Statement) {
      if (!matchOrInsertSemicolon()) {
        return false;
      }
>>>>>>> upstream-releases
    }
<<<<<<< HEAD
    funbox->setEnd(anyChars);
    if (kind == FunctionSyntaxKind::Statement && !matchOrInsertSemicolon()) {
      return false;
    }
  }
||||||| merged common ancestors
=======
  }
>>>>>>> upstream-releases

<<<<<<< HEAD
  if (IsMethodDefinitionKind(kind) && pc->superScopeNeedsHomeObject()) {
    funbox->setNeedsHomeObject();
  }
||||||| merged common ancestors
    handler.setEndPosition(body, pos().begin);
    handler.setEndPosition(*funNode, pos().end);
    handler.setFunctionBody(*funNode, body);
=======
  if (IsMethodDefinitionKind(kind) && pc_->superScopeNeedsHomeObject()) {
    funbox->setNeedsHomeObject();
  }
>>>>>>> upstream-releases

<<<<<<< HEAD
  if (!finishFunction(isStandaloneFunction)) {
    return false;
  }

  handler.setEndPosition(body, pos().begin);
  handler.setEndPosition(*funNode, pos().end);
  handler.setFunctionBody(*funNode, body);

  return true;
||||||| merged common ancestors
    return true;
=======
  if (!finishFunction(isStandaloneFunction)) {
    return false;
  }

  handler_.setEndPosition(body, pos().begin);
  handler_.setEndPosition(*funNode, pos().end);
  handler_.setFunctionBody(*funNode, body);

  return true;
>>>>>>> upstream-releases
}

template <class ParseHandler, typename Unit>
typename ParseHandler::FunctionNodeType
GeneralParser<ParseHandler, Unit>::functionStmt(uint32_t toStringStart,
<<<<<<< HEAD
                                                YieldHandling yieldHandling,
                                                DefaultHandling defaultHandling,
                                                FunctionAsyncKind asyncKind) {
  MOZ_ASSERT(anyChars.isCurrentTokenType(TokenKind::Function));

  // In sloppy mode, Annex B.3.2 allows labelled function declarations.
  // Otherwise it's a parse error.
  ParseContext::Statement* declaredInStmt = pc->innermostStatement();
  if (declaredInStmt && declaredInStmt->kind() == StatementKind::Label) {
    MOZ_ASSERT(!pc->sc()->strict(),
               "labeled functions shouldn't be parsed in strict mode");

    // Find the innermost non-label statement.  Report an error if it's
    // unbraced: functions can't appear in it.  Otherwise the statement
    // (or its absence) determines the scope the function's bound in.
    while (declaredInStmt && declaredInStmt->kind() == StatementKind::Label) {
      declaredInStmt = declaredInStmt->enclosing();
    }

    if (declaredInStmt && !StatementKindIsBraced(declaredInStmt->kind())) {
      error(JSMSG_SLOPPY_FUNCTION_LABEL);
      return null();
    }
  }

  TokenKind tt;
  if (!tokenStream.getToken(&tt)) {
    return null();
  }
||||||| merged common ancestors
                                                YieldHandling yieldHandling,
                                                DefaultHandling defaultHandling,
                                                FunctionAsyncKind asyncKind)
{
    MOZ_ASSERT(anyChars.isCurrentTokenType(TokenKind::Function));

    // In sloppy mode, Annex B.3.2 allows labelled function declarations.
    // Otherwise it's a parse error.
    ParseContext::Statement* declaredInStmt = pc->innermostStatement();
    if (declaredInStmt && declaredInStmt->kind() == StatementKind::Label) {
        MOZ_ASSERT(!pc->sc()->strict(),
                   "labeled functions shouldn't be parsed in strict mode");

        // Find the innermost non-label statement.  Report an error if it's
        // unbraced: functions can't appear in it.  Otherwise the statement
        // (or its absence) determines the scope the function's bound in.
        while (declaredInStmt && declaredInStmt->kind() == StatementKind::Label) {
            declaredInStmt = declaredInStmt->enclosing();
        }

        if (declaredInStmt && !StatementKindIsBraced(declaredInStmt->kind())) {
            error(JSMSG_SLOPPY_FUNCTION_LABEL);
            return null();
        }
    }

    TokenKind tt;
    if (!tokenStream.getToken(&tt)) {
        return null();
    }

    GeneratorKind generatorKind = GeneratorKind::NotGenerator;
    if (tt == TokenKind::Mul) {
        generatorKind = GeneratorKind::Generator;
        if (!tokenStream.getToken(&tt)) {
            return null();
        }
    }

    RootedPropertyName name(context);
    if (TokenKindIsPossibleIdentifier(tt)) {
        name = bindingIdentifier(yieldHandling);
        if (!name) {
            return null();
        }
    } else if (defaultHandling == AllowDefaultName) {
        name = context->names().default_;
        anyChars.ungetToken();
    } else {
        /* Unnamed function expressions are forbidden in statement context. */
        error(JSMSG_UNNAMED_FUNCTION_STMT);
        return null();
    }

    // Note the declared name and check for early errors.
    DeclarationKind kind;
    if (declaredInStmt) {
        MOZ_ASSERT(declaredInStmt->kind() != StatementKind::Label);
        MOZ_ASSERT(StatementKindIsBraced(declaredInStmt->kind()));
=======
                                                YieldHandling yieldHandling,
                                                DefaultHandling defaultHandling,
                                                FunctionAsyncKind asyncKind) {
  MOZ_ASSERT(anyChars.isCurrentTokenType(TokenKind::Function));
>>>>>>> upstream-releases

<<<<<<< HEAD
  GeneratorKind generatorKind = GeneratorKind::NotGenerator;
  if (tt == TokenKind::Mul) {
    generatorKind = GeneratorKind::Generator;
    if (!tokenStream.getToken(&tt)) {
      return null();
    }
  }
||||||| merged common ancestors
        kind = (!pc->sc()->strict() &&
                generatorKind == GeneratorKind::NotGenerator &&
                asyncKind == FunctionAsyncKind::SyncFunction)
                ? DeclarationKind::SloppyLexicalFunction
                : DeclarationKind::LexicalFunction;
    } else {
        kind = pc->atModuleLevel()
               ? DeclarationKind::ModuleBodyLevelFunction
               : DeclarationKind::BodyLevelFunction;
    }
=======
  // In sloppy mode, Annex B.3.2 allows labelled function declarations.
  // Otherwise it's a parse error.
  ParseContext::Statement* declaredInStmt = pc_->innermostStatement();
  if (declaredInStmt && declaredInStmt->kind() == StatementKind::Label) {
    MOZ_ASSERT(!pc_->sc()->strict(),
               "labeled functions shouldn't be parsed in strict mode");
>>>>>>> upstream-releases

<<<<<<< HEAD
  RootedPropertyName name(context);
  if (TokenKindIsPossibleIdentifier(tt)) {
    name = bindingIdentifier(yieldHandling);
    if (!name) {
      return null();
||||||| merged common ancestors
    if (!noteDeclaredName(name, kind, pos())) {
        return null();
=======
    // Find the innermost non-label statement.  Report an error if it's
    // unbraced: functions can't appear in it.  Otherwise the statement
    // (or its absence) determines the scope the function's bound in.
    while (declaredInStmt && declaredInStmt->kind() == StatementKind::Label) {
      declaredInStmt = declaredInStmt->enclosing();
>>>>>>> upstream-releases
    }
  } else if (defaultHandling == AllowDefaultName) {
    name = context->names().default_;
    anyChars.ungetToken();
  } else {
    /* Unnamed function expressions are forbidden in statement context. */
    error(JSMSG_UNNAMED_FUNCTION_STMT);
    return null();
  }

  // Note the declared name and check for early errors.
  DeclarationKind kind;
  if (declaredInStmt) {
    MOZ_ASSERT(declaredInStmt->kind() != StatementKind::Label);
    MOZ_ASSERT(StatementKindIsBraced(declaredInStmt->kind()));

    kind =
        (!pc->sc()->strict() && generatorKind == GeneratorKind::NotGenerator &&
         asyncKind == FunctionAsyncKind::SyncFunction)
            ? DeclarationKind::SloppyLexicalFunction
            : DeclarationKind::LexicalFunction;
  } else {
    kind = pc->atModuleLevel() ? DeclarationKind::ModuleBodyLevelFunction
                               : DeclarationKind::BodyLevelFunction;
  }

  if (!noteDeclaredName(name, kind, pos())) {
    return null();
  }

<<<<<<< HEAD
  CodeNodeType funNode = handler.newFunctionStatement(pos());
  if (!funNode) {
    return null();
  }
||||||| merged common ancestors
    CodeNodeType funNode = handler.newFunctionStatement(pos());
    if (!funNode) {
        return null();
    }
=======
    if (declaredInStmt && !StatementKindIsBraced(declaredInStmt->kind())) {
      error(JSMSG_SLOPPY_FUNCTION_LABEL);
      return null();
    }
  }
>>>>>>> upstream-releases

<<<<<<< HEAD
  // Under sloppy mode, try Annex B.3.3 semantics. If making an additional
  // 'var' binding of the same name does not throw an early error, do so.
  // This 'var' binding would be assigned the function object when its
  // declaration is reached, not at the start of the block.
  //
  // This semantics is implemented upon Scope exit in
  // Scope::propagateAndMarkAnnexBFunctionBoxes.
  bool tryAnnexB = kind == DeclarationKind::SloppyLexicalFunction;

  YieldHandling newYieldHandling = GetYieldHandling(generatorKind);
  return functionDefinition(funNode, toStringStart, InAllowed, newYieldHandling,
                            name, FunctionSyntaxKind::Statement, generatorKind,
                            asyncKind, tryAnnexB);
}

template <class ParseHandler, typename Unit>
typename ParseHandler::CodeNodeType
GeneralParser<ParseHandler, Unit>::functionExpr(uint32_t toStringStart,
                                                InvokedPrediction invoked,
                                                FunctionAsyncKind asyncKind) {
  MOZ_ASSERT(anyChars.isCurrentTokenType(TokenKind::Function));

  AutoAwaitIsKeyword<ParseHandler, Unit> awaitIsKeyword(
      this, GetAwaitHandling(asyncKind));
  GeneratorKind generatorKind = GeneratorKind::NotGenerator;
  TokenKind tt;
  if (!tokenStream.getToken(&tt)) {
    return null();
  }
||||||| merged common ancestors
    // Under sloppy mode, try Annex B.3.3 semantics. If making an additional
    // 'var' binding of the same name does not throw an early error, do so.
    // This 'var' binding would be assigned the function object when its
    // declaration is reached, not at the start of the block.
    //
    // This semantics is implemented upon Scope exit in
    // Scope::propagateAndMarkAnnexBFunctionBoxes.
    bool tryAnnexB = kind == DeclarationKind::SloppyLexicalFunction;

    YieldHandling newYieldHandling = GetYieldHandling(generatorKind);
    return functionDefinition(funNode, toStringStart, InAllowed, newYieldHandling, name,
                              FunctionSyntaxKind::Statement, generatorKind, asyncKind, tryAnnexB);
}

template <class ParseHandler, typename Unit>
typename ParseHandler::CodeNodeType
GeneralParser<ParseHandler, Unit>::functionExpr(uint32_t toStringStart, InvokedPrediction invoked,
                                                FunctionAsyncKind asyncKind)
{
    MOZ_ASSERT(anyChars.isCurrentTokenType(TokenKind::Function));
=======
  TokenKind tt;
  if (!tokenStream.getToken(&tt)) {
    return null();
  }
>>>>>>> upstream-releases

<<<<<<< HEAD
  if (tt == TokenKind::Mul) {
    generatorKind = GeneratorKind::Generator;
||||||| merged common ancestors
    AutoAwaitIsKeyword<ParseHandler, Unit> awaitIsKeyword(this, GetAwaitHandling(asyncKind));
    GeneratorKind generatorKind = GeneratorKind::NotGenerator;
    TokenKind tt;
=======
  GeneratorKind generatorKind = GeneratorKind::NotGenerator;
  if (tt == TokenKind::Mul) {
    generatorKind = GeneratorKind::Generator;
>>>>>>> upstream-releases
    if (!tokenStream.getToken(&tt)) {
<<<<<<< HEAD
      return null();
||||||| merged common ancestors
        return null();
    }

    if (tt == TokenKind::Mul) {
        generatorKind = GeneratorKind::Generator;
        if (!tokenStream.getToken(&tt)) {
            return null();
        }
=======
      return null();
    }
  }

  RootedPropertyName name(cx_);
  if (TokenKindIsPossibleIdentifier(tt)) {
    name = bindingIdentifier(yieldHandling);
    if (!name) {
      return null();
>>>>>>> upstream-releases
    }
<<<<<<< HEAD
  }
||||||| merged common ancestors
=======
  } else if (defaultHandling == AllowDefaultName) {
    name = cx_->names().default_;
    anyChars.ungetToken();
  } else {
    /* Unnamed function expressions are forbidden in statement context. */
    error(JSMSG_UNNAMED_FUNCTION_STMT);
    return null();
  }

  // Note the declared name and check for early errors.
  DeclarationKind kind;
  if (declaredInStmt) {
    MOZ_ASSERT(declaredInStmt->kind() != StatementKind::Label);
    MOZ_ASSERT(StatementKindIsBraced(declaredInStmt->kind()));

    kind =
        (!pc_->sc()->strict() && generatorKind == GeneratorKind::NotGenerator &&
         asyncKind == FunctionAsyncKind::SyncFunction)
            ? DeclarationKind::SloppyLexicalFunction
            : DeclarationKind::LexicalFunction;
  } else {
    kind = pc_->atModuleLevel() ? DeclarationKind::ModuleBodyLevelFunction
                                : DeclarationKind::BodyLevelFunction;
  }

  if (!noteDeclaredName(name, kind, pos())) {
    return null();
  }
>>>>>>> upstream-releases

<<<<<<< HEAD
  YieldHandling yieldHandling = GetYieldHandling(generatorKind);
||||||| merged common ancestors
    YieldHandling yieldHandling = GetYieldHandling(generatorKind);
=======
  FunctionSyntaxKind syntaxKind = FunctionSyntaxKind::Statement;
  FunctionNodeType funNode = handler_.newFunction(syntaxKind, pos());
  if (!funNode) {
    return null();
  }

  // Under sloppy mode, try Annex B.3.3 semantics. If making an additional
  // 'var' binding of the same name does not throw an early error, do so.
  // This 'var' binding would be assigned the function object when its
  // declaration is reached, not at the start of the block.
  //
  // This semantics is implemented upon Scope exit in
  // Scope::propagateAndMarkAnnexBFunctionBoxes.
  bool tryAnnexB = kind == DeclarationKind::SloppyLexicalFunction;

  YieldHandling newYieldHandling = GetYieldHandling(generatorKind);
  return functionDefinition(funNode, toStringStart, InAllowed, newYieldHandling,
                            name, syntaxKind, generatorKind, asyncKind,
                            tryAnnexB);
}

template <class ParseHandler, typename Unit>
typename ParseHandler::FunctionNodeType
GeneralParser<ParseHandler, Unit>::functionExpr(uint32_t toStringStart,
                                                InvokedPrediction invoked,
                                                FunctionAsyncKind asyncKind) {
  MOZ_ASSERT(anyChars.isCurrentTokenType(TokenKind::Function));

  AutoAwaitIsKeyword<ParseHandler, Unit> awaitIsKeyword(
      this, GetAwaitHandling(asyncKind));
  GeneratorKind generatorKind = GeneratorKind::NotGenerator;
  TokenKind tt;
  if (!tokenStream.getToken(&tt)) {
    return null();
  }
>>>>>>> upstream-releases

<<<<<<< HEAD
  RootedPropertyName name(context);
  if (TokenKindIsPossibleIdentifier(tt)) {
    name = bindingIdentifier(yieldHandling);
    if (!name) {
      return null();
||||||| merged common ancestors
    RootedPropertyName name(context);
    if (TokenKindIsPossibleIdentifier(tt)) {
        name = bindingIdentifier(yieldHandling);
        if (!name) {
            return null();
        }
    } else {
        anyChars.ungetToken();
=======
  if (tt == TokenKind::Mul) {
    generatorKind = GeneratorKind::Generator;
    if (!tokenStream.getToken(&tt)) {
      return null();
>>>>>>> upstream-releases
    }
<<<<<<< HEAD
  } else {
    anyChars.ungetToken();
  }
||||||| merged common ancestors
=======
  }
>>>>>>> upstream-releases

<<<<<<< HEAD
  CodeNodeType funNode = handler.newFunctionExpression(pos());
  if (!funNode) {
    return null();
  }
||||||| merged common ancestors
    CodeNodeType funNode = handler.newFunctionExpression(pos());
    if (!funNode) {
        return null();
    }
=======
  YieldHandling yieldHandling = GetYieldHandling(generatorKind);
>>>>>>> upstream-releases

<<<<<<< HEAD
  if (invoked) {
    funNode = handler.setLikelyIIFE(funNode);
  }
||||||| merged common ancestors
    if (invoked) {
        funNode = handler.setLikelyIIFE(funNode);
    }
=======
  RootedPropertyName name(cx_);
  if (TokenKindIsPossibleIdentifier(tt)) {
    name = bindingIdentifier(yieldHandling);
    if (!name) {
      return null();
    }
  } else {
    anyChars.ungetToken();
  }
>>>>>>> upstream-releases

<<<<<<< HEAD
  return functionDefinition(funNode, toStringStart, InAllowed, yieldHandling,
                            name, FunctionSyntaxKind::Expression, generatorKind,
                            asyncKind);
||||||| merged common ancestors
    return functionDefinition(funNode, toStringStart, InAllowed, yieldHandling, name,
                              FunctionSyntaxKind::Expression, generatorKind, asyncKind);
=======
  FunctionSyntaxKind syntaxKind = FunctionSyntaxKind::Expression;
  FunctionNodeType funNode = handler_.newFunction(syntaxKind, pos());
  if (!funNode) {
    return null();
  }

  if (invoked) {
    funNode = handler_.setLikelyIIFE(funNode);
  }

  return functionDefinition(funNode, toStringStart, InAllowed, yieldHandling,
                            name, syntaxKind, generatorKind, asyncKind);
>>>>>>> upstream-releases
}

/*
 * Return true if this node, known to be an unparenthesized string literal,
 * could be the string of a directive in a Directive Prologue. Directive
 * strings never contain escape sequences or line continuations.
 * isEscapeFreeStringLiteral, below, checks whether the node itself could be
 * a directive.
 */
static inline bool IsEscapeFreeStringLiteral(const TokenPos& pos, JSAtom* str) {
  /*
   * If the string's length in the source code is its length as a value,
   * accounting for the quotes, then it must not contain any escape
   * sequences or line continuations.
   */
  return pos.begin + str->length() + 2 == pos.end;
}

template <typename Unit>
<<<<<<< HEAD
bool Parser<SyntaxParseHandler, Unit>::asmJS(ListNodeType list) {
  // While asm.js could technically be validated and compiled during syntax
  // parsing, we have no guarantee that some later JS wouldn't abort the
  // syntax parse and cause us to re-parse (and re-compile) the asm.js module.
  // For simplicity, unconditionally abort the syntax parse when "use asm" is
  // encountered so that asm.js is always validated/compiled exactly once
  // during a full parse.
  MOZ_ALWAYS_FALSE(abortIfSyntaxParser());

  // Record that the current script source constains some AsmJS, to disable
  // any incremental encoder, as AsmJS cannot be encoded with XDR at the
  // moment.
  if (ss) {
    ss->setContainsAsmJS();
  }
  return false;
}

template <>
bool Parser<FullParseHandler, char16_t>::asmJS(ListNodeType list) {
  // Disable syntax parsing in anything nested inside the asm.js module.
  disableSyntaxParser();

  // We should be encountering the "use asm" directive for the first time; if
  // the directive is already, we must have failed asm.js validation and we're
  // reparsing. In that case, don't try to validate again. A non-null
  // newDirectives means we're not in a normal function.
  if (!pc->newDirectives || pc->newDirectives->asmJS()) {
    return true;
  }
||||||| merged common ancestors
bool
Parser<SyntaxParseHandler, Unit>::asmJS(ListNodeType list)
{
    // While asm.js could technically be validated and compiled during syntax
    // parsing, we have no guarantee that some later JS wouldn't abort the
    // syntax parse and cause us to re-parse (and re-compile) the asm.js module.
    // For simplicity, unconditionally abort the syntax parse when "use asm" is
    // encountered so that asm.js is always validated/compiled exactly once
    // during a full parse.
    MOZ_ALWAYS_FALSE(abortIfSyntaxParser());

    // Record that the current script source constains some AsmJS, to disable
    // any incremental encoder, as AsmJS cannot be encoded with XDR at the
    // moment.
    if (ss) {
        ss->setContainsAsmJS();
    }
    return false;
}

template <>
bool
Parser<FullParseHandler, char16_t>::asmJS(ListNodeType list)
{
    // Disable syntax parsing in anything nested inside the asm.js module.
    disableSyntaxParser();

    // We should be encountering the "use asm" directive for the first time; if
    // the directive is already, we must have failed asm.js validation and we're
    // reparsing. In that case, don't try to validate again. A non-null
    // newDirectives means we're not in a normal function.
    if (!pc->newDirectives || pc->newDirectives->asmJS()) {
        return true;
    }

    // If there is no ScriptSource, then we are doing a non-compiling parse and
    // so we shouldn't (and can't, without a ScriptSource) compile.
    if (ss == nullptr) {
        return true;
    }

    ss->setContainsAsmJS();
    pc->functionBox()->useAsm = true;

    // Attempt to validate and compile this asm.js module. On success, the
    // tokenStream has been advanced to the closing }. On failure, the
    // tokenStream is in an indeterminate state and we must reparse the
    // function from the beginning. Reparsing is triggered by marking that a
    // new directive has been encountered and returning 'false'.
    bool validated;
    if (!CompileAsmJS(context, *this, list, &validated)) {
        return false;
    }
    if (!validated) {
        pc->newDirectives->setAsmJS();
        return false;
    }
=======
bool Parser<SyntaxParseHandler, Unit>::asmJS(ListNodeType list) {
  // While asm.js could technically be validated and compiled during syntax
  // parsing, we have no guarantee that some later JS wouldn't abort the
  // syntax parse and cause us to re-parse (and re-compile) the asm.js module.
  // For simplicity, unconditionally abort the syntax parse when "use asm" is
  // encountered so that asm.js is always validated/compiled exactly once
  // during a full parse.
  MOZ_ALWAYS_FALSE(abortIfSyntaxParser());

  // Record that the current script source constains some AsmJS, to disable
  // any incremental encoder, as AsmJS cannot be encoded with XDR at the
  // moment.
  if (ss) {
    ss->setContainsAsmJS();
  }
  return false;
}
>>>>>>> upstream-releases

<<<<<<< HEAD
  // If there is no ScriptSource, then we are doing a non-compiling parse and
  // so we shouldn't (and can't, without a ScriptSource) compile.
  if (ss == nullptr) {
||||||| merged common ancestors
=======
template <typename Unit>
bool Parser<FullParseHandler, Unit>::asmJS(ListNodeType list) {
  // Disable syntax parsing in anything nested inside the asm.js module.
  disableSyntaxParser();

  // We should be encountering the "use asm" directive for the first time; if
  // the directive is already, we must have failed asm.js validation and we're
  // reparsing. In that case, don't try to validate again. A non-null
  // newDirectives means we're not in a normal function.
  if (!pc_->newDirectives || pc_->newDirectives->asmJS()) {
>>>>>>> upstream-releases
    return true;
<<<<<<< HEAD
  }

  ss->setContainsAsmJS();
  pc->functionBox()->useAsm = true;

  // Attempt to validate and compile this asm.js module. On success, the
  // tokenStream has been advanced to the closing }. On failure, the
  // tokenStream is in an indeterminate state and we must reparse the
  // function from the beginning. Reparsing is triggered by marking that a
  // new directive has been encountered and returning 'false'.
  bool validated;
  if (!CompileAsmJS(context, *this, list, &validated)) {
    return false;
  }
  if (!validated) {
    pc->newDirectives->setAsmJS();
    return false;
  }

  return true;
}
||||||| merged common ancestors
}
=======
  }
>>>>>>> upstream-releases

<<<<<<< HEAD
template <>
bool Parser<FullParseHandler, Utf8Unit>::asmJS(ListNodeType list) {
  // Just succeed without setting the asm.js directive flag.  Given Web
  // Assembly's rapid advance, it's probably not worth the trouble to really
  // support UTF-8 asm.js.
  return true;
||||||| merged common ancestors
template <>
bool
Parser<FullParseHandler, Utf8Unit>::asmJS(ListNodeType list)
{
    // Just succeed without setting the asm.js directive flag.  Given Web
    // Assembly's rapid advance, it's probably not worth the trouble to really
    // support UTF-8 asm.js.
    return true;
=======
  // If there is no ScriptSource, then we are doing a non-compiling parse and
  // so we shouldn't (and can't, without a ScriptSource) compile.
  if (ss == nullptr) {
    return true;
  }

  ss->setContainsAsmJS();
  pc_->functionBox()->useAsm = true;

  // Attempt to validate and compile this asm.js module. On success, the
  // tokenStream has been advanced to the closing }. On failure, the
  // tokenStream is in an indeterminate state and we must reparse the
  // function from the beginning. Reparsing is triggered by marking that a
  // new directive has been encountered and returning 'false'.
  bool validated;
  if (!CompileAsmJS(cx_, *this, list, &validated)) {
    return false;
  }
  if (!validated) {
    pc_->newDirectives->setAsmJS();
    return false;
  }

  return true;
>>>>>>> upstream-releases
}

template <class ParseHandler, typename Unit>
inline bool GeneralParser<ParseHandler, Unit>::asmJS(ListNodeType list) {
  return asFinalParser()->asmJS(list);
}

/*
 * Recognize Directive Prologue members and directives. Assuming |pn| is a
 * candidate for membership in a directive prologue, recognize directives and
 * set |pc_|'s flags accordingly. If |pn| is indeed part of a prologue, set its
 * |prologue| flag.
 *
 * Note that the following is a strict mode function:
 *
 * function foo() {
 *   "blah" // inserted semi colon
 *        "blurgh"
 *   "use\x20loose"
 *   "use strict"
 * }
 *
 * That is, even though "use\x20loose" can never be a directive, now or in the
 * future (because of the hex escape), the Directive Prologue extends through it
 * to the "use strict" statement, which is indeed a directive.
 */
template <class ParseHandler, typename Unit>
<<<<<<< HEAD
bool GeneralParser<ParseHandler, Unit>::maybeParseDirective(
    ListNodeType list, Node possibleDirective, bool* cont) {
  TokenPos directivePos;
  JSAtom* directive =
      handler.isStringExprStatement(possibleDirective, &directivePos);
||||||| merged common ancestors
bool
GeneralParser<ParseHandler, Unit>::maybeParseDirective(ListNodeType list, Node possibleDirective,
                                                       bool* cont)
{
    TokenPos directivePos;
    JSAtom* directive = handler.isStringExprStatement(possibleDirective, &directivePos);

    *cont = !!directive;
    if (!*cont) {
        return true;
    }
=======
bool GeneralParser<ParseHandler, Unit>::maybeParseDirective(
    ListNodeType list, Node possibleDirective, bool* cont) {
  TokenPos directivePos;
  JSAtom* directive =
      handler_.isStringExprStatement(possibleDirective, &directivePos);
>>>>>>> upstream-releases

  *cont = !!directive;
  if (!*cont) {
    return true;
  }

<<<<<<< HEAD
  if (IsEscapeFreeStringLiteral(directivePos, directive)) {
    // Mark this statement as being a possibly legitimate part of a
    // directive prologue, so the bytecode emitter won't warn about it being
    // useless code. (We mustn't just omit the statement entirely yet, as it
    // could be producing the value of an eval or JSScript execution.)
    //
    // Note that even if the string isn't one we recognize as a directive,
    // the emitter still shouldn't flag it as useless, as it could become a
    // directive in the future. We don't want to interfere with people
    // taking advantage of directive-prologue-enabled features that appear
    // in other browsers first.
    handler.setInDirectivePrologue(handler.asUnary(possibleDirective));

    if (directive == context->names().useStrict) {
      // Functions with non-simple parameter lists (destructuring,
      // default or rest parameters) must not contain a "use strict"
      // directive.
      if (pc->isFunctionBox()) {
        FunctionBox* funbox = pc->functionBox();
        if (!funbox->hasSimpleParameterList()) {
          const char* parameterKind =
              funbox->hasDestructuringArgs
                  ? "destructuring"
                  : funbox->hasParameterExprs ? "default" : "rest";
          errorAt(directivePos.begin, JSMSG_STRICT_NON_SIMPLE_PARAMS,
                  parameterKind);
          return false;
||||||| merged common ancestors
            // We're going to be in strict mode. Note that this scope explicitly
            // had "use strict";
            pc->sc()->setExplicitUseStrict();
            if (!pc->sc()->strict()) {
                // We keep track of the one possible strict violation that could
                // occur in the directive prologue -- octal escapes -- and
                // complain now.
                if (anyChars.sawOctalEscape()) {
                    error(JSMSG_DEPRECATED_OCTAL);
                    return false;
                }
                pc->sc()->strictScript = true;
            }
        } else if (directive == context->names().useAsm) {
            if (pc->isFunctionBox()) {
                return asmJS(list);
            }
            return warningAt(directivePos.begin, JSMSG_USE_ASM_DIRECTIVE_FAIL);
=======
  if (IsEscapeFreeStringLiteral(directivePos, directive)) {
    // Mark this statement as being a possibly legitimate part of a
    // directive prologue, so the bytecode emitter won't warn about it being
    // useless code. (We mustn't just omit the statement entirely yet, as it
    // could be producing the value of an eval or JSScript execution.)
    //
    // Note that even if the string isn't one we recognize as a directive,
    // the emitter still shouldn't flag it as useless, as it could become a
    // directive in the future. We don't want to interfere with people
    // taking advantage of directive-prologue-enabled features that appear
    // in other browsers first.
    handler_.setInDirectivePrologue(handler_.asUnary(possibleDirective));

    if (directive == cx_->names().useStrict) {
      // Functions with non-simple parameter lists (destructuring,
      // default or rest parameters) must not contain a "use strict"
      // directive.
      if (pc_->isFunctionBox()) {
        FunctionBox* funbox = pc_->functionBox();
        if (!funbox->hasSimpleParameterList()) {
          const char* parameterKind =
              funbox->hasDestructuringArgs
                  ? "destructuring"
                  : funbox->hasParameterExprs ? "default" : "rest";
          errorAt(directivePos.begin, JSMSG_STRICT_NON_SIMPLE_PARAMS,
                  parameterKind);
          return false;
>>>>>>> upstream-releases
        }
<<<<<<< HEAD
      }

      // We're going to be in strict mode. Note that this scope explicitly
      // had "use strict";
      pc->sc()->setExplicitUseStrict();
      if (!pc->sc()->strict()) {
        // We keep track of the one possible strict violation that could
        // occur in the directive prologue -- octal escapes -- and
        // complain now.
        if (anyChars.sawOctalEscape()) {
          error(JSMSG_DEPRECATED_OCTAL);
          return false;
        }
        pc->sc()->strictScript = true;
      }
    } else if (directive == context->names().useAsm) {
      if (pc->isFunctionBox()) {
        return asmJS(list);
      }
      return warningAt(directivePos.begin, JSMSG_USE_ASM_DIRECTIVE_FAIL);
||||||| merged common ancestors
=======
      }

      // We're going to be in strict mode. Note that this scope explicitly
      // had "use strict";
      pc_->sc()->setExplicitUseStrict();
      if (!pc_->sc()->strict()) {
        // We keep track of the one possible strict violation that could
        // occur in the directive prologue -- octal escapes -- and
        // complain now.
        if (anyChars.sawOctalEscape()) {
          error(JSMSG_DEPRECATED_OCTAL);
          return false;
        }
        pc_->sc()->strictScript = true;
      }
    } else if (directive == cx_->names().useAsm) {
      if (pc_->isFunctionBox()) {
        return asmJS(list);
      }
      return warningAt(directivePos.begin, JSMSG_USE_ASM_DIRECTIVE_FAIL);
>>>>>>> upstream-releases
    }
  }
  return true;
}

template <class ParseHandler, typename Unit>
typename ParseHandler::ListNodeType
<<<<<<< HEAD
GeneralParser<ParseHandler, Unit>::statementList(YieldHandling yieldHandling) {
  if (!CheckRecursionLimit(context)) {
    return null();
  }

  ListNodeType stmtList = handler.newStatementList(pos());
  if (!stmtList) {
    return null();
  }

  bool canHaveDirectives = pc->atBodyLevel();
  if (canHaveDirectives) {
    anyChars.clearSawOctalEscape();
  }
  bool afterReturn = false;
  bool warnedAboutStatementsAfterReturn = false;
  uint32_t statementBegin = 0;
  for (;;) {
    TokenKind tt = TokenKind::Eof;
    if (!tokenStream.peekToken(&tt, TokenStream::Operand)) {
      if (anyChars.isEOF()) {
        isUnexpectedEOF_ = true;
      }
      return null();
    }
    if (tt == TokenKind::Eof || tt == TokenKind::RightCurly) {
      TokenPos pos;
      if (!tokenStream.peekTokenPos(&pos, TokenStream::Operand)) {
||||||| merged common ancestors
GeneralParser<ParseHandler, Unit>::statementList(YieldHandling yieldHandling)
{
    if (!CheckRecursionLimit(context)) {
=======
GeneralParser<ParseHandler, Unit>::statementList(YieldHandling yieldHandling) {
  if (!CheckRecursionLimit(cx_)) {
    return null();
  }

  ListNodeType stmtList = handler_.newStatementList(pos());
  if (!stmtList) {
    return null();
  }

  bool canHaveDirectives = pc_->atBodyLevel();
  if (canHaveDirectives) {
    anyChars.clearSawOctalEscape();
  }

  bool canHaveHashbangComment = pc_->atTopLevel();
  if (canHaveHashbangComment) {
    tokenStream.consumeOptionalHashbangComment();
  }

  bool afterReturn = false;
  bool warnedAboutStatementsAfterReturn = false;
  uint32_t statementBegin = 0;
  for (;;) {
    TokenKind tt = TokenKind::Eof;
    if (!tokenStream.peekToken(&tt, TokenStream::SlashIsRegExp)) {
      if (anyChars.isEOF()) {
        isUnexpectedEOF_ = true;
      }
      return null();
    }
    if (tt == TokenKind::Eof || tt == TokenKind::RightCurly) {
      TokenPos pos;
      if (!tokenStream.peekTokenPos(&pos, TokenStream::SlashIsRegExp)) {
>>>>>>> upstream-releases
        return null();
<<<<<<< HEAD
      }
      handler.setListEndPosition(stmtList, pos);
      break;
||||||| merged common ancestors
=======
      }
      handler_.setListEndPosition(stmtList, pos);
      break;
>>>>>>> upstream-releases
    }
<<<<<<< HEAD
    if (afterReturn) {
      if (!tokenStream.peekOffset(&statementBegin, TokenStream::Operand)) {
||||||| merged common ancestors

    ListNodeType stmtList = handler.newStatementList(pos());
    if (!stmtList) {
=======
    if (afterReturn) {
      if (!tokenStream.peekOffset(&statementBegin,
                                  TokenStream::SlashIsRegExp)) {
>>>>>>> upstream-releases
        return null();
      }
    }
    Node next = statementListItem(yieldHandling, canHaveDirectives);
    if (!next) {
      if (anyChars.isEOF()) {
        isUnexpectedEOF_ = true;
      }
      return null();
    }
<<<<<<< HEAD
    if (!warnedAboutStatementsAfterReturn) {
      if (afterReturn) {
        if (!handler.isStatementPermittedAfterReturnStatement(next)) {
          if (!warningAt(statementBegin, JSMSG_STMT_AFTER_RETURN)) {
||||||| merged common ancestors
    bool afterReturn = false;
    bool warnedAboutStatementsAfterReturn = false;
    uint32_t statementBegin = 0;
    for (;;) {
        TokenKind tt = TokenKind::Eof;
        if (!tokenStream.peekToken(&tt, TokenStream::Operand)) {
            if (anyChars.isEOF()) {
                isUnexpectedEOF_ = true;
            }
            return null();
        }
        if (tt == TokenKind::Eof || tt == TokenKind::RightCurly) {
            TokenPos pos;
            if (!tokenStream.peekTokenPos(&pos, TokenStream::Operand)) {
                return null();
            }
            handler.setListEndPosition(stmtList, pos);
            break;
        }
        if (afterReturn) {
            if (!tokenStream.peekOffset(&statementBegin, TokenStream::Operand)) {
                return null();
            }
        }
        Node next = statementListItem(yieldHandling, canHaveDirectives);
        if (!next) {
            if (anyChars.isEOF()) {
                isUnexpectedEOF_ = true;
            }
=======
    if (!warnedAboutStatementsAfterReturn) {
      if (afterReturn) {
        if (!handler_.isStatementPermittedAfterReturnStatement(next)) {
          if (!warningAt(statementBegin, JSMSG_STMT_AFTER_RETURN)) {
>>>>>>> upstream-releases
            return null();
          }

          warnedAboutStatementsAfterReturn = true;
        }
<<<<<<< HEAD
      } else if (handler.isReturnStatement(next)) {
        afterReturn = true;
      }
    }
||||||| merged common ancestors
=======
      } else if (handler_.isReturnStatement(next)) {
        afterReturn = true;
      }
    }
>>>>>>> upstream-releases

    if (canHaveDirectives) {
      if (!maybeParseDirective(stmtList, next, &canHaveDirectives)) {
        return null();
      }
    }

<<<<<<< HEAD
    handler.addStatementToList(stmtList, next);
  }

  return stmtList;
||||||| merged common ancestors
    return stmtList;
=======
    handler_.addStatementToList(stmtList, next);
  }

  return stmtList;
>>>>>>> upstream-releases
}

template <class ParseHandler, typename Unit>
typename ParseHandler::Node GeneralParser<ParseHandler, Unit>::condition(
    InHandling inHandling, YieldHandling yieldHandling) {
  if (!mustMatchToken(TokenKind::LeftParen, JSMSG_PAREN_BEFORE_COND)) {
    return null();
  }

  Node pn = exprInParens(inHandling, yieldHandling, TripledotProhibited);
  if (!pn) {
    return null();
  }

<<<<<<< HEAD
  if (!mustMatchToken(TokenKind::RightParen, TokenStream::Operand,
                      JSMSG_PAREN_AFTER_COND)) {
    return null();
  }
||||||| merged common ancestors
    MUST_MATCH_TOKEN_MOD(TokenKind::RightParen, TokenStream::Operand, JSMSG_PAREN_AFTER_COND);
=======
  if (!mustMatchToken(TokenKind::RightParen, JSMSG_PAREN_AFTER_COND)) {
    return null();
  }
>>>>>>> upstream-releases

<<<<<<< HEAD
  /* Check for (a = b) and warn about possible (a == b) mistype. */
  if (handler.isUnparenthesizedAssignment(pn)) {
    if (!extraWarning(JSMSG_EQUAL_AS_ASSIGN)) {
      return null();
||||||| merged common ancestors
    /* Check for (a = b) and warn about possible (a == b) mistype. */
    if (handler.isUnparenthesizedAssignment(pn)) {
        if (!extraWarning(JSMSG_EQUAL_AS_ASSIGN)) {
            return null();
        }
=======
  /* Check for (a = b) and warn about possible (a == b) mistype. */
  if (handler_.isUnparenthesizedAssignment(pn)) {
    if (!extraWarning(JSMSG_EQUAL_AS_ASSIGN)) {
      return null();
>>>>>>> upstream-releases
    }
  }
  return pn;
}

template <class ParseHandler, typename Unit>
<<<<<<< HEAD
bool GeneralParser<ParseHandler, Unit>::matchLabel(
    YieldHandling yieldHandling, MutableHandle<PropertyName*> label) {
  TokenKind tt = TokenKind::Eof;
  if (!tokenStream.peekTokenSameLine(&tt, TokenStream::Operand)) {
    return false;
  }
||||||| merged common ancestors
bool
GeneralParser<ParseHandler, Unit>::matchLabel(YieldHandling yieldHandling,
                                              MutableHandle<PropertyName*> label)
{
    TokenKind tt = TokenKind::Eof;
    if (!tokenStream.peekTokenSameLine(&tt, TokenStream::Operand)) {
        return false;
    }
=======
bool GeneralParser<ParseHandler, Unit>::matchLabel(
    YieldHandling yieldHandling, MutableHandle<PropertyName*> label) {
  TokenKind tt = TokenKind::Eof;
  if (!tokenStream.peekTokenSameLine(&tt, TokenStream::SlashIsRegExp)) {
    return false;
  }
>>>>>>> upstream-releases

<<<<<<< HEAD
  if (TokenKindIsPossibleIdentifier(tt)) {
    tokenStream.consumeKnownToken(tt, TokenStream::Operand);
||||||| merged common ancestors
    if (TokenKindIsPossibleIdentifier(tt)) {
        tokenStream.consumeKnownToken(tt, TokenStream::Operand);
=======
  if (TokenKindIsPossibleIdentifier(tt)) {
    tokenStream.consumeKnownToken(tt, TokenStream::SlashIsRegExp);
>>>>>>> upstream-releases

    label.set(labelIdentifier(yieldHandling));
    if (!label) {
      return false;
    }
  } else {
    label.set(nullptr);
  }
  return true;
}

template <class ParseHandler, typename Unit>
GeneralParser<ParseHandler, Unit>::PossibleError::PossibleError(
    GeneralParser<ParseHandler, Unit>& parser)
    : parser_(parser) {}

template <class ParseHandler, typename Unit>
typename GeneralParser<ParseHandler, Unit>::PossibleError::Error&
GeneralParser<ParseHandler, Unit>::PossibleError::error(ErrorKind kind) {
  if (kind == ErrorKind::Expression) {
    return exprError_;
  }
  if (kind == ErrorKind::Destructuring) {
    return destructuringError_;
  }
  MOZ_ASSERT(kind == ErrorKind::DestructuringWarning);
  return destructuringWarning_;
}

template <class ParseHandler, typename Unit>
void GeneralParser<ParseHandler, Unit>::PossibleError::setResolved(
    ErrorKind kind) {
  error(kind).state_ = ErrorState::None;
}

template <class ParseHandler, typename Unit>
bool GeneralParser<ParseHandler, Unit>::PossibleError::hasError(
    ErrorKind kind) {
  return error(kind).state_ == ErrorState::Pending;
}

template <class ParseHandler, typename Unit>
bool GeneralParser<ParseHandler,
                   Unit>::PossibleError::hasPendingDestructuringError() {
  return hasError(ErrorKind::Destructuring);
}

template <class ParseHandler, typename Unit>
void GeneralParser<ParseHandler, Unit>::PossibleError::setPending(
    ErrorKind kind, const TokenPos& pos, unsigned errorNumber) {
  // Don't overwrite a previously recorded error.
  if (hasError(kind)) {
    return;
  }

  // If we report an error later, we'll do it from the position where we set
  // the state to pending.
  Error& err = error(kind);
  err.offset_ = pos.begin;
  err.errorNumber_ = errorNumber;
  err.state_ = ErrorState::Pending;
}

template <class ParseHandler, typename Unit>
void GeneralParser<ParseHandler, Unit>::PossibleError::
    setPendingDestructuringErrorAt(const TokenPos& pos, unsigned errorNumber) {
  setPending(ErrorKind::Destructuring, pos, errorNumber);
}

template <class ParseHandler, typename Unit>
void GeneralParser<ParseHandler, Unit>::PossibleError::
    setPendingDestructuringWarningAt(const TokenPos& pos,
                                     unsigned errorNumber) {
  setPending(ErrorKind::DestructuringWarning, pos, errorNumber);
}

template <class ParseHandler, typename Unit>
void GeneralParser<ParseHandler, Unit>::PossibleError::
    setPendingExpressionErrorAt(const TokenPos& pos, unsigned errorNumber) {
  setPending(ErrorKind::Expression, pos, errorNumber);
}

template <class ParseHandler, typename Unit>
bool GeneralParser<ParseHandler, Unit>::PossibleError::checkForError(
    ErrorKind kind) {
  if (!hasError(kind)) {
    return true;
  }

  Error& err = error(kind);
  parser_.errorAt(err.offset_, err.errorNumber_);
  return false;
}

template <class ParseHandler, typename Unit>
bool GeneralParser<ParseHandler, Unit>::PossibleError::checkForWarning(
    ErrorKind kind) {
  if (!hasError(kind)) {
    return true;
  }

  Error& err = error(kind);
  return parser_.extraWarningAt(err.offset_, err.errorNumber_);
}

template <class ParseHandler, typename Unit>
bool GeneralParser<ParseHandler,
                   Unit>::PossibleError::checkForDestructuringErrorOrWarning() {
  // Clear pending expression error, because we're definitely not in an
  // expression context.
  setResolved(ErrorKind::Expression);

  // Report any pending destructuring error or warning.
  return checkForError(ErrorKind::Destructuring) &&
         checkForWarning(ErrorKind::DestructuringWarning);
}

template <class ParseHandler, typename Unit>
bool GeneralParser<ParseHandler,
                   Unit>::PossibleError::checkForExpressionError() {
  // Clear pending destructuring error or warning, because we're definitely
  // not in a destructuring context.
  setResolved(ErrorKind::Destructuring);
  setResolved(ErrorKind::DestructuringWarning);

  // Report any pending expression error.
  return checkForError(ErrorKind::Expression);
}

template <class ParseHandler, typename Unit>
void GeneralParser<ParseHandler, Unit>::PossibleError::transferErrorTo(
    ErrorKind kind, PossibleError* other) {
  if (hasError(kind) && !other->hasError(kind)) {
    Error& err = error(kind);
    Error& otherErr = other->error(kind);
    otherErr.offset_ = err.offset_;
    otherErr.errorNumber_ = err.errorNumber_;
    otherErr.state_ = err.state_;
  }
}

template <class ParseHandler, typename Unit>
void GeneralParser<ParseHandler, Unit>::PossibleError::transferErrorsTo(
    PossibleError* other) {
  MOZ_ASSERT(other);
  MOZ_ASSERT(this != other);
  MOZ_ASSERT(&parser_ == &other->parser_,
             "Can't transfer fields to an instance which belongs to a "
             "different parser");

  transferErrorTo(ErrorKind::Destructuring, other);
  transferErrorTo(ErrorKind::Expression, other);
}

template <class ParseHandler, typename Unit>
typename ParseHandler::BinaryNodeType
GeneralParser<ParseHandler, Unit>::bindingInitializer(
    Node lhs, DeclarationKind kind, YieldHandling yieldHandling) {
  MOZ_ASSERT(anyChars.isCurrentTokenType(TokenKind::Assign));

<<<<<<< HEAD
  if (kind == DeclarationKind::FormalParameter) {
    pc->functionBox()->hasParameterExprs = true;
  }

  Node rhs = assignExpr(InAllowed, yieldHandling, TripledotProhibited);
  if (!rhs) {
    return null();
  }
||||||| merged common ancestors
    if (kind == DeclarationKind::FormalParameter) {
        pc->functionBox()->hasParameterExprs = true;
    }

    Node rhs = assignExpr(InAllowed, yieldHandling, TripledotProhibited);
    if (!rhs) {
        return null();
    }
=======
  if (kind == DeclarationKind::FormalParameter) {
    pc_->functionBox()->hasParameterExprs = true;
  }
>>>>>>> upstream-releases

<<<<<<< HEAD
  BinaryNodeType assign =
      handler.newAssignment(ParseNodeKind::Assign, lhs, rhs);
  if (!assign) {
    return null();
  }
||||||| merged common ancestors
    BinaryNodeType assign = handler.newAssignment(ParseNodeKind::Assign, lhs, rhs);
    if (!assign) {
        return null();
    }
=======
  Node rhs = assignExpr(InAllowed, yieldHandling, TripledotProhibited);
  if (!rhs) {
    return null();
  }
>>>>>>> upstream-releases

<<<<<<< HEAD
  if (foldConstants) {
    Node node = assign;
    if (!FoldConstants(context, &node, this)) {
      return null();
    }
    assign = handler.asBinary(node);
  }
||||||| merged common ancestors
    if (foldConstants) {
        Node node = assign;
        if (!FoldConstants(context, &node, this)) {
            return null();
        }
        assign = handler.asBinary(node);
    }
=======
  BinaryNodeType assign =
      handler_.newAssignment(ParseNodeKind::AssignExpr, lhs, rhs);
  if (!assign) {
    return null();
  }
>>>>>>> upstream-releases

  return assign;
}

template <class ParseHandler, typename Unit>
typename ParseHandler::NameNodeType
<<<<<<< HEAD
GeneralParser<ParseHandler, Unit>::bindingIdentifier(
    DeclarationKind kind, YieldHandling yieldHandling) {
  RootedPropertyName name(context, bindingIdentifier(yieldHandling));
  if (!name) {
    return null();
  }
||||||| merged common ancestors
GeneralParser<ParseHandler, Unit>::bindingIdentifier(DeclarationKind kind,
                                                     YieldHandling yieldHandling)
{
    RootedPropertyName name(context, bindingIdentifier(yieldHandling));
    if (!name) {
        return null();
    }
=======
GeneralParser<ParseHandler, Unit>::bindingIdentifier(
    DeclarationKind kind, YieldHandling yieldHandling) {
  RootedPropertyName name(cx_, bindingIdentifier(yieldHandling));
  if (!name) {
    return null();
  }
>>>>>>> upstream-releases

  NameNodeType binding = newName(name);
  if (!binding || !noteDeclaredName(name, kind, pos())) {
    return null();
  }

  return binding;
}

template <class ParseHandler, typename Unit>
typename ParseHandler::Node
<<<<<<< HEAD
GeneralParser<ParseHandler, Unit>::bindingIdentifierOrPattern(
    DeclarationKind kind, YieldHandling yieldHandling, TokenKind tt) {
  if (tt == TokenKind::LeftBracket) {
    return arrayBindingPattern(kind, yieldHandling);
  }

  if (tt == TokenKind::LeftCurly) {
    return objectBindingPattern(kind, yieldHandling);
  }

  if (!TokenKindIsPossibleIdentifierName(tt)) {
    error(JSMSG_NO_VARIABLE_NAME);
    return null();
  }
||||||| merged common ancestors
GeneralParser<ParseHandler, Unit>::bindingIdentifierOrPattern(DeclarationKind kind,
                                                              YieldHandling yieldHandling,
                                                              TokenKind tt)
{
    if (tt == TokenKind::LeftBracket) {
        return arrayBindingPattern(kind, yieldHandling);
    }
=======
GeneralParser<ParseHandler, Unit>::bindingIdentifierOrPattern(
    DeclarationKind kind, YieldHandling yieldHandling, TokenKind tt) {
  if (tt == TokenKind::LeftBracket) {
    return arrayBindingPattern(kind, yieldHandling);
  }

  if (tt == TokenKind::LeftCurly) {
    return objectBindingPattern(kind, yieldHandling);
  }

  if (!TokenKindIsPossibleIdentifierName(tt)) {
    error(JSMSG_NO_VARIABLE_NAME);
    return null();
  }

  return bindingIdentifier(kind, yieldHandling);
}

template <class ParseHandler, typename Unit>
typename ParseHandler::ListNodeType
GeneralParser<ParseHandler, Unit>::objectBindingPattern(
    DeclarationKind kind, YieldHandling yieldHandling) {
  MOZ_ASSERT(anyChars.isCurrentTokenType(TokenKind::LeftCurly));

  if (!CheckRecursionLimit(cx_)) {
    return null();
  }

  uint32_t begin = pos().begin;
  ListNodeType literal = handler_.newObjectLiteral(begin);
  if (!literal) {
    return null();
  }
>>>>>>> upstream-releases

<<<<<<< HEAD
  return bindingIdentifier(kind, yieldHandling);
}

template <class ParseHandler, typename Unit>
typename ParseHandler::ListNodeType
GeneralParser<ParseHandler, Unit>::objectBindingPattern(
    DeclarationKind kind, YieldHandling yieldHandling) {
  MOZ_ASSERT(anyChars.isCurrentTokenType(TokenKind::LeftCurly));

  if (!CheckRecursionLimit(context)) {
    return null();
  }

  uint32_t begin = pos().begin;
  ListNodeType literal = handler.newObjectLiteral(begin);
  if (!literal) {
    return null();
  }

  Maybe<DeclarationKind> declKind = Some(kind);
  RootedAtom propAtom(context);
  for (;;) {
    TokenKind tt;
    if (!tokenStream.peekToken(&tt)) {
      return null();
    }
    if (tt == TokenKind::RightCurly) {
      anyChars.addModifierException(TokenStream::OperandIsNone);
      break;
||||||| merged common ancestors
    if (tt == TokenKind::LeftCurly) {
        return objectBindingPattern(kind, yieldHandling);
=======
  Maybe<DeclarationKind> declKind = Some(kind);
  RootedAtom propAtom(cx_);
  for (;;) {
    TokenKind tt;
    if (!tokenStream.peekToken(&tt)) {
      return null();
    }
    if (tt == TokenKind::RightCurly) {
      break;
>>>>>>> upstream-releases
    }

    if (tt == TokenKind::TripleDot) {
      tokenStream.consumeKnownToken(TokenKind::TripleDot);
      uint32_t begin = pos().begin;

      TokenKind tt;
      if (!tokenStream.getToken(&tt)) {
        return null();
      }

      if (!TokenKindIsPossibleIdentifierName(tt)) {
        error(JSMSG_NO_VARIABLE_NAME);
        return null();
      }

      NameNodeType inner = bindingIdentifier(kind, yieldHandling);
      if (!inner) {
        return null();
      }

<<<<<<< HEAD
      if (!handler.addSpreadProperty(literal, begin, inner)) {
||||||| merged common ancestors
    if (!CheckRecursionLimit(context)) {
=======
      if (!handler_.addSpreadProperty(literal, begin, inner)) {
>>>>>>> upstream-releases
        return null();
      }
    } else {
      TokenPos namePos = anyChars.nextToken().pos;

<<<<<<< HEAD
      PropertyType propType;
      Node propName = propertyName(yieldHandling, PropertyNameInPattern,
                                   declKind, literal, &propType, &propAtom);
      if (!propName) {
||||||| merged common ancestors
    uint32_t begin = pos().begin;
    ListNodeType literal = handler.newObjectLiteral(begin);
    if (!literal) {
=======
      PropertyType propType;
      Node propName =
          propertyOrMethodName(yieldHandling, PropertyNameInPattern, declKind,
                               literal, &propType, &propAtom);
      if (!propName) {
>>>>>>> upstream-releases
        return null();
      }

<<<<<<< HEAD
      if (propType == PropertyType::Normal) {
        // Handle e.g., |var {p: x} = o| and |var {p: x=0} = o|.

        if (!tokenStream.getToken(&tt, TokenStream::Operand)) {
          return null();
||||||| merged common ancestors
    Maybe<DeclarationKind> declKind = Some(kind);
    RootedAtom propAtom(context);
    for (;;) {
        TokenKind tt;
        if (!tokenStream.peekToken(&tt)) {
            return null();
=======
      if (propType == PropertyType::Normal) {
        // Handle e.g., |var {p: x} = o| and |var {p: x=0} = o|.

        if (!tokenStream.getToken(&tt, TokenStream::SlashIsRegExp)) {
          return null();
>>>>>>> upstream-releases
        }

        Node binding = bindingIdentifierOrPattern(kind, yieldHandling, tt);
        if (!binding) {
          return null();
        }

<<<<<<< HEAD
        bool hasInitializer;
        if (!tokenStream.matchToken(&hasInitializer, TokenKind::Assign,
                                    TokenStream::Operand)) {
          return null();
        }
||||||| merged common ancestors
        if (tt == TokenKind::TripleDot) {
            tokenStream.consumeKnownToken(TokenKind::TripleDot);
            uint32_t begin = pos().begin;
=======
        bool hasInitializer;
        if (!tokenStream.matchToken(&hasInitializer, TokenKind::Assign,
                                    TokenStream::SlashIsRegExp)) {
          return null();
        }
>>>>>>> upstream-releases

        Node bindingExpr =
            hasInitializer ? bindingInitializer(binding, kind, yieldHandling)
                           : binding;
        if (!bindingExpr) {
          return null();
        }

<<<<<<< HEAD
        if (!handler.addPropertyDefinition(literal, propName, bindingExpr)) {
          return null();
        }
      } else if (propType == PropertyType::Shorthand) {
        // Handle e.g., |var {x, y} = o| as destructuring shorthand
        // for |var {x: x, y: y} = o|.
        MOZ_ASSERT(TokenKindIsPossibleIdentifierName(tt));
||||||| merged common ancestors
            if (!TokenKindIsPossibleIdentifierName(tt)) {
                error(JSMSG_NO_VARIABLE_NAME);
                return null();
            }
=======
        if (!handler_.addPropertyDefinition(literal, propName, bindingExpr)) {
          return null();
        }
      } else if (propType == PropertyType::Shorthand) {
        // Handle e.g., |var {x, y} = o| as destructuring shorthand
        // for |var {x: x, y: y} = o|.
        MOZ_ASSERT(TokenKindIsPossibleIdentifierName(tt));
>>>>>>> upstream-releases

        NameNodeType binding = bindingIdentifier(kind, yieldHandling);
        if (!binding) {
          return null();
        }

<<<<<<< HEAD
        if (!handler.addShorthand(literal, handler.asName(propName), binding)) {
          return null();
        }
      } else if (propType == PropertyType::CoverInitializedName) {
        // Handle e.g., |var {x=1, y=2} = o| as destructuring
        // shorthand with default values.
        MOZ_ASSERT(TokenKindIsPossibleIdentifierName(tt));
||||||| merged common ancestors
            if (!handler.addSpreadProperty(literal, begin, inner)) {
                return null();
            }
        } else {
            TokenPos namePos = anyChars.nextToken().pos;
=======
        if (!handler_.addShorthand(literal, handler_.asName(propName),
                                   binding)) {
          return null();
        }
      } else if (propType == PropertyType::CoverInitializedName) {
        // Handle e.g., |var {x=1, y=2} = o| as destructuring
        // shorthand with default values.
        MOZ_ASSERT(TokenKindIsPossibleIdentifierName(tt));
>>>>>>> upstream-releases

<<<<<<< HEAD
        NameNodeType binding = bindingIdentifier(kind, yieldHandling);
        if (!binding) {
          return null();
        }

        tokenStream.consumeKnownToken(TokenKind::Assign);

        BinaryNodeType bindingExpr =
            bindingInitializer(binding, kind, yieldHandling);
        if (!bindingExpr) {
          return null();
        }

        if (!handler.addPropertyDefinition(literal, propName, bindingExpr)) {
          return null();
        }
      } else {
        errorAt(namePos.begin, JSMSG_NO_VARIABLE_NAME);
        return null();
      }
    }

    bool matched;
    if (!tokenStream.matchToken(&matched, TokenKind::Comma,
                                TokenStream::Operand)) {
      return null();
    }
    if (!matched) {
      break;
    }
    if (tt == TokenKind::TripleDot) {
      error(JSMSG_REST_WITH_COMMA);
      return null();
    }
  }

  if (!mustMatchToken(TokenKind::RightCurly, TokenStream::Operand,
                      [this, begin](TokenKind actual) {
                        this->reportMissingClosing(JSMSG_CURLY_AFTER_LIST,
                                                   JSMSG_CURLY_OPENED, begin);
                      })) {
    return null();
  }

  handler.setEndPosition(literal, pos().end);
  return literal;
}

template <class ParseHandler, typename Unit>
typename ParseHandler::ListNodeType
GeneralParser<ParseHandler, Unit>::arrayBindingPattern(
    DeclarationKind kind, YieldHandling yieldHandling) {
  MOZ_ASSERT(anyChars.isCurrentTokenType(TokenKind::LeftBracket));

  if (!CheckRecursionLimit(context)) {
    return null();
  }

  uint32_t begin = pos().begin;
  ListNodeType literal = handler.newArrayLiteral(begin);
  if (!literal) {
    return null();
  }

  uint32_t index = 0;
  for (;; index++) {
    if (index >= NativeObject::MAX_DENSE_ELEMENTS_COUNT) {
      error(JSMSG_ARRAY_INIT_TOO_BIG);
      return null();
    }
||||||| merged common ancestors
            PropertyType propType;
            Node propName = propertyName(yieldHandling, declKind, literal, &propType, &propAtom);
            if (!propName) {
                return null();
            }
=======
        NameNodeType binding = bindingIdentifier(kind, yieldHandling);
        if (!binding) {
          return null();
        }
>>>>>>> upstream-releases

<<<<<<< HEAD
    TokenKind tt;
    if (!tokenStream.getToken(&tt)) {
      return null();
    }
||||||| merged common ancestors
            if (propType == PropertyType::Normal) {
                // Handle e.g., |var {p: x} = o| and |var {p: x=0} = o|.

                if (!tokenStream.getToken(&tt, TokenStream::Operand)) {
                    return null();
                }

                Node binding = bindingIdentifierOrPattern(kind, yieldHandling, tt);
                if (!binding) {
                    return null();
                }

                bool hasInitializer;
                if (!tokenStream.matchToken(&hasInitializer, TokenKind::Assign, TokenStream::Operand)) {
                    return null();
                }

                Node bindingExpr = hasInitializer
                                   ? bindingInitializer(binding, kind, yieldHandling)
                                   : binding;
                if (!bindingExpr) {
                    return null();
                }

                if (!handler.addPropertyDefinition(literal, propName, bindingExpr)) {
                    return null();
                }
            } else if (propType == PropertyType::Shorthand) {
                // Handle e.g., |var {x, y} = o| as destructuring shorthand
                // for |var {x: x, y: y} = o|.
                MOZ_ASSERT(TokenKindIsPossibleIdentifierName(tt));

                NameNodeType binding = bindingIdentifier(kind, yieldHandling);
                if (!binding) {
                    return null();
                }

                if (!handler.addShorthand(literal, handler.asName(propName), binding)) {
                    return null();
                }
            } else if (propType == PropertyType::CoverInitializedName) {
                // Handle e.g., |var {x=1, y=2} = o| as destructuring
                // shorthand with default values.
                MOZ_ASSERT(TokenKindIsPossibleIdentifierName(tt));

                NameNodeType binding = bindingIdentifier(kind, yieldHandling);
                if (!binding) {
                    return null();
                }

                tokenStream.consumeKnownToken(TokenKind::Assign);

                BinaryNodeType bindingExpr = bindingInitializer(binding, kind, yieldHandling);
                if (!bindingExpr) {
                    return null();
                }

                if (!handler.addPropertyDefinition(literal, propName, bindingExpr)) {
                    return null();
                }
            } else {
                errorAt(namePos.begin, JSMSG_NO_VARIABLE_NAME);
                return null();
            }
        }
=======
        tokenStream.consumeKnownToken(TokenKind::Assign);

        BinaryNodeType bindingExpr =
            bindingInitializer(binding, kind, yieldHandling);
        if (!bindingExpr) {
          return null();
        }
>>>>>>> upstream-releases

<<<<<<< HEAD
    if (tt == TokenKind::RightBracket) {
      anyChars.ungetToken();
      anyChars.addModifierException(TokenStream::OperandIsNone);
      break;
||||||| merged common ancestors
        bool matched;
        if (!tokenStream.matchToken(&matched, TokenKind::Comma, TokenStream::Operand)) {
            return null();
        }
        if (!matched) {
            break;
        }
        if (tt == TokenKind::TripleDot) {
            error(JSMSG_REST_WITH_COMMA);
            return null();
        }
=======
        if (!handler_.addPropertyDefinition(literal, propName, bindingExpr)) {
          return null();
        }
      } else {
        errorAt(namePos.begin, JSMSG_NO_VARIABLE_NAME);
        return null();
      }
    }

    bool matched;
    if (!tokenStream.matchToken(&matched, TokenKind::Comma,
                                TokenStream::SlashIsInvalid)) {
      return null();
    }
    if (!matched) {
      break;
    }
    if (tt == TokenKind::TripleDot) {
      error(JSMSG_REST_WITH_COMMA);
      return null();
    }
  }

  if (!mustMatchToken(TokenKind::RightCurly, [this, begin](TokenKind actual) {
        this->reportMissingClosing(JSMSG_CURLY_AFTER_LIST, JSMSG_CURLY_OPENED,
                                   begin);
      })) {
    return null();
  }

  handler_.setEndPosition(literal, pos().end);
  return literal;
}

template <class ParseHandler, typename Unit>
typename ParseHandler::ListNodeType
GeneralParser<ParseHandler, Unit>::arrayBindingPattern(
    DeclarationKind kind, YieldHandling yieldHandling) {
  MOZ_ASSERT(anyChars.isCurrentTokenType(TokenKind::LeftBracket));

  if (!CheckRecursionLimit(cx_)) {
    return null();
  }

  uint32_t begin = pos().begin;
  ListNodeType literal = handler_.newArrayLiteral(begin);
  if (!literal) {
    return null();
  }

  uint32_t index = 0;
  for (;; index++) {
    if (index >= NativeObject::MAX_DENSE_ELEMENTS_COUNT) {
      error(JSMSG_ARRAY_INIT_TOO_BIG);
      return null();
    }

    TokenKind tt;
    if (!tokenStream.getToken(&tt)) {
      return null();
    }

    if (tt == TokenKind::RightBracket) {
      anyChars.ungetToken();
      break;
>>>>>>> upstream-releases
    }

<<<<<<< HEAD
    if (tt == TokenKind::Comma) {
      if (!handler.addElision(literal, pos())) {
        return null();
      }
    } else if (tt == TokenKind::TripleDot) {
      uint32_t begin = pos().begin;
||||||| merged common ancestors
    MUST_MATCH_TOKEN_MOD_WITH_REPORT(TokenKind::RightCurly, TokenStream::Operand,
                                     reportMissingClosing(JSMSG_CURLY_AFTER_LIST,
                                                          JSMSG_CURLY_OPENED, begin));
=======
    if (tt == TokenKind::Comma) {
      if (!handler_.addElision(literal, pos())) {
        return null();
      }
    } else if (tt == TokenKind::TripleDot) {
      uint32_t begin = pos().begin;
>>>>>>> upstream-releases

      TokenKind tt;
      if (!tokenStream.getToken(&tt)) {
        return null();
      }

<<<<<<< HEAD
      Node inner = bindingIdentifierOrPattern(kind, yieldHandling, tt);
      if (!inner) {
        return null();
      }

      if (!handler.addSpreadElement(literal, begin, inner)) {
        return null();
      }
    } else {
      Node binding = bindingIdentifierOrPattern(kind, yieldHandling, tt);
      if (!binding) {
        return null();
      }
||||||| merged common ancestors
template <class ParseHandler, typename Unit>
typename ParseHandler::ListNodeType
GeneralParser<ParseHandler, Unit>::arrayBindingPattern(DeclarationKind kind,
                                                       YieldHandling yieldHandling)
{
    MOZ_ASSERT(anyChars.isCurrentTokenType(TokenKind::LeftBracket));
=======
      Node inner = bindingIdentifierOrPattern(kind, yieldHandling, tt);
      if (!inner) {
        return null();
      }
>>>>>>> upstream-releases

<<<<<<< HEAD
      bool hasInitializer;
      if (!tokenStream.matchToken(&hasInitializer, TokenKind::Assign,
                                  TokenStream::Operand)) {
||||||| merged common ancestors
    if (!CheckRecursionLimit(context)) {
=======
      if (!handler_.addSpreadElement(literal, begin, inner)) {
>>>>>>> upstream-releases
        return null();
<<<<<<< HEAD
      }

      Node element = hasInitializer
                         ? bindingInitializer(binding, kind, yieldHandling)
                         : binding;
      if (!element) {
        return null();
      }

      handler.addArrayElement(literal, element);
||||||| merged common ancestors
=======
      }
    } else {
      Node binding = bindingIdentifierOrPattern(kind, yieldHandling, tt);
      if (!binding) {
        return null();
      }

      bool hasInitializer;
      if (!tokenStream.matchToken(&hasInitializer, TokenKind::Assign,
                                  TokenStream::SlashIsRegExp)) {
        return null();
      }

      Node element = hasInitializer
                         ? bindingInitializer(binding, kind, yieldHandling)
                         : binding;
      if (!element) {
        return null();
      }

      handler_.addArrayElement(literal, element);
>>>>>>> upstream-releases
    }

<<<<<<< HEAD
    if (tt != TokenKind::Comma) {
      // If we didn't already match TokenKind::Comma in above case.
      bool matched;
      if (!tokenStream.matchToken(&matched, TokenKind::Comma,
                                  TokenStream::Operand)) {
        return null();
      }
      if (!matched) {
        break;
      }

      if (tt == TokenKind::TripleDot) {
        error(JSMSG_REST_WITH_COMMA);
||||||| merged common ancestors
    uint32_t begin = pos().begin;
    ListNodeType literal = handler.newArrayLiteral(begin);
    if (!literal) {
=======
    if (tt != TokenKind::Comma) {
      // If we didn't already match TokenKind::Comma in above case.
      bool matched;
      if (!tokenStream.matchToken(&matched, TokenKind::Comma,
                                  TokenStream::SlashIsRegExp)) {
        return null();
      }
      if (!matched) {
        break;
      }

      if (tt == TokenKind::TripleDot) {
        error(JSMSG_REST_WITH_COMMA);
>>>>>>> upstream-releases
        return null();
      }
    }
<<<<<<< HEAD
  }

  if (!mustMatchToken(TokenKind::RightBracket, TokenStream::Operand,
                      [this, begin](TokenKind actual) {
                        this->reportMissingClosing(JSMSG_BRACKET_AFTER_LIST,
                                                   JSMSG_BRACKET_OPENED, begin);
                      })) {
    return null();
  }
||||||| merged common ancestors
=======
  }

  if (!mustMatchToken(TokenKind::RightBracket, [this, begin](TokenKind actual) {
        this->reportMissingClosing(JSMSG_BRACKET_AFTER_LIST,
                                   JSMSG_BRACKET_OPENED, begin);
      })) {
    return null();
  }
>>>>>>> upstream-releases

<<<<<<< HEAD
  handler.setEndPosition(literal, pos().end);
  return literal;
||||||| merged common ancestors
     uint32_t index = 0;
     for (; ; index++) {
         if (index >= NativeObject::MAX_DENSE_ELEMENTS_COUNT) {
             error(JSMSG_ARRAY_INIT_TOO_BIG);
             return null();
         }

         TokenKind tt;
         if (!tokenStream.getToken(&tt)) {
             return null();
         }

         if (tt == TokenKind::RightBracket) {
             anyChars.ungetToken();
             anyChars.addModifierException(TokenStream::OperandIsNone);
             break;
         }

         if (tt == TokenKind::Comma) {
             if (!handler.addElision(literal, pos())) {
                 return null();
             }
         } else if (tt == TokenKind::TripleDot) {
             uint32_t begin = pos().begin;

             TokenKind tt;
             if (!tokenStream.getToken(&tt)) {
                 return null();
             }

             Node inner = bindingIdentifierOrPattern(kind, yieldHandling, tt);
             if (!inner) {
                 return null();
             }

             if (!handler.addSpreadElement(literal, begin, inner)) {
                 return null();
             }
         } else {
             Node binding = bindingIdentifierOrPattern(kind, yieldHandling, tt);
             if (!binding) {
                 return null();
             }

             bool hasInitializer;
             if (!tokenStream.matchToken(&hasInitializer, TokenKind::Assign, TokenStream::Operand)) {
                 return null();
             }

             Node element = hasInitializer
                            ? bindingInitializer(binding, kind, yieldHandling)
                            : binding;
             if (!element) {
                 return null();
             }

             handler.addArrayElement(literal, element);
         }

         if (tt != TokenKind::Comma) {
             // If we didn't already match TokenKind::Comma in above case.
             bool matched;
             if (!tokenStream.matchToken(&matched, TokenKind::Comma, TokenStream::Operand)) {
                 return null();
             }
             if (!matched) {
                 break;
             }

             if (tt == TokenKind::TripleDot) {
                 error(JSMSG_REST_WITH_COMMA);
                 return null();
             }
         }
     }

     MUST_MATCH_TOKEN_MOD_WITH_REPORT(TokenKind::RightBracket, TokenStream::Operand,
                                      reportMissingClosing(JSMSG_BRACKET_AFTER_LIST,
                                                           JSMSG_BRACKET_OPENED, begin));

    handler.setEndPosition(literal, pos().end);
    return literal;
=======
  handler_.setEndPosition(literal, pos().end);
  return literal;
>>>>>>> upstream-releases
}

template <class ParseHandler, typename Unit>
typename ParseHandler::Node
GeneralParser<ParseHandler, Unit>::destructuringDeclaration(
    DeclarationKind kind, YieldHandling yieldHandling, TokenKind tt) {
  MOZ_ASSERT(anyChars.isCurrentTokenType(tt));
  MOZ_ASSERT(tt == TokenKind::LeftBracket || tt == TokenKind::LeftCurly);

  return tt == TokenKind::LeftBracket
             ? arrayBindingPattern(kind, yieldHandling)
             : objectBindingPattern(kind, yieldHandling);
}

template <class ParseHandler, typename Unit>
typename ParseHandler::Node
<<<<<<< HEAD
GeneralParser<ParseHandler, Unit>::destructuringDeclarationWithoutYieldOrAwait(
    DeclarationKind kind, YieldHandling yieldHandling, TokenKind tt) {
  uint32_t startYieldOffset = pc->lastYieldOffset;
  uint32_t startAwaitOffset = pc->lastAwaitOffset;
  Node res = destructuringDeclaration(kind, yieldHandling, tt);
  if (res) {
    if (pc->lastYieldOffset != startYieldOffset) {
      errorAt(pc->lastYieldOffset, JSMSG_YIELD_IN_PARAMETER);
      return null();
    }
    if (pc->lastAwaitOffset != startAwaitOffset) {
      errorAt(pc->lastAwaitOffset, JSMSG_AWAIT_IN_PARAMETER);
      return null();
||||||| merged common ancestors
GeneralParser<ParseHandler, Unit>::destructuringDeclarationWithoutYieldOrAwait(DeclarationKind kind,
                                                                               YieldHandling yieldHandling,
                                                                               TokenKind tt)
{
    uint32_t startYieldOffset = pc->lastYieldOffset;
    uint32_t startAwaitOffset = pc->lastAwaitOffset;
    Node res = destructuringDeclaration(kind, yieldHandling, tt);
    if (res) {
        if (pc->lastYieldOffset != startYieldOffset) {
            errorAt(pc->lastYieldOffset, JSMSG_YIELD_IN_PARAMETER);
            return null();
        }
        if (pc->lastAwaitOffset != startAwaitOffset) {
            errorAt(pc->lastAwaitOffset, JSMSG_AWAIT_IN_PARAMETER);
            return null();
        }
=======
GeneralParser<ParseHandler, Unit>::destructuringDeclarationWithoutYieldOrAwait(
    DeclarationKind kind, YieldHandling yieldHandling, TokenKind tt) {
  uint32_t startYieldOffset = pc_->lastYieldOffset;
  uint32_t startAwaitOffset = pc_->lastAwaitOffset;
  Node res = destructuringDeclaration(kind, yieldHandling, tt);
  if (res) {
    if (pc_->lastYieldOffset != startYieldOffset) {
      errorAt(pc_->lastYieldOffset, JSMSG_YIELD_IN_PARAMETER);
      return null();
    }
    if (pc_->lastAwaitOffset != startAwaitOffset) {
      errorAt(pc_->lastAwaitOffset, JSMSG_AWAIT_IN_PARAMETER);
      return null();
>>>>>>> upstream-releases
    }
  }
  return res;
}

template <class ParseHandler, typename Unit>
typename ParseHandler::LexicalScopeNodeType
GeneralParser<ParseHandler, Unit>::blockStatement(YieldHandling yieldHandling,
                                                  unsigned errorNumber) {
  MOZ_ASSERT(anyChars.isCurrentTokenType(TokenKind::LeftCurly));
  uint32_t openedPos = pos().begin;

<<<<<<< HEAD
  ParseContext::Statement stmt(pc, StatementKind::Block);
  ParseContext::Scope scope(this);
  if (!scope.init(pc)) {
    return null();
  }
||||||| merged common ancestors
    ParseContext::Statement stmt(pc, StatementKind::Block);
    ParseContext::Scope scope(this);
    if (!scope.init(pc)) {
        return null();
    }
=======
  ParseContext::Statement stmt(pc_, StatementKind::Block);
  ParseContext::Scope scope(this);
  if (!scope.init(pc_)) {
    return null();
  }
>>>>>>> upstream-releases

  ListNodeType list = statementList(yieldHandling);
  if (!list) {
    return null();
  }

<<<<<<< HEAD
  if (!mustMatchToken(TokenKind::RightCurly, TokenStream::Operand,
                      [this, errorNumber, openedPos](TokenKind actual) {
                        this->reportMissingClosing(
                            errorNumber, JSMSG_CURLY_OPENED, openedPos);
                      })) {
    return null();
  }
||||||| merged common ancestors
    MUST_MATCH_TOKEN_MOD_WITH_REPORT(TokenKind::RightCurly, TokenStream::Operand,
                                     reportMissingClosing(errorNumber, JSMSG_CURLY_OPENED,
                                                          openedPos));
=======
  if (!mustMatchToken(TokenKind::RightCurly, [this, errorNumber,
                                              openedPos](TokenKind actual) {
        this->reportMissingClosing(errorNumber, JSMSG_CURLY_OPENED, openedPos);
      })) {
    return null();
  }
>>>>>>> upstream-releases

  return finishLexicalScope(scope, list);
}

template <class ParseHandler, typename Unit>
typename ParseHandler::Node
GeneralParser<ParseHandler, Unit>::expressionAfterForInOrOf(
    ParseNodeKind forHeadKind, YieldHandling yieldHandling) {
  MOZ_ASSERT(forHeadKind == ParseNodeKind::ForIn ||
             forHeadKind == ParseNodeKind::ForOf);
  Node pn = forHeadKind == ParseNodeKind::ForOf
                ? assignExpr(InAllowed, yieldHandling, TripledotProhibited)
                : expr(InAllowed, yieldHandling, TripledotProhibited);
  return pn;
}

template <class ParseHandler, typename Unit>
typename ParseHandler::Node
<<<<<<< HEAD
GeneralParser<ParseHandler, Unit>::declarationPattern(
    DeclarationKind declKind, TokenKind tt, bool initialDeclaration,
    YieldHandling yieldHandling, ParseNodeKind* forHeadKind,
    Node* forInOrOfExpression) {
  MOZ_ASSERT(anyChars.isCurrentTokenType(TokenKind::LeftBracket) ||
             anyChars.isCurrentTokenType(TokenKind::LeftCurly));

  Node pattern = destructuringDeclaration(declKind, yieldHandling, tt);
  if (!pattern) {
    return null();
  }

  if (initialDeclaration && forHeadKind) {
    bool isForIn, isForOf;
    if (!matchInOrOf(&isForIn, &isForOf)) {
      return null();
    }
||||||| merged common ancestors
GeneralParser<ParseHandler, Unit>::declarationPattern(DeclarationKind declKind, TokenKind tt,
                                                      bool initialDeclaration,
                                                      YieldHandling yieldHandling,
                                                      ParseNodeKind* forHeadKind,
                                                      Node* forInOrOfExpression)
{
    MOZ_ASSERT(anyChars.isCurrentTokenType(TokenKind::LeftBracket) ||
               anyChars.isCurrentTokenType(TokenKind::LeftCurly));

    Node pattern = destructuringDeclaration(declKind, yieldHandling, tt);
    if (!pattern) {
        return null();
    }

    if (initialDeclaration && forHeadKind) {
        bool isForIn, isForOf;
        if (!matchInOrOf(&isForIn, &isForOf)) {
            return null();
        }
=======
GeneralParser<ParseHandler, Unit>::declarationPattern(
    DeclarationKind declKind, TokenKind tt, bool initialDeclaration,
    YieldHandling yieldHandling, ParseNodeKind* forHeadKind,
    Node* forInOrOfExpression) {
  MOZ_ASSERT(anyChars.isCurrentTokenType(TokenKind::LeftBracket) ||
             anyChars.isCurrentTokenType(TokenKind::LeftCurly));

  Node pattern = destructuringDeclaration(declKind, yieldHandling, tt);
  if (!pattern) {
    return null();
  }
>>>>>>> upstream-releases

<<<<<<< HEAD
    if (isForIn) {
      *forHeadKind = ParseNodeKind::ForIn;
    } else if (isForOf) {
      *forHeadKind = ParseNodeKind::ForOf;
||||||| merged common ancestors
        if (isForIn) {
            *forHeadKind = ParseNodeKind::ForIn;
        } else if (isForOf) {
            *forHeadKind = ParseNodeKind::ForOf;
=======
  if (initialDeclaration && forHeadKind) {
    bool isForIn, isForOf;
    if (!matchInOrOf(&isForIn, &isForOf)) {
      return null();
    }
>>>>>>> upstream-releases

<<<<<<< HEAD
      // Annex B.3.5 has different early errors for vars in for-of loops.
      if (declKind == DeclarationKind::Var) {
        declKind = DeclarationKind::ForOfVar;
      }
    } else {
      *forHeadKind = ParseNodeKind::ForHead;
    }
||||||| merged common ancestors
            // Annex B.3.5 has different early errors for vars in for-of loops.
            if (declKind == DeclarationKind::Var) {
                declKind = DeclarationKind::ForOfVar;
            }
        } else {
            *forHeadKind = ParseNodeKind::ForHead;
        }
=======
    if (isForIn) {
      *forHeadKind = ParseNodeKind::ForIn;
    } else if (isForOf) {
      *forHeadKind = ParseNodeKind::ForOf;
    } else {
      *forHeadKind = ParseNodeKind::ForHead;
    }
>>>>>>> upstream-releases

    if (*forHeadKind != ParseNodeKind::ForHead) {
      *forInOrOfExpression =
          expressionAfterForInOrOf(*forHeadKind, yieldHandling);
      if (!*forInOrOfExpression) {
        return null();
      }

      return pattern;
    }
  }

<<<<<<< HEAD
  if (!mustMatchToken(TokenKind::Assign, TokenStream::Operand,
                      JSMSG_BAD_DESTRUCT_DECL)) {
    return null();
  }
||||||| merged common ancestors
    MUST_MATCH_TOKEN_MOD(TokenKind::Assign, TokenStream::Operand, JSMSG_BAD_DESTRUCT_DECL);
=======
  if (!mustMatchToken(TokenKind::Assign, JSMSG_BAD_DESTRUCT_DECL)) {
    return null();
  }
>>>>>>> upstream-releases

  Node init = assignExpr(forHeadKind ? InProhibited : InAllowed, yieldHandling,
                         TripledotProhibited);
  if (!init) {
    return null();
  }

<<<<<<< HEAD
  return handler.newAssignment(ParseNodeKind::Assign, pattern, init);
||||||| merged common ancestors
    return handler.newAssignment(ParseNodeKind::Assign, pattern, init);
=======
  return handler_.newAssignment(ParseNodeKind::AssignExpr, pattern, init);
>>>>>>> upstream-releases
}

template <class ParseHandler, typename Unit>
<<<<<<< HEAD
bool GeneralParser<ParseHandler, Unit>::initializerInNameDeclaration(
    NameNodeType binding, DeclarationKind declKind, bool initialDeclaration,
    YieldHandling yieldHandling, ParseNodeKind* forHeadKind,
    Node* forInOrOfExpression) {
  MOZ_ASSERT(anyChars.isCurrentTokenType(TokenKind::Assign));

  uint32_t initializerOffset;
  if (!tokenStream.peekOffset(&initializerOffset, TokenStream::Operand)) {
    return false;
  }

  Node initializer = assignExpr(forHeadKind ? InProhibited : InAllowed,
                                yieldHandling, TripledotProhibited);
  if (!initializer) {
    return false;
  }

  if (forHeadKind && initialDeclaration) {
    bool isForIn, isForOf;
    if (!matchInOrOf(&isForIn, &isForOf)) {
      return false;
    }

    // An initialized declaration can't appear in a for-of:
    //
    //   for (var/let/const x = ... of ...); // BAD
    if (isForOf) {
      errorAt(initializerOffset, JSMSG_OF_AFTER_FOR_LOOP_DECL);
      return false;
    }
||||||| merged common ancestors
bool
GeneralParser<ParseHandler, Unit>::initializerInNameDeclaration(NameNodeType binding,
                                                                DeclarationKind declKind,
                                                                bool initialDeclaration,
                                                                YieldHandling yieldHandling,
                                                                ParseNodeKind* forHeadKind,
                                                                Node* forInOrOfExpression)
{
    MOZ_ASSERT(anyChars.isCurrentTokenType(TokenKind::Assign));

    uint32_t initializerOffset;
    if (!tokenStream.peekOffset(&initializerOffset, TokenStream::Operand)) {
        return false;
    }

    Node initializer = assignExpr(forHeadKind ? InProhibited : InAllowed,
                                  yieldHandling, TripledotProhibited);
    if (!initializer) {
        return false;
    }

    if (forHeadKind && initialDeclaration) {
        bool isForIn, isForOf;
        if (!matchInOrOf(&isForIn, &isForOf)) {
            return false;
        }

        // An initialized declaration can't appear in a for-of:
        //
        //   for (var/let/const x = ... of ...); // BAD
        if (isForOf) {
            errorAt(initializerOffset, JSMSG_OF_AFTER_FOR_LOOP_DECL);
            return false;
        }
=======
typename ParseHandler::AssignmentNodeType
GeneralParser<ParseHandler, Unit>::initializerInNameDeclaration(
    NameNodeType binding, DeclarationKind declKind, bool initialDeclaration,
    YieldHandling yieldHandling, ParseNodeKind* forHeadKind,
    Node* forInOrOfExpression) {
  MOZ_ASSERT(anyChars.isCurrentTokenType(TokenKind::Assign));
>>>>>>> upstream-releases

<<<<<<< HEAD
    if (isForIn) {
      // Lexical declarations in for-in loops can't be initialized:
      //
      //   for (let/const x = ... in ...); // BAD
      if (DeclarationKindIsLexical(declKind)) {
        errorAt(initializerOffset, JSMSG_IN_AFTER_LEXICAL_FOR_DECL);
        return false;
      }
||||||| merged common ancestors
        if (isForIn) {
            // Lexical declarations in for-in loops can't be initialized:
            //
            //   for (let/const x = ... in ...); // BAD
            if (DeclarationKindIsLexical(declKind)) {
                errorAt(initializerOffset, JSMSG_IN_AFTER_LEXICAL_FOR_DECL);
                return false;
            }
=======
  uint32_t initializerOffset;
  if (!tokenStream.peekOffset(&initializerOffset, TokenStream::SlashIsRegExp)) {
    return null();
  }
>>>>>>> upstream-releases

<<<<<<< HEAD
      // This leaves only initialized for-in |var| declarations.  ES6
      // forbids these; later ES un-forbids in non-strict mode code.
      *forHeadKind = ParseNodeKind::ForIn;
      if (!strictModeErrorAt(initializerOffset,
                             JSMSG_INVALID_FOR_IN_DECL_WITH_INIT)) {
        return false;
      }
||||||| merged common ancestors
            // This leaves only initialized for-in |var| declarations.  ES6
            // forbids these; later ES un-forbids in non-strict mode code.
            *forHeadKind = ParseNodeKind::ForIn;
            if (!strictModeErrorAt(initializerOffset, JSMSG_INVALID_FOR_IN_DECL_WITH_INIT)) {
                return false;
            }
=======
  Node initializer = assignExpr(forHeadKind ? InProhibited : InAllowed,
                                yieldHandling, TripledotProhibited);
  if (!initializer) {
    return null();
  }
>>>>>>> upstream-releases

<<<<<<< HEAD
      *forInOrOfExpression =
          expressionAfterForInOrOf(ParseNodeKind::ForIn, yieldHandling);
      if (!*forInOrOfExpression) {
        return false;
      }
    } else {
      *forHeadKind = ParseNodeKind::ForHead;
||||||| merged common ancestors
            *forInOrOfExpression = expressionAfterForInOrOf(ParseNodeKind::ForIn, yieldHandling);
            if (!*forInOrOfExpression) {
                return false;
            }
        } else {
            *forHeadKind = ParseNodeKind::ForHead;
        }
=======
  if (forHeadKind && initialDeclaration) {
    bool isForIn, isForOf;
    if (!matchInOrOf(&isForIn, &isForOf)) {
      return null();
>>>>>>> upstream-releases
    }
  }

<<<<<<< HEAD
  return handler.finishInitializerAssignment(binding, initializer);
}
||||||| merged common ancestors
    return handler.finishInitializerAssignment(binding, initializer);
}
=======
    // An initialized declaration can't appear in a for-of:
    //
    //   for (var/let/const x = ... of ...); // BAD
    if (isForOf) {
      errorAt(initializerOffset, JSMSG_OF_AFTER_FOR_LOOP_DECL);
      return null();
    }
>>>>>>> upstream-releases

<<<<<<< HEAD
template <class ParseHandler, typename Unit>
typename ParseHandler::NameNodeType
GeneralParser<ParseHandler, Unit>::declarationName(DeclarationKind declKind,
                                                   TokenKind tt,
                                                   bool initialDeclaration,
                                                   YieldHandling yieldHandling,
                                                   ParseNodeKind* forHeadKind,
                                                   Node* forInOrOfExpression) {
  // Anything other than possible identifier is an error.
  if (!TokenKindIsPossibleIdentifier(tt)) {
    error(JSMSG_NO_VARIABLE_NAME);
    return null();
  }
||||||| merged common ancestors
template <class ParseHandler, typename Unit>
typename ParseHandler::NameNodeType
GeneralParser<ParseHandler, Unit>::declarationName(DeclarationKind declKind, TokenKind tt,
                                                   bool initialDeclaration,
                                                   YieldHandling yieldHandling,
                                                   ParseNodeKind* forHeadKind,
                                                   Node* forInOrOfExpression)
{
    // Anything other than possible identifier is an error.
    if (!TokenKindIsPossibleIdentifier(tt)) {
        error(JSMSG_NO_VARIABLE_NAME);
        return null();
    }

    RootedPropertyName name(context, bindingIdentifier(yieldHandling));
    if (!name) {
        return null();
    }
=======
    if (isForIn) {
      // Lexical declarations in for-in loops can't be initialized:
      //
      //   for (let/const x = ... in ...); // BAD
      if (DeclarationKindIsLexical(declKind)) {
        errorAt(initializerOffset, JSMSG_IN_AFTER_LEXICAL_FOR_DECL);
        return null();
      }

      // This leaves only initialized for-in |var| declarations.  ES6
      // forbids these; later ES un-forbids in non-strict mode code.
      *forHeadKind = ParseNodeKind::ForIn;
      if (!strictModeErrorAt(initializerOffset,
                             JSMSG_INVALID_FOR_IN_DECL_WITH_INIT)) {
        return null();
      }
>>>>>>> upstream-releases

<<<<<<< HEAD
  RootedPropertyName name(context, bindingIdentifier(yieldHandling));
  if (!name) {
    return null();
  }
||||||| merged common ancestors
    NameNodeType binding = newName(name);
    if (!binding) {
        return null();
    }
=======
      *forInOrOfExpression =
          expressionAfterForInOrOf(ParseNodeKind::ForIn, yieldHandling);
      if (!*forInOrOfExpression) {
        return null();
      }
    } else {
      *forHeadKind = ParseNodeKind::ForHead;
    }
  }
>>>>>>> upstream-releases

<<<<<<< HEAD
  NameNodeType binding = newName(name);
  if (!binding) {
    return null();
  }

  TokenPos namePos = pos();

  // The '=' context after a variable name in a declaration is an opportunity
  // for ASI, and thus for the next token to start an ExpressionStatement:
  //
  //  var foo   // VariableDeclaration
  //  /bar/g;   // ExpressionStatement
  //
  // Therefore get the token here as Operand.
  bool matched;
  if (!tokenStream.matchToken(&matched, TokenKind::Assign,
                              TokenStream::Operand)) {
    return null();
  }
||||||| merged common ancestors
    TokenPos namePos = pos();
=======
  return handler_.finishInitializerAssignment(binding, initializer);
}
>>>>>>> upstream-releases

<<<<<<< HEAD
  if (matched) {
    if (!initializerInNameDeclaration(binding, declKind, initialDeclaration,
                                      yieldHandling, forHeadKind,
                                      forInOrOfExpression)) {
      return null();
    }
  } else {
    if (initialDeclaration && forHeadKind) {
      bool isForIn, isForOf;
      if (!matchInOrOf(&isForIn, &isForOf)) {
        return null();
      }
||||||| merged common ancestors
    // The '=' context after a variable name in a declaration is an opportunity
    // for ASI, and thus for the next token to start an ExpressionStatement:
    //
    //  var foo   // VariableDeclaration
    //  /bar/g;   // ExpressionStatement
    //
    // Therefore get the token here as Operand.
    bool matched;
    if (!tokenStream.matchToken(&matched, TokenKind::Assign, TokenStream::Operand)) {
        return null();
    }
=======
template <class ParseHandler, typename Unit>
typename ParseHandler::Node GeneralParser<ParseHandler, Unit>::declarationName(
    DeclarationKind declKind, TokenKind tt, bool initialDeclaration,
    YieldHandling yieldHandling, ParseNodeKind* forHeadKind,
    Node* forInOrOfExpression) {
  // Anything other than possible identifier is an error.
  if (!TokenKindIsPossibleIdentifier(tt)) {
    error(JSMSG_NO_VARIABLE_NAME);
    return null();
  }
>>>>>>> upstream-releases

<<<<<<< HEAD
      if (isForIn) {
        *forHeadKind = ParseNodeKind::ForIn;
      } else if (isForOf) {
        *forHeadKind = ParseNodeKind::ForOf;
||||||| merged common ancestors
    if (matched) {
        if (!initializerInNameDeclaration(binding, declKind, initialDeclaration,
                                          yieldHandling, forHeadKind, forInOrOfExpression))
        {
            return null();
        }
    } else {
        if (initialDeclaration && forHeadKind) {
            bool isForIn, isForOf;
            if (!matchInOrOf(&isForIn, &isForOf)) {
                return null();
            }

            if (isForIn) {
                *forHeadKind = ParseNodeKind::ForIn;
            } else if (isForOf) {
                *forHeadKind = ParseNodeKind::ForOf;

                // Annex B.3.5 has different early errors for vars in for-of loops.
                if (declKind == DeclarationKind::Var) {
                    declKind = DeclarationKind::ForOfVar;
                }
            } else {
                *forHeadKind = ParseNodeKind::ForHead;
            }
        }
=======
  RootedPropertyName name(cx_, bindingIdentifier(yieldHandling));
  if (!name) {
    return null();
  }

  NameNodeType binding = newName(name);
  if (!binding) {
    return null();
  }

  TokenPos namePos = pos();

  // The '=' context after a variable name in a declaration is an opportunity
  // for ASI, and thus for the next token to start an ExpressionStatement:
  //
  //  var foo   // VariableDeclaration
  //  /bar/g;   // ExpressionStatement
  //
  // Therefore get the token here with SlashIsRegExp.
  bool matched;
  if (!tokenStream.matchToken(&matched, TokenKind::Assign,
                              TokenStream::SlashIsRegExp)) {
    return null();
  }

  Node declaration;
  if (matched) {
    declaration = initializerInNameDeclaration(
        binding, declKind, initialDeclaration, yieldHandling, forHeadKind,
        forInOrOfExpression);
    if (!declaration) {
      return null();
    }
  } else {
    declaration = binding;
>>>>>>> upstream-releases

<<<<<<< HEAD
        // Annex B.3.5 has different early errors for vars in for-of loops.
        if (declKind == DeclarationKind::Var) {
          declKind = DeclarationKind::ForOfVar;
        }
      } else {
        *forHeadKind = ParseNodeKind::ForHead;
      }
||||||| merged common ancestors
        if (forHeadKind && *forHeadKind != ParseNodeKind::ForHead) {
            *forInOrOfExpression = expressionAfterForInOrOf(*forHeadKind, yieldHandling);
            if (!*forInOrOfExpression) {
                return null();
            }
        } else {
            // Normal const declarations, and const declarations in for(;;)
            // heads, must be initialized.
            if (declKind == DeclarationKind::Const) {
                errorAt(namePos.begin, JSMSG_BAD_CONST_DECL);
                return null();
            }
        }
=======
    if (initialDeclaration && forHeadKind) {
      bool isForIn, isForOf;
      if (!matchInOrOf(&isForIn, &isForOf)) {
        return null();
      }

      if (isForIn) {
        *forHeadKind = ParseNodeKind::ForIn;
      } else if (isForOf) {
        *forHeadKind = ParseNodeKind::ForOf;
      } else {
        *forHeadKind = ParseNodeKind::ForHead;
      }
>>>>>>> upstream-releases
    }

<<<<<<< HEAD
    if (forHeadKind && *forHeadKind != ParseNodeKind::ForHead) {
      *forInOrOfExpression =
          expressionAfterForInOrOf(*forHeadKind, yieldHandling);
      if (!*forInOrOfExpression) {
        return null();
      }
    } else {
      // Normal const declarations, and const declarations in for(;;)
      // heads, must be initialized.
      if (declKind == DeclarationKind::Const) {
        errorAt(namePos.begin, JSMSG_BAD_CONST_DECL);
||||||| merged common ancestors
    // Note the declared name after knowing whether or not we are in a for-of
    // loop, due to special early error semantics in Annex B.3.5.
    if (!noteDeclaredName(name, declKind, namePos)) {
=======
    if (forHeadKind && *forHeadKind != ParseNodeKind::ForHead) {
      *forInOrOfExpression =
          expressionAfterForInOrOf(*forHeadKind, yieldHandling);
      if (!*forInOrOfExpression) {
>>>>>>> upstream-releases
        return null();
<<<<<<< HEAD
      }
||||||| merged common ancestors
=======
      }
    } else {
      // Normal const declarations, and const declarations in for(;;)
      // heads, must be initialized.
      if (declKind == DeclarationKind::Const) {
        errorAt(namePos.begin, JSMSG_BAD_CONST_DECL);
        return null();
      }
>>>>>>> upstream-releases
    }
<<<<<<< HEAD
  }

  // Note the declared name after knowing whether or not we are in a for-of
  // loop, due to special early error semantics in Annex B.3.5.
  if (!noteDeclaredName(name, declKind, namePos)) {
    return null();
  }
||||||| merged common ancestors
=======
  }
>>>>>>> upstream-releases

<<<<<<< HEAD
  return binding;
||||||| merged common ancestors
    return binding;
=======
  // Note the declared name after knowing whether or not we are in a for-of
  // loop, due to special early error semantics in Annex B.3.5.
  if (!noteDeclaredName(name, declKind, namePos)) {
    return null();
  }

  return declaration;
>>>>>>> upstream-releases
}

template <class ParseHandler, typename Unit>
typename ParseHandler::ListNodeType
<<<<<<< HEAD
GeneralParser<ParseHandler, Unit>::declarationList(
    YieldHandling yieldHandling, ParseNodeKind kind,
    ParseNodeKind* forHeadKind /* = nullptr */,
    Node* forInOrOfExpression /* = nullptr */) {
  MOZ_ASSERT(kind == ParseNodeKind::Var || kind == ParseNodeKind::Let ||
             kind == ParseNodeKind::Const);

  DeclarationKind declKind;
  switch (kind) {
    case ParseNodeKind::Var:
      declKind = DeclarationKind::Var;
      break;
    case ParseNodeKind::Const:
      declKind = DeclarationKind::Const;
      break;
    case ParseNodeKind::Let:
      declKind = DeclarationKind::Let;
      break;
    default:
      MOZ_CRASH("Unknown declaration kind");
  }

  ListNodeType decl = handler.newDeclarationList(kind, pos());
  if (!decl) {
    return null();
  }
||||||| merged common ancestors
GeneralParser<ParseHandler, Unit>::declarationList(YieldHandling yieldHandling,
                                                   ParseNodeKind kind,
                                                   ParseNodeKind* forHeadKind /* = nullptr */,
                                                   Node* forInOrOfExpression /* = nullptr */)
{
    MOZ_ASSERT(kind == ParseNodeKind::Var ||
               kind == ParseNodeKind::Let ||
               kind == ParseNodeKind::Const);

    DeclarationKind declKind;
    switch (kind) {
      case ParseNodeKind::Var:
        declKind = DeclarationKind::Var;
        break;
      case ParseNodeKind::Const:
        declKind = DeclarationKind::Const;
        break;
      case ParseNodeKind::Let:
        declKind = DeclarationKind::Let;
        break;
      default:
        MOZ_CRASH("Unknown declaration kind");
    }

    ListNodeType decl = handler.newDeclarationList(kind, pos());
    if (!decl) {
        return null();
    }
=======
GeneralParser<ParseHandler, Unit>::declarationList(
    YieldHandling yieldHandling, ParseNodeKind kind,
    ParseNodeKind* forHeadKind /* = nullptr */,
    Node* forInOrOfExpression /* = nullptr */) {
  MOZ_ASSERT(kind == ParseNodeKind::VarStmt || kind == ParseNodeKind::LetDecl ||
             kind == ParseNodeKind::ConstDecl);

  DeclarationKind declKind;
  switch (kind) {
    case ParseNodeKind::VarStmt:
      declKind = DeclarationKind::Var;
      break;
    case ParseNodeKind::ConstDecl:
      declKind = DeclarationKind::Const;
      break;
    case ParseNodeKind::LetDecl:
      declKind = DeclarationKind::Let;
      break;
    default:
      MOZ_CRASH("Unknown declaration kind");
  }

  ListNodeType decl = handler_.newDeclarationList(kind, pos());
  if (!decl) {
    return null();
  }
>>>>>>> upstream-releases

  bool moreDeclarations;
  bool initialDeclaration = true;
  do {
    MOZ_ASSERT_IF(!initialDeclaration && forHeadKind,
                  *forHeadKind == ParseNodeKind::ForHead);

    TokenKind tt;
    if (!tokenStream.getToken(&tt)) {
      return null();
    }

    Node binding =
        (tt == TokenKind::LeftBracket || tt == TokenKind::LeftCurly)
            ? declarationPattern(declKind, tt, initialDeclaration,
                                 yieldHandling, forHeadKind,
                                 forInOrOfExpression)
            : declarationName(declKind, tt, initialDeclaration, yieldHandling,
                              forHeadKind, forInOrOfExpression);
    if (!binding) {
      return null();
    }

<<<<<<< HEAD
    handler.addList(decl, binding);
||||||| merged common ancestors
        handler.addList(decl, binding);
=======
    handler_.addList(decl, binding);
>>>>>>> upstream-releases

    // If we have a for-in/of loop, the above call matches the entirety
    // of the loop head (up to the closing parenthesis).
    if (forHeadKind && *forHeadKind != ParseNodeKind::ForHead) {
      break;
    }

    initialDeclaration = false;

<<<<<<< HEAD
    if (!tokenStream.matchToken(&moreDeclarations, TokenKind::Comma,
                                TokenStream::Operand)) {
      return null();
    }
  } while (moreDeclarations);
||||||| merged common ancestors
        if (!tokenStream.matchToken(&moreDeclarations, TokenKind::Comma, TokenStream::Operand)) {
            return null();
        }
    } while (moreDeclarations);
=======
    if (!tokenStream.matchToken(&moreDeclarations, TokenKind::Comma,
                                TokenStream::SlashIsRegExp)) {
      return null();
    }
  } while (moreDeclarations);
>>>>>>> upstream-releases

  return decl;
}

template <class ParseHandler, typename Unit>
typename ParseHandler::ListNodeType
<<<<<<< HEAD
GeneralParser<ParseHandler, Unit>::lexicalDeclaration(
    YieldHandling yieldHandling, DeclarationKind kind) {
  MOZ_ASSERT(kind == DeclarationKind::Const || kind == DeclarationKind::Let);

  /*
   * Parse body-level lets without a new block object. ES6 specs
   * that an execution environment's initial lexical environment
   * is the VariableEnvironment, i.e., body-level lets are in
   * the same environment record as vars.
   *
   * However, they cannot be parsed exactly as vars, as ES6
   * requires that uninitialized lets throw ReferenceError on use.
   *
   * See 8.1.1.1.6 and the note in 13.2.1.
   */
  ListNodeType decl = declarationList(
      yieldHandling, kind == DeclarationKind::Const ? ParseNodeKind::Const
                                                    : ParseNodeKind::Let);
  if (!decl || !matchOrInsertSemicolon()) {
    return null();
  }
||||||| merged common ancestors
GeneralParser<ParseHandler, Unit>::lexicalDeclaration(YieldHandling yieldHandling,
                                                      DeclarationKind kind)
{
    MOZ_ASSERT(kind == DeclarationKind::Const || kind == DeclarationKind::Let);

    /*
     * Parse body-level lets without a new block object. ES6 specs
     * that an execution environment's initial lexical environment
     * is the VariableEnvironment, i.e., body-level lets are in
     * the same environment record as vars.
     *
     * However, they cannot be parsed exactly as vars, as ES6
     * requires that uninitialized lets throw ReferenceError on use.
     *
     * See 8.1.1.1.6 and the note in 13.2.1.
     */
    ListNodeType decl = declarationList(yieldHandling,
                                        kind == DeclarationKind::Const
                                        ? ParseNodeKind::Const
                                        : ParseNodeKind::Let);
    if (!decl || !matchOrInsertSemicolon()) {
        return null();
    }
=======
GeneralParser<ParseHandler, Unit>::lexicalDeclaration(
    YieldHandling yieldHandling, DeclarationKind kind) {
  MOZ_ASSERT(kind == DeclarationKind::Const || kind == DeclarationKind::Let);

  /*
   * Parse body-level lets without a new block object. ES6 specs
   * that an execution environment's initial lexical environment
   * is the VariableEnvironment, i.e., body-level lets are in
   * the same environment record as vars.
   *
   * However, they cannot be parsed exactly as vars, as ES6
   * requires that uninitialized lets throw ReferenceError on use.
   *
   * See 8.1.1.1.6 and the note in 13.2.1.
   */
  ListNodeType decl = declarationList(
      yieldHandling, kind == DeclarationKind::Const ? ParseNodeKind::ConstDecl
                                                    : ParseNodeKind::LetDecl);
  if (!decl || !matchOrInsertSemicolon()) {
    return null();
  }
>>>>>>> upstream-releases

  return decl;
}

template <typename Unit>
<<<<<<< HEAD
bool Parser<FullParseHandler, Unit>::namedImportsOrNamespaceImport(
    TokenKind tt, ListNodeType importSpecSet) {
  if (tt == TokenKind::LeftCurly) {
    while (true) {
      // Handle the forms |import {} from 'a'| and
      // |import { ..., } from 'a'| (where ... is non empty), by
      // escaping the loop early if the next token is }.
      if (!tokenStream.getToken(&tt)) {
        return false;
      }

      if (tt == TokenKind::RightCurly) {
        break;
      }

      if (!TokenKindIsPossibleIdentifierName(tt)) {
        error(JSMSG_NO_IMPORT_NAME);
        return false;
      }

      Rooted<PropertyName*> importName(context, anyChars.currentName());
      TokenPos importNamePos = pos();

      bool matched;
      if (!tokenStream.matchToken(&matched, TokenKind::As)) {
        return false;
      }
||||||| merged common ancestors
bool
Parser<FullParseHandler, Unit>::namedImportsOrNamespaceImport(TokenKind tt,
                                                              ListNodeType importSpecSet)
{
    if (tt == TokenKind::LeftCurly) {
        while (true) {
            // Handle the forms |import {} from 'a'| and
            // |import { ..., } from 'a'| (where ... is non empty), by
            // escaping the loop early if the next token is }.
            if (!tokenStream.getToken(&tt)) {
                return false;
            }

            if (tt == TokenKind::RightCurly) {
                break;
            }

            if (!TokenKindIsPossibleIdentifierName(tt)) {
                error(JSMSG_NO_IMPORT_NAME);
                return false;
            }

            Rooted<PropertyName*> importName(context, anyChars.currentName());
            TokenPos importNamePos = pos();

            bool matched;
            if (!tokenStream.matchToken(&matched, TokenKind::As)) {
                return false;
            }

            if (matched) {
                TokenKind afterAs;
                if (!tokenStream.getToken(&afterAs)) {
                    return false;
                }

                if (!TokenKindIsPossibleIdentifierName(afterAs)) {
                    error(JSMSG_NO_BINDING_NAME);
                    return false;
                }
            } else {
                // Keywords cannot be bound to themselves, so an import name
                // that is a keyword is a syntax error if it is not followed
                // by the keyword 'as'.
                // See the ImportSpecifier production in ES6 section 15.2.2.
                if (IsKeyword(importName)) {
                    error(JSMSG_AS_AFTER_RESERVED_WORD, ReservedWordToCharZ(importName));
                    return false;
                }
            }

            RootedPropertyName bindingAtom(context, importedBinding());
            if (!bindingAtom) {
                return false;
            }

            NameNodeType bindingName = newName(bindingAtom);
            if (!bindingName) {
                return false;
            }
            if (!noteDeclaredName(bindingAtom, DeclarationKind::Import, pos())) {
                return false;
            }
=======
bool Parser<FullParseHandler, Unit>::namedImportsOrNamespaceImport(
    TokenKind tt, ListNodeType importSpecSet) {
  if (tt == TokenKind::LeftCurly) {
    while (true) {
      // Handle the forms |import {} from 'a'| and
      // |import { ..., } from 'a'| (where ... is non empty), by
      // escaping the loop early if the next token is }.
      if (!tokenStream.getToken(&tt)) {
        return false;
      }
>>>>>>> upstream-releases

<<<<<<< HEAD
      if (matched) {
        TokenKind afterAs;
        if (!tokenStream.getToken(&afterAs)) {
          return false;
        }
||||||| merged common ancestors
            NameNodeType importNameNode = newName(importName, importNamePos);
            if (!importNameNode) {
                return false;
            }
=======
      if (tt == TokenKind::RightCurly) {
        break;
      }
>>>>>>> upstream-releases

<<<<<<< HEAD
        if (!TokenKindIsPossibleIdentifierName(afterAs)) {
          error(JSMSG_NO_BINDING_NAME);
          return false;
        }
      } else {
        // Keywords cannot be bound to themselves, so an import name
        // that is a keyword is a syntax error if it is not followed
        // by the keyword 'as'.
        // See the ImportSpecifier production in ES6 section 15.2.2.
        if (IsKeyword(importName)) {
          error(JSMSG_AS_AFTER_RESERVED_WORD, ReservedWordToCharZ(importName));
          return false;
        }
      }
||||||| merged common ancestors
            BinaryNodeType importSpec = handler.newImportSpec(importNameNode, bindingName);
            if (!importSpec) {
                return false;
            }
=======
      if (!TokenKindIsPossibleIdentifierName(tt)) {
        error(JSMSG_NO_IMPORT_NAME);
        return false;
      }
>>>>>>> upstream-releases

<<<<<<< HEAD
      RootedPropertyName bindingAtom(context, importedBinding());
      if (!bindingAtom) {
        return false;
      }
||||||| merged common ancestors
            handler.addList(importSpecSet, importSpec);
=======
      Rooted<PropertyName*> importName(cx_, anyChars.currentName());
      TokenPos importNamePos = pos();
>>>>>>> upstream-releases

<<<<<<< HEAD
      NameNodeType bindingName = newName(bindingAtom);
      if (!bindingName) {
        return false;
      }
      if (!noteDeclaredName(bindingAtom, DeclarationKind::Import, pos())) {
        return false;
      }
||||||| merged common ancestors
            TokenKind next;
            if (!tokenStream.getToken(&next)) {
                return false;
            }
=======
      bool matched;
      if (!tokenStream.matchToken(&matched, TokenKind::As)) {
        return false;
      }
>>>>>>> upstream-releases

<<<<<<< HEAD
      NameNodeType importNameNode = newName(importName, importNamePos);
      if (!importNameNode) {
        return false;
      }
||||||| merged common ancestors
            if (next == TokenKind::RightCurly) {
                break;
            }
=======
      if (matched) {
        TokenKind afterAs;
        if (!tokenStream.getToken(&afterAs)) {
          return false;
        }
>>>>>>> upstream-releases

<<<<<<< HEAD
      BinaryNodeType importSpec =
          handler.newImportSpec(importNameNode, bindingName);
      if (!importSpec) {
        return false;
      }
||||||| merged common ancestors
            if (next != TokenKind::Comma) {
                error(JSMSG_RC_AFTER_IMPORT_SPEC_LIST);
                return false;
            }
        }
    } else {
        MOZ_ASSERT(tt == TokenKind::Mul);
=======
        if (!TokenKindIsPossibleIdentifierName(afterAs)) {
          error(JSMSG_NO_BINDING_NAME);
          return false;
        }
      } else {
        // Keywords cannot be bound to themselves, so an import name
        // that is a keyword is a syntax error if it is not followed
        // by the keyword 'as'.
        // See the ImportSpecifier production in ES6 section 15.2.2.
        if (IsKeyword(importName)) {
          error(JSMSG_AS_AFTER_RESERVED_WORD, ReservedWordToCharZ(importName));
          return false;
        }
      }

      RootedPropertyName bindingAtom(cx_, importedBinding());
      if (!bindingAtom) {
        return false;
      }
>>>>>>> upstream-releases

<<<<<<< HEAD
      handler.addList(importSpecSet, importSpec);
||||||| merged common ancestors
        MUST_MATCH_TOKEN_OR(TokenKind::As, JSMSG_AS_AFTER_IMPORT_STAR, false);
=======
      NameNodeType bindingName = newName(bindingAtom);
      if (!bindingName) {
        return false;
      }
      if (!noteDeclaredName(bindingAtom, DeclarationKind::Import, pos())) {
        return false;
      }
>>>>>>> upstream-releases

<<<<<<< HEAD
      TokenKind next;
      if (!tokenStream.getToken(&next)) {
        return false;
      }
||||||| merged common ancestors
        MUST_MATCH_TOKEN_FUNC_OR(TokenKindIsPossibleIdentifierName, JSMSG_NO_BINDING_NAME, false);
=======
      NameNodeType importNameNode = newName(importName, importNamePos);
      if (!importNameNode) {
        return false;
      }
>>>>>>> upstream-releases

<<<<<<< HEAD
      if (next == TokenKind::RightCurly) {
        break;
      }
||||||| merged common ancestors
        NameNodeType importName = newName(context->names().star);
        if (!importName) {
            return false;
        }
=======
      BinaryNodeType importSpec =
          handler_.newImportSpec(importNameNode, bindingName);
      if (!importSpec) {
        return false;
      }
>>>>>>> upstream-releases

<<<<<<< HEAD
      if (next != TokenKind::Comma) {
        error(JSMSG_RC_AFTER_IMPORT_SPEC_LIST);
        return false;
      }
    }
  } else {
    MOZ_ASSERT(tt == TokenKind::Mul);
||||||| merged common ancestors
        // Namespace imports are are not indirect bindings but lexical
        // definitions that hold a module namespace object. They are treated
        // as const variables which are initialized during the
        // ModuleInstantiate step.
        RootedPropertyName bindingName(context, importedBinding());
        if (!bindingName) {
            return false;
        }
        NameNodeType bindingNameNode = newName(bindingName);
        if (!bindingNameNode) {
            return false;
        }
        if (!noteDeclaredName(bindingName, DeclarationKind::Const, pos())) {
            return false;
        }
=======
      handler_.addList(importSpecSet, importSpec);
>>>>>>> upstream-releases

<<<<<<< HEAD
    if (!mustMatchToken(TokenKind::As, JSMSG_AS_AFTER_IMPORT_STAR)) {
      return false;
    }
||||||| merged common ancestors
        // The namespace import name is currently required to live on the
        // environment.
        pc->varScope().lookupDeclaredName(bindingName)->value()->setClosedOver();
=======
      TokenKind next;
      if (!tokenStream.getToken(&next)) {
        return false;
      }
>>>>>>> upstream-releases

<<<<<<< HEAD
    if (!mustMatchToken(TokenKindIsPossibleIdentifierName,
                        JSMSG_NO_BINDING_NAME)) {
      return false;
    }
||||||| merged common ancestors
        BinaryNodeType importSpec = handler.newImportSpec(importName, bindingNameNode);
        if (!importSpec) {
            return false;
        }
=======
      if (next == TokenKind::RightCurly) {
        break;
      }
>>>>>>> upstream-releases

<<<<<<< HEAD
    NameNodeType importName = newName(context->names().star);
    if (!importName) {
      return false;
||||||| merged common ancestors
        handler.addList(importSpecSet, importSpec);
=======
      if (next != TokenKind::Comma) {
        error(JSMSG_RC_AFTER_IMPORT_SPEC_LIST);
        return false;
      }
>>>>>>> upstream-releases
    }
  } else {
    MOZ_ASSERT(tt == TokenKind::Mul);

<<<<<<< HEAD
    // Namespace imports are are not indirect bindings but lexical
    // definitions that hold a module namespace object. They are treated
    // as const variables which are initialized during the
    // ModuleInstantiate step.
    RootedPropertyName bindingName(context, importedBinding());
    if (!bindingName) {
      return false;
    }
    NameNodeType bindingNameNode = newName(bindingName);
    if (!bindingNameNode) {
      return false;
    }
    if (!noteDeclaredName(bindingName, DeclarationKind::Const, pos())) {
      return false;
    }
||||||| merged common ancestors
    return true;
}
=======
    if (!mustMatchToken(TokenKind::As, JSMSG_AS_AFTER_IMPORT_STAR)) {
      return false;
    }
>>>>>>> upstream-releases

<<<<<<< HEAD
    // The namespace import name is currently required to live on the
    // environment.
    pc->varScope().lookupDeclaredName(bindingName)->value()->setClosedOver();
||||||| merged common ancestors
template<typename Unit>
BinaryNode*
Parser<FullParseHandler, Unit>::importDeclaration()
{
    MOZ_ASSERT(anyChars.isCurrentTokenType(TokenKind::Import));
=======
    if (!mustMatchToken(TokenKindIsPossibleIdentifierName,
                        JSMSG_NO_BINDING_NAME)) {
      return false;
    }
>>>>>>> upstream-releases

<<<<<<< HEAD
    BinaryNodeType importSpec =
        handler.newImportSpec(importName, bindingNameNode);
    if (!importSpec) {
      return false;
||||||| merged common ancestors
    if (!pc->atModuleLevel()) {
        error(JSMSG_IMPORT_DECL_AT_TOP_LEVEL);
        return null();
=======
    NameNodeType importName = newName(cx_->names().star);
    if (!importName) {
      return false;
>>>>>>> upstream-releases
    }

<<<<<<< HEAD
    handler.addList(importSpecSet, importSpec);
  }
||||||| merged common ancestors
    uint32_t begin = pos().begin;
    TokenKind tt;
    if (!tokenStream.getToken(&tt)) {
        return null();
    }
=======
    // Namespace imports are are not indirect bindings but lexical
    // definitions that hold a module namespace object. They are treated
    // as const variables which are initialized during the
    // ModuleInstantiate step.
    RootedPropertyName bindingName(cx_, importedBinding());
    if (!bindingName) {
      return false;
    }
    NameNodeType bindingNameNode = newName(bindingName);
    if (!bindingNameNode) {
      return false;
    }
    if (!noteDeclaredName(bindingName, DeclarationKind::Const, pos())) {
      return false;
    }
>>>>>>> upstream-releases

<<<<<<< HEAD
  return true;
}
||||||| merged common ancestors
    ListNodeType importSpecSet = handler.newList(ParseNodeKind::ImportSpecList, pos());
    if (!importSpecSet) {
        return null();
    }
=======
    // The namespace import name is currently required to live on the
    // environment.
    pc_->varScope().lookupDeclaredName(bindingName)->value()->setClosedOver();

    BinaryNodeType importSpec =
        handler_.newImportSpec(importName, bindingNameNode);
    if (!importSpec) {
      return false;
    }
>>>>>>> upstream-releases

<<<<<<< HEAD
template <typename Unit>
BinaryNode* Parser<FullParseHandler, Unit>::importDeclaration() {
  MOZ_ASSERT(anyChars.isCurrentTokenType(TokenKind::Import));
||||||| merged common ancestors
    if (tt == TokenKind::String) {
        // Handle the form |import 'a'| by leaving the list empty. This is
        // equivalent to |import {} from 'a'|.
        importSpecSet->pn_pos.end = importSpecSet->pn_pos.begin;
    } else {
        if (tt == TokenKind::LeftCurly || tt == TokenKind::Mul) {
            if (!namedImportsOrNamespaceImport(tt, importSpecSet)) {
                return null();
            }
        } else if (TokenKindIsPossibleIdentifierName(tt)) {
            // Handle the form |import a from 'b'|, by adding a single import
            // specifier to the list, with 'default' as the import name and
            // 'a' as the binding name. This is equivalent to
            // |import { default as a } from 'b'|.
            Node importName = newName(context->names().default_);
            if (!importName) {
                return null();
            }
=======
    handler_.addList(importSpecSet, importSpec);
  }
>>>>>>> upstream-releases

<<<<<<< HEAD
  if (!pc->atModuleLevel()) {
    error(JSMSG_IMPORT_DECL_AT_TOP_LEVEL);
    return null();
  }
||||||| merged common ancestors
            RootedPropertyName bindingAtom(context, importedBinding());
            if (!bindingAtom) {
                return null();
            }
=======
  return true;
}
>>>>>>> upstream-releases

<<<<<<< HEAD
  uint32_t begin = pos().begin;
  TokenKind tt;
  if (!tokenStream.getToken(&tt)) {
    return null();
  }
||||||| merged common ancestors
            Node bindingName = newName(bindingAtom);
            if (!bindingName) {
                return null();
            }
=======
template <typename Unit>
BinaryNode* Parser<FullParseHandler, Unit>::importDeclaration() {
  MOZ_ASSERT(anyChars.isCurrentTokenType(TokenKind::Import));
>>>>>>> upstream-releases

<<<<<<< HEAD
  ListNodeType importSpecSet =
      handler.newList(ParseNodeKind::ImportSpecList, pos());
  if (!importSpecSet) {
    return null();
  }
||||||| merged common ancestors
            if (!noteDeclaredName(bindingAtom, DeclarationKind::Import, pos())) {
                return null();
            }
=======
  if (!pc_->atModuleLevel()) {
    error(JSMSG_IMPORT_DECL_AT_TOP_LEVEL);
    return null();
  }
>>>>>>> upstream-releases

<<<<<<< HEAD
  if (tt == TokenKind::String) {
    // Handle the form |import 'a'| by leaving the list empty. This is
    // equivalent to |import {} from 'a'|.
    importSpecSet->pn_pos.end = importSpecSet->pn_pos.begin;
  } else {
    if (tt == TokenKind::LeftCurly || tt == TokenKind::Mul) {
      if (!namedImportsOrNamespaceImport(tt, importSpecSet)) {
        return null();
      }
    } else if (TokenKindIsPossibleIdentifierName(tt)) {
      // Handle the form |import a from 'b'|, by adding a single import
      // specifier to the list, with 'default' as the import name and
      // 'a' as the binding name. This is equivalent to
      // |import { default as a } from 'b'|.
      Node importName = newName(context->names().default_);
      if (!importName) {
        return null();
      }
||||||| merged common ancestors
            BinaryNodeType importSpec = handler.newImportSpec(importName, bindingName);
            if (!importSpec) {
                return null();
            }
=======
  uint32_t begin = pos().begin;
  TokenKind tt;
  if (!tokenStream.getToken(&tt)) {
    return null();
  }
>>>>>>> upstream-releases

<<<<<<< HEAD
      RootedPropertyName bindingAtom(context, importedBinding());
      if (!bindingAtom) {
        return null();
      }
||||||| merged common ancestors
            handler.addList(importSpecSet, importSpec);
=======
  ListNodeType importSpecSet =
      handler_.newList(ParseNodeKind::ImportSpecList, pos());
  if (!importSpecSet) {
    return null();
  }
>>>>>>> upstream-releases

<<<<<<< HEAD
      Node bindingName = newName(bindingAtom);
      if (!bindingName) {
        return null();
      }

      if (!noteDeclaredName(bindingAtom, DeclarationKind::Import, pos())) {
        return null();
      }
||||||| merged common ancestors
            if (!tokenStream.peekToken(&tt)) {
                return null();
            }
=======
  if (tt == TokenKind::String) {
    // Handle the form |import 'a'| by leaving the list empty. This is
    // equivalent to |import {} from 'a'|.
    importSpecSet->pn_pos.end = importSpecSet->pn_pos.begin;
  } else {
    if (tt == TokenKind::LeftCurly || tt == TokenKind::Mul) {
      if (!namedImportsOrNamespaceImport(tt, importSpecSet)) {
        return null();
      }
    } else if (TokenKindIsPossibleIdentifierName(tt)) {
      // Handle the form |import a from 'b'|, by adding a single import
      // specifier to the list, with 'default' as the import name and
      // 'a' as the binding name. This is equivalent to
      // |import { default as a } from 'b'|.
      NameNodeType importName = newName(cx_->names().default_);
      if (!importName) {
        return null();
      }
>>>>>>> upstream-releases

<<<<<<< HEAD
      BinaryNodeType importSpec =
          handler.newImportSpec(importName, bindingName);
      if (!importSpec) {
        return null();
      }
||||||| merged common ancestors
            if (tt == TokenKind::Comma) {
                tokenStream.consumeKnownToken(tt);
                if (!tokenStream.getToken(&tt)) {
                    return null();
                }
=======
      RootedPropertyName bindingAtom(cx_, importedBinding());
      if (!bindingAtom) {
        return null();
      }
>>>>>>> upstream-releases

<<<<<<< HEAD
      handler.addList(importSpecSet, importSpec);
||||||| merged common ancestors
                if (tt != TokenKind::LeftCurly && tt != TokenKind::Mul) {
                    error(JSMSG_NAMED_IMPORTS_OR_NAMESPACE_IMPORT);
                    return null();
                }
=======
      NameNodeType bindingName = newName(bindingAtom);
      if (!bindingName) {
        return null();
      }
>>>>>>> upstream-releases

<<<<<<< HEAD
      if (!tokenStream.peekToken(&tt)) {
        return null();
      }

      if (tt == TokenKind::Comma) {
        tokenStream.consumeKnownToken(tt);
        if (!tokenStream.getToken(&tt)) {
          return null();
        }
||||||| merged common ancestors
                if (!namedImportsOrNamespaceImport(tt, importSpecSet)) {
                    return null();
                }
            }
        } else {
            error(JSMSG_DECLARATION_AFTER_IMPORT);
            return null();
        }
=======
      if (!noteDeclaredName(bindingAtom, DeclarationKind::Import, pos())) {
        return null();
      }
>>>>>>> upstream-releases

<<<<<<< HEAD
        if (tt != TokenKind::LeftCurly && tt != TokenKind::Mul) {
          error(JSMSG_NAMED_IMPORTS_OR_NAMESPACE_IMPORT);
          return null();
        }
||||||| merged common ancestors
        MUST_MATCH_TOKEN(TokenKind::From, JSMSG_FROM_AFTER_IMPORT_CLAUSE);
=======
      BinaryNodeType importSpec =
          handler_.newImportSpec(importName, bindingName);
      if (!importSpec) {
        return null();
      }
>>>>>>> upstream-releases

<<<<<<< HEAD
        if (!namedImportsOrNamespaceImport(tt, importSpecSet)) {
          return null();
        }
      }
    } else {
      error(JSMSG_DECLARATION_AFTER_IMPORT);
      return null();
    }
||||||| merged common ancestors
        MUST_MATCH_TOKEN(TokenKind::String, JSMSG_MODULE_SPEC_AFTER_FROM);
    }
=======
      handler_.addList(importSpecSet, importSpec);
>>>>>>> upstream-releases

<<<<<<< HEAD
    if (!mustMatchToken(TokenKind::From, JSMSG_FROM_AFTER_IMPORT_CLAUSE)) {
      return null();
||||||| merged common ancestors
    NameNodeType moduleSpec = stringLiteral();
    if (!moduleSpec) {
        return null();
=======
      if (!tokenStream.peekToken(&tt)) {
        return null();
      }

      if (tt == TokenKind::Comma) {
        tokenStream.consumeKnownToken(tt);
        if (!tokenStream.getToken(&tt)) {
          return null();
        }

        if (tt != TokenKind::LeftCurly && tt != TokenKind::Mul) {
          error(JSMSG_NAMED_IMPORTS_OR_NAMESPACE_IMPORT);
          return null();
        }

        if (!namedImportsOrNamespaceImport(tt, importSpecSet)) {
          return null();
        }
      }
    } else {
      error(JSMSG_DECLARATION_AFTER_IMPORT);
      return null();
>>>>>>> upstream-releases
    }

<<<<<<< HEAD
    if (!mustMatchToken(TokenKind::String, JSMSG_MODULE_SPEC_AFTER_FROM)) {
      return null();
||||||| merged common ancestors
    if (!matchOrInsertSemicolon()) {
        return null();
=======
    if (!mustMatchToken(TokenKind::From, JSMSG_FROM_AFTER_IMPORT_CLAUSE)) {
      return null();
>>>>>>> upstream-releases
    }
  }

<<<<<<< HEAD
  NameNodeType moduleSpec = stringLiteral();
  if (!moduleSpec) {
    return null();
  }

  if (!matchOrInsertSemicolon()) {
    return null();
  }

  BinaryNode* node = handler.newImportDeclaration(importSpecSet, moduleSpec,
                                                  TokenPos(begin, pos().end));
  if (!node || !pc->sc()->asModuleContext()->builder.processImport(node)) {
    return null();
  }
||||||| merged common ancestors
    BinaryNode* node =
        handler.newImportDeclaration(importSpecSet, moduleSpec, TokenPos(begin, pos().end));
    if (!node || !pc->sc()->asModuleContext()->builder.processImport(node)) {
        return null();
    }
=======
    if (!mustMatchToken(TokenKind::String, JSMSG_MODULE_SPEC_AFTER_FROM)) {
      return null();
    }
  }

  NameNodeType moduleSpec = stringLiteral();
  if (!moduleSpec) {
    return null();
  }
>>>>>>> upstream-releases

<<<<<<< HEAD
  return node;
||||||| merged common ancestors
    return node;
=======
  if (!matchOrInsertSemicolon()) {
    return null();
  }

  BinaryNode* node = handler_.newImportDeclaration(importSpecSet, moduleSpec,
                                                   TokenPos(begin, pos().end));
  if (!node || !pc_->sc()->asModuleContext()->builder.processImport(node)) {
    return null();
  }

  return node;
>>>>>>> upstream-releases
}

template <typename Unit>
inline SyntaxParseHandler::BinaryNodeType
Parser<SyntaxParseHandler, Unit>::importDeclaration() {
  MOZ_ALWAYS_FALSE(abortIfSyntaxParser());
  return SyntaxParseHandler::NodeFailure;
}

template <class ParseHandler, typename Unit>
inline typename ParseHandler::BinaryNodeType
GeneralParser<ParseHandler, Unit>::importDeclaration() {
  return asFinalParser()->importDeclaration();
}

template <class ParseHandler, typename Unit>
inline typename ParseHandler::Node
GeneralParser<ParseHandler, Unit>::importDeclarationOrImportExpr(
    YieldHandling yieldHandling) {
  MOZ_ASSERT(anyChars.isCurrentTokenType(TokenKind::Import));

  TokenKind tt;
  if (!tokenStream.peekToken(&tt)) {
    return null();
  }

  if (tt == TokenKind::Dot || tt == TokenKind::LeftParen) {
    return expressionStatement(yieldHandling);
  }

  return importDeclaration();
}

<<<<<<< HEAD
template <typename Unit>
bool Parser<FullParseHandler, Unit>::checkExportedName(JSAtom* exportName) {
  if (!pc->sc()->asModuleContext()->builder.hasExportedName(exportName)) {
    return true;
  }
||||||| merged common ancestors
template<typename Unit>
bool
Parser<FullParseHandler, Unit>::checkExportedName(JSAtom* exportName)
{
    if (!pc->sc()->asModuleContext()->builder.hasExportedName(exportName)) {
        return true;
    }

    UniqueChars str = AtomToPrintableString(context, exportName);
    if (!str) {
        return false;
    }
=======
template <typename Unit>
bool Parser<FullParseHandler, Unit>::checkExportedName(JSAtom* exportName) {
  if (!pc_->sc()->asModuleContext()->builder.hasExportedName(exportName)) {
    return true;
  }
>>>>>>> upstream-releases

<<<<<<< HEAD
  UniqueChars str = AtomToPrintableString(context, exportName);
  if (!str) {
||||||| merged common ancestors
    error(JSMSG_DUPLICATE_EXPORT_NAME, str.get());
=======
  UniqueChars str = AtomToPrintableString(cx_, exportName);
  if (!str) {
>>>>>>> upstream-releases
    return false;
  }

  error(JSMSG_DUPLICATE_EXPORT_NAME, str.get());
  return false;
}

template <typename Unit>
inline bool Parser<SyntaxParseHandler, Unit>::checkExportedName(
    JSAtom* exportName) {
  MOZ_ALWAYS_FALSE(abortIfSyntaxParser());
  return false;
}

template <class ParseHandler, typename Unit>
inline bool GeneralParser<ParseHandler, Unit>::checkExportedName(
    JSAtom* exportName) {
  return asFinalParser()->checkExportedName(exportName);
}

<<<<<<< HEAD
template <typename Unit>
bool Parser<FullParseHandler, Unit>::checkExportedNamesForArrayBinding(
    ListNode* array) {
  MOZ_ASSERT(array->isKind(ParseNodeKind::Array));
||||||| merged common ancestors
template<typename Unit>
bool
Parser<FullParseHandler, Unit>::checkExportedNamesForArrayBinding(ListNode* array)
{
    MOZ_ASSERT(array->isKind(ParseNodeKind::Array));
=======
template <typename Unit>
bool Parser<FullParseHandler, Unit>::checkExportedNamesForArrayBinding(
    ListNode* array) {
  MOZ_ASSERT(array->isKind(ParseNodeKind::ArrayExpr));
>>>>>>> upstream-releases

  for (ParseNode* node : array->contents()) {
    if (node->isKind(ParseNodeKind::Elision)) {
      continue;
    }

<<<<<<< HEAD
    ParseNode* binding;
    if (node->isKind(ParseNodeKind::Spread)) {
      binding = node->as<UnaryNode>().kid();
    } else if (node->isKind(ParseNodeKind::Assign)) {
      binding = node->as<AssignmentNode>().left();
    } else {
      binding = node;
    }
||||||| merged common ancestors
        ParseNode* binding;
        if (node->isKind(ParseNodeKind::Spread)) {
            binding = node->as<UnaryNode>().kid();
        } else if (node->isKind(ParseNodeKind::Assign)) {
            binding = node->as<AssignmentNode>().left();
        } else {
            binding = node;
        }
=======
    ParseNode* binding;
    if (node->isKind(ParseNodeKind::Spread)) {
      binding = node->as<UnaryNode>().kid();
    } else if (node->isKind(ParseNodeKind::AssignExpr)) {
      binding = node->as<AssignmentNode>().left();
    } else {
      binding = node;
    }
>>>>>>> upstream-releases

    if (!checkExportedNamesForDeclaration(binding)) {
      return false;
    }
  }

  return true;
}

template <typename Unit>
inline bool Parser<SyntaxParseHandler, Unit>::checkExportedNamesForArrayBinding(
    ListNodeType array) {
  MOZ_ALWAYS_FALSE(abortIfSyntaxParser());
  return false;
}

template <class ParseHandler, typename Unit>
inline bool
GeneralParser<ParseHandler, Unit>::checkExportedNamesForArrayBinding(
    ListNodeType array) {
  return asFinalParser()->checkExportedNamesForArrayBinding(array);
}

<<<<<<< HEAD
template <typename Unit>
bool Parser<FullParseHandler, Unit>::checkExportedNamesForObjectBinding(
    ListNode* obj) {
  MOZ_ASSERT(obj->isKind(ParseNodeKind::Object));

  for (ParseNode* node : obj->contents()) {
    MOZ_ASSERT(node->isKind(ParseNodeKind::MutateProto) ||
               node->isKind(ParseNodeKind::Colon) ||
               node->isKind(ParseNodeKind::Shorthand) ||
               node->isKind(ParseNodeKind::Spread));

    ParseNode* target;
    if (node->isKind(ParseNodeKind::Spread)) {
      target = node->as<UnaryNode>().kid();
    } else {
      if (node->isKind(ParseNodeKind::MutateProto)) {
        target = node->as<UnaryNode>().kid();
      } else {
        target = node->as<BinaryNode>().right();
      }
||||||| merged common ancestors
            if (target->isKind(ParseNodeKind::Assign)) {
                target = target->as<AssignmentNode>().left();
            }
        }
=======
template <typename Unit>
bool Parser<FullParseHandler, Unit>::checkExportedNamesForObjectBinding(
    ListNode* obj) {
  MOZ_ASSERT(obj->isKind(ParseNodeKind::ObjectExpr));

  for (ParseNode* node : obj->contents()) {
    MOZ_ASSERT(node->isKind(ParseNodeKind::MutateProto) ||
               node->isKind(ParseNodeKind::PropertyDefinition) ||
               node->isKind(ParseNodeKind::Shorthand) ||
               node->isKind(ParseNodeKind::Spread));

    ParseNode* target;
    if (node->isKind(ParseNodeKind::Spread)) {
      target = node->as<UnaryNode>().kid();
    } else {
      if (node->isKind(ParseNodeKind::MutateProto)) {
        target = node->as<UnaryNode>().kid();
      } else {
        target = node->as<BinaryNode>().right();
      }
>>>>>>> upstream-releases

<<<<<<< HEAD
      if (target->isKind(ParseNodeKind::Assign)) {
        target = target->as<AssignmentNode>().left();
      }
||||||| merged common ancestors
        if (!checkExportedNamesForDeclaration(target)) {
            return false;
        }
=======
      if (target->isKind(ParseNodeKind::AssignExpr)) {
        target = target->as<AssignmentNode>().left();
      }
>>>>>>> upstream-releases
    }

    if (!checkExportedNamesForDeclaration(target)) {
      return false;
    }
  }

  return true;
}

template <typename Unit>
inline bool Parser<SyntaxParseHandler,
                   Unit>::checkExportedNamesForObjectBinding(ListNodeType obj) {
  MOZ_ALWAYS_FALSE(abortIfSyntaxParser());
  return false;
}

template <class ParseHandler, typename Unit>
inline bool
GeneralParser<ParseHandler, Unit>::checkExportedNamesForObjectBinding(
    ListNodeType obj) {
  return asFinalParser()->checkExportedNamesForObjectBinding(obj);
}

<<<<<<< HEAD
template <typename Unit>
bool Parser<FullParseHandler, Unit>::checkExportedNamesForDeclaration(
    ParseNode* node) {
  if (node->isKind(ParseNodeKind::Name)) {
    if (!checkExportedName(node->as<NameNode>().atom())) {
      return false;
    }
  } else if (node->isKind(ParseNodeKind::Array)) {
    if (!checkExportedNamesForArrayBinding(&node->as<ListNode>())) {
      return false;
||||||| merged common ancestors
template<typename Unit>
bool
Parser<FullParseHandler, Unit>::checkExportedNamesForDeclaration(ParseNode* node)
{
    if (node->isKind(ParseNodeKind::Name)) {
        if (!checkExportedName(node->as<NameNode>().atom())) {
            return false;
        }
    } else if (node->isKind(ParseNodeKind::Array)) {
        if (!checkExportedNamesForArrayBinding(&node->as<ListNode>())) {
            return false;
        }
    } else {
        MOZ_ASSERT(node->isKind(ParseNodeKind::Object));
        if (!checkExportedNamesForObjectBinding(&node->as<ListNode>())) {
            return false;
        }
=======
template <typename Unit>
bool Parser<FullParseHandler, Unit>::checkExportedNamesForDeclaration(
    ParseNode* node) {
  if (node->isKind(ParseNodeKind::Name)) {
    if (!checkExportedName(node->as<NameNode>().atom())) {
      return false;
    }
  } else if (node->isKind(ParseNodeKind::ArrayExpr)) {
    if (!checkExportedNamesForArrayBinding(&node->as<ListNode>())) {
      return false;
>>>>>>> upstream-releases
    }
<<<<<<< HEAD
  } else {
    MOZ_ASSERT(node->isKind(ParseNodeKind::Object));
    if (!checkExportedNamesForObjectBinding(&node->as<ListNode>())) {
      return false;
    }
  }
||||||| merged common ancestors
=======
  } else {
    MOZ_ASSERT(node->isKind(ParseNodeKind::ObjectExpr));
    if (!checkExportedNamesForObjectBinding(&node->as<ListNode>())) {
      return false;
    }
  }
>>>>>>> upstream-releases

  return true;
}

template <typename Unit>
inline bool Parser<SyntaxParseHandler, Unit>::checkExportedNamesForDeclaration(
    Node node) {
  MOZ_ALWAYS_FALSE(abortIfSyntaxParser());
  return false;
}

template <class ParseHandler, typename Unit>
inline bool GeneralParser<ParseHandler, Unit>::checkExportedNamesForDeclaration(
    Node node) {
  return asFinalParser()->checkExportedNamesForDeclaration(node);
}

<<<<<<< HEAD
template <typename Unit>
bool Parser<FullParseHandler, Unit>::checkExportedNamesForDeclarationList(
    ListNode* node) {
  for (ParseNode* binding : node->contents()) {
    if (binding->isKind(ParseNodeKind::Assign)) {
      binding = binding->as<AssignmentNode>().left();
    } else {
      MOZ_ASSERT(binding->isKind(ParseNodeKind::Name));
||||||| merged common ancestors
        if (!checkExportedNamesForDeclaration(binding)) {
            return false;
        }
=======
template <typename Unit>
bool Parser<FullParseHandler, Unit>::checkExportedNamesForDeclarationList(
    ListNode* node) {
  for (ParseNode* binding : node->contents()) {
    if (binding->isKind(ParseNodeKind::AssignExpr)) {
      binding = binding->as<AssignmentNode>().left();
    } else {
      MOZ_ASSERT(binding->isKind(ParseNodeKind::Name));
>>>>>>> upstream-releases
    }

    if (!checkExportedNamesForDeclaration(binding)) {
      return false;
    }
  }

  return true;
}

template <typename Unit>
inline bool
Parser<SyntaxParseHandler, Unit>::checkExportedNamesForDeclarationList(
    ListNodeType node) {
  MOZ_ALWAYS_FALSE(abortIfSyntaxParser());
  return false;
}

template <class ParseHandler, typename Unit>
inline bool
GeneralParser<ParseHandler, Unit>::checkExportedNamesForDeclarationList(
    ListNodeType node) {
  return asFinalParser()->checkExportedNamesForDeclarationList(node);
}

template <typename Unit>
inline bool Parser<FullParseHandler, Unit>::checkExportedNameForClause(
    NameNode* nameNode) {
  return checkExportedName(nameNode->atom());
}

template <typename Unit>
inline bool Parser<SyntaxParseHandler, Unit>::checkExportedNameForClause(
    NameNodeType nameNode) {
  MOZ_ALWAYS_FALSE(abortIfSyntaxParser());
  return false;
}

template <class ParseHandler, typename Unit>
inline bool GeneralParser<ParseHandler, Unit>::checkExportedNameForClause(
    NameNodeType nameNode) {
  return asFinalParser()->checkExportedNameForClause(nameNode);
}

<<<<<<< HEAD
template <typename Unit>
bool Parser<FullParseHandler, Unit>::checkExportedNameForFunction(
    CodeNode* funNode) {
  return checkExportedName(funNode->funbox()->function()->explicitName());
||||||| merged common ancestors
template<typename Unit>
bool
Parser<FullParseHandler, Unit>::checkExportedNameForFunction(CodeNode* funNode)
{
    return checkExportedName(funNode->funbox()->function()->explicitName());
=======
template <typename Unit>
bool Parser<FullParseHandler, Unit>::checkExportedNameForFunction(
    FunctionNode* funNode) {
  return checkExportedName(funNode->funbox()->explicitName());
>>>>>>> upstream-releases
}

<<<<<<< HEAD
template <typename Unit>
inline bool Parser<SyntaxParseHandler, Unit>::checkExportedNameForFunction(
    CodeNodeType funNode) {
  MOZ_ALWAYS_FALSE(abortIfSyntaxParser());
  return false;
||||||| merged common ancestors
template<typename Unit>
inline bool
Parser<SyntaxParseHandler, Unit>::checkExportedNameForFunction(CodeNodeType funNode)
{
    MOZ_ALWAYS_FALSE(abortIfSyntaxParser());
    return false;
=======
template <typename Unit>
inline bool Parser<SyntaxParseHandler, Unit>::checkExportedNameForFunction(
    FunctionNodeType funNode) {
  MOZ_ALWAYS_FALSE(abortIfSyntaxParser());
  return false;
>>>>>>> upstream-releases
}

<<<<<<< HEAD
template <class ParseHandler, typename Unit>
inline bool GeneralParser<ParseHandler, Unit>::checkExportedNameForFunction(
    CodeNodeType funNode) {
  return asFinalParser()->checkExportedNameForFunction(funNode);
||||||| merged common ancestors
template<class ParseHandler, typename Unit>
inline bool
GeneralParser<ParseHandler, Unit>::checkExportedNameForFunction(CodeNodeType funNode)
{
    return asFinalParser()->checkExportedNameForFunction(funNode);
=======
template <class ParseHandler, typename Unit>
inline bool GeneralParser<ParseHandler, Unit>::checkExportedNameForFunction(
    FunctionNodeType funNode) {
  return asFinalParser()->checkExportedNameForFunction(funNode);
>>>>>>> upstream-releases
}

template <typename Unit>
bool Parser<FullParseHandler, Unit>::checkExportedNameForClass(
    ClassNode* classNode) {
  MOZ_ASSERT(classNode->names());
  return checkExportedName(classNode->names()->innerBinding()->atom());
}

template <typename Unit>
inline bool Parser<SyntaxParseHandler, Unit>::checkExportedNameForClass(
    ClassNodeType classNode) {
  MOZ_ALWAYS_FALSE(abortIfSyntaxParser());
  return false;
}

template <class ParseHandler, typename Unit>
inline bool GeneralParser<ParseHandler, Unit>::checkExportedNameForClass(
    ClassNodeType classNode) {
  return asFinalParser()->checkExportedNameForClass(classNode);
}

<<<<<<< HEAD
template <>
inline bool PerHandlerParser<FullParseHandler>::processExport(ParseNode* node) {
  return pc->sc()->asModuleContext()->builder.processExport(node);
||||||| merged common ancestors
template<>
inline bool
PerHandlerParser<FullParseHandler>::processExport(ParseNode* node)
{
    return pc->sc()->asModuleContext()->builder.processExport(node);
=======
template <>
inline bool PerHandlerParser<FullParseHandler>::processExport(ParseNode* node) {
  return pc_->sc()->asModuleContext()->builder.processExport(node);
>>>>>>> upstream-releases
}

template <>
inline bool PerHandlerParser<SyntaxParseHandler>::processExport(Node node) {
  MOZ_ALWAYS_FALSE(abortIfSyntaxParser());
  return false;
}

<<<<<<< HEAD
template <>
inline bool PerHandlerParser<FullParseHandler>::processExportFrom(
    BinaryNodeType node) {
  return pc->sc()->asModuleContext()->builder.processExportFrom(node);
||||||| merged common ancestors
template<>
inline bool
PerHandlerParser<FullParseHandler>::processExportFrom(BinaryNodeType node)
{
    return pc->sc()->asModuleContext()->builder.processExportFrom(node);
=======
template <>
inline bool PerHandlerParser<FullParseHandler>::processExportFrom(
    BinaryNodeType node) {
  return pc_->sc()->asModuleContext()->builder.processExportFrom(node);
>>>>>>> upstream-releases
}

template <>
inline bool PerHandlerParser<SyntaxParseHandler>::processExportFrom(
    BinaryNodeType node) {
  MOZ_ALWAYS_FALSE(abortIfSyntaxParser());
  return false;
}

template <class ParseHandler, typename Unit>
typename ParseHandler::BinaryNodeType
GeneralParser<ParseHandler, Unit>::exportFrom(uint32_t begin, Node specList) {
  if (!abortIfSyntaxParser()) {
    return null();
  }

  MOZ_ASSERT(anyChars.isCurrentTokenType(TokenKind::From));

  if (!abortIfSyntaxParser()) {
    return null();
  }

  if (!mustMatchToken(TokenKind::String, JSMSG_MODULE_SPEC_AFTER_FROM)) {
    return null();
  }

  NameNodeType moduleSpec = stringLiteral();
  if (!moduleSpec) {
    return null();
  }

  if (!matchOrInsertSemicolon()) {
    return null();
  }

<<<<<<< HEAD
  BinaryNodeType node =
      handler.newExportFromDeclaration(begin, specList, moduleSpec);
  if (!node) {
    return null();
  }
||||||| merged common ancestors
    BinaryNodeType node = handler.newExportFromDeclaration(begin, specList, moduleSpec);
    if (!node) {
        return null();
    }
=======
  BinaryNodeType node =
      handler_.newExportFromDeclaration(begin, specList, moduleSpec);
  if (!node) {
    return null();
  }
>>>>>>> upstream-releases

  if (!processExportFrom(node)) {
    return null();
  }

  return node;
}

template <class ParseHandler, typename Unit>
typename ParseHandler::BinaryNodeType
GeneralParser<ParseHandler, Unit>::exportBatch(uint32_t begin) {
  if (!abortIfSyntaxParser()) {
    return null();
  }

  MOZ_ASSERT(anyChars.isCurrentTokenType(TokenKind::Mul));

<<<<<<< HEAD
  ListNodeType kid = handler.newList(ParseNodeKind::ExportSpecList, pos());
  if (!kid) {
    return null();
  }
||||||| merged common ancestors
    ListNodeType kid = handler.newList(ParseNodeKind::ExportSpecList, pos());
    if (!kid) {
        return null();
    }
=======
  ListNodeType kid = handler_.newList(ParseNodeKind::ExportSpecList, pos());
  if (!kid) {
    return null();
  }
>>>>>>> upstream-releases

<<<<<<< HEAD
  // Handle the form |export *| by adding a special export batch
  // specifier to the list.
  NullaryNodeType exportSpec = handler.newExportBatchSpec(pos());
  if (!exportSpec) {
    return null();
  }
||||||| merged common ancestors
    // Handle the form |export *| by adding a special export batch
    // specifier to the list.
    NullaryNodeType exportSpec = handler.newExportBatchSpec(pos());
    if (!exportSpec) {
        return null();
    }
=======
  // Handle the form |export *| by adding a special export batch
  // specifier to the list.
  NullaryNodeType exportSpec = handler_.newExportBatchSpec(pos());
  if (!exportSpec) {
    return null();
  }
>>>>>>> upstream-releases

<<<<<<< HEAD
  handler.addList(kid, exportSpec);
||||||| merged common ancestors
    handler.addList(kid, exportSpec);
=======
  handler_.addList(kid, exportSpec);
>>>>>>> upstream-releases

  if (!mustMatchToken(TokenKind::From, JSMSG_FROM_AFTER_EXPORT_STAR)) {
    return null();
  }

  return exportFrom(begin, kid);
}

template <typename Unit>
bool Parser<FullParseHandler, Unit>::checkLocalExportNames(ListNode* node) {
  // ES 2017 draft 15.2.3.1.
  for (ParseNode* next : node->contents()) {
    ParseNode* name = next->as<BinaryNode>().left();
    MOZ_ASSERT(name->isKind(ParseNodeKind::Name));

<<<<<<< HEAD
    RootedPropertyName ident(context,
                             name->as<NameNode>().atom()->asPropertyName());
    if (!checkLocalExportName(ident, name->pn_pos.begin)) {
      return false;
||||||| merged common ancestors
        RootedPropertyName ident(context, name->as<NameNode>().atom()->asPropertyName());
        if (!checkLocalExportName(ident, name->pn_pos.begin)) {
            return false;
        }
=======
    RootedPropertyName ident(cx_,
                             name->as<NameNode>().atom()->asPropertyName());
    if (!checkLocalExportName(ident, name->pn_pos.begin)) {
      return false;
>>>>>>> upstream-releases
    }
  }

<<<<<<< HEAD
  return true;
||||||| merged common ancestors
    return true;
}

template<typename Unit>
bool
Parser<SyntaxParseHandler, Unit>::checkLocalExportNames(ListNodeType node)
{
    MOZ_ALWAYS_FALSE(abortIfSyntaxParser());
    return false;
}

template<class ParseHandler, typename Unit>
inline bool
GeneralParser<ParseHandler, Unit>::checkLocalExportNames(ListNodeType node)
{
    return asFinalParser()->checkLocalExportNames(node);
=======
  return true;
}

template <typename Unit>
bool Parser<SyntaxParseHandler, Unit>::checkLocalExportNames(
    ListNodeType node) {
  MOZ_ALWAYS_FALSE(abortIfSyntaxParser());
  return false;
>>>>>>> upstream-releases
}

<<<<<<< HEAD
template <typename Unit>
bool Parser<SyntaxParseHandler, Unit>::checkLocalExportNames(
    ListNodeType node) {
  MOZ_ALWAYS_FALSE(abortIfSyntaxParser());
  return false;
}
||||||| merged common ancestors
template <class ParseHandler, typename Unit>
typename ParseHandler::Node
GeneralParser<ParseHandler, Unit>::exportClause(uint32_t begin)
{
    if (!abortIfSyntaxParser()) {
        return null();
    }

    MOZ_ASSERT(anyChars.isCurrentTokenType(TokenKind::LeftCurly));

    ListNodeType kid = handler.newList(ParseNodeKind::ExportSpecList, pos());
    if (!kid) {
        return null();
    }

    TokenKind tt;
    while (true) {
        // Handle the forms |export {}| and |export { ..., }| (where ... is non
        // empty), by escaping the loop early if the next token is }.
        if (!tokenStream.getToken(&tt)) {
            return null();
        }

        if (tt == TokenKind::RightCurly) {
            break;
        }

        if (!TokenKindIsPossibleIdentifierName(tt)) {
            error(JSMSG_NO_BINDING_NAME);
            return null();
        }

        NameNodeType bindingName = newName(anyChars.currentName());
        if (!bindingName) {
            return null();
        }

        bool foundAs;
        if (!tokenStream.matchToken(&foundAs, TokenKind::As)) {
            return null();
        }
        if (foundAs) {
            MUST_MATCH_TOKEN_FUNC(TokenKindIsPossibleIdentifierName, JSMSG_NO_EXPORT_NAME);
        }

        NameNodeType exportName = newName(anyChars.currentName());
        if (!exportName) {
            return null();
        }

        if (!checkExportedNameForClause(exportName)) {
            return null();
        }
=======
template <class ParseHandler, typename Unit>
inline bool GeneralParser<ParseHandler, Unit>::checkLocalExportNames(
    ListNodeType node) {
  return asFinalParser()->checkLocalExportNames(node);
}
>>>>>>> upstream-releases

<<<<<<< HEAD
template <class ParseHandler, typename Unit>
inline bool GeneralParser<ParseHandler, Unit>::checkLocalExportNames(
    ListNodeType node) {
  return asFinalParser()->checkLocalExportNames(node);
}
||||||| merged common ancestors
        BinaryNodeType exportSpec = handler.newExportSpec(bindingName, exportName);
        if (!exportSpec) {
            return null();
        }
=======
template <class ParseHandler, typename Unit>
typename ParseHandler::Node GeneralParser<ParseHandler, Unit>::exportClause(
    uint32_t begin) {
  if (!abortIfSyntaxParser()) {
    return null();
  }
>>>>>>> upstream-releases

<<<<<<< HEAD
template <class ParseHandler, typename Unit>
typename ParseHandler::Node GeneralParser<ParseHandler, Unit>::exportClause(
    uint32_t begin) {
  if (!abortIfSyntaxParser()) {
    return null();
  }
||||||| merged common ancestors
        handler.addList(kid, exportSpec);
=======
  MOZ_ASSERT(anyChars.isCurrentTokenType(TokenKind::LeftCurly));
>>>>>>> upstream-releases

<<<<<<< HEAD
  MOZ_ASSERT(anyChars.isCurrentTokenType(TokenKind::LeftCurly));
||||||| merged common ancestors
        TokenKind next;
        if (!tokenStream.getToken(&next)) {
            return null();
        }
=======
  ListNodeType kid = handler_.newList(ParseNodeKind::ExportSpecList, pos());
  if (!kid) {
    return null();
  }
>>>>>>> upstream-releases

<<<<<<< HEAD
  ListNodeType kid = handler.newList(ParseNodeKind::ExportSpecList, pos());
  if (!kid) {
    return null();
  }
||||||| merged common ancestors
        if (next == TokenKind::RightCurly) {
            break;
        }
=======
  TokenKind tt;
  while (true) {
    // Handle the forms |export {}| and |export { ..., }| (where ... is non
    // empty), by escaping the loop early if the next token is }.
    if (!tokenStream.getToken(&tt)) {
      return null();
    }
>>>>>>> upstream-releases

<<<<<<< HEAD
  TokenKind tt;
  while (true) {
    // Handle the forms |export {}| and |export { ..., }| (where ... is non
    // empty), by escaping the loop early if the next token is }.
    if (!tokenStream.getToken(&tt)) {
      return null();
||||||| merged common ancestors
        if (next != TokenKind::Comma) {
            error(JSMSG_RC_AFTER_EXPORT_SPEC_LIST);
            return null();
        }
=======
    if (tt == TokenKind::RightCurly) {
      break;
>>>>>>> upstream-releases
    }

<<<<<<< HEAD
    if (tt == TokenKind::RightCurly) {
      break;
||||||| merged common ancestors
    // Careful!  If |from| follows, even on a new line, it must start a
    // FromClause:
    //
    //   export { x }
    //   from "foo"; // a single ExportDeclaration
    //
    // But if it doesn't, we might have an ASI opportunity in Operand context:
    //
    //   export { x }   // ExportDeclaration, terminated by ASI
    //   fro\u006D      // ExpressionStatement, the name "from"
    //
    // In that case let matchOrInsertSemicolon sort out ASI or any necessary
    // error.
    bool matched;
    if (!tokenStream.matchToken(&matched, TokenKind::From, TokenStream::Operand)) {
        return null();
=======
    if (!TokenKindIsPossibleIdentifierName(tt)) {
      error(JSMSG_NO_BINDING_NAME);
      return null();
>>>>>>> upstream-releases
    }

<<<<<<< HEAD
    if (!TokenKindIsPossibleIdentifierName(tt)) {
      error(JSMSG_NO_BINDING_NAME);
      return null();
||||||| merged common ancestors
    if (matched) {
        return exportFrom(begin, kid);
=======
    NameNodeType bindingName = newName(anyChars.currentName());
    if (!bindingName) {
      return null();
>>>>>>> upstream-releases
    }

<<<<<<< HEAD
    NameNodeType bindingName = newName(anyChars.currentName());
    if (!bindingName) {
      return null();
||||||| merged common ancestors
    if (!matchOrInsertSemicolon()) {
        return null();
=======
    bool foundAs;
    if (!tokenStream.matchToken(&foundAs, TokenKind::As)) {
      return null();
    }
    if (foundAs) {
      if (!mustMatchToken(TokenKindIsPossibleIdentifierName,
                          JSMSG_NO_EXPORT_NAME)) {
        return null();
      }
>>>>>>> upstream-releases
    }

<<<<<<< HEAD
    bool foundAs;
    if (!tokenStream.matchToken(&foundAs, TokenKind::As)) {
      return null();
    }
    if (foundAs) {
      if (!mustMatchToken(TokenKindIsPossibleIdentifierName,
                          JSMSG_NO_EXPORT_NAME)) {
        return null();
      }
||||||| merged common ancestors
    if (!checkLocalExportNames(kid)) {
        return null();
=======
    NameNodeType exportName = newName(anyChars.currentName());
    if (!exportName) {
      return null();
>>>>>>> upstream-releases
    }

<<<<<<< HEAD
    NameNodeType exportName = newName(anyChars.currentName());
    if (!exportName) {
      return null();
||||||| merged common ancestors
    UnaryNodeType node = handler.newExportDeclaration(kid, TokenPos(begin, pos().end));
    if (!node) {
        return null();
=======
    if (!checkExportedNameForClause(exportName)) {
      return null();
>>>>>>> upstream-releases
    }

<<<<<<< HEAD
    if (!checkExportedNameForClause(exportName)) {
      return null();
    }

    BinaryNodeType exportSpec = handler.newExportSpec(bindingName, exportName);
    if (!exportSpec) {
      return null();
||||||| merged common ancestors
    if (!processExport(node)) {
        return null();
=======
    BinaryNodeType exportSpec = handler_.newExportSpec(bindingName, exportName);
    if (!exportSpec) {
      return null();
>>>>>>> upstream-releases
    }

<<<<<<< HEAD
    handler.addList(kid, exportSpec);

    TokenKind next;
    if (!tokenStream.getToken(&next)) {
      return null();
    }

    if (next == TokenKind::RightCurly) {
      break;
    }

    if (next != TokenKind::Comma) {
      error(JSMSG_RC_AFTER_EXPORT_SPEC_LIST);
      return null();
    }
  }

  // Careful!  If |from| follows, even on a new line, it must start a
  // FromClause:
  //
  //   export { x }
  //   from "foo"; // a single ExportDeclaration
  //
  // But if it doesn't, we might have an ASI opportunity in Operand context:
  //
  //   export { x }   // ExportDeclaration, terminated by ASI
  //   fro\u006D      // ExpressionStatement, the name "from"
  //
  // In that case let matchOrInsertSemicolon sort out ASI or any necessary
  // error.
  bool matched;
  if (!tokenStream.matchToken(&matched, TokenKind::From,
                              TokenStream::Operand)) {
    return null();
  }

  if (matched) {
    return exportFrom(begin, kid);
  }

  if (!matchOrInsertSemicolon()) {
    return null();
  }

  if (!checkLocalExportNames(kid)) {
    return null();
  }

  UnaryNodeType node =
      handler.newExportDeclaration(kid, TokenPos(begin, pos().end));
  if (!node) {
    return null();
  }

  if (!processExport(node)) {
    return null();
  }

  return node;
||||||| merged common ancestors
    return node;
=======
    handler_.addList(kid, exportSpec);

    TokenKind next;
    if (!tokenStream.getToken(&next)) {
      return null();
    }

    if (next == TokenKind::RightCurly) {
      break;
    }

    if (next != TokenKind::Comma) {
      error(JSMSG_RC_AFTER_EXPORT_SPEC_LIST);
      return null();
    }
  }

  // Careful!  If |from| follows, even on a new line, it must start a
  // FromClause:
  //
  //   export { x }
  //   from "foo"; // a single ExportDeclaration
  //
  // But if it doesn't, we might have an ASI opportunity in SlashIsRegExp
  // context:
  //
  //   export { x }   // ExportDeclaration, terminated by ASI
  //   fro\u006D      // ExpressionStatement, the name "from"
  //
  // In that case let matchOrInsertSemicolon sort out ASI or any necessary
  // error.
  bool matched;
  if (!tokenStream.matchToken(&matched, TokenKind::From,
                              TokenStream::SlashIsRegExp)) {
    return null();
  }

  if (matched) {
    return exportFrom(begin, kid);
  }

  if (!matchOrInsertSemicolon()) {
    return null();
  }

  if (!checkLocalExportNames(kid)) {
    return null();
  }

  UnaryNodeType node =
      handler_.newExportDeclaration(kid, TokenPos(begin, pos().end));
  if (!node) {
    return null();
  }

  if (!processExport(node)) {
    return null();
  }

  return node;
>>>>>>> upstream-releases
}

template <class ParseHandler, typename Unit>
typename ParseHandler::UnaryNodeType
GeneralParser<ParseHandler, Unit>::exportVariableStatement(uint32_t begin) {
  if (!abortIfSyntaxParser()) {
    return null();
  }

  MOZ_ASSERT(anyChars.isCurrentTokenType(TokenKind::Var));

<<<<<<< HEAD
  ListNodeType kid = declarationList(YieldIsName, ParseNodeKind::Var);
  if (!kid) {
    return null();
  }
  if (!matchOrInsertSemicolon()) {
    return null();
  }
  if (!checkExportedNamesForDeclarationList(kid)) {
    return null();
  }
||||||| merged common ancestors
    ListNodeType kid = declarationList(YieldIsName, ParseNodeKind::Var);
    if (!kid) {
        return null();
    }
    if (!matchOrInsertSemicolon()) {
        return null();
    }
    if (!checkExportedNamesForDeclarationList(kid)) {
        return null();
    }
=======
  ListNodeType kid = declarationList(YieldIsName, ParseNodeKind::VarStmt);
  if (!kid) {
    return null();
  }
  if (!matchOrInsertSemicolon()) {
    return null();
  }
  if (!checkExportedNamesForDeclarationList(kid)) {
    return null();
  }
>>>>>>> upstream-releases

<<<<<<< HEAD
  UnaryNodeType node =
      handler.newExportDeclaration(kid, TokenPos(begin, pos().end));
  if (!node) {
    return null();
  }
||||||| merged common ancestors
    UnaryNodeType node = handler.newExportDeclaration(kid, TokenPos(begin, pos().end));
    if (!node) {
        return null();
    }
=======
  UnaryNodeType node =
      handler_.newExportDeclaration(kid, TokenPos(begin, pos().end));
  if (!node) {
    return null();
  }
>>>>>>> upstream-releases

  if (!processExport(node)) {
    return null();
  }

  return node;
}

template <class ParseHandler, typename Unit>
typename ParseHandler::UnaryNodeType
GeneralParser<ParseHandler, Unit>::exportFunctionDeclaration(
    uint32_t begin, uint32_t toStringStart,
    FunctionAsyncKind asyncKind /* = SyncFunction */) {
  if (!abortIfSyntaxParser()) {
    return null();
  }

  MOZ_ASSERT(anyChars.isCurrentTokenType(TokenKind::Function));

  Node kid = functionStmt(toStringStart, YieldIsName, NameRequired, asyncKind);
  if (!kid) {
    return null();
  }

<<<<<<< HEAD
  if (!checkExportedNameForFunction(handler.asCode(kid))) {
    return null();
  }
||||||| merged common ancestors
    if (!checkExportedNameForFunction(handler.asCode(kid))) {
        return null();
    }
=======
  if (!checkExportedNameForFunction(handler_.asFunction(kid))) {
    return null();
  }
>>>>>>> upstream-releases

<<<<<<< HEAD
  UnaryNodeType node =
      handler.newExportDeclaration(kid, TokenPos(begin, pos().end));
  if (!node) {
    return null();
  }
||||||| merged common ancestors
    UnaryNodeType node = handler.newExportDeclaration(kid, TokenPos(begin, pos().end));
    if (!node) {
        return null();
    }
=======
  UnaryNodeType node =
      handler_.newExportDeclaration(kid, TokenPos(begin, pos().end));
  if (!node) {
    return null();
  }
>>>>>>> upstream-releases

  if (!processExport(node)) {
    return null();
  }

  return node;
}

template <class ParseHandler, typename Unit>
typename ParseHandler::UnaryNodeType
GeneralParser<ParseHandler, Unit>::exportClassDeclaration(uint32_t begin) {
  if (!abortIfSyntaxParser()) {
    return null();
  }

  MOZ_ASSERT(anyChars.isCurrentTokenType(TokenKind::Class));

  ClassNodeType kid =
      classDefinition(YieldIsName, ClassStatement, NameRequired);
  if (!kid) {
    return null();
  }

  if (!checkExportedNameForClass(kid)) {
    return null();
  }

<<<<<<< HEAD
  UnaryNodeType node =
      handler.newExportDeclaration(kid, TokenPos(begin, pos().end));
  if (!node) {
    return null();
  }
||||||| merged common ancestors
    UnaryNodeType node = handler.newExportDeclaration(kid, TokenPos(begin, pos().end));
    if (!node) {
        return null();
    }
=======
  UnaryNodeType node =
      handler_.newExportDeclaration(kid, TokenPos(begin, pos().end));
  if (!node) {
    return null();
  }
>>>>>>> upstream-releases

  if (!processExport(node)) {
    return null();
  }

  return node;
}

template <class ParseHandler, typename Unit>
typename ParseHandler::UnaryNodeType
GeneralParser<ParseHandler, Unit>::exportLexicalDeclaration(
    uint32_t begin, DeclarationKind kind) {
  if (!abortIfSyntaxParser()) {
    return null();
  }

  MOZ_ASSERT(kind == DeclarationKind::Const || kind == DeclarationKind::Let);
  MOZ_ASSERT_IF(kind == DeclarationKind::Const,
                anyChars.isCurrentTokenType(TokenKind::Const));
  MOZ_ASSERT_IF(kind == DeclarationKind::Let,
                anyChars.isCurrentTokenType(TokenKind::Let));

  ListNodeType kid = lexicalDeclaration(YieldIsName, kind);
  if (!kid) {
    return null();
  }
  if (!checkExportedNamesForDeclarationList(kid)) {
    return null();
  }

<<<<<<< HEAD
  UnaryNodeType node =
      handler.newExportDeclaration(kid, TokenPos(begin, pos().end));
  if (!node) {
    return null();
  }
||||||| merged common ancestors
    UnaryNodeType node = handler.newExportDeclaration(kid, TokenPos(begin, pos().end));
    if (!node) {
        return null();
    }
=======
  UnaryNodeType node =
      handler_.newExportDeclaration(kid, TokenPos(begin, pos().end));
  if (!node) {
    return null();
  }
>>>>>>> upstream-releases

  if (!processExport(node)) {
    return null();
  }

  return node;
}

template <class ParseHandler, typename Unit>
typename ParseHandler::BinaryNodeType
GeneralParser<ParseHandler, Unit>::exportDefaultFunctionDeclaration(
    uint32_t begin, uint32_t toStringStart,
    FunctionAsyncKind asyncKind /* = SyncFunction */) {
  if (!abortIfSyntaxParser()) {
    return null();
  }

  MOZ_ASSERT(anyChars.isCurrentTokenType(TokenKind::Function));

  Node kid =
      functionStmt(toStringStart, YieldIsName, AllowDefaultName, asyncKind);
  if (!kid) {
    return null();
  }

<<<<<<< HEAD
  BinaryNodeType node = handler.newExportDefaultDeclaration(
      kid, null(), TokenPos(begin, pos().end));
  if (!node) {
    return null();
  }
||||||| merged common ancestors
    BinaryNodeType node = handler.newExportDefaultDeclaration(kid, null(),
                                                              TokenPos(begin, pos().end));
    if (!node) {
        return null();
    }
=======
  BinaryNodeType node = handler_.newExportDefaultDeclaration(
      kid, null(), TokenPos(begin, pos().end));
  if (!node) {
    return null();
  }
>>>>>>> upstream-releases

  if (!processExport(node)) {
    return null();
  }

  return node;
}

template <class ParseHandler, typename Unit>
typename ParseHandler::BinaryNodeType
GeneralParser<ParseHandler, Unit>::exportDefaultClassDeclaration(
    uint32_t begin) {
  if (!abortIfSyntaxParser()) {
    return null();
  }

  MOZ_ASSERT(anyChars.isCurrentTokenType(TokenKind::Class));

  ClassNodeType kid =
      classDefinition(YieldIsName, ClassStatement, AllowDefaultName);
  if (!kid) {
    return null();
  }

<<<<<<< HEAD
  BinaryNodeType node = handler.newExportDefaultDeclaration(
      kid, null(), TokenPos(begin, pos().end));
  if (!node) {
    return null();
  }
||||||| merged common ancestors
    BinaryNodeType node = handler.newExportDefaultDeclaration(kid, null(),
                                                              TokenPos(begin, pos().end));
    if (!node) {
        return null();
    }
=======
  BinaryNodeType node = handler_.newExportDefaultDeclaration(
      kid, null(), TokenPos(begin, pos().end));
  if (!node) {
    return null();
  }
>>>>>>> upstream-releases

  if (!processExport(node)) {
    return null();
  }

  return node;
}

template <class ParseHandler, typename Unit>
typename ParseHandler::BinaryNodeType
GeneralParser<ParseHandler, Unit>::exportDefaultAssignExpr(uint32_t begin) {
  if (!abortIfSyntaxParser()) {
    return null();
  }

<<<<<<< HEAD
  HandlePropertyName name = context->names().default_;
  NameNodeType nameNode = newName(name);
  if (!nameNode) {
    return null();
  }
  if (!noteDeclaredName(name, DeclarationKind::Const, pos())) {
    return null();
  }
||||||| merged common ancestors
    HandlePropertyName name = context->names().default_;
    NameNodeType nameNode = newName(name);
    if (!nameNode) {
        return null();
    }
    if (!noteDeclaredName(name, DeclarationKind::Const, pos())) {
        return null();
    }
=======
  HandlePropertyName name = cx_->names().default_;
  NameNodeType nameNode = newName(name);
  if (!nameNode) {
    return null();
  }
  if (!noteDeclaredName(name, DeclarationKind::Const, pos())) {
    return null();
  }
>>>>>>> upstream-releases

  Node kid = assignExpr(InAllowed, YieldIsName, TripledotProhibited);
  if (!kid) {
    return null();
  }

  if (!matchOrInsertSemicolon()) {
    return null();
  }

<<<<<<< HEAD
  BinaryNodeType node = handler.newExportDefaultDeclaration(
      kid, nameNode, TokenPos(begin, pos().end));
  if (!node) {
    return null();
  }
||||||| merged common ancestors
    BinaryNodeType node = handler.newExportDefaultDeclaration(kid, nameNode,
                                                              TokenPos(begin, pos().end));
    if (!node) {
        return null();
    }
=======
  BinaryNodeType node = handler_.newExportDefaultDeclaration(
      kid, nameNode, TokenPos(begin, pos().end));
  if (!node) {
    return null();
  }
>>>>>>> upstream-releases

  if (!processExport(node)) {
    return null();
  }

  return node;
}

template <class ParseHandler, typename Unit>
typename ParseHandler::BinaryNodeType
GeneralParser<ParseHandler, Unit>::exportDefault(uint32_t begin) {
  if (!abortIfSyntaxParser()) {
    return null();
  }

  MOZ_ASSERT(anyChars.isCurrentTokenType(TokenKind::Default));

<<<<<<< HEAD
  TokenKind tt;
  if (!tokenStream.getToken(&tt, TokenStream::Operand)) {
    return null();
  }
||||||| merged common ancestors
    switch (tt) {
      case TokenKind::Function:
        return exportDefaultFunctionDeclaration(begin, pos().begin);
=======
  TokenKind tt;
  if (!tokenStream.getToken(&tt, TokenStream::SlashIsRegExp)) {
    return null();
  }
>>>>>>> upstream-releases

<<<<<<< HEAD
  if (!checkExportedName(context->names().default_)) {
    return null();
  }
||||||| merged common ancestors
      case TokenKind::Async: {
        TokenKind nextSameLine = TokenKind::Eof;
        if (!tokenStream.peekTokenSameLine(&nextSameLine)) {
            return null();
        }
=======
  if (!checkExportedName(cx_->names().default_)) {
    return null();
  }
>>>>>>> upstream-releases

  switch (tt) {
    case TokenKind::Function:
      return exportDefaultFunctionDeclaration(begin, pos().begin);

    case TokenKind::Async: {
      TokenKind nextSameLine = TokenKind::Eof;
      if (!tokenStream.peekTokenSameLine(&nextSameLine)) {
        return null();
      }

      if (nextSameLine == TokenKind::Function) {
        uint32_t toStringStart = pos().begin;
        tokenStream.consumeKnownToken(TokenKind::Function);
        return exportDefaultFunctionDeclaration(
            begin, toStringStart, FunctionAsyncKind::AsyncFunction);
      }

      anyChars.ungetToken();
      return exportDefaultAssignExpr(begin);
    }

    case TokenKind::Class:
      return exportDefaultClassDeclaration(begin);

    default:
      anyChars.ungetToken();
      return exportDefaultAssignExpr(begin);
  }
}

template <class ParseHandler, typename Unit>
typename ParseHandler::Node
GeneralParser<ParseHandler, Unit>::exportDeclaration() {
  if (!abortIfSyntaxParser()) {
    return null();
  }

  MOZ_ASSERT(anyChars.isCurrentTokenType(TokenKind::Export));

<<<<<<< HEAD
  if (!pc->atModuleLevel()) {
    error(JSMSG_EXPORT_DECL_AT_TOP_LEVEL);
    return null();
  }
||||||| merged common ancestors
    TokenKind tt;
    if (!tokenStream.getToken(&tt)) {
        return null();
    }
    switch (tt) {
      case TokenKind::Mul:
        return exportBatch(begin);
=======
  if (!pc_->atModuleLevel()) {
    error(JSMSG_EXPORT_DECL_AT_TOP_LEVEL);
    return null();
  }
>>>>>>> upstream-releases

  uint32_t begin = pos().begin;

  TokenKind tt;
  if (!tokenStream.getToken(&tt)) {
    return null();
  }
  switch (tt) {
    case TokenKind::Mul:
      return exportBatch(begin);

    case TokenKind::LeftCurly:
      return exportClause(begin);

    case TokenKind::Var:
      return exportVariableStatement(begin);

    case TokenKind::Function:
      return exportFunctionDeclaration(begin, pos().begin);

    case TokenKind::Async: {
      TokenKind nextSameLine = TokenKind::Eof;
      if (!tokenStream.peekTokenSameLine(&nextSameLine)) {
        return null();
      }

      if (nextSameLine == TokenKind::Function) {
        uint32_t toStringStart = pos().begin;
        tokenStream.consumeKnownToken(TokenKind::Function);
        return exportFunctionDeclaration(begin, toStringStart,
                                         FunctionAsyncKind::AsyncFunction);
      }

      error(JSMSG_DECLARATION_AFTER_EXPORT);
      return null();
    }

<<<<<<< HEAD
    case TokenKind::Class:
      return exportClassDeclaration(begin);

    case TokenKind::Const:
      return exportLexicalDeclaration(begin, DeclarationKind::Const);
||||||| merged common ancestors
    // Annex B.3.4 says that unbraced FunctionDeclarations under if/else in
    // non-strict code act as if they were braced: |if (x) function f() {}|
    // parses as |if (x) { function f() {} }|.
    //
    // Careful!  FunctionDeclaration doesn't include generators or async
    // functions.
    if (next == TokenKind::Function) {
        tokenStream.consumeKnownToken(next, TokenStream::Operand);

        // Parser::statement would handle this, but as this function handles
        // every other error case, it seems best to handle this.
        if (pc->sc()->strict()) {
            error(JSMSG_FORBIDDEN_AS_STATEMENT, "function declarations");
            return null();
        }

        TokenKind maybeStar;
        if (!tokenStream.peekToken(&maybeStar)) {
            return null();
        }

        if (maybeStar == TokenKind::Mul) {
            error(JSMSG_FORBIDDEN_AS_STATEMENT, "generator declarations");
            return null();
        }

        ParseContext::Statement stmt(pc, StatementKind::Block);
        ParseContext::Scope scope(this);
        if (!scope.init(pc)) {
            return null();
        }

        TokenPos funcPos = pos();
        Node fun = functionStmt(pos().begin, yieldHandling, NameRequired);
        if (!fun) {
            return null();
        }
=======
    case TokenKind::Class:
      return exportClassDeclaration(begin);
>>>>>>> upstream-releases

<<<<<<< HEAD
    case TokenKind::Let:
      return exportLexicalDeclaration(begin, DeclarationKind::Let);
||||||| merged common ancestors
        ListNodeType block = handler.newStatementList(funcPos);
        if (!block) {
            return null();
        }
=======
    case TokenKind::Const:
      return exportLexicalDeclaration(begin, DeclarationKind::Const);
>>>>>>> upstream-releases

<<<<<<< HEAD
    case TokenKind::Default:
      return exportDefault(begin);
||||||| merged common ancestors
        handler.addStatementToList(block, fun);
        return finishLexicalScope(scope, block);
    }
=======
    case TokenKind::Let:
      return exportLexicalDeclaration(begin, DeclarationKind::Let);

    case TokenKind::Default:
      return exportDefault(begin);
>>>>>>> upstream-releases

    default:
      error(JSMSG_DECLARATION_AFTER_EXPORT);
      return null();
  }
}

template <class ParseHandler, typename Unit>
<<<<<<< HEAD
typename ParseHandler::UnaryNodeType
GeneralParser<ParseHandler, Unit>::expressionStatement(
    YieldHandling yieldHandling, InvokedPrediction invoked) {
  anyChars.ungetToken();
  Node pnexpr = expr(InAllowed, yieldHandling, TripledotProhibited,
                     /* possibleError = */ nullptr, invoked);
  if (!pnexpr) {
    return null();
  }
  if (!matchOrInsertSemicolon()) {
    return null();
  }
  return handler.newExprStatement(pnexpr, pos().end);
}
||||||| merged common ancestors
typename ParseHandler::TernaryNodeType
GeneralParser<ParseHandler, Unit>::ifStatement(YieldHandling yieldHandling)
{
    Vector<Node, 4> condList(context), thenList(context);
    Vector<uint32_t, 4> posList(context);
    Node elseBranch;

    ParseContext::Statement stmt(pc, StatementKind::If);

    while (true) {
        uint32_t begin = pos().begin;
=======
typename ParseHandler::UnaryNodeType
GeneralParser<ParseHandler, Unit>::expressionStatement(
    YieldHandling yieldHandling, InvokedPrediction invoked) {
  anyChars.ungetToken();
  Node pnexpr = expr(InAllowed, yieldHandling, TripledotProhibited,
                     /* possibleError = */ nullptr, invoked);
  if (!pnexpr) {
    return null();
  }
  if (!matchOrInsertSemicolon()) {
    return null();
  }
  return handler_.newExprStatement(pnexpr, pos().end);
}

template <class ParseHandler, typename Unit>
typename ParseHandler::Node
GeneralParser<ParseHandler, Unit>::consequentOrAlternative(
    YieldHandling yieldHandling) {
  TokenKind next;
  if (!tokenStream.peekToken(&next, TokenStream::SlashIsRegExp)) {
    return null();
  }

  // Annex B.3.4 says that unbraced FunctionDeclarations under if/else in
  // non-strict code act as if they were braced: |if (x) function f() {}|
  // parses as |if (x) { function f() {} }|.
  //
  // Careful!  FunctionDeclaration doesn't include generators or async
  // functions.
  if (next == TokenKind::Function) {
    tokenStream.consumeKnownToken(next, TokenStream::SlashIsRegExp);
>>>>>>> upstream-releases

<<<<<<< HEAD
template <class ParseHandler, typename Unit>
typename ParseHandler::Node
GeneralParser<ParseHandler, Unit>::consequentOrAlternative(
    YieldHandling yieldHandling) {
  TokenKind next;
  if (!tokenStream.peekToken(&next, TokenStream::Operand)) {
    return null();
  }

  // Annex B.3.4 says that unbraced FunctionDeclarations under if/else in
  // non-strict code act as if they were braced: |if (x) function f() {}|
  // parses as |if (x) { function f() {} }|.
  //
  // Careful!  FunctionDeclaration doesn't include generators or async
  // functions.
  if (next == TokenKind::Function) {
    tokenStream.consumeKnownToken(next, TokenStream::Operand);

    // Parser::statement would handle this, but as this function handles
    // every other error case, it seems best to handle this.
    if (pc->sc()->strict()) {
      error(JSMSG_FORBIDDEN_AS_STATEMENT, "function declarations");
      return null();
    }
||||||| merged common ancestors
        /* An IF node has three kids: condition, then, and optional else. */
        Node cond = condition(InAllowed, yieldHandling);
        if (!cond) {
            return null();
        }
=======
    // Parser::statement would handle this, but as this function handles
    // every other error case, it seems best to handle this.
    if (pc_->sc()->strict()) {
      error(JSMSG_FORBIDDEN_AS_STATEMENT, "function declarations");
      return null();
    }
>>>>>>> upstream-releases

    TokenKind maybeStar;
    if (!tokenStream.peekToken(&maybeStar)) {
      return null();
    }

    if (maybeStar == TokenKind::Mul) {
      error(JSMSG_FORBIDDEN_AS_STATEMENT, "generator declarations");
      return null();
    }

<<<<<<< HEAD
    ParseContext::Statement stmt(pc, StatementKind::Block);
    ParseContext::Scope scope(this);
    if (!scope.init(pc)) {
      return null();
    }
||||||| merged common ancestors
        if (!condList.append(cond) || !thenList.append(thenBranch) || !posList.append(begin)) {
            return null();
        }
=======
    ParseContext::Statement stmt(pc_, StatementKind::Block);
    ParseContext::Scope scope(this);
    if (!scope.init(pc_)) {
      return null();
    }
>>>>>>> upstream-releases

    TokenPos funcPos = pos();
    Node fun = functionStmt(pos().begin, yieldHandling, NameRequired);
    if (!fun) {
      return null();
    }

<<<<<<< HEAD
    ListNodeType block = handler.newStatementList(funcPos);
    if (!block) {
      return null();
||||||| merged common ancestors
    TernaryNodeType ifNode;
    for (int i = condList.length() - 1; i >= 0; i--) {
        ifNode = handler.newIfStatement(posList[i], condList[i], thenList[i], elseBranch);
        if (!ifNode) {
            return null();
        }
        elseBranch = ifNode;
=======
    ListNodeType block = handler_.newStatementList(funcPos);
    if (!block) {
      return null();
>>>>>>> upstream-releases
    }

<<<<<<< HEAD
    handler.addStatementToList(block, fun);
    return finishLexicalScope(scope, block);
  }

  return statement(yieldHandling);
||||||| merged common ancestors
    return ifNode;
=======
    handler_.addStatementToList(block, fun);
    return finishLexicalScope(scope, block);
  }

  return statement(yieldHandling);
>>>>>>> upstream-releases
}

template <class ParseHandler, typename Unit>
<<<<<<< HEAD
typename ParseHandler::TernaryNodeType
GeneralParser<ParseHandler, Unit>::ifStatement(YieldHandling yieldHandling) {
  Vector<Node, 4> condList(context), thenList(context);
  Vector<uint32_t, 4> posList(context);
  Node elseBranch;

  ParseContext::Statement stmt(pc, StatementKind::If);

  while (true) {
||||||| merged common ancestors
typename ParseHandler::BinaryNodeType
GeneralParser<ParseHandler, Unit>::doWhileStatement(YieldHandling yieldHandling)
{
=======
typename ParseHandler::TernaryNodeType
GeneralParser<ParseHandler, Unit>::ifStatement(YieldHandling yieldHandling) {
  Vector<Node, 4> condList(cx_), thenList(cx_);
  Vector<uint32_t, 4> posList(cx_);
  Node elseBranch;

  ParseContext::Statement stmt(pc_, StatementKind::If);

  while (true) {
>>>>>>> upstream-releases
    uint32_t begin = pos().begin;

    /* An IF node has three kids: condition, then, and optional else. */
    Node cond = condition(InAllowed, yieldHandling);
    if (!cond) {
      return null();
    }

<<<<<<< HEAD
    TokenKind tt;
    if (!tokenStream.peekToken(&tt, TokenStream::Operand)) {
      return null();
    }
    if (tt == TokenKind::Semi) {
      if (!extraWarning(JSMSG_EMPTY_CONSEQUENT)) {
||||||| merged common ancestors
    // The semicolon after do-while is even more optional than most
    // semicolons in JS.  Web compat required this by 2004:
    //   http://bugzilla.mozilla.org/show_bug.cgi?id=238945
    // ES3 and ES5 disagreed, but ES6 conforms to Web reality:
    //   https://bugs.ecmascript.org/show_bug.cgi?id=157
    // To parse |do {} while (true) false| correctly, use Operand.
    bool ignored;
    if (!tokenStream.matchToken(&ignored, TokenKind::Semi, TokenStream::Operand)) {
=======
    TokenKind tt;
    if (!tokenStream.peekToken(&tt, TokenStream::SlashIsRegExp)) {
      return null();
    }
    if (tt == TokenKind::Semi) {
      if (!extraWarning(JSMSG_EMPTY_CONSEQUENT)) {
>>>>>>> upstream-releases
        return null();
      }
    }

    Node thenBranch = consequentOrAlternative(yieldHandling);
    if (!thenBranch) {
      return null();
    }

    if (!condList.append(cond) || !thenList.append(thenBranch) ||
        !posList.append(begin)) {
      return null();
    }

<<<<<<< HEAD
    bool matched;
    if (!tokenStream.matchToken(&matched, TokenKind::Else,
                                TokenStream::Operand)) {
      return null();
    }
    if (matched) {
      if (!tokenStream.matchToken(&matched, TokenKind::If,
                                  TokenStream::Operand)) {
        return null();
      }
      if (matched) {
        continue;
      }
      elseBranch = consequentOrAlternative(yieldHandling);
      if (!elseBranch) {
        return null();
      }
    } else {
      elseBranch = null();
||||||| merged common ancestors
template <class ParseHandler, typename Unit>
bool
GeneralParser<ParseHandler, Unit>::matchInOrOf(bool* isForInp, bool* isForOfp)
{
    TokenKind tt;
    if (!tokenStream.getToken(&tt, TokenStream::Operand)) {
        return false;
=======
    bool matched;
    if (!tokenStream.matchToken(&matched, TokenKind::Else,
                                TokenStream::SlashIsRegExp)) {
      return null();
    }
    if (matched) {
      if (!tokenStream.matchToken(&matched, TokenKind::If,
                                  TokenStream::SlashIsRegExp)) {
        return null();
      }
      if (matched) {
        continue;
      }
      elseBranch = consequentOrAlternative(yieldHandling);
      if (!elseBranch) {
        return null();
      }
    } else {
      elseBranch = null();
>>>>>>> upstream-releases
    }
    break;
  }

<<<<<<< HEAD
  TernaryNodeType ifNode;
  for (int i = condList.length() - 1; i >= 0; i--) {
    ifNode = handler.newIfStatement(posList[i], condList[i], thenList[i],
                                    elseBranch);
    if (!ifNode) {
      return null();
||||||| merged common ancestors
    *isForInp = tt == TokenKind::In;
    *isForOfp = tt == TokenKind::Of;
    if (!*isForInp && !*isForOfp) {
        anyChars.ungetToken();
=======
  TernaryNodeType ifNode;
  for (int i = condList.length() - 1; i >= 0; i--) {
    ifNode = handler_.newIfStatement(posList[i], condList[i], thenList[i],
                                     elseBranch);
    if (!ifNode) {
      return null();
>>>>>>> upstream-releases
    }
    elseBranch = ifNode;
  }

  return ifNode;
}

template <class ParseHandler, typename Unit>
<<<<<<< HEAD
typename ParseHandler::BinaryNodeType
GeneralParser<ParseHandler, Unit>::doWhileStatement(
    YieldHandling yieldHandling) {
  uint32_t begin = pos().begin;
  ParseContext::Statement stmt(pc, StatementKind::DoLoop);
  Node body = statement(yieldHandling);
  if (!body) {
    return null();
  }
  if (!mustMatchToken(TokenKind::While, TokenStream::Operand,
                      JSMSG_WHILE_AFTER_DO)) {
    return null();
  }
  Node cond = condition(InAllowed, yieldHandling);
  if (!cond) {
    return null();
  }

  // The semicolon after do-while is even more optional than most
  // semicolons in JS.  Web compat required this by 2004:
  //   http://bugzilla.mozilla.org/show_bug.cgi?id=238945
  // ES3 and ES5 disagreed, but ES6 conforms to Web reality:
  //   https://bugs.ecmascript.org/show_bug.cgi?id=157
  // To parse |do {} while (true) false| correctly, use Operand.
  bool ignored;
  if (!tokenStream.matchToken(&ignored, TokenKind::Semi,
                              TokenStream::Operand)) {
    return null();
  }
  return handler.newDoWhileStatement(body, cond, TokenPos(begin, pos().end));
}
||||||| merged common ancestors
bool
GeneralParser<ParseHandler, Unit>::forHeadStart(YieldHandling yieldHandling,
                                                ParseNodeKind* forHeadKind, Node* forInitialPart,
                                                Maybe<ParseContext::Scope>& forLoopLexicalScope,
                                                Node* forInOrOfExpression)
{
    MOZ_ASSERT(anyChars.isCurrentTokenType(TokenKind::LeftParen));
=======
typename ParseHandler::BinaryNodeType
GeneralParser<ParseHandler, Unit>::doWhileStatement(
    YieldHandling yieldHandling) {
  uint32_t begin = pos().begin;
  ParseContext::Statement stmt(pc_, StatementKind::DoLoop);
  Node body = statement(yieldHandling);
  if (!body) {
    return null();
  }
  if (!mustMatchToken(TokenKind::While, JSMSG_WHILE_AFTER_DO)) {
    return null();
  }
  Node cond = condition(InAllowed, yieldHandling);
  if (!cond) {
    return null();
  }

  // The semicolon after do-while is even more optional than most
  // semicolons in JS.  Web compat required this by 2004:
  //   http://bugzilla.mozilla.org/show_bug.cgi?id=238945
  // ES3 and ES5 disagreed, but ES6 conforms to Web reality:
  //   https://bugs.ecmascript.org/show_bug.cgi?id=157
  // To parse |do {} while (true) false| correctly, use SlashIsRegExp.
  bool ignored;
  if (!tokenStream.matchToken(&ignored, TokenKind::Semi,
                              TokenStream::SlashIsRegExp)) {
    return null();
  }
  return handler_.newDoWhileStatement(body, cond, TokenPos(begin, pos().end));
}
>>>>>>> upstream-releases

<<<<<<< HEAD
template <class ParseHandler, typename Unit>
typename ParseHandler::BinaryNodeType
GeneralParser<ParseHandler, Unit>::whileStatement(YieldHandling yieldHandling) {
  uint32_t begin = pos().begin;
  ParseContext::Statement stmt(pc, StatementKind::WhileLoop);
  Node cond = condition(InAllowed, yieldHandling);
  if (!cond) {
    return null();
  }
  Node body = statement(yieldHandling);
  if (!body) {
    return null();
  }
  return handler.newWhileStatement(begin, cond, body);
}
||||||| merged common ancestors
    TokenKind tt;
    if (!tokenStream.peekToken(&tt, TokenStream::Operand)) {
        return false;
    }
=======
template <class ParseHandler, typename Unit>
typename ParseHandler::BinaryNodeType
GeneralParser<ParseHandler, Unit>::whileStatement(YieldHandling yieldHandling) {
  uint32_t begin = pos().begin;
  ParseContext::Statement stmt(pc_, StatementKind::WhileLoop);
  Node cond = condition(InAllowed, yieldHandling);
  if (!cond) {
    return null();
  }
  Node body = statement(yieldHandling);
  if (!body) {
    return null();
  }
  return handler_.newWhileStatement(begin, cond, body);
}
>>>>>>> upstream-releases

<<<<<<< HEAD
template <class ParseHandler, typename Unit>
bool GeneralParser<ParseHandler, Unit>::matchInOrOf(bool* isForInp,
                                                    bool* isForOfp) {
  TokenKind tt;
  if (!tokenStream.getToken(&tt, TokenStream::Operand)) {
    return false;
  }
||||||| merged common ancestors
    // Super-duper easy case: |for (;| is a C-style for-loop with no init
    // component.
    if (tt == TokenKind::Semi) {
        *forInitialPart = null();
        *forHeadKind = ParseNodeKind::ForHead;
        return true;
    }
=======
template <class ParseHandler, typename Unit>
bool GeneralParser<ParseHandler, Unit>::matchInOrOf(bool* isForInp,
                                                    bool* isForOfp) {
  TokenKind tt;
  if (!tokenStream.getToken(&tt, TokenStream::SlashIsRegExp)) {
    return false;
  }
>>>>>>> upstream-releases

  *isForInp = tt == TokenKind::In;
  *isForOfp = tt == TokenKind::Of;
  if (!*isForInp && !*isForOfp) {
    anyChars.ungetToken();
  }

  MOZ_ASSERT_IF(*isForInp || *isForOfp, *isForInp != *isForOfp);
  return true;
}

template <class ParseHandler, typename Unit>
bool GeneralParser<ParseHandler, Unit>::forHeadStart(
    YieldHandling yieldHandling, ParseNodeKind* forHeadKind,
    Node* forInitialPart, Maybe<ParseContext::Scope>& forLoopLexicalScope,
    Node* forInOrOfExpression) {
  MOZ_ASSERT(anyChars.isCurrentTokenType(TokenKind::LeftParen));

<<<<<<< HEAD
  TokenKind tt;
  if (!tokenStream.peekToken(&tt, TokenStream::Operand)) {
    return false;
  }
||||||| merged common ancestors
        // Push a temporary ForLoopLexicalHead Statement that allows for
        // lexical declarations, as they are usually allowed only in braced
        // statements.
        ParseContext::Statement forHeadStmt(pc, StatementKind::ForLoopLexicalHead);
=======
  TokenKind tt;
  if (!tokenStream.peekToken(&tt, TokenStream::SlashIsRegExp)) {
    return false;
  }
>>>>>>> upstream-releases

<<<<<<< HEAD
  // Super-duper easy case: |for (;| is a C-style for-loop with no init
  // component.
  if (tt == TokenKind::Semi) {
    *forInitialPart = null();
    *forHeadKind = ParseNodeKind::ForHead;
    return true;
  }

  // Parsing after |for (var| is also relatively simple (from this method's
  // point of view).  No block-related work complicates matters, so delegate
  // to Parser::declaration.
  if (tt == TokenKind::Var) {
    tokenStream.consumeKnownToken(tt, TokenStream::Operand);

    // Pass null for block object because |var| declarations don't use one.
    *forInitialPart = declarationList(yieldHandling, ParseNodeKind::Var,
                                      forHeadKind, forInOrOfExpression);
    return *forInitialPart != null();
  }

  // Otherwise we have a lexical declaration or an expression.

  // For-in loop backwards compatibility requires that |let| starting a
  // for-loop that's not a (new to ES6) for-of loop, in non-strict mode code,
  // parse as an identifier.  (|let| in for-of is always a declaration.)
  bool parsingLexicalDeclaration = false;
  bool letIsIdentifier = false;
  if (tt == TokenKind::Const) {
    parsingLexicalDeclaration = true;
    tokenStream.consumeKnownToken(tt, TokenStream::Operand);
  } else if (tt == TokenKind::Let) {
    // We could have a {For,Lexical}Declaration, or we could have a
    // LeftHandSideExpression with lookahead restrictions so it's not
    // ambiguous with the former.  Check for a continuation of the former
    // to decide which we have.
    tokenStream.consumeKnownToken(TokenKind::Let, TokenStream::Operand);
||||||| merged common ancestors
        *forInitialPart = declarationList(yieldHandling,
                                          tt == TokenKind::Const
                                          ? ParseNodeKind::Const
                                          : ParseNodeKind::Let,
                                          forHeadKind, forInOrOfExpression);
        return *forInitialPart != null();
    }
=======
  // Super-duper easy case: |for (;| is a C-style for-loop with no init
  // component.
  if (tt == TokenKind::Semi) {
    *forInitialPart = null();
    *forHeadKind = ParseNodeKind::ForHead;
    return true;
  }

  // Parsing after |for (var| is also relatively simple (from this method's
  // point of view).  No block-related work complicates matters, so delegate
  // to Parser::declaration.
  if (tt == TokenKind::Var) {
    tokenStream.consumeKnownToken(tt, TokenStream::SlashIsRegExp);

    // Pass null for block object because |var| declarations don't use one.
    *forInitialPart = declarationList(yieldHandling, ParseNodeKind::VarStmt,
                                      forHeadKind, forInOrOfExpression);
    return *forInitialPart != null();
  }

  // Otherwise we have a lexical declaration or an expression.

  // For-in loop backwards compatibility requires that |let| starting a
  // for-loop that's not a (new to ES6) for-of loop, in non-strict mode code,
  // parse as an identifier.  (|let| in for-of is always a declaration.)
  bool parsingLexicalDeclaration = false;
  bool letIsIdentifier = false;
  if (tt == TokenKind::Const) {
    parsingLexicalDeclaration = true;
    tokenStream.consumeKnownToken(tt, TokenStream::SlashIsRegExp);
  } else if (tt == TokenKind::Let) {
    // We could have a {For,Lexical}Declaration, or we could have a
    // LeftHandSideExpression with lookahead restrictions so it's not
    // ambiguous with the former.  Check for a continuation of the former
    // to decide which we have.
    tokenStream.consumeKnownToken(TokenKind::Let, TokenStream::SlashIsRegExp);
>>>>>>> upstream-releases

    TokenKind next;
    if (!tokenStream.peekToken(&next)) {
      return false;
    }

    parsingLexicalDeclaration = nextTokenContinuesLetDeclaration(next);
    if (!parsingLexicalDeclaration) {
      anyChars.ungetToken();
      letIsIdentifier = true;
    }
  }

<<<<<<< HEAD
  if (parsingLexicalDeclaration) {
    forLoopLexicalScope.emplace(this);
    if (!forLoopLexicalScope->init(pc)) {
      return false;
||||||| merged common ancestors
    bool isForIn, isForOf;
    if (!matchInOrOf(&isForIn, &isForOf)) {
        return false;
=======
  if (parsingLexicalDeclaration) {
    forLoopLexicalScope.emplace(this);
    if (!forLoopLexicalScope->init(pc_)) {
      return false;
>>>>>>> upstream-releases
    }

<<<<<<< HEAD
    // Push a temporary ForLoopLexicalHead Statement that allows for
    // lexical declarations, as they are usually allowed only in braced
    // statements.
    ParseContext::Statement forHeadStmt(pc, StatementKind::ForLoopLexicalHead);
||||||| merged common ancestors
    // If we don't encounter 'in'/'of', we have a for(;;) loop.  We've handled
    // the init expression; the caller handles the rest.
    if (!isForIn && !isForOf) {
        if (!possibleError.checkForExpressionError()) {
            return false;
        }
=======
    // Push a temporary ForLoopLexicalHead Statement that allows for
    // lexical declarations, as they are usually allowed only in braced
    // statements.
    ParseContext::Statement forHeadStmt(pc_, StatementKind::ForLoopLexicalHead);
>>>>>>> upstream-releases

<<<<<<< HEAD
    *forInitialPart = declarationList(
        yieldHandling,
        tt == TokenKind::Const ? ParseNodeKind::Const : ParseNodeKind::Let,
        forHeadKind, forInOrOfExpression);
    return *forInitialPart != null();
  }
||||||| merged common ancestors
        *forHeadKind = ParseNodeKind::ForHead;
        return true;
    }
=======
    *forInitialPart =
        declarationList(yieldHandling,
                        tt == TokenKind::Const ? ParseNodeKind::ConstDecl
                                               : ParseNodeKind::LetDecl,
                        forHeadKind, forInOrOfExpression);
    return *forInitialPart != null();
  }
>>>>>>> upstream-releases

<<<<<<< HEAD
  uint32_t exprOffset;
  if (!tokenStream.peekOffset(&exprOffset, TokenStream::Operand)) {
    return false;
  }

  // Finally, handle for-loops that start with expressions.  Pass
  // |InProhibited| so that |in| isn't parsed in a RelationalExpression as a
  // binary operator.  |in| makes it a for-in loop, *not* an |in| expression.
  PossibleError possibleError(*this);
  *forInitialPart =
      expr(InProhibited, yieldHandling, TripledotProhibited, &possibleError);
  if (!*forInitialPart) {
    return false;
  }
||||||| merged common ancestors
    MOZ_ASSERT(isForIn != isForOf);
=======
  uint32_t exprOffset;
  if (!tokenStream.peekOffset(&exprOffset, TokenStream::SlashIsRegExp)) {
    return false;
  }

  // Finally, handle for-loops that start with expressions.  Pass
  // |InProhibited| so that |in| isn't parsed in a RelationalExpression as a
  // binary operator.  |in| makes it a for-in loop, *not* an |in| expression.
  PossibleError possibleError(*this);
  *forInitialPart =
      expr(InProhibited, yieldHandling, TripledotProhibited, &possibleError);
  if (!*forInitialPart) {
    return false;
  }
>>>>>>> upstream-releases

  bool isForIn, isForOf;
  if (!matchInOrOf(&isForIn, &isForOf)) {
    return false;
  }

  // If we don't encounter 'in'/'of', we have a for(;;) loop.  We've handled
  // the init expression; the caller handles the rest.
  if (!isForIn && !isForOf) {
    if (!possibleError.checkForExpressionError()) {
      return false;
    }

    *forHeadKind = ParseNodeKind::ForHead;
    return true;
  }

  MOZ_ASSERT(isForIn != isForOf);

  // In a for-of loop, 'let' that starts the loop head is a |let| keyword,
  // per the [lookahead ≠ let] restriction on the LeftHandSideExpression
  // variant of such loops.  Expressions that start with |let| can't be used
  // here.
  //
  //   var let = {};
  //   for (let.prop of [1]) // BAD
  //     break;
  //
  // See ES6 13.7.
  if (isForOf && letIsIdentifier) {
    errorAt(exprOffset, JSMSG_LET_STARTING_FOROF_LHS);
    return false;
  }

  *forHeadKind = isForIn ? ParseNodeKind::ForIn : ParseNodeKind::ForOf;

<<<<<<< HEAD
  // Verify the left-hand side expression doesn't have a forbidden form.
  if (handler.isUnparenthesizedDestructuringPattern(*forInitialPart)) {
    if (!possibleError.checkForDestructuringErrorOrWarning()) {
      return false;
    }
  } else if (handler.isName(*forInitialPart)) {
    if (const char* chars = nameIsArgumentsOrEval(*forInitialPart)) {
      // |chars| is "arguments" or "eval" here.
      if (!strictModeErrorAt(exprOffset, JSMSG_BAD_STRICT_ASSIGN, chars)) {
        return false;
      }
||||||| merged common ancestors
        handler.adjustGetToSet(*forInitialPart);
    } else if (handler.isPropertyAccess(*forInitialPart)) {
        // Permitted: no additional testing/fixup needed.
    } else if (handler.isFunctionCall(*forInitialPart)) {
        if (!strictModeErrorAt(exprOffset, JSMSG_BAD_FOR_LEFTSIDE)) {
            return false;
        }
    } else {
        errorAt(exprOffset, JSMSG_BAD_FOR_LEFTSIDE);
        return false;
=======
  // Verify the left-hand side expression doesn't have a forbidden form.
  if (handler_.isUnparenthesizedDestructuringPattern(*forInitialPart)) {
    if (!possibleError.checkForDestructuringErrorOrWarning()) {
      return false;
>>>>>>> upstream-releases
    }
<<<<<<< HEAD

    handler.adjustGetToSet(*forInitialPart);
  } else if (handler.isPropertyAccess(*forInitialPart)) {
    // Permitted: no additional testing/fixup needed.
  } else if (handler.isFunctionCall(*forInitialPart)) {
    if (!strictModeErrorAt(exprOffset, JSMSG_BAD_FOR_LEFTSIDE)) {
      return false;
||||||| merged common ancestors

    if (!possibleError.checkForExpressionError()) {
        return false;
=======
  } else if (handler_.isName(*forInitialPart)) {
    if (const char* chars = nameIsArgumentsOrEval(*forInitialPart)) {
      // |chars| is "arguments" or "eval" here.
      if (!strictModeErrorAt(exprOffset, JSMSG_BAD_STRICT_ASSIGN, chars)) {
        return false;
      }
>>>>>>> upstream-releases
    }
<<<<<<< HEAD
  } else {
    errorAt(exprOffset, JSMSG_BAD_FOR_LEFTSIDE);
    return false;
  }

  if (!possibleError.checkForExpressionError()) {
    return false;
  }
||||||| merged common ancestors
=======
  } else if (handler_.isPropertyAccess(*forInitialPart)) {
    // Permitted: no additional testing/fixup needed.
  } else if (handler_.isFunctionCall(*forInitialPart)) {
    if (!strictModeErrorAt(exprOffset, JSMSG_BAD_FOR_LEFTSIDE)) {
      return false;
    }
  } else {
    errorAt(exprOffset, JSMSG_BAD_FOR_LEFTSIDE);
    return false;
  }
>>>>>>> upstream-releases

<<<<<<< HEAD
  // Finally, parse the iterated expression, making the for-loop's closing
  // ')' the next token.
  *forInOrOfExpression = expressionAfterForInOrOf(*forHeadKind, yieldHandling);
  return *forInOrOfExpression != null();
||||||| merged common ancestors
    // Finally, parse the iterated expression, making the for-loop's closing
    // ')' the next token.
    *forInOrOfExpression = expressionAfterForInOrOf(*forHeadKind, yieldHandling);
    return *forInOrOfExpression != null();
=======
  if (!possibleError.checkForExpressionError()) {
    return false;
  }

  // Finally, parse the iterated expression, making the for-loop's closing
  // ')' the next token.
  *forInOrOfExpression = expressionAfterForInOrOf(*forHeadKind, yieldHandling);
  return *forInOrOfExpression != null();
>>>>>>> upstream-releases
}

template <class ParseHandler, typename Unit>
<<<<<<< HEAD
typename ParseHandler::Node GeneralParser<ParseHandler, Unit>::forStatement(
    YieldHandling yieldHandling) {
  MOZ_ASSERT(anyChars.isCurrentTokenType(TokenKind::For));
||||||| merged common ancestors
typename ParseHandler::Node
GeneralParser<ParseHandler, Unit>::forStatement(YieldHandling yieldHandling)
{
    MOZ_ASSERT(anyChars.isCurrentTokenType(TokenKind::For));

    uint32_t begin = pos().begin;
=======
typename ParseHandler::Node GeneralParser<ParseHandler, Unit>::forStatement(
    YieldHandling yieldHandling) {
  MOZ_ASSERT(anyChars.isCurrentTokenType(TokenKind::For));

  uint32_t begin = pos().begin;
>>>>>>> upstream-releases

<<<<<<< HEAD
  uint32_t begin = pos().begin;
||||||| merged common ancestors
    ParseContext::Statement stmt(pc, StatementKind::ForLoop);
=======
  ParseContext::Statement stmt(pc_, StatementKind::ForLoop);
>>>>>>> upstream-releases

<<<<<<< HEAD
  ParseContext::Statement stmt(pc, StatementKind::ForLoop);
||||||| merged common ancestors
    IteratorKind iterKind = IteratorKind::Sync;
    unsigned iflags = 0;
=======
  IteratorKind iterKind = IteratorKind::Sync;
  unsigned iflags = 0;
>>>>>>> upstream-releases

<<<<<<< HEAD
  IteratorKind iterKind = IteratorKind::Sync;
  unsigned iflags = 0;
||||||| merged common ancestors
    if (pc->isAsync()) {
        bool matched;
        if (!tokenStream.matchToken(&matched, TokenKind::Await)) {
            return null();
        }
=======
  if (pc_->isAsync()) {
    bool matched;
    if (!tokenStream.matchToken(&matched, TokenKind::Await)) {
      return null();
    }
>>>>>>> upstream-releases

<<<<<<< HEAD
  if (pc->isAsync()) {
    bool matched;
    if (!tokenStream.matchToken(&matched, TokenKind::Await)) {
      return null();
||||||| merged common ancestors
        if (matched) {
            iflags |= JSITER_FORAWAITOF;
            iterKind = IteratorKind::Async;
        }
=======
    if (matched) {
      iflags |= JSITER_FORAWAITOF;
      iterKind = IteratorKind::Async;
>>>>>>> upstream-releases
    }
  }

<<<<<<< HEAD
    if (matched) {
      iflags |= JSITER_FORAWAITOF;
      iterKind = IteratorKind::Async;
    }
  }
||||||| merged common ancestors
    MUST_MATCH_TOKEN_MOD_WITH_REPORT(TokenKind::LeftParen, TokenStream::None,
                                     error((token == TokenKind::Await && !pc->isAsync())
                                           ? JSMSG_FOR_AWAIT_OUTSIDE_ASYNC
                                           : JSMSG_PAREN_AFTER_FOR));

    // ParseNodeKind::ForHead, ParseNodeKind::ForIn, or
    // ParseNodeKind::ForOf depending on the loop type.
    ParseNodeKind headKind;
=======
  if (!mustMatchToken(TokenKind::LeftParen, [this](TokenKind actual) {
        this->error((actual == TokenKind::Await && !this->pc_->isAsync())
                        ? JSMSG_FOR_AWAIT_OUTSIDE_ASYNC
                        : JSMSG_PAREN_AFTER_FOR);
      })) {
    return null();
  }

  // ParseNodeKind::ForHead, ParseNodeKind::ForIn, or
  // ParseNodeKind::ForOf depending on the loop type.
  ParseNodeKind headKind;

  // |x| in either |for (x; ...; ...)| or |for (x in/of ...)|.
  Node startNode;

  // The next two variables are used to implement `for (let/const ...)`.
  //
  // We generate an implicit block, wrapping the whole loop, to store loop
  // variables declared this way. Note that if the loop uses `for (var...)`
  // instead, those variables go on some existing enclosing scope, so no
  // implicit block scope is created.
  //
  // Both variables remain null/none if the loop is any other form.

  // The static block scope for the implicit block scope.
  Maybe<ParseContext::Scope> forLoopLexicalScope;

  // The expression being iterated over, for for-in/of loops only.  Unused
  // for for(;;) loops.
  Node iteratedExpr;

  // Parse the entirety of the loop-head for a for-in/of loop (so the next
  // token is the closing ')'):
  //
  //   for (... in/of ...) ...
  //                     ^next token
  //
  // ...OR, parse up to the first ';' in a C-style for-loop:
  //
  //   for (...; ...; ...) ...
  //           ^next token
  //
  // In either case the subsequent token can be consistently accessed using
  // TokenStream::SlashIsDiv semantics.
  if (!forHeadStart(yieldHandling, &headKind, &startNode, forLoopLexicalScope,
                    &iteratedExpr)) {
    return null();
  }

  MOZ_ASSERT(headKind == ParseNodeKind::ForIn ||
             headKind == ParseNodeKind::ForOf ||
             headKind == ParseNodeKind::ForHead);
>>>>>>> upstream-releases

<<<<<<< HEAD
  if (!mustMatchToken(TokenKind::LeftParen, [this](TokenKind actual) {
        this->error((actual == TokenKind::Await && !this->pc->isAsync())
                        ? JSMSG_FOR_AWAIT_OUTSIDE_ASYNC
                        : JSMSG_PAREN_AFTER_FOR);
      })) {
    return null();
  }

  // ParseNodeKind::ForHead, ParseNodeKind::ForIn, or
  // ParseNodeKind::ForOf depending on the loop type.
  ParseNodeKind headKind;

  // |x| in either |for (x; ...; ...)| or |for (x in/of ...)|.
  Node startNode;

  // The next two variables are used to implement `for (let/const ...)`.
  //
  // We generate an implicit block, wrapping the whole loop, to store loop
  // variables declared this way. Note that if the loop uses `for (var...)`
  // instead, those variables go on some existing enclosing scope, so no
  // implicit block scope is created.
  //
  // Both variables remain null/none if the loop is any other form.

  // The static block scope for the implicit block scope.
  Maybe<ParseContext::Scope> forLoopLexicalScope;

  // The expression being iterated over, for for-in/of loops only.  Unused
  // for for(;;) loops.
  Node iteratedExpr;

  // Parse the entirety of the loop-head for a for-in/of loop (so the next
  // token is the closing ')'):
  //
  //   for (... in/of ...) ...
  //                     ^next token
  //
  // ...OR, parse up to the first ';' in a C-style for-loop:
  //
  //   for (...; ...; ...) ...
  //           ^next token
  //
  // In either case the subsequent token can be consistently accessed using
  // TokenStream::None semantics.
  if (!forHeadStart(yieldHandling, &headKind, &startNode, forLoopLexicalScope,
                    &iteratedExpr)) {
    return null();
  }
||||||| merged common ancestors
    // |x| in either |for (x; ...; ...)| or |for (x in/of ...)|.
    Node startNode;
=======
  if (iterKind == IteratorKind::Async && headKind != ParseNodeKind::ForOf) {
    errorAt(begin, JSMSG_FOR_AWAIT_NOT_OF);
    return null();
  }
>>>>>>> upstream-releases

<<<<<<< HEAD
  MOZ_ASSERT(headKind == ParseNodeKind::ForIn ||
             headKind == ParseNodeKind::ForOf ||
             headKind == ParseNodeKind::ForHead);
||||||| merged common ancestors
    // The next two variables are used to implement `for (let/const ...)`.
    //
    // We generate an implicit block, wrapping the whole loop, to store loop
    // variables declared this way. Note that if the loop uses `for (var...)`
    // instead, those variables go on some existing enclosing scope, so no
    // implicit block scope is created.
    //
    // Both variables remain null/none if the loop is any other form.
=======
  TernaryNodeType forHead;
  if (headKind == ParseNodeKind::ForHead) {
    Node init = startNode;
>>>>>>> upstream-releases

<<<<<<< HEAD
  if (iterKind == IteratorKind::Async && headKind != ParseNodeKind::ForOf) {
    errorAt(begin, JSMSG_FOR_AWAIT_NOT_OF);
    return null();
  }
||||||| merged common ancestors
    // The static block scope for the implicit block scope.
    Maybe<ParseContext::Scope> forLoopLexicalScope;
=======
    // Look for an operand: |for (;| means we might have already examined
    // this semicolon with that modifier.
    if (!mustMatchToken(TokenKind::Semi, JSMSG_SEMI_AFTER_FOR_INIT)) {
      return null();
    }
>>>>>>> upstream-releases

<<<<<<< HEAD
  TernaryNodeType forHead;
  if (headKind == ParseNodeKind::ForHead) {
    Node init = startNode;
||||||| merged common ancestors
    // The expression being iterated over, for for-in/of loops only.  Unused
    // for for(;;) loops.
    Node iteratedExpr;
=======
    TokenKind tt;
    if (!tokenStream.peekToken(&tt, TokenStream::SlashIsRegExp)) {
      return null();
    }
>>>>>>> upstream-releases

<<<<<<< HEAD
    // Look for an operand: |for (;| means we might have already examined
    // this semicolon with that modifier.
    if (!mustMatchToken(TokenKind::Semi, TokenStream::Operand,
                        JSMSG_SEMI_AFTER_FOR_INIT)) {
      return null();
||||||| merged common ancestors
    // Parse the entirety of the loop-head for a for-in/of loop (so the next
    // token is the closing ')'):
    //
    //   for (... in/of ...) ...
    //                     ^next token
    //
    // ...OR, parse up to the first ';' in a C-style for-loop:
    //
    //   for (...; ...; ...) ...
    //           ^next token
    //
    // In either case the subsequent token can be consistently accessed using
    // TokenStream::None semantics.
    if (!forHeadStart(yieldHandling, &headKind, &startNode, forLoopLexicalScope, &iteratedExpr)) {
        return null();
=======
    Node test;
    if (tt == TokenKind::Semi) {
      test = null();
    } else {
      test = expr(InAllowed, yieldHandling, TripledotProhibited);
      if (!test) {
        return null();
      }
>>>>>>> upstream-releases
    }

<<<<<<< HEAD
    TokenKind tt;
    if (!tokenStream.peekToken(&tt, TokenStream::Operand)) {
      return null();
    }
||||||| merged common ancestors
    MOZ_ASSERT(headKind == ParseNodeKind::ForIn ||
               headKind == ParseNodeKind::ForOf ||
               headKind == ParseNodeKind::ForHead);
=======
    if (!mustMatchToken(TokenKind::Semi, JSMSG_SEMI_AFTER_FOR_COND)) {
      return null();
    }

    if (!tokenStream.peekToken(&tt, TokenStream::SlashIsRegExp)) {
      return null();
    }
>>>>>>> upstream-releases

<<<<<<< HEAD
    Node test;
    if (tt == TokenKind::Semi) {
      test = null();
    } else {
      test = expr(InAllowed, yieldHandling, TripledotProhibited);
      if (!test) {
||||||| merged common ancestors
    if (iterKind == IteratorKind::Async && headKind != ParseNodeKind::ForOf) {
        errorAt(begin, JSMSG_FOR_AWAIT_NOT_OF);
=======
    Node update;
    if (tt == TokenKind::RightParen) {
      update = null();
    } else {
      update = expr(InAllowed, yieldHandling, TripledotProhibited);
      if (!update) {
>>>>>>> upstream-releases
        return null();
<<<<<<< HEAD
      }
||||||| merged common ancestors
=======
      }
    }

    if (!mustMatchToken(TokenKind::RightParen, JSMSG_PAREN_AFTER_FOR_CTRL)) {
      return null();
>>>>>>> upstream-releases
    }

<<<<<<< HEAD
    if (!mustMatchToken(TokenKind::Semi, TokenStream::Operand,
                        JSMSG_SEMI_AFTER_FOR_COND)) {
      return null();
    }
||||||| merged common ancestors
    TernaryNodeType forHead;
    if (headKind == ParseNodeKind::ForHead) {
        Node init = startNode;

        // Look for an operand: |for (;| means we might have already examined
        // this semicolon with that modifier.
        MUST_MATCH_TOKEN_MOD(TokenKind::Semi, TokenStream::Operand, JSMSG_SEMI_AFTER_FOR_INIT);

        TokenKind tt;
        if (!tokenStream.peekToken(&tt, TokenStream::Operand)) {
            return null();
        }

        Node test;
        if (tt == TokenKind::Semi) {
            test = null();
        } else {
            test = expr(InAllowed, yieldHandling, TripledotProhibited);
            if (!test) {
                return null();
            }
        }

        MUST_MATCH_TOKEN_MOD(TokenKind::Semi, TokenStream::Operand, JSMSG_SEMI_AFTER_FOR_COND);

        if (!tokenStream.peekToken(&tt, TokenStream::Operand)) {
            return null();
        }

        Node update;
        if (tt == TokenKind::RightParen) {
            update = null();
        } else {
            update = expr(InAllowed, yieldHandling, TripledotProhibited);
            if (!update) {
                return null();
            }
        }
=======
    TokenPos headPos(begin, pos().end);
    forHead = handler_.newForHead(init, test, update, headPos);
    if (!forHead) {
      return null();
    }
  } else {
    MOZ_ASSERT(headKind == ParseNodeKind::ForIn ||
               headKind == ParseNodeKind::ForOf);

    // |target| is the LeftHandSideExpression or declaration to which the
    // per-iteration value (an arbitrary value exposed by the iteration
    // protocol, or a string naming a property) is assigned.
    Node target = startNode;

    // Parse the rest of the for-in/of head.
    if (headKind == ParseNodeKind::ForIn) {
      stmt.refineForKind(StatementKind::ForInLoop);
    } else {
      stmt.refineForKind(StatementKind::ForOfLoop);
    }

    // Parser::declaration consumed everything up to the closing ')'.  That
    // token follows an {Assignment,}Expression and so must be interpreted
    // as an operand to be consistent with normal expression tokenizing.
    if (!mustMatchToken(TokenKind::RightParen, JSMSG_PAREN_AFTER_FOR_CTRL)) {
      return null();
    }

    TokenPos headPos(begin, pos().end);
    forHead =
        handler_.newForInOrOfHead(headKind, target, iteratedExpr, headPos);
    if (!forHead) {
      return null();
    }
  }

  Node body = statement(yieldHandling);
  if (!body) {
    return null();
  }

  ForNodeType forLoop = handler_.newForStatement(begin, forHead, body, iflags);
  if (!forLoop) {
    return null();
  }

  if (forLoopLexicalScope) {
    return finishLexicalScope(*forLoopLexicalScope, forLoop);
  }

  return forLoop;
}

template <class ParseHandler, typename Unit>
typename ParseHandler::SwitchStatementType
GeneralParser<ParseHandler, Unit>::switchStatement(
    YieldHandling yieldHandling) {
  MOZ_ASSERT(anyChars.isCurrentTokenType(TokenKind::Switch));
  uint32_t begin = pos().begin;

  if (!mustMatchToken(TokenKind::LeftParen, JSMSG_PAREN_BEFORE_SWITCH)) {
    return null();
  }

  Node discriminant =
      exprInParens(InAllowed, yieldHandling, TripledotProhibited);
  if (!discriminant) {
    return null();
  }

  if (!mustMatchToken(TokenKind::RightParen, JSMSG_PAREN_AFTER_SWITCH)) {
    return null();
  }
  if (!mustMatchToken(TokenKind::LeftCurly, JSMSG_CURLY_BEFORE_SWITCH)) {
    return null();
  }

  ParseContext::Statement stmt(pc_, StatementKind::Switch);
  ParseContext::Scope scope(this);
  if (!scope.init(pc_)) {
    return null();
  }

  ListNodeType caseList = handler_.newStatementList(pos());
  if (!caseList) {
    return null();
  }

  bool seenDefault = false;
  TokenKind tt;
  while (true) {
    if (!tokenStream.getToken(&tt, TokenStream::SlashIsRegExp)) {
      return null();
    }
    if (tt == TokenKind::RightCurly) {
      break;
    }
    uint32_t caseBegin = pos().begin;

    Node caseExpr;
    switch (tt) {
      case TokenKind::Default:
        if (seenDefault) {
          error(JSMSG_TOO_MANY_DEFAULTS);
          return null();
        }
        seenDefault = true;
        caseExpr = null();  // The default case has pn_left == nullptr.
        break;

      case TokenKind::Case:
        caseExpr = expr(InAllowed, yieldHandling, TripledotProhibited);
        if (!caseExpr) {
          return null();
        }
        break;
>>>>>>> upstream-releases

<<<<<<< HEAD
    if (!tokenStream.peekToken(&tt, TokenStream::Operand)) {
      return null();
    }
||||||| merged common ancestors
        MUST_MATCH_TOKEN_MOD(TokenKind::RightParen, TokenStream::Operand, JSMSG_PAREN_AFTER_FOR_CTRL);
=======
      default:
        error(JSMSG_BAD_SWITCH);
        return null();
    }
>>>>>>> upstream-releases

<<<<<<< HEAD
    Node update;
    if (tt == TokenKind::RightParen) {
      update = null();
    } else {
      update = expr(InAllowed, yieldHandling, TripledotProhibited);
      if (!update) {
        return null();
      }
    }
||||||| merged common ancestors
        TokenPos headPos(begin, pos().end);
        forHead = handler.newForHead(init, test, update, headPos);
        if (!forHead) {
            return null();
        }
    } else {
        MOZ_ASSERT(headKind == ParseNodeKind::ForIn || headKind == ParseNodeKind::ForOf);
=======
    if (!mustMatchToken(TokenKind::Colon, JSMSG_COLON_AFTER_CASE)) {
      return null();
    }
>>>>>>> upstream-releases

<<<<<<< HEAD
    if (!mustMatchToken(TokenKind::RightParen, TokenStream::Operand,
                        JSMSG_PAREN_AFTER_FOR_CTRL)) {
      return null();
    }
||||||| merged common ancestors
        // |target| is the LeftHandSideExpression or declaration to which the
        // per-iteration value (an arbitrary value exposed by the iteration
        // protocol, or a string naming a property) is assigned.
        Node target = startNode;
=======
    ListNodeType body = handler_.newStatementList(pos());
    if (!body) {
      return null();
    }
>>>>>>> upstream-releases

<<<<<<< HEAD
    TokenPos headPos(begin, pos().end);
    forHead = handler.newForHead(init, test, update, headPos);
    if (!forHead) {
      return null();
    }
  } else {
    MOZ_ASSERT(headKind == ParseNodeKind::ForIn ||
               headKind == ParseNodeKind::ForOf);
||||||| merged common ancestors
        // Parse the rest of the for-in/of head.
        if (headKind == ParseNodeKind::ForIn) {
            stmt.refineForKind(StatementKind::ForInLoop);
        } else {
            stmt.refineForKind(StatementKind::ForOfLoop);
        }
=======
    bool afterReturn = false;
    bool warnedAboutStatementsAfterReturn = false;
    uint32_t statementBegin = 0;
    while (true) {
      if (!tokenStream.peekToken(&tt, TokenStream::SlashIsRegExp)) {
        return null();
      }
      if (tt == TokenKind::RightCurly || tt == TokenKind::Case ||
          tt == TokenKind::Default) {
        break;
      }
      if (afterReturn) {
        if (!tokenStream.peekOffset(&statementBegin,
                                    TokenStream::SlashIsRegExp)) {
          return null();
        }
      }
      Node stmt = statementListItem(yieldHandling);
      if (!stmt) {
        return null();
      }
      if (!warnedAboutStatementsAfterReturn) {
        if (afterReturn) {
          if (!handler_.isStatementPermittedAfterReturnStatement(stmt)) {
            if (!warningAt(statementBegin, JSMSG_STMT_AFTER_RETURN)) {
              return null();
            }
>>>>>>> upstream-releases

<<<<<<< HEAD
    // |target| is the LeftHandSideExpression or declaration to which the
    // per-iteration value (an arbitrary value exposed by the iteration
    // protocol, or a string naming a property) is assigned.
    Node target = startNode;

    // Parse the rest of the for-in/of head.
    if (headKind == ParseNodeKind::ForIn) {
      stmt.refineForKind(StatementKind::ForInLoop);
    } else {
      stmt.refineForKind(StatementKind::ForOfLoop);
||||||| merged common ancestors
        // Parser::declaration consumed everything up to the closing ')'.  That
        // token follows an {Assignment,}Expression and so must be interpreted
        // as an operand to be consistent with normal expression tokenizing.
        MUST_MATCH_TOKEN_MOD(TokenKind::RightParen, TokenStream::Operand, JSMSG_PAREN_AFTER_FOR_CTRL);

        TokenPos headPos(begin, pos().end);
        forHead = handler.newForInOrOfHead(headKind, target, iteratedExpr, headPos);
        if (!forHead) {
            return null();
        }
=======
            warnedAboutStatementsAfterReturn = true;
          }
        } else if (handler_.isReturnStatement(stmt)) {
          afterReturn = true;
        }
      }
      handler_.addStatementToList(body, stmt);
    }

    CaseClauseType caseClause =
        handler_.newCaseOrDefault(caseBegin, caseExpr, body);
    if (!caseClause) {
      return null();
>>>>>>> upstream-releases
    }
    handler_.addCaseStatementToList(caseList, caseClause);
  }

  LexicalScopeNodeType lexicalForCaseList = finishLexicalScope(scope, caseList);
  if (!lexicalForCaseList) {
    return null();
  }

  handler_.setEndPosition(lexicalForCaseList, pos().end);

  return handler_.newSwitchStatement(begin, discriminant, lexicalForCaseList,
                                     seenDefault);
}

template <class ParseHandler, typename Unit>
typename ParseHandler::ContinueStatementType
GeneralParser<ParseHandler, Unit>::continueStatement(
    YieldHandling yieldHandling) {
  MOZ_ASSERT(anyChars.isCurrentTokenType(TokenKind::Continue));
  uint32_t begin = pos().begin;

<<<<<<< HEAD
    // Parser::declaration consumed everything up to the closing ')'.  That
    // token follows an {Assignment,}Expression and so must be interpreted
    // as an operand to be consistent with normal expression tokenizing.
    if (!mustMatchToken(TokenKind::RightParen, TokenStream::Operand,
                        JSMSG_PAREN_AFTER_FOR_CTRL)) {
      return null();
    }
||||||| merged common ancestors
    Node body = statement(yieldHandling);
    if (!body) {
        return null();
    }
=======
  RootedPropertyName label(cx_);
  if (!matchLabel(yieldHandling, &label)) {
    return null();
  }
>>>>>>> upstream-releases

<<<<<<< HEAD
    TokenPos headPos(begin, pos().end);
    forHead = handler.newForInOrOfHead(headKind, target, iteratedExpr, headPos);
    if (!forHead) {
      return null();
||||||| merged common ancestors
    ForNodeType forLoop = handler.newForStatement(begin, forHead, body, iflags);
    if (!forLoop) {
        return null();
=======
  auto validity = pc_->checkContinueStatement(label);
  if (validity.isErr()) {
    switch (validity.unwrapErr()) {
      case ParseContext::ContinueStatementError::NotInALoop:
        errorAt(begin, JSMSG_BAD_CONTINUE);
        break;
      case ParseContext::ContinueStatementError::LabelNotFound:
        error(JSMSG_LABEL_NOT_FOUND);
        break;
>>>>>>> upstream-releases
    }
<<<<<<< HEAD
  }
||||||| merged common ancestors
=======
    return null();
  }
>>>>>>> upstream-releases

<<<<<<< HEAD
  Node body = statement(yieldHandling);
  if (!body) {
    return null();
  }
||||||| merged common ancestors
    if (forLoopLexicalScope) {
        return finishLexicalScope(*forLoopLexicalScope, forLoop);
    }
=======
  if (!matchOrInsertSemicolon()) {
    return null();
  }
>>>>>>> upstream-releases

<<<<<<< HEAD
  ForNodeType forLoop = handler.newForStatement(begin, forHead, body, iflags);
  if (!forLoop) {
    return null();
  }

  if (forLoopLexicalScope) {
    return finishLexicalScope(*forLoopLexicalScope, forLoop);
  }

  return forLoop;
||||||| merged common ancestors
    return forLoop;
=======
  return handler_.newContinueStatement(label, TokenPos(begin, pos().end));
>>>>>>> upstream-releases
}

template <class ParseHandler, typename Unit>
<<<<<<< HEAD
typename ParseHandler::SwitchStatementType
GeneralParser<ParseHandler, Unit>::switchStatement(
    YieldHandling yieldHandling) {
  MOZ_ASSERT(anyChars.isCurrentTokenType(TokenKind::Switch));
  uint32_t begin = pos().begin;
||||||| merged common ancestors
typename ParseHandler::SwitchStatementType
GeneralParser<ParseHandler, Unit>::switchStatement(YieldHandling yieldHandling)
{
    MOZ_ASSERT(anyChars.isCurrentTokenType(TokenKind::Switch));
    uint32_t begin = pos().begin;
=======
typename ParseHandler::BreakStatementType
GeneralParser<ParseHandler, Unit>::breakStatement(YieldHandling yieldHandling) {
  MOZ_ASSERT(anyChars.isCurrentTokenType(TokenKind::Break));
  uint32_t begin = pos().begin;
>>>>>>> upstream-releases

<<<<<<< HEAD
  if (!mustMatchToken(TokenKind::LeftParen, JSMSG_PAREN_BEFORE_SWITCH)) {
    return null();
  }
||||||| merged common ancestors
    MUST_MATCH_TOKEN(TokenKind::LeftParen, JSMSG_PAREN_BEFORE_SWITCH);
=======
  RootedPropertyName label(cx_);
  if (!matchLabel(yieldHandling, &label)) {
    return null();
  }
>>>>>>> upstream-releases

<<<<<<< HEAD
  Node discriminant =
      exprInParens(InAllowed, yieldHandling, TripledotProhibited);
  if (!discriminant) {
    return null();
  }
||||||| merged common ancestors
    Node discriminant = exprInParens(InAllowed, yieldHandling, TripledotProhibited);
    if (!discriminant) {
        return null();
    }
=======
  auto validity = pc_->checkBreakStatement(label);
  if (validity.isErr()) {
    switch (validity.unwrapErr()) {
      case ParseContext::BreakStatementError::ToughBreak:
        errorAt(begin, JSMSG_TOUGH_BREAK);
        return null();
      case ParseContext::BreakStatementError::LabelNotFound:
        error(JSMSG_LABEL_NOT_FOUND);
        return null();
    }
  }
>>>>>>> upstream-releases

<<<<<<< HEAD
  if (!mustMatchToken(TokenKind::RightParen, TokenStream::Operand,
                      JSMSG_PAREN_AFTER_SWITCH)) {
    return null();
  }
  if (!mustMatchToken(TokenKind::LeftCurly, JSMSG_CURLY_BEFORE_SWITCH)) {
    return null();
  }
||||||| merged common ancestors
    MUST_MATCH_TOKEN_MOD(TokenKind::RightParen, TokenStream::Operand, JSMSG_PAREN_AFTER_SWITCH);
    MUST_MATCH_TOKEN(TokenKind::LeftCurly, JSMSG_CURLY_BEFORE_SWITCH);
=======
  if (!matchOrInsertSemicolon()) {
    return null();
  }
>>>>>>> upstream-releases

<<<<<<< HEAD
  ParseContext::Statement stmt(pc, StatementKind::Switch);
  ParseContext::Scope scope(this);
  if (!scope.init(pc)) {
    return null();
  }
||||||| merged common ancestors
    ParseContext::Statement stmt(pc, StatementKind::Switch);
    ParseContext::Scope scope(this);
    if (!scope.init(pc)) {
        return null();
    }
=======
  return handler_.newBreakStatement(label, TokenPos(begin, pos().end));
}
>>>>>>> upstream-releases

<<<<<<< HEAD
  ListNodeType caseList = handler.newStatementList(pos());
  if (!caseList) {
    return null();
  }

  bool seenDefault = false;
  TokenKind tt;
  while (true) {
    if (!tokenStream.getToken(&tt, TokenStream::Operand)) {
      return null();
    }
    if (tt == TokenKind::RightCurly) {
      break;
||||||| merged common ancestors
    ListNodeType caseList = handler.newStatementList(pos());
    if (!caseList) {
        return null();
=======
template <class ParseHandler, typename Unit>
typename ParseHandler::UnaryNodeType
GeneralParser<ParseHandler, Unit>::returnStatement(
    YieldHandling yieldHandling) {
  MOZ_ASSERT(anyChars.isCurrentTokenType(TokenKind::Return));
  uint32_t begin = pos().begin;

  MOZ_ASSERT(pc_->isFunctionBox());
  pc_->functionBox()->usesReturn = true;

  // Parse an optional operand.
  //
  // This is ugly, but we don't want to require a semicolon.
  Node exprNode;
  TokenKind tt = TokenKind::Eof;
  if (!tokenStream.peekTokenSameLine(&tt, TokenStream::SlashIsRegExp)) {
    return null();
  }
  switch (tt) {
    case TokenKind::Eol:
    case TokenKind::Eof:
    case TokenKind::Semi:
    case TokenKind::RightCurly:
      exprNode = null();
      break;
    default: {
      exprNode = expr(InAllowed, yieldHandling, TripledotProhibited);
      if (!exprNode) {
        return null();
      }
>>>>>>> upstream-releases
    }
<<<<<<< HEAD
    uint32_t caseBegin = pos().begin;
||||||| merged common ancestors
=======
  }
>>>>>>> upstream-releases

<<<<<<< HEAD
    Node caseExpr;
    switch (tt) {
      case TokenKind::Default:
        if (seenDefault) {
          error(JSMSG_TOO_MANY_DEFAULTS);
          return null();
        }
        seenDefault = true;
        caseExpr = null();  // The default case has pn_left == nullptr.
        break;

      case TokenKind::Case:
        caseExpr = expr(InAllowed, yieldHandling, TripledotProhibited);
        if (!caseExpr) {
          return null();
        }
        break;
||||||| merged common ancestors
    bool seenDefault = false;
    TokenKind tt;
    while (true) {
        if (!tokenStream.getToken(&tt, TokenStream::Operand)) {
            return null();
        }
        if (tt == TokenKind::RightCurly) {
            break;
        }
        uint32_t caseBegin = pos().begin;

        Node caseExpr;
        switch (tt) {
          case TokenKind::Default:
            if (seenDefault) {
                error(JSMSG_TOO_MANY_DEFAULTS);
                return null();
            }
            seenDefault = true;
            caseExpr = null();  // The default case has pn_left == nullptr.
            break;
=======
  if (!matchOrInsertSemicolon()) {
    return null();
  }
>>>>>>> upstream-releases

<<<<<<< HEAD
      default:
        error(JSMSG_BAD_SWITCH);
        return null();
    }
||||||| merged common ancestors
          case TokenKind::Case:
            caseExpr = expr(InAllowed, yieldHandling, TripledotProhibited);
            if (!caseExpr) {
                return null();
            }
            break;
=======
  return handler_.newReturnStatement(exprNode, TokenPos(begin, pos().end));
}
>>>>>>> upstream-releases

<<<<<<< HEAD
    if (!mustMatchToken(TokenKind::Colon, TokenStream::Operand,
                        JSMSG_COLON_AFTER_CASE)) {
      return null();
    }
||||||| merged common ancestors
          default:
            error(JSMSG_BAD_SWITCH);
            return null();
        }
=======
template <class ParseHandler, typename Unit>
typename ParseHandler::UnaryNodeType
GeneralParser<ParseHandler, Unit>::yieldExpression(InHandling inHandling) {
  MOZ_ASSERT(anyChars.isCurrentTokenType(TokenKind::Yield));
  uint32_t begin = pos().begin;
>>>>>>> upstream-releases

<<<<<<< HEAD
    ListNodeType body = handler.newStatementList(pos());
    if (!body) {
      return null();
    }
||||||| merged common ancestors
        MUST_MATCH_TOKEN_MOD(TokenKind::Colon, TokenStream::Operand, JSMSG_COLON_AFTER_CASE);
=======
  MOZ_ASSERT(pc_->isGenerator());
  MOZ_ASSERT(pc_->isFunctionBox());
>>>>>>> upstream-releases

<<<<<<< HEAD
    bool afterReturn = false;
    bool warnedAboutStatementsAfterReturn = false;
    uint32_t statementBegin = 0;
    while (true) {
      if (!tokenStream.peekToken(&tt, TokenStream::Operand)) {
        return null();
      }
      if (tt == TokenKind::RightCurly || tt == TokenKind::Case ||
          tt == TokenKind::Default) {
        break;
      }
      if (afterReturn) {
        if (!tokenStream.peekOffset(&statementBegin, TokenStream::Operand)) {
          return null();
        }
      }
      Node stmt = statementListItem(yieldHandling);
      if (!stmt) {
        return null();
      }
      if (!warnedAboutStatementsAfterReturn) {
        if (afterReturn) {
          if (!handler.isStatementPermittedAfterReturnStatement(stmt)) {
            if (!warningAt(statementBegin, JSMSG_STMT_AFTER_RETURN)) {
              return null();
            }
||||||| merged common ancestors
        ListNodeType body = handler.newStatementList(pos());
        if (!body) {
            return null();
        }

        bool afterReturn = false;
        bool warnedAboutStatementsAfterReturn = false;
        uint32_t statementBegin = 0;
        while (true) {
            if (!tokenStream.peekToken(&tt, TokenStream::Operand)) {
                return null();
            }
            if (tt == TokenKind::RightCurly || tt == TokenKind::Case || tt == TokenKind::Default) {
                break;
            }
            if (afterReturn) {
                if (!tokenStream.peekOffset(&statementBegin, TokenStream::Operand)) {
                    return null();
                }
            }
            Node stmt = statementListItem(yieldHandling);
            if (!stmt) {
                return null();
            }
            if (!warnedAboutStatementsAfterReturn) {
                if (afterReturn) {
                    if (!handler.isStatementPermittedAfterReturnStatement(stmt)) {
                        if (!warningAt(statementBegin, JSMSG_STMT_AFTER_RETURN)) {
                            return null();
                        }

                        warnedAboutStatementsAfterReturn = true;
                    }
                } else if (handler.isReturnStatement(stmt)) {
                    afterReturn = true;
                }
            }
            handler.addStatementToList(body, stmt);
        }
=======
  pc_->lastYieldOffset = begin;

  Node exprNode;
  ParseNodeKind kind = ParseNodeKind::YieldExpr;
  TokenKind tt = TokenKind::Eof;
  if (!tokenStream.peekTokenSameLine(&tt, TokenStream::SlashIsRegExp)) {
    return null();
  }
  switch (tt) {
    // TokenKind::Eol is special; it implements the [no LineTerminator here]
    // quirk in the grammar.
    case TokenKind::Eol:
    // The rest of these make up the complete set of tokens that can
    // appear after any of the places where AssignmentExpression is used
    // throughout the grammar.  Conveniently, none of them can also be the
    // start an expression.
    case TokenKind::Eof:
    case TokenKind::Semi:
    case TokenKind::RightCurly:
    case TokenKind::RightBracket:
    case TokenKind::RightParen:
    case TokenKind::Colon:
    case TokenKind::Comma:
    case TokenKind::In:  // Annex B.3.6 `for (x = yield in y) ;`
      // No value.
      exprNode = null();
      break;
    case TokenKind::Mul:
      kind = ParseNodeKind::YieldStarExpr;
      tokenStream.consumeKnownToken(TokenKind::Mul, TokenStream::SlashIsRegExp);
      MOZ_FALLTHROUGH;
    default:
      exprNode = assignExpr(inHandling, YieldIsKeyword, TripledotProhibited);
      if (!exprNode) {
        return null();
      }
  }
  if (kind == ParseNodeKind::YieldStarExpr) {
    return handler_.newYieldStarExpression(begin, exprNode);
  }
  return handler_.newYieldExpression(begin, exprNode);
}
>>>>>>> upstream-releases

<<<<<<< HEAD
            warnedAboutStatementsAfterReturn = true;
          }
        } else if (handler.isReturnStatement(stmt)) {
          afterReturn = true;
        }
      }
      handler.addStatementToList(body, stmt);
    }
||||||| merged common ancestors
        CaseClauseType caseClause = handler.newCaseOrDefault(caseBegin, caseExpr, body);
        if (!caseClause) {
            return null();
        }
        handler.addCaseStatementToList(caseList, caseClause);
    }
=======
template <class ParseHandler, typename Unit>
typename ParseHandler::BinaryNodeType
GeneralParser<ParseHandler, Unit>::withStatement(YieldHandling yieldHandling) {
  MOZ_ASSERT(anyChars.isCurrentTokenType(TokenKind::With));
  uint32_t begin = pos().begin;

  // Usually we want the constructs forbidden in strict mode code to be a
  // subset of those that ContextOptions::extraWarnings() warns about, and we
  // use strictModeError directly.  But while 'with' is forbidden in strict
  // mode code, it doesn't even merit a warning in non-strict code.  See
  // https://bugzilla.mozilla.org/show_bug.cgi?id=514576#c1.
  if (pc_->sc()->strict()) {
    if (!strictModeError(JSMSG_STRICT_CODE_WITH)) {
      return null();
    }
  }

  if (!mustMatchToken(TokenKind::LeftParen, JSMSG_PAREN_BEFORE_WITH)) {
    return null();
  }
>>>>>>> upstream-releases

<<<<<<< HEAD
    CaseClauseType caseClause =
        handler.newCaseOrDefault(caseBegin, caseExpr, body);
    if (!caseClause) {
      return null();
||||||| merged common ancestors
    LexicalScopeNodeType lexicalForCaseList = finishLexicalScope(scope, caseList);
    if (!lexicalForCaseList) {
        return null();
=======
  Node objectExpr = exprInParens(InAllowed, yieldHandling, TripledotProhibited);
  if (!objectExpr) {
    return null();
  }

  if (!mustMatchToken(TokenKind::RightParen, JSMSG_PAREN_AFTER_WITH)) {
    return null();
  }

  Node innerBlock;
  {
    ParseContext::Statement stmt(pc_, StatementKind::With);
    innerBlock = statement(yieldHandling);
    if (!innerBlock) {
      return null();
>>>>>>> upstream-releases
    }
<<<<<<< HEAD
    handler.addCaseStatementToList(caseList, caseClause);
  }

  LexicalScopeNodeType lexicalForCaseList = finishLexicalScope(scope, caseList);
  if (!lexicalForCaseList) {
    return null();
  }
||||||| merged common ancestors
=======
  }
>>>>>>> upstream-releases

<<<<<<< HEAD
  handler.setEndPosition(lexicalForCaseList, pos().end);
||||||| merged common ancestors
    handler.setEndPosition(lexicalForCaseList, pos().end);
=======
  pc_->sc()->setBindingsAccessedDynamically();
>>>>>>> upstream-releases

<<<<<<< HEAD
  return handler.newSwitchStatement(begin, discriminant, lexicalForCaseList,
                                    seenDefault);
||||||| merged common ancestors
    return handler.newSwitchStatement(begin, discriminant, lexicalForCaseList, seenDefault);
=======
  return handler_.newWithStatement(begin, objectExpr, innerBlock);
>>>>>>> upstream-releases
}

template <class ParseHandler, typename Unit>
<<<<<<< HEAD
typename ParseHandler::ContinueStatementType
GeneralParser<ParseHandler, Unit>::continueStatement(
    YieldHandling yieldHandling) {
  MOZ_ASSERT(anyChars.isCurrentTokenType(TokenKind::Continue));
  uint32_t begin = pos().begin;
||||||| merged common ancestors
typename ParseHandler::ContinueStatementType
GeneralParser<ParseHandler, Unit>::continueStatement(YieldHandling yieldHandling)
{
    MOZ_ASSERT(anyChars.isCurrentTokenType(TokenKind::Continue));
    uint32_t begin = pos().begin;
=======
typename ParseHandler::Node GeneralParser<ParseHandler, Unit>::labeledItem(
    YieldHandling yieldHandling) {
  TokenKind tt;
  if (!tokenStream.getToken(&tt, TokenStream::SlashIsRegExp)) {
    return null();
  }
>>>>>>> upstream-releases

<<<<<<< HEAD
  RootedPropertyName label(context);
  if (!matchLabel(yieldHandling, &label)) {
    return null();
  }
||||||| merged common ancestors
    RootedPropertyName label(context);
    if (!matchLabel(yieldHandling, &label)) {
        return null();
    }
=======
  if (tt == TokenKind::Function) {
    TokenKind next;
    if (!tokenStream.peekToken(&next)) {
      return null();
    }
>>>>>>> upstream-releases

<<<<<<< HEAD
  auto validity = pc->checkContinueStatement(label);
  if (validity.isErr()) {
    switch (validity.unwrapErr()) {
      case ParseContext::ContinueStatementError::NotInALoop:
        errorAt(begin, JSMSG_BAD_CONTINUE);
        break;
      case ParseContext::ContinueStatementError::LabelNotFound:
        error(JSMSG_LABEL_NOT_FOUND);
        break;
||||||| merged common ancestors
    auto validity = pc->checkContinueStatement(label);
    if (validity.isErr()) {
        switch (validity.unwrapErr()) {
          case ParseContext::ContinueStatementError::NotInALoop:
            errorAt(begin, JSMSG_BAD_CONTINUE);
            break;
          case ParseContext::ContinueStatementError::LabelNotFound:
            error(JSMSG_LABEL_NOT_FOUND);
            break;
        }
        return null();
=======
    // GeneratorDeclaration is only matched by HoistableDeclaration in
    // StatementListItem, so generators can't be inside labels.
    if (next == TokenKind::Mul) {
      error(JSMSG_GENERATOR_LABEL);
      return null();
>>>>>>> upstream-releases
    }
    return null();
  }

<<<<<<< HEAD
  if (!matchOrInsertSemicolon()) {
    return null();
  }
||||||| merged common ancestors
    if (!matchOrInsertSemicolon()) {
        return null();
    }
=======
    // Per 13.13.1 it's a syntax error if LabelledItem: FunctionDeclaration
    // is ever matched.  Per Annex B.3.2 that modifies this text, this
    // applies only to strict mode code.
    if (pc_->sc()->strict()) {
      error(JSMSG_FUNCTION_LABEL);
      return null();
    }
>>>>>>> upstream-releases

<<<<<<< HEAD
  return handler.newContinueStatement(label, TokenPos(begin, pos().end));
||||||| merged common ancestors
    return handler.newContinueStatement(label, TokenPos(begin, pos().end));
=======
    return functionStmt(pos().begin, yieldHandling, NameRequired);
  }

  anyChars.ungetToken();
  return statement(yieldHandling);
>>>>>>> upstream-releases
}

template <class ParseHandler, typename Unit>
<<<<<<< HEAD
typename ParseHandler::BreakStatementType
GeneralParser<ParseHandler, Unit>::breakStatement(YieldHandling yieldHandling) {
  MOZ_ASSERT(anyChars.isCurrentTokenType(TokenKind::Break));
  uint32_t begin = pos().begin;
||||||| merged common ancestors
typename ParseHandler::BreakStatementType
GeneralParser<ParseHandler, Unit>::breakStatement(YieldHandling yieldHandling)
{
    MOZ_ASSERT(anyChars.isCurrentTokenType(TokenKind::Break));
    uint32_t begin = pos().begin;

    RootedPropertyName label(context);
    if (!matchLabel(yieldHandling, &label)) {
        return null();
    }
=======
typename ParseHandler::LabeledStatementType
GeneralParser<ParseHandler, Unit>::labeledStatement(
    YieldHandling yieldHandling) {
  RootedPropertyName label(cx_, labelIdentifier(yieldHandling));
  if (!label) {
    return null();
  }

  auto hasSameLabel = [&label](ParseContext::LabelStatement* stmt) {
    return stmt->label() == label;
  };
>>>>>>> upstream-releases

<<<<<<< HEAD
  RootedPropertyName label(context);
  if (!matchLabel(yieldHandling, &label)) {
    return null();
  }
||||||| merged common ancestors
    // Labeled 'break' statements target the nearest labeled statements (could
    // be any kind) with the same label. Unlabeled 'break' statements target
    // the innermost loop or switch statement.
    if (label) {
        auto hasSameLabel = [&label](ParseContext::LabelStatement* stmt) {
            return stmt->label() == label;
        };
=======
  uint32_t begin = pos().begin;
>>>>>>> upstream-releases

<<<<<<< HEAD
  // Labeled 'break' statements target the nearest labeled statements (could
  // be any kind) with the same label. Unlabeled 'break' statements target
  // the innermost loop or switch statement.
  if (label) {
    auto hasSameLabel = [&label](ParseContext::LabelStatement* stmt) {
      return stmt->label() == label;
    };
||||||| merged common ancestors
        if (!pc->template findInnermostStatement<ParseContext::LabelStatement>(hasSameLabel)) {
            error(JSMSG_LABEL_NOT_FOUND);
            return null();
        }
    } else {
        auto isBreakTarget = [](ParseContext::Statement* stmt) {
            return StatementKindIsUnlabeledBreakTarget(stmt->kind());
        };
=======
  if (pc_->template findInnermostStatement<ParseContext::LabelStatement>(
          hasSameLabel)) {
    errorAt(begin, JSMSG_DUPLICATE_LABEL);
    return null();
  }
>>>>>>> upstream-releases

<<<<<<< HEAD
    if (!pc->template findInnermostStatement<ParseContext::LabelStatement>(
            hasSameLabel)) {
      error(JSMSG_LABEL_NOT_FOUND);
      return null();
    }
  } else {
    auto isBreakTarget = [](ParseContext::Statement* stmt) {
      return StatementKindIsUnlabeledBreakTarget(stmt->kind());
    };
||||||| merged common ancestors
        if (!pc->findInnermostStatement(isBreakTarget)) {
            errorAt(begin, JSMSG_TOUGH_BREAK);
            return null();
        }
    }
=======
  tokenStream.consumeKnownToken(TokenKind::Colon);
>>>>>>> upstream-releases

<<<<<<< HEAD
    if (!pc->findInnermostStatement(isBreakTarget)) {
      errorAt(begin, JSMSG_TOUGH_BREAK);
      return null();
    }
  }
||||||| merged common ancestors
    if (!matchOrInsertSemicolon()) {
        return null();
    }
=======
  /* Push a label struct and parse the statement. */
  ParseContext::LabelStatement stmt(pc_, label);
  Node pn = labeledItem(yieldHandling);
  if (!pn) {
    return null();
  }
>>>>>>> upstream-releases

<<<<<<< HEAD
  if (!matchOrInsertSemicolon()) {
    return null();
  }

  return handler.newBreakStatement(label, TokenPos(begin, pos().end));
||||||| merged common ancestors
    return handler.newBreakStatement(label, TokenPos(begin, pos().end));
=======
  return handler_.newLabeledStatement(label, pn, begin);
>>>>>>> upstream-releases
}

template <class ParseHandler, typename Unit>
typename ParseHandler::UnaryNodeType
<<<<<<< HEAD
GeneralParser<ParseHandler, Unit>::returnStatement(
    YieldHandling yieldHandling) {
  MOZ_ASSERT(anyChars.isCurrentTokenType(TokenKind::Return));
  uint32_t begin = pos().begin;

  MOZ_ASSERT(pc->isFunctionBox());
  pc->functionBox()->usesReturn = true;

  // Parse an optional operand.
  //
  // This is ugly, but we don't want to require a semicolon.
  Node exprNode;
  TokenKind tt = TokenKind::Eof;
  if (!tokenStream.peekTokenSameLine(&tt, TokenStream::Operand)) {
    return null();
  }
  switch (tt) {
    case TokenKind::Eol:
    case TokenKind::Eof:
    case TokenKind::Semi:
    case TokenKind::RightCurly:
      exprNode = null();
      break;
    default: {
      exprNode = expr(InAllowed, yieldHandling, TripledotProhibited);
      if (!exprNode) {
        return null();
      }
    }
  }
||||||| merged common ancestors
GeneralParser<ParseHandler, Unit>::returnStatement(YieldHandling yieldHandling)
{
    MOZ_ASSERT(anyChars.isCurrentTokenType(TokenKind::Return));
    uint32_t begin = pos().begin;

    MOZ_ASSERT(pc->isFunctionBox());
    pc->functionBox()->usesReturn = true;

    // Parse an optional operand.
    //
    // This is ugly, but we don't want to require a semicolon.
    Node exprNode;
    TokenKind tt = TokenKind::Eof;
    if (!tokenStream.peekTokenSameLine(&tt, TokenStream::Operand)) {
        return null();
    }
    switch (tt) {
      case TokenKind::Eol:
      case TokenKind::Eof:
      case TokenKind::Semi:
      case TokenKind::RightCurly:
        exprNode = null();
        break;
      default: {
        exprNode = expr(InAllowed, yieldHandling, TripledotProhibited);
        if (!exprNode) {
            return null();
        }
      }
    }
=======
GeneralParser<ParseHandler, Unit>::throwStatement(YieldHandling yieldHandling) {
  MOZ_ASSERT(anyChars.isCurrentTokenType(TokenKind::Throw));
  uint32_t begin = pos().begin;

  /* ECMA-262 Edition 3 says 'throw [no LineTerminator here] Expr'. */
  TokenKind tt = TokenKind::Eof;
  if (!tokenStream.peekTokenSameLine(&tt, TokenStream::SlashIsRegExp)) {
    return null();
  }
  if (tt == TokenKind::Eof || tt == TokenKind::Semi ||
      tt == TokenKind::RightCurly) {
    error(JSMSG_MISSING_EXPR_AFTER_THROW);
    return null();
  }
  if (tt == TokenKind::Eol) {
    error(JSMSG_LINE_BREAK_AFTER_THROW);
    return null();
  }

  Node throwExpr = expr(InAllowed, yieldHandling, TripledotProhibited);
  if (!throwExpr) {
    return null();
  }
>>>>>>> upstream-releases

  if (!matchOrInsertSemicolon()) {
    return null();
  }

<<<<<<< HEAD
  return handler.newReturnStatement(exprNode, TokenPos(begin, pos().end));
||||||| merged common ancestors
    return handler.newReturnStatement(exprNode, TokenPos(begin, pos().end));
=======
  return handler_.newThrowStatement(throwExpr, TokenPos(begin, pos().end));
>>>>>>> upstream-releases
}

template <class ParseHandler, typename Unit>
<<<<<<< HEAD
typename ParseHandler::UnaryNodeType
GeneralParser<ParseHandler, Unit>::yieldExpression(InHandling inHandling) {
  MOZ_ASSERT(anyChars.isCurrentTokenType(TokenKind::Yield));
  uint32_t begin = pos().begin;
||||||| merged common ancestors
typename ParseHandler::UnaryNodeType
GeneralParser<ParseHandler, Unit>::yieldExpression(InHandling inHandling)
{
    MOZ_ASSERT(anyChars.isCurrentTokenType(TokenKind::Yield));
    uint32_t begin = pos().begin;
=======
typename ParseHandler::TernaryNodeType
GeneralParser<ParseHandler, Unit>::tryStatement(YieldHandling yieldHandling) {
  MOZ_ASSERT(anyChars.isCurrentTokenType(TokenKind::Try));
  uint32_t begin = pos().begin;

  /*
   * try nodes are ternary.
   * kid1 is the try statement
   * kid2 is the catch node list or null
   * kid3 is the finally statement
   *
   * catch nodes are binary.
   * left is the catch-name/pattern or null
   * right is the catch block
   *
   * catch lvalue nodes are either:
   *   a single identifier
   *   TokenKind::RightBracket for a destructuring left-hand side
   *   TokenKind::RightCurly for a destructuring left-hand side
   *
   * finally nodes are TokenKind::LeftCurly statement lists.
   */

  Node innerBlock;
  {
    if (!mustMatchToken(TokenKind::LeftCurly, JSMSG_CURLY_BEFORE_TRY)) {
      return null();
    }
>>>>>>> upstream-releases

<<<<<<< HEAD
  MOZ_ASSERT(pc->isGenerator());
  MOZ_ASSERT(pc->isFunctionBox());
||||||| merged common ancestors
    MOZ_ASSERT(pc->isGenerator());
    MOZ_ASSERT(pc->isFunctionBox());
=======
    uint32_t openedPos = pos().begin;
>>>>>>> upstream-releases

<<<<<<< HEAD
  pc->lastYieldOffset = begin;
||||||| merged common ancestors
    pc->lastYieldOffset = begin;
=======
    ParseContext::Statement stmt(pc_, StatementKind::Try);
    ParseContext::Scope scope(this);
    if (!scope.init(pc_)) {
      return null();
    }
>>>>>>> upstream-releases

<<<<<<< HEAD
  Node exprNode;
  ParseNodeKind kind = ParseNodeKind::Yield;
  TokenKind tt = TokenKind::Eof;
  if (!tokenStream.peekTokenSameLine(&tt, TokenStream::Operand)) {
    return null();
  }
  switch (tt) {
    // TokenKind::Eol is special; it implements the [no LineTerminator here]
    // quirk in the grammar.
    case TokenKind::Eol:
    // The rest of these make up the complete set of tokens that can
    // appear after any of the places where AssignmentExpression is used
    // throughout the grammar.  Conveniently, none of them can also be the
    // start an expression.
    case TokenKind::Eof:
    case TokenKind::Semi:
    case TokenKind::RightCurly:
    case TokenKind::RightBracket:
    case TokenKind::RightParen:
    case TokenKind::Colon:
    case TokenKind::Comma:
    case TokenKind::In:
      // No value.
      exprNode = null();
      anyChars.addModifierException(TokenStream::NoneIsOperand);
      break;
    case TokenKind::Mul:
      kind = ParseNodeKind::YieldStar;
      tokenStream.consumeKnownToken(TokenKind::Mul, TokenStream::Operand);
      MOZ_FALLTHROUGH;
    default:
      exprNode = assignExpr(inHandling, YieldIsKeyword, TripledotProhibited);
      if (!exprNode) {
        return null();
      }
  }
  if (kind == ParseNodeKind::YieldStar) {
    return handler.newYieldStarExpression(begin, exprNode);
  }
  return handler.newYieldExpression(begin, exprNode);
}
||||||| merged common ancestors
    Node exprNode;
    ParseNodeKind kind = ParseNodeKind::Yield;
    TokenKind tt = TokenKind::Eof;
    if (!tokenStream.peekTokenSameLine(&tt, TokenStream::Operand)) {
        return null();
    }
    switch (tt) {
      // TokenKind::Eol is special; it implements the [no LineTerminator here]
      // quirk in the grammar.
      case TokenKind::Eol:
      // The rest of these make up the complete set of tokens that can
      // appear after any of the places where AssignmentExpression is used
      // throughout the grammar.  Conveniently, none of them can also be the
      // start an expression.
      case TokenKind::Eof:
      case TokenKind::Semi:
      case TokenKind::RightCurly:
      case TokenKind::RightBracket:
      case TokenKind::RightParen:
      case TokenKind::Colon:
      case TokenKind::Comma:
      case TokenKind::In:
        // No value.
        exprNode = null();
        anyChars.addModifierException(TokenStream::NoneIsOperand);
        break;
      case TokenKind::Mul:
        kind = ParseNodeKind::YieldStar;
        tokenStream.consumeKnownToken(TokenKind::Mul, TokenStream::Operand);
        MOZ_FALLTHROUGH;
      default:
        exprNode = assignExpr(inHandling, YieldIsKeyword, TripledotProhibited);
        if (!exprNode) {
            return null();
        }
    }
    if (kind == ParseNodeKind::YieldStar) {
        return handler.newYieldStarExpression(begin, exprNode);
    }
    return handler.newYieldExpression(begin, exprNode);
}
=======
    innerBlock = statementList(yieldHandling);
    if (!innerBlock) {
      return null();
    }

    innerBlock = finishLexicalScope(scope, innerBlock);
    if (!innerBlock) {
      return null();
    }

    if (!mustMatchToken(
            TokenKind::RightCurly, [this, openedPos](TokenKind actual) {
              this->reportMissingClosing(JSMSG_CURLY_AFTER_TRY,
                                         JSMSG_CURLY_OPENED, openedPos);
            })) {
      return null();
    }
  }
>>>>>>> upstream-releases

<<<<<<< HEAD
template <class ParseHandler, typename Unit>
typename ParseHandler::BinaryNodeType
GeneralParser<ParseHandler, Unit>::withStatement(YieldHandling yieldHandling) {
  MOZ_ASSERT(anyChars.isCurrentTokenType(TokenKind::With));
  uint32_t begin = pos().begin;

  // Usually we want the constructs forbidden in strict mode code to be a
  // subset of those that ContextOptions::extraWarnings() warns about, and we
  // use strictModeError directly.  But while 'with' is forbidden in strict
  // mode code, it doesn't even merit a warning in non-strict code.  See
  // https://bugzilla.mozilla.org/show_bug.cgi?id=514576#c1.
  if (pc->sc()->strict()) {
    if (!strictModeError(JSMSG_STRICT_CODE_WITH)) {
      return null();
    }
  }

  if (!mustMatchToken(TokenKind::LeftParen, JSMSG_PAREN_BEFORE_WITH)) {
    return null();
  }
||||||| merged common ancestors
template <class ParseHandler, typename Unit>
typename ParseHandler::BinaryNodeType
GeneralParser<ParseHandler, Unit>::withStatement(YieldHandling yieldHandling)
{
    MOZ_ASSERT(anyChars.isCurrentTokenType(TokenKind::With));
    uint32_t begin = pos().begin;

    // Usually we want the constructs forbidden in strict mode code to be a
    // subset of those that ContextOptions::extraWarnings() warns about, and we
    // use strictModeError directly.  But while 'with' is forbidden in strict
    // mode code, it doesn't even merit a warning in non-strict code.  See
    // https://bugzilla.mozilla.org/show_bug.cgi?id=514576#c1.
    if (pc->sc()->strict()) {
        if (!strictModeError(JSMSG_STRICT_CODE_WITH)) {
            return null();
        }
    }

    MUST_MATCH_TOKEN(TokenKind::LeftParen, JSMSG_PAREN_BEFORE_WITH);
=======
  LexicalScopeNodeType catchScope = null();
  TokenKind tt;
  if (!tokenStream.getToken(&tt)) {
    return null();
  }
  if (tt == TokenKind::Catch) {
    /*
     * Create a lexical scope node around the whole catch clause,
     * including the head.
     */
    ParseContext::Statement stmt(pc_, StatementKind::Catch);
    ParseContext::Scope scope(this);
    if (!scope.init(pc_)) {
      return null();
    }

    /*
     * Legal catch forms are:
     *   catch (lhs) {
     *   catch {
     * where lhs is a name or a destructuring left-hand side.
     */
    bool omittedBinding;
    if (!tokenStream.matchToken(&omittedBinding, TokenKind::LeftCurly)) {
      return null();
    }

    Node catchName;
    if (omittedBinding) {
      catchName = null();
    } else {
      if (!mustMatchToken(TokenKind::LeftParen, JSMSG_PAREN_BEFORE_CATCH)) {
        return null();
      }
>>>>>>> upstream-releases

<<<<<<< HEAD
  Node objectExpr = exprInParens(InAllowed, yieldHandling, TripledotProhibited);
  if (!objectExpr) {
    return null();
  }
||||||| merged common ancestors
    Node objectExpr = exprInParens(InAllowed, yieldHandling, TripledotProhibited);
    if (!objectExpr) {
        return null();
    }
=======
      if (!tokenStream.getToken(&tt)) {
        return null();
      }
      switch (tt) {
        case TokenKind::LeftBracket:
        case TokenKind::LeftCurly:
          catchName = destructuringDeclaration(DeclarationKind::CatchParameter,
                                               yieldHandling, tt);
          if (!catchName) {
            return null();
          }
          break;
>>>>>>> upstream-releases

<<<<<<< HEAD
  if (!mustMatchToken(TokenKind::RightParen, TokenStream::Operand,
                      JSMSG_PAREN_AFTER_WITH)) {
    return null();
  }
||||||| merged common ancestors
    MUST_MATCH_TOKEN_MOD(TokenKind::RightParen, TokenStream::Operand, JSMSG_PAREN_AFTER_WITH);
=======
        default: {
          if (!TokenKindIsPossibleIdentifierName(tt)) {
            error(JSMSG_CATCH_IDENTIFIER);
            return null();
          }
>>>>>>> upstream-releases

<<<<<<< HEAD
  Node innerBlock;
  {
    ParseContext::Statement stmt(pc, StatementKind::With);
    innerBlock = statement(yieldHandling);
    if (!innerBlock) {
      return null();
    }
  }

  pc->sc()->setBindingsAccessedDynamically();
||||||| merged common ancestors
    Node innerBlock;
    {
        ParseContext::Statement stmt(pc, StatementKind::With);
        innerBlock = statement(yieldHandling);
        if (!innerBlock) {
            return null();
        }
    }

    pc->sc()->setBindingsAccessedDynamically();
=======
          catchName = bindingIdentifier(DeclarationKind::SimpleCatchParameter,
                                        yieldHandling);
          if (!catchName) {
            return null();
          }
          break;
        }
      }
>>>>>>> upstream-releases

<<<<<<< HEAD
  return handler.newWithStatement(begin, objectExpr, innerBlock);
}
||||||| merged common ancestors
    return handler.newWithStatement(begin, objectExpr, innerBlock);
}
=======
      if (!mustMatchToken(TokenKind::RightParen, JSMSG_PAREN_AFTER_CATCH)) {
        return null();
      }
>>>>>>> upstream-releases

<<<<<<< HEAD
template <class ParseHandler, typename Unit>
typename ParseHandler::Node GeneralParser<ParseHandler, Unit>::labeledItem(
    YieldHandling yieldHandling) {
  TokenKind tt;
  if (!tokenStream.getToken(&tt, TokenStream::Operand)) {
    return null();
  }
||||||| merged common ancestors
template <class ParseHandler, typename Unit>
typename ParseHandler::Node
GeneralParser<ParseHandler, Unit>::labeledItem(YieldHandling yieldHandling)
{
    TokenKind tt;
    if (!tokenStream.getToken(&tt, TokenStream::Operand)) {
        return null();
    }

    if (tt == TokenKind::Function) {
        TokenKind next;
        if (!tokenStream.peekToken(&next)) {
            return null();
        }
=======
      if (!mustMatchToken(TokenKind::LeftCurly, JSMSG_CURLY_BEFORE_CATCH)) {
        return null();
      }
    }

    LexicalScopeNodeType catchBody = catchBlockStatement(yieldHandling, scope);
    if (!catchBody) {
      return null();
    }
>>>>>>> upstream-releases

<<<<<<< HEAD
  if (tt == TokenKind::Function) {
    TokenKind next;
    if (!tokenStream.peekToken(&next)) {
      return null();
    }
||||||| merged common ancestors
        // GeneratorDeclaration is only matched by HoistableDeclaration in
        // StatementListItem, so generators can't be inside labels.
        if (next == TokenKind::Mul) {
            error(JSMSG_GENERATOR_LABEL);
            return null();
        }
=======
    catchScope = finishLexicalScope(scope, catchBody);
    if (!catchScope) {
      return null();
    }
>>>>>>> upstream-releases

<<<<<<< HEAD
    // GeneratorDeclaration is only matched by HoistableDeclaration in
    // StatementListItem, so generators can't be inside labels.
    if (next == TokenKind::Mul) {
      error(JSMSG_GENERATOR_LABEL);
      return null();
    }
||||||| merged common ancestors
        // Per 13.13.1 it's a syntax error if LabelledItem: FunctionDeclaration
        // is ever matched.  Per Annex B.3.2 that modifies this text, this
        // applies only to strict mode code.
        if (pc->sc()->strict()) {
            error(JSMSG_FUNCTION_LABEL);
            return null();
        }
=======
    if (!handler_.setupCatchScope(catchScope, catchName, catchBody)) {
      return null();
    }
    handler_.setEndPosition(catchScope, pos().end);
>>>>>>> upstream-releases

<<<<<<< HEAD
    // Per 13.13.1 it's a syntax error if LabelledItem: FunctionDeclaration
    // is ever matched.  Per Annex B.3.2 that modifies this text, this
    // applies only to strict mode code.
    if (pc->sc()->strict()) {
      error(JSMSG_FUNCTION_LABEL);
      return null();
||||||| merged common ancestors
        return functionStmt(pos().begin, yieldHandling, NameRequired);
=======
    if (!tokenStream.getToken(&tt, TokenStream::SlashIsRegExp)) {
      return null();
>>>>>>> upstream-releases
    }
  }

<<<<<<< HEAD
    return functionStmt(pos().begin, yieldHandling, NameRequired);
  }

  anyChars.ungetToken();
  return statement(yieldHandling);
}
||||||| merged common ancestors
    anyChars.ungetToken();
    return statement(yieldHandling);
}
=======
  Node finallyBlock = null();
>>>>>>> upstream-releases

<<<<<<< HEAD
template <class ParseHandler, typename Unit>
typename ParseHandler::LabeledStatementType
GeneralParser<ParseHandler, Unit>::labeledStatement(
    YieldHandling yieldHandling) {
  RootedPropertyName label(context, labelIdentifier(yieldHandling));
  if (!label) {
    return null();
  }
||||||| merged common ancestors
template <class ParseHandler, typename Unit>
typename ParseHandler::LabeledStatementType
GeneralParser<ParseHandler, Unit>::labeledStatement(YieldHandling yieldHandling)
{
    RootedPropertyName label(context, labelIdentifier(yieldHandling));
    if (!label) {
        return null();
    }
=======
  if (tt == TokenKind::Finally) {
    if (!mustMatchToken(TokenKind::LeftCurly, JSMSG_CURLY_BEFORE_FINALLY)) {
      return null();
    }
>>>>>>> upstream-releases

<<<<<<< HEAD
  auto hasSameLabel = [&label](ParseContext::LabelStatement* stmt) {
    return stmt->label() == label;
  };
||||||| merged common ancestors
    auto hasSameLabel = [&label](ParseContext::LabelStatement* stmt) {
        return stmt->label() == label;
    };
=======
    uint32_t openedPos = pos().begin;
>>>>>>> upstream-releases

<<<<<<< HEAD
  uint32_t begin = pos().begin;
||||||| merged common ancestors
    uint32_t begin = pos().begin;
=======
    ParseContext::Statement stmt(pc_, StatementKind::Finally);
    ParseContext::Scope scope(this);
    if (!scope.init(pc_)) {
      return null();
    }
>>>>>>> upstream-releases

<<<<<<< HEAD
  if (pc->template findInnermostStatement<ParseContext::LabelStatement>(
          hasSameLabel)) {
    errorAt(begin, JSMSG_DUPLICATE_LABEL);
    return null();
  }
||||||| merged common ancestors
    if (pc->template findInnermostStatement<ParseContext::LabelStatement>(hasSameLabel)) {
        errorAt(begin, JSMSG_DUPLICATE_LABEL);
        return null();
    }
=======
    finallyBlock = statementList(yieldHandling);
    if (!finallyBlock) {
      return null();
    }
>>>>>>> upstream-releases

<<<<<<< HEAD
  tokenStream.consumeKnownToken(TokenKind::Colon);
||||||| merged common ancestors
    tokenStream.consumeKnownToken(TokenKind::Colon);
=======
    finallyBlock = finishLexicalScope(scope, finallyBlock);
    if (!finallyBlock) {
      return null();
    }
>>>>>>> upstream-releases

<<<<<<< HEAD
  /* Push a label struct and parse the statement. */
  ParseContext::LabelStatement stmt(pc, label);
  Node pn = labeledItem(yieldHandling);
  if (!pn) {
    return null();
  }
||||||| merged common ancestors
    /* Push a label struct and parse the statement. */
    ParseContext::LabelStatement stmt(pc, label);
    Node pn = labeledItem(yieldHandling);
    if (!pn) {
        return null();
    }
=======
    if (!mustMatchToken(
            TokenKind::RightCurly, [this, openedPos](TokenKind actual) {
              this->reportMissingClosing(JSMSG_CURLY_AFTER_FINALLY,
                                         JSMSG_CURLY_OPENED, openedPos);
            })) {
      return null();
    }
  } else {
    anyChars.ungetToken();
  }
  if (!catchScope && !finallyBlock) {
    error(JSMSG_CATCH_OR_FINALLY);
    return null();
  }
>>>>>>> upstream-releases

<<<<<<< HEAD
  return handler.newLabeledStatement(label, pn, begin);
||||||| merged common ancestors
    return handler.newLabeledStatement(label, pn, begin);
=======
  return handler_.newTryStatement(begin, innerBlock, catchScope, finallyBlock);
>>>>>>> upstream-releases
}

template <class ParseHandler, typename Unit>
<<<<<<< HEAD
typename ParseHandler::UnaryNodeType
GeneralParser<ParseHandler, Unit>::throwStatement(YieldHandling yieldHandling) {
  MOZ_ASSERT(anyChars.isCurrentTokenType(TokenKind::Throw));
  uint32_t begin = pos().begin;
||||||| merged common ancestors
typename ParseHandler::UnaryNodeType
GeneralParser<ParseHandler, Unit>::throwStatement(YieldHandling yieldHandling)
{
    MOZ_ASSERT(anyChars.isCurrentTokenType(TokenKind::Throw));
    uint32_t begin = pos().begin;
=======
typename ParseHandler::LexicalScopeNodeType
GeneralParser<ParseHandler, Unit>::catchBlockStatement(
    YieldHandling yieldHandling, ParseContext::Scope& catchParamScope) {
  uint32_t openedPos = pos().begin;

  ParseContext::Statement stmt(pc_, StatementKind::Block);

  // ES 13.15.7 CatchClauseEvaluation
  //
  // Step 8 means that the body of a catch block always has an additional
  // lexical scope.
  ParseContext::Scope scope(this);
  if (!scope.init(pc_)) {
    return null();
  }
>>>>>>> upstream-releases

<<<<<<< HEAD
  /* ECMA-262 Edition 3 says 'throw [no LineTerminator here] Expr'. */
  TokenKind tt = TokenKind::Eof;
  if (!tokenStream.peekTokenSameLine(&tt, TokenStream::Operand)) {
    return null();
  }
  if (tt == TokenKind::Eof || tt == TokenKind::Semi ||
      tt == TokenKind::RightCurly) {
    error(JSMSG_MISSING_EXPR_AFTER_THROW);
    return null();
  }
  if (tt == TokenKind::Eol) {
    error(JSMSG_LINE_BREAK_AFTER_THROW);
    return null();
  }
||||||| merged common ancestors
    /* ECMA-262 Edition 3 says 'throw [no LineTerminator here] Expr'. */
    TokenKind tt = TokenKind::Eof;
    if (!tokenStream.peekTokenSameLine(&tt, TokenStream::Operand)) {
        return null();
    }
    if (tt == TokenKind::Eof || tt == TokenKind::Semi || tt == TokenKind::RightCurly) {
        error(JSMSG_MISSING_EXPR_AFTER_THROW);
        return null();
    }
    if (tt == TokenKind::Eol) {
        error(JSMSG_LINE_BREAK_AFTER_THROW);
        return null();
    }
=======
  // The catch parameter names cannot be redeclared inside the catch
  // block, so declare the name in the inner scope.
  if (!scope.addCatchParameters(pc_, catchParamScope)) {
    return null();
  }
>>>>>>> upstream-releases

<<<<<<< HEAD
  Node throwExpr = expr(InAllowed, yieldHandling, TripledotProhibited);
  if (!throwExpr) {
    return null();
  }
||||||| merged common ancestors
    Node throwExpr = expr(InAllowed, yieldHandling, TripledotProhibited);
    if (!throwExpr) {
        return null();
    }
=======
  ListNodeType list = statementList(yieldHandling);
  if (!list) {
    return null();
  }
>>>>>>> upstream-releases

<<<<<<< HEAD
  if (!matchOrInsertSemicolon()) {
    return null();
  }
||||||| merged common ancestors
    if (!matchOrInsertSemicolon()) {
        return null();
    }
=======
  if (!mustMatchToken(
          TokenKind::RightCurly, [this, openedPos](TokenKind actual) {
            this->reportMissingClosing(JSMSG_CURLY_AFTER_CATCH,
                                       JSMSG_CURLY_OPENED, openedPos);
          })) {
    return null();
  }
>>>>>>> upstream-releases

<<<<<<< HEAD
  return handler.newThrowStatement(throwExpr, TokenPos(begin, pos().end));
||||||| merged common ancestors
    return handler.newThrowStatement(throwExpr, TokenPos(begin, pos().end));
=======
  // The catch parameter names are not bound in the body scope, so remove
  // them before generating bindings.
  scope.removeCatchParameters(pc_, catchParamScope);
  return finishLexicalScope(scope, list);
>>>>>>> upstream-releases
}

template <class ParseHandler, typename Unit>
<<<<<<< HEAD
typename ParseHandler::TernaryNodeType
GeneralParser<ParseHandler, Unit>::tryStatement(YieldHandling yieldHandling) {
  MOZ_ASSERT(anyChars.isCurrentTokenType(TokenKind::Try));
  uint32_t begin = pos().begin;

  /*
   * try nodes are ternary.
   * kid1 is the try statement
   * kid2 is the catch node list or null
   * kid3 is the finally statement
   *
   * catch nodes are binary.
   * left is the catch-name/pattern or null
   * right is the catch block
   *
   * catch lvalue nodes are either:
   *   a single identifier
   *   TokenKind::RightBracket for a destructuring left-hand side
   *   TokenKind::RightCurly for a destructuring left-hand side
   *
   * finally nodes are TokenKind::LeftCurly statement lists.
   */

  Node innerBlock;
  {
    if (!mustMatchToken(TokenKind::LeftCurly, JSMSG_CURLY_BEFORE_TRY)) {
      return null();
    }
||||||| merged common ancestors
typename ParseHandler::TernaryNodeType
GeneralParser<ParseHandler, Unit>::tryStatement(YieldHandling yieldHandling)
{
    MOZ_ASSERT(anyChars.isCurrentTokenType(TokenKind::Try));
    uint32_t begin = pos().begin;
=======
typename ParseHandler::DebuggerStatementType
GeneralParser<ParseHandler, Unit>::debuggerStatement() {
  TokenPos p;
  p.begin = pos().begin;
  if (!matchOrInsertSemicolon()) {
    return null();
  }
  p.end = pos().end;

  return handler_.newDebuggerStatement(p);
}

static AccessorType ToAccessorType(PropertyType propType) {
  switch (propType) {
    case PropertyType::Getter:
      return AccessorType::Getter;
    case PropertyType::Setter:
      return AccessorType::Setter;
    case PropertyType::Normal:
    case PropertyType::Method:
    case PropertyType::GeneratorMethod:
    case PropertyType::AsyncMethod:
    case PropertyType::AsyncGeneratorMethod:
    case PropertyType::Constructor:
    case PropertyType::DerivedConstructor:
      return AccessorType::None;
    default:
      MOZ_CRASH("unexpected property type");
  }
}

template <class ParseHandler, typename Unit>
bool GeneralParser<ParseHandler, Unit>::classMember(
    YieldHandling yieldHandling, DefaultHandling defaultHandling,
    const ParseContext::ClassStatement& classStmt, HandlePropertyName className,
    uint32_t classStartOffset, HasHeritage hasHeritage, size_t& numFields,
    size_t& numFieldKeys, ListNodeType& classMembers, bool* done) {
  *done = false;

  TokenKind tt;
  if (!tokenStream.getToken(&tt, TokenStream::SlashIsInvalid)) {
    return false;
  }
  if (tt == TokenKind::RightCurly) {
    *done = true;
    return true;
  }
>>>>>>> upstream-releases

<<<<<<< HEAD
    uint32_t openedPos = pos().begin;
||||||| merged common ancestors
    /*
     * try nodes are ternary.
     * kid1 is the try statement
     * kid2 is the catch node list or null
     * kid3 is the finally statement
     *
     * catch nodes are binary.
     * left is the catch-name/pattern or null
     * right is the catch block
     *
     * catch lvalue nodes are either:
     *   a single identifier
     *   TokenKind::RightBracket or TokenKind::RightCurly for a destructuring left-hand side
     *
     * finally nodes are TokenKind::LeftCurly statement lists.
     */
=======
  if (tt == TokenKind::Semi) {
    return true;
  }
>>>>>>> upstream-releases

<<<<<<< HEAD
    ParseContext::Statement stmt(pc, StatementKind::Try);
    ParseContext::Scope scope(this);
    if (!scope.init(pc)) {
      return null();
    }
||||||| merged common ancestors
    Node innerBlock;
    {
        MUST_MATCH_TOKEN(TokenKind::LeftCurly, JSMSG_CURLY_BEFORE_TRY);
=======
  bool isStatic = false;
  if (tt == TokenKind::Static) {
    if (!tokenStream.peekToken(&tt)) {
      return false;
    }
    if (tt == TokenKind::RightCurly) {
      tokenStream.consumeKnownToken(tt);
      error(JSMSG_UNEXPECTED_TOKEN, "property name", TokenKindToDesc(tt));
      return false;
    }
>>>>>>> upstream-releases

<<<<<<< HEAD
    innerBlock = statementList(yieldHandling);
    if (!innerBlock) {
      return null();
    }
||||||| merged common ancestors
        uint32_t openedPos = pos().begin;
=======
    if (tt != TokenKind::LeftParen) {
      isStatic = true;
    } else {
      anyChars.ungetToken();
    }
  } else {
    anyChars.ungetToken();
  }
>>>>>>> upstream-releases

<<<<<<< HEAD
    innerBlock = finishLexicalScope(scope, innerBlock);
    if (!innerBlock) {
      return null();
    }
||||||| merged common ancestors
        ParseContext::Statement stmt(pc, StatementKind::Try);
        ParseContext::Scope scope(this);
        if (!scope.init(pc)) {
            return null();
        }
=======
  uint32_t propNameOffset;
  if (!tokenStream.peekOffset(&propNameOffset, TokenStream::SlashIsInvalid)) {
    return false;
  }

  RootedAtom propAtom(cx_);
  PropertyType propType;
  Node propName = propertyOrMethodName(yieldHandling, PropertyNameInClass,
                                       /* maybeDecl = */ Nothing(),
                                       classMembers, &propType, &propAtom);
  if (!propName) {
    return false;
  }
>>>>>>> upstream-releases

<<<<<<< HEAD
    if (!mustMatchToken(TokenKind::RightCurly, TokenStream::Operand,
                        [this, openedPos](TokenKind actual) {
                          this->reportMissingClosing(JSMSG_CURLY_AFTER_TRY,
                                                     JSMSG_CURLY_OPENED,
                                                     openedPos);
                        })) {
      return null();
    }
  }
||||||| merged common ancestors
        innerBlock = statementList(yieldHandling);
        if (!innerBlock) {
            return null();
        }
=======
  if (propType == PropertyType::Field) {
    if (!options().fieldsEnabledOption) {
      errorAt(propNameOffset, JSMSG_FIELDS_NOT_SUPPORTED);
      return false;
    }
>>>>>>> upstream-releases

<<<<<<< HEAD
  LexicalScopeNodeType catchScope = null();
  TokenKind tt;
  if (!tokenStream.getToken(&tt)) {
    return null();
  }
  if (tt == TokenKind::Catch) {
    /*
     * Create a lexical scope node around the whole catch clause,
     * including the head.
     */
    ParseContext::Statement stmt(pc, StatementKind::Catch);
    ParseContext::Scope scope(this);
    if (!scope.init(pc)) {
      return null();
    }
||||||| merged common ancestors
        innerBlock = finishLexicalScope(scope, innerBlock);
        if (!innerBlock) {
            return null();
        }
=======
    if (isStatic) {
      errorAt(propNameOffset, JSMSG_BAD_METHOD_DEF);
      return false;
    }
>>>>>>> upstream-releases

<<<<<<< HEAD
    /*
     * Legal catch forms are:
     *   catch (lhs) {
     *   catch {
     * where lhs is a name or a destructuring left-hand side.
     */
    bool omittedBinding;
    if (!tokenStream.matchToken(&omittedBinding, TokenKind::LeftCurly)) {
      return null();
||||||| merged common ancestors
        MUST_MATCH_TOKEN_MOD_WITH_REPORT(TokenKind::RightCurly, TokenStream::Operand,
                                         reportMissingClosing(JSMSG_CURLY_AFTER_TRY,
                                                              JSMSG_CURLY_OPENED, openedPos));
=======
    if (propAtom == cx_->names().constructor) {
      errorAt(propNameOffset, JSMSG_BAD_METHOD_DEF);
      return false;
>>>>>>> upstream-releases
    }

<<<<<<< HEAD
    Node catchName;
    if (omittedBinding) {
      catchName = null();
    } else {
      if (!mustMatchToken(TokenKind::LeftParen, JSMSG_PAREN_BEFORE_CATCH)) {
        return null();
      }
||||||| merged common ancestors
    LexicalScopeNodeType catchScope = null();
    TokenKind tt;
    if (!tokenStream.getToken(&tt)) {
        return null();
    }
    if (tt == TokenKind::Catch) {
        /*
         * Create a lexical scope node around the whole catch clause,
         * including the head.
         */
        ParseContext::Statement stmt(pc, StatementKind::Catch);
        ParseContext::Scope scope(this);
        if (!scope.init(pc)) {
            return null();
        }
=======
    if (!abortIfSyntaxParser()) {
      return false;
    }
>>>>>>> upstream-releases

<<<<<<< HEAD
      if (!tokenStream.getToken(&tt)) {
        return null();
      }
      switch (tt) {
        case TokenKind::LeftBracket:
        case TokenKind::LeftCurly:
          catchName = destructuringDeclaration(DeclarationKind::CatchParameter,
                                               yieldHandling, tt);
          if (!catchName) {
            return null();
          }
          break;
||||||| merged common ancestors
        /*
         * Legal catch forms are:
         *   catch (lhs) {
         *   catch {
         * where lhs is a name or a destructuring left-hand side.
         */
        bool omittedBinding;
        if (!tokenStream.matchToken(&omittedBinding, TokenKind::LeftCurly)) {
            return null();
        }

        Node catchName;
        if (omittedBinding) {
            catchName = null();
        } else {
            MUST_MATCH_TOKEN(TokenKind::LeftParen, JSMSG_PAREN_BEFORE_CATCH);

            if (!tokenStream.getToken(&tt)) {
                return null();
            }
            switch (tt) {
              case TokenKind::LeftBracket:
              case TokenKind::LeftCurly:
                catchName = destructuringDeclaration(DeclarationKind::CatchParameter,
                                                     yieldHandling, tt);
                if (!catchName) {
                    return null();
                }
                break;

              default: {
                if (!TokenKindIsPossibleIdentifierName(tt)) {
                    error(JSMSG_CATCH_IDENTIFIER);
                    return null();
                }

                catchName = bindingIdentifier(DeclarationKind::SimpleCatchParameter,
                                              yieldHandling);
                if (!catchName) {
                    return null();
                }
                break;
              }
            }

            MUST_MATCH_TOKEN_MOD(TokenKind::RightParen, TokenStream::Operand, JSMSG_PAREN_AFTER_CATCH);

            MUST_MATCH_TOKEN(TokenKind::LeftCurly, JSMSG_CURLY_BEFORE_CATCH);
        }
=======
    numFields++;

    FunctionNodeType initializer = fieldInitializerOpt(
        yieldHandling, hasHeritage, propName, propAtom, numFieldKeys);
    if (!initializer) {
      return false;
    }

    if (!matchOrInsertSemicolon(TokenStream::SlashIsInvalid)) {
      return false;
    }

    ClassFieldType field =
        handler_.newClassFieldDefinition(propName, initializer);
    if (!field) {
      return false;
    }

    return handler_.addClassMemberDefinition(classMembers, field);
  }

  if (propType != PropertyType::Getter && propType != PropertyType::Setter &&
      propType != PropertyType::Method &&
      propType != PropertyType::GeneratorMethod &&
      propType != PropertyType::AsyncMethod &&
      propType != PropertyType::AsyncGeneratorMethod) {
    errorAt(propNameOffset, JSMSG_BAD_METHOD_DEF);
    return false;
  }

  bool isConstructor = !isStatic && propAtom == cx_->names().constructor;
  if (isConstructor) {
    if (propType != PropertyType::Method) {
      errorAt(propNameOffset, JSMSG_BAD_METHOD_DEF);
      return false;
    }
    if (classStmt.constructorBox) {
      errorAt(propNameOffset, JSMSG_DUPLICATE_PROPERTY, "constructor");
      return false;
    }
    propType = hasHeritage == HasHeritage::Yes
                   ? PropertyType::DerivedConstructor
                   : PropertyType::Constructor;
  } else if (isStatic && propAtom == cx_->names().prototype) {
    errorAt(propNameOffset, JSMSG_BAD_METHOD_DEF);
    return false;
  }

  RootedAtom funName(cx_);
  switch (propType) {
    case PropertyType::Getter:
    case PropertyType::Setter:
      if (!anyChars.isCurrentTokenType(TokenKind::RightBracket)) {
        funName = prefixAccessorName(propType, propAtom);
        if (!funName) {
          return false;
        }
      }
      break;
    case PropertyType::Constructor:
    case PropertyType::DerivedConstructor:
      funName = className;
      break;
    default:
      if (!anyChars.isCurrentTokenType(TokenKind::RightBracket)) {
        funName = propAtom;
      }
  }

  // .fieldKeys must be declared outside the scope .initializers is declared in,
  // hence this extra scope.
  Maybe<ParseContext::Scope> dotInitializersScope;
  if (isConstructor && !options().selfHostingMode) {
    dotInitializersScope.emplace(this);
    if (!dotInitializersScope->init(pc_)) {
      return false;
    }

    if (!noteDeclaredName(cx_->names().dotInitializers, DeclarationKind::Let,
                          pos())) {
      return false;
    }
  }

  // Calling toString on constructors need to return the source text for
  // the entire class. The end offset is unknown at this point in
  // parsing and will be amended when class parsing finishes below.
  FunctionNodeType funNode = methodDefinition(
      isConstructor ? classStartOffset : propNameOffset, propType, funName);
  if (!funNode) {
    return false;
  }

  AccessorType atype = ToAccessorType(propType);
>>>>>>> upstream-releases

<<<<<<< HEAD
        default: {
          if (!TokenKindIsPossibleIdentifierName(tt)) {
            error(JSMSG_CATCH_IDENTIFIER);
            return null();
          }
||||||| merged common ancestors
        LexicalScopeNodeType catchBody = catchBlockStatement(yieldHandling, scope);
        if (!catchBody) {
            return null();
        }
=======
  Node method =
      handler_.newClassMethodDefinition(propName, funNode, atype, isStatic);
  if (!method) {
    return false;
  }
>>>>>>> upstream-releases

<<<<<<< HEAD
          catchName = bindingIdentifier(DeclarationKind::SimpleCatchParameter,
                                        yieldHandling);
          if (!catchName) {
            return null();
          }
          break;
        }
      }
||||||| merged common ancestors
        catchScope = finishLexicalScope(scope, catchBody);
        if (!catchScope) {
            return null();
        }
=======
  if (dotInitializersScope.isSome()) {
    method = finishLexicalScope(*dotInitializersScope, method);
    if (!method) {
      return false;
    }
    dotInitializersScope.reset();
  }
>>>>>>> upstream-releases

<<<<<<< HEAD
      if (!mustMatchToken(TokenKind::RightParen, TokenStream::Operand,
                          JSMSG_PAREN_AFTER_CATCH)) {
        return null();
      }
||||||| merged common ancestors
        if (!handler.setupCatchScope(catchScope, catchName, catchBody)) {
            return null();
        }
        handler.setEndPosition(catchScope, pos().end);
=======
  return handler_.addClassMemberDefinition(classMembers, method);
}
>>>>>>> upstream-releases

<<<<<<< HEAD
      if (!mustMatchToken(TokenKind::LeftCurly, JSMSG_CURLY_BEFORE_CATCH)) {
        return null();
      }
||||||| merged common ancestors
        if (!tokenStream.getToken(&tt, TokenStream::Operand)) {
            return null();
        }
=======
template <class ParseHandler, typename Unit>
bool GeneralParser<ParseHandler, Unit>::finishClassConstructor(
    const ParseContext::ClassStatement& classStmt, HandlePropertyName className,
    HasHeritage hasHeritage, uint32_t classStartOffset, uint32_t classEndOffset,
    size_t numFields, ListNodeType& classMembers) {
  // Fields cannot re-use the constructor obtained via JSOP_CLASSCONSTRUCTOR or
  // JSOP_DERIVEDCONSTRUCTOR due to needing to emit calls to the field
  // initializers in the constructor. So, synthesize a new one.
  if (classStmt.constructorBox == nullptr && numFields > 0) {
    MOZ_ASSERT(!options().selfHostingMode);
    // Unconditionally create the scope here, because it's always the
    // constructor.
    ParseContext::Scope dotInitializersScope(this);
    if (!dotInitializersScope.init(pc_)) {
      return false;
>>>>>>> upstream-releases
    }

<<<<<<< HEAD
    LexicalScopeNodeType catchBody = catchBlockStatement(yieldHandling, scope);
    if (!catchBody) {
      return null();
    }
||||||| merged common ancestors
    Node finallyBlock = null();

    if (tt == TokenKind::Finally) {
        MUST_MATCH_TOKEN(TokenKind::LeftCurly, JSMSG_CURLY_BEFORE_FINALLY);
=======
    if (!noteDeclaredName(cx_->names().dotInitializers, DeclarationKind::Let,
                          pos())) {
      return false;
    }

    // synthesizeConstructor assigns to classStmt.constructorBox
    FunctionNodeType synthesizedCtor =
        synthesizeConstructor(className, classStartOffset, hasHeritage);
    if (!synthesizedCtor) {
      return false;
    }
>>>>>>> upstream-releases

<<<<<<< HEAD
    catchScope = finishLexicalScope(scope, catchBody);
    if (!catchScope) {
      return null();
    }
||||||| merged common ancestors
        uint32_t openedPos = pos().begin;
=======
    MOZ_ASSERT(classStmt.constructorBox != nullptr);
>>>>>>> upstream-releases

<<<<<<< HEAD
    if (!handler.setupCatchScope(catchScope, catchName, catchBody)) {
      return null();
    }
    handler.setEndPosition(catchScope, pos().end);
||||||| merged common ancestors
        ParseContext::Statement stmt(pc, StatementKind::Finally);
        ParseContext::Scope scope(this);
        if (!scope.init(pc)) {
            return null();
        }
=======
    // Note: the *function* has the name of the class, but the *property*
    // containing the function has the name "constructor"
    Node constructorNameNode =
        handler_.newObjectLiteralPropertyName(cx_->names().constructor, pos());
    if (!constructorNameNode) {
      return false;
    }
    ClassMethodType method = handler_.newClassMethodDefinition(
        constructorNameNode, synthesizedCtor, AccessorType::None,
        /* isStatic = */ false);
    if (!method) {
      return false;
    }
    LexicalScopeNodeType scope =
        finishLexicalScope(dotInitializersScope, method);
    if (!scope) {
      return false;
    }
    if (!handler_.addClassMemberDefinition(classMembers, scope)) {
      return false;
    }
  }
>>>>>>> upstream-releases

<<<<<<< HEAD
    if (!tokenStream.getToken(&tt, TokenStream::Operand)) {
      return null();
    }
  }
||||||| merged common ancestors
        finallyBlock = statementList(yieldHandling);
        if (!finallyBlock) {
            return null();
        }
=======
  if (FunctionBox* ctorbox = classStmt.constructorBox) {
    // Amend the toStringEnd offset for the constructor now that we've
    // finished parsing the class.
    ctorbox->toStringEnd = classEndOffset;
>>>>>>> upstream-releases

<<<<<<< HEAD
  Node finallyBlock = null();
||||||| merged common ancestors
        finallyBlock = finishLexicalScope(scope, finallyBlock);
        if (!finallyBlock) {
            return null();
        }
=======
    if (numFields > 0) {
      // Field initialization need access to `this`.
      ctorbox->setHasThisBinding();
    }
>>>>>>> upstream-releases

<<<<<<< HEAD
  if (tt == TokenKind::Finally) {
    if (!mustMatchToken(TokenKind::LeftCurly, JSMSG_CURLY_BEFORE_FINALLY)) {
      return null();
||||||| merged common ancestors
        MUST_MATCH_TOKEN_MOD_WITH_REPORT(TokenKind::RightCurly, TokenStream::Operand,
                                         reportMissingClosing(JSMSG_CURLY_AFTER_FINALLY,
                                                              JSMSG_CURLY_OPENED, openedPos));
    } else {
        anyChars.ungetToken();
    }
    if (!catchScope && !finallyBlock) {
        error(JSMSG_CATCH_OR_FINALLY);
        return null();
=======
    // Set the same information, but on the lazyScript.
    if (ctorbox->function()->isInterpretedLazy()) {
      ctorbox->function()->lazyScript()->setToStringEnd(classEndOffset);

      if (numFields > 0) {
        ctorbox->function()->lazyScript()->setHasThisBinding();
      }
    }

    if (numFields == 0) {
      handler_.deleteConstructorScope(cx_, classMembers);
>>>>>>> upstream-releases
    }
  }

<<<<<<< HEAD
    uint32_t openedPos = pos().begin;
||||||| merged common ancestors
    return handler.newTryStatement(begin, innerBlock, catchScope, finallyBlock);
}

template <class ParseHandler, typename Unit>
typename ParseHandler::LexicalScopeNodeType
GeneralParser<ParseHandler, Unit>::catchBlockStatement(YieldHandling yieldHandling,
                                                       ParseContext::Scope& catchParamScope)
{
    uint32_t openedPos = pos().begin;
=======
  return true;
}

template <class ParseHandler, typename Unit>
typename ParseHandler::ClassNodeType
GeneralParser<ParseHandler, Unit>::classDefinition(
    YieldHandling yieldHandling, ClassContext classContext,
    DefaultHandling defaultHandling) {
  MOZ_ASSERT(anyChars.isCurrentTokenType(TokenKind::Class));
>>>>>>> upstream-releases

<<<<<<< HEAD
    ParseContext::Statement stmt(pc, StatementKind::Finally);
    ParseContext::Scope scope(this);
    if (!scope.init(pc)) {
      return null();
||||||| merged common ancestors
    ParseContext::Statement stmt(pc, StatementKind::Block);

    // ES 13.15.7 CatchClauseEvaluation
    //
    // Step 8 means that the body of a catch block always has an additional
    // lexical scope.
    ParseContext::Scope scope(this);
    if (!scope.init(pc)) {
        return null();
=======
  uint32_t classStartOffset = pos().begin;
  bool savedStrictness = setLocalStrictMode(true);

  TokenKind tt;
  if (!tokenStream.getToken(&tt)) {
    return null();
  }

  RootedPropertyName className(cx_);
  if (TokenKindIsPossibleIdentifier(tt)) {
    className = bindingIdentifier(yieldHandling);
    if (!className) {
      return null();
    }
  } else if (classContext == ClassStatement) {
    if (defaultHandling == AllowDefaultName) {
      className = cx_->names().default_;
      anyChars.ungetToken();
    } else {
      // Class statements must have a bound name
      error(JSMSG_UNNAMED_CLASS_STMT);
      return null();
    }
  } else {
    // Make sure to put it back, whatever it was
    anyChars.ungetToken();
  }

  // Because the binding definitions keep track of their blockId, we need to
  // create at least the inner binding later. Keep track of the name's
  // position in order to provide it for the nodes created later.
  TokenPos namePos = pos();

  // Push a ParseContext::ClassStatement to keep track of the constructor
  // funbox.
  ParseContext::ClassStatement classStmt(pc_);

  NameNodeType innerName;
  Node nameNode = null();
  Node classHeritage = null();
  LexicalScopeNodeType classBlock = null();
  uint32_t classEndOffset;
  {
    // A named class creates a new lexical scope with a const binding of the
    // class name for the "inner name".
    ParseContext::Statement innerScopeStmt(pc_, StatementKind::Block);
    ParseContext::Scope innerScope(this);
    if (!innerScope.init(pc_)) {
      return null();
    }

    bool hasHeritageBool;
    if (!tokenStream.matchToken(&hasHeritageBool, TokenKind::Extends)) {
      return null();
    }
    HasHeritage hasHeritage =
        hasHeritageBool ? HasHeritage::Yes : HasHeritage::No;
    if (hasHeritage == HasHeritage::Yes) {
      if (!tokenStream.getToken(&tt)) {
        return null();
      }
      classHeritage = memberExpr(yieldHandling, TripledotProhibited, tt);
      if (!classHeritage) {
        return null();
      }
    }

    if (!mustMatchToken(TokenKind::LeftCurly, JSMSG_CURLY_BEFORE_CLASS)) {
      return null();
    }

    ListNodeType classMembers = handler_.newClassMemberList(pos().begin);
    if (!classMembers) {
      return null();
>>>>>>> upstream-releases
    }

<<<<<<< HEAD
    finallyBlock = statementList(yieldHandling);
    if (!finallyBlock) {
      return null();
    }

    finallyBlock = finishLexicalScope(scope, finallyBlock);
    if (!finallyBlock) {
      return null();
    }

    if (!mustMatchToken(TokenKind::RightCurly, TokenStream::Operand,
                        [this, openedPos](TokenKind actual) {
                          this->reportMissingClosing(JSMSG_CURLY_AFTER_FINALLY,
                                                     JSMSG_CURLY_OPENED,
                                                     openedPos);
                        })) {
      return null();
||||||| merged common ancestors
    // The catch parameter names cannot be redeclared inside the catch
    // block, so declare the name in the inner scope.
    if (!scope.addCatchParameters(pc, catchParamScope)) {
        return null();
=======
    size_t numFields = 0;
    size_t numFieldKeys = 0;
    for (;;) {
      bool done;
      if (!classMember(yieldHandling, defaultHandling, classStmt, className,
                       classStartOffset, hasHeritage, numFields, numFieldKeys,
                       classMembers, &done)) {
        return null();
      }
      if (done) {
        break;
      }
>>>>>>> upstream-releases
    }
  } else {
    anyChars.ungetToken();
  }
  if (!catchScope && !finallyBlock) {
    error(JSMSG_CATCH_OR_FINALLY);
    return null();
  }

  return handler.newTryStatement(begin, innerBlock, catchScope, finallyBlock);
}

template <class ParseHandler, typename Unit>
typename ParseHandler::LexicalScopeNodeType
GeneralParser<ParseHandler, Unit>::catchBlockStatement(
    YieldHandling yieldHandling, ParseContext::Scope& catchParamScope) {
  uint32_t openedPos = pos().begin;

  ParseContext::Statement stmt(pc, StatementKind::Block);

  // ES 13.15.7 CatchClauseEvaluation
  //
  // Step 8 means that the body of a catch block always has an additional
  // lexical scope.
  ParseContext::Scope scope(this);
  if (!scope.init(pc)) {
    return null();
  }

<<<<<<< HEAD
  // The catch parameter names cannot be redeclared inside the catch
  // block, so declare the name in the inner scope.
  if (!scope.addCatchParameters(pc, catchParamScope)) {
    return null();
  }
||||||| merged common ancestors
    ListNodeType list = statementList(yieldHandling);
    if (!list) {
        return null();
    }
=======
    if (numFieldKeys > 0) {
      if (!noteDeclaredName(cx_->names().dotFieldKeys, DeclarationKind::Let,
                            namePos)) {
        return null();
      }
    }
>>>>>>> upstream-releases

<<<<<<< HEAD
  ListNodeType list = statementList(yieldHandling);
  if (!list) {
    return null();
  }

  if (!mustMatchToken(TokenKind::RightCurly, TokenStream::Operand,
                      [this, openedPos](TokenKind actual) {
                        this->reportMissingClosing(JSMSG_CURLY_AFTER_CATCH,
                                                   JSMSG_CURLY_OPENED,
                                                   openedPos);
                      })) {
    return null();
  }
||||||| merged common ancestors
    MUST_MATCH_TOKEN_MOD_WITH_REPORT(TokenKind::RightCurly, TokenStream::Operand,
                                     reportMissingClosing(JSMSG_CURLY_AFTER_CATCH,
                                                          JSMSG_CURLY_OPENED, openedPos));
=======
    classEndOffset = pos().end;
    if (!finishClassConstructor(classStmt, className, hasHeritage,
                                classStartOffset, classEndOffset, numFields,
                                classMembers)) {
      return null();
    }
>>>>>>> upstream-releases

<<<<<<< HEAD
  // The catch parameter names are not bound in the body scope, so remove
  // them before generating bindings.
  scope.removeCatchParameters(pc, catchParamScope);
  return finishLexicalScope(scope, list);
}
||||||| merged common ancestors
    // The catch parameter names are not bound in the body scope, so remove
    // them before generating bindings.
    scope.removeCatchParameters(pc, catchParamScope);
    return finishLexicalScope(scope, list);
}
=======
    if (className) {
      // The inner name is immutable.
      if (!noteDeclaredName(className, DeclarationKind::Const, namePos)) {
        return null();
      }
>>>>>>> upstream-releases

<<<<<<< HEAD
template <class ParseHandler, typename Unit>
typename ParseHandler::DebuggerStatementType
GeneralParser<ParseHandler, Unit>::debuggerStatement() {
  TokenPos p;
  p.begin = pos().begin;
  if (!matchOrInsertSemicolon()) {
    return null();
  }
  p.end = pos().end;

  pc->sc()->setBindingsAccessedDynamically();
  pc->sc()->setHasDebuggerStatement();

  return handler.newDebuggerStatement(p);
}

static AccessorType ToAccessorType(PropertyType propType) {
  switch (propType) {
    case PropertyType::Getter:
      return AccessorType::Getter;
    case PropertyType::Setter:
      return AccessorType::Setter;
    case PropertyType::Normal:
    case PropertyType::Method:
    case PropertyType::GeneratorMethod:
    case PropertyType::AsyncMethod:
    case PropertyType::AsyncGeneratorMethod:
    case PropertyType::Constructor:
    case PropertyType::DerivedConstructor:
      return AccessorType::None;
    default:
      MOZ_CRASH("unexpected property type");
  }
}
||||||| merged common ancestors
template <class ParseHandler, typename Unit>
typename ParseHandler::DebuggerStatementType
GeneralParser<ParseHandler, Unit>::debuggerStatement()
{
    TokenPos p;
    p.begin = pos().begin;
    if (!matchOrInsertSemicolon()) {
        return null();
    }
    p.end = pos().end;

    pc->sc()->setBindingsAccessedDynamically();
    pc->sc()->setHasDebuggerStatement();

    return handler.newDebuggerStatement(p);
}

static AccessorType
ToAccessorType(PropertyType propType)
{
    switch (propType) {
      case PropertyType::Getter:
        return AccessorType::Getter;
      case PropertyType::Setter:
        return AccessorType::Setter;
      case PropertyType::Normal:
      case PropertyType::Method:
      case PropertyType::GeneratorMethod:
      case PropertyType::AsyncMethod:
      case PropertyType::AsyncGeneratorMethod:
      case PropertyType::Constructor:
      case PropertyType::DerivedConstructor:
        return AccessorType::None;
      default:
        MOZ_CRASH("unexpected property type");
    }
}
=======
      innerName = newName(className, namePos);
      if (!innerName) {
        return null();
      }
    }

    classBlock = finishLexicalScope(innerScope, classMembers);
    if (!classBlock) {
      return null();
    }
>>>>>>> upstream-releases

<<<<<<< HEAD
template <class ParseHandler, typename Unit>
typename ParseHandler::ClassNodeType
GeneralParser<ParseHandler, Unit>::classDefinition(
    YieldHandling yieldHandling, ClassContext classContext,
    DefaultHandling defaultHandling) {
  MOZ_ASSERT(anyChars.isCurrentTokenType(TokenKind::Class));
||||||| merged common ancestors
template <class ParseHandler, typename Unit>
typename ParseHandler::ClassNodeType
GeneralParser<ParseHandler, Unit>::classDefinition(YieldHandling yieldHandling,
                                                   ClassContext classContext,
                                                   DefaultHandling defaultHandling)
{
    MOZ_ASSERT(anyChars.isCurrentTokenType(TokenKind::Class));
=======
    // Pop the inner scope.
  }
>>>>>>> upstream-releases

<<<<<<< HEAD
  uint32_t classStartOffset = pos().begin;
  bool savedStrictness = setLocalStrictMode(true);
||||||| merged common ancestors
    uint32_t classStartOffset = pos().begin;
    bool savedStrictness = setLocalStrictMode(true);
=======
  if (className) {
    NameNodeType outerName = null();
    if (classContext == ClassStatement) {
      // The outer name is mutable.
      if (!noteDeclaredName(className, DeclarationKind::Class, namePos)) {
        return null();
      }
>>>>>>> upstream-releases

<<<<<<< HEAD
  TokenKind tt;
  if (!tokenStream.getToken(&tt)) {
    return null();
  }
||||||| merged common ancestors
    TokenKind tt;
    if (!tokenStream.getToken(&tt)) {
        return null();
    }
=======
      outerName = newName(className, namePos);
      if (!outerName) {
        return null();
      }
    }
>>>>>>> upstream-releases

<<<<<<< HEAD
  RootedPropertyName name(context);
  if (TokenKindIsPossibleIdentifier(tt)) {
    name = bindingIdentifier(yieldHandling);
    if (!name) {
      return null();
    }
  } else if (classContext == ClassStatement) {
    if (defaultHandling == AllowDefaultName) {
      name = context->names().default_;
      anyChars.ungetToken();
    } else {
      // Class statements must have a bound name
      error(JSMSG_UNNAMED_CLASS_STMT);
      return null();
    }
  } else {
    // Make sure to put it back, whatever it was
    anyChars.ungetToken();
  }

  // Push a ParseContext::ClassStatement to keep track of the constructor
  // funbox.
  ParseContext::ClassStatement classStmt(pc);

  RootedAtom propAtom(context);

  // A named class creates a new lexical scope with a const binding of the
  // class name for the "inner name".
  Maybe<ParseContext::Statement> innerScopeStmt;
  Maybe<ParseContext::Scope> innerScope;
  if (name) {
    innerScopeStmt.emplace(pc, StatementKind::Block);
    innerScope.emplace(this);
    if (!innerScope->init(pc)) {
      return null();
    }
  }

  // Because the binding definitions keep track of their blockId, we need to
  // create at least the inner binding later. Keep track of the name's position
  // in order to provide it for the nodes created later.
  TokenPos namePos = pos();

  Node classHeritage = null();
  bool hasHeritage;
  if (!tokenStream.matchToken(&hasHeritage, TokenKind::Extends)) {
    return null();
  }
  if (hasHeritage) {
    if (!tokenStream.getToken(&tt)) {
      return null();
    }
    classHeritage = memberExpr(yieldHandling, TripledotProhibited, tt);
    if (!classHeritage) {
      return null();
||||||| merged common ancestors
    RootedPropertyName name(context);
    if (TokenKindIsPossibleIdentifier(tt)) {
        name = bindingIdentifier(yieldHandling);
        if (!name) {
            return null();
        }
    } else if (classContext == ClassStatement) {
        if (defaultHandling == AllowDefaultName) {
            name = context->names().default_;
            anyChars.ungetToken();
        } else {
            // Class statements must have a bound name
            error(JSMSG_UNNAMED_CLASS_STMT);
            return null();
        }
    } else {
        // Make sure to put it back, whatever it was
        anyChars.ungetToken();
=======
    nameNode = handler_.newClassNames(outerName, innerName, namePos);
    if (!nameNode) {
      return null();
>>>>>>> upstream-releases
    }
  }

<<<<<<< HEAD
  if (!mustMatchToken(TokenKind::LeftCurly, JSMSG_CURLY_BEFORE_CLASS)) {
    return null();
  }
||||||| merged common ancestors
    // Push a ParseContext::ClassStatement to keep track of the constructor
    // funbox.
    ParseContext::ClassStatement classStmt(pc);
=======
  MOZ_ALWAYS_TRUE(setLocalStrictMode(savedStrictness));
>>>>>>> upstream-releases

<<<<<<< HEAD
  ListNodeType classMembers = handler.newClassMemberList(pos().begin);
  if (!classMembers) {
    return null();
  }
||||||| merged common ancestors
    RootedAtom propAtom(context);
=======
  return handler_.newClass(nameNode, classHeritage, classBlock,
                           TokenPos(classStartOffset, classEndOffset));
}
>>>>>>> upstream-releases

<<<<<<< HEAD
  Maybe<DeclarationKind> declKind = Nothing();
  for (;;) {
    TokenKind tt;
    if (!tokenStream.getToken(&tt)) {
      return null();
    }
    if (tt == TokenKind::RightCurly) {
      break;
||||||| merged common ancestors
    // A named class creates a new lexical scope with a const binding of the
    // class name for the "inner name".
    Maybe<ParseContext::Statement> innerScopeStmt;
    Maybe<ParseContext::Scope> innerScope;
    if (name) {
        innerScopeStmt.emplace(pc, StatementKind::Block);
        innerScope.emplace(this);
        if (!innerScope->init(pc)) {
            return null();
        }
=======
template <class ParseHandler, typename Unit>
typename ParseHandler::FunctionNodeType
GeneralParser<ParseHandler, Unit>::synthesizeConstructor(
    HandleAtom className, uint32_t classNameOffset, HasHeritage hasHeritage) {
  FunctionSyntaxKind functionSyntaxKind =
      hasHeritage == HasHeritage::Yes
          ? FunctionSyntaxKind::DerivedClassConstructor
          : FunctionSyntaxKind::ClassConstructor;

  // Create the function object.
  RootedFunction fun(cx_, newFunction(className, functionSyntaxKind,
                                      GeneratorKind::NotGenerator,
                                      FunctionAsyncKind::SyncFunction));
  if (!fun) {
    return null();
  }

  // Create the top-level field initializer node.
  FunctionNodeType funNode = handler_.newFunction(functionSyntaxKind, pos());
  if (!funNode) {
    return null();
  }

  // Create the FunctionBox and link it to the function object.
  Directives directives(true);
  FunctionBox* funbox = newFunctionBox(funNode, fun, classNameOffset,
                                       directives, GeneratorKind::NotGenerator,
                                       FunctionAsyncKind::SyncFunction);
  if (!funbox) {
    return null();
  }
  funbox->initWithEnclosingParseContext(pc_, fun, functionSyntaxKind);
  handler_.setFunctionBox(funNode, funbox);
  setFunctionEndFromCurrentToken(funbox);

  // Push a SourceParseContext on to the stack.
  SourceParseContext funpc(this, funbox, /* newDirectives = */ nullptr);
  if (!funpc.init()) {
    return null();
  }

  TokenPos synthesizedBodyPos = TokenPos(classNameOffset, classNameOffset + 1);
  // Create a ListNode for the parameters + body (there are no parameters).
  ListNodeType argsbody =
      handler_.newList(ParseNodeKind::ParamsBody, synthesizedBodyPos);
  if (!argsbody) {
    return null();
  }
  handler_.setFunctionFormalParametersAndBody(funNode, argsbody);
  setFunctionStartAtCurrentToken(funbox);

  if (hasHeritage == HasHeritage::Yes) {
    // Synthesize the equivalent to `function f(...args)`
    funbox->setHasRest();
    if (!notePositionalFormalParameter(funNode, cx_->names().args,
                                       synthesizedBodyPos.begin,
                                       /* disallowDuplicateParams = */ false,
                                       /* duplicatedParam = */ nullptr)) {
      return null();
    }
    funbox->function()->setArgCount(1);
  } else {
    funbox->function()->setArgCount(0);
  }

  pc_->functionScope().useAsVarScope(pc_);

  auto stmtList = handler_.newStatementList(synthesizedBodyPos);
  if (!stmtList) {
    return null();
  }

  if (!noteUsedName(cx_->names().dotThis)) {
    return null();
  }

  if (!noteUsedName(cx_->names().dotInitializers)) {
    return null();
  }

  bool canSkipLazyClosedOverBindings = handler_.canSkipLazyClosedOverBindings();
  if (!pc_->declareFunctionThis(usedNames_, canSkipLazyClosedOverBindings)) {
    return null();
  }

  if (hasHeritage == HasHeritage::Yes) {
    NameNodeType thisName = newThisName();
    if (!thisName) {
      return null();
    }

    UnaryNodeType superBase =
        handler_.newSuperBase(thisName, synthesizedBodyPos);
    if (!superBase) {
      return null();
>>>>>>> upstream-releases
    }

<<<<<<< HEAD
    if (tt == TokenKind::Semi) {
      continue;
    }
||||||| merged common ancestors
    // Because the binding definitions keep track of their blockId, we need to
    // create at least the inner binding later. Keep track of the name's position
    // in order to provide it for the nodes created later.
    TokenPos namePos = pos();
=======
    ListNodeType arguments = handler_.newArguments(synthesizedBodyPos);
    if (!arguments) {
      return null();
    }
>>>>>>> upstream-releases

<<<<<<< HEAD
    bool isStatic = false;
    if (tt == TokenKind::Static) {
      if (!tokenStream.peekToken(&tt)) {
        return null();
      }
      if (tt == TokenKind::RightCurly) {
        tokenStream.consumeKnownToken(tt);
        error(JSMSG_UNEXPECTED_TOKEN, "property name", TokenKindToDesc(tt));
        return null();
      }

      if (tt != TokenKind::LeftParen) {
        isStatic = true;
      } else {
        anyChars.ungetToken();
      }
    } else {
      anyChars.ungetToken();
||||||| merged common ancestors
    Node classHeritage = null();
    bool hasHeritage;
    if (!tokenStream.matchToken(&hasHeritage, TokenKind::Extends)) {
        return null();
    }
    if (hasHeritage) {
        if (!tokenStream.getToken(&tt)) {
            return null();
        }
        classHeritage = memberExpr(yieldHandling, TripledotProhibited, tt);
        if (!classHeritage) {
            return null();
        }
=======
    NameNodeType argsNameNode = newName(cx_->names().args, synthesizedBodyPos);
    if (!argsNameNode) {
      return null();
    }
    if (!noteUsedName(cx_->names().args)) {
      return null();
>>>>>>> upstream-releases
    }

<<<<<<< HEAD
    uint32_t nameOffset;
    if (!tokenStream.peekOffset(&nameOffset)) {
      return null();
    }
||||||| merged common ancestors
    MUST_MATCH_TOKEN(TokenKind::LeftCurly, JSMSG_CURLY_BEFORE_CLASS);
=======
    UnaryNodeType spreadArgs =
        handler_.newSpread(synthesizedBodyPos.begin, argsNameNode);
    if (!spreadArgs) {
      return null();
    }
    handler_.addList(arguments, spreadArgs);
>>>>>>> upstream-releases

<<<<<<< HEAD
    PropertyType propType;
    Node propName = propertyName(yieldHandling, PropertyNameInClass, declKind,
                                 classMembers, &propType, &propAtom);
    if (!propName) {
      return null();
||||||| merged common ancestors
    ListNodeType classMethods = handler.newClassMethodList(pos().begin);
    if (!classMethods) {
        return null();
=======
    CallNodeType superCall =
        handler_.newSuperCall(superBase, arguments, /* isSpread = */ true);
    if (!superCall) {
      return null();
>>>>>>> upstream-releases
    }

<<<<<<< HEAD
    if (propType == PropertyType::Field) {
      if (isStatic) {
        errorAt(nameOffset, JSMSG_BAD_METHOD_DEF);
        return null();
      }
      if (!tokenStream.getToken(&tt)) {
        return null();
      }
      Node initializer = null();
      if (tt == TokenKind::Assign) {
        initializer = assignExpr(InAllowed, yieldHandling, TripledotProhibited);
        if (!initializer) {
          return null();
        }
        if (!tokenStream.getToken(&tt)) {
          return null();
        }
      }
||||||| merged common ancestors
    Maybe<DeclarationKind> declKind = Nothing();
    for (;;) {
        TokenKind tt;
        if (!tokenStream.getToken(&tt)) {
            return null();
        }
        if (tt == TokenKind::RightCurly) {
            break;
        }

        if (tt == TokenKind::Semi) {
            continue;
        }
=======
    BinaryNodeType setThis = handler_.newSetThis(thisName, superCall);
    if (!setThis) {
      return null();
    }

    UnaryNodeType exprStatement =
        handler_.newExprStatement(setThis, synthesizedBodyPos.end);
    if (!exprStatement) {
      return null();
    }
>>>>>>> upstream-releases

<<<<<<< HEAD
      // TODO(khyperia): Implement ASI
      if (tt != TokenKind::Semi) {
        error(JSMSG_MISSING_SEMI_FIELD);
        return null();
      }
||||||| merged common ancestors
        bool isStatic = false;
        if (tt == TokenKind::Static) {
            if (!tokenStream.peekToken(&tt)) {
                return null();
            }
            if (tt == TokenKind::RightCurly) {
                tokenStream.consumeKnownToken(tt);
                error(JSMSG_UNEXPECTED_TOKEN, "property name", TokenKindToDesc(tt));
                return null();
            }
=======
    handler_.addStatementToList(stmtList, exprStatement);
  }
>>>>>>> upstream-releases

<<<<<<< HEAD
      if (!handler.addClassFieldDefinition(classMembers, propName,
                                           initializer)) {
        return null();
      }
||||||| merged common ancestors
            if (tt != TokenKind::LeftParen) {
                isStatic = true;
            } else {
                anyChars.ungetToken();
            }
        } else {
            anyChars.ungetToken();
        }
=======
  auto initializerBody = finishLexicalScope(pc_->varScope(), stmtList);
  if (!initializerBody) {
    return null();
  }
  handler_.setBeginPosition(initializerBody, stmtList);
  handler_.setEndPosition(initializerBody, stmtList);
>>>>>>> upstream-releases

<<<<<<< HEAD
      // TODO(khyperia): Change the below to `continue;` once fields are
      // fully supported in the backend. We can't fail in BytecodeCompiler
      // because of lazy parsing.
      errorAt(nameOffset, JSMSG_FIELDS_NOT_SUPPORTED);
      return null();
    }
||||||| merged common ancestors
        uint32_t nameOffset;
        if (!tokenStream.peekOffset(&nameOffset)) {
            return null();
        }
=======
  handler_.setFunctionBody(funNode, initializerBody);
>>>>>>> upstream-releases

<<<<<<< HEAD
    if (propType != PropertyType::Getter && propType != PropertyType::Setter &&
        propType != PropertyType::Method &&
        propType != PropertyType::GeneratorMethod &&
        propType != PropertyType::AsyncMethod &&
        propType != PropertyType::AsyncGeneratorMethod) {
      errorAt(nameOffset, JSMSG_BAD_METHOD_DEF);
      return null();
    }
||||||| merged common ancestors
        PropertyType propType;
        Node propName = propertyName(yieldHandling, declKind, classMethods, &propType, &propAtom);
        if (!propName) {
            return null();
        }
=======
  if (!finishFunction()) {
    return null();
  }
>>>>>>> upstream-releases

<<<<<<< HEAD
    bool isConstructor = !isStatic && propAtom == context->names().constructor;
    if (isConstructor) {
      if (propType != PropertyType::Method) {
        errorAt(nameOffset, JSMSG_BAD_METHOD_DEF);
        return null();
      }
      if (classStmt.constructorBox) {
        errorAt(nameOffset, JSMSG_DUPLICATE_PROPERTY, "constructor");
        return null();
      }
      propType = hasHeritage ? PropertyType::DerivedConstructor
                             : PropertyType::Constructor;
    } else if (isStatic && propAtom == context->names().prototype) {
      errorAt(nameOffset, JSMSG_BAD_METHOD_DEF);
      return null();
    }
||||||| merged common ancestors
        if (propType != PropertyType::Getter && propType != PropertyType::Setter &&
            propType != PropertyType::Method && propType != PropertyType::GeneratorMethod &&
            propType != PropertyType::AsyncMethod &&
            propType != PropertyType::AsyncGeneratorMethod)
        {
            errorAt(nameOffset, JSMSG_BAD_METHOD_DEF);
            return null();
        }
=======
  // This function is asserted to set classStmt->constructorBox - however, it's
  // not directly set in this function, but rather in
  // initWithEnclosingParseContext.
>>>>>>> upstream-releases

<<<<<<< HEAD
    RootedAtom funName(context);
    switch (propType) {
      case PropertyType::Getter:
      case PropertyType::Setter:
        if (!anyChars.isCurrentTokenType(TokenKind::RightBracket)) {
          funName = prefixAccessorName(propType, propAtom);
          if (!funName) {
            return null();
          }
        }
        break;
      case PropertyType::Constructor:
      case PropertyType::DerivedConstructor:
        funName = name;
        break;
      default:
        if (!anyChars.isCurrentTokenType(TokenKind::RightBracket)) {
          funName = propAtom;
        }
    }
||||||| merged common ancestors
        bool isConstructor = !isStatic && propAtom == context->names().constructor;
        if (isConstructor) {
            if (propType != PropertyType::Method) {
                errorAt(nameOffset, JSMSG_BAD_METHOD_DEF);
                return null();
            }
            if (classStmt.constructorBox) {
                errorAt(nameOffset, JSMSG_DUPLICATE_PROPERTY, "constructor");
                return null();
            }
            propType = hasHeritage ? PropertyType::DerivedConstructor : PropertyType::Constructor;
        } else if (isStatic && propAtom == context->names().prototype) {
            errorAt(nameOffset, JSMSG_BAD_METHOD_DEF);
            return null();
        }

        RootedAtom funName(context);
        switch (propType) {
          case PropertyType::Getter:
          case PropertyType::Setter:
            if (!anyChars.isCurrentTokenType(TokenKind::RightBracket)) {
                funName = prefixAccessorName(propType, propAtom);
                if (!funName) {
                    return null();
                }
            }
            break;
          case PropertyType::Constructor:
          case PropertyType::DerivedConstructor:
            funName = name;
            break;
          default:
            if (!anyChars.isCurrentTokenType(TokenKind::RightBracket)) {
                funName = propAtom;
            }
        }
=======
  return funNode;
}

template <class ParseHandler, typename Unit>
typename ParseHandler::FunctionNodeType
GeneralParser<ParseHandler, Unit>::fieldInitializerOpt(
    YieldHandling yieldHandling, HasHeritage hasHeritage, Node propName,
    HandleAtom propAtom, size_t& numFieldKeys) {
  bool hasInitializer = false;
  if (!tokenStream.matchToken(&hasInitializer, TokenKind::Assign,
                              TokenStream::SlashIsDiv)) {
    return null();
  }

  TokenPos firstTokenPos;
  if (hasInitializer) {
    firstTokenPos = pos();
  } else {
    // the location of the "initializer" should be a zero-width span:
    // class C {
    //   x /* here */ ;
    // }
    uint32_t endPos = pos().end;
    firstTokenPos = TokenPos(endPos, endPos);
  }

  // Create the function object.
  RootedFunction fun(cx_, newFunction(nullptr, FunctionSyntaxKind::Method,
                                      GeneratorKind::NotGenerator,
                                      FunctionAsyncKind::SyncFunction));
  if (!fun) {
    return null();
  }
>>>>>>> upstream-releases

<<<<<<< HEAD
    // Calling toString on constructors need to return the source text for
    // the entire class. The end offset is unknown at this point in
    // parsing and will be amended when class parsing finishes below.
    CodeNodeType funNode = methodDefinition(
        isConstructor ? classStartOffset : nameOffset, propType, funName);
    if (!funNode) {
      return null();
    }
||||||| merged common ancestors
        // Calling toString on constructors need to return the source text for
        // the entire class. The end offset is unknown at this point in
        // parsing and will be amended when class parsing finishes below.
        CodeNodeType funNode = methodDefinition(isConstructor ? classStartOffset : nameOffset,
                                                propType, funName);
        if (!funNode) {
            return null();
        }
=======
  // Create the top-level field initializer node.
  FunctionNodeType funNode =
      handler_.newFunction(FunctionSyntaxKind::Method, firstTokenPos);
  if (!funNode) {
    return null();
  }

  // Create the FunctionBox and link it to the function object.
  Directives directives(true);
  FunctionBox* funbox = newFunctionBox(funNode, fun, firstTokenPos.begin,
                                       directives, GeneratorKind::NotGenerator,
                                       FunctionAsyncKind::SyncFunction);
  if (!funbox) {
    return null();
  }
  funbox->initFieldInitializer(pc_, fun, hasHeritage);
  handler_.setFunctionBox(funNode, funbox);

  // We can't use setFunctionStartAtCurrentToken because that uses pos().begin,
  // which is incorrect for fields without initializers (pos() points to the
  // field identifier)
  uint32_t firstTokenLine, firstTokenColumn;
  tokenStream.computeLineAndColumn(firstTokenPos.begin, &firstTokenLine,
                                   &firstTokenColumn);

  funbox->setStart(firstTokenPos.begin, firstTokenLine, firstTokenColumn);

  // Push a SourceParseContext on to the stack.
  SourceParseContext funpc(this, funbox, /* newDirectives = */ nullptr);
  if (!funpc.init()) {
    return null();
  }
>>>>>>> upstream-releases

<<<<<<< HEAD
    AccessorType atype = ToAccessorType(propType);
    if (!handler.addClassMethodDefinition(classMembers, propName, funNode,
                                          atype, isStatic)) {
      return null();
||||||| merged common ancestors
        AccessorType atype = ToAccessorType(propType);
        if (!handler.addClassMethodDefinition(classMethods, propName, funNode, atype, isStatic)) {
            return null();
        }
=======
  pc_->functionScope().useAsVarScope(pc_);

  Node initializerExpr;
  TokenPos wholeInitializerPos;
  if (hasInitializer) {
    // Parse the expression for the field initializer.
    initializerExpr = assignExpr(InAllowed, yieldHandling, TripledotProhibited);
    if (!initializerExpr) {
      return null();
>>>>>>> upstream-releases
    }
  }

<<<<<<< HEAD
  // Amend the toStringEnd offset for the constructor now that we've
  // finished parsing the class.
  uint32_t classEndOffset = pos().end;
  if (FunctionBox* ctorbox = classStmt.constructorBox) {
    if (ctorbox->function()->isInterpretedLazy()) {
      ctorbox->function()->lazyScript()->setToStringEnd(classEndOffset);
||||||| merged common ancestors
    // Amend the toStringEnd offset for the constructor now that we've
    // finished parsing the class.
    uint32_t classEndOffset = pos().end;
    if (FunctionBox* ctorbox = classStmt.constructorBox) {
        if (ctorbox->function()->isInterpretedLazy()) {
            ctorbox->function()->lazyScript()->setToStringEnd(classEndOffset);
        }
        ctorbox->toStringEnd = classEndOffset;
=======
    handler_.checkAndSetIsDirectRHSAnonFunction(initializerExpr);

    wholeInitializerPos = pos();
    wholeInitializerPos.begin = firstTokenPos.begin;
  } else {
    initializerExpr = handler_.newRawUndefinedLiteral(firstTokenPos);
    if (!initializerExpr) {
      return null();
>>>>>>> upstream-releases
    }
<<<<<<< HEAD
    ctorbox->toStringEnd = classEndOffset;
  }
||||||| merged common ancestors
=======
    wholeInitializerPos = firstTokenPos;
  }
>>>>>>> upstream-releases

<<<<<<< HEAD
  Node nameNode = null();
  Node membersOrBlock = classMembers;
  if (name) {
    // The inner name is immutable.
    if (!noteDeclaredName(name, DeclarationKind::Const, namePos)) {
      return null();
    }
||||||| merged common ancestors
    Node nameNode = null();
    Node methodsOrBlock = classMethods;
    if (name) {
        // The inner name is immutable.
        if (!noteDeclaredName(name, DeclarationKind::Const, namePos)) {
            return null();
        }
=======
  // Update the end position of the parse node.
  handler_.setEndPosition(funNode, wholeInitializerPos.end);
  setFunctionEndFromCurrentToken(funbox);
>>>>>>> upstream-releases

<<<<<<< HEAD
    NameNodeType innerName = newName(name, namePos);
    if (!innerName) {
      return null();
    }
||||||| merged common ancestors
        NameNodeType innerName = newName(name, namePos);
        if (!innerName) {
            return null();
        }
=======
  // Create a ListNode for the parameters + body (there are no parameters).
  ListNodeType argsbody =
      handler_.newList(ParseNodeKind::ParamsBody, wholeInitializerPos);
  if (!argsbody) {
    return null();
  }
  handler_.setFunctionFormalParametersAndBody(funNode, argsbody);
  funbox->function()->setArgCount(0);
>>>>>>> upstream-releases

<<<<<<< HEAD
    Node classBlock = finishLexicalScope(*innerScope, classMembers);
    if (!classBlock) {
      return null();
    }
||||||| merged common ancestors
        Node classBlock = finishLexicalScope(*innerScope, classMethods);
        if (!classBlock) {
            return null();
        }
=======
  funbox->usesThis = true;
  NameNodeType thisName = newThisName();
  if (!thisName) {
    return null();
  }
>>>>>>> upstream-releases

<<<<<<< HEAD
    membersOrBlock = classBlock;
||||||| merged common ancestors
        methodsOrBlock = classBlock;
=======
  // Build `this.field` expression.
  ThisLiteralType propAssignThis =
      handler_.newThisLiteral(wholeInitializerPos, thisName);
  if (!propAssignThis) {
    return null();
  }
>>>>>>> upstream-releases

<<<<<<< HEAD
    // Pop the inner scope.
    innerScope.reset();
    innerScopeStmt.reset();
||||||| merged common ancestors
        // Pop the inner scope.
        innerScope.reset();
        innerScopeStmt.reset();
=======
  Node propAssignFieldAccess;
  uint32_t indexValue;
  if (!propAtom) {
    // See BytecodeEmitter::emitCreateFieldKeys for an explanation of what
    // .fieldKeys means and its purpose.
    Node dotFieldKeys = newInternalDotName(cx_->names().dotFieldKeys);
    if (!dotFieldKeys) {
      return null();
    }
>>>>>>> upstream-releases

<<<<<<< HEAD
    NameNodeType outerName = null();
    if (classContext == ClassStatement) {
      // The outer name is mutable.
      if (!noteDeclaredName(name, DeclarationKind::Class, namePos)) {
        return null();
      }
||||||| merged common ancestors
        NameNodeType outerName = null();
        if (classContext == ClassStatement) {
            // The outer name is mutable.
            if (!noteDeclaredName(name, DeclarationKind::Class, namePos)) {
                return null();
            }
=======
    double fieldKeyIndex = numFieldKeys;
    numFieldKeys++;
    Node fieldKeyIndexNode = handler_.newNumber(
        fieldKeyIndex, DecimalPoint::NoDecimal, wholeInitializerPos);
    if (!fieldKeyIndexNode) {
      return null();
    }
>>>>>>> upstream-releases

<<<<<<< HEAD
      outerName = newName(name, namePos);
      if (!outerName) {
        return null();
      }
    }
||||||| merged common ancestors
            outerName = newName(name, namePos);
            if (!outerName) {
                return null();
            }
        }
=======
    Node fieldKeyValue = handler_.newPropertyByValue(
        dotFieldKeys, fieldKeyIndexNode, wholeInitializerPos.end);
    if (!fieldKeyValue) {
      return null();
    }
>>>>>>> upstream-releases

<<<<<<< HEAD
    nameNode = handler.newClassNames(outerName, innerName, namePos);
    if (!nameNode) {
      return null();
||||||| merged common ancestors
        nameNode = handler.newClassNames(outerName, innerName, namePos);
        if (!nameNode) {
            return null();
        }
=======
    propAssignFieldAccess = handler_.newPropertyByValue(
        propAssignThis, fieldKeyValue, wholeInitializerPos.end);
    if (!propAssignFieldAccess) {
      return null();
    }
  } else if (propAtom->isIndex(&indexValue)) {
    propAssignFieldAccess = handler_.newPropertyByValue(
        propAssignThis, propName, wholeInitializerPos.end);
    if (!propAssignFieldAccess) {
      return null();
    }
  } else {
    NameNodeType propAssignName = handler_.newPropertyName(
        propAtom->asPropertyName(), wholeInitializerPos);
    if (!propAssignName) {
      return null();
>>>>>>> upstream-releases
    }
  }

<<<<<<< HEAD
  MOZ_ALWAYS_TRUE(setLocalStrictMode(savedStrictness));
||||||| merged common ancestors
    MOZ_ALWAYS_TRUE(setLocalStrictMode(savedStrictness));
=======
    propAssignFieldAccess =
        handler_.newPropertyAccess(propAssignThis, propAssignName);
    if (!propAssignFieldAccess) {
      return null();
    }
  }

  // Synthesize an property init.
  AssignmentNodeType initializerPropInit = handler_.newAssignment(
      ParseNodeKind::InitExpr, propAssignFieldAccess, initializerExpr);
  if (!initializerPropInit) {
    return null();
  }

  bool canSkipLazyClosedOverBindings = handler_.canSkipLazyClosedOverBindings();
  if (!pc_->declareFunctionThis(usedNames_, canSkipLazyClosedOverBindings)) {
    return null();
  }

  UnaryNodeType exprStatement =
      handler_.newExprStatement(initializerPropInit, wholeInitializerPos.end);
  if (!exprStatement) {
    return null();
  }

  ListNodeType statementList = handler_.newStatementList(wholeInitializerPos);
  if (!statementList) {
    return null();
  }
  handler_.addStatementToList(statementList, exprStatement);

  // Set the function's body to the field assignment.
  LexicalScopeNodeType initializerBody =
      finishLexicalScope(pc_->varScope(), statementList);
  if (!initializerBody) {
    return null();
  }

  handler_.setFunctionBody(funNode, initializerBody);
>>>>>>> upstream-releases

<<<<<<< HEAD
  return handler.newClass(nameNode, classHeritage, membersOrBlock,
                          TokenPos(classStartOffset, classEndOffset));
||||||| merged common ancestors
    return handler.newClass(nameNode, classHeritage, methodsOrBlock,
                            TokenPos(classStartOffset, classEndOffset));
=======
  if (pc_->superScopeNeedsHomeObject()) {
    funbox->setNeedsHomeObject();
  }

  if (!finishFunction(/* isStandaloneFunction = */ false,
                      IsFieldInitializer::Yes)) {
    return null();
  }

  return funNode;
>>>>>>> upstream-releases
}

bool ParserBase::nextTokenContinuesLetDeclaration(TokenKind next) {
  MOZ_ASSERT(anyChars.isCurrentTokenType(TokenKind::Let));
  MOZ_ASSERT(anyChars.nextToken().type == next);

<<<<<<< HEAD
  TokenStreamShared::verifyConsistentModifier(TokenStreamShared::None,
                                              anyChars.nextToken());
||||||| merged common ancestors
    TokenStreamShared::verifyConsistentModifier(TokenStreamShared::None, anyChars.nextToken());
=======
  TokenStreamShared::verifyConsistentModifier(TokenStreamShared::SlashIsDiv,
                                              anyChars.nextToken());
>>>>>>> upstream-releases

  // Destructuring continues a let declaration.
  if (next == TokenKind::LeftBracket || next == TokenKind::LeftCurly) {
    return true;
  }

  // A "let" edge case deserves special comment.  Consider this:
  //
  //   let     // not an ASI opportunity
  //   let;
  //
  // Static semantics in §13.3.1.1 turn a LexicalDeclaration that binds
  // "let" into an early error.  Does this retroactively permit ASI so
  // that we should parse this as two ExpressionStatements?   No.  ASI
  // resolves during parsing.  Static semantics only apply to the full
  // parse tree with ASI applied.  No backsies!

  // Otherwise a let declaration must have a name.
  return TokenKindIsPossibleIdentifier(next);
}

template <class ParseHandler, typename Unit>
<<<<<<< HEAD
typename ParseHandler::Node
GeneralParser<ParseHandler, Unit>::variableStatement(
    YieldHandling yieldHandling) {
  ListNodeType vars = declarationList(yieldHandling, ParseNodeKind::Var);
  if (!vars) {
    return null();
  }
  if (!matchOrInsertSemicolon()) {
    return null();
  }
  return vars;
||||||| merged common ancestors
typename ParseHandler::Node
GeneralParser<ParseHandler, Unit>::variableStatement(YieldHandling yieldHandling)
{
    ListNodeType vars = declarationList(yieldHandling, ParseNodeKind::Var);
    if (!vars) {
        return null();
    }
    if (!matchOrInsertSemicolon()) {
        return null();
    }
    return vars;
=======
typename ParseHandler::ListNodeType
GeneralParser<ParseHandler, Unit>::variableStatement(
    YieldHandling yieldHandling) {
  ListNodeType vars = declarationList(yieldHandling, ParseNodeKind::VarStmt);
  if (!vars) {
    return null();
  }
  if (!matchOrInsertSemicolon()) {
    return null();
  }
  return vars;
>>>>>>> upstream-releases
}

template <class ParseHandler, typename Unit>
<<<<<<< HEAD
typename ParseHandler::Node GeneralParser<ParseHandler, Unit>::statement(
    YieldHandling yieldHandling) {
  MOZ_ASSERT(checkOptionsCalled);
||||||| merged common ancestors
typename ParseHandler::Node
GeneralParser<ParseHandler, Unit>::statement(YieldHandling yieldHandling)
{
    MOZ_ASSERT(checkOptionsCalled);
=======
typename ParseHandler::Node GeneralParser<ParseHandler, Unit>::statement(
    YieldHandling yieldHandling) {
  MOZ_ASSERT(checkOptionsCalled_);
>>>>>>> upstream-releases

<<<<<<< HEAD
  if (!CheckRecursionLimit(context)) {
    return null();
  }
||||||| merged common ancestors
    if (!CheckRecursionLimit(context)) {
        return null();
    }
=======
  if (!CheckRecursionLimit(cx_)) {
    return null();
  }
>>>>>>> upstream-releases

<<<<<<< HEAD
  TokenKind tt;
  if (!tokenStream.getToken(&tt, TokenStream::Operand)) {
    return null();
  }
||||||| merged common ancestors
    TokenKind tt;
    if (!tokenStream.getToken(&tt, TokenStream::Operand)) {
        return null();
    }
=======
  TokenKind tt;
  if (!tokenStream.getToken(&tt, TokenStream::SlashIsRegExp)) {
    return null();
  }

  switch (tt) {
    // BlockStatement[?Yield, ?Return]
    case TokenKind::LeftCurly:
      return blockStatement(yieldHandling);

    // VariableStatement[?Yield]
    case TokenKind::Var:
      return variableStatement(yieldHandling);

    // EmptyStatement
    case TokenKind::Semi:
      return handler_.newEmptyStatement(pos());

      // ExpressionStatement[?Yield].

    case TokenKind::Yield: {
      // Don't use a ternary operator here due to obscure linker issues
      // around using static consts in the arms of a ternary.
      Modifier modifier;
      if (yieldExpressionsSupported()) {
        modifier = TokenStream::SlashIsRegExp;
      } else {
        modifier = TokenStream::SlashIsDiv;
      }

      TokenKind next;
      if (!tokenStream.peekToken(&next, modifier)) {
        return null();
      }

      if (next == TokenKind::Colon) {
        return labeledStatement(yieldHandling);
      }

      return expressionStatement(yieldHandling);
    }
>>>>>>> upstream-releases

<<<<<<< HEAD
  switch (tt) {
    // BlockStatement[?Yield, ?Return]
    case TokenKind::LeftCurly:
      return blockStatement(yieldHandling);
||||||| merged common ancestors
    switch (tt) {
      // BlockStatement[?Yield, ?Return]
      case TokenKind::LeftCurly:
        return blockStatement(yieldHandling);
=======
    default: {
      // Avoid getting next token with SlashIsDiv.
      if (tt == TokenKind::Await && pc_->isAsync()) {
        return expressionStatement(yieldHandling);
      }

      if (!TokenKindIsPossibleIdentifier(tt)) {
        return expressionStatement(yieldHandling);
      }
>>>>>>> upstream-releases

<<<<<<< HEAD
    // VariableStatement[?Yield]
    case TokenKind::Var:
      return variableStatement(yieldHandling);
||||||| merged common ancestors
      // VariableStatement[?Yield]
      case TokenKind::Var:
        return variableStatement(yieldHandling);
=======
      TokenKind next;
      if (!tokenStream.peekToken(&next)) {
        return null();
      }
>>>>>>> upstream-releases

<<<<<<< HEAD
    // EmptyStatement
    case TokenKind::Semi:
      return handler.newEmptyStatement(pos());
||||||| merged common ancestors
      // EmptyStatement
      case TokenKind::Semi:
        return handler.newEmptyStatement(pos());
=======
      // |let| here can only be an Identifier, not a declaration.  Give nicer
      // errors for declaration-looking typos.
      if (tt == TokenKind::Let) {
        bool forbiddenLetDeclaration = false;

        if (next == TokenKind::LeftBracket) {
          // Enforce ExpressionStatement's 'let [' lookahead restriction.
          forbiddenLetDeclaration = true;
        } else if (next == TokenKind::LeftCurly ||
                   TokenKindIsPossibleIdentifier(next)) {
          // 'let {' and 'let foo' aren't completely forbidden, if ASI
          // causes 'let' to be the entire Statement.  But if they're
          // same-line, we can aggressively give a better error message.
          //
          // Note that this ignores 'yield' as TokenKind::Yield: we'll handle it
          // correctly but with a worse error message.
          TokenKind nextSameLine;
          if (!tokenStream.peekTokenSameLine(&nextSameLine)) {
            return null();
          }
>>>>>>> upstream-releases

          MOZ_ASSERT(TokenKindIsPossibleIdentifier(nextSameLine) ||
                     nextSameLine == TokenKind::LeftCurly ||
                     nextSameLine == TokenKind::Eol);

<<<<<<< HEAD
    case TokenKind::Yield: {
      // Don't use a ternary operator here due to obscure linker issues
      // around using static consts in the arms of a ternary.
      Modifier modifier;
      if (yieldExpressionsSupported()) {
        modifier = TokenStream::Operand;
      } else {
        modifier = TokenStream::None;
      }
||||||| merged common ancestors
      case TokenKind::Yield: {
        // Don't use a ternary operator here due to obscure linker issues
        // around using static consts in the arms of a ternary.
        Modifier modifier;
        if (yieldExpressionsSupported()) {
            modifier = TokenStream::Operand;
        } else {
            modifier = TokenStream::None;
        }
=======
          forbiddenLetDeclaration = nextSameLine != TokenKind::Eol;
        }
>>>>>>> upstream-releases

<<<<<<< HEAD
      TokenKind next;
      if (!tokenStream.peekToken(&next, modifier)) {
        return null();
      }
||||||| merged common ancestors
        TokenKind next;
        if (!tokenStream.peekToken(&next, modifier)) {
            return null();
        }
=======
        if (forbiddenLetDeclaration) {
          error(JSMSG_FORBIDDEN_AS_STATEMENT, "lexical declarations");
          return null();
        }
      } else if (tt == TokenKind::Async) {
        // Peek only on the same line: ExpressionStatement's lookahead
        // restriction is phrased as
        //
        //   [lookahead ∉ { '{',
        //                  function,
        //                  async [no LineTerminator here] function,
        //                  class,
        //                  let '[' }]
        //
        // meaning that code like this is valid:
        //
        //   if (true)
        //     async       // ASI opportunity
        //   function clownshoes() {}
        TokenKind maybeFunction;
        if (!tokenStream.peekTokenSameLine(&maybeFunction)) {
          return null();
        }
>>>>>>> upstream-releases

<<<<<<< HEAD
      if (next == TokenKind::Colon) {
        return labeledStatement(yieldHandling);
      }

      return expressionStatement(yieldHandling);
    }
||||||| merged common ancestors
        if (next == TokenKind::Colon) {
            return labeledStatement(yieldHandling);
        }
=======
        if (maybeFunction == TokenKind::Function) {
          error(JSMSG_FORBIDDEN_AS_STATEMENT, "async function declarations");
          return null();
        }
>>>>>>> upstream-releases

<<<<<<< HEAD
    default: {
      // Avoid getting next token with None.
      if (tt == TokenKind::Await && pc->isAsync()) {
        return expressionStatement(yieldHandling);
||||||| merged common ancestors
        return expressionStatement(yieldHandling);
=======
        // Otherwise this |async| begins an ExpressionStatement or is a
        // label name.
>>>>>>> upstream-releases
      }

<<<<<<< HEAD
      if (!TokenKindIsPossibleIdentifier(tt)) {
        return expressionStatement(yieldHandling);
      }

      TokenKind next;
      if (!tokenStream.peekToken(&next)) {
        return null();
      }
||||||| merged common ancestors
      default: {
        // Avoid getting next token with None.
        if (tt == TokenKind::Await && pc->isAsync()) {
            return expressionStatement(yieldHandling);
        }

        if (!TokenKindIsPossibleIdentifier(tt)) {
            return expressionStatement(yieldHandling);
        }
=======
      // NOTE: It's unfortunately allowed to have a label named 'let' in
      //       non-strict code.  💯
      if (next == TokenKind::Colon) {
        return labeledStatement(yieldHandling);
      }
>>>>>>> upstream-releases

<<<<<<< HEAD
      // |let| here can only be an Identifier, not a declaration.  Give nicer
      // errors for declaration-looking typos.
      if (tt == TokenKind::Let) {
        bool forbiddenLetDeclaration = false;

        if (next == TokenKind::LeftBracket) {
          // Enforce ExpressionStatement's 'let [' lookahead restriction.
          forbiddenLetDeclaration = true;
        } else if (next == TokenKind::LeftCurly ||
                   TokenKindIsPossibleIdentifier(next)) {
          // 'let {' and 'let foo' aren't completely forbidden, if ASI
          // causes 'let' to be the entire Statement.  But if they're
          // same-line, we can aggressively give a better error message.
          //
          // Note that this ignores 'yield' as TokenKind::Yield: we'll handle it
          // correctly but with a worse error message.
          TokenKind nextSameLine;
          if (!tokenStream.peekTokenSameLine(&nextSameLine)) {
            return null();
          }
||||||| merged common ancestors
        TokenKind next;
        if (!tokenStream.peekToken(&next)) {
            return null();
        }

        // |let| here can only be an Identifier, not a declaration.  Give nicer
        // errors for declaration-looking typos.
        if (tt == TokenKind::Let) {
            bool forbiddenLetDeclaration = false;

            if (next == TokenKind::LeftBracket) {
                // Enforce ExpressionStatement's 'let [' lookahead restriction.
                forbiddenLetDeclaration = true;
            } else if (next == TokenKind::LeftCurly || TokenKindIsPossibleIdentifier(next)) {
                // 'let {' and 'let foo' aren't completely forbidden, if ASI
                // causes 'let' to be the entire Statement.  But if they're
                // same-line, we can aggressively give a better error message.
                //
                // Note that this ignores 'yield' as TokenKind::Yield: we'll handle it
                // correctly but with a worse error message.
                TokenKind nextSameLine;
                if (!tokenStream.peekTokenSameLine(&nextSameLine)) {
                    return null();
                }

                MOZ_ASSERT(TokenKindIsPossibleIdentifier(nextSameLine) ||
                           nextSameLine == TokenKind::LeftCurly ||
                           nextSameLine == TokenKind::Eol);

                forbiddenLetDeclaration = nextSameLine != TokenKind::Eol;
            }
=======
      return expressionStatement(yieldHandling);
    }

    case TokenKind::New:
      return expressionStatement(yieldHandling, PredictInvoked);
>>>>>>> upstream-releases

<<<<<<< HEAD
          MOZ_ASSERT(TokenKindIsPossibleIdentifier(nextSameLine) ||
                     nextSameLine == TokenKind::LeftCurly ||
                     nextSameLine == TokenKind::Eol);
||||||| merged common ancestors
            if (forbiddenLetDeclaration) {
                error(JSMSG_FORBIDDEN_AS_STATEMENT, "lexical declarations");
                return null();
            }
        } else if (tt == TokenKind::Async) {
            // Peek only on the same line: ExpressionStatement's lookahead
            // restriction is phrased as
            //
            //   [lookahead ∉ { {, function, async [no LineTerminator here] function, class, let [ }]
            //
            // meaning that code like this is valid:
            //
            //   if (true)
            //     async       // ASI opportunity
            //   function clownshoes() {}
            TokenKind maybeFunction;
            if (!tokenStream.peekTokenSameLine(&maybeFunction)) {
                return null();
            }
=======
    // IfStatement[?Yield, ?Return]
    case TokenKind::If:
      return ifStatement(yieldHandling);
>>>>>>> upstream-releases

<<<<<<< HEAD
          forbiddenLetDeclaration = nextSameLine != TokenKind::Eol;
        }
||||||| merged common ancestors
            if (maybeFunction == TokenKind::Function) {
                error(JSMSG_FORBIDDEN_AS_STATEMENT, "async function declarations");
                return null();
            }
=======
    // BreakableStatement[?Yield, ?Return]
    //
    // BreakableStatement[Yield, Return]:
    //   IterationStatement[?Yield, ?Return]
    //   SwitchStatement[?Yield, ?Return]
    case TokenKind::Do:
      return doWhileStatement(yieldHandling);
>>>>>>> upstream-releases

<<<<<<< HEAD
        if (forbiddenLetDeclaration) {
          error(JSMSG_FORBIDDEN_AS_STATEMENT, "lexical declarations");
          return null();
        }
      } else if (tt == TokenKind::Async) {
        // Peek only on the same line: ExpressionStatement's lookahead
        // restriction is phrased as
        //
        //   [lookahead ∉ { '{',
        //                  function,
        //                  async [no LineTerminator here] function,
        //                  class,
        //                  let '[' }]
        //
        // meaning that code like this is valid:
        //
        //   if (true)
        //     async       // ASI opportunity
        //   function clownshoes() {}
        TokenKind maybeFunction;
        if (!tokenStream.peekTokenSameLine(&maybeFunction)) {
          return null();
        }
||||||| merged common ancestors
            // Otherwise this |async| begins an ExpressionStatement or is a
            // label name.
        }
=======
    case TokenKind::While:
      return whileStatement(yieldHandling);
>>>>>>> upstream-releases

<<<<<<< HEAD
        if (maybeFunction == TokenKind::Function) {
          error(JSMSG_FORBIDDEN_AS_STATEMENT, "async function declarations");
          return null();
        }
||||||| merged common ancestors
        // NOTE: It's unfortunately allowed to have a label named 'let' in
        //       non-strict code.  💯
        if (next == TokenKind::Colon) {
            return labeledStatement(yieldHandling);
        }
=======
    case TokenKind::For:
      return forStatement(yieldHandling);
>>>>>>> upstream-releases

<<<<<<< HEAD
        // Otherwise this |async| begins an ExpressionStatement or is a
        // label name.
      }
||||||| merged common ancestors
        return expressionStatement(yieldHandling);
      }
=======
    case TokenKind::Switch:
      return switchStatement(yieldHandling);
>>>>>>> upstream-releases

<<<<<<< HEAD
      // NOTE: It's unfortunately allowed to have a label named 'let' in
      //       non-strict code.  💯
      if (next == TokenKind::Colon) {
        return labeledStatement(yieldHandling);
      }
||||||| merged common ancestors
      case TokenKind::New:
        return expressionStatement(yieldHandling, PredictInvoked);
=======
    // ContinueStatement[?Yield]
    case TokenKind::Continue:
      return continueStatement(yieldHandling);
>>>>>>> upstream-releases

<<<<<<< HEAD
      return expressionStatement(yieldHandling);
    }
||||||| merged common ancestors
      // IfStatement[?Yield, ?Return]
      case TokenKind::If:
        return ifStatement(yieldHandling);
=======
    // BreakStatement[?Yield]
    case TokenKind::Break:
      return breakStatement(yieldHandling);
>>>>>>> upstream-releases

<<<<<<< HEAD
    case TokenKind::New:
      return expressionStatement(yieldHandling, PredictInvoked);
||||||| merged common ancestors
      // BreakableStatement[?Yield, ?Return]
      //
      // BreakableStatement[Yield, Return]:
      //   IterationStatement[?Yield, ?Return]
      //   SwitchStatement[?Yield, ?Return]
      case TokenKind::Do:
        return doWhileStatement(yieldHandling);

      case TokenKind::While:
        return whileStatement(yieldHandling);

      case TokenKind::For:
        return forStatement(yieldHandling);

      case TokenKind::Switch:
        return switchStatement(yieldHandling);

      // ContinueStatement[?Yield]
      case TokenKind::Continue:
        return continueStatement(yieldHandling);

      // BreakStatement[?Yield]
      case TokenKind::Break:
        return breakStatement(yieldHandling);

      // [+Return] ReturnStatement[?Yield]
      case TokenKind::Return:
        // The Return parameter is only used here, and the effect is easily
        // detected this way, so don't bother passing around an extra parameter
        // everywhere.
        if (!pc->isFunctionBox()) {
            error(JSMSG_BAD_RETURN_OR_YIELD, js_return_str);
            return null();
        }
        return returnStatement(yieldHandling);
=======
    // [+Return] ReturnStatement[?Yield]
    case TokenKind::Return:
      // The Return parameter is only used here, and the effect is easily
      // detected this way, so don't bother passing around an extra parameter
      // everywhere.
      if (!pc_->isFunctionBox()) {
        error(JSMSG_BAD_RETURN_OR_YIELD, js_return_str);
        return null();
      }
      return returnStatement(yieldHandling);
>>>>>>> upstream-releases

<<<<<<< HEAD
    // IfStatement[?Yield, ?Return]
    case TokenKind::If:
      return ifStatement(yieldHandling);
||||||| merged common ancestors
      // WithStatement[?Yield, ?Return]
      case TokenKind::With:
        return withStatement(yieldHandling);
=======
    // WithStatement[?Yield, ?Return]
    case TokenKind::With:
      return withStatement(yieldHandling);
>>>>>>> upstream-releases

<<<<<<< HEAD
    // BreakableStatement[?Yield, ?Return]
    //
    // BreakableStatement[Yield, Return]:
    //   IterationStatement[?Yield, ?Return]
    //   SwitchStatement[?Yield, ?Return]
    case TokenKind::Do:
      return doWhileStatement(yieldHandling);
||||||| merged common ancestors
      // LabelledStatement[?Yield, ?Return]
      // This is really handled by default and TokenKind::Yield cases above.
=======
    // LabelledStatement[?Yield, ?Return]
    // This is really handled by default and TokenKind::Yield cases above.
>>>>>>> upstream-releases

<<<<<<< HEAD
    case TokenKind::While:
      return whileStatement(yieldHandling);
||||||| merged common ancestors
      // ThrowStatement[?Yield]
      case TokenKind::Throw:
        return throwStatement(yieldHandling);
=======
    // ThrowStatement[?Yield]
    case TokenKind::Throw:
      return throwStatement(yieldHandling);
>>>>>>> upstream-releases

<<<<<<< HEAD
    case TokenKind::For:
      return forStatement(yieldHandling);
||||||| merged common ancestors
      // TryStatement[?Yield, ?Return]
      case TokenKind::Try:
        return tryStatement(yieldHandling);
=======
    // TryStatement[?Yield, ?Return]
    case TokenKind::Try:
      return tryStatement(yieldHandling);
>>>>>>> upstream-releases

<<<<<<< HEAD
    case TokenKind::Switch:
      return switchStatement(yieldHandling);
||||||| merged common ancestors
      // DebuggerStatement
      case TokenKind::Debugger:
        return debuggerStatement();
=======
    // DebuggerStatement
    case TokenKind::Debugger:
      return debuggerStatement();
>>>>>>> upstream-releases

<<<<<<< HEAD
    // ContinueStatement[?Yield]
    case TokenKind::Continue:
      return continueStatement(yieldHandling);
||||||| merged common ancestors
      // |function| is forbidden by lookahead restriction (unless as child
      // statement of |if| or |else|, but Parser::consequentOrAlternative
      // handles that).
      case TokenKind::Function:
        error(JSMSG_FORBIDDEN_AS_STATEMENT, "function declarations");
        return null();
=======
    // |function| is forbidden by lookahead restriction (unless as child
    // statement of |if| or |else|, but Parser::consequentOrAlternative
    // handles that).
    case TokenKind::Function:
      error(JSMSG_FORBIDDEN_AS_STATEMENT, "function declarations");
      return null();
>>>>>>> upstream-releases

<<<<<<< HEAD
    // BreakStatement[?Yield]
    case TokenKind::Break:
      return breakStatement(yieldHandling);

    // [+Return] ReturnStatement[?Yield]
    case TokenKind::Return:
      // The Return parameter is only used here, and the effect is easily
      // detected this way, so don't bother passing around an extra parameter
      // everywhere.
      if (!pc->isFunctionBox()) {
        error(JSMSG_BAD_RETURN_OR_YIELD, js_return_str);
        return null();
      }
      return returnStatement(yieldHandling);

    // WithStatement[?Yield, ?Return]
    case TokenKind::With:
      return withStatement(yieldHandling);

    // LabelledStatement[?Yield, ?Return]
    // This is really handled by default and TokenKind::Yield cases above.
||||||| merged common ancestors
      // |class| is also forbidden by lookahead restriction.
      case TokenKind::Class:
        error(JSMSG_FORBIDDEN_AS_STATEMENT, "classes");
        return null();
=======
    // |class| is also forbidden by lookahead restriction.
    case TokenKind::Class:
      error(JSMSG_FORBIDDEN_AS_STATEMENT, "classes");
      return null();
>>>>>>> upstream-releases

<<<<<<< HEAD
    // ThrowStatement[?Yield]
    case TokenKind::Throw:
      return throwStatement(yieldHandling);
||||||| merged common ancestors
      // ImportDeclaration (only inside modules)
      case TokenKind::Import:
        return importDeclarationOrImportMeta(yieldHandling);
=======
    // ImportDeclaration (only inside modules)
    case TokenKind::Import:
      return importDeclarationOrImportExpr(yieldHandling);
>>>>>>> upstream-releases

<<<<<<< HEAD
    // TryStatement[?Yield, ?Return]
    case TokenKind::Try:
      return tryStatement(yieldHandling);

    // DebuggerStatement
    case TokenKind::Debugger:
      return debuggerStatement();

    // |function| is forbidden by lookahead restriction (unless as child
    // statement of |if| or |else|, but Parser::consequentOrAlternative
    // handles that).
    case TokenKind::Function:
      error(JSMSG_FORBIDDEN_AS_STATEMENT, "function declarations");
      return null();

    // |class| is also forbidden by lookahead restriction.
    case TokenKind::Class:
      error(JSMSG_FORBIDDEN_AS_STATEMENT, "classes");
      return null();

    // ImportDeclaration (only inside modules)
    case TokenKind::Import:
      return importDeclarationOrImportExpr(yieldHandling);

    // ExportDeclaration (only inside modules)
    case TokenKind::Export:
      return exportDeclaration();
||||||| merged common ancestors
      // ExportDeclaration (only inside modules)
      case TokenKind::Export:
        return exportDeclaration();
=======
    // ExportDeclaration (only inside modules)
    case TokenKind::Export:
      return exportDeclaration();
>>>>>>> upstream-releases

      // Miscellaneous error cases arguably better caught here than elsewhere.

    case TokenKind::Catch:
      error(JSMSG_CATCH_WITHOUT_TRY);
      return null();

    case TokenKind::Finally:
      error(JSMSG_FINALLY_WITHOUT_TRY);
      return null();

      // NOTE: default case handled in the ExpressionStatement section.
  }
}

template <class ParseHandler, typename Unit>
typename ParseHandler::Node
<<<<<<< HEAD
GeneralParser<ParseHandler, Unit>::statementListItem(
    YieldHandling yieldHandling, bool canHaveDirectives /* = false */) {
  MOZ_ASSERT(checkOptionsCalled);
||||||| merged common ancestors
GeneralParser<ParseHandler, Unit>::statementListItem(YieldHandling yieldHandling,
                                                     bool canHaveDirectives /* = false */)
{
    MOZ_ASSERT(checkOptionsCalled);
=======
GeneralParser<ParseHandler, Unit>::statementListItem(
    YieldHandling yieldHandling, bool canHaveDirectives /* = false */) {
  MOZ_ASSERT(checkOptionsCalled_);
>>>>>>> upstream-releases

<<<<<<< HEAD
  if (!CheckRecursionLimit(context)) {
    return null();
  }
||||||| merged common ancestors
    if (!CheckRecursionLimit(context)) {
        return null();
    }
=======
  if (!CheckRecursionLimit(cx_)) {
    return null();
  }
>>>>>>> upstream-releases

<<<<<<< HEAD
  TokenKind tt;
  if (!tokenStream.getToken(&tt, TokenStream::Operand)) {
    return null();
  }
||||||| merged common ancestors
    TokenKind tt;
    if (!tokenStream.getToken(&tt, TokenStream::Operand)) {
        return null();
    }
=======
  TokenKind tt;
  if (!tokenStream.getToken(&tt, TokenStream::SlashIsRegExp)) {
    return null();
  }
>>>>>>> upstream-releases

  switch (tt) {
    // BlockStatement[?Yield, ?Return]
    case TokenKind::LeftCurly:
      return blockStatement(yieldHandling);

    // VariableStatement[?Yield]
    case TokenKind::Var:
      return variableStatement(yieldHandling);

<<<<<<< HEAD
    // EmptyStatement
    case TokenKind::Semi:
      return handler.newEmptyStatement(pos());
||||||| merged common ancestors
      // EmptyStatement
      case TokenKind::Semi:
        return handler.newEmptyStatement(pos());
=======
    // EmptyStatement
    case TokenKind::Semi:
      return handler_.newEmptyStatement(pos());
>>>>>>> upstream-releases

<<<<<<< HEAD
    // ExpressionStatement[?Yield].
    //
    // These should probably be handled by a single ExpressionStatement
    // function in a default, not split up this way.
    case TokenKind::String:
      if (!canHaveDirectives &&
          anyChars.currentToken().atom() == context->names().useAsm) {
        if (!abortIfSyntaxParser()) {
          return null();
||||||| merged common ancestors
      // ExpressionStatement[?Yield].
      //
      // These should probably be handled by a single ExpressionStatement
      // function in a default, not split up this way.
      case TokenKind::String:
        if (!canHaveDirectives && anyChars.currentToken().atom() == context->names().useAsm) {
            if (!abortIfSyntaxParser()) {
                return null();
            }
            if (!warning(JSMSG_USE_ASM_DIRECTIVE_FAIL)) {
                return null();
            }
=======
    // ExpressionStatement[?Yield].
    //
    // These should probably be handled by a single ExpressionStatement
    // function in a default, not split up this way.
    case TokenKind::String:
      if (!canHaveDirectives &&
          anyChars.currentToken().atom() == cx_->names().useAsm) {
        if (!abortIfSyntaxParser()) {
          return null();
>>>>>>> upstream-releases
        }
<<<<<<< HEAD
        if (!warning(JSMSG_USE_ASM_DIRECTIVE_FAIL)) {
          return null();
        }
      }
      return expressionStatement(yieldHandling);

    case TokenKind::Yield: {
      // Don't use a ternary operator here due to obscure linker issues
      // around using static consts in the arms of a ternary.
      Modifier modifier;
      if (yieldExpressionsSupported()) {
        modifier = TokenStream::Operand;
      } else {
        modifier = TokenStream::None;
      }

      TokenKind next;
      if (!tokenStream.peekToken(&next, modifier)) {
        return null();
      }

      if (next == TokenKind::Colon) {
        return labeledStatement(yieldHandling);
      }

      return expressionStatement(yieldHandling);
    }

    default: {
      // Avoid getting next token with None.
      if (tt == TokenKind::Await && pc->isAsync()) {
        return expressionStatement(yieldHandling);
      }

      if (!TokenKindIsPossibleIdentifier(tt)) {
        return expressionStatement(yieldHandling);
      }
||||||| merged common ancestors
        return expressionStatement(yieldHandling);

      case TokenKind::Yield: {
        // Don't use a ternary operator here due to obscure linker issues
        // around using static consts in the arms of a ternary.
        Modifier modifier;
        if (yieldExpressionsSupported()) {
            modifier = TokenStream::Operand;
        } else {
            modifier = TokenStream::None;
        }
=======
        if (!warning(JSMSG_USE_ASM_DIRECTIVE_FAIL)) {
          return null();
        }
      }
      return expressionStatement(yieldHandling);

    case TokenKind::Yield: {
      // Don't use a ternary operator here due to obscure linker issues
      // around using static consts in the arms of a ternary.
      Modifier modifier;
      if (yieldExpressionsSupported()) {
        modifier = TokenStream::SlashIsRegExp;
      } else {
        modifier = TokenStream::SlashIsDiv;
      }
>>>>>>> upstream-releases

<<<<<<< HEAD
      TokenKind next;
      if (!tokenStream.peekToken(&next)) {
        return null();
      }

      if (tt == TokenKind::Let && nextTokenContinuesLetDeclaration(next)) {
        return lexicalDeclaration(yieldHandling, DeclarationKind::Let);
      }
||||||| merged common ancestors
        TokenKind next;
        if (!tokenStream.peekToken(&next, modifier)) {
            return null();
        }
=======
      TokenKind next;
      if (!tokenStream.peekToken(&next, modifier)) {
        return null();
      }
>>>>>>> upstream-releases

<<<<<<< HEAD
      if (tt == TokenKind::Async) {
        TokenKind nextSameLine = TokenKind::Eof;
        if (!tokenStream.peekTokenSameLine(&nextSameLine)) {
          return null();
        }
        if (nextSameLine == TokenKind::Function) {
          uint32_t toStringStart = pos().begin;
          tokenStream.consumeKnownToken(TokenKind::Function);
          return functionStmt(toStringStart, yieldHandling, NameRequired,
                              FunctionAsyncKind::AsyncFunction);
        }
      }
||||||| merged common ancestors
        if (next == TokenKind::Colon) {
            return labeledStatement(yieldHandling);
        }
=======
      if (next == TokenKind::Colon) {
        return labeledStatement(yieldHandling);
      }

      return expressionStatement(yieldHandling);
    }
>>>>>>> upstream-releases

<<<<<<< HEAD
      if (next == TokenKind::Colon) {
        return labeledStatement(yieldHandling);
||||||| merged common ancestors
        return expressionStatement(yieldHandling);
=======
    default: {
      // Avoid getting next token with SlashIsDiv.
      if (tt == TokenKind::Await && pc_->isAsync()) {
        return expressionStatement(yieldHandling);
>>>>>>> upstream-releases
      }

<<<<<<< HEAD
      return expressionStatement(yieldHandling);
    }

    case TokenKind::New:
      return expressionStatement(yieldHandling, PredictInvoked);
||||||| merged common ancestors
      default: {
        // Avoid getting next token with None.
        if (tt == TokenKind::Await && pc->isAsync()) {
            return expressionStatement(yieldHandling);
        }

        if (!TokenKindIsPossibleIdentifier(tt)) {
            return expressionStatement(yieldHandling);
        }
=======
      if (!TokenKindIsPossibleIdentifier(tt)) {
        return expressionStatement(yieldHandling);
      }
>>>>>>> upstream-releases

<<<<<<< HEAD
    // IfStatement[?Yield, ?Return]
    case TokenKind::If:
      return ifStatement(yieldHandling);
||||||| merged common ancestors
        TokenKind next;
        if (!tokenStream.peekToken(&next)) {
            return null();
        }
=======
      TokenKind next;
      if (!tokenStream.peekToken(&next)) {
        return null();
      }
>>>>>>> upstream-releases

<<<<<<< HEAD
    // BreakableStatement[?Yield, ?Return]
    //
    // BreakableStatement[Yield, Return]:
    //   IterationStatement[?Yield, ?Return]
    //   SwitchStatement[?Yield, ?Return]
    case TokenKind::Do:
      return doWhileStatement(yieldHandling);
||||||| merged common ancestors
        if (tt == TokenKind::Let && nextTokenContinuesLetDeclaration(next)) {
            return lexicalDeclaration(yieldHandling, DeclarationKind::Let);
        }
=======
      if (tt == TokenKind::Let && nextTokenContinuesLetDeclaration(next)) {
        return lexicalDeclaration(yieldHandling, DeclarationKind::Let);
      }
>>>>>>> upstream-releases

<<<<<<< HEAD
    case TokenKind::While:
      return whileStatement(yieldHandling);

    case TokenKind::For:
      return forStatement(yieldHandling);
||||||| merged common ancestors
        if (tt == TokenKind::Async) {
            TokenKind nextSameLine = TokenKind::Eof;
            if (!tokenStream.peekTokenSameLine(&nextSameLine)) {
                return null();
            }
            if (nextSameLine == TokenKind::Function) {
                uint32_t toStringStart = pos().begin;
                tokenStream.consumeKnownToken(TokenKind::Function);
                return functionStmt(toStringStart, yieldHandling, NameRequired,
                                    FunctionAsyncKind::AsyncFunction);
            }
        }

        if (next == TokenKind::Colon) {
            return labeledStatement(yieldHandling);
        }
=======
      if (tt == TokenKind::Async) {
        TokenKind nextSameLine = TokenKind::Eof;
        if (!tokenStream.peekTokenSameLine(&nextSameLine)) {
          return null();
        }
        if (nextSameLine == TokenKind::Function) {
          uint32_t toStringStart = pos().begin;
          tokenStream.consumeKnownToken(TokenKind::Function);
          return functionStmt(toStringStart, yieldHandling, NameRequired,
                              FunctionAsyncKind::AsyncFunction);
        }
      }
>>>>>>> upstream-releases

<<<<<<< HEAD
    case TokenKind::Switch:
      return switchStatement(yieldHandling);
||||||| merged common ancestors
        return expressionStatement(yieldHandling);
      }
=======
      if (next == TokenKind::Colon) {
        return labeledStatement(yieldHandling);
      }
>>>>>>> upstream-releases

<<<<<<< HEAD
    // ContinueStatement[?Yield]
    case TokenKind::Continue:
      return continueStatement(yieldHandling);
||||||| merged common ancestors
      case TokenKind::New:
        return expressionStatement(yieldHandling, PredictInvoked);
=======
      return expressionStatement(yieldHandling);
    }
>>>>>>> upstream-releases

<<<<<<< HEAD
    // BreakStatement[?Yield]
    case TokenKind::Break:
      return breakStatement(yieldHandling);
||||||| merged common ancestors
      // IfStatement[?Yield, ?Return]
      case TokenKind::If:
        return ifStatement(yieldHandling);
=======
    case TokenKind::New:
      return expressionStatement(yieldHandling, PredictInvoked);
>>>>>>> upstream-releases

<<<<<<< HEAD
    // [+Return] ReturnStatement[?Yield]
    case TokenKind::Return:
      // The Return parameter is only used here, and the effect is easily
      // detected this way, so don't bother passing around an extra parameter
      // everywhere.
      if (!pc->isFunctionBox()) {
        error(JSMSG_BAD_RETURN_OR_YIELD, js_return_str);
        return null();
      }
      return returnStatement(yieldHandling);
||||||| merged common ancestors
      // BreakableStatement[?Yield, ?Return]
      //
      // BreakableStatement[Yield, Return]:
      //   IterationStatement[?Yield, ?Return]
      //   SwitchStatement[?Yield, ?Return]
      case TokenKind::Do:
        return doWhileStatement(yieldHandling);

      case TokenKind::While:
        return whileStatement(yieldHandling);

      case TokenKind::For:
        return forStatement(yieldHandling);

      case TokenKind::Switch:
        return switchStatement(yieldHandling);

      // ContinueStatement[?Yield]
      case TokenKind::Continue:
        return continueStatement(yieldHandling);

      // BreakStatement[?Yield]
      case TokenKind::Break:
        return breakStatement(yieldHandling);

      // [+Return] ReturnStatement[?Yield]
      case TokenKind::Return:
        // The Return parameter is only used here, and the effect is easily
        // detected this way, so don't bother passing around an extra parameter
        // everywhere.
        if (!pc->isFunctionBox()) {
            error(JSMSG_BAD_RETURN_OR_YIELD, js_return_str);
            return null();
        }
        return returnStatement(yieldHandling);
=======
    // IfStatement[?Yield, ?Return]
    case TokenKind::If:
      return ifStatement(yieldHandling);

    // BreakableStatement[?Yield, ?Return]
    //
    // BreakableStatement[Yield, Return]:
    //   IterationStatement[?Yield, ?Return]
    //   SwitchStatement[?Yield, ?Return]
    case TokenKind::Do:
      return doWhileStatement(yieldHandling);

    case TokenKind::While:
      return whileStatement(yieldHandling);
>>>>>>> upstream-releases

<<<<<<< HEAD
    // WithStatement[?Yield, ?Return]
    case TokenKind::With:
      return withStatement(yieldHandling);
||||||| merged common ancestors
      // WithStatement[?Yield, ?Return]
      case TokenKind::With:
        return withStatement(yieldHandling);
=======
    case TokenKind::For:
      return forStatement(yieldHandling);
>>>>>>> upstream-releases

<<<<<<< HEAD
    // LabelledStatement[?Yield, ?Return]
    // This is really handled by default and TokenKind::Yield cases above.
||||||| merged common ancestors
      // LabelledStatement[?Yield, ?Return]
      // This is really handled by default and TokenKind::Yield cases above.
=======
    case TokenKind::Switch:
      return switchStatement(yieldHandling);
>>>>>>> upstream-releases

<<<<<<< HEAD
    // ThrowStatement[?Yield]
    case TokenKind::Throw:
      return throwStatement(yieldHandling);
||||||| merged common ancestors
      // ThrowStatement[?Yield]
      case TokenKind::Throw:
        return throwStatement(yieldHandling);
=======
    // ContinueStatement[?Yield]
    case TokenKind::Continue:
      return continueStatement(yieldHandling);
>>>>>>> upstream-releases

<<<<<<< HEAD
    // TryStatement[?Yield, ?Return]
    case TokenKind::Try:
      return tryStatement(yieldHandling);
||||||| merged common ancestors
      // TryStatement[?Yield, ?Return]
      case TokenKind::Try:
        return tryStatement(yieldHandling);
=======
    // BreakStatement[?Yield]
    case TokenKind::Break:
      return breakStatement(yieldHandling);
>>>>>>> upstream-releases

<<<<<<< HEAD
    // DebuggerStatement
    case TokenKind::Debugger:
      return debuggerStatement();
||||||| merged common ancestors
      // DebuggerStatement
      case TokenKind::Debugger:
        return debuggerStatement();
=======
    // [+Return] ReturnStatement[?Yield]
    case TokenKind::Return:
      // The Return parameter is only used here, and the effect is easily
      // detected this way, so don't bother passing around an extra parameter
      // everywhere.
      if (!pc_->isFunctionBox()) {
        error(JSMSG_BAD_RETURN_OR_YIELD, js_return_str);
        return null();
      }
      return returnStatement(yieldHandling);

    // WithStatement[?Yield, ?Return]
    case TokenKind::With:
      return withStatement(yieldHandling);

    // LabelledStatement[?Yield, ?Return]
    // This is really handled by default and TokenKind::Yield cases above.
>>>>>>> upstream-releases

<<<<<<< HEAD
    // Declaration[Yield]:
||||||| merged common ancestors
      // Declaration[Yield]:
=======
    // ThrowStatement[?Yield]
    case TokenKind::Throw:
      return throwStatement(yieldHandling);
>>>>>>> upstream-releases

<<<<<<< HEAD
    //   HoistableDeclaration[?Yield, ~Default]
    case TokenKind::Function:
      return functionStmt(pos().begin, yieldHandling, NameRequired);
||||||| merged common ancestors
      //   HoistableDeclaration[?Yield, ~Default]
      case TokenKind::Function:
        return functionStmt(pos().begin, yieldHandling, NameRequired);
=======
    // TryStatement[?Yield, ?Return]
    case TokenKind::Try:
      return tryStatement(yieldHandling);
>>>>>>> upstream-releases

<<<<<<< HEAD
    //   ClassDeclaration[?Yield, ~Default]
    case TokenKind::Class:
      return classDefinition(yieldHandling, ClassStatement, NameRequired);
||||||| merged common ancestors
      //   ClassDeclaration[?Yield, ~Default]
      case TokenKind::Class:
        return classDefinition(yieldHandling, ClassStatement, NameRequired);
=======
    // DebuggerStatement
    case TokenKind::Debugger:
      return debuggerStatement();
>>>>>>> upstream-releases

<<<<<<< HEAD
    //   LexicalDeclaration[In, ?Yield]
    //     LetOrConst BindingList[?In, ?Yield]
    case TokenKind::Const:
      // [In] is the default behavior, because for-loops specially parse
      // their heads to handle |in| in this situation.
      return lexicalDeclaration(yieldHandling, DeclarationKind::Const);
||||||| merged common ancestors
      //   LexicalDeclaration[In, ?Yield]
      //     LetOrConst BindingList[?In, ?Yield]
      case TokenKind::Const:
        // [In] is the default behavior, because for-loops specially parse
        // their heads to handle |in| in this situation.
        return lexicalDeclaration(yieldHandling, DeclarationKind::Const);
=======
    // Declaration[Yield]:
>>>>>>> upstream-releases

<<<<<<< HEAD
    // ImportDeclaration (only inside modules)
    case TokenKind::Import:
      return importDeclarationOrImportExpr(yieldHandling);
||||||| merged common ancestors
      // ImportDeclaration (only inside modules)
      case TokenKind::Import:
        return importDeclarationOrImportMeta(yieldHandling);
=======
    //   HoistableDeclaration[?Yield, ~Default]
    case TokenKind::Function:
      return functionStmt(pos().begin, yieldHandling, NameRequired);
>>>>>>> upstream-releases

<<<<<<< HEAD
    // ExportDeclaration (only inside modules)
    case TokenKind::Export:
      return exportDeclaration();
||||||| merged common ancestors
      // ExportDeclaration (only inside modules)
      case TokenKind::Export:
        return exportDeclaration();
=======
    //   ClassDeclaration[?Yield, ~Default]
    case TokenKind::Class:
      return classDefinition(yieldHandling, ClassStatement, NameRequired);

    //   LexicalDeclaration[In, ?Yield]
    //     LetOrConst BindingList[?In, ?Yield]
    case TokenKind::Const:
      // [In] is the default behavior, because for-loops specially parse
      // their heads to handle |in| in this situation.
      return lexicalDeclaration(yieldHandling, DeclarationKind::Const);

    // ImportDeclaration (only inside modules)
    case TokenKind::Import:
      return importDeclarationOrImportExpr(yieldHandling);

    // ExportDeclaration (only inside modules)
    case TokenKind::Export:
      return exportDeclaration();
>>>>>>> upstream-releases

      // Miscellaneous error cases arguably better caught here than elsewhere.

    case TokenKind::Catch:
      error(JSMSG_CATCH_WITHOUT_TRY);
      return null();

    case TokenKind::Finally:
      error(JSMSG_FINALLY_WITHOUT_TRY);
      return null();

      // NOTE: default case handled in the ExpressionStatement section.
  }
}

template <class ParseHandler, typename Unit>
typename ParseHandler::Node GeneralParser<ParseHandler, Unit>::expr(
    InHandling inHandling, YieldHandling yieldHandling,
    TripledotHandling tripledotHandling,
    PossibleError* possibleError /* = nullptr */,
    InvokedPrediction invoked /* = PredictUninvoked */) {
  Node pn = assignExpr(inHandling, yieldHandling, tripledotHandling,
                       possibleError, invoked);
  if (!pn) {
    return null();
  }

<<<<<<< HEAD
  bool matched;
  if (!tokenStream.matchToken(&matched, TokenKind::Comma,
                              TokenStream::Operand)) {
    return null();
  }
  if (!matched) {
    return pn;
  }
||||||| merged common ancestors
    bool matched;
    if (!tokenStream.matchToken(&matched, TokenKind::Comma, TokenStream::Operand)) {
        return null();
    }
    if (!matched) {
        return pn;
    }
=======
  bool matched;
  if (!tokenStream.matchToken(&matched, TokenKind::Comma,
                              TokenStream::SlashIsRegExp)) {
    return null();
  }
  if (!matched) {
    return pn;
  }
>>>>>>> upstream-releases

<<<<<<< HEAD
  ListNodeType seq = handler.newCommaExpressionList(pn);
  if (!seq) {
    return null();
  }
  while (true) {
    // Trailing comma before the closing parenthesis is valid in an arrow
    // function parameters list: `(a, b, ) => body`. Check if we are
    // directly under CoverParenthesizedExpressionAndArrowParameterList,
    // and the next two tokens are closing parenthesis and arrow. If all
    // are present allow the trailing comma.
    if (tripledotHandling == TripledotAllowed) {
      TokenKind tt;
      if (!tokenStream.peekToken(&tt, TokenStream::Operand)) {
||||||| merged common ancestors
    ListNodeType seq = handler.newCommaExpressionList(pn);
    if (!seq) {
=======
  ListNodeType seq = handler_.newCommaExpressionList(pn);
  if (!seq) {
    return null();
  }
  while (true) {
    // Trailing comma before the closing parenthesis is valid in an arrow
    // function parameters list: `(a, b, ) => body`. Check if we are
    // directly under CoverParenthesizedExpressionAndArrowParameterList,
    // and the next two tokens are closing parenthesis and arrow. If all
    // are present allow the trailing comma.
    if (tripledotHandling == TripledotAllowed) {
      TokenKind tt;
      if (!tokenStream.peekToken(&tt, TokenStream::SlashIsRegExp)) {
>>>>>>> upstream-releases
        return null();
      }

<<<<<<< HEAD
      if (tt == TokenKind::RightParen) {
        tokenStream.consumeKnownToken(TokenKind::RightParen,
                                      TokenStream::Operand);
||||||| merged common ancestors
                if (!tokenStream.peekToken(&tt)) {
                    return null();
                }
                if (tt != TokenKind::Arrow) {
                    error(JSMSG_UNEXPECTED_TOKEN, "expression", TokenKindToDesc(TokenKind::RightParen));
                    return null();
                }
=======
      if (tt == TokenKind::RightParen) {
        tokenStream.consumeKnownToken(TokenKind::RightParen,
                                      TokenStream::SlashIsRegExp);
>>>>>>> upstream-releases

        if (!tokenStream.peekToken(&tt)) {
          return null();
        }
        if (tt != TokenKind::Arrow) {
          error(JSMSG_UNEXPECTED_TOKEN, "expression",
                TokenKindToDesc(TokenKind::RightParen));
          return null();
        }

        anyChars.ungetToken();  // put back right paren
        break;
      }
    }

    // Additional calls to assignExpr should not reuse the possibleError
    // which had been passed into the function. Otherwise we would lose
    // information needed to determine whether or not we're dealing with
    // a non-recoverable situation.
    PossibleError possibleErrorInner(*this);
    pn = assignExpr(inHandling, yieldHandling, tripledotHandling,
                    &possibleErrorInner);
    if (!pn) {
      return null();
    }

<<<<<<< HEAD
    if (!possibleError) {
      // Report any pending expression error.
      if (!possibleErrorInner.checkForExpressionError()) {
        return null();
      }
    } else {
      possibleErrorInner.transferErrorsTo(possibleError);
||||||| merged common ancestors
        if (!tokenStream.matchToken(&matched, TokenKind::Comma, TokenStream::Operand)) {
            return null();
        }
        if (!matched) {
            break;
        }
=======
    if (!possibleError) {
      // Report any pending expression error.
      if (!possibleErrorInner.checkForExpressionError()) {
        return null();
      }
    } else {
      possibleErrorInner.transferErrorsTo(possibleError);
    }

    handler_.addList(seq, pn);

    if (!tokenStream.matchToken(&matched, TokenKind::Comma,
                                TokenStream::SlashIsRegExp)) {
      return null();
    }
    if (!matched) {
      break;
>>>>>>> upstream-releases
    }
<<<<<<< HEAD

    handler.addList(seq, pn);

    if (!tokenStream.matchToken(&matched, TokenKind::Comma,
                                TokenStream::Operand)) {
      return null();
    }
    if (!matched) {
      break;
    }
  }
  return seq;
||||||| merged common ancestors
    return seq;
=======
  }
  return seq;
>>>>>>> upstream-releases
}

static ParseNodeKind BinaryOpTokenKindToParseNodeKind(TokenKind tok) {
  MOZ_ASSERT(TokenKindIsBinaryOp(tok));
  return ParseNodeKind(size_t(ParseNodeKind::BinOpFirst) +
                       (size_t(tok) - size_t(TokenKind::BinOpFirst)));
}

static const int PrecedenceTable[] = {
    1,  /* ParseNodeKind::PipeLine */
    2,  /* ParseNodeKind::Or */
    3,  /* ParseNodeKind::And */
    4,  /* ParseNodeKind::BitOr */
    5,  /* ParseNodeKind::BitXor */
    6,  /* ParseNodeKind::BitAnd */
    7,  /* ParseNodeKind::StrictEq */
    7,  /* ParseNodeKind::Eq */
    7,  /* ParseNodeKind::StrictNe */
    7,  /* ParseNodeKind::Ne */
    8,  /* ParseNodeKind::Lt */
    8,  /* ParseNodeKind::Le */
    8,  /* ParseNodeKind::Gt */
    8,  /* ParseNodeKind::Ge */
    8,  /* ParseNodeKind::InstanceOf */
    8,  /* ParseNodeKind::In */
    9,  /* ParseNodeKind::Lsh */
    9,  /* ParseNodeKind::Rsh */
    9,  /* ParseNodeKind::Ursh */
    10, /* ParseNodeKind::Add */
    10, /* ParseNodeKind::Sub */
    11, /* ParseNodeKind::Star */
    11, /* ParseNodeKind::Div */
    11, /* ParseNodeKind::Mod */
    12  /* ParseNodeKind::Pow */
};

static const int PRECEDENCE_CLASSES = 12;

static int Precedence(ParseNodeKind pnk) {
  // Everything binds tighter than ParseNodeKind::Limit, because we want
  // to reduce all nodes to a single node when we reach a token that is not
  // another binary operator.
  if (pnk == ParseNodeKind::Limit) {
    return 0;
  }

  MOZ_ASSERT(pnk >= ParseNodeKind::BinOpFirst);
  MOZ_ASSERT(pnk <= ParseNodeKind::BinOpLast);
  return PrecedenceTable[size_t(pnk) - size_t(ParseNodeKind::BinOpFirst)];
}

template <class ParseHandler, typename Unit>
MOZ_ALWAYS_INLINE typename ParseHandler::Node
GeneralParser<ParseHandler, Unit>::orExpr(
    InHandling inHandling, YieldHandling yieldHandling,
    TripledotHandling tripledotHandling, PossibleError* possibleError,
    InvokedPrediction invoked /* = PredictUninvoked */) {
  // Shift-reduce parser for the binary operator part of the JS expression
  // syntax.

  // Conceptually there's just one stack, a stack of pairs (lhs, op).
  // It's implemented using two separate arrays, though.
  Node nodeStack[PRECEDENCE_CLASSES];
  ParseNodeKind kindStack[PRECEDENCE_CLASSES];
  int depth = 0;
  Node pn;
  for (;;) {
    pn = unaryExpr(yieldHandling, tripledotHandling, possibleError, invoked);
    if (!pn) {
      return null();
    }

    // If a binary operator follows, consume it and compute the
    // corresponding operator.
    TokenKind tok;
    if (!tokenStream.getToken(&tok)) {
      return null();
    }

<<<<<<< HEAD
    ParseNodeKind pnk;
    if (tok == TokenKind::In ? inHandling == InAllowed
                             : TokenKindIsBinaryOp(tok)) {
      // We're definitely not in a destructuring context, so report any
      // pending expression error now.
      if (possibleError && !possibleError->checkForExpressionError()) {
        return null();
      }
      // Report an error for unary expressions on the LHS of **.
      if (tok == TokenKind::Pow &&
          handler.isUnparenthesizedUnaryExpression(pn)) {
        error(JSMSG_BAD_POW_LEFTSIDE);
        return null();
      }
      pnk = BinaryOpTokenKindToParseNodeKind(tok);
    } else {
      tok = TokenKind::Eof;
      pnk = ParseNodeKind::Limit;
    }
||||||| merged common ancestors
        // From this point on, destructuring defaults are definitely an error.
        possibleError = nullptr;
=======
    ParseNodeKind pnk;
    if (tok == TokenKind::In ? inHandling == InAllowed
                             : TokenKindIsBinaryOp(tok)) {
      // We're definitely not in a destructuring context, so report any
      // pending expression error now.
      if (possibleError && !possibleError->checkForExpressionError()) {
        return null();
      }
      // Report an error for unary expressions on the LHS of **.
      if (tok == TokenKind::Pow &&
          handler_.isUnparenthesizedUnaryExpression(pn)) {
        error(JSMSG_BAD_POW_LEFTSIDE);
        return null();
      }
      pnk = BinaryOpTokenKindToParseNodeKind(tok);
    } else {
      tok = TokenKind::Eof;
      pnk = ParseNodeKind::Limit;
    }
>>>>>>> upstream-releases

    // From this point on, destructuring defaults are definitely an error.
    possibleError = nullptr;

<<<<<<< HEAD
    // If pnk has precedence less than or equal to another operator on the
    // stack, reduce. This combines nodes on the stack until we form the
    // actual lhs of pnk.
    //
    // The >= in this condition works because it is appendOrCreateList's
    // job to decide if the operator in question is left- or
    // right-associative, and build the corresponding tree.
    while (depth > 0 && Precedence(kindStack[depth - 1]) >= Precedence(pnk)) {
      depth--;
      ParseNodeKind combiningPnk = kindStack[depth];
      pn = handler.appendOrCreateList(combiningPnk, nodeStack[depth], pn, pc);
      if (!pn) {
        return null();
      }
    }
||||||| merged common ancestors
        if (pnk == ParseNodeKind::Limit) {
            break;
        }
=======
    // If pnk has precedence less than or equal to another operator on the
    // stack, reduce. This combines nodes on the stack until we form the
    // actual lhs of pnk.
    //
    // The >= in this condition works because it is appendOrCreateList's
    // job to decide if the operator in question is left- or
    // right-associative, and build the corresponding tree.
    while (depth > 0 && Precedence(kindStack[depth - 1]) >= Precedence(pnk)) {
      depth--;
      ParseNodeKind combiningPnk = kindStack[depth];
      pn = handler_.appendOrCreateList(combiningPnk, nodeStack[depth], pn, pc_);
      if (!pn) {
        return null();
      }
    }
>>>>>>> upstream-releases

    if (pnk == ParseNodeKind::Limit) {
      break;
    }

    nodeStack[depth] = pn;
    kindStack[depth] = pnk;
    depth++;
    MOZ_ASSERT(depth <= PRECEDENCE_CLASSES);
  }

<<<<<<< HEAD
  // When the next token is no longer a binary operator, it's potentially the
  // start of an expression.  Add a modifier exception so that the next token
  // modifier can be Operand.
  anyChars.ungetToken();
  anyChars.addModifierException(TokenStream::OperandIsNone);

  MOZ_ASSERT(depth == 0);
  return pn;
||||||| merged common ancestors
    MOZ_ASSERT(depth == 0);
    return pn;
=======
  anyChars.ungetToken();

  // Had the next token been a Div, we would have consumed it. So there's no
  // ambiguity if we later (after ASI) re-get this token with SlashIsRegExp.
  anyChars.allowGettingNextTokenWithSlashIsRegExp();

  MOZ_ASSERT(depth == 0);
  return pn;
>>>>>>> upstream-releases
}

template <class ParseHandler, typename Unit>
MOZ_ALWAYS_INLINE typename ParseHandler::Node
GeneralParser<ParseHandler, Unit>::condExpr(
    InHandling inHandling, YieldHandling yieldHandling,
    TripledotHandling tripledotHandling, PossibleError* possibleError,
    InvokedPrediction invoked /* = PredictUninvoked */) {
  Node condition = orExpr(inHandling, yieldHandling, tripledotHandling,
                          possibleError, invoked);
  if (!condition) {
    return null();
  }

<<<<<<< HEAD
  bool matched;
  if (!tokenStream.matchToken(&matched, TokenKind::Hook)) {
    return null();
  }
  if (!matched) {
    return condition;
  }
||||||| merged common ancestors
    bool matched;
    if (!tokenStream.matchToken(&matched, TokenKind::Hook)) {
        return null();
    }
    if (!matched) {
        return condition;
    }
=======
  bool matched;
  if (!tokenStream.matchToken(&matched, TokenKind::Hook,
                              TokenStream::SlashIsInvalid)) {
    return null();
  }
  if (!matched) {
    return condition;
  }
>>>>>>> upstream-releases

  Node thenExpr = assignExpr(InAllowed, yieldHandling, TripledotProhibited);
  if (!thenExpr) {
    return null();
  }

<<<<<<< HEAD
  if (!mustMatchToken(TokenKind::Colon, TokenStream::Operand,
                      JSMSG_COLON_IN_COND)) {
    return null();
  }
||||||| merged common ancestors
    MUST_MATCH_TOKEN_MOD(TokenKind::Colon, TokenStream::Operand, JSMSG_COLON_IN_COND);
=======
  if (!mustMatchToken(TokenKind::Colon, JSMSG_COLON_IN_COND)) {
    return null();
  }
>>>>>>> upstream-releases

  Node elseExpr = assignExpr(inHandling, yieldHandling, TripledotProhibited);
  if (!elseExpr) {
    return null();
  }

<<<<<<< HEAD
  return handler.newConditional(condition, thenExpr, elseExpr);
||||||| merged common ancestors
    return handler.newConditional(condition, thenExpr, elseExpr);
=======
  return handler_.newConditional(condition, thenExpr, elseExpr);
>>>>>>> upstream-releases
}

template <class ParseHandler, typename Unit>
<<<<<<< HEAD
typename ParseHandler::Node GeneralParser<ParseHandler, Unit>::assignExpr(
    InHandling inHandling, YieldHandling yieldHandling,
    TripledotHandling tripledotHandling,
    PossibleError* possibleError /* = nullptr */,
    InvokedPrediction invoked /* = PredictUninvoked */) {
  if (!CheckRecursionLimit(context)) {
    return null();
  }

  // It's very common at this point to have a "detectably simple" expression,
  // i.e. a name/number/string token followed by one of the following tokens
  // that obviously isn't part of an expression: , ; : ) ] }
  //
  // (In Parsemark this happens 81.4% of the time;  in code with large
  // numeric arrays, such as some Kraken benchmarks, it happens more often.)
  //
  // In such cases, we can avoid the full expression parsing route through
  // assignExpr(), condExpr(), orExpr(), unaryExpr(), memberExpr(), and
  // primaryExpr().

  TokenKind firstToken;
  if (!tokenStream.getToken(&firstToken, TokenStream::Operand)) {
    return null();
  }
||||||| merged common ancestors
typename ParseHandler::Node
GeneralParser<ParseHandler, Unit>::assignExpr(InHandling inHandling, YieldHandling yieldHandling,
                                              TripledotHandling tripledotHandling,
                                              PossibleError* possibleError /* = nullptr */,
                                              InvokedPrediction invoked /* = PredictUninvoked */)
{
    if (!CheckRecursionLimit(context)) {
        return null();
    }

    // It's very common at this point to have a "detectably simple" expression,
    // i.e. a name/number/string token followed by one of the following tokens
    // that obviously isn't part of an expression: , ; : ) ] }
    //
    // (In Parsemark this happens 81.4% of the time;  in code with large
    // numeric arrays, such as some Kraken benchmarks, it happens more often.)
    //
    // In such cases, we can avoid the full expression parsing route through
    // assignExpr(), condExpr(), orExpr(), unaryExpr(), memberExpr(), and
    // primaryExpr().

    TokenKind firstToken;
    if (!tokenStream.getToken(&firstToken, TokenStream::Operand)) {
        return null();
    }

    TokenPos exprPos = pos();
=======
typename ParseHandler::Node GeneralParser<ParseHandler, Unit>::assignExpr(
    InHandling inHandling, YieldHandling yieldHandling,
    TripledotHandling tripledotHandling,
    PossibleError* possibleError /* = nullptr */,
    InvokedPrediction invoked /* = PredictUninvoked */) {
  if (!CheckRecursionLimit(cx_)) {
    return null();
  }

  // It's very common at this point to have a "detectably simple" expression,
  // i.e. a name/number/string token followed by one of the following tokens
  // that obviously isn't part of an expression: , ; : ) ] }
  //
  // (In Parsemark this happens 81.4% of the time;  in code with large
  // numeric arrays, such as some Kraken benchmarks, it happens more often.)
  //
  // In such cases, we can avoid the full expression parsing route through
  // assignExpr(), condExpr(), orExpr(), unaryExpr(), memberExpr(), and
  // primaryExpr().

  TokenKind firstToken;
  if (!tokenStream.getToken(&firstToken, TokenStream::SlashIsRegExp)) {
    return null();
  }
>>>>>>> upstream-releases

  TokenPos exprPos = pos();

  bool endsExpr;

  // This only handles identifiers that *never* have special meaning anywhere
  // in the language.  Contextual keywords, reserved words in strict mode,
  // and other hard cases are handled outside this fast path.
  if (firstToken == TokenKind::Name) {
    if (!tokenStream.nextTokenEndsExpr(&endsExpr)) {
      return null();
    }
<<<<<<< HEAD
    if (endsExpr) {
      Rooted<PropertyName*> name(context, identifierReference(yieldHandling));
      if (!name) {
        return null();
      }
||||||| merged common ancestors
=======
    if (endsExpr) {
      Rooted<PropertyName*> name(cx_, identifierReference(yieldHandling));
      if (!name) {
        return null();
      }
>>>>>>> upstream-releases

      return identifierReference(name);
    }
  }

  if (firstToken == TokenKind::Number) {
    if (!tokenStream.nextTokenEndsExpr(&endsExpr)) {
      return null();
    }
    if (endsExpr) {
      return newNumber(anyChars.currentToken());
    }
  }

  if (firstToken == TokenKind::String) {
    if (!tokenStream.nextTokenEndsExpr(&endsExpr)) {
      return null();
    }
    if (endsExpr) {
      return stringLiteral();
    }
  }

<<<<<<< HEAD
  if (firstToken == TokenKind::Yield && yieldExpressionsSupported()) {
    return yieldExpression(inHandling);
  }
||||||| merged common ancestors
    anyChars.ungetToken();

    // Save the tokenizer state in case we find an arrow function and have to
    // rewind.
    Position start(keepAtoms, tokenStream);
=======
  if (firstToken == TokenKind::Yield && yieldExpressionsSupported()) {
    return yieldExpression(inHandling);
  }

  bool maybeAsyncArrow = false;
  if (firstToken == TokenKind::Async) {
    TokenKind nextSameLine = TokenKind::Eof;
    if (!tokenStream.peekTokenSameLine(&nextSameLine)) {
      return null();
    }
>>>>>>> upstream-releases

<<<<<<< HEAD
  bool maybeAsyncArrow = false;
  if (firstToken == TokenKind::Async) {
    TokenKind nextSameLine = TokenKind::Eof;
    if (!tokenStream.peekTokenSameLine(&nextSameLine)) {
      return null();
    }
||||||| merged common ancestors
    PossibleError possibleErrorInner(*this);
    Node lhs;
    TokenKind tokenAfterLHS;
    bool isArrow;
    if (maybeAsyncArrow) {
        tokenStream.consumeKnownToken(TokenKind::Async, TokenStream::Operand);

        TokenKind tokenAfterAsync;
        if (!tokenStream.getToken(&tokenAfterAsync)) {
            return null();
        }
        MOZ_ASSERT(TokenKindIsPossibleIdentifier(tokenAfterAsync));
=======
    if (TokenKindIsPossibleIdentifier(nextSameLine)) {
      maybeAsyncArrow = true;
    }
  }
>>>>>>> upstream-releases

<<<<<<< HEAD
    if (TokenKindIsPossibleIdentifier(nextSameLine)) {
      maybeAsyncArrow = true;
    }
  }
||||||| merged common ancestors
        // Check yield validity here.
        RootedPropertyName name(context, bindingIdentifier(yieldHandling));
        if (!name) {
            return null();
        }
=======
  anyChars.ungetToken();
>>>>>>> upstream-releases

<<<<<<< HEAD
  anyChars.ungetToken();
||||||| merged common ancestors
        if (!tokenStream.peekTokenSameLine(&tokenAfterLHS)) {
            return null();
        }
        if (tokenAfterLHS != TokenKind::Arrow) {
            error(JSMSG_UNEXPECTED_TOKEN,
                  "'=>' on the same line after an argument list", TokenKindToDesc(tokenAfterLHS));
            return null();
        }
=======
  // Save the tokenizer state in case we find an arrow function and have to
  // rewind.
  Position start(keepAtoms_, tokenStream);
>>>>>>> upstream-releases

<<<<<<< HEAD
  // Save the tokenizer state in case we find an arrow function and have to
  // rewind.
  Position start(keepAtoms, tokenStream);
||||||| merged common ancestors
        isArrow = true;
    } else {
        lhs = condExpr(inHandling, yieldHandling, tripledotHandling, &possibleErrorInner, invoked);
        if (!lhs) {
            return null();
        }
=======
  PossibleError possibleErrorInner(*this);
  Node lhs;
  TokenKind tokenAfterLHS;
  bool isArrow;
  if (maybeAsyncArrow) {
    tokenStream.consumeKnownToken(TokenKind::Async, TokenStream::SlashIsRegExp);
>>>>>>> upstream-releases

<<<<<<< HEAD
  PossibleError possibleErrorInner(*this);
  Node lhs;
  TokenKind tokenAfterLHS;
  bool isArrow;
  if (maybeAsyncArrow) {
    tokenStream.consumeKnownToken(TokenKind::Async, TokenStream::Operand);
||||||| merged common ancestors
        // Use Operand here because the ConditionalExpression parsed above
        // could be the entirety of this AssignmentExpression, and then ASI
        // permits this token to be a regular expression.
        if (!tokenStream.peekTokenSameLine(&tokenAfterLHS, TokenStream::Operand)) {
            return null();
        }
=======
    TokenKind tokenAfterAsync;
    if (!tokenStream.getToken(&tokenAfterAsync)) {
      return null();
    }
    MOZ_ASSERT(TokenKindIsPossibleIdentifier(tokenAfterAsync));
>>>>>>> upstream-releases

<<<<<<< HEAD
    TokenKind tokenAfterAsync;
    if (!tokenStream.getToken(&tokenAfterAsync)) {
      return null();
||||||| merged common ancestors
        isArrow = tokenAfterLHS == TokenKind::Arrow;
=======
    // Check yield validity here.
    RootedPropertyName name(cx_, bindingIdentifier(yieldHandling));
    if (!name) {
      return null();
>>>>>>> upstream-releases
    }
    MOZ_ASSERT(TokenKindIsPossibleIdentifier(tokenAfterAsync));

<<<<<<< HEAD
    // Check yield validity here.
    RootedPropertyName name(context, bindingIdentifier(yieldHandling));
    if (!name) {
      return null();
    }
||||||| merged common ancestors
    if (isArrow) {
        tokenStream.seek(start);

        TokenKind next;
        if (!tokenStream.getToken(&next, TokenStream::Operand)) {
            return null();
        }
        uint32_t toStringStart = pos().begin;
        anyChars.ungetToken();
=======
    if (!tokenStream.peekTokenSameLine(&tokenAfterLHS)) {
      return null();
    }
    if (tokenAfterLHS != TokenKind::Arrow) {
      error(JSMSG_UNEXPECTED_TOKEN,
            "'=>' on the same line after an argument list",
            TokenKindToDesc(tokenAfterLHS));
      return null();
    }

    isArrow = true;
  } else {
    lhs = condExpr(inHandling, yieldHandling, tripledotHandling,
                   &possibleErrorInner, invoked);
    if (!lhs) {
      return null();
    }
>>>>>>> upstream-releases

<<<<<<< HEAD
    if (!tokenStream.peekTokenSameLine(&tokenAfterLHS)) {
      return null();
    }
    if (tokenAfterLHS != TokenKind::Arrow) {
      error(JSMSG_UNEXPECTED_TOKEN,
            "'=>' on the same line after an argument list",
            TokenKindToDesc(tokenAfterLHS));
      return null();
    }
||||||| merged common ancestors
        FunctionAsyncKind asyncKind = FunctionAsyncKind::SyncFunction;
=======
    // Use SlashIsRegExp here because the ConditionalExpression parsed above
    // could be the entirety of this AssignmentExpression, and then ASI
    // permits this token to be a regular expression.
    if (!tokenStream.peekTokenSameLine(&tokenAfterLHS,
                                       TokenStream::SlashIsRegExp)) {
      return null();
    }
>>>>>>> upstream-releases

<<<<<<< HEAD
    isArrow = true;
  } else {
    lhs = condExpr(inHandling, yieldHandling, tripledotHandling,
                   &possibleErrorInner, invoked);
    if (!lhs) {
      return null();
    }
||||||| merged common ancestors
        if (next == TokenKind::Async) {
            tokenStream.consumeKnownToken(next, TokenStream::Operand);
=======
    isArrow = tokenAfterLHS == TokenKind::Arrow;
  }
>>>>>>> upstream-releases

<<<<<<< HEAD
    // Use Operand here because the ConditionalExpression parsed above
    // could be the entirety of this AssignmentExpression, and then ASI
    // permits this token to be a regular expression.
    if (!tokenStream.peekTokenSameLine(&tokenAfterLHS, TokenStream::Operand)) {
      return null();
    }
||||||| merged common ancestors
            TokenKind nextSameLine = TokenKind::Eof;
            if (!tokenStream.peekTokenSameLine(&nextSameLine)) {
                return null();
            }
=======
  if (isArrow) {
    tokenStream.seek(start);
>>>>>>> upstream-releases

<<<<<<< HEAD
    isArrow = tokenAfterLHS == TokenKind::Arrow;
  }
||||||| merged common ancestors
            // The AsyncArrowFunction production are
            //   async [no LineTerminator here] AsyncArrowBindingIdentifier ...
            //   async [no LineTerminator here] ArrowFormalParameters ...
            if (TokenKindIsPossibleIdentifier(nextSameLine) || nextSameLine == TokenKind::LeftParen) {
                asyncKind = FunctionAsyncKind::AsyncFunction;
            } else {
                anyChars.ungetToken();
            }
        }
=======
    TokenKind next;
    if (!tokenStream.getToken(&next, TokenStream::SlashIsRegExp)) {
      return null();
    }
    TokenPos startPos = pos();
    uint32_t toStringStart = startPos.begin;
    anyChars.ungetToken();
>>>>>>> upstream-releases

<<<<<<< HEAD
  if (isArrow) {
    tokenStream.seek(start);
||||||| merged common ancestors
        CodeNodeType funNode = handler.newArrowFunction(pos());
        if (!funNode) {
            return null();
        }
=======
    FunctionAsyncKind asyncKind = FunctionAsyncKind::SyncFunction;
>>>>>>> upstream-releases

<<<<<<< HEAD
    TokenKind next;
    if (!tokenStream.getToken(&next, TokenStream::Operand)) {
      return null();
    }
    uint32_t toStringStart = pos().begin;
    anyChars.ungetToken();

    FunctionAsyncKind asyncKind = FunctionAsyncKind::SyncFunction;

    if (next == TokenKind::Async) {
      tokenStream.consumeKnownToken(next, TokenStream::Operand);
||||||| merged common ancestors
        return functionDefinition(funNode, toStringStart, inHandling, yieldHandling, nullptr,
                                  FunctionSyntaxKind::Arrow, GeneratorKind::NotGenerator,
                                  asyncKind);
    }

    MOZ_ALWAYS_TRUE(tokenStream.getToken(&tokenAfterLHS, TokenStream::Operand));

    ParseNodeKind kind;
    switch (tokenAfterLHS) {
      case TokenKind::Assign:       kind = ParseNodeKind::Assign;       break;
      case TokenKind::AddAssign:    kind = ParseNodeKind::AddAssign;    break;
      case TokenKind::SubAssign:    kind = ParseNodeKind::SubAssign;    break;
      case TokenKind::BitOrAssign:  kind = ParseNodeKind::BitOrAssign;  break;
      case TokenKind::BitXorAssign: kind = ParseNodeKind::BitXorAssign; break;
      case TokenKind::BitAndAssign: kind = ParseNodeKind::BitAndAssign; break;
      case TokenKind::LshAssign:    kind = ParseNodeKind::LshAssign;    break;
      case TokenKind::RshAssign:    kind = ParseNodeKind::RshAssign;    break;
      case TokenKind::UrshAssign:   kind = ParseNodeKind::UrshAssign;   break;
      case TokenKind::MulAssign:    kind = ParseNodeKind::MulAssign;    break;
      case TokenKind::DivAssign:    kind = ParseNodeKind::DivAssign;    break;
      case TokenKind::ModAssign:    kind = ParseNodeKind::ModAssign;    break;
      case TokenKind::PowAssign:    kind = ParseNodeKind::PowAssign;    break;
=======
    if (next == TokenKind::Async) {
      tokenStream.consumeKnownToken(next, TokenStream::SlashIsRegExp);
>>>>>>> upstream-releases

      TokenKind nextSameLine = TokenKind::Eof;
      if (!tokenStream.peekTokenSameLine(&nextSameLine)) {
        return null();
      }

      // The AsyncArrowFunction production are
      //   async [no LineTerminator here] AsyncArrowBindingIdentifier ...
      //   async [no LineTerminator here] ArrowFormalParameters ...
      if (TokenKindIsPossibleIdentifier(nextSameLine) ||
          nextSameLine == TokenKind::LeftParen) {
        asyncKind = FunctionAsyncKind::AsyncFunction;
      } else {
        anyChars.ungetToken();
      }
    }

<<<<<<< HEAD
    CodeNodeType funNode = handler.newArrowFunction(pos());
    if (!funNode) {
      return null();
    }

    return functionDefinition(funNode, toStringStart, inHandling, yieldHandling,
                              nullptr, FunctionSyntaxKind::Arrow,
                              GeneratorKind::NotGenerator, asyncKind);
  }

  MOZ_ALWAYS_TRUE(tokenStream.getToken(&tokenAfterLHS, TokenStream::Operand));

  ParseNodeKind kind;
  switch (tokenAfterLHS) {
    case TokenKind::Assign:
      kind = ParseNodeKind::Assign;
      break;
    case TokenKind::AddAssign:
      kind = ParseNodeKind::AddAssign;
      break;
    case TokenKind::SubAssign:
      kind = ParseNodeKind::SubAssign;
      break;
    case TokenKind::BitOrAssign:
      kind = ParseNodeKind::BitOrAssign;
      break;
    case TokenKind::BitXorAssign:
      kind = ParseNodeKind::BitXorAssign;
      break;
    case TokenKind::BitAndAssign:
      kind = ParseNodeKind::BitAndAssign;
      break;
    case TokenKind::LshAssign:
      kind = ParseNodeKind::LshAssign;
      break;
    case TokenKind::RshAssign:
      kind = ParseNodeKind::RshAssign;
      break;
    case TokenKind::UrshAssign:
      kind = ParseNodeKind::UrshAssign;
      break;
    case TokenKind::MulAssign:
      kind = ParseNodeKind::MulAssign;
      break;
    case TokenKind::DivAssign:
      kind = ParseNodeKind::DivAssign;
      break;
    case TokenKind::ModAssign:
      kind = ParseNodeKind::ModAssign;
      break;
    case TokenKind::PowAssign:
      kind = ParseNodeKind::PowAssign;
      break;

    default:
      MOZ_ASSERT(!anyChars.isCurrentTokenAssignment());
      if (!possibleError) {
        if (!possibleErrorInner.checkForExpressionError()) {
          return null();
        }
      } else {
        possibleErrorInner.transferErrorsTo(possibleError);
      }

      anyChars.ungetToken();
      return lhs;
  }
||||||| merged common ancestors
    // Verify the left-hand side expression doesn't have a forbidden form.
    if (handler.isUnparenthesizedDestructuringPattern(lhs)) {
        if (kind != ParseNodeKind::Assign) {
            error(JSMSG_BAD_DESTRUCT_ASS);
            return null();
        }

        if (!possibleErrorInner.checkForDestructuringErrorOrWarning()) {
            return null();
        }
    } else if (handler.isName(lhs)) {
        if (const char* chars = nameIsArgumentsOrEval(lhs)) {
            // |chars| is "arguments" or "eval" here.
            if (!strictModeErrorAt(exprPos.begin, JSMSG_BAD_STRICT_ASSIGN, chars)) {
                return null();
            }
        }
=======
    FunctionSyntaxKind syntaxKind = FunctionSyntaxKind::Arrow;
    FunctionNodeType funNode = handler_.newFunction(syntaxKind, startPos);
    if (!funNode) {
      return null();
    }

    return functionDefinition(funNode, toStringStart, inHandling, yieldHandling,
                              nullptr, syntaxKind, GeneratorKind::NotGenerator,
                              asyncKind);
  }

  MOZ_ALWAYS_TRUE(
      tokenStream.getToken(&tokenAfterLHS, TokenStream::SlashIsRegExp));

  ParseNodeKind kind;
  switch (tokenAfterLHS) {
    case TokenKind::Assign:
      kind = ParseNodeKind::AssignExpr;
      break;
    case TokenKind::AddAssign:
      kind = ParseNodeKind::AddAssignExpr;
      break;
    case TokenKind::SubAssign:
      kind = ParseNodeKind::SubAssignExpr;
      break;
    case TokenKind::BitOrAssign:
      kind = ParseNodeKind::BitOrAssignExpr;
      break;
    case TokenKind::BitXorAssign:
      kind = ParseNodeKind::BitXorAssignExpr;
      break;
    case TokenKind::BitAndAssign:
      kind = ParseNodeKind::BitAndAssignExpr;
      break;
    case TokenKind::LshAssign:
      kind = ParseNodeKind::LshAssignExpr;
      break;
    case TokenKind::RshAssign:
      kind = ParseNodeKind::RshAssignExpr;
      break;
    case TokenKind::UrshAssign:
      kind = ParseNodeKind::UrshAssignExpr;
      break;
    case TokenKind::MulAssign:
      kind = ParseNodeKind::MulAssignExpr;
      break;
    case TokenKind::DivAssign:
      kind = ParseNodeKind::DivAssignExpr;
      break;
    case TokenKind::ModAssign:
      kind = ParseNodeKind::ModAssignExpr;
      break;
    case TokenKind::PowAssign:
      kind = ParseNodeKind::PowAssignExpr;
      break;

    default:
      MOZ_ASSERT(!anyChars.isCurrentTokenAssignment());
      if (!possibleError) {
        if (!possibleErrorInner.checkForExpressionError()) {
          return null();
        }
      } else {
        possibleErrorInner.transferErrorsTo(possibleError);
      }
>>>>>>> upstream-releases

<<<<<<< HEAD
  // Verify the left-hand side expression doesn't have a forbidden form.
  if (handler.isUnparenthesizedDestructuringPattern(lhs)) {
    if (kind != ParseNodeKind::Assign) {
      error(JSMSG_BAD_DESTRUCT_ASS);
      return null();
    }
||||||| merged common ancestors
        handler.adjustGetToSet(lhs);
    } else if (handler.isPropertyAccess(lhs)) {
        // Permitted: no additional testing/fixup needed.
    } else if (handler.isFunctionCall(lhs)) {
        if (!strictModeErrorAt(exprPos.begin, JSMSG_BAD_LEFTSIDE_OF_ASS)) {
            return null();
        }
=======
      anyChars.ungetToken();
      return lhs;
  }
>>>>>>> upstream-releases

<<<<<<< HEAD
    if (!possibleErrorInner.checkForDestructuringErrorOrWarning()) {
      return null();
    }
  } else if (handler.isName(lhs)) {
    if (const char* chars = nameIsArgumentsOrEval(lhs)) {
      // |chars| is "arguments" or "eval" here.
      if (!strictModeErrorAt(exprPos.begin, JSMSG_BAD_STRICT_ASSIGN, chars)) {
        return null();
      }
||||||| merged common ancestors
        if (possibleError) {
            possibleError->setPendingDestructuringErrorAt(exprPos, JSMSG_BAD_DESTRUCT_TARGET);
        }
    } else {
        errorAt(exprPos.begin, JSMSG_BAD_LEFTSIDE_OF_ASS);
        return null();
=======
  // Verify the left-hand side expression doesn't have a forbidden form.
  if (handler_.isUnparenthesizedDestructuringPattern(lhs)) {
    if (kind != ParseNodeKind::AssignExpr) {
      error(JSMSG_BAD_DESTRUCT_ASS);
      return null();
>>>>>>> upstream-releases
    }

<<<<<<< HEAD
    handler.adjustGetToSet(lhs);
  } else if (handler.isPropertyAccess(lhs)) {
    // Permitted: no additional testing/fixup needed.
  } else if (handler.isFunctionCall(lhs)) {
    if (!strictModeErrorAt(exprPos.begin, JSMSG_BAD_LEFTSIDE_OF_ASS)) {
      return null();
||||||| merged common ancestors
    if (!possibleErrorInner.checkForExpressionError()) {
        return null();
=======
    if (!possibleErrorInner.checkForDestructuringErrorOrWarning()) {
      return null();
    }
  } else if (handler_.isName(lhs)) {
    if (const char* chars = nameIsArgumentsOrEval(lhs)) {
      // |chars| is "arguments" or "eval" here.
      if (!strictModeErrorAt(exprPos.begin, JSMSG_BAD_STRICT_ASSIGN, chars)) {
        return null();
      }
    }
  } else if (handler_.isPropertyAccess(lhs)) {
    // Permitted: no additional testing/fixup needed.
  } else if (handler_.isFunctionCall(lhs)) {
    if (!strictModeErrorAt(exprPos.begin, JSMSG_BAD_LEFTSIDE_OF_ASS)) {
      return null();
>>>>>>> upstream-releases
    }

    if (possibleError) {
      possibleError->setPendingDestructuringErrorAt(exprPos,
                                                    JSMSG_BAD_DESTRUCT_TARGET);
    }
<<<<<<< HEAD
  } else {
    errorAt(exprPos.begin, JSMSG_BAD_LEFTSIDE_OF_ASS);
    return null();
  }

  if (!possibleErrorInner.checkForExpressionError()) {
    return null();
  }

  Node rhs = assignExpr(inHandling, yieldHandling, TripledotProhibited);
  if (!rhs) {
    return null();
  }
||||||| merged common ancestors
=======
  } else {
    errorAt(exprPos.begin, JSMSG_BAD_LEFTSIDE_OF_ASS);
    return null();
  }

  if (!possibleErrorInner.checkForExpressionError()) {
    return null();
  }
>>>>>>> upstream-releases

<<<<<<< HEAD
  return handler.newAssignment(kind, lhs, rhs);
||||||| merged common ancestors
    return handler.newAssignment(kind, lhs, rhs);
=======
  Node rhs = assignExpr(inHandling, yieldHandling, TripledotProhibited);
  if (!rhs) {
    return null();
  }

  return handler_.newAssignment(kind, lhs, rhs);
>>>>>>> upstream-releases
}

template <class ParseHandler>
bool PerHandlerParser<ParseHandler>::isValidSimpleAssignmentTarget(
    Node node,
    FunctionCallBehavior behavior /* = ForbidAssignmentToFunctionCalls */) {
  // Note that this method implements *only* a boolean test.  Reporting an
  // error for the various syntaxes that fail this, and warning for the
  // various syntaxes that "pass" this but should not, occurs elsewhere.

<<<<<<< HEAD
  if (handler.isName(node)) {
    if (!pc->sc()->strict()) {
      return true;
||||||| merged common ancestors
        return !nameIsArgumentsOrEval(node);
=======
  if (handler_.isName(node)) {
    if (!pc_->sc()->strict()) {
      return true;
>>>>>>> upstream-releases
    }

    return !nameIsArgumentsOrEval(node);
  }

<<<<<<< HEAD
  if (handler.isPropertyAccess(node)) {
    return true;
  }

  if (behavior == PermitAssignmentToFunctionCalls) {
    if (handler.isFunctionCall(node)) {
      return true;
||||||| merged common ancestors
    if (behavior == PermitAssignmentToFunctionCalls) {
        if (handler.isFunctionCall(node)) {
            return true;
        }
=======
  if (handler_.isPropertyAccess(node)) {
    return true;
  }

  if (behavior == PermitAssignmentToFunctionCalls) {
    if (handler_.isFunctionCall(node)) {
      return true;
>>>>>>> upstream-releases
    }
  }

  return false;
}

template <class ParseHandler>
<<<<<<< HEAD
const char* PerHandlerParser<ParseHandler>::nameIsArgumentsOrEval(Node node) {
  MOZ_ASSERT(handler.isName(node),
             "must only call this function on known names");
||||||| merged common ancestors
const char*
PerHandlerParser<ParseHandler>::nameIsArgumentsOrEval(Node node)
{
    MOZ_ASSERT(handler.isName(node), "must only call this function on known names");
=======
const char* PerHandlerParser<ParseHandler>::nameIsArgumentsOrEval(Node node) {
  MOZ_ASSERT(handler_.isName(node),
             "must only call this function on known names");
>>>>>>> upstream-releases

<<<<<<< HEAD
  if (handler.isEvalName(node, context)) {
    return js_eval_str;
  }
  if (handler.isArgumentsName(node, context)) {
    return js_arguments_str;
  }
  return nullptr;
||||||| merged common ancestors
    if (handler.isEvalName(node, context)) {
        return js_eval_str;
    }
    if (handler.isArgumentsName(node, context)) {
        return js_arguments_str;
    }
    return nullptr;
=======
  if (handler_.isEvalName(node, cx_)) {
    return js_eval_str;
  }
  if (handler_.isArgumentsName(node, cx_)) {
    return js_arguments_str;
  }
  return nullptr;
>>>>>>> upstream-releases
}

template <class ParseHandler, typename Unit>
<<<<<<< HEAD
bool GeneralParser<ParseHandler, Unit>::checkIncDecOperand(
    Node operand, uint32_t operandOffset) {
  if (handler.isName(operand)) {
    if (const char* chars = nameIsArgumentsOrEval(operand)) {
      if (!strictModeErrorAt(operandOffset, JSMSG_BAD_STRICT_ASSIGN, chars)) {
||||||| merged common ancestors
bool
GeneralParser<ParseHandler, Unit>::checkIncDecOperand(Node operand, uint32_t operandOffset)
{
    if (handler.isName(operand)) {
        if (const char* chars = nameIsArgumentsOrEval(operand)) {
            if (!strictModeErrorAt(operandOffset, JSMSG_BAD_STRICT_ASSIGN, chars)) {
                return false;
            }
        }
    } else if (handler.isPropertyAccess(operand)) {
        // Permitted: no additional testing/fixup needed.
    } else if (handler.isFunctionCall(operand)) {
        // Assignment to function calls is forbidden in ES6.  We're still
        // somewhat concerned about sites using this in dead code, so forbid it
        // only in strict mode code (or if the werror option has been set), and
        // otherwise warn.
        if (!strictModeErrorAt(operandOffset, JSMSG_BAD_INCOP_OPERAND)) {
            return false;
        }
    } else {
        errorAt(operandOffset, JSMSG_BAD_INCOP_OPERAND);
=======
bool GeneralParser<ParseHandler, Unit>::checkIncDecOperand(
    Node operand, uint32_t operandOffset) {
  if (handler_.isName(operand)) {
    if (const char* chars = nameIsArgumentsOrEval(operand)) {
      if (!strictModeErrorAt(operandOffset, JSMSG_BAD_STRICT_ASSIGN, chars)) {
>>>>>>> upstream-releases
        return false;
      }
    }
<<<<<<< HEAD
  } else if (handler.isPropertyAccess(operand)) {
    // Permitted: no additional testing/fixup needed.
  } else if (handler.isFunctionCall(operand)) {
    // Assignment to function calls is forbidden in ES6.  We're still
    // somewhat concerned about sites using this in dead code, so forbid it
    // only in strict mode code (or if the werror option has been set), and
    // otherwise warn.
    if (!strictModeErrorAt(operandOffset, JSMSG_BAD_INCOP_OPERAND)) {
      return false;
    }
  } else {
    errorAt(operandOffset, JSMSG_BAD_INCOP_OPERAND);
    return false;
  }
||||||| merged common ancestors
=======
  } else if (handler_.isPropertyAccess(operand)) {
    // Permitted: no additional testing/fixup needed.
  } else if (handler_.isFunctionCall(operand)) {
    // Assignment to function calls is forbidden in ES6.  We're still
    // somewhat concerned about sites using this in dead code, so forbid it
    // only in strict mode code (or if the werror option has been set), and
    // otherwise warn.
    if (!strictModeErrorAt(operandOffset, JSMSG_BAD_INCOP_OPERAND)) {
      return false;
    }
  } else {
    errorAt(operandOffset, JSMSG_BAD_INCOP_OPERAND);
    return false;
  }
>>>>>>> upstream-releases

  MOZ_ASSERT(
      isValidSimpleAssignmentTarget(operand, PermitAssignmentToFunctionCalls),
      "inconsistent increment/decrement operand validation");
  return true;
}

template <class ParseHandler, typename Unit>
typename ParseHandler::UnaryNodeType
<<<<<<< HEAD
GeneralParser<ParseHandler, Unit>::unaryOpExpr(YieldHandling yieldHandling,
                                               ParseNodeKind kind,
                                               uint32_t begin) {
  Node kid = unaryExpr(yieldHandling, TripledotProhibited);
  if (!kid) {
    return null();
  }
  return handler.newUnary(kind, begin, kid);
||||||| merged common ancestors
GeneralParser<ParseHandler, Unit>::unaryOpExpr(YieldHandling yieldHandling, ParseNodeKind kind,
                                                uint32_t begin)
{
    Node kid = unaryExpr(yieldHandling, TripledotProhibited);
    if (!kid) {
        return null();
    }
    return handler.newUnary(kind, begin, kid);
=======
GeneralParser<ParseHandler, Unit>::unaryOpExpr(YieldHandling yieldHandling,
                                               ParseNodeKind kind,
                                               uint32_t begin) {
  Node kid = unaryExpr(yieldHandling, TripledotProhibited);
  if (!kid) {
    return null();
  }
  return handler_.newUnary(kind, begin, kid);
>>>>>>> upstream-releases
}

template <class ParseHandler, typename Unit>
<<<<<<< HEAD
typename ParseHandler::Node GeneralParser<ParseHandler, Unit>::unaryExpr(
    YieldHandling yieldHandling, TripledotHandling tripledotHandling,
    PossibleError* possibleError /* = nullptr */,
    InvokedPrediction invoked /* = PredictUninvoked */) {
  if (!CheckRecursionLimit(context)) {
    return null();
  }

  TokenKind tt;
  if (!tokenStream.getToken(&tt, TokenStream::Operand)) {
    return null();
  }
  uint32_t begin = pos().begin;
  switch (tt) {
    case TokenKind::Void:
      return unaryOpExpr(yieldHandling, ParseNodeKind::Void, begin);
    case TokenKind::Not:
      return unaryOpExpr(yieldHandling, ParseNodeKind::Not, begin);
    case TokenKind::BitNot:
      return unaryOpExpr(yieldHandling, ParseNodeKind::BitNot, begin);
    case TokenKind::Add:
      return unaryOpExpr(yieldHandling, ParseNodeKind::Pos, begin);
    case TokenKind::Sub:
      return unaryOpExpr(yieldHandling, ParseNodeKind::Neg, begin);

    case TokenKind::TypeOf: {
      // The |typeof| operator is specially parsed to distinguish its
      // application to a name, from its application to a non-name
      // expression:
      //
      //   // Looks up the name, doesn't find it and so evaluates to
      //   // "undefined".
      //   assertEq(typeof nonExistentName, "undefined");
      //
      //   // Evaluates expression, triggering a runtime ReferenceError for
      //   // the undefined name.
      //   typeof (1, nonExistentName);
      Node kid = unaryExpr(yieldHandling, TripledotProhibited);
      if (!kid) {
||||||| merged common ancestors
typename ParseHandler::Node
GeneralParser<ParseHandler, Unit>::unaryExpr(YieldHandling yieldHandling,
                                             TripledotHandling tripledotHandling,
                                             PossibleError* possibleError /* = nullptr */,
                                             InvokedPrediction invoked /* = PredictUninvoked */)
{
    if (!CheckRecursionLimit(context)) {
=======
typename ParseHandler::Node GeneralParser<ParseHandler, Unit>::unaryExpr(
    YieldHandling yieldHandling, TripledotHandling tripledotHandling,
    PossibleError* possibleError /* = nullptr */,
    InvokedPrediction invoked /* = PredictUninvoked */) {
  if (!CheckRecursionLimit(cx_)) {
    return null();
  }

  TokenKind tt;
  if (!tokenStream.getToken(&tt, TokenStream::SlashIsRegExp)) {
    return null();
  }
  uint32_t begin = pos().begin;
  switch (tt) {
    case TokenKind::Void:
      return unaryOpExpr(yieldHandling, ParseNodeKind::VoidExpr, begin);
    case TokenKind::Not:
      return unaryOpExpr(yieldHandling, ParseNodeKind::NotExpr, begin);
    case TokenKind::BitNot:
      return unaryOpExpr(yieldHandling, ParseNodeKind::BitNotExpr, begin);
    case TokenKind::Add:
      return unaryOpExpr(yieldHandling, ParseNodeKind::PosExpr, begin);
    case TokenKind::Sub:
      return unaryOpExpr(yieldHandling, ParseNodeKind::NegExpr, begin);

    case TokenKind::TypeOf: {
      // The |typeof| operator is specially parsed to distinguish its
      // application to a name, from its application to a non-name
      // expression:
      //
      //   // Looks up the name, doesn't find it and so evaluates to
      //   // "undefined".
      //   assertEq(typeof nonExistentName, "undefined");
      //
      //   // Evaluates expression, triggering a runtime ReferenceError for
      //   // the undefined name.
      //   typeof (1, nonExistentName);
      Node kid = unaryExpr(yieldHandling, TripledotProhibited);
      if (!kid) {
>>>>>>> upstream-releases
        return null();
<<<<<<< HEAD
      }

      return handler.newTypeof(begin, kid);
||||||| merged common ancestors
=======
      }

      return handler_.newTypeof(begin, kid);
>>>>>>> upstream-releases
    }

<<<<<<< HEAD
    case TokenKind::Inc:
    case TokenKind::Dec: {
      TokenKind tt2;
      if (!tokenStream.getToken(&tt2, TokenStream::Operand)) {
||||||| merged common ancestors
    TokenKind tt;
    if (!tokenStream.getToken(&tt, TokenStream::Operand)) {
=======
    case TokenKind::Inc:
    case TokenKind::Dec: {
      TokenKind tt2;
      if (!tokenStream.getToken(&tt2, TokenStream::SlashIsRegExp)) {
>>>>>>> upstream-releases
        return null();
      }

      uint32_t operandOffset = pos().begin;
      Node operand = memberExpr(yieldHandling, TripledotProhibited, tt2);
      if (!operand || !checkIncDecOperand(operand, operandOffset)) {
        return null();
      }
<<<<<<< HEAD
      ParseNodeKind pnk = (tt == TokenKind::Inc) ? ParseNodeKind::PreIncrement
                                                 : ParseNodeKind::PreDecrement;
      return handler.newUpdate(pnk, begin, operand);
    }
||||||| merged common ancestors
=======
      ParseNodeKind pnk = (tt == TokenKind::Inc)
                              ? ParseNodeKind::PreIncrementExpr
                              : ParseNodeKind::PreDecrementExpr;
      return handler_.newUpdate(pnk, begin, operand);
    }
>>>>>>> upstream-releases

<<<<<<< HEAD
    case TokenKind::Delete: {
      uint32_t exprOffset;
      if (!tokenStream.peekOffset(&exprOffset, TokenStream::Operand)) {
        return null();
      }
||||||| merged common ancestors
      case TokenKind::Inc:
      case TokenKind::Dec:
      {
        TokenKind tt2;
        if (!tokenStream.getToken(&tt2, TokenStream::Operand)) {
            return null();
        }
=======
    case TokenKind::Delete: {
      uint32_t exprOffset;
      if (!tokenStream.peekOffset(&exprOffset, TokenStream::SlashIsRegExp)) {
        return null();
      }
>>>>>>> upstream-releases

      Node expr = unaryExpr(yieldHandling, TripledotProhibited);
      if (!expr) {
        return null();
      }

<<<<<<< HEAD
      // Per spec, deleting any unary expression is valid -- it simply
      // returns true -- except for one case that is illegal in strict mode.
      if (handler.isName(expr)) {
        if (!strictModeErrorAt(exprOffset, JSMSG_DEPRECATED_DELETE_OPERAND)) {
          return null();
||||||| merged common ancestors
      case TokenKind::Delete: {
        uint32_t exprOffset;
        if (!tokenStream.peekOffset(&exprOffset, TokenStream::Operand)) {
            return null();
=======
      // Per spec, deleting any unary expression is valid -- it simply
      // returns true -- except for one case that is illegal in strict mode.
      if (handler_.isName(expr)) {
        if (!strictModeErrorAt(exprOffset, JSMSG_DEPRECATED_DELETE_OPERAND)) {
          return null();
>>>>>>> upstream-releases
        }

<<<<<<< HEAD
        pc->sc()->setBindingsAccessedDynamically();
      }
||||||| merged common ancestors
        Node expr = unaryExpr(yieldHandling, TripledotProhibited);
        if (!expr) {
            return null();
        }
=======
        pc_->sc()->setBindingsAccessedDynamically();
      }
>>>>>>> upstream-releases

<<<<<<< HEAD
      return handler.newDelete(begin, expr);
    }
||||||| merged common ancestors
        // Per spec, deleting any unary expression is valid -- it simply
        // returns true -- except for one case that is illegal in strict mode.
        if (handler.isName(expr)) {
            if (!strictModeErrorAt(exprOffset, JSMSG_DEPRECATED_DELETE_OPERAND)) {
                return null();
            }
=======
      return handler_.newDelete(begin, expr);
    }
>>>>>>> upstream-releases

<<<<<<< HEAD
    case TokenKind::Await: {
      if (pc->isAsync()) {
        if (inParametersOfAsyncFunction()) {
          error(JSMSG_AWAIT_IN_PARAMETER);
          return null();
||||||| merged common ancestors
            pc->sc()->setBindingsAccessedDynamically();
=======
    case TokenKind::Await: {
      if (pc_->isAsync()) {
        if (inParametersOfAsyncFunction()) {
          error(JSMSG_AWAIT_IN_PARAMETER);
          return null();
>>>>>>> upstream-releases
        }
        Node kid =
            unaryExpr(yieldHandling, tripledotHandling, possibleError, invoked);
        if (!kid) {
          return null();
        }
<<<<<<< HEAD
        pc->lastAwaitOffset = begin;
        return handler.newAwaitExpression(begin, kid);
||||||| merged common ancestors
=======
        pc_->lastAwaitOffset = begin;
        return handler_.newAwaitExpression(begin, kid);
>>>>>>> upstream-releases
      }
    }

      MOZ_FALLTHROUGH;

    default: {
      Node expr =
          memberExpr(yieldHandling, tripledotHandling, tt,
                     /* allowCallSyntax = */ true, possibleError, invoked);
      if (!expr) {
        return null();
      }

      /* Don't look across a newline boundary for a postfix incop. */
      if (!tokenStream.peekTokenSameLine(&tt)) {
        return null();
      }

      if (tt != TokenKind::Inc && tt != TokenKind::Dec) {
        return expr;
      }

      tokenStream.consumeKnownToken(tt);
      if (!checkIncDecOperand(expr, begin)) {
        return null();
      }
<<<<<<< HEAD

      ParseNodeKind pnk = (tt == TokenKind::Inc) ? ParseNodeKind::PostIncrement
                                                 : ParseNodeKind::PostDecrement;
      return handler.newUpdate(pnk, begin, expr);
||||||| merged common ancestors
=======

      ParseNodeKind pnk = (tt == TokenKind::Inc)
                              ? ParseNodeKind::PostIncrementExpr
                              : ParseNodeKind::PostDecrementExpr;
      return handler_.newUpdate(pnk, begin, expr);
>>>>>>> upstream-releases
    }
  }
}

template <class ParseHandler, typename Unit>
typename ParseHandler::Node
<<<<<<< HEAD
GeneralParser<ParseHandler, Unit>::assignExprWithoutYieldOrAwait(
    YieldHandling yieldHandling) {
  uint32_t startYieldOffset = pc->lastYieldOffset;
  uint32_t startAwaitOffset = pc->lastAwaitOffset;
  Node res = assignExpr(InAllowed, yieldHandling, TripledotProhibited);
  if (res) {
    if (pc->lastYieldOffset != startYieldOffset) {
      errorAt(pc->lastYieldOffset, JSMSG_YIELD_IN_PARAMETER);
      return null();
    }
    if (pc->lastAwaitOffset != startAwaitOffset) {
      errorAt(pc->lastAwaitOffset, JSMSG_AWAIT_IN_PARAMETER);
      return null();
||||||| merged common ancestors
GeneralParser<ParseHandler, Unit>::assignExprWithoutYieldOrAwait(YieldHandling yieldHandling)
{
    uint32_t startYieldOffset = pc->lastYieldOffset;
    uint32_t startAwaitOffset = pc->lastAwaitOffset;
    Node res = assignExpr(InAllowed, yieldHandling, TripledotProhibited);
    if (res) {
        if (pc->lastYieldOffset != startYieldOffset) {
            errorAt(pc->lastYieldOffset, JSMSG_YIELD_IN_PARAMETER);
            return null();
        }
        if (pc->lastAwaitOffset != startAwaitOffset) {
            errorAt(pc->lastAwaitOffset, JSMSG_AWAIT_IN_PARAMETER);
            return null();
        }
=======
GeneralParser<ParseHandler, Unit>::assignExprWithoutYieldOrAwait(
    YieldHandling yieldHandling) {
  uint32_t startYieldOffset = pc_->lastYieldOffset;
  uint32_t startAwaitOffset = pc_->lastAwaitOffset;
  Node res = assignExpr(InAllowed, yieldHandling, TripledotProhibited);
  if (res) {
    if (pc_->lastYieldOffset != startYieldOffset) {
      errorAt(pc_->lastYieldOffset, JSMSG_YIELD_IN_PARAMETER);
      return null();
    }
    if (pc_->lastAwaitOffset != startAwaitOffset) {
      errorAt(pc_->lastAwaitOffset, JSMSG_AWAIT_IN_PARAMETER);
      return null();
>>>>>>> upstream-releases
    }
  }
  return res;
}

template <class ParseHandler, typename Unit>
typename ParseHandler::ListNodeType
<<<<<<< HEAD
GeneralParser<ParseHandler, Unit>::argumentList(
    YieldHandling yieldHandling, bool* isSpread,
    PossibleError* possibleError /* = nullptr */) {
  ListNodeType argsList = handler.newArguments(pos());
  if (!argsList) {
    return null();
  }
||||||| merged common ancestors
GeneralParser<ParseHandler, Unit>::argumentList(YieldHandling yieldHandling, bool* isSpread,
                                                PossibleError* possibleError /* = nullptr */)
{
    ListNodeType argsList = handler.newArguments(pos());
    if (!argsList) {
        return null();
    }
=======
GeneralParser<ParseHandler, Unit>::argumentList(
    YieldHandling yieldHandling, bool* isSpread,
    PossibleError* possibleError /* = nullptr */) {
  ListNodeType argsList = handler_.newArguments(pos());
  if (!argsList) {
    return null();
  }
>>>>>>> upstream-releases

<<<<<<< HEAD
  bool matched;
  if (!tokenStream.matchToken(&matched, TokenKind::RightParen,
                              TokenStream::Operand)) {
    return null();
  }
  if (matched) {
    handler.setEndPosition(argsList, pos().end);
    return argsList;
  }

  while (true) {
    bool spread = false;
    uint32_t begin = 0;
    if (!tokenStream.matchToken(&matched, TokenKind::TripleDot,
                                TokenStream::Operand)) {
      return null();
||||||| merged common ancestors
    bool matched;
    if (!tokenStream.matchToken(&matched, TokenKind::RightParen, TokenStream::Operand)) {
        return null();
=======
  bool matched;
  if (!tokenStream.matchToken(&matched, TokenKind::RightParen,
                              TokenStream::SlashIsRegExp)) {
    return null();
  }
  if (matched) {
    handler_.setEndPosition(argsList, pos().end);
    return argsList;
  }

  while (true) {
    bool spread = false;
    uint32_t begin = 0;
    if (!tokenStream.matchToken(&matched, TokenKind::TripleDot,
                                TokenStream::SlashIsRegExp)) {
      return null();
>>>>>>> upstream-releases
    }
    if (matched) {
      spread = true;
      begin = pos().begin;
      *isSpread = true;
    }

<<<<<<< HEAD
    Node argNode = assignExpr(InAllowed, yieldHandling, TripledotProhibited,
                              possibleError);
    if (!argNode) {
      return null();
    }
    if (spread) {
      argNode = handler.newSpread(begin, argNode);
      if (!argNode) {
        return null();
      }
    }
||||||| merged common ancestors
    while (true) {
        bool spread = false;
        uint32_t begin = 0;
        if (!tokenStream.matchToken(&matched, TokenKind::TripleDot, TokenStream::Operand)) {
            return null();
        }
        if (matched) {
            spread = true;
            begin = pos().begin;
            *isSpread = true;
        }

        Node argNode = assignExpr(InAllowed, yieldHandling, TripledotProhibited, possibleError);
        if (!argNode) {
            return null();
        }
        if (spread) {
            argNode = handler.newSpread(begin, argNode);
            if (!argNode) {
                return null();
            }
        }
=======
    Node argNode = assignExpr(InAllowed, yieldHandling, TripledotProhibited,
                              possibleError);
    if (!argNode) {
      return null();
    }
    if (spread) {
      argNode = handler_.newSpread(begin, argNode);
      if (!argNode) {
        return null();
      }
    }
>>>>>>> upstream-releases

<<<<<<< HEAD
    handler.addList(argsList, argNode);
||||||| merged common ancestors
        handler.addList(argsList, argNode);
=======
    handler_.addList(argsList, argNode);
>>>>>>> upstream-releases

<<<<<<< HEAD
    bool matched;
    if (!tokenStream.matchToken(&matched, TokenKind::Comma,
                                TokenStream::Operand)) {
      return null();
    }
    if (!matched) {
      break;
    }
||||||| merged common ancestors
        bool matched;
        if (!tokenStream.matchToken(&matched, TokenKind::Comma, TokenStream::Operand)) {
            return null();
        }
        if (!matched) {
            break;
        }
=======
    bool matched;
    if (!tokenStream.matchToken(&matched, TokenKind::Comma,
                                TokenStream::SlashIsRegExp)) {
      return null();
    }
    if (!matched) {
      break;
    }
>>>>>>> upstream-releases

<<<<<<< HEAD
    TokenKind tt;
    if (!tokenStream.peekToken(&tt, TokenStream::Operand)) {
      return null();
||||||| merged common ancestors
        TokenKind tt;
        if (!tokenStream.peekToken(&tt, TokenStream::Operand)) {
            return null();
        }
        if (tt == TokenKind::RightParen) {
            break;
        }
=======
    TokenKind tt;
    if (!tokenStream.peekToken(&tt, TokenStream::SlashIsRegExp)) {
      return null();
    }
    if (tt == TokenKind::RightParen) {
      break;
>>>>>>> upstream-releases
    }
<<<<<<< HEAD
    if (tt == TokenKind::RightParen) {
      break;
    }
  }
||||||| merged common ancestors
=======
  }
>>>>>>> upstream-releases

<<<<<<< HEAD
  if (!mustMatchToken(TokenKind::RightParen, TokenStream::Operand,
                      JSMSG_PAREN_AFTER_ARGS)) {
    return null();
  }
||||||| merged common ancestors
    MUST_MATCH_TOKEN_MOD(TokenKind::RightParen, TokenStream::Operand, JSMSG_PAREN_AFTER_ARGS);
=======
  if (!mustMatchToken(TokenKind::RightParen, JSMSG_PAREN_AFTER_ARGS)) {
    return null();
  }
>>>>>>> upstream-releases

<<<<<<< HEAD
  handler.setEndPosition(argsList, pos().end);
  return argsList;
||||||| merged common ancestors
    handler.setEndPosition(argsList, pos().end);
    return argsList;
=======
  handler_.setEndPosition(argsList, pos().end);
  return argsList;
>>>>>>> upstream-releases
}

<<<<<<< HEAD
bool ParserBase::checkAndMarkSuperScope() {
  if (!pc->sc()->allowSuperProperty()) {
    return false;
  }
||||||| merged common ancestors
bool
ParserBase::checkAndMarkSuperScope()
{
    if (!pc->sc()->allowSuperProperty()) {
        return false;
    }
=======
bool ParserBase::checkAndMarkSuperScope() {
  if (!pc_->sc()->allowSuperProperty()) {
    return false;
  }
>>>>>>> upstream-releases

<<<<<<< HEAD
  pc->setSuperScopeNeedsHomeObject();
  return true;
||||||| merged common ancestors
    pc->setSuperScopeNeedsHomeObject();
    return true;
=======
  pc_->setSuperScopeNeedsHomeObject();
  return true;
>>>>>>> upstream-releases
}

template <class ParseHandler, typename Unit>
<<<<<<< HEAD
typename ParseHandler::Node GeneralParser<ParseHandler, Unit>::memberExpr(
    YieldHandling yieldHandling, TripledotHandling tripledotHandling,
    TokenKind tt, bool allowCallSyntax /* = true */,
    PossibleError* possibleError /* = nullptr */,
    InvokedPrediction invoked /* = PredictUninvoked */) {
  MOZ_ASSERT(anyChars.isCurrentTokenType(tt));

  Node lhs;

  if (!CheckRecursionLimit(context)) {
    return null();
  }

  /* Check for new expression first. */
  if (tt == TokenKind::New) {
    uint32_t newBegin = pos().begin;
    // Make sure this wasn't a |new.target| in disguise.
    BinaryNodeType newTarget;
    if (!tryNewTarget(&newTarget)) {
      return null();
    }
    if (newTarget) {
      lhs = newTarget;
    } else {
      // Gotten by tryNewTarget
      tt = anyChars.currentToken().type;
      Node ctorExpr = memberExpr(yieldHandling, TripledotProhibited, tt,
                                 /* allowCallSyntax = */ false,
                                 /* possibleError = */ nullptr, PredictInvoked);
      if (!ctorExpr) {
        return null();
      }

      bool matched;
      if (!tokenStream.matchToken(&matched, TokenKind::LeftParen)) {
        return null();
      }
||||||| merged common ancestors
typename ParseHandler::Node
GeneralParser<ParseHandler, Unit>::memberExpr(YieldHandling yieldHandling,
                                              TripledotHandling tripledotHandling,
                                              TokenKind tt, bool allowCallSyntax /* = true */,
                                              PossibleError* possibleError /* = nullptr */,
                                              InvokedPrediction invoked /* = PredictUninvoked */)
{
    MOZ_ASSERT(anyChars.isCurrentTokenType(tt));
=======
bool GeneralParser<ParseHandler, Unit>::computeErrorMetadata(
    ErrorMetadata* err, const ErrorReportMixin::ErrorOffset& offset) {
  if (offset.is<ErrorReportMixin::Current>()) {
    return tokenStream.computeErrorMetadata(err, AsVariant(pos().begin));
  }
  return tokenStream.computeErrorMetadata(err, offset);
}

template <class ParseHandler, typename Unit>
typename ParseHandler::Node GeneralParser<ParseHandler, Unit>::memberExpr(
    YieldHandling yieldHandling, TripledotHandling tripledotHandling,
    TokenKind tt, bool allowCallSyntax /* = true */,
    PossibleError* possibleError /* = nullptr */,
    InvokedPrediction invoked /* = PredictUninvoked */) {
  MOZ_ASSERT(anyChars.isCurrentTokenType(tt));
>>>>>>> upstream-releases

<<<<<<< HEAD
      bool isSpread = false;
      Node args;
      if (matched) {
        args = argumentList(yieldHandling, &isSpread);
      } else {
        args = handler.newArguments(pos());
      }

      if (!args) {
        return null();
      }
||||||| merged common ancestors
    Node lhs;
=======
  Node lhs;
>>>>>>> upstream-releases

<<<<<<< HEAD
      lhs = handler.newNewExpression(newBegin, ctorExpr, args);
      if (!lhs) {
||||||| merged common ancestors
    if (!CheckRecursionLimit(context)) {
=======
  if (!CheckRecursionLimit(cx_)) {
    return null();
  }

  /* Check for new expression first. */
  if (tt == TokenKind::New) {
    uint32_t newBegin = pos().begin;
    // Make sure this wasn't a |new.target| in disguise.
    BinaryNodeType newTarget;
    if (!tryNewTarget(&newTarget)) {
      return null();
    }
    if (newTarget) {
      lhs = newTarget;
    } else {
      // Gotten by tryNewTarget
      tt = anyChars.currentToken().type;
      Node ctorExpr = memberExpr(yieldHandling, TripledotProhibited, tt,
                                 /* allowCallSyntax = */ false,
                                 /* possibleError = */ nullptr, PredictInvoked);
      if (!ctorExpr) {
>>>>>>> upstream-releases
        return null();
<<<<<<< HEAD
      }

      if (isSpread) {
        handler.setOp(lhs, JSOP_SPREADNEW);
      }
    }
  } else if (tt == TokenKind::Super) {
    NameNodeType thisName = newThisName();
    if (!thisName) {
      return null();
    }
    lhs = handler.newSuperBase(thisName, pos());
    if (!lhs) {
      return null();
    }
  } else if (tt == TokenKind::Import) {
    lhs = importExpr(yieldHandling, allowCallSyntax);
    if (!lhs) {
      return null();
    }
  } else {
    lhs = primaryExpr(yieldHandling, tripledotHandling, tt, possibleError,
                      invoked);
    if (!lhs) {
      return null();
    }
  }
||||||| merged common ancestors
    }
=======
      }
>>>>>>> upstream-releases

<<<<<<< HEAD
  MOZ_ASSERT_IF(handler.isSuperBase(lhs),
                anyChars.isCurrentTokenType(TokenKind::Super));
||||||| merged common ancestors
    /* Check for new expression first. */
    if (tt == TokenKind::New) {
        uint32_t newBegin = pos().begin;
        // Make sure this wasn't a |new.target| in disguise.
        BinaryNodeType newTarget;
        if (!tryNewTarget(&newTarget)) {
            return null();
        }
        if (newTarget) {
            lhs = newTarget;
        } else {
            // Gotten by tryNewTarget
            tt = anyChars.currentToken().type;
            Node ctorExpr = memberExpr(yieldHandling, TripledotProhibited, tt,
                                       /* allowCallSyntax = */ false,
                                       /* possibleError = */ nullptr, PredictInvoked);
            if (!ctorExpr) {
                return null();
            }
=======
      bool matched;
      if (!tokenStream.matchToken(&matched, TokenKind::LeftParen)) {
        return null();
      }
>>>>>>> upstream-releases

<<<<<<< HEAD
  while (true) {
    if (!tokenStream.getToken(&tt)) {
      return null();
    }
    if (tt == TokenKind::Eof) {
      break;
    }
||||||| merged common ancestors
            bool matched;
            if (!tokenStream.matchToken(&matched, TokenKind::LeftParen)) {
                return null();
            }
=======
      bool isSpread = false;
      Node args;
      if (matched) {
        args = argumentList(yieldHandling, &isSpread);
      } else {
        args = handler_.newArguments(pos());
      }
>>>>>>> upstream-releases

<<<<<<< HEAD
    Node nextMember;
    if (tt == TokenKind::Dot) {
      if (!tokenStream.getToken(&tt)) {
        return null();
      }
||||||| merged common ancestors
            bool isSpread = false;
            Node args;
            if (matched) {
                args = argumentList(yieldHandling, &isSpread);
            } else {
                args = handler.newArguments(pos());
            }
=======
      if (!args) {
        return null();
      }
>>>>>>> upstream-releases

<<<<<<< HEAD
      if (TokenKindIsPossibleIdentifierName(tt)) {
        PropertyName* field = anyChars.currentName();
        if (handler.isSuperBase(lhs) && !checkAndMarkSuperScope()) {
          error(JSMSG_BAD_SUPERPROP, "property");
          return null();
        }
||||||| merged common ancestors
            if (!args) {
                return null();
            }
=======
      lhs = handler_.newNewExpression(newBegin, ctorExpr, args, isSpread);
      if (!lhs) {
        return null();
      }
    }
  } else if (tt == TokenKind::Super) {
    NameNodeType thisName = newThisName();
    if (!thisName) {
      return null();
    }
    lhs = handler_.newSuperBase(thisName, pos());
    if (!lhs) {
      return null();
    }
  } else if (tt == TokenKind::Import) {
    lhs = importExpr(yieldHandling, allowCallSyntax);
    if (!lhs) {
      return null();
    }
  } else {
    lhs = primaryExpr(yieldHandling, tripledotHandling, tt, possibleError,
                      invoked);
    if (!lhs) {
      return null();
    }
  }
>>>>>>> upstream-releases

<<<<<<< HEAD
        NameNodeType name = handler.newPropertyName(field, pos());
        if (!name) {
          return null();
        }
||||||| merged common ancestors
            lhs = handler.newNewExpression(newBegin, ctorExpr, args);
            if (!lhs) {
                return null();
            }
=======
  MOZ_ASSERT_IF(handler_.isSuperBase(lhs),
                anyChars.isCurrentTokenType(TokenKind::Super));
>>>>>>> upstream-releases

<<<<<<< HEAD
        nextMember = handler.newPropertyAccess(lhs, name);
        if (!nextMember) {
          return null();
||||||| merged common ancestors
            if (isSpread) {
                handler.setOp(lhs, JSOP_SPREADNEW);
            }
=======
  while (true) {
    if (!tokenStream.getToken(&tt)) {
      return null();
    }
    if (tt == TokenKind::Eof) {
      break;
    }

    Node nextMember;
    if (tt == TokenKind::Dot) {
      if (!tokenStream.getToken(&tt)) {
        return null();
      }

      if (TokenKindIsPossibleIdentifierName(tt)) {
        PropertyName* field = anyChars.currentName();
        if (handler_.isSuperBase(lhs) && !checkAndMarkSuperScope()) {
          error(JSMSG_BAD_SUPERPROP, "property");
          return null();
>>>>>>> upstream-releases
        }
<<<<<<< HEAD
      } else {
        error(JSMSG_NAME_AFTER_DOT);
        return null();
      }
    } else if (tt == TokenKind::LeftBracket) {
      Node propExpr = expr(InAllowed, yieldHandling, TripledotProhibited);
      if (!propExpr) {
        return null();
      }

      if (!mustMatchToken(TokenKind::RightBracket, TokenStream::Operand,
                          JSMSG_BRACKET_IN_INDEX)) {
        return null();
      }

      if (handler.isSuperBase(lhs) && !checkAndMarkSuperScope()) {
        error(JSMSG_BAD_SUPERPROP, "member");
        return null();
      }
      nextMember = handler.newPropertyByValue(lhs, propExpr, pos().end);
      if (!nextMember) {
        return null();
      }
    } else if ((allowCallSyntax && tt == TokenKind::LeftParen) ||
               tt == TokenKind::TemplateHead ||
               tt == TokenKind::NoSubsTemplate) {
      if (handler.isSuperBase(lhs)) {
        if (!pc->sc()->allowSuperCall()) {
          error(JSMSG_BAD_SUPERCALL);
          return null();
||||||| merged common ancestors
    } else if (tt == TokenKind::Super) {
        NameNodeType thisName = newThisName();
        if (!thisName) {
            return null();
=======

        NameNodeType name = handler_.newPropertyName(field, pos());
        if (!name) {
          return null();
>>>>>>> upstream-releases
        }
<<<<<<< HEAD

        if (tt != TokenKind::LeftParen) {
          error(JSMSG_BAD_SUPER);
          return null();
||||||| merged common ancestors
        lhs = handler.newSuperBase(thisName, pos());
        if (!lhs) {
            return null();
=======

        nextMember = handler_.newPropertyAccess(lhs, name);
        if (!nextMember) {
          return null();
>>>>>>> upstream-releases
        }
<<<<<<< HEAD

        // Despite the fact that it's impossible to have |super()| in a
        // generator, we still inherit the yieldHandling of the
        // memberExpression, per spec. Curious.
        bool isSpread = false;
        Node args = argumentList(yieldHandling, &isSpread);
        if (!args) {
          return null();
||||||| merged common ancestors
    } else if (tt == TokenKind::Import) {
        lhs = importMeta();
        if (!lhs) {
            return null();
=======
      } else {
        error(JSMSG_NAME_AFTER_DOT);
        return null();
      }
    } else if (tt == TokenKind::LeftBracket) {
      Node propExpr = expr(InAllowed, yieldHandling, TripledotProhibited);
      if (!propExpr) {
        return null();
      }

      if (!mustMatchToken(TokenKind::RightBracket, JSMSG_BRACKET_IN_INDEX)) {
        return null();
      }

      if (handler_.isSuperBase(lhs) && !checkAndMarkSuperScope()) {
        error(JSMSG_BAD_SUPERPROP, "member");
        return null();
      }
      nextMember = handler_.newPropertyByValue(lhs, propExpr, pos().end);
      if (!nextMember) {
        return null();
      }
    } else if ((allowCallSyntax && tt == TokenKind::LeftParen) ||
               tt == TokenKind::TemplateHead ||
               tt == TokenKind::NoSubsTemplate) {
      if (handler_.isSuperBase(lhs)) {
        if (!pc_->sc()->allowSuperCall()) {
          error(JSMSG_BAD_SUPERCALL);
          return null();
>>>>>>> upstream-releases
        }
<<<<<<< HEAD

        nextMember = handler.newSuperCall(lhs, args);
        if (!nextMember) {
          return null();
||||||| merged common ancestors
    } else {
        lhs = primaryExpr(yieldHandling, tripledotHandling, tt, possibleError, invoked);
        if (!lhs) {
            return null();
=======

        if (tt != TokenKind::LeftParen) {
          error(JSMSG_BAD_SUPER);
          return null();
>>>>>>> upstream-releases
        }

<<<<<<< HEAD
        if (isSpread) {
          handler.setOp(nextMember, JSOP_SPREADSUPERCALL);
        }
||||||| merged common ancestors
    MOZ_ASSERT_IF(handler.isSuperBase(lhs), anyChars.isCurrentTokenType(TokenKind::Super));
=======
        // Despite the fact that it's impossible to have |super()| in a
        // generator, we still inherit the yieldHandling of the
        // memberExpression, per spec. Curious.
        bool isSpread = false;
        Node args = argumentList(yieldHandling, &isSpread);
        if (!args) {
          return null();
        }
>>>>>>> upstream-releases

<<<<<<< HEAD
        NameNodeType thisName = newThisName();
        if (!thisName) {
          return null();
||||||| merged common ancestors
    while (true) {
        if (!tokenStream.getToken(&tt)) {
            return null();
=======
        CallNodeType superCall = handler_.newSuperCall(lhs, args, isSpread);
        if (!superCall) {
          return null();
>>>>>>> upstream-releases
        }
<<<<<<< HEAD

        nextMember = handler.newSetThis(thisName, nextMember);
        if (!nextMember) {
          return null();
        }
      } else {
        if (options().selfHostingMode && handler.isPropertyAccess(lhs)) {
          error(JSMSG_SELFHOSTED_METHOD_CALL);
          return null();
||||||| merged common ancestors
        if (tt == TokenKind::Eof) {
            break;
=======

        nextMember = superCall;

        NameNodeType thisName = newThisName();
        if (!thisName) {
          return null();
>>>>>>> upstream-releases
        }

<<<<<<< HEAD
        JSOp op = JSOP_CALL;
        bool maybeAsyncArrow = false;
        if (PropertyName* prop = handler.maybeDottedProperty(lhs)) {
          // Use the JSOP_FUN{APPLY,CALL} optimizations given the
          // right syntax.
          if (prop == context->names().apply) {
            op = JSOP_FUNAPPLY;
            if (pc->isFunctionBox()) {
              pc->functionBox()->usesApply = true;
            }
          } else if (prop == context->names().call) {
            op = JSOP_FUNCALL;
          }
        } else if (tt == TokenKind::LeftParen) {
          if (handler.isAsyncKeyword(lhs, context)) {
            // |async (| can be the start of an async arrow
            // function, so we need to defer reporting possible
            // errors from destructuring syntax. To give better
            // error messages, we only allow the AsyncArrowHead
            // part of the CoverCallExpressionAndAsyncArrowHead
            // syntax when the initial name is "async".
            maybeAsyncArrow = true;
          } else if (handler.isEvalName(lhs, context)) {
            // Select the right EVAL op and flag pc as having a
            // direct eval.
            op = pc->sc()->strict() ? JSOP_STRICTEVAL : JSOP_EVAL;
            pc->sc()->setBindingsAccessedDynamically();
            pc->sc()->setHasDirectEval();

            // In non-strict mode code, direct calls to eval can
            // add variables to the call object.
            if (pc->isFunctionBox() && !pc->sc()->strict()) {
              pc->functionBox()->setHasExtensibleScope();
            }
||||||| merged common ancestors
        Node nextMember;
        if (tt == TokenKind::Dot) {
            if (!tokenStream.getToken(&tt)) {
                return null();
            }
            if (TokenKindIsPossibleIdentifierName(tt)) {
                PropertyName* field = anyChars.currentName();
                if (handler.isSuperBase(lhs) && !checkAndMarkSuperScope()) {
                    error(JSMSG_BAD_SUPERPROP, "property");
                    return null();
                }

                NameNodeType name = handler.newPropertyName(field, pos());
                if (!name) {
                    return null();
                }

                nextMember = handler.newPropertyAccess(lhs, name);
                if (!nextMember) {
                    return null();
                }
            } else {
                error(JSMSG_NAME_AFTER_DOT);
                return null();
            }
        } else if (tt == TokenKind::LeftBracket) {
            Node propExpr = expr(InAllowed, yieldHandling, TripledotProhibited);
            if (!propExpr) {
                return null();
            }
=======
        nextMember = handler_.newSetThis(thisName, nextMember);
        if (!nextMember) {
          return null();
        }
>>>>>>> upstream-releases

<<<<<<< HEAD
            // If we're in a method, mark the method as requiring
            // support for 'super', since direct eval code can use
            // it. (If we're not in a method, that's fine, so
            // ignore the return value.)
            checkAndMarkSuperScope();
          }
        }
||||||| merged common ancestors
            MUST_MATCH_TOKEN_MOD(TokenKind::RightBracket, TokenStream::Operand, JSMSG_BRACKET_IN_INDEX);
=======
        if (!noteUsedName(cx_->names().dotInitializers)) {
          return null();
        }
      } else {
        if (options().selfHostingMode && handler_.isPropertyAccess(lhs)) {
          error(JSMSG_SELFHOSTED_METHOD_CALL);
          return null();
        }
>>>>>>> upstream-releases

<<<<<<< HEAD
        if (tt == TokenKind::LeftParen) {
          bool isSpread = false;
          PossibleError* asyncPossibleError =
              maybeAsyncArrow ? possibleError : nullptr;
          Node args =
              argumentList(yieldHandling, &isSpread, asyncPossibleError);
          if (!args) {
            return null();
          }
          if (isSpread) {
            if (op == JSOP_EVAL) {
              op = JSOP_SPREADEVAL;
            } else if (op == JSOP_STRICTEVAL) {
              op = JSOP_STRICTSPREADEVAL;
||||||| merged common ancestors
            if (handler.isSuperBase(lhs) && !checkAndMarkSuperScope()) {
                error(JSMSG_BAD_SUPERPROP, "member");
                return null();
            }
            nextMember = handler.newPropertyByValue(lhs, propExpr, pos().end);
            if (!nextMember) {
                return null();
            }
        } else if ((allowCallSyntax && tt == TokenKind::LeftParen) ||
                   tt == TokenKind::TemplateHead ||
                   tt == TokenKind::NoSubsTemplate)
        {
            if (handler.isSuperBase(lhs)) {
                if (!pc->sc()->allowSuperCall()) {
                    error(JSMSG_BAD_SUPERCALL);
                    return null();
                }

                if (tt != TokenKind::LeftParen) {
                    error(JSMSG_BAD_SUPER);
                    return null();
                }

                // Despite the fact that it's impossible to have |super()| in a
                // generator, we still inherit the yieldHandling of the
                // memberExpression, per spec. Curious.
                bool isSpread = false;
                Node args = argumentList(yieldHandling, &isSpread);
                if (!args) {
                    return null();
                }

                nextMember = handler.newSuperCall(lhs, args);
                if (!nextMember) {
                    return null();
                }

                if (isSpread) {
                    handler.setOp(nextMember, JSOP_SPREADSUPERCALL);
                }

                NameNodeType thisName = newThisName();
                if (!thisName) {
                    return null();
                }

                nextMember = handler.newSetThis(thisName, nextMember);
                if (!nextMember) {
                    return null();
                }
=======
        JSOp op = JSOP_CALL;
        bool maybeAsyncArrow = false;
        if (PropertyName* prop = handler_.maybeDottedProperty(lhs)) {
          // Use the JSOP_FUN{APPLY,CALL} optimizations given the
          // right syntax.
          if (prop == cx_->names().apply) {
            op = JSOP_FUNAPPLY;
            if (pc_->isFunctionBox()) {
              pc_->functionBox()->usesApply = true;
            }
          } else if (prop == cx_->names().call) {
            op = JSOP_FUNCALL;
          }
        } else if (tt == TokenKind::LeftParen) {
          if (handler_.isAsyncKeyword(lhs, cx_)) {
            // |async (| can be the start of an async arrow
            // function, so we need to defer reporting possible
            // errors from destructuring syntax. To give better
            // error messages, we only allow the AsyncArrowHead
            // part of the CoverCallExpressionAndAsyncArrowHead
            // syntax when the initial name is "async".
            maybeAsyncArrow = true;
          } else if (handler_.isEvalName(lhs, cx_)) {
            // Select the right EVAL op and flag pc_ as having a
            // direct eval.
            op = pc_->sc()->strict() ? JSOP_STRICTEVAL : JSOP_EVAL;
            pc_->sc()->setBindingsAccessedDynamically();
            pc_->sc()->setHasDirectEval();

            // In non-strict mode code, direct calls to eval can
            // add variables to the call object.
            if (pc_->isFunctionBox() && !pc_->sc()->strict()) {
              pc_->functionBox()->setHasExtensibleScope();
            }

            // If we're in a method, mark the method as requiring
            // support for 'super', since direct eval code can use
            // it. (If we're not in a method, that's fine, so
            // ignore the return value.)
            checkAndMarkSuperScope();
          }
        }

        if (tt == TokenKind::LeftParen) {
          bool isSpread = false;
          PossibleError* asyncPossibleError =
              maybeAsyncArrow ? possibleError : nullptr;
          Node args =
              argumentList(yieldHandling, &isSpread, asyncPossibleError);
          if (!args) {
            return null();
          }
          if (isSpread) {
            if (op == JSOP_EVAL) {
              op = JSOP_SPREADEVAL;
            } else if (op == JSOP_STRICTEVAL) {
              op = JSOP_STRICTSPREADEVAL;
>>>>>>> upstream-releases
            } else {
              op = JSOP_SPREADCALL;
            }
<<<<<<< HEAD
          }

          nextMember = handler.newCall(lhs, args);
          if (!nextMember) {
            return null();
          }
||||||| merged common ancestors
=======
          }

          nextMember = handler_.newCall(lhs, args, op);
          if (!nextMember) {
            return null();
          }
>>>>>>> upstream-releases
        } else {
<<<<<<< HEAD
          ListNodeType args = handler.newArguments(pos());
          if (!args) {
            return null();
          }
||||||| merged common ancestors
            anyChars.ungetToken();
            if (handler.isSuperBase(lhs)) {
                break;
            }
            return lhs;
        }
=======
          ListNodeType args = handler_.newArguments(pos());
          if (!args) {
            return null();
          }
>>>>>>> upstream-releases

          if (!taggedTemplate(yieldHandling, args, tt)) {
            return null();
          }

<<<<<<< HEAD
          nextMember = handler.newTaggedTemplate(lhs, args);
          if (!nextMember) {
            return null();
          }
        }
        handler.setOp(nextMember, op);
      }
    } else {
      anyChars.ungetToken();
      if (handler.isSuperBase(lhs)) {
        break;
      }
      return lhs;
||||||| merged common ancestors
    if (handler.isSuperBase(lhs)) {
        error(JSMSG_BAD_SUPER);
        return null();
=======
          nextMember = handler_.newTaggedTemplate(lhs, args, op);
          if (!nextMember) {
            return null();
          }
        }
      }
    } else {
      anyChars.ungetToken();
      if (handler_.isSuperBase(lhs)) {
        break;
      }
      return lhs;
>>>>>>> upstream-releases
    }

<<<<<<< HEAD
    lhs = nextMember;
  }

  if (handler.isSuperBase(lhs)) {
    error(JSMSG_BAD_SUPER);
    return null();
  }

  return lhs;
||||||| merged common ancestors
    return lhs;
=======
    lhs = nextMember;
  }

  if (handler_.isSuperBase(lhs)) {
    error(JSMSG_BAD_SUPER);
    return null();
  }

  return lhs;
>>>>>>> upstream-releases
}

template <class ParseHandler>
inline typename ParseHandler::NameNodeType
PerHandlerParser<ParseHandler>::newName(PropertyName* name) {
  return newName(name, pos());
}

template <class ParseHandler>
inline typename ParseHandler::NameNodeType
<<<<<<< HEAD
PerHandlerParser<ParseHandler>::newName(PropertyName* name, TokenPos pos) {
  return handler.newName(name, pos, context);
||||||| merged common ancestors
PerHandlerParser<ParseHandler>::newName(PropertyName* name, TokenPos pos)
{
    return handler.newName(name, pos, context);
=======
PerHandlerParser<ParseHandler>::newName(PropertyName* name, TokenPos pos) {
  return handler_.newName(name, pos, cx_);
>>>>>>> upstream-releases
}

template <class ParseHandler, typename Unit>
bool GeneralParser<ParseHandler, Unit>::checkLabelOrIdentifierReference(
    PropertyName* ident, uint32_t offset, YieldHandling yieldHandling,
    TokenKind hint /* = TokenKind::Limit */) {
  TokenKind tt;
  if (hint == TokenKind::Limit) {
    tt = ReservedWordTokenKind(ident);
  } else {
    MOZ_ASSERT(hint == ReservedWordTokenKind(ident),
               "hint doesn't match actual token kind");
    tt = hint;
  }

<<<<<<< HEAD
  if (tt == TokenKind::Name || tt == TokenKind::PrivateName) {
    return true;
  }
  if (TokenKindIsContextualKeyword(tt)) {
    if (tt == TokenKind::Yield) {
      if (yieldHandling == YieldIsKeyword) {
        errorAt(offset, JSMSG_RESERVED_ID, "yield");
        return false;
      }
      if (pc->sc()->needStrictChecks()) {
        if (!strictModeErrorAt(offset, JSMSG_RESERVED_ID, "yield")) {
          return false;
        }
      }
      return true;
    }
    if (tt == TokenKind::Await) {
      if (awaitIsKeyword()) {
        errorAt(offset, JSMSG_RESERVED_ID, "await");
        return false;
      }
      return true;
    }
    if (pc->sc()->needStrictChecks()) {
      if (tt == TokenKind::Let) {
        if (!strictModeErrorAt(offset, JSMSG_RESERVED_ID, "let")) {
          return false;
||||||| merged common ancestors
    if (tt == TokenKind::Name) {
        return true;
    }
    if (TokenKindIsContextualKeyword(tt)) {
        if (tt == TokenKind::Yield) {
            if (yieldHandling == YieldIsKeyword) {
                errorAt(offset, JSMSG_RESERVED_ID, "yield");
                return false;
            }
            if (pc->sc()->needStrictChecks()) {
                if (!strictModeErrorAt(offset, JSMSG_RESERVED_ID, "yield")) {
                    return false;
                }
            }
            return true;
        }
        if (tt == TokenKind::Await) {
            if (awaitIsKeyword()) {
                errorAt(offset, JSMSG_RESERVED_ID, "await");
                return false;
            }
            return true;
        }
        if (pc->sc()->needStrictChecks()) {
            if (tt == TokenKind::Let) {
                if (!strictModeErrorAt(offset, JSMSG_RESERVED_ID, "let")) {
                    return false;
                }
                return true;
            }
            if (tt == TokenKind::Static) {
                if (!strictModeErrorAt(offset, JSMSG_RESERVED_ID, "static")) {
                    return false;
                }
                return true;
            }
=======
  if (!pc_->sc()->allowArguments() && ident == cx_->names().arguments) {
    error(JSMSG_BAD_ARGUMENTS);
    return false;
  }

  if (tt == TokenKind::Name || tt == TokenKind::PrivateName) {
    return true;
  }
  if (TokenKindIsContextualKeyword(tt)) {
    if (tt == TokenKind::Yield) {
      if (yieldHandling == YieldIsKeyword) {
        errorAt(offset, JSMSG_RESERVED_ID, "yield");
        return false;
      }
      if (pc_->sc()->needStrictChecks()) {
        if (!strictModeErrorAt(offset, JSMSG_RESERVED_ID, "yield")) {
          return false;
        }
      }
      return true;
    }
    if (tt == TokenKind::Await) {
      if (awaitIsKeyword()) {
        errorAt(offset, JSMSG_RESERVED_ID, "await");
        return false;
      }
      return true;
    }
    if (pc_->sc()->needStrictChecks()) {
      if (tt == TokenKind::Let) {
        if (!strictModeErrorAt(offset, JSMSG_RESERVED_ID, "let")) {
          return false;
>>>>>>> upstream-releases
        }
        return true;
      }
      if (tt == TokenKind::Static) {
        if (!strictModeErrorAt(offset, JSMSG_RESERVED_ID, "static")) {
          return false;
        }
        return true;
      }
    }
<<<<<<< HEAD
    return true;
  }
  if (TokenKindIsStrictReservedWord(tt)) {
    if (pc->sc()->needStrictChecks()) {
      if (!strictModeErrorAt(offset, JSMSG_RESERVED_ID,
                             ReservedWordToCharZ(tt))) {
||||||| merged common ancestors
    if (TokenKindIsKeyword(tt) || TokenKindIsReservedWordLiteral(tt)) {
        errorAt(offset, JSMSG_INVALID_ID, ReservedWordToCharZ(tt));
        return false;
    }
    if (TokenKindIsFutureReservedWord(tt)) {
        errorAt(offset, JSMSG_RESERVED_ID, ReservedWordToCharZ(tt));
=======
    return true;
  }
  if (TokenKindIsStrictReservedWord(tt)) {
    if (pc_->sc()->needStrictChecks()) {
      if (!strictModeErrorAt(offset, JSMSG_RESERVED_ID,
                             ReservedWordToCharZ(tt))) {
>>>>>>> upstream-releases
        return false;
      }
    }
    return true;
  }
  if (TokenKindIsKeyword(tt) || TokenKindIsReservedWordLiteral(tt)) {
    errorAt(offset, JSMSG_INVALID_ID, ReservedWordToCharZ(tt));
    return false;
  }
  if (TokenKindIsFutureReservedWord(tt)) {
    errorAt(offset, JSMSG_RESERVED_ID, ReservedWordToCharZ(tt));
    return false;
  }
  MOZ_ASSERT_UNREACHABLE("Unexpected reserved word kind.");
  return false;
}

template <class ParseHandler, typename Unit>
<<<<<<< HEAD
bool GeneralParser<ParseHandler, Unit>::checkBindingIdentifier(
    PropertyName* ident, uint32_t offset, YieldHandling yieldHandling,
    TokenKind hint /* = TokenKind::Limit */) {
  if (pc->sc()->needStrictChecks()) {
    if (ident == context->names().arguments) {
      if (!strictModeErrorAt(offset, JSMSG_BAD_STRICT_ASSIGN, "arguments")) {
        return false;
      }
      return true;
    }
||||||| merged common ancestors
bool
GeneralParser<ParseHandler, Unit>::checkBindingIdentifier(PropertyName* ident, uint32_t offset,
                                                          YieldHandling yieldHandling,
                                                          TokenKind hint /* = TokenKind::Limit */)
{
    if (pc->sc()->needStrictChecks()) {
        if (ident == context->names().arguments) {
            if (!strictModeErrorAt(offset, JSMSG_BAD_STRICT_ASSIGN, "arguments")) {
                return false;
            }
            return true;
        }
=======
bool GeneralParser<ParseHandler, Unit>::checkBindingIdentifier(
    PropertyName* ident, uint32_t offset, YieldHandling yieldHandling,
    TokenKind hint /* = TokenKind::Limit */) {
  if (pc_->sc()->needStrictChecks()) {
    if (ident == cx_->names().arguments) {
      if (!strictModeErrorAt(offset, JSMSG_BAD_STRICT_ASSIGN, "arguments")) {
        return false;
      }
      return true;
    }
>>>>>>> upstream-releases

<<<<<<< HEAD
    if (ident == context->names().eval) {
      if (!strictModeErrorAt(offset, JSMSG_BAD_STRICT_ASSIGN, "eval")) {
        return false;
      }
      return true;
||||||| merged common ancestors
        if (ident == context->names().eval) {
            if (!strictModeErrorAt(offset, JSMSG_BAD_STRICT_ASSIGN, "eval")) {
                return false;
            }
            return true;
        }
=======
    if (ident == cx_->names().eval) {
      if (!strictModeErrorAt(offset, JSMSG_BAD_STRICT_ASSIGN, "eval")) {
        return false;
      }
      return true;
>>>>>>> upstream-releases
    }
  }

  return checkLabelOrIdentifierReference(ident, offset, yieldHandling, hint);
}

template <class ParseHandler, typename Unit>
PropertyName* GeneralParser<ParseHandler, Unit>::labelOrIdentifierReference(
    YieldHandling yieldHandling) {
  // ES 2017 draft 12.1.1.
  //   StringValue of IdentifierName normalizes any Unicode escape sequences
  //   in IdentifierName hence such escapes cannot be used to write an
  //   Identifier whose code point sequence is the same as a ReservedWord.
  //
  // Use PropertyName* instead of TokenKind to reflect the normalization.

<<<<<<< HEAD
  // Unless the name contains escapes, we can reuse the current TokenKind
  // to determine if the name is a restricted identifier.
  TokenKind hint = !anyChars.currentNameHasEscapes()
                       ? anyChars.currentToken().type
                       : TokenKind::Limit;
  RootedPropertyName ident(context, anyChars.currentName());
  if (!checkLabelOrIdentifierReference(ident, pos().begin, yieldHandling,
                                       hint)) {
    return nullptr;
  }
  return ident;
||||||| merged common ancestors
    // Unless the name contains escapes, we can reuse the current TokenKind
    // to determine if the name is a restricted identifier.
    TokenKind hint = !anyChars.currentNameHasEscapes()
                     ? anyChars.currentToken().type
                     : TokenKind::Limit;
    RootedPropertyName ident(context, anyChars.currentName());
    if (!checkLabelOrIdentifierReference(ident, pos().begin, yieldHandling, hint)) {
        return nullptr;
    }
    return ident;
=======
  // Unless the name contains escapes, we can reuse the current TokenKind
  // to determine if the name is a restricted identifier.
  TokenKind hint = !anyChars.currentNameHasEscapes()
                       ? anyChars.currentToken().type
                       : TokenKind::Limit;
  RootedPropertyName ident(cx_, anyChars.currentName());
  if (!checkLabelOrIdentifierReference(ident, pos().begin, yieldHandling,
                                       hint)) {
    return nullptr;
  }
  return ident;
>>>>>>> upstream-releases
}

template <class ParseHandler, typename Unit>
<<<<<<< HEAD
PropertyName* GeneralParser<ParseHandler, Unit>::bindingIdentifier(
    YieldHandling yieldHandling) {
  TokenKind hint = !anyChars.currentNameHasEscapes()
                       ? anyChars.currentToken().type
                       : TokenKind::Limit;
  RootedPropertyName ident(context, anyChars.currentName());
  if (!checkBindingIdentifier(ident, pos().begin, yieldHandling, hint)) {
    return nullptr;
  }
  return ident;
||||||| merged common ancestors
PropertyName*
GeneralParser<ParseHandler, Unit>::bindingIdentifier(YieldHandling yieldHandling)
{
    TokenKind hint = !anyChars.currentNameHasEscapes()
                     ? anyChars.currentToken().type
                     : TokenKind::Limit;
    RootedPropertyName ident(context, anyChars.currentName());
    if (!checkBindingIdentifier(ident, pos().begin, yieldHandling, hint)) {
        return nullptr;
    }
    return ident;
=======
PropertyName* GeneralParser<ParseHandler, Unit>::bindingIdentifier(
    YieldHandling yieldHandling) {
  TokenKind hint = !anyChars.currentNameHasEscapes()
                       ? anyChars.currentToken().type
                       : TokenKind::Limit;
  RootedPropertyName ident(cx_, anyChars.currentName());
  if (!checkBindingIdentifier(ident, pos().begin, yieldHandling, hint)) {
    return nullptr;
  }
  return ident;
>>>>>>> upstream-releases
}

template <class ParseHandler>
typename ParseHandler::NameNodeType
PerHandlerParser<ParseHandler>::identifierReference(
    Handle<PropertyName*> name) {
  NameNodeType id = newName(name);
  if (!id) {
    return null();
  }

  if (!noteUsedName(name)) {
    return null();
  }

  return id;
}

template <class ParseHandler>
typename ParseHandler::NameNodeType
<<<<<<< HEAD
PerHandlerParser<ParseHandler>::stringLiteral() {
  return handler.newStringLiteral(anyChars.currentToken().atom(), pos());
||||||| merged common ancestors
PerHandlerParser<ParseHandler>::stringLiteral()
{
    return handler.newStringLiteral(anyChars.currentToken().atom(), pos());
=======
PerHandlerParser<ParseHandler>::stringLiteral() {
  return handler_.newStringLiteral(anyChars.currentToken().atom(), pos());
>>>>>>> upstream-releases
}

template <class ParseHandler>
typename ParseHandler::Node
<<<<<<< HEAD
PerHandlerParser<ParseHandler>::noSubstitutionTaggedTemplate() {
  if (anyChars.hasInvalidTemplateEscape()) {
    anyChars.clearInvalidTemplateEscape();
    return handler.newRawUndefinedLiteral(pos());
  }
||||||| merged common ancestors
PerHandlerParser<ParseHandler>::noSubstitutionTaggedTemplate()
{
    if (anyChars.hasInvalidTemplateEscape()) {
        anyChars.clearInvalidTemplateEscape();
        return handler.newRawUndefinedLiteral(pos());
    }
=======
PerHandlerParser<ParseHandler>::noSubstitutionTaggedTemplate() {
  if (anyChars.hasInvalidTemplateEscape()) {
    anyChars.clearInvalidTemplateEscape();
    return handler_.newRawUndefinedLiteral(pos());
  }
>>>>>>> upstream-releases

<<<<<<< HEAD
  return handler.newTemplateStringLiteral(anyChars.currentToken().atom(),
                                          pos());
||||||| merged common ancestors
    return handler.newTemplateStringLiteral(anyChars.currentToken().atom(), pos());
=======
  return handler_.newTemplateStringLiteral(anyChars.currentToken().atom(),
                                           pos());
>>>>>>> upstream-releases
}

template <class ParseHandler, typename Unit>
typename ParseHandler::NameNodeType
GeneralParser<ParseHandler, Unit>::noSubstitutionUntaggedTemplate() {
  if (!tokenStream.checkForInvalidTemplateEscapeError()) {
    return null();
  }

<<<<<<< HEAD
  return handler.newTemplateStringLiteral(anyChars.currentToken().atom(),
                                          pos());
||||||| merged common ancestors
    return handler.newTemplateStringLiteral(anyChars.currentToken().atom(), pos());
=======
  return handler_.newTemplateStringLiteral(anyChars.currentToken().atom(),
                                           pos());
>>>>>>> upstream-releases
}

template <typename Unit>
<<<<<<< HEAD
RegExpLiteral* Parser<FullParseHandler, Unit>::newRegExp() {
  MOZ_ASSERT(!options().selfHostingMode);

  // Create the regexp and check its syntax.
  const auto& chars = tokenStream.getCharBuffer();
  RegExpFlag flags = anyChars.currentToken().regExpFlags();

  Rooted<RegExpObject*> reobj(context);
  reobj = RegExpObject::create(context, chars.begin(), chars.length(), flags,
                               anyChars, TenuredObject);
  if (!reobj) {
    return null();
  }
||||||| merged common ancestors
RegExpLiteral*
Parser<FullParseHandler, Unit>::newRegExp()
{
    MOZ_ASSERT(!options().selfHostingMode);

    // Create the regexp and check its syntax.
    const auto& chars = tokenStream.getCharBuffer();
    RegExpFlag flags = anyChars.currentToken().regExpFlags();

    Rooted<RegExpObject*> reobj(context);
    reobj = RegExpObject::create(context, chars.begin(), chars.length(), flags, anyChars, alloc,
                                 TenuredObject);
    if (!reobj) {
        return null();
    }
=======
RegExpLiteral* Parser<FullParseHandler, Unit>::newRegExp() {
  MOZ_ASSERT(!options().selfHostingMode);

  // Create the regexp and check its syntax.
  const auto& chars = tokenStream.getCharBuffer();
  RegExpFlags flags = anyChars.currentToken().regExpFlags();

  Rooted<RegExpObject*> reobj(cx_);
  reobj = RegExpObject::create(cx_, chars.begin(), chars.length(), flags,
                               anyChars, TenuredObject);
  if (!reobj) {
    return null();
  }
>>>>>>> upstream-releases

<<<<<<< HEAD
  return handler.newRegExp(reobj, pos(), *this);
||||||| merged common ancestors
    return handler.newRegExp(reobj, pos(), *this);
=======
  return handler_.newRegExp(reobj, pos(), *this);
>>>>>>> upstream-releases
}

template <typename Unit>
SyntaxParseHandler::RegExpLiteralType
Parser<SyntaxParseHandler, Unit>::newRegExp() {
  MOZ_ASSERT(!options().selfHostingMode);

<<<<<<< HEAD
  // Only check the regexp's syntax, but don't create a regexp object.
  const auto& chars = tokenStream.getCharBuffer();
  RegExpFlag flags = anyChars.currentToken().regExpFlags();
||||||| merged common ancestors
    // Only check the regexp's syntax, but don't create a regexp object.
    const auto& chars = tokenStream.getCharBuffer();
    RegExpFlag flags = anyChars.currentToken().regExpFlags();
=======
  // Only check the regexp's syntax, but don't create a regexp object.
  const auto& chars = tokenStream.getCharBuffer();
  RegExpFlags flags = anyChars.currentToken().regExpFlags();
>>>>>>> upstream-releases

<<<<<<< HEAD
  mozilla::Range<const char16_t> source(chars.begin(), chars.length());
  {
    LifoAllocScope scopeAlloc(&alloc);
    if (!js::irregexp::ParsePatternSyntax(anyChars, scopeAlloc.alloc(), source,
                                          flags & UnicodeFlag)) {
      return null();
||||||| merged common ancestors
    mozilla::Range<const char16_t> source(chars.begin(), chars.length());
    if (!js::irregexp::ParsePatternSyntax(anyChars, alloc, source, flags & UnicodeFlag)) {
        return null();
=======
  mozilla::Range<const char16_t> source(chars.begin(), chars.length());
  {
    LifoAllocScope scopeAlloc(&alloc_);
    if (!js::irregexp::ParsePatternSyntax(anyChars, scopeAlloc.alloc(), source,
                                          flags.unicode())) {
      return null();
>>>>>>> upstream-releases
    }
  }

<<<<<<< HEAD
  return handler.newRegExp(SyntaxParseHandler::NodeGeneric, pos(), *this);
||||||| merged common ancestors
    return handler.newRegExp(SyntaxParseHandler::NodeGeneric, pos(), *this);
=======
  return handler_.newRegExp(SyntaxParseHandler::NodeGeneric, pos(), *this);
>>>>>>> upstream-releases
}

template <class ParseHandler, typename Unit>
typename ParseHandler::RegExpLiteralType
GeneralParser<ParseHandler, Unit>::newRegExp() {
  return asFinalParser()->newRegExp();
}

<<<<<<< HEAD
#ifdef ENABLE_BIGINT
template <typename Unit>
BigIntLiteral* Parser<FullParseHandler, Unit>::newBigInt() {
  // The token's charBuffer contains the DecimalIntegerLiteral or
  // NumericLiteralBase production, and as such does not include the
  // BigIntLiteralSuffix (the trailing "n").  Note that NumericLiteralBase
  // productions may start with 0[bBoOxX], indicating binary/octal/hex.
  const auto& chars = tokenStream.getCharBuffer();
  mozilla::Range<const char16_t> source(chars.begin(), chars.length());

  BigInt* b = js::StringToBigInt(context, source);
  if (!b) {
    return null();
  }
||||||| merged common ancestors
// |exprPossibleError| is the PossibleError state within |expr|,
// |possibleError| is the surrounding PossibleError state.
template <class ParseHandler, typename Unit>
bool
GeneralParser<ParseHandler, Unit>::checkDestructuringAssignmentTarget(Node expr, TokenPos exprPos,
                                                                      PossibleError* exprPossibleError,
                                                                      PossibleError* possibleError,
                                                                      TargetBehavior behavior)
{
    // Report any pending expression error if we're definitely not in a
    // destructuring context or the possible destructuring target is a
    // property accessor.
    if (!possibleError || handler.isPropertyAccess(expr)) {
        return exprPossibleError->checkForExpressionError();
    }
=======
template <typename Unit>
BigIntLiteral* Parser<FullParseHandler, Unit>::newBigInt() {
  // The token's charBuffer contains the DecimalIntegerLiteral or
  // NumericLiteralBase production, and as such does not include the
  // BigIntLiteralSuffix (the trailing "n").  Note that NumericLiteralBase
  // productions may start with 0[bBoOxX], indicating binary/octal/hex.
  const auto& chars = tokenStream.getCharBuffer();
  mozilla::Range<const char16_t> source(chars.begin(), chars.length());

  BigInt* b = js::ParseBigIntLiteral(cx_, source);
  if (!b) {
    return null();
  }
>>>>>>> upstream-releases

<<<<<<< HEAD
  // newBigInt immediately puts "b" in a BigIntBox, which is allocated using
  // tempLifoAlloc, avoiding any potential GC.  Therefore it's OK to pass a
  // raw pointer.
  return handler.newBigInt(b, pos(), *this);
}
||||||| merged common ancestors
    // |expr| may end up as a destructuring assignment target, so we need to
    // validate it's either a name or can be parsed as a nested destructuring
    // pattern. Property accessors are also valid assignment targets, but
    // those are already handled above.
=======
  // newBigInt immediately puts "b" in a BigIntBox, which is allocated using
  // tempLifoAlloc, avoiding any potential GC.  Therefore it's OK to pass a
  // raw pointer.
  return handler_.newBigInt(b, pos(), *this);
}
>>>>>>> upstream-releases

template <typename Unit>
SyntaxParseHandler::BigIntLiteralType
Parser<SyntaxParseHandler, Unit>::newBigInt() {
  // The tokenizer has already checked the syntax of the bigint.

<<<<<<< HEAD
  return handler.newBigInt();
}
||||||| merged common ancestors
    // Return early if a pending destructuring error is already present.
    if (possibleError->hasPendingDestructuringError()) {
        return true;
    }
=======
  return handler_.newBigInt();
}
>>>>>>> upstream-releases

<<<<<<< HEAD
template <class ParseHandler, typename Unit>
typename ParseHandler::BigIntLiteralType
GeneralParser<ParseHandler, Unit>::newBigInt() {
  return asFinalParser()->newBigInt();
}
#endif /* ENABLE_BIGINT */
||||||| merged common ancestors
    if (handler.isName(expr)) {
        checkDestructuringAssignmentName(handler.asName(expr), exprPos, possibleError);
        return true;
    }
=======
template <class ParseHandler, typename Unit>
typename ParseHandler::BigIntLiteralType
GeneralParser<ParseHandler, Unit>::newBigInt() {
  return asFinalParser()->newBigInt();
}
>>>>>>> upstream-releases

<<<<<<< HEAD
// |exprPossibleError| is the PossibleError state within |expr|,
// |possibleError| is the surrounding PossibleError state.
template <class ParseHandler, typename Unit>
bool GeneralParser<ParseHandler, Unit>::checkDestructuringAssignmentTarget(
    Node expr, TokenPos exprPos, PossibleError* exprPossibleError,
    PossibleError* possibleError, TargetBehavior behavior) {
  // Report any pending expression error if we're definitely not in a
  // destructuring context or the possible destructuring target is a
  // property accessor.
  if (!possibleError || handler.isPropertyAccess(expr)) {
    return exprPossibleError->checkForExpressionError();
  }

  // |expr| may end up as a destructuring assignment target, so we need to
  // validate it's either a name or can be parsed as a nested destructuring
  // pattern. Property accessors are also valid assignment targets, but
  // those are already handled above.

  exprPossibleError->transferErrorsTo(possibleError);

  // Return early if a pending destructuring error is already present.
  if (possibleError->hasPendingDestructuringError()) {
    return true;
  }
||||||| merged common ancestors
    if (handler.isUnparenthesizedDestructuringPattern(expr)) {
        if (behavior == TargetBehavior::ForbidAssignmentPattern) {
            possibleError->setPendingDestructuringErrorAt(exprPos, JSMSG_BAD_DESTRUCT_TARGET);
        }
        return true;
    }
=======
// |exprPossibleError| is the PossibleError state within |expr|,
// |possibleError| is the surrounding PossibleError state.
template <class ParseHandler, typename Unit>
bool GeneralParser<ParseHandler, Unit>::checkDestructuringAssignmentTarget(
    Node expr, TokenPos exprPos, PossibleError* exprPossibleError,
    PossibleError* possibleError, TargetBehavior behavior) {
  // Report any pending expression error if we're definitely not in a
  // destructuring context or the possible destructuring target is a
  // property accessor.
  if (!possibleError || handler_.isPropertyAccess(expr)) {
    return exprPossibleError->checkForExpressionError();
  }

  // |expr| may end up as a destructuring assignment target, so we need to
  // validate it's either a name or can be parsed as a nested destructuring
  // pattern. Property accessors are also valid assignment targets, but
  // those are already handled above.

  exprPossibleError->transferErrorsTo(possibleError);

  // Return early if a pending destructuring error is already present.
  if (possibleError->hasPendingDestructuringError()) {
    return true;
  }
>>>>>>> upstream-releases

<<<<<<< HEAD
  if (handler.isName(expr)) {
    checkDestructuringAssignmentName(handler.asName(expr), exprPos,
                                     possibleError);
    return true;
  }
||||||| merged common ancestors
    // Parentheses are forbidden around destructuring *patterns* (but allowed
    // around names). Use our nicer error message for parenthesized, nested
    // patterns if nested destructuring patterns are allowed.
    if (handler.isParenthesizedDestructuringPattern(expr) &&
        behavior != TargetBehavior::ForbidAssignmentPattern)
    {
        possibleError->setPendingDestructuringErrorAt(exprPos, JSMSG_BAD_DESTRUCT_PARENS);
    } else {
        possibleError->setPendingDestructuringErrorAt(exprPos, JSMSG_BAD_DESTRUCT_TARGET);
    }
=======
  if (handler_.isName(expr)) {
    checkDestructuringAssignmentName(handler_.asName(expr), exprPos,
                                     possibleError);
    return true;
  }
>>>>>>> upstream-releases

<<<<<<< HEAD
  if (handler.isUnparenthesizedDestructuringPattern(expr)) {
    if (behavior == TargetBehavior::ForbidAssignmentPattern) {
      possibleError->setPendingDestructuringErrorAt(exprPos,
                                                    JSMSG_BAD_DESTRUCT_TARGET);
    }
||||||| merged common ancestors
=======
  if (handler_.isUnparenthesizedDestructuringPattern(expr)) {
    if (behavior == TargetBehavior::ForbidAssignmentPattern) {
      possibleError->setPendingDestructuringErrorAt(exprPos,
                                                    JSMSG_BAD_DESTRUCT_TARGET);
    }
>>>>>>> upstream-releases
    return true;
<<<<<<< HEAD
  }

  // Parentheses are forbidden around destructuring *patterns* (but allowed
  // around names). Use our nicer error message for parenthesized, nested
  // patterns if nested destructuring patterns are allowed.
  if (handler.isParenthesizedDestructuringPattern(expr) &&
      behavior != TargetBehavior::ForbidAssignmentPattern) {
    possibleError->setPendingDestructuringErrorAt(exprPos,
                                                  JSMSG_BAD_DESTRUCT_PARENS);
  } else {
    possibleError->setPendingDestructuringErrorAt(exprPos,
                                                  JSMSG_BAD_DESTRUCT_TARGET);
  }

  return true;
||||||| merged common ancestors
=======
  }

  // Parentheses are forbidden around destructuring *patterns* (but allowed
  // around names). Use our nicer error message for parenthesized, nested
  // patterns if nested destructuring patterns are allowed.
  if (handler_.isParenthesizedDestructuringPattern(expr) &&
      behavior != TargetBehavior::ForbidAssignmentPattern) {
    possibleError->setPendingDestructuringErrorAt(exprPos,
                                                  JSMSG_BAD_DESTRUCT_PARENS);
  } else {
    possibleError->setPendingDestructuringErrorAt(exprPos,
                                                  JSMSG_BAD_DESTRUCT_TARGET);
  }

  return true;
>>>>>>> upstream-releases
}

template <class ParseHandler, typename Unit>
void GeneralParser<ParseHandler, Unit>::checkDestructuringAssignmentName(
    NameNodeType name, TokenPos namePos, PossibleError* possibleError) {
#ifdef DEBUG
<<<<<<< HEAD
  // GCC 8.0.1 crashes if this is a one-liner.
  bool isName = handler.isName(name);
  MOZ_ASSERT(isName);
||||||| merged common ancestors
    // GCC 8.0.1 crashes if this is a one-liner.
    bool isName = handler.isName(name);
    MOZ_ASSERT(isName);
=======
  // GCC 8.0.1 crashes if this is a one-liner.
  bool isName = handler_.isName(name);
  MOZ_ASSERT(isName);
>>>>>>> upstream-releases
#endif

<<<<<<< HEAD
  // Return early if a pending destructuring error is already present.
  if (possibleError->hasPendingDestructuringError()) {
    return;
  }

  if (pc->sc()->needStrictChecks()) {
    if (handler.isArgumentsName(name, context)) {
      if (pc->sc()->strict()) {
        possibleError->setPendingDestructuringErrorAt(
            namePos, JSMSG_BAD_STRICT_ASSIGN_ARGUMENTS);
      } else {
        possibleError->setPendingDestructuringWarningAt(
            namePos, JSMSG_BAD_STRICT_ASSIGN_ARGUMENTS);
      }
      return;
||||||| merged common ancestors
    // Return early if a pending destructuring error is already present.
    if (possibleError->hasPendingDestructuringError()) {
        return;
=======
  // Return early if a pending destructuring error is already present.
  if (possibleError->hasPendingDestructuringError()) {
    return;
  }

  if (pc_->sc()->needStrictChecks()) {
    if (handler_.isArgumentsName(name, cx_)) {
      if (pc_->sc()->strict()) {
        possibleError->setPendingDestructuringErrorAt(
            namePos, JSMSG_BAD_STRICT_ASSIGN_ARGUMENTS);
      } else {
        possibleError->setPendingDestructuringWarningAt(
            namePos, JSMSG_BAD_STRICT_ASSIGN_ARGUMENTS);
      }
      return;
>>>>>>> upstream-releases
    }

<<<<<<< HEAD
    if (handler.isEvalName(name, context)) {
      if (pc->sc()->strict()) {
        possibleError->setPendingDestructuringErrorAt(
            namePos, JSMSG_BAD_STRICT_ASSIGN_EVAL);
      } else {
        possibleError->setPendingDestructuringWarningAt(
            namePos, JSMSG_BAD_STRICT_ASSIGN_EVAL);
      }
      return;
||||||| merged common ancestors
    if (pc->sc()->needStrictChecks()) {
        if (handler.isArgumentsName(name, context)) {
            if (pc->sc()->strict()) {
                possibleError->setPendingDestructuringErrorAt(namePos,
                                                              JSMSG_BAD_STRICT_ASSIGN_ARGUMENTS);
            } else {
                possibleError->setPendingDestructuringWarningAt(namePos,
                                                                JSMSG_BAD_STRICT_ASSIGN_ARGUMENTS);
            }
            return;
        }

        if (handler.isEvalName(name, context)) {
            if (pc->sc()->strict()) {
                possibleError->setPendingDestructuringErrorAt(namePos,
                                                              JSMSG_BAD_STRICT_ASSIGN_EVAL);
            } else {
                possibleError->setPendingDestructuringWarningAt(namePos,
                                                                JSMSG_BAD_STRICT_ASSIGN_EVAL);
            }
            return;
        }
=======
    if (handler_.isEvalName(name, cx_)) {
      if (pc_->sc()->strict()) {
        possibleError->setPendingDestructuringErrorAt(
            namePos, JSMSG_BAD_STRICT_ASSIGN_EVAL);
      } else {
        possibleError->setPendingDestructuringWarningAt(
            namePos, JSMSG_BAD_STRICT_ASSIGN_EVAL);
      }
      return;
>>>>>>> upstream-releases
    }
  }
}

template <class ParseHandler, typename Unit>
bool GeneralParser<ParseHandler, Unit>::checkDestructuringAssignmentElement(
    Node expr, TokenPos exprPos, PossibleError* exprPossibleError,
    PossibleError* possibleError) {
  // ES2018 draft rev 0719f44aab93215ed9a626b2f45bd34f36916834
  // 12.15.5 Destructuring Assignment
  //
  // AssignmentElement[Yield, Await]:
  //   DestructuringAssignmentTarget[?Yield, ?Await]
  //   DestructuringAssignmentTarget[?Yield, ?Await] Initializer[+In,
  //                                                             ?Yield,
  //                                                             ?Await]

<<<<<<< HEAD
  // If |expr| is an assignment element with an initializer expression, its
  // destructuring assignment target was already validated in assignExpr().
  // Otherwise we need to check that |expr| is a valid destructuring target.
  if (handler.isUnparenthesizedAssignment(expr)) {
    // Report any pending expression error if we're definitely not in a
    // destructuring context.
    if (!possibleError) {
      return exprPossibleError->checkForExpressionError();
||||||| merged common ancestors
        exprPossibleError->transferErrorsTo(possibleError);
        return true;
=======
  // If |expr| is an assignment element with an initializer expression, its
  // destructuring assignment target was already validated in assignExpr().
  // Otherwise we need to check that |expr| is a valid destructuring target.
  if (handler_.isUnparenthesizedAssignment(expr)) {
    // Report any pending expression error if we're definitely not in a
    // destructuring context.
    if (!possibleError) {
      return exprPossibleError->checkForExpressionError();
>>>>>>> upstream-releases
    }

    exprPossibleError->transferErrorsTo(possibleError);
    return true;
  }
  return checkDestructuringAssignmentTarget(expr, exprPos, exprPossibleError,
                                            possibleError);
}

template <class ParseHandler, typename Unit>
typename ParseHandler::ListNodeType
<<<<<<< HEAD
GeneralParser<ParseHandler, Unit>::arrayInitializer(
    YieldHandling yieldHandling, PossibleError* possibleError) {
  MOZ_ASSERT(anyChars.isCurrentTokenType(TokenKind::LeftBracket));

  uint32_t begin = pos().begin;
  ListNodeType literal = handler.newArrayLiteral(begin);
  if (!literal) {
    return null();
  }

  TokenKind tt;
  if (!tokenStream.getToken(&tt, TokenStream::Operand)) {
    return null();
  }

  if (tt == TokenKind::RightBracket) {
    /*
     * Mark empty arrays as non-constant, since we cannot easily
     * determine their type.
     */
    handler.setListHasNonConstInitializer(literal);
  } else {
    anyChars.ungetToken();

    for (uint32_t index = 0;; index++) {
      if (index >= NativeObject::MAX_DENSE_ELEMENTS_COUNT) {
        error(JSMSG_ARRAY_INIT_TOO_BIG);
        return null();
      }

      TokenKind tt;
      if (!tokenStream.peekToken(&tt, TokenStream::Operand)) {
        return null();
      }
      if (tt == TokenKind::RightBracket) {
        break;
      }

      if (tt == TokenKind::Comma) {
        tokenStream.consumeKnownToken(TokenKind::Comma, TokenStream::Operand);
        if (!handler.addElision(literal, pos())) {
          return null();
        }
        continue;
      }

      if (tt == TokenKind::TripleDot) {
        tokenStream.consumeKnownToken(TokenKind::TripleDot,
                                      TokenStream::Operand);
        uint32_t begin = pos().begin;

        TokenPos innerPos;
        if (!tokenStream.peekTokenPos(&innerPos, TokenStream::Operand)) {
          return null();
        }

        PossibleError possibleErrorInner(*this);
        Node inner = assignExpr(InAllowed, yieldHandling, TripledotProhibited,
                                &possibleErrorInner);
        if (!inner) {
          return null();
        }
        if (!checkDestructuringAssignmentTarget(
                inner, innerPos, &possibleErrorInner, possibleError)) {
          return null();
        }
||||||| merged common ancestors
GeneralParser<ParseHandler, Unit>::arrayInitializer(YieldHandling yieldHandling,
                                                    PossibleError* possibleError)
{
    MOZ_ASSERT(anyChars.isCurrentTokenType(TokenKind::LeftBracket));

    uint32_t begin = pos().begin;
    ListNodeType literal = handler.newArrayLiteral(begin);
    if (!literal) {
        return null();
    }

    TokenKind tt;
    if (!tokenStream.getToken(&tt, TokenStream::Operand)) {
        return null();
    }

    if (tt == TokenKind::RightBracket) {
        /*
         * Mark empty arrays as non-constant, since we cannot easily
         * determine their type.
         */
        handler.setListHasNonConstInitializer(literal);
    } else {
        anyChars.ungetToken();

        for (uint32_t index = 0; ; index++) {
            if (index >= NativeObject::MAX_DENSE_ELEMENTS_COUNT) {
                error(JSMSG_ARRAY_INIT_TOO_BIG);
                return null();
            }

            TokenKind tt;
            if (!tokenStream.peekToken(&tt, TokenStream::Operand)) {
                return null();
            }
            if (tt == TokenKind::RightBracket) {
                break;
            }

            if (tt == TokenKind::Comma) {
                tokenStream.consumeKnownToken(TokenKind::Comma, TokenStream::Operand);
                if (!handler.addElision(literal, pos())) {
                    return null();
                }
                continue;
            }

            if (tt == TokenKind::TripleDot) {
                tokenStream.consumeKnownToken(TokenKind::TripleDot, TokenStream::Operand);
                uint32_t begin = pos().begin;

                TokenPos innerPos;
                if (!tokenStream.peekTokenPos(&innerPos, TokenStream::Operand)) {
                    return null();
                }

                PossibleError possibleErrorInner(*this);
                Node inner = assignExpr(InAllowed, yieldHandling, TripledotProhibited,
                                        &possibleErrorInner);
                if (!inner) {
                    return null();
                }
                if (!checkDestructuringAssignmentTarget(inner, innerPos, &possibleErrorInner,
                                                        possibleError))
                {
                    return null();
                }

                if (!handler.addSpreadElement(literal, begin, inner)) {
                    return null();
                }
            } else {
                TokenPos elementPos;
                if (!tokenStream.peekTokenPos(&elementPos, TokenStream::Operand)) {
                    return null();
                }

                PossibleError possibleErrorInner(*this);
                Node element = assignExpr(InAllowed, yieldHandling, TripledotProhibited,
                                          &possibleErrorInner);
                if (!element) {
                    return null();
                }
                if (!checkDestructuringAssignmentElement(element, elementPos, &possibleErrorInner,
                                                         possibleError))
                {
                    return null();
                }
                if (foldConstants && !FoldConstants(context, &element, this)) {
                    return null();
                }
                handler.addArrayElement(literal, element);
            }
=======
GeneralParser<ParseHandler, Unit>::arrayInitializer(
    YieldHandling yieldHandling, PossibleError* possibleError) {
  MOZ_ASSERT(anyChars.isCurrentTokenType(TokenKind::LeftBracket));
>>>>>>> upstream-releases

<<<<<<< HEAD
        if (!handler.addSpreadElement(literal, begin, inner)) {
          return null();
        }
      } else {
        TokenPos elementPos;
        if (!tokenStream.peekTokenPos(&elementPos, TokenStream::Operand)) {
          return null();
        }
||||||| merged common ancestors
            bool matched;
            if (!tokenStream.matchToken(&matched, TokenKind::Comma, TokenStream::Operand)) {
                return null();
            }
            if (!matched) {
                break;
            }
=======
  uint32_t begin = pos().begin;
  ListNodeType literal = handler_.newArrayLiteral(begin);
  if (!literal) {
    return null();
  }
>>>>>>> upstream-releases

<<<<<<< HEAD
        PossibleError possibleErrorInner(*this);
        Node element = assignExpr(InAllowed, yieldHandling, TripledotProhibited,
                                  &possibleErrorInner);
        if (!element) {
          return null();
        }
        if (!checkDestructuringAssignmentElement(
                element, elementPos, &possibleErrorInner, possibleError)) {
          return null();
        }
        if (foldConstants && !FoldConstants(context, &element, this)) {
          return null();
        }
        handler.addArrayElement(literal, element);
      }
||||||| merged common ancestors
            if (tt == TokenKind::TripleDot && possibleError) {
                possibleError->setPendingDestructuringErrorAt(pos(), JSMSG_REST_WITH_COMMA);
            }
        }
=======
  TokenKind tt;
  if (!tokenStream.getToken(&tt, TokenStream::SlashIsRegExp)) {
    return null();
  }
>>>>>>> upstream-releases

<<<<<<< HEAD
      bool matched;
      if (!tokenStream.matchToken(&matched, TokenKind::Comma,
                                  TokenStream::Operand)) {
        return null();
      }
      if (!matched) {
        break;
      }

      if (tt == TokenKind::TripleDot && possibleError) {
        possibleError->setPendingDestructuringErrorAt(pos(),
                                                      JSMSG_REST_WITH_COMMA);
      }
    }

    if (!mustMatchToken(TokenKind::RightBracket, TokenStream::Operand,
                        [this, begin](TokenKind actual) {
                          this->reportMissingClosing(JSMSG_BRACKET_AFTER_LIST,
                                                     JSMSG_BRACKET_OPENED,
                                                     begin);
                        })) {
      return null();
    }
  }
||||||| merged common ancestors
        MUST_MATCH_TOKEN_MOD_WITH_REPORT(TokenKind::RightBracket, TokenStream::Operand,
                                         reportMissingClosing(JSMSG_BRACKET_AFTER_LIST,
                                                              JSMSG_BRACKET_OPENED, begin));
    }
=======
  if (tt == TokenKind::RightBracket) {
    /*
     * Mark empty arrays as non-constant, since we cannot easily
     * determine their type.
     */
    handler_.setListHasNonConstInitializer(literal);
  } else {
    anyChars.ungetToken();
>>>>>>> upstream-releases

<<<<<<< HEAD
  handler.setEndPosition(literal, pos().end);
  return literal;
}
||||||| merged common ancestors
    handler.setEndPosition(literal, pos().end);
    return literal;
}
=======
    for (uint32_t index = 0;; index++) {
      if (index >= NativeObject::MAX_DENSE_ELEMENTS_COUNT) {
        error(JSMSG_ARRAY_INIT_TOO_BIG);
        return null();
      }
>>>>>>> upstream-releases

<<<<<<< HEAD
template <class ParseHandler, typename Unit>
typename ParseHandler::Node GeneralParser<ParseHandler, Unit>::propertyName(
    YieldHandling yieldHandling, PropertyNameContext propertyNameContext,
    const Maybe<DeclarationKind>& maybeDecl, ListNodeType propList,
    PropertyType* propType, MutableHandleAtom propAtom) {
  TokenKind ltok;
  if (!tokenStream.getToken(&ltok)) {
    return null();
  }
||||||| merged common ancestors
template <class ParseHandler, typename Unit>
typename ParseHandler::Node
GeneralParser<ParseHandler, Unit>::propertyName(YieldHandling yieldHandling,
                                                const Maybe<DeclarationKind>& maybeDecl,
                                                ListNodeType propList,
                                                PropertyType* propType,
                                                MutableHandleAtom propAtom)
{
    TokenKind ltok;
    if (!tokenStream.getToken(&ltok)) {
        return null();
    }
=======
      TokenKind tt;
      if (!tokenStream.peekToken(&tt, TokenStream::SlashIsRegExp)) {
        return null();
      }
      if (tt == TokenKind::RightBracket) {
        break;
      }
>>>>>>> upstream-releases

<<<<<<< HEAD
  MOZ_ASSERT(ltok != TokenKind::RightCurly,
             "caller should have handled TokenKind::RightCurly");
||||||| merged common ancestors
    MOZ_ASSERT(ltok != TokenKind::RightCurly, "caller should have handled TokenKind::RightCurly");
=======
      if (tt == TokenKind::Comma) {
        tokenStream.consumeKnownToken(TokenKind::Comma,
                                      TokenStream::SlashIsRegExp);
        if (!handler_.addElision(literal, pos())) {
          return null();
        }
        continue;
      }
>>>>>>> upstream-releases

<<<<<<< HEAD
  bool isGenerator = false;
  bool isAsync = false;
||||||| merged common ancestors
    bool isGenerator = false;
    bool isAsync = false;
=======
      if (tt == TokenKind::TripleDot) {
        tokenStream.consumeKnownToken(TokenKind::TripleDot,
                                      TokenStream::SlashIsRegExp);
        uint32_t begin = pos().begin;
>>>>>>> upstream-releases

<<<<<<< HEAD
  if (ltok == TokenKind::Async) {
    // AsyncMethod[Yield, Await]:
    //   async [no LineTerminator here] PropertyName[?Yield, ?Await] ...
    //
    //  AsyncGeneratorMethod[Yield, Await]:
    //    async [no LineTerminator here] * PropertyName[?Yield, ?Await] ...
    //
    // PropertyName:
    //   LiteralPropertyName
    //   ComputedPropertyName[?Yield, ?Await]
    //
    // LiteralPropertyName:
    //   IdentifierName
    //   StringLiteral
    //   NumericLiteral
    //
    // ComputedPropertyName[Yield, Await]:
    //   [ ...
    TokenKind tt = TokenKind::Eof;
    if (!tokenStream.peekTokenSameLine(&tt)) {
      return null();
    }
    if (tt == TokenKind::String || tt == TokenKind::Number ||
        tt == TokenKind::LeftBracket || TokenKindIsPossibleIdentifierName(tt) ||
        tt == TokenKind::Mul) {
      isAsync = true;
      tokenStream.consumeKnownToken(tt);
      ltok = tt;
    }
  }
||||||| merged common ancestors
    if (ltok == TokenKind::Async) {
        // AsyncMethod[Yield, Await]:
        //   async [no LineTerminator here] PropertyName[?Yield, ?Await] ...
        //
        //  AsyncGeneratorMethod[Yield, Await]:
        //    async [no LineTerminator here] * PropertyName[?Yield, ?Await] ...
        //
        // PropertyName:
        //   LiteralPropertyName
        //   ComputedPropertyName[?Yield, ?Await]
        //
        // LiteralPropertyName:
        //   IdentifierName
        //   StringLiteral
        //   NumericLiteral
        //
        // ComputedPropertyName[Yield, Await]:
        //   [ ...
        TokenKind tt = TokenKind::Eof;
        if (!tokenStream.peekTokenSameLine(&tt)) {
            return null();
        }
        if (tt == TokenKind::String || tt == TokenKind::Number || tt == TokenKind::LeftBracket ||
            TokenKindIsPossibleIdentifierName(tt) || tt == TokenKind::Mul)
        {
            isAsync = true;
            tokenStream.consumeKnownToken(tt);
            ltok = tt;
        }
    }
=======
        TokenPos innerPos;
        if (!tokenStream.peekTokenPos(&innerPos, TokenStream::SlashIsRegExp)) {
          return null();
        }
>>>>>>> upstream-releases

<<<<<<< HEAD
  if (ltok == TokenKind::Mul) {
    isGenerator = true;
    if (!tokenStream.getToken(&ltok)) {
      return null();
    }
  }
||||||| merged common ancestors
    if (ltok == TokenKind::Mul) {
        isGenerator = true;
        if (!tokenStream.getToken(&ltok)) {
            return null();
        }
    }
=======
        PossibleError possibleErrorInner(*this);
        Node inner = assignExpr(InAllowed, yieldHandling, TripledotProhibited,
                                &possibleErrorInner);
        if (!inner) {
          return null();
        }
        if (!checkDestructuringAssignmentTarget(
                inner, innerPos, &possibleErrorInner, possibleError)) {
          return null();
        }
>>>>>>> upstream-releases

<<<<<<< HEAD
  propAtom.set(nullptr);
  Node propName;
  switch (ltok) {
    case TokenKind::Number:
      propAtom.set(NumberToAtom(context, anyChars.currentToken().number()));
      if (!propAtom.get()) {
        return null();
      }
      propName = newNumber(anyChars.currentToken());
      if (!propName) {
        return null();
      }
      break;
||||||| merged common ancestors
    propAtom.set(nullptr);
    Node propName;
    switch (ltok) {
      case TokenKind::Number:
        propAtom.set(NumberToAtom(context, anyChars.currentToken().number()));
        if (!propAtom.get()) {
            return null();
        }
        propName = newNumber(anyChars.currentToken());
        if (!propName) {
            return null();
        }
        break;
=======
        if (!handler_.addSpreadElement(literal, begin, inner)) {
          return null();
        }
      } else {
        TokenPos elementPos;
        if (!tokenStream.peekTokenPos(&elementPos,
                                      TokenStream::SlashIsRegExp)) {
          return null();
        }
>>>>>>> upstream-releases

<<<<<<< HEAD
    case TokenKind::String: {
      propAtom.set(anyChars.currentToken().atom());
      uint32_t index;
      if (propAtom->isIndex(&index)) {
        propName = handler.newNumber(index, NoDecimal, pos());
        if (!propName) {
          return null();
||||||| merged common ancestors
      case TokenKind::String: {
        propAtom.set(anyChars.currentToken().atom());
        uint32_t index;
        if (propAtom->isIndex(&index)) {
            propName = handler.newNumber(index, NoDecimal, pos());
            if (!propName) {
                return null();
            }
            break;
        }
        propName = stringLiteral();
        if (!propName) {
            return null();
=======
        PossibleError possibleErrorInner(*this);
        Node element = assignExpr(InAllowed, yieldHandling, TripledotProhibited,
                                  &possibleErrorInner);
        if (!element) {
          return null();
        }
        if (!checkDestructuringAssignmentElement(
                element, elementPos, &possibleErrorInner, possibleError)) {
          return null();
>>>>>>> upstream-releases
        }
        handler_.addArrayElement(literal, element);
      }
      propName = stringLiteral();
      if (!propName) {
        return null();
      }
      break;
    }

    case TokenKind::LeftBracket:
      propName = computedPropertyName(yieldHandling, maybeDecl, propList);
      if (!propName) {
        return null();
      }
      break;

    default: {
      if (!TokenKindIsPossibleIdentifierName(ltok)) {
        error(JSMSG_UNEXPECTED_TOKEN, "property name", TokenKindToDesc(ltok));
        return null();
      }

<<<<<<< HEAD
      propAtom.set(anyChars.currentName());
      // Do not look for accessor syntax on generator or async methods.
      if (isGenerator || isAsync ||
          !(ltok == TokenKind::Get || ltok == TokenKind::Set)) {
        propName = handler.newObjectLiteralPropertyName(propAtom, pos());
        if (!propName) {
          return null();
        }
||||||| merged common ancestors
      case TokenKind::LeftBracket:
        propName = computedPropertyName(yieldHandling, maybeDecl, propList);
        if (!propName) {
            return null();
        }
=======
      bool matched;
      if (!tokenStream.matchToken(&matched, TokenKind::Comma,
                                  TokenStream::SlashIsRegExp)) {
        return null();
      }
      if (!matched) {
>>>>>>> upstream-releases
        break;
      }

<<<<<<< HEAD
      *propType =
          ltok == TokenKind::Get ? PropertyType::Getter : PropertyType::Setter;

      // We have parsed |get| or |set|. Look for an accessor property
      // name next.
      TokenKind tt;
      if (!tokenStream.peekToken(&tt)) {
        return null();
      }
      if (TokenKindIsPossibleIdentifierName(tt)) {
        tokenStream.consumeKnownToken(tt);
||||||| merged common ancestors
      default: {
        if (!TokenKindIsPossibleIdentifierName(ltok)) {
            error(JSMSG_UNEXPECTED_TOKEN, "property name", TokenKindToDesc(ltok));
            return null();
        }
=======
      if (tt == TokenKind::TripleDot && possibleError) {
        possibleError->setPendingDestructuringErrorAt(pos(),
                                                      JSMSG_REST_WITH_COMMA);
      }
    }
>>>>>>> upstream-releases

<<<<<<< HEAD
        propAtom.set(anyChars.currentName());
        return handler.newObjectLiteralPropertyName(propAtom, pos());
      }
      if (tt == TokenKind::String) {
        tokenStream.consumeKnownToken(TokenKind::String);
||||||| merged common ancestors
        propAtom.set(anyChars.currentName());
        // Do not look for accessor syntax on generator or async methods.
        if (isGenerator || isAsync || !(ltok == TokenKind::Get || ltok == TokenKind::Set)) {
            propName = handler.newObjectLiteralPropertyName(propAtom, pos());
            if (!propName) {
                return null();
            }
            break;
        }
=======
    if (!mustMatchToken(
            TokenKind::RightBracket, [this, begin](TokenKind actual) {
              this->reportMissingClosing(JSMSG_BRACKET_AFTER_LIST,
                                         JSMSG_BRACKET_OPENED, begin);
            })) {
      return null();
    }
  }
>>>>>>> upstream-releases

<<<<<<< HEAD
        propAtom.set(anyChars.currentToken().atom());
||||||| merged common ancestors
        *propType = ltok == TokenKind::Get ? PropertyType::Getter : PropertyType::Setter;
=======
  handler_.setEndPosition(literal, pos().end);
  return literal;
}
>>>>>>> upstream-releases

<<<<<<< HEAD
        uint32_t index;
        if (propAtom->isIndex(&index)) {
          propAtom.set(NumberToAtom(context, index));
          if (!propAtom.get()) {
            return null();
          }
          return handler.newNumber(index, NoDecimal, pos());
        }
        return stringLiteral();
      }
      if (tt == TokenKind::Number) {
        tokenStream.consumeKnownToken(TokenKind::Number);
||||||| merged common ancestors
        // We have parsed |get| or |set|. Look for an accessor property
        // name next.
        TokenKind tt;
        if (!tokenStream.peekToken(&tt)) {
            return null();
        }
        if (TokenKindIsPossibleIdentifierName(tt)) {
            tokenStream.consumeKnownToken(tt);
=======
template <class ParseHandler, typename Unit>
typename ParseHandler::Node GeneralParser<ParseHandler, Unit>::propertyName(
    YieldHandling yieldHandling, PropertyNameContext propertyNameContext,
    const Maybe<DeclarationKind>& maybeDecl, ListNodeType propList,
    MutableHandleAtom propAtom) {
  // PropertyName[Yield, Await]:
  //   LiteralPropertyName
  //   ComputedPropertyName[?Yield, ?Await]
  //
  // LiteralPropertyName:
  //   IdentifierName
  //   StringLiteral
  //   NumericLiteral
  TokenKind ltok = anyChars.currentToken().type;
>>>>>>> upstream-releases

<<<<<<< HEAD
        propAtom.set(NumberToAtom(context, anyChars.currentToken().number()));
        if (!propAtom.get()) {
          return null();
        }
        return newNumber(anyChars.currentToken());
      }
      if (tt == TokenKind::LeftBracket) {
        tokenStream.consumeKnownToken(TokenKind::LeftBracket);

        return computedPropertyName(yieldHandling, maybeDecl, propList);
      }

      // Not an accessor property after all.
      propName = handler.newObjectLiteralPropertyName(propAtom.get(), pos());
      if (!propName) {
        return null();
      }
      break;
    }
  }
||||||| merged common ancestors
            propAtom.set(anyChars.currentName());
            return handler.newObjectLiteralPropertyName(propAtom, pos());
        }
        if (tt == TokenKind::String) {
            tokenStream.consumeKnownToken(TokenKind::String);

            propAtom.set(anyChars.currentToken().atom());

            uint32_t index;
            if (propAtom->isIndex(&index)) {
                propAtom.set(NumberToAtom(context, index));
                if (!propAtom.get()) {
                    return null();
                }
                return handler.newNumber(index, NoDecimal, pos());
            }
            return stringLiteral();
        }
        if (tt == TokenKind::Number) {
            tokenStream.consumeKnownToken(TokenKind::Number);
=======
  propAtom.set(nullptr);
  switch (ltok) {
    case TokenKind::Number:
      propAtom.set(NumberToAtom(cx_, anyChars.currentToken().number()));
      if (!propAtom.get()) {
        return null();
      }
      return newNumber(anyChars.currentToken());
>>>>>>> upstream-releases

<<<<<<< HEAD
  TokenKind tt;
  if (!tokenStream.getToken(&tt)) {
    return null();
  }
||||||| merged common ancestors
            propAtom.set(NumberToAtom(context, anyChars.currentToken().number()));
            if (!propAtom.get()) {
                return null();
            }
            return newNumber(anyChars.currentToken());
        }
        if (tt == TokenKind::LeftBracket) {
            tokenStream.consumeKnownToken(TokenKind::LeftBracket);
=======
    case TokenKind::String: {
      propAtom.set(anyChars.currentToken().atom());
      uint32_t index;
      if (propAtom->isIndex(&index)) {
        return handler_.newNumber(index, NoDecimal, pos());
      }
      return stringLiteral();
    }
>>>>>>> upstream-releases

<<<<<<< HEAD
  if (tt == TokenKind::Colon) {
    if (isGenerator || isAsync) {
      error(JSMSG_BAD_PROP_ID);
      return null();
    }
    *propType = PropertyType::Normal;
    return propName;
  }
||||||| merged common ancestors
            return computedPropertyName(yieldHandling, maybeDecl, propList);
        }
=======
    case TokenKind::LeftBracket:
      return computedPropertyName(yieldHandling, maybeDecl, propertyNameContext,
                                  propList);
>>>>>>> upstream-releases

<<<<<<< HEAD
  if (propertyNameContext == PropertyNameInClass &&
      (tt == TokenKind::Semi || tt == TokenKind::Assign)) {
    if (isGenerator || isAsync) {
      error(JSMSG_BAD_PROP_ID);
      return null();
    }
    anyChars.ungetToken();
    *propType = PropertyType::Field;
    return propName;
  }

  if (TokenKindIsPossibleIdentifierName(ltok) &&
      (tt == TokenKind::Comma || tt == TokenKind::RightCurly ||
       tt == TokenKind::Assign)) {
    if (isGenerator || isAsync) {
      error(JSMSG_BAD_PROP_ID);
      return null();
    }

    anyChars.ungetToken();
    anyChars.addModifierException(TokenStream::OperandIsNone);
    *propType = tt == TokenKind::Assign ? PropertyType::CoverInitializedName
                                        : PropertyType::Shorthand;
    return propName;
  }

  if (tt == TokenKind::LeftParen) {
    anyChars.ungetToken();

    if (isGenerator && isAsync) {
      *propType = PropertyType::AsyncGeneratorMethod;
    } else if (isGenerator) {
      *propType = PropertyType::GeneratorMethod;
    } else if (isAsync) {
      *propType = PropertyType::AsyncMethod;
    } else {
      *propType = PropertyType::Method;
    }
    return propName;
  }

  error(JSMSG_COLON_AFTER_ID);
  return null();
}

template <class ParseHandler, typename Unit>
typename ParseHandler::UnaryNodeType
GeneralParser<ParseHandler, Unit>::computedPropertyName(
    YieldHandling yieldHandling, const Maybe<DeclarationKind>& maybeDecl,
    ListNodeType literal) {
  MOZ_ASSERT(anyChars.isCurrentTokenType(TokenKind::LeftBracket));

  uint32_t begin = pos().begin;

  if (maybeDecl) {
    if (*maybeDecl == DeclarationKind::FormalParameter) {
      pc->functionBox()->hasParameterExprs = true;
||||||| merged common ancestors
        // Not an accessor property after all.
        propName = handler.newObjectLiteralPropertyName(propAtom.get(), pos());
        if (!propName) {
            return null();
        }
        break;
      }
=======
    default: {
      if (!TokenKindIsPossibleIdentifierName(ltok)) {
        error(JSMSG_UNEXPECTED_TOKEN, "property name", TokenKindToDesc(ltok));
        return null();
      }

      propAtom.set(anyChars.currentName());
      return handler_.newObjectLiteralPropertyName(propAtom, pos());
    }
  }
}

// True if `kind` can be the first token of a PropertyName.
static bool TokenKindCanStartPropertyName(TokenKind tt) {
  return TokenKindIsPossibleIdentifierName(tt) || tt == TokenKind::String ||
         tt == TokenKind::Number || tt == TokenKind::LeftBracket ||
         tt == TokenKind::Mul;
}

template <class ParseHandler, typename Unit>
typename ParseHandler::Node
GeneralParser<ParseHandler, Unit>::propertyOrMethodName(
    YieldHandling yieldHandling, PropertyNameContext propertyNameContext,
    const Maybe<DeclarationKind>& maybeDecl, ListNodeType propList,
    PropertyType* propType, MutableHandleAtom propAtom) {
  // We're parsing an object literal, class, or destructuring pattern;
  // propertyNameContext tells which one. This method parses any of the
  // following, storing the corresponding PropertyType in `*propType` to tell
  // the caller what we parsed:
  //
  //     async [no LineTerminator here] PropertyName
  //                            ==> PropertyType::AsyncMethod
  //     async [no LineTerminator here] * PropertyName
  //                            ==> PropertyType::AsyncGeneratorMethod
  //     * PropertyName         ==> PropertyType::GeneratorMethod
  //     get PropertyName       ==> PropertyType::Getter
  //     set PropertyName       ==> PropertyType::Setter
  //     PropertyName :         ==> PropertyType::Normal
  //     PropertyName           ==> see below
  //
  // In the last case, where there's not a `:` token to consume, we peek at
  // (but don't consume) the next token to decide how to set `*propType`.
  //
  //     `,` or `}`             ==> PropertyType::Shorthand
  //     `(`                    ==> PropertyType::Method
  //     `=`, not in a class    ==> PropertyType::CoverInitializedName
  //     '=', in a class        ==> PropertyType::Field
  //     any token, in a class  ==> PropertyType::Field (ASI)
  //
  // The caller must check `*propType` and throw if whatever we parsed isn't
  // allowed here (for example, a getter in a destructuring pattern).
  //
  // This method does *not* match `static` (allowed in classes) or `...`
  // (allowed in object literals and patterns). The caller must take care of
  // those before calling this method.

  TokenKind ltok;
  if (!tokenStream.getToken(&ltok, TokenStream::SlashIsInvalid)) {
    return null();
  }

  MOZ_ASSERT(ltok != TokenKind::RightCurly,
             "caller should have handled TokenKind::RightCurly");

  // Accept `async` and/or `*`, indicating an async or generator method;
  // or `get` or `set`, indicating an accessor.
  bool isGenerator = false;
  bool isAsync = false;
  bool isGetter = false;
  bool isSetter = false;

  if (ltok == TokenKind::Async) {
    // `async` is also a PropertyName by itself (it's a conditional keyword),
    // so peek at the next token to see if we're really looking at a method.
    TokenKind tt = TokenKind::Eof;
    if (!tokenStream.peekTokenSameLine(&tt)) {
      return null();
    }
    if (TokenKindCanStartPropertyName(tt)) {
      isAsync = true;
      tokenStream.consumeKnownToken(tt);
      ltok = tt;
>>>>>>> upstream-releases
    }
<<<<<<< HEAD
  } else {
    handler.setListHasNonConstInitializer(literal);
  }

  Node assignNode = assignExpr(InAllowed, yieldHandling, TripledotProhibited);
  if (!assignNode) {
    return null();
  }

  if (!mustMatchToken(TokenKind::RightBracket, TokenStream::Operand,
                      JSMSG_COMP_PROP_UNTERM_EXPR)) {
    return null();
  }
  return handler.newComputedName(assignNode, begin, pos().end);
}
||||||| merged common ancestors
=======
  }
>>>>>>> upstream-releases

<<<<<<< HEAD
template <class ParseHandler, typename Unit>
typename ParseHandler::ListNodeType
GeneralParser<ParseHandler, Unit>::objectLiteral(YieldHandling yieldHandling,
                                                 PossibleError* possibleError) {
  MOZ_ASSERT(anyChars.isCurrentTokenType(TokenKind::LeftCurly));

  uint32_t openedPos = pos().begin;

  ListNodeType literal = handler.newObjectLiteral(pos().begin);
  if (!literal) {
    return null();
  }

  bool seenPrototypeMutation = false;
  bool seenCoverInitializedName = false;
  Maybe<DeclarationKind> declKind = Nothing();
  RootedAtom propAtom(context);
  for (;;) {
||||||| merged common ancestors
=======
  if (ltok == TokenKind::Mul) {
    isGenerator = true;
    if (!tokenStream.getToken(&ltok)) {
      return null();
    }
  }

  if (!isAsync && !isGenerator &&
      (ltok == TokenKind::Get || ltok == TokenKind::Set)) {
    // We have parsed |get| or |set|. Look for an accessor property
    // name next.
>>>>>>> upstream-releases
    TokenKind tt;
<<<<<<< HEAD
    if (!tokenStream.peekToken(&tt)) {
      return null();
    }
    if (tt == TokenKind::RightCurly) {
      anyChars.addModifierException(TokenStream::OperandIsNone);
      break;
    }

    if (tt == TokenKind::TripleDot) {
      tokenStream.consumeKnownToken(TokenKind::TripleDot);
      uint32_t begin = pos().begin;

      TokenPos innerPos;
      if (!tokenStream.peekTokenPos(&innerPos, TokenStream::Operand)) {
        return null();
      }

      PossibleError possibleErrorInner(*this);
      Node inner = assignExpr(InAllowed, yieldHandling, TripledotProhibited,
                              &possibleErrorInner);
      if (!inner) {
        return null();
      }
      if (!checkDestructuringAssignmentTarget(
              inner, innerPos, &possibleErrorInner, possibleError,
              TargetBehavior::ForbidAssignmentPattern)) {
        return null();
      }
      if (!handler.addSpreadProperty(literal, begin, inner)) {
        return null();
      }
    } else {
      TokenPos namePos = anyChars.nextToken().pos;

      PropertyType propType;
      Node propName = propertyName(yieldHandling, PropertyNameInLiteral,
                                   declKind, literal, &propType, &propAtom);
      if (!propName) {
        return null();
      }
||||||| merged common ancestors
    if (!tokenStream.getToken(&tt)) {
        return null();
    }
=======
    if (!tokenStream.peekToken(&tt)) {
      return null();
    }
    if (TokenKindCanStartPropertyName(tt)) {
      tokenStream.consumeKnownToken(tt);
      isGetter = (ltok == TokenKind::Get);
      isSetter = (ltok == TokenKind::Set);
    }
  }

  Node propName = propertyName(yieldHandling, propertyNameContext, maybeDecl,
                               propList, propAtom);
  if (!propName) {
    return null();
  }

  // Grab the next token following the property/method name.
  // (If this isn't a colon, we're going to either put it back or throw.)
  TokenKind tt;
  if (!tokenStream.getToken(&tt)) {
    return null();
  }

  if (tt == TokenKind::Colon) {
    if (isGenerator || isAsync || isGetter || isSetter) {
      error(JSMSG_BAD_PROP_ID);
      return null();
    }
    *propType = PropertyType::Normal;
    return propName;
  }
>>>>>>> upstream-releases

<<<<<<< HEAD
      if (propType == PropertyType::Normal) {
        TokenPos exprPos;
        if (!tokenStream.peekTokenPos(&exprPos, TokenStream::Operand)) {
          return null();
        }
||||||| merged common ancestors
    if (tt == TokenKind::Colon) {
        if (isGenerator || isAsync) {
            error(JSMSG_BAD_PROP_ID);
            return null();
        }
        *propType = PropertyType::Normal;
        return propName;
    }
=======
  if (propertyNameContext != PropertyNameInClass &&
      TokenKindIsPossibleIdentifierName(ltok) &&
      (tt == TokenKind::Comma || tt == TokenKind::RightCurly ||
       tt == TokenKind::Assign)) {
    if (isGenerator || isAsync || isGetter || isSetter) {
      error(JSMSG_BAD_PROP_ID);
      return null();
    }
>>>>>>> upstream-releases

<<<<<<< HEAD
        PossibleError possibleErrorInner(*this);
        Node propExpr = assignExpr(InAllowed, yieldHandling,
                                   TripledotProhibited, &possibleErrorInner);
        if (!propExpr) {
          return null();
        }
||||||| merged common ancestors
    if (TokenKindIsPossibleIdentifierName(ltok) &&
        (tt == TokenKind::Comma || tt == TokenKind::RightCurly || tt == TokenKind::Assign))
    {
        if (isGenerator || isAsync) {
            error(JSMSG_BAD_PROP_ID);
            return null();
        }
=======
    anyChars.ungetToken();
    *propType = tt == TokenKind::Assign ? PropertyType::CoverInitializedName
                                        : PropertyType::Shorthand;
    return propName;
  }
>>>>>>> upstream-releases

<<<<<<< HEAD
        if (!checkDestructuringAssignmentElement(
                propExpr, exprPos, &possibleErrorInner, possibleError)) {
          return null();
        }
||||||| merged common ancestors
        anyChars.ungetToken();
        anyChars.addModifierException(TokenStream::OperandIsNone);
        *propType = tt == TokenKind::Assign
                    ? PropertyType::CoverInitializedName
                    : PropertyType::Shorthand;
        return propName;
    }
=======
  if (tt == TokenKind::LeftParen) {
    anyChars.ungetToken();
>>>>>>> upstream-releases

<<<<<<< HEAD
        if (propAtom == context->names().proto) {
          if (seenPrototypeMutation) {
            // Directly report the error when we're definitely not
            // in a destructuring context.
            if (!possibleError) {
              errorAt(namePos.begin, JSMSG_DUPLICATE_PROTO_PROPERTY);
              return null();
            }
||||||| merged common ancestors
    if (tt == TokenKind::LeftParen) {
        anyChars.ungetToken();
=======
    if (isGenerator && isAsync) {
      *propType = PropertyType::AsyncGeneratorMethod;
    } else if (isGenerator) {
      *propType = PropertyType::GeneratorMethod;
    } else if (isAsync) {
      *propType = PropertyType::AsyncMethod;
    } else if (isGetter) {
      *propType = PropertyType::Getter;
    } else if (isSetter) {
      *propType = PropertyType::Setter;
    } else {
      *propType = PropertyType::Method;
    }
    return propName;
  }
>>>>>>> upstream-releases

<<<<<<< HEAD
            // Otherwise delay error reporting until we've
            // determined whether or not we're destructuring.
            possibleError->setPendingExpressionErrorAt(
                namePos, JSMSG_DUPLICATE_PROTO_PROPERTY);
          }
          seenPrototypeMutation = true;
||||||| merged common ancestors
        if (isGenerator && isAsync) {
            *propType = PropertyType::AsyncGeneratorMethod;
        } else if (isGenerator) {
            *propType = PropertyType::GeneratorMethod;
        } else if (isAsync) {
            *propType = PropertyType::AsyncMethod;
        } else {
            *propType = PropertyType::Method;
        }
        return propName;
    }
=======
  if (propertyNameContext == PropertyNameInClass) {
    if (isGenerator || isAsync || isGetter || isSetter) {
      error(JSMSG_BAD_PROP_ID);
      return null();
    }
    anyChars.ungetToken();
    *propType = PropertyType::Field;
    return propName;
  }
>>>>>>> upstream-releases

<<<<<<< HEAD
          if (foldConstants && !FoldConstants(context, &propExpr, this)) {
            return null();
          }
||||||| merged common ancestors
    error(JSMSG_COLON_AFTER_ID);
    return null();
}
=======
  error(JSMSG_COLON_AFTER_ID);
  return null();
}
>>>>>>> upstream-releases

<<<<<<< HEAD
          // This occurs *only* if we observe PropertyType::Normal!
          // Only |__proto__: v| mutates [[Prototype]]. Getters,
          // setters, method/generator definitions, computed
          // property name versions of all of these, and shorthands
          // do not.
          if (!handler.addPrototypeMutation(literal, namePos.begin, propExpr)) {
            return null();
          }
        } else {
          // Use Node instead of BinaryNodeType to pass it to
          // FoldConstants.
          Node propDef = handler.newPropertyDefinition(propName, propExpr);
          if (!propDef) {
            return null();
          }
||||||| merged common ancestors
template <class ParseHandler, typename Unit>
typename ParseHandler::UnaryNodeType
GeneralParser<ParseHandler, Unit>::computedPropertyName(YieldHandling yieldHandling,
                                                        const Maybe<DeclarationKind>& maybeDecl,
                                                        ListNodeType literal)
{
    MOZ_ASSERT(anyChars.isCurrentTokenType(TokenKind::LeftBracket));
=======
template <class ParseHandler, typename Unit>
typename ParseHandler::UnaryNodeType
GeneralParser<ParseHandler, Unit>::computedPropertyName(
    YieldHandling yieldHandling, const Maybe<DeclarationKind>& maybeDecl,
    PropertyNameContext propertyNameContext, ListNodeType literal) {
  MOZ_ASSERT(anyChars.isCurrentTokenType(TokenKind::LeftBracket));
>>>>>>> upstream-releases

<<<<<<< HEAD
          if (foldConstants && !FoldConstants(context, &propDef, this)) {
            return null();
          }
||||||| merged common ancestors
    uint32_t begin = pos().begin;
=======
  uint32_t begin = pos().begin;
>>>>>>> upstream-releases

<<<<<<< HEAD
          handler.addPropertyDefinition(literal, handler.asBinary(propDef));
        }
      } else if (propType == PropertyType::Shorthand) {
        /*
         * Support, e.g., |({x, y} = o)| as destructuring shorthand
         * for |({x: x, y: y} = o)|, and |var o = {x, y}| as
         * initializer shorthand for |var o = {x: x, y: y}|.
         */
        Rooted<PropertyName*> name(context, identifierReference(yieldHandling));
        if (!name) {
          return null();
        }
||||||| merged common ancestors
    if (maybeDecl) {
        if (*maybeDecl == DeclarationKind::FormalParameter) {
            pc->functionBox()->hasParameterExprs = true;
        }
    } else {
        handler.setListHasNonConstInitializer(literal);
    }
=======
  if (maybeDecl) {
    if (*maybeDecl == DeclarationKind::FormalParameter) {
      pc_->functionBox()->hasParameterExprs = true;
    }
  } else if (propertyNameContext ==
             PropertyNameContext::PropertyNameInLiteral) {
    handler_.setListHasNonConstInitializer(literal);
  }
>>>>>>> upstream-releases

<<<<<<< HEAD
        NameNodeType nameExpr = identifierReference(name);
        if (!nameExpr) {
          return null();
        }
||||||| merged common ancestors
    Node assignNode = assignExpr(InAllowed, yieldHandling, TripledotProhibited);
    if (!assignNode) {
        return null();
    }
=======
  Node assignNode = assignExpr(InAllowed, yieldHandling, TripledotProhibited);
  if (!assignNode) {
    return null();
  }
>>>>>>> upstream-releases

<<<<<<< HEAD
        if (possibleError) {
          checkDestructuringAssignmentName(nameExpr, namePos, possibleError);
        }
||||||| merged common ancestors
    MUST_MATCH_TOKEN_MOD(TokenKind::RightBracket, TokenStream::Operand, JSMSG_COMP_PROP_UNTERM_EXPR);
    return handler.newComputedName(assignNode, begin, pos().end);
}
=======
  if (!mustMatchToken(TokenKind::RightBracket, JSMSG_COMP_PROP_UNTERM_EXPR)) {
    return null();
  }
  return handler_.newComputedName(assignNode, begin, pos().end);
}
>>>>>>> upstream-releases

<<<<<<< HEAD
        if (!handler.addShorthand(literal, handler.asName(propName),
                                  nameExpr)) {
          return null();
        }
      } else if (propType == PropertyType::CoverInitializedName) {
        /*
         * Support, e.g., |({x=1, y=2} = o)| as destructuring
         * shorthand with default values, as per ES6 12.14.5
         */
        Rooted<PropertyName*> name(context, identifierReference(yieldHandling));
        if (!name) {
          return null();
        }
||||||| merged common ancestors
template <class ParseHandler, typename Unit>
typename ParseHandler::ListNodeType
GeneralParser<ParseHandler, Unit>::objectLiteral(YieldHandling yieldHandling,
                                                 PossibleError* possibleError)
{
    MOZ_ASSERT(anyChars.isCurrentTokenType(TokenKind::LeftCurly));
=======
template <class ParseHandler, typename Unit>
typename ParseHandler::ListNodeType
GeneralParser<ParseHandler, Unit>::objectLiteral(YieldHandling yieldHandling,
                                                 PossibleError* possibleError) {
  MOZ_ASSERT(anyChars.isCurrentTokenType(TokenKind::LeftCurly));
>>>>>>> upstream-releases

<<<<<<< HEAD
        Node lhs = identifierReference(name);
        if (!lhs) {
          return null();
        }
||||||| merged common ancestors
    uint32_t openedPos = pos().begin;
=======
  uint32_t openedPos = pos().begin;
>>>>>>> upstream-releases

<<<<<<< HEAD
        tokenStream.consumeKnownToken(TokenKind::Assign);
||||||| merged common ancestors
    ListNodeType literal = handler.newObjectLiteral(pos().begin);
    if (!literal) {
        return null();
    }
=======
  ListNodeType literal = handler_.newObjectLiteral(pos().begin);
  if (!literal) {
    return null();
  }

  bool seenPrototypeMutation = false;
  bool seenCoverInitializedName = false;
  Maybe<DeclarationKind> declKind = Nothing();
  RootedAtom propAtom(cx_);
  for (;;) {
    TokenKind tt;
    if (!tokenStream.peekToken(&tt)) {
      return null();
    }
    if (tt == TokenKind::RightCurly) {
      break;
    }
>>>>>>> upstream-releases

<<<<<<< HEAD
        if (!seenCoverInitializedName) {
          // "shorthand default" or "CoverInitializedName" syntax is
          // only valid in the case of destructuring.
          seenCoverInitializedName = true;

          if (!possibleError) {
            // Destructuring defaults are definitely not allowed
            // in this object literal, because of something the
            // caller knows about the preceding code. For example,
            // maybe the preceding token is an operator:
            // |x + {y=z}|.
            error(JSMSG_COLON_AFTER_ID);
            return null();
          }

          // Here we set a pending error so that later in the parse,
          // once we've determined whether or not we're
          // destructuring, the error can be reported or ignored
          // appropriately.
          possibleError->setPendingExpressionErrorAt(pos(),
                                                     JSMSG_COLON_AFTER_ID);
||||||| merged common ancestors
    bool seenPrototypeMutation = false;
    bool seenCoverInitializedName = false;
    Maybe<DeclarationKind> declKind = Nothing();
    RootedAtom propAtom(context);
    for (;;) {
        TokenKind tt;
        if (!tokenStream.peekToken(&tt)) {
            return null();
        }
        if (tt == TokenKind::RightCurly) {
            anyChars.addModifierException(TokenStream::OperandIsNone);
            break;
=======
    if (tt == TokenKind::TripleDot) {
      tokenStream.consumeKnownToken(TokenKind::TripleDot);
      uint32_t begin = pos().begin;

      TokenPos innerPos;
      if (!tokenStream.peekTokenPos(&innerPos, TokenStream::SlashIsRegExp)) {
        return null();
      }

      PossibleError possibleErrorInner(*this);
      Node inner = assignExpr(InAllowed, yieldHandling, TripledotProhibited,
                              &possibleErrorInner);
      if (!inner) {
        return null();
      }
      if (!checkDestructuringAssignmentTarget(
              inner, innerPos, &possibleErrorInner, possibleError,
              TargetBehavior::ForbidAssignmentPattern)) {
        return null();
      }
      if (!handler_.addSpreadProperty(literal, begin, inner)) {
        return null();
      }
    } else {
      TokenPos namePos = anyChars.nextToken().pos;

      PropertyType propType;
      Node propName =
          propertyOrMethodName(yieldHandling, PropertyNameInLiteral, declKind,
                               literal, &propType, &propAtom);
      if (!propName) {
        return null();
      }

      if (propType == PropertyType::Normal) {
        TokenPos exprPos;
        if (!tokenStream.peekTokenPos(&exprPos, TokenStream::SlashIsRegExp)) {
          return null();
        }

        PossibleError possibleErrorInner(*this);
        Node propExpr = assignExpr(InAllowed, yieldHandling,
                                   TripledotProhibited, &possibleErrorInner);
        if (!propExpr) {
          return null();
>>>>>>> upstream-releases
        }

<<<<<<< HEAD
        if (const char* chars = nameIsArgumentsOrEval(lhs)) {
          // |chars| is "arguments" or "eval" here.
          if (!strictModeErrorAt(namePos.begin, JSMSG_BAD_STRICT_ASSIGN,
                                 chars)) {
            return null();
          }
        }
||||||| merged common ancestors
        if (tt == TokenKind::TripleDot) {
            tokenStream.consumeKnownToken(TokenKind::TripleDot);
            uint32_t begin = pos().begin;
=======
        if (!checkDestructuringAssignmentElement(
                propExpr, exprPos, &possibleErrorInner, possibleError)) {
          return null();
        }
>>>>>>> upstream-releases

<<<<<<< HEAD
        Node rhs = assignExpr(InAllowed, yieldHandling, TripledotProhibited);
        if (!rhs) {
          return null();
        }
||||||| merged common ancestors
            TokenPos innerPos;
            if (!tokenStream.peekTokenPos(&innerPos, TokenStream::Operand)) {
                return null();
            }
=======
        if (propAtom == cx_->names().proto) {
          if (seenPrototypeMutation) {
            // Directly report the error when we're definitely not
            // in a destructuring context.
            if (!possibleError) {
              errorAt(namePos.begin, JSMSG_DUPLICATE_PROTO_PROPERTY);
              return null();
            }
>>>>>>> upstream-releases

<<<<<<< HEAD
        BinaryNodeType propExpr =
            handler.newAssignment(ParseNodeKind::Assign, lhs, rhs);
        if (!propExpr) {
          return null();
        }
||||||| merged common ancestors
            PossibleError possibleErrorInner(*this);
            Node inner = assignExpr(InAllowed, yieldHandling, TripledotProhibited,
                                    &possibleErrorInner);
            if (!inner) {
                return null();
            }
            if (!checkDestructuringAssignmentTarget(inner, innerPos, &possibleErrorInner,
                                                    possibleError,
                                                    TargetBehavior::ForbidAssignmentPattern))
            {
                return null();
            }
            if (!handler.addSpreadProperty(literal, begin, inner)) {
                return null();
            }
        } else {
            TokenPos namePos = anyChars.nextToken().pos;
=======
            // Otherwise delay error reporting until we've
            // determined whether or not we're destructuring.
            possibleError->setPendingExpressionErrorAt(
                namePos, JSMSG_DUPLICATE_PROTO_PROPERTY);
          }
          seenPrototypeMutation = true;

          // This occurs *only* if we observe PropertyType::Normal!
          // Only |__proto__: v| mutates [[Prototype]]. Getters,
          // setters, method/generator definitions, computed
          // property name versions of all of these, and shorthands
          // do not.
          if (!handler_.addPrototypeMutation(literal, namePos.begin,
                                             propExpr)) {
            return null();
          }
        } else {
          BinaryNodeType propDef =
              handler_.newPropertyDefinition(propName, propExpr);
          if (!propDef) {
            return null();
          }
>>>>>>> upstream-releases

<<<<<<< HEAD
        if (!handler.addPropertyDefinition(literal, propName, propExpr)) {
          return null();
        }
      } else {
        RootedAtom funName(context);
        if (!anyChars.isCurrentTokenType(TokenKind::RightBracket)) {
          funName = propAtom;
||||||| merged common ancestors
            PropertyType propType;
            Node propName = propertyName(yieldHandling, declKind, literal, &propType, &propAtom);
            if (!propName) {
                return null();
            }
=======
          handler_.addPropertyDefinition(literal, propDef);
        }
      } else if (propType == PropertyType::Shorthand) {
        /*
         * Support, e.g., |({x, y} = o)| as destructuring shorthand
         * for |({x: x, y: y} = o)|, and |var o = {x, y}| as
         * initializer shorthand for |var o = {x: x, y: y}|.
         */
        Rooted<PropertyName*> name(cx_, identifierReference(yieldHandling));
        if (!name) {
          return null();
        }
>>>>>>> upstream-releases

<<<<<<< HEAD
          if (propType == PropertyType::Getter ||
              propType == PropertyType::Setter) {
            funName = prefixAccessorName(propType, propAtom);
            if (!funName) {
              return null();
            }
          }
||||||| merged common ancestors
            if (propType == PropertyType::Normal) {
                TokenPos exprPos;
                if (!tokenStream.peekTokenPos(&exprPos, TokenStream::Operand)) {
                    return null();
                }

                PossibleError possibleErrorInner(*this);
                Node propExpr = assignExpr(InAllowed, yieldHandling, TripledotProhibited,
                                           &possibleErrorInner);
                if (!propExpr) {
                    return null();
                }

                if (!checkDestructuringAssignmentElement(propExpr, exprPos, &possibleErrorInner,
                                                         possibleError))
                {
                    return null();
                }

                if (propAtom == context->names().proto) {
                    if (seenPrototypeMutation) {
                        // Directly report the error when we're definitely not
                        // in a destructuring context.
                        if (!possibleError) {
                            errorAt(namePos.begin, JSMSG_DUPLICATE_PROTO_PROPERTY);
                            return null();
                        }

                        // Otherwise delay error reporting until we've
                        // determined whether or not we're destructuring.
                        possibleError->setPendingExpressionErrorAt(namePos,
                                                                   JSMSG_DUPLICATE_PROTO_PROPERTY);
                    }
                    seenPrototypeMutation = true;

                    if (foldConstants && !FoldConstants(context, &propExpr, this)) {
                        return null();
                    }

                    // This occurs *only* if we observe PropertyType::Normal!
                    // Only |__proto__: v| mutates [[Prototype]]. Getters,
                    // setters, method/generator definitions, computed
                    // property name versions of all of these, and shorthands
                    // do not.
                    if (!handler.addPrototypeMutation(literal, namePos.begin, propExpr)) {
                        return null();
                    }
                } else {
                    // Use Node instead of BinaryNodeType to pass it to
                    // FoldConstants.
                    Node propDef = handler.newPropertyDefinition(propName, propExpr);
                    if (!propDef) {
                        return null();
                    }

                    if (foldConstants && !FoldConstants(context, &propDef, this)) {
                        return null();
                    }

                    handler.addPropertyDefinition(literal, handler.asBinary(propDef));
                }
            } else if (propType == PropertyType::Shorthand) {
                /*
                 * Support, e.g., |({x, y} = o)| as destructuring shorthand
                 * for |({x: x, y: y} = o)|, and |var o = {x, y}| as
                 * initializer shorthand for |var o = {x: x, y: y}|.
                 */
                Rooted<PropertyName*> name(context, identifierReference(yieldHandling));
                if (!name) {
                    return null();
                }

                NameNodeType nameExpr = identifierReference(name);
                if (!nameExpr) {
                    return null();
                }

                if (possibleError) {
                    checkDestructuringAssignmentName(nameExpr, namePos, possibleError);
                }

                if (!handler.addShorthand(literal, handler.asName(propName), nameExpr)) {
                    return null();
                }
            } else if (propType == PropertyType::CoverInitializedName) {
                /*
                 * Support, e.g., |({x=1, y=2} = o)| as destructuring
                 * shorthand with default values, as per ES6 12.14.5
                 */
                Rooted<PropertyName*> name(context, identifierReference(yieldHandling));
                if (!name) {
                    return null();
                }

                Node lhs = identifierReference(name);
                if (!lhs) {
                    return null();
                }

                tokenStream.consumeKnownToken(TokenKind::Assign);

                if (!seenCoverInitializedName) {
                    // "shorthand default" or "CoverInitializedName" syntax is
                    // only valid in the case of destructuring.
                    seenCoverInitializedName = true;

                    if (!possibleError) {
                        // Destructuring defaults are definitely not allowed
                        // in this object literal, because of something the
                        // caller knows about the preceding code. For example,
                        // maybe the preceding token is an operator:
                        // |x + {y=z}|.
                        error(JSMSG_COLON_AFTER_ID);
                        return null();
                    }

                    // Here we set a pending error so that later in the parse,
                    // once we've determined whether or not we're
                    // destructuring, the error can be reported or ignored
                    // appropriately.
                    possibleError->setPendingExpressionErrorAt(pos(), JSMSG_COLON_AFTER_ID);
                }

                if (const char* chars = nameIsArgumentsOrEval(lhs)) {
                    // |chars| is "arguments" or "eval" here.
                    if (!strictModeErrorAt(namePos.begin, JSMSG_BAD_STRICT_ASSIGN, chars)) {
                        return null();
                    }
                }

                Node rhs = assignExpr(InAllowed, yieldHandling, TripledotProhibited);
                if (!rhs) {
                    return null();
                }

                BinaryNodeType propExpr = handler.newAssignment(ParseNodeKind::Assign, lhs, rhs);
                if (!propExpr) {
                    return null();
                }

                if (!handler.addPropertyDefinition(literal, propName, propExpr)) {
                    return null();
                }
            } else {
                RootedAtom funName(context);
                if (!anyChars.isCurrentTokenType(TokenKind::RightBracket)) {
                    funName = propAtom;

                    if (propType == PropertyType::Getter || propType == PropertyType::Setter) {
                        funName = prefixAccessorName(propType, propAtom);
                        if (!funName) {
                            return null();
                        }
                    }
                }

                CodeNodeType funNode = methodDefinition(namePos.begin, propType, funName);
                if (!funNode) {
                    return null();
                }

                AccessorType atype = ToAccessorType(propType);
                if (!handler.addObjectMethodDefinition(literal, propName, funNode, atype)) {
                    return null();
                }

                if (possibleError) {
                    possibleError->setPendingDestructuringErrorAt(namePos,
                                                                  JSMSG_BAD_DESTRUCT_TARGET);
                }
            }
=======
        NameNodeType nameExpr = identifierReference(name);
        if (!nameExpr) {
          return null();
        }

        if (possibleError) {
          checkDestructuringAssignmentName(nameExpr, namePos, possibleError);
        }

        if (!handler_.addShorthand(literal, handler_.asName(propName),
                                   nameExpr)) {
          return null();
        }
      } else if (propType == PropertyType::CoverInitializedName) {
        /*
         * Support, e.g., |({x=1, y=2} = o)| as destructuring
         * shorthand with default values, as per ES6 12.14.5
         */
        Rooted<PropertyName*> name(cx_, identifierReference(yieldHandling));
        if (!name) {
          return null();
        }

        Node lhs = identifierReference(name);
        if (!lhs) {
          return null();
>>>>>>> upstream-releases
        }

<<<<<<< HEAD
        CodeNodeType funNode =
            methodDefinition(namePos.begin, propType, funName);
        if (!funNode) {
          return null();
||||||| merged common ancestors
        bool matched;
        if (!tokenStream.matchToken(&matched, TokenKind::Comma, TokenStream::Operand)) {
            return null();
=======
        tokenStream.consumeKnownToken(TokenKind::Assign);

        if (!seenCoverInitializedName) {
          // "shorthand default" or "CoverInitializedName" syntax is
          // only valid in the case of destructuring.
          seenCoverInitializedName = true;

          if (!possibleError) {
            // Destructuring defaults are definitely not allowed
            // in this object literal, because of something the
            // caller knows about the preceding code. For example,
            // maybe the preceding token is an operator:
            // |x + {y=z}|.
            error(JSMSG_COLON_AFTER_ID);
            return null();
          }

          // Here we set a pending error so that later in the parse,
          // once we've determined whether or not we're
          // destructuring, the error can be reported or ignored
          // appropriately.
          possibleError->setPendingExpressionErrorAt(pos(),
                                                     JSMSG_COLON_AFTER_ID);
>>>>>>> upstream-releases
        }
<<<<<<< HEAD

        AccessorType atype = ToAccessorType(propType);
        if (!handler.addObjectMethodDefinition(literal, propName, funNode,
                                               atype)) {
          return null();
||||||| merged common ancestors
        if (!matched) {
            break;
=======

        if (const char* chars = nameIsArgumentsOrEval(lhs)) {
          // |chars| is "arguments" or "eval" here.
          if (!strictModeErrorAt(namePos.begin, JSMSG_BAD_STRICT_ASSIGN,
                                 chars)) {
            return null();
          }
        }

        Node rhs = assignExpr(InAllowed, yieldHandling, TripledotProhibited);
        if (!rhs) {
          return null();
        }

        BinaryNodeType propExpr =
            handler_.newAssignment(ParseNodeKind::AssignExpr, lhs, rhs);
        if (!propExpr) {
          return null();
        }

        if (!handler_.addPropertyDefinition(literal, propName, propExpr)) {
          return null();
        }
      } else {
        RootedAtom funName(cx_);
        if (!anyChars.isCurrentTokenType(TokenKind::RightBracket)) {
          funName = propAtom;

          if (propType == PropertyType::Getter ||
              propType == PropertyType::Setter) {
            funName = prefixAccessorName(propType, propAtom);
            if (!funName) {
              return null();
            }
          }
        }

        FunctionNodeType funNode =
            methodDefinition(namePos.begin, propType, funName);
        if (!funNode) {
          return null();
>>>>>>> upstream-releases
        }
<<<<<<< HEAD

        if (possibleError) {
          possibleError->setPendingDestructuringErrorAt(
              namePos, JSMSG_BAD_DESTRUCT_TARGET);
||||||| merged common ancestors
        if (tt == TokenKind::TripleDot && possibleError) {
            possibleError->setPendingDestructuringErrorAt(pos(), JSMSG_REST_WITH_COMMA);
=======

        AccessorType atype = ToAccessorType(propType);
        if (!handler_.addObjectMethodDefinition(literal, propName, funNode,
                                                atype)) {
          return null();
        }

        if (possibleError) {
          possibleError->setPendingDestructuringErrorAt(
              namePos, JSMSG_BAD_DESTRUCT_TARGET);
>>>>>>> upstream-releases
        }
<<<<<<< HEAD
      }
    }

    bool matched;
    if (!tokenStream.matchToken(&matched, TokenKind::Comma,
                                TokenStream::Operand)) {
      return null();
    }
    if (!matched) {
      break;
||||||| merged common ancestors
=======
      }
    }

    bool matched;
    if (!tokenStream.matchToken(&matched, TokenKind::Comma,
                                TokenStream::SlashIsInvalid)) {
      return null();
    }
    if (!matched) {
      break;
    }
    if (tt == TokenKind::TripleDot && possibleError) {
      possibleError->setPendingDestructuringErrorAt(pos(),
                                                    JSMSG_REST_WITH_COMMA);
>>>>>>> upstream-releases
    }
<<<<<<< HEAD
    if (tt == TokenKind::TripleDot && possibleError) {
      possibleError->setPendingDestructuringErrorAt(pos(),
                                                    JSMSG_REST_WITH_COMMA);
    }
  }
||||||| merged common ancestors
=======
  }
>>>>>>> upstream-releases

<<<<<<< HEAD
  if (!mustMatchToken(TokenKind::RightCurly, TokenStream::Operand,
                      [this, openedPos](TokenKind actual) {
                        this->reportMissingClosing(JSMSG_CURLY_AFTER_LIST,
                                                   JSMSG_CURLY_OPENED,
                                                   openedPos);
                      })) {
    return null();
  }
||||||| merged common ancestors
    MUST_MATCH_TOKEN_MOD_WITH_REPORT(TokenKind::RightCurly, TokenStream::Operand,
                                     reportMissingClosing(JSMSG_CURLY_AFTER_LIST,
                                                          JSMSG_CURLY_OPENED, openedPos));
=======
  if (!mustMatchToken(
          TokenKind::RightCurly, [this, openedPos](TokenKind actual) {
            this->reportMissingClosing(JSMSG_CURLY_AFTER_LIST,
                                       JSMSG_CURLY_OPENED, openedPos);
          })) {
    return null();
  }
>>>>>>> upstream-releases

<<<<<<< HEAD
  handler.setEndPosition(literal, pos().end);
  return literal;
||||||| merged common ancestors
    handler.setEndPosition(literal, pos().end);
    return literal;
=======
  handler_.setEndPosition(literal, pos().end);
  return literal;
>>>>>>> upstream-releases
}

template <class ParseHandler, typename Unit>
<<<<<<< HEAD
typename ParseHandler::CodeNodeType
GeneralParser<ParseHandler, Unit>::methodDefinition(uint32_t toStringStart,
                                                    PropertyType propType,
                                                    HandleAtom funName) {
  FunctionSyntaxKind kind;
  switch (propType) {
    case PropertyType::Getter:
      kind = FunctionSyntaxKind::Getter;
      break;

    case PropertyType::Setter:
      kind = FunctionSyntaxKind::Setter;
      break;

    case PropertyType::Method:
    case PropertyType::GeneratorMethod:
    case PropertyType::AsyncMethod:
    case PropertyType::AsyncGeneratorMethod:
      kind = FunctionSyntaxKind::Method;
      break;

    case PropertyType::Constructor:
      kind = FunctionSyntaxKind::ClassConstructor;
      break;

    case PropertyType::DerivedConstructor:
      kind = FunctionSyntaxKind::DerivedClassConstructor;
      break;

    default:
      MOZ_CRASH("unexpected property type");
  }

  GeneratorKind generatorKind = (propType == PropertyType::GeneratorMethod ||
                                 propType == PropertyType::AsyncGeneratorMethod)
                                    ? GeneratorKind::Generator
                                    : GeneratorKind::NotGenerator;

  FunctionAsyncKind asyncKind = (propType == PropertyType::AsyncMethod ||
                                 propType == PropertyType::AsyncGeneratorMethod)
                                    ? FunctionAsyncKind::AsyncFunction
                                    : FunctionAsyncKind::SyncFunction;

  YieldHandling yieldHandling = GetYieldHandling(generatorKind);

  CodeNodeType funNode = handler.newFunctionExpression(pos());
  if (!funNode) {
    return null();
  }
||||||| merged common ancestors
typename ParseHandler::CodeNodeType
GeneralParser<ParseHandler, Unit>::methodDefinition(uint32_t toStringStart, PropertyType propType,
                                                    HandleAtom funName)
{
    FunctionSyntaxKind kind;
    switch (propType) {
      case PropertyType::Getter:
        kind = FunctionSyntaxKind::Getter;
        break;
=======
typename ParseHandler::FunctionNodeType
GeneralParser<ParseHandler, Unit>::methodDefinition(uint32_t toStringStart,
                                                    PropertyType propType,
                                                    HandleAtom funName) {
  FunctionSyntaxKind syntaxKind;
  switch (propType) {
    case PropertyType::Getter:
      syntaxKind = FunctionSyntaxKind::Getter;
      break;
>>>>>>> upstream-releases

<<<<<<< HEAD
  return functionDefinition(funNode, toStringStart, InAllowed, yieldHandling,
                            funName, kind, generatorKind, asyncKind);
}
||||||| merged common ancestors
      case PropertyType::Setter:
        kind = FunctionSyntaxKind::Setter;
        break;
=======
    case PropertyType::Setter:
      syntaxKind = FunctionSyntaxKind::Setter;
      break;
>>>>>>> upstream-releases

<<<<<<< HEAD
template <class ParseHandler, typename Unit>
bool GeneralParser<ParseHandler, Unit>::tryNewTarget(
    BinaryNodeType* newTarget) {
  MOZ_ASSERT(anyChars.isCurrentTokenType(TokenKind::New));
||||||| merged common ancestors
      case PropertyType::Method:
      case PropertyType::GeneratorMethod:
      case PropertyType::AsyncMethod:
      case PropertyType::AsyncGeneratorMethod:
        kind = FunctionSyntaxKind::Method;
        break;
=======
    case PropertyType::Method:
    case PropertyType::GeneratorMethod:
    case PropertyType::AsyncMethod:
    case PropertyType::AsyncGeneratorMethod:
      syntaxKind = FunctionSyntaxKind::Method;
      break;
>>>>>>> upstream-releases

<<<<<<< HEAD
  *newTarget = null();
||||||| merged common ancestors
      case PropertyType::Constructor:
        kind = FunctionSyntaxKind::ClassConstructor;
        break;
=======
    case PropertyType::Constructor:
      syntaxKind = FunctionSyntaxKind::ClassConstructor;
      break;
>>>>>>> upstream-releases

<<<<<<< HEAD
  NullaryNodeType newHolder = handler.newPosHolder(pos());
  if (!newHolder) {
    return false;
  }
||||||| merged common ancestors
      case PropertyType::DerivedConstructor:
        kind = FunctionSyntaxKind::DerivedClassConstructor;
        break;
=======
    case PropertyType::DerivedConstructor:
      syntaxKind = FunctionSyntaxKind::DerivedClassConstructor;
      break;
>>>>>>> upstream-releases

<<<<<<< HEAD
  uint32_t begin = pos().begin;

  // |new| expects to look for an operand, so we will honor that.
  TokenKind next;
  if (!tokenStream.getToken(&next, TokenStream::Operand)) {
    return false;
  }
||||||| merged common ancestors
      default:
        MOZ_CRASH("unexpected property type");
    }
=======
    default:
      MOZ_CRASH("unexpected property type");
  }
>>>>>>> upstream-releases

<<<<<<< HEAD
  // Don't unget the token, since lookahead cannot handle someone calling
  // getToken() with a different modifier. Callers should inspect
  // currentToken().
  if (next != TokenKind::Dot) {
    return true;
  }
||||||| merged common ancestors
    GeneratorKind generatorKind = (propType == PropertyType::GeneratorMethod ||
                                   propType == PropertyType::AsyncGeneratorMethod)
                                  ? GeneratorKind::Generator
                                  : GeneratorKind::NotGenerator;
=======
  GeneratorKind generatorKind = (propType == PropertyType::GeneratorMethod ||
                                 propType == PropertyType::AsyncGeneratorMethod)
                                    ? GeneratorKind::Generator
                                    : GeneratorKind::NotGenerator;
>>>>>>> upstream-releases

<<<<<<< HEAD
  if (!tokenStream.getToken(&next)) {
    return false;
  }
  if (next != TokenKind::Target) {
    error(JSMSG_UNEXPECTED_TOKEN, "target", TokenKindToDesc(next));
    return false;
  }
||||||| merged common ancestors
    FunctionAsyncKind asyncKind = (propType == PropertyType::AsyncMethod ||
                                   propType == PropertyType::AsyncGeneratorMethod)
                                  ? FunctionAsyncKind::AsyncFunction
                                  : FunctionAsyncKind::SyncFunction;
=======
  FunctionAsyncKind asyncKind = (propType == PropertyType::AsyncMethod ||
                                 propType == PropertyType::AsyncGeneratorMethod)
                                    ? FunctionAsyncKind::AsyncFunction
                                    : FunctionAsyncKind::SyncFunction;
>>>>>>> upstream-releases

<<<<<<< HEAD
  if (!pc->sc()->allowNewTarget()) {
    errorAt(begin, JSMSG_BAD_NEWTARGET);
    return false;
  }
||||||| merged common ancestors
    YieldHandling yieldHandling = GetYieldHandling(generatorKind);
=======
  YieldHandling yieldHandling = GetYieldHandling(generatorKind);
>>>>>>> upstream-releases

<<<<<<< HEAD
  NullaryNodeType targetHolder = handler.newPosHolder(pos());
  if (!targetHolder) {
    return false;
  }
||||||| merged common ancestors
    CodeNodeType funNode = handler.newFunctionExpression(pos());
    if (!funNode) {
        return null();
    }
=======
  FunctionNodeType funNode = handler_.newFunction(syntaxKind, pos());
  if (!funNode) {
    return null();
  }
>>>>>>> upstream-releases

<<<<<<< HEAD
  *newTarget = handler.newNewTarget(newHolder, targetHolder);
  return !!*newTarget;
||||||| merged common ancestors
    return functionDefinition(funNode, toStringStart, InAllowed, yieldHandling, funName, kind,
                              generatorKind, asyncKind);
=======
  return functionDefinition(funNode, toStringStart, InAllowed, yieldHandling,
                            funName, syntaxKind, generatorKind, asyncKind);
>>>>>>> upstream-releases
}

template <class ParseHandler, typename Unit>
<<<<<<< HEAD
typename ParseHandler::BinaryNodeType
GeneralParser<ParseHandler, Unit>::importExpr(YieldHandling yieldHandling,
                                              bool allowCallSyntax) {
  MOZ_ASSERT(anyChars.isCurrentTokenType(TokenKind::Import));
||||||| merged common ancestors
bool
GeneralParser<ParseHandler, Unit>::tryNewTarget(BinaryNodeType* newTarget)
{
    MOZ_ASSERT(anyChars.isCurrentTokenType(TokenKind::New));

    *newTarget = null();
=======
bool GeneralParser<ParseHandler, Unit>::tryNewTarget(
    BinaryNodeType* newTarget) {
  MOZ_ASSERT(anyChars.isCurrentTokenType(TokenKind::New));

  *newTarget = null();
>>>>>>> upstream-releases

<<<<<<< HEAD
  NullaryNodeType importHolder = handler.newPosHolder(pos());
  if (!importHolder) {
    return null();
  }
||||||| merged common ancestors
    NullaryNodeType newHolder = handler.newPosHolder(pos());
    if (!newHolder) {
        return false;
    }
=======
  NullaryNodeType newHolder = handler_.newPosHolder(pos());
  if (!newHolder) {
    return false;
  }
>>>>>>> upstream-releases

<<<<<<< HEAD
  TokenKind next;
  if (!tokenStream.getToken(&next)) {
    return null();
  }
||||||| merged common ancestors
    uint32_t begin = pos().begin;
=======
  uint32_t begin = pos().begin;
>>>>>>> upstream-releases

<<<<<<< HEAD
  if (next == TokenKind::Dot) {
    if (!tokenStream.getToken(&next)) {
      return null();
    }
    if (next != TokenKind::Meta) {
      error(JSMSG_UNEXPECTED_TOKEN, "meta", TokenKindToDesc(next));
      return null();
    }
||||||| merged common ancestors
    // |new| expects to look for an operand, so we will honor that.
    TokenKind next;
    if (!tokenStream.getToken(&next, TokenStream::Operand)) {
        return false;
    }
=======
  // |new| expects to look for an operand, so we will honor that.
  TokenKind next;
  if (!tokenStream.getToken(&next, TokenStream::SlashIsRegExp)) {
    return false;
  }
>>>>>>> upstream-releases

<<<<<<< HEAD
    if (parseGoal() != ParseGoal::Module) {
      errorAt(pos().begin, JSMSG_IMPORT_META_OUTSIDE_MODULE);
      return null();
    }
||||||| merged common ancestors
    // Don't unget the token, since lookahead cannot handle someone calling
    // getToken() with a different modifier. Callers should inspect currentToken().
    if (next != TokenKind::Dot) {
        return true;
    }
=======
  // Don't unget the token, since lookahead cannot handle someone calling
  // getToken() with a different modifier. Callers should inspect
  // currentToken().
  if (next != TokenKind::Dot) {
    return true;
  }
>>>>>>> upstream-releases

<<<<<<< HEAD
    NullaryNodeType metaHolder = handler.newPosHolder(pos());
    if (!metaHolder) {
      return null();
    }

    return handler.newImportMeta(importHolder, metaHolder);
  } else if (next == TokenKind::LeftParen && allowCallSyntax) {
    Node arg = assignExpr(InAllowed, yieldHandling, TripledotProhibited);
    if (!arg) {
      return null();
    }
||||||| merged common ancestors
    if (!tokenStream.getToken(&next)) {
        return false;
    }
    if (next != TokenKind::Target) {
        error(JSMSG_UNEXPECTED_TOKEN, "target", TokenKindToDesc(next));
        return false;
    }
=======
  if (!tokenStream.getToken(&next)) {
    return false;
  }
  if (next != TokenKind::Target) {
    error(JSMSG_UNEXPECTED_TOKEN, "target", TokenKindToDesc(next));
    return false;
  }
>>>>>>> upstream-releases

<<<<<<< HEAD
    if (!mustMatchToken(TokenKind::RightParen, TokenStream::Operand,
                        JSMSG_PAREN_AFTER_ARGS)) {
      return null();
    }
||||||| merged common ancestors
    if (!pc->sc()->allowNewTarget()) {
        errorAt(begin, JSMSG_BAD_NEWTARGET);
        return false;
    }
=======
  if (!pc_->sc()->allowNewTarget()) {
    errorAt(begin, JSMSG_BAD_NEWTARGET);
    return false;
  }
>>>>>>> upstream-releases

<<<<<<< HEAD
    if (!context->runtime()->moduleDynamicImportHook) {
      error(JSMSG_NO_DYNAMIC_IMPORT);
      return null();
    }
||||||| merged common ancestors
    NullaryNodeType targetHolder = handler.newPosHolder(pos());
    if (!targetHolder) {
        return false;
    }
=======
  NullaryNodeType targetHolder = handler_.newPosHolder(pos());
  if (!targetHolder) {
    return false;
  }
>>>>>>> upstream-releases

<<<<<<< HEAD
    return handler.newCallImport(importHolder, arg);
  } else {
    error(JSMSG_UNEXPECTED_TOKEN_NO_EXPECT, TokenKindToDesc(next));
    return null();
  }
||||||| merged common ancestors
    *newTarget = handler.newNewTarget(newHolder, targetHolder);
    return !!*newTarget;
=======
  *newTarget = handler_.newNewTarget(newHolder, targetHolder);
  return !!*newTarget;
>>>>>>> upstream-releases
}

template <class ParseHandler, typename Unit>
<<<<<<< HEAD
typename ParseHandler::Node GeneralParser<ParseHandler, Unit>::primaryExpr(
    YieldHandling yieldHandling, TripledotHandling tripledotHandling,
    TokenKind tt, PossibleError* possibleError,
    InvokedPrediction invoked /* = PredictUninvoked */) {
  MOZ_ASSERT(anyChars.isCurrentTokenType(tt));
  if (!CheckRecursionLimit(context)) {
    return null();
  }
||||||| merged common ancestors
typename ParseHandler::BinaryNodeType
GeneralParser<ParseHandler, Unit>::importMeta()
{
    MOZ_ASSERT(anyChars.isCurrentTokenType(TokenKind::Import));
=======
typename ParseHandler::BinaryNodeType
GeneralParser<ParseHandler, Unit>::importExpr(YieldHandling yieldHandling,
                                              bool allowCallSyntax) {
  MOZ_ASSERT(anyChars.isCurrentTokenType(TokenKind::Import));
>>>>>>> upstream-releases

<<<<<<< HEAD
  switch (tt) {
    case TokenKind::Function:
      return functionExpr(pos().begin, invoked,
                          FunctionAsyncKind::SyncFunction);
||||||| merged common ancestors
    uint32_t begin = pos().begin;
=======
  NullaryNodeType importHolder = handler_.newPosHolder(pos());
  if (!importHolder) {
    return null();
  }
>>>>>>> upstream-releases

<<<<<<< HEAD
    case TokenKind::Class:
      return classDefinition(yieldHandling, ClassExpression, NameRequired);
||||||| merged common ancestors
    if (parseGoal() != ParseGoal::Module) {
        errorAt(begin, JSMSG_IMPORT_OUTSIDE_MODULE);
        return null();
    }
=======
  TokenKind next;
  if (!tokenStream.getToken(&next)) {
    return null();
  }
>>>>>>> upstream-releases

<<<<<<< HEAD
    case TokenKind::LeftBracket:
      return arrayInitializer(yieldHandling, possibleError);
||||||| merged common ancestors
    NullaryNodeType importHolder = handler.newPosHolder(pos());
    if (!importHolder) {
        return null();
    }
=======
  if (next == TokenKind::Dot) {
    if (!tokenStream.getToken(&next)) {
      return null();
    }
    if (next != TokenKind::Meta) {
      error(JSMSG_UNEXPECTED_TOKEN, "meta", TokenKindToDesc(next));
      return null();
    }
>>>>>>> upstream-releases

<<<<<<< HEAD
    case TokenKind::LeftCurly:
      return objectLiteral(yieldHandling, possibleError);
||||||| merged common ancestors
    TokenKind next;
    if (!tokenStream.getToken(&next)) {
        return null();
    }
    if (next != TokenKind::Dot) {
        error(JSMSG_UNEXPECTED_TOKEN, "dot", TokenKindToDesc(next));
        return null();
    }
=======
    if (parseGoal() != ParseGoal::Module) {
      errorAt(pos().begin, JSMSG_IMPORT_META_OUTSIDE_MODULE);
      return null();
    }

    NullaryNodeType metaHolder = handler_.newPosHolder(pos());
    if (!metaHolder) {
      return null();
    }
>>>>>>> upstream-releases

<<<<<<< HEAD
    case TokenKind::LeftParen: {
      TokenKind next;
      if (!tokenStream.peekToken(&next, TokenStream::Operand)) {
        return null();
      }
||||||| merged common ancestors
    if (!tokenStream.getToken(&next)) {
        return null();
    }
    if (next != TokenKind::Meta) {
        error(JSMSG_UNEXPECTED_TOKEN, "meta", TokenKindToDesc(next));
        return null();
    }
=======
    return handler_.newImportMeta(importHolder, metaHolder);
  } else if (next == TokenKind::LeftParen && allowCallSyntax) {
    Node arg = assignExpr(InAllowed, yieldHandling, TripledotProhibited);
    if (!arg) {
      return null();
    }

    if (!mustMatchToken(TokenKind::RightParen, JSMSG_PAREN_AFTER_ARGS)) {
      return null();
    }
>>>>>>> upstream-releases

<<<<<<< HEAD
      if (next == TokenKind::RightParen) {
        // Not valid expression syntax, but this is valid in an arrow function
        // with no params: `() => body`.
        tokenStream.consumeKnownToken(TokenKind::RightParen,
                                      TokenStream::Operand);

        if (!tokenStream.peekToken(&next)) {
          return null();
        }
        if (next != TokenKind::Arrow) {
          error(JSMSG_UNEXPECTED_TOKEN, "expression",
                TokenKindToDesc(TokenKind::RightParen));
          return null();
        }
||||||| merged common ancestors
    NullaryNodeType metaHolder = handler.newPosHolder(pos());
    if (!metaHolder) {
        return null();
        }
=======
    if (!cx_->runtime()->moduleDynamicImportHook) {
      error(JSMSG_NO_DYNAMIC_IMPORT);
      return null();
    }
>>>>>>> upstream-releases

<<<<<<< HEAD
        // Now just return something that will allow parsing to continue.
        // It doesn't matter what; when we reach the =>, we will rewind and
        // reparse the whole arrow function. See Parser::assignExpr.
        return handler.newNullLiteral(pos());
      }
||||||| merged common ancestors
    return handler.newImportMeta(importHolder, metaHolder);
}
=======
    return handler_.newCallImport(importHolder, arg);
  } else {
    error(JSMSG_UNEXPECTED_TOKEN_NO_EXPECT, TokenKindToDesc(next));
    return null();
  }
}
>>>>>>> upstream-releases

<<<<<<< HEAD
      // Pass |possibleError| to support destructuring in arrow parameters.
      Node expr = exprInParens(InAllowed, yieldHandling, TripledotAllowed,
                               possibleError);
      if (!expr) {
        return null();
      }
      if (!mustMatchToken(TokenKind::RightParen, TokenStream::Operand,
                          JSMSG_PAREN_IN_PAREN)) {
        return null();
      }
      return handler.parenthesize(expr);
    }

    case TokenKind::TemplateHead:
      return templateLiteral(yieldHandling);
||||||| merged common ancestors
template <class ParseHandler, typename Unit>
typename ParseHandler::Node
GeneralParser<ParseHandler, Unit>::primaryExpr(YieldHandling yieldHandling,
                                               TripledotHandling tripledotHandling,
                                               TokenKind tt, PossibleError* possibleError,
                                               InvokedPrediction invoked /* = PredictUninvoked */)
{
    MOZ_ASSERT(anyChars.isCurrentTokenType(tt));
    if (!CheckRecursionLimit(context)) {
        return null();
    }

    switch (tt) {
      case TokenKind::Function:
        return functionExpr(pos().begin, invoked, FunctionAsyncKind::SyncFunction);

      case TokenKind::Class:
        return classDefinition(yieldHandling, ClassExpression, NameRequired);

      case TokenKind::LeftBracket:
        return arrayInitializer(yieldHandling, possibleError);
=======
template <class ParseHandler, typename Unit>
typename ParseHandler::Node GeneralParser<ParseHandler, Unit>::primaryExpr(
    YieldHandling yieldHandling, TripledotHandling tripledotHandling,
    TokenKind tt, PossibleError* possibleError,
    InvokedPrediction invoked /* = PredictUninvoked */) {
  MOZ_ASSERT(anyChars.isCurrentTokenType(tt));
  if (!CheckRecursionLimit(cx_)) {
    return null();
  }

  switch (tt) {
    case TokenKind::Function:
      return functionExpr(pos().begin, invoked,
                          FunctionAsyncKind::SyncFunction);

    case TokenKind::Class:
      return classDefinition(yieldHandling, ClassExpression, NameRequired);
>>>>>>> upstream-releases

<<<<<<< HEAD
    case TokenKind::NoSubsTemplate:
      return noSubstitutionUntaggedTemplate();
||||||| merged common ancestors
      case TokenKind::LeftCurly:
        return objectLiteral(yieldHandling, possibleError);
=======
    case TokenKind::LeftBracket:
      return arrayInitializer(yieldHandling, possibleError);
>>>>>>> upstream-releases

<<<<<<< HEAD
    case TokenKind::String:
      return stringLiteral();
||||||| merged common ancestors
      case TokenKind::LeftParen: {
        TokenKind next;
        if (!tokenStream.peekToken(&next, TokenStream::Operand)) {
            return null();
        }
=======
    case TokenKind::LeftCurly:
      return objectLiteral(yieldHandling, possibleError);
>>>>>>> upstream-releases

<<<<<<< HEAD
    default: {
      if (!TokenKindIsPossibleIdentifier(tt)) {
        error(JSMSG_UNEXPECTED_TOKEN, "expression", TokenKindToDesc(tt));
        return null();
      }
||||||| merged common ancestors
        if (next == TokenKind::RightParen) {
            // Not valid expression syntax, but this is valid in an arrow function
            // with no params: `() => body`.
            tokenStream.consumeKnownToken(TokenKind::RightParen, TokenStream::Operand);

            if (!tokenStream.peekToken(&next)) {
                return null();
            }
            if (next != TokenKind::Arrow) {
                error(JSMSG_UNEXPECTED_TOKEN, "expression", TokenKindToDesc(TokenKind::RightParen));
                return null();
            }
=======
    case TokenKind::LeftParen: {
      TokenKind next;
      if (!tokenStream.peekToken(&next, TokenStream::SlashIsRegExp)) {
        return null();
      }

      if (next == TokenKind::RightParen) {
        // Not valid expression syntax, but this is valid in an arrow function
        // with no params: `() => body`.
        tokenStream.consumeKnownToken(TokenKind::RightParen,
                                      TokenStream::SlashIsRegExp);
>>>>>>> upstream-releases

<<<<<<< HEAD
      if (tt == TokenKind::Async) {
        TokenKind nextSameLine = TokenKind::Eof;
        if (!tokenStream.peekTokenSameLine(&nextSameLine)) {
          return null();
||||||| merged common ancestors
            // Now just return something that will allow parsing to continue.
            // It doesn't matter what; when we reach the =>, we will rewind and
            // reparse the whole arrow function. See Parser::assignExpr.
            return handler.newNullLiteral(pos());
=======
        if (!tokenStream.peekToken(&next)) {
          return null();
>>>>>>> upstream-releases
        }
<<<<<<< HEAD

        if (nextSameLine == TokenKind::Function) {
          uint32_t toStringStart = pos().begin;
          tokenStream.consumeKnownToken(TokenKind::Function);
          return functionExpr(toStringStart, PredictUninvoked,
                              FunctionAsyncKind::AsyncFunction);
||||||| merged common ancestors

        // Pass |possibleError| to support destructuring in arrow parameters.
        Node expr = exprInParens(InAllowed, yieldHandling, TripledotAllowed, possibleError);
        if (!expr) {
            return null();
=======
        if (next != TokenKind::Arrow) {
          error(JSMSG_UNEXPECTED_TOKEN, "expression",
                TokenKindToDesc(TokenKind::RightParen));
          return null();
>>>>>>> upstream-releases
        }
<<<<<<< HEAD
||||||| merged common ancestors
        MUST_MATCH_TOKEN_MOD(TokenKind::RightParen, TokenStream::Operand, JSMSG_PAREN_IN_PAREN);
        return handler.parenthesize(expr);
=======

        // Now just return something that will allow parsing to continue.
        // It doesn't matter what; when we reach the =>, we will rewind and
        // reparse the whole arrow function. See Parser::assignExpr.
        return handler_.newNullLiteral(pos());
>>>>>>> upstream-releases
      }

<<<<<<< HEAD
      Rooted<PropertyName*> name(context, identifierReference(yieldHandling));
      if (!name) {
        return null();
      }
||||||| merged common ancestors
      case TokenKind::TemplateHead:
        return templateLiteral(yieldHandling);

      case TokenKind::NoSubsTemplate:
        return noSubstitutionUntaggedTemplate();

      case TokenKind::String:
        return stringLiteral();
=======
      // Pass |possibleError| to support destructuring in arrow parameters.
      Node expr = exprInParens(InAllowed, yieldHandling, TripledotAllowed,
                               possibleError);
      if (!expr) {
        return null();
      }
      if (!mustMatchToken(TokenKind::RightParen, JSMSG_PAREN_IN_PAREN)) {
        return null();
      }
      return handler_.parenthesize(expr);
    }

    case TokenKind::TemplateHead:
      return templateLiteral(yieldHandling);

    case TokenKind::NoSubsTemplate:
      return noSubstitutionUntaggedTemplate();
>>>>>>> upstream-releases

<<<<<<< HEAD
      return identifierReference(name);
    }
||||||| merged common ancestors
      default: {
        if (!TokenKindIsPossibleIdentifier(tt)) {
            error(JSMSG_UNEXPECTED_TOKEN, "expression", TokenKindToDesc(tt));
            return null();
        }
=======
    case TokenKind::String:
      return stringLiteral();
>>>>>>> upstream-releases

<<<<<<< HEAD
    case TokenKind::RegExp:
      return newRegExp();
||||||| merged common ancestors
        if (tt == TokenKind::Async) {
            TokenKind nextSameLine = TokenKind::Eof;
            if (!tokenStream.peekTokenSameLine(&nextSameLine)) {
                return null();
            }
=======
    default: {
      if (!TokenKindIsPossibleIdentifier(tt)) {
        error(JSMSG_UNEXPECTED_TOKEN, "expression", TokenKindToDesc(tt));
        return null();
      }
>>>>>>> upstream-releases

<<<<<<< HEAD
    case TokenKind::Number:
      return newNumber(anyChars.currentToken());
||||||| merged common ancestors
            if (nextSameLine == TokenKind::Function) {
                uint32_t toStringStart = pos().begin;
                tokenStream.consumeKnownToken(TokenKind::Function);
                return functionExpr(toStringStart, PredictUninvoked, FunctionAsyncKind::AsyncFunction);
            }
        }
=======
      if (tt == TokenKind::Async) {
        TokenKind nextSameLine = TokenKind::Eof;
        if (!tokenStream.peekTokenSameLine(&nextSameLine)) {
          return null();
        }
>>>>>>> upstream-releases

<<<<<<< HEAD
#ifdef ENABLE_BIGINT
    case TokenKind::BigInt:
      return newBigInt();
#endif
||||||| merged common ancestors
        Rooted<PropertyName*> name(context, identifierReference(yieldHandling));
        if (!name) {
            return null();
        }
=======
        if (nextSameLine == TokenKind::Function) {
          uint32_t toStringStart = pos().begin;
          tokenStream.consumeKnownToken(TokenKind::Function);
          return functionExpr(toStringStart, PredictUninvoked,
                              FunctionAsyncKind::AsyncFunction);
        }
      }
>>>>>>> upstream-releases

<<<<<<< HEAD
    case TokenKind::True:
      return handler.newBooleanLiteral(true, pos());
    case TokenKind::False:
      return handler.newBooleanLiteral(false, pos());
    case TokenKind::This: {
      if (pc->isFunctionBox()) {
        pc->functionBox()->usesThis = true;
||||||| merged common ancestors
        return identifierReference(name);
=======
      Rooted<PropertyName*> name(cx_, identifierReference(yieldHandling));
      if (!name) {
        return null();
>>>>>>> upstream-releases
      }
<<<<<<< HEAD
      NameNodeType thisName = null();
      if (pc->sc()->thisBinding() == ThisBinding::Function) {
        thisName = newThisName();
        if (!thisName) {
          return null();
        }
      }
      return handler.newThisLiteral(pos(), thisName);
    }
    case TokenKind::Null:
      return handler.newNullLiteral(pos());

    case TokenKind::TripleDot: {
      // This isn't valid expression syntax, but it's valid in an arrow
      // function as a trailing rest param: `(a, b, ...rest) => body`.  Check
      // if it's directly under
      // CoverParenthesizedExpressionAndArrowParameterList, and check for a
      // name, closing parenthesis, and arrow, and allow it only if all are
      // present.
      if (tripledotHandling != TripledotAllowed) {
        error(JSMSG_UNEXPECTED_TOKEN, "expression", TokenKindToDesc(tt));
        return null();
      }
||||||| merged common ancestors

      case TokenKind::RegExp:
        return newRegExp();

      case TokenKind::Number:
        return newNumber(anyChars.currentToken());

      case TokenKind::True:
        return handler.newBooleanLiteral(true, pos());
      case TokenKind::False:
        return handler.newBooleanLiteral(false, pos());
      case TokenKind::This: {
        if (pc->isFunctionBox()) {
            pc->functionBox()->usesThis = true;
        }
        NameNodeType thisName = null();
        if (pc->sc()->thisBinding() == ThisBinding::Function) {
            thisName = newThisName();
            if (!thisName) {
                return null();
            }
        }
        return handler.newThisLiteral(pos(), thisName);
      }
      case TokenKind::Null:
        return handler.newNullLiteral(pos());

      case TokenKind::TripleDot: {
        // This isn't valid expression syntax, but it's valid in an arrow
        // function as a trailing rest param: `(a, b, ...rest) => body`.  Check
        // if it's directly under
        // CoverParenthesizedExpressionAndArrowParameterList, and check for a
        // name, closing parenthesis, and arrow, and allow it only if all are
        // present.
        if (tripledotHandling != TripledotAllowed) {
            error(JSMSG_UNEXPECTED_TOKEN, "expression", TokenKindToDesc(tt));
            return null();
        }
=======

      return identifierReference(name);
    }

    case TokenKind::RegExp:
      return newRegExp();

    case TokenKind::Number:
      return newNumber(anyChars.currentToken());
>>>>>>> upstream-releases

<<<<<<< HEAD
      TokenKind next;
      if (!tokenStream.getToken(&next)) {
        return null();
      }
||||||| merged common ancestors
        TokenKind next;
        if (!tokenStream.getToken(&next)) {
            return null();
        }
=======
    case TokenKind::BigInt:
      return newBigInt();
>>>>>>> upstream-releases

<<<<<<< HEAD
      if (next == TokenKind::LeftBracket || next == TokenKind::LeftCurly) {
        // Validate, but don't store the pattern right now. The whole arrow
        // function is reparsed in functionFormalParametersAndBody().
        if (!destructuringDeclaration(DeclarationKind::CoverArrowParameter,
                                      yieldHandling, next)) {
          return null();
        }
      } else {
        // This doesn't check that the provided name is allowed, e.g. if
        // the enclosing code is strict mode code, any of "let", "yield",
        // or "arguments" should be prohibited.  Argument-parsing code
        // handles that.
        if (!TokenKindIsPossibleIdentifier(next)) {
          error(JSMSG_UNEXPECTED_TOKEN, "rest argument name",
                TokenKindToDesc(next));
          return null();
||||||| merged common ancestors
        if (next == TokenKind::LeftBracket || next == TokenKind::LeftCurly) {
            // Validate, but don't store the pattern right now. The whole arrow
            // function is reparsed in functionFormalParametersAndBody().
            if (!destructuringDeclaration(DeclarationKind::CoverArrowParameter, yieldHandling,
                                          next))
            {
                return null();
            }
        } else {
            // This doesn't check that the provided name is allowed, e.g. if
            // the enclosing code is strict mode code, any of "let", "yield",
            // or "arguments" should be prohibited.  Argument-parsing code
            // handles that.
            if (!TokenKindIsPossibleIdentifier(next)) {
                error(JSMSG_UNEXPECTED_TOKEN, "rest argument name", TokenKindToDesc(next));
                return null();
            }
=======
    case TokenKind::True:
      return handler_.newBooleanLiteral(true, pos());
    case TokenKind::False:
      return handler_.newBooleanLiteral(false, pos());
    case TokenKind::This: {
      if (pc_->isFunctionBox()) {
        pc_->functionBox()->usesThis = true;
      }
      NameNodeType thisName = null();
      if (pc_->sc()->thisBinding() == ThisBinding::Function) {
        thisName = newThisName();
        if (!thisName) {
          return null();
>>>>>>> upstream-releases
        }
<<<<<<< HEAD
      }
||||||| merged common ancestors
=======
      }
      return handler_.newThisLiteral(pos(), thisName);
    }
    case TokenKind::Null:
      return handler_.newNullLiteral(pos());

    case TokenKind::TripleDot: {
      // This isn't valid expression syntax, but it's valid in an arrow
      // function as a trailing rest param: `(a, b, ...rest) => body`.  Check
      // if it's directly under
      // CoverParenthesizedExpressionAndArrowParameterList, and check for a
      // name, closing parenthesis, and arrow, and allow it only if all are
      // present.
      if (tripledotHandling != TripledotAllowed) {
        error(JSMSG_UNEXPECTED_TOKEN, "expression", TokenKindToDesc(tt));
        return null();
      }
>>>>>>> upstream-releases

<<<<<<< HEAD
      if (!tokenStream.getToken(&next)) {
        return null();
      }
      if (next != TokenKind::RightParen) {
        error(JSMSG_UNEXPECTED_TOKEN, "closing parenthesis",
              TokenKindToDesc(next));
        return null();
      }
||||||| merged common ancestors
        if (!tokenStream.getToken(&next)) {
            return null();
        }
        if (next != TokenKind::RightParen) {
            error(JSMSG_UNEXPECTED_TOKEN, "closing parenthesis", TokenKindToDesc(next));
            return null();
        }
=======
      TokenKind next;
      if (!tokenStream.getToken(&next)) {
        return null();
      }
>>>>>>> upstream-releases

<<<<<<< HEAD
      if (!tokenStream.peekToken(&next)) {
        return null();
      }
      if (next != TokenKind::Arrow) {
        // Advance the scanner for proper error location reporting.
        tokenStream.consumeKnownToken(next);
        error(JSMSG_UNEXPECTED_TOKEN, "'=>' after argument list",
              TokenKindToDesc(next));
        return null();
      }
||||||| merged common ancestors
        if (!tokenStream.peekToken(&next)) {
            return null();
        }
        if (next != TokenKind::Arrow) {
            // Advance the scanner for proper error location reporting.
            tokenStream.consumeKnownToken(next);
            error(JSMSG_UNEXPECTED_TOKEN, "'=>' after argument list", TokenKindToDesc(next));
            return null();
        }
=======
      if (next == TokenKind::LeftBracket || next == TokenKind::LeftCurly) {
        // Validate, but don't store the pattern right now. The whole arrow
        // function is reparsed in functionFormalParametersAndBody().
        if (!destructuringDeclaration(DeclarationKind::CoverArrowParameter,
                                      yieldHandling, next)) {
          return null();
        }
      } else {
        // This doesn't check that the provided name is allowed, e.g. if
        // the enclosing code is strict mode code, any of "let", "yield",
        // or "arguments" should be prohibited.  Argument-parsing code
        // handles that.
        if (!TokenKindIsPossibleIdentifier(next)) {
          error(JSMSG_UNEXPECTED_TOKEN, "rest argument name",
                TokenKindToDesc(next));
          return null();
        }
      }
>>>>>>> upstream-releases

<<<<<<< HEAD
      anyChars.ungetToken();  // put back right paren
||||||| merged common ancestors
        anyChars.ungetToken();  // put back right paren
=======
      if (!tokenStream.getToken(&next)) {
        return null();
      }
      if (next != TokenKind::RightParen) {
        error(JSMSG_UNEXPECTED_TOKEN, "closing parenthesis",
              TokenKindToDesc(next));
        return null();
      }
>>>>>>> upstream-releases

<<<<<<< HEAD
      // Return an arbitrary expression node. See case TokenKind::RightParen
      // above.
      return handler.newNullLiteral(pos());
||||||| merged common ancestors
        // Return an arbitrary expression node. See case TokenKind::RightParen above.
        return handler.newNullLiteral(pos());
      }
=======
      if (!tokenStream.peekToken(&next)) {
        return null();
      }
      if (next != TokenKind::Arrow) {
        // Advance the scanner for proper error location reporting.
        tokenStream.consumeKnownToken(next);
        error(JSMSG_UNEXPECTED_TOKEN, "'=>' after argument list",
              TokenKindToDesc(next));
        return null();
      }

      anyChars.ungetToken();  // put back right paren

      // Return an arbitrary expression node. See case TokenKind::RightParen
      // above.
      return handler_.newNullLiteral(pos());
>>>>>>> upstream-releases
    }
  }
}

template <class ParseHandler, typename Unit>
typename ParseHandler::Node GeneralParser<ParseHandler, Unit>::exprInParens(
    InHandling inHandling, YieldHandling yieldHandling,
    TripledotHandling tripledotHandling,
    PossibleError* possibleError /* = nullptr */) {
  MOZ_ASSERT(anyChars.isCurrentTokenType(TokenKind::LeftParen));
  return expr(inHandling, yieldHandling, tripledotHandling, possibleError,
              PredictInvoked);
}

template class PerHandlerParser<FullParseHandler>;
template class PerHandlerParser<SyntaxParseHandler>;
template class GeneralParser<FullParseHandler, Utf8Unit>;
template class GeneralParser<SyntaxParseHandler, Utf8Unit>;
template class GeneralParser<FullParseHandler, char16_t>;
template class GeneralParser<SyntaxParseHandler, char16_t>;
template class Parser<FullParseHandler, Utf8Unit>;
template class Parser<SyntaxParseHandler, Utf8Unit>;
template class Parser<FullParseHandler, char16_t>;
template class Parser<SyntaxParseHandler, char16_t>;

} /* namespace frontend */
} /* namespace js */
