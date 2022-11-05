//
// Created by Nafour on 27/10/22.
//

#include "UsesPRelationExtractor.h"

#include <utility>

#include "design_extractor/results/RelationResult.h"
namespace DE {
UsesPRelationExtractor::UsesPRelationExtractor(
    shared_ptr<DataModifier> dataModifier, shared_ptr<ProgramNode> programNode)
    : UsesRelationExtractor(std::move(dataModifier), std::move(programNode)) {}

shared_ptr<ExtractorResult> UsesPRelationExtractor::extract() {
  auto mappedCallNodesToProcedures =
      extractCallNodesFromProcedures(programNode);
  for (const auto& pair : *proceduresToUsedVarsMap) {
    unordered_set<string> uniqueVarList = unordered_set<string>();
    string procedureName = pair.first;
    auto currentUsedVarList = pair.second;
    uniqueVarList.insert(currentUsedVarList->begin(),
                         currentUsedVarList->end());

    // if the procedures has call nodes, handle them
    if (mappedCallNodesToProcedures.count(procedureName) != 0) {
      vector<shared_ptr<CallNode>> callNodes = vector<shared_ptr<CallNode>>();
      if (mappedCallNodesToProcedures.count(procedureName)) {
        callNodes = mappedCallNodesToProcedures.at(procedureName);
      }
      unordered_set<string> visitedProcs;
      for (const auto& node : callNodes) {
        if (visitedProcs.count(node->procedureName)) continue;
        visitedProcs.insert(node->procedureName);
        extractVariablesFromCallNodesInProceduresToList(
            node, proceduresToUsedVarsMap, mappedCallNodesToProcedures,
            uniqueVarList);
      }
    }

    for (const auto& v : uniqueVarList) {
      vector<string> usePEntry;
      usePEntry.push_back(procedureName);
      usePEntry.push_back(v);
      output->push_back(usePEntry);
    }
  }
  return make_shared<RelationResult>(output);
}

void UsesPRelationExtractor::save(shared_ptr<ExtractorResult> result) {
  shared_ptr<RelationResult> usesPResult =
      static_pointer_cast<RelationResult>(result);
  for (const auto& entry : *usesPResult->getResult()) {
    dataModifier->saveUsesP(entry);
  }
}
}  // namespace DE