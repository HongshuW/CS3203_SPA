//
// Created by hongshu wang on 7/10/22.
//

#include "CacheManager.h"

#include "./design_extractor/args/StmtNoArgs.h"

const int CacheManager::WILDCARD_STMTNO = 0;

CacheManager::CacheManager(shared_ptr<DesignExtractor> designExtractor) {
  this->designExtractor = designExtractor;
}

shared_ptr<list<vector<string>>> CacheManager::getNextTRelations() {
  return make_shared<list<vector<string>>>(
      designExtractor->getAllNextStarRelations());
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
  return make_shared<vector<string>>(
      designExtractor->getNextStarRelations(stmtNoArgs));
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
  return make_shared<vector<string>>(
      designExtractor->getNextStarRelations(stmtNoArgs));
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
  return !designExtractor->getNextStarRelations(stmtNoArgs).empty();
}

shared_ptr<list<vector<string>>> CacheManager::getAffectsRelations() {
  return make_shared<list<vector<string>>>(
      designExtractor->getAllAffectsRelations());
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
  return make_shared<vector<string>>(
      designExtractor->getAffectsRelations(stmtNoArgs));
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
  return make_shared<vector<string>>(
      designExtractor->getAffectsRelations(stmtNoArgs));
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
  return !designExtractor->getAffectsRelations(stmtNoArgs).empty();
}

shared_ptr<list<vector<string>>> CacheManager::getAffectsTRelations() {
  return make_shared<list<vector<string>>>(
      designExtractor->getAllAffectsStarRelations());
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
  return make_shared<vector<string>>(
      designExtractor->getAffectsStarRelations(stmtNoArgs));
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
  return make_shared<vector<string>>(
      designExtractor->getAffectsStarRelations(stmtNoArgs));
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
  return !designExtractor->getAffectsStarRelations(stmtNoArgs).empty();
}
