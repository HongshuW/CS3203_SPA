//
// Created by Xingchen Lin on 28/10/22.
//

#ifndef SPA_DESIGNEXTRACTORMANAGER_H
#define SPA_DESIGNEXTRACTORMANAGER_H

#include <vector>

#include "AST/ProgramNode.h"
#include "AST/utils/ASTUtils.h"
#include "design_extractor/AbstractDesignExtractor.h"
#include "design_extractor/extractors/AssignPatternExtractor.h"
#include "design_extractor/extractors/CallsRelationExtractor.h"
#include "design_extractor/extractors/CallsTRelationExtractor.h"
#include "design_extractor/extractors/ConstantExtractor.h"
#include "design_extractor/extractors/EntitiesExtractor.h"
#include "design_extractor/extractors/FollowsRelationExtractor.h"
#include "design_extractor/extractors/FollowsTRelationExtractor.h"
#include "design_extractor/extractors/IfPatternExtractor.h"
#include "design_extractor/extractors/IfWhilePatternExtractor.h"
#include "design_extractor/extractors/ModifiesPRelationExtractor.h"
#include "design_extractor/extractors/ModifiesSRelationExtractor.h"
#include "design_extractor/extractors/NextRelationExtractor.h"
#include "design_extractor/extractors/ParentRelationExtractor.h"
#include "design_extractor/extractors/ParentTRelationExtractor.h"
#include "design_extractor/extractors/ProcedureExtractor.h"
#include "design_extractor/extractors/UsesPRelationExtractor.h"
#include "design_extractor/extractors/UsesSRelationExtractor.h"
#include "design_extractor/extractors/VariableExtractor.h"
#include "design_extractor/extractors/WhilePatternExtractor.h"
#include "pkb/DataModifier.h"

using namespace std;

namespace DE {
/**
 * Takes a program node and a data modifier and extract all the relations out
 * and save them to PKB.
 *
 * @param DataModifier dataModifier, ProgramNode programNode,
 */
class DesignExtractorManager {
 private:
  shared_ptr<ProgramNode> programNode;
  shared_ptr<DataModifier> dataModifier;
  void saveToPKB();
  void saveEntitiesToPKB();
  void savePatternsToPKB();
  void saveRelationsToPKB();

 public:
  explicit DesignExtractorManager(shared_ptr<DataModifier> dataModifier,
                                  shared_ptr<ProgramNode> programNode);
  void run();
};

}  // namespace DE

#endif  // SPA_DESIGNEXTRACTORMANAGER_H
