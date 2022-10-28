//
// Created by Xingchen Lin on 28/10/22.
//

#ifndef SPA_AFFECTSRELATIONEXTRACTOR_H
#define SPA_AFFECTSRELATIONEXTRACTOR_H

#include <set>
#include <utility>

#include "../args/StmtNoArgs.h"
#include "AST/utils/ASTUtils.h"
#include "CFG/CFG.h"
#include "design_extractor/QueryTimeDesignExtractor.h"
#include "design_extractor/args/StmtNoArgs.h"
#include "design_extractor/extractors/interfaces/CFGExtractionInterface.h"
#include "design_extractor/extractors/interfaces/CallNodeExtractionInterface.h"
#include "design_extractor/extractors/interfaces/VariableExtractionInterface.h"
#include "design_extractor/results/RelationResult.h"

namespace DE {

typedef unordered_map<int, shared_ptr<ProcedureNode>> StmtNoToProcMap;
typedef unordered_map<int, shared_ptr<StmtNode>> LineNoToNodePtrMap;
typedef unordered_map<string, shared_ptr<unordered_set<string>>> ModifiesPMap;
typedef unordered_map<string, int> LastModifiedMap;
typedef set<vector<string>> AffectsTable;

class AffectsRelationExtractor : public QueryTimeDesignExtractor,
                                 public CFGExtractionInterface,
                                 public CallNodeExtractionInterface,
                                 public VariableExtractionInterface {
 private:
  shared_ptr<StmtNoToProcMap> stmtNoToProcMap;
  shared_ptr<LineNoToNodePtrMap> lineNoToNodePtrMap;
  shared_ptr<ProcCFGMap> procCFGMap;
  shared_ptr<ModifiesPMap> modifiesPMap;
  // shared_ptr<LastModifiedMap> lastModifiedMap;
  shared_ptr<AffectsTable> affectsTable;

  void initialize();
  void DFS(int curr, CFG cfg, shared_ptr<vector<int>> visitCount, int offset,
           unordered_map<string, int> LastModifiedMap);

 public:
  AffectsRelationExtractor(shared_ptr<DataModifier> dataModifier,
                           shared_ptr<ProgramNode> programNode);
  shared_ptr<ExtractorResult> extract(StmtNoArgs args) override;
  shared_ptr<ExtractorResult> extractAllRelations() override;
};

}  // namespace DE

#endif  // SPA_AFFECTSRELATIONEXTRACTOR_H
