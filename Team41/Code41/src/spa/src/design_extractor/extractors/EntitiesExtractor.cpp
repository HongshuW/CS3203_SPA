//
// Created by Nafour on 26/10/22.
//

#include "EntitiesExtractor.h"

#include <queue>

#include "AST/ASTNode.h"
#include "AST/utils/ASTUtils.h"

namespace DE {

EntitiesExtractor::EntitiesExtractor(shared_ptr<DataModifier> dataModifier,
                                     shared_ptr<ProgramNode> programNode)
    : AbstractDesignExtractor(dataModifier, programNode) {
  stmtNodeList = extractStmtNodes();
}

shared_ptr<unordered_set<string>> EntitiesExtractor::extractVariables(
    shared_ptr<StmtNode> stmtNode) {
  shared_ptr<unordered_set<string>> set = make_shared<unordered_set<string>>();
  NodeType nodeType = ASTUtils::getNodeType(stmtNode);
  switch (nodeType) {
    case AST::PRINT_NODE: {
      shared_ptr<PrintNode> printNode =
          dynamic_pointer_cast<PrintNode>(stmtNode);
      set->insert(printNode->variableNode->variable);
      break;
    }
    case AST::ASSIGN_NODE: {
      shared_ptr<AssignNode> assignNode =
          dynamic_pointer_cast<AssignNode>(stmtNode);
      set->insert(assignNode->variableNode->variable);
      unordered_set<string> variables =
          getVariablesFromExprNode(assignNode->expressionNode);
      set->insert(variables.begin(), variables.end());
      break;
    }
    case AST::IF_NODE: {
      shared_ptr<IfNode> ifNode = dynamic_pointer_cast<IfNode>(stmtNode);
      unordered_set<string> variables =
          getVariablesFromCondExprNode(ifNode->condExpr);
      set->insert(variables.begin(), variables.end());
      break;
    }
    case AST::READ_NODE: {
      shared_ptr<ReadNode> readNode = dynamic_pointer_cast<ReadNode>(stmtNode);
      set->insert(readNode->variableNode->variable);
      break;
    }
    case AST::WHILE_NODE: {
      shared_ptr<WhileNode> whileNode =
          dynamic_pointer_cast<WhileNode>(stmtNode);
      unordered_set<string> variables =
          getVariablesFromCondExprNode(whileNode->condExpr);
      set->insert(variables.begin(), variables.end());
      break;
    }
    default: {
      break;
    }
  }
  return set;
}

shared_ptr<unordered_set<string>> EntitiesExtractor::extractConstants(
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
  ;
}

shared_ptr<unordered_set<string>> EntitiesExtractor::extractAllVariables() {
  shared_ptr<unordered_set<string>> output =
      make_shared<unordered_set<string>>();
  for (const auto& stmtNode : stmtNodeList) {
    auto variableSet = extractVariables(stmtNode);
    output->insert(variableSet->begin(), variableSet->end());
  }
  return output;
}

vector<shared_ptr<StmtNode>> EntitiesExtractor::extractStmtNodes() {
  vector<shared_ptr<StmtNode>> output;
  auto procedureList = programNode->procedureList;
  for (const auto& proc : procedureList) {
    auto stmtList = proc->stmtList;
    queue<shared_ptr<StmtNode>> queue;
    for (const auto& stmtNode : stmtList) {
      queue.push(stmtNode);
    }
    while (!queue.empty()) {
      auto node = queue.front();
      queue.pop();
      NodeType nodeType = ASTUtils::getNodeType(node);
      switch (nodeType) {
        case AST::IF_NODE: {
          output.push_back(node);
          shared_ptr<IfNode> ifNode = dynamic_pointer_cast<IfNode>(node);
          for (auto childStmtNode : ifNode->ifStmtList) {
            queue.push(childStmtNode);
          }
          for (auto childStmtNode : ifNode->elseStmtList) {
            queue.push(childStmtNode);
          }
          break;
        }
        case AST::WHILE_NODE: {
          output.push_back(node);
          shared_ptr<WhileNode> whileNode =
              dynamic_pointer_cast<WhileNode>(node);
          for (auto childStmtNode : whileNode->stmtList) {
            queue.push(childStmtNode);
          }
          break;
        }
        default: {
          output.push_back(node);
          break;
        }
      }
    }
  }
  return output;
}

shared_ptr<unordered_set<string>> EntitiesExtractor::extractAllConstants() {
  shared_ptr<unordered_set<string>> output =
      make_shared<unordered_set<string>>();
  for (const auto& stmtNode : stmtNodeList) {
    auto variableSet = extractConstants(stmtNode);
    output->insert(variableSet->begin(), variableSet->end());
  }
  return output;
}

shared_ptr<unordered_set<string>> EntitiesExtractor::extractAllProcedures() {
  shared_ptr<unordered_set<string>> set = make_shared<unordered_set<string>>();
  for (const auto& procedureNode : programNode->procedureList) {
    set->insert(procedureNode->procedureName);
  }
  return set;
}
unordered_set<string> EntitiesExtractor::getVariablesFromExprNode(
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

unordered_set<string> EntitiesExtractor::getConstantsFromExprNode(
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

unordered_set<string> EntitiesExtractor::getVariablesFromCondExprNode(
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

unordered_set<string> EntitiesExtractor::getConstantsFromCondExprNode(
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
}  // namespace DE
