//
// Created by Nafour on 26/10/22.
//

#include "VariableExtractor.h"

#include <queue>

#include "AST/utils/ASTUtils.h"
#include "design_extractor/results/EntityResult.h"

namespace DE {
VariableExtractor::VariableExtractor(shared_ptr<DataModifier> dataModifier,
                                     shared_ptr<ProgramNode> programNode)
    : EntitiesExtractor(dataModifier, programNode) {}

shared_ptr<unordered_set<string>> VariableExtractor::extractVariables(
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

shared_ptr<ExtractorResult> VariableExtractor::extract() {
  shared_ptr<unordered_set<string>> output =
      make_shared<unordered_set<string>>();
  for (const auto& stmtNode : stmtNodeList) {
    auto variableSet = extractVariables(stmtNode);
    output->insert(variableSet->begin(), variableSet->end());
  }

  shared_ptr<list<string>> entityResultList = make_shared<list<string>>();
  auto it = entityResultList->begin();
  for (const auto& elem : *output) {
    it = entityResultList->insert(it, elem);
  }

  return make_shared<EntityResult>(entityResultList);
}

void VariableExtractor::save(shared_ptr<ExtractorResult> result) {
  shared_ptr<EntityResult> entityResult =
      static_pointer_cast<EntityResult>(result);
  dataModifier->saveVariables(*entityResult->getResult());
}
}  // namespace DE