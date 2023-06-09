//
// Created by Xingchen Lin on 26/10/22.
//

#include "CallsTRelationExtractor.h"

#include <queue>
#include <utility>

namespace DE {
CallsTRelationExtractor::CallsTRelationExtractor(
    shared_ptr<DataModifier> dataModifier, shared_ptr<ProgramNode> programNode)
    : AbstractDesignExtractor(std::move(dataModifier), std::move(programNode)) {

}

shared_ptr<ExtractorResult> CallsTRelationExtractor::extract() {
  shared_ptr<list<vector<string>>> output = make_shared<list<vector<string>>>();

  auto mappedCallNodesToProcedures =
      extractCallNodesFromProcedures(programNode);
  shared_ptr<unordered_map<shared_ptr<StmtNode>, int>> stmtNumbers =
      ASTUtils::getNodePtrToLineNumMap(programNode);
  for (auto& it : mappedCallNodesToProcedures) {
    string name = it.first;
    vector<shared_ptr<CallNode>> listOfCallNodes = it.second;
    unordered_set<string> reachableCallNodeNames;
    for (const auto& callNode : listOfCallNodes) {
      queue<shared_ptr<CallNode>> queue;
      queue.push(callNode);
      while (!queue.empty()) {
        auto callNodeEntry = queue.front();
        queue.pop();
        if (reachableCallNodeNames.count(callNodeEntry->procedureName))
          continue;
        reachableCallNodeNames.insert(callNodeEntry->procedureName);
        if (mappedCallNodesToProcedures.count(callNodeEntry->procedureName) !=
            0) {
          auto otherCallNodes =
              mappedCallNodesToProcedures.at(callNodeEntry->procedureName);
          for (const auto& n : otherCallNodes) {
            if (reachableCallNodeNames.count(n->procedureName)) continue;
            queue.push(n);
          }
        }
      }
    }
    for (const auto& nodeNames : reachableCallNodeNames) {
      vector<string> callsEntry;
      callsEntry.push_back(name);
      callsEntry.push_back(nodeNames);
      output->push_back(callsEntry);
    }
  }
  return make_shared<RelationResult>(output);
}

void CallsTRelationExtractor::save(shared_ptr<ExtractorResult> result) {
  shared_ptr<RelationResult> callsTResult =
      static_pointer_cast<RelationResult>(result);
  for (const auto& entry : *callsTResult->getResult()) {
    dataModifier->saveCallsT(entry);
  }
}
}  // namespace DE