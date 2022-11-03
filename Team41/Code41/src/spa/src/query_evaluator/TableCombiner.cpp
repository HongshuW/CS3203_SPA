//
// Created by Nafour on 6/9/22.
//

#include "TableCombiner.h"

#include <unordered_set>

#include "iostream"
using namespace std;
namespace QE {

const string CONCAT_SYM = "@";

shared_ptr<Table> TableCombiner::crossProduct(shared_ptr<Table> t1,
                                              shared_ptr<Table> t2) {
  if (t1->isHeaderEmpty()) return t2;
  if (t2->isHeaderEmpty()) return t1;
  auto productTable = make_shared<Table>();
  productTable->insertIntoHeader(t1->getHeader());
  productTable->insertIntoHeader(t2->getHeader());

  for (auto &row1 : *t1->getRowsPointerReadOnly()) {
    for (auto &row2 : *t2->getRowsPointerReadOnly()) {
      auto productRow = vector<string>();
      productRow.insert(productRow.begin(), row2.begin(), row2.end());
      productRow.insert(productRow.begin(), row1.begin(), row1.end());
      productTable->appendRow(productRow);
    }
  }
  return productTable;
}

shared_ptr<Table> TableCombiner::hashJoin(shared_ptr<Table> t1,
                                          shared_ptr<Table> t2) {
  if (t1->isHeaderEmpty()) return t2;
  if (t2->isHeaderEmpty()) return t1;
  int FIRST_DUP_IDX = 0;
  int SECOND_DUP_IDX = 1;

  int t1Size = t1->getNumberOfRows();
  int t2Size = t2->getNumberOfRows();
  bool isT1Smaller = t1Size <= t2Size;
  auto smallerTablePtr = isT1Smaller ? t1 : t2;
  auto biggerTablePtr = isT1Smaller ? t2 : t1;

  auto dupHeaders = this->findDupHeaders(smallerTablePtr->getHeader(),
                                         biggerTablePtr->getHeader());
  vector<int> firstColInxToCheck;
  for (auto dupPair : dupHeaders) {
    firstColInxToCheck.push_back(dupPair[FIRST_DUP_IDX]);
  }
  typedef unordered_map<string, vector<vector<string> *>> RowHashmap;
  RowHashmap map = unordered_map<string, vector<vector<string> *>>();

  vector<vector<string>> *smallerTableRows = smallerTablePtr->getRowsPointer();
  for (auto &row : *smallerTableRows) {
    string key = createFilterRowKey(row, firstColInxToCheck);
    if (map.find(key) == map.end()) {
      map.insert({key, vector<vector<string> *>()});
    }
    map.at(key).push_back(&row);
  }
  auto resultTable = make_shared<Table>();
  resultTable->insertIntoHeader(smallerTablePtr->getHeader());

  vector<int> secondColInxToCheck;
  for (auto dupPair : dupHeaders) {
    secondColInxToCheck.push_back(dupPair[SECOND_DUP_IDX]);
  }

  auto biggerTableHeaders = biggerTablePtr->getHeader();
  for (int i = 0; i < biggerTablePtr->getNumberOfColumns(); i++) {
    bool isDupHeader =
        std::find(secondColInxToCheck.begin(), secondColInxToCheck.end(), i) !=
        secondColInxToCheck.end();
    if (isDupHeader) continue;
    resultTable->insertIntoHeader(vector<string>{biggerTableHeaders[i]});
  }

  for (auto &row : *biggerTablePtr->getRowsPointer()) {
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
      resultTable->appendRow(rowToInsert);
    }
  }

  return resultTable;
}

vector<vector<int>> TableCombiner::findDupHeaders(const vector<string> &h1,
                                                  const vector<string> &h2) {
  unordered_map<string, int> h1Map;
  vector<vector<int>> ans;
  for (int i = 0; i < h1.size(); ++i) {
    if (h1[i].find(Table::DEFAULT_HEADER_PREFIX) != string::npos)
      continue;  // skip default headers.
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
  string key = "";
  for (int idx : colsToCheck) {
    key += row[idx];
    key += CONCAT_SYM;
  }
  return key;
}
}  // namespace QE