//
// Created by Nafour on 27/10/22.
//

#ifndef SPA_MODIFIESSRELATIONEXTRACTOR_H
#define SPA_MODIFIESSRELATIONEXTRACTOR_H
#include <set>

#include "AST/CallNode.h"
#include "design_extractor/AbstractDesignExtractor.h"

namespace DE {
typedef shared_ptr<unordered_map<string, shared_ptr<unordered_set<string>>>>
    StrToSetMap;
class ModifiesSRelationExtractor : public AbstractDesignExtractor {
  void extractorHelper(shared_ptr<ASTNode> node);
  void insertCallsForModifiesS();
  void insertCallsInIfAndWhileForModifiesS();
  shared_ptr<unordered_set<string>> getModifiedVariablesFromProcedure(
      shared_ptr<ProcedureNode> procedureNode);
  void initProceduresToModifiedVarsMap();
  void initIfAndWhileStmtNoToModifiedVarsMap();

  shared_ptr<vector<string>> ancestors;
  shared_ptr<list<vector<string>>> output;
  StrToSetMap proceduresToModifiedVarsMap;
  StrToSetMap ifWhileStmtNoToModifiedVarsMap;

 public:
  ModifiesSRelationExtractor(shared_ptr<DataModifier> dataModifier,
                             shared_ptr<ProgramNode> programNode);
  shared_ptr<ExtractorResult> extract() override;
  void save(shared_ptr<ExtractorResult> result) override;
};

}  // namespace DE

#endif  // SPA_MODIFIESSRELATIONEXTRACTOR_H
