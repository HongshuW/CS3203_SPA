//
// Created by hongshu wang on 7/10/22.
//

#include "Cachable.h"

const string Cachable::WILDCARD = "_";

bool Cachable::areAllRelationsCached() { return allRelationsCached; }

void Cachable::setAllRelationsStatus(bool status) {
  allRelationsCached = status;
}

/**
 * Given a query, check whether it has been cached, if so, return the range in
 * the cache table.
 *
 * @param query vector<string>{firstStatement, secondStatement}
 * @return Range in the cache table if exists.
 */
vector<string> Cachable::getCachedLocation(vector<string> query) {
  string first = query[firstStmtIndex];
  string second = query[secondStmtIndex];
  for (vector<string> row : cacheInformation) {
    if (row[firstStmtIndex] == first && row[secondStmtIndex] == second) {
      return vector<string>{row[startIndex], row[endIndex]};
    }
  }
  return vector<string>();
}

/**
 * Given a query, save it in cacheInformation.
 *
 * @param query vector<string>{firstStatement, secondStatement, startIndex,
 * endIndex}
 */
void Cachable::setQueried(vector<string> query) {
  cacheInformation.push_back(query);
}

void Cachable::clearCacheInformation() { cacheInformation.clear(); }

void Cachable::clearCache() {
  cacheInformation.clear();
  setAllRelationsStatus(false);
}
