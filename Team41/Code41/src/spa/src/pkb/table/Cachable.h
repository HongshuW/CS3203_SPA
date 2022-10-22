//
// Created by hongshu wang on 7/10/22.
//

#ifndef SPA_CACHABLE_H
#define SPA_CACHABLE_H

#include <string>
#include <vector>

#include "RelationTable.h"

using namespace std;

class Cachable : public RelationTable {
 public:
  static const string WILDCARD;

  bool areAllRelationsCached();
  void setAllRelationsStatus(bool status);
  vector<string> getCachedLocation(vector<string> query);
  void setQueried(vector<string> query);
  void clearCacheInformation();
  virtual void clearCache();

 private:
  bool allRelationsCached = false;
  vector<vector<string>> cacheInformation;

  int firstStmtIndex = 0;
  int secondStmtIndex = 1;
  int startIndex = 2;
  int endIndex = 3;
};

#endif  // SPA_CACHABLE_H
