//
// Created by Nafour on 6/9/22.
//

#ifndef SPA_TABLECOMBINER_H
#define SPA_TABLECOMBINER_H

#include <unordered_map>

#include "pkb/table/Table.h"
namespace QE {

class TableCombiner {
  /**
   * @return pairs of duplicate headers' index in the cross product table
   */
  vector<vector<int>> findDuplicateHeadersWithOffset(vector<string> h1,
                                                     vector<string> h2);

  vector<vector<int>> findDupHeaders(vector<string> h1, vector<string> h2);

  string createFilterRowKey(const vector<string>& row,
                            const vector<int>& colsToCheck);

 public:
  Table crossProduct(const Table& t1, const Table& t2);
  Table joinTable(const Table& t1, const Table& t2);
  Table hashJoin(Table& t1, Table& t2);
};

}  // namespace QE

#endif  // SPA_TABLECOMBINER_H
