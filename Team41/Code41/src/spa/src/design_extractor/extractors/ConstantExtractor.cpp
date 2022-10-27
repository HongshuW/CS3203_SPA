//
// Created by Nafour on 27/10/22.
//

#include "ConstantExtractor.h"

#include <queue>

#include "AST/utils/ASTUtils.h"
#include "design_extractor/results/EntityResult.h"
namespace DE {
ConstantExtractor::ConstantExtractor(shared_ptr<DataModifier> dataModifier,
                                     shared_ptr<ProgramNode> programNode)
    : EntitiesExtractor(dataModifier, programNode) {}

shared_ptr<unordered_set<string>> ConstantExtractor::extractConstants(
    shared_ptr<StmtNode> stmtNode) {
  shared_ptr<unordered_set<string>> set = make_shared<unordered_set<string>>();
  NodeType nodeType = ASTUtils::getNodeType(stmtNode);
  switch (nodeType) {
    case AST::ASSIGN_NODE: {
      shared_ptr<AssignNode> assignNode =
          dynamic_pointer_cast<AssignNode>(stmtNode);
      unordered_set<string> constants =
          getConstantsFromExprNode(assignNode->expressionNode);
      set->insert(constants.begin(), constants.end());
      break;
    }
    case AST::IF_NODE: {
      shared_ptr<IfNode> ifNode = dynamic_pointer_cast<IfNode>(stmtNode);
      unordered_set<string> constants =
          getConstantsFromCondExprNode(ifNode->condExpr);
      set->insert(constants.begin(), constants.end());
      break;
    }
    case AST::WHILE_NODE: {
      shared_ptr<WhileNode> whileNode =
          dynamic_pointer_cast<WhileNode>(stmtNode);
      unordered_set<string> constants =
          getConstantsFromCondExprNode(whileNode->condExpr);
      set->insert(constants.begin(), constants.end());
      break;
    }
    default: {
      break;
    }
  }
  return set;
}

unordered_set<string> ConstantExtractor::getConstantsFromExprNode(
    shared_ptr<ExprNode> exprNode) {
  unordered_set<string> ans;
  if (!exprNode) return ans;
  queue<shared_ptr<ExprNode>> queue;
  queue.push(exprNode);

  while (!queue.empty()) {
    auto currNode = queue.front();
    queue.pop();
    if (Utils::isValidNumber(currNode->expr)) {
      ans.insert(currNode->expr);
    }
    if (currNode->left) {
      queue.push(currNode->left);
    }
    if (currNode->right) {
      queue.push(currNode->right);
    }
  }
  return ans;
}

unordered_set<string> ConstantExtractor::getConstantsFromCondExprNode(
    shared_ptr<CondExprNode> condExprNode) {
  unordered_set<string> ans;
  if (!condExprNode) return ans;
  queue<shared_ptr<CondExprNode>> queue;
  queue.push(condExprNode);
  while (!queue.empty()) {
    auto curr = queue.front();
    queue.pop();
    if (curr->relExprNode) {
      auto setL = getConstantsFromExprNode(curr->relExprNode->exprNodeLHS);
      auto setR = getConstantsFromExprNode(curr->relExprNode->exprNodeRHS);
      ans.insert(setL.begin(), setL.end());
      ans.insert(setR.begin(), setR.end());
    }
    if (curr->condExprLHS) queue.push(curr->condExprLHS);
    if (curr->condExprRHS) queue.push(curr->condExprRHS);
  }
  return ans;
}

shared_ptr<ExtractorResult> ConstantExtractor::extract() {
  shared_ptr<unordered_set<string>> output =
      make_shared<unordered_set<string>>();
  for (const auto& stmtNode : stmtNodeList) {
    auto variableSet = extractConstants(stmtNode);
    output->insert(variableSet->begin(), variableSet->end());
  }
  shared_ptr<list<string>> entityResultList = make_shared<list<string>>();
  auto it = entityResultList->begin();
  for (const auto& elem : *output) {
    it = entityResultList->insert(it, elem);
  }
  return make_shared<EntityResult>(entityResultList);
}

void ConstantExtractor::save(shared_ptr<ExtractorResult> result) {
  shared_ptr<EntityResult> entityResult =
      static_pointer_cast<EntityResult>(result);
  dataModifier->saveConstants(*entityResult->getResult());
}

}  // namespace DE