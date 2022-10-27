//
// Created by Nafour on 27/10/22.
//

#ifndef SPA_MODIFIESSRELATIONEXTRACTOR_H
#define SPA_MODIFIESSRELATIONEXTRACTOR_H
#include <set>

#include "AST/CallNode.h"
#include "design_extractor/extractors/ModifiesRelationExtractor.h"
#include "design_extractor/utils/DesignExtractorUtils.h"

namespace DE {

class ModifiesSRelationExtractor : public ModifiesRelationExtractor {
  void extractorHelper(shared_ptr<ASTNode> node);
  void insertCallsForModifiesS();
  void insertCallsInIfAndWhileForModifiesS();

  void initIfAndWhileStmtNoToModifiedVarsMap();

  shared_ptr<vector<string>> ancestors;
  shared_ptr<list<vector<string>>> output;
  StrToSetMap ifWhileStmtNoToModifiedVarsMap;

 public:
  ModifiesSRelationExtractor(shared_ptr<DataModifier> dataModifier,
                             shared_ptr<ProgramNode> programNode);
  shared_ptr<ExtractorResult> extract() override;
  void save(shared_ptr<ExtractorResult> result) override;
};

}  // namespace DE

#endif  // SPA_MODIFIESSRELATIONEXTRACTOR_H
