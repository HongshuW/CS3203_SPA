//
// Created by Nafour on 27/10/22.
//

#include "ModifiesPRelationExtractor.h"

#include <utility>

#include "design_extractor/results/RelationResult.h"
namespace DE {
ModifiesPRelationExtractor::ModifiesPRelationExtractor(
    shared_ptr<DataModifier> dataModifier, shared_ptr<ProgramNode> programNode)
    : ModifiesRelationExtractor(std::move(dataModifier), std::move(programNode)) {}

shared_ptr<ExtractorResult> ModifiesPRelationExtractor::extract() {
  shared_ptr<list<vector<string>>> ans = make_shared<list<vector<string>>>();
  auto mappedCallNodesToProcedures =
      extractCallNodesFromProcedures(programNode);
  for (const auto& pair : *proceduresToModifiedVarsMap) {
    unordered_set<string> uniqueVarList;
    string procedureName = pair.first;
    auto currentUsedVarList = pair.second;
    for (const auto& v : *currentUsedVarList) {
      uniqueVarList.insert(v);
    }

    // if the procedures has call nodes, handle them
    if (mappedCallNodesToProcedures.count(procedureName) != 0) {
      auto callNodes = mappedCallNodesToProcedures.at(procedureName);

      for (const auto& node : callNodes) {
        extractVariablesFromCallNodesInProceduresToList(
            node, proceduresToModifiedVarsMap, mappedCallNodesToProcedures,
            uniqueVarList);
      }
    }

    for (const auto& v : uniqueVarList) {
      vector<string> modifiesPEntry;
      modifiesPEntry.push_back(procedureName);
      modifiesPEntry.push_back(v);
      ans->push_back(modifiesPEntry);
    }
  }
  return make_shared<RelationResult>(ans);
}

void ModifiesPRelationExtractor::save(shared_ptr<ExtractorResult> result) {
  shared_ptr<RelationResult> modifiesPResult =
      static_pointer_cast<RelationResult>(result);
  for (const auto& entry : *modifiesPResult->getResult()) {
    dataModifier->saveModifiesP(entry);
  }
}
}  // namespace DE