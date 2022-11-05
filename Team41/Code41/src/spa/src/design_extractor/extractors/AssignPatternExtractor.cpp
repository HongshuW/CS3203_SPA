//
// Created by Nafour on 27/10/22.
//

#include "AssignPatternExtractor.h"

#include <utility>

#include "AST/utils/ASTUtils.h"

namespace DE {
AssignPatternExtractor::AssignPatternExtractor(
    shared_ptr<DataModifier> dataModifier, shared_ptr<ProgramNode> programNode)
    : AbstractDesignExtractor(std::move(dataModifier), std::move(programNode)) {
  output = make_shared<
      vector<pair<pair<int, string>, std::shared_ptr<AssignNode>>>>();
}

shared_ptr<ExtractorResult> AssignPatternExtractor::extract() {
  for (const auto& procedureNode : programNode->procedureList) {
    extractorHelper(procedureNode);
  }
  return make_shared<AssignPatternResult>(output);
}

void AssignPatternExtractor::save(shared_ptr<ExtractorResult> result) {
  shared_ptr<AssignPatternResult> resultList =
      static_pointer_cast<AssignPatternResult>(result);
  for (auto& resultRow : *resultList->getResult()) {
    string lineNumStr = to_string(resultRow.first.first);
    string varName = resultRow.first.second;
    auto exprNode = resultRow.second->expressionNode;
    dataModifier->saveAssignPattern({lineNumStr, varName}, exprNode);
  }
}

void AssignPatternExtractor::extractorHelper(const shared_ptr<ASTNode>& node) {
  NodeType nodeType = ASTUtils::getNodeType(node);

  if (nodeType == AST::PROCEDURE_NODE) {
    shared_ptr<ProcedureNode> procedureNode1 =
        dynamic_pointer_cast<ProcedureNode>(node);
    for (const auto& stmtNode : procedureNode1->stmtList) {
      extractorHelper(stmtNode);
    }
    return;
  }
  switch (nodeType) {
    case AST::ASSIGN_NODE: {
      shared_ptr<AssignNode> assignNode =
          dynamic_pointer_cast<AssignNode>(node);
      int lineNum = stmtNumbers->at(assignNode);
      output->insert(
          output->begin(),
          pair(pair(lineNum, assignNode->variableNode->variable), assignNode));
      return;
    }
    case AST::IF_NODE: {
      shared_ptr<IfNode> ifNode = dynamic_pointer_cast<IfNode>(node);
      for (const auto& stmtNode : ifNode->ifStmtList) {
        extractorHelper(stmtNode);
      }
      for (const auto& stmtNode : ifNode->elseStmtList) {
        extractorHelper(stmtNode);
      }
      return;
    }
    case AST::WHILE_NODE: {
      shared_ptr<WhileNode> whileNode = dynamic_pointer_cast<WhileNode>(node);
      for (const auto& stmtNode : whileNode->stmtList) {
        extractorHelper(stmtNode);
      }
      return;
    }
    default:
      return;
  }
}
}  // namespace DE