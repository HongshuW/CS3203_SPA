//
// Created by Nafour on 29/10/22.
//

#ifndef SPA_AFFECTSCOMMONEXTRACTOR_H
#define SPA_AFFECTSCOMMONEXTRACTOR_H
#include <memory>
#include <set>
#include <vector>

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
typedef set<vector<string>> AffectsRelationTable;

class AffectsCommonExtractor : public QueryTimeDesignExtractor,
                               public CFGExtractionInterface,
                               public CallNodeExtractionInterface,
                               public VariableExtractionInterface {
 protected:
  AffectsCommonExtractor(shared_ptr<ProgramNode> programNode);
  int offset;
  int programSize;
  shared_ptr<CFG> currCFG;
  shared_ptr<StmtNoToProcMap> stmtNoToProcMap;
  shared_ptr<LineNoToNodePtrMap> lineNoToNodePtrMap;
  shared_ptr<ProcCFGMap> procCFGMap;
  shared_ptr<ModifiesPMap> modifiesPMap;
  shared_ptr<AffectsRelationTable> affectsTable;
  void initialize();
  void DFS(int curr, shared_ptr<vector<int>> visitCount,
           unordered_map<string, int> LastModifiedMap);
  void generateAffectsTable();
};

}  // namespace DE

#endif  // SPA_AFFECTSCOMMONEXTRACTOR_H
