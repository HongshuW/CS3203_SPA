//
// Created by hongshu wang on 7/10/22.
//

#ifndef SPA_CACHEMANAGER_H
#define SPA_CACHEMANAGER_H

#include "./design_extractor/DesignExtractor.h"

using namespace DE;

class CacheManager {
 public:
  typedef shared_ptr<vector<string>> (CacheManager::*partialGetter)(int);
  typedef shared_ptr<list<vector<string>>> (CacheManager::*fullGetter)();
  typedef bool (CacheManager::*exactGetter)(int, int);

  static const int WILDCARD_STMTNO;
  shared_ptr<DesignExtractor> designExtractor;

  explicit CacheManager(shared_ptr<DesignExtractor> designExtractor);

  shared_ptr<list<vector<string>>> getNextTRelations();
  shared_ptr<vector<string>> getNextTStatements(int stmtNo);
  shared_ptr<vector<string>> getPreviousTStatements(int stmtNo);
  bool getNextTResult(int precedingStatement, int ensuingStatement);
  shared_ptr<list<vector<string>>> getAffectsRelations();
  shared_ptr<vector<string>> getAffectedStatements(int stmtNo);
  shared_ptr<vector<string>> getAffectingStatements(int stmtNo);
  bool getAffectsResult(int affectingStatement, int affectedStatement);
  shared_ptr<list<vector<string>>> getAffectsTRelations();
  shared_ptr<vector<string>> getAffectedTStatements(int stmtNo);
  shared_ptr<vector<string>> getAffectingTStatements(int stmtNo);
  bool getAffectsTResult(int affectingStatement, int affectedStatement);
};

#endif  // SPA_CACHEMANAGER_H
