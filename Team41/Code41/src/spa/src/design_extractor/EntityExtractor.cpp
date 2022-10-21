//
// Created by Nafour on 10/9/22.
//

#include "EntityExtractor.h"

#include <queue>

#include "utils/Utils.h"

namespace DE {
shared_ptr<unordered_set<string>> EntityExtractor::extractVariables(
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

shared_ptr<unordered_set<string>> EntityExtractor::extractConstants(
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

vector<shared_ptr<StmtNode>> EntityExtractor::extractStmtNodes(
    shared_ptr<ProgramNode> rootPtr) {
  vector<shared_ptr<StmtNode>> output;
  auto procedureList = rootPtr->procedureList;
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

shared_ptr<unordered_set<string>> EntityExtractor::extractAllVariables(
    shared_ptr<ProgramNode> rootPtr) {
  vector<shared_ptr<StmtNode>> stmtNodeList =
      EntityExtractor::extractStmtNodes(rootPtr);
  shared_ptr<unordered_set<string>> output =
      make_shared<unordered_set<string>>();
  for (const auto& stmtNode : stmtNodeList) {
    auto variableSet = EntityExtractor::extractVariables(stmtNode);
    output->insert(variableSet->begin(), variableSet->end());
  }
  return output;
}

shared_ptr<unordered_set<string>> EntityExtractor::extractAllConstants(
    shared_ptr<ProgramNode> rootPtr) {
  vector<shared_ptr<StmtNode>> stmtNodeList =
      EntityExtractor::extractStmtNodes(rootPtr);
  shared_ptr<unordered_set<string>> output =
      make_shared<unordered_set<string>>();
  for (const auto& stmtNode : stmtNodeList) {
    auto variableSet = EntityExtractor::extractConstants(stmtNode);
    output->insert(variableSet->begin(), variableSet->end());
  }
  return output;
}

shared_ptr<unordered_set<string>> EntityExtractor::extractAllProcedures(
    shared_ptr<ProgramNode> rootPtr) {
  shared_ptr<unordered_set<string>> set = make_shared<unordered_set<string>>();
  auto procedureList = rootPtr->procedureList;
  for (const auto& procedureNode : procedureList) {
    set->insert(procedureNode->procedureName);
  }
  return set;
}

bool EntityExtractor::is_number(const std::string& s) {
  std::string::const_iterator it = s.begin();
  while (it != s.end() && std::isdigit(*it)) ++it;
  return !s.empty() && it == s.end();
}

unordered_set<string> EntityExtractor::getVariablesFromExprNode(
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

unordered_set<string> EntityExtractor::getConstantsFromExprNode(
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

unordered_map<string, vector<shared_ptr<CallNode>>>
EntityExtractor::extractCallNodesFromProcedures(
    shared_ptr<AST::ProgramNode> rootPtr) {
  auto mapCallNodesToProcedures =
      unordered_map<string, vector<shared_ptr<CallNode>>>();
  vector<shared_ptr<ProcedureNode>> procedureList = rootPtr->procedureList;
  for (auto procedureNode : procedureList) {
    string name = procedureNode->procedureName;
    auto listOfCallNodes = vector<shared_ptr<CallNode>>();
    queue<vector<shared_ptr<StmtNode>>> queue;
    queue.push(procedureNode->stmtList);
    while (!queue.empty()) {
      auto stmtList = queue.front();
      queue.pop();
      for (auto stmtNode : stmtList) {
        NodeType nodeType = ASTUtils::getNodeType(stmtNode);
        switch (nodeType) {
          case AST::CALL_NODE: {
            shared_ptr<CallNode> callNode =
                dynamic_pointer_cast<CallNode>(stmtNode);
            listOfCallNodes.push_back(callNode);
            break;
          }
          case AST::IF_NODE: {
            shared_ptr<IfNode> ifNode = dynamic_pointer_cast<IfNode>(stmtNode);
            vector<shared_ptr<StmtNode>> ifStmtList = ifNode->ifStmtList;
            vector<shared_ptr<StmtNode>> elseStmtList = ifNode->elseStmtList;
            queue.push(ifStmtList);
            queue.push(elseStmtList);
            break;
          }
          case AST::WHILE_NODE: {
            shared_ptr<WhileNode> whileNode =
                dynamic_pointer_cast<WhileNode>(stmtNode);
            vector<shared_ptr<StmtNode>> whileStmtList = whileNode->stmtList;
            queue.push(whileStmtList);
            break;
          }
          default:
            break;
        }
      }
    }
    if (!listOfCallNodes.empty()) {
      mapCallNodesToProcedures.insert(make_pair(name, listOfCallNodes));
    }
  }
  return mapCallNodesToProcedures;
}

void EntityExtractor::extractVariablesFromCallNodesInProceduresToList(
    shared_ptr<CallNode> callNode,
    unordered_map<string, unordered_set<string>> mappedProceduresToVar,
    unordered_map<string, vector<shared_ptr<CallNode>>>
        mappedCallNodesToProcedures,
    unordered_set<string>& uniqueVarList) {
  queue<shared_ptr<CallNode>> queue;
  queue.push(callNode);
  while (!queue.empty()) {
    auto callNodeEntry = queue.front();
    queue.pop();
    unordered_set<string> usedVarList = unordered_set<string>();
    if (mappedProceduresToVar.count(callNodeEntry->procedureName)) {
      usedVarList = mappedProceduresToVar.at(callNodeEntry->procedureName);
    }

    uniqueVarList.insert(usedVarList.begin(), usedVarList.end());

    if (mappedCallNodesToProcedures.count(callNodeEntry->procedureName) != 0) {
      auto otherCallNodes =
          mappedCallNodesToProcedures.at(callNodeEntry->procedureName);
      for (auto n : otherCallNodes) {
        queue.push(n);
      }
    }
  }
}

vector<shared_ptr<StmtNode>>
EntityExtractor::extractIfAndWhileNodesFromProcedures(
    shared_ptr<ProgramNode> rootPtr) {
  vector<shared_ptr<StmtNode>> extractedNodes;

  for (auto procedureNode : rootPtr->procedureList) {
    queue<vector<shared_ptr<StmtNode>>> queue;
    queue.push(procedureNode->stmtList);
    while (!queue.empty()) {
      auto stmtList = queue.front();
      queue.pop();
      for (auto stmtNode : stmtList) {
        NodeType nodeType = ASTUtils::getNodeType(stmtNode);
        switch (nodeType) {
          case AST::IF_NODE: {
            shared_ptr<IfNode> ifNode = dynamic_pointer_cast<IfNode>(stmtNode);
            extractedNodes.push_back(ifNode);
            queue.push(ifNode->ifStmtList);
            queue.push(ifNode->elseStmtList);
            break;
          }
          case AST::WHILE_NODE: {
            shared_ptr<WhileNode> whileNode =
                dynamic_pointer_cast<WhileNode>(stmtNode);
            extractedNodes.push_back(whileNode);
            queue.push(whileNode->stmtList);
            break;
          }
          default:
            break;
        }
      }
    }
  }

  return extractedNodes;
}

unordered_set<string> EntityExtractor::getVariablesFromCondExprNode(
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

unordered_set<string> EntityExtractor::getConstantsFromCondExprNode(
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