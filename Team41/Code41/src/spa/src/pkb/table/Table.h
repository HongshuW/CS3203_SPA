//
// Created by hongshu wang on 31/8/22.
//

#ifndef SPA_TABLE_H
#define SPA_TABLE_H

#include <list>
#include <memory>
#include <string>
#include <unordered_set>
#include <vector>

using namespace std;

class Table {
 public:
  vector<string> header;
  vector<vector<string>> rows;
  int primaryKeyColumnIndex = 0;

  // getters
  vector<string> getColumnByName(string columnName);
  vector<string> getColumnByIndex(int index);
  vector<string> getRowByPrimaryKey(string key);
  shared_ptr<unordered_set<string>> getValuesByKey(string key,
                                                   int keyColumnNumber = 0,
                                                   int valueColumnNumber = 1);
  int getColIdxByName(string colName);
  shared_ptr<Table> getSubTable(int startIndex, int endIndex);
  shared_ptr<Table> getSubTableByColumn(string key, int keyColumnNumber);
  bool contains(vector<string> row, int startIndex, int endIndex);

  // setters
  void renameHeader(vector<string> newHeader);
  void appendRow(vector<string> row);
  void appendRows(list<vector<string>> rows);
  void addValues(list<string> values);
  void dropRows();

  Table dropCol(int colIdx);
  Table dupCol(int colIdx, string dupColName = "$dup_col");

  bool hasCol(string colName);
  bool isEqual(const Table &otherTable);
  bool isHeaderEmpty() const;
  bool isBodyEmpty() const;

  static const string DEFAULT_HEADER_PREFIX;
};

#endif  // SPA_TABLE_H
