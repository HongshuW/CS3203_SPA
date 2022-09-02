//
// Created by hongshu wang on 31/8/22.
//

#include "Table.h"

list<string> Table::getColumnByName(string columnName) {
    int numOfColumns = header.size();
    for (int i = 0; i < numOfColumns; i++) {
        if (header[i] == columnName) {
            return getColumnByIndex(i);
        }
    }
    return getColumnByIndex(0);
}

list<string> Table::getColumnByIndex(int index) {
    list<string> output;
    auto rowsIterator = rows.begin();
    while (rowsIterator != rows.end()) {
        output.push_back((*rowsIterator)[index]);
        advance(rowsIterator, 1);
    }
    return output;
}