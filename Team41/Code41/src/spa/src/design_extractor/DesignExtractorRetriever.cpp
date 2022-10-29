//
// Created by Xingchen Lin on 28/10/22.
//

#include "DesignExtractorRetriever.h"

namespace DE {
DesignExtractorRetriever::DesignExtractorRetriever(
				shared_ptr<DataModifier> dataModifier, shared_ptr<ProgramNode> programNode)
    : programNode(programNode) {

}

void DesignExtractorRetriever::initialize() {
//	nextTRelationExtractor = make_shared<NextTRelationExtractor>(dataModifier, programNode);
//	affectsRelationExtractor = make_shared<AffectsRelationExtractor>(dataModifier, programNode);
//	affectsTRelationExtractor = make_shared<AffectsTRelationExtractor>(dataModifier, programNode);
}

shared_ptr<vector<string>> DesignExtractorRetriever::getNextStarRelations(
    StmtNoArgs args) {
  return shared_ptr<vector<string>>();
}

shared_ptr<vector<string>> DesignExtractorRetriever::getAffectsRelations(
    StmtNoArgs args) {
  return shared_ptr<vector<string>>();
}

shared_ptr<vector<string>> DesignExtractorRetriever::getAffectsStarRelations(
    StmtNoArgs args) {
  return shared_ptr<vector<string>>();
}

shared_ptr<list<vector<string>>>
DesignExtractorRetriever::getAllNextStarRelations() {
  return shared_ptr<list<vector<string>>>();
}

shared_ptr<list<vector<string>>>
DesignExtractorRetriever::getAllAffectsRelations() {
  return shared_ptr<list<vector<string>>>();
}

shared_ptr<list<vector<string>>>
DesignExtractorRetriever::getAllAffectsStarRelations() {
  return shared_ptr<list<vector<string>>>();
}
}  // namespace DE