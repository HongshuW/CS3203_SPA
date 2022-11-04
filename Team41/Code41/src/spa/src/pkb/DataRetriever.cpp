//
// Created by hongshu wang on 31/8/22.
//

#include "DataRetriever.h"

#include "./query_evaluator/constants/ClauseVisitorConstants.h"

DataRetriever::DataRetriever(shared_ptr<PKBStorage> pkbStorage) {
  this->pkbStorage = pkbStorage;
}

Table DataRetriever::getTableByDesignEntity(DesignEntity designEntity) {
  switch (designEntity) {
    case DesignEntity::PROCEDURE:
      return *pkbStorage->getProcedures();
    case DesignEntity::STMT:
      return *pkbStorage->getStatements();
    case DesignEntity::VARIABLE:
      return *pkbStorage->getVariables();
    case DesignEntity::CONSTANT:
      return *pkbStorage->getConstants();
    default:
      return Table();
  }
}

Table DataRetriever::getFollowsTable() { return *pkbStorage->getFollows(); }

Table DataRetriever::getFollowsTTable() { return *pkbStorage->getFollowsT(); }

Table DataRetriever::getParentTable() { return *pkbStorage->getParent(); }

Table DataRetriever::getParentTTable() { return *pkbStorage->getParentT(); }

Table DataRetriever::getUsesSTable() { return *pkbStorage->getUsesS(); }

Table DataRetriever::getUsesPTable() { return *pkbStorage->getUsesP(); }

Table DataRetriever::getModifiesSTable() { return *pkbStorage->getModifiesS(); }

Table DataRetriever::getModifiesPTable() { return *pkbStorage->getModifiesP(); }

Table DataRetriever::getCallsTable() { return *pkbStorage->getCalls(); }

Table DataRetriever::getCallsTTable() { return *pkbStorage->getCallsT(); }

Table DataRetriever::getNextTable() { return *pkbStorage->getNext(); }

/**
 * Get all Next* Relations.
 *
 * @return all Next* Relations in a Table.
 */
Table DataRetriever::getNextTTable() {
  shared_ptr<NextTable> nextTTable = pkbStorage->getNextT();
  shared_ptr<Table> table = cacheManager->getNextTTable(nextTTable);
  return *table;
}

/**
 * Get all the statements s such that Next*(stmtNo, s) is true.
 *
 * @param stmtNo The statement number queried
 * @return Next* relations in a table
 */
Table DataRetriever::getNextTStatements(int stmtNo) {
  shared_ptr<NextTable> nextTTable = pkbStorage->getNextT();
  int columnNumber = 0;
  shared_ptr<Table> table =
      cacheManager->getNextTStatements(nextTTable, stmtNo);
  return *table;
}

/**
 * Get all the statements s such that Next*(s, stmtNo) is true.
 *
 * @param stmtNo The statement number queried
 * @return Next* relations in a table
 */
Table DataRetriever::getPreviousTStatements(int stmtNo) {
  shared_ptr<NextTable> nextTTable = pkbStorage->getNextT();
  int columnNumber = 1;
  shared_ptr<Table> table =
      cacheManager->getPreviousTStatements(nextTTable, stmtNo);
  return *table;
}

/**
 * Check Next*(precedingStatement, ensuingStatement) and return the result in a
 * Table.
 *
 * @param precedingStatement The preceding statement queried
 * @param ensuingStatement The ensuing statement queried
 * @return whether the Next* relation exists
 */
Table DataRetriever::getNextTResult(int precedingStatement,
                                    int ensuingStatement) {
  shared_ptr<NextTable> nextTTable = pkbStorage->getNextT();
  shared_ptr<Table> table = cacheManager->getNextTResult(
      nextTTable, precedingStatement, ensuingStatement);
  return *table;
}

/**
 * Get all Affects Relations.
 *
 * @return all Affects Relations in a Table.
 */
Table DataRetriever::getAffectsTable() {
  shared_ptr<AffectsTable> affectsTable = pkbStorage->getAffects();
  shared_ptr<Table> table = cacheManager->getAffectsTable(affectsTable);
  return *table;
}

/**
 * Get all Affects* Relations.
 *
 * @return all Affects* Relations in a Table.
 */
Table DataRetriever::getAffectsTTable() {
  shared_ptr<AffectsTable> affectsTTable = pkbStorage->getAffectsT();
  shared_ptr<Table> table = cacheManager->getAffectsTTable(affectsTTable);
  return *table;
}

/**
 * Get all statements s such that Affects(stmtNo, s) is true.
 *
 * @param stmtNo The statement number queried
 * @return Affects relations in a table
 */
Table DataRetriever::getAffectedStatements(int stmtNo) {
  shared_ptr<AffectsTable> affectsTable = pkbStorage->getAffects();
  shared_ptr<Table> table =
      cacheManager->getAffectedStatements(affectsTable, stmtNo);
  return *table;
}

/**
 * Get all statement s such that Affects(s, stmtNo) is true.
 *
 * @param stmtNo The statement number queried
 * @return Affects relations in a table
 */
Table DataRetriever::getAffectingStatements(int stmtNo) {
  shared_ptr<AffectsTable> affectsTable = pkbStorage->getAffects();
  shared_ptr<Table> table =
      cacheManager->getAffectingStatements(affectsTable, stmtNo);
  return *table;
}

/**
 * Check Affects(affectingStatement, affectedStatement) and return the result in
 * a Table.
 *
 * @param affectingStatement The affecting statement queried
 * @param affectedStatement The affected statement queried
 * @return whether the Affects relation exists
 */
Table DataRetriever::getAffectsResult(int affectingStatement,
                                      int affectedStatement) {
  shared_ptr<AffectsTable> affectsTable = pkbStorage->getAffects();
  shared_ptr<Table> table = cacheManager->getAffectsResult(
      affectsTable, affectingStatement, affectedStatement);
  return *table;
}

/**
 * Get all statements s such that Affects*(stmtNo, s) is true.
 *
 * @param stmtNo The statement number queried
 * @return Affects* relations in a table
 */
Table DataRetriever::getAffectedTStatements(int stmtNo) {
  shared_ptr<AffectsTable> affectsTTable = pkbStorage->getAffectsT();
  shared_ptr<Table> table =
      cacheManager->getAffectedTStatements(affectsTTable, stmtNo);
  return *table;
}

/**
 * Get all statement s such that Affects*(s, stmtNo) is true.
 *
 * @param stmtNo The statement number queried
 * @return Affects* relations in a table
 */
Table DataRetriever::getAffectingTStatements(int stmtNo) {
  shared_ptr<AffectsTable> affectsTTable = pkbStorage->getAffectsT();
  shared_ptr<Table> table =
      cacheManager->getAffectingTStatements(affectsTTable, stmtNo);
  return *table;
}

/**
 * Check Affects*(affectingStatement, affectedStatement) and return the result
 * in a Table.
 *
 * @param affectingStatement The affecting statement queried
 * @param affectedStatement The affected statement queried
 * @return whether the Affects* relation exists
 */
Table DataRetriever::getAffectsTResult(int affectingStatement,
                                       int affectedStatement) {
  shared_ptr<AffectsTable> affectsTTable = pkbStorage->getAffectsT();
  shared_ptr<Table> table = cacheManager->getAffectsTResult(
      affectsTTable, affectingStatement, affectedStatement);
  return *table;
}

Table DataRetriever::getAssignPatternTable(ExpressionSpec expressionSpec) {
  return *pkbStorage->getMatchedAssignPatterns(expressionSpec);
}

Table DataRetriever::getIfPatternTable() {
  return *pkbStorage->getIfPatterns();
}

Table DataRetriever::getWhilePatternTable() {
  return *pkbStorage->getWhilePatterns();
}

Table DataRetriever::getCallsProcedureNames() {
  return *pkbStorage->getCallsProcedureNames();
}

Table DataRetriever::getPrintVariableNames() {
  return *pkbStorage->getPrintVariableNames();
}

Table DataRetriever::getReadVariableNames() {
  return *pkbStorage->getReadVariableNames();
}

DesignEntity DataRetriever::getDesignEntityOfStmt(int stmtNumber) {
  string stmtNumberString = to_string(stmtNumber);
  string type = pkbStorage->getStmtType(stmtNumberString);
  return getDesignEntity(type);
}

void DataRetriever::clearCache() { pkbStorage->clearCache(); }
