//
// Created by Nafour on 27/10/22.
//

#ifndef SPA_USESRELATIONEXTRACTOR_H
#define SPA_USESRELATIONEXTRACTOR_H
#include "design_extractor/AbstractDesignExtractor.h"
#include "design_extractor/extractors/interfaces/CallNodeExtractionInterface.h"
#include "design_extractor/extractors/interfaces/VariableExtractionInterface.h"

namespace DE {

class UsesRelationExtractor : public AbstractDesignExtractor,
                              public CallNodeExtractionInterface,
                              public VariableExtractionInterface {
 protected:
  UsesRelationExtractor(shared_ptr<DataModifier> dataModifier,
                        shared_ptr<ProgramNode> programNode);

  shared_ptr<list<vector<string>>> output;

  unordered_set<string> extractUsesSHelper(
      shared_ptr<ASTNode> node, shared_ptr<list<vector<string>>> result);
  void initProceduresToUsedVarsMap();
  StrToSetMap proceduresToUsedVarsMap;
};

}  // namespace DE

#endif  // SPA_USESRELATIONEXTRACTOR_H
