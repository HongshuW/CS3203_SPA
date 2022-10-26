//
// Created by Nafour on 26/10/22.
//

#ifndef SPA_ENTITIESEXTRACTOR_H
#define SPA_ENTITIESEXTRACTOR_H
#include "AST/CallNode.h"
#include "AST/CondExprNode.h"
#include "design_extractor/AbstractDesignExtractor.h"
#include "design_extractor/results/ExtractorResult.h"

namespace DE {

class EntitiesExtractor : public AbstractDesignExtractor {
 protected:
  vector<shared_ptr<StmtNode>> extractStmtNodes();

  vector<shared_ptr<StmtNode>> stmtNodeList;

 public:
  EntitiesExtractor(shared_ptr<DataModifier> dataModifier,
                    shared_ptr<ProgramNode> programNode);
};

}  // namespace DE

#endif  // SPA_ENTITIESEXTRACTOR_H
