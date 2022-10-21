//
// Created by Aaron on 14/10/22.
//

#include "ExtractedProceduresSaver.h"

#include "design_extractor/EntityExtractor.h"

using namespace DE;

void ExtractedProceduresSaver::save(shared_ptr<ProgramNode> programNode,
                                    shared_ptr<DataModifier> dataModifier) {
  shared_ptr<unordered_set<string>> set =
      EntityExtractor::extractAllProcedures(programNode);

  list<string> entityResultList;
  auto it = entityResultList.begin();
  for (const auto& elem : *set) {
    it = entityResultList.insert(it, elem);
  }

  dataModifier->saveProcedures(entityResultList);
}