//
// Created by hongshu wang on 7/10/22.
//

#include "CacheManager.h"

CacheManager::CacheManager(
    shared_ptr<DesignExtractorRetriever> designExtractorRetriever) {
  this->designExtractorRetriever = designExtractorRetriever;
}

shared_ptr<Table> CacheManager::getStatementsHelper(
    shared_ptr<Cachable> cachable, vector<int> metaInfo,
    CacheContentLoader::partialGetter func) {
  int stmtNo = metaInfo[0];
  int columnNumber = metaInfo[1];
  string queriedStmt = to_string(stmtNo);
  if (cachable->areAllRelationsCached()) {
    // if all relations are cached, search for the relations queried
    return cachable->getSubTableByColumn(queriedStmt, columnNumber);
  } else {
    vector<string> range = cachable->getCachedLocation(
        columnNumber == 0 ? vector<string>{queriedStmt, Cachable::WILDCARD}
                          : vector<string>{Cachable::WILDCARD, queriedStmt});
    if (range.empty()) {
      // if cache is empty, traverse the cfg to get relations
      CacheContentLoader cacheContentLoader =
          CacheContentLoader(this->designExtractorRetriever);
      shared_ptr<vector<string>> stmts = (cacheContentLoader.*func)(stmtNo);
      int startIndex = cachable->getNumberOfRows();
      for (string s : *stmts) {
        cachable->appendRow(columnNumber == 0 ? vector<string>{queriedStmt, s}
                                              : vector<string>{s, queriedStmt});
      }
      int endIndex = cachable->getNumberOfRows();
      cachable->setQueried(
          columnNumber == 0
              ? vector<string>{queriedStmt, Cachable::WILDCARD,
                               to_string(startIndex), to_string(endIndex)}
              : vector<string>{Cachable::WILDCARD, queriedStmt,
                               to_string(startIndex), to_string(endIndex)});
      return cachable->getSubTable(startIndex, endIndex);
    } else {
      // if cache is not empty, load cache
      return cachable->getSubTable(stoi(range[0]), stoi(range[1]));
    }
  }
}

void CacheManager::getAllRelationsHelper(shared_ptr<Cachable> cachable,
                                         CacheContentLoader::fullGetter func) {
  if (!cachable->areAllRelationsCached()) {
    // clear cache before storing all the relations
    cachable->clearCache();
    cachable->dropRows();
    // store all the relations
    CacheContentLoader cacheContentLoader =
        CacheContentLoader(this->designExtractorRetriever);
    shared_ptr<list<vector<string>>> relations = (cacheContentLoader.*func)();
    cachable->appendRows(*relations);
    cachable->setAllRelationsStatus(true);
  }
}

int CacheManager::getDifference(vector<string> startAndEndIndices) {
  int startIndex = stoi(startAndEndIndices[0]);
  int endIndex = stoi(startAndEndIndices[1]);
  return endIndex - startIndex;
}

shared_ptr<Table> CacheManager::getExactRelationHelper(
    shared_ptr<Cachable> cachable, vector<int> stmts,
    CacheContentLoader::exactGetter func) {
  int firstStmt = stmts[0];
  int secondStmt = stmts[1];
  string first = to_string(firstStmt);
  string second = to_string(secondStmt);
  vector<string> row = vector<string>{first, second};
  bool result = false;
  if (cachable->areAllRelationsCached()) {
    // if all relations are cached, search for the queried relation
    result = cachable->contains(row, 0, cachable->getNumberOfRows());
  } else {
    vector<string> firstRange =
        cachable->getCachedLocation(vector<string>{first, Cachable::WILDCARD});
    vector<string> secondRange =
        cachable->getCachedLocation(vector<string>{Cachable::WILDCARD, second});
    if (firstRange.empty() && secondRange.empty()) {
      // if the relation hasn't been queried before, load the relations
      CacheContentLoader cacheContentLoader =
          CacheContentLoader(this->designExtractorRetriever);
      result = (cacheContentLoader.*func)(firstStmt, secondStmt);
    } else {
      // the relation has been queried before, search in cache
      int firstDifference =
          firstRange.empty() ? INT_MAX : getDifference(firstRange);
      int secondDifference =
          secondRange.empty() ? INT_MAX : getDifference(secondRange);
      // search in the smaller range
      if (firstDifference <= secondDifference) {
        result =
            cachable->contains(row, stoi(firstRange[0]), stoi(firstRange[1]));
      } else {
        result =
            cachable->contains(row, stoi(secondRange[0]), stoi(secondRange[1]));
      }
    }
  }
  return result ? make_shared<Table>(QE::ClauseVisitorConstants::TRUE_TABLE)
                : make_shared<Table>(QE::ClauseVisitorConstants::FALSE_TABLE);
}

shared_ptr<Table> CacheManager::getNextTTable(shared_ptr<Cachable> nextTTable) {
  getAllRelationsHelper(nextTTable, &CacheContentLoader::getNextTRelations);
  return nextTTable;
}

shared_ptr<Table> CacheManager::getNextTStatements(
    shared_ptr<Cachable> nextTTable, int stmtNo) {
  int columnNumber = 0;
  return getStatementsHelper(nextTTable, vector<int>{stmtNo, columnNumber},
                             &CacheContentLoader::getNextTStatements);
}

shared_ptr<Table> CacheManager::getPreviousTStatements(
    shared_ptr<Cachable> nextTTable, int stmtNo) {
  int columnNumber = 1;
  return getStatementsHelper(nextTTable, vector<int>{stmtNo, columnNumber},
                             &CacheContentLoader::getPreviousTStatements);
}

shared_ptr<Table> CacheManager::getNextTResult(shared_ptr<Cachable> nextTTable,
                                               int precedingStatement,
                                               int ensuingStatement) {
  return getExactRelationHelper(
      nextTTable, vector<int>{precedingStatement, ensuingStatement},
      &CacheContentLoader::getNextTResult);
}

shared_ptr<Table> CacheManager::getAffectsTable(
    shared_ptr<Cachable> affectsTable) {
  getAllRelationsHelper(affectsTable, &CacheContentLoader::getAffectsRelations);
  return affectsTable;
}

shared_ptr<Table> CacheManager::getAffectedStatements(
    shared_ptr<Cachable> affectsTable, int stmtNo) {
  int columnNumber = 0;
  return getStatementsHelper(affectsTable, vector<int>{stmtNo, columnNumber},
                             &CacheContentLoader::getAffectedStatements);
}

shared_ptr<Table> CacheManager::getAffectingStatements(
    shared_ptr<Cachable> affectsTable, int stmtNo) {
  int columnNumber = 1;
  return getStatementsHelper(affectsTable, vector<int>{stmtNo, columnNumber},
                             &CacheContentLoader::getAffectingStatements);
}

shared_ptr<Table> CacheManager::getAffectsResult(
    shared_ptr<Cachable> affectsTable, int affectingStatement,
    int affectedStatement) {
  return getExactRelationHelper(
      affectsTable, vector<int>{affectingStatement, affectedStatement},
      &CacheContentLoader::getAffectsResult);
}

shared_ptr<Table> CacheManager::getAffectsTTable(
    shared_ptr<Cachable> affectsTTable) {
  getAllRelationsHelper(affectsTTable,
                        &CacheContentLoader::getAffectsTRelations);
  return affectsTTable;
}

shared_ptr<Table> CacheManager::getAffectedTStatements(
    shared_ptr<Cachable> affectsTTable, int stmtNo) {
  int columnNumber = 0;
  return getStatementsHelper(affectsTTable, vector<int>{stmtNo, columnNumber},
                             &CacheContentLoader::getAffectedTStatements);
}

shared_ptr<Table> CacheManager::getAffectingTStatements(
    shared_ptr<Cachable> affectsTTable, int stmtNo) {
  int columnNumber = 1;
  return getStatementsHelper(affectsTTable, vector<int>{stmtNo, columnNumber},
                             &CacheContentLoader::getAffectingTStatements);
}

shared_ptr<Table> CacheManager::getAffectsTResult(
    shared_ptr<Cachable> affectsTTable, int affectingStatement,
    int affectedStatement) {
  return getExactRelationHelper(
      affectsTTable, vector<int>{affectingStatement, affectedStatement},
      &CacheContentLoader::getAffectsTResult);
}