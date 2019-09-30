/* -*- Mode: C++; tab-width: 8; indent-tabs-mode: nil; c-basic-offset: 2 -*-
 * vim: set ts=8 sts=2 et sw=2 tw=80:
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/. */

#ifndef frontend_ParseNode_h
#define frontend_ParseNode_h

#include "mozilla/Attributes.h"

<<<<<<< HEAD
#include "frontend/TokenStream.h"
#ifdef ENABLE_BIGINT
#include "vm/BigIntType.h"
#endif
||||||| merged common ancestors
#include "frontend/TokenStream.h"
=======
#include "frontend/Token.h"
#include "vm/BigIntType.h"
>>>>>>> upstream-releases
#include "vm/BytecodeUtil.h"
#include "vm/Printer.h"
#include "vm/Scope.h"

/* clang-format off */
//
// A few notes on lifetime of ParseNode trees:
//
// - All the `ParseNode` instances MUST BE explicitly allocated in the context's `LifoAlloc`.
//   This is typically implemented by the `FullParseHandler` or it can be reimplemented with
//   a custom `new_`.
//
// - The tree is bulk-deallocated when the parser is deallocated. Consequently, references
//   to a subtree MUST NOT exist once the parser has been deallocated.
//
// - This bulk-deallocation DOES NOT run destructors.
//
// - Instances of `LexicalScope::Data` MUST BE allocated as instances of `ParseNode`, in the same
//   `LifoAlloc`. They are bulk-deallocated alongside the rest of the tree.
//
// - Instances of `JSAtom` used throughout the tree (including instances of `PropertyName`) MUST
//   be kept alive by the parser. This is done through an instance of `AutoKeepAtoms` held by
//   the parser.
//
// - Once the parser is deallocated, the `JSAtom` instances MAY be garbage-collected.
//
/* clang-format on */

namespace js {
namespace frontend {

class ParseContext;
class FullParseHandler;
class FunctionBox;
class ObjectBox;
<<<<<<< HEAD
#ifdef ENABLE_BIGINT
class BigIntBox;
#endif

#define FOR_EACH_PARSE_NODE_KIND(F)                                          \
  F(EmptyStatement, PN_NULLARY)                                              \
  F(ExpressionStatement, PN_UNARY)                                           \
  F(Comma, PN_LIST)                                                          \
  F(Conditional, PN_TERNARY)                                                 \
  F(Colon, PN_BINARY)                                                        \
  F(Shorthand, PN_BINARY)                                                    \
  F(Pos, PN_UNARY)                                                           \
  F(Neg, PN_UNARY)                                                           \
  F(PreIncrement, PN_UNARY)                                                  \
  F(PostIncrement, PN_UNARY)                                                 \
  F(PreDecrement, PN_UNARY)                                                  \
  F(PostDecrement, PN_UNARY)                                                 \
  F(PropertyName, PN_NAME)                                                   \
  F(Dot, PN_BINARY)                                                          \
  F(Elem, PN_BINARY)                                                         \
  F(Array, PN_LIST)                                                          \
  F(Elision, PN_NULLARY)                                                     \
  F(StatementList, PN_LIST)                                                  \
  F(Label, PN_NAME)                                                          \
  F(Object, PN_LIST)                                                         \
  F(Call, PN_BINARY)                                                         \
  F(Arguments, PN_LIST)                                                      \
  F(Name, PN_NAME)                                                           \
  F(ObjectPropertyName, PN_NAME)                                             \
  F(PrivateName, PN_NAME)                                                    \
  F(ComputedName, PN_UNARY)                                                  \
  F(Number, PN_NUMBER)                                                       \
  IF_BIGINT(F(BigInt, PN_BIGINT), /**/)                                      \
  F(String, PN_NAME)                                                         \
  F(TemplateStringList, PN_LIST)                                             \
  F(TemplateString, PN_NAME)                                                 \
  F(TaggedTemplate, PN_BINARY)                                               \
  F(CallSiteObj, PN_LIST)                                                    \
  F(RegExp, PN_REGEXP)                                                       \
  F(True, PN_NULLARY)                                                        \
  F(False, PN_NULLARY)                                                       \
  F(Null, PN_NULLARY)                                                        \
  F(RawUndefined, PN_NULLARY)                                                \
  F(This, PN_UNARY)                                                          \
  F(Function, PN_CODE)                                                       \
  F(Module, PN_CODE)                                                         \
  F(If, PN_TERNARY)                                                          \
  F(Switch, PN_BINARY)                                                       \
  F(Case, PN_BINARY)                                                         \
  F(While, PN_BINARY)                                                        \
  F(DoWhile, PN_BINARY)                                                      \
  F(For, PN_BINARY)                                                          \
  F(Break, PN_LOOP)                                                          \
  F(Continue, PN_LOOP)                                                       \
  F(Var, PN_LIST)                                                            \
  F(Const, PN_LIST)                                                          \
  F(With, PN_BINARY)                                                         \
  F(Return, PN_UNARY)                                                        \
  F(New, PN_BINARY)                                                          \
  /* Delete operations.  These must be sequential. */                        \
  F(DeleteName, PN_UNARY)                                                    \
  F(DeleteProp, PN_UNARY)                                                    \
  F(DeleteElem, PN_UNARY)                                                    \
  F(DeleteExpr, PN_UNARY)                                                    \
  F(Try, PN_TERNARY)                                                         \
  F(Catch, PN_BINARY)                                                        \
  F(Throw, PN_UNARY)                                                         \
  F(Debugger, PN_NULLARY)                                                    \
  F(Generator, PN_NULLARY)                                                   \
  F(InitialYield, PN_UNARY)                                                  \
  F(Yield, PN_UNARY)                                                         \
  F(YieldStar, PN_UNARY)                                                     \
  F(LexicalScope, PN_SCOPE)                                                  \
  F(Let, PN_LIST)                                                            \
  F(Import, PN_BINARY)                                                       \
  F(ImportSpecList, PN_LIST)                                                 \
  F(ImportSpec, PN_BINARY)                                                   \
  F(Export, PN_UNARY)                                                        \
  F(ExportFrom, PN_BINARY)                                                   \
  F(ExportDefault, PN_BINARY)                                                \
  F(ExportSpecList, PN_LIST)                                                 \
  F(ExportSpec, PN_BINARY)                                                   \
  F(ExportBatchSpec, PN_NULLARY)                                             \
  F(ForIn, PN_TERNARY)                                                       \
  F(ForOf, PN_TERNARY)                                                       \
  F(ForHead, PN_TERNARY)                                                     \
  F(ParamsBody, PN_LIST)                                                     \
  F(Spread, PN_UNARY)                                                        \
  F(MutateProto, PN_UNARY)                                                   \
  F(Class, PN_TERNARY)                                                       \
  F(ClassMethod, PN_BINARY)                                                  \
  F(ClassField, PN_FIELD)                                                    \
  F(ClassMemberList, PN_LIST)                                                \
  F(ClassNames, PN_BINARY)                                                   \
  F(NewTarget, PN_BINARY)                                                    \
  F(PosHolder, PN_NULLARY)                                                   \
  F(SuperBase, PN_UNARY)                                                     \
  F(SuperCall, PN_BINARY)                                                    \
  F(SetThis, PN_BINARY)                                                      \
  F(ImportMeta, PN_BINARY)                                                   \
  F(CallImport, PN_BINARY)                                                   \
                                                                             \
  /* Unary operators. */                                                     \
  F(TypeOfName, PN_UNARY)                                                    \
  F(TypeOfExpr, PN_UNARY)                                                    \
  F(Void, PN_UNARY)                                                          \
  F(Not, PN_UNARY)                                                           \
  F(BitNot, PN_UNARY)                                                        \
  F(Await, PN_UNARY)                                                         \
                                                                             \
  /*                                                                         \
   * Binary operators.                                                       \
   * These must be in the same order as TOK_OR and friends in TokenStream.h. \
   */                                                                        \
  F(Pipeline, PN_LIST)                                                       \
  F(Or, PN_LIST)                                                             \
  F(And, PN_LIST)                                                            \
  F(BitOr, PN_LIST)                                                          \
  F(BitXor, PN_LIST)                                                         \
  F(BitAnd, PN_LIST)                                                         \
  F(StrictEq, PN_LIST)                                                       \
  F(Eq, PN_LIST)                                                             \
  F(StrictNe, PN_LIST)                                                       \
  F(Ne, PN_LIST)                                                             \
  F(Lt, PN_LIST)                                                             \
  F(Le, PN_LIST)                                                             \
  F(Gt, PN_LIST)                                                             \
  F(Ge, PN_LIST)                                                             \
  F(InstanceOf, PN_LIST)                                                     \
  F(In, PN_LIST)                                                             \
  F(Lsh, PN_LIST)                                                            \
  F(Rsh, PN_LIST)                                                            \
  F(Ursh, PN_LIST)                                                           \
  F(Add, PN_LIST)                                                            \
  F(Sub, PN_LIST)                                                            \
  F(Star, PN_LIST)                                                           \
  F(Div, PN_LIST)                                                            \
  F(Mod, PN_LIST)                                                            \
  F(Pow, PN_LIST)                                                            \
                                                                             \
  /* Assignment operators (= += -= etc.). */                                 \
  /* ParseNode::isAssignment assumes all these are consecutive. */           \
  F(Assign, PN_BINARY)                                                       \
  F(AddAssign, PN_BINARY)                                                    \
  F(SubAssign, PN_BINARY)                                                    \
  F(BitOrAssign, PN_BINARY)                                                  \
  F(BitXorAssign, PN_BINARY)                                                 \
  F(BitAndAssign, PN_BINARY)                                                 \
  F(LshAssign, PN_BINARY)                                                    \
  F(RshAssign, PN_BINARY)                                                    \
  F(UrshAssign, PN_BINARY)                                                   \
  F(MulAssign, PN_BINARY)                                                    \
  F(DivAssign, PN_BINARY)                                                    \
  F(ModAssign, PN_BINARY)                                                    \
  F(PowAssign, PN_BINARY)
||||||| merged common ancestors

#define FOR_EACH_PARSE_NODE_KIND(F) \
    F(EmptyStatement) \
    F(ExpressionStatement) \
    F(Comma) \
    F(Conditional) \
    F(Colon) \
    F(Shorthand) \
    F(Pos) \
    F(Neg) \
    F(PreIncrement) \
    F(PostIncrement) \
    F(PreDecrement) \
    F(PostDecrement) \
    F(PropertyName) \
    F(Dot) \
    F(Elem) \
    F(Array) \
    F(Elision) \
    F(StatementList) \
    F(Label) \
    F(Object) \
    F(Call) \
    F(Arguments) \
    F(Name) \
    F(ObjectPropertyName) \
    F(ComputedName) \
    F(Number) \
    F(String) \
    F(TemplateStringList) \
    F(TemplateString) \
    F(TaggedTemplate) \
    F(CallSiteObj) \
    F(RegExp) \
    F(True) \
    F(False) \
    F(Null) \
    F(RawUndefined) \
    F(This) \
    F(Function) \
    F(Module) \
    F(If) \
    F(Switch) \
    F(Case) \
    F(While) \
    F(DoWhile) \
    F(For) \
    F(Break) \
    F(Continue) \
    F(Var) \
    F(Const) \
    F(With) \
    F(Return) \
    F(New) \
    /* Delete operations.  These must be sequential. */ \
    F(DeleteName) \
    F(DeleteProp) \
    F(DeleteElem) \
    F(DeleteExpr) \
    F(Try) \
    F(Catch) \
    F(Throw) \
    F(Debugger) \
    F(Generator) \
    F(InitialYield) \
    F(Yield) \
    F(YieldStar) \
    F(LexicalScope) \
    F(Let) \
    F(Import) \
    F(ImportSpecList) \
    F(ImportSpec) \
    F(Export) \
    F(ExportFrom) \
    F(ExportDefault) \
    F(ExportSpecList) \
    F(ExportSpec) \
    F(ExportBatchSpec) \
    F(ForIn) \
    F(ForOf) \
    F(ForHead) \
    F(ParamsBody) \
    F(Spread) \
    F(MutateProto) \
    F(Class) \
    F(ClassMethod) \
    F(ClassMethodList) \
    F(ClassNames) \
    F(NewTarget) \
    F(PosHolder) \
    F(SuperBase) \
    F(SuperCall) \
    F(SetThis) \
    F(ImportMeta) \
    \
    /* Unary operators. */ \
    F(TypeOfName) \
    F(TypeOfExpr) \
    F(Void) \
    F(Not) \
    F(BitNot) \
    F(Await) \
    \
    /* \
     * Binary operators. \
     * These must be in the same order as TOK_OR and friends in TokenStream.h. \
     */ \
    F(Pipeline) \
    F(Or) \
    F(And) \
    F(BitOr) \
    F(BitXor) \
    F(BitAnd) \
    F(StrictEq) \
    F(Eq) \
    F(StrictNe) \
    F(Ne) \
    F(Lt) \
    F(Le) \
    F(Gt) \
    F(Ge) \
    F(InstanceOf) \
    F(In) \
    F(Lsh) \
    F(Rsh) \
    F(Ursh) \
    F(Add) \
    F(Sub) \
    F(Star) \
    F(Div) \
    F(Mod) \
    F(Pow) \
    \
    /* Assignment operators (= += -= etc.). */ \
    /* ParseNode::isAssignment assumes all these are consecutive. */ \
    F(Assign) \
    F(AddAssign) \
    F(SubAssign) \
    F(BitOrAssign) \
    F(BitXorAssign) \
    F(BitAndAssign) \
    F(LshAssign) \
    F(RshAssign) \
    F(UrshAssign) \
    F(MulAssign) \
    F(DivAssign) \
    F(ModAssign) \
    F(PowAssign)
=======
class BigIntBox;

#define FOR_EACH_PARSE_NODE_KIND(F)                                          \
  F(EmptyStmt, NullaryNode)                                                  \
  F(ExpressionStmt, UnaryNode)                                               \
  F(CommaExpr, ListNode)                                                     \
  F(ConditionalExpr, ConditionalExpression)                                  \
  F(PropertyDefinition, PropertyDefinition)                                  \
  F(Shorthand, BinaryNode)                                                   \
  F(PosExpr, UnaryNode)                                                      \
  F(NegExpr, UnaryNode)                                                      \
  F(PreIncrementExpr, UnaryNode)                                             \
  F(PostIncrementExpr, UnaryNode)                                            \
  F(PreDecrementExpr, UnaryNode)                                             \
  F(PostDecrementExpr, UnaryNode)                                            \
  F(PropertyNameExpr, NameNode)                                              \
  F(DotExpr, PropertyAccess)                                                 \
  F(ElemExpr, PropertyByValue)                                               \
  F(ArrayExpr, ListNode)                                                     \
  F(Elision, NullaryNode)                                                    \
  F(StatementList, ListNode)                                                 \
  F(LabelStmt, LabeledStatement)                                             \
  F(ObjectExpr, ListNode)                                                    \
  F(CallExpr, BinaryNode)                                                    \
  F(Arguments, ListNode)                                                     \
  F(Name, NameNode)                                                          \
  F(ObjectPropertyName, NameNode)                                            \
  F(PrivateName, NameNode)                                                   \
  F(ComputedName, UnaryNode)                                                 \
  F(NumberExpr, NumericLiteral)                                              \
  F(BigIntExpr, BigIntLiteral)                                               \
  F(StringExpr, NameNode)                                                    \
  F(TemplateStringListExpr, ListNode)                                        \
  F(TemplateStringExpr, NameNode)                                            \
  F(TaggedTemplateExpr, BinaryNode)                                          \
  F(CallSiteObj, CallSiteNode)                                               \
  F(RegExpExpr, RegExpLiteral)                                               \
  F(TrueExpr, BooleanLiteral)                                                \
  F(FalseExpr, BooleanLiteral)                                               \
  F(NullExpr, NullLiteral)                                                   \
  F(RawUndefinedExpr, RawUndefinedLiteral)                                   \
  F(ThisExpr, UnaryNode)                                                     \
  F(Function, FunctionNode)                                                  \
  F(Module, ModuleNode)                                                      \
  F(IfStmt, TernaryNode)                                                     \
  F(SwitchStmt, SwitchStatement)                                             \
  F(Case, CaseClause)                                                        \
  F(WhileStmt, BinaryNode)                                                   \
  F(DoWhileStmt, BinaryNode)                                                 \
  F(ForStmt, ForNode)                                                        \
  F(BreakStmt, BreakStatement)                                               \
  F(ContinueStmt, ContinueStatement)                                         \
  F(VarStmt, ListNode)                                                       \
  F(ConstDecl, ListNode)                                                     \
  F(WithStmt, BinaryNode)                                                    \
  F(ReturnStmt, UnaryNode)                                                   \
  F(NewExpr, BinaryNode)                                                     \
  /* Delete operations.  These must be sequential. */                        \
  F(DeleteNameExpr, UnaryNode)                                               \
  F(DeletePropExpr, UnaryNode)                                               \
  F(DeleteElemExpr, UnaryNode)                                               \
  F(DeleteExpr, UnaryNode)                                                   \
  F(TryStmt, TernaryNode)                                                    \
  F(Catch, BinaryNode)                                                       \
  F(ThrowStmt, UnaryNode)                                                    \
  F(DebuggerStmt, DebuggerStatement)                                         \
  F(Generator, NullaryNode)                                                  \
  F(InitialYield, UnaryNode)                                                 \
  F(YieldExpr, UnaryNode)                                                    \
  F(YieldStarExpr, UnaryNode)                                                \
  F(LexicalScope, LexicalScopeNode)                                          \
  F(LetDecl, ListNode)                                                       \
  F(ImportDecl, BinaryNode)                                                  \
  F(ImportSpecList, ListNode)                                                \
  F(ImportSpec, BinaryNode)                                                  \
  F(ExportStmt, UnaryNode)                                                   \
  F(ExportFromStmt, BinaryNode)                                              \
  F(ExportDefaultStmt, BinaryNode)                                           \
  F(ExportSpecList, ListNode)                                                \
  F(ExportSpec, BinaryNode)                                                  \
  F(ExportBatchSpecStmt, NullaryNode)                                        \
  F(ForIn, TernaryNode)                                                      \
  F(ForOf, TernaryNode)                                                      \
  F(ForHead, TernaryNode)                                                    \
  F(ParamsBody, ListNode)                                                    \
  F(Spread, UnaryNode)                                                       \
  F(MutateProto, UnaryNode)                                                  \
  F(ClassDecl, ClassNode)                                                    \
  F(ClassMethod, ClassMethod)                                                \
  F(ClassField, ClassField)                                                  \
  F(ClassMemberList, ListNode)                                               \
  F(ClassNames, ClassNames)                                                  \
  F(NewTargetExpr, BinaryNode)                                               \
  F(PosHolder, NullaryNode)                                                  \
  F(SuperBase, UnaryNode)                                                    \
  F(SuperCallExpr, BinaryNode)                                               \
  F(SetThis, BinaryNode)                                                     \
  F(ImportMetaExpr, BinaryNode)                                              \
  F(CallImportExpr, BinaryNode)                                              \
  F(InitExpr, BinaryNode)                                                    \
                                                                             \
  /* Unary operators. */                                                     \
  F(TypeOfNameExpr, UnaryNode)                                               \
  F(TypeOfExpr, UnaryNode)                                                   \
  F(VoidExpr, UnaryNode)                                                     \
  F(NotExpr, UnaryNode)                                                      \
  F(BitNotExpr, UnaryNode)                                                   \
  F(AwaitExpr, UnaryNode)                                                    \
                                                                             \
  /*                                                                         \
   * Binary operators.                                                       \
   * These must be in the same order as TOK_OR and friends in TokenStream.h. \
   */                                                                        \
  F(PipelineExpr, ListNode)                                                  \
  F(OrExpr, ListNode)                                                        \
  F(AndExpr, ListNode)                                                       \
  F(BitOrExpr, ListNode)                                                     \
  F(BitXorExpr, ListNode)                                                    \
  F(BitAndExpr, ListNode)                                                    \
  F(StrictEqExpr, ListNode)                                                  \
  F(EqExpr, ListNode)                                                        \
  F(StrictNeExpr, ListNode)                                                  \
  F(NeExpr, ListNode)                                                        \
  F(LtExpr, ListNode)                                                        \
  F(LeExpr, ListNode)                                                        \
  F(GtExpr, ListNode)                                                        \
  F(GeExpr, ListNode)                                                        \
  F(InstanceOfExpr, ListNode)                                                \
  F(InExpr, ListNode)                                                        \
  F(LshExpr, ListNode)                                                       \
  F(RshExpr, ListNode)                                                       \
  F(UrshExpr, ListNode)                                                      \
  F(AddExpr, ListNode)                                                       \
  F(SubExpr, ListNode)                                                       \
  F(MulExpr, ListNode)                                                       \
  F(DivExpr, ListNode)                                                       \
  F(ModExpr, ListNode)                                                       \
  F(PowExpr, ListNode)                                                       \
                                                                             \
  /* Assignment operators (= += -= etc.). */                                 \
  /* AssignmentNode::test assumes all these are consecutive. */              \
  F(AssignExpr, AssignmentNode)                                              \
  F(AddAssignExpr, AssignmentNode)                                           \
  F(SubAssignExpr, AssignmentNode)                                           \
  F(BitOrAssignExpr, AssignmentNode)                                         \
  F(BitXorAssignExpr, AssignmentNode)                                        \
  F(BitAndAssignExpr, AssignmentNode)                                        \
  F(LshAssignExpr, AssignmentNode)                                           \
  F(RshAssignExpr, AssignmentNode)                                           \
  F(UrshAssignExpr, AssignmentNode)                                          \
  F(MulAssignExpr, AssignmentNode)                                           \
  F(DivAssignExpr, AssignmentNode)                                           \
  F(ModAssignExpr, AssignmentNode)                                           \
  F(PowAssignExpr, AssignmentNode)
>>>>>>> upstream-releases

/*
 * Parsing builds a tree of nodes that directs code generation.  This tree is
 * not a concrete syntax tree in all respects (for example, || and && are left
 * associative, but (A && B && C) translates into the right-associated tree
 * <A && <B && C>> so that code generation can emit a left-associative branch
 * around <B && C> when A is false).  Nodes are labeled by kind.
 *
 * The long comment after this enum block describes the kinds in detail.
 */
<<<<<<< HEAD
enum class ParseNodeKind : uint16_t {
#define EMIT_ENUM(name, _arity) name,
  FOR_EACH_PARSE_NODE_KIND(EMIT_ENUM)
||||||| merged common ancestors
enum class ParseNodeKind : uint16_t
{
#define EMIT_ENUM(name) name,
    FOR_EACH_PARSE_NODE_KIND(EMIT_ENUM)
=======
enum class ParseNodeKind : uint16_t {
  // These constants start at 1001, the better to catch
  LastUnused = 1000,
#define EMIT_ENUM(name, _type) name,
  FOR_EACH_PARSE_NODE_KIND(EMIT_ENUM)
>>>>>>> upstream-releases
#undef EMIT_ENUM
<<<<<<< HEAD
      Limit, /* domain size */
  BinOpFirst = ParseNodeKind::Pipeline,
  BinOpLast = ParseNodeKind::Pow,
  AssignmentStart = ParseNodeKind::Assign,
  AssignmentLast = ParseNodeKind::PowAssign
||||||| merged common ancestors
    Limit, /* domain size */
    BinOpFirst = ParseNodeKind::Pipeline,
    BinOpLast = ParseNodeKind::Pow,
    AssignmentStart = ParseNodeKind::Assign,
    AssignmentLast = ParseNodeKind::PowAssign
=======
      Limit,
  Start = LastUnused + 1,
  BinOpFirst = ParseNodeKind::PipelineExpr,
  BinOpLast = ParseNodeKind::PowExpr,
  AssignmentStart = ParseNodeKind::AssignExpr,
  AssignmentLast = ParseNodeKind::PowAssignExpr,
>>>>>>> upstream-releases
};

<<<<<<< HEAD
inline bool IsDeleteKind(ParseNodeKind kind) {
  return ParseNodeKind::DeleteName <= kind && kind <= ParseNodeKind::DeleteExpr;
||||||| merged common ancestors
inline bool
IsDeleteKind(ParseNodeKind kind)
{
    return ParseNodeKind::DeleteName <= kind && kind <= ParseNodeKind::DeleteExpr;
=======
inline bool IsDeleteKind(ParseNodeKind kind) {
  return ParseNodeKind::DeleteNameExpr <= kind &&
         kind <= ParseNodeKind::DeleteExpr;
>>>>>>> upstream-releases
}

<<<<<<< HEAD
inline bool IsTypeofKind(ParseNodeKind kind) {
  return ParseNodeKind::TypeOfName <= kind && kind <= ParseNodeKind::TypeOfExpr;
||||||| merged common ancestors
inline bool
IsTypeofKind(ParseNodeKind kind)
{
    return ParseNodeKind::TypeOfName <= kind && kind <= ParseNodeKind::TypeOfExpr;
=======
inline bool IsTypeofKind(ParseNodeKind kind) {
  return ParseNodeKind::TypeOfNameExpr <= kind &&
         kind <= ParseNodeKind::TypeOfExpr;
>>>>>>> upstream-releases
}

/*
 * <Definitions>
 * Function (FunctionNode)
 *   funbox: ptr to js::FunctionBox holding function object containing arg and
 *           var properties.  We create the function object at parse (not emit)
 *           time to specialize arg and var bytecodes early.
 *   body: ParamsBody or null for lazily-parsed function, ordinarily;
 *         ParseNodeKind::LexicalScope for implicit function in genexpr
 *   syntaxKind: the syntax of the function
 * ParamsBody (ListNode)
 *   head: list of formal parameters with
 *           * Name node with non-empty name for SingleNameBinding without
 *             Initializer
 *           * AssignExpr node for SingleNameBinding with Initializer
 *           * Name node with empty name for destructuring
 *               expr: Array or Object for BindingPattern without
 *                     Initializer, Assign for BindingPattern with
 *                     Initializer
 *         followed by either:
 *           * StatementList node for function body statements
 *           * ReturnStmt for expression closure
 *   count: number of formal parameters + 1
 * Spread (UnaryNode)
 *   kid: expression being spread
 * ClassDecl (ClassNode)
 *   kid1: ClassNames for class name. can be null for anonymous class.
 *   kid2: expression after `extends`. null if no expression
 *   kid3: either of
 *           * ClassMemberList, if anonymous class
 *           * LexicalScopeNode which contains ClassMemberList as scopeBody,
 *             if named class
 * ClassNames (ClassNames)
 *   left: Name node for outer binding, or null if the class is an expression
 *         that doesn't create an outer binding
 *   right: Name node for inner binding
 * ClassMemberList (ListNode)
 *   head: list of N ClassMethod or ClassField nodes
 *   count: N >= 0
 * ClassMethod (ClassMethod)
 *   name: propertyName
 *   method: methodDefinition
 * Module (ModuleNode)
 *   body: statement list of the module
 *
 * <Statements>
 * StatementList (ListNode)
 *   head: list of N statements
 *   count: N >= 0
 * IfStmt (TernaryNode)
 *   kid1: cond
 *   kid2: then
 *   kid3: else or null
 * SwitchStmt (SwitchStatement)
 *   left: discriminant
 *   right: LexicalScope node that contains the list of Case nodes, with at
 *          most one default node.
 *   hasDefault: true if there's a default case
 * Case (CaseClause)
 *   left: case-expression if CaseClause, or null if DefaultClause
 *   right: StatementList node for this case's statements
 * WhileStmt (BinaryNode)
 *   left: cond
 *   right: body
 * DoWhileStmt (BinaryNode)
 *   left: body
 *   right: cond
 * ForStmt (ForNode)
 *   left: one of
 *           * ForIn: for (x in y) ...
 *           * ForOf: for (x of x) ...
 *           * ForHead: for (;;) ...
 *   right: body
 * ForIn (TernaryNode)
 *   kid1: declaration or expression to left of 'in'
 *   kid2: null
 *   kid3: object expr to right of 'in'
 * ForOf (TernaryNode)
 *   kid1: declaration or expression to left of 'of'
 *   kid2: null
 *   kid3: expr to right of 'of'
 * ForHead (TernaryNode)
 *   kid1:  init expr before first ';' or nullptr
 *   kid2:  cond expr before second ';' or nullptr
 *   kid3:  update expr after second ';' or nullptr
 * ThrowStmt (UnaryNode)
 *   kid: thrown exception
 * TryStmt (TernaryNode)
 *   kid1: try block
 *   kid2: null or LexicalScope for catch-block with scopeBody pointing to a
 *         Catch node
 *   kid3: null or finally block
 * Catch (BinaryNode)
 *   left: Name, Array, or Object catch var node
 *         (Array or Object if destructuring),
 *         or null if optional catch binding
 *   right: catch block statements
 * BreakStmt (BreakStatement)
 *   label: label or null
 * ContinueStmt (ContinueStatement)
 *   label: label or null
 * WithStmt (BinaryNode)
 *   left: head expr
 *   right: body
 * VarStmt, LetDecl, ConstDecl (ListNode)
 *   head: list of N Name or AssignExpr nodes
 *         each name node has either
 *           atom: variable name
 *           expr: initializer or null
 *         or
 *           atom: variable name
 *         each assignment node has
 *           left: pattern
 *           right: initializer
 *   count: N > 0
 * ReturnStmt (UnaryNode)
 *   kid: returned expression, or null if none
 * ExpressionStmt (UnaryNode)
 *   kid: expr
 *   prologue: true if Directive Prologue member in original source, not
 *             introduced via constant folding or other tree rewriting
 * EmptyStmt (NullaryNode)
 *   (no fields)
 * LabelStmt (LabeledStatement)
 *   atom: label
 *   expr: labeled statement
 * ImportDecl (BinaryNode)
 *   left: ImportSpecList import specifiers
 *   right: String module specifier
 * ImportSpecList (ListNode)
 *   head: list of N ImportSpec nodes
 *   count: N >= 0 (N = 0 for `import {} from ...`)
 * ImportSpec (BinaryNode)
 *   left: import name
 *   right: local binding name
 * ExportStmt (UnaryNode)
 *   kid: declaration expression
 * ExportFromStmt (BinaryNode)
 *   left: ExportSpecList export specifiers
 *   right: String module specifier
 * ExportSpecList (ListNode)
 *   head: list of N ExportSpec nodes
 *   count: N >= 0 (N = 0 for `export {}`)
 * ExportSpec (BinaryNode)
 *   left: local binding name
 *   right: export name
 * ExportDefaultStmt (BinaryNode)
 *   left: export default declaration or expression
 *   right: Name node for assignment
 *
 * <Expressions>
 * The `Expr` suffix is used for nodes that can appear anywhere an expression
 * could appear.  It is not used on a few weird kinds like Arguments and
 * CallSiteObj that are always the child node of an expression node, but which
 * can't stand alone.
 *
 * All left-associated binary trees of the same type are optimized into lists
 * to avoid recursion when processing expression chains.
 *
 * CommaExpr (ListNode)
 *   head: list of N comma-separated exprs
 *   count: N >= 2
 * AssignExpr (BinaryNode)
 *   left: target of assignment
 *   right: value to assign
 * AddAssignExpr, SubAssignExpr, BitOrAssignExpr, BitXorAssignExpr,
 * BitAndAssignExpr, LshAssignExpr, RshAssignExpr, UrshAssignExpr,
 * MulAssignExpr, DivAssignExpr, ModAssignExpr, PowAssignExpr (AssignmentNode)
 *   left: target of assignment
 *   right: value to assign
 * ConditionalExpr (ConditionalExpression)
 *   (cond ? thenExpr : elseExpr)
 *   kid1: cond
 *   kid2: thenExpr
 *   kid3: elseExpr
 * PipelineExpr, OrExpr, AndExpr, BitOrExpr, BitXorExpr, BitAndExpr,
 * StrictEqExpr, EqExpr, StrictNeExpr, NeExpr, LtExpr, LeExpr, GtExpr, GeExpr,
 * InstanceOfExpr, InExpr, LshExpr, RshExpr, UrshExpr, AddExpr, SubExpr,
 * MulExpr, DivExpr, ModExpr, PowExpr (ListNode)
 *   head: list of N subexpressions
 *         All of these operators are left-associative except Pow which is
 *         right-associative, but still forms a list (see comments in
 *         ParseNode::appendOrCreateList).
 *   count: N >= 2
 * PosExpr, NegExpr, VoidExpr, NotExpr, BitNotExpr, TypeOfNameExpr,
 * TypeOfExpr (UnaryNode)
 *   kid: unary expr
 * PreIncrementExpr, PostIncrementExpr, PreDecrementExpr,
 * PostDecrementExpr (UnaryNode)
 *   kid: member expr
 * NewExpr (BinaryNode)
 *   left: ctor expression on the left of the '('
 *   right: Arguments
 * DeleteNameExpr, DeletePropExpr, DeleteElemExpr, DeleteExpr (UnaryNode)
 *   kid: expression that's evaluated, then the overall delete evaluates to
 *        true; can't be a kind for a more-specific ParseNodeKind::Delete*
 *        unless constant folding (or a similar parse tree manipulation) has
 *        occurred
 *          * DeleteNameExpr: Name expr
 *          * DeletePropExpr: Dot expr
 *          * DeleteElemExpr: Elem expr
 *          * DeleteExpr: Member expr
 * PropertyNameExpr (NameNode)
 *   atom: property name being accessed
 * DotExpr (PropertyAccess)
 *   left: MEMBER expr to left of '.'
 *   right: PropertyName to right of '.'
 * ElemExpr (PropertyByValue)
 *   left: MEMBER expr to left of '['
 *   right: expr between '[' and ']'
 * CallExpr (BinaryNode)
 *   left: callee expression on the left of the '('
 *   right: Arguments
 * Arguments (ListNode)
 *   head: list of arg1, arg2, ... argN
 *   count: N >= 0
 * ArrayExpr (ListNode)
 *   head: list of N array element expressions
 *         holes ([,,]) are represented by Elision nodes,
 *         spread elements ([...X]) are represented by Spread nodes
 *   count: N >= 0
 * ObjectExpr (ListNode)
 *   head: list of N nodes, each item is one of:
 *           * MutateProto
 *           * PropertyDefinition
 *           * Shorthand
 *           * Spread
 *   count: N >= 0
 * PropertyDefinition (PropertyDefinition)
 *   key-value pair in object initializer or destructuring lhs
 *   left: property id
 *   right: value
 * Shorthand (BinaryNode)
 *   Same fields as PropertyDefinition. This is used for object literal
 *   properties using shorthand ({x}).
 * ComputedName (UnaryNode)
 *   ES6 ComputedPropertyName.
 *   kid: the AssignmentExpression inside the square brackets
 * Name (NameNode)
 *   atom: name, or object atom
 * StringExpr (NameNode)
 *   atom: string
 * TemplateStringListExpr (ListNode)
 *   head: list of alternating expr and template strings
 *           TemplateString [, expression, TemplateString]+
 *         there's at least one expression.  If the template literal contains
 *         no ${}-delimited expression, it's parsed as a single TemplateString
 * TemplateStringExpr (NameNode)
 *   atom: template string atom
 * TaggedTemplateExpr (BinaryNode)
 *   left: tag expression
 *   right: Arguments, with the first being the call site object, then
 *          arg1, arg2, ... argN
 * CallSiteObj (CallSiteNode)
 *   head:  an Array of raw TemplateString, then corresponding cooked
 *          TemplateString nodes
 *            Array [, cooked TemplateString]+
 *          where the Array is
 *            [raw TemplateString]+
 * RegExpExpr (RegExpLiteral)
 *   regexp: RegExp model object
 * NumberExpr (NumericLiteral)
 *   value: double value of numeric literal
<<<<<<< HEAD
 * BigInt (BigIntLiteral)
 *   box: BigIntBox holding BigInt* value
 * True, False (BooleanLiteral)
 *   pn_op: JSOp bytecode
 * Null (NullLiteral)
 *   pn_op: JSOp bytecode
 * RawUndefined (RawUndefinedLiteral)
 *   pn_op: JSOp bytecode
||||||| merged common ancestors
 * True, False (BooleanLiteral)
 *   pn_op: JSOp bytecode
 * Null (NullLiteral)
 *   pn_op: JSOp bytecode
 * RawUndefined (RawUndefinedLiteral)
 *   pn_op: JSOp bytecode
=======
 * BigIntExpr (BigIntLiteral)
 *   box: BigIntBox holding BigInt* value
 * TrueExpr, FalseExpr (BooleanLiteral)
 * NullExpr (NullLiteral)
 * RawUndefinedExpr (RawUndefinedLiteral)
>>>>>>> upstream-releases
 *
 * ThisExpr (UnaryNode)
 *   kid: '.this' Name if function `this`, else nullptr
 * SuperBase (UnaryNode)
 *   kid: '.this' Name
 * SuperCallExpr (BinaryNode)
 *   left: SuperBase
 *   right: Arguments
 * SetThis (BinaryNode)
 *   left: '.this' Name
 *   right: SuperCall
 *
 * LexicalScope (LexicalScopeNode)
 *   scopeBindings: scope bindings
 *   scopeBody: scope body
 * Generator (NullaryNode)
 * InitialYield (UnaryNode)
 *   kid: generator object
 * YieldExpr, YieldStarExpr, AwaitExpr (UnaryNode)
 *   kid: expr or null
 */
<<<<<<< HEAD
enum ParseNodeArity {
  PN_NULLARY, /* 0 kids */
  PN_UNARY,   /* one kid, plus a couple of scalars */
  PN_BINARY,  /* two kids, plus a couple of scalars */
  PN_TERNARY, /* three kids */
  PN_CODE,    /* module or function definition node */
  PN_LIST,    /* generic singly linked list */
  PN_NAME,    /* name, label, string */
  PN_FIELD,   /* field name, optional initializer */
  PN_NUMBER,  /* numeric literal */
#ifdef ENABLE_BIGINT
  PN_BIGINT, /* BigInt literal */
#endif
  PN_REGEXP, /* regexp literal */
  PN_LOOP,   /* loop control (break/continue) */
  PN_SCOPE   /* lexical scope */
};
||||||| merged common ancestors
enum ParseNodeArity
{
    PN_NULLARY,                         /* 0 kids */
    PN_UNARY,                           /* one kid, plus a couple of scalars */
    PN_BINARY,                          /* two kids, plus a couple of scalars */
    PN_TERNARY,                         /* three kids */
    PN_CODE,                            /* module or function definition node */
    PN_LIST,                            /* generic singly linked list */
    PN_NAME,                            /* name, label, string */
    PN_NUMBER,                          /* numeric literal */
    PN_REGEXP,                          /* regexp literal */
    PN_LOOP,                            /* loop control (break/continue) */
    PN_SCOPE                            /* lexical scope */
};
=======
>>>>>>> upstream-releases

// FIXME: Remove `*Type` (bug 1489008)
<<<<<<< HEAD
#define FOR_EACH_PARSENODE_SUBCLASS(MACRO)                                   \
  MACRO(BinaryNode, BinaryNodeType, asBinary)                                \
  MACRO(AssignmentNode, AssignmentNodeType, asAssignment)                    \
  MACRO(CaseClause, CaseClauseType, asCaseClause)                            \
  MACRO(ClassMethod, ClassMethodType, asClassMethod)                         \
  MACRO(ClassField, ClassFieldType, asClassField)                            \
  MACRO(ClassNames, ClassNamesType, asClassNames)                            \
  MACRO(ForNode, ForNodeType, asFor)                                         \
  MACRO(PropertyAccess, PropertyAccessType, asPropertyAccess)                \
  MACRO(PropertyByValue, PropertyByValueType, asPropertyByValue)             \
  MACRO(SwitchStatement, SwitchStatementType, asSwitchStatement)             \
                                                                             \
  MACRO(CodeNode, CodeNodeType, asCode)                                      \
                                                                             \
  MACRO(LexicalScopeNode, LexicalScopeNodeType, asLexicalScope)              \
                                                                             \
  MACRO(ListNode, ListNodeType, asList)                                      \
  MACRO(CallSiteNode, CallSiteNodeType, asCallSite)                          \
                                                                             \
  MACRO(LoopControlStatement, LoopControlStatementType,                      \
        asLoopControlStatement)                                              \
  MACRO(BreakStatement, BreakStatementType, asBreakStatement)                \
  MACRO(ContinueStatement, ContinueStatementType, asContinueStatement)       \
                                                                             \
  MACRO(NameNode, NameNodeType, asName)                                      \
  MACRO(LabeledStatement, LabeledStatementType, asLabeledStatement)          \
                                                                             \
  MACRO(NullaryNode, NullaryNodeType, asNullary)                             \
  MACRO(BooleanLiteral, BooleanLiteralType, asBooleanLiteral)                \
  MACRO(DebuggerStatement, DebuggerStatementType, asDebuggerStatement)       \
  MACRO(NullLiteral, NullLiteralType, asNullLiteral)                         \
  MACRO(RawUndefinedLiteral, RawUndefinedLiteralType, asRawUndefinedLiteral) \
                                                                             \
  MACRO(NumericLiteral, NumericLiteralType, asNumericLiteral)                \
  IF_BIGINT(MACRO(BigIntLiteral, BigIntLiteralType, asBigIntLiteral), )      \
                                                                             \
  MACRO(RegExpLiteral, RegExpLiteralType, asRegExpLiteral)                   \
                                                                             \
  MACRO(TernaryNode, TernaryNodeType, asTernary)                             \
  MACRO(ClassNode, ClassNodeType, asClass)                                   \
  MACRO(ConditionalExpression, ConditionalExpressionType,                    \
        asConditionalExpression)                                             \
  MACRO(TryNode, TryNodeType, asTry)                                         \
                                                                             \
  MACRO(UnaryNode, UnaryNodeType, asUnary)                                   \
  MACRO(ThisLiteral, ThisLiteralType, asThisLiteral)

#define DECLARE_CLASS(typeName, longTypeName, asMethodName) class typeName;
||||||| merged common ancestors
#define FOR_EACH_PARSENODE_SUBCLASS(macro) \
    macro(BinaryNode, BinaryNodeType, asBinary) \
    macro(AssignmentNode, AssignmentNodeType, asAssignment) \
    macro(CaseClause, CaseClauseType, asCaseClause) \
    macro(ClassMethod, ClassMethodType, asClassMethod) \
    macro(ClassNames, ClassNamesType, asClassNames) \
    macro(ForNode, ForNodeType, asFor) \
    macro(PropertyAccess, PropertyAccessType, asPropertyAccess) \
    macro(PropertyByValue, PropertyByValueType, asPropertyByValue) \
    macro(SwitchStatement, SwitchStatementType, asSwitchStatement) \
    \
    macro(CodeNode, CodeNodeType, asCode) \
    \
    macro(LexicalScopeNode, LexicalScopeNodeType, asLexicalScope) \
    \
    macro(ListNode, ListNodeType, asList) \
    macro(CallSiteNode, CallSiteNodeType, asCallSite) \
    \
    macro(LoopControlStatement, LoopControlStatementType, asLoopControlStatement) \
    macro(BreakStatement, BreakStatementType, asBreakStatement) \
    macro(ContinueStatement, ContinueStatementType, asContinueStatement) \
    \
    macro(NameNode, NameNodeType, asName) \
    macro(LabeledStatement, LabeledStatementType, asLabeledStatement) \
    \
    macro(NullaryNode, NullaryNodeType, asNullary) \
    macro(BooleanLiteral, BooleanLiteralType, asBooleanLiteral) \
    macro(DebuggerStatement, DebuggerStatementType, asDebuggerStatement) \
    macro(NullLiteral, NullLiteralType, asNullLiteral) \
    macro(RawUndefinedLiteral, RawUndefinedLiteralType, asRawUndefinedLiteral) \
    \
    macro(NumericLiteral, NumericLiteralType, asNumericLiteral) \
    \
    macro(RegExpLiteral, RegExpLiteralType, asRegExpLiteral) \
    \
    macro(TernaryNode, TernaryNodeType, asTernary) \
    macro(ClassNode, ClassNodeType, asClass) \
    macro(ConditionalExpression, ConditionalExpressionType, asConditionalExpression) \
    macro(TryNode, TryNodeType, asTry) \
    \
    macro(UnaryNode, UnaryNodeType, asUnary) \
    macro(ThisLiteral, ThisLiteralType, asThisLiteral)

#define DECLARE_CLASS(typeName, longTypeName, asMethodName) \
class typeName;
=======
#define FOR_EACH_PARSENODE_SUBCLASS(MACRO)                                   \
  MACRO(BinaryNode, BinaryNodeType, asBinary)                                \
  MACRO(AssignmentNode, AssignmentNodeType, asAssignment)                    \
  MACRO(CaseClause, CaseClauseType, asCaseClause)                            \
  MACRO(ClassMethod, ClassMethodType, asClassMethod)                         \
  MACRO(ClassField, ClassFieldType, asClassField)                            \
  MACRO(PropertyDefinition, PropertyDefinitionType, asPropertyDefinition)    \
  MACRO(ClassNames, ClassNamesType, asClassNames)                            \
  MACRO(ForNode, ForNodeType, asFor)                                         \
  MACRO(PropertyAccess, PropertyAccessType, asPropertyAccess)                \
  MACRO(PropertyByValue, PropertyByValueType, asPropertyByValue)             \
  MACRO(SwitchStatement, SwitchStatementType, asSwitchStatement)             \
                                                                             \
  MACRO(FunctionNode, FunctionNodeType, asFunction)                          \
  MACRO(ModuleNode, ModuleNodeType, asModule)                                \
                                                                             \
  MACRO(LexicalScopeNode, LexicalScopeNodeType, asLexicalScope)              \
                                                                             \
  MACRO(ListNode, ListNodeType, asList)                                      \
  MACRO(CallSiteNode, CallSiteNodeType, asCallSite)                          \
  MACRO(CallNode, CallNodeType, asCallNode)                                  \
                                                                             \
  MACRO(LoopControlStatement, LoopControlStatementType,                      \
        asLoopControlStatement)                                              \
  MACRO(BreakStatement, BreakStatementType, asBreakStatement)                \
  MACRO(ContinueStatement, ContinueStatementType, asContinueStatement)       \
                                                                             \
  MACRO(NameNode, NameNodeType, asName)                                      \
  MACRO(LabeledStatement, LabeledStatementType, asLabeledStatement)          \
                                                                             \
  MACRO(NullaryNode, NullaryNodeType, asNullary)                             \
  MACRO(BooleanLiteral, BooleanLiteralType, asBooleanLiteral)                \
  MACRO(DebuggerStatement, DebuggerStatementType, asDebuggerStatement)       \
  MACRO(NullLiteral, NullLiteralType, asNullLiteral)                         \
  MACRO(RawUndefinedLiteral, RawUndefinedLiteralType, asRawUndefinedLiteral) \
                                                                             \
  MACRO(NumericLiteral, NumericLiteralType, asNumericLiteral)                \
  MACRO(BigIntLiteral, BigIntLiteralType, asBigIntLiteral)                   \
                                                                             \
  MACRO(RegExpLiteral, RegExpLiteralType, asRegExpLiteral)                   \
                                                                             \
  MACRO(TernaryNode, TernaryNodeType, asTernary)                             \
  MACRO(ClassNode, ClassNodeType, asClass)                                   \
  MACRO(ConditionalExpression, ConditionalExpressionType,                    \
        asConditionalExpression)                                             \
  MACRO(TryNode, TryNodeType, asTry)                                         \
                                                                             \
  MACRO(UnaryNode, UnaryNodeType, asUnary)                                   \
  MACRO(ThisLiteral, ThisLiteralType, asThisLiteral)

#define DECLARE_CLASS(typeName, longTypeName, asMethodName) class typeName;
>>>>>>> upstream-releases
FOR_EACH_PARSENODE_SUBCLASS(DECLARE_CLASS)
#undef DECLARE_CLASS

<<<<<<< HEAD
// ParseNodeKindArity[size_t(pnk)] is the arity of a ParseNode of kind pnk.
extern const ParseNodeArity ParseNodeKindArity[];

class ParseNode {
  ParseNodeKind pn_type; /* ParseNodeKind::PNK_* type */

  // pn_op is not declared as the correct enum type due to difficulties with
  // MS bitfield layout rules and a GCC bug.  See
  // https://bugzilla.mozilla.org/show_bug.cgi?id=1383157#c4 for details.
  uint8_t pn_op;            /* see JSOp enum and jsopcode.tbl */
  bool pn_parens : 1;       /* this expr was enclosed in parens */
  bool pn_rhs_anon_fun : 1; /* this expr is anonymous function or class that
                             * is a direct RHS of ParseNodeKind::Assign or
                             * ParseNodeKind::Colon of property, that needs
                             * SetFunctionName. */

  ParseNode(const ParseNode& other) = delete;
  void operator=(const ParseNode& other) = delete;

 public:
  ParseNode(ParseNodeKind kind, JSOp op)
||||||| merged common ancestors
class ParseNode
{
    ParseNodeKind pn_type;   /* ParseNodeKind::PNK_* type */
    // pn_op and pn_arity are not declared as the correct enum types
    // due to difficulties with MS bitfield layout rules and a GCC
    // bug.  See https://bugzilla.mozilla.org/show_bug.cgi?id=1383157#c4 for
    // details.
    uint8_t pn_op;      /* see JSOp enum and jsopcode.tbl */
    uint8_t pn_arity:4; /* see ParseNodeArity enum */
    bool pn_parens:1;   /* this expr was enclosed in parens */
    bool pn_rhs_anon_fun:1;  /* this expr is anonymous function or class that
                              * is a direct RHS of ParseNodeKind::Assign or ParseNodeKind::Colon of
                              * property, that needs SetFunctionName. */

    ParseNode(const ParseNode& other) = delete;
    void operator=(const ParseNode& other) = delete;

  public:
    ParseNode(ParseNodeKind kind, JSOp op, ParseNodeArity arity)
=======
enum class FunctionSyntaxKind {
  // A non-arrow function expression.
  Expression,

  // A named function appearing as a Statement.
  Statement,

  Arrow,

  // Method of a class or object. Field initializers also desugar to methods.
  Method,

  ClassConstructor,
  DerivedClassConstructor,
  Getter,
  Setter,
};

enum class AccessorType { None, Getter, Setter };

static inline bool IsConstructorKind(FunctionSyntaxKind kind) {
  return kind == FunctionSyntaxKind::ClassConstructor ||
         kind == FunctionSyntaxKind::DerivedClassConstructor;
}

static inline bool IsMethodDefinitionKind(FunctionSyntaxKind kind) {
  return IsConstructorKind(kind) || kind == FunctionSyntaxKind::Method ||
         kind == FunctionSyntaxKind::Getter ||
         kind == FunctionSyntaxKind::Setter;
}

// To help diagnose sporadic crashes in the frontend, a few assertions are
// enabled in early beta builds. (Most are not; those still use MOZ_ASSERT.)
// See bug 1547561.
#if defined(EARLY_BETA_OR_EARLIER)
#  define JS_PARSE_NODE_ASSERT MOZ_RELEASE_ASSERT
#else
#  define JS_PARSE_NODE_ASSERT MOZ_ASSERT
#endif

class ParseNode {
  const ParseNodeKind pn_type; /* ParseNodeKind::PNK_* type */

  bool pn_parens : 1;       /* this expr was enclosed in parens */
  bool pn_rhs_anon_fun : 1; /* this expr is anonymous function or class that
                             * is a direct RHS of ParseNodeKind::Assign or
                             * ParseNodeKind::PropertyDefinition of property,
                             * that needs SetFunctionName. */

  ParseNode(const ParseNode& other) = delete;
  void operator=(const ParseNode& other) = delete;

 public:
  explicit ParseNode(ParseNodeKind kind)
>>>>>>> upstream-releases
      : pn_type(kind),
<<<<<<< HEAD
        pn_op(op),
||||||| merged common ancestors
        pn_op(op),
        pn_arity(arity),
=======
>>>>>>> upstream-releases
        pn_parens(false),
        pn_rhs_anon_fun(false),
        pn_pos(0, 0),
<<<<<<< HEAD
        pn_next(nullptr) {
    MOZ_ASSERT(kind < ParseNodeKind::Limit);
    memset(&pn_u, 0, sizeof pn_u);
  }
||||||| merged common ancestors
        pn_next(nullptr)
    {
        MOZ_ASSERT(kind < ParseNodeKind::Limit);
        memset(&pn_u, 0, sizeof pn_u);
    }
=======
        pn_next(nullptr) {
    JS_PARSE_NODE_ASSERT(ParseNodeKind::Start <= kind);
    JS_PARSE_NODE_ASSERT(kind < ParseNodeKind::Limit);
  }
>>>>>>> upstream-releases

<<<<<<< HEAD
  ParseNode(ParseNodeKind kind, JSOp op, const TokenPos& pos)
||||||| merged common ancestors
    ParseNode(ParseNodeKind kind, JSOp op, ParseNodeArity arity, const TokenPos& pos)
=======
  ParseNode(ParseNodeKind kind, const TokenPos& pos)
>>>>>>> upstream-releases
      : pn_type(kind),
<<<<<<< HEAD
        pn_op(op),
||||||| merged common ancestors
        pn_op(op),
        pn_arity(arity),
=======
>>>>>>> upstream-releases
        pn_parens(false),
        pn_rhs_anon_fun(false),
        pn_pos(pos),
<<<<<<< HEAD
        pn_next(nullptr) {
    MOZ_ASSERT(kind < ParseNodeKind::Limit);
    memset(&pn_u, 0, sizeof pn_u);
  }
||||||| merged common ancestors
        pn_next(nullptr)
    {
        MOZ_ASSERT(kind < ParseNodeKind::Limit);
        memset(&pn_u, 0, sizeof pn_u);
    }

    JSOp getOp() const                     { return JSOp(pn_op); }
    void setOp(JSOp op)                    { pn_op = op; }
    bool isOp(JSOp op) const               { return getOp() == op; }

    ParseNodeKind getKind() const {
        MOZ_ASSERT(pn_type < ParseNodeKind::Limit);
        return pn_type;
    }
    void setKind(ParseNodeKind kind) {
        MOZ_ASSERT(kind < ParseNodeKind::Limit);
        pn_type = kind;
    }
    bool isKind(ParseNodeKind kind) const  { return getKind() == kind; }

    ParseNodeArity getArity() const        { return ParseNodeArity(pn_arity); }
    bool isArity(ParseNodeArity a) const   { return getArity() == a; }
    void setArity(ParseNodeArity a)        { pn_arity = a; }

    bool isBinaryOperation() const {
        ParseNodeKind kind = getKind();
        return ParseNodeKind::BinOpFirst <= kind && kind <= ParseNodeKind::BinOpLast;
    }
    inline bool isName(PropertyName* name) const;

    /* Boolean attributes. */
    bool isInParens() const                { return pn_parens; }
    bool isLikelyIIFE() const              { return isInParens(); }
    void setInParens(bool enabled)         { pn_parens = enabled; }

    bool isDirectRHSAnonFunction() const {
        return pn_rhs_anon_fun;
    }
    void setDirectRHSAnonFunction(bool enabled) {
        pn_rhs_anon_fun = enabled;
    }
=======
        pn_next(nullptr) {
    JS_PARSE_NODE_ASSERT(ParseNodeKind::Start <= kind);
    JS_PARSE_NODE_ASSERT(kind < ParseNodeKind::Limit);
  }

  ParseNodeKind getKind() const {
    JS_PARSE_NODE_ASSERT(ParseNodeKind::Start <= pn_type);
    JS_PARSE_NODE_ASSERT(pn_type < ParseNodeKind::Limit);
    return pn_type;
  }
  bool isKind(ParseNodeKind kind) const { return getKind() == kind; }
>>>>>>> upstream-releases

<<<<<<< HEAD
  JSOp getOp() const { return JSOp(pn_op); }
  void setOp(JSOp op) { pn_op = op; }
  bool isOp(JSOp op) const { return getOp() == op; }
||||||| merged common ancestors
    TokenPos            pn_pos;         /* two 16-bit pairs here, for 64 bits */
    ParseNode*          pn_next;        /* intrinsic link in parent PN_LIST */

    union {
        struct {                        /* list of next-linked nodes */
          private:
            friend class ListNode;
            ParseNode*  head;           /* first node in list */
            ParseNode** tail;           /* ptr to last node's pn_next in list */
            uint32_t    count;          /* number of nodes in list */
            uint32_t    xflags;         /* see ListNode class */
        } list;
        struct {                        /* ternary: if, for(;;), ?: */
          private:
            friend class TernaryNode;
            ParseNode*  kid1;           /* condition, discriminant, etc. */
            ParseNode*  kid2;           /* then-part, case list, etc. */
            ParseNode*  kid3;           /* else-part, default case, etc. */
        } ternary;
        struct {                        /* two kids if binary */
          private:
            friend class BinaryNode;
            friend class ForNode;
            friend class ClassMethod;
            friend class PropertyAccess;
            friend class SwitchStatement;
            ParseNode*  left;
            ParseNode*  right;
            union {
                unsigned iflags;        /* JSITER_* flags for ParseNodeKind::For node */
                bool isStatic;          /* only for ParseNodeKind::ClassMethod */
                bool hasDefault;        /* only for ParseNodeKind::Switch */
            };
        } binary;
        struct {                        /* one kid if unary */
          private:
            friend class UnaryNode;
            ParseNode*  kid;
            bool        prologue;       /* directive prologue member */
        } unary;
        struct {                        /* name, labeled statement, etc. */
          private:
            friend class NameNode;
            JSAtom*      atom;          /* lexical name or label atom */
            ParseNode*  initOrStmt;     /* var initializer, argument default,
                                         * or label statement target */
        } name;
        struct {
          private:
            friend class RegExpLiteral;
            ObjectBox* objbox;
        } regexp;
        struct {
          private:
            friend class CodeNode;
            FunctionBox* funbox;        /* function object */
            ParseNode*  body;           /* module or function body */
        } code;
        struct {
          private:
            friend class LexicalScopeNode;
            LexicalScope::Data* bindings;
            ParseNode*          body;
        } scope;
        struct {
          private:
            friend class NumericLiteral;
            double       value;         /* aligned numeric literal value */
            DecimalPoint decimalPoint;  /* Whether the number has a decimal point */
        } number;
        class {
          private:
            friend class LoopControlStatement;
            PropertyName*    label;    /* target of break/continue statement */
        } loopControl;
    } pn_u;

  public:
    /*
     * If |left| is a list of the given kind/left-associative op, append
     * |right| to it and return |left|.  Otherwise return a [left, right] list.
     */
    static ParseNode*
    appendOrCreateList(ParseNodeKind kind, ParseNode* left, ParseNode* right,
                       FullParseHandler* handler, ParseContext* pc);

    /* True if pn is a parsenode representing a literal constant. */
    bool isLiteral() const {
        return isKind(ParseNodeKind::Number) ||
               isKind(ParseNodeKind::String) ||
               isKind(ParseNodeKind::True) ||
               isKind(ParseNodeKind::False) ||
               isKind(ParseNodeKind::Null) ||
               isKind(ParseNodeKind::RawUndefined);
    }
=======
 protected:
  size_t getKindAsIndex() const {
    return size_t(getKind()) - size_t(ParseNodeKind::Start);
  }
>>>>>>> upstream-releases

<<<<<<< HEAD
  ParseNodeKind getKind() const {
    MOZ_ASSERT(pn_type < ParseNodeKind::Limit);
    return pn_type;
  }
  void setKind(ParseNodeKind kind) {
    MOZ_ASSERT(kind < ParseNodeKind::Limit);
    pn_type = kind;
  }
  bool isKind(ParseNodeKind kind) const { return getKind() == kind; }
||||||| merged common ancestors
    // True iff this is a for-in/of loop variable declaration (var/let/const).
    inline bool isForLoopDeclaration() const;
=======
  // Used to implement test() on a few ParseNodes efficiently.
  // (This enum doesn't fully reflect the ParseNode class hierarchy,
  // so don't use it for anything else.)
  enum class TypeCode : uint8_t {
    Nullary,
    Unary,
    Binary,
    Ternary,
    List,
    Name,
    Other
  };

  // typeCodeTable[getKindAsIndex()] is the type code of a ParseNode of kind
  // pnk.
  static const TypeCode typeCodeTable[];

 private:
#ifdef DEBUG
  static const size_t sizeTable[];
#endif
>>>>>>> upstream-releases

<<<<<<< HEAD
  ParseNodeArity getArity() const {
    return ParseNodeKindArity[size_t(getKind())];
  }
  bool isArity(ParseNodeArity a) const { return getArity() == a; }
||||||| merged common ancestors
    enum AllowConstantObjects {
        DontAllowObjects = 0,
        AllowObjects,
        ForCopyOnWriteArray
    };
=======
 public:
  TypeCode typeCode() const { return typeCodeTable[getKindAsIndex()]; }
>>>>>>> upstream-releases

<<<<<<< HEAD
  bool isBinaryOperation() const {
    ParseNodeKind kind = getKind();
    return ParseNodeKind::BinOpFirst <= kind &&
           kind <= ParseNodeKind::BinOpLast;
  }
  inline bool isName(PropertyName* name) const;

  /* Boolean attributes. */
  bool isInParens() const { return pn_parens; }
  bool isLikelyIIFE() const { return isInParens(); }
  void setInParens(bool enabled) { pn_parens = enabled; }

  bool isDirectRHSAnonFunction() const { return pn_rhs_anon_fun; }
  void setDirectRHSAnonFunction(bool enabled) { pn_rhs_anon_fun = enabled; }

  TokenPos pn_pos;    /* two 16-bit pairs here, for 64 bits */
  ParseNode* pn_next; /* intrinsic link in parent PN_LIST */

  union {
    struct { /* list of next-linked nodes */
     private:
      friend class ListNode;
      ParseNode* head;  /* first node in list */
      ParseNode** tail; /* ptr to last node's pn_next in list */
      uint32_t count;   /* number of nodes in list */
      uint32_t xflags;  /* see ListNode class */
    } list;
    struct { /* ternary: if, for(;;), ?: */
     private:
      friend class TernaryNode;
      ParseNode* kid1; /* condition, discriminant, etc. */
      ParseNode* kid2; /* then-part, case list, etc. */
      ParseNode* kid3; /* else-part, default case, etc. */
    } ternary;
    struct { /* two kids if binary */
     private:
      friend class BinaryNode;
      friend class ForNode;
      friend class ClassMethod;
      friend class PropertyAccess;
      friend class SwitchStatement;
      ParseNode* left;
      ParseNode* right;
      union {
        unsigned iflags; /* JSITER_* flags for ParseNodeKind::For node */
        bool isStatic;   /* only for ParseNodeKind::ClassMethod */
        bool hasDefault; /* only for ParseNodeKind::Switch */
      };
    } binary;
    struct { /* one kid if unary */
     private:
      friend class UnaryNode;
      ParseNode* kid;
      bool prologue; /* directive prologue member */
    } unary;
    struct { /* name, labeled statement, etc. */
     private:
      friend class NameNode;
      JSAtom* atom;          /* lexical name or label atom */
      ParseNode* initOrStmt; /* var initializer, argument default,
                              * or label statement target */
    } name;
    struct {
     private:
      friend class ClassField;
      ParseNode* name;
      ParseNode* initializer; /* field initializer - optional */
    } field;
    struct {
     private:
      friend class RegExpLiteral;
      ObjectBox* objbox;
    } regexp;
    struct {
     private:
      friend class CodeNode;
      FunctionBox* funbox; /* function object */
      ParseNode* body;     /* module or function body */
    } code;
    struct {
     private:
      friend class LexicalScopeNode;
      LexicalScope::Data* bindings;
      ParseNode* body;
    } scope;
    struct {
     private:
      friend class NumericLiteral;
      double value;              /* aligned numeric literal value */
      DecimalPoint decimalPoint; /* Whether the number has a decimal point */
    } number;
#ifdef ENABLE_BIGINT
    struct {
     private:
      friend class BigIntLiteral;
      BigIntBox* box;
    } bigint;
#endif
    class {
     private:
      friend class LoopControlStatement;
      PropertyName* label; /* target of break/continue statement */
    } loopControl;
  } pn_u;

 public:
  /*
   * If |left| is a list of the given kind/left-associative op, append
   * |right| to it and return |left|.  Otherwise return a [left, right] list.
   */
  static ParseNode* appendOrCreateList(ParseNodeKind kind, ParseNode* left,
                                       ParseNode* right,
                                       FullParseHandler* handler,
                                       ParseContext* pc);

  /* True if pn is a parsenode representing a literal constant. */
  bool isLiteral() const {
    return isKind(ParseNodeKind::Number) ||
#ifdef ENABLE_BIGINT
           isKind(ParseNodeKind::BigInt) ||
#endif
           isKind(ParseNodeKind::String) || isKind(ParseNodeKind::True) ||
           isKind(ParseNodeKind::False) || isKind(ParseNodeKind::Null) ||
           isKind(ParseNodeKind::RawUndefined);
  }
||||||| merged common ancestors
    MOZ_MUST_USE bool getConstantValue(JSContext* cx, AllowConstantObjects allowObjects,
                                       MutableHandleValue vp, Value* compare = nullptr,
                                       size_t ncompare = 0, NewObjectKind newKind = TenuredObject);
    inline bool isConstant();
=======
  bool isBinaryOperation() const {
    ParseNodeKind kind = getKind();
    return ParseNodeKind::BinOpFirst <= kind &&
           kind <= ParseNodeKind::BinOpLast;
  }
  inline bool isName(PropertyName* name) const;

  /* Boolean attributes. */
  bool isInParens() const { return pn_parens; }
  bool isLikelyIIFE() const { return isInParens(); }
  void setInParens(bool enabled) { pn_parens = enabled; }

  bool isDirectRHSAnonFunction() const { return pn_rhs_anon_fun; }
  void setDirectRHSAnonFunction(bool enabled) { pn_rhs_anon_fun = enabled; }

  TokenPos pn_pos;    /* two 16-bit pairs here, for 64 bits */
  ParseNode* pn_next; /* intrinsic link in parent PN_LIST */

 public:
  /*
   * If |left| is a list of the given kind/left-associative op, append
   * |right| to it and return |left|.  Otherwise return a [left, right] list.
   */
  static ParseNode* appendOrCreateList(ParseNodeKind kind, ParseNode* left,
                                       ParseNode* right,
                                       FullParseHandler* handler,
                                       ParseContext* pc);

  /* True if pn is a parsenode representing a literal constant. */
  bool isLiteral() const {
    return isKind(ParseNodeKind::NumberExpr) ||
           isKind(ParseNodeKind::BigIntExpr) ||
           isKind(ParseNodeKind::StringExpr) ||
           isKind(ParseNodeKind::TrueExpr) ||
           isKind(ParseNodeKind::FalseExpr) ||
           isKind(ParseNodeKind::NullExpr) ||
           isKind(ParseNodeKind::RawUndefinedExpr);
  }
>>>>>>> upstream-releases

  // True iff this is a for-in/of loop variable declaration (var/let/const).
  inline bool isForLoopDeclaration() const;

  enum AllowConstantObjects {
    DontAllowObjects = 0,
    AllowObjects,
    ForCopyOnWriteArray
  };

  MOZ_MUST_USE bool getConstantValue(JSContext* cx,
                                     AllowConstantObjects allowObjects,
                                     MutableHandleValue vp,
                                     Value* compare = nullptr,
                                     size_t ncompare = 0,
                                     NewObjectKind newKind = TenuredObject);
  inline bool isConstant();

  template <class NodeType>
  inline bool is() const {
    return NodeType::test(*this);
  }

  /* Casting operations. */
  template <class NodeType>
  inline NodeType& as() {
    MOZ_ASSERT(NodeType::test(*this));
    return *static_cast<NodeType*>(this);
  }

  template <class NodeType>
  inline const NodeType& as() const {
    MOZ_ASSERT(NodeType::test(*this));
    return *static_cast<const NodeType*>(this);
  }

#ifdef DEBUG
<<<<<<< HEAD
  // Debugger-friendly stderr printer.
  void dump();
  void dump(GenericPrinter& out);
  void dump(GenericPrinter& out, int indent);
||||||| merged common ancestors
    // Debugger-friendly stderr printer.
    void dump();
    void dump(GenericPrinter& out);
    void dump(GenericPrinter& out, int indent);
=======
  // Debugger-friendly stderr printer.
  void dump();
  void dump(GenericPrinter& out);
  void dump(GenericPrinter& out, int indent);

  // The size of this node, in bytes.
  size_t size() const { return sizeTable[getKindAsIndex()]; }
>>>>>>> upstream-releases
#endif
};

<<<<<<< HEAD
class NullaryNode : public ParseNode {
 public:
  NullaryNode(ParseNodeKind kind, const TokenPos& pos)
      : ParseNode(kind, JSOP_NOP, pos) {
    MOZ_ASSERT(is<NullaryNode>());
  }
||||||| merged common ancestors
class NullaryNode : public ParseNode
{
  public:
    NullaryNode(ParseNodeKind kind, const TokenPos& pos)
      : ParseNode(kind, JSOP_NOP, PN_NULLARY, pos) {}
=======
// Remove a ParseNode, **pnp, from a parse tree, putting another ParseNode,
// *pn, in its place.
//
// pnp points to a ParseNode pointer. This must be the only pointer that points
// to the parse node being replaced. The replacement, *pn, is unchanged except
// for its pn_next pointer; updating that is necessary if *pn's new parent is a
// list node.
inline void ReplaceNode(ParseNode** pnp, ParseNode* pn) {
  pn->pn_next = (*pnp)->pn_next;
  *pnp = pn;
}
>>>>>>> upstream-releases

<<<<<<< HEAD
  NullaryNode(ParseNodeKind kind, JSOp op, const TokenPos& pos)
      : ParseNode(kind, op, pos) {
    MOZ_ASSERT(is<NullaryNode>());
  }
||||||| merged common ancestors
    NullaryNode(ParseNodeKind kind, JSOp op, const TokenPos& pos)
      : ParseNode(kind, op, PN_NULLARY, pos) {}
=======
class NullaryNode : public ParseNode {
 public:
  NullaryNode(ParseNodeKind kind, const TokenPos& pos) : ParseNode(kind, pos) {
    MOZ_ASSERT(is<NullaryNode>());
  }
>>>>>>> upstream-releases

<<<<<<< HEAD
  static bool test(const ParseNode& node) { return node.isArity(PN_NULLARY); }
||||||| merged common ancestors
    static bool test(const ParseNode& node) {
        return node.isArity(PN_NULLARY);
    }
=======
  static bool test(const ParseNode& node) {
    return node.typeCode() == TypeCode::Nullary;
  }

  static constexpr TypeCode classTypeCode() { return TypeCode::Nullary; }

  template <typename Visitor>
  bool accept(Visitor& visitor) {
    return true;
  }
>>>>>>> upstream-releases

#ifdef DEBUG
<<<<<<< HEAD
  void dump(GenericPrinter& out);
||||||| merged common ancestors
    void dump(GenericPrinter& out);
=======
  void dumpImpl(GenericPrinter& out, int indent);
>>>>>>> upstream-releases
#endif
};

<<<<<<< HEAD
class NameNode : public ParseNode {
 protected:
  NameNode(ParseNodeKind kind, JSOp op, JSAtom* atom, ParseNode* initOrStmt,
           const TokenPos& pos)
      : ParseNode(kind, op, pos) {
    pn_u.name.atom = atom;
    pn_u.name.initOrStmt = initOrStmt;
    MOZ_ASSERT(is<NameNode>());
  }
||||||| merged common ancestors
class NameNode : public ParseNode
{
  protected:
    NameNode(ParseNodeKind kind, JSOp op, JSAtom* atom, ParseNode* initOrStmt, const TokenPos& pos)
      : ParseNode(kind, op, PN_NAME, pos)
    {
        pn_u.name.atom = atom;
        pn_u.name.initOrStmt = initOrStmt;
    }
=======
class NameNode : public ParseNode {
  JSAtom* atom_; /* lexical name or label atom */
>>>>>>> upstream-releases

<<<<<<< HEAD
 public:
  NameNode(ParseNodeKind kind, JSOp op, JSAtom* atom, const TokenPos& pos)
      : ParseNode(kind, op, pos) {
    pn_u.name.atom = atom;
    pn_u.name.initOrStmt = nullptr;
    MOZ_ASSERT(is<NameNode>());
  }
||||||| merged common ancestors
  public:
    NameNode(ParseNodeKind kind, JSOp op, JSAtom* atom, const TokenPos& pos)
      : ParseNode(kind, op, PN_NAME, pos)
    {
        pn_u.name.atom = atom;
        pn_u.name.initOrStmt = nullptr;
    }
=======
 public:
  NameNode(ParseNodeKind kind, JSAtom* atom, const TokenPos& pos)
      : ParseNode(kind, pos), atom_(atom) {
    MOZ_ASSERT(is<NameNode>());
  }
>>>>>>> upstream-releases

<<<<<<< HEAD
  static bool test(const ParseNode& node) { return node.isArity(PN_NAME); }
||||||| merged common ancestors
    static bool test(const ParseNode& node) {
        return node.isArity(PN_NAME);
    }
=======
  static bool test(const ParseNode& node) {
    return node.typeCode() == TypeCode::Name;
  }

  static constexpr TypeCode classTypeCode() { return TypeCode::Name; }

  template <typename Visitor>
  bool accept(Visitor& visitor) {
    return true;
  }
>>>>>>> upstream-releases

#ifdef DEBUG
<<<<<<< HEAD
  void dump(GenericPrinter& out, int indent);
||||||| merged common ancestors
    void dump(GenericPrinter& out, int indent);
=======
  void dumpImpl(GenericPrinter& out, int indent);
>>>>>>> upstream-releases
#endif

<<<<<<< HEAD
  JSAtom* atom() const { return pn_u.name.atom; }
||||||| merged common ancestors
    JSAtom* atom() const {
        return pn_u.name.atom;
    }
=======
  JSAtom* atom() const { return atom_; }
>>>>>>> upstream-releases

  PropertyName* name() const {
    MOZ_ASSERT(isKind(ParseNodeKind::Name));
    return atom()->asPropertyName();
  }

<<<<<<< HEAD
  ParseNode* initializer() const { return pn_u.name.initOrStmt; }
||||||| merged common ancestors
    ParseNode* initializer() const {
        return pn_u.name.initOrStmt;
    }
=======
  void setAtom(JSAtom* atom) { atom_ = atom; }
};
>>>>>>> upstream-releases

<<<<<<< HEAD
  void setAtom(JSAtom* atom) { pn_u.name.atom = atom; }
||||||| merged common ancestors
    void setAtom(JSAtom* atom) {
        pn_u.name.atom = atom;
    }
=======
inline bool ParseNode::isName(PropertyName* name) const {
  return getKind() == ParseNodeKind::Name && as<NameNode>().name() == name;
}
>>>>>>> upstream-releases

<<<<<<< HEAD
  void setInitializer(ParseNode* init) { pn_u.name.initOrStmt = init; }
||||||| merged common ancestors
    void setInitializer(ParseNode* init) {
        pn_u.name.initOrStmt = init;
    }
=======
class UnaryNode : public ParseNode {
  ParseNode* kid_;
  bool prologue; /* directive prologue member */
>>>>>>> upstream-releases

<<<<<<< HEAD
  // Methods used by FoldConstants.cpp.
  ParseNode** unsafeInitializerReference() { return &pn_u.name.initOrStmt; }
};
||||||| merged common ancestors
    // Methods used by FoldConstants.cpp.
    ParseNode** unsafeInitializerReference() {
        return &pn_u.name.initOrStmt;
    }
};
=======
 public:
  UnaryNode(ParseNodeKind kind, const TokenPos& pos, ParseNode* kid)
      : ParseNode(kind, pos), kid_(kid), prologue(false) {
    MOZ_ASSERT(is<UnaryNode>());
  }
>>>>>>> upstream-releases

<<<<<<< HEAD
inline bool ParseNode::isName(PropertyName* name) const {
  return getKind() == ParseNodeKind::Name && as<NameNode>().name() == name;
}
||||||| merged common ancestors
inline bool
ParseNode::isName(PropertyName* name) const
{
    return getKind() == ParseNodeKind::Name && as<NameNode>().name() == name;
}
=======
  static bool test(const ParseNode& node) {
    return node.typeCode() == TypeCode::Unary;
  }
>>>>>>> upstream-releases

<<<<<<< HEAD
class UnaryNode : public ParseNode {
 public:
  UnaryNode(ParseNodeKind kind, const TokenPos& pos, ParseNode* kid)
      : ParseNode(kind, JSOP_NOP, pos) {
    pn_u.unary.kid = kid;
    MOZ_ASSERT(is<UnaryNode>());
  }
||||||| merged common ancestors
class UnaryNode : public ParseNode
{
  public:
    UnaryNode(ParseNodeKind kind, const TokenPos& pos, ParseNode* kid)
      : ParseNode(kind, JSOP_NOP, PN_UNARY, pos)
    {
        pn_u.unary.kid = kid;
    }
=======
  static constexpr TypeCode classTypeCode() { return TypeCode::Unary; }
>>>>>>> upstream-releases

<<<<<<< HEAD
  static bool test(const ParseNode& node) { return node.isArity(PN_UNARY); }
||||||| merged common ancestors
    static bool test(const ParseNode& node) {
        return node.isArity(PN_UNARY);
    }
=======
  template <typename Visitor>
  bool accept(Visitor& visitor) {
    if (kid_) {
      if (!visitor.visit(kid_)) {
        return false;
      }
    }
    return true;
  }
>>>>>>> upstream-releases

#ifdef DEBUG
<<<<<<< HEAD
  void dump(GenericPrinter& out, int indent);
||||||| merged common ancestors
    void dump(GenericPrinter& out, int indent);
=======
  void dumpImpl(GenericPrinter& out, int indent);
>>>>>>> upstream-releases
#endif

<<<<<<< HEAD
  ParseNode* kid() const { return pn_u.unary.kid; }

  /* Return true if this node appears in a Directive Prologue. */
  bool isDirectivePrologueMember() const { return pn_u.unary.prologue; }

  void setIsDirectivePrologueMember() { pn_u.unary.prologue = true; }

  /*
   * Non-null if this is a statement node which could be a member of a
   * Directive Prologue: an expression statement consisting of a single
   * string literal.
   *
   * This considers only the node and its children, not its context. After
   * parsing, check the node's prologue flag to see if it is indeed part of
   * a directive prologue.
   *
   * Note that a Directive Prologue can contain statements that cannot
   * themselves be directives (string literals that include escape sequences
   * or escaped newlines, say). This member function returns true for such
   * nodes; we use it to determine the extent of the prologue.
   */
  JSAtom* isStringExprStatement() const {
    if (isKind(ParseNodeKind::ExpressionStatement)) {
      if (kid()->isKind(ParseNodeKind::String) && !kid()->isInParens()) {
        return kid()->as<NameNode>().atom();
      }
    }
    return nullptr;
  }
||||||| merged common ancestors
    ParseNode* kid() const {
        return pn_u.unary.kid;
    }

    /* Return true if this node appears in a Directive Prologue. */
    bool isDirectivePrologueMember() const {
        return pn_u.unary.prologue;
    }

    void setIsDirectivePrologueMember() {
        pn_u.unary.prologue = true;
    }

    /*
     * Non-null if this is a statement node which could be a member of a
     * Directive Prologue: an expression statement consisting of a single
     * string literal.
     *
     * This considers only the node and its children, not its context. After
     * parsing, check the node's prologue flag to see if it is indeed part of
     * a directive prologue.
     *
     * Note that a Directive Prologue can contain statements that cannot
     * themselves be directives (string literals that include escape sequences
     * or escaped newlines, say). This member function returns true for such
     * nodes; we use it to determine the extent of the prologue.
     */
    JSAtom* isStringExprStatement() const {
        if (isKind(ParseNodeKind::ExpressionStatement)) {
            if (kid()->isKind(ParseNodeKind::String) && !kid()->isInParens()) {
                return kid()->as<NameNode>().atom();
            }
        }
        return nullptr;
    }
=======
  ParseNode* kid() const { return kid_; }

  /* Return true if this node appears in a Directive Prologue. */
  bool isDirectivePrologueMember() const { return prologue; }

  void setIsDirectivePrologueMember() { prologue = true; }

  /*
   * Non-null if this is a statement node which could be a member of a
   * Directive Prologue: an expression statement consisting of a single
   * string literal.
   *
   * This considers only the node and its children, not its context. After
   * parsing, check the node's prologue flag to see if it is indeed part of
   * a directive prologue.
   *
   * Note that a Directive Prologue can contain statements that cannot
   * themselves be directives (string literals that include escape sequences
   * or escaped newlines, say). This member function returns true for such
   * nodes; we use it to determine the extent of the prologue.
   */
  JSAtom* isStringExprStatement() const {
    if (isKind(ParseNodeKind::ExpressionStmt)) {
      if (kid()->isKind(ParseNodeKind::StringExpr) && !kid()->isInParens()) {
        return kid()->as<NameNode>().atom();
      }
    }
    return nullptr;
  }

  // Methods used by FoldConstants.cpp.
  ParseNode** unsafeKidReference() { return &kid_; }
};

class BinaryNode : public ParseNode {
  ParseNode* left_;
  ParseNode* right_;

 public:
  BinaryNode(ParseNodeKind kind, const TokenPos& pos, ParseNode* left,
             ParseNode* right)
      : ParseNode(kind, pos), left_(left), right_(right) {
    MOZ_ASSERT(is<BinaryNode>());
  }
>>>>>>> upstream-releases

<<<<<<< HEAD
  // Methods used by FoldConstants.cpp.
  ParseNode** unsafeKidReference() { return &pn_u.unary.kid; }
};
||||||| merged common ancestors
    // Methods used by FoldConstants.cpp.
    ParseNode** unsafeKidReference() {
        return &pn_u.unary.kid;
    }
};
=======
  BinaryNode(ParseNodeKind kind, ParseNode* left, ParseNode* right)
      : ParseNode(kind, TokenPos::box(left->pn_pos, right->pn_pos)),
        left_(left),
        right_(right) {
    MOZ_ASSERT(is<BinaryNode>());
  }
>>>>>>> upstream-releases

<<<<<<< HEAD
class BinaryNode : public ParseNode {
 public:
  BinaryNode(ParseNodeKind kind, JSOp op, const TokenPos& pos, ParseNode* left,
             ParseNode* right)
      : ParseNode(kind, op, pos) {
    pn_u.binary.left = left;
    pn_u.binary.right = right;
    MOZ_ASSERT(is<BinaryNode>());
  }
||||||| merged common ancestors
class BinaryNode : public ParseNode
{
  public:
    BinaryNode(ParseNodeKind kind, JSOp op, const TokenPos& pos, ParseNode* left, ParseNode* right)
      : ParseNode(kind, op, PN_BINARY, pos)
    {
        pn_u.binary.left = left;
        pn_u.binary.right = right;
    }
=======
  static bool test(const ParseNode& node) {
    return node.typeCode() == TypeCode::Binary;
  }
>>>>>>> upstream-releases

<<<<<<< HEAD
  BinaryNode(ParseNodeKind kind, JSOp op, ParseNode* left, ParseNode* right)
      : ParseNode(kind, op, TokenPos::box(left->pn_pos, right->pn_pos)) {
    pn_u.binary.left = left;
    pn_u.binary.right = right;
    MOZ_ASSERT(is<BinaryNode>());
  }
||||||| merged common ancestors
    BinaryNode(ParseNodeKind kind, JSOp op, ParseNode* left, ParseNode* right)
      : ParseNode(kind, op, PN_BINARY, TokenPos::box(left->pn_pos, right->pn_pos))
    {
        pn_u.binary.left = left;
        pn_u.binary.right = right;
    }
=======
  static constexpr TypeCode classTypeCode() { return TypeCode::Binary; }
>>>>>>> upstream-releases

<<<<<<< HEAD
  static bool test(const ParseNode& node) { return node.isArity(PN_BINARY); }
||||||| merged common ancestors
    static bool test(const ParseNode& node) {
        return node.isArity(PN_BINARY);
    }
=======
  template <typename Visitor>
  bool accept(Visitor& visitor) {
    if (left_) {
      if (!visitor.visit(left_)) {
        return false;
      }
    }
    if (right_) {
      if (!visitor.visit(right_)) {
        return false;
      }
    }
    return true;
  }
>>>>>>> upstream-releases

#ifdef DEBUG
<<<<<<< HEAD
  void dump(GenericPrinter& out, int indent);
||||||| merged common ancestors
    void dump(GenericPrinter& out, int indent);
=======
  void dumpImpl(GenericPrinter& out, int indent);
>>>>>>> upstream-releases
#endif

<<<<<<< HEAD
  ParseNode* left() const { return pn_u.binary.left; }
||||||| merged common ancestors
    ParseNode* left() const {
        return pn_u.binary.left;
    }
=======
  ParseNode* left() const { return left_; }
>>>>>>> upstream-releases

<<<<<<< HEAD
  ParseNode* right() const { return pn_u.binary.right; }
||||||| merged common ancestors
    ParseNode* right() const {
        return pn_u.binary.right;
    }
=======
  ParseNode* right() const { return right_; }
>>>>>>> upstream-releases

<<<<<<< HEAD
  // Methods used by FoldConstants.cpp.
  // callers are responsible for keeping the list consistent.
  ParseNode** unsafeLeftReference() { return &pn_u.binary.left; }
||||||| merged common ancestors
    // Methods used by FoldConstants.cpp.
    // caller are responsible for keeping the list consistent.
    ParseNode** unsafeLeftReference() {
        return &pn_u.binary.left;
    }
=======
  // Methods used by FoldConstants.cpp.
  // callers are responsible for keeping the list consistent.
  ParseNode** unsafeLeftReference() { return &left_; }
>>>>>>> upstream-releases

<<<<<<< HEAD
  ParseNode** unsafeRightReference() { return &pn_u.binary.right; }
||||||| merged common ancestors
    ParseNode** unsafeRightReference() {
        return &pn_u.binary.right;
    }
=======
  ParseNode** unsafeRightReference() { return &right_; }
>>>>>>> upstream-releases
};

<<<<<<< HEAD
class AssignmentNode : public BinaryNode {
 public:
  AssignmentNode(ParseNodeKind kind, JSOp op, ParseNode* left, ParseNode* right)
      : BinaryNode(kind, op, TokenPos(left->pn_pos.begin, right->pn_pos.end),
                   left, right) {}

  static bool test(const ParseNode& node) {
    ParseNodeKind kind = node.getKind();
    bool match = ParseNodeKind::AssignmentStart <= kind &&
                 kind <= ParseNodeKind::AssignmentLast;
    MOZ_ASSERT_IF(match, node.is<BinaryNode>());
    return match;
  }
||||||| merged common ancestors
class AssignmentNode : public BinaryNode
{
  public:
    AssignmentNode(ParseNodeKind kind, JSOp op, ParseNode* left, ParseNode* right)
      : BinaryNode(kind, op, TokenPos(left->pn_pos.begin, right->pn_pos.end), left, right)
    {}

    static bool test(const ParseNode& node) {
        ParseNodeKind kind = node.getKind();
        bool match = ParseNodeKind::AssignmentStart <= kind &&
                     kind <= ParseNodeKind::AssignmentLast;
        MOZ_ASSERT_IF(match, node.is<BinaryNode>());
        return match;
    }
=======
class AssignmentNode : public BinaryNode {
 public:
  AssignmentNode(ParseNodeKind kind, ParseNode* left, ParseNode* right)
      : BinaryNode(kind, TokenPos(left->pn_pos.begin, right->pn_pos.end), left,
                   right) {}

  static bool test(const ParseNode& node) {
    ParseNodeKind kind = node.getKind();
    bool match = ParseNodeKind::AssignmentStart <= kind &&
                 kind <= ParseNodeKind::AssignmentLast;
    MOZ_ASSERT_IF(match, node.is<BinaryNode>());
    return match;
  }
>>>>>>> upstream-releases
};

<<<<<<< HEAD
class ForNode : public BinaryNode {
 public:
  ForNode(const TokenPos& pos, ParseNode* forHead, ParseNode* body,
          unsigned iflags)
      : BinaryNode(ParseNodeKind::For,
                   forHead->isKind(ParseNodeKind::ForIn) ? JSOP_ITER : JSOP_NOP,
                   pos, forHead, body) {
    MOZ_ASSERT(forHead->isKind(ParseNodeKind::ForIn) ||
               forHead->isKind(ParseNodeKind::ForOf) ||
               forHead->isKind(ParseNodeKind::ForHead));
    pn_u.binary.iflags = iflags;
  }
||||||| merged common ancestors
class ForNode : public BinaryNode
{
  public:
    ForNode(const TokenPos& pos, ParseNode* forHead, ParseNode* body, unsigned iflags)
      : BinaryNode(ParseNodeKind::For,
                   forHead->isKind(ParseNodeKind::ForIn) ? JSOP_ITER : JSOP_NOP,
                   pos, forHead, body)
    {
        MOZ_ASSERT(forHead->isKind(ParseNodeKind::ForIn) ||
                   forHead->isKind(ParseNodeKind::ForOf) ||
                   forHead->isKind(ParseNodeKind::ForHead));
        pn_u.binary.iflags = iflags;
    }
=======
class ForNode : public BinaryNode {
  unsigned iflags_; /* JSITER_* flags */

 public:
  ForNode(const TokenPos& pos, ParseNode* forHead, ParseNode* body,
          unsigned iflags)
      : BinaryNode(ParseNodeKind::ForStmt, pos, forHead, body),
        iflags_(iflags) {
    MOZ_ASSERT(forHead->isKind(ParseNodeKind::ForIn) ||
               forHead->isKind(ParseNodeKind::ForOf) ||
               forHead->isKind(ParseNodeKind::ForHead));
  }
>>>>>>> upstream-releases

<<<<<<< HEAD
  static bool test(const ParseNode& node) {
    bool match = node.isKind(ParseNodeKind::For);
    MOZ_ASSERT_IF(match, node.is<BinaryNode>());
    return match;
  }
||||||| merged common ancestors
    static bool test(const ParseNode& node) {
        bool match = node.isKind(ParseNodeKind::For);
        MOZ_ASSERT_IF(match, node.is<BinaryNode>());
        return match;
    }
=======
  static bool test(const ParseNode& node) {
    bool match = node.isKind(ParseNodeKind::ForStmt);
    MOZ_ASSERT_IF(match, node.is<BinaryNode>());
    return match;
  }
>>>>>>> upstream-releases

  TernaryNode* head() const { return &left()->as<TernaryNode>(); }

  ParseNode* body() const { return right(); }

<<<<<<< HEAD
  unsigned iflags() const { return pn_u.binary.iflags; }
||||||| merged common ancestors
    unsigned iflags() const {
        return pn_u.binary.iflags;
    }
=======
  unsigned iflags() const { return iflags_; }
>>>>>>> upstream-releases
};

<<<<<<< HEAD
class TernaryNode : public ParseNode {
 public:
  TernaryNode(ParseNodeKind kind, ParseNode* kid1, ParseNode* kid2,
              ParseNode* kid3)
||||||| merged common ancestors
class TernaryNode : public ParseNode
{
  public:
    TernaryNode(ParseNodeKind kind, ParseNode* kid1, ParseNode* kid2, ParseNode* kid3)
=======
class TernaryNode : public ParseNode {
  ParseNode* kid1_; /* condition, discriminant, etc. */
  ParseNode* kid2_; /* then-part, case list, etc. */
  ParseNode* kid3_; /* else-part, default case, etc. */

 public:
  TernaryNode(ParseNodeKind kind, ParseNode* kid1, ParseNode* kid2,
              ParseNode* kid3)
>>>>>>> upstream-releases
      : TernaryNode(kind, kid1, kid2, kid3,
                    TokenPos((kid1 ? kid1 : kid2 ? kid2 : kid3)->pn_pos.begin,
<<<<<<< HEAD
                             (kid3 ? kid3 : kid2 ? kid2 : kid1)->pn_pos.end)) {}

  TernaryNode(ParseNodeKind kind, ParseNode* kid1, ParseNode* kid2,
              ParseNode* kid3, const TokenPos& pos)
      : ParseNode(kind, JSOP_NOP, pos) {
    pn_u.ternary.kid1 = kid1;
    pn_u.ternary.kid2 = kid2;
    pn_u.ternary.kid3 = kid3;
    MOZ_ASSERT(is<TernaryNode>());
  }
||||||| merged common ancestors
                             (kid3 ? kid3 : kid2 ? kid2 : kid1)->pn_pos.end))
    {}

    TernaryNode(ParseNodeKind kind, ParseNode* kid1, ParseNode* kid2, ParseNode* kid3,
                const TokenPos& pos)
      : ParseNode(kind, JSOP_NOP, PN_TERNARY, pos)
    {
        pn_u.ternary.kid1 = kid1;
        pn_u.ternary.kid2 = kid2;
        pn_u.ternary.kid3 = kid3;
    }
=======
                             (kid3 ? kid3 : kid2 ? kid2 : kid1)->pn_pos.end)) {}

  TernaryNode(ParseNodeKind kind, ParseNode* kid1, ParseNode* kid2,
              ParseNode* kid3, const TokenPos& pos)
      : ParseNode(kind, pos), kid1_(kid1), kid2_(kid2), kid3_(kid3) {
    MOZ_ASSERT(is<TernaryNode>());
  }
>>>>>>> upstream-releases

<<<<<<< HEAD
  static bool test(const ParseNode& node) { return node.isArity(PN_TERNARY); }
||||||| merged common ancestors
    static bool test(const ParseNode& node) {
        return node.isArity(PN_TERNARY);
    }
=======
  static bool test(const ParseNode& node) {
    return node.typeCode() == TypeCode::Ternary;
  }

  static constexpr TypeCode classTypeCode() { return TypeCode::Ternary; }

  template <typename Visitor>
  bool accept(Visitor& visitor) {
    if (kid1_) {
      if (!visitor.visit(kid1_)) {
        return false;
      }
    }
    if (kid2_) {
      if (!visitor.visit(kid2_)) {
        return false;
      }
    }
    if (kid3_) {
      if (!visitor.visit(kid3_)) {
        return false;
      }
    }
    return true;
  }
>>>>>>> upstream-releases

#ifdef DEBUG
<<<<<<< HEAD
  void dump(GenericPrinter& out, int indent);
||||||| merged common ancestors
    void dump(GenericPrinter& out, int indent);
=======
  void dumpImpl(GenericPrinter& out, int indent);
>>>>>>> upstream-releases
#endif

<<<<<<< HEAD
  ParseNode* kid1() const { return pn_u.ternary.kid1; }
||||||| merged common ancestors
    ParseNode* kid1() const {
        return pn_u.ternary.kid1;
    }
=======
  ParseNode* kid1() const { return kid1_; }
>>>>>>> upstream-releases

<<<<<<< HEAD
  ParseNode* kid2() const { return pn_u.ternary.kid2; }
||||||| merged common ancestors
    ParseNode* kid2() const {
        return pn_u.ternary.kid2;
    }
=======
  ParseNode* kid2() const { return kid2_; }
>>>>>>> upstream-releases

<<<<<<< HEAD
  ParseNode* kid3() const { return pn_u.ternary.kid3; }
||||||| merged common ancestors
    ParseNode* kid3() const {
        return pn_u.ternary.kid3;
    }
=======
  ParseNode* kid3() const { return kid3_; }
>>>>>>> upstream-releases

<<<<<<< HEAD
  // Methods used by FoldConstants.cpp.
  ParseNode** unsafeKid1Reference() { return &pn_u.ternary.kid1; }
||||||| merged common ancestors
    // Methods used by FoldConstants.cpp.
    ParseNode** unsafeKid1Reference() {
        return &pn_u.ternary.kid1;
    }
=======
  // Methods used by FoldConstants.cpp.
  ParseNode** unsafeKid1Reference() { return &kid1_; }
>>>>>>> upstream-releases

<<<<<<< HEAD
  ParseNode** unsafeKid2Reference() { return &pn_u.ternary.kid2; }
||||||| merged common ancestors
    ParseNode** unsafeKid2Reference() {
        return &pn_u.ternary.kid2;
    }
=======
  ParseNode** unsafeKid2Reference() { return &kid2_; }
>>>>>>> upstream-releases

<<<<<<< HEAD
  ParseNode** unsafeKid3Reference() { return &pn_u.ternary.kid3; }
||||||| merged common ancestors
    ParseNode** unsafeKid3Reference() {
        return &pn_u.ternary.kid3;
    }
=======
  ParseNode** unsafeKid3Reference() { return &kid3_; }
>>>>>>> upstream-releases
};

<<<<<<< HEAD
class ListNode : public ParseNode {
 private:
  // xflags bits.

  // Statement list has top-level function statements.
  static constexpr uint32_t hasTopLevelFunctionDeclarationsBit = 0x01;

  // One or more of
  //   * array has holes
  //   * array has spread node
  static constexpr uint32_t hasArrayHoleOrSpreadBit = 0x02;

  // Array/Object/Class initializer has non-constants.
  //   * array has holes
  //   * array has spread node
  //   * array has element which is known not to be constant
  //   * array has no element
  //   * object/class has __proto__
  //   * object/class has property which is known not to be constant
  //   * object/class shorthand property
  //   * object/class spread property
  //   * object/class has method
  //   * object/class has computed property
  static constexpr uint32_t hasNonConstInitializerBit = 0x04;

  void checkConsistency() const
#ifndef DEBUG
  {
  }
#endif
  ;
||||||| merged common ancestors
class ListNode : public ParseNode
{
  private:
    // xflags bits.

    // Statement list has top-level function statements.
    static constexpr uint32_t hasTopLevelFunctionDeclarationsBit = 0x01;

    // One or more of
    //   * array has holes
    //   * array has spread node
    static constexpr uint32_t hasArrayHoleOrSpreadBit = 0x02;

    // Array/Object/Class initializer has non-constants.
    //   * array has holes
    //   * array has spread node
    //   * array has element which is known not to be constant
    //   * array has no element
    //   * object/class has __proto__
    //   * object/class has property which is known not to be constant
    //   * object/class shorthand property
    //   * object/class spread property
    //   * object/class has method
    //   * object/class has computed property
    static constexpr uint32_t hasNonConstInitializerBit = 0x04;

    void checkConsistency() const
#ifndef DEBUG
    {}
#endif
    ;
=======
class ListNode : public ParseNode {
  ParseNode* head_;  /* first node in list */
  ParseNode** tail_; /* ptr to last node's pn_next in list */
  uint32_t count_;   /* number of nodes in list */
  uint32_t xflags;

 private:
  // xflags bits.

  // Statement list has top-level function statements.
  static constexpr uint32_t hasTopLevelFunctionDeclarationsBit = 0x01;

  // One or more of
  //   * array has holes
  //   * array has spread node
  static constexpr uint32_t hasArrayHoleOrSpreadBit = 0x02;

  // Array/Object/Class initializer has non-constants.
  //   * array has holes
  //   * array has spread node
  //   * array has element which is known not to be constant
  //   * array has no element
  //   * object/class has __proto__
  //   * object/class has property which is known not to be constant
  //   * object/class shorthand property
  //   * object/class spread property
  //   * object/class has method
  //   * object/class has computed property
  static constexpr uint32_t hasNonConstInitializerBit = 0x04;

  // Flag set by the emitter after emitting top-level function statements.
  static constexpr uint32_t emittedTopLevelFunctionDeclarationsBit = 0x08;

 public:
  ListNode(ParseNodeKind kind, const TokenPos& pos) : ParseNode(kind, pos) {
    makeEmpty();
    MOZ_ASSERT(is<ListNode>());
  }
>>>>>>> upstream-releases

<<<<<<< HEAD
 public:
  ListNode(ParseNodeKind kind, const TokenPos& pos)
      : ParseNode(kind, JSOP_NOP, pos) {
    makeEmpty();
    MOZ_ASSERT(is<ListNode>());
  }
||||||| merged common ancestors
  public:
    ListNode(ParseNodeKind kind, const TokenPos& pos)
      : ParseNode(kind, JSOP_NOP, PN_LIST, pos)
    {
        makeEmpty();
    }
=======
  ListNode(ParseNodeKind kind, ParseNode* kid)
      : ParseNode(kind, kid->pn_pos),
        head_(kid),
        tail_(&kid->pn_next),
        count_(1),
        xflags(0) {
    if (kid->pn_pos.begin < pn_pos.begin) {
      pn_pos.begin = kid->pn_pos.begin;
    }
    pn_pos.end = kid->pn_pos.end;
>>>>>>> upstream-releases

<<<<<<< HEAD
  ListNode(ParseNodeKind kind, JSOp op, const TokenPos& pos)
      : ParseNode(kind, op, pos) {
    makeEmpty();
    MOZ_ASSERT(is<ListNode>());
  }
||||||| merged common ancestors
    ListNode(ParseNodeKind kind, JSOp op, const TokenPos& pos)
      : ParseNode(kind, op, PN_LIST, pos)
    {
        makeEmpty();
    }
=======
    MOZ_ASSERT(is<ListNode>());
  }
>>>>>>> upstream-releases

<<<<<<< HEAD
  ListNode(ParseNodeKind kind, JSOp op, ParseNode* kid)
      : ParseNode(kind, op, kid->pn_pos) {
    if (kid->pn_pos.begin < pn_pos.begin) {
      pn_pos.begin = kid->pn_pos.begin;
    }
    pn_pos.end = kid->pn_pos.end;
||||||| merged common ancestors
    ListNode(ParseNodeKind kind, JSOp op, ParseNode* kid)
      : ParseNode(kind, op, PN_LIST, kid->pn_pos)
    {
        if (kid->pn_pos.begin < pn_pos.begin) {
            pn_pos.begin = kid->pn_pos.begin;
        }
        pn_pos.end = kid->pn_pos.end;

        pn_u.list.head = kid;
        pn_u.list.tail = &kid->pn_next;
        pn_u.list.count = 1;
        pn_u.list.xflags = 0;
    }
=======
  static bool test(const ParseNode& node) {
    return node.typeCode() == TypeCode::List;
  }

  static constexpr TypeCode classTypeCode() { return TypeCode::List; }
>>>>>>> upstream-releases

<<<<<<< HEAD
    pn_u.list.head = kid;
    pn_u.list.tail = &kid->pn_next;
    pn_u.list.count = 1;
    pn_u.list.xflags = 0;
    MOZ_ASSERT(is<ListNode>());
  }

  static bool test(const ParseNode& node) { return node.isArity(PN_LIST); }
||||||| merged common ancestors
    static bool test(const ParseNode& node) {
        return node.isArity(PN_LIST);
    }
=======
  template <typename Visitor>
  bool accept(Visitor& visitor) {
    ParseNode** listp = &head_;
    for (; *listp; listp = &(*listp)->pn_next) {
      // Don't use PN*& because we want to check if it changed, so we can use
      // ReplaceNode
      ParseNode* pn = *listp;
      if (!visitor.visit(pn)) {
        return false;
      }
      if (pn != *listp) {
        ReplaceNode(listp, pn);
      }
    }
    unsafeReplaceTail(listp);
    return true;
  }
>>>>>>> upstream-releases

#ifdef DEBUG
<<<<<<< HEAD
  void dump(GenericPrinter& out, int indent);
||||||| merged common ancestors
    void dump(GenericPrinter& out, int indent);
=======
  void dumpImpl(GenericPrinter& out, int indent);
>>>>>>> upstream-releases
#endif

<<<<<<< HEAD
  ParseNode* head() const { return pn_u.list.head; }
||||||| merged common ancestors
    ParseNode* head() const {
        return pn_u.list.head;
    }
=======
  ParseNode* head() const { return head_; }
>>>>>>> upstream-releases

<<<<<<< HEAD
  ParseNode** tail() const { return pn_u.list.tail; }
||||||| merged common ancestors
    ParseNode** tail() const {
        return pn_u.list.tail;
    }
=======
  ParseNode** tail() const { return tail_; }
>>>>>>> upstream-releases

<<<<<<< HEAD
  uint32_t count() const { return pn_u.list.count; }
||||||| merged common ancestors
    uint32_t count() const {
        return pn_u.list.count;
    }
=======
  uint32_t count() const { return count_; }
>>>>>>> upstream-releases

  bool empty() const { return count() == 0; }

<<<<<<< HEAD
  MOZ_MUST_USE bool hasTopLevelFunctionDeclarations() const {
    MOZ_ASSERT(isKind(ParseNodeKind::StatementList));
    return pn_u.list.xflags & hasTopLevelFunctionDeclarationsBit;
  }
||||||| merged common ancestors
    MOZ_MUST_USE bool hasTopLevelFunctionDeclarations() const {
        MOZ_ASSERT(isKind(ParseNodeKind::StatementList));
        return pn_u.list.xflags & hasTopLevelFunctionDeclarationsBit;
    }
=======
  void checkConsistency() const
#ifndef DEBUG
  {
  }
#endif
  ;
>>>>>>> upstream-releases

<<<<<<< HEAD
  MOZ_MUST_USE bool hasArrayHoleOrSpread() const {
    MOZ_ASSERT(isKind(ParseNodeKind::Array));
    return pn_u.list.xflags & hasArrayHoleOrSpreadBit;
  }
||||||| merged common ancestors
    MOZ_MUST_USE bool hasArrayHoleOrSpread() const {
        MOZ_ASSERT(isKind(ParseNodeKind::Array));
        return pn_u.list.xflags & hasArrayHoleOrSpreadBit;
    }
=======
  MOZ_MUST_USE bool hasTopLevelFunctionDeclarations() const {
    MOZ_ASSERT(isKind(ParseNodeKind::StatementList));
    return xflags & hasTopLevelFunctionDeclarationsBit;
  }
>>>>>>> upstream-releases

<<<<<<< HEAD
  MOZ_MUST_USE bool hasNonConstInitializer() const {
    MOZ_ASSERT(isKind(ParseNodeKind::Array) || isKind(ParseNodeKind::Object) ||
               isKind(ParseNodeKind::ClassMemberList));
    return pn_u.list.xflags & hasNonConstInitializerBit;
  }
||||||| merged common ancestors
    MOZ_MUST_USE bool hasNonConstInitializer() const {
        MOZ_ASSERT(isKind(ParseNodeKind::Array) ||
                   isKind(ParseNodeKind::Object) ||
                   isKind(ParseNodeKind::ClassMethodList));
        return pn_u.list.xflags & hasNonConstInitializerBit;
    }
=======
  MOZ_MUST_USE bool emittedTopLevelFunctionDeclarations() const {
    MOZ_ASSERT(isKind(ParseNodeKind::StatementList));
    MOZ_ASSERT(hasTopLevelFunctionDeclarations());
    return xflags & emittedTopLevelFunctionDeclarationsBit;
  }
>>>>>>> upstream-releases

<<<<<<< HEAD
  void setHasTopLevelFunctionDeclarations() {
    MOZ_ASSERT(isKind(ParseNodeKind::StatementList));
    pn_u.list.xflags |= hasTopLevelFunctionDeclarationsBit;
  }
||||||| merged common ancestors
    void setHasTopLevelFunctionDeclarations() {
        MOZ_ASSERT(isKind(ParseNodeKind::StatementList));
        pn_u.list.xflags |= hasTopLevelFunctionDeclarationsBit;
    }
=======
  MOZ_MUST_USE bool hasArrayHoleOrSpread() const {
    MOZ_ASSERT(isKind(ParseNodeKind::ArrayExpr));
    return xflags & hasArrayHoleOrSpreadBit;
  }
>>>>>>> upstream-releases

<<<<<<< HEAD
  void setHasArrayHoleOrSpread() {
    MOZ_ASSERT(isKind(ParseNodeKind::Array));
    pn_u.list.xflags |= hasArrayHoleOrSpreadBit;
  }
||||||| merged common ancestors
    void setHasArrayHoleOrSpread() {
        MOZ_ASSERT(isKind(ParseNodeKind::Array));
        pn_u.list.xflags |= hasArrayHoleOrSpreadBit;
    }
=======
  MOZ_MUST_USE bool hasNonConstInitializer() const {
    MOZ_ASSERT(isKind(ParseNodeKind::ArrayExpr) ||
               isKind(ParseNodeKind::ObjectExpr));
    return xflags & hasNonConstInitializerBit;
  }
>>>>>>> upstream-releases

<<<<<<< HEAD
  void setHasNonConstInitializer() {
    MOZ_ASSERT(isKind(ParseNodeKind::Array) || isKind(ParseNodeKind::Object) ||
               isKind(ParseNodeKind::ClassMemberList));
    pn_u.list.xflags |= hasNonConstInitializerBit;
  }
||||||| merged common ancestors
    void setHasNonConstInitializer() {
        MOZ_ASSERT(isKind(ParseNodeKind::Array) ||
                   isKind(ParseNodeKind::Object) ||
                   isKind(ParseNodeKind::ClassMethodList));
        pn_u.list.xflags |= hasNonConstInitializerBit;
    }
=======
  void setHasTopLevelFunctionDeclarations() {
    MOZ_ASSERT(isKind(ParseNodeKind::StatementList));
    xflags |= hasTopLevelFunctionDeclarationsBit;
  }
>>>>>>> upstream-releases

<<<<<<< HEAD
  /*
   * Compute a pointer to the last element in a singly-linked list. NB: list
   * must be non-empty -- this is asserted!
   */
  ParseNode* last() const {
    MOZ_ASSERT(!empty());
    //
    // ParseNode                      ParseNode
    // +-----+---------+-----+        +-----+---------+-----+
    // | ... | pn_next | ... | +-...->| ... | pn_next | ... |
    // +-----+---------+-----+ |      +-----+---------+-----+
    // ^       |               |      ^     ^
    // |       +---------------+      |     |
    // |                              |     tail()
    // |                              |
    // head()                         last()
    //
    return (ParseNode*)(uintptr_t(tail()) - offsetof(ParseNode, pn_next));
  }
||||||| merged common ancestors
    /*
     * Compute a pointer to the last element in a singly-linked list. NB: list
     * must be non-empty -- this is asserted!
     */
    ParseNode* last() const {
        MOZ_ASSERT(!empty());
        //
        // ParseNode                      ParseNode
        // +-----+---------+-----+        +-----+---------+-----+
        // | ... | pn_next | ... | +-...->| ... | pn_next | ... |
        // +-----+---------+-----+ |      +-----+---------+-----+
        // ^       |               |      ^     ^
        // |       +---------------+      |     |
        // |                              |     tail()
        // |                              |
        // head()                         last()
        //
        return (ParseNode*)(uintptr_t(tail()) - offsetof(ParseNode, pn_next));
    }
=======
  void setEmittedTopLevelFunctionDeclarations() {
    MOZ_ASSERT(isKind(ParseNodeKind::StatementList));
    MOZ_ASSERT(hasTopLevelFunctionDeclarations());
    xflags |= emittedTopLevelFunctionDeclarationsBit;
  }
>>>>>>> upstream-releases

<<<<<<< HEAD
  void replaceLast(ParseNode* node) {
    MOZ_ASSERT(!empty());
    pn_pos.end = node->pn_pos.end;

    ParseNode* item = head();
    ParseNode* lastNode = last();
    MOZ_ASSERT(item);
    if (item == lastNode) {
      pn_u.list.head = node;
    } else {
      while (item->pn_next != lastNode) {
        MOZ_ASSERT(item->pn_next);
        item = item->pn_next;
      }
      item->pn_next = node;
    }
    pn_u.list.tail = &node->pn_next;
  }
||||||| merged common ancestors
    void replaceLast(ParseNode* node) {
        MOZ_ASSERT(!empty());
        pn_pos.end = node->pn_pos.end;

        ParseNode* item = head();
        ParseNode* lastNode = last();
        MOZ_ASSERT(item);
        if (item == lastNode) {
            pn_u.list.head = node;
        } else {
            while (item->pn_next != lastNode) {
                MOZ_ASSERT(item->pn_next);
                item = item->pn_next;
            }
            item->pn_next = node;
        }
        pn_u.list.tail = &node->pn_next;
    }
=======
  void setHasArrayHoleOrSpread() {
    MOZ_ASSERT(isKind(ParseNodeKind::ArrayExpr));
    xflags |= hasArrayHoleOrSpreadBit;
  }
>>>>>>> upstream-releases

<<<<<<< HEAD
  void makeEmpty() {
    pn_u.list.head = nullptr;
    pn_u.list.tail = &pn_u.list.head;
    pn_u.list.count = 0;
    pn_u.list.xflags = 0;
  }
||||||| merged common ancestors
    void makeEmpty() {
        pn_u.list.head = nullptr;
        pn_u.list.tail = &pn_u.list.head;
        pn_u.list.count = 0;
        pn_u.list.xflags = 0;
    }
=======
  void setHasNonConstInitializer() {
    MOZ_ASSERT(isKind(ParseNodeKind::ArrayExpr) ||
               isKind(ParseNodeKind::ObjectExpr));
    xflags |= hasNonConstInitializerBit;
  }
>>>>>>> upstream-releases

<<<<<<< HEAD
  void append(ParseNode* item) {
    MOZ_ASSERT(item->pn_pos.begin >= pn_pos.begin);
    appendWithoutOrderAssumption(item);
  }
||||||| merged common ancestors
    void append(ParseNode* item) {
        MOZ_ASSERT(item->pn_pos.begin >= pn_pos.begin);
        appendWithoutOrderAssumption(item);
    }
=======
  void unsetHasNonConstInitializer() {
    MOZ_ASSERT(isKind(ParseNodeKind::ArrayExpr) ||
               isKind(ParseNodeKind::ObjectExpr));
    xflags &= ~hasNonConstInitializerBit;
  }
>>>>>>> upstream-releases

<<<<<<< HEAD
  void appendWithoutOrderAssumption(ParseNode* item) {
    pn_pos.end = item->pn_pos.end;
    *pn_u.list.tail = item;
    pn_u.list.tail = &item->pn_next;
    pn_u.list.count++;
  }
||||||| merged common ancestors
    void appendWithoutOrderAssumption(ParseNode* item) {
        pn_pos.end = item->pn_pos.end;
        *pn_u.list.tail = item;
        pn_u.list.tail = &item->pn_next;
        pn_u.list.count++;
    }
=======
  /*
   * Compute a pointer to the last element in a singly-linked list. NB: list
   * must be non-empty -- this is asserted!
   */
  ParseNode* last() const {
    MOZ_ASSERT(!empty());
    //
    // ParseNode                      ParseNode
    // +-----+---------+-----+        +-----+---------+-----+
    // | ... | pn_next | ... | +-...->| ... | pn_next | ... |
    // +-----+---------+-----+ |      +-----+---------+-----+
    // ^       |               |      ^     ^
    // |       +---------------+      |     |
    // |                              |     tail()
    // |                              |
    // head()                         last()
    //
    return (ParseNode*)(uintptr_t(tail()) - offsetof(ParseNode, pn_next));
  }
>>>>>>> upstream-releases

<<<<<<< HEAD
  void prepend(ParseNode* item) {
    item->pn_next = pn_u.list.head;
    pn_u.list.head = item;
    if (pn_u.list.tail == &pn_u.list.head) {
      pn_u.list.tail = &item->pn_next;
||||||| merged common ancestors
    void prepend(ParseNode* item) {
        item->pn_next = pn_u.list.head;
        pn_u.list.head = item;
        if (pn_u.list.tail == &pn_u.list.head) {
            pn_u.list.tail = &item->pn_next;
        }
        pn_u.list.count++;
=======
  void replaceLast(ParseNode* node) {
    MOZ_ASSERT(!empty());
    pn_pos.end = node->pn_pos.end;

    ParseNode* item = head();
    ParseNode* lastNode = last();
    MOZ_ASSERT(item);
    if (item == lastNode) {
      head_ = node;
    } else {
      while (item->pn_next != lastNode) {
        MOZ_ASSERT(item->pn_next);
        item = item->pn_next;
      }
      item->pn_next = node;
>>>>>>> upstream-releases
    }
<<<<<<< HEAD
    pn_u.list.count++;
  }
||||||| merged common ancestors
=======
    tail_ = &node->pn_next;
  }
>>>>>>> upstream-releases

<<<<<<< HEAD
  void prependAndUpdatePos(ParseNode* item) {
    prepend(item);
    pn_pos.begin = item->pn_pos.begin;
  }

  // Methods used by FoldConstants.cpp.
  // Caller is responsible for keeping the list consistent.
  ParseNode** unsafeHeadReference() { return &pn_u.list.head; }

  void unsafeReplaceTail(ParseNode** newTail) {
    pn_u.list.tail = newTail;
    checkConsistency();
  }

  void unsafeDecrementCount() {
    MOZ_ASSERT(count() > 1);
    pn_u.list.count--;
  }
||||||| merged common ancestors
    // Methods used by FoldConstants.cpp.
    // Caller is responsible for keeping the list consistent.
    ParseNode** unsafeHeadReference() {
        return &pn_u.list.head;
    }
=======
  void makeEmpty() {
    head_ = nullptr;
    tail_ = &head_;
    count_ = 0;
    xflags = 0;
  }

  void append(ParseNode* item) {
    MOZ_ASSERT(item->pn_pos.begin >= pn_pos.begin);
    appendWithoutOrderAssumption(item);
  }

  void appendWithoutOrderAssumption(ParseNode* item) {
    pn_pos.end = item->pn_pos.end;
    *tail_ = item;
    tail_ = &item->pn_next;
    count_++;
  }

  void prepend(ParseNode* item) {
    item->pn_next = head_;
    head_ = item;
    if (tail_ == &head_) {
      tail_ = &item->pn_next;
    }
    count_++;
  }

  void prependAndUpdatePos(ParseNode* item) {
    prepend(item);
    pn_pos.begin = item->pn_pos.begin;
  }

  // Methods used by FoldConstants.cpp.
  // Caller is responsible for keeping the list consistent.
  ParseNode** unsafeHeadReference() { return &head_; }
>>>>>>> upstream-releases

<<<<<<< HEAD
 private:
  // Classes to iterate over ListNode contents:
  //
  // Usage:
  //   ListNode* list;
  //   for (ParseNode* item : list->contents()) {
  //     // item is ParseNode* typed.
  //   }
  class iterator {
   private:
    ParseNode* node_;

    friend class ListNode;
    explicit iterator(ParseNode* node) : node_(node) {}

   public:
    bool operator==(const iterator& other) const {
      return node_ == other.node_;
||||||| merged common ancestors
    void unsafeReplaceTail(ParseNode** newTail) {
        pn_u.list.tail = newTail;
        checkConsistency();
=======
  void unsafeReplaceTail(ParseNode** newTail) {
    tail_ = newTail;
    checkConsistency();
  }

  void unsafeDecrementCount() {
    MOZ_ASSERT(count() > 1);
    count_--;
  }

 private:
  // Classes to iterate over ListNode contents:
  //
  // Usage:
  //   ListNode* list;
  //   for (ParseNode* item : list->contents()) {
  //     // item is ParseNode* typed.
  //   }
  class iterator {
   private:
    ParseNode* node_;

    friend class ListNode;
    explicit iterator(ParseNode* node) : node_(node) {}

   public:
    bool operator==(const iterator& other) const {
      return node_ == other.node_;
>>>>>>> upstream-releases
    }

    bool operator!=(const iterator& other) const { return !(*this == other); }

    iterator& operator++() {
      node_ = node_->pn_next;
      return *this;
    }

    ParseNode* operator*() { return node_; }

    const ParseNode* operator*() const { return node_; }
  };

  class range {
   private:
    ParseNode* begin_;
    ParseNode* end_;

    friend class ListNode;
    range(ParseNode* begin, ParseNode* end) : begin_(begin), end_(end) {}

   public:
    iterator begin() { return iterator(begin_); }

    iterator end() { return iterator(end_); }

    const iterator begin() const { return iterator(begin_); }

    const iterator end() const { return iterator(end_); }

    const iterator cbegin() const { return begin(); }

    const iterator cend() const { return end(); }
  };

#ifdef DEBUG
  MOZ_MUST_USE bool contains(ParseNode* target) const {
    MOZ_ASSERT(target);
    for (ParseNode* node : contents()) {
      if (target == node) {
        return true;
      }
    }
    return false;
  }
#endif

 public:
  range contents() { return range(head(), nullptr); }

  const range contents() const { return range(head(), nullptr); }

  range contentsFrom(ParseNode* begin) {
    MOZ_ASSERT_IF(begin, contains(begin));
    return range(begin, nullptr);
  }

  const range contentsFrom(ParseNode* begin) const {
    MOZ_ASSERT_IF(begin, contains(begin));
    return range(begin, nullptr);
  }

  range contentsTo(ParseNode* end) {
    MOZ_ASSERT_IF(end, contains(end));
    return range(head(), end);
  }

  const range contentsTo(ParseNode* end) const {
    MOZ_ASSERT_IF(end, contains(end));
    return range(head(), end);
  }
};

<<<<<<< HEAD
inline bool ParseNode::isForLoopDeclaration() const {
  if (isKind(ParseNodeKind::Var) || isKind(ParseNodeKind::Let) ||
      isKind(ParseNodeKind::Const)) {
    MOZ_ASSERT(!as<ListNode>().empty());
    return true;
  }
||||||| merged common ancestors
inline bool
ParseNode::isForLoopDeclaration() const
{
    if (isKind(ParseNodeKind::Var) || isKind(ParseNodeKind::Let) || isKind(ParseNodeKind::Const)) {
        MOZ_ASSERT(!as<ListNode>().empty());
        return true;
    }
=======
inline bool ParseNode::isForLoopDeclaration() const {
  if (isKind(ParseNodeKind::VarStmt) || isKind(ParseNodeKind::LetDecl) ||
      isKind(ParseNodeKind::ConstDecl)) {
    MOZ_ASSERT(!as<ListNode>().empty());
    return true;
  }
>>>>>>> upstream-releases

  return false;
}

<<<<<<< HEAD
class CodeNode : public ParseNode {
 public:
  CodeNode(ParseNodeKind kind, JSOp op, const TokenPos& pos)
      : ParseNode(kind, op, pos) {
    MOZ_ASSERT(kind == ParseNodeKind::Function ||
               kind == ParseNodeKind::Module);
    MOZ_ASSERT_IF(kind == ParseNodeKind::Module, op == JSOP_NOP);
    MOZ_ASSERT(op == JSOP_NOP ||           // statement, module
               op == JSOP_LAMBDA_ARROW ||  // arrow function
               op == JSOP_LAMBDA);         // expression, method, accessor, &c.
    MOZ_ASSERT(!pn_u.code.body);
    MOZ_ASSERT(!pn_u.code.funbox);
    MOZ_ASSERT(is<CodeNode>());
  }
||||||| merged common ancestors
class CodeNode : public ParseNode
{
  public:
    CodeNode(ParseNodeKind kind, JSOp op, const TokenPos& pos)
      : ParseNode(kind, op, PN_CODE, pos)
    {
        MOZ_ASSERT(kind == ParseNodeKind::Function || kind == ParseNodeKind::Module);
        MOZ_ASSERT_IF(kind == ParseNodeKind::Module, op == JSOP_NOP);
        MOZ_ASSERT(op == JSOP_NOP || // statement, module
                   op == JSOP_LAMBDA_ARROW || // arrow function
                   op == JSOP_LAMBDA); // expression, method, accessor, &c.
        MOZ_ASSERT(!pn_u.code.body);
        MOZ_ASSERT(!pn_u.code.funbox);
    }
=======
class FunctionNode : public ParseNode {
  FunctionBox* funbox_;
  ParseNode* body_;
  FunctionSyntaxKind syntaxKind_;

 public:
  FunctionNode(FunctionSyntaxKind syntaxKind, const TokenPos& pos)
      : ParseNode(ParseNodeKind::Function, pos),
        funbox_(nullptr),
        body_(nullptr),
        syntaxKind_(syntaxKind) {
    MOZ_ASSERT(!body_);
    MOZ_ASSERT(!funbox_);
    MOZ_ASSERT(is<FunctionNode>());
  }

  static bool test(const ParseNode& node) {
    return node.isKind(ParseNodeKind::Function);
  }
>>>>>>> upstream-releases

<<<<<<< HEAD
  static bool test(const ParseNode& node) {
    bool match = node.isKind(ParseNodeKind::Function) ||
                 node.isKind(ParseNodeKind::Module);
    MOZ_ASSERT_IF(match, node.isArity(PN_CODE));
    return match;
  }
||||||| merged common ancestors
    static bool test(const ParseNode& node) {
        bool match = node.isKind(ParseNodeKind::Function) || node.isKind(ParseNodeKind::Module);
        MOZ_ASSERT_IF(match, node.isArity(PN_CODE));
        return match;
    }
=======
  static constexpr TypeCode classTypeCode() { return TypeCode::Other; }

  template <typename Visitor>
  bool accept(Visitor& visitor) {
    // Note: body is null for lazily-parsed functions.
    if (body_) {
      if (!visitor.visit(body_)) {
        return false;
      }
    }
    return true;
  }
>>>>>>> upstream-releases

#ifdef DEBUG
  void dumpImpl(GenericPrinter& out, int indent);
#endif

<<<<<<< HEAD
  FunctionBox* funbox() const { return pn_u.code.funbox; }
||||||| merged common ancestors
    FunctionBox* funbox() const {
        return pn_u.code.funbox;
    }
=======
  FunctionBox* funbox() const { return funbox_; }
>>>>>>> upstream-releases

<<<<<<< HEAD
  ListNode* body() const {
    return pn_u.code.body ? &pn_u.code.body->as<ListNode>() : nullptr;
  }
||||||| merged common ancestors
    ListNode* body() const {
        return pn_u.code.body ? &pn_u.code.body->as<ListNode>() : nullptr;
    }
=======
  ListNode* body() const { return body_ ? &body_->as<ListNode>() : nullptr; }
>>>>>>> upstream-releases

<<<<<<< HEAD
  void setFunbox(FunctionBox* funbox) { pn_u.code.funbox = funbox; }
||||||| merged common ancestors
    void setFunbox(FunctionBox* funbox) {
        pn_u.code.funbox = funbox;
    }
=======
  void setFunbox(FunctionBox* funbox) { funbox_ = funbox; }
>>>>>>> upstream-releases

<<<<<<< HEAD
  void setBody(ListNode* body) { pn_u.code.body = body; }
||||||| merged common ancestors
    void setBody(ListNode* body) {
        pn_u.code.body = body;
    }
=======
  void setBody(ListNode* body) { body_ = body; }
>>>>>>> upstream-releases

<<<<<<< HEAD
  // Methods used by FoldConstants.cpp.
  ParseNode** unsafeBodyReference() { return &pn_u.code.body; }
||||||| merged common ancestors
    // Methods used by FoldConstants.cpp.
    ParseNode** unsafeBodyReference() {
        return &pn_u.code.body;
    }
=======
  FunctionSyntaxKind syntaxKind() const { return syntaxKind_; }
>>>>>>> upstream-releases

<<<<<<< HEAD
  bool functionIsHoisted() const {
    MOZ_ASSERT(isKind(ParseNodeKind::Function));
    MOZ_ASSERT(
        isOp(JSOP_LAMBDA) ||        // lambda
        isOp(JSOP_LAMBDA_ARROW) ||  // arrow function
        isOp(JSOP_DEFFUN) ||        // non-body-level function statement
        isOp(JSOP_NOP) ||           // body-level function stmt in global code
        isOp(JSOP_GETLOCAL) ||      // body-level function stmt in function code
        isOp(JSOP_GETARG) ||        // body-level function redeclaring formal
        isOp(JSOP_INITLEXICAL));    // block-level function stmt
    return !isOp(JSOP_LAMBDA) && !isOp(JSOP_LAMBDA_ARROW) && !isOp(JSOP_DEFFUN);
  }
||||||| merged common ancestors
    bool functionIsHoisted() const {
        MOZ_ASSERT(isKind(ParseNodeKind::Function));
        MOZ_ASSERT(isOp(JSOP_LAMBDA) ||        // lambda
                   isOp(JSOP_LAMBDA_ARROW) ||  // arrow function
                   isOp(JSOP_DEFFUN) ||        // non-body-level function statement
                   isOp(JSOP_NOP) ||           // body-level function stmt in global code
                   isOp(JSOP_GETLOCAL) ||      // body-level function stmt in function code
                   isOp(JSOP_GETARG) ||        // body-level function redeclaring formal
                   isOp(JSOP_INITLEXICAL));    // block-level function stmt
        return !isOp(JSOP_LAMBDA) && !isOp(JSOP_LAMBDA_ARROW) && !isOp(JSOP_DEFFUN);
    }
=======
  bool functionIsHoisted() const {
    return syntaxKind() == FunctionSyntaxKind::Statement;
  }
>>>>>>> upstream-releases
};

<<<<<<< HEAD
class NumericLiteral : public ParseNode {
 public:
  NumericLiteral(double value, DecimalPoint decimalPoint, const TokenPos& pos)
      : ParseNode(ParseNodeKind::Number, JSOP_NOP, pos) {
    pn_u.number.value = value;
    pn_u.number.decimalPoint = decimalPoint;
  }
||||||| merged common ancestors
class NumericLiteral : public ParseNode
{
  public:
    NumericLiteral(double value, DecimalPoint decimalPoint, const TokenPos& pos)
      : ParseNode(ParseNodeKind::Number, JSOP_NOP, PN_NUMBER, pos)
    {
        pn_u.number.value = value;
        pn_u.number.decimalPoint = decimalPoint;
    }
=======
class ModuleNode : public ParseNode {
  ParseNode* body_;
>>>>>>> upstream-releases

<<<<<<< HEAD
  static bool test(const ParseNode& node) {
    bool match = node.isKind(ParseNodeKind::Number);
    MOZ_ASSERT_IF(match, node.isArity(PN_NUMBER));
    return match;
  }
||||||| merged common ancestors
    static bool test(const ParseNode& node) {
        bool match = node.isKind(ParseNodeKind::Number);
        MOZ_ASSERT_IF(match, node.isArity(PN_NUMBER));
        return match;
    }
=======
 public:
  explicit ModuleNode(const TokenPos& pos)
      : ParseNode(ParseNodeKind::Module, pos), body_(nullptr) {
    MOZ_ASSERT(!body_);
    MOZ_ASSERT(is<ModuleNode>());
  }

  static bool test(const ParseNode& node) {
    return node.isKind(ParseNodeKind::Module);
  }

  static constexpr TypeCode classTypeCode() { return TypeCode::Other; }

  template <typename Visitor>
  bool accept(Visitor& visitor) {
    return visitor.visit(body_);
  }
>>>>>>> upstream-releases

#ifdef DEBUG
<<<<<<< HEAD
  void dump(GenericPrinter& out, int indent);
||||||| merged common ancestors
    void dump(GenericPrinter& out, int indent);
=======
  void dumpImpl(GenericPrinter& out, int indent);
>>>>>>> upstream-releases
#endif

<<<<<<< HEAD
  double value() const { return pn_u.number.value; }
||||||| merged common ancestors
    double value() const {
        return pn_u.number.value;
    }
=======
  ListNode* body() const { return &body_->as<ListNode>(); }
>>>>>>> upstream-releases

<<<<<<< HEAD
  DecimalPoint decimalPoint() const { return pn_u.number.decimalPoint; }
||||||| merged common ancestors
    DecimalPoint decimalPoint() const {
        return pn_u.number.decimalPoint;
    }
=======
  void setBody(ListNode* body) { body_ = body; }
};
>>>>>>> upstream-releases

<<<<<<< HEAD
  void setValue(double v) { pn_u.number.value = v; }
||||||| merged common ancestors
    void setValue(double v) {
        pn_u.number.value = v;
    }
=======
class NumericLiteral : public ParseNode {
  double value_;              /* aligned numeric literal value */
  DecimalPoint decimalPoint_; /* Whether the number has a decimal point */

 public:
  NumericLiteral(double value, DecimalPoint decimalPoint, const TokenPos& pos)
      : ParseNode(ParseNodeKind::NumberExpr, pos),
        value_(value),
        decimalPoint_(decimalPoint) {}

  static bool test(const ParseNode& node) {
    return node.isKind(ParseNodeKind::NumberExpr);
  }

  static constexpr TypeCode classTypeCode() { return TypeCode::Other; }

  template <typename Visitor>
  bool accept(Visitor& visitor) {
    return true;
  }

#ifdef DEBUG
  void dumpImpl(GenericPrinter& out, int indent);
#endif

  double value() const { return value_; }

  DecimalPoint decimalPoint() const { return decimalPoint_; }

  void setValue(double v) { value_ = v; }

  void setDecimalPoint(DecimalPoint d) { decimalPoint_ = d; }
>>>>>>> upstream-releases
};

<<<<<<< HEAD
#ifdef ENABLE_BIGINT
class BigIntLiteral : public ParseNode {
 public:
  BigIntLiteral(BigIntBox* bibox, const TokenPos& pos)
      : ParseNode(ParseNodeKind::BigInt, JSOP_NOP, pos) {
    pn_u.bigint.box = bibox;
  }
||||||| merged common ancestors
class LexicalScopeNode : public ParseNode
{
  public:
    LexicalScopeNode(LexicalScope::Data* bindings, ParseNode* body)
      : ParseNode(ParseNodeKind::LexicalScope, JSOP_NOP, PN_SCOPE, body->pn_pos)
    {
        pn_u.scope.bindings = bindings;
        pn_u.scope.body = body;
    }
=======
class BigIntLiteral : public ParseNode {
  BigIntBox* box_;
>>>>>>> upstream-releases

<<<<<<< HEAD
  static bool test(const ParseNode& node) {
    bool match = node.isKind(ParseNodeKind::BigInt);
    MOZ_ASSERT_IF(match, node.isArity(PN_BIGINT));
    return match;
  }
||||||| merged common ancestors
    static bool test(const ParseNode& node) {
        bool match = node.isKind(ParseNodeKind::LexicalScope);
        MOZ_ASSERT_IF(match, node.isArity(PN_SCOPE));
        return match;
    }
=======
 public:
  BigIntLiteral(BigIntBox* bibox, const TokenPos& pos)
      : ParseNode(ParseNodeKind::BigIntExpr, pos), box_(bibox) {}

  static bool test(const ParseNode& node) {
    return node.isKind(ParseNodeKind::BigIntExpr);
  }

  static constexpr TypeCode classTypeCode() { return TypeCode::Other; }

  template <typename Visitor>
  bool accept(Visitor& visitor) {
    return true;
  }
>>>>>>> upstream-releases

#ifdef DEBUG
<<<<<<< HEAD
  void dump(GenericPrinter& out, int indent);
||||||| merged common ancestors
    void dump(GenericPrinter& out, int indent);
=======
  void dumpImpl(GenericPrinter& out, int indent);
>>>>>>> upstream-releases
#endif

<<<<<<< HEAD
  BigIntBox* box() const { return pn_u.bigint.box; }
};
#endif
||||||| merged common ancestors
    Handle<LexicalScope::Data*> scopeBindings() const {
        MOZ_ASSERT(!isEmptyScope());
        // Bindings' GC safety depend on the presence of an AutoKeepAtoms that
        // the rest of the frontend also depends on.
        return Handle<LexicalScope::Data*>::fromMarkedLocation(&pn_u.scope.bindings);
    }
=======
  BigIntBox* box() const { return box_; }
};
>>>>>>> upstream-releases

<<<<<<< HEAD
class LexicalScopeNode : public ParseNode {
 public:
  LexicalScopeNode(LexicalScope::Data* bindings, ParseNode* body)
      : ParseNode(ParseNodeKind::LexicalScope, JSOP_NOP, body->pn_pos) {
    pn_u.scope.bindings = bindings;
    pn_u.scope.body = body;
  }
||||||| merged common ancestors
    ParseNode* scopeBody() const {
        return pn_u.scope.body;
    }
=======
class LexicalScopeNode : public ParseNode {
  LexicalScope::Data* bindings;
  ParseNode* body;
>>>>>>> upstream-releases

<<<<<<< HEAD
  static bool test(const ParseNode& node) {
    bool match = node.isKind(ParseNodeKind::LexicalScope);
    MOZ_ASSERT_IF(match, node.isArity(PN_SCOPE));
    return match;
  }
||||||| merged common ancestors
    void setScopeBody(ParseNode* body) {
        pn_u.scope.body = body;
    }
=======
 public:
  LexicalScopeNode(LexicalScope::Data* bindings, ParseNode* body)
      : ParseNode(ParseNodeKind::LexicalScope, body->pn_pos),
        bindings(bindings),
        body(body) {}
>>>>>>> upstream-releases

<<<<<<< HEAD
#ifdef DEBUG
  void dump(GenericPrinter& out, int indent);
#endif
||||||| merged common ancestors
    bool isEmptyScope() const {
        return !pn_u.scope.bindings;
    }
=======
  static bool test(const ParseNode& node) {
    return node.isKind(ParseNodeKind::LexicalScope);
  }
>>>>>>> upstream-releases

<<<<<<< HEAD
  Handle<LexicalScope::Data*> scopeBindings() const {
    MOZ_ASSERT(!isEmptyScope());
    // Bindings' GC safety depend on the presence of an AutoKeepAtoms that
    // the rest of the frontend also depends on.
    return Handle<LexicalScope::Data*>::fromMarkedLocation(
        &pn_u.scope.bindings);
  }

  ParseNode* scopeBody() const { return pn_u.scope.body; }

  void setScopeBody(ParseNode* body) { pn_u.scope.body = body; }

  bool isEmptyScope() const { return !pn_u.scope.bindings; }

  ParseNode** unsafeScopeBodyReference() { return &pn_u.scope.body; }
||||||| merged common ancestors
    ParseNode** unsafeScopeBodyReference() {
        return &pn_u.scope.body;
    }
=======
  static constexpr TypeCode classTypeCode() { return TypeCode::Other; }

  template <typename Visitor>
  bool accept(Visitor& visitor) {
    return visitor.visit(body);
  }

#ifdef DEBUG
  void dumpImpl(GenericPrinter& out, int indent);
#endif

  Handle<LexicalScope::Data*> scopeBindings() const {
    MOZ_ASSERT(!isEmptyScope());
    // Bindings' GC safety depend on the presence of an AutoKeepAtoms that
    // the rest of the frontend also depends on.
    return Handle<LexicalScope::Data*>::fromMarkedLocation(&bindings);
  }

  void clearScopeBindings() { this->bindings = nullptr; }

  ParseNode* scopeBody() const { return body; }

  void setScopeBody(ParseNode* body) { this->body = body; }

  bool isEmptyScope() const { return !bindings; }
>>>>>>> upstream-releases
};

<<<<<<< HEAD
class LabeledStatement : public NameNode {
 public:
  LabeledStatement(PropertyName* label, ParseNode* stmt, uint32_t begin)
      : NameNode(ParseNodeKind::Label, JSOP_NOP, label, stmt,
                 TokenPos(begin, stmt->pn_pos.end)) {}
||||||| merged common ancestors
class LabeledStatement : public NameNode
{
  public:
    LabeledStatement(PropertyName* label, ParseNode* stmt, uint32_t begin)
      : NameNode(ParseNodeKind::Label, JSOP_NOP, label, stmt, TokenPos(begin, stmt->pn_pos.end))
    {}
=======
class LabeledStatement : public NameNode {
  ParseNode* statement_;
>>>>>>> upstream-releases

<<<<<<< HEAD
  PropertyName* label() const { return atom()->asPropertyName(); }
||||||| merged common ancestors
    PropertyName* label() const {
        return atom()->asPropertyName();
    }
=======
 public:
  LabeledStatement(PropertyName* label, ParseNode* stmt, uint32_t begin)
      : NameNode(ParseNodeKind::LabelStmt, label,
                 TokenPos(begin, stmt->pn_pos.end)),
        statement_(stmt) {}
>>>>>>> upstream-releases

<<<<<<< HEAD
  ParseNode* statement() const { return initializer(); }
||||||| merged common ancestors
    ParseNode* statement() const {
        return initializer();
    }
=======
  PropertyName* label() const { return atom()->asPropertyName(); }
>>>>>>> upstream-releases

<<<<<<< HEAD
  static bool test(const ParseNode& node) {
    bool match = node.isKind(ParseNodeKind::Label);
    MOZ_ASSERT_IF(match, node.isArity(PN_NAME));
    MOZ_ASSERT_IF(match, node.isOp(JSOP_NOP));
    return match;
  }
||||||| merged common ancestors
    static bool test(const ParseNode& node) {
        bool match = node.isKind(ParseNodeKind::Label);
        MOZ_ASSERT_IF(match, node.isArity(PN_NAME));
        MOZ_ASSERT_IF(match, node.isOp(JSOP_NOP));
        return match;
    }
=======
  ParseNode* statement() const { return statement_; }
>>>>>>> upstream-releases

<<<<<<< HEAD
  // Methods used by FoldConstants.cpp.
  ParseNode** unsafeStatementReference() {
    return unsafeInitializerReference();
  }
||||||| merged common ancestors
    // Methods used by FoldConstants.cpp.
    ParseNode** unsafeStatementReference() {
        return unsafeInitializerReference();
    }
=======
  static bool test(const ParseNode& node) {
    return node.isKind(ParseNodeKind::LabelStmt);
  }

  template <typename Visitor>
  bool accept(Visitor& visitor) {
    if (statement_) {
      if (!visitor.visit(statement_)) {
        return false;
      }
    }
    return true;
  }

#ifdef DEBUG
  void dumpImpl(GenericPrinter& out, int indent);
#endif
>>>>>>> upstream-releases
};

// Inside a switch statement, a CaseClause is a case-label and the subsequent
// statements. The same node type is used for DefaultClauses. The only
// difference is that their caseExpression() is null.
<<<<<<< HEAD
class CaseClause : public BinaryNode {
 public:
  CaseClause(ParseNode* expr, ParseNode* stmts, uint32_t begin)
      : BinaryNode(ParseNodeKind::Case, JSOP_NOP,
                   TokenPos(begin, stmts->pn_pos.end), expr, stmts) {}
||||||| merged common ancestors
class CaseClause : public BinaryNode
{
  public:
    CaseClause(ParseNode* expr, ParseNode* stmts, uint32_t begin)
      : BinaryNode(ParseNodeKind::Case, JSOP_NOP, TokenPos(begin, stmts->pn_pos.end), expr, stmts)
    {}

    ParseNode* caseExpression() const {
        return left();
    }
=======
class CaseClause : public BinaryNode {
 public:
  CaseClause(ParseNode* expr, ParseNode* stmts, uint32_t begin)
      : BinaryNode(ParseNodeKind::Case, TokenPos(begin, stmts->pn_pos.end),
                   expr, stmts) {}
>>>>>>> upstream-releases

  ParseNode* caseExpression() const { return left(); }

  bool isDefault() const { return !caseExpression(); }

<<<<<<< HEAD
  ListNode* statementList() const { return &right()->as<ListNode>(); }

  static bool test(const ParseNode& node) {
    bool match = node.isKind(ParseNodeKind::Case);
    MOZ_ASSERT_IF(match, node.is<BinaryNode>());
    MOZ_ASSERT_IF(match, node.isOp(JSOP_NOP));
    return match;
  }
||||||| merged common ancestors
    static bool test(const ParseNode& node) {
        bool match = node.isKind(ParseNodeKind::Case);
        MOZ_ASSERT_IF(match, node.is<BinaryNode>());
        MOZ_ASSERT_IF(match, node.isOp(JSOP_NOP));
        return match;
    }
=======
  ListNode* statementList() const { return &right()->as<ListNode>(); }

  static bool test(const ParseNode& node) {
    bool match = node.isKind(ParseNodeKind::Case);
    MOZ_ASSERT_IF(match, node.is<BinaryNode>());
    return match;
  }
>>>>>>> upstream-releases
};

<<<<<<< HEAD
class LoopControlStatement : public ParseNode {
 protected:
  LoopControlStatement(ParseNodeKind kind, PropertyName* label,
                       const TokenPos& pos)
      : ParseNode(kind, JSOP_NOP, pos) {
    MOZ_ASSERT(kind == ParseNodeKind::Break || kind == ParseNodeKind::Continue);
    pn_u.loopControl.label = label;
    MOZ_ASSERT(is<LoopControlStatement>());
  }
||||||| merged common ancestors
class LoopControlStatement : public ParseNode
{
  protected:
    LoopControlStatement(ParseNodeKind kind, PropertyName* label, const TokenPos& pos)
      : ParseNode(kind, JSOP_NOP, PN_LOOP, pos)
    {
        MOZ_ASSERT(kind == ParseNodeKind::Break || kind == ParseNodeKind::Continue);
        pn_u.loopControl.label = label;
    }
=======
class LoopControlStatement : public ParseNode {
  PropertyName* label_; /* target of break/continue statement */
>>>>>>> upstream-releases

<<<<<<< HEAD
 public:
  /* Label associated with this break/continue statement, if any. */
  PropertyName* label() const { return pn_u.loopControl.label; }
||||||| merged common ancestors
  public:
    /* Label associated with this break/continue statement, if any. */
    PropertyName* label() const {
        return pn_u.loopControl.label;
    }
=======
 protected:
  LoopControlStatement(ParseNodeKind kind, PropertyName* label,
                       const TokenPos& pos)
      : ParseNode(kind, pos), label_(label) {
    MOZ_ASSERT(kind == ParseNodeKind::BreakStmt ||
               kind == ParseNodeKind::ContinueStmt);
    MOZ_ASSERT(is<LoopControlStatement>());
  }

 public:
  /* Label associated with this break/continue statement, if any. */
  PropertyName* label() const { return label_; }
>>>>>>> upstream-releases

#ifdef DEBUG
<<<<<<< HEAD
  void dump(GenericPrinter& out, int indent);
||||||| merged common ancestors
    void dump(GenericPrinter& out, int indent);
=======
  void dumpImpl(GenericPrinter& out, int indent);
>>>>>>> upstream-releases
#endif

<<<<<<< HEAD
  static bool test(const ParseNode& node) {
    bool match = node.isKind(ParseNodeKind::Break) ||
                 node.isKind(ParseNodeKind::Continue);
    MOZ_ASSERT_IF(match, node.isArity(PN_LOOP));
    MOZ_ASSERT_IF(match, node.isOp(JSOP_NOP));
    return match;
  }
||||||| merged common ancestors
    static bool test(const ParseNode& node) {
        bool match = node.isKind(ParseNodeKind::Break) || node.isKind(ParseNodeKind::Continue);
        MOZ_ASSERT_IF(match, node.isArity(PN_LOOP));
        MOZ_ASSERT_IF(match, node.isOp(JSOP_NOP));
        return match;
    }
=======
  static bool test(const ParseNode& node) {
    return node.isKind(ParseNodeKind::BreakStmt) ||
           node.isKind(ParseNodeKind::ContinueStmt);
  }

  static constexpr TypeCode classTypeCode() { return TypeCode::Other; }

  template <typename Visitor>
  bool accept(Visitor& visitor) {
    return true;
  }
>>>>>>> upstream-releases
};

<<<<<<< HEAD
class BreakStatement : public LoopControlStatement {
 public:
  BreakStatement(PropertyName* label, const TokenPos& pos)
      : LoopControlStatement(ParseNodeKind::Break, label, pos) {}

  static bool test(const ParseNode& node) {
    bool match = node.isKind(ParseNodeKind::Break);
    MOZ_ASSERT_IF(match, node.is<LoopControlStatement>());
    MOZ_ASSERT_IF(match, node.isOp(JSOP_NOP));
    return match;
  }
||||||| merged common ancestors
class BreakStatement : public LoopControlStatement
{
  public:
    BreakStatement(PropertyName* label, const TokenPos& pos)
      : LoopControlStatement(ParseNodeKind::Break, label, pos)
    { }

    static bool test(const ParseNode& node) {
        bool match = node.isKind(ParseNodeKind::Break);
        MOZ_ASSERT_IF(match, node.is<LoopControlStatement>());
        MOZ_ASSERT_IF(match, node.isOp(JSOP_NOP));
        return match;
    }
=======
class BreakStatement : public LoopControlStatement {
 public:
  BreakStatement(PropertyName* label, const TokenPos& pos)
      : LoopControlStatement(ParseNodeKind::BreakStmt, label, pos) {}

  static bool test(const ParseNode& node) {
    bool match = node.isKind(ParseNodeKind::BreakStmt);
    MOZ_ASSERT_IF(match, node.is<LoopControlStatement>());
    return match;
  }
>>>>>>> upstream-releases
};

<<<<<<< HEAD
class ContinueStatement : public LoopControlStatement {
 public:
  ContinueStatement(PropertyName* label, const TokenPos& pos)
      : LoopControlStatement(ParseNodeKind::Continue, label, pos) {}

  static bool test(const ParseNode& node) {
    bool match = node.isKind(ParseNodeKind::Continue);
    MOZ_ASSERT_IF(match, node.is<LoopControlStatement>());
    MOZ_ASSERT_IF(match, node.isOp(JSOP_NOP));
    return match;
  }
||||||| merged common ancestors
class ContinueStatement : public LoopControlStatement
{
  public:
    ContinueStatement(PropertyName* label, const TokenPos& pos)
      : LoopControlStatement(ParseNodeKind::Continue, label, pos)
    { }

    static bool test(const ParseNode& node) {
        bool match = node.isKind(ParseNodeKind::Continue);
        MOZ_ASSERT_IF(match, node.is<LoopControlStatement>());
        MOZ_ASSERT_IF(match, node.isOp(JSOP_NOP));
        return match;
    }
=======
class ContinueStatement : public LoopControlStatement {
 public:
  ContinueStatement(PropertyName* label, const TokenPos& pos)
      : LoopControlStatement(ParseNodeKind::ContinueStmt, label, pos) {}

  static bool test(const ParseNode& node) {
    bool match = node.isKind(ParseNodeKind::ContinueStmt);
    MOZ_ASSERT_IF(match, node.is<LoopControlStatement>());
    return match;
  }
>>>>>>> upstream-releases
};

<<<<<<< HEAD
class DebuggerStatement : public NullaryNode {
 public:
  explicit DebuggerStatement(const TokenPos& pos)
      : NullaryNode(ParseNodeKind::Debugger, JSOP_NOP, pos) {}

  static bool test(const ParseNode& node) {
    bool match = node.isKind(ParseNodeKind::Debugger);
    MOZ_ASSERT_IF(match, node.is<NullaryNode>());
    return match;
  }
||||||| merged common ancestors
class DebuggerStatement : public NullaryNode
{
  public:
    explicit DebuggerStatement(const TokenPos& pos)
      : NullaryNode(ParseNodeKind::Debugger, JSOP_NOP, pos)
    { }

    static bool test(const ParseNode& node) {
        bool match = node.isKind(ParseNodeKind::Debugger);
        MOZ_ASSERT_IF(match, node.is<NullaryNode>());
        return match;
    }
=======
class DebuggerStatement : public NullaryNode {
 public:
  explicit DebuggerStatement(const TokenPos& pos)
      : NullaryNode(ParseNodeKind::DebuggerStmt, pos) {}

  static bool test(const ParseNode& node) {
    bool match = node.isKind(ParseNodeKind::DebuggerStmt);
    MOZ_ASSERT_IF(match, node.is<NullaryNode>());
    return match;
  }
>>>>>>> upstream-releases
};

<<<<<<< HEAD
class ConditionalExpression : public TernaryNode {
 public:
  ConditionalExpression(ParseNode* condition, ParseNode* thenExpr,
                        ParseNode* elseExpr)
      : TernaryNode(ParseNodeKind::Conditional, condition, thenExpr, elseExpr,
                    TokenPos(condition->pn_pos.begin, elseExpr->pn_pos.end)) {
    MOZ_ASSERT(condition);
    MOZ_ASSERT(thenExpr);
    MOZ_ASSERT(elseExpr);
  }
||||||| merged common ancestors
class ConditionalExpression : public TernaryNode
{
  public:
    ConditionalExpression(ParseNode* condition, ParseNode* thenExpr, ParseNode* elseExpr)
      : TernaryNode(ParseNodeKind::Conditional, condition, thenExpr, elseExpr,
                    TokenPos(condition->pn_pos.begin, elseExpr->pn_pos.end))
    {
        MOZ_ASSERT(condition);
        MOZ_ASSERT(thenExpr);
        MOZ_ASSERT(elseExpr);
    }
=======
class ConditionalExpression : public TernaryNode {
 public:
  ConditionalExpression(ParseNode* condition, ParseNode* thenExpr,
                        ParseNode* elseExpr)
      : TernaryNode(ParseNodeKind::ConditionalExpr, condition, thenExpr,
                    elseExpr,
                    TokenPos(condition->pn_pos.begin, elseExpr->pn_pos.end)) {
    MOZ_ASSERT(condition);
    MOZ_ASSERT(thenExpr);
    MOZ_ASSERT(elseExpr);
  }
>>>>>>> upstream-releases

  ParseNode& condition() const { return *kid1(); }

  ParseNode& thenExpression() const { return *kid2(); }

  ParseNode& elseExpression() const { return *kid3(); }

<<<<<<< HEAD
  static bool test(const ParseNode& node) {
    bool match = node.isKind(ParseNodeKind::Conditional);
    MOZ_ASSERT_IF(match, node.is<TernaryNode>());
    MOZ_ASSERT_IF(match, node.isOp(JSOP_NOP));
    return match;
  }
||||||| merged common ancestors
    static bool test(const ParseNode& node) {
        bool match = node.isKind(ParseNodeKind::Conditional);
        MOZ_ASSERT_IF(match, node.is<TernaryNode>());
        MOZ_ASSERT_IF(match, node.isOp(JSOP_NOP));
        return match;
    }
=======
  static bool test(const ParseNode& node) {
    bool match = node.isKind(ParseNodeKind::ConditionalExpr);
    MOZ_ASSERT_IF(match, node.is<TernaryNode>());
    return match;
  }
>>>>>>> upstream-releases
};

<<<<<<< HEAD
class TryNode : public TernaryNode {
 public:
  TryNode(uint32_t begin, ParseNode* body, LexicalScopeNode* catchScope,
          ParseNode* finallyBlock)
      : TernaryNode(
            ParseNodeKind::Try, body, catchScope, finallyBlock,
            TokenPos(begin,
                     (finallyBlock ? finallyBlock : catchScope)->pn_pos.end)) {
    MOZ_ASSERT(body);
    MOZ_ASSERT(catchScope || finallyBlock);
  }
||||||| merged common ancestors
class TryNode : public TernaryNode
{
  public:
    TryNode(uint32_t begin, ParseNode* body, LexicalScopeNode* catchScope,
            ParseNode* finallyBlock)
      : TernaryNode(ParseNodeKind::Try, body, catchScope, finallyBlock,
                    TokenPos(begin, (finallyBlock ? finallyBlock : catchScope)->pn_pos.end))
    {
        MOZ_ASSERT(body);
        MOZ_ASSERT(catchScope || finallyBlock);
    }
=======
class TryNode : public TernaryNode {
 public:
  TryNode(uint32_t begin, ParseNode* body, LexicalScopeNode* catchScope,
          ParseNode* finallyBlock)
      : TernaryNode(
            ParseNodeKind::TryStmt, body, catchScope, finallyBlock,
            TokenPos(begin,
                     (finallyBlock ? finallyBlock : catchScope)->pn_pos.end)) {
    MOZ_ASSERT(body);
    MOZ_ASSERT(catchScope || finallyBlock);
  }
>>>>>>> upstream-releases

<<<<<<< HEAD
  static bool test(const ParseNode& node) {
    bool match = node.isKind(ParseNodeKind::Try);
    MOZ_ASSERT_IF(match, node.is<TernaryNode>());
    return match;
  }
||||||| merged common ancestors
    static bool test(const ParseNode& node) {
        bool match = node.isKind(ParseNodeKind::Try);
        MOZ_ASSERT_IF(match, node.is<TernaryNode>());
        return match;
    }
=======
  static bool test(const ParseNode& node) {
    bool match = node.isKind(ParseNodeKind::TryStmt);
    MOZ_ASSERT_IF(match, node.is<TernaryNode>());
    return match;
  }
>>>>>>> upstream-releases

  ParseNode* body() const { return kid1(); }

  LexicalScopeNode* catchScope() const {
    return kid2() ? &kid2()->as<LexicalScopeNode>() : nullptr;
  }

  ParseNode* finallyBlock() const { return kid3(); }
};

<<<<<<< HEAD
class ThisLiteral : public UnaryNode {
 public:
  ThisLiteral(const TokenPos& pos, ParseNode* thisName)
      : UnaryNode(ParseNodeKind::This, pos, thisName) {}

  static bool test(const ParseNode& node) {
    bool match = node.isKind(ParseNodeKind::This);
    MOZ_ASSERT_IF(match, node.is<UnaryNode>());
    return match;
  }
||||||| merged common ancestors
class ThisLiteral : public UnaryNode
{
  public:
    ThisLiteral(const TokenPos& pos, ParseNode* thisName)
      : UnaryNode(ParseNodeKind::This, pos, thisName)
    { }

    static bool test(const ParseNode& node) {
        bool match = node.isKind(ParseNodeKind::This);
        MOZ_ASSERT_IF(match, node.is<UnaryNode>());
        return match;
    }
=======
class ThisLiteral : public UnaryNode {
 public:
  ThisLiteral(const TokenPos& pos, ParseNode* thisName)
      : UnaryNode(ParseNodeKind::ThisExpr, pos, thisName) {}

  static bool test(const ParseNode& node) {
    bool match = node.isKind(ParseNodeKind::ThisExpr);
    MOZ_ASSERT_IF(match, node.is<UnaryNode>());
    return match;
  }
>>>>>>> upstream-releases
};

<<<<<<< HEAD
class NullLiteral : public NullaryNode {
 public:
  explicit NullLiteral(const TokenPos& pos)
      : NullaryNode(ParseNodeKind::Null, JSOP_NULL, pos) {}

  static bool test(const ParseNode& node) {
    bool match = node.isKind(ParseNodeKind::Null);
    MOZ_ASSERT_IF(match, node.is<NullaryNode>());
    return match;
  }
||||||| merged common ancestors
class NullLiteral : public NullaryNode
{
  public:
    explicit NullLiteral(const TokenPos& pos)
      : NullaryNode(ParseNodeKind::Null, JSOP_NULL, pos)
    { }

    static bool test(const ParseNode& node) {
        bool match = node.isKind(ParseNodeKind::Null);
        MOZ_ASSERT_IF(match, node.is<NullaryNode>());
        return match;
    }
=======
class NullLiteral : public NullaryNode {
 public:
  explicit NullLiteral(const TokenPos& pos)
      : NullaryNode(ParseNodeKind::NullExpr, pos) {}

  static bool test(const ParseNode& node) {
    bool match = node.isKind(ParseNodeKind::NullExpr);
    MOZ_ASSERT_IF(match, node.is<NullaryNode>());
    return match;
  }
>>>>>>> upstream-releases
};

<<<<<<< HEAD
// This is only used internally, currently just for tagged templates.
// It represents the value 'undefined' (aka `void 0`), like NullLiteral
// represents the value 'null'.
class RawUndefinedLiteral : public NullaryNode {
 public:
  explicit RawUndefinedLiteral(const TokenPos& pos)
      : NullaryNode(ParseNodeKind::RawUndefined, JSOP_UNDEFINED, pos) {}

  static bool test(const ParseNode& node) {
    bool match = node.isKind(ParseNodeKind::RawUndefined);
    MOZ_ASSERT_IF(match, node.is<NullaryNode>());
    return match;
  }
||||||| merged common ancestors
// This is only used internally, currently just for tagged templates.
// It represents the value 'undefined' (aka `void 0`), like NullLiteral
// represents the value 'null'.
class RawUndefinedLiteral : public NullaryNode
{
  public:
    explicit RawUndefinedLiteral(const TokenPos& pos)
      : NullaryNode(ParseNodeKind::RawUndefined, JSOP_UNDEFINED, pos) { }

    static bool test(const ParseNode& node) {
        bool match = node.isKind(ParseNodeKind::RawUndefined);
        MOZ_ASSERT_IF(match, node.is<NullaryNode>());
        return match;
    }
=======
// This is only used internally, currently just for tagged templates and the
// initial value of fields without initializers. It represents the value
// 'undefined' (aka `void 0`), like NullLiteral represents the value 'null'.
class RawUndefinedLiteral : public NullaryNode {
 public:
  explicit RawUndefinedLiteral(const TokenPos& pos)
      : NullaryNode(ParseNodeKind::RawUndefinedExpr, pos) {}

  static bool test(const ParseNode& node) {
    bool match = node.isKind(ParseNodeKind::RawUndefinedExpr);
    MOZ_ASSERT_IF(match, node.is<NullaryNode>());
    return match;
  }
>>>>>>> upstream-releases
};

<<<<<<< HEAD
class BooleanLiteral : public NullaryNode {
 public:
  BooleanLiteral(bool b, const TokenPos& pos)
      : NullaryNode(b ? ParseNodeKind::True : ParseNodeKind::False,
                    b ? JSOP_TRUE : JSOP_FALSE, pos) {}

  static bool test(const ParseNode& node) {
    bool match =
        node.isKind(ParseNodeKind::True) || node.isKind(ParseNodeKind::False);
    MOZ_ASSERT_IF(match, node.is<NullaryNode>());
    return match;
  }
||||||| merged common ancestors
class BooleanLiteral : public NullaryNode
{
  public:
    BooleanLiteral(bool b, const TokenPos& pos)
      : NullaryNode(b ? ParseNodeKind::True : ParseNodeKind::False,
                    b ? JSOP_TRUE : JSOP_FALSE, pos)
    { }

    static bool test(const ParseNode& node) {
        bool match = node.isKind(ParseNodeKind::True) || node.isKind(ParseNodeKind::False);
        MOZ_ASSERT_IF(match, node.is<NullaryNode>());
        return match;
    }
=======
class BooleanLiteral : public NullaryNode {
 public:
  BooleanLiteral(bool b, const TokenPos& pos)
      : NullaryNode(b ? ParseNodeKind::TrueExpr : ParseNodeKind::FalseExpr,
                    pos) {}

  static bool test(const ParseNode& node) {
    bool match = node.isKind(ParseNodeKind::TrueExpr) ||
                 node.isKind(ParseNodeKind::FalseExpr);
    MOZ_ASSERT_IF(match, node.is<NullaryNode>());
    return match;
  }
>>>>>>> upstream-releases
};

<<<<<<< HEAD
class RegExpLiteral : public ParseNode {
 public:
  RegExpLiteral(ObjectBox* reobj, const TokenPos& pos)
      : ParseNode(ParseNodeKind::RegExp, JSOP_REGEXP, pos) {
    pn_u.regexp.objbox = reobj;
  }
||||||| merged common ancestors
class RegExpLiteral : public ParseNode
{
  public:
    RegExpLiteral(ObjectBox* reobj, const TokenPos& pos)
      : ParseNode(ParseNodeKind::RegExp, JSOP_REGEXP, PN_REGEXP, pos)
    {
        pn_u.regexp.objbox = reobj;
    }
=======
class RegExpLiteral : public ParseNode {
  ObjectBox* objbox_;
>>>>>>> upstream-releases

<<<<<<< HEAD
  ObjectBox* objbox() const { return pn_u.regexp.objbox; }
||||||| merged common ancestors
    ObjectBox* objbox() const {
        return pn_u.regexp.objbox;
    }
=======
 public:
  RegExpLiteral(ObjectBox* reobj, const TokenPos& pos)
      : ParseNode(ParseNodeKind::RegExpExpr, pos), objbox_(reobj) {}

  ObjectBox* objbox() const { return objbox_; }
>>>>>>> upstream-releases

#ifdef DEBUG
<<<<<<< HEAD
  void dump(GenericPrinter& out, int indent);
||||||| merged common ancestors
    void dump(GenericPrinter& out, int indent);
=======
  void dumpImpl(GenericPrinter& out, int indent);
>>>>>>> upstream-releases
#endif

<<<<<<< HEAD
  static bool test(const ParseNode& node) {
    bool match = node.isKind(ParseNodeKind::RegExp);
    MOZ_ASSERT_IF(match, node.isArity(PN_REGEXP));
    MOZ_ASSERT_IF(match, node.isOp(JSOP_REGEXP));
    return match;
  }
||||||| merged common ancestors
    static bool test(const ParseNode& node) {
        bool match = node.isKind(ParseNodeKind::RegExp);
        MOZ_ASSERT_IF(match, node.isArity(PN_REGEXP));
        MOZ_ASSERT_IF(match, node.isOp(JSOP_REGEXP));
        return match;
    }
=======
  static bool test(const ParseNode& node) {
    return node.isKind(ParseNodeKind::RegExpExpr);
  }

  static constexpr TypeCode classTypeCode() { return TypeCode::Other; }

  template <typename Visitor>
  bool accept(Visitor& visitor) {
    return true;
  }
>>>>>>> upstream-releases
};

<<<<<<< HEAD
class PropertyAccess : public BinaryNode {
 public:
  /*
   * PropertyAccess nodes can have any expression/'super' as left-hand
   * side, but the name must be a ParseNodeKind::PropertyName node.
   */
  PropertyAccess(ParseNode* lhs, NameNode* name, uint32_t begin, uint32_t end)
      : BinaryNode(ParseNodeKind::Dot, JSOP_NOP, TokenPos(begin, end), lhs,
                   name) {
    MOZ_ASSERT(lhs);
    MOZ_ASSERT(name);
  }
||||||| merged common ancestors
class PropertyAccess : public BinaryNode
{
  public:
    /*
     * PropertyAccess nodes can have any expression/'super' as left-hand
     * side, but the name must be a ParseNodeKind::PropertyName node.
     */
    PropertyAccess(ParseNode* lhs, NameNode* name, uint32_t begin, uint32_t end)
      : BinaryNode(ParseNodeKind::Dot, JSOP_NOP, TokenPos(begin, end), lhs, name)
    {
        MOZ_ASSERT(lhs);
        MOZ_ASSERT(name);
    }
=======
class PropertyAccess : public BinaryNode {
 public:
  /*
   * PropertyAccess nodes can have any expression/'super' as left-hand
   * side, but the name must be a ParseNodeKind::PropertyName node.
   */
  PropertyAccess(ParseNode* lhs, NameNode* name, uint32_t begin, uint32_t end)
      : BinaryNode(ParseNodeKind::DotExpr, TokenPos(begin, end), lhs, name) {
    MOZ_ASSERT(lhs);
    MOZ_ASSERT(name);
  }
>>>>>>> upstream-releases

<<<<<<< HEAD
  static bool test(const ParseNode& node) {
    bool match = node.isKind(ParseNodeKind::Dot);
    MOZ_ASSERT_IF(match, node.is<BinaryNode>());
    MOZ_ASSERT_IF(match, node.as<BinaryNode>().right()->isKind(
                             ParseNodeKind::PropertyName));
    return match;
  }
||||||| merged common ancestors
    static bool test(const ParseNode& node) {
        bool match = node.isKind(ParseNodeKind::Dot);
        MOZ_ASSERT_IF(match, node.is<BinaryNode>());
        MOZ_ASSERT_IF(match, node.as<BinaryNode>().right()->isKind(ParseNodeKind::PropertyName));
        return match;
    }
=======
  static bool test(const ParseNode& node) {
    bool match = node.isKind(ParseNodeKind::DotExpr);
    MOZ_ASSERT_IF(match, node.is<BinaryNode>());
    MOZ_ASSERT_IF(match, node.as<BinaryNode>().right()->isKind(
                             ParseNodeKind::PropertyNameExpr));
    return match;
  }
>>>>>>> upstream-releases

  ParseNode& expression() const { return *left(); }

  NameNode& key() const { return right()->as<NameNode>(); }

  // Method used by BytecodeEmitter::emitPropLHS for optimization.
  // Those methods allow expression to temporarily be nullptr for
  // optimization purpose.
  ParseNode* maybeExpression() const { return left(); }

<<<<<<< HEAD
  void setExpression(ParseNode* pn) { pn_u.binary.left = pn; }
||||||| merged common ancestors
    void setExpression(ParseNode* pn) {
        pn_u.binary.left = pn;
    }
=======
  void setExpression(ParseNode* pn) { *unsafeLeftReference() = pn; }
>>>>>>> upstream-releases

  PropertyName& name() const {
    return *right()->as<NameNode>().atom()->asPropertyName();
  }

  bool isSuper() const {
    // ParseNodeKind::SuperBase cannot result from any expression syntax.
    return expression().isKind(ParseNodeKind::SuperBase);
  }
};

<<<<<<< HEAD
class PropertyByValue : public BinaryNode {
 public:
  PropertyByValue(ParseNode* lhs, ParseNode* propExpr, uint32_t begin,
                  uint32_t end)
      : BinaryNode(ParseNodeKind::Elem, JSOP_NOP, TokenPos(begin, end), lhs,
                   propExpr) {}

  static bool test(const ParseNode& node) {
    bool match = node.isKind(ParseNodeKind::Elem);
    MOZ_ASSERT_IF(match, node.is<BinaryNode>());
    return match;
  }
||||||| merged common ancestors
class PropertyByValue : public BinaryNode
{
  public:
    PropertyByValue(ParseNode* lhs, ParseNode* propExpr, uint32_t begin, uint32_t end)
      : BinaryNode(ParseNodeKind::Elem, JSOP_NOP, TokenPos(begin, end), lhs, propExpr)
    {}

    static bool test(const ParseNode& node) {
        bool match = node.isKind(ParseNodeKind::Elem);
        MOZ_ASSERT_IF(match, node.is<BinaryNode>());
        return match;
    }
=======
class PropertyByValue : public BinaryNode {
 public:
  PropertyByValue(ParseNode* lhs, ParseNode* propExpr, uint32_t begin,
                  uint32_t end)
      : BinaryNode(ParseNodeKind::ElemExpr, TokenPos(begin, end), lhs,
                   propExpr) {}

  static bool test(const ParseNode& node) {
    bool match = node.isKind(ParseNodeKind::ElemExpr);
    MOZ_ASSERT_IF(match, node.is<BinaryNode>());
    return match;
  }
>>>>>>> upstream-releases

  ParseNode& expression() const { return *left(); }

  ParseNode& key() const { return *right(); }

  bool isSuper() const { return left()->isKind(ParseNodeKind::SuperBase); }
};

/*
 * A CallSiteNode represents the implicit call site object argument in a
 * TaggedTemplate.
 */
class CallSiteNode : public ListNode {
 public:
  explicit CallSiteNode(uint32_t begin)
      : ListNode(ParseNodeKind::CallSiteObj, TokenPos(begin, begin + 1)) {}

  static bool test(const ParseNode& node) {
    bool match = node.isKind(ParseNodeKind::CallSiteObj);
    MOZ_ASSERT_IF(match, node.is<ListNode>());
    return match;
  }

  MOZ_MUST_USE bool getRawArrayValue(JSContext* cx, MutableHandleValue vp) {
    return head()->getConstantValue(cx, AllowObjects, vp);
  }

  ListNode* rawNodes() const {
    MOZ_ASSERT(head());
    return &head()->as<ListNode>();
  }
};

<<<<<<< HEAD
class ClassMethod : public BinaryNode {
 public:
  /*
   * Method definitions often keep a name and function body that overlap,
   * so explicitly define the beginning and end here.
   */
  ClassMethod(ParseNode* name, ParseNode* body, JSOp op, bool isStatic)
      : BinaryNode(ParseNodeKind::ClassMethod, op,
                   TokenPos(name->pn_pos.begin, body->pn_pos.end), name, body) {
    pn_u.binary.isStatic = isStatic;
  }
||||||| merged common ancestors
class ClassMethod : public BinaryNode
{
  public:
    /*
     * Method definitions often keep a name and function body that overlap,
     * so explicitly define the beginning and end here.
     */
    ClassMethod(ParseNode* name, ParseNode* body, JSOp op, bool isStatic)
      : BinaryNode(ParseNodeKind::ClassMethod, op, TokenPos(name->pn_pos.begin, body->pn_pos.end), name, body)
    {
        pn_u.binary.isStatic = isStatic;
    }
=======
class CallNode : public BinaryNode {
  const JSOp callOp_;
>>>>>>> upstream-releases

<<<<<<< HEAD
  static bool test(const ParseNode& node) {
    bool match = node.isKind(ParseNodeKind::ClassMethod);
    MOZ_ASSERT_IF(match, node.is<BinaryNode>());
    return match;
  }
||||||| merged common ancestors
    static bool test(const ParseNode& node) {
        bool match = node.isKind(ParseNodeKind::ClassMethod);
        MOZ_ASSERT_IF(match, node.is<BinaryNode>());
        return match;
    }
=======
 public:
  CallNode(ParseNodeKind kind, JSOp callOp, ParseNode* left, ParseNode* right)
      : CallNode(kind, callOp, TokenPos(left->pn_pos.begin, right->pn_pos.end),
                 left, right) {}
>>>>>>> upstream-releases

<<<<<<< HEAD
  ParseNode& name() const { return *left(); }
||||||| merged common ancestors
    ParseNode& name() const {
        return *left();
    }
=======
  CallNode(ParseNodeKind kind, JSOp callOp, TokenPos pos, ParseNode* left,
           ParseNode* right)
      : BinaryNode(kind, pos, left, right), callOp_(callOp) {
    MOZ_ASSERT(is<CallNode>());
  }
>>>>>>> upstream-releases

<<<<<<< HEAD
  CodeNode& method() const { return right()->as<CodeNode>(); }
||||||| merged common ancestors
    CodeNode& method() const {
        return right()->as<CodeNode>();
    }
=======
  static bool test(const ParseNode& node) {
    bool match = node.isKind(ParseNodeKind::CallExpr) ||
                 node.isKind(ParseNodeKind::SuperCallExpr) ||
                 node.isKind(ParseNodeKind::TaggedTemplateExpr) ||
                 node.isKind(ParseNodeKind::CallImportExpr) ||
                 node.isKind(ParseNodeKind::NewExpr);
    MOZ_ASSERT_IF(match, node.is<BinaryNode>());
    return match;
  }
>>>>>>> upstream-releases

<<<<<<< HEAD
  bool isStatic() const { return pn_u.binary.isStatic; }
};

class ClassField : public ParseNode {
 public:
  ClassField(ParseNode* name, ParseNode* initializer)
      : ParseNode(ParseNodeKind::ClassField, JSOP_NOP,
                  initializer == nullptr
                      ? name->pn_pos
                      : TokenPos::box(name->pn_pos, initializer->pn_pos)) {
    pn_u.field.name = name;
    pn_u.field.initializer = initializer;
  }

  static bool test(const ParseNode& node) {
    return node.isKind(ParseNodeKind::ClassField);
  }

  ParseNode& name() const { return *pn_u.field.name; }

  bool hasInitializer() const { return pn_u.field.initializer != nullptr; }

  ParseNode& initializer() const { return *pn_u.field.initializer; }

#ifdef DEBUG
  void dump(GenericPrinter& out, int indent);
#endif

  // Methods used by FoldConstants.cpp.
  // callers are responsible for keeping the list consistent.
  ParseNode** unsafeNameReference() { return &pn_u.field.name; }

  ParseNode** unsafeInitializerReference() { return &pn_u.field.initializer; }
||||||| merged common ancestors
    bool isStatic() const {
        return pn_u.binary.isStatic;
    }
=======
  JSOp callOp() { return callOp_; }
};

class ClassMethod : public BinaryNode {
  bool isStatic_;
  AccessorType accessorType_;

 public:
  /*
   * Method definitions often keep a name and function body that overlap,
   * so explicitly define the beginning and end here.
   */
  ClassMethod(ParseNode* name, ParseNode* body, AccessorType accessorType,
              bool isStatic)
      : BinaryNode(ParseNodeKind::ClassMethod,
                   TokenPos(name->pn_pos.begin, body->pn_pos.end), name, body),
        isStatic_(isStatic),
        accessorType_(accessorType) {}

  static bool test(const ParseNode& node) {
    bool match = node.isKind(ParseNodeKind::ClassMethod);
    MOZ_ASSERT_IF(match, node.is<BinaryNode>());
    return match;
  }

  ParseNode& name() const { return *left(); }

  FunctionNode& method() const { return right()->as<FunctionNode>(); }

  bool isStatic() const { return isStatic_; }

  AccessorType accessorType() const { return accessorType_; }
>>>>>>> upstream-releases
};

<<<<<<< HEAD
class SwitchStatement : public BinaryNode {
 public:
  SwitchStatement(uint32_t begin, ParseNode* discriminant,
                  LexicalScopeNode* lexicalForCaseList, bool hasDefault)
      : BinaryNode(ParseNodeKind::Switch, JSOP_NOP,
||||||| merged common ancestors
class SwitchStatement : public BinaryNode
{
  public:
    SwitchStatement(uint32_t begin, ParseNode* discriminant, LexicalScopeNode* lexicalForCaseList,
                    bool hasDefault)
      : BinaryNode(ParseNodeKind::Switch, JSOP_NOP,
=======
class ClassField : public BinaryNode {
 public:
  ClassField(ParseNode* name, ParseNode* initializer)
      : BinaryNode(ParseNodeKind::ClassField,
                   initializer == nullptr
                       ? name->pn_pos
                       : TokenPos::box(name->pn_pos, initializer->pn_pos),
                   name, initializer) {}

  static bool test(const ParseNode& node) {
    bool match = node.isKind(ParseNodeKind::ClassField);
    MOZ_ASSERT_IF(match, node.is<BinaryNode>());
    return match;
  }

  ParseNode& name() const { return *left(); }

  FunctionNode* initializer() const {
    return right() ? &right()->as<FunctionNode>() : nullptr;
  }
};

class PropertyDefinition : public BinaryNode {
  AccessorType accessorType_;

 public:
  PropertyDefinition(ParseNode* name, ParseNode* value,
                     AccessorType accessorType)
      : BinaryNode(ParseNodeKind::PropertyDefinition,
                   TokenPos(name->pn_pos.begin, value->pn_pos.end), name,
                   value),
        accessorType_(accessorType) {}

  static bool test(const ParseNode& node) {
    bool match = node.isKind(ParseNodeKind::PropertyDefinition);
    MOZ_ASSERT_IF(match, node.is<BinaryNode>());
    return match;
  }

  AccessorType accessorType() { return accessorType_; }
};

class SwitchStatement : public BinaryNode {
  bool hasDefault_; /* only for ParseNodeKind::Switch */

 public:
  SwitchStatement(uint32_t begin, ParseNode* discriminant,
                  LexicalScopeNode* lexicalForCaseList, bool hasDefault)
      : BinaryNode(ParseNodeKind::SwitchStmt,
>>>>>>> upstream-releases
                   TokenPos(begin, lexicalForCaseList->pn_pos.end),
<<<<<<< HEAD
                   discriminant, lexicalForCaseList) {
||||||| merged common ancestors
                   discriminant, lexicalForCaseList)
    {
=======
                   discriminant, lexicalForCaseList),
        hasDefault_(hasDefault) {
>>>>>>> upstream-releases
#ifdef DEBUG
<<<<<<< HEAD
    ListNode* cases = &lexicalForCaseList->scopeBody()->as<ListNode>();
    MOZ_ASSERT(cases->isKind(ParseNodeKind::StatementList));
    bool found = false;
    for (ParseNode* item : cases->contents()) {
      CaseClause* caseNode = &item->as<CaseClause>();
      if (caseNode->isDefault()) {
        found = true;
        break;
      }
    }
    MOZ_ASSERT(found == hasDefault);
#endif

    pn_u.binary.hasDefault = hasDefault;
  }
||||||| merged common ancestors
        ListNode* cases = &lexicalForCaseList->scopeBody()->as<ListNode>();
        MOZ_ASSERT(cases->isKind(ParseNodeKind::StatementList));
        bool found = false;
        for (ParseNode* item : cases->contents()) {
            CaseClause* caseNode = &item->as<CaseClause>();
            if (caseNode->isDefault()) {
                found = true;
                break;
            }
        }
        MOZ_ASSERT(found == hasDefault);
#endif

        pn_u.binary.hasDefault = hasDefault;
    }
=======
    ListNode* cases = &lexicalForCaseList->scopeBody()->as<ListNode>();
    MOZ_ASSERT(cases->isKind(ParseNodeKind::StatementList));
    bool found = false;
    for (ParseNode* item : cases->contents()) {
      CaseClause* caseNode = &item->as<CaseClause>();
      if (caseNode->isDefault()) {
        found = true;
        break;
      }
    }
    MOZ_ASSERT(found == hasDefault);
#endif
  }
>>>>>>> upstream-releases

<<<<<<< HEAD
  static bool test(const ParseNode& node) {
    bool match = node.isKind(ParseNodeKind::Switch);
    MOZ_ASSERT_IF(match, node.is<BinaryNode>());
    return match;
  }
||||||| merged common ancestors
    static bool test(const ParseNode& node) {
        bool match = node.isKind(ParseNodeKind::Switch);
        MOZ_ASSERT_IF(match, node.is<BinaryNode>());
        return match;
    }
=======
  static bool test(const ParseNode& node) {
    bool match = node.isKind(ParseNodeKind::SwitchStmt);
    MOZ_ASSERT_IF(match, node.is<BinaryNode>());
    return match;
  }
>>>>>>> upstream-releases

  ParseNode& discriminant() const { return *left(); }

  LexicalScopeNode& lexicalForCaseList() const {
    return right()->as<LexicalScopeNode>();
  }

<<<<<<< HEAD
  bool hasDefault() const { return pn_u.binary.hasDefault; }
||||||| merged common ancestors
    bool hasDefault() const {
        return pn_u.binary.hasDefault;
    }
=======
  bool hasDefault() const { return hasDefault_; }
>>>>>>> upstream-releases
};

<<<<<<< HEAD
class ClassNames : public BinaryNode {
 public:
  ClassNames(ParseNode* outerBinding, ParseNode* innerBinding,
             const TokenPos& pos)
      : BinaryNode(ParseNodeKind::ClassNames, JSOP_NOP, pos, outerBinding,
                   innerBinding) {
    MOZ_ASSERT_IF(outerBinding, outerBinding->isKind(ParseNodeKind::Name));
    MOZ_ASSERT(innerBinding->isKind(ParseNodeKind::Name));
    MOZ_ASSERT_IF(outerBinding, innerBinding->as<NameNode>().atom() ==
                                    outerBinding->as<NameNode>().atom());
  }
||||||| merged common ancestors
class ClassNames : public BinaryNode
{
  public:
    ClassNames(ParseNode* outerBinding, ParseNode* innerBinding, const TokenPos& pos)
      : BinaryNode(ParseNodeKind::ClassNames, JSOP_NOP, pos, outerBinding, innerBinding)
    {
        MOZ_ASSERT_IF(outerBinding, outerBinding->isKind(ParseNodeKind::Name));
        MOZ_ASSERT(innerBinding->isKind(ParseNodeKind::Name));
        MOZ_ASSERT_IF(outerBinding,
                      innerBinding->as<NameNode>().atom() == outerBinding->as<NameNode>().atom());
    }
=======
class ClassNames : public BinaryNode {
 public:
  ClassNames(ParseNode* outerBinding, ParseNode* innerBinding,
             const TokenPos& pos)
      : BinaryNode(ParseNodeKind::ClassNames, pos, outerBinding, innerBinding) {
    MOZ_ASSERT_IF(outerBinding, outerBinding->isKind(ParseNodeKind::Name));
    MOZ_ASSERT(innerBinding->isKind(ParseNodeKind::Name));
    MOZ_ASSERT_IF(outerBinding, innerBinding->as<NameNode>().atom() ==
                                    outerBinding->as<NameNode>().atom());
  }
>>>>>>> upstream-releases

  static bool test(const ParseNode& node) {
    bool match = node.isKind(ParseNodeKind::ClassNames);
    MOZ_ASSERT_IF(match, node.is<BinaryNode>());
    return match;
  }

  /*
   * Classes require two definitions: The first "outer" binding binds the
   * class into the scope in which it was declared. the outer binding is a
   * mutable lexial binding. The second "inner" binding binds the class by
   * name inside a block in which the methods are evaulated. It is immutable,
   * giving the methods access to the static members of the class even if
   * the outer binding has been overwritten.
   */
  NameNode* outerBinding() const {
    if (ParseNode* binding = left()) {
      return &binding->as<NameNode>();
    }
    return nullptr;
  }

  NameNode* innerBinding() const { return &right()->as<NameNode>(); }
};

<<<<<<< HEAD
class ClassNode : public TernaryNode {
 public:
  ClassNode(ParseNode* names, ParseNode* heritage, ParseNode* membersOrBlock,
            const TokenPos& pos)
      : TernaryNode(ParseNodeKind::Class, names, heritage, membersOrBlock,
                    pos) {
    MOZ_ASSERT_IF(names, names->is<ClassNames>());
    MOZ_ASSERT(membersOrBlock->is<LexicalScopeNode>() ||
               membersOrBlock->isKind(ParseNodeKind::ClassMemberList));
  }
||||||| merged common ancestors
class ClassNode : public TernaryNode
{
  public:
    ClassNode(ParseNode* names, ParseNode* heritage, ParseNode* methodsOrBlock,
              const TokenPos& pos)
      : TernaryNode(ParseNodeKind::Class, names, heritage, methodsOrBlock, pos)
    {
        MOZ_ASSERT_IF(names, names->is<ClassNames>());
        MOZ_ASSERT(methodsOrBlock->is<LexicalScopeNode>() ||
                   methodsOrBlock->isKind(ParseNodeKind::ClassMethodList));
    }
=======
class ClassNode : public TernaryNode {
 public:
  ClassNode(ParseNode* names, ParseNode* heritage,
            LexicalScopeNode* memberBlock, const TokenPos& pos)
      : TernaryNode(ParseNodeKind::ClassDecl, names, heritage, memberBlock,
                    pos) {
    MOZ_ASSERT_IF(names, names->is<ClassNames>());
  }
>>>>>>> upstream-releases

<<<<<<< HEAD
  static bool test(const ParseNode& node) {
    bool match = node.isKind(ParseNodeKind::Class);
    MOZ_ASSERT_IF(match, node.is<TernaryNode>());
    return match;
  }
||||||| merged common ancestors
    static bool test(const ParseNode& node) {
        bool match = node.isKind(ParseNodeKind::Class);
        MOZ_ASSERT_IF(match, node.is<TernaryNode>());
        return match;
    }
=======
  static bool test(const ParseNode& node) {
    bool match = node.isKind(ParseNodeKind::ClassDecl);
    MOZ_ASSERT_IF(match, node.is<TernaryNode>());
    return match;
  }
>>>>>>> upstream-releases

<<<<<<< HEAD
  ClassNames* names() const {
    return kid1() ? &kid1()->as<ClassNames>() : nullptr;
  }
  ParseNode* heritage() const { return kid2(); }
  ListNode* memberList() const {
    ParseNode* membersOrBlock = kid3();
    if (membersOrBlock->isKind(ParseNodeKind::ClassMemberList)) {
      return &membersOrBlock->as<ListNode>();
    }

    ListNode* list =
        &membersOrBlock->as<LexicalScopeNode>().scopeBody()->as<ListNode>();
    MOZ_ASSERT(list->isKind(ParseNodeKind::ClassMemberList));
    return list;
  }
  Handle<LexicalScope::Data*> scopeBindings() const {
    ParseNode* scope = kid3();
    return scope->as<LexicalScopeNode>().scopeBindings();
  }
||||||| merged common ancestors
    ClassNames* names() const {
        return kid1() ? &kid1()->as<ClassNames>() : nullptr;
    }
    ParseNode* heritage() const {
        return kid2();
    }
    ListNode* methodList() const {
        ParseNode* methodsOrBlock = kid3();
        if (methodsOrBlock->isKind(ParseNodeKind::ClassMethodList)) {
            return &methodsOrBlock->as<ListNode>();
        }

        ListNode* list = &methodsOrBlock->as<LexicalScopeNode>().scopeBody()->as<ListNode>();
        MOZ_ASSERT(list->isKind(ParseNodeKind::ClassMethodList));
        return list;
    }
    Handle<LexicalScope::Data*> scopeBindings() const {
        ParseNode* scope = kid3();
        return scope->as<LexicalScopeNode>().scopeBindings();
    }
=======
  ClassNames* names() const {
    return kid1() ? &kid1()->as<ClassNames>() : nullptr;
  }

  ParseNode* heritage() const { return kid2(); }

  ListNode* memberList() const {
    ListNode* list =
        &kid3()->as<LexicalScopeNode>().scopeBody()->as<ListNode>();
    MOZ_ASSERT(list->isKind(ParseNodeKind::ClassMemberList));
    return list;
  }

  LexicalScopeNode* scopeBindings() const {
    LexicalScopeNode* scope = &kid3()->as<LexicalScopeNode>();
    return scope->isEmptyScope() ? nullptr : scope;
  }
>>>>>>> upstream-releases
};

#ifdef DEBUG
void DumpParseTree(ParseNode* pn, GenericPrinter& out, int indent = 0);
#endif

class ParseNodeAllocator {
 public:
  explicit ParseNodeAllocator(JSContext* cx, LifoAlloc& alloc)
      : cx(cx), alloc(alloc) {}

<<<<<<< HEAD
  void* allocNode();
||||||| merged common ancestors
    void* allocNode();
=======
  void* allocNode(size_t size);
>>>>>>> upstream-releases

 private:
  JSContext* cx;
  LifoAlloc& alloc;
};

<<<<<<< HEAD
inline bool ParseNode::isConstant() {
  switch (pn_type) {
    case ParseNodeKind::Number:
    case ParseNodeKind::String:
    case ParseNodeKind::TemplateString:
    case ParseNodeKind::Null:
    case ParseNodeKind::RawUndefined:
    case ParseNodeKind::False:
    case ParseNodeKind::True:
      return true;
    case ParseNodeKind::Array:
    case ParseNodeKind::Object:
      return !as<ListNode>().hasNonConstInitializer();
    default:
      return false;
  }
||||||| merged common ancestors
inline bool
ParseNode::isConstant()
{
    switch (pn_type) {
      case ParseNodeKind::Number:
      case ParseNodeKind::String:
      case ParseNodeKind::TemplateString:
      case ParseNodeKind::Null:
      case ParseNodeKind::RawUndefined:
      case ParseNodeKind::False:
      case ParseNodeKind::True:
        return true;
      case ParseNodeKind::Array:
      case ParseNodeKind::Object:
        return !as<ListNode>().hasNonConstInitializer();
      default:
        return false;
    }
=======
inline bool ParseNode::isConstant() {
  switch (pn_type) {
    case ParseNodeKind::NumberExpr:
    case ParseNodeKind::StringExpr:
    case ParseNodeKind::TemplateStringExpr:
    case ParseNodeKind::NullExpr:
    case ParseNodeKind::RawUndefinedExpr:
    case ParseNodeKind::FalseExpr:
    case ParseNodeKind::TrueExpr:
      return true;
    case ParseNodeKind::ArrayExpr:
    case ParseNodeKind::ObjectExpr:
      return !as<ListNode>().hasNonConstInitializer();
    default:
      return false;
  }
>>>>>>> upstream-releases
}

class TraceListNode {
 protected:
  js::gc::Cell* gcThing;
  TraceListNode* traceLink;

<<<<<<< HEAD
  TraceListNode(js::gc::Cell* gcThing, TraceListNode* traceLink);

#ifdef ENABLE_BIGINT
  bool isBigIntBox() const { return gcThing->is<BigInt>(); }
#endif
  bool isObjectBox() const { return gcThing->is<JSObject>(); }
||||||| merged common ancestors
    ObjectBox(JSObject* object, ObjectBox* traceLink);
    bool isFunctionBox() { return object->is<JSFunction>(); }
    FunctionBox* asFunctionBox();
    virtual void trace(JSTracer* trc);
=======
  TraceListNode(js::gc::Cell* gcThing, TraceListNode* traceLink);
>>>>>>> upstream-releases

<<<<<<< HEAD
#ifdef ENABLE_BIGINT
  BigIntBox* asBigIntBox();
#endif
  ObjectBox* asObjectBox();
||||||| merged common ancestors
    static void TraceList(JSTracer* trc, ObjectBox* listHead);
=======
  bool isBigIntBox() const { return gcThing->is<BigInt>(); }
  bool isObjectBox() const { return gcThing->is<JSObject>(); }
>>>>>>> upstream-releases

<<<<<<< HEAD
  virtual void trace(JSTracer* trc);
||||||| merged common ancestors
  protected:
    friend struct CGObjectList;
=======
  BigIntBox* asBigIntBox();
  ObjectBox* asObjectBox();
>>>>>>> upstream-releases

<<<<<<< HEAD
 public:
  static void TraceList(JSTracer* trc, TraceListNode* listHead);
};
||||||| merged common ancestors
    ObjectBox* traceLink;
    ObjectBox* emitLink;
=======
  virtual void trace(JSTracer* trc);
>>>>>>> upstream-releases

<<<<<<< HEAD
#ifdef ENABLE_BIGINT
class BigIntBox : public TraceListNode {
 public:
  BigIntBox(BigInt* bi, TraceListNode* link);
  BigInt* value() const { return gcThing->as<BigInt>(); }
||||||| merged common ancestors
    ObjectBox(JSFunction* function, ObjectBox* traceLink);
=======
 public:
  static void TraceList(JSTracer* trc, TraceListNode* listHead);
>>>>>>> upstream-releases
};
#endif

class ObjectBox : public TraceListNode {
 protected:
  friend struct CGObjectList;
  ObjectBox* emitLink;

  ObjectBox(JSFunction* function, TraceListNode* link);

 public:
  ObjectBox(JSObject* obj, TraceListNode* link);

<<<<<<< HEAD
  JSObject* object() const { return gcThing->as<JSObject>(); }

  bool isFunctionBox() const { return object()->is<JSFunction>(); }
  FunctionBox* asFunctionBox();
||||||| merged common ancestors
enum ParseReportKind
{
    ParseError,
    ParseWarning,
    ParseExtraWarning,
    ParseStrictError
=======
class BigIntBox : public TraceListNode {
 public:
  BigIntBox(BigInt* bi, TraceListNode* link);
  BigInt* value() const { return gcThing->as<BigInt>(); }
>>>>>>> upstream-releases
};

<<<<<<< HEAD
enum ParseReportKind {
  ParseError,
  ParseWarning,
  ParseExtraWarning,
  ParseStrictError
};
||||||| merged common ancestors
enum class AccessorType {
    None,
    Getter,
    Setter
};
=======
class ObjectBox : public TraceListNode {
 protected:
  friend struct GCThingList;
  ObjectBox* emitLink;
>>>>>>> upstream-releases

<<<<<<< HEAD
enum class AccessorType { None, Getter, Setter };

inline JSOp AccessorTypeToJSOp(AccessorType atype) {
  switch (atype) {
    case AccessorType::None:
      return JSOP_INITPROP;
    case AccessorType::Getter:
      return JSOP_INITPROP_GETTER;
    case AccessorType::Setter:
      return JSOP_INITPROP_SETTER;
    default:
      MOZ_CRASH("unexpected accessor type");
  }
}
||||||| merged common ancestors
inline JSOp
AccessorTypeToJSOp(AccessorType atype)
{
    switch (atype) {
      case AccessorType::None:
        return JSOP_INITPROP;
      case AccessorType::Getter:
        return JSOP_INITPROP_GETTER;
      case AccessorType::Setter:
        return JSOP_INITPROP_SETTER;
      default:
        MOZ_CRASH("unexpected accessor type");
    }
}
=======
  ObjectBox(JSFunction* function, TraceListNode* link);
>>>>>>> upstream-releases

<<<<<<< HEAD
enum class FunctionSyntaxKind {
  // A non-arrow function expression.
  Expression,
||||||| merged common ancestors
enum class FunctionSyntaxKind
{
    // A non-arrow function expression.
    Expression,
=======
 public:
  ObjectBox(JSObject* obj, TraceListNode* link);
>>>>>>> upstream-releases

<<<<<<< HEAD
  // A named function appearing as a Statement.
  Statement,
||||||| merged common ancestors
    // A named function appearing as a Statement.
    Statement,
=======
  JSObject* object() const { return gcThing->as<JSObject>(); }
>>>>>>> upstream-releases

<<<<<<< HEAD
  Arrow,
  Method,
  ClassConstructor,
  DerivedClassConstructor,
  Getter,
  Setter,
||||||| merged common ancestors
    Arrow,
    Method,
    ClassConstructor,
    DerivedClassConstructor,
    Getter,
    Setter,
=======
  bool isFunctionBox() const { return object()->is<JSFunction>(); }
  FunctionBox* asFunctionBox();
>>>>>>> upstream-releases
};

<<<<<<< HEAD
static inline bool IsConstructorKind(FunctionSyntaxKind kind) {
  return kind == FunctionSyntaxKind::ClassConstructor ||
         kind == FunctionSyntaxKind::DerivedClassConstructor;
}

static inline bool IsMethodDefinitionKind(FunctionSyntaxKind kind) {
  return IsConstructorKind(kind) || kind == FunctionSyntaxKind::Method ||
         kind == FunctionSyntaxKind::Getter ||
         kind == FunctionSyntaxKind::Setter;
}
||||||| merged common ancestors
static inline bool
IsConstructorKind(FunctionSyntaxKind kind)
{
    return kind == FunctionSyntaxKind::ClassConstructor ||
           kind == FunctionSyntaxKind::DerivedClassConstructor;
}

static inline bool
IsMethodDefinitionKind(FunctionSyntaxKind kind)
{
    return IsConstructorKind(kind) ||
           kind == FunctionSyntaxKind::Method ||
           kind == FunctionSyntaxKind::Getter ||
           kind == FunctionSyntaxKind::Setter;
}
=======
enum ParseReportKind {
  ParseError,
  ParseWarning,
  ParseExtraWarning,
  ParseStrictError
};
>>>>>>> upstream-releases

<<<<<<< HEAD
static inline ParseNode* FunctionFormalParametersList(ParseNode* fn,
                                                      unsigned* numFormals) {
  MOZ_ASSERT(fn->isKind(ParseNodeKind::Function));
  ListNode* argsBody = fn->as<CodeNode>().body();
  MOZ_ASSERT(argsBody->isKind(ParseNodeKind::ParamsBody));
  *numFormals = argsBody->count();
  if (*numFormals > 0 && argsBody->last()->is<LexicalScopeNode>() &&
      argsBody->last()->as<LexicalScopeNode>().scopeBody()->isKind(
          ParseNodeKind::StatementList)) {
    (*numFormals)--;
  }
  return argsBody->head();
||||||| merged common ancestors
static inline ParseNode*
FunctionFormalParametersList(ParseNode* fn, unsigned* numFormals)
{
    MOZ_ASSERT(fn->isKind(ParseNodeKind::Function));
    ListNode* argsBody = fn->as<CodeNode>().body();
    MOZ_ASSERT(argsBody->isKind(ParseNodeKind::ParamsBody));
    *numFormals = argsBody->count();
    if (*numFormals > 0 &&
        argsBody->last()->is<LexicalScopeNode>() &&
        argsBody->last()->as<LexicalScopeNode>().scopeBody()->isKind(ParseNodeKind::StatementList))
    {
        (*numFormals)--;
    }
    return argsBody->head();
=======
static inline ParseNode* FunctionFormalParametersList(ParseNode* fn,
                                                      unsigned* numFormals) {
  MOZ_ASSERT(fn->isKind(ParseNodeKind::Function));
  ListNode* argsBody = fn->as<FunctionNode>().body();
  MOZ_ASSERT(argsBody->isKind(ParseNodeKind::ParamsBody));
  *numFormals = argsBody->count();
  if (*numFormals > 0 && argsBody->last()->is<LexicalScopeNode>() &&
      argsBody->last()->as<LexicalScopeNode>().scopeBody()->isKind(
          ParseNodeKind::StatementList)) {
    (*numFormals)--;
  }
  return argsBody->head();
>>>>>>> upstream-releases
}

bool IsAnonymousFunctionDefinition(ParseNode* pn);

} /* namespace frontend */
} /* namespace js */

#endif /* frontend_ParseNode_h */
