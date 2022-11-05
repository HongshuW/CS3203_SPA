//
// Created by Nafour on 27/10/22.
//

#include "IfPatternExtractor.h"

#include <queue>
#include <utility>

#include "AST/utils/ASTUtils.h"
#include "design_extractor/results/RelationResult.h"
namespace DE {
IfPatternExtractor::IfPatternExtractor(shared_ptr<DataModifier> dataModifier,
                                       shared_ptr<ProgramNode> programNode)
    : IfWhilePatternExtractor(std::move(dataModifier), std::move(programNode)) {}

shared_ptr<ExtractorResult> IfPatternExtractor::extract() {
  for (const auto& procedureNode : programNode->procedureList) {
    queue<vector<shared_ptr<StmtNode>>> queue;
    queue.push(procedureNode->stmtList);
    while (!queue.empty()) {
      auto stmtList = queue.front();
      queue.pop();
      for (const auto& stmtNode : stmtList) {
        NodeType nodeType = ASTUtils::getNodeType(stmtNode);
        switch (nodeType) {
          case AST::IF_NODE: {
            shared_ptr<IfNode> ifNode = dynamic_pointer_cast<IfNode>(stmtNode);
            int stmtNo = stmtNumbers->at(ifNode);
            unordered_set<string> varList =
                condExprNodeHandler(ifNode->condExpr);
            for (const string& var : varList) {
              vector<string> ifEntry;
              ifEntry.push_back(to_string(stmtNo));
              ifEntry.push_back(var);
              output->push_back(ifEntry);
            }

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
  }
  return make_shared<RelationResult>(output);
}

void IfPatternExtractor::save(shared_ptr<ExtractorResult> result) {
  shared_ptr<RelationResult> relationResult =
      static_pointer_cast<RelationResult>(result);
  for (auto &entry : *relationResult->getResult()) {
    dataModifier->saveIfPattern(entry);
  }
}
}  // namespace DE