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

vector<string> Table::getColumnByIndex(int index) {
    vector<string> output;
    int size = rows.size();
    for (int i = 0; i < size; i++) {
        output.push_back(rows[i][index]);
    }
    return output;
}

vector<string> Table::getRowByPrimaryKey(string key) {
    // assume the first column is the primary key
    int size = rows.size();
    for (int i = 0; i < size; i++) {
        if (rows[i][0] == key) {
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

void Table::appendRow(vector<string> row) {
    rows.push_back(row);
}

void Table::appendRows(list<vector<string>> rows) {
    auto rowsIterator = rows.begin();
    while (rowsIterator != rows.end()) {
        appendRow(*rowsIterator);
        advance(rowsIterator, 1);
    }
}

void Table::addValues(list<string> values) {
    // used in tables with one column only
    auto valuesIterator = values.begin();
    while (valuesIterator != values.end()) {
        vector<string> row;
        row.push_back(*valuesIterator);
        appendRow(row);
        advance(valuesIterator, 1);
    }
}

void Table::renameHeader(vector<string> newHeader) {
    header = newHeader;
}

bool Table::isEqual(Table otherTable) {
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

bool Table::isHeaderEmpty() {
    return this->header.size() == 0;
}

bool Table::isBodyEmpty() {
    return this->rows.size() == 0;
}
