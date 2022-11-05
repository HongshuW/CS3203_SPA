//
// Created by Nafour on 27/10/22.
//

#include "UsesRelationExtractor.h"

#include <utility>

#include "AST/utils/ASTUtils.h"

namespace DE {
UsesRelationExtractor::UsesRelationExtractor(
    shared_ptr<DataModifier> dataModifier, shared_ptr<ProgramNode> programNode)
    : AbstractDesignExtractor(std::move(dataModifier), std::move(programNode)) {
  output = make_shared<list<vector<string>>>();
  proceduresToUsedVarsMap =
      make_shared<unordered_map<string, shared_ptr<unordered_set<string>>>>();
  initProceduresToUsedVarsMap();
}

void UsesRelationExtractor::initProceduresToUsedVarsMap() {
  for (const auto& procedureNode : programNode->procedureList) {
    shared_ptr<list<vector<string>>> result =
        make_shared<list<vector<string>>>();
    extractUsesSHelper(procedureNode, result);
    string procedureName = procedureNode->procedureName;
    shared_ptr<unordered_set<string>> variableList =
        make_shared<unordered_set<string>>();
    for (auto pair : *result) {
      variableList->insert(pair[1]);
    }
    proceduresToUsedVarsMap->insert(make_pair(procedureName, variableList));
  }
}

unordered_set<string> UsesRelationExtractor::extractUsesSHelper(
    const shared_ptr<ASTNode>& node, const shared_ptr<list<vector<string>>>& result) {
  NodeType nodeType = ASTUtils::getNodeType(node);

  if (nodeType == AST::PROCEDURE_NODE) {
    shared_ptr<ProcedureNode> procedureNode1 =
        dynamic_pointer_cast<ProcedureNode>(node);
    unordered_set<string> usedVarsProcedure = unordered_set<string>();
    for (const auto& stmtNode : procedureNode1->stmtList) {
      auto fromChild = extractUsesSHelper(stmtNode, result);
      usedVarsProcedure.insert(fromChild.begin(), fromChild.end());
    }
    return usedVarsProcedure;
  }

  switch (nodeType) {
    case AST::PRINT_NODE: {
      shared_ptr<PrintNode> printNode = dynamic_pointer_cast<PrintNode>(node);
      int stmtNum = stmtNumbers->at(printNode);
      string var = printNode->variableNode->variable;
      result->insert(result->begin(), {to_string(stmtNum), var});
      return unordered_set<string>({var});
    }
    case AST::IF_NODE: {
      shared_ptr<IfNode> ifNode = dynamic_pointer_cast<IfNode>(node);
      unordered_set<string> usedVarsIf = unordered_set<string>();

      for (const auto& child : ifNode->ifStmtList) {
        auto fromChild = extractUsesSHelper(child, result);
        usedVarsIf.insert(fromChild.begin(), fromChild.end());
      }

      for (const auto& child : ifNode->elseStmtList) {
        auto fromChild = extractUsesSHelper(child, result);
        usedVarsIf.insert(fromChild.begin(), fromChild.end());
      }

      auto variables = getVariablesFromCondExprNode(ifNode->condExpr);
      usedVarsIf.insert(variables.begin(), variables.end());

      for (const auto& usedVarIf : usedVarsIf) {
        int stmtNum = stmtNumbers->at(ifNode);
        result->insert(result->begin(), {to_string(stmtNum), usedVarIf});
      }
      return usedVarsIf;
    }
    case AST::WHILE_NODE: {
      shared_ptr<WhileNode> whileNode = dynamic_pointer_cast<WhileNode>(node);
      unordered_set<string> usedVarsWhile = unordered_set<string>();

      for (const auto& child : whileNode->stmtList) {
        auto fromChild = extractUsesSHelper(child, result);
        usedVarsWhile.insert(fromChild.begin(), fromChild.end());
      }

      auto variables = getVariablesFromCondExprNode(whileNode->condExpr);
      usedVarsWhile.insert(variables.begin(), variables.end());

      for (const auto& usedVarWhile : usedVarsWhile) {
        int stmtNum = stmtNumbers->at(whileNode);
        result->insert(result->begin(), {to_string(stmtNum), usedVarWhile});
      }
      return usedVarsWhile;
    }
    case AST::ASSIGN_NODE: {
      shared_ptr<AssignNode> assignNode =
          dynamic_pointer_cast<AssignNode>(node);
      auto variables = getVariablesFromExprNode(assignNode->expressionNode);
      for (const auto& var : variables) {
        int stmtNum = stmtNumbers->at(assignNode);
        result->insert(result->begin(), {to_string(stmtNum), var});
      }
      return variables;
    }
    default: {
      return {};
    }
  }
}

}  // namespace DE