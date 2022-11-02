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
  vector<string> getHeader() const;
  vector<vector<string>> getRows() const;
  vector<vector<string>>* getRowsPointer();
  vector<string> getRowByIndex(int index);
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
  int getNumberOfRows();
  int getNumberOfColumns();
    bool hasCol(string colName);
    bool isEqual(const Table &otherTable);
    bool isHeaderEmpty() const;
    bool isBodyEmpty() const;

  // setters
  void renameHeader(vector<string> newHeader);
  void appendRow(vector<string> row);
  void appendRows(list<vector<string>> rows);
  void setRows(vector<vector<string>> rows);
  void addValues(list<string> values);
  void dropRows();
  void insertIntoHeader(vector<string> headerToInsert);
  void dupCol(int colIdx, string dupColName = "$dup_col");
  void dropColFromThis(int colIdx);
  void removeDupRow();

  static const string DEFAULT_HEADER_PREFIX;
};

#endif  // SPA_TABLE_H
