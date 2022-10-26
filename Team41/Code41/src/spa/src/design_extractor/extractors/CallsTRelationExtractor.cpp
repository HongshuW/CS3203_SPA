//
// Created by Xingchen Lin on 26/10/22.
//

#include "CallsTRelationExtractor.h"

#include <utility>

namespace DE {
CallsTRelationExtractor::CallsTRelationExtractor(
    shared_ptr<DataModifier> dataModifier, shared_ptr<ProgramNode> programNode)
    : AbstractDesignExtractor(std::move(dataModifier), std::move(programNode)) {

}

ExtractorResult CallsTRelationExtractor::extract() {
  list<vector<string>> output;
  DesignExtractorUtils designExtractorUtils = DesignExtractorUtils();
  auto mappedCallNodesToProcedures =
      designExtractorUtils.extractCallNodesFromProcedures(programNode);
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
        reachableCallNodeNames.insert(callNodeEntry->procedureName);
        if (mappedCallNodesToProcedures.count(callNodeEntry->procedureName) !=
            0) {
          auto otherCallNodes =
              mappedCallNodesToProcedures.at(callNodeEntry->procedureName);
          for (const auto& n : otherCallNodes) {
            queue.push(n);
          }
        }
      }
    }
    for (const auto& nodeNames : reachableCallNodeNames) {
      vector<string> callsEntry;
      callsEntry.push_back(name);
      callsEntry.push_back(nodeNames);
      output.push_back(callsEntry);
    }
  }
  return RelationResult(make_shared<list<vector<string>>>(output));
}

void CallsTRelationExtractor::save(ExtractorResult result) {
  RelationResult callsTResult = static_cast<RelationResult&>(result);
  for (const auto& entry : *callsTResult.getResult()) {
    dataModifier->saveCallsT(entry);
  }
}
}  // namespace DE