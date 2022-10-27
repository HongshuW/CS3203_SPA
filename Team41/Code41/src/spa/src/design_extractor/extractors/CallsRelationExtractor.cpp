//
// Created by Xingchen Lin on 26/10/22.
//

#include "CallsRelationExtractor.h"

#include <memory>
#include <utility>

namespace DE {
CallsRelationExtractor::CallsRelationExtractor(
    shared_ptr<DataModifier> dataModifier, shared_ptr<ProgramNode> programNode)
    : AbstractDesignExtractor(std::move(dataModifier), std::move(programNode)) {

}

shared_ptr<ExtractorResult> CallsRelationExtractor::extract() {
  shared_ptr<list<vector<string>>> output = make_shared<list<vector<string>>>();

  auto mappedCallNodesToProcedures =
      extractCallNodesFromProcedures(programNode);
  shared_ptr<unordered_map<shared_ptr<StmtNode>, int>> stmtNumbers =
      ASTUtils::getNodePtrToLineNumMap(programNode);
  for (auto& it : mappedCallNodesToProcedures) {
    string name = it.first;
    vector<shared_ptr<CallNode>> listOfCallNodes = it.second;
    for (const auto& callNode : listOfCallNodes) {
      vector<string> callsEntry;
      string callNodeName = callNode->procedureName;
      callsEntry.push_back(name);
      callsEntry.push_back(callNodeName);
      callsEntry.push_back(to_string(stmtNumbers->at(callNode)));
      output->push_back(callsEntry);
    }
  }
  return make_shared<RelationResult>(output);
}

void CallsRelationExtractor::save(shared_ptr<ExtractorResult> result) {
  shared_ptr<RelationResult> callsResult =
      static_pointer_cast<RelationResult>(result);
  for (auto entry : *callsResult->getResult()) {
    dataModifier->saveCalls(entry);
  }
}
}  // namespace DE