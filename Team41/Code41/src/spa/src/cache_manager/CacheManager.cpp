//
// Created by hongshu wang on 7/10/22.
//

#include "CacheManager.h"

const int CacheManager::WILDCARD_STMTNO = 0;

CacheManager::CacheManager(shared_ptr<DesignExtractorRetriever> designExtractorRetriever) {
  this->designExtractorRetriever = designExtractorRetriever;
}

shared_ptr<list<vector<string>>> CacheManager::getNextTRelations() {
  return designExtractorRetriever->getAllNextStarRelations();
}

/**
 * Get all the statements s such that Next*(stmtNo, s) is true.
 *
 * @param stmtNo The statement number queried
 * @return Next* relations
 */
shared_ptr<vector<string>> CacheManager::getNextTStatements(int stmtNo) {
  StmtNoArgs stmtNoArgs = StmtNoArgs();
  stmtNoArgs.setStartAndEndStmtNo(stmtNo, WILDCARD_STMTNO);
  return designExtractorRetriever->getNextStarRelations(stmtNoArgs);
}

/**
 * Get all the statements s such that Next*(s, stmtNo) is true.
 *
 * @param stmtNo The statement number queried
 * @return Next* relations
 */
shared_ptr<vector<string>> CacheManager::getPreviousTStatements(int stmtNo) {
  StmtNoArgs stmtNoArgs = StmtNoArgs();
  stmtNoArgs.setStartAndEndStmtNo(WILDCARD_STMTNO, stmtNo);
  return designExtractorRetriever->getNextStarRelations(stmtNoArgs);
}

/**
 * Check Next*(precedingStatement, ensuingStatement).
 *
 * @param precedingStatement The preceding statement queried
 * @param ensuingStatement The ensuing statement queried
 * @return whether the Next* relation exists
 */
bool CacheManager::getNextTResult(int precedingStatement,
                                  int ensuingStatement) {
  StmtNoArgs stmtNoArgs = StmtNoArgs();
  stmtNoArgs.setStartAndEndStmtNo(precedingStatement, ensuingStatement);
  return !designExtractorRetriever->getNextStarRelations(stmtNoArgs)->empty();
}

shared_ptr<list<vector<string>>> CacheManager::getAffectsRelations() {
  return designExtractorRetriever->getAllAffectsRelations();
}

/**
 * Get all statements s such that Affects(stmtNo, s) is true.
 *
 * @param stmtNo The statement number queried
 * @return Affects relations
 */
shared_ptr<vector<string>> CacheManager::getAffectedStatements(int stmtNo) {
  StmtNoArgs stmtNoArgs = StmtNoArgs();
  stmtNoArgs.setStartAndEndStmtNo(stmtNo, WILDCARD_STMTNO);
  return designExtractorRetriever->getAffectsRelations(stmtNoArgs);
}

/**
 * Get all statement s such that Affects(s, stmtNo) is true.
 *
 * @param stmtNo The statement number queried
 * @return Affects relations
 */
shared_ptr<vector<string>> CacheManager::getAffectingStatements(int stmtNo) {
  StmtNoArgs stmtNoArgs = StmtNoArgs();
  stmtNoArgs.setStartAndEndStmtNo(WILDCARD_STMTNO, stmtNo);
  return designExtractorRetriever->getAffectsRelations(stmtNoArgs);
}

/**
 * Check Affects(affectingStatement, affectedStatement).
 *
 * @param affectingStatement The affecting statement queried
 * @param affectedStatement The affected statement queried
 * @return whether the Affects relation exists
 */
bool CacheManager::getAffectsResult(int affectingStatement,
                                    int affectedStatement) {
  StmtNoArgs stmtNoArgs = StmtNoArgs();
  stmtNoArgs.setStartAndEndStmtNo(affectingStatement, affectedStatement);
  return !designExtractorRetriever->getAffectsRelations(stmtNoArgs)->empty();
}

shared_ptr<list<vector<string>>> CacheManager::getAffectsTRelations() {
  return designExtractorRetriever->getAllAffectsStarRelations();
}

/**
 * Get all statements s such that Affects*(stmtNo, s) is true.
 *
 * @param stmtNo The statement number queried
 * @return Affects relations
 */
shared_ptr<vector<string>> CacheManager::getAffectedTStatements(int stmtNo) {
  StmtNoArgs stmtNoArgs = StmtNoArgs();
  stmtNoArgs.setStartAndEndStmtNo(stmtNo, WILDCARD_STMTNO);
  return designExtractorRetriever->getAffectsStarRelations(stmtNoArgs);
}

/**
 * Get all statement s such that Affects*(s, stmtNo) is true.
 *
 * @param stmtNo The statement number queried
 * @return Affects relations
 */
shared_ptr<vector<string>> CacheManager::getAffectingTStatements(int stmtNo) {
  StmtNoArgs stmtNoArgs = StmtNoArgs();
  stmtNoArgs.setStartAndEndStmtNo(WILDCARD_STMTNO, stmtNo);
  return designExtractorRetriever->getAffectsStarRelations(stmtNoArgs);
}

/**
 * Check Affects*(affectingStatement, affectedStatement).
 *
 * @param affectingStatement The affecting statement queried
 * @param affectedStatement The affected statement queried
 * @return whether the Affects* relation exists
 */
bool CacheManager::getAffectsTResult(int affectingStatement,
                                     int affectedStatement) {
  StmtNoArgs stmtNoArgs = StmtNoArgs();
  stmtNoArgs.setStartAndEndStmtNo(affectingStatement, affectedStatement);
  return !designExtractorRetriever->getAffectsStarRelations(stmtNoArgs)->empty();
}
