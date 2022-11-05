//
// Created by Nafour on 27/10/22.
//

#include "FollowsTRelationExtractor.h"

#include <queue>
#include <utility>

#include "AST/utils/ASTUtils.h"
#include "design_extractor/results/RelationResult.h"
namespace DE {
FollowsTRelationExtractor::FollowsTRelationExtractor(
    shared_ptr<DataModifier> dataModifier, shared_ptr<ProgramNode> programNode)
    : AbstractDesignExtractor(std::move(dataModifier), std::move(programNode)) {
  initListOfStmtList();
  output = make_shared<list<vector<string>>>();
}

void FollowsTRelationExtractor::initListOfStmtList() {
  vector<shared_ptr<ProcedureNode>> procedureList = programNode->procedureList;
  for (const auto& procedureNode : procedureList) {
    queue<vector<shared_ptr<StmtNode>>> queue;
    queue.push(procedureNode->stmtList);
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

void FollowsTRelationExtractor::extractHelper(
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
    deque<int> entryDeque;
    for (int& i : stmtNoList) {
      entryDeque.push_back(i);
    }

    while (!entryDeque.empty()) {
      int front = entryDeque.front();
      entryDeque.pop_front();
      for (auto& j : entryDeque) {
        vector<string> followsEntry;
        followsEntry.push_back(to_string(front));
        followsEntry.push_back(to_string(j));
        output->push_back(followsEntry);
      }
    }
  }
}

shared_ptr<ExtractorResult> FollowsTRelationExtractor::extract() {
  for (const auto& stmtList : listOfStmtList) {
    extractHelper(stmtList);
  }
  return make_shared<RelationResult>(output);
}

void FollowsTRelationExtractor::save(shared_ptr<ExtractorResult> result) {
  shared_ptr<RelationResult> relationResult =
      static_pointer_cast<RelationResult>(result);
  for (const auto& entry : *relationResult->getResult()) {
    dataModifier->saveFollowsT(entry);
  }
}
}  // namespace DE