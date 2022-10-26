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
}  // namespace DE
