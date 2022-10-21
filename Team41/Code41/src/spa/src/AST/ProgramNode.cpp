//
// Created by Tan Wei En on 3/9/22.
//

#include "ProgramNode.h"

#include <utility>

ProgramNode::ProgramNode(
    std::vector<std::shared_ptr<ProcedureNode>> procedureList)
    : procedureList(std::move(procedureList)) {}

bool ProgramNode::operator==(const ASTNode& node) const {
  auto castedNode = dynamic_cast<const ProgramNode*>(&node);
  bool isEqual =
      castedNode != nullptr &&
      equal(begin(procedureList), end(procedureList),
            begin(castedNode->procedureList), end(castedNode->procedureList),
            [](const shared_ptr<ProcedureNode>& lhs,
               const shared_ptr<ProcedureNode>& rhs) { return *lhs == *rhs; });
  return isEqual;
}
