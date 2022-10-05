//
// Created by hongshu wang on 31/8/22.
//

#include "Table.h"

vector<string> Table::getColumnByName(string columnName) {
    int numOfColumns = header.size();
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
    int numOfColumns = header.size();
    for (int i = 0; i < numOfColumns; i++) {
        if (header[i] == colName) {
            return i;
        }
    }
    return invalidColumn;
}

vector<string> Table::getColumnByIndex(int index) {
    vector<string> output;
    int size = rows.size();
    for (int i = 0; i < size; i++) {
        output.push_back(rows[i][index]);
    }
    return output;
}

vector<string> Table::getRowByPrimaryKey(string key) {
    int size = rows.size();
    for (int i = 0; i < size; i++) {
        if (rows[i][primaryKeyColumnIndex] == key) {
            return rows[i];
        }
    }
    return vector<string>{};
}

shared_ptr<unordered_set<string>> Table::getValuesByKey(string key, int keyColumnNumber, int valueColumnNumber) {
    int size = rows.size();
    shared_ptr<unordered_set<string>> output = make_shared<unordered_set<string>>(unordered_set<string>{});
    for (int i = 0; i < size; i++) {
        if (rows[i][keyColumnNumber] == key) {
            output->insert(rows[i][valueColumnNumber]);
        }
    }
    return output;
}

bool Table::hasCol(std::string colName) {
    return std::count(header.begin(), header.end(), colName);
}

void Table::appendRow(vector<string> row) {
    rows.push_back(row);
}

void Table::appendRows(list<vector<string>> rows) {
    auto rowsIterator = rows.begin();
    int stepSize = 1;
    while (rowsIterator != rows.end()) {
        appendRow(*rowsIterator);
        advance(rowsIterator, stepSize);
    }
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
    header = newHeader;
}

void Table::dropRows() {
    this->rows = {};
}

Table Table::dropCol(int colIdx) {
    if (colIdx >= this->header.size()) return *this;
    Table droppedTable = Table();
    droppedTable.header = {};
    droppedTable.rows = {};

    for (int i = 0; i < this->header.size(); ++i) {
        if (i != colIdx) {
            droppedTable.header.insert(droppedTable.header.begin(), this->header[i]);
        }
    }

    for (int i = 0; i < this->rows.size(); ++i) {
        vector<string> droppedRow;
        for (int j = 0; j < this->rows[i].size(); j++) {
            if (j != colIdx) {
                droppedRow.insert(droppedRow.begin(), this->rows[i][j]);
            }
        }
        droppedTable.appendRow(droppedRow);
    }

    return droppedTable;
}

bool Table::isEqual(const Table &otherTable) {
    if (header.size() != otherTable.header.size()) return false;
    if (rows.size() != otherTable.rows.size()) return false;
    for (int i = 0; i < otherTable.header.size(); ++i) {
        if (otherTable.header[i] != header[i]) return false;
    }
    for (int i = 0; i < otherTable.rows.size(); ++i) {
        auto expectedRow = otherTable.rows[i];
        auto testResultRow = rows[i];
        if (expectedRow.size() != testResultRow.size()) return false;
        for (int j = 0; j < expectedRow.size(); ++j) {
            if (expectedRow[j] != testResultRow[j]) return false;
        }

    }
    return true;
}

bool Table::isHeaderEmpty() const {
    return this->header.empty();
}

bool Table::isBodyEmpty() const {
    return this->rows.empty();
}

Table Table::dupCol(int colIdx, string dupColName) {
    if (colIdx >= this->header.size()) return Table();
    Table resultTable = Table();
    resultTable.header = this->header;
    resultTable.rows = this->rows;

    //append the dup col to the right of the table
    resultTable.header.push_back(dupColName);
    for (int i = 0; i < this->rows.size() ; i++) {
        resultTable.rows[i].push_back(this->rows[i][colIdx]);
    }
    return resultTable;
}
