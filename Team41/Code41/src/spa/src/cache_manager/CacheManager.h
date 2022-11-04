//
// Created by hongshu wang on 7/10/22.
//

#ifndef SPA_CACHEMANAGER_H
#define SPA_CACHEMANAGER_H

#include "CacheContentLoader.h"
#include "query_evaluator/constants/ClauseVisitorConstants.h"

class CacheManager {
 public:
  explicit CacheManager(
      shared_ptr<DesignExtractorRetriever> designExtractorRetriever);

  shared_ptr<Table> getNextTTable(shared_ptr<Cachable> nextTTable);
  shared_ptr<Table> getNextTStatements(shared_ptr<Cachable> nextTTable, int stmtNo);
  shared_ptr<Table> getPreviousTStatements(shared_ptr<Cachable> nextTTable, int stmtNo);
  shared_ptr<Table> getNextTResult(shared_ptr<Cachable> nextTTable, int precedingStatement, int ensuingStatement);
  shared_ptr<Table> getAffectsTable(shared_ptr<Cachable> affectsTable);
  shared_ptr<Table> getAffectedStatements(shared_ptr<Cachable> affectsTable, int stmtNo);
  shared_ptr<Table> getAffectingStatements(shared_ptr<Cachable> affectsTable, int stmtNo);
  shared_ptr<Table> getAffectsResult(shared_ptr<Cachable> affectsTable, int affectingStatement, int affectedStatement);
    shared_ptr<Table> getAffectsTTable(shared_ptr<Cachable> affectsTTable);
  shared_ptr<Table> getAffectedTStatements(shared_ptr<Cachable> affectsTTable, int stmtNo);
  shared_ptr<Table> getAffectingTStatements(shared_ptr<Cachable> affectsTTable, int stmtNo);
  shared_ptr<Table> getAffectsTResult(shared_ptr<Cachable> affectsTTable, int affectingStatement, int affectedStatement);

private:
    shared_ptr<DesignExtractorRetriever> designExtractorRetriever;

    shared_ptr<Table> getStatementsHelper(shared_ptr<Cachable> cachable, vector<int> metaInfo, CacheContentLoader::partialGetter func);
    void getAllRelationsHelper(shared_ptr<Cachable> cachable, CacheContentLoader::fullGetter func);
    int getDifference(vector<string> startAndEndIndices);
    shared_ptr<Table> getExactRelationHelper(shared_ptr<Cachable> cachable, vector<int> stmts, CacheContentLoader::exactGetter func);
};

#endif  // SPA_CACHEMANAGER_H
