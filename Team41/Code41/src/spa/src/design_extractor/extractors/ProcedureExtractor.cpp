//
// Created by Nafour on 27/10/22.
//

#include "ProcedureExtractor.h"

#include <utility>

#include "design_extractor/results/EntityResult.h"
namespace DE {
ProcedureExtractor::ProcedureExtractor(shared_ptr<DataModifier> dataModifier,
                                       shared_ptr<ProgramNode> programNode)
    : EntitiesExtractor(std::move(dataModifier), std::move(programNode)) {}

shared_ptr<ExtractorResult> ProcedureExtractor::extract() {
  shared_ptr<unordered_set<string>> set = make_shared<unordered_set<string>>();
  for (const auto& procedureNode : programNode->procedureList) {
    set->insert(procedureNode->procedureName);
  }
  shared_ptr<list<string>> entityResultList = make_shared<list<string>>();
  auto it = entityResultList->begin();
  for (const auto& elem : *set) {
    it = entityResultList->insert(it, elem);
  }
  return make_shared<EntityResult>(entityResultList);
}

void ProcedureExtractor::save(shared_ptr<ExtractorResult> result) {
  shared_ptr<EntityResult> entityResult =
      static_pointer_cast<EntityResult>(result);
  dataModifier->saveProcedures(*entityResult->getResult());
}
}  // namespace DE