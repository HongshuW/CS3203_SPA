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
 private:
  vector<string> header;
  vector<vector<string>> rows;

 public:
  int primaryKeyColumnIndex = 0;

  explicit Table(vector<string> header, vector<vector<string>> rows);
  explicit Table();

  // getters
  [[nodiscard]] vector<string> getHeader() const;
  [[nodiscard]] vector<vector<string>> getRows() const;
  vector<vector<string>> *getRowsPointer();
  [[nodiscard]] const vector<vector<string>> *getRowsPointerReadOnly() const;
  vector<string> getColumnByName(const string& columnName);
  vector<string> getColumnByIndex(int index);
  vector<string> getRowByPrimaryKey(const string& key);
  shared_ptr<unordered_set<string>> getValuesByKey(const string& key,
                                                   int keyColumnNumber = 0,
                                                   int valueColumnNumber = 1);
  int getColIdxByName(const string& colName);
  shared_ptr<Table> getSubTable(int startIndex, int endIndex);
  shared_ptr<Table> getSubTableByColumn(const string& key, int keyColumnNumber);
  bool contains(const vector<string>& row, int startIndex, int endIndex);
  int getNumberOfRows();
  int getNumberOfColumns();
  bool isEqual(const Table &otherTable);
  [[nodiscard]] bool isHeaderEmpty() const;
  [[nodiscard]] bool isBodyEmpty() const;

  // setters
  void renameHeader(vector<string> newHeader);
  void appendRow(const vector<string> &row);
  void appendRows(const list<vector<string>> &newRows);
  void setRows(const vector<vector<string>> &newRows);
  void addValues(list<string> values);
  void dropRows();
  void insertIntoHeader(const vector<string> &headerToInsert);
  void dupCol(int colIdx, const string& dupColName = "$dup_col");
  void dropColFromThis(int colIdx);
  void removeDupRow();

  static const string DEFAULT_HEADER_PREFIX;
};

#endif  // SPA_TABLE_H
