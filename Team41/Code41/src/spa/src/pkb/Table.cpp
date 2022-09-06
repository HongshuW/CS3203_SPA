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

void Table::appendRow(vector<string> row) {
    rows.push_back(row);
}

void Table::renameHeader(vector<string> newHeader) {
    header = newHeader;
}

bool Table::isEqual(Table expected) {
    if (header.size() != expected.header.size()) return false;
    if (rows.size() != expected.rows.size()) return false;
    for (int i = 0; i < expected.header.size(); ++i) {
        if (expected.header[i] != header[i]) return false;
    }
    for (int i = 0; i < expected.rows.size(); ++i) {
        auto expectedRow = expected.rows[i];
        auto testResultRow = rows[i];
        if (expectedRow.size() != testResultRow.size()) return false;
        for (int j = 0; j < expectedRow.size(); ++j) {
            if (expectedRow[j] != testResultRow[j]) return false;
        }

    }
    return true;
}