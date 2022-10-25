//
// Created by Nafour on 6/9/22.
//

#include "TableCombiner.h"

#include <unordered_set>

#include "iostream"
using namespace std;
namespace QE {
Table TableCombiner::crossProduct(const Table &t1, const Table &t2) {
  if (t1.isHeaderEmpty()) return t2;
  if (t2.isHeaderEmpty()) return t1;
  Table productTable = Table();
  productTable.header.insert(productTable.header.end(), t1.header.begin(),
                             t1.header.end());
  productTable.header.insert(productTable.header.end(), t2.header.begin(),
                             t2.header.end());
  for (auto row1 : t1.rows) {
    for (auto row2 : t2.rows) {
      auto productRow = vector<string>();
      productRow.insert(productRow.begin(), row2.begin(), row2.end());
      productRow.insert(productRow.begin(), row1.begin(), row1.end());
      productTable.rows.push_back(productRow);
    }
  }
  return productTable;
}

Table TableCombiner::joinTable(const Table &t1, const Table &t2) {
  if (t1.isHeaderEmpty()) return t2;
  if (t2.isHeaderEmpty()) return t1;
  int FIRST_DUP_IDX = 0;
  int SECOND_DUP_IDX = 1;

  Table productTable = this->crossProduct(t1, t2);
  Table joinResultTable = Table();
  auto dupHeaders = this->findDuplicateHeadersWithOffset(t1.header, t2.header);
  unordered_set<int> colInxToRemove;
  for (auto dupPair : dupHeaders) {
    colInxToRemove.insert(dupPair[SECOND_DUP_IDX]);
  }

  for (int i = 0; i < productTable.header.size(); ++i) {
    if (colInxToRemove.count(i)) continue;
    joinResultTable.header.push_back(productTable.header[i]);
  }

  for (auto row : productTable.rows) {
    bool isAdding = true;
    for (auto dupHeaderPair : dupHeaders) {
      int col1Idx = dupHeaderPair[FIRST_DUP_IDX];
      int col2Idx = dupHeaderPair[SECOND_DUP_IDX];
      if (row[col1Idx] != row[col2Idx]) isAdding = false;
    }

    if (!isAdding) continue;
    vector<string> rowToInsert = vector<string>();
    for (int i = 0; i < row.size(); ++i) {
      if (colInxToRemove.count(i)) continue;
      rowToInsert.push_back(row[i]);
    }
    joinResultTable.rows.push_back(rowToInsert);
  }
  return joinResultTable;
}

vector<vector<int>> TableCombiner::findDuplicateHeadersWithOffset(
    vector<string> h1, vector<string> h2) {
  unordered_map<string, int> h1Map;
  vector<vector<int>> ans;
  int h1Offset = h1.size();
  for (int i = 0; i < h1.size(); ++i) {
    if (h1[i].find("$") != string::npos) continue;  // skip default headers.
    h1Map.insert({h1[i], i});
  }
  for (int i = 0; i < h2.size(); ++i) {
    if (h1Map.find(h2[i]) != h1Map.end()) {
      // found dup
      ans.push_back(vector<int>{h1Map.at(h2[i]), i + h1Offset});
    }
  }
  return ans;
}

Table TableCombiner::hashJoin(Table &t1, Table &t2) {
  if (t1.isHeaderEmpty()) return t2;
  if (t2.isHeaderEmpty()) return t1;
  int FIRST_DUP_IDX = 0;
  int SECOND_DUP_IDX = 1;

  int t1Size = t1.rows.size();
  int t2Size = t2.rows.size();
  bool isT1Smaller = t1Size <= t2Size;
  Table *smallerTablePtr = isT1Smaller ? &t1 : &t2;
  Table *biggerTablePtr = isT1Smaller ? &t2 : &t1;

  auto dupHeaders =
      this->findDupHeaders(smallerTablePtr->header, biggerTablePtr->header);
  vector<int> firstColInxToCheck;
  for (auto dupPair : dupHeaders) {
    firstColInxToCheck.push_back(dupPair[FIRST_DUP_IDX]);
  }
  typedef unordered_map<string, vector<vector<string> *>> RowHashmap;
  RowHashmap map = unordered_map<string, vector<vector<string> *>>();

  for (auto &row : smallerTablePtr->rows) {
    string key = createFilterRowKey(row, firstColInxToCheck);
    if (map.find(key) == map.end()) {
      map.insert({key, vector<vector<string> *>()});
    }
    map.at(key).push_back(&row);
  }
  Table resultTable = Table();
  resultTable.header.insert(resultTable.header.end(),
                            smallerTablePtr->header.begin(),
                            smallerTablePtr->header.end());

  vector<int> secondColInxToCheck;
  for (auto dupPair : dupHeaders) {
    secondColInxToCheck.push_back(dupPair[SECOND_DUP_IDX]);
  }

  for (int i = 0; i < biggerTablePtr->header.size(); i++) {
    bool isDupHeader =
        std::find(secondColInxToCheck.begin(), secondColInxToCheck.end(), i) !=
        secondColInxToCheck.end();
    if (isDupHeader) continue;
    resultTable.header.push_back(biggerTablePtr->header[i]);
  }

  for (auto row : isT1Smaller ? t2.rows : t1.rows) {
    string key = createFilterRowKey(row, secondColInxToCheck);
    if (map.find(key) == map.end()) continue;

    for (vector<string> *rowToCopy : map.at(key)) {
      vector<string> rowToInsert = vector<string>();
      rowToInsert.insert(rowToInsert.end(), rowToCopy->begin(),
                         rowToCopy->end());
      for (int i = 0; i < row.size(); i++) {
        // do not include dup values
        if (std::find(secondColInxToCheck.begin(), secondColInxToCheck.end(),
                      i) != secondColInxToCheck.end())
          continue;
        rowToInsert.push_back(row[i]);
      }
      resultTable.rows.push_back(rowToInsert);
    }
  }

  return resultTable;
}

vector<vector<int>> TableCombiner::findDupHeaders(vector<string> h1,
                                                  vector<string> h2) {
  unordered_map<string, int> h1Map;
  vector<vector<int>> ans;
  for (int i = 0; i < h1.size(); ++i) {
    if (h1[i].find("$") != string::npos) continue;  // skip default headers.
    h1Map.insert({h1[i], i});
  }
  for (int i = 0; i < h2.size(); ++i) {
    if (h1Map.find(h2[i]) != h1Map.end()) {
      // found dup
      ans.push_back(vector<int>{h1Map.at(h2[i]), i});
    }
  }
  return ans;
}

string TableCombiner::createFilterRowKey(const vector<string> &row,
                                         const vector<int> &colsToCheck) {
  const string CONCAT_SYM = "#";
  string key = "";
  for (int idx : colsToCheck) {
    key += row[idx];
    key += CONCAT_SYM;
  }
  return key;
}
}  // namespace QE