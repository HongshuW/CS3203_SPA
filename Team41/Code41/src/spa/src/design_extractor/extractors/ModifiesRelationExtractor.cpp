//
// Created by Nafour on 27/10/22.
//

#include "ModifiesRelationExtractor.h"

namespace DE {
ModifiesRelationExtractor::ModifiesRelationExtractor(
    shared_ptr<DataModifier> dataModifier, shared_ptr<ProgramNode> programNode)
    : AbstractDesignExtractor(dataModifier, programNode) {
  proceduresToModifiedVarsMap =
      make_shared<unordered_map<string, shared_ptr<unordered_set<string>>>>();
  initProceduresToModifiedVarsMap();
}
void ModifiesRelationExtractor::initProceduresToModifiedVarsMap() {
  for (auto procedure : programNode->procedureList) {
    string procedureName = procedure->procedureName;
    shared_ptr<unordered_set<string>> modifiedVariables =
        DesignExtractorUtils::getModifiedVariablesFromProcedure(procedure);
    proceduresToModifiedVarsMap->insert({procedureName, modifiedVariables});
  }
}

}  // namespace DE