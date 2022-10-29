//
// Created by Xingchen Lin on 28/10/22.
//

#include "DesignExtractorRetriever.h"

namespace DE {
DesignExtractorRetriever::DesignExtractorRetriever(shared_ptr<ProgramNode> programNode)
    : programNode(programNode) {
	initialize();
}

void DesignExtractorRetriever::initialize() {
	nextTRelationExtractor = make_shared<NextTRelationExtractor>(programNode);
	affectsRelationExtractor = make_shared<AffectsRelationExtractor>(programNode);
	affectsTRelationExtractor = make_shared<AffectsTRelationExtractor>(programNode);
}

shared_ptr<vector<string>> DesignExtractorRetriever::getNextStarRelations(
    StmtNoArgs args) {
	auto result = static_pointer_cast<QueryTimeResult>(
					nextTRelationExtractor->extract(args))
					->getResult();
	return result;
}

shared_ptr<list<vector<string>>> DesignExtractorRetriever::getAllNextStarRelations() {
	auto result = static_pointer_cast<RelationResult>(
					nextTRelationExtractor->extractAllRelations())
					->getResult();
	return result;
}


shared_ptr<vector<string>> DesignExtractorRetriever::getAffectsRelations(
    StmtNoArgs args) {
	auto result = static_pointer_cast<QueryTimeResult>(
					affectsRelationExtractor->extract(args))
					->getResult();
	return result;
}

shared_ptr<list<vector<string>>> DesignExtractorRetriever::getAllAffectsRelations() {
	auto result = static_pointer_cast<RelationResult>(
					affectsRelationExtractor->extractAllRelations())
					->getResult();
	return result;
}

shared_ptr<vector<string>> DesignExtractorRetriever::getAffectsStarRelations(
    StmtNoArgs args) {
	auto result = static_pointer_cast<QueryTimeResult>(
					affectsTRelationExtractor->extract(args))
					->getResult();
	return result;
}

shared_ptr<list<vector<string>>>
DesignExtractorRetriever::getAllAffectsStarRelations() {
	auto result = static_pointer_cast<RelationResult>(
					affectsTRelationExtractor->extractAllRelations())
					->getResult();
	return result;
}
}  // namespace DE