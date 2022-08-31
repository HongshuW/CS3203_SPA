//
// Created by hongshu wang on 31/8/22.
//

#include "Table.h"
#include <vector>

using namespace std;

template <typename... Types>
Table<Types...>::Table(vector<string> header, vector<tuple<Types...>> rows) {
    this->header = header;
    this->rows = rows;
}

template <typename... Types>
tuple<Types...> Table<Types...>::getRowByIndex(int index) {
    return this->rows[index];
}

template <typename... Types>
void Table<Types...>::insertRow(tuple<Types...> row) {
    this->rows.push_back(row);
}
