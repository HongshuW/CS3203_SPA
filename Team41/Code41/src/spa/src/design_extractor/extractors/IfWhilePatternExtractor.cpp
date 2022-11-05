//
// Created by Nafour on 27/10/22.
//

#include "IfWhilePatternExtractor.h"

#include <utility>
namespace DE {

IfWhilePatternExtractor::IfWhilePatternExtractor(
    shared_ptr<DataModifier> dataModifier, shared_ptr<ProgramNode> programNode)
    : AbstractDesignExtractor(std::move(dataModifier), std::move(programNode)) {
  output = make_shared<list<vector<string>>>();
}

unordered_set<string> IfWhilePatternExtractor::condExprNodeHandler(
    const shared_ptr<CondExprNode>& condExpr) {
  vector<shared_ptr<RelExprNode>> relExprNodeList;
  unordered_set<string> varList;

  if (condExpr->relExprNode) {
    relExprNodeList.push_back(condExpr->relExprNode);
  }

  if (condExpr->condExprLHS) {
    getRelExprNodesDFS(condExpr->condExprLHS, relExprNodeList);
  }

  if (condExpr->condExprRHS) {
    getRelExprNodesDFS(condExpr->condExprRHS, relExprNodeList);
  }

  for (const auto& relExpr : relExprNodeList) {
    relExprNodeHandler(relExpr, varList);
  }
  return varList;
}

void IfWhilePatternExtractor::relExprNodeHandler(
    const shared_ptr<RelExprNode>& relExpr, unordered_set<string> &varList) {
  auto exprLHS = relExpr->exprNodeLHS;
  auto exprRHS = relExpr->exprNodeRHS;
  exprNodeHandler(exprLHS, varList);
  exprNodeHandler(exprRHS, varList);
}

void IfWhilePatternExtractor::exprNodeHandler(const shared_ptr<ExprNode>& expr,
                                              unordered_set<string> &varList) {
  getVarFromExprNodesDFS(expr, varList);
}

void IfWhilePatternExtractor::getVarFromExprNodesDFS(
    const shared_ptr<ExprNode>& expr, unordered_set<string> &varList) {
  if (expr->isVariableNode()) {
    varList.insert(expr->expr);
  }

  if (expr->isOperatorNode()) {
    auto LHS = expr->left;
    auto RHS = expr->right;
    getVarFromExprNodesDFS(LHS, varList);
    getVarFromExprNodesDFS(RHS, varList);
  }
}

void IfWhilePatternExtractor::getRelExprNodesDFS(
    const shared_ptr<CondExprNode>& condExpr,
    vector<shared_ptr<RelExprNode>> &relExprNodeList) {
  if (condExpr->relExprNode) {
    relExprNodeList.push_back(condExpr->relExprNode);
  }

  if (condExpr->condExprLHS) {
    getRelExprNodesDFS(condExpr->condExprLHS, relExprNodeList);
  }

  if (condExpr->condExprRHS) {
    getRelExprNodesDFS(condExpr->condExprRHS, relExprNodeList);
  }
}
}  // namespace DE