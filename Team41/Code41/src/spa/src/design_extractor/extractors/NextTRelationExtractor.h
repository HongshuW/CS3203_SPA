//
// Created by Xingchen Lin on 27/10/22.
//

#ifndef SPA_NEXTTRELATIONEXTRACTOR_H
#define SPA_NEXTTRELATIONEXTRACTOR_H

#include "../args/StmtNoArgs.h"
#include "CFG/CFG.h"
#include "design_extractor/QueryTimeDesignExtractor.h"
#include "design_extractor/args/StmtNoArgs.h"
#include "design_extractor/extractors/interfaces/CFGExtractionInterface.h"
#include "design_extractor/results/QueryTimeResult.h"
#include "design_extractor/results/RelationResult.h"

typedef unordered_map<int, shared_ptr<ProcedureNode>> StmtNoToProcMap;

namespace DE {

class NextTRelationExtractor : public QueryTimeDesignExtractor,
                               public CFGExtractionInterface {
 private:
  shared_ptr<StmtNoToProcMap> stmtNoToProcMap;
  shared_ptr<ProcCFGMap> procCFGMap;

  shared_ptr<vector<string>> extractNoWildcard(StmtNoArgs args);
  shared_ptr<vector<string>> extractOneWildcard(StmtNoArgs args,
                                                bool isStartGiven);
  bool checkNoWildcardBFS(int start, int end, const CFG& cfg);
  shared_ptr<vector<string>> extractOneWildcardDFS(const CFG& cfg,
                                                   int startingStmt,
                                                   bool isStartGiven);
  bool areBothArgsValid(int start, int end);
  void initialise();

 public:
  explicit NextTRelationExtractor(shared_ptr<ProgramNode> programNode);
  shared_ptr<ExtractorResult> extract(StmtNoArgs args) override;
  shared_ptr<ExtractorResult> extractAllRelations() override;
};

}  // namespace DE

#endif  // SPA_NEXTTRELATIONEXTRACTOR_H
