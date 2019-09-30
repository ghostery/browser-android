/* -*- Mode: C++; tab-width: 8; indent-tabs-mode: nil; c-basic-offset: 2 -*-
 * vim: set ts=8 sts=2 et sw=2 tw=80:
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/. */

#include "frontend/FoldConstants.h"

#include "mozilla/FloatingPoint.h"

#include "jslibmath.h"
#include "jsnum.h"

#include "frontend/ParseNode.h"
#include "frontend/ParseNodeVisitor.h"
#include "frontend/Parser.h"
#include "js/Conversions.h"
#include "vm/StringType.h"

using namespace js;
using namespace js::frontend;

using JS::GenericNaN;
using JS::ToInt32;
using JS::ToUint32;
using mozilla::IsNaN;
using mozilla::IsNegative;
using mozilla::NegativeInfinity;
using mozilla::PositiveInfinity;

<<<<<<< HEAD
static bool ContainsHoistedDeclaration(JSContext* cx, ParseNode* node,
                                       bool* result);
||||||| merged common ancestors
static bool
ContainsHoistedDeclaration(JSContext* cx, ParseNode* node, bool* result);
=======
// Don't use ReplaceNode directly, because we want the constant folder to keep
// the attributes isInParens and isDirectRHSAnonFunction of the old node being
// replaced.
inline MOZ_MUST_USE bool TryReplaceNode(ParseNode** pnp, ParseNode* pn) {
  // convenience check: can call TryReplaceNode(pnp, alloc_parsenode())
  // directly, without having to worry about alloc returning null.
  if (!pn) {
    return false;
  }
  pn->setInParens((*pnp)->isInParens());
  pn->setDirectRHSAnonFunction((*pnp)->isDirectRHSAnonFunction());
  ReplaceNode(pnp, pn);
  return true;
}
>>>>>>> upstream-releases

<<<<<<< HEAD
static bool ListContainsHoistedDeclaration(JSContext* cx, ListNode* list,
                                           bool* result) {
  for (ParseNode* node : list->contents()) {
    if (!ContainsHoistedDeclaration(cx, node, result)) {
      return false;
||||||| merged common ancestors
static bool
ListContainsHoistedDeclaration(JSContext* cx, ListNode* list, bool* result)
{
    for (ParseNode* node : list->contents()) {
        if (!ContainsHoistedDeclaration(cx, node, result)) {
            return false;
        }
        if (*result) {
            return true;
        }
=======
static bool ContainsHoistedDeclaration(JSContext* cx, ParseNode* node,
                                       bool* result);

static bool ListContainsHoistedDeclaration(JSContext* cx, ListNode* list,
                                           bool* result) {
  for (ParseNode* node : list->contents()) {
    if (!ContainsHoistedDeclaration(cx, node, result)) {
      return false;
>>>>>>> upstream-releases
    }
    if (*result) {
      return true;
    }
  }

  *result = false;
  return true;
}

// Determines whether the given ParseNode contains any declarations whose
// visibility will extend outside the node itself -- that is, whether the
// ParseNode contains any var statements.
//
// THIS IS NOT A GENERAL-PURPOSE FUNCTION.  It is only written to work in the
// specific context of deciding that |node|, as one arm of a ParseNodeKind::If
// controlled by a constant condition, contains a declaration that forbids
// |node| being completely eliminated as dead.
<<<<<<< HEAD
static bool ContainsHoistedDeclaration(JSContext* cx, ParseNode* node,
                                       bool* result) {
  if (!CheckRecursionLimit(cx)) {
    return false;
  }

restart:

  // With a better-typed AST, we would have distinct parse node classes for
  // expressions and for statements and would characterize expressions with
  // ExpressionKind and statements with StatementKind.  Perhaps someday.  In
  // the meantime we must characterize every ParseNodeKind, even the
  // expression/sub-expression ones that, if we handle all statement kinds
  // correctly, we'll never see.
  switch (node->getKind()) {
    // Base case.
    case ParseNodeKind::Var:
      *result = true;
      return true;

    // Non-global lexical declarations are block-scoped (ergo not hoistable).
    case ParseNodeKind::Let:
    case ParseNodeKind::Const:
      MOZ_ASSERT(node->is<ListNode>());
      *result = false;
      return true;

    // Similarly to the lexical declarations above, classes cannot add hoisted
    // declarations
    case ParseNodeKind::Class:
      MOZ_ASSERT(node->is<ClassNode>());
      *result = false;
      return true;

    // Function declarations *can* be hoisted declarations.  But in the
    // magical world of the rewritten frontend, the declaration necessitated
    // by a nested function statement, not at body level, doesn't require
    // that we preserve an unreachable function declaration node against
    // dead-code removal.
    case ParseNodeKind::Function:
      MOZ_ASSERT(node->is<CodeNode>());
      *result = false;
      return true;

    case ParseNodeKind::Module:
      *result = false;
      return true;

    // Statements with no sub-components at all.
    case ParseNodeKind::EmptyStatement:
      MOZ_ASSERT(node->is<NullaryNode>());
      *result = false;
      return true;

    case ParseNodeKind::Debugger:
      MOZ_ASSERT(node->is<DebuggerStatement>());
      *result = false;
      return true;

    // Statements containing only an expression have no declarations.
    case ParseNodeKind::ExpressionStatement:
    case ParseNodeKind::Throw:
    case ParseNodeKind::Return:
      MOZ_ASSERT(node->is<UnaryNode>());
      *result = false;
      return true;

    // These two aren't statements in the spec, but we sometimes insert them
    // in statement lists anyway.
    case ParseNodeKind::InitialYield:
    case ParseNodeKind::YieldStar:
    case ParseNodeKind::Yield:
      MOZ_ASSERT(node->is<UnaryNode>());
      *result = false;
      return true;

    // Other statements with no sub-statement components.
    case ParseNodeKind::Break:
    case ParseNodeKind::Continue:
    case ParseNodeKind::Import:
    case ParseNodeKind::ImportSpecList:
    case ParseNodeKind::ImportSpec:
    case ParseNodeKind::ExportFrom:
    case ParseNodeKind::ExportDefault:
    case ParseNodeKind::ExportSpecList:
    case ParseNodeKind::ExportSpec:
    case ParseNodeKind::Export:
    case ParseNodeKind::ExportBatchSpec:
    case ParseNodeKind::CallImport:
      *result = false;
      return true;

    // Statements possibly containing hoistable declarations only in the left
    // half, in ParseNode terms -- the loop body in AST terms.
    case ParseNodeKind::DoWhile:
      return ContainsHoistedDeclaration(cx, node->as<BinaryNode>().left(),
                                        result);

    // Statements possibly containing hoistable declarations only in the
    // right half, in ParseNode terms -- the loop body or nested statement
    // (usually a block statement), in AST terms.
    case ParseNodeKind::While:
    case ParseNodeKind::With:
      return ContainsHoistedDeclaration(cx, node->as<BinaryNode>().right(),
                                        result);

    case ParseNodeKind::Label:
      return ContainsHoistedDeclaration(
          cx, node->as<LabeledStatement>().statement(), result);

    // Statements with more complicated structures.

    // if-statement nodes may have hoisted declarations in their consequent
    // and alternative components.
    case ParseNodeKind::If: {
      TernaryNode* ifNode = &node->as<TernaryNode>();
      ParseNode* consequent = ifNode->kid2();
      if (!ContainsHoistedDeclaration(cx, consequent, result)) {
||||||| merged common ancestors
static bool
ContainsHoistedDeclaration(JSContext* cx, ParseNode* node, bool* result)
{
    if (!CheckRecursionLimit(cx)) {
=======
static bool ContainsHoistedDeclaration(JSContext* cx, ParseNode* node,
                                       bool* result) {
  if (!CheckRecursionLimit(cx)) {
    return false;
  }

restart:

  // With a better-typed AST, we would have distinct parse node classes for
  // expressions and for statements and would characterize expressions with
  // ExpressionKind and statements with StatementKind.  Perhaps someday.  In
  // the meantime we must characterize every ParseNodeKind, even the
  // expression/sub-expression ones that, if we handle all statement kinds
  // correctly, we'll never see.
  switch (node->getKind()) {
    // Base case.
    case ParseNodeKind::VarStmt:
      *result = true;
      return true;

    // Non-global lexical declarations are block-scoped (ergo not hoistable).
    case ParseNodeKind::LetDecl:
    case ParseNodeKind::ConstDecl:
      MOZ_ASSERT(node->is<ListNode>());
      *result = false;
      return true;

    // Similarly to the lexical declarations above, classes cannot add hoisted
    // declarations
    case ParseNodeKind::ClassDecl:
      MOZ_ASSERT(node->is<ClassNode>());
      *result = false;
      return true;

    // Function declarations *can* be hoisted declarations.  But in the
    // magical world of the rewritten frontend, the declaration necessitated
    // by a nested function statement, not at body level, doesn't require
    // that we preserve an unreachable function declaration node against
    // dead-code removal.
    case ParseNodeKind::Function:
      *result = false;
      return true;

    case ParseNodeKind::Module:
      *result = false;
      return true;

    // Statements with no sub-components at all.
    case ParseNodeKind::EmptyStmt:
      MOZ_ASSERT(node->is<NullaryNode>());
      *result = false;
      return true;

    case ParseNodeKind::DebuggerStmt:
      MOZ_ASSERT(node->is<DebuggerStatement>());
      *result = false;
      return true;

    // Statements containing only an expression have no declarations.
    case ParseNodeKind::ExpressionStmt:
    case ParseNodeKind::ThrowStmt:
    case ParseNodeKind::ReturnStmt:
      MOZ_ASSERT(node->is<UnaryNode>());
      *result = false;
      return true;

    // These two aren't statements in the spec, but we sometimes insert them
    // in statement lists anyway.
    case ParseNodeKind::InitialYield:
    case ParseNodeKind::YieldStarExpr:
    case ParseNodeKind::YieldExpr:
      MOZ_ASSERT(node->is<UnaryNode>());
      *result = false;
      return true;

    // Other statements with no sub-statement components.
    case ParseNodeKind::BreakStmt:
    case ParseNodeKind::ContinueStmt:
    case ParseNodeKind::ImportDecl:
    case ParseNodeKind::ImportSpecList:
    case ParseNodeKind::ImportSpec:
    case ParseNodeKind::ExportFromStmt:
    case ParseNodeKind::ExportDefaultStmt:
    case ParseNodeKind::ExportSpecList:
    case ParseNodeKind::ExportSpec:
    case ParseNodeKind::ExportStmt:
    case ParseNodeKind::ExportBatchSpecStmt:
    case ParseNodeKind::CallImportExpr:
      *result = false;
      return true;

    // Statements possibly containing hoistable declarations only in the left
    // half, in ParseNode terms -- the loop body in AST terms.
    case ParseNodeKind::DoWhileStmt:
      return ContainsHoistedDeclaration(cx, node->as<BinaryNode>().left(),
                                        result);

    // Statements possibly containing hoistable declarations only in the
    // right half, in ParseNode terms -- the loop body or nested statement
    // (usually a block statement), in AST terms.
    case ParseNodeKind::WhileStmt:
    case ParseNodeKind::WithStmt:
      return ContainsHoistedDeclaration(cx, node->as<BinaryNode>().right(),
                                        result);

    case ParseNodeKind::LabelStmt:
      return ContainsHoistedDeclaration(
          cx, node->as<LabeledStatement>().statement(), result);

    // Statements with more complicated structures.

    // if-statement nodes may have hoisted declarations in their consequent
    // and alternative components.
    case ParseNodeKind::IfStmt: {
      TernaryNode* ifNode = &node->as<TernaryNode>();
      ParseNode* consequent = ifNode->kid2();
      if (!ContainsHoistedDeclaration(cx, consequent, result)) {
>>>>>>> upstream-releases
        return false;
      }
      if (*result) {
        return true;
      }

      if ((node = ifNode->kid3())) {
        goto restart;
      }

      *result = false;
      return true;
    }

<<<<<<< HEAD
    // try-statements have statements to execute, and one or both of a
    // catch-list and a finally-block.
    case ParseNodeKind::Try: {
      TernaryNode* tryNode = &node->as<TernaryNode>();
||||||| merged common ancestors
      case ParseNodeKind::Debugger:
        MOZ_ASSERT(node->is<DebuggerStatement>());
        *result = false;
        return true;
=======
    // try-statements have statements to execute, and one or both of a
    // catch-list and a finally-block.
    case ParseNodeKind::TryStmt: {
      TernaryNode* tryNode = &node->as<TernaryNode>();
>>>>>>> upstream-releases

      MOZ_ASSERT(tryNode->kid2() || tryNode->kid3(),
                 "must have either catch or finally");

      ParseNode* tryBlock = tryNode->kid1();
      if (!ContainsHoistedDeclaration(cx, tryBlock, result)) {
        return false;
      }
      if (*result) {
        return true;
      }

      if (ParseNode* catchScope = tryNode->kid2()) {
        BinaryNode* catchNode =
            &catchScope->as<LexicalScopeNode>().scopeBody()->as<BinaryNode>();
        MOZ_ASSERT(catchNode->isKind(ParseNodeKind::Catch));

        ParseNode* catchStatements = catchNode->right();
        if (!ContainsHoistedDeclaration(cx, catchStatements, result)) {
          return false;
        }
        if (*result) {
          return true;
        }
      }

      if (ParseNode* finallyBlock = tryNode->kid3()) {
        return ContainsHoistedDeclaration(cx, finallyBlock, result);
      }

      *result = false;
      return true;
    }

<<<<<<< HEAD
    // A switch node's left half is an expression; only its right half (a
    // list of cases/defaults, or a block node) could contain hoisted
    // declarations.
    case ParseNodeKind::Switch: {
      SwitchStatement* switchNode = &node->as<SwitchStatement>();
      return ContainsHoistedDeclaration(cx, &switchNode->lexicalForCaseList(),
                                        result);
    }
||||||| merged common ancestors
        ParseNode* tryBlock = tryNode->kid1();
        if (!ContainsHoistedDeclaration(cx, tryBlock, result)) {
            return false;
        }
        if (*result) {
            return true;
        }
=======
    // A switch node's left half is an expression; only its right half (a
    // list of cases/defaults, or a block node) could contain hoisted
    // declarations.
    case ParseNodeKind::SwitchStmt: {
      SwitchStatement* switchNode = &node->as<SwitchStatement>();
      return ContainsHoistedDeclaration(cx, &switchNode->lexicalForCaseList(),
                                        result);
    }
>>>>>>> upstream-releases

    case ParseNodeKind::Case: {
      CaseClause* caseClause = &node->as<CaseClause>();
      return ContainsHoistedDeclaration(cx, caseClause->statementList(),
                                        result);
    }

<<<<<<< HEAD
    case ParseNodeKind::For: {
      ForNode* forNode = &node->as<ForNode>();
      TernaryNode* loopHead = forNode->head();
      MOZ_ASSERT(loopHead->isKind(ParseNodeKind::ForHead) ||
                 loopHead->isKind(ParseNodeKind::ForIn) ||
                 loopHead->isKind(ParseNodeKind::ForOf));

      if (loopHead->isKind(ParseNodeKind::ForHead)) {
        // for (init?; cond?; update?), with only init possibly containing
        // a hoisted declaration.  (Note: a lexical-declaration |init| is
        // (at present) hoisted in SpiderMonkey parlance -- but such
        // hoisting doesn't extend outside of this statement, so it is not
        // hoisting in the sense meant by ContainsHoistedDeclaration.)
        ParseNode* init = loopHead->kid1();
        if (init && init->isKind(ParseNodeKind::Var)) {
          *result = true;
          return true;
||||||| merged common ancestors
        if (ParseNode* finallyBlock = tryNode->kid3()) {
            return ContainsHoistedDeclaration(cx, finallyBlock, result);
=======
    case ParseNodeKind::ForStmt: {
      ForNode* forNode = &node->as<ForNode>();
      TernaryNode* loopHead = forNode->head();
      MOZ_ASSERT(loopHead->isKind(ParseNodeKind::ForHead) ||
                 loopHead->isKind(ParseNodeKind::ForIn) ||
                 loopHead->isKind(ParseNodeKind::ForOf));

      if (loopHead->isKind(ParseNodeKind::ForHead)) {
        // for (init?; cond?; update?), with only init possibly containing
        // a hoisted declaration.  (Note: a lexical-declaration |init| is
        // (at present) hoisted in SpiderMonkey parlance -- but such
        // hoisting doesn't extend outside of this statement, so it is not
        // hoisting in the sense meant by ContainsHoistedDeclaration.)
        ParseNode* init = loopHead->kid1();
        if (init && init->isKind(ParseNodeKind::VarStmt)) {
          *result = true;
          return true;
>>>>>>> upstream-releases
        }
      } else {
        MOZ_ASSERT(loopHead->isKind(ParseNodeKind::ForIn) ||
                   loopHead->isKind(ParseNodeKind::ForOf));

<<<<<<< HEAD
        // for each? (target in ...), where only target may introduce
        // hoisted declarations.
        //
        //   -- or --
        //
        // for (target of ...), where only target may introduce hoisted
        // declarations.
        //
        // Either way, if |target| contains a declaration, it's |loopHead|'s
        // first kid.
        ParseNode* decl = loopHead->kid1();
        if (decl && decl->isKind(ParseNodeKind::Var)) {
          *result = true;
          return true;
||||||| merged common ancestors
        if (loopHead->isKind(ParseNodeKind::ForHead)) {
            // for (init?; cond?; update?), with only init possibly containing
            // a hoisted declaration.  (Note: a lexical-declaration |init| is
            // (at present) hoisted in SpiderMonkey parlance -- but such
            // hoisting doesn't extend outside of this statement, so it is not
            // hoisting in the sense meant by ContainsHoistedDeclaration.)
            ParseNode* init = loopHead->kid1();
            if (init && init->isKind(ParseNodeKind::Var)) {
                *result = true;
                return true;
            }
        } else {
            MOZ_ASSERT(loopHead->isKind(ParseNodeKind::ForIn) ||
                       loopHead->isKind(ParseNodeKind::ForOf));

            // for each? (target in ...), where only target may introduce
            // hoisted declarations.
            //
            //   -- or --
            //
            // for (target of ...), where only target may introduce hoisted
            // declarations.
            //
            // Either way, if |target| contains a declaration, it's |loopHead|'s
            // first kid.
            ParseNode* decl = loopHead->kid1();
            if (decl && decl->isKind(ParseNodeKind::Var)) {
                *result = true;
                return true;
            }
=======
        // for each? (target in ...), where only target may introduce
        // hoisted declarations.
        //
        //   -- or --
        //
        // for (target of ...), where only target may introduce hoisted
        // declarations.
        //
        // Either way, if |target| contains a declaration, it's |loopHead|'s
        // first kid.
        ParseNode* decl = loopHead->kid1();
        if (decl && decl->isKind(ParseNodeKind::VarStmt)) {
          *result = true;
          return true;
>>>>>>> upstream-releases
        }
      }

      ParseNode* loopBody = forNode->body();
      return ContainsHoistedDeclaration(cx, loopBody, result);
    }

    case ParseNodeKind::LexicalScope: {
      LexicalScopeNode* scope = &node->as<LexicalScopeNode>();
      ParseNode* expr = scope->scopeBody();

<<<<<<< HEAD
      if (expr->isKind(ParseNodeKind::For) ||
          expr->isKind(ParseNodeKind::Function)) {
        return ContainsHoistedDeclaration(cx, expr, result);
||||||| merged common ancestors
        MOZ_ASSERT(expr->isKind(ParseNodeKind::StatementList));
        return ListContainsHoistedDeclaration(cx, &scope->scopeBody()->as<ListNode>(), result);
=======
      if (expr->isKind(ParseNodeKind::ForStmt) || expr->is<FunctionNode>()) {
        return ContainsHoistedDeclaration(cx, expr, result);
>>>>>>> upstream-releases
      }

      MOZ_ASSERT(expr->isKind(ParseNodeKind::StatementList));
      return ListContainsHoistedDeclaration(
          cx, &scope->scopeBody()->as<ListNode>(), result);
    }

<<<<<<< HEAD
    // List nodes with all non-null children.
    case ParseNodeKind::StatementList:
      return ListContainsHoistedDeclaration(cx, &node->as<ListNode>(), result);

    // Grammar sub-components that should never be reached directly by this
    // method, because some parent component should have asserted itself.
    case ParseNodeKind::ObjectPropertyName:
    case ParseNodeKind::ComputedName:
    case ParseNodeKind::Spread:
    case ParseNodeKind::MutateProto:
    case ParseNodeKind::Colon:
    case ParseNodeKind::Shorthand:
    case ParseNodeKind::Conditional:
    case ParseNodeKind::TypeOfName:
    case ParseNodeKind::TypeOfExpr:
    case ParseNodeKind::Await:
    case ParseNodeKind::Void:
    case ParseNodeKind::Not:
    case ParseNodeKind::BitNot:
    case ParseNodeKind::DeleteName:
    case ParseNodeKind::DeleteProp:
    case ParseNodeKind::DeleteElem:
    case ParseNodeKind::DeleteExpr:
    case ParseNodeKind::Pos:
    case ParseNodeKind::Neg:
    case ParseNodeKind::PreIncrement:
    case ParseNodeKind::PostIncrement:
    case ParseNodeKind::PreDecrement:
    case ParseNodeKind::PostDecrement:
    case ParseNodeKind::Or:
    case ParseNodeKind::And:
    case ParseNodeKind::BitOr:
    case ParseNodeKind::BitXor:
    case ParseNodeKind::BitAnd:
    case ParseNodeKind::StrictEq:
    case ParseNodeKind::Eq:
    case ParseNodeKind::StrictNe:
    case ParseNodeKind::Ne:
    case ParseNodeKind::Lt:
    case ParseNodeKind::Le:
    case ParseNodeKind::Gt:
    case ParseNodeKind::Ge:
    case ParseNodeKind::InstanceOf:
    case ParseNodeKind::In:
    case ParseNodeKind::Lsh:
    case ParseNodeKind::Rsh:
    case ParseNodeKind::Ursh:
    case ParseNodeKind::Add:
    case ParseNodeKind::Sub:
    case ParseNodeKind::Star:
    case ParseNodeKind::Div:
    case ParseNodeKind::Mod:
    case ParseNodeKind::Pow:
    case ParseNodeKind::Assign:
    case ParseNodeKind::AddAssign:
    case ParseNodeKind::SubAssign:
    case ParseNodeKind::BitOrAssign:
    case ParseNodeKind::BitXorAssign:
    case ParseNodeKind::BitAndAssign:
    case ParseNodeKind::LshAssign:
    case ParseNodeKind::RshAssign:
    case ParseNodeKind::UrshAssign:
    case ParseNodeKind::MulAssign:
    case ParseNodeKind::DivAssign:
    case ParseNodeKind::ModAssign:
    case ParseNodeKind::PowAssign:
    case ParseNodeKind::Comma:
    case ParseNodeKind::Array:
    case ParseNodeKind::Object:
    case ParseNodeKind::PropertyName:
    case ParseNodeKind::Dot:
    case ParseNodeKind::Elem:
    case ParseNodeKind::Arguments:
    case ParseNodeKind::Call:
    case ParseNodeKind::Name:
    case ParseNodeKind::PrivateName:
    case ParseNodeKind::TemplateString:
    case ParseNodeKind::TemplateStringList:
    case ParseNodeKind::TaggedTemplate:
    case ParseNodeKind::CallSiteObj:
    case ParseNodeKind::String:
    case ParseNodeKind::RegExp:
    case ParseNodeKind::True:
    case ParseNodeKind::False:
    case ParseNodeKind::Null:
    case ParseNodeKind::RawUndefined:
    case ParseNodeKind::This:
    case ParseNodeKind::Elision:
    case ParseNodeKind::Number:
#ifdef ENABLE_BIGINT
    case ParseNodeKind::BigInt:
#endif
    case ParseNodeKind::New:
    case ParseNodeKind::Generator:
    case ParseNodeKind::ParamsBody:
    case ParseNodeKind::Catch:
    case ParseNodeKind::ForIn:
    case ParseNodeKind::ForOf:
    case ParseNodeKind::ForHead:
    case ParseNodeKind::ClassMethod:
    case ParseNodeKind::ClassField:
    case ParseNodeKind::ClassMemberList:
    case ParseNodeKind::ClassNames:
    case ParseNodeKind::NewTarget:
    case ParseNodeKind::ImportMeta:
    case ParseNodeKind::PosHolder:
    case ParseNodeKind::SuperCall:
    case ParseNodeKind::SuperBase:
    case ParseNodeKind::SetThis:
      MOZ_CRASH(
          "ContainsHoistedDeclaration should have indicated false on "
          "some parent node without recurring to test this node");

    case ParseNodeKind::Pipeline:
      MOZ_ASSERT(node->is<ListNode>());
      *result = false;
      return true;

    case ParseNodeKind::Limit:  // invalid sentinel value
      MOZ_CRASH("unexpected ParseNodeKind::Limit in node");
  }

  MOZ_CRASH("invalid node kind");
||||||| merged common ancestors
    MOZ_CRASH("invalid node kind");
=======
    // List nodes with all non-null children.
    case ParseNodeKind::StatementList:
      return ListContainsHoistedDeclaration(cx, &node->as<ListNode>(), result);

    // Grammar sub-components that should never be reached directly by this
    // method, because some parent component should have asserted itself.
    case ParseNodeKind::ObjectPropertyName:
    case ParseNodeKind::ComputedName:
    case ParseNodeKind::Spread:
    case ParseNodeKind::MutateProto:
    case ParseNodeKind::PropertyDefinition:
    case ParseNodeKind::Shorthand:
    case ParseNodeKind::ConditionalExpr:
    case ParseNodeKind::TypeOfNameExpr:
    case ParseNodeKind::TypeOfExpr:
    case ParseNodeKind::AwaitExpr:
    case ParseNodeKind::VoidExpr:
    case ParseNodeKind::NotExpr:
    case ParseNodeKind::BitNotExpr:
    case ParseNodeKind::DeleteNameExpr:
    case ParseNodeKind::DeletePropExpr:
    case ParseNodeKind::DeleteElemExpr:
    case ParseNodeKind::DeleteExpr:
    case ParseNodeKind::PosExpr:
    case ParseNodeKind::NegExpr:
    case ParseNodeKind::PreIncrementExpr:
    case ParseNodeKind::PostIncrementExpr:
    case ParseNodeKind::PreDecrementExpr:
    case ParseNodeKind::PostDecrementExpr:
    case ParseNodeKind::OrExpr:
    case ParseNodeKind::AndExpr:
    case ParseNodeKind::BitOrExpr:
    case ParseNodeKind::BitXorExpr:
    case ParseNodeKind::BitAndExpr:
    case ParseNodeKind::StrictEqExpr:
    case ParseNodeKind::EqExpr:
    case ParseNodeKind::StrictNeExpr:
    case ParseNodeKind::NeExpr:
    case ParseNodeKind::LtExpr:
    case ParseNodeKind::LeExpr:
    case ParseNodeKind::GtExpr:
    case ParseNodeKind::GeExpr:
    case ParseNodeKind::InstanceOfExpr:
    case ParseNodeKind::InExpr:
    case ParseNodeKind::LshExpr:
    case ParseNodeKind::RshExpr:
    case ParseNodeKind::UrshExpr:
    case ParseNodeKind::AddExpr:
    case ParseNodeKind::SubExpr:
    case ParseNodeKind::MulExpr:
    case ParseNodeKind::DivExpr:
    case ParseNodeKind::ModExpr:
    case ParseNodeKind::PowExpr:
    case ParseNodeKind::InitExpr:
    case ParseNodeKind::AssignExpr:
    case ParseNodeKind::AddAssignExpr:
    case ParseNodeKind::SubAssignExpr:
    case ParseNodeKind::BitOrAssignExpr:
    case ParseNodeKind::BitXorAssignExpr:
    case ParseNodeKind::BitAndAssignExpr:
    case ParseNodeKind::LshAssignExpr:
    case ParseNodeKind::RshAssignExpr:
    case ParseNodeKind::UrshAssignExpr:
    case ParseNodeKind::MulAssignExpr:
    case ParseNodeKind::DivAssignExpr:
    case ParseNodeKind::ModAssignExpr:
    case ParseNodeKind::PowAssignExpr:
    case ParseNodeKind::CommaExpr:
    case ParseNodeKind::ArrayExpr:
    case ParseNodeKind::ObjectExpr:
    case ParseNodeKind::PropertyNameExpr:
    case ParseNodeKind::DotExpr:
    case ParseNodeKind::ElemExpr:
    case ParseNodeKind::Arguments:
    case ParseNodeKind::CallExpr:
    case ParseNodeKind::Name:
    case ParseNodeKind::PrivateName:
    case ParseNodeKind::TemplateStringExpr:
    case ParseNodeKind::TemplateStringListExpr:
    case ParseNodeKind::TaggedTemplateExpr:
    case ParseNodeKind::CallSiteObj:
    case ParseNodeKind::StringExpr:
    case ParseNodeKind::RegExpExpr:
    case ParseNodeKind::TrueExpr:
    case ParseNodeKind::FalseExpr:
    case ParseNodeKind::NullExpr:
    case ParseNodeKind::RawUndefinedExpr:
    case ParseNodeKind::ThisExpr:
    case ParseNodeKind::Elision:
    case ParseNodeKind::NumberExpr:
    case ParseNodeKind::BigIntExpr:
    case ParseNodeKind::NewExpr:
    case ParseNodeKind::Generator:
    case ParseNodeKind::ParamsBody:
    case ParseNodeKind::Catch:
    case ParseNodeKind::ForIn:
    case ParseNodeKind::ForOf:
    case ParseNodeKind::ForHead:
    case ParseNodeKind::ClassMethod:
    case ParseNodeKind::ClassField:
    case ParseNodeKind::ClassMemberList:
    case ParseNodeKind::ClassNames:
    case ParseNodeKind::NewTargetExpr:
    case ParseNodeKind::ImportMetaExpr:
    case ParseNodeKind::PosHolder:
    case ParseNodeKind::SuperCallExpr:
    case ParseNodeKind::SuperBase:
    case ParseNodeKind::SetThis:
      MOZ_CRASH(
          "ContainsHoistedDeclaration should have indicated false on "
          "some parent node without recurring to test this node");

    case ParseNodeKind::PipelineExpr:
      MOZ_ASSERT(node->is<ListNode>());
      *result = false;
      return true;

    case ParseNodeKind::LastUnused:
    case ParseNodeKind::Limit:
      MOZ_CRASH("unexpected sentinel ParseNodeKind in node");
  }

  MOZ_CRASH("invalid node kind");
>>>>>>> upstream-releases
}

/*
 * Fold from one constant type to another.
 * XXX handles only strings and numbers for now
 */
<<<<<<< HEAD
static bool FoldType(JSContext* cx, ParseNode* pn, ParseNodeKind kind) {
  if (!pn->isKind(kind)) {
    switch (kind) {
      case ParseNodeKind::Number:
        if (pn->isKind(ParseNodeKind::String)) {
          double d;
          if (!StringToNumber(cx, pn->as<NameNode>().atom(), &d)) {
            return false;
          }
          pn->setKind(ParseNodeKind::Number);
          pn->setOp(JSOP_DOUBLE);
          pn->as<NumericLiteral>().setValue(d);
        }
        break;

      case ParseNodeKind::String:
        if (pn->isKind(ParseNodeKind::Number)) {
          JSAtom* atom = NumberToAtom(cx, pn->as<NumericLiteral>().value());
          if (!atom) {
            return false;
          }
          pn->setKind(ParseNodeKind::String);
          pn->setOp(JSOP_STRING);
          pn->as<NameNode>().setAtom(atom);
||||||| merged common ancestors
static bool
FoldType(JSContext* cx, ParseNode* pn, ParseNodeKind kind)
{
    if (!pn->isKind(kind)) {
        switch (kind) {
          case ParseNodeKind::Number:
            if (pn->isKind(ParseNodeKind::String)) {
                double d;
                if (!StringToNumber(cx, pn->as<NameNode>().atom(), &d)) {
                    return false;
                }
                pn->setKind(ParseNodeKind::Number);
                pn->setArity(PN_NUMBER);
                pn->setOp(JSOP_DOUBLE);
                pn->as<NumericLiteral>().setValue(d);
            }
            break;

          case ParseNodeKind::String:
            if (pn->isKind(ParseNodeKind::Number)) {
                JSAtom* atom = NumberToAtom(cx, pn->as<NumericLiteral>().value());
                if (!atom) {
                    return false;
                }
                pn->setKind(ParseNodeKind::String);
                pn->setArity(PN_NAME);
                pn->setOp(JSOP_STRING);
                pn->as<NameNode>().setAtom(atom);
            }
            break;

          default:;
=======
static bool FoldType(JSContext* cx, FullParseHandler* handler, ParseNode** pnp,
                     ParseNodeKind kind) {
  const ParseNode* pn = *pnp;
  if (!pn->isKind(kind)) {
    switch (kind) {
      case ParseNodeKind::NumberExpr:
        if (pn->isKind(ParseNodeKind::StringExpr)) {
          double d;
          if (!StringToNumber(cx, pn->as<NameNode>().atom(), &d)) {
            return false;
          }
          if (!TryReplaceNode(pnp,
                              handler->newNumber(d, NoDecimal, pn->pn_pos))) {
            return false;
          }
>>>>>>> upstream-releases
        }
<<<<<<< HEAD
        break;

      default:;
    }
  }
  return true;
}

// Remove a ParseNode, **pnp, from a parse tree, putting another ParseNode,
// *pn, in its place.
//
// pnp points to a ParseNode pointer. This must be the only pointer that points
// to the parse node being replaced. The replacement, *pn, is unchanged except
// for its pn_next pointer; updating that is necessary if *pn's new parent is a
// list node.
static void ReplaceNode(ParseNode** pnp, ParseNode* pn) {
  pn->pn_next = (*pnp)->pn_next;
  *pnp = pn;
}

static bool IsEffectless(ParseNode* node) {
  return node->isKind(ParseNodeKind::True) ||
         node->isKind(ParseNodeKind::False) ||
         node->isKind(ParseNodeKind::String) ||
         node->isKind(ParseNodeKind::TemplateString) ||
         node->isKind(ParseNodeKind::Number) ||
#ifdef ENABLE_BIGINT
         node->isKind(ParseNodeKind::BigInt) ||
#endif
         node->isKind(ParseNodeKind::Null) ||
         node->isKind(ParseNodeKind::RawUndefined) ||
         node->isKind(ParseNodeKind::Function);
}

enum Truthiness { Truthy, Falsy, Unknown };
||||||| merged common ancestors
    }
    return true;
}

// Remove a ParseNode, **pnp, from a parse tree, putting another ParseNode,
// *pn, in its place.
//
// pnp points to a ParseNode pointer. This must be the only pointer that points
// to the parse node being replaced. The replacement, *pn, is unchanged except
// for its pn_next pointer; updating that is necessary if *pn's new parent is a
// list node.
static void
ReplaceNode(ParseNode** pnp, ParseNode* pn)
{
    pn->pn_next = (*pnp)->pn_next;
    *pnp = pn;
}

static bool
IsEffectless(ParseNode* node)
{
    return node->isKind(ParseNodeKind::True) ||
           node->isKind(ParseNodeKind::False) ||
           node->isKind(ParseNodeKind::String) ||
           node->isKind(ParseNodeKind::TemplateString) ||
           node->isKind(ParseNodeKind::Number) ||
           node->isKind(ParseNodeKind::Null) ||
           node->isKind(ParseNodeKind::RawUndefined) ||
           node->isKind(ParseNodeKind::Function);
}

enum Truthiness { Truthy, Falsy, Unknown };
=======
        break;
>>>>>>> upstream-releases

<<<<<<< HEAD
static Truthiness Boolish(ParseNode* pn) {
  switch (pn->getKind()) {
    case ParseNodeKind::Number:
      return (pn->as<NumericLiteral>().value() != 0 &&
              !IsNaN(pn->as<NumericLiteral>().value()))
                 ? Truthy
                 : Falsy;

#ifdef ENABLE_BIGINT
    case ParseNodeKind::BigInt:
      return (pn->as<BigIntLiteral>().box()->value()->toBoolean()) ? Truthy
                                                                   : Falsy;
#endif
||||||| merged common ancestors
static Truthiness
Boolish(ParseNode* pn)
{
    switch (pn->getKind()) {
      case ParseNodeKind::Number:
        return (pn->as<NumericLiteral>().value() != 0 && !IsNaN(pn->as<NumericLiteral>().value())) ? Truthy : Falsy;

      case ParseNodeKind::String:
      case ParseNodeKind::TemplateString:
        return (pn->as<NameNode>().atom()->length() > 0) ? Truthy : Falsy;

      case ParseNodeKind::True:
      case ParseNodeKind::Function:
        return Truthy;

      case ParseNodeKind::False:
      case ParseNodeKind::Null:
      case ParseNodeKind::RawUndefined:
        return Falsy;

      case ParseNodeKind::Void: {
        // |void <foo>| evaluates to |undefined| which isn't truthy.  But the
        // sense of this method requires that the expression be literally
        // replaceable with true/false: not the case if the nested expression
        // is effectful, might throw, &c.  Walk past the |void| (and nested
        // |void| expressions, for good measure) and check that the nested
        // expression doesn't break this requirement before indicating falsity.
        do {
            pn = pn->as<UnaryNode>().kid();
        } while (pn->isKind(ParseNodeKind::Void));

        return IsEffectless(pn) ? Falsy : Unknown;
      }

      default:
        return Unknown;
    }
}

static bool
Fold(JSContext* cx, ParseNode** pnp, PerHandlerParser<FullParseHandler>& parser);

static bool
FoldCondition(JSContext* cx, ParseNode** nodePtr, PerHandlerParser<FullParseHandler>& parser)
{
    // Conditions fold like any other expression...
    if (!Fold(cx, nodePtr, parser)) {
        return false;
    }
=======
      case ParseNodeKind::StringExpr:
        if (pn->isKind(ParseNodeKind::NumberExpr)) {
          JSAtom* atom = NumberToAtom(cx, pn->as<NumericLiteral>().value());
          if (!atom) {
            return false;
          }
          if (!TryReplaceNode(pnp,
                              handler->newStringLiteral(atom, pn->pn_pos))) {
            return false;
          }
        }
        break;

      default:
        MOZ_CRASH("Invalid type in constant folding FoldType");
    }
  }
  return true;
}

static bool IsEffectless(ParseNode* node) {
  return node->isKind(ParseNodeKind::TrueExpr) ||
         node->isKind(ParseNodeKind::FalseExpr) ||
         node->isKind(ParseNodeKind::StringExpr) ||
         node->isKind(ParseNodeKind::TemplateStringExpr) ||
         node->isKind(ParseNodeKind::NumberExpr) ||
         node->isKind(ParseNodeKind::BigIntExpr) ||
         node->isKind(ParseNodeKind::NullExpr) ||
         node->isKind(ParseNodeKind::RawUndefinedExpr) ||
         node->isKind(ParseNodeKind::Function);
}

enum Truthiness { Truthy, Falsy, Unknown };
>>>>>>> upstream-releases

<<<<<<< HEAD
    case ParseNodeKind::String:
    case ParseNodeKind::TemplateString:
      return (pn->as<NameNode>().atom()->length() > 0) ? Truthy : Falsy;

    case ParseNodeKind::True:
    case ParseNodeKind::Function:
      return Truthy;

    case ParseNodeKind::False:
    case ParseNodeKind::Null:
    case ParseNodeKind::RawUndefined:
      return Falsy;

    case ParseNodeKind::Void: {
      // |void <foo>| evaluates to |undefined| which isn't truthy.  But the
      // sense of this method requires that the expression be literally
      // replaceable with true/false: not the case if the nested expression
      // is effectful, might throw, &c.  Walk past the |void| (and nested
      // |void| expressions, for good measure) and check that the nested
      // expression doesn't break this requirement before indicating falsity.
      do {
        pn = pn->as<UnaryNode>().kid();
      } while (pn->isKind(ParseNodeKind::Void));

      return IsEffectless(pn) ? Falsy : Unknown;
||||||| merged common ancestors
    // ...but then they sometimes can be further folded to constants.
    ParseNode* node = *nodePtr;
    Truthiness t = Boolish(node);
    if (t != Unknown) {
        // We can turn function nodes into constant nodes here, but mutating
        // function nodes is tricky --- in particular, mutating a function node
        // that appears on a method list corrupts the method list. However,
        // methods are M's in statements of the form 'this.foo = M;', which we
        // never fold, so we're okay.
        if (t == Truthy) {
            node->setKind(ParseNodeKind::True);
            node->setOp(JSOP_TRUE);
        } else {
            node->setKind(ParseNodeKind::False);
            node->setOp(JSOP_FALSE);
        }
        node->setArity(PN_NULLARY);
=======
static Truthiness Boolish(ParseNode* pn) {
  switch (pn->getKind()) {
    case ParseNodeKind::NumberExpr:
      return (pn->as<NumericLiteral>().value() != 0 &&
              !IsNaN(pn->as<NumericLiteral>().value()))
                 ? Truthy
                 : Falsy;

    case ParseNodeKind::BigIntExpr:
      return (pn->as<BigIntLiteral>().box()->value()->isZero()) ? Falsy
                                                                : Truthy;

    case ParseNodeKind::StringExpr:
    case ParseNodeKind::TemplateStringExpr:
      return (pn->as<NameNode>().atom()->length() > 0) ? Truthy : Falsy;

    case ParseNodeKind::TrueExpr:
    case ParseNodeKind::Function:
      return Truthy;

    case ParseNodeKind::FalseExpr:
    case ParseNodeKind::NullExpr:
    case ParseNodeKind::RawUndefinedExpr:
      return Falsy;

    case ParseNodeKind::VoidExpr: {
      // |void <foo>| evaluates to |undefined| which isn't truthy.  But the
      // sense of this method requires that the expression be literally
      // replaceable with true/false: not the case if the nested expression
      // is effectful, might throw, &c.  Walk past the |void| (and nested
      // |void| expressions, for good measure) and check that the nested
      // expression doesn't break this requirement before indicating falsity.
      do {
        pn = pn->as<UnaryNode>().kid();
      } while (pn->isKind(ParseNodeKind::VoidExpr));

      return IsEffectless(pn) ? Falsy : Unknown;
>>>>>>> upstream-releases
    }

    default:
      return Unknown;
  }
}

<<<<<<< HEAD
static bool Fold(JSContext* cx, ParseNode** pnp,
                 PerHandlerParser<FullParseHandler>& parser);

static bool FoldCondition(JSContext* cx, ParseNode** nodePtr,
                          PerHandlerParser<FullParseHandler>& parser) {
  // Conditions fold like any other expression...
  if (!Fold(cx, nodePtr, parser)) {
    return false;
  }

  // ...but then they sometimes can be further folded to constants.
  ParseNode* node = *nodePtr;
  Truthiness t = Boolish(node);
  if (t != Unknown) {
    // We can turn function nodes into constant nodes here, but mutating
    // function nodes is tricky --- in particular, mutating a function node
    // that appears on a method list corrupts the method list. However,
    // methods are M's in statements of the form 'this.foo = M;', which we
    // never fold, so we're okay.
    if (t == Truthy) {
      node->setKind(ParseNodeKind::True);
      node->setOp(JSOP_TRUE);
    } else {
      node->setKind(ParseNodeKind::False);
      node->setOp(JSOP_FALSE);
||||||| merged common ancestors
static bool
FoldTypeOfExpr(JSContext* cx, UnaryNode* node, PerHandlerParser<FullParseHandler>& parser)
{
    MOZ_ASSERT(node->isKind(ParseNodeKind::TypeOfExpr));

    if (!Fold(cx, node->unsafeKidReference(), parser)) {
        return false;
=======
static bool SimplifyCondition(JSContext* cx, FullParseHandler* handler,
                              ParseNode** nodePtr) {
  // Conditions fold like any other expression, but then they sometimes can be
  // further folded to constants. *nodePtr should already have been
  // constant-folded.

  ParseNode* node = *nodePtr;
  Truthiness t = Boolish(node);
  if (t != Unknown) {
    // We can turn function nodes into constant nodes here, but mutating
    // function nodes is tricky --- in particular, mutating a function node
    // that appears on a method list corrupts the method list. However,
    // methods are M's in statements of the form 'this.foo = M;', which we
    // never fold, so we're okay.
    if (!TryReplaceNode(
            nodePtr, handler->newBooleanLiteral(t == Truthy, node->pn_pos))) {
      return false;
>>>>>>> upstream-releases
    }
  }

  return true;
}

<<<<<<< HEAD
static bool FoldTypeOfExpr(JSContext* cx, UnaryNode* node,
                           PerHandlerParser<FullParseHandler>& parser) {
  MOZ_ASSERT(node->isKind(ParseNodeKind::TypeOfExpr));
||||||| merged common ancestors
    if (result) {
        node->setKind(ParseNodeKind::String);
        node->setArity(PN_NAME);
        node->setOp(JSOP_NOP);
        node->as<NameNode>().setAtom(result);
    }
=======
static bool FoldTypeOfExpr(JSContext* cx, FullParseHandler* handler,
                           ParseNode** nodePtr) {
  UnaryNode* node = &(*nodePtr)->as<UnaryNode>();
  MOZ_ASSERT(node->isKind(ParseNodeKind::TypeOfExpr));
  ParseNode* expr = node->kid();

  // Constant-fold the entire |typeof| if given a constant with known type.
  RootedPropertyName result(cx);
  if (expr->isKind(ParseNodeKind::StringExpr) ||
      expr->isKind(ParseNodeKind::TemplateStringExpr)) {
    result = cx->names().string;
  } else if (expr->isKind(ParseNodeKind::NumberExpr)) {
    result = cx->names().number;
  } else if (expr->isKind(ParseNodeKind::BigIntExpr)) {
    result = cx->names().bigint;
  } else if (expr->isKind(ParseNodeKind::NullExpr)) {
    result = cx->names().object;
  } else if (expr->isKind(ParseNodeKind::TrueExpr) ||
             expr->isKind(ParseNodeKind::FalseExpr)) {
    result = cx->names().boolean;
  } else if (expr->is<FunctionNode>()) {
    result = cx->names().function;
  }

  if (result) {
    if (!TryReplaceNode(nodePtr,
                        handler->newStringLiteral(result, node->pn_pos))) {
      return false;
    }
  }
>>>>>>> upstream-releases

<<<<<<< HEAD
  if (!Fold(cx, node->unsafeKidReference(), parser)) {
    return false;
  }

  ParseNode* expr = node->kid();

  // Constant-fold the entire |typeof| if given a constant with known type.
  RootedPropertyName result(cx);
  if (expr->isKind(ParseNodeKind::String) ||
      expr->isKind(ParseNodeKind::TemplateString)) {
    result = cx->names().string;
  } else if (expr->isKind(ParseNodeKind::Number)) {
    result = cx->names().number;
  }
#ifdef ENABLE_BIGINT
  else if (expr->isKind(ParseNodeKind::BigInt)) {
    result = cx->names().bigint;
  }
#endif
  else if (expr->isKind(ParseNodeKind::Null)) {
    result = cx->names().object;
  } else if (expr->isKind(ParseNodeKind::True) ||
             expr->isKind(ParseNodeKind::False)) {
    result = cx->names().boolean;
  } else if (expr->isKind(ParseNodeKind::Function)) {
    result = cx->names().function;
  }

  if (result) {
    node->setKind(ParseNodeKind::String);
    node->setOp(JSOP_NOP);
    node->as<NameNode>().setAtom(result);
  }

  return true;
||||||| merged common ancestors
    return true;
=======
  return true;
>>>>>>> upstream-releases
}

<<<<<<< HEAD
static bool FoldDeleteExpr(JSContext* cx, UnaryNode* node,
                           PerHandlerParser<FullParseHandler>& parser) {
  MOZ_ASSERT(node->isKind(ParseNodeKind::DeleteExpr));
||||||| merged common ancestors
static bool
FoldDeleteExpr(JSContext* cx, UnaryNode* node, PerHandlerParser<FullParseHandler>& parser)
{
    MOZ_ASSERT(node->isKind(ParseNodeKind::DeleteExpr));
=======
static bool FoldDeleteExpr(JSContext* cx, FullParseHandler* handler,
                           ParseNode** nodePtr) {
  UnaryNode* node = &(*nodePtr)->as<UnaryNode>();
>>>>>>> upstream-releases

<<<<<<< HEAD
  if (!Fold(cx, node->unsafeKidReference(), parser)) {
    return false;
  }
||||||| merged common ancestors
    if (!Fold(cx, node->unsafeKidReference(), parser)) {
        return false;
    }
=======
  MOZ_ASSERT(node->isKind(ParseNodeKind::DeleteExpr));
  ParseNode* expr = node->kid();
>>>>>>> upstream-releases

<<<<<<< HEAD
  ParseNode* expr = node->kid();

  // Expression deletion evaluates the expression, then evaluates to true.
  // For effectless expressions, eliminate the expression evaluation.
  if (IsEffectless(expr)) {
    node->setKind(ParseNodeKind::True);
    node->setOp(JSOP_TRUE);
  }
||||||| merged common ancestors
    ParseNode* expr = node->kid();

    // Expression deletion evaluates the expression, then evaluates to true.
    // For effectless expressions, eliminate the expression evaluation.
    if (IsEffectless(expr)) {
        node->setKind(ParseNodeKind::True);
        node->setArity(PN_NULLARY);
        node->setOp(JSOP_TRUE);
    }
=======
  // Expression deletion evaluates the expression, then evaluates to true.
  // For effectless expressions, eliminate the expression evaluation.
  if (IsEffectless(expr)) {
    if (!TryReplaceNode(nodePtr,
                        handler->newBooleanLiteral(true, node->pn_pos))) {
      return false;
    }
  }
>>>>>>> upstream-releases

  return true;
}

<<<<<<< HEAD
static bool FoldDeleteElement(JSContext* cx, UnaryNode* node,
                              PerHandlerParser<FullParseHandler>& parser) {
  MOZ_ASSERT(node->isKind(ParseNodeKind::DeleteElem));
  MOZ_ASSERT(node->kid()->isKind(ParseNodeKind::Elem));
||||||| merged common ancestors
static bool
FoldDeleteElement(JSContext* cx, UnaryNode* node, PerHandlerParser<FullParseHandler>& parser)
{
    MOZ_ASSERT(node->isKind(ParseNodeKind::DeleteElem));
    MOZ_ASSERT(node->kid()->isKind(ParseNodeKind::Elem));

    if (!Fold(cx, node->unsafeKidReference(), parser)) {
        return false;
    }

    ParseNode* expr = node->kid();

    // If we're deleting an element, but constant-folding converted our
    // element reference into a dotted property access, we must *also*
    // morph the node's kind.
    //
    // In principle this also applies to |super["foo"] -> super.foo|,
    // but we don't constant-fold |super["foo"]| yet.
    MOZ_ASSERT(expr->isKind(ParseNodeKind::Elem) || expr->isKind(ParseNodeKind::Dot));
    if (expr->isKind(ParseNodeKind::Dot)) {
        node->setKind(ParseNodeKind::DeleteProp);
    }
=======
static bool FoldDeleteElement(JSContext* cx, FullParseHandler* handler,
                              ParseNode** nodePtr) {
  UnaryNode* node = &(*nodePtr)->as<UnaryNode>();
  MOZ_ASSERT(node->isKind(ParseNodeKind::DeleteElemExpr));
  ParseNode* expr = node->kid();

  // If we're deleting an element, but constant-folding converted our
  // element reference into a dotted property access, we must *also*
  // morph the node's kind.
  //
  // In principle this also applies to |super["foo"] -> super.foo|,
  // but we don't constant-fold |super["foo"]| yet.
  MOZ_ASSERT(expr->isKind(ParseNodeKind::ElemExpr) ||
             expr->isKind(ParseNodeKind::DotExpr));
  if (expr->isKind(ParseNodeKind::DotExpr)) {
    // newDelete will detect and use DeletePropExpr
    if (!TryReplaceNode(nodePtr,
                        handler->newDelete(node->pn_pos.begin, expr))) {
      return false;
    }
    MOZ_ASSERT((*nodePtr)->getKind() == ParseNodeKind::DeletePropExpr);
  }

  return true;
}

static bool FoldNot(JSContext* cx, FullParseHandler* handler,
                    ParseNode** nodePtr) {
  UnaryNode* node = &(*nodePtr)->as<UnaryNode>();
  MOZ_ASSERT(node->isKind(ParseNodeKind::NotExpr));
>>>>>>> upstream-releases

<<<<<<< HEAD
  if (!Fold(cx, node->unsafeKidReference(), parser)) {
    return false;
  }

  ParseNode* expr = node->kid();

  // If we're deleting an element, but constant-folding converted our
  // element reference into a dotted property access, we must *also*
  // morph the node's kind.
  //
  // In principle this also applies to |super["foo"] -> super.foo|,
  // but we don't constant-fold |super["foo"]| yet.
  MOZ_ASSERT(expr->isKind(ParseNodeKind::Elem) ||
             expr->isKind(ParseNodeKind::Dot));
  if (expr->isKind(ParseNodeKind::Dot)) {
    node->setKind(ParseNodeKind::DeleteProp);
  }

  return true;
}
||||||| merged common ancestors
    return true;
}
=======
  if (!SimplifyCondition(cx, handler, node->unsafeKidReference())) {
    return false;
  }
>>>>>>> upstream-releases

<<<<<<< HEAD
static bool FoldDeleteProperty(JSContext* cx, UnaryNode* node,
                               PerHandlerParser<FullParseHandler>& parser) {
  MOZ_ASSERT(node->isKind(ParseNodeKind::DeleteProp));
  MOZ_ASSERT(node->kid()->isKind(ParseNodeKind::Dot));
||||||| merged common ancestors
static bool
FoldDeleteProperty(JSContext* cx, UnaryNode* node, PerHandlerParser<FullParseHandler>& parser)
{
    MOZ_ASSERT(node->isKind(ParseNodeKind::DeleteProp));
    MOZ_ASSERT(node->kid()->isKind(ParseNodeKind::Dot));
=======
  ParseNode* expr = node->kid();
>>>>>>> upstream-releases

<<<<<<< HEAD
#ifdef DEBUG
  ParseNodeKind oldKind = node->kid()->getKind();
#endif
||||||| merged common ancestors
#ifdef DEBUG
    ParseNodeKind oldKind = node->kid()->getKind();
#endif
=======
  if (expr->isKind(ParseNodeKind::TrueExpr) ||
      expr->isKind(ParseNodeKind::FalseExpr)) {
    bool newval = !expr->isKind(ParseNodeKind::TrueExpr);
>>>>>>> upstream-releases

<<<<<<< HEAD
  if (!Fold(cx, node->unsafeKidReference(), parser)) {
    return false;
  }
||||||| merged common ancestors
    if (!Fold(cx, node->unsafeKidReference(), parser)) {
        return false;
    }
=======
    if (!TryReplaceNode(nodePtr,
                        handler->newBooleanLiteral(newval, node->pn_pos))) {
      return false;
    }
  }
>>>>>>> upstream-releases

<<<<<<< HEAD
  MOZ_ASSERT(node->kid()->isKind(oldKind),
             "kind should have remained invariant under folding");

  return true;
||||||| merged common ancestors
    MOZ_ASSERT(node->kid()->isKind(oldKind),
               "kind should have remained invariant under folding");

    return true;
=======
  return true;
>>>>>>> upstream-releases
}

<<<<<<< HEAD
static bool FoldNot(JSContext* cx, UnaryNode* node,
                    PerHandlerParser<FullParseHandler>& parser) {
  MOZ_ASSERT(node->isKind(ParseNodeKind::Not));

  if (!FoldCondition(cx, node->unsafeKidReference(), parser)) {
    return false;
  }
||||||| merged common ancestors
static bool
FoldNot(JSContext* cx, UnaryNode* node, PerHandlerParser<FullParseHandler>& parser)
{
    MOZ_ASSERT(node->isKind(ParseNodeKind::Not));

    if (!FoldCondition(cx, node->unsafeKidReference(), parser)) {
        return false;
    }

    ParseNode* expr = node->kid();
=======
static bool FoldUnaryArithmetic(JSContext* cx, FullParseHandler* handler,
                                ParseNode** nodePtr) {
  UnaryNode* node = &(*nodePtr)->as<UnaryNode>();
  MOZ_ASSERT(node->isKind(ParseNodeKind::BitNotExpr) ||
                 node->isKind(ParseNodeKind::PosExpr) ||
                 node->isKind(ParseNodeKind::NegExpr),
             "need a different method for this node kind");

  ParseNode* expr = node->kid();
>>>>>>> upstream-releases

<<<<<<< HEAD
  ParseNode* expr = node->kid();

  if (expr->isKind(ParseNodeKind::Number)) {
    double d = expr->as<NumericLiteral>().value();
||||||| merged common ancestors
    if (expr->isKind(ParseNodeKind::Number)) {
        double d = expr->as<NumericLiteral>().value();

        if (d == 0 || IsNaN(d)) {
            node->setKind(ParseNodeKind::True);
            node->setOp(JSOP_TRUE);
        } else {
            node->setKind(ParseNodeKind::False);
            node->setOp(JSOP_FALSE);
        }
        node->setArity(PN_NULLARY);
    } else if (expr->isKind(ParseNodeKind::True) || expr->isKind(ParseNodeKind::False)) {
        bool newval = !expr->isKind(ParseNodeKind::True);
=======
  if (expr->isKind(ParseNodeKind::NumberExpr) ||
      expr->isKind(ParseNodeKind::TrueExpr) ||
      expr->isKind(ParseNodeKind::FalseExpr)) {
    double d = expr->isKind(ParseNodeKind::NumberExpr)
                   ? expr->as<NumericLiteral>().value()
                   : double(expr->isKind(ParseNodeKind::TrueExpr));

    if (node->isKind(ParseNodeKind::BitNotExpr)) {
      d = ~ToInt32(d);
    } else if (node->isKind(ParseNodeKind::NegExpr)) {
      d = -d;
    } else {
      MOZ_ASSERT(node->isKind(ParseNodeKind::PosExpr));  // nothing to do
    }
>>>>>>> upstream-releases

<<<<<<< HEAD
    if (d == 0 || IsNaN(d)) {
      node->setKind(ParseNodeKind::True);
      node->setOp(JSOP_TRUE);
    } else {
      node->setKind(ParseNodeKind::False);
      node->setOp(JSOP_FALSE);
||||||| merged common ancestors
        node->setKind(newval ? ParseNodeKind::True : ParseNodeKind::False);
        node->setArity(PN_NULLARY);
        node->setOp(newval ? JSOP_TRUE : JSOP_FALSE);
=======
    if (!TryReplaceNode(nodePtr,
                        handler->newNumber(d, NoDecimal, node->pn_pos))) {
      return false;
>>>>>>> upstream-releases
    }
<<<<<<< HEAD
  } else if (expr->isKind(ParseNodeKind::True) ||
             expr->isKind(ParseNodeKind::False)) {
    bool newval = !expr->isKind(ParseNodeKind::True);
||||||| merged common ancestors
=======
  }
>>>>>>> upstream-releases

<<<<<<< HEAD
    node->setKind(newval ? ParseNodeKind::True : ParseNodeKind::False);
    node->setOp(newval ? JSOP_TRUE : JSOP_FALSE);
  }

  return true;
||||||| merged common ancestors
    return true;
=======
  return true;
>>>>>>> upstream-releases
}

<<<<<<< HEAD
static bool FoldUnaryArithmetic(JSContext* cx, UnaryNode* node,
                                PerHandlerParser<FullParseHandler>& parser) {
  MOZ_ASSERT(node->isKind(ParseNodeKind::BitNot) ||
                 node->isKind(ParseNodeKind::Pos) ||
                 node->isKind(ParseNodeKind::Neg),
             "need a different method for this node kind");

  if (!Fold(cx, node->unsafeKidReference(), parser)) {
    return false;
  }
||||||| merged common ancestors
static bool
FoldUnaryArithmetic(JSContext* cx, UnaryNode* node, PerHandlerParser<FullParseHandler>& parser)
{
    MOZ_ASSERT(node->isKind(ParseNodeKind::BitNot) ||
               node->isKind(ParseNodeKind::Pos) ||
               node->isKind(ParseNodeKind::Neg),
               "need a different method for this node kind");

    if (!Fold(cx, node->unsafeKidReference(), parser)) {
        return false;
    }
=======
static bool FoldAndOr(JSContext* cx, ParseNode** nodePtr) {
  ListNode* node = &(*nodePtr)->as<ListNode>();
>>>>>>> upstream-releases

<<<<<<< HEAD
  ParseNode* expr = node->kid();
||||||| merged common ancestors
    ParseNode* expr = node->kid();
=======
  MOZ_ASSERT(node->isKind(ParseNodeKind::AndExpr) ||
             node->isKind(ParseNodeKind::OrExpr));
>>>>>>> upstream-releases

<<<<<<< HEAD
  if (expr->isKind(ParseNodeKind::Number) ||
      expr->isKind(ParseNodeKind::True) || expr->isKind(ParseNodeKind::False)) {
    double d = expr->isKind(ParseNodeKind::Number)
                   ? expr->as<NumericLiteral>().value()
                   : double(expr->isKind(ParseNodeKind::True));

    if (node->isKind(ParseNodeKind::BitNot)) {
      d = ~ToInt32(d);
    } else if (node->isKind(ParseNodeKind::Neg)) {
      d = -d;
    } else {
      MOZ_ASSERT(node->isKind(ParseNodeKind::Pos));  // nothing to do
||||||| merged common ancestors
    if (expr->isKind(ParseNodeKind::Number) ||
        expr->isKind(ParseNodeKind::True) ||
        expr->isKind(ParseNodeKind::False))
    {
        double d = expr->isKind(ParseNodeKind::Number)
                   ? expr->as<NumericLiteral>().value()
                   : double(expr->isKind(ParseNodeKind::True));

        if (node->isKind(ParseNodeKind::BitNot)) {
            d = ~ToInt32(d);
        } else if (node->isKind(ParseNodeKind::Neg)) {
            d = -d;
        } else {
            MOZ_ASSERT(node->isKind(ParseNodeKind::Pos)); // nothing to do
        }

        node->setKind(ParseNodeKind::Number);
        node->setArity(PN_NUMBER);
        node->setOp(JSOP_DOUBLE);
        node->as<NumericLiteral>().setValue(d);
=======
  bool isOrNode = node->isKind(ParseNodeKind::OrExpr);
  ParseNode** elem = node->unsafeHeadReference();
  do {
    Truthiness t = Boolish(*elem);

    // If we don't know the constant-folded node's truthiness, we can't
    // reduce this node with its surroundings.  Continue folding any
    // remaining nodes.
    if (t == Unknown) {
      elem = &(*elem)->pn_next;
      continue;
>>>>>>> upstream-releases
    }

<<<<<<< HEAD
    node->setKind(ParseNodeKind::Number);
    node->setOp(JSOP_DOUBLE);
    node->as<NumericLiteral>().setValue(d);
  }

  return true;
}

static bool FoldIncrementDecrement(JSContext* cx, UnaryNode* incDec,
                                   PerHandlerParser<FullParseHandler>& parser) {
  MOZ_ASSERT(incDec->isKind(ParseNodeKind::PreIncrement) ||
             incDec->isKind(ParseNodeKind::PostIncrement) ||
             incDec->isKind(ParseNodeKind::PreDecrement) ||
             incDec->isKind(ParseNodeKind::PostDecrement));
||||||| merged common ancestors
    return true;
}

static bool
FoldIncrementDecrement(JSContext* cx, UnaryNode* incDec,
                       PerHandlerParser<FullParseHandler>& parser)
{
    MOZ_ASSERT(incDec->isKind(ParseNodeKind::PreIncrement) ||
               incDec->isKind(ParseNodeKind::PostIncrement) ||
               incDec->isKind(ParseNodeKind::PreDecrement) ||
               incDec->isKind(ParseNodeKind::PostDecrement));
=======
    // If the constant-folded node's truthiness will terminate the
    // condition -- `a || true || expr` or |b && false && expr| -- then
    // trailing nodes will never be evaluated.  Truncate the list after
    // the known-truthiness node, as it's the overall result.
    if ((t == Truthy) == isOrNode) {
      for (ParseNode* next = (*elem)->pn_next; next; next = next->pn_next) {
        node->unsafeDecrementCount();
      }
>>>>>>> upstream-releases

<<<<<<< HEAD
  MOZ_ASSERT(parser.isValidSimpleAssignmentTarget(
      incDec->kid(), PermitAssignmentToFunctionCalls));
||||||| merged common ancestors
    MOZ_ASSERT(parser.isValidSimpleAssignmentTarget(incDec->kid(),
                                                    PermitAssignmentToFunctionCalls));
=======
      // Terminate the original and/or list at the known-truthiness
      // node.
      (*elem)->pn_next = nullptr;
      elem = &(*elem)->pn_next;
      break;
    }
>>>>>>> upstream-releases

<<<<<<< HEAD
  if (!Fold(cx, incDec->unsafeKidReference(), parser)) {
    return false;
  }
||||||| merged common ancestors
    if (!Fold(cx, incDec->unsafeKidReference(), parser)) {
        return false;
    }
=======
    MOZ_ASSERT((t == Truthy) == !isOrNode);

    // We've encountered a vacuous node that'll never short-circuit
    // evaluation.
    if ((*elem)->pn_next) {
      // This node is never the overall result when there are
      // subsequent nodes.  Remove it.
      ParseNode* elt = *elem;
      *elem = elt->pn_next;
      node->unsafeDecrementCount();
    } else {
      // Otherwise this node is the result of the overall expression,
      // so leave it alone.  And we're done.
      elem = &(*elem)->pn_next;
      break;
    }
  } while (*elem);
>>>>>>> upstream-releases

<<<<<<< HEAD
  MOZ_ASSERT(parser.isValidSimpleAssignmentTarget(
      incDec->kid(), PermitAssignmentToFunctionCalls));
||||||| merged common ancestors
    MOZ_ASSERT(parser.isValidSimpleAssignmentTarget(incDec->kid(),
                                                    PermitAssignmentToFunctionCalls));
=======
  node->unsafeReplaceTail(elem);
>>>>>>> upstream-releases

<<<<<<< HEAD
  return true;
||||||| merged common ancestors
    return true;
=======
  // If we removed nodes, we may have to replace a one-element list with
  // its element.
  if (node->count() == 1) {
    ParseNode* first = node->head();
    ReplaceNode(nodePtr, first);
  }

  return true;
>>>>>>> upstream-releases
}

<<<<<<< HEAD
static bool FoldAndOr(JSContext* cx, ParseNode** nodePtr,
                      PerHandlerParser<FullParseHandler>& parser) {
  ListNode* node = &(*nodePtr)->as<ListNode>();
||||||| merged common ancestors
static bool
FoldAndOr(JSContext* cx, ParseNode** nodePtr, PerHandlerParser<FullParseHandler>& parser)
{
    ListNode* node = &(*nodePtr)->as<ListNode>();

    MOZ_ASSERT(node->isKind(ParseNodeKind::And) || node->isKind(ParseNodeKind::Or));
=======
static bool Fold(JSContext* cx, FullParseHandler* handler, ParseNode** pnp);

static bool FoldConditional(JSContext* cx, FullParseHandler* handler,
                            ParseNode** nodePtr) {
  ParseNode** nextNode = nodePtr;
>>>>>>> upstream-releases

<<<<<<< HEAD
  MOZ_ASSERT(node->isKind(ParseNodeKind::And) ||
             node->isKind(ParseNodeKind::Or));
||||||| merged common ancestors
    bool isOrNode = node->isKind(ParseNodeKind::Or);
    ParseNode** elem = node->unsafeHeadReference();
    do {
        if (!Fold(cx, elem, parser)) {
            return false;
        }
=======
  do {
    // |nextNode| on entry points to the C?T:F expression to be folded.
    // Reset it to exit the loop in the common case where F isn't another
    // ?: expression.
    nodePtr = nextNode;
    nextNode = nullptr;
>>>>>>> upstream-releases

<<<<<<< HEAD
  bool isOrNode = node->isKind(ParseNodeKind::Or);
  ParseNode** elem = node->unsafeHeadReference();
  do {
    if (!Fold(cx, elem, parser)) {
      return false;
    }
||||||| merged common ancestors
        Truthiness t = Boolish(*elem);
=======
    TernaryNode* node = &(*nodePtr)->as<TernaryNode>();
    MOZ_ASSERT(node->isKind(ParseNodeKind::ConditionalExpr));
>>>>>>> upstream-releases

<<<<<<< HEAD
    Truthiness t = Boolish(*elem);
||||||| merged common ancestors
        // If we don't know the constant-folded node's truthiness, we can't
        // reduce this node with its surroundings.  Continue folding any
        // remaining nodes.
        if (t == Unknown) {
            elem = &(*elem)->pn_next;
            continue;
        }
=======
    ParseNode** expr = node->unsafeKid1Reference();
    if (!Fold(cx, handler, expr)) {
      return false;
    }
    if (!SimplifyCondition(cx, handler, expr)) {
      return false;
    }
>>>>>>> upstream-releases

<<<<<<< HEAD
    // If we don't know the constant-folded node's truthiness, we can't
    // reduce this node with its surroundings.  Continue folding any
    // remaining nodes.
    if (t == Unknown) {
      elem = &(*elem)->pn_next;
      continue;
    }
||||||| merged common ancestors
        // If the constant-folded node's truthiness will terminate the
        // condition -- `a || true || expr` or |b && false && expr| -- then
        // trailing nodes will never be evaluated.  Truncate the list after
        // the known-truthiness node, as it's the overall result.
        if ((t == Truthy) == isOrNode) {
            for (ParseNode* next = (*elem)->pn_next; next; next = next->pn_next) {
                node->unsafeDecrementCount();
            }

            // Terminate the original and/or list at the known-truthiness
            // node.
            (*elem)->pn_next = nullptr;
            elem = &(*elem)->pn_next;
            break;
        }
=======
    ParseNode** ifTruthy = node->unsafeKid2Reference();
    if (!Fold(cx, handler, ifTruthy)) {
      return false;
    }
>>>>>>> upstream-releases

<<<<<<< HEAD
    // If the constant-folded node's truthiness will terminate the
    // condition -- `a || true || expr` or |b && false && expr| -- then
    // trailing nodes will never be evaluated.  Truncate the list after
    // the known-truthiness node, as it's the overall result.
    if ((t == Truthy) == isOrNode) {
      for (ParseNode* next = (*elem)->pn_next; next; next = next->pn_next) {
        node->unsafeDecrementCount();
      }
||||||| merged common ancestors
        MOZ_ASSERT((t == Truthy) == !isOrNode);

        // We've encountered a vacuous node that'll never short-circuit
        // evaluation.
        if ((*elem)->pn_next) {
            // This node is never the overall result when there are
            // subsequent nodes.  Remove it.
            ParseNode* elt = *elem;
            *elem = elt->pn_next;
            node->unsafeDecrementCount();
        } else {
            // Otherwise this node is the result of the overall expression,
            // so leave it alone.  And we're done.
            elem = &(*elem)->pn_next;
            break;
        }
    } while (*elem);
=======
    ParseNode** ifFalsy = node->unsafeKid3Reference();
>>>>>>> upstream-releases

<<<<<<< HEAD
      // Terminate the original and/or list at the known-truthiness
      // node.
      (*elem)->pn_next = nullptr;
      elem = &(*elem)->pn_next;
      break;
    }
||||||| merged common ancestors
    node->unsafeReplaceTail(elem);
=======
    // If our C?T:F node has F as another ?: node, *iteratively* constant-
    // fold F *after* folding C and T (and possibly eliminating C and one
    // of T/F entirely); otherwise fold F normally.  Making |nextNode| non-
    // null causes this loop to run again to fold F.
    //
    // Conceivably we could instead/also iteratively constant-fold T, if T
    // were more complex than F.  Such an optimization is unimplemented.
    if ((*ifFalsy)->isKind(ParseNodeKind::ConditionalExpr)) {
      MOZ_ASSERT((*ifFalsy)->is<TernaryNode>());
      nextNode = ifFalsy;
    } else {
      if (!Fold(cx, handler, ifFalsy)) {
        return false;
      }
    }
>>>>>>> upstream-releases

<<<<<<< HEAD
    MOZ_ASSERT((t == Truthy) == !isOrNode);

    // We've encountered a vacuous node that'll never short-circuit
    // evaluation.
    if ((*elem)->pn_next) {
      // This node is never the overall result when there are
      // subsequent nodes.  Remove it.
      ParseNode* elt = *elem;
      *elem = elt->pn_next;
      node->unsafeDecrementCount();
    } else {
      // Otherwise this node is the result of the overall expression,
      // so leave it alone.  And we're done.
      elem = &(*elem)->pn_next;
      break;
||||||| merged common ancestors
    // If we removed nodes, we may have to replace a one-element list with
    // its element.
    if (node->count() == 1) {
        ParseNode* first = node->head();
        ReplaceNode(nodePtr, first);
=======
    // Try to constant-fold based on the condition expression.
    Truthiness t = Boolish(*expr);
    if (t == Unknown) {
      continue;
>>>>>>> upstream-releases
    }
  } while (*elem);

<<<<<<< HEAD
  node->unsafeReplaceTail(elem);

  // If we removed nodes, we may have to replace a one-element list with
  // its element.
  if (node->count() == 1) {
    ParseNode* first = node->head();
    ReplaceNode(nodePtr, first);
  }

  return true;
}
||||||| merged common ancestors
    return true;
}
=======
    // Otherwise reduce 'C ? T : F' to T or F as directed by C.
    ParseNode* replacement = t == Truthy ? *ifTruthy : *ifFalsy;
>>>>>>> upstream-releases

<<<<<<< HEAD
static bool FoldConditional(JSContext* cx, ParseNode** nodePtr,
                            PerHandlerParser<FullParseHandler>& parser) {
  ParseNode** nextNode = nodePtr;
||||||| merged common ancestors
static bool
FoldConditional(JSContext* cx, ParseNode** nodePtr, PerHandlerParser<FullParseHandler>& parser)
{
    ParseNode** nextNode = nodePtr;
=======
    // Otherwise perform a replacement.  This invalidates |nextNode|, so
    // reset it (if the replacement requires folding) or clear it (if
    // |ifFalsy| is dead code) as needed.
    if (nextNode) {
      nextNode = (*nextNode == replacement) ? nodePtr : nullptr;
    }
    ReplaceNode(nodePtr, replacement);
  } while (nextNode);
>>>>>>> upstream-releases

<<<<<<< HEAD
  do {
    // |nextNode| on entry points to the C?T:F expression to be folded.
    // Reset it to exit the loop in the common case where F isn't another
    // ?: expression.
    nodePtr = nextNode;
    nextNode = nullptr;
||||||| merged common ancestors
    do {
        // |nextNode| on entry points to the C?T:F expression to be folded.
        // Reset it to exit the loop in the common case where F isn't another
        // ?: expression.
        nodePtr = nextNode;
        nextNode = nullptr;
=======
  return true;
}
>>>>>>> upstream-releases

<<<<<<< HEAD
    TernaryNode* node = &(*nodePtr)->as<TernaryNode>();
    MOZ_ASSERT(node->isKind(ParseNodeKind::Conditional));
||||||| merged common ancestors
        TernaryNode* node = &(*nodePtr)->as<TernaryNode>();
        MOZ_ASSERT(node->isKind(ParseNodeKind::Conditional));
=======
static bool FoldIf(JSContext* cx, FullParseHandler* handler,
                   ParseNode** nodePtr) {
  ParseNode** nextNode = nodePtr;
>>>>>>> upstream-releases

<<<<<<< HEAD
    ParseNode** expr = node->unsafeKid1Reference();
    if (!FoldCondition(cx, expr, parser)) {
      return false;
    }
||||||| merged common ancestors
        ParseNode** expr = node->unsafeKid1Reference();
        if (!FoldCondition(cx, expr, parser)) {
            return false;
        }
=======
  do {
    // |nextNode| on entry points to the initial |if| to be folded.  Reset
    // it to exit the loop when the |else| arm isn't another |if|.
    nodePtr = nextNode;
    nextNode = nullptr;
>>>>>>> upstream-releases

<<<<<<< HEAD
    ParseNode** ifTruthy = node->unsafeKid2Reference();
    if (!Fold(cx, ifTruthy, parser)) {
      return false;
    }
||||||| merged common ancestors
        ParseNode** ifTruthy = node->unsafeKid2Reference();
        if (!Fold(cx, ifTruthy, parser)) {
            return false;
        }
=======
    TernaryNode* node = &(*nodePtr)->as<TernaryNode>();
    MOZ_ASSERT(node->isKind(ParseNodeKind::IfStmt));
>>>>>>> upstream-releases

<<<<<<< HEAD
    ParseNode** ifFalsy = node->unsafeKid3Reference();
||||||| merged common ancestors
        ParseNode** ifFalsy = node->unsafeKid3Reference();
=======
    ParseNode** expr = node->unsafeKid1Reference();
    if (!Fold(cx, handler, expr)) {
      return false;
    }
    if (!SimplifyCondition(cx, handler, expr)) {
      return false;
    }
>>>>>>> upstream-releases

<<<<<<< HEAD
    // If our C?T:F node has F as another ?: node, *iteratively* constant-
    // fold F *after* folding C and T (and possibly eliminating C and one
    // of T/F entirely); otherwise fold F normally.  Making |nextNode| non-
    // null causes this loop to run again to fold F.
    //
    // Conceivably we could instead/also iteratively constant-fold T, if T
    // were more complex than F.  Such an optimization is unimplemented.
    if ((*ifFalsy)->isKind(ParseNodeKind::Conditional)) {
      MOZ_ASSERT((*ifFalsy)->is<TernaryNode>());
      nextNode = ifFalsy;
    } else {
      if (!Fold(cx, ifFalsy, parser)) {
        return false;
      }
    }
||||||| merged common ancestors
        // If our C?T:F node has F as another ?: node, *iteratively* constant-
        // fold F *after* folding C and T (and possibly eliminating C and one
        // of T/F entirely); otherwise fold F normally.  Making |nextNode| non-
        // null causes this loop to run again to fold F.
        //
        // Conceivably we could instead/also iteratively constant-fold T, if T
        // were more complex than F.  Such an optimization is unimplemented.
        if ((*ifFalsy)->isKind(ParseNodeKind::Conditional)) {
            MOZ_ASSERT((*ifFalsy)->is<TernaryNode>());
            nextNode = ifFalsy;
        } else {
            if (!Fold(cx, ifFalsy, parser)) {
                return false;
            }
        }
=======
    ParseNode** consequent = node->unsafeKid2Reference();
    if (!Fold(cx, handler, consequent)) {
      return false;
    }
>>>>>>> upstream-releases

<<<<<<< HEAD
    // Try to constant-fold based on the condition expression.
    Truthiness t = Boolish(*expr);
    if (t == Unknown) {
      continue;
    }
||||||| merged common ancestors
        // Try to constant-fold based on the condition expression.
        Truthiness t = Boolish(*expr);
        if (t == Unknown) {
            continue;
        }
=======
    ParseNode** alternative = node->unsafeKid3Reference();
    if (*alternative) {
      // If in |if (C) T; else F;| we have |F| as another |if|,
      // *iteratively* constant-fold |F| *after* folding |C| and |T| (and
      // possibly completely replacing the whole thing with |T| or |F|);
      // otherwise fold F normally.  Making |nextNode| non-null causes
      // this loop to run again to fold F.
      if ((*alternative)->isKind(ParseNodeKind::IfStmt)) {
        MOZ_ASSERT((*alternative)->is<TernaryNode>());
        nextNode = alternative;
      } else {
        if (!Fold(cx, handler, alternative)) {
          return false;
        }
      }
    }
>>>>>>> upstream-releases

<<<<<<< HEAD
    // Otherwise reduce 'C ? T : F' to T or F as directed by C.
    ParseNode* replacement = t == Truthy ? *ifTruthy : *ifFalsy;
||||||| merged common ancestors
        // Otherwise reduce 'C ? T : F' to T or F as directed by C.
        ParseNode* replacement = t == Truthy ? *ifTruthy : *ifFalsy;
=======
    // Eliminate the consequent or alternative if the condition has
    // constant truthiness.
    Truthiness t = Boolish(*expr);
    if (t == Unknown) {
      continue;
    }
>>>>>>> upstream-releases

<<<<<<< HEAD
    // Otherwise perform a replacement.  This invalidates |nextNode|, so
    // reset it (if the replacement requires folding) or clear it (if
    // |ifFalsy| is dead code) as needed.
    if (nextNode) {
      nextNode = (*nextNode == replacement) ? nodePtr : nullptr;
    }
    ReplaceNode(nodePtr, replacement);
  } while (nextNode);
||||||| merged common ancestors
        // Otherwise perform a replacement.  This invalidates |nextNode|, so
        // reset it (if the replacement requires folding) or clear it (if
        // |ifFalsy| is dead code) as needed.
        if (nextNode) {
            nextNode = (*nextNode == replacement) ? nodePtr : nullptr;
        }
        ReplaceNode(nodePtr, replacement);
    } while (nextNode);
=======
    // Careful!  Either of these can be null: |replacement| in |if (0) T;|,
    // and |discarded| in |if (true) T;|.
    ParseNode* replacement;
    ParseNode* discarded;
    if (t == Truthy) {
      replacement = *consequent;
      discarded = *alternative;
    } else {
      replacement = *alternative;
      discarded = *consequent;
    }
>>>>>>> upstream-releases

<<<<<<< HEAD
  return true;
}
||||||| merged common ancestors
    return true;
}
=======
    bool performReplacement = true;
    if (discarded) {
      // A declaration that hoists outside the discarded arm prevents the
      // |if| from being folded away.
      bool containsHoistedDecls;
      if (!ContainsHoistedDeclaration(cx, discarded, &containsHoistedDecls)) {
        return false;
      }
>>>>>>> upstream-releases

<<<<<<< HEAD
static bool FoldIf(JSContext* cx, ParseNode** nodePtr,
                   PerHandlerParser<FullParseHandler>& parser) {
  ParseNode** nextNode = nodePtr;
||||||| merged common ancestors
static bool
FoldIf(JSContext* cx, ParseNode** nodePtr, PerHandlerParser<FullParseHandler>& parser)
{
    ParseNode** nextNode = nodePtr;
=======
      performReplacement = !containsHoistedDecls;
    }
>>>>>>> upstream-releases

<<<<<<< HEAD
  do {
    // |nextNode| on entry points to the initial |if| to be folded.  Reset
    // it to exit the loop when the |else| arm isn't another |if|.
    nodePtr = nextNode;
    nextNode = nullptr;
||||||| merged common ancestors
    do {
        // |nextNode| on entry points to the initial |if| to be folded.  Reset
        // it to exit the loop when the |else| arm isn't another |if|.
        nodePtr = nextNode;
        nextNode = nullptr;
=======
    if (!performReplacement) {
      continue;
    }
>>>>>>> upstream-releases

<<<<<<< HEAD
    TernaryNode* node = &(*nodePtr)->as<TernaryNode>();
    MOZ_ASSERT(node->isKind(ParseNodeKind::If));
||||||| merged common ancestors
        TernaryNode* node = &(*nodePtr)->as<TernaryNode>();
        MOZ_ASSERT(node->isKind(ParseNodeKind::If));
=======
    if (!replacement) {
      // If there's no replacement node, we have a constantly-false |if|
      // with no |else|.  Replace the entire thing with an empty
      // statement list.
      if (!TryReplaceNode(nodePtr, handler->newStatementList(node->pn_pos))) {
        return false;
      }
    } else {
      // Replacement invalidates |nextNode|, so reset it (if the
      // replacement requires folding) or clear it (if |alternative|
      // is dead code) as needed.
      if (nextNode) {
        nextNode = (*nextNode == replacement) ? nodePtr : nullptr;
      }
      ReplaceNode(nodePtr, replacement);
    }
  } while (nextNode);
>>>>>>> upstream-releases

<<<<<<< HEAD
    ParseNode** expr = node->unsafeKid1Reference();
    if (!FoldCondition(cx, expr, parser)) {
      return false;
    }
||||||| merged common ancestors
        ParseNode** expr = node->unsafeKid1Reference();
        if (!FoldCondition(cx, expr, parser)) {
            return false;
        }
=======
  return true;
}
>>>>>>> upstream-releases

<<<<<<< HEAD
    ParseNode** consequent = node->unsafeKid2Reference();
    if (!Fold(cx, consequent, parser)) {
      return false;
    }
||||||| merged common ancestors
        ParseNode** consequent = node->unsafeKid2Reference();
        if (!Fold(cx, consequent, parser)) {
            return false;
        }
=======
static double ComputeBinary(ParseNodeKind kind, double left, double right) {
  if (kind == ParseNodeKind::AddExpr) {
    return left + right;
  }
>>>>>>> upstream-releases

<<<<<<< HEAD
    ParseNode** alternative = node->unsafeKid3Reference();
    if (*alternative) {
      // If in |if (C) T; else F;| we have |F| as another |if|,
      // *iteratively* constant-fold |F| *after* folding |C| and |T| (and
      // possibly completely replacing the whole thing with |T| or |F|);
      // otherwise fold F normally.  Making |nextNode| non-null causes
      // this loop to run again to fold F.
      if ((*alternative)->isKind(ParseNodeKind::If)) {
        MOZ_ASSERT((*alternative)->is<TernaryNode>());
        nextNode = alternative;
      } else {
        if (!Fold(cx, alternative, parser)) {
          return false;
        }
      }
    }
||||||| merged common ancestors
        ParseNode** alternative = node->unsafeKid3Reference();
        if (*alternative) {
            // If in |if (C) T; else F;| we have |F| as another |if|,
            // *iteratively* constant-fold |F| *after* folding |C| and |T| (and
            // possibly completely replacing the whole thing with |T| or |F|);
            // otherwise fold F normally.  Making |nextNode| non-null causes
            // this loop to run again to fold F.
            if ((*alternative)->isKind(ParseNodeKind::If)) {
                MOZ_ASSERT((*alternative)->is<TernaryNode>());
                nextNode = alternative;
            } else {
                if (!Fold(cx, alternative, parser)) {
                    return false;
                }
            }
        }
=======
  if (kind == ParseNodeKind::SubExpr) {
    return left - right;
  }
>>>>>>> upstream-releases

<<<<<<< HEAD
    // Eliminate the consequent or alternative if the condition has
    // constant truthiness.
    Truthiness t = Boolish(*expr);
    if (t == Unknown) {
      continue;
    }
||||||| merged common ancestors
        // Eliminate the consequent or alternative if the condition has
        // constant truthiness.
        Truthiness t = Boolish(*expr);
        if (t == Unknown) {
            continue;
        }
=======
  if (kind == ParseNodeKind::MulExpr) {
    return left * right;
  }
>>>>>>> upstream-releases

<<<<<<< HEAD
    // Careful!  Either of these can be null: |replacement| in |if (0) T;|,
    // and |discarded| in |if (true) T;|.
    ParseNode* replacement;
    ParseNode* discarded;
    if (t == Truthy) {
      replacement = *consequent;
      discarded = *alternative;
    } else {
      replacement = *alternative;
      discarded = *consequent;
    }
||||||| merged common ancestors
        // Careful!  Either of these can be null: |replacement| in |if (0) T;|,
        // and |discarded| in |if (true) T;|.
        ParseNode* replacement;
        ParseNode* discarded;
        if (t == Truthy) {
            replacement = *consequent;
            discarded = *alternative;
        } else {
            replacement = *alternative;
            discarded = *consequent;
        }
=======
  if (kind == ParseNodeKind::ModExpr) {
    return NumberMod(left, right);
  }
>>>>>>> upstream-releases

<<<<<<< HEAD
    bool performReplacement = true;
    if (discarded) {
      // A declaration that hoists outside the discarded arm prevents the
      // |if| from being folded away.
      bool containsHoistedDecls;
      if (!ContainsHoistedDeclaration(cx, discarded, &containsHoistedDecls)) {
        return false;
      }
||||||| merged common ancestors
        bool performReplacement = true;
        if (discarded) {
            // A declaration that hoists outside the discarded arm prevents the
            // |if| from being folded away.
            bool containsHoistedDecls;
            if (!ContainsHoistedDeclaration(cx, discarded, &containsHoistedDecls)) {
                return false;
            }
=======
  if (kind == ParseNodeKind::UrshExpr) {
    return ToUint32(left) >> (ToUint32(right) & 31);
  }
>>>>>>> upstream-releases

<<<<<<< HEAD
      performReplacement = !containsHoistedDecls;
    }
||||||| merged common ancestors
            performReplacement = !containsHoistedDecls;
        }
=======
  if (kind == ParseNodeKind::DivExpr) {
    return NumberDiv(left, right);
  }
>>>>>>> upstream-releases

<<<<<<< HEAD
    if (!performReplacement) {
      continue;
    }

    if (!replacement) {
      // If there's no replacement node, we have a constantly-false |if|
      // with no |else|.  Replace the entire thing with an empty
      // statement list.
      node->setKind(ParseNodeKind::StatementList);
      node->as<ListNode>().makeEmpty();
    } else {
      // Replacement invalidates |nextNode|, so reset it (if the
      // replacement requires folding) or clear it (if |alternative|
      // is dead code) as needed.
      if (nextNode) {
        nextNode = (*nextNode == replacement) ? nodePtr : nullptr;
      }
      ReplaceNode(nodePtr, replacement);
    }
  } while (nextNode);
||||||| merged common ancestors
        if (!performReplacement) {
            continue;
        }

        if (!replacement) {
            // If there's no replacement node, we have a constantly-false |if|
            // with no |else|.  Replace the entire thing with an empty
            // statement list.
            node->setKind(ParseNodeKind::StatementList);
            node->setArity(PN_LIST);
            node->as<ListNode>().makeEmpty();
        } else {
            // Replacement invalidates |nextNode|, so reset it (if the
            // replacement requires folding) or clear it (if |alternative|
            // is dead code) as needed.
            if (nextNode) {
                nextNode = (*nextNode == replacement) ? nodePtr : nullptr;
            }
            ReplaceNode(nodePtr, replacement);
        }
    } while (nextNode);
=======
  MOZ_ASSERT(kind == ParseNodeKind::LshExpr || kind == ParseNodeKind::RshExpr);
>>>>>>> upstream-releases

<<<<<<< HEAD
  return true;
||||||| merged common ancestors
    return true;
=======
  int32_t i = ToInt32(left);
  uint32_t j = ToUint32(right) & 31;
  return int32_t((kind == ParseNodeKind::LshExpr) ? uint32_t(i) << j : i >> j);
>>>>>>> upstream-releases
}

<<<<<<< HEAD
static bool FoldFunction(JSContext* cx, CodeNode* node,
                         PerHandlerParser<FullParseHandler>& parser) {
  MOZ_ASSERT(node->isKind(ParseNodeKind::Function));

  // Don't constant-fold inside "use asm" code, as this could create a parse
  // tree that doesn't type-check as asm.js.
  if (node->funbox()->useAsmOrInsideUseAsm()) {
    return true;
  }
||||||| merged common ancestors
static bool
FoldFunction(JSContext* cx, CodeNode* node, PerHandlerParser<FullParseHandler>& parser)
{
    MOZ_ASSERT(node->isKind(ParseNodeKind::Function));

    // Don't constant-fold inside "use asm" code, as this could create a parse
    // tree that doesn't type-check as asm.js.
    if (node->funbox()->useAsmOrInsideUseAsm()) {
        return true;
    }
=======
static bool FoldBinaryArithmetic(JSContext* cx, FullParseHandler* handler,
                                 ParseNode** nodePtr) {
  ListNode* node = &(*nodePtr)->as<ListNode>();
  MOZ_ASSERT(node->isKind(ParseNodeKind::SubExpr) ||
             node->isKind(ParseNodeKind::MulExpr) ||
             node->isKind(ParseNodeKind::LshExpr) ||
             node->isKind(ParseNodeKind::RshExpr) ||
             node->isKind(ParseNodeKind::UrshExpr) ||
             node->isKind(ParseNodeKind::DivExpr) ||
             node->isKind(ParseNodeKind::ModExpr));
  MOZ_ASSERT(node->count() >= 2);

  // Fold each operand to a number if possible.
  ParseNode** listp = node->unsafeHeadReference();
  for (; *listp; listp = &(*listp)->pn_next) {
    if (!FoldType(cx, handler, listp, ParseNodeKind::NumberExpr)) {
      return false;
    }
  }
  node->unsafeReplaceTail(listp);

  // Now fold all leading numeric terms together into a single number.
  // (Trailing terms for the non-shift operations can't be folded together
  // due to floating point imprecision.  For example, if |x === -2**53|,
  // |x - 1 - 1 === -2**53| but |x - 2 === -2**53 - 2|.  Shifts could be
  // folded, but it doesn't seem worth the effort.)
  ParseNode** elem = node->unsafeHeadReference();
  ParseNode** next = &(*elem)->pn_next;
  if ((*elem)->isKind(ParseNodeKind::NumberExpr)) {
    ParseNodeKind kind = node->getKind();
    while (true) {
      if (!*next || !(*next)->isKind(ParseNodeKind::NumberExpr)) {
        break;
      }
>>>>>>> upstream-releases

<<<<<<< HEAD
  // Note: body is null for lazily-parsed functions.
  if (node->body()) {
    if (!Fold(cx, node->unsafeBodyReference(), parser)) {
      return false;
    }
  }
||||||| merged common ancestors
    // Note: body is null for lazily-parsed functions.
    if (node->body()) {
        if (!Fold(cx, node->unsafeBodyReference(), parser)) {
            return false;
        }
    }
=======
      double d = ComputeBinary(kind, (*elem)->as<NumericLiteral>().value(),
                               (*next)->as<NumericLiteral>().value());
>>>>>>> upstream-releases

<<<<<<< HEAD
  return true;
}
||||||| merged common ancestors
    return true;
}
=======
      TokenPos pos((*elem)->pn_pos.begin, (*next)->pn_pos.end);
      if (!TryReplaceNode(elem, handler->newNumber(d, NoDecimal, pos))) {
        return false;
      }
>>>>>>> upstream-releases

<<<<<<< HEAD
static double ComputeBinary(ParseNodeKind kind, double left, double right) {
  if (kind == ParseNodeKind::Add) {
    return left + right;
  }
||||||| merged common ancestors
static double
ComputeBinary(ParseNodeKind kind, double left, double right)
{
    if (kind == ParseNodeKind::Add) {
        return left + right;
    }
=======
      (*elem)->pn_next = (*next)->pn_next;
      next = &(*elem)->pn_next;
      node->unsafeDecrementCount();
    }
>>>>>>> upstream-releases

<<<<<<< HEAD
  if (kind == ParseNodeKind::Sub) {
    return left - right;
  }
||||||| merged common ancestors
    if (kind == ParseNodeKind::Sub) {
        return left - right;
    }
=======
    if (node->count() == 1) {
      MOZ_ASSERT(node->head() == *elem);
      MOZ_ASSERT((*elem)->isKind(ParseNodeKind::NumberExpr));
>>>>>>> upstream-releases

<<<<<<< HEAD
  if (kind == ParseNodeKind::Star) {
    return left * right;
  }
||||||| merged common ancestors
    if (kind == ParseNodeKind::Star) {
        return left * right;
    }
=======
      if (!TryReplaceNode(nodePtr, *elem)) {
        return false;
      }
    }
  }
>>>>>>> upstream-releases

<<<<<<< HEAD
  if (kind == ParseNodeKind::Mod) {
    return NumberMod(left, right);
  }
||||||| merged common ancestors
    if (kind == ParseNodeKind::Mod) {
        return NumberMod(left, right);
    }
=======
  return true;
}
>>>>>>> upstream-releases

<<<<<<< HEAD
  if (kind == ParseNodeKind::Ursh) {
    return ToUint32(left) >> (ToUint32(right) & 31);
  }
||||||| merged common ancestors
    if (kind == ParseNodeKind::Ursh) {
        return ToUint32(left) >> (ToUint32(right) & 31);
    }
=======
static bool FoldExponentiation(JSContext* cx, FullParseHandler* handler,
                               ParseNode** nodePtr) {
  ListNode* node = &(*nodePtr)->as<ListNode>();
  MOZ_ASSERT(node->isKind(ParseNodeKind::PowExpr));
  MOZ_ASSERT(node->count() >= 2);

  // Fold each operand, ideally into a number.
  ParseNode** listp = node->unsafeHeadReference();
  for (; *listp; listp = &(*listp)->pn_next) {
    if (!FoldType(cx, handler, listp, ParseNodeKind::NumberExpr)) {
      return false;
    }
  }
>>>>>>> upstream-releases

<<<<<<< HEAD
  if (kind == ParseNodeKind::Div) {
    return NumberDiv(left, right);
  }
||||||| merged common ancestors
    if (kind == ParseNodeKind::Div) {
        return NumberDiv(left, right);
    }
=======
  node->unsafeReplaceTail(listp);
>>>>>>> upstream-releases

<<<<<<< HEAD
  MOZ_ASSERT(kind == ParseNodeKind::Lsh || kind == ParseNodeKind::Rsh);
||||||| merged common ancestors
    MOZ_ASSERT(kind == ParseNodeKind::Lsh || kind == ParseNodeKind::Rsh);
=======
  // Unlike all other binary arithmetic operators, ** is right-associative:
  // 2**3**5 is 2**(3**5), not (2**3)**5.  As list nodes singly-link their
  // children, full constant-folding requires either linear space or dodgy
  // in-place linked list reversal.  So we only fold one exponentiation: it's
  // easy and addresses common cases like |2**32|.
  if (node->count() > 2) {
    return true;
  }
>>>>>>> upstream-releases

<<<<<<< HEAD
  int32_t i = ToInt32(left);
  uint32_t j = ToUint32(right) & 31;
  return int32_t((kind == ParseNodeKind::Lsh) ? uint32_t(i) << j : i >> j);
}
||||||| merged common ancestors
    int32_t i = ToInt32(left);
    uint32_t j = ToUint32(right) & 31;
    return int32_t((kind == ParseNodeKind::Lsh) ? uint32_t(i) << j : i >> j);
}
=======
  ParseNode* base = node->head();
  ParseNode* exponent = base->pn_next;
  if (!base->isKind(ParseNodeKind::NumberExpr) ||
      !exponent->isKind(ParseNodeKind::NumberExpr)) {
    return true;
  }
>>>>>>> upstream-releases

<<<<<<< HEAD
static bool FoldModule(JSContext* cx, CodeNode* node,
                       PerHandlerParser<FullParseHandler>& parser) {
  MOZ_ASSERT(node->isKind(ParseNodeKind::Module));
||||||| merged common ancestors
static bool
FoldModule(JSContext* cx, CodeNode* node, PerHandlerParser<FullParseHandler>& parser)
{
    MOZ_ASSERT(node->isKind(ParseNodeKind::Module));
=======
  double d1 = base->as<NumericLiteral>().value();
  double d2 = exponent->as<NumericLiteral>().value();
>>>>>>> upstream-releases

<<<<<<< HEAD
  MOZ_ASSERT(node->body());
  return Fold(cx, node->unsafeBodyReference(), parser);
||||||| merged common ancestors
    MOZ_ASSERT(node->body());
    return Fold(cx, node->unsafeBodyReference(), parser);
=======
  return TryReplaceNode(
      nodePtr, handler->newNumber(ecmaPow(d1, d2), NoDecimal, node->pn_pos));
>>>>>>> upstream-releases
}

<<<<<<< HEAD
static bool FoldBinaryArithmetic(JSContext* cx, ListNode* node,
                                 PerHandlerParser<FullParseHandler>& parser) {
  MOZ_ASSERT(
      node->isKind(ParseNodeKind::Sub) || node->isKind(ParseNodeKind::Star) ||
      node->isKind(ParseNodeKind::Lsh) || node->isKind(ParseNodeKind::Rsh) ||
      node->isKind(ParseNodeKind::Ursh) || node->isKind(ParseNodeKind::Div) ||
      node->isKind(ParseNodeKind::Mod));
  MOZ_ASSERT(node->count() >= 2);

  // Fold each operand, ideally into a number.
  ParseNode** listp = node->unsafeHeadReference();
  for (; *listp; listp = &(*listp)->pn_next) {
    if (!Fold(cx, listp, parser)) {
      return false;
||||||| merged common ancestors
static bool
FoldBinaryArithmetic(JSContext* cx, ListNode* node, PerHandlerParser<FullParseHandler>& parser)
{
    MOZ_ASSERT(node->isKind(ParseNodeKind::Sub) ||
               node->isKind(ParseNodeKind::Star) ||
               node->isKind(ParseNodeKind::Lsh) ||
               node->isKind(ParseNodeKind::Rsh) ||
               node->isKind(ParseNodeKind::Ursh) ||
               node->isKind(ParseNodeKind::Div) ||
               node->isKind(ParseNodeKind::Mod));
    MOZ_ASSERT(node->count() >= 2);

    // Fold each operand, ideally into a number.
    ParseNode** listp = node->unsafeHeadReference();
    for (; *listp; listp = &(*listp)->pn_next) {
        if (!Fold(cx, listp, parser)) {
            return false;
        }

        if (!FoldType(cx, *listp, ParseNodeKind::Number)) {
            return false;
        }
=======
static bool FoldElement(JSContext* cx, FullParseHandler* handler,
                        ParseNode** nodePtr) {
  PropertyByValue* elem = &(*nodePtr)->as<PropertyByValue>();

  ParseNode* expr = &elem->expression();
  ParseNode* key = &elem->key();
  PropertyName* name = nullptr;
  if (key->isKind(ParseNodeKind::StringExpr)) {
    JSAtom* atom = key->as<NameNode>().atom();
    uint32_t index;

    if (atom->isIndex(&index)) {
      // Optimization 1: We have something like expr["100"]. This is
      // equivalent to expr[100] which is faster.
      if (!TryReplaceNode(elem->unsafeRightReference(),
                          handler->newNumber(index, NoDecimal, key->pn_pos))) {
        return false;
      }
      key = &elem->key();
    } else {
      name = atom->asPropertyName();
    }
  } else if (key->isKind(ParseNodeKind::NumberExpr)) {
    double number = key->as<NumericLiteral>().value();
    if (number != ToUint32(number)) {
      // Optimization 2: We have something like expr[3.14]. The number
      // isn't an array index, so it converts to a string ("3.14"),
      // enabling optimization 3 below.
      JSAtom* atom = NumberToAtom(cx, number);
      if (!atom) {
        return false;
      }
      name = atom->asPropertyName();
>>>>>>> upstream-releases
    }
  }

<<<<<<< HEAD
    if (!FoldType(cx, *listp, ParseNodeKind::Number)) {
      return false;
    }
  }

  node->unsafeReplaceTail(listp);

  // Now fold all leading numeric terms together into a single number.
  // (Trailing terms for the non-shift operations can't be folded together
  // due to floating point imprecision.  For example, if |x === -2**53|,
  // |x - 1 - 1 === -2**53| but |x - 2 === -2**53 - 2|.  Shifts could be
  // folded, but it doesn't seem worth the effort.)
  ParseNode* elem = node->head();
  ParseNode* next = elem->pn_next;
  if (elem->isKind(ParseNodeKind::Number)) {
    ParseNodeKind kind = node->getKind();
    while (true) {
      if (!next || !next->isKind(ParseNodeKind::Number)) {
        break;
      }

      double d = ComputeBinary(kind, elem->as<NumericLiteral>().value(),
                               next->as<NumericLiteral>().value());

      next = next->pn_next;
      elem->pn_next = next;

      elem->setKind(ParseNodeKind::Number);
      elem->setOp(JSOP_DOUBLE);
      elem->as<NumericLiteral>().setValue(d);

      node->unsafeDecrementCount();
    }
||||||| merged common ancestors
    node->unsafeReplaceTail(listp);

    // Now fold all leading numeric terms together into a single number.
    // (Trailing terms for the non-shift operations can't be folded together
    // due to floating point imprecision.  For example, if |x === -2**53|,
    // |x - 1 - 1 === -2**53| but |x - 2 === -2**53 - 2|.  Shifts could be
    // folded, but it doesn't seem worth the effort.)
    ParseNode* elem = node->head();
    ParseNode* next = elem->pn_next;
    if (elem->isKind(ParseNodeKind::Number)) {
        ParseNodeKind kind = node->getKind();
        while (true) {
            if (!next || !next->isKind(ParseNodeKind::Number)) {
                break;
            }

            double d = ComputeBinary(kind, elem->as<NumericLiteral>().value(), next->as<NumericLiteral>().value());

            next = next->pn_next;
            elem->pn_next = next;

            elem->setKind(ParseNodeKind::Number);
            elem->setArity(PN_NUMBER);
            elem->setOp(JSOP_DOUBLE);
            elem->as<NumericLiteral>().setValue(d);

            node->unsafeDecrementCount();
        }
=======
  // If we don't have a name, we can't optimize to getprop.
  if (!name) {
    return true;
  }
>>>>>>> upstream-releases

<<<<<<< HEAD
    if (node->count() == 1) {
      MOZ_ASSERT(node->head() == elem);
      MOZ_ASSERT(elem->isKind(ParseNodeKind::Number));
||||||| merged common ancestors
        if (node->count() == 1) {
            MOZ_ASSERT(node->head() == elem);
            MOZ_ASSERT(elem->isKind(ParseNodeKind::Number));
=======
  // Optimization 3: We have expr["foo"] where foo is not an index.  Convert
  // to a property access (like expr.foo) that optimizes better downstream.
>>>>>>> upstream-releases

<<<<<<< HEAD
      double d = elem->as<NumericLiteral>().value();
      node->setKind(ParseNodeKind::Number);
      node->setOp(JSOP_DOUBLE);
      node->as<NumericLiteral>().setValue(d);
    }
  }
||||||| merged common ancestors
            double d = elem->as<NumericLiteral>().value();
            node->setKind(ParseNodeKind::Number);
            node->setArity(PN_NUMBER);
            node->setOp(JSOP_DOUBLE);
            node->as<NumericLiteral>().setValue(d);
        }
    }
=======
  NameNode* propertyNameExpr = handler->newPropertyName(name, key->pn_pos);
  if (!propertyNameExpr) {
    return false;
  }
  if (!TryReplaceNode(nodePtr,
                      handler->newPropertyAccess(expr, propertyNameExpr))) {
    return false;
  }
>>>>>>> upstream-releases

  return true;
}

<<<<<<< HEAD
static bool FoldExponentiation(JSContext* cx, ListNode* node,
                               PerHandlerParser<FullParseHandler>& parser) {
  MOZ_ASSERT(node->isKind(ParseNodeKind::Pow));
  MOZ_ASSERT(node->count() >= 2);
||||||| merged common ancestors
static bool
FoldExponentiation(JSContext* cx, ListNode* node, PerHandlerParser<FullParseHandler>& parser)
{
    MOZ_ASSERT(node->isKind(ParseNodeKind::Pow));
    MOZ_ASSERT(node->count() >= 2);

    // Fold each operand, ideally into a number.
    ParseNode** listp = node->unsafeHeadReference();
    for (; *listp; listp = &(*listp)->pn_next) {
        if (!Fold(cx, listp, parser)) {
            return false;
        }
=======
static bool FoldAdd(JSContext* cx, FullParseHandler* handler,
                    ParseNode** nodePtr) {
  ListNode* node = &(*nodePtr)->as<ListNode>();

  MOZ_ASSERT(node->isKind(ParseNodeKind::AddExpr));
  MOZ_ASSERT(node->count() >= 2);

  // Fold leading numeric operands together:
  //
  //   (1 + 2 + x)  becomes  (3 + x)
  //
  // Don't go past the leading operands: additions after a string are
  // string concatenations, not additions: ("1" + 2 + 3 === "123").
  ParseNode** current = node->unsafeHeadReference();
  ParseNode** next = &(*current)->pn_next;
  if ((*current)->isKind(ParseNodeKind::NumberExpr)) {
    do {
      if (!(*next)->isKind(ParseNodeKind::NumberExpr)) {
        break;
      }

      double left = (*current)->as<NumericLiteral>().value();
      double right = (*next)->as<NumericLiteral>().value();
      TokenPos pos((*current)->pn_pos.begin, (*next)->pn_pos.end);
>>>>>>> upstream-releases

<<<<<<< HEAD
  // Fold each operand, ideally into a number.
  ParseNode** listp = node->unsafeHeadReference();
  for (; *listp; listp = &(*listp)->pn_next) {
    if (!Fold(cx, listp, parser)) {
      return false;
    }
||||||| merged common ancestors
        if (!FoldType(cx, *listp, ParseNodeKind::Number)) {
            return false;
        }
    }
=======
      if (!TryReplaceNode(current,
                          handler->newNumber(left + right, NoDecimal, pos))) {
        return false;
      }
>>>>>>> upstream-releases

<<<<<<< HEAD
    if (!FoldType(cx, *listp, ParseNodeKind::Number)) {
      return false;
||||||| merged common ancestors
    node->unsafeReplaceTail(listp);

    // Unlike all other binary arithmetic operators, ** is right-associative:
    // 2**3**5 is 2**(3**5), not (2**3)**5.  As list nodes singly-link their
    // children, full constant-folding requires either linear space or dodgy
    // in-place linked list reversal.  So we only fold one exponentiation: it's
    // easy and addresses common cases like |2**32|.
    if (node->count() > 2) {
        return true;
=======
      (*current)->pn_next = (*next)->pn_next;
      next = &(*current)->pn_next;

      node->unsafeDecrementCount();
    } while (*next);
  }

  // If any operands remain, attempt string concatenation folding.
  do {
    // If no operands remain, we're done.
    if (!*next) {
      break;
>>>>>>> upstream-releases
    }
  }

<<<<<<< HEAD
  node->unsafeReplaceTail(listp);
||||||| merged common ancestors
    ParseNode* base = node->head();
    ParseNode* exponent = base->pn_next;
    if (!base->isKind(ParseNodeKind::Number) || !exponent->isKind(ParseNodeKind::Number)) {
        return true;
    }
=======
    // (number + string) is string concatenation *only* at the start of
    // the list: (x + 1 + "2" !== x + "12") when x is a number.
    if ((*current)->isKind(ParseNodeKind::NumberExpr) &&
        (*next)->isKind(ParseNodeKind::StringExpr)) {
      if (!FoldType(cx, handler, current, ParseNodeKind::StringExpr)) {
        return false;
      }
      next = &(*current)->pn_next;
    }
>>>>>>> upstream-releases

<<<<<<< HEAD
  // Unlike all other binary arithmetic operators, ** is right-associative:
  // 2**3**5 is 2**(3**5), not (2**3)**5.  As list nodes singly-link their
  // children, full constant-folding requires either linear space or dodgy
  // in-place linked list reversal.  So we only fold one exponentiation: it's
  // easy and addresses common cases like |2**32|.
  if (node->count() > 2) {
    return true;
  }
||||||| merged common ancestors
    double d1 = base->as<NumericLiteral>().value();
    double d2 = exponent->as<NumericLiteral>().value();
=======
    // The first string forces all subsequent additions to be
    // string concatenations.
    do {
      if ((*current)->isKind(ParseNodeKind::StringExpr)) {
        break;
      }
>>>>>>> upstream-releases

<<<<<<< HEAD
  ParseNode* base = node->head();
  ParseNode* exponent = base->pn_next;
  if (!base->isKind(ParseNodeKind::Number) ||
      !exponent->isKind(ParseNodeKind::Number)) {
    return true;
  }

  double d1 = base->as<NumericLiteral>().value();
  double d2 = exponent->as<NumericLiteral>().value();

  node->setKind(ParseNodeKind::Number);
  node->setOp(JSOP_DOUBLE);
  node->as<NumericLiteral>().setValue(ecmaPow(d1, d2));
  return true;
}
||||||| merged common ancestors
    node->setKind(ParseNodeKind::Number);
    node->setArity(PN_NUMBER);
    node->setOp(JSOP_DOUBLE);
    node->as<NumericLiteral>().setValue(ecmaPow(d1, d2));
    return true;
}
=======
      current = next;
      next = &(*current)->pn_next;
    } while (*next);
>>>>>>> upstream-releases

<<<<<<< HEAD
static bool FoldList(JSContext* cx, ListNode* list,
                     PerHandlerParser<FullParseHandler>& parser) {
  ParseNode** elem = list->unsafeHeadReference();
  for (; *elem; elem = &(*elem)->pn_next) {
    if (!Fold(cx, elem, parser)) {
      return false;
||||||| merged common ancestors
static bool
FoldList(JSContext* cx, ListNode* list, PerHandlerParser<FullParseHandler>& parser)
{
    ParseNode** elem = list->unsafeHeadReference();
    for (; *elem; elem = &(*elem)->pn_next) {
        if (!Fold(cx, elem, parser)) {
            return false;
        }
=======
    // If there's nothing left to fold, we're done.
    if (!*next) {
      break;
>>>>>>> upstream-releases
    }
  }

<<<<<<< HEAD
  list->unsafeReplaceTail(elem);
||||||| merged common ancestors
    list->unsafeReplaceTail(elem);
=======
    RootedString combination(cx);
    RootedString tmp(cx);
    do {
      // Create a rope of the current string and all succeeding
      // constants that we can convert to strings, then atomize it
      // and replace them all with that fresh string.
      MOZ_ASSERT((*current)->isKind(ParseNodeKind::StringExpr));

      combination = (*current)->as<NameNode>().atom();
>>>>>>> upstream-releases

<<<<<<< HEAD
  return true;
}
||||||| merged common ancestors
    return true;
}
=======
      do {
        // Try folding the next operand to a string.
        if (!FoldType(cx, handler, next, ParseNodeKind::StringExpr)) {
          return false;
        }
>>>>>>> upstream-releases

<<<<<<< HEAD
static bool FoldReturn(JSContext* cx, UnaryNode* node,
                       PerHandlerParser<FullParseHandler>& parser) {
  MOZ_ASSERT(node->isKind(ParseNodeKind::Return));
||||||| merged common ancestors
static bool
FoldReturn(JSContext* cx, UnaryNode* node, PerHandlerParser<FullParseHandler>& parser)
{
    MOZ_ASSERT(node->isKind(ParseNodeKind::Return));
=======
        // Stop glomming once folding doesn't produce a string.
        if (!(*next)->isKind(ParseNodeKind::StringExpr)) {
          break;
        }
>>>>>>> upstream-releases

<<<<<<< HEAD
  if (node->kid()) {
    if (!Fold(cx, node->unsafeKidReference(), parser)) {
      return false;
    }
  }
||||||| merged common ancestors
    if (node->kid()) {
        if (!Fold(cx, node->unsafeKidReference(), parser)) {
            return false;
        }
    }
=======
        // Add this string to the combination and remove the node.
        tmp = (*next)->as<NameNode>().atom();
        combination = ConcatStrings<CanGC>(cx, combination, tmp);
        if (!combination) {
          return false;
        }
>>>>>>> upstream-releases

<<<<<<< HEAD
  return true;
}
||||||| merged common ancestors
    return true;
}
=======
        (*current)->pn_next = (*next)->pn_next;
        next = &(*current)->pn_next;

        node->unsafeDecrementCount();
      } while (*next);
>>>>>>> upstream-releases

<<<<<<< HEAD
static bool FoldTry(JSContext* cx, TryNode* node,
                    PerHandlerParser<FullParseHandler>& parser) {
  ParseNode** statements = node->unsafeKid1Reference();
  if (!Fold(cx, statements, parser)) {
    return false;
  }
||||||| merged common ancestors
static bool
FoldTry(JSContext* cx, TryNode* node, PerHandlerParser<FullParseHandler>& parser)
{
    ParseNode** statements = node->unsafeKid1Reference();
    if (!Fold(cx, statements, parser)) {
        return false;
    }
=======
      // Replace |current|'s string with the entire combination.
      MOZ_ASSERT((*current)->isKind(ParseNodeKind::StringExpr));
      combination = AtomizeString(cx, combination);
      if (!combination) {
        return false;
      }
      (*current)->as<NameNode>().setAtom(&combination->asAtom());
>>>>>>> upstream-releases

<<<<<<< HEAD
  ParseNode** catchScope = node->unsafeKid2Reference();
  if (*catchScope) {
    if (!Fold(cx, catchScope, parser)) {
      return false;
    }
  }
||||||| merged common ancestors
    ParseNode** catchScope = node->unsafeKid2Reference();
    if (*catchScope) {
        if (!Fold(cx, catchScope, parser)) {
            return false;
        }
    }
=======
      // If we're out of nodes, we're done.
      if (!*next) {
        break;
      }
>>>>>>> upstream-releases

<<<<<<< HEAD
  ParseNode** finally = node->unsafeKid3Reference();
  if (*finally) {
    if (!Fold(cx, finally, parser)) {
      return false;
    }
  }
||||||| merged common ancestors
    ParseNode** finally = node->unsafeKid3Reference();
    if (*finally) {
        if (!Fold(cx, finally, parser)) {
            return false;
        }
    }
=======
      current = next;
      next = &(*current)->pn_next;
>>>>>>> upstream-releases

<<<<<<< HEAD
  return true;
}
||||||| merged common ancestors
    return true;
}
=======
      // If we're out of nodes *after* the non-foldable-to-string
      // node, we're done.
      if (!*next) {
        break;
      }
>>>>>>> upstream-releases

<<<<<<< HEAD
static bool FoldCatch(JSContext* cx, BinaryNode* node,
                      PerHandlerParser<FullParseHandler>& parser) {
  ParseNode** declPattern = node->unsafeLeftReference();
  if (*declPattern) {
    if (!Fold(cx, declPattern, parser)) {
      return false;
    }
  }
||||||| merged common ancestors
static bool
FoldCatch(JSContext* cx, BinaryNode* node, PerHandlerParser<FullParseHandler>& parser)
{
    ParseNode** declPattern = node->unsafeLeftReference();
    if (*declPattern) {
        if (!Fold(cx, declPattern, parser)) {
            return false;
        }
    }
=======
      // Otherwise find the next node foldable to a string, and loop.
      do {
        current = next;
>>>>>>> upstream-releases

<<<<<<< HEAD
  ParseNode** statements = node->unsafeRightReference();
  if (*statements) {
    if (!Fold(cx, statements, parser)) {
      return false;
    }
  }
||||||| merged common ancestors
    ParseNode** statements = node->unsafeRightReference();
    if (*statements) {
        if (!Fold(cx, statements, parser)) {
            return false;
        }
    }
=======
        if (!FoldType(cx, handler, current, ParseNodeKind::StringExpr)) {
          return false;
        }
        next = &(*current)->pn_next;
      } while (!(*current)->isKind(ParseNodeKind::StringExpr) && *next);
    } while (*next);
  } while (false);
>>>>>>> upstream-releases

<<<<<<< HEAD
  return true;
||||||| merged common ancestors
    return true;
=======
  MOZ_ASSERT(!*next, "must have considered all nodes here");
  MOZ_ASSERT(!(*current)->pn_next, "current node must be the last node");

  node->unsafeReplaceTail(&(*current)->pn_next);

  if (node->count() == 1) {
    // We reduced the list to a constant.  Replace the ParseNodeKind::Add node
    // with that constant.
    ReplaceNode(nodePtr, *current);
  }

  return true;
>>>>>>> upstream-releases
}

<<<<<<< HEAD
static bool FoldClass(JSContext* cx, ClassNode* node,
                      PerHandlerParser<FullParseHandler>& parser) {
  MOZ_ASSERT(node->isKind(ParseNodeKind::Class));
||||||| merged common ancestors
static bool
FoldClass(JSContext* cx, ClassNode* node, PerHandlerParser<FullParseHandler>& parser)
{
    MOZ_ASSERT(node->isKind(ParseNodeKind::Class));
=======
class FoldVisitor : public RewritingParseNodeVisitor<FoldVisitor> {
  using Base = RewritingParseNodeVisitor;
>>>>>>> upstream-releases

<<<<<<< HEAD
  ParseNode** classNames = node->unsafeKid1Reference();
  if (*classNames) {
    if (!Fold(cx, classNames, parser)) {
      return false;
    }
  }
||||||| merged common ancestors
    ParseNode** classNames = node->unsafeKid1Reference();
    if (*classNames) {
        if (!Fold(cx, classNames, parser)) {
            return false;
        }
    }
=======
  FullParseHandler* handler;
>>>>>>> upstream-releases

<<<<<<< HEAD
  ParseNode** heritage = node->unsafeKid2Reference();
  if (*heritage) {
    if (!Fold(cx, heritage, parser)) {
      return false;
    }
  }
||||||| merged common ancestors
    ParseNode** heritage = node->unsafeKid2Reference();
    if (*heritage) {
        if (!Fold(cx, heritage, parser)) {
            return false;
        }
    }
=======
 public:
  explicit FoldVisitor(JSContext* cx, FullParseHandler* handler)
      : RewritingParseNodeVisitor(cx), handler(handler) {}
>>>>>>> upstream-releases

<<<<<<< HEAD
  ParseNode** body = node->unsafeKid3Reference();
  return Fold(cx, body, parser);
}
||||||| merged common ancestors
    ParseNode** body = node->unsafeKid3Reference();
    return Fold(cx, body, parser);
}
=======
  bool visitElemExpr(ParseNode*& pn) {
    return Base::visitElemExpr(pn) && FoldElement(cx_, handler, &pn);
  }
>>>>>>> upstream-releases

<<<<<<< HEAD
static bool FoldElement(JSContext* cx, ParseNode** nodePtr,
                        PerHandlerParser<FullParseHandler>& parser) {
  PropertyByValue* elem = &(*nodePtr)->as<PropertyByValue>();
||||||| merged common ancestors
static bool
FoldElement(JSContext* cx, ParseNode** nodePtr, PerHandlerParser<FullParseHandler>& parser)
{
    PropertyByValue* elem = &(*nodePtr)->as<PropertyByValue>();

    if (!Fold(cx, elem->unsafeLeftReference(), parser)) {
        return false;
    }
=======
  bool visitTypeOfExpr(ParseNode*& pn) {
    return Base::visitTypeOfExpr(pn) && FoldTypeOfExpr(cx_, handler, &pn);
  }

  bool visitDeleteExpr(ParseNode*& pn) {
    return Base::visitDeleteExpr(pn) && FoldDeleteExpr(cx_, handler, &pn);
  }
>>>>>>> upstream-releases

<<<<<<< HEAD
  if (!Fold(cx, elem->unsafeLeftReference(), parser)) {
    return false;
  }
||||||| merged common ancestors
    if (!Fold(cx, elem->unsafeRightReference(), parser)) {
        return false;
    }

    ParseNode* expr = &elem->expression();
    ParseNode* key = &elem->key();
    PropertyName* name = nullptr;
    if (key->isKind(ParseNodeKind::String)) {
        JSAtom* atom = key->as<NameNode>().atom();
        uint32_t index;

        if (atom->isIndex(&index)) {
            // Optimization 1: We have something like expr["100"]. This is
            // equivalent to expr[100] which is faster.
            key->setKind(ParseNodeKind::Number);
            key->setArity(PN_NUMBER);
            key->setOp(JSOP_DOUBLE);
            key->as<NumericLiteral>().setValue(index);
        } else {
            name = atom->asPropertyName();
        }
    } else if (key->isKind(ParseNodeKind::Number)) {
        double number = key->as<NumericLiteral>().value();
        if (number != ToUint32(number)) {
            // Optimization 2: We have something like expr[3.14]. The number
            // isn't an array index, so it converts to a string ("3.14"),
            // enabling optimization 3 below.
            JSAtom* atom = NumberToAtom(cx, number);
            if (!atom) {
                return false;
            }
            name = atom->asPropertyName();
        }
    }

    // If we don't have a name, we can't optimize to getprop.
    if (!name) {
        return true;
    }
=======
  bool visitDeleteElemExpr(ParseNode*& pn) {
    return Base::visitDeleteElemExpr(pn) &&
           FoldDeleteElement(cx_, handler, &pn);
  }

  bool visitNotExpr(ParseNode*& pn) {
    return Base::visitNotExpr(pn) && FoldNot(cx_, handler, &pn);
  }

  bool visitBitNotExpr(ParseNode*& pn) {
    return Base::visitBitNotExpr(pn) && FoldUnaryArithmetic(cx_, handler, &pn);
  }
>>>>>>> upstream-releases

<<<<<<< HEAD
  if (!Fold(cx, elem->unsafeRightReference(), parser)) {
    return false;
  }

  ParseNode* expr = &elem->expression();
  ParseNode* key = &elem->key();
  PropertyName* name = nullptr;
  if (key->isKind(ParseNodeKind::String)) {
    JSAtom* atom = key->as<NameNode>().atom();
    uint32_t index;

    if (atom->isIndex(&index)) {
      // Optimization 1: We have something like expr["100"]. This is
      // equivalent to expr[100] which is faster.
      key->setKind(ParseNodeKind::Number);
      key->setOp(JSOP_DOUBLE);
      key->as<NumericLiteral>().setValue(index);
    } else {
      name = atom->asPropertyName();
    }
  } else if (key->isKind(ParseNodeKind::Number)) {
    double number = key->as<NumericLiteral>().value();
    if (number != ToUint32(number)) {
      // Optimization 2: We have something like expr[3.14]. The number
      // isn't an array index, so it converts to a string ("3.14"),
      // enabling optimization 3 below.
      JSAtom* atom = NumberToAtom(cx, number);
      if (!atom) {
        return false;
      }
      name = atom->asPropertyName();
    }
  }
||||||| merged common ancestors
    // Optimization 3: We have expr["foo"] where foo is not an index.  Convert
    // to a property access (like expr.foo) that optimizes better downstream.
    NameNode* nameNode = parser.newPropertyName(name, key->pn_pos);
    if (!nameNode) {
        return false;
    }
    ParseNode* dottedAccess = parser.newPropertyAccess(expr, nameNode);
    if (!dottedAccess) {
        return false;
    }
    dottedAccess->setInParens(elem->isInParens());
    ReplaceNode(nodePtr, dottedAccess);
=======
  bool visitPosExpr(ParseNode*& pn) {
    return Base::visitPosExpr(pn) && FoldUnaryArithmetic(cx_, handler, &pn);
  }
>>>>>>> upstream-releases

<<<<<<< HEAD
  // If we don't have a name, we can't optimize to getprop.
  if (!name) {
    return true;
  }

  // Optimization 3: We have expr["foo"] where foo is not an index.  Convert
  // to a property access (like expr.foo) that optimizes better downstream.
  NameNode* nameNode = parser.newPropertyName(name, key->pn_pos);
  if (!nameNode) {
    return false;
  }
  ParseNode* dottedAccess = parser.newPropertyAccess(expr, nameNode);
  if (!dottedAccess) {
    return false;
  }
  dottedAccess->setInParens(elem->isInParens());
  ReplaceNode(nodePtr, dottedAccess);

  return true;
}
||||||| merged common ancestors
    return true;
}
=======
  bool visitNegExpr(ParseNode*& pn) {
    return Base::visitNegExpr(pn) && FoldUnaryArithmetic(cx_, handler, &pn);
  }
>>>>>>> upstream-releases

<<<<<<< HEAD
static bool FoldAdd(JSContext* cx, ParseNode** nodePtr,
                    PerHandlerParser<FullParseHandler>& parser) {
  ListNode* node = &(*nodePtr)->as<ListNode>();
||||||| merged common ancestors
static bool
FoldAdd(JSContext* cx, ParseNode** nodePtr, PerHandlerParser<FullParseHandler>& parser)
{
    ListNode* node = &(*nodePtr)->as<ListNode>();
=======
  bool visitPowExpr(ParseNode*& pn) {
    return Base::visitPowExpr(pn) && FoldExponentiation(cx_, handler, &pn);
  }
>>>>>>> upstream-releases

<<<<<<< HEAD
  MOZ_ASSERT(node->isKind(ParseNodeKind::Add));
  MOZ_ASSERT(node->count() >= 2);
||||||| merged common ancestors
    MOZ_ASSERT(node->isKind(ParseNodeKind::Add));
    MOZ_ASSERT(node->count() >= 2);
=======
  bool visitMulExpr(ParseNode*& pn) {
    return Base::visitMulExpr(pn) && FoldBinaryArithmetic(cx_, handler, &pn);
  }
>>>>>>> upstream-releases

<<<<<<< HEAD
  // Generically fold all operands first.
  if (!FoldList(cx, node, parser)) {
    return false;
  }

  // Fold leading numeric operands together:
  //
  //   (1 + 2 + x)  becomes  (3 + x)
  //
  // Don't go past the leading operands: additions after a string are
  // string concatenations, not additions: ("1" + 2 + 3 === "123").
  ParseNode* current = node->head();
  ParseNode* next = current->pn_next;
  if (current->isKind(ParseNodeKind::Number)) {
    do {
      if (!next->isKind(ParseNodeKind::Number)) {
        break;
      }
||||||| merged common ancestors
    // Generically fold all operands first.
    if (!FoldList(cx, node, parser)) {
        return false;
    }
=======
  bool visitDivExpr(ParseNode*& pn) {
    return Base::visitDivExpr(pn) && FoldBinaryArithmetic(cx_, handler, &pn);
  }
>>>>>>> upstream-releases

<<<<<<< HEAD
      NumericLiteral* num = &current->as<NumericLiteral>();

      num->setValue(num->value() + next->as<NumericLiteral>().value());
      current->pn_next = next->pn_next;
      next = current->pn_next;

      node->unsafeDecrementCount();
    } while (next);
  }

  // If any operands remain, attempt string concatenation folding.
  do {
    // If no operands remain, we're done.
    if (!next) {
      break;
    }
||||||| merged common ancestors
    // Fold leading numeric operands together:
    //
    //   (1 + 2 + x)  becomes  (3 + x)
    //
    // Don't go past the leading operands: additions after a string are
    // string concatenations, not additions: ("1" + 2 + 3 === "123").
    ParseNode* current = node->head();
    ParseNode* next = current->pn_next;
    if (current->isKind(ParseNodeKind::Number)) {
        do {
            if (!next->isKind(ParseNodeKind::Number)) {
                break;
            }

            NumericLiteral* num = &current->as<NumericLiteral>();

            num->setValue(num->value() + next->as<NumericLiteral>().value());
            current->pn_next = next->pn_next;
            next = current->pn_next;

            node->unsafeDecrementCount();
        } while (next);
    }
=======
  bool visitModExpr(ParseNode*& pn) {
    return Base::visitModExpr(pn) && FoldBinaryArithmetic(cx_, handler, &pn);
  }
>>>>>>> upstream-releases

<<<<<<< HEAD
    // (number + string) is string concatenation *only* at the start of
    // the list: (x + 1 + "2" !== x + "12") when x is a number.
    if (current->isKind(ParseNodeKind::Number) &&
        next->isKind(ParseNodeKind::String)) {
      if (!FoldType(cx, current, ParseNodeKind::String)) {
        return false;
      }
      next = current->pn_next;
    }

    // The first string forces all subsequent additions to be
    // string concatenations.
    do {
      if (current->isKind(ParseNodeKind::String)) {
        break;
      }
||||||| merged common ancestors
    // If any operands remain, attempt string concatenation folding.
    do {
        // If no operands remain, we're done.
        if (!next) {
            break;
        }
=======
  bool visitAddExpr(ParseNode*& pn) {
    return Base::visitAddExpr(pn) && FoldAdd(cx_, handler, &pn);
  }
>>>>>>> upstream-releases

<<<<<<< HEAD
      current = next;
      next = next->pn_next;
    } while (next);
||||||| merged common ancestors
        // (number + string) is string concatenation *only* at the start of
        // the list: (x + 1 + "2" !== x + "12") when x is a number.
        if (current->isKind(ParseNodeKind::Number) && next->isKind(ParseNodeKind::String)) {
            if (!FoldType(cx, current, ParseNodeKind::String)) {
                return false;
            }
            next = current->pn_next;
        }
=======
  bool visitSubExpr(ParseNode*& pn) {
    return Base::visitSubExpr(pn) && FoldBinaryArithmetic(cx_, handler, &pn);
  }
>>>>>>> upstream-releases

<<<<<<< HEAD
    // If there's nothing left to fold, we're done.
    if (!next) {
      break;
    }
||||||| merged common ancestors
        // The first string forces all subsequent additions to be
        // string concatenations.
        do {
            if (current->isKind(ParseNodeKind::String)) {
                break;
            }
=======
  bool visitLshExpr(ParseNode*& pn) {
    return Base::visitLshExpr(pn) && FoldBinaryArithmetic(cx_, handler, &pn);
  }
>>>>>>> upstream-releases

<<<<<<< HEAD
    RootedString combination(cx);
    RootedString tmp(cx);
    do {
      // Create a rope of the current string and all succeeding
      // constants that we can convert to strings, then atomize it
      // and replace them all with that fresh string.
      MOZ_ASSERT(current->isKind(ParseNodeKind::String));
||||||| merged common ancestors
            current = next;
            next = next->pn_next;
        } while (next);
=======
  bool visitRshExpr(ParseNode*& pn) {
    return Base::visitRshExpr(pn) && FoldBinaryArithmetic(cx_, handler, &pn);
  }
>>>>>>> upstream-releases

<<<<<<< HEAD
      combination = current->as<NameNode>().atom();
||||||| merged common ancestors
        // If there's nothing left to fold, we're done.
        if (!next) {
            break;
        }
=======
  bool visitUrshExpr(ParseNode*& pn) {
    return Base::visitUrshExpr(pn) && FoldBinaryArithmetic(cx_, handler, &pn);
  }
>>>>>>> upstream-releases

<<<<<<< HEAD
      do {
        // Try folding the next operand to a string.
        if (!FoldType(cx, next, ParseNodeKind::String)) {
          return false;
        }
||||||| merged common ancestors
        RootedString combination(cx);
        RootedString tmp(cx);
        do {
            // Create a rope of the current string and all succeeding
            // constants that we can convert to strings, then atomize it
            // and replace them all with that fresh string.
            MOZ_ASSERT(current->isKind(ParseNodeKind::String));

            combination = current->as<NameNode>().atom();

            do {
                // Try folding the next operand to a string.
                if (!FoldType(cx, next, ParseNodeKind::String)) {
                    return false;
                }

                // Stop glomming once folding doesn't produce a string.
                if (!next->isKind(ParseNodeKind::String)) {
                    break;
                }

                // Add this string to the combination and remove the node.
                tmp = next->as<NameNode>().atom();
                combination = ConcatStrings<CanGC>(cx, combination, tmp);
                if (!combination) {
                    return false;
                }

                next = next->pn_next;
                current->pn_next = next;

                node->unsafeDecrementCount();
            } while (next);

            // Replace |current|'s string with the entire combination.
            MOZ_ASSERT(current->isKind(ParseNodeKind::String));
            combination = AtomizeString(cx, combination);
            if (!combination) {
                return false;
            }
            current->as<NameNode>().setAtom(&combination->asAtom());


            // If we're out of nodes, we're done.
            if (!next) {
                break;
            }

            current = next;
            next = current->pn_next;

            // If we're out of nodes *after* the non-foldable-to-string
            // node, we're done.
            if (!next) {
                break;
            }

            // Otherwise find the next node foldable to a string, and loop.
            do {
                current = next;
                next = current->pn_next;

                if (!FoldType(cx, current, ParseNodeKind::String)) {
                    return false;
                }
                next = current->pn_next;
            } while (!current->isKind(ParseNodeKind::String) && next);
        } while (next);
    } while (false);

    MOZ_ASSERT(!next, "must have considered all nodes here");
    MOZ_ASSERT(!current->pn_next, "current node must be the last node");

    node->unsafeReplaceTail(&current->pn_next);
=======
  bool visitAndExpr(ParseNode*& pn) {
    // Note that this does result in the unfortunate fact that dead arms of this
    // node get constant folded. The same goes for visitOr.
    return Base::visitAndExpr(pn) && FoldAndOr(cx_, &pn);
  }
>>>>>>> upstream-releases

<<<<<<< HEAD
        // Stop glomming once folding doesn't produce a string.
        if (!next->isKind(ParseNodeKind::String)) {
          break;
        }
||||||| merged common ancestors
    if (node->count() == 1) {
        // We reduced the list to a constant.  Replace the ParseNodeKind::Add node
        // with that constant.
        ReplaceNode(nodePtr, current);
    }
=======
  bool visitOrExpr(ParseNode*& pn) {
    return Base::visitOrExpr(pn) && FoldAndOr(cx_, &pn);
  }
>>>>>>> upstream-releases

<<<<<<< HEAD
        // Add this string to the combination and remove the node.
        tmp = next->as<NameNode>().atom();
        combination = ConcatStrings<CanGC>(cx, combination, tmp);
        if (!combination) {
          return false;
        }
||||||| merged common ancestors
    return true;
}
=======
  bool visitConditionalExpr(ParseNode*& pn) {
    // Don't call base-class visitConditional because FoldConditional processes
    // pn's child nodes specially to save stack space.
    return FoldConditional(cx_, handler, &pn);
  }
>>>>>>> upstream-releases

<<<<<<< HEAD
        next = next->pn_next;
        current->pn_next = next;
||||||| merged common ancestors
static bool
FoldCall(JSContext* cx, BinaryNode* node, PerHandlerParser<FullParseHandler>& parser)
{
    MOZ_ASSERT(node->isKind(ParseNodeKind::Call) ||
               node->isKind(ParseNodeKind::SuperCall) ||
               node->isKind(ParseNodeKind::New) ||
               node->isKind(ParseNodeKind::TaggedTemplate));
=======
 private:
  bool internalVisitCall(BinaryNode* node) {
    MOZ_ASSERT(node->isKind(ParseNodeKind::CallExpr) ||
               node->isKind(ParseNodeKind::SuperCallExpr) ||
               node->isKind(ParseNodeKind::NewExpr) ||
               node->isKind(ParseNodeKind::TaggedTemplateExpr));
>>>>>>> upstream-releases

<<<<<<< HEAD
        node->unsafeDecrementCount();
      } while (next);

      // Replace |current|'s string with the entire combination.
      MOZ_ASSERT(current->isKind(ParseNodeKind::String));
      combination = AtomizeString(cx, combination);
      if (!combination) {
||||||| merged common ancestors
    // Don't fold a parenthesized callable component in an invocation, as this
    // might cause a different |this| value to be used, changing semantics:
    //
    //   var prop = "global";
    //   var obj = { prop: "obj", f: function() { return this.prop; } };
    //   assertEq((true ? obj.f : null)(), "global");
    //   assertEq(obj.f(), "obj");
    //   assertEq((true ? obj.f : null)``, "global");
    //   assertEq(obj.f``, "obj");
    //
    // As an exception to this, we do allow folding the function in
    // `(function() { ... })()` (the module pattern), because that lets us
    // constant fold code inside that function.
    //
    // See bug 537673 and bug 1182373.
    ParseNode* callee = node->left();
    if (node->isKind(ParseNodeKind::New) ||
        !callee->isInParens() ||
        callee->isKind(ParseNodeKind::Function))
    {
        if (!Fold(cx, node->unsafeLeftReference(), parser)) {
            return false;
        }
    }

    if (!Fold(cx, node->unsafeRightReference(), parser)) {
=======
    // Don't fold a parenthesized callable component in an invocation, as this
    // might cause a different |this| value to be used, changing semantics:
    //
    //   var prop = "global";
    //   var obj = { prop: "obj", f: function() { return this.prop; } };
    //   assertEq((true ? obj.f : null)(), "global");
    //   assertEq(obj.f(), "obj");
    //   assertEq((true ? obj.f : null)``, "global");
    //   assertEq(obj.f``, "obj");
    //
    // As an exception to this, we do allow folding the function in
    // `(function() { ... })()` (the module pattern), because that lets us
    // constant fold code inside that function.
    //
    // See bug 537673 and bug 1182373.
    ParseNode* callee = node->left();
    if (node->isKind(ParseNodeKind::NewExpr) || !callee->isInParens() ||
        callee->is<FunctionNode>()) {
      if (!visit(*node->unsafeLeftReference())) {
>>>>>>> upstream-releases
        return false;
<<<<<<< HEAD
      }
      current->as<NameNode>().setAtom(&combination->asAtom());
||||||| merged common ancestors
    }
=======
      }
    }
>>>>>>> upstream-releases

<<<<<<< HEAD
      // If we're out of nodes, we're done.
      if (!next) {
        break;
      }

      current = next;
      next = current->pn_next;

      // If we're out of nodes *after* the non-foldable-to-string
      // node, we're done.
      if (!next) {
        break;
      }

      // Otherwise find the next node foldable to a string, and loop.
      do {
        current = next;
        next = current->pn_next;

        if (!FoldType(cx, current, ParseNodeKind::String)) {
          return false;
        }
        next = current->pn_next;
      } while (!current->isKind(ParseNodeKind::String) && next);
    } while (next);
  } while (false);
||||||| merged common ancestors
    return true;
}

static bool
FoldArguments(JSContext* cx, ListNode* node, PerHandlerParser<FullParseHandler>& parser)
{
    MOZ_ASSERT(node->isKind(ParseNodeKind::Arguments));

    ParseNode** listp = node->unsafeHeadReference();
    for (; *listp; listp = &(*listp)->pn_next) {
        if (!Fold(cx, listp, parser)) {
            return false;
        }
    }
=======
    if (!visit(*node->unsafeRightReference())) {
      return false;
    }
>>>>>>> upstream-releases

<<<<<<< HEAD
  MOZ_ASSERT(!next, "must have considered all nodes here");
  MOZ_ASSERT(!current->pn_next, "current node must be the last node");

  node->unsafeReplaceTail(&current->pn_next);

  if (node->count() == 1) {
    // We reduced the list to a constant.  Replace the ParseNodeKind::Add node
    // with that constant.
    ReplaceNode(nodePtr, current);
  }

  return true;
}

static bool FoldCall(JSContext* cx, BinaryNode* node,
                     PerHandlerParser<FullParseHandler>& parser) {
  MOZ_ASSERT(node->isKind(ParseNodeKind::Call) ||
             node->isKind(ParseNodeKind::SuperCall) ||
             node->isKind(ParseNodeKind::New) ||
             node->isKind(ParseNodeKind::TaggedTemplate));

  // Don't fold a parenthesized callable component in an invocation, as this
  // might cause a different |this| value to be used, changing semantics:
  //
  //   var prop = "global";
  //   var obj = { prop: "obj", f: function() { return this.prop; } };
  //   assertEq((true ? obj.f : null)(), "global");
  //   assertEq(obj.f(), "obj");
  //   assertEq((true ? obj.f : null)``, "global");
  //   assertEq(obj.f``, "obj");
  //
  // As an exception to this, we do allow folding the function in
  // `(function() { ... })()` (the module pattern), because that lets us
  // constant fold code inside that function.
  //
  // See bug 537673 and bug 1182373.
  ParseNode* callee = node->left();
  if (node->isKind(ParseNodeKind::New) || !callee->isInParens() ||
      callee->isKind(ParseNodeKind::Function)) {
    if (!Fold(cx, node->unsafeLeftReference(), parser)) {
      return false;
||||||| merged common ancestors
    node->unsafeReplaceTail(listp);
    return true;
}

static bool
FoldForInOrOf(JSContext* cx, TernaryNode* node, PerHandlerParser<FullParseHandler>& parser)
{
    MOZ_ASSERT(node->isKind(ParseNodeKind::ForIn) ||
               node->isKind(ParseNodeKind::ForOf));
    MOZ_ASSERT(!node->kid2());

    return Fold(cx, node->unsafeKid1Reference(), parser) &&
           Fold(cx, node->unsafeKid3Reference(), parser);
}

static bool
FoldForHead(JSContext* cx, TernaryNode* node, PerHandlerParser<FullParseHandler>& parser)
{
    MOZ_ASSERT(node->isKind(ParseNodeKind::ForHead));

    ParseNode** init = node->unsafeKid1Reference();
    if (*init) {
        if (!Fold(cx, init, parser)) {
            return false;
        }
=======
    return true;
  }

 public:
  bool visitCallExpr(ParseNode*& pn) {
    return internalVisitCall(&pn->as<BinaryNode>());
  }

  bool visitNewExpr(ParseNode*& pn) {
    return internalVisitCall(&pn->as<BinaryNode>());
  }

  bool visitSuperCallExpr(ParseNode*& pn) {
    return internalVisitCall(&pn->as<BinaryNode>());
  }

  bool visitTaggedTemplateExpr(ParseNode*& pn) {
    return internalVisitCall(&pn->as<BinaryNode>());
  }

  bool visitIfStmt(ParseNode*& pn) {
    // Don't call base-class visitIf because FoldIf processes pn's child nodes
    // specially to save stack space.
    return FoldIf(cx_, handler, &pn);
  }

  bool visitForStmt(ParseNode*& pn) {
    if (!Base::visitForStmt(pn)) {
      return false;
>>>>>>> upstream-releases
    }
  }

<<<<<<< HEAD
  if (!Fold(cx, node->unsafeRightReference(), parser)) {
    return false;
  }

  return true;
}

static bool FoldArguments(JSContext* cx, ListNode* node,
                          PerHandlerParser<FullParseHandler>& parser) {
  MOZ_ASSERT(node->isKind(ParseNodeKind::Arguments));

  ParseNode** listp = node->unsafeHeadReference();
  for (; *listp; listp = &(*listp)->pn_next) {
    if (!Fold(cx, listp, parser)) {
      return false;
||||||| merged common ancestors
    ParseNode** test = node->unsafeKid2Reference();
    if (*test) {
        if (!FoldCondition(cx, test, parser)) {
            return false;
        }

        if ((*test)->isKind(ParseNodeKind::True)) {
            (*test) = nullptr;
        }
    }

    ParseNode** update = node->unsafeKid3Reference();
    if (*update) {
        if (!Fold(cx, update, parser)) {
            return false;
        }
=======
    ForNode& stmt = pn->as<ForNode>();
    if (stmt.left()->isKind(ParseNodeKind::ForHead)) {
      TernaryNode& head = stmt.left()->as<TernaryNode>();
      ParseNode** test = head.unsafeKid2Reference();
      if (*test) {
        if (!SimplifyCondition(cx_, handler, test)) {
          return false;
        }
        if ((*test)->isKind(ParseNodeKind::TrueExpr)) {
          *test = nullptr;
        }
      }
>>>>>>> upstream-releases
    }
  }

<<<<<<< HEAD
  node->unsafeReplaceTail(listp);
  return true;
}
||||||| merged common ancestors
    return true;
}
=======
    return true;
  }
>>>>>>> upstream-releases

<<<<<<< HEAD
static bool FoldForInOrOf(JSContext* cx, TernaryNode* node,
                          PerHandlerParser<FullParseHandler>& parser) {
  MOZ_ASSERT(node->isKind(ParseNodeKind::ForIn) ||
             node->isKind(ParseNodeKind::ForOf));
  MOZ_ASSERT(!node->kid2());
||||||| merged common ancestors
static bool
FoldDottedProperty(JSContext* cx, PropertyAccess* prop, PerHandlerParser<FullParseHandler>& parser)
{
    // Iterate through a long chain of dotted property accesses to find the
    // most-nested non-dotted property node, then fold that.
    ParseNode** nested = prop->unsafeLeftReference();
    while ((*nested)->isKind(ParseNodeKind::Dot)) {
        nested = (*nested)->as<PropertyAccess>().unsafeLeftReference();
    }
=======
  bool visitWhileStmt(ParseNode*& pn) {
    BinaryNode& node = pn->as<BinaryNode>();
    return Base::visitWhileStmt(pn) &&
           SimplifyCondition(cx_, handler, node.unsafeLeftReference());
  }
>>>>>>> upstream-releases

<<<<<<< HEAD
  return Fold(cx, node->unsafeKid1Reference(), parser) &&
         Fold(cx, node->unsafeKid3Reference(), parser);
}
||||||| merged common ancestors
    return Fold(cx, nested, parser);
}
=======
  bool visitDoWhileStmt(ParseNode*& pn) {
    BinaryNode& node = pn->as<BinaryNode>();
    return Base::visitDoWhileStmt(pn) &&
           SimplifyCondition(cx_, handler, node.unsafeRightReference());
  }
>>>>>>> upstream-releases

<<<<<<< HEAD
static bool FoldForHead(JSContext* cx, TernaryNode* node,
                        PerHandlerParser<FullParseHandler>& parser) {
  MOZ_ASSERT(node->isKind(ParseNodeKind::ForHead));
||||||| merged common ancestors
static bool
FoldName(JSContext* cx, NameNode* nameNode, PerHandlerParser<FullParseHandler>& parser)
{
    MOZ_ASSERT(nameNode->isKind(ParseNodeKind::Name));
=======
  bool visitFunction(ParseNode*& pn) {
    FunctionNode& node = pn->as<FunctionNode>();
>>>>>>> upstream-releases

<<<<<<< HEAD
  ParseNode** init = node->unsafeKid1Reference();
  if (*init) {
    if (!Fold(cx, init, parser)) {
      return false;
||||||| merged common ancestors
    if (!nameNode->initializer()) {
        return true;
=======
    // Don't constant-fold inside "use asm" code, as this could create a parse
    // tree that doesn't type-check as asm.js.
    if (node.funbox()->useAsmOrInsideUseAsm()) {
      return true;
>>>>>>> upstream-releases
    }
  }

<<<<<<< HEAD
  ParseNode** test = node->unsafeKid2Reference();
  if (*test) {
    if (!FoldCondition(cx, test, parser)) {
      return false;
    }
||||||| merged common ancestors
    return Fold(cx, nameNode->unsafeInitializerReference(), parser);
}
=======
    return Base::visitFunction(pn);
  }
>>>>>>> upstream-releases

<<<<<<< HEAD
    if ((*test)->isKind(ParseNodeKind::True)) {
      (*test) = nullptr;
||||||| merged common ancestors
bool
Fold(JSContext* cx, ParseNode** pnp, PerHandlerParser<FullParseHandler>& parser)
{
    if (!CheckRecursionLimit(cx)) {
        return false;
=======
  bool visitArrayExpr(ParseNode*& pn) {
    if (!Base::visitArrayExpr(pn)) {
      return false;
>>>>>>> upstream-releases
    }
  }

<<<<<<< HEAD
  ParseNode** update = node->unsafeKid3Reference();
  if (*update) {
    if (!Fold(cx, update, parser)) {
      return false;
    }
  }

  return true;
}

static bool FoldDottedProperty(JSContext* cx, PropertyAccess* prop,
                               PerHandlerParser<FullParseHandler>& parser) {
  // Iterate through a long chain of dotted property accesses to find the
  // most-nested non-dotted property node, then fold that.
  ParseNode** nested = prop->unsafeLeftReference();
  while ((*nested)->isKind(ParseNodeKind::Dot)) {
    nested = (*nested)->as<PropertyAccess>().unsafeLeftReference();
  }

  return Fold(cx, nested, parser);
}

static bool FoldName(JSContext* cx, NameNode* nameNode,
                     PerHandlerParser<FullParseHandler>& parser) {
  MOZ_ASSERT(nameNode->isKind(ParseNodeKind::Name));

  if (!nameNode->initializer()) {
    return true;
  }

  return Fold(cx, nameNode->unsafeInitializerReference(), parser);
}

bool Fold(JSContext* cx, ParseNode** pnp,
          PerHandlerParser<FullParseHandler>& parser) {
  if (!CheckRecursionLimit(cx)) {
    return false;
  }

  ParseNode* pn = *pnp;

  switch (pn->getKind()) {
    case ParseNodeKind::EmptyStatement:
    case ParseNodeKind::True:
    case ParseNodeKind::False:
    case ParseNodeKind::Null:
    case ParseNodeKind::RawUndefined:
    case ParseNodeKind::Elision:
    case ParseNodeKind::Generator:
    case ParseNodeKind::ExportBatchSpec:
    case ParseNodeKind::PosHolder:
      MOZ_ASSERT(pn->is<NullaryNode>());
      return true;

    case ParseNodeKind::Debugger:
      MOZ_ASSERT(pn->is<DebuggerStatement>());
      return true;

    case ParseNodeKind::Break:
      MOZ_ASSERT(pn->is<BreakStatement>());
      return true;

    case ParseNodeKind::Continue:
      MOZ_ASSERT(pn->is<ContinueStatement>());
      return true;

    case ParseNodeKind::ObjectPropertyName:
    case ParseNodeKind::PrivateName:
    case ParseNodeKind::String:
    case ParseNodeKind::TemplateString:
      MOZ_ASSERT(pn->is<NameNode>());
      return true;

    case ParseNodeKind::RegExp:
      MOZ_ASSERT(pn->is<RegExpLiteral>());
      return true;

    case ParseNodeKind::Number:
      MOZ_ASSERT(pn->is<NumericLiteral>());
      return true;

#ifdef ENABLE_BIGINT
    case ParseNodeKind::BigInt:
      MOZ_ASSERT(pn->is<BigIntLiteral>());
      return true;
#endif

    case ParseNodeKind::SuperBase:
    case ParseNodeKind::TypeOfName: {
#ifdef DEBUG
      UnaryNode* node = &pn->as<UnaryNode>();
      NameNode* nameNode = &node->kid()->as<NameNode>();
      MOZ_ASSERT(nameNode->isKind(ParseNodeKind::Name));
      MOZ_ASSERT(!nameNode->initializer());
#endif
      return true;
    }

    case ParseNodeKind::TypeOfExpr:
      return FoldTypeOfExpr(cx, &pn->as<UnaryNode>(), parser);

    case ParseNodeKind::DeleteName:
      MOZ_ASSERT(pn->as<UnaryNode>().kid()->isKind(ParseNodeKind::Name));
      return true;

    case ParseNodeKind::DeleteExpr:
      return FoldDeleteExpr(cx, &pn->as<UnaryNode>(), parser);

    case ParseNodeKind::DeleteElem:
      return FoldDeleteElement(cx, &pn->as<UnaryNode>(), parser);

    case ParseNodeKind::DeleteProp:
      return FoldDeleteProperty(cx, &pn->as<UnaryNode>(), parser);

    case ParseNodeKind::Conditional:
      MOZ_ASSERT((*pnp)->is<TernaryNode>());
      return FoldConditional(cx, pnp, parser);

    case ParseNodeKind::If:
      MOZ_ASSERT((*pnp)->is<TernaryNode>());
      return FoldIf(cx, pnp, parser);

    case ParseNodeKind::Not:
      return FoldNot(cx, &pn->as<UnaryNode>(), parser);

    case ParseNodeKind::BitNot:
    case ParseNodeKind::Pos:
    case ParseNodeKind::Neg:
      return FoldUnaryArithmetic(cx, &pn->as<UnaryNode>(), parser);

    case ParseNodeKind::PreIncrement:
    case ParseNodeKind::PostIncrement:
    case ParseNodeKind::PreDecrement:
    case ParseNodeKind::PostDecrement:
      return FoldIncrementDecrement(cx, &pn->as<UnaryNode>(), parser);

    case ParseNodeKind::ExpressionStatement:
    case ParseNodeKind::Throw:
    case ParseNodeKind::MutateProto:
    case ParseNodeKind::ComputedName:
    case ParseNodeKind::Spread:
    case ParseNodeKind::Export:
    case ParseNodeKind::Void:
      return Fold(cx, pn->as<UnaryNode>().unsafeKidReference(), parser);

    case ParseNodeKind::ExportDefault:
      return Fold(cx, pn->as<BinaryNode>().unsafeLeftReference(), parser);

    case ParseNodeKind::This: {
      ThisLiteral* node = &pn->as<ThisLiteral>();
      ParseNode** expr = node->unsafeKidReference();
      if (*expr) {
        return Fold(cx, expr, parser);
      }
      return true;
    }

    case ParseNodeKind::Pipeline:
      return true;

    case ParseNodeKind::And:
    case ParseNodeKind::Or:
      MOZ_ASSERT((*pnp)->is<ListNode>());
      return FoldAndOr(cx, pnp, parser);

    case ParseNodeKind::Function:
      return FoldFunction(cx, &pn->as<CodeNode>(), parser);

    case ParseNodeKind::Module:
      return FoldModule(cx, &pn->as<CodeNode>(), parser);

    case ParseNodeKind::Sub:
    case ParseNodeKind::Star:
    case ParseNodeKind::Lsh:
    case ParseNodeKind::Rsh:
    case ParseNodeKind::Ursh:
    case ParseNodeKind::Div:
    case ParseNodeKind::Mod:
      return FoldBinaryArithmetic(cx, &pn->as<ListNode>(), parser);

    case ParseNodeKind::Pow:
      return FoldExponentiation(cx, &pn->as<ListNode>(), parser);

    // Various list nodes not requiring care to minimally fold.  Some of
    // these could be further folded/optimized, but we don't make the effort.
    case ParseNodeKind::BitOr:
    case ParseNodeKind::BitXor:
    case ParseNodeKind::BitAnd:
    case ParseNodeKind::StrictEq:
    case ParseNodeKind::Eq:
    case ParseNodeKind::StrictNe:
    case ParseNodeKind::Ne:
    case ParseNodeKind::Lt:
    case ParseNodeKind::Le:
    case ParseNodeKind::Gt:
    case ParseNodeKind::Ge:
    case ParseNodeKind::InstanceOf:
    case ParseNodeKind::In:
    case ParseNodeKind::Comma:
    case ParseNodeKind::Array:
    case ParseNodeKind::Object:
    case ParseNodeKind::StatementList:
    case ParseNodeKind::ClassMemberList:
    case ParseNodeKind::TemplateStringList:
    case ParseNodeKind::Var:
    case ParseNodeKind::Const:
    case ParseNodeKind::Let:
    case ParseNodeKind::ParamsBody:
    case ParseNodeKind::CallSiteObj:
    case ParseNodeKind::ExportSpecList:
    case ParseNodeKind::ImportSpecList:
      return FoldList(cx, &pn->as<ListNode>(), parser);

    case ParseNodeKind::InitialYield: {
#ifdef DEBUG
      AssignmentNode* assignNode =
          &pn->as<UnaryNode>().kid()->as<AssignmentNode>();
      MOZ_ASSERT(assignNode->left()->isKind(ParseNodeKind::Name));
      MOZ_ASSERT(assignNode->right()->isKind(ParseNodeKind::Generator));
#endif
      return true;
    }

    case ParseNodeKind::YieldStar:
      return Fold(cx, pn->as<UnaryNode>().unsafeKidReference(), parser);

    case ParseNodeKind::Yield:
    case ParseNodeKind::Await: {
      UnaryNode* node = &pn->as<UnaryNode>();
      if (!node->kid()) {
        return true;
      }
      return Fold(cx, node->unsafeKidReference(), parser);
    }

    case ParseNodeKind::Return:
      return FoldReturn(cx, &pn->as<UnaryNode>(), parser);

    case ParseNodeKind::Try:
      return FoldTry(cx, &pn->as<TryNode>(), parser);

    case ParseNodeKind::Catch:
      return FoldCatch(cx, &pn->as<BinaryNode>(), parser);

    case ParseNodeKind::Class:
      return FoldClass(cx, &pn->as<ClassNode>(), parser);

    case ParseNodeKind::Elem: {
      MOZ_ASSERT((*pnp)->is<PropertyByValue>());
      return FoldElement(cx, pnp, parser);
    }

    case ParseNodeKind::Add:
      MOZ_ASSERT((*pnp)->is<ListNode>());
      return FoldAdd(cx, pnp, parser);

    case ParseNodeKind::Call:
    case ParseNodeKind::New:
    case ParseNodeKind::SuperCall:
    case ParseNodeKind::TaggedTemplate:
      return FoldCall(cx, &pn->as<BinaryNode>(), parser);

    case ParseNodeKind::Arguments:
      return FoldArguments(cx, &pn->as<ListNode>(), parser);

    case ParseNodeKind::Switch:
    case ParseNodeKind::Colon:
    case ParseNodeKind::Assign:
    case ParseNodeKind::AddAssign:
    case ParseNodeKind::SubAssign:
    case ParseNodeKind::BitOrAssign:
    case ParseNodeKind::BitAndAssign:
    case ParseNodeKind::BitXorAssign:
    case ParseNodeKind::LshAssign:
    case ParseNodeKind::RshAssign:
    case ParseNodeKind::UrshAssign:
    case ParseNodeKind::DivAssign:
    case ParseNodeKind::ModAssign:
    case ParseNodeKind::MulAssign:
    case ParseNodeKind::PowAssign:
    case ParseNodeKind::Import:
    case ParseNodeKind::ExportFrom:
    case ParseNodeKind::Shorthand:
    case ParseNodeKind::For:
    case ParseNodeKind::ClassMethod:
    case ParseNodeKind::ImportSpec:
    case ParseNodeKind::ExportSpec:
    case ParseNodeKind::SetThis: {
      BinaryNode* node = &pn->as<BinaryNode>();
      return Fold(cx, node->unsafeLeftReference(), parser) &&
             Fold(cx, node->unsafeRightReference(), parser);
    }

    case ParseNodeKind::ClassField: {
      ClassField* node = &pn->as<ClassField>();
      if (node->hasInitializer()) {
        if (!Fold(cx, node->unsafeInitializerReference(), parser)) {
          return false;
        }
      }

      return true;
    }
||||||| merged common ancestors
    ParseNode* pn = *pnp;

    switch (pn->getKind()) {
      case ParseNodeKind::EmptyStatement:
      case ParseNodeKind::True:
      case ParseNodeKind::False:
      case ParseNodeKind::Null:
      case ParseNodeKind::RawUndefined:
      case ParseNodeKind::Elision:
      case ParseNodeKind::Generator:
      case ParseNodeKind::ExportBatchSpec:
      case ParseNodeKind::PosHolder:
        MOZ_ASSERT(pn->is<NullaryNode>());
        return true;

      case ParseNodeKind::Debugger:
        MOZ_ASSERT(pn->is<DebuggerStatement>());
        return true;

      case ParseNodeKind::Break:
        MOZ_ASSERT(pn->is<BreakStatement>());
        return true;

      case ParseNodeKind::Continue:
        MOZ_ASSERT(pn->is<ContinueStatement>());
        return true;

      case ParseNodeKind::ObjectPropertyName:
      case ParseNodeKind::String:
      case ParseNodeKind::TemplateString:
        MOZ_ASSERT(pn->is<NameNode>());
        return true;

      case ParseNodeKind::RegExp:
        MOZ_ASSERT(pn->is<RegExpLiteral>());
        return true;

      case ParseNodeKind::Number:
        MOZ_ASSERT(pn->is<NumericLiteral>());
        return true;

      case ParseNodeKind::SuperBase:
      case ParseNodeKind::TypeOfName: {
#ifdef DEBUG
        UnaryNode* node = &pn->as<UnaryNode>();
        NameNode* nameNode = &node->kid()->as<NameNode>();
        MOZ_ASSERT(nameNode->isKind(ParseNodeKind::Name));
        MOZ_ASSERT(!nameNode->initializer());
#endif
        return true;
      }

      case ParseNodeKind::TypeOfExpr:
        return FoldTypeOfExpr(cx, &pn->as<UnaryNode>(), parser);

      case ParseNodeKind::DeleteName:
        MOZ_ASSERT(pn->as<UnaryNode>().kid()->isKind(ParseNodeKind::Name));
        return true;

      case ParseNodeKind::DeleteExpr:
        return FoldDeleteExpr(cx, &pn->as<UnaryNode>(), parser);

      case ParseNodeKind::DeleteElem:
        return FoldDeleteElement(cx, &pn->as<UnaryNode>(), parser);

      case ParseNodeKind::DeleteProp:
        return FoldDeleteProperty(cx, &pn->as<UnaryNode>(), parser);

      case ParseNodeKind::Conditional:
        MOZ_ASSERT((*pnp)->is<TernaryNode>());
        return FoldConditional(cx, pnp, parser);

      case ParseNodeKind::If:
        MOZ_ASSERT((*pnp)->is<TernaryNode>());
        return FoldIf(cx, pnp, parser);

      case ParseNodeKind::Not:
        return FoldNot(cx, &pn->as<UnaryNode>(), parser);

      case ParseNodeKind::BitNot:
      case ParseNodeKind::Pos:
      case ParseNodeKind::Neg:
        return FoldUnaryArithmetic(cx, &pn->as<UnaryNode>(), parser);

      case ParseNodeKind::PreIncrement:
      case ParseNodeKind::PostIncrement:
      case ParseNodeKind::PreDecrement:
      case ParseNodeKind::PostDecrement:
        return FoldIncrementDecrement(cx, &pn->as<UnaryNode>(), parser);

      case ParseNodeKind::ExpressionStatement:
      case ParseNodeKind::Throw:
      case ParseNodeKind::MutateProto:
      case ParseNodeKind::ComputedName:
      case ParseNodeKind::Spread:
      case ParseNodeKind::Export:
      case ParseNodeKind::Void:
        return Fold(cx, pn->as<UnaryNode>().unsafeKidReference(), parser);

      case ParseNodeKind::ExportDefault:
        return Fold(cx, pn->as<BinaryNode>().unsafeLeftReference(), parser);

      case ParseNodeKind::This: {
        ThisLiteral* node = &pn->as<ThisLiteral>();
        ParseNode** expr = node->unsafeKidReference();
        if (*expr) {
            return Fold(cx, expr, parser);
        }
        return true;
      }

      case ParseNodeKind::Pipeline:
        return true;

      case ParseNodeKind::And:
      case ParseNodeKind::Or:
        MOZ_ASSERT((*pnp)->is<ListNode>());
        return FoldAndOr(cx, pnp, parser);

      case ParseNodeKind::Function:
        return FoldFunction(cx, &pn->as<CodeNode>(), parser);

      case ParseNodeKind::Module:
        return FoldModule(cx, &pn->as<CodeNode>(), parser);

      case ParseNodeKind::Sub:
      case ParseNodeKind::Star:
      case ParseNodeKind::Lsh:
      case ParseNodeKind::Rsh:
      case ParseNodeKind::Ursh:
      case ParseNodeKind::Div:
      case ParseNodeKind::Mod:
        return FoldBinaryArithmetic(cx, &pn->as<ListNode>(), parser);

      case ParseNodeKind::Pow:
        return FoldExponentiation(cx, &pn->as<ListNode>(), parser);

      // Various list nodes not requiring care to minimally fold.  Some of
      // these could be further folded/optimized, but we don't make the effort.
      case ParseNodeKind::BitOr:
      case ParseNodeKind::BitXor:
      case ParseNodeKind::BitAnd:
      case ParseNodeKind::StrictEq:
      case ParseNodeKind::Eq:
      case ParseNodeKind::StrictNe:
      case ParseNodeKind::Ne:
      case ParseNodeKind::Lt:
      case ParseNodeKind::Le:
      case ParseNodeKind::Gt:
      case ParseNodeKind::Ge:
      case ParseNodeKind::InstanceOf:
      case ParseNodeKind::In:
      case ParseNodeKind::Comma:
      case ParseNodeKind::Array:
      case ParseNodeKind::Object:
      case ParseNodeKind::StatementList:
      case ParseNodeKind::ClassMethodList:
      case ParseNodeKind::TemplateStringList:
      case ParseNodeKind::Var:
      case ParseNodeKind::Const:
      case ParseNodeKind::Let:
      case ParseNodeKind::ParamsBody:
      case ParseNodeKind::CallSiteObj:
      case ParseNodeKind::ExportSpecList:
      case ParseNodeKind::ImportSpecList:
        return FoldList(cx, &pn->as<ListNode>(), parser);

      case ParseNodeKind::InitialYield: {
#ifdef DEBUG
        AssignmentNode* assignNode = &pn->as<UnaryNode>().kid()->as<AssignmentNode>();
        MOZ_ASSERT(assignNode->left()->isKind(ParseNodeKind::Name));
        MOZ_ASSERT(assignNode->right()->isKind(ParseNodeKind::Generator));
#endif
        return true;
      }

      case ParseNodeKind::YieldStar:
        return Fold(cx, pn->as<UnaryNode>().unsafeKidReference(), parser);

      case ParseNodeKind::Yield:
      case ParseNodeKind::Await: {
        UnaryNode* node = &pn->as<UnaryNode>();
        if (!node->kid()) {
            return true;
        }
        return Fold(cx, node->unsafeKidReference(), parser);
      }

      case ParseNodeKind::Return:
        return FoldReturn(cx, &pn->as<UnaryNode>(), parser);

      case ParseNodeKind::Try:
        return FoldTry(cx, &pn->as<TryNode>(), parser);

      case ParseNodeKind::Catch:
        return FoldCatch(cx, &pn->as<BinaryNode>(), parser);

      case ParseNodeKind::Class:
        return FoldClass(cx, &pn->as<ClassNode>(), parser);

      case ParseNodeKind::Elem: {
        MOZ_ASSERT((*pnp)->is<PropertyByValue>());
        return FoldElement(cx, pnp, parser);
      }

      case ParseNodeKind::Add:
        MOZ_ASSERT((*pnp)->is<ListNode>());
        return FoldAdd(cx, pnp, parser);

      case ParseNodeKind::Call:
      case ParseNodeKind::New:
      case ParseNodeKind::SuperCall:
      case ParseNodeKind::TaggedTemplate:
        return FoldCall(cx, &pn->as<BinaryNode>(), parser);

      case ParseNodeKind::Arguments:
        return FoldArguments(cx, &pn->as<ListNode>(), parser);

      case ParseNodeKind::Switch:
      case ParseNodeKind::Colon:
      case ParseNodeKind::Assign:
      case ParseNodeKind::AddAssign:
      case ParseNodeKind::SubAssign:
      case ParseNodeKind::BitOrAssign:
      case ParseNodeKind::BitAndAssign:
      case ParseNodeKind::BitXorAssign:
      case ParseNodeKind::LshAssign:
      case ParseNodeKind::RshAssign:
      case ParseNodeKind::UrshAssign:
      case ParseNodeKind::DivAssign:
      case ParseNodeKind::ModAssign:
      case ParseNodeKind::MulAssign:
      case ParseNodeKind::PowAssign:
      case ParseNodeKind::Import:
      case ParseNodeKind::ExportFrom:
      case ParseNodeKind::Shorthand:
      case ParseNodeKind::For:
      case ParseNodeKind::ClassMethod:
      case ParseNodeKind::ImportSpec:
      case ParseNodeKind::ExportSpec:
      case ParseNodeKind::SetThis: {
        BinaryNode* node = &pn->as<BinaryNode>();
        return Fold(cx, node->unsafeLeftReference(), parser) &&
               Fold(cx, node->unsafeRightReference(), parser);
      }
=======
    ListNode* list = &pn->as<ListNode>();
    // Empty arrays are non-constant, since we cannot easily determine their
    // type.
    if (list->hasNonConstInitializer() && list->count() > 0) {
      for (ParseNode* node : list->contents()) {
        if (!node->isConstant()) {
          return true;
        }
      }
      list->unsetHasNonConstInitializer();
    }
    return true;
  }
>>>>>>> upstream-releases

<<<<<<< HEAD
    case ParseNodeKind::NewTarget:
    case ParseNodeKind::ImportMeta: {
#ifdef DEBUG
      BinaryNode* node = &pn->as<BinaryNode>();
      MOZ_ASSERT(node->left()->isKind(ParseNodeKind::PosHolder));
      MOZ_ASSERT(node->right()->isKind(ParseNodeKind::PosHolder));
#endif
      return true;
    }
||||||| merged common ancestors
      case ParseNodeKind::NewTarget:
      case ParseNodeKind::ImportMeta: {
#ifdef DEBUG
        BinaryNode* node = &pn->as<BinaryNode>();
        MOZ_ASSERT(node->left()->isKind(ParseNodeKind::PosHolder));
        MOZ_ASSERT(node->right()->isKind(ParseNodeKind::PosHolder));
#endif
        return true;
      }
=======
  bool visitObjectExpr(ParseNode*& pn) {
    if (!Base::visitObjectExpr(pn)) {
      return false;
    }
>>>>>>> upstream-releases

<<<<<<< HEAD
    case ParseNodeKind::CallImport: {
      BinaryNode* node = &pn->as<BinaryNode>();
      MOZ_ASSERT(node->left()->isKind(ParseNodeKind::PosHolder));
      return Fold(cx, node->unsafeRightReference(), parser);
    }

    case ParseNodeKind::ClassNames: {
      ClassNames* names = &pn->as<ClassNames>();
      if (names->outerBinding()) {
        if (!Fold(cx, names->unsafeLeftReference(), parser)) {
          return false;
||||||| merged common ancestors
      case ParseNodeKind::ClassNames: {
        ClassNames* names = &pn->as<ClassNames>();
        if (names->outerBinding()) {
            if (!Fold(cx, names->unsafeLeftReference(), parser)) {
                return false;
            }
=======
    ListNode* list = &pn->as<ListNode>();
    if (list->hasNonConstInitializer()) {
      for (ParseNode* node : list->contents()) {
        if (node->getKind() != ParseNodeKind::PropertyDefinition) {
          return true;
>>>>>>> upstream-releases
        }
<<<<<<< HEAD
      }
      return Fold(cx, names->unsafeRightReference(), parser);
    }

    case ParseNodeKind::DoWhile: {
      BinaryNode* node = &pn->as<BinaryNode>();
      return Fold(cx, node->unsafeLeftReference(), parser) &&
             FoldCondition(cx, node->unsafeRightReference(), parser);
    }

    case ParseNodeKind::While: {
      BinaryNode* node = &pn->as<BinaryNode>();
      return FoldCondition(cx, node->unsafeLeftReference(), parser) &&
             Fold(cx, node->unsafeRightReference(), parser);
    }

    case ParseNodeKind::Case: {
      CaseClause* caseClause = &pn->as<CaseClause>();

      // left (caseExpression) is null for DefaultClauses.
      if (caseClause->left()) {
        if (!Fold(cx, caseClause->unsafeLeftReference(), parser)) {
          return false;
||||||| merged common ancestors
        return Fold(cx, names->unsafeRightReference(), parser);
      }

      case ParseNodeKind::DoWhile: {
        BinaryNode* node = &pn->as<BinaryNode>();
        return Fold(cx, node->unsafeLeftReference(), parser) &&
               FoldCondition(cx, node->unsafeRightReference(), parser);
      }

      case ParseNodeKind::While: {
        BinaryNode* node = &pn->as<BinaryNode>();
        return FoldCondition(cx, node->unsafeLeftReference(), parser) &&
               Fold(cx, node->unsafeRightReference(), parser);
      }

      case ParseNodeKind::Case: {
        CaseClause* caseClause = &pn->as<CaseClause>();

        // left (caseExpression) is null for DefaultClauses.
        if (caseClause->left()) {
            if (!Fold(cx, caseClause->unsafeLeftReference(), parser)) {
                return false;
            }
=======
        BinaryNode* binary = &node->as<BinaryNode>();
        if (binary->left()->isKind(ParseNodeKind::ComputedName)) {
          return true;
>>>>>>> upstream-releases
        }
<<<<<<< HEAD
      }
      return Fold(cx, caseClause->unsafeRightReference(), parser);
    }

    case ParseNodeKind::With: {
      BinaryNode* node = &pn->as<BinaryNode>();
      return Fold(cx, node->unsafeLeftReference(), parser) &&
             Fold(cx, node->unsafeRightReference(), parser);
    }

    case ParseNodeKind::ForIn:
    case ParseNodeKind::ForOf:
      return FoldForInOrOf(cx, &pn->as<TernaryNode>(), parser);

    case ParseNodeKind::ForHead:
      return FoldForHead(cx, &pn->as<TernaryNode>(), parser);

    case ParseNodeKind::Label:
      return Fold(cx, pn->as<LabeledStatement>().unsafeStatementReference(),
                  parser);

    case ParseNodeKind::PropertyName:
      MOZ_CRASH("unreachable, handled by ::Dot");

    case ParseNodeKind::Dot:
      return FoldDottedProperty(cx, &pn->as<PropertyAccess>(), parser);

    case ParseNodeKind::LexicalScope: {
      LexicalScopeNode* node = &pn->as<LexicalScopeNode>();
      if (!node->scopeBody()) {
        return true;
||||||| merged common ancestors
        return Fold(cx, caseClause->unsafeRightReference(), parser);
      }

      case ParseNodeKind::With: {
        BinaryNode* node = &pn->as<BinaryNode>();
        return Fold(cx, node->unsafeLeftReference(), parser) &&
               Fold(cx, node->unsafeRightReference(), parser);
      }

      case ParseNodeKind::ForIn:
      case ParseNodeKind::ForOf:
        return FoldForInOrOf(cx, &pn->as<TernaryNode>(), parser);

      case ParseNodeKind::ForHead:
        return FoldForHead(cx, &pn->as<TernaryNode>(), parser);

      case ParseNodeKind::Label:
        return Fold(cx, pn->as<LabeledStatement>().unsafeStatementReference(), parser);

      case ParseNodeKind::PropertyName:
        MOZ_CRASH("unreachable, handled by ::Dot");

      case ParseNodeKind::Dot:
        return FoldDottedProperty(cx, &pn->as<PropertyAccess>(), parser);

      case ParseNodeKind::LexicalScope: {
        LexicalScopeNode* node = &pn->as<LexicalScopeNode>();
        if (!node->scopeBody()) {
            return true;
        }
        return Fold(cx, node->unsafeScopeBodyReference(), parser);
=======
        if (!binary->right()->isConstant()) {
          return true;
        }
>>>>>>> upstream-releases
      }
<<<<<<< HEAD
      return Fold(cx, node->unsafeScopeBodyReference(), parser);
    }

    case ParseNodeKind::Name:
      return FoldName(cx, &pn->as<NameNode>(), parser);

    case ParseNodeKind::Limit:  // invalid sentinel value
      MOZ_CRASH("invalid node kind");
  }
||||||| merged common ancestors

      case ParseNodeKind::Name:
        return FoldName(cx, &pn->as<NameNode>(), parser);

      case ParseNodeKind::Limit: // invalid sentinel value
        MOZ_CRASH("invalid node kind");
    }
=======
      list->unsetHasNonConstInitializer();
    }
    return true;
  }
};
>>>>>>> upstream-releases

<<<<<<< HEAD
  MOZ_CRASH("shouldn't reach here");
  return false;
||||||| merged common ancestors
    MOZ_CRASH("shouldn't reach here");
    return false;
=======
bool Fold(JSContext* cx, FullParseHandler* handler, ParseNode** pnp) {
  FoldVisitor visitor(cx, handler);
  return visitor.visit(*pnp);
>>>>>>> upstream-releases
}

<<<<<<< HEAD
bool frontend::FoldConstants(JSContext* cx, ParseNode** pnp,
                             PerHandlerParser<FullParseHandler>* parser) {
  // Don't constant-fold inside "use asm" code, as this could create a parse
  // tree that doesn't type-check as asm.js.
  if (parser->pc->useAsmOrInsideUseAsm()) {
    return true;
  }
||||||| merged common ancestors
bool
frontend::FoldConstants(JSContext* cx, ParseNode** pnp, PerHandlerParser<FullParseHandler>* parser)
{
    // Don't constant-fold inside "use asm" code, as this could create a parse
    // tree that doesn't type-check as asm.js.
    if (parser->pc->useAsmOrInsideUseAsm()) {
        return true;
    }
=======
bool frontend::FoldConstants(JSContext* cx, ParseNode** pnp,
                             FullParseHandler* handler) {
  AutoTraceLog traceLog(TraceLoggerForCurrentThread(cx),
                        TraceLogger_BytecodeFoldConstants);
>>>>>>> upstream-releases

<<<<<<< HEAD
  AutoTraceLog traceLog(TraceLoggerForCurrentThread(cx),
                        TraceLogger_BytecodeFoldConstants);
  return Fold(cx, pnp, *parser);
||||||| merged common ancestors
    AutoTraceLog traceLog(TraceLoggerForCurrentThread(cx), TraceLogger_BytecodeFoldConstants);
    return Fold(cx, pnp, *parser);
=======
  return Fold(cx, handler, pnp);
>>>>>>> upstream-releases
}
