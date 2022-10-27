//
// Created by Nafour on 27/10/22.
//

#include "VariableExtractionInterface.h"

#include <queue>

namespace DE {
unordered_set<string> VariableExtractionInterface::getVariablesFromCondExprNode(
    shared_ptr<CondExprNode> condExprNode) {
  unordered_set<string> ans;
  if (!condExprNode) return ans;
  queue<shared_ptr<CondExprNode>> queue;
  queue.push(condExprNode);

  while (!queue.empty()) {
    auto curr = queue.front();
    queue.pop();
    if (curr->relExprNode) {
      auto setL = getVariablesFromExprNode(curr->relExprNode->exprNodeLHS);
      auto setR = getVariablesFromExprNode(curr->relExprNode->exprNodeRHS);
      ans.insert(setL.begin(), setL.end());
      ans.insert(setR.begin(), setR.end());
    }
    if (curr->condExprLHS) queue.push(curr->condExprLHS);
    if (curr->condExprRHS) queue.push(curr->condExprRHS);
  }
  return ans;
}

unordered_set<string> VariableExtractionInterface::getVariablesFromExprNode(
    shared_ptr<ExprNode> exprNode) {
  unordered_set<string> ans;
  if (!exprNode) return ans;
  queue<shared_ptr<ExprNode>> queue;
  queue.push(exprNode);

  while (!queue.empty()) {
    auto currNode = queue.front();
    queue.pop();
    if (Utils::isValidName(currNode->expr)) {
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
}  // namespace DE