//
// Created by Nafour on 27/10/22.
//

#include "FollowsRelationExtractor.h"

#include <queue>
#include <utility>

#include "AST/ASTNode.h"
#include "AST/utils/ASTUtils.h"

namespace DE {
FollowsRelationExtractor::FollowsRelationExtractor(
    shared_ptr<DataModifier> dataModifier, shared_ptr<ProgramNode> programNode)
    : AbstractDesignExtractor(std::move(dataModifier), std::move(programNode)) {
  initListOfStmtList();
  output = make_shared<list<vector<string>>>();
}

void FollowsRelationExtractor::initListOfStmtList() {
  vector<shared_ptr<ProcedureNode>> procedureList = programNode->procedureList;
  for (const auto& procedureNode : procedureList) {
    queue<vector<shared_ptr<StmtNode>>> queue;
    queue.push(procedureNode->stmtList);
    // to do: refactor deep nesting
    while (!queue.empty()) {
      auto stmtList = queue.front();
      queue.pop();
      listOfStmtList.push_back(stmtList);
      for (const auto& stmtNode : stmtList) {
        NodeType nodeType = ASTUtils::getNodeType(stmtNode);
        switch (nodeType) {
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
  }
}

shared_ptr<ExtractorResult> FollowsRelationExtractor::extract() {
  for (const auto& stmtList : listOfStmtList) {
    extractHelper(stmtList);
  }
  return make_shared<RelationResult>(output);
}

void FollowsRelationExtractor::extractHelper(
    const vector<shared_ptr<StmtNode>>& stmtList) {
  vector<int> stmtNoList;
  if (stmtList.size() <= 1) {
    return;
  } else {
    for (const auto& stmtNode : stmtList) {
      int stmtNo = stmtNumbers->at(stmtNode);
      stmtNoList.push_back(stmtNo);
    }

    std::sort(stmtNoList.begin(), stmtNoList.end());
    for (int i = 0; i < stmtNoList.size() - 1; i++) {
      vector<string> followsEntry;
      string s1 = to_string(stmtNoList[i]);
      string s2 = to_string(stmtNoList[i + 1]);
      followsEntry.push_back(s1);
      followsEntry.push_back(s2);
      output->push_back(followsEntry);
    }
  }
}

void FollowsRelationExtractor::save(shared_ptr<ExtractorResult> result) {
  shared_ptr<RelationResult> relationResult =
      static_pointer_cast<RelationResult>(result);
  for (const auto& entry : *relationResult->getResult()) {
    dataModifier->saveFollows(entry);
  }
}
}  // namespace DE