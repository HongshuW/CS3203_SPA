//
// Created by hongshu wang on 31/8/22.
//

#include "Table.h"

#include <algorithm>
#include <numeric>
#include <utility>
const string Table::DEFAULT_HEADER_PREFIX = "$";

Table::Table(vector<string> header, vector<vector<string>> rows) {
  this->header = std::move(header);
  this->rows = std::move(rows);
}

Table::Table() {
  this->header = vector<string>();
  this->rows = vector<vector<string>>();
}

vector<string> Table::getHeader() const { return this->header; }

vector<vector<string>> Table::getRows() const { return this->rows; }

vector<vector<string>> *Table::getRowsPointer() { return &rows; }

const vector<vector<string>> *Table::getRowsPointerReadOnly() const {
  return &rows;
}

vector<string> Table::getColumnByName(string columnName) {
  unsigned long numOfColumns = header.size();
  for (int i = 0; i < numOfColumns; i++) {
    if (header[i] == columnName) {
      return getColumnByIndex(i);
    }
  }
  vector<string> emptyVector;
  return emptyVector;
}

int Table::getColIdxByName(string colName) {
  int invalidColumn = -1;
  unsigned long numOfColumns = header.size();
  for (int i = 0; i < numOfColumns; i++) {
    if (header[i] == colName) {
      return i;
    }
  }
  return invalidColumn;
}

vector<string> Table::getColumnByIndex(int index) {
  vector<string> output;
  unsigned long size = rows.size();
  for (int i = 0; i < size; i++) {
    output.push_back(rows[i][index]);
  }
  return output;
}

vector<string> Table::getRowByPrimaryKey(string key) {
  unsigned long size = rows.size();
  for (int i = 0; i < size; i++) {
    if (rows[i][primaryKeyColumnIndex] == key) {
      return rows[i];
    }
  }
  return vector<string>{};
}

shared_ptr<Table> Table::getSubTable(int startIndex, int endIndex) {
  shared_ptr<Table> table = make_shared<Table>(Table());
  table->header = header;
  for (int i = startIndex; i < endIndex; i++) {
    table->appendRow(rows[i]);
  }
  return table;
}

shared_ptr<Table> Table::getSubTableByColumn(const string &key,
                                             int keyColumnNumber) {
  shared_ptr<Table> table = make_shared<Table>(Table());
  table->header = header;
  for (vector<string> row : rows) {
    if (row[keyColumnNumber] == key) {
      table->appendRow(row);
    }
  }
  return table;
}

bool Table::contains(const vector<string> &row, int startIndex, int endIndex) {
  for (int i = startIndex; i < endIndex; i++) {
    if (rows[i] == row) {
      return true;
    }
  }
  return false;
}

int Table::getNumberOfRows() { return (int) rows.size(); }

int Table::getNumberOfColumns() { return header.size(); }

void Table::appendRow(const vector<string> &row) { rows.push_back(row); }

void Table::appendRows(const list<vector<string>> &newRows) {
  auto rowsIterator = newRows.begin();
  int stepSize = 1;
  while (rowsIterator != newRows.end()) {
    appendRow(*rowsIterator);
    advance(rowsIterator, stepSize);
  }
}

void Table::setRows(const vector<vector<string>> &newRows) {
  this->rows = newRows;
}

void Table::addValues(list<string> values) {
  // used in tables with one column only
  auto valuesIterator = values.begin();
  int stepSize = 1;
  while (valuesIterator != values.end()) {
    vector<string> row;
    row.push_back(*valuesIterator);
    appendRow(row);
    advance(valuesIterator, stepSize);
  }
}

void Table::renameHeader(vector<string> newHeader) {
  header = std::move(newHeader);
}

void Table::dropRows() { this->rows.clear(); }

void Table::insertIntoHeader(const vector<string> &headerToInsert) {
  this->header.insert(this->header.end(), headerToInsert.begin(),
                      headerToInsert.end());
}

bool Table::isEqual(const Table &otherTable) {
  if (header.size() != otherTable.header.size()) return false;
  if (rows.size() != otherTable.rows.size()) return false;
  // order of the headers must be the same
  for (int i = 0; i < otherTable.header.size(); ++i) {
    if (otherTable.header[i] != header[i]) return false;
  }
  unordered_set<string> r1_set = unordered_set<string>();
  unordered_set<string> r2_set = unordered_set<string>();

  auto dash_fold = [](std::string a, const string &b) {
    return std::move(a) + '-' + b;
  };
  for (int i = 0; i < otherTable.rows.size(); ++i) {
    auto &expectedRow = otherTable.rows[i];
    auto &testResultRow = rows[i];
    if (expectedRow.size() != testResultRow.size()) return false;
    r1_set.insert(std::accumulate(next(expectedRow.begin()), expectedRow.end(),
                                  expectedRow[0], dash_fold));
    r2_set.insert(std::accumulate(next(testResultRow.begin()),
                                  testResultRow.end(), testResultRow[0],
                                  dash_fold));
  }
  return r1_set == r2_set;
}

bool Table::isHeaderEmpty() const { return this->header.empty(); }

bool Table::isBodyEmpty() const { return this->rows.empty(); }

void Table::dupCol(int colIdx, const string &dupColName) {
  if (colIdx >= this->header.size()) return;
  // append the dup col to the right of the table
  header.push_back(dupColName);
  for (int i = 0; i < this->rows.size(); i++) {
    rows[i].push_back(this->rows[i][colIdx]);
  }
}

void Table::dropColFromThis(int colIdx) {
  if (colIdx >= this->header.size()) return;
  // erase header
  this->header[colIdx] = this->header[this->header.size() - 1];
  this->header.erase(this->header.end() - 1);
  auto rowItr = this->rows.begin();

  while (rowItr != this->rows.end()) {
    rowItr->at(colIdx) = rowItr->at(rowItr->size() - 1);
    rowItr->erase(rowItr->end() - 1);

    if (rowItr->empty()) {
      *rowItr = this->rows[this->rows.size() - 1];
      this->rows.erase(this->rows.end() - 1);
    } else {
      ++rowItr;
    }
  }
}

void Table::removeDupRow() {
  std::sort(this->rows.begin(), this->rows.end());
  this->rows.erase(std::unique(this->rows.begin(), this->rows.end()),
                   this->rows.end());
}
